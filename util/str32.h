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
class Str8;
#endif

class Str32 {
public:
  uint32 *d;                  // main internal data storage 32bits  (if u mess with it, call updateLen() !!! )
  int32 len;                  // string length in bytes
  int32 nrChars;              // number of characters in string (combining diacriticals are considered part of a character, therefore not counted)
  int32 nrCombs;              // number of diacriticals in string
  /// conversion to other strings. data is kept! (use clean() to remove them manually)

  uint8  *d8;                 /// secondary utf-8 bit data storage  (if u mess with it, call updateLen() !!! )
  uint16 *dWin;               /// used for windows compatibility -16 bit data storage    (if u mess with it, call updateLen() !!! )

private:
  bool modif8;                /// [internal] flag: if the string is modified, d8[] needs to be recreated (in case it is used
  bool modifWin;              /// [internal] flag: if the string is modified, dWin[] needs to be recreated (in case it is used)

public:
  // main functions

  void updateLen();           /// updates string length vars, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]

  void *convert8();           /// UTF-32 to UTF-8 conversion
  uint16 *convertWin();       /// UTF-32 to windows 16bit wide character format
  void lower();               /// converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();               /// converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  

  // WIP vvv
  void wrap(uint32 *);


  Str32 &f(cchar *text, ...);
  Str32 &addInt(int64);
  Str32 &addUInt(uint64);
  Str32 &addFloat(float);
  Str32 &addDouble(double);
  // WIP ^^^

  // UTF8 functions. These are SECURE functions. Read & validate each character

  Str32 &secureUTF8(cvoid *);      // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
  void readUTF8(FILE *);             /// read all (remaining) file       (SECURE)
  void readUTF8n(FILE *, size_t);    /// read n characters from file     (SECURE)
  void readLineUTF8(FILE *);         /// read till end of line (or file) (SECURE)

  // combining diacritical characters

  void clearComb();                     /// clears all combining diacritical characters from the string

  // constructors

  Str32();                                   // main constructor
  Str32(const Str32 &s): d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false) { *this= s; } /// from another string32
  Str32(cuint32 *s):     d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false) { *this= s; } /// from UTF-32 array string
  Str32(cuint8 *s):      d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false) { *this= s; } /// from UTF-8 array string
  Str32(uint32 c):       d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false) { *this= c; } /// makes 1 char length string + terminator
  Str32(cuint16 *s):     d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false) { *this= s; } /// from windows 16bit wide char string
  Str32(cchar *s): Str32((uint8 *)s) {}   // c++11 feature

  // destructor / cleaners

  ~Str32();
  void delData();                         /// can be used to clean the string - this is used by destructor too
  void clean();                           /// clears everything but internal data [d] variable (other buffers: d8/ dWin)

  //operators

  Str32 &operator= (const Str32 &); /// assign from another string32 class
  Str32 &operator= (cuint32 *);        /// assign from UTF-32 array string
  Str32 &operator= (cuint8 *);         /// assign from UTF-8 array string
  Str32 &operator= (uint32);           /// makes a 1 legth string of input char (+terminator)
  Str32 &operator= (cuint16 *);        /// assign from windows 16bit wide char array string
  Str32 &operator= (cchar *s) { return *this= (uint8 *)s; } /// assign from UTF-8 array string

  Str32 operator+(const Str32 &s) const { return Str32(*this)+= s; } /// adds two string32 then returns the result
  Str32 operator+(cuint32 *s) const  { return Str32(*this)+= s; }       /// adds string32 and UTF-32 arr str then returns the result
  Str32 operator+(cuint8 *s) const   { return Str32(*this)+= s; }       /// adds string32 and UTF-8 arr str then returns the result
  Str32 operator+(uint32 c) const    { return Str32(*this)+= c; }       /// adds string32 and a single unicode char then returns the result
  Str32 operator+(cchar *s) const    { return Str32(*this)+= (uint8 *)s; } /// adds string32 and UTF-8 arr str then returns the result

  Str32 &operator+=(const Str32 &); /// adds another string32 to current string32 class
  Str32 &operator+=(cuint32 *);        /// adds UTF-32 arr str to current string32 class
  Str32 &operator+=(cuint8 *);         /// adds UTF-8 arr str to current string32 class
  Str32 &operator+=(cuint16 *);        /// adds windows 16bit wide char str to current string32 class
  Str32 &operator+=(uint32);           /// adds adds a single unicode character to current string32 class
  Str32 &operator+=(cchar *s) { return *this+= (uint8 *)s; }   /// adds UTF-8 arr str to current string32 class

  Str32 &operator-=(int n);            /// clears n character(s) from string
  Str32 operator-(int n) const;        /// returns a temp string32 that has n less character(s)

  operator uint32 *() { return d; }                                        /// returns pointer to internal data (d)
  operator uint8 *()  { return (modif8? (uint8 *)convert8():  d8); }       /// returns pointer to internal data (d8)
  operator uint16 *() { return (modifWin? convertWin(): dWin); }           /// returns pointer to internal data (dWin)
  operator char *()   { return (modif8? (char *)convert8(): (char *)d8); } /// returns pointer to internal data (d8)

  operator bool() const     { return (len? true: false); } /// returns true if string has any characters

  uint32 *operator[](uint n) const { return Str::getChar32(d, n); } /// returns n-th character in string (there can be combining diacriticals in this full character)
  uint32 *getChar(uint n) const    { return Str::getChar32(d, n); } /// returns n-th character in string (same as operator[])
  uint32 getUnicode(uint n) const  { return d[n]; }                 /// returns the [n] unicode value in the string (doesn't care if it's a normal char or a comb)

  bool operator==(const Str32 &s) const;           /// checks if strings are identical
  bool operator==(cuint32 *) const;                   /// checks if strings are identical                  (utf-32)
  bool operator==(cuint8 *s) const { return operator==(Str32(s)); } /// checks if strings are identical (utf-8)
  bool operator==(uint32) const;                      /// string must be 1 char long (+terminator) and identical to passed unicode character
  bool operator==(cchar *s)  const { return operator==(Str32(s)); } /// checks if strings are identical (utf-8)
  bool operator==(const uint16 *) const;              /// checks if strings are identical (windows 16bit wide char string)
  

  bool operator!=(const Str32 &s) const { return !(operator==(s)); }
  bool operator!=(cuint32 *s) const     { return !(operator==(s)); }
  bool operator!=(cuint8 *s) const      { return !(operator==(s)); }
  bool operator!=(uint32 c) const       { return !(operator==(c)); }
  bool operator!=(cuint16 *s) const     { return !(operator==(s)); }
  bool operator!=(cchar *s) const       { return !(operator==(s)); }

  bool operator!() { return len? true: false; };

  #ifdef STR8CLASSINCLUDED
  Str32(const Str8 &s): d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false) { *this= s.d; }
  Str32 &operator= (const Str8 &s) { return *this= s.d; }
  Str32 operator+(const Str8 &s) const { return Str32(*this)+= s.d; }
  Str32 &operator+=(const Str8 &s) { return *this+= s.d; }
  #endif /// STR8CLASSINCLUDED
};






