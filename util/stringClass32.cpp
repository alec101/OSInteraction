#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <stdio.h>
#include "typeShortcuts.h"
#include "stringClass32.h"


/* UTF 8 research
  -they say to start a file with a ZERO WIDTH NOBREAK SPACE (U+FEFF), which is
    in this role also referred to as the �signature� or �byte-order mark (BOM)�
  -linux avoids BOM, so i don't think there's gonna be any testing using BOM.
    only to ignore a BOM if the file starts with it

  -The following byte sequences are used to represent a character.
    The sequence to be used depends on the Unicode number of the character:
    U-00000000 � U-0000007F:  0xxxxxxx
    U-00000080 � U-000007FF:  110xxxxx 10xxxxxx
    U-00000800 � U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
    U-00010000 � U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    U-00200000 � U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    U-04000000 � U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

  -0300-036F Combining Diacritical Marks, since version 1.0, with modifications in subsequent versions down to 4.1
   1DC0-1DFF Combining Diacritical Marks Supplement, versions 4.1 to 5.2
   20D0-20FF Combining Diacritical Marks for Symbols, since version 1.0, with modifications in subsequent versions down to 5.1
   FE20-FE2F Combining Half Marks, versions 1.0, updates in 5.2

  -An important note for developers of UTF-8 decoding routines: For security reasons,
    a UTF-8 decoder must not accept UTF-8 sequences that are longer than necessary
    to encode a character. For example, the character U+000A (line feed) must be
    accepted from a UTF-8 stream only in the form 0x0A, but not in any of the
    following five possible overlong forms:
    0xC0 0x8A
    0xE0 0x80 0x8A
    0xF0 0x80 0x80 0x8A
    0xF8 0x80 0x80 0x80 0x8A
    0xFC 0x80 0x80 0x80 0x80 0x8A

   Any overlong UTF-8 sequence could be abused to bypass UTF-8 substring32 tests
     that look only for the shortest possible encoding.
   All overlong UTF-8 sequences start with one of the following byte patterns:

    1100000x (10xxxxxx)
    11100000 100xxxxx (10xxxxxx)
    11110000 1000xxxx (10xxxxxx 10xxxxxx)
    11111000 10000xxx (10xxxxxx 10xxxxxx 10xxxxxx)
    11111100 100000xx (10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx)

  -Also note that the code positions U+D800 to U+DFFF (UTF-16 surrogates) as well
   as U+FFFE and U+FFFF must not occur in normal UTF-8 or UCS-4 data.
   UTF-8 decoders should treat them like malformed or overlong sequences for safety reasons.

  -Markus Kuhn�s UTF-8 decoder stress test file contains a systematic collection of
   malformed and overlong UTF-8 sequences and will help you to verify the robustness of your decoder.
   http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

   putwchar(c)
{
  if (c < 0x80) {
    putchar (c);
  }
  else if (c < 0x800) {
    putchar (0xC0 | c>>6);
    putchar (0x80 | c & 0x3F);
  }
  else if (c < 0x10000) {
    putchar (0xE0 | c>>12);
    putchar (0x80 | c>>6 & 0x3F);
    putchar (0x80 | c & 0x3F);
  }
  else if (c < 0x200000) {
    putchar (0xF0 | c>>18);
    putchar (0x80 | c>>12 & 0x3F);
    putchar (0x80 | c>>6 & 0x3F);
    putchar (0x80 | c & 0x3F);
  }
}

*/


string32::string32():
modif8(false), modif16(false), d(null), len(0), d8(null), d16(null)
{}

string32::~string32() {
  delData();
}

void string32::delData() {
  if(d)   delete[] d;
  if(d8)  delete[] d8;
  if(d16) delete[] d16;

  d= null;
  d8= null;
  d16= null;
  len= 0;
  modif8= modif16= false;
}

void string32::clean() {
  if(d8)  { delete[] d8;  d8= null;  }
  if(d16) { delete[] d16; d16= null; }
}

// OPERATOR= //
///---------///
string32 &string32::operator=(const string32 &s) {
  modif16= modif8= true;
  len= s.len;
/// copy
  if(d) delete[] d;
  d= new ulong[len+ 1];
  for(uint a= 0; a< len+ 1; a++)
    d[a]= s.d[a];

  return *this;
}


string32 &string32::operator=(const ulong *s) {
  modif16= modif8= true;
/// string32 length
  const ulong *p= s;
  while(*p++);
  len= p- s- 1;
/// copy
  if(d) delete[] d;
  d= new ulong[len+ 1];
  for(uint a= 0; a< len+ 1; a++)
    d[a]= s[a];

  return *this;
}


string32 &string32::operator=(const ushort *s) {
  modif16= true;
  modif8= true;
/// string32 length
  const ushort *p= s;
  while(*p++);
  len= p- s- 1;
/// copy
  if(d) delete[] d;
  d= new ulong[len+ 1];
  for(size_t a= 0; a< len+ 1; a++)
    d[a]= s[a];

  return *this;
}


string32 &string32::operator=(const char *s) {
  modif8= true;
  modif16= true;
/// length
  const uchar *p= (const uchar*)s;
  len= 0;

  while(*p++)
    if((*p& 0xc0)!= 0x80)   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      len++;

// copy
  if(d) delete[] d;
  d= new ulong[len+ 1];
  p= (const uchar*)s;

  for(size_t a= 0; a< len+ 1; a++) {  /// for each character
/// character is ascii 0-127
    if(*p < 128) {
      d[a]= *p++;
      continue;
    }
/// character has 2 bytes
    if((*p& 0xe0) == 0xc0) {        /// test if first 3 bits (0xe0= 11100000) are 110xxxxx (0xc0= 11000000)
      d[a]= (*p++)& 0x1f;                /// last 5 bits (0x1f= 00011111)
      d[a]<<=6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      continue;
    }
/// character has 3 bytes
    if((*p& 0xf0) == 0xe0) {        /// test if first 4 bits (0xf0= 11110000) are 1110xxxx (0xe0= 11100000)
      d[a]= (*p++)& 0x0f;                 /// last 4 bits (0x0f= 00001111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      continue;
    }
/// character has 4 bytes
    if((*p& 0xf8) == 0xf0) {        /// test if first 5 bits (0xf8= 11111000) are 11110xxx (0xf0= 11110000)
      d[a]= (*p++)& 0x07;                 /// last 3 bits (0x07= 00000111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      continue;
    }
// the last 2 bytes are not used, but avaible if in the future unicode will expand
/// character has 5 bytes
    if((*p& 0xfc) == 0xf8) {        /// test if first 6 bits (0xfc= 11111100) are 111110xx (0xf8= 11111000)
      d[a]= (*p++)& 0x03;                 /// last 2 bits (0x03= 00000011)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      continue;
    }
/// character has 6 bytes
    if((*p& 0xfe) == 0xfc) {        /// test if first 7 bits (0xfe= 11111110) are 1111110x (0xfc= 11111100)
      d[a]= (*p++)& 0x01;                 /// last 1 bit  (0x01= 00000001)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      continue;
    }
  }	/// for each character

// MAKE d8 now? or only when conversions are needed? it just needs a copy from s

  return *this;
}


string32 &string32::operator=(const ulong c) {
  if(!c)
    if(d) {
      delData();
      return *this;
    }

  if(d)
    delete[] d;
  d= new ulong[2];
  d[0]= c;
  d[1]= 0;
  len= 1;
  modif8= modif16= true;

  return *this;
}



// OPERATOR += //
///-----------///

// this one is heavily used
string32 &string32::operator+=(const string32 &s) {
  if(!s.len) return *this;            /// other string32 empty -> return *this
  modif8= modif16= true;
  if(!len) return *this= s;           /// current string32 empty -> just copy other string32

  size_t a;
  ulong *p= new ulong[len+ s.len+ 1]; /// new string32 allocation
/// copy string32 1 to new string32
  for(a= 0; a< len; a++)
    p[a]= d[a];
  delete[] d;
  d= p;
/// copy string32 2 to new string32
  p+= a;
  for(a= 0; a< s.len+ 1; a++)	        /// +1= terminator
    p[a]= s.d[a];

  len+= s.len;
  return *this;
}


string32 &string32::operator+=(const char *s) {
  if(!s) return *this;
  modif8= modif16= true;
  if(!len) return *this= s;

  string32 p= s;
  return *this+= p;
}


string32 &string32::operator+=(const ushort *s) {
  if(!s) return *this;
  modif8= modif16= true;
  if(!len) return *this= s;

  string32 p= s;
  return *this+= p;
}


string32 &string32::operator+=(const ulong *s) {
  if(!s) return *this;
  modif8= modif16= true;
  if(!len) return *this= s;

  string32 p= s;
  return *this+= p;
}


string32 &string32::operator+=(const ulong c) {
  if(!c) return *this;
  modif8= modif16= true;
  if(!len) return *this= c;

/// create the new string32
  ulong *t= new ulong[len+ 2];
  size_t a;

  for(a= 0; a< len; a++)
    t[a]= d[a];
  t[a]= c;
  t[a+ 1]= 0;
/// delete old+ assign d to the new string32
  delete[] d;
  d= t;
  len++;

  return *this;
}





// CONVERSION operators //
///--------------------///

// UTF-8 conversion from UTF-32 or basically an array of ulongs (internal format)
char *string32::convert8() {
  if(modif8) {
    modif8= false;
    if(!len) return null;

/// length of d8
    size_t l= 0;

    for(size_t a= 0; a< len; a++)	 // for each character in d
      if(d[a]<=      0x0000007F)  /// 1 byte long   U-00000000�U-0000007F:  0xxxxxxx
        l++;
      else if(d[a]<= 0x000007FF)  /// 2 bytes long  U-00000080�U-000007FF:  110xxxxx 10xxxxxx
        l+= 2;
      else if(d[a]<= 0x0000FFFF)  /// 3 bytes long  U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        l+= 3;
      else if(d[a]<= 0x001FFFFF)  /// 4 bytes long  U-00010000�U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        l+= 4;
      else if(d[a]<= 0x03FFFFFF)  /// 5 bytes long  U-00200000�U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        l+= 5;
      else if(d[a]<= 0x7FFFFFFF)  /// 6 bytes long  U-04000000�U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        l+= 6;

/// create d8
    if(d8) delete[] d8;
    d8= new char[l+ 1];
    uchar *p= (uchar *)d8;

    for(size_t a= 0; a< len; a++)     // for each character in d
      if(d[a]<= 0x0000007F) {         //  1 byte   U-00000000�U-0000007F:  0xxxxxxx
        *p++= (uchar)d[a];
      } else if(d[a]<= 0x000007FF) {  //  2 bytes  U-00000080�U-000007FF:  110xxxxx 10xxxxxx
        *p++= (uchar)(d[a]>> 6)         | 0xC0;   /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
        *p++= (uchar)(d[a]&        0x3f)| 0x80;   /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(d[a]<= 0x0000FFFF) {  //  3 bytes  U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *p++= (uchar) (d[a]>> 12)       | 0xE0;   /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
        *p++= (uchar)((d[a]>> 6)&  0x3F)| 0x80;   /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uchar) (d[a]&       0x3F)| 0x80;   /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(d[a]<= 0x001FFFFF) {	//  4 bytes U-00010000�U-001FFFFF:   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *p++= (uchar) (d[a]>> 18)       | 0xF0;   /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
        *p++= (uchar)((d[a]>> 12)& 0x3F)| 0x80;   /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p++= (uchar)((d[a]>>  6)& 0x3F)| 0x80;   /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uchar) (d[a]&       0x3F)| 0x80;   /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)

// last 2 bytes, UNUSED by utf ATM, but there be the code
      } else if(d[a]<= 0x03FFFFFF) {  //  5 bytes U-00200000�U-03FFFFFF:   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *p++= (uchar) (d[a]>> 24)       | 0xF8;   /// [BYTE 1]      >> 24= 000000xx 00000000 00000000 00000000 00000000  then header | f8 (11111000)
        *p++= (uchar)((d[a]>> 18)& 0x3f)| 0x80;   /// [BYTE 2] >> 18 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p++= (uchar)((d[a]>> 12)& 0x3f)| 0x80;   /// [BYTE 3] >> 12 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p++= (uchar)((d[a]>>  6)& 0x3f)| 0x80;   /// [BYTE 4] >>  6 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uchar) (d[a]&       0x3f)| 0x80;   /// [BYTE 5]       & 3f= 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(d[a]<= 0x7FFFFFFF) {  //  6 bytes U-04000000�U-7FFFFFFF:   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *p++= (uchar) (d[a]>> 30)       | 0xFC;   /// [BYTE 1]      >> 30= 0000000x 00000000 00000000 00000000 00000000 00000000  then header | fc (11111100)
        *p++= (uchar)((d[a]>> 24)& 0x3f)| 0x80;   /// [BYTE 2] >> 24 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000 00000000  then header | 80 (10000000)
        *p++= (uchar)((d[a]>> 18)& 0x3f)| 0x80;   /// [BYTE 3] >> 18 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p++= (uchar)((d[a]>> 12)& 0x3f)| 0x80;   /// [BYTE 4] >> 12 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p++= (uchar)((d[a]>>  6)& 0x3f)| 0x80;   /// [BYTE 5] >>  6 & 3f= 00000000 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uchar) (d[a]&       0x3f)| 0x80;   /// [BYTE 6]         3f= 00000000 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      }
    *p= 0;                                        // string32 terminator
  } /// if string8 was modified

  return d8;
}

// windows compatibility
ushort *string32::convert16() {
  if(modif16) {
    modif16= false;
    if(!len) return null;
/// alloc mem
    if(d16) delete[] d16;
    d16= new ushort[len+ 1];
/// copy
    for(size_t a= 0; a< len+ 1; a++)
      d16[a]= (ushort)d[a];
  }
  return d16;
}


///------------------------///
// OPERATOR - / OPERATOR -= // clears n chars from strings
///------------------------///


string32 &string32::operator-=(int n) {
/// basic checks
  if(!n) return *this;
  modif8= modif16= true;
  if(len- n<= 0) {
    delData();
    return *this;
  }
/// create the new string, n less chars
  ulong *p= new ulong[len- n+ 1];
/// copy from old string
  size_t a;
  for(a= 0; a< len- n; a++)
    p[a]= d[a];
  p[a]= 0;                              /// terminator

  delete[] d;
  d= p;
  len-= n;
  return *this;
}


string32 string32::operator-(int n) const {
/// basic checks
  if(!n) return string32(*this);
  if(len- n<= 0) return string32();
/// create return string, n chars less
  string32 ret;
  ret.d= new ulong[len- n+ 1];
/// copy from *this
  size_t a;
  for(a= 0; a< len- n; a++)
    ret.d[a]= d[a];
  ret.d[a]= 0;                        /// terminator
  ret.len-= n;
  return ret;
}



///-----------///
// OPERATOR == //
///-----------///

bool string32::operator==(const string32 &s) const {
  if(s.len!= len) return false;
  for(size_t a= 0; a< len; a++)
    if(d[a]!=s.d[a])
      return false;

  return true; // this point reached -> there is no difference between strings
}


bool string32::operator==(culong *s) const {
  if(!s) {
    if(!d)  return true;
    else    return false;
  }

  culong *p1= d;
  culong *p2= s;

  while(*p1)                      // while there are characters in string 1
    if((!*p2) || (*p1++!= *p2++)) // if string 2 ends || characters differ (increment pointers too)
      return false;

  if(*p2) return false;           // there are still chars in string 2 when string 1 ended ?

  return true;                    // <- reached this point, strings match
}


bool string32::operator==(cushort *s) const {
  if(!s) {
    if(!d)  return true;
    else    return false;
  }

  culong *p1= d;
  cushort *p2= s;

  while(*p1)
    if( (!*p2) || (*p1++!= *p2++))
      return false;

  if(*p2) return false;

  return true;
}


bool string32::operator==(culong c) const {
  if(len!= 1) return false;
  if(d[0]!= c) return false;
  return true;
}








///-----------------------------------------///
// COMBining diacritical character functions //
///-----------------------------------------///

bool string32::isComb(culong c) const {
/// 0300-036F Combining Diacritical Marks, since version 1.0, with modifications in subsequent versions down to 4.1
  if( (c>= 0x0300)&& (c<= 0x036f) ) return true;
/// 1DC0-1DFF Combining Diacritical Marks Supplement, versions 4.1 to 5.2
  if( (c>= 0x1dc0)&& (c<= 0x1dff) ) return true;
/// 20D0-20FF Combining Diacritical Marks for Symbols, since version 1.0, with modifications in subsequent versions down to 5.1
  if( (c>= 0x20d0)&& (c<= 0x20ff) ) return true;
/// FE20-FE2F Combining Half Marks, versions 1.0, updates in 5.2
  if( (c>= 0xfe20)&& (c<= 0xfe2f) ) return true;
  return false;
}


void string32::clearComb() {
  if(!len) return;
  modif8= modif16= true;
///count comb's
  size_t n= 0;
  for(size_t a= 0; a< len; a++)
    if(isComb(d[a]))
      n++;

/// if found combining diacriticals, recreate the string32 without them
  if(n) {
    ulong *t= new ulong[len- n+ 1];
    for(size_t a= 0, b= 0; a< len; a++)
      if(!isComb(d[a]))
        t[b++]= d[a];
/// update internals
    delete[] d;
    d= t;
    len-= n;
  }
}










///----------------------------------------------///
// UTILITY FUNCTIONS, usually good for any string //
///----------------------------------------------///

void string32::updateLen() {
  if(!d) { len= 0; return; }
  modif8= modif16= true;
  ulong *p= d;
  while(*p++);
  len=  p- d- 1;
}

/// returns string length (with all combined chars) for a 32bit string
size_t string32::strlen(const ulong *s) const {
  const ulong *p= s;
  while(*p++);
  return p- s- 1;
}

/// returns string length (with all combined chars) for a 8bit string
size_t string32::strlen8(const char *s) const {
  const char *p= s;
  while(*p++);
  return p- s- 1;
}


/// returns string length for a 16bit string (windows compatibility) NOT UTF-16
size_t string32::strlen16(const ushort *s) const {
  const ushort *p= s;
  while(*p++);
  return p- s- 1;
}

// counts the number of characters in a utf-8 string (totals - both chars and combs)
size_t string32::strchars8(const char *s) const {
  const uchar *p= (const uchar *)s;
  size_t l= 0;

  while(*p++)
    if((*p& 0xc0)!= 0x80)   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      l++;

  return l;
}

// counts the number of comb caracters in a utf-8 string
size_t string32::strcombs8(cchar *s) const {        // <---------------------------------------------------------------------
  size_t ret= 0;
  cuchar *p= (cuchar *)s;
  ulong a;

  while(1) {                           /// while till terminator found
/// character is ascii 0-127
    if(*p < 128) {
      a= *p++;
/// character has 2 bytes
    } else if((*p& 0xe0) == 0xc0) {      /// test if first 3 bits (0xe0= 11100000) are 110xxxxx (0xc0= 11000000)
      a=  *p++ & 0x1f; a<<=6;
      a+= *p++ & 0x3f;
/// character has 3 bytes
    } else if((*p& 0xf0) == 0xe0) {      /// test if first 4 bits (0xf0= 11110000) are 1110xxxx (0xe0= 11100000)
      a=  *p++ & 0x0f; a<<= 6;
      a+= *p++ & 0x3f; a<<= 6;
      a+= *p++ & 0x3f;
/// character has 4 bytes
    } else if((*p& 0xf8) == 0xf0) {      /// test if first 5 bits (0xf8= 11111000) are 11110xxx (0xf0= 11110000)
      a=  *p++ & 0x07; a<<= 6;
      a+= *p++ & 0x3f; a<<= 6;
      a+= *p++ & 0x3f; a<<= 6;
      a+= *p++ & 0x3f;
// the last 2 bytes are not used, but avaible if in the future unicode will expand
/// character has 5 bytes
    } else if((*p& 0xfc) == 0xf8) {      /// test if first 6 bits (0xfc= 11111100) are 111110xx (0xf8= 11111000)
      a=  *p++ & 0x03;
      for(short b= 0; b< 4; b++)
        { a<<= 6; a+= *p++ & 0x3f; }
/// character has 6 bytes
    } else if((*p& 0xfe) == 0xfc) {      /// test if first 7 bits (0xfe= 11111110) are 1111110x (0xfc= 11111100)
      a=  *p++ & 0x01;
      for(short b= 0; b< 5; b++)
        { a<<= 6; a+= *p++ & 0x3f; }
    }
    if(!a) break;
    if(isComb(a))
      ret++;
    a= 0;
  }	/// while all characters
  return ret;
}


ulong string32::utf8to32(const char *s) const {
  cuchar *p= (cuchar*)s;
  ulong ret= 0;

/// unpack character and return it
  if(*p < 128) {
    ret= *p++;
  } else if((*p& 0xe0) == 0xc0) {/// character has 2 bytes
    ret=  *p++ & 0x1f; ret<<=6;
    ret+= *p++ & 0x3f;
  } else if((*p& 0xf0) == 0xe0) {/// character has 3 bytes
    ret=  *p++ & 0x0f; ret<<= 6;
    ret+= *p++ & 0x3f; ret<<= 6;
    ret+= *p++ & 0x3f;
  } else if((*p& 0xf8) == 0xf0) {/// character has 4 bytes
    ret=  *p++ & 0x07; ret<<= 6;
    ret+= *p++ & 0x3f; ret<<= 6;
    ret+= *p++ & 0x3f; ret<<= 6;
    ret+= *p++ & 0x3f;
// the last 2 bytes are not used, but avaible if in the future unicode will expand
  } else if((*p& 0xfc) == 0xf8) {/// character has 5 bytes
    ret=  *p++ & 0x03;
    for(short b= 0; b< 4; b++)
      { ret<<= 6; ret+= *p++ & 0x3f; }
  } else if((*p& 0xfe) == 0xfc) {/// character has 6 bytes
    ret=  *p++ & 0x01;
    for(short b= 0; b< 5; b++)
      { ret<<= 6; ret+= *p++ & 0x3f; }
  }
  return ret;
}


/// gets n-th char in string s (format is utf-8)
ulong string32::utf8to32n(const char *s, int n) const {
  cuchar *p= (cuchar*)s;
/// pass thru all characters, till n is reached
  for(int a= 0; a< n;)
    if((*p++ & 0xc0)!= 0x80)
      a++;

  return utf8to32((const char*)(p- 1));
}






///--------------------------------///
// UTF-8 SECURITY / READ FROM FILES // (any read from a file should be considered UNSAFE)
///--------------------------------///


// bad characters will be MARKED with 0xFFFD

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
string32 &string32::secureUTF8(const char *s) {
  delData();
  modif8= modif16= true;
/// length
  const uchar *p= (const uchar*)s;
  len= 0;

// can't assume that every character is ok, there can be garbage @ every level
  while(*p++) {
    if(*p < 128) len++;                   /// valid char if less than 128
/// search for headers, everything else is ignored.
    else if((*p& 0xe0) == 0xc0) len++;    /// header for 2 chars
    else if((*p& 0xf0) == 0xe0) len++;    /// header for 3 chars
    else if((*p& 0xf8) == 0xf0) len++;    /// header for 4 chars
    else if((*p& 0xfc) == 0xf8) len++;    /// header for 5 chars  marked as bad
    else if((*p& 0xfe) == 0xfc) len++;    /// header for 6 chars  marked as bad
  }

// copy
  d= new ulong[len+ 1];
  p= (const uchar*)s;
  size_t a= 0;
  while(a< len) {                       /// for each (valid) character
/// character is ascii 0-127
    if(*p < 128) {                      /// 1 byte characters are safe
      d[a++]= *p++;
      continue;
/// character has 2 bytes
    } else if((*p& 0xe0) == 0xc0) {     /// header for 2 bytes? (0xe0= 11100000)
    // [BYTE 1]
                     // test for overlong bytes 1100000x (10xxxxxx)
      if((*p& 0x1e) == 0) {             /// 1e= 00011110 if these 4 bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        continue;
      }
      d[a]= *p++& 0x1f;                 /// byte seems ok - copy from it (0x1f= 00011111)

    // [BYTE 2]
      if((*p& 0xc0)!= 0x80) {           /// byte 2 has to be a continuation byte (start with 10xxxxxx)
        d[a++]= 0xFFFD;
        continue;                       /// don't increase p
      }
      d[a]<<=6; d[a]+= *p++ & 0x3f;     /// byte seems ok - copy from it (0x3f= 00111111)

/// character has 3 bytes
  } else if((*p& 0xf0) == 0xe0) {       /// header for 3 bytes ? (0xf0= 11110000)
    // [BYTE 1]
                     // test for overlong bytes 11100000 100xxxxx (10xxxxxx)
      if(     ((*p& 0x0f) == 0) &&      ///  f= 00001111
         ((*(p+ 1)& 0x20) == 0)) {      /// 20=          00100000 if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        continue;
      }
      d[a]= *p++& 0x0f;                 /// byte seems ok - copy from it (0x0f= 00001111)

    // [BYTE 2-3]
      for(short b= 0; b< 2; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-3 have to be continuation bytes (start with 10xxxxxx)
          d[a++]= 0xFFFD;
          goto cont;                    /// don't increase p
        }
        d[a]<<= 6; d[a]+= *p++& 0x3f;   /// byte seems ok - copy from it (0x3f= 00111111)
      }
/// character has 4 bytes
    } else if((*p& 0xf8) == 0xf0) {     /// header for 4 bytes ? (0xf8= 11111000)
    // [BYTE 1]
                     // test for overlong bytes 11110000 1000xxxx (10xxxxxx 10xxxxxx)
      if(      ((*p& 0x7) == 0) &&      ///  7= 00000111
         ((*(p+ 1)& 0x30) == 0)) {      /// 30=          00110000 if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        continue;
      }
      d[a]= *p++& 0x07;                 /// byte seems ok - copy from it (0x07= 00000111)

    // [BYTE 2-4]
      for(short b= 0; b< 3; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-4 have to be continuation bytes (start with 10xxxxxx)
          d[a++]= 0xFFFD;
          goto cont;                    /// don't increase p
        }
        d[a]<<= 6; d[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
// the last 2 bytes are not used, but avaible if in the future unicode will expand
/// character has 5 bytes
    } else if((*p& 0xfc) == 0xf8) {     /// header for 5 bytes ? (0xfc= 11111100)
      d[a++]= 0xFFFD;
      p++;
      continue;

/* >>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 5 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // [BYTE 1]
                     // test for overlong bytes 11111000 10000xxx (10xxxxxx 10xxxxxx 10xxxxxx)
      if(     (*p& 0x3  == 0) &&        ///  3= 00000011
         (*(p+ 1)& 0x38 == 0)) {        /// 38=          00111000 if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        continue;
      }
      d[a]= *p++& 0x03;                 /// byte seems ok - copy from it (0x03= 00000011)

    // [BYTE 2-5]
      for(short b= 0; b< 4; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-5 have to be continuation bytes (start with 10xxxxxx)
          d[a++]= 0xFFFD;
          goto cont;                    /// don't increase p
        }
        d[a]<<= 6; d[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
*/

/// character has 6 bytes
    } else if((*p& 0xfe) == 0xfc) {              /// header for 6 bytes ? (0xfe= 11111110)
      d[a++]= 0xFFFD;
      p++;
      continue;

/* >>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 6 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // [BYTE 1]
                     // test for overlong bytes 11111100 100000xx (10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx)
      if(     (*p& 0x1  == 0) &&        ///  1= 00000001
         (*(p+ 1)& 0x3C == 0)) {        /// 3C=          00111100 if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        continue;
      }
      d[a]= *p++& 0x01;                 /// byte seems ok - copy from it (0x01= 00000001)

    // [BYTE 2-6]
      for(short b= 0; b< 5; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-6 have to be continuation bytes (start with 10xxxxxx)
          d[a++]= 0xFFFD;
          goto cont;                    /// don't increase p
        }
        d[a]<<= 6; d[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
*/
    } else {
      p++;                         // character unreadable
      continue;
    }

  /// test result character d[a]
    if(d[a]>= 0x10FFFF)                 // limit of Unicode <--------------------- (maybe this changes in the future)
      d[a]= 0xFFFD;

    if( (d[a]>= 0xD800) && (d[a]<= 0xDFFF) )  /// it can't be a utf-16 surrogate
      d[a]= 0xFFFD;

    // further tests can be done here. there are still some chars that should be marked as malformed (0xFFFD) <<<=========================


    a++;
cont:
    ;
  }	/// for each character

  d[a]= 0;                              /// terminator

  return *this;
}






// fopen knows of utf-8 but the win version wants to put a fukin BOM in the file, wich cause problems in linux, so file must be opened as pure binary

/// read all file                   (validates each char)
void string32::readUTF8(FILE *f) {
/// read / ignore the BOM in an UTF file
  ushort bom;
  uint pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, (long)pos, SEEK_SET);  /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);          /// maybe it read 1 byte? just go back anyways

/// determine remaining filesize in bytes (maybe it is not reading from the start)
	uint fs;
	fseek(f, 0L, SEEK_END);
	fs= ftell(f);
	fseek(f, (long)pos, SEEK_SET);
  fs-= pos;

  if(!fs)
    return;

/// read all file
  char *buffer= new char[fs+ 1];
  fread(buffer, 1, fs, f);
  buffer[fs]= 0;                      /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}



/// read n characters from file     (validates each char)
void string32::readUTF8n(FILE *f, size_t n) {
/// read / ignore the BOM in an UTF file
  ushort bom;
  uint pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, (long)pos, SEEK_SET);  /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);          /// maybe it read 1 byte? just go back anyways

/// determine remaining filesize in bytes (maybe it is not reading from the start)
	uint fs;
	fseek(f, 0L, SEEK_END);
	fs= ftell(f);
	fseek(f, (long)pos, SEEK_SET);
  fs-= pos;

  if(!fs)                             /// remaining filesize is 0 ?
    return;

  if(n> fs)                           /// it will read what it can
    n= fs;

/// read n bytes from file
  char *buffer= new char[n+ 1];
  fread(buffer, 1, n, f);
  buffer[n]= 0;                       /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}

/// read till end of line (or file) (validates each char)
void string32::readLineUTF8(FILE *f) {
/// read / ignore the BOM in an UTF file
  ushort bom;
  uint pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, (long)pos, SEEK_SET);  /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);          /// maybe it read 1 byte? just go back anyways

/// compute line length
  uint len;
  char r;

  while(fread(&r, 1, 1, f))
    if(r == '\n')
      break;

  len= ftell(f);
  len-= pos;
  if(!len) return;

/// read line from file
  char *buffer= new char[len+ 1];
  fseek(f, pos, SEEK_SET);
  fread(buffer, 1, len, f);
  buffer[len]= 0;                     /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}







