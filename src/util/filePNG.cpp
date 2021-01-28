#if defined _WIN32 && !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include "util/typeShortcuts.h"
#include "util/str8.h"
#include "util/mzPacker.h"
#include "util/imgClass.h"
#include "util/mlib.hpp"


/* TODO:
- errors could be bit encoded, so any number of errors could be recorded in err var
- MUST test 16 bpc images, see if byteSwps conversion is needed
*/

// 0 - OK, no errors
// 1 - cannot open file
// 2 - file read error
// 3 - file write error
// 4 - image size error
// 5 - cmap is empty / cmap expected - not found
// 6 - alloc error
// 7 - invalid BPP
// 8 - cmap palette not suported
// 9 - unknown / unsupported image type
// 10 - PNG header not read first
// 11 - IHDR chunk error
// 12 - PLTE chunk error
// 13 - IDAT chunk error
// 14 - decompression error
// 15 - raw data not filled
// 16 - PNG does not support this image format
// 17 - compression error



using namespace Str;

///==================================================================///
// PNG image file =======================---------------------------- //
///==================================================================///

// Img PNG private
class _ImgPNG {
  /// PNG uses big endian byte order(network order), iverse of what c/ c++ uses, therefore inverse all them bytes
  inline static uint32 byteSwp32(uint32 n) {
    uint8 *c= (uint8 *)&n;
    return (c[0]<< 24)| (c[1]<< 16)| (c[2]<< 8)| c[3];
  }

  /// PNG uses big endian byte order(network order), iverse of what c/ c++ uses, therefore inverse all them bytes
  inline static uint16 byteSwp16(uint16 n) {
    uint8 *c= (uint8 *)&n;
    return (c[0]<< 8)| c[1];
  }


  // these don't work... maybe make them work someday...

  //#define byteSwp32(n) (((uint8 *)&(n))[0]<< 24)| (((uint8 *)&(n))[1]<< 16)| (((uint8 *)&(n))[2]<< 8)| ((uint8 *)&(n))[3]
  //#define byteSwp16(n) (((uint8 *)&(n))[0]<< 8)| ((int8 *)&(n))[1]

  struct Adam7 {
    int8 x0, y0, xnext, ynext;
  };

  static const Adam7 adam7[7];

  struct IDATdata {
    uint8 *bitmap;     // bitmap that will hold the PNG
    uint8 *p;          // current position on bitmap
    uint8 bpp;         // bitmap bits per pixel
    int32 xpos, ypos;  // current X & Y coordinate position (pixel coords)
    int8 fline;        // current pixel line filter type
    int32 lineSize;    // linesize in bytes of the bitmap
    uint8 mask;        // a mask used for less than 8bpp images
    int8 ilevel;       // current Adam7 level
    int8 depth;        // pixel depth used only for 8bpp and more
    int8 pixByte;      // byte number in current pixel
    int8 nbit;         // bit number in current byte
    uint8 *l1, *l2;    // buffers that will hold 2 scanlines, used only for interlaced PNG's to help the filter ... filter
    uint8 *linePrev, *lineCur, *linePos;  /// used with l1 and l2
    IDATdata() { bitmap= p= null; bpp= 0; xpos= ypos= 0; fline= 0; lineSize= 0; mask= 0; ilevel= 0; depth= 0; pixByte= nbit= 0; l1= l2= linePrev= lineCur= linePos= null; }
    ~IDATdata() { if(l1) delete[] l1; if(l2) delete[] l2; }
  };



  inline uint8 static _filter(uint8 interlace, IDATdata *d) {
    uint32 xprev= 0, yprev= 0, xyprev= 0;

    // 8bpp and over PNGs
    if(d->bpp>= 8) {
      if(!interlace) {
        xprev=   (d->xpos?             *(d->p- d->depth):              0);
        yprev=   (d->ypos?             *(d->p- d->lineSize):           0);
        xyprev= ((d->ypos && d->xpos)? *(d->p- d->lineSize- d->depth): 0);
      } else {
        bool xOK= ((d->xpos- adam7[d->ilevel].xnext>= 0) ? true: false);
        bool yOK= ((d->ypos- adam7[d->ilevel].ynext>= 0) ? true: false);
        xprev=  (xOK?         *(d->p- adam7[d->ilevel].xnext* d->depth):                                      0);
        yprev=  (yOK?         *(d->p- adam7[d->ilevel].ynext* d->lineSize):                                   0);
        xyprev=((xOK && yOK)? *(d->p- adam7[d->ilevel].xnext* d->depth- adam7[d->ilevel].ynext* d->lineSize): 0);
      }

    // less than 8bpp PNGs - tons of computations
    } else {
      if(!interlace) {
        bool xOK= ((d->xpos/ d->bpp> 0)? true: false);
        bool yOK= ((d->ypos/ d->bpp> 0)? true: false);
        xprev=   (xOK?         *(d->p- 1):              0);
        yprev=   (yOK?         *(d->p- d->lineSize):    0);
        xyprev= ((xOK && yOK)? *(d->p- d->lineSize- 1): 0);
      } else {
        bool xOK= (((d->xpos- adam7[d->ilevel].xnext)>= 0) ? true: false);
        bool yOK= (((d->ypos- adam7[d->ilevel].ynext)>= 0) ? true: false);
        xprev=   (xOK?         *(d->linePos- 1):                            0);
        yprev=   (yOK?         *(d->linePrev+ (d->linePos- d->lineCur)):    0);
        xyprev= ((xOK && yOK)? *(d->linePrev+ (d->linePos- d->lineCur- 1)): 0);
      }
    }


    if(!d->fline)                     /// no filter
      return 0;
  
    else if(d->fline== 1)             /// 'Sub' filter
      return (uint8)xprev;
  
    else if(d->fline== 2)             /// 'Up' filter
      return (uint8)yprev; 

    else if(d->fline== 3)             /// 'Average' filter
      return (uint8)((xprev+ yprev)/ 2);

    else if(d->fline== 4) {           /// 'Paeth' filter
      int16 pa= mlib::abs32((int32)yprev- (int32)xyprev);
      int16 pb= mlib::abs32((int32)xprev- (int32)xyprev);
      int16 pc= mlib::abs32((int32)xprev+ (int32)yprev- (int32)xyprev- (int32)xyprev);

      if(pc < pa && pc < pb) return (uint8)xyprev;
      else if(pb < pa) return (uint8)yprev;
      else return (uint8)xprev;
    } /// filter types
    return 0;
  }


  inline bool static _findGoodXY(int8 &ilevel, int32 &xpos, int32 &ypos, const uint32 dx, const uint32 dy) {
    CheckAgain:
    if(xpos>= (int32)dx) {
      xpos= _ImgPNG::adam7[ilevel].x0;
      ypos+= adam7[ilevel].ynext;
      if(xpos>= (int32)dx) {
        ilevel++;
        if(ilevel> 6) return false;
        xpos= adam7[ilevel].x0;
        ypos= adam7[ilevel].y0;
        goto CheckAgain;
      }
    }

    if(ypos>= (int32)dy) {
      ilevel++;
      if(ilevel> 6) return false;
      xpos= adam7[ilevel].x0;
      ypos= adam7[ilevel].y0;
      goto CheckAgain;
    }
    return true;
  }

  friend class Img;
}; // Img PNG private


const _ImgPNG::Adam7 _ImgPNG::adam7[]= { {0, 0, 8, 8},   /// lev 0
                                         {4, 0, 8, 8},   /// lev 1
                                         {0, 4, 4, 8},   /// lev 2
                                         {2, 0, 4, 4},   /// lev 3
                                         {0, 2, 2, 4},   /// lev 4
                                         {1, 0, 2, 2},   /// lev 5
                                         {0, 1, 1, 2} }; /// lev 6

























// PNG load func ====================------------------------
///==========================================================


#define SFREAD(a, b, c, d) if(fread(a, b, c, d)!= (c)) { i->err= 9; goto ReadError; }

bool Img::_loadPNG(const char *fname, Img *i) {
  i->delData();
  i->err= 0;

  char PNGfileCheck[9];     /// PNG file stamp
  bool tRNSloaded= false;   /// security checks
    /// header vars / chunk vars
  int32 clength= 0;         /// chunk length
  char cname[5];            /// chunk name (4 bytes)
  cname[4]= 0;              /// string terminator
  uint32 cCRC;              /// chunk CRC
  uint8 colType;            /// PNG color type
  uint8 compression;        /// compression type (only 1 i think there is)
  uint8 filter;             /// filter type (only 1 in specs i think)
  uint8 interlace;          /// interlace type (only on or off i think)
  uint8 bpc;                /// bits per channel;
  
  /// bitmap
  //int64 n= 0;             /// n will be the number of bytes processed (no bigger than bitmapSize + dy)
  uint8 *bitmap= null;      /// image raw data, i->bitmap or i->wrapbitmap will point to it in return
  uint64 bitmapSize= 0;     /// bitmap raw img data size in bytes
  bool bitmapFilled= false; /// if the bitmap data was all filled, this flag will be true
  /// decompressor
  mzPacker pack;
  int64 size;               /// size of the decompressed data
  uint8 *p1;                /// p1 will walk the decompressed data
  /// filter tmp variables
  //uint8 *pprev= null;

  _ImgPNG::IDATdata d;


  // open file
  i->fileName= fname;
  i->fileType= 2;

  FILE *f= fopen(fname, "rb");
  if(!f) { i->err= 8; return false; }

  /// read PNG file check
  SFREAD(PNGfileCheck, 8, 1, f)
  PNGfileCheck[8]= 0;     /// string terminator

  /// make shure it's a PNG image
  if(strcmp8("\x89PNG\xd\xa\x1a\xa", PNGfileCheck)) {
    i->err= 14; goto ReadError; }


  /// pass thru all PNG chunks, until end chunk
  while(1) {

    /// read chunk length
    SFREAD(&clength, 4, 1, f) clength= _ImgPNG::byteSwp32(clength);
    SFREAD(cname, 4, 1, f)

    // IHDR chunk, first, PNG header
    ///=============================
    if(!strcmp8(cname, "IHDR")&& !i->dx) {   /// !dx - safety check: only 1 IHDR chunk must be present
      SFREAD(&i->dx,       4, 1, f) i->dx= _ImgPNG::byteSwp32(i->dx);
      SFREAD(&i->dy,       4, 1, f) i->dy= _ImgPNG::byteSwp32(i->dy);
      SFREAD(&bpc,         1, 1, f)
      SFREAD(&colType,     1, 1, f)
      SFREAD(&compression, 1, 1, f)
      SFREAD(&filter,      1, 1, f)
      SFREAD(&interlace,   1, 1, f)

      /// abort checks
      if((!i->dx)|| (!i->dy))   { i->err= 2; goto ReadError; }
      if(i->dx> 32768 || i->dy> 32768) { i->err= 2; goto ReadError; }
      if(filter!= 0)            { i->err= 14; goto ReadError; }
      if(interlace> 1)          { i->err= 14; goto ReadError; }
      if(compression!= 0)       { i->err= 14; goto ReadError; }

      // PNG image type

      /// PNG type 0 - greyscale image
      if(colType== 0) {
        if     (bpc== 1)  i->_set(ImgFormat::R8_UNORM); // IMG_GREY1
        else if(bpc== 2)  i->_set(ImgFormat::R8_UNORM); // IMG_GREY2
        else if(bpc== 4)  i->_set(ImgFormat::R8_UNORM);  // IMG_GREY4
        else if(bpc== 8)  i->_set(ImgFormat::R8_UNORM);  // IMG_GREY
        else if(bpc== 16) i->_set(ImgFormat::R16_UNORM); // IMG_GREY16
        else { i->err= 3; goto ReadError; }

      /// PNG type 2 - RBG image
      } else if(colType== 2) {
        if     (bpc== 8)  i->_set(ImgFormat::R8G8B8_UNORM);
        else if(bpc== 16) i->_set(ImgFormat::R16G16B16_UNORM);
        else { i->err= 3; goto ReadError; }

      /// PNG type 3 - cmap image
      } else if(colType== 3) {
        if     (bpc== 1) i->_set(ImgFormat::CMAP_RGB); /// 2 max cmap colors, to fit in 1 bit
        else if(bpc== 2) i->_set(ImgFormat::CMAP_RGB); /// 4 max cmap colors, to fit in 2 bits
        else if(bpc== 4) i->_set(ImgFormat::CMAP_RGB); /// 16 max cmap colors, to fit in 4 bits
        else if(bpc== 8) i->_set(ImgFormat::CMAP_RGB); /// 256 max cmap colors, to fit in 8 bits

      /// PNG type 4 - grey + alpha image
      } else if(colType== 4) {
        if     (bpc== 8)  i->_set(ImgFormat::R8G8_UNORM);
        else if(bpc== 16) i->_set(ImgFormat::R16G16_UNORM);
        else { i->err= 3; goto ReadError; }

      /// PNG type 6 - RGBA image
      } else if(colType== 6) {
        if     (bpc== 8)  i->_set(ImgFormat::R8G8B8A8_UNORM);
        else if(bpc== 16) i->_set(ImgFormat::R16G16B16A16_UNORM);
        else { i->err= 3; goto ReadError; }

      /// unknown image type
      } else { i->err= 14;  goto ReadError; }

      /// raw image buffer size; if it doesn't fit perfectly in full bytes, add 1 byte at the end of each line
      bitmapSize= (((i->dx* i->bpp)% 8)? ((i->dx* i->bpp)/ 8+ 1)* i->dy: ((i->dx* i->bpp)/ 8)* i->dy);
      //if(i->format== ImgFormat::CMAP_RGB) bitmapSize= i->dx* i->dy;
      //else bitmapSize= (i->dx* i->nchannels)* i->dy;

    // PLTE chunk - cmap palette
    ///=========================
    } else if(!strcmp8(cname, "PLTE")&& !i->cmap) {
      if(!i->dx) { i->err= 17; goto ReadError; }       /// if IHDR is not first, nothing is initialized
      if(clength> 768) { i->err= 19; goto ReadError; } /// chunk length bigger than maximum cmap size
      if(i->cmap!= null) { i->err= 19; goto ReadError; } /// more than 1 PLTE chunk

      i->cmap= new uint8[768];           /// specs say there should be only one PLTE chunk... if cmap is already allocated...
      SFREAD(i->cmap, clength, 1, f)
      //i->cmapBpp= 24;

    // IDAT chunk - PNG compressed data chunk
    ///======================================
    } else if(!strcmp8(cname, "IDAT")) {
      if(!bitmapSize) { i->err= 17; goto ReadError; }  /// if IHDR is not first, nothing is initialized
      if(bitmapFilled) { i->err= 20; goto ReadError; } /// image raw data aquired, yet another IDAT chunk found...
      

      d.bpp= bpc* i->nchannels;
      d.lineSize= (uint32)(bitmapSize/ i->dy);  /// bitmap linesize in bytes
      d.depth= ((d.bpp/ 8)== 0? 1: (d.bpp/ 8)); /// pixel depth in bytes, only used for 8bpp and more
      d.mask= 0xFF>> (8- d.bpp);                /// mask of how many bits in the byte are used for 1 pixel for less than 8bpp images
      
      bool useLineBuffers= (((d.bpp< 8) && interlace)? true: false);

      /// alloc the bitmap
      if(!bitmap) {
        bitmap= new uint8[(unsigned int)bitmapSize];
        if(i->_wrap) *i->wrapBitmap= bitmap;
        i->bitmap= bitmap;

        d.p= (uint8 *)bitmap;
        d.bitmap= (uint8 *)bitmap;
        if(d.bpp< 8) {
          for(uint64 a= 0; a< bitmapSize; a++)       /// wipe the bitmap, zeroes are _used_ (it's not for neatness)
            d.p[a]= 0;
          /// interlaced+filter+less than 8bpp= 2 new buffers with current and previous scanlines are required, for the damn FILTER @$W$#@$@#
          if(interlace) {
            d.l1= new uint8[(unsigned int)(bitmapSize/ i->dy)];
            d.l2= new uint8[(unsigned int)(bitmapSize/ i->dy)];
            d.linePrev= d.l2;
            d.lineCur=  d.l1;
            d.linePos=  d.lineCur;
          }
        }
      }
      
      
      //pack.startAdvDecomp(clength, mzTarget::STDIO_FILE, f, (int64)clength, mzTarget::INT_BUFFER, null, 0);
      pack.startAdvDecomp(clength, mzTarget::STDIO_FILE, f, clength, mzTarget::INT_BUFFER, null, 0);

      while(1) {
        p1= (uint8 *)pack.doAdvDecomp(0, &size);
        if(pack.err) { i->err= 16; goto ReadError; }
        if((!size) && pack.results.srcFullyProcessed) break;
        
        /// the start of the process - only
        if((!d.xpos) && (!d.ypos)) {
          d.fline= *p1;
          p1++;
          size--;
        }

        /// helper vars
        uint8 ntimes= (d.bpp< 8? 8/ d.bpp: 1);            /// number of times that each decompressed byte will be processed
        uint8 *linePos= (uint8 *)bitmap+ d.ypos* d.lineSize;/// pointer to the start of the current line

        // pass thru all the bytes in the unpacked data
        for(uint64 a= 0; a< (uint64)size; a++, p1++) {
          if(d.xpos>= (int32)i->dx) {
            d.fline= *p1;                                   /// filter type byte
            if(interlace) {
              if(!_ImgPNG::_findGoodXY(d.ilevel, d.xpos, d.ypos, i->dx, i->dy)) { i->err= 20; goto ReadError; }
            } else {
              d.xpos= 0;                                    /// reset x position on bitmap
              d.ypos++;                                     /// increase y position on bitmap (only if n> 0)
              if(d.ypos>= (int32)i->dy) {
                i->err= 20;
                goto ReadError; }
            }
            linePos= (uint8 *)bitmap+ d.ypos* d.lineSize;  /// pointer to the start of the bitmap line (yposition)
            d.p= linePos+ ((d.xpos* d.bpp)/ 8);            /// update p
            d.nbit= (d.xpos* d.bpp)% 8;                    /// update nbit

            /// this next code is used only for less than 8bpp interlaced PNGs, swaps 2 scanline buffers
            if(useLineBuffers) {
              d.linePos= d.lineCur;                         /// d.linePos is used here as middle for swapping
              d.lineCur= d.linePrev;
              d.linePrev= d.linePos;
              d.linePos= d.lineCur;
            }
            continue;
          }

          uint8 v= *p1;                                     /// working and changing p1, results in mzPacker decompression errors

          if(filter== 0)
            v+= _ImgPNG::_filter(interlace, &d);            /// apply the filter

          if(useLineBuffers)                                /// interlaced+filter garbage for less than 8bpp PNGs
            *d.linePos= v;

          for(uint b= 0; b< ntimes; b++) {                  /// process a whole byte
            if(d.xpos>= (int32)i->dx) break;                /// ignore the rest of the byte (garbage at the end of the line)

            *d.p= (d.bpp>= 8? v: *d.p+ (((v>> (8- d.bpp- b))& d.mask)<< (8- d.bpp- d.nbit)));

            if((++d.pixByte)>= d.depth) {
              d.pixByte= 0;
              if(interlace) d.xpos+= _ImgPNG::adam7[d.ilevel].xnext; /// advance xpos
              else          d.xpos++;

              d.p= linePos+ (d.xpos* d.bpp)/ 8;            /// update p
              d.nbit= (d.xpos* d.bpp)% 8;                  /// update nbit

            } else
              d.p++;

          } /// pass thru all bits in the byte (or just one pass for bpp>=8)

          if(useLineBuffers)                                /// interlaced+filter garbage for less than 8bpp 
            d.linePos++;
        } /// pass thru each byte decompressed
      } /// extract whole data


      // convert the bitmap from less than 8bpp to 8bpp
      /// there's only 2 posibilities this can happen: cmap and greyscale.
      if(d.bpp< 8) {
        uint8 *oldBitmap= bitmap;

        // alloc
        bitmap= new uint8[i->dx* i->dy];    /// 8bpp always, so dx*dy will do
        i->bitmap= bitmap;
        if(i->_wrap) *i->wrapBitmap= bitmap;

        uint8 nbit= 0;
        uint8 *p1= (uint8 *)bitmap;
        uint8 *p2= bitmap;                  /// will walk the old bitmap

        for(uint64 a= 0, amax= (uint64)i->dx* (uint64)i->dy; a< amax; a++, p1++) {
          *p1= (*p2>> (8- d.bpp- nbit))& d.mask;

          if(i->format== ImgFormat::R8_UNORM) {
            if(d.bpp== 4)      *p1*= 17;    /// 4bpp  15(4bpp max)* 17= 255 (8bit max)
            else if(d.bpp== 2) *p1*= 85;    /// 2bpp   3(2bpp max)* 85= 255 (8bit max)
            else               *p1*= 255;   /// 1bpp
          }

          nbit+= d.bpp;
          if(nbit== 8)           /// check if reached the end of the byte
            nbit= 0, p2++;
        }
        delete[] oldBitmap;
      }

    // IEND chunk - last chunk / end of file
    ///=====================================
    } else if(!strcmp8(cname, "IEND")) {
      /// some error checks; if none, just break from loop
      if(!i->dx) { i->err= 17; goto ReadError; }       /// IHDR was not read
      if((i->format== ImgFormat::CMAP_RGB) && !i->cmap) { i->err= 19; goto ReadError; } /// PLTE was not read and it should've been
      if(!bitmap) { i->err= 20; goto ReadError; }      /// IDAT was not read
      // if(!bitmapFilled) { err= 21; goto ReadError; } /// bitmap data buffer not filled <<< bitmapFilled is not updated anymore. a check to ypos is done, for overflow
      break;


    // Ancillary chunks (extra chunks)
    ///===============================

    // tRNS - transparency 
    } else if(!strcmp8(cname, "tRNS")&& !tRNSloaded) {
      if(!bitmapSize) { i->err= 17; goto ReadError; }  /// if IHDR is not first, nothing is initialized
      tRNSloaded= true;

      //if(colType== 0) {
      //  i->hasTrnCol= true;
      //  SFREAD(&i->trnCol.r, 2, 1, f) i->trnCol.r= byteSwp16(i->trnCol.r);

      //} else if(colType== 2) {
      //  i->hasTrnCol= true;
      //  SFREAD(&i->trnCol.r, 2, 1, f) i->trnCol.r= byteSwp16(i->trnCol.r);
      //  SFREAD(&i->trnCol.g, 2, 1, f) i->trnCol.g= byteSwp16(i->trnCol.g);
      //  SFREAD(&i->trnCol.b, 2, 1, f) i->trnCol.b= byteSwp16(i->trnCol.b);

      //} else 
      if(colType== 3) {
        uint8 *tcmap= new uint8[1024];
        for(uint16 a= 0; a< 256; a++) {
          tcmap[a* 4]= i->cmap[a* 3];
          tcmap[a* 4+ 1]= i->cmap[a* 3+ 1];
          tcmap[a* 4+ 2]= i->cmap[a* 3+ 2];
          if(clength> a) {
            SFREAD(&tcmap[a* 4+ 3], 1, 1, f)
          } else
            tcmap[a* 4+ 3]= 255;
        }
        delete[] i->cmap;
        i->_set(ImgFormat::CMAP_RGBA);
        i->cmap= tcmap;
        //i->cmapBpp= 32;

      /// ignore transparent color if not CMAP
      } else {
        if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }
      }

    // gAMA chunk: Image gamma - IGNORED
    } else if(!strcmp8(cname, "gAMA")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // cHRM chunk: Primary chromaticities - IGNORED
    } else if(!strcmp8(cname, "cHRM")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // sRGB chunk: Standard RGB color space - IGNORED
    } else if(!strcmp8(cname, "sRGB")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }
    
    // iCCP chunk: Embedded ICC profile - IGNORED
    } else if(!strcmp8(cname, "iCCP")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // iTXt, tEXt, and zTXt <<< NEEDS FURTHER THINKING, to be or not to be <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // bKGD chunk: Background color - IGNORED
    } else if(!strcmp8(cname, "bKGD")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // pHYs chunk: Physical pixel dimensions - IGNORED
    } else if(!strcmp8(cname, "pHYs")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // sBIT chunk: Significant bits - IGNORED
    } else if(!strcmp8(cname, "sBIT")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // sPLT chunk: Suggested palette - IGNORED
    } else if(!strcmp8(cname, "sPLT")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // hIST chunk: Palette histogram - IGNORED
    } else if(!strcmp8(cname, "hIST")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // tIME chunk: Image last-modification time - IGNORED
    } else if(!strcmp8(cname, "tIME")) {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    // this is an UNKNOWN chunk, try to skip it
    } else {
      if(fseek(f, clength, SEEK_CUR)) { i->err= 9; goto ReadError; }

    } /// what kind of chunk this is

    // CRC checks ARE NOT DONE. they can be done, but heck...
    // they are helpful in web images, and this lib is not for that
    SFREAD(&cCRC, 4, 1, f) cCRC= _ImgPNG::byteSwp32(cCRC);

  } /// pass thru all chunks


  fclose(f);

  return true;  // PNG read was ok if reached this point

ReadError:      // this avoids placing all these lines in every error check
  fclose(f);
  i->delData();
  return false;
}

#undef SFREAD












// PNG save func ====================------------------------
///==========================================================

/// a safe fwrite, configured for the next func only
#define SFWRITE(a, b, c, d) if(fwrite(a, b, c, d)!= (c)) { i->err= 10; fclose(f); return false; }

bool Img::_savePNG(const char *fname, Img *i) {

  i->err= 0;
  static uint8 PNGfileStamp[9]= "\x89PNG\xd\xa\x1a\xa";
  mzPacker pack;
  //pack.setCompressionLevel(0);
  void *bitmap= (i->_wrap? *i->wrapBitmap: i->bitmap);

  uint32 clength;         /// chunk length
  uint8 *cdata;           /// will hold chunk data for writing
  uint32 cCRC;            /// chunk CRC
  uint8 colType;          /// PNG color type
  uint8 compression= 0;   /// compression type (only 1 i think there is)
  uint8 filter= 0;        /// filter type (only 1 in specs i think)
  uint8 interlace= 0;     /// interlace type (only on or off i think)
  //bool hasCMAP= false;

  // size_t clengthPos, cdataPos, curPos;
  if(i->nchannels== 1 && i->bpp>= 8 && i->bpp<= 16 && !i->compressed)
    colType= 0;
  else if(i->nchannels== 3 && i->bpp>= 24 && i->bpp<= 48 && !i->compressed)
    colType= 2;
  else if(i->format== ImgFormat::CMAP_RGB || i->format== ImgFormat::CMAP_RGBA)
    colType= 3;
  else if(i->nchannels== 2 && i->bpp>= 16 && i->bpp<= 32 && !i->compressed)
    colType= 4;
  else if(i->nchannels== 4 && i->bpp>= 32 && i->bpp<= 64 && !i->compressed)
    colType= 6;
  else {
    i->err= 22;      /// PNG doesn't support this image format
    return false;
  }

  /// abort checks
  //if(i->cmapBpp== 16) {
  //  i->err= 13;
  //  return false;
  //}
  
  // >> open file <<
  FILE *f= fopen(fname, "wb");
  if(!f) { i->err= 8; return false; }

  /// write PNG file stamp
  SFWRITE(PNGfileStamp, 8, 1, f)


  // IHDR chunk, first, PNG header
  ///=============================
  clength= _ImgPNG::byteSwp32(13);                     /// IHDR has fixed data size

  /// chunk data
  cdata= new uint8[13];
  ((uint32 *)cdata)[0]= _ImgPNG::byteSwp32(i->dx);
  ((uint32 *)cdata)[1]= _ImgPNG::byteSwp32(i->dy);
  cdata[8]= i->bpc[0];
  cdata[9]= colType;
  cdata[10]= compression;
  cdata[11]= filter;
  cdata[12]= interlace;
  
  cCRC= pack.crc32_MK1(0, "IHDR", 4);
  cCRC= _ImgPNG::byteSwp32(pack.crc32_MK1(cCRC, cdata, 13));

  // write
  SFWRITE(&clength, 4,  1, f)
  SFWRITE("IHDR",   4,  1, f)
  SFWRITE(cdata,    13, 1, f)
  SFWRITE(&cCRC,    4,  1, f)

  delete[] cdata;                  // dealloc ... never forget those days...

  // PLTE chunk - cmap palette
  ///=========================
  if(colType== 3) {
    clength= 768; clength= _ImgPNG::byteSwp32(clength); /// fixed length 256 colors * 3
    bool delCdata= false;
    cCRC= pack.crc32_MK1(0, "PLTE", 4);

    if(i->format== ImgFormat::CMAP_RGB) {
      cdata= i->cmap;
      cCRC= _ImgPNG::byteSwp32(pack.crc32_MK1(cCRC, i->cmap, 768));
    } else {
      
      /// 32bit cmap -> 24bit cmap
      cdata= new uint8[768];
      delCdata= true;             /// flag cdata for delete[] operation
      
      for(int16 a= 0; a< 256; a++) {
        cdata[a* 3]=    i->cmap[a* 4];
        cdata[a* 3+ 1]= i->cmap[a* 4+ 1];
        cdata[a* 3+ 2]= i->cmap[a* 4+ 2];
      }
      
      cCRC= _ImgPNG::byteSwp32(pack.crc32_MK1(cCRC, cdata, 768));
    } /// 24 or 32 bit cmap

    SFWRITE(&clength, 4, 1, f)
    SFWRITE("PLTE",   4, 1, f)
    SFWRITE(cdata,    768, 1, f)  /// just write the cmap
    SFWRITE(&cCRC,    4, 1, f)

    if(delCdata) delete[] cdata;   // dealloc
  } /// if image type == CMAP


  // tRNS chunk - transparency
  ///=========================
  
  /*
  // RGB image
  if(i->hasTrnCol&& colType== 2) {
    
    /// populate cdata
    cdata= new uint8[6];

    if(i->bpp== 24) {          /// 24bit RGB
      cdata[0]= 0;
      cdata[1]= (uint8)i->trnCol.r;
      cdata[2]= 0;
      cdata[3]= (uint8)i->trnCol.g;
      cdata[4]= 0;
      cdata[5]= (uint8)i->trnCol.b;
    } else {                /// 48bit RGB
      ((uint16 *)cdata)[0]= byteSwp16(i->trnCol.r);
      ((uint16 *)cdata)[1]= byteSwp16(i->trnCol.g);
      ((uint16 *)cdata)[2]= byteSwp16(i->trnCol.b);
    }
    /// rest of vars
    clength= byteSwp32(6);
    cCRC= pack.crc32(0, "tRNS", 4);
    cCRC= byteSwp32(pack.crc32(cCRC, cdata, 6));
    
    /// write
    SFWRITE(&clength, 4, 1, f)
    SFWRITE("tRNS",   4, 1, f)
    SFWRITE(cdata,    6, 1, f)
    SFWRITE(&cCRC,    4, 1, f)

    delete[] cdata;          // dealloc
  } /// RGB image tRNS chunk

  // GREY image
  if(i->hasTrnCol&& colType== 0) {
    /// populate cdata
    cdata= new uint8[2];
    if(i->bpp<= 8) {           /// 8bit and less GREY
      cdata[0]= 0;
      cdata[1]= (uint8)i->trnCol.r;
    } else {                /// 16bit GREY
      *((uint16 *)cdata)= byteSwp16(i->trnCol.r);
    }
    /// rest of vars
    clength= byteSwp32(2);
    cCRC= pack.crc32(0, "tRNS", 4);
    cCRC= byteSwp32(pack.crc32(cCRC, cdata, 2));

    /// write
    SFWRITE(&clength, 4, 1, f)
    SFWRITE("tRNS",   4, 1, f)
    SFWRITE(cdata,    2, 1, f)
    SFWRITE(&cCRC,    4, 1, f)
    
    delete[] cdata;          // dealloc
  } /// GREY image tRBS chunk
  */

  // CMAP image
  if(colType== 3 && i->format== ImgFormat::CMAP_RGBA) {
    /// cdata populate
    cdata= new uint8[256];
    for(int16 a= 0; a< 256; a++)
      cdata[a]= i->cmap[a* 4];
    /// rest of vars
    clength= _ImgPNG::byteSwp32(256);
    cCRC= pack.crc32_MK1(0, "tRNS", 4);
    cCRC= _ImgPNG::byteSwp32(pack.crc32_MK1(cCRC, cdata, 256));

    SFWRITE(&clength, 4, 1, f)
    SFWRITE("tRNS",   4, 1, f)
    SFWRITE(cdata,    2, 1, f)
    SFWRITE(&cCRC,    4, 1, f)

    delete[] cdata;
  } /// CMAP image tRBS chunk
  

  // IDAT chunk - PNG compressed data chunk
  ///======================================

  /// alloc a buffer for compression
  uint32 lineSize= ((i->dx* i->bpp)% 8== 0)? (i->dx* i->bpp)/ 8+ 1: (i->dx* i->bpp)/ 8+ 1+ 1;
  uint64 bitmapSize= (lineSize- 1)* i->dy;
  

  uint64 len= pack.compressBound(bitmapSize+ i->dy);

  cdata= new uint8[(uint)len];

  uint8 *tmp= new uint8[(unsigned int)(bitmapSize+ i->dy)];
  for(uint32 a= 0; a< i->dy; a++) {
    tmp[a* lineSize]= 0;    /// filter type
    for(uint32 b= 1; b< lineSize; b++)
      tmp[a* lineSize+ b]= ((uint8 *)bitmap)[a* (lineSize- 1)+ (b- 1)];
  }


  // compress data
  pack.compress(tmp, bitmapSize+ i->dy, cdata, len);
  if(pack.err) { i->err= 15; delete[] cdata; fclose(f); return false; }

  clength= _ImgPNG::byteSwp32((int32)pack.results.outFilled);
  cCRC= pack.crc32_MK1(0, "IDAT", 4);
  cCRC= _ImgPNG::byteSwp32(pack.crc32_MK1(cCRC, cdata, pack.results.outFilled));

  SFWRITE(&clength, 4, 1, f)
  SFWRITE("IDAT",   4, 1, f)
  SFWRITE(cdata,    (size_t)pack.results.outFilled, 1, f)
  SFWRITE(&cCRC,    4, 1, f)

  delete[] tmp;
  delete[] cdata;

  // IEND chunk - last chunk / end of file
  ///=====================================
  clength= 0;

  cCRC= pack.crc32_MK1(0, "IEND", 4);
  cCRC= _ImgPNG::byteSwp32(cCRC);

  SFWRITE(&clength, 4, 1, f)
  SFWRITE("IEND",   4, 1, f)
  SFWRITE(&cCRC,    4, 1, f)

  fclose(f);

  /// update Img's file
  i->fileName= fname;
  i->fileType= 2;

  return true;
}

