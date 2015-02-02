#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif


//#include "OSinteraction.h"
#include <stdio.h>

#include "util/typeShortcuts.h"
#include "util/fileTGA.h"

/* TODO:
*/

// TGA file header
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
  uint8 imgDesc;    // 1B   TOTAl 18 + idlen
  int8 *id;         // idlen

  TGAH() { id= null; delData(); }
  ~TGAH() { delData(); }
  void delData() { if(id) delete[] id; id= null; idlen= cmapFlag= imgType= 0; cmapOrig= cmapLen= 0; cmapBpc= 0; x0= y0= dx= dy= 0; bpp= imgDesc= 0; }
};


TGA::TGA() {
  bitmap= NULL;
  cmap= NULL;

  delData();
}


void TGA::delData() {
  if(bitmap) delete[] (int8 *)bitmap; bitmap= NULL;
  if(cmap)   delete[] cmap;           cmap= NULL;
  
  type= IMG_NONE;
  dx= dy= 0;
  bpp= bpc= cmapBpp= 0;

  err= 0;
}

// TARGA LOAD func, uncompressed only
bool TGA::load(cchar *filename) {
  delData();

  bool compressed= false;
  bool swap= false;

  uint8 *tb= null;
  uint8 t[4];
  uint8 h, n, c, swp;
  int16 i;
  TGAH tgah;
  err= 0;

  FILE *f= fopen(filename, "rb");
  if(f==NULL) {
    err= 1;
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
    if(!tgah.id) { err= 4; delData(); fclose(f); return false; } // alloc failed
    if(fread(tgah.id, tgah.idlen, 1, f)!= 1)   /// tgah.idlen in size
      goto ReadError;
  }

  /// abort checks
  if(tgah.cmapFlag)
  if(tgah.cmapBpc!= 16)
    if(tgah.cmapBpc!= 24)
      if(tgah.cmapBpc!= 32) {
        err= 6; // cmap palette not supported
        fclose(f);
        return false;
      }

  if((!tgah.dy)|| (!tgah.dx)) {
    err= 2;
    fclose(f);
    delData();
    return false;
  }
  
  /// fill in various vars
  bpp= tgah.bpp;
  cmapBpp= tgah.cmapBpc;
  dx= tgah.dx;
  dy= tgah.dy;
  if(tgah.imgType>= 9)
    compressed= true;     /// flag TGA file as compressed
  c= bpp/ 8;              /// c will hold the number of pixel channels or 2 for 16bit BGRA

  if((tgah.imgType== 2)&& (tgah.bpp== 32))       type= IMG_RGBA;
  else if((tgah.imgType== 2)&& (tgah.bpp== 16))  type= IMG_RGBA16;
  else if((tgah.imgType== 2)&& (tgah.bpp== 24))  type= IMG_RGB;
  else if((tgah.imgType== 2)&& (tgah.bpp== 16))  type= IMG_RGB16;
  else if((tgah.imgType== 1)&& (tgah.bpp== 8))   type= IMG_CMAP;
  else if((tgah.imgType== 3)&& (tgah.bpp== 8))   type= IMG_GREY;
  else if((tgah.imgType== 3)&& (tgah.bpp== 16))  type= IMG_GREY_ALPHA;
  else if((tgah.imgType== 9)&& (tgah.bpp== 8))   type= IMG_CMAP;
  else if((tgah.imgType== 10)&& (tgah.bpp== 32)) type= IMG_RGBA;
  else if((tgah.imgType== 10)&& (tgah.bpp== 16)) type= IMG_RGBA16;
  else if((tgah.imgType== 10)&& (tgah.bpp== 24)) type= IMG_RGB;
  else if((tgah.imgType== 10)&& (tgah.bpp== 16)) type= IMG_RGB16;
  else if((tgah.imgType== 11)&& (tgah.bpp== 8))  type= IMG_GREY;
  else if((tgah.imgType== 11)&& (tgah.bpp== 16)) type= IMG_GREY_ALPHA;
  else {
    err= 7; // unsupported TGA
    fclose(f);
    return false;
  }

  /// flag to swap TGA's internal format of BGR to RGB
  if(type== IMG_RGBA|| type== IMG_RGB|| type== IMG_RGB16|| type== IMG_RGBA16)
    swap= true;

  /// load palette for CMAP (color map palette)
  if(type== IMG_CMAP) {
    uint8 *tp;
    uint8 t[4];
    
    cmap= new uint8[768]; if(!cmap) {err= 4; delData(); fclose(f); return false;}
    cmapBpp= 24;

    tp= cmap;
    for(int16 a= 0; a< 256; a++) {
      if(fread(t, cmapBpp/ 8, 1, f)!= 1)  /// read color value to be multiplied
        goto ReadError;

      if(cmapBpp== 16) {                  /// 16 bpcolor
        tp[0]= t[0]; tp[1]= t[1];
      } else if(cmapBpp== 24) {           /// 24 bpc
        tp[0]= t[2]; tp[1]= t[1]; tp[2]= t[0];              /// BGR to RGB
      } else {                            /// 32 bpc
        tp[0]= t[2]; tp[1]= t[1]; tp[2]= t[0]; tp[3]= t[3]; /// BGRA to RGBA
      }
      tp+= cmapBpp/ 8;                    /// advance pointer
    }
  }
  
    
  /// bitmap memory alloc
  bitmap= new int8[dx* dy* c];
  if(!bitmap) { err= 4; delData(); fclose(f); return false;}
  tb= (uint8 *)bitmap;                    /// tb will 'walk' the bitmap

  // not compressed TGA image
  if(!compressed) {
    if(type== IMG_RGBA|| type== IMG_RGB|| type== IMG_RGB16|| type== IMG_RGBA16) {
      for(int64 a= 0; a< dx* dy; a++, tb+= c) {
        if(fread(tb, c, 1, f)!= 1)        /// read BGR
          goto ReadError;

        /// BGR to RGB
        if(bpp== 16) {                    /// 16Bpp
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
    } else if(type== IMG_CMAP|| type== IMG_GREY|| type== IMG_GREY_ALPHA) {
      if(fread(bitmap, dx* dy, 1, f)!= 1)
        goto ReadError;
    } else if(type== IMG_GREY_ALPHA) {
      if(fread(bitmap, dx* dy, 2, f)!= 2)
        goto ReadError;
    }


  // compressed TGA image
  } else {
    for(int64 a= 0; a< dx* dy;) {
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
          i= b* c;
          for(int16 x= 0; x< c; x++)    /// for each pixel channel
            tb[i+ x]= t[x];

          /// swap BGR to RGB if neccesary
          if(swap) {
            if(bpp== 16) {
              // convert from [GGGBBBBB ARRRRRGG] to [RRRRRGGG GGBBBBBA]
              uint8 b1= (tb[i+ 1]<< 1)+ (tb[i]>> 7);  /// t[0]= RRRRRGG0+ 0000000G= RRRRRGGG
              uint8 b2= (tb[i]<< 1)+ (tb[i+ 1]>> 7);  /// t[1]= GGBBBBB0+ 0000000A= GGBBBBBA
              tb[i]= b1;
              tb[i+ 1]= b2;
            } else {
              swp= tb[i];
              tb[i]= tb[i+ 2];
              tb[i+ 2]= swp;
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
          if(bpp== 16) {
            for(int16 x= 0; x< n; x++) {
              i= x* c;
              // convert from [GGGBBBBB ARRRRRGG] to [RRRRRGGG GGBBBBBA]
              t[0]= (tb[i+ 1]<< 1)+ (tb[i]>> 7); /// t[0]= RRRRRGG0+ 0000000G= RRRRRGGG
              t[1]= (tb[i]<< 1)+ (tb[i+ 1]>> 7); /// t[1]= GGBBBBB0+ 0000000A= GGBBBBBA
              //t[0]= tb[i+ 1];
              //t[1]= tb[i];
              tb[i]= t[0];
              tb[i+ 1]= t[1];
            }
          } else {
            for(int16 x= 0; x< n; x++) {
              i= x* c;
              swp= tb[i];
              tb[i]= tb[i+ 2];
              tb[i+ 2]= swp;
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
  err= 8;
  fclose(f);
  delData();
  return false;
}


bool TGA::save(cchar *filename) {
  if((bpp!= 8) && (bpp!= 24) && (bpp!= 32) && (bpp!= 16)) {
    err= 5;
    return false;
  }
  if(!bitmap) return false;
  
  if((!dy) || (!dx)) {
    err= 2;
    return false;
  }

  TGAH tgah;

  FILE *f= fopen(filename,"wb");
  if(!f) {
    err= 1;
    return false;
  }

  tgah.idlen= 0;      // 1B imgDesc size
  tgah.cmapOrig= 0;   // 2B
  tgah.cmapLen= 0;    // 2B pal size should be 256 only. bigger than this, just do a rgb16 image ffs
  tgah.cmapBpc= 0;    // 1B bits per color 16 / 24 / 32 (512-1024 pal size)
  tgah.x0= 0;         // 2B image x origin ... not shure if ever used
  tgah.y0= 0;         // 2B image y origin ... not shure if ever used
  tgah.dx= dx;        // 2B image width
  tgah.dy= dy;        // 2B image height
  tgah.bpp= bpp;      // 1B 16 / 24 / 32

  if(type== IMG_RGBA|| type== IMG_RGBA16) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 2;       /// 1/2/3 - uncompressed; 9/10/11 - RLE compression
    tgah.imgDesc= 8;        // ??
  } else if(type== IMG_RGB|| type== IMG_RGB16) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 2;       /// 1/2/3 - uncompressed; 9/10/11 - RLE compression
    tgah.imgDesc= 0;        // ??
  } else if(type== IMG_CMAP) {
    tgah.cmapFlag= 1;      /// cmap (true/false)
    tgah.imgType= 1;       /// 1/2/3 - uncompressed, 9/10/11 - RLE compression
    tgah.imgDesc= 8;        // ??
  } else if(type== IMG_GREY) {
    tgah.cmapFlag= 0;      /// cmap (true/false)
    tgah.imgType= 3;       /// 1/2/3 - uncompressed, 9/10/11 - RLE compression
    tgah.imgDesc= 8;        // ??
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
  
  

  if(type== IMG_CMAP) {             /// write palette
    uint8 *t= cmap;
    for(uint a= 0; a< 256; a++)	{
      for(int b= 0; b< 3; b++)
        fwrite(&t[2- b], 1, 1, f);  /// convert to BGR
      t+= 3;
    }
  }

  uint8 *t= (uint8 *)bitmap;

  if(bpp== 8)
    fwrite(bitmap, dx* dy, 1, f);

  /// 16 bpp
  else if(bpp== 16) {
    uint8 b1, b2;
    for(int64 a= 0; a< dx* dy; a++) {
      // [RRRRRGGG GGBBBBBA] -> [GGGBBBBB ARRRRRGG]
      b1= (t[1]>> 1)+ (t[0]<< 7);
      b2= (t[0]>> 1)+ (t[1]<< 7);
      //b1= t[1];
      //b2= t[0];
      fwrite(&b1, 1, 1, f);
      fwrite(&b2, 1, 1, f);
      t+= 2;               /// advance t   
    }
  /// 24 & 32 bpp
  } else if(bpp== 24|| bpp== 32) {
    for(int64 a= 0; a< dx* dy; a++) {
      for(int b= 0; b< 3; b++)
        fwrite(&t[2- b], 1, 1, f);  /// convert RGB to BGR
      if(bpp== 32)                  /// write alpha channel if there is one
        fwrite(&t[3], 1, 1, f);
      t+= bpp/ 8;
    }
  }

  fclose(f);
  err= 0;
  return true;
}


cchar *TGA::getError() {
  if(err== 0) return "TGA ok";
  if(err== 1) return "TGA ERROR: file not found";
  if(err== 2) return "TGA ERROR: bitmap size";
  if(err== 3) return "TGA ERROR: palette is empty";
  if(err== 4) return "TGA ERROR: allocation error";
  if(err== 5) return "TGA ERROR: invalid BPP";
  if(err== 6) return "TGA ERROR: cmap palette not suported";
  if(err== 7) return "TGA ERROR: unknown / unsupported image type";
  if(err== 8) return "TGA ERROR: file read error";
  return "TGA UNKNOWN";
}


bool TGA::loadPalette(cchar *name) {
  if(cmap==NULL) cmap= new uint8[768];

  FILE *f= fopen(name,"rb");
  if(!f) {
    delete[] cmap;
    cmap= NULL;
    err= 1;
    return false;
  }
  if(fread(cmap, 768, 1, f)!= 768) {
    delete[] cmap;
    cmap= null;
    fclose(f);
    err= 8;
    return false;
  }
    
  fclose(f);
  err= 0;
  return true;
}


bool TGA::savePalette(cchar *name) {
  if(cmap== NULL) { err= 3; return false; }
  FILE *f= fopen(name, "wb");
  if(!f)
    return false;
  fwrite(cmap, 768, 1, f);
  fclose(f);
  err= 0;
  return true;
}






