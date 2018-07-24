#include "util/strCommon.h"

// **WARNING**
// strncpy16 strchars16, strcombs16
//  ^^^ are based on the fact that currently, diacriticals are not packed into surrogates, if this changes, funcs need an update


//TODO:
/*
 * when formatting is done, a null check for string that is set to null would print null. or something
 * think about operator==: when comparing a NULL pointer to a wrapped no unicode inside string? too much? maybe
*/


//using namespace Str;

/// character case change helper structs (initialized at the back of the file)
struct _L2U {
  uint32_t l, u;
};



void _parseUTF8(const char *str, int32_t *outChars= NULL, int32_t *outCombs= NULL, int32_t *outUnicodes= NULL) {
  int32_t chars, combs, unicodes;
  int32_t *pchars= (outChars? outChars: &chars);
  int32_t *pcombs= (outCombs? outCombs: &combs);
  int32_t *punicodes= (outUnicodes? outUnicodes: &unicodes);

  *punicodes= *pchars= *pcombs= 0;

  for(uint8_t *p= (uint8_t *)str; *p; p++)
    if((*p& 0xc0)!= 0x80) {   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000

      (*punicodes)++;

      if(Str::isComb(Str::utf8to32((char *)p))) (*pcombs)++;
      else                                      (*pchars)++;
    }
}      

void _parseUTF16(const char16_t *str, int32_t *outChars= NULL, int32_t *outCombs= NULL, int32_t *outUnicodes= NULL, bool *hasSurrogates= NULL) {
  int32_t chars, combs, unicodes;
  bool surrogates;
  int32_t *pchars= (outChars? outChars: &chars);
  int32_t *pcombs= (outCombs? outCombs: &combs);
  int32_t *punicodes= (outUnicodes? outUnicodes: &unicodes);
  bool *psurrogates= (hasSurrogates? hasSurrogates: &surrogates);

  *punicodes= *pchars= *pcombs= 0;
  *psurrogates= false;

  for(uint16_t *p= (uint16_t *)str; *p; p++)
    if(!Str::isLowSurrogate(*p)) {
      (*punicodes)++;
      if(Str::isComb(Str::utf16to32((char16_t *)p)))
        (*pcombs)++;
      else
        (*pchars)++;

      if(Str::isSurrogate(*p))
        *psurrogates= true;
    }
}

void _parseUTF32(const char32_t *str, int32_t *outChars= NULL, int32_t *outCombs= NULL, int32_t *outUnicodes= NULL) {
  int32_t chars, combs, unicodes;
  int32_t *pchars= (outChars? outChars: &chars);
  int32_t *pcombs= (outCombs? outCombs: &combs);
  int32_t *punicodes= (outUnicodes? outUnicodes: &unicodes);

  *punicodes= *pchars= *pcombs= 0;

  for(uint32_t *p= (uint32_t *)str; *p; p++) {
    (*punicodes)++;
    if(Str::isComb(*p)) (*pcombs)++;
    else                (*pchars)++;
  }
}


// func that is used to check the validity of a UTF-32 unicode value
char32_t _secure32check(char32_t u) {
  if(Str::isSurrogate((uint32_t)u)) return 0xFFFD; // [security check] surrogates are marked as bad (0xFFFD) - no place for them in a UTF-32 str
  else if(u>  0x10FFFF)   return 0xFFFD; // [security check] limit of Unicode <--------------------- (maybe this changes in the future)
  else return u;
}


// internal func that is used for secureUTF16
char32_t _secure16to32advance(const char16_t **in_str) {
  char32_t c= 0;
  const uint16_t *p= (const uint16_t *)*in_str;

  if(Str::isHighSurrogate(*p)) {          // [security check] next byte must be avaible
    if(!*(p+ 1)) {
      (*((uint16_t**)in_str))++;
      return 0xFFFD;
    }
    if(!Str::isLowSurrogate(*(p+ 1))) {   // [security check] next byte must be a surrogate
      (*((uint16_t**)in_str))++;
      return 0xFFFD;
    }

    // successful unpack of surrogates
    c= (*p<< 10)+ *(p+ 1)+ Str::UTF16_SURROGATE_OFFSET;
    (*((uint16_t**)in_str))+= 2;
      
  } else if(Str::isLowSurrogate(*p)) {    // [security check] a low surrogate before a high surrogate causes a bad character
    (*((uint16_t**)in_str))++;
    c= 0xFFFD;
    return 0xFFFD;
  }

  // successful direct unicode copy
  else 
    c= *p,
    (*((uint16_t**)in_str))++;

  if(Str::isSurrogate(c)) c= 0xFFFD;      // [security check] the surrogates unpacked into another surrogate
  if(c> 0x10FFFF)         c= 0xFFFD;      // [security check] limit of unicode <-----------------------
  return c;
}



bool Str::isComb(char32_t c) {
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


int Str::utf8nrBytes(char32_t n) {
  if     (n<= 0x0000007F) return 1; /// 1 byte  U-00000000-> U-0000007F:  0xxxxxxx 
  else if(n<= 0x000007FF) return 2; /// 2 bytes U-00000080-> U-000007FF:  110xxxxx 10xxxxxx 
  else if(n<= 0x0000FFFF) return 3; /// 3 bytes U-00000800-> U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
  else if(n<= 0x001FFFFF) return 4; /// 4 bytes U-00010000-> U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
  else if(n<= 0x03FFFFFF) return 5; /// 5 bytes U-00200000-> U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
  else if(n<= 0x7FFFFFFF) return 6;
  else return 0;
}

// returns the number of bytes that the specified utf8 header byte reports it has
inline int Str::utf8headerBytes(char b) {
  if((uint8_t)b < 128)                 return 1;
  else if(((uint8_t)b& 0xe0) == 0xc0)  return 2;
  else if(((uint8_t)b& 0xf0) == 0xe0)  return 3;
  else if(((uint8_t)b& 0xf8) == 0xf0)  return 4;
  // the last 2 bytes are not used, but avaible if in the future unicode will expand
  else if(((uint8_t)b& 0xfc) == 0xf8)  return 5;
  else if(((uint8_t)b& 0xfe) == 0xfc)  return 6;
  
  return 0;
}



///=======================================///
// utf8-utf16-utf32 in-between conversions //
///=======================================///

/// UTF-8 to UTF-16 conversion
void Str::utf8to16(const char *in_str, char16_t *out_str, int32_t *out_utf8bytes, int32_t *out_utf16ints) {
  int32_t utf8, utf16;
  utf16= utf32to16(Str::utf8to32(in_str, &utf8), out_str);
  if(out_utf8bytes) *out_utf8bytes= utf8;
  if(out_utf16ints) *out_utf16ints= utf16;
}


/// converts UTF-8 unicode value to UTF-32
char32_t Str::utf8to32(const char *s, int32_t *out_bytes) {
  const uint8_t *p= (const uint8_t *)s;
  char32_t ret= 0;

  /// check how many chars this utf8 pack has
  int32_t n= 0;

  if(*p < 128)                n= 1, ret= *p++;
  else if((*p& 0xe0) == 0xc0) n= 2, ret= (*p++)& 0x1f;
  else if((*p& 0xf0) == 0xe0) n= 3, ret= (*p++)& 0x0f;
  else if((*p& 0xf8) == 0xf0) n= 4, ret= (*p++)& 0x07;
  else if((*p& 0xfc) == 0xf8) n= 5, ret= (*p++)& 0x03;    // 5bytes should not be used
  else if((*p& 0xfe) == 0xfc) n= 6, ret= (*p++)& 0x01;    // 6bytes should not be used
  else { if(out_bytes) *out_bytes= 1; return 0xFFFD; }

  /// optional number of bytes return value
  if(out_bytes)
    *out_bytes= n;

  /// unpack the utf8
  while(--n> 0)
    ret<<= 6, ret+= *p++ & 0x3f;

  return ret;
}



void Str::utf16to8(const char16_t *in_str, char *out_str) {
  utf32to8(Str::utf16to32(in_str), out_str);
}


char32_t Str::utf16to32(const char16_t *in_str, int32_t *out_nInt16) {
  if(isHighSurrogate(*in_str)) {
    if(out_nInt16) *out_nInt16= 2;
    return (*in_str<< 10)+ *(in_str+ 1)+ Str::UTF16_SURROGATE_OFFSET;
  } 
  if(out_nInt16) *out_nInt16= 1;
  return *in_str;
}



int32_t Str::utf32to8(char32_t n, char *in_dst) {
  uint8_t *dst= (uint8_t *)in_dst;
  /// compress unicode value into utf-8
  if(n<= 0x0000007F) {          //  1 byte   U-00000000->U-0000007F:  0xxxxxxx 
    *dst++= (uint8_t) n;
    return 1;
  } else if(n<= 0x000007FF) {   //  2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
    *dst++= (uint8_t) (n>> 6)        | 0xC0;   /// [BYTE 1]       >> 6= 000xxxxx 00000000  then header | c0 (11000000)
    *dst++= (uint8_t) (n&       0x3f)| 0x80;   /// [BYTE 2]         3f= 00000000 00xxxxxx  then header | 80 (10000000)
    return 2;
  } else if(n<= 0x0000FFFF) {   //  3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
    *dst++= (uint8_t) (n>> 12)       | 0xE0;   /// [BYTE 1]      >> 12= 0000xxxx 00000000 00000000  then header | e0 (11100000)
    *dst++= (uint8_t)((n>> 6)&  0x3F)| 0x80;   /// [BYTE 2]  >> 6 & 3f= 00000000 00xxxxxx 00000000  then header | 80 (10000000)
    *dst++= (uint8_t) (n&       0x3F)| 0x80;   /// [BYTE 3]       & 3f= 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    return 3;
  } else if(n<= 0x001FFFFF) {   // 4 bytes U-00010000->U-001FFFFF:    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
    *dst++= (uint8_t) (n>> 18)       | 0xF0;   /// [BYTE 1]      >> 18= 00000xxx 00000000 00000000 00000000  then header | f0 (11110000)
    *dst++= (uint8_t)((n>> 12)& 0x3F)| 0x80;   /// [BYTE 2] >> 12 & 3f= 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
    *dst++= (uint8_t)((n>>  6)& 0x3F)| 0x80;   /// [BYTE 3] >>  6 & 3f= 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
    *dst++= (uint8_t) (n&       0x3F)| 0x80;   /// [BYTE 4]       & 3f= 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    return 4;
  // last 2 bytes, UNUSED by utf ATM, but there be the code
  } else if(n<= 0x03FFFFFF) {   //  5 bytes U-00200000->U-03FFFFFF:   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
    *dst++= (uint8_t) (n>> 24)       | 0xF8;   /// [BYTE 1]      >> 24= 000000xx 00000000 00000000 00000000 00000000  then header | f8 (11111000)
    *dst++= (uint8_t)((n>> 18)& 0x3f)| 0x80;   /// [BYTE 2] >> 18 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
    *dst++= (uint8_t)((n>> 12)& 0x3f)| 0x80;   /// [BYTE 3] >> 12 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
    *dst++= (uint8_t)((n>>  6)& 0x3f)| 0x80;   /// [BYTE 4] >>  6 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
    *dst++= (uint8_t) (n&       0x3f)| 0x80;   /// [BYTE 5]       & 3f= 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    return 5;
  } else if(n<= 0x7FFFFFFF) {   //  6 bytes U-04000000->U-7FFFFFFF:   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    *dst++= (uint8_t) (n>> 30)       | 0xFC;   /// [BYTE 1]      >> 30= 0000000x 00000000 00000000 00000000 00000000 00000000  then header | fc (11111100)
    *dst++= (uint8_t)((n>> 24)& 0x3f)| 0x80;   /// [BYTE 2] >> 24 & 3f= 00000000 00xxxxxx 00000000 00000000 00000000 00000000  then header | 80 (10000000)
    *dst++= (uint8_t)((n>> 18)& 0x3f)| 0x80;   /// [BYTE 3] >> 18 & 3f= 00000000 00000000 00xxxxxx 00000000 00000000 00000000  then header | 80 (10000000)
    *dst++= (uint8_t)((n>> 12)& 0x3f)| 0x80;   /// [BYTE 4] >> 12 & 3f= 00000000 00000000 00000000 00xxxxxx 00000000 00000000  then header | 80 (10000000)
    *dst++= (uint8_t)((n>>  6)& 0x3f)| 0x80;   /// [BYTE 5] >>  6 & 3f= 00000000 00000000 00000000 00000000 00xxxxxx 00000000  then header | 80 (10000000)
    *dst++= (uint8_t) (n&       0x3f)| 0x80;   /// [BYTE 6]         3f= 00000000 00000000 00000000 00000000 00000000 00xxxxxx  then header | 80 (10000000)
    return 6;
  }
  return 0;
  

  /* short version
  int32_t a= -1, ret;
  /// compress unicode value into utf-8
  if(n<= 0x0000007F) {          //  1 byte   U-00000000->U-0000007F:  0xxxxxxx 
    *dst++= (uint8_t) n;
    return 1;
  }
  else if(n<= 0x000007FF) *dst++= (uint8_t) (n>> 6) | 0xC0, a= 0, ret= 2;   // 2 bytes  U-00000080->U-000007FF:  110xxxxx 10xxxxxx 
  else if(n<= 0x0000FFFF) *dst++= (uint8_t) (n>> 12)| 0xE0, a= 6, ret= 3;   // 3 bytes  U-00000800->U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx 
  else if(n<= 0x001FFFFF) *dst++= (uint8_t) (n>> 18)| 0xF0, a= 12, ret= 4;  // 4 bytes  U-00010000->U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
  // last 2 bytes, UNUSED by utf ATM, but there be the code
  else if(n<= 0x03FFFFFF) *dst++= (uint8_t) (n>> 24)| 0xF8, a= 18, ret= 5;  // 5 bytes  U-00200000->U-03FFFFFF:   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
  else if(n<= 0x7FFFFFFF) *dst++= (uint8_t) (n>> 30)| 0xFC, a= 24, ret= 6;  // 6 bytes  U-04000000->U-7FFFFFFF:   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

  while(a>= 0)
    *dst++= (uint8_t)((n>>  a)& 0x3f)| 0x80, a-= 6;

  return ret;
  */
}


int32_t Str::utf32to16(char32_t unicode, char16_t *out_str) {
  if(unicode>= 0x10000) {
    *((uint16_t *)out_str)=    Str::UTF16_LEAD_OFFSET+ (unicode>> 10);
    *((uint16_t *)out_str+ 1)= 0xDC00+ (unicode& 0x3FF);
    return 2;
  } else {
    *out_str= (char16_t)unicode;
    return 1;
  }
}


/// returns unicode value [n] in UTF-8 string as UTF-32 (direct unicode value)
char32_t Str::utf8to32n(const char *s, int n) {
  const uint8_t *p= (const uint8_t *)s;
  /// pass thru all characters, till n is reached
  while(n> 0)
    if((*p++ & 0xc0)!= 0x80)
      n--;

  return utf8to32((const char *)(p- 1));
}






///====================================================================///
// getChar funcs - get a specific character / unicode value in a string //
///====================================================================///


/// returns [n]'th unicode value in the string - it can be a character or a diacritical
char *Str::getUnicode8(const char *s, int32_t n) {
  uint8_t* p= (uint8_t *)s;

  while((n> 0) && *p)
    if((*++p& 0xc0)!= 0x80)   /// count only utf8 header bytes
      n--;

  return (char *)p;
}

// UTF-16 str: returns the [n] unicode value in string (does not care if it's a normal character or diacritical) 
char16_t *Str::getUnicode16(const char16_t *s, int32_t n) {
  uint16_t *p= (uint16_t *)s;
  while((n> 0) && *p)
    if(!isLowSurrogate(*++p))
      n--;

  return (char16_t *)p;
}


char32_t *Str::getUnicode32(const char32_t *s, int32_t n) {
  uint32_t *p= (uint32_t *)s;
  while((n> 0) && *p)
    n--, p++;
  return (char32_t *)p;
}



/// return [n]'th character (WARNING: a character can be made of multiple diacriticals)
char *Str::getChar8(const char *s, int32_t n) {
  uint8_t* p= (uint8_t *)s;

  while((n> 0) && *p) {
    int32_t nbytes;
    if(!isComb(utf8to32((const char *)p, &nbytes)))
      n--;
    p+= nbytes;
  }

  return (char *)p;
}

// UTF-16 str: returns the [n] char in string (diacriticals are considered part of a character)
char16_t *Str::getChar16(const char16_t *s, int32_t n) {
  uint16_t *p= (uint16_t *)s;
  while((n> 0) && *p) {
    if(!isComb(utf16to32((const char16_t *)(p++))))
      n--;
    if(isLowSurrogate(*p))
      p++;
  }

  return (char16_t *)p;
}


char32_t *Str::getChar32(const char32_t *s, int32_t n) {
  uint32_t *p= (uint32_t *)s;
  while((n> 0) && *p)
    if(!isComb(*p++))
      n--;
  return (char32_t *)p;
}


int32_t Str::getUnicodesInChar8(const char *in_s) {
  int32_t ret= 0;
  uint32_t u;
  while(*in_s) {
    in_s= (const char *)utf8to32fast((const uint8_t *)in_s, &u);
    ret++;
    if(!isComb(u))
      break;
  }

  return ret;
}


int32_t Str::getUnicodesInChar16(const char16_t *in_s) {
  int32_t ret= 0;
  uint32_t u;
  while(*in_s) {
    in_s= (const char16_t *)utf16to32fast((const uint16_t *)in_s, &u);
    ret++;
    if(!isComb(u))
      break;
  }

  return ret;
}


int32_t Str::getUnicodesInChar32(const char32_t *in_s) {
  int32_t ret= 0;
  while(*in_s) {
    ret++;
    if(!isComb(*in_s))
      break;
    in_s++;
  }

  return ret;
}




int32_t Str::getUnicodesInChars8(const char *in_s, int32_t in_maxChars) {
  int32_t ret= 0;
  while(in_maxChars && *in_s) {
    uint32_t c;
    in_s= (const char *)utf8to32fast((const uint8_t *)in_s, &c);
    if(!isComb(c))
    in_maxChars--;
    in_s++;
    ret++;
  }
  return ret;
}



int32_t Str::getUnicodesInChars16(const char16_t *in_s, int32_t in_maxChars) {
  int32_t ret= 0;
  while(in_maxChars && *in_s) {
    uint32_t c;
    in_s= (const char16_t *)utf16to32fast((const uint16_t *)in_s, &c);
    if(!isComb(c))
    in_maxChars--;
    in_s++;
    ret++;
  }
  return ret;
}



int32_t Str::getUnicodesInChars32(const char32_t *in_s, int32_t in_maxChars) {
  int32_t ret= 0;
  while(in_maxChars && *in_s) {
    if(!isComb(*in_s))
    in_maxChars--;
    in_s++;
    ret++;
  }
  return ret;
}






///=====================================///
// strlen funcs - string length in bytes //
///=====================================///

/// returns utf-8 string length in BYTES (includes str terminator)
int32_t Str::strlen8(const char *s) {
  if(!s) return 0;
  uint8_t *p= (uint8_t *)s;
  while(*p++);
  return (int32_t)(p- (uint8_t *)s);
}

/// returns utf-16 string length in BYTES (includes str terminator)
int32_t Str::strlen16(const char16_t *s) {
  if(!s) return 0;
  uint16_t *p= (uint16_t *)s;
  while(*p++);
  return (int32_t)((uint8_t *)p- (uint8_t *)s);
}

/// returns utf-32 string length in BYTES (includes str terminator)
int32_t Str::strlen32(const char32_t *s) {
  if(!s) return 0;
  uint32_t *p= (uint32_t *)s;
  while(*p++);
  return (int32_t)((uint8_t *)p- (uint8_t *)s);
}





///====================================///
// strcpy / strncpy - string copy funcs //
///====================================///


/// copies src to dest (both utf-8 array strings
void Str::strcpy8(char *dst, const char *src, bool terminator) {
  uint8_t *p1= (uint8_t *)dst, *p2= (uint8_t *)src;

  while(*p2)
    *p1++= *p2++;

  if(terminator) *p1= 0;
}


/// copies n unicode values from src to dst
void Str::strncpy8(char *dst, const char *src, int32_t n, bool terminator) {
  uint8_t *p1= (uint8_t *)dst, *p2= (uint8_t *)src;
  while(n--> 0) {
    int32_t nbytes= utf8headerBytes(*p2);

    while(nbytes--)
      *p1++= *p2++;
  }
  if(terminator) *p1= 0;
}


void Str::strcpy16(char16_t *dst, const char16_t *src, bool terminator) {
  uint16_t *p1= (uint16_t *)dst, *p2= (uint16_t *)src;
  while(*p2)
    *p1++= *p2++;
  if(terminator) *p1= 0;  /// terminator
}


void Str::strncpy16(char16_t *dst, const char16_t *src, int32_t n, bool terminator) {
  uint16_t *p1= (uint16_t *)dst, *p2= (uint16_t *)src;
  while(n--> 0) {
    *p1++= *p2++;
    if(isLowSurrogate(*p2))
      *p1++= *p2++;
  }

  if(terminator) *p2= 0;
}


void Str::strcpy32(char32_t *dst, const char32_t *src, bool terminator) {
  uint32_t *p1= (uint32_t *)dst, *p2= (uint32_t *)src;
  while(*p2)
    *p1++= *p2++;
  if(terminator) *p1= 0;  /// terminator
}


void Str::strncpy32(char32_t *dst, const char32_t *src, int32_t n, bool terminator) {
  uint32_t *p1= (uint32_t *)dst, *p2= (uint32_t *)src;
  while(n--> 0)
    *p1++= *p2++;
  if(terminator) *p1= 0;
}




///================================///
// string various characters COUNTS //
///================================///

/// returns number of unicode values in a string
int32_t Str::strunicodes8(const char *s) {
  if(!s) return 0;
  int32_t ret= 0;

  for(uint8_t *p= (uint8_t *)s; *p; p++)
    if((*p& 0xc0)!= 0x80)   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      ret++;

  return ret;
}

/// nr characters in UTF-8 array string (comb diacriticals are considered part of a character, so those unicode values are not counted)
int32_t Str::strchars8(const char *s) {
  if(!s) return 0;

  int32_t l= 0;
  for(uint8_t *p= (uint8_t *)s; *p; p++)
    if((*p& 0xc0)!= 0x80)   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      if(!isComb(utf8to32((char *)p)))
        l++;

  return l;
}

/// counts the number of comb diacriticalsin UTF-8 array string
int32_t Str::strcombs8(const char *s) {
  if(!s) return 0;

  int32_t ret= 0;
  for(uint8_t *p= (uint8_t *)s; *p; p++)
    if((*p& 0xc0)!= 0x80)   /// 0xc0= 11000000, first two bits  0x80= 10000000, test for 10xxxxxx the last 6 bits wont matter, as they are not tested so test to 10000000
      if(isComb(utf8to32((char *)p)))
        ret++;

  return ret;
}

/// counts the number of unicode values in a UTF-16 str
int32_t Str::strunicodes16(const char16_t *s) {
  if(!s) return 0;

  int32_t ret= 0;
  for(uint16_t *p= (uint16_t *)s; *p; p++)
     if(!isLowSurrogate(*p))
      ret++;

  return ret;
}

/// nr characters in UTF-16 array string (comb diacriticals are considered part of a character, so those unicode values are not counted)
int32_t Str::strchars16(const char16_t *s) {
  if(!s) return 0;
  
  int32_t ret= 0;
  for(uint32_t *p= (uint32_t *)s; *p; p++)
    if(!isLowSurrogate(*p))
      if(!isComb(*p))
        ret++;

  return ret;
}

/// counts the number of comb diacriticals in UTF-16 array string
int32_t Str::strcombs16(const char16_t *s) {
  if(!s) return 0;

  int32_t ret= 0;
  for(uint32_t *p= (uint32_t *)s; *p; p++)
    if(isComb(*p)) ret++;     // ATM COMBS CAN'T BE INSIDE SURROGATES

  return ret;
}

/// counts the number of unicode values in a string
int32_t Str::strunicodes32(const char32_t *s) {
  if(!s) return 0;
  uint32_t *p= (uint32_t *)s;
  for(; *p; p++);
  return (int32_t)(p- (uint32_t *)s);
}

/// nr characters in UTF-32 array string (comb diacriticals are considered part of a character, so those unicode values are not counted)
int32_t Str::strchars32(const char32_t *s) {
  if(!s) return 0;

  int32_t ret= 0;
  for(uint32_t *p= (uint32_t *)s; *p; p++)
    if(!isComb(*p)) ret++;

  return ret;
}

/// counts the number of comb diacriticals in UTF-32 array string
int32_t Str::strcombs32(const char32_t *s) {
  if(!s) return 0;

  int32_t ret= 0;
  for(uint32_t *p= (uint32_t *)s; *p; p++)
    if(isComb(*p)) ret++;

  return ret;
}






///=================///
// strings comparing //
///=================///

int32_t Str::strcmp8(const char *s1, const char *s2) {
  /// if both are NULL, then there's no difference
  if(s2== NULL) {
    if(s1== NULL) return 0;
    else          return 1;
  }
  if(s1== NULL) return -1;

  uint8_t *p1= (uint8_t *)s1, *p2= (uint8_t *)s2;

  /// pass thru each character to check for differences
  while(*p1 && *p2)
    if(*p1 != *p2)
      return -1;
    else p1++, p2++;

  if(*p1) return 1;   /// still stuff in str1, strings not the same
  if(*p2) return -1;  /// still stuff in str2, strings not the same

  return 0;           // reached this point, there is no difference
}


int32_t Str::strcmp16(const char16_t *s1, const char16_t *s2) {
  /// if both are NULL, then there's no difference
  if(s2== NULL) {
    if(s1== NULL) return 0;
    else          return 1;
  }
  if(s1== NULL) return -1;

  /// pass thru each character to check for differences
  uint16_t *p1= (uint16_t *)s1, *p2= (uint16_t *)s2;
  while(*p1 && *p2)
    if(*p1 != *p2)
      return -1;
    else p1++, p2++;

  if(*p1) return 1;   /// still stuff in str1, strings not the same
  if(*p2) return -1;  /// still stuff in str2, strings not the same

  return 0;           // reached this point, there is no difference
}


int32_t Str::strcmp32(const char32_t *s1, const char32_t *s2) {
  /// if both are NULL, then there's no difference
  if(s2== NULL) {
    if(s1== NULL) return 0;
    else          return 1;
  }
  if(s1== NULL) return -1;

  /// pass thru each character to check for differences
  uint32_t *p1= (uint32_t *)s1, *p2= (uint32_t *)s2;
  while(*p1 && *p2)
    if(*p1 != *p2)
      return -1;
    else p1++, p2++;

  if(*p1) return 1;   /// still stuff in str1, strings not the same
  if(*p2) return -1;  /// still stuff in str2, strings not the same

  return 0;           // reached this point, there is no difference
}








///-----------------///
// CASE CHANGE funcs //
///-----------------///


// these 2 databases are used to convert from uppercase to lowercase and vice-versa;
// check toupper and tolower funcs; the search algorithm cycle is 10 passes max, so not a big time sink;

/// character case change; lowercase to uppercase
_L2U _cl2u[]= {
  0x000061, 0x000041, 0x000062, 0x000042, 0x000063, 0x000043, 0x000064, 0x000044,
  0x000065, 0x000045, 0x000066, 0x000046, 0x000067, 0x000047, 0x000068, 0x000048,
  0x000069, 0x000049, 0x00006A, 0x00004A, 0x00006B, 0x00004B, 0x00006C, 0x00004C,
  0x00006D, 0x00004D, 0x00006E, 0x00004E, 0x00006F, 0x00004F, 0x000070, 0x000050,
  0x000071, 0x000051, 0x000072, 0x000052, 0x000073, 0x000053, 0x000074, 0x000054,
  0x000075, 0x000055, 0x000076, 0x000056, 0x000077, 0x000057, 0x000078, 0x000058,
  0x000079, 0x000059, 0x00007A, 0x00005A, 0x0000B5, 0x00039C, 0x0000E0, 0x0000C0,
  0x0000E1, 0x0000C1, 0x0000E2, 0x0000C2, 0x0000E3, 0x0000C3, 0x0000E4, 0x0000C4,
  0x0000E5, 0x0000C5, 0x0000E6, 0x0000C6, 0x0000E7, 0x0000C7, 0x0000E8, 0x0000C8,
  0x0000E9, 0x0000C9, 0x0000EA, 0x0000CA, 0x0000EB, 0x0000CB, 0x0000EC, 0x0000CC,
  0x0000ED, 0x0000CD, 0x0000EE, 0x0000CE, 0x0000EF, 0x0000CF, 0x0000F0, 0x0000D0,
  0x0000F1, 0x0000D1, 0x0000F2, 0x0000D2, 0x0000F3, 0x0000D3, 0x0000F4, 0x0000D4,
  0x0000F5, 0x0000D5, 0x0000F6, 0x0000D6, 0x0000F8, 0x0000D8, 0x0000F9, 0x0000D9,
  0x0000FA, 0x0000DA, 0x0000FB, 0x0000DB, 0x0000FC, 0x0000DC, 0x0000FD, 0x0000DD,
  0x0000FE, 0x0000DE, 0x0000FF, 0x000178, 0x000101, 0x000100, 0x000103, 0x000102,
  0x000105, 0x000104, 0x000107, 0x000106, 0x000109, 0x000108, 0x00010B, 0x00010A,
  0x00010D, 0x00010C, 0x00010F, 0x00010E, 0x000111, 0x000110, 0x000113, 0x000112,
  0x000115, 0x000114, 0x000117, 0x000116, 0x000119, 0x000118, 0x00011B, 0x00011A,
  0x00011D, 0x00011C, 0x00011F, 0x00011E, 0x000121, 0x000120, 0x000123, 0x000122,
  0x000125, 0x000124, 0x000127, 0x000126, 0x000129, 0x000128, 0x00012B, 0x00012A,
  0x00012D, 0x00012C, 0x00012F, 0x00012E, 0x000131, 0x000049, 0x000133, 0x000132,
  0x000135, 0x000134, 0x000137, 0x000136, 0x00013A, 0x000139, 0x00013C, 0x00013B,
  0x00013E, 0x00013D, 0x000140, 0x00013F, 0x000142, 0x000141, 0x000144, 0x000143,
  0x000146, 0x000145, 0x000148, 0x000147, 0x00014B, 0x00014A, 0x00014D, 0x00014C,
  0x00014F, 0x00014E, 0x000151, 0x000150, 0x000153, 0x000152, 0x000155, 0x000154,
  0x000157, 0x000156, 0x000159, 0x000158, 0x00015B, 0x00015A, 0x00015D, 0x00015C,
  0x00015F, 0x00015E, 0x000161, 0x000160, 0x000163, 0x000162, 0x000165, 0x000164,
  0x000167, 0x000166, 0x000169, 0x000168, 0x00016B, 0x00016A, 0x00016D, 0x00016C,
  0x00016F, 0x00016E, 0x000171, 0x000170, 0x000173, 0x000172, 0x000175, 0x000174,
  0x000177, 0x000176, 0x00017A, 0x000179, 0x00017C, 0x00017B, 0x00017E, 0x00017D,
  0x00017F, 0x000053, 0x000180, 0x000243, 0x000183, 0x000182, 0x000185, 0x000184,
  0x000188, 0x000187, 0x00018C, 0x00018B, 0x000192, 0x000191, 0x000195, 0x0001F6,
  0x000199, 0x000198, 0x00019A, 0x00023D, 0x00019E, 0x000220, 0x0001A1, 0x0001A0,
  0x0001A3, 0x0001A2, 0x0001A5, 0x0001A4, 0x0001A8, 0x0001A7, 0x0001AD, 0x0001AC,
  0x0001B0, 0x0001AF, 0x0001B4, 0x0001B3, 0x0001B6, 0x0001B5, 0x0001B9, 0x0001B8,
  0x0001BD, 0x0001BC, 0x0001BF, 0x0001F7, 0x0001C6, 0x0001C4, 0x0001C9, 0x0001C7,
  0x0001CC, 0x0001CA, 0x0001CE, 0x0001CD, 0x0001D0, 0x0001CF, 0x0001D2, 0x0001D1,
  0x0001D4, 0x0001D3, 0x0001D6, 0x0001D5, 0x0001D8, 0x0001D7, 0x0001DA, 0x0001D9,
  0x0001DC, 0x0001DB, 0x0001DD, 0x00018E, 0x0001DF, 0x0001DE, 0x0001E1, 0x0001E0,
  0x0001E3, 0x0001E2, 0x0001E5, 0x0001E4, 0x0001E7, 0x0001E6, 0x0001E9, 0x0001E8,
  0x0001EB, 0x0001EA, 0x0001ED, 0x0001EC, 0x0001EF, 0x0001EE, 0x0001F3, 0x0001F1,
  0x0001F5, 0x0001F4, 0x0001F9, 0x0001F8, 0x0001FB, 0x0001FA, 0x0001FD, 0x0001FC,
  0x0001FF, 0x0001FE, 0x000201, 0x000200, 0x000203, 0x000202, 0x000205, 0x000204,
  0x000207, 0x000206, 0x000209, 0x000208, 0x00020B, 0x00020A, 0x00020D, 0x00020C,
  0x00020F, 0x00020E, 0x000211, 0x000210, 0x000213, 0x000212, 0x000215, 0x000214,
  0x000217, 0x000216, 0x000219, 0x000218, 0x00021B, 0x00021A, 0x00021D, 0x00021C,
  0x00021F, 0x00021E, 0x000223, 0x000222, 0x000225, 0x000224, 0x000227, 0x000226,
  0x000229, 0x000228, 0x00022B, 0x00022A, 0x00022D, 0x00022C, 0x00022F, 0x00022E,
  0x000231, 0x000230, 0x000233, 0x000232, 0x00023C, 0x00023B, 0x00023F, 0x002C7E,
  0x000240, 0x002C7F, 0x000242, 0x000241, 0x000247, 0x000246, 0x000249, 0x000248,
  0x00024B, 0x00024A, 0x00024D, 0x00024C, 0x00024F, 0x00024E, 0x000250, 0x002C6F,
  0x000251, 0x002C6D, 0x000252, 0x002C70, 0x000253, 0x000181, 0x000254, 0x000186,
  0x000256, 0x000189, 0x000257, 0x00018A, 0x000259, 0x00018F, 0x00025B, 0x000190,
  0x000260, 0x000193, 0x000263, 0x000194, 0x000265, 0x00A78D, 0x000266, 0x00A7AA,
  0x000268, 0x000197, 0x000269, 0x000196, 0x00026B, 0x002C62, 0x00026F, 0x00019C,
  0x000271, 0x002C6E, 0x000272, 0x00019D, 0x000275, 0x00019F, 0x00027D, 0x002C64,
  0x000280, 0x0001A6, 0x000283, 0x0001A9, 0x000288, 0x0001AE, 0x000289, 0x000244,
  0x00028A, 0x0001B1, 0x00028B, 0x0001B2, 0x00028C, 0x000245, 0x000292, 0x0001B7,
  0x000371, 0x000370, 0x000373, 0x000372, 0x000377, 0x000376, 0x00037B, 0x0003FD,
  0x00037C, 0x0003FE, 0x00037D, 0x0003FF, 0x0003AC, 0x000386, 0x0003AD, 0x000388,
  0x0003AE, 0x000389, 0x0003AF, 0x00038A, 0x0003B1, 0x000391, 0x0003B2, 0x000392,
  0x0003B3, 0x000393, 0x0003B4, 0x000394, 0x0003B5, 0x000395, 0x0003B6, 0x000396,
  0x0003B7, 0x000397, 0x0003B8, 0x000398, 0x0003B9, 0x000399, 0x0003BA, 0x00039A,
  0x0003BB, 0x00039B, 0x0003BC, 0x00039C, 0x0003BD, 0x00039D, 0x0003BE, 0x00039E,
  0x0003BF, 0x00039F, 0x0003C0, 0x0003A0, 0x0003C1, 0x0003A1, 0x0003C2, 0x0003A3,
  0x0003C3, 0x0003A3, 0x0003C4, 0x0003A4, 0x0003C5, 0x0003A5, 0x0003C6, 0x0003A6,
  0x0003C7, 0x0003A7, 0x0003C8, 0x0003A8, 0x0003C9, 0x0003A9, 0x0003CA, 0x0003AA,
  0x0003CB, 0x0003AB, 0x0003CC, 0x00038C, 0x0003CD, 0x00038E, 0x0003CE, 0x00038F,
  0x0003D0, 0x000392, 0x0003D1, 0x000398, 0x0003D5, 0x0003A6, 0x0003D6, 0x0003A0,
  0x0003D7, 0x0003CF, 0x0003D9, 0x0003D8, 0x0003DB, 0x0003DA, 0x0003DD, 0x0003DC,
  0x0003DF, 0x0003DE, 0x0003E1, 0x0003E0, 0x0003E3, 0x0003E2, 0x0003E5, 0x0003E4,
  0x0003E7, 0x0003E6, 0x0003E9, 0x0003E8, 0x0003EB, 0x0003EA, 0x0003ED, 0x0003EC,
  0x0003EF, 0x0003EE, 0x0003F0, 0x00039A, 0x0003F1, 0x0003A1, 0x0003F2, 0x0003F9,
  0x0003F5, 0x000395, 0x0003F8, 0x0003F7, 0x0003FB, 0x0003FA, 0x000430, 0x000410,
  0x000431, 0x000411, 0x000432, 0x000412, 0x000433, 0x000413, 0x000434, 0x000414,
  0x000435, 0x000415, 0x000436, 0x000416, 0x000437, 0x000417, 0x000438, 0x000418,
  0x000439, 0x000419, 0x00043A, 0x00041A, 0x00043B, 0x00041B, 0x00043C, 0x00041C,
  0x00043D, 0x00041D, 0x00043E, 0x00041E, 0x00043F, 0x00041F, 0x000440, 0x000420,
  0x000441, 0x000421, 0x000442, 0x000422, 0x000443, 0x000423, 0x000444, 0x000424,
  0x000445, 0x000425, 0x000446, 0x000426, 0x000447, 0x000427, 0x000448, 0x000428,
  0x000449, 0x000429, 0x00044A, 0x00042A, 0x00044B, 0x00042B, 0x00044C, 0x00042C,
  0x00044D, 0x00042D, 0x00044E, 0x00042E, 0x00044F, 0x00042F, 0x000450, 0x000400,
  0x000451, 0x000401, 0x000452, 0x000402, 0x000453, 0x000403, 0x000454, 0x000404,
  0x000455, 0x000405, 0x000456, 0x000406, 0x000457, 0x000407, 0x000458, 0x000408,
  0x000459, 0x000409, 0x00045A, 0x00040A, 0x00045B, 0x00040B, 0x00045C, 0x00040C,
  0x00045D, 0x00040D, 0x00045E, 0x00040E, 0x00045F, 0x00040F, 0x000461, 0x000460,
  0x000463, 0x000462, 0x000465, 0x000464, 0x000467, 0x000466, 0x000469, 0x000468,
  0x00046B, 0x00046A, 0x00046D, 0x00046C, 0x00046F, 0x00046E, 0x000471, 0x000470,
  0x000473, 0x000472, 0x000475, 0x000474, 0x000477, 0x000476, 0x000479, 0x000478,
  0x00047B, 0x00047A, 0x00047D, 0x00047C, 0x00047F, 0x00047E, 0x000481, 0x000480,
  0x00048B, 0x00048A, 0x00048D, 0x00048C, 0x00048F, 0x00048E, 0x000491, 0x000490,
  0x000493, 0x000492, 0x000495, 0x000494, 0x000497, 0x000496, 0x000499, 0x000498,
  0x00049B, 0x00049A, 0x00049D, 0x00049C, 0x00049F, 0x00049E, 0x0004A1, 0x0004A0,
  0x0004A3, 0x0004A2, 0x0004A5, 0x0004A4, 0x0004A7, 0x0004A6, 0x0004A9, 0x0004A8,
  0x0004AB, 0x0004AA, 0x0004AD, 0x0004AC, 0x0004AF, 0x0004AE, 0x0004B1, 0x0004B0,
  0x0004B3, 0x0004B2, 0x0004B5, 0x0004B4, 0x0004B7, 0x0004B6, 0x0004B9, 0x0004B8,
  0x0004BB, 0x0004BA, 0x0004BD, 0x0004BC, 0x0004BF, 0x0004BE, 0x0004C2, 0x0004C1,
  0x0004C4, 0x0004C3, 0x0004C6, 0x0004C5, 0x0004C8, 0x0004C7, 0x0004CA, 0x0004C9,
  0x0004CC, 0x0004CB, 0x0004CE, 0x0004CD, 0x0004CF, 0x0004C0, 0x0004D1, 0x0004D0,
  0x0004D3, 0x0004D2, 0x0004D5, 0x0004D4, 0x0004D7, 0x0004D6, 0x0004D9, 0x0004D8,
  0x0004DB, 0x0004DA, 0x0004DD, 0x0004DC, 0x0004DF, 0x0004DE, 0x0004E1, 0x0004E0,
  0x0004E3, 0x0004E2, 0x0004E5, 0x0004E4, 0x0004E7, 0x0004E6, 0x0004E9, 0x0004E8,
  0x0004EB, 0x0004EA, 0x0004ED, 0x0004EC, 0x0004EF, 0x0004EE, 0x0004F1, 0x0004F0,
  0x0004F3, 0x0004F2, 0x0004F5, 0x0004F4, 0x0004F7, 0x0004F6, 0x0004F9, 0x0004F8,
  0x0004FB, 0x0004FA, 0x0004FD, 0x0004FC, 0x0004FF, 0x0004FE, 0x000501, 0x000500,
  0x000503, 0x000502, 0x000505, 0x000504, 0x000507, 0x000506, 0x000509, 0x000508,
  0x00050B, 0x00050A, 0x00050D, 0x00050C, 0x00050F, 0x00050E, 0x000511, 0x000510,
  0x000513, 0x000512, 0x000515, 0x000514, 0x000517, 0x000516, 0x000519, 0x000518,
  0x00051B, 0x00051A, 0x00051D, 0x00051C, 0x00051F, 0x00051E, 0x000521, 0x000520,
  0x000523, 0x000522, 0x000525, 0x000524, 0x000527, 0x000526, 0x000561, 0x000531,
  0x000562, 0x000532, 0x000563, 0x000533, 0x000564, 0x000534, 0x000565, 0x000535,
  0x000566, 0x000536, 0x000567, 0x000537, 0x000568, 0x000538, 0x000569, 0x000539,
  0x00056A, 0x00053A, 0x00056B, 0x00053B, 0x00056C, 0x00053C, 0x00056D, 0x00053D,
  0x00056E, 0x00053E, 0x00056F, 0x00053F, 0x000570, 0x000540, 0x000571, 0x000541,
  0x000572, 0x000542, 0x000573, 0x000543, 0x000574, 0x000544, 0x000575, 0x000545,
  0x000576, 0x000546, 0x000577, 0x000547, 0x000578, 0x000548, 0x000579, 0x000549,
  0x00057A, 0x00054A, 0x00057B, 0x00054B, 0x00057C, 0x00054C, 0x00057D, 0x00054D,
  0x00057E, 0x00054E, 0x00057F, 0x00054F, 0x000580, 0x000550, 0x000581, 0x000551,
  0x000582, 0x000552, 0x000583, 0x000553, 0x000584, 0x000554, 0x000585, 0x000555,
  0x000586, 0x000556, 0x001D79, 0x00A77D, 0x001D7D, 0x002C63, 0x001E01, 0x001E00,
  0x001E03, 0x001E02, 0x001E05, 0x001E04, 0x001E07, 0x001E06, 0x001E09, 0x001E08,
  0x001E0B, 0x001E0A, 0x001E0D, 0x001E0C, 0x001E0F, 0x001E0E, 0x001E11, 0x001E10,
  0x001E13, 0x001E12, 0x001E15, 0x001E14, 0x001E17, 0x001E16, 0x001E19, 0x001E18,
  0x001E1B, 0x001E1A, 0x001E1D, 0x001E1C, 0x001E1F, 0x001E1E, 0x001E21, 0x001E20,
  0x001E23, 0x001E22, 0x001E25, 0x001E24, 0x001E27, 0x001E26, 0x001E29, 0x001E28,
  0x001E2B, 0x001E2A, 0x001E2D, 0x001E2C, 0x001E2F, 0x001E2E, 0x001E31, 0x001E30,
  0x001E33, 0x001E32, 0x001E35, 0x001E34, 0x001E37, 0x001E36, 0x001E39, 0x001E38,
  0x001E3B, 0x001E3A, 0x001E3D, 0x001E3C, 0x001E3F, 0x001E3E, 0x001E41, 0x001E40,
  0x001E43, 0x001E42, 0x001E45, 0x001E44, 0x001E47, 0x001E46, 0x001E49, 0x001E48,
  0x001E4B, 0x001E4A, 0x001E4D, 0x001E4C, 0x001E4F, 0x001E4E, 0x001E51, 0x001E50,
  0x001E53, 0x001E52, 0x001E55, 0x001E54, 0x001E57, 0x001E56, 0x001E59, 0x001E58,
  0x001E5B, 0x001E5A, 0x001E5D, 0x001E5C, 0x001E5F, 0x001E5E, 0x001E61, 0x001E60,
  0x001E63, 0x001E62, 0x001E65, 0x001E64, 0x001E67, 0x001E66, 0x001E69, 0x001E68,
  0x001E6B, 0x001E6A, 0x001E6D, 0x001E6C, 0x001E6F, 0x001E6E, 0x001E71, 0x001E70,
  0x001E73, 0x001E72, 0x001E75, 0x001E74, 0x001E77, 0x001E76, 0x001E79, 0x001E78,
  0x001E7B, 0x001E7A, 0x001E7D, 0x001E7C, 0x001E7F, 0x001E7E, 0x001E81, 0x001E80,
  0x001E83, 0x001E82, 0x001E85, 0x001E84, 0x001E87, 0x001E86, 0x001E89, 0x001E88,
  0x001E8B, 0x001E8A, 0x001E8D, 0x001E8C, 0x001E8F, 0x001E8E, 0x001E91, 0x001E90,
  0x001E93, 0x001E92, 0x001E95, 0x001E94, 0x001E9B, 0x001E60, 0x001EA1, 0x001EA0,
  0x001EA3, 0x001EA2, 0x001EA5, 0x001EA4, 0x001EA7, 0x001EA6, 0x001EA9, 0x001EA8,
  0x001EAB, 0x001EAA, 0x001EAD, 0x001EAC, 0x001EAF, 0x001EAE, 0x001EB1, 0x001EB0,
  0x001EB3, 0x001EB2, 0x001EB5, 0x001EB4, 0x001EB7, 0x001EB6, 0x001EB9, 0x001EB8,
  0x001EBB, 0x001EBA, 0x001EBD, 0x001EBC, 0x001EBF, 0x001EBE, 0x001EC1, 0x001EC0,
  0x001EC3, 0x001EC2, 0x001EC5, 0x001EC4, 0x001EC7, 0x001EC6, 0x001EC9, 0x001EC8,
  0x001ECB, 0x001ECA, 0x001ECD, 0x001ECC, 0x001ECF, 0x001ECE, 0x001ED1, 0x001ED0,
  0x001ED3, 0x001ED2, 0x001ED5, 0x001ED4, 0x001ED7, 0x001ED6, 0x001ED9, 0x001ED8,
  0x001EDB, 0x001EDA, 0x001EDD, 0x001EDC, 0x001EDF, 0x001EDE, 0x001EE1, 0x001EE0,
  0x001EE3, 0x001EE2, 0x001EE5, 0x001EE4, 0x001EE7, 0x001EE6, 0x001EE9, 0x001EE8,
  0x001EEB, 0x001EEA, 0x001EED, 0x001EEC, 0x001EEF, 0x001EEE, 0x001EF1, 0x001EF0,
  0x001EF3, 0x001EF2, 0x001EF5, 0x001EF4, 0x001EF7, 0x001EF6, 0x001EF9, 0x001EF8,
  0x001EFB, 0x001EFA, 0x001EFD, 0x001EFC, 0x001EFF, 0x001EFE, 0x001F00, 0x001F08,
  0x001F01, 0x001F09, 0x001F02, 0x001F0A, 0x001F03, 0x001F0B, 0x001F04, 0x001F0C,
  0x001F05, 0x001F0D, 0x001F06, 0x001F0E, 0x001F07, 0x001F0F, 0x001F10, 0x001F18,
  0x001F11, 0x001F19, 0x001F12, 0x001F1A, 0x001F13, 0x001F1B, 0x001F14, 0x001F1C,
  0x001F15, 0x001F1D, 0x001F20, 0x001F28, 0x001F21, 0x001F29, 0x001F22, 0x001F2A,
  0x001F23, 0x001F2B, 0x001F24, 0x001F2C, 0x001F25, 0x001F2D, 0x001F26, 0x001F2E,
  0x001F27, 0x001F2F, 0x001F30, 0x001F38, 0x001F31, 0x001F39, 0x001F32, 0x001F3A,
  0x001F33, 0x001F3B, 0x001F34, 0x001F3C, 0x001F35, 0x001F3D, 0x001F36, 0x001F3E,
  0x001F37, 0x001F3F, 0x001F40, 0x001F48, 0x001F41, 0x001F49, 0x001F42, 0x001F4A,
  0x001F43, 0x001F4B, 0x001F44, 0x001F4C, 0x001F45, 0x001F4D, 0x001F51, 0x001F59,
  0x001F53, 0x001F5B, 0x001F55, 0x001F5D, 0x001F57, 0x001F5F, 0x001F60, 0x001F68,
  0x001F61, 0x001F69, 0x001F62, 0x001F6A, 0x001F63, 0x001F6B, 0x001F64, 0x001F6C,
  0x001F65, 0x001F6D, 0x001F66, 0x001F6E, 0x001F67, 0x001F6F, 0x001F70, 0x001FBA,
  0x001F71, 0x001FBB, 0x001F72, 0x001FC8, 0x001F73, 0x001FC9, 0x001F74, 0x001FCA,
  0x001F75, 0x001FCB, 0x001F76, 0x001FDA, 0x001F77, 0x001FDB, 0x001F78, 0x001FF8,
  0x001F79, 0x001FF9, 0x001F7A, 0x001FEA, 0x001F7B, 0x001FEB, 0x001F7C, 0x001FFA,
  0x001F7D, 0x001FFB, 0x001F80, 0x001F88, 0x001F81, 0x001F89, 0x001F82, 0x001F8A,
  0x001F83, 0x001F8B, 0x001F84, 0x001F8C, 0x001F85, 0x001F8D, 0x001F86, 0x001F8E,
  0x001F87, 0x001F8F, 0x001F90, 0x001F98, 0x001F91, 0x001F99, 0x001F92, 0x001F9A,
  0x001F93, 0x001F9B, 0x001F94, 0x001F9C, 0x001F95, 0x001F9D, 0x001F96, 0x001F9E,
  0x001F97, 0x001F9F, 0x001FA0, 0x001FA8, 0x001FA1, 0x001FA9, 0x001FA2, 0x001FAA,
  0x001FA3, 0x001FAB, 0x001FA4, 0x001FAC, 0x001FA5, 0x001FAD, 0x001FA6, 0x001FAE,
  0x001FA7, 0x001FAF, 0x001FB0, 0x001FB8, 0x001FB1, 0x001FB9, 0x001FB3, 0x001FBC,
  0x001FBE, 0x000399, 0x001FC3, 0x001FCC, 0x001FD0, 0x001FD8, 0x001FD1, 0x001FD9,
  0x001FE0, 0x001FE8, 0x001FE1, 0x001FE9, 0x001FE5, 0x001FEC, 0x001FF3, 0x001FFC,
  0x00214E, 0x002132, 0x002184, 0x002183, 0x002C30, 0x002C00, 0x002C31, 0x002C01,
  0x002C32, 0x002C02, 0x002C33, 0x002C03, 0x002C34, 0x002C04, 0x002C35, 0x002C05,
  0x002C36, 0x002C06, 0x002C37, 0x002C07, 0x002C38, 0x002C08, 0x002C39, 0x002C09,
  0x002C3A, 0x002C0A, 0x002C3B, 0x002C0B, 0x002C3C, 0x002C0C, 0x002C3D, 0x002C0D,
  0x002C3E, 0x002C0E, 0x002C3F, 0x002C0F, 0x002C40, 0x002C10, 0x002C41, 0x002C11,
  0x002C42, 0x002C12, 0x002C43, 0x002C13, 0x002C44, 0x002C14, 0x002C45, 0x002C15,
  0x002C46, 0x002C16, 0x002C47, 0x002C17, 0x002C48, 0x002C18, 0x002C49, 0x002C19,
  0x002C4A, 0x002C1A, 0x002C4B, 0x002C1B, 0x002C4C, 0x002C1C, 0x002C4D, 0x002C1D,
  0x002C4E, 0x002C1E, 0x002C4F, 0x002C1F, 0x002C50, 0x002C20, 0x002C51, 0x002C21,
  0x002C52, 0x002C22, 0x002C53, 0x002C23, 0x002C54, 0x002C24, 0x002C55, 0x002C25,
  0x002C56, 0x002C26, 0x002C57, 0x002C27, 0x002C58, 0x002C28, 0x002C59, 0x002C29,
  0x002C5A, 0x002C2A, 0x002C5B, 0x002C2B, 0x002C5C, 0x002C2C, 0x002C5D, 0x002C2D,
  0x002C5E, 0x002C2E, 0x002C61, 0x002C60, 0x002C65, 0x00023A, 0x002C66, 0x00023E,
  0x002C68, 0x002C67, 0x002C6A, 0x002C69, 0x002C6C, 0x002C6B, 0x002C73, 0x002C72,
  0x002C76, 0x002C75, 0x002C81, 0x002C80, 0x002C83, 0x002C82, 0x002C85, 0x002C84,
  0x002C87, 0x002C86, 0x002C89, 0x002C88, 0x002C8B, 0x002C8A, 0x002C8D, 0x002C8C,
  0x002C8F, 0x002C8E, 0x002C91, 0x002C90, 0x002C93, 0x002C92, 0x002C95, 0x002C94,
  0x002C97, 0x002C96, 0x002C99, 0x002C98, 0x002C9B, 0x002C9A, 0x002C9D, 0x002C9C,
  0x002C9F, 0x002C9E, 0x002CA1, 0x002CA0, 0x002CA3, 0x002CA2, 0x002CA5, 0x002CA4,
  0x002CA7, 0x002CA6, 0x002CA9, 0x002CA8, 0x002CAB, 0x002CAA, 0x002CAD, 0x002CAC,
  0x002CAF, 0x002CAE, 0x002CB1, 0x002CB0, 0x002CB3, 0x002CB2, 0x002CB5, 0x002CB4,
  0x002CB7, 0x002CB6, 0x002CB9, 0x002CB8, 0x002CBB, 0x002CBA, 0x002CBD, 0x002CBC,
  0x002CBF, 0x002CBE, 0x002CC1, 0x002CC0, 0x002CC3, 0x002CC2, 0x002CC5, 0x002CC4,
  0x002CC7, 0x002CC6, 0x002CC9, 0x002CC8, 0x002CCB, 0x002CCA, 0x002CCD, 0x002CCC,
  0x002CCF, 0x002CCE, 0x002CD1, 0x002CD0, 0x002CD3, 0x002CD2, 0x002CD5, 0x002CD4,
  0x002CD7, 0x002CD6, 0x002CD9, 0x002CD8, 0x002CDB, 0x002CDA, 0x002CDD, 0x002CDC,
  0x002CDF, 0x002CDE, 0x002CE1, 0x002CE0, 0x002CE3, 0x002CE2, 0x002CEC, 0x002CEB,
  0x002CEE, 0x002CED, 0x002CF3, 0x002CF2, 0x002D00, 0x0010A0, 0x002D01, 0x0010A1,
  0x002D02, 0x0010A2, 0x002D03, 0x0010A3, 0x002D04, 0x0010A4, 0x002D05, 0x0010A5,
  0x002D06, 0x0010A6, 0x002D07, 0x0010A7, 0x002D08, 0x0010A8, 0x002D09, 0x0010A9,
  0x002D0A, 0x0010AA, 0x002D0B, 0x0010AB, 0x002D0C, 0x0010AC, 0x002D0D, 0x0010AD,
  0x002D0E, 0x0010AE, 0x002D0F, 0x0010AF, 0x002D10, 0x0010B0, 0x002D11, 0x0010B1,
  0x002D12, 0x0010B2, 0x002D13, 0x0010B3, 0x002D14, 0x0010B4, 0x002D15, 0x0010B5,
  0x002D16, 0x0010B6, 0x002D17, 0x0010B7, 0x002D18, 0x0010B8, 0x002D19, 0x0010B9,
  0x002D1A, 0x0010BA, 0x002D1B, 0x0010BB, 0x002D1C, 0x0010BC, 0x002D1D, 0x0010BD,
  0x002D1E, 0x0010BE, 0x002D1F, 0x0010BF, 0x002D20, 0x0010C0, 0x002D21, 0x0010C1,
  0x002D22, 0x0010C2, 0x002D23, 0x0010C3, 0x002D24, 0x0010C4, 0x002D25, 0x0010C5,
  0x002D27, 0x0010C7, 0x002D2D, 0x0010CD, 0x00A641, 0x00A640, 0x00A643, 0x00A642,
  0x00A645, 0x00A644, 0x00A647, 0x00A646, 0x00A649, 0x00A648, 0x00A64B, 0x00A64A,
  0x00A64D, 0x00A64C, 0x00A64F, 0x00A64E, 0x00A651, 0x00A650, 0x00A653, 0x00A652,
  0x00A655, 0x00A654, 0x00A657, 0x00A656, 0x00A659, 0x00A658, 0x00A65B, 0x00A65A,
  0x00A65D, 0x00A65C, 0x00A65F, 0x00A65E, 0x00A661, 0x00A660, 0x00A663, 0x00A662,
  0x00A665, 0x00A664, 0x00A667, 0x00A666, 0x00A669, 0x00A668, 0x00A66B, 0x00A66A,
  0x00A66D, 0x00A66C, 0x00A681, 0x00A680, 0x00A683, 0x00A682, 0x00A685, 0x00A684,
  0x00A687, 0x00A686, 0x00A689, 0x00A688, 0x00A68B, 0x00A68A, 0x00A68D, 0x00A68C,
  0x00A68F, 0x00A68E, 0x00A691, 0x00A690, 0x00A693, 0x00A692, 0x00A695, 0x00A694,
  0x00A697, 0x00A696, 0x00A723, 0x00A722, 0x00A725, 0x00A724, 0x00A727, 0x00A726,
  0x00A729, 0x00A728, 0x00A72B, 0x00A72A, 0x00A72D, 0x00A72C, 0x00A72F, 0x00A72E,
  0x00A733, 0x00A732, 0x00A735, 0x00A734, 0x00A737, 0x00A736, 0x00A739, 0x00A738,
  0x00A73B, 0x00A73A, 0x00A73D, 0x00A73C, 0x00A73F, 0x00A73E, 0x00A741, 0x00A740,
  0x00A743, 0x00A742, 0x00A745, 0x00A744, 0x00A747, 0x00A746, 0x00A749, 0x00A748,
  0x00A74B, 0x00A74A, 0x00A74D, 0x00A74C, 0x00A74F, 0x00A74E, 0x00A751, 0x00A750,
  0x00A753, 0x00A752, 0x00A755, 0x00A754, 0x00A757, 0x00A756, 0x00A759, 0x00A758,
  0x00A75B, 0x00A75A, 0x00A75D, 0x00A75C, 0x00A75F, 0x00A75E, 0x00A761, 0x00A760,
  0x00A763, 0x00A762, 0x00A765, 0x00A764, 0x00A767, 0x00A766, 0x00A769, 0x00A768,
  0x00A76B, 0x00A76A, 0x00A76D, 0x00A76C, 0x00A76F, 0x00A76E, 0x00A77A, 0x00A779,
  0x00A77C, 0x00A77B, 0x00A77F, 0x00A77E, 0x00A781, 0x00A780, 0x00A783, 0x00A782,
  0x00A785, 0x00A784, 0x00A787, 0x00A786, 0x00A78C, 0x00A78B, 0x00A791, 0x00A790,
  0x00A793, 0x00A792, 0x00A7A1, 0x00A7A0, 0x00A7A3, 0x00A7A2, 0x00A7A5, 0x00A7A4,
  0x00A7A7, 0x00A7A6, 0x00A7A9, 0x00A7A8, 0x00FF41, 0x00FF21, 0x00FF42, 0x00FF22,
  0x00FF43, 0x00FF23, 0x00FF44, 0x00FF24, 0x00FF45, 0x00FF25, 0x00FF46, 0x00FF26,
  0x00FF47, 0x00FF27, 0x00FF48, 0x00FF28, 0x00FF49, 0x00FF29, 0x00FF4A, 0x00FF2A,
  0x00FF4B, 0x00FF2B, 0x00FF4C, 0x00FF2C, 0x00FF4D, 0x00FF2D, 0x00FF4E, 0x00FF2E,
  0x00FF4F, 0x00FF2F, 0x00FF50, 0x00FF30, 0x00FF51, 0x00FF31, 0x00FF52, 0x00FF32,
  0x00FF53, 0x00FF33, 0x00FF54, 0x00FF34, 0x00FF55, 0x00FF35, 0x00FF56, 0x00FF36,
  0x00FF57, 0x00FF37, 0x00FF58, 0x00FF38, 0x00FF59, 0x00FF39, 0x00FF5A, 0x00FF3A,
  0x010428, 0x010400, 0x010429, 0x010401, 0x01042A, 0x010402, 0x01042B, 0x010403,
  0x01042C, 0x010404, 0x01042D, 0x010405, 0x01042E, 0x010406, 0x01042F, 0x010407,
  0x010430, 0x010408, 0x010431, 0x010409, 0x010432, 0x01040A, 0x010433, 0x01040B,
  0x010434, 0x01040C, 0x010435, 0x01040D, 0x010436, 0x01040E, 0x010437, 0x01040F,
  0x010438, 0x010410, 0x010439, 0x010411, 0x01043A, 0x010412, 0x01043B, 0x010413,
  0x01043C, 0x010414, 0x01043D, 0x010415, 0x01043E, 0x010416, 0x01043F, 0x010417,
  0x010440, 0x010418, 0x010441, 0x010419, 0x010442, 0x01041A, 0x010443, 0x01041B,
  0x010444, 0x01041C, 0x010445, 0x01041D, 0x010446, 0x01041E, 0x010447, 0x01041F,
  0x010448, 0x010420, 0x010449, 0x010421, 0x01044A, 0x010422, 0x01044B, 0x010423,
  0x01044C, 0x010424, 0x01044D, 0x010425, 0x01044E, 0x010426, 0x01044F, 0x010427,
  0, 0    /// terminator
};



/// character case change; uppercase to lowercase
_L2U _cu2l[]= {
  0x000061, 0x000041, 0x000062, 0x000042, 0x000063, 0x000043, 0x000064, 0x000044,
  0x000065, 0x000045, 0x000066, 0x000046, 0x000067, 0x000047, 0x000068, 0x000048,
  0x000069, 0x000049, 0x00006A, 0x00004A, 0x00006B, 0x00004B, 0x00006C, 0x00004C,
  0x00006D, 0x00004D, 0x00006E, 0x00004E, 0x00006F, 0x00004F, 0x000070, 0x000050,
  0x000071, 0x000051, 0x000072, 0x000052, 0x000073, 0x000053, 0x000074, 0x000054,
  0x000075, 0x000055, 0x000076, 0x000056, 0x000077, 0x000057, 0x000078, 0x000058,
  0x000079, 0x000059, 0x00007A, 0x00005A, 0x0000E0, 0x0000C0, 0x0000E1, 0x0000C1,
  0x0000E2, 0x0000C2, 0x0000E3, 0x0000C3, 0x0000E4, 0x0000C4, 0x0000E5, 0x0000C5,
  0x0000E6, 0x0000C6, 0x0000E7, 0x0000C7, 0x0000E8, 0x0000C8, 0x0000E9, 0x0000C9,
  0x0000EA, 0x0000CA, 0x0000EB, 0x0000CB, 0x0000EC, 0x0000CC, 0x0000ED, 0x0000CD,
  0x0000EE, 0x0000CE, 0x0000EF, 0x0000CF, 0x0000F0, 0x0000D0, 0x0000F1, 0x0000D1,
  0x0000F2, 0x0000D2, 0x0000F3, 0x0000D3, 0x0000F4, 0x0000D4, 0x0000F5, 0x0000D5,
  0x0000F6, 0x0000D6, 0x0000F8, 0x0000D8, 0x0000F9, 0x0000D9, 0x0000FA, 0x0000DA,
  0x0000FB, 0x0000DB, 0x0000FC, 0x0000DC, 0x0000FD, 0x0000DD, 0x0000FE, 0x0000DE,
  0x000101, 0x000100, 0x000103, 0x000102, 0x000105, 0x000104, 0x000107, 0x000106,
  0x000109, 0x000108, 0x00010B, 0x00010A, 0x00010D, 0x00010C, 0x00010F, 0x00010E,
  0x000111, 0x000110, 0x000113, 0x000112, 0x000115, 0x000114, 0x000117, 0x000116,
  0x000119, 0x000118, 0x00011B, 0x00011A, 0x00011D, 0x00011C, 0x00011F, 0x00011E,
  0x000121, 0x000120, 0x000123, 0x000122, 0x000125, 0x000124, 0x000127, 0x000126,
  0x000129, 0x000128, 0x00012B, 0x00012A, 0x00012D, 0x00012C, 0x00012F, 0x00012E,
  0x000069, 0x000130, 0x000133, 0x000132, 0x000135, 0x000134, 0x000137, 0x000136,
  0x00013A, 0x000139, 0x00013C, 0x00013B, 0x00013E, 0x00013D, 0x000140, 0x00013F,
  0x000142, 0x000141, 0x000144, 0x000143, 0x000146, 0x000145, 0x000148, 0x000147,
  0x00014B, 0x00014A, 0x00014D, 0x00014C, 0x00014F, 0x00014E, 0x000151, 0x000150,
  0x000153, 0x000152, 0x000155, 0x000154, 0x000157, 0x000156, 0x000159, 0x000158,
  0x00015B, 0x00015A, 0x00015D, 0x00015C, 0x00015F, 0x00015E, 0x000161, 0x000160,
  0x000163, 0x000162, 0x000165, 0x000164, 0x000167, 0x000166, 0x000169, 0x000168,
  0x00016B, 0x00016A, 0x00016D, 0x00016C, 0x00016F, 0x00016E, 0x000171, 0x000170,
  0x000173, 0x000172, 0x000175, 0x000174, 0x000177, 0x000176, 0x0000FF, 0x000178,
  0x00017A, 0x000179, 0x00017C, 0x00017B, 0x00017E, 0x00017D, 0x000253, 0x000181,
  0x000183, 0x000182, 0x000185, 0x000184, 0x000254, 0x000186, 0x000188, 0x000187,
  0x000256, 0x000189, 0x000257, 0x00018A, 0x00018C, 0x00018B, 0x0001DD, 0x00018E,
  0x000259, 0x00018F, 0x00025B, 0x000190, 0x000192, 0x000191, 0x000260, 0x000193,
  0x000263, 0x000194, 0x000269, 0x000196, 0x000268, 0x000197, 0x000199, 0x000198,
  0x00026F, 0x00019C, 0x000272, 0x00019D, 0x000275, 0x00019F, 0x0001A1, 0x0001A0,
  0x0001A3, 0x0001A2, 0x0001A5, 0x0001A4, 0x000280, 0x0001A6, 0x0001A8, 0x0001A7,
  0x000283, 0x0001A9, 0x0001AD, 0x0001AC, 0x000288, 0x0001AE, 0x0001B0, 0x0001AF,
  0x00028A, 0x0001B1, 0x00028B, 0x0001B2, 0x0001B4, 0x0001B3, 0x0001B6, 0x0001B5,
  0x000292, 0x0001B7, 0x0001B9, 0x0001B8, 0x0001BD, 0x0001BC, 0x0001C6, 0x0001C4,
  0x0001C9, 0x0001C7, 0x0001CC, 0x0001CA, 0x0001CE, 0x0001CD, 0x0001D0, 0x0001CF,
  0x0001D2, 0x0001D1, 0x0001D4, 0x0001D3, 0x0001D6, 0x0001D5, 0x0001D8, 0x0001D7,
  0x0001DA, 0x0001D9, 0x0001DC, 0x0001DB, 0x0001DF, 0x0001DE, 0x0001E1, 0x0001E0,
  0x0001E3, 0x0001E2, 0x0001E5, 0x0001E4, 0x0001E7, 0x0001E6, 0x0001E9, 0x0001E8,
  0x0001EB, 0x0001EA, 0x0001ED, 0x0001EC, 0x0001EF, 0x0001EE, 0x0001F3, 0x0001F1,
  0x0001F5, 0x0001F4, 0x000195, 0x0001F6, 0x0001BF, 0x0001F7, 0x0001F9, 0x0001F8,
  0x0001FB, 0x0001FA, 0x0001FD, 0x0001FC, 0x0001FF, 0x0001FE, 0x000201, 0x000200,
  0x000203, 0x000202, 0x000205, 0x000204, 0x000207, 0x000206, 0x000209, 0x000208,
  0x00020B, 0x00020A, 0x00020D, 0x00020C, 0x00020F, 0x00020E, 0x000211, 0x000210,
  0x000213, 0x000212, 0x000215, 0x000214, 0x000217, 0x000216, 0x000219, 0x000218,
  0x00021B, 0x00021A, 0x00021D, 0x00021C, 0x00021F, 0x00021E, 0x00019E, 0x000220,
  0x000223, 0x000222, 0x000225, 0x000224, 0x000227, 0x000226, 0x000229, 0x000228,
  0x00022B, 0x00022A, 0x00022D, 0x00022C, 0x00022F, 0x00022E, 0x000231, 0x000230,
  0x000233, 0x000232, 0x002C65, 0x00023A, 0x00023C, 0x00023B, 0x00019A, 0x00023D,
  0x002C66, 0x00023E, 0x000242, 0x000241, 0x000180, 0x000243, 0x000289, 0x000244,
  0x00028C, 0x000245, 0x000247, 0x000246, 0x000249, 0x000248, 0x00024B, 0x00024A,
  0x00024D, 0x00024C, 0x00024F, 0x00024E, 0x000371, 0x000370, 0x000373, 0x000372,
  0x000377, 0x000376, 0x0003AC, 0x000386, 0x0003AD, 0x000388, 0x0003AE, 0x000389,
  0x0003AF, 0x00038A, 0x0003CC, 0x00038C, 0x0003CD, 0x00038E, 0x0003CE, 0x00038F,
  0x0003B1, 0x000391, 0x0003B2, 0x000392, 0x0003B3, 0x000393, 0x0003B4, 0x000394,
  0x0003B5, 0x000395, 0x0003B6, 0x000396, 0x0003B7, 0x000397, 0x0003B8, 0x000398,
  0x0003B9, 0x000399, 0x0003BA, 0x00039A, 0x0003BB, 0x00039B, 0x0003BC, 0x00039C,
  0x0003BD, 0x00039D, 0x0003BE, 0x00039E, 0x0003BF, 0x00039F, 0x0003C0, 0x0003A0,
  0x0003C1, 0x0003A1, 0x0003C3, 0x0003A3, 0x0003C4, 0x0003A4, 0x0003C5, 0x0003A5,
  0x0003C6, 0x0003A6, 0x0003C7, 0x0003A7, 0x0003C8, 0x0003A8, 0x0003C9, 0x0003A9,
  0x0003CA, 0x0003AA, 0x0003CB, 0x0003AB, 0x0003D7, 0x0003CF, 0x0003D9, 0x0003D8,
  0x0003DB, 0x0003DA, 0x0003DD, 0x0003DC, 0x0003DF, 0x0003DE, 0x0003E1, 0x0003E0,
  0x0003E3, 0x0003E2, 0x0003E5, 0x0003E4, 0x0003E7, 0x0003E6, 0x0003E9, 0x0003E8,
  0x0003EB, 0x0003EA, 0x0003ED, 0x0003EC, 0x0003EF, 0x0003EE, 0x0003B8, 0x0003F4,
  0x0003F8, 0x0003F7, 0x0003F2, 0x0003F9, 0x0003FB, 0x0003FA, 0x00037B, 0x0003FD,
  0x00037C, 0x0003FE, 0x00037D, 0x0003FF, 0x000450, 0x000400, 0x000451, 0x000401,
  0x000452, 0x000402, 0x000453, 0x000403, 0x000454, 0x000404, 0x000455, 0x000405,
  0x000456, 0x000406, 0x000457, 0x000407, 0x000458, 0x000408, 0x000459, 0x000409,
  0x00045A, 0x00040A, 0x00045B, 0x00040B, 0x00045C, 0x00040C, 0x00045D, 0x00040D,
  0x00045E, 0x00040E, 0x00045F, 0x00040F, 0x000430, 0x000410, 0x000431, 0x000411,
  0x000432, 0x000412, 0x000433, 0x000413, 0x000434, 0x000414, 0x000435, 0x000415,
  0x000436, 0x000416, 0x000437, 0x000417, 0x000438, 0x000418, 0x000439, 0x000419,
  0x00043A, 0x00041A, 0x00043B, 0x00041B, 0x00043C, 0x00041C, 0x00043D, 0x00041D,
  0x00043E, 0x00041E, 0x00043F, 0x00041F, 0x000440, 0x000420, 0x000441, 0x000421,
  0x000442, 0x000422, 0x000443, 0x000423, 0x000444, 0x000424, 0x000445, 0x000425,
  0x000446, 0x000426, 0x000447, 0x000427, 0x000448, 0x000428, 0x000449, 0x000429,
  0x00044A, 0x00042A, 0x00044B, 0x00042B, 0x00044C, 0x00042C, 0x00044D, 0x00042D,
  0x00044E, 0x00042E, 0x00044F, 0x00042F, 0x000461, 0x000460, 0x000463, 0x000462,
  0x000465, 0x000464, 0x000467, 0x000466, 0x000469, 0x000468, 0x00046B, 0x00046A,
  0x00046D, 0x00046C, 0x00046F, 0x00046E, 0x000471, 0x000470, 0x000473, 0x000472,
  0x000475, 0x000474, 0x000477, 0x000476, 0x000479, 0x000478, 0x00047B, 0x00047A,
  0x00047D, 0x00047C, 0x00047F, 0x00047E, 0x000481, 0x000480, 0x00048B, 0x00048A,
  0x00048D, 0x00048C, 0x00048F, 0x00048E, 0x000491, 0x000490, 0x000493, 0x000492,
  0x000495, 0x000494, 0x000497, 0x000496, 0x000499, 0x000498, 0x00049B, 0x00049A,
  0x00049D, 0x00049C, 0x00049F, 0x00049E, 0x0004A1, 0x0004A0, 0x0004A3, 0x0004A2,
  0x0004A5, 0x0004A4, 0x0004A7, 0x0004A6, 0x0004A9, 0x0004A8, 0x0004AB, 0x0004AA,
  0x0004AD, 0x0004AC, 0x0004AF, 0x0004AE, 0x0004B1, 0x0004B0, 0x0004B3, 0x0004B2,
  0x0004B5, 0x0004B4, 0x0004B7, 0x0004B6, 0x0004B9, 0x0004B8, 0x0004BB, 0x0004BA,
  0x0004BD, 0x0004BC, 0x0004BF, 0x0004BE, 0x0004CF, 0x0004C0, 0x0004C2, 0x0004C1,
  0x0004C4, 0x0004C3, 0x0004C6, 0x0004C5, 0x0004C8, 0x0004C7, 0x0004CA, 0x0004C9,
  0x0004CC, 0x0004CB, 0x0004CE, 0x0004CD, 0x0004D1, 0x0004D0, 0x0004D3, 0x0004D2,
  0x0004D5, 0x0004D4, 0x0004D7, 0x0004D6, 0x0004D9, 0x0004D8, 0x0004DB, 0x0004DA,
  0x0004DD, 0x0004DC, 0x0004DF, 0x0004DE, 0x0004E1, 0x0004E0, 0x0004E3, 0x0004E2,
  0x0004E5, 0x0004E4, 0x0004E7, 0x0004E6, 0x0004E9, 0x0004E8, 0x0004EB, 0x0004EA,
  0x0004ED, 0x0004EC, 0x0004EF, 0x0004EE, 0x0004F1, 0x0004F0, 0x0004F3, 0x0004F2,
  0x0004F5, 0x0004F4, 0x0004F7, 0x0004F6, 0x0004F9, 0x0004F8, 0x0004FB, 0x0004FA,
  0x0004FD, 0x0004FC, 0x0004FF, 0x0004FE, 0x000501, 0x000500, 0x000503, 0x000502,
  0x000505, 0x000504, 0x000507, 0x000506, 0x000509, 0x000508, 0x00050B, 0x00050A,
  0x00050D, 0x00050C, 0x00050F, 0x00050E, 0x000511, 0x000510, 0x000513, 0x000512,
  0x000515, 0x000514, 0x000517, 0x000516, 0x000519, 0x000518, 0x00051B, 0x00051A,
  0x00051D, 0x00051C, 0x00051F, 0x00051E, 0x000521, 0x000520, 0x000523, 0x000522,
  0x000525, 0x000524, 0x000527, 0x000526, 0x000561, 0x000531, 0x000562, 0x000532,
  0x000563, 0x000533, 0x000564, 0x000534, 0x000565, 0x000535, 0x000566, 0x000536,
  0x000567, 0x000537, 0x000568, 0x000538, 0x000569, 0x000539, 0x00056A, 0x00053A,
  0x00056B, 0x00053B, 0x00056C, 0x00053C, 0x00056D, 0x00053D, 0x00056E, 0x00053E,
  0x00056F, 0x00053F, 0x000570, 0x000540, 0x000571, 0x000541, 0x000572, 0x000542,
  0x000573, 0x000543, 0x000574, 0x000544, 0x000575, 0x000545, 0x000576, 0x000546,
  0x000577, 0x000547, 0x000578, 0x000548, 0x000579, 0x000549, 0x00057A, 0x00054A,
  0x00057B, 0x00054B, 0x00057C, 0x00054C, 0x00057D, 0x00054D, 0x00057E, 0x00054E,
  0x00057F, 0x00054F, 0x000580, 0x000550, 0x000581, 0x000551, 0x000582, 0x000552,
  0x000583, 0x000553, 0x000584, 0x000554, 0x000585, 0x000555, 0x000586, 0x000556,
  0x002D00, 0x0010A0, 0x002D01, 0x0010A1, 0x002D02, 0x0010A2, 0x002D03, 0x0010A3,
  0x002D04, 0x0010A4, 0x002D05, 0x0010A5, 0x002D06, 0x0010A6, 0x002D07, 0x0010A7,
  0x002D08, 0x0010A8, 0x002D09, 0x0010A9, 0x002D0A, 0x0010AA, 0x002D0B, 0x0010AB,
  0x002D0C, 0x0010AC, 0x002D0D, 0x0010AD, 0x002D0E, 0x0010AE, 0x002D0F, 0x0010AF,
  0x002D10, 0x0010B0, 0x002D11, 0x0010B1, 0x002D12, 0x0010B2, 0x002D13, 0x0010B3,
  0x002D14, 0x0010B4, 0x002D15, 0x0010B5, 0x002D16, 0x0010B6, 0x002D17, 0x0010B7,
  0x002D18, 0x0010B8, 0x002D19, 0x0010B9, 0x002D1A, 0x0010BA, 0x002D1B, 0x0010BB,
  0x002D1C, 0x0010BC, 0x002D1D, 0x0010BD, 0x002D1E, 0x0010BE, 0x002D1F, 0x0010BF,
  0x002D20, 0x0010C0, 0x002D21, 0x0010C1, 0x002D22, 0x0010C2, 0x002D23, 0x0010C3,
  0x002D24, 0x0010C4, 0x002D25, 0x0010C5, 0x002D27, 0x0010C7, 0x002D2D, 0x0010CD,
  0x001E01, 0x001E00, 0x001E03, 0x001E02, 0x001E05, 0x001E04, 0x001E07, 0x001E06,
  0x001E09, 0x001E08, 0x001E0B, 0x001E0A, 0x001E0D, 0x001E0C, 0x001E0F, 0x001E0E,
  0x001E11, 0x001E10, 0x001E13, 0x001E12, 0x001E15, 0x001E14, 0x001E17, 0x001E16,
  0x001E19, 0x001E18, 0x001E1B, 0x001E1A, 0x001E1D, 0x001E1C, 0x001E1F, 0x001E1E,
  0x001E21, 0x001E20, 0x001E23, 0x001E22, 0x001E25, 0x001E24, 0x001E27, 0x001E26,
  0x001E29, 0x001E28, 0x001E2B, 0x001E2A, 0x001E2D, 0x001E2C, 0x001E2F, 0x001E2E,
  0x001E31, 0x001E30, 0x001E33, 0x001E32, 0x001E35, 0x001E34, 0x001E37, 0x001E36,
  0x001E39, 0x001E38, 0x001E3B, 0x001E3A, 0x001E3D, 0x001E3C, 0x001E3F, 0x001E3E,
  0x001E41, 0x001E40, 0x001E43, 0x001E42, 0x001E45, 0x001E44, 0x001E47, 0x001E46,
  0x001E49, 0x001E48, 0x001E4B, 0x001E4A, 0x001E4D, 0x001E4C, 0x001E4F, 0x001E4E,
  0x001E51, 0x001E50, 0x001E53, 0x001E52, 0x001E55, 0x001E54, 0x001E57, 0x001E56,
  0x001E59, 0x001E58, 0x001E5B, 0x001E5A, 0x001E5D, 0x001E5C, 0x001E5F, 0x001E5E,
  0x001E61, 0x001E60, 0x001E63, 0x001E62, 0x001E65, 0x001E64, 0x001E67, 0x001E66,
  0x001E69, 0x001E68, 0x001E6B, 0x001E6A, 0x001E6D, 0x001E6C, 0x001E6F, 0x001E6E,
  0x001E71, 0x001E70, 0x001E73, 0x001E72, 0x001E75, 0x001E74, 0x001E77, 0x001E76,
  0x001E79, 0x001E78, 0x001E7B, 0x001E7A, 0x001E7D, 0x001E7C, 0x001E7F, 0x001E7E,
  0x001E81, 0x001E80, 0x001E83, 0x001E82, 0x001E85, 0x001E84, 0x001E87, 0x001E86,
  0x001E89, 0x001E88, 0x001E8B, 0x001E8A, 0x001E8D, 0x001E8C, 0x001E8F, 0x001E8E,
  0x001E91, 0x001E90, 0x001E93, 0x001E92, 0x001E95, 0x001E94, 0x0000DF, 0x001E9E,
  0x001EA1, 0x001EA0, 0x001EA3, 0x001EA2, 0x001EA5, 0x001EA4, 0x001EA7, 0x001EA6,
  0x001EA9, 0x001EA8, 0x001EAB, 0x001EAA, 0x001EAD, 0x001EAC, 0x001EAF, 0x001EAE,
  0x001EB1, 0x001EB0, 0x001EB3, 0x001EB2, 0x001EB5, 0x001EB4, 0x001EB7, 0x001EB6,
  0x001EB9, 0x001EB8, 0x001EBB, 0x001EBA, 0x001EBD, 0x001EBC, 0x001EBF, 0x001EBE,
  0x001EC1, 0x001EC0, 0x001EC3, 0x001EC2, 0x001EC5, 0x001EC4, 0x001EC7, 0x001EC6,
  0x001EC9, 0x001EC8, 0x001ECB, 0x001ECA, 0x001ECD, 0x001ECC, 0x001ECF, 0x001ECE,
  0x001ED1, 0x001ED0, 0x001ED3, 0x001ED2, 0x001ED5, 0x001ED4, 0x001ED7, 0x001ED6,
  0x001ED9, 0x001ED8, 0x001EDB, 0x001EDA, 0x001EDD, 0x001EDC, 0x001EDF, 0x001EDE,
  0x001EE1, 0x001EE0, 0x001EE3, 0x001EE2, 0x001EE5, 0x001EE4, 0x001EE7, 0x001EE6,
  0x001EE9, 0x001EE8, 0x001EEB, 0x001EEA, 0x001EED, 0x001EEC, 0x001EEF, 0x001EEE,
  0x001EF1, 0x001EF0, 0x001EF3, 0x001EF2, 0x001EF5, 0x001EF4, 0x001EF7, 0x001EF6,
  0x001EF9, 0x001EF8, 0x001EFB, 0x001EFA, 0x001EFD, 0x001EFC, 0x001EFF, 0x001EFE,
  0x001F00, 0x001F08, 0x001F01, 0x001F09, 0x001F02, 0x001F0A, 0x001F03, 0x001F0B,
  0x001F04, 0x001F0C, 0x001F05, 0x001F0D, 0x001F06, 0x001F0E, 0x001F07, 0x001F0F,
  0x001F10, 0x001F18, 0x001F11, 0x001F19, 0x001F12, 0x001F1A, 0x001F13, 0x001F1B,
  0x001F14, 0x001F1C, 0x001F15, 0x001F1D, 0x001F20, 0x001F28, 0x001F21, 0x001F29,
  0x001F22, 0x001F2A, 0x001F23, 0x001F2B, 0x001F24, 0x001F2C, 0x001F25, 0x001F2D,
  0x001F26, 0x001F2E, 0x001F27, 0x001F2F, 0x001F30, 0x001F38, 0x001F31, 0x001F39,
  0x001F32, 0x001F3A, 0x001F33, 0x001F3B, 0x001F34, 0x001F3C, 0x001F35, 0x001F3D,
  0x001F36, 0x001F3E, 0x001F37, 0x001F3F, 0x001F40, 0x001F48, 0x001F41, 0x001F49,
  0x001F42, 0x001F4A, 0x001F43, 0x001F4B, 0x001F44, 0x001F4C, 0x001F45, 0x001F4D,
  0x001F51, 0x001F59, 0x001F53, 0x001F5B, 0x001F55, 0x001F5D, 0x001F57, 0x001F5F,
  0x001F60, 0x001F68, 0x001F61, 0x001F69, 0x001F62, 0x001F6A, 0x001F63, 0x001F6B,
  0x001F64, 0x001F6C, 0x001F65, 0x001F6D, 0x001F66, 0x001F6E, 0x001F67, 0x001F6F,
  0x001FB0, 0x001FB8, 0x001FB1, 0x001FB9, 0x001F70, 0x001FBA, 0x001F71, 0x001FBB,
  0x001F72, 0x001FC8, 0x001F73, 0x001FC9, 0x001F74, 0x001FCA, 0x001F75, 0x001FCB,
  0x001FD0, 0x001FD8, 0x001FD1, 0x001FD9, 0x001F76, 0x001FDA, 0x001F77, 0x001FDB,
  0x001FE0, 0x001FE8, 0x001FE1, 0x001FE9, 0x001F7A, 0x001FEA, 0x001F7B, 0x001FEB,
  0x001FE5, 0x001FEC, 0x001F78, 0x001FF8, 0x001F79, 0x001FF9, 0x001F7C, 0x001FFA,
  0x001F7D, 0x001FFB, 0x0003C9, 0x002126, 0x00006B, 0x00212A, 0x0000E5, 0x00212B,
  0x00214E, 0x002132, 0x002184, 0x002183, 0x002C30, 0x002C00, 0x002C31, 0x002C01,
  0x002C32, 0x002C02, 0x002C33, 0x002C03, 0x002C34, 0x002C04, 0x002C35, 0x002C05,
  0x002C36, 0x002C06, 0x002C37, 0x002C07, 0x002C38, 0x002C08, 0x002C39, 0x002C09,
  0x002C3A, 0x002C0A, 0x002C3B, 0x002C0B, 0x002C3C, 0x002C0C, 0x002C3D, 0x002C0D,
  0x002C3E, 0x002C0E, 0x002C3F, 0x002C0F, 0x002C40, 0x002C10, 0x002C41, 0x002C11,
  0x002C42, 0x002C12, 0x002C43, 0x002C13, 0x002C44, 0x002C14, 0x002C45, 0x002C15,
  0x002C46, 0x002C16, 0x002C47, 0x002C17, 0x002C48, 0x002C18, 0x002C49, 0x002C19,
  0x002C4A, 0x002C1A, 0x002C4B, 0x002C1B, 0x002C4C, 0x002C1C, 0x002C4D, 0x002C1D,
  0x002C4E, 0x002C1E, 0x002C4F, 0x002C1F, 0x002C50, 0x002C20, 0x002C51, 0x002C21,
  0x002C52, 0x002C22, 0x002C53, 0x002C23, 0x002C54, 0x002C24, 0x002C55, 0x002C25,
  0x002C56, 0x002C26, 0x002C57, 0x002C27, 0x002C58, 0x002C28, 0x002C59, 0x002C29,
  0x002C5A, 0x002C2A, 0x002C5B, 0x002C2B, 0x002C5C, 0x002C2C, 0x002C5D, 0x002C2D,
  0x002C5E, 0x002C2E, 0x002C61, 0x002C60, 0x00026B, 0x002C62, 0x001D7D, 0x002C63,
  0x00027D, 0x002C64, 0x002C68, 0x002C67, 0x002C6A, 0x002C69, 0x002C6C, 0x002C6B,
  0x000251, 0x002C6D, 0x000271, 0x002C6E, 0x000250, 0x002C6F, 0x000252, 0x002C70,
  0x002C73, 0x002C72, 0x002C76, 0x002C75, 0x00023F, 0x002C7E, 0x000240, 0x002C7F,
  0x002C81, 0x002C80, 0x002C83, 0x002C82, 0x002C85, 0x002C84, 0x002C87, 0x002C86,
  0x002C89, 0x002C88, 0x002C8B, 0x002C8A, 0x002C8D, 0x002C8C, 0x002C8F, 0x002C8E,
  0x002C91, 0x002C90, 0x002C93, 0x002C92, 0x002C95, 0x002C94, 0x002C97, 0x002C96,
  0x002C99, 0x002C98, 0x002C9B, 0x002C9A, 0x002C9D, 0x002C9C, 0x002C9F, 0x002C9E,
  0x002CA1, 0x002CA0, 0x002CA3, 0x002CA2, 0x002CA5, 0x002CA4, 0x002CA7, 0x002CA6,
  0x002CA9, 0x002CA8, 0x002CAB, 0x002CAA, 0x002CAD, 0x002CAC, 0x002CAF, 0x002CAE,
  0x002CB1, 0x002CB0, 0x002CB3, 0x002CB2, 0x002CB5, 0x002CB4, 0x002CB7, 0x002CB6,
  0x002CB9, 0x002CB8, 0x002CBB, 0x002CBA, 0x002CBD, 0x002CBC, 0x002CBF, 0x002CBE,
  0x002CC1, 0x002CC0, 0x002CC3, 0x002CC2, 0x002CC5, 0x002CC4, 0x002CC7, 0x002CC6,
  0x002CC9, 0x002CC8, 0x002CCB, 0x002CCA, 0x002CCD, 0x002CCC, 0x002CCF, 0x002CCE,
  0x002CD1, 0x002CD0, 0x002CD3, 0x002CD2, 0x002CD5, 0x002CD4, 0x002CD7, 0x002CD6,
  0x002CD9, 0x002CD8, 0x002CDB, 0x002CDA, 0x002CDD, 0x002CDC, 0x002CDF, 0x002CDE,
  0x002CE1, 0x002CE0, 0x002CE3, 0x002CE2, 0x002CEC, 0x002CEB, 0x002CEE, 0x002CED,
  0x002CF3, 0x002CF2, 0x00A641, 0x00A640, 0x00A643, 0x00A642, 0x00A645, 0x00A644,
  0x00A647, 0x00A646, 0x00A649, 0x00A648, 0x00A64B, 0x00A64A, 0x00A64D, 0x00A64C,
  0x00A64F, 0x00A64E, 0x00A651, 0x00A650, 0x00A653, 0x00A652, 0x00A655, 0x00A654,
  0x00A657, 0x00A656, 0x00A659, 0x00A658, 0x00A65B, 0x00A65A, 0x00A65D, 0x00A65C,
  0x00A65F, 0x00A65E, 0x00A661, 0x00A660, 0x00A663, 0x00A662, 0x00A665, 0x00A664,
  0x00A667, 0x00A666, 0x00A669, 0x00A668, 0x00A66B, 0x00A66A, 0x00A66D, 0x00A66C,
  0x00A681, 0x00A680, 0x00A683, 0x00A682, 0x00A685, 0x00A684, 0x00A687, 0x00A686,
  0x00A689, 0x00A688, 0x00A68B, 0x00A68A, 0x00A68D, 0x00A68C, 0x00A68F, 0x00A68E,
  0x00A691, 0x00A690, 0x00A693, 0x00A692, 0x00A695, 0x00A694, 0x00A697, 0x00A696,
  0x00A723, 0x00A722, 0x00A725, 0x00A724, 0x00A727, 0x00A726, 0x00A729, 0x00A728,
  0x00A72B, 0x00A72A, 0x00A72D, 0x00A72C, 0x00A72F, 0x00A72E, 0x00A733, 0x00A732,
  0x00A735, 0x00A734, 0x00A737, 0x00A736, 0x00A739, 0x00A738, 0x00A73B, 0x00A73A,
  0x00A73D, 0x00A73C, 0x00A73F, 0x00A73E, 0x00A741, 0x00A740, 0x00A743, 0x00A742,
  0x00A745, 0x00A744, 0x00A747, 0x00A746, 0x00A749, 0x00A748, 0x00A74B, 0x00A74A,
  0x00A74D, 0x00A74C, 0x00A74F, 0x00A74E, 0x00A751, 0x00A750, 0x00A753, 0x00A752,
  0x00A755, 0x00A754, 0x00A757, 0x00A756, 0x00A759, 0x00A758, 0x00A75B, 0x00A75A,
  0x00A75D, 0x00A75C, 0x00A75F, 0x00A75E, 0x00A761, 0x00A760, 0x00A763, 0x00A762,
  0x00A765, 0x00A764, 0x00A767, 0x00A766, 0x00A769, 0x00A768, 0x00A76B, 0x00A76A,
  0x00A76D, 0x00A76C, 0x00A76F, 0x00A76E, 0x00A77A, 0x00A779, 0x00A77C, 0x00A77B,
  0x001D79, 0x00A77D, 0x00A77F, 0x00A77E, 0x00A781, 0x00A780, 0x00A783, 0x00A782,
  0x00A785, 0x00A784, 0x00A787, 0x00A786, 0x00A78C, 0x00A78B, 0x000265, 0x00A78D,
  0x00A791, 0x00A790, 0x00A793, 0x00A792, 0x00A7A1, 0x00A7A0, 0x00A7A3, 0x00A7A2,
  0x00A7A5, 0x00A7A4, 0x00A7A7, 0x00A7A6, 0x00A7A9, 0x00A7A8, 0x000266, 0x00A7AA,
  0x00FF41, 0x00FF21, 0x00FF42, 0x00FF22, 0x00FF43, 0x00FF23, 0x00FF44, 0x00FF24,
  0x00FF45, 0x00FF25, 0x00FF46, 0x00FF26, 0x00FF47, 0x00FF27, 0x00FF48, 0x00FF28,
  0x00FF49, 0x00FF29, 0x00FF4A, 0x00FF2A, 0x00FF4B, 0x00FF2B, 0x00FF4C, 0x00FF2C,
  0x00FF4D, 0x00FF2D, 0x00FF4E, 0x00FF2E, 0x00FF4F, 0x00FF2F, 0x00FF50, 0x00FF30,
  0x00FF51, 0x00FF31, 0x00FF52, 0x00FF32, 0x00FF53, 0x00FF33, 0x00FF54, 0x00FF34,
  0x00FF55, 0x00FF35, 0x00FF56, 0x00FF36, 0x00FF57, 0x00FF37, 0x00FF58, 0x00FF38,
  0x00FF59, 0x00FF39, 0x00FF5A, 0x00FF3A, 0x010428, 0x010400, 0x010429, 0x010401,
  0x01042A, 0x010402, 0x01042B, 0x010403, 0x01042C, 0x010404, 0x01042D, 0x010405,
  0x01042E, 0x010406, 0x01042F, 0x010407, 0x010430, 0x010408, 0x010431, 0x010409,
  0x010432, 0x01040A, 0x010433, 0x01040B, 0x010434, 0x01040C, 0x010435, 0x01040D,
  0x010436, 0x01040E, 0x010437, 0x01040F, 0x010438, 0x010410, 0x010439, 0x010411,
  0x01043A, 0x010412, 0x01043B, 0x010413, 0x01043C, 0x010414, 0x01043D, 0x010415,
  0x01043E, 0x010416, 0x01043F, 0x010417, 0x010440, 0x010418, 0x010441, 0x010419,
  0x010442, 0x01041A, 0x010443, 0x01041B, 0x010444, 0x01041C, 0x010445, 0x01041D,
  0x010446, 0x01041E, 0x010447, 0x01041F, 0x010448, 0x010420, 0x010449, 0x010421,
  0x01044A, 0x010422, 0x01044B, 0x010423, 0x01044C, 0x010424, 0x01044D, 0x010425,
  0x01044E, 0x010426, 0x01044F, 0x010427,
  0, 0    /// terminator
};






// single character conversion to lowercase
char32_t Str::tolower(char32_t c) {
  /// check for Latin-1 characters (ASCII)
  if(c>= 0x41 && c<= 0x5A)
    return c+ 0x20;
  /// quick check if the char is already lowercase ASCII
  if(c>= 0x61 && c<= 0x7A)
    return c;

  int min= 0;
  int max= sizeof(_cu2l)/ sizeof(struct _L2U)- 1;
  int mid;

  /// binary search in table; mid search method - 10 passes max for a list of 1024
  while(max >= min) {
    mid = (min+ max)/ 2;
    if(_cu2l[mid].u < c)
	    min= mid+ 1;
    else if(_cu2l[mid].u > c)
	    max= mid- 1;
    else                               // found it
      return _cu2l[mid].l;
  }

  return c;                           /// not found, return c
}


// single character conversion to uppercase
char32_t Str::toupper(char32_t c) {
  /// check for Latin-1 characters (ASCII)
  if(c>= 0x61 && c<= 0x7A)
    return c- 0x20;
  /// quick check if the char is already uppercase ASCII
  if(c>= 0x41 && c<= 0x5A)
    return c;

  int min= 0;
  int max= sizeof(_cl2u)/ sizeof(struct _L2U)- 1;
  int mid;

  /// binary search in table; mid search method - 10 passes max for a list of 1024
  while(max >= min) {
    mid = (min+ max)/ 2;
    if(_cl2u[mid].l < c)
	    min= mid+ 1;
    else if(_cl2u[mid].l > c)
	    max= mid- 1;
    else                               // found it
      return _cl2u[mid].u;
  }
  return c;                           /// not found, return c
}





#include "util/mlib.hpp"
using namespace mlib;


///-------------------------///
// UTF8 to number =====----- //
///-------------------------///

// number base:
//   0x or 0X starting numbers are condidered hexazecimal
//   0 (zero starting) numbers are considered octal
//   0b or 0B starting numbers are considered binary

// <s>: string to covert to number;
// <zecimal>: forces to read from a zecimal number (you can read numbers like 000987 and not confuse them with octal)
int64_t Str::utf8toInt64(const char *s, bool zecimal) {
  uint8_t *p= (uint8_t *)s;
  int64_t ret= 0;             /// return value
  bool sign= false;           /// sign tmp var
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
    if(!*p) return 0;         /// end of string check
  }

  // check for a base number notation - [0x hexa] [0nr octa] [0b bynary]
  if(*p== '0' && !zecimal) {
    if(!*(++p)) return 0;     /// end of string check

    uint8_t a, max;
    if(*p== 'b' || *p== 'B')      a= 1, max= '1', p++; /// binary number
    else if(*p== 'x' || *p== 'X') a= 4, max= '9', p++; /// hexazecimal number
    else                          a= 3, max= '7';      /// octal number

    for(; *p; p++)
      if(*p>= '0' && *p<= max)
        ret<<= a, ret+= (*p- '0');
      else if(a== 4) {
        if(*p>= 'a' && *p<= 'f')
          ret<<= a, ret+= (0xA+ (*p- 'a'));
        else if(*p>= 'A' && *p<= 'F')
          ret<<= a, ret+= (0xA+ (*p- 'A'));
        else break;
      } else break;

  /// zecimal number if not starting with [0b 0x 0nr]
  } else 
    for(; *p; p++)
      if(*p>= '0' && *p<= '9')
        ret= (ret* 10)+ (*p- '0');
      else break;
  
  /// number sign
  if(sign)
    ret= -ret;

  return ret;
}

// <s>: string to convert
// <zecimal>: forces to read from a zecimal number (you can read numbers like 000987 and not confuse them with octal)
uint64_t Str::utf8toUint64(const char *s, bool zecimal) {
  uint8_t *p= (uint8_t *)s;
  uint64_t ret= 0;              /// return value

  /// search for number start
  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      break;

  // check for a base number notation - [0x hexa] [0nr octa] [0b bynary]
  if(*p== '0' && !zecimal) {
    if(!*(++p)) return 0;     /// end of string check

    uint8_t a, max;
    if(*p== 'b' || *p== 'B')      a= 1, max= '1', p++; /// binary number
    else if(*p== 'x' || *p== 'X') a= 4, max= '9', p++; /// hexazecimal number
    else                          a= 3, max= '7';      /// octal number

    for(; *p; p++)
      if(*p>= '0' && *p<= max)
        ret<<= a, ret+= (*p- '0');
      else if(a== 4) {
        if(*p>= 'a' && *p<= 'f')
          ret<<= a, ret+= (0xA+ (*p- 'a'));
        else if(*p>= 'A' && *p<= 'F')
          ret<<= a, ret+= (0xA+ (*p- 'A'));
        else break;
      } else break;
    
  /// zecimal number if not starting with [0b 0x 0nr]
  } else 
    for(; *p; p++)
      if(*p>= '0' && *p<= '9')
        ret= (ret* 10)+ (*p- '0');
      else
        break;
  
  return ret;
}


float Str::utf8toFloat(const char *s) {
  uint8_t *p= (uint8_t *)s;   /// p will walk s
  float ret= 0.0f;            /// return value
  int64_t i1= 0, i2= 0;       /// integer part and fractional part
  int n= 38;                  /// used with powtenf[] - 38 is 1e0
  bool sign= false;           /// tmp used for number sign
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-') || (*p== '.'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
  }

  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      i1= (i1* 10)+ (*p- '0');
    else
      break;

  ret= (float)i1;                  // integer part

  if(*p== '.') {
    for(p++; *p; p++)
      if(*p>= '0' && *p<= '9') {
        i2= (i2* 10)+ (*p- '0');
        n--;
      } else
        break;
    
    if(n< 0) n= 0;
    ret/= mlib::pow10f[n];
    ret+= (float)i2;
    ret*= mlib::pow10f[n];               // frac part
  }

  if(*p== 'e' || *p== 'E') {
    p++;
    bool sign2= false;
    if(*p== '-') {
      sign2= true;
      p++;
    }
    if(*p== '+') p++;

    int i= 0;
    for(; *p; p++)
      i= (i* 10)+ (*p- '0');
    if(i> 38) i= 38;

    if(sign2) i= -i;

    ret*= mlib::pow10f[38+ i];
  }

  if(sign) ret= -ret;

  return ret;
}


double Str::utf8toDouble(const char *s) {
  uint8_t *p= (uint8_t *)s;   /// p will walk s
  double ret= 0.0;            /// return value
  int64_t i1= 0, i2= 0;       /// integer part and fractional part
  int n= 308;                 /// used with powten[] ->  308 is 1e0
  bool sign= false;           /// tmp used for number sign
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-') || (*p== '.'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
  }

  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      i1= (i1* 10)+ (*p- '0');
    else
      break;

  ret= (double)i1;               // integer part

  if(*p== '.') {
    for(p++; *p; p++)
      if(*p>= '0' && *p<= '9') {
        i2= (i2* 10)+ (*p- '0');
        n--;
      } else
        break;

    if(n< 0) n= 0;

    ret/= mlib::pow10d[n];
    ret+= (double)i2;
    ret*= mlib::pow10d[n];             // frac part
  }

  if(*p== 'e' || *p== 'E') {
    p++;
    bool sign2= false;
    if(*p== '-') {
      sign2= true;
      p++;
    }
    if(*p== '+') p++;

    int i= 0;
    for(; *p; p++)
      i= (i* 10)+ (*p- '0');
    if(i> 308) i= 308;

    if(sign2) i= -i;

    ret*= mlib::pow10d[308+ i];
  }

  if(sign) ret= -ret;

  return ret;
}


///--------------------------///
// UTF16 to number =====----- //
///--------------------------///

// <zecimal>: forces to read from a zecimal number (you can read numbers like 000987 and not confuse them with octal)
int64_t Str::utf16toInt64(const char16_t *s, bool zecimal) {
  uint16_t *p= (uint16_t *)s;
  int64_t ret= 0;             /// return value
  bool sign= false;           /// sign tmp var
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
    if(!*p) return 0;         /// end of string check
  }

  // check for a base number notation - [0x hexa] [0nr octa] [0b bynary]
  if(*p== '0' && !zecimal) {
    if(!*(++p)) return 0;     /// end of string check

    uint32_t a, max;
    if(*p== 'b' || *p== 'B')      a= 1, max= '1', p++; /// binary number
    else if(*p== 'x' || *p== 'X') a= 4, max= '9', p++; /// hexazecimal number
    else                          a= 3, max= '7';      /// octal number

    for(; *p; p++)
      if(*p>= '0' && *p<= max)
        ret<<= a, ret+= (*p- '0');
      else if(a== 4) {
        if(*p>= 'a' && *p<= 'f')
          ret<<= a, ret+= (0xA+ (*p- 'a'));
        else if(*p>= 'A' && *p<= 'F')
          ret<<= a, ret+= (0xA+ (*p- 'A'));
        else break;
      } else break;

  /// zecimal number if not starting with [0b 0x 0nr]
  } else 
    for(; *p; p++)
      if(*p>= '0' && *p<= '9')
        ret= (ret* 10)+ (*p- '0');
      else break;
  
  /// number sign
  if(sign)
    ret= -ret;

  return ret;
}

// <zecimal>: forces to read from a zecimal number (you can read numbers like 000987 and not confuse them with octal)
uint64_t Str::utf16toUint64(const char16_t *s, bool zecimal) {
  uint16_t *p= (uint16_t *)s;
  uint64_t ret= 0;              /// return value

  /// search for number start
  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      break;

  // check for a base number notation - [0x hexa] [0nr octa] [0b bynary]
  if(*p== '0' && !zecimal) {
    if(!*(++p)) return 0;     /// end of string check

    uint32_t a, max;
    if(*p== 'b' || *p== 'B')      a= 1, max= '1', p++; /// binary number
    else if(*p== 'x' || *p== 'X') a= 4, max= '9', p++; /// hexazecimal number
    else                          a= 3, max= '7';      /// octal number

    for(; *p; p++)
      if(*p>= '0' && *p<= max)
        ret<<= a, ret+= (*p- '0');
      else if(a== 4) {
        if(*p>= 'a' && *p<= 'f')
          ret<<= a, ret+= (0xA+ (*p- 'a'));
        else if(*p>= 'A' && *p<= 'F')
          ret<<= a, ret+= (0xA+ (*p- 'A'));
        else break;
      } else break;
    
  /// zecimal number if not starting with [0b 0x 0nr]
  } else 
    for(; *p; p++)
      if(*p>= '0' && *p<= '9')
        ret= (ret* 10)+ (*p- '0');
      else
        break;
  
  return ret;
}


float Str::utf16toFloat(const char16_t *s) {
  uint16_t *p= (uint16_t *)s; /// p will walk s
  float ret= 0.0f;            /// return value
  int64_t i1= 0, i2= 0;       /// integer part and fractional part
  int n= 38;                  /// used with pow10f[] - 38 is 1e0
  bool sign= false;           /// tmp used for number sign
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-') || (*p== '.'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
  }

  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      i1= (i1* 10)+ (*p- '0');
    else
      break;

  ret= (float)i1;                  // integer part

  if(*p== '.') {
    for(p++; *p; p++)
      if(*p>= '0' && *p<= '9') {
        i2= (i2* 10)+ (*p- '0');
        n--;
      } else
        break;

    if(n< 0) n= 0;
    ret/= mlib::pow10f[n];
    ret+= (float)i2;
    ret*= mlib::pow10f[n];               // frac part
  }

  if(*p== 'e' || *p== 'E') {
    p++;
    bool sign2= false;
    if(*p== '-') {
      sign2= true;
      p++;
    }
    if(*p== '+') p++;

    int i= 0;
    for(; *p; p++)
      i= (i* 10)+ (*p- '0');
    if(i> 38) i= 38;

    if(sign2) i= -i;

    ret*= mlib::pow10f[38+ i];
  }

  if(sign) ret= -ret;

  return ret;
}


double Str::utf16toDouble(const char16_t *s) {
  uint16_t *p= (uint16_t *)s; /// p will walk s
  double ret= 0.0;            /// return value
  int64_t i1= 0, i2= 0;       /// integer part and fractional part
  int n= 308;                 /// used with pow10[] ->  308 is 1e0
  bool sign= false;           /// tmp used for number sign
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-') || (*p== '.'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
  }

  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      i1= (i1* 10)+ (*p- '0');
    else
      break;

  ret= (double)i1;               // integer part

  if(*p== '.') {
    for(p++; *p; p++)
      if(*p>= '0' && *p<= '9') {
        i2= (i2* 10)+ (*p- '0');
        n--;
      } else
        break;

    if(n< 0) n= 0;

    ret/= mlib::pow10d[n];
    ret+= (double)i2;
    ret*= mlib::pow10d[n];             // frac part
  }

  if(*p== 'e' || *p== 'E') {
    p++;
    bool sign2= false;
    if(*p== '-') {
      sign2= true;
      p++;
    }
    if(*p== '+') p++;

    int i= 0;
    for(; *p; p++)
      i= (i* 10)+ (*p- '0');
    if(i> 308) i= 308;

    if(sign2) i= -i;

    ret*= mlib::pow10d[308+ i];
  }

  if(sign) ret= -ret;

  return ret;
}


///--------------------------///
// UTF32 to number =====----- //
///--------------------------///

// <zecimal>: forces to read from a zecimal number (you can read numbers like 000987 and not confuse them with octal)
int64_t Str::utf32toInt64(const char32_t *s, bool zecimal) {
  uint32_t *p= (uint32_t *)s;
  int64_t ret= 0;             /// return value
  bool sign= false;           /// sign tmp var
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
    if(!*p) return 0;         /// end of string check
  }

  // check for a base number notation - [0x hexa] [0nr octa] [0b bynary]
  if(*p== '0' && !zecimal) {
    if(!*(++p)) return 0;     /// end of string check

    uint32_t a, max;
    if(*p== 'b' || *p== 'B')      a= 1, max= '1', p++; /// binary number
    else if(*p== 'x' || *p== 'X') a= 4, max= '9', p++; /// hexazecimal number
    else                          a= 3, max= '7';      /// octal number

    for(; *p; p++)
      if(*p>= '0' && *p<= max)
        ret<<= a, ret+= (*p- '0');
      else if(a== 4) {
        if(*p>= 'a' && *p<= 'f')
          ret<<= a, ret+= (0xA+ (*p- 'a'));
        else if(*p>= 'A' && *p<= 'F')
          ret<<= a, ret+= (0xA+ (*p- 'A'));
        else break;
      } else break;

  /// zecimal number if not starting with [0b 0x 0nr]
  } else 
    for(; *p; p++)
      if(*p>= '0' && *p<= '9')
        ret= (ret* 10)+ (*p- '0');
      else break;
  
  /// number sign
  if(sign)
    ret= -ret;

  return ret;
}

// <zecimal>: forces to read from a zecimal number (you can read numbers like 000987 and not confuse them with octal)
uint64_t Str::utf32toUint64(const char32_t *s, bool zecimal) {
  uint32_t *p= (uint32_t *)s;
  uint64_t ret= 0;              /// return value

  /// search for number start
  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      break;

  // check for a base number notation - [0x hexa] [0nr octa] [0b bynary]
  if(*p== '0' && !zecimal) {
    if(!*(++p)) return 0;     /// end of string check

    uint32_t a, max;
    if(*p== 'b' || *p== 'B')      a= 1, max= '1', p++; /// binary number
    else if(*p== 'x' || *p== 'X') a= 4, max= '9', p++; /// hexazecimal number
    else                          a= 3, max= '7';      /// octal number

    for(; *p; p++)
      if(*p>= '0' && *p<= max)
        ret<<= a, ret+= (*p- '0');
      else if(a== 4) {
        if(*p>= 'a' && *p<= 'f')
          ret<<= a, ret+= (0xA+ (*p- 'a'));
        else if(*p>= 'A' && *p<= 'F')
          ret<<= a, ret+= (0xA+ (*p- 'A'));
        else break;
      } else break;
    
  /// zecimal number if not starting with [0b 0x 0nr]
  } else 
    for(; *p; p++)
      if(*p>= '0' && *p<= '9')
        ret= (ret* 10)+ (*p- '0');
      else
        break;
  
  return ret;
}


float Str::utf32toFloat(const char32_t *s) {
  uint32_t *p= (uint32_t *)s; /// p will walk s
  float ret= 0.0f;            /// return value
  int64_t i1= 0, i2= 0;       /// integer part and fractional part
  int n= 38;                  /// used with pow10f[] - 38 is 1e0
  bool sign= false;           /// tmp used for number sign
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-') || (*p== '.'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
  }

  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      i1= (i1* 10)+ (*p- '0');
    else
      break;

  ret= (float)i1;                  // integer part

  if(*p== '.') {
    for(p++; *p; p++)
      if(*p>= '0' && *p<= '9') {
        i2= (i2* 10)+ (*p- '0');
        n--;
      } else
        break;

    if(n< 0) n= 0;
    ret/= mlib::pow10f[n];
    ret+= (float)i2;
    ret*= mlib::pow10f[n];               // frac part
  }

  if(*p== 'e' || *p== 'E') {
    p++;
    bool sign2= false;
    if(*p== '-') {
      sign2= true;
      p++;
    }
    if(*p== '+') p++;

    int i= 0;
    for(; *p; p++)
      i= (i* 10)+ (*p- '0');
    if(i> 38) i= 38;

    if(sign2) i= -i;

    ret*= mlib::pow10f[38+ i];
  }

  if(sign) ret= -ret;

  return ret;
}


double Str::utf32toDouble(const char32_t *s) {
  uint32_t *p= (uint32_t *)s; /// p will walk s
  double ret= 0.0;            /// return value
  int64_t i1= 0, i2= 0;       /// integer part and fractional part
  int n= 308;                 /// used with pow10[] ->  308 is 1e0
  bool sign= false;           /// tmp used for number sign
  
  /// search for number start
  for(; *p; p++)
    if((*p>= '0' && *p<= '9') || (*p== '-') || (*p== '.'))
      break;

  /// number sign check
  if(*p== '-') {
    sign= true;
    p++;
    // ANOTHER LOOP TILL A NUMBER IS FOUND CAN BE DONE HERE
  }

  for(; *p; p++)
    if(*p>= '0' && *p<= '9')
      i1= (i1* 10)+ (*p- '0');
    else
      break;

  ret= (double)i1;               // integer part

  if(*p== '.') {
    for(p++; *p; p++)
      if(*p>= '0' && *p<= '9') {
        i2= (i2* 10)+ (*p- '0');
        n--;
      } else
        break;

    if(n< 0) n= 0;

    ret/= mlib::pow10d[n];
    ret+= (double)i2;
    ret*= mlib::pow10d[n];             // frac part
  }

  if(*p== 'e' || *p== 'E') {
    p++;
    bool sign2= false;
    if(*p== '-') {
      sign2= true;
      p++;
    }
    if(*p== '+') p++;

    int i= 0;
    for(; *p; p++)
      i= (i* 10)+ (*p- '0');
    if(i> 308) i= 308;

    if(sign2) i= -i;

    ret*= mlib::pow10d[308+ i];
  }

  if(sign) ret= -ret;

  return ret;
}


///-------------------------///
// number to UTF8 =====----- //
///-------------------------///

int Str::int64toUtf8(int64_t n, char *buf, int8_t base, bool uppercase) {
  if(base!= 10 && base!= 2 && base!= 8 && base!= 16) base= 10; // safety
  uint8_t *p= (uint8_t *)buf;       /// p will walk buf, backwards
  if(n== 0) { *p= '0'; *(p+ 1)= 0; return 1; }  /// special case

  /// number sign
  int8_t sign= (n< 0? -1: 1);

  /// check the number length in characters
  int len= 0;                   /// len is also return value - the number of characters in the string
  int64_t t= n;                 /// t is used only in the next few lines
  if(t< 0) len++;
  while(t)
    t/= base, len++;

  // start to populate the string
  p+= len+ 1;                   /// space for terminator, and 1 because of the algorithm (*--p)
  *--p= 0;                      /// string terminator
  
  /// zecimal number
  if(base== 10)
    while(n)
      *--p= '0'+ (uint8_t)(sign* (n% 10)),
      n/= 10;

  /// binary/ octal/ hexa number
  else {
    uint8_t c= uppercase? 'A': 'a';
    while(n) {
      int8_t a= (int8_t)(sign* (n% base));
      if(a> 9)
        *--p= c+ a- 10;
      else
        *--p= '0'+ a;
      n/= base;
    }
  }

  if(sign< 0) *--p= '-';        /// negative number sign

  return len;
}


int Str::uint64toUtf8(uint64_t n, char *buf, int8_t base, bool uppercase) {
  if(base!= 10 && base!= 2 && base!= 8 && base!= 16) base= 10; // safety
  uint8_t *p= (uint8_t *)buf;   /// p will walk buf, backwards
  if(n== 0) { *p= '0'; *(p+ 1)= 0; return 1; }  /// special case

  /// check the number length in characters
  int len= 0;                   /// len is also return value - the number of characters in the string
  uint64_t t= n;                /// t is used only in the next few lines

  while(t)
    t/= base, len++;

  // start to populate the string
  p+= len+ 1;                   /// space for terminator, and 1 because of the algorithm (*--p)
  *--p= 0;                      /// string terminator
  
  /// zecimal number
  if(base== 10)
    while(n)
      *--p= '0'+ (uint8_t)(n% 10),
      n/= 10;

  /// binary/ octal/ hexa number
  else {
    uint8_t c= uppercase? 'A': 'a';
    while(n) {
      uint8_t a= (uint8_t)(n% base);
      if(a> 9)
        *--p= c+ a- 10;
      else
        *--p= '0'+ a;
      n/= base;
    }
  }

  return len;
}


int Str::floatToUtf8(float n, char *buf, int precision, bool useE) {
  uint8_t *p= (uint8_t *)buf; /// p will populate the string, backwards
  int len= 0;                 /// return value - number of characters in string

  /// NaN check
  if(n!= n) {
    p[0]= 'N'; p[1]= 'a'; p[2]= 'N'; p[3]= 0;
    return 3;
  }

  /// INFINITY check
  if((n- n) != 0.0f) {
    if(n > 0.0f) p[0]= '+';
    else         p[0]= '-';
    p[1]= 'I'; p[2]= 'N'; p[3]= 'F'; p[4]= 0;
    return 4;
  }

  /// negative numbers will be switched to positive and the sign remembered
  bool sign;
  if(n< 0.0f) {
    sign= true;
    n= -n;
    len++;
  } else
    sign= false;

  /// scientific exponent
  int e= 0;
  if(useE) {
    if(n< 1.0f&& n> 0.0f)
      while(n< 1.0f)
        n*= 10.0f, e--;
    else
      while(n> 10.0f)
        n/= 10.0f, e++;
  }
  
  int64_t n1= (int64_t)n;
  int64_t n2= (int64_t)((n- (int64_t)n)* pow10i[20+ precision]);

  /// compute number length in string chars
  int64_t t= n1;          /// integer part length
  if(t== 0) len++;
  while(t)
    t/= 10, len++;
  if(precision)           /// float part length
    len+= precision+ 1;
  if(e) {                 /// exponent length
    t= e;
    while(t)
      t/= 10, len++;
    len+= 2;
  }

  // buf will be filled backwards by p
  p+= len+ 1;                 /// 2= space for terminator+ the way the algorithm works (*--p)
  *--p= 0;                    /// string terminator

  /// scientific exponent
  if(e) {
    uint8_t s= (e>= 0? '+': '-');
    if(e< 0) e= -e;
    while(e)
      *--p= '0'+ e% 10, e/= 10;
    *--p= s;
    *--p= 'e';
  }

  /// fractionary part only if precision > 0
  if(precision) {
    for(int a= 0; a< precision; a++)
      *--p= '0'+ n2% 10, n2/= 10;
    *--p= '.';
  }

  /// integer part
  if(n1== 0) *--p= '0';         /// n1 is 0 - special case
  else 
    while(n1)                   /// n1 to text
      *--p= '0'+ n1% 10, n1/= 10;

  if(sign)
    *--p= '-';

  return len;
}


int Str::doubleToUtf8(double n, char *buf, int precision, bool useE) {
  uint8_t *p= (uint8_t *)buf; /// p will populate the string, backwards
  int len= 0;                 /// return value - number of characters in string
  /// NaN check
  if(n!= n) {
    p[0]= 'N', p[1]= 'a', p[2]= 'N', p[3]= 0;
    return 3;
  }

  /// INFINITY check
  if((n- n) != 0.0) {
    if(n > 0.0) p[0]= '+';
    else        p[0]= '-';
    p[1]= 'I', p[2]= 'N', p[3]= 'F', p[4]= 0;
    return 4;
  }

  /// negative numbers will be switched to positive and the sign remembered
  bool sign;
  if(n< 0.0) {
    sign= true;
    n= -n;
    len++;
  } else
    sign= false;

  /// scientific exponent
  int e= 0;
  if(useE) {
    if(n< 1.0)
      while(n> 0.0 && n< 1.0)
        n*= 10.0, e--;
    else
      while(n> 10.0)
        n/= 10.0, e++;
  }
  
  int64_t n1= (int64_t)n;
  int64_t n2= (int64_t)((n- (int64_t)n)* pow10i[20+ precision]);

  /// compute number length in string chars
  int64_t t= n1;          /// integer part length
  if(t== 0) len++;
  while(t)
    t/= 10, len++;
  if(precision)           /// float part length
    len+= precision+ 1;
  if(e) {                 /// exponent length
    t= e;
    while(t)
      t/= 10, len++;
    len+= 2;
  }

  // buf will be filled backwards by p
  p+= len+ 1;                 /// 2= space for terminator+ the way the algorithm works (*--p)
  *--p= 0;                    /// string terminator

  /// scientific exponent
  if(e) {
    uint8_t s= (e>= 0? '+': '-');
    if(e< 0) e= -e;
    while(e)
      *--p= '0'+ e% 10, e/= 10;
    *--p= s;
    *--p= 'e';
  }

  /// fractionary part only if precision > 0
  if(precision) {
    for(int a= 0; a< precision; a++)
      *--p= '0'+ n2% 10, n2/= 10;
    *--p= '.';
  }

  /// integer part
  if(n1== 0) *--p= '0';         /// n1 is 0 - special case
  else 
    while(n1)                   /// n1 to text
      *--p= '0'+ n1% 10, n1/= 10;

  if(sign)
    *--p= '-';

  return len;
}


///-----------------------------------------///
// number to UTF16 ========----------------- //
///-----------------------------------------///

int Str::int64toUtf16(int64_t n, char16_t *buf, int8_t base, bool uppercase) {
  if(base!= 10 && base!= 2 && base!= 8 && base!= 16) base= 10; // safety
  uint16_t *p= (uint16_t *)buf; /// p will walk buf, backwards
  if(n== 0) { *p= '0'; *(p+ 1)= 0; return 1; }  /// special case

  /// number sign
  int8_t sign= (n< 0? -1: 1);

  /// check the number length in characters
  int len= 0;                   /// len is also return value - the number of characters in the string
  int64_t t= n;                 /// t is used only in the next few lines
  if(t< 0) len++;
  while(t)
    t/= base, len++;

  // start to populate the string
  p+= len+ 1;                   /// space for terminator, and 1 because of the algorithm (*--p)
  *--p= 0;                      /// string terminator
  
  /// zecimal number
  if(base== 10)
    while(n)
      *--p= '0'+ sign* (n% 10), n/= 10;
  
  /// binary/ octal/ hexa number
  else {
    uint32_t c= uppercase? 'A': 'a';
    while(n) {
      int8_t a= sign* (n% base);
      if(a> 9)
        *--p= c+ a- 10;
      else
        *--p= '0'+ a;
      n/= base;
    }
  }

  if(sign< 0) *--p= '-';        /// negative number sign

  return len;
}


int Str::uint64toUtf16(uint64_t n, char16_t *buf, int8_t base, bool uppercase) {
  if(base!= 10 && base!= 2 && base!= 8 && base!= 16) base= 10; // safety
  uint16_t *p= (uint16_t *)buf; /// p will walk buf, backwards
  if(n== 0) { *p= '0'; *(p+ 1)= 0; return 1; }  /// special case

  /// check the number length in characters
  int len= 0;                   /// len is also return value - the number of characters in the string
  uint64_t t= n;                /// t is used only in the next few lines
  
  while(t)
    t/= base, len++;

  // start to populate the string
  p+= len+ 1;                   /// space for terminator, and 1 because of the algorithm (*--p)
  *--p= 0;                      /// string terminator
  
  /// zecimal number
  if(base== 10)
    while(n)
      *--p= '0'+ (n% 10),
      n/= 10;

  /// binary/ octal/ hexa number
  else {
    uint32_t c= uppercase? 'A': 'a';
    while(n) {
      uint32_t a= (uint32_t)(n% base);
      if(a> 9)
        *--p= c+ a- 10;
      else
        *--p= '0'+ a;
      n/= base;
    }
  }

  return len;
}


int Str::floatToUtf16(float n, char16_t *buf, int precision, bool useE) {
  uint16_t *p= (uint16_t *)buf;   /// p will populate the string, backwards
  int len= 0;                 /// return value - number of characters in string

  /// NaN check
  if(n!= n) {
    p[0]= 'N'; p[1]= 'a'; p[2]= 'N'; p[3]= 0;
    return 3;
  }

  /// INFINITY check
  if((n- n) != 0.0f) {
    if(n > 0.0f) p[0]= '+';
    else         p[0]= '-';
    p[1]= 'I'; p[2]= 'N'; p[3]= 'F'; p[4]= 0;
    return 4;
  }

  /// negative numbers will be switched to positive and the sign remembered
  bool sign;
  if(n< 0.0f) {
    sign= true;
    n= -n;
    len++;
  } else
    sign= false;

  /// scientific exponent
  int e= 0;
  if(useE) {
    if(n< 1.0f && n> 0.0f)
      while(n< 1.0f)
        n*= 10.0f, e--;
    else
      while(n> 10.0f)
        n/= 10.0f, e++;
  }
  
  int64_t n1= (int64_t)n;
  int64_t n2= (int64_t)((n- (int64_t)n)* pow10i[20+ precision]);

  /// compute number length in string chars
  int64_t t= n1;          /// integer part length
  if(t== 0) len++;
  while(t)
    t/= 10, len++;
  if(precision)           /// float part length
    len+= precision+ 1;
  if(e) {                 /// exponent length
    t= e;
    while(t)
      t/= 10, len++;
    len+= 2;
  }

  // buf will be filled backwards by p
  p+= len+ 1;                 /// 2= space for terminator+ the way the algorithm works (*--p)
  *--p= 0;                    /// string terminator

  /// scientific exponent
  if(e) {
    uint32_t s= (e>= 0? '+': '-');
    if(e< 0) e= -e;
    while(e)
      *--p= '0'+ e% 10, e/= 10;
    *--p= s;
    *--p= 'e';
  }

  /// fractionary part only if precision > 0
  if(precision) {
    for(int a= 0; a< precision; a++)
      *--p= '0'+ n2% 10, n2/= 10;
    *--p= '.';
  }

  /// integer part
  if(n1== 0) *--p= '0';         /// n1 is 0 - special case
  else 
    while(n1)                   /// n1 to text
      *--p= '0'+ n1% 10, n1/= 10;

  if(sign)
    *--p= '-';

  return len;
}


int Str::doubleToUtf16(double n, char16_t *buf, int precision, bool useE) {
  uint16_t *p= (uint16_t *)buf; /// p will populate the string, backwards
  int len= 0;                   /// return value - number of characters in string
  /// NaN check
  if(n!= n) {
    p[0]= 'N', p[1]= 'a', p[2]= 'N', p[3]= 0;
    return 3;
  }

  /// INFINITY check
  if((n- n) != 0.0) {
    if(n > 0.0) p[0]= '+';
    else        p[0]= '-';
    p[1]= 'I', p[2]= 'N', p[3]= 'F', p[4]= 0;
    return 4;
  }

  /// negative numbers will be switched to positive and the sign remembered
  bool sign;
  if(n< 0.0) {
    sign= true;
    n= -n;
    len++;
  } else
    sign= false;

  /// scientific exponent
  int e= 0;
  if(useE) {
    if(n< 1.0)
      while(n< 1.0 && n> 0.0)
        n*= 10.0, e--;
    else
      while(n> 10.0)
        n/= 10.0, e++;
  }
  
  int64_t n1= (int64_t)n;
  int64_t n2= (int64_t)((n- (int64_t)n)* pow10i[20+ precision]);

  /// compute number length in string chars
  int64_t t= n1;          /// integer part length
  if(t== 0) len++;
  while(t)
    t/= 10, len++;
  if(precision)           /// float part length
    len+= precision+ 1;
  if(e) {                 /// exponent length
    t= e;
    while(t)
      t/= 10, len++;
    len+= 2;
  }

  // buf will be filled backwards by p
  p+= len+ 1;                 /// 1= space for terminator+ the way the algorithm works (*--p)
  *--p= 0;                    /// string terminator

  /// scientific exponent
  if(e) {
    uint32_t s= (e>= 0? '+': '-');
    if(e< 0) e= -e;
    while(e)
      *--p= '0'+ e% 10, e/= 10;
    *--p= s;
    *--p= 'e';
  }

  /// fractionary part only if precision > 0
  if(precision) {
    for(int a= 0; a< precision; a++)
      *--p= '0'+ n2% 10, n2/= 10;
    *--p= '.';
  }

  /// integer part
  if(n1== 0) *--p= '0';         /// n1 is 0 - special case
  else 
    while(n1)                   /// n1 to text
      *--p= '0'+ n1% 10, n1/= 10;

  if(sign)
    *--p= '-';

  return len;
}


///-----------------------------------------///
// number to UTF32 ========----------------- //
///-----------------------------------------///

int Str::int64toUtf32(int64_t n, char32_t *buf, int8_t base, bool uppercase) {
  if(base!= 10 && base!= 2 && base!= 8 && base!= 16) base= 10; // safety
  uint32_t *p= (uint32_t *)buf; /// p will walk buf, backwards
  if(n== 0) { *p= '0'; *(p+ 1)= 0; return 1; }  /// special case

  /// number sign
  int8_t sign= (n< 0? -1: 1);

  /// check the number length in characters
  int len= 0;                   /// len is also return value - the number of characters in the string
  int64_t t= n;                 /// t is used only in the next few lines
  if(t< 0) len++;
  while(t)
    t/= base, len++;

  // start to populate the string
  p+= len+ 1;                   /// space for terminator, and 1 because of the algorithm (*--p)
  *--p= 0;                      /// string terminator
  
  /// zecimal number
  if(base== 10)
    while(n)
      *--p= '0'+ sign* (n% 10), n/= 10;
  
  /// binary/ octal/ hexa number
  else {
    uint32_t c= uppercase? 'A': 'a';
    while(n) {
      int8_t a= sign* (n% base);
      if(a> 9)
        *--p= c+ a- 10;
      else
        *--p= '0'+ a;
      n/= base;
    }
  }

  if(sign< 0) *--p= '-';        /// negative number sign

  return len;
}


int Str::uint64toUtf32(uint64_t n, char32_t *buf, int8_t base, bool uppercase) {
  if(base!= 10 && base!= 2 && base!= 8 && base!= 16) base= 10; // safety
  uint32_t *p= (uint32_t *)buf; /// p will walk buf, backwards
  if(n== 0) { *p= '0'; *(p+ 1)= 0; return 1; }  /// special case

  /// check the number length in characters
  int len= 0;                   /// len is also return value - the number of characters in the string
  uint64_t t= n;                /// t is used only in the next few lines
  
  while(t)
    t/= base, len++;

  // start to populate the string
  p+= len+ 1;                   /// space for terminator, and 1 because of the algorithm (*--p)
  *--p= 0;                      /// string terminator
  
  /// zecimal number
  if(base== 10)
    while(n)
      *--p= '0'+ (n% 10),
      n/= 10;

  /// binary/ octal/ hexa number
  else {
    uint32_t c= uppercase? 'A': 'a';
    while(n) {
      uint32_t a= (uint32_t)(n% base);
      if(a> 9)
        *--p= c+ a- 10;
      else
        *--p= '0'+ a;
      n/= base;
    }
  }

  return len;
}


int Str::floatToUtf32(float n, char32_t *buf, int precision, bool useE) {
  uint32_t *p= (uint32_t *)buf;   /// p will populate the string, backwards
  int len= 0;                 /// return value - number of characters in string

  /// NaN check
  if(n!= n) {
    p[0]= 'N'; p[1]= 'a'; p[2]= 'N'; p[3]= 0;
    return 3;
  }

  /// INFINITY check
  if((n- n) != 0.0f) {
    if(n > 0.0f) p[0]= '+';
    else         p[0]= '-';
    p[1]= 'I'; p[2]= 'N'; p[3]= 'F'; p[4]= 0;
    return 4;
  }

  /// negative numbers will be switched to positive and the sign remembered
  bool sign;
  if(n< 0.0f) {
    sign= true;
    n= -n;
    len++;
  } else
    sign= false;

  /// scientific exponent
  int e= 0;
  if(useE) {
    if(n< 1.0f && n> 0.0f)
      while(n< 1.0f)
        n*= 10.0f, e--;
    else
      while(n> 10.0f)
        n/= 10.0f, e++;
  }
  
  int64_t n1= (int64_t)n;
  int64_t n2= (int64_t)((n- (int64_t)n)* pow10i[20+ precision]);

  /// compute number length in string chars
  int64_t t= n1;          /// integer part length
  if(t== 0) len++;
  while(t)
    t/= 10, len++;
  if(precision)           /// float part length
    len+= precision+ 1;
  if(e) {                 /// exponent length
    t= e;
    while(t)
      t/= 10, len++;
    len+= 2;
  }

  // buf will be filled backwards by p
  p+= len+ 1;                 /// 2= space for terminator+ the way the algorithm works (*--p)
  *--p= 0;                    /// string terminator

  /// scientific exponent
  if(e) {
    uint32_t s= (e>= 0? '+': '-');
    if(e< 0) e= -e;
    while(e)
      *--p= '0'+ e% 10, e/= 10;
    *--p= s;
    *--p= 'e';
  }

  /// fractionary part only if precision > 0
  if(precision) {
    for(int a= 0; a< precision; a++)
      *--p= '0'+ n2% 10, n2/= 10;
    *--p= '.';
  }

  /// integer part
  if(n1== 0) *--p= '0';         /// n1 is 0 - special case
  else 
    while(n1)                   /// n1 to text
      *--p= '0'+ n1% 10, n1/= 10;

  if(sign)
    *--p= '-';

  return len;
}


int Str::doubleToUtf32(double n, char32_t *buf, int precision, bool useE) {
  uint32_t *p= (uint32_t *)buf; /// p will populate the string, backwards
  int len= 0;                   /// return value - number of characters in string
  /// NaN check
  if(n!= n) {
    p[0]= 'N', p[1]= 'a', p[2]= 'N', p[3]= 0;
    return 3;
  }

  /// INFINITY check
  if((n- n) != 0.0) {
    if(n > 0.0) p[0]= '+';
    else        p[0]= '-';
    p[1]= 'I', p[2]= 'N', p[3]= 'F', p[4]= 0;
    return 4;
  }

  /// negative numbers will be switched to positive and the sign remembered
  bool sign;
  if(n< 0.0) {
    sign= true;
    n= -n;
    len++;
  } else
    sign= false;

  /// scientific exponent
  int e= 0;
  if(useE) {
    if(n< 1.0)
      while(n< 1.0 && n> 0.0)
        n*= 10.0, e--;
    else
      while(n> 10.0)
        n/= 10.0, e++;
  }
  
  int64_t n1= (int64_t)n;
  int64_t n2= (int64_t)((n- (int64_t)n)* pow10i[20+ precision]);

  /// compute number length in string chars
  int64_t t= n1;          /// integer part length
  if(t== 0) len++;
  while(t)
    t/= 10, len++;
  if(precision)           /// float part length
    len+= precision+ 1;
  if(e) {                 /// exponent length
    t= e;
    while(t)
      t/= 10, len++;
    len+= 2;
  }

  // buf will be filled backwards by p
  p+= len+ 1;                 /// 1= space for terminator+ the way the algorithm works (*--p)
  *--p= 0;                    /// string terminator

  /// scientific exponent
  if(e) {
    uint32_t s= (e>= 0? '+': '-');
    if(e< 0) e= -e;
    while(e)
      *--p= '0'+ e% 10, e/= 10;
    *--p= s;
    *--p= 'e';
  }

  /// fractionary part only if precision > 0
  if(precision) {
    for(int a= 0; a< precision; a++)
      *--p= '0'+ n2% 10, n2/= 10;
    *--p= '.';
  }

  /// integer part
  if(n1== 0) *--p= '0';         /// n1 is 0 - special case
  else 
    while(n1)                   /// n1 to text
      *--p= '0'+ n1% 10, n1/= 10;

  if(sign)
    *--p= '-';

  return len;
}











///============================================================///
// character / string   insert / delete ----------------------- //
///============================================================///


// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_unicode>- unicode value to insert
// <in_pos>- insert position (selected unicode value at position is moved to the right);
//           if left -1, unicode is inserted at the end of the string;
// <returns>- resulting string length in bytes (including str terminator)
int32_t Str::insert8(char **out_str, char32_t in_unicode, int32_t in_pos) {
  int32_t n1= strlen8(*out_str);      /// current string size
  int32_t n2= utf8nrBytes(in_unicode);/// unicode addition size
  if(!n1) n1= 1;                      /// n1 will hold at least a terminator

  uint8_t *buf= new uint8_t[n1+ n2];  /// will hold the resulting string
  uint8_t *p1= (uint8_t *)*out_str;   /// will walk source string
  uint8_t *p2= buf;                   /// will walk resulting string

  // skip unicodes until insertion point
  /// skip unicodes & copy from old string to new one, what is skipped
  if(in_pos>= 0 && p1) {
    while(*p1) {
      if((*p1 & 0xc0)!= 0x80)       /// if this is not a continuation byte, it's a start of a character
        in_pos--;
      if(in_pos< 0) break;           // loop break condition

      *p2= *p1, p1++, p2++;         /// copy & advance pointers
    }
  }

  /// skip to the end & copy from old string to new one, what is skipped
  else
    if(p1)
      while(*p1)
        *p2= *p1, p2++, p1++;

  
  // insert the new unicode value
  Str::utf32to8(in_unicode, (char *)p2);
  p2+= n2;

  /// copy the rest of the string from the source, if there is any left
  if(p1)
    while((*p2++= *p1++));
  else
    *p2= 0;   /// str terminator

  // resulting string ('return value')
  if(*out_str)
    delete[] (*out_str);
  *out_str= (char *)buf;

  return n1+ n2;
}


// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_unicode>- unicode value to insert
// <in_pos>- insert position (selected unicode value at position is moved to the right);
//           if left -1, unicode is inserted at the end of the string;
// <returns>- resulting string length in bytes (including str terminator)
int32_t Str::insert16(char16_t **out_str, char32_t in_unicode, int32_t in_pos) {
  int32_t n1= strlen16(*out_str);             /// current string size
  int32_t n2= Str::utf16nrBytes(in_unicode);
  if(!n1) n1= 2;                              /// n1 will hold at least a terminator

  void *buf= new uint8_t[n1+ n2];             /// will hold the resulting string
  uint16_t *p1= (uint16_t *)*out_str;         /// will walk source string
  uint16_t *p2= (uint16_t *)buf;              /// will walk resulting string

  // skip unicodes until insertion point
  /// skip unicodes & copy from old string to new one, what is skipped
  if(in_pos>= 0)
    while(in_pos-- && *p1) {
      *p2++= *p1++;
      if(Str::isLowSurrogate(*p1))      /// copy next int16 too, if this is a (low) surrogate
        *p2++= *p1++;
    }

  /// skip to the end & copy from old string to new one, what is skipped
  else
    if(p1)
      while(*p1)
        *p2++= *p1++;

  
  // insert the new unicode value
  Str::utf32to16(in_unicode, (char16_t *)p2);
  p2+= ((in_unicode>= 0x10000)? 2: 1);

  /// copy the rest of the string from the source, if there is any left
  if(p1)
    while((*p2++= *p1++));
  else
    *p2= 0;

  // resulting string ('return value')
  if(*out_str)
    delete[] *out_str;
  *out_str= (char16_t *)buf;

  return n1+ n2;
}



// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_unicode>- unicode value to insert
// <in_pos>- insert position (selected unicode value at position is moved to the right);
//           if left -1, unicode is inserted at the end of the string;
// <returns>- resulting string length in bytes (including str terminator)
int32_t Str::insert32(char32_t **out_str, char32_t in_unicode, int32_t in_pos) {
  int32_t n1= strlen32(*out_str);       /// current string size
  if(!n1) n1= 4;                        /// n1 will hold at least a terminator
  void *buf= new uint8_t[n1+ 4];        /// will hold the resulting string
  uint32_t *p1= (uint32_t *)*out_str;   /// will walk source string
  uint32_t *p2= (uint32_t *)buf;        /// will walk resulting string

  // skip unicodes until insertion point
  /// skip unicodes & copy from old string to new one, what is skipped
  if(in_pos>= 0 && p1)
    while(in_pos && *p1)
      *p2++= *p1++, in_pos--;

  /// skip to the end & copy from old string to new one, what is skipped
  else
    if(p1)                            /// can be null, therefore creating a new string
      while(*p1)
        *p2++= *p1++;
  
  // insert the new unicode value
  *p2++= in_unicode;

  /// copy the rest of the string from the source, if there is any left
  if(*out_str)                        /// can be null, therefore creating a new string
    while((*p2++= *p1++));
  else
    *p2= 0;

  // resulting string ('return value')
  if(*out_str)
    delete[] *out_str;
  *out_str= (char32_t *)buf;

  return n1+ 4;
}



// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_str> - string to insert (UTF8)
// <n>      - number of unicodes to insert; when -1 (default), will insert the whole string
// <in_pos> - insert position (selected unicode value at position is moved to the right);
//            if left -1, unicode is inserted at the end of the string;
// <returns>- resulting string length in bytes (including str terminator)
int32_t Str::insertStr8(char **out_str, const char *in_str, int32_t n, int32_t in_pos) {
  int32_t n1= strlen8(*out_str);              /// current string size
  int32_t n2= (int32_t)(n<= 0? strlen8(in_str)- 1: (getChar8(in_str, n)- in_str)); /// insert string size
  if(!n2) return n1;
  if(!n1) n1= 1;                              /// n1 will hold at least a terminator

  uint8_t *buf= new uint8_t[n1+ n2];          /// will hold the resulting string
  uint8_t *p1= (uint8_t *)*out_str;           /// will walk source string
  uint8_t *p2= buf;                           /// will walk resulting string

  // skip unicodes until insertion point
  /// skip unicodes & copy from old string to new one, what is skipped
  if(in_pos>= 0 && p1) {
    while(*p1) {
      if((*p1 & 0xc0)!= 0x80)       /// if this is not a continuation byte, it's a start of a character
        in_pos--;
      if(in_pos< 0) break;           // loop break condition

      *p2= *p1, p1++, p2++;         /// copy & advance pointers
    }
  }

  /// skip to the end & copy from old string to new one, what is skipped
  else
    if(p1)
      while(*p1)
        *p2= *p1, p2++, p1++;

  // insert the new string
  memcpy(p2, in_str, n2);
  p2+= n2;

  /// copy the rest of the string from the source, if there is any left
  if(p1)
    while((*p2++= *p1++));
  else
    *p2= 0;                       /// str terminator

  // resulting string ('return value')
  if(*out_str)
    delete[] (*out_str);
  *out_str= (char *)buf;

  return n1+ n2;
}


// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_str> - string to insert (UTF16)
// <n>      - number of unicodes to insert; when -1 (default), will insert the whole string
// <in_pos> - insert position (selected unicode value at position is moved to the right);
//            if left -1, unicode is inserted at the end of the string;
// <returns>- resulting string length in bytes (including str terminator)
int32_t Str::insertStr16(char16_t **out_str, const char16_t *in_str, int32_t n, int32_t in_pos) {
  int32_t n1= strlen16(*out_str);     /// current string size
  int32_t n2= (int32_t)(n<= 0? strlen16(in_str)- 2: ((Str::getChar16(in_str, n)- in_str)* 2));    /// insert string size (without terminator)
  if(!n2) return n1;
  if(!n1) n1= 2;                      /// n1 will hold at least a terminator

  void *buf= new uint8_t[n1+ n2];     /// will hold the resulting string
  uint16_t *p1= (uint16_t *)*out_str; /// will walk source string
  uint16_t *p2= (uint16_t *)buf;      /// will walk resulting string

  // skip unicodes until insertion point
  /// skip unicodes & copy from old string to new one, what is skipped
  if(in_pos>= 0 && p1)
    while(in_pos-- && *p1) {
      *p2++= *p1++;                 /// copy & advance pointers
      if(Str::isLowSurrogate(*p1))  /// copy next int16 too, if this is a (low) surrogate
        *p2++= *p1++;
    }

  /// skip to the end & copy from old string to new one, what is skipped
  else
    if(p1)
      while(*p1)
        *p2++= *p1++;
  
  // insert the new string
  Str::memcpy(p2, in_str, n2);
  p2+= n2/ 2;

  /// copy the rest of the string from the source, if there is any left
  if(p1)
    while((*p2++= *p1++));
  else
    *p2= 0;                         /// str terminator

  // resulting string ('return value')
  if(*out_str)
    delete[] *out_str;
  *out_str= (char16_t *)buf;

  return n1+ n2;
}


// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_str> - string to insert (UTF32)
// <n>      - number of unicodes to insert; when -1 (default), will insert the whole string
// <in_pos> - insert position (selected unicode value at position is moved to the right);
//            if left -1, unicode is inserted at the end of the string;
// <returns>- resulting string length in bytes (including str terminator)
int32_t Str::insertStr32(char32_t **out_str, const char32_t *in_str, int32_t n, int32_t in_pos) {
  int32_t n1= strlen32(*out_str);             /// current string size
  int32_t n2= (n< 0? strlen32(in_str)- 4: (n* 4)); /// insert string size
  if(!n2) return n1;
  if(!n1) n1= 4;                              /// n1 will hold at least a terminator

  void *buf= new uint8_t[n1+ n2];             /// will hold the resulting string
  uint32_t *p1= (uint32_t *)*out_str;         /// will walk source string
  uint32_t *p2= (uint32_t *)buf;              /// will walk resulting string

  // skip either unicodes or chars, until insertion point

  /// skip unicodes & copy from old string to new one, what is skipped
  if(in_pos>= 0 && p1)
    while(in_pos-- && *p1)
      *p2++= *p1++;                 /// copy & advance pointers

  /// skip to the end & copy from old string to new one, what is skipped
  else
    if(p1)
      while(*p1)
        *p2= *p1, p2++, p1++;
  
  // insert the new string
  Str::memcpy(p2, in_str, n2);
  p2+= n2/ 4;


  /// copy the rest of the string from the source, if there is any left
  if(p1)
    while((*p2++= *p1++));
  else
    *p2= 0;                         /// str terminator

  // resulting string ('return value')
  if(*out_str)
    delete[] *out_str;
  *out_str= (char32_t *)buf;

  return n1+ n2;
}


// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_nUnicodesToDel>- number of unicodes to del from string;
// <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied;
//          if it remains -1, the last unicode in the string is the position;
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::del8(char **out_str, int32_t in_nUnicodesToDel, int32_t in_pos) {
  int32_t l1= strlen8(*out_str);
  uint8_t *p1= (uint8_t *)*out_str, *p2;
  int32_t cutStart= 0, cutAmount= 0;  /// cut values, in bytes

  if(in_nUnicodesToDel<= 0 || !*p1) return l1;  /// basic checks, for quick return

  // compute where the cutting starts and ends
  /// a valid position is requested
  if(in_pos>= 0) {
    /// find cutStart
    for(; in_pos && *p1; cutStart++)
      if((*++p1 & 0xc0)!= 0x80)     /// skip continuation bytes
        in_pos--;

    /// find cutAmount
    for(; in_nUnicodesToDel && *p1; cutAmount++)
      if((*++p1 & 0xc0)!= 0x80)     /// skip continuation bytes
        in_nUnicodesToDel--;
  }

  /// when position is -1, unicodes are cut from the back of the string
  else {
    /// skip to the end (p1 and cutStart point to str terminator)
    cutStart= l1- 1;
    p1+= cutStart;

    /// back away each unicode until in_nUnicodesToDel is satisfied
    while(in_nUnicodesToDel--) {
      cutStart--, p1--, cutAmount++;
      while((*p1& 0xc0)== 0x80)       /// back every utf8 continuation byte
        cutStart--, p1--, cutAmount++;
    }

    if(cutStart< 0)
      cutStart= 0, cutAmount= l1- 1;  /// just delete everything in this case
  }
  if(cutAmount<= 0) return l1;

  // start cutting - got every variable needed
  int32_t l2= l1- cutAmount;
  if(l2<= 0) return l1;             /// safety check - something is wrong if this happens, SHOULD IT JUST CRASH? (MIGHT BE BETTER FOR BUG SQUISHING)

  uint8_t *buf= new uint8_t[l2];

  /// copy everything until cut start
  p1= buf;
  p2= (uint8_t *)*out_str;

  while(cutStart--)
    *p1= *p2, p1++, p2++;

  /// skip what bytes are deleted
  p2+= cutAmount;

  /// copy what bytes remain after the cutted part
  while((*p1++= *p2++));

  // return / output values
  delete[] (*out_str);
  *out_str= (char *)buf;

  return l2;
}


// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_nUnicodesToDel>- number of unicodes to del from string;
// <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied;
//          if it remains -1, unicodes are deleted at the back of the string
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::del16(char16_t **out_str, int32_t in_nUnicodesToDel, int32_t in_pos) {
  int32_t l1= strlen16(*out_str);
  uint16_t *p1= (uint16_t *)*out_str, *p2;
  int32_t cutStart= 0, cutAmount= 0;          /// cut values (number in int16's)

  if(in_nUnicodesToDel<= 0|| !*p1) return l1; /// basic checks, for quick return

  // compute where the cutting starts and ends
  if(in_pos>= 0) {
    /// find cutStart
    while(in_pos && *p1) {
      in_pos--, p1++, cutStart++;
      if(isLowSurrogate(*p1))
        p1++, cutStart++;
    }
    /// find cutAmount
    while(in_nUnicodesToDel && *p1) {
      in_nUnicodesToDel--, p1++, cutAmount++;
      if(isLowSurrogate(*p1))
        p1++, cutAmount++;
    }
  }

  /// when position is -1, unicodes are cut from the end of the string
  else {
    /// skip to the end of the string (p1 and cutStart point to the str terminator)
    cutStart= (l1/ 2)- 1;
    p1+= cutStart;

    /// back away each unicode until in_nUnicodesToDel is satisfied
    while(in_nUnicodesToDel--) {
      cutStart--, p1--, cutAmount++;
      if(isLowSurrogate(*p1))             /// back away another int16 if this char is made of 2 int16 (2 surrogates)
        cutStart--, p1--, cutAmount++;
    }

    if(cutStart< 0)                       /// just delete everything, but no crashes
      cutStart= 0, cutAmount= (l1/ 2)- 1;
  }

  // start cutting - got every variable needed
  if(cutAmount<= 0) return l1;    /// safety check

  int32_t l2= l1- (cutAmount* 2); /// resulting string length in bytes
  if(l2< 2) return l1;            /// safety check
  uint8_t *buf= new uint8_t[l2];  /// mem alloc resulting string

  /// copy everthing until cut start
  p1= (uint16_t *)buf;
  p2= (uint16_t *)*out_str;
  while(cutStart--)
    *p1++= *p2++;

  p2+= cutAmount;                 // skip cut amount (basically deletion part)

  /// copy everything until source string end
  while((*p1++= *p2++));

  // return / output values
  delete[] *out_str;
  *out_str= (char16_t *)buf;

  return l2;
}


// <out_str>- output (resulting) string; the pointer will be mem-alocated (that's why there's the need for pointer to pointer)
// <in_nUnicodesToDel>- number of unicodes to del from string;
// <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied;
//          if it remains -1, unicodes are deleted at the back of the string
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::del32(char32_t **out_str, int32_t in_nUnicodesToDel, int32_t in_pos) {
  int32_t l1= strlen32(*out_str);
  uint32_t *p1= (uint32_t *)*out_str, *p2;
  int32_t cutStart= 0, cutAmount= 0;  /// cut values (number of unicodes)

  if(in_nUnicodesToDel<= 0 || !*p1) return l1;  /// basic checks for quick return

  // compute where the cutting starts and ends
  if(in_pos>= 0) {
    cutStart= in_pos;
    cutAmount= in_nUnicodesToDel;

  /// when position stays -1, unicodes are cut from the end of the string
  } else {
    /// cutStart will point to the str terminator
    cutStart= l1/ 4- 1;
    /// rest is easy to figure out
    cutStart-= in_nUnicodesToDel;
    cutAmount= in_nUnicodesToDel;

    if(cutStart< 0)                         /// delete everything in this case
      cutStart= 0, cutAmount= (l1/ 4)- 1;
  }

  // start cutting - got every variable needed
  if(cutAmount<= 0) return l1;    /// safety check

  int32_t l2= l1- (cutAmount* 4); /// resulting string length in bytes
  if(l2< 4) return l1;            /// safety check
  uint8_t *buf= new uint8_t[l2];  /// resulting string

  /// copy everything till cut start
  p1= (uint32_t *)buf;
  p2= (uint32_t *)*out_str;

  while(cutStart--)
    *p1++= *p2++;

  p2+= cutAmount;             // skip what is being deleted

  /// copy everyhing after deleted part, until string end
  while((*p1++= *p2++));

  // return / output values
  delete[] *out_str;
  *out_str= (char32_t *)buf;

  return l2;
}




///------------------------------------------------------------------------------------------------///
// insert / delete of strings / single univals, working on a static buffer, no memory allocs happen //
///------------------------------------------------------------------------------------------------///


// <out_buf>- output buffer, it must be preallocated, no mem allocs happen in this func
// <in_bufSize>- [REQUIRED] out_buf's size, in bytes;
// <in_unicode>- unicode value to insert;
// <in_pos>- insert position (selected unicode value at position is moved to the right);
//           if left -1, unicode is inserted at the end of the string;
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::insert8static(char *out_buf, int32_t in_bufSize, char32_t in_unicode, int32_t in_pos) {
  int32_t n1= strlen8(out_buf);       /// current string size
  int32_t n2= utf8nrBytes(in_unicode);/// unicode addition size
  if(n1+ n2> in_bufSize) return n1;   /// unicode value must fit inside the buffer

  uint8_t *p1, *p2, *insertPoint;

  /// compute insertion point (insertPoint)
  if(in_pos>= 0)
    insertPoint= (uint8_t *)Str::getUnicode8(out_buf, in_pos);
  else            
    insertPoint= (uint8_t *)out_buf+ n1- 1; /// -1 position means at the end of the string
  
  /// move everything to the right to make room for inserted unival
  p2= (uint8_t *)out_buf+ n1- 1;
  p1= p2+ n2;
  while(p2>= insertPoint)
    *p1--= *p2--;
  
  // character insert
  Str::utf32to8(in_unicode, (char *)insertPoint);

  return n1+ n2;
}


// <out_buf>- output buffer, it must be preallocated, no mem allocs happen in this func
// <in_bufSize>- [REQUIRED] out_buf's size, in int16 units;
// <in_unicode>- unicode value to insert;
// <in_pos>- insert position (selected unicode value at position is moved to the right);
//           if left -1, unicode is inserted at the end of the string;
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::insert16static(char16_t *out_buf, int32_t in_bufSize, char32_t in_unicode, int32_t in_pos) {
  int32_t n1= strlen16(out_buf);            /// current string size in bytes
  int32_t n2= (in_unicode>= 0x10000? 4: 2); /// unicode addition size in bytes (it can consist of 2 surrogates)
  if(n1+ n2> in_bufSize) return n1;         /// unicode value must fit inside the buffer;

  uint16_t *p1, *p2, *insertPoint;

  /// compute insertion point (insertPoint)
  if(in_pos>= 0)
    insertPoint= (uint16_t *)Str::getUnicode16(out_buf, in_pos);
  else
    insertPoint= (uint16_t *)out_buf+ (n1/ 2)- 1;     /// -1 on both means insert at the back of the string

  /// move everything to the right to make room for inserted unival
  p2= (uint16_t *)out_buf+ (n1/ 2)- 1;
  p1= p2+ (n2/ 2);
  while(p2>= insertPoint)
    *p1--= *p2--;

  // character insert
  Str::utf32to16(in_unicode, (char16_t *)insertPoint);

  return n1+ n2;
}


// <out_buf>- output buffer, it must be preallocated, no mem allocs happen in this func
// <in_bufSize>- [REQUIRED] out_buf's size, in int32 units;
// <in_unicode>- unicode value to insert;
// <in_pos>- insert position (selected unicode value at position is moved to the right);
//           if left -1, unicode is inserted at the end of the string;
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::insert32static(char32_t *out_buf, int32_t in_bufSize, char32_t in_unicode, int32_t in_pos) {
  int32_t n= strunicodes32(out_buf);    /// number of unicode values in current string
  if(n+ 1> in_bufSize- 1) return n* 4;  /// unicode value must fit inside the buffer;

  uint32_t *p1, *p2, *insertPoint;

  /// compute insertion point (insertPoint)
  if(in_pos>= 0)
    insertPoint= (uint32_t *)Str::getUnicode32(out_buf, in_pos);
  else
    insertPoint= (uint32_t *)out_buf+ n;
  
  /// move everything 1 univode value to the right
  p2= (uint32_t *)out_buf+ n;
  p1= p2+ 1;
  while(p2>= insertPoint)
    *p1--= *p2--;

  // unicode insert
  *insertPoint= in_unicode;

  return (n+ 2)* 4;
}


// <out_buf>   - output buffer, it must be preallocated, no mem allocs happen in this func
// <in_bufSize>- [REQUIRED] out_buf's size, in bytes;
// <in_str>    - NULL terminated string that is to be inserted in out_buf
// <n>         - number of unicodes to insert; when -1 (default), will insert the whole string
// <in_pos>    - insert position (in unicodes); when -1 (default), position is at end of string
// <return>    - resulting string length in bytes (incl str terminator)
int32_t Str::insertStr8static(char *out_buf, int32_t in_bufSize, const char *in_str, int32_t in_n, int32_t in_pos) {
  int32_t n1= strlen8(out_buf);       /// current string size
  int32_t n2= (int32_t)(in_n<= 0? strlen8(in_str)- 1: (getChar8(in_str, in_n)- in_str)); /// added string size

  uint8_t *p1, *p2, *insertPoint;

  /// string must fit inside the buffer - if it doesn't, whatever is possible to insert is inserted
  if(n1+ n2> in_bufSize) {
    /// pass thru in_str, try to find out how many characters would fit in the new string
    for(p1= (uint8 *)in_str; *p1; ) {
      int32_t n= Str::utf8headerBytes(*p1);
      if(n1+ n2+ n<= in_bufSize)  /// character fits
        n2+= n, p1+= n;
      else                        /// doesn't fit, end of added string
        break;
    }
  }
  if(n2<= 0) return n1;      /// if not even one character fits in out_buf, then just return

  // find out insertPoint - where the new string will be placed
  if(in_pos>= 0)
    insertPoint= (uint8_t *)Str::getUnicode8(out_buf, in_pos);
  else
    insertPoint= (uint8_t *)out_buf+ n1- 1;

  /// move everything to the right, to make space for iserted string
  p2= (uint8_t *)out_buf+ n1- 1;
  p1= p2+ n2;
  while(p2>= insertPoint)
    *p1--= *p2--;

  // string insert
  int32 n= n2;
  p1= (uint8_t *)in_str;
  while(n--)
    *insertPoint++= *p1++;

  return n1+ n2;
}


// <out_buf>   - output buffer, it must be preallocated, no mem allocs happen in this func
// <in_bufSize>- [REQUIRED] out_buf's size, in int15 units;
// <in_str>    - NULL terminated string that is to be inserted in out_buf
// <n>         - number of unicodes to insert; when -1 (default), will insert the whole string
// <in_pos>    - insert position (in unicodes); when -1 (default), position is at end of string
// <return>    - resulting string length in bytes (incl str terminator)
int32_t Str::insertStr16static(char16_t *out_buf, int32_t in_bufSize, const char16_t *in_str, int32_t in_n, int32_t in_pos) {
  int32_t n1= strlen16(out_buf);
  int32_t n2= (int32_t)(in_n<= 0? strlen16(in_str)- 2: (getChar16(in_str, in_n)- in_str));
  uint16_t *p1, *p2, *insertPoint;

  /// string must fit inside the buffer - if it doesn't, whatever is possible to insert is inserted
  if((n1+ n2)/ 2> in_bufSize) {
    /// pass thru in_str, try to find out how many characters would fit in the new string
    p1= (uint16_t *)in_str;
    int16 i1= n1/ 2, i2= 0;         /// better to use these than convert from byte->int16 every pass of the loop
    while(*p1) {
      int32 n= isHighSurrogate(*p1)? 2: 1;
      if(i1+ i2+ n<= in_bufSize)    /// character fits
        i2+= n, p1+= n;
      else                          /// doesn't fit, end of added string
        break;
    }
    n2= i2* 2;
  }
  if(n2<= 0) return n1;

  /// finding insert point
  if(in_pos>= 0)
    insertPoint= (uint16_t *)Str::getUnicode16(out_buf, in_pos);
  else
    insertPoint= (uint16_t *)out_buf+ (n1/ 2)- 1;  /// end of the string

  /// move everything n2 chars over
  p2= (uint16_t *)out_buf+ (n1/ 2)- 1;
  p1= p2+ (n2/ 2);
  while(p2>= insertPoint)
    *p1--= *p2--;

  // string insert
  int32 n= n2/ 2;
  while(n--)
    *insertPoint++= *in_str++;

  return n1+ n2;
}


// <out_buf>   - output buffer, it must be preallocated, no mem allocs happen in this func
// <in_bufSize>- [REQUIRED] out_buf's size, in int32 units;
// <in_str>    - NULL terminated string that is to be inserted in out_buf
// <n>         - number of unicodes to insert; when -1 (default), will insert the whole string
// <in_pos>    - insert position (in unicodes); when -1 (default), position is at end of string
// <return>    - resulting string length in bytes (incl str terminator)
int32_t Str::insertStr32static(char32_t *out_buf, int32_t in_bufSize, const char32_t *in_str, int32_t in_n, int32_t in_pos) {
  int32_t n1= strunicodes32(out_buf);
  int32_t n2= (in_n<= 0? strunicodes32(in_str): in_n);
  
  /// inserted string must fit inside the fixed length buffer
  if(n1+ n2+ 1> in_bufSize)
    n2= in_bufSize- n1- 1;
  if(n2<= 0) return (n1+ 1)* 4;

  uint32_t *p1, *p2, *insertPoint;

  /// finding out insert point
  if(in_pos>= 0)
    insertPoint= (uint32_t *)Str::getUnicode32(out_buf, in_pos);
  else
    insertPoint= (uint32_t *)out_buf+ n1;         /// back of the string (-1)

  /// move everything n2 chars over
  p2= (uint32_t *)out_buf+ n1;
  p1= p2+ n2;
  while(p2>= insertPoint)
    *p1--= *p2--;

  // string insert
  int32 n= n2;
  while(n--)
    *insertPoint++= *in_str++;

  return (n1+ n2+ 1)* 4;
}


// <out_buf>- output buffer, it must be preallocated, no mem allocs happen in this func
// <in_nUnicodesToDel>- number of unicodes to del from str;
// <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied;
//           if it remains -1, the last unicode in the string is the position;
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::del8static(char *out_buf, int32_t in_nUnicodesToDel, int32_t in_pos) {
  int32_t l= strlen8(out_buf);
  uint8_t *p1= (uint8_t *)out_buf, *p2;
  int32_t cutStart= 0, cutAmount= 0;  /// cut values, in bytes

  if(in_nUnicodesToDel<= 0 || !*p1) return l;

  // compute where the cutting starts and ends
  /// a valid position is requested
  if(in_pos>= 0) {
    /// find cutStart
    for(; in_pos && *p1; cutStart++)
      if((*++p1 & 0xc0)!= 0x80)     /// skip continuation bytes
        in_pos--;

    /// find cutAmount
    for(; in_nUnicodesToDel && *p1; cutAmount++)
      if((*++p1 & 0xc0)!= 0x80)     /// skip continuation bytes
        in_nUnicodesToDel--;
  }

  /// when position is -1, unicodes are cut from the back of the string
  else {
    /// skip to the end (p1 and cutStart point to str terminator)
    cutStart= l- 1;
    p1+= cutStart;

    /// back away each unicode until in_nUnicodesToDel is satisfied
    while(in_nUnicodesToDel--) {
      cutStart--, p1--, cutAmount++;
      while((*p1& 0xc0)== 0x80)       /// back every utf8 continuation byte
        cutStart--, p1--, cutAmount++;
    }

    if(cutStart< 0)
      cutStart= 0, cutAmount= l- 1;  /// just delete everything in this case
  }
  if(cutAmount<= 0) return l;
  if(l- cutAmount<= 0) return l;

  // start cutting - got every variable needed

  /// move everything from the right of the cut to the start of the cut (overwrite basically)
  p1= (uint8_t *)out_buf+ cutStart;
  p2= p1+ cutAmount;
  while((*p1++= *p2++));

  return l- cutAmount;
}


// <out_buf>- output buffer, it must be preallocated, no mem allocs happen in this func
// <in_nUnicodesToDel>- number of unicodes to del from str;
// <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied;
//           if it remains -1, the last unicode in the string is the position;
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::del16static(char16_t *out_buf, int32_t in_nUnicodesToDel, int32_t in_pos) {
  int32_t l= strlen16(out_buf);
  uint16_t *p1= (uint16_t *)out_buf, *p2;
  int32_t cutStart= 0, cutAmount= 0;  /// cut values (number of unicodes)
  if(in_nUnicodesToDel<= 0 || !*p1) return l;

  // compute where the cutting starts and ends
  if(in_pos>= 0) {
    /// find cutStart
    while(in_pos && *p1) {
      in_pos--, p1++, cutStart++;
      if(isLowSurrogate(*p1))
        p1++, cutStart++;
    }
    /// find cutAmount
    while(in_nUnicodesToDel && *p1) {
      in_nUnicodesToDel--, p1++, cutAmount++;
      if(isLowSurrogate(*p1))
        p1++, cutAmount++;
    }
  }

  /// when position is -1, unicodes are cut from the end of the string
  else {
    /// skip to the end of the string (p1 and cutStart point to the str terminator)
    cutStart= (l/ 2)- 1;
    p1+= cutStart;

    /// back away each unicode until in_nUnicodesToDel is satisfied
    while(in_nUnicodesToDel--) {
      cutStart--, p1--, cutAmount++;
      if(isLowSurrogate(*p1))             /// back away another int16 if this char is made of 2 int16 (2 surrogates)
        cutStart--, p1--, cutAmount++;
    }

    if(cutStart< 0)                       /// just delete everything, but no crashes
      cutStart= 0, cutAmount= (l/ 2)- 1;
  }
  if(cutAmount<= 0) return l;
  if(l- (cutAmount* 2)< 2) return l;

  // start cutting - got every variable needed
  p1= (uint16_t *)out_buf+ cutStart;
  p2= p1+ cutAmount;
  while((*p1++= *p2++));

  return l- (cutAmount* 2);
}


// <out_buf>- output buffer, it must be preallocated, no mem allocs happen in this func
// <in_nUnicodesToDel>- number of unicodes to del from str;
// <in_pos>- delete position- selected unicode is deleted and every unicode to the right until <in_nrUnicodesToDel> is satisfied;
//           if it remains -1, the last unicode in the string is the position;
// <return>- resulting string length in bytes (incl str terminator)
int32_t Str::del32static(char32_t *out_buf, int32_t in_nUnicodesToDel, int32_t in_pos) {
  int32_t l= strlen32(out_buf);
  uint32_t *p1= (uint32_t *)out_buf, *p2;
  int32_t cutStart= 0, cutAmount= 0;  /// cut values (number of unicodes)
  if(in_nUnicodesToDel<= 0 || !*p1) return l;

  // compute where the cutting starts and ends
  /// specific location is avaible for cut
  if(in_pos>= 0)
    cutStart= in_pos, cutAmount= in_nUnicodesToDel;

  /// when nothing is requested, by default the string is cut from the back
  else {
    /// cutStart will point to the str terminator
    cutStart= l/ 4;
    /// rest is easy to figure out
    cutStart-= in_nUnicodesToDel;
    cutAmount= in_nUnicodesToDel;

    if(cutStart< 0)                 /// delete everything in this case
      cutStart= 0, cutAmount= (l/ 4)- 1;
  }
  if(cutAmount<= 0) return l;       /// safety check
  int32 ret= l- (cutAmount* 4);     /// resulting string length in bytes
  if(ret< 4) return l;              /// safety check

  // start cutting - got every variable needed
  p1= (uint32_t *)out_buf+ cutStart;
  p2= p1+ cutAmount;
  while((*p1++= *p2++));

  return ret;
}






// used for search8 func ATM, only
// unpacks utf8 character, and returns the number of bytes that the utf8 character is packed on, too
inline void _utf8to32custom(const uint8_t *p, char32_t *out_unicode= NULL, int *out_utf8nrBytes= NULL) {
  uint32_t ret= 0;
  int n= 0;

  /// check how many chars this utf8 pack has
  if(*p < 128)                n= 1;
  else if((*p& 0xe0) == 0xc0) n= 2, ret= (*p++)& 0x1f;
  else if((*p& 0xf0) == 0xe0) n= 3, ret= (*p++)& 0x0f;
  else if((*p& 0xf8) == 0xf0) n= 4, ret= (*p++)& 0x07;
  else if((*p& 0xfc) == 0xf8) n= 5, ret= (*p++)& 0x03;
  else if((*p& 0xfe) == 0xfc) n= 6, ret= (*p++)& 0x01;
  else ret= 0xFFFD;

  if(out_utf8nrBytes) *out_utf8nrBytes= !n? 1: n;

  /// unpack the utf8
  if(n== 1) ret= *p;
  else while(--n> 0)
    ret<<= 6, ret+= *p++ & 0x3f;

  if(out_unicode) *out_unicode= ret;
}



// <in_str>- main text
// <in_search>- search text to be found in <in_str>
// <in_caseSensitive>- use case sensitive or not
// <return>- NULL if nothing found;
//           IF FOUND, points to the begining of found text location in the <in_str>
char *Str::search8(const char *in_str, const char *in_search, bool in_caseSensitive) {
  uint8_t *p1= (uint8_t *)in_str;
  uint8_t *p2= (uint8_t *)in_search;
  char32_t c1, c2, first;
  int skip1, skip2, skip3;

  if((p1== NULL) || (p2== NULL)) return NULL;
  if((!*p1) || (!*p2)) return NULL;

  _utf8to32custom(p2, &first);     /// this will never change

  while(*p1) {

    _utf8to32custom(p1, &c1, &skip3);
    
    /// first characters match, possible whole search match
    if(in_caseSensitive? c1== first: (c1== first) || (tolower(c1)== tolower(first))) {
      uint8_t *potentialMatch= p1;
      bool match= true;             /// start true, mark false if not matching

      while(*p2 && *p1) {
        _utf8to32custom(p1, &c1, &skip1);
        _utf8to32custom(p2, &c2, &skip2);

        if(in_caseSensitive? c1!= c2: (c1!= c2) && (tolower(c1) != tolower(c2))) {
          match= false;
          break;
        }
        p1+= skip1;
        p2+= skip2;
      }

      if(*p2) match= false;       /// there is still stuff in p2, and p1 reached the end

      // found exact match
      if(match)
        return (char *)potentialMatch;

      /// reset pointers to search further
      p1= potentialMatch;
      p2= (uint8_t *)in_search;
    }

    /// not found, search more
    p1+= skip3;
  }
  return NULL;
}

// <in_str>- main text
// <in_search>- search text to be found in <in_str>
// <in_caseSensitive>- use case sensitive or not
// <return>- NULL if nothing found;
//           IF FOUND, points to the begining of found text location in the <in_str>
char16_t *Str::search16(const char16_t *in_str, const char16_t *in_search, bool in_caseSensitive) {
  uint16_t *p1= (uint16_t *)in_str;
  uint16_t *p2= (uint16_t *)in_search;
  uint32_t c1, c2, first;

  if((p1== NULL) || (p2== NULL)) return NULL;
  if((!*p1) || (!*p2)) return NULL;
  
  first= Str::utf16to32((char16_t *)p2); /// this will never change

  while(*p1) {
    c1= Str::utf16to32((char16_t *)p1);

    /// first characters match, possible whole search match
    if(in_caseSensitive? c1== first: (c1== first) || (tolower(c1)== tolower(first))) {
      uint16_t *potentialMatch= p1;
      bool match= true;             /// start true, mark false if not matching

      while(*p1 && *p2) {
        c1= Str::utf16to32((char16_t *)p1);
        c2= Str::utf16to32((char16_t *)p2);

        if(in_caseSensitive? c1!= c2: (c1!= c2) && (tolower(c1) != tolower(c2))) {
          match= false;
          break;
        }
        p1++, p2++;
        if(Str::isLowSurrogate(*p1))
          p1++, p2++;
      }

      if(*p2) match= false;       /// there is still stuff in p2, and p1 reached the end

      // found exact match
      if(match)
        return (char16_t *)potentialMatch;

      /// reset pointers to search further
      p1= potentialMatch;
      p2= (uint16_t *)in_search;
    }

    /// not found, search more
    p1++;
    if(Str::isLowSurrogate(*p1))
      p1++;
  }
  return NULL;
}

// <in_str>- main text
// <in_search>- search text to be found in <in_str>
// <in_caseSensitive>- use case sensitive or not
// <return>- NULL if nothing found;
//           IF FOUND, points to the begining of found text location in the <in_str>
char32_t *Str::search32(const char32_t *in_str, const char32_t *in_search, bool in_caseSensitive) {
  uint32_t *p1= (uint32_t *)in_str;
  uint32_t *p2= (uint32_t *)in_search;

  if((p1== NULL) || (p2== NULL)) return NULL;
  if((!*p1) || (!*p2)) return NULL;
  
  while(*p1) {
    
    /// first characters match, possible whole search match
    if(in_caseSensitive? *p1== *p2: (*p1== *p2) || (tolower(*p1)== tolower(*p2))) {
      uint32_t *potentialMatch= p1;
      bool match= true;             /// start true, mark false if not matching

      while(*p2 && *p1) {
        if(in_caseSensitive? *p1!= *p2: (*p1!= *p2) && (tolower(*p1) != tolower(*p2))) {
          match= false;
          break;
        }
        p1++, p2++;
      }

      if(*p2) match= false;       /// there is still stuff in p2, and p1 reached the end

      // found exact match
      if(match)
        return (char32_t *)potentialMatch;

      /// reset pointers to search further
      p1= potentialMatch;
      p2= (uint32_t *)in_search;
    }

    /// not found, search more
    p1++;
  } 
  return NULL;
}













