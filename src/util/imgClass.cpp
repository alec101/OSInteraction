#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "osinteraction.h"
#include "util/mzPacker.h"
#include "util/imgClass.h"
#include "util/mlib.hpp"
#include "util/fileOp.h"
#include <stdio.h>

//#ifdef OSI_USE_OPENGL
//#define OSI_USE_OPENGL_EXOTIC_EXT 1
//#endif


// TODO:
/*  
   - _cmap / _cmapBpp are not thread safe!!!!! - global vars used with conversion -
       cmap is VERY OLD STUFF tho, so... if you write a program to convert old cmaps with multiple threads... goin too far, right ???
   - packed / unpacked stuff, recheck everything, because i think the packed stuff is done, but not checked if unpacked (delData sets it to def=packed, tho)
   - recheck the png save. all of it.
   - adjust a image to be a power of 2
   - pixel converts - these must be fast tho
   - conversion from rgb(or anything without alpha) to RGBA(or anything with alpha) ignores trnColor (transparency color) this should be mended
   - PNG: MUST test 16 bpc images, see if byteSwps conversion is needed (i might have done this)
*/


// data formats/ compressed textures
// https://www.khronos.org/registry/DataFormat/specs/1.1/dataformat.1.1.html#RGTC

// VULKAN compression formats
/*
BC1-7   - desktops - seems to be the directx one
ETC     - desktops
ASTC    - new-kewl but only on phones
*/

/* MAYBE???????????????????
uint32_t getChannel0_32(uint32_t in_data, ImgFormat in_f) {
  _Img::_FormatData *_f= _Img::_FormatData::get(in_f);
  return in_data>> (_f->bpc[1]+ _f->bpc[2]+ _f->bpc[3]);
}
*/


using namespace Str;
using namespace mlib;




// img private
class _Img {
  friend class Img;
  friend class _ImgGlFormatConvConstructor;
  
  static uint8_t *_cmap;          // color map palette
  //uint8_t _cmapBpp;        // color palette bits per color (16 / 24 / 32)

  //static bool _loadPNG(const char *, Img *);
  //static bool _savePNG(const char *, Img *);
  //static bool _loadTGA(const char *, Img *);
  //static bool _saveTGA(const char *, Img *);

  

  ///====================================================================///
  // Formats Database / conversions ===================================== //
  ///====================================================================///

  struct RGBAconv {
    union {
      struct { uint64_t r, g, b, a; };
      uint64_t v[4];
    };
  };

  //database with all the formats, with their conversion funcs
  struct _FormatData {
    ImgFormat format;
    uint16_t bpp;          // bits per pixel
    uint8_t bpc[4];       // bits per channel, for each 4 possible channels
    uint8_t swizzle[4];   // [def:Identity] normally the data is read as RGBA. if the channel format differs, this is used to identify each channel's color
    uint8_t nchannels;    // number of active channels (0-4)
    uint8_t compressed;   // [bool] compressed format or not
    void (*(_fromFunc)) (uint8_t *, _FormatData *, uint8_t, _Img::RGBAconv *);
    void (*(_toFunc)) (RGBAconv *, uint8_t *, _FormatData *, uint8_t);

    #ifdef IMG_CLASS_USE_OPENGL
    GLint glFormat;
    GLenum glChannelFormat;
    GLenum glDataType;
    inline void setGl(GLint f, GLenum cFrmt, GLenum datType) { glFormat= f, glChannelFormat= cFrmt, glDataType= datType; }
    static _FormatData *getGl(GLint);     // points to the FormatData that has the oGL format
    #endif
    
    static _FormatData *get(ImgFormat);   // points to the ImgFormatData that has ImgFormat
  };


  #ifdef IMG_CLASS_USE_OPENGL
  static void setGlData();
  inline static void set(ImgFormat f, GLint glF, GLenum c, GLenum d) { _Img::_FormatData::get(f)->setGl(glF, c, d); }
  #endif

  static _FormatData _formats[];



  static const uint64_t _2toConv= 0x5555555555555555ULL;  // u64 max / u2 max (3)=            0x5555555555555555
  static const uint64_t _4toConv= 0x1111111111111111ULL;  // u64 max / u4 max (15)=           0x1111111111111111
  static const uint64_t _8toConv= 0x101010101010101ULL;   // u64 max / u8 max (255)=          0x101010101010101
  static const uint64_t _16toConv= 0x1000100010001ULL;    // u64 max / u16 max (65535)=       0x1000100010001
  static const uint64_t _32toConv= 0x100000001ULL;        // u64 max / u32 max (‭4294967295‬)=  0x100000001
//const uint64_t _10toConv= 0xFFFFFFFFFFFFFFFFui64/ 0x3FFui64;
                     
// const uint16_t _8to16= 257; u16 max/ 255= 257




  static void from8(uint8_t *from, _FormatData *f, uint8_t unused2, _Img::RGBAconv *ret) {
    // u64 max / 255= 0x‭101010101010101‬
    ret->v[0]=                   from[0]* _8toConv;
    ret->v[1]= (f->nchannels> 1? from[1]* _8toConv: 0);
    ret->v[2]= (f->nchannels> 2? from[2]* _8toConv: 0);
    ret->v[3]= (f->nchannels> 3? from[3]* _8toConv: 0);
  
    /*
    ret->g= ret->b= ret->r= from[0]* 257; /// scaling: 255(max RGBA32) * 257 = 65535 (max RGBA64)
    ret->a= 65535;
    */
  }

  static void from16(uint8_t *from, _FormatData *f, uint8_t unused2, _Img::RGBAconv *ret) {
    // u64 max/ 65535= 0x1000100010001
    ret->v[0]=                   ((uint16_t *)from)[0]* _16toConv;
    ret->v[1]= (f->nchannels> 1? ((uint16_t *)from)[1]* _16toConv: 0);
    ret->v[2]= (f->nchannels> 2? ((uint16_t *)from)[2]* _16toConv: 0);
    ret->v[3]= (f->nchannels> 3? ((uint16_t *)from)[3]* _16toConv: 0);
  }

  static void from32(uint8_t *from, _FormatData *f, uint8_t unused2, _Img::RGBAconv *ret) {
    ret->v[0]=                   ((uint32_t *)from)[0]* _32toConv;
    ret->v[1]= (f->nchannels> 1? ((uint32_t *)from)[1]* _32toConv: 0);
    ret->v[2]= (f->nchannels> 2? ((uint32_t *)from)[2]* _32toConv: 0);
    ret->v[3]= (f->nchannels> 3? ((uint32_t *)from)[3]* _32toConv: 0);
  }

  static void from64(uint8_t *from, _FormatData *f, uint8_t unused2, _Img::RGBAconv *ret) {
    ret->v[0]=                   ((uint64_t *)from)[0];
    ret->v[1]= (f->nchannels> 1? ((uint64_t *)from)[1]: 0);
    ret->v[2]= (f->nchannels> 2? ((uint64_t *)from)[2]: 0);
    ret->v[3]= (f->nchannels> 3? ((uint64_t *)from)[3]: 0);
  }

  static void from44(uint8_t *from, _FormatData *f, uint8_t unused2, RGBAconv *ret) {
    ret->r= ((*from)>> 4)*  _4toConv;
    ret->g= ((*from)& 0xF)* _4toConv;
    ret->b= 0;
    ret->a= 0;
  }

  static void from4444(uint8_t *from, _FormatData *f, uint8_t unused2, RGBAconv *ret) {
    uint16_t *p16= (uint16_t *)from;
    ret->r= (*p16>> 12)* _4toConv;
    ret->g= ((*p16>> 8)& 0xF)* _4toConv;
    ret->b= ((*p16>> 4)& 0xF)* _4toConv;
    ret->a= (*p16& 0xF)* _4toConv;
  }

  static void from565(uint8_t *from, _FormatData *f, uint8_t unused2, RGBAconv *ret) {
    uint16_t *p16= (uint16_t *)from;
    ret->r= ((( (uint64_t)*p16>> 11)*       65535)/ 31)* _16toConv;   /// scaling: RGB565 R max is 31, R16G16B16 max is 65535
    ret->g= (((((uint64_t)*p16>> 5)& 0x3F)* 65535)/ 63)* _16toConv;   /// scaling: RGB565 G max is 63, R16G16B16 max is 65535
    ret->b= ((( (uint64_t)*p16     & 0x1F)* 65535)/ 31)* _16toConv;
    ret->a= ~0ull;
  }

  static void from5551(uint8_t *from, _FormatData *f, uint8_t unused2, RGBAconv *ret) {
    uint16_t *p16= (uint16_t *)from;
    ret->r= ((( (uint64_t)*p16>> 11)*       65535)/ 31)* _16toConv;   /// scaling: RGB565 R max is 31, R16G16B16 max is 65535
    ret->g= (((((uint64_t)*p16>> 6)& 0x1F)* 65535)/ 31)* _16toConv;
    ret->b= (((((uint64_t)*p16>> 1)& 0x1F)* 65535)/ 31)* _16toConv;
    ret->a= (*p16& 0x01? ~0ull: 0);
  }

  static void from1555(uint8_t *from, _FormatData *f, uint8_t unused2, RGBAconv *ret) {
    uint16_t *p16= (uint16_t *)from;
    ret->r= (((((uint64_t)*p16>> 10)& 0x1F)* 0xFFFF)/ 0x1F)* _16toConv;   /// scaling: RGB565 R max is 31, R16G16B16 max is 65535
    ret->g= (((((uint64_t)*p16>> 5)&  0x1F)* 0xFFFF)/ 0x1F)* _16toConv;
    ret->b= (((((uint64_t)*p16)&      0x1F)* 0xFFFF)/ 0x1F)* _16toConv;
    ret->a= (*p16>> 15? ~0ull: 0);
  }

  static void from2101010(uint8_t *from, _FormatData *f, uint8_t unused2, RGBAconv *ret) {
    uint32_t *p32= (uint32_t *)from;
    ret->r= (((((uint64_t)*p32>> 20)& 0x3FF)* 0xFFFFFFFF)/ 0x3FF)* _32toConv;
    ret->g= (((((uint64_t)*p32>> 10)& 0x3FF)* 0xFFFFFFFF)/ 0x3FF)* _32toConv;    //4294967295 i32 max (unsigned)
    ret->b= (((((uint64_t)*p32)     & 0x3FF)* 0xFFFFFFFF)/ 0x3FF)* _32toConv;
    ret->a= (((uint64_t)*p32>> 30)& 0x3)* _2toConv;
  }

  static void fromCMAP_1(uint8_t *from, _FormatData *f, uint8_t bitPos, RGBAconv *ret) {
    uint8_t n= (*from>> (7- bitPos))& 0x01;
    uint8_t *p= &_cmap[n* (f->bpp/ 8)];

    if(f->bpp== 16)
      from5551(p, 0, 0, ret);
    else
      from8(p, f, 0, ret);
  }

  static void fromCMAP_2(uint8_t *from, _FormatData *f, uint8_t bitPos, RGBAconv *ret) {
    uint8_t n= (*from>> (6- bitPos))& 0x03;
    uint8_t *p= &_cmap[n* (f->bpp/ 8)];

    if(f->bpp== 16)
      from5551(p, 0, 0, ret);
    else
      from8(p, f, 0, ret);
  }

  static void fromCMAP_4(uint8_t *from, _FormatData *f, uint8_t bitPos, RGBAconv *ret) {
    uint8_t n= (*from>> (4- bitPos))& 0x0f;
    uint8_t *p= &_cmap[n* (f->bpp/ 8)];
    if(f->bpp== 16)
      from5551(p, 0, 0, ret);
    else
      from8(p, f, 0, ret);
  }

  static void fromCMAP_8(uint8_t *from, _FormatData *f, uint8_t unused2, RGBAconv *ret) {
    uint8_t *p= _cmap+ from[0]* (f->bpp/ 8);

    if(f->bpp== 16)
      from5551(p, 0, 0, ret);
    else
      from8(p, f, 0, ret);

    /*
    uint16_t *p16;
    if(_cmapBpp== 16) { // R5G5B5A1
      p16= (uint16_t *)p;
      ret->r= (((uint64_t)(*p16>> 11)*        65535)/ 31)* _16toConv;
      ret->g= (((uint64_t)((*p16>> 6)& 0x1F)* 65535)/ 31)* _16toConv;
      ret->b= (((uint64_t)((*p16>> 1)& 0x1F)* 65535)/ 31)* _16toConv;
      ret->a= (*p16& 0x01? ~0ui64: 0);

    } else {
      ret->r= p[0]* _8toConv;
      ret->g= p[1]* _8toConv;
      ret->b= p[2]* _8toConv;
      ret->a= _cmapBpp== 32? p[3]* _8toConv: ~0ui64;
    }
    */
  }

  /*
  void fromGREY_1(uint8_t *from, int8_t bitPos, _RGBA64 *ret) {
    ret->r= ret->g= ret->b= ((*from>> (7- bitPos))& 0x01)* 65535;   /// scaling: 0 or 65535
    ret->a= 65535;
  }

  void fromGREY_2(uint8_t *from, int8_t bitPos, _RGBA64 *ret) {
    ret->r= ret->g= ret->b= ((*from>> (6- bitPos))& 0x03)* 21845;   /// scaling: 65535 / 3(max val)= 21845
    ret->a= 65535;
  }

  void fromGREY_4(uint8_t *from, int8_t bitPos, _RGBA64 *ret) {
    ret->r= ret->g= ret->b= ((*from>> (4- bitPos))& 0x0F)* 4369;    /// scaling: 65535 / 15(max val)= 4369
    ret->a= 65535;
  }
  */


  // TO ================================================



  static void to8(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    //for(unsigned int a= 0; a< f->nchannels; a++)
    //  ret[a]= (uint8_t)(from->v[a]/ _8toConv);
                        ret[0]= (uint8_t)(from->r/ _8toConv);
    if(f->nchannels> 1) ret[1]= (uint8_t)(from->g/ _8toConv);
    if(f->nchannels> 2) ret[2]= (uint8_t)(from->b/ _8toConv);
    if(f->nchannels> 3) ret[3]= (uint8_t)(from->a/ _8toConv);
  }


  static void to16(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
                        ((uint16_t*)ret)[0]= (uint16_t)(from->r/ _16toConv);
    if(f->nchannels> 1) ((uint16_t*)ret)[1]= (uint16_t)(from->g/ _16toConv);
    if(f->nchannels> 2) ((uint16_t*)ret)[2]= (uint16_t)(from->b/ _16toConv);
    if(f->nchannels> 3) ((uint16_t*)ret)[3]= (uint16_t)(from->a/ _16toConv);
  }

  static void to32(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {

                        ((uint32_t*)ret)[0]= (uint32_t)(from->r/ _32toConv);
    if(f->nchannels> 1) ((uint32_t*)ret)[1]= (uint32_t)(from->g/ _32toConv);
    if(f->nchannels> 2) ((uint32_t*)ret)[2]= (uint32_t)(from->b/ _32toConv);
    if(f->nchannels> 3) ((uint32_t*)ret)[3]= (uint32_t)(from->a/ _32toConv);
  }

  static void to64(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
                        ((uint64_t*)ret)[0]= from->r;
    if(f->nchannels> 1) ((uint64_t*)ret)[1]= from->g;
    if(f->nchannels> 2) ((uint64_t*)ret)[2]= from->b;
    if(f->nchannels> 3) ((uint64_t*)ret)[3]= from->a;
  }


  static void to44(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    uint64_t r= from->r/ _4toConv;
    uint64_t g= from->g/ _4toConv;
    *ret= (uint8_t)((r<< 4)+ g);
  }

  static void to4444(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    uint64_t r= from->r/ _4toConv;
    uint64_t g= from->g/ _4toConv;
    uint64_t b= from->b/ _4toConv;
    uint64_t a= from->a/ _4toConv;
    *((uint16_t *)ret)= (uint16_t)((r<< 12)+ (g<< 8)+ (b<< 4)+ a);
  }

  static void to565(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    uint64_t r= ((from->r/ _16toConv)* 0x1F)/ 0xFFFF; // 0x1F(31) r5 max
    uint64_t g= ((from->g/ _16toConv)* 0x3F)/ 0xFFFF; // 0x3F     g6 max
    uint64_t b= ((from->b/ _16toConv)* 0x1F)/ 0xFFFF;
    *((uint16_t *)ret)= (uint16_t)((r<< 11)+ (g<< 5)+ b);
  }

  static void to5551(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    uint64_t r= ((from->r/ _16toConv)* 0x1F)/ 0xFFFF; // 0x1F(31) r5 max
    uint64_t g= ((from->g/ _16toConv)* 0x1F)/ 0xFFFF;
    uint64_t b= ((from->b/ _16toConv)* 0x1F)/ 0xFFFF;
    uint64_t a= (from->a? 1: 0);
    *((uint16_t *)ret)= (uint16_t)((r<< 11)+ (g<< 6)+ (b<< 1)+ a);
  }

  static void to1555(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    uint64_t r= ((from->r/ _16toConv)* 0x1F)/ 0xFFFF; // 0x1F(31) r5 max
    uint64_t g= ((from->g/ _16toConv)* 0x1F)/ 0xFFFF;
    uint64_t b= ((from->b/ _16toConv)* 0x1F)/ 0xFFFF;
    uint64_t a= (from->a? 1: 0);
    *((uint16_t *)ret)= (uint16_t)((a<< 15)+ (r<< 10)+ (g<< 5)+ b);
  }

  static void to2101010(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    uint64_t r= ((from->r/ _32toConv)* 0x3FF)/ 0xFFFFFFFF; // 0x3FF r10 max
    uint64_t g= ((from->r/ _32toConv)* 0x3FF)/ 0xFFFFFFFF;
    uint64_t b= ((from->r/ _32toConv)* 0x3FF)/ 0xFFFFFFFF;
    uint64_t a= from->a/ _2toConv;
    *((uint32_t *)ret)= (uint32_t)((a<< 30)+ (r<< 20)+ (g<< 10)+ b);
  }


  // finds the closest CMAP for the provided color
  static uint8_t findCMAP(RGBAconv *from, unsigned int CMAPsize, uint8_t CMAPbpp) {
    uint8_t cmapID= 0;
    uint16_t _r= (uint16_t)(from->r/ _16toConv);
    uint16_t _g= (uint16_t)(from->g/ _16toConv);
    uint16_t _b= (uint16_t)(from->b/ _16toConv);
    uint16_t _a= (uint16_t)(from->a/ _16toConv);

    int32_t diffR, diffG, diffB, diffA;
    diffR= diffG= diffB= diffA= 65535;

    if(CMAPbpp== 16) {
      for(unsigned int a= 0; a< CMAPsize; a++) {
        uint16_t col= ((uint16_t *)_cmap)[a];
        /// smallest difference in color; differenceR+ diffG+ diffB+ diffA < source diffR+ diffG+ diffB+ diffA
        if(abs64((((int64_t)Img::getR5551(col)* 65535)/ 31)- (int64_t)(_r))+ /// scaling: 31(RGBA16 max) - 65535(RGBA64 max)
           abs64((((int64_t)Img::getG5551(col)* 65535)/ 31)- (int64_t)(_g))+ 
           abs64((((int64_t)Img::getB5551(col)* 65535)/ 31)- (int64_t)(_b))+ 
           abs64(  (int64_t)Img::getA5551(col)* 65535-       (int64_t)(_a)) < 
           (int64_t)(diffR+ diffG+ diffB+ diffA)) {
          cmapID= a;

          diffR= ((int32_t)Img::getR5551(col)* 65535)/ 31- (int32_t)(_r);
          diffG= ((int32_t)Img::getG5551(col)* 65535)/ 31- (int32_t)(_g);
          diffB= ((int32_t)Img::getB5551(col)* 65535)/ 31- (int32_t)(_b);
          diffA=  (int32_t)Img::getA5551(col)* 65535-      (int32_t)(_a);
        }
      } /// for each cmap index
    } else {
      for(unsigned int a= 0; a< CMAPsize; a++) {
        uint8_t *p= &_cmap[a* (CMAPbpp/ 8)];

        /// smallest difference in color; differenceR+ diffG+ diffB+ diffA < source diffR+ diffG+ diffB+ diffA
        if(abs64((int64_t)(p[0])* 257- (int64_t)_r)+ /// scaling: 255(RGBA32 max) - 65535(RGBA64 max); 255* 257= 65535
           abs64((int64_t)(p[1])* 257- (int64_t)_g)+
           abs64((int64_t)(p[2])* 257- (int64_t)_b)+
           abs64((CMAPbpp== 32? (int64_t)(p[3])* 257: 65535)- (int64_t)_a) <
           (int64_t)(diffR+ diffG+ diffB+ diffA)) {

          cmapID= a;
          diffR= (int32_t)abs64((int64_t)(p[0])* 257- (int64_t)(_r));
          diffG= (int32_t)abs64((int64_t)(p[1])* 257- (int64_t)(_g));
          diffB= (int32_t)abs64((int64_t)(p[2])* 257- (int64_t)(_b));
          diffA= (int32_t)abs64((CMAPbpp== 32? (int64_t)(p[3])* 257: 65535- (int64_t)(_a)));
        }
      }
    }
    return cmapID;
  }

  // NOT TESTED
  static void toCMAP_1(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t bitPos) {
    uint8_t cmapID= findCMAP(from, 2, (uint8_t)f->bpp);

    *ret&= ~(1<< (7- bitPos));    /// clear bitPos
    cmapID<<= (7- bitPos);
    *ret+= cmapID;                /// set that bit to cmapID
  }

  static void toCMAP_2(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t bitPos) {
    uint8_t cmapID= findCMAP(from, 4, (uint8_t)f->bpp);
  
    *ret&= ~(1<< (7- bitPos));      /// clear bitPos 1
    *ret&= ~(1<< (7- (bitPos+ 1))); /// clear bitPos 2
    cmapID<<= (6- bitPos);
    *ret+= cmapID;                  /// set that bit to cmapID
  }


  static void toCMAP_4(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t bitPos) {
    uint8_t cmapID= findCMAP(from, 16, (uint8_t)f->bpp);

    *ret&= ~(1<< (7- bitPos));        /// clear bitPos 1
    *ret&= ~(1<< (7- (bitPos+ 1)));   /// clear bitPos 2
    *ret&= ~(1<< (7- (bitPos+ 2)));   /// clear bitPos 3
    *ret&= ~(1<< (7- (bitPos+ 3)));   /// clear bitPos 4
    cmapID<<= (4- bitPos);
    *ret+= cmapID;                    /// set that bit to cmapID
  }

  // not tested
  static void toCMAP_8(RGBAconv *from, uint8_t *ret, _FormatData *f, uint8_t unused2) {
    uint8_t cmapID= findCMAP(from, 256, (uint8_t)f->bpp);
    *ret= cmapID;
  }


  /*
  void toGREY_1(RGBAconv *from, uint8_t *ret, uint8_t bitPos) {
    uint32_t c= ((from->r+ from->g+ from->b)? 1: 0)<< (7- bitPos);
    *ret&= ~(1 << (7- bitPos))+ c;
  }

  void toGREY_2(RGBAconv *from, uint8_t *ret, uint8_t bitPos) {
    uint32_t c= (from->r+ from->g+ from->b)/ 3; /// average number between R, G and B
    c= (c* 3)/ 65535;                        /// scaling: 3(GREY2 max) - 65535 (RGBA64 max)
    *ret&= ~(1 << (7- bitPos));              /// clear bit 1
    *ret&= ~(1 << (7- (bitPos+ 1)));         /// clear bit 2
    *ret+= (uint8_t)c;
  }

  void toGREY_4(RGBAconv *from, uint8_t *ret, uint8_t bitPos) {
    uint32_t c= (from->r+ from->g+ from->b)/ 3; /// average number between R, G and B
    c= (c* 3)/ 65535;                        /// scaling: 3(GREY2 max) - 65535 (RGBA64 max)
    *ret&= ~(1 << (7- bitPos));              /// clear bit 1
    *ret&= ~(1 << (7- (bitPos+ 1)));         /// clear bit 2
    *ret&= ~(1 << (7- (bitPos+ 2)));         /// clear bit 3
    *ret&= ~(1 << (7- (bitPos+ 3)));         /// clear bit 4
    *ret+= (uint8_t)c;
  }
  */

  public:
  _Img() {
    #ifdef IMG_CLASS_USE_OPENGL
    setGlData();
    #endif
  }

} ; /// _Img private class


// static vars
uint8_t *_Img::_cmap;



_Img::_FormatData *_Img::_FormatData::get(ImgFormat in_f) {
  // simple match for first 184
  if((uint32_t)in_f<= 184) return (_FormatData *)&_Img::_formats[(unsigned int)in_f];

  // cmap formats range
  if(in_f>= ImgFormat::CMAP_START_RANGE&& in_f<= ImgFormat::CMAP_END_RANGE)
    return (_FormatData *)&_Img::_formats[(int)ImgFormat::CMAP_START_INDEX+ ((int)in_f- (int)ImgFormat::CMAP_START_RANGE)];

  return nullptr;
  // future formats must be placed here
}



#ifdef IMG_CLASS_USE_OPENGL
_Img::_FormatData *_Img::_FormatData::getGl(GLint in_f) {
  for(unsigned int a= 1; _Img::_formats[a].format!= ImgFormat::UNDEFINED; a++)
    if(_Img::_formats[a].glFormat== in_f)
      return (_FormatData *)&_Img::_formats[a];

  return (_FormatData *)&_Img::_formats[0];
}
#endif


#ifdef IMG_CLASS_USE_OPENGL
#define GLPART , GL_NONE, GL_NONE, GL_NONE
#else
#define GLPART  
#endif

//#define _I (uint8_t)Img::Swizzle::IDENTITY
#define _R 0 // (uint8_t)Img::Swizzle::R
#define _G 1 // (uint8_t)Img::Swizzle::G
#define _B 2 // (uint8_t)Img::Swizzle::B
#define _A 3 // (uint8_t)Img::Swizzle::A
_Img::_FormatData _Img::_formats[]= {
  //                                 bpp    bpc             swizzle     nch compr conv funcs
  { ImgFormat::UNDEFINED,             0,  {0, 0, 0, 0}, {_R, _G, _B, _A}, 0, 0, nullptr, nullptr GLPART},
  { ImgFormat::R4G4_UNORM_PACK8,      8,  {4, 4, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from44, _Img::to44 GLPART},
  { ImgFormat::R4G4B4A4_UNORM_PACK16, 16, {4, 4, 4, 4}, {_R, _G, _B, _A}, 4, 0, _Img::from4444, _Img::to4444 GLPART},
  { ImgFormat::B4G4R4A4_UNORM_PACK16, 16, {4, 4, 4, 4}, {_B, _G, _R, _A}, 4, 0, _Img::from4444, _Img::to4444 GLPART},
  { ImgFormat::R5G6B5_UNORM_PACK16,   16, {5, 6, 5, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from565, _Img::to565 GLPART},
  { ImgFormat::B5G6R5_UNORM_PACK16,   16, {5, 6, 5, 0}, {_B, _G, _R, _A}, 3, 0, _Img::from565, _Img::to565 GLPART},
  { ImgFormat::R5G5B5A1_UNORM_PACK16, 16, {5, 5, 5, 1}, {_R, _G, _B, _A}, 4, 0, _Img::from5551, _Img::to5551 GLPART},
  { ImgFormat::B5G5R5A1_UNORM_PACK16, 16, {5, 5, 5, 1}, {_B, _G, _R, _A}, 4, 0, _Img::from5551, _Img::to5551 GLPART},
  { ImgFormat::A1R5G5B5_UNORM_PACK16, 16, {1, 5, 5, 5}, {_A, _R, _G, _B}, 4, 0, _Img::from1555, _Img::to1555 GLPART},

  { ImgFormat::R8_UNORM,              8, {8, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8_SNORM,              8, {8, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8_USCALED,            8, {8, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8_SSCALED,            8, {8, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8_UINT,               8, {8, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8_SINT,               8, {8, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8_SRGB,               8, {8, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, nullptr, nullptr GLPART},

  { ImgFormat::R8G8_UNORM,            16, {8, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8_SNORM,            16, {8, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8_USCALED,          16, {8, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8_SSCALED,          16, {8, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8_UINT,             16, {8, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8_SINT,             16, {8, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8_SRGB,             16, {8, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, nullptr, nullptr GLPART},
  { ImgFormat::R8G8B8_UNORM,          24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8_SNORM,          24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8_USCALED,        24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8_SSCALED,        24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8_UINT,           24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8_SINT,           24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8_SRGB,           24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, nullptr, nullptr GLPART},
  { ImgFormat::B8G8R8_UNORM,          24, {8, 8, 8, 0}, {_B, _G, _R, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8_SNORM,          24, {8, 8, 8, 0}, {_B, _G, _R, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8_USCALED,        24, {8, 8, 8, 0}, {_B, _G, _R, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8_SSCALED,        24, {8, 8, 8, 0}, {_B, _G, _R, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8_UINT,           24, {8, 8, 8, 0}, {_B, _G, _R, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8_SINT,           24, {8, 8, 8, 0}, {_B, _G, _R, _A}, 3, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8_SRGB,           24, {8, 8, 8, 0}, {_B, _G, _R, _A}, 3, 0, nullptr, nullptr GLPART},

  { ImgFormat::R8G8B8A8_UNORM,          32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8A8_SNORM,          32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8A8_USCALED,        32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8A8_SSCALED,        32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8A8_UINT,           32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8A8_SINT,           32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::R8G8B8A8_SRGB,           32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, nullptr, nullptr GLPART},
  { ImgFormat::B8G8R8A8_UNORM,          32, {8, 8, 8, 8}, {_B, _G, _R, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8A8_SNORM,          32, {8, 8, 8, 8}, {_B, _G, _R, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8A8_USCALED,        32, {8, 8, 8, 8}, {_B, _G, _R, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8A8_SSCALED,        32, {8, 8, 8, 8}, {_B, _G, _R, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8A8_UINT,           32, {8, 8, 8, 8}, {_B, _G, _R, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8A8_SINT,           32, {8, 8, 8, 8}, {_B, _G, _R, _A}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::B8G8R8A8_SRGB,           32, {8, 8, 8, 8}, {_B, _G, _R, _A}, 4, 0, nullptr, nullptr GLPART},
  { ImgFormat::A8B8G8R8_UNORM_PACK32,   32, {8, 8, 8, 8}, {_A, _B, _G, _R}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::A8B8G8R8_SNORM_PACK32,   32, {8, 8, 8, 8}, {_A, _B, _G, _R}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::A8B8G8R8_USCALED_PACK32, 32, {8, 8, 8, 8}, {_A, _B, _G, _R}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::A8B8G8R8_SSCALED_PACK32, 32, {8, 8, 8, 8}, {_A, _B, _G, _R}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::A8B8G8R8_UINT_PACK32,    32, {8, 8, 8, 8}, {_A, _B, _G, _R}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::A8B8G8R8_SINT_PACK32,    32, {8, 8, 8, 8}, {_A, _B, _G, _R}, 4, 0, _Img::from8, _Img::to8 GLPART},
  { ImgFormat::A8B8G8R8_SRGB_PACK32,    32, {8, 8, 8, 8}, {_A, _B, _G, _R}, 4, 0, nullptr, nullptr GLPART},

  { ImgFormat::A2R10G10B10_UNORM_PACK32,    32, {2, 10, 10, 10}, {_A, _R, _G, _B}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2R10G10B10_SNORM_PACK32,    32, {2, 10, 10, 10}, {_A, _R, _G, _B}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2R10G10B10_USCALED_PACK32,  32, {2, 10, 10, 10}, {_A, _R, _G, _B}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2R10G10B10_SSCALED_PACK32,  32, {2, 10, 10, 10}, {_A, _R, _G, _B}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2R10G10B10_UINT_PACK32,     32, {2, 10, 10, 10}, {_A, _R, _G, _B}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2R10G10B10_SINT_PACK32,     32, {2, 10, 10, 10}, {_A, _R, _G, _B}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2B10G10R10_UNORM_PACK32,    32, {2, 10, 10, 10}, {_A, _B, _G, _R}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2B10G10R10_SNORM_PACK32,    32, {2, 10, 10, 10}, {_A, _B, _G, _R}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2B10G10R10_USCALED_PACK32,  32, {2, 10, 10, 10}, {_A, _B, _G, _R}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2B10G10R10_SSCALED_PACK32,  32, {2, 10, 10, 10}, {_A, _B, _G, _R}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2B10G10R10_UINT_PACK32,     32, {2, 10, 10, 10}, {_A, _B, _G, _R}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},
  { ImgFormat::A2B10G10R10_SINT_PACK32,     32, {2, 10, 10, 10}, {_A, _B, _G, _R}, 4, 0, _Img::from2101010, _Img::to2101010 GLPART},

  { ImgFormat::R16_UNORM,   16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16_SNORM,   16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16_USCALED, 16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16_SSCALED, 16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16_UINT,    16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16_SINT,    16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16_SFLOAT,  16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, nullptr, nullptr GLPART},
  { ImgFormat::R16G16_UNORM,    32, {16, 16, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16_SNORM,    32, {16, 16, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16_USCALED,  32, {16, 16, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16_SSCALED,  32, {16, 16, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16_UINT,     32, {16, 16, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16_SINT,     32, {16, 16, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16_SFLOAT,   32, {16, 16, 0, 0}, {_R, _G, _B, _A}, 2, 0, nullptr, nullptr GLPART},
  { ImgFormat::R16G16B16_UNORM,       48, {16, 16, 16, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16_SNORM,       48, {16, 16, 16, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16_USCALED,     48, {16, 16, 16, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16_SSCALED,     48, {16, 16, 16, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16_UINT,        48, {16, 16, 16, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16_SINT,        48, {16, 16, 16, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16_SFLOAT,      48, {16, 16, 16, 0}, {_R, _G, _B, _A}, 3, 0, nullptr, nullptr GLPART},
  { ImgFormat::R16G16B16A16_UNORM,    64, {16, 16, 16, 16}, {_R, _G, _B, _A}, 4, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16A16_SNORM,    64, {16, 16, 16, 16}, {_R, _G, _B, _A}, 4, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16A16_USCALED,  64, {16, 16, 16, 16}, {_R, _G, _B, _A}, 4, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16A16_SSCALED,  64, {16, 16, 16, 16}, {_R, _G, _B, _A}, 4, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16A16_UINT,     64, {16, 16, 16, 16}, {_R, _G, _B, _A}, 4, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16A16_SINT,     64, {16, 16, 16, 16}, {_R, _G, _B, _A}, 4, 0, _Img::from16, _Img::to16 GLPART},
  { ImgFormat::R16G16B16A16_SFLOAT,   64, {16, 16, 16, 16}, {_R, _G, _B, _A}, 4, 0, nullptr, nullptr GLPART},

  { ImgFormat::R32_UINT,              32, {32, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32_SINT,              32, {32, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32_SFLOAT,            32, {32, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, nullptr,      nullptr    GLPART},
  { ImgFormat::R32G32_UINT,           64, {32, 32, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32G32_SINT,           64, {32, 32, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32G32_SFLOAT,         64, {32, 32, 0, 0}, {_R, _G, _B, _A}, 2, 0, nullptr, nullptr GLPART},
  { ImgFormat::R32G32B32_UINT,        96, {32, 32, 32, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32G32B32_SINT,        96, {32, 32, 32, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32G32B32_SFLOAT,      96, {32, 32, 32, 0}, {_R, _G, _B, _A}, 3, 0, nullptr, nullptr GLPART},
  { ImgFormat::R32G32B32A32_UINT,     128, {32, 32, 32, 32}, {_R, _G, _B, _A}, 4, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32G32B32A32_SINT,     128, {32, 32, 32, 32}, {_R, _G, _B, _A}, 4, 0, _Img::from32, _Img::to32 GLPART},
  { ImgFormat::R32G32B32A32_SFLOAT,   128, {32, 32, 32, 32}, {_R, _G, _B, _A}, 4, 0, nullptr, nullptr GLPART},

  { ImgFormat::R64_UINT,              64, {64, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64_SINT,              64, {64, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64_SFLOAT,            64, {64, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, nullptr, nullptr GLPART},
  { ImgFormat::R64G64_UINT,           128, {64, 64, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64G64_SINT,           128, {64, 64, 0, 0}, {_R, _G, _B, _A}, 2, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64G64_SFLOAT,         128, {64, 64, 0, 0}, {_R, _G, _B, _A}, 2, 0, nullptr, nullptr GLPART},
  { ImgFormat::R64G64B64_UINT,        192, {64, 64, 64, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64G64B64_SINT,        192, {64, 64, 64, 0}, {_R, _G, _B, _A}, 3, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64G64B64_SFLOAT,      192, {64, 64, 64, 0}, {_R, _G, _B, _A}, 3, 0, nullptr, nullptr GLPART},
  { ImgFormat::R64G64B64A64_UINT,     256, {64, 64, 64, 64}, {_R, _G, _B, _A}, 4, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64G64B64A64_SINT,     256, {64, 64, 64, 64}, {_R, _G, _B, _A}, 4, 0, _Img::from64, _Img::to64 GLPART},
  { ImgFormat::R64G64B64A64_SFLOAT,   256, {64, 64, 64, 64}, {_R, _G, _B, _A}, 4, 0, nullptr, nullptr GLPART},

  { ImgFormat::B10G11R11_UFLOAT_PACK32, 32, {10, 11, 11, 0}, {_B, _G, _R, _A}, 3, 0, nullptr, nullptr GLPART},
  { ImgFormat::E5B9G9R9_UFLOAT_PACK32,  32, {5,  9,  9,  9}, {_A, _B, _G, _R}, 4, 0, nullptr, nullptr GLPART},
  // depth/stencil
  { ImgFormat::D16_UNORM,               16, {16, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, nullptr, nullptr GLPART},
  { ImgFormat::X8_D24_UNORM_PACK32,     32, {24, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, nullptr, nullptr GLPART},
  { ImgFormat::D32_SFLOAT,              32, {32, 0, 0, 0}, {_R, _G, _B, _A}, 1, 0, nullptr, nullptr GLPART},
  { ImgFormat::S8_UINT,                 8,  {8, 0, 0, 0},  {_R, _G, _B, _A}, 1, 0, nullptr, nullptr GLPART},
  { ImgFormat::D16_UNORM_S8_UINT,       24, {16, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, nullptr, nullptr GLPART},
  { ImgFormat::D24_UNORM_S8_UINT,       32, {24, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, nullptr, nullptr GLPART},
  { ImgFormat::D32_SFLOAT_S8_UINT,      40, {32, 8, 0, 0}, {_R, _G, _B, _A}, 2, 0, nullptr, nullptr GLPART},
  // BC1-7 compressed
  { ImgFormat::BC1_RGB_UNORM_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 3, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC1_RGB_SRGB_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 3, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC1_RGBA_UNORM_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC1_RGBA_SRGB_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC2_UNORM_BLOCK,       0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC2_SRGB_BLOCK,        0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC3_UNORM_BLOCK,       0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC3_SRGB_BLOCK,        0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC4_UNORM_BLOCK,       0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 1, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC4_SNORM_BLOCK,       0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 1, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC5_UNORM_BLOCK,       0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 2, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC5_SNORM_BLOCK,       0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 2, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC6H_UFLOAT_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 3, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC6H_SFLOAT_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 3, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC7_UNORM_BLOCK,       0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::BC7_SRGB_BLOCK,        0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  // ETC2 compressed
  { ImgFormat::ETC2_R8G8B8_UNORM_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 3, 1, nullptr, nullptr GLPART},
  { ImgFormat::ETC2_R8G8B8_SRGB_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 3, 1, nullptr, nullptr GLPART},
  { ImgFormat::ETC2_R8G8B8A1_UNORM_BLOCK, 0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ETC2_R8G8B8A1_SRGB_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ETC2_R8G8B8A8_UNORM_BLOCK, 0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ETC2_R8G8B8A8_SRGB_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  // EAC compressed
  { ImgFormat::EAC_R11_UNORM_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 1, 1, nullptr, nullptr GLPART},
  { ImgFormat::EAC_R11_SNORM_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 1, 1, nullptr, nullptr GLPART},
  { ImgFormat::EAC_R11G11_UNORM_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 2, 1, nullptr, nullptr GLPART},
  { ImgFormat::EAC_R11G11_SNORM_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 2, 1, nullptr, nullptr GLPART},
  // ASTC compressed
  { ImgFormat::ASTC_4x4_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_4x4_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_5x4_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_5x4_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_5x5_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_5x5_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_6x5_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_6x5_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_6x6_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_6x6_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_8x5_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_8x5_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_8x6_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_8x6_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_8x8_UNORM_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_8x8_SRGB_BLOCK,     0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x5_UNORM_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x5_SRGB_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x6_UNORM_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x6_SRGB_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x8_UNORM_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x8_SRGB_BLOCK,    0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x10_UNORM_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_10x10_SRGB_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_12x10_UNORM_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_12x10_SRGB_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_12x12_UNORM_BLOCK,  0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},
  { ImgFormat::ASTC_12x12_SRGB_BLOCK,   0, {0, 0, 0, 0}, {_R, _G, _B, _A}, 4, 1, nullptr, nullptr GLPART},

  { ImgFormat::CMAP_RGB,                24, {8, 8, 8, 0}, {_R, _G, _B, _A}, 3, 0, _Img::fromCMAP_8, _Img::toCMAP_8 GLPART},
  { ImgFormat::CMAP_RGBA,               32, {8, 8, 8, 8}, {_R, _G, _B, _A}, 4, 0, _Img::fromCMAP_8, _Img::toCMAP_8 GLPART},

  // terminator
  { ImgFormat::UNDEFINED,             0,  {0, 0, 0, 0}, {_R, _G, _B, _A}, 0, 0, nullptr, nullptr GLPART}
  #undef _R
  #undef _G
  #undef _B
  #undef _A
};

_Img _imgClassPrivateConstructor;



































// IIIIII  MM  MM    GGGG
//   II    MMMMMM  GG
//   II    MM  MM  GG  GG
// IIIIII  MM  MM    GGGG class




Img::Img() {
  bitmap= nullptr;
  cmap= nullptr;
  _wrap= false;
  wrapBitmap= nullptr;

  err= 0;
  
  delData();
}


void Img::delData() {
  if(_wrap) {
    if(*wrapBitmap) delete[] (int8_t *)*wrapBitmap;
    *wrapBitmap= nullptr;
    bitmap= nullptr;
   
  } else {
    if(bitmap)
      delete[] (int8_t *)bitmap;
    bitmap= nullptr;
    wrapBitmap= nullptr;
  }
  
  fileName.delData();
  fileType= 0;        // TEX default
  format= ImgFormat::UNDEFINED;
  dx= dy= depth= 0;
  bpp= 0;
  nchannels= bpc[0]= bpc[1]= bpc[2]= bpc[3]= 0;
  compressed= false;

  if(cmap) { delete[] cmap; cmap= nullptr; }
  //cmapBpp= 0;
  
  //hasTrnCol= false;
  //trnCol.r= trnCol.g= trnCol.b= 0;
  packedPixels= true;

  // err= 0; SHOULD THIS WIPE THE ERROR? (IF THERE IS ONE) NOPE, I THINK
}





void Img::wrap(void **p, uint32_t _dx, uint32_t _dy, ImgFormat t) {
  delData();
  bitmap= (uint8_t *)*p;
  wrapBitmap= (uint8_t **)p;
  if(_dx) dx= _dx;
  if(_dy) dy= _dy;
  if(t== ImgFormat::UNDEFINED) {
    format= t;
    computePixelInfo();
  }

  _wrap= true;
}


void Img::stopWrapping() {
  _wrap= false;
  wrapBitmap= nullptr;
  bitmap= nullptr;

  delData();
}

inline void Img::_updateWrap(Img *img) {
  if(!img->_wrap) return;
  // USELESS DEL INCOMING
}

/// checks standard Img class variables, something is wrong, returns false
inline bool Img::_checkVars(Img *p) {
  if(p->_wrap) {
    if(!p->wrapBitmap)  p->err= 1;
    else if(!*p->wrapBitmap) p->err= 1;
  } else
    if(!p->bitmap)      p->err= 1;
  if(!p->dx|| !p->dy)   p->err= 2;
  if(!p->bpp|| !p->bpc[0]) p->err= 3;

  return p->err? false: true;
}


//RESAMPLING FUNC(s)
#define IMAGE_IDX(x,y) (((x)+(y)*width)*components)
#define NIMAGE_IDX(x,y) (((x)+(y)*newWidth)*components)

void *Img::resample2(void *image, uint32_t width, uint32_t height, uint32_t components, uint32_t &err) {
  uint32_t newWidth, newHeight;
  uint8_t *newPtr, *ptr;
  
  if(width < 1 || height < 1 || components < 1 || components > 4 || image == NULL) {
    err = 14;	// invalid input
    return NULL;
  }

  ptr = (uint8_t *)image;
  
  if(width > 2)
    newWidth = width / 2;
  else
    newWidth = 1;

  if(height > 2)
    newHeight = height / 2;
  else
    newHeight = 1;

  newPtr = new uint8_t[newWidth * newHeight * components];
  if(newPtr == NULL)
  {
    err = 12; // not enough memory
    return NULL;
  }

  for(uint32_t i = 0; i < newWidth; i++) {
    for(uint32_t j = 0; j < newHeight; j++) {
      int nidx = NIMAGE_IDX(i, j);
        
      uint32_t idx1 = IMAGE_IDX((i* 2)%    width, (j* 2)%    height);
      uint32_t idx2 = IMAGE_IDX((i* 2)%    width, (j* 2+ 1)% height);
      uint32_t idx3 = IMAGE_IDX((i* 2+ 1)% width, (j* 2+ 1)% height);
      uint32_t idx4 = IMAGE_IDX((i* 2+ 1)% width, (j* 2)%    height);

      for(uint32_t k = 0; k < components; k++){
        uint64_t x = ((uint64_t)ptr[idx1 + k] + (uint64_t)ptr[idx2 + k] + (uint64_t)ptr[idx3 + k] + (uint64_t)ptr[idx4 + k])/ 4;
        newPtr[k+ nidx] = (uint8_t)x;
      }
    }
  }
  return newPtr;
}



bool Img::areSizesPowerOfTwo() {
  /// if a size is 0, it is marked as ok, no NPOT is needed
  bool bx= (dx? false: true),
       by= (dy? false: true),
       bd= (depth? false: true);

  for(unsigned int a= 1, b= 0; b< 20; b++, a*= 2) { /// 1,048,576 max size (2^20)
    if(dx== a) bx= true;        /// width
    if(dy== a) by= true;        /// height
    if(depth== a) bd= true;     /// depth must be POT too
  }

  if((!bx) || (!by) || (!bd))
    return false;

  return true;
}


bool Img::loadPalette(const char *name, uint8_t in_bpp) {
  if((in_bpp!= 32) && (in_bpp!= 24))
    return false;

  FILE *f= fopen(name,"rb");
  if(!f) { err= 8; return false; }

  if(cmap) delete[] cmap;
  uint32_t n= 256* (in_bpp/ 8); /// cmap size in bytes, based on the file Bpp
  cmap= new uint8_t[n];

  if(fread(cmap, n, 1, f)!= 1) {
    delete[] cmap;
    cmap= nullptr;
    fclose(f);
    err= 9;
    return false;
  }
    
  fclose(f);
  err= 0;
  return true;
}


bool Img::savePalette(const char *name) {
  int cmapBpp;
  if(format== ImgFormat::CMAP_RGB) cmapBpp= 24;
  else if(format== ImgFormat::CMAP_RGBA) cmapBpp= 32;
  else return false;

  if(cmap== NULL) { err= 11; return false; }
  FILE *f= fopen(name, "wb");
  if(!f) return false;
  fwrite(cmap, 256* ((size_t)cmapBpp/ 8), 1, f);
  fclose(f);

  err= 0;
  return true;
}



bool Img::load(const char *fname) {

  
  // distinguish between a normal file load and a 3D texture load, with multiple files
  // the actual file with [fname] does not exist, but it's a base for multiple files to load fbase0000.ext
  bool is3D= false;
  FILE *f= fopen(fname, "rb");
  if(!f) is3D= true;
  else fclose(f);

  // single file load
  if(!is3D) {
    str8 s= pointFileExt(fname);
    s.lower();
    
    if(s== "png") return Img::_loadPNG(fname, this);
    if(s== "tga") return Img::_loadTGA(fname, this);

  // multiple file load (3D texture)
  } else {
    load3D(fname);
  }

  return false;
}


bool Img::save(const char *fname, int dummy) {
  str8 s= pointFileExt(fname);
  s.lower();
  
  if(s== "png") return Img::_savePNG(fname, this);
  if(s== "tga") return Img::_saveTGA(fname, this);
  return false;
}

// filename must be a base - first image on disk would be [basename0000.format]
bool Img::load3D(const char *in_fn) {
  delData();
  err= 0;
  
  /// file name
  str8 fname, base, ext, ext2;
  getFileName(in_fn, &base);
  getFileExt(in_fn, &ext);
  ext2= ext; ext2.lower();        /// ext2 will be the lowercase extension, used to check strings - the filename will be case in linux

  unsigned int len= Str::strlen8(in_fn);
  fname.d= (char *)new uint8_t[len+ 4];

  /// Img::depth - count how many images the 3D texture has
  for(depth= 0; depth< 10000; depth++) {
    sprintf(fname.d, "%s%04d.%s", base.d, depth, ext.d);
    FILE *f= fopen(fname, "rb");
    if(!f) break;
    fclose(f);
  }

  /// error checking
  if(!depth) { err= 8; return false; }
  if(ext2!= "png" || ext2!= "tga") { err= 14; return false; }
  
  Img t;
  uint32_t bsize= 0;

  for(unsigned int a= 0; a< depth; a++) { /// for each 3D image
    sprintf(fname.d, "%s%04d.%s", base.d, a, ext.d);
    if(!t.load(fname)) { delData(); err= t.err; return false; }
      
      
    if(!bitmap) { /// first time alloc / var populate
      /// unsuported image types
      if(t.format== ImgFormat::CMAP_RGBA || t.format== ImgFormat::CMAP_RGB) { delData(); err= 13; return false; }

      fileName= in_fn;
      if(ext2== "tga")       fileType= 1;
      else if(ext2== "png")  fileType= 2;
      format= t.format;
      dx= t.dx;
      dy= t.dy;
      bpp= t.bpp;
      bpc[0]= t.bpc[0];
      bpc[1]= t.bpc[1];
      bpc[2]= t.bpc[2];
      bpc[3]= t.bpc[3];
      nchannels= t.nchannels;
      packedPixels= t.packedPixels;
      //hasTrnCol= t.hasTrnCol;
      //trnCol.r= t.trnCol.r;
      //trnCol.g= t.trnCol.g;
      //trnCol.b= t.trnCol.b;
      
      if(packedPixels) bsize= (uint64_t)(((dx* bpp)/ 8)* dy)* depth;
      else             bsize= (uint64_t)((bpp< 8? dx* dy: dx* dy* (bpp/ 8)))* depth;

      if(_wrap) {
        *wrapBitmap= new uint8_t[bsize];
        bitmap= *wrapBitmap;
      } else
        bitmap=      new uint8_t[bsize];

    } /// first time alloc / var populate

    Str::memcpy((uint8_t *)bitmap+ (bsize* a), t.bitmap, bsize);
  } /// for each 3D image
  return true;
}


bool Img::save3D(const char *in_fn) {
  if(depth<= 1) { err= 2; return false; }
  if(!bitmap) { err= 1; return false; }
  err= 0;

  /// file name disection
  str8 fname, base, ext, ext2;
  getFileName(in_fn, &base);
  getFileExt(in_fn, &ext);
  ext2= ext; ext2.lower();
  if(ext2!= "png" || ext2!= "tga") { err= 14; return false; }
  unsigned int len= (unsigned int)Str::strlen8(in_fn);
  fname.d= (char *)new uint8_t[len+ 4];

  /// update this-> fileName and fileType
  fileName= in_fn;
  if(ext2== "tga")       fileType= 1;
  else if(ext2== "png")  fileType= 2;

  Img i;
  i.format= format;
  i.dx= dx;
  i.dy= dy;
  i.bpp= bpp;
  i.bpc[0]= bpc[0];
  i.bpc[1]= bpc[1];
  i.bpc[2]= bpc[2];
  i.bpc[3]= bpc[3];
  i.nchannels= nchannels;
  //i.hasTrnCol= hasTrnCol;
  //i.trnCol.r= trnCol.r;
  //i.trnCol.g= trnCol.g;
  //i.trnCol.b= trnCol.b;
  i.packedPixels= packedPixels;

  uint64_t bsize;
  if(packedPixels) bsize= (uint64_t)((dx* bpp)/ 8)* dy;
  else             bsize= (uint64_t)(bpp< 8? dx* dy: dx* dy* (bpp/ 8));


  for(unsigned int a= 0; a< depth; a++) {
    sprintf(fname.d, "%s%04d.%s", base.d, a, ext.d);
    i.bitmap= (uint8_t *)bitmap+ bsize* a;
    if(!i.save(fname)) { err= i.err; i.bitmap= nullptr; return false; }
  }

  i.bitmap= nullptr;
  return true;
}


uint32_t Img::mipmapGetMaxLevels(Img *i) {
  uint32_t a= 1;
  for(unsigned int b= MAX(MAX(i->dx, i->dy), i->depth); b!= 1; b/= 2)
    a++;
  return a;
}


void Img::computePixelInfo(ImgFormat in_f) {
  // either get the format from func parameter, or current Img::format variable
  ImgFormat tmp;
  if(in_f!= ImgFormat::UNDEFINED) tmp= in_f;
  else                            tmp= format;

  const _Img::_FormatData *p= _Img::_FormatData::get(tmp);
  bpp= p->bpp;
  bpc[0]= p->bpc[0];
  bpc[1]= p->bpc[1];
  bpc[2]= p->bpc[2];
  bpc[3]= p->bpc[3];
  nchannels= p->nchannels;
}


void Img::getPixelInfo(ImgFormat in_f, uint16_t *out_bpp, uint8_t *out_nchannels, uint8_t *out_bpc[4]) {
  const _Img::_FormatData *p= _Img::_FormatData::get(in_f);
  if(p== nullptr) return;
  if(out_bpp) *out_bpp= p->bpp;
  if(out_nchannels) *out_nchannels= p->nchannels;
  if(out_bpc) *out_bpc[0]= p->bpc[0], *out_bpc[1]= p->bpc[1], *out_bpc[2]= p->bpc[2], *out_bpc[3]= p->bpc[3];
}


bool Img::isFormatCompressed(ImgFormat in_f) {
  const _Img::_FormatData *p= _Img::_FormatData::get(in_f);
  return p->compressed;
}


void Img::_set(ImgFormat in_f) {
  _Img::_FormatData *fd= _Img::_FormatData::get(in_f);
  if(fd== nullptr) return;

  format= in_f;
  bpc[0]= fd->bpc[0];
  bpc[1]= fd->bpc[1];
  bpc[2]= fd->bpc[2];
  bpc[3]= fd->bpc[3];
  bpp= fd->bpp;
  nchannels= fd->nchannels;
  compressed= fd->compressed;
}






const char *Img::getError() {
  if(err== 0)  return "OK - No errors";
  if(err== 1)  return "Image internal data error";
  if(err== 2)  return "Image size error";
  if(err== 3)  return "Invalid BPP / BPC";
  if(err== 4)  return "Image already packed / unpacked";
  if(err== 5)  return "Image type does not support pixel pack/unpack operations (only [rgb16]/[grey1/2/4]/[cmap1/2/4])";
  if(err== 6)  return "Image type does not support channel swapping (only 1 channel?)";
  if(err== 7)  return "Channel number error (asking for channel 3 of a 2 channel image?)";
  if(err== 8)  return "Cannot open file";
  if(err== 9)  return "File read error";
  if(err== 10) return "File write error";
  if(err== 11) return "CMAP is empty / cmap expected - not found";
  if(err== 12) return "Memory allocation error";
  if(err== 13) return "CMAP palette not suported";
  if(err== 14) return "Unknown / unsupported image type";
  if(err== 15) return "Compression error";
  if(err== 16) return "Decompression error";
  if(err== 17) return "PNG header not read first";
  if(err== 18) return "PNG IHDR chunk error";
  if(err== 19) return "PNG PLTE chunk error";
  if(err== 20) return "PNG IDAT chunk error";
  if(err== 21) return "PNG raw data not filled";
  if(err== 22) return "PNG does not support this image format";

  return "Unknown error - memory corrupted? error number mistake?";
}














///==========================================================///
// CONVERSION FUNCTION ===============----------------------- //
///==========================================================///

// PACKED / UNPACKED ver !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool Img::convert(ImgFormat in_newFormat){

  /*
  1. do not bother with _SFLOAT _UFLOAT, 16/32. that's for vectors/coords, NOT images.
     if they want some conversion for that, they must do such conversion themselfs
  
       conversion funcs seem all done
       some tweaks should happen with the BGR/RGB thingie
    */

  err= 0;
  /// error checks
  if(!_checkVars(this)) return false;

  /// tmp vars used
  _Img::_FormatData *oldFormat= _Img::_FormatData::get(format);
  _Img::_FormatData *newFormat= _Img::_FormatData::get(in_newFormat);
  if(oldFormat== newFormat) return true;

  if(oldFormat->_fromFunc== nullptr) { err= 14; return false; }
  if(newFormat->_toFunc== nullptr) { err= 14; return false; }

  void *oldBitmap= (_wrap? *wrapBitmap: bitmap);
  //ImgFormat oldFormat= format;

  _Img::_cmap= cmap;                        /// global _cmap is used by fromXXX toXXX conversion funcs
  //_cmapBpp= cmapBpp;                  /// global _cmapBpp is used by fromXXX toXXX conversion funcs

  /// new vars - bpp / bpc / nchannels / type
  format= in_newFormat;
  computePixelInfo();      /// compute the new bpp, bpc and nchannels, based on the new image type

  /// image line size in bytes, 1 byte added if needed
  uint64_t oldLineSize= ((dx* oldFormat->bpp)% 8? (dx* oldFormat->bpp)/ 8+ 1: (dx* oldFormat->bpp)/ 8);
  uint64_t lineSize=    ((dx* bpp   )% 8? (dx* bpp   )/ 8+ 1: (dx* bpp   )/ 8);

  /// new bitmap alloc
  if(!_wrap)
    bitmap= new uint8_t[lineSize* dy];
  else {
    *wrapBitmap= new uint8_t[lineSize* dy];
    bitmap= *wrapBitmap;
  }

  /// pass thru all pixels and convert them
  for(uint32_t a= 0; a< dy; a++)          /// for each dy - image line
    for(uint32_t b= 0; b< dx; b++) {      /// for each dx - pixel on line a
      _Img::RGBAconv pixel;                   /// will hold the pixel value, for the transfer between oldBitmap - newBitmap
    
      uint32_t srcBytePos= (b* (uint32_t)oldFormat->bpp)/ 8;
      uint8_t srcBitPos= (uint8_t)(uint32_t)((b* (uint32_t)oldFormat->bpp)% 8);
      oldFormat->_fromFunc((uint8_t *)oldBitmap+ a* oldLineSize+ srcBytePos, oldFormat, srcBitPos, &pixel);

      uint32_t dstBytePos= (b* (uint32_t)bpp)/ 8;
      uint8_t dstBitPos= (uint8_t)(uint32_t)((b* (uint32_t)bpp)% 8);
      newFormat->_toFunc(&pixel, (uint8_t *)bitmap+ a* lineSize+ dstBytePos, newFormat, dstBitPos);
    }

  _updateWrap(this);

  /// dealloc old bitmap
  if(oldBitmap)
    delete[] (uint8_t *)oldBitmap;
  
  return true;
}


bool Img::convertTo8bpcUNORM() {
  _Img::_FormatData *p= _Img::_FormatData::get(format);
  if(p->nchannels== 1)
    return convert(ImgFormat::R8_UNORM);
  if(p->nchannels== 2)
    return convert(ImgFormat::R8G8_UNORM);
  if(p->nchannels== 3)
    return convert(ImgFormat::R8G8B8_UNORM);
  if(p->nchannels== 4)
    return convert(ImgFormat::R8G8B8A8_UNORM);
  return false;
}


bool Img::packPixels() {
  err= 0;
  /// error checks
  if(packedPixels) { err= 4; return false; }
  if(!_checkVars(this)) return false;

  /// if image is not one of the following, there is no packing possible
  if(format!= ImgFormat::R5G6B5_UNORM_PACK16 && format!= ImgFormat::B5G6R5_UNORM_PACK16 &&
     format!= ImgFormat::R5G5B5A1_UNORM_PACK16 && format!= ImgFormat::B5G5R5A1_UNORM_PACK16 && 
     format!= ImgFormat::A1R5G5B5_UNORM_PACK16 && 
     format!= ImgFormat::R4G4B4A4_UNORM_PACK16 && format!= ImgFormat::B4G4R4A4_UNORM_PACK16 &&
     format!= ImgFormat::R4G4_UNORM_PACK8) {
     //format!= ImgFormat::CMAP1_R5G5B5A1 && format!= ImgFormat::CMAP1_R8G8B8 && format!= ImgFormat::CMAP1_R8G8B8A8 &&
     //format!= ImgFormat::CMAP2_R5G5B5A1 && format!= ImgFormat::CMAP2_R8G8B8 && format!= ImgFormat::CMAP2_R8G8B8A8 &&
     //format!= ImgFormat::CMAP4_R5G5B5A1 && format!= ImgFormat::CMAP4_R8G8B8 && format!= ImgFormat::CMAP4_R8G8B8A8) {
    err= 5;
    return false;
  }

  uint8_t *oldBitmap= (uint8_t *)(_wrap? *wrapBitmap: bitmap);

  /// image line size in bytes, 1 byte added if needed
  //uint64_t oldLineSize= dx* nchannels;
  uint64_t lineSize= ((dx* bpp)% 8? ((dx* bpp)/ 8)+ 1: (dx* bpp)/ 8);

  /// new bitmap alloc
  if(!_wrap)
    bitmap= new uint8_t[lineSize* dy];
  else {
    *wrapBitmap= new uint8_t[lineSize* dy];
    bitmap= *wrapBitmap;
  }

  uint16_t n= 0;
  uint8_t nbit= 0;
  //uint8_t *p= (uint8_t *)bitmap; *p= 0;
  uint8_t *p= (uint8_t *)oldBitmap;
  uint8_t *p8;
  uint16_t *p16;

  // actual packing done in next part; some splitting was done to eliminate further checks per pixel

  // less than 8bpp formats, 1 channel only (weird cmaps)
  if(bpp< 8 && nchannels== 1) {

    p8= (uint8_t *)bitmap; *p8= 0; // clear initial byte

    for(uint32_t a= dx* dy; a> 0; a--, n++) {
        *p+= oldBitmap[n]<< (8- bpp- nbit);
        nbit+= bpp;

        /// check if reached the end of the byte
        if(nbit== 8) {
          nbit= 0;
          p++;
          *p= 0;        /// clean each byte
        }
      }

  // 8bpp formats
  } else if(bpc[0]+ bpc[1]+ bpc[2]+ bpc[3]== 8) {

    p8= (uint8_t *)bitmap;
    for(uint32_t a= dx* dy; a> 0; a--, p+= nchannels)
      *p8=     (p8[0]<< (bpc[1]+ bpc[2]+ bpc[3]))+
      (bpc[1]? (p8[1]<< (bpc[2]+ bpc[3])):     0)+
      (bpc[2]? (p8[2]<<  bpc[3]):              0)+
      (bpc[3]?  p8[3]:                         0),
      p8++;

  // 16bpp formats
  } else if(bpc[0]+ bpc[1]+ bpc[2]+ bpc[3]<= 16) {
    p16= (uint16_t *)bitmap;
    for(uint32_t a= dx* dy; a> 0; a--, p+= nchannels)
      *p16++= (((uint16_t)p[0])<< (bpc[1]+ bpc[2]+ bpc[3]))+
     (bpc[1]? (((uint16_t)p[1])<< (bpc[2]+ bpc[3])):     0)+
     (bpc[2]? (((uint16_t)p[2])<< (bpc[3])):             0)+
     (bpc[3]? (((uint16_t)p[3])):                        0);
  }

  /// dealloc old bitmap
  delete[] oldBitmap;

  packedPixels= true;
  return true;
}


bool Img::unpackPixels() {
  err= 0;
  /// error checks
  if(packedPixels) { err= 4; return false; }
  if(!_checkVars(this)) return false;

  /// if image is not one of the following, there is no packing possible
  if(format!= ImgFormat::R5G6B5_UNORM_PACK16 && format!= ImgFormat::B5G6R5_UNORM_PACK16 &&
     format!= ImgFormat::R5G5B5A1_UNORM_PACK16 && format!= ImgFormat::B5G5R5A1_UNORM_PACK16 && 
     format!= ImgFormat::A1R5G5B5_UNORM_PACK16 && 
     format!= ImgFormat::R4G4B4A4_UNORM_PACK16 && format!= ImgFormat::B4G4R4A4_UNORM_PACK16 &&
     format!= ImgFormat::R4G4_UNORM_PACK8) {
     //format!= ImgFormat::CMAP1_R5G5B5A1 && format!= ImgFormat::CMAP1_R8G8B8 && format!= ImgFormat::CMAP1_R8G8B8A8 &&
     //format!= ImgFormat::CMAP2_R5G5B5A1 && format!= ImgFormat::CMAP2_R8G8B8 && format!= ImgFormat::CMAP2_R8G8B8A8 &&
     //format!= ImgFormat::CMAP4_R5G5B5A1 && format!= ImgFormat::CMAP4_R8G8B8 && format!= ImgFormat::CMAP4_R8G8B8A8) {
    err= 5;
    return false;
  }

  uint8_t *oldBitmap= (uint8_t *)(_wrap? *wrapBitmap: bitmap);
  uint64_t lineSize= dx* nchannels;

  /// new bitmap alloc
  if(!_wrap)
    bitmap= new uint8_t[lineSize* dy];
  else {
    *wrapBitmap= new uint8_t[lineSize* dy];
    bitmap= *wrapBitmap;
  }

  uint8_t nbit= 0;
  uint8_t *p= (uint8_t *)bitmap;
  uint8_t *p8;
  uint16_t *p16;
  uint16_t mask[3];

  // less than 8bit formats
  if((bpp< 8) && (nchannels== 1)) {

    p8= (uint8_t *)oldBitmap;
    mask[0]= 0xFF>> (8- bpp);  /// [bpp= 1, mask= 1] [bpp=2, mask= 11] [bpp= 4, mask= 1111]

    for(uint32_t a= dx* dy; a> 0; a--) {
      *p++= (*p8>> (8- bpp- nbit))& mask[0];
      nbit+= bpp;
      if(nbit== 8)           /// check if reached the end of the byte
        nbit= 0, p++;
    }

  // 8bit formats
  } else if(bpc[0]+ bpc[1]+ bpc[2]+ bpc[3]== 8) {
    p8= (uint8_t *)oldBitmap;
    mask[0]= 0xFF>> (8- bpc[1]);
    mask[1]= 0xFF>> (8- bpc[2]);
    mask[2]= 0xFF>> (8- bpc[3]);

    for(uint32_t a= dx* dy; a> 0; a--, p8++) {
      *p++= (*p8)>> (bpc[1]+ bpc[2]+ bpc[3]) ;
      if(bpc[1]) *p++= ((*p8)>> (bpc[2]+ bpc[3]))& mask[0];
      if(bpc[2]) *p++= ((*p8)>>  bpc[3])&          mask[1];
      if(bpc[3]) *p++=  (*p8)&                     mask[2];
    }

  // 16bit formats
  } else if(bpc[0]+ bpc[1]+ bpc[2]+ bpc[3]<= 16) {
    p16= (uint16_t *)oldBitmap;
    mask[0]= 0xFFFF>> (16- bpc[1]);
    mask[1]= 0xFFFF>> (16- bpc[2]);
    mask[2]= 0xFFFF>> (16- bpc[3]);

    for(uint32_t a= dx* dy; a> 0; a--, p16++) {
      *p++= (*p16)>> (bpc[1]+ bpc[2]+ bpc[3]);
      if(bpc[1]) *p++= ((*p16)>> (bpc[2]+ bpc[3]))& mask[0];
      if(bpc[2]) *p++= ((*p16)>>  bpc[3])&          mask[1];
      if(bpc[3]) *p++=  (*p16)&                     mask[2];
    }
  }

  /// dealloc old bitmap
  delete[] oldBitmap;
  
  packedPixels= false;
  return true;
}



bool Img::swapChannel(uint8_t c1, uint8_t c2) {
  err= 0;
  /// error checks
  if(!packedPixels) { err= 4; return false; }
  if(c1>= nchannels || c2>= nchannels) { err= 7; return false; }
  if(!_checkVars(this)) return false;
  if(nchannels== 1) return true;  // can't swap same channel

  /// tmp vars
  uint8_t *p8= (uint8_t *)bitmap;
  uint16_t *p16= (uint16_t *)bitmap;
  uint32_t *p32= (uint32_t *)bitmap;
  uint64_t *p64= (uint64_t *)bitmap;
  
  uint32_t arr32[4];
  //uint64_t arr64[4];
  uint32_t s32;       /// will be used for channel swapping
  uint64_t s64;       /// will be used for channel swapping

  if(format== ImgFormat::R5G6B5_UNORM_PACK16 || format== ImgFormat::B5G6R5_UNORM_PACK16) {
    for(uint32_t a= dx* dy; a> 0; a--, p16++) {
      arr32[0]= getR565(*p16);
      arr32[1]= getG565(*p16);
      arr32[2]= getB565(*p16);

      /// swap channels
      s32= arr32[c1];        /// tmp swap holder
      arr32[c1]= arr32[c2];
      arr32[c2]= s32;

      /// scaling between 5bit and 6bit if necessary
      if(c1== 1) {
        arr32[1]=  (arr32[1]*  63)/ 31; /// scaled up
        arr32[c2]= (arr32[c2]* 31)/ 63; /// scaled down
      } else if(c2== 1) {
        arr32[1]=  (arr32[1]*  63)/ 31; /// scaled up
        arr32[c1]= (arr32[c1]* 31)/ 63; /// scaled down
      }

      *p16= (arr32[0]<< 11)+ (arr32[1]<< 5)+ arr32[2];
    } /// for each texel

  } else if(format== ImgFormat::R5G5B5A1_UNORM_PACK16 || format== ImgFormat::B5G5R5A1_UNORM_PACK16) {
    for(uint32_t a= dx* dy; a> 0; a--, p16++) {
      arr32[0]= getR5551(*p16);
      arr32[1]= getG5551(*p16);
      arr32[2]= getB5551(*p16);

      /// swap channels
      s32= arr32[c1];        /// tmp swap holder
      arr32[c1]= arr32[c2];
      arr32[c2]= s32;

      *p16= (arr32[0]<< 11)+ (arr32[1]<< 6)+ (arr32[2]<< 1)+ (*p16& 0x01);
    }

  } else if(format== ImgFormat::A1R5G5B5_UNORM_PACK16) {
    for(uint32_t a= dx* dy; a> 0; a--, p16++) {
      arr32[0]= getR1555(*p16);
      arr32[1]= getG1555(*p16);
      arr32[2]= getB1555(*p16);

      /// swap channels
      s32= arr32[c1];        /// tmp swap holder
      arr32[c1]= arr32[c2];
      arr32[c2]= s32;

      *p16= ((*p16)& 0x8000)+ (arr32[0]<< 10)+ (arr32[1]<< 5)+ arr32[2];
    }

  } else if(format== ImgFormat::R4G4_UNORM_PACK8) {
    for(uint32_t a= dx* dy; a> 0; a--, p8++) {
      arr32[0]= (*p8)>> 4;
      arr32[1]= (*p8)& 0xF;

      *p16= (arr32[1])<< (4+ arr32[0]);
    }

  } else if(format== ImgFormat::R4G4B4A4_UNORM_PACK16 || format== ImgFormat::B4G4R4A4_UNORM_PACK16) {
    for(uint32_t a= dx* dy; a> 0; a--, p16++) {
      arr32[0]= (*p16)>> 12;
      arr32[1]= ((*p16)>> 8)& 0xF;
      arr32[2]= ((*p16)>> 4)& 0xF;
      arr32[3]= (*p16)& 0xF;

      s32= arr32[c1];
      arr32[c1]= arr32[c2];
      arr32[c2]= s32;

      *p16= (arr32[0]<< 12)+ (arr32[1]<< 8)+ (arr32[2]<< 4)+ arr32[3];
    }

  // general cases: 16-256 bpp, equal sized channels, 8/16/32/64 bits per channel
  } else if((bpc[c1]== bpc[c2]) && (bpp>= 16) && (bpp<= 256)) {

    // 8bpc
    if(bpc[c1]== 8) {
      for(uint32_t a= dx* dy; a> 0; a--, p8+= nchannels)
        s32=    p8[c1],
        p8[c1]= p8[c2],
        p8[c2]= (uint8_t)s32;

    // 16 bpc
    } else if(bpc[c1]== 16) {
      for(uint32_t a= dx* dy; a> 0; a--, p16+= nchannels)
        s32=     p16[c1],
        p16[c1]= p16[c2],
        p16[c2]= (uint16_t)s32;

    // 32 bpc
    } else if(bpc[c1]== 32) {
      for(uint32_t a= dx* dy; a> 0; a--, p32+= nchannels)
        s32=     p32[c1],
        p32[c1]= p32[c2],
        p32[c2]= s32;

    // 64 bpc
    } else if(bpc[c1]== 64) {
      for(uint32_t a= dx* dy; a> 0; a--, p64+= nchannels)
        s64=     p64[c1],
        p64[c1]= p64[c2],
        p64[c2]= s64;

    } else {
      err= 6;
      return false;
    }

  } else {  /// other formats cannot swap any channels
    err= 6;
    return false;
  }

  return true;
}


ImgFormat Img::compressedToUncompressed(ImgFormat in_f) {
  if(in_f== ImgFormat::BC1_RGB_UNORM_BLOCK) return ImgFormat::R5G6B5_UNORM_PACK16;  // DXT1 / S3TC, RGB       565
  else if(in_f== ImgFormat::BC1_RGB_SRGB_BLOCK) return ImgFormat::R8G8B8_SRGB;      // DXT1 / S3TC, sRGB      565
  else if(in_f== ImgFormat::BC1_RGBA_UNORM_BLOCK) return ImgFormat::R5G5B5A1_UNORM_PACK16; // DXT1 / S3TC, RGBA      5551
  else if(in_f== ImgFormat::BC1_RGBA_SRGB_BLOCK) return ImgFormat::R8G8B8_SRGB; // DXT1 / S3TC, sRGB+A    5551
  else if(in_f== ImgFormat::BC2_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;  // DXT3 / S3TC, RGBA      565+4
  else if(in_f== ImgFormat::BC2_SRGB_BLOCK) return ImgFormat::R8G8B8A8_SRGB;    // DXT3 / S3TC, sRGB+A    565+4
  else if(in_f== ImgFormat::BC3_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;  // DXT5 / S3TC, RGBA      565+8
  else if(in_f== ImgFormat::BC3_SRGB_BLOCK) return ImgFormat::R8G8B8A8_SRGB;    // DXT5 / S3TC, sRGB+A    565+8
  // NOT SURE
  else if(in_f== ImgFormat::BC4_UNORM_BLOCK) return ImgFormat::R16_UNORM;   // RGTC, R ideal for normalized floats (0 to 1.0)             16
  else if(in_f== ImgFormat::BC4_SNORM_BLOCK) return ImgFormat::R16_SNORM;   // RGTC, R ideal for signed normalized floats (-1.0 to 1.0)   16 signed
  else if(in_f== ImgFormat::BC5_UNORM_BLOCK) return ImgFormat::R16G16_UNORM;// RGTC, RG ideal for normalized floats (0 to 1.0)            16+16
  else if(in_f== ImgFormat::BC5_SNORM_BLOCK) return ImgFormat::R16G16_SNORM;// RGTC, RG ideal for signed normalized floats (-1.0 to 1.0)  16+16 signed
  // NOT SURE
  else if(in_f== ImgFormat::BC6H_UFLOAT_BLOCK) return ImgFormat::R16_UINT;  // BPTC, RGB HDR, 16bit unsigned floats (half floats)
  else if(in_f== ImgFormat::BC6H_SFLOAT_BLOCK) return ImgFormat::R16_SFLOAT;// BPTC, RGB HDR, 16bit signed floats (half floats)
  else if(in_f== ImgFormat::BC7_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;// BPTC, RGBA, 8bit per channel, best quality BC for color textures
  else if(in_f== ImgFormat::BC7_SRGB_BLOCK) return ImgFormat::R8G8B8A8_SRGB;// BPTC, sRGB+A 8bit per channel, best quality BC for color textures
  
  else if(in_f== ImgFormat::ETC2_R8G8B8_UNORM_BLOCK) return ImgFormat::R8G8B8_UNORM;    // ETC2, RGB
  else if(in_f== ImgFormat::ETC2_R8G8B8_SRGB_BLOCK) return ImgFormat::R8G8B8_SRGB;      // ETC2, sRGB
  else if(in_f== ImgFormat::ETC2_R8G8B8A1_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;// ETC2, RGBA
  else if(in_f== ImgFormat::ETC2_R8G8B8A1_SRGB_BLOCK) return ImgFormat::R8G8B8A8_SRGB;  // ETC2, sRGB+A
  else if(in_f== ImgFormat::ETC2_R8G8B8A8_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;// ETC2, RGBA
  else if(in_f== ImgFormat::ETC2_R8G8B8A8_SRGB_BLOCK) return ImgFormat::R8G8B8A8_SRGB;  // ETC2, sRGB+a

  else if(in_f== ImgFormat::EAC_R11_UNORM_BLOCK) return ImgFormat::R16_UNORM;
  else if(in_f== ImgFormat::EAC_R11_SNORM_BLOCK) return ImgFormat::R16_SNORM;
  else if(in_f== ImgFormat::EAC_R11G11_UNORM_BLOCK) return ImgFormat::R16G16_UNORM;
  else if(in_f== ImgFormat::EAC_R11G11_SNORM_BLOCK) return ImgFormat::R16G16_SNORM;

  else if(in_f== ImgFormat::ASTC_4x4_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_4x4_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_5x4_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_5x4_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_5x5_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_5x5_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_6x5_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_6x5_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_6x6_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_6x6_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_8x5_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_8x5_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_8x6_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_8x6_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_8x8_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_8x8_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_10x5_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_10x5_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_10x6_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_10x6_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_10x8_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_10x8_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_10x10_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_10x10_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_12x10_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_12x10_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else if(in_f== ImgFormat::ASTC_12x12_UNORM_BLOCK) return ImgFormat::R8G8B8A8_UNORM;
  else if(in_f== ImgFormat::ASTC_12x12_SRGB_BLOCK)  return ImgFormat::R8G8B8A8_SRGB;
  else return ImgFormat::UNDEFINED;
}



















///============================================================///
// OPENGL ==========================--------------------------- //
///============================================================///


#ifdef IMG_CLASS_USE_OPENGL
/*
class _ImgGlFormatConvConstructor {
public:
  _ImgGlFormatConvConstructor();
  inline void set(ImgFormat f, GLint glF, GLenum c, GLenum d) { _Img::_FormatData::get(f)->setGl(glF, c, d); }
  friend class _Img;
} _imgGlFormatConvConstructor__;


_ImgGlFormatConvConstructor::_ImgGlFormatConvConstructor() {
*/

void _Img::setGlData() {
  // https://www.khronos.org/opengl/wiki/Image_Format

  ///set(ImgFormat::UNDEFINED,             GL_NONE,    GL_NONE, GL_NONE);
  ///set(ImgFormat::R4G4_UNORM_PACK8,      GL_NONE,    GL_NONE, GL_NONE);
  set(ImgFormat::R4G4B4A4_UNORM_PACK16, GL_RGBA4,   GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4);
  set(ImgFormat::B4G4R4A4_UNORM_PACK16, GL_RGBA4,   GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4);
  set(ImgFormat::R5G6B5_UNORM_PACK16,   GL_RGB565,  GL_RGB,  GL_UNSIGNED_SHORT_5_6_5);
  set(ImgFormat::B5G6R5_UNORM_PACK16,   GL_RGB565,  GL_BGR,  GL_UNSIGNED_SHORT_5_6_5);
  set(ImgFormat::R5G5B5A1_UNORM_PACK16, GL_RGB5_A1, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1);
  set(ImgFormat::B5G5R5A1_UNORM_PACK16, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1);
  ///set(ImgFormat::A1R5G5B5_UNORM_PACK16, GL_NONE,    GL_NONE, GL_NONE);
    
  set(ImgFormat::R8_UNORM,              GL_R8,       GL_RED, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8_SNORM,              GL_R8_SNORM, GL_RED, GL_BYTE);
  ///set(ImgFormat::R8_USCALED,            , , );
  ///set(ImgFormat::R8_SSCALED,            , , );
  set(ImgFormat::R8_UINT,               GL_R8UI,     GL_RED, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8_SINT,               GL_R8I,      GL_RED, GL_BYTE);
  ///set(ImgFormat::R8_SRGB,               , , );

  set(ImgFormat::R8G8_UNORM,            GL_RG8,       GL_RG, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8G8_SNORM,            GL_RG8_SNORM, GL_RG, GL_BYTE);
  ///set(ImgFormat::R8G8_USCALED,          , , );
  ///set(ImgFormat::R8G8_SSCALED,          , , );
  set(ImgFormat::R8G8_UINT,             GL_RG8UI,     GL_RG, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8G8_SINT,             GL_RG8I,      GL_RG, GL_BYTE);
  ///set(ImgFormat::R8G8_SRGB,             , , );

  set(ImgFormat::R8G8B8_UNORM,          GL_RGB8,       GL_RGB, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8G8B8_SNORM,          GL_RGB8_SNORM, GL_RGB, GL_BYTE);
  ///set(ImgFormat::R8G8B8_USCALED,        , , );
  ///set(ImgFormat::R8G8B8_SSCALED,        , , );
  set(ImgFormat::R8G8B8_UINT,           GL_RGB8UI,     GL_RGB, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8G8B8_SINT,           GL_RGB8I,      GL_RGB, GL_BYTE);
  set(ImgFormat::R8G8B8_SRGB,           GL_SRGB8,      GL_RGB, GL_UNSIGNED_BYTE);
  set(ImgFormat::B8G8R8_UNORM,          GL_RGB8,       GL_BGR, GL_UNSIGNED_BYTE);
  set(ImgFormat::B8G8R8_SNORM,          GL_RGB8_SNORM, GL_BGR, GL_BYTE);
  ///set(ImgFormat::B8G8R8_USCALED,        , , );
  ///set(ImgFormat::B8G8R8_SSCALED,        , , );
  set(ImgFormat::B8G8R8_UINT,           GL_RGB8UI,     GL_BGR, GL_UNSIGNED_BYTE);
  set(ImgFormat::B8G8R8_SINT,           GL_RGB8I,      GL_BGR, GL_BYTE);
  set(ImgFormat::B8G8R8_SRGB,           GL_SRGB,       GL_BGR, GL_UNSIGNED_BYTE);

  set(ImgFormat::R8G8B8A8_UNORM,          GL_RGBA8,        GL_RGBA, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8G8B8A8_SNORM,          GL_RGBA8_SNORM,  GL_RGBA, GL_BYTE);
  ///set(ImgFormat::R8G8B8A8_USCALED,        , , );
  ///set(ImgFormat::R8G8B8A8_SSCALED,        , , );
  set(ImgFormat::R8G8B8A8_UINT,           GL_RGBA8UI,      GL_RGBA, GL_UNSIGNED_BYTE);
  set(ImgFormat::R8G8B8A8_SINT,           GL_RGBA8I,       GL_RGBA, GL_BYTE);
  set(ImgFormat::R8G8B8A8_SRGB,           GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE);
  set(ImgFormat::B8G8R8A8_UNORM,          GL_RGBA8,        GL_BGRA, GL_UNSIGNED_BYTE);
  set(ImgFormat::B8G8R8A8_SNORM,          GL_RGBA8_SNORM,  GL_BGRA, GL_BYTE);
  ///set(ImgFormat::B8G8R8A8_USCALED,        , , );
  ///set(ImgFormat::B8G8R8A8_SSCALED,        , , );
  set(ImgFormat::B8G8R8A8_UINT,           GL_RGBA8UI,      GL_BGRA, GL_UNSIGNED_BYTE);
  set(ImgFormat::B8G8R8A8_SINT,           GL_RGBA8I,       GL_BGRA, GL_BYTE);
  set(ImgFormat::B8G8R8A8_SRGB,           GL_SRGB8_ALPHA8, GL_BGRA, GL_UNSIGNED_BYTE);
  ///set(ImgFormat::A8B8G8R8_UNORM_PACK32,   , , );
  ///set(ImgFormat::A8B8G8R8_SNORM_PACK32,   , , );
  ///set(ImgFormat::A8B8G8R8_USCALED_PACK32, , , );
  ///set(ImgFormat::A8B8G8R8_SSCALED_PACK32, , , );
  ///set(ImgFormat::A8B8G8R8_UINT_PACK32,    , , );
  ///set(ImgFormat::A8B8G8R8_SINT_PACK32,    , , );
  ///set(ImgFormat::A8B8G8R8_SRGB_PACK32,    , , );

  set(ImgFormat::A2R10G10B10_UNORM_PACK32,    GL_RGB10_A2,   GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV);
  ///set(ImgFormat::A2R10G10B10_SNORM_PACK32,    , , );
  ///set(ImgFormat::A2R10G10B10_USCALED_PACK32,  , , );
  ///set(ImgFormat::A2R10G10B10_SSCALED_PACK32,  , , );
  set(ImgFormat::A2R10G10B10_UINT_PACK32,     GL_RGB10_A2UI, GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV);
  ///set(ImgFormat::A2R10G10B10_SINT_PACK32,     , , );
  set(ImgFormat::A2B10G10R10_UNORM_PACK32,    GL_RGB10_A2,   GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV);
  ///set(ImgFormat::A2B10G10R10_SNORM_PACK32,    , , );
  ///set(ImgFormat::A2B10G10R10_USCALED_PACK32,  , , );
  ///set(ImgFormat::A2B10G10R10_SSCALED_PACK32,  , , );
  set(ImgFormat::A2B10G10R10_UINT_PACK32,     GL_RGB10_A2UI, GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV);
  ///set(ImgFormat::A2B10G10R10_SINT_PACK32,     , , );

  set(ImgFormat::R16_UNORM,   GL_R16,       GL_RED, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16_SNORM,   GL_R16_SNORM, GL_RED, GL_SHORT);
  ///set(ImgFormat::R16_USCALED, , , );
  ///set(ImgFormat::R16_SSCALED, , , );
  set(ImgFormat::R16_UINT,    GL_R16UI,     GL_RED, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16_SINT,    GL_R16I,      GL_RED, GL_SHORT);
  set(ImgFormat::R16_SFLOAT,  GL_R16F,      GL_RED, GL_HALF_FLOAT);

  set(ImgFormat::R16G16_UNORM,    GL_RG16,       GL_RG, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16G16_SNORM,    GL_RG16_SNORM, GL_RG, GL_SHORT);
  ///set(ImgFormat::R16G16_USCALED,  , , );
  ///set(ImgFormat::R16G16_SSCALED,  , , );
  set(ImgFormat::R16G16_UINT,     GL_RG16UI,     GL_RG, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16G16_SINT,     GL_RG16I,      GL_RG, GL_SHORT);
  set(ImgFormat::R16G16_SFLOAT,   GL_RG16F,      GL_RG, GL_HALF_FLOAT);

  set(ImgFormat::R16G16B16_UNORM,       GL_RGB16,       GL_RGB, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16G16B16_SNORM,       GL_RGB16_SNORM, GL_RGB, GL_SHORT);
  ///set(ImgFormat::R16G16B16_USCALED,     , , );
  ///set(ImgFormat::R16G16B16_SSCALED,     , , );
  set(ImgFormat::R16G16B16_UINT,        GL_RGB16UI,     GL_RGB, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16G16B16_SINT,        GL_RGB16I,      GL_RGB, GL_SHORT);
  set(ImgFormat::R16G16B16_SFLOAT,      GL_RGB16F,      GL_RGB, GL_HALF_FLOAT);

  set(ImgFormat::R16G16B16A16_UNORM,    GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16G16B16A16_SNORM,    GL_RGBA16_SNORM, GL_RGBA, GL_SHORT);
  ///set(ImgFormat::R16G16B16A16_USCALED,  , , );
  ///set(ImgFormat::R16G16B16A16_SSCALED,  , , );
  set(ImgFormat::R16G16B16A16_UINT,     GL_RGBA16UI,     GL_RGBA, GL_UNSIGNED_SHORT);
  set(ImgFormat::R16G16B16A16_SINT,     GL_RGBA16I,      GL_RGBA, GL_SHORT);
  set(ImgFormat::R16G16B16A16_SFLOAT,   GL_RGBA16F,      GL_RGBA, GL_HALF_FLOAT);

  set(ImgFormat::R32_UINT,              GL_R32UI,    GL_RED,  GL_UNSIGNED_INT);
  set(ImgFormat::R32_SINT,              GL_R32I,     GL_RED,  GL_INT);
  set(ImgFormat::R32_SFLOAT,            GL_R32F,     GL_RED,  GL_FLOAT);
  set(ImgFormat::R32G32_UINT,           GL_RG32UI,   GL_RG,   GL_UNSIGNED_INT);
  set(ImgFormat::R32G32_SINT,           GL_RG32I,    GL_RG,   GL_INT);
  set(ImgFormat::R32G32_SFLOAT,         GL_RG32F,    GL_RG,   GL_FLOAT);
  set(ImgFormat::R32G32B32_UINT,        GL_RGB32UI,  GL_RGB,  GL_UNSIGNED_INT);
  set(ImgFormat::R32G32B32_SINT,        GL_RGB32I,   GL_RGB,  GL_INT);
  set(ImgFormat::R32G32B32_SFLOAT,      GL_RGB32UI,  GL_RGB,  GL_FLOAT);
  set(ImgFormat::R32G32B32A32_UINT,     GL_RGBA32UI, GL_RGBA, GL_UNSIGNED_INT);
  set(ImgFormat::R32G32B32A32_SINT,     GL_RGBA32I,  GL_RGBA, GL_INT);
  set(ImgFormat::R32G32B32A32_SFLOAT,   GL_RGBA32F,  GL_RGBA, GL_FLOAT);

  ///set(ImgFormat::R64_UINT,              , , );
  ///set(ImgFormat::R64_SINT,              , , );
  ///set(ImgFormat::R64_SFLOAT,            , , );
  ///set(ImgFormat::R64G64_UINT,           , , );
  ///set(ImgFormat::R64G64_SINT,           , , );
  ///set(ImgFormat::R64G64_SFLOAT,         , , );
  ///set(ImgFormat::R64G64B64_UINT,        , , );
  ///set(ImgFormat::R64G64B64_SINT,        , , );
  ///set(ImgFormat::R64G64B64_SFLOAT,      , , );
  ///set(ImgFormat::R64G64B64A64_UINT,     , , );
  ///set(ImgFormat::R64G64B64A64_SINT,     , , );
  ///set(ImgFormat::R64G64B64A64_SFLOAT,   , , );

  set(ImgFormat::B10G11R11_UFLOAT_PACK32, GL_R11F_G11F_B10F, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV);
  #ifdef GL_EXT_texture_shared_exponent
  set(ImgFormat::E5B9G9R9_UFLOAT_PACK32,  GL_RGB9_E5,        GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV_EXT);
  #endif

  // depth/stencil
  set(ImgFormat::D16_UNORM,               GL_DEPTH_COMPONENT16,  GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
  set(ImgFormat::X8_D24_UNORM_PACK32,     GL_DEPTH_COMPONENT24,  GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
  set(ImgFormat::D32_SFLOAT,              GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
  set(ImgFormat::S8_UINT,                 GL_STENCIL_INDEX8,     GL_STENCIL,         GL_UNSIGNED_BYTE);
  ///set(ImgFormat::D16_UNORM_S8_UINT,       , , );
  set(ImgFormat::D24_UNORM_S8_UINT,       GL_DEPTH24_STENCIL8,   GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8);
  set(ImgFormat::D32_SFLOAT_S8_UINT,      GL_DEPTH32F_STENCIL8,  GL_DEPTH_STENCIL,   GL_FLOAT_32_UNSIGNED_INT_24_8_REV);

  // BC1-7 compressed
  #ifdef GL_EXT_texture_compression_s3tc
  set(ImgFormat::BC1_RGB_UNORM_BLOCK,   GL_COMPRESSED_RGB_S3TC_DXT1_EXT,        GL_NONE, GL_NONE);
  set(ImgFormat::BC1_RGBA_UNORM_BLOCK,  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,       GL_NONE, GL_NONE);
  set(ImgFormat::BC2_UNORM_BLOCK,       GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,       GL_NONE, GL_NONE);
  set(ImgFormat::BC3_UNORM_BLOCK,       GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,       GL_NONE, GL_NONE);
  #endif

  #ifdef GL_EXT_texture_sRGB
  set(ImgFormat::BC1_RGB_SRGB_BLOCK,    GL_COMPRESSED_SRGB_S3TC_DXT1_EXT,       GL_NONE, GL_NONE);
  set(ImgFormat::BC1_RGBA_SRGB_BLOCK,   GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, GL_NONE, GL_NONE);
  set(ImgFormat::BC2_SRGB_BLOCK,        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, GL_NONE, GL_NONE);
  set(ImgFormat::BC3_SRGB_BLOCK,        GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, GL_NONE, GL_NONE);
  #endif

  #ifdef GL_VERSION_3_0
  set(ImgFormat::BC4_UNORM_BLOCK,       GL_COMPRESSED_RED_RGTC1,        GL_NONE, GL_NONE);
  set(ImgFormat::BC4_SNORM_BLOCK,       GL_COMPRESSED_SIGNED_RED_RGTC1, GL_NONE, GL_NONE);
  set(ImgFormat::BC5_UNORM_BLOCK,       GL_COMPRESSED_RG_RGTC2,         GL_NONE, GL_NONE);
  set(ImgFormat::BC5_SNORM_BLOCK,       GL_COMPRESSED_SIGNED_RG_RGTC2,  GL_NONE, GL_NONE);
  #endif

  #ifdef GL_VERSION_4_2
  set(ImgFormat::BC6H_UFLOAT_BLOCK,     GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,   GL_NONE, GL_NONE);
  set(ImgFormat::BC6H_SFLOAT_BLOCK,     GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT, GL_NONE, GL_NONE);
  set(ImgFormat::BC7_UNORM_BLOCK,       GL_COMPRESSED_RGBA_BPTC_UNORM,         GL_NONE, GL_NONE);
  set(ImgFormat::BC7_SRGB_BLOCK,        GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,   GL_NONE, GL_NONE);
  #endif

  #ifdef GL_VERSION_4_3
  // ETC2 compressed
  set(ImgFormat::ETC2_R8G8B8_UNORM_BLOCK,   GL_COMPRESSED_RGB8_ETC2,                      GL_NONE, GL_NONE);
  set(ImgFormat::ETC2_R8G8B8_SRGB_BLOCK,    GL_COMPRESSED_SRGB8_ETC2,                     GL_NONE, GL_NONE);
  set(ImgFormat::ETC2_R8G8B8A1_UNORM_BLOCK, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,  GL_NONE, GL_NONE);
  set(ImgFormat::ETC2_R8G8B8A1_SRGB_BLOCK,  GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2, GL_NONE, GL_NONE);
  set(ImgFormat::ETC2_R8G8B8A8_UNORM_BLOCK, GL_COMPRESSED_RGBA8_ETC2_EAC,                 GL_NONE, GL_NONE);
  set(ImgFormat::ETC2_R8G8B8A8_SRGB_BLOCK,  GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,          GL_NONE, GL_NONE);
  // EAC compressed
  set(ImgFormat::EAC_R11_UNORM_BLOCK,     GL_COMPRESSED_R11_EAC,         GL_NONE, GL_NONE);
  set(ImgFormat::EAC_R11_SNORM_BLOCK,     GL_COMPRESSED_SIGNED_R11_EAC,  GL_NONE, GL_NONE);
  set(ImgFormat::EAC_R11G11_UNORM_BLOCK,  GL_COMPRESSED_RG11_EAC,        GL_NONE, GL_NONE);
  set(ImgFormat::EAC_R11G11_SNORM_BLOCK,  GL_COMPRESSED_SIGNED_RG11_EAC, GL_NONE, GL_NONE);
  #endif

  #ifdef GL_KHR_texture_compression_astc_hdr
  // ASTC compressed
  set(ImgFormat::ASTC_4x4_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_4x4_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_4x4_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_5x4_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_5x4_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_5x4_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_5x5_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_5x5_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_5x5_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_6x5_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_6x5_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_6x5_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_6x6_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_6x6_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_6x6_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_8x5_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_8x5_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_8x5_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_8x6_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_8x6_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_8x6_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_8x8_UNORM_BLOCK,    GL_COMPRESSED_RGBA_ASTC_8x8_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_8x8_SRGB_BLOCK,     GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x5_UNORM_BLOCK,   GL_COMPRESSED_RGBA_ASTC_10x5_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x5_SRGB_BLOCK,    GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x6_UNORM_BLOCK,   GL_COMPRESSED_RGBA_ASTC_10x6_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x6_SRGB_BLOCK,    GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x8_UNORM_BLOCK,   GL_COMPRESSED_RGBA_ASTC_10x8_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x8_SRGB_BLOCK,    GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x10_UNORM_BLOCK,  GL_COMPRESSED_RGBA_ASTC_10x10_KHR,        GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_10x10_SRGB_BLOCK,   GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_12x10_UNORM_BLOCK,  GL_COMPRESSED_RGBA_ASTC_12x10_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_12x10_SRGB_BLOCK,   GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR, GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_12x12_UNORM_BLOCK,  GL_COMPRESSED_RGBA_ASTC_12x12_KHR,         GL_NONE, GL_NONE);
  set(ImgFormat::ASTC_12x12_SRGB_BLOCK,   GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR, GL_NONE, GL_NONE);
  #endif // GL_KHR_texture_compression_astc_hdr
}
  

/// this is ok to be outside the gl scope?
bool Img::glIsCompatible() {
  if((format>= ImgFormat::CMAP_START_RANGE) && (format<= ImgFormat::CMAP_END_RANGE)) return false;
  return true;
}


//#if(defined(__gl_h_)|| defined(__glcorearb_h_))


int Img::glGetDataType(ImgFormat in_f) {
  return _Img::_FormatData::get(in_f)->glDataType;
}


int Img::glGetGlFormat(ImgFormat in_f) {
  return _Img::_FormatData::get(in_f)->glFormat;
}


// returns the ImgFormat closest resembling the OGL internal format
ImgFormat Img::glGetFormat(int in_glFormat) {
  _Img::_FormatData *p= _Img::_FormatData::get((ImgFormat)1);
  while(p->format!= ImgFormat::UNDEFINED) {
    if(p->glFormat== in_glFormat)
      return p->format;
  }
  return ImgFormat::UNDEFINED;
}


void Img::glApplyGreyscaleSwizzle() {
  _Img::_FormatData *p= _Img::_FormatData::get(format);

  if(p->nchannels== 1) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
  } else if(p->nchannels== 2) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
  }
}

int Img::glGetNrChannels(int in_format) const {
  return _Img::_FormatData::getGl(in_format)->nchannels;
}



bool Img::glConvertCompatible() {
  //if(format== ImgFormat::CMAP1_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP1_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP1_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  //if(format== ImgFormat::CMAP2_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP2_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP2_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  //if(format== ImgFormat::CMAP4_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP4_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP4_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  //if(format== ImgFormat::CMAP8_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP8_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP8_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  if(format== ImgFormat::CMAP_RGB) return convert(ImgFormat::R8G8B8_UNORM);
  if(format== ImgFormat::CMAP_RGBA) return convert(ImgFormat::R8G8B8A8_UNORM);
  return true;
}

int32_t Img::glGetGlType(Img::Type in_type) {
  if(in_type== Type::T_2D) return GL_TEXTURE_2D;
  if(in_type== Type::T_1D) return GL_TEXTURE_1D;
  if(in_type== Type::T_3D) return GL_TEXTURE_3D;
  return GL_TEXTURE_2D;
}


Img::Type Img::glGetType(int32_t in_type) {
  if(in_type== GL_TEXTURE_2D) return Type::T_2D;
  if(in_type== GL_TEXTURE_1D) return Type::T_1D;
  if(in_type== GL_TEXTURE_3D) return Type::T_3D;
  return Type::T_2D;
}


int32_t Img::glGetGlWrap(Wrap in_wrap) {
  if(in_wrap== Wrap::REPEAT)               return GL_REPEAT;
  if(in_wrap== Wrap::MIRRORED_REPEAT)      return GL_MIRRORED_REPEAT;
  if(in_wrap== Wrap::CLAMP_TO_EDGE)        return GL_CLAMP_TO_EDGE;
  if(in_wrap== Wrap::CLAMP_TO_BORDER)      return GL_CLAMP_TO_BORDER;
  if(in_wrap== Wrap::MIRROR_CLAMP_TO_EDGE) return GL_MIRROR_CLAMP_TO_EDGE;
  return GL_REPEAT;
}


Img::Wrap Img::glGetWrap(int32_t in_wrap) {
  if(in_wrap== GL_REPEAT)               return Wrap::REPEAT;
  if(in_wrap== GL_MIRRORED_REPEAT)      return Wrap::MIRRORED_REPEAT;
  if(in_wrap== GL_CLAMP_TO_EDGE)        return Wrap::CLAMP_TO_EDGE;
  if(in_wrap== GL_CLAMP_TO_BORDER)      return Wrap::CLAMP_TO_BORDER;
  if(in_wrap== GL_MIRROR_CLAMP_TO_EDGE) return Wrap::MIRROR_CLAMP_TO_EDGE;
  return Wrap::REPEAT;
}


Img::Swizzle Img::glGetSwizzle(int in_s, int in_channel) {
  if(in_s== in_channel) return Swizzle::IDENTITY;
  if(in_s== GL_RED)   return Swizzle::R;
  if(in_s== GL_GREEN) return Swizzle::G;
  if(in_s== GL_BLUE)  return Swizzle::B;
  if(in_s== GL_ALPHA) return Swizzle::A;
  if(in_s== GL_ZERO)  return Swizzle::ZERO;
  if(in_s== GL_ONE)   return Swizzle::ONE;
  return Swizzle::IDENTITY;
}


int Img::glGetGlSwizzle(Swizzle in_s, int in_channel) {
  if(in_s== Swizzle::IDENTITY) return in_channel;
  if(in_s== Swizzle::R)    return GL_RED;
  if(in_s== Swizzle::G)    return GL_GREEN;
  if(in_s== Swizzle::B)    return GL_BLUE;
  if(in_s== Swizzle::A)    return GL_ALPHA;
  if(in_s== Swizzle::ZERO) return GL_ZERO;
  if(in_s== Swizzle::ONE)  return GL_ONE;
  return in_channel;
}

/*
Img::CompareFunc Img::glGetCompareFunc(GLint in_c) {
  if(in_c== GL_NEVER)     return CompareFunc::NEVER;
  if(in_c== GL_LESS)      return CompareFunc::LESS;
  if(in_c== GL_EQUAL)     return CompareFunc::EQUAL;
  if(in_c== GL_LEQUAL)    return CompareFunc::LESS_OR_EQUAL;
  if(in_c== GL_GREATER)   return CompareFunc::GREATER;
  if(in_c== GL_NOTEQUAL)  return CompareFunc::NOT_EQUAL;
  if(in_c== GL_GEQUAL)    return CompareFunc::GREATER_OR_EQUAL;
  if(in_c== GL_ALWAYS)    return CompareFunc::ALWAYS;
  return CompareFunc::LESS; // default value
}


GLint Img::glGetGlCompareFunc(CompareFunc in_c) {
  if(in_c== CompareFunc::NEVER)            return GL_NEVER;
  if(in_c== CompareFunc::LESS)             return GL_LESS;
  if(in_c== CompareFunc::EQUAL)            return GL_EQUAL;
  if(in_c== CompareFunc::LESS_OR_EQUAL)    return GL_LEQUAL;
  if(in_c== CompareFunc::GREATER)          return GL_GREATER;
  if(in_c== CompareFunc::NOT_EQUAL)        return GL_NOTEQUAL;
  if(in_c== CompareFunc::GREATER_OR_EQUAL) return GL_GEQUAL;
  if(in_c== CompareFunc::ALWAYS)           return GL_ALWAYS;
  return GL_LESS; // default
}
*/
#endif /// USE_OPENGL


///============================================================///
// VULKAN ==========================--------------------------- //
///============================================================///

#ifdef IMG_CLASS_USE_VULKAN
/*
VkFormat Img::vkGetVkFormat(ImgFormat in_format) {
  if     (in_format== IMG_GREY)       return VK_FORMAT_R8_UNORM;             /// 8bit,  8bits per channel,  1 channel
  else if(in_format== IMG_GREY_ALPHA) return VK_FORMAT_R8G8_UNORM;           /// 16bit, 8bits per channel,  2 channels
  else if(in_format== IMG_RGB)        return VK_FORMAT_R8G8B8_UNORM;         /// 24bit, 8bits per channel,  3 channels
  else if(in_format== IMG_RGBA)       return VK_FORMAT_R8G8B8A8_UNORM;       /// 32bit, 8bits per channel,  4 channels
  else if(in_format== IMG_RGB16)      return VK_FORMAT_R5G6B5_UNORM_PACK16;  /// 16bit, RRRRRGGG GGGBBBBB,  3 channels
  else if(in_format== IMG_RGBA16)     return VK_FORMAT_R5G5B5A1_UNORM_PACK16;/// 16bit, RRRRRGGG GGBBBBBA,  4 channels
  else if(in_format== IMG_RGB48)      return VK_FORMAT_R16G16B16_UNORM;      /// 48bit, 16bits per channel, 3 channels
  else if(in_format== IMG_RGBA64)     return VK_FORMAT_R16G16B16A16_UNORM;   /// 64bit, 16bits per channel, 3 channels
  else if(in_format== IMG_GREY16)     return VK_FORMAT_R16_UNORM;            /// 16bit, 16bits per channel, 1 channel
  else if(in_format== IMG_GREY_ALPHA32) return VK_FORMAT_R16G16_UNORM;       /// 32bit, 16bits per channel, 2 channels
  else return VK_FORMAT_UNDEFINED;
}
*/

bool Img::vkConvertCompatible() {
  //if(format== ImgFormat::CMAP1_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP1_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP1_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  //if(format== ImgFormat::CMAP2_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP2_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP2_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  //if(format== ImgFormat::CMAP4_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP4_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP4_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  //if(format== ImgFormat::CMAP8_R5G5B5A1) return convert(ImgFormat::R5G5B5A1_UNORM_PACK16);
  //if(format== ImgFormat::CMAP8_R8G8B8)   return convert(ImgFormat::R8G8B8A8_UNORM);
  //if(format== ImgFormat::CMAP8_R8G8B8A8) return convert(ImgFormat::R8G8B8_UNORM);
  if(format== ImgFormat::CMAP_RGB) return convert(ImgFormat::R8G8B8_UNORM);
  if(format== ImgFormat::CMAP_RGBA) return convert(ImgFormat::R8G8B8A8_UNORM);
  return true;
}


uint32_t Img::vkGetAspectFromFormat(ImgFormat in_format) {
  if((in_format>= ImgFormat::D16_UNORM) && (in_format<= ImgFormat::D32_SFLOAT))
    return VK_IMAGE_ASPECT_DEPTH_BIT;
  if(in_format== ImgFormat::S8_UINT)
    return VK_IMAGE_ASPECT_STENCIL_BIT;
  if((in_format>= ImgFormat::D16_UNORM_S8_UINT) && (in_format<= ImgFormat::D32_SFLOAT_S8_UINT))
    return VK_IMAGE_ASPECT_DEPTH_BIT| VK_IMAGE_ASPECT_STENCIL_BIT;

  return VK_IMAGE_ASPECT_COLOR_BIT;
}


/*
// returns the ImgFormat closest resembling the OGL internal format
ImgFormat Img::vkGetImageFormat(VkFormat in_f) {
  // https://www.khronos.org/registry/vulkan/specs/1.0/html/chap32.html

  if     (in_f== VK_FORMAT_R8_UNORM)        return IMG_GREY;
  else if(in_f== VK_FORMAT_R8G8_UNORM)      return IMG_GREY_ALPHA;
  else if(in_f== VK_FORMAT_R8G8B8_UNORM)    return IMG_RGB;
  else if(in_f== VK_FORMAT_R8G8B8A8_UNORM)  return IMG_RGBA;

  else if(in_f== VK_FORMAT_R5G6B5_UNORM_PACK16)   return IMG_RGB16;
  else if(in_f== VK_FORMAT_R5G5B5A1_UNORM_PACK16) return IMG_RGBA16;

  else if(in_f== VK_FORMAT_R16G16B16_UNORM)    return IMG_RGB48;
  else if(in_f== VK_FORMAT_R16G16B16A16_UNORM) return IMG_RGBA64;

  else if(in_f== VK_FORMAT_R16_UNORM)     return IMG_GREY16;
  else if(in_f== VK_FORMAT_R16G16_UNORM)  return IMG_GREY_ALPHA32;

  // BC1-7 ASTC ETC are not implemented atm
  
  return IMG_NONE;
}
*/

/*
VkImageType Img::vkGetVkType(ImageType in_type) {
  if(in_type== TYPE_2D) return VK_IMAGE_TYPE_2D;
  if(in_type== TYPE_1D) return VK_IMAGE_TYPE_1D;
  if(in_type== TYPE_3D) return VK_IMAGE_TYPE_3D;
}


ImageType Img::vkGetType(VkImageType in_type) {
  if(in_type== VK_IMAGE_TYPE_2D) return TYPE_2D ;
  if(in_type== VK_IMAGE_TYPE_1D) return TYPE_1D;
  if(in_type== VK_IMAGE_TYPE_3D) return TYPE_3D;
}


VkSamplerAddressMode Img::vkGetVkWrap(ImageWrap in_wrap) {
  if(in_wrap== WRAP_REPEAT)               return VK_SAMPLER_ADDRESS_MODE_REPEAT;
  if(in_wrap== WRAP_MIRRORED_REPEAT)      return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
  if(in_wrap== WRAP_CLAMP_TO_EDGE)        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  if(in_wrap== WRAP_CLAMP_TO_BORDER)      return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
  if(in_wrap== WRAP_MIRROR_CLAMP_TO_EDGE) return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
}


ImageWrap Img::vkGetWrap(VkSamplerAddressMode in_wrap) {
  if(in_wrap== VK_SAMPLER_ADDRESS_MODE_REPEAT)               return WRAP_REPEAT;
  if(in_wrap== VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT)      return WRAP_MIRRORED_REPEAT;
  if(in_wrap== VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)        return WRAP_CLAMP_TO_EDGE;
  if(in_wrap== VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)      return WRAP_CLAMP_TO_BORDER;
  if(in_wrap== VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE) return WRAP_MIRROR_CLAMP_TO_EDGE;
}


IMG_SWIZZLE Img::vkGetSwizzle(VkComponentSwizzle in_s) {
  return in_s;
  if(in_s== VK_COMPONENT_SWIZZLE_R)         return SWIZZLE_R;
  if(in_s== VK_COMPONENT_SWIZZLE_G)         return SWIZZLE_G;
  if(in_s== VK_COMPONENT_SWIZZLE_B)         return SWIZZLE_B;
  if(in_s== VK_COMPONENT_SWIZZLE_A)         return SWIZZLE_A;
  if(in_s== VK_COMPONENT_SWIZZLE_ONE)       return SWIZZLE_ONE;
  if(in_s== VK_COMPONENT_SWIZZLE_ZERO)      return SWIZZLE_ZERO;
  if(in_s== VK_COMPONENT_SWIZZLE_IDENTITY)  return SWIZZLE_IDENTITY;
}


VkComponentSwizzle Img::vkGetVkSwizzle(ImageSwizzle in_s) {
  if(in_s== SWIZZLE_R)        return VK_COMPONENT_SWIZZLE_R;
  if(in_s== SWIZZLE_G)        return VK_COMPONENT_SWIZZLE_G;
  if(in_s== SWIZZLE_B)        return VK_COMPONENT_SWIZZLE_B;
  if(in_s== SWIZZLE_A)        return VK_COMPONENT_SWIZZLE_A;
  if(in_s== SWIZZLE_ONE)      return VK_COMPONENT_SWIZZLE_ONE;
  if(in_s== SWIZZLE_ZERO)     return VK_COMPONENT_SWIZZLE_ZERO;
  if(in_s== SWIZZLE_IDENTITY) return VK_COMPONENT_SWIZZLE_IDENTITY;
}
*/
#endif // USE VULKAN























