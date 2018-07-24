#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

//#include <stdio.h> // not needed anymore
#include "util/strCommon.h"
#include "util/str8.h"
#include "util/str16.h"
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


//using namespace Str;


// CHANGES THE WAY THIS CLASS WORKS: uses specified buffer for every operation, no memory allocs are made.
// This buffer must be big enough for every operation made,
//   but critical errors will not occur if it's not big enough (the text will not be OK)
// <in_buffer>:   the buffer that the class will use or 'wrap' onto
// <in_size>:     the size of the buffer in uint32 units - this should be 2 minimum (1 for terminator, 1 for a character)
//                if buffer size is 0, THE SIZE IS DETERMINED BY THE STRING SIZE INSIDE THE BUFFER.
void str32::wrap(char32_t *in_buffer, int32_t in_size) {
  if((in_buffer== NULL) || (in_size== 1)) return;       /// minimum 2 (0 means is determined by the str that is inside)

  if(wrapping) stopWrapping();    /// stop current wrapping, if there is one
  delData();

  d= in_buffer;

  if(!in_size) {
    updateLen();
    wrapSize= len/ 4;
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
  d= NULL;
  wrapSize= 0;
  wrapping= false;
}



///----------------------------------------------///
// UTILITY FUNCTIONS, usually good for any string //
///----------------------------------------------///

void str32::updateLen() {
  if(!d) { nrUnicodes= len= 0; return; }
  
  uint32_t *p= (uint32_t *)d;
  while(*p++);
  nrUnicodes= (int32_t)(p- (uint32_t *)d- 1);
  len= (int32_t)((uint8_t *)p- (uint8_t *)d);
}


// main constructor / destructor
str32::str32(const str16 &s): str32() { *this= s.d; }
str32::str32(const str8 &s): str32()  { *this= s.d; }

str32::~str32() {
  delData();
  clean();
}

void str32::delData() {
  if(wrapping)
    *d= 0;                            /// just force a terminator on the first character
  else if(d) { delete[] d; d= NULL; }  // d8 & dWin must be deallocated only on destructor & convert8 & convertWin
    
  len= nrUnicodes= 0;
}


void str32::clean() {
  if(d8)  { delete[] d8;  d8= NULL;  }
  if(d16) { delete[] d16; d16= NULL; }
}



// OPERATOR= //
///---------///

str32 &str32::operator=(const str32 &s) {
  if(!s.len) { delData(); return *this; }
  if(this== &s) return *this;

  uint32_t *p1= (uint32_t *)d, *p2= (uint32_t *)s.d;

  if(wrapping) {
    int32_t n= wrapSize- 1;     /// maximum amount to copy
    while(*p2 && n--> 0)
      *p1++= *p2++;
    *p1= 0;

    nrUnicodes= (int32_t)(p1-(uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update internal vars
    len= s.len;
    nrUnicodes= s.nrUnicodes;

    /// alloc
    if(d) delete[] d;
    d= (char32_t *)new uint32_t[nrUnicodes+ 1];

    // copy
    p1= (uint32_t *)d;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator=(const str16 &s) { return *this= s.d; }
str32 &str32::operator=(const str8 &s)  { return *this= s.d; }


str32 &str32::operator=(const char32_t *s) {
  if(!s) { delData(); return *this; }

  uint32_t *p1= (uint32_t *)d, *p2= (uint32_t *)s;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- 1;   /// maximum amount to copy
    while(*p2 && n--> 0)
      *p1++= *p2++;
    *p1= 0;

    /// update internal vars
    nrUnicodes= (int32_t)(p1- (uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update internal vars
    nrUnicodes= Str::strunicodes32(s);
    len= (nrUnicodes+ 1)* 4;

    /// alloc
    if(d) delete[] d;
    d= (char32_t *)new uint32_t[nrUnicodes+ 1];

    // copy
    p1= (uint32_t *)d;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator=(const char16_t *s) {
  if(!s) { delData(); return *this; }

  uint32_t *p1= (uint32_t *)d;
  uint16_t *p2= (uint16_t *)s;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- 1;   /// maximum amount to copy
    while(*p2 && n--> 0)
      if(Str::isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;

    /// update internal vars
    nrUnicodes= (int32_t)(p1- (uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update internal vars
    nrUnicodes= Str::strunicodes16(s);
    len= (nrUnicodes+ 1)* 4;

    /// alloc
    if(d) delete[] d;
    d= (char32_t *)new uint32_t[nrUnicodes+ 1];

    // copy
    p1= (uint32_t *)d;
    while(*p2)
      if(Str::isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;
  }

  return *this;
}


str32 &str32::operator=(const char *s) {
  if(!s) { delData(); return *this; }

  uint32_t *p1;
  uint8_t *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- 1;   /// maximum amount to copy
    for(p1= (uint32_t *)d, p2= (uint8_t *)s; *p2 && n--> 0; ) {
      //*p1++= Str::utf8to32(p2, &nbytes), p2+= nbytes; // many times slower, only the code is very nice
      int32_t l;
      if(*p2 < 128) { *p1++= *p2++; continue; }
      else if((*p2& 0xe0) == 0xc0) l= 2, *p1= (*p2++)& 0x1f;
      else if((*p2& 0xf0) == 0xe0) l= 3, *p1= (*p2++)& 0x0f;
      else if((*p2& 0xf8) == 0xf0) l= 4, *p1= (*p2++)& 0x07;
      else if((*p2& 0xfc) == 0xf8) l= 5, *p1= (*p2++)& 0x03;  /// utf8 5 bytes are unused
      else if((*p2& 0xfe) == 0xfc) l= 6, *p1= (*p2++)& 0x01;  /// utf8 6 bytes are unused
      else { p2++; continue; }
      while(--l> 0)                   /// unpack utf8 continuation bytes
        *p1<<= 6, *p1+= *p2++ & 0x3f;

      p1++;
    }
    *p1= 0; /// str terminator

    /// update internal vars
    nrUnicodes= (int32_t)(p1- (uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update internal vars
    nrUnicodes= Str::strunicodes8(s);
    len= (nrUnicodes+ 1)* 4;

    /// alloc
    if(d) delete[] d;
    d= (char32_t *)new uint32_t[nrUnicodes+ 1];
    
    // copy
    for(p1= (uint32_t *)d, p2= (uint8_t *)s; *p2; ) {
      //*p1++= Str::utf8to32(p2, &nbytes), p2+= nbytes; // slower
      int32_t l;
      if(*p2 < 128) { *p1++= *p2++; continue; }
      else if((*p2& 0xe0) == 0xc0) l= 2, *p1= (*p2++)& 0x1f;
      else if((*p2& 0xf0) == 0xe0) l= 3, *p1= (*p2++)& 0x0f;
      else if((*p2& 0xf8) == 0xf0) l= 4, *p1= (*p2++)& 0x07;
      else if((*p2& 0xfc) == 0xf8) l= 5, *p1= (*p2++)& 0x03;  /// 5bytes utf8 should't be used
      else if((*p2& 0xfe) == 0xfc) l= 6, *p1= (*p2++)& 0x01;  /// 6bytes utf8 should't be used
      else { p2++; continue; }
      while(--l> 0)                   /// unpack utf8 continuation bytes
        *p1<<= 6, *p1+= *p2++ & 0x3f;

      p1++;
    }
    *p1= 0;
  }

  return *this;
}


str32 &str32::operator=(char32_t c) {
  if(!c) { delData(); return *this; }

  if(!wrapping)
    if(d && len!= 8) {
      delete[] d;
      d= (char32_t *)new uint32_t[2];
    }
  
  ((uint32_t *)d)[0]= c, ((uint32_t *)d)[1]= 0;
  len= 8, nrUnicodes= 1;

  return *this;
}








// OPERATOR + //
///----------///

str32 str32::operator+(const str32 &s) const {
  //return str32(*this)+= s;
  /// fast checks
  if(!s.nrUnicodes) return str32(*this);
  if(!nrUnicodes) return str32(s);
  
  uint32_t *p1, *p2;
  
  /// return string's variables
  str32 ret;
  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.len= len+ s.len- 4;
  ret.d= (char32_t *)new uint32_t[ret.nrUnicodes+ 1];

  // copy *this
  for(p1= (uint32_t *)ret.d, p2= (uint32_t *)d; *p2; )
    *p1++= *p2++;

  // copy s
  for(p2= (uint32_t *)s.d; *p2;)
    *p1++= *p2++;
  *p1= 0;

  return ret;
}

str32 str32::operator+(const str16 &s) const {
  //return str32(*this)+= s.d; // short version, slow
  /// fast checks
  if(!s.nrUnicodes) return str32(*this);
  if(!nrUnicodes) return str32(s);
  /// tmp vars
  uint32_t *p1, *p3;
  uint16_t *p2;

  /// return string's variables
  str32 ret;
  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.len= (ret.nrUnicodes+ 1)* 4;
  ret.d= (char32_t *)new uint32_t[ret.nrUnicodes+ 1];

  // copy *this
  for(p1= (uint32_t *)ret.d, p3= (uint32_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint16_t *)s.d; *p2; )
    if(Str::isHighSurrogate(*p2))
      *p1++= (*p2 << 10) + *(p2 + 1) + Str::UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      *p1++= *p2++;

  *p1= 0;

  return ret;
}



str32 str32::operator+(const str8 &s) const {
  //return str32(*this)+= s.d;    // short version, slow
  /// fast checks
  if(!s.nrUnicodes) return str32(*this);
  if(!nrUnicodes) return str32(s);
  /// tmp vars
  uint32_t *p1, *p3;
  uint8_t *p2;
  str32 ret;

  /// return string's variables
  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.len= (ret.nrUnicodes+ 1)* 4;
  ret.d= (char32_t *)new uint32_t[ret.nrUnicodes+ 1];

  // copy *this
  for(p1= (uint32_t *)ret.d, p3= (uint32_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint8_t *)s.d; *p2; ) {
    int32_t n;
    if(*p2 < 128) { *p1++= *p2++; continue; }
    else if((*p2& 0xe0) == 0xc0) n= 2, *p1= *p2++ & 0x1f;
    else if((*p2& 0xf0) == 0xe0) n= 3, *p1= *p2++ & 0x0f;
    else if((*p2& 0xf8) == 0xf0) n= 4, *p1= *p2++ & 0x07;
    else if((*p2& 0xfc) == 0xf8) n= 5, *p1= *p2++ & 0x03;   // utf8 5bytes should not be used
    else if((*p2& 0xfe) == 0xfc) n= 6, *p1= *p2++ & 0x01;   // utf8 6bytes should not be used
    else { p2++; continue; }
    
    while (--n> 0)        /// unpack continuation bytes
      *p1<<= 6, *p1+= *p2++ & 0x3f;
    p1++;
  }

  *p1= 0;

  return ret;
}

str32 str32::operator+(const char32_t *s) const {
  //return str32(*this)+= s;
  if(!s) return str32(*this);
  if(!*s) return str32(*this);
  if(!nrUnicodes) return str32(s);

  uint32_t *p1, *p2;

  /// ret - returned string
  str32 ret;
  for(p2= (uint32_t *)s; *p2; p2++);
  ret.nrUnicodes= nrUnicodes+ (int32_t)(p2- (uint32_t *)s);
  ret.len= (ret.nrUnicodes+ 1)* 4;
  ret.d= (char32_t *)new uint32_t[ret.nrUnicodes+ 1];

  // copy this
  for(p1= (uint32_t *)ret.d, p2= (uint32_t *)d; *p2; )
    *p1++= *p2++;

  // copy s
  for(p2= (uint32_t *)s; *p2; )
    *p1++= *p2++;
  *p1= 0;

  return ret;
}

str32 str32::operator+(const char16_t *s) const {
  //return str32(*this)+= s;
    /// fast checks
  if(!s) return str32(*this);
  if(!*s) return str32(*this);
  if(!nrUnicodes) return str32(s);
  /// tmp vars
  uint32_t *p1, *p3;
  uint16_t *p2;

  /// return string's variables
  str32 ret;
  ret.nrUnicodes= nrUnicodes;
  for(p2= (uint16_t *)s; *p2; ) {
    if(Str::isHighSurrogate(*p2)) p2+= 2;
    else p2++;
    ret.nrUnicodes++;
  }
  ret.len= (ret.nrUnicodes+ 1)* 4;
  ret.d= (char32_t *)new uint32_t[ret.nrUnicodes+ 1];

  // copy *this
  for(p1= (uint32_t *)ret.d, p3= (uint32_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint16_t *)s; *p2; )
    if(Str::isHighSurrogate(*p2))
      *p1++= (*p2 << 10) + *(p2 + 1) + Str::UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      *p1++= *p2++;
  *p1= 0;

  return ret;
}

str32 str32::operator+(const char *s) const {
  //return str32(*this)+= s;
  /// fast checks
  if(!s) return str32(*this);
  if(!*s) return str32(*this);
  if(!nrUnicodes) return str32(s);
  /// tmp vars
  uint32_t *p1, *p3;
  uint8_t *p2;
  /// return string's variables
  str32 ret;
  ret.nrUnicodes= nrUnicodes;
  for(p2= (uint8_t *)s; *p2; p2++)
    if((*p2& 0xc0)!= 0x80) /// if not a UTF-8 continuation byte, it's a new unicode value
      ret.nrUnicodes++;
  ret.len= (ret.nrUnicodes+ 1)* 4;
  ret.d= (char32_t *)new uint32_t[ret.nrUnicodes+ 1];

  // copy *this
  for(p1= (uint32_t *)ret.d, p3= (uint32_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint8_t *)s; *p2; ) {
    int32_t n;
    if(*p2 < 128) { *p1++= *p2++; continue; }
    else if((*p2& 0xe0) == 0xc0) n= 2, *p1= (*p2++) & 0x1f;
    else if((*p2& 0xf0) == 0xe0) n= 3, *p1= (*p2++) & 0x0f;
    else if((*p2& 0xf8) == 0xf0) n= 4, *p1= (*p2++) & 0x07;
    else if((*p2& 0xfc) == 0xf8) n= 5, *p1= (*p2++) & 0x03;   // 5bytes should not be used
    else if((*p2& 0xfe) == 0xfc) n= 6, *p1= (*p2++) & 0x01;   // 6bytes should not be used
    else { p2++; continue; }
    while (--n> 0)        /// unpack continuation bytes
      *p1<<= 6, *p1+= *p2++ & 0x3f;

    p1++;
  }
  *p1= 0;

  return ret;
}

str32 str32::operator+(char32_t c) const {
  //return str32(*this)+= c;
  if(!c) return str32(*this);
  if(!nrUnicodes) return str32(c);

  uint32_t *p1, *p2;

  str32 ret;
  ret.nrUnicodes= nrUnicodes+ 1;
  ret.len= len+ 4;
  ret.d= (char32_t *)new uint32_t[ret.nrUnicodes+ 1];

  for(p1= (uint32_t *)ret.d, p2= (uint32_t *)d; *p2; )
    *p1++= *p2++;
  *p1++= c;
  *p1= 0;

  return ret;
}









// OPERATOR += //
///-----------///

// this one is heavily used
str32 &str32::operator+=(const str32 &s) {
  if(!s.len) return *this;              /// other str32 empty -> return *this
  if(!len) return *this= s;             /// current str32 empty -> just copy other str32
  if(this== &s) return *this+= str32(s); /// a+= a would not work, this solves it

  uint32_t *p1, *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- nrUnicodes- 1;   /// maximum copy amount
    p1= (uint32_t *)d+ nrUnicodes;
    p2= (uint32_t *)s.d;
    while(*p2 && (n--> 0))
      *p1++= *p2++;
    *p1= 0;

    /// update vars
    nrUnicodes= (int32_t)(p1- (uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update vars
    nrUnicodes+= s.nrUnicodes;
    len+= s.len- 4;
    uint32_t *p= new uint32_t[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    p1= p, p2= (uint32_t *)d;
    while(*p2)
      *p1++= *p2++;

    delete[] d;
    d= (char32_t *)p;

    /// add new string
    p2= (uint32_t *)s.d;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator+=(const str16 &s) { return *this+= s.d; }
str32 &str32::operator+=(const str8 &s)  { return *this+= s.d; }


str32 &str32::operator+=(const char32_t *s) {
  if(!s) return *this;                /// other str32 empty -> return *this
  if(!len) return *this= s;           /// current str32 empty -> just copy other str32

  uint32_t *p1, *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- nrUnicodes- 1; /// maximum copy amount
    p1= (uint32_t *)d+ nrUnicodes;
    p2= (uint32_t *)s;
    while(*p2 && (n--> 0))
      *p1++= *p2++;
    *p1= 0;

    /// update vars
    nrUnicodes= (int32_t)(p1- (uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update vars
    nrUnicodes+= Str::strunicodes32(s);
    len= (nrUnicodes+ 1)* 4;
    uint32_t *p= new uint32_t[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    p1= p, p2= (uint32_t *)d;
    while(*p2)
      *p1++= *p2++;

    delete[] d;
    d= (char32_t *)p;

    /// add new string
    p2= (uint32_t *)s;
    while((*p1++= *p2++));
  }

  return *this;
}


str32 &str32::operator+=(const char16_t *s) {
  if(!s) return *this;          /// other str32 empty -> return *this
  if(!len) return *this= s;     /// current str32 empty -> just copy other str32

  uint32_t *p1, *p3;
  uint16_t *p2;
  
  if(wrapping) {
    // copy
    int32_t n= wrapSize- nrUnicodes- 1;       /// maximum copy amount
    p1= (uint32_t *)d+ nrUnicodes;
    p2= (uint16_t *)s;
    while(*p2 && (n--> 0))
      if(Str::isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;

    /// update vars
    nrUnicodes= (int32_t)(p1- (uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update vars
    nrUnicodes+= Str::strunicodes16(s);
    len= (nrUnicodes+ 1)* 4;
    uint32_t *p= new uint32_t[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    p1= p, p3= (uint32_t *)d;
    while(*p3)
      *p1++= *p3++;

    delete[] d;
    d= (char32_t *)p;

    /// add new string
    p2= (uint16_t *)s;
    while(*p2)
      if(Str::isHighSurrogate(*p2))
        *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
      else
        *p1++= *p2++;
    *p1= 0;
  }

  return *this;
}


str32 &str32::operator+=(const char *s) {
  if(!s) return *this;          /// other str32 empty -> return *this
  if(!len) return *this= s;     /// current str32 empty -> just copy other str32

  uint32_t *p1, *p3;
  uint8_t *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- nrUnicodes- 1;       /// maximum copy amount
    p1= (uint32_t *)d+ nrUnicodes;
    p2= (uint8_t *)s;
    while(*p2 && (n--> 0)) {
      //*p1++= Str::utf8to32(p2, &nbytes), p2+= nbytes;
      int32_t l;
      if (*p2 < 128) { *p1++= *p2++; continue; }
      else if((*p2& 0xe0) == 0xc0) l= 2, *p1= (*p2++)& 0x1f;
      else if((*p2& 0xf0) == 0xe0) l= 3, *p1= (*p2++)& 0x0f;
      else if((*p2& 0xf8) == 0xf0) l= 4, *p1= (*p2++)& 0x07;
      else if((*p2& 0xfc) == 0xf8) l= 5, *p1= (*p2++)& 0x03;  /// 5bytes utf8 should't be used
      else if((*p2& 0xfe) == 0xfc) l= 6, *p1= (*p2++)& 0x01;  /// 6bytes utf8 should't be used
      else { p2++; continue; }
      while (--l> 0)        /// unpack continuation bytes
        *p1<<= 6, *p1+= *p2++ & 0x3f;

      p1++;
    }
    *p1= 0;

    /// update vars
    nrUnicodes= (int32_t)(p1- (uint32_t *)d);
    len= (int32_t)((uint8_t *)p1- (uint8_t *)d)+ 4;

  } else {
    /// update vars
    nrUnicodes+= Str::strunicodes8(s);
    len= (nrUnicodes+ 1)* 4;
    uint32_t *p= new uint32_t[nrUnicodes+ 1];   /// new buffer alloc

    /// copy old string
    for(p1= p, p3= (uint32_t *)d; *p3; )
      *p1++= *p3++;

    delete[] d;
    d= (char32_t *)p;

    /// add new string
    for(p2= (uint8_t *)s; *p2; ) {
      int32_t n = 0;
      if (*p2 < 128) { *p1++= *p2++; continue; }
      else if((*p2& 0xe0) == 0xc0) n= 2, *p1= (*p2++)& 0x1f;
      else if((*p2& 0xf0) == 0xe0) n= 3, *p1= (*p2++)& 0x0f;
      else if((*p2& 0xf8) == 0xf0) n= 4, *p1= (*p2++)& 0x07;
      else if((*p2& 0xfc) == 0xf8) n= 5, *p1= (*p2++)& 0x03;  /// 5bytes utf8 should't be used
      else if((*p2& 0xfe) == 0xfc) n= 6, *p1= (*p2++)& 0x01;  /// 6bytes utf8 should't be used
      else { p2++; continue; }
      while (--n> 0)        /// unpack continuation bytes
        *p1<<= 6, *p1+= *p2++ & 0x3f;

      p1++;
    }
    *p1= 0;
  }

  return *this;
}


str32 &str32::operator+=(char32_t c) {
  if(!c) return *this;
  if(!len) return *this= c;

  if(wrapping) {
    if(nrUnicodes+ 1> wrapSize- 1) return *this;  /// buffer overflow check

    ((uint32_t *)d)[nrUnicodes]= c;
    nrUnicodes++;
    ((uint32_t *)d)[nrUnicodes]= 0;
    len+= 4;

  } else {
    /// update vars
    nrUnicodes++;
    len+= 4;
    uint32_t *p= new uint32_t[nrUnicodes+ 1];

    /// copy old string and add unival
    uint32_t *p1= p, *p2= (uint32_t *)d;
    while(*p2)
      *p1++= *p2++;
    *p1++= c;           // add unicode val
    *p1= 0;           /// str terminator

    delete[] d;
    d= (char32_t *)p;
  }

  return *this;
}





// CONVERSION operators //
///--------------------///



// convert to UTF-8
// returned string is part of the class, it will be auto deallocated. Use getUTF8() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char *str32::convert8(int32_t *out_len) {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  /// length of d8
  int32_t l= 0;
  uint32_t *p2= (uint32_t *)d;
  for(; *p2; p2++)              /// for each character in d
    l+= Str::utf8nrBytes(*p2);
  l++;

  /// create d8
  if(d8) delete[] d8;
  d8= (char *)new uint8_t[l];
  
  // UTF32 to UTF8 copy
  uint8_t *p= (uint8_t *)d8;
  for(p2= (uint32_t *)d; *p2; p2++) {   /// for each character in d
    //p+= Str::utf32to8(*p2, (char *)p);
    int32_t a;
    /// compress unicode value into utf-8
    if(*p2<= 0x0000007F)    { *p++= (uint8_t) *p2; continue; }
    else if(*p2<= 0x000007FF) *p++= (uint8_t) (*p2>> 6) | 0xC0, a= 0;
    else if(*p2<= 0x0000FFFF) *p++= (uint8_t) (*p2>> 12)| 0xE0, a= 6;
    else if(*p2<= 0x001FFFFF) *p++= (uint8_t) (*p2>> 18)| 0xF0, a= 12;
    else if(*p2<= 0x03FFFFFF) *p++= (uint8_t) (*p2>> 24)| 0xF8, a= 18; // 5bytes utf8 should't happen
    else if(*p2<= 0x7FFFFFFF) *p++= (uint8_t) (*p2>> 30)| 0xFC, a= 24; // 6bytes utf8 should't happen
    else continue;
    while(a>= 0)              /// pack utf8 continuation bytes
      *p++= (uint8_t)((*p2>>  a)& 0x3f)| 0x80, a-= 6;
  }
  *p= 0;                      /// str terminator

  if(out_len) *out_len= l;
  return d8;
}

// convert to UTF-16
// returned string is part of the class, it will be auto deallocated. Use getUTF16() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char16_t *str32::convert16(int32_t *out_len) {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  /// length of d16 in int16 units
  int32_t l= 0;
  uint32_t *p2= (uint32_t *)d;
  for(; *p2; p2++)
    l+= (*p2> 0xFFFF? 2: 1);
  l++;

  /// create d16
  if(d16) delete[] d16;
  d16= (char16_t *)new uint16_t[l];

  // UTF-32 to UTF-16 copy
  uint16_t *p= (uint16_t *)d16;
  for(p2= (uint32_t *)d; *p2; p2++)
    if(*p2> 0xFFFF) {
      *p++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (*p2>> 10));
      *p++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
    } else
      *p++= (uint16_t)*p2;

  *p= 0;

  if(out_len) *out_len= l* 2;
  return d16;
}


// convert to UTF-8
// similar to convert8() but returned string is released, and not part of the class 
//    it must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char *str32::getUTF8(int32_t *out_len) const {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  /// length of d8
  int32_t l= 0;
  uint32_t *p2= (uint32_t *)d;
  for(; *p2; p2++)              /// for each character in d
    l+= Str::utf8nrBytes(*p2);
  l++;

  /// create d8
  uint8_t *buf8= new uint8_t[l];
  
  // UTF32 to UTF8 copy
  uint8_t *p= buf8;
  for(p2= (uint32_t *)d; *p2; p2++) {   /// for each character in d
    //p+= Str::utf32to8(*p2, (char *)p);
    int32_t a;
    /// compress unicode value into utf-8
    if(*p2<= 0x0000007F)    { *p++= (uint8_t) *p2; continue; }
    else if(*p2<= 0x000007FF) *p++= (uint8_t) (*p2>> 6) | 0xC0, a= 0;
    else if(*p2<= 0x0000FFFF) *p++= (uint8_t) (*p2>> 12)| 0xE0, a= 6;
    else if(*p2<= 0x001FFFFF) *p++= (uint8_t) (*p2>> 18)| 0xF0, a= 12;
    else if(*p2<= 0x03FFFFFF) *p++= (uint8_t) (*p2>> 24)| 0xF8, a= 18; // 5bytes utf8 should't happen
    else if(*p2<= 0x7FFFFFFF) *p++= (uint8_t) (*p2>> 30)| 0xFC, a= 24; // 6bytes utf8 should't happen
    while(a>= 0)              /// pack utf8 continuation bytes
      *p++= (uint8_t)((*p2>>  a)& 0x3f)| 0x80, a-= 6;
  }
  *p= 0;                      /// str terminator

  if(out_len) *out_len= l;
  return (char *)buf8;
}

// convert to UTF-16
// similar to convert16() but returned string is released, and not part of the class 
//    it must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char16_t *str32::getUTF16(int32_t *out_len) const {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  /// length of d16
  int32_t l= 0;
  uint32_t *p2= (uint32_t *)d;
  for(; *p2; p2++)
    l+= (*p2> 0xFFFF? 2: 1);
  l++;

  /// create d16
  uint16_t *buf16= new uint16_t[l];

  // UTF-32 to UTF-16 copy
  uint16_t *p= buf16;
  for(p2= (uint32_t *)d; *p2; p2++)
    if(*p2> 0xFFFF) {
      *p++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (*p2>> 10));
      *p++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
    } else
      *p++= (uint16_t)*p2;
  *p= 0;

  if(out_len) *out_len= l* 2;
  return (char16_t *)buf16;
}


// returns string as UTF-8 in <out_buf>;
// please specify buffer size in <in_bufSize>;
// <out_nrUnicodes>, <out_len> (in bytes includes str terminator) - optional return values
void str32::convert8static(char *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if((!out_buf) || (in_bufSize<= 1)) return;
  if((!len) || (!nrUnicodes) || (!d)) { *(uint8_t *)out_buf= 0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(out_len) *out_len= 0; if(in_bufSize> 0) *(uint8_t *)out_buf= 0; return; }

  int32_t l= 0, u= 0;

  // UTF32 to UTF8 copy
  uint8_t *p1= (uint8_t *)out_buf;
  uint32_t *p2= (uint32_t *)d;
  int32_t m= in_bufSize- 1;
  for(; *p2; p2++) {
    int32_t a;

    /// compress unicode value into UTF-8
    if(*p2<= 0x0000007F) {
      if((l+= 1)> m) { l-= 1; break; }
      *p1++= (uint8_t) *p2; u++; continue;

    } else if(*p2<= 0x000007FF) {
      if((l+= 2)> m) { l-= 2; break; }
      *p1++= (uint8_t) (*p2>> 6) | 0xC0, a= 0;

    } else if(*p2<= 0x0000FFFF) {
      if((l+= 3)> m) { l-= 3; break; }
      *p1++= (uint8_t) (*p2>> 12)| 0xE0, a= 6;

    } else if(*p2<= 0x001FFFFF) {
      if((l+= 4)> m) { l-= 4; break; }
      *p1++= (uint8_t) (*p2>> 18)| 0xF0, a= 12;

    /// last 2 bytes shouldn't be used
    } else if(*p2<= 0x03FFFFFF) {
      if((l+= 5)> m) { l-= 5; break; }
      *p1++= (uint8_t) (*p2>> 24)| 0xF8, a= 18;

    } else if(*p2<= 0x7FFFFFFF) {
      if((l+= 6)> m) { l-= 6; break; }
      *p1++= (uint8_t) (*p2>> 30)| 0xFC, a= 24;
    } else continue;

    while(a>= 0)
      *p1++= (uint8_t)((*p2>>  a)& 0x3f)| 0x80, a-= 6;

    u++;
  }
  *p1= 0;

  if(out_len) *out_len= l+ 1;
  if(out_nrUnicodes) *out_nrUnicodes= u;
}

// returns string as UTF-16 in <out_buf>;
// please specify buffer size in <in_bufSize>;
// <out_nrUnicodes>, <out_len> (in bytes includes str terminator) - optional return values
void str32::convert16static(char16_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if((!out_buf) || (in_bufSize<= 1)) return;
  if((!len) || (!nrUnicodes) || (!d)) { *out_buf= 0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(out_len) *out_len= 0; if(in_bufSize> 0) *out_buf= 0; return; }

  int32_t l= 0, u= 0;

  // UTF32 to UTF8 copy
  uint16_t *p1= (uint16_t *)out_buf;
  uint32_t *p2= (uint32_t *)d;
  int32_t m= in_bufSize- 1;
  for(; *p2; p2++) {
    if(*p2>= 0x10000) {
      if((l+= 2)> m) { l-= 2; break; }
      *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (*p2>> 10));
      *p1++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
    } else {
      if((l+= 1)> m) { l-= 1; break; }
      *p1++= (uint16_t)*p2;
    }
    u++;
  }
  *p1= 0;

  if(out_len) *out_len= (l+ 1)* 2 ;
  if(out_nrUnicodes) *out_nrUnicodes= u;
}







// whole string conversion to lowercase
void str32::lower() {
  if(d== NULL) return;
  /// lowercase each character
  for(uint32_t *p= (uint32_t *)d; *p; p++)
    *p= Str::tolower(*p);
}


// whole string conversion to uppercase
void str32::upper() {
  if(d== NULL) return;
  /// uppercase each character
  for(uint32_t *p= (uint32_t *)d; *p; p++)
    *p= Str::toupper(*p);
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
  ((uint32_t *)d)[nrUnicodes]= 0;

  if(!wrapping) { /// alloc new string and copy from old
    uint32_t *p= (uint32_t *)d;
    d= (char32_t *)new uint32_t[nrUnicodes+ 1];

    uint32_t *p1= (uint32_t *)d, *p2= p;
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
  char32_t *p= d+ nrUnicodes- n;
  char32_t x= *p;                 /// x will hold the value where the cut was made
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
  if(s.nrUnicodes!= nrUnicodes) return false;
  /// pass thru all characters
  uint32_t *p1= (uint32_t *)d, *p2= (uint32_t *)s.d;
  while(*p1 && *p2)
    if(*p1++ != *p2++)
      return false;
  //if((*p1) || (*p2)) return false;  /// anything remains in any string -> they're not the same <<< this cannot happen if nrUnicodes are checked

  // this point reached -> there is no difference between strings
  return true;
}


bool str32::operator==(const str16 &s) const { return operator==(s.d); }
bool str32::operator==(const str8 &s) const  { return operator==(s.d); }


bool str32::operator==(const char32_t *s) const {
  /// if both are NULL, then there's no difference
  if(s== NULL) {
    if(d== NULL) return true;
    else         return false;
  }
  if(d== NULL) return false;

  /// pass thru each character to check for differences
  uint32_t *p1= (uint32_t *)d, *p2= (uint32_t *)s;
  while(*p1 && *p2)
    if(*p1++ != *p2++)
      return false;
  /// if there's still text in either one of the strings, there is a difference
  if(*p1 || *p2) return false;
  // reached this point, there is no difference
  return true;
}


bool str32::operator==(const char16_t *s) const {
  /// if both are NULL, then there's no difference
  if(s== NULL) {
    if(d== NULL) return true;
    else         return false;
  }
  if(d== NULL) return false;

  /// pass thru each character to check for differences
  uint32_t *p1= (uint32_t *)d;
  uint16_t *p2= (uint16_t *)s;
  while(*p1 && *p2) {
    uint32_t c;
    if(Str::isHighSurrogate(*p2))
      c= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c= *p2++;
    if(*p1++!= c) return false;
  }
  if(*p1 || *p2) return false;  /// if there's still text in either one of the strings, there is a difference

  // reached this point, there is no difference
  return true;
}


bool str32::operator==(const char *s) const {
  /// NULL strings logical branches
  if(s== NULL) {
    if(d== NULL) return true;   /// both NULL, no difference
    else         return false;  /// one NULL one not
  }
  if(d== NULL) return false;    /// one NULL one not

  /// pass thru each character to check for differences
  uint32_t *p1= (uint32_t *)d;
  uint8_t  *p2= (uint8_t *) s;
  while(*p1 && *p2) {
    /// check how many chars this utf8 pack has
    int32_t n;
    uint32_t c;
    if(*p2 < 128) { if(*p1++ != *p2++) return false; continue; }
    else if((*p2& 0xe0) == 0xc0) n= 2, c= (*p2++)& 0x1f;
    else if((*p2& 0xf0) == 0xe0) n= 3, c= (*p2++)& 0x0f;
    else if((*p2& 0xf8) == 0xf0) n= 4, c= (*p2++)& 0x07;
    else if((*p2& 0xfc) == 0xf8) n= 5, c= (*p2++)& 0x03;
    else if((*p2& 0xfe) == 0xfc) n= 6, c= (*p2++)& 0x01;
    else c= 0xFFFD, n= 0, p2++;
    /// unpack utf8 continuation bytes
    while(--n> 0)
      c<<= 6, c+= *p2++ & 0x3f;

    if(*p1++!= c) return false;
  }

  /// if there's still text in either one of the strings, there is a difference
  if(*p1 || *p2) return false;

  // reached this point, there is no difference
  return true;
}


bool str32::operator==(char32_t c) const {
  if(!c) {
    if(!d) return true;
    else   return false;
  }
  if(nrUnicodes!= 1) return false;
  return (*(uint32_t *)d== c? true: false);
}








///-----------------------------------------///
// COMBining diacritical character functions //
///-----------------------------------------///


// removes all combining diacriticals from the string
void str32::clearCombs() {
  if((!nrUnicodes) || (!d) || (!len)) return;

  // pass thru string, copying each char but skipping diacriticals 
  uint32_t *p1= (uint32_t *)d, *p2= (uint32_t *)d;
  for(; *p2; p2++)
    if(Str::isComb(*p2))      /// if it's a diacritical, skip it, update vars
      len-= 4, nrUnicodes--;
    else                      /// if it's not a diacritical, copy it
      *p1++= *p2;
  *p1= 0;

  /* to be or not to be - just clears mem of garbage, if any, but very little... doesn't worth it imho
  if(!wrapping) {
    uint32_t *p= (uint32_t *)d;
    d= (char32_t *)new uint32_t[nrUnicodes+ 1];
    Str::strcpy32(d, (char32_t *)p);
    delete[] p;
  }
  */
}

// returns the number of combining diacriticals (if any) in string
int32_t str32::nrCombs() const {
  if(!d) return 0;
  int32_t ret= 0;
  for(uint32_t *p= (uint32_t *)d; *p; p++)
    if(Str::isComb(*p))
      ret++;
  return ret;
}


// returns the number of characters in str, WHITOUT the number of combining diacriticals
int32_t str32::nrChars() const {
  if(!d) return 0;
  int32_t ret= 0;
  for(uint32_t *p= (uint32_t *)d; *p; p++)
    if(!Str::isComb(*p))
      ret++;
  return ret;
}







///--------------------------------///
// UTF-8 SECURITY / READ FROM FILES // (any read from a file should be considered UNSAFE)
///--------------------------------///


// bad characters will be MARKED with 0xFFFD

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
str32 &str32::secureUTF8(const char *s, int32_t in_len) {
  delData();
  if(s== NULL) return *this;
  if(in_len< 0) return *this;

  const uint8_t *p;
  bool bad, usingLimit= false;
  int32_t n= 0, limit;
  if(in_len) {
    usingLimit= true;
    limit= in_len;
  }

  /// length in unicode vals

  // can't assume that every character is ok, there can be garbage @ every level
  for(p= (const uint8_t *)s; *p; p++) {
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
    d= (char32_t *)new uint32_t[n+ 1];          /// UNPACKED STRING
  }
  uint32_t *u= (uint32_t *)d;
  limit= in_len;
  p= (const uint8_t *)s;
  int32_t a= 0;
  while(a< n) {                         /// for each (valid) unicode value
    if(usingLimit) if(limit<= 0) break;
    int32_t contBytes= 0;

    /// character is ascii 0-127
    if(*p < 128) {                      /// 1 byte characters are safe
      u[a++]= *p++;
      if(usingLimit) limit--;
      continue;

    /// character has 2 bytes
    } else if((*p& 0xe0) == 0xc0) {     /// header for 2 bytes? (0xe0= 11100000)
      // check for string limit reach; the string must have at least 2 bytes left for this char
      if(usingLimit) { limit-= 2; if(limit< 0) { u[a++]= 0xFFFD; break; }}

      // test for overlong bytes                1100000x (10xxxxxx)
      if((*p& 0x1e) == 0) {             /// 1e= 00011110 if these 4 bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 1;       /// update limit, only one byte [of 2] was processed
        continue;
      }
      u[a]= *p++& 0x1f;                 /// byte seems ok - copy from it (0x1f= 00011111)
      contBytes= 1;

    /// character has 3 bytes
    } else if((*p& 0xf0) == 0xe0) {       /// header for 3 bytes ? (0xf0= 11110000)
      // check for string limit reach; the string must have at least 3 bytes left for this char
      if(usingLimit) { limit-= 3; if(limit< 0) { u[a++]= 0xFFFD; break; }}

      // test for overlong bytes                11100000 100xxxxx (10xxxxxx) 
      if(     ((*p& 0x0f) == 0) &&      ///  f= 00001111          <--
         ((*(p+ 1)& 0x20) == 0)) {      /// 20=          00100000 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 2;       /// update limit, only one byte [of 3] was processed
        continue;
      }
      u[a]= *p++& 0x0f;                 /// byte seems ok - copy from it (0x0f= 00001111)
      contBytes= 2;

    /// character has 4 bytes
    } else if((*p& 0xf8) == 0xf0) {     /// header for 4 bytes ? (0xf8= 11111000)
      // check for string limit reach; the string must have at least 4 bytes left for this char
      if(usingLimit) { limit-= 4; if(limit< 0) { u[a++]= 0xFFFD; break; }}   

      // test for overlong bytes                11110000 1000xxxx (10xxxxxx 10xxxxxx) 
      if(      ((*p& 0x7) == 0) &&      ///  7= 00000111          <--
         ((*(p+ 1)& 0x30) == 0)) {      /// 30=          00110000 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 3;       /// update limit, only one byte [of 4] was processed
        continue;
      }
      u[a]= *p++& 0x07;                 /// byte seems ok - copy from it (0x07= 00000111)
      contBytes= 3;

    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    /// character has 5 bytes
    } else if((*p& 0xfc) == 0xf8) {     /// header for 5 bytes ? (0xfc= 11111100)
      // comment next 4 lines to make 5byte chars avaible
      u[a++]= 0xFFFD; // MARKED AS BAD
      p++;
      if(usingLimit) limit--;
      continue;

/* //>>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 5 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      // check for string limit reach; the string must have at least 5 bytes left for this char
      if(usingLimit) { limit-= 5; if(limit< 0) { u[a++]= 0xFFFD; break; }}

      // test for overlong bytes                11111000 10000xxx (10xxxxxx 10xxxxxx 10xxxxxx) 
      if(     (*p& 0x3  == 0) &&        ///  3= 00000011          <-\     
         (*(p+ 1)& 0x38 == 0)) {        /// 38=          00111000 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 4;       /// update limit, only one byte [of 5] was processed
        continue;
      }
      u[a]= *p++& 0x03;                 /// byte seems ok - copy from it (0x03= 00000011)
      contBytes= 4;
*/
    
    /// character has 6 bytes
    } else if((*p& 0xfe) == 0xfc) {              /// header for 6 bytes ? (0xfe= 11111110)
      // comment next 4 lines to make 6byte chars avaible
      u[a++]= 0xFFFD; // MARKED AS BAD
      p++;
      if(usingLimit) limit--;
      continue;

/* //>>>>>>>>>>>>>>>>>> UNCOMMENT THIS IF UTF-8 WILL USE 6 BYTES IN THE FUTURE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      // check for string limit reach; the string must have at least 6 bytes left for this char
      if(usingLimit) { limit-= 6; if(limit< 0) { u[a++]= 0xFFFD; break; }}

      // test for overlong bytes                11111100 100000xx (10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx)
      if(     (*p& 0x1  == 0) &&        ///  1= 00000001          <-\     
         (*(p+ 1)& 0x3C == 0)) {        /// 3C=          00111100 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 5;       /// update limit, only one byte [of 6] was processed
        continue;
      }
      u[a]= *p++& 0x01;                 /// byte seems ok - copy from it (0x01= 00000001)
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
        u[a++]= 0xFFFD;
        bad= true;
        if(usingLimit) limit+= contBytes;   /// update limit, only some bytes were read from
        break;                        /// don't increase p
      }
      u[a]<<= 6; u[a]+= *p++ & 0x3f;  /// byte seems ok - copy from it (0x3f= 00111111)
    }
    if(bad) continue;


    /// test result character d[a]
    if(u[a]>= 0x10FFFF)                 // limit of Unicode <--------------------- (maybe this changes in the future)
      u[a]= 0xFFFD;

    if( (u[a]>= 0xD800) && (u[a]<= 0xDFFF) )  /// it can't be a utf-16 surrogate
      u[a]= 0xFFFD;

    // further tests can be done here. there are still some chars that should be marked as malformed (0xFFFD) <<<=========================

    a++;
  }	/// for each character

  u[a]= 0;                              /// terminator
  updateLen();

  return *this;
}


extern char32_t _secure16to32advance(const char16_t **);

str32 &str32::secureUTF16(const char16_t *in_s, int32_t in_len) {
  delData();
  if(in_s== NULL) return *this;

  uint32_t *p1;
  uint16_t *p2;

  bool usingLimit= false;
  int32_t limit, n= 0;
  if(in_len) 
    usingLimit= true, limit= in_len;

  /// count nr of unicodes
  for(p2= (uint16_t *)in_s; *p2; ) {
    if(usingLimit) {
      if(Str::isHighSurrogate(*p2)) { if((limit- 4)< 0) break; }
      else                            if((limit- 2)< 0) break; }

    uint32_t c= _secure16to32advance((const char16_t **)&p2);
    limit= in_len- (int32_t)((uint8_t *)p2- (uint8_t *)in_s);
    n++;
  }
  if(n<= 0) return *this;

  /// wrapping / non wrapping branch
  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else {
    if(d) delete[] d;
    d= (char32_t *)new uint32_t[n+ 1];
  }

  // safe copy from source
  limit= in_len;
  p1= (uint32_t *)d, p2= (uint16_t *)in_s;
  while(n--> 0) {
    if(usingLimit) {
      if(Str::isHighSurrogate(*p2)) { if((limit- 4)< 0) break; }
      else                            if((limit- 2)< 0) break; }

    *p1++= _secure16to32advance((const char16_t **)&p2);
    limit= in_len- (int32_t)((uint8_t *)p2- (uint8_t *)in_s);

  } /// for each char/comb in s
  *p1= 0;                                   /// str terminator
  updateLen();

  return *this;
}


extern char32_t _secure32check(char32_t);

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf32 string
str32 &str32::secureUTF32(const char32_t *in_s, int32_t in_limit) {
  delData();
  if(in_s== NULL) return *this;

  uint32_t *p1;
  const uint32_t *p2;
  bool usingLimit= false;
  int32_t limit, n= 0;
  if(in_limit> 0) {
    limit= in_limit;
    usingLimit= true;
  }

  /// string length in int32 units (basically unicode values)
  for(p2= (const uint32_t *)in_s; *p2; p2++) {
    if(usingLimit) { limit-= 4; if(limit< 0) break; }
    n++;
  }

  /// wrapping / non wrapping branch
  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else 
    d= (char32_t *)new uint32_t[n+ 1];

  // safe copy from UTF-32
  p1= (uint32_t *)d, p2= (uint32_t *)in_s;
  limit= in_limit;
  while(n--> 0) {
    if(usingLimit) { limit-= 4; if(limit< 0) break; }
    *p1++= _secure32check(*p2++);
  }
  *p1= 0;          /// string terminator
  updateLen();

  return *this;
}






///------------------------------------------------------///
// character or string insertion / deletion from a string //
///------------------------------------------------------///

void str32::insert(char32_t in_unicode, int32_t in_pos) {
  if(wrapping)
    len= Str::insert32static(d, wrapSize, in_unicode, in_pos);
  else
    len= Str::insert32(&d, in_unicode, in_pos);
  nrUnicodes= (len/ 4)- 1;
}


void str32::insertStr(const char32_t *in_str, int32_t in_n, int32_t in_pos) {
  if(wrapping)
    len= Str::insertStr32static(d, wrapSize, in_str, in_n, in_pos);
  else
    len= Str::insertStr32(&d, in_str, in_n, in_pos);
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




/*
str32 &str32::f(const char *text, ...) {
}

str32 &str32::addInt(int64_t) {
}

str32 &str32::addUInt(uint64_t) {
}

str32 &str32::addFloat(float) {
}

str32 &str32::addDouble(double) {
}
*/











