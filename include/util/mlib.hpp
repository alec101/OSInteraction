#pragma once
#define MLIB_INCLUDED 1
#include <math.h>

// http://stackoverflow.com/questions/11228855/header-files-for-simd-intrinsics
//#include <mmintrin.h>  // MMX 1996
//#include <xmmintrin.h> // SSE 1999
//#include <emmintrin.h> // SSE2 2001
//#include <pmmintrin.h> // SSE3 2004  - _mm_hadd_ps (add between a __m128)
//#include <tmmintrin.h> // SSSE3 2006
//#include <smmintrin.h> // SSE4.1 2006  - _mm_dp_sp (dot product)
//#include <nmmintrin.h> // SSE4.2 2006
//#include <ammintrin.h> // SSE4A 2006
//#include <wmmintrin.h> // AES 2008
//#include <immintrin.h> // AVX 2008

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// sse seems to be a waste of time, imho - tests run, way slower. You need to be running some parralel stuff, bla-bla, neah. not working
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// _MM_SHUFFLE macro:
// shuffle(3,2,1,0) on same m128 will do nothing
// shuffle(0,1,2,3) on same m128 will invert all the floats
// therefore: the first var in _MM_SHUFFLE will be placed last (or first on the hiest byte order)
// the first two vars are from m128_1, the last two from m128_2 ??? this is never trully explained (from what positions, what order what way etc)
// m1(abcd) m2(efgh)   m3= _mm_shuffle_ps(m1, m2, __MMSHUFFLE(1,0,3,2)) -> m3(ghab)
//    0123 ?   0123 ? is this the notation? HOPEFULLY YEA!

/* TODO:
  - if faceforward will be used, intrinsics are not made for it
  - glsl has swizzling: vec3 thirdVec = otherVec.zyy; or thirdVec= otherVec.zyw
      _maybe_ something like this can be done, but only if it's very fast, will think about it

      https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
Reciprocal of the square root[edit]
Main article: Fast inverse square root
A variant of the above routine is included below, which can be used to compute the reciprocal of the square root, i.e., {\displaystyle x^{-{1 \over 2}}} x^{-{1 \over 2}} instead, was written by Greg Walsh. The integer-shift approximation produced a relative error of less than 4%, and the error dropped further to 0.15% with one iteration of Newton's method on the following line.[13] In computer graphics it is a very efficient way to normalize a vector.
float invSqrt(float x)
{
    float xhalf = 0.5f*x;
    union
    {
        float x;
        int i;
    } u;
    u.x = x;
    u.i = 0x5f3759df - (u.i >> 1);
    // The next line can be repeated any number of times to increase accuracy
    u.x = u.x * (1.5f - xhalf * u.x * u.x);
    return u.x;
}


http://stackoverflow.com/questions/19611198/finding-square-root-without-using-sqrt-function

sin - it could be done too
http://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b

http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648



IT SEEMS sqrt CAN BE FOUND, AND THERE SHOULD BE DOUBLE AND FLOAT VARIANTS, I NEED TO DO TESTS FOR VARIOUS NUMBERS, BIG, SMALL, AND SEE THE SPEEDS WITH STD



*/


#include "util/typeShortcuts.h"

// #define MLIB_USE_INTRINSICS 1


namespace mlib {

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif

// multiply with deg2rad or rad2deg to convert
#define DEG2RAD 0.01745329251994329576923690768489f // (PI/ 180.f)
#define RAD2DEG 57.295779513082320876798154814105f  // (180.f/ PI)
inline float degrees(float radians) { return radians* RAD2DEG; }
inline float radians(float degrees) { return degrees* DEG2RAD; }


inline int64_t abs64(int64_t n) { return (n< 0? -n: n); }
inline int32 abs32(int32 n) { return (n< 0? -n: n); }
inline double absd(double n) { return (n< 0? -n: n); }
inline float absf(float n) { return (n< 0? -n: n); }

//#define cInt(x) ((x)> 0? (((x)- int(x)< 0.5)? int(x): int(x)+ 1) : ((int(x)- (x)< 0.5)? int(x): int(x)- 1))
inline int32 roundf(float f)  { return (int32)(f> 0.0f? (f+ 0.5f): f- 0.5f); }
inline int64 roundd(double d) { return (int64)(d> 0.0?  (d+ 0.5):  d- 0.5); }
inline int32 ceilf(float f)  { return (f> 0.0f? int32(f)+ 1: int32(f)- 1); }
inline int64 ceild(double d) { return (d> 0.0?  int64(d)+ 1: int64(d)- 1); }


#ifndef max
#define max(a, b)   (((a)< (b))? (b): (a))
#endif
#ifndef min
#define min(a, b)   (((a)< (b))? (a): (b))
#endif



struct vec2;
struct vec3;
struct vec4;
struct mat4;

///=====================///
// basic funcs --------- //
///=====================///


// Not a Number check - NAN
inline bool isNAN(float n) { return ((n!= n)? true: false); }
inline bool isNANv(const float *n, int components) { for(int a= components; a> 0; a--, n++) if((*n)!= (*n)) return true; return false; }
bool isNAN(const vec2 &o);
bool isNAN(const vec3 &o);
bool isNAN(const vec4 &o);
bool isNAN(const mat4 &o);

float length(const vec2 &o);
float length(const vec3 &o);
float length(const vec4 &o);
float distance(const vec2 &v1, const vec2 &v2);
float distance(const vec3 &v1, const vec3 &v2);
float distance(const vec4 &v1, const vec4 &v2);
const vec2 &normalize(vec2 *inout_v);
const vec3 &normalize(vec3 *inout_v);
const vec4 &normalize(vec4 *inout_v);
float dot(const vec2 &v1, const vec2 &v2);
float dot(const vec3 &v1, const vec3 &v2);
float dot(const vec4 &v1, const vec4 &v2);
const vec3 cross(const vec3 &v1, const vec3 &v2);
const vec3 faceForward(const vec3 *v1, const vec3 &v2, const vec3 &v3); // <<<< NO SSE
// reflect & refract checkout further <<<
/*
vec3 refract(vec3 &v1, vec3 &v2, float eta) {
  vec3 d(dot(v2, v1));
  vec3 k(1.0f- eta* eta* (1- d* d));
  return (eta* v1- (eta* d+ sqrtf(k))* v2)* (k>= 0));
}
*/

#ifdef MLIB_USE_INTRINSICS
const __m128 dotSSE(const __m128 &v1, const __m128 &v2);
const __m128 lengthSSE(const __m128 &v);
const __m128 distanceSSE(const __m128 &v1, const __m128 &v2);
const __m128 crossSSE(const __m128 &v1, const __m128 &v2);
const __m128 normalizeSSE(const __m128 &v);
const __m128 norm2SSE(const __m128 &v);
#endif





#ifdef MLIB_USE_INTRINSICS
struct ALIGNED(16) vec2 {
#else
struct ALIGNED(4) vec2 {
#endif
  union {
    struct { float x, y; };
    struct { float r, g; };
    #ifdef MLIB_USE_INTRINSICS
    float v[4];               /// allignment for intrinsics needs 16 bytes anyways this can be 4
    #else
    float v[2];
    #endif
  };
  int getComponents() { return 2; } /// xy

  // constructors

  vec2();
  vec2(const vec2 &o);
  vec2(const vec3 &o);
  vec2(const vec4 &o);
  vec2(float _x, float _y);
  vec2(const float *n);
  vec2(float n);
  #ifdef MLIB_USE_INTRINSICS
  vec2(__m128 n);
  #endif

  // operators

  vec2 &operator=(float scalar);
  vec2 &operator=(const vec2 &v2);
  vec2 &operator=(const vec3 &v2);
  vec2 &operator=(const vec4 &v2);
  vec2 &operator=(const float *arr);       /// array with 2 floats
  #ifdef MLIB_USE_INTRINSICS
  vec2 &operator=(const __m128 &n);
  #endif

  vec2 &operator+=(float scalar);
  vec2 &operator+=(const vec2 &v2);
  vec2 &operator+=(const vec3 &v2);
  vec2 &operator+=(const vec4 &v2);
  vec2 &operator+=(const float *arr);      /// array with 2 floats

  vec2 &operator-=(float scalar);
  vec2 &operator-=(const vec2 &v2);
  vec2 &operator-=(const vec3 &v2);
  vec2 &operator-=(const vec4 &v2);
  vec2 &operator-=(const float *arr);      /// array with 2 floats

  vec2 &operator*=(float scalar);
  vec2 &operator*=(const vec2 &v2);
  vec2 &operator*=(const vec3 &v2);
  vec2 &operator*=(const vec4 &v2);
  vec2 &operator*=(const float *arr);      /// array with 2 floats

  vec2 &operator/=(float scalar);
  vec2 &operator/=(const vec2 &v2);
  vec2 &operator/=(const vec3 &v2);
  vec2 &operator/=(const vec4 &v2);
  vec2 &operator/=(const float *arr);      /// array with 2 floats

  const vec2 operator+(float scalar)   const;
  const vec2 operator+(const vec2 &v2) const;
  const vec2 operator+(const vec3 &v2) const;
  const vec2 operator+(const vec4 &v2) const;
  const vec2 operator+(const float *arr2) const;

  const vec2 operator-(float scalar)  const;
  const vec2 operator-(const vec2 &o) const;
  const vec2 operator-(const vec3 &o) const;
  const vec2 operator-(const vec4 &o) const;
  const vec2 operator-(const float *n) const;

  const vec2 operator*(float scalar)  const;
  const vec2 operator*(const vec2 &o) const;
  const vec2 operator*(const vec3 &o) const;
  const vec2 operator*(const vec4 &o) const;
  const vec2 operator*(const float *n) const;

  const vec2 operator/(float scalar)  const;
  const vec2 operator/(const vec2 &o) const;
  const vec2 operator/(const vec3 &o) const;
  const vec2 operator/(const vec4 &o) const;
  const vec2 operator/(const float *n) const;

  bool operator==(float n)       const;    // this compares vector's length, not each element to n
  bool operator==(const vec2 &o) const;
  bool operator==(const vec3 &o) const;
  bool operator==(const vec4 &o) const;
  bool operator==(const float *n) const;

  bool operator!=(float n)       const;    // this compares vector's length, not each element to n
  bool operator!=(const vec2 &o) const;
  bool operator!=(const vec3 &o) const;
  bool operator!=(const vec4 &o) const;
  bool operator!=(const float *n) const;
  

  bool operator!() const;

  const vec2 operator-() const;

  float &operator[](int i);
  const float operator[](int i) const;
  operator float* ();
  operator const float *() const;
  #ifdef MLIB_USE_INTRINSICS
  operator const __m128() const;
  #endif

  // dot product

  float dot(const vec2 &o)      const;
  float dot(const vec3 &o)      const;
  float dot(const vec4 &o)      const;
  float dot(float _x, float _y) const;
  float dot(const float *n)     const;

  // rest of funcs

  vec2 &normalize();
  float length() const;
  float norm2();
};

const vec2 operator+(float scalar, const vec2 &v);
const vec2 operator-(float scalar, const vec2 &v);
const vec2 operator*(float scalar, const vec2 &v);
const vec2 operator/(float scalar, const vec2 &v);




#ifdef MLIB_USE_INTRINSICS
struct ALIGNED(16) vec3 {    /// aligned(16) if intrinsics are used
#else
struct ALIGNED(4) vec3 {
#endif
  union {
    struct { float x, y, z; };
    struct { float r, g, b; };
    #ifdef MLIB_USE_INTRINSICS
    float v[4];
    #else
    float v[3];
    #endif
  };
  int getComponents() { return 3; } /// xyz

  // constructors

  vec3();
  vec3(const vec2 &o, float _z= 0);
  vec3(const vec3 &o);
  vec3(const vec4 &o);
  vec3(float _x, float _y, float _z);
  vec3(const float *n);
  vec3(float n);
  #ifdef MLIB_USE_INTRINSICS
  vec3(const __m128 &n);
  #endif

  // operators

  vec3 &operator=(float n);
  vec3 &operator=(const vec2 &o);
  vec3 &operator=(const vec3 &o);
  vec3 &operator=(const vec4 &o);
  vec3 &operator=(const float *n);
  #ifdef MLIB_USE_INTRINSICS
  vec3 &operator=(const __m128 &n);
  #endif

  vec3 &operator+=(float scalar);
  vec3 &operator+=(const vec2 &v2);
  vec3 &operator+=(const vec3 &v2);
  vec3 &operator+=(const vec4 &v2);
  vec3 &operator+=(const float *arr);

  vec3 &operator-=(float scalar);
  vec3 &operator-=(const vec2 &v2);
  vec3 &operator-=(const vec3 &v2);
  vec3 &operator-=(const vec4 &v2);
  vec3 &operator-=(const float *arr);

  vec3 &operator*=(float scalar);
  vec3 &operator*=(const vec2 &v2);
  vec3 &operator*=(const vec3 &v2);
  vec3 &operator*=(const vec4 &v2);
  vec3 &operator*=(const float *arr);

  vec3 &operator/=(float scalar);
  vec3 &operator/=(const vec2 &v2);
  vec3 &operator/=(const vec3 &v2);
  vec3 &operator/=(const vec4 &v2);
  vec3 &operator/=(const float *arr);

  const vec3 operator+(float scalar)   const;
  const vec3 operator+(const vec2 &v2) const;
  const vec3 operator+(const vec3 &v2) const;
  const vec3 operator+(const vec4 &v2) const;
  const vec3 operator+(const float *arr) const;

  const vec3 operator-(float scalar)   const;
  const vec3 operator-(const vec2 &v2) const;
  const vec3 operator-(const vec3 &v2) const;
  const vec3 operator-(const vec4 &v2) const;
  const vec3 operator-(const float *arr) const;

  const vec3 operator*(float scalar)   const;
  const vec3 operator*(const vec2 &v2) const;
  const vec3 operator*(const vec3 &v2) const;
  const vec3 operator*(const vec4 &v2) const;
  const vec3 operator*(const float *arr) const;

  const vec3 operator/(float scalar)   const;
  const vec3 operator/(const vec2 &v2) const;
  const vec3 operator/(const vec3 &v2) const;
  const vec3 operator/(const vec4 &v2) const;
  const vec3 operator/(const float *arr) const;

  bool operator==(float n)       const; // this compares vector's length, not each element with n
  bool operator==(const vec2 &o) const;
  bool operator==(const vec3 &o) const;
  bool operator==(const vec4 &o) const;
  bool operator==(const float *n) const;

  bool operator!=(float n)       const; // this compares vector's length, not each element with n
  bool operator!=(const vec2 &o) const;
  bool operator!=(const vec3 &o) const;
  bool operator!=(const vec4 &o) const;
  bool operator!=(const float *n) const;
  

  bool operator!() const;

  const vec3 operator-() const;

  float &operator[](int i);
  const float operator[](int i) const;
  operator float* ();
  operator const float *() const;
  #ifdef MLIB_USE_INTRINSICS
  operator const __m128() const;
  #endif

  // dot product

  float dot(const vec2 &o, float _z= 0.0f) const;
  float dot(const vec3 &o)                 const;
  float dot(const vec4 &o)                 const;
  float dot(float _x, float _y, float _z)  const;
  float dot(const float *n)                     const;

  // cross product

  vec3 cross(const vec2 &o, float _z)      const;
  vec3 cross(const vec3 &o)                const;
  vec3 cross(const vec4 &o)                const;
  vec3 cross(float _x, float _y, float _z) const;

  // funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl

  vec3 &normalize();
  float length() const;
  float norm2() const;
  void addConstant(float n);
};

const vec3 operator+(float scalar, const vec3 &v);
const vec3 operator-(float scalar, const vec3 &v);
const vec3 operator*(float scalar, const vec3 &v);
const vec3 operator/(float scalar, const vec3 &v);



struct ALIGNED(4) vec3i {
  union {
    struct { int32 x, y, z; };
    struct { int32 r, g, b; };
    int32 v[3];
  };
  int getComponents() { return 3; } /// xyz

  // constructors

  vec3i();
  //vec3i(const vec2i &o, int32 _z= 0);
  vec3i(const vec3i &o);
  //vec3i(const vec4i &o);
  vec3i(int32 _x, int32 _y, int32 _z);
  vec3i(const int32 *n);
  vec3i(int32 n);

  // operators

  vec3i &operator=(int32 n);
  //vec3i &operator=(const vec2i &o);
  vec3i &operator=(const vec3i &o);
  //vec3i &operator=(const vec4i &o);
  vec3i &operator=(const int32 *n);

  inline vec3i &set(int32 in_x, int32 in_y, int32 in_z) { x= in_x, y= in_y, z= in_z; return *this; }


  vec3i &operator+=(int32 scalar);
  //vec3i &operator+=(const vec2i &v2);
  vec3i &operator+=(const vec3i &v2);
  //vec3i &operator+=(const vec4i &v2);
  vec3i &operator+=(const int32 *arr);

  vec3i &operator-=(int32 scalar);
  //vec3i &operator-=(const vec2i &v2);
  vec3i &operator-=(const vec3i &v2);
  //vec3i &operator-=(const vec4i &v2);
  vec3i &operator-=(const int32 *arr);

  vec3i &operator*=(int32 scalar);
  //vec3i &operator*=(const vec2i &v2);
  vec3i &operator*=(const vec3i &v2);
  //vec3i &operator*=(const vec4i &v2);
  vec3i &operator*=(const int32 *arr);

  vec3i &operator/=(int32 scalar);
  //vec3i &operator/=(const vec2i &v2);
  vec3i &operator/=(const vec3i &v2);
  //vec3i &operator/=(const vec4i &v2);
  vec3i &operator/=(const int32 *arr);

  const vec3i operator+(int32 scalar)   const;
  //const vec3i operator+(const vec2i &v2) const;
  const vec3i operator+(const vec3i &v2) const;
  //const vec3i operator+(const vec4i &v2) const;
  const vec3i operator+(const int32 *arr) const;

  const vec3i operator-(int32 scalar)   const;
  //const vec3i operator-(const vec2i &v2) const;
  const vec3i operator-(const vec3i &v2) const;
  //const vec3i operator-(const vec4i &v2) const;
  const vec3i operator-(const int32 *arr) const;

  const vec3i operator*(int32 scalar)   const;
  //const vec3i operator*(const vec2i &v2) const;
  const vec3i operator*(const vec3i &v2) const;
  //const vec3i operator*(const vec4i &v2) const;
  const vec3i operator*(const int32 *arr) const;

  const vec3i operator/(int32 scalar)   const;
  //const vec3i operator/(const vec2i &v2) const;
  const vec3i operator/(const vec3i &v2) const;
  //const vec3i operator/(const vec4i &v2) const;
  const vec3i operator/(const int32 *arr) const;

  bool operator==(int32 n)       const; // this compares vector's length, not each element with n
  //bool operator==(const vec2i &o) const;
  bool operator==(const vec3i &o) const;
  //bool operator==(const vec4i &o) const;
  bool operator==(const int32 *n) const;

  bool operator!=(int32 n)        const; // this compares vector's length, not each element with n
  //bool operator!=(const vec2i &o)   const;
  bool operator!=(const vec3i &o)   const;
  //bool operator!=(const vec4i &o)   const;
  bool operator!=(const int32 *n) const;
  

  bool operator!() const;

  const vec3i operator-() const;

  int32 &operator[](int i);
  const int32 operator[](int i) const;
  operator int32* ();
  operator const int32 *() const;

  // dot product

  //int32 dot(const vec2i &o, int32 _z= 0.0f)   const;
  int32 dot(const vec3i &o)                     const;
  //int32 dot(const vec4i &o)                     const;
  int32 dot(int32 _x, int32 _y, int32 _z) const;
  int32 dot(const int32 *n)                   const;

  // cross product

  //vec3i cross(const vec2i &o, int32 _z)         const;
  vec3i cross(const vec3i &o)                     const;
  //vec3i cross(const vec4i &o)                     const;
  vec3i cross(int32 _x, int32 _y, int32 _z) const;

  // funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl

  vec3i &normalize();
  int32 length() const;
  int32 norm2() const;
  void addConstant(int32 n);
};

const vec3i operator+(int32 scalar, const vec3i &v);
const vec3i operator-(int32 scalar, const vec3i &v);
const vec3i operator*(int32 scalar, const vec3i &v);
const vec3i operator/(int32 scalar, const vec3i &v);



#ifdef MLIB_USE_INTRINSICS
struct ALIGNED(16) vec4 {    // alignment of 16 if in the future intrinsics will be used
#else
struct ALIGNED(4) vec4 {    // alignment of 16 if in the future intrinsics will be used
#endif
  union {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    float v[4];
  };
  int getComponents() { return 4; } /// xyzw

  // constructors
  
  vec4();
  vec4(float _x, float _y, float _z, float _w);
  vec4(float n);
  vec4(const vec2 &o, float _z= 0, float _w= 0);
  vec4(const vec3 &o, float _w= 0);
  vec4(const vec4 &o);
  vec4(const float *n);
  #ifdef MLIB_USE_INTRINSICS
  vec4(__m128 n);
  #endif

  // operators

  vec4 &operator=(float n);
  vec4 &operator=(const vec2 &o);
  vec4 &operator=(const vec3 &o);
  vec4 &operator=(const vec4 &o);
  vec4 &operator=(const float *n);
  #ifdef MLIB_USE_INTRINSICS
  vec4 &operator= (__m128 n);
  #endif

  vec4 &operator+=(float n);
  vec4 &operator+=(const vec2 &o);
  vec4 &operator+=(const vec3 &o);
  vec4 &operator+=(const vec4 &o);
  vec4 &operator+=(const float *n);

  vec4 &operator-=(float n);
  vec4 &operator-=(const vec2 &o);
  vec4 &operator-=(const vec3 &o);
  vec4 &operator-=(const vec4 &o);
  vec4 &operator-=(const float *n);

  vec4 &operator*=(float n);
  vec4 &operator*=(const vec2 &o);
  vec4 &operator*=(const vec3 &o);
  vec4 &operator*=(const vec4 &o);
  vec4 &operator*=(const float *n);

  vec4 &operator/=(float n);
  vec4 &operator/=(const vec2 &o);
  vec4 &operator/=(const vec3 &o);
  vec4 &operator/=(const vec4 &o);
  vec4 &operator/=(const float *n);
  
  const vec4 operator+(float n)       const;
  const vec4 operator+(const vec2 &o) const;
  const vec4 operator+(const vec3 &o) const;
  const vec4 operator+(const vec4 &o) const;
  const vec4 operator+(const float *n)     const;

  const vec4 operator-(float n)       const;
  const vec4 operator-(const vec2 &o) const;
  const vec4 operator-(const vec3 &o) const;
  const vec4 operator-(const vec4 &o) const;
  const vec4 operator-(const float *n)     const;

  const vec4 operator*(float n)       const;
  const vec4 operator*(const vec2 &o) const;
  const vec4 operator*(const vec3 &o) const;
  const vec4 operator*(const vec4 &o) const;
  const vec4 operator*(const float *n)     const;

  const vec4 operator/(float n)       const;
  const vec4 operator/(const vec2 &o) const;
  const vec4 operator/(const vec3 &o) const;
  const vec4 operator/(const vec4 &o) const;
  const vec4 operator/(const float *n)     const;

  bool operator==(float n)       const;
  bool operator==(const vec2 &o) const;
  bool operator==(const vec3 &o) const;
  bool operator==(const vec4 &o) const;
  bool operator==(const float *n)     const;

  bool operator!=(const vec2 &o) const;
  bool operator!=(const vec3 &o) const;
  bool operator!=(const vec4 &o) const;
  bool operator!=(const float *n)     const;
  bool operator!=(float n)       const;

  bool operator!() const;

  const vec4 operator-() const;

  float &operator[](int i);
  const float operator[](int i) const;
  operator float* ();
  operator const float *() const;
  #ifdef MLIB_USE_INTRINSICS
  operator const __m128() const;
  #endif

  // dot product

  float dot(const vec2 &o, float _z= 0.0f, float _w= 0.0f) const;
  float dot(const vec3 &o, float _w= 0.0f)                 const;
  float dot(const vec4 &o)                                 const;
  float dot(float _x, float _y, float _z, float _w)        const;
  float dot(const float *n)                                const;

  // funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl

  vec4 &set(float, float, float, float);
  vec4 &normalize();
  float length() const;
  float norm2() const;
};

const vec4 operator+(float scalar, const vec4 &v);
const vec4 operator-(float scalar, const vec4 &v);
const vec4 operator*(float scalar, const vec4 &v);
const vec4 operator/(float scalar, const vec4 &v);




#ifdef MLIB_USE_INTRINSICS
struct ALIGNED(16) mat4 {
#else
struct ALIGNED(4) mat4 {
#endif
  float v[16];

  // constructors

  mat4();
  mat4(const mat4 &o);
  mat4(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33);
  mat4(float *o);

  // various matrix types

  mat4 &identity();
  mat4 &zero();

  // rotation functions

  // see http://www.khronos.org/opengles/documentation/html/glRotate.html if there are any bugs :)

  mat4 &rotate(float angle, const vec3 &axis);
  mat4 &rotate(float angle, float x, float y, float z);
  mat4 &rotateX(float angle);     /// uses fewer computations than rotate
  mat4 &rotateY(float angle);     /// uses fewer computations than rotate
  mat4 &rotateZ(float angle);     /// uses fewer computations than rotate

  mat4 &translate(const vec3 &o);
  mat4 &translate(float x,float y, float z);
  
  mat4 &scale(const vec3 &o);
  mat4 &scale(float x,float y,float z);

  // operators

  operator float *();
  operator const float *() const;
  //const float operator[](int i) const { return v[i]; } // might not be needed (further thinking needed)
  //float &operator[](int i) { return v[i]; }            // might not be needed (further thinking needed)

  mat4 operator+(const mat4 &o);
  mat4 operator+(float n);
  mat4 operator-(const mat4 &o);
  mat4 operator-(float n);
  mat4 operator*(const mat4 &o);

  // assume the vectors are padded with a 1 (homogenous coordinates)
  vec2 operator*(const vec2 &o);
  vec3 operator*(const vec3 &o);
  vec4 operator*(const vec4 &o);

  mat4& operator*=(const mat4 &o);
  mat4& operator+=(const mat4 &o);
  mat4& operator-=(const mat4 &o);

  // funcs

  mat4 &ortho(float left, float right, float bottom, float top, float znear, float zfar);
  mat4 &frustrum(float left, float right, float bottom, float top, float znear, float zfar);
  mat4 &perspective(float fovy, float aspect, float znear, float zfar);
  mat4 &lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);

  // vector operations

  

  // matrix operations


};
//typedef mat4 mat4;








///============///
// DECLARATIONS //
///============///

inline bool isNAN(const vec2 &o) { return isNANv(o.v, 2); }
inline bool isNAN(const vec3 &o) { return isNANv(o.v, 3); }
inline bool isNAN(const vec4 &o) { return isNANv(o.v, 4); }
inline bool isNAN(const mat4 &o) { return isNANv(o.v, 16); }


/*
#ifdef MLIB_USE_INTRINSICS
float length(const vec2 &o) { float ret; _mm_store_ss(&ret, lengthSSE(o)); return ret; } 
float length(const vec3 &o) { float ret; _mm_store_ss(&ret, lengthSSE(o)); return ret; } 
float length(const vec4 &o) { float ret; _mm_store_ss(&ret, lengthSSE(o)); return ret; } 
float distance(const vec2 &v1, const vec2 &v2) { float ret; _mm_store_ss(&ret, distanceSSE(v1, v2)); return ret; }
float distance(const vec3 &v1, const vec3 &v2) { float ret; _mm_store_ss(&ret, distanceSSE(v1, v2)); return ret; }
float distance(const vec4 &v1, const vec4 &v2) { float ret; _mm_store_ss(&ret, distanceSSE(v1, v2)); return ret; }
const vec2 &normalize(vec2 *inout_v) { *inout_v= normalizeSSE(*inout_v); return *inout_v; }
const vec3 &normalize(vec3 *inout_v) { *inout_v= normalizeSSE(*inout_v); return *inout_v; }
const vec4 &normalize(vec4 *inout_v) { *inout_v= normalizeSSE(*inout_v); return *inout_v; }
float dot(const vec2 &v1, const vec2 &v2) { float ret; _mm_store_ss(&ret, dotSSE(v1, v2)); return ret; }
float dot(const vec3 &v1, const vec3 &v2) { float ret; _mm_store_ss(&ret, dotSSE(v1, v2)); return ret; }
float dot(const vec4 &v1, const vec4 &v2) { float ret; _mm_store_ss(&ret, dotSSE(v1, v2)); return ret; }
const vec3 cross(const vec3 &v1, const vec3 &v2) { return vec3(crossSSE(v1, v2)); }
const vec3 faceForward(const vec3 *v1, const vec3 &v2, const vec3 &v3) { return dot(v3, v2)< 0.0f? *v1: -(*v1); }  // <<<< NO SSE
#else
*/
inline float length(const vec2 &o) { return sqrtf(o.x* o.x+ o.y* o.y); }
inline float length(const vec3 &o) { return sqrtf(o.x* o.x+ o.y* o.y+ o.z* o.z); }
inline float length(const vec4 &o) { return sqrtf(o.x* o.x+ o.y* o.y+ o.z* o.z+ o.w* o.w); }
inline float distance(const vec2 &v1, const vec2 &v2) { return length(v2- v1); }
inline float distance(const vec3 &v1, const vec3 &v2) { return length(v2- v1); }
inline float distance(const vec4 &v1, const vec4 &v2) { return length(v2- v1); }
inline const vec2 &normalize(vec2 *out_v) { return out_v->normalize(); }
inline const vec3 &normalize(vec3 *out_v) { return out_v->normalize(); }
inline const vec4 &normalize(vec4 *out_v) { return out_v->normalize(); }
inline float dot(const vec2 &v1, const vec2 &v2) { return v1.x* v2.x+ v1.y* v2.y; }
inline float dot(const vec3 &v1, const vec3 &v2) { return v1.x* v2.x+ v1.y* v2.y+ v1.z* v2.z; }
inline float dot(const vec4 &v1, const vec4 &v2) { return v1.x* v2.x+ v1.y* v2.y+ v1.z* v2.z+ v1.w* v2.w; }
inline const vec3 cross(const vec3 &v1, const vec3 &v2) { return vec3(v1.y* v2.z- v1.z* v2.y, v1.z* v2.x- v1.x* v2.z, v1.x* v2.y- v1.y* v2.x); }
inline const vec3 faceForward(vec3 *out_v1, const vec3 &v2, const vec3 &v3) { return dot(v3, v2)< 0.0f? *out_v1: -(*out_v1); }  // <<<< NO SSE
//#endif


#ifdef MLIB_USE_INTRINSICS
inline const __m128 dotSSE(const __m128 &v1, const __m128 &v2) {
  /* method 1 - shuffles - SSE1
  __m128 reg1= _mm_mul_ps(v1, v2);                                  // reg1(WZYX)
  __m128 reg2= _mm_shuffle_ps(reg1, reg1, _MM_SHUFFLE(2, 1, 0, 3)); // reg2(XWZY)
  __m128 reg3= _mm_add_ps(reg1, reg2);                              // reg3(W+X, Z+W, Y+Z, X+Y)
  __m128 reg2= _mm_shuffle_ps(reg3, reg3, _MM_SHUFFLE(1, 3, 2, 0)); // reg2(bla, bla, bla, Z+W)
  return _mm_add_ss(reg3, reg2);                                    //  ret(bla, bla, bla, X+Y+Z+W)
  */
  // _MM_SHUFFLE macro:
  // shuffle(3,2,1,0) on same m128 will do nothing
  // shuffle(0,1,2,3) on same m128 will invert all the floats
  // therefore: the first var in _MM_SHUFFLE will be placed last (or first on the hiest byte order)
  // the first two vars are from m128_1, the last two from m128_2 ??? this is never trully explained (from what positions, what order what way etc)
  // m1(abcd) m2(efgh)   m3= _mm_shuffle_ps(m1, m2, __MMSHUFFLE(1,0,3,2)) -> m3(ghab)
  //    0123 ?   0123 ? is this the notation? HOPEFULLY YEA!

  // method 1.1:
  // v1(ABCD)+ v2(BADC) = add(A+B,     B+A,     C+D,     D+C)
  // v2 swap(0123)=        v2(D+C,     C+D,     B+A,     A+B)
  //                      add(A+B+D+C, B+A+C+D, C+D+B+A, D+C+A+B)

  // method 2: - SSE3
  __m128 reg1= _mm_mul_ps(v1, v2);
  __m128 reg2= _mm_hadd_ps(reg1, reg1);
  return _mm_hadd_ps(reg2, reg2);
}

inline const __m128 lengthSSE(const __m128 &v) {
  return _mm_sqrt_ss(dotSSE(v, v));
}

inline const __m128 distanceSSE(const __m128 &v1, const __m128 &v2) {
  return lengthSSE(_mm_sub_ps(v2, v1));
}

inline const __m128 crossSSE(const __m128 &v1, const __m128 &v2) {
                                                                // 0 1 2 3
                                                                // W Z Y X
                                                                // v1(WZYX), v2(WZYX)
  __m128 reg1= _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(2, 1, 3, 0)); // reg1(v1.WXZY)
  __m128 reg2= _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(1, 3, 2, 0)); // reg2(v2.WYXZ)
  __m128 reg3= _mm_mul_ps(reg1, reg2);
         reg1= _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(1, 3, 2, 0)); // reg1(v1.WYXZ)
         reg2= _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 1, 3, 0)); // reg2(v2.WXZY)
  __m128 reg4= _mm_mul_ps(reg1, reg2);
  return _mm_sub_ps(reg3, reg4);
}

inline const __m128 normalizeSSE(const __m128 &v) {
  __m128 size= lengthSSE(v);
  
  if(!(_mm_cmpeq_ss(size, _mm_setzero_ps()).m128_u8[0])) {
    size.m128_f32[3]= size.m128_f32[2]= size.m128_f32[1]= size.m128_f32[0];
    return _mm_div_ps(v, size);
    
  }
  return v;
}

inline const __m128 norm2SSE(const __m128 &v) {
  return dotSSE(v, v);
}
#endif





///=========================================///
// vec2 ================-------------------- //
///=========================================///

// constructors

inline vec2::vec2():                   x(0),    y(0)    {}
inline vec2::vec2(const vec2 &o):      x(o.x),  y(o.y)  {}
inline vec2::vec2(const vec3 &o):      x(o.x),  y(o.y)  {}
inline vec2::vec2(const vec4 &o):      x(o.x),  y(o.y)  {}
inline vec2::vec2(float _x, float _y): x(_x),   y(_y)   {}
inline vec2::vec2(const float *n):     x(n[0]), y(n[1]) {}
inline vec2::vec2(float n):            x(n),    y(n)    {}
#ifdef MLIB_USE_INTRINSICS
inline vec2::vec2(__m128 n) { *this= vec4(n); }
#endif

// operators

inline vec2 &vec2::operator=(float n)        { x= n;    y= n;    return *this; }
inline vec2 &vec2::operator=(const vec2 &o)  { x= o.x;  y= o.y;  return *this; }
inline vec2 &vec2::operator=(const vec3 &o)  { x= o.x;  y= o.y;  return *this; }
inline vec2 &vec2::operator=(const vec4 &o)  { x= o.x;  y= o.y;  return *this; }
inline vec2 &vec2::operator=(const float *n) { x= n[0]; y= n[1]; return *this; }
#ifdef MLIB_USE_INTRINSICS
inline vec2 &vec2::operator=(const __m128 &n) { return *this= vec4(n); }
#endif


inline vec2 &vec2::operator+=(float n)        { x+= n;    y+= n;    return *this; }
inline vec2 &vec2::operator+=(const vec2 &o)  { x+= o.x;  y+= o.y;  return *this; }
inline vec2 &vec2::operator+=(const vec3 &o)  { x+= o.x;  y+= o.y;  return *this; }
inline vec2 &vec2::operator+=(const vec4 &o)  { x+= o.x;  y+= o.y;  return *this; }
inline vec2 &vec2::operator+=(const float *n) { x+= n[0]; y+= n[1]; return *this; }

inline vec2 &vec2::operator-=(float n)        { x-= n;    y-= n;    return *this; }
inline vec2 &vec2::operator-=(const vec2 &o)  { x-= o.x;  y-= o.y;  return *this; }
inline vec2 &vec2::operator-=(const vec3 &o)  { x-= o.x;  y-= o.y;  return *this; }
inline vec2 &vec2::operator-=(const vec4 &o)  { x-= o.x;  y-= o.y;  return *this; }
inline vec2 &vec2::operator-=(const float *n) { x-= n[0]; y-= n[1]; return *this; }

inline vec2 &vec2::operator*=(float n)        { x*= n;    y*= n;    return *this; }
inline vec2 &vec2::operator*=(const vec2 &o)  { x*= o.x;  y*= o.y;  return *this; }
inline vec2 &vec2::operator*=(const vec3 &o)  { x*= o.x;  y*= o.y;  return *this; }
inline vec2 &vec2::operator*=(const vec4 &o)  { x*= o.x;  y*= o.y;  return *this; }
inline vec2 &vec2::operator*=(const float *n) { x*= n[0]; y*= n[1]; return *this; }

inline vec2 &vec2::operator/=(float n)        { x/= n;    y/= n;    return *this; }
inline vec2 &vec2::operator/=(const vec2 &o)  { x/= o.x;  y/= o.y;  return *this; }
inline vec2 &vec2::operator/=(const vec3 &o)  { x/= o.x;  y/= o.y;  return *this; }
inline vec2 &vec2::operator/=(const vec4 &o)  { x/= o.x;  y/= o.y;  return *this; }
inline vec2 &vec2::operator/=(const float *n) { x/= n[0]; y/= n[1]; return *this; }

inline const vec2 vec2::operator+(float n)        const { return vec2(*this)+= n; }
inline const vec2 vec2::operator+(const vec2 &o)  const { return vec2(*this)+= o; }
inline const vec2 vec2::operator+(const vec3 &o)  const { return vec2(*this)+= o; }
inline const vec2 vec2::operator+(const vec4 &o)  const { return vec2(*this)+= o; }
inline const vec2 vec2::operator+(const float *n) const { return vec2(*this)+= n; }

inline const vec2 vec2::operator-(float n)        const { return vec2(*this)-= n; }
inline const vec2 vec2::operator-(const vec2 &o)  const { return vec2(*this)-= o; }
inline const vec2 vec2::operator-(const vec3 &o)  const { return vec2(*this)-= o; }
inline const vec2 vec2::operator-(const vec4 &o)  const { return vec2(*this)-= o; }
inline const vec2 vec2::operator-(const float *n) const { return vec2(*this)-= n; }

inline const vec2 vec2::operator*(float n)        const { return vec2(*this)*= n; }
inline const vec2 vec2::operator*(const vec2 &o)  const { return vec2(*this)*= o; }
inline const vec2 vec2::operator*(const vec3 &o)  const { return vec2(*this)*= o; }
inline const vec2 vec2::operator*(const vec4 &o)  const { return vec2(*this)*= o; }
inline const vec2 vec2::operator*(const float *n) const { return vec2(*this)*= n; }

inline const vec2 vec2::operator/(float n)        const { return vec2(*this)/= n; }
inline const vec2 vec2::operator/(const vec2 &o)  const { return vec2(*this)/= o; }
inline const vec2 vec2::operator/(const vec3 &o)  const { return vec2(*this)/= o; }
inline const vec2 vec2::operator/(const vec4 &o)  const { return vec2(*this)/= o; }
inline const vec2 vec2::operator/(const float *n) const { return vec2(*this)/= n; }

inline const vec2 operator+(float scalar, const vec2 &v) { return vec2(scalar+ v.x, scalar+ v.y); }
inline const vec2 operator-(float scalar, const vec2 &v) { return vec2(scalar- v.x, scalar- v.y); }
inline const vec2 operator*(float scalar, const vec2 &v) { return vec2(scalar* v.x, scalar* v.y); }
inline const vec2 operator/(float scalar, const vec2 &v) { return vec2(scalar/ v.x, scalar/ v.y); }

inline bool vec2::operator==(float n)        const { return (n== length()); }
inline bool vec2::operator==(const vec2 &o)  const { return ((x== o.x)  && (y== o.y)); }
inline bool vec2::operator==(const vec3 &o)  const { return ((x== o.x)  && (y== o.y)); }
inline bool vec2::operator==(const vec4 &o)  const { return ((x== o.x)  && (y== o.y)); }
inline bool vec2::operator==(const float *n) const { return ((x== n[0]) && (y== n[1])); }

inline bool vec2::operator!=(float n)        const { return (n!= length()); }
inline bool vec2::operator!=(const vec2 &o)  const { return ((x!= o.x)  || (y!= o.y)); }
inline bool vec2::operator!=(const vec3 &o)  const { return ((x!= o.x)  || (y!= o.y)); }
inline bool vec2::operator!=(const vec4 &o)  const { return ((x!= o.x)  || (y!= o.y)); }
inline bool vec2::operator!=(const float *n) const { return ((x!= n[0]) || (y!= n[1])); }

inline bool vec2::operator!() const { return ((x || y)? false: true); }

inline const vec2 vec2::operator-() const { return vec2(-x, -y); }

inline float &vec2::operator[](int i)            { return v[i]; }
inline const float vec2::operator[](int i) const { return v[i]; }
inline vec2::operator float* ()             { return v; }
inline vec2::operator const float *() const { return v; }
#ifdef MLIB_USE_INTRINSICS
inline vec2::operator const __m128() const { return _mm_set_ps(0.0f, 0.0f, y, x); }
#endif

// dot product

inline float vec2::dot(const vec2 &o) const { return this->dot(o.x,  o.y); }
inline float vec2::dot(const vec3 &o) const { return this->dot(o.x,  o.y); }
inline float vec2::dot(const vec4 &o) const { return this->dot(o.x,  o.y); }
inline float vec2::dot(const float *n)     const { return this->dot(n[0], n[1]); }
inline float vec2::dot(float _x, float _y) const { return x* _x+    y* _y; }

// rest of funcs

inline vec2 &vec2::normalize() { float size= length(); if(size) { this->operator/=(size); } return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option

inline float vec2::length() const { return sqrtf(x* x+ y* y); }
inline float vec2::norm2() { return x* x + y* y; }




///=========================================///
// vec3 ================-------------------- //
///=========================================///

// constructors
inline vec3::vec3():                             x(0.0f), y(0.0f), z(0.0f) {}
inline vec3::vec3(const vec2 &o, float _z):      x(o.x),  y(o.y),  z(_z)   {}
inline vec3::vec3(const vec3 &o):                x(o.x),  y(o.y),  z(o.z)  {}
inline vec3::vec3(const vec4 &o):                x(o.x),  y(o.y),  z(o.z)  {}
inline vec3::vec3(float _x, float _y, float _z): x(_x),   y(_y),   z(_z)   {}
inline vec3::vec3(const float *n):               x(n[0]), y(n[1]), z(n[2]) {}
inline vec3::vec3(float n):                      x(n),    y(n),    z(n)    {}
#ifdef MLIB_USE_INTRINSICS
inline vec3::vec3(const __m128 &n) { *this= vec4(n); }
#endif

// operators

inline vec3 &vec3::operator=(float n)        { x= n;    y= n;    z= n;    return *this; }
inline vec3 &vec3::operator=(const vec2 &o)  { x= o.x;  y= o.y;           return *this; }
inline vec3 &vec3::operator=(const vec3 &o)  { x= o.x;  y= o.y;  z= o.z;  return *this; }
inline vec3 &vec3::operator=(const vec4 &o)  { x= o.x;  y= o.y;  z= o.z;  return *this; }
inline vec3 &vec3::operator=(const float *n) { x= n[0]; y= n[1]; z= n[2]; return *this; }
#ifdef MLIB_USE_INTRINSICS
inline vec3 &vec3::operator=(const __m128 &n) { return *this= vec4(n); }
#endif

inline vec3 &vec3::operator+=(float n)        { x+= n;    y+= n;    z+= n;    return *this; }
inline vec3 &vec3::operator+=(const vec2 &o)  { x+= o.x;  y+= o.y;            return *this; }
inline vec3 &vec3::operator+=(const vec3 &o)  { x+= o.x;  y+= o.y;  z+= o.z;  return *this; }
inline vec3 &vec3::operator+=(const vec4 &o)  { x+= o.x;  y+= o.y;  z+= o.z;  return *this; }
inline vec3 &vec3::operator+=(const float *n) { x+= n[0]; y+= n[1]; z+= n[2]; return *this; }

inline vec3 &vec3::operator-=(float n)        { x-= n;    y-= n;    z-= n;    return *this; }
inline vec3 &vec3::operator-=(const vec2 &o)  { x-= o.x;  y-= o.y;            return *this; }
inline vec3 &vec3::operator-=(const vec3 &o)  { x-= o.x;  y-= o.y;  z-= o.z;  return *this; }
inline vec3 &vec3::operator-=(const vec4 &o)  { x-= o.x;  y-= o.y;  z-= o.z;  return *this; }
inline vec3 &vec3::operator-=(const float *n) { x-= n[0]; y-= n[1]; z-= n[2]; return *this; }

inline vec3 &vec3::operator*=(float n)        { x*= n;    y*= n;    z*= n;    return *this; }
inline vec3 &vec3::operator*=(const vec2 &o)  { x*= o.x;  y*= o.y;            return *this; }
inline vec3 &vec3::operator*=(const vec3 &o)  { x*= o.x;  y*= o.y;  z*= o.z;  return *this; }
inline vec3 &vec3::operator*=(const vec4 &o)  { x*= o.x;  y*= o.y;  z*= o.z;  return *this; }
inline vec3 &vec3::operator*=(const float *n) { x*= n[0]; y*= n[1]; z*= n[2]; return *this; }

inline vec3 &vec3::operator/=(float n)        { x/= n;    y/= n;    z/= n;    return *this; }
inline vec3 &vec3::operator/=(const vec2 &o)  { x/= o.x;  y/= o.y;            return *this; }
inline vec3 &vec3::operator/=(const vec3 &o)  { x/= o.x;  y/= o.y;  z/= o.z;  return *this; }
inline vec3 &vec3::operator/=(const vec4 &o)  { x/= o.x;  y/= o.y;  z/= o.z;  return *this; }
inline vec3 &vec3::operator/=(const float *n) { x/= n[0]; y/= n[1]; z/= n[2]; return *this; }

inline const vec3 vec3::operator+(float n)        const { return vec3(*this)+= n; }
inline const vec3 vec3::operator+(const vec2 &o)  const { return vec3(*this)+= o; }
inline const vec3 vec3::operator+(const vec3 &o)  const { return vec3(*this)+= o; }
inline const vec3 vec3::operator+(const vec4 &o)  const { return vec3(*this)+= o; }
inline const vec3 vec3::operator+(const float *n) const { return vec3(*this)+= n; }

inline const vec3 vec3::operator-(float n)        const { return vec3(*this)-= n; }
inline const vec3 vec3::operator-(const vec2 &o)  const { return vec3(*this)-= o; }
inline const vec3 vec3::operator-(const vec3 &o)  const { return vec3(*this)-= o; }
inline const vec3 vec3::operator-(const vec4 &o)  const { return vec3(*this)-= o; }
inline const vec3 vec3::operator-(const float *n) const { return vec3(*this)-= n; }

inline const vec3 vec3::operator*(float n)        const { return vec3(*this)*= n; }
inline const vec3 vec3::operator*(const vec2 &o)  const { return vec3(*this)*= o; }
inline const vec3 vec3::operator*(const vec3 &o)  const { return vec3(*this)*= o; }
inline const vec3 vec3::operator*(const vec4 &o)  const { return vec3(*this)*= o; }
inline const vec3 vec3::operator*(const float *n) const { return vec3(*this)*= n; }

inline const vec3 vec3::operator/(float n)        const { return vec3(*this)/= n; }
inline const vec3 vec3::operator/(const vec2 &o)  const { return vec3(*this)/= o; }
inline const vec3 vec3::operator/(const vec3 &o)  const { return vec3(*this)/= o; }
inline const vec3 vec3::operator/(const vec4 &o)  const { return vec3(*this)/= o; }
inline const vec3 vec3::operator/(const float *n) const { return vec3(*this)/= n; }

inline const vec3 operator+(float scalar, const vec3 &v) { return vec3(scalar+ v.x, scalar+ v.y, scalar+ v.z); }
inline const vec3 operator-(float scalar, const vec3 &v) { return vec3(scalar- v.x, scalar- v.y, scalar- v.z); }
inline const vec3 operator*(float scalar, const vec3 &v) { return vec3(scalar* v.x, scalar* v.y, scalar* v.z); }
inline const vec3 operator/(float scalar, const vec3 &v) { return vec3(scalar/ v.x, scalar/ v.y, scalar/ v.z); }

inline bool vec3::operator==(float n)        const { return (n== length()); }
inline bool vec3::operator==(const vec2 &o)  const { return ((x== o.x)  && (y== o.y)); }
inline bool vec3::operator==(const vec3 &o)  const { return ((x== o.x)  && (y== o.y)  && (z== o.z)); }
inline bool vec3::operator==(const vec4 &o)  const { return ((x== o.x)  && (y== o.y)  && (z== o.z)); }
inline bool vec3::operator==(const float *n) const { return ((x== n[0]) && (y== n[1]) && (z== n[2])); }

inline bool vec3::operator!=(float n)        const { return (n!= length()); }
inline bool vec3::operator!=(const vec2 &o)  const { return ((x!= o.x)  || (y!= o.y)); }
inline bool vec3::operator!=(const vec3 &o)  const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)); }
inline bool vec3::operator!=(const vec4 &o)  const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)); }
inline bool vec3::operator!=(const float *n) const { return ((x!= n[0]) || (y!= n[1]) || (z!= n[2])); }

inline bool vec3::operator!() const { return ((x || y || z)? false: true); }

inline const vec3 vec3::operator-() const { return vec3(-x, -y, -z); }

inline float &vec3::operator[](int i)            { return v[i]; }
inline const float vec3::operator[](int i) const { return v[i]; }
inline vec3::operator float* ()             { return v; }
inline vec3::operator const float *() const { return v; }
#ifdef MLIB_USE_INTRINSICS
vec3::operator const __m128() const { return _mm_set_ps(0.0f, z, y, x); }
#endif

// dot product

inline float vec3::dot(const vec2 &o, float _z)      const { return x* o.x+   y* o.y+  z* _z; }
inline float vec3::dot(const vec3 &o)                const { return x* o.x+   y* o.y+  z* o.z; }
inline float vec3::dot(const vec4 &o)                const { return x* o.x+   y* o.y+  z* o.z; }
inline float vec3::dot(float _x, float _y, float _z) const { return x* _x+    y* _y+   z* _z; }
inline float vec3::dot(const float *n)               const { return x* n[0] + y* n[1]+ z* n[2]; }

// cross product

//vec3 vec3::cross(const vec3 &o)                       const { return vec3(crossSSE(*this, o)); }
inline vec3 vec3::cross(const vec2 &o, float _z)      const { return vec3(y* _z-  z* o.y, z* o.x- x* _z,  x* o.y- y* o.x); }
inline vec3 vec3::cross(const vec3 &o)                const { return vec3(y* o.z- z* o.y, z* o.x- x* o.z, x* o.y- y* o.x); }
inline vec3 vec3::cross(const vec4 &o)                const { return vec3(y* o.z- z* o.y, z* o.x- x* o.z, x* o.y- y* o.x); }
inline vec3 vec3::cross(float _x, float _y, float _z) const { return vec3(y* _z-  z* _y,  z* _x-  x* _z,  x* _y-  y* _x); }

// funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl

inline vec3 &vec3::normalize() { float size= length(); if(size) { this->operator/=(size); } return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option

inline float vec3::length() const { return sqrtf(x* x+ y* y+ z* z); };
inline float vec3::norm2() const { return x* x + y* y + z* z; }

inline void vec3::addConstant(float n) {
  float xx= x* x, yy= y* y, zz= z* z, size= length()+ n;
  if(x!= 0.0f) x= (fabsf(x)/ x)* size/ sqrtf(yy/ xx+ zz/ xx+ 1.0f);
  if(y!= 0.0f) y= (fabsf(y)/ y)* size/ sqrtf(xx/ yy+ zz/ yy+ 1.0f);
  if(z!= 0.0f) z= (fabsf(z)/ z)* size/ sqrtf(xx/ zz+ yy/ zz+ 1.0f);
};
  





///==========================================///
// vec3i ================-------------------- //
///==========================================///

// constructors
inline vec3i::vec3i():                             x(0), y(0), z(0) {}
//inline vec3i::vec3i(const vec2i &o, int32 _z):     x(o.x),  y(o.y),  z(_z)   {}
inline vec3i::vec3i(const vec3i &o):               x(o.x),  y(o.y),  z(o.z)  {}
//inline vec3i::vec3i(const vec4i &o):               x(o.x),  y(o.y),  z(o.z)  {}
inline vec3i::vec3i(int32 _x, int32 _y, int32 _z): x(_x),   y(_y),   z(_z)   {}
inline vec3i::vec3i(const int32 *n):               x(n[0]), y(n[1]), z(n[2]) {}
inline vec3i::vec3i(int32 n):                      x(n),    y(n),    z(n)    {}

// operators

inline vec3i &vec3i::operator=(int32 n)        { x= n;    y= n;    z= n;    return *this; }
//inline vec3i &vec3i::operator=(const vec2i &o) { x= o.x;  y= o.y;           return *this; }
inline vec3i &vec3i::operator=(const vec3i &o) { x= o.x;  y= o.y;  z= o.z;  return *this; }
//inline vec3i &vec3i::operator=(const vec4i &o) { x= o.x;  y= o.y;  z= o.z;  return *this; }
inline vec3i &vec3i::operator=(const int32 *n) { x= n[0]; y= n[1]; z= n[2]; return *this; }

inline vec3i &vec3i::operator+=(int32 n)        { x+= n;    y+= n;    z+= n;    return *this; }
//inline vec3i &vec3i::operator+=(const vec2i &o) { x+= o.x;  y+= o.y;            return *this; }
inline vec3i &vec3i::operator+=(const vec3i &o) { x+= o.x;  y+= o.y;  z+= o.z;  return *this; }
//inline vec3i &vec3i::operator+=(const vec4i &o) { x+= o.x;  y+= o.y;  z+= o.z;  return *this; }
inline vec3i &vec3i::operator+=(const int32 *n) { x+= n[0]; y+= n[1]; z+= n[2]; return *this; }

inline vec3i &vec3i::operator-=(int32 n)        { x-= n;    y-= n;    z-= n;    return *this; }
//inline vec3i &vec3i::operator-=(const vec2i &o)   { x-= o.x;  y-= o.y;            return *this; }
inline vec3i &vec3i::operator-=(const vec3i &o)   { x-= o.x;  y-= o.y;  z-= o.z;  return *this; }
//inline vec3i &vec3i::operator-=(const vec4i &o)   { x-= o.x;  y-= o.y;  z-= o.z;  return *this; }
inline vec3i &vec3i::operator-=(const int32 *n) { x-= n[0]; y-= n[1]; z-= n[2]; return *this; }

inline vec3i &vec3i::operator*=(int32 n)        { x*= n;    y*= n;    z*= n;    return *this; }
//inline vec3i &vec3i::operator*=(const vec2i &o)   { x*= o.x;  y*= o.y;            return *this; }
inline vec3i &vec3i::operator*=(const vec3i &o)   { x*= o.x;  y*= o.y;  z*= o.z;  return *this; }
//inline vec3i &vec3i::operator*=(const vec4i &o)   { x*= o.x;  y*= o.y;  z*= o.z;  return *this; }
inline vec3i &vec3i::operator*=(const int32 *n) { x*= n[0]; y*= n[1]; z*= n[2]; return *this; }

inline vec3i &vec3i::operator/=(int32 n)        { x/= n;    y/= n;    z/= n;    return *this; }
//inline vec3i &vec3i::operator/=(const vec2i &o)   { x/= o.x;  y/= o.y;            return *this; }
inline vec3i &vec3i::operator/=(const vec3i &o)   { x/= o.x;  y/= o.y;  z/= o.z;  return *this; }
//inline vec3i &vec3i::operator/=(const vec4i &o)   { x/= o.x;  y/= o.y;  z/= o.z;  return *this; }
inline vec3i &vec3i::operator/=(const int32 *n) { x/= n[0]; y/= n[1]; z/= n[2]; return *this; }

inline const vec3i vec3i::operator+(int32 n)        const { return vec3i(*this)+= n; }
//inline const vec3i vec3i::operator+(const vec2i &o)   const { return vec3i(*this)+= o; }
inline const vec3i vec3i::operator+(const vec3i &o)   const { return vec3i(*this)+= o; }
//inline const vec3i vec3i::operator+(const vec4i &o)   const { return vec3i(*this)+= o; }
inline const vec3i vec3i::operator+(const int32 *n) const { return vec3i(*this)+= n; }

inline const vec3i vec3i::operator-(int32 n)        const { return vec3i(*this)-= n; }
//inline const vec3i vec3i::operator-(const vec2i &o)   const { return vec3i(*this)-= o; }
inline const vec3i vec3i::operator-(const vec3i &o)   const { return vec3i(*this)-= o; }
//inline const vec3i vec3i::operator-(const vec4i &o)   const { return vec3i(*this)-= o; }
inline const vec3i vec3i::operator-(const int32 *n) const { return vec3i(*this)-= n; }

inline const vec3i vec3i::operator*(int32 n)        const { return vec3i(*this)*= n; }
//inline const vec3i vec3i::operator*(const vec2i &o)   const { return vec3i(*this)*= o; }
inline const vec3i vec3i::operator*(const vec3i &o)   const { return vec3i(*this)*= o; }
//inline const vec3i vec3i::operator*(const vec4i &o)   const { return vec3i(*this)*= o; }
inline const vec3i vec3i::operator*(const int32 *n) const { return vec3i(*this)*= n; }

inline const vec3i vec3i::operator/(int32 n)        const { return vec3i(*this)/= n; }
//inline const vec3i vec3i::operator/(const vec2i &o)   const { return vec3i(*this)/= o; }
inline const vec3i vec3i::operator/(const vec3i &o)   const { return vec3i(*this)/= o; }
//inline const vec3i vec3i::operator/(const vec4i &o)   const { return vec3i(*this)/= o; }
inline const vec3i vec3i::operator/(const int32 *n) const { return vec3i(*this)/= n; }

inline const vec3i operator+(int32 scalar, const vec3i &v) { return vec3i(scalar+ v.x, scalar+ v.y, scalar+ v.z); }
inline const vec3i operator-(int32 scalar, const vec3i &v) { return vec3i(scalar- v.x, scalar- v.y, scalar- v.z); }
inline const vec3i operator*(int32 scalar, const vec3i &v) { return vec3i(scalar* v.x, scalar* v.y, scalar* v.z); }
inline const vec3i operator/(int32 scalar, const vec3i &v) { return vec3i(scalar/ v.x, scalar/ v.y, scalar/ v.z); }

inline bool vec3i::operator==(int32 n)        const { return (n== length()); }
//inline bool vec3i::operator==(const vec2i &o)   const { return ((x== o.x)  && (y== o.y)); }
inline bool vec3i::operator==(const vec3i &o)   const { return ((x== o.x)  && (y== o.y)  && (z== o.z)); }
//inline bool vec3i::operator==(const vec4i &o)   const { return ((x== o.x)  && (y== o.y)  && (z== o.z)); }
inline bool vec3i::operator==(const int32 *n) const { return ((x== n[0]) && (y== n[1]) && (z== n[2])); }

inline bool vec3i::operator!=(int32 n)        const { return (n!= length()); }
//inline bool vec3i::operator!=(const vec2i &o)   const { return ((x!= o.x)  || (y!= o.y)); }
inline bool vec3i::operator!=(const vec3i &o)   const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)); }
//inline bool vec3i::operator!=(const vec4i &o)   const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)); }
inline bool vec3i::operator!=(const int32 *n) const { return ((x!= n[0]) || (y!= n[1]) || (z!= n[2])); }

inline bool vec3i::operator!() const { return ((x || y || z)? false: true); }

inline const vec3i vec3i::operator-() const { return vec3i(-x, -y, -z); }

inline int32 &vec3i::operator[](int i)            { return v[i]; }
inline const int32 vec3i::operator[](int i) const { return v[i]; }
inline vec3i::operator int32* ()             { return v; }
inline vec3i::operator const int32 *() const { return v; }

// dot product

//inline int32 vec3i::dot(const vec2i &o, int32 _z)         const { return x* o.x+   y* o.y+  z* _z; }
inline int32 vec3i::dot(const vec3i &o)                     const { return x* o.x+   y* o.y+  z* o.z; }
//inline int32 vec3i::dot(const vec4i &o)                     const { return x* o.x+   y* o.y+  z* o.z; }
inline int32 vec3i::dot(int32 _x, int32 _y, int32 _z) const { return x* _x+    y* _y+   z* _z; }
inline int32 vec3i::dot(const int32 *n)                   const { return x* n[0] + y* n[1]+ z* n[2]; }

// cross product

//vec3i vec3i::cross(const vec3i &o)                            const { return vec3i(crossSSE(*this, o)); }
//inline vec3i vec3i::cross(const vec2i &o, int32 _z)         const { return vec3i(y* _z-  z* o.y, z* o.x- x* _z,  x* o.y- y* o.x); }
inline vec3i vec3i::cross(const vec3i &o)                     const { return vec3i(y* o.z- z* o.y, z* o.x- x* o.z, x* o.y- y* o.x); }
//inline vec3i vec3i::cross(const vec4i &o)                     const { return vec3i(y* o.z- z* o.y, z* o.x- x* o.z, x* o.y- y* o.x); }
inline vec3i vec3i::cross(int32 _x, int32 _y, int32 _z) const { return vec3i(y* _z-  z* _y,  z* _x-  x* _z,  x* _y-  y* _x); }


inline vec3i &vec3i::normalize() { int32 size= length(); if(size) { this->operator/=(size); } return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option

inline int32 vec3i::length() const { return roundf(sqrtf((float)(x* x+ y* y+ z* z))); };
inline int32 vec3i::norm2() const { return x* x + y* y + z* z; }

inline void vec3i::addConstant(int32 n) {
  int32 xx= x* x, yy= y* y, zz= z* z, size= length()+ n;
  if(x!= 0) x= (abs32(x)/ x)* size/ roundf(sqrtf(yy/ xx+ zz/ xx+ 1.0f));
  if(y!= 0) y= (abs32(y)/ y)* size/ roundf(sqrtf(xx/ yy+ zz/ yy+ 1.0f));
  if(z!= 0) z= (abs32(z)/ z)* size/ roundf(sqrtf(xx/ zz+ yy/ zz+ 1.0f));
};






///=========================================///
// vec4 ================-------------------- //
///=========================================///


// constructors

inline vec4::vec4():                                        x(0),    y(0),    z(0),    w(0)    {}
inline vec4::vec4(float _x, float _y, float _z, float _w):  x(_x),   y(_y),   z(_z),   w(_w)   {}
inline vec4::vec4(float n):                                 x(n),    y(n),    z(n),    w(n)    {}
inline vec4::vec4(const vec2 &o, float _z, float _w):       x(o.x),  y(o.y),  z(_z),   w(_w)   {}
inline vec4::vec4(const vec3 &o, float _w):                 x(o.x),  y(o.y),  z(o.z),  w(_w)   {}
inline vec4::vec4(const vec4 &o):                           x(o.x),  y(o.y),  z(o.z),  w(o.w)  {}
inline vec4::vec4(const float *n):                          x(n[0]), y(n[1]), z(n[2]), w(n[3]) {}
#ifdef MLIB_USE_INTRINSICS // this might be useless - MUST TEST THIS - SPEEDS
inline vec4::vec4(__m128 n)                                { _mm_store_ps(v, n); }
#endif

// operators

#ifdef MLIB_USE_INTRINSICS
inline vec4 &vec4::operator=(__m128 n)      { _mm_store_ps(v, n); return *this; }
#endif
inline vec4 &vec4::operator=(float n)        { x= n;    y= n;    z= n;    w= n;    return *this; }
inline vec4 &vec4::operator=(const vec2 &o)  { x= o.x;  y= o.y;                    return *this; }
inline vec4 &vec4::operator=(const vec3 &o)  { x= o.x;  y= o.y;  z= o.z;           return *this; }
inline vec4 &vec4::operator=(const vec4 &o)  { x= o.x;  y= o.y;  z= o.z;  w= o.w;  return *this; }
inline vec4 &vec4::operator=(const float *n) { x= n[0]; y= n[1]; z= n[2]; w= n[3]; return *this; }


/*#ifdef MLIB_USE_INTRINSICS -slower -.-
inline vec4 &vec4::operator+=(float n)        { return *this= _mm_add_ps(*this, _mm_set_ps1(n)); }
inline vec4 &vec4::operator+=(const vec2 &o)  { return *this= _mm_add_ps(*this, o); }
inline vec4 &vec4::operator+=(const vec3 &o)  { return *this= _mm_add_ps(*this, o); }
inline vec4 &vec4::operator+=(const vec4 &o)  { return *this= _mm_add_ps(*this, o); } // this is faster, it should be the same: { _mm_store_ps(v, _mm_add_ps(_mm_load_ps(v), _mm_load_ps(o.v))); return *this; }
inline vec4 &vec4::operator+=(const float *n) { return *this= _mm_add_ps(*this, _mm_load_ps(n)); }

inline vec4 &vec4::operator-=(float n)        { return *this= _mm_sub_ps(*this, _mm_set_ps1(n)); }
inline vec4 &vec4::operator-=(const vec2 &o)  { return *this= _mm_sub_ps(*this, o); }
inline vec4 &vec4::operator-=(const vec3 &o)  { return *this= _mm_sub_ps(*this, o); }
inline vec4 &vec4::operator-=(const vec4 &o)  { return *this= _mm_sub_ps(*this, o); }
inline vec4 &vec4::operator-=(const float *n) { return *this= _mm_sub_ps(*this, _mm_load_ps(n)); }

inline vec4 &vec4::operator*=(float n)        { return *this= _mm_mul_ps(*this, _mm_set_ps1(n)); }
inline vec4 &vec4::operator*=(const vec2 &o)  { return *this= _mm_mul_ps(*this, o); }
inline vec4 &vec4::operator*=(const vec3 &o)  { return *this= _mm_mul_ps(*this, o); }
inline vec4 &vec4::operator*=(const vec4 &o)  { return *this= _mm_mul_ps(*this, o); }
inline vec4 &vec4::operator*=(const float *n) { return *this= _mm_mul_ps(*this, _mm_load_ps(n)); }

inline vec4 &vec4::operator/=(float n)        { return *this= _mm_div_ps(*this, _mm_set_ps1(n)); }
inline vec4 &vec4::operator/=(const vec2 &o)  { return *this= _mm_div_ps(*this, o); }
inline vec4 &vec4::operator/=(const vec3 &o)  { return *this= _mm_div_ps(*this, o); }
inline vec4 &vec4::operator/=(const vec4 &o)  { return *this= _mm_div_ps(*this, o); }
inline vec4 &vec4::operator/=(const float *n) { return *this= _mm_div_ps(*this, _mm_load_ps(n)); }
#else */
inline vec4 &vec4::operator+=(float n)        { x+= n;    y+= n;    z+= n;    w+= n;    return *this; }
inline vec4 &vec4::operator+=(const vec2 &o)  { x+= o.x;  y+= o.y;                      return *this; }
inline vec4 &vec4::operator+=(const vec3 &o)  { x+= o.x;  y+= o.y;  z+= o.z;            return *this; }
inline vec4 &vec4::operator+=(const vec4 &o)  { x+= o.x;  y+= o.y;  z+= o.z;  w+= o.w;  return *this; }
inline vec4 &vec4::operator+=(const float *n) { x+= n[0]; y+= n[1]; z+= n[2]; w+= n[3]; return *this; }  

inline vec4 &vec4::operator-=(float n)        { x-= n;    y-= n;    z-= n;    w-= n;    return *this; }
inline vec4 &vec4::operator-=(const vec2 &o)  { x-= o.x;  y-= o.y;                      return *this; }
inline vec4 &vec4::operator-=(const vec3 &o)  { x-= o.x;  y-= o.y;  z-= o.z;            return *this; }
inline vec4 &vec4::operator-=(const vec4 &o)  { x-= o.x;  y-= o.y;  z-= o.z;  w-= o.w;  return *this; }
inline vec4 &vec4::operator-=(const float *n) { x-= n[0]; y-= n[1]; z-= n[2]; w-= n[3]; return *this; }

inline vec4 &vec4::operator*=(float n)        { x*= n;    y*= n;    z*= n;    w*= n;    return *this; }
inline vec4 &vec4::operator*=(const vec2 &o)  { x*= o.x;  y*= o.y;                      return *this; }
inline vec4 &vec4::operator*=(const vec3 &o)  { x*= o.x;  y*= o.y;  z*= o.z;            return *this; }
inline vec4 &vec4::operator*=(const vec4 &o)  { x*= o.x;  y*= o.y;  z*= o.z;  w*= o.w;  return *this; }
inline vec4 &vec4::operator*=(const float *n) { x*= n[0]; y*= n[1]; z*= n[2]; w*= n[3]; return *this; }

inline vec4 &vec4::operator/=(float n)        { x/= n;    y/= n;    z/= n;    w/= n;    return *this; }
inline vec4 &vec4::operator/=(const vec2 &o)  { x/= o.x;  y/= o.y;                      return *this; }
inline vec4 &vec4::operator/=(const vec3 &o)  { x/= o.x;  y/= o.y;  z/= o.z;            return *this; }
inline vec4 &vec4::operator/=(const vec4 &o)  { x/= o.x;  y/= o.y;  z/= o.z;  w/= o.w;  return *this; }
inline vec4 &vec4::operator/=(const float *n) { x/= n[0]; y/= n[1]; z/= n[2]; w/= n[3]; return *this; }
//#endif

inline const vec4 vec4::operator+(float n)        const { return vec4(*this)+= n; }
inline const vec4 vec4::operator+(const vec2 &o)  const { return vec4(*this)+= o; }
inline const vec4 vec4::operator+(const vec3 &o)  const { return vec4(*this)+= o; }
inline const vec4 vec4::operator+(const vec4 &o)  const { return vec4(*this)+= o; }
inline const vec4 vec4::operator+(const float *n) const { return vec4(*this)+= n; }

inline const vec4 vec4::operator-(float n)        const { return vec4(*this)-= n; }
inline const vec4 vec4::operator-(const vec2 &o)  const { return vec4(*this)-= o; }
inline const vec4 vec4::operator-(const vec3 &o)  const { return vec4(*this)-= o; }
inline const vec4 vec4::operator-(const vec4 &o)  const { return vec4(*this)-= o; }
inline const vec4 vec4::operator-(const float *n) const { return vec4(*this)-= n; }

inline const vec4 vec4::operator*(float n)        const { return vec4(*this)*= n; }
inline const vec4 vec4::operator*(const vec2 &o)  const { return vec4(*this)*= o; }
inline const vec4 vec4::operator*(const vec3 &o)  const { return vec4(*this)*= o; }
inline const vec4 vec4::operator*(const vec4 &o)  const { return vec4(*this)*= o; }
inline const vec4 vec4::operator*(const float *n) const { return vec4(*this)*= n; }

inline const vec4 vec4::operator/(float n)        const { return vec4(*this)/= n; }
inline const vec4 vec4::operator/(const vec2 &o)  const { return vec4(*this)/= o; }
inline const vec4 vec4::operator/(const vec3 &o)  const { return vec4(*this)/= o; }
inline const vec4 vec4::operator/(const vec4 &o)  const { return vec4(*this)/= o; }
inline const vec4 vec4::operator/(const float *n) const { return vec4(*this)/= n; }

/*#ifdef MLIB_USE_INTRINSICS - slower -.-
inline const vec4 operator+(float scalar, const vec4 &v) { return vec4(_mm_add_ps(_mm_set_ps1(scalar), v)); }
inline const vec4 operator-(float scalar, const vec4 &v) { return vec4(_mm_sub_ps(_mm_set_ps1(scalar), v)); }
inline const vec4 operator*(float scalar, const vec4 &v) { return vec4(_mm_mul_ps(_mm_set_ps1(scalar), v)); }
inline const vec4 operator/(float scalar, const vec4 &v) { return vec4(_mm_div_ps(_mm_set_ps1(scalar), v)); }
#else */
inline const vec4 operator+(float scalar, const vec4 &v) { return vec4(scalar+ v.x, scalar+ v.y, scalar+ v.z, scalar+ v.w); }
inline const vec4 operator-(float scalar, const vec4 &v) { return vec4(scalar- v.x, scalar- v.y, scalar- v.z, scalar- v.w); }
inline const vec4 operator*(float scalar, const vec4 &v) { return vec4(scalar* v.x, scalar* v.y, scalar* v.z, scalar* v.w); }
inline const vec4 operator/(float scalar, const vec4 &v) { return vec4(scalar/ v.x, scalar/ v.y, scalar/ v.z, scalar/ v.w); }
//#endif

inline bool vec4::operator==(float n)        const { return (n== length()); }
inline bool vec4::operator==(const vec2 &o)  const { return ((x== o.x)  && (y== o.y)); }
inline bool vec4::operator==(const vec3 &o)  const { return ((x== o.x)  && (y== o.y)  && (z== o.z)); }
inline bool vec4::operator==(const vec4 &o)  const { return ((x== o.x)  && (y== o.y)  && (z== o.z)  && (w== o.w)); }
inline bool vec4::operator==(const float *n) const { return ((x== n[0]) && (y== n[1]) && (z== n[2]) && (w== n[3])); }

inline bool vec4::operator!=(const vec2 &o)  const { return ((x!= o.x)  || (y!= o.y)); }
inline bool vec4::operator!=(const vec3 &o)  const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)); }
inline bool vec4::operator!=(const vec4 &o)  const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)  || (w!= o.w)); }
inline bool vec4::operator!=(const float *n) const { return ((x!= n[0]) || (y!= n[1]) || (z!= n[2]) || (w!= n[3])); }
inline bool vec4::operator!=(float n)        const { return (n!= length()); }

inline bool vec4::operator!() const { return ((x || y || z || w)? false: true); }

inline const vec4 vec4::operator-() const { return vec4(-x, -y, -z, -w); }

inline float &vec4::operator[](int i)            { return v[i]; }
inline const float vec4::operator[](int i) const { return v[i]; }
inline vec4::operator float* ()             { return v; }
inline vec4::operator const float *() const { return v; }
#ifdef MLIB_USE_INTRINSICS
inline vec4::operator const __m128() const { return _mm_load_ps(v); }
#endif

// dot product

inline float vec4::dot(const vec2 &o, float _z, float _w) const { return this->dot(o.x,  o.y,  _z,   _w); }
inline float vec4::dot(const vec3 &o, float _w)           const { return this->dot(o.x,  o.y,  o.z,  _w); }
inline float vec4::dot(const vec4 &o)                     const { return this->dot(o.x,  o.y,  o.z,  o.w); }
inline float vec4::dot(const float *n)                    const { return this->dot(n[0], n[1], n[2], n[3]); }
//#ifdef MLIB_USE_INTRINSICS
//inline float vec4::dot(float _x, float _y, float _z, float _w) const { float ret; _mm_store_ss(&ret, dotSSE(*this, _mm_set_ps(_w, _z, _y, _x))); return ret; }
//#else
inline float vec4::dot(float _x, float _y, float _z, float _w) const { return x* _x+   y* _y+   z* _z+   w* _w; }
//#endif

  

// funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl
inline vec4 &vec4::set(float _x, float _y, float _z, float _w) { x= _x; y= _y; z= _z; w= _w; return *this; }
inline vec4 &vec4::normalize() { float size= length(); if(size) { this->operator/=(size); } return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option

//#ifdef MLIB_USE_INTRINSICS 2.5x times slower
//inline float vec4::length() const { float ret; _mm_store_ss(&ret, lengthSSE(*this)); return ret; };
//inline float vec4::norm2() const { float ret; _mm_store_ss(&ret, norm2SSE(*this)); return ret; }
//#else
inline float vec4::length() const { return sqrtf(x* x+ y* y+ z* z+ w* w); };
inline float vec4::norm2() const { return x* x+ y* y+ z* z+ w* w; }
//#endif






///=========================================///
// mat4 ================-------------------- //
///=========================================///

// constructors

inline mat4::mat4() { identity(); };

inline mat4::mat4(const mat4 &o) {	
  v[0]= o[0];   v[4]= o[4];   v[8]=  o[8];    v[12]= o[12];
  v[1]= o[1];   v[5]= o[5];   v[9]=  o[9];    v[13]= o[13];
  v[2]= o[2];   v[6]= o[6];   v[10]= o[10];   v[14]= o[14];
  v[3]= o[3];   v[7]= o[7];   v[11]= o[11];   v[15]= o[15];
};

inline mat4::mat4(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33) {
  v[0]= n00;   v[4]= n10;   v[8]=  n20;   v[12]= n30;
  v[1]= n01;   v[5]= n11;   v[9]=  n21;   v[13]= n31;
  v[2]= n02;   v[6]= n12;   v[10]= n22;   v[14]= n32;
  v[3]= n03;   v[7]= n13;   v[11]= n23;   v[15]= n33;
}

inline mat4::mat4(float *o) {
  v[0]= o[0];   v[4]= o[4];   v[8]=  o[8];    v[12]= o[12];
  v[1]= o[1];   v[5]= o[5];   v[9]=  o[9];    v[13]= o[13];
  v[2]= o[2];   v[6]= o[6];   v[10]= o[10];   v[14]= o[14];
  v[3]= o[3];   v[7]= o[7];   v[11]= o[11];   v[15]= o[15];
};
  


// various matrix types

inline mat4 &mat4::identity() {
  v[0]= 1.0f; v[4]= 0.0f; v[8]=  0.0f; v[12]= 0.0f;
  v[1]= 0.0f; v[5]= 1.0f; v[9]=  0.0f; v[13]= 0.0f;
  v[2]= 0.0f; v[6]= 0.0f; v[10]= 1.0f; v[14]= 0.0f;
  v[3]= 0.0f; v[7]= 0.0f; v[11]= 0.0f; v[15]= 1.0f;
  return *this;
}

inline mat4 &mat4::zero() {
  v[0]= v[4]= v[8]=  v[12]= 
  v[1]= v[5]= v[9]=  v[13]= 
  v[2]= v[6]= v[10]= v[14]= 
  v[3]= v[7]= v[11]= v[15]= 0.0f;
  return *this;
};

// rotation functions

// see http://www.khronos.org/opengles/documentation/html/glRotate.html if there are any bugs :)

inline mat4 &mat4::rotate(float angle, const vec3 &axis) { // axis to rotate about, angle = angle in degrees
  vec3 a= axis;
  a.normalize();

  float s= sinf(angle* DEG2RAD);
  float c= cosf(angle* DEG2RAD);
  float c1= 1- c;
  float x1c= a.x* c1;
  float y1c= a.y* c1;
  float z1c= a.z* c1;

  v[0]=  a.x*  x1c+ c;
  v[1]=  a.x*  y1c+ a.z* s;
  v[2]=  a.x*  z1c- a.y* s;
  v[3]=  0.0f;

  v[4]=  a.x*  y1c- a.z* s;
  v[5]=  a.y*  y1c+ c;
  v[6]=  a.y*  y1c+ a.x* s;
  v[7]=  0.0f;

  v[8]=  a.x*  z1c+ a.y* s;
  v[9]=  a.y*  z1c- a.x* s;
  v[10]= a.z*  z1c+ c;
  v[11]= 0.0f;

  v[12]= 0.0f;
  v[13]= 0.0f;
  v[14]= 0.0f;
  v[15]= 1.0f;
  return *this;
}

inline mat4 &mat4::rotate(float angle, float x, float y, float z) { return rotate(angle, vec3(x, y, z)); }

/// uses fewer computations than rotate
inline mat4 &mat4::rotateX(float angle) {
  float c= cosf(angle* DEG2RAD);
  float s= sinf(angle* DEG2RAD);

  v[0]= 1.0f;   v[4]= 0.0f;   v[8]=  0.0f;   v[12]= 0.0f;
  v[1]= 0.0f;   v[5]= c;      v[9]=  -s;     v[13]= 0.0f;
  v[2]= 0.0f;   v[6]= s;      v[10]= c;      v[14]= 0.0f;
  v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
  return *this;
}

/// uses fewer computations than rotate
inline mat4 &mat4::rotateY(float angle) {
  float c= cosf(angle* DEG2RAD);
  float s= sinf(angle* DEG2RAD);

  v[0]= c;      v[4]= 0.0f;   v[8]=  s;      v[12]= 0.0f;
  v[1]= 0.0f;   v[5]= 1.0f;   v[9]=  0.0f;   v[13]= 0.0f;
  v[2]= -s;     v[6]= 0.0f;   v[10]= c;      v[14]= 0.0f;
  v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
  return *this;
}

/// uses fewer computations than rotate
inline mat4 &mat4::rotateZ(float angle) {
  float c= cosf(angle* DEG2RAD);
  float s= sinf(angle* DEG2RAD);

  v[0]= c;      v[4]= -s;     v[8]=  0.0f;   v[12]= 0.0f;
  v[1]= s;      v[5]= c;      v[9]=  0.0f;   v[13]= 0.0f;
  v[2]= 0.0f;   v[6]= 0.0f;   v[10]= 1.0f;   v[14]= 0.0f;
  v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
  return *this;
}

inline mat4 &mat4::translate(const vec3 &o) {
  v[0]= 1.0f; v[4]= 0.0f; v[8]=  0.0f; v[12]= o[0];
  v[1]= 0.0f; v[5]= 1.0f; v[9]=  0.0f; v[13]= o[1];
  v[2]= 0.0f; v[6]= 0.0f; v[10]= 1.0f; v[14]= o[2];
  v[3]= 0.0f; v[7]= 0.0f; v[11]= 0.0f; v[15]= 1.0f;
  return *this;
} 

inline mat4 &mat4::translate(float x,float y, float z) { return translate(vec3(x, y, z)); }
  
inline mat4 &mat4::scale(const vec3 &o) {
  v[0]= o[0];   v[4]= 0.0f;   v[8]= 0.0f;    v[12]= 0.0f;
  v[1]= 0.0f;   v[5]= o[1];   v[9]= 0.0f;    v[13]= 0.0f;
  v[2]= 0.0f;   v[6]= 0.0f;   v[10]= o[2];   v[14]= 0.0f;
  v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
  return *this;
}
  
inline mat4 &mat4::scale(float x,float y,float z) { return scale(vec3(x, y, z)); } 






// operators

inline mat4::operator float *() { return v; }
inline mat4::operator const float *() const { return v; }
//inline const float operator[](int i) const { return v[i]; } // might not be needed (further thinking needed)
//inline float &operator[](int i) { return v[i]; }            // might not be needed (further thinking needed)

inline mat4 mat4::operator+(const mat4 &o) {
  return mat4(v[0]+  o[0],   v[1]+  o[1],   v[2]+  o[2],    v[3]+  o[3],
              v[4]+  o[4],   v[5]+  o[5],   v[6]+  o[6],    v[7]+  o[7],
              v[8]+  o[8],   v[9]+  o[9],   v[10]+ o[10],   v[11]+ o[11],
              v[12]+ o[12],  v[13]+ o[13],  v[14]+ o[14],   v[15]+ o[15]);
}

inline mat4 mat4::operator+(float n) {
  return mat4(v[0]+  n,   v[1]+  n,   v[2]+  n,    v[3]+  n,
              v[4]+  n,   v[5]+  n,   v[6]+  n,    v[7]+  n,
              v[8]+  n,   v[9]+  n,   v[10]+ n,    v[11]+ n,
              v[12]+ n,   v[13]+ n,   v[14]+ n,    v[15]+ n);
}

inline mat4 mat4::operator-(const mat4 &o) {
  return mat4(v[0]-  o[0],   v[1]-  o[1],   v[2]-  o[2],    v[3]-  o[3],
              v[4]-  o[4],   v[5]-  o[5],   v[6]-  o[6],    v[7]-  o[7],
              v[8]-  o[8],   v[9]-  o[9],   v[10]- o[10],   v[11]- o[11],
              v[12]- o[12],  v[13]- o[13],  v[14]- o[14],   v[15]- o[15]);
}

inline mat4 mat4::operator-(float n) {
  return mat4(v[0]-  n,   v[1]-  n,   v[2]-  n,    v[3]-  n,
              v[4]-  n,   v[5]-  n,   v[6]-  n,    v[7]-  n,
              v[8]-  n,   v[9]-  n,   v[10]- n,    v[11]- n,
              v[12]- n,   v[13]- n,   v[14]- n,    v[15]- n);
}

inline mat4 mat4::operator*(const mat4 &o) {
                /// first column
  return mat4(v[0]* o[0]+  v[4]* o[1]+  v[8]*  o[2]+  v[12]* o[3],
              v[1]* o[0]+  v[5]* o[1]+  v[9]*  o[2]+  v[13]* o[3],
              v[2]* o[0]+  v[6]* o[1]+  v[10]* o[2]+  v[14]* o[3],
              v[3]* o[0]+  v[7]* o[1]+  v[11]* o[2]+  v[15]* o[3],
              /// second column
              v[0]* o[4]+  v[4]* o[5]+  v[8]*  o[6]+  v[12]* o[7],
              v[1]* o[4]+  v[5]* o[5]+  v[9]*  o[6]+  v[13]* o[7],
              v[2]* o[4]+  v[6]* o[5]+  v[10]* o[6]+  v[14]* o[7],
              v[3]* o[4]+  v[7]* o[5]+  v[11]* o[6]+  v[15]* o[7],
              /// third column
              v[0]* o[8]+  v[4]* o[9]+  v[8]*  o[10]+ v[12]* o[11],
              v[1]* o[8]+  v[5]* o[9]+  v[9]*  o[10]+ v[13]* o[11],
              v[2]* o[8]+  v[6]* o[9]+  v[10]* o[10]+ v[14]* o[11],
              v[3]* o[8]+  v[7]* o[9]+  v[11]* o[10]+ v[15]* o[11],
              /// fourth column
              v[0]* o[12]+ v[4]* o[13]+ v[8]*  o[14]+ v[12]* o[15],
              v[1]* o[12]+ v[5]* o[13]+ v[9]*  o[14]+ v[13]* o[15],
              v[2]* o[12]+ v[6]* o[13]+ v[10]* o[14]+ v[14]* o[15],
              v[3]* o[12]+ v[7]* o[13]+ v[11]* o[14]+ v[15]* o[15]);
              /// wheew. done.
}

// intrinsic multiply - yes, slower -.-
/*
inline mat4 mat4::operator*(const mat4 &o) {
  __m128 reg1, reg2;
  vec4 tmp;
  mat4 ret;

  for(int a= 0; a< 4; a++) {
    reg1= _mm_load_ps(v+ a* 4);
    reg2= _mm_set_ps(o[0+ a], o[4+ a], o[8+ a], o[12+ a]);
  
    tmp= _mm_mul_ss(reg1, reg2);
    ret[0]+= tmp.x; ret[1]+= tmp.y; ret[2]+= tmp.z; ret[3]+= tmp.w;

    _mm_shuffle_ps(reg2, reg2, _MM_SHUFFLE(2, 3, 1, 0));
    tmp= _mm_mul_ss(reg1, reg2);
    ret[4]+= tmp.x; ret[5]+= tmp.y; ret[6]+= tmp.z; ret[7]+= tmp.w;

    _mm_shuffle_ps(reg2, reg2, _MM_SHUFFLE(1, 0, 2, 3));
    tmp= _mm_mul_ss(reg1, reg2);
    ret[8]+= tmp.x; ret[9]+= tmp.y; ret[10]+= tmp.z; ret[11]+= tmp.w;

    _mm_shuffle_ps(reg2, reg2, _MM_SHUFFLE(2, 3, 1, 0));
    tmp= _mm_mul_ss(reg1, reg2);
    ret[12]+= tmp.x; ret[13]+= tmp.y; ret[14]+= tmp.z; ret[15]+= tmp.w;
  }

  return ret;
}
*/


// assume the vectors are padded with a 1 (homogenous coordinates)
inline vec2 mat4::operator*(const vec2 &o) {
  return vec3(v[0]* o[0]+ v[4]* o[1]+ v[8]*  o[2]+ v[12],
              v[1]* o[0]+ v[5]* o[1]+ v[9]*  o[2]+ v[13]);
}

inline vec3 mat4::operator*(const vec3 &o) {
  return vec3(v[0]* o[0]+ v[4]* o[1]+ v[8]*  o[2]+ v[12],
              v[1]* o[0]+ v[5]* o[1]+ v[9]*  o[2]+ v[13],
              v[2]* o[0]+ v[6]* o[1]+ v[10]* o[2]+ v[14]);
}

inline vec4 mat4::operator*(const vec4 &o) {
  return vec4(v[0]* o[0]+ v[4]* o[1]+ v[8]*  o[2]+ v[12],
              v[1]* o[0]+ v[5]* o[1]+ v[9]*  o[2]+ v[13],
              v[2]* o[0]+ v[6]* o[1]+ v[10]* o[2]+ v[14],
              v[3]* o[0]+ v[7]* o[1]+ v[11]* o[2]+ v[15]);
}

inline mat4& mat4::operator*=(const mat4 &o) { return *this= *this* o; }
inline mat4& mat4::operator+=(const mat4 &o) { return *this= *this+ o; }
inline mat4& mat4::operator-=(const mat4 &o) { return *this= *this- o; }


// vector operations



// funcs

inline mat4 &mat4::ortho(float l, float r, float b, float t, float zn, float zf) {
  float dx= r- l, dy= t- b, dz= zf- zn;

  v[0]= 2.0f/ dx;   v[4]= 0.0f;       v[8]=   0.0f;       v[12]= -(r+ l)/ dx;
  v[1]= 0.0f;       v[5]= 2.0f/ dy;   v[9]=   0.0f;       v[13]= -(t+ b)/ dy;
  v[2]= 0.0f;       v[6]= 0.0f;       v[10]= -2.0f/ dz;   v[14]= -(zf+ zn)/ dz;
  v[3]= 0.0f;       v[7]= 0.0f;       v[11]=  0.0f;       v[15]= 1.0f;

  return *this;
}


inline mat4 &mat4::frustrum(float l, float r, float b, float t, float zn, float zf) {
  float dx= r- l, dy= t- b, dz= zf- zn;

  v[0]= (2.0f* zn)/ dx;   v[4]= 0.0f;             v[8]=  0.0f;            v[12]= 0.0f;
  v[1]= 0.0f;             v[5]= (2.0f* zn)/ dy;   v[9]=  (t+ b)/ dy;      v[13]= 0.0f;
  v[2]= 0.0f;             v[6]= 0.0f;             v[10]= -(zf+ zn)/ dz;   v[14]= -(2.0f* zf* zn)/ dz;
  v[3]= 0.0f;             v[7]= 0.0f;             v[11]= -1.0f;           v[15]= 1.0f;

  return *this;
}


inline mat4 &mat4::perspective(float fovy, float aspect, float znear, float zfar) {
  if((!fovy) || (fabsf(fovy)== 360.0f) || (!(zfar- znear)) || (!aspect))
    return *this;

  float dz= zfar- znear;
  float ctg= 1.0f/ tanf(fovy/ 2.0f* DEG2RAD);
  
  v[0]= ctg/ aspect;  v[4]= 0.0f;   v[8]= 0;                    v[12]= 0.0f;
  v[1]= 0.0f;         v[5]= ctg;    v[9]= 0;                    v[13]= 0.0f;
  v[3]= 0.0f;         v[6]= 0.0f;   v[10]= -(zfar+ znear)/ dz;  v[14]= -2.0f* znear* zfar/ dz;
  v[2]= 0.0f;         v[7]= 0.0f;   v[11]= -1.0f;               v[15]= 1.0f;

  return *this;
}


inline mat4 &mat4::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
  vec3 f(center- eye);  f.normalize();
  vec3 s(cross(f, up)); s.normalize();
  vec3 u(cross(s, f));
  
  v[0]=  s.x;   v[4]=  s.y;   v[8]=   s.z;   v[12]= -dot(s, eye);
  v[1]=  u.x;   v[5]=  u.y;   v[9]=   u.z;   v[13]= -dot(u, eye);
  v[2]= -f.x;   v[6]= -f.y;   v[10]= -f.z;   v[14]=  dot(f, eye);
  v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]=  1.0f;

  return *this;
}



// 1e-38= pow10f[0]   1e0= pow10f[38]   1e38= pow10f[76]
static const float pow10f[]= {
                    1e-38f, 1e-37f, 1e-36f, 1e-35f, 1e-34f, 1e-33f, 1e-32f, 1e-31f, 1e-30f, 1e-29f, 1e-28f,
                    1e-27f, 1e-26f, 1e-25f, 1e-24f, 1e-23f, 1e-22f, 1e-21f, 1e-20f, 1e-19f, 1e-18f, 1e-17f,
                    1e-16f, 1e-15f, 1e-14f, 1e-13f, 1e-12f, 1e-11f, 1e-10f, 1e-9f,  1e-8f,  1e-7f,  1e-6f,
                    1e-5f,  1e-4f,  1e-3f,  1e-2f,  1e-1f,  1e0f,   1e1f,   1e2f,   1e3f,   1e4f,   1e5f,
                    1e6f,   1e7f,   1e8f,   1e9f,   1e10f,  1e11f,  1e12f,  1e13f,  1e14f,  1e15f,  1e16f,
                    1e17f,  1e18f,  1e19f,  1e20f,  1e21f,  1e22f,  1e23f,  1e24f,  1e25f,  1e26f,  1e27f,
                    1e28f,  1e29f,  1e30f,  1e31f,  1e32f,  1e33f,  1e34f,  1e35f,  1e36f,  1e37f,  1e38f };

// 1e-308= pow10[0]   1e0= pow10[308]   1e308= pow10[616]
static const double pow10d[]= {
                    1e-308, 1e-307, 1e-306, 1e-305, 1e-304, 1e-303, 1e-302, 1e-301, 1e-300, 1e-299,
                    1e-298, 1e-297, 1e-296, 1e-295, 1e-294, 1e-293, 1e-292, 1e-291, 1e-290, 1e-289,
                    1e-288, 1e-287, 1e-286, 1e-285, 1e-284, 1e-283, 1e-282, 1e-281, 1e-280, 1e-279,
                    1e-278, 1e-277, 1e-276, 1e-275, 1e-274, 1e-273, 1e-272, 1e-271, 1e-270, 1e-269,
                    1e-268, 1e-267, 1e-266, 1e-265, 1e-264, 1e-263, 1e-262, 1e-261, 1e-260, 1e-259,
                    1e-258, 1e-257, 1e-256, 1e-255, 1e-254, 1e-253, 1e-252, 1e-251, 1e-250, 1e-249,
                    1e-248, 1e-247, 1e-246, 1e-245, 1e-244, 1e-243, 1e-242, 1e-241, 1e-240, 1e-239,
                    1e-238, 1e-237, 1e-236, 1e-235, 1e-234, 1e-233, 1e-232, 1e-231, 1e-230, 1e-229,
                    1e-228, 1e-227, 1e-226, 1e-225, 1e-224, 1e-223, 1e-222, 1e-221, 1e-220, 1e-219,
                    1e-218, 1e-217, 1e-216, 1e-215, 1e-214, 1e-213, 1e-212, 1e-211, 1e-210, 1e-209,
                    1e-208, 1e-207, 1e-206, 1e-205, 1e-204, 1e-203, 1e-202, 1e-201, 1e-200, 1e-199,
                    1e-198, 1e-197, 1e-196, 1e-195, 1e-194, 1e-193, 1e-192, 1e-191, 1e-190, 1e-189,
                    1e-188, 1e-187, 1e-186, 1e-185, 1e-184, 1e-183, 1e-182, 1e-181, 1e-180, 1e-179,
                    1e-178, 1e-177, 1e-176, 1e-175, 1e-174, 1e-173, 1e-172, 1e-171, 1e-170, 1e-169,
                    1e-168, 1e-167, 1e-166, 1e-165, 1e-164, 1e-163, 1e-162, 1e-161, 1e-160, 1e-159,
                    1e-158, 1e-157, 1e-156, 1e-155, 1e-154, 1e-153, 1e-152, 1e-151, 1e-150, 1e-149,
                    1e-148, 1e-147, 1e-146, 1e-145, 1e-144, 1e-143, 1e-142, 1e-141, 1e-140, 1e-139,
                    1e-138, 1e-137, 1e-136, 1e-135, 1e-134, 1e-133, 1e-132, 1e-131, 1e-130, 1e-129,
                    1e-128, 1e-127, 1e-126, 1e-125, 1e-124, 1e-123, 1e-122, 1e-121, 1e-120, 1e-119,
                    1e-118, 1e-117, 1e-116, 1e-115, 1e-114, 1e-113, 1e-112, 1e-111, 1e-110, 1e-109,
                    1e-108, 1e-107, 1e-106, 1e-105, 1e-104, 1e-103, 1e-102, 1e-101, 1e-100, 1e-99,
                    1e-98,  1e-97,  1e-96,  1e-95,  1e-94,  1e-93,  1e-92,  1e-91,  1e-90,  1e-89,
                    1e-88,  1e-87,  1e-86,  1e-85,  1e-84,  1e-83,  1e-82,  1e-81,  1e-80,  1e-79,
                    1e-78,  1e-77,  1e-76,  1e-75,  1e-74,  1e-73,  1e-72,  1e-71,  1e-70,  1e-69,
                    1e-68,  1e-67,  1e-66,  1e-65,  1e-64,  1e-63,  1e-62,  1e-61,  1e-60,  1e-59,
                    1e-58,  1e-57,  1e-56,  1e-55,  1e-54,  1e-53,  1e-52,  1e-51,  1e-50,  1e-49,
                    1e-48,  1e-47,  1e-46,  1e-45,  1e-44,  1e-43,  1e-42,  1e-41,  1e-40,  1e-39,
                    1e-38,  1e-37,  1e-36,  1e-35,  1e-34,  1e-33,  1e-32,  1e-31,  1e-30,  1e-29,
                    1e-28,  1e-27,  1e-26,  1e-25,  1e-24,  1e-23,  1e-22,  1e-21,  1e-20,  1e-19,
                    1e-18,  1e-17,  1e-16,  1e-15,  1e-14,  1e-13,  1e-12,  1e-11,  1e-10,  1e-9,
                    1e-8,   1e-7,   1e-6,   1e-5,   1e-4,   1e-3,   1e-2,   1e-1,   1e0,    1e1,
                    1e2,    1e3,    1e4,    1e5,    1e6,    1e7,    1e8,    1e9,    1e10,   1e11,
                    1e12,   1e13,   1e14,   1e15,   1e16,   1e17,   1e18,   1e19,   1e20,   1e21,
                    1e22,   1e23,   1e24,   1e25,   1e26,   1e27,   1e28,   1e29,   1e30,   1e31,
                    1e32,   1e33,   1e34,   1e35,   1e36,   1e37,   1e38,   1e39,   1e40,   1e41,
                    1e42,   1e43,   1e44,   1e45,   1e46,   1e47,   1e48,   1e49,   1e50,   1e51,
                    1e52,   1e53,   1e54,   1e55,   1e56,   1e57,   1e58,   1e59,   1e60,   1e61,
                    1e62,   1e63,   1e64,   1e65,   1e66,   1e67,   1e68,   1e69,   1e70,   1e71,
                    1e72,   1e73,   1e74,   1e75,   1e76,   1e77,   1e78,   1e79,   1e80,   1e81,
                    1e82,   1e83,   1e84,   1e85,   1e86,   1e87,   1e88,   1e89,   1e90,   1e91,
                    1e92,   1e93,   1e94,   1e95,   1e96,   1e97,   1e98,   1e99,   1e100,  1e101,
                    1e102,  1e103,  1e104,  1e105,  1e106,  1e107,  1e108,  1e109,  1e110,  1e111,
                    1e112,  1e113,  1e114,  1e115,  1e116,  1e117,  1e118,  1e119,  1e120,  1e121,
                    1e122,  1e123,  1e124,  1e125,  1e126,  1e127,  1e128,  1e129,  1e130,  1e131,
                    1e132,  1e133,  1e134,  1e135,  1e136,  1e137,  1e138,  1e139,  1e140,  1e141,
                    1e142,  1e143,  1e144,  1e145,  1e146,  1e147,  1e148,  1e149,  1e150,  1e151,
                    1e152,  1e153,  1e154,  1e155,  1e156,  1e157,  1e158,  1e159,  1e160,  1e161,
                    1e162,  1e163,  1e164,  1e165,  1e166,  1e167,  1e168,  1e169,  1e170,  1e171,
                    1e172,  1e173,  1e174,  1e175,  1e176,  1e177,  1e178,  1e179,  1e180,  1e181,
                    1e182,  1e183,  1e184,  1e185,  1e186,  1e187,  1e188,  1e189,  1e190,  1e191,
                    1e192,  1e193,  1e194,  1e195,  1e196,  1e197,  1e198,  1e199,  1e200,  1e201,
                    1e202,  1e203,  1e204,  1e205,  1e206,  1e207,  1e208,  1e209,  1e210,  1e211,
                    1e212,  1e213,  1e214,  1e215,  1e216,  1e217,  1e218,  1e219,  1e220,  1e221,
                    1e222,  1e223,  1e224,  1e225,  1e226,  1e227,  1e228,  1e229,  1e230,  1e231,
                    1e232,  1e233,  1e234,  1e235,  1e236,  1e237,  1e238,  1e239,  1e240,  1e241,
                    1e242,  1e243,  1e244,  1e245,  1e246,  1e247,  1e248,  1e249,  1e250,  1e251,
                    1e252,  1e253,  1e254,  1e255,  1e256,  1e257,  1e258,  1e259,  1e260,  1e261,
                    1e262,  1e263,  1e264,  1e265,  1e266,  1e267,  1e268,  1e269,  1e270,  1e271,
                    1e272,  1e273,  1e274,  1e275,  1e276,  1e277,  1e278,  1e279,  1e280,  1e281,
                    1e282,  1e283,  1e284,  1e285,  1e286,  1e287,  1e288,  1e289,  1e290,  1e291,
                    1e292,  1e293,  1e294,  1e295,  1e296,  1e297,  1e298,  1e299,  1e300,  1e301,
                    1e302,  1e303,  1e304,  1e305,  1e306,  1e307,  1e308 };

// -1e18= pow10i[0]   0= pow10i[19]   1e18= pow10i[38]
static const int64_t pow10i[]= { 
            -1000000000000000000ll, -100000000000000000ll, -10000000000000000ll, -1000000000000000ll, -100000000000000ll,
            -10000000000000ll, -1000000000000ll, -100000000000ll, -10000000000ll, -1000000000ll, -100000000ll, -10000000ll,
            -1000000ll, -100000ll, -10000ll, -1000ll, -100ll, -10ll, -1ll, 0ll, +1ll, +10ll, +100ll, +1000ll, +10000ll, +100000ll, +1000000ll,
            +10000000ll, +100000000ll, +1000000000ll, +10000000000ll, +100000000000ll, +1000000000000ll, +10000000000000ll,
            +100000000000000l, +1000000000000000l, +10000000000000000l, +100000000000000000l, +1000000000000000000ll };

// 0= pow10ui[0]   1e19= pow10ui[20]
static const uint64_t pow10ui[]= {
            0Ull, 1Ull, 10Ull, 100Ull, 1000Ull, 10000Ull, 100000Ull, 1000000Ull, 10000000Ull,
            100000000Ull, 1000000000Ull, 10000000000Ull, 100000000000Ull, 1000000000000Ull, 10000000000000Ull, 100000000000000Ull,
            1000000000000000Ull, 10000000000000000Ull, 100000000000000000Ull, 1000000000000000000Ull, 10000000000000000000Ull };

}; // namespace mlib





























