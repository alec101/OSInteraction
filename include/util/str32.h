#pragma once
#define STR32CLASSINCLUDED 1

// WARNING:
// -if you manually mess with d/d8/d16 (internal data), use updateLen(), to update
//  the <len> var, as it is used internally string assumes <len> variable has the
//  correct number set, so it doesn't recalculate it @ every string operation
//  (SPEED is favoured)

// -this class is not using any c/c++ string function. At this moment with the windows/linux war...
//  setting locale& other stuff is just not possible as everything differs.

// -fopen knows of utf-8 but the win version wants to put a BOM in the file,
//  wich cause problems in linux, so a file should be opened as pure binary (at
//  least when writing). This should avoid fseek problems too (fseek is used internally)


// NOTES:
// -character              - char - usually what every program knows about a character
// -combining diacritical  - comb - each character can be created using multiple combining diacriticals.
//                                 string handles them; usually is good not to use them unless building
//                                 a sofisticated editor / whatever sofisticated character handling program
//                                 * use clearComb() to remove all of them from a string
//                                 * use isComb() to know if a character is a combining diacritical
// -bad characters are marked with 0xFFFD when using secure read funcs

// -the difference between char on windows (default signed) and char on macs (default unsigned) will not affect the end result of strings
// -if char or char16_t or char32_t are bigger than 8, 16 and 32bits respectively, it will not matter,
//  the functions are using (u)int8_t, (u)int16_t and (u)int32_t internally, which are guaranteed to be 8, 16 and 32bits



// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

/// TODO:
/// * think about memory alloc... every change is a delete+alloc

#include "strCommon.h"

#ifndef STR16INCLUDED
class str16;
#endif
#ifndef STR8INCLUDED
class str8;
#endif

class str32 {
public:
  // WIP vvv
  str32 &f(const char *text, ...);
  str32 &addInt(int64_t);
  str32 &addUInt(uint64_t);
  str32 &addFloat(float);
  str32 &addDouble(double);
  // WIP ^^^

  char32_t *d;                  // main internal data storage 32bits  (if u mess with it, call updateLen() !!! )
  char *d8;                     // used for UTF32 to UTF8 conversion - convert8(), so you don't need to manually dealloc mem for it
  char16_t *d16;                // used for UTF32 to UTF16 conversion - convert16(), so you don't need to manually dealloc mem for it
  int32_t len;                  // string length in bytes, including the str terminator
  int32_t nrUnicodes;           // number of unicode values in string

  // wrapping on a custom buffer - changes the way the class works (it should be faster, no memory allocs)

  bool wrapping;                // this flag signals if wrapping on a specific buffer
  int32_t wrapSize;             // wrapped buffer size, in uint32_t units

  void wrap(char32_t *in_buffer, int32_t in_size= 0); // CHANGES THE WAY THIS CLASS WORKS: uses specified buffer for every operation, no memory allocs are made. This buffer must be big enough for every operation made, but critical errors will not occur if it's not big enough (the text will not be OK). wrapSize is in int32 units. if buffer size is 0, THE SIZE IS DETERMINED BY THE STRING SIZE INSIDE THE BUFFER.
  void stopWrapping();                                // returns the way the class works to the normal functionality - d will be memory allocated, the custom buffer is released

  // main functions

  void updateLen();           // updates string length vars, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]
  void lower();               // converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();               // converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  int32_t nrChars() const;    // returns the number of characters in str, WHITOUT the number of combining diacriticals

  char *convert8(int32_t *out_len= NULL);       // UTF-32 to UTF-8 conversion. returned pointer is part of the class (no need for dealloc). use getUTF8 for an 'unbound' pointer
  char16_t *convert16(int32_t *out_len= NULL);  // UTF-32 to UTF-16 conversion. returned pointer is part of the class (no need for dealloc). use getUTF16 for an 'unbound' pointer
  char *getUTF8(int32_t *out_len= NULL) const;        // similar to convert8, but the returned pointer is not part of the class and must be manually deallocated
  char16_t *getUTF16(int32_t *out_len= NULL) const;   // similar to convert16, but the returned pointer is not part of the class and must be manually deallocated
  void convert8static(char *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const;      // returns string as UTF-8 in out_buf; please specify buffer size in <in_bufSize> (in bytes); out_nrUnicodes, out_len(in bytes includes str terminator) - optional return values
  void convert16static(char16_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const; // returns string as UTF-16 in out_buf; please specify buffer size in <in_bufSize> (in int16 units); out_nrUnicodes, out_len(in bytes includes str terminator) - optional return values
  
  // These are SECURE functions. Read & validate each character

  str32 &secureUTF32(const char32_t *str, int32_t in_len= 0);  // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF32() to validate a UTF-32 string;  <in_len>: the length in bytes of the string. If left 0, the string is null terminated and the function will use this
  str32 &secureUTF16(const char16_t *str, int32_t in_len= 0);  // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF16() to validate a UTF-16 string;  <in_len>: the length in bytes of the string. If left 0, the string is null terminated and the function will use this
  str32 &secureUTF8(const char *str,      int32_t in_len= 0);  // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a UTF-8 string;    <in_len>: the length in bytes of the string. If left 0, the string is null terminated and the function will use this
  
  // combining diacritical characters

  void clearCombs();                 // clears all combining diacritical characters from the string
  int32_t nrCombs() const;           // returns the number of combining diacriticals (if any) in string

  // character or string insertion / deletion from a string

  void insert(char32_t in_unicode,       int32_t in_pos= -1); // <in_unicode>- unicode to insert; <in_pos>- insert position (-1= insert at the end of str)
  void insertStr(const char32_t *in_str, int32_t n= -1, int32_t in_pos= -1); // <in_str>- str to insert; <in_pos>- insert position (-1= at the end of the str)
  void del(int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1); // <in_nrToDel>- number of unicode values to delete; <in_pos>- delete position - (del at the left, -1= end of string);  <returns>- resulting string length in bytes (without terminator)

  // search in string funcs
  
  // <in_search>- text to search; <in_caseSensitive>- self explanatory; <return> NULL if not found; IF FOUND, points to the begining of found text location in the <in_str>
  inline char32_t *search(const char32_t *in_str, const char32_t *in_search, bool in_caseSensitive= true) {
    return Str::search32(d, in_search, in_caseSensitive); }

  // number to/from string

  inline int64_t toInt()   const { return Str::utf32toInt64(d); }
  inline uint64_t toUint() const { return Str::utf32toUint64(d); }
  inline float toFloat()   const { return Str::utf32toFloat(d); }
  inline double toDouble() const { return Str::utf32toDouble(d); }

  str32 &fromInt(int64_t n, int8_t base= 10, bool uppercase= true)   { if(!wrapping) if(len< 268) { if(d) delete[] d; d= (char32_t *)new uint32_t[67]; } nrUnicodes= Str::int64toUtf32(n, d, base, uppercase);  len= (nrUnicodes+ 1)* 4; return *this; }
  str32 &fromUint(uint64_t n, int8_t base= 10, bool uppercase= true) { if(!wrapping) if(len< 268) { if(d) delete[] d; d= (char32_t *)new uint32_t[67]; } nrUnicodes= Str::uint64toUtf32(n, d, base, uppercase); len= (nrUnicodes+ 1)* 4; return *this; }
  str32 &fromFloat(float n, int precision= 2, bool useE= false)      { if(!wrapping) if(len< 156) { if(d) delete[] d; d= (char32_t *)new uint32_t[39]; } nrUnicodes= Str::floatToUtf32(n, d, precision, useE);  len= (nrUnicodes+ 1)* 4; return *this; }
  str32 &fromDouble(double n, int precision= 2, bool useE= false)    { if(!wrapping) if(len< 156) { if(d) delete[] d; d= (char32_t *)new uint32_t[39]; } nrUnicodes= Str::doubleToUtf32(n, d, precision, useE); len= (nrUnicodes+ 1)* 4; return *this; }




  // constructors

  str32(): d(NULL), d8(NULL), d16(NULL), len(0), nrUnicodes(0), wrapping(false), wrapSize(0) {} // main constructor
  str32(const str32 &s):    str32() { *this= s; } // from another str32
  str32(const str16 &s);
  str32(const str8 &s);
  str32(const char32_t *s, bool in_wrap= false): str32() { if(in_wrap) wrap((char32_t *)s); else *this= s; } // from UTF-32 string
  str32(const char16_t *s): str32() { *this= s; } // from UTF-16 string
  str32(const char *s):     str32() { *this= s; } // from UTF-8 string
  str32(char32_t c):        str32() { *this= c; } // makes 1 char length string + terminator

  // destructor / cleaners

  ~str32();
  void delData();                         // can be used to clean the string - this is used by destructor too
  void clean();                           // unallocs d16 and d8 (buffers used for conversion to UTF-8/16 only

  //operators

  str32 &operator= (const str32 &);       // assign from another str32 class
  str32 &operator= (const str16 &);       // assign from another str16 class
  str32 &operator= (const str8 &);        // assign from another str8 class
  str32 &operator= (const char32_t *);    // assign from UTF-32 array string
  str32 &operator= (const char16_t *);    // assign from UTF-16 array string
  str32 &operator= (const char *s);       // assign from UTF-8 array string
  str32 &operator= (char32_t);            // makes a 1 legth string of input char (+terminator)

  str32 operator+(const str32 &s) const;    // adds two str32 then returns the result
  str32 operator+(const str16 &s) const;    // adds two str then returns the result
  str32 operator+(const str8 &s) const;     // adds two str then returns the result
  str32 operator+(const char32_t *s) const; // adds str32 and UTF-32 str then returns the result
  str32 operator+(const char16_t *s) const; // adds str32 and UTF-16 str then returns the result
  str32 operator+(const char *s) const;     // adds str32 and UTF-8  str then returns the result
  str32 operator+(char32_t c) const;        // adds str32 and a single unicode char then returns the result

  str32 &operator+=(const str32 &);     // adds another str32 to current str32
  str32 &operator+=(const str16 &);     // adds another str16 to current str32
  str32 &operator+=(const str8 &);      // adds another str8 to current str32
  str32 &operator+=(const char32_t *);  // adds UTF-32 str to current str32
  str32 &operator+=(const char16_t *);  // adds UTF-16 str to current str32
  str32 &operator+=(const char *);      // adds UTF-8  str to current str32
  str32 &operator+=(char32_t);          // adds adds a single unicode character to current str32 class
  
  str32 &operator-=(int n);             // removes n unicode values from the back of the string
  str32 operator-(int n) const;         // returns a temp str32 that has n less unicode values

  inline operator char32_t *() const { return d; }           // returns pointer to internal data (d)
  inline operator char16_t *()       { return convert16(); } // converts to UTF-16. returned pointer is part of the class, no need for dealloc
  inline operator char *()           { return convert8(); }  // converts to UTF-8. returned pointer is part of the class, no need for dealloc

  inline explicit operator bool() const { return (bool)nrUnicodes; /*? true: false);*/ } // returns true if string has any characters
  inline bool operator!()         const { return (bool)nrUnicodes; /*? true: false;*/ };

  inline char32_t *operator[](unsigned int n) const   { return (char32_t *)((uint32_t *)d+ n); } // points to the [n] unicode value in string
  inline char32_t *pointUnicode(unsigned int n) const { return (char32_t *)((uint32_t *)d+ n); } // points to the [n] unicode value in string
  inline char32_t getUnicode(unsigned int n) const    { return ((uint32_t *)d)[n]; }             // returns the [n] unicode value in string
  //char32_t *getChar(unsigned int n) const    { return Str::getChar32(d, n); } // returns n-th character in string (diacriticals are part of a character)
  
  bool operator==(const str32 &s) const;   // checks if strings are identical
  bool operator==(const str16 &s) const;   // checks if strings are identical
  bool operator==(const str8 &s) const;    // checks if strings are identical
  bool operator==(const char32_t *) const; // checks if strings are identical (utf-32)
  bool operator==(const char16_t *) const; // checks if strings are identical (UTF-16)
  bool operator==(const char *s) const;    // checks if strings are identical (utf-8)
  bool operator==(char32_t) const;         // string must be 1 char long (+terminator) and identical to passed unicode character

  inline bool operator!=(const str32 &s) const    { return !(operator==(s)); }
  inline bool operator!=(const str16 &s) const    { return !(operator==(s)); }
  inline bool operator!=(const str8 &s) const     { return !(operator==(s)); }
  inline bool operator!=(const char32_t *s) const { return !(operator==(s)); }
  inline bool operator!=(const char16_t *s) const { return !(operator==(s)); }
  inline bool operator!=(const char *s) const     { return !(operator==(s)); }
  inline bool operator!=(char32_t c) const        { return !(operator==(c)); }
};


//these should have inline, and declared in the cpp, or whatever; basically CHECK IF THESE COMPILE (i remember there were problems)
inline str32 operator+(const char *s1,     const str32 &s2) { return str32(s1)+= s2; }
inline str32 operator+(const char16_t *s1, const str32 &s2) { return str32(s1)+= s2; }
inline str32 operator+(const char32_t *s1, const str32 &s2) { return str32(s1)+= s2; }














