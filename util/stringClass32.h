#pragma once
#define STRINGCLASS32INCLUDED 1


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
//                                 string handles them; usually is good not to use them unless building a sofisticated editor / whatever sofisticated character handling program
//                                 use clearComb() to remove all of them from a string
//                                 use isComb() to know if a character is a combining diacritical

// bad characters are marked with 0xFFFD when using secure read funcs

// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

/// TODO:
/// * think about memory alloc... every change is a delete+alloc

#ifdef STRINGCLASS8INCLUDED
class string8;
#endif

class string32 {
  // internal - if the string was changed, these vars are set to true, so the convert functions are called if needed
  bool modif8;            /// internal flag: if the string is modified, d8[] needs to be recreated (in case it is used
  bool modif16;           /// internal flag: if the string is modified, d16[] needs to be recreated (in case it is used)

public:
  ulong  *d;                  // main internal data storage 32bits  (if u mess with it, call updateLen() !!! )
  size_t len;                 // characters + combining diacriticals

  /// conversion to other strings. data is kept! (use clean() to remove them manually)

  char   *d8;                 /// secondary utf-8 bit data storage  (if u mess with it, call updateLen() !!! )
  ushort *d16;                /// secondary 16 bit data storage     (if u mess with it, call updateLen() !!! )

  // main functions

  void updateLen();           /// updates len variable, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]

  char *convert8();           /// makes d8 if necessary then returns it;
  ushort *convert16();        /// makes d16 if neccessary then returns it

  // UTF8 functions. These are SECURE functions. Read & validate each character

  string32 &secureUTF8(const char *); // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
  void readUTF8(FILE *);             /// read all (remaining) file       (SECURE)
  void readUTF8n(FILE *, size_t);    /// read n characters from file     (SECURE)
  void readLineUTF8(FILE *);         /// read till end of line (or file) (SECURE)

  // combining diacritical characters

  bool isComb(const ulong c) const;         /// is it a combining diacritical (if u dont know what they are, use clearComb() to remove them)
  void clearComb();                         /// clears all combining diacritical characters from the string

  // utility functions, they won't AFFECT anything in string's internal data, or USE string internal data
  // util funcs start with <str>/<utfx> so there's a clear delimitation
  // any number of funcs can be put here, i guess, as there are few utf-x specific functions anyways/or OS independant funcs

  size_t strlen(const ulong *) const;       /// size in longs  (same as nr of chars)
  size_t strlen16(const ushort *) const;    /// size in shorts (same as nr of chars)
  size_t strlen8(const char *) const;       /// size in bytes  (NOT THE NUMBER OF STRING CHARACTERS)

  size_t strchars8(const char *) const;     /// nr chars in UTF-8 string (WITH combining diacriticals)
  size_t strcombs8(const char *) const;     /// nr combining diacriticals in UTF-8 string

  ulong utf8to32(const char *) const;       /// returns character as utf-32
  ulong utf8to32n(const char *, int) const; /// returns n-th character as utf-32

  // constructors

  string32();                                   // main constructor
  string32(const string32 &s):modif8(false), modif16(false), d(null), len(0), d8(null), d16(null) { *this= s; } // main constructor
  string32(const ulong *s):   modif8(false), modif16(false), d(null), len(0), d8(null), d16(null) { *this= s; } // main constructor
  string32(const ushort *s):  modif8(false), modif16(false), d(null), len(0), d8(null), d16(null) { *this= s; } /// converts from a 16bit string (windows compatibility) not utf-16 !!!
  string32(const char *s):    modif8(false), modif16(false), d(null), len(0), d8(null), d16(null) { *this= s; } /// converts from utf-8 string
  string32(const ulong c):    modif8(false), modif16(false), d(null), len(0), d8(null), d16(null) { *this= c; } /// makes 1 char length string + terminator

  // destructor / cleaners

  ~string32();
  void delData();                         /// called by destructor, can be used to clean the string
  void clean();                           /// clears everything but d (deallocs d8/d16)

  //operators

  string32 &operator= (const string32 &);   /// assign other string, same as constructor, dunno if it is needed
  string32 &operator= (const ulong *);      /// assign from a utf-32 array (bassicaly what is used internally)
  string32 &operator= (const char *);       /// assign from a utf-8 simple string
  string32 &operator= (const ushort *);     /// assign from a 16bit character simple string	(windows compatibility, shouldn't exist)
  string32 &operator= (const ulong);        /// makes a 1 legth string of input char (+terminator)

  string32 operator+(const string32 &s) const { return string32(*this)+= s; } /// return a temp string (this+other)
  string32 operator+(const char *s) const   { return string32(*this)+= s; } /// return a temp string (this+other)
  string32 operator+(const ulong c) const   { return string32(*this)+= c; } /// return a temp string (this+ long char)

  string32 &operator+=(const string32 &);     /// adds another string to current
  string32 &operator+=(const ulong *);      /// adds an utf-32 string
  string32 &operator+=(const char *);       /// adds an utf-8 string
  string32 &operator+=(const ushort *);     /// windows compat
  string32 &operator+=(const ulong);        /// adds a character


  string32 &operator-=(const int n);        /// clears n char(s) from string
  string32 operator-(int n) const;          /// returns a temp string that has n less chars

  operator ulong*() const { return d; }
  operator char*()        { return modif8?  convert8():  d8;  }  /// makes d8 if neccesary then returns it
  operator ushort*()      { return modif16? convert16(): d16; }  /// makes d16 if necessary then returns it
  operator bool() const   { return len? true: false; }


  ulong operator[](uint n) const { return d[n]; }         /// returns n-th char in string
  ulong getChar(uint n) const    { return d[n]; }         /// operator[] - return n-th character in string;


  bool operator==(const string32 &s) const;               /// checks if strings are identical
  bool operator==(const ulong *) const;                   /// checks if strings are identical                     (utf-32)
  bool operator==(const char *s) const    { return operator==(string32(s)); } /// checks if strings are identical (utf-8)
  bool operator==(const ushort *) const;                  /// checks if strings are identical (windows compatibility)
  bool operator==(const ulong) const;                     /// string must be 1 char long (+terminator) and identical to input character

  bool operator!=(const string32 &s) const  { return !(operator==(s)); }
  bool operator!=(culong *s) const          { return !(operator==(s)); }
  bool operator!=(cchar *s) const           { return !(operator==(s)); }
  bool operator!=(cushort *s) const         { return !(operator==(s)); }
  bool operator!=(culong c) const           { return !(operator==(c)); }


  bool operator!() { return len? true: false; };

  #ifdef STRINGCLASS8INCLUDED
  string32(const string8 &s):modif8(false), modif16(false), d(null), len(0), d8(null), d16(null) { *this= s.d; }
  string32 &operator= (const string8 &s) { return *this= s.d; }
  string32 operator+(const string8 &s) const { return string32(*this)+= s.d; }
  string32 &operator+=(const string8 &s) { return *this+= s.d; }
  #endif /// STRINGCLASS8INCLUDED
};






