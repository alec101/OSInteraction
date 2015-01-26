#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <stdio.h>
#include "typeShortcuts.h"
#include "strCommon.h"
#include "str32.h"


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

   Any overlong UTF-8 sequence could be abused to bypass UTF-8 substr32 tests
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
*/


using namespace Str;


str32::str32():
d(null), len(0), nrChars(0), nrCombs(0), d8(null), dWin(null), modif8(false), modifWin(false)
{}

str32::~str32() {
  delData();
}

void str32::delData() {
  if(d)    { delete[] d;    d= null; }
  // d8 & dWin must be deallocated only on destructor & convert8 & convertWin

  len= nrChars= nrCombs= 0;
  modif8= modifWin= false;
}

void str32::clean() {
  if(d8)   { delete[] d8;   d8= null;  }
  if(dWin) { delete[] dWin; dWin= null; }
}



// OPERATOR= //
///---------///
str32 &str32::operator=(const str32 &s) {
  modifWin= modif8= true;
  len=     s.len;
  nrChars= s.nrChars;
  nrCombs= s.nrCombs;

  /// copy
  if(d) delete[] d;
  int32 n= nrChars+ nrCombs;
  d= new uint32[n+ 1];
  for(int32 a= 0; a<= n; a++)
    d[a]= s.d[a];

  return *this;
}


str32 &str32::operator=(cuint32 *s) {
  delData();
  modif8= modifWin= true;

  /// string [s] length in unicode values
  cuint32 *p= s;
  while(*p++);
  int32 n= (int32)(p- s- 1);
  len= n* 4;
  nrChars= nrCombs= 0;
  /// copy
  if(d) delete[] d;
  d= new uint32[n+ 1];
  for(int32 a= 0; a<= n; a++) {
    d[a]= s[a];
    if(isComb(d[a])) nrCombs++;
    else             nrChars++;
  }

  return *this;
}


str32 &str32::operator=(cuint16 *s) {
  delData();
  modif8= modifWin= true;

  /// string [s] length in unicode values (16bit tho, windows can't do anymore than that... it's handicapped... let's not take on the disabled ppl that did it)
  cuint16 *p= s;
  while(*p++);
  int32 n= (int32)(p- s- 1);

  len= n* 4;

  /// copy
  if(d) delete[] d;
  d= new uint32[n+ 1];
  for(int32 a= 0; a<= n; a++) {
    d[a]= s[a];
    if(isComb(d[a])) nrCombs++;
    else             nrChars++;
  }

  return *this;
}


str32 &str32::operator=(cuint8 *s) {
  delData();
  modif8= modifWin= true;

  /// length
  cuint8 *p= s;
  int32 n= 0;

  while(*p++)
    if((*p& 0xc0)!= 0x80)   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      n++;

  len= n* 4;

  // copy
  d= new uint32[n+ 1];
  p= s;

  for(int32 a= 0; a<= n; a++) {  /// for each character
    /// character is ascii 0-127
    if(*p < 128) {
      d[a]= *p++;
      continue;
    }
    /// character has 2 bytes
    else if((*p& 0xe0) == 0xc0) {/// test if first 3 bits (0xe0= 11100000) are 110xxxxx (0xc0= 11000000)
      d[a]= (*p++)& 0x1f;                 /// last 5 bits (0x1f= 00011111)
      d[a]<<=6; d[a]+= (*p++)& 0x3f;      /// last 6 bits (0x3f= 00111111)
    }
    /// character has 3 bytes
    else if((*p& 0xf0) == 0xe0) {/// test if first 4 bits (0xf0= 11110000) are 1110xxxx (0xe0= 11100000)
      d[a]= (*p++)& 0x0f;                 /// last 4 bits (0x0f= 00001111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
    }
    /// character has 4 bytes
    else if((*p& 0xf8) == 0xf0) {/// test if first 5 bits (0xf8= 11111000) are 11110xxx (0xf0= 11110000)
      d[a]= (*p++)& 0x07;                 /// last 3 bits (0x07= 00000111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
    }
    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    /// character has 5 bytes
    else if((*p& 0xfc) == 0xf8) {/// test if first 6 bits (0xfc= 11111100) are 111110xx (0xf8= 11111000)
      d[a]= (*p++)& 0x03;                 /// last 2 bits (0x03= 00000011)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
    }
    /// character has 6 bytes
    else if((*p& 0xfe) == 0xfc) {/// test if first 7 bits (0xfe= 11111110) are 1111110x (0xfc= 11111100)
      d[a]= (*p++)& 0x01;                 /// last 1 bit  (0x01= 00000001)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
      d[a]<<= 6; d[a]+= (*p++)& 0x3f;     /// last 6 bits (0x3f= 00111111)
    }

    if(isComb(d[a])) nrCombs++;
    else             nrChars++;
  }	/// for each character

  // MAKE d8 now? or only when conversions are needed? it just needs a copy from s
  return *this;
}


str32 &str32::operator=(uint32 c) {
  if(!c)
    if(d) {
      delData();
      return *this;
    }

  if(d)
    delete[] d;
  d= new uint32[2];
  d[0]= c;
  d[1]= 0;

  if(isComb(d[0])) nrCombs++;
  else             nrChars++;
  len= 4;

  modif8= modifWin= true;

  return *this;
}



// OPERATOR += //
///-----------///

// this one is heavily used
str32 &str32::operator+=(const str32 &s) {
  if(!s.len) return *this;                /// other str32 empty -> return *this
  modif8= modifWin= true;
  if(!len) return *this= s;               /// current str32 empty -> just copy other str32

  int32 a, n;
  uint32 *p= new uint32[nrChars+ nrCombs+ s.nrChars+ s.nrCombs+ 1]; /// new str32 allocation

  /// copy string 1 to new string
  for(a= 0, n= nrChars+ nrCombs; a< n; a++)
    p[a]= d[a];
  delete[] d;
  d= p;

  /// copy string 2 to new string
  p+= a;
  for(a= 0, n= s.nrChars+ s.nrCombs; a<= n; a++)    /// +1= terminator
    p[a]= s.d[a];

  len+= s.len;
  nrChars+= s.nrChars;
  nrCombs+= s.nrCombs;

  return *this;
}


str32 &str32::operator+=(cuint8 *s) {
  if(!s) return *this;
  modif8= modifWin= true;
  if(!len) return *this= s;

  return *this+= str32(s);
}


str32 &str32::operator+=(cuint16 *s) {
  if(!s) return *this;
  modif8= modifWin= true;
  if(!len) return *this= s;

  return *this+= str32(s);
}


str32 &str32::operator+=(cuint32 *s) {
  if(!s) return *this;
  modif8= modifWin= true;
  if(!nrChars) return *this= s;

  return *this+= str32(s);
}


str32 &str32::operator+=(uint32 c) {
  if(!c) return *this;
  modif8= modifWin= true;
  if(!nrChars) return *this= c;

  /// create the new string
  uint32 *t= new uint32[nrChars+ nrCombs+ 2];

  int32 a= 0;
  for(int32 n= nrChars+ nrCombs; a< n; a++)
    t[a]= d[a];
  t[a]= c;
  t[a+ 1]= 0;

  /// delete old+ assign d to the new string
  delete[] d;
  d= t;
  len+= 4;
  if(isComb(c)) nrCombs++; else nrChars++;

  return *this;
}





// CONVERSION operators //
///--------------------///



// UTF-8 conversion from UTF-32 or basically an array of uint32s (internal format)
void *str32::convert8() {
  if(modif8) {
    modif8= false;
    if(!len) return null;

    /// length of d8
    int32 l= 0;
    int32 n= nrChars+ nrCombs;

    for(int32 a= 0; a< n; a++) // for each character in d
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
    d8= new uint8[l+ 1];
    uint8 *p= d8;

    for(int32 a= 0; a< n; a++)     // for each character in d
      if(d[a]<= 0x0000007F) {         //  1 byte   U-00000000�U-0000007F:  0xxxxxxx
        *p++= (uint8)d[a];
      } else if(d[a]<= 0x000007FF) {  //  2 bytes  U-00000080�U-000007FF:  110xxxxx 10xxxxxx
        *p++= (uint8)(d[a]>> 6)         | 0xC0;   /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
        *p++= (uint8)(d[a]&        0x3f)| 0x80;   /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(d[a]<= 0x0000FFFF) {  //  3 bytes  U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *p++= (uint8) (d[a]>> 12)       | 0xE0;   /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
        *p++= (uint8)((d[a]>> 6)&  0x3F)| 0x80;   /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uint8) (d[a]&       0x3F)| 0x80;   /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(d[a]<= 0x001FFFFF) {	//  4 bytes U-00010000�U-001FFFFF:   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *p++= (uint8) (d[a]>> 18)       | 0xF0;   /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
        *p++= (uint8)((d[a]>> 12)& 0x3F)| 0x80;   /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p++= (uint8)((d[a]>>  6)& 0x3F)| 0x80;   /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uint8) (d[a]&       0x3F)| 0x80;   /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)

      // last 2 bytes, UNUSED by utf ATM, but there be the code
      } else if(d[a]<= 0x03FFFFFF) {  //  5 bytes U-00200000�U-03FFFFFF:   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *p++= (uint8) (d[a]>> 24)       | 0xF8;   /// [BYTE 1]      >> 24= 000000xx 00000000 00000000 00000000 00000000  then header | f8 (11111000)
        *p++= (uint8)((d[a]>> 18)& 0x3f)| 0x80;   /// [BYTE 2] >> 18 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p++= (uint8)((d[a]>> 12)& 0x3f)| 0x80;   /// [BYTE 3] >> 12 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p++= (uint8)((d[a]>>  6)& 0x3f)| 0x80;   /// [BYTE 4] >>  6 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uint8) (d[a]&       0x3f)| 0x80;   /// [BYTE 5]       & 3f= 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(d[a]<= 0x7FFFFFFF) {  //  6 bytes U-04000000�U-7FFFFFFF:   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *p++= (uint8) (d[a]>> 30)       | 0xFC;   /// [BYTE 1]      >> 30= 0000000x 00000000 00000000 00000000 00000000 00000000  then header | fc (11111100)
        *p++= (uint8)((d[a]>> 24)& 0x3f)| 0x80;   /// [BYTE 2] >> 24 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000 00000000  then header | 80 (10000000)
        *p++= (uint8)((d[a]>> 18)& 0x3f)| 0x80;   /// [BYTE 3] >> 18 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p++= (uint8)((d[a]>> 12)& 0x3f)| 0x80;   /// [BYTE 4] >> 12 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p++= (uint8)((d[a]>>  6)& 0x3f)| 0x80;   /// [BYTE 5] >>  6 & 3f= 00000000 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p++= (uint8) (d[a]&       0x3f)| 0x80;   /// [BYTE 6]         3f= 00000000 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      }
    *p= 0;                                        // str32 terminator
  } /// if string8 was modified

  return d8;
}

/// UTF-32 to windows wide char (16bit)
uint16 *str32::convertWin() {
  if(modifWin) {
    modifWin= false;
    if(!len) return null;
    /// alloc mem
    if(dWin) delete[] dWin;
    dWin= new uint16[nrChars+ nrCombs+ 1];
    /// copy
    for(int32 a= 0, n= nrChars+ nrCombs; a<= n; a++)
      dWin[a]= (uint16)d[a];
  }
  return dWin;
}

// whole string conversion to lowercase
void str32::lower() {
  for(int32 a= 0, n= nrChars+ nrCombs; a< n; a++)
    d[a]= tolower(d[a]);      /// lowercase each character
}


// whole string conversion to uppercase
void str32::upper() {
  for(int32 a= 0, n= nrChars+ nrCombs; a< n; a++)
    d[a]= toupper(d[a]);      /// lowercase each character
}

///------------------------///
// OPERATOR - / OPERATOR -= // clears n chars from strings
///------------------------///


str32 &str32::operator-=(int n) {
  /// basic checks
  if(!n) return *this;
  modif8= modifWin= true;
  if(nrChars- n<= 0) { delData(); return *this; }

  /// cut the string by placing a terminator where it should end
  uint32 *p= getChar(nrChars- n);
  *p= 0;

  /// p will hold the old string
  p= d;
  d= null;        /// making d null, will trick the next line not to delete[] the array buffer

  *this= p;        // copy from old string array
  delete[] p;     /// no longer needed

  return *this;
}


str32 str32::operator-(int n) const {
  /// basic checks
  if(!n) return str32(*this);
  if(nrChars- n<= 0) return str32();

  /// temporary cut the string
  uint32 *p= getChar(nrChars- n);
  uint32 x= *p;                   /// x will hold the value where the cut was made
  *p= 0;                          /// cut by placing a terminator

  str32 ret(d);                   /// return string is created with cut string
  *p= x;                          /// restore *this

  return ret;
}



///-----------///
// OPERATOR == //
///-----------///

bool str32::operator==(const str32 &s) const {
  if((s.nrChars!= nrChars) || (s.nrCombs!= nrCombs))
    return false;

  for(int32 a= 0, n= nrChars+ nrCombs; a< n; a++)
    if(d[a]!=s.d[a])
      return false;

  return true; // this point reached -> there is no difference between strings
}


bool str32::operator==(cuint32 *s) const {
  if(!s) {
    if(!d)  return true;
    else    return false;
  }

  cuint32 *p1= d;
  cuint32 *p2= s;

  while(*p1)                      // while there are characters in string 1
    if((!*p2) || (*p1++!= *p2++)) // if string 2 ends || characters differ (increment pointers too)
      return false;

  if(*p2) return false;           // there are still chars in string 2 when string 1 ended ?

  return true;                    // <- reached this point, strings match
}


bool str32::operator==(cuint16 *s) const {
  if(!s) {
    if(!d)  return true;
    else    return false;
  }

  cuint32 *p1= d;
  cuint16 *p2= s;

  while(*p1)
    if( (!*p2) || (*p1++!= *p2++))
      return false;

  if(*p2) return false;

  return true;
}


bool str32::operator==(uint32 c) const {
  if(nrChars!= 1) return false;
  return (d[0]== c? true: false);
}








///-----------------------------------------///
// COMBining diacritical character functions //
///-----------------------------------------///



void str32::clearComb() {
  if(!nrCombs) return;
  if(!nrChars) { delData(); return; } /// there can be only some combs in this string, so del everything

  modif8= modifWin= true;

  /// create a new internal data, without any combs
  uint32 *t= new uint32[nrChars+ 1];
  for(int32 a= 0, b= 0, n= nrChars+ nrCombs+ 1; a< n; a++)   /// +1 to copy terminator
    if(!isComb(d[a]))
      t[b++]= d[a];
  
  /// new internal vals
  nrCombs= 0;
  len= nrChars* 4;
  delete[] d;
  d= t;
}










///----------------------------------------------///
// UTILITY FUNCTIONS, usually good for any string //
///----------------------------------------------///

void str32::updateLen() {
  if(!d) { len= 0; nrChars= 0; nrCombs= 0; return; }
  modif8= modifWin= true;

  uint32 *p= d;

  while(*p++)
    if(isComb(*p)) nrCombs++;
    else           nrChars++;

  len= (nrChars+ nrCombs)* 4;
}



///--------------------------------///
// UTF-8 SECURITY / READ FROM FILES // (any read from a file should be considered UNSAFE)
///--------------------------------///


// bad characters will be MARKED with 0xFFFD

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
str32 &str32::secureUTF8(cvoid *s) {
  delData();
  modif8= modifWin= true;

  /// length
  cuint8 *p= (cuint8 *)s;

  // can't assume that every character is ok, there can be garbage @ every level
  int32 n= 0;                                /// n will hold the number of (valid) unicode values (chars+ combs)
  while(*p++) {
    if(*p < 128) n++;                   /// valid char if less than 128
    /// search for headers, everything else is ignored.
    else if((*p& 0xe0) == 0xc0) n++;    /// header for 2 bytes
    else if((*p& 0xf0) == 0xe0) n++;    /// header for 3 bytes
    else if((*p& 0xf8) == 0xf0) n++;    /// header for 4 bytes
    else if((*p& 0xfc) == 0xf8) n++;    /// header for 5 bytes  marked as bad
    else if((*p& 0xfe) == 0xfc) n++;    /// header for 6 bytes  marked as bad
  }
  len= n* 4;

  // copy
  d= new uint32[n+ 1];
  p= (uint8 *)s;
  int32 a= 0;
  while(a< n) {                         /// for each (valid) unicode value
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
          continue;                     /// don't increase p
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
          continue;                     /// don't increase p
        }
        d[a]<<= 6; d[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    /// character has 5 bytes
    } else if((*p& 0xfc) == 0xf8) {     /// header for 5 bytes ? (0xfc= 11111100)
      // comment next 3 lines to enable 5byte chars
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
          continue;                     /// don't increase p
        }
        d[a]<<= 6; d[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
*/

    /// character has 6 bytes
    } else if((*p& 0xfe) == 0xfc) {              /// header for 6 bytes ? (0xfe= 11111110)
      // comment next 3 lines to enable 6byte chars
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
          continue;                     /// don't increase p
        }
        d[a]<<= 6; d[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
*/
    } else {
      p++;                              // character unreadable
      continue;
    }

    /// test result character d[a]
    if(d[a]>= 0x10FFFF)                 // limit of Unicode <--------------------- (maybe this changes in the future)
      d[a]= 0xFFFD;

    if( (d[a]>= 0xD800) && (d[a]<= 0xDFFF) )  /// it can't be a utf-16 surrogate
      d[a]= 0xFFFD;

    // further tests can be done here. there are still some chars that should be marked as malformed (0xFFFD) <<<=========================


    a++;
  }	/// for each character

  d[a]= 0;                              /// terminator

  return *this;
}






// fopen knows of utf-8 but the win version wants to put a freakin BOM in the file, wich cause problems in linux, so file must be opened as binary

/// read all file                   (validates each char)
void str32::readUTF8(FILE *f) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, pos, SEEK_SET);    /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);      /// maybe it read 1 byte? just go back anyways

  /// determine remaining filesize in bytes (maybe it is not reading from the start)
	long fs;
	fseek(f, 0L, SEEK_END);
	fs= ftell(f);
	fseek(f, pos, SEEK_SET);
  fs-= pos;

  if(!fs)
    return;

  /// read all file
  int8 *buffer= new int8[fs+ 1];
  fread(buffer, 1, fs, f);
  buffer[fs]= 0;                  /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}



/// read n characters from file     (validates each char)
void str32::readUTF8n(FILE *f, size_t n) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, pos, SEEK_SET);    /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);      /// maybe it read 1 byte? just go back anyways

  /// determine remaining filesize in bytes (maybe it is not reading from the start)
	size_t fs;
	fseek(f, 0L, SEEK_END);
	fs= ftell(f);
	fseek(f, pos, SEEK_SET);
  fs-= pos;

  if(!fs)                         /// remaining filesize is 0 ?
    return;

  if(n> fs)                       /// it will read what it can
    n= fs;

  /// read n bytes from file
  int8 *buffer= new int8[n+ 1];
  fread(buffer, 1, n, f);
  buffer[n]= 0;                   /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}

/// read till end of line (or file) (validates each char)
void str32::readLineUTF8(FILE *f) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, pos, SEEK_SET);  /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);          /// maybe it read 1 byte? just go back anyways

  /// compute line length
  long l;
  int8 r;

  while(fread(&r, 1, 1, f))
    if(r == '\n')
      break;

  l= ftell(f);
  l-= pos;
  if(!l) return;

  /// read line from file
  int8 *buffer= new int8[l+ 1];
  fseek(f, pos, SEEK_SET);
  fread(buffer, 1, l, f);
  buffer[l]= 0;                       /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}









