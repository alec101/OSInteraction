#include "OSinteraction.h"

GlExtFuncs glExt;

GlExtFuncs::GlExtFuncs() {
  glTexImage3D= null;
  //Compressed Textures
  glCompressedTexImage3DARB= null;
  glCompressedTexImage2DARB= null;
  glCompressedTexImage1DARB= null;
  glCompressedTexSubImage3DARB= null;
  glCompressedTexSubImage2DARB= null;
  glCompressedTexSubImage1DARB= null;
  glGetCompressedTexImageARB= null;
  //Index Color Tex
  //PFNGLCOLORTABLEEXTPROC glColorTableEXT = NULL;
  //PFNGLGETCOLORTABLEPARAMETERIVEXTPROC glGetColorTableParameterivEXT = NULL;

  //Multi Textures
  glMultiTexCoord1fARB= null;
  glMultiTexCoord2fARB= null;
  glMultiTexCoord3fARB= null;
  glMultiTexCoord4fARB= null;
  glActiveTextureARB= null;
  glClientActiveTextureARB= null;
  //VBO
  glBindBufferARB= null;
  glBufferDataARB= null;
  glGenBuffersARB= null;
  glMapBufferARB= null;
  glUnmapBufferARB= null;
  glIsBufferARB= null;
  glDeleteBuffersARB= null;
  //Particle System
  glPointParameterf= null;
  glPointParameterfv= null;
}



template<class T> bool getExtension(cchar *name, T& address) {
  bool chatty= true;
  #ifdef OS_WIN
  address= (T)wglGetProcAddress(name);

  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // CANNOT RELY ON FUNCTION POINTER IF THE EXTENSION IS SUPPORTED. THERE IS A SPECIAL FUNCTION FOR THAT;
  // ON SOME SYSTEMS IT WILL JUST AQUIRE BUT NOT WORK
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  if(chatty) {
    if(address) printf("OK - %s\n", name);
    else        printf("NOT SUPPORTED - %s\n", name);
  }

  #endif /// OS_WIN

  #ifdef OS_LINUX
  makeme
  #endif /// OS_LINUX

  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC
  if(address) return true; else return false;
}




void OSInteraction::getExtensions() {
  bool chatty= true;
  // probly, just get ALL functions; start with inline, that calls glRenderer->realFunc
  // only in win there's the need for glRenderer tied funcs, but i guess will do for every system...
  // or glRenderer will point to a struct with all the dang functions, that will be memory allocated;
  // on linux and mac there will be only 1 alloc, all renderers will point to the same structure
  // on windows, there will be an alloc for each renderer


  if(chatty) printf("Vendor Name: %s\n", glGetString(GL_VENDOR));
  if(chatty) printf("Graphic Card: %s\n", glGetString(GL_RENDERER));
  if(chatty) printf("GL Version: %s\n", glGetString(GL_VERSION));
  if(chatty) printf("Testing GL Extensions\n");

  int max3D;
  GLint maxTexelUnits;

  glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTexelUnits);
  if(chatty) printf("Texel units avaible: %d\n", maxTexelUnits);

  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &max3D);
  if(chatty) printf("Maximum 3d texture size %dx%dx%d\n", max3D, max3D, max3D);

  getExtension("glTexImage3DEXT", glExt.glTexImage3D);
  //Compressed Textures
  getExtension("glCompressedTexImage3DARB", glExt.glCompressedTexImage3DARB);
  getExtension("glCompressedTexImage2DARB", glExt.glCompressedTexImage2DARB);
  getExtension("glCompressedTexImage1DARB", glExt.glCompressedTexImage1DARB);
  getExtension("glCompressedTexSubImage3DARB", glExt.glCompressedTexSubImage3DARB);
  getExtension("glCompressedTexSubImage2DARB", glExt.glCompressedTexSubImage2DARB);
  getExtension("glCompressedTexSubImage1DARB", glExt.glCompressedTexSubImage1DARB);
  getExtension("glGetCompressedTexImageARB", glExt.glGetCompressedTexImageARB);
  //MultiTextures
  getExtension("glMultiTexCoord1fARB", glExt.glMultiTexCoord1fARB);
  getExtension("glMultiTexCoord2fARB", glExt.glMultiTexCoord2fARB);
  getExtension("glMultiTexCoord3fARB", glExt.glMultiTexCoord3fARB);
  getExtension("glMultiTexCoord4fARB", glExt.glMultiTexCoord4fARB);
  getExtension("glActiveTextureARB", glExt.glActiveTextureARB);
  getExtension("glClientActiveTextureARB", glExt.glClientActiveTextureARB);
  //Index Color
  //getExtension("glColorTableEXT", glColorTableEXT);
  //getExtension("glGetColorTableParameterivEXT", glGetColorTableParameterivEXT);
  //VBO
  getExtension("glBindBufferARB", glExt.glBindBufferARB);
  getExtension("glBufferDataARB", glExt.glBufferDataARB);
  getExtension("glGenBuffersARB", glExt.glGenBuffersARB);
  getExtension("glMapBufferARB", glExt.glMapBufferARB);
  getExtension("glUnmapBufferARB", glExt.glUnmapBufferARB);
  getExtension("glIsBufferARB", glExt.glIsBufferARB);
  getExtension("glDeleteBuffersARB", glExt.glDeleteBuffersARB);
  //Particle System
  getExtension("glPointParameterf", glExt.glPointParameterf);
  getExtension("glPointParameterfv", glExt.glPointParameterfv);
  
  if(chatty) printf("----------------------------------------------------------\n");
}


// a current renderer pointer must be made !!!!!!!!!!!!!!!!!!!




// Compressed textures
inline GLAPI void APIENTRY glActiveTextureARB(GLenum texture) {
  glExt.glActiveTextureARB(texture); }
inline GLAPI void APIENTRY glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  glExt.glTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels); }
inline GLAPI void APIENTRY glCompressedTexImage3DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) {
  glExt.glCompressedTexImage3DARB(target, level, internalformat, width, height, depth, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexImage2DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) {
  glExt.glCompressedTexImage2DARB(target, level, internalformat, width, height, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexImage1DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) {
  glExt.glCompressedTexImage1DARB(target, level, internalformat, width, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage3DARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) {
  glExt.glCompressedTexSubImage3DARB(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage2DARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
  glExt.glCompressedTexSubImage2DARB(target, level, xoffset, yoffset, width, height, format, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage1DARB(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) {
  glExt.glCompressedTexSubImage1DARB(target, level, xoffset, width, format, imageSize, data); }
inline GLAPI void APIENTRY glGetCompressedTexImageARB(GLenum target, GLint level, void *img) {
  glExt.glGetCompressedTexImageARB(target, level, img); }

// Multi textures
inline GLAPI void APIENTRY glMultiTexCoord1fARB(GLenum target, GLfloat s) {
  glExt.glMultiTexCoord1fARB(target, s); }
inline GLAPI void APIENTRY glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t) {
  glExt.glMultiTexCoord2fARB(target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r) {
  glExt.glMultiTexCoord3fARB(target, s, t, r); }
inline GLAPI void APIENTRY glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
  glExt.glMultiTexCoord4fARB(target, s, t, r, q); }

inline GLAPI void APIENTRY glBindBufferARB(GLenum target, GLuint buffer) {
  glExt.glBindBufferARB(target, buffer); }
inline GLAPI void APIENTRY glDeleteBuffersARB(GLsizei n, const GLuint *buffers) {
  glExt.glDeleteBuffersARB(n, buffers); }
inline GLAPI void APIENTRY glGenBuffersARB(GLsizei n, GLuint *buffers) {
  glExt.glGenBuffersARB(n, buffers); }
inline GLAPI GLboolean APIENTRY glIsBufferARB(GLuint buffer) {
  return glExt.glIsBufferARB(buffer); }
inline GLAPI void APIENTRY glBufferDataARB(GLenum target, GLsizeiptrARB size, const void *data, GLenum usage) {
  glExt.glBufferDataARB(target, size, data, usage); }
//inline GLAPI void APIENTRY glBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void *data) {
//  glExt.glBufferSubDataARB(target, offset, size, data); }
//inline GLAPI void APIENTRY glGetBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, void *data) {
inline GLAPI void *APIENTRY glMapBufferARB(GLenum target, GLenum access) {
  return glExt.glMapBufferARB(target, access); }
inline GLAPI GLboolean APIENTRY glUnmapBufferARB(GLenum target) {
  return glExt.glUnmapBufferARB(target); }
//inline GLAPI void APIENTRY glGetBufferParameterivARB (GLenum target, GLenum pname, GLint *params) {
//inline GLAPI void APIENTRY glGetBufferPointervARB (GLenum target, GLenum pname, void **params) {

inline GLAPI void APIENTRY glPointParameterf(GLenum pname, GLfloat param) {
  glExt.glPointParameterf(pname, param); }
inline GLAPI void APIENTRY glPointParameterfv(GLenum pname, const GLfloat *params) {
  glExt.glPointParameterfv(pname, params); }

 







