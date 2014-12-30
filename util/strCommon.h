#pragma once

// STRING MANIPULATION FUNCTIONS ================================------------------------
///======================================================================================

// these functions act only on UTF-8 or UTF-32 array strings (UTF-16 is on the horizon)
// all funcs are OS independent - they work the same on Windows/Linux/MacOS


namespace Str {
  int32 strlen8(cvoid *);          /// size in BYTES of a UTF-8 string
  int32 strlen32(cuint32 *);       /// size in BYTES of a UTF-32 string
  int32 strlenWin(cuint16 *);      /// size in BYTES of a windows 16bit string

  int32 strchars8(cvoid *);         /// UTF-8 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32 strcombs8(cvoid *);         /// UTF-8 str: counts nr of combining diacriticals
  int32 strunicodes8(cvoid *);      /// UTF-8 str: counts nr of unicode values (both chars and combs)
  int32 strchars32(cuint32 *);      /// UTF-32 str: counts nr of chars (combining diacriticals are considered part of a character - basically not counted)
  int32 strcombs32(cuint32 *);      /// UTF-32 str: counts nr of combining diacriticals
  int32 strunicodes32(cuint32 *);   /// UTF-32 str: counts nr of unicode values (both chars and combs)

  void strcpy8(void *dst, cvoid *src);                   /// UTF-8 str: copies src to dst; dst must be prealocated
  void strncpy8(void *dst, cvoid *src, int32);           /// UTF-8 str: copies n CHARACTERS from src to dst; if combining diacriticals are found, they are treated as part of a full character and copied with that character
  void strncpy8_unicodes(void *dst, cvoid *src, int32);  /// UTF-8 str: copies n unicode values from src to dst; dst must be prealocated; does not care if a unicode value is a character or diacritical
  void strcpy32(uint32 *dst, cuint32 *src);                  /// UTF-32 str: copies src to dst; dst must be prealocated
  void strncpy32(uint32 *dst, cuint32 *src, int32);          /// UTF-32 str: copies n CHARS from src to dst; if combining diacriticals are found, they are treated as part of a full character and copied with that character
  void strncpy32_unicodes(uint32 *dst, cuint32 *src, int32); /// UTF-32 str: copies n unicode values from src to dst; dst must be prealocated; does not care if a unicode value is a character or diacritical

  uint32 utf8to32(cvoid *);         /// returns unicode value as UTF-32 (or direct unicode number)
  uint32 utf8to32n(cvoid *, int);   /// returns n-th unicode value as UTF-32 (or direct unicode number)

  int32 strcmp8(cvoid *, cvoid *);       /// UTF-8 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa
  int32 strcmp32(cuint32 *, cuint32 *);  /// UTF-32 str: compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa

  uint32 tolower(uint32);           /// converts a single unicode char to lowercase (special cases that convert to multiple characters are NOT HANDLED)
  uint32 toupper(uint32);           /// converts a single unicode char to uppercase (special cases that convert to multiple characters are NOT HANDLED)
  
  void *getChar8(cvoid *s, int32 n);         /// UTF-8 str: returns [n]-th char in string (diacriticals are considered part of a character)
  void *getUnicode8(cvoid *s, int32 n);      /// UTF-8 str: returns the [n] unicode value in string (does not care if it's a normal character or diacritical)
  uint32 *getChar32(cuint32 *s, int32 n);    /// UTF-32 str: returns [n]-th char in string (diacriticals are considered part of a character)
  inline uint32 getUnicode32(cuint32 *s, int32 n) { return s[n]; } /// UTF-32 str: returns the [n] unicode value in string (does not care if it's a normal character or diacritical)

  bool isComb(uint32 n);           /// checks specified unicode value to see if this is a combining diacritical (if u dont know what they are, you can use clearComb() to remove them all from a string, and avoid headaches)



  inline void memcpy(void *dst, cvoid *src, uint32 n) { int8 *p= (int8 *)dst, *t= (int8 *)src; for(uint32 a= 0; a< n; a++) *p++= *t++; }
  inline void memclr(void *dst, uint32 n, uint8 v= 0) { uint8 *p= (uint8 *)dst; for(uint32 a= 0; a< n; a++) *p++= v; }

  // other versions: 

  //inline void memcpy(void *dst, cvoid *src, uint32 n) { for(uint32 a= 0; a< n; a++) ((int8 *)dst)[a]= ((int8 *)src)[a]; }
  //inline void memclr(void *dst, uint32 n, uint8 v= 0) { for(uint32 a= 0; a< n; a++) ((int8 *)dst)[a]= v;

  //inline void memcpy(void *dst, cvoid *src, uint64 n) { for(uint64 a= 0; a< n; a++) ((int8 *)dst)[a]= ((int8 *)src)[a]; }
  //inline void memcpy(void *dst, cvoid *src, uint64 n) { int8 *p1= (int8 *)dst, *p2= (int8 *)src; for(uint64 a= 0; a< n; a++, p1++, p2++) *p1= *p2; }
}








