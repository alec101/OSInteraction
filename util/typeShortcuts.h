#pragma once
#include <stdint.h>

// Rant:
//   I used to allways count on the long to have 32bits, did lots of coding based on that.
// Never do this. 'long' under win32/64 is 32bit, under linux64 _64bit_. This caused crc32 funcs to fail
// int8_t, int16_t, int32_t, int64_t are the only guaranteed integer types to have 8, 16, 32 and 64bit lengths

// NEVER COUNT ON int/short/long TO HAVE A FIXED VALUES.
//  The only guaranteed size is their MINIMUM size that they can have (this is noted after each).



// these will always have 8/16/32/64 bits

#define int8 int8_t       /// guaranteed 8bits
#define int16 int16_t     /// guaranteed 16bits
#define int32 int32_t     /// guaranteed 32bits
#define int64 int64_t     /// guaranteed 64bits

#define uint8 uint8_t     /// guaranteed 8bits
#define uint16 uint16_t   /// guaranteed 16bits
#define uint32 uint32_t   /// guaranteed 32bits
#define uint64 uint64_t   /// guaranteed 64bits

#define byte uint8_t      /// guaranteed 8bits
#define word uint16_t     /// guaranteed 16bits
#define dword uint32_t    /// guaranteed 32bits


// usefull shortcuts - NOTE: 

#define uint unsigned int           /// at least 16 bit
#define uchar unsigned char         /// at least  8 bit
#define ushort unsigned short       /// at least 16 bit 
#define ulong unsigned long         /// at least 32 bit (under Ubuntu64 this is 64bit, under win32/64 32bit)
#define ulong64 unsigned long long  /// at least 64 bit (never know what will happen in the future, 128bit? etc)

#define cint const int              /// at least 16 bit
#define cchar const char            /// at least  8 bit
#define cshort const short          /// at least 16 bit
#define clong const long            /// at least 32 bit
#define cint64 const long long      /// at least 64 bit

#define cuint const unsigned int          /// at least 16 bit
#define cuchar const unsigned char        /// at least  8 bit
#define cushort const unsigned short      /// at least 16 bit
#define culong const unsigned long        /// at least 32 bit
#define cuint64 const unsigned long long  /// at least 64 bit

#define cvoid const void

//#define string string8        // ?????????? stringClass
#define cstring const string8   // ?????????? stringClass

#define null NULL               /// i just like the null without the yelling (don't think anyone would use smallcase null for anything else)

#define wchar wchar_t           /// this is 16bit under windows, 32bit under linux, so... i recommend using stringClass)

#define MAKEUINT32(a,b,c,d) ((unsigned long)((((unsigned char)(d)|((unsigned short)((unsigned char)(c))<<8))|(((unsigned long)(unsigned char)(b))<<16))|(((unsigned long)(unsigned char)(a))<<24)))
#define GETBYTE4UINT32(a) ((unsigned char)(a))
#define GETBYTE3UINT32(a) ((unsigned char)((a)>> 8))
#define GETBYTE2UINT32(a) ((unsigned char)((a)>>16))
#define GETBYTE1UINT32(a) ((unsigned char)((a)>>24))



