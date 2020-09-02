#pragma once
#ifndef NULL
  #ifdef __cplusplus
    #define NULL 0
  #else
    #define NULL ((void *)0)
  #endif
#endif

#include <stdint.h>

  /*
  on the 'outside', everything can be char, char16_t, char32_t,
  but on the inside, fix values are used (uint8, uint16, uint32)
  the difference between char on mac and char on windows should not matter
  basically, these are treated as [___void * pointers___], theres only their names as char, char16, char32
  this would be the 'clean' variant...

    ^^^ i think this is the winner


    this would be the final polish
    */

// STRING MANIPULATION FUNCTIONS ================================------------------------
///======================================================================================

// these functions act only on UTF-8 UTF-16 or UTF-32 array strings
// all funcs are OS independent - they work the same on Windows/Linux/MacOS

// string to integer and integer to string funcs are about 3 times faster than microsoft std funcs
// string to double and double to string funcs are about 20 times faster than microsoft std funcs and very precise

// the difference between char on windows (default signed) and char on macs (default unsigned) will not affect the end result of strings

// if char or char16_t or char32_t are bigger than 8, 16 and 32bits respectively, it will not matter,
// the functions are using (u)int8_t, (u)int16_t and (u)int32_t internally, which are guaranteed to be 8, 16 and 32bits

namespace Str {
  const uint32_t UNICODE_MAX= 0x10FFFF;
  const uint32_t UTF16_LEAD_OFFSET= 55232U;           // 0xD800-  (0x10000>> 10);
  const uint32_t UTF16_SURROGATE_OFFSET= 4238353408U; // 0x10000- (0xD800<< 10)- 0xDC00;

  int32_t strlen8(const char *);          // size in BYTES of a UTF-8 string  (includes str terminator)
  int32_t strlen16(const char16_t *);     // size in BYTES of a UTF-16 string (includes str terminator)
  int32_t strlen32(const char32_t *);     // size in BYTES of a UTF-32 string (includes str terminator)

  int32_t strunicodes8(const char *);      // UTF-8 str: counts nr of unicode values
  int32_t strchars8(const char *);         // UTF-8 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs8(const char *);         // UTF-8 str: counts nr of combining diacriticals
  int32_t strunicodes16(const char16_t *); // UTF-16 str: counts nr of unicode values
  int32_t strchars16(const char16_t *);    // UTF-16 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs16(const char16_t *);    // UTF-16 str: counts nr of combining diacriticals
  int32_t strunicodes32(const char32_t *); // UTF-32 str: counts nr of unicode values
  int32_t strchars32(const char32_t *);    // UTF-32 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs32(const char32_t *);    // UTF-32 str: counts nr of combining diacriticals
  

  void strcpy8(char *dst, const char *src, bool terminator= true);                    // UTF-8: copies src to dst; dst must be prealocated;  <terminator>: when true, dst ends with a terminator (null)
  void strncpy8(char *dst, const char *src, int32_t, bool terminator= true);          // UTF-8: copies n unicode values from src to dst; dst must be prealocated <terminator>: when true, dst ends with a terminator (null)
  void strcpy16(char16_t *dst, const char16_t *src, bool terminator= true);           // UTF-16: copies src to dst; dst must be prealocated;  <terminator>: when true, dst ends with a terminator (null)
  void strncpy16(char16_t *dst, const char16_t *src, int32_t, bool terminator= true); // UTF-16: copies n unicode values from src to dst; dst must be prealocated <terminator>: when true, dst ends with a terminator (null)
  void strcpy32(char32_t *dst, const char32_t *src, bool terminator= true);           // UTF-32: copies src to dst; dst must be prealocated;  <terminator>: when true, dst ends with a terminator (null)
  void strncpy32(char32_t *dst, const char32_t *src, int32_t, bool terminator= true); // UTF-32: copies n unicode values from src to dst; dst must be prealocated <terminator>: when true, dst ends with a terminator (null)

  // utf-8 utf-16 utf-32 in-between conversions

  char32_t utf8to32(const char *in_utf8, int32_t *out_bytes= NULL); // unpacks UTF-8 and returns unicode value (UTF-32); <out_bytes>: [optional] returns how many bytes has the character in UTF8 format
  
  void utf8to16(const char *in_utf8, char16_t *out_utf16, int32_t *out_utf8bytes= NULL, int32_t *out_utf16ints= NULL);          // unpacks UTF-8 and packs to UTF-16 (if neccesary); in_str: UTF-8 input; out_str: UTF-16 output
  char32_t utf16to32(const char16_t *in_utf16, int32_t *out_nInt16= NULL); // unpacks (if surrogates present) UTF-16 and returns unicode value; out_nInt16: [optional] returns the number of int16 in the packed UTF-16 (it can be 1 or 2, basically if using surrogates or not)
  void utf16to8(const char16_t *in_utf16, char *out_utf8);          // unpacks (if surrogates present) UTF-16 and packs into UTF-8
  int32_t utf32to8(char32_t unicode, char *out_utf8);               // packs unicode value to UTF-8; (out_utf8 must be at least 4 bytes long); returns the number of bytes that it was packed into
  int32_t utf32to16(char32_t unicode, char16_t *out_utf16);         // packs unicode value to utf-16 (if surrogates are needed); returns the number of int16 that was packed into (1 or 2, 2 for surrogates)

  // utf helper funcs

  char32_t utf8to32n(const char *, int);   // returns n-th unicode value as UTF-32 (or direct unicode number)
  int utf8nrBytes(char32_t unicode);       // returns the number of bytes that the unicode will be packed into
  int utf8headerBytes(char b);            // returns the number of bytes that the specified utf8 header byte reports it has
  inline int utf16nrBytes(char32_t unicode) { if(unicode>= 0x10000) return 4; return 2; } // returns the number of bytes that the unicode will have in a utf-16 string (2 or 4)

  int32_t strcmp8(const char *,      const char *);     // UTF-8 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa
  int32_t strcmp16(const char16_t *, const char16_t *); // UTF-16 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa
  int32_t strcmp32(const char32_t *, const char32_t *); // UTF-32 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa

  char32_t tolower(char32_t);           // converts a single unicode char to lowercase (special cases that convert to multiple characters are NOT HANDLED)
  char32_t toupper(char32_t);           // converts a single unicode char to uppercase (special cases that convert to multiple characters are NOT HANDLED)
  
  char *getUnicode8(const char *s, int32_t n);          // UTF-8:  returns the [n] unicode value in string
  char *getChar8(const char    *s, int32_t n);          // UTF-8:  returns the [n] char in string (diacriticals are considered part of a character)
  char16_t *getUnicode16(const char16_t *s, int32_t n); // UTF-16: returns the [n] unicode value in string
  char16_t *getChar16(const char16_t *s,    int32_t n); // UTF-16: returns the [n] char in string (diacriticals are considered part of a character)
  char32_t *getUnicode32(const char32_t *s, int32_t n); // UTF-32: returns the [n] unicode value in string; you can just use s[n], but this checks for end string terminator, SLOW FUNC, passes thru string
  char32_t *getChar32(const char32_t *s,    int32_t n); // UTF-32: returns the [n] char in string (diacriticals are considered part of a character)

  int32_t getUnicodesInChar8(const char *s);        // UTF-8:  returns the number of unicodes a char is composed of (how many diacriticals+ the actual char glyph)
  int32_t getUnicodesInChar16(const char16_t *s);   // UTF-16: returns the number of unicodes a char is composed of (how many diacriticals+ the actual char glyph)
  int32_t getUnicodesInChar32(const char32_t *s);   // UTF-32: returns the number of unicodes a char is composed of (how many diacriticals+ the actual char glyph)
  int32_t getUnicodesInChars8(const char *in_s, int32_t in_maxChars);   // UTF-8:  returns the number of unicodes in a text with <in_maxChars> number of characters
  int32_t getUnicodesInChars16(const char16_t *in_s, int32_t in_maxChars);  // UTF-16: returns the number of unicodes in a text with <in_maxChars> number of characters
  int32_t getUnicodesInChars32(const char32_t *in_s, int32_t in_maxChars);  // UTF-32: returns the number of unicodes in a text with <in_maxChars> number of characters

  bool isComb(char32_t n);              // checks specified unicode value to see if this is a combining diacritical (if u dont know what they are, you can use clearComb() to remove them all from a string, and avoid headaches)
  inline bool isHighSurrogate(uint32_t n) { return (n>= 0xD800 && n<= 0xDBFF); } // checks specified value to see if it's a utf-16 high surrogate
  inline bool isLowSurrogate(uint32_t n)  { return (n>= 0xDC00 && n<= 0xDFFF); } // checks specified value to see if it's a utf-16 low surrogate
  inline bool isSurrogate(uint32_t n)     { return (n>= 0xD800 && n<= 0xDFFF); } // checks specified value to see if it's a utf-16 surrogate

  // string to number conversion
  
  int64_t  utf8toInt64(const char *, bool zecimal= false);   // string to int;  suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...); <zecimal> forces to convert from a zecimal number
  uint64_t utf8toUint64(const char *, bool zecimal= false);  // string to uint; suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...); <zecimal> forces to convert from a zecimal number
  float    utf8toFloat(const char *);   // string to float;  supports scientific exponent (e+-... / E+-...)
  double   utf8toDouble(const char *);  // string to double; supports scientific exponent (e+-... / E+-...)

  int64_t  utf16toInt64(const char16_t *, bool zecimal= false);  // string to int;  suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  uint64_t utf16toUint64(const char16_t *, bool zecimal= false); // string to uint; suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  float    utf16toFloat(const char16_t *);  // string to float;  supports scientific exponent (e+-... / E+-...)
  double   utf16toDouble(const char16_t *); // string to double; supports scientific exponent (e+-... / E+-...)

  int64_t  utf32toInt64(const char32_t *, bool zecimal= false);  // string to int;  suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  uint64_t utf32toUint64(const char32_t *, bool zecimal= false); // string to uint; suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  float    utf32toFloat(const char32_t *);  // string to float;  supports scientific exponent (e+-... / E+-...)
  double   utf32toDouble(const char32_t *); // string to double; supports scientific exponent (e+-... / E+-...)

  // number to string conversion
  // *supports hexa 0x/0X, octal 0(zero starting), binary 0b/0B, and zecimal numbers
  // *the output buffer for these funcs must be manually allocated (don't forget space for a str terminator):
  //  -67 characters should support any number (binary numbers are the biggest, 64bits= 64 chars + 0b+ str terminator)
  //  -21 characters for integer to zecimal and hexa are enough;
  //  -39 characters for float/double should be enough (sign, 18.18 - funcs use -/+int64.int64, but not all of int64)
  
  int int64toUtf8(int64_t,   char *out_utf8, int8_t base= 10, bool uppercase= true);  // integer to string; <out_utf8> must be preallocated; <returns> nr chars in string
  int uint64toUtf8(uint64_t, char *out_utf8, int8_t base= 10, bool uppercase= true);  // unsigned integer to string; <out_utf8> must be preallocated; <returns> nr chars in string
  int floatToUtf8(float,   char *out_utf8, int precision= 2, bool useE= false);   // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string 
  int doubleToUtf8(double, char *out_utf8, int precision= 2, bool useE= false);   // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string 

  int int64toUtf16(int64_t,   char16_t *out_utf16, int8_t base= 10, bool uppercase= true); // integer to string; <out_utf16> must be preallocated; <returns> nr chars in string
  int uint64toUtf16(uint64_t, char16_t *out_utf16, int8_t base= 10, bool uppercase= true); // unsigned integer to string; <out_utf16> must be preallocated; <returns> nr chars in string
  int floatToUtf16(float,   char16_t *out_utf16, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string
  int doubleToUtf16(double, char16_t *out_utf16, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string

  int int64toUtf32(int64_t,   char32_t *out_utf32, int8_t base= 10, bool uppercase= true); // integer to string; <out_utf32> must be preallocated; <returns> nr chars in string
  int uint64toUtf32(uint64_t, char32_t *out_utf32, int8_t base= 10, bool uppercase= true); // unsigned integer to string; <out_utf32> must be preallocated; <returns> nr chars in string
  int floatToUtf32(float,   char32_t *out_utf32, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string
  int doubleToUtf32(double, char32_t *out_utf32, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string

  // character or string insertion / deletion from a string

  int32_t insert8(char      **out_str, char32_t in_unicode, int32_t in_pos= -1); // <in_unicode>-unicode value to insert <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (including str terminator)
  int32_t insert16(char16_t **out_str, char32_t in_unicode, int32_t in_pos= -1); // <in_unicode>-unicode value to insert <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insert32(char32_t **out_str, char32_t in_unicode, int32_t in_pos= -1); // <in_unicode>-unicode value to insert <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr8(char      **out_str, const char     *in_str, int32_t n= -1, int32_t in_pos= -1); // <in_str>- string to insert (UTF8);  <n>- nr unicodes to insert; when -1(default), will insert the whole string; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string;  <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr16(char16_t **out_str, const char16_t *in_str, int32_t n= -1, int32_t in_pos= -1); // <in_str>- string to insert (UTF16); <n>- nr unicodes to insert; when -1(default), will insert the whole string; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string;  <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr32(char32_t **out_str, const char32_t *in_str, int32_t n= -1, int32_t in_pos= -1); // <in_str>- string to insert (UTF32); <n>- nr unicodes to insert; when -1(default), will insert the whole string; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string;  <returns> resulting string length in bytes (incl str terminator)

  int32_t del8(char      **out_str, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF8 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del16(char16_t **out_str, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF16 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del32(char32_t **out_str, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF32 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)

  // next functions act on the supplied fixed size buffer, no memory allocations are made

  int32_t insert8static(char      *out_buf, int32_t in_bufSize, char32_t in_unicode, int32_t in_pos= -1);   // <in_bufSize>- [REQUIRED] out_buf's size, in bytes; <in_unicode>- unicode value to insert; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insert16static(char16_t *out_buf, int32_t in_bufSize, char32_t in_unicode, int32_t in_pos= -1);   // <in_bufSize>- [REQUIRED] out_buf's size, in int16 units; <in_unicode>- unicode value to insert; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insert32static(char32_t *out_buf, int32_t in_bufSize, char32_t in_unicode, int32_t in_pos= -1);   // <in_bufSize>- [REQUIRED] out_buf's size, in int32 units; <in_unicode>- unicode value to insert; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr8static(char      *out_buf, int32_t in_bufSize, const char *in_str,     int32_t n, int32_t in_pos= -1); // <in_bufSize>- [REQUIRED] out_buf's size, in bytes;       <in_str>- str to insert (UTF8);  <n>- nr unicodes to insert; when -1(default), will insert the whole string; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr16static(char16_t *out_buf, int32_t in_bufSize, const char16_t *in_str, int32_t n, int32_t in_pos= -1); // <in_bufSize>- [REQUIRED] out_buf's size, in int16 units; <in_str>- str to insert (UTF16); <n>- nr unicodes to insert; when -1(default), will insert the whole string; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr32static(char32_t *out_buf, int32_t in_bufSize, const char32_t *in_str, int32_t n, int32_t in_pos= -1); // <in_bufSize>- [REQUIRED] out_buf's size, in int32 units; <in_str>- str to insert (UTF32); <n>- nr unicodes to insert; when -1(default), will insert the whole string; <in_pos>- insert position (in unicodes); when -1 (default), position is end of string; <returns> resulting string length in bytes (incl str terminator)
  
  int32_t del8static(char      *out_buf, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF8 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del16static(char16_t *out_buf, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF16 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del32static(char32_t *out_buf, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF32 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  
  // search in string funcs

  char     *search8 (const char *in_utf8,      const char *in_search,     bool in_caseSensitive= true);
  char16_t *search16(const char16_t *in_utf16, const char16_t *in_search, bool in_caseSensitive= true);
  char32_t *search32(const char32_t *in_utf32, const char32_t *in_search, bool in_caseSensitive= true);

  // utility

  inline void memclr(void *dst, uint32_t n, uint8_t v= 0) {
    uint64_t *p1;
    uint8_t *p2;
    uint32_t i;

    for(i= n/ 8, p1= (uint64_t *)dst; i; i--)
      *p1++= v;
    for(i= n% 8, p2= (uint8_t *)p1; i; i--)
      *p2++= v;
  }

  // copies 8 bytes at a time - 2x slower than Microsoft memcpy
  inline void memcpy(void *dst, const void *src, uint64_t n) {
    if(dst< src) {
      
      uint64_t *p1= (uint64_t *)dst, *p2= (uint64_t *)src;
      while(n>= 8)                /// 8 bytes at a time copy
        *p1++= *p2++, n-= 8;

      uint8_t *p3= (uint8_t *)p1, *p4= (uint8_t *)p2;
      while(n)                    /// rest of bytes 1 by 1 (max 7)
        *p3++= *p4++, n--;

    } else {
      uint64_t *p1= (uint64_t *)((uint8_t *)dst+ n), *p2= (uint64_t *)((uint8_t *)src+ n);
      while(n>= 8)                /// 8 bytes at a time copy
        *--p1= *--p2, n-= 8;

      uint8_t *p3= (uint8_t *)p1, *p4= (uint8_t *)p2;
      while(n)                    /// rest of bytes 1 by 1 (max 7)
        *--p3= *--p4, n--;
    }
  }

  inline void memcpyWIP(void *dst, const void *src, uint64_t n) {
    uint64_t n64= (n/ 8);
    uint64_t n8= (n% 8);

    // special case - a copy from same buffer and src < dst
    if(src< dst)
      if((uint8_t *)src+ n>= dst) {

        if(n8)                                    /// copy bytes 1 by 1 (max 7)
          for(int64_t i= n- 1; i> (int64_t)(n- n8- 1); i--)
            ((uint8_t *)dst)[i]= ((uint8_t *)src)[i];

        if(n64)
          for(int64_t i= n64- 1; i>= 0; i--)      /// copy 8 bytes at a time
            ((uint64_t *)dst)[i]= ((uint64_t *)src)[i];

        return;
      }

    // normal copy
    if(n64)
      for(uint64_t i= 0; i< n64; i++)             /// copy 8 bytes at a time
        ((uint64_t *)dst)[i]= ((uint64_t *)src)[i];
    
    if(n8) {                                      /// rest of bytes 1 by 1 (max 7)
      dst= (uint64_t *)dst+ n64;
      src= (uint64_t *)src+ n64;
      for(uint64_t i= 0; i< n8; i++)
        ((uint8_t *)dst)[i]= ((uint8_t *)src)[i];
    }
  }




  // unpacks UTF-8 and returns unicode value (UTF-32) 
  inline uint8_t *utf8to32fast(const uint8_t *in_txt, uint32_t *out_unicode) {
    if(*in_txt < 128) {                    /// character uses 1 byte (ascii 0-127)
      *out_unicode= *in_txt++;
    } else if((*in_txt& 0xe0) == 0xc0) {   /// character uses 2 bytes
      *out_unicode= (*in_txt++)& 0x1f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
    } else if((*in_txt& 0xf0) == 0xe0) {   /// character uses 3 bytes
      *out_unicode= (*in_txt++)& 0x0f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
    } else if((*in_txt& 0xf8) == 0xf0) {   /// character uses 4 bytes
      *out_unicode= (*in_txt++)& 0x07;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
    } else if((*in_txt& 0xfc) == 0xf8) {   /// character uses 5 bytes
      // the last 2 bytes are not used, but printed if in the future unicode will expand
      *out_unicode= (*in_txt++)& 0x03;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
    } else if((*in_txt& 0xfe) == 0xfc) {   /// character uses 6 bytes
      *out_unicode= (*in_txt++)& 0x01;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
      *out_unicode<<= 6; *out_unicode+= (*in_txt++)& 0x3f;
    } else 
      *out_unicode= 0xFFFD;
    return (uint8_t *)in_txt;
  }

  // unpacks UTF-16 and returns unicode value (UTF-32)
  inline uint16_t *utf16to32fast(const uint16_t *in_txt, uint32_t *out_unicode) {
    if(Str::isHighSurrogate(*in_txt))
      *out_unicode= (*in_txt<< 10)+ *(in_txt+ 1)+ Str::UTF16_SURROGATE_OFFSET, in_txt+= 2;
    else
      *out_unicode= *in_txt, in_txt++;
    return (uint16_t *)in_txt;
  }

  // other versions: 

  //inline void memcpy(void *dst, const void *src, uint32_t n) { for(uint32_t a= 0; a< n; a++) ((int8_t *)dst)[a]= ((int8_t *)src)[a]; }
  //inline void memclr(void *dst, uint32_t n, uint8_t v= 0) { for(uint32_t a= 0; a< n; a++) ((int8_t *)dst)[a]= v;

  //inline void memcpy(void *dst, const void *src, uint64_t n) { for(uint64_t a= 0; a< n; a++) ((int8_t *)dst)[a]= ((int8_t *)src)[a]; }
  //inline void memcpy(void *dst, const void *src, uint64_t n) { int8_t *p1= (int8_t *)dst, *p2= (int8_t *)src; for(uint64_t a= 0; a< n; a++, p1++, p2++) *p1= *p2; }
} /// namespace Str








