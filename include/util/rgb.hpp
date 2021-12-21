#pragma once

#include "osi/include/util/rgb.hpp"

#define b2f(x) ((float)x)* (1.0f/ 255.0f)
#define f2b(x) (uint8_t)(((float)x)* 255.0f)

struct rgb;



///=====================///
// RGBA (4 bytes, 0-255) //
///=====================///

struct rgba {
  union {
    struct { uint8_t r, g, b, a; };
    uint8_t c[4];
  };

  // constructors

  inline rgba(): r(0), g(0), b(0), a(255) {}
  inline rgba(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a): r(_r), g(_g), b(_b), a(_a) {}
  inline rgba(uint8_t v[]): r(v[0]), g(v[1]), b(v[2]), a(v[3]) {}
  inline rgba(const rgba &o): r(o.r), g(o.g), b(o.b),  a(o.a) {}
  inline rgba(const rgb  &o); // at end of file
  inline rgba(const mlib::vec4 &o): r(f2b(o.r)), g(f2b(o.g)), b(f2b(o.b)), a(f2b(o.a)) {}
  inline rgba(const mlib::vec3 &o): r(f2b(o.r)), g(f2b(o.g)), b(f2b(o.b)), a(255) {}
  inline rgba(uint32_t n) { setUInt32(n); }
  inline rgba(uint16_t n) { setUInt16(n); }

  // operators

  inline rgba& operator= (const rgba &o) { r= o.r, g= o.g, b= o.b, a= o.a; return *this; }
  inline rgba& operator= (const rgb  &o); // at end of file
  inline rgba& operator= (const mlib::vec4 &o) { r= f2b(o.r), g= f2b(o.g), b= f2b(o.b), a= f2b(o.a); return *this; }
  inline rgba& operator= (const mlib::vec3 &o) { r= f2b(o.r), g= f2b(o.g), b= f2b(o.b), a= 255; return *this; }
  inline rgba& operator= (uint32_t v) { setUInt32(v); return *this; }
  inline rgba& operator= (uint16_t v) { setUInt16(v); return *this; }

  inline operator uint8_t *()             { return c; }
  inline operator const uint8_t *() const { return c; }
  inline uint8_t& operator[](int i)       { return c[i]; }

  // funcs

  inline rgba& setUInt32(uint32_t v) { r= (uint8_t)(v>> 16),
                                     g= (uint8_t)((uint16_t)v>> 8),
                                     b= (uint8_t)v,
                                     a= (uint8_t)(v>> 24); return *this; }

  inline uint32_t getUInt32() { return ((uint32_t)a<< 24)+  ((uint32_t)r<< 16)+ ((uint32_t)g<< 8)+ (uint32_t)b; }

  inline rgba& setUInt16(uint16_t value) { r= ((value>> 8)>> 3)<< 3,
                                         g= (value>> 5)<< 2,
                                         b= (uint16_t) (((uint16_t)value<< 11)>> 11) << 3,
                                         a= 255; return *this; }

  inline uint16_t getUInt16() { return (uint16_t)((b>> 3)+ ((g>> 2)<< 5)+ ((r>> 3)<< 0xb)); }

  inline rgba& set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a= 255) { r= _r, g= _g, b= _b, a= _a; return *this; }
};





///====================///
// RGB (3 bytes, 0-255) //
///====================///

struct rgb {
  union {
    struct { uint8_t r, g, b; };
    uint8_t c[3];
  };
  
  // constructors

  inline rgb(): r(0), g(0), b(0) {}
  inline rgb(uint8_t _r, uint8_t _g,  uint8_t _b): r(_r), g(_g), b(_b) {}
  inline rgb(const uint8_t v[]): r(v[0]), g(v[1]), b(v[2]) {}
  inline rgb(const rgb  &o): r(o.r), g(o.g), b(o.b) {}
  inline rgb(const rgba &o): r(o.r), g(o.g), b(o.b) {}
  inline rgb(const mlib::vec3  &o): r(f2b(o.r)), g(f2b(o.g)), b(f2b(o.b)) {}
  inline rgb(const mlib::vec4  &o): r(f2b(o.r)), g(f2b(o.g)), b(f2b(o.b)) {}
  inline rgb(uint32_t n) { setUInt32(n); }
  inline rgb(uint16_t n) { setUInt16(n); }

  // operators

  inline rgb& operator= (const rgb  &o) { r= o.r, g= o.g, b= o.b; return *this; }
  inline rgb& operator= (const rgba &o) { r= o.r, g= o.g, b= o.b; return *this; }
  inline rgb& operator= (const mlib::vec3 &o) { r= f2b(o.r), g= f2b(o.g), b= f2b(o.b); return *this; }
  inline rgb& operator= (const mlib::vec4 &o) { r= f2b(o.r), g= f2b(o.g), b= f2b(o.b); return *this; }
  inline rgb& operator= (uint32_t v) { setUInt32(v); return *this; }
  inline rgb& operator= (uint16_t v) { setUInt16(v); return *this; }

  inline operator uint8_t*()              { return c; }
  inline operator const uint8_t *() const { return c; }
  inline unsigned char & operator[](const int i) { return c[i]; }

  // funcs

  inline rgb &setUInt32(uint32_t v) { r= (uint8_t)(v>> 16),
                                      g= (uint8_t)((uint16_t)v>> 8),
                                      b= (uint8_t)v; return *this; }

  inline uint32_t getUInt32() { return ((uint32_t)255<< 24)+ ((uint32_t)r<< 16)+ ((uint32_t)g<< 8)+ (uint32_t)b; }

  inline rgb &setUInt16(uint16_t value) { r= ((value>> 8)>> 3)<< 3,
                                          g= (value>> 5)<< 2,
                                          b= (uint16_t) (((uint16_t)value<< 11)>> 11) << 3; return *this; }

  inline uint16_t getUInt16() { return (uint16_t)((b>> 3)+ ((g>> 2)<< 5)+ ((r>> 3)<< 0xb)); }

  inline rgb &set(uint8_t _r, uint8_t _g, uint8_t _b) { r= _r, g= _g, b= _b; return *this; }
};


inline rgba::rgba(const rgb &o): r(o.r), g(o.g), b(o.b), a(255) {}
inline rgba &rgba::operator=(const rgb &o) { r= o.r, g= o.g, b= o.b, a= 255; return *this; }









