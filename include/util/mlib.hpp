#pragma once
#define MLIB_INCLUDED 1
#include "typeShortcuts.h"


/* TODO:
  - glsl has swizzling: vec3 thirdVec = otherVec.zyy; or thirdVec= otherVec.zyw
      _maybe_ something like this can be done, but only if it's very fast, will think about it

*/




namespace mlib {

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif

#define FPII 0.159154943091895f                   // 1/ (2* PI)
#define PI_HALF 1.570796326794896619f             // PI/ 2
#define PI_DBL 6.283185307179586476925286766559f  // PI* 2
#define II_PI 0.63661977236758134307553505349006f // 2/ PI

// multiply with deg2rad or rad2deg to convert
#define DEG2RAD 0.01745329251994329576923690768489f // (PI/ 180.f)
#define RAD2DEG 57.295779513082320876798154814105f  // (180.f/ PI)
inline float degrees(float radians) { return radians* RAD2DEG; }
inline float radians(float degrees) { return degrees* DEG2RAD; }


inline int64 abs64(int64 n)  { return (n< 0? -n: n); }
inline int32 abs32(int32 n)  { return (n< 0? -n: n); }
inline float absf(float n)   { return (n< 0? -n: n); }
inline double absd(double n) { return (n< 0? -n: n); }


//#define cInt(x) ((x)> 0? (((x)- int(x)< 0.5)? int(x): int(x)+ 1) : ((int(x)- (x)< 0.5)? int(x): int(x)- 1))
inline int32 roundf(float f)  { return (int32)(f> 0.0f? (f+ 0.5f): f- 0.5f); }
inline int64 roundd(double d) { return (int64)(d> 0.0?  (d+ 0.5):  d- 0.5); }
inline int32 ceilf(float f)  { return (f>= 0.0f? int32(f)+ 1: int32(f)); }
inline int64 ceild(double d) { return (d>= 0.0 ? int64(d)+ 1: int64(d)); }
inline int32 floorf(float f)  { return (f>= 0.0f? (int32)(f): (int32)(f)- 1); }
inline int64 floord(double f) { return (f>= 0.0 ? (int64)(f): (int64)(f)- 1); }

#ifndef MAX
#define MAX(a, b)   (((a)< (b))? (b): (a))
#endif
#ifndef MIN
#define MIN(a, b)   (((a)< (b))? (a): (b))
#endif





// ===============
// = Square Root ===================================================================
// ===============

// https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
// based on the quake inv sqrt [Q_rsqrt()]

// extreem precise
inline float isqrtfPrecise(float n) {
  float nhalf= n* 0.5f;
  int32 *i= (int32 *)&n;
  *i= 0x5f3759df- (*i>> 1);     // the magic constant

  n*= (1.5f- (nhalf* n* n));    // iteration 1 <<< 3.4% error
  n*= (1.5f- (nhalf* n* n));    // iteration 2 <<< 0.17% error
  n*= (1.5f- (nhalf* n* n));    // iteration 3 <<< probly it's darn precise
  return n;
}


// 0.17% error
inline float isqrtf(float n) {
  float nhalf= n* 0.5f;
  int32 *i= (int32 *)&n;
  *i= 0x5f3759df- (*i>> 1);     // the magic constant

  n*= (1.5f- (nhalf* n* n));    // iteration 1 <<< 3.4% error
  n*= (1.5f- (nhalf* n* n));    // iteration 2 <<< 0.17% error
  return n;
}


// 3.4% error
inline float isqrtfFast(float n) {
  float nhalf= n* 0.5f;
  int32 *i= (int32 *)&n;
  *i= 0x5f3759df- (*i>> 1);     // the magic constant

  n*= (1.5f- (nhalf* n* n));    // iteration 1 <<< 3.4% error
  return n;
}


// extreem acurate
inline float sqrtfPrecise(float n) { return 1.0f/ isqrtfPrecise(n); }

// 0.17% error
inline float sqrtf(float n)        { return 1.0f/ isqrtf(n); }

// 3.4% error
inline float sqrtfFast(float n)    { return 1.0f/ isqrtfFast(n); }






// =============
// = sin / cos =----------------------------------------------------------------
// =============

// https://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b


// ~0.00109 precision, fastest func from all methods
inline float cosf(float n) {
  n*= FPII;
  n-= 0.25f+ mlib::floorf(n+ 0.25f);
  n*= 16.0f* ((n< 0.0f? -n: n)- 0.5f);       // << 0.056 precision
  n+= 0.225f* n* ((n< 0.0f? -n: n)- 1.0f);   // << 0.00109 precision
  return n;
}

// ~0.00109 precision, fastest func from all methods
inline float sinf(float n) { return mlib::cosf(n- PI_HALF); }






// TAYLOR SERIES 
// cos(x)= 1- x^2/ 2! + x^4/ 4! - x^6/ 6! + ...
// sin(x)= x- x^3/ 3! + x^5/ 5! - x^7/ 7! + ...
// varies in accuracy and speed. atm it is set to ~0.00001 accuracy

inline float sinfPrecise(float n) {
  // the func runs on -PI to +PI. <n> must be adjusted to this boundary
  //if(n< -PI || n> PI)
    n-= mlib::floorf((n+ PI)* FPII)* PI_DBL;
  /// adjust n to -PI/2 -> +PI/2 - extra precision - the further away from 0, the more the error
  if(n> PI_HALF)       n= PI- n;   
  else if(n< -PI_HALF) n= -PI- n;

  const float f3=  0.16666666666666666666666666666667f;   // 1/ 3!
  const float f5=  0.00833333333333333333333333333333f;   // 1/ 5!
  const float f7=  1.984126984126984126984126984127e-4f;  // 1/ 7!
  const float f9=  2.7557319223985890652557319223986e-6f; // 1/ 9!
  //const float f11= 2.5052108385441718775052108385442e-8f; // 1/ 11!

  float n2= n* n;
  float n3= n2* n, n5= n3* n2, n7= n5* n2;
  float n9= n7* n2;
  //float n11= n9* n2;

  // sin(x)= x- x^3/ 3! + x^5/ 5! - x^7/ 7! + ...
  //return n- (n3* f3)+ (n5* f5)- (n7* f7);                           // ~0.0001 accuracy
  return n- (n3* f3)+ (n5* f5)- (n7* f7)+ (n9* f9);                 // ~0.00001 accuracy
  //return n- (n3* f3)+ (n5* f5)- (n7* f7)+ (n9* f9)- (n11* f11);       // ~preety freaking accurate
}



inline float cosfPrecise(float n) {
  return sinfPrecise(n+ PI_HALF);
  // see cosf_Taylor() for the expanded taylor series func
}



inline float tanf(float n) {
  //tan(n)= sin(n)/ cos(n);
  float c= mlib::cosf(n);
  return c== 0.0f? 0.0f: mlib::sinf(n)/ c;
}


inline float tanfPrecise(float n) {
  //tan(n)= sin(n)/ cos(n);
  float c= mlib::cosfPrecise(n);
  return c== 0.0f? 0.0f: mlib::sinfPrecise(n)/ c;
}



struct vec2;
struct vec3;
struct vec4;
struct mat4;

struct vec2i;
struct vec3i;





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





///========
// = vec2 =---------------------------------------------------------------------
///========

struct ALIGNED(4) vec2 {
  union {
    struct { float x, y; };
    struct { float r, g; };
    float v[2];
  };
  inline int getComponents() { return 2; } /// xy

  // constructors

  inline vec2():                   x(0.0f), y(0.0f) {}
  inline vec2(float _x, float _y): x(_x),   y(_y)   {}
  inline vec2(const float *n):     x(n[0]), y(n[1]) {}
  inline vec2(float n):            x(n),    y(n)    {}
  inline vec2(const vec2 &v2):     x(v2.x), y(v2.y)  {}
  vec2(const vec3 &v2);
  vec2(const vec4 &v2);

  // operators

  inline vec2 &operator=(float n)        { x= y= n;          return *this; }
  inline vec2 &operator=(const float *n) { x= n[0], y= n[1]; return *this; }      /// array with 2 floats
  inline vec2 &operator=(const vec2 &v2) { x= v2.x, y= v2.y; return *this; }
  vec2 &operator=(const vec3 &v2);
  vec2 &operator=(const vec4 &v2);

  inline vec2 &operator+=(float n)        { x+= n, y+= n;       return *this; }
  inline vec2 &operator+=(const float *n) { x+= n[0], y+= n[1]; return *this; }     /// array with 2 floats
  inline vec2 &operator+=(const vec2 &v2) { x+= v2.x, y+= v2.y; return *this; }
  vec2 &operator+=(const vec3 &v2);
  vec2 &operator+=(const vec4 &v2);

  inline vec2 &operator-=(float n)        { x-= n, y-= n;       return *this; }
  inline vec2 &operator-=(const float *n) { x-= n[0], y-= n[1]; return *this; }      /// array with 2 floats
  inline vec2 &operator-=(const vec2 &v2) { x-= v2.x, y-= v2.y; return *this; }
  vec2 &operator-=(const vec3 &v2);
  vec2 &operator-=(const vec4 &v2);

  inline vec2 &operator*=(float n)        { x*= n, y*= n;       return *this; }
  inline vec2 &operator*=(const float *n) { x*= n[0], y*= n[1]; return *this; }     /// array with 2 floats
  inline vec2 &operator*=(const vec2 &v2) { x*= v2.x, y*= v2.y; return *this; }
  vec2 &operator*=(const vec3 &v2);
  vec2 &operator*=(const vec4 &v2);

  inline vec2 &operator/=(float n)        { x/= n, y/= n;       return *this; }
  inline vec2 &operator/=(const float *n) { x/= n[0], y/= n[1]; return *this; }     /// array with 2 floats
  inline vec2 &operator/=(const vec2 &v2) { x/= v2.x, y/= v2.y; return *this; }
  vec2 &operator/=(const vec3 &v2);
  vec2 &operator/=(const vec4 &v2);

  inline const vec2 operator+(float n)        const { return vec2(*this)+= n; }
  inline const vec2 operator+(const float *n) const { return vec2(*this)+= n; }
  inline const vec2 operator+(const vec2 &v2) const { return vec2(*this)+= v2; }
  const vec2 operator+(const vec3 &v2) const;
  const vec2 operator+(const vec4 &v2) const;

  inline const vec2 operator-(float n)        const { return vec2(*this)-= n; }
  inline const vec2 operator-(const float *n) const { return vec2(*this)-= n; }
  inline const vec2 operator-(const vec2 &v2) const { return vec2(*this)-= v2; }
  const vec2 operator-(const vec3 &v2) const;
  const vec2 operator-(const vec4 &v2) const;

  inline const vec2 operator*(float n)        const { return vec2(*this)*= n; }
  inline const vec2 operator*(const float *n) const { return vec2(*this)*= n; }
  inline const vec2 operator*(const vec2 &v2) const { return vec2(*this)*= v2; }
  const vec2 operator*(const vec3 &v2) const;
  const vec2 operator*(const vec4 &v2) const;

  inline const vec2 operator/(float n)        const { return vec2(*this)/= n; }
  inline const vec2 operator/(const float *n) const { return vec2(*this)/= n; }
  inline const vec2 operator/(const vec2 &v2) const { return vec2(*this)/= v2; }
  const vec2 operator/(const vec3 &v2) const;
  const vec2 operator/(const vec4 &v2) const;

  inline bool operator==(float n)        const { return (n== length()); }   // this compares vector's length, not each element to n
  inline bool operator==(const float *n) const { return ((x== n[0]) && (y== n[1])); }
  inline bool operator==(const vec2 &v2) const { return ((x== v2.x) && (y== v2.y)); }
  bool operator==(const vec3 &v2) const;
  bool operator==(const vec4 &v2) const;
  
  inline bool operator!=(float n)        const { return (n!= length()); }    // this compares vector's length, not each element to n
  inline bool operator!=(const float *n) const { return ((x!= n[0]) || (y!= n[1])); }
  inline bool operator!=(const vec2 &v2) const { return ((x!= v2.x) || (y!= v2.y)); }
  bool operator!=(const vec3 &v2) const;
  bool operator!=(const vec4 &v2) const;

  inline bool operator!() const { return (x== 0.0f) && (y== 0.0f); }
  
  inline const vec2 operator-() const { return vec2(-x, -y); }

  //inline float &operator[](int i)            { return v[i]; }
  inline const float operator[](int i) const { return v[i]; }
  inline operator float* ()                  { return v; }
  inline operator const float *()      const { return v; }

  // dot product

  inline float dot(float _x, float _y) const { return (x* _x)+ (y* _y); }
  inline float dot(const float *n)     const { return this->dot(n[0], n[1]); }
  inline float dot(const vec2 &v2)     const { return this->dot(v2.x, v2.y); }
  float dot(const vec3 &v2) const;
  float dot(const vec4 &v2) const;

  // rest of funcs

  inline vec2 &normalize() { float size= length(); if(size) this->operator/=(size); return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option
  inline float length() const { return sqrtf((x* x)+ (y* y)); }
  inline float norm2() { return (x* x) + (y* y); }
};


inline const vec2 operator+(float n, const vec2 &v) { return vec2(n+ v.x, n+ v.y); }
inline const vec2 operator-(float n, const vec2 &v) { return vec2(n- v.x, n- v.y); }
inline const vec2 operator*(float n, const vec2 &v) { return vec2(n* v.x, n* v.y); }
inline const vec2 operator/(float n, const vec2 &v) { return vec2(n/ v.x, n/ v.y); }






// =========
// = vec2i =--------------------------------------------------------------------
// =========

struct ALIGNED(4) vec2i {
  union {
    struct { int32 x, y; };
    struct { int32 r, g; };
    int32 v[2];
  };
  inline int getComponents() { return 2; } /// xy

  // constructors

  inline vec2i():                   x(0),    y(0) {}
  inline vec2i(int32 _x, int32 _y): x(_x),   y(_y)   {}
  inline vec2i(const int32 *n):     x(n[0]), y(n[1]) {}
  inline vec2i(int32 n):            x(n),    y(n)    {}
  inline vec2i(const vec2i &v2):    x(v2.x), y(v2.y) {}
  vec2i(const vec3i &v2);
  //vec2i(const vec4 &v2);

  // operators

  inline vec2i &operator=(int32 n)         { x= y= n;          return *this; }
  inline vec2i &operator=(const int32 *n)  { x= n[0], y= n[1]; return *this; }      /// array with 2 floats
  inline vec2i &operator=(const vec2i &v2) { x= v2.x, y= v2.y; return *this; }
  vec2i &operator=(const vec3i &v2);
  //vec2i &operator=(const vec4i &v2);

  inline vec2i &operator+=(int32 n)         { x+= n,    y+= n;    return *this; }
  inline vec2i &operator+=(const int32 *n)  { x+= n[0], y+= n[1]; return *this; }     /// array with 2 floats
  inline vec2i &operator+=(const vec2i &v2) { x+= v2.x, y+= v2.y; return *this; }
  vec2i &operator+=(const vec3i &v2);
  //vec2i &operator+=(const vec4i &v2);

  inline vec2i &operator-=(int32 n)         { x-= n,    y-= n;    return *this; }
  inline vec2i &operator-=(const int32 *n)  { x-= n[0], y-= n[1]; return *this; }      /// array with 2 floats
  inline vec2i &operator-=(const vec2i &v2) { x-= v2.x, y-= v2.y; return *this; }
  vec2i &operator-=(const vec3i &v2);
  //vec2i &operator-=(const vec4i &v2);

  inline vec2i &operator*=(int32 n)         { x*= n,    y*= n;    return *this; }
  inline vec2i &operator*=(const int32 *n)  { x*= n[0], y*= n[1]; return *this; }     /// array with 2 floats
  inline vec2i &operator*=(const vec2i &v2) { x*= v2.x, y*= v2.y; return *this; }
  vec2i &operator*=(const vec3i &v2);
  //vec2i &operator*=(const vec4i &v2);

  inline vec2i &operator/=(int32 n)         { x/= n,    y/= n;    return *this; }
  inline vec2i &operator/=(const int32 *n)  { x/= n[0], y/= n[1]; return *this; }     /// array with 2 floats
  inline vec2i &operator/=(const vec2i &v2) { x/= v2.x, y/= v2.y; return *this; }
  vec2i &operator/=(const vec3i &v2);
  //vec2i &operator/=(const vec4i &v2);

  inline const vec2i operator+(int32 n)         const { return vec2i(*this)+= n; }
  inline const vec2i operator+(const int32 *n)  const { return vec2i(*this)+= n; }
  inline const vec2i operator+(const vec2i &v2) const { return vec2i(*this)+= v2; }
  const vec2i operator+(const vec3i &v2) const;
  //const vec2i operator+(const vec4i &v2) const;

  inline const vec2i operator-(int32 n)         const { return vec2i(*this)-= n; }
  inline const vec2i operator-(const int32 *n)  const { return vec2i(*this)-= n; }
  inline const vec2i operator-(const vec2i &v2) const { return vec2i(*this)-= v2; }
  const vec2i operator-(const vec3i &v2) const;
  //const vec2i operator-(const vec4i &v2) const;

  inline const vec2i operator*(int32 n)         const { return vec2i(*this)*= n; }
  inline const vec2i operator*(const int32 *n)  const { return vec2i(*this)*= n; }
  inline const vec2i operator*(const vec2i &v2) const { return vec2i(*this)*= v2; }
  const vec2i operator*(const vec3i &v2) const;
  //const vec2i operator*(const vec4i &v2) const;

  inline const vec2i operator/(int32 n)         const { return vec2i(*this)/= n; }
  inline const vec2i operator/(const int32 *n)  const { return vec2i(*this)/= n; }
  inline const vec2i operator/(const vec2i &v2) const { return vec2i(*this)/= v2; }
  const vec2i operator/(const vec3i &v2) const;
  //const vec2i operator/(const vec4i &v2) const;

  inline bool operator==(int32 n)         const { return (n== length()); }   // this compares vector's length, not each element to n
  inline bool operator==(const int32 *n)  const { return ((x== n[0]) && (y== n[1])); }
  inline bool operator==(const vec2i &v2) const { return ((x== v2.x) && (y== v2.y)); }
  bool operator==(const vec3i &v2) const;
  //bool operator==(const vec4i &v2) const;
  
  inline bool operator!=(int32 n)         const { return (n!= length()); }    // this compares vector's length, not each element to n
  inline bool operator!=(const int32 *n)  const { return ((x!= n[0]) || (y!= n[1])); }
  inline bool operator!=(const vec2i &v2) const { return ((x!= v2.x) || (y!= v2.y)); }
  bool operator!=(const vec3i &v2) const;
  //bool operator!=(const vec4i &v2) const;

  inline bool operator!() const { return (x== 0.0f) && (y== 0.0f); }
  
  inline const vec2i operator-() const { return vec2i(-x, -y); }

  //inline int32 &operator[](int i)            { return v[i]; }
  inline const int32 operator[](int i) const { return v[i]; }
  inline operator int32* ()                  { return v; }
  inline operator const int32 *()      const { return v; }

  // dot product

  inline int32 dot(int32 _x, int32 _y) const { return (x* _x)+ (y* _y); }
  inline int32 dot(const int32 *n)     const { return this->dot(n[0], n[1]); }
  inline int32 dot(const vec2i &v2)     const { return this->dot(v2.x, v2.y); }
  int32 dot(const vec3i &v2) const;
  //int32 dot(const vec4i &v2) const;

  // rest of funcs
  inline vec2i &set(int32 in_x, int32 in_y) { x= in_x, y= in_y; return *this; }
  inline vec2i &normalize() { int32 size= length(); if(size) this->operator/=(size); return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option
  inline int32 length() const { return roundf(sqrtf((float)((x* x)+ (y* y)))); }
  inline int32 norm2() { return (x* x) + (y* y); }
};


inline const vec2i operator+(int32 n, const vec2i &v) { return vec2i(n+ v.x, n+ v.y); }
inline const vec2i operator-(int32 n, const vec2i &v) { return vec2i(n- v.x, n- v.y); }
inline const vec2i operator*(int32 n, const vec2i &v) { return vec2i(n* v.x, n* v.y); }
inline const vec2i operator/(int32 n, const vec2i &v) { return vec2i(n/ v.x, n/ v.y); }








// ========
// = vec3 =---------------------------------------------------------------------
// ========

struct ALIGNED(4) vec3 {
  union {
    struct { float x, y, z; };
    struct { float r, g, b; };
    float v[3];
  };
  inline int getComponents() { return 3; } /// xyz

  // constructors

  inline vec3():                             x(0.0f), y(0.0f), z(0.0f) {}
  inline vec3(float _x, float _y, float _z): x(_x),   y(_y),   z(_z)   {}
  inline vec3(const float *n):               x(n[0]), y(n[1]), z(n[2]) {}
  inline vec3(float n):                      x(n),    y(n),    z(n)    {}
  inline vec3(const vec2 &v2, float _z= 0):  x(v2.x), y(v2.y), z(_z)   {}
  inline vec3(const vec3 &v2):               x(v2.x), y(v2.y), z(v2.z) {}
  vec3(const vec4 &v2);

  // operators

  inline vec3 &operator=(float n)        { x= y= z= n;                return *this; }
  inline vec3 &operator=(const float *n) { x= n[0], y= n[1], z= n[2]; return *this; }
  inline vec3 &operator=(const vec2 &v2) { x= v2.x, y= v2.y, z= 0.0f; return *this; }
  inline vec3 &operator=(const vec3 &v2) { x= v2.x, y= v2.y, z= v2.z; return *this; }
  vec3 &operator=(const vec4 &v2);

  inline vec3 &operator+=(float n)        { x+= n, y+= n, z+= n;          return *this; }
  inline vec3 &operator+=(const float *n) { x+= n[0], y+= n[1], z+= n[2]; return *this; }
  inline vec3 &operator+=(const vec2 &v2) { x+= v2.x, y+= v2.y;           return *this; }
  inline vec3 &operator+=(const vec3 &v2) { x+= v2.x, y+= v2.y, z+= v2.z; return *this; }
  vec3 &operator+=(const vec4 &v2);

  inline vec3 &operator-=(float n)        { x-= n, y-= n, z-= n;          return *this; }
  inline vec3 &operator-=(const float *n) { x-= n[0], y-= n[1], z-= n[2]; return *this; }
  inline vec3 &operator-=(const vec2 &v2) { x-= v2.x, y-= v2.y;           return *this; }
  inline vec3 &operator-=(const vec3 &v2) { x-= v2.x, y-= v2.y, z-= v2.z; return *this; }
  vec3 &operator-=(const vec4 &v2);

  inline vec3 &operator*=(float n)        { x*= n, y*= n, z*= n;          return *this; }
  inline vec3 &operator*=(const float *n) { x*= n[0], y*= n[1], z*= n[2]; return *this; }
  inline vec3 &operator*=(const vec2 &v2) { x*= v2.x, y*= v2.y;           return *this; }
  inline vec3 &operator*=(const vec3 &v2) { x*= v2.x, y*= v2.y, z*= v2.z; return *this; }
  vec3 &operator*=(const vec4 &v2);
  
  inline vec3 &operator/=(float n)        { x/= n, y/= n, z/= n;          return *this; }
  inline vec3 &operator/=(const float *n) { x/= n[0], y/= n[1], z/= n[2]; return *this; }
  inline vec3 &operator/=(const vec2 &v2) { x/= v2.x, y/= v2.y;           return *this; }
  inline vec3 &operator/=(const vec3 &v2) { x/= v2.x, y/= v2.y, z/= v2.z; return *this; }
  vec3 &operator/=(const vec4 &v2);
  
  inline const vec3 operator+(float n)        const { return vec3(*this)+= n; }
  inline const vec3 operator+(const float *n) const { return vec3(*this)+= n; }
  inline const vec3 operator+(const vec2 &v2) const { return vec3(*this)+= v2; }
  inline const vec3 operator+(const vec3 &v2) const { return vec3(*this)+= v2; }
  const vec3 operator+(const vec4 &v2) const;
  

  inline const vec3 operator-(float n)        const { return vec3(*this)-= n; }
  inline const vec3 operator-(const float *n) const { return vec3(*this)-= n; }
  inline const vec3 operator-(const vec2 &v2) const { return vec3(*this)-= v2; }
  inline const vec3 operator-(const vec3 &v2) const { return vec3(*this)-= v2; }
  const vec3 operator-(const vec4 &v2) const;

  inline const vec3 operator*(float n)        const { return vec3(*this)*= n; }
  inline const vec3 operator*(const float *n) const { return vec3(*this)*= n; }
  inline const vec3 operator*(const vec2 &v2) const { return vec3(*this)*= v2; }
  inline const vec3 operator*(const vec3 &v2) const { return vec3(*this)*= v2; }
  const vec3 operator*(const vec4 &v2) const;

  inline const vec3 operator/(float n)        const { return vec3(*this)/= n; }
  inline const vec3 operator/(const float *n) const { return vec3(*this)/= n; }
  inline const vec3 operator/(const vec2 &v2) const { return vec3(*this)/= v2; }
  inline const vec3 operator/(const vec3 &v2) const { return vec3(*this)/= v2; }
  const vec3 operator/(const vec4 &v2) const;

  inline bool operator==(float n)        const { return n== length(); } // this compares vector's length, not each element with n
  inline bool operator==(const float *n) const { return ((x== n[0]) && (y== n[1]) && (z== n[2])); }
  inline bool operator==(const vec2 &v2) const { return ((x== v2.x) && (y== v2.y)); }
  inline bool operator==(const vec3 &v2) const { return ((x== v2.x) && (y== v2.y) && (z== v2.z)); }
  bool operator==(const vec4 &v2) const;
  
  inline bool operator!=(float n)        const { return n!= length(); } // this compares vector's length, not each element with n
  inline bool operator!=(const float *n) const { return ((x!= n[0]) || (y!= n[1]) || (z!= n[2])); }
  inline bool operator!=(const vec2 &v2) const { return ((x!= v2.x) || (y!= v2.y)); }
  inline bool operator!=(const vec3 &v2) const { return ((x!= v2.x) || (y!= v2.y) || (z!= v2.z)); }
  bool operator!=(const vec4 &v2) const;
  
  inline bool operator!() const { return ((x== 0.0f) && (y== 0.0f) && (z== 0.0f)); }

  inline const vec3 operator-() const { return vec3(-x, -y, -z); }

  float &operator[](int i)            { return v[i]; }
  const float operator[](int i) const { return v[i]; }
  operator float* ()                  { return v; }
  operator const float *() const      { return v; }

  // dot product

  inline float dot(float _x, float _y, float _z)   const { return (x* _x)+ (y* _y)+ (z* _z); }
  inline float dot(const float *n)                 const { return this->dot(n[0], n[1], n[2]); }
  inline float dot(const vec2 &v2, float _z= 0.0f) const { return this->dot(v2.x, v2.y, _z); }
  inline float dot(const vec3 &v2)                 const { return this->dot(v2.x, v2.y, v2.z); }
  float dot(const vec4 &v2) const;

  // cross product

  inline vec3 cross(float _x, float _y, float _z) const { return vec3((y* _z)- (z* _y), (z* _x)- (x* _z), (x* _y)- (y* _x)); }
  inline vec3 cross(const vec2 &v2, float _z)     const { return this->cross(v2.x, v2.y,   _z); }
  inline vec3 cross(const vec3 &v2)               const { return this->cross(v2.x, v2.y, v2.z); }
  vec3 cross(const vec4 &v2) const;
  
  // funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl

  inline vec3 &normalize() { float size= length(); if(size) { this->operator/=(size); } return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option
  inline float length() const { return sqrtf((x* x)+ (y* y)+ (z* z)); };
  inline float norm2() const { return (x* x) + (y* y) + (z* z); }

  inline void addConstant(float n) {
    float xx= x* x, yy= y* y, zz= z* z, size= length()+ n;
    if(x!= 0.0f) x= (absf(x)/ x)* size/ sqrtf((yy/ xx)+ (zz/ xx)+ 1.0f);
    if(y!= 0.0f) y= (absf(y)/ y)* size/ sqrtf((xx/ yy)+ (zz/ yy)+ 1.0f);
    if(z!= 0.0f) z= (absf(z)/ z)* size/ sqrtf((xx/ zz)+ (yy/ zz)+ 1.0f);
  };
};

inline const vec3 operator+(float n, const vec3 &v) { return vec3(n+ v.x, n+ v.y, n+ v.z); }
inline const vec3 operator-(float n, const vec3 &v) { return vec3(n- v.x, n- v.y, n- v.z); }
inline const vec3 operator*(float n, const vec3 &v) { return vec3(n* v.x, n* v.y, n* v.z); }
inline const vec3 operator/(float n, const vec3 &v) { return vec3(n/ v.x, n/ v.y, n/ v.z); }









///=========
// = vec3i =--------------------------------------------------------------------
///=========


struct ALIGNED(4) vec3i {
  union {
    struct { int32 x, y, z; };
    struct { int32 r, g, b; };
    int32 v[3];
  };
  inline int getComponents() { return 3; } /// xyz

  // constructors

  inline vec3i():                             x(0),    y(0),    z(0)    {}
  inline vec3i(int32 _x, int32 _y, int32 _z): x(0),    y(0),    z(0)    {}
  inline vec3i(const int32 *n):               x(n[0]), y(n[1]), z(n[2]) {}
  inline vec3i(int32 n):                      x(n),    y(n),    z(n)    {}
  inline vec3i(const vec2i &v2, int32 _z= 0): x(v2.x), y(v2.y), z(_z)   {}
  inline vec3i(const vec3i &v2):              x(v2.x), y(v2.y), z(v2.z) {}
  //vec3i(const vec4i &v2);

  // operators

  inline vec3i &operator=(int32 n)         { x= y= z= n;                return *this; }
  inline vec3i &operator=(const int32 *n)  { x= n[0], y= n[1], z= n[2]; return *this; }
  inline vec3i &operator=(const vec2i &v2) { x= v2.x, y= v2.y;          return *this; }
  inline vec3i &operator=(const vec3i &v2) { x= v2.x, y= v2.y, z= v2.y; return *this; }
  //vec3i &operator=(const vec4i &v2);
  
  inline vec3i &set(int32 in_x, int32 in_y, int32 in_z) { x= in_x, y= in_y, z= in_z; return *this; }

  inline vec3i &operator+=(int32 n)         { x+= n,    y+= n,    z+= n;    return *this; }
  inline vec3i &operator+=(const int32 *n)  { x+= n[0], y+= n[1], z+= n[2]; return *this; }
  inline vec3i &operator+=(const vec2i &v2) { x+= v2.x, y+= v2.y;           return *this; }
  inline vec3i &operator+=(const vec3i &v2) { x+= v2.x, y+= v2.y, z+= v2.z; return *this; }
  //vec3i &operator+=(const vec4i &v2);
  
  inline vec3i &operator-=(int32 n)         { x-= n,    y-= n,    z-= n;    return *this; }
  inline vec3i &operator-=(const int32 *n)  { x-= n[0], y-= n[1], z-= n[2]; return *this; }
  inline vec3i &operator-=(const vec2i &v2) { x-= v2.x, y-= v2.y;           return *this; }
  inline vec3i &operator-=(const vec3i &v2) { x-= v2.x, y-= v2.y, z-= v2.z; return *this; }
  //vec3i &operator-=(const vec4i &v2);
  
  inline vec3i &operator*=(int32 n)         { x*= n,    y*= n,    z*= n;    return *this; }
  inline vec3i &operator*=(const int32 *n)  { x*= n[0], y*= n[1], z*= n[2]; return *this; }
  inline vec3i &operator*=(const vec2i &v2) { x*= v2.x, y*= v2.y;           return *this; }
  inline vec3i &operator*=(const vec3i &v2) { x*= v2.x, y*= v2.y, z*= v2.z; return *this; }
  //vec3i &operator*=(const vec4i &v2);

  inline vec3i &operator/=(int32 n)         { x/= n,    y/= n,    z/= n;    return *this; }
  inline vec3i &operator/=(const int32 *n)  { x/= n[0], y/= n[1], z/= n[2]; return *this; }
  inline vec3i &operator/=(const vec2i &v2) { x/= v2.x, y/= v2.y;           return *this; }
  inline vec3i &operator/=(const vec3i &v2) { x/= v2.x, y/= v2.y, z/= v2.z; return *this; }
  //vec3i &operator/=(const vec4i &v2);

  inline const vec3i operator+(int32 n)         const { return vec3i(*this)+= n; }
  inline const vec3i operator+(const int32 *n)  const { return vec3i(*this)+= n; }
  inline const vec3i operator+(const vec2i &v2) const { return vec3i(*this)+= v2; }
  inline const vec3i operator+(const vec3i &v2) const { return vec3i(*this)+= v2; }
  //const vec3i operator+(const vec4i &v2) const;

  inline const vec3i operator-(int32 n)         const { return vec3i(*this)-= n; }
  inline const vec3i operator-(const int32 *n)  const { return vec3i(*this)-= n; }
  inline const vec3i operator-(const vec2i &v2) const { return vec3i(*this)-= v2; }
  inline const vec3i operator-(const vec3i &v2) const { return vec3i(*this)-= v2; }
  //const vec3i operator-(const vec4i &v2) const;

  inline const vec3i operator*(int32 n)         const { return vec3i(*this)*= n; }
  inline const vec3i operator*(const int32 *n)  const { return vec3i(*this)*= n; }
  inline const vec3i operator*(const vec2i &v2) const { return vec3i(*this)*= v2; }
  inline const vec3i operator*(const vec3i &v2) const { return vec3i(*this)*= v2; }
  //const vec3i operator*(const vec4i &v2) const;

  inline const vec3i operator/(int32 n)         const { return vec3i(*this)/= n; }
  inline const vec3i operator/(const int32 *n)  const { return vec3i(*this)/= n; }
  inline const vec3i operator/(const vec2i &v2) const { return vec3i(*this)/= v2; }
  inline const vec3i operator/(const vec3i &v2) const { return vec3i(*this)/= v2; }
  //const vec3i operator/(const vec4i &v2) const;

  inline bool operator==(int32 n)         const { return (n== length()); } // this compares vector's length, not each element with n
  inline bool operator==(const int32 *n)  const { return ((x== n[0]) && (y== n[1]) && (z== n[2])); }
  inline bool operator==(const vec2i &v2) const { return ((x== v2.x) && (y== v2.y)); }
  inline bool operator==(const vec3i &v2) const { return ((x== v2.x) && (y== v2.y) && (z== v2.z)); }
  //bool operator==(const vec4i &v2) const;

  inline bool operator!=(int32 n)         const { return (n!= length());} // this compares vector's length, not each element with n
  inline bool operator!=(const int32 *n)  const { return ((x!= n[0]) || (y!= n[1]) || (z!= n[2])); }
  inline bool operator!=(const vec2i &v2) const { return ((x!= v2.x) || (y!= v2.y)); }
  inline bool operator!=(const vec3i &v2) const { return ((x!= v2.x) || (y!= v2.y) || (z!= v2.z)); }
  //bool operator!=(const vec4i &v2)   const;

  inline bool operator!() const { return !(x && y); }

  inline const vec3i operator-() const { return vec3i(-x, -y, -z); }

  inline int32 &operator[](int i)            { return v[i]; }
  inline const int32 operator[](int i) const { return v[i]; }
  inline operator int32 *()                  { return v; }
  inline operator const int32 *()      const { return v; }

  // dot product

  inline int32 dot(int32 _x, int32 _y, int32 _z) const { return (x* _x)+  (y* _y)+ (z* _z); }
  inline int32 dot(const int32 *n)               const { return this->dot(n[0], n[1], n[2]); }
  inline int32 dot(const vec2i &v2, int32 _z= 0) const { return this->dot(v2.x, v2.y, _z); }
  inline int32 dot(const vec3i &v2)              const { return this->dot(v2.x, v2.y, v2.z); }
  //inline int32 dot(const vec4i &v2)              const { return this->dot(v2.x, v2.y, v2.z); }

  // cross product

  inline vec3i cross(int32 _x, int32 _y, int32 _z) const { return vec3i((y* _z)- (z* _y),  (z* _x)- (x* _z), (x* _y)- (y* _x)); }
  inline vec3i cross(const vec2i &v2, int32 _z)    const { return this->cross(v2.x, v2.y,   _z); }
  inline vec3i cross(const vec3i &v2)              const { return this->cross(v2.x, v2.y, v2.z); }
  //inline vec3i vec3i::cross(const vec4i &v2)              const { return this->cross(v2.x, v2.y, v2.z); }

  // funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl

  inline vec3i &normalize() { int32 size= length(); if(size) this->operator/= (size); return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option

  inline int32 length() const { return roundf(sqrtf((float)((x* x)+ (y* y)+ (z* z)))); };
  inline int32 norm2() const { return (x* x)+ (y* y)+ (z* z); }

  inline void addConstant(int32 n) {
    int32 xx= x* x, yy= y* y, zz= z* z, size= length()+ n;
    if(x!= 0) x= (abs32(x)/ x)* size/ roundf(sqrtf(yy/ xx+ zz/ xx+ 1.0f));
    if(y!= 0) y= (abs32(y)/ y)* size/ roundf(sqrtf(xx/ yy+ zz/ yy+ 1.0f));
    if(z!= 0) z= (abs32(z)/ z)* size/ roundf(sqrtf(xx/ zz+ yy/ zz+ 1.0f));
  };
};


inline const vec3i operator+(int32 n, const vec3i &v) { return vec3i(n+ v.x, n+ v.y, n+ v.z); }
inline const vec3i operator-(int32 n, const vec3i &v) { return vec3i(n- v.x, n- v.y, n- v.z); }
inline const vec3i operator*(int32 n, const vec3i &v) { return vec3i(n* v.x, n* v.y, n* v.z); }
inline const vec3i operator/(int32 n, const vec3i &v) { return vec3i(n/ v.x, n/ v.y, n/ v.z); }
















///========
// = vec4 =---------------------------------------------------------------------
///========


struct ALIGNED(4) vec4 {    // alignment of 16 if in the future intrinsics will be used
  union {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    float v[4];
  };
  inline int getComponents() { return 4; } /// xyzw

  // constructors
  
  inline vec4():                                         x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
  inline vec4(float _x, float _y, float _z, float _w):   x(_x),   y(_y),   z(_z),   w(_w)   {}
  inline vec4(float n):                                  x(n),    y(n),    z(n),    w(n)    {}
  inline vec4(const float *n):                           x(n[0]), y(n[1]), z(n[2]), w(n[3]) {}
  inline vec4(const vec2 &v2, float _z= 0, float _w= 0): x(v2.x), y(v2.y), z(_z),   w(_w)   {}
  inline vec4(const vec3 &v2, float _w= 0):              x(v2.x), y(v2.y), z(v2.z), w(_w)   {}
  inline vec4(const vec4 &v2):                           x(v2.x), y(v2.y), z(v2.z), w(v2.w) {}

  // operators

  inline vec4 &operator=(float n)        { x= y= z= w= n;                      return *this; }
  inline vec4 &operator=(const float *n) { x= n[0], y= n[1], z= n[2], w= n[3]; return *this; }
  inline vec4 &operator=(const vec2 &v2) { x= v2.x, y= v2.y;                   return *this; }
  inline vec4 &operator=(const vec3 &v2) { x= v2.x, y= v2.y, z= v2.z;          return *this; }
  inline vec4 &operator=(const vec4 &v2) { x= v2.x, y= v2.y, z= v2.z, w= v2.w; return *this; }
   
  inline vec4 &operator+=(float n)        { x+= n,    y+= n,    z+= n,    w+= n;    return *this; }
  inline vec4 &operator+=(const float *n) { x+= n[0], y+= n[1], z+= n[2], w+= n[3]; return *this; }
  inline vec4 &operator+=(const vec2 &v2) { x+= v2.x, y+= v2.y;                     return *this; }
  inline vec4 &operator+=(const vec3 &v2) { x+= v2.x, y+= v2.y, z+= v2.z;           return *this; }
  inline vec4 &operator+=(const vec4 &v2) { x+= v2.x, y+= v2.y, z+= v2.z, w+= v2.w; return *this; }

  inline vec4 &operator-=(float n)        { x-= n,    y-= n,    z-= n,    w-= n;    return *this; }
  inline vec4 &operator-=(const float *n) { x-= n[0], y-= n[1], z-= n[2], w-= n[3]; return *this; }
  inline vec4 &operator-=(const vec2 &v2) { x-= v2.x, y-= v2.y;                     return *this; }
  inline vec4 &operator-=(const vec3 &v2) { x-= v2.x, y-= v2.y, z-= v2.z;           return *this; }
  inline vec4 &operator-=(const vec4 &v2) { x-= v2.x, y-= v2.y, z-= v2.z, w-= v2.w; return *this; }

  inline vec4 &operator*=(float n)        { x*= n,    y*= n,    z*= n,    w*= n;    return *this; }
  inline vec4 &operator*=(const float *n) { x*= n[0], y*= n[1], z*= n[2], w*= n[3]; return *this; }
  inline vec4 &operator*=(const vec2 &v2) { x*= v2.x, y*= v2.y;                     return *this; }
  inline vec4 &operator*=(const vec3 &v2) { x*= v2.x, y*= v2.y, z*= v2.z;           return *this; }
  inline vec4 &operator*=(const vec4 &v2) { x*= v2.x, y*= v2.y, z*= v2.z, w*= v2.w; return *this; }

  inline vec4 &operator/=(float n)        { x/= n,    y/= n,    z/= n,    w/= n;    return *this; }
  inline vec4 &operator/=(const float *n) { x/= n[0], y/= n[1], z/= n[2], w/= n[3]; return *this; }
  inline vec4 &operator/=(const vec2 &v2) { x/= v2.x, y/= v2.y;                     return *this; }
  inline vec4 &operator/=(const vec3 &v2) { x/= v2.x, y/= v2.y, z/= v2.z;           return *this; }
  inline vec4 &operator/=(const vec4 &v2) { x/= v2.x, y/= v2.y, z/= v2.z, w/= v2.w; return *this; }
  
  inline const vec4 operator+(float n)        const { return vec4(*this)+= n; }
  inline const vec4 operator+(const float *n) const { return vec4(*this)+= n; }
  inline const vec4 operator+(const vec2 &v2) const { return vec4(*this)+= v2; }
  inline const vec4 operator+(const vec3 &v2) const { return vec4(*this)+= v2; }
  inline const vec4 operator+(const vec4 &v2) const { return vec4(*this)+= v2; }

  inline const vec4 operator-(float n)        const { return vec4(*this)-= n; }
  inline const vec4 operator-(const float *n) const { return vec4(*this)-= n; }
  inline const vec4 operator-(const vec2 &v2) const { return vec4(*this)-= v2; }
  inline const vec4 operator-(const vec3 &v2) const { return vec4(*this)-= v2; }
  inline const vec4 operator-(const vec4 &v2) const { return vec4(*this)-= v2; }

  inline const vec4 operator*(float n)        const { return vec4(*this)*= n; }
  inline const vec4 operator*(const float *n) const { return vec4(*this)*= n; }
  inline const vec4 operator*(const vec2 &v2) const { return vec4(*this)*= v2; }
  inline const vec4 operator*(const vec3 &v2) const { return vec4(*this)*= v2; }
  inline const vec4 operator*(const vec4 &v2) const { return vec4(*this)*= v2; }

  inline const vec4 operator/(float n)        const { return vec4(*this)/= n; }
  inline const vec4 operator/(const float *n) const { return vec4(*this)/= n; }
  inline const vec4 operator/(const vec2 &v2) const { return vec4(*this)/= v2; }
  inline const vec4 operator/(const vec3 &v2) const { return vec4(*this)/= v2; }
  inline const vec4 operator/(const vec4 &v2) const { return vec4(*this)/= v2; }

  inline bool operator==(float n)        const { return (n== length()); }
  inline bool operator==(const vec2 &v2) const { return ((x== v2.x) && (y== v2.y)); }
  inline bool operator==(const vec3 &v2) const { return ((x== v2.x) && (y== v2.y) && (z== v2.z)); }
  inline bool operator==(const vec4 &v2) const { return ((x== v2.x) && (y== v2.y) && (z== v2.z) && (w== v2.w)); }
  inline bool operator==(const float *n) const { return ((x== n[0]) && (y== n[1]) && (z== n[2]) && (w== n[3])); }

  inline bool operator!=(float n)        const { return (n!= length()); }
  inline bool operator!=(const float *n) const { return ((x!= n[0]) || (y!= n[1]) || (z!= n[2]) || (w!= n[3])); }
  inline bool operator!=(const vec2 &v2) const { return ((x!= v2.x) || (y!= v2.y)); }
  inline bool operator!=(const vec3 &v2) const { return ((x!= v2.x) || (y!= v2.y) || (z!= v2.z)); }
  inline bool operator!=(const vec4 &v2) const { return ((x!= v2.x) || (y!= v2.y) || (z!= v2.z) || (w!= v2.w)); }
  
  inline bool operator!() const { return (x!= 0.0f) && (y!= 0.0f) && (z!= 0.0f) && (w!= 0.0f); }

  inline const vec4 operator-() const { return vec4(-x, -y, -z, -w); }

  inline float &operator[](int i)            { return v[i]; }
  inline const float operator[](int i) const { return v[i]; }
  inline operator float* ()                  { return v; }
  inline operator const float *()      const { return v; }

  // dot product

  inline float dot(float _x, float _y, float _z, float _w)         const { return (x* _x)+ (y* _y)+ (z* _z)+ (w* _w); }
  inline float dot(const float *n)                                 const { return this->dot(n[0], n[1], n[2], n[3]); }
  inline float dot(const vec2 &v2, float _z= 0.0f, float _w= 0.0f) const { return this->dot(v2.x, v2.y,   _z,   _w); }
  inline float dot(const vec3 &v2, float _w= 0.0f)                 const { return this->dot(v2.x, v2.y, v2.z,   _w); }
  inline float dot(const vec4 &v2)                                 const { return this->dot(v2.x, v2.y, v2.z, v2.w); }

  // funcs - research: https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl

  inline vec4 &setColoru8(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) { return set(((float)_r)/ 255.0f, ((float)_g)/ 255.0f, ((float)_b)/ 255.0f, ((float)_a)/ 255.0f); }
  inline vec4 &set(float _x, float _y, float _z, float _w) { x= _x, y= _y, z= _z, w= _w; return *this; }
  inline vec4 &normalize() { float size= length(); if(size) { this->operator/=(size); } return *this; } // return vec* inversesqrt(dot(vec, vec)); - another option
  inline float length() const { return sqrtf((x* x)+ (y* y)+ (z* z)+ (w* w)); };
  inline float norm2() const { return (x* x)+ (y* y)+ (z* z)+ (w* w); }
};

inline const vec4 operator+(float n, const vec4 &v) { return vec4(n+ v.x, n+ v.y, n+ v.z, n+ v.w); }
inline const vec4 operator-(float n, const vec4 &v) { return vec4(n- v.x, n- v.y, n- v.z, n- v.w); }
inline const vec4 operator*(float n, const vec4 &v) { return vec4(n* v.x, n* v.y, n* v.z, n* v.w); }
inline const vec4 operator/(float n, const vec4 &v) { return vec4(n/ v.x, n/ v.y, n/ v.z, n/ v.w); }
















///========
// = mat4 =---------------------------------------------------------------------
///========


struct ALIGNED(4) mat4 {
  float v[16];
  
  // constructors

  inline mat4() { identity(); };
  inline mat4(const mat4 &o) {	
    v[0]= o[0];   v[4]= o[4];   v[8]=  o[8];    v[12]= o[12];
    v[1]= o[1];   v[5]= o[5];   v[9]=  o[9];    v[13]= o[13];
    v[2]= o[2];   v[6]= o[6];   v[10]= o[10];   v[14]= o[14];
    v[3]= o[3];   v[7]= o[7];   v[11]= o[11];   v[15]= o[15];
  };
  inline mat4(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33) {
    v[0]= n00;   v[4]= n10;   v[8]=  n20;   v[12]= n30;
    v[1]= n01;   v[5]= n11;   v[9]=  n21;   v[13]= n31;
    v[2]= n02;   v[6]= n12;   v[10]= n22;   v[14]= n32;
    v[3]= n03;   v[7]= n13;   v[11]= n23;   v[15]= n33;
  }
  inline mat4(float *o) {
    v[0]= o[0];   v[4]= o[4];   v[8]=  o[8];    v[12]= o[12];
    v[1]= o[1];   v[5]= o[5];   v[9]=  o[9];    v[13]= o[13];
    v[2]= o[2];   v[6]= o[6];   v[10]= o[10];   v[14]= o[14];
    v[3]= o[3];   v[7]= o[7];   v[11]= o[11];   v[15]= o[15];
  };
  

  // various matrix types
  
  inline mat4 &identity() {
    v[0]= 1.0f; v[4]= 0.0f; v[8]=  0.0f; v[12]= 0.0f;
    v[1]= 0.0f; v[5]= 1.0f; v[9]=  0.0f; v[13]= 0.0f;
    v[2]= 0.0f; v[6]= 0.0f; v[10]= 1.0f; v[14]= 0.0f;
    v[3]= 0.0f; v[7]= 0.0f; v[11]= 0.0f; v[15]= 1.0f;
    return *this;
  }
  inline mat4 &zero() { v[0]= v[4]= v[8]= v[12]= v[1]= v[5]= v[9]= v[13]= v[2]= v[6]= v[10]= v[14]= v[3]= v[7]= v[11]= v[15]= 0.0f; return *this; };

  // rotation functions

  // see http://www.khronos.org/opengles/documentation/html/glRotate.html if there are any bugs :)

  inline mat4 &rotate(float angle, const vec3 &axis) { // axis to rotate about, angle = angle in degrees
    vec3 a= axis;
    a.normalize();

    float s= sinf(angle* DEG2RAD);
    float c= cosf(angle* DEG2RAD);
    float c1= 1- c;
    float x1c= a.x* c1;
    float y1c= a.y* c1;
    float z1c= a.z* c1;

    v[0]=  a.x*  x1c+ c;       v[4]=  a.x*  y1c- a.z* s;  v[8]=  a.x*  z1c+ a.y* s;  v[12]= 0.0f;
    v[1]=  a.x*  y1c+ a.z* s;  v[5]=  a.y*  y1c+ c;       v[9]=  a.y*  z1c- a.x* s;  v[13]= 0.0f;
    v[2]=  a.x*  z1c- a.y* s;  v[6]=  a.y*  y1c+ a.x* s;  v[10]= a.z*  z1c+ c;       v[14]= 0.0f;
    v[3]=  0.0f;               v[7]=  0.0f;               v[11]= 0.0f;               v[15]= 1.0f;
    
    return *this;
  }

  inline mat4 &rotate(float angle, float x, float y, float z) { return rotate(angle, vec3(x, y, z)); }

  /// uses fewer computations than rotate
  inline mat4 &rotateX(float angle) {
    float c= cosf(angle* DEG2RAD);
    float s= sinf(angle* DEG2RAD);

    v[0]= 1.0f;   v[4]= 0.0f;   v[8]=  0.0f;   v[12]= 0.0f;
    v[1]= 0.0f;   v[5]= c;      v[9]=  -s;     v[13]= 0.0f;
    v[2]= 0.0f;   v[6]= s;      v[10]= c;      v[14]= 0.0f;
    v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
    return *this;
  }

  /// uses fewer computations than rotate
  inline mat4 &rotateY(float angle) {
    float c= cosf(angle* DEG2RAD);
    float s= sinf(angle* DEG2RAD);

    v[0]= c;      v[4]= 0.0f;   v[8]=  s;      v[12]= 0.0f;
    v[1]= 0.0f;   v[5]= 1.0f;   v[9]=  0.0f;   v[13]= 0.0f;
    v[2]= -s;     v[6]= 0.0f;   v[10]= c;      v[14]= 0.0f;
    v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
    return *this;
  }

  /// uses fewer computations than rotate
  inline mat4 &rotateZ(float angle) {
    float c= cosf(angle* DEG2RAD);
    float s= sinf(angle* DEG2RAD);

    v[0]= c;      v[4]= -s;     v[8]=  0.0f;   v[12]= 0.0f;
    v[1]= s;      v[5]= c;      v[9]=  0.0f;   v[13]= 0.0f;
    v[2]= 0.0f;   v[6]= 0.0f;   v[10]= 1.0f;   v[14]= 0.0f;
    v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
    return *this;
  }

  inline mat4 &translate(const vec3 &o) {
    v[0]= 1.0f; v[4]= 0.0f; v[8]=  0.0f; v[12]= o[0];
    v[1]= 0.0f; v[5]= 1.0f; v[9]=  0.0f; v[13]= o[1];
    v[2]= 0.0f; v[6]= 0.0f; v[10]= 1.0f; v[14]= o[2];
    v[3]= 0.0f; v[7]= 0.0f; v[11]= 0.0f; v[15]= 1.0f;
    return *this;
  } 

  inline mat4 &translate(float x, float y, float z) { return translate(vec3(x, y, z)); }
  
  inline mat4 &scale(const vec3 &o) {
    v[0]= o[0];   v[4]= 0.0f;   v[8]= 0.0f;    v[12]= 0.0f;
    v[1]= 0.0f;   v[5]= o[1];   v[9]= 0.0f;    v[13]= 0.0f;
    v[2]= 0.0f;   v[6]= 0.0f;   v[10]= o[2];   v[14]= 0.0f;
    v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]= 1.0f;
    return *this;
  }
  
  inline mat4 &scale(float x, float y, float z) { return scale(vec3(x, y, z)); } 

  // operators
  
  inline operator float *() { return v; }
  inline operator const float *() const { return v; }
  //inline const float operator[](int i) const { return v[i]; } // might not be needed (further thinking needed)
  //inline float &operator[](int i) { return v[i]; }            // might not be needed (further thinking needed)

  inline mat4 operator+(const mat4 &o) {
    return mat4(v[0]+  o[0],   v[1]+  o[1],   v[2]+  o[2],    v[3]+  o[3],
                v[4]+  o[4],   v[5]+  o[5],   v[6]+  o[6],    v[7]+  o[7],
                v[8]+  o[8],   v[9]+  o[9],   v[10]+ o[10],   v[11]+ o[11],
                v[12]+ o[12],  v[13]+ o[13],  v[14]+ o[14],   v[15]+ o[15]);
  }

  inline mat4 operator+(float n) {
    return mat4(v[0]+  n,   v[1]+  n,   v[2]+  n,    v[3]+  n,
                v[4]+  n,   v[5]+  n,   v[6]+  n,    v[7]+  n,
                v[8]+  n,   v[9]+  n,   v[10]+ n,    v[11]+ n,
                v[12]+ n,   v[13]+ n,   v[14]+ n,    v[15]+ n);
  }

  inline mat4 operator-(const mat4 &o) {
    return mat4(v[0]-  o[0],   v[1]-  o[1],   v[2]-  o[2],    v[3]-  o[3],
                v[4]-  o[4],   v[5]-  o[5],   v[6]-  o[6],    v[7]-  o[7],
                v[8]-  o[8],   v[9]-  o[9],   v[10]- o[10],   v[11]- o[11],
                v[12]- o[12],  v[13]- o[13],  v[14]- o[14],   v[15]- o[15]);
  }

  inline mat4 operator-(float n) {
    return mat4(v[0]-  n,   v[1]-  n,   v[2]-  n,    v[3]-  n,
                v[4]-  n,   v[5]-  n,   v[6]-  n,    v[7]-  n,
                v[8]-  n,   v[9]-  n,   v[10]- n,    v[11]- n,
                v[12]- n,   v[13]- n,   v[14]- n,    v[15]- n);
  }

  inline mat4 operator*(const mat4 &o) {
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

  // assume the vectors are padded with a 1 (homogenous coordinates)
  inline vec2 operator*(const vec2 &o) {
    return vec3(v[0]* o[0]+ v[4]* o[1]+ v[8]*  o[2]+ v[12],
                v[1]* o[0]+ v[5]* o[1]+ v[9]*  o[2]+ v[13]);
  }

  inline vec3 operator*(const vec3 &o) {
    return vec3(v[0]* o[0]+ v[4]* o[1]+ v[8]*  o[2]+ v[12],
                v[1]* o[0]+ v[5]* o[1]+ v[9]*  o[2]+ v[13],
                v[2]* o[0]+ v[6]* o[1]+ v[10]* o[2]+ v[14]);
  }

  inline vec4 operator*(const vec4 &o) {
    return vec4(v[0]* o[0]+ v[4]* o[1]+ v[8]*  o[2]+ v[12],
                v[1]* o[0]+ v[5]* o[1]+ v[9]*  o[2]+ v[13],
                v[2]* o[0]+ v[6]* o[1]+ v[10]* o[2]+ v[14],
                v[3]* o[0]+ v[7]* o[1]+ v[11]* o[2]+ v[15]);
  }

  inline mat4 &operator*=(const mat4 &o) { return *this= *this* o; }
  inline mat4 &operator+=(const mat4 &o) { return *this= *this+ o; }
  inline mat4 &operator-=(const mat4 &o) { return *this= *this- o; }


  // funcs

  // l= left, r= right, b= bottom, t= top, zn= z coord near, zf= z coord far
  inline mat4 &ortho(float l, float r, float b, float t, float zn, float zf) {
    /*float dx= r- l, dy= t- b, dz= zf- zn;

    v[0]= 2.0f/ dx;   v[4]= 0.0f;       v[8]=   0.0f;       v[12]= -(r+ l)/ dx;
    v[1]= 0.0f;       v[5]= 2.0f/ dy;   v[9]=   0.0f;       v[13]= -(t+ b)/ dy;
    v[2]= 0.0f;       v[6]= 0.0f;       v[10]= -2.0f/ dz;   v[14]= -(zf+ zn)/ dz;
    v[3]= 0.0f;       v[7]= 0.0f;       v[11]=  0.0f;       v[15]= 1.0f;
    return *this;
    */
    float dx= r- l, dy= b- t, dz= zf- zn;

    v[0]= 2.0f/ dx;   v[4]= 0.0f;       v[8]=   0.0f;       v[12]= -(r+ l)/ dx;
    v[1]= 0.0f;       v[5]= 2.0f/ dy;   v[9]=   0.0f;       v[13]= -(t+ b)/ dy;
    v[2]= 0.0f;       v[6]= 0.0f;       v[10]= 2.0f/ dz;   v[14]= -(zf+ zn)/ dz;
    v[3]= 0.0f;       v[7]= 0.0f;       v[11]=  0.0f;       v[15]= 1.0f;

    return *this;
  }

  inline mat4 &frustrum(float l, float r, float b, float t, float zn, float zf) {
    float dx= r- l, dy= t- b, dz= zf- zn;

    v[0]= (2.0f* zn)/ dx;   v[4]= 0.0f;             v[8]=  0.0f;            v[12]= 0.0f;
    v[1]= 0.0f;             v[5]= (2.0f* zn)/ dy;   v[9]=  (t+ b)/ dy;      v[13]= 0.0f;
    v[2]= 0.0f;             v[6]= 0.0f;             v[10]= -(zf+ zn)/ dz;   v[14]= -(2.0f* zf* zn)/ dz;
    v[3]= 0.0f;             v[7]= 0.0f;             v[11]= -1.0f;           v[15]= 1.0f;
    return *this;
  }

  inline mat4 &perspective(float fovy, float aspect, float znear, float zfar) {
    if((!fovy) || (absf(fovy)== 360.0f) || (!(zfar- znear)) || (!aspect))
      return *this;

    float dz= zfar- znear;
    float ctg= 1.0f/ tanf(fovy/ 2.0f* DEG2RAD);
  
    v[0]= ctg/ aspect;  v[4]= 0.0f;   v[8]= 0;                    v[12]= 0.0f;
    v[1]= 0.0f;         v[5]= ctg;    v[9]= 0;                    v[13]= 0.0f;
    v[3]= 0.0f;         v[6]= 0.0f;   v[10]= -(zfar+ znear)/ dz;  v[14]= -2.0f* znear* zfar/ dz;
    v[2]= 0.0f;         v[7]= 0.0f;   v[11]= -1.0f;               v[15]= 1.0f;
    return *this;
  }

  inline mat4 &lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
    vec3 f(center- eye);  f.normalize();
    vec3 s(cross(f, up)); s.normalize();
    vec3 u(cross(s, f));
  
    v[0]=  s.x;   v[4]=  s.y;   v[8]=   s.z;   v[12]= -dot(s, eye);
    v[1]=  u.x;   v[5]=  u.y;   v[9]=   u.z;   v[13]= -dot(u, eye);
    v[2]= -f.x;   v[6]= -f.y;   v[10]= -f.z;   v[14]=  dot(f, eye);
    v[3]= 0.0f;   v[7]= 0.0f;   v[11]= 0.0f;   v[15]=  1.0f;
    return *this;
  }

  // vector operations

  

  // matrix operations


};















///================
// = DECLARATIONS =-------------------------------------------------------------
///================



inline bool isNAN(const vec2 &o) { return isNANv(o.v, 2); }
inline bool isNAN(const vec3 &o) { return isNANv(o.v, 3); }
inline bool isNAN(const vec4 &o) { return isNANv(o.v, 4); }
inline bool isNAN(const mat4 &o) { return isNANv(o.v, 16); }


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








///=========================================///
// vec2 ================-------------------- //
///=========================================///

// constructors


inline vec2::vec2(const vec3 &o):      x(o.x),  y(o.y)  {}
inline vec2::vec2(const vec4 &o):      x(o.x),  y(o.y)  {}

// operators

inline vec2 &vec2::operator=(const vec3 &o)  { x= o.x;  y= o.y;  return *this; }
inline vec2 &vec2::operator=(const vec4 &o)  { x= o.x;  y= o.y;  return *this; }

inline vec2 &vec2::operator+=(const vec3 &o)  { x+= o.x;  y+= o.y;  return *this; }
inline vec2 &vec2::operator+=(const vec4 &o)  { x+= o.x;  y+= o.y;  return *this; }

inline vec2 &vec2::operator-=(const vec3 &o)  { x-= o.x;  y-= o.y;  return *this; }
inline vec2 &vec2::operator-=(const vec4 &o)  { x-= o.x;  y-= o.y;  return *this; }

inline vec2 &vec2::operator*=(const vec3 &o)  { x*= o.x;  y*= o.y;  return *this; }
inline vec2 &vec2::operator*=(const vec4 &o)  { x*= o.x;  y*= o.y;  return *this; }

inline vec2 &vec2::operator/=(const vec3 &o)  { x/= o.x;  y/= o.y;  return *this; }
inline vec2 &vec2::operator/=(const vec4 &o)  { x/= o.x;  y/= o.y;  return *this; }

inline const vec2 vec2::operator+(const vec3 &o)  const { return vec2(*this)+= o; }
inline const vec2 vec2::operator+(const vec4 &o)  const { return vec2(*this)+= o; }

inline const vec2 vec2::operator-(const vec3 &o)  const { return vec2(*this)-= o; }
inline const vec2 vec2::operator-(const vec4 &o)  const { return vec2(*this)-= o; }

inline const vec2 vec2::operator*(const vec3 &o)  const { return vec2(*this)*= o; }
inline const vec2 vec2::operator*(const vec4 &o)  const { return vec2(*this)*= o; }

inline const vec2 vec2::operator/(const vec3 &o)  const { return vec2(*this)/= o; }
inline const vec2 vec2::operator/(const vec4 &o)  const { return vec2(*this)/= o; }

inline bool vec2::operator==(const vec3 &o)  const { return ((x== o.x)  && (y== o.y)); }
inline bool vec2::operator==(const vec4 &o)  const { return ((x== o.x)  && (y== o.y)); }

inline bool vec2::operator!=(const vec3 &o)  const { return ((x!= o.x)  || (y!= o.y)); }
inline bool vec2::operator!=(const vec4 &o)  const { return ((x!= o.x)  || (y!= o.y)); }

// dot product

inline float vec2::dot(const vec3 &o) const { return this->dot(o.x,  o.y); }
inline float vec2::dot(const vec4 &o) const { return this->dot(o.x,  o.y); }




///=========================================///
// vec2i ================------------------- //
///=========================================///

// constructors


inline vec2i::vec2i(const vec3i &o):      x(o.x),  y(o.y)  {}
//inline vec2i::vec2i(const vec4i &o):      x(o.x),  y(o.y)  {}

// operators

inline vec2i &vec2i::operator=(const vec3i &o)  { x= o.x;  y= o.y;  return *this; }
//inline vec2i &vec2i::operator=(const vec4i &o)  { x= o.x;  y= o.y;  return *this; }

inline vec2i &vec2i::operator+=(const vec3i &o)  { x+= o.x;  y+= o.y;  return *this; }
//inline vec2i &vec2i::operator+=(const vec4i &o)  { x+= o.x;  y+= o.y;  return *this; }

inline vec2i &vec2i::operator-=(const vec3i &o)  { x-= o.x;  y-= o.y;  return *this; }
//inline vec2i &vec2i::operator-=(const vec4i &o)  { x-= o.x;  y-= o.y;  return *this; }

inline vec2i &vec2i::operator*=(const vec3i &o)  { x*= o.x;  y*= o.y;  return *this; }
//inline vec2i &vec2i::operator*=(const vec4i &o)  { x*= o.x;  y*= o.y;  return *this; }

inline vec2i &vec2i::operator/=(const vec3i &o)  { x/= o.x;  y/= o.y;  return *this; }
//inline vec2i &vec2i::operator/=(const vec4i &o)  { x/= o.x;  y/= o.y;  return *this; }

inline const vec2i vec2i::operator+(const vec3i &o)  const { return vec2i(*this)+= o; }
//inline const vec2i vec2i::operator+(const vec4i &o)  const { return vec2i(*this)+= o; }

inline const vec2i vec2i::operator-(const vec3i &o)  const { return vec2i(*this)-= o; }
//inline const vec2i vec2i::operator-(const vec4i &o)  const { return vec2i(*this)-= o; }

inline const vec2i vec2i::operator*(const vec3i &o)  const { return vec2i(*this)*= o; }
//inline const vec2i vec2i::operator*(const vec4i &o)  const { return vec2i(*this)*= o; }

inline const vec2i vec2i::operator/(const vec3i &o)  const { return vec2i(*this)/= o; }
//inline const vec2i vec2i::operator/(const vec4i &o)  const { return vec2i(*this)/= o; }

inline bool vec2i::operator==(const vec3i &o)  const { return ((x== o.x)  && (y== o.y)); }
//inline bool vec2i::operator==(const vec4i &o)  const { return ((x== o.x)  && (y== o.y)); }

inline bool vec2i::operator!=(const vec3i &o)  const { return ((x!= o.x)  || (y!= o.y)); }
//inline bool vec2i::operator!=(const vec4i &o)  const { return ((x!= o.x)  || (y!= o.y)); }

// dot product

inline int32 vec2i::dot(const vec3i &o) const { return this->dot(o.x,  o.y); }
//inline int32 vec2i::dot(const vec4i &o) const { return this->dot(o.x,  o.y); }









///=========================================///
// vec3 ================-------------------- //
///=========================================///

// constructors

inline vec3::vec3(const vec4 &o):                x(o.x),  y(o.y),  z(o.z)  {}

// operators

inline vec3 &vec3::operator=(const vec4 &o)  { x= o.x;  y= o.y;  z= o.z;  return *this; }

inline vec3 &vec3::operator+=(const vec4 &o)  { x+= o.x;  y+= o.y;  z+= o.z;  return *this; }

inline vec3 &vec3::operator-=(const vec4 &o)  { x-= o.x;  y-= o.y;  z-= o.z;  return *this; }

inline vec3 &vec3::operator*=(const vec4 &o)  { x*= o.x;  y*= o.y;  z*= o.z;  return *this; }

inline vec3 &vec3::operator/=(const vec4 &o)  { x/= o.x;  y/= o.y;  z/= o.z;  return *this; }

inline const vec3 vec3::operator+(const vec4 &o)  const { return vec3(*this)+= o; }

inline const vec3 vec3::operator-(const vec4 &o)  const { return vec3(*this)-= o; }

inline const vec3 vec3::operator*(const vec4 &o)  const { return vec3(*this)*= o; }

inline const vec3 vec3::operator/(const vec4 &o)  const { return vec3(*this)/= o; }

inline bool vec3::operator==(const vec4 &o)  const { return ((x== o.x)  && (y== o.y)  && (z== o.z)); }

inline bool vec3::operator!=(const vec4 &o)  const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)); }

// dot product

inline float vec3::dot(const vec4 &o)           const { return this->dot(o.x, o.y, o.z); }

// cross product

inline vec3 vec3::cross(const vec4 &o)          const { return vec3((y* o.z)- (z* o.y), (z* o.x)- (x* o.z), (x* o.y)- (y* o.x)); }





///==========================================///
// vec3i ================-------------------- //
///==========================================///

// constructors
//inline vec3i::vec3i(const vec4i &o):               x(o.x),  y(o.y),  z(o.z)  {}

// operators

//inline vec3i &vec3i::operator=(const vec4i &o) { x= o.x;  y= o.y;  z= o.z;  return *this; }

//inline vec3i &vec3i::operator+=(const vec4i &o) { x+= o.x;  y+= o.y;  z+= o.z;  return *this; }

//inline vec3i &vec3i::operator-=(const vec4i &o)   { x-= o.x;  y-= o.y;  z-= o.z;  return *this; }

//inline vec3i &vec3i::operator*=(const vec4i &o)   { x*= o.x;  y*= o.y;  z*= o.z;  return *this; }

//inline vec3i &vec3i::operator/=(const vec4i &o)   { x/= o.x;  y/= o.y;  z/= o.z;  return *this; }

//inline const vec3i vec3i::operator+(const vec4i &o)   const { return vec3i(*this)+= o; }

//inline const vec3i vec3i::operator-(const vec4i &o)   const { return vec3i(*this)-= o; }

//inline const vec3i vec3i::operator*(const vec4i &o)   const { return vec3i(*this)*= o; }

//inline const vec3i vec3i::operator/(const vec4i &o)   const { return vec3i(*this)/= o; }


//inline bool vec3i::operator==(const vec4i &o)   const { return ((x== o.x)  && (y== o.y)  && (z== o.z)); }

//inline bool vec3i::operator!=(const vec4i &o)   const { return ((x!= o.x)  || (y!= o.y)  || (z!= o.z)); }

// dot product

//inline int32 vec3i::dot(const vec4i &o)                     const { return x* o.x+   y* o.y+  z* o.z; }

// cross product

//inline vec3i vec3i::cross(const vec4i &o)                     const { return vec3i(y* o.z- z* o.y, z* o.x- x* o.z, x* o.y- y* o.x); }








///=========================================///
// vec4 ================-------------------- //
///=========================================///






///=========================================///
// mat4 ================-------------------- //
///=========================================///

// constructors




// various matrix types


// rotation functions




// operators

// vector operations



// funcs



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














// =========================================================
// = DOCUMENTATION PURPOSES, VARIOUS INTERESTING FUNCTIONS ===================================================
// =========================================================





// ZX Spectrum method, Chebyshev polynomials:
// https://albertveli.wordpress.com/2015/01/10/zx-sine/
// http://www.ams.org/journals/mcom/1954-08-047/S0025-5718-1954-0063487-2/S0025-5718-1954-0063487-2.pdf
// This method is extreem acurate, but twice as slow, unfortunately. I left it here just for documentation purposes

#define T0 (1)
#define T1 (b)
#define T2 ((2*  b2)- 1)
#define T3 ((4*  b3)- (3*  b))
#define T4 ((8*  b4)- (8*  b2)+ 1)
#define T5 ((16* b5)- (20* b3)+ (5* b))

#define C0  1.276278962f
#define C1 -0.285261569f
#define C2  0.009118016f
#define C3 -0.000136587f
#define C4  0.000001185f
#define C5 -0.000000007f
 
inline float sinf_ZXSpectrum(float n) {
  
  /// make n -PI to +PI
  if(n> PI)
    n-= ((int32)((n+ PI)* FPII))* PI_DBL;         /// n-= ((int32)((n+ PI)/ PI_DBL))* PI_DBL;   OR  if(n> PI) while(n> PI) n-= PI_DBL;
  else if(n< -PI)
    n+= ((int32)((-n+ PI)* FPII))* PI_DBL;        /// n+= ((int32)((-n+ PI)/ PI_DBL))* PI_DBL;  OR  if(n< -PI) while(n< -PI) n+= PI_DBL;

  /// make n -PI/2 to +PI/2
  if(n> PI/ 2)
    n= PI- n;   
  else if(n< -PI/ 2)
    n= -PI- n;

  // convert n from -PI/2 -> +PI/2 to -0.25 -> +0.25 range
  n= (n* 0.25f)* II_PI;   // n= (n* 0.25f)/ (PI/ 2.0f);
  
  float a= 4* n;
  float b= (2* a* a)- 1;
  float b2= b* b;
  float b3= b2* b;
  float b4= b3* b;
  float b5= b4* b;
  
  return (((C0* T0)+ (C1* T1)+ (C2* T2)+ (C3* T3)+ (C4* T4)+ (C5* T5))* a);
}

inline float cosf_ZXSpectrum(float n) { return sinf_ZXSpectrum(n+ PI_HALF); }

#undef T0
#undef T1
#undef T2
#undef T3
#undef T4
#undef T5
#undef C0
#undef C1
#undef C2
#undef C3
#undef C4
#undef C5




// Taylor series TAN
// x+ (1/3)*x^3 + (2/15)* x^5 + (17/315)* x^7,   x must be -PI/2 +PI/2
// this method works accurately and speedy for -PI/4 -> +PI/4
// the accuracy drops to the extreem - with angles more than 75degrees, you need like 50 computations to do anything close to accurate
// and not even then. The speed is too slow for that many computations, so taylor for tan is out of the picture
// for -PI/4 to +PI/4, it's _the fastest function_, good acuracy
inline float tanf_Taylor(float n) {
  /// adjust n to -PI -> +PI
  n-= mlib::floorf((n+ PI)* FPII)* PI_DBL;

  /// adjust n to -PI/2 -> +PI/2
  if(n> PI_HALF)       n-= PI;
  else if(n< -PI_HALF) n+= PI;

  const float c1= 1.0f/ 3.0f;
  const float c2= 2.0f/ 15.0f;
  const float c3= 17.0f/ 315.0f;

  //const float c4= 62.0f/ 2835.0f;
  //const float c5= 1382.0f/ 155925.0f;
  //const float c6= 0.00359212803657248101692546136991f;   // 21844/ 6081075;
  //const float c7= 0.0014558343870513182682494851807f;    // 929569/ 638512875
  //const float c8= 5.9002744094558598137807599370002e-4f; // 6404582/ 10854718875
  //const float c9= 2.3912911424355248148573145888511e-4f; // 443861162/ 1856156927625

  float n2=  n* n;
  float n3=  n* n2,    n5=  n3*  n2,  n7=  n5*  n2;
  //float n9=  n7* n2,   n11= n9*  n2,  n13= n11* n2;
  //float n15= n13* n2,  n17= n15* n2,  n19= n17* n2;

  return n+ (c1* n3)+ (c2* n5)+ (c3* n7);
  //return n+ (c1* n3)+ (c2* n5)+ (c3* n7)+ (c4* n9);
  //return n+ (c1* n3)+ (c2* n5)+ (c3* n7)+ (c4* n9)+ (c5* n11);
  //return n+ (c1* n3)+ (c2* n5)+ (c3* n7)+ (c4* n9)+ (c5* n11)+ (c6* n13);

  // not even with these many computations, anyhting over PI/4 is extreem inacurate
  //return n+ (c1* n3)+ (c2* n5)+ (c3* n7)+ (c4* n9)+ (c5* n11)+ (c6* n13)+ (c7* n15)+ (c8* n17)+ (c9* n19);
}


// you need more computations to achieve the same amount of precision like the sin version
// also, using only the cos or the sin version is better
// left this here just for documentation purposes
inline float cosf_Taylor(float n) {
  // the func runs on -PI to +PI. <n> must be adjusted to this boundary
  n-= mlib::floorf((n+ PI)* FPII)* PI_DBL;

  constexpr float f2=  0.5f;                                    // 1/ 2!
  constexpr float f4=  0.04166666666666666666666666666667f;     // 1/ 4!  0.0416666679f
  constexpr float f6=  0.00138888888888888888888888888889f;     // 1/ 6!  0.00138888892f
  constexpr float f8=  2.4801587301587301587301587301587e-5f;   // 1/ 8!  2.48015876e-05f
  constexpr float f10= 2.755731922398589e-7f;                   // 1/ 10! 2.75573200e-07f
  //constexpr float f12= 2.0876756987868098979210090321201e-9f;   // 1/ 12!

  float n2= n* n, n4= n2* n2, n6= n4* n2;
  float n8= n6* n2;
  float n10= n8* n2;
  //float n12= n10* n2;

  // cos(x)= 1- x^2/ 2! + x^4/ 4! - x^6/ 6! + ...
  //return 1.0f- (n2* f2)+ (n4* f4)- (n6* f6);                                    // ~0.002 accuracy, extreem fast
  //return 1.0f- (n2* f2)+ (n4* f4)- (n6* f6)+ (n8* f8);                          // ~0.0001 accuracy, way faster than std
  return 1.0f- (n2* f2)+ (n4* f4)- (n6* f6)+ (n8* f8)- (n10* f10);              // ~0.000001 accuracy, faster than std
  //return 1.0f- (n2* f2)+ (n4* f4)- (n6* f6)+ (n8* f8)- (n10* f10)+ (n12* f12);  // extreem acurate, slower than std
}







}; // namespace mlib





























