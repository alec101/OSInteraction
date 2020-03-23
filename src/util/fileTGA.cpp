#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <stdio.h>

#include "util/typeShortcuts.h"
#include "util/imgClass.h"

/* TODO:
*/


class _ImgTGA {
  // TGA header // http://www.paulbourke.net/dataformats/tga/
  ///==========
  struct TGAH {
    uint8 idlen;      // 1B imgDesc size
    uint8 cmapFlag;   // 1B tga has a cmap (true/false)
    uint8 imgType;    // 1B RGB/GREY/INDEX/ compressed(for all prev types)
    int16 cmapOrig;   // 2B should always be 0; other values not handled / not shure why this even exists
    int16 cmapLen;    // 2B pal size should be 256 only. bigger than this, just do a rgb16 image ffs
    int8 cmapBpc;     // 1B bits per color 16 / 24 / 32 (512-1024 pal size)
    uint16 x0;        // 2B image x origin ... not shure if ever used
    uint16 y0;        // 2B image y origin ... not shure if ever used
    uint16 dx;        // 2B image width
    uint16 dy;        // 2B image height
    uint8 bpp;        // 1B 16 / 24 / 32
    uint8 imgDesc;    // 1B 0000xxxx = alpha/attrib size, xxxx0000 one is reserved, one is origin, 2 are interleaving flag
    // TOTAl 18 + idlen
    int8 *id;         // idlen

    TGAH() { id= null; delData(); }
    ~TGAH() { delData(); }
    void delData() { if(id) delete[] id; id= null; idlen= cmapFlag= imgType= 0; cmapOrig= cmapLen= 0; cmapBpc= 0; x0= y0= dx= dy= 0; bpp= imgDesc= 0; }
  };

  friend class Img;
}; /// Img TGA private




///=========================================///
// TGA LOAD func ================----------- //
///=========================================///

bool Img::_loadTGA(const char *filename, Img *i) {
  i->delData();

  bool compressed= false;
  bool swap= false;

  uint8 *tb= null;
  uint8 t[4];
  uint8 h, n, c, swp;
  int16 j;
  _ImgTGA::TGAH tgah;
  uint8 *bitmap= null;
  i->err= 0;
  uint cmapBpp= 0;

  i->fileName= filename;
  i->fileType= 1;

  FILE *f= fopen(filename, "rb");
  if(f==NULL) {
    i->err= 8;
    return false;
  }

  if(fread(&tgah.idlen,    1, 1, f)!= 1) goto ReadError;    /// 1B
  if(fread(&tgah.cmapFlag, 1, 1, f)!= 1) goto ReadError;    /// 1B
  if(fread(&tgah.imgType,  1, 1, f)!= 1) goto ReadError;    /// 1B
  if(fread(&tgah.cmapOrig, 2, 1, f)!= 1) goto ReadError;    /// 2B
  if(fread(&tgah.cmapLen,  2, 1, f)!= 1) goto ReadError;    /// 2B
  if(fread(&tgah.cmapBpc,  1, 1 ,f)!= 1) goto ReadError;    /// 1B
  if(fread(&tgah.x0,       2, 1, f)!= 1) goto ReadError;    /// 2B
  if(fread(&tgah.y0,       2, 1, f)!= 1) goto ReadError;    /// 2B
  if(fread(&tgah.dx,       2, 1, f)!= 1) goto ReadError;    /// 2B
  if(fread(&tgah.dy,       2, 1, f)!= 1) goto ReadError;    /// 2B
  if(fread(&tgah.bpp,      1, 1, f)!= 1) goto ReadError;    /// 1B
  if(fread(&tgah.imgDesc,  1, 1, f)!= 1) goto ReadError;    /// 1B   TOTAL 18 + idlen

  if(tgah.idlen> 0) {
    tgah.id= new int8[tgah.idlen+ 1];
    if(!tgah.id) { i->err= 12; i->delData(); fclose(f); return false; } // alloc failed
    if(fread(tgah.id, tgah.idlen, 1, f)!= 1)   /// tgah.idlen in size
      goto ReadError;
  }

  /// abort checks
  if(tgah.cmapFlag)
  if(tgah.cmapBpc!= 16)
    if(tgah.cmapBpc!= 24)
      if(tgah.cmapBpc!= 32) {
        i->err= 13; // cmap palette not supported
        fclose(f);
        return false;
      }

  if((!tgah.dy)|| (!tgah.dx)) {
    i->err= 2;
    fclose(f);
    i->delData();
    return false;
  }
  
  /// fill in various vars
  //i->bpp= tgah.bpp;
  cmapBpp= tgah.cmapBpc;
  i->dx= tgah.dx;
  i->dy= tgah.dy;
  if(tgah.imgType>= 9)
    compressed= true;         /// flag TGA file as compressed
  c= tgah.bpp/ 8;             /// c will hold the number of pixel channels or 2 for 16bit BGRA

  if((tgah.imgType== 2)&& (tgah.bpp== 32))      i->_set(ImgFormat::R8G8B8A8_UNORM);
  else if((tgah.imgType== 2)&& (tgah.bpp== 16)) i->_set(ImgFormat::R5G5B5A1_UNORM_PACK16);
  else if((tgah.imgType== 2)&& (tgah.bpp== 24)) i->_set(ImgFormat::R8G8B8_UNORM);
  //else if((tgah.imgType== 2)&& (tgah.bpp== 16))  i->format= IMG_RGB16;
  else if((tgah.imgType== 1)&& (tgah.cmapBpc== 16)) i->_set(ImgFormat::CMAP_RGBA);     //IMG_CMAP;
  else if((tgah.imgType== 1)&& (tgah.cmapBpc== 24)) i->_set(ImgFormat::CMAP_RGB);      //IMG_CMAP;
  else if((tgah.imgType== 1)&& (tgah.cmapBpc== 32)) i->_set(ImgFormat::CMAP_RGBA);     //IMG_CMAP;
  else if((tgah.imgType== 3)&& (tgah.bpp== 8))   i->_set(ImgFormat::R8_UNORM);              //IMG_GREY;
  else if((tgah.imgType== 3)&& (tgah.bpp== 16))  i->_set(ImgFormat::R8G8_UNORM);            //IMG_GREY_ALPHA;
  else if((tgah.imgType== 9)&& (tgah.bpp== 8))   i->_set(ImgFormat::CMAP_RGB);              //IMG_CMAP;
  else if((tgah.imgType== 10)&& (tgah.bpp== 32)) i->_set(ImgFormat::R8G8B8A8_UNORM);        //IMG_RGBA;
  else if((tgah.imgType== 10)&& (tgah.bpp== 16)) i->_set(ImgFormat::R5G5B5A1_UNORM_PACK16); //IMG_RGBA16;
  else if((tgah.imgType== 10)&& (tgah.bpp== 24)) i->_set(ImgFormat::R8G8B8_UNORM);          //IMG_RGB;
  //else if((tgah.imgType== 10)&& (tgah.bpp== 16)) i->_set(ImgFormat::R5G6B5_UNORM_PACK16);   //IMG_RGB16;
  else if((tgah.imgType== 11)&& (tgah.bpp== 8))  i->_set(ImgFormat::R8_UNORM);              //IMG_GREY;
  else if((tgah.imgType== 11)&& (tgah.bpp== 16)) i->_set(ImgFormat::R8G8_UNORM);            //IMG_GREY_ALPHA;
  else {
    i->err= 14; // unsupported TGA
    fclose(f);
    return false;
  }

  /// flag to swap TGA's internal format of BGR to RGB
  if(i->format== ImgFormat::R8G8B8A8_UNORM || i->format== ImgFormat::R8G8B8_UNORM || i->format== ImgFormat::R5G5B5A1_UNORM_PACK16)
    swap= true;

  /// load palette for CMAP (color map palette)
  if(i->format== ImgFormat::CMAP_RGB) {
    uint8 *tp;
    uint8 t[4];
    uint32 tmp;

    i->cmap= new uint8[256* (cmapBpp/ 8)];
    //cmapBpp= 24;

    tp= i->cmap;
    for(int16 a= 0; a< 256; a++) {
      if(fread(t, 1, cmapBpp/ 8, f)!= cmapBpp/ 8)  /// read color value to be multiplied
        goto ReadError;

      if(cmapBpp== 16) {                  /// 16 bpcolor
        //GGGBBBBB ARRRRRGG  << tga 16bit storage
        tmp= (t[1]>> 2)& 0x1F;               tp[0]= (uint8)((uint32)((tmp* 0xFF)/ 0x1F));
        tmp= ((t[1]& 0x03)<< 3)+ (t[0]>> 5); tp[1]= (uint8)((uint32)((tmp* 0xFF)/ 0x1F));
        tmp= t[0]& 0x1F;                     tp[2]= (uint8)((uint32)((tmp* 0xFF)/ 0x1F));
        tmp= t[1]>> 7;                       tp[3]= (tmp? 0xFF: 0);

      } else if(cmapBpp== 24) {           /// 24 bpc
        /// BGR to RGB
        tp[0]= t[2];
        tp[1]= t[1];
        tp[2]= t[0];
      } else {                            /// 32 bpc
        /// BGRA to RGBA
        tp[0]= t[2];
        tp[1]= t[1];
        tp[2]= t[0];
        tp[3]= t[3];
      }
      tp+= cmapBpp/ 8;                    /// advance pointer
    }
  }
    
  /// bitmap memory alloc
  bitmap= new uint8[(uint64)(i->dx* i->dy* c)];

  if(i->_wrap)
    *i->wrapBitmap= bitmap;
  i->bitmap= bitmap;

  if(!bitmap) { i->err= 12; i->delData(); fclose(f); return false;}
  tb= (uint8 *)bitmap;                    /// tb will 'walk' the bitmap
  
  // not compressed TGA image
  if(!compressed) {
    if(i->format== ImgFormat::R8G8B8A8_UNORM || i->format== ImgFormat::R8G8B8_UNORM ||
       i->format== ImgFormat::R5G5B5A1_UNORM_PACK16 || i->format== ImgFormat::R5G6B5_UNORM_PACK16) {
      for(uint64 a= 0, s= i->dx* i->dy; a< s; a++, tb+= c) {
        if(fread(tb, c, 1, f)!= 1)        /// read BGR
          goto ReadError;

        /// BGR to RGB
        if(i->bpp== 16) {                    /// 16Bpp
          // convert from [GGGBBBBB ARRRRRGG] to [RRRRRGGG GGBBBBBA]
          t[0]= (tb[1]<< 1)+ (tb[0]>> 7); /// t[0]= RRRRRGG0+ 0000000G= RRRRRGGG
          t[1]= (tb[0]<< 1)+ (tb[1]>> 7); /// t[1]= GGBBBBB0+ 0000000A= GGBBBBBA
          tb[0]= t[0];
          tb[1]= t[1];
        } else {                          /// 24 / 32Bpp
          swp= tb[0];
          tb[0]= tb[2];
          tb[2]= swp;
        }
      }
    } else if(i->format== ImgFormat::CMAP_RGB || i->format== ImgFormat::CMAP_RGBA || i->format== ImgFormat::R8_UNORM) {
      if(fread(bitmap, 1, i->dx* i->dy, f)!= i->dx* i->dy)
        goto ReadError;
    } else if(i->format== ImgFormat::R8G8_UNORM) {
      if(fread(bitmap, 1, i->dx* i->dy* 2, f)!= i->dx* i->dy* 2)
        goto ReadError;
    }


  // compressed TGA image
  } else {
    for(uint64 a= 0; a< i->dx* i->dy;) {
      if(fread(&h, 1, 1, f)!= 1)         // read chunk header
        goto ReadError;

      n= h& 0x7f;                       /// chunk value
      n++;                               // n has to be increased, this is the rule of tga

      // RLE compressed chunk
      if(h& 0x80) {                     /// if the first bit (from left to right) is 1, this is a compression
        if(fread(t, c, 1, f)!= 1)       /// read what color will be assigned
          goto ReadError;

        /// assign t to n pixels
        for(int16 b= 0; b< n; b++) {
          j= b* c;
          for(int16 x= 0; x< c; x++)    /// for each pixel channel
            tb[j+ x]= t[x];

          /// swap BGR to RGB if neccesary
          if(swap) {
            if(i->bpp== 16) {
              // convert from [GGGBBBBB ARRRRRGG] to [RRRRRGGG GGBBBBBA]
              uint8 b1= (tb[j+ 1]<< 1)+ (tb[j]>> 7);  /// t[0]= RRRRRGG0+ 0000000G= RRRRRGGG
              uint8 b2= (tb[j]<< 1)+ (tb[j+ 1]>> 7);  /// t[1]= GGBBBBB0+ 0000000A= GGBBBBBA
              tb[j]= b1;
              tb[j+ 1]= b2;
            } else {
              swp= tb[j];
              tb[j]= tb[j+ 2];
              tb[j+ 2]= swp;
            }
          }
        } /// assign same color for n pixels

        tb+= (n)* c;                      /// advance tb
        a+= n;
      // raw uncompressed chunk
      } else {
        if(fread(tb, c, n, f)!= n)   /// read n pixels from file
          goto ReadError;

        /// swap BGR to RBG
        if(swap) {
          if(i->bpp== 16) {
            for(uint16 x= 0; x< n; x++) {
              j= x* c;
              // convert from [GGGBBBBB ARRRRRGG] to [RRRRRGGG GGBBBBBA]
              t[0]= (tb[j+ 1]<< 1)+ (tb[j]>> 7); /// t[0]= RRRRRGG0+ 0000000G= RRRRRGGG
              t[1]= (tb[j]<< 1)+ (tb[j+ 1]>> 7); /// t[1]= GGBBBBB0+ 0000000A= GGBBBBBA
              tb[j]= t[0];
              tb[j+ 1]= t[1];
            }
          } else {
            for(uint16 x= 0; x< n; x++) {
              j= x* c;
              swp= tb[j];
              tb[j]= tb[j+ 2];
              tb[j+ 2]= swp;
            }
          }
        }
        tb+= (n)* c;                    /// advance tb
        a+= n;                          /// advance a
      } /// chunk type (RLE or raw data)
    } /// for each pixel in image
  } /// if compressed or !compressed

  fclose(f);

  // if tga's text desc is used for something... this line should go away
  if(tgah.id!= NULL) { delete[] tgah.id; tgah.id= NULL; }
  
  return true;

ReadError:
  i->err= 9;
  fclose(f);
  i->delData();
  return false;
}







///=========================================///
// TGA SAVE func ================----------- //
///=========================================///

bool Img::_saveTGA(const char *filename, Img *i) {
  if((i->bpp!= 8) && (i->bpp!= 24) && (i->bpp!= 32) && (i->bpp!= 16)) {
    i->err= 3;
    return false;
  }
  if(i->compressed) { i->err= 14; return false; }
  if((!i->dy) || (!i->dx)) { i->err= 2; return false; }

  void *bitmap= (i->_wrap? *i->wrapBitmap: i->bitmap);
  if(!bitmap) { i->err= 1; return false; }

  _ImgTGA::TGAH tgah;

  tgah.idlen= 0;            // 1B imgDesc size
  tgah.cmapOrig= 0;         // 2B
  tgah.cmapLen= 0;          // 2B pal size should be 256 only. bigger than this, just do a rgb16 image ffs
  tgah.cmapBpc= 0;          // 1B bits per color 16 / 24 / 32 (512-1024 pal size)
  tgah.x0= 0;               // 2B image x origin ... not shure if ever used
  tgah.y0= 0;               // 2B image y origin ... not shure if ever used
  tgah.dx= i->dx;           // 2B image width
  tgah.dy= i->dy;           // 2B image height
  tgah.bpp= (uint8)i->bpp;  // 1B 16 / 24 / 32

  if(i->format>= ImgFormat::R8G8B8A8_UNORM && i->format<= ImgFormat::R8G8B8A8_SRGB) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 2;       /// 1/2/3 - uncompressed; 9/10/11 - RLE compression
    tgah.imgDesc= 8;        // 8 bit alpha, no other flags
  } else if(i->format>= ImgFormat::R8G8B8_UNORM && i->format<= ImgFormat::R8G8B8_SRGB) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 2;       /// 1/2/3 - uncompressed; 9/10/11 - RLE compression
    tgah.imgDesc= 0;        // 0 bit alpha, no other flags
  } else if(i->format== ImgFormat::R5G5B5A1_UNORM_PACK16) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 2;       /// 1/2/3 - uncompressed; 9/10/11 - RLE compression
    tgah.imgDesc= 1;        // 1 bit alpha, no other flags
  } else if(i->format== ImgFormat::CMAP_RGB) {
    tgah.cmapFlag= 1;      /// cmap (true/false)
    tgah.imgType= 1;       /// 1/2/3 - uncompressed, 9/10/11 - RLE compression
    tgah.imgDesc= 0;        // 0 bit alpha, no other flags
  } else if(i->format== ImgFormat::CMAP_RGBA) {
    tgah.cmapFlag= 1;      /// cmap (true/false)
    tgah.imgType= 1;       /// 1/2/3 - uncompressed, 9/10/11 - RLE compression
    tgah.imgDesc= 8;        // 8 bit alpha, no other flags
  } else if(i->format>= ImgFormat::R8_UNORM && i->format<= ImgFormat::R8_SRGB) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 3;       /// 1/2/3 - uncompressed, 9/10/11 - RLE compression
    tgah.imgDesc= 0;        // 0 bit alpha, no other flags
  } else if(i->format>= ImgFormat::R8G8_UNORM && i->format<= ImgFormat::R8G8_SRGB) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 3;       /// 1/2/3 - uncompressed, 9/10/11 - RLE compression
    tgah.imgDesc= 8;        // 8 bit alpha, no other flags
  } else {
    i->err= 14;
    return false;
  }

  FILE *f= fopen(filename,"wb");
  if(!f) {
    i->err= 8;
    return false;
  }
  fwrite(&tgah.idlen,     1, 1, f);	/// 1B
  fwrite(&tgah.cmapFlag,  1, 1, f);	/// 1B
  fwrite(&tgah.imgType,   1, 1, f);	/// 1B
  fwrite(&tgah.cmapOrig,  2, 1, f); /// 2B
  fwrite(&tgah.cmapLen,   2, 1, f); /// 2B
  fwrite(&tgah.cmapBpc,   1, 1, f); /// 1B
  fwrite(&tgah.x0,        2, 1, f);	/// 2B
  fwrite(&tgah.y0,        2, 1, f);	/// 2B
  fwrite(&tgah.dx,        2, 1, f);	/// 2B
  fwrite(&tgah.dy,        2, 1, f);	/// 2B
  fwrite(&tgah.bpp,       1, 1, f);	/// 1B
  fwrite(&tgah.imgDesc,   1, 1, f);	/// 1B    TOTAl 18 + idlen
  
  if(i->format== ImgFormat::CMAP_RGB || i->format== ImgFormat::CMAP_RGBA) {          /// write palette
    uint8 *t= i->cmap;
    for(uint a= 0; a< 256; a++)	{
      for(int b= 0; b< 3; b++)
        fwrite(&t[2- b], 1, 1, f);    /// convert to BGR
      if(i->bpp== 32)
        fwrite(&t[3], 1, 1, f);       /// alpha channel
      t+= i->bpp/ 8;
    }
  }

  uint8 *t= (uint8 *)bitmap;
  // 8 bpp
  if(i->bpp== 8)
    fwrite(bitmap, 1, i->dx* i->dy, f);

  // 16 bpp
  else if(i->bpp== 16) {
    uint8 b1, b2;
    for(uint64 a= i->dx* i->dy; a> 0; a--) {
      // [RRRRRGGG GGBBBBBA] -> [GGGBBBBB ARRRRRGG]
      b1= (t[1]>> 1)+ (t[0]<< 7);
      b2= (t[0]>> 1)+ (t[1]<< 7);
      fwrite(&b1, 1, 1, f);
      fwrite(&b2, 1, 1, f);
      t+= 2;               /// advance t   
    }

  // 24 & 32 bpp
  } else if(i->bpp== 24|| i->bpp== 32) {
    for(uint64 a= i->dx* i->dy; a> 0; a--) {
      for(uint b= 0; b< 3; b++)
        fwrite(&t[2- b], 1, 1, f);      /// convert RGB to BGR
      if(i->bpp== 32)                   /// write alpha channel if there is one
        fwrite(&t[3], 1, 1, f);
      t+= i->bpp/ 8;
    }
  }

  fclose(f);

  i->fileName= filename;
  i->fileType= 1;
  i->err= 0;
  return true;
}

//} // namespace _Img










