#pragma once
#define STR8CLASSINCLUDED 1

// WARNING:
// -if you manually mess with d (internal data), use updateLen(), to update
//    the <len/nrchars> vars, as they are used internally; Str assumes
//    <len/nrchars> variables have the correct values set, so it doesn't
//    recalculate them @ every operation (SPEED is favoured)

// -this class is not using any c/c++ string function. At this moment with the
//    windows/linux war... setting locale& other stuff is just not possible as
//    everithing differs.

// -fopen knows of utf-8 but the win version wants to put a frakin BOM in the
//    file, wich cause problems in linux, so a file should be opened as pure
//    binary (at least when writing). This should avoid fseek problems too
//    (fseek is used internally)

// -utf-16 is NOT handled atm



// NOTES:
// character              - char - usually what every program knows about a character
// combining diacritical  - comb - each character can be created using multiple combining diacriticals.
//                                 string handles them; usually is good not to use them unless building a sofisticated editor / whatever sofisticated character handling program
//                                 use clearComb() to remove all of them from a string
//                                 use isComb() to know if a specific unicode value is a combining diacritical

// bad characters are marked with 0xFFFD when using secure read funcs

// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

// TODO:
// * [LOW PRIO] a nice formatting? fast & clean... current format produces tons of warnings on diff os-es (use 8/16/32/64 bit for an int)
// * think about memory alloc... every change is a delete+alloc
// * check asembler book for processor string instructions, maybe do the whole thing in asembler (probly it is best for the string8 class)
// * if utf-16 will be implemented, all short* strings will be utf-16 compatible, and windows compatibility will remain in convertWin() and strlenWin()
//   there has to be a new type of operator= (windows string)
//   ^^^^^^^^^ utf-16 to be or not to be ^^^^^^^^^

#include "strCommon.h"

class Str8 {
public:
  uint8 *d;                 // main internal data storage UTF-8 format (if u mess with it, call updateLen() !!! )
  int32 len;                // size of string in bytes (NOT NUMBER OF CHARACTERS! check nrChars for that / use strchars())
  int32 nrChars;            // nr of characters in string (a character can have multiple diacriticals)
  int32 nrCombs;            // nr of combining diacriticals in string

  uint32 *d32;              /// buffer for corventing to utf-32
  uint16 *dWin;             /// buffer for converting to windows 16bit strings

  // main functions
  
  Str8 &f(cvoid *, ...);    // format string using sprintf(...) MAX LENGTH IS 1024, so dont re-write books with this one; use it on a line by line basis
  void updateLen();         /// updates len and nrChars vars, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]

  uint32 *convert32();      /// returns string as UTF-32
  uint16 *convertWin();     /// returns string as windows 16bit wide char
  void lower();             /// converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();             /// converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  
  // UTF8 functions. These are SECURE functions. Read & validate each character - they unpack the whole string& pack it back up after validating each char

  Str8 &secureUTF8(cvoid *);         // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a UTF-8 string
  void readUTF8(FILE *);            /// read all (remaining) file       (SECURE)
  void readUTF8n(FILE *, size_t);   /// read n characters from file     (SECURE)
  void readLineUTF8(FILE *);        /// read till end of line (or file) (SECURE)
  
  // combining diacritical characters
  
  void clearComb();                   /// clears all combining diacritical characters from the string

  // constructors
  
  Str8():              d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) {}            // main constructor
  Str8(const Str8 &s): d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) { *this= s; } // main constructor
  Str8(cuint8 *s):     d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) { *this= s; } // main constructor - copy from UTF-8 array string
  Str8(cuint32 *s):    d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) { *this= s; } /// converts from utf-32
  Str8(cuint32 c):     d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) { *this= c; } /// makes 1 char length string + terminator
  Str8(cchar *s):      d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) { *this= (cuint8 *)s; } /// asumes the char string is UTF-8 encoded (char can vary from os to os)
  Str8(cuint16 *s):    d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) { *this= s; } /// converts from a 16bit string (windows compatibility) not utf-16 !!!

  // destructor / cleaners
  
  ~Str8() { delData(); clean(); }
  void delData();                     /// called by destructor, can be used to clean the string
  void clean();                       /// deallocs conversion buffers (conversion buffers must be deallocated only on destructor & convert32 & convertWin)

  //operators
  
  Str8 &operator= (const Str8 &);     /// assign from another string8 class
  Str8 &operator= (cuint8 *);         /// assign from a utf-8 simple string
  Str8 &operator= (cuint32 *);        /// assign from a utf-32 array string
  Str8 &operator= (cuint32);       /// makes a 1 char length string of input char (+terminator)
  Str8 &operator= (cuint16 *);        /// assign from a windows 16bit character array string (THIS SHOULD BE REPLACED WHEN UTF-16 IS MADE)
  Str8 &operator= (cchar *s) { return *this= (cuint8 *)s; }  /// asumes the char string is utf-8 encoded (char can vary from os to os)


  Str8 operator+(const Str8 &s) const { return Str8(*this)+= s; }  /// return a temp string (this+other)
  //Str8 operator+(cuint8 *s) const        { return Str8(*this)+= s; } /// return a temp string (this+other)
  Str8 operator+(cuint32 c) const        { return Str8(*this)+= c; }  /// return a temp string (this+ long char)
  Str8 operator+(cchar *s) const  { return Str8(*this)+= (cuint8 *)s; } /// return a temp string (this+other)

  Str8 &operator+=(const Str8 &);   /// adds another string to current
  Str8 &operator+=(cuint8 *);          /// adds an utf-8 array string
  Str8 &operator+=(cuint32 *);         /// adds an utf-32 array string
  Str8 &operator+=(cuint32);           /// adds a (unicode) character
  Str8 &operator+=(cuint16 *);         /// adds a windows 16bit wide character
  Str8 &operator+=(cchar *s) { return *this= (cuint8 *)s; }  /// asume [char *] points to an utf-8 encoded string

  Str8 &operator-=(const int n);       /// clears n char(s) from string
  Str8 operator-(int n) const;         /// returns a temp string that has n less chars

  //operator char*()   { return (char *)d; }    /// returns the string as utf-8
  operator cchar*()  { return (cchar *)d; }   /// returns the string as utf-8
  operator cuint8*()  { return d; }            /// returns the string as utf-8
  operator cuint32*() { return convert32(); }  /// returns the string as utf-32
  operator cuint16*() { return convertWin(); } /// converts string to windows 16bit 

  uint8 *operator[](uint n) const { return (uint8 *)Str::getChar8(d, n); }    /// returns n-th char in string as utf-8 (diacriticals are considered part of a character)
  uint8 *getChar(uint n) const    { return (uint8 *)Str::getChar8(d, n); }    /// returns n-th char in string as utf-8 same as operator[] (diacriticals are considered part of a character)
  uint8 *getUnicode(uint n) const { return (uint8 *)Str::getUnicode8(d, n); } /// returns n-th unicode value in the string - either a char or a diacritical


  bool operator==(const Str8 &s) const;    /// checks if strings are identical
  bool operator==(cuint8 *s) const;           /// checks if strings are identical (utf-8)
  bool operator==(cuint32 *) const;           /// checks if strings are identical (utf-32)
  bool operator==(cuint32) const;             /// string must be 1 char long (+terminator) and identical to input character
  bool operator==(cchar *s) const { return *this== (uint8 *)s; } /// checks if strings are identical (utf-8)
  //bool operator==(char *s) const;             /// checks if strings are identical (utf-8)
  bool operator==(const uint16 *) const;      /// checks if strings are identical (windows compatibility)
  

  bool operator!=(const Str8 &s) const  { return !(operator==(s)); }
  bool operator!=(cuint8 *s) const      { return !(operator==(s)); }
  bool operator!=(cuint32 *s) const     { return !(operator==(s)); }
  bool operator!=(cuint32 c) const      { return !(operator==(c)); }
  bool operator!=(cchar *s) const       { return !(operator==((uint8 *)s)); }
  bool operator!=(cuint16 *s) const     { return !(operator==(s)); }
  
  
  operator bool() const { return len? true: false; }
  bool operator!() { return len? false: true; }

  #ifdef STR32CLASSINCLUDED
  Str8(const Str32 &s): d(null), len(0), nrChars(0), nrCombs(0), d32(null), dWin(null) { *this= s.d; }
  Str8 &operator= (const Str32 &s)      { return *this= s.d; }
  Str8 operator+(const Str32 &s) const  { return Str8(*this)+= s.d; }
  Str8 &operator+=(const Str32 &s)      { return *this+= s.d; }
  #endif

};

//Str8 operator+(cuint8 *s1, const Str8 &s2);
Str8 operator+(cchar *s1, const Str8 &s2); // { return Str8(s1)+= s2; }
//Str8 operator+(const Str8 &s1, const char s2); // { return Str8(s1)+= s2; }
//Str8 operator+(cchar *s1, const Str8 &s2) { return Str8(s1)+ s2; }









