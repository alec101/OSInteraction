#pragma once
#define STR32CLASSINCLUDED 1


// WARNING:
// -if you manually mess with d/d8/d16 (internal data), use updateLen(), to update
//  the <len> var, as it is used internally string assumes <len> variable has the
//  correct number set, so it doesn't recalculate it @ every string operation
//  (SPEED is favoured)

// -this class is not using any c/c++ string function. At this moment with the windows/linux war...
//  setting locale& other stuff is just not possible as everything differs.

// -fopen knows of utf-8 but the win version wants to put a frakin BOM in the file,
//  wich cause problems in linux, so a file should be opened as pure binary (at
//  least when writing). This should avoid fseek problems too (fseek is used internally)

// -utf-16 is NOT handled atm. maybe some util funcs will be made, but nothing for 'internal' use.
//  d16 (internal data) is used for windows compatibility, IT IS NOT UTF-16


// NOTES:
// character              - char - usually what every program knows about a character
// combining diacritical  - comb - each character can be created using multiple combining diacriticals.
//                                 string handles them; usually is good not to use them unless building
//                                 a sofisticated editor / whatever sofisticated character handling program
//                                 * use clearComb() to remove all of them from a string
//                                 * use isComb() to know if a character is a combining diacritical

// bad characters are marked with 0xFFFD when using secure read funcs

// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

/// TODO:
/// * think about memory alloc... every change is a delete+alloc

#include "strCommon.h"

#ifdef STR8CLASSINCLUDED
class str8;
#endif

class str32 {
public:
  uint32_t *d;                  // main internal data storage 32bits  (if u mess with it, call updateLen() !!! )
  int32_t len;                  // string length in bytes
  int32_t nrChars;              // number of characters in string (combining diacriticals are considered part of a character, therefore not counted)
  int32_t nrCombs;              // number of diacriticals in string
  // conversion to other strings. data is kept! (use clean() to remove them manually)

  uint8_t  *d8;                 // secondary utf-8 bit data storage  (if u mess with it, call updateLen() !!! )
  uint16_t *dWin;               // used for windows compatibility -16 bit data storage    (if u mess with it, call updateLen() !!! )

private:
  bool modif8;                // [internal] flag: if the string is modified, d8[] needs to be recreated (in case it is used
  bool modifWin;              // [internal] flag: if the string is modified, dWin[] needs to be recreated (in case it is used)

public:
  // main functions

  void updateLen();           // updates string length vars, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]

  void *convert8();           // UTF-32 to UTF-8 conversion
  uint16_t *convertWin();     // UTF-32 to windows 16bit wide character format
  void lower();               // converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();               // converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  

  // WIP vvv
  void wrap(uint32_t *);


  str32 &f(const char *text, ...);
  str32 &addInt(int64_t);
  str32 &addUInt(uint64_t);
  str32 &addFloat(float);
  str32 &addDouble(double);
  // WIP ^^^

  // UTF8 functions. These are SECURE functions. Read & validate each character

  str32 &secureUTF8(const void *);   // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
  void readUTF8(FILE *);             // read all (remaining) file       (SECURE)
  void readUTF8n(FILE *, size_t);    // read n characters from file     (SECURE)
  void readLineUTF8(FILE *);         // read till end of line (or file) (SECURE)

  // combining diacritical characters

  void clearComb();                  // clears all combining diacritical characters from the string

  // character or string insertion / deletion from a string

  void insert(uint32_t in_unicode, int32_t in_nUnicode= -1, int32_t in_nChar= -1);        // -1 = ignore; in_nUnicode - insert after n'th unicode; in_nChar - insert after n'th character (that can have combining diacriticals); if both are ignored, unicode is inserted at the end of the string; returns resulting string length in bytes (without terminator)
  void insertStr(const void *in_str, int32_t in_nUnicode= -1, int32_t in_nChar= -1);      // -1 = ignore; in_nUnicode - insert after n'th unicode; in_nChar - insert after n'th character (that can have combining diacriticals); if both are ignored, unicode is inserted at the end of the string; returns resulting string length in bytes (without terminator)
  void del(int32_t in_nUnicodesToDel= 1, int32_t in_nCharsToDel= 0, int32_t in_nUnicode= -1, int32_t in_nChar= -1); // -1 = ignore; in_nUnicodesToDel / in_nCharsToDel - number of unicodes or chars to del from string; in_nUnicode - delete n'th unicode (first if multiple); in_nChar - delete n'th character (first if multiple); if both are -1, the last unicode is deleted; returns resulting string length in bytes (without terminator)

  // search in string funcs

  void *search(void *in_str, void *in_search, bool in_caseSensitive= true);


  // constructors

  str32();                                   // main constructor
  str32(const str32 &s):    d(NULL), len(0), nrChars(0), nrCombs(0), d8(NULL), dWin(NULL), modif8(false), modifWin(false) { *this= s; } // from another string32
  str32(const uint32_t *s): d(NULL), len(0), nrChars(0), nrCombs(0), d8(NULL), dWin(NULL), modif8(false), modifWin(false) { *this= s; } // from UTF-32 array string
  str32(const uint8_t *s):  d(NULL), len(0), nrChars(0), nrCombs(0), d8(NULL), dWin(NULL), modif8(false), modifWin(false) { *this= s; } // from UTF-8 array string
  str32(uint32_t c):        d(NULL), len(0), nrChars(0), nrCombs(0), d8(NULL), dWin(NULL), modif8(false), modifWin(false) { *this= c; } // makes 1 char length string + terminator
  str32(const uint16_t *s): d(NULL), len(0), nrChars(0), nrCombs(0), d8(NULL), dWin(NULL), modif8(false), modifWin(false) { *this= s; } // from windows 16bit wide char string
  str32(const char *s):     str32((uint8_t *)s) {}   // c++11 feature

  // destructor / cleaners

  ~str32();
  void delData();                         // can be used to clean the string - this is used by destructor too
  void clean();                           // clears everything but internal data [d] variable (other buffers: d8/ dWin)

  //operators

  str32 &operator= (const str32 &);       // assign from another string32 class
  str32 &operator= (const uint32_t *);    // assign from UTF-32 array string
  str32 &operator= (const uint8_t *);     // assign from UTF-8 array string
  str32 &operator= (uint32_t);            // makes a 1 legth string of input char (+terminator)
  str32 &operator= (const uint16_t *);    // assign from windows 16bit wide char array string
  str32 &operator= (const char *s) { return *this= (uint8_t *)s; } // assign from UTF-8 array string

  str32 operator+(const str32 &s) const { return str32(*this)+= s; }      // adds two string32 then returns the result
  str32 operator+(const uint32_t *s) const  { return str32(*this)+= s; }  // adds string32 and UTF-32 arr str then returns the result
  str32 operator+(const uint8_t *s) const   { return str32(*this)+= s; }  // adds string32 and UTF-8 arr str then returns the result
  str32 operator+(uint32_t c) const    { return str32(*this)+= c; }       // adds string32 and a single unicode char then returns the result
  str32 operator+(const char *s) const    { return str32(*this)+= (uint8_t *)s; } // adds string32 and UTF-8 arr str then returns the result

  str32 &operator+=(const str32 &);     // adds another string32 to current string32 class
  str32 &operator+=(const uint32_t *);  // adds UTF-32 arr str to current string32 class
  str32 &operator+=(const uint8_t *);   // adds UTF-8 arr str to current string32 class
  str32 &operator+=(const uint16_t *);  // adds windows 16bit wide char str to current string32 class
  str32 &operator+=(uint32_t);          // adds adds a single unicode character to current string32 class
  str32 &operator+=(const char *s) { return *this+= (uint8_t *)s; }   // adds UTF-8 arr str to current string32 class

  str32 &operator-=(int n);             // clears n character(s) from string
  str32 operator-(int n) const;         // returns a temp string32 that has n less character(s)

  operator uint32_t *() { return d; }                                       // returns pointer to internal data (d)
  operator uint8_t *()  { return (modif8? (uint8_t *)convert8():  d8); }    // returns pointer to internal data (d8)
  operator uint16_t *() { return (modifWin? convertWin(): dWin); }          // returns pointer to internal data (dWin)
  operator char *()     { return (modif8? (char *)convert8(): (char *)d8); }// returns pointer to internal data (d8)

  operator bool() const     { return (len? true: false); } // returns true if string has any characters

  uint32_t *operator[](unsigned int n) const { return Str::getChar32(d, n); } // returns n-th character in string (there can be combining diacriticals in this full character)
  uint32_t *getChar(unsigned int n) const    { return Str::getChar32(d, n); } // returns n-th character in string (same as operator[])
  uint32_t getUnicode(unsigned int n) const  { return d[n]; }                 // returns the [n] unicode value in the string (doesn't care if it's a normal char or a comb)

  bool operator==(const str32 &s) const;           // checks if strings are identical
  bool operator==(const uint32_t *) const;         // checks if strings are identical                  (utf-32)
  bool operator==(const uint8_t *s) const { return operator==(str32(s)); } // checks if strings are identical (utf-8)
  bool operator==(uint32_t) const;                 // string must be 1 char long (+terminator) and identical to passed unicode character
  bool operator==(const char *s)  const { return operator==(str32(s)); } // checks if strings are identical (utf-8)
  bool operator==(const uint16_t *) const;         // checks if strings are identical (windows 16bit wide char string)
  

  bool operator!=(const str32 &s) const    { return !(operator==(s)); }
  bool operator!=(const uint32_t *s) const { return !(operator==(s)); }
  bool operator!=(const uint8_t *s) const  { return !(operator==(s)); }
  bool operator!=(uint32_t c) const        { return !(operator==(c)); }
  bool operator!=(const uint16_t *s) const { return !(operator==(s)); }
  bool operator!=(const char *s) const     { return !(operator==(s)); }

  bool operator!() { return len? true: false; };

  #ifdef STR8CLASSINCLUDED
  str32(const str8 &s): d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false) { *this= s.d; }
  str32 &operator= (const str8 &s) { return *this= s.d; }
  str32 operator+(const str8 &s) const { return str32(*this)+= s.d; }
  str32 &operator+=(const str8 &s) { return *this+= s.d; }
  #endif /// STR8CLASSINCLUDED

  inline int64_t toInt()   const { return Str::utf32toInt64(d); }
  inline uint64_t toUint() const { return Str::utf32toUint64(d); }
  inline float toFloat()   const { return Str::utf32toFloat(d); }
  inline double toDouble() const { return Str::utf32toDouble(d); }

  str32 &fromInt(int64_t n, int8_t base= 10, bool uppercase= true)   { if(len< 264) { if(d) delete[] d; d= new uint32_t[66]; len= 264; } nrChars= Str::int64toUtf32(n, d, base, uppercase);  nrCombs= 0; modif8= modifWin= true; return *this; }
  str32 &fromUint(uint64_t n, int8_t base= 10, bool uppercase= true) { if(len< 264) { if(d) delete[] d; d= new uint32_t[66]; len= 264; } nrChars= Str::uint64toUtf32(n, d, base, uppercase); nrCombs= 0; modif8= modifWin= true; return *this; }
  str32 &fromFloat(float n, int precision= 2, bool useE= false)      { if(len< 264) { if(d) delete[] d; d= new uint32_t[66]; len= 264; } nrChars= Str::floatToUtf32(n, d, precision, useE);  nrCombs= 0; modif8= modifWin= true; return *this; }
  str32 &fromDouble(double n, int precision= 2, bool useE= false)    { if(len< 264) { if(d) delete[] d; d= new uint32_t[66]; len= 264; } nrChars= Str::doubleToUtf32(n, d, precision, useE); nrCombs= 0; modif8= modifWin= true; return *this; }
};






