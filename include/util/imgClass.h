#pragma once

#ifdef OSI_USE_OPENGL
#define IMG_CLASS_USE_OPENGL 1   // enables/disables opengl funcs
#endif
#ifdef OSI_USE_VKO
#define IMG_CLASS_USE_VULKAN 1   // enables/disables vulkan funcs
#endif

#include "osi/include/util/typeShortcuts.h"
#include "osi/include/util/str8.h"

enum class ImgFormat: uint32 {
  UNDEFINED = 0,
  R4G4_UNORM_PACK8 = 1,
  R4G4B4A4_UNORM_PACK16 = 2,
  B4G4R4A4_UNORM_PACK16 = 3,
  R5G6B5_UNORM_PACK16 = 4,
  B5G6R5_UNORM_PACK16 = 5,
  R5G5B5A1_UNORM_PACK16 = 6,
  B5G5R5A1_UNORM_PACK16 = 7,
  A1R5G5B5_UNORM_PACK16 = 8,
  R8_UNORM = 9,
  R8_SNORM = 10,
  R8_USCALED = 11,
  R8_SSCALED = 12,
  R8_UINT = 13,
  R8_SINT = 14,
  R8_SRGB = 15,
  R8G8_UNORM = 16,
  R8G8_SNORM = 17,
  R8G8_USCALED = 18,
  R8G8_SSCALED = 19,
  R8G8_UINT = 20,
  R8G8_SINT = 21,
  R8G8_SRGB = 22,
  R8G8B8_UNORM = 23,
  R8G8B8_SNORM = 24,
  R8G8B8_USCALED = 25,
  R8G8B8_SSCALED = 26,
  R8G8B8_UINT = 27,
  R8G8B8_SINT = 28,
  R8G8B8_SRGB = 29,
  B8G8R8_UNORM = 30,
  B8G8R8_SNORM = 31,
  B8G8R8_USCALED = 32,
  B8G8R8_SSCALED = 33,
  B8G8R8_UINT = 34,
  B8G8R8_SINT = 35,
  B8G8R8_SRGB = 36,
  R8G8B8A8_UNORM = 37,
  R8G8B8A8_SNORM = 38,
  R8G8B8A8_USCALED = 39,
  R8G8B8A8_SSCALED = 40,
  R8G8B8A8_UINT = 41,
  R8G8B8A8_SINT = 42,
  R8G8B8A8_SRGB = 43,
  B8G8R8A8_UNORM = 44,
  B8G8R8A8_SNORM = 45,
  B8G8R8A8_USCALED = 46,
  B8G8R8A8_SSCALED = 47,
  B8G8R8A8_UINT = 48,
  B8G8R8A8_SINT = 49,
  B8G8R8A8_SRGB = 50,
  A8B8G8R8_UNORM_PACK32 = 51,
  A8B8G8R8_SNORM_PACK32 = 52,
  A8B8G8R8_USCALED_PACK32 = 53,
  A8B8G8R8_SSCALED_PACK32 = 54,
  A8B8G8R8_UINT_PACK32 = 55,
  A8B8G8R8_SINT_PACK32 = 56,
  A8B8G8R8_SRGB_PACK32 = 57,
  A2R10G10B10_UNORM_PACK32 = 58,
  A2R10G10B10_SNORM_PACK32 = 59,
  A2R10G10B10_USCALED_PACK32 = 60,
  A2R10G10B10_SSCALED_PACK32 = 61,
  A2R10G10B10_UINT_PACK32 = 62,
  A2R10G10B10_SINT_PACK32 = 63,
  A2B10G10R10_UNORM_PACK32 = 64,
  A2B10G10R10_SNORM_PACK32 = 65,
  A2B10G10R10_USCALED_PACK32 = 66,
  A2B10G10R10_SSCALED_PACK32 = 67,
  A2B10G10R10_UINT_PACK32 = 68,
  A2B10G10R10_SINT_PACK32 = 69,
  R16_UNORM = 70,
  R16_SNORM = 71,
  R16_USCALED = 72,
  R16_SSCALED = 73,
  R16_UINT = 74,
  R16_SINT = 75,
  R16_SFLOAT = 76,
  R16G16_UNORM = 77,
  R16G16_SNORM = 78,
  R16G16_USCALED = 79,
  R16G16_SSCALED = 80,
  R16G16_UINT = 81,
  R16G16_SINT = 82,
  R16G16_SFLOAT = 83,
  R16G16B16_UNORM = 84,
  R16G16B16_SNORM = 85,
  R16G16B16_USCALED = 86,
  R16G16B16_SSCALED = 87,
  R16G16B16_UINT = 88,
  R16G16B16_SINT = 89,
  R16G16B16_SFLOAT = 90,
  R16G16B16A16_UNORM = 91,
  R16G16B16A16_SNORM = 92,
  R16G16B16A16_USCALED = 93,
  R16G16B16A16_SSCALED = 94,
  R16G16B16A16_UINT = 95,
  R16G16B16A16_SINT = 96,
  R16G16B16A16_SFLOAT = 97,
  R32_UINT = 98,
  R32_SINT = 99,
  R32_SFLOAT = 100,
  R32G32_UINT = 101,
  R32G32_SINT = 102,
  R32G32_SFLOAT = 103,
  R32G32B32_UINT = 104,
  R32G32B32_SINT = 105,
  R32G32B32_SFLOAT = 106,
  R32G32B32A32_UINT = 107,
  R32G32B32A32_SINT = 108,
  R32G32B32A32_SFLOAT = 109,
  R64_UINT = 110,
  R64_SINT = 111,
  R64_SFLOAT = 112,
  R64G64_UINT = 113,
  R64G64_SINT = 114,
  R64G64_SFLOAT = 115,
  R64G64B64_UINT = 116,
  R64G64B64_SINT = 117,
  R64G64B64_SFLOAT = 118,
  R64G64B64A64_UINT = 119,
  R64G64B64A64_SINT = 120,
  R64G64B64A64_SFLOAT = 121,
  B10G11R11_UFLOAT_PACK32 = 122,
  E5B9G9R9_UFLOAT_PACK32 = 123,
  D16_UNORM = 124,
  X8_D24_UNORM_PACK32 = 125,
  D32_SFLOAT = 126,
  S8_UINT = 127,
  D16_UNORM_S8_UINT = 128,
  D24_UNORM_S8_UINT = 129,
  D32_SFLOAT_S8_UINT = 130,
  // compressed formats
  BC1_RGB_UNORM_BLOCK = 131,  // DXT1 / S3TC, RGB       565
  BC1_RGB_SRGB_BLOCK = 132,   // DXT1 / S3TC, sRGB      565
  BC1_RGBA_UNORM_BLOCK = 133, // DXT1 / S3TC, RGBA      5551
  BC1_RGBA_SRGB_BLOCK = 134,  // DXT1 / S3TC, sRGB+A    5551
  BC2_UNORM_BLOCK = 135,      // DXT3 / S3TC, RGBA      565+4
  BC2_SRGB_BLOCK = 136,       // DXT3 / S3TC, sRGB+A    565+4
  BC3_UNORM_BLOCK = 137,      // DXT5 / S3TC, RGBA      565+8
  BC3_SRGB_BLOCK = 138,       // DXT5 / S3TC, sRGB+A    565+8
  BC4_UNORM_BLOCK = 139,      // RGTC, R ideal for normalized floats (0 to 1.0)             16
  BC4_SNORM_BLOCK = 140,      // RGTC, R ideal for signed normalized floats (-1.0 to 1.0)   16 signed
  BC5_UNORM_BLOCK = 141,      // RGTC, RG ideal for normalized floats (0 to 1.0)            16+16
  BC5_SNORM_BLOCK = 142,      // RGTC, RG ideal for signed normalized floats (-1.0 to 1.0)  16+16 signed
  BC6H_UFLOAT_BLOCK = 143,    // BPTC, RGB HDR, 16bit unsigned half floats
  BC6H_SFLOAT_BLOCK = 144,    // BPTC, RGB HDR, 16bit signed half floats
  BC7_UNORM_BLOCK = 145,      // BPTC, RGBA, 8bit per channel, best quality BC for color textures
  BC7_SRGB_BLOCK = 146,       // BPTC, sRGB+A 8bit per channel, best quality BC for color textures

  ETC2_R8G8B8_UNORM_BLOCK = 147,    // ETC2, RGB
  ETC2_R8G8B8_SRGB_BLOCK = 148,     // ETC2, sRGB
  ETC2_R8G8B8A1_UNORM_BLOCK = 149,  // ETC2, RGBA
  ETC2_R8G8B8A1_SRGB_BLOCK = 150,   // ETC2, sRGB+A
  ETC2_R8G8B8A8_UNORM_BLOCK = 151,  // ETC2, RGBA
  ETC2_R8G8B8A8_SRGB_BLOCK = 152,   // ETC2, sRGB+a

  EAC_R11_UNORM_BLOCK = 153,
  EAC_R11_SNORM_BLOCK = 154,
  EAC_R11G11_UNORM_BLOCK = 155,
  EAC_R11G11_SNORM_BLOCK = 156,

  ASTC_4x4_UNORM_BLOCK = 157,
  ASTC_4x4_SRGB_BLOCK = 158,
  ASTC_5x4_UNORM_BLOCK = 159,
  ASTC_5x4_SRGB_BLOCK = 160,
  ASTC_5x5_UNORM_BLOCK = 161,
  ASTC_5x5_SRGB_BLOCK = 162,
  ASTC_6x5_UNORM_BLOCK = 163,
  ASTC_6x5_SRGB_BLOCK = 164,
  ASTC_6x6_UNORM_BLOCK = 165,
  ASTC_6x6_SRGB_BLOCK = 166,
  ASTC_8x5_UNORM_BLOCK = 167,
  ASTC_8x5_SRGB_BLOCK = 168,
  ASTC_8x6_UNORM_BLOCK = 169,
  ASTC_8x6_SRGB_BLOCK = 170,
  ASTC_8x8_UNORM_BLOCK = 171,
  ASTC_8x8_SRGB_BLOCK = 172,
  ASTC_10x5_UNORM_BLOCK = 173,
  ASTC_10x5_SRGB_BLOCK = 174,
  ASTC_10x6_UNORM_BLOCK = 175,
  ASTC_10x6_SRGB_BLOCK = 176,
  ASTC_10x8_UNORM_BLOCK = 177,
  ASTC_10x8_SRGB_BLOCK = 178,
  ASTC_10x10_UNORM_BLOCK = 179,
  ASTC_10x10_SRGB_BLOCK = 180,
  ASTC_12x10_UNORM_BLOCK = 181,
  ASTC_12x10_SRGB_BLOCK = 182,
  ASTC_12x12_UNORM_BLOCK = 183,
  ASTC_12x12_SRGB_BLOCK = 184,

  CMAP_RGB=  10000,         // color map, 256 colors, 1 byte per channel, 3 channels, no gl/vulkan variant
  CMAP_RGBA= 10001,         // color map, 256 colors, 1 byte per channel, 4 channels, no gl/vulkan variant

  CMAP_START_RANGE= CMAP_RGB,
  CMAP_END_RANGE= CMAP_RGBA,
  CMAP_START_INDEX= ASTC_12x12_SRGB_BLOCK+ 1
};





class Img {
public:

  // base vars

  str8 fileName;        // image base fileName, if there is one
  uint8 fileType;       // 0= TEX, 1= TGA, 2= PNG
  uint8 *bitmap;        // raw data
  uint32 dx, dy;        // image size
  uint32 depth;         // 3D texture: basically number of images combined in this object

  uint16 bpp;           // bits per pixel
  uint8 bpc[4];         // bits per channel, for all 4 possible channels
  uint8 nchannels;      // number of channels per pixel (rgb= 3, rgba= 4, etc)
  ImgFormat format;     // image format - check ImageFormat enum
  bool compressed;      // the image is compressed or not

  uint8 *cmap;          // color map palette
  //uint8 cmapBpp;        // color palette bits per color (16 / 24 / 32)

  //bool hasTrnCol;       // indicates that the image uses the transparent (trnCol);
  //struct { uint16 r, g, b; } trnCol;  // the transparent color, can be used in RGB / GREY image types
  bool packedPixels;    // images with less than 8bits per pixel can be packed or unpacked, meaning that each pixel can have 1 byte, or multiple pixels can be packed in 1 byte

  uint32 err;           // error number; 0= no error/ everything OK
  

  // funcs

  virtual bool load(cchar *);     // can be used to load a 3D texture, if the filename doesn't acutally exist on the disk
  virtual bool save(cchar *);
  virtual bool load3D(cchar *);   // filename is a basename - the first image on disk would be [basename0000.format] - can use just load(fname) instead, wich will call this func
  virtual bool save3D(cchar *);

  bool loadPalette(cchar *fname, uint8 fileBpp= 24); // cmap bits per pixel of the file. 24bpp= 768 pal size, 32bpp= 1024 pal size, 16bpp= 512 pal size
  bool savePalette(cchar *);

  inline static uint16 getR565(uint16 c) { return c>> 11; }
  inline static uint16 getG565(uint16 c) { return (c>> 5)& 0x3F; }
  inline static uint16 getB565(uint16 c) { return c& 0x1F; }

  inline static uint16 getR5551(uint16 c) { return c>> 11; }
  inline static uint16 getG5551(uint16 c) { return (c>> 6)& 0x1F; }
  inline static uint16 getB5551(uint16 c) { return (c>> 1)& 0x1F; }
  inline static uint16 getA5551(uint16 c) { return c& 0x01; }

  inline static uint16 getR1555(uint16 c) { return (c>> 10)& 0x1F; }
  inline static uint16 getG1555(uint16 c) { return (c>> 5)& 0x1F; }
  inline static uint16 getB1555(uint16 c) { return c& 0x1F; }
  inline static uint16 getA1555(uint16 c) { return c>>15; }


  cchar *getError();

  void wrap(void **p, uint32 _sx= 0, uint32 _sy= 0, ImgFormat t= ImgFormat::UNDEFINED);   // wraps this class on a different buffer, specify the buffer's sizes and internalFormat
  void stopWrapping();                  // signals to release wrapping on the specified buffer, and do it's internal allocs/deallocs
  
  void computePixelInfo(ImgFormat t= ImgFormat::UNDEFINED); // computes bpp and bpc from either supplied [t] or internal [type]

  bool convert(ImgFormat newType);      // convert image to specified new image format
  bool convertTo8bpcUNORM();            // convert the image channels to 8bpc (it can be 1/2/3/4 channels, all will be converted to 8bpc)
  bool swapChannel(uint8 c1, uint8 c2); // swaps channel [c1] and [c2] in the image
  bool packPixels();                    // for images with less than 8bpp, this packs multiple pixels under 1 byte
  bool unpackPixels();                  // for images with less than 8bpp, this unpacks pixels: each pixel will have 1 byte

  static ImgFormat compressedToUncompressed(ImgFormat);  // returns a coresponding format that would fit best the compressed format
  

  bool areSizesPowerOfTwo();            // checks if the image sizes are power of 2 (usefull for OpenGL mainly)
  static void* resample2(void *image, uint32 width, uint32 height, uint32 components, uint32 &err); // 2->1 average (linear) resampling, tex power of 2

  //bool isGlCompatible();              // checks if this internal format is OpenGL compatible
  bool glIsCompatible();                // checks if current image's format is OpenGL compatible
  bool vkIsCompatible();                // checks if current image's format is Vulkan compatible
  static uint32 mipmapGetMaxLevels(Img *); // returns the maximum number of mipmap levels the texture can have


  #ifdef IMG_CLASS_USE_OPENGL
  static int glGetDataType(ImgFormat);        // returns OpenGL data type (for use with glTexImage or something similar)
  static int glGetChannelsFormat(ImgFormat);  // returns OpenGL channel format (internalFormat
  static int glGetGlFormat(ImgFormat);        // converts ImageFormat -> OpenGL data format
  static ImgFormat glGetFormat(int glFormat); // converts OpenGL data format -> ImageFormat
  //static int glCompressedToUncompressedFormat(int format);
  //static int glUncompressedToCompressedFormat(int format);
  void glApplyGreyscaleSwizzle();          // GREY and GREY_ALPHA images, need swizzle in order to work. After a tex is created, call this func
  bool glCreateTexture(bool compressed);  // 
  int glGetNrChannels(int intData) const; // returns number of components based on texture internal format
  bool glConvertCompatible();             // CMAP->RGBA, 1/2/4/16bpc to 8bpc
  
  #endif // use OpenGL


  #ifdef IMG_CLASS_USE_VULKAN
  bool vkConvertCompatible();                       // converts the image into something Vulkan can process. Usually only CMAP images and grey1/4 are not supported
  static uint32 vkGetAspectFromFormat(ImgFormat in_format);
  #endif

  
  enum class Type: uint8 {
    T_1D= 0,
    T_2D= 1,
    T_3D= 2,
  };

  #ifdef IMG_CLASS_USE_OPENGL
  static Type glGetType(int32);      // converts GL type -> ImageType
  static int32 glGetGlType(Type);    // converts ImageType -> GL type
  #endif

  enum class Wrap: uint8 {
    REPEAT=               0,
    MIRRORED_REPEAT=      1,
    CLAMP_TO_EDGE=        2,
    CLAMP_TO_BORDER=      3,
    MIRROR_CLAMP_TO_EDGE= 4
  };

  #ifdef IMG_CLASS_USE_OPENGL
  static Wrap glGetWrap(int32);      // converts GL wrap -> ImageWrap
  static int32 glGetGlWrap(Wrap);    // converts ImageWrap -> GL wrap
  #endif

  // direct compatible with VkComponentSwizzle
  enum class Swizzle: uint8 {
    IDENTITY= 0,
    ZERO=     1,
    ONE=      2,
    R=        3,
    G=        4,
    B=        5,
    A=        6
  };

  #ifdef IMG_CLASS_USE_OPENGL
  static Swizzle glGetSwizzle(int, int);    // converts GL swizzle -> ImageSwizzle, needs to know what channel (r,g,b or a) as GL_RED, etc
  static int glGetGlSwizzle(Swizzle, int);  // converts ImageSwizzle -> GL swizzle, needs to know what channel (r,g,b or a) as GL_RED, etc
  #endif

  
  /*
  // depth compare function/operation
  enum class CompareFunc {
    NEVER= 0,           // the test never passes
    LESS= 1,            // the test passes when zf<za
    EQUAL= 2,           // the test passes when zf=za
    LESS_OR_EQUAL= 3,   // the test passes when zf≤za
    GREATER= 4,         // the test passes when zf>za
    NOT_EQUAL= 5,       // the test passes when zf≠za
    GREATER_OR_EQUAL= 6,// the test passes when zf≥za
    ALWAYS= 7           // the test always passes
  };

  #ifdef IMG_UTIL_USE_OPENGL
  static CompareFunc glGetCompareFunc(GLint);
  static GLint glGetGlCompareFunc(CompareFunc);
  #endif
  */
  
  // constructor / destructor

  Img();
  virtual ~Img() { delData(); }
  virtual void delData();                       /// if it's wrapping, it will clean the wrapped object too
   
protected:
  
  bool _wrap;           /// if this class was wrapped on a buffer, to modify it. signals to not delete [bitmap] buffer
  uint8 **wrapBitmap;   /// pointer to the bitmap pointer, that can be changed / reallocated when this class acts as a wrapper

  void _set(ImgFormat); /// sets format then updates bpp, bpc, nchannels
  void _updateWrap(Img *); // NO MORE USES DEL INCOMING
  bool _checkVars(Img *);

  static bool _loadPNG(const char *, Img *);
  static bool _savePNG(const char *, Img *);
  static bool _loadTGA(const char *, Img *);
  static bool _saveTGA(const char *, Img *);

  friend class _Img;
};





// Error numbers:

// 0 - OK - No errors
// 1 - [bitmap]/[wrapBitmap] error
// 2 - image size error
// 3 - invalid BPP / BPC
// 4 - Image already packed / unpacked
// 5 - image type does not support pixel pack/unpack operations (only [rgb16]/[grey1/2/4]/[cmap1/2/4])
// 6 - image type does not support channel swapping (only 1 channel?)
// 7 - channel number error (asking for channel 3 of a 2 channel image?)
// 8 - cannot open file
// 9 - file read error
// 10 - file write error
// 11 - cmap is empty / cmap expected - not found
// 12 - alloc error
// 13 - cmap palette not suported
// 14 - unknown / unsupported image type
// 15 - compression error
// 16 - decompression error
// 17 - PNG header not read first
// 18 - PNG IHDR chunk error
// 19 - PNG PLTE chunk error
// 20 - PNG IDAT chunk error
// 21 - PNG raw data not filled
// 22 - PNG does not support this image format


