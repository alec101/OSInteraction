#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

//#include <stdio.h>
#include "util/typeShortcuts.h"
#include "util/strCommon.h"
#include "util/str32.h"


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


// CHANGES THE WAY THIS CLASS WORKS: uses specified buffer for every operation, no memory allocs are made.
// This buffer must be big enough for every operation made,
//   but critical errors will not occur if it's not big enough (the text will not be OK)
// <in_buffer>:   the buffer that the class will use or 'wrap' onto
// <in_size>:     the size of the buffer in uint32 units - this should be 2 minimum (1 for terminator, 1 for a character)
//                if buffer size is 0, THE SIZE IS DETERMINED BY THE STRING SIZE INSIDE THE BUFFER.
void str32::wrap(uint32 *in_buffer, int32 in_size) {
  if((in_buffer== null) || (in_size<= 1)) return;

  if(wrapping) stopWrapping();    /// stop current wrapping, if there is one
  delData();

  d= in_buffer;

  if(!in_size) {
    updateLen();
    wrapSize= len;
  } else {
    wrapSize= in_size;
    d[wrapSize- 1]= 0;       /// force a string terminator
    updateLen();
  }

  wrapping= true;
}

// returns the way the class works to the normal functionality
// d will be memory allocated
void str32::stopWrapping() {
  d= null;
  wrapSize= 0;
  wrapping= false;
}



///----------------------------------------------///
// UTILITY FUNCTIONS, usually good for any string //
///----------------------------------------------///

void str32::updateLen() {
  if(!d) { nrUnicodes= len= 0; return; }
  
  uint32 *p= d;
  while(*p++);
  nrUnicodes= (int32)(p- d- 1);
  len= (nrUnicodes+ 1)* 4;
}


// main constructor / destructor

str32::~str32() {
  delData();
  clean();
}

void str32::delData() {
  if(wrapping)
    *d= 0;                            /// just force a terminator on the first character
  else if(d) { delete[] d; d= null; }  // d8 & dWin must be deallocated only on destructor & convert8 & convertWin
    
  len= nrUnicodes= 0;
}


void str32::clean() {
  if(d8)  { delete[] d8;  d8= null;  }
  if(d16) { delete[] d16; d16= null; }
}



// OPERATOR= //
///---------///

str32 &str32::operator=(const str32 &s) {
  if(!s.len) { delData(); return *this; }

  uint32 *p1= d, *p2= (uint32 *)s.d;

  if(wrapping) {
    int32 n= wrapSize- 1;     /// maximum amount to copy
    while(*p2 && n--> 0)
      *p1++= *p2++;
    *p1= 0;

    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update internal vars
    len= s.len;
    nrUnicodes= s.nrUnicodes;

    /// alloc
    if(d) delete[] d;
    d= new uint32[nrUnicodes+ 1];

    // copy
    p1= d;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator=(cuint32 *s) {
  if(!s) { delData(); return *this; }

  uint32 *p1= d, *p2= (uint32 *)s;

  if(wrapping) {
    // copy
    int32 n= wrapSize- 1;   /// maximum amount to copy
    while(*p2 && n--> 0)
      *p1++= *p2++;
    *p1= 0;

    /// update internal vars
    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update internal vars
    nrUnicodes= Str::strunicodes32(s);
    len= (nrUnicodes+ 1)* 4;

    /// alloc
    if(d) delete[] d;
    d= new uint32[nrUnicodes+ 1];

    // copy
    p1= d;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator=(cuint16 *s) {
  if(!s) { delData(); return *this; }

  uint32 *p1= d;
  cuint16 *p2= s;

  if(wrapping) {
    // copy
    int32 n= wrapSize- 1;   /// maximum amount to copy
    while(*p2 && n--> 0)
      if(isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;

    /// update internal vars
    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update internal vars
    nrUnicodes= Str::strunicodes16(s);
    len= (nrUnicodes+ 1)* 4;

    /// alloc
    if(d) delete[] d;
    d= new uint32[nrUnicodes+ 1];

    // copy
    p1= d;
    while(*p2)
      if(isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;
  }

  return *this;
}


str32 &str32::operator=(cuint8 *s) {
  if(!s) { delData(); return *this; }

  uint32 *p1= d;
  cuint8 *p2= s;
  int32 nbytes;

  if(wrapping) {
    // copy
    int32 n= wrapSize- 1;   /// maximum amount to copy
    while(*p2 && n--> 0)
      *p1++= Str::utf8to32(p2, &nbytes), p2+= nbytes;
    *p1= 0;

    /// update internal vars
    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update internal vars
    nrUnicodes= Str::strunicodes8(s);
    len= (nrUnicodes+ 1)* 4;

    /// alloc
    if(d) delete[] d;
    d= new uint32[nrUnicodes+ 1];
    
    // copy
    p1= d;
    while(*p2)
      *p1++= Str::utf8to32(p2, &nbytes), p2+= nbytes;
    *p1= 0;
  }

  return *this;
}


str32 &str32::operator=(uint32 c) {
  if(!c) { delData(); return *this; }

  if(!wrapping)
    if(d && len!= 8) {
      delete[] d;
      d= new uint32[2];
    }
  
  d[0]= c, d[1]= 0;
  len= 8, nrUnicodes= 1;

  return *this;
}



// OPERATOR += //
///-----------///

// this one is heavily used
str32 &str32::operator+=(const str32 &s) {
  if(!s.len) return *this;              /// other str32 empty -> return *this
  if(!len) return *this= s;             /// current str32 empty -> just copy other str32

  uint32 *p1, *p2= s.d;

  if(wrapping) {
    // copy
    int32 n= wrapSize- nrUnicodes- 1;   /// maximum copy amount
    p1= d+ nrUnicodes;
    while(*p2 && (n--> 0))
      *p1++= *p2++;
    *p1= 0;

    /// update vars
    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update vars
    nrUnicodes+= s.nrUnicodes;
    len+= s.len- 4;
    uint32 *p= new uint32[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    p1= p, p2= d;
    while(*p2)
      *p1++= *p2++;

    delete[] d;
    d= p;

    /// add new string
    p2= s.d;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator+=(cuint32 *s) {
  if(!s) return *this;                /// other str32 empty -> return *this
  if(!len) return *this= s;           /// current str32 empty -> just copy other str32

  uint32 *p1;
  cuint32 *p2= s;

  if(wrapping) {
    // copy
    int32 n= wrapSize- nrUnicodes- 1; /// maximum copy amount
    p1= d+ nrUnicodes;
    while(*p2 && (n--> 0))
      *p1++= *p2++;
    *p1= 0;

    /// update vars
    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update vars
    nrUnicodes+= Str::strunicodes32(s);
    len= (nrUnicodes+ 1)* 4;
    uint32 *p= new uint32[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    p1= p, p2= d;
    while(*p2)
      *p1++= *p2++;

    delete[] d;
    d= p;

    /// add new string
    p2= s;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator+=(cuint16 *s) {
  if(!s) return *this;          /// other str32 empty -> return *this
  if(!len) return *this= s;     /// current str32 empty -> just copy other str32

  uint32 *p1, *p3;
  cuint16 *p2= s;

  if(wrapping) {
    // copy
    int32 n= wrapSize- nrUnicodes- 1;       /// maximum copy amount
    p1= d+ nrUnicodes;
    while(*p2 && (n--> 0))
      if(isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;

    /// update vars
    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update vars
    nrUnicodes+= Str::strunicodes16(s);
    len= (nrUnicodes+ 1)* 4;
    uint32 *p= new uint32[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    p1= p, p3= d;
    while(*p3)
      *p1++= *p3++;

    delete[] d;
    d= p;

    /// add new string
    p2= s;
    while(*p2)
      if(isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;
  }

  return *this;
}



str32 &str32::operator+=(cuint8 *s) {
  if(!s) return *this;          /// other str32 empty -> return *this
  if(!len) return *this= s;     /// current str32 empty -> just copy other str32

  uint32 *p1, *p3;
  cuint8 *p2= s;
  int32 nbytes;

  if(wrapping) {
    // copy
    int32 n= wrapSize- nrUnicodes- 1;       /// maximum copy amount
    p1= d+ nrUnicodes;
    while(*p2 && (n--> 0))
      *p1++= Str::utf8to32(p2, &nbytes), p2+= nbytes;
    *p1= 0;

    /// update vars
    nrUnicodes= (int32)(p1- d- 1);
    len= (nrUnicodes+ 1)* 4;

  } else {
    /// update vars
    nrUnicodes+= Str::strunicodes8(s);
    len= (nrUnicodes+ 1)* 4;
    uint32 *p= new uint32[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    p1= p, p3= d;
    while(*p3)
      *p1++= *p3++;

    delete[] d;
    d= p;

    /// add new string
    p2= s;
    while(*p2)
      *p1++= Str::utf8to32(p2, &nbytes), p2+= nbytes;
    *p1= 0;
  }

  return *this;
}


str32 &str32::operator+=(uint32 c) {
  if(!c) return *this;
  if(!len) return *this= c;

  if(wrapping) {
    if(nrUnicodes+ 1> wrapSize- 1) return *this;  /// buffer overflow check

    d[nrUnicodes]= c;
    nrUnicodes++;
    d[nrUnicodes]= 0;
    len+= 4;

  } else {
    /// update vars
    nrUnicodes++;
    len+= 4;
    uint32 *p= new uint32[nrUnicodes+ 1];

    /// copy old string and add unival
    uint32 *p1= p, *p2= d;
    while(*p2)
      *p1++= *p2++;
    *p1++= c;           // add unicode val
    *p1= 0;           /// str terminator

    delete[] d;
    d= p;
  }

  return *this;
}





// CONVERSION operators //
///--------------------///



// convert to UTF-8
// returned string is part of the class, it will be auto deallocated. Use getUTF8() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
uint8 *str32::convert8(int32 *out_len) {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return null; }

  /// length of d8
  int32 l= 0;
  uint32 *p2= d;
  for(; *p2; p2++)              /// for each character in d
    l+= Str::utf8nrBytes(*p2);
  l++;

  /// create d8
  if(d8) delete[] d8;
  d8= new uint8[l];
  
  // UTF32 to UTF8 copy
  uint8 *p= d8;
  for(p2= d; *p2; p2++)         /// for each character in d
    p+= Str::utf32to8(*p2, p);
  *p= 0;                /// str terminator

  if(out_len) *out_len= l;
  return d8;
}

// convert to UTF-16
// returned string is part of the class, it will be auto deallocated. Use getUTF16() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
uint16 *str32::convert16(int32 *out_len) {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return null; }

  /// length of d16
  int32 l= 0;
  uint32 *p2= d;
  for(; *p2; p2++)
    l+= (*p2> 0xFFFF? 2: 1);
  l++;

  /// create d16
  if(d16) delete[] d16;
  d16= new uint16[l];

  // UTF-32 to UTF-16 copy
  uint16 *p= d16;
  for(p2= d; *p2; p2++)
    p+= Str::utf32to16(*p2, p);
  *p= 0;

  if(out_len) *out_len= l* 2;
  return d16;
}


// convert to UTF-8
// similar to convert8() but returned string is released, and not part of the class 
//    it must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
uint8 *str32::getUTF8(int32 *out_len) const {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return null; }

  /// length of d8
  int32 l= 0;
  uint32 *p2= d;
  for(; *p2; p2++)              /// for each character in d
    l+= Str::utf8nrBytes(*p2);
  l++;

  /// create d8
  uint8 *buf8= new uint8[l];
  
  // UTF32 to UTF8 copy
  uint8 *p= buf8;
  for(p2= d; *p2; p2++)         /// for each character in d
    p+= Str::utf32to8(*p2, p);
  *p= 0;                        /// str terminator

  if(out_len) *out_len= l;
  return buf8;
}

// convert to UTF-16
// similar to convert16() but returned string is released, and not part of the class 
//    it must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
uint16 *str32::getUTF16(int32 *out_len) const {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return null; }

  /// length of d16
  int32 l= 0;
  uint32 *p2= d;
  for(; *p2; p2++)
    l+= (*p2> 0xFFFF? 2: 1);
  l++;

  /// create d16
  uint16 *buf16= new uint16[l];

  // UTF-32 to UTF-16 copy
  uint16 *p= buf16;
  for(p2= d; *p2; p2++)
    p+= Str::utf32to16(*p2, p);
  *p= 0;

  if(out_len) *out_len= l* 2;
  return buf16;
}


// returns string as UTF-8 in <out_buf>;
// please specify buffer size in <in_bufSize>;
// <out_nrUnicodes>, <out_len> (in bytes includes str terminator) - optional return values
void str32::convert8static(uint8_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if((!out_buf) || (in_bufSize<= 1)) return;
  if((!len) || (!nrUnicodes) || (!d)) { *out_buf=0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(out_len) *out_len= 0; if(in_bufSize> 0) *out_buf= 0; return; }

  int32 l= 0, u= 0;

  // UTF32 to UTF8 copy
  uint8 *p1= out_buf;
  uint32 *p2= d;
  int32 m= in_bufSize- 1;
  for(; *p2; p2++) {
    int32 a= -1;

    /// compress unicode value into UTF-8
    if(*p2<= 0x0000007F) {
      if((l+= 1)> m) { l-= 1; break; }
      *p1++= (uint8) *p2,              a= -1;

    } else if(*p2<= 0x000007FF) {
      if((l+= 2)> m) { l-= 2; break; }
      *p1++= (uint8) (*p2>> 6) | 0xC0, a= 0;

    } else if(*p2<= 0x0000FFFF) {
      if((l+= 3)> m) { l-= 3; break; }
      *p1++= (uint8) (*p2>> 12)| 0xE0, a= 6;

    } else if(*p2<= 0x001FFFFF) {
      if((l+= 4)> m) { l-= 4; break; }
      *p1++= (uint8) (*p2>> 18)| 0xF0, a= 12;

    // last 2 bytes, UNUSED by utf ATM, but there be the code
    } else if(*p2<= 0x03FFFFFF) {
      if((l+= 5)> m) { l-= 5; break; }
      *p1++= (uint8) (*p2>> 24)| 0xF8, a= 18;

    } else if(*p2<= 0x7FFFFFFF) {
      if((l+= 6)> m) { l-= 6; break; }
      *p1++= (uint8) (*p2>> 30)| 0xFC, a= 24;
    }

    while(a>= 0)
      *p1++= (uint8)((*p2>>  a)& 0x3f)| 0x80, a-= 6;

    u++;
  }
  *p1= 0;

  if(out_len) *out_len= l+ 1;
  if(out_nrUnicodes) *out_nrUnicodes= u;
}

// returns string as UTF-16 in <out_buf>;
// please specify buffer size in <in_bufSize>;
// <out_nrUnicodes>, <out_len> (in bytes includes str terminator) - optional return values
void str32::convert16static(uint16_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if((!out_buf) || (in_bufSize<= 1)) return;
  if((!len) || (!nrUnicodes) || (!d)) { *out_buf=0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(out_len) *out_len= 0; if(in_bufSize> 0) *out_buf= 0; return; }

  int32 l= 0, u= 0;

  // UTF32 to UTF8 copy
  uint16 *p1= out_buf;
  uint32 *p2= d;
  int32 m= in_bufSize- 1;
  for(; *p2; p2++) {
    if(*p2>= 0x10000) {
      if((l+= 2)> m) { l-= 2; break; }
      *p1++= UTF16_LEAD_OFFSET+ (*p2>> 10);
      *p1++= 0xDC00+ (*p2& 0x3FF);
    } else {
      if((l+= 1)> m) { l-= 1; break; }
      *p1++= (uint16)*p2;
    }
    u++;
  }
  *p1= 0;

  if(out_len) *out_len= (l+ 1)* 2 ;
  if(out_nrUnicodes) *out_nrUnicodes= u;
}







// whole string conversion to lowercase
void str32::lower() {
  if(d== null) return;
  /// lowercase each character
  for(uint32 *p= d; *p; p++)
    *p= tolower(*p);
}


// whole string conversion to uppercase
void str32::upper() {
  if(d== null) return;
  /// uppercase each character
  for(uint32 *p= d; *p; p++)
    *p= toupper(*p);
}

///------------------------///
// OPERATOR - / OPERATOR -= // clears n chars from strings
///------------------------///

// cuts n unicode values from the end of this string
str32 &str32::operator-=(int n) {
  if(!n) return *this;
  if(nrUnicodes- n<= 0) { delData(); return *this; }
  
  nrUnicodes-= n;
  len-= n* 4;
  d[nrUnicodes]= 0;

  if(!wrapping) { /// alloc new string and copy from old
    uint32 *p= d;
    d= new uint32[nrUnicodes+ 1];

    uint32 *p1= d, *p2= p;
    while((*p1++= *p2++));

    delete[] p;
  }

  return *this;
}

// returns a string with n less characters
str32 str32::operator-(int n) const {
  /// basic checks
  if(!n) return str32(*this);
  if(nrUnicodes- n<= 0) return str32();

  /// temporary cut the string
  uint32 *p= d+ nrUnicodes- n;
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
  /// same number of characters
  if(s.nrUnicodes!= nrUnicodes)
    return false;
  /// pass thru all characters
  cuint32 *p1= d, *p2= s.d;
  while(*p1 && *p2)
    if(*p1++ != *p2++)
      return false;
  if((*p1) || (*p2)) return false;  /// anything remains in any string -> they're not the same

  // this point reached -> there is no difference between strings
  return true;
}


bool str32::operator==(cuint32 *s) const {
  /// if both are null, then there's no difference
  if(s== null) {
    if(d== null) return true;
    else         return false;
  }
  if(d== null) return false;

  /// pass thru each character to check for differences
  cuint32 *p1= d, *p2= s;
  while(*p1 && *p2)
    if(*p1++ != *p2++)
      return false;
  /// if there's still text in either one of the strings, there is a difference
  if(*p1 || *p2) return false;
  // reached this point, there is no difference
  return true;
}


bool str32::operator==(cuint16 *s) const {
  /// if both are null, then there's no difference
  if(s== null) {
    if(d== null) return true;
    else         return false;
  }
  if(d== null) return false;

  /// pass thru each character to check for differences
  cuint32 *p1= d;
  cuint16 *p2= s;
  uint32 c;
  while(*p1 && *p2) {
    if(isHighSurrogate(*p2))
      c= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c= *p2++;
    if(*p1++!= c) return false;
  }
  if(*p1 || *p2) return false;  /// if there's still text in either one of the strings, there is a difference

  // reached this point, there is no difference
  return true;
}


bool str32::operator==(const uint8_t *s) const {
  /// null strings logical branches
  if(s== null) {
    if(d== null) return true;   /// both null, no difference
    else         return false;  /// one null one not
  }
  if(d== null) return false;    /// one null one not

  /// pass thru each character to check for differences
  cuint32 *p1= d;
  cuint8 *p2= s;
  uint32 c;
  int32 nbytes;
  while(*p1 && *p2) {
    c= Str::utf8to32(p2, &nbytes), p2+= nbytes;
    if(*p1++!= c) return false;
  }

  /// if there's still text in either one of the strings, there is a difference
  if(*p1 || *p2) return false;

  // reached this point, there is no difference
  return true;
}


bool str32::operator==(uint32 c) const {
  if(!c) {
    if(!d) return true;
    else   return false;
  }
  if(nrUnicodes!= 1) return false;
  return (*d== c? true: false);
}








///-----------------------------------------///
// COMBining diacritical character functions //
///-----------------------------------------///


// removes all combining diacriticals from the string
void str32::clearCombs() {
  if((!nrUnicodes) || (!d) || (!len)) return;

  // pass thru string, copying each char but skipping diacriticals 
  uint32 *p1= d, *p2= d;
  for(; *p2; p2++)
    if(isComb(*p2))           /// if it's a diacritical, skip it, update vars
      len-= 4, nrUnicodes--;
    else                      /// if it's not a diacritical, copy it
      *p1++= *p2;
  *p1= 0;

  /* to be or not to be - just clears mem of garbage, if any, but very little... doesn't worth it imho
  if(!wrapping) {
    uint32 *p= d;
    d= new uint32[nrUnicodes+ 1];
    Str::strcpy32(d, p);
    delete[] p;
  }
  */
}

// returns the number of combining diacriticals (if any) in string
int32 str32::nrCombs() const {
  if(!d) return 0;
  int32 ret= 0;
  for(uint32 *p= d; *p; p++)
    if(isComb(*p))
      ret++;
  return ret;
}


// returns the number of characters in str, WHITOUT the number of combining diacriticals
int32_t str32::nrChars() const {
  if(!d) return 0;
  int32 ret= 0;
  for(uint32 *p= d; *p; p++)
    if(!isComb(*p))
      ret++;
  return ret;
}







///--------------------------------///
// UTF-8 SECURITY / READ FROM FILES // (any read from a file should be considered UNSAFE)
///--------------------------------///


// bad characters will be MARKED with 0xFFFD

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
str32 &str32::secureUTF8(cvoid *s, int32 in_len) {
  delData();
  if(s== null) return *this;
  if(in_len< 0) return *this;

  cuint8 *p;
  bool bad, usingLimit= false;
  int32 n= 0, limit;
  if(in_len) {
    usingLimit= true;
    limit= in_len;
  }

  /// length in unicode vals

  // can't assume that every character is ok, there can be garbage @ every level
  for(p= (cuint8 *)s; *p; p++) {
    if(usingLimit) if(limit<= 0) break; /// at least one byte must be able to be read
    if(*p < 128) n++;                 /// direct character read             (1 byte total)
    // search for headers, everything else is ignored.
    else if((*p& 0xe0) == 0xc0) n++;  /// UTF8 header+ 1 continuation byte  (2 bytes total)
    else if((*p& 0xf0) == 0xe0) n++;  /// UTF8 header+ 2 continuation bytes (3 bytes total)
    else if((*p& 0xf8) == 0xf0) n++;  /// UTF8 header+ 3 continuation bytes (4 bytes total)
    else if((*p& 0xfc) == 0xf8) n++;  /// UTF8 header+ 4 continuation bytes (5 bytes total)  marked as bad
    else if((*p& 0xfe) == 0xfc) n++;  /// UTF8 header+ 5 continuation bytes (6 bytes total)  marked as bad
    // THERE'S THE OPTION FOR AN UNREADABLE CHAR (else here) TO BE MARKED AS BAD, BUT n++ IS A MUST
    
    if(usingLimit) limit--;
  }
  if(n<= 0) return *this;

  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else {
    if(d) delete[] d;
    d= new uint32[n+ 1];          /// UNPACKED STRING
  }

  limit= in_len;
  p= (cuint8 *)s;
  int32 a= 0;
  while(a< n) {                         /// for each (valid) unicode value
    if(usingLimit) if(limit<= 0) break;
    int32 contBytes= 0;

    /// character is ascii 0-127
    if(*p < 128) {                      /// 1 byte characters are safe
      d[a++]= *p++;
      if(usingLimit) limit--;
      continue;

    /// character has 2 bytes
    } else if((*p& 0xe0) == 0xc0) {     /// header for 2 bytes? (0xe0= 11100000)
      // check for string limit reach; the string must have at least 2 bytes left for this char
      if(usingLimit) { limit-= 2; if(limit< 0) { d[a++]= 0xFFFD; break; }}

      // test for overlong bytes                1100000x (10xxxxxx)
      if((*p& 0x1e) == 0) {             /// 1e= 00011110 if these 4 bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 1;       /// update limit, only one byte [of 2] was processed
        continue;
      }
      d[a]= *p++& 0x1f;                 /// byte seems ok - copy from it (0x1f= 00011111)
      contBytes= 1;

    /// character has 3 bytes
    } else if((*p& 0xf0) == 0xe0) {       /// header for 3 bytes ? (0xf0= 11110000)
      // check for string limit reach; the string must have at least 3 bytes left for this char
      if(usingLimit) { limit-= 3; if(limit< 0) { d[a++]= 0xFFFD; break; }}

      // test for overlong bytes                11100000 100xxxxx (10xxxxxx) 
      if(     ((*p& 0x0f) == 0) &&      ///  f= 00001111          <--
         ((*(p+ 1)& 0x20) == 0)) {      /// 20=          00100000 <--if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 2;       /// update limit, only one byte [of 3] was processed
        continue;
      }
      d[a]= *p++& 0x0f;                 /// byte seems ok - copy from it (0x0f= 00001111)
      contBytes= 2;

    /// character has 4 bytes
    } else if((*p& 0xf8) == 0xf0) {     /// header for 4 bytes ? (0xf8= 11111000)
      // check for string limit reach; the string must have at least 4 bytes left for this char
      if(usingLimit) { limit-= 4; if(limit< 0) { d[a++]= 0xFFFD; break; }}   

      // test for overlong bytes                11110000 1000xxxx (10xxxxxx 10xxxxxx) 
      if(      ((*p& 0x7) == 0) &&      ///  7= 00000111          <--
         ((*(p+ 1)& 0x30) == 0)) {      /// 30=          00110000 <--if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 3;       /// update limit, only one byte [of 4] was processed
        continue;
      }
      d[a]= *p++& 0x07;                 /// byte seems ok - copy from it (0x07= 00000111)
      contBytes= 3;

    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    /// character has 5 bytes
    } else if((*p& 0xfc) == 0xf8) {     /// header for 5 bytes ? (0xfc= 11111100)
      // comment next 4 lines to make 5byte chars avaible
      d[a++]= 0xFFFD; // MARKED AS BAD
      p++;
      if(usingLimit) limit--;
      continue;

/* //>>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 5 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      // check for string limit reach; the string must have at least 5 bytes left for this char
      if(usingLimit) { limit-= 5; if(limit< 0) { d[a++]= 0xFFFD; break; }}

      // test for overlong bytes                11111000 10000xxx (10xxxxxx 10xxxxxx 10xxxxxx) 
      if(     (*p& 0x3  == 0) &&        ///  3= 00000011          <-\     
         (*(p+ 1)& 0x38 == 0)) {        /// 38=          00111000 <--if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 4;       /// update limit, only one byte [of 5] was processed
        continue;
      }
      d[a]= *p++& 0x03;                 /// byte seems ok - copy from it (0x03= 00000011)
      contBytes= 4;
*/
    
    /// character has 6 bytes
    } else if((*p& 0xfe) == 0xfc) {              /// header for 6 bytes ? (0xfe= 11111110)
      // comment next 4 lines to make 6byte chars avaible
      d[a++]= 0xFFFD; // MARKED AS BAD
      p++;
      if(usingLimit) limit--;
      continue;

/* //>>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 6 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      // check for string limit reach; the string must have at least 6 bytes left for this char
      if(usingLimit) { limit-= 6; if(limit< 0) { d[a++]= 0xFFFD; break; }}

      // test for overlong bytes                11111100 100000xx (10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx)
      if(     (*p& 0x1  == 0) &&        ///  1= 00000001          <-\     
         (*(p+ 1)& 0x3C == 0)) {        /// 3C=          00111100 <--if these bits are 0, this is a overlong byte
        d[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 5;       /// update limit, only one byte [of 6] was processed
        continue;
      }
      d[a]= *p++& 0x01;                 /// byte seems ok - copy from it (0x01= 00000001)
      contBytes= 5;
*/
    } else {
      // THERE'S THE OPTION FOR AN UNREADABLE CHAR TO BE MARKED AS BAD, HERE. AS IT IS, IT IS SKIPPED
      p++;                               // character unreadable
      if(usingLimit) limit--;
      continue;
    }
    

    // continuation bytes (max 5, last 2 are not used)
    bad= false;
    for(; contBytes; contBytes--) {
      if((*p& 0xc0)!= 0x80) {         /// bytes 2-6 have to be continuation bytes (start with 10xxxxxx)
        d[a++]= 0xFFFD;
        bad= true;
        if(usingLimit) limit+= contBytes;   /// update limit, only some bytes were read from
        break;                        /// don't increase p
      }
      d[a]<<= 6; d[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
    }
    if(bad) continue;


    /// test result character d[a]
    if(d[a]>= 0x10FFFF)                 // limit of Unicode <--------------------- (maybe this changes in the future)
      d[a]= 0xFFFD;

    if( (d[a]>= 0xD800) && (d[a]<= 0xDFFF) )  /// it can't be a utf-16 surrogate
      d[a]= 0xFFFD;

    // further tests can be done here. there are still some chars that should be marked as malformed (0xFFFD) <<<=========================

    a++;
  }	/// for each character

  d[a]= 0;                              /// terminator
  updateLen();

  return *this;
}


extern uint32 _secure16to32advance(cuint16 **);

str32 &str32::secureUTF16(cuint16 *in_s, int32 in_len) {
  delData();
  if(in_s== null) return *this;

  uint32 *p1;
  cuint16 *p2;

  bool usingLimit= false;
  int32 limit, n= 0;
  if(in_len) 
    usingLimit= true, limit= in_len;

  /// count nr of unicodes
  for(p2= in_s; *p2; ) {
    if(usingLimit) {
      if(limit<= 1) break;    /// there must be at least 2 bytes left to read to be able to do anything
      if(isHighSurrogate(*p2)) limit-= 4;
      else                     limit-= 2;
      if(limit< 0) break;     /// the limit would be broken if this char is read
    }

    uint32 c= _secure16to32advance(&p2);
    n++;
  }
  if(n<= 0) return *this;

  /// wrapping / non wrapping branch
  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else {
    if(d) delete[] d;
    d= new uint32[n+ 1];
  }

  // safe copy from source
  limit= in_len;
  p1= d, p2= in_s;
  while(n--> 0) {
    if(usingLimit) {
      if(limit<= 1) break;
      if(isHighSurrogate(*p2)) limit-= 4;
      else                     limit-= 2;
      if(limit< 0) break;
    }

    *p1++= _secure16to32advance(&p2);
  } /// for each char/comb in s
  *p1= 0;                                   /// str terminator
  updateLen();

  return *this;
}


extern uint32 _secure32check(uint32);

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf32 string
str32 &str32::secureUTF32(cuint32 *in_s, int32 in_limit) {
  delData();
  if(in_s== null) return *this;

  uint32 *p1;
  cuint32 *p2;
  bool usingLimit= false;
  int32 limit, n= 0;
  if(in_limit> 0) {
    limit= in_limit;
    usingLimit= true;
  }

  /// string length in int32 units (basically unicode values)
  for(p2= in_s; *p2; p2++) {
    if(usingLimit) { limit-= 4; if(limit< 0) break; }
    n++;
  }

  /// wrapping / non wrapping branch
  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else 
    d= new uint32[n+ 1];

  // safe copy from UTF-32
  p1= d, p2= in_s;
  limit= in_limit;
  while(n--> 0) {
    if(usingLimit) { limit-= 4; if(limit< 0) break; }
    *p1++= _secure32check(*p2++);
  }
  *p1= 0;          /// string terminator
  updateLen();

  return *this;
}







/*
// fopen knows of utf-8 but the win version wants to put a freakin BOM in the file, wich cause problems in linux, so file must be opened as binary

/// read all file                   (validates each char)
void str32::readUTF8(FILE *f) {
  /// read / ignore the BOM in an UTF file
  // BOM:
  // UTF-16:  U+FEFF/ U+FFFE
  // UTF-8:   0xEF,0xBB,0xBF
  uint8 bom[3];
  long start= ftell(f), pos;
  if(fread(bom, 3, 1, f)) {
    if((bom[0]!= 0xEF) && (bom[1]!= 0xBB) && (bom[2]!= 0xBF))
      fseek(f, start, SEEK_SET);        /// go back if it is not the BOM
  } else
    fseek(f, start, SEEK_SET);          /// maybe it read 1 byte? just go back anyways
  pos= ftell(f);
  
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
  buffer[fs]= buffer[fs- 1]= 0;         /// terminator bytes 1 + 2
  buffer[fs- 2]= buffer[fs- 3]= 0;      /// terminator bytes 3 + 4

  secureUTF8(buffer, fs+ 1);

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
      fseek(f, pos, SEEK_SET);    /// go back if it is not the BOM
  } else
    fseek(f, pos, SEEK_SET);      /// maybe it read 1 byte? just go back anyways

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
  buffer[l]= 0;                   /// str terminator

  secureUTF8(buffer);

  delete[] buffer;
}
*/




///------------------------------------------------------///
// character or string insertion / deletion from a string //
///------------------------------------------------------///

void str32::insert(uint32_t in_unicode, int32_t in_pos) {
  if(wrapping)
    len= Str::insert32static(d, wrapSize, in_unicode, in_pos);
  else
    len= Str::insert32(&d, in_unicode, in_pos);
  nrUnicodes= (len/ 4)- 1;
}


void str32::insertStr(cuint32 *in_str, int32_t in_pos) {
  if(wrapping)
    len= Str::insertStr32static(d, wrapSize, in_str, in_pos);
  else
    len= Str::insertStr32(&d, in_str, in_pos);
  nrUnicodes= (len/ 4)- 1;
}


void str32::del(int32_t in_nUnicodesToDel, int32_t in_pos) {
  // removing characters doesn't require to realloc memory, only very slight garbage remains in memory;
  // so del32static can be used instead of del32 if more speed is needed
  if(wrapping)
    len= Str::del32static(d, in_nUnicodesToDel, in_pos);
  else
    len= Str::del32(&d, in_nUnicodesToDel, in_pos);
  nrUnicodes= (len/ 4)- 1;
}










