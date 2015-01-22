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

class str8 {
public:
  uint8_t *d;                 // main internal data storage UTF-8 format (if u mess with it, call updateLen() !!! )
  int32_t len;                // size of string in bytes (NOT NUMBER OF CHARACTERS! check nrChars for that / use strchars())
  int32_t nrChars;            // nr of characters in string (a character can have multiple diacriticals)
  int32_t nrCombs;            // nr of combining diacriticals in string

  uint32_t *d32;              /// buffer for corventing to utf-32
  uint16_t *dWin;             /// buffer for converting to windows 16bit strings

  // main functions
  
  str8 &f(const void *, ...);    // format string using sprintf(...) MAX LENGTH IS 1024, so dont re-write books with this one; use it on a line by line basis
  void updateLen();         /// updates len and nrChars vars, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]

  uint32_t *convert32();      /// returns string as UTF-32
  uint16_t *convertWin();     /// returns string as windows 16bit wide char
  void lower();             /// converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();             /// converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  
  // UTF8 functions. These are SECURE functions. Read & validate each character - they unpack the whole string& pack it back up after validating each char

  str8 &secureUTF8(const void *);         // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a UTF-8 string
  void readUTF8(FILE *);            /// read all (remaining) file       (SECURE)
  void readUTF8n(FILE *, size_t);   /// read n characters from file     (SECURE)
  void readLineUTF8(FILE *);        /// read till end of line (or file) (SECURE)
  
  // combining diacritical characters
  
  void clearComb();                   /// clears all combining diacritical characters from the string

  // constructors
  
  str8():                  d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) {}            // main constructor
  str8(const str8 &s):     d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) { *this= s; } // main constructor
  str8(const uint8_t *s):  d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) { *this= s; } // main constructor - copy from UTF-8 array string
  str8(const uint32_t *s): d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) { *this= s; } /// converts from utf-32
  str8(const uint32_t c):  d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) { *this= c; } /// makes 1 char length string + terminator
  str8(const char *s):     d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) { *this= (const uint8_t *)s; } /// asumes the char string is UTF-8 encoded (char can vary from os to os)
  str8(const uint16_t *s): d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) { *this= s; } /// converts from a 16bit string (windows compatibility) not utf-16 !!!

  // destructor / cleaners
  
  ~str8() { delData(); clean(); }
  void delData();                     /// called by destructor, can be used to clean the string
  void clean();                       /// deallocs conversion buffers (conversion buffers must be deallocated only on destructor & convert32 & convertWin)

  //operators
  
  str8 &operator= (const str8 &);     /// assign from another string8 class
  str8 &operator= (const uint8_t *);         /// assign from a utf-8 simple string
  str8 &operator= (const uint32_t *);        /// assign from a utf-32 array string
  str8 &operator= (const uint32_t);       /// makes a 1 char length string of input char (+terminator)
  str8 &operator= (const uint16_t *);        /// assign from a windows 16bit character array string (THIS SHOULD BE REPLACED WHEN UTF-16 IS MADE)
  str8 &operator= (const char *s) { return *this= (const uint8_t *)s; }  /// asumes the char string is utf-8 encoded (char can vary from os to os)


  str8 operator+(const str8 &s) const    { return str8(*this)+= s; }  /// return a temp string (this+other)
  //str8 operator+(cuint8 *s) const        { return str8(*this)+= s; } /// return a temp string (this+other)
  str8 operator+(const uint32_t c) const { return str8(*this)+= c; }  /// return a temp string (this+ long char)
  str8 operator+(const char *s) const    { return str8(*this)+= (const uint8_t *)s; } /// return a temp string (this+other)

  str8 &operator+=(const str8 &);   /// adds another string to current
  str8 &operator+=(const uint8_t *);          /// adds an utf-8 array string
  str8 &operator+=(const uint32_t *);         /// adds an utf-32 array string
  str8 &operator+=(const uint32_t);           /// adds a (unicode) character
  str8 &operator+=(const uint16_t *);         /// adds a windows 16bit wide character
  str8 &operator+=(const char *s) { return *this= (const uint8_t *)s; }  /// asume [char *] points to an utf-8 encoded string

  str8 &operator-=(const int n);       /// clears n char(s) from string
  str8 operator-(int n) const;         /// returns a temp string that has n less chars

  //operator char*()   { return (char *)d; }    /// returns the string as utf-8
  operator const char*()  { return (const char *)d; }   /// returns the string as utf-8
  operator const uint8_t *()  { return d; }            /// returns the string as utf-8
  operator const uint32_t *() { return convert32(); }  /// returns the string as utf-32
  operator const uint16_t *() { return convertWin(); } /// converts string to windows 16bit 

  uint8_t *operator[](unsigned int n) const { return (uint8_t *)Str::getChar8(d, n); }    /// returns n-th char in string as utf-8 (diacriticals are considered part of a character)
  uint8_t *getChar(unsigned int n) const    { return (uint8_t *)Str::getChar8(d, n); }    /// returns n-th char in string as utf-8 same as operator[] (diacriticals are considered part of a character)
  uint8_t *getUnicode(unsigned int n) const { return (uint8_t *)Str::getUnicode8(d, n); } /// returns n-th unicode value in the string - either a char or a diacritical


  bool operator==(const str8 &s) const;    /// checks if strings are identical
  bool operator==(const uint8_t *s) const;           /// checks if strings are identical (utf-8)
  bool operator==(const uint32_t *) const;           /// checks if strings are identical (utf-32)
  bool operator==(const uint32_t) const;             /// string must be 1 char long (+terminator) and identical to input character
  bool operator==(const char *s) const { return *this== (uint8_t *)s; } /// checks if strings are identical (utf-8)
  //bool operator==(char *s) const;             /// checks if strings are identical (utf-8)
  bool operator==(const uint16_t *) const;      /// checks if strings are identical (windows compatibility)
  

  bool operator!=(const str8 &s) const  { return !(operator==(s)); }
  bool operator!=(const uint8_t *s) const      { return !(operator==(s)); }
  bool operator!=(const uint32_t *s) const     { return !(operator==(s)); }
  bool operator!=(const uint32_t c) const      { return !(operator==(c)); }
  bool operator!=(const char *s) const       { return !(operator==((uint8_t *)s)); }
  bool operator!=(const uint16_t *s) const     { return !(operator==(s)); }
  
  
  operator bool() const { return len? true: false; }
  bool operator!() { return len? false: true; }

  #ifdef STR32CLASSINCLUDED
  str8(const str32 &s): d(NULL), len(0), nrChars(0), nrCombs(0), d32(NULL), dWin(NULL) { *this= s.d; }
  str8 &operator= (const str32 &s)      { return *this= s.d; }
  str8 operator+(const str32 &s) const  { return str8(*this)+= s.d; }
  str8 &operator+=(const str32 &s)      { return *this+= s.d; }
  #endif

};

//str8 operator+(const uint8_t *s1, const str8 &s2);
str8 operator+(const char *s1, const str8 &s2); // { return str8(s1)+= s2; }
//str8 operator+(const str8 &s1, const char s2); // { return str8(s1)+= s2; }
//str8 operator+(const char *s1, const str8 &s2) { return str8(s1)+ s2; }









