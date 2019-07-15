#pragma once
#define STR8CLASSINCLUDED 1


// WARNING:
// -if you manually mess with d (internal data), use updateLen(), to update
//    the <len/nrUnicodes> vars, as they are used internally; Str assumes
//    <len/nrUnicodes> variables have the correct values set, so it doesn't
//    recalculate them @ every operation (SPEED is favoured)

// NOTES:
// -character              - char - usually what every program knows about a character
// -combining diacritical  - comb - each character can be created using multiple combining diacriticals.
//                                  string handles them; usually is good not to use them unless building a sofisticated editor / whatever sofisticated character handling program
//                                  use clearComb() to remove all of them from a string
//                                  use isComb() to know if a specific unicode value is a combining diacritical
// -bad characters are marked with 0xFFFD when using secure read funcs

// -the difference between char on windows (default signed) and char on macs (default unsigned) will not affect the end result of strings
// -if char or char16_t or char32_t are bigger than 8, 16 and 32bits respectively, it will not matter,
//  the functions are using (u)int8_t, (u)int16_t and (u)int32_t internally, which are guaranteed to be 8, 16 and 32bits


// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

// TODO:
/*
 * [HIGH PRIO] a nice formatting? fast & clean... current format produces tons of warnings on diff os-es (use 8/16/32/64 bit for an int)
               not only this, but the current format would use locales, right? not only this, but this would eliminate the need to include stdio.h
 * check asembler book for processor string instructions, maybe do the whole thing in asembler (probly it is best for the str8 class)
*/

#include "strCommon.h"

#ifndef STR16INCLUDED
class str16;
#endif
#ifndef STR32INCLUDED
class str32;
#endif

class str8 {
public:
  char *d;                    // main internal data storage UTF-8 format (if u mess with it, call updateLen() !!! )
  char16_t *d16;              // used for UTF8 to UTF16 conversion - convert16(), so you don't need to manually dealloc mem for it
  char32_t *d32;              // used for UTF8 to UTF32 conversion - convert32(), so you don't need to manually dealloc mem for it
  int32_t len;                // size of string in bytes, including the str terminator
  int32_t nrUnicodes;         // nr of unicode values in string

  // wrapping on a custom buffer - changes the way the class works (it should be faster, no memory allocs)

  bool wrapping;                // this flag signals if wrapping on a specific buffer
  int32_t wrapSize;             // wrapped buffer size, in 8bit units

  void wrap(const char *, int32_t in_size= 0);  // CHANGES THE WAY THIS CLASS WORKS: uses specified buffer for every operation, no memory allocs are made. This buffer must be big enough for every operation made, but critical errors will not occur if it's not big enough (the text will not be OK). wrapSize is in int8 units. if buffer size is 0, THE SIZE IS DETERMINED BY THE STRING SIZE INSIDE THE BUFFER
  void stopWrapping();                          // returns the way the class works to the normal functionality - d will be memory allocated, the custom buffer is released

  // main functions
  
  str8 &f(const char *, ...); // format string using sprintf(...) MAX LENGTH IS 1024, so dont re-write books with this one; use it on a line by line basis
  void updateLen();           // updates len and nrUnicodes vars, in case you want to mess with the string internal data (d). [WARNING: see start of this header file]
  void lower();               // converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();               // converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  int32_t nrChars() const;    // returns the number of characters, excluding combining diacriticals

  // UTF-8 to UTF-32/UTF-16

  char16_t *convert16(int32_t *out_len= NULL);  // returns string as UTF-16; returned pointer is part of the class (no need for dealloc). use getUTF16 for an 'unbound' pointer
  char32_t *convert32(int32_t *out_len= NULL);  // returns string as UTF-32; returned pointer is part of the class (no need for dealloc). use getUTF32 for an 'unbound' pointer
  char16_t *getUTF16(int32_t *out_len= NULL) const; // similar to convert16, but the returned pointer is not part of the class and must be manually deallocated
  char32_t *getUTF32(int32_t *out_len= NULL) const; // similar to convert32, but the returned pointer is not part of the class and must be manually deallocated
  void convert16static(char16_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const;  // returns string as UTF-16; <out_buf>- the buffer that will be filled with the converted string; <out_bufSize> - [MUST SPECIFY] the size of <out_buf> in uint16 units (not bytes); <out_nrUnicodes> & <out_len> optional return values
  void convert32static(char32_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const;  // returns string as UTF-32; <out_buf>- the buffer that will be filled with the converted string; <out_bufSize> - [MUST SPECIFY] the size of <out_buf> in uint32 units (not bytes); <out_nrUnicodes> & <out_len> optional return values

  // character / string insertion deletion at specific points

  void insert(char32_t in_unicode, int32_t in_pos= -1);       // <in_unicode>- unicode to insert; <in_pos>- insert position (-1= insert at the end of str)
  void insertStr(const char *in_str, int32_t n= -1, int32_t in_pos= -1);     // <in_str>- str(UTF-8) to insert; <in_pos>- insert position (-1= at the end of the str)
  void del(int32_t in_nrToDel= 1, int32_t in_pos= -1);        // <in_nrToDel>- number of unicode values to delete; <in_pos>- delete position - (del at the left, -1= end of string);

  // search in string funcs

  char *search(const char *in_search, bool in_caseSensitive= true); // <in_search>- text to search; <in_caseSensitive>- self explanatory; <return> NULL if not found; IF FOUND, points to the begining of found text location in the <in_str>

  // UTF8 functions. These are SECURE functions. Read & validate each character - they unpack the whole string& pack it back up after validating each char

  str8 &secureUTF8(const char *,      int32_t in_len= 0); // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a UTF-8 string  ; <in_len>: the length in bytes of the string. If left 0, the string is null terminated and the function will use this
  str8 &secureUTF16(const char16_t *, int32_t in_len= 0); // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF16() to validate a UTF-16 string; <in_len>: the length in bytes of the string. If left 0, the string is null terminated and the function will use this
  str8 &secureUTF32(const char32_t *, int32_t in_len= 0); // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF32() to validate a UTF-32 string; <in_len>: the length in bytes of the string. If left 0, the string is null terminated and the function will use this
  
  // combining diacritical characters
  
  void clearCombs();                 // clears all combining diacritical characters from the string
  int32_t nrCombs() const;           // returns the number of combining diacriticals (if any) in string

  // string to/from number

  int64_t toInt()   const { return Str::utf8toInt64(d); }
  uint64_t toUint() const { return Str::utf8toUint64(d); }
  float toFloat()   const { return Str::utf8toFloat(d); }
  double toDouble() const { return Str::utf8toDouble(d); }

  str8 &fromInt(int64_t n, int8_t base= 10, bool uppercase= true)   { if(!wrapping) if(len< 67) { if(d) delete[] d; d= (char *)new uint8_t[67]; } nrUnicodes= Str::int64toUtf8(n, d, base, uppercase);  len= nrUnicodes+ 1; return *this; }
  str8 &fromUint(uint64_t n, int8_t base= 10, bool uppercase= true) { if(!wrapping) if(len< 67) { if(d) delete[] d; d= (char *)new uint8_t[67]; } nrUnicodes= Str::uint64toUtf8(n, d, base, uppercase); len= nrUnicodes+ 1; return *this; }
  str8 &fromFloat(float n, int precision= 2, bool useE= false)      { if(!wrapping) if(len< 39) { if(d) delete[] d; d= (char *)new uint8_t[39]; } nrUnicodes= Str::floatToUtf8(n, d, precision, useE);  len= nrUnicodes+ 1; return *this; }
  str8 &fromDouble(double n, int precision= 2, bool useE= false)    { if(!wrapping) if(len< 39) { if(d) delete[] d; d= (char *)new uint8_t[39]; } nrUnicodes= Str::doubleToUtf8(n, d, precision, useE); len= nrUnicodes+ 1; return *this; }

  

  // constructors
  
  str8(): d(NULL), d16(NULL), d32(NULL), len(0), nrUnicodes(0), wrapping(false), wrapSize(0) {} // main constructor
  str8(const str8 &s):     str8() { *this= s; } // main constructor
  str8(const str16 &);
  str8(const str32 &);
  str8(const char *s, bool in_wrap= false): str8() { if(in_wrap) wrap(s); else *this= s; } // asumes the char string is UTF-8 encoded (char can vary from os to os); if <in_wrap> is set to true, the string will be wrapped (see wrap() func)
  str8(const char16_t *s): str8() { *this= s; } // converts from a UTF-16 string
  str8(const char32_t *s): str8() { *this= s; } // converts from utf-32
  str8(const char32_t c):  str8() { *this= c; } // makes 1 char length string + terminator

  // destructor / cleaners
  
  ~str8() { delData(); clean(); }
  void delData();                     // called by destructor, can be used to clean the string
  void clean();                       // unallocs d16 and d32

  //operators
  
  str8 &operator= (const str8 &);     // assign from another str8 class
  str8 &operator= (const str16 &s);   // assign from a str16 class
  str8 &operator= (const str32 &);    // assign from a str32 class
  str8 &operator= (const char *);     // assign from a UTF-8 str
  str8 &operator= (const char16_t *); // assign from a UTF-16 str
  str8 &operator= (const char32_t *); // assign from a UTF-32 str
  str8 &operator= (const char32_t);   // makes a 1 char length string of specified unicode value (+terminator)
  
  str8 operator+(const str8 &s) const;     // adds two str8 then returns the result
  str8 operator+(const str16 &s) const;    // adds two str then returns the result
  str8 operator+(const str32 &s) const;    // adds two str then returns the result
  str8 operator+(const char *s) const;     // adds str8 and UTF-8 arr str then returns the result
  str8 operator+(const char16_t *s) const; // adds str8 and UTF-16 arr str then returns the result
  str8 operator+(const char32_t *s) const; // adds str8 and UTF-32 arr str then returns the result
  str8 operator+(char32_t c) const;        // adds str8 and a single unicode char then returns the result

  str8 &operator+=(const str8 &);         // adds another string to current
  str8 &operator+=(const str16 &);
  str8 &operator+=(const str32 &);
  str8 &operator+=(const char *);         // adds an UTF-8 array str
  str8 &operator+=(const char16_t *);     // adds am UTF-16 array str
  str8 &operator+=(const char32_t *);     // adds an UTF-32 array str
  str8 &operator+=(const char32_t);       // adds a (unicode) character

  str8 &operator-=(const int n);          // clears n char(s) from string
  str8 operator-(int n) const;            // returns a temp string that has n less chars

  //inline operator void*() const { return d; }
  //inline operator const void*() const { return d; }
  inline operator char*() const { return d; }     // returns the string as UTF-8
  //operator const char*() { return (const char *)d; }   // returns the string as UTF-8
  inline operator char16_t *() { return convert16(); }   // returns the string as UTF-16; returned pointer is part of the class, no need for dealloc
  inline operator char32_t *() { return convert32(); }   // returns the string as UTF-32; returned pointer is part of the class, no need for dealloc
  inline explicit operator const bool() const { return (bool)nrUnicodes; /*? true: false;*/ }
  inline bool operator!() const { return (bool)nrUnicodes;/*? false: true;*/ }
  inline const bool exists() const { return (bool)nrUnicodes; }

  inline char *operator[](unsigned n) const   { return Str::getUnicode8(d, n); } // points to the n-th unicode value in the string
  inline char *pointUnicode(unsigned n) const { return Str::getUnicode8(d, n); } // points to the n-th unicode value in the string
  inline char32_t getUnicode(unsigned n) const   { return Str::utf8to32(Str::getUnicode8(d, n)); } // returns n-th unicode value in the string
  
  bool operator==(const str8 &s) const;      // checks if strings are identical
  bool operator==(const str16 &s) const;
  bool operator==(const str32 &s) const;
  bool operator==(const char *s) const;      // checks if strings are identical (UTF-8)
  bool operator==(const char16_t *) const;   // checks if strings are identical (UTF-16)
  bool operator==(const char32_t *) const;   // checks if strings are identical (UTF-32)
  bool operator==(const char32_t) const;     // string must be 1 char long (+terminator) and identical to input unicode value
  
  bool operator!=(const str8 &s) const      { return !(operator==(s)); }
  bool operator!=(const str16 &s) const     { return !(operator==(s)); }
  bool operator!=(const str32 &s) const     { return !(operator==(s)); }
  bool operator!=(const char *s) const      { return !(operator==(s)); }
  bool operator!=(const char16_t *s) const  { return !(operator==(s)); }
  bool operator!=(const char32_t *s) const  { return !(operator==(s)); }
  bool operator!=(const char32_t c) const   { return !(operator==(c)); }
  bool operator!=(char *s) const { return !(operator==((const char *)s)); }
    

#ifdef OS_WIN // osinteraction for LPCSTR convertion
  inline operator WCHAR*() { return (WCHAR *)convert16(); }
  inline str8 &operator= (const WCHAR *s) { return operator= ((char16_t *)s); }
#endif /// OS_WIN
  private:
    bool _changed;
};

inline str8 operator+(const char *s1,     const str8 &s2) { return str8(s1)+= s2; }
inline str8 operator+(const char16_t *s1, const str8 &s2) { return str8(s1)+= s2; }
inline str8 operator+(const char32_t *s1, const str8 &s2) { return str8(s1)+= s2; }





