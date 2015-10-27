#pragma once
#include <stdint.h>
//#include <stdio.h>
// STRING MANIPULATION FUNCTIONS ================================------------------------
///======================================================================================

// these functions act only on UTF-8 UTF-16 or UTF-32 array strings
// all funcs are OS independent - they work the same on Windows/Linux/MacOS

// string to integer and integer to string funcs are about 3 times faster than microsoft std funcs
// string to double and double to string funcs are about 20 times faster than microsoft std funcs and very precise


namespace Str {
  const uint32_t UTF16_LEAD_OFFSET= 0xD800- (0x10000>> 10);
  const uint32_t UTF16_SURROGATE_OFFSET= 0x10000- (0xD800<< 10)- 0xDC00;

  int32_t strlen8(const void *);          // size in BYTES of a UTF-8 string  (includes str terminator)
  int32_t strlen16(const uint16_t *);     // size in BYTES of a UTF-16 string (includes str terminator)
  int32_t strlen32(const uint32_t *);     // size in BYTES of a UTF-32 string (includes str terminator)

  int32_t strunicodes8(const void *);      // UTF-8 str: counts nr of unicode values
  int32_t strchars8(const void *);         // UTF-8 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs8(const void *);         // UTF-8 str: counts nr of combining diacriticals
  int32_t strunicodes16(const uint16_t *); // UTF-16 str: counts nr of unicode values
  int32_t strchars16(const uint16_t *);    // UTF-16 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs16(const uint16_t *);    // UTF-16 str: counts nr of combining diacriticals
  int32_t strunicodes32(const uint32_t *); // UTF-32 str: counts nr of unicode values
  int32_t strchars32(const uint32_t *);    // UTF-32 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs32(const uint32_t *);    // UTF-32 str: counts nr of combining diacriticals
  

  void strcpy8(void *dst, const void *src, bool terminator= true);                    // UTF-8: copies src to dst; dst must be prealocated;  <terminator>: when true, dst ends with a terminator (null)
  void strncpy8(void *dst, const void *src, int32_t, bool terminator= true);          // UTF-8: copies n unicode values from src to dst; dst must be prealocated <terminator>: when true, dst ends with a terminator (null)
  void strcpy16(uint16_t *dst, const uint16_t *src, bool terminator= true);           // UTF-16: copies src to dst; dst must be prealocated;  <terminator>: when true, dst ends with a terminator (null)
  void strncpy16(uint16_t *dst, const uint16_t *src, int32_t, bool terminator= true); // UTF-16: copies n unicode values from src to dst; dst must be prealocated <terminator>: when true, dst ends with a terminator (null)
  void strcpy32(uint32_t *dst, const uint32_t *src, bool terminator= true);           // UTF-32: copies src to dst; dst must be prealocated;  <terminator>: when true, dst ends with a terminator (null)
  void strncpy32(uint32_t *dst, const uint32_t *src, int32_t, bool terminator= true); // UTF-32: copies n unicode values from src to dst; dst must be prealocated <terminator>: when true, dst ends with a terminator (null)

  // utf-8 utf-16 utf-32 in-between conversions

  uint32_t utf8to32(const void *in_utf8, int32_t *out_bytes= NULL); // unpacks UTF-8 and returns unicode value (UTF-32); <out_bytes>: [optional] returns how many bytes has the character in UTF8 format
  void utf8to16(const void *in_utf8, uint16_t *out_utf16, int32_t *out_utf8bytes= NULL, int32_t *out_utf16ints= NULL);          // unpacks UTF-8 and packs to UTF-16 (if neccesary); in_str: UTF-8 input; out_str: UTF-16 output
  uint32_t utf16to32(const uint16_t *in_utf16, int32_t *out_nInt16= NULL); // unpacks (if surrogates present) UTF-16 and returns unicode value; out_nInt16: [optional] returns the number of int16 in the packed UTF-16 (it can be 1 or 2, basically if using surrogates or not)
  void utf16to8(const uint16_t *in_utf16, void *out_utf8);          // unpacks (if surrogates present) UTF-16 and packs into UTF-8
  int32_t utf32to8(uint32_t unicode, uint8_t *out_utf8);            // packs unicode value to UTF-8; (out_utf8 must be at least 4 bytes long); returns the number of bytes that it was packed into
  int32_t utf32to16(uint32_t unicode, uint16_t *out_utf16);         // packs unicode value to utf-16 (if surrogates are needed); returns the number of int16 that was packed into (1 or 2, 2 for surrogates)

  // utf helper funcs

  uint32_t utf8to32n(const void *, int);   // returns n-th unicode value as UTF-32 (or direct unicode number)
  int utf8nrBytes(uint32_t unicode);       // returns the number of bytes that the unicode will be packed into
  int utf8headerBytes(uint8_t b);          // returns the number of bytes that the specified utf8 header byte reports it has
  inline int utf16nrBytes(uint32_t unicode) { if(unicode>= 0x10000) return 4; return 2; } // returns the number of bytes that the unicode will have in a utf-16 string (2 or 4)

  int32_t strcmp8(const void *, const void *);          // UTF-8 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa
  int32_t strcmp16(const uint16_t *, const uint16_t *); // UTF-16 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa
  int32_t strcmp32(const uint32_t *, const uint32_t *); // UTF-32 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa

  uint32_t tolower(uint32_t);           // converts a single unicode char to lowercase (special cases that convert to multiple characters are NOT HANDLED)
  uint32_t toupper(uint32_t);           // converts a single unicode char to uppercase (special cases that convert to multiple characters are NOT HANDLED)
  
  uint8_t *getUnicode8(const void *s, int32_t n);       // UTF-8:  returns the [n] unicode value in string
  uint8_t *getChar8(const void    *s, int32_t n);       // UTF-8:  returns the [n] char in string (diacriticals are considered part of a character)
  uint16_t *getUnicode16(const uint16_t *s, int32_t n); // UTF-16: returns the [n] unicode value in string
  uint16_t *getChar16(const uint16_t *s,    int32_t n); // UTF-16: returns the [n] char in string (diacriticals are considered part of a character)
  uint32_t *getUnicode32(const uint32_t *s, int32_t n); // UTF-32: returns the [n] unicode value in string; you can just use s[n], but this checks for end string terminator, SLOW FUNC, passes thru string
  uint32_t *getChar32(const uint32_t *s,    int32_t n); // UTF-32: returns the [n] char in string (diacriticals are considered part of a character)
  


  bool isComb(uint32_t n);              // checks specified unicode value to see if this is a combining diacritical (if u dont know what they are, you can use clearComb() to remove them all from a string, and avoid headaches)
  inline bool isHighSurrogate(uint32_t n) { return (n>= 0xD800 && n<= 0xDBFF); } // checks specified value to see if it's a utf-16 high surrogate
  inline bool isLowSurrogate(uint32_t n)  { return (n>= 0xDC00 && n<= 0xDFFF); } // checks specified value to see if it's a utf-16 low surrogate
  inline bool isSurrogate(uint32_t n)     { return (n>= 0xD800 && n<= 0xDFFF); } // checks specified value to see if it's a utf-16 surrogate

  // string to number conversion
  
  int64_t  utf8toInt64(const void *, bool zecimal= false);   // string to int;  suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...); <zecimal> forces to convert from a zecimal number
  uint64_t utf8toUint64(const void *, bool zecimal= false);  // string to uint; suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...); <zecimal> forces to convert from a zecimal number
  float    utf8toFloat(const void *);   // string to float;  supports scientific exponent (e+-... / E+-...)
  double   utf8toDouble(const void *);  // string to double; supports scientific exponent (e+-... / E+-...)

  int64_t  utf16toInt64(const void *, bool zecimal= false);  // string to int;  suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  uint64_t utf16toUint64(const void *, bool zecimal= false); // string to uint; suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  float    utf16toFloat(const void *);  // string to float;  supports scientific exponent (e+-... / E+-...)
  double   utf16toDouble(const void *); // string to double; supports scientific exponent (e+-... / E+-...)

  int64_t  utf32toInt64(const void *, bool zecimal= false);  // string to int;  suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  uint64_t utf32toUint64(const void *, bool zecimal= false); // string to uint; suppors zecimal numbers, hexazecimal(0x... / 0X...), octal (0...), binary (0b... / 0B...) <zecimal> forces to convert from a zecimal number
  float    utf32toFloat(const void *);  // string to float;  supports scientific exponent (e+-... / E+-...)
  double   utf32toDouble(const void *); // string to double; supports scientific exponent (e+-... / E+-...)

  // number to string conversion
  // *supports hexa 0x/0X, octal 0(zero starting), binary 0b/0B, and zecimal numbers
  // *the output buffer for these funcs must be manually allocated (don't forget space for a str terminator):
  //  -67 characters should support any number (binary numbers are the biggest, 64bits= 64 chars + 0b+ str terminator)
  //  -21 characters for integer to zecimal and hexa are enough;
  //  -39 characters for float/double should be enough (sign, 18.18 - funcs use -/+int64.int64, but not all of int64)
  
  int int64toUtf8(int64_t,   void *out_utf8, int8_t base= 10, bool uppercase= true);  // integer to string; <out_utf8> must be preallocated; <returns> nr chars in string
  int uint64toUtf8(uint64_t, void *out_utf8, int8_t base= 10, bool uppercase= true);  // unsigned integer to string; <out_utf8> must be preallocated; <returns> nr chars in string
  int floatToUtf8(float,   void *out_utf16, int precision= 2, bool useE= false);   // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string 
  int doubleToUtf8(double, void *out_utf16, int precision= 2, bool useE= false);   // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string 

  int int64toUtf16(int64_t,   void *out_utf16, int8_t base= 10, bool uppercase= true); // integer to string; <out_utf16> must be preallocated; <returns> nr chars in string
  int uint64toUtf16(uint64_t, void *out_utf16, int8_t base= 10, bool uppercase= true); // unsigned integer to string; <out_utf16> must be preallocated; <returns> nr chars in string
  int floatToUtf16(float,   void *out_utf16, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string
  int doubleToUtf16(double, void *out_utf16, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string

  int int64toUtf32(int64_t,   void *out_utf32, int8_t base= 10, bool uppercase= true); // integer to string; <out_utf32> must be preallocated; <returns> nr chars in string
  int uint64toUtf32(uint64_t, void *out_utf32, int8_t base= 10, bool uppercase= true); // unsigned integer to string; <out_utf32> must be preallocated; <returns> nr chars in string
  int floatToUtf32(float,   void *out_utf32, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string
  int doubleToUtf32(double, void *out_utf32, int precision= 2, bool useE= false);  // [max 18chars.18chars or 1char.18chars and scientific exponent] <return>: nr chars in string

  // character or string insertion / deletion from a string

  int32_t insert8(void      **out_str, uint32_t in_unicode, int32_t in_pos= -1); // <in_unicode>-unicode value to insert <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (including str terminator)
  int32_t insert16(uint16_t **out_str, uint32_t in_unicode, int32_t in_pos= -1); // <in_unicode>-unicode value to insert <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insert32(uint32_t **out_str, uint32_t in_unicode, int32_t in_pos= -1); // <in_unicode>-unicode value to insert <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr8(void      **out_str, const void     *in_str, int32_t in_pos= -1); // <in_str>- string to insert (UTF8) <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr16(uint16_t **out_str, const uint16_t *in_str, int32_t in_pos= -1); // <in_str>- string to insert (UTF16) <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr32(uint32_t **out_str, const uint32_t *in_str, int32_t in_pos= -1); // <in_str>- string to insert (UTF32) <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t del8(void      **out_str, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF8 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del16(uint16_t **out_str, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF16 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del32(uint32_t **out_str, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF32 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)

  // next functions act on the supplied fixed size buffer, no memory allocations are made

  int32_t insert8static(void      *out_buf, int32_t in_bufSize, uint32_t in_unicode, int32_t in_pos= -1);   // <in_bufSize>- [REQUIRED] out_buf's size, in bytes; <in_unicode>- unicode value to insert; <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insert16static(uint16_t *out_buf, int32_t in_bufSize, uint32_t in_unicode, int32_t in_pos= -1);   // <in_bufSize>- [REQUIRED] out_buf's size, in int16 units; <in_unicode>- unicode value to insert; <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insert32static(uint32_t *out_buf, int32_t in_bufSize, uint32_t in_unicode, int32_t in_pos= -1);   // <in_bufSize>- [REQUIRED] out_buf's size, in int32 units; <in_unicode>- unicode value to insert; <in_pos>- insert position (selected unicode value at position is moved to the right); if left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr8static(void      *out_buf, int32_t in_bufSize, const void *in_str,     int32_t in_pos= -1); // <in_bufSize>- [REQUIRED] out_buf's size, in bytes; <in_str>- str to insert (UTF8); <in_pos>- insert position (selected unicode value at position is moved to the right); if  left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr16static(uint16_t *out_buf, int32_t in_bufSize, const uint16_t *in_str, int32_t in_pos= -1); // <in_bufSize>- [REQUIRED] out_buf's size, in int16 units; <in_str>- str to insert (UTF16); <in_pos>- insert position (selected unicode value at position is moved to the right); if  left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t insertStr32static(uint32_t *out_buf, int32_t in_bufSize, const uint32_t *in_str, int32_t in_pos= -1); // <in_bufSize>- [REQUIRED] out_buf's size, in int32 units; <in_str>- str to insert (UTF32); <in_pos>- insert position (selected unicode value at position is moved to the right); if  left -1, unicode is inserted at the end of the string; <returns> resulting string length in bytes (incl str terminator)
  int32_t del8static(void      *out_buf, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF8 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del16static(uint16_t *out_buf, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF16 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  int32_t del32static(uint32_t *out_buf, int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nUnicodesToDel>- number of unicodes to del from UTF32 str; <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied; if it remains -1, the last unicode in the string is the position; <returns> resulting string length in bytes (incl str terminator)
  
  // search in string funcs

  uint8_t *search8(const void *in_utf8, const void *in_search, bool in_caseSensitive= true);
  uint16_t *search16(const uint16_t *in_utf16, const uint16_t *in_search, bool in_caseSensitive= true);
  uint32_t *search32(const uint32_t *in_utf32, const uint32_t *in_search, bool in_caseSensitive= true);

  // utility

  inline void memclr(void *dst, uint32_t n, uint8_t v= 0) { for(uint8_t *p= (uint8_t *)dst; n> 0; n--) *p++= v; }

  // copies 1 byte at a time
  inline void memcpy(void *dst, const void *src, uint32_t n) { for(int8_t *p= (int8_t *)dst, *t= (int8_t *)src; n> 0; n--) *p++= *t++; }
  // copies 4 bytes at a time - better for bigger memory copy operations
  inline void memcpy32(void *dst, const void *src, uint32_t n) {
    uint32_t i, *p1, *p2;
    uint8_t *p3, *p4;

    for(i= n/ 4, p1= (uint32_t *)dst, p2= (uint32_t *)src; i; )
      *p1++= *p2++, i--;

    for(i= n% 4, p3= (uint8_t *)p1,   p4= (uint8_t *)p2;   i; )
      *p3++= *p4++, i--;
  }

  // copies 8 bytes at a time - better for bigger memory copy operations
  inline void memcpy64(void *dst, const void *src, uint64_t n) {
    uint64_t i, *p1, *p2;
    uint8_t *p3, *p4;

    for(i= n/ 8, p1= (uint64_t *)dst, p2= (uint64_t *)src; i;)
      *p1++= *p2++, i--;

    for(i= n% 8, p3= (uint8_t *)p1, p4= (uint8_t *)p2; i; )
      *p3++= *p4++, i--;

  }

  // other versions: 

  //inline void memcpy(void *dst, const void *src, uint32_t n) { for(uint32_t a= 0; a< n; a++) ((int8_t *)dst)[a]= ((int8_t *)src)[a]; }
  //inline void memclr(void *dst, uint32_t n, uint8_t v= 0) { for(uint32_t a= 0; a< n; a++) ((int8_t *)dst)[a]= v;

  //inline void memcpy(void *dst, const void *src, uint64_t n) { for(uint64_t a= 0; a< n; a++) ((int8_t *)dst)[a]= ((int8_t *)src)[a]; }
  //inline void memcpy(void *dst, const void *src, uint64_t n) { int8_t *p1= (int8_t *)dst, *p2= (int8_t *)src; for(uint64_t a= 0; a< n; a++, p1++, p2++) *p1= *p2; }
}








