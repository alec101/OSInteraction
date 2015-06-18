#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <stdio.h>
#include <stdarg.h>
#include "util/typeShortcuts.h"
#include "util/strCommon.h"
#include "util/str8.h"

// lower+upper can be done for chars that expand into multiple chars and vice-versa, easily
//       (2 funcs to check for these few cases, when counting how big the str is)
// there's another case that i think another word change when lowercasing/uppercasing...
//       this... i don't think is done easily... actually very hard


/* UTF 8 research
  -they say to start a file with a ZERO WIDTH NOBREAK SPACE (U+FEFF), which is
     in this role also referred to as the �signature� or �byte-order mark (BOM)�
  -linux avoids BOM, so i don't think there's gonna be any testing using BOM;
     only to ignore a BOM if the file starts with it

  -The following byte sequences are used to represent a character.
 *  The sequence to be used depends on the Unicode number of the character: 
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
     accepted from a UTF-8 stream only in the form 0x0A, but not in any of the following
     five possible overlong forms: 
    0xC0 0x8A
    0xE0 0x80 0x8A
    0xF0 0x80 0x80 0x8A
    0xF8 0x80 0x80 0x80 0x8A
    0xFC 0x80 0x80 0x80 0x80 0x8A

   Any overlong UTF-8 sequence could be abused to bypass UTF-8 substring tests
      that look only for the shortest possible encoding.
   All overlong UTF-8 sequences start with one of the following byte patterns: 
    
    1100000x (10xxxxxx) 
    11100000 100xxxxx (10xxxxxx) 
    11110000 1000xxxx (10xxxxxx 10xxxxxx) 
    11111000 10000xxx (10xxxxxx 10xxxxxx 10xxxxxx) 
    11111100 100000xx (10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx) 
    
   Also note that the code positions U+D800 to U+DFFF (UTF-16 surrogates) as well
     as U+FFFE and U+FFFF must not occur in normal UTF-8 or UCS-4 data.
   UTF-8 decoders should treat them like malformed or overlong sequences for safety reasons.

   Markus Kuhn�s UTF-8 decoder stress test file contains a systematic collection
   of malformed and overlong UTF-8 sequences and will help you to verify
   the robustness of your decoder.
   http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

*/

using namespace Str;


/// character case change helper structs (initialized at the back of the file)
struct _L2U {
  uint32 l, u;
};// _cl2u[], _cu2l[];


void str8::delData() {
  if(d)    { delete[] d;    d= null; }
  // d32 & dWin must be deallocated only on destructor & convert32 & convertWin

  len= nrChars= nrCombs= 0;
}

void str8::clean() {
  if(dWin) { delete[] dWin; dWin= null; } 
  if(d32)  { delete[] d32;  d32= null; }
}


str8 operator+(cchar *s1, const str8 &s2) { return str8(s1)+= s2; }





uint8 fBuf[2048];

str8 &str8::f(cvoid *format, ...) {
	int32 l= strlen8(format);
	if(l> 2047) {
		*this= "TOO BIG, FORMAT FAILED";
		return *this;
	}
  /// create formatted text (use a buffer)
  fBuf[0]= '\0';
	va_list args;
  va_start(args, format);
  vsprintf((char *)fBuf, (cchar *)format, args);
  va_end(args);

	return *this= fBuf;
}






// OPERATOR= //
///---------///
str8 &str8::operator=(const str8 &s) {
  if(!s.len) { delData(); return *this; }

  if(len!= s.len) {
    len= s.len;
    if(d) delete[] d;
    d= new uint8[len];
  }

  nrChars= s.nrChars;
  nrCombs= s.nrCombs;
  
  /// copy
  for(int a= 0; a< len; a++)
    d[a]= s.d[a];

  return *this;
}


str8 &str8::operator=(cuint8 *s) {
  delData();
  if(!s) return *this;
  
  cuint8 *str= s;
  while(*str++) {
    len++;
    if((*str& 0xc0)!= 0x80) {    /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      if(isComb(utf8to32(str))) nrCombs++;
      else                      nrChars++;
    }
  }
  len++;

  /// alloc+ copy
  d= new uint8[len];

  int32 l= len;
  while(l) {
    l--;
    d[l]= s[l];                // d[--l] won't decrease s[l] too in clang :(
  }
  
  return *this;
}



str8 &str8::operator=(cuint32 *s) {
  delData();
  int32 n= strlen32(s)/ 4;              /// n will hold the number of unicode values in the string (chars+combs)
  if(!n) return *this;

  /// length in bytes of d (len is 0 cuz of delData())
  for(int32 a= 0; a< n; a++) {           // for each character in s
    if(s[a]<=      0x0000007F) len++;   /// 1 byte  U-00000000�U-0000007F:  0xxxxxxx 
    else if(s[a]<= 0x000007FF) len+= 2; /// 2 bytes U-00000080�U-000007FF:  110xxxxx 10xxxxxx 
    else if(s[a]<= 0x0000FFFF) len+= 3; /// 3 bytes U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
    else if(s[a]<= 0x001FFFFF) len+= 4; /// 4 bytes U-00010000�U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
    else if(s[a]<= 0x03FFFFFF) len+= 5; /// 5 bytes U-00200000�U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
    else if(s[a]<= 0x7FFFFFFF) len+= 6; /// 6 bytes U-04000000�U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
  }
  len++;              /// terminator;

  /// create d
  d= new uint8[len];
  uint8 *p= d;                      /// p will walk d

  for(int32 a= 0; a< n; a++) {      // for each char/comb in s
    /// check the unicode value - if it's a char or a comb
    if(isComb(s[a])) nrCombs++; else nrChars++;

    /// compress unicode value into utf-8
    if(s[a]<= 0x0000007F) {          //  1 byte   U-00000000�U-0000007F:  0xxxxxxx 
      *p++= (uint8) s[a];
    } else if(s[a]<= 0x000007FF) {   //  2 bytes  U-00000080�U-000007FF:  110xxxxx 10xxxxxx 
      *p++= (uint8) (s[a]>> 6)        | 0xC0;    /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
      *p++= (uint8) (s[a]&       0x3f)| 0x80;    /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(s[a]<= 0x0000FFFF) {   //  3 bytes  U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
      *p++= (uint8) (s[a]>> 12)       | 0xE0;    /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
      *p++= (uint8)((s[a]>> 6)&  0x3F)| 0x80;    /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p++= (uint8) (s[a]&       0x3F)| 0x80;    /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(s[a]<= 0x001FFFFF) {   // 4 bytes U-00010000�U-001FFFFF:    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
      *p++= (uint8) (s[a]>> 18)       | 0xF0;    /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
      *p++= (uint8)((s[a]>> 12)& 0x3F)| 0x80;    /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
      *p++= (uint8)((s[a]>>  6)& 0x3F)| 0x80;    /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p++= (uint8) (s[a]&       0x3F)| 0x80;    /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)

    // last 2 bytes, UNUSED by utf ATM, but there be the code
    } else if(s[a]<= 0x03FFFFFF) {   //  5 bytes U-00200000�U-03FFFFFF:   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
      *p++= (uint8) (s[a]>> 24)       | 0xF8;    /// [BYTE 1]      >> 24= 000000xx 00000000 00000000 00000000 00000000  then header | f8 (11111000)
      *p++= (uint8)((s[a]>> 18)& 0x3f)| 0x80;    /// [BYTE 2] >> 18 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
      *p++= (uint8)((s[a]>> 12)& 0x3f)| 0x80;    /// [BYTE 3] >> 12 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
      *p++= (uint8)((s[a]>>  6)& 0x3f)| 0x80;    /// [BYTE 4] >>  6 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p++= (uint8) (s[a]&       0x3f)| 0x80;    /// [BYTE 5]       & 3f= 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(s[a]<= 0x7FFFFFFF) {   //  6 bytes U-04000000�U-7FFFFFFF:   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      *p++= (uint8) (s[a]>> 30)       | 0xFC;    /// [BYTE 1]      >> 30= 0000000x 00000000 00000000 00000000 00000000 00000000  then header | fc (11111100)
      *p++= (uint8)((s[a]>> 24)& 0x3f)| 0x80;    /// [BYTE 2] >> 24 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000 00000000  then header | 80 (10000000)
      *p++= (uint8)((s[a]>> 18)& 0x3f)| 0x80;    /// [BYTE 3] >> 18 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
      *p++= (uint8)((s[a]>> 12)& 0x3f)| 0x80;    /// [BYTE 4] >> 12 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
      *p++= (uint8)((s[a]>>  6)& 0x3f)| 0x80;    /// [BYTE 5] >>  6 & 3f= 00000000 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p++= (uint8) (s[a]&       0x3f)| 0x80;    /// [BYTE 6]         3f= 00000000 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    }
  } /// for each char/comb in s
  *p= 0;                                        // string terminator

  return *this;
}


str8 &str8::operator=(uint32 c) {
  uint32 buf[2]= { c, 0 };
  return *this= buf;
}


// WINDOWS COMPATIBILITY
str8 &str8::operator=(cuint16 *s) {
  delData();

  int32 n= strlenWin(s)/ 2;
  if(!n) return *this;

  /// length in bytes of d
  for(int32 a= 0; a< n; a++)  // for each character in s
    if(s[a]<=      0x007F) len++;   /// 1 byte  U-00000000�U-0000007F:  0xxxxxxx 
    else if(s[a]<= 0x07FF) len+= 2; /// 2 bytes U-00000080�U-000007FF:  110xxxxx 10xxxxxx 
    else if(s[a]<= 0xFFFF) len+= 3; /// 3 bytes U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
                                     // windows wide char fits in max 3 bytes
  len++;                    /// terminator;

  /// create d
  d= new uint8[len];
  uint8 *p= d;

  for(int32 a= 0; a< n; a++) {    // for each character in s
    /// check the unicode value - if it's a char or a comb
    if(isComb(s[a])) nrCombs++;
    else             nrChars++;

    /// compress windows 16bit wide character into utf-8
    if(s[a]<= 0x007F) {            // 1 byte   U-00000000�U-0000007F:  0xxxxxxx 
      *p++= (uint8)  s[a];
    } else if(s[a]<= 0x07FF) {     // 2 bytes  U-00000080�U-000007FF:  110xxxxx 10xxxxxx 
      *p++= (uint8) (s[a]>> 6)        | 0xC0; /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
      *p++= (uint8) (s[a]&       0x3f)| 0x80; /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(s[a]<= 0xFFFF) {     // 3 bytes  U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
      *p++= (uint8) (s[a]>> 12)       | 0xE0; /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
      *p++= (uint8)((s[a]>> 6)&  0x3F)| 0x80; /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p++= (uint8) (s[a]&       0x3F)| 0x80; /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    }
  } /// for each char/comb in s
  *p= 0;                                    // string terminator

  return *this;
}





// OPERATOR += //
///-----------///

// this one is heavily used - concatenates current str and another str8
str8 &str8::operator+=(const str8 &s) {
  if(!s.len) return *this;              /// other str8 empty -> return *this
  if(!len) return *this= s;             /// current str8 empty -> just copy other str8
  
  uint8 *p= new uint8[len+ s.len- 1];   /// new str8 allocation
  
  /// copy string 1 to new string
  int a= 0;
  for(a= 0; a< len- 1; a++)             /// -1, to skip the terminator
    p[a]= d[a];

  delete[] d;
  d= p;
  /// copy string 2 to new string
  p+= a;
  for(a= 0; a< s.len; a++)
    *p++= s.d[a];

  /// string sizes 
  nrChars+= s.nrChars;
  nrCombs+= s.nrCombs;
  len+= s.len- 1;                     /// minus one of the terminators

  return *this;
}

/// concatenate current str and another utf-8 string
str8 &str8::operator+=(cuint8 *s) {
  if(!s) return *this;
  if(!len) return *this= s;

  str8 p= s;
  return *this+= p;
}

/// concatenates current string and another utf-32 string
str8 &str8::operator+=(cuint32 *s) {
  if(!s) return *this;
  if(!len) return *this= s;

  str8 p= s;
  return *this+= p;
}

/// add a unicode character
str8 &str8::operator+=(uint32 c) {
  if(!c) return *this;
  if(!len) return *this= c;

  str8 p= c;
  return *this+= p;
}

/// windows compat
str8 &str8::operator+=(cuint16 *s) {
  if(!s) return *this;
  if(!len) return *this= s;

  str8 p= s;
  return *this+= p;
}


// CONVERSION operators //
///--------------------///

// UTF-8 conversion from UTF-32 or basically an array of uint32's




uint32 *str8::convert32() {
  if(!len) return null;

  /// alloc d32
  int32 n= nrChars+ nrCombs;               /// n will hold the number of unicode values in the string (chars+combs)
  if(d32) delete[] d32;
  d32= new uint32[n+ 1];

  // copy
  cuint8 *p= d;                             /// p will 'walk' d

  for(int32 a= 0; a<= n; a++) {              // for each character
    /// character is ascii 0-127
    if(*p < 128) {
      d32[a]= *p++;
      continue;
    }
    /// character has 2 bytes
    if((*p& 0xe0) == 0xc0) {                /// test if first 3 bits (0xe0= 11100000) are 110xxxxx (0xc0= 11000000)
      d32[a]= (*p++)& 0x1f;                 /// [byte 1] last 5 bits (0x1f= 00011111)
      d32[a]<<=6; d32[a]+= (*p++)& 0x3f;    /// [byte 2] last 6 bits (0x3f= 00111111)
      continue;
    }
    /// character has 3 bytes
    if((*p& 0xf0) == 0xe0) {                /// test if first 4 bits (0xf0= 11110000) are 1110xxxx (0xe0= 11100000)
      d32[a]= (*p++)& 0x0f;                 /// [byte 1] last 4 bits (0x0f= 00001111)
      d32[a]<<= 6; d32[a]+= (*p++)& 0x3f;   /// [byte 2] last 6 bits (0x3f= 00111111)
      d32[a]<<= 6; d32[a]+= (*p++)& 0x3f;   /// [byte 3] last 6 bits (0x3f= 00111111)
      continue;
    }
    /// character has 4 bytes
    if((*p& 0xf8) == 0xf0) {                /// test if first 5 bits (0xf8= 11111000) are 11110xxx (0xf0= 11110000)
      d32[a]= (*p++)& 0x07;                 /// [byte 1] last 3 bits (0x07= 00000111)
      d32[a]<<= 6; d32[a]+= (*p++)& 0x3f;   /// [byte 2] last 6 bits (0x3f= 00111111)
      d32[a]<<= 6; d32[a]+= (*p++)& 0x3f;   /// [byte 3] last 6 bits (0x3f= 00111111)
      d32[a]<<= 6; d32[a]+= (*p++)& 0x3f;   /// [byte 4] last 6 bits (0x3f= 00111111)
      continue;
    }
    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    /// character has 5 bytes
    if((*p& 0xfc) == 0xf8) {                /// test if first 6 bits (0xfc= 11111100) are 111110xx (0xf8= 11111000)
      d32[a]= (*p++)& 0x03;                 /// [byte 1]   last 2 bits (0x03= 00000011)
      for(char b= 0; b< 4; b++)
        d32[a]<<= 6; d32[a]+= (*p++)& 0x3f; /// [byte 2-5] last 6 bits (0x3f= 00111111)
      continue;
    }
    /// character has 6 bytes
    if((*p& 0xfe) == 0xfc) {                /// test if first 7 bits (0xfe= 11111110) are 1111110x (0xfc= 11111100)
      d32[a]= (*p++)& 0x01;                 /// [byte 1]   last bit    (0x01= 00000001)
      for(char b= 0; b< 5; b++)
        d32[a]<<= 6; d32[a]+= (*p++)& 0x3f; /// [byte 2-6] last 6 bits (0x3f= 00111111)
      continue;
    }
  }	/// for each character
  
  return d32;
}


// windows compatibility
uint16 *str8::convertWin() {
  if(!len) return null;
  
  /// alloc dWin
  int32 n= nrChars+ nrCombs;               /// n will hold the number of unicode values in the string (chars+combs)
  if(dWin) delete[] dWin;
  dWin= new uint16[n+ 1];

  // copy
  uint8 *p= d;                              /// p 'walks' d

  for(int32 a= 0; a<= n; a++) {              // for each character
    /// character is ascii 0-127
    if(*p < 128) {
      dWin[a]= *p++;
      continue;
    }
    /// character has 2 bytes
    if((*p& 0xe0) == 0xc0) {                /// test if first 3 bits (0xe0= 11100000) are 110xxxxx (0xc0= 11000000)
      dWin[a]= (*p++)& 0x1f;                /// [byte 1] last 5 bits (0x1f= 00011111)
      dWin[a]<<=6; dWin[a]+= (*p++)& 0x3f;  /// [byte 2] last 6 bits (0x3f= 00111111)
      continue;
    }
    /// character has 3 bytes
    if((*p& 0xf0) == 0xe0) {                /// test if first 4 bits (0xf0= 11110000) are 1110xxxx (0xe0= 11100000)
      dWin[a]= (*p++)& 0x0f;                /// [byte 1] last 4 bits (0x0f= 00001111)
      dWin[a]<<= 6; dWin[a]+= (*p++)& 0x3f; /// [byte 2] last 6 bits (0x3f= 00111111)
      dWin[a]<<= 6; dWin[a]+= (*p++)& 0x3f; /// [byte 3] last 6 bits (0x3f= 00111111)
      continue;
    }
    /// character has 4 bytes
    if((*p& 0xf8) == 0xf0) {                /// test if first 5 bits (0xf8= 11111000) are 11110xxx (0xf0= 11110000)
      dWin[a]= 0xFFFD;                       // windows wide char can't handle unicode values greater than 2 bytes (lulz)
      p++;
      continue;
    }
    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    /// character has 5 bytes
    if((*p& 0xfc) == 0xf8) {                /// test if first 6 bits (0xfc= 11111100) are 111110xx (0xf8= 11111000)
      dWin[a]= 0xFFFD;                       // windows wide char can't handle unicode values greater than 2 bytes (lulz)
      p++;
      continue;
    }
    /// character has 6 bytes
    if((*p& 0xfe) == 0xfc) {                /// test if first 7 bits (0xfe= 11111110) are 1111110x (0xfc= 11111100)
      dWin[a]= 0xFFFD;                       // windows wide char can't handle unicode values greater than 2 bytes (lulz)
      p++;
      continue;
    }
  }	/// for each character
  
  return dWin;
}

// whole string conversion to lowercase
void str8::lower() {
  if(!d) return;                  /// if string is null, there is nothing to convert

  uint32 *buf= convert32();       /// buf will hold current string unpacked, utf-32 format
  
  for(int32 a= 0, n= nrChars+ nrCombs; a< n; a++)
    buf[a]= Str::tolower(buf[a]); /// lowercase each character
  
  *this= buf;
}


// whole string conversion to uppercase
void str8::upper() {
  if(!d) return;                  /// if string is null, there is nothing to convert

  uint32 *buf= convert32();       /// buf will hold current string unpacked, utf-32 format
  
  for(int32 a= 0, n= nrChars+ nrCombs; a< n; a++)
    buf[a]= Str::toupper(buf[a]); /// uppercase each character

  this->operator=(buf);           /// basically d= buf (utf32 to utf8)
}




///------------------------///
// OPERATOR - / OPERATOR -= // clears n chars from str8s
///------------------------///

/// clears n chars from the back of the string
str8 &str8::operator-=(int n) {
  /// basic checks
  if(!n) return *this;
  if(nrChars- n<= 0) { delData(); return *this; }
  
  return *this= (*this).operator-(n);     /// ... heh make shure it is the operator-
}


str8 str8::operator-(int n) const {
  /// basic checks
  if(!n) return str8(*this);
  if(nrChars- n<= 0) return str8();


  int remain= nrChars- n;         /// how many characters will remain in returned string
  uint8 *p= getChar(remain);   /// p will point right where the string needs to be cut
  uint8 c= *p;                    /// hold current value of *p to restore it
  *p= 0;                          /// cut current string temporary

  str8 ret(d);
  *p= c;                          /// restore current string
  return ret;
}



///-----------///
// OPERATOR == //
///-----------///

bool str8::operator==(const str8 &s) const {
  if(s.nrChars!= nrChars) return false;
  if(s.nrCombs!= nrCombs) return false;

  for(int32 a= 0, n= nrChars+ nrCombs; a< n; a++)
    if(d[a]!=s.d[a])
      return false;

  return true; // this point reached -> there is no difference between str8s
}


bool str8::operator==(cuint8 *s) const {
  if(!s) {
    if(!len)  return true;
    else      return false;
  }
  cuint8 *p1= d;
  cuint8 *p2= s;

  while(*p1)
    if((!*p2) || (*p1++!= *p2++)) // if string 2 ends || characters differ (increment pointers too)
      return false;

  if(*p2) return false;           // there are still chars in string 2 when string 1 ended ?

  return true;
} /// checks if strings are identical (utf-8)


bool str8::operator==(cuint32 *s) const {
  if(!s) {
    if(!d)  return true;
    else    return false;
  }

  str8 t(s);
  return (*this).operator==(t);
}


bool str8::operator==(cuint32 c) const {
  if(nrChars+ nrCombs!= 1) return false;
  return (utf8to32(d) == c);
}


bool str8::operator==(cuint16 *s) const {
  if(!s) {
    if(!d)  return true;
    else    return false;
  }
  str8 t(s);
  return (*this).operator==(t);
}



///-----------------------------------------///
// COMBining diacritical character functions //
///-----------------------------------------///

void str8::clearComb() {
  if(!len || !nrCombs) return;
  uint32 *u= convert32();

  /// recreate the string without diacriticals
  uint32 *t= new uint32[nrChars- nrCombs+ 1];
  for(int32 a= 0, b= 0; a< nrChars; a++)
    if(!isComb(u[a]))
      t[b++]= u[a];
    
  *this= t;     /// update internals
  delete[] t;
}


///----------------------------------------------///
// UTILITY FUNCTIONS, usually good for any string //
///----------------------------------------------///

void str8::updateLen() {
  if(!d) { len= nrChars= nrCombs= 0; return; }
  len= nrChars= nrCombs= 0;

  cuint8 *str= d;
  while(*str++) {
    len++;
    if((*str& 0xc0)!= 0x80) {    /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      if(isComb(utf8to32(str))) nrCombs++;
      else                      nrChars++;
    }
  }
  len++;
}





///--------------------------------///
// UTF-8 SECURITY / READ FROM FILES // (any read from a file should be considered UNSAFE)
///--------------------------------///


// bad characters will be MARKED with 0xFFFD

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 str8
str8 &str8::secureUTF8(cvoid *s) {
  delData();
  
  /// length in unicode vals
  int32 n= 0;
  cuint8 *p= (cuint8 *)s;
  bool bad;

  // can't assume that every character is ok, there can be garbage @ every level
  while(*p++) {
    if(*p < 128) n++;                   /// valid char if less than 128
    // search for headers, everything else is ignored.
    else if((*p& 0xe0) == 0xc0) n++;    /// header for 2 bytes
    else if((*p& 0xf0) == 0xe0) n++;    /// header for 3 bytes
    else if((*p& 0xf8) == 0xf0) n++;    /// header for 4 bytes
    else if((*p& 0xfc) == 0xf8) n++;    /// header for 5 bytes  marked as bad
    else if((*p& 0xfe) == 0xfc) n++;    /// header for 6 bytes  marked as bad
  }

  /// copy to temporary unpacked string
  uint32 *u= new uint32[n+ 1];          /// UNPACKED STRING

  p= (cuint8 *)s;
  int32 a= 0;
  while(a< n) {                         /// for each (valid) unicode value
    /// character is ascii 0-127
    if(*p < 128) {                      /// 1 byte characters are safe
      u[a++]= *p++;
      continue;
    /// character has 2 bytes
    } else if((*p& 0xe0) == 0xc0) {     /// header for 2 bytes? (0xe0= 11100000)
      // [BYTE 1] 
                     // test for overlong bytes 1100000x (10xxxxxx)
      if((*p& 0x1e) == 0) {             /// 1e= 00011110 if these 4 bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        continue;
      }
      u[a]= *p++& 0x1f;                 /// byte seems ok - copy from it (0x1f= 00011111)

      // [BYTE 2] 
      if((*p& 0xc0)!= 0x80) {           /// byte 2 has to be a continuation byte (start with 10xxxxxx)
        u[a++]= 0xFFFD;
        continue;                       /// don't increase p
      }
      u[a]<<=6; u[a]+= *p++ & 0x3f;     /// byte seems ok - copy from it (0x3f= 00111111)

  /// character has 3 bytes
  } else if((*p& 0xf0) == 0xe0) {       /// header for 3 bytes ? (0xf0= 11110000)
      // [BYTE 1]
                     // test for overlong bytes 11100000 100xxxxx (10xxxxxx) 
      if(     ((*p& 0x0f) == 0) &&      ///  f= 00001111               
         ((*(p+ 1)& 0x20) == 0)) {      /// 20=          00100000 if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        continue;
      }
      u[a]= *p++& 0x0f;                 /// byte seems ok - copy from it (0x0f= 00001111)

      // [BYTE 2-3]
      bad= false;
      for(short b= 0; b< 2; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-3 have to be continuation bytes (start with 10xxxxxx)
          u[a++]= 0xFFFD;
          bad= true;
          break;                     /// don't increase p
        }
        u[a]<<= 6; u[a]+= *p++& 0x3f;   /// byte seems ok - copy from it (0x3f= 00111111)
      }
      if(bad) continue;

    /// character has 4 bytes
    } else if((*p& 0xf8) == 0xf0) {     /// header for 4 bytes ? (0xf8= 11111000)
      // [BYTE 1]
                     // test for overlong bytes 11110000 1000xxxx (10xxxxxx 10xxxxxx) 
      if(      ((*p& 0x7) == 0) &&      ///  7= 00000111               
         ((*(p+ 1)& 0x30) == 0)) {      /// 30=          00110000 if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        continue;
      }
      u[a]= *p++& 0x07;                 /// byte seems ok - copy from it (0x07= 00000111)

      // [BYTE 2-4]
      bad= false;
      for(short b= 0; b< 3; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-4 have to be continuation bytes (start with 10xxxxxx)
          u[a++]= 0xFFFD;
          bad= true;                    /// don't increase p
          break;
        }
        u[a]<<= 6; u[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
      if(bad) continue;

    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    /// character has 5 bytes
    } else if((*p& 0xfc) == 0xf8) {     /// header for 5 bytes ? (0xfc= 11111100)
      // comment next 3 lines to make 5byte chars avaible
      u[a++]= 0xFFFD; // MARKED AS BAD
      p++;
      continue;

/* >>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 5 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      // [BYTE 1]
                     // test for overlong bytes 11111000 10000xxx (10xxxxxx 10xxxxxx 10xxxxxx) 
      if(     (*p& 0x3  == 0) &&        ///  3= 00000011               
         (*(p+ 1)& 0x38 == 0)) {        /// 38=          00111000 if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        continue;
      }
      u[a]= *p++& 0x03;                 /// byte seems ok - copy from it (0x03= 00000011)

      // [BYTE 2-5]
      for(short b= 0; b< 4; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-5 have to be continuation bytes (start with 10xxxxxx)
          u[a++]= 0xFFFD;
          continue;                     /// don't increase p
        }
        u[a]<<= 6; u[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
*/
    
    /// character has 6 bytes
    } else if((*p& 0xfe) == 0xfc) {              /// header for 6 bytes ? (0xfe= 11111110)
      // comment next 3 lines to make 6byte chars avaible
      u[a++]= 0xFFFD; // MARKED AS BAD
      p++;
      continue;

/* >>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 6 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      // [BYTE 1]
                     // test for overlong bytes 11111100 100000xx (10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx)
      if(     (*p& 0x1  == 0) &&        ///  1= 00000001               
         (*(p+ 1)& 0x3C == 0)) {        /// 3C=          00111100 if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        continue;
      }
      u[a]= *p++& 0x01;                 /// byte seems ok - copy from it (0x01= 00000001)

      // [BYTE 2-6]
      for(short b= 0; b< 5; b++) {
        if((*p& 0xc0)!= 0x80) {         /// bytes 2-6 have to be continuation bytes (start with 10xxxxxx)
          u[a++]= 0xFFFD;
          continue;                     /// don't increase p
        }
        u[a]<<= 6; u[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
      }
*/
    } else {
      p++;                               // character unreadable
      continue;
    }
    
    /// test result character d[a]
    if(u[a]>= 0x10FFFF)                 // limit of Unicode <--------------------- (maybe this changes in the future)
      u[a]= 0xFFFD;

    if( (u[a]>= 0xD800) && (u[a]<= 0xDFFF) )  /// it can't be a utf-16 surrogate
      u[a]= 0xFFFD;

    // further tests can be done here. there are still some chars that should be marked as malformed (0xFFFD) <<<=========================


    a++;
  }	/// for each character

  u[a]= 0;                              /// terminator

  /// compress unpacked string to *this
  *this= u;
  delete[] u;
  return *this;
}






// fopen knows of utf-8 but the win version wants to put a fukin BOM in the file, wich cause problems in linux, so file must be opened as pure binary

// read all file                   (validates each char)
void str8::readUTF8(FILE *f) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, pos, SEEK_SET);        /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);          /// maybe it read 1 byte? just go back anyways

  /// determine remaining filesize in bytes (maybe it is not reading from the start)
	long fs;
	fseek(f, 0L, SEEK_END);
	fs= ftell(f);
	fseek(f, pos, SEEK_SET);
  fs-= pos;

  if(!fs)
    return;

  /// read all file
  uint8 *buffer= new uint8[fs+ 1];
  fread(buffer, 1, fs, f);
  buffer[fs]= 0;                      /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}



// read n characters from file     (validates each char)
void str8::readUTF8n(FILE *f, size_t n) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, pos, SEEK_SET);        /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);          /// maybe it read 1 byte? just go back anyways

  /// determine remaining filesize in bytes (maybe it is not reading from the start)
	size_t fs;
	fseek(f, 0L, SEEK_END);
	fs= ftell(f);
	fseek(f, pos, SEEK_SET);
  fs-= pos;

  if(!fs)                             /// remaining filesize is 0 ?
    return;
  
  if(n> fs)                           /// it will read what it can
    n= fs;

  /// read n bytes from file
  uint8 *buffer= new uint8[n+ 1];
  fread(buffer, 1, n, f);
  buffer[n]= 0;                       /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}

/// read till end of line (or file) (validates each char)
void str8::readLineUTF8(FILE *f) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF)
      fseek(f, pos, SEEK_SET);        /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);          /// maybe it read 1 byte? just go back anyways

  /// compute line length
  long l;
  char r;

  while(fread(&r, 1, 1, f))
    if(r == '\n')
      break;
      
  l= ftell(f);
  l-= pos;
  if(!l) return;

  /// read line from file
  uint8 *buffer= new uint8[l+ 1];
  fseek(f, pos, SEEK_SET);
  fread(buffer, 1, l, f);
  buffer[l]= 0;                     /// terminator

  secureUTF8(buffer);

  delete[] buffer;
}











