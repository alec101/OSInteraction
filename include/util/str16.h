#pragma once
#define STR16CLASSINCLUDED 1

// WARNING:
// -if you manually mess with d/d8/d32 (internal data), use updateLen(), to update
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
//                                  string handles them; usually is good not to use them unless building
//                                  a sofisticated editor / whatever sofisticated character handling program
//                                  * use clearComb() to remove all of them from a string
//                                  * use isComb() to know if a character is a combining diacritical

// -bad characters are marked with 0xFFFD when using secure read funcs

// -the difference between char on windows (default signed) and char on macs (default unsigned) will not affect the end result of strings
// -if char or char16_t or char32_t are bigger than 8, 16 and 32bits respectively, it will not matter,
//  the functions are using (u)int8_t, (u)int16_t and (u)int32_t internally, which are guaranteed to be 8, 16 and 32bits



// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

/// TODO:
/// * think about memory alloc... every change is a delete+alloc

#include "strCommon.h"


#ifndef STR8CLASSINCLUDED
class str8;
#endif
#ifndef STR32CLASSINCLUDED
class str32;
#endif

class str16 {
public:
  char16_t *d;                  // main internal data storage 16bits  (if u mess with it, call updateLen() !!! )
  char *d8;                     // secondary utf-8 data storage  (if u mess with it, call updateLen() !!! )
  char32_t *d32;                // secondary utf-32 data storage (if u mess with it, call updateLen() !!! )
  int32_t len;                  // string length in bytes, including the str terminator
  int32_t nrUnicodes;           // number of unicode values in string

  // wrapping on a custom buffer - changes the way the class works (it should be faster, no memory allocs)

  bool wrapping;                // this flag signals if wrapping on a specific buffer
  int32_t wrapSize;             // wrapped buffer size, in uint16_t units

  void wrap(char16_t *in_buffer, int32_t in_size= 0);    // CHANGES THE WAY THIS CLASS WORKS: uses specified buffer for every operation, no memory allocs are made. This buffer must be big enough for every operation made, but critical errors will not occur if it's not big enough (the text will not be OK). wrapSize is in int16 units. if buffer size is 0, THE SIZE IS DETERMINED BY THE STRING SIZE INSIDE THE BUFFER.
  void stopWrapping();                              // returns the way the class works to the normal functionality - d will be memory allocated, the custom buffer is released

  // main functions

  void updateLen();           // updates string length vars, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]
  int32_t nrChars() const;    // returns the number of characters in string, WHITOUT the number of combining diacriticals
  void lower();               // converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();               // converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)

  char *convert8(int32_t *out_len= NULL);        // UTF-16 to UTF-8 conversion; returned buffer is part of class and doesn't need dealloc
  char32_t *convert32(int32_t *out_len= NULL);      // UTF-16 to UTF-32 conversion; returned buffer is part of class and doesn't need dealloc
  char *getUTF8(int32_t *out_len= NULL) const;   // similar to convert8() but returned string is released, and not part of the class 
  char32_t *getUTF32(int32_t *out_len= NULL) const; // similar to convert32() but returned string is released, and not part of the class 
  void convert8static(char *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes= NULL, int32_t *out_len= NULL) const;   // converts to UTF8 on specified static buffer
  void convert32static(char32_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes= NULL, int32_t *out_len= NULL) const; // converts to UTF32 on specified static buffer

  // These are SECURE functions. Read & validate each character

  str16 &secureUTF8(const char *, int32_t in_len= 0);       // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf-8 string
  str16 &secureUTF16(const char16_t *, int32_t in_len= 0);  // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF16() to validate a utf-16 string
  str16 &secureUTF32(const char32_t *, int32_t in_len= 0);  // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF32() to validate a utf-32 string

  // these would require stdio.h
  //void readUTF8(FILE *);             // read all (remaining) file       (SECURE)
  //void readUTF8n(FILE *, size_t);    // read n characters from file     (SECURE)
  //void readLineUTF8(FILE *);         // read till end of line (or file) (SECURE)

  // combining diacritical characters

  void clearCombs();                  // clears all combining diacritical characters from the string
  int32_t nrCombs() const;            // counts the number of combining diacriticals in the string

  // character or string insertion / deletion from a string

  void insert(char32_t in_unicode,       int32_t in_pos= -1);  // <in_unicode>- unicode to insert; <in_pos>- insert position (-1= insert at the end of str)
  void insertStr(const char16_t *in_str, int32_t in_pos= -1);  // <in_str>- str(UTF-16) to insert; <in_pos>- insert position (-1= at the end of the str)
  void del(int32_t in_nUnicodesToDel= 1, int32_t in_pos= -1);  // <in_nrToDel>- number of unicode values to delete; <in_pos>- delete position - (del at the left, -1= end of string);

  // search in string funcs

  char16_t *search(const char16_t *in_str, const char16_t *in_search, bool in_caseSensitive= true) const;

  // number to/from string

  inline int64_t toInt()   const { return Str::utf16toInt64(d); }
  inline uint64_t toUint() const { return Str::utf16toUint64(d); }
  inline float toFloat()   const { return Str::utf16toFloat(d); }
  inline double toDouble() const { return Str::utf16toDouble(d); }
  
  str16 &fromInt(int64_t n, int8_t base= 10, bool uppercase= true)   { if(!wrapping) if(len< 132) { if(d) delete[] d; d= (char16_t *)new uint16_t[66]; } nrUnicodes= Str::int64toUtf16(n, d, base, uppercase);  len= (nrUnicodes+ 1)* 2; return *this; }
  str16 &fromUint(uint64_t n, int8_t base= 10, bool uppercase= true) { if(!wrapping) if(len< 132) { if(d) delete[] d; d= (char16_t *)new uint16_t[66]; } nrUnicodes= Str::uint64toUtf16(n, d, base, uppercase); len= (nrUnicodes+ 1)* 2; return *this; }
  str16 &fromFloat(float n, int precision= 2, bool useE= false)      { if(!wrapping) if(len< 78)  { if(d) delete[] d; d= (char16_t *)new uint16_t[39]; } nrUnicodes= Str::floatToUtf16(n, d, precision, useE);  len= (nrUnicodes+ 1)* 2; return *this; }
  str16 &fromDouble(double n, int precision= 2, bool useE= false)    { if(!wrapping) if(len< 78)  { if(d) delete[] d; d= (char16_t *)new uint16_t[39]; } nrUnicodes= Str::doubleToUtf16(n, d, precision, useE); len= (nrUnicodes+ 1)* 2; return *this; }



  // constructors

  str16(): d(NULL), d8(NULL), d32(NULL), len(0), nrUnicodes(0), wrapping(false), wrapSize(0) {} // main constructor
  str16(const str16 &s):    str16() { *this= s; } // from another str16
  str16(const str32 &);
  str16(const str8 &);
  str16(const char16_t *s, bool in_wrap= false): str16() { if(in_wrap) wrap((char16_t *)s); else *this= s; } // from UTF-16 string
  str16(const char32_t *s): str16() { *this= s; } // from UTF-32 string
  str16(const char *s):     str16() { *this= s; } // from UTF-8 string
  str16(uint32_t c):        str16() { *this= c; } // makes 1 char length string + terminator

  // destructor / cleaners

  ~str16();
  void delData();                         // can be used to clean the string - this is used by destructor too
  void clean();                           // clears everything but internal data [d] variable (other buffers: d8/ dWin)

  //operators

  str16 &operator= (const str16 &);       // assign from another str16 class
  str16 &operator= (const str32 &);
  str16 &operator= (const str8 &);
  str16 &operator= (const char16_t *);    // assign from UTF-16 array string
  str16 &operator= (const char32_t *);    // assign from UTF-32 array string
  str16 &operator= (const char *);        // assign from UTF-8 array string
  str16 &operator= (char32_t);            // makes a 1 legth string of input unicode val (+terminator)
  
  str16 operator+(const str16 &s) const;    // adds two str16 then returns the result
  str16 operator+(const str32 &s) const;    // adds two str then returns the result
  str16 operator+(const str8 &s) const;     // adds two str then returns the result
  str16 operator+(const char16_t *s) const; // adds str16 and UTF-16 arr str then returns the result
  str16 operator+(const char32_t *s) const; // adds str16 and UTF-32 arr str then returns the result
  str16 operator+(const char *s) const;     // adds str16 and UTF-8 arr str then returns the result
  str16 operator+(char32_t c) const;        // adds str16 and a single unicode char then returns the result

  str16 &operator+=(const str16 &);     // adds another string32 to current string32 class
  str16 &operator+=(const str32 &);     // adds another string32 to current string32 class
  str16 &operator+=(const str8 &);      // adds another string32 to current string32 class
  str16 &operator+=(const char16_t *);  // adds UTF-16 arr str to current str16 class
  str16 &operator+=(const char32_t *);  // adds UTF-32 arr str to current str16 class
  str16 &operator+=(const char *);      // adds UTF-8 arr str to current str16 class
  str16 &operator+=(char32_t);          // adds adds a single unicode character to current str16 class

  str16 &operator-=(int n);             // removes n unicode values from the back of the string
  str16 operator-(int n) const;         // returns a temp str32 that has n less unicode values

  inline operator char16_t *() { return d; }           // returns pointer to internal data (d)   UTF-16 format
  //operator char32_t *() { return convert32(); } // returns pointer to internal data (d32) UTF-32 format
  //operator char *()     { return convert8(); }  // returns pointer to internal data (d8)  UTF-8 format

  inline operator bool() const     { return (nrUnicodes? true: false); } // returns true if string has any characters

  inline char16_t *operator[](unsigned int n) const   { return Str::getUnicode16(d, n); }  // points to the [n] unicode value in the string
  inline char16_t *pointUnicode(unsigned int n) const { return Str::getUnicode16(d, n); }  // points to the [n] unicode value in the string
  inline char32_t getUnicode(unsigned int n) const    { return Str::utf16to32(Str::getUnicode16(d, n)); } // returns the [n] unicode value in the string
  //uint16_t *getChar(unsigned int n) const    { return Str::getChar16(d, n); }    // returns n-th character in string (diacriticals are considered part of a character)
  
  bool operator==(const str16 &s) const;   // checks if strings are identical
  bool operator==(const str32 &s) const;   // checks if strings are identical
  bool operator==(const str8 &s) const;    // checks if strings are identical
  bool operator==(const char16_t *) const; // checks if strings are identical (UTF-16)
  bool operator==(const char32_t *) const; // checks if strings are identical (UTF-32)
  bool operator==(const char *s) const;    // checks if strings are identical (UTF-8)
  bool operator==(char32_t) const;         // string must be 1 char long (+terminator) and identical to passed unicode value
  
  inline bool operator!=(const str16 &s)    const { return !(operator==(s)); }
  inline bool operator!=(const str32 &s)    const { return !(operator==(s)); }
  inline bool operator!=(const str8 &s)     const { return !(operator==(s)); }
  inline bool operator!=(const char32_t *s) const { return !(operator==(s)); }
  inline bool operator!=(const char16_t *s) const { return !(operator==(s)); }
  inline bool operator!=(const char *s)     const { return !(operator==(s)); }
  inline bool operator!=(char32_t c)        const { return !(operator==(c)); }

  inline bool operator!() { return len? true: false; };
};


inline str16 operator+(const char *s1,     const str16 &s2) { return str16(s1)+= s2; }
inline str16 operator+(const char16_t *s1, const str16 &s2) { return str16(s1)+= s2; }
inline str16 operator+(const char32_t *s1, const str16 &s2) { return str16(s1)+= s2; }






