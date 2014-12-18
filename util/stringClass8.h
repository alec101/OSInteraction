#pragma once
#define STRINGCLASS8INCLUDED 1

// WARNING:
// -if you manually mess with d (internal data), use updateLen(), to update
//    the <len/nrchars> vars, as they are used internally; string assumes
//    <len/nrchars> variables have the correct values set, so it doesn't
//    recalculate them @ every operation (SPEED is favoured)

// -this class is not using any c/c++ string function. At this moment with the
//    windows/linux war... setting locale& other stuff is just not possible as
//    everithing differs.

// -fopen knows of utf-8 but the win version wants to put a frakin BOM in the
//    file, wich cause problems in linux, so a file should be opened as pure
//    binary (at least when writing). This should avoid fseek problems too
//    (fseek is used internally)

// -utf-16 is NOT handled atm. maybe some util funcs will be made, but nothing
//    for 'internal' use. 



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
/// * check asembler book for processor string instructions, maybe do the whole thing in asembler (probly it is best for the string8 class)

/// * if utf-16 will be implemented, all short* strings will be utf-16 compatible, and windows compatibility will remain in convertWin() and strlenWin()
///   there has to be a new type of operator= (windows string)
///   ^^^^^^^^^ utf-16 to be or not to be ^^^^^^^^^



class string8 {
public:
  char  *d;                  // main internal data storage UTF-8 format (if u mess with it, call updateLen() !!! )
  long len;                // size bytes (NOT NUMBER OF CHARACTERS! use strchars() )
  long nrchars;            // nr of characters in string (including diacriticals)
  
  // main functions
  
  string8 &f(cchar *, ...);   //  format string using sprintf(...) MAX LENGTH IS 1024, so dont re-write books with this one; use it on a line by line basis
  string8 &fw(wchar_t *, ...);
  void updateLen();           /// updates len variable, in case you want to mess with the string internal data (d/d8/etc). [WARNING: see start of this header file]

  ulong *convert32();         /// returns string as utf-32
  ushort *convertWin();       /// returns string as windows compatible wide char
  void lower();               /// converts whole string to lowercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)
  void upper();               /// converts whole string to uppercase (special cases that 1 character transforms into multiple characters ARE NOT HANDLED)

  // UTF8 functions. These are SECURE functions. Read & validate each character - they unpack the whole string& pack it back up after validating each char

  string8 &secureUTF8(const char *);  // reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
  void readUTF8(FILE *);            /// read all (remaining) file       (SECURE)
  void readUTF8n(FILE *, size_t);   /// read n characters from file     (SECURE)
  void readLineUTF8(FILE *);        /// read till end of line (or file) (SECURE)
  
  // combining diacritical characters
  
  static bool isComb(const ulong c);         /// is it a combining diacritical (if u dont know what they are, use clearComb() to remove them)
  void clearComb();                         /// clears all combining diacritical characters from the string

  // utility functions, they won't AFFECT anything in string's internal data, or USE string internal data
  // util funcs start with <str>/<utfx> so there's a clear delimitation
  // any number of funcs can be put here, i guess, as there are few utf-x specific functions anyways/or OS independant funcs
  
  static size_t strlen(const char *);          /// size in bytes
  static size_t strlenWin(const ushort *);     /// size in shorts (same as nr of chars) WINDOWS COMPATIBILITY
  static size_t strlen32(const ulong *);       /// size in longs  (same as nr of chars)
  static size_t strchars(const char *);        /// nr chars in an UTF-8 string (WITH combining diacriticals)
  static size_t strcombs(const char *);        /// nr combining diacriticals in an UTF-8 string
  static void strncpy(char *dst, cchar *src, uint); /// copies n chars from src to dst, 
  static void strcpy(char *dst, cchar *src);   /// copies src to dst; dst must be prealocated
  static ulong utf8to32(const char *);         /// returns character as utf-32
  static ulong utf8to32n(const char *, int);   /// returns n-th character as utf-32
  static size_t strcmp(cchar *, cchar *);      /// compares two strings, returns 0 if identical or <0 if str1 is less than str2, and viceversa
  static ulong tolower(ulong);                 /// converts a single unicode char to lowercase (special cases that convert to multiple characters are NOT HANDLED)
  static ulong toupper(ulong);                 /// converts a single unicode char to uppercase (special cases that convert to multiple characters are NOT HANDLED)
  
  // constructors
  
  string8();                                              // main constructor
  string8(const string8 &s):d(null), len(0) { *this= s; } // main constructor
  string8(const char *s):   d(null), len(0) { *this= s; } // main constructor
  string8(const ulong *s):  d(null), len(0) { *this= s; } /// converts from utf-32
  string8(const ushort *s): d(null), len(0) { *this= s; } /// converts from a 16bit string (windows compatibility) not utf-16 !!!
  string8(const ulong c):   d(null), len(0) { *this= c; } /// makes 1 char length string + terminator

  // destructor / cleaners
  
  ~string8();
  void delData();                         /// called by destructor, can be used to clean the string

  //operators
  
  string8 &operator= (const string8 &);    /// assign other string, same as constructor, dunno if it is needed
  string8 &operator= (const char *);       /// assign from a utf-8 simple string
  string8 &operator= (const ulong *);      /// assign from a utf-32 array (bassicaly what is used internally)
  string8 &operator= (const ushort *);     /// assign from a 16bit character simple string	(windows compatibility, shouldn't exist)

  // needs more testing v
  string8 &operator= (const ulong);        /// makes a 1 char length string of input char (+terminator)
  // needs more testing ^

  string8 operator+(const string8 &s) const { return string8(*this)+= s; } /// return a temp string (this+other)
  string8 operator+(const char *s) const    { return string8(*this)+= s; } /// return a temp string (this+other)
  string8 operator+(const ulong c) const    { return string8(*this)+= c; } /// return a temp string (this+ long char)

  string8 &operator+=(const string8 &);       /// adds another string to current
  string8 &operator+=(const ulong *);         /// adds an utf-32 string
  string8 &operator+=(const char *);          /// adds an utf-8 string
  string8 &operator+=(const ushort *);        /// windows compat
  string8 &operator+=(const ulong);           /// adds a (unicode) character

  string8 &operator-=(const int n);           /// clears n char(s) from string
  string8 operator-(int n) const;             /// returns a temp string that has n less chars

  operator char*()   { return d; }            /// returns string
  operator const char*() { return d; }
  operator ulong*()  { return convert32(); }  /// converts string to utf-32
  operator ushort*() { return convertWin(); } /// converts string to windows16bit

  cchar *operator[](uint n) const;            /// returns pointer to n-th char in string
  ulong getChar(uint n) const;                /// returns n-th character in string as unicode

  bool operator==(const string8 &s) const;      /// checks if strings are identical
  bool operator==(const ulong *) const;         /// checks if strings are identical (utf-32)
  bool operator==(const char *s) const;         /// checks if strings are identical (utf-8)
  bool operator==(char *s) const;               /// checks if strings are identical (utf-8)
  bool operator==(const ushort *) const;        /// checks if strings are identical (windows compatibility)
  bool operator==(const ulong) const;           /// string must be 1 char long (+terminator) and identical to input character

  bool operator!=(const string8 &s) const { return !(operator==(s)); }
  bool operator!=(culong *s) const        { return !(operator==(s)); }
  //  bool operator!=(cchar *s) const         { return !(operator==(s)); }
  bool operator!=(char *s) const          { return !(operator==(s)); }
  bool operator!=(cushort *s) const       { return !(operator==(s)); }
  bool operator!=(culong c) const         { return !(operator==(c)); }
  
  operator bool() const { return len? true: false; }
  bool operator!() { return len? false: true; }

  #ifdef STRINGCLASS32INCLUDED
  string8(const string32 &s): d(null), len(0) { *this= s.d; }
  string8 &operator= (const string32 &s)      { return *this= s.d; }
  string8 operator+(const string32 &s) const  { return string8(*this)+= s.d; }
  string8 &operator+=(const string32 &s)      { return *this+= s.d; }
  #endif

};

string8 operator+(cchar *s1, const string8 &s2);

#define _STRINGCLASS_CASECHANGE_DEFINED








