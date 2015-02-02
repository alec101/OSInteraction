#pragma once

#define FILEPNG_INCLUDED

#ifndef IMG_TYPE_DEFINED
#define IMG_TYPE_DEFINED
enum ImageType {      // notice first 4, most important img types also have the number of channels in the enum value
  // the major image types first
  IMG_NONE= 0,        /// default - image not initialized / everything is empty
  IMG_GREY= 1,        /// 8bit greyscale /             1 channel
  IMG_GREY_ALPHA= 2,  /// 16bit greyscale with alpha / 2 channels
  IMG_RGB= 3,         /// 24bit RGB /                  3 channels
  IMG_RGBA= 4,        /// 32bit RGBA /                 4 channels
  IMG_CMAP= 5,        /// 8bit color map palette, 256 colors in cmap / 1 channel (16/24/32bits/color)
  // major image types name aliases
  IMG_RGB_888= 3,     /// 24bit RGB /  3 channels
  IMG_RGBA_8888= 4,   /// 32bit RGBA / 4 channels
  IMG_CMAP_8= 5,      /// 8bit color map palette, 256 colors in cmap / 1 channel (16/24/32bits/color)
  // minor image types from here on
  IMG_RGB16= 6,           /// 16bit RGB  [RRRRRGGG GGGBBBBB] / 3 channels
  IMG_RGB_565= 6,         /// 16but RGB  [RRRRRGGG GGGBBBBB] / 3 channels
  IMG_RGBA16= 7,          /// 16bit RGBA [RRRRRGGG GGBBBBBA] / 4 channels
  IMG_RGBA_5551= 7,       /// 16bit RGBA [RRRRRGGG GGBBBBBA] / 4 channels
  IMG_RGB48= 8,           /// 48bit RGB 16bits per channel /   3 channels
  IMG_RGB_161616= 8,      /// 48bit RGB 16bits per channel /   3 channels
  IMG_RGBA64= 9,          /// 64bit RGBA 16bits per channel /  4 channels
  IMG_RGBA_16161616= 9,   /// 64bit RGBA 16bits per channel /  4 channels
  IMG_GREY1= 10,          /// 1bit GREY /  1 channel
  IMG_GREY_1= 10,         /// 1bit GREY /  1 channel
  IMG_GREY2= 11,          /// 2bit GREY /  1 channel
  IMG_GREY_2= 11,         /// 2bit GREY /  1 channel
  IMG_GREY4= 12,          /// 4bit GREY /  1 channel
  IMG_GREY_4= 12,         /// 4bit GREY /  1 channel
  IMG_GREY16= 13,         /// 16bit GREY / 1 channel
  IMG_GREY_16= 13,        /// 16bit GREY / 1 channel
  IMG_GREY_ALPHA32= 14,   /// 32bit GREY + ALPHA, 16bits per channel / 2 channels
  IMG_GREY_ALPHA_1616= 14,/// 32bit GREY + ALPHA, 16bits per channel / 2 channels
  IMG_CMAP1= 15,          /// 1bit cmap palette 2 max colors in cmap /  1 channel
  IMG_CMAP_1= 15,         /// 1bit cmap palette 2 max colors in cmap /  1 channel
  IMG_CMAP2= 16,          /// 2bit cmap palette 4 max colors in cmap /  1 channel
  IMG_CMAP_2= 16,         /// 2bit cmap palette 4 max colors in cmap /  1 channel
  IMG_CMAP4= 17,          /// 4bit cmap palette 16 max colors in cmap / 1 channel
  IMG_CMAP_4= 17          /// 4bit cmap palette 16 max colors in cmap / 1 channel
};
#endif /// IMG_TYPE_DEFINED

#include "mzPacker.h"

class PNG {
  
  mzPacker pack;        /// miniz packer class
public:
  
  void *bitmap;         /// raw data
  int32_t dx, dy;        /// image size
  int8_t bpp, bpc;       /// bits per pixel / bits per channel
  ImageType type;       /// image type - check ImageType enum

  uint8_t *cmap;          /// color map palette
  int8_t cmapBpp;        /// color palette bits per color (16 / 24 / 32)

  bool hasTrnCol;       /// indicates that the image uses the transparent (trnCol);
  struct { uint16_t r, g, b; } trnCol;  /// the transparent color, can be used in RGB / GREY image types

  // saving / loading

  bool load(const char *);   /// loads a PNG, about any kind
  bool save(const char *);   /// saves a PNG, fill in dx/dy/type/bpp, bitmap should hold raw data before saving
  
  // error handling

  uint16_t err;           /// error number - 0= no errors
  const char *getError();    /// returns error as text

  // constructor / destructor

  PNG();
  ~PNG() { delData(); }
  void delData();

  // to be or not to be
  
  // bool loadPalette(cchar *);
  // bool savePalette(cchar *);
};





/* Error numbers:
0 - OK, no errors
1 - cannot open file
2 - file read error
3 - file write error
4 - image size error
5 - cmap is empty / cmap expected - not found
6 - alloc error
7 - invalid BPP
8 - cmap palette not suported
9 - unknown / unsupported image type
10 - PNG header not read first
11 - IHDR chunk error
12 - PLTE chunk error
13 - IDAT chunk error
14 - decompression error
15 - raw data not filled
16 - PNG does not support this image format
17 - compression error
*/

