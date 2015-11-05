#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

//#include <stdio.h>
#include "util/strCommon.h"
#include "util/str8.h"
#include "util/str16.h"
#include "util/str32.h"

// WARNING:
// lower() and upper(): At this moment, there are no surrogates that can lower into non-surogate value,
//   and vice-versa; these funcs are based on this rule, which can change in the future


/* UTF-16 research
  - the file signature (if present), or BOM, for UTF-16 is 0xFFFE or 0xFEFF

  http://unicode.org/faq/utf_bom.html
  -_Surrogates_ are code points from two special ranges of Unicode values, reserved for use as the leading,
   and trailing values of paired code units in UTF-16.
   
   High surrogates 0xD800 -> 0xDBFF, also called leading
   Low surrogates  0xDC00 -> 0xDFFF, also called trailing

   They are called surrogates, since they do not represent characters directly, but only as a pair.

  -
  Packing and unpacking to/from UTF-16

  Constants:
    const uint32 LEAD_OFFSET      = 0xD800 - (0x10000 >> 10);
    const uint32 SURROGATE_OFFSET = 0x10000 - (0xD800 << 10) - 0xDC00;

  Packing a 'codepoint' (unicode value) to UTF-16:
    uint16 lead  = LEAD_OFFSET + (codepoint >> 10);
    uint16 trail = 0xDC00 + (codepoint & 0x3FF);

  Unpacking from 2 surrogates (high and low):
    uint32 codepoint = (lead << 10) + trail + SURROGATE_OFFSET;
*/


using namespace Str;


// CHANGES THE WAY THIS CLASS WORKS: uses specified buffer for every operation, no memory allocs are made.
// This buffer must be big enough for every operation made,
//   but critical errors will not occur if it's not big enough (the text will not be OK)
// <in_buffer>:   the buffer that the class will use or 'wrap' onto
// <in_size>:     the size of the buffer in uint32 units - this should be 2 minimum (1 for terminator, 1 for a character)
//                if buffer size is 0, THE SIZE IS DETERMINED BY THE STRING SIZE INSIDE THE BUFFER.
void str16::wrap(char16_t *in_buffer, int32_t in_size) {
  if((in_buffer== NULL) || (in_size== 1)) return;   /// cannot be only 1 char, need space for terminator

  if(wrapping) stopWrapping();    /// stop current wrapping, if there is one
  delData();

  d= in_buffer;

  if(!in_size) {
    updateLen();
    wrapSize= len/ 2;
  } else {
    wrapSize= in_size;
    d[wrapSize- 1]= 0;       /// force a string terminator
    updateLen();
  }

  wrapping= true;
}

// returns the way the class works to the normal functionality
// d will be memory allocated
void str16::stopWrapping() {
  d= NULL;
  wrapSize= 0;
  wrapping= false;
}



///----------------------------------------------///
// UTILITY FUNCTIONS, usually good for any string //
///----------------------------------------------///

void str16::updateLen() {
  if(!d) { nrUnicodes= len= 0; return; }
  
  uint16_t *p= (uint16_t *)d;
  uint32_t nsurrogates= 0;
  for(; *p; p++)
    if(Str::isHighSurrogate(*p))
      nsurrogates++;

  nrUnicodes= (int32_t)(p- (uint16_t *)d- nsurrogates);
  len= (int32_t)((uint8_t *)p- (uint8_t *)d+ 2);
}


// main constructor / destructor

str16::str16(const str32 &s): str16() { *this= s.d; }
str16::str16(const str8 &s):  str16() { *this= s.d; }


str16::~str16() {
  delData();
  clean();
}

void str16::delData() {
  if(wrapping)
    *d= 0;                            /// just force a terminator on the first character
  else if(d) { delete[] d; d= NULL; }  // d8 & dWin must be deallocated only on destructor & convert8 & convertWin
    
  len= nrUnicodes= 0;
}


void str16::clean() {
  if(d8)  { delete[] d8;  d8= NULL;  }
  if(d32) { delete[] d32; d32= NULL; }
}



// OPERATOR= //
///---------///

str16 &str16::operator=(const str16 &s) {
  if(!s.len) { delData(); return *this; }
  if(this== &s) return *this;       // same object assignment check

  uint16_t *p1;
  const uint16_t *p2;

  if(wrapping) {
    int32_t n= wrapSize- 1;     /// maximum amount to copy
    int32_t nsurrogates= 0;
    for(p1= (uint16_t *)d, p2= (uint16_t *)s.d; *p2 && (n--> 0); ) {
      if(Str::isLowSurrogate(*p2))
        nsurrogates++;
      *p1++= *p2++;
    }
    *p1= 0;
    if(isHighSurrogate(*(p1- 1))) *(--p1)= 0;    /// cannot end with only one surrogate
    
    nrUnicodes= (int32_t)(p1- (uint16_t *)d- nsurrogates);
    len= (int32_t)(p1- (uint16_t *)d+ 1)* 2;

  } else {
    /// update internal vars
    len= s.len;
    nrUnicodes= s.nrUnicodes;

    /// alloc
    if(d) delete[] d;
    d= (char16_t *)new uint16_t[len/ 2];

    // copy
    for(p1= (uint16_t *)d, p2= (uint16_t *)s.d; (*p1++= *p2++); );
  }

  return *this;
}

str16 &str16::operator=(const str32 &s) { return *this= s.d; }
str16 &str16::operator=(const str8 &s)  { return *this= s.d; }


str16 &str16::operator=(const char16_t *s) {
  if(!s) { delData(); return *this; }

  uint16_t *p1, *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- 1;   /// maximum amount to copy
    int32_t nsurrogates= 0;
    for(p1= (uint16_t *)d, p2= (uint16_t *)s; *p2 && (n--> 0); ) {
      if(Str::isLowSurrogate(*p2))
        nsurrogates++;
      *p1++= *p2++;
    }
    *p1= 0;
    if(Str::isHighSurrogate(*(p1- 1))) *(--p1)= 0;    /// cannot end with only one surrogate

    /// update internal vars
    nrUnicodes= (int32_t)(p1- (uint16_t *)d- nsurrogates);
    len= (int32_t)(p1- (uint16_t *)d+ 1)* 2;

  } else {
    /// update internal vars
    int32_t nsurrogates= 0;
    for(p1= (uint16_t *)s; *p1; p1++)
      if(Str::isHighSurrogate(*p1))
        nsurrogates++;

    nrUnicodes= (int32_t)(p1- (uint16_t *)s- nsurrogates);
    len= (int32_t)(p1- (uint16_t *)s+ 1)* 2;

    /// alloc
    if(d) delete[] d;
    d= (char16_t *)new uint16_t[len/ 2];

    // copy
    for(p1= (uint16_t *)d, p2= (uint16_t *)s; (*p1++= *p2++); );
  }

  return *this;
}


str16 &str16::operator=(const char32_t *s) {
  if(!s) { delData(); return *this; }

  uint16_t *p1;
  uint32_t *p2;
  
  if(wrapping) {
    // copy
    int32_t n= wrapSize- 1;   /// maximum amount to copy
    int32_t nsurrogates= 0;
    for(p1= (uint16_t *)d, p2= (uint32_t *)s; *p2; p2++) {
      if(*p2> 0xFFFF) {
        if((n-= 2)< 0) break;
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (*p2>> 10));
        *p1++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
        nsurrogates++;
      } else {
        if((n-= 1)< 0) break;
        *p1++= (uint16_t)*p2;
      }
    }
    *p1= 0;         /// str terminator

    /// update internal vars
    nrUnicodes= (int32_t)(p1- (uint16_t *)d- nsurrogates);
    len= (int32_t)(p1- (uint16_t *)d+ 1)* 2;
    
  } else {
    /// update internal vars
    len= nrUnicodes= 0;
    for(p2= (uint32_t *)s; *p2; p2++) {
      if(*p2> 0xFFFF) len+= 4;
      else            len+= 2;
      nrUnicodes++;
    }
    len+= 2;        /// str terminator

    /// alloc
    if(d) delete[] d;
    d= (char16_t *)new uint16_t[len/ 2];

    // copy
    for(p1= (uint16_t *)d, p2= (uint32_t *)s; *p2; p2++)
      if(*p2> 0xFFFF) {
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (*p2>> 10));
        *p1++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
      } else
        *p1++= (uint16_t)*p2;
    *p1= 0;         /// str terminator
  }

  return *this;
}


str16 &str16::operator=(const char *s) {
  if(!s) { delData(); return *this; }

  uint16_t *p1;
  uint8_t *p2;
  int32_t nsurrogates= 0;
  uint32_t c;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- 1;   /// maximum amount to copy
    for(p1= (uint16_t *)d, p2= (uint8_t *)s; *p2; ) {
      /// utf8 to unicode32
      int32_t l;
      if(*p2 < 128) { if((n-= 1)< 0) break; *p1++= *p2++; continue; } // direct value to value, next char
      else if((*p2& 0xe0) == 0xc0) l= 2, c= (*p2++)& 0x1f;
      else if((*p2& 0xf0) == 0xe0) l= 3, c= (*p2++)& 0x0f;
      else if((*p2& 0xf8) == 0xf0) l= 4, c= (*p2++)& 0x07;
      else if((*p2& 0xfc) == 0xf8) { p2+= 5; continue; }  /// byte 5 ignored (cannot convert to utf-16)
      else if((*p2& 0xfe) == 0xfc) { p2+= 6; continue; }  /// byte 6 ignored (cannot convert to utf-16)
      else { p2++; continue; }
      while(--l> 0)
        c<<= 6, c+= *p2++ & 0x3f;

      /// unicode to utf16
      if(c> 0xFFFF) {
        if((n-= 2)< 0) break;
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (c>> 10));
        *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
        nsurrogates++;
      } else {
        if((n-= 1)< 0) break;
        *p1++= (uint16_t)c;
      }
    }
    *p1= 0;

    /// update internal vars
    nrUnicodes= (int32_t)(p1- (uint16_t *)d- nsurrogates);
    len= (int32_t)(p1- (uint16_t *)d+ 1)* 2;

  } else {
    /// update internal vars
    for(p2= (uint8_t *)s; *p2; )
      if(*p2< 128)                  len+= 2, p2++;
      else if((*p2& 0xe0) == 0xc0)  len+= 2, p2+= 2;
      else if((*p2& 0xf0) == 0xe0)  len+= 2, p2+= 3;
      else if((*p2& 0xf8) == 0xf0)  len+= 4, p2+= 4, nsurrogates++;
      else if((*p2& 0xfc) == 0xf8)  { p2+= 5; continue; } /// skip utf8 5 bytes char
      else if((*p2& 0xfe) == 0xfc)  { p2+= 6; continue; } /// skip utf8 6 bytes char
      else { p2++; continue; }

    nrUnicodes= (len/ 2)- nsurrogates;
    len+= 2;                            /// space for terminator
    
    /// alloc
    if(d) delete[] d;
    d= (char16_t *)new uint16_t[len/ 2];
    
    // copy
    for(p1= (uint16_t *)d, p2= (uint8_t *)s; *p2; ) {
      /// UTF-8 to unicode32
      int32_t l;
      if(*p2 < 128) { *p1++= *p2++; continue; }
      else if((*p2& 0xe0) == 0xc0) l= 2, c= (*p2++)& 0x1f;
      else if((*p2& 0xf0) == 0xe0) l= 3, c= (*p2++)& 0x0f;
      else if((*p2& 0xf8) == 0xf0) l= 4, c= (*p2++)& 0x07;
      else if((*p2& 0xfc) == 0xf8) { p2+= 5; continue; }  /// utf8 5 bytes skipped (cannot convert to utf-16)
      else if((*p2& 0xfe) == 0xfc) { p2+= 6; continue; }  /// utf8 6 bytes skipped (cannot convert to utf-16)
      else { p2++; continue; }
      while(--l> 0)
        c<<= 6, c+= *p2++ & 0x3f;

      /// unicode32 to UTF-16
      if(c> 0xFFFF) {
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (c>> 10));
        *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
      } else 
        *p1++= (uint16_t)c;
    }
    *p1= 0;
  }

  return *this;
}


str16 &str16::operator=(char32_t c) {
  if(!c) { delData(); return *this; }

  int32_t n= (c> 0xFFFF? 2: 1);

  if(wrapping) {
    if(n> wrapSize- 1) return *this;
  } else
    if(d && len!= (n+ 1)* 2) {
      delete[] d;
      d= (char16_t *)new uint16_t[n+ 1];
    }
  
  Str::utf32to16(c, d);
  ((uint16_t *)d)[n]= 0;         /// str terminator
  len= (n+ 1)* 2, nrUnicodes= 1;

  return *this;
}







///==========///
// OPERATOR + //
///==========///


str16 str16::operator+(const str8 &s) const {
  //return str16(*this)+= s.d; // very slow
  if(!s.len) return str16(*this); /// other str16 empty -> return *this
  if(!len) return str16(s);       /// current str16 empty -> just copy other str16

  uint16_t *p1, *p3;
  uint8_t *p2;

  /// ret - return string
  str16 ret;
  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.len= len;
  for(p2= (uint8_t *)s.d; *p2; )
    if(*p2 < 128)                 p2++,   ret.len+= 2;
    else if((*p2& 0xe0) == 0xc0)  p2+= 2, ret.len+= 2;
    else if((*p2& 0xf0) == 0xe0)  p2+= 3, ret.len+= 2;
    else if((*p2& 0xf8) == 0xf0)  p2+= 4, ret.len+= 4;
    else if((*p2& 0xfc) == 0xf8)  p2+= 5; /// utf8 with 5 bytes thould not be used
    else if((*p2& 0xfe) == 0xfc)  p2+= 6; /// utf8 with 6 bytes should not be used
    else p2++;

  ret.d= (char16_t *)new uint16_t[ret.len/ 2];

  // copy this
  for(p1= (uint16_t *)ret.d, p3= (uint16_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint8_t *)s.d; *p2; ) {
    // utf8 to unicode32
    uint32_t c;
    int32_t n;

    if(*p2 < 128)                n= 1, c= *p2++;
    else if((*p2& 0xe0) == 0xc0) n= 2, c= (*p2++)& 0x1f;
    else if((*p2& 0xf0) == 0xe0) n= 3, c= (*p2++)& 0x0f;
    else if((*p2& 0xf8) == 0xf0) n= 4, c= (*p2++)& 0x07;
    else if((*p2& 0xfc) == 0xf8) { p2+= 5; continue; } // n= 5, c= (*p2++)& 0x03;
    else if((*p2& 0xfe) == 0xfc) { p2+= 6; continue; } // n= 6, c= (*p2++)& 0x01;
    else { p2++; continue; }

    /// unpack the utf8
    while(--n> 0)
      c<<= 6, c+= *p2++ & 0x3f;

    // unicode32 to UTF-16 pack
    if(c> 0xFFFF) {
      *p1++= (uint16_t)(UTF16_LEAD_OFFSET+ (c>> 10));
      *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
    } else
      *p1++= (uint16_t)c;
  }
  *p1= 0;                         /// str terminator

  return ret;
}

str16 str16::operator+(const char *s) const {
  //return str16(*this)+= s; // very slow
  if(!s) return str16(*this);     /// other str16 empty -> return *this
  if(!*s) return str16(*this);    /// other str16 empty -> return *this
  if(!len) return str16(s);       /// current str16 empty -> just copy other str16

  uint16_t *p1, *p3;
  uint8_t *p2;

  /// ret - return string
  str16 ret;
  ret.nrUnicodes= nrUnicodes;
  ret.len= len;
  for(p2= (uint8_t *)s; *p2; ) {
    if(*p2 < 128)                 p2++,   ret.len+= 2;
    else if((*p2& 0xe0) == 0xc0)  p2+= 2, ret.len+= 2;
    else if((*p2& 0xf0) == 0xe0)  p2+= 3, ret.len+= 2;
    else if((*p2& 0xf8) == 0xf0)  p2+= 4, ret.len+= 4;
    else if((*p2& 0xfc) == 0xf8)  p2+= 5; /// utf8 5bytes ignored
    else if((*p2& 0xfe) == 0xfc)  p2+= 6; /// utf8 6bytes ignored
    else p2++;
    ret.nrUnicodes++;
  }

  ret.d= (char16_t *)new uint16_t[ret.len/ 2];

  // copy this
  for(p1= (uint16_t *)ret.d, p3= (uint16_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint8_t *)s; *p2; ) {
    // utf8 to unicode32
    uint32_t c;
    int32_t n;

    if(*p2 < 128) { *p1++= *p2++; continue; }
    else if((*p2& 0xe0) == 0xc0) n= 2, c= (*p2++)& 0x1f;
    else if((*p2& 0xf0) == 0xe0) n= 3, c= (*p2++)& 0x0f;
    else if((*p2& 0xf8) == 0xf0) n= 4, c= (*p2++)& 0x07;
    else if((*p2& 0xfc) == 0xf8) { p2+= 5; continue; } // n= 5, c= (*p2++)& 0x03;
    else if((*p2& 0xfe) == 0xfc) { p2+= 6; continue; } // n= 6, c= (*p2++)& 0x01;
    else { p2++; continue; }

    /// unpack the utf8
    while(--n> 0)
      c<<= 6, c+= *p2++ & 0x3f;

    // unicode32 to UTF-16 pack
    if(c> 0xFFFF) {
      *p1++= (uint16_t)(UTF16_LEAD_OFFSET+ (c>> 10));
      *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
    } else
      *p1++= (uint16_t)c;
  }
  *p1= 0;                         /// str terminator

  return ret;
}

str16 str16::operator+(const str16 &s) const {
  //return str16(*this)+= s; // very slow
  if(!s.len) return str16(*this); /// other str16 empty -> return *this
  if(!len) return str16(s);       /// current str16 empty -> just copy other str16

  uint16_t *p1, *p2;

  /// ret - return string
  str16 ret;
  ret.len= len+ s.len- 2;
  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.d= (char16_t *)new uint16_t[ret.len/ 2];

  // copy this
  for(p1= (uint16_t *)ret.d, p2= (uint16_t *)d; *p2; )
    *p1++= *p2++;

  // copy s
  for(p2= (uint16_t *)s.d; *p2; )
    *p1++= *p2++;
  *p1= 0;                         /// str terminator

  return ret;
}

str16 str16::operator+(const char16_t *s) const {
  //return str16(*this)+= s; // very slow
  if(!s) return str16(*this);     /// other str16 empty -> return *this
  if(!*s) return str16(*this);    /// other str16 empty -> return *this
  if(!len) return str16(s);       /// current str16 empty -> just copy other str16

  uint16_t *p1, *p2;

  /// ret - return string
  str16 ret;
  ret.len= len;
  ret.nrUnicodes= nrUnicodes;
  for(p2= (uint16_t *)s; *p2; ) {
    if(isHighSurrogate(*p2)) ret.len+= 4, p2+= 2;
    else                     ret.len+= 2, p2++;
    ret.nrUnicodes++;
  }
  ret.d= (char16_t *)new uint16_t[ret.len/ 2];

  // copy this
  for(p1= (uint16_t *)ret.d, p2= (uint16_t *)d; *p2; )
    *p1++= *p2++;

  // copy s
  for(p2= (uint16_t *)s; *p2; )
    *p1++= *p2++;
  *p1= 0;

  return ret;
}

str16 str16::operator+(const str32 &s) const {
  //return str16(*this)+= s.d; // very slow
  if(!s.len) return str16(*this); /// other str empty -> return *this
  if(!len) return str16(s);       /// current str empty -> just copy other str16

  uint16_t *p1, *p3;
  uint32_t *p2;

  /// ret - return string
  str16 ret;
  ret.len= len+ s.len- 4;
  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.d= (char16_t *)new uint16_t[ret.len/ 2];

  // copy this
  for(p1= (uint16_t *)ret.d, p3= (uint16_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint32_t *)s.d; *p2; p2++)
    if(*p2> 0xFFFF) {
      *p1++= (uint16_t)(UTF16_LEAD_OFFSET+ (*p2>> 10));
      *p1++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
    } else
      *p1++= (uint16_t)*p2;

  *p1= 0;                         /// str terminator

  return ret;
}

str16 str16::operator+(const char32_t *s) const {
  //return str16(*this)+= s.d; // very slow
  if(!s) return str16(*this);     /// other str empty -> return *this
  if(!*s) return str16(*this);    /// other str empty -> return *this
  if(!len) return str16(s);       /// current str empty -> just copy other str16

  uint16_t *p1, *p3;
  uint32_t *p2;

  /// ret - return string
  str16 ret;
  ret.len= len;
  ret.nrUnicodes= nrUnicodes;
  for(p2= (uint32_t *)s; *p2; p2++) {
    if(*p2> 0xFFFF) ret.len+= 4;
    else            ret.len+= 2;
    ret.nrUnicodes++;
  }
  ret.d= (char16_t *)new uint16_t[ret.len/ 2];

  // copy this
  for(p1= (uint16_t *)ret.d, p3= (uint16_t *)d; *p3; )
    *p1++= *p3++;

  // copy s
  for(p2= (uint32_t *)s; *p2; p2++)
    if(*p2> 0xFFFF) {
      *p1++= (char16_t)(UTF16_LEAD_OFFSET+ (*p2>> 10));
      *p1++= (char16_t)(0xDC00+ (*p2& 0x3FF));
    } else
      *p1++= (char16_t)*p2;

  *p1= 0;                         /// str terminator

  return ret;
}

str16 str16::operator+(char32_t c) const {
  if(!c) return str16(*this);
  if(!nrUnicodes) return str16(c);

  uint16_t *p1, *p2;

  str16 ret;
  ret.nrUnicodes= nrUnicodes+ 1;
  ret.len= len;
  if(c> 0xFFFF) ret.len+= 4;
  else          ret.len+= 2;
  ret.d= (char16_t *)new uint16_t[ret.len/ 2];

  // copy this
  for(p1= (uint16_t *)ret.d, p2= (uint16_t *)d; *p2; )
    *p1++= *p2++;

  // add new unicode
  if(c> 0xFFFF) {
    *p1++= (uint16_t)(UTF16_LEAD_OFFSET+ (c>> 10));
    *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
  } else
    *p1++= (uint16_t)c;
  *p1= 0;         /// str terminator

  return ret;
}






// OPERATOR += //
///-----------///

// this one is heavily used
str16 &str16::operator+=(const str16 &s) {
  if(!s.len) return *this;              /// other str16 empty -> return *this
  if(!len) return *this= s;             /// current str16 empty -> just copy other str16
  if(this== &s) return *this+= str16(*this);

  uint16_t *p1, *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- (len/ 2);      /// maximum copy amount
    p1= (uint16_t *)d+ ((len- 2)/ 2);
    p2= (uint16_t *)s.d;
    while(*p2 && n--> 0) {
      if(!Str::isLowSurrogate(*p2))
        nrUnicodes++;
      *p1++= *p2++;
    }
    *p1= 0;
    if(isHighSurrogate(*(p1- 1))) *--p1= 0, nrUnicodes--;    /// cannot end with only one surrogate
    /// update vars
    len= (int32_t)(p1- (uint16_t *)d+ 1)* 2;

  } else {
    /// update vars
    nrUnicodes+= s.nrUnicodes;
    len+= s.len- 2;
    uint16_t *p= new uint16_t[len/ 2];   /// new buffer alloc

    /// copy old string
    for(p1= p, p2= (uint16_t *)d; *p2; )
      *p1++= *p2++;

    delete[] d;
    d= (char16_t *)p;

    /// add new string
    p2= (uint16_t *)s.d;
    while((*p1++= *p2++));
  }

  return *this;
}


str16 &str16::operator+=(const str32 &s) { return *this+= s.d; }
str16 &str16::operator+=(const str8 &s)  { return *this+= s.d; }


str16 &str16::operator+=(const char16_t *s) {
  if(!s) return *this;          /// other str32 empty -> return *this
  if(!len) return *this= s;     /// current str32 empty -> just copy other str32

  uint16_t *p1, *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- (len/ 2);   /// maximum copy amount
    p1= (uint16_t *)d+ ((len- 2)/ 2);
    p2= (uint16_t *)s;
    while(*p2 && (n--> 0)) {
      if(!Str::isLowSurrogate(*p2))
        nrUnicodes++;
      *p1++= *p2++;
    }
    *p1= 0;
    if(isHighSurrogate(*(p1- 1))) *(--p1)= 0, nrUnicodes--;    /// cannot end with only one surrogate
    /// update vars
    len= (int32_t)(p1- (uint16_t *)d+ 1)* 2;

  } else {
    /// update vars
    int32_t nsurrogates= 0;
    for(p1= (uint16_t *)s; *p1; p1++)
      if(Str::isHighSurrogate(*p1))
        nsurrogates++;

    nrUnicodes+= (int32_t)(p1- (uint16_t *)s- nsurrogates);
    len+= (int32_t)((uint8_t *)p1- (uint8_t *)s);

    /// alloc
    uint16_t *p= new uint16_t[len/ 2];
    
    /// copy old string
    p1= p, p2= (uint16_t *)d;
    while(*p2)
      *p1++= *p2++;

    delete[] d;
    d= (char16_t *)p;

    /// add new string
    p2= (uint16_t *)s;
    while((*p1++= *p2++));
  }

  return *this;
}


str16 &str16::operator+=(const char32_t *s) {
  if(!s) return *this;                /// other str empty -> return *this
  if(!len) return *this= s;           /// current str empty -> just copy other str

  uint16_t *p1, *p3;
  uint32_t *p2;

  if(wrapping) {
    // copy
    int32_t n= wrapSize- (len/ 2); /// maximum copy amount
    p1= (uint16_t *)d+ (len/ 2)- 1;
    for(p2= (uint32_t *)s; *p2; p2++) {
      if(*p2> 0xFFFF) {
        if((n-= 2)< 0) break;
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (*p2>> 10));
        *p1++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
      } else {
        if((n-= 1)< 0) break;
        *p1++= (uint16_t)*p2;
      }
      nrUnicodes++;
    }
    *p1= 0;

    len+= (int32_t)(p1- (uint16_t *)d+ 1)* 2;

  } else {
    /// update internal vars
    for(p2= (uint32_t *)s; *p2; p2++) {
      if(*p2> 0xFFFF) len+= 4;
      else            len+= 2;
      nrUnicodes++;
    }
    /// alloc
    uint16_t *p= new uint16_t[len/ 2];

    /// copy old string
    p1= p, p3= (uint16_t *)d;
    while(*p3)
      *p1++= *p3++;

    delete[] d;
    d= (char16_t *)p;

    // add new string
    for(p2= (uint32_t *)s; *p2; p2++)
      if(*p2> 0xFFFF) {
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (*p2>> 10));
        *p1++= (uint16_t)(0xDC00+ (*p2& 0x3FF));
      } else
        *p1++= (uint16_t)*p2;
    *p1= 0;         /// str terminator
  }

  return *this;
}



str16 &str16::operator+=(const char *s) {
  if(!s) return *this;          /// other str32 empty -> return *this
  if(!len) return *this= s;     /// current str32 empty -> just copy other str32

  uint16_t *p1, *p3;
  uint8_t *p2;
  uint32_t c;

  if(wrapping) {
    // add new string
    int32_t n= wrapSize- (len/ 2);  /// maximum amount to copy
    p1= (uint16_t *)d+ (len/ 2)- 1; /// set pointer to the end of the current string
    p2= (uint8_t *)s;
    while(*p2) {
      int32_t l;
      if(*p2 < 128) { if((n-= 1)< 0) break; *p1++= *p2++; continue; }
      else if((*p2& 0xe0) == 0xc0) l= 2, c= (*p2++)& 0x1f;
      else if((*p2& 0xf0) == 0xe0) l= 3, c= (*p2++)& 0x0f;
      else if((*p2& 0xf8) == 0xf0) l= 4, c= (*p2++)& 0x07;
      else if((*p2& 0xfc) == 0xf8) { p2+= 5; continue; }
      else if((*p2& 0xfe) == 0xfc) { p2+= 6; continue; }
      else { p2++; continue; }
      while(--l> 0)       /// unpack continuation bytes
        c<<= 6, c+= *p2++ & 0x3f;

      if(c> 0xFFFF) {
        if((n-= 2)< 0) break;     /// buffer overflow check
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (c>> 10));
        *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
      } else {
        if((n-= 1)< 0) break;     /// buffer overflow check
        *p1++= (uint16_t)c;
      }
      nrUnicodes++;               /// update nrUnicodes
    }
    *p1= 0;                       /// str terminator

    len= (int32_t)(p1- (uint16_t *)d+ 1)* 2;  /// update len

  } else {
    /// update internal vars
    for(p2= (uint8_t *)s; *p2; ) {
      if(*p2< 128)                 p2++,   len+= 2;
      else if((*p2& 0xe0) == 0xc0) p2+= 2, len+= 2;
      else if((*p2& 0xf0) == 0xe0) p2+= 3, len+= 2;
      else if((*p2& 0xf8) == 0xf0) p2+= 4, len+= 4;
      else if((*p2& 0xfc) == 0xf8) { p2+= 5; continue; }
      else if((*p2& 0xfe) == 0xfc) { p2+= 6; continue; }
      else { p2++; continue; }
      nrUnicodes++;
    }
    uint16_t *p= new uint16_t[len/ 2];

    /// copy old string
    for(p1= p, p3= (uint16_t *)d; *p3; )
      *p1++= *p3++;

    delete[] d;
    d= (char16_t *)p;

    // add new string
    for(p2= (uint8_t *)s; *p2; ) {
      /// check how many chars this utf8 pack has
      int l;
      if(*p < 128) { *p1++= *p2++; continue; }
      else if((*p& 0xe0) == 0xc0) l= 2, c= (*p++)& 0x1f;
      else if((*p& 0xf0) == 0xe0) l= 3, c= (*p++)& 0x0f;
      else if((*p& 0xf8) == 0xf0) l= 4, c= (*p++)& 0x07;
      else if((*p& 0xfc) == 0xf8) { p2+= 5; continue; }
      else if((*p& 0xfe) == 0xfc) { p2+= 6; continue; }
      else { p2++; continue; }
      while(--l> 0)         /// unpack continuation bytes
        c<<= 6, c+= *p2++ & 0x3f;

      if(c> 0xFFFF) {
        *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (c>> 10));
        *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
      } else
        *p1++= (uint16_t)c;
    }
    *p1= 0;
  }

  return *this;
}


str16 &str16::operator+=(char32_t c) {
  if(!c) return *this;
  if(!len) return *this= c;

  int32_t n= (c> 0xFFFF? 2: 1);       /// will hold number of int16 the unicode value will pack into UTF-16

  if(wrapping) {
    if((len/ 2)+ n> wrapSize) return *this;   /// buffer overflow

    uint16_t *p= (uint16_t *)d+ (len/ 2)- 1;  /// set a pointer to end of string

    Str::utf32to16(c, (char16_t *)p); /// write new char
    p[n]= 0;                          /// str terminator

    len+= n* 2, nrUnicodes++;         /// update vars

  } else {
    /// update vars
    nrUnicodes++;
    len+= n* 2;
    uint16_t *p= new uint16_t[len/ 2];

    /// copy old string and add unival
    uint16_t *p1= p, *p2= (uint16_t *)d;
    while(*p2)
      *p1++= *p2++;

    // write new char + str terminator
    Str::utf32to16(c, (char16_t *)p1), p1+= n;
    *p1= 0;

    delete[] d;
    d= (char16_t *)p;
  }

  return *this;
}





// CONVERSION operators //
///--------------------///



// convert to UTF-8
// returned string is part of the class, it will be auto deallocated. Use getUTF8() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char *str16::convert8(int32_t *out_len) {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  uint16_t *p2;
  uint32_t c;

  /// length of d8
  int32_t l= 0;
  for(p2= (uint16_t *)d; *p2; ) { /// for each character in d
    if(isHighSurrogate(*p2))
      c= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c= *p2++;

    if     (c<= 0x0000007F) l++;
    else if(c<= 0x000007FF) l+= 2;
    else if(c<= 0x0000FFFF) l+= 3;
    else if(c<= 0x001FFFFF) l+= 4;
  }
  l++;

  /// create d8
  if(d8) delete[] d8;
  d8= (char *)new uint8_t[l];

  // UTF16 to UTF8 convert
  uint8_t *p= (uint8_t *)d8;
  for(p2= (uint16_t *)d; *p2; ) {
    /// UTF-16 to unicode32
    if(isHighSurrogate(*p2))
      c= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c= *p2++;

    /// unicode32 to UTF-8
    if       (c<= 0x0000007F) {         // 1 bytes
      *p++= (uint8_t) c;
    } else if(c<= 0x000007FF) {         // 2 bytes
      *p++= (uint8_t) (c>> 6)        | 0xC0;
      *p++= (uint8_t) (c&       0x3f)| 0x80;
    } else if(c<= 0x0000FFFF) {         // 3 bytes
      *p++= (uint8_t) (c>> 12)       | 0xE0;
      *p++= (uint8_t)((c>> 6)&  0x3F)| 0x80;
      *p++= (uint8_t) (c&       0x3F)| 0x80;
    } else if(c<= 0x001FFFFF) {         // 4 bytes
      *p++= (uint8_t) (c>> 18)       | 0xF0;
      *p++= (uint8_t)((c>> 12)& 0x3F)| 0x80;
      *p++= (uint8_t)((c>>  6)& 0x3F)| 0x80;
      *p++= (uint8_t) (c&       0x3F)| 0x80;
    }
  }
  *p= 0;                /// str terminator

  if(out_len) *out_len= l;
  return d8;
}


// convert to UTF-16
// returned string is part of the class, it will be auto deallocated. Use getUTF16() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char32_t *str16::convert32(int32_t *out_len) {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  /// create d16
  if(d32) delete[] d32;
  d32= new char32_t[nrUnicodes+ 1];

  // UTF-32 to UTF-16 copy
  uint32_t *p1= (uint32_t *)d32;
  uint16_t *p2= (uint16_t *)d;

  while(*p2)
    if(isHighSurrogate(*p2))
      *p1++= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      *p1++= *p2++;
  *p1= 0;

  if(out_len) *out_len= (nrUnicodes+ 1)* 4;
  return d32;
}


// convert to UTF-8
// similar to convert8() but returned string is released, and not part of the class 
//    it must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char *str16::getUTF8(int32_t *out_len) const {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  uint16_t *p2;
  uint32_t c;

  /// length of d8
  int32_t l= 0;
  for(p2= (uint16_t *)d; *p2; ) { /// for each character in d
    if(isHighSurrogate(*p2))
      c= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c= *p2++;

    if     (c<= 0x0000007F) l++;
    else if(c<= 0x000007FF) l+= 2;
    else if(c<= 0x0000FFFF) l+= 3;
    else if(c<= 0x001FFFFF) l+= 4;
  }
  l++;

  /// create the buffer that will hold the converted string
  uint8_t *buf8= new uint8_t[l];

  // UTF16 to UTF8 convert
  uint8_t *p= buf8;
  for(p2= (uint16_t *)d; *p2; ) {
    /// UTF-16 to unicode32
    if(isHighSurrogate(*p2))
      c= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c= *p2++;

    /// unicode32 to UTF-8
    if       (c<= 0x0000007F) {         // 1 bytes
      *p++= (uint8_t) c;
    } else if(c<= 0x000007FF) {         // 2 bytes
      *p++= (uint8_t) (c>> 6)        | 0xC0;
      *p++= (uint8_t) (c&       0x3f)| 0x80;
    } else if(c<= 0x0000FFFF) {         // 3 bytes
      *p++= (uint8_t) (c>> 12)       | 0xE0;
      *p++= (uint8_t)((c>> 6)&  0x3F)| 0x80;
      *p++= (uint8_t) (c&       0x3F)| 0x80;
    } else if(c<= 0x001FFFFF) {         // 4 bytes
      *p++= (uint8_t) (c>> 18)       | 0xF0;
      *p++= (uint8_t)((c>> 12)& 0x3F)| 0x80;
      *p++= (uint8_t)((c>>  6)& 0x3F)| 0x80;
      *p++= (uint8_t) (c&       0x3F)| 0x80;
    }
  }
  *p= 0;                /// str terminator

  if(out_len) *out_len= l;
  return (char *)buf8;
}

// convert to UTF-16
// similar to convert16() but returned string is released, and not part of the class 
//    it must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char32_t *str16::getUTF32(int32_t *out_len) const {
  if((!len) || (!nrUnicodes) || (!d)) { if(out_len) *out_len= 0; return NULL; }

  char32_t *buf32= new char32_t[nrUnicodes+ 1];

  // UTF-32 to UTF-16 copy
  char32_t *p1= buf32;
  char16_t *p2= d;
  while(*p2)
    if(isHighSurrogate(*p2))
      *p1++= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      *p1++= *p2++;
  *p1= 0;

  if(out_len) *out_len= (nrUnicodes+ 1)* 4;
  return buf32;
}


// returns string as UTF-8 in <out_buf>;
// please specify buffer size in <in_bufSize>;
// <out_nrUnicodes>, <out_len> (in bytes includes str terminator) - optional return values
void str16::convert8static(char *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if((!out_buf) || (in_bufSize<= 1)) return;
  if((!len) || (!nrUnicodes) || (!d)) { *(uint8_t *)out_buf= 0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(out_len) *out_len= 0; if(in_bufSize> 0) *(uint8_t *)out_buf= 0; return; }

  int32_t l= 0, u= 0;

  // UTF16 to UTF8 copy
  uint8_t *p1= (uint8_t *)out_buf;
  uint16_t *p2= (uint16_t *)d;
  uint32_t c;
  int32_t m= in_bufSize- 1;

  while(*p2) {
    if(isHighSurrogate(*p2))
      c= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c= *p2++;

    int32_t a= -1;

    /// compress unicode value into UTF-8
    if(c<= 0x0000007F) {
      if((l+= 1)> m) { l-= 1; break; }
      *p1++= (uint8_t) c, u++; continue;

    } else if(c<= 0x000007FF) {
      if((l+= 2)> m) { l-= 2; break; }
      *p1++= (uint8_t) (c>> 6) | 0xC0, a= 0;

    } else if(c<= 0x0000FFFF) {
      if((l+= 3)> m) { l-= 3; break; }
      *p1++= (uint8_t) (c>> 12)| 0xE0, a= 6;

    } else if(c<= 0x001FFFFF) {
      if((l+= 4)> m) { l-= 4; break; }
      *p1++= (uint8_t) (c>> 18)| 0xF0, a= 12;

    } else continue;

    while(a>= 0)
      *p1++= (uint8_t)((c>>  a)& 0x3f)| 0x80, a-= 6;

    u++;
  }
  *p1= 0;

  if(out_len) *out_len= l+ 1;
  if(out_nrUnicodes) *out_nrUnicodes= u;
}


// returns string as UTF-16 in <out_buf>;
// please specify buffer size in <in_bufSize>;
// <out_nrUnicodes>, <out_len> (in bytes includes str terminator) - optional return values
void str16::convert32static(char32_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if((!out_buf) || (in_bufSize<= 1)) return;
  if((!len) || (!nrUnicodes) || (!d)) { *out_buf=0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(out_len) *out_len= 0; if(in_bufSize> 0) *out_buf= 0; return; }

  int32_t l= 0;

  // UTF32 to UTF8 copy
  uint32_t *p1= (uint32_t *)out_buf;
  uint16_t *p2= (uint16_t *)d;
  int32_t m= in_bufSize- 1;
  while(*p2 && (m--> 0)) {
    if(isHighSurrogate(*p2))
      *p1++= (*p2<< 10)+ *(p2+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      *p1++= *p2++;
    l++;
  }
  *p1= 0;

  if(out_len) *out_len= (l+ 1)* 4;
  if(out_nrUnicodes) *out_nrUnicodes= l;
}







// whole string conversion to lowercase
void str16::lower() {
  // 25/10/2016: no surrogate pair changes case in non-surrogate unicode - this func is based on this
  if((!len) || (!d)) return;
  /// lowercase each character
  int32_t nint16;
  for(uint16_t *p= (uint16_t *)d; *p;) {
    uint32_t c= Str::utf16to32((char16_t *)p, &nint16);
    uint32_t lowered= (uint32_t)tolower(c);
    if(lowered!= c)
      Str::utf32to16(c, (char16_t *)p);
    p+= nint16;
  }
}


// whole string conversion to uppercase
void str16::upper() {
  // 25/10/2016: no surrogate pair changes case in non-surrogate unicode - this func is based on this
  if((!len) || (!d)) return;
  /// uppercase each character
  int32_t nint16;
  for(uint16_t *p= (uint16_t *)d; *p;) {
    uint32_t c= Str::utf16to32((char16_t *)p, &nint16);
    uint32_t up= toupper(c);
    if(up!= c)
      nint16= Str::utf32to16(c, (char16_t *)p);
    p+= nint16;
  }
}



///------------------------///
// OPERATOR - / OPERATOR -= // clears n chars from strings
///------------------------///

// cuts n unicode values from the end of this string
str16 &str16::operator-=(int n) {
  if(!n) return *this;
  if(nrUnicodes- n<= 0) { delData(); return *this; }
  
  nrUnicodes-= n;

  uint16_t *p= (uint16_t *)d+ (len/ 2)- 1;
  while(n--)
    if(isLowSurrogate(*(p- 1)))
      p-= 2, len-= 4;
    else
      p--, len-= 2;
  *p= 0;               // cut string with a terminator

  if(!wrapping) {     /// alloc new string and copy from old
    p= (uint16_t *)d;
    d= (char16_t *)new uint16_t[len/ 2];

    uint16_t *p1= (uint16_t *)d, *p2= p;
    while((*p1++= *p2++));

    delete[] p;
  }

  return *this;
}

// returns a string with n less characters
str16 str16::operator-(int n) const {
  /// basic checks
  if(!n) return str16(*this);
  if(nrUnicodes- n<= 0) return str16();

  /// temporary cut the string
  uint16_t *p= (uint16_t *)d+ (len/ 2)- 1;  /// move to the end of the string
  while(n--)
    if(isLowSurrogate(*(p- 1)))
      p-= 2;
    else
      p--;

  uint16_t x= *p;     /// x will hold the value that the str terminator was placed over
  *p= 0;              /// cut string with a terminator

  str16 ret(d);        // return value

  *p= x;              /// restore *this

  return ret;
}



///-----------///
// OPERATOR == //
///-----------///

bool str16::operator==(const str16 &s) const {
  /// same number of characters
  if(s.nrUnicodes!= nrUnicodes) return false;
  if(!nrUnicodes) return true;
  /// pass thru all characters
  for(uint16_t *p1= (uint16_t *)d, *p2= (uint16_t *)s.d; *p1 && *p2; p1++, p2++)
    if(*p1!= *p2)
      return false;

  // this point reached -> there is no difference between strings
  return true;
}


bool str16::operator==(const str32 &s) const { return operator==(s.d); }
bool str16::operator==(const str8 &s) const { return operator==(s.d); }


bool str16::operator==(const char16_t *s) const {
  /// if both are NULL, then there's no difference
  if(s== NULL) {
    if(d== NULL) return true;
    else         return false;
  }
  if(d== NULL) return false;

  /// pass thru all characters
  uint16_t *p1= (uint16_t *)d, *p2= (uint16_t *)s;
  while(*p1 && *p2)
    if(*p1++ != *p2++)
      return false;
  if((*p1) || (*p2)) return false;  /// anything remains in any string -> they're not the same

  // this point reached -> there is no difference between strings
  return true;
}


bool str16::operator==(const char32_t *s) const {
  /// if both are NULL, then there's no difference
  if(s== NULL) {
    if(d== NULL) return true;
    else         return false;
  }
  if(d== NULL) return false;

  /// pass thru each character to check for differences
  uint16_t *p1= (uint16_t *)d;
  uint32_t *p2= (uint32_t *)s;
  uint32_t c;

  while(*p1 && *p2) {
    if(isHighSurrogate(*p1))
      c= (*p1<< 10)+ *(p1+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else 
      c= *p1++;

    if(c != *p2++)
      return false;
  }
  /// if there's still text in either one of the strings, there is a difference
  if(*p1 || *p2) return false;
  // reached this point, there is no difference
  return true;
}


// checks if strings are identical (UTF-8)
bool str16::operator==(const char *s) const {
  /// if both are NULL, then there's no difference
  if(s== NULL) {
    if(d== NULL) return true;
    else         return false;
  }
  if(d== NULL) return false;

  /// pass thru each character to check for differences
  uint16_t *p1= (uint16_t *)d;
  uint8_t *p2= (uint8_t *)s;
  uint32_t c1, c2;

  while(*p1 && *p2) {
    /// UTF-16 unicode value (utf16to32 is way slower)
    if(isHighSurrogate(*p1))
      c1= (*p1<< 10)+ *(p1+ 1)+ UTF16_SURROGATE_OFFSET, p2+= 2;
    else 
      c1= *p1++;

    /// UTF-8 unicode value (utf8to32 is way slower)
    int32_t n = 0;
    if (*p2 < 128)                n= 1, c2= *p2++;
    else if((*p2 & 0xe0) == 0xc0) n= 2, c2= (*p2++) & 0x1f;
    else if((*p2 & 0xf0) == 0xe0) n= 3, c2= (*p2++) & 0x0f;
    else if((*p2 & 0xf8) == 0xf0) n= 4, c2= (*p2++) & 0x07;
    else c2= 0xFFFD, p2++;
    
    while (--n> 0)        /// unpack continuation bytes
      c2<<= 6, c2+= *p2++ & 0x3f;

    // character compare
    if(c1 != c2)
      return false;
  }
  /// if there's still text in either one of the strings, there is a difference
  if(*p1 || *p2) return false;
  // reached this point, there is no difference
  return true;
}


bool str16::operator==(char32_t c) const {
  if(!c) {
    if(!d) return true;
    else   return false;
  }
  if(nrUnicodes!= 1) return false;
  return (Str::utf16to32(d)== c? true: false);
}








///-----------------------------------------///
// COMBining diacritical character functions //
///-----------------------------------------///


// removes all combining diacriticals from the string
void str16::clearCombs() {
  if((!nrUnicodes) || (!d) || (!len)) return;

  // pass thru string, copying each char but skipping diacriticals 
  uint16_t *p1= (uint16_t *)d, *p2= (uint16_t *)d;
  uint32_t c;
  int32_t nint16;
  while(*p2) {
    c= Str::utf16to32((char16_t *)p2, &nint16), p2+= nint16;
    if(isComb(c))             /// if it's a diacritical, skip it, update vars
      len-= nint16* 2, nrUnicodes--;
    else                      /// if it's not a diacritical, copy it
      Str::utf32to16(c, (char16_t *)p1), p1+= nint16;
  }

  *p1= 0;

  /* to be or not to be - just clears mem of garbage, if any, but very little... doesn't worth it imho
  if(!wrapping) {
    uint16_t *p= (uint16_t *)d;
    d= (char16_t *)new uint16_t[len/ 2];
    Str::strcpy16(d, (char16_t *)p);
    delete[] p;
  }
  */
}

// returns the number of combining diacriticals (if any) in string
int32_t str16::nrCombs() const {
  if(!d) return 0;
  
  int32_t ret= 0;
  // BASED ON THE FACT THAT SURROGATES CANNOT BE DIACRITICALS 26.10.2015
  for(uint16_t *p= (uint16_t *)d; *p; p++)
    if(isComb(*p))
      ret++;

  return ret;
}


// returns the number of characters in string, WHITOUT the number of combining diacriticals
int32_t str16::nrChars() const {
  if(!d) return 0;

  int32_t ret= nrUnicodes;
  // BASED ON THE FACT THAT SURROGATES CANNOT BE DIACRITICALS 26.10.2015
  for(uint16_t *p= (uint16_t *)d; *p;)
    if(isComb(*p))
      ret--;

  return ret;
}







///--------------------------------///
// UTF-8 SECURITY / READ FROM FILES // (any read from a file should be considered UNSAFE)
///--------------------------------///


// bad characters will be MARKED with 0xFFFD

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 string
str16 &str16::secureUTF8(const char *s, int32_t in_len) {
  delData();
  if(s== NULL) return *this;
  if(in_len< 0) return *this;

  /// length in unicode vals
  int32_t n= 0;
  const uint8_t *p= (const uint8_t *)s;
  bool bad, usingLimit= false;
  int32_t limit;
  uint32_t c;

  if(in_len) {
    usingLimit= true;
    limit= in_len;
  }

  // can't assume that every character is ok, there can be garbage @ every level
  for(; *p; p++) {
    if(usingLimit) if(limit--<= 0) break; /// at least one byte must be able to be read

    if(*p < 128) n++;                 /// direct character read             (1 byte total)
    // search for headers, everything else is ignored.
    else if((*p& 0xe0) == 0xc0) n++;  /// UTF8 header+ 1 continuation byte  (2 bytes total)
    else if((*p& 0xf0) == 0xe0) n++;  /// UTF8 header+ 2 continuation bytes (3 bytes total)
    else if((*p& 0xf8) == 0xf0) n+= 2;  /// UTF8 header+ 3 continuation bytes (4 bytes total)
    else if((*p& 0xfc) == 0xf8) n+= 2;  /// UTF8 header+ 4 continuation bytes (5 bytes total)  marked as bad
    else if((*p& 0xfe) == 0xfc) n+= 2;  /// UTF8 header+ 5 continuation bytes (6 bytes total)  marked as bad
    // THERE'S THE OPTION FOR AN UNREADABLE CHAR (else here) TO BE MARKED AS BAD, BUT n++ IS A MUST
  }
  if(n<= 0) return *this;

  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else {
    if(d) delete[] d;
    /// if a char that would pack into a surrogate pair is bad, it is marked FFFD,
    /// so the 4bytes that that char would occupy would turn into 2.
    /// this could create some garbage (alloc more than is needed) but the amount should be insignifient
    d= (char16_t *)new uint16_t[n+ 1];                /// UNPACKED STRING
  }
  

  limit= in_len;
  p= (const uint8_t *)s;
  uint16_t *u= (uint16_t *)d;
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
      if(usingLimit) if((limit-= 2)< 0) { u[a++]= 0xFFFD; break; }

      // test for overlong bytes                1100000x (10xxxxxx)
      if((*p& 0x1e) == 0) {             /// 1e= 00011110 if these 4 bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 1;       /// update limit, only one byte [of 2] was processed
        continue;
      }
      c= *p++& 0x1f;                    /// byte seems ok - copy from it (0x1f= 00011111)
      contBytes= 1;

    /// character has 3 bytes
    } else if((*p& 0xf0) == 0xe0) {       /// header for 3 bytes ? (0xf0= 11110000)
      // check for string limit reach; the string must have at least 3 bytes left for this char
      if(usingLimit) if((limit-= 3)< 0) { u[a++]= 0xFFFD; break; }

      // test for overlong bytes                11100000 100xxxxx (10xxxxxx) 
      if(     ((*p& 0x0f) == 0) &&      ///  f= 00001111          <--
         ((*(p+ 1)& 0x20) == 0)) {      /// 20=          00100000 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 2;       /// update limit, only one byte [of 3] was processed
        continue;
      }
      c= *p++& 0x0f;                    /// byte seems ok - copy from it (0x0f= 00001111)
      contBytes= 2;

    /// character has 4 bytes
    } else if((*p& 0xf8) == 0xf0) {     /// header for 4 bytes ? (0xf8= 11111000)
      // check for string limit reach; the string must have at least 4 bytes left for this char
      if(usingLimit) if((limit-= 4)< 0) { u[a++]= 0xFFFD; break; }

      // test for overlong bytes                11110000 1000xxxx (10xxxxxx 10xxxxxx) 
      if(      ((*p& 0x7) == 0) &&      ///  7= 00000111          <--
         ((*(p+ 1)& 0x30) == 0)) {      /// 30=          00110000 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 3;       /// update limit, only one byte [of 4] was processed
        continue;
      }
      c= *p++& 0x07;                    /// byte seems ok - copy from it (0x07= 00000111)
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
      if(usingLimit) if((limit-= 5)< 0) { u[a++]= 0xFFFD; break; }

      // test for overlong bytes                11111000 10000xxx (10xxxxxx 10xxxxxx 10xxxxxx) 
      if(     (*p& 0x3  == 0) &&        ///  3= 00000011          <--
         (*(p+ 1)& 0x38 == 0)) {        /// 38=          00111000 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 4;       /// update limit, only one byte [of 5] was processed
        continue;
      }
      c= *p++& 0x03;                    /// byte seems ok - copy from it (0x03= 00000011)
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
      if(usingLimit) if((limit-= 6)< 0) { u[a++]= 0xFFFD; break; }

      // test for overlong bytes                11111100 100000xx (10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx)
      if(     (*p& 0x1  == 0) &&        ///  1= 00000001          <--
         (*(p+ 1)& 0x3C == 0)) {        /// 3C=          00111100 <--if these bits are 0, this is a overlong byte
        u[a++]= 0xFFFD;
        p++;
        if(usingLimit) limit+= 5;       /// update limit, only one byte [of 6] was processed
        continue;
      }
      c= *p++& 0x01;                    /// byte seems ok - copy from it (0x01= 00000001)
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
      c<<= 6; c+= *p++ & 0x3f;        /// byte seems ok - copy from it (0x3f= 00111111)
    }
    if(bad) continue;

    /// test result character d[a]
    if(c>= 0x10FFFF)                 // limit of Unicode <--------------------- (maybe this changes in the future)
      c= 0xFFFD;

    if( (c>= 0xD800) && (c<= 0xDFFF) )  /// it can't be a utf-16 surrogate
      c= 0xFFFD;

    // further tests can be done here. there are still some chars that should be marked as malformed (0xFFFD) <<<=========================

    if(c> 0xFFFF) {
      if(a+ 1>= n) break;
      Str::utf32to16(c, (char16_t *)u+ a);
      a+= 2;
    } else
      u[a++]= (char16_t)c;
  }	/// for each character

  u[a]= 0;                              /// terminator

  return *this;
}



extern char32_t _secure16to32advance(const char16_t **in_str);



str16 &str16::secureUTF16(const char16_t *in_s, int32_t in_len) {
  delData();
  if(in_s== NULL) return *this;

  uint16_t *p1;
  uint32_t c;
  const uint16_t *p2;
  bool usingLimit= false;
  int32_t limit, n= 0;
  if(in_len) 
    usingLimit= true, limit= in_len;

  /// count nr of unicodes
  for(p2= (uint16_t *)in_s; *p2; ) {
    if(usingLimit) {
      if(isHighSurrogate(*p2)) {
        if((limit- 4)< 0) break;
      } else
        if((limit- 2)< 0) break;
    }

    c= _secure16to32advance((const char16_t **)&p2);
    limit= in_len- (int32_t)((uint8_t *)p2- (uint8_t *)in_s);

    if(c> 0xFFFF) n+= 2;
    else          n++;
  }
  if(n<= 0) return *this;

  /// wrapping / non wrapping branch
  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else {
    if(d) delete[] d;
    d= (char16_t *)new uint16_t[n+ 1];
  }

  /// internal vars
  // nrUnicodes= n; safer to use updateLen()
  // len= (n+ 1)* 4;

  // safe copy from source
  limit= in_len;
  p1= (uint16_t *)d, p2= (uint16_t *)in_s;
  while(n> 0) {
    if(usingLimit) {
      if(isHighSurrogate(*p2)) { if((limit- 4)< 0) break; }
      else                       if((limit- 2)< 0) break; }

    c= _secure16to32advance((const char16_t **)&p2);
    limit= in_len- (int32_t)((uint8_t *)p2- (uint8_t *)in_s);

    if(c> 0xFFFF) {
      if((n-= 2)< 0) break;
      *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (c>> 10));
      *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
    } else
      *p1++= (uint16_t)c, n--;
  } /// for each char/comb in s
  *p1= 0;                                   /// str terminator
  updateLen();

  return *this;
}



/// internal func that is used for secureUTF32
extern char32_t _secure32check(char32_t);

// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf32 string
str16 &str16::secureUTF32(const char32_t *in_s, int32_t in_limit) {
  delData();
  if(in_s== NULL) return *this;

  uint16_t *p1;
  const uint32_t *p2;
  bool usingLimit= false;
  int32_t limit, n= 0;
  if(in_limit> 0) {
    limit= in_limit;
    usingLimit= true;
  }

  /// string length in int32 units (basically unicode values)
  for(p2= (uint32_t *)in_s; *p2; p2++) {
    if(usingLimit) { limit-= 4; if(limit< 0) break; }
    if(*p2> 0xFFFF) n+= 2;
    else            n++;
  }

  /// wrapping / non wrapping branch
  if(wrapping) {
    if(n> wrapSize- 1)
      n= wrapSize- 1;
  } else 
    d= new char16_t[n+ 1];

  // safe copy from UTF-32
  p1= (uint16_t *)d, p2= (uint32_t *)in_s;
  limit= in_limit;
  for(; n> 0; n--) {
    if(usingLimit) { limit-= 4; if(limit< 0) break; }
    uint32_t c= _secure32check(*p2++);
    if(c> 0xFFFF) {
      if((n-= 2)< 0) break;
      *p1++= (uint16_t)(Str::UTF16_LEAD_OFFSET+ (c>> 10));
      *p1++= (uint16_t)(0xDC00+ (c& 0x3FF));
    } else
      *p1++= (uint16_t)c, n--;
  }
  *p1= 0;          /// string terminator
  updateLen();

  return *this;
}







/*
// fopen knows of utf-8 but the win version wants to put a freakin BOM in the file, wich cause problems in linux, so file must be opened as binary

/// read all file                   (validates each char)
void str16::readUTF8(FILE *f) {
  // BOM:
  // UTF-16:  U+FEFF/ U+FFFE
  // UTF-8:   0xEF,0xBB,0xBF
  uint8 bom[3];
  long start= ftell(f), pos;
  if(fread(bom, 3, 1, f)) {
    if((bom[0]!= 0xEF) && (bom[1]!= 0xBB) && (bom[2]!= 0xBF)) <<THIS IS UTF8- CHECK FOR BOTH FEFF AND FFFE
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
  buffer[fs]= buffer[fs- 1]= 0;                  /// terminator

  secureUTF8((char *)buffer, fs+ 1);

  delete[] buffer;
}



/// read n characters from file     (validates each char)
void str16::readUTF8n(FILE *f, size_t n) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF) FFFE CAN BE TOO
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

  secureUTF8((char *)buffer);

  delete[] buffer;
}

/// read till end of line (or file) (validates each char)
void str16::readLineUTF8(FILE *f) {
  /// read / ignore the BOM in an UTF file
  uint16 bom;
  long pos= ftell(f);
  if(fread(&bom, 2, 1, f)) {
    if(bom!= 0xFEFF) FFFE CAN BE TOO
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

void str16::insert(char32_t in_unicode, int32_t in_pos) {
  int32_t l;
  if(wrapping) l= Str::insert16static(d, wrapSize, in_unicode, in_pos);
  else         l= Str::insert16(&d, in_unicode, in_pos);
  if(l> len) nrUnicodes++, len= l;
}


void str16::insertStr(const char16_t *in_str, int32_t in_pos) {
  if(wrapping)  Str::insertStr16static(d, wrapSize, in_str, in_pos);
  else          Str::insertStr16(&d, in_str, in_pos);
  updateLen();
}


void str16::del(int32_t in_nUnicodesToDel, int32_t in_pos) {
  // removing characters doesn't require to realloc memory, only very slight garbage remains in memory;
  // so del32static can be used instead of del32 if more speed is needed
  if(wrapping)  Str::del16static(d, in_nUnicodesToDel, in_pos);
  else          Str::del16(&d, in_nUnicodesToDel, in_pos);
  updateLen();
}










