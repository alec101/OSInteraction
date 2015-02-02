#pragma once
/*
  - SUPPORTED formats:
      all uncompressed and RLE compressed TGA types (img type 1, 2, 3, 9, 10, 11)

  - NOT SUPPORTED formats:
      zip compression NOT SUPPORTED (image types 32 & 33)


ERROR NUMBERS:
______________
0= no error
1= file not found
2= size error
3= palette is empty
4= alloc error
5= invalid BPP
6= cmap palette not suported
7= unknown / unsupported image type
8= file read error
*/

#define FILETGA_INCLUDED

#ifndef IMG_TYPE_DEFINED
#define IMG_TYPE_DEFINED
enum ImageType {      // notice first 4, most important img types also have the number of channels in the enum value
  // the major image types first
  IMG_NONE= 0,        /// default - image not initialized / everything is empty
  IMG_GREY= 1,        /// 8bit greyscale /             1 channel
  IMG_GREY_ALPHA= 2,  /// 16bit greyscale with alpha / 2 channels
  IMG_RGB= 3,         /// 24bit RGB /                  3 channels
  IMG_RGBA= 4,        /// 32bit RGBA /                 4 channels
  IMG_CMAP= 5,        /// 8bit color map palette /     1 channel (16/24/32bits/color)
  // major image types name aliases
  IMG_RGB_888= 3,     /// 24bit RGB / 3 channels
  IMG_RGBA_8888= 4,   /// 32bit RGBA / 4 channels
  // minor image types from here on
  IMG_RGB16= 6,           /// 16bit RGB  [RRRRRGGG GGGBBBBB] / 3 channels
  IMG_RGB_565= 6,         /// 16bit RGB  [RRRRRGGG GGGBBBBB] / 3 channels
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
  IMG_GREY_ALPHA_1616= 14 /// 32bit GREY + ALPHA, 16bits per channel / 2 channels
};
#endif /// IMG_TYPE_DEFINED






class TGA {

public:
  
  void *bitmap;             /// raw data
  int32_t dx, dy;             /// image size
  int8_t bpp, bpc;            /// bits per pixel / bits per channel
  ImageType type;           /// type value is the number of channels too

  uint8_t *cmap;              /// color palette
  int8_t cmapBpp;             /// color palette bits per color (16 / 24 / 32)

  // saving / loading

  bool load(const char *);        // loads a TGA (RLE compressed or uncompressed)
  bool save(const char *);        // TGA uncompressed
  
  bool loadPalette(const char *);
  bool savePalette(const char *);

  // error handling

  unsigned int err;                 /// see beggining of file
  const char *getError();        /// returns error as text

  // constructor / destructor

  TGA();
  ~TGA() { delData(); }
  void delData();
};







