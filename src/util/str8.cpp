#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

//#include <stdio.h>            // only for file handling... this might go
#include <stdarg.h>
#include "util/strCommon.h"
#include "util/str32.h"
#include "util/str16.h"
#include "util/str8.h"

// lower+upper can be done for chars that expand into multiple chars and vice-versa, easily
//       (2 funcs to check for these few cases, when counting how big the str is)
// there's another case that i think another word change when lowercasing/uppercasing...
//       this... i don't think is done easily... actually very hard


/* UTF 8 research
  -they say to start a file with a [0xEF,0xBB,0xBF], which is
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

//using namespace Str;


/// character case change helper structs (initialized at the back of the file)
struct _L2U {
  char32_t l, u;
};// _cl2u[], _cu2l[];



str8::str8(const str16 &s): str8() { *this= s.d; }
str8::str8(const str32 &s): str8() { *this= s.d; }



void str8::delData() {
  if(wrapping) *d= 0;
  else if(d) { delete[] d; d= NULL; }
  len= nrUnicodes= 0;
}

void str8::clean() {
  if(d16) { delete[] d16; d16= NULL; }
  if(d32) { delete[] d32; d32= NULL; }
}



///-----------------------------------///
// Wrap handling ==========----------- //
///-----------------------------------///


// CHANGES THE WAY THIS CLASS WORKS: uses specified buffer for every operation, no memory allocs are made.
// This buffer must be big enough for every operation made,
//   but critical errors will not occur if it's not big enough (the text will not be OK)
// in_buffer:         the buffer that the class will use or 'wrap' onto
// in_size:           the size of the buffer in uint32 units - this should be 2 minimum (1 for terminator, 1 for a character)
//                    if buffer size is 0, THE SIZE IS DETERMINED BY THE STRING SIZE INSIDE THE BUFFER.
void str8::wrap(const char *in_buffer, int32_t in_size) {
  if((in_buffer== NULL) || (in_size== 1)) return; /// a 1 char size cannot do anything. size 0 is str determined. minimum is 2

  if(wrapping) stopWrapping();      /// terminate current wrapping if there is one
  delData();

  d= (char *)in_buffer;

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
// d will be memory allocated, the custom buffer is not used in any way. IT IS NOT UNALLOCATED
void str8::stopWrapping() {
  d= NULL;
  wrapSize= 0;
  wrapping= false;
}



///----------------------------------------------///
// UTILITY FUNCTIONS, usually good for any string //
///----------------------------------------------///

void str8::updateLen() {
  len= nrUnicodes= 0;
  if(d== NULL) return;

  uint8_t *str= (uint8_t *)d;
  for(; *str; str++)
    if((*str& 0xC0)!= 0x80)   /// if not a continuation byte, it's a start of a new unicode value (if bits 11xxxxxx(0xC0) != 10xxxxxx (0x80). every continuation byte starts with 10xxxxxx )
      nrUnicodes++;

  len= (int32_t)(str- (uint8_t *)d+ 1);
}

//#undef uint
//#undef ushort
//#undef ulong
#include <mutex>
std::mutex m;
uint8_t fBuf[2048];



// waaait a minute, this format works now, but... locales? what happens in other os'es?
// a custom formatted IS REQUIRED, the std format just will not work

str8 &str8::f(const char *format, ...) {
  //m.lock();
	int32_t l= Str::strlen8(format);
	if(l> 2048) {
		*this= "TOO BIG, FORMAT FAILED";
		return *this;
	}
  /// create formatted text (use a buffer)
  fBuf[0]= '\0';
	va_list args;
  va_start(args, format);
  vsprintf((char *)fBuf, (const char *)format, args);
  va_end(args);

	return *this= (char *)fBuf;     // WRAP SAFE, BECAUSE OF operator=
  //m.unlock();
}




///---------///
// OPERATOR= //
///---------///


str8 &str8::operator=(const str8 &s) {
  if(!s.len) { delData(); return *this; }
  if(this== &s) return *this;   // same object assignment <<< DOES THIS HAVE ANY SENSE? I MEAN, IF YOU DO a= a you're already an idiot, right?
                                // let's say this is ok to check, there can be arrays of strings that are managed en masse...
                                // dunno, it could happen in a big program...

  bool overflow= false;         /// [wrapping only] start with false, mark true if buffer overflow

  /// wrapping a buffer
  if(wrapping) {
    if(s.len> wrapSize)
      overflow= true;

  /// using memory allocs
  } else if(len!= s.len) {
    if(d) delete[] d;
    d= (char *)new uint8_t[s.len];
  }

  nrUnicodes= s.nrUnicodes;
  len= s.len;

  // copy
  uint8_t *p1= (uint8_t *)d, *p2= (uint8_t *)s.d;

  if(overflow) {
    nrUnicodes= 0, len= 0;
    for(int32_t m= wrapSize- 1; *p2; ) {
      if((*p2& 0xC0)!= 0x80) {  /// if not a continuation byte, it's a start of a new unicode value (if bits 11xxxxxx(0xC0) != 10xxxxxx (0x80). every continuation byte starts with 10xxxxxx )

        /// check if the character fits inside the wrapped buffer
        if(len+ Str::utf8headerBytes(*p2)> m)   
          break;

        nrUnicodes++;
      }
      *p1++= *p2++, len++;
    }
    len++;                      /// space for str terminator
  } else
    while(*p2)
      *p1++= *p2++;

  *p1= 0;                       /// str terminator

  return *this;
}

str8 &str8::operator=(const str16 &s) {
  return *this= s.d;
}

str8 &str8::operator=(const str32 &s) {
  return *this= s.d;
}



// assign from UTF-8 string
str8 &str8::operator=(const char *s) {
  delData();
  if(!s) return *this;
  
  uint8_t *p1, *p2;

  // wrapping version
  if(wrapping) {
    int32_t n= wrapSize- 1;
    for(p1= (uint8_t *)d, p2= (uint8_t *)s; *p2; ) {
      if((*p2& 0xc0)!= 0x80) {     /// if not a continuation byte, it's a start of a new unicode value (if bits 11xxxxxx(0xC0) != 10xxxxxx (0x80). every continuation byte starts with 10xxxxxx )
        /// buffer overflow check
        int32_t l;
        if      (*p2 < 128)          l= 1;
        else if((*p2& 0xe0) == 0xc0) l= 2;
        else if((*p2& 0xf0) == 0xe0) l= 3;
        else if((*p2& 0xf8) == 0xf0) l= 4;
        else if((*p2& 0xfc) == 0xf8) l= 5;
        else if((*p2& 0xfe) == 0xfc) l= 6;
        else l= 1;

        if(len+ l> n)
          break;
        
        nrUnicodes++;
      }
      *p1++= *p2++, len++;
    }
    len++;

  // not wrapping version
  } else {
    len= Str::strlen8(s);
    d= (char *)new uint8_t[len];

    for(p1= (uint8_t *)d, p2= (uint8_t *)s; *p2; ) {
      if((*p2& 0xc0)!= 0x80)      /// if not a continuation byte, it's a start of a new unicode value (if bits 11xxxxxx(0xC0) != 10xxxxxx (0x80). every continuation byte starts with 10xxxxxx )
        nrUnicodes++;
      *p1++= *p2++;
    }
  }

  *p1= 0;      /// str terminator

  return *this;
}


// assign from UTF-16 str
str8 &str8::operator=(const char16_t *s) {
  delData();
  if(!s) return *this;
  
  uint8_t *p1;
  const uint16_t *p2;
  char32_t c;

  // wrapping version
  if(wrapping) {
    int32_t m= wrapSize- 1;

    for(p1= (uint8_t *)d, p2= (uint16_t *)s; *p2; ){
      /// unpack UTF-16 unicode and advance pointer
      if(Str::isHighSurrogate(*p2))
        c= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p2+= 2;
      else
        c= *p2,  p2++;
    
      /// compress unicode value into UTF-8
      if       (c<= 0x0000007F) { // 1 byte    U-00000000-U-0000007F:  0xxxxxxx
        if(len+ 1> m) break;
        len++, nrUnicodes++;
        *p1++= (uint8_t)  c;
      } else if(c<= 0x000007FF) { // 2 bytes   U-00000080-U-000007FF:  110xxxxx 10xxxxxx 
        if(len+ 2> m) break;
        len+= 2, nrUnicodes++;
        *p1++= (uint8_t) (c>> 6)        | 0xC0; /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
        *p1++= (uint8_t) (c&       0x3f)| 0x80; /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(c<= 0x0000FFFF) { // 3 bytes   U-00000800-U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
        if(len+ 3> m) break;
        len+= 3, nrUnicodes++;
        *p1++= (uint8_t) (c>> 12)       | 0xE0; /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
        *p1++= (uint8_t)((c>> 6)&  0x3F)| 0x80; /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (c&       0x3F)| 0x80; /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(c<= 0x001FFFFF) { // 4 bytes   U-00010000-U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
        if(len+ 4> m) break;
        len+= 4, nrUnicodes++;
        *p1++= (uint8_t) (c>> 18)       | 0xF0; /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
        *p1++= (uint8_t)((c>> 12)& 0x3F)| 0x80; /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((c>>  6)& 0x3F)| 0x80; /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (c&       0x3F)| 0x80; /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      }
    } /// for each unicode in source string

  // not wrapping version
  } else {
    /// string length in bytes
    for(p2= (uint16_t *)s; *p2;) {
      if(Str::isHighSurrogate(*p2))
        c= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p2+= 2;
      else
        c= *p2,  p2++;
      nrUnicodes++;
      len+= Str::utf8nrBytes(c);
    }
    len++;                        /// space for str terminator
    /// alloc mem
    d= (char *)new uint8_t[len];

    for(p1= (uint8_t *)d, p2= (uint16_t *)s; *p2; ) {
      /// unpack UTF-16 unicode and advance pointer
      if(Str::isHighSurrogate(*p2))
        c= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p2+= 2;
      else
        c= *p2,  p2++;

      p1+= Str::utf32to8(c, (char *)p1);
    } /// for each unicode in source string
  }
  *p1= 0;         /// string terminator

  return *this;
}




// assign from UTF-32 str
str8 &str8::operator=(const char32_t *s) {
  delData();
  if(!s) return *this;
  
  uint8_t *p1;
  const uint32_t *p2;

  // wrapping version
  if(wrapping) {
    int32_t m= wrapSize- 1;
    for(p1= (uint8_t *)d, p2= (uint32_t *)s; *p2; p2++) {
      /// compress unicode value into utf-8
      if       (*p2<= 0x0000007F) {      //  1 byte   U-00000000�U-0000007F:  0xxxxxxx 
        if((++len)> m) { --len; break; }
        *p1++= (uint8_t) *p2;
      } else if(*p2<= 0x000007FF) {     //  2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
        if((len+= 2)> m) { len-= 2; break; }
        *p1++= (uint8_t) (*p2>> 6)        | 0xC0;    /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
        *p1++= (uint8_t) (*p2&       0x3f)| 0x80;    /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(*p2<= 0x0000FFFF) {     //  3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
        if((len+= 3)> m) { len-= 3; break; }
        *p1++= (uint8_t) (*p2>> 12)       | 0xE0;    /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
        *p1++= (uint8_t)((*p2>> 6)&  0x3F)| 0x80;    /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3F)| 0x80;    /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(*p2<= 0x001FFFFF) {     // 4 bytes U-00010000->U-001FFFFF:    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
        if((len+= 4)> m) { len-= 4; break; }
        *p1++= (uint8_t) (*p2>> 18)       | 0xF0;    /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
        *p1++= (uint8_t)((*p2>> 12)& 0x3F)| 0x80;    /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>>  6)& 0x3F)| 0x80;    /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3F)| 0x80;    /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      // last 2 bytes, UNUSED by utf ATM, but there be the code
      } else if(*p2<= 0x03FFFFFF) {     //  5 bytes U-00200000->U-03FFFFFF:   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
        if((len+= 5)> m) { len-= 5; break; }
        *p1++= (uint8_t) (*p2>> 24)       | 0xF8;    /// [BYTE 1]      >> 24= 000000xx 00000000 00000000 00000000 00000000  then header | f8 (11111000)
        *p1++= (uint8_t)((*p2>> 18)& 0x3f)| 0x80;    /// [BYTE 2] >> 18 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>> 12)& 0x3f)| 0x80;    /// [BYTE 3] >> 12 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>>  6)& 0x3f)| 0x80;    /// [BYTE 4] >>  6 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3f)| 0x80;    /// [BYTE 5]       & 3f= 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(*p2<= 0x7FFFFFFF) {     //  6 bytes U-04000000->U-7FFFFFFF:   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if((len+= 6)> m) { len-= 6; break; }
        *p1++= (uint8_t) (*p2>> 30)       | 0xFC;    /// [BYTE 1]      >> 30= 0000000x 00000000 00000000 00000000 00000000 00000000  then header | fc (11111100)
        *p1++= (uint8_t)((*p2>> 24)& 0x3f)| 0x80;    /// [BYTE 2] >> 24 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>> 18)& 0x3f)| 0x80;    /// [BYTE 3] >> 18 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>> 12)& 0x3f)| 0x80;    /// [BYTE 4] >> 12 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>>  6)& 0x3f)| 0x80;    /// [BYTE 5] >>  6 & 3f= 00000000 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3f)| 0x80;    /// [BYTE 6]         3f= 00000000 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else continue;
      nrUnicodes++;
    } /// for each unicode value in source string
    len++;

  // not wrapping version
  } else {

    for(p2= (uint32_t *)s; *p2; p2++) {
      if     (*p2<= 0x0000007F) len++;    /// 1 byte  U-00000000->U-0000007F:  0xxxxxxx 
      else if(*p2<= 0x000007FF) len+= 2;  /// 2 bytes U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
      else if(*p2<= 0x0000FFFF) len+= 3;  /// 3 bytes U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
      else if(*p2<= 0x001FFFFF) len+= 4;  /// 4 bytes U-00010000->U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
      else if(*p2<= 0x03FFFFFF) len+= 5;  /// 5 bytes U-00200000->U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
      else if(*p2<= 0x7FFFFFFF) len+= 6;  /// 6 bytes U-04000000->U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    }
    nrUnicodes= (int32_t)(p2- (uint32_t *)s);
    len++;                          /// space for terminator
  
    d= (char *)new uint8_t[len];

    for(p1= (uint8_t *)d, p2= (uint32_t *)s; *p2; p2++)
      ///  p1+= Str::utf32to8(*p2, p1);
      if       (*p2<= 0x0000007F) {     //  1 byte   U-00000000->U-0000007F:  0xxxxxxx 
        *p1++= (uint8_t) *p2;
      } else if(*p2<= 0x000007FF) {     //  2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
        *p1++= (uint8_t) (*p2>> 6)        | 0xC0;    /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
        *p1++= (uint8_t) (*p2&       0x3f)| 0x80;    /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(*p2<= 0x0000FFFF) {     //  3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
        *p1++= (uint8_t) (*p2>> 12)       | 0xE0;    /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
        *p1++= (uint8_t)((*p2>> 6)&  0x3F)| 0x80;    /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3F)| 0x80;    /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(*p2<= 0x001FFFFF) {     // 4 bytes U-00010000->U-001FFFFF:    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
        *p1++= (uint8_t) (*p2>> 18)       | 0xF0;    /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
        *p1++= (uint8_t)((*p2>> 12)& 0x3F)| 0x80;    /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>>  6)& 0x3F)| 0x80;    /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3F)| 0x80;    /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      // last 2 bytes, UNUSED by utf ATM, but there be the code
      } else if(*p2<= 0x03FFFFFF) {     //  5 bytes U-00200000->U-03FFFFFF:   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
        *p1++= (uint8_t) (*p2>> 24)       | 0xF8;    /// [BYTE 1]      >> 24= 000000xx 00000000 00000000 00000000 00000000  then header | f8 (11111000)
        *p1++= (uint8_t)((*p2>> 18)& 0x3f)| 0x80;    /// [BYTE 2] >> 18 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>> 12)& 0x3f)| 0x80;    /// [BYTE 3] >> 12 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>>  6)& 0x3f)| 0x80;    /// [BYTE 4] >>  6 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3f)| 0x80;    /// [BYTE 5]       & 3f= 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else if(*p2<= 0x7FFFFFFF) {     //  6 bytes U-04000000->U-7FFFFFFF:   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *p1++= (uint8_t) (*p2>> 30)       | 0xFC;    /// [BYTE 1]      >> 30= 0000000x 00000000 00000000 00000000 00000000 00000000  then header | fc (11111100)
        *p1++= (uint8_t)((*p2>> 24)& 0x3f)| 0x80;    /// [BYTE 2] >> 24 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>> 18)& 0x3f)| 0x80;    /// [BYTE 3] >> 18 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>> 12)& 0x3f)| 0x80;    /// [BYTE 4] >> 12 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
        *p1++= (uint8_t)((*p2>>  6)& 0x3f)| 0x80;    /// [BYTE 5] >>  6 & 3f= 00000000 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
        *p1++= (uint8_t) (*p2&       0x3f)| 0x80;    /// [BYTE 6]         3f= 00000000 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
      } else continue;
  }
      
  *p1= 0;         /// string terminator

  return *this;
}


str8 &str8::operator=(char32_t c) {
  if(wrapping) {
    delData();
    int32_t l= Str::utf8nrBytes(c)+ 1;/// how big the string would be in bytes (+ terminator)
    if(l> wrapSize) return *this;   /// check if it fits inside the wrapped buffer
    Str::utf32to8(c, d);             // create the UTF-8 char
    d[l- 1]= 0;                     /// string terminator
    len= l, nrUnicodes= 1;          /// str numbers
    return *this;
  } else {
    char32_t buf[2]= { c, 0 };
    return *this= buf;
  }
}





// OPERATOR + //
///----------///

str8 str8::operator+(const str8 &s) const {
  //return str8(*this)+= s;   // slower
  if(!s.nrUnicodes) return str8(*this);
  if(!nrUnicodes) return str8(s);

  uint8_t *p1, *p2;
  /// return string variables
  str8 ret;
  ret.len= len+ s.len- 1;
  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.d= (char *)new uint8_t[ret.len];
  /// copy this
  for(p1= (uint8_t *)ret.d, p2= (uint8_t *)d; *p2; )
    *p1++= *p2++;
  /// copy s
  for(p2= (uint8_t *)s.d; *p2; )
    *p1++= *p2++;
  *p1= 0;

  return ret;
}


str8 str8::operator+(const str16 &s) const {
  //return str8(*this)+= s.d; // very slow version
  if(!s.nrUnicodes) return str8(*this);
  if(!nrUnicodes) return str8(s);

  uint8_t *p1, *p2;
  uint16_t *p3;
  /// return string variables
  str8 ret;

  ret.len= len;
  for(p3= (uint16_t *)s.d; *p3; )
    if(*p3<=      0x0000007F)     ret.len++,   p3++;    /// 1 byte
    else if(*p3<= 0x000007FF)     ret.len+= 2, p3++;    /// 2 bytes
    else if(Str::isHighSurrogate(*p3)) ret.len+= 4, p3+= 2;  /// 4 bytes
    else                          ret.len+= 3, p3++;    /// 3 bytes

  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.d= (char *)new uint8_t[ret.len];
  /// copy this
  for(p1= (uint8_t *)ret.d, p2= (uint8_t *)d; *p2; )
    *p1++= *p2++;
  /// copy s
  for(p3= (uint16_t *)s.d; *p3; ) {
    char32_t c;
    /// 16 to 32
    if(Str::isHighSurrogate(*p3))
      c= (*p3<< 10)+ *(p3+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p3+= 2;
    else
      c= *p3++;

    /// 32 to 8
    int32_t n;
    if     (c<= 0x0000007F) { *p1++= (uint8_t) c; continue; }        // 1 byte   U-00000000->U-0000007F:  0xxxxxxx 
    else if(c<= 0x000007FF) *p1++= (uint8_t) (c>> 6) | 0xC0, n= 0;   // 2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
    else if(c<= 0x0000FFFF) *p1++= (uint8_t) (c>> 12)| 0xE0, n= 6;   // 3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
    else if(c<= 0x001FFFFF) *p1++= (uint8_t) (c>> 18)| 0xF0, n= 12;  // 4 bytes  U-00010000->U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
    else continue;

    while(n>= 0)
      *p1++= (uint8_t)((c>>  n)& 0x3f)| 0x80, n-= 6;
  }
  *p1= 0;

  return ret;
}


str8 str8::operator+(const str32 &s) const {
  //return str8(*this)+= s.d; // short but much slower
  if(!s.nrUnicodes) return str8(*this);
  if(!nrUnicodes) return str8(s);

  uint8_t *p1, *p2;
  uint32_t *p3;
  /// return string variables
  str8 ret;

  ret.len= len;
  for(p3= (uint32_t *)s.d; *p3; )
    if     (*p3<= 0x0000007F) ret.len++,   p3++; /// 1 byte
    else if(*p3<= 0x000007FF) ret.len+= 2, p3++; /// 2 bytes
    else if(*p3<= 0x0000FFFF) ret.len+= 3, p3++; /// 3 bytes
    else if(*p3<= 0x001FFFFF) ret.len+= 4, p3++; /// 4 bytes
    else if(*p3<= 0x03FFFFFF) ret.len+= 5, p3++; /// 5 bytes
    else if(*p3<= 0x7FFFFFFF) ret.len+= 6, p3++; /// 6 bytes

  ret.nrUnicodes= nrUnicodes+ s.nrUnicodes;
  ret.d= (char *)new uint8_t[ret.len];
  /// copy this
  for(p1= (uint8_t *)ret.d, p2= (uint8_t *)d; *p2; )
    *p1++= *p2++;
  /// copy s
  for(p3= (uint32_t *)s.d; *p3; p3++) {
    int32_t n;
    if     (*p3<= 0x0000007F) { *p1++= (uint8_t)*p3; continue; }         // 1 byte   U-00000000->U-0000007F:  0xxxxxxx 
    else if(*p3<= 0x000007FF) *p1++= (uint8_t) (*p3>> 6) | 0xC0, n= 0;   // 2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
    else if(*p3<= 0x0000FFFF) *p1++= (uint8_t) (*p3>> 12)| 0xE0, n= 6;   // 3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
    else if(*p3<= 0x001FFFFF) *p1++= (uint8_t) (*p3>> 18)| 0xF0, n= 12;  // 4 bytes  U-00010000->U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
    else continue;

    while(n>= 0)
      *p1++= (uint8_t)((*p3>>  n)& 0x3f)| 0x80, n-= 6;
  }
  *p1= 0;

  return ret;
}


str8 str8::operator+(const char *s) const {
  //return str8(*this)+= s;
  if(!s) return str8(*this);
  if(!*s) return str8(*this);
  if(!nrUnicodes) return str8(s);

  uint8_t *p1, *p2;
  /// return string variables
  str8 ret;
  ret.nrUnicodes= nrUnicodes;
  ret.len= len;

  for(p1= (uint8_t *)s; *p1; p1++)
    if((*p1& 0xc0)!= 0x80)   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      ret.nrUnicodes++;
  ret.len+= (int32_t)(p1- (uint8_t *)s);

  ret.d= (char *)new uint8_t[ret.len];
  /// copy this
  for(p1= (uint8_t *)ret.d, p2= (uint8_t *)d; *p2; )
    *p1++= *p2++;
  /// copy s
  while((*p1++= *s++));

  return ret;
}


str8 str8::operator+(const char16_t *s) const {
  //return str8(*this)+= s;
  if(!s) return str8(*this);
  if(!*s) return str8(*this);
  if(!nrUnicodes) return str8(s);

  uint8_t *p1, *p2;
  const uint16_t *p3;
  /// return string variables
  str8 ret;
  ret.len= len;
  ret.nrUnicodes= nrUnicodes;

  for(p3= (uint16_t *)s; *p3; ret.nrUnicodes++)
    if(*p3<=      0x0000007F)     ret.len++,   p3++;    /// 1 byte
    else if(*p3<= 0x000007FF)     ret.len+= 2, p3++;    /// 2 bytes
    else if(Str::isHighSurrogate(*p3)) ret.len+= 4, p3+= 2;  /// 4 bytes
    else                          ret.len+= 3, p3++;    /// 3 bytes
  
  ret.d= (char *)new uint8_t[ret.len];
  /// copy this
  for(p1= (uint8_t *)ret.d, p2= (uint8_t *)d; *p2; )
    *p1++= *p2++;
  /// copy s
  for(p3= (uint16_t *)s; *p3; ) {
    char32_t c;
    /// 16 to 32
    if(Str::isHighSurrogate(*p3))
      c= (*p3<< 10)+ *(p3+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p3+= 2;
    else
      c= *p3++;

    /// 32 to 8
    int32_t n;
    if     (c<= 0x0000007F) { *p1++= (uint8_t) c; continue; }        // 1 byte   U-00000000->U-0000007F:  0xxxxxxx 
    else if(c<= 0x000007FF) *p1++= (uint8_t) (c>> 6) | 0xC0, n= 0;   // 2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
    else if(c<= 0x0000FFFF) *p1++= (uint8_t) (c>> 12)| 0xE0, n= 6;   // 3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
    else if(c<= 0x001FFFFF) *p1++= (uint8_t) (c>> 18)| 0xF0, n= 12;  // 4 bytes  U-00010000->U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
    else continue;

    while(n>= 0)
      *p1++= (uint8_t)((c>>  n)& 0x3f)| 0x80, n-= 6;
  }
  *p1= 0;

  return ret;
}

str8 str8::operator+(const char32_t *s) const {
  //return str8(*this)+= s;
  if(!s) return str8(*this);
  if(!*s) return str8(*this);
  if(!nrUnicodes) return str8(s);

  uint8_t *p1, *p2;
  const uint32_t *p3;
  /// return string variables
  str8 ret;

  ret.len= len;
  ret.nrUnicodes= nrUnicodes;
  for(p3= (uint32_t *)s; *p3; ret.nrUnicodes++)
    if     (*p3<= 0x0000007F) ret.len++,   p3++; /// 1 byte
    else if(*p3<= 0x000007FF) ret.len+= 2, p3++; /// 2 bytes
    else if(*p3<= 0x0000FFFF) ret.len+= 3, p3++; /// 3 bytes
    else if(*p3<= 0x001FFFFF) ret.len+= 4, p3++; /// 4 bytes
    //else if(*p3<= 0x03FFFFFF) ret.len+= 5, p3++; /// 5 bytes
    //else if(*p3<= 0x7FFFFFFF) ret.len+= 6, p3++; /// 6 bytes
  
  ret.d= (char *)new uint8_t[ret.len];
  /// copy this
  for(p1= (uint8_t *)ret.d, p2= (uint8_t *)d; *p2; )
    *p1++= *p2++;
  /// copy s
  for(p3= (uint32_t *)s; *p3; p3++) {
    int32_t n;
    if     (*p3<= 0x0000007F) { *p1++= (uint8_t)*p3; continue; }         // 1 byte   U-00000000->U-0000007F:  0xxxxxxx 
    else if(*p3<= 0x000007FF) *p1++= (uint8_t) (*p3>> 6) | 0xC0, n= 0;   // 2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
    else if(*p3<= 0x0000FFFF) *p1++= (uint8_t) (*p3>> 12)| 0xE0, n= 6;   // 3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
    else if(*p3<= 0x001FFFFF) *p1++= (uint8_t) (*p3>> 18)| 0xF0, n= 12;  // 4 bytes  U-00010000->U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
    else continue;

    while(n>= 0)
      *p1++= (uint8_t)((*p3>>  n)& 0x3f)| 0x80, n-= 6;
  }
  *p1= 0;

  return ret;
}

str8 str8::operator+(char32_t c) const {
  //return str8(*this)+= c;
  if(!c) return str8(*this);
  if(!nrUnicodes) return str8(c);

  str8 ret;
  ret.nrUnicodes= nrUnicodes+ 1;
  ret.len= len;
  if     (c<= 0x0000007F) ret.len++;   /// 1 byte
  else if(c<= 0x000007FF) ret.len+= 2; /// 2 bytes
  else if(c<= 0x0000FFFF) ret.len+= 3; /// 3 bytes
  else if(c<= 0x001FFFFF) ret.len+= 4; /// 4 bytes
  ret.d= (char *)new uint8_t[ret.len];

  // *this copy
  uint8_t *p1, *p2;
  for(p1= (uint8_t *)ret.d, p2= (uint8_t *)d; *p2; )
    *p1++= *p2++;

  // new unicode add
  int n= -1;
  if     (c<= 0x0000007F) { *p1++= (uint8_t)c; }         // 1 byte   U-00000000->U-0000007F:  0xxxxxxx 
  else if(c<= 0x000007FF) *p1++= (uint8_t) (c>> 6) | 0xC0, n= 0;   // 2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
  else if(c<= 0x0000FFFF) *p1++= (uint8_t) (c>> 12)| 0xE0, n= 6;   // 3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
  else if(c<= 0x001FFFFF) *p1++= (uint8_t) (c>> 18)| 0xF0, n= 12;  // 4 bytes  U-00010000->U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
  while(n>= 0)
    *p1++= (uint8_t)((c>>  n)& 0x3f)| 0x80, n-= 6;

  *p1= 0;

  return ret;
}



// OPERATOR += //
///-----------///

// this one is heavily used - concatenates current str and another str8
str8 &str8::operator+=(const str8 &s) {
  if(!s.len) return *this;      /// other str8 empty -> return *this
  if(!len) return *this= s;     /// current str8 empty -> just copy other str8
  if(this== &s) return *this+= str8(*this);

  uint8_t *p1, *p2;

  if(wrapping) {
    // buffer overflow, copy what can fit inside (slower, counts len and unicodes+ further checks)
    if(len+ s.len- 1> wrapSize) {
      for(p1= (uint8_t *)d+ len- 1, p2= (uint8_t *)s.d; *p2;) {
        if((*p2& 0xc0)!= 0x80) {
          if(len+ Str::utf8headerBytes(*p2)> wrapSize)
            break;
          nrUnicodes++;
        }
        *p1++= *p2++, len++;
      }
    // no buffer overflow branch (it's faster, therefore another branch)
    } else {
      for(p1= (uint8_t *)d+ len- 1, p2= (uint8_t *)s.d; *p2; )
        *p1++= *p2++;
      nrUnicodes+= s.nrUnicodes;
      len+= s.len- 1;
    }

  // no wrapping branch - mem allocs
  } else {
    len+= s.len- 1;             /// new str length in bytes
    nrUnicodes+= s.nrUnicodes;  /// new str unicodes
    uint8_t *p= new uint8_t[len];   /// new str buffer

    /// copy old string to a newly allocated one
    for(p1= p, p2= (uint8_t *)d; *p2; )
      *p1++= *p2++;

    delete[] d;
    d= (char *)p;

    /// copy the other string to the new string
    for(p2= (uint8_t *)s.d; *p2; )
      *p1++= *p2++;
  }

  *p1= 0;                   /// str terminator
  return *this;
}

str8 &str8::operator+=(const str16 &s) { return *this+= s.d; }
str8 &str8::operator+=(const str32 &s) { return *this+= s.d; }


/// concatenate current str and another utf-8 string
str8 &str8::operator+=(const char *s) {
  if(!s) return *this;
  if(!len) return *this= s;

  uint8_t *p1, *p2;

  // wrapping branch
  if(wrapping) {
    for(p1= (uint8_t *)d+ len- 1, p2= (uint8_t *)s; *p2;) {
      if((*p2& 0xc0)!= 0x80) {
        if(len+ Str::utf8headerBytes(*p2)> wrapSize)
          break;
        nrUnicodes++;
      }
      *p1++= *p2++, len++;
    }

  // no wrapping branch - mem allocs
  } else {
    len+= Str::strlen8(s)- 1; /// new str length in bytes
    uint8_t *p= new uint8_t[len]; /// new str buffer

    /// copy old string to a newly allocated one
    for(p1= p, p2= (uint8_t *)d; *p2; )
      *p1++= *p2++;

    delete[] d;
    d= (char *)p;

    /// copy the other string
    for(p2= (uint8_t *)s; *p2;) {
      if((*p2& 0xc0)!= 0x80)
        nrUnicodes++;
      *p1++= *p2++;
    }
  }

  *p1= 0;                   /// str terminator
  return *this;
}

/// UTF-8(this) add a UTF-16 str
str8 &str8::operator+=(const char16_t *s) {
  if(!s) return *this;
  if(!len) return *this= s;

  uint8_t *p1, *p3; const uint16_t *p2;
  char32_t c;

  // wrapping branch
  if(wrapping) {
    for(p1= (uint8_t *)d+ len- 1, p2= (uint16_t *)s; *p2;) {
      /// unpack UTF-16 unicode and advance pointer
      if(Str::isHighSurrogate(*p2))
        c= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p2+= 2;
      else
        c= *p2,  p2++;

      /// compress unicode value into UTF-8
      if(c<= 0x0000007F) {
        if((len+= 1)> wrapSize) { len-= 1; break; }
        *p1++= (uint8_t) c;
      } else if(c<= 0x000007FF) {
        if((len+= 2)> wrapSize) { len-= 2; break; }
        *p1++= (uint8_t) (c>> 6)        | 0xC0;
        *p1++= (uint8_t) (c&     0x3f)| 0x80;
      } else if(c<= 0x0000FFFF) {
        if((len+= 3)> wrapSize) { len-= 3; break; }
        *p1++= (uint8_t) (c>> 12)       | 0xE0;
        *p1++= (uint8_t)((c>> 6)&  0x3F)| 0x80;
        *p1++= (uint8_t) (c&     0x3F)| 0x80;
      } else if(c<= 0x001FFFFF) {
        if((len+= 4)> wrapSize) { len-= 4; break; }
        *p1++= (uint8_t) (c>> 18)       | 0xF0;
        *p1++= (uint8_t)((c>> 12)& 0x3F)| 0x80;
        *p1++= (uint8_t)((c>>  6)& 0x3F)| 0x80;
        *p1++= (uint8_t) (c&     0x3F)| 0x80;
      }

      nrUnicodes++;
    }

  // no wrapping branch - mem allocs
  } else {
    
    for(p2= (uint16_t *)s; *p2;) {
      if(Str::isHighSurrogate(*p2))
        c= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p2+= 2;
      else
        c= *p2,  p2++;
      len+= Str::utf8nrBytes(c);      /// new str len in bytes
      nrUnicodes++;                   /// new str nr of unicodes
    }
    uint8_t *p= new uint8_t[len];         /// new str data

    /// copy old string to a newly allocated one
    for(p1= p, p3= (uint8_t *)d; *p3; )
      *p1++= *p3++;
    
    delete[] d;
    d= (char *)p;

    /// copy the other string
    for(p2= (uint16_t *)s; *p2;) {
      if(Str::isHighSurrogate(*p2))
        c= (*p2<< 10)+ *(p2+ 1)+ Str::UTF16_SURROGATE_OFFSET,  p2+= 2;
      else
        c= *p2,  p2++;
      p1+= Str::utf32to8(c, (char *)p1);
    }
  }

  *p1= 0;                   /// str terminator
  return *this;
}

/// concatenates current string and another utf-32 string
str8 &str8::operator+=(const char32_t *s) {
  if(!s) return *this;
  if(!len) return *this= s;

  uint8_t *p1, *p3; const uint32_t *p2;

  // wrapping branch
  if(wrapping) {
    for(p1= (uint8_t *)d+ len- 1, p2= (uint32_t *)s; *p2;) {

      /// compress unicode value into UTF-8
      if(*p2<= 0x0000007F) {
        if((len+= 1)> wrapSize) { len-= 1; break; }
        *p1++= (uint8_t) *p2++;
      } else if(*p2<= 0x000007FF) {
        if((len+= 2)> wrapSize) { len-= 2; break; }
        *p1++= (uint8_t) (*p2>> 6)        | 0xC0;
        *p1++= (uint8_t) (*p2++&     0x3f)| 0x80;
      } else if(*p2<= 0x0000FFFF) {
        if((len+= 3)> wrapSize) { len-= 3; break; }
        *p1++= (uint8_t) (*p2>> 12)       | 0xE0;
        *p1++= (uint8_t)((*p2>> 6)&  0x3F)| 0x80;
        *p1++= (uint8_t) (*p2++&     0x3F)| 0x80;
      } else if(*p2<= 0x001FFFFF) {
        if((len+= 4)> wrapSize) { len-= 4; break; }
        *p1++= (uint8_t) (*p2>> 18)       | 0xF0;
        *p1++= (uint8_t)((*p2>> 12)& 0x3F)| 0x80;
        *p1++= (uint8_t)((*p2>>  6)& 0x3F)| 0x80;
        *p1++= (uint8_t) (*p2++&     0x3F)| 0x80;
      }

      nrUnicodes++;
    }

  // no wrapping branch - mem allocs
  } else {
    
    for(p2= (uint32_t *)s; *p2; p2++ )
      len+= Str::utf8nrBytes(*p2);    /// new str len in bytes
    nrUnicodes+= (int32_t)(p2- (uint32_t *)s- 1);   /// new str nr of unicodes
    uint8_t *p= new uint8_t[len];         /// new str data

    /// copy old string to a newly allocated one
    for(p1= p, p3= (uint8_t *)d; *p3; )
      *p1++= *p3++;
    
    delete[] d;
    d= (char *)p;

    /// copy the other string
    for(p2= (uint32_t *)s; *p2; p2++)
      p1+= Str::utf32to8(*p2, (char *)p1);
  }

  *p1= 0;                   /// str terminator
  return *this;
}


/// add a unicode character
str8 &str8::operator+=(char32_t c) {
  if(!c) return *this;
  if(!len) return *this= c;

  // wrapping branch
  if(wrapping) {
    uint8_t *p= (uint8_t *)d+ len- 1;
    /// compress unicode value into UTF-8
    if(c<= 0x0000007F) {
      if((len+= 1)> wrapSize) { len-= 1; return *this; }
      *p++= (uint8_t) c;
    } else if(c<= 0x000007FF) {
      if((len+= 2)> wrapSize) { len-= 2; return *this; }
      *p++= (uint8_t) (c>> 6)        | 0xC0;
      *p++= (uint8_t) (c&       0x3f)| 0x80;
    } else if(c<= 0x0000FFFF) {
      if((len+= 3)> wrapSize) { len-= 3; return *this; }
      *p++= (uint8_t) (c>> 12)       | 0xE0;
      *p++= (uint8_t)((c>> 6)&  0x3F)| 0x80;
      *p++= (uint8_t) (c&       0x3F)| 0x80;
    } else if(c<= 0x001FFFFF) {
      if((len+= 4)> wrapSize) { len-= 4; return *this; }
      *p++= (uint8_t) (c>> 18)       | 0xF0;
      *p++= (uint8_t)((c>> 12)& 0x3F)| 0x80;
      *p++= (uint8_t)((c>>  6)& 0x3F)| 0x80;
      *p++= (uint8_t) (c&       0x3F)| 0x80;
    }
    *p= 0;
    
    nrUnicodes++;

  // not wrapping branch
  } else {
    len+= Str::utf8nrBytes(c);;
    nrUnicodes++;
    uint8_t *p= new uint8_t[len], *p1= p, *p2= (uint8_t *)d;
    /// copy old string to new one
    while(*p2)
      *p1++= *p2++;
    /// add single unicode value
    p1+= Str::utf32to8(c, (char *)p1);
    *p1= 0;                   /// str terminator

    delete[] d;
    d= (char *)p;
  }
  return *this;
}



// CONVERSION operators //
///--------------------///




// convert to UTF-16
// returned string is part of the class, it will be auto deallocated. Use getUTF16() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char16_t *str8::convert16(int32_t *out_len) {
  if(!len) { if(out_len) *out_len= 0; return NULL; }

  uint16_t *p1;
  uint8_t *p2;
  uint32_t c, n;
  
  /// UTF-16 string size - unicodes >= 0x00010000 fit into 2 surrogates
  for(p2= (uint8_t *)d, n= 0; *p2; ) {
    if(*p2< 128)                 p2++,   n++;
    else if((*p2& 0xe0) == 0xc0) p2+= 2, n++;
    else if((*p2& 0xf0) == 0xe0) p2+= 3, n++;
    else if((*p2& 0xf8) == 0xf0) p2+= 4, n+= 2;
    else if((*p2& 0xfc) == 0xf8) p2+= 5;
    else if((*p2& 0xfe) == 0xfc) p2+= 6;
    else p2++;
  }
  n++;  /// str terminator

  if(out_len) *out_len= n* 2; /// optional return value

  /// alloc mem
  if(d16) delete[] d16;
  d16= (char16_t *)new uint16_t[n];

  // copy
  for(p1= (uint16_t *)d16, p2= (uint8_t *)d; *p2; ) {
    /// unpack UTF-8
    if      (*p2 < 128) { *p1++= *p2++; continue; }     /// first 128 (ASCII) values are direct copy
    else if((*p2& 0xE0)== 0xC0) n= 2, c= (*p2++)& 0x1F; /// if bits 11100000 (0xE0) are 110xxxxx (0xC0) then assign xxx11111 (0x1F) bits to unicode value
    else if((*p2& 0xF0)== 0xE0) n= 3, c= (*p2++)& 0x0F; /// if bits 11110000 (0xF0) are 1110xxxx (0xE0) then assign xxxx1111 (0x0F) bits to unicode value
    else if((*p2& 0xF8)== 0xF0) n= 4, c= (*p2++)& 0x07; /// if bits 11111000 (0xF8) are 11110xxx (0xF0) then assign xxxxx111 (0x07) bits to unicode value
    else if((*p2& 0xFC)== 0xF8) { p2+= 5; continue; }
    else if((*p2& 0xFE)== 0xFC) { p2+= 6; continue; }
    else { p2++; continue; }
    
    while(--n)                                          /// for each continuation byte (there can be a max of 5)
      c<<=6, c+= (*p2++)& 0x3F;                         /// assign 6 bits xx111111 (0x3F) from continuation byte(s)

    /// pack into UTF-16 (pack if needed)
    if(c> 0xFFFF) {
      *p1++= Str::UTF16_LEAD_OFFSET+ (c>> 10);
      *p1++= 0xDC00+ (c& 0x3FF);
    } else
      *p1++= (char16_t)c;
  }
  *p1= 0;   /// str terminator

  return d16;
}


// convert to UTF-32
// returned string is part of the class, it will be auto deallocated. Use getUTF32() for a pointer that is handled by the user
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char32_t *str8::convert32(int32_t *out_len) {
  if(!len) { if(out_len) *out_len= 0; return NULL; }
  
  /// alloc
  if(d32) delete[] d32;
  d32= (char32_t *)new uint32_t[nrUnicodes+ 1];

  /// copy
  uint32_t *p1= (uint32_t *)d32;
  uint8_t *p2= (uint8_t *)d;
  int32_t n;

  while(*p2) {
    if      (*p2 < 128) { *p1++= *p2++; continue; }       /// first 128 (ASCII) values are direct copy
    else if((*p2& 0xE0)== 0xC0) n= 2, *p1= (*p2++)& 0x1F; /// if bits 11100000 (0xE0) are 110xxxxx (0xC0) then assign xxx11111 (0x1F) bits to unicode value
    else if((*p2& 0xF0)== 0xE0) n= 3, *p1= (*p2++)& 0x0F; /// if bits 11110000 (0xF0) are 1110xxxx (0xE0) then assign xxxx1111 (0x0F) bits to unicode value
    else if((*p2& 0xF8)== 0xF0) n= 4, *p1= (*p2++)& 0x07; /// if bits 11111000 (0xF8) are 11110xxx (0xF0) then assign xxxxx111 (0x07) bits to unicode value
    /// the last 2 bytes should not be used
    else if((*p2& 0xFC)== 0xF8) n= 5, *p1= (*p2++)& 0x03; /// if bits 11111100 (0xFC) are 111110xx (0xF8) then assign xxxxxx11 (0x03) bits to unicode value
    else if((*p2& 0xFE)== 0xFC) n= 6, *p1= (*p2++)& 0x01; /// if bits 11111110 (0xFE) are 1111110x (0xFC) then assign xxxxxxx1 (0x01) bit to unicode value
    else { p2++; continue; }   // bad unicode value / some string error
    
    while(--n)                                            /// for each continuation byte (there can be a max of 5)
      *p1<<=6, *p1+= (*p2++)& 0x3f;                       /// assign 6 bits xx111111 (0x3F) from continuation byte(s)
    p1++;
  } /// pass thru all the source string
  *p1= 0; /// str terminator

  if(out_len) *out_len= (nrUnicodes+ 1)* 4;

  return d32;
}

// convert to UTF-16
// returned string must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char16_t *str8::getUTF16(int32_t *out_len) const {
  if(!len) { if(out_len) *out_len= 0; return NULL; }

  uint16_t *p1;
  uint8_t *p2;
  uint32_t c, n;
  
  /// UTF-16 string size - unicodes >= 0x00010000 fit into 2 surrogates
  for(p2= (uint8_t *)d, n= 0; *p2; ) {
    if(*p2< 128)                 p2++,   n++;
    else if((*p2& 0xe0) == 0xc0) p2+= 2, n++;
    else if((*p2& 0xf0) == 0xe0) p2+= 3, n++;
    else if((*p2& 0xf8) == 0xf0) p2+= 4, n+= 2;
    else if((*p2& 0xfc) == 0xf8) p2+= 5;
    else if((*p2& 0xfe) == 0xfc) p2+= 6;
    else p2++;
  }
  n++;  /// str terminator

  if(out_len) *out_len= n* 2; /// optional return value

  /// alloc mem
  uint16_t *buf16= (uint16_t *)new uint16_t[n];

  // copy
  for(p1= buf16, p2= (uint8_t *)d; *p2; ) {
    /// unpack UTF-8
    if      (*p2 < 128) { *p1++= *p2++; continue; }     /// first 128 (ASCII) values are direct copy
    else if((*p2& 0xE0)== 0xC0) n= 2, c= (*p2++)& 0x1F; /// if bits 11100000 (0xE0) are 110xxxxx (0xC0) then assign xxx11111 (0x1F) bits to unicode value
    else if((*p2& 0xF0)== 0xE0) n= 3, c= (*p2++)& 0x0F; /// if bits 11110000 (0xF0) are 1110xxxx (0xE0) then assign xxxx1111 (0x0F) bits to unicode value
    else if((*p2& 0xF8)== 0xF0) n= 4, c= (*p2++)& 0x07; /// if bits 11111000 (0xF8) are 11110xxx (0xF0) then assign xxxxx111 (0x07) bits to unicode value
    else if((*p2& 0xFC)== 0xF8) { p2+= 5; continue; }
    else if((*p2& 0xFE)== 0xFC) { p2+= 6; continue; }
    else { p2++; continue; }
    
    while(--n)                                          /// for each continuation byte (there can be a max of 5)
      c<<=6, c+= (*p2++)& 0x3F;                         /// assign 6 bits xx111111 (0x3F) from continuation byte(s)

    /// pack into UTF-16 (pack if needed)
    if(c> 0xFFFF) {
      *p1++= Str::UTF16_LEAD_OFFSET+ (c>> 10);
      *p1++= 0xDC00+ (c& 0x3FF);
    } else
      *p1++= (char16_t)c;
  }
  *p1= 0;   /// str terminator

  return (char16_t *)buf16;
}


// convert to UTF-32
// returned string must be manually deallocated when not needed anymore
// <out_len> [optional] - returns the string length in bytes, includes the str terminator
char32_t *str8::getUTF32(int32_t *out_len) const {
  if(!len) { if(out_len) *out_len= 0; return NULL; }
  
  /// alloc
  uint32_t *buf32= new uint32_t[nrUnicodes+ 1];

  /// copy
  uint32_t *p1= buf32;
  uint8_t *p2= (uint8_t *)d;
  int32_t n;

  while(*p2) {
    if      (*p2 < 128) { *p1++= *p2++; continue; }       /// first 128 (ASCII) values are direct copy
    else if((*p2& 0xE0)== 0xC0) n= 2, *p1= (*p2++)& 0x1F; /// if bits 11100000 (0xE0) are 110xxxxx (0xC0) then assign xxx11111 (0x1F) bits to unicode value
    else if((*p2& 0xF0)== 0xE0) n= 3, *p1= (*p2++)& 0x0F; /// if bits 11110000 (0xF0) are 1110xxxx (0xE0) then assign xxxx1111 (0x0F) bits to unicode value
    else if((*p2& 0xF8)== 0xF0) n= 4, *p1= (*p2++)& 0x07; /// if bits 11111000 (0xF8) are 11110xxx (0xF0) then assign xxxxx111 (0x07) bits to unicode value
    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    else if((*p2& 0xFC)== 0xF8) n= 5, *p1= (*p2++)& 0x03; /// if bits 11111100 (0xFC) are 111110xx (0xF8) then assign xxxxxx11 (0x03) bits to unicode value
    else if((*p2& 0xFE)== 0xFC) n= 6, *p1= (*p2++)& 0x01; /// if bits 11111110 (0xFE) are 1111110x (0xFC) then assign xxxxxxx1 (0x01) bit to unicode value
    else { p2++; continue; } // bad unicode value / some string error
    
    while(--n)                                            /// for each continuation byte (there can be a max of 5)
      *p1<<=6, *p1+= (*p2++)& 0x3f;                       /// assign 6 bits xx111111 (0x3F) from continuation byte(s)
    p1++;
  } /// pass thru all the source string
  *p1= 0; /// str terminator

  if(out_len) *out_len= (nrUnicodes+ 1)* 4;

  return (char32_t *)buf32;
}


// returns string as UTF-16
// <out_buf> - the buffer that will be filled with the converted string
// <out_bufSize> - [MUST SPECIFY] the size of <out_buf> in uint16 units (not bytes)
// <out_nrUnicodes> - [optional] returns the number of unicodes in the output string
// <out_len> [optional] returns the resulting string size in bytes, including the string terminator
void str8::convert16static(char16_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if(!len) { if(out_len) *out_len= 0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(in_bufSize> 0) *out_buf= 0; }

  uint16_t *p1;
  uint8_t *p2;
  uint32_t c, n, u= 0;
  int32_t l= 0, m= in_bufSize- 1;
  // copy
  for(p1= (uint16_t *)out_buf, p2= (uint8_t *)d; *p2; ) {
    /// unpack UTF-8
    if      (*p2 < 128) { if((l+= 1)> m) { l-= 1; break; } *p1++= *p2++; u++; continue; }/// first 128 (ASCII) values are direct copy
    else if((*p2& 0xE0)== 0xC0) n= 2, c= (*p2++)& 0x1F; /// if bits 11100000 (0xE0) are 110xxxxx (0xC0) then assign xxx11111 (0x1F) bits to unicode value
    else if((*p2& 0xF0)== 0xE0) n= 3, c= (*p2++)& 0x0F; /// if bits 11110000 (0xF0) are 1110xxxx (0xE0) then assign xxxx1111 (0x0F) bits to unicode value
    else if((*p2& 0xF8)== 0xF0) n= 4, c= (*p2++)& 0x07; /// if bits 11111000 (0xF8) are 11110xxx (0xF0) then assign xxxxx111 (0x07) bits to unicode value
    else if((*p2& 0xFC)== 0xF8) { p2+= 5; continue; }
    else if((*p2& 0xFE)== 0xFC) { p2+= 6; continue; }
    else { p2++; continue; }    // bad unicode value / some string error
    
    while(--n)                                          /// for each continuation byte (there can be a max of 5)
      c<<=6, c+= (*p2++)& 0x3F;                         /// assign 6 bits xx111111 (0x3F) from continuation byte(s)

    /// pack into UTF-16 (pack if needed)
    if(c>= 0x10000) {
      if((l+= 2)> m) { l-= 2; break; }      /// buffer overflow test
      *p1++= Str::UTF16_LEAD_OFFSET+ (c>> 10);
      *p1++= 0xDC00+ (c& 0x3FF);
    } else {
      if((l+= 1)> m) { l-= 1; break; }      /// buffer overflow test
      *p1++= (char16_t)c;
    }
    u++;
  }
  if(out_nrUnicodes) *out_nrUnicodes= u;
  if(out_len) *out_len= (l+ 1)* 2;
}

// returns string as UTF-32
// <out_buf> - the buffer that will be filled with the converted string
// <out_bufSize> - [MUST SPECIFY] the size of <out_buf> in uint32_t units (not bytes)
// <out_nrUnicodes> - [optional] returns the number of unicodes in the output string
// <out_len> [optional] returns the resulting string size in bytes, including the string terminator
void str8::convert32static(char32_t *out_buf, int32_t in_bufSize, int32_t *out_nrUnicodes, int32_t *out_len) const {
  if(!len) { if(out_len) *out_len= 0; if(out_nrUnicodes) *out_nrUnicodes= 0; if(in_bufSize> 0) *out_buf= 0; }

  uint32_t *p1;
  uint8_t *p2;
  uint32_t c, n;
  int32_t l= 0, m= in_bufSize- 1;

  // copy
  for(p1= (uint32_t *)out_buf, p2= (uint8_t *)d; *p2; ) {
    /// unpack UTF-8
    if      (*p2 < 128) { if(++l> m) { l--; break; } *p1++= *p2++; continue; } /// first 128 (ASCII) values are direct copy
    else if((*p2& 0xE0)== 0xC0) n= 2, c= (*p2++)& 0x1F; /// if bits 11100000 (0xE0) are 110xxxxx (0xC0) then assign xxx11111 (0x1F) bits to unicode value
    else if((*p2& 0xF0)== 0xE0) n= 3, c= (*p2++)& 0x0F; /// if bits 11110000 (0xF0) are 1110xxxx (0xE0) then assign xxxx1111 (0x0F) bits to unicode value
    else if((*p2& 0xF8)== 0xF0) n= 4, c= (*p2++)& 0x07; /// if bits 11111000 (0xF8) are 11110xxx (0xF0) then assign xxxxx111 (0x07) bits to unicode value
    // the last 2 bytes are not used, but avaible if in the future unicode will expand
    else if((*p2& 0xFC)== 0xF8) n= 5, c= (*p2++)& 0x03; /// if bits 11111100 (0xFC) are 111110xx (0xF8) then assign xxxxxx11 (0x03) bits to unicode value
    else if((*p2& 0xFE)== 0xFC) n= 6, c= (*p2++)& 0x01; /// if bits 11111110 (0xFE) are 1111110x (0xFC) then assign xxxxxxx1 (0x01) bit to unicode value
    else { p2++; continue; } // bad unicode value / some string error
    
    while(--n)                                          /// for each continuation byte (there can be a max of 5)
      c<<=6, c+= (*p2++)& 0x3F;                         /// assign 6 bits xx111111 (0x3F) from continuation byte(s)

    if(++l> m) { l--; break; }
    *p1++= c;
  }
  if(out_nrUnicodes) *out_nrUnicodes= l;
  if(out_len) *out_len= (l+ 1)* 4;
}







// whole string conversion to lowercase
void str8::lower() {
  if(!d) return;                  /// if string is NULL, there is nothing to convert

  uint32_t *buf= (uint32_t *)convert32();       /// buf will hold current string unpacked, utf-32 format

  for(uint32_t *p= buf; *p; p++)
    *p= Str::tolower(*p);         /// lowercase each character
  
  *this= (char32_t *)buf;
}


// whole string conversion to uppercase
void str8::upper() {
  if(!d) return;                          /// if string is NULL, there is nothing to convert

  uint32_t *buf= (uint32_t *)convert32(); /// buf will hold current string unpacked, utf-32 format
  
  for(uint32_t *p= buf; *p; p++)
    *p= Str::toupper(*p);                 /// uppercase each character

  this->operator=((char32_t *)buf);       /// basically d= buf (utf32 to utf8)
}




///------------------------///
// OPERATOR - / OPERATOR -= // clears n chars from str8s
///------------------------///

/// clears n chars from the back of the string
str8 &str8::operator-=(int n) {
  /// basic checks
  if(!n) return *this;
  if(nrUnicodes- n<= 0) { delData(); return *this; }

  if(wrapping) {
    uint8_t *cut= (uint8_t *)Str::getUnicode8(d, nrUnicodes);    /// cut right after the last unicode value
    len= (int32_t)(cut- (uint8_t *)d)+ 1;
    nrUnicodes-= n;
    *cut= 0;
    return *this;

  } else {
    int remain= nrUnicodes- n;      /// how many characters will remain in returned string
    if(remain< 0) remain= 0;
    *pointUnicode(remain)= 0;       /// cut right after the last unicode value
    uint8_t *p= (uint8_t *)d;
    d= NULL, len= 0, nrUnicodes= 0; /// reset internal vars
    *this= (char *)p;               /// copy from old string
    delete[] p;

    return *this;
  }
}


str8 str8::operator-(int n) const {
  /// basic checks
  if(!n) return str8(*this);
  if(nrUnicodes- n<= 0) return str8();

  uint8_t *p= (uint8_t *)pointUnicode(nrUnicodes- n); /// p will point right where the string needs to be cut
  uint8_t c= *p;                  /// hold current value of *p to restore it
  *p= 0;                          /// cut current string temporary

  str8 ret(d);
  *p= c;                          /// restore current string
  return ret;
}



///-----------///
// OPERATOR == //
///-----------///

bool str8::operator==(const str8 &s) const {
  if(s.nrUnicodes!= nrUnicodes) return false;
  uint8_t *p1= (uint8_t *)d, *p2= (uint8_t *)s.d;
  while(*p1 && *p2)
    if(*p1++ != *p2++)
      return false;
  if(*p1 || *p2) return false;    /// anything remains in any string -> they're not the same

  return true; // this point reached -> there is no difference between str8s
}


bool str8::operator==(const str16 &s) const { return operator==(s.d); }
bool str8::operator==(const str32 &s) const { return operator==(s.d); }


// UTF-8 str matches another UTF-8 str
bool str8::operator==(const char *s) const {
  /// fast check based on NULLs, first
  if(!s) {
    if(!d) return true;
    return false;
  }
  if(!d) return false;
  /// temp vars
  const uint8_t *p1= (uint8_t *)d, *p2= (uint8_t *)s;
  /// check each character
  while(*p1 && *p2)
    if(*p1++ != *p2++)
      return false;
  /// any chars remain in any of the strings, they are not the same
  if(*p1 || *p2) return false;
  // reached this point, they are the same
  return true;
}




bool str8::operator==(const char16_t *s) const {
  /// fast check based on NULLs, first
  if (!s) {
    if (!d) return true;
    else    return false;
  }
  if (!d) return false;

  /// temp vars
  const uint8_t *p1= (uint8_t *)d;   /// walks this
  const uint16_t *p2= (uint16_t *)s; /// walks the other string
  char32_t c1, c2;
  
  /// check each character
  while (*p1 && *p2) {
    /// UTF-8 to unicode32
    //c1 = Str::utf8to32(p1, &nbytes1); THIS IS WAY SLOWER
    int32_t n = 0;
    if (*p1 < 128)                n= 1, c1= *p1++;
    else if((*p1 & 0xe0) == 0xc0) n= 2, c1= (*p1++) & 0x1f;
    else if((*p1 & 0xf0) == 0xe0) n= 3, c1= (*p1++) & 0x0f;
    else if((*p1 & 0xf8) == 0xf0) n= 4, c1= (*p1++) & 0x07;
    else c1= 0xFFFD, p1++;
    while (--n> 0)                  /// unpack the continuation bytes
      c1<<= 6, c1+= *p1++ & 0x3f;

    /// UTF-16 to unicode32
    //c2 = Str::utf16to32(p2, &nbytes2);  // THIS IS WAY SLOWER
    if (Str::isHighSurrogate(*p2))
      c2= (*p2 << 10) + *(p2 + 1) + Str::UTF16_SURROGATE_OFFSET, p2+= 2;
    else
      c2= *p2++;

    if(c1 != c2) return false;
    //p1+= nbytes1, p2+= nbytes2;
  }
  /// any chars remain in any of the strings, they are not the same
  if(*p1 || *p2) return false;
  // reached this point, they are the same
  return true;
}









// UTF-8 str matches another UTF-32 str
bool str8::operator==(const char32_t *s) const {
  /// fast check based on NULLs, first
  if(!s) {
    if(!d)  return true;
    else    return false;
  }
  if(!d) return false;
  /// temp vars
  const uint8_t *p1= (uint8_t *)d;
  const uint32_t *p2= (uint32_t *)s;
  char32_t c1;
  /// check each character
  while(*p1 && *p2) {
    /// utf8to32 unpacked, no extra ifs, way faster
    int32_t n = 0;
    if (*p1 < 128)               n= 1, c1= *p1++;
    else if((*p1& 0xe0) == 0xc0) n= 2, c1= (*p1++)& 0x1f;
    else if((*p1& 0xf0) == 0xe0) n= 3, c1= (*p1++)& 0x0f;
    else if((*p1& 0xf8) == 0xf0) n= 4, c1= (*p1++)& 0x07;
    else if((*p1& 0xfc) == 0xf8) n= 5, c1= (*p1++)& 0x03;    // 5bytes should not be used
    else if((*p1& 0xfe) == 0xfc) n= 6, c1= (*p1++)& 0x01;    // 6bytes should not be used
    else c1= 0xFFFD, p1++;
    /// unpack the extra continuation bytes
    while (--n> 0)
      c1<<= 6, c1+= *p1++ & 0x3f;

    if(c1!= *p2++) return false;
  }
  /// any chars remain in any of the strings, they are not the same
  if(*p1 || *p2) return false;
  // reached this point, they are the same
  return true;
}


bool str8::operator==(char32_t c) const {
  if(!c) {
    if(!d) return true;
    else   return false;
  }
  if(nrUnicodes!= 1) return false;
  return (Str::utf8to32(d) == c);
}






///-----------------------------------------///
// COMBining diacritical character functions //
///-----------------------------------------///

void str8::clearCombs() {
  if((!len) || (!nrUnicodes) || (!d)) return;

  uint8_t *p1= (uint8_t *)d, *p2= (uint8_t *)d;
  char32_t c;
  int32_t adv;
  /// remove diacriticals: copy the same buffer
  ///   the trick is advancing the second pointer and skipping the copy if a diacritical is found
  while(*p2) {
    c= Str::utf8to32((char *)p2, &adv);
    /// skip this char from copy if it's a diacritical
    if(Str::isComb(c)) {
      p2+= adv;
      nrUnicodes--;
      len-= adv;
    } else 
      while(adv--)
        *p1++= *p2++;
  }
  *p1= 0;
  
  /* this is just removing very few mem garbage, for extra drag
  if(!wrapping) {
    uint8_t *p= d;
    d= new uint8[len];
    Str::strcpy8(d, p);
    delete[] p;
  }*/
}


// returns the number of combining diacriticals (if any) in string
int32_t str8::nrCombs() const {
  if(!d) return 0;
  int32_t ret= 0;
  for(uint8_t *p= (uint8_t *)d; *p; p++)
    if((*p& 0xc0)!= 0x80)   /// count only utf8 header bytes
      if(Str::isComb(Str::utf8to32((char *)p)))
        ret++;
  return ret;
}


// returns the number of characters, excluding combining diacriticals
int32_t str8::nrChars() const {
  if(!d) return 0;
  int32_t ret= 0;
  for(uint8_t *p= (uint8_t *)d; *p; p++)
    if((*p& 0xc0)!= 0x80)   /// count only utf8 header bytes
      if(!Str::isComb(Str::utf8to32((char *)p)))
        ret++;
  return ret;
}



///--------------------------------///
// UTF-8 SECURITY / READ FROM FILES // (any read from a file should be considered UNSAFE)
///--------------------------------///


// bad characters will be MARKED with 0xFFFD
// reading from UNSAFE SOURCES / FILES / INPUT is NOT SAFE. use secureUTF8() to validate a utf8 str
str8 &str8::secureUTF8(const char *s, int32_t in_len) {
  delData();
  if(s== NULL) return *this;
  if(in_len< 0) return *this;

  /// length in unicode vals
  int32_t n= 0;
  const uint8_t *p= (const uint8_t *)s;
  bool bad, usingLimit= false;
  int32_t limit;

  if(in_len) {
    usingLimit= true;
    limit= in_len;
  }

  // can't assume that every character is ok, there can be garbage @ every level
  for(; *p; p++) {
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

  /// copy to temporary unpacked string
  uint32_t *u= new uint32_t[n+ 1];          /// UNPACKED STRING
  
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

  /// compress unpacked string to *this
  *this= (char32_t *)u;
  delete[] u;
  return *this;
}




extern char32_t _secure16to32advance(const char16_t **);


// verify a UTF-16 string and create <this> from it
str8 &str8::secureUTF16(const char16_t *in_s, int32_t in_len) {
  delData();
  if(in_s== NULL) return *this;

  uint8_t *p1;
  const uint16_t *p2;
  bool overflow= false;

  bool usingLimit= false;
  int32_t limit, m;
  if(in_len) 
    usingLimit= true, limit= in_len;

  
  for(p2= (uint16_t *)in_s; *p2; ) {
    if(usingLimit) {
      if(Str::isHighSurrogate(*p2)) { if((limit- 4)< 0) break; }
      else                            if((limit- 2)< 0) break;
    }

    char32_t c= _secure16to32advance((const char16_t **)&p2);
    limit= in_len- (int32_t)((uint8_t *)p2- (uint8_t *)in_s);

    /// check how many bytes the character would pack into
    if     (c<= 0x0000007F) len++;    /// 1 byte  U-00000000 - U-0000007F:  0xxxxxxx 
    else if(c<= 0x000007FF) len+= 2;  /// 2 bytes U-00000080 - U-000007FF:  110xxxxx 10xxxxxx 
    else if(c<= 0x0000FFFF) len+= 3;  /// 3 bytes U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
    else if(c<= 0x001FFFFF) len+= 4;  /// 4 bytes U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
  }

  if(!len) return *this;
  len++;                    /// space for terminator

  if(wrapping) {
    if(len> wrapSize)
      overflow= true;
    m= wrapSize- 1;
  } else
    d= (char *)new uint8_t[len];      /// create d

  limit= in_len;
  for(p1= (uint8_t *)d, p2= (uint16_t *)in_s; *p2;) {
    if(usingLimit) {
      if(Str::isHighSurrogate(*p2)) { if(limit- 4< 0) break; }
      else                            if(limit- 2< 0) break;
    }
    char32_t c= _secure16to32advance((const char16_t **)&p2);
    limit= in_len- (int32_t)((uint8_t *)p2- (uint8_t *)in_s);


    // pack the character - no more checks
    if(c<= 0x007F) {             // 1 byte   U-00000000-U-0000007F:  0xxxxxxx 
      if(overflow) if((m-= 1)< 0) break;
      *p1++= (uint8_t)  c;
    } else if(c<= 0x07FF) {      // 2 bytes  U-00000080-U-000007FF:  110xxxxx 10xxxxxx 
      if(overflow) if((m-= 2)< 0) break;
      *p1++= (uint8_t) (c>> 6)        | 0xC0; /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
      *p1++= (uint8_t) (c&       0x3f)| 0x80; /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(c<= 0xFFFF) {      // 3 bytes  U-00000800-U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
      if(overflow) if((m-= 3)< 0) break;
      *p1++= (uint8_t) (c>> 12)       | 0xE0; /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
      *p1++= (uint8_t)((c>> 6)&  0x3F)| 0x80; /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p1++= (uint8_t) (c&       0x3F)| 0x80; /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(c<= 0x001FFFFF) {  // 4 bytes  U-00010000-U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
      if(overflow) if((m-= 4)< 0) break;
      *p1++= (uint8_t) (c>> 18)       | 0xF0; /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
      *p1++= (uint8_t)((c>> 12)& 0x3F)| 0x80; /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
      *p1++= (uint8_t)((c>>  6)& 0x3F)| 0x80; /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p1++= (uint8_t) (c&       0x3F)| 0x80; /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    }
    nrUnicodes++;
  } /// for each char/comb in s
  *p1= 0;                                   /// str terminator
  if(overflow) len= Str::strlen8(d);        /// wrapping overflow handling

  return *this;
}




extern char32_t _secure32check(char32_t);


// SECURE UTF-32 to UTF-8
str8 &str8::secureUTF32(const char32_t *in_s, int32_t in_len) {
  delData();
  if(in_s== NULL) return *this;

  uint8_t *p1;
  const uint32_t *p2= (uint32_t *)in_s;
  bool overflow= false;
  int32_t limit, m;
  bool usingLimit= false;
  if(in_len)
    usingLimit= true, limit= in_len;


  /// length in bytes of resulting string - errors are checked here too
  for( ; *p2; p2++) {
    if(usingLimit) { limit-= 4; if(limit< 0) break; }
    char32_t c= _secure32check(*p2);

    if     (c<= 0x0000007F) len++;   /// 1 byte long   U-00000000�U-0000007F:  0xxxxxxx
    else if(c<= 0x000007FF) len+= 2; /// 2 bytes long  U-00000080�U-000007FF:  110xxxxx 10xxxxxx
    else if(c<= 0x0000FFFF) len+= 3; /// 3 bytes long  U-00000800�U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
    else if(c<= 0x001FFFFF) len+= 4; /// 4 bytes long  U-00010000�U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    //else if(c<= 0x03FFFFFF) len+= 5; /// 5 bytes long  U-00200000�U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    //else if(c<= 0x7FFFFFFF) len+= 6; /// 6 bytes long  U-04000000�U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
  }

  if(!len) return *this;
  len++;                         /// space for terminator;

  if(wrapping) {
    if(len> wrapSize)
      overflow= true;
    m= wrapSize- 1;
  } else
    d= (char *)new uint8_t[len];        /// create d
  
  // verify & copy
  limit= in_len;
  for(p1= (uint8_t *)d, p2= (uint32_t *)in_s; *p2; p2++) {    // for each character in s
    if(usingLimit) { limit-= 4; if(limit< 0) break; }

    char32_t c= _secure32check(*p2);

    /// pack unicode value into UTF-8
    if(c<= 0x007F) {            // 1 byte   U-00000000-U-0000007F:  0xxxxxxx 
      if(overflow) { m-= 1; if(m< 0) break; }
      *p1++= (uint8_t)  c;
    } else if(c<= 0x07FF) {     // 2 bytes  U-00000080-U-000007FF:  110xxxxx 10xxxxxx 
      if(overflow) { m-= 2; if(m< 0) break; }
      *p1++= (uint8_t) (c>> 6)        | 0xC0; /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
      *p1++= (uint8_t) (c&       0x3f)| 0x80; /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(c<= 0xFFFF) {     // 3 bytes  U-00000800-U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
      if(overflow) { m-= 3; if(m< 0) break; }
      *p1++= (uint8_t) (c>> 12)       | 0xE0; /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
      *p1++= (uint8_t)((c>> 6)&  0x3F)| 0x80; /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p1++= (uint8_t) (c&       0x3F)| 0x80; /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    } else if(c<= 0x001FFFFF) { // 4 bytes  U-00010000-U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
      if(overflow) { m-= 4; if(m< 0) break; }
      *p1++= (uint8_t) (c>> 18)       | 0xF0; /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
      *p1++= (uint8_t)((c>> 12)& 0x3F)| 0x80; /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
      *p1++= (uint8_t)((c>>  6)& 0x3F)| 0x80; /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
      *p1++= (uint8_t) (c&       0x3F)| 0x80; /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    }
    nrUnicodes++;
  } /// for each char/comb in s
  *p1= 0;                                   /// str terminator
  if(overflow) len= Str::strlen8(d);        /// wrapping overflow handling

  return *this;
}






///--------------------------------------------------------///
// character / string insertion deletion at specific points //
///--------------------------------------------------------///


// <in_unicode>: unicode value to insert in the string
// <in_pos>: insert position of <in_unicode> in current string;
//           if left -1, the unicode is inserted at the end of the string
//           the unicode value which this points to (and every unicodes after) is moved to the right
void str8::insert(char32_t in_unicode, int32_t in_pos) {
  int32_t l;
  if(wrapping) l= Str::insert8static(d, wrapSize, in_unicode, in_pos);
  else         l= Str::insert8(&d, in_unicode, in_pos);
  /// insert worked? (resulting str must be bigger that what is currently)
  if(l> len)
    nrUnicodes++, len= l;
}


// <in_str> - string (UTF-8) to insert
// <in_pos>: insert position of <in_str> in current string;
//           if left -1, the string is inserted at the end of the current string
//           the unicode value which this points to (and every unicodes after) is moved to the right
void str8::insertStr(const char *in_str, int32_t in_pos) {
  if(wrapping) Str::insertStr8static(d, wrapSize, in_str, in_pos);
  else         Str::insertStr8(&d, in_str, in_pos);
  updateLen();
}


// <in_nUnicodesToDel>- number of unicode values to del from this string
// <in_pos>- delete position- selected unicode is deleted
//           and every unicode to the left until <in_nrUnicodesToDel> is satisfied;
//           if it remains -1, the last unicode in the string is the position;
void str8::del(int32_t in_nUnicodesToDel, int32_t in_pos) {
  if(wrapping) Str::del8static(d, in_nUnicodesToDel, in_pos);
  else         Str::del8(&d, in_nUnicodesToDel, in_pos);
  updateLen();
}



///----------------------///
// search in string funcs //
///----------------------///

char *str8::search(const char *in_search, bool in_caseSensitive) {
  return Str::search8(d, in_search, in_caseSensitive);
}









