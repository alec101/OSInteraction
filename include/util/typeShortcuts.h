#pragma once
#include <stdint.h>


// these will always have 8/16/32/64 bits - fixed sizes (NO MORE, NO LESS)

#define int8 int8_t       // guaranteed  8 bit
#define int16 int16_t     // guaranteed 16 bit
#define int32 int32_t     // guaranteed 32 bit
#define int64 int64_t     // guaranteed 64 bit

#define uint8 uint8_t     // guaranteed  8 bit
#define uint16 uint16_t   // guaranteed 16 bit
#define uint32 uint32_t   // guaranteed 32 bit
#define uint64 uint64_t   // guaranteed 64 bit

#define cint8  const int8_t   // guaranteed  8 bit
#define cint16 const int16_t  // guaranteed 16 bit
#define cint32 const int32_t  // guaranteed 32 bit
#define cint64 const int64_t  // guaranteed 64 bit

#define cuint8  const uint8_t   // guaranteed  8 bit
#define cuint16 const uint16_t  // guaranteed 16 bit
#define cuint32 const uint32_t  // guaranteed 32 bit
#define cuint64 const uint64_t  // guaranteed 64 bit

#define char8 uint8_t           // on mac, char= unsigned int8, in windows char= signed int8
#define cchar8 const uint8_t    //
#define char16 char16_t         //
#define char32 char32_t         //
#define cchar16 const char16_t  //
#define cchar32 const char32_t  //

#define byte uint8_t      // guaranteed  8 bit
#define word uint16_t     // guaranteed 16 bit
#define dword uint32_t    // guaranteed 32 bit
#define qword uint64_t    // guaranteed 64 bit

// useful shortcuts - these can vary in size, but they have at LEAST n bits (check each comment for n)

#define uint unsigned int           // at least 16 bit (usually this is 32bit, nowadays... note: nowadays)
#define uchar unsigned char         // at least  8 bit
#define ushort unsigned short       // at least 16 bit 
#define ulong unsigned long         // at least 32 bit (under Ubuntu64 this is 64bit, under win32/64 32bit)
#define ulong64 unsigned long long  // at least 64 bit (never know what will happen in the future, 128bit? etc)

#define cint const int              // at least 16 bit
#define cchar const char            // at least  8 bit
#define cshort const short          // at least 16 bit
#define clong const long            // at least 32 bit
#define clong64 const long long     // at least 64 bit

#define cuint const unsigned int          // at least 16 bit
#define cuchar const unsigned char        // at least  8 bit
#define cushort const unsigned short      // at least 16 bit
#define culong const unsigned long        // at least 32 bit
#define culong64 const unsigned long long // at least 64 bit

#define cvoid const void

//#define string str8        // ?????????? stringClass
//#define cstring const str8   // ?????????? stringClass

#define cfloat const float
#define cdouble const double

#ifndef NULL
#define NULL 0
#endif

// Microsoft alignment
#if defined(_MSC_VER)
#ifndef ALIGNED
#define ALIGNED(x) __declspec(align(x))
#endif
#else
// clang / gnu alignment
//#if defined(__GNUC__)
#ifndef ALIGNED
#define ALIGNED(x) __attribute__ ((aligned(x)))
#endif
//#endif
#endif



#define null NULL

#define wchar wchar_t           // this is 16bit under windows, 32bit under linux, so... i would recommand against using this

#define MAKEUINT32(a,b,c,d) ((uint32)((((uint8)(d)|((uint16)((uint8)(c))<<8))|(((uint32)(uint8)(b))<<16))|(((uint32)(uint8)(a))<<24)))
#define GETBYTE4UINT32(a) ((uint8)(a))
#define GETBYTE3UINT32(a) ((uint8)((a)>> 8))
#define GETBYTE2UINT32(a) ((uint8)((a)>>16))
#define GETBYTE1UINT32(a) ((uint8)((a)>>24))



