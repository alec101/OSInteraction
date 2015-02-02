#pragma once

// STRING MANIPULATION FUNCTIONS ================================------------------------
///======================================================================================

// these functions act only on UTF-8 or UTF-32 array strings (UTF-16 is on the horizon)
// all funcs are OS independent - they work the same on Windows/Linux/MacOS


namespace Str {
  int32_t strlen8(const void *);          /// size in BYTES of a UTF-8 string
  int32_t strlen32(const uint32_t *);       /// size in BYTES of a UTF-32 string
  int32_t strlenWin(const uint16_t *);      /// size in BYTES of a windows 16bit string

  int32_t strchars8(const void *);         /// UTF-8 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs8(const void *);         /// UTF-8 str: counts nr of combining diacriticals
  int32_t strunicodes8(const void *);      /// UTF-8 str: counts nr of unicode values (both chars and combs)
  int32_t strchars32(const uint32_t *);      /// UTF-32 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32_t strcombs32(const uint32_t *);      /// UTF-32 str: counts nr of combining diacriticals
  int32_t strunicodes32(const uint32_t *);   /// UTF-32 str: counts nr of unicode values (both chars and combs)

  void strcpy8(void *dst, const void *src);                   /// UTF-8 str: copies src to dst; dst must be prealocated
  void strncpy8(void *dst, const void *src, int32_t);           /// UTF-8 str: copies n CHARACTERS from src to dst; if combining diacriticals are found, they are treated as part of a full character and copied with that character
  void strncpy8_unicodes(void *dst, const void *src, int32_t);  /// UTF-8 str: copies n unicode values from src to dst; dst must be prealocated; does not care if a unicode value is a character or diacritical
  void strcpy32(uint32_t *dst, const uint32_t *src);                  /// UTF-32 str: copies src to dst; dst must be prealocated
  void strncpy32(uint32_t *dst, const uint32_t *src, int32_t);          /// UTF-32 str: copies n CHARS from src to dst; if combining diacriticals are found, they are treated as part of a full character and copied with that character
  void strncpy32_unicodes(uint32_t *dst, const uint32_t *src, int32_t); /// UTF-32 str: copies n unicode values from src to dst; dst must be prealocated; does not care if a unicode value is a character or diacritical

  uint32_t utf8to32(const void *);         /// returns unicode value as UTF-32 (or direct unicode number)
  uint32_t utf8to32n(const void *, int);   /// returns n-th unicode value as UTF-32 (or direct unicode number)

  int32_t strcmp8(const void *, const void *);       /// UTF-8 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa
  int32_t strcmp32(const uint32_t *, const uint32_t *);  /// UTF-32 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa

  uint32_t tolower(uint32_t);           /// converts a single unicode char to lowercase (special cases that convert to multiple characters are NOT HANDLED)
  uint32_t toupper(uint32_t);           /// converts a single unicode char to uppercase (special cases that convert to multiple characters are NOT HANDLED)
  
  void *getChar8(const void *s, int32_t n);         /// UTF-8 str: returns [n]-th char in string (diacriticals are considered part of a character)
  void *getUnicode8(const void *s, int32_t n);      /// UTF-8 str: returns the [n] unicode value in string (does not care if it's a normal character or diacritical)
  uint32_t *getChar32(const uint32_t *s, int32_t n);    /// UTF-32 str: returns [n]-th char in string (diacriticals are considered part of a character)
  inline uint32_t getUnicode32(const uint32_t *s, int32_t n) { return s[n]; } /// UTF-32 str: returns the [n] unicode value in string (does not care if it's a normal character or diacritical)

  bool isComb(uint32_t n);           /// checks specified unicode value to see if this is a combining diacritical (if u dont know what they are, you can use clearComb() to remove them all from a string, and avoid headaches)



  inline void memcpy(void *dst, const void *src, uint32_t n) { int8_t *p= (int8_t *)dst, *t= (int8_t *)src; for(uint32_t a= 0; a< n; a++) *p++= *t++; }
  inline void memclr(void *dst, uint32_t n, uint8_t v= 0) { uint8_t *p= (uint8_t *)dst; for(uint32_t a= 0; a< n; a++) *p++= v; }

  // other versions: 

  //inline void memcpy(void *dst, cvoid *src, uint32 n) { for(uint32 a= 0; a< n; a++) ((int8 *)dst)[a]= ((int8 *)src)[a]; }
  //inline void memclr(void *dst, uint32 n, uint8 v= 0) { for(uint32 a= 0; a< n; a++) ((int8 *)dst)[a]= v;

  //inline void memcpy(void *dst, cvoid *src, uint64 n) { for(uint64 a= 0; a< n; a++) ((int8 *)dst)[a]= ((int8 *)src)[a]; }
  //inline void memcpy(void *dst, cvoid *src, uint64 n) { int8 *p1= (int8 *)dst, *p2= (int8 *)src; for(uint64 a= 0; a< n; a++, p1++, p2++) *p1= *p2; }
}








