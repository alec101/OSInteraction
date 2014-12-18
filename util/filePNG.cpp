#define _CRT_SECURE_NO_WARNINGS


// TTTESTING vvvv
//#include "osinteraction.h"
// ^^^^^^^^^^^^^^^^^^^^^


#include <stdio.h>
#include <stdlib.h>


#include "typeShortcuts.h"
#include "stringClass8.h"
#include "mzPacker.h"
#include "filePNG.h"



/* TODO:
- errors could be bit encoded, so any number of errors could be recorded in err var
- MUST test 16 bpc images, see if byteSwps conversion is needed
*/


/// PNG uses big endian byte order(network order), iverse of what c/ c++ uses, therefore inverse all them bytes
inline long byteSwpl(long n) {
  uchar *c= (uchar *)&n;
  return (c[0]<< 24)| (c[1]<< 16)| (c[2]<< 8)| c[3];
}

/// PNG uses big endian byte order(network order), iverse of what c/ c++ uses, therefore inverse all them bytes
inline short byteSwps(short n) {
  uchar *c= (uchar *)&n;
  return (c[0]<< 8)| c[1];
}

// these don't work... maybe make them work someday...

//#define byteSwpl(n) (((uchar *)&(n))[0]<< 24)| (((uchar *)&(n))[1]<< 16)| (((uchar *)&(n))[2]<< 8)| ((uchar *)&(n))[3]
//#define byteSwps(n) (((char *)&(n))[0]<< 8)| ((char *)&(n))[1]


PNG::PNG() {
  bitmap= null;
  cmap= null;

  delData();
}


void PNG::delData() {
  if(bitmap) delete[] (char *)bitmap; bitmap= null;
  if(cmap) delete[] cmap; cmap= null;

  type= IMG_NONE;
  dx= dy= 0;
  bpp= bpc= cmapBpp= 0;
  hasTrnCol= false;
  trnCol.r= trnCol.g= trnCol.b= 0;

  err= 0;
}


struct Adam7 {
  uchar x0, y0, xnext, ynext;
} adam7[7]= { {0, 0, 8, 8},   /// lev 0
              {4, 0, 8, 8},   /// lev 1
              {0, 4, 4, 8},   /// lev 2
              {2, 0, 4, 4},   /// lev 3
              {0, 2, 2, 4},   /// lev 4
              {1, 0, 2, 2},   /// lev 5
              {0, 1, 1, 2} }; /// lev 6

struct IDATdata {
  uchar *bitmap;    /// bitmap that will hold the PNG
  uchar *p;         /// current position on bitmap
  uchar bpp;        /// bitmap bits per pixel
  long xpos, ypos;  /// current X & Y coordinate position (pixel coords)
  uchar fline;      /// current pixel line filter type
  int lineSize;     /// linesize in bytes of the bitmap
  uchar mask;       /// a mask used for less than 8bpp images
  uchar ilevel;     /// current Adam7 level
  uchar depth;      /// pixel depth 0 used only for 8bpp and more
  uchar pixByte;    /// byte number in current pixel
  uchar nbit;       /// bit number in current byte
  uchar *l1, *l2;   /// buffers that will hold 2 scanlines, used only for interlaced PNG's to help the filter ... filter
  uchar *linePrev, *lineCur, *linePos;  /// used with l1 and l2
  IDATdata() { bitmap= p= null; bpp= 0; xpos= ypos= 0; fline= 0; lineSize= 0; mask= 0; ilevel= 0; depth= 0; pixByte= nbit= 0; l1= l2= linePrev= lineCur= linePos= null; }
  ~IDATdata() { if(l1) delete[] l1; if(l2) delete[] l2; }
};

inline uchar _filter(uchar interlace, IDATdata *d);
//inline uchar _filter(uchar type, short a, short b, short c);
bool _findGoodXY(uchar &ilevel, long &xpos, long &ypos, const long dx, const long dy);

#define SFREAD(a, b, c, d) if(fread(a, b, c, d)!= (c)) { err= 2; goto ReadError; }

bool PNG::load(cchar *fname) {
  delData();
  err= 0;

  char PNGfileCheck[9];     /// PNG file stamp
  bool tRNSloaded= false;   /// security checks
    /// header vars / chunk vars
  ulong clength= 0;         /// chunk length
  char cname[5];            /// chunk name (4 bytes)
  cname[4]= 0;              /// string terminator
  ulong cCRC;               /// chunk CRC
  uchar colType;            /// PNG color type
  uchar compression;        /// compression type (only 1 i think there is)
  uchar filter;             /// filter type (only 1 in specs i think)
  uchar interlace;          /// interlace type (only on or off i think)
  /// bitmap
  //size_t n= 0;              /// n will be the number of bytes processed (no bigger than bitmapSize + dy)
  size_t bitmapSize= 0;     /// bitmap raw img data size in bytes
  bool bitmapFilled= false; /// if the bitmap data was all filled, this flag will be true
  /// decompressor
  
  size_t size;              /// size of the decompressed data
  uchar *p1;                /// p1 will walk the decompressed data
  /// filter tmp variables
  uchar *pprev= null;

  IDATdata d;

  

  // open file
  FILE *f= fopen(fname, "rb");
  if(!f) { err= 1; return false; }

  /// read PNG file check
  SFREAD(PNGfileCheck, 8, 1, f)
  PNGfileCheck[8]= 0;     /// string terminator

  /// make shure it's a PNG image
  if(string8::strcmp("\x89PNG\xd\xa\x1a\xa", PNGfileCheck)) {
    err= 9; goto ReadError; }


  /// pass thru all PNG chunks, until end chunk
  while(1) {

    /// read chunk length
    SFREAD(&clength, 4, 1, f) clength= byteSwpl(clength);
    SFREAD(cname, 4, 1, f)

    // IHDR chunk, first, PNG header
    ///=============================
    if(!string8::strcmp(cname, "IHDR")&& !dx) {   /// !dx - safety check: only 1 IHDR chunk must be present
      SFREAD(&dx,          4, 1, f) dx= byteSwpl(dx);
      SFREAD(&dy,          4, 1, f) dy= byteSwpl(dy);
      SFREAD(&bpc,         1, 1, f)
      SFREAD(&colType,     1, 1, f)
      SFREAD(&compression, 1, 1, f)
      SFREAD(&filter,      1, 1, f)
      SFREAD(&interlace,   1, 1, f)

      /// abort checks
      if(!dx|| !dy)             { err= 4; goto ReadError; }
      if(dx> 32768|| dy> 32768) { err= 4; goto ReadError; }
      if(filter!= 0)      { err= 9; goto ReadError; }
      if(interlace> 1)    { err= 9; goto ReadError; }
      if(compression!= 0) { err= 9; goto ReadError; }

      // PNG image type

      /// PNG type 0 - greyscale image
      if(colType== 0) {
        if     (bpc== 1)  { type= IMG_GREY1;  bpp= 1; }   // <<<< worth it?
        else if(bpc== 2)  { type= IMG_GREY2;  bpp= 2; }   // <<<< worth it?
        else if(bpc== 4)  { type= IMG_GREY4;  bpp= 4; }   // <<<< worth it?
        else if(bpc== 8)  { type= IMG_GREY;   bpp= 8; }
        else if(bpc== 16) { type= IMG_GREY16; bpp= 16; }
        else { err= 7; goto ReadError; }

      /// PNG type 2 - RBG image
      } else if(colType== 2) {
        if     (bpc== 8)  { type= IMG_RGB;   bpp= 24; }
        else if(bpc== 16) { type= IMG_RGB48; bpp= 48; }
        else { err= 7; goto ReadError; }

      /// PNG type 3 - cmap image
      } else if(colType== 3) {
        if     (bpc== 1) { type= IMG_CMAP1; bpp= 1; }   // <<<< worth it?
        else if(bpc== 2) { type= IMG_CMAP2; bpp= 2; }   // <<<< worth it?
        else if(bpc== 4) { type= IMG_CMAP4; bpp= 4; }   // <<<< worth it?
        else if(bpc== 8) { type= IMG_CMAP;  bpp= 8; }

      /// PNG type 4 - grey + alpha image
      } else if(colType== 4) {
        if     (bpc== 8)  { type= IMG_GREY_ALPHA;   bpp= 16; }
        else if(bpc== 16) { type= IMG_GREY_ALPHA32; bpp= 32; }
        else { err= 7; goto ReadError; }

      /// PNG type 6 - RGBA image
      } else if(colType== 6) {
        if     (bpc== 8)  { type= IMG_RGBA;   bpp= 32; }
        else if(bpc== 16) { type= IMG_RGBA64; bpp= 64; }
        else { err= 7; goto ReadError; }

      /// unknown image type
      } else { err= 9;  goto ReadError; }

      /// raw image buffer size; if it doesn't fit perfectly in full bytes, add 1 byte at the end of each line
      bitmapSize= (((dx* bpp)% 8)? ((dx* bpp)/ 8+ 1)* dy: ((dx* bpp)/ 8)* dy);

    // PLTE chunk - cmap palette
    ///=========================
    } else if(!string8::strcmp(cname, "PLTE")&& !cmap) {
      if(!dx) { err= 10; goto ReadError; }          /// if IHDR is not first, nothing is initialized
      if(clength> 768) { err= 12; goto ReadError; } /// chunk length bigger than maximum cmap size

      cmap= (uchar *)new char[768];       /// specs say there should be only one PLTE chunk... if cmap is already allocated...
      SFREAD(cmap, clength, 1, f)
      cmapBpp= 24;

    // IDAT chunk - PNG compressed data chunk
    ///======================================
    } else if(!string8::strcmp(cname, "IDAT")) {
      if(!bitmapSize) { err= 10; goto ReadError; }  /// if IHDR is not first, nothing is initialized
      if(bitmapFilled) { err= 13; goto ReadError; } /// image raw data aquired, yet another IDAT chunk found...
      
      /// alloc the bitmap
      if(!bitmap) {
        bitmap= new uchar[bitmapSize];
        d.p= (uchar *)bitmap;
        d.bitmap= (uchar *)bitmap;
        if(bpp< 8) {
          for(ulong a= 0; a< bitmapSize; a++)
            d.p[a]= 0;
          /// interlaced+filter+less than 8bpp= 2 new buffers with current and previous scanlines are required, for the damn FILTER @$W$#@$@#
          if(interlace) {
            d.l1= new uchar[bitmapSize/ dy];
            d.l2= new uchar[bitmapSize/ dy];
            d.linePrev= d.l2;
            d.lineCur=  d.l1;
            d.linePos=  d.lineCur;
          }
        }
      }
      
      d.lineSize= (int)(bitmapSize/ dy);    /// bitmap linesize in bytes
      d.depth= ((bpp/ 8)== 0? 1: (bpp/ 8)); /// pixel depth in bytes, only used for 8bpp and more
      d.mask= 0xff>> (8- bpp);              /// mask of how many bits in the byte are used for 1 pixel for less than 8bpp images
      d.bpp= bpp;
      bool useLineBuffers= (((bpp< 8) && interlace)? true: false);

      pack.startAdvDecomp(clength, mzTarget::STDIO_FILE, f, clength, mzTarget::INT_BUFFER, null, 0);

      while(1) {
        p1= (uchar *)pack.doAdvDecomp(0, &size);
        if(pack.err) { err= 14; goto ReadError; }
        if((!size) && pack.results.srcFullyProcessed) break;
        
        /// the start of the process - only
        if((!d.xpos) && (!d.ypos)) {
          d.fline= *p1;
          p1++;
          size--;
        }

        /// helper vars
        uchar ntimes= (bpp< 8? 8/ bpp: 1);                  /// number of times that each decompressed byte will be processed
        uchar *linePos= (uchar *)bitmap+ d.ypos* d.lineSize;/// pointer to the start of the current line

        // pass thru all the bytes in the unpacked data
        for(uint a= 0; a< size; a++, p1++) {
          if(d.xpos>= (long)dx) {
            d.fline= *p1;                                   /// filter type byte
            if(interlace) {
              if(!_findGoodXY(d.ilevel, d.xpos, d.ypos, dx, dy)) { err= 13; goto ReadError; }
            } else {
              d.xpos= 0;                                    /// reset x position on bitmap
              d.ypos++;                                     /// increase y position on bitmap (only if n> 0)
              if(d.ypos>= (long)dy) { err= 13; goto ReadError; }
            }
            linePos= (uchar *)bitmap+ d.ypos* d.lineSize;   /// pointer to the start of the bitmap line (yposition)
            d.p= linePos+ (d.xpos* bpp)/ 8;                 /// update p
            d.nbit= (d.xpos* bpp)% 8;                       /// update nbit

            /// this next code is used only for less than 8bpp interlaced PNGs, swaps 2 scanline buffers
            if(useLineBuffers) {
              d.linePos= d.lineCur;                         /// d.linePos is used here as middle for swapping
              d.lineCur= d.linePrev;
              d.linePrev= d.linePos;
              d.linePos= d.lineCur;
            }
            continue;
          }

          uchar v= *p1;                                     /// working and changing p1, results in mzPacker decompression errors

          if(filter== 0)
            v+= _filter(interlace, &d);                     /// apply the filter

          if(useLineBuffers)                                /// interlaced+filter garbage for less than 8bpp PNGs
            *d.linePos= v;

          for(uint b= 0; b< ntimes; b++) {                  /// process a whole byte
            if(d.xpos>= (long)dx) break;                    /// ignore the rest of the byte (garbage at the end of the line)

            *d.p= (bpp>= 8? v: *d.p+ (((v>> (8- bpp- b))& d.mask)<< (8- bpp- d.nbit)));

            if((++d.pixByte)>= d.depth) {
              d.pixByte= 0;
              if(interlace) d.xpos+= adam7[d.ilevel].xnext; /// advance xpos
              else          d.xpos++;

              d.p= linePos+ (d.xpos* bpp)/ 8;               /// update p
              d.nbit= (d.xpos* bpp)% 8;                     /// update nbit

            } else
              d.p++;

          } /// pass thru all bits in the byte (or just one pass for bpp>=8)

          if(useLineBuffers)                                /// interlaced+filter garbage for less than 8bpp 
            d.linePos++;
        } /// pass thru each byte decompressed
      } /// extract whole data


    // IEND chunk - last chunk / end of file
    ///=====================================
    } else if(!string8::strcmp(cname, "IEND")) {
      /// some error checks; if none, just break from loop
      if(!dx) { err= 10; goto ReadError; }          /// IHDR was not read
      if(type== IMG_CMAP && !cmap) { err= 12; goto ReadError; } /// PLTE was not read and it should've been
      if(!bitmap) { err= 13; goto ReadError; }      /// IDAT was not read
      // if(!bitmapFilled) { err= 15; goto ReadError; } /// bitmap data buffer not filled <<< bitmapFilled is not updated anymore. a check to ypos is done, for overflow
      break;


    // Ancillary chunks (extra chunks)
    ///===============================

    // tRNS - transparency 
    } else if(!string8::strcmp(cname, "tRNS")&& !tRNSloaded) {
      tRNSloaded= true;
      if(colType== 0) {
        hasTrnCol= true;
        SFREAD(&trnCol.r, 2, 1, f) trnCol.r= byteSwps(trnCol.r);
      } else if(colType== 2) {
        hasTrnCol= true;
        SFREAD(&trnCol.r, 2, 1, f) trnCol.r= byteSwps(trnCol.r);
        SFREAD(&trnCol.g, 2, 1, f) trnCol.g= byteSwps(trnCol.g);
        SFREAD(&trnCol.b, 2, 1, f) trnCol.b= byteSwps(trnCol.b);
      } else if(colType== 3) {
        uchar *tcmap= new uchar[1024];
        for(ushort a= 0; a< 256; a++) {
          tcmap[a* 4]= cmap[a* 3];
          tcmap[a* 4+ 1]= cmap[a* 3+ 1];
          tcmap[a* 4+ 2]= cmap[a* 3+ 2];
          if(clength> a) {
            SFREAD(&tcmap[a* 4+ 3], 1, 1, f)
          } else
            tcmap[a* 4+ 3]= 255;
        }
        delete[] cmap;
        cmap= tcmap;
        cmapBpp= 32;
      }

    // gAMA chunk: Image gamma - IGNORED
    } else if(!string8::strcmp(cname, "gAMA")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // cHRM chunk: Primary chromaticities - IGNORED
    } else if(!string8::strcmp(cname, "cHRM")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // sRGB chunk: Standard RGB color space - IGNORED
    } else if(!string8::strcmp(cname, "sRGB")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }
    
    // iCCP chunk: Embedded ICC profile - IGNORED
    } else if(!string8::strcmp(cname, "iCCP")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // iTXt, tEXt, and zTXt <<< NEEDS FURTHER THINKING, to be or not to be <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // bKGD chunk: Background color - IGNORED
    } else if(!string8::strcmp(cname, "bKGD")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // pHYs chunk: Physical pixel dimensions - IGNORED
    } else if(!string8::strcmp(cname, "pHYs")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // sBIT chunk: Significant bits - IGNORED
    } else if(!string8::strcmp(cname, "sBIT")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // sPLT chunk: Suggested palette - IGNORED
    } else if(!string8::strcmp(cname, "sPLT")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // hIST chunk: Palette histogram - IGNORED
    } else if(!string8::strcmp(cname, "hIST")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // tIME chunk: Image last-modification time - IGNORED
    } else if(!string8::strcmp(cname, "tIME")) {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    // this is an UNKNOWN chunk, try to skip it
    } else {
      if(fseek(f, clength, SEEK_CUR)) { err= 2; goto ReadError; }

    } /// what kind of chunk this is

    // CRC checks ARE NOT DONE. they can be done, but heck...
    // they are helpful in web images, and this lib is not for that
    SFREAD(&cCRC, 4, 1, f) cCRC= byteSwpl(cCRC);

  } /// pass thru all chunks


  fclose(f);

  return true;  // PNG read was ok if reached this point

ReadError:      // this avoids placing all these lines in every error check
  fclose(f);
  delData();
  return false;
}

#undef SFREAD


inline uchar _filter(uchar interlace, IDATdata *d) {
  int xprev= 0, yprev= 0, xyprev= 0;

  // 8bpp and over PNGs
  if(d->bpp>= 8) {
    if(!interlace) {
      xprev=  (d->xpos?              *(d->p- d->depth):              0);
      yprev=  (d->ypos?              *(d->p- d->lineSize):           0);
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

      /*
      /// xprev
      if((d->xpos- adam7[d->ilevel].xnext)/ d->bpp>= 0) {
        //xprev= (((uchar *)d->bitmap)[d->ypos* d->lineSize+ nbyte]>> (8- d->bpp- nbit))& d->mask;
        xprev= d->p- 
      }
      /// xyprev
      if(d->xpos- adam7[d->ilevel].xnext>= 0 && d->ypos- adam7[d->ilevel].ynext>= 0)     /// uses previous nbit& nbyte
        xyprev= (((uchar *)d->bitmap)[(d->ypos- adam7[d->ilevel].ynext)* d->lineSize+ nbyte]>> (8- d->bpp- nbit))& d->mask;
      /// yprev
      if(d->ypos- adam7[d->ilevel].ynext>= 0) {
        nbit=  (d->xpos* d->bpp)% 8;
        nbyte= (d->xpos* d->bpp)/ 8;
        yprev= (((uchar *)d->bitmap)[(d->ypos- adam7[d->ilevel].ynext)* d->lineSize+ nbyte]>> (8- d->bpp- nbit))& d->mask;
      }
      */
    }
  }


  if(!d->fline)                     /// no filter
    return 0;
  
  else if(d->fline== 1)             /// 'Sub' filter
    return (uchar)xprev;
  
  else if(d->fline== 2)             /// 'Up' filter
    return (uchar)yprev; 

  else if(d->fline== 3)             /// 'Average' filter
    return (uchar)((xprev+ yprev)/ 2);

  else if(d->fline== 4) {           /// 'Paeth' filter
    short pa= abs(yprev- xyprev);
    short pb= abs(xprev- xyprev);
    short pc= abs(xprev+ yprev- xyprev- xyprev);

    if(pc < pa && pc < pb) return (uchar)xyprev;
    else if(pb < pa) return (uchar)yprev;
    else return (uchar)xprev;
  } /// filter types
  return 0;
}


inline bool _findGoodXY(uchar &ilevel, long &xpos, long &ypos, const long dx, const long dy) {
  CheckAgain:
  if(xpos>= dx) {
    xpos= adam7[ilevel].x0;
    ypos+= adam7[ilevel].ynext;
    if(xpos>= dx) {
      ilevel++;
      if(ilevel> 6) return false;
      xpos= adam7[ilevel].x0;
      ypos= adam7[ilevel].y0;
      goto CheckAgain;
    }
  }

  if(ypos>= dy) {
    ilevel++;
    if(ilevel> 6) return false;
    xpos= adam7[ilevel].x0;
    ypos= adam7[ilevel].y0;
    goto CheckAgain;
  }
  return true;
}










// PNG save func ====================------------------------
///==========================================================

/// a safe fwrite, configured for the next func only
#define SFWRITE(a, b, c, d) if(fwrite(a, b, c, d)!= (c)) { err= 3; fclose(f); return false; }

bool PNG::save(cchar *fname) {

  err= 0;
  static char PNGfileStamp[9]= "\x89PNG\xd\xa\x1a\xa";
  mzPacker pack;
  //pack.setCompressionLevel(0);
  
  
  ulong clength;          /// chunk length
  uchar *cdata;           /// will hold chunk data for writing
  ulong cCRC;             /// chunk CRC
  uchar colType;          /// PNG color type
  uchar compression= 0;   /// compression type (only 1 i think there is)
  uchar filter= 0;        /// filter type (only 1 in specs i think)
  uchar interlace= 0;     /// interlace type (only on or off i think)

  // size_t clengthPos, cdataPos, curPos;

  if(type== IMG_GREY|| type== IMG_GREY1|| type== IMG_GREY2|| type== IMG_GREY4|| type== IMG_GREY16)
    colType= 0;
  else if(type== IMG_RGB|| type== IMG_RGB48)
    colType= 2;
  else if(type== IMG_CMAP|| type== IMG_CMAP1|| type== IMG_CMAP2|| type== IMG_CMAP4)
    colType= 3;
  else if(type== IMG_GREY_ALPHA|| type== IMG_GREY_ALPHA32)
    colType= 4;
  else if(type== IMG_RGBA|| type== IMG_RGBA64)
    colType= 6;
  else {
    err= 16;      /// PNG doesn't support this image format
    return false;
  }

  /// abort checks
  if(cmapBpp== 16) {
    err= 8;
    return false;
  }
  
  // >> open file <<
  FILE *f= fopen(fname, "wb");
  if(!f) { err= 1; return false; }

  /// write PNG file stamp
  SFWRITE(PNGfileStamp, 8, 1, f)


  // IHDR chunk, first, PNG header
  ///=============================
  clength= byteSwpl(13);                     /// IHDR has fixed data size

  /// chunk data
  cdata= new uchar[13];
  ((ulong *)cdata)[0]= byteSwpl(dx);
  ((ulong *)cdata)[1]= byteSwpl(dy);
  cdata[8]= bpc;
  cdata[9]= colType;
  cdata[10]= compression;
  cdata[11]= filter;
  cdata[12]= interlace;
  
  cCRC= pack.crc32(0, (cuchar *)"IHDR", 4);
  cCRC= byteSwpl(pack.crc32(cCRC, cdata, 13));

  // write
  SFWRITE(&clength, 4,  1, f)
  SFWRITE("IHDR",   4,  1, f)
  SFWRITE(cdata,    13, 1, f)
  SFWRITE(&cCRC,    4,  1, f)

  delete[] cdata;                  // dealloc ... never forget those days...

  // PLTE chunk - cmap palette
  ///=========================
  if(colType== 3) {
    clength= 768; clength= byteSwpl(clength); /// fixed length 256 colors * 3
    bool delCdata= false;
    cCRC= pack.crc32(0, (cuchar *)"PLTE", 4);

    if(cmapBpp== 24) {
      cdata= cmap;
      cCRC= byteSwpl(pack.crc32(cCRC, cmap, 768));
    } else {
      
      /// 32bit cmap -> 24bit cmap
      cdata= new uchar[768];
      delCdata= true;             /// flag cdata for delete[] operation
      
      for(short a= 0; a< 256; a++) {
        cdata[a* 3]= cmap[a* 4];
        cdata[a* 3+ 1]= cmap[a* 4+ 1];
        cdata[a* 3+ 2]= cmap[a* 4+ 2];
      }
      
      cCRC= byteSwpl(pack.crc32(cCRC, cdata, 768));
    } /// 24 or 32 bit cmap

    SFWRITE(&clength, 4, 1, f)
    SFWRITE("PLTE",   4, 1, f)
    SFWRITE(cdata,    768, 1, f)  /// just write the cmap
    SFWRITE(&cCRC,    4, 1, f)

    if(delCdata) delete[] cdata;   // dealloc
  } /// if image type == CMAP


  // tRNS chunk - transparency
  ///=========================

  // RGB image
  if(hasTrnCol&& colType== 2) {
    
    /// populate cdata
    cdata= new uchar[6];

    if(bpp== 24) {          /// 24bit RGB
      cdata[0]= 0;
      cdata[1]= (uchar)trnCol.r;
      cdata[2]= 0;
      cdata[3]= (uchar)trnCol.g;
      cdata[4]= 0;
      cdata[5]= (uchar)trnCol.b;
    } else {                /// 48bit RGB
      ((ushort *)cdata)[0]= byteSwps(trnCol.r);
      ((ushort *)cdata)[1]= byteSwps(trnCol.g);
      ((ushort *)cdata)[2]= byteSwps(trnCol.b);
    }
    /// rest of vars
    clength= byteSwpl(6);
    cCRC= pack.crc32(0, (cuchar *)"tRNS", 4);
    cCRC= byteSwpl(pack.crc32(cCRC, cdata, 6));
    
    /// write
    SFWRITE(&clength, 4, 1, f)
    SFWRITE("tRNS",   4, 1, f)
    SFWRITE(cdata,    6, 1, f)
    SFWRITE(&cCRC,    4, 1, f)

    delete[] cdata;          // dealloc
  } /// RGB image tRNS chunk

  // GREY image
  if(hasTrnCol&& colType== 0) {
    /// populate cdata
    cdata= new uchar[2];
    if(bpp<= 8) {           /// 8bit and less GREY
      cdata[0]= 0;
      cdata[1]= (uchar)trnCol.r;
    } else {                /// 16bit GREY
      *((ushort*)cdata)= byteSwps(trnCol.r);
    }
    /// rest of vars
    clength= byteSwpl(2);
    cCRC= pack.crc32(0, (cuchar *)"tRNS", 4);
    cCRC= byteSwpl(pack.crc32(cCRC, cdata, 2));

    /// write
    SFWRITE(&clength, 4, 1, f)
    SFWRITE("tRNS",   4, 1, f)
    SFWRITE(cdata,    2, 1, f)
    SFWRITE(&cCRC,    4, 1, f)
    
    delete[] cdata;          // dealloc
  } /// GREY image tRBS chunk
  
  // CMAP image
  if(colType== 3 && cmapBpp== 32) {
    /// cdata populate
    cdata= new uchar[256];
    for(short a= 0; a< 256; a++)
      cdata[a]= cmap[a* 4];
    /// rest of vars
    clength= byteSwpl(256);
    cCRC= pack.crc32(0, (cuchar *)"tRNS", 4);
    cCRC= byteSwpl(pack.crc32(cCRC, cdata, 256));

    SFWRITE(&clength, 4, 1, f)
    SFWRITE("tRNS",   4, 1, f)
    SFWRITE(cdata,    2, 1, f)
    SFWRITE(&cCRC,    4, 1, f)

    delete[] cdata;
  } /// CMAP image tRBS chunk
  

  // IDAT chunk - PNG compressed data chunk
  ///======================================

  /// alloc a buffer for compression
  int lineSize= ((dx* bpp)% 8== 0)? (dx* bpp)/ 8+ 1: (dx* bpp)/ 8+ 1+ 1;
  size_t bitmapSize= (lineSize- 1)* dy;
  

  size_t len= pack.compressBound(bitmapSize+ dy);

  cdata= new uchar[len];

  uchar *tmp= new uchar[bitmapSize+ dy];
  for(int a= 0; a< (int)dy; a++) {
    tmp[a* lineSize]= 0;    /// filter type
    for(int b= 1; b< lineSize; b++)
      tmp[a* lineSize+ b]= ((uchar *)bitmap)[a* (lineSize- 1)+ (b- 1)];
  }


  // compress data
  pack.compress(tmp, bitmapSize+ dy, cdata, len);
  if(pack.err) {
    err= 17; delete[] cdata; fclose(f); return false; }

  clength= byteSwpl((ulong)pack.results.outFilled);
  cCRC= pack.crc32(0, (cuchar *)"IDAT", 4);
  cCRC= byteSwpl(pack.crc32(cCRC, cdata, pack.results.outFilled));

  SFWRITE(&clength, 4, 1, f)
  SFWRITE("IDAT",   4, 1, f)
  SFWRITE(cdata,    pack.results.outFilled, 1, f)
  SFWRITE(&cCRC,    4, 1, f)

  delete[] tmp;
  delete[] cdata;

  // IEND chunk - last chunk / end of file
  ///=====================================
  clength= 0;

  cCRC= pack.crc32(0, (cuchar *)"IEND", 4);
  cCRC= byteSwpl(cCRC);

  SFWRITE(&clength, 4, 1, f)
  SFWRITE("IEND",   4, 1, f)
  SFWRITE(&cCRC,    4, 1, f)

  fclose(f);

  return true;
}

#undef SFWRITE



   /* I THINK THE CRC32 func in mzPacker is just fine.
   unsigned long crc_table[256];      // Table of CRCs of all 8-bit messages.
   
   
   int crc_table_computed = 0;    // Flag: has the table been computed? Initially false.
   
   // Make the table for a fast CRC.
   void make_crc_table(void) {
     unsigned long c;
     int n, k;
   
     for (n = 0; n < 256; n++) {
       c = (unsigned long) n;
       for (k = 0; k < 8; k++) {
         if (c & 1)
           c = 0xedb88320L ^ (c >> 1);
         else
           c = c >> 1;
       }
       crc_table[n] = c;
     }
     crc_table_computed = 1;
   }
   
   // Update a running CRC with the bytes buf[0..len-1]--the CRC
   // should be initialized to all 1's, and the transmitted value
   // is the 1's complement of the final running CRC (see the
   // crc() routine below)).
   
   unsigned long update_crc(unsigned long crc, unsigned char *buf, int len) {
     unsigned long c = crc;
     int n;
   
     if (!crc_table_computed)
       make_crc_table();

     for (n = 0; n < len; n++) {
       c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
     }

     return c;
   }
   
// Return the CRC of the bytes buf[0..len-1].
unsigned long crc(unsigned char *buf, int len) {
  return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

*/



