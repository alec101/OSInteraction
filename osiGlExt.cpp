#include "osinteraction.h"

// TODO:
/*
 * [win] MUST check ogl 1.1, if there are any funcs got from gl.h
 */


/// one single struct that holds all the gl extensions, for LINUX and MAC
/// under windows, each renderer must have it's own functions
#ifndef OS_WIN
GlExtFuncs glExt;
#endif /// OS_MAC or OS_LINUX


/// [internal] function that will be called for each extension function
template<class T> bool getGlProc(cchar *name, T& address) {
  bool chatty= true;
  #ifdef OS_WIN
  address= (T)wglGetProcAddress(name);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  address= (T)glXGetProcAddressARB((const GLubyte *)name);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC
  if(address) return true; else return false;
}

#define GETGLPROCMACRO(_x_x) getGlProc(#_x_x, r->glExt. _x_x);

///=============================///
// OpenGL version funcs aquiring //
///=============================///
void getVERfuncs(osiRenderer *r, int major, int minor) {
  getGlProc("glTexImage3DEXT", r->glExt.glTexImage3D);
  
  #ifdef OS_LINUX
  // linux GLX versions //
  ///==================///

  /// GLX_VERSION_1_3
  getGlProc("glXGetFBConfigs", r->glExt.glXGetFBConfigs); //GETGLPROCMACRO(glXGetFBConfigs)
  GETGLPROCMACRO(glXChooseFBConfig)
  GETGLPROCMACRO(glXGetFBConfigAttrib)
  GETGLPROCMACRO(glXGetVisualFromFBConfig)
  GETGLPROCMACRO(glXCreateWindow)
  GETGLPROCMACRO(glXDestroyWindow)
  GETGLPROCMACRO(glXCreatePixmap)
  GETGLPROCMACRO(glXDestroyPixmap)
  GETGLPROCMACRO(glXCreatePbuffer)
  GETGLPROCMACRO(glXDestroyPbuffer)
  GETGLPROCMACRO(glXQueryDrawable)
  GETGLPROCMACRO(glXCreateNewContext)
  GETGLPROCMACRO(glXMakeContextCurrent)
  GETGLPROCMACRO(glXGetCurrentReadDrawable)
  GETGLPROCMACRO(glXQueryContext)
  GETGLPROCMACRO(glXSelectEvent)
  GETGLPROCMACRO(glXGetSelectedEvent)

  /// GLX_VERSION_1_4
  //GETGLPROCMACRO(glXGetProcAddress)
  #endif /// OS_LINUX

  if(major< 1)
    return;

  /// OpenGL 1.2 funcs =================------------------------------
  if(major== 1 && minor< 2)
    return;

  /*
  getGlProc("glDrawRangeElements", r->glExt.glDrawRangeElements);
  getGlProc("glTexImage3D", r->glExt.glTexImage3D);
  getGlProc("glTexSubImage3D", r->glExt.glTexSubImage3D);
  getGlProc("glCopyTexSubImage3D", r->glExt.glCopyTexSubImage3D);
  */
  GETGLPROCMACRO(glDrawRangeElements)
  GETGLPROCMACRO(glTexImage3D)
  GETGLPROCMACRO(glTexSubImage3D)
  GETGLPROCMACRO(glCopyTexSubImage3D)

  /// OpenGL 1.3 funcs =================------------------------------
  if(major== 1 && minor< 3)
    return;

  GETGLPROCMACRO(glActiveTexture)
  GETGLPROCMACRO(glSampleCoverage)
  GETGLPROCMACRO(glCompressedTexImage3D)
  GETGLPROCMACRO(glCompressedTexImage2D)
  GETGLPROCMACRO(glCompressedTexImage1D)
  GETGLPROCMACRO(glCompressedTexSubImage3D)
  GETGLPROCMACRO(glCompressedTexSubImage2D)
  GETGLPROCMACRO(glCompressedTexSubImage1D)
  GETGLPROCMACRO(glGetCompressedTexImage)
  #ifdef OS_WIN
  GETGLPROCMACRO(glClientActiveTexture)
  GETGLPROCMACRO(glMultiTexCoord1d)
  GETGLPROCMACRO(glMultiTexCoord1dv)
  GETGLPROCMACRO(glMultiTexCoord1f)
  GETGLPROCMACRO(glMultiTexCoord1fv)
  GETGLPROCMACRO(glMultiTexCoord1i)
  GETGLPROCMACRO(glMultiTexCoord1iv)
  GETGLPROCMACRO(glMultiTexCoord1s)
  GETGLPROCMACRO(glMultiTexCoord1sv)
  GETGLPROCMACRO(glMultiTexCoord2d)
  GETGLPROCMACRO(glMultiTexCoord2dv)
  GETGLPROCMACRO(glMultiTexCoord2f)
  GETGLPROCMACRO(glMultiTexCoord2fv)
  GETGLPROCMACRO(glMultiTexCoord2i)
  GETGLPROCMACRO(glMultiTexCoord2iv)
  GETGLPROCMACRO(glMultiTexCoord2s)
  GETGLPROCMACRO(glMultiTexCoord2sv)
  GETGLPROCMACRO(glMultiTexCoord3d)
  GETGLPROCMACRO(glMultiTexCoord3dv)
  GETGLPROCMACRO(glMultiTexCoord3f)
  GETGLPROCMACRO(glMultiTexCoord3fv)
  GETGLPROCMACRO(glMultiTexCoord3i)
  GETGLPROCMACRO(glMultiTexCoord3iv)
  GETGLPROCMACRO(glMultiTexCoord3s)
  GETGLPROCMACRO(glMultiTexCoord3sv)
  GETGLPROCMACRO(glMultiTexCoord4d)
  GETGLPROCMACRO(glMultiTexCoord4dv)
  GETGLPROCMACRO(glMultiTexCoord4f)
  GETGLPROCMACRO(glMultiTexCoord4fv)
  GETGLPROCMACRO(glMultiTexCoord4i)
  GETGLPROCMACRO(glMultiTexCoord4iv)
  GETGLPROCMACRO(glMultiTexCoord4s)
  GETGLPROCMACRO(glMultiTexCoord4sv)
  GETGLPROCMACRO(glLoadTransposeMatrixf)
  GETGLPROCMACRO(glLoadTransposeMatrixd)
  GETGLPROCMACRO(glMultTransposeMatrixf)
  GETGLPROCMACRO(glMultTransposeMatrixd)
  #endif /// OS_WIN

  /// OpenGL 1.4 funcs =================------------------------------
  if(major== 1 && minor< 4)
    return;

  GETGLPROCMACRO(glBlendFuncSeparate)
  GETGLPROCMACRO(glMultiDrawArrays)
  GETGLPROCMACRO(glMultiDrawElements)
  GETGLPROCMACRO(glPointParameterf)
  GETGLPROCMACRO(glPointParameterfv)
  GETGLPROCMACRO(glPointParameteri)
  GETGLPROCMACRO(glPointParameteriv)
  GETGLPROCMACRO(glFogCoordf)
  GETGLPROCMACRO(glFogCoordfv)
  GETGLPROCMACRO(glFogCoordd)
  GETGLPROCMACRO(glFogCoorddv)
  GETGLPROCMACRO(glFogCoordPointer)
  GETGLPROCMACRO(glSecondaryColor3b)
  GETGLPROCMACRO(glSecondaryColor3bv)
  GETGLPROCMACRO(glSecondaryColor3d)
  GETGLPROCMACRO(glSecondaryColor3dv)
  GETGLPROCMACRO(glSecondaryColor3f)
  GETGLPROCMACRO(glSecondaryColor3fv)
  GETGLPROCMACRO(glSecondaryColor3i)
  GETGLPROCMACRO(glSecondaryColor3iv)
  GETGLPROCMACRO(glSecondaryColor3s)
  GETGLPROCMACRO(glSecondaryColor3sv)
  GETGLPROCMACRO(glSecondaryColor3ub)
  GETGLPROCMACRO(glSecondaryColor3ubv)
  GETGLPROCMACRO(glSecondaryColor3ui)
  GETGLPROCMACRO(glSecondaryColor3uiv)
  GETGLPROCMACRO(glSecondaryColor3us)
  GETGLPROCMACRO(glSecondaryColor3usv)
  GETGLPROCMACRO(glSecondaryColorPointer)
  GETGLPROCMACRO(glWindowPos2d)
  GETGLPROCMACRO(glWindowPos2dv)
  GETGLPROCMACRO(glWindowPos2f)
  GETGLPROCMACRO(glWindowPos2fv)
  GETGLPROCMACRO(glWindowPos2i)
  GETGLPROCMACRO(glWindowPos2iv)
  GETGLPROCMACRO(glWindowPos2s)
  GETGLPROCMACRO(glWindowPos2sv)
  GETGLPROCMACRO(glWindowPos3d)
  GETGLPROCMACRO(glWindowPos3dv)
  GETGLPROCMACRO(glWindowPos3f)
  GETGLPROCMACRO(glWindowPos3fv)
  GETGLPROCMACRO(glWindowPos3i)
  GETGLPROCMACRO(glWindowPos3iv)
  GETGLPROCMACRO(glWindowPos3s)
  GETGLPROCMACRO(glWindowPos3sv)
  GETGLPROCMACRO(glBlendColor)
  GETGLPROCMACRO(glBlendEquation)

  /// OpenGL 1.5 funcs =================------------------------------
  if(major== 1 && minor< 5)
    return;

  GETGLPROCMACRO(glGenQueries)
  GETGLPROCMACRO(glDeleteQueries)
  GETGLPROCMACRO(glIsQuery)
  GETGLPROCMACRO(glBeginQuery)
  GETGLPROCMACRO(glEndQuery)
  GETGLPROCMACRO(glGetQueryiv)
  GETGLPROCMACRO(glGetQueryObjectiv)
  GETGLPROCMACRO(glGetQueryObjectuiv)
  GETGLPROCMACRO(glBindBuffer)
  GETGLPROCMACRO(glDeleteBuffers)
  GETGLPROCMACRO(glGenBuffers)
  GETGLPROCMACRO(glIsBuffer)
  GETGLPROCMACRO(glBufferData)
  GETGLPROCMACRO(glBufferSubData)
  GETGLPROCMACRO(glGetBufferSubData)
  GETGLPROCMACRO(glMapBuffer)
  GETGLPROCMACRO(glUnmapBuffer)
  GETGLPROCMACRO(glGetBufferParameteriv)
  GETGLPROCMACRO(glGetBufferPointerv)

  /// OpenGL 2.0 funcs =================------------------------------
  if(major< 2)
    return;

  GETGLPROCMACRO(glBlendEquationSeparate)
  GETGLPROCMACRO(glDrawBuffers)
  GETGLPROCMACRO(glStencilOpSeparate)
  GETGLPROCMACRO(glStencilFuncSeparate)
  GETGLPROCMACRO(glStencilMaskSeparate)
  GETGLPROCMACRO(glAttachShader)
  GETGLPROCMACRO(glBindAttribLocation)
  GETGLPROCMACRO(glCompileShader)
  GETGLPROCMACRO(glCreateProgram)
  GETGLPROCMACRO(glCreateShader)
  GETGLPROCMACRO(glDeleteProgram)
  GETGLPROCMACRO(glDeleteShader)
  GETGLPROCMACRO(glDetachShader)
  GETGLPROCMACRO(glDisableVertexAttribArray)
  GETGLPROCMACRO(glEnableVertexAttribArray)
  GETGLPROCMACRO(glGetActiveAttrib)
  GETGLPROCMACRO(glGetActiveUniform)
  GETGLPROCMACRO(glGetAttachedShaders)
  GETGLPROCMACRO(glGetAttribLocation)
  GETGLPROCMACRO(glGetProgramiv)
  GETGLPROCMACRO(glGetProgramInfoLog)
  GETGLPROCMACRO(glGetShaderiv)
  GETGLPROCMACRO(glGetShaderInfoLog)
  GETGLPROCMACRO(glGetShaderSource)
  GETGLPROCMACRO(glGetUniformLocation)
  GETGLPROCMACRO(glGetUniformfv)
  GETGLPROCMACRO(glGetUniformiv)
  GETGLPROCMACRO(glGetVertexAttribdv)
  GETGLPROCMACRO(glGetVertexAttribfv)
  GETGLPROCMACRO(glGetVertexAttribiv)
  GETGLPROCMACRO(glGetVertexAttribPointerv)
  GETGLPROCMACRO(glIsProgram)
  GETGLPROCMACRO(glIsShader)
  GETGLPROCMACRO(glLinkProgram)
  GETGLPROCMACRO(glShaderSource)
  GETGLPROCMACRO(glUseProgram)
  GETGLPROCMACRO(glUniform1f)
  GETGLPROCMACRO(glUniform2f)
  GETGLPROCMACRO(glUniform3f)
  GETGLPROCMACRO(glUniform4f)
  GETGLPROCMACRO(glUniform1i)
  GETGLPROCMACRO(glUniform2i)
  GETGLPROCMACRO(glUniform3i)
  GETGLPROCMACRO(glUniform4i)
  GETGLPROCMACRO(glUniform1fv)
  GETGLPROCMACRO(glUniform2fv)
  GETGLPROCMACRO(glUniform3fv)
  GETGLPROCMACRO(glUniform4fv)
  GETGLPROCMACRO(glUniform1iv)
  GETGLPROCMACRO(glUniform2iv)
  GETGLPROCMACRO(glUniform3iv)
  GETGLPROCMACRO(glUniform4iv)
  GETGLPROCMACRO(glUniformMatrix2fv)
  GETGLPROCMACRO(glUniformMatrix3fv)
  GETGLPROCMACRO(glUniformMatrix4fv)
  GETGLPROCMACRO(glValidateProgram)
  GETGLPROCMACRO(glVertexAttrib1d)
  GETGLPROCMACRO(glVertexAttrib1dv)
  GETGLPROCMACRO(glVertexAttrib1f)
  GETGLPROCMACRO(glVertexAttrib1fv)
  GETGLPROCMACRO(glVertexAttrib1s)
  GETGLPROCMACRO(glVertexAttrib1sv)
  GETGLPROCMACRO(glVertexAttrib2d)
  GETGLPROCMACRO(glVertexAttrib2dv)
  GETGLPROCMACRO(glVertexAttrib2f)
  GETGLPROCMACRO(glVertexAttrib2fv)
  GETGLPROCMACRO(glVertexAttrib2s)
  GETGLPROCMACRO(glVertexAttrib2sv)
  GETGLPROCMACRO(glVertexAttrib3d)
  GETGLPROCMACRO(glVertexAttrib3dv)
  GETGLPROCMACRO(glVertexAttrib3f)
  GETGLPROCMACRO(glVertexAttrib3fv)
  GETGLPROCMACRO(glVertexAttrib3s)
  GETGLPROCMACRO(glVertexAttrib3sv)
  GETGLPROCMACRO(glVertexAttrib4Nbv)
  GETGLPROCMACRO(glVertexAttrib4Niv)
  GETGLPROCMACRO(glVertexAttrib4Nsv)
  GETGLPROCMACRO(glVertexAttrib4Nub)
  GETGLPROCMACRO(glVertexAttrib4Nubv)
  GETGLPROCMACRO(glVertexAttrib4Nuiv)
  GETGLPROCMACRO(glVertexAttrib4Nusv)
  GETGLPROCMACRO(glVertexAttrib4bv)
  GETGLPROCMACRO(glVertexAttrib4d)
  GETGLPROCMACRO(glVertexAttrib4dv)
  GETGLPROCMACRO(glVertexAttrib4f)
  GETGLPROCMACRO(glVertexAttrib4fv)
  GETGLPROCMACRO(glVertexAttrib4iv)
  GETGLPROCMACRO(glVertexAttrib4s)
  GETGLPROCMACRO(glVertexAttrib4sv)
  GETGLPROCMACRO(glVertexAttrib4ubv)
  GETGLPROCMACRO(glVertexAttrib4uiv)
  GETGLPROCMACRO(glVertexAttrib4usv)
  GETGLPROCMACRO(glVertexAttribPointer)

  /// OpenGL 2.1 funcs =================------------------------------
  if(major== 2 && minor< 1)
    return;

  GETGLPROCMACRO(glUniformMatrix2x3fv)
  GETGLPROCMACRO(glUniformMatrix3x2fv)
  GETGLPROCMACRO(glUniformMatrix2x4fv)
  GETGLPROCMACRO(glUniformMatrix4x2fv)
  GETGLPROCMACRO(glUniformMatrix3x4fv)
  GETGLPROCMACRO(glUniformMatrix4x3fv)

  /// OpenGL 3.0 funcs =================------------------------------
  if(major< 3)
    return;

  GETGLPROCMACRO(glColorMaski)
  GETGLPROCMACRO(glGetBooleani_v)
  GETGLPROCMACRO(glGetIntegeri_v)
  GETGLPROCMACRO(glEnablei)
  GETGLPROCMACRO(glDisablei)
  GETGLPROCMACRO(glIsEnabledi)
  GETGLPROCMACRO(glBeginTransformFeedback)
  GETGLPROCMACRO(glEndTransformFeedback)
  GETGLPROCMACRO(glBindBufferRange)
  GETGLPROCMACRO(glBindBufferBase)
  GETGLPROCMACRO(glTransformFeedbackVaryings)
  GETGLPROCMACRO(glGetTransformFeedbackVarying)
  GETGLPROCMACRO(glClampColor)
  GETGLPROCMACRO(glBeginConditionalRender)
  GETGLPROCMACRO(glEndConditionalRender)
  GETGLPROCMACRO(glVertexAttribIPointer)
  GETGLPROCMACRO(glGetVertexAttribIiv)
  GETGLPROCMACRO(glGetVertexAttribIuiv)
  GETGLPROCMACRO(glVertexAttribI1i)
  GETGLPROCMACRO(glVertexAttribI2i)
  GETGLPROCMACRO(glVertexAttribI3i)
  GETGLPROCMACRO(glVertexAttribI4i)
  GETGLPROCMACRO(glVertexAttribI1ui)
  GETGLPROCMACRO(glVertexAttribI2ui)
  GETGLPROCMACRO(glVertexAttribI3ui)
  GETGLPROCMACRO(glVertexAttribI4ui)
  GETGLPROCMACRO(glVertexAttribI1iv)
  GETGLPROCMACRO(glVertexAttribI2iv)
  GETGLPROCMACRO(glVertexAttribI3iv)
  GETGLPROCMACRO(glVertexAttribI4iv)
  GETGLPROCMACRO(glVertexAttribI1uiv)
  GETGLPROCMACRO(glVertexAttribI2uiv)
  GETGLPROCMACRO(glVertexAttribI3uiv)
  GETGLPROCMACRO(glVertexAttribI4uiv)
  GETGLPROCMACRO(glVertexAttribI4bv)
  GETGLPROCMACRO(glVertexAttribI4sv)
  GETGLPROCMACRO(glVertexAttribI4ubv)
  GETGLPROCMACRO(glVertexAttribI4usv)
  GETGLPROCMACRO(glGetUniformuiv)
  GETGLPROCMACRO(glBindFragDataLocation)
  GETGLPROCMACRO(glGetFragDataLocation)
  GETGLPROCMACRO(glUniform1ui)
  GETGLPROCMACRO(glUniform2ui)
  GETGLPROCMACRO(glUniform3ui)
  GETGLPROCMACRO(glUniform4ui)
  GETGLPROCMACRO(glUniform1uiv)
  GETGLPROCMACRO(glUniform2uiv)
  GETGLPROCMACRO(glUniform3uiv)
  GETGLPROCMACRO(glUniform4uiv)
  GETGLPROCMACRO(glTexParameterIiv)
  GETGLPROCMACRO(glTexParameterIuiv)
  GETGLPROCMACRO(glGetTexParameterIiv)
  GETGLPROCMACRO(glGetTexParameterIuiv)
  GETGLPROCMACRO(glClearBufferiv)
  GETGLPROCMACRO(glClearBufferuiv)
  GETGLPROCMACRO(glClearBufferfv)
  GETGLPROCMACRO(glClearBufferfi)
  GETGLPROCMACRO(glGetStringi)
  GETGLPROCMACRO(glIsRenderbuffer)
  GETGLPROCMACRO(glBindRenderbuffer)
  GETGLPROCMACRO(glDeleteRenderbuffers)
  GETGLPROCMACRO(glGenRenderbuffers)
  GETGLPROCMACRO(glRenderbufferStorage)
  GETGLPROCMACRO(glGetRenderbufferParameteriv)
  GETGLPROCMACRO(glIsFramebuffer)
  GETGLPROCMACRO(glBindFramebuffer)
  GETGLPROCMACRO(glDeleteFramebuffers)
  GETGLPROCMACRO(glGenFramebuffers)
  GETGLPROCMACRO(glCheckFramebufferStatus)
  GETGLPROCMACRO(glFramebufferTexture1D)
  GETGLPROCMACRO(glFramebufferTexture2D)
  GETGLPROCMACRO(glFramebufferTexture3D)
  GETGLPROCMACRO(glFramebufferRenderbuffer)
  GETGLPROCMACRO(glGetFramebufferAttachmentParameteriv)
  GETGLPROCMACRO(glGenerateMipmap)
  GETGLPROCMACRO(glBlitFramebuffer)
  GETGLPROCMACRO(glRenderbufferStorageMultisample)
  GETGLPROCMACRO(glFramebufferTextureLayer)
  GETGLPROCMACRO(glMapBufferRange)
  GETGLPROCMACRO(glFlushMappedBufferRange)
  GETGLPROCMACRO(glBindVertexArray)
  GETGLPROCMACRO(glDeleteVertexArrays)
  GETGLPROCMACRO(glGenVertexArrays)
  GETGLPROCMACRO(glIsVertexArray)

  /// OpenGL 3.1 funcs =================------------------------------
  if(major== 3 && minor< 1)
    return;

  GETGLPROCMACRO(glDrawArraysInstanced)
  GETGLPROCMACRO(glDrawElementsInstanced)
  GETGLPROCMACRO(glTexBuffer)
  GETGLPROCMACRO(glPrimitiveRestartIndex)
  GETGLPROCMACRO(glCopyBufferSubData)
  GETGLPROCMACRO(glGetUniformIndices)
  GETGLPROCMACRO(glGetActiveUniformsiv)
  GETGLPROCMACRO(glGetActiveUniformName)
  GETGLPROCMACRO(glGetUniformBlockIndex)
  GETGLPROCMACRO(glGetActiveUniformBlockiv)
  GETGLPROCMACRO(glGetActiveUniformBlockName)
  GETGLPROCMACRO(glUniformBlockBinding)

  /// OpenGL 3.2 funcs =================------------------------------
  if(major== 3 && minor< 2)
    return;

  GETGLPROCMACRO(glDrawElementsBaseVertex)
  GETGLPROCMACRO(glDrawRangeElementsBaseVertex)
  GETGLPROCMACRO(glDrawElementsInstancedBaseVertex)
  GETGLPROCMACRO(glMultiDrawElementsBaseVertex)
  GETGLPROCMACRO(glProvokingVertex)
  GETGLPROCMACRO(glFenceSync)
  GETGLPROCMACRO(glIsSync)
  GETGLPROCMACRO(glDeleteSync)
  GETGLPROCMACRO(glClientWaitSync)
  GETGLPROCMACRO(glWaitSync)
  GETGLPROCMACRO(glGetInteger64v)
  GETGLPROCMACRO(glGetSynciv)
  GETGLPROCMACRO(glGetInteger64i_v)
  GETGLPROCMACRO(glGetBufferParameteri64v)
  GETGLPROCMACRO(glFramebufferTexture)
  GETGLPROCMACRO(glTexImage2DMultisample)
  GETGLPROCMACRO(glTexImage3DMultisample)
  GETGLPROCMACRO(glGetMultisamplefv)
  GETGLPROCMACRO(glSampleMaski)

  /// OpenGL 3.3 funcs =================------------------------------
  if(major== 3 && minor< 3)
    return;

  GETGLPROCMACRO(glBindFragDataLocationIndexed)
  GETGLPROCMACRO(glGetFragDataIndex)
  GETGLPROCMACRO(glGenSamplers)
  GETGLPROCMACRO(glDeleteSamplers)
  GETGLPROCMACRO(glIsSampler)
  GETGLPROCMACRO(glBindSampler)
  GETGLPROCMACRO(glSamplerParameteri)
  GETGLPROCMACRO(glSamplerParameteriv)
  GETGLPROCMACRO(glSamplerParameterf)
  GETGLPROCMACRO(glSamplerParameterfv)
  GETGLPROCMACRO(glSamplerParameterIiv)
  GETGLPROCMACRO(glSamplerParameterIuiv)
  GETGLPROCMACRO(glGetSamplerParameteriv)
  GETGLPROCMACRO(glGetSamplerParameterIiv)
  GETGLPROCMACRO(glGetSamplerParameterfv)
  GETGLPROCMACRO(glGetSamplerParameterIuiv)
  GETGLPROCMACRO(glQueryCounter)
  GETGLPROCMACRO(glGetQueryObjecti64v)
  GETGLPROCMACRO(glGetQueryObjectui64v)
  GETGLPROCMACRO(glVertexAttribDivisor)
  GETGLPROCMACRO(glVertexAttribP1ui)
  GETGLPROCMACRO(glVertexAttribP1uiv)
  GETGLPROCMACRO(glVertexAttribP2ui)
  GETGLPROCMACRO(glVertexAttribP2uiv)
  GETGLPROCMACRO(glVertexAttribP3ui)
  GETGLPROCMACRO(glVertexAttribP3uiv)
  GETGLPROCMACRO(glVertexAttribP4ui)
  GETGLPROCMACRO(glVertexAttribP4uiv)
  GETGLPROCMACRO(glVertexP2ui)
  GETGLPROCMACRO(glVertexP2uiv)
  GETGLPROCMACRO(glVertexP3ui)
  GETGLPROCMACRO(glVertexP3uiv)
  GETGLPROCMACRO(glVertexP4ui)
  GETGLPROCMACRO(glVertexP4uiv)
  GETGLPROCMACRO(glTexCoordP1ui)
  GETGLPROCMACRO(glTexCoordP1uiv)
  GETGLPROCMACRO(glTexCoordP2ui)
  GETGLPROCMACRO(glTexCoordP2uiv)
  GETGLPROCMACRO(glTexCoordP3ui)
  GETGLPROCMACRO(glTexCoordP3uiv)
  GETGLPROCMACRO(glTexCoordP4ui)
  GETGLPROCMACRO(glTexCoordP4uiv)
  GETGLPROCMACRO(glMultiTexCoordP1ui)
  GETGLPROCMACRO(glMultiTexCoordP1uiv)
  GETGLPROCMACRO(glMultiTexCoordP2ui)
  GETGLPROCMACRO(glMultiTexCoordP2uiv)
  GETGLPROCMACRO(glMultiTexCoordP3ui)
  GETGLPROCMACRO(glMultiTexCoordP3uiv)
  GETGLPROCMACRO(glMultiTexCoordP4ui)
  GETGLPROCMACRO(glMultiTexCoordP4uiv)
  GETGLPROCMACRO(glNormalP3ui)
  GETGLPROCMACRO(glNormalP3uiv)
  GETGLPROCMACRO(glColorP3ui)
  GETGLPROCMACRO(glColorP3uiv)
  GETGLPROCMACRO(glColorP4ui)
  GETGLPROCMACRO(glColorP4uiv)
  GETGLPROCMACRO(glSecondaryColorP3ui)
  GETGLPROCMACRO(glSecondaryColorP3uiv)

  /// OpenGL 4.0 funcs =================------------------------------
  if(major< 4)
    return;

  GETGLPROCMACRO(glMinSampleShading)
  GETGLPROCMACRO(glBlendEquationi)
  GETGLPROCMACRO(glBlendEquationSeparatei)
  GETGLPROCMACRO(glBlendFunci)
  GETGLPROCMACRO(glBlendFuncSeparatei)
  GETGLPROCMACRO(glDrawArraysIndirect)
  GETGLPROCMACRO(glDrawElementsIndirect)
  GETGLPROCMACRO(glUniform1d)
  GETGLPROCMACRO(glUniform2d)
  GETGLPROCMACRO(glUniform3d)
  GETGLPROCMACRO(glUniform4d)
  GETGLPROCMACRO(glUniform1dv)
  GETGLPROCMACRO(glUniform2dv)
  GETGLPROCMACRO(glUniform3dv)
  GETGLPROCMACRO(glUniform4dv)
  GETGLPROCMACRO(glUniformMatrix2dv)
  GETGLPROCMACRO(glUniformMatrix3dv)
  GETGLPROCMACRO(glUniformMatrix4dv)
  GETGLPROCMACRO(glUniformMatrix2x3dv)
  GETGLPROCMACRO(glUniformMatrix2x4dv)
  GETGLPROCMACRO(glUniformMatrix3x2dv)
  GETGLPROCMACRO(glUniformMatrix3x4dv)
  GETGLPROCMACRO(glUniformMatrix4x2dv)
  GETGLPROCMACRO(glUniformMatrix4x3dv)
  GETGLPROCMACRO(glGetUniformdv)
  GETGLPROCMACRO(glGetSubroutineUniformLocation)
  GETGLPROCMACRO(glGetSubroutineIndex)
  GETGLPROCMACRO(glGetActiveSubroutineUniformiv)
  GETGLPROCMACRO(glGetActiveSubroutineUniformName)
  GETGLPROCMACRO(glGetActiveSubroutineName)
  GETGLPROCMACRO(glUniformSubroutinesuiv)
  GETGLPROCMACRO(glGetUniformSubroutineuiv)
  GETGLPROCMACRO(glGetProgramStageiv)
  GETGLPROCMACRO(glPatchParameteri)
  GETGLPROCMACRO(glPatchParameterfv)
  GETGLPROCMACRO(glBindTransformFeedback)
  GETGLPROCMACRO(glDeleteTransformFeedbacks)
  GETGLPROCMACRO(glGenTransformFeedbacks)
  GETGLPROCMACRO(glIsTransformFeedback)
  GETGLPROCMACRO(glPauseTransformFeedback)
  GETGLPROCMACRO(glResumeTransformFeedback)
  GETGLPROCMACRO(glDrawTransformFeedback)
  GETGLPROCMACRO(glDrawTransformFeedbackStream)
  GETGLPROCMACRO(glBeginQueryIndexed)
  GETGLPROCMACRO(glEndQueryIndexed)
  GETGLPROCMACRO(glGetQueryIndexediv)

  /// OpenGL 4.1 funcs =================------------------------------
  if(major== 4 && minor< 1)
    return;
  GETGLPROCMACRO(glReleaseShaderCompiler)
  GETGLPROCMACRO(glShaderBinary)
  GETGLPROCMACRO(glGetShaderPrecisionFormat)
  GETGLPROCMACRO(glDepthRangef)
  GETGLPROCMACRO(glClearDepthf)
  GETGLPROCMACRO(glGetProgramBinary)
  GETGLPROCMACRO(glProgramBinary)
  GETGLPROCMACRO(glProgramParameteri)
  GETGLPROCMACRO(glUseProgramStages)
  GETGLPROCMACRO(glActiveShaderProgram)
  GETGLPROCMACRO(glCreateShaderProgramv)
  GETGLPROCMACRO(glBindProgramPipeline)
  GETGLPROCMACRO(glDeleteProgramPipelines)
  GETGLPROCMACRO(glGenProgramPipelines)
  GETGLPROCMACRO(glIsProgramPipeline)
  GETGLPROCMACRO(glGetProgramPipelineiv)
  GETGLPROCMACRO(glProgramUniform1i)
  GETGLPROCMACRO(glProgramUniform1iv)
  GETGLPROCMACRO(glProgramUniform1f)
  GETGLPROCMACRO(glProgramUniform1fv)
  GETGLPROCMACRO(glProgramUniform1d)
  GETGLPROCMACRO(glProgramUniform1dv)
  GETGLPROCMACRO(glProgramUniform1ui)
  GETGLPROCMACRO(glProgramUniform1uiv)
  GETGLPROCMACRO(glProgramUniform2i)
  GETGLPROCMACRO(glProgramUniform2iv)
  GETGLPROCMACRO(glProgramUniform2f)
  GETGLPROCMACRO(glProgramUniform2fv)
  GETGLPROCMACRO(glProgramUniform2d)
  GETGLPROCMACRO(glProgramUniform2dv)
  GETGLPROCMACRO(glProgramUniform2ui)
  GETGLPROCMACRO(glProgramUniform2uiv)
  GETGLPROCMACRO(glProgramUniform3i)
  GETGLPROCMACRO(glProgramUniform3iv)
  GETGLPROCMACRO(glProgramUniform3f)
  GETGLPROCMACRO(glProgramUniform3fv)
  GETGLPROCMACRO(glProgramUniform3d)
  GETGLPROCMACRO(glProgramUniform3dv)
  GETGLPROCMACRO(glProgramUniform3ui)
  GETGLPROCMACRO(glProgramUniform3uiv)
  GETGLPROCMACRO(glProgramUniform4i)
  GETGLPROCMACRO(glProgramUniform4iv)
  GETGLPROCMACRO(glProgramUniform4f)
  GETGLPROCMACRO(glProgramUniform4fv)
  GETGLPROCMACRO(glProgramUniform4d)
  GETGLPROCMACRO(glProgramUniform4dv)
  GETGLPROCMACRO(glProgramUniform4ui)
  GETGLPROCMACRO(glProgramUniform4uiv)
  GETGLPROCMACRO(glProgramUniformMatrix2fv)
  GETGLPROCMACRO(glProgramUniformMatrix3fv)
  GETGLPROCMACRO(glProgramUniformMatrix4fv)
  GETGLPROCMACRO(glProgramUniformMatrix2dv)
  GETGLPROCMACRO(glProgramUniformMatrix3dv)
  GETGLPROCMACRO(glProgramUniformMatrix4dv)
  GETGLPROCMACRO(glProgramUniformMatrix2x3fv)
  GETGLPROCMACRO(glProgramUniformMatrix3x2fv)
  GETGLPROCMACRO(glProgramUniformMatrix2x4fv)
  GETGLPROCMACRO(glProgramUniformMatrix4x2fv)
  GETGLPROCMACRO(glProgramUniformMatrix3x4fv)
  GETGLPROCMACRO(glProgramUniformMatrix4x3fv)
  GETGLPROCMACRO(glProgramUniformMatrix2x3dv)
  GETGLPROCMACRO(glProgramUniformMatrix3x2dv)
  GETGLPROCMACRO(glProgramUniformMatrix2x4dv)
  GETGLPROCMACRO(glProgramUniformMatrix4x2dv)
  GETGLPROCMACRO(glProgramUniformMatrix3x4dv)
  GETGLPROCMACRO(glProgramUniformMatrix4x3dv)
  GETGLPROCMACRO(glValidateProgramPipeline)
  GETGLPROCMACRO(glGetProgramPipelineInfoLog)
  GETGLPROCMACRO(glVertexAttribL1d)
  GETGLPROCMACRO(glVertexAttribL2d)
  GETGLPROCMACRO(glVertexAttribL3d)
  GETGLPROCMACRO(glVertexAttribL4d)
  GETGLPROCMACRO(glVertexAttribL1dv)
  GETGLPROCMACRO(glVertexAttribL2dv)
  GETGLPROCMACRO(glVertexAttribL3dv)
  GETGLPROCMACRO(glVertexAttribL4dv)
  GETGLPROCMACRO(glVertexAttribLPointer)
  GETGLPROCMACRO(glGetVertexAttribLdv)
  GETGLPROCMACRO(glViewportArrayv)
  GETGLPROCMACRO(glViewportIndexedf)
  GETGLPROCMACRO(glViewportIndexedfv)
  GETGLPROCMACRO(glScissorArrayv)
  GETGLPROCMACRO(glScissorIndexed)
  GETGLPROCMACRO(glScissorIndexedv)
  GETGLPROCMACRO(glDepthRangeArrayv)
  GETGLPROCMACRO(glDepthRangeIndexed)
  GETGLPROCMACRO(glGetFloati_v)
  GETGLPROCMACRO(glGetDoublei_v)

  /// OpenGL 4.2 funcs =================------------------------------
  if(major== 4 && minor< 2)
    return;

  GETGLPROCMACRO(glDrawArraysInstancedBaseInstance)
  GETGLPROCMACRO(glDrawElementsInstancedBaseInstance)
  GETGLPROCMACRO(glDrawElementsInstancedBaseVertexBaseInstance)
  GETGLPROCMACRO(glGetInternalformativ)
  GETGLPROCMACRO(glGetActiveAtomicCounterBufferiv)
  GETGLPROCMACRO(glBindImageTexture)
  GETGLPROCMACRO(glMemoryBarrier)
  GETGLPROCMACRO(glTexStorage1D)
  GETGLPROCMACRO(glTexStorage2D)
  GETGLPROCMACRO(glTexStorage3D)
  GETGLPROCMACRO(glDrawTransformFeedbackInstanced)
  GETGLPROCMACRO(glDrawTransformFeedbackStreamInstanced)

  /// OpenGL 4.3 funcs =================------------------------------
  if(major== 4 && minor< 3)
    return;

  GETGLPROCMACRO(glClearBufferData)
  GETGLPROCMACRO(glClearBufferSubData)
  GETGLPROCMACRO(glDispatchCompute)
  GETGLPROCMACRO(glDispatchComputeIndirect)
  GETGLPROCMACRO(glCopyImageSubData)
  GETGLPROCMACRO(glFramebufferParameteri)
  GETGLPROCMACRO(glGetFramebufferParameteriv)
  GETGLPROCMACRO(glGetInternalformati64v)
  GETGLPROCMACRO(glInvalidateTexSubImage)
  GETGLPROCMACRO(glInvalidateTexImage)
  GETGLPROCMACRO(glInvalidateBufferSubData)
  GETGLPROCMACRO(glInvalidateBufferData)
  GETGLPROCMACRO(glInvalidateFramebuffer)
  GETGLPROCMACRO(glInvalidateSubFramebuffer)
  GETGLPROCMACRO(glMultiDrawArraysIndirect)
  GETGLPROCMACRO(glMultiDrawElementsIndirect)
  GETGLPROCMACRO(glGetProgramInterfaceiv)
  GETGLPROCMACRO(glGetProgramResourceIndex)
  GETGLPROCMACRO(glGetProgramResourceName)
  GETGLPROCMACRO(glGetProgramResourceiv)
  GETGLPROCMACRO(glGetProgramResourceLocation)
  GETGLPROCMACRO(glGetProgramResourceLocationIndex)
  GETGLPROCMACRO(glShaderStorageBlockBinding)
  GETGLPROCMACRO(glTexBufferRange)
  GETGLPROCMACRO(glTexStorage2DMultisample)
  GETGLPROCMACRO(glTexStorage3DMultisample)
  GETGLPROCMACRO(glTextureView)
  GETGLPROCMACRO(glBindVertexBuffer)
  GETGLPROCMACRO(glVertexAttribFormat)
  GETGLPROCMACRO(glVertexAttribIFormat)
  GETGLPROCMACRO(glVertexAttribLFormat)
  GETGLPROCMACRO(glVertexAttribBinding)
  GETGLPROCMACRO(glVertexBindingDivisor)
  GETGLPROCMACRO(glDebugMessageControl)
  GETGLPROCMACRO(glDebugMessageInsert)
  GETGLPROCMACRO(glDebugMessageCallback)
  GETGLPROCMACRO(glGetDebugMessageLog)
  GETGLPROCMACRO(glPushDebugGroup)
  GETGLPROCMACRO(glPopDebugGroup)
  GETGLPROCMACRO(glObjectLabel)
  GETGLPROCMACRO(glGetObjectLabel)
  GETGLPROCMACRO(glObjectPtrLabel)
  GETGLPROCMACRO(glGetObjectPtrLabel)
  
  /// OpenGL 4.4 funcs =================------------------------------
  if(major== 4 && minor< 4)
    return;

  GETGLPROCMACRO(glBufferStorage)
  GETGLPROCMACRO(glClearTexImage)
  GETGLPROCMACRO(glClearTexSubImage)
  GETGLPROCMACRO(glBindBuffersBase)
  GETGLPROCMACRO(glBindBuffersRange)
  GETGLPROCMACRO(glBindTextures)
  GETGLPROCMACRO(glBindSamplers)
  GETGLPROCMACRO(glBindImageTextures)
  GETGLPROCMACRO(glBindVertexBuffers)


}



///=================================///
// extensions not in ARB or EXT list //
///=================================///
void getOTHERfuncs(osiRenderer *r) {
  #ifdef OS_WIN
  if(r->glOTHERlist[0].avaible) {    /// GL_ARB_imaging
    GETGLPROCMACRO(glColorTable)
    GETGLPROCMACRO(glColorTableParameterfv)
    GETGLPROCMACRO(glColorTableParameteriv)
    GETGLPROCMACRO(glCopyColorTable)
    GETGLPROCMACRO(glGetColorTable)
    GETGLPROCMACRO(glGetColorTableParameterfv)
    GETGLPROCMACRO(glGetColorTableParameteriv)
    GETGLPROCMACRO(glColorSubTable)
    GETGLPROCMACRO(glCopyColorSubTable)
    GETGLPROCMACRO(glConvolutionFilter1D)
    GETGLPROCMACRO(glConvolutionFilter2D)
    GETGLPROCMACRO(glConvolutionParameterf)
    GETGLPROCMACRO(glConvolutionParameterfv)
    GETGLPROCMACRO(glConvolutionParameteri)
    GETGLPROCMACRO(glConvolutionParameteriv)
    GETGLPROCMACRO(glCopyConvolutionFilter1D)
    GETGLPROCMACRO(glCopyConvolutionFilter2D)
    GETGLPROCMACRO(glGetConvolutionFilter)
    GETGLPROCMACRO(glGetConvolutionParameterfv)
    GETGLPROCMACRO(glGetConvolutionParameteriv)
    GETGLPROCMACRO(glGetSeparableFilter)
    GETGLPROCMACRO(glSeparableFilter2D)
    GETGLPROCMACRO(glGetHistogram)
    GETGLPROCMACRO(glGetHistogramParameterfv)
    GETGLPROCMACRO(glGetHistogramParameteriv)
    GETGLPROCMACRO(glGetMinmax)
    GETGLPROCMACRO(glGetMinmaxParameterfv)
    GETGLPROCMACRO(glGetMinmaxParameteriv)
    GETGLPROCMACRO(glHistogram)
    GETGLPROCMACRO(glMinmax)
    GETGLPROCMACRO(glResetHistogram)
    GETGLPROCMACRO(glResetMinmax)
  }
  #endif /// OS_WIN
  if(r->glOTHERlist[1].avaible) {    /// GL_ARB_bindless_texture
    GETGLPROCMACRO(glGetTextureHandleARB)
    GETGLPROCMACRO(glGetTextureSamplerHandleARB)
    GETGLPROCMACRO(glMakeTextureHandleResidentARB)
    GETGLPROCMACRO(glMakeTextureHandleNonResidentARB)
    GETGLPROCMACRO(glGetImageHandleARB)
    GETGLPROCMACRO(glMakeImageHandleResidentARB)
    GETGLPROCMACRO(glMakeImageHandleNonResidentARB)
    GETGLPROCMACRO(glUniformHandleui64ARB)
    GETGLPROCMACRO(glUniformHandleui64vARB)
    GETGLPROCMACRO(glProgramUniformHandleui64ARB)
    GETGLPROCMACRO(glProgramUniformHandleui64vARB)
    GETGLPROCMACRO(glIsTextureHandleResidentARB)
    GETGLPROCMACRO(glIsImageHandleResidentARB)
    GETGLPROCMACRO(glVertexAttribL1ui64ARB)
    GETGLPROCMACRO(glVertexAttribL1ui64vARB)
    GETGLPROCMACRO(glGetVertexAttribLui64vARB)
  }
  if(r->glOTHERlist[2].avaible) {    /// GL_INGR_blend_func_separate ?
    GETGLPROCMACRO(glBlendFuncSeparateINGR)
  }
  if(r->glOTHERlist[3].avaible) {    /// GL_SGIX_fragment_lighting ?
    GETGLPROCMACRO(glFragmentColorMaterialSGIX)
    GETGLPROCMACRO(glFragmentLightfSGIX)
    GETGLPROCMACRO(glFragmentLightfvSGIX)
    GETGLPROCMACRO(glFragmentLightiSGIX)
    GETGLPROCMACRO(glFragmentLightivSGIX)
    GETGLPROCMACRO(glFragmentLightModelfSGIX)
    GETGLPROCMACRO(glFragmentLightModelfvSGIX)
    GETGLPROCMACRO(glFragmentLightModeliSGIX)
    GETGLPROCMACRO(glFragmentLightModelivSGIX)
    GETGLPROCMACRO(glFragmentMaterialfSGIX)
    GETGLPROCMACRO(glFragmentMaterialfvSGIX)
    GETGLPROCMACRO(glFragmentMaterialiSGIX)
    GETGLPROCMACRO(glFragmentMaterialivSGIX)
    GETGLPROCMACRO(glGetFragmentLightfvSGIX)
    GETGLPROCMACRO(glGetFragmentLightivSGIX)
    GETGLPROCMACRO(glGetFragmentMaterialfvSGIX)
    GETGLPROCMACRO(glGetFragmentMaterialivSGIX)
    GETGLPROCMACRO(glLightEnviSGIX)
  }
  if(r->glOTHERlist[4].avaible) {    /// GL_SGIX_polynomial_ffd
    GETGLPROCMACRO(glDeformationMap3dSGIX)
    GETGLPROCMACRO(glDeformationMap3fSGIX)
    GETGLPROCMACRO(glDeformSGIX)
    GETGLPROCMACRO(glLoadIdentityDeformationMapSGIX)
  }
  if(r->glOTHERlist[5].avaible) {    /// GL_SGIS_point_parameters
    GETGLPROCMACRO(glPointParameterfSGIS)
    GETGLPROCMACRO(glPointParameterfvSGIS)
  }
  if(r->glOTHERlist[6].avaible) {    /// GL_SGIX_igloo_interface
    GETGLPROCMACRO(glIglooInterfaceSGIX)

  }
  #ifdef OS_WIN
  if(r->glOTHERlist[7].avaible) {    /// WGL_NV_vertex_array_range
    GETGLPROCMACRO(wglAllocateMemoryNV)
    GETGLPROCMACRO(wglFreeMemoryNV)
  }
  #endif /// OS_WIN

}



///=============================///
// ARB extensions funcs aquiring //
///=============================///
void getARBfuncs(osiRenderer *r) {

  if(r->glARBlist[0].avaible) {             /// #1 GL_ARB_multitexture  http://www.opengl.org/registry/specs/ARB/multitexture.txt
    GETGLPROCMACRO(glActiveTextureARB)
    GETGLPROCMACRO(glClientActiveTextureARB)
    GETGLPROCMACRO(glMultiTexCoord1dARB)
    GETGLPROCMACRO(glMultiTexCoord1dvARB)
    GETGLPROCMACRO(glMultiTexCoord1fARB)
    GETGLPROCMACRO(glMultiTexCoord1fvARB)
    GETGLPROCMACRO(glMultiTexCoord1iARB)
    GETGLPROCMACRO(glMultiTexCoord1ivARB)
    GETGLPROCMACRO(glMultiTexCoord1sARB)
    GETGLPROCMACRO(glMultiTexCoord1svARB)
    GETGLPROCMACRO(glMultiTexCoord2dARB)
    GETGLPROCMACRO(glMultiTexCoord2dvARB)
    GETGLPROCMACRO(glMultiTexCoord2fARB)
    GETGLPROCMACRO(glMultiTexCoord2fvARB)
    GETGLPROCMACRO(glMultiTexCoord2iARB)
    GETGLPROCMACRO(glMultiTexCoord2ivARB)
    GETGLPROCMACRO(glMultiTexCoord2sARB)
    GETGLPROCMACRO(glMultiTexCoord2svARB)
    GETGLPROCMACRO(glMultiTexCoord3dARB)
    GETGLPROCMACRO(glMultiTexCoord3dvARB)
    GETGLPROCMACRO(glMultiTexCoord3fARB)
    GETGLPROCMACRO(glMultiTexCoord3fvARB)
    GETGLPROCMACRO(glMultiTexCoord3iARB)
    GETGLPROCMACRO(glMultiTexCoord3ivARB)
    GETGLPROCMACRO(glMultiTexCoord3sARB)
    GETGLPROCMACRO(glMultiTexCoord3svARB)
    GETGLPROCMACRO(glMultiTexCoord4dARB)
    GETGLPROCMACRO(glMultiTexCoord4dvARB)
    GETGLPROCMACRO(glMultiTexCoord4fARB)
    GETGLPROCMACRO(glMultiTexCoord4fvARB)
    GETGLPROCMACRO(glMultiTexCoord4iARB)
    GETGLPROCMACRO(glMultiTexCoord4ivARB)
    GETGLPROCMACRO(glMultiTexCoord4sARB)
    GETGLPROCMACRO(glMultiTexCoord4svARB)
  }

  #ifdef OS_LINUX
  //if(r->glARBlist[1].avaible)       /// #2 GLX_ARB_get_proc_address  http://www.opengl.org/registry/specs/ARB/get_proc_address.txt
    //GETGLPROCMACRO(glXGetProcAddressARB)
  #endif /// OS_LINUX
  
  if(r->glARBlist[2].avaible) {     /// #3 GL_ARB_transpose_matrix  http://www.opengl.org/registry/specs/ARB/transpose_matrix.txt
    GETGLPROCMACRO(glLoadTransposeMatrixfARB)
    GETGLPROCMACRO(glLoadTransposeMatrixdARB)
    GETGLPROCMACRO(glMultTransposeMatrixfARB)
    GETGLPROCMACRO(glMultTransposeMatrixdARB)
  }

  #ifdef OS_WIN
  if(r->glARBlist[3].avaible) {     /// #4 WGL_ARB_buffer_region  http://www.opengl.org/registry/specs/ARB/wgl_buffer_region.txt
    GETGLPROCMACRO(wglCreateBufferRegionARB)
    GETGLPROCMACRO(wglDeleteBufferRegionARB)
    GETGLPROCMACRO(wglSaveBufferRegionARB)
    GETGLPROCMACRO(wglRestoreBufferRegionARB)
  }
  #endif /// OS_WIN

  if(r->glARBlist[4].avaible)         /// #5 GL_ARB_multisample GLX_ARB_multisample WGL_ARB_multisample  http://www.opengl.org/registry/specs/ARB/multisample.txt
    GETGLPROCMACRO(glSampleCoverageARB)

  #ifdef OS_WIN
  if(r->glARBlist[7].avaible)         /// #8 WGL_ARB_extensions_string http://www.opengl.org/registry/specs/ARB/wgl_extensions_string.txt
    GETGLPROCMACRO(wglGetExtensionsStringARB)
  if(r->glARBlist[8].avaible) {       /// #9 WGL_ARB_pixel_format http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
    GETGLPROCMACRO(wglGetPixelFormatAttribivARB)
    GETGLPROCMACRO(wglGetPixelFormatAttribfvARB)
    GETGLPROCMACRO(wglChoosePixelFormatARB)
  }
  if(r->glARBlist[9].avaible) {       /// #10 WGL_ARB_make_current_read http://www.opengl.org/registry/specs/ARB/wgl_make_current_read.txt
    GETGLPROCMACRO(wglMakeContextCurrentARB)
    GETGLPROCMACRO(wglGetCurrentReadDCARB)
  }
  if(r->glARBlist[10].avaible) {      /// #11 WGL_ARB_pbuffer http://www.opengl.org/registry/specs/ARB/wgl_pbuffer.txt
    GETGLPROCMACRO(wglCreatePbufferARB)
    GETGLPROCMACRO(wglGetPbufferDCARB)
    GETGLPROCMACRO(wglReleasePbufferDCARB)
    GETGLPROCMACRO(wglDestroyPbufferARB)
    GETGLPROCMACRO(wglQueryPbufferARB)
  }
  #endif /// OS_WIN

  if(r->glARBlist[11].avaible) {      /// #12 GL_ARB_texture_compression http://www.opengl.org/registry/specs/ARB/texture_compression.txt
    GETGLPROCMACRO(glCompressedTexImage3DARB)
    GETGLPROCMACRO(glCompressedTexImage2DARB)
    GETGLPROCMACRO(glCompressedTexImage1DARB)
    GETGLPROCMACRO(glCompressedTexSubImage3DARB)
    GETGLPROCMACRO(glCompressedTexSubImage2DARB)
    GETGLPROCMACRO(glCompressedTexSubImage1DARB)
    GETGLPROCMACRO(glGetCompressedTexImageARB)
  }
  if(r->glARBlist[13].avaible) {      /// #14 GL_ARB_point_parameters http://www.opengl.org/registry/specs/ARB/point_parameters.txt
    GETGLPROCMACRO(glPointParameterfARB)
    GETGLPROCMACRO(glPointParameterfvARB)
  }
  if(r->glARBlist[14].avaible) {      /// #15 GL_ARB_vertex_blend http://www.opengl.org/registry/specs/ARB/vertex_blend.txt
    GETGLPROCMACRO(glWeightbvARB)
    GETGLPROCMACRO(glWeightsvARB)
    GETGLPROCMACRO(glWeightivARB)
    GETGLPROCMACRO(glWeightfvARB)
    GETGLPROCMACRO(glWeightdvARB)
    GETGLPROCMACRO(glWeightubvARB)
    GETGLPROCMACRO(glWeightusvARB)
    GETGLPROCMACRO(glWeightuivARB)
    GETGLPROCMACRO(glWeightPointerARB)
    GETGLPROCMACRO(glVertexBlendARB)
  }
  if(r->glARBlist[15].avaible) {      /// #16 GL_ARB_matrix_palette http://www.opengl.org/registry/specs/ARB/matrix_palette.txt
    GETGLPROCMACRO(glCurrentPaletteMatrixARB)
    GETGLPROCMACRO(glMatrixIndexubvARB)
    GETGLPROCMACRO(glMatrixIndexusvARB)
    GETGLPROCMACRO(glMatrixIndexuivARB)
    GETGLPROCMACRO(glMatrixIndexPointerARB)
  }
  #ifdef OS_WIN
  if(r->glARBlist[19].avaible) {      /// #20 WGL_ARB_render_texture http://www.opengl.org/registry/specs/ARB/wgl_render_texture.txt
    GETGLPROCMACRO(wglBindTexImageARB)
    GETGLPROCMACRO(wglReleaseTexImageARB)
    GETGLPROCMACRO(wglSetPbufferAttribARB)
  }
  #endif /// OS_WIN
  if(r->glARBlist[24].avaible) {      /// #25 GL_ARB_window_pos http://www.opengl.org/registry/specs/ARB/window_pos.txt
    GETGLPROCMACRO(glWindowPos2dARB)
    GETGLPROCMACRO(glWindowPos2dvARB)
    GETGLPROCMACRO(glWindowPos2fARB)
    GETGLPROCMACRO(glWindowPos2fvARB)
    GETGLPROCMACRO(glWindowPos2iARB)
    GETGLPROCMACRO(glWindowPos2ivARB)
    GETGLPROCMACRO(glWindowPos2sARB)
    GETGLPROCMACRO(glWindowPos2svARB)
    GETGLPROCMACRO(glWindowPos3dARB)
    GETGLPROCMACRO(glWindowPos3dvARB)
    GETGLPROCMACRO(glWindowPos3fARB)
    GETGLPROCMACRO(glWindowPos3fvARB)
    GETGLPROCMACRO(glWindowPos3iARB)
    GETGLPROCMACRO(glWindowPos3ivARB)
    GETGLPROCMACRO(glWindowPos3sARB)
    GETGLPROCMACRO(glWindowPos3svARB)
  }
  if(r->glARBlist[25].avaible) {      /// #26 GL_ARB_vertex_program http://www.opengl.org/registry/specs/ARB/vertex_program.txt
    GETGLPROCMACRO(glVertexAttrib1dARB)
    GETGLPROCMACRO(glVertexAttrib1dvARB)
    GETGLPROCMACRO(glVertexAttrib1fARB)
    GETGLPROCMACRO(glVertexAttrib1fvARB)
    GETGLPROCMACRO(glVertexAttrib1sARB)
    GETGLPROCMACRO(glVertexAttrib1svARB)
    GETGLPROCMACRO(glVertexAttrib2dARB)
    GETGLPROCMACRO(glVertexAttrib2dvARB)
    GETGLPROCMACRO(glVertexAttrib2fARB)
    GETGLPROCMACRO(glVertexAttrib2fvARB)
    GETGLPROCMACRO(glVertexAttrib2sARB)
    GETGLPROCMACRO(glVertexAttrib2svARB)
    GETGLPROCMACRO(glVertexAttrib3dARB)
    GETGLPROCMACRO(glVertexAttrib3dvARB)
    GETGLPROCMACRO(glVertexAttrib3fARB)
    GETGLPROCMACRO(glVertexAttrib3fvARB)
    GETGLPROCMACRO(glVertexAttrib3sARB)
    GETGLPROCMACRO(glVertexAttrib3svARB)
    GETGLPROCMACRO(glVertexAttrib4NbvARB)
    GETGLPROCMACRO(glVertexAttrib4NivARB)
    GETGLPROCMACRO(glVertexAttrib4NsvARB)
    GETGLPROCMACRO(glVertexAttrib4NubARB)
    GETGLPROCMACRO(glVertexAttrib4NubvARB)
    GETGLPROCMACRO(glVertexAttrib4NuivARB)
    GETGLPROCMACRO(glVertexAttrib4NusvARB)
    GETGLPROCMACRO(glVertexAttrib4bvARB)
    GETGLPROCMACRO(glVertexAttrib4dARB)
    GETGLPROCMACRO(glVertexAttrib4dvARB)
    GETGLPROCMACRO(glVertexAttrib4fARB)
    GETGLPROCMACRO(glVertexAttrib4fvARB)
    GETGLPROCMACRO(glVertexAttrib4ivARB)
    GETGLPROCMACRO(glVertexAttrib4sARB)
    GETGLPROCMACRO(glVertexAttrib4svARB)
    GETGLPROCMACRO(glVertexAttrib4ubvARB)
    GETGLPROCMACRO(glVertexAttrib4uivARB)
    GETGLPROCMACRO(glVertexAttrib4usvARB)
    GETGLPROCMACRO(glVertexAttribPointerARB)
    GETGLPROCMACRO(glEnableVertexAttribArrayARB)
    GETGLPROCMACRO(glDisableVertexAttribArrayARB)
    GETGLPROCMACRO(glGetVertexAttribdvARB)
    GETGLPROCMACRO(glGetVertexAttribfvARB)
    GETGLPROCMACRO(glGetVertexAttribivARB)
    GETGLPROCMACRO(glGetVertexAttribPointervARB)
  }
  if(r->glARBlist[26].avaible) {      /// #27 GL_ARB_fragment_program http://www.opengl.org/registry/specs/ARB/fragment_program.txt
    GETGLPROCMACRO(glProgramStringARB)
    GETGLPROCMACRO(glBindProgramARB)
    GETGLPROCMACRO(glDeleteProgramsARB)
    GETGLPROCMACRO(glGenProgramsARB)
    GETGLPROCMACRO(glProgramEnvParameter4dARB)
    GETGLPROCMACRO(glProgramEnvParameter4dvARB)
    GETGLPROCMACRO(glProgramEnvParameter4fARB)
    GETGLPROCMACRO(glProgramEnvParameter4fvARB)
    GETGLPROCMACRO(glProgramLocalParameter4dARB)
    GETGLPROCMACRO(glProgramLocalParameter4dvARB)
    GETGLPROCMACRO(glProgramLocalParameter4fARB)
    GETGLPROCMACRO(glProgramLocalParameter4fvARB)
    GETGLPROCMACRO(glGetProgramEnvParameterdvARB)
    GETGLPROCMACRO(glGetProgramEnvParameterfvARB)
    GETGLPROCMACRO(glGetProgramLocalParameterdvARB)
    GETGLPROCMACRO(glGetProgramLocalParameterfvARB)
    GETGLPROCMACRO(glGetProgramivARB)
    GETGLPROCMACRO(glGetProgramStringARB)
    GETGLPROCMACRO(glIsProgramARB)
  }
  if(r->glARBlist[27].avaible) {      /// #28 GL_ARB_vertex_buffer_object GLX_ARB_vertex_buffer_object http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
    GETGLPROCMACRO(glBindBufferARB)
    GETGLPROCMACRO(glDeleteBuffersARB)
    GETGLPROCMACRO(glGenBuffersARB)
    GETGLPROCMACRO(glIsBufferARB)
    GETGLPROCMACRO(glBufferDataARB)
    GETGLPROCMACRO(glBufferSubDataARB)
    GETGLPROCMACRO(glGetBufferSubDataARB)
    GETGLPROCMACRO(glMapBufferARB)
    GETGLPROCMACRO(glUnmapBufferARB)
    GETGLPROCMACRO(glGetBufferParameterivARB)
    GETGLPROCMACRO(glGetBufferPointervARB)
  }
  if(r->glARBlist[28].avaible) {      /// #29 GL_ARB_occlusion_query http://www.opengl.org/registry/specs/ARB/occlusion_query.txt
    GETGLPROCMACRO(glGenQueriesARB)
    GETGLPROCMACRO(glDeleteQueriesARB)
    GETGLPROCMACRO(glIsQueryARB)
    GETGLPROCMACRO(glBeginQueryARB)
    GETGLPROCMACRO(glEndQueryARB)
    GETGLPROCMACRO(glGetQueryivARB)
    GETGLPROCMACRO(glGetQueryObjectivARB)
    GETGLPROCMACRO(glGetQueryObjectuivARB)
  }
  if(r->glARBlist[29].avaible) {      /// #30 GL_ARB_shader_objects http://www.opengl.org/registry/specs/ARB/shader_objects.txt
    GETGLPROCMACRO(glDeleteObjectARB)
    GETGLPROCMACRO(glGetHandleARB)
    GETGLPROCMACRO(glDetachObjectARB)
    GETGLPROCMACRO(glCreateShaderObjectARB)
    GETGLPROCMACRO(glShaderSourceARB)
    GETGLPROCMACRO(glCompileShaderARB)
    GETGLPROCMACRO(glCreateProgramObjectARB)
    GETGLPROCMACRO(glAttachObjectARB)
    GETGLPROCMACRO(glLinkProgramARB)
    GETGLPROCMACRO(glUseProgramObjectARB)
    GETGLPROCMACRO(glValidateProgramARB)
    GETGLPROCMACRO(glUniform1fARB)
    GETGLPROCMACRO(glUniform2fARB)
    GETGLPROCMACRO(glUniform3fARB)
    GETGLPROCMACRO(glUniform4fARB)
    GETGLPROCMACRO(glUniform1iARB)
    GETGLPROCMACRO(glUniform2iARB)
    GETGLPROCMACRO(glUniform3iARB)
    GETGLPROCMACRO(glUniform4iARB)
    GETGLPROCMACRO(glUniform1fvARB)
    GETGLPROCMACRO(glUniform2fvARB)
    GETGLPROCMACRO(glUniform3fvARB)
    GETGLPROCMACRO(glUniform4fvARB)
    GETGLPROCMACRO(glUniform1ivARB)
    GETGLPROCMACRO(glUniform2ivARB)
    GETGLPROCMACRO(glUniform3ivARB)
    GETGLPROCMACRO(glUniform4ivARB)
    GETGLPROCMACRO(glUniformMatrix2fvARB)
    GETGLPROCMACRO(glUniformMatrix3fvARB)
    GETGLPROCMACRO(glUniformMatrix4fvARB)
    GETGLPROCMACRO(glGetObjectParameterfvARB)
    GETGLPROCMACRO(glGetObjectParameterivARB)
    GETGLPROCMACRO(glGetInfoLogARB)
    GETGLPROCMACRO(glGetAttachedObjectsARB)
    GETGLPROCMACRO(glGetUniformLocationARB)
    GETGLPROCMACRO(glGetActiveUniformARB)
    GETGLPROCMACRO(glGetUniformfvARB)
    GETGLPROCMACRO(glGetUniformivARB)
    GETGLPROCMACRO(glGetShaderSourceARB)
  }
  if(r->glARBlist[30].avaible){       /// #31 GL_ARB_vertex_shader http://www.opengl.org/registry/specs/ARB/vertex_shader.txt
    GETGLPROCMACRO(glBindAttribLocationARB) // doc includes GL_ARB_vertex_program funcs...
    GETGLPROCMACRO(glGetActiveAttribARB)
    GETGLPROCMACRO(glGetAttribLocationARB)
  }
  if(r->glARBlist[36].avaible)        /// #37 GL_ARB_draw_buffers http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
    GETGLPROCMACRO(glDrawBuffersARB)

  if(r->glARBlist[38].avaible) {      /// #39 GL_ARB_color_buffer_float WGL_ARB_pixel_format_float GLX_ARB_fbconfig_float http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
    GETGLPROCMACRO(glClampColorARB)
    #ifdef OS_WIN
    GETGLPROCMACRO(wglGetPixelFormatAttribivARB)
    GETGLPROCMACRO(wglGetPixelFormatAttribfvARB)
    GETGLPROCMACRO(wglChoosePixelFormatARB)
    #endif /// OS_WIN
  }
  if(r->glARBlist[43].avaible) {      /// #44 GL_ARB_draw_instanced http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
    GETGLPROCMACRO(glDrawArraysInstancedARB)
    GETGLPROCMACRO(glDrawElementsInstancedARB)
  }
  if(r->glARBlist[46].avaible) {      /// #47 GL_ARB_geometry_shader4 http://www.opengl.org/registry/specs/ARB/geometry_shader4.txt
    GETGLPROCMACRO(glProgramParameteriARB)
    GETGLPROCMACRO(glFramebufferTextureARB)
    GETGLPROCMACRO(glFramebufferTextureLayerARB)
    GETGLPROCMACRO(glFramebufferTextureFaceARB)
  }
  if(r->glARBlist[48].avaible)        /// #49 GL_ARB_instanced_arrays http://www.opengl.org/registry/specs/ARB/instanced_arrays.txt
    GETGLPROCMACRO(glVertexAttribDivisorARB)
  if(r->glARBlist[50].avaible)        /// #51 GL_ARB_texture_buffer_object http://www.opengl.org/registry/specs/ARB/texture_buffer_object.txt
    GETGLPROCMACRO(glTexBufferARB)

  #ifdef OS_WIN
  if(r->glARBlist[54].avaible)        /// #55 #74 !!! WGL_ARB_create_context !!! WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    GETGLPROCMACRO(wglCreateContextAttribsARB)
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(r->glARBlist[55].avaible)       /// #56 #75 GLX_ARB_create_context !!! GLX_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
    GETGLPROCMACRO(glXCreateContextAttribsARB)
  #endif /// OS_LINUX

  if(r->glARBlist[68].avaible) {      /// #69 GL_ARB_draw_buffers_blend http://www.opengl.org/registry/specs/ARB/draw_buffers_blend.txt
    GETGLPROCMACRO(glBlendEquationiARB)
    GETGLPROCMACRO(glBlendEquationSeparateiARB)
    GETGLPROCMACRO(glBlendFunciARB)
    GETGLPROCMACRO(glBlendFuncSeparateiARB)
  }
  if(r->glARBlist[69].avaible)        /// #70 GL_ARB_sample_shading http://www.opengl.org/registry/specs/ARB/sample_shading.txt
    GETGLPROCMACRO(glMinSampleShadingARB)
  if(r->glARBlist[75].avaible) {      /// #76 GL_ARB_shading_language_include http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
    GETGLPROCMACRO(glNamedStringARB)
    GETGLPROCMACRO(glDeleteNamedStringARB)
    GETGLPROCMACRO(glCompileShaderIncludeARB)
    GETGLPROCMACRO(glIsNamedStringARB)
    GETGLPROCMACRO(glGetNamedStringARB)
    GETGLPROCMACRO(glGetNamedStringivARB)
  }
  if(r->glARBlist[102].avaible)        /// #103 GL_ARB_cl_event http://www.opengl.org/registry/specs/ARB/cl_event.txt
    GETGLPROCMACRO(glCreateSyncFromCLeventARB)
  if(r->glARBlist[103].avaible) {      /// #104 GL_ARB_debug_output http://www.opengl.org/registry/specs/ARB/debug_output.txt
    GETGLPROCMACRO(glDebugMessageControlARB)
    GETGLPROCMACRO(glDebugMessageInsertARB)
    GETGLPROCMACRO(glDebugMessageCallbackARB)
    GETGLPROCMACRO(glGetDebugMessageLogARB)
  }
  if(r->glARBlist[104].avaible) {      /// #105 GL_ARB_robustness http://www.opengl.org/registry/specs/ARB/robustness.txt
    GETGLPROCMACRO(glGetGraphicsResetStatusARB)
    GETGLPROCMACRO(glGetnMapdvARB)
    GETGLPROCMACRO(glGetnMapfvARB)
    GETGLPROCMACRO(glGetnMapivARB)
    GETGLPROCMACRO(glGetnPixelMapfvARB)
    GETGLPROCMACRO(glGetnPixelMapuivARB)
    GETGLPROCMACRO(glGetnPixelMapusvARB)
    GETGLPROCMACRO(glGetnPolygonStippleARB)
    GETGLPROCMACRO(glGetnTexImageARB)
    GETGLPROCMACRO(glReadnPixelsARB)
    GETGLPROCMACRO(glGetnColorTableARB)
    GETGLPROCMACRO(glGetnConvolutionFilterARB)
    GETGLPROCMACRO(glGetnSeparableFilterARB)
    GETGLPROCMACRO(glGetnHistogramARB)
    GETGLPROCMACRO(glGetnMinmaxARB)
    GETGLPROCMACRO(glGetnCompressedTexImageARB)
    GETGLPROCMACRO(glGetnUniformfvARB)
    GETGLPROCMACRO(glGetnUniformivARB)
    GETGLPROCMACRO(glGetnUniformuivARB)
    GETGLPROCMACRO(glGetnUniformdvARB)
  }
  if(r->glARBlist[152].avaible)        /// #153 GL_ARB_compute_variable_group_size http://www.opengl.org/registry/specs/ARB/compute_variable_group_size.txt
    GETGLPROCMACRO(glDispatchComputeGroupSizeARB)    /// shader stuff
  if(r->glARBlist[153].avaible) {      /// #154 GL_ARB_indirect_parameters http://www.opengl.org/registry/specs/ARB/indirect_parameters.txt
    GETGLPROCMACRO(glMultiDrawArraysIndirectCountARB)
    GETGLPROCMACRO(glMultiDrawElementsIndirectCountARB)
  }
  if(r->glARBlist[157].avaible)        /// #158 GL_ARB_sparse_texture http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
    GETGLPROCMACRO(glTexPageCommitmentARB)       /// texture memory allocation management

}



///=============================///
// EXT and vendor funcs aquiring //
///=============================///
void getEXTfuncs(osiRenderer *r) {
  if(r->glEXTlist[1].avaible) {    /// #2 GL_EXT_blend_color http://www.opengl.org/registry/specs/EXT/blend_color.txt
    GETGLPROCMACRO(glBlendColorEXT)
  }
  if(r->glEXTlist[2].avaible) {    /// #3 GL_EXT_polygon_offset http://www.opengl.org/registry/specs/EXT/polygon_offset.txt
    GETGLPROCMACRO(glPolygonOffsetEXT)
  }
  if(r->glEXTlist[5].avaible) {    /// #6 GL_EXT_texture3D http://www.opengl.org/registry/specs/EXT/texture3D.txt
    GETGLPROCMACRO(glTexImage3DEXT)
    GETGLPROCMACRO(glTexSubImage3DEXT)
  }
  if(r->glEXTlist[6].avaible) {    /// #7 GL_SGIS_texture_filter4 http://www.opengl.org/registry/specs/SGIS/texture_filter4.txt
    GETGLPROCMACRO(glGetTexFilterFuncSGIS)
    GETGLPROCMACRO(glTexFilterFuncSGIS)
  }
  if(r->glEXTlist[8].avaible) {    /// #9 GL_EXT_subtexture http://www.opengl.org/registry/specs/EXT/subtexture.txt
    GETGLPROCMACRO(glTexSubImage1DEXT)
    GETGLPROCMACRO(glTexSubImage2DEXT)
  }
  if(r->glEXTlist[9].avaible) {    /// #10 GL_EXT_copy_texture http://www.opengl.org/registry/specs/EXT/copy_texture.txt
    GETGLPROCMACRO(glCopyTexImage1DEXT)
    GETGLPROCMACRO(glCopyTexImage2DEXT)
    GETGLPROCMACRO(glCopyTexSubImage1DEXT)
    GETGLPROCMACRO(glCopyTexSubImage2DEXT)
    GETGLPROCMACRO(glCopyTexSubImage3DEXT)
  }
  if(r->glEXTlist[10].avaible) {    /// #11 GL_EXT_histogram http://www.opengl.org/registry/specs/EXT/histogram.txt
    GETGLPROCMACRO(glGetHistogramEXT)
    GETGLPROCMACRO(glGetHistogramParameterfvEXT)
    GETGLPROCMACRO(glGetHistogramParameterivEXT)
    GETGLPROCMACRO(glGetMinmaxEXT)
    GETGLPROCMACRO(glGetMinmaxParameterfvEXT)
    GETGLPROCMACRO(glGetMinmaxParameterivEXT)
    GETGLPROCMACRO(glHistogramEXT)
    GETGLPROCMACRO(glMinmaxEXT)
    GETGLPROCMACRO(glResetHistogramEXT)
    GETGLPROCMACRO(glResetMinmaxEXT)
  }
  if(r->glEXTlist[11].avaible) {    /// #12 GL_EXT_convolution http://www.opengl.org/registry/specs/EXT/convolution.txt
    GETGLPROCMACRO(glConvolutionFilter1DEXT)
    GETGLPROCMACRO(glConvolutionFilter2DEXT)
    GETGLPROCMACRO(glConvolutionParameterfEXT)
    GETGLPROCMACRO(glConvolutionParameterfvEXT)
    GETGLPROCMACRO(glConvolutionParameteriEXT)
    GETGLPROCMACRO(glConvolutionParameterivEXT)
    GETGLPROCMACRO(glCopyConvolutionFilter1DEXT)
    GETGLPROCMACRO(glCopyConvolutionFilter2DEXT)
    GETGLPROCMACRO(glGetConvolutionFilterEXT)
    GETGLPROCMACRO(glGetConvolutionParameterfvEXT)
    GETGLPROCMACRO(glGetConvolutionParameterivEXT)
    GETGLPROCMACRO(glGetSeparableFilterEXT)
    GETGLPROCMACRO(glSeparableFilter2DEXT)
  }
  if(r->glEXTlist[13].avaible) {    /// #14 GL_SGI_color_table http://www.opengl.org/registry/specs/SGI/color_table.txt
    GETGLPROCMACRO(glColorTableSGI)
    GETGLPROCMACRO(glColorTableParameterfvSGI)
    GETGLPROCMACRO(glColorTableParameterivSGI)
    GETGLPROCMACRO(glCopyColorTableSGI)
    GETGLPROCMACRO(glGetColorTableSGI)
    GETGLPROCMACRO(glGetColorTableParameterfvSGI)
    GETGLPROCMACRO(glGetColorTableParameterivSGI)
  }
  if(r->glEXTlist[14].avaible) {    /// #15 & #15a GL_SGIS_pixel_texture GL_SGIX_pixel_texture http://www.opengl.org/registry/specs/SGIS/pixel_texture.txt http://www.opengl.org/registry/specs/SGIX/sgix_pixel_texture.txt
    GETGLPROCMACRO(glPixelTexGenParameteriSGIS)
    GETGLPROCMACRO(glPixelTexGenParameterivSGIS)
    GETGLPROCMACRO(glPixelTexGenParameterfSGIS)
    GETGLPROCMACRO(glPixelTexGenParameterfvSGIS)
    GETGLPROCMACRO(glGetPixelTexGenParameterivSGIS)
    GETGLPROCMACRO(glGetPixelTexGenParameterfvSGIS)
    GETGLPROCMACRO(glPixelTexGenSGIX) /// GL_SGIX_pixel_texture
  }
  if(r->glEXTlist[15].avaible) {    /// #16 GL_SGIS_texture4D http://www.opengl.org/registry/specs/SGIS/texture4D.txt
    GETGLPROCMACRO(glTexImage4DSGIS)
    GETGLPROCMACRO(glTexSubImage4DSGIS)
  }
  if(r->glEXTlist[19].avaible) {    /// #20 GL_EXT_texture_object http://www.opengl.org/registry/specs/EXT/texture_object.txt
    GETGLPROCMACRO(glAreTexturesResidentEXT)
    GETGLPROCMACRO(glBindTextureEXT)
    GETGLPROCMACRO(glDeleteTexturesEXT)
    GETGLPROCMACRO(glGenTexturesEXT)
    GETGLPROCMACRO(glIsTextureEXT)
    GETGLPROCMACRO(glPrioritizeTexturesEXT)
  }
  if(r->glEXTlist[20].avaible) {    /// #21 GL_SGIS_detail_texture http://www.opengl.org/registry/specs/SGIS/detail_texture.txt
    GETGLPROCMACRO(glDetailTexFuncSGIS)
    GETGLPROCMACRO(glGetDetailTexFuncSGIS)
  }
  if(r->glEXTlist[21].avaible) {    /// #22 GL_SGIS_sharpen_texture http://www.opengl.org/registry/specs/SGIS/sharpen_texture.txt
    GETGLPROCMACRO(glSharpenTexFuncSGIS)
    GETGLPROCMACRO(glGetSharpenTexFuncSGIS)
  }
  if(r->glEXTlist[24].avaible) {    /// #25 GL_SGIS_multisample GLX_SGIS_multisample http://www.opengl.org/registry/specs/SGIS/multisample.txt
    GETGLPROCMACRO(glSampleMaskSGIS)
    GETGLPROCMACRO(glSamplePatternSGIS)
  }
  if(r->glEXTlist[29].avaible) {    /// #30 GL_EXT_vertex_array http://www.opengl.org/registry/specs/EXT/vertex_array.txt
    GETGLPROCMACRO(glArrayElementEXT)
    GETGLPROCMACRO(glColorPointerEXT)
    GETGLPROCMACRO(glDrawArraysEXT)
    GETGLPROCMACRO(glEdgeFlagPointerEXT)
    GETGLPROCMACRO(glGetPointervEXT)
    GETGLPROCMACRO(glIndexPointerEXT)
    GETGLPROCMACRO(glNormalPointerEXT)
    GETGLPROCMACRO(glTexCoordPointerEXT)
    GETGLPROCMACRO(glVertexPointerEXT)
  }
  if(r->glEXTlist[36].avaible) {    /// #37 GL_EXT_blend_minmax http://www.opengl.org/registry/specs/EXT/blend_minmax.txt
    GETGLPROCMACRO(glBlendEquationEXT)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[39].avaible) {    /// #40 GLX_SGI_swap_control http://www.opengl.org/registry/specs/SGI/swap_control.txt
    GETGLPROCMACRO(glXSwapIntervalSGI)
  }
  if(r->glEXTlist[40].avaible) {    /// #41 GLX_SGI_video_sync http://www.opengl.org/registry/specs/SGI/video_sync.txt
    GETGLPROCMACRO(glXGetVideoSyncSGI)
    GETGLPROCMACRO(glXWaitVideoSyncSGI)
  }
  if(r->glEXTlist[41].avaible) {    /// #42 GLX_SGI_make_current_read http://www.opengl.org/registry/specs/SGI/make_current_read.txt
    GETGLPROCMACRO(glXMakeCurrentReadSGI)
    GETGLPROCMACRO(glXGetCurrentReadDrawableSGI)
  }
  
  if(r->glEXTlist[42].avaible) {    /// #43 GLX_SGIX_video_source http://www.opengl.org/registry/specs/SGIX/video_source.txt
    #ifdef _VL_H
    GETGLPROCMACRO(glXCreateGLXVideoSourceSGIX)
    GETGLPROCMACRO(glXDestroyGLXVideoSourceSGIX)
    #endif
  }
  if(r->glEXTlist[46].avaible) {    /// #47 GLX_EXT_import_context http://www.opengl.org/registry/specs/EXT/import_context.txt
    GETGLPROCMACRO(glXGetCurrentDisplayEXT)
    GETGLPROCMACRO(glXQueryContextInfoEXT)
    GETGLPROCMACRO(glXGetContextIDEXT)
    GETGLPROCMACRO(glXImportContextEXT)
    GETGLPROCMACRO(glXFreeContextEXT)
  }
  if(r->glEXTlist[48].avaible) {    /// #49 GLX_SGIX_fbconfig http://www.opengl.org/registry/specs/SGIX/fbconfig.txt
    GETGLPROCMACRO(glXGetFBConfigAttribSGIX)
    GETGLPROCMACRO(glXChooseFBConfigSGIX)
    GETGLPROCMACRO(glXCreateGLXPixmapWithConfigSGIX)
    GETGLPROCMACRO(glXCreateContextWithConfigSGIX)
    GETGLPROCMACRO(glXGetVisualFromFBConfigSGIX)
    GETGLPROCMACRO(glXGetFBConfigFromVisualSGIX)
  }
  if(r->glEXTlist[49].avaible) {    /// #50 GLX_SGIX_pbuffer http://www.opengl.org/registry/specs/SGIX/pbuffer.txt
    GETGLPROCMACRO(glXCreateGLXPbufferSGIX)
    GETGLPROCMACRO(glXDestroyGLXPbufferSGIX)
    GETGLPROCMACRO(glXQueryGLXPbufferSGIX)
    GETGLPROCMACRO(glXSelectEventSGIX)
    GETGLPROCMACRO(glXGetSelectedEventSGIX)
  }
  #endif
  if(r->glEXTlist[51].avaible) {    /// #52 GL_SGIX_sprite http://www.opengl.org/registry/specs/SGIX/sprite.txt
    GETGLPROCMACRO(glSpriteParameterfSGIX)
    GETGLPROCMACRO(glSpriteParameterfvSGIX)
    GETGLPROCMACRO(glSpriteParameteriSGIX)
    GETGLPROCMACRO(glSpriteParameterivSGIX)
  }
  if(r->glEXTlist[53].avaible) {    /// #54 GL_EXT_point_parameters http://www.opengl.org/registry/specs/EXT/point_parameters.txt
    GETGLPROCMACRO(glPointParameterfEXT)
    GETGLPROCMACRO(glPointParameterfvEXT)
  }
  if(r->glEXTlist[54].avaible) {    /// #55 GL_SGIX_instruments http://www.opengl.org/registry/specs/SGIX/instruments.txt
    GETGLPROCMACRO(glGetInstrumentsSGIX)
    GETGLPROCMACRO(glInstrumentsBufferSGIX)
    GETGLPROCMACRO(glPollInstrumentsSGIX)
    GETGLPROCMACRO(glReadInstrumentsSGIX)
    GETGLPROCMACRO(glStartInstrumentsSGIX)
    GETGLPROCMACRO(glStopInstrumentsSGIX)
  }
  if(r->glEXTlist[56].avaible) {    /// #57 GL_SGIX_framezoom http://www.opengl.org/registry/specs/SGIX/framezoom.txt
    GETGLPROCMACRO(glFrameZoomSGIX)
  }
  if(r->glEXTlist[57].avaible) {    /// #58 GL_SGIX_tag_sample_buffer http://www.opengl.org/registry/specs/SGIX/tag_sample_buffer.txt
    GETGLPROCMACRO(glTagSampleBufferSGIX)
  }
  if(r->glEXTlist[59].avaible) {    /// #60 GL_SGIX_reference_plane http://www.opengl.org/registry/specs/SGIX/reference_plane.txt
    GETGLPROCMACRO(glReferencePlaneSGIX)
  }
  if(r->glEXTlist[60].avaible) {    /// #61 GL_SGIX_flush_raster http://www.opengl.org/registry/specs/SGIX/flush_raster.txt
    GETGLPROCMACRO(glFlushRasterSGIX)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[61].avaible) {    /// #62 GLX_SGI_cushion http://www.opengl.org/registry/specs/SGI/cushion.txt
    GETGLPROCMACRO(glXCushionSGI)
  }
  #endif
  if(r->glEXTlist[63].avaible) {    /// #64 GL_SGIS_fog_function http://www.opengl.org/registry/specs/SGIS/fog_func.txt
    GETGLPROCMACRO(glFogFuncSGIS)
    GETGLPROCMACRO(glGetFogFuncSGIS)
  }
  if(r->glEXTlist[65].avaible) {    /// #66 GL_HP_image_transform http://www.opengl.org/registry/specs/HP/image_transform.txt
    GETGLPROCMACRO(glImageTransformParameteriHP)
    GETGLPROCMACRO(glImageTransformParameterfHP)
    GETGLPROCMACRO(glImageTransformParameterivHP)
    GETGLPROCMACRO(glImageTransformParameterfvHP)
    GETGLPROCMACRO(glGetImageTransformParameterivHP)
    GETGLPROCMACRO(glGetImageTransformParameterfvHP)
  }
  if(r->glEXTlist[73].avaible) {    /// #74 GL_EXT_color_subtable http://www.opengl.org/registry/specs/EXT/color_subtable.txt
    GETGLPROCMACRO(glColorSubTableEXT)
    GETGLPROCMACRO(glCopyColorSubTableEXT)
  }
  if(r->glEXTlist[76].avaible) {    /// #77 GL_PGI_misc_hints http://www.opengl.org/registry/specs/PGI/misc_hints.txt
    GETGLPROCMACRO(glHintPGI)
  }
  if(r->glEXTlist[77].avaible) {    /// #78 GL_EXT_paletted_texture http://www.opengl.org/registry/specs/EXT/paletted_texture.txt
    GETGLPROCMACRO(glColorTableEXT)
    GETGLPROCMACRO(glGetColorTableEXT)
    GETGLPROCMACRO(glGetColorTableParameterivEXT)
    GETGLPROCMACRO(glGetColorTableParameterfvEXT)
  }
  if(r->glEXTlist[79].avaible) {    /// #80 incomplete! GL_SGIX_list_priority  http://www.opengl.org/registry/specs/SGIX/list_priority.txt
    GETGLPROCMACRO(glGetListParameterfvSGIX)
    GETGLPROCMACRO(glGetListParameterivSGIX)
    GETGLPROCMACRO(glListParameterfSGIX)
    GETGLPROCMACRO(glListParameterfvSGIX)
    GETGLPROCMACRO(glListParameteriSGIX)
    GETGLPROCMACRO(glListParameterivSGIX)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[82].avaible) {    /// #83 GLX_SGIX_video_resize http://www.opengl.org/registry/specs/SGIX/video_resize.txt
    GETGLPROCMACRO(glXBindChannelToWindowSGIX)
    GETGLPROCMACRO(glXChannelRectSGIX)
    GETGLPROCMACRO(glXQueryChannelRectSGIX)
    GETGLPROCMACRO(glXQueryChannelDeltasSGIX)
    GETGLPROCMACRO(glXChannelRectSyncSGIX)
  }
  if(r->glEXTlist[85].avaible) {    /// #86 GLX_SGIX_dm_buffer incomplete !!!GLX_SGIX_dmbuffer!!! http://www.opengl.org/registry/specs/SGIX/dmbuffer.txt
    #ifdef _DM_BUFFER_H_
    GETGLPROCMACRO(glXAssociateDMPbufferSGIX)
    #endif
  }
  if(r->glEXTlist[90].avaible) {    /// #91 GLX_SGIX_swap_group http://www.opengl.org/registry/specs/SGIX/swap_group.txt
    GETGLPROCMACRO(glXJoinSwapGroupSGIX)
  }
  if(r->glEXTlist[91].avaible) {    /// #92 GLX_SGIX_swap_barrier http://www.opengl.org/registry/specs/SGIX/swap_barrier.txt
    GETGLPROCMACRO(glXBindSwapBarrierSGIX)
    GETGLPROCMACRO(glXQueryMaxSwapBarriersSGIX)
  }
  #endif
  if(r->glEXTlist[93].avaible) {    /// #94 GL_EXT_index_material http://www.opengl.org/registry/specs/EXT/index_material.txt
    GETGLPROCMACRO(glIndexMaterialEXT)
  }
  if(r->glEXTlist[94].avaible) {    /// #95 GL_EXT_index_func http://www.opengl.org/registry/specs/EXT/index_func.txt
    GETGLPROCMACRO(glIndexFuncEXT)
  }
  if(r->glEXTlist[96].avaible) {    /// #97 incomplete! GL_EXT_compiled_vertex_array  http://www.opengl.org/registry/specs/EXT/compiled_vertex_array.txt
    GETGLPROCMACRO(glLockArraysEXT)
    GETGLPROCMACRO(glUnlockArraysEXT)
  }
  if(r->glEXTlist[97].avaible) {    /// #98 incomplete! GL_EXT_cull_vertex http://www.opengl.org/registry/specs/EXT/cull_vertex.txt
    GETGLPROCMACRO(glCullParameterdvEXT)
    GETGLPROCMACRO(glCullParameterfvEXT)
  }
  if(r->glEXTlist[99].avaible) {    // ?? /// #100 GLU_EXT_nurbs_tessellator http://www.opengl.org/registry/specs/EXT/nurbs_tessellator.txt
    /// #102 inc! GL_EXT_fragment_lighting http://www.opengl.org/registry/specs/EXT/fragment_lighting.txt
    // has funcs but none in glext.h ATM
    /// #112 GL_EXT_draw_range_elements http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
    GETGLPROCMACRO(glDrawRangeElementsEXT)
  }
  if(r->glEXTlist[116].avaible) {    /// #117 GL_EXT_light_texture http://www.opengl.org/registry/specs/EXT/light_texture.txt
    GETGLPROCMACRO(glApplyTextureEXT)
    GETGLPROCMACRO(glTextureLightEXT)
    GETGLPROCMACRO(glTextureMaterialEXT)
  }
  if(r->glEXTlist[119].avaible) {    /// #120 GL_EXT_scene_marker GLX_EXT_scene_marker http://www.opengl.org/registry/specs/EXT/scene_marker.txt
    // some funcs but not in glext.h ATM
  }
  if(r->glEXTlist[131].avaible) {    /// #132 incomplete! GL_SGIX_async http://www.opengl.org/registry/specs/SGIX/async.txt
    GETGLPROCMACRO(glAsyncMarkerSGIX)
    GETGLPROCMACRO(glFinishAsyncSGIX)
    GETGLPROCMACRO(glPollAsyncSGIX)
    GETGLPROCMACRO(glGenAsyncMarkersSGIX)
    GETGLPROCMACRO(glDeleteAsyncMarkersSGIX)
    GETGLPROCMACRO(glIsAsyncMarkerSGIX)
  }
  if(r->glEXTlist[135].avaible) {    /// #136 GL_INTEL_parallel_arrays http://www.opengl.org/registry/specs/INTEL/parallel_arrays.txt
    GETGLPROCMACRO(glVertexPointervINTEL)
    GETGLPROCMACRO(glNormalPointervINTEL)
    GETGLPROCMACRO(glColorPointervINTEL)
    GETGLPROCMACRO(glTexCoordPointervINTEL)
  }
  if(r->glEXTlist[137].avaible) {    /// #138 GL_EXT_pixel_transform http://www.opengl.org/registry/specs/EXT/pixel_transform.txt
    GETGLPROCMACRO(glPixelTransformParameteriEXT)
    GETGLPROCMACRO(glPixelTransformParameterfEXT)
    GETGLPROCMACRO(glPixelTransformParameterivEXT)
    GETGLPROCMACRO(glPixelTransformParameterfvEXT)
    GETGLPROCMACRO(glGetPixelTransformParameterivEXT)
    GETGLPROCMACRO(glGetPixelTransformParameterfvEXT)
  }
  if(r->glEXTlist[144].avaible) {    /// #145 GL_EXT_secondary_color http://www.opengl.org/registry/specs/EXT/secondary_color.txt
    GETGLPROCMACRO(glSecondaryColor3bEXT)
    GETGLPROCMACRO(glSecondaryColor3bvEXT)
    GETGLPROCMACRO(glSecondaryColor3dEXT)
    GETGLPROCMACRO(glSecondaryColor3dvEXT)
    GETGLPROCMACRO(glSecondaryColor3fEXT)
    GETGLPROCMACRO(glSecondaryColor3fvEXT)
    GETGLPROCMACRO(glSecondaryColor3iEXT)
    GETGLPROCMACRO(glSecondaryColor3ivEXT)
    GETGLPROCMACRO(glSecondaryColor3sEXT)
    GETGLPROCMACRO(glSecondaryColor3svEXT)
    GETGLPROCMACRO(glSecondaryColor3ubEXT)
    GETGLPROCMACRO(glSecondaryColor3ubvEXT)
    GETGLPROCMACRO(glSecondaryColor3uiEXT)
    GETGLPROCMACRO(glSecondaryColor3uivEXT)
    GETGLPROCMACRO(glSecondaryColor3usEXT)
    GETGLPROCMACRO(glSecondaryColor3usvEXT)
    GETGLPROCMACRO(glSecondaryColorPointerEXT)
  }
  if(r->glEXTlist[146].avaible) {    /// #147 GL_EXT_texture_perturb_normal http://www.opengl.org/registry/specs/EXT/texture_perturb_normal.txt
    GETGLPROCMACRO(glTextureNormalEXT)
  }
  if(r->glEXTlist[147].avaible) {    /// #148 GL_EXT_multi_draw_arrays GL_SUN_multi_draw_arrays http://www.opengl.org/registry/specs/EXT/multi_draw_arrays.txt
    GETGLPROCMACRO(glMultiDrawArraysEXT)
    GETGLPROCMACRO(glMultiDrawElementsEXT)
  }
  if(r->glEXTlist[148].avaible) {    /// #149 GL_EXT_fog_coord http://www.opengl.org/registry/specs/EXT/fog_coord.txt
    GETGLPROCMACRO(glFogCoordfEXT)
    GETGLPROCMACRO(glFogCoordfvEXT)
    GETGLPROCMACRO(glFogCoorddEXT)
    GETGLPROCMACRO(glFogCoorddvEXT)
    GETGLPROCMACRO(glFogCoordPointerEXT)
  }
  if(r->glEXTlist[155].avaible) {    /// #156 inc! GL_EXT_coordinate_frame http://www.opengl.org/registry/specs/EXT/coordinate_frame.txt
    GETGLPROCMACRO(glTangent3bEXT)
    GETGLPROCMACRO(glTangent3bvEXT)
    GETGLPROCMACRO(glTangent3dEXT)
    GETGLPROCMACRO(glTangent3dvEXT)
    GETGLPROCMACRO(glTangent3fEXT)
    GETGLPROCMACRO(glTangent3fvEXT)
    GETGLPROCMACRO(glTangent3iEXT)
    GETGLPROCMACRO(glTangent3ivEXT)
    GETGLPROCMACRO(glTangent3sEXT)
    GETGLPROCMACRO(glTangent3svEXT)
    GETGLPROCMACRO(glBinormal3bEXT)
    GETGLPROCMACRO(glBinormal3bvEXT)
    GETGLPROCMACRO(glBinormal3dEXT)
    GETGLPROCMACRO(glBinormal3dvEXT)
    GETGLPROCMACRO(glBinormal3fEXT)
    GETGLPROCMACRO(glBinormal3fvEXT)
    GETGLPROCMACRO(glBinormal3iEXT)
    GETGLPROCMACRO(glBinormal3ivEXT)
    GETGLPROCMACRO(glBinormal3sEXT)
    GETGLPROCMACRO(glBinormal3svEXT)
    GETGLPROCMACRO(glTangentPointerEXT)
    GETGLPROCMACRO(glBinormalPointerEXT)
  }
  if(r->glEXTlist[162].avaible) {    /// #163 GL_SUNX_constant_data http://www.opengl.org/registry/specs/SUNX/constant_data.txt
    GETGLPROCMACRO(glFinishTextureSUNX)
  }
  if(r->glEXTlist[163].avaible) {    /// #164 GL_SUN_global_alpha http://www.opengl.org/registry/specs/SUN/global_alpha.txt
    GETGLPROCMACRO(glGlobalAlphaFactorbSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorsSUN)
    GETGLPROCMACRO(glGlobalAlphaFactoriSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorfSUN)
    GETGLPROCMACRO(glGlobalAlphaFactordSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorubSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorusSUN)
    GETGLPROCMACRO(glGlobalAlphaFactoruiSUN)
  }
  if(r->glEXTlist[164].avaible) {    /// #165 GL_SUN_triangle_list http://www.opengl.org/registry/specs/SUN/triangle_list.txt
    GETGLPROCMACRO(glReplacementCodeuiSUN)
    GETGLPROCMACRO(glReplacementCodeusSUN)
    GETGLPROCMACRO(glReplacementCodeubSUN)
    GETGLPROCMACRO(glReplacementCodeuivSUN)
    GETGLPROCMACRO(glReplacementCodeusvSUN)
    GETGLPROCMACRO(glReplacementCodeubvSUN)
    GETGLPROCMACRO(glReplacementCodePointerSUN)
  }
  if(r->glEXTlist[165].avaible) {    /// #166 GL_SUN_vertex http://www.opengl.org/registry/specs/SUN/vertex.txt
    GETGLPROCMACRO(glColor4ubVertex2fSUN)
    GETGLPROCMACRO(glColor4ubVertex2fvSUN)
    GETGLPROCMACRO(glColor4ubVertex3fSUN)
    GETGLPROCMACRO(glColor4ubVertex3fvSUN)
    GETGLPROCMACRO(glColor3fVertex3fSUN)
    GETGLPROCMACRO(glColor3fVertex3fvSUN)
    GETGLPROCMACRO(glNormal3fVertex3fSUN)
    GETGLPROCMACRO(glNormal3fVertex3fvSUN)
    GETGLPROCMACRO(glColor4fNormal3fVertex3fSUN)
    GETGLPROCMACRO(glColor4fNormal3fVertex3fvSUN)
    GETGLPROCMACRO(glTexCoord2fVertex3fSUN)
    GETGLPROCMACRO(glTexCoord2fVertex3fvSUN)
    GETGLPROCMACRO(glTexCoord4fVertex4fSUN)
    GETGLPROCMACRO(glTexCoord4fVertex4fvSUN)
    GETGLPROCMACRO(glTexCoord2fColor4ubVertex3fSUN)
    GETGLPROCMACRO(glTexCoord2fColor4ubVertex3fvSUN)
    GETGLPROCMACRO(glTexCoord2fColor3fVertex3fSUN)
    GETGLPROCMACRO(glTexCoord2fColor3fVertex3fvSUN)
    GETGLPROCMACRO(glTexCoord2fNormal3fVertex3fSUN)
    GETGLPROCMACRO(glTexCoord2fNormal3fVertex3fvSUN)
    GETGLPROCMACRO(glTexCoord2fColor4fNormal3fVertex3fSUN)
    GETGLPROCMACRO(glTexCoord2fColor4fNormal3fVertex3fvSUN)
    GETGLPROCMACRO(glTexCoord4fColor4fNormal3fVertex4fSUN)
    GETGLPROCMACRO(glTexCoord4fColor4fNormal3fVertex4fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiVertex3fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiColor4ubVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiColor4ubVertex3fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiColor3fVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiColor3fVertex3fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiNormal3fVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiNormal3fVertex3fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiColor4fNormal3fVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiColor4fNormal3fVertex3fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiTexCoord2fVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiTexCoord2fVertex3fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN)
    GETGLPROCMACRO(glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN)
    GETGLPROCMACRO(glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN)
  }
  #ifdef OS_WIN
  if(r->glEXTlist[166].avaible) {    /// #167 inc! WGL_EXT_display_color_table http://www.opengl.org/registry/specs/EXT/wgl_display_color_table.txt
    GETGLPROCMACRO(wglCreateDisplayColorTableEXT)
    GETGLPROCMACRO(wglLoadDisplayColorTableEXT)
    GETGLPROCMACRO(wglBindDisplayColorTableEXT)
    GETGLPROCMACRO(wglDestroyDisplayColorTableEXT)
  }
  if(r->glEXTlist[167].avaible) {    /// #168 WGL_EXT_extensions_string http://www.opengl.org/registry/specs/EXT/wgl_extensions_string.txt
    GETGLPROCMACRO(wglGetExtensionsStringEXT)
  }
  if(r->glEXTlist[168].avaible) {    /// #169 WGL_EXT_make_current_read http://www.opengl.org/registry/specs/EXT/wgl_make_current_read.txt
    GETGLPROCMACRO(wglMakeContextCurrentEXT)
    GETGLPROCMACRO(wglGetCurrentReadDCEXT)
  }
  if(r->glEXTlist[169].avaible) {    /// #170 WGL_EXT_pixel_format http://www.opengl.org/registry/specs/EXT/wgl_pixel_format.txt
    GETGLPROCMACRO(wglGetPixelFormatAttribivEXT)
    GETGLPROCMACRO(wglGetPixelFormatAttribfvEXT)
    GETGLPROCMACRO(wglChoosePixelFormatEXT)
  }
  if(r->glEXTlist[170].avaible) {    /// #171 WGL_EXT_pbuffer http://www.opengl.org/registry/specs/EXT/wgl_pbuffer.txt
    GETGLPROCMACRO(wglCreatePbufferEXT)
    GETGLPROCMACRO(wglGetPbufferDCEXT)
    GETGLPROCMACRO(wglReleasePbufferDCEXT)
    GETGLPROCMACRO(wglDestroyPbufferEXT)
    GETGLPROCMACRO(wglQueryPbufferEXT)
  }
  if(r->glEXTlist[171].avaible) {    /// #172 WGL_EXT_swap_control http://www.opengl.org/registry/specs/EXT/wgl_swap_control.txt
    GETGLPROCMACRO(wglSwapIntervalEXT)
    GETGLPROCMACRO(wglGetSwapIntervalEXT)
  }
  #endif
  if(r->glEXTlist[172].avaible) {    /// #173 GL_EXT_blend_func_separate http://www.opengl.org/registry/specs/EXT/blend_func_separate.txt
    GETGLPROCMACRO(glBlendFuncSeparateEXT)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[182].avaible) {    /// #183 GLX_SUN_get_transparent_index http://www.opengl.org/registry/specs/SUN/get_transparent_index.txt
    GETGLPROCMACRO(glXGetTransparentIndexSUN)
  }
  #endif
  if(r->glEXTlist[187].avaible) {    /// #188 GL_EXT_vertex_weighting http://www.opengl.org/registry/specs/EXT/vertex_weighting.txt
    GETGLPROCMACRO(glVertexWeightfEXT)
    GETGLPROCMACRO(glVertexWeightfvEXT)
    GETGLPROCMACRO(glVertexWeightPointerEXT)
  }
  if(r->glEXTlist[189].avaible) {    /// #190 GL_NV_vertex_array_range http://www.opengl.org/registry/specs/NV/vertex_array_range.txt
    GETGLPROCMACRO(glFlushVertexArrayRangeNV)
    GETGLPROCMACRO(glVertexArrayRangeNV)
  }
  if(r->glEXTlist[190].avaible) {    /// #191 GL_NV_register_combiners http://www.opengl.org/registry/specs/NV/register_combiners.txt
    GETGLPROCMACRO(glCombinerParameterfvNV)
    GETGLPROCMACRO(glCombinerParameterfNV)
    GETGLPROCMACRO(glCombinerParameterivNV)
    GETGLPROCMACRO(glCombinerParameteriNV)
    GETGLPROCMACRO(glCombinerInputNV)
    GETGLPROCMACRO(glCombinerOutputNV)
    GETGLPROCMACRO(glFinalCombinerInputNV)
    GETGLPROCMACRO(glGetCombinerInputParameterfvNV)
    GETGLPROCMACRO(glGetCombinerInputParameterivNV)
    GETGLPROCMACRO(glGetCombinerOutputParameterfvNV)
    GETGLPROCMACRO(glGetCombinerOutputParameterivNV)
    GETGLPROCMACRO(glGetFinalCombinerInputParameterfvNV)
    GETGLPROCMACRO(glGetFinalCombinerInputParameterivNV)
  }
  if(r->glEXTlist[195].avaible) {    /// #196 GL_MESA_resize_buffers http://www.opengl.org/registry/specs/MESA/resize_buffers.txt
    GETGLPROCMACRO(glResizeBuffersMESA)
  }
  if(r->glEXTlist[196].avaible) {    /// #197 GL_MESA_window_pos http://www.opengl.org/registry/specs/MESA/window_pos.txt
    GETGLPROCMACRO(glWindowPos2dMESA)
    GETGLPROCMACRO(glWindowPos2dvMESA)
    GETGLPROCMACRO(glWindowPos2fMESA)
    GETGLPROCMACRO(glWindowPos2fvMESA)
    GETGLPROCMACRO(glWindowPos2iMESA)
    GETGLPROCMACRO(glWindowPos2ivMESA)
    GETGLPROCMACRO(glWindowPos2sMESA)
    GETGLPROCMACRO(glWindowPos2svMESA)
    GETGLPROCMACRO(glWindowPos3dMESA)
    GETGLPROCMACRO(glWindowPos3dvMESA)
    GETGLPROCMACRO(glWindowPos3fMESA)
    GETGLPROCMACRO(glWindowPos3fvMESA)
    GETGLPROCMACRO(glWindowPos3iMESA)
    GETGLPROCMACRO(glWindowPos3ivMESA)
    GETGLPROCMACRO(glWindowPos3sMESA)
    GETGLPROCMACRO(glWindowPos3svMESA)
    GETGLPROCMACRO(glWindowPos4dMESA)
    GETGLPROCMACRO(glWindowPos4dvMESA)
    GETGLPROCMACRO(glWindowPos4fMESA)
    GETGLPROCMACRO(glWindowPos4fvMESA)
    GETGLPROCMACRO(glWindowPos4iMESA)
    GETGLPROCMACRO(glWindowPos4ivMESA)
    GETGLPROCMACRO(glWindowPos4sMESA)
    GETGLPROCMACRO(glWindowPos4svMESA)
  }
  if(r->glEXTlist[199].avaible) {    /// #200 GL_IBM_multimode_draw_arrays http://www.opengl.org/registry/specs/IBM/multimode_draw_arrays.txt
    GETGLPROCMACRO(glMultiModeDrawArraysIBM)
    GETGLPROCMACRO(glMultiModeDrawElementsIBM)
  }
  if(r->glEXTlist[200].avaible) {    /// #201 GL_IBM_vertex_array_lists http://www.opengl.org/registry/specs/IBM/vertex_array_lists.txt
    GETGLPROCMACRO(glColorPointerListIBM)
    GETGLPROCMACRO(glSecondaryColorPointerListIBM)
    GETGLPROCMACRO(glEdgeFlagPointerListIBM)
    GETGLPROCMACRO(glFogCoordPointerListIBM)
    GETGLPROCMACRO(glIndexPointerListIBM)
    GETGLPROCMACRO(glNormalPointerListIBM)
    GETGLPROCMACRO(glTexCoordPointerListIBM)
    GETGLPROCMACRO(glVertexPointerListIBM)
  }
  if(r->glEXTlist[207].avaible) {    /// #208 GL_3DFX_tbuffer http://www.opengl.org/registry/specs/3DFX/tbuffer.txt
    GETGLPROCMACRO(glTbufferMask3DFX)
  }
  if(r->glEXTlist[208].avaible) {    /// #209 GL_EXT_multisample WGL_EXT_multisample http://www.opengl.org/registry/specs/EXT/wgl_multisample.txt
    GETGLPROCMACRO(glSampleMaskEXT)
    GETGLPROCMACRO(glSamplePatternEXT)
  }
  if(r->glEXTlist[213].avaible) {    /// #214 GL_SGIS_texture_color_mask http://www.opengl.org/registry/specs/SGIS/texture_color_mask.txt
    GETGLPROCMACRO(glTextureColorMaskSGIS)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[214].avaible) {    /// #215 GLX_MESA_copy_sub_buffer http://www.opengl.org/registry/specs/MESA/copy_sub_buffer.txt
    GETGLPROCMACRO(glXCopySubBufferMESA)
  }
  if(r->glEXTlist[215].avaible) {    /// #216 GLX_MESA_pixmap_colormap http://www.opengl.org/registry/specs/MESA/pixmap_colormap.txt
    GETGLPROCMACRO(glXCreateGLXPixmapMESA)
  }
  if(r->glEXTlist[216].avaible) {    /// #217 GLX_MESA_release_buffers http://www.opengl.org/registry/specs/MESA/release_buffers.txt
    GETGLPROCMACRO(glXReleaseBuffersMESA)
  }
  if(r->glEXTlist[217].avaible) {    /// #218 GLX_MESA_set_3dfx_mode http://www.opengl.org/registry/specs/MESA/set_3dfx_mode.txt
    GETGLPROCMACRO(glXSet3DfxModeMESA)
  }
  #endif
  if(r->glEXTlist[221].avaible) {    /// #222 GL_NV_fence http://www.opengl.org/registry/specs/NV/fence.txt
    GETGLPROCMACRO(glDeleteFencesNV)
    GETGLPROCMACRO(glGenFencesNV)
    GETGLPROCMACRO(glIsFenceNV)
    GETGLPROCMACRO(glTestFenceNV)
    GETGLPROCMACRO(glGetFenceivNV)
    GETGLPROCMACRO(glFinishFenceNV)
    GETGLPROCMACRO(glSetFenceNV)
  }
  if(r->glEXTlist[222].avaible) {    /// #223 GL_IBM_static_data http://www.opengl.org/registry/specs/IBM/static_data.txt
    GETGLPROCMACRO(glFlushStaticDataIBM)
  }
  if(r->glEXTlist[224].avaible) {    /// #225 GL_NV_evaluators http://www.opengl.org/registry/specs/NV/evaluators.txt
    GETGLPROCMACRO(glMapControlPointsNV)
    GETGLPROCMACRO(glMapParameterivNV)
    GETGLPROCMACRO(glMapParameterfvNV)
    GETGLPROCMACRO(glGetMapControlPointsNV)
    GETGLPROCMACRO(glGetMapParameterivNV)
    GETGLPROCMACRO(glGetMapParameterfvNV)
    GETGLPROCMACRO(glGetMapAttribParameterivNV)
    GETGLPROCMACRO(glGetMapAttribParameterfvNV)
    GETGLPROCMACRO(glEvalMapsNV)
  }
  if(r->glEXTlist[226].avaible) {    /// #227 GL_NV_register_combiners2 http://www.opengl.org/registry/specs/NV/register_combiners2.txt
    GETGLPROCMACRO(glCombinerStageParameterfvNV)
    GETGLPROCMACRO(glGetCombinerStageParameterfvNV)
  }
  if(r->glEXTlist[232].avaible) {    /// #233 GL_NV_vertex_program http://www.opengl.org/registry/specs/NV/vertex_program.txt
    GETGLPROCMACRO(glAreProgramsResidentNV)
    GETGLPROCMACRO(glBindProgramNV)
    GETGLPROCMACRO(glDeleteProgramsNV)
    GETGLPROCMACRO(glExecuteProgramNV)
    GETGLPROCMACRO(glGenProgramsNV)
    GETGLPROCMACRO(glGetProgramParameterdvNV)
    GETGLPROCMACRO(glGetProgramParameterfvNV)
    GETGLPROCMACRO(glGetProgramivNV)
    GETGLPROCMACRO(glGetProgramStringNV)
    GETGLPROCMACRO(glGetTrackMatrixivNV)
    GETGLPROCMACRO(glGetVertexAttribdvNV)
    GETGLPROCMACRO(glGetVertexAttribfvNV)
    GETGLPROCMACRO(glGetVertexAttribivNV)
    GETGLPROCMACRO(glGetVertexAttribPointervNV)
    GETGLPROCMACRO(glIsProgramNV)
    GETGLPROCMACRO(glLoadProgramNV)
    GETGLPROCMACRO(glProgramParameter4dNV)
    GETGLPROCMACRO(glProgramParameter4dvNV)
    GETGLPROCMACRO(glProgramParameter4fNV)
    GETGLPROCMACRO(glProgramParameter4fvNV)
    GETGLPROCMACRO(glProgramParameters4dvNV)
    GETGLPROCMACRO(glProgramParameters4fvNV)
    GETGLPROCMACRO(glRequestResidentProgramsNV)
    GETGLPROCMACRO(glTrackMatrixNV)
    GETGLPROCMACRO(glVertexAttribPointerNV)
    GETGLPROCMACRO(glVertexAttrib1dNV)
    GETGLPROCMACRO(glVertexAttrib1dvNV)
    GETGLPROCMACRO(glVertexAttrib1fNV)
    GETGLPROCMACRO(glVertexAttrib1fvNV)
    GETGLPROCMACRO(glVertexAttrib1sNV)
    GETGLPROCMACRO(glVertexAttrib1svNV)
    GETGLPROCMACRO(glVertexAttrib2dNV)
    GETGLPROCMACRO(glVertexAttrib2dvNV)
    GETGLPROCMACRO(glVertexAttrib2fNV)
    GETGLPROCMACRO(glVertexAttrib2fvNV)
    GETGLPROCMACRO(glVertexAttrib2sNV)
    GETGLPROCMACRO(glVertexAttrib2svNV)
    GETGLPROCMACRO(glVertexAttrib3dNV)
    GETGLPROCMACRO(glVertexAttrib3dvNV)
    GETGLPROCMACRO(glVertexAttrib3fNV)
    GETGLPROCMACRO(glVertexAttrib3fvNV)
    GETGLPROCMACRO(glVertexAttrib3sNV)
    GETGLPROCMACRO(glVertexAttrib3svNV)
    GETGLPROCMACRO(glVertexAttrib4dNV)
    GETGLPROCMACRO(glVertexAttrib4dvNV)
    GETGLPROCMACRO(glVertexAttrib4fNV)
    GETGLPROCMACRO(glVertexAttrib4fvNV)
    GETGLPROCMACRO(glVertexAttrib4sNV)
    GETGLPROCMACRO(glVertexAttrib4svNV)
    GETGLPROCMACRO(glVertexAttrib4ubNV)
    GETGLPROCMACRO(glVertexAttrib4ubvNV)
    GETGLPROCMACRO(glVertexAttribs1dvNV)
    GETGLPROCMACRO(glVertexAttribs1fvNV)
    GETGLPROCMACRO(glVertexAttribs1svNV)
    GETGLPROCMACRO(glVertexAttribs2dvNV)
    GETGLPROCMACRO(glVertexAttribs2fvNV)
    GETGLPROCMACRO(glVertexAttribs2svNV)
    GETGLPROCMACRO(glVertexAttribs3dvNV)
    GETGLPROCMACRO(glVertexAttribs3fvNV)
    GETGLPROCMACRO(glVertexAttribs3svNV)
    GETGLPROCMACRO(glVertexAttribs4dvNV)
    GETGLPROCMACRO(glVertexAttribs4fvNV)
    GETGLPROCMACRO(glVertexAttribs4svNV)
    GETGLPROCMACRO(glVertexAttribs4ubvNV)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[237].avaible) {    /// #238 GLX_OML_sync_control http://www.opengl.org/registry/specs/OML/glx_sync_control.txt
    GETGLPROCMACRO(glXGetSyncValuesOML)
    GETGLPROCMACRO(glXGetMscRateOML)
    GETGLPROCMACRO(glXSwapBuffersMscOML)
    GETGLPROCMACRO(glXWaitForMscOML)
    GETGLPROCMACRO(glXWaitForSbcOML)
  }
  #endif
  #ifdef OS_WIN
  if(r->glEXTlist[241].avaible) {    /// #242 WGL_OML_sync_control http://www.opengl.org/registry/specs/OML/wgl_sync_control.txt
    GETGLPROCMACRO(wglGetSyncValuesOML)
    GETGLPROCMACRO(wglGetMscRateOML)
    GETGLPROCMACRO(wglSwapBuffersMscOML)
    GETGLPROCMACRO(wglSwapLayerBuffersMscOML)
    GETGLPROCMACRO(wglWaitForMscOML)
    GETGLPROCMACRO(wglWaitForSbcOML)
  }
  #endif
  if(r->glEXTlist[243].avaible) {    /// #244 GL_ATI_envmap_bumpmap http://www.opengl.org/registry/specs/ATI/envmap_bumpmap.txt
    GETGLPROCMACRO(glTexBumpParameterivATI)
    GETGLPROCMACRO(glTexBumpParameterfvATI)
    GETGLPROCMACRO(glGetTexBumpParameterivATI)
    GETGLPROCMACRO(glGetTexBumpParameterfvATI)
  }
  if(r->glEXTlist[244].avaible) {    /// #245 GL_ATI_fragment_shader http://www.opengl.org/registry/specs/ATI/fragment_shader.txt
    GETGLPROCMACRO(glGenFragmentShadersATI)
    GETGLPROCMACRO(glBindFragmentShaderATI)
    GETGLPROCMACRO(glDeleteFragmentShaderATI)
    GETGLPROCMACRO(glBeginFragmentShaderATI)
    GETGLPROCMACRO(glEndFragmentShaderATI)
    GETGLPROCMACRO(glPassTexCoordATI)
    GETGLPROCMACRO(glSampleMapATI)
    GETGLPROCMACRO(glColorFragmentOp1ATI)
    GETGLPROCMACRO(glColorFragmentOp2ATI)
    GETGLPROCMACRO(glColorFragmentOp3ATI)
    GETGLPROCMACRO(glAlphaFragmentOp1ATI)
    GETGLPROCMACRO(glAlphaFragmentOp2ATI)
    GETGLPROCMACRO(glAlphaFragmentOp3ATI)
    GETGLPROCMACRO(glSetFragmentShaderConstantATI)
  }
  if(r->glEXTlist[245].avaible) {    /// #246 GL_ATI_pn_triangles http://www.opengl.org/registry/specs/ATI/pn_triangles.txt
    GETGLPROCMACRO(glPNTrianglesiATI)
    GETGLPROCMACRO(glPNTrianglesfATI)
  }
  if(r->glEXTlist[246].avaible) {    /// #247 GL_ATI_vertex_array_object http://www.opengl.org/registry/specs/ATI/vertex_array_object.txt
    GETGLPROCMACRO(glNewObjectBufferATI)
    GETGLPROCMACRO(glIsObjectBufferATI)
    GETGLPROCMACRO(glUpdateObjectBufferATI)
    GETGLPROCMACRO(glGetObjectBufferfvATI)
    GETGLPROCMACRO(glGetObjectBufferivATI)
    GETGLPROCMACRO(glFreeObjectBufferATI)
    GETGLPROCMACRO(glArrayObjectATI)
    GETGLPROCMACRO(glGetArrayObjectfvATI)
    GETGLPROCMACRO(glGetArrayObjectivATI)
    GETGLPROCMACRO(glVariantArrayObjectATI)
    GETGLPROCMACRO(glGetVariantArrayObjectfvATI)
    GETGLPROCMACRO(glGetVariantArrayObjectivATI)
  }
  if(r->glEXTlist[247].avaible) {    /// #248 GL_EXT_vertex_shader http://www.opengl.org/registry/specs/EXT/vertex_shader.txt
    GETGLPROCMACRO(glBeginVertexShaderEXT)
    GETGLPROCMACRO(glEndVertexShaderEXT)
    GETGLPROCMACRO(glBindVertexShaderEXT)
    GETGLPROCMACRO(glGenVertexShadersEXT)
    GETGLPROCMACRO(glDeleteVertexShaderEXT)
    GETGLPROCMACRO(glShaderOp1EXT)
    GETGLPROCMACRO(glShaderOp2EXT)
    GETGLPROCMACRO(glShaderOp3EXT)
    GETGLPROCMACRO(glSwizzleEXT)
    GETGLPROCMACRO(glWriteMaskEXT)
    GETGLPROCMACRO(glInsertComponentEXT)
    GETGLPROCMACRO(glExtractComponentEXT)
    GETGLPROCMACRO(glGenSymbolsEXT)
    GETGLPROCMACRO(glSetInvariantEXT)
    GETGLPROCMACRO(glSetLocalConstantEXT)
    GETGLPROCMACRO(glVariantbvEXT)
    GETGLPROCMACRO(glVariantsvEXT)
    GETGLPROCMACRO(glVariantivEXT)
    GETGLPROCMACRO(glVariantfvEXT)
    GETGLPROCMACRO(glVariantdvEXT)
    GETGLPROCMACRO(glVariantubvEXT)
    GETGLPROCMACRO(glVariantusvEXT)
    GETGLPROCMACRO(glVariantuivEXT)
    GETGLPROCMACRO(glVariantPointerEXT)
    GETGLPROCMACRO(glEnableVariantClientStateEXT)
    GETGLPROCMACRO(glDisableVariantClientStateEXT)
    GETGLPROCMACRO(glBindLightParameterEXT)
    GETGLPROCMACRO(glBindMaterialParameterEXT)
    GETGLPROCMACRO(glBindTexGenParameterEXT)
    GETGLPROCMACRO(glBindTextureUnitParameterEXT)
    GETGLPROCMACRO(glBindParameterEXT)
    GETGLPROCMACRO(glIsVariantEnabledEXT)
    GETGLPROCMACRO(glGetVariantBooleanvEXT)
    GETGLPROCMACRO(glGetVariantIntegervEXT)
    GETGLPROCMACRO(glGetVariantFloatvEXT)
    GETGLPROCMACRO(glGetVariantPointervEXT)
    GETGLPROCMACRO(glGetInvariantBooleanvEXT)
    GETGLPROCMACRO(glGetInvariantIntegervEXT)
    GETGLPROCMACRO(glGetInvariantFloatvEXT)
    GETGLPROCMACRO(glGetLocalConstantBooleanvEXT)
    GETGLPROCMACRO(glGetLocalConstantIntegervEXT)
    GETGLPROCMACRO(glGetLocalConstantFloatvEXT)
  }
  if(r->glEXTlist[248].avaible) {    /// #249 GL_ATI_vertex_streams http://www.opengl.org/registry/specs/ATI/vertex_streams.txt
    GETGLPROCMACRO(glVertexStream1sATI)
    GETGLPROCMACRO(glVertexStream1svATI)
    GETGLPROCMACRO(glVertexStream1iATI)
    GETGLPROCMACRO(glVertexStream1ivATI)
    GETGLPROCMACRO(glVertexStream1fATI)
    GETGLPROCMACRO(glVertexStream1fvATI)
    GETGLPROCMACRO(glVertexStream1dATI)
    GETGLPROCMACRO(glVertexStream1dvATI)
    GETGLPROCMACRO(glVertexStream2sATI)
    GETGLPROCMACRO(glVertexStream2svATI)
    GETGLPROCMACRO(glVertexStream2iATI)
    GETGLPROCMACRO(glVertexStream2ivATI)
    GETGLPROCMACRO(glVertexStream2fATI)
    GETGLPROCMACRO(glVertexStream2fvATI)
    GETGLPROCMACRO(glVertexStream2dATI)
    GETGLPROCMACRO(glVertexStream2dvATI)
    GETGLPROCMACRO(glVertexStream3sATI)
    GETGLPROCMACRO(glVertexStream3svATI)
    GETGLPROCMACRO(glVertexStream3iATI)
    GETGLPROCMACRO(glVertexStream3ivATI)
    GETGLPROCMACRO(glVertexStream3fATI)
    GETGLPROCMACRO(glVertexStream3fvATI)
    GETGLPROCMACRO(glVertexStream3dATI)
    GETGLPROCMACRO(glVertexStream3dvATI)
    GETGLPROCMACRO(glVertexStream4sATI)
    GETGLPROCMACRO(glVertexStream4svATI)
    GETGLPROCMACRO(glVertexStream4iATI)
    GETGLPROCMACRO(glVertexStream4ivATI)
    GETGLPROCMACRO(glVertexStream4fATI)
    GETGLPROCMACRO(glVertexStream4fvATI)
    GETGLPROCMACRO(glVertexStream4dATI)
    GETGLPROCMACRO(glVertexStream4dvATI)
    GETGLPROCMACRO(glNormalStream3bATI)
    GETGLPROCMACRO(glNormalStream3bvATI)
    GETGLPROCMACRO(glNormalStream3sATI)
    GETGLPROCMACRO(glNormalStream3svATI)
    GETGLPROCMACRO(glNormalStream3iATI)
    GETGLPROCMACRO(glNormalStream3ivATI)
    GETGLPROCMACRO(glNormalStream3fATI)
    GETGLPROCMACRO(glNormalStream3fvATI)
    GETGLPROCMACRO(glNormalStream3dATI)
    GETGLPROCMACRO(glNormalStream3dvATI)
    GETGLPROCMACRO(glClientActiveVertexStreamATI)
    GETGLPROCMACRO(glVertexBlendEnviATI)
    GETGLPROCMACRO(glVertexBlendEnvfATI)
  }
  #ifdef OS_WIN
  if(r->glEXTlist[249].avaible) {    /// #250 WGL_I3D_digital_video_control http://www.opengl.org/registry/specs/I3D/wgl_digital_video_control.txt
    GETGLPROCMACRO(wglGetDigitalVideoParametersI3D)
    GETGLPROCMACRO(wglSetDigitalVideoParametersI3D)
  }
  if(r->glEXTlist[250].avaible) {    /// #251 WGL_I3D_gamma http://www.opengl.org/registry/specs/I3D/wgl_gamma.txt
    GETGLPROCMACRO(wglGetGammaTableParametersI3D)
    GETGLPROCMACRO(wglSetGammaTableParametersI3D)
    GETGLPROCMACRO(wglGetGammaTableI3D)
    GETGLPROCMACRO(wglSetGammaTableI3D)
  }
  if(r->glEXTlist[251].avaible) {    /// #252 WGL_I3D_genlock http://www.opengl.org/registry/specs/I3D/wgl_genlock.txt
    GETGLPROCMACRO(wglEnableGenlockI3D)
    GETGLPROCMACRO(wglDisableGenlockI3D)
    GETGLPROCMACRO(wglIsEnabledGenlockI3D)
    GETGLPROCMACRO(wglGenlockSourceI3D)
    GETGLPROCMACRO(wglGetGenlockSourceI3D)
    GETGLPROCMACRO(wglGenlockSourceEdgeI3D)
    GETGLPROCMACRO(wglGetGenlockSourceEdgeI3D)
    GETGLPROCMACRO(wglGenlockSampleRateI3D)
    GETGLPROCMACRO(wglGetGenlockSampleRateI3D)
    GETGLPROCMACRO(wglGenlockSourceDelayI3D)
    GETGLPROCMACRO(wglGetGenlockSourceDelayI3D)
    GETGLPROCMACRO(wglQueryGenlockMaxSourceDelayI3D)
  }
  if(r->glEXTlist[252].avaible) {    /// #253 WGL_I3D_image_buffer http://www.opengl.org/registry/specs/I3D/wgl_image_buffer.txt
    GETGLPROCMACRO(wglCreateImageBufferI3D)
    GETGLPROCMACRO(wglDestroyImageBufferI3D)
    GETGLPROCMACRO(wglAssociateImageBufferEventsI3D)
    GETGLPROCMACRO(wglReleaseImageBufferEventsI3D)
  }
  if(r->glEXTlist[253].avaible) {    /// #254 WGL_I3D_swap_frame_lock http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_lock.txt
    GETGLPROCMACRO(wglEnableFrameLockI3D)
    GETGLPROCMACRO(wglDisableFrameLockI3D)
    GETGLPROCMACRO(wglIsEnabledFrameLockI3D)
    GETGLPROCMACRO(wglQueryFrameLockMasterI3D)
  }
  if(r->glEXTlist[254].avaible) {    /// #255 WGL_I3D_swap_frame_usage http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_usage.txt
    GETGLPROCMACRO(wglGetFrameUsageI3D)
    GETGLPROCMACRO(wglBeginFrameTrackingI3D)
    GETGLPROCMACRO(wglEndFrameTrackingI3D)
    GETGLPROCMACRO(wglQueryFrameTrackingI3D)
  }
  #endif
  if(r->glEXTlist[255].avaible) {    /// #256 GL_ATI_element_array http://www.opengl.org/registry/specs/ATI/element_array.txt
    GETGLPROCMACRO(glElementPointerATI)
    GETGLPROCMACRO(glDrawElementArrayATI)
    GETGLPROCMACRO(glDrawRangeElementArrayATI)
  }
  if(r->glEXTlist[256].avaible) {    /// #257 GL_SUN_mesh_array http://www.opengl.org/registry/specs/SUN/mesh_array.txt
    GETGLPROCMACRO(glDrawMeshArraysSUN)
  }
  if(r->glEXTlist[260].avaible) {    /// #261 GL_NV_occlusion_query http://www.opengl.org/registry/specs/NV/occlusion_query.txt
    GETGLPROCMACRO(glGenOcclusionQueriesNV)
    GETGLPROCMACRO(glDeleteOcclusionQueriesNV)
    GETGLPROCMACRO(glIsOcclusionQueryNV)
    GETGLPROCMACRO(glBeginOcclusionQueryNV)
    GETGLPROCMACRO(glEndOcclusionQueryNV)
    GETGLPROCMACRO(glGetOcclusionQueryivNV)
    GETGLPROCMACRO(glGetOcclusionQueryuivNV)
  }
  if(r->glEXTlist[261].avaible) {    /// #262 GL_NV_point_sprite http://www.opengl.org/registry/specs/NV/point_sprite.txt
    GETGLPROCMACRO(glPointParameteriNV)
    GETGLPROCMACRO(glPointParameterivNV)
  }
  if(r->glEXTlist[267].avaible) {    /// #268 GL_EXT_stencil_two_side http://www.opengl.org/registry/specs/EXT/stencil_two_side.txt
    GETGLPROCMACRO(glActiveStencilFaceEXT)
  }
  if(r->glEXTlist[270].avaible) {    /// #271 GL_APPLE_element_array http://www.opengl.org/registry/specs/APPLE/element_array.txt
    GETGLPROCMACRO(glElementPointerAPPLE)
    GETGLPROCMACRO(glDrawElementArrayAPPLE)
    GETGLPROCMACRO(glDrawRangeElementArrayAPPLE)
    GETGLPROCMACRO(glMultiDrawElementArrayAPPLE)
    GETGLPROCMACRO(glMultiDrawRangeElementArrayAPPLE)
  }
  if(r->glEXTlist[271].avaible) {    /// #272 GL_APPLE_fence http://www.opengl.org/registry/specs/APPLE/fence.txt
    GETGLPROCMACRO(glGenFencesAPPLE)
    GETGLPROCMACRO(glDeleteFencesAPPLE)
    GETGLPROCMACRO(glSetFenceAPPLE)
    GETGLPROCMACRO(glIsFenceAPPLE)
    GETGLPROCMACRO(glTestFenceAPPLE)
    GETGLPROCMACRO(glFinishFenceAPPLE)
    GETGLPROCMACRO(glTestObjectAPPLE)
    GETGLPROCMACRO(glFinishObjectAPPLE)
  }
  if(r->glEXTlist[272].avaible) {    /// #273 GL_APPLE_vertex_array_object http://www.opengl.org/registry/specs/APPLE/vertex_array_object.txt
    GETGLPROCMACRO(glBindVertexArrayAPPLE)
    GETGLPROCMACRO(glDeleteVertexArraysAPPLE)
    GETGLPROCMACRO(glGenVertexArraysAPPLE)
    GETGLPROCMACRO(glIsVertexArrayAPPLE)
  }
  if(r->glEXTlist[273].avaible) {    /// #274 GL_APPLE_vertex_array_range http://www.opengl.org/registry/specs/APPLE/vertex_array_range.txt
    GETGLPROCMACRO(glVertexArrayRangeAPPLE)
    GETGLPROCMACRO(glFlushVertexArrayRangeAPPLE)
    GETGLPROCMACRO(glVertexArrayParameteriAPPLE)
  }
  if(r->glEXTlist[276].avaible) {    /// #277 GL_ATI_draw_buffers http://www.opengl.org/registry/specs/ATI/draw_buffers.txt
    GETGLPROCMACRO(glDrawBuffersATI)
  }
  if(r->glEXTlist[281].avaible) {    /// #282 GL_NV_fragment_program http://www.opengl.org/registry/specs/NV/fragment_program.txt
    GETGLPROCMACRO(glProgramNamedParameter4fNV)
    GETGLPROCMACRO(glProgramNamedParameter4fvNV)
    GETGLPROCMACRO(glProgramNamedParameter4dNV)
    GETGLPROCMACRO(glProgramNamedParameter4dvNV)
    GETGLPROCMACRO(glGetProgramNamedParameterfvNV)
    GETGLPROCMACRO(glGetProgramNamedParameterdvNV)
  }
  if(r->glEXTlist[282].avaible) {    /// #283 GL_NV_half_float http://www.opengl.org/registry/specs/NV/half_float.txt
    GETGLPROCMACRO(glVertex2hNV)
    GETGLPROCMACRO(glVertex2hvNV)
    GETGLPROCMACRO(glVertex3hNV)
    GETGLPROCMACRO(glVertex3hvNV)
    GETGLPROCMACRO(glVertex4hNV)
    GETGLPROCMACRO(glVertex4hvNV)
    GETGLPROCMACRO(glNormal3hNV)
    GETGLPROCMACRO(glNormal3hvNV)
    GETGLPROCMACRO(glColor3hNV)
    GETGLPROCMACRO(glColor3hvNV)
    GETGLPROCMACRO(glColor4hNV)
    GETGLPROCMACRO(glColor4hvNV)
    GETGLPROCMACRO(glTexCoord1hNV)
    GETGLPROCMACRO(glTexCoord1hvNV)
    GETGLPROCMACRO(glTexCoord2hNV)
    GETGLPROCMACRO(glTexCoord2hvNV)
    GETGLPROCMACRO(glTexCoord3hNV)
    GETGLPROCMACRO(glTexCoord3hvNV)
    GETGLPROCMACRO(glTexCoord4hNV)
    GETGLPROCMACRO(glTexCoord4hvNV)
    GETGLPROCMACRO(glMultiTexCoord1hNV)
    GETGLPROCMACRO(glMultiTexCoord1hvNV)
    GETGLPROCMACRO(glMultiTexCoord2hNV)
    GETGLPROCMACRO(glMultiTexCoord2hvNV)
    GETGLPROCMACRO(glMultiTexCoord3hNV)
    GETGLPROCMACRO(glMultiTexCoord3hvNV)
    GETGLPROCMACRO(glMultiTexCoord4hNV)
    GETGLPROCMACRO(glMultiTexCoord4hvNV)
    GETGLPROCMACRO(glFogCoordhNV)
    GETGLPROCMACRO(glFogCoordhvNV)
    GETGLPROCMACRO(glSecondaryColor3hNV)
    GETGLPROCMACRO(glSecondaryColor3hvNV)
    GETGLPROCMACRO(glVertexWeighthNV)
    GETGLPROCMACRO(glVertexWeighthvNV)
    GETGLPROCMACRO(glVertexAttrib1hNV)
    GETGLPROCMACRO(glVertexAttrib1hvNV)
    GETGLPROCMACRO(glVertexAttrib2hNV)
    GETGLPROCMACRO(glVertexAttrib2hvNV)
    GETGLPROCMACRO(glVertexAttrib3hNV)
    GETGLPROCMACRO(glVertexAttrib3hvNV)
    GETGLPROCMACRO(glVertexAttrib4hNV)
    GETGLPROCMACRO(glVertexAttrib4hvNV)
    GETGLPROCMACRO(glVertexAttribs1hvNV)
    GETGLPROCMACRO(glVertexAttribs2hvNV)
    GETGLPROCMACRO(glVertexAttribs3hvNV)
    GETGLPROCMACRO(glVertexAttribs4hvNV)
  }
  if(r->glEXTlist[283].avaible) {    /// #284 GL_NV_pixel_data_range http://www.opengl.org/registry/specs/NV/pixel_data_range.txt
    GETGLPROCMACRO(glPixelDataRangeNV)
    GETGLPROCMACRO(glFlushPixelDataRangeNV)
  }
  if(r->glEXTlist[284].avaible) {    /// #285 GL_NV_primitive_restart http://www.opengl.org/registry/specs/NV/primitive_restart.txt
    GETGLPROCMACRO(glPrimitiveRestartNV)
    GETGLPROCMACRO(glPrimitiveRestartIndexNV)
  }
  if(r->glEXTlist[287].avaible) {    /// #288 GL_ATI_map_object_buffer http://www.opengl.org/registry/specs/ATI/map_object_buffer.txt
    GETGLPROCMACRO(glMapObjectBufferATI)
    GETGLPROCMACRO(glUnmapObjectBufferATI)
  }
  if(r->glEXTlist[288].avaible) {    /// #289 GL_ATI_separate_stencil http://www.opengl.org/registry/specs/ATI/separate_stencil.txt
    GETGLPROCMACRO(glStencilOpSeparateATI)
    GETGLPROCMACRO(glStencilFuncSeparateATI)
  }
  if(r->glEXTlist[289].avaible) {    /// #290 GL_ATI_vertex_attrib_array_object http://www.opengl.org/registry/specs/ATI/vertex_attrib_array_object.txt
    GETGLPROCMACRO(glVertexAttribArrayObjectATI)
    GETGLPROCMACRO(glGetVertexAttribArrayObjectfvATI)
    GETGLPROCMACRO(glGetVertexAttribArrayObjectivATI)
  }
  if(r->glEXTlist[290].avaible) {    /// #291 GL_OES_byte_coordinates http://www.opengl.org/registry/specs/OES/OES_byte_coordinates.txt
    GETGLPROCMACRO(glMultiTexCoord1bOES)
    GETGLPROCMACRO(glMultiTexCoord1bvOES)
    GETGLPROCMACRO(glMultiTexCoord2bOES)
    GETGLPROCMACRO(glMultiTexCoord2bvOES)
    GETGLPROCMACRO(glMultiTexCoord3bOES)
    GETGLPROCMACRO(glMultiTexCoord3bvOES)
    GETGLPROCMACRO(glMultiTexCoord4bOES)
    GETGLPROCMACRO(glMultiTexCoord4bvOES)
    GETGLPROCMACRO(glTexCoord1bOES)
    GETGLPROCMACRO(glTexCoord1bvOES)
    GETGLPROCMACRO(glTexCoord2bOES)
    GETGLPROCMACRO(glTexCoord2bvOES)
    GETGLPROCMACRO(glTexCoord3bOES)
    GETGLPROCMACRO(glTexCoord3bvOES)
    GETGLPROCMACRO(glTexCoord4bOES)
    GETGLPROCMACRO(glTexCoord4bvOES)
    GETGLPROCMACRO(glVertex2bOES)
    GETGLPROCMACRO(glVertex2bvOES)
    GETGLPROCMACRO(glVertex3bOES)
    GETGLPROCMACRO(glVertex3bvOES)
    GETGLPROCMACRO(glVertex4bOES)
    GETGLPROCMACRO(glVertex4bvOES)
  }
  if(r->glEXTlist[291].avaible) {    /// #292 GL_OES_fixed_point http://www.opengl.org/registry/specs/OES/OES_fixed_point.txt
    GETGLPROCMACRO(glAlphaFuncxOES)
    GETGLPROCMACRO(glClearColorxOES)
    GETGLPROCMACRO(glClearDepthxOES)
    GETGLPROCMACRO(glClipPlanexOES)
    GETGLPROCMACRO(glColor4xOES)
    GETGLPROCMACRO(glDepthRangexOES)
    GETGLPROCMACRO(glFogxOES)
    GETGLPROCMACRO(glFogxvOES)
    GETGLPROCMACRO(glFrustumxOES)
    GETGLPROCMACRO(glGetClipPlanexOES)
    GETGLPROCMACRO(glGetFixedvOES)
    GETGLPROCMACRO(glGetTexEnvxvOES)
    GETGLPROCMACRO(glGetTexParameterxvOES)
    GETGLPROCMACRO(glLightModelxOES)
    GETGLPROCMACRO(glLightModelxvOES)
    GETGLPROCMACRO(glLightxOES)
    GETGLPROCMACRO(glLightxvOES)
    GETGLPROCMACRO(glLineWidthxOES)
    GETGLPROCMACRO(glLoadMatrixxOES)
    GETGLPROCMACRO(glMaterialxOES)
    GETGLPROCMACRO(glMaterialxvOES)
    GETGLPROCMACRO(glMultMatrixxOES)
    GETGLPROCMACRO(glMultiTexCoord4xOES)
    GETGLPROCMACRO(glNormal3xOES)
    GETGLPROCMACRO(glOrthoxOES)
    GETGLPROCMACRO(glPointParameterxvOES)
    GETGLPROCMACRO(glPointSizexOES)
    GETGLPROCMACRO(glPolygonOffsetxOES)
    GETGLPROCMACRO(glRotatexOES)
    GETGLPROCMACRO(glSampleCoverageOES)
    GETGLPROCMACRO(glScalexOES)
    GETGLPROCMACRO(glTexEnvxOES)
    GETGLPROCMACRO(glTexEnvxvOES)
    GETGLPROCMACRO(glTexParameterxOES)
    GETGLPROCMACRO(glTexParameterxvOES)
    GETGLPROCMACRO(glTranslatexOES)
    GETGLPROCMACRO(glAccumxOES)
    GETGLPROCMACRO(glBitmapxOES)
    GETGLPROCMACRO(glBlendColorxOES)
    GETGLPROCMACRO(glClearAccumxOES)
    GETGLPROCMACRO(glColor3xOES)
    GETGLPROCMACRO(glColor3xvOES)
    GETGLPROCMACRO(glColor4xvOES)
    GETGLPROCMACRO(glConvolutionParameterxOES)
    GETGLPROCMACRO(glConvolutionParameterxvOES)
    GETGLPROCMACRO(glEvalCoord1xOES)
    GETGLPROCMACRO(glEvalCoord1xvOES)
    GETGLPROCMACRO(glEvalCoord2xOES)
    GETGLPROCMACRO(glEvalCoord2xvOES)
    GETGLPROCMACRO(glFeedbackBufferxOES)
    GETGLPROCMACRO(glGetConvolutionParameterxvOES)
    GETGLPROCMACRO(glGetHistogramParameterxvOES)
    GETGLPROCMACRO(glGetLightxOES)
    GETGLPROCMACRO(glGetMapxvOES)
    GETGLPROCMACRO(glGetMaterialxOES)
    GETGLPROCMACRO(glGetPixelMapxv)
    GETGLPROCMACRO(glGetTexGenxvOES)
    GETGLPROCMACRO(glGetTexLevelParameterxvOES)
    GETGLPROCMACRO(glIndexxOES)
    GETGLPROCMACRO(glIndexxvOES)
    GETGLPROCMACRO(glLoadTransposeMatrixxOES)
    GETGLPROCMACRO(glMap1xOES)
    GETGLPROCMACRO(glMap2xOES)
    GETGLPROCMACRO(glMapGrid1xOES)
    GETGLPROCMACRO(glMapGrid2xOES)
    GETGLPROCMACRO(glMultTransposeMatrixxOES)
    GETGLPROCMACRO(glMultiTexCoord1xOES)
    GETGLPROCMACRO(glMultiTexCoord1xvOES)
    GETGLPROCMACRO(glMultiTexCoord2xOES)
    GETGLPROCMACRO(glMultiTexCoord2xvOES)
    GETGLPROCMACRO(glMultiTexCoord3xOES)
    GETGLPROCMACRO(glMultiTexCoord3xvOES)
    GETGLPROCMACRO(glMultiTexCoord4xvOES)
    GETGLPROCMACRO(glNormal3xvOES)
    GETGLPROCMACRO(glPassThroughxOES)
    GETGLPROCMACRO(glPixelMapx)
    GETGLPROCMACRO(glPixelStorex)
    GETGLPROCMACRO(glPixelTransferxOES)
    GETGLPROCMACRO(glPixelZoomxOES)
    GETGLPROCMACRO(glPrioritizeTexturesxOES)
    GETGLPROCMACRO(glRasterPos2xOES)
    GETGLPROCMACRO(glRasterPos2xvOES)
    GETGLPROCMACRO(glRasterPos3xOES)
    GETGLPROCMACRO(glRasterPos3xvOES)
    GETGLPROCMACRO(glRasterPos4xOES)
    GETGLPROCMACRO(glRasterPos4xvOES)
    GETGLPROCMACRO(glRectxOES)
    GETGLPROCMACRO(glRectxvOES)
    GETGLPROCMACRO(glTexCoord1xOES)
    GETGLPROCMACRO(glTexCoord1xvOES)
    GETGLPROCMACRO(glTexCoord2xOES)
    GETGLPROCMACRO(glTexCoord2xvOES)
    GETGLPROCMACRO(glTexCoord3xOES)
    GETGLPROCMACRO(glTexCoord3xvOES)
    GETGLPROCMACRO(glTexCoord4xOES)
    GETGLPROCMACRO(glTexCoord4xvOES)
    GETGLPROCMACRO(glTexGenxOES)
    GETGLPROCMACRO(glTexGenxvOES)
    GETGLPROCMACRO(glVertex2xOES)
    GETGLPROCMACRO(glVertex2xvOES)
    GETGLPROCMACRO(glVertex3xOES)
    GETGLPROCMACRO(glVertex3xvOES)
    GETGLPROCMACRO(glVertex4xOES)
    GETGLPROCMACRO(glVertex4xvOES)
  }
  if(r->glEXTlist[292].avaible) {    /// #293 GL_OES_single_precision http://www.opengl.org/registry/specs/OES/OES_single_precision.txt
    GETGLPROCMACRO(glClearDepthfOES)
    GETGLPROCMACRO(glClipPlanefOES)
    GETGLPROCMACRO(glDepthRangefOES)
    GETGLPROCMACRO(glFrustumfOES)
    GETGLPROCMACRO(glGetClipPlanefOES)
    GETGLPROCMACRO(glOrthofOES)
  }
  if(r->glEXTlist[295].avaible) {    /// #296 GL_OES_query_matrix http://www.opengl.org/registry/specs/OES/OES_query_matrix.txt
    GETGLPROCMACRO(glQueryMatrixxOES)
  }
  if(r->glEXTlist[296].avaible) {    /// #297 GL_EXT_depth_bounds_test http://www.opengl.org/registry/specs/EXT/depth_bounds_test.txt
    GETGLPROCMACRO(glDepthBoundsEXT)
  }
  if(r->glEXTlist[298].avaible) {    /// #299 GL_EXT_blend_equation_separate http://www.opengl.org/registry/specs/EXT/blend_equation_separate.txt
    GETGLPROCMACRO(glBlendEquationSeparateEXT)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[306].avaible) {    /// #307 GLX_SGIX_hyperpipe http://www.opengl.org/registry/specs/SGIX/hyperpipe_group.txt
    GETGLPROCMACRO(glXQueryHyperpipeNetworkSGIX)
    GETGLPROCMACRO(glXHyperpipeConfigSGIX)
    GETGLPROCMACRO(glXQueryHyperpipeConfigSGIX)
    GETGLPROCMACRO(glXDestroyHyperpipeConfigSGIX)
    GETGLPROCMACRO(glXBindHyperpipeSGIX)
    GETGLPROCMACRO(glXQueryHyperpipeBestAttribSGIX)
    GETGLPROCMACRO(glXHyperpipeAttribSGIX)
    GETGLPROCMACRO(glXQueryHyperpipeAttribSGIX)
  }
  if(r->glEXTlist[307].avaible) {    /// #308 GLX_MESA_agp_offset http://www.opengl.org/registry/specs/MESA/agp_offset.txt
    GETGLPROCMACRO(glXGetAGPOffsetMESA)
  }
  #endif
  if(r->glEXTlist[309].avaible) {    /// #310 GL_EXT_framebuffer_object http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
    GETGLPROCMACRO(glIsRenderbufferEXT)
    GETGLPROCMACRO(glBindRenderbufferEXT)
    GETGLPROCMACRO(glDeleteRenderbuffersEXT)
    GETGLPROCMACRO(glGenRenderbuffersEXT)
    GETGLPROCMACRO(glRenderbufferStorageEXT)
    GETGLPROCMACRO(glGetRenderbufferParameterivEXT)
    GETGLPROCMACRO(glIsFramebufferEXT)
    GETGLPROCMACRO(glBindFramebufferEXT)
    GETGLPROCMACRO(glDeleteFramebuffersEXT)
    GETGLPROCMACRO(glGenFramebuffersEXT)
    GETGLPROCMACRO(glCheckFramebufferStatusEXT)
    GETGLPROCMACRO(glFramebufferTexture1DEXT)
    GETGLPROCMACRO(glFramebufferTexture2DEXT)
    GETGLPROCMACRO(glFramebufferTexture3DEXT)
    GETGLPROCMACRO(glFramebufferRenderbufferEXT)
    GETGLPROCMACRO(glGetFramebufferAttachmentParameterivEXT)
    GETGLPROCMACRO(glGenerateMipmapEXT)
  }
  if(r->glEXTlist[310].avaible) {    /// #311 GL_GREMEDY_string_marker http://www.opengl.org/registry/specs/GREMEDY/string_marker.txt
    GETGLPROCMACRO(glStringMarkerGREMEDY)
  }
  #ifdef OS_WIN
  if(r->glEXTlist[312].avaible) {    /// #313 WGL_3DL_stereo_control http://www.opengl.org/registry/specs/3DL/stereo_control.txt
    GETGLPROCMACRO(wglSetStereoEmitterState3DL)
  }
  #endif
  if(r->glEXTlist[313].avaible) {    /// #314 GL_EXT_stencil_clear_tag http://www.opengl.org/registry/specs/EXT/stencil_clear_tag.txt
    GETGLPROCMACRO(glStencilClearTagEXT)
  }
  if(r->glEXTlist[315].avaible) {    /// #316 GL_EXT_framebuffer_blit http://www.opengl.org/registry/specs/EXT/framebuffer_blit.txt
    GETGLPROCMACRO(glBlitFramebufferEXT)
  }
  if(r->glEXTlist[316].avaible) {    /// #317 GL_EXT_framebuffer_multisample http://www.opengl.org/registry/specs/EXT/framebuffer_multisample.txt
    GETGLPROCMACRO(glRenderbufferStorageMultisampleEXT)
  }
  if(r->glEXTlist[318].avaible) {    /// #319 GL_EXT_timer_query http://www.opengl.org/registry/specs/EXT/timer_query.txt
    GETGLPROCMACRO(glGetQueryObjecti64vEXT)
    GETGLPROCMACRO(glGetQueryObjectui64vEXT)
  }
  if(r->glEXTlist[319].avaible) {    /// #320 GL_EXT_gpu_program_parameters http://www.opengl.org/registry/specs/EXT/gpu_program_parameters.txt
    GETGLPROCMACRO(glProgramEnvParameters4fvEXT)
    GETGLPROCMACRO(glProgramLocalParameters4fvEXT)
  }
  if(r->glEXTlist[320].avaible) {    /// #321 GL_APPLE_flush_buffer_range http://www.opengl.org/registry/specs/APPLE/flush_buffer_range.txt
    GETGLPROCMACRO(glBufferParameteriAPPLE)
    GETGLPROCMACRO(glFlushMappedBufferRangeAPPLE)
  }
  if(r->glEXTlist[321].avaible) {    /// #322 GL_NV_gpu_program4 http://www.opengl.org/registry/specs/NV/gpu_program4.txt
    GETGLPROCMACRO(glProgramLocalParameterI4iNV)
    GETGLPROCMACRO(glProgramLocalParameterI4ivNV)
    GETGLPROCMACRO(glProgramLocalParametersI4ivNV)
    GETGLPROCMACRO(glProgramLocalParameterI4uiNV)
    GETGLPROCMACRO(glProgramLocalParameterI4uivNV)
    GETGLPROCMACRO(glProgramLocalParametersI4uivNV)
    GETGLPROCMACRO(glProgramEnvParameterI4iNV)
    GETGLPROCMACRO(glProgramEnvParameterI4ivNV)
    GETGLPROCMACRO(glProgramEnvParametersI4ivNV)
    GETGLPROCMACRO(glProgramEnvParameterI4uiNV)
    GETGLPROCMACRO(glProgramEnvParameterI4uivNV)
    GETGLPROCMACRO(glProgramEnvParametersI4uivNV)
    GETGLPROCMACRO(glGetProgramLocalParameterIivNV)
    GETGLPROCMACRO(glGetProgramLocalParameterIuivNV)
    GETGLPROCMACRO(glGetProgramEnvParameterIivNV)
    GETGLPROCMACRO(glGetProgramEnvParameterIuivNV)
  }
  if(r->glEXTlist[322].avaible) {    /// #323 GL_NV_geometry_program4 http://www.opengl.org/registry/specs/NV/geometry_program4.txt
    GETGLPROCMACRO(glProgramVertexLimitNV)
    GETGLPROCMACRO(glFramebufferTextureEXT)
    GETGLPROCMACRO(glFramebufferTextureLayerEXT)
    GETGLPROCMACRO(glFramebufferTextureFaceEXT)
  }
  if(r->glEXTlist[323].avaible) {    /// #324 GL_EXT_geometry_shader4 http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
    GETGLPROCMACRO(glProgramParameteriEXT)
  }
  if(r->glEXTlist[324].avaible) {    /// #325 GL_NV_vertex_program4 http://www.opengl.org/registry/specs/NV/vertex_program4.txt
    GETGLPROCMACRO(glVertexAttribI1iEXT)
    GETGLPROCMACRO(glVertexAttribI2iEXT)
    GETGLPROCMACRO(glVertexAttribI3iEXT)
    GETGLPROCMACRO(glVertexAttribI4iEXT)
    GETGLPROCMACRO(glVertexAttribI1uiEXT)
    GETGLPROCMACRO(glVertexAttribI2uiEXT)
    GETGLPROCMACRO(glVertexAttribI3uiEXT)
    GETGLPROCMACRO(glVertexAttribI4uiEXT)
    GETGLPROCMACRO(glVertexAttribI1ivEXT)
    GETGLPROCMACRO(glVertexAttribI2ivEXT)
    GETGLPROCMACRO(glVertexAttribI3ivEXT)
    GETGLPROCMACRO(glVertexAttribI4ivEXT)
    GETGLPROCMACRO(glVertexAttribI1uivEXT)
    GETGLPROCMACRO(glVertexAttribI2uivEXT)
    GETGLPROCMACRO(glVertexAttribI3uivEXT)
    GETGLPROCMACRO(glVertexAttribI4uivEXT)
    GETGLPROCMACRO(glVertexAttribI4bvEXT)
    GETGLPROCMACRO(glVertexAttribI4svEXT)
    GETGLPROCMACRO(glVertexAttribI4ubvEXT)
    GETGLPROCMACRO(glVertexAttribI4usvEXT)
    GETGLPROCMACRO(glVertexAttribIPointerEXT)
    GETGLPROCMACRO(glGetVertexAttribIivEXT)
    GETGLPROCMACRO(glGetVertexAttribIuivEXT)
  }
  if(r->glEXTlist[325].avaible) {    /// #326 GL_EXT_gpu_shader4 http://www.opengl.org/registry/specs/EXT/gpu_shader4.txt
    GETGLPROCMACRO(glGetUniformuivEXT)
    GETGLPROCMACRO(glBindFragDataLocationEXT)
    GETGLPROCMACRO(glGetFragDataLocationEXT)
    GETGLPROCMACRO(glUniform1uiEXT)
    GETGLPROCMACRO(glUniform2uiEXT)
    GETGLPROCMACRO(glUniform3uiEXT)
    GETGLPROCMACRO(glUniform4uiEXT)
    GETGLPROCMACRO(glUniform1uivEXT)
    GETGLPROCMACRO(glUniform2uivEXT)
    GETGLPROCMACRO(glUniform3uivEXT)
    GETGLPROCMACRO(glUniform4uivEXT)
  }
  if(r->glEXTlist[326].avaible) {    /// #327 GL_EXT_draw_instanced http://www.opengl.org/registry/specs/EXT/draw_instanced.txt
    GETGLPROCMACRO(glDrawArraysInstancedEXT)
    GETGLPROCMACRO(glDrawElementsInstancedEXT)
  }
  if(r->glEXTlist[329].avaible) {    /// #330 GL_EXT_texture_buffer_object http://www.opengl.org/registry/specs/EXT/texture_buffer_object.txt
    GETGLPROCMACRO(glTexBufferEXT)
  }
  if(r->glEXTlist[333].avaible) {    /// #334 GL_NV_depth_buffer_float http://www.opengl.org/registry/specs/NV/depth_buffer_float.txt
    GETGLPROCMACRO(glDepthRangedNV)
    GETGLPROCMACRO(glClearDepthdNV)
    GETGLPROCMACRO(glDepthBoundsdNV)
  }
  if(r->glEXTlist[335].avaible) {    /// #336 GL_NV_framebuffer_multisample_coverage http://www.opengl.org/registry/specs/NV/framebuffer_multisample_coverage.txt
    GETGLPROCMACRO(glRenderbufferStorageMultisampleCoverageNV)
  }
  if(r->glEXTlist[338].avaible) {    /// #339 GL_NV_parameter_buffer_object http://www.opengl.org/registry/specs/NV/parameter_buffer_object.txt
    GETGLPROCMACRO(glProgramBufferParametersfvNV)
    GETGLPROCMACRO(glProgramBufferParametersIivNV)
    GETGLPROCMACRO(glProgramBufferParametersIuivNV)
  }
  if(r->glEXTlist[339].avaible) {    /// #340 GL_EXT_draw_buffers2 http://www.opengl.org/registry/specs/EXT/draw_buffers2.txt
    GETGLPROCMACRO(glColorMaskIndexedEXT)
  }
  if(r->glEXTlist[340].avaible) {    /// #341 GL_NV_transform_feedback http://www.opengl.org/registry/specs/NV/transform_feedback.txt
    GETGLPROCMACRO(glBeginTransformFeedbackNV)
    GETGLPROCMACRO(glEndTransformFeedbackNV)
    GETGLPROCMACRO(glTransformFeedbackAttribsNV)
    GETGLPROCMACRO(glBindBufferRangeNV)
    GETGLPROCMACRO(glBindBufferOffsetNV)
    GETGLPROCMACRO(glBindBufferBaseNV)
    GETGLPROCMACRO(glTransformFeedbackVaryingsNV)
    GETGLPROCMACRO(glActiveVaryingNV)
    GETGLPROCMACRO(glGetVaryingLocationNV)
    GETGLPROCMACRO(glGetActiveVaryingNV)
    GETGLPROCMACRO(glGetTransformFeedbackVaryingNV)
    GETGLPROCMACRO(glTransformFeedbackStreamAttribsNV)
  }
  if(r->glEXTlist[341].avaible) {    /// #342 GL_EXT_bindable_uniform http://www.opengl.org/registry/specs/EXT/bindable_uniform.txt
    GETGLPROCMACRO(glUniformBufferEXT)
    GETGLPROCMACRO(glGetUniformBufferSizeEXT)
    GETGLPROCMACRO(glGetUniformOffsetEXT)
  }
  if(r->glEXTlist[342].avaible) {    /// #343 GL_EXT_texture_integer http://www.opengl.org/registry/specs/EXT/texture_integer.txt
    GETGLPROCMACRO(glTexParameterIivEXT)
    GETGLPROCMACRO(glTexParameterIuivEXT)
    GETGLPROCMACRO(glGetTexParameterIivEXT)
    GETGLPROCMACRO(glGetTexParameterIuivEXT)
    GETGLPROCMACRO(glClearColorIiEXT)
    GETGLPROCMACRO(glClearColorIuiEXT)
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[343].avaible) {    /// #344 GLX_EXT_texture_from_pixmap http://www.opengl.org/registry/specs/EXT/texture_from_pixmap.txt
    GETGLPROCMACRO(glXBindTexImageEXT)
    GETGLPROCMACRO(glXReleaseTexImageEXT)
  }
  #endif
  if(r->glEXTlist[344].avaible) {    /// #345 GL_GREMEDY_frame_terminator http://www.opengl.org/registry/specs/GREMEDY/frame_terminator.txt
    GETGLPROCMACRO(glFrameTerminatorGREMEDY)
  }
  if(r->glEXTlist[345].avaible) {    /// #346 GL_NV_conditional_render http://www.opengl.org/registry/specs/NV/conditional_render.txt
    GETGLPROCMACRO(glBeginConditionalRenderNV)
    GETGLPROCMACRO(glEndConditionalRenderNV)
  }
  if(r->glEXTlist[346].avaible) {    /// #347 GL_NV_present_video GLX_NV_present_video WGL_NV_present_video http://www.opengl.org/registry/specs/NV/present_video.txt
    GETGLPROCMACRO(glPresentFrameKeyedNV)
    GETGLPROCMACRO(glPresentFrameDualFillNV)
    GETGLPROCMACRO(glGetVideoivNV)
    GETGLPROCMACRO(glGetVideouivNV)
    GETGLPROCMACRO(glGetVideoi64vNV)
    GETGLPROCMACRO(glGetVideoui64vNV)
    #ifdef OS_LINUX
    GETGLPROCMACRO(glXEnumerateVideoDevicesNV)
    GETGLPROCMACRO(glXBindVideoDeviceNV)
    #endif /// OS_LINUX
    #ifdef OS_WIN
    GETGLPROCMACRO(wglEnumerateVideoDevicesNV)
    GETGLPROCMACRO(wglBindVideoDeviceNV)
    GETGLPROCMACRO(wglQueryCurrentContextNV)
    #endif /// OS_WIN
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[347].avaible) {    /// #348 GLX_NV_video_out http://www.opengl.org/registry/specs/NV/glx_video_output.txt
    GETGLPROCMACRO(glXGetVideoDeviceNV)
    GETGLPROCMACRO(glXReleaseVideoDeviceNV)
    GETGLPROCMACRO(glXBindVideoImageNV)
    GETGLPROCMACRO(glXReleaseVideoImageNV)
    GETGLPROCMACRO(glXSendPbufferToVideoNV)
    GETGLPROCMACRO(glXGetVideoInfoNV)
  }
  #endif
  #ifdef OS_WIN
  if(r->glEXTlist[348].avaible) {    /// #349 WGL_NV_video_output http://www.opengl.org/registry/specs/NV/wgl_video_output.txt
    GETGLPROCMACRO(wglGetVideoDeviceNV)
    GETGLPROCMACRO(wglReleaseVideoDeviceNV)
    GETGLPROCMACRO(wglBindVideoImageNV)
    GETGLPROCMACRO(wglReleaseVideoImageNV)
    GETGLPROCMACRO(wglSendPbufferToVideoNV)
    GETGLPROCMACRO(wglGetVideoInfoNV)
  }
  #endif
  #ifdef OS_LINUX
  if(r->glEXTlist[349].avaible) {    /// #350 GLX_NV_swap_group http://www.opengl.org/registry/specs/NV/glx_swap_group.txt
    GETGLPROCMACRO(glXJoinSwapGroupNV)
    GETGLPROCMACRO(glXBindSwapBarrierNV)
    GETGLPROCMACRO(glXQuerySwapGroupNV)
    GETGLPROCMACRO(glXQueryMaxSwapGroupsNV)
    GETGLPROCMACRO(glXQueryFrameCountNV)
    GETGLPROCMACRO(glXResetFrameCountNV)
  }
  #endif
  #ifdef OS_WIN
  if(r->glEXTlist[350].avaible) {    /// #351 WGL_NV_swap_group http://www.opengl.org/registry/specs/NV/wgl_swap_group.txt
    GETGLPROCMACRO(wglJoinSwapGroupNV)
    GETGLPROCMACRO(wglBindSwapBarrierNV)
    GETGLPROCMACRO(wglQuerySwapGroupNV)
    GETGLPROCMACRO(wglQueryMaxSwapGroupsNV)
    GETGLPROCMACRO(wglQueryFrameCountNV)
    GETGLPROCMACRO(wglResetFrameCountNV)
  }
  #endif
  if(r->glEXTlist[351].avaible) {    /// #352 GL_EXT_transform_feedback http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
    GETGLPROCMACRO(glBeginTransformFeedbackEXT)
    GETGLPROCMACRO(glEndTransformFeedbackEXT)
    GETGLPROCMACRO(glBindBufferRangeEXT)
    GETGLPROCMACRO(glBindBufferOffsetEXT)
    GETGLPROCMACRO(glBindBufferBaseEXT)
    GETGLPROCMACRO(glTransformFeedbackVaryingsEXT)
    GETGLPROCMACRO(glGetTransformFeedbackVaryingEXT)
  }
  if(r->glEXTlist[352].avaible) {    /// #353 GL_EXT_direct_state_access http://www.opengl.org/registry/specs/EXT/direct_state_access.txt
    GETGLPROCMACRO(glMatrixLoadfEXT)
    GETGLPROCMACRO(glMatrixLoaddEXT)
    GETGLPROCMACRO(glMatrixMultfEXT)
    GETGLPROCMACRO(glMatrixMultdEXT)
    GETGLPROCMACRO(glMatrixLoadIdentityEXT)
    GETGLPROCMACRO(glMatrixRotatefEXT)
    GETGLPROCMACRO(glMatrixRotatedEXT)
    GETGLPROCMACRO(glMatrixScalefEXT)
    GETGLPROCMACRO(glMatrixScaledEXT)
    GETGLPROCMACRO(glMatrixTranslatefEXT)
    GETGLPROCMACRO(glMatrixTranslatedEXT)
    GETGLPROCMACRO(glMatrixFrustumEXT)
    GETGLPROCMACRO(glMatrixOrthoEXT)
    GETGLPROCMACRO(glMatrixPopEXT)
    GETGLPROCMACRO(glMatrixPushEXT)
    GETGLPROCMACRO(glClientAttribDefaultEXT)
    GETGLPROCMACRO(glPushClientAttribDefaultEXT)
    GETGLPROCMACRO(glTextureParameterfEXT)
    GETGLPROCMACRO(glTextureParameterfvEXT)
    GETGLPROCMACRO(glTextureParameteriEXT)
    GETGLPROCMACRO(glTextureParameterivEXT)
    GETGLPROCMACRO(glTextureImage1DEXT)
    GETGLPROCMACRO(glTextureImage2DEXT)
    GETGLPROCMACRO(glTextureSubImage1DEXT)
    GETGLPROCMACRO(glTextureSubImage2DEXT)
    GETGLPROCMACRO(glCopyTextureImage1DEXT)
    GETGLPROCMACRO(glCopyTextureImage2DEXT)
    GETGLPROCMACRO(glCopyTextureSubImage1DEXT)
    GETGLPROCMACRO(glCopyTextureSubImage2DEXT)
    GETGLPROCMACRO(glGetTextureImageEXT)
    GETGLPROCMACRO(glGetTextureParameterfvEXT)
    GETGLPROCMACRO(glGetTextureParameterivEXT)
    GETGLPROCMACRO(glGetTextureLevelParameterfvEXT)
    GETGLPROCMACRO(glGetTextureLevelParameterivEXT)
    GETGLPROCMACRO(glTextureImage3DEXT)
    GETGLPROCMACRO(glTextureSubImage3DEXT)
    GETGLPROCMACRO(glCopyTextureSubImage3DEXT)
    GETGLPROCMACRO(glBindMultiTextureEXT)
    GETGLPROCMACRO(glMultiTexCoordPointerEXT)
    GETGLPROCMACRO(glMultiTexEnvfEXT)
    GETGLPROCMACRO(glMultiTexEnvfvEXT)
    GETGLPROCMACRO(glMultiTexEnviEXT)
    GETGLPROCMACRO(glMultiTexEnvivEXT)
    GETGLPROCMACRO(glMultiTexGendEXT)
    GETGLPROCMACRO(glMultiTexGendvEXT)
    GETGLPROCMACRO(glMultiTexGenfEXT)
    GETGLPROCMACRO(glMultiTexGenfvEXT)
    GETGLPROCMACRO(glMultiTexGeniEXT)
    GETGLPROCMACRO(glMultiTexGenivEXT)
    GETGLPROCMACRO(glGetMultiTexEnvfvEXT)
    GETGLPROCMACRO(glGetMultiTexEnvivEXT)
    GETGLPROCMACRO(glGetMultiTexGendvEXT)
    GETGLPROCMACRO(glGetMultiTexGenfvEXT)
    GETGLPROCMACRO(glGetMultiTexGenivEXT)
    GETGLPROCMACRO(glMultiTexParameteriEXT)
    GETGLPROCMACRO(glMultiTexParameterivEXT)
    GETGLPROCMACRO(glMultiTexParameterfEXT)
    GETGLPROCMACRO(glMultiTexParameterfvEXT)
    GETGLPROCMACRO(glMultiTexImage1DEXT)
    GETGLPROCMACRO(glMultiTexImage2DEXT)
    GETGLPROCMACRO(glMultiTexSubImage1DEXT)
    GETGLPROCMACRO(glMultiTexSubImage2DEXT)
    GETGLPROCMACRO(glCopyMultiTexImage1DEXT)
    GETGLPROCMACRO(glCopyMultiTexImage2DEXT)
    GETGLPROCMACRO(glCopyMultiTexSubImage1DEXT)
    GETGLPROCMACRO(glCopyMultiTexSubImage2DEXT)
    GETGLPROCMACRO(glGetMultiTexImageEXT)
    GETGLPROCMACRO(glGetMultiTexParameterfvEXT)
    GETGLPROCMACRO(glGetMultiTexParameterivEXT)
    GETGLPROCMACRO(glGetMultiTexLevelParameterfvEXT)
    GETGLPROCMACRO(glGetMultiTexLevelParameterivEXT)
    GETGLPROCMACRO(glMultiTexImage3DEXT)
    GETGLPROCMACRO(glMultiTexSubImage3DEXT)
    GETGLPROCMACRO(glCopyMultiTexSubImage3DEXT)
    GETGLPROCMACRO(glEnableClientStateIndexedEXT)
    GETGLPROCMACRO(glDisableClientStateIndexedEXT)
    GETGLPROCMACRO(glGetFloatIndexedvEXT)
    GETGLPROCMACRO(glGetDoubleIndexedvEXT)
    GETGLPROCMACRO(glGetPointerIndexedvEXT)
    GETGLPROCMACRO(glEnableIndexedEXT)
    GETGLPROCMACRO(glDisableIndexedEXT)
    GETGLPROCMACRO(glIsEnabledIndexedEXT)
    GETGLPROCMACRO(glGetIntegerIndexedvEXT)
    GETGLPROCMACRO(glGetBooleanIndexedvEXT)
    GETGLPROCMACRO(glCompressedTextureImage3DEXT)
    GETGLPROCMACRO(glCompressedTextureImage2DEXT)
    GETGLPROCMACRO(glCompressedTextureImage1DEXT)
    GETGLPROCMACRO(glCompressedTextureSubImage3DEXT)
    GETGLPROCMACRO(glCompressedTextureSubImage2DEXT)
    GETGLPROCMACRO(glCompressedTextureSubImage1DEXT)
    GETGLPROCMACRO(glGetCompressedTextureImageEXT)
    GETGLPROCMACRO(glCompressedMultiTexImage3DEXT)
    GETGLPROCMACRO(glCompressedMultiTexImage2DEXT)
    GETGLPROCMACRO(glCompressedMultiTexImage1DEXT)
    GETGLPROCMACRO(glCompressedMultiTexSubImage3DEXT)
    GETGLPROCMACRO(glCompressedMultiTexSubImage2DEXT)
    GETGLPROCMACRO(glCompressedMultiTexSubImage1DEXT)
    GETGLPROCMACRO(glGetCompressedMultiTexImageEXT)
    GETGLPROCMACRO(glMatrixLoadTransposefEXT)
    GETGLPROCMACRO(glMatrixLoadTransposedEXT)
    GETGLPROCMACRO(glMatrixMultTransposefEXT)
    GETGLPROCMACRO(glMatrixMultTransposedEXT)
    GETGLPROCMACRO(glNamedBufferDataEXT)
    GETGLPROCMACRO(glNamedBufferSubDataEXT)
    GETGLPROCMACRO(glMapNamedBufferEXT)
    GETGLPROCMACRO(glUnmapNamedBufferEXT)
    GETGLPROCMACRO(glGetNamedBufferParameterivEXT)
    GETGLPROCMACRO(glGetNamedBufferPointervEXT)
    GETGLPROCMACRO(glGetNamedBufferSubDataEXT)
    GETGLPROCMACRO(glProgramUniform1fEXT)
    GETGLPROCMACRO(glProgramUniform2fEXT)
    GETGLPROCMACRO(glProgramUniform3fEXT)
    GETGLPROCMACRO(glProgramUniform4fEXT)
    GETGLPROCMACRO(glProgramUniform1iEXT)
    GETGLPROCMACRO(glProgramUniform2iEXT)
    GETGLPROCMACRO(glProgramUniform3iEXT)
    GETGLPROCMACRO(glProgramUniform4iEXT)
    GETGLPROCMACRO(glProgramUniform1fvEXT)
    GETGLPROCMACRO(glProgramUniform2fvEXT)
    GETGLPROCMACRO(glProgramUniform3fvEXT)
    GETGLPROCMACRO(glProgramUniform4fvEXT)
    GETGLPROCMACRO(glProgramUniform1ivEXT)
    GETGLPROCMACRO(glProgramUniform2ivEXT)
    GETGLPROCMACRO(glProgramUniform3ivEXT)
    GETGLPROCMACRO(glProgramUniform4ivEXT)
    GETGLPROCMACRO(glProgramUniformMatrix2fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix3fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix4fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix2x3fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix3x2fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix2x4fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix4x2fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix3x4fvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix4x3fvEXT)
    GETGLPROCMACRO(glTextureBufferEXT)
    GETGLPROCMACRO(glMultiTexBufferEXT)
    GETGLPROCMACRO(glTextureParameterIivEXT)
    GETGLPROCMACRO(glTextureParameterIuivEXT)
    GETGLPROCMACRO(glGetTextureParameterIivEXT)
    GETGLPROCMACRO(glGetTextureParameterIuivEXT)
    GETGLPROCMACRO(glMultiTexParameterIivEXT)
    GETGLPROCMACRO(glMultiTexParameterIuivEXT)
    GETGLPROCMACRO(glGetMultiTexParameterIivEXT)
    GETGLPROCMACRO(glGetMultiTexParameterIuivEXT)
    GETGLPROCMACRO(glProgramUniform1uiEXT)
    GETGLPROCMACRO(glProgramUniform2uiEXT)
    GETGLPROCMACRO(glProgramUniform3uiEXT)
    GETGLPROCMACRO(glProgramUniform4uiEXT)
    GETGLPROCMACRO(glProgramUniform1uivEXT)
    GETGLPROCMACRO(glProgramUniform2uivEXT)
    GETGLPROCMACRO(glProgramUniform3uivEXT)
    GETGLPROCMACRO(glProgramUniform4uivEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameters4fvEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameterI4iEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameterI4ivEXT)
    GETGLPROCMACRO(glNamedProgramLocalParametersI4ivEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameterI4uiEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameterI4uivEXT)
    GETGLPROCMACRO(glNamedProgramLocalParametersI4uivEXT)
    GETGLPROCMACRO(glGetNamedProgramLocalParameterIivEXT)
    GETGLPROCMACRO(glGetNamedProgramLocalParameterIuivEXT)
    GETGLPROCMACRO(glEnableClientStateiEXT)
    GETGLPROCMACRO(glDisableClientStateiEXT)
    GETGLPROCMACRO(glGetFloati_vEXT)
    GETGLPROCMACRO(glGetDoublei_vEXT)
    GETGLPROCMACRO(glGetPointeri_vEXT)
    GETGLPROCMACRO(glNamedProgramStringEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameter4dEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameter4dvEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameter4fEXT)
    GETGLPROCMACRO(glNamedProgramLocalParameter4fvEXT)
    GETGLPROCMACRO(glGetNamedProgramLocalParameterdvEXT)
    GETGLPROCMACRO(glGetNamedProgramLocalParameterfvEXT)
    GETGLPROCMACRO(glGetNamedProgramivEXT)
    GETGLPROCMACRO(glGetNamedProgramStringEXT)
    GETGLPROCMACRO(glNamedRenderbufferStorageEXT)
    GETGLPROCMACRO(glGetNamedRenderbufferParameterivEXT)
    GETGLPROCMACRO(glNamedRenderbufferStorageMultisampleEXT)
    GETGLPROCMACRO(glNamedRenderbufferStorageMultisampleCoverageEXT)
    GETGLPROCMACRO(glCheckNamedFramebufferStatusEXT)
    GETGLPROCMACRO(glNamedFramebufferTexture1DEXT)
    GETGLPROCMACRO(glNamedFramebufferTexture2DEXT)
    GETGLPROCMACRO(glNamedFramebufferTexture3DEXT)
    GETGLPROCMACRO(glNamedFramebufferRenderbufferEXT)
    GETGLPROCMACRO(glGetNamedFramebufferAttachmentParameterivEXT)
    GETGLPROCMACRO(glGenerateTextureMipmapEXT)
    GETGLPROCMACRO(glGenerateMultiTexMipmapEXT)
    GETGLPROCMACRO(glFramebufferDrawBufferEXT)
    GETGLPROCMACRO(glFramebufferDrawBuffersEXT)
    GETGLPROCMACRO(glFramebufferReadBufferEXT)
    GETGLPROCMACRO(glGetFramebufferParameterivEXT)
    GETGLPROCMACRO(glNamedCopyBufferSubDataEXT)
    GETGLPROCMACRO(glNamedFramebufferTextureEXT)
    GETGLPROCMACRO(glNamedFramebufferTextureLayerEXT)
    GETGLPROCMACRO(glNamedFramebufferTextureFaceEXT)
    GETGLPROCMACRO(glTextureRenderbufferEXT)
    GETGLPROCMACRO(glMultiTexRenderbufferEXT)
    GETGLPROCMACRO(glVertexArrayVertexOffsetEXT)
    GETGLPROCMACRO(glVertexArrayColorOffsetEXT)
    GETGLPROCMACRO(glVertexArrayEdgeFlagOffsetEXT)
    GETGLPROCMACRO(glVertexArrayIndexOffsetEXT)
    GETGLPROCMACRO(glVertexArrayNormalOffsetEXT)
    GETGLPROCMACRO(glVertexArrayTexCoordOffsetEXT)
    GETGLPROCMACRO(glVertexArrayMultiTexCoordOffsetEXT)
    GETGLPROCMACRO(glVertexArrayFogCoordOffsetEXT)
    GETGLPROCMACRO(glVertexArraySecondaryColorOffsetEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribOffsetEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribIOffsetEXT)
    GETGLPROCMACRO(glEnableVertexArrayEXT)
    GETGLPROCMACRO(glDisableVertexArrayEXT)
    GETGLPROCMACRO(glEnableVertexArrayAttribEXT)
    GETGLPROCMACRO(glDisableVertexArrayAttribEXT)
    GETGLPROCMACRO(glGetVertexArrayIntegervEXT)
    GETGLPROCMACRO(glGetVertexArrayPointervEXT)
    GETGLPROCMACRO(glGetVertexArrayIntegeri_vEXT)
    GETGLPROCMACRO(glGetVertexArrayPointeri_vEXT)
    GETGLPROCMACRO(glMapNamedBufferRangeEXT)
    GETGLPROCMACRO(glFlushMappedNamedBufferRangeEXT)
    GETGLPROCMACRO(glNamedBufferStorageEXT)
    GETGLPROCMACRO(glClearNamedBufferDataEXT)
    GETGLPROCMACRO(glClearNamedBufferSubDataEXT)
    GETGLPROCMACRO(glNamedFramebufferParameteriEXT)
    GETGLPROCMACRO(glGetNamedFramebufferParameterivEXT)
    GETGLPROCMACRO(glProgramUniform1dEXT)
    GETGLPROCMACRO(glProgramUniform2dEXT)
    GETGLPROCMACRO(glProgramUniform3dEXT)
    GETGLPROCMACRO(glProgramUniform4dEXT)
    GETGLPROCMACRO(glProgramUniform1dvEXT)
    GETGLPROCMACRO(glProgramUniform2dvEXT)
    GETGLPROCMACRO(glProgramUniform3dvEXT)
    GETGLPROCMACRO(glProgramUniform4dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix2dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix3dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix4dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix2x3dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix2x4dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix3x2dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix3x4dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix4x2dvEXT)
    GETGLPROCMACRO(glProgramUniformMatrix4x3dvEXT)
    GETGLPROCMACRO(glTextureBufferRangeEXT)
    GETGLPROCMACRO(glTextureStorage1DEXT)
    GETGLPROCMACRO(glTextureStorage2DEXT)
    GETGLPROCMACRO(glTextureStorage3DEXT)
    GETGLPROCMACRO(glTextureStorage2DMultisampleEXT)
    GETGLPROCMACRO(glTextureStorage3DMultisampleEXT)
    GETGLPROCMACRO(glVertexArrayBindVertexBufferEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribFormatEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribIFormatEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribLFormatEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribBindingEXT)
    GETGLPROCMACRO(glVertexArrayVertexBindingDivisorEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribLOffsetEXT)
    GETGLPROCMACRO(glTexturePageCommitmentEXT)
    GETGLPROCMACRO(glVertexArrayVertexAttribDivisorEXT)
  }
  #ifdef OS_WIN
  if(r->glEXTlist[354].avaible) {    /// #355 WGL_NV_gpu_affinity http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
    GETGLPROCMACRO(wglEnumGpusNV)
    GETGLPROCMACRO(wglEnumGpuDevicesNV)
    GETGLPROCMACRO(wglCreateAffinityDCNV)
    GETGLPROCMACRO(wglEnumGpusFromAffinityDCNV)
    GETGLPROCMACRO(wglDeleteDCNV)
  }
  #endif
  if(r->glEXTlist[356].avaible) {    /// #357 GL_NV_explicit_multisample http://www.opengl.org/registry/specs/NV/explicit_multisample.txt
    GETGLPROCMACRO(glGetMultisamplefvNV)
    GETGLPROCMACRO(glSampleMaskIndexedNV)
    GETGLPROCMACRO(glTexRenderbufferNV)
  }
  if(r->glEXTlist[357].avaible) {    /// #358 GL_NV_transform_feedback2 http://www.opengl.org/registry/specs/NV/transform_feedback2.txt
    GETGLPROCMACRO(glBindTransformFeedbackNV)
    GETGLPROCMACRO(glDeleteTransformFeedbacksNV)
    GETGLPROCMACRO(glGenTransformFeedbacksNV)
    GETGLPROCMACRO(glIsTransformFeedbackNV)
    GETGLPROCMACRO(glPauseTransformFeedbackNV)
    GETGLPROCMACRO(glResumeTransformFeedbackNV)
    GETGLPROCMACRO(glDrawTransformFeedbackNV)
  }
  if(r->glEXTlist[359].avaible) {    /// #360 GL_AMD_performance_monitor http://www.opengl.org/registry/specs/AMD/performance_monitor.txt
    GETGLPROCMACRO(glGetPerfMonitorGroupsAMD)
    GETGLPROCMACRO(glGetPerfMonitorCountersAMD)
    GETGLPROCMACRO(glGetPerfMonitorGroupStringAMD)
    GETGLPROCMACRO(glGetPerfMonitorCounterStringAMD)
    GETGLPROCMACRO(glGetPerfMonitorCounterInfoAMD)
    GETGLPROCMACRO(glGenPerfMonitorsAMD)
    GETGLPROCMACRO(glDeletePerfMonitorsAMD)
    GETGLPROCMACRO(glSelectPerfMonitorCountersAMD)
    GETGLPROCMACRO(glBeginPerfMonitorAMD)
    GETGLPROCMACRO(glEndPerfMonitorAMD)
    GETGLPROCMACRO(glGetPerfMonitorCounterDataAMD)
  }
  #ifdef OS_WIN
  if(r->glEXTlist[360].avaible) {    /// #361 WGL_AMD_gpu_association http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
    GETGLPROCMACRO(wglGetGPUIDsAMD)
    GETGLPROCMACRO(wglGetGPUInfoAMD)
    GETGLPROCMACRO(wglGetContextGPUIDAMD)
    GETGLPROCMACRO(wglCreateAssociatedContextAMD)
    GETGLPROCMACRO(wglCreateAssociatedContextAttribsAMD)
    GETGLPROCMACRO(wglDeleteAssociatedContextAMD)
    GETGLPROCMACRO(wglMakeAssociatedContextCurrentAMD)
    GETGLPROCMACRO(wglGetCurrentAssociatedContextAMD)
    GETGLPROCMACRO(wglBlitContextFramebufferAMD)
  }
  #endif
  if(r->glEXTlist[362].avaible) {    /// #363 GL_AMD_vertex_shader_tessellator http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt
    GETGLPROCMACRO(glTessellationFactorAMD)
    GETGLPROCMACRO(glTessellationModeAMD)
  }
  if(r->glEXTlist[363].avaible) {    /// #364 GL_EXT_provoking_vertex http://www.opengl.org/registry/specs/EXT/provoking_vertex.txt
    GETGLPROCMACRO(glProvokingVertexEXT)
  }
  if(r->glEXTlist[365].avaible) {    /// #366 GL_AMD_draw_buffers_blend http://www.opengl.org/registry/specs/AMD/draw_buffers_blend.txt
    GETGLPROCMACRO(glBlendFuncIndexedAMD)
    GETGLPROCMACRO(glBlendFuncSeparateIndexedAMD)
    GETGLPROCMACRO(glBlendEquationIndexedAMD)
    GETGLPROCMACRO(glBlendEquationSeparateIndexedAMD)
  }
  if(r->glEXTlist[366].avaible) {    /// #367 GL_APPLE_texture_range http://www.opengl.org/registry/specs/APPLE/texture_range.txt
    GETGLPROCMACRO(glTextureRangeAPPLE)
    GETGLPROCMACRO(glGetTexParameterPointervAPPLE)
  }
  if(r->glEXTlist[368].avaible) {    /// #369 GL_APPLE_vertex_program_evaluators http://www.opengl.org/registry/specs/APPLE/vertex_program_evaluators.txt
    GETGLPROCMACRO(glEnableVertexAttribAPPLE)
    GETGLPROCMACRO(glDisableVertexAttribAPPLE)
    GETGLPROCMACRO(glIsVertexAttribEnabledAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib1dAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib1fAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib2dAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib2fAPPLE)
  }
  if(r->glEXTlist[370].avaible) {    /// #371 GL_APPLE_object_purgeable http://www.opengl.org/registry/specs/APPLE/object_purgeable.txt
    GETGLPROCMACRO(glObjectPurgeableAPPLE)
    GETGLPROCMACRO(glObjectUnpurgeableAPPLE)
    GETGLPROCMACRO(glGetObjectParameterivAPPLE)
  }
  if(r->glEXTlist[373].avaible) {    /// #374 GL_NV_video_capture GLX_NV_video_capture WGL_NV_video_capture http://www.opengl.org/registry/specs/NV/video_capture.txt
    GETGLPROCMACRO(glBeginVideoCaptureNV)
    GETGLPROCMACRO(glBindVideoCaptureStreamBufferNV)
    GETGLPROCMACRO(glBindVideoCaptureStreamTextureNV)
    GETGLPROCMACRO(glEndVideoCaptureNV)
    GETGLPROCMACRO(glGetVideoCaptureivNV)
    GETGLPROCMACRO(glGetVideoCaptureStreamivNV)
    GETGLPROCMACRO(glGetVideoCaptureStreamfvNV)
    GETGLPROCMACRO(glGetVideoCaptureStreamdvNV)
    GETGLPROCMACRO(glVideoCaptureNV)
    GETGLPROCMACRO(glVideoCaptureStreamParameterivNV)
    GETGLPROCMACRO(glVideoCaptureStreamParameterfvNV)
    GETGLPROCMACRO(glVideoCaptureStreamParameterdvNV)
    #ifdef OS_LINUX
    GETGLPROCMACRO(glXBindVideoCaptureDeviceNV)
    GETGLPROCMACRO(glXEnumerateVideoCaptureDevicesNV)
    GETGLPROCMACRO(glXLockVideoCaptureDeviceNV)
    GETGLPROCMACRO(glXQueryVideoCaptureDeviceNV)
    GETGLPROCMACRO(glXReleaseVideoCaptureDeviceNV)
    #endif /// OS_LINUX
    #ifdef OS_WIN
    GETGLPROCMACRO(wglBindVideoCaptureDeviceNV)
    GETGLPROCMACRO(wglEnumerateVideoCaptureDevicesNV)
    GETGLPROCMACRO(wglLockVideoCaptureDeviceNV)
    GETGLPROCMACRO(wglQueryVideoCaptureDeviceNV)
    GETGLPROCMACRO(wglReleaseVideoCaptureDeviceNV)
    #endif
  }
  #ifdef OS_LINUX
  if(r->glEXTlist[374].avaible) {    /// #375 GLX_EXT_swap_control http://www.opengl.org/registry/specs/EXT/swap_control.txt
    GETGLPROCMACRO(glXSwapIntervalEXT)
  }
  #endif
  if(r->glEXTlist[375].avaible) {    /// #376 GL_NV_copy_image WGL_NV_copy_image GLX_NV_copy_image http://www.opengl.org/registry/specs/NV/copy_image.txt
    GETGLPROCMACRO(glCopyImageSubDataNV)
    #ifdef OS_LINUX
    GETGLPROCMACRO(glXCopyImageSubDataNV)
    #endif
    #ifdef OS_WIN
    GETGLPROCMACRO(wglCopyImageSubDataNV)
    #endif
  }
  if(r->glEXTlist[376].avaible) {    /// #377 GL_EXT_separate_shader_objects http://www.opengl.org/registry/specs/EXT/separate_shader_objects.txt
    GETGLPROCMACRO(glUseShaderProgramEXT)
    GETGLPROCMACRO(glActiveProgramEXT)
    GETGLPROCMACRO(glCreateShaderProgramEXT)
  }
  if(r->glEXTlist[378].avaible) {    /// #379 GL_NV_shader_buffer_load http://www.opengl.org/registry/specs/NV/shader_buffer_load.txt
    GETGLPROCMACRO(glMakeBufferResidentNV)
    GETGLPROCMACRO(glMakeBufferNonResidentNV)
    GETGLPROCMACRO(glIsBufferResidentNV)
    GETGLPROCMACRO(glMakeNamedBufferResidentNV)
    GETGLPROCMACRO(glMakeNamedBufferNonResidentNV)
    GETGLPROCMACRO(glIsNamedBufferResidentNV)
    GETGLPROCMACRO(glGetBufferParameterui64vNV)
    GETGLPROCMACRO(glGetNamedBufferParameterui64vNV)
    GETGLPROCMACRO(glGetIntegerui64vNV)
    GETGLPROCMACRO(glUniformui64NV)
    GETGLPROCMACRO(glUniformui64vNV)
    GETGLPROCMACRO(glProgramUniformui64NV)
    GETGLPROCMACRO(glProgramUniformui64vNV)
  }
  if(r->glEXTlist[379].avaible) {    /// #380 GL_NV_vertex_buffer_unified_memory http://www.opengl.org/registry/specs/NV/vertex_buffer_unified_memory.txt
    GETGLPROCMACRO(glBufferAddressRangeNV)
    GETGLPROCMACRO(glVertexFormatNV)
    GETGLPROCMACRO(glNormalFormatNV)
    GETGLPROCMACRO(glColorFormatNV)
    GETGLPROCMACRO(glIndexFormatNV)
    GETGLPROCMACRO(glTexCoordFormatNV)
    GETGLPROCMACRO(glEdgeFlagFormatNV)
    GETGLPROCMACRO(glSecondaryColorFormatNV)
    GETGLPROCMACRO(glFogCoordFormatNV)
    GETGLPROCMACRO(glVertexAttribFormatNV)
    GETGLPROCMACRO(glVertexAttribIFormatNV)
    GETGLPROCMACRO(glGetIntegerui64i_vNV)
  }
  if(r->glEXTlist[380].avaible) {    /// #381 GL_NV_texture_barrier http://www.opengl.org/registry/specs/NV/texture_barrier.txt
    GETGLPROCMACRO(glTextureBarrierNV)
  }
  if(r->glEXTlist[385].avaible) {    /// #386 GL_EXT_shader_image_load_store http://www.opengl.org/registry/specs/EXT/shader_image_load_store.txt
    GETGLPROCMACRO(glBindImageTextureEXT)
    GETGLPROCMACRO(glMemoryBarrierEXT)
  }
  if(r->glEXTlist[386].avaible) {    /// #387 GL_EXT_vertex_attrib_64bit http://www.opengl.org/registry/specs/EXT/vertex_attrib_64bit.txt
    GETGLPROCMACRO(glVertexAttribL1dEXT)
    GETGLPROCMACRO(glVertexAttribL2dEXT)
    GETGLPROCMACRO(glVertexAttribL3dEXT)
    GETGLPROCMACRO(glVertexAttribL4dEXT)
    GETGLPROCMACRO(glVertexAttribL1dvEXT)
    GETGLPROCMACRO(glVertexAttribL2dvEXT)
    GETGLPROCMACRO(glVertexAttribL3dvEXT)
    GETGLPROCMACRO(glVertexAttribL4dvEXT)
    GETGLPROCMACRO(glVertexAttribLPointerEXT)
    GETGLPROCMACRO(glGetVertexAttribLdvEXT)
  }
  if(r->glEXTlist[387].avaible) {    /// #388 GL_NV_gpu_program5 GL_NV_gpu_program_fp64 http://www.opengl.org/registry/specs/NV/gpu_program5.txt
    GETGLPROCMACRO(glProgramSubroutineParametersuivNV)
    GETGLPROCMACRO(glGetProgramSubroutineParameteruivNV)
  }
  if(r->glEXTlist[391].avaible) {    /// #392 GL_NV_vertex_attrib_integer_64bit http://www.opengl.org/registry/specs/NV/vertex_attrib_integer_64bit.txt
    GETGLPROCMACRO(glVertexAttribL1i64NV)
    GETGLPROCMACRO(glVertexAttribL2i64NV)
    GETGLPROCMACRO(glVertexAttribL3i64NV)
    GETGLPROCMACRO(glVertexAttribL4i64NV)
    GETGLPROCMACRO(glVertexAttribL1i64vNV)
    GETGLPROCMACRO(glVertexAttribL2i64vNV)
    GETGLPROCMACRO(glVertexAttribL3i64vNV)
    GETGLPROCMACRO(glVertexAttribL4i64vNV)
    GETGLPROCMACRO(glVertexAttribL1ui64NV)
    GETGLPROCMACRO(glVertexAttribL2ui64NV)
    GETGLPROCMACRO(glVertexAttribL3ui64NV)
    GETGLPROCMACRO(glVertexAttribL4ui64NV)
    GETGLPROCMACRO(glVertexAttribL1ui64vNV)
    GETGLPROCMACRO(glVertexAttribL2ui64vNV)
    GETGLPROCMACRO(glVertexAttribL3ui64vNV)
    GETGLPROCMACRO(glVertexAttribL4ui64vNV)
    GETGLPROCMACRO(glGetVertexAttribLi64vNV)
    GETGLPROCMACRO(glGetVertexAttribLui64vNV)
    GETGLPROCMACRO(glVertexAttribLFormatNV)
  }
  if(r->glEXTlist[393].avaible) {    /// #394 GL_AMD_name_gen_delete http://www.opengl.org/registry/specs/AMD/name_gen_delete.txt
    GETGLPROCMACRO(glGenNamesAMD)
    GETGLPROCMACRO(glDeleteNamesAMD)
    GETGLPROCMACRO(glIsNameAMD)
  }
  if(r->glEXTlist[394].avaible) {    /// #395 GL_AMD_debug_output http://www.opengl.org/registry/specs/AMD/debug_output.txt
    GETGLPROCMACRO(glDebugMessageEnableAMD)
    GETGLPROCMACRO(glDebugMessageInsertAMD)
    GETGLPROCMACRO(glDebugMessageCallbackAMD)
    GETGLPROCMACRO(glGetDebugMessageLogAMD)
  }
  if(r->glEXTlist[395].avaible) {    /// #396 GL_NV_vdpau_interop http://www.opengl.org/registry/specs/NV/vdpau_interop.txt
    GETGLPROCMACRO(glVDPAUInitNV)
    GETGLPROCMACRO(glVDPAUFiniNV)
    GETGLPROCMACRO(glVDPAURegisterVideoSurfaceNV)
    GETGLPROCMACRO(glVDPAURegisterOutputSurfaceNV)
    GETGLPROCMACRO(glVDPAUIsSurfaceNV)
    GETGLPROCMACRO(glVDPAUUnregisterSurfaceNV)
    GETGLPROCMACRO(glVDPAUGetSurfaceivNV)
    GETGLPROCMACRO(glVDPAUSurfaceAccessNV)
    GETGLPROCMACRO(glVDPAUMapSurfacesNV)
    GETGLPROCMACRO(glVDPAUUnmapSurfacesNV)
  }
  if(r->glEXTlist[402].avaible) {    /// #403 GL_NV_texture_multisample GL_ missing in registry, asuming mistake http://www.opengl.org/registry/specs/NV/texture_multisample.txt
    GETGLPROCMACRO(glTexImage2DMultisampleCoverageNV)
    GETGLPROCMACRO(glTexImage3DMultisampleCoverageNV)
    GETGLPROCMACRO(glTextureImage2DMultisampleNV)
    GETGLPROCMACRO(glTextureImage3DMultisampleNV)
    GETGLPROCMACRO(glTextureImage2DMultisampleCoverageNV)
    GETGLPROCMACRO(glTextureImage3DMultisampleCoverageNV)
  }
  if(r->glEXTlist[404].avaible) {    /// #405 GL_AMD_sample_positions http://www.opengl.org/registry/specs/AMD/sample_positions.txt
    GETGLPROCMACRO(glSetMultisamplefvAMD)
  }
  if(r->glEXTlist[405].avaible) {    /// #406 GL_EXT_x11_sync_object http://www.opengl.org/registry/specs/EXT/x11_sync_object.txt
    GETGLPROCMACRO(glImportSyncEXT)
  }
  #ifdef OS_WIN
  if(r->glEXTlist[406].avaible) {    /// #407 WGL_NV_DX_interop http://www.opengl.org/registry/specs/NV/DX_interop.txt
    GETGLPROCMACRO(wglDXSetResourceShareHandleNV)
    GETGLPROCMACRO(wglDXOpenDeviceNV)
    GETGLPROCMACRO(wglDXCloseDeviceNV)
    GETGLPROCMACRO(wglDXRegisterObjectNV)
    GETGLPROCMACRO(wglDXUnregisterObjectNV)
    GETGLPROCMACRO(wglDXObjectAccessNV)
    GETGLPROCMACRO(wglDXLockObjectsNV)
    GETGLPROCMACRO(wglDXUnlockObjectsNV)
  }
  #endif
  if(r->glEXTlist[407].avaible) {    /// #408 GL_AMD_multi_draw_indirect http://www.opengl.org/registry/specs/AMD/multi_draw_indirect.txt
    GETGLPROCMACRO(glMultiDrawArraysIndirectAMD)
    GETGLPROCMACRO(glMultiDrawElementsIndirectAMD)
  }
  if(r->glEXTlist[409].avaible) {    /// #410 GL_NV_path_rendering http://www.opengl.org/registry/specs/NV/path_rendering.txt
    GETGLPROCMACRO(glGenPathsNV)
    GETGLPROCMACRO(glDeletePathsNV)
    GETGLPROCMACRO(glIsPathNV)
    GETGLPROCMACRO(glPathCommandsNV)
    GETGLPROCMACRO(glPathCoordsNV)
    GETGLPROCMACRO(glPathSubCommandsNV)
    GETGLPROCMACRO(glPathSubCoordsNV)
    GETGLPROCMACRO(glPathStringNV)
    GETGLPROCMACRO(glPathGlyphsNV)
    GETGLPROCMACRO(glPathGlyphRangeNV)
    GETGLPROCMACRO(glWeightPathsNV)
    GETGLPROCMACRO(glCopyPathNV)
    GETGLPROCMACRO(glInterpolatePathsNV)
    GETGLPROCMACRO(glTransformPathNV)
    GETGLPROCMACRO(glPathParameterivNV)
    GETGLPROCMACRO(glPathParameteriNV)
    GETGLPROCMACRO(glPathParameterfvNV)
    GETGLPROCMACRO(glPathParameterfNV)
    GETGLPROCMACRO(glPathDashArrayNV)
    GETGLPROCMACRO(glPathStencilFuncNV)
    GETGLPROCMACRO(glPathStencilDepthOffsetNV)
    GETGLPROCMACRO(glStencilFillPathNV)
    GETGLPROCMACRO(glStencilStrokePathNV)
    GETGLPROCMACRO(glStencilFillPathInstancedNV)
    GETGLPROCMACRO(glStencilStrokePathInstancedNV)
    GETGLPROCMACRO(glPathCoverDepthFuncNV)
    GETGLPROCMACRO(glPathColorGenNV)
    GETGLPROCMACRO(glPathTexGenNV)
    GETGLPROCMACRO(glPathFogGenNV)
    GETGLPROCMACRO(glCoverFillPathNV)
    GETGLPROCMACRO(glCoverStrokePathNV)
    GETGLPROCMACRO(glCoverFillPathInstancedNV)
    GETGLPROCMACRO(glCoverStrokePathInstancedNV)
    GETGLPROCMACRO(glGetPathParameterivNV)
    GETGLPROCMACRO(glGetPathParameterfvNV)
    GETGLPROCMACRO(glGetPathCommandsNV)
    GETGLPROCMACRO(glGetPathCoordsNV)
    GETGLPROCMACRO(glGetPathDashArrayNV)
    GETGLPROCMACRO(glGetPathMetricsNV)
    GETGLPROCMACRO(glGetPathMetricRangeNV)
    GETGLPROCMACRO(glGetPathSpacingNV)
    GETGLPROCMACRO(glGetPathColorGenivNV)
    GETGLPROCMACRO(glGetPathColorGenfvNV)
    GETGLPROCMACRO(glGetPathTexGenivNV)
    GETGLPROCMACRO(glGetPathTexGenfvNV)
    GETGLPROCMACRO(glIsPointInFillPathNV)
    GETGLPROCMACRO(glIsPointInStrokePathNV)
    GETGLPROCMACRO(glGetPathLengthNV)
    GETGLPROCMACRO(glPointAlongPathNV)
  }
  if(r->glEXTlist[412].avaible) {    /// #413 GL_AMD_stencil_operation_extended http://www.opengl.org/registry/specs/AMD/stencil_operation_extended.txt
    GETGLPROCMACRO(glStencilOpValueAMD)
  }
  if(r->glEXTlist[417].avaible) {    /// #418 GL_NV_bindless_texture http://www.opengl.org/registry/specs/NV/bindless_texture.txt
    GETGLPROCMACRO(glGetTextureHandleNV)
    GETGLPROCMACRO(glGetTextureSamplerHandleNV)
    GETGLPROCMACRO(glMakeTextureHandleResidentNV)
    GETGLPROCMACRO(glMakeTextureHandleNonResidentNV)
    GETGLPROCMACRO(glGetImageHandleNV)
    GETGLPROCMACRO(glMakeImageHandleResidentNV)
    GETGLPROCMACRO(glMakeImageHandleNonResidentNV)
    GETGLPROCMACRO(glUniformHandleui64NV)
    GETGLPROCMACRO(glUniformHandleui64vNV)
    GETGLPROCMACRO(glProgramUniformHandleui64NV)
    GETGLPROCMACRO(glProgramUniformHandleui64vNV)
    GETGLPROCMACRO(glIsTextureHandleResidentNV)
    GETGLPROCMACRO(glIsImageHandleResidentNV)
  }
  if(r->glEXTlist[424].avaible) {    /// #425 GL_NVX_conditional_render http://www.opengl.org/registry/specs/NVX/nvx_conditional_render.txt
    GETGLPROCMACRO(glBeginConditionalRenderNVX)
    GETGLPROCMACRO(glEndConditionalRenderNVX)
  }
  if(r->glEXTlist[425].avaible) {    /// #426 GL_AMD_sparse_texture http://www.opengl.org/registry/specs/AMD/sparse_texture.txt
    GETGLPROCMACRO(glTexStorageSparseAMD)
    GETGLPROCMACRO(glTextureStorageSparseAMD)
  }
  if(r->glEXTlist[428].avaible) {    /// #429 GL_INTEL_map_texture http://www.opengl.org/registry/specs/INTEL/map_texture.txt
    GETGLPROCMACRO(glSyncTextureINTEL)
    GETGLPROCMACRO(glUnmapTexture2DINTEL)
    GETGLPROCMACRO(glMapTexture2DINTEL)
  }
  if(r->glEXTlist[429].avaible) {    /// #430 GL_NV_draw_texture http://www.opengl.org/registry/specs/NV/draw_texture.txt
    GETGLPROCMACRO(glDrawTextureNV)
  }
  if(r->glEXTlist[430].avaible) {    /// #431 GL_AMD_interleaved_elements http://www.opengl.org/registry/specs/AMD/interleaved_elements.txt
    GETGLPROCMACRO(glVertexAttribParameteriAMD)
  }
  if(r->glEXTlist[431].avaible) {    /// #432 GL_NV_bindless_multi_draw_indirect http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect.txt
    GETGLPROCMACRO(glMultiDrawArraysIndirectBindlessNV)
    GETGLPROCMACRO(glMultiDrawElementsIndirectBindlessNV)
  }
  if(r->glEXTlist[432].avaible) {    /// #433 GL_NV_blend_equation_advanced GL_NV_blend_equation_advanced_coherent http://www.opengl.org/registry/specs/NV/blend_equation_advanced.txt
    GETGLPROCMACRO(glBlendParameteriNV)
    GETGLPROCMACRO(glBlendBarrierNV)
  
  }
  #ifdef OS_WIN
  if(r->glEXTlist[435].avaible) {    /// #436 WGL_NV_delay_before_swap http://www.opengl.org/registry/specs/NV/wgl_delay_before_swap.txt
    GETGLPROCMACRO(wglDelayBeforeSwapNV)
  }
  #endif
  if(r->glEXTlist[438].avaible) {    /// #439 GL_EXT_debug_label http://www.opengl.org/registry/specs/EXT/EXT_debug_label.txt
    GETGLPROCMACRO(glLabelObjectEXT)
    GETGLPROCMACRO(glGetObjectLabelEXT)
  }
  if(r->glEXTlist[439].avaible) {    /// #440 GL_EXT_debug_marker http://www.opengl.org/registry/specs/EXT/EXT_debug_marker.txt
    GETGLPROCMACRO(glInsertEventMarkerEXT)
    GETGLPROCMACRO(glPushGroupMarkerEXT)
    GETGLPROCMACRO(glPopGroupMarkerEXT)
  }
  if(r->glEXTlist[441].avaible) {    /// #442 GL_AMD_occlusion_query_event http://www.opengl.org/registry/specs/AMD/occlusion_query_event.txt
    GETGLPROCMACRO(glQueryObjectParameteruiAMD)
  }
  if(r->glEXTlist[442].avaible) {    /// #443 GL_INTEL_performance_query http://www.opengl.org/registry/specs/INTEL/performance_query.txt
    GETGLPROCMACRO(glBeginPerfQueryINTEL)
    GETGLPROCMACRO(glCreatePerfQueryINTEL)
    GETGLPROCMACRO(glDeletePerfQueryINTEL)
    GETGLPROCMACRO(glEndPerfQueryINTEL)
    GETGLPROCMACRO(glGetFirstPerfQueryIdINTEL)
    GETGLPROCMACRO(glGetNextPerfQueryIdINTEL)
    GETGLPROCMACRO(glGetPerfCounterInfoINTEL)
    GETGLPROCMACRO(glGetPerfQueryDataINTEL)
    GETGLPROCMACRO(glGetPerfQueryIdByNameINTEL)
    GETGLPROCMACRO(glGetPerfQueryInfoINTEL)
  }
  if(r->glEXTlist[450].avaible) {    /// #451 GL_AMD_gpu_shader_int64 http://www.opengl.org/registry/specs/AMD/gpu_shader_int64.txt
    GETGLPROCMACRO(glUniform1i64NV)
    GETGLPROCMACRO(glUniform2i64NV)
    GETGLPROCMACRO(glUniform3i64NV)
    GETGLPROCMACRO(glUniform4i64NV)
    GETGLPROCMACRO(glUniform1i64vNV)
    GETGLPROCMACRO(glUniform2i64vNV)
    GETGLPROCMACRO(glUniform3i64vNV)
    GETGLPROCMACRO(glUniform4i64vNV)
    GETGLPROCMACRO(glUniform1ui64NV)
    GETGLPROCMACRO(glUniform2ui64NV)
    GETGLPROCMACRO(glUniform3ui64NV)
    GETGLPROCMACRO(glUniform4ui64NV)
    GETGLPROCMACRO(glUniform1ui64vNV)
    GETGLPROCMACRO(glUniform2ui64vNV)
    GETGLPROCMACRO(glUniform3ui64vNV)
    GETGLPROCMACRO(glUniform4ui64vNV)
    GETGLPROCMACRO(glGetUniformi64vNV)
    GETGLPROCMACRO(glGetUniformui64vNV)
    GETGLPROCMACRO(glProgramUniform1i64NV)
    GETGLPROCMACRO(glProgramUniform2i64NV)
    GETGLPROCMACRO(glProgramUniform3i64NV)
    GETGLPROCMACRO(glProgramUniform4i64NV)
    GETGLPROCMACRO(glProgramUniform1i64vNV)
    GETGLPROCMACRO(glProgramUniform2i64vNV)
    GETGLPROCMACRO(glProgramUniform3i64vNV)
    GETGLPROCMACRO(glProgramUniform4i64vNV)
    GETGLPROCMACRO(glProgramUniform1ui64NV)
    GETGLPROCMACRO(glProgramUniform2ui64NV)
    GETGLPROCMACRO(glProgramUniform3ui64NV)
    GETGLPROCMACRO(glProgramUniform4ui64NV)
    GETGLPROCMACRO(glProgramUniform1ui64vNV)
    GETGLPROCMACRO(glProgramUniform2ui64vNV)
    GETGLPROCMACRO(glProgramUniform3ui64vNV)
    GETGLPROCMACRO(glProgramUniform4ui64vNV)
  }
}














///==========================///
// OPENGL EXTENSION FUNCTIONS //
///==========================///
/// all inline, each will call current renderer's extension function

// this one is used to get extension strings in openGL 3.x+
//inline GLAPI const GLubyte *APIENTRY glGetStringi (GLenum name, GLuint index) {
//  return osi.glr->glExt.glGetStringi(name, index); }



/// OpenGL 1.2 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawRangeElements (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) {
            osi.glr->glExt.glDrawRangeElements (       mode,        start,        end,         count,        type,             indices); }
inline GLAPI void APIENTRY glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexImage3D (target, level, internalformat, width, height, depth, border, format, type, pixels); }
inline GLAPI void APIENTRY glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexSubImage3D (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels); }
inline GLAPI void APIENTRY glCopyTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyTexSubImage3D (target, level, xoffset, yoffset, zoffset, x, y, width, height); }

/// OpenGL 1.3 funcs =================------------------------------
inline GLAPI void APIENTRY glActiveTexture (GLenum texture) {
  osi.glr->glExt.glActiveTexture (texture); }
inline GLAPI void APIENTRY glSampleCoverage (GLfloat value, GLboolean invert) {
  osi.glr->glExt.glSampleCoverage (value, invert); }
inline GLAPI void APIENTRY glCompressedTexImage3D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexImage3D (target, level, internalformat, width, height, depth, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexImage2D (target, level, internalformat, width, height, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexImage1D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexImage1D (target, level, internalformat, width, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexSubImage3D (target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexSubImage2D (target, level, xoffset, yoffset, width, height, format, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexSubImage1D (target, level, xoffset, width, format, imageSize, data); }
inline GLAPI void APIENTRY glGetCompressedTexImage (GLenum target, GLint level, void *img) {
  osi.glr->glExt.glGetCompressedTexImage (target, level, img); }
#ifdef OS_WIN
inline GLAPI void APIENTRY glClientActiveTexture (GLenum texture) {
  osi.glr->glExt.glClientActiveTexture (texture); }
inline GLAPI void APIENTRY glMultiTexCoord1d (GLenum target, GLdouble s) {
  osi.glr->glExt.glMultiTexCoord1d (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1dv (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord1dv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord1f (GLenum target, GLfloat s) {
  osi.glr->glExt.glMultiTexCoord1f (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1fv (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord1fv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord1i (GLenum target, GLint s) {
  osi.glr->glExt.glMultiTexCoord1i (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1iv (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord1iv (target, v); } 
inline GLAPI void APIENTRY glMultiTexCoord1s (GLenum target, GLshort s) {
  osi.glr->glExt.glMultiTexCoord1s (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1sv (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord1sv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2d (GLenum target, GLdouble s, GLdouble t) {
  osi.glr->glExt.glMultiTexCoord2d (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2dv (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord2dv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2f (GLenum target, GLfloat s, GLfloat t) {
  osi.glr->glExt.glMultiTexCoord2f (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2fv (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord2fv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2i (GLenum target, GLint s, GLint t) {
  osi.glr->glExt.glMultiTexCoord2i (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2iv (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord2iv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2s (GLenum target, GLshort s, GLshort t) {
  osi.glr->glExt.glMultiTexCoord2s (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2sv (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord2sv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord3d (GLenum target, GLdouble s, GLdouble t, GLdouble r) {
  osi.glr->glExt.glMultiTexCoord3d (target, s, t, r); }
inline GLAPI void APIENTRY glMultiTexCoord3dv (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord3dv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord3f (GLenum target, GLfloat s, GLfloat t, GLfloat r) {
  osi.glr->glExt.glMultiTexCoord3f (target, s, t, r); }
inline GLAPI void APIENTRY glMultiTexCoord3fv (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord3fv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord3i (GLenum target, GLint s, GLint t, GLint r) {
  osi.glr->glExt.glMultiTexCoord3i (target, s, t, r); }
inline GLAPI void APIENTRY glMultiTexCoord3iv (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord3iv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3s (GLenum target, GLshort s, GLshort t, GLshort r) {
  osi.glr->glExt.glMultiTexCoord3s (target,  s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3sv (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord3sv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord4d (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q) {
  osi.glr->glExt.glMultiTexCoord4d (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4dv (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord4dv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
  osi.glr->glExt.glMultiTexCoord4f (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4fv (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord4fv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4i (GLenum target, GLint s, GLint t, GLint r, GLint q) {
  osi.glr->glExt.glMultiTexCoord4i (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4iv (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord4iv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4s (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q) {
  osi.glr->glExt.glMultiTexCoord4s (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4sv (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord4sv (target, v);}
inline GLAPI void APIENTRY glLoadTransposeMatrixf (const GLfloat *m) {
  osi.glr->glExt.glLoadTransposeMatrixf (m);}
inline GLAPI void APIENTRY glLoadTransposeMatrixd (const GLdouble *m) {
  osi.glr->glExt.glLoadTransposeMatrixd (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixf (const GLfloat *m) {
  osi.glr->glExt.glMultTransposeMatrixf (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixd (const GLdouble *m) {
  osi.glr->glExt.glMultTransposeMatrixd (m);}
#endif /// OS_WIN

/// OpenGL 1.4 funcs =================------------------------------
inline GLAPI void APIENTRY glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
  osi.glr->glExt.glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);}
inline GLAPI void APIENTRY glMultiDrawArrays (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) {
  osi.glr->glExt.glMultiDrawArrays (mode, first, count, drawcount);}
inline GLAPI void APIENTRY glMultiDrawElements (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount) {
  osi.glr->glExt.glMultiDrawElements (mode, count, type, indices, drawcount);}
inline GLAPI void APIENTRY glPointParameterf (GLenum pname, GLfloat param) {
  osi.glr->glExt.glPointParameterf (pname, param);}
inline GLAPI void APIENTRY glPointParameterfv (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glPointParameterfv (pname, params);}
inline GLAPI void APIENTRY glPointParameteri (GLenum pname, GLint param) {
  osi.glr->glExt.glPointParameteri (pname, param);}
inline GLAPI void APIENTRY glPointParameteriv (GLenum pname, const GLint *params) {
  osi.glr->glExt.glPointParameteriv (pname, params);}
inline GLAPI void APIENTRY glFogCoordf (GLfloat coord) {
  osi.glr->glExt.glFogCoordf (coord);}
inline GLAPI void APIENTRY glFogCoordfv (const GLfloat *coord) {
  osi.glr->glExt.glFogCoordfv (coord);}
inline GLAPI void APIENTRY glFogCoordd (GLdouble coord) {
  osi.glr->glExt.glFogCoordd (coord);}
inline GLAPI void APIENTRY glFogCoorddv (const GLdouble *coord) {
  osi.glr->glExt.glFogCoorddv (coord);}
inline GLAPI void APIENTRY glFogCoordPointer (GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glFogCoordPointer (type, stride, pointer);}
inline GLAPI void APIENTRY glSecondaryColor3b (GLbyte red, GLbyte green, GLbyte blue) {
  osi.glr->glExt.glSecondaryColor3b (red, green,  blue);}
inline GLAPI void APIENTRY glSecondaryColor3bv (const GLbyte *v) {
  osi.glr->glExt.glSecondaryColor3bv (v);}
inline GLAPI void APIENTRY glSecondaryColor3d (GLdouble red, GLdouble green, GLdouble blue) {
  osi.glr->glExt.glSecondaryColor3d (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3dv (const GLdouble *v) {
  osi.glr->glExt.glSecondaryColor3dv (v);}
inline GLAPI void APIENTRY glSecondaryColor3f (GLfloat red, GLfloat green, GLfloat blue) {
  osi.glr->glExt.glSecondaryColor3f (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3fv (const GLfloat *v) {
  osi.glr->glExt.glSecondaryColor3fv (v);}
inline GLAPI void APIENTRY glSecondaryColor3i (GLint red, GLint green, GLint blue) {
  osi.glr->glExt.glSecondaryColor3i (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3iv (const GLint *v) {
  osi.glr->glExt.glSecondaryColor3iv (v);}
inline GLAPI void APIENTRY glSecondaryColor3s (GLshort red, GLshort green, GLshort blue) {
  osi.glr->glExt.glSecondaryColor3s (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3sv (const GLshort *v) {
  osi.glr->glExt.glSecondaryColor3sv (v);}
inline GLAPI void APIENTRY glSecondaryColor3ub (GLubyte red, GLubyte green, GLubyte blue) {
  osi.glr->glExt.glSecondaryColor3ub (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3ubv (const GLubyte *v) {
  osi.glr->glExt.glSecondaryColor3ubv (v);}
inline GLAPI void APIENTRY glSecondaryColor3ui (GLuint red, GLuint green, GLuint blue) {
  osi.glr->glExt.glSecondaryColor3ui (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3uiv (const GLuint *v) {
  osi.glr->glExt.glSecondaryColor3uiv (v);}
inline GLAPI void APIENTRY glSecondaryColor3us (GLushort red, GLushort green, GLushort blue) {
  osi.glr->glExt.glSecondaryColor3us (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3usv (const GLushort *v) {
  osi.glr->glExt.glSecondaryColor3usv (v);}
inline GLAPI void APIENTRY glSecondaryColorPointer (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glSecondaryColorPointer (size, type, stride, pointer);}
inline GLAPI void APIENTRY glWindowPos2d (GLdouble x, GLdouble y) {
  osi.glr->glExt.glWindowPos2d (x, y);}
inline GLAPI void APIENTRY glWindowPos2dv (const GLdouble *v) {
  osi.glr->glExt.glWindowPos2dv (v);}
inline GLAPI void APIENTRY glWindowPos2f (GLfloat x, GLfloat y) {
  osi.glr->glExt.glWindowPos2f (x, y); }
inline GLAPI void APIENTRY glWindowPos2fv (const GLfloat *v) {
  osi.glr->glExt.glWindowPos2fv (v);}
inline GLAPI void APIENTRY glWindowPos2i (GLint x, GLint y) {
  osi.glr->glExt.glWindowPos2i (x, y);}
inline GLAPI void APIENTRY glWindowPos2iv (const GLint *v) {
  osi.glr->glExt.glWindowPos2iv (v);}
inline GLAPI void APIENTRY glWindowPos2s (GLshort x, GLshort y) {
  osi.glr->glExt.glWindowPos2s (x, y);}
inline GLAPI void APIENTRY glWindowPos2sv (const GLshort *v) {
  osi.glr->glExt.glWindowPos2sv (v);}
inline GLAPI void APIENTRY glWindowPos3d (GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glWindowPos3d (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3dv (const GLdouble *v) {
  osi.glr->glExt.glWindowPos3dv (v);}
inline GLAPI void APIENTRY glWindowPos3f (GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glWindowPos3f (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3fv (const GLfloat *v) {
  osi.glr->glExt.glWindowPos3fv (v);}
inline GLAPI void APIENTRY glWindowPos3i (GLint x, GLint y, GLint z) {
  osi.glr->glExt.glWindowPos3i (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3iv (const GLint *v) {
  osi.glr->glExt.glWindowPos3iv (v);}
inline GLAPI void APIENTRY glWindowPos3s (GLshort x, GLshort y, GLshort z) {
  osi.glr->glExt.glWindowPos3s (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3sv (const GLshort *v) {
  osi.glr->glExt.glWindowPos3sv (v);}
inline GLAPI void APIENTRY glBlendColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  osi.glr->glExt.glBlendColor (red, green, blue, alpha);}
inline GLAPI void APIENTRY glBlendEquation (GLenum mode) {
  osi.glr->glExt.glBlendEquation (mode);}

/// OpenGL 1.5 funcs =================------------------------------
inline GLAPI void APIENTRY glGenQueries (GLsizei n, GLuint *ids) {
  osi.glr->glExt.glGenQueries (n, ids);}
inline GLAPI void APIENTRY glDeleteQueries (GLsizei n, const GLuint *ids) {
  osi.glr->glExt.glDeleteQueries (n, ids);}
inline GLAPI GLboolean APIENTRY glIsQuery (GLuint id) {
  return osi.glr->glExt.glIsQuery (id);}
inline GLAPI void APIENTRY glBeginQuery (GLenum target, GLuint id) {
  osi.glr->glExt.glBeginQuery (target, id);}
inline GLAPI void APIENTRY glEndQuery (GLenum target) {
  osi.glr->glExt.glEndQuery (target);}
inline GLAPI void APIENTRY glGetQueryiv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetQueryiv (target, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectiv (GLuint id, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetQueryObjectiv (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectuiv (GLuint id, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetQueryObjectuiv (id, pname, params);}
inline GLAPI void APIENTRY glBindBuffer (GLenum target, GLuint buffer) {
  osi.glr->glExt.glBindBuffer (target, buffer);}
inline GLAPI void APIENTRY glDeleteBuffers (GLsizei n, const GLuint *buffers) {
  osi.glr->glExt.glDeleteBuffers (n, buffers);}
inline GLAPI void APIENTRY glGenBuffers (GLsizei n, GLuint *buffers) {
  osi.glr->glExt.glGenBuffers (n, buffers);}
inline GLAPI GLboolean APIENTRY glIsBuffer (GLuint buffer) {
  return osi.glr->glExt.glIsBuffer (buffer);}
inline GLAPI void APIENTRY glBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
  osi.glr->glExt.glBufferData (target, size, data, usage);}
inline GLAPI void APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void *data) {
  osi.glr->glExt.glBufferSubData (target, offset, size, data);}
inline GLAPI void APIENTRY glGetBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, void *data) {
  osi.glr->glExt.glGetBufferSubData (target, offset, size, data);}
inline GLAPI void *APIENTRY glMapBuffer (GLenum target, GLenum access) {
  return osi.glr->glExt.glMapBuffer (target, access);}
inline GLAPI GLboolean APIENTRY glUnmapBuffer (GLenum target) {
  return osi.glr->glExt.glUnmapBuffer (target);}
inline GLAPI void APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetBufferParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetBufferPointerv (GLenum target, GLenum pname, void **params) {
  osi.glr->glExt.glGetBufferPointerv (target, pname, params);}

/// OpenGL 2.0 funcs =================------------------------------
inline GLAPI void APIENTRY glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha) {
  osi.glr->glExt.glBlendEquationSeparate (modeRGB, modeAlpha);}
inline GLAPI void APIENTRY glDrawBuffers (GLsizei n, const GLenum *bufs) {
  osi.glr->glExt.glDrawBuffers (n, bufs);}
inline GLAPI void APIENTRY glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
  osi.glr->glExt.glStencilOpSeparate (face, sfail, dpfail, dppass);}
inline GLAPI void APIENTRY glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask) {
  osi.glr->glExt.glStencilFuncSeparate (face, func, ref, mask);}
inline GLAPI void APIENTRY glStencilMaskSeparate (GLenum face, GLuint mask) {
  osi.glr->glExt.glStencilMaskSeparate (face, mask);}
inline GLAPI void APIENTRY glAttachShader (GLuint program, GLuint shader) {
  osi.glr->glExt.glAttachShader (program, shader);}
inline GLAPI void APIENTRY glBindAttribLocation (GLuint program, GLuint index, const GLchar *name) {
  osi.glr->glExt.glBindAttribLocation (program, index, name);}
inline GLAPI void APIENTRY glCompileShader (GLuint shader) {
  osi.glr->glExt.glCompileShader (shader);}
inline GLAPI GLuint APIENTRY glCreateProgram (void) {
  return osi.glr->glExt.glCreateProgram ();}
inline GLAPI GLuint APIENTRY glCreateShader (GLenum type) {
  return osi.glr->glExt.glCreateShader (type);}
inline GLAPI void APIENTRY glDeleteProgram (GLuint program) {
  osi.glr->glExt.glDeleteProgram (program);}
inline GLAPI void APIENTRY glDeleteShader (GLuint shader) {
  osi.glr->glExt.glDeleteShader (shader);}
inline GLAPI void APIENTRY glDetachShader (GLuint program, GLuint shader) {
  osi.glr->glExt.glDetachShader (program, shader);}
inline GLAPI void APIENTRY glDisableVertexAttribArray (GLuint index) {
  osi.glr->glExt.glDisableVertexAttribArray (index);}
inline GLAPI void APIENTRY glEnableVertexAttribArray (GLuint index) {
  osi.glr->glExt.glEnableVertexAttribArray (index);}
inline GLAPI void APIENTRY glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
  osi.glr->glExt.glGetActiveAttrib (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
  osi.glr->glExt.glGetActiveUniform (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) {
  osi.glr->glExt.glGetAttachedShaders (program, maxCount, count, shaders);}
inline GLAPI GLint APIENTRY glGetAttribLocation (GLuint program, const GLchar *name) {
  return osi.glr->glExt.glGetAttribLocation (program, name);}
inline GLAPI void APIENTRY glGetProgramiv (GLuint program, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetProgramiv (program, pname, params);}
inline GLAPI void APIENTRY glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
  osi.glr->glExt.glGetProgramInfoLog (program, bufSize, length, infoLog);}
inline GLAPI void APIENTRY glGetShaderiv (GLuint shader, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetShaderiv (shader, pname, params);}
inline GLAPI void APIENTRY glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
  osi.glr->glExt.glGetShaderInfoLog (shader, bufSize, length, infoLog);}
inline GLAPI void APIENTRY glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) {
  osi.glr->glExt.glGetShaderSource (shader, bufSize, length, source);}
inline GLAPI GLint APIENTRY glGetUniformLocation (GLuint program, const GLchar *name) {
  return osi.glr->glExt.glGetUniformLocation (program, name);}
inline GLAPI void APIENTRY glGetUniformfv (GLuint program, GLint location, GLfloat *params) {
  osi.glr->glExt.glGetUniformfv (program, location, params);}
inline GLAPI void APIENTRY glGetUniformiv (GLuint program, GLint location, GLint *params) {
  osi.glr->glExt.glGetUniformiv (program, location, params);}
inline GLAPI void APIENTRY glGetVertexAttribdv (GLuint index, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetVertexAttribdv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetVertexAttribfv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVertexAttribiv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribPointerv (GLuint index, GLenum pname, void **pointer) {
  osi.glr->glExt.glGetVertexAttribPointerv (index, pname, pointer);}
inline GLAPI GLboolean APIENTRY glIsProgram (GLuint program) {
  return osi.glr->glExt.glIsProgram (program);}
inline GLAPI GLboolean APIENTRY glIsShader (GLuint shader) {
  return osi.glr->glExt.glIsShader (shader);}
inline GLAPI void APIENTRY glLinkProgram (GLuint program) {
  osi.glr->glExt.glLinkProgram (program);}
inline GLAPI void APIENTRY glShaderSource (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) {
  osi.glr->glExt.glShaderSource (shader, count, string, length);}
inline GLAPI void APIENTRY glUseProgram (GLuint program) {
  osi.glr->glExt.glUseProgram (program);}
inline GLAPI void APIENTRY glUniform1f (GLint location, GLfloat v0) {
  osi.glr->glExt.glUniform1f (location, v0);}
inline GLAPI void APIENTRY glUniform2f (GLint location, GLfloat v0, GLfloat v1) {
  osi.glr->glExt.glUniform2f (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  osi.glr->glExt.glUniform3f (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  osi.glr->glExt.glUniform4f (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1i (GLint location, GLint v0) {
  osi.glr->glExt.glUniform1i (location, v0);}
inline GLAPI void APIENTRY glUniform2i (GLint location, GLint v0, GLint v1) {
  osi.glr->glExt.glUniform2i (location,  v0,  v1);}
inline GLAPI void APIENTRY glUniform3i (GLint location, GLint v0, GLint v1, GLint v2) {
  osi.glr->glExt.glUniform3i (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  osi.glr->glExt.glUniform4i (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1fv (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform1fv (location, count, value);}
inline GLAPI void APIENTRY glUniform2fv (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform2fv (location, count, value);}
inline GLAPI void APIENTRY glUniform3fv (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform3fv (location, count, value);}
inline GLAPI void APIENTRY glUniform4fv (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform4fv (location, count, value);}
inline GLAPI void APIENTRY glUniform1iv (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform1iv (location, count, value);}
inline GLAPI void APIENTRY glUniform2iv (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform2iv (location, count, value);}
inline GLAPI void APIENTRY glUniform3iv (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform3iv (location, count, value);}
inline GLAPI void APIENTRY glUniform4iv (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform4iv (location, count, value);}
inline GLAPI void APIENTRY glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix2fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix3fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix4fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glValidateProgram (GLuint program) {
  osi.glr->glExt.glValidateProgram (program);}
inline GLAPI void APIENTRY glVertexAttrib1d (GLuint index, GLdouble x) {
  osi.glr->glExt.glVertexAttrib1d (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib1dv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1f (GLuint index, GLfloat x) {
  osi.glr->glExt.glVertexAttrib1f (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1fv (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib1fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1s (GLuint index, GLshort x) {
  osi.glr->glExt.glVertexAttrib1s (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1sv (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib1sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2d (GLuint index, GLdouble x, GLdouble y) {
  osi.glr->glExt.glVertexAttrib2d (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib2dv (index,  v);}
inline GLAPI void APIENTRY glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y) {
  osi.glr->glExt.glVertexAttrib2f (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2fv (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib2fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2s (GLuint index, GLshort x, GLshort y) {
  osi.glr->glExt.glVertexAttrib2s (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2sv (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib2sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3d (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glVertexAttrib3d (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib3dv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glVertexAttrib3f (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3fv (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib3fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3s (GLuint index, GLshort x, GLshort y, GLshort z) {
  osi.glr->glExt.glVertexAttrib3s (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3sv (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib3sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nbv (GLuint index, const GLbyte *v) {
  osi.glr->glExt.glVertexAttrib4Nbv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Niv (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttrib4Niv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nsv (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib4Nsv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nub (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
  osi.glr->glExt.glVertexAttrib4Nub (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4Nubv (GLuint index, const GLubyte *v) {
  osi.glr->glExt.glVertexAttrib4Nubv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nuiv (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttrib4Nuiv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nusv (GLuint index, const GLushort *v) {
  osi.glr->glExt.glVertexAttrib4Nusv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4bv (GLuint index, const GLbyte *v) {
  osi.glr->glExt.glVertexAttrib4bv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4d (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glVertexAttrib4d (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib4dv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glVertexAttrib4f (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4fv (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib4fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4iv (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttrib4iv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4s (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
  osi.glr->glExt.glVertexAttrib4s (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4sv (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib4sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ubv (GLuint index, const GLubyte *v) {
  osi.glr->glExt.glVertexAttrib4ubv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4uiv (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttrib4uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4usv (GLuint index, const GLushort *v) {
  osi.glr->glExt.glVertexAttrib4usv (index, v);}
inline GLAPI void APIENTRY glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexAttribPointer (index, size, type, normalized, stride, pointer);}

/// OpenGL 2.1 funcs =================------------------------------
inline GLAPI void APIENTRY glUniformMatrix2x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix2x3fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix3x2fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix2x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix2x4fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix4x2fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix3x4fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix4x3fv (location, count, transpose, value);}

/// OpenGL 3.0 funcs =================------------------------------
inline GLAPI void APIENTRY glColorMaski (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
  osi.glr->glExt.glColorMaski (index, r, g, b, a);}
inline GLAPI void APIENTRY glGetBooleani_v (GLenum target, GLuint index, GLboolean *data) {
  osi.glr->glExt.glGetBooleani_v (target, index, data);}
inline GLAPI void APIENTRY glGetIntegeri_v (GLenum target, GLuint index, GLint *data) {
  osi.glr->glExt.glGetIntegeri_v (target, index, data);}
inline GLAPI void APIENTRY glEnablei (GLenum target, GLuint index) {
  osi.glr->glExt.glEnablei (target, index);}
inline GLAPI void APIENTRY glDisablei (GLenum target, GLuint index) {
  osi.glr->glExt.glDisablei (target, index);}
inline GLAPI GLboolean APIENTRY glIsEnabledi (GLenum target, GLuint index) {
  return osi.glr->glExt.glIsEnabledi (target, index);}
inline GLAPI void APIENTRY glBeginTransformFeedback (GLenum primitiveMode) {
  osi.glr->glExt.glBeginTransformFeedback (primitiveMode);}
inline GLAPI void APIENTRY glEndTransformFeedback (void) {
  osi.glr->glExt.glEndTransformFeedback ();}
inline GLAPI void APIENTRY glBindBufferRange (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  osi.glr->glExt.glBindBufferRange (target, index, buffer,  offset, size);}
inline GLAPI void APIENTRY glBindBufferBase (GLenum target, GLuint index, GLuint buffer) {
  osi.glr->glExt.glBindBufferBase (target, index, buffer);}
inline GLAPI void APIENTRY glTransformFeedbackVaryings (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode) {
  osi.glr->glExt.glTransformFeedbackVaryings (program, count, varyings, bufferMode);}
inline GLAPI void APIENTRY glGetTransformFeedbackVarying (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
  osi.glr->glExt.glGetTransformFeedbackVarying (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glClampColor (GLenum target, GLenum clamp) {
  osi.glr->glExt.glClampColor (target, clamp);}
inline GLAPI void APIENTRY glBeginConditionalRender (GLuint id, GLenum mode) {
  osi.glr->glExt.glBeginConditionalRender (id, mode);}
inline GLAPI void APIENTRY glEndConditionalRender (void) {
  osi.glr->glExt.glEndConditionalRender ();}
inline GLAPI void APIENTRY glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexAttribIPointer (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribIiv (GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVertexAttribIiv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribIuiv (GLuint index, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetVertexAttribIuiv (index, pname, params);}
inline GLAPI void APIENTRY glVertexAttribI1i (GLuint index, GLint x) {
  osi.glr->glExt.glVertexAttribI1i (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2i (GLuint index, GLint x, GLint y) {
  osi.glr->glExt.glVertexAttribI2i (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3i (GLuint index, GLint x, GLint y, GLint z) {
  osi.glr->glExt.glVertexAttribI3i (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4i (GLuint index, GLint x, GLint y, GLint z, GLint w) {
  osi.glr->glExt.glVertexAttribI4i (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1ui (GLuint index, GLuint x) {
  osi.glr->glExt.glVertexAttribI1ui (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2ui (GLuint index, GLuint x, GLuint y) {
  osi.glr->glExt.glVertexAttribI2ui (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3ui (GLuint index, GLuint x, GLuint y, GLuint z) {
  osi.glr->glExt.glVertexAttribI3ui (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4ui (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  osi.glr->glExt.glVertexAttribI4ui (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1iv (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI1iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2iv (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI2iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3iv (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI3iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4iv (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI4iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI1uiv (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI1uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2uiv (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI2uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3uiv (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI3uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4uiv (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI4uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4bv (GLuint index, const GLbyte *v) {
  osi.glr->glExt.glVertexAttribI4bv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4sv (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttribI4sv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4ubv (GLuint index, const GLubyte *v) {
  osi.glr->glExt.glVertexAttribI4ubv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4usv (GLuint index, const GLushort *v) {
  osi.glr->glExt.glVertexAttribI4usv (index, v);}
inline GLAPI void APIENTRY glGetUniformuiv (GLuint program, GLint location, GLuint *params) {
  osi.glr->glExt.glGetUniformuiv (program, location, params);}
inline GLAPI void APIENTRY glBindFragDataLocation (GLuint program, GLuint color, const GLchar *name) {
  osi.glr->glExt.glBindFragDataLocation (program, color, name);}
inline GLAPI GLint APIENTRY glGetFragDataLocation (GLuint program, const GLchar *name) {
  return osi.glr->glExt.glGetFragDataLocation (program, name);}
inline GLAPI void APIENTRY glUniform1ui (GLint location, GLuint v0) {
  osi.glr->glExt.glUniform1ui (location, v0);}
inline GLAPI void APIENTRY glUniform2ui (GLint location, GLuint v0, GLuint v1) {
  osi.glr->glExt.glUniform2ui (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2) {
  osi.glr->glExt.glUniform3ui (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  osi.glr->glExt.glUniform4ui (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1uiv (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform1uiv (location, count, value);}
inline GLAPI void APIENTRY glUniform2uiv (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform2uiv (location, count, value);}
inline GLAPI void APIENTRY glUniform3uiv (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform3uiv (location,  count, value);}
inline GLAPI void APIENTRY glUniform4uiv (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform4uiv (location, count, value);}
inline GLAPI void APIENTRY glTexParameterIiv (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glTexParameterIiv (target, pname, params);}
inline GLAPI void APIENTRY glTexParameterIuiv (GLenum target, GLenum pname, const GLuint *params) {
  osi.glr->glExt.glTexParameterIuiv (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIiv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetTexParameterIiv (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIuiv (GLenum target, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetTexParameterIuiv (target, pname, params);}
inline GLAPI void APIENTRY glClearBufferiv (GLenum buffer, GLint drawbuffer, const GLint *value) {
  osi.glr->glExt.glClearBufferiv (buffer, drawbuffer, value);}
inline GLAPI void APIENTRY glClearBufferuiv (GLenum buffer, GLint drawbuffer, const GLuint *value) {
  osi.glr->glExt.glClearBufferuiv (buffer, drawbuffer, value);}
inline GLAPI void APIENTRY glClearBufferfv (GLenum buffer, GLint drawbuffer, const GLfloat *value) {
  osi.glr->glExt.glClearBufferfv (buffer, drawbuffer, value);}
inline GLAPI void APIENTRY glClearBufferfi (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
  osi.glr->glExt.glClearBufferfi (buffer, drawbuffer, depth, stencil);}
inline GLAPI const GLubyte *APIENTRY glGetStringi (GLenum name, GLuint index) {
  return osi.glr->glExt.glGetStringi (name, index);}
inline GLAPI GLboolean APIENTRY glIsRenderbuffer (GLuint renderbuffer) {
  return osi.glr->glExt.glIsRenderbuffer (renderbuffer);}
inline GLAPI void APIENTRY glBindRenderbuffer (GLenum target, GLuint renderbuffer) {
  osi.glr->glExt.glBindRenderbuffer (target, renderbuffer);}
inline GLAPI void APIENTRY glDeleteRenderbuffers (GLsizei n, const GLuint *renderbuffers) {
  osi.glr->glExt.glDeleteRenderbuffers (n, renderbuffers);}
inline GLAPI void APIENTRY glGenRenderbuffers (GLsizei n, GLuint *renderbuffers) {
  osi.glr->glExt.glGenRenderbuffers (n, renderbuffers);}
inline GLAPI void APIENTRY glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glRenderbufferStorage (target, internalformat, width, height);}
inline GLAPI void APIENTRY glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetRenderbufferParameteriv (target, pname, params);}
inline GLAPI GLboolean APIENTRY glIsFramebuffer (GLuint framebuffer) {
  return osi.glr->glExt.glIsFramebuffer (framebuffer);}
inline GLAPI void APIENTRY glBindFramebuffer (GLenum target, GLuint framebuffer) {
  osi.glr->glExt.glBindFramebuffer (target, framebuffer);}
inline GLAPI void APIENTRY glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers) {
  osi.glr->glExt.glDeleteFramebuffers (n, framebuffers);}
inline GLAPI void APIENTRY glGenFramebuffers (GLsizei n, GLuint *framebuffers) {
  osi.glr->glExt.glGenFramebuffers (n, framebuffers);}
inline GLAPI GLenum APIENTRY glCheckFramebufferStatus (GLenum target) {
  return osi.glr->glExt.glCheckFramebufferStatus (target);}
inline GLAPI void APIENTRY glFramebufferTexture1D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  osi.glr->glExt.glFramebufferTexture1D (target, attachment, textarget,  texture,  level);}
inline GLAPI void APIENTRY glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  osi.glr->glExt.glFramebufferTexture2D (target, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glFramebufferTexture3D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
  osi.glr->glExt.glFramebufferTexture3D (target, attachment,  textarget, texture, level, zoffset);}
inline GLAPI void APIENTRY glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
  osi.glr->glExt.glFramebufferRenderbuffer (target, attachment, renderbuffertarget, renderbuffer);}
inline GLAPI void APIENTRY glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFramebufferAttachmentParameteriv (target, attachment, pname, params);}
inline GLAPI void APIENTRY glGenerateMipmap (GLenum target) {
  osi.glr->glExt.glGenerateMipmap (target);}
inline GLAPI void APIENTRY glBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
  osi.glr->glExt.glBlitFramebuffer (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);}
inline GLAPI void APIENTRY glRenderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glRenderbufferStorageMultisample (target, samples, internalformat, width, height);}
inline GLAPI void APIENTRY glFramebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  osi.glr->glExt.glFramebufferTextureLayer (target, attachment, texture, level, layer);}
inline GLAPI void *APIENTRY glMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
  return osi.glr->glExt.glMapBufferRange (target, offset, length, access);}
inline GLAPI void APIENTRY glFlushMappedBufferRange (GLenum target, GLintptr offset, GLsizeiptr length) {
  osi.glr->glExt.glFlushMappedBufferRange (target, offset, length);}
inline GLAPI void APIENTRY glBindVertexArray (GLuint array) {
  osi.glr->glExt.glBindVertexArray (array);}
inline GLAPI void APIENTRY glDeleteVertexArrays (GLsizei n, const GLuint *arrays) {
  osi.glr->glExt.glDeleteVertexArrays (n, arrays);}
inline GLAPI void APIENTRY glGenVertexArrays (GLsizei n, GLuint *arrays) {
  osi.glr->glExt.glGenVertexArrays (n, arrays);}
inline GLAPI GLboolean APIENTRY glIsVertexArray (GLuint array) {
  return osi.glr->glExt.glIsVertexArray (array);}

/// OpenGL 3.1 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount) {
  osi.glr->glExt.glDrawArraysInstanced (mode, first, count, instancecount);}
inline GLAPI void APIENTRY glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) {
  osi.glr->glExt.glDrawElementsInstanced (mode, count, type, indices, instancecount);}
inline GLAPI void APIENTRY glTexBuffer (GLenum target, GLenum internalformat, GLuint buffer) {
  osi.glr->glExt.glTexBuffer (target, internalformat, buffer);}
inline GLAPI void APIENTRY glPrimitiveRestartIndex (GLuint index) {
  osi.glr->glExt.glPrimitiveRestartIndex (index);}
inline GLAPI void APIENTRY glCopyBufferSubData (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
  osi.glr->glExt.glCopyBufferSubData (readTarget, writeTarget, readOffset, writeOffset, size);}
inline GLAPI void APIENTRY glGetUniformIndices (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices) {
  osi.glr->glExt.glGetUniformIndices (program, uniformCount, uniformNames, uniformIndices);}
inline GLAPI void APIENTRY glGetActiveUniformsiv (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetActiveUniformsiv (program, uniformCount, uniformIndices, pname, params);}
inline GLAPI void APIENTRY glGetActiveUniformName (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName) {
  osi.glr->glExt.glGetActiveUniformName (program, uniformIndex, bufSize, length, uniformName);}
inline GLAPI GLuint APIENTRY glGetUniformBlockIndex (GLuint program, const GLchar *uniformBlockName) {
  return osi.glr->glExt.glGetUniformBlockIndex (program, uniformBlockName);}
inline GLAPI void APIENTRY glGetActiveUniformBlockiv (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetActiveUniformBlockiv (program, uniformBlockIndex, pname, params);}
inline GLAPI void APIENTRY glGetActiveUniformBlockName (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) {
  osi.glr->glExt.glGetActiveUniformBlockName (program, uniformBlockIndex, bufSize, length, uniformBlockName);}
inline GLAPI void APIENTRY glUniformBlockBinding (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
  osi.glr->glExt.glUniformBlockBinding (program, uniformBlockIndex, uniformBlockBinding);}

/// OpenGL 3.2 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawElementsBaseVertex (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) {
  osi.glr->glExt.glDrawElementsBaseVertex (mode, count, type, indices, basevertex);}
inline GLAPI void APIENTRY glDrawRangeElementsBaseVertex (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex) {
  osi.glr->glExt.glDrawRangeElementsBaseVertex (mode, start, end, count, type, indices, basevertex);}
inline GLAPI void APIENTRY glDrawElementsInstancedBaseVertex (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex) {
  osi.glr->glExt.glDrawElementsInstancedBaseVertex (mode, count, type, indices, instancecount, basevertex);}
inline GLAPI void APIENTRY glMultiDrawElementsBaseVertex (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex) {
  osi.glr->glExt.glMultiDrawElementsBaseVertex (mode, count, type, indices, drawcount, basevertex);}
inline GLAPI void APIENTRY glProvokingVertex (GLenum mode) {
  osi.glr->glExt.glProvokingVertex (mode);}
inline GLAPI GLsync APIENTRY glFenceSync (GLenum condition, GLbitfield flags) {
  return osi.glr->glExt.glFenceSync (condition, flags);}
inline GLAPI GLboolean APIENTRY glIsSync (GLsync sync) {
  return osi.glr->glExt.glIsSync (sync);}
inline GLAPI void APIENTRY glDeleteSync (GLsync sync) {
  osi.glr->glExt.glDeleteSync (sync);}
inline GLAPI GLenum APIENTRY glClientWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout) {
  return osi.glr->glExt.glClientWaitSync (sync, flags, timeout);}
inline GLAPI void APIENTRY glWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout) {
  osi.glr->glExt.glWaitSync (sync, flags, timeout);}
inline GLAPI void APIENTRY glGetInteger64v (GLenum pname, GLint64 *data) {
  osi.glr->glExt.glGetInteger64v (pname, data);}
inline GLAPI void APIENTRY glGetSynciv (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) {
  osi.glr->glExt.glGetSynciv (sync, pname, bufSize, length, values);}
inline GLAPI void APIENTRY glGetInteger64i_v (GLenum target, GLuint index, GLint64 *data) {
  osi.glr->glExt.glGetInteger64i_v (target, index, data);}
inline GLAPI void APIENTRY glGetBufferParameteri64v (GLenum target, GLenum pname, GLint64 *params) {
  osi.glr->glExt.glGetBufferParameteri64v (target, pname, params);}
inline GLAPI void APIENTRY glFramebufferTexture (GLenum target, GLenum attachment, GLuint texture, GLint level) {
  osi.glr->glExt.glFramebufferTexture (target, attachment, texture, level);}
inline GLAPI void APIENTRY glTexImage2DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
  osi.glr->glExt.glTexImage2DMultisample (target, samples, internalformat, width, height, fixedsamplelocations);}
inline GLAPI void APIENTRY glTexImage3DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
  osi.glr->glExt.glTexImage3DMultisample (target, samples, internalformat, width, height, depth, fixedsamplelocations);}
inline GLAPI void APIENTRY glGetMultisamplefv (GLenum pname, GLuint index, GLfloat *val) {
  osi.glr->glExt.glGetMultisamplefv (pname, index, val);}
inline GLAPI void APIENTRY glSampleMaski (GLuint maskNumber, GLbitfield mask) {
  osi.glr->glExt.glSampleMaski (maskNumber, mask);}

/// OpenGL 3.3 funcs =================------------------------------
inline GLAPI void APIENTRY glBindFragDataLocationIndexed (GLuint program, GLuint colorNumber, GLuint index, const GLchar *name) {
  osi.glr->glExt.glBindFragDataLocationIndexed (program, colorNumber, index, name);}
inline GLAPI GLint APIENTRY glGetFragDataIndex (GLuint program, const GLchar *name) {
  return osi.glr->glExt.glGetFragDataIndex (program, name);}
inline GLAPI void APIENTRY glGenSamplers (GLsizei count, GLuint *samplers) {
  osi.glr->glExt.glGenSamplers (count, samplers);}
inline GLAPI void APIENTRY glDeleteSamplers (GLsizei count, const GLuint *samplers) {
  osi.glr->glExt.glDeleteSamplers (count, samplers);}
inline GLAPI GLboolean APIENTRY glIsSampler (GLuint sampler) {
  return osi.glr->glExt.glIsSampler (sampler);}
inline GLAPI void APIENTRY glBindSampler (GLuint unit, GLuint sampler) {
  osi.glr->glExt.glBindSampler (unit, sampler);}
inline GLAPI void APIENTRY glSamplerParameteri (GLuint sampler, GLenum pname, GLint param) {
  osi.glr->glExt.glSamplerParameteri (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameteriv (GLuint sampler, GLenum pname, const GLint *param) {
  osi.glr->glExt.glSamplerParameteriv (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterf (GLuint sampler, GLenum pname, GLfloat param) {
  osi.glr->glExt.glSamplerParameterf (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterfv (GLuint sampler, GLenum pname, const GLfloat *param) {
  osi.glr->glExt.glSamplerParameterfv (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterIiv (GLuint sampler, GLenum pname, const GLint *param) {
  osi.glr->glExt.glSamplerParameterIiv (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterIuiv (GLuint sampler, GLenum pname, const GLuint *param) {
  osi.glr->glExt.glSamplerParameterIuiv (sampler, pname, param);}
inline GLAPI void APIENTRY glGetSamplerParameteriv (GLuint sampler, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetSamplerParameteriv (sampler, pname, params);}
inline GLAPI void APIENTRY glGetSamplerParameterIiv (GLuint sampler, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetSamplerParameterIiv (sampler, pname, params);}
inline GLAPI void APIENTRY glGetSamplerParameterfv (GLuint sampler, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetSamplerParameterfv (sampler, pname, params);}
inline GLAPI void APIENTRY glGetSamplerParameterIuiv (GLuint sampler, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetSamplerParameterIuiv (sampler, pname, params);}
inline GLAPI void APIENTRY glQueryCounter (GLuint id, GLenum target) {
  osi.glr->glExt.glQueryCounter (id, target);}
inline GLAPI void APIENTRY glGetQueryObjecti64v (GLuint id, GLenum pname, GLint64 *params) {
  osi.glr->glExt.glGetQueryObjecti64v (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectui64v (GLuint id, GLenum pname, GLuint64 *params) {
  osi.glr->glExt.glGetQueryObjectui64v (id, pname, params);}
inline GLAPI void APIENTRY glVertexAttribDivisor (GLuint index, GLuint divisor) {
  osi.glr->glExt.glVertexAttribDivisor (index, divisor);}
inline GLAPI void APIENTRY glVertexAttribP1ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  osi.glr->glExt.glVertexAttribP1ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP1uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  osi.glr->glExt.glVertexAttribP1uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP2ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  osi.glr->glExt.glVertexAttribP2ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP2uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  osi.glr->glExt.glVertexAttribP2uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP3ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  osi.glr->glExt.glVertexAttribP3ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP3uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  osi.glr->glExt.glVertexAttribP3uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP4ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  osi.glr->glExt.glVertexAttribP4ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP4uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  osi.glr->glExt.glVertexAttribP4uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexP2ui (GLenum type, GLuint value) {
  osi.glr->glExt.glVertexP2ui (type, value);}
inline GLAPI void APIENTRY glVertexP2uiv (GLenum type, const GLuint *value) {
  osi.glr->glExt.glVertexP2uiv (type, value);}
inline GLAPI void APIENTRY glVertexP3ui (GLenum type, GLuint value) {
  osi.glr->glExt.glVertexP3ui (type, value);}
inline GLAPI void APIENTRY glVertexP3uiv (GLenum type, const GLuint *value) {
  osi.glr->glExt.glVertexP3uiv (type, value);}
inline GLAPI void APIENTRY glVertexP4ui (GLenum type, GLuint value) {
  osi.glr->glExt.glVertexP4ui (type, value);}
inline GLAPI void APIENTRY glVertexP4uiv (GLenum type, const GLuint *value) {
  osi.glr->glExt.glVertexP4uiv (type, value);}
inline GLAPI void APIENTRY glTexCoordP1ui (GLenum type, GLuint coords) {
  osi.glr->glExt.glTexCoordP1ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP1uiv (GLenum type, const GLuint *coords) {
  osi.glr->glExt.glTexCoordP1uiv (type, coords);}
inline GLAPI void APIENTRY glTexCoordP2ui (GLenum type, GLuint coords) {
  osi.glr->glExt.glTexCoordP2ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP2uiv (GLenum type, const GLuint *coords) {
  osi.glr->glExt.glTexCoordP2uiv (type, coords);}
inline GLAPI void APIENTRY glTexCoordP3ui (GLenum type, GLuint coords) {
  osi.glr->glExt.glTexCoordP3ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP3uiv (GLenum type, const GLuint *coords) {
  osi.glr->glExt.glTexCoordP3uiv (type, coords);}
inline GLAPI void APIENTRY glTexCoordP4ui (GLenum type, GLuint coords) {
  osi.glr->glExt.glTexCoordP4ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP4uiv (GLenum type, const GLuint *coords) {
  osi.glr->glExt.glTexCoordP4uiv (type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP1ui (GLenum texture, GLenum type, GLuint coords) {
  osi.glr->glExt.glMultiTexCoordP1ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP1uiv (GLenum texture, GLenum type, const GLuint *coords) {
  osi.glr->glExt.glMultiTexCoordP1uiv (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP2ui (GLenum texture, GLenum type, GLuint coords) {
  osi.glr->glExt.glMultiTexCoordP2ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP2uiv (GLenum texture, GLenum type, const GLuint *coords) {
  osi.glr->glExt.glMultiTexCoordP2uiv (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP3ui (GLenum texture, GLenum type, GLuint coords) {
  osi.glr->glExt.glMultiTexCoordP3ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP3uiv (GLenum texture, GLenum type, const GLuint *coords) {
  osi.glr->glExt.glMultiTexCoordP3uiv (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP4ui (GLenum texture, GLenum type, GLuint coords) {
  osi.glr->glExt.glMultiTexCoordP4ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP4uiv (GLenum texture, GLenum type, const GLuint *coords) {
  osi.glr->glExt.glMultiTexCoordP4uiv (texture, type, coords);}
inline GLAPI void APIENTRY glNormalP3ui (GLenum type, GLuint coords) {
  osi.glr->glExt.glNormalP3ui (type, coords);}
inline GLAPI void APIENTRY glNormalP3uiv (GLenum type, const GLuint *coords) {
  osi.glr->glExt.glNormalP3uiv (type, coords);}
inline GLAPI void APIENTRY glColorP3ui (GLenum type, GLuint color) {
  osi.glr->glExt.glColorP3ui (type, color);}
inline GLAPI void APIENTRY glColorP3uiv (GLenum type, const GLuint *color) {
  osi.glr->glExt.glColorP3uiv (type, color);}
inline GLAPI void APIENTRY glColorP4ui (GLenum type, GLuint color) {
  osi.glr->glExt.glColorP4ui (type, color);}
inline GLAPI void APIENTRY glColorP4uiv (GLenum type, const GLuint *color) {
  osi.glr->glExt.glColorP4uiv (type, color);}
inline GLAPI void APIENTRY glSecondaryColorP3ui (GLenum type, GLuint color) {
  osi.glr->glExt.glSecondaryColorP3ui (type,  color);}
inline GLAPI void APIENTRY glSecondaryColorP3uiv (GLenum type, const GLuint *color) {
  osi.glr->glExt.glSecondaryColorP3uiv (type, color);}

/// OpenGL 4.0 funcs =================------------------------------
inline GLAPI void APIENTRY glMinSampleShading (GLfloat value) {
  osi.glr->glExt.glMinSampleShading (value);}
inline GLAPI void APIENTRY glBlendEquationi (GLuint buf, GLenum mode) {
  osi.glr->glExt.glBlendEquationi (buf, mode);}
inline GLAPI void APIENTRY glBlendEquationSeparatei (GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
  osi.glr->glExt.glBlendEquationSeparatei (buf, modeRGB, modeAlpha);}
inline GLAPI void APIENTRY glBlendFunci (GLuint buf, GLenum src, GLenum dst) {
  osi.glr->glExt.glBlendFunci (buf, src, dst);}
inline GLAPI void APIENTRY glBlendFuncSeparatei (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  osi.glr->glExt.glBlendFuncSeparatei (buf, srcRGB, dstRGB, srcAlpha, dstAlpha);}
inline GLAPI void APIENTRY glDrawArraysIndirect (GLenum mode, const void *indirect) {
  osi.glr->glExt.glDrawArraysIndirect (mode, indirect);}
inline GLAPI void APIENTRY glDrawElementsIndirect (GLenum mode, GLenum type, const void *indirect) {
  osi.glr->glExt.glDrawElementsIndirect (mode, type, indirect);}
inline GLAPI void APIENTRY glUniform1d (GLint location, GLdouble x) {
  osi.glr->glExt.glUniform1d (location, x);}
inline GLAPI void APIENTRY glUniform2d (GLint location, GLdouble x, GLdouble y) {
  osi.glr->glExt.glUniform2d (location, x, y);}
inline GLAPI void APIENTRY glUniform3d (GLint location, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glUniform3d (location, x, y, z);}
inline GLAPI void APIENTRY glUniform4d (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glUniform4d (location, x, y, z, w);}
inline GLAPI void APIENTRY glUniform1dv (GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glUniform1dv (location, count, value);}
inline GLAPI void APIENTRY glUniform2dv (GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glUniform2dv (location, count, value);}
inline GLAPI void APIENTRY glUniform3dv (GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glUniform3dv (location, count, value);}
inline GLAPI void APIENTRY glUniform4dv (GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glUniform4dv (location, count, value);}
inline GLAPI void APIENTRY glUniformMatrix2dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix2dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix3dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix4dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix2x3dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix2x3dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix2x4dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix2x4dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x2dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix3x2dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x4dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix3x4dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x2dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix4x2dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x3dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glUniformMatrix4x3dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glGetUniformdv (GLuint program, GLint location, GLdouble *params) {
  osi.glr->glExt.glGetUniformdv (program, location, params);}
inline GLAPI GLint APIENTRY glGetSubroutineUniformLocation (GLuint program, GLenum shadertype, const GLchar *name) {
  return osi.glr->glExt.glGetSubroutineUniformLocation (program, shadertype, name);}
inline GLAPI GLuint APIENTRY glGetSubroutineIndex (GLuint program, GLenum shadertype, const GLchar *name) {
  return osi.glr->glExt.glGetSubroutineIndex (program, shadertype, name);}
inline GLAPI void APIENTRY glGetActiveSubroutineUniformiv (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values) {
  osi.glr->glExt.glGetActiveSubroutineUniformiv (program, shadertype, index, pname, values);}
inline GLAPI void APIENTRY glGetActiveSubroutineUniformName (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name) {
  osi.glr->glExt.glGetActiveSubroutineUniformName (program, shadertype, index, bufsize, length, name);}
inline GLAPI void APIENTRY glGetActiveSubroutineName (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name) {
  osi.glr->glExt.glGetActiveSubroutineName (program, shadertype, index, bufsize, length, name);}
inline GLAPI void APIENTRY glUniformSubroutinesuiv (GLenum shadertype, GLsizei count, const GLuint *indices) {
  osi.glr->glExt.glUniformSubroutinesuiv (shadertype, count, indices);}
inline GLAPI void APIENTRY glGetUniformSubroutineuiv (GLenum shadertype, GLint location, GLuint *params) {
  osi.glr->glExt.glGetUniformSubroutineuiv (shadertype, location, params);}
inline GLAPI void APIENTRY glGetProgramStageiv (GLuint program, GLenum shadertype, GLenum pname, GLint *values) {
  osi.glr->glExt.glGetProgramStageiv (program, shadertype, pname, values);}
inline GLAPI void APIENTRY glPatchParameteri (GLenum pname, GLint value) {
  osi.glr->glExt.glPatchParameteri (pname, value);}
inline GLAPI void APIENTRY glPatchParameterfv (GLenum pname, const GLfloat *values) {
  osi.glr->glExt.glPatchParameterfv (pname, values);}
inline GLAPI void APIENTRY glBindTransformFeedback (GLenum target, GLuint id) {
  osi.glr->glExt.glBindTransformFeedback (target, id);}
inline GLAPI void APIENTRY glDeleteTransformFeedbacks (GLsizei n, const GLuint *ids) {
  osi.glr->glExt.glDeleteTransformFeedbacks (n, ids);}
inline GLAPI void APIENTRY glGenTransformFeedbacks (GLsizei n, GLuint *ids) {
  osi.glr->glExt.glGenTransformFeedbacks (n, ids);}
inline GLAPI GLboolean APIENTRY glIsTransformFeedback (GLuint id) {
  return osi.glr->glExt.glIsTransformFeedback (id);}
inline GLAPI void APIENTRY glPauseTransformFeedback (void) {
  osi.glr->glExt.glPauseTransformFeedback ();}
inline GLAPI void APIENTRY glResumeTransformFeedback (void) {
  osi.glr->glExt.glResumeTransformFeedback ();}
inline GLAPI void APIENTRY glDrawTransformFeedback (GLenum mode, GLuint id) {
  osi.glr->glExt.glDrawTransformFeedback (mode, id);}
inline GLAPI void APIENTRY glDrawTransformFeedbackStream (GLenum mode, GLuint id, GLuint stream) {
  osi.glr->glExt.glDrawTransformFeedbackStream (mode, id, stream);}
inline GLAPI void APIENTRY glBeginQueryIndexed (GLenum target, GLuint index, GLuint id) {
  osi.glr->glExt.glBeginQueryIndexed (target, index, id);}
inline GLAPI void APIENTRY glEndQueryIndexed (GLenum target, GLuint index) {
  osi.glr->glExt.glEndQueryIndexed (target, index);}
inline GLAPI void APIENTRY glGetQueryIndexediv (GLenum target, GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetQueryIndexediv (target, index, pname, params);}

/// OpenGL 4.1 funcs =================------------------------------
inline GLAPI void APIENTRY glReleaseShaderCompiler (void) {
  osi.glr->glExt.glReleaseShaderCompiler ();}
inline GLAPI void APIENTRY glShaderBinary (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length) {
  osi.glr->glExt.glShaderBinary (count, shaders, binaryformat, binary, length);}
inline GLAPI void APIENTRY glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) {
  osi.glr->glExt.glGetShaderPrecisionFormat (shadertype, precisiontype, range, precision);}
inline GLAPI void APIENTRY glDepthRangef (GLfloat n, GLfloat f) {
  osi.glr->glExt.glDepthRangef (n, f);}
inline GLAPI void APIENTRY glClearDepthf (GLfloat d) {
  osi.glr->glExt.glClearDepthf (d);}
inline GLAPI void APIENTRY glGetProgramBinary (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) {
  osi.glr->glExt.glGetProgramBinary (program, bufSize, length, binaryFormat, binary);}
inline GLAPI void APIENTRY glProgramBinary (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length) {
  osi.glr->glExt.glProgramBinary (program, binaryFormat, binary, length);}
inline GLAPI void APIENTRY glProgramParameteri (GLuint program, GLenum pname, GLint value) {
  osi.glr->glExt.glProgramParameteri (program, pname, value);}
inline GLAPI void APIENTRY glUseProgramStages (GLuint pipeline, GLbitfield stages, GLuint program) {
  osi.glr->glExt.glUseProgramStages (pipeline, stages, program);}
inline GLAPI void APIENTRY glActiveShaderProgram (GLuint pipeline, GLuint program) {
  osi.glr->glExt.glActiveShaderProgram (pipeline, program);}
inline GLAPI GLuint APIENTRY glCreateShaderProgramv (GLenum type, GLsizei count, const GLchar *const*strings) {
  return osi.glr->glExt.glCreateShaderProgramv (type, count, strings);}
inline GLAPI void APIENTRY glBindProgramPipeline (GLuint pipeline) {
  osi.glr->glExt.glBindProgramPipeline (pipeline);}
inline GLAPI void APIENTRY glDeleteProgramPipelines (GLsizei n, const GLuint *pipelines) {
  osi.glr->glExt.glDeleteProgramPipelines (n, pipelines);}
inline GLAPI void APIENTRY glGenProgramPipelines (GLsizei n, GLuint *pipelines) {
  osi.glr->glExt.glGenProgramPipelines (n, pipelines);}
inline GLAPI GLboolean APIENTRY glIsProgramPipeline (GLuint pipeline) {
  return osi.glr->glExt.glIsProgramPipeline (pipeline);}
inline GLAPI void APIENTRY glGetProgramPipelineiv (GLuint pipeline, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetProgramPipelineiv (pipeline, pname, params);}
inline GLAPI void APIENTRY glProgramUniform1i (GLuint program, GLint location, GLint v0) {
  osi.glr->glExt.glProgramUniform1i (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform1iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform1iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1f (GLuint program, GLint location, GLfloat v0) {
  osi.glr->glExt.glProgramUniform1f (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform1fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform1fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1d (GLuint program, GLint location, GLdouble v0) {
  osi.glr->glExt.glProgramUniform1d (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform1dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform1dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1ui (GLuint program, GLint location, GLuint v0) {
  osi.glr->glExt.glProgramUniform1ui (program, location,  v0);}
inline GLAPI void APIENTRY glProgramUniform1uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform1uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2i (GLuint program, GLint location, GLint v0, GLint v1) {
  osi.glr->glExt.glProgramUniform2i (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform2iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2f (GLuint program, GLint location, GLfloat v0, GLfloat v1) {
  osi.glr->glExt.glProgramUniform2f (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform2fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2d (GLuint program, GLint location, GLdouble v0, GLdouble v1) {
  osi.glr->glExt.glProgramUniform2d (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform2dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2ui (GLuint program, GLint location, GLuint v0, GLuint v1) {
  osi.glr->glExt.glProgramUniform2ui (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform2uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3i (GLuint program, GLint location, GLint v0, GLint v1, GLint v2) {
  osi.glr->glExt.glProgramUniform3i (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform3iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  osi.glr->glExt.glProgramUniform3f (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform3fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3d (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2) {
  osi.glr->glExt.glProgramUniform3d (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform3dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3ui (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) {
  osi.glr->glExt.glProgramUniform3ui (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform3uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4i (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  osi.glr->glExt.glProgramUniform4i (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform4iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  osi.glr->glExt.glProgramUniform4f (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform4fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4d (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) {
  osi.glr->glExt.glProgramUniform4d (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform4dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4ui (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  osi.glr->glExt.glProgramUniform4ui (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform4uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix2fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix3fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix4fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix2dv (program,  location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix3dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix4dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix2x3fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix3x2fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix2x4fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix4x2fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix3x4fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix4x3fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix2x3dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix3x2dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix2x4dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix4x2dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix3x4dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix4x3dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glValidateProgramPipeline (GLuint pipeline) {
  osi.glr->glExt.glValidateProgramPipeline (pipeline);}
inline GLAPI void APIENTRY glGetProgramPipelineInfoLog (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
  osi.glr->glExt.glGetProgramPipelineInfoLog (pipeline, bufSize, length, infoLog);}
inline GLAPI void APIENTRY glVertexAttribL1d (GLuint index, GLdouble x) {
  osi.glr->glExt.glVertexAttribL1d (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2d (GLuint index, GLdouble x, GLdouble y) {
  osi.glr->glExt.glVertexAttribL2d (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3d (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glVertexAttribL3d (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4d (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glVertexAttribL4d (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL1dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL2dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL3dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4dv (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL4dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribLPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexAttribLPointer (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribLdv (GLuint index, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetVertexAttribLdv (index, pname, params);}
inline GLAPI void APIENTRY glViewportArrayv (GLuint first, GLsizei count, const GLfloat *v) {
  osi.glr->glExt.glViewportArrayv (first, count, v);}
inline GLAPI void APIENTRY glViewportIndexedf (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
  osi.glr->glExt.glViewportIndexedf (index, x, y, w, h);}
inline GLAPI void APIENTRY glViewportIndexedfv (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glViewportIndexedfv (index, v);}
inline GLAPI void APIENTRY glScissorArrayv (GLuint first, GLsizei count, const GLint *v) {
  osi.glr->glExt.glScissorArrayv (first, count, v);}
inline GLAPI void APIENTRY glScissorIndexed (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height) {
  osi.glr->glExt.glScissorIndexed (index, left, bottom,  width, height);}
inline GLAPI void APIENTRY glScissorIndexedv (GLuint index, const GLint *v) {
  osi.glr->glExt.glScissorIndexedv (index, v);}
inline GLAPI void APIENTRY glDepthRangeArrayv (GLuint first, GLsizei count, const GLdouble *v) {
  osi.glr->glExt.glDepthRangeArrayv (first, count, v);}
inline GLAPI void APIENTRY glDepthRangeIndexed (GLuint index, GLdouble n, GLdouble f) {
  osi.glr->glExt.glDepthRangeIndexed (index, n, f);}
inline GLAPI void APIENTRY glGetFloati_v (GLenum target, GLuint index, GLfloat *data) {
  osi.glr->glExt.glGetFloati_v (target, index, data);}
inline GLAPI void APIENTRY glGetDoublei_v (GLenum target, GLuint index, GLdouble *data) {
  osi.glr->glExt.glGetDoublei_v (target, index, data);}

/// OpenGL 4.2 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawArraysInstancedBaseInstance (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance) {
  osi.glr->glExt.glDrawArraysInstancedBaseInstance (mode, first, count, instancecount, baseinstance);}
inline GLAPI void APIENTRY glDrawElementsInstancedBaseInstance (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance) {
  osi.glr->glExt.glDrawElementsInstancedBaseInstance (mode, count, type, indices,  instancecount, baseinstance);}
inline GLAPI void APIENTRY glDrawElementsInstancedBaseVertexBaseInstance (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance) {
  osi.glr->glExt.glDrawElementsInstancedBaseVertexBaseInstance (mode, count, type, indices, instancecount, basevertex, baseinstance);}
inline GLAPI void APIENTRY glGetInternalformativ (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params) {
  osi.glr->glExt.glGetInternalformativ (target, internalformat, pname, bufSize, params);}
inline GLAPI void APIENTRY glGetActiveAtomicCounterBufferiv (GLuint program, GLuint bufferIndex, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetActiveAtomicCounterBufferiv (program, bufferIndex, pname, params);}
inline GLAPI void APIENTRY glBindImageTexture (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) {
  osi.glr->glExt.glBindImageTexture (unit, texture, level, layered, layer, access, format);}
inline GLAPI void APIENTRY glMemoryBarrier (GLbitfield barriers) {
  osi.glr->glExt.glMemoryBarrier (barriers);}
inline GLAPI void APIENTRY glTexStorage1D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) {
  osi.glr->glExt.glTexStorage1D (target, levels, internalformat, width);}
inline GLAPI void APIENTRY glTexStorage2D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glTexStorage2D (target, levels, internalformat, width, height);}
inline GLAPI void APIENTRY glTexStorage3D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
  osi.glr->glExt.glTexStorage3D (target, levels, internalformat, width, height, depth);}
inline GLAPI void APIENTRY glDrawTransformFeedbackInstanced (GLenum mode, GLuint id, GLsizei instancecount) {
  osi.glr->glExt.glDrawTransformFeedbackInstanced (mode, id, instancecount);}
inline GLAPI void APIENTRY glDrawTransformFeedbackStreamInstanced (GLenum mode, GLuint id, GLuint stream, GLsizei instancecount) {
  osi.glr->glExt.glDrawTransformFeedbackStreamInstanced (mode, id, stream, instancecount);}

/// OpenGL 4.3 funcs =================------------------------------
inline GLAPI void APIENTRY glClearBufferData (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glClearBufferData (target, internalformat, format, type, data);}
inline GLAPI void APIENTRY glClearBufferSubData (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glClearBufferSubData (target, internalformat, offset, size, format, type, data);}
inline GLAPI void APIENTRY glDispatchCompute (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) {
  osi.glr->glExt.glDispatchCompute (num_groups_x, num_groups_y, num_groups_z);}
inline GLAPI void APIENTRY glDispatchComputeIndirect (GLintptr indirect) {
  osi.glr->glExt.glDispatchComputeIndirect (indirect);}
inline GLAPI void APIENTRY glCopyImageSubData (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) {
  osi.glr->glExt.glCopyImageSubData (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);}
inline GLAPI void APIENTRY glFramebufferParameteri (GLenum target, GLenum pname, GLint param) {
  osi.glr->glExt.glFramebufferParameteri (target, pname, param);}
inline GLAPI void APIENTRY glGetFramebufferParameteriv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFramebufferParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetInternalformati64v (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params) {
  osi.glr->glExt.glGetInternalformati64v (target, internalformat, pname, bufSize, params);}
inline GLAPI void APIENTRY glInvalidateTexSubImage (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth) {
  osi.glr->glExt.glInvalidateTexSubImage (texture, level, xoffset, yoffset,  zoffset,  width, height, depth);}
inline GLAPI void APIENTRY glInvalidateTexImage (GLuint texture, GLint level) {
  osi.glr->glExt.glInvalidateTexImage (texture, level);}
inline GLAPI void APIENTRY glInvalidateBufferSubData (GLuint buffer, GLintptr offset, GLsizeiptr length) {
  osi.glr->glExt.glInvalidateBufferSubData (buffer, offset, length);}
inline GLAPI void APIENTRY glInvalidateBufferData (GLuint buffer) {
  osi.glr->glExt.glInvalidateBufferData (buffer);}
inline GLAPI void APIENTRY glInvalidateFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments) {
  osi.glr->glExt.glInvalidateFramebuffer (target, numAttachments, attachments);}
inline GLAPI void APIENTRY glInvalidateSubFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glInvalidateSubFramebuffer (target, numAttachments, attachments, x, y, width, height);}
inline GLAPI void APIENTRY glMultiDrawArraysIndirect (GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride) {
  osi.glr->glExt.glMultiDrawArraysIndirect (mode, indirect, drawcount, stride);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirect (GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride) {
  osi.glr->glExt.glMultiDrawElementsIndirect (mode, type, indirect, drawcount, stride);}
inline GLAPI void APIENTRY glGetProgramInterfaceiv (GLuint program, GLenum programInterface, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetProgramInterfaceiv (program, programInterface, pname, params);}
inline GLAPI GLuint APIENTRY glGetProgramResourceIndex (GLuint program, GLenum programInterface, const GLchar *name) {
  return osi.glr->glExt.glGetProgramResourceIndex (program, programInterface, name);}
inline GLAPI void APIENTRY glGetProgramResourceName (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name) {
  osi.glr->glExt.glGetProgramResourceName (program, programInterface, index, bufSize, length, name);}
inline GLAPI void APIENTRY glGetProgramResourceiv (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params) {
  osi.glr->glExt.glGetProgramResourceiv (program, programInterface, index, propCount, props, bufSize, length, params);}
inline GLAPI GLint APIENTRY glGetProgramResourceLocation (GLuint program, GLenum programInterface, const GLchar *name) {
  return osi.glr->glExt.glGetProgramResourceLocation (program, programInterface, name);}
inline GLAPI GLint APIENTRY glGetProgramResourceLocationIndex (GLuint program, GLenum programInterface, const GLchar *name) {
  return osi.glr->glExt.glGetProgramResourceLocationIndex (program, programInterface, name);}
inline GLAPI void APIENTRY glShaderStorageBlockBinding (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding) {
  osi.glr->glExt.glShaderStorageBlockBinding (program, storageBlockIndex, storageBlockBinding);}
inline GLAPI void APIENTRY glTexBufferRange (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  osi.glr->glExt.glTexBufferRange (target, internalformat, buffer, offset, size);}
inline GLAPI void APIENTRY glTexStorage2DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
  osi.glr->glExt.glTexStorage2DMultisample (target, samples, internalformat, width, height, fixedsamplelocations);}
inline GLAPI void APIENTRY glTexStorage3DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
  osi.glr->glExt.glTexStorage3DMultisample (target, samples, internalformat, width, height, depth, fixedsamplelocations);}
inline GLAPI void APIENTRY glTextureView (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers) {
  osi.glr->glExt.glTextureView (texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);}
inline GLAPI void APIENTRY glBindVertexBuffer (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
  osi.glr->glExt.glBindVertexBuffer (bindingindex, buffer, offset, stride);}
inline GLAPI void APIENTRY glVertexAttribFormat (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
  osi.glr->glExt.glVertexAttribFormat (attribindex, size, type, normalized, relativeoffset);}
inline GLAPI void APIENTRY glVertexAttribIFormat (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  osi.glr->glExt.glVertexAttribIFormat (attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexAttribLFormat (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  osi.glr->glExt.glVertexAttribLFormat (attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexAttribBinding (GLuint attribindex, GLuint bindingindex) {
  osi.glr->glExt.glVertexAttribBinding (attribindex, bindingindex);}
inline GLAPI void APIENTRY glVertexBindingDivisor (GLuint bindingindex, GLuint divisor) {
  osi.glr->glExt.glVertexBindingDivisor (bindingindex, divisor);}
inline GLAPI void APIENTRY glDebugMessageControl (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) {
  osi.glr->glExt.glDebugMessageControl (source, type, severity, count, ids, enabled);}
inline GLAPI void APIENTRY glDebugMessageInsert (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf) {
  osi.glr->glExt.glDebugMessageInsert (source, type, id, severity, length, buf);}
inline GLAPI void APIENTRY glDebugMessageCallback (GLDEBUGPROC callback, const void *userParam) {
  osi.glr->glExt.glDebugMessageCallback (callback, userParam);}
inline GLAPI GLuint APIENTRY glGetDebugMessageLog (GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog) {
  return osi.glr->glExt.glGetDebugMessageLog (count, bufSize, sources, types, ids, severities, lengths, messageLog);}
inline GLAPI void APIENTRY glPushDebugGroup (GLenum source, GLuint id, GLsizei length, const GLchar *message) {
  osi.glr->glExt.glPushDebugGroup (source, id, length, message);}
inline GLAPI void APIENTRY glPopDebugGroup (void) {
  osi.glr->glExt.glPopDebugGroup ();}
inline GLAPI void APIENTRY glObjectLabel (GLenum identifier, GLuint name, GLsizei length, const GLchar *label) {
  osi.glr->glExt.glObjectLabel (identifier, name, length, label);}
inline GLAPI void APIENTRY glGetObjectLabel (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label) {
  osi.glr->glExt.glGetObjectLabel (identifier, name, bufSize, length, label);}
inline GLAPI void APIENTRY glObjectPtrLabel (const void *ptr, GLsizei length, const GLchar *label) {
  osi.glr->glExt.glObjectPtrLabel (ptr, length, label);}
inline GLAPI void APIENTRY glGetObjectPtrLabel (const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label) {
  osi.glr->glExt.glGetObjectPtrLabel (ptr, bufSize, length, label);}

/// OpenGL 4.4 funcs =================------------------------------
inline GLAPI void APIENTRY glBufferStorage (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags) {
  osi.glr->glExt.glBufferStorage (target, size, data, flags);}
inline GLAPI void APIENTRY glClearTexImage (GLuint texture, GLint level, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glClearTexImage (texture, level, format, type, data);}
inline GLAPI void APIENTRY glClearTexSubImage (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glClearTexSubImage (texture, level, xoffset, yoffset, zoffset, width, height, depth, format,  type, data);}
inline GLAPI void APIENTRY glBindBuffersBase (GLenum target, GLuint first, GLsizei count, const GLuint *buffers) {
  osi.glr->glExt.glBindBuffersBase (target, first, count, buffers);}
inline GLAPI void APIENTRY glBindBuffersRange (GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes) {
  osi.glr->glExt.glBindBuffersRange (target, first, count, buffers, offsets, sizes);}
inline GLAPI void APIENTRY glBindTextures (GLuint first, GLsizei count, const GLuint *textures) {
  osi.glr->glExt.glBindTextures (first, count, textures);}
inline GLAPI void APIENTRY glBindSamplers (GLuint first, GLsizei count, const GLuint *samplers) {
  osi.glr->glExt.glBindSamplers (first, count, samplers);}
inline GLAPI void APIENTRY glBindImageTextures (GLuint first, GLsizei count, const GLuint *textures) {
  osi.glr->glExt.glBindImageTextures (first, count, textures);}
inline GLAPI void APIENTRY glBindVertexBuffers (GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides) {
  osi.glr->glExt.glBindVertexBuffers (first, count, buffers, offsets, strides);}


#ifdef OS_LINUX
///==================///
// linux GLX versions //
///==================///

/// GLX_VERSION_1_3
inline GLXFBConfig *glXGetFBConfigs (Display *dpy, int screen, int *nelements) {
  return osi.glr->glExt.glXGetFBConfigs (dpy, screen, nelements);}
inline GLXFBConfig *glXChooseFBConfig (Display *dpy, int screen, const int *attrib_list, int *nelements) {
  return osi.glr->glExt.glXChooseFBConfig (dpy, screen, attrib_list, nelements);}
inline int glXGetFBConfigAttrib (Display *dpy, GLXFBConfig config, int attribute, int *value) {
  return osi.glr->glExt.glXGetFBConfigAttrib (dpy, config, attribute, value);}
inline XVisualInfo *glXGetVisualFromFBConfig (Display *dpy, GLXFBConfig config) {
  return osi.glr->glExt.glXGetVisualFromFBConfig (dpy, config);}
inline GLXWindow glXCreateWindow (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list) {
  return osi.glr->glExt.glXCreateWindow (dpy, config, win, attrib_list);}
inline void glXDestroyWindow (Display *dpy, GLXWindow win) {
  osi.glr->glExt.glXDestroyWindow (dpy, win);}
inline GLXPixmap glXCreatePixmap (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list) {
  return osi.glr->glExt.glXCreatePixmap (dpy, config, pixmap, attrib_list);}
inline void glXDestroyPixmap (Display *dpy, GLXPixmap pixmap) {
  osi.glr->glExt.glXDestroyPixmap (dpy, pixmap);}
inline GLXPbuffer glXCreatePbuffer (Display *dpy, GLXFBConfig config, const int *attrib_list) {
  return osi.glr->glExt.glXCreatePbuffer (dpy, config, attrib_list);}
inline void glXDestroyPbuffer (Display *dpy, GLXPbuffer pbuf) {
  osi.glr->glExt.glXDestroyPbuffer (dpy, pbuf);}
inline void glXQueryDrawable (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value) {
  osi.glr->glExt.glXQueryDrawable (dpy, draw, attribute, value);}
inline GLXContext glXCreateNewContext (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct) {
  return osi.glr->glExt.glXCreateNewContext (dpy, config, render_type, share_list, direct);}
inline Bool glXMakeContextCurrent (Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
  return osi.glr->glExt.glXMakeContextCurrent (dpy, draw, read, ctx);}
inline GLXDrawable glXGetCurrentReadDrawable (void) {
  return osi.glr->glExt.glXGetCurrentReadDrawable ();}
inline int glXQueryContext (Display *dpy, GLXContext ctx, int attribute, int *value) {
  return osi.glr->glExt.glXQueryContext (dpy, ctx, attribute, value);}
inline void glXSelectEvent (Display *dpy, GLXDrawable draw, unsigned long event_mask) {
  osi.glr->glExt.glXSelectEvent (dpy, draw, event_mask);}
inline void glXGetSelectedEvent (Display *dpy, GLXDrawable draw, unsigned long *event_mask) {
  osi.glr->glExt.glXGetSelectedEvent (dpy, draw, event_mask);}

/// GLX_VERSION_1_4
// well, how can one get the pointer for the function that gets the pointer?
#endif /// OS_LINUX




// extensions not found in oGL registry =====================================
///=====================================-------------------------------------


#ifdef OS_WIN
/// WGL_NV_vertex_array_range
inline void *WINAPI wglAllocateMemoryNV (GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority) {
  return osi.glr->glExt.wglAllocateMemoryNV (size, readfreq, writefreq, priority);}
inline void WINAPI wglFreeMemoryNV (void *pointer) {
  osi.glr->glExt.wglFreeMemoryNV (pointer);}
#endif /// OS_WIN





///==============///
// ARB extensions //
///==============///

// GL_ARB_bindless_texture core 4.4, it is in glext.h, not in OpenGL registry pages tho...
inline GLAPI GLuint64 APIENTRY glGetTextureHandleARB (GLuint texture) {
  return osi.glr->glExt.glGetTextureHandleARB (texture);}
inline GLAPI GLuint64 APIENTRY glGetTextureSamplerHandleARB (GLuint texture, GLuint sampler) {
  return osi.glr->glExt.glGetTextureSamplerHandleARB (texture, sampler);}
inline GLAPI void APIENTRY glMakeTextureHandleResidentARB (GLuint64 handle) {
  osi.glr->glExt.glMakeTextureHandleResidentARB (handle);}
inline GLAPI void APIENTRY glMakeTextureHandleNonResidentARB (GLuint64 handle) {
  osi.glr->glExt.glMakeTextureHandleNonResidentARB (handle);}
inline GLAPI GLuint64 APIENTRY glGetImageHandleARB (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format) {
  return osi.glr->glExt.glGetImageHandleARB (texture, level, layered, layer,  format);}
inline GLAPI void APIENTRY glMakeImageHandleResidentARB (GLuint64 handle, GLenum access) {
  osi.glr->glExt.glMakeImageHandleResidentARB (handle, access);}
inline GLAPI void APIENTRY glMakeImageHandleNonResidentARB (GLuint64 handle) {
  osi.glr->glExt.glMakeImageHandleNonResidentARB (handle);}
inline GLAPI void APIENTRY glUniformHandleui64ARB (GLint location, GLuint64 value) {
  osi.glr->glExt.glUniformHandleui64ARB (location, value);}
inline GLAPI void APIENTRY glUniformHandleui64vARB (GLint location, GLsizei count, const GLuint64 *value) {
  osi.glr->glExt.glUniformHandleui64vARB (location, count, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64ARB (GLuint program, GLint location, GLuint64 value) {
  osi.glr->glExt.glProgramUniformHandleui64ARB (program, location, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64vARB (GLuint program, GLint location, GLsizei count, const GLuint64 *values) {
  osi.glr->glExt.glProgramUniformHandleui64vARB (program, location, count, values);}
inline GLAPI GLboolean APIENTRY glIsTextureHandleResidentARB (GLuint64 handle) {
  return osi.glr->glExt.glIsTextureHandleResidentARB (handle);}
inline GLAPI GLboolean APIENTRY glIsImageHandleResidentARB (GLuint64 handle) {
  return osi.glr->glExt.glIsImageHandleResidentARB (handle);}
inline GLAPI void APIENTRY glVertexAttribL1ui64ARB (GLuint index, GLuint64EXT x) {
  osi.glr->glExt.glVertexAttribL1ui64ARB (index, x);}
inline GLAPI void APIENTRY glVertexAttribL1ui64vARB (GLuint index, const GLuint64EXT *v) {
  osi.glr->glExt.glVertexAttribL1ui64vARB (index, v);}
inline GLAPI void APIENTRY glGetVertexAttribLui64vARB (GLuint index, GLenum pname, GLuint64EXT *params) {
  osi.glr->glExt.glGetVertexAttribLui64vARB (index, pname, params);}
#ifdef OS_LINUX
// GLX_ARB_get_proc_address
// hope this func just works as it is...
//inline __GLXextFuncPtr glXGetProcAddressARB (const GLubyte *procName) {
  //return osi.glr->glExt.glXGetProcAddressARB (procName);}
#endif /// OS_LINUX
// GL_ARB_cl_event
inline GLAPI GLsync APIENTRY glCreateSyncFromCLeventARB (struct _cl_context *context, struct _cl_event *event, GLbitfield flags) {
  return osi.glr->glExt.glCreateSyncFromCLeventARB (context, event, flags);}
// GL_ARB_color_buffer_float
inline GLAPI void APIENTRY glClampColorARB (GLenum target, GLenum clamp) {
  osi.glr->glExt.glClampColorARB (target, clamp);}

#ifdef OS_WIN
//inline BOOL WINAPI wglGetPixelFormatAttribivARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues) {
//  return osi.glr->glExt.wglGetPixelFormatAttribivARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);}
//inline BOOL WINAPI wglGetPixelFormatAttribfvARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues) {
//  return osi.glr->glExt.wglGetPixelFormatAttribfvARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);}
//inline BOOL WINAPI wglChoosePixelFormatARB (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats) {
//  return osi.glr->glExt.wglChoosePixelFormatARB (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);}
#endif /// OS_WIN











// GL_ARB_compute_variable_group_size
inline GLAPI void APIENTRY glDispatchComputeGroupSizeARB (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z) {
  osi.glr->glExt.glDispatchComputeGroupSizeARB (num_groups_x, num_groups_y, num_groups_z, group_size_x, group_size_y, group_size_z);}
// GL_ARB_debug_output
inline GLAPI void APIENTRY glDebugMessageControlARB (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) {
  osi.glr->glExt.glDebugMessageControlARB (source, type, severity, count, ids, enabled);}
inline GLAPI void APIENTRY glDebugMessageInsertARB (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf) {
  osi.glr->glExt.glDebugMessageInsertARB (source, type, id, severity, length, buf);}
inline GLAPI void APIENTRY glDebugMessageCallbackARB (GLDEBUGPROCARB callback, const void *userParam) {
  osi.glr->glExt.glDebugMessageCallbackARB (callback, userParam);}
inline GLAPI GLuint APIENTRY glGetDebugMessageLogARB (GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog) {
  return osi.glr->glExt.glGetDebugMessageLogARB (count, bufSize, sources, types, ids, severities, lengths, messageLog);}
// GL_ARB_draw_buffers
inline GLAPI void APIENTRY glDrawBuffersARB (GLsizei n, const GLenum *bufs) {
  osi.glr->glExt.glDrawBuffersARB (n, bufs);}
// GL_ARB_draw_buffers_blend
inline GLAPI void APIENTRY glBlendEquationiARB (GLuint buf, GLenum mode) {
  osi.glr->glExt.glBlendEquationiARB (buf,  mode);}
inline GLAPI void APIENTRY glBlendEquationSeparateiARB (GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
  osi.glr->glExt.glBlendEquationSeparateiARB (buf, modeRGB, modeAlpha);}
inline GLAPI void APIENTRY glBlendFunciARB (GLuint buf, GLenum src, GLenum dst) {
  osi.glr->glExt.glBlendFunciARB (buf, src, dst);}
inline GLAPI void APIENTRY glBlendFuncSeparateiARB (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  osi.glr->glExt.glBlendFuncSeparateiARB (buf, srcRGB, dstRGB, srcAlpha, dstAlpha);}
// GL_ARB_draw_instanced
inline GLAPI void APIENTRY glDrawArraysInstancedARB (GLenum mode, GLint first, GLsizei count, GLsizei primcount) {
  osi.glr->glExt.glDrawArraysInstancedARB (mode, first, count, primcount);}
inline GLAPI void APIENTRY glDrawElementsInstancedARB (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount) {
  osi.glr->glExt.glDrawElementsInstancedARB (mode, count, type, indices, primcount);}
// GL_ARB_fragment_program
inline GLAPI void APIENTRY glProgramStringARB (GLenum target, GLenum format, GLsizei len, const void *string) {
  osi.glr->glExt.glProgramStringARB (target, format, len, string);}
inline GLAPI void APIENTRY glBindProgramARB (GLenum target, GLuint program) {
  osi.glr->glExt.glBindProgramARB (target, program);}
inline GLAPI void APIENTRY glDeleteProgramsARB (GLsizei n, const GLuint *programs) {
  osi.glr->glExt.glDeleteProgramsARB (n, programs);}
inline GLAPI void APIENTRY glGenProgramsARB (GLsizei n, GLuint *programs) {
  osi.glr->glExt.glGenProgramsARB (n, programs);}
inline GLAPI void APIENTRY glProgramEnvParameter4dARB (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glProgramEnvParameter4dARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameter4dvARB (GLenum target, GLuint index, const GLdouble *params) {
  osi.glr->glExt.glProgramEnvParameter4dvARB (target, index, params);}
inline GLAPI void APIENTRY glProgramEnvParameter4fARB (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glProgramEnvParameter4fARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameter4fvARB (GLenum target, GLuint index, const GLfloat *params) {
  osi.glr->glExt.glProgramEnvParameter4fvARB (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParameter4dARB (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glProgramLocalParameter4dARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameter4dvARB (GLenum target, GLuint index, const GLdouble *params) {
  osi.glr->glExt.glProgramLocalParameter4dvARB (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParameter4fARB (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glProgramLocalParameter4fARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameter4fvARB (GLenum target, GLuint index, const GLfloat *params) {
  osi.glr->glExt.glProgramLocalParameter4fvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterdvARB (GLenum target, GLuint index, GLdouble *params) {
  osi.glr->glExt.glGetProgramEnvParameterdvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterfvARB (GLenum target, GLuint index, GLfloat *params) {
  osi.glr->glExt.glGetProgramEnvParameterfvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterdvARB (GLenum target, GLuint index, GLdouble *params) {
  osi.glr->glExt.glGetProgramLocalParameterdvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterfvARB (GLenum target, GLuint index, GLfloat *params) {
  osi.glr->glExt.glGetProgramLocalParameterfvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramivARB (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetProgramivARB (target, pname, params);}
inline GLAPI void APIENTRY glGetProgramStringARB (GLenum target, GLenum pname, void *string) {
  osi.glr->glExt.glGetProgramStringARB (target, pname, string);}
inline GLAPI GLboolean APIENTRY glIsProgramARB (GLuint program) {
  return osi.glr->glExt.glIsProgramARB (program);}
// GL_ARB_geometry_shader4
inline GLAPI void APIENTRY glProgramParameteriARB (GLuint program, GLenum pname, GLint value) {
  osi.glr->glExt.glProgramParameteriARB (program, pname, value);}
inline GLAPI void APIENTRY glFramebufferTextureARB (GLenum target, GLenum attachment, GLuint texture, GLint level) {
  osi.glr->glExt.glFramebufferTextureARB (target, attachment, texture, level);}
inline GLAPI void APIENTRY glFramebufferTextureLayerARB (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  osi.glr->glExt.glFramebufferTextureLayerARB (target, attachment, texture, level, layer);}
inline GLAPI void APIENTRY glFramebufferTextureFaceARB (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face) {
  osi.glr->glExt.glFramebufferTextureFaceARB (target, attachment, texture, level, face);}
#ifdef OS_WIN
// GL_ARB_imaging - not in openGL registry list
inline GLAPI void APIENTRY glColorTable (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *table) {
  osi.glr->glExt.glColorTable (target, internalformat, width, format, type, table);}
inline GLAPI void APIENTRY glColorTableParameterfv (GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glColorTableParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glColorTableParameteriv (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glColorTableParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glCopyColorTable (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyColorTable (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glGetColorTable (GLenum target, GLenum format, GLenum type, void *table) {
  osi.glr->glExt.glGetColorTable (target, format, type, table);}
inline GLAPI void APIENTRY glGetColorTableParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetColorTableParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetColorTableParameteriv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetColorTableParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glColorSubTable (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glColorSubTable (target, start, count, format, type, data);}
inline GLAPI void APIENTRY glCopyColorSubTable (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyColorSubTable (target, start, x, y, width);}
inline GLAPI void APIENTRY glConvolutionFilter1D (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image) {
  osi.glr->glExt.glConvolutionFilter1D (target, internalformat, width, format, type, image);}
inline GLAPI void APIENTRY glConvolutionFilter2D (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image) {
  osi.glr->glExt.glConvolutionFilter2D (target, internalformat, width, height, format, type, image);}
inline GLAPI void APIENTRY glConvolutionParameterf (GLenum target, GLenum pname, GLfloat params) {
  osi.glr->glExt.glConvolutionParameterf (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameterfv (GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glConvolutionParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameteri (GLenum target, GLenum pname, GLint params) {
  osi.glr->glExt.glConvolutionParameteri (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameteriv (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glConvolutionParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glCopyConvolutionFilter1D (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyConvolutionFilter1D (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glCopyConvolutionFilter2D (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyConvolutionFilter2D (target, internalformat, x, y, width, height);}
inline GLAPI void APIENTRY glGetConvolutionFilter (GLenum target, GLenum format, GLenum type, void *image) {
  osi.glr->glExt.glGetConvolutionFilter (target, format, type, image);}
inline GLAPI void APIENTRY glGetConvolutionParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetConvolutionParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetConvolutionParameteriv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetConvolutionParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetSeparableFilter (GLenum target, GLenum format, GLenum type, void *row, void *column, void *span) {
  osi.glr->glExt.glGetSeparableFilter (target, format, type, row, column, span);}
inline GLAPI void APIENTRY glSeparableFilter2D (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column) {
  osi.glr->glExt.glSeparableFilter2D (target, internalformat, width, height, format, type, row, column);}
inline GLAPI void APIENTRY glGetHistogram (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values) {
  osi.glr->glExt.glGetHistogram (target, reset, format, type, values);}
inline GLAPI void APIENTRY glGetHistogramParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetHistogramParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetHistogramParameteriv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetHistogramParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmax (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values) {
  osi.glr->glExt.glGetMinmax (target, reset, format, type, values);}
inline GLAPI void APIENTRY glGetMinmaxParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMinmaxParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmaxParameteriv (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMinmaxParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glHistogram (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink) {
  osi.glr->glExt.glHistogram (target, width, internalformat, sink);}
inline GLAPI void APIENTRY glMinmax (GLenum target, GLenum internalformat, GLboolean sink) {
  osi.glr->glExt.glMinmax (target, internalformat, sink);}
inline GLAPI void APIENTRY glResetHistogram (GLenum target) {
  osi.glr->glExt.glResetHistogram (target);}
inline GLAPI void APIENTRY glResetMinmax (GLenum target) {
  osi.glr->glExt.glResetMinmax (target);}
#endif /// OS_WIN
// GL_ARB_indirect_parameters
inline GLAPI void APIENTRY glMultiDrawArraysIndirectCountARB (GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
  osi.glr->glExt.glMultiDrawArraysIndirectCountARB (mode, indirect, drawcount, maxdrawcount, stride);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirectCountARB (GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
  osi.glr->glExt.glMultiDrawElementsIndirectCountARB (mode, type, indirect, drawcount, maxdrawcount, stride);}
// GL_ARB_instanced_arrays
inline GLAPI void APIENTRY glVertexAttribDivisorARB (GLuint index, GLuint divisor) {
  osi.glr->glExt.glVertexAttribDivisorARB (index, divisor);}
// GL_ARB_matrix_palette
inline GLAPI void APIENTRY glCurrentPaletteMatrixARB (GLint index) {
  osi.glr->glExt.glCurrentPaletteMatrixARB (index);}
inline GLAPI void APIENTRY glMatrixIndexubvARB (GLint size, const GLubyte *indices) {
  osi.glr->glExt.glMatrixIndexubvARB (size, indices);}
inline GLAPI void APIENTRY glMatrixIndexusvARB (GLint size, const GLushort *indices) {
  osi.glr->glExt.glMatrixIndexusvARB (size, indices);}
inline GLAPI void APIENTRY glMatrixIndexuivARB (GLint size, const GLuint *indices) {
  osi.glr->glExt.glMatrixIndexuivARB (size, indices);}
inline GLAPI void APIENTRY glMatrixIndexPointerARB (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glMatrixIndexPointerARB (size,  type, stride, pointer);}
// GL_ARB_multisample
inline GLAPI void APIENTRY glSampleCoverageARB (GLfloat value, GLboolean invert) {
  osi.glr->glExt.glSampleCoverageARB (value, invert);}
// GL_ARB_multitexture
inline GLAPI void APIENTRY glActiveTextureARB (GLenum texture) {
  osi.glr->glExt.glActiveTextureARB (texture);}
inline GLAPI void APIENTRY glClientActiveTextureARB (GLenum texture) {
  osi.glr->glExt.glClientActiveTextureARB (texture);}
inline GLAPI void APIENTRY glMultiTexCoord1dARB (GLenum target, GLdouble s) {
  osi.glr->glExt.glMultiTexCoord1dARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1dvARB (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord1dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord1fARB (GLenum target, GLfloat s) {
  osi.glr->glExt.glMultiTexCoord1fARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1fvARB (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord1fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord1iARB (GLenum target, GLint s) {
  osi.glr->glExt.glMultiTexCoord1iARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1ivARB (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord1ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord1sARB (GLenum target, GLshort s) {
  osi.glr->glExt.glMultiTexCoord1sARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1svARB (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord1svARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2dARB (GLenum target, GLdouble s, GLdouble t) {
  osi.glr->glExt.glMultiTexCoord2dARB (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2dvARB (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord2dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2fARB (GLenum target, GLfloat s, GLfloat t) {
  osi.glr->glExt.glMultiTexCoord2fARB (target,  s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2fvARB (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord2fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2iARB (GLenum target, GLint s, GLint t) {
  osi.glr->glExt.glMultiTexCoord2iARB (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2ivARB (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord2ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2sARB (GLenum target, GLshort s, GLshort t) {
  osi.glr->glExt.glMultiTexCoord2sARB (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2svARB (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord2svARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r) {
  osi.glr->glExt.glMultiTexCoord3dARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3dvARB (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord3dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r) {
  osi.glr->glExt.glMultiTexCoord3fARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3fvARB (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord3fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3iARB (GLenum target, GLint s, GLint t, GLint r) {
  osi.glr->glExt.glMultiTexCoord3iARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3ivARB (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord3ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3sARB (GLenum target, GLshort s, GLshort t, GLshort r) {
  osi.glr->glExt.glMultiTexCoord3sARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3svARB (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord3svARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q) {
  osi.glr->glExt.glMultiTexCoord4dARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4dvARB (GLenum target, const GLdouble *v) {
  osi.glr->glExt.glMultiTexCoord4dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
  osi.glr->glExt.glMultiTexCoord4fARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4fvARB (GLenum target, const GLfloat *v) {
  osi.glr->glExt.glMultiTexCoord4fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4iARB (GLenum target, GLint s, GLint t, GLint r, GLint q) {
  osi.glr->glExt.glMultiTexCoord4iARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4ivARB (GLenum target, const GLint *v) {
  osi.glr->glExt.glMultiTexCoord4ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4sARB (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q) {
  osi.glr->glExt.glMultiTexCoord4sARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4svARB (GLenum target, const GLshort *v) {
  osi.glr->glExt.glMultiTexCoord4svARB (target, v);}
// GL_ARB_occlusion_query
inline GLAPI void APIENTRY glGenQueriesARB (GLsizei n, GLuint *ids) {
  osi.glr->glExt.glGenQueriesARB (n, ids);}
inline GLAPI void APIENTRY glDeleteQueriesARB (GLsizei n, const GLuint *ids) {
  osi.glr->glExt.glDeleteQueriesARB (n, ids);}
inline GLAPI GLboolean APIENTRY glIsQueryARB (GLuint id) {
  return osi.glr->glExt.glIsQueryARB (id);}
inline GLAPI void APIENTRY glBeginQueryARB (GLenum target, GLuint id) {
  osi.glr->glExt.glBeginQueryARB (target,  id);}
inline GLAPI void APIENTRY glEndQueryARB (GLenum target) {
  osi.glr->glExt.glEndQueryARB (target);}
inline GLAPI void APIENTRY glGetQueryivARB (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetQueryivARB (target, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectivARB (GLuint id, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetQueryObjectivARB (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectuivARB (GLuint id, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetQueryObjectuivARB (id, pname, params);}
// GL_ARB_point_parameters
inline GLAPI void APIENTRY glPointParameterfARB (GLenum pname, GLfloat param) {
  osi.glr->glExt.glPointParameterfARB (pname, param);}
inline GLAPI void APIENTRY glPointParameterfvARB (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glPointParameterfvARB (pname, params);}
// GL_ARB_robustness
inline GLAPI GLenum APIENTRY glGetGraphicsResetStatusARB (void) {
  return osi.glr->glExt.glGetGraphicsResetStatusARB ();}
inline GLAPI void APIENTRY glGetnTexImageARB (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *img) {
  osi.glr->glExt.glGetnTexImageARB (target, level, format, type, bufSize, img);}
inline GLAPI void APIENTRY glReadnPixelsARB (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data) {
  osi.glr->glExt.glReadnPixelsARB (x, y, width, height, format, type, bufSize, data);}
inline GLAPI void APIENTRY glGetnCompressedTexImageARB (GLenum target, GLint lod, GLsizei bufSize, void *img) {
  osi.glr->glExt.glGetnCompressedTexImageARB (target, lod, bufSize, img);}
inline GLAPI void APIENTRY glGetnUniformfvARB (GLuint program, GLint location, GLsizei bufSize, GLfloat *params) {
  osi.glr->glExt.glGetnUniformfvARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnUniformivARB (GLuint program, GLint location, GLsizei bufSize, GLint *params) {
  osi.glr->glExt.glGetnUniformivARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnUniformuivARB (GLuint program, GLint location, GLsizei bufSize, GLuint *params) {
  osi.glr->glExt.glGetnUniformuivARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnUniformdvARB (GLuint program, GLint location, GLsizei bufSize, GLdouble *params) {
  osi.glr->glExt.glGetnUniformdvARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnMapdvARB (GLenum target, GLenum query, GLsizei bufSize, GLdouble *v) {
  osi.glr->glExt.glGetnMapdvARB (target, query, bufSize, v);}
inline GLAPI void APIENTRY glGetnMapfvARB (GLenum target, GLenum query, GLsizei bufSize, GLfloat *v) {
  osi.glr->glExt.glGetnMapfvARB (target, query, bufSize, v);}
inline GLAPI void APIENTRY glGetnMapivARB (GLenum target, GLenum query, GLsizei bufSize, GLint *v) {
  osi.glr->glExt.glGetnMapivARB (target, query, bufSize, v);}
inline GLAPI void APIENTRY glGetnPixelMapfvARB (GLenum map, GLsizei bufSize, GLfloat *values) {
  osi.glr->glExt.glGetnPixelMapfvARB (map, bufSize, values);}
inline GLAPI void APIENTRY glGetnPixelMapuivARB (GLenum map, GLsizei bufSize, GLuint *values) {
  osi.glr->glExt.glGetnPixelMapuivARB (map, bufSize, values);}
inline GLAPI void APIENTRY glGetnPixelMapusvARB (GLenum map, GLsizei bufSize, GLushort *values) {
  osi.glr->glExt.glGetnPixelMapusvARB (map, bufSize, values);}
inline GLAPI void APIENTRY glGetnPolygonStippleARB (GLsizei bufSize, GLubyte *pattern) {
  osi.glr->glExt.glGetnPolygonStippleARB (bufSize, pattern);}
inline GLAPI void APIENTRY glGetnColorTableARB (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table) {
  osi.glr->glExt.glGetnColorTableARB (target, format, type, bufSize, table);}
inline GLAPI void APIENTRY glGetnConvolutionFilterARB (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image) {
  osi.glr->glExt.glGetnConvolutionFilterARB (target, format, type, bufSize, image);}
inline GLAPI void APIENTRY glGetnSeparableFilterARB (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span) {
  osi.glr->glExt.glGetnSeparableFilterARB (target, format, type, rowBufSize, row, columnBufSize, column, span);}
inline GLAPI void APIENTRY glGetnHistogramARB (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) {
  osi.glr->glExt.glGetnHistogramARB (target, reset, format, type, bufSize, values);}
inline GLAPI void APIENTRY glGetnMinmaxARB (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) {
  osi.glr->glExt.glGetnMinmaxARB (target, reset, format, type, bufSize, values);}
// GL_ARB_sample_shading
inline GLAPI void APIENTRY glMinSampleShadingARB (GLfloat value) {
  osi.glr->glExt.glMinSampleShadingARB (value);}
// GL_ARB_shader_objects
inline GLAPI void APIENTRY glDeleteObjectARB (GLhandleARB obj) {
  osi.glr->glExt.glDeleteObjectARB (obj);}
inline GLAPI GLhandleARB APIENTRY glGetHandleARB (GLenum pname) {
  return osi.glr->glExt.glGetHandleARB (pname);}
inline GLAPI void APIENTRY glDetachObjectARB (GLhandleARB containerObj, GLhandleARB attachedObj) {
  osi.glr->glExt.glDetachObjectARB (containerObj, attachedObj);}
inline GLAPI GLhandleARB APIENTRY glCreateShaderObjectARB (GLenum shaderType) {
  return osi.glr->glExt.glCreateShaderObjectARB (shaderType);}
inline GLAPI void APIENTRY glShaderSourceARB (GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length) {
  osi.glr->glExt.glShaderSourceARB (shaderObj, count, string, length);}
inline GLAPI void APIENTRY glCompileShaderARB (GLhandleARB shaderObj) {
  osi.glr->glExt.glCompileShaderARB (shaderObj);}
inline GLAPI GLhandleARB APIENTRY glCreateProgramObjectARB (void) {
  return osi.glr->glExt.glCreateProgramObjectARB ();}
inline GLAPI void APIENTRY glAttachObjectARB (GLhandleARB containerObj, GLhandleARB obj) {
  osi.glr->glExt.glAttachObjectARB (containerObj, obj);}
inline GLAPI void APIENTRY glLinkProgramARB (GLhandleARB programObj) {
  osi.glr->glExt.glLinkProgramARB (programObj);}
inline GLAPI void APIENTRY glUseProgramObjectARB (GLhandleARB programObj) {
  osi.glr->glExt.glUseProgramObjectARB (programObj);}
inline GLAPI void APIENTRY glValidateProgramARB (GLhandleARB programObj) {
  osi.glr->glExt.glValidateProgramARB (programObj);}
inline GLAPI void APIENTRY glUniform1fARB (GLint location, GLfloat v0) {
  osi.glr->glExt.glUniform1fARB (location, v0);}
inline GLAPI void APIENTRY glUniform2fARB (GLint location, GLfloat v0, GLfloat v1) {
  osi.glr->glExt.glUniform2fARB (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3fARB (GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  osi.glr->glExt.glUniform3fARB (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4fARB (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  osi.glr->glExt.glUniform4fARB (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1iARB (GLint location, GLint v0) {
  osi.glr->glExt.glUniform1iARB (location, v0);}
inline GLAPI void APIENTRY glUniform2iARB (GLint location, GLint v0, GLint v1) {
  osi.glr->glExt.glUniform2iARB (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3iARB (GLint location, GLint v0, GLint v1, GLint v2) {
  osi.glr->glExt.glUniform3iARB (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4iARB (GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  osi.glr->glExt.glUniform4iARB (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1fvARB (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform1fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform2fvARB (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform2fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform3fvARB (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform3fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform4fvARB (GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glUniform4fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform1ivARB (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform1ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniform2ivARB (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform2ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniform3ivARB (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform3ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniform4ivARB (GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glUniform4ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniformMatrix2fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix2fvARB (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix3fvARB (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glUniformMatrix4fvARB (location, count, transpose, value);}
inline GLAPI void APIENTRY glGetObjectParameterfvARB (GLhandleARB obj, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetObjectParameterfvARB (obj, pname, params);}
inline GLAPI void APIENTRY glGetObjectParameterivARB (GLhandleARB obj, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetObjectParameterivARB (obj, pname, params);}
inline GLAPI void APIENTRY glGetInfoLogARB (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog) {
  osi.glr->glExt.glGetInfoLogARB (obj, maxLength, length, infoLog);}
inline GLAPI void APIENTRY glGetAttachedObjectsARB (GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj) {
  osi.glr->glExt.glGetAttachedObjectsARB (containerObj, maxCount, count, obj);}
inline GLAPI GLint APIENTRY glGetUniformLocationARB (GLhandleARB programObj, const GLcharARB *name) {
  return osi.glr->glExt.glGetUniformLocationARB (programObj, name);}
inline GLAPI void APIENTRY glGetActiveUniformARB (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name) {
  osi.glr->glExt.glGetActiveUniformARB (programObj, index, maxLength, length, size, type, name);}
inline GLAPI void APIENTRY glGetUniformfvARB (GLhandleARB programObj, GLint location, GLfloat *params) {
  osi.glr->glExt.glGetUniformfvARB (programObj, location, params);}
inline GLAPI void APIENTRY glGetUniformivARB (GLhandleARB programObj, GLint location, GLint *params) {
  osi.glr->glExt.glGetUniformivARB (programObj, location, params);}
inline GLAPI void APIENTRY glGetShaderSourceARB (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source) {
  osi.glr->glExt.glGetShaderSourceARB (obj, maxLength, length, source);}
// GL_ARB_shading_language_include
inline GLAPI void APIENTRY glNamedStringARB (GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string) {
  osi.glr->glExt.glNamedStringARB (type, namelen, name, stringlen, string);}
inline GLAPI void APIENTRY glDeleteNamedStringARB (GLint namelen, const GLchar *name) {
  osi.glr->glExt.glDeleteNamedStringARB (namelen, name);}
inline GLAPI void APIENTRY glCompileShaderIncludeARB (GLuint shader, GLsizei count, const GLchar *const*path, const GLint *length) {
  osi.glr->glExt.glCompileShaderIncludeARB (shader, count, path, length);}
inline GLAPI GLboolean APIENTRY glIsNamedStringARB (GLint namelen, const GLchar *name) {
  return osi.glr->glExt.glIsNamedStringARB (namelen, name);}
inline GLAPI void APIENTRY glGetNamedStringARB (GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string) {
  osi.glr->glExt.glGetNamedStringARB (namelen, name, bufSize, stringlen, string);}
inline GLAPI void APIENTRY glGetNamedStringivARB (GLint namelen, const GLchar *name, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetNamedStringivARB (namelen, name, pname, params);}
// GL_ARB_sparse_texture
inline GLAPI void APIENTRY glTexPageCommitmentARB (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean resident) {
  osi.glr->glExt.glTexPageCommitmentARB (target, level, xoffset, yoffset, zoffset, width, height, depth, resident);}
// GL_ARB_texture_buffer_object
inline GLAPI void APIENTRY glTexBufferARB (GLenum target, GLenum internalformat, GLuint buffer) {
  osi.glr->glExt.glTexBufferARB (target, internalformat, buffer);}
// GL_ARB_texture_compression
inline GLAPI void APIENTRY glCompressedTexImage3DARB (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexImage3DARB (target, level, internalformat, width, height, depth, border, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexImage2DARB (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexImage2DARB (target, level, internalformat, width, height, border, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexImage1DARB (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexImage1DARB (target, level, internalformat, width, border, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexSubImage3DARB (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexSubImage3DARB (target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexSubImage2DARB (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexSubImage2DARB (target, level, xoffset, yoffset, width, height, format, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexSubImage1DARB (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) {
  osi.glr->glExt.glCompressedTexSubImage1DARB (target, level, xoffset, width, format, imageSize, data);}
inline GLAPI void APIENTRY glGetCompressedTexImageARB (GLenum target, GLint level, void *img) {
  osi.glr->glExt.glGetCompressedTexImageARB (target, level, img);}
// GL_ARB_transpose_matrix
inline GLAPI void APIENTRY glLoadTransposeMatrixfARB (const GLfloat *m) {
  osi.glr->glExt.glLoadTransposeMatrixfARB (m);}
inline GLAPI void APIENTRY glLoadTransposeMatrixdARB (const GLdouble *m) {
  osi.glr->glExt.glLoadTransposeMatrixdARB (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixfARB (const GLfloat *m) {
  osi.glr->glExt.glMultTransposeMatrixfARB (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixdARB (const GLdouble *m) {
  osi.glr->glExt.glMultTransposeMatrixdARB (m);}
// GL_ARB_vertex_blend
inline GLAPI void APIENTRY glWeightbvARB (GLint size, const GLbyte *weights) {
  osi.glr->glExt.glWeightbvARB (size, weights);}
inline GLAPI void APIENTRY glWeightsvARB (GLint size, const GLshort *weights) {
  osi.glr->glExt.glWeightsvARB (size, weights);}
inline GLAPI void APIENTRY glWeightivARB (GLint size, const GLint *weights) {
  osi.glr->glExt.glWeightivARB (size, weights);}
inline GLAPI void APIENTRY glWeightfvARB (GLint size, const GLfloat *weights) {
  osi.glr->glExt.glWeightfvARB (size, weights);}
inline GLAPI void APIENTRY glWeightdvARB (GLint size, const GLdouble *weights) {
  osi.glr->glExt.glWeightdvARB (size, weights);}
inline GLAPI void APIENTRY glWeightubvARB (GLint size, const GLubyte *weights) {
  osi.glr->glExt.glWeightubvARB (size, weights);}
inline GLAPI void APIENTRY glWeightusvARB (GLint size, const GLushort *weights) {
  osi.glr->glExt.glWeightusvARB (size, weights);}
inline GLAPI void APIENTRY glWeightuivARB (GLint size, const GLuint *weights) {
  osi.glr->glExt.glWeightuivARB (size, weights);}
inline GLAPI void APIENTRY glWeightPointerARB (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glWeightPointerARB (size, type, stride, pointer);}
inline GLAPI void APIENTRY glVertexBlendARB (GLint count) {
  osi.glr->glExt.glVertexBlendARB (count);}
// GL_ARB_vertex_buffer_object
inline GLAPI void APIENTRY glBindBufferARB (GLenum target, GLuint buffer) {
  osi.glr->glExt.glBindBufferARB (target, buffer);}
inline GLAPI void APIENTRY glDeleteBuffersARB (GLsizei n, const GLuint *buffers) {
  osi.glr->glExt.glDeleteBuffersARB (n, buffers);}
inline GLAPI void APIENTRY glGenBuffersARB (GLsizei n, GLuint *buffers) {
  osi.glr->glExt.glGenBuffersARB (n, buffers);}
inline GLAPI GLboolean APIENTRY glIsBufferARB (GLuint buffer) {
  return osi.glr->glExt.glIsBufferARB (buffer);}
inline GLAPI void APIENTRY glBufferDataARB (GLenum target, GLsizeiptrARB size, const void *data, GLenum usage) {
  osi.glr->glExt.glBufferDataARB (target, size, data, usage);}
inline GLAPI void APIENTRY glBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void *data) {
  osi.glr->glExt.glBufferSubDataARB (target, offset, size, data);}
inline GLAPI void APIENTRY glGetBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, void *data) {
  osi.glr->glExt.glGetBufferSubDataARB (target, offset, size, data);}
inline GLAPI void *APIENTRY glMapBufferARB (GLenum target, GLenum access) {
  return osi.glr->glExt.glMapBufferARB (target, access);}
inline GLAPI GLboolean APIENTRY glUnmapBufferARB (GLenum target) {
  return osi.glr->glExt.glUnmapBufferARB (target);}
inline GLAPI void APIENTRY glGetBufferParameterivARB (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetBufferParameterivARB (target, pname, params);}
inline GLAPI void APIENTRY glGetBufferPointervARB (GLenum target, GLenum pname, void **params) {
  osi.glr->glExt.glGetBufferPointervARB (target, pname, params);}
// GL_ARB_vertex_program
inline GLAPI void APIENTRY glVertexAttrib1dARB (GLuint index, GLdouble x) {
  osi.glr->glExt.glVertexAttrib1dARB (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1dvARB (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib1dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1fARB (GLuint index, GLfloat x) {
  osi.glr->glExt.glVertexAttrib1fARB (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1fvARB (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib1fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1sARB (GLuint index, GLshort x) {
  osi.glr->glExt.glVertexAttrib1sARB (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1svARB (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib1svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2dARB (GLuint index, GLdouble x, GLdouble y) {
  osi.glr->glExt.glVertexAttrib2dARB (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2dvARB (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib2dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2fARB (GLuint index, GLfloat x, GLfloat y) {
  osi.glr->glExt.glVertexAttrib2fARB (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2fvARB (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib2fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2sARB (GLuint index, GLshort x, GLshort y) {
  osi.glr->glExt.glVertexAttrib2sARB (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2svARB (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib2svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3dARB (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glVertexAttrib3dARB (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3dvARB (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib3dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3fARB (GLuint index, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glVertexAttrib3fARB (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3fvARB (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib3fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3sARB (GLuint index, GLshort x, GLshort y, GLshort z) {
  osi.glr->glExt.glVertexAttrib3sARB (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3svARB (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib3svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NbvARB (GLuint index, const GLbyte *v) {
  osi.glr->glExt.glVertexAttrib4NbvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NivARB (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttrib4NivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NsvARB (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib4NsvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NubARB (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
  osi.glr->glExt.glVertexAttrib4NubARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4NubvARB (GLuint index, const GLubyte *v) {
  osi.glr->glExt.glVertexAttrib4NubvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NuivARB (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttrib4NuivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NusvARB (GLuint index, const GLushort *v) {
  osi.glr->glExt.glVertexAttrib4NusvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4bvARB (GLuint index, const GLbyte *v) {
  osi.glr->glExt.glVertexAttrib4bvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4dARB (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glVertexAttrib4dARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4dvARB (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib4dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4fARB (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glVertexAttrib4fARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4fvARB (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib4fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ivARB (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttrib4ivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4sARB (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
  osi.glr->glExt.glVertexAttrib4sARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4svARB (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib4svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ubvARB (GLuint index, const GLubyte *v) {
  osi.glr->glExt.glVertexAttrib4ubvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4uivARB (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttrib4uivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4usvARB (GLuint index, const GLushort *v) {
  osi.glr->glExt.glVertexAttrib4usvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttribPointerARB (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexAttribPointerARB (index, size, type, normalized, stride, pointer);}
inline GLAPI void APIENTRY glEnableVertexAttribArrayARB (GLuint index) {
  osi.glr->glExt.glEnableVertexAttribArrayARB (index);}
inline GLAPI void APIENTRY glDisableVertexAttribArrayARB (GLuint index) {
  osi.glr->glExt.glDisableVertexAttribArrayARB (index);}
inline GLAPI void APIENTRY glGetVertexAttribdvARB (GLuint index, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetVertexAttribdvARB (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribfvARB (GLuint index, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetVertexAttribfvARB (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribivARB (GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVertexAttribivARB (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribPointervARB (GLuint index, GLenum pname, void **pointer) {
  osi.glr->glExt.glGetVertexAttribPointervARB (index, pname, pointer);}
// GL_ARB_vertex_shader
inline GLAPI void APIENTRY glBindAttribLocationARB (GLhandleARB programObj, GLuint index, const GLcharARB *name) {
  osi.glr->glExt.glBindAttribLocationARB (programObj, index, name);}
inline GLAPI void APIENTRY glGetActiveAttribARB (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name) {
  osi.glr->glExt.glGetActiveAttribARB (programObj, index, maxLength, length, size, type, name);}
inline GLAPI GLint APIENTRY glGetAttribLocationARB (GLhandleARB programObj, const GLcharARB *name) {
  return osi.glr->glExt.glGetAttribLocationARB (programObj, name);}
// GL_ARB_window_pos
inline GLAPI void APIENTRY glWindowPos2dARB (GLdouble x, GLdouble y) {
  osi.glr->glExt.glWindowPos2dARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2dvARB (const GLdouble *v) {
  osi.glr->glExt.glWindowPos2dvARB (v);}
inline GLAPI void APIENTRY glWindowPos2fARB (GLfloat x, GLfloat y) {
  osi.glr->glExt.glWindowPos2fARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2fvARB (const GLfloat *v) {
  osi.glr->glExt.glWindowPos2fvARB (v);}
inline GLAPI void APIENTRY glWindowPos2iARB (GLint x, GLint y) {
  osi.glr->glExt.glWindowPos2iARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2ivARB (const GLint *v) {
  osi.glr->glExt.glWindowPos2ivARB (v);}
inline GLAPI void APIENTRY glWindowPos2sARB (GLshort x, GLshort y) {
  osi.glr->glExt.glWindowPos2sARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2svARB (const GLshort *v) {
  osi.glr->glExt.glWindowPos2svARB (v);}
inline GLAPI void APIENTRY glWindowPos3dARB (GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glWindowPos3dARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3dvARB (const GLdouble *v) {
  osi.glr->glExt.glWindowPos3dvARB (v);}
inline GLAPI void APIENTRY glWindowPos3fARB (GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glWindowPos3fARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3fvARB (const GLfloat *v) {
  osi.glr->glExt.glWindowPos3fvARB (v);}
inline GLAPI void APIENTRY glWindowPos3iARB (GLint x, GLint y, GLint z) {
  osi.glr->glExt.glWindowPos3iARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3ivARB (const GLint *v) {
  osi.glr->glExt.glWindowPos3ivARB (v);}
inline GLAPI void APIENTRY glWindowPos3sARB (GLshort x, GLshort y, GLshort z) {
  osi.glr->glExt.glWindowPos3sARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3svARB (const GLshort *v) {
  osi.glr->glExt.glWindowPos3svARB (v);}









///=======================///
// EXT & vendor extensions //
///=======================///


// GL_OES_byte_coordinates
inline GLAPI void APIENTRY glMultiTexCoord1bOES (GLenum texture, GLbyte s) {
  osi.glr->glExt.glMultiTexCoord1bOES (texture, s);}
inline GLAPI void APIENTRY glMultiTexCoord1bvOES (GLenum texture, const GLbyte *coords) {
  osi.glr->glExt.glMultiTexCoord1bvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord2bOES (GLenum texture, GLbyte s, GLbyte t) {
  osi.glr->glExt.glMultiTexCoord2bOES (texture, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2bvOES (GLenum texture, const GLbyte *coords) {
  osi.glr->glExt.glMultiTexCoord2bvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord3bOES (GLenum texture, GLbyte s, GLbyte t, GLbyte r) {
  osi.glr->glExt.glMultiTexCoord3bOES (texture, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3bvOES (GLenum texture, const GLbyte *coords) {
  osi.glr->glExt.glMultiTexCoord3bvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord4bOES (GLenum texture, GLbyte s, GLbyte t, GLbyte r, GLbyte q) {
  osi.glr->glExt.glMultiTexCoord4bOES (texture, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4bvOES (GLenum texture, const GLbyte *coords) {
  osi.glr->glExt.glMultiTexCoord4bvOES (texture, coords);}
inline GLAPI void APIENTRY glTexCoord1bOES (GLbyte s) {
  osi.glr->glExt.glTexCoord1bOES (s);}
inline GLAPI void APIENTRY glTexCoord1bvOES (const GLbyte *coords) {
  osi.glr->glExt.glTexCoord1bvOES (coords);}
inline GLAPI void APIENTRY glTexCoord2bOES (GLbyte s, GLbyte t) {
  osi.glr->glExt.glTexCoord2bOES (s, t);}
inline GLAPI void APIENTRY glTexCoord2bvOES (const GLbyte *coords) {
  osi.glr->glExt.glTexCoord2bvOES (coords);}
inline GLAPI void APIENTRY glTexCoord3bOES (GLbyte s, GLbyte t, GLbyte r) {
  osi.glr->glExt.glTexCoord3bOES (s, t, r);}
inline GLAPI void APIENTRY glTexCoord3bvOES (const GLbyte *coords) {
  osi.glr->glExt.glTexCoord3bvOES (coords);}
inline GLAPI void APIENTRY glTexCoord4bOES (GLbyte s, GLbyte t, GLbyte r, GLbyte q) {
  osi.glr->glExt.glTexCoord4bOES (s, t, r,  q);}
inline GLAPI void APIENTRY glTexCoord4bvOES (const GLbyte *coords) {
  osi.glr->glExt.glTexCoord4bvOES (coords);}
inline GLAPI void APIENTRY glVertex2bOES (GLbyte x) {
  osi.glr->glExt.glVertex2bOES (x);}
inline GLAPI void APIENTRY glVertex2bvOES (const GLbyte *coords) {
  osi.glr->glExt.glVertex2bvOES (coords);}
inline GLAPI void APIENTRY glVertex3bOES (GLbyte x, GLbyte y) {
  osi.glr->glExt.glVertex3bOES (x, y);}
inline GLAPI void APIENTRY glVertex3bvOES (const GLbyte *coords) {
  osi.glr->glExt.glVertex3bvOES (coords);}
inline GLAPI void APIENTRY glVertex4bOES (GLbyte x, GLbyte y, GLbyte z) {
  osi.glr->glExt.glVertex4bOES (x, y, z);}
inline GLAPI void APIENTRY glVertex4bvOES (const GLbyte *coords) {
  osi.glr->glExt.glVertex4bvOES (coords);}
// GL_OES_fixed_point
inline GLAPI void APIENTRY glAlphaFuncxOES (GLenum func, GLfixed ref) {
  osi.glr->glExt.glAlphaFuncxOES (func, ref);}
inline GLAPI void APIENTRY glClearColorxOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  osi.glr->glExt.glClearColorxOES (red, green, blue, alpha);}
inline GLAPI void APIENTRY glClearDepthxOES (GLfixed depth) {
  osi.glr->glExt.glClearDepthxOES (depth);}
inline GLAPI void APIENTRY glClipPlanexOES (GLenum plane, const GLfixed *equation) {
  osi.glr->glExt.glClipPlanexOES (plane, equation);}
inline GLAPI void APIENTRY glColor4xOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  osi.glr->glExt.glColor4xOES (red, green, blue, alpha);}
inline GLAPI void APIENTRY glDepthRangexOES (GLfixed n, GLfixed f) {
  osi.glr->glExt.glDepthRangexOES (n, f);}
inline GLAPI void APIENTRY glFogxOES (GLenum pname, GLfixed param) {
  osi.glr->glExt.glFogxOES (pname, param);}
inline GLAPI void APIENTRY glFogxvOES (GLenum pname, const GLfixed *param) {
  osi.glr->glExt.glFogxvOES (pname, param);}
inline GLAPI void APIENTRY glFrustumxOES (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f) {
  osi.glr->glExt.glFrustumxOES (l, r, b, t, n, f);}
inline GLAPI void APIENTRY glGetClipPlanexOES (GLenum plane, GLfixed *equation) {
  osi.glr->glExt.glGetClipPlanexOES (plane, equation);}
inline GLAPI void APIENTRY glGetFixedvOES (GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetFixedvOES (pname, params);}
inline GLAPI void APIENTRY glGetTexEnvxvOES (GLenum target, GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetTexEnvxvOES (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterxvOES (GLenum target, GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetTexParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glLightModelxOES (GLenum pname, GLfixed param) {
  osi.glr->glExt.glLightModelxOES (pname, param);}
inline GLAPI void APIENTRY glLightModelxvOES (GLenum pname, const GLfixed *param) {
  osi.glr->glExt.glLightModelxvOES (pname, param);}
inline GLAPI void APIENTRY glLightxOES (GLenum light, GLenum pname, GLfixed param) {
  osi.glr->glExt.glLightxOES (light, pname, param);}
inline GLAPI void APIENTRY glLightxvOES (GLenum light, GLenum pname, const GLfixed *params) {
  osi.glr->glExt.glLightxvOES (light, pname, params);}
inline GLAPI void APIENTRY glLineWidthxOES (GLfixed width) {
  osi.glr->glExt.glLineWidthxOES (width);}
inline GLAPI void APIENTRY glLoadMatrixxOES (const GLfixed *m) {
  osi.glr->glExt.glLoadMatrixxOES (m);}
inline GLAPI void APIENTRY glMaterialxOES (GLenum face, GLenum pname, GLfixed param) {
  osi.glr->glExt.glMaterialxOES (face, pname, param);}
inline GLAPI void APIENTRY glMaterialxvOES (GLenum face, GLenum pname, const GLfixed *param) {
  osi.glr->glExt.glMaterialxvOES (face, pname, param);}
inline GLAPI void APIENTRY glMultMatrixxOES (const GLfixed *m) {
  osi.glr->glExt.glMultMatrixxOES (m);}
inline GLAPI void APIENTRY glMultiTexCoord4xOES (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q) {
  osi.glr->glExt.glMultiTexCoord4xOES (texture, s, t, r, q);}
inline GLAPI void APIENTRY glNormal3xOES (GLfixed nx, GLfixed ny, GLfixed nz) {
  osi.glr->glExt.glNormal3xOES (nx, ny, nz);}
inline GLAPI void APIENTRY glOrthoxOES (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f) {
  osi.glr->glExt.glOrthoxOES (l, r, b, t, n, f);}
inline GLAPI void APIENTRY glPointParameterxvOES (GLenum pname, const GLfixed *params) {
  osi.glr->glExt.glPointParameterxvOES (pname, params);}
inline GLAPI void APIENTRY glPointSizexOES (GLfixed size) {
  osi.glr->glExt.glPointSizexOES (size);}
inline GLAPI void APIENTRY glPolygonOffsetxOES (GLfixed factor, GLfixed units) {
  osi.glr->glExt.glPolygonOffsetxOES (factor, units);}
inline GLAPI void APIENTRY glRotatexOES (GLfixed angle, GLfixed x, GLfixed y, GLfixed z) {
  osi.glr->glExt.glRotatexOES (angle, x, y, z);}
inline GLAPI void APIENTRY glSampleCoverageOES (GLfixed value, GLboolean invert) { 
  osi.glr->glExt.glSampleCoverageOES (value, invert);}
inline GLAPI void APIENTRY glScalexOES (GLfixed x, GLfixed y, GLfixed z) {
  osi.glr->glExt.glScalexOES (x, y, z);}
inline GLAPI void APIENTRY glTexEnvxOES (GLenum target, GLenum pname, GLfixed param) {
  osi.glr->glExt.glTexEnvxOES (target, pname, param);}
inline GLAPI void APIENTRY glTexEnvxvOES (GLenum target, GLenum pname, const GLfixed *params) {
  osi.glr->glExt.glTexEnvxvOES (target, pname, params);}
inline GLAPI void APIENTRY glTexParameterxOES (GLenum target, GLenum pname, GLfixed param) {
  osi.glr->glExt.glTexParameterxOES (target, pname, param);}
inline GLAPI void APIENTRY glTexParameterxvOES (GLenum target, GLenum pname, const GLfixed *params) {
  osi.glr->glExt.glTexParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glTranslatexOES (GLfixed x, GLfixed y, GLfixed z) {
  osi.glr->glExt.glTranslatexOES (x, y, z);}
inline GLAPI void APIENTRY glAccumxOES (GLenum op, GLfixed value) {
  osi.glr->glExt.glAccumxOES (op, value);}
inline GLAPI void APIENTRY glBitmapxOES (GLsizei width, GLsizei height, GLfixed xorig, GLfixed yorig, GLfixed xmove, GLfixed ymove, const GLubyte *bitmap) {
  osi.glr->glExt.glBitmapxOES (width, height, xorig, yorig, xmove, ymove, bitmap);}
inline GLAPI void APIENTRY glBlendColorxOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  osi.glr->glExt.glBlendColorxOES (red, green, blue, alpha);}
inline GLAPI void APIENTRY glClearAccumxOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  osi.glr->glExt.glClearAccumxOES (red, green, blue,  alpha);}
inline GLAPI void APIENTRY glColor3xOES (GLfixed red, GLfixed green, GLfixed blue) {
  osi.glr->glExt.glColor3xOES (red, green,  blue);}
inline GLAPI void APIENTRY glColor3xvOES (const GLfixed *components) {
  osi.glr->glExt.glColor3xvOES (components);}
inline GLAPI void APIENTRY glColor4xvOES (const GLfixed *components) {
  osi.glr->glExt.glColor4xvOES (components);}
inline GLAPI void APIENTRY glConvolutionParameterxOES (GLenum target, GLenum pname, GLfixed param) {
  osi.glr->glExt.glConvolutionParameterxOES (target, pname,  param);}
inline GLAPI void APIENTRY glConvolutionParameterxvOES (GLenum target, GLenum pname, const GLfixed *params) {
  osi.glr->glExt.glConvolutionParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glEvalCoord1xOES (GLfixed u) {
  osi.glr->glExt.glEvalCoord1xOES (u);}
inline GLAPI void APIENTRY glEvalCoord1xvOES (const GLfixed *coords) {
  osi.glr->glExt.glEvalCoord1xvOES (coords);}
inline GLAPI void APIENTRY glEvalCoord2xOES (GLfixed u, GLfixed v) {
  osi.glr->glExt.glEvalCoord2xOES (u, v);}
inline GLAPI void APIENTRY glEvalCoord2xvOES (const GLfixed *coords) {
  osi.glr->glExt.glEvalCoord2xvOES (coords);}
inline GLAPI void APIENTRY glFeedbackBufferxOES (GLsizei n, GLenum type, const GLfixed *buffer) {
  osi.glr->glExt.glFeedbackBufferxOES (n, type, buffer);}
inline GLAPI void APIENTRY glGetConvolutionParameterxvOES (GLenum target, GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetConvolutionParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glGetHistogramParameterxvOES (GLenum target, GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetHistogramParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glGetLightxOES (GLenum light, GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetLightxOES (light, pname, params);}
inline GLAPI void APIENTRY glGetMapxvOES (GLenum target, GLenum query, GLfixed *v) {
  osi.glr->glExt.glGetMapxvOES (target, query, v);}
inline GLAPI void APIENTRY glGetMaterialxOES (GLenum face, GLenum pname, GLfixed param) {
  osi.glr->glExt.glGetMaterialxOES (face, pname, param);}
inline GLAPI void APIENTRY glGetPixelMapxv (GLenum map, GLint size, GLfixed *values) {
  osi.glr->glExt.glGetPixelMapxv (map, size, values);}
inline GLAPI void APIENTRY glGetTexGenxvOES (GLenum coord, GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetTexGenxvOES (coord, pname, params);}
inline GLAPI void APIENTRY glGetTexLevelParameterxvOES (GLenum target, GLint level, GLenum pname, GLfixed *params) {
  osi.glr->glExt.glGetTexLevelParameterxvOES (target, level, pname, params);}
inline GLAPI void APIENTRY glIndexxOES (GLfixed component) {
  osi.glr->glExt.glIndexxOES (component);}
inline GLAPI void APIENTRY glIndexxvOES (const GLfixed *component) {
  osi.glr->glExt.glIndexxvOES (component);}
inline GLAPI void APIENTRY glLoadTransposeMatrixxOES (const GLfixed *m) {
  osi.glr->glExt.glLoadTransposeMatrixxOES (m);}
inline GLAPI void APIENTRY glMap1xOES (GLenum target, GLfixed u1, GLfixed u2, GLint stride, GLint order, GLfixed points) {
  osi.glr->glExt.glMap1xOES (target, u1, u2, stride, order, points);}
inline GLAPI void APIENTRY glMap2xOES (GLenum target, GLfixed u1, GLfixed u2, GLint ustride, GLint uorder, GLfixed v1, GLfixed v2, GLint vstride, GLint vorder, GLfixed points) {
  osi.glr->glExt.glMap2xOES (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
inline GLAPI void APIENTRY glMapGrid1xOES (GLint n, GLfixed u1, GLfixed u2) {
  osi.glr->glExt.glMapGrid1xOES (n, u1, u2);}
inline GLAPI void APIENTRY glMapGrid2xOES (GLint n, GLfixed u1, GLfixed u2, GLfixed v1, GLfixed v2) {
  osi.glr->glExt.glMapGrid2xOES (n, u1, u2, v1, v2);}
inline GLAPI void APIENTRY glMultTransposeMatrixxOES (const GLfixed *m) {
  osi.glr->glExt.glMultTransposeMatrixxOES (m);}
inline GLAPI void APIENTRY glMultiTexCoord1xOES (GLenum texture, GLfixed s) {
  osi.glr->glExt.glMultiTexCoord1xOES (texture, s);}
inline GLAPI void APIENTRY glMultiTexCoord1xvOES (GLenum texture, const GLfixed *coords) {
  osi.glr->glExt.glMultiTexCoord1xvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord2xOES (GLenum texture, GLfixed s, GLfixed t) {
  osi.glr->glExt.glMultiTexCoord2xOES (texture, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2xvOES (GLenum texture, const GLfixed *coords) {
  osi.glr->glExt.glMultiTexCoord2xvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord3xOES (GLenum texture, GLfixed s, GLfixed t, GLfixed r) {
  osi.glr->glExt.glMultiTexCoord3xOES (texture, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3xvOES (GLenum texture, const GLfixed *coords) {
  osi.glr->glExt.glMultiTexCoord3xvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord4xvOES (GLenum texture, const GLfixed *coords) {
  osi.glr->glExt.glMultiTexCoord4xvOES (texture, coords);}
inline GLAPI void APIENTRY glNormal3xvOES (const GLfixed *coords) {
  osi.glr->glExt.glNormal3xvOES (coords);}
inline GLAPI void APIENTRY glPassThroughxOES (GLfixed token) {
  osi.glr->glExt.glPassThroughxOES (token);}
inline GLAPI void APIENTRY glPixelMapx (GLenum map, GLint size, const GLfixed *values) {
  osi.glr->glExt.glPixelMapx (map, size, values);}
inline GLAPI void APIENTRY glPixelStorex (GLenum pname, GLfixed param) {
  osi.glr->glExt.glPixelStorex (pname, param);}
inline GLAPI void APIENTRY glPixelTransferxOES (GLenum pname, GLfixed param) {
  osi.glr->glExt.glPixelTransferxOES (pname, param);}
inline GLAPI void APIENTRY glPixelZoomxOES (GLfixed xfactor, GLfixed yfactor) {
  osi.glr->glExt.glPixelZoomxOES (xfactor, yfactor);}
inline GLAPI void APIENTRY glPrioritizeTexturesxOES (GLsizei n, const GLuint *textures, const GLfixed *priorities) {
  osi.glr->glExt.glPrioritizeTexturesxOES (n, textures, priorities);}
inline GLAPI void APIENTRY glRasterPos2xOES (GLfixed x, GLfixed y) {
  osi.glr->glExt.glRasterPos2xOES (x, y);}
inline GLAPI void APIENTRY glRasterPos2xvOES (const GLfixed *coords) {
  osi.glr->glExt.glRasterPos2xvOES (coords);}
inline GLAPI void APIENTRY glRasterPos3xOES (GLfixed x, GLfixed y, GLfixed z) {
  osi.glr->glExt.glRasterPos3xOES (x, y, z);}
inline GLAPI void APIENTRY glRasterPos3xvOES (const GLfixed *coords) {
  osi.glr->glExt.glRasterPos3xvOES (coords);}
inline GLAPI void APIENTRY glRasterPos4xOES (GLfixed x, GLfixed y, GLfixed z, GLfixed w) {
  osi.glr->glExt.glRasterPos4xOES (x, y, z, w);}
inline GLAPI void APIENTRY glRasterPos4xvOES (const GLfixed *coords) {
  osi.glr->glExt.glRasterPos4xvOES (coords);}
inline GLAPI void APIENTRY glRectxOES (GLfixed x1, GLfixed y1, GLfixed x2, GLfixed y2) {
  osi.glr->glExt.glRectxOES (x1, y1, x2, y2);}
inline GLAPI void APIENTRY glRectxvOES (const GLfixed *v1, const GLfixed *v2) {
  osi.glr->glExt.glRectxvOES (v1, v2);}
inline GLAPI void APIENTRY glTexCoord1xOES (GLfixed s) {
  osi.glr->glExt.glTexCoord1xOES (s);}
inline GLAPI void APIENTRY glTexCoord1xvOES (const GLfixed *coords) {
  osi.glr->glExt.glTexCoord1xvOES (coords);}
inline GLAPI void APIENTRY glTexCoord2xOES (GLfixed s, GLfixed t) {
  osi.glr->glExt.glTexCoord2xOES (s, t);}
inline GLAPI void APIENTRY glTexCoord2xvOES (const GLfixed *coords) {
  osi.glr->glExt.glTexCoord2xvOES (coords);}
inline GLAPI void APIENTRY glTexCoord3xOES (GLfixed s, GLfixed t, GLfixed r) {
  osi.glr->glExt.glTexCoord3xOES (s, t, r);}
inline GLAPI void APIENTRY glTexCoord3xvOES (const GLfixed *coords) {
  osi.glr->glExt.glTexCoord3xvOES (coords);}
inline GLAPI void APIENTRY glTexCoord4xOES (GLfixed s, GLfixed t, GLfixed r, GLfixed q) {
  osi.glr->glExt.glTexCoord4xOES (s, t, r, q);}
inline GLAPI void APIENTRY glTexCoord4xvOES (const GLfixed *coords) {
  osi.glr->glExt.glTexCoord4xvOES (coords);}
inline GLAPI void APIENTRY glTexGenxOES (GLenum coord, GLenum pname, GLfixed param) {
  osi.glr->glExt.glTexGenxOES (coord, pname,  param);}
inline GLAPI void APIENTRY glTexGenxvOES (GLenum coord, GLenum pname, const GLfixed *params) {
  osi.glr->glExt.glTexGenxvOES (coord, pname, params);}
inline GLAPI void APIENTRY glVertex2xOES (GLfixed x) {
  osi.glr->glExt.glVertex2xOES (x);}
inline GLAPI void APIENTRY glVertex2xvOES (const GLfixed *coords) {
  osi.glr->glExt.glVertex2xvOES (coords);}
inline GLAPI void APIENTRY glVertex3xOES (GLfixed x, GLfixed y) {
  osi.glr->glExt.glVertex3xOES (x, y);}
inline GLAPI void APIENTRY glVertex3xvOES (const GLfixed *coords) {
  osi.glr->glExt.glVertex3xvOES (coords);}
inline GLAPI void APIENTRY glVertex4xOES (GLfixed x, GLfixed y, GLfixed z) {
  osi.glr->glExt.glVertex4xOES (x, y, z);}
inline GLAPI void APIENTRY glVertex4xvOES (const GLfixed *coords) {
  osi.glr->glExt.glVertex4xvOES (coords);}
// GL_OES_query_matrix
inline GLAPI GLbitfield APIENTRY glQueryMatrixxOES (GLfixed *mantissa, GLint *exponent) {
  return osi.glr->glExt.glQueryMatrixxOES (mantissa, exponent);}
// GL_OES_single_precision
inline GLAPI void APIENTRY glClearDepthfOES (GLclampf depth) {
  osi.glr->glExt.glClearDepthfOES (depth);}
inline GLAPI void APIENTRY glClipPlanefOES (GLenum plane, const GLfloat *equation) {
  osi.glr->glExt.glClipPlanefOES (plane, equation);}
inline GLAPI void APIENTRY glDepthRangefOES (GLclampf n, GLclampf f) {
  osi.glr->glExt.glDepthRangefOES (n, f);}
inline GLAPI void APIENTRY glFrustumfOES (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f) {
  osi.glr->glExt.glFrustumfOES (l, r, b, t, n, f);}
inline GLAPI void APIENTRY glGetClipPlanefOES (GLenum plane, GLfloat *equation) {
  osi.glr->glExt.glGetClipPlanefOES (plane, equation);}
inline GLAPI void APIENTRY glOrthofOES (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f) {
  osi.glr->glExt.glOrthofOES (l, r, b, t, n, f);}
// GL_3DFX_tbuffer
inline GLAPI void APIENTRY glTbufferMask3DFX (GLuint mask) {
  osi.glr->glExt.glTbufferMask3DFX (mask);}
// GL_AMD_debug_output
inline GLAPI void APIENTRY glDebugMessageEnableAMD (GLenum category, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) {
  osi.glr->glExt.glDebugMessageEnableAMD (category, severity, count, ids, enabled);}
inline GLAPI void APIENTRY glDebugMessageInsertAMD (GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar *buf) {
  osi.glr->glExt.glDebugMessageInsertAMD (category, severity, id, length, buf);}
inline GLAPI void APIENTRY glDebugMessageCallbackAMD (GLDEBUGPROCAMD callback, void *userParam) {
  osi.glr->glExt.glDebugMessageCallbackAMD (callback, userParam);}
inline GLAPI GLuint APIENTRY glGetDebugMessageLogAMD (GLuint count, GLsizei bufsize, GLenum *categories, GLuint *severities, GLuint *ids, GLsizei *lengths, GLchar *message) {
  return osi.glr->glExt.glGetDebugMessageLogAMD (count, bufsize, categories, severities, ids, lengths, message);}
// GL_AMD_draw_buffers_blend
inline GLAPI void APIENTRY glBlendFuncIndexedAMD (GLuint buf, GLenum src, GLenum dst) {
  osi.glr->glExt.glBlendFuncIndexedAMD (buf, src, dst);}
inline GLAPI void APIENTRY glBlendFuncSeparateIndexedAMD (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  osi.glr->glExt.glBlendFuncSeparateIndexedAMD (buf, srcRGB, dstRGB, srcAlpha, dstAlpha);}
inline GLAPI void APIENTRY glBlendEquationIndexedAMD (GLuint buf, GLenum mode) {
  osi.glr->glExt.glBlendEquationIndexedAMD (buf, mode);}
inline GLAPI void APIENTRY glBlendEquationSeparateIndexedAMD (GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
  osi.glr->glExt.glBlendEquationSeparateIndexedAMD (buf, modeRGB, modeAlpha);}
// GL_AMD_gpu_shader_int64
inline GLAPI void APIENTRY glUniform1i64NV (GLint location, GLint64EXT x) {
  osi.glr->glExt.glUniform1i64NV (location, x);}
inline GLAPI void APIENTRY glUniform2i64NV (GLint location, GLint64EXT x, GLint64EXT y) {
  osi.glr->glExt.glUniform2i64NV (location, x, y);}
inline GLAPI void APIENTRY glUniform3i64NV (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z) {
  osi.glr->glExt.glUniform3i64NV (location, x, y, z);}
inline GLAPI void APIENTRY glUniform4i64NV (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w) {
  osi.glr->glExt.glUniform4i64NV (location, x, y, z, w);}
inline GLAPI void APIENTRY glUniform1i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glUniform1i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform2i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glUniform2i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform3i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glUniform3i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform4i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glUniform4i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform1ui64NV (GLint location, GLuint64EXT x) {
  osi.glr->glExt.glUniform1ui64NV (location, x);}
inline GLAPI void APIENTRY glUniform2ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y) {
  osi.glr->glExt.glUniform2ui64NV (location, x, y);}
inline GLAPI void APIENTRY glUniform3ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z) {
  osi.glr->glExt.glUniform3ui64NV (location, x, y, z);}
inline GLAPI void APIENTRY glUniform4ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w) {
  osi.glr->glExt.glUniform4ui64NV (location, x, y, z, w);}
inline GLAPI void APIENTRY glUniform1ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glUniform1ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform2ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glUniform2ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform3ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glUniform3ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform4ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glUniform4ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glGetUniformi64vNV (GLuint program, GLint location, GLint64EXT *params) {
  osi.glr->glExt.glGetUniformi64vNV (program, location, params);}
inline GLAPI void APIENTRY glGetUniformui64vNV (GLuint program, GLint location, GLuint64EXT *params) {
  osi.glr->glExt.glGetUniformui64vNV (program, location, params);}
inline GLAPI void APIENTRY glProgramUniform1i64NV (GLuint program, GLint location, GLint64EXT x) {
  osi.glr->glExt.glProgramUniform1i64NV (program, location, x);}
inline GLAPI void APIENTRY glProgramUniform2i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y) {
  osi.glr->glExt.glProgramUniform2i64NV (program, location, x, y);}
inline GLAPI void APIENTRY glProgramUniform3i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z) {
  osi.glr->glExt.glProgramUniform3i64NV (program, location,  x, y, z);}
inline GLAPI void APIENTRY glProgramUniform4i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w) {
  osi.glr->glExt.glProgramUniform4i64NV (program, location, x, y, z, w);}
inline GLAPI void APIENTRY glProgramUniform1i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glProgramUniform1i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glProgramUniform2i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glProgramUniform3i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  osi.glr->glExt.glProgramUniform4i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1ui64NV (GLuint program, GLint location, GLuint64EXT x) {
  osi.glr->glExt.glProgramUniform1ui64NV (program, location, x);}
inline GLAPI void APIENTRY glProgramUniform2ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y) {
  osi.glr->glExt.glProgramUniform2ui64NV (program, location, x, y);}
inline GLAPI void APIENTRY glProgramUniform3ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z) {
  osi.glr->glExt.glProgramUniform3ui64NV (program, location, x, y, z);}
inline GLAPI void APIENTRY glProgramUniform4ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w) {
  osi.glr->glExt.glProgramUniform4ui64NV (program, location, x, y, z, w);}
inline GLAPI void APIENTRY glProgramUniform1ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glProgramUniform1ui64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glProgramUniform2ui64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glProgramUniform3ui64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glProgramUniform4ui64vNV (program, location, count, value);}
// GL_AMD_interleaved_elements
inline GLAPI void APIENTRY glVertexAttribParameteriAMD (GLuint index, GLenum pname, GLint param) {
  osi.glr->glExt.glVertexAttribParameteriAMD (index, pname, param);}
// GL_AMD_multi_draw_indirect
inline GLAPI void APIENTRY glMultiDrawArraysIndirectAMD (GLenum mode, const void *indirect, GLsizei primcount, GLsizei stride) {
  osi.glr->glExt.glMultiDrawArraysIndirectAMD (mode, indirect, primcount, stride);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirectAMD (GLenum mode, GLenum type, const void *indirect, GLsizei primcount, GLsizei stride) {
  osi.glr->glExt.glMultiDrawElementsIndirectAMD (mode, type, indirect, primcount, stride);}
// GL_AMD_name_gen_delete
inline GLAPI void APIENTRY glGenNamesAMD (GLenum identifier, GLuint num, GLuint *names) {
  osi.glr->glExt.glGenNamesAMD (identifier, num, names);}
inline GLAPI void APIENTRY glDeleteNamesAMD (GLenum identifier, GLuint num, const GLuint *names) {
  osi.glr->glExt.glDeleteNamesAMD (identifier, num, names);}
inline GLAPI GLboolean APIENTRY glIsNameAMD (GLenum identifier, GLuint name) {
  return osi.glr->glExt.glIsNameAMD (identifier, name);}
// GL_AMD_occlusion_query_event
inline GLAPI void APIENTRY glQueryObjectParameteruiAMD (GLenum target, GLuint id, GLenum pname, GLuint param) {
  osi.glr->glExt.glQueryObjectParameteruiAMD (target, id, pname, param);}
// GL_AMD_performance_monitor
inline GLAPI void APIENTRY glGetPerfMonitorGroupsAMD (GLint *numGroups, GLsizei groupsSize, GLuint *groups) {
  osi.glr->glExt.glGetPerfMonitorGroupsAMD (numGroups, groupsSize, groups);}
inline GLAPI void APIENTRY glGetPerfMonitorCountersAMD (GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei counterSize, GLuint *counters) {
  osi.glr->glExt.glGetPerfMonitorCountersAMD (group, numCounters, maxActiveCounters, counterSize, counters);}
inline GLAPI void APIENTRY glGetPerfMonitorGroupStringAMD (GLuint group, GLsizei bufSize, GLsizei *length, GLchar *groupString) {
  osi.glr->glExt.glGetPerfMonitorGroupStringAMD (group, bufSize, length, groupString);}
inline GLAPI void APIENTRY glGetPerfMonitorCounterStringAMD (GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, GLchar *counterString) {
  osi.glr->glExt.glGetPerfMonitorCounterStringAMD (group, counter, bufSize, length, counterString);}
inline GLAPI void APIENTRY glGetPerfMonitorCounterInfoAMD (GLuint group, GLuint counter, GLenum pname, void *data) {
  osi.glr->glExt.glGetPerfMonitorCounterInfoAMD (group, counter, pname, data);}
inline GLAPI void APIENTRY glGenPerfMonitorsAMD (GLsizei n, GLuint *monitors) {
  osi.glr->glExt.glGenPerfMonitorsAMD (n, monitors);}
inline GLAPI void APIENTRY glDeletePerfMonitorsAMD (GLsizei n, GLuint *monitors) {
  osi.glr->glExt.glDeletePerfMonitorsAMD (n, monitors);}
inline GLAPI void APIENTRY glSelectPerfMonitorCountersAMD (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *counterList) {
  osi.glr->glExt.glSelectPerfMonitorCountersAMD (monitor, enable, group, numCounters, counterList);}
inline GLAPI void APIENTRY glBeginPerfMonitorAMD (GLuint monitor) {
  osi.glr->glExt.glBeginPerfMonitorAMD (monitor);}
inline GLAPI void APIENTRY glEndPerfMonitorAMD (GLuint monitor) {
  osi.glr->glExt.glEndPerfMonitorAMD (monitor);}
inline GLAPI void APIENTRY glGetPerfMonitorCounterDataAMD (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten) {
  osi.glr->glExt.glGetPerfMonitorCounterDataAMD (monitor, pname, dataSize, data, bytesWritten);}
// GL_AMD_sample_positions
inline GLAPI void APIENTRY glSetMultisamplefvAMD (GLenum pname, GLuint index, const GLfloat *val) {
  osi.glr->glExt.glSetMultisamplefvAMD (pname, index, val);}
// GL_AMD_sparse_texture
inline GLAPI void APIENTRY glTexStorageSparseAMD (GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags) {
  osi.glr->glExt.glTexStorageSparseAMD (target, internalFormat, width, height, depth, layers, flags);}
inline GLAPI void APIENTRY glTextureStorageSparseAMD (GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags) {
  osi.glr->glExt.glTextureStorageSparseAMD (texture, target, internalFormat, width, height, depth, layers, flags);}
// GL_AMD_stencil_operation_extended
inline GLAPI void APIENTRY glStencilOpValueAMD (GLenum face, GLuint value) {
  osi.glr->glExt.glStencilOpValueAMD (face, value);}
// GL_AMD_vertex_shader_tessellator
inline GLAPI void APIENTRY glTessellationFactorAMD (GLfloat factor) {
  osi.glr->glExt.glTessellationFactorAMD (factor);}
inline GLAPI void APIENTRY glTessellationModeAMD (GLenum mode) {
  osi.glr->glExt.glTessellationModeAMD (mode);}
// GL_APPLE_element_array
inline GLAPI void APIENTRY glElementPointerAPPLE (GLenum type, const void *pointer) {
  osi.glr->glExt.glElementPointerAPPLE (type, pointer);}
inline GLAPI void APIENTRY glDrawElementArrayAPPLE (GLenum mode, GLint first, GLsizei count) {
  osi.glr->glExt.glDrawElementArrayAPPLE (mode, first, count);}
inline GLAPI void APIENTRY glDrawRangeElementArrayAPPLE (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count) {
  osi.glr->glExt.glDrawRangeElementArrayAPPLE (mode, start, end, first, count);}
inline GLAPI void APIENTRY glMultiDrawElementArrayAPPLE (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount) {
  osi.glr->glExt.glMultiDrawElementArrayAPPLE (mode, first, count, primcount);}
inline GLAPI void APIENTRY glMultiDrawRangeElementArrayAPPLE (GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount) {
  osi.glr->glExt.glMultiDrawRangeElementArrayAPPLE (mode, start, end, first, count, primcount);}
// GL_APPLE_fence
inline GLAPI void APIENTRY glGenFencesAPPLE (GLsizei n, GLuint *fences) {
  osi.glr->glExt.glGenFencesAPPLE (n, fences);}
inline GLAPI void APIENTRY glDeleteFencesAPPLE (GLsizei n, const GLuint *fences) {
  osi.glr->glExt.glDeleteFencesAPPLE (n, fences);}
inline GLAPI void APIENTRY glSetFenceAPPLE (GLuint fence) {
  osi.glr->glExt.glSetFenceAPPLE (fence);}
inline GLAPI GLboolean APIENTRY glIsFenceAPPLE (GLuint fence) {
  return osi.glr->glExt.glIsFenceAPPLE (fence);}
inline GLAPI GLboolean APIENTRY glTestFenceAPPLE (GLuint fence) {
  return osi.glr->glExt.glTestFenceAPPLE (fence);}
inline GLAPI void APIENTRY glFinishFenceAPPLE (GLuint fence) {
  osi.glr->glExt.glFinishFenceAPPLE (fence);}
inline GLAPI GLboolean APIENTRY glTestObjectAPPLE (GLenum object, GLuint name) {
  return osi.glr->glExt.glTestObjectAPPLE (object, name);}
inline GLAPI void APIENTRY glFinishObjectAPPLE (GLenum object, GLint name) {
  osi.glr->glExt.glFinishObjectAPPLE (object, name);}
// GL_APPLE_flush_buffer_range
inline GLAPI void APIENTRY glBufferParameteriAPPLE (GLenum target, GLenum pname, GLint param) {
  osi.glr->glExt.glBufferParameteriAPPLE (target, pname, param);}
inline GLAPI void APIENTRY glFlushMappedBufferRangeAPPLE (GLenum target, GLintptr offset, GLsizeiptr size) {
  osi.glr->glExt.glFlushMappedBufferRangeAPPLE (target, offset, size);}
// GL_APPLE_object_purgeable
inline GLAPI GLenum APIENTRY glObjectPurgeableAPPLE (GLenum objectType, GLuint name, GLenum option) {
  return osi.glr->glExt.glObjectPurgeableAPPLE (objectType, name, option);}
inline GLAPI GLenum APIENTRY glObjectUnpurgeableAPPLE (GLenum objectType, GLuint name, GLenum option) {
  return osi.glr->glExt.glObjectUnpurgeableAPPLE (objectType, name, option);}
inline GLAPI void APIENTRY glGetObjectParameterivAPPLE (GLenum objectType, GLuint name, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetObjectParameterivAPPLE (objectType, name, pname, params);}
// GL_APPLE_texture_range
inline GLAPI void APIENTRY glTextureRangeAPPLE (GLenum target, GLsizei length, const void *pointer) {
  osi.glr->glExt.glTextureRangeAPPLE (target, length, pointer);}
inline GLAPI void APIENTRY glGetTexParameterPointervAPPLE (GLenum target, GLenum pname, void **params) {
  osi.glr->glExt.glGetTexParameterPointervAPPLE (target, pname, params);}
// GL_APPLE_vertex_array_object
inline GLAPI void APIENTRY glBindVertexArrayAPPLE (GLuint array) {
  osi.glr->glExt.glBindVertexArrayAPPLE (array);}
inline GLAPI void APIENTRY glDeleteVertexArraysAPPLE (GLsizei n, const GLuint *arrays) {
  osi.glr->glExt.glDeleteVertexArraysAPPLE (n, arrays);}
inline GLAPI void APIENTRY glGenVertexArraysAPPLE (GLsizei n, GLuint *arrays) {
  osi.glr->glExt.glGenVertexArraysAPPLE (n, arrays);}
inline GLAPI GLboolean APIENTRY glIsVertexArrayAPPLE (GLuint array) {
  return osi.glr->glExt.glIsVertexArrayAPPLE (array);}
// GL_APPLE_vertex_array_range
inline GLAPI void APIENTRY glVertexArrayRangeAPPLE (GLsizei length, void *pointer) {
  osi.glr->glExt.glVertexArrayRangeAPPLE (length, pointer);}
inline GLAPI void APIENTRY glFlushVertexArrayRangeAPPLE (GLsizei length, void *pointer) {
  osi.glr->glExt.glFlushVertexArrayRangeAPPLE (length, pointer);}
inline GLAPI void APIENTRY glVertexArrayParameteriAPPLE (GLenum pname, GLint param) {
  osi.glr->glExt.glVertexArrayParameteriAPPLE (pname, param);}
// GL_APPLE_vertex_program_evaluators
inline GLAPI void APIENTRY glEnableVertexAttribAPPLE (GLuint index, GLenum pname) {
  osi.glr->glExt.glEnableVertexAttribAPPLE (index, pname);}
inline GLAPI void APIENTRY glDisableVertexAttribAPPLE (GLuint index, GLenum pname) {
  osi.glr->glExt.glDisableVertexAttribAPPLE (index, pname);}
inline GLAPI GLboolean APIENTRY glIsVertexAttribEnabledAPPLE (GLuint index, GLenum pname) {
  return osi.glr->glExt.glIsVertexAttribEnabledAPPLE (index, pname);}
inline GLAPI void APIENTRY glMapVertexAttrib1dAPPLE (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points) {
  osi.glr->glExt.glMapVertexAttrib1dAPPLE (index, size, u1, u2, stride, order, points);}
inline GLAPI void APIENTRY glMapVertexAttrib1fAPPLE (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points) {
  osi.glr->glExt.glMapVertexAttrib1fAPPLE (index, size, u1, u2, stride, order, points);}
inline GLAPI void APIENTRY glMapVertexAttrib2dAPPLE (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points) {
  osi.glr->glExt.glMapVertexAttrib2dAPPLE (index, size, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
inline GLAPI void APIENTRY glMapVertexAttrib2fAPPLE (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points) {
  osi.glr->glExt.glMapVertexAttrib2fAPPLE (index, size, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
// GL_ATI_draw_buffers
inline GLAPI void APIENTRY glDrawBuffersATI (GLsizei n, const GLenum *bufs) {
  osi.glr->glExt.glDrawBuffersATI (n, bufs);}
// GL_ATI_element_array
inline GLAPI void APIENTRY glElementPointerATI (GLenum type, const void *pointer) {
  osi.glr->glExt.glElementPointerATI (type, pointer);}
inline GLAPI void APIENTRY glDrawElementArrayATI (GLenum mode, GLsizei count) {
  osi.glr->glExt.glDrawElementArrayATI (mode, count);}
inline GLAPI void APIENTRY glDrawRangeElementArrayATI (GLenum mode, GLuint start, GLuint end, GLsizei count) {
  osi.glr->glExt.glDrawRangeElementArrayATI (mode, start,  end,  count);}
// GL_ATI_envmap_bumpmap
inline GLAPI void APIENTRY glTexBumpParameterivATI (GLenum pname, const GLint *param) {
  osi.glr->glExt.glTexBumpParameterivATI (pname, param);}
inline GLAPI void APIENTRY glTexBumpParameterfvATI (GLenum pname, const GLfloat *param) {
  osi.glr->glExt.glTexBumpParameterfvATI (pname, param);}
inline GLAPI void APIENTRY glGetTexBumpParameterivATI (GLenum pname, GLint *param) {
  osi.glr->glExt.glGetTexBumpParameterivATI (pname, param);}
inline GLAPI void APIENTRY glGetTexBumpParameterfvATI (GLenum pname, GLfloat *param) {
  osi.glr->glExt.glGetTexBumpParameterfvATI (pname, param);}
// GL_ATI_fragment_shader
inline GLAPI GLuint APIENTRY glGenFragmentShadersATI (GLuint range) {
  return osi.glr->glExt.glGenFragmentShadersATI (range);}
inline GLAPI void APIENTRY glBindFragmentShaderATI (GLuint id) {
  osi.glr->glExt.glBindFragmentShaderATI (id);}
inline GLAPI void APIENTRY glDeleteFragmentShaderATI (GLuint id) {
  osi.glr->glExt.glDeleteFragmentShaderATI (id);}
inline GLAPI void APIENTRY glBeginFragmentShaderATI (void) {
  osi.glr->glExt.glBeginFragmentShaderATI ();}
inline GLAPI void APIENTRY glEndFragmentShaderATI (void) {
  osi.glr->glExt.glEndFragmentShaderATI ();}
inline GLAPI void APIENTRY glPassTexCoordATI (GLuint dst, GLuint coord, GLenum swizzle) {
  osi.glr->glExt.glPassTexCoordATI (dst, coord, swizzle);}
inline GLAPI void APIENTRY glSampleMapATI (GLuint dst, GLuint interp, GLenum swizzle) {
  osi.glr->glExt.glSampleMapATI (dst, interp, swizzle);}
inline GLAPI void APIENTRY glColorFragmentOp1ATI (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod) {
  osi.glr->glExt.glColorFragmentOp1ATI (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod);}
inline GLAPI void APIENTRY glColorFragmentOp2ATI (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod) {
  osi.glr->glExt.glColorFragmentOp2ATI (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod);}
inline GLAPI void APIENTRY glColorFragmentOp3ATI (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod) {
  osi.glr->glExt.glColorFragmentOp3ATI (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod);}
inline GLAPI void APIENTRY glAlphaFragmentOp1ATI (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod) {
  osi.glr->glExt.glAlphaFragmentOp1ATI (op, dst, dstMod, arg1, arg1Rep, arg1Mod);}
inline GLAPI void APIENTRY glAlphaFragmentOp2ATI (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod) {
  osi.glr->glExt.glAlphaFragmentOp2ATI (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod);}
inline GLAPI void APIENTRY glAlphaFragmentOp3ATI (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod) {
  osi.glr->glExt.glAlphaFragmentOp3ATI (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod);}
inline GLAPI void APIENTRY glSetFragmentShaderConstantATI (GLuint dst, const GLfloat *value) {
  osi.glr->glExt.glSetFragmentShaderConstantATI (dst, value);}
// GL_ATI_map_object_buffer
inline GLAPI void *APIENTRY glMapObjectBufferATI (GLuint buffer) {
  return osi.glr->glExt.glMapObjectBufferATI (buffer);}
inline GLAPI void APIENTRY glUnmapObjectBufferATI (GLuint buffer) {
  osi.glr->glExt.glUnmapObjectBufferATI (buffer);}
// GL_ATI_pn_triangles
inline GLAPI void APIENTRY glPNTrianglesiATI (GLenum pname, GLint param) {
  osi.glr->glExt.glPNTrianglesiATI (pname, param);}
inline GLAPI void APIENTRY glPNTrianglesfATI (GLenum pname, GLfloat param) {
  osi.glr->glExt.glPNTrianglesfATI (pname, param);}
// GL_ATI_separate_stencil
inline GLAPI void APIENTRY glStencilOpSeparateATI (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
  osi.glr->glExt.glStencilOpSeparateATI (face, sfail, dpfail, dppass);}
inline GLAPI void APIENTRY glStencilFuncSeparateATI (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask) {
  osi.glr->glExt.glStencilFuncSeparateATI (frontfunc, backfunc, ref, mask);}
// GL_ATI_vertex_array_object
inline GLAPI GLuint APIENTRY glNewObjectBufferATI (GLsizei size, const void *pointer, GLenum usage) {
  return osi.glr->glExt.glNewObjectBufferATI (size, pointer,  usage);}
inline GLAPI GLboolean APIENTRY glIsObjectBufferATI (GLuint buffer) {
  return osi.glr->glExt.glIsObjectBufferATI (buffer);}
inline GLAPI void APIENTRY glUpdateObjectBufferATI (GLuint buffer, GLuint offset, GLsizei size, const void *pointer, GLenum preserve) {
  osi.glr->glExt.glUpdateObjectBufferATI (buffer, offset, size, pointer,  preserve);}
inline GLAPI void APIENTRY glGetObjectBufferfvATI (GLuint buffer, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetObjectBufferfvATI (buffer, pname, params);}
inline GLAPI void APIENTRY glGetObjectBufferivATI (GLuint buffer, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetObjectBufferivATI (buffer, pname, params);}
inline GLAPI void APIENTRY glFreeObjectBufferATI (GLuint buffer) {
  osi.glr->glExt.glFreeObjectBufferATI (buffer);}
inline GLAPI void APIENTRY glArrayObjectATI (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset) {
  osi.glr->glExt.glArrayObjectATI (array, size, type, stride, buffer, offset);}
inline GLAPI void APIENTRY glGetArrayObjectfvATI (GLenum array, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetArrayObjectfvATI (array, pname, params);}
inline GLAPI void APIENTRY glGetArrayObjectivATI (GLenum array, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetArrayObjectivATI (array, pname, params);}
inline GLAPI void APIENTRY glVariantArrayObjectATI (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset) {
  osi.glr->glExt.glVariantArrayObjectATI (id, type, stride, buffer, offset);}
inline GLAPI void APIENTRY glGetVariantArrayObjectfvATI (GLuint id, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetVariantArrayObjectfvATI (id, pname, params);}
inline GLAPI void APIENTRY glGetVariantArrayObjectivATI (GLuint id, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVariantArrayObjectivATI (id, pname, params);}
// GL_ATI_vertex_attrib_array_object
inline GLAPI void APIENTRY glVertexAttribArrayObjectATI (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset) {
  osi.glr->glExt.glVertexAttribArrayObjectATI (index, size, type, normalized, stride, buffer, offset);}
inline GLAPI void APIENTRY glGetVertexAttribArrayObjectfvATI (GLuint index, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetVertexAttribArrayObjectfvATI (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribArrayObjectivATI (GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVertexAttribArrayObjectivATI (index, pname, params);}
// GL_ATI_vertex_streams
inline GLAPI void APIENTRY glVertexStream1sATI (GLenum stream, GLshort x) {
  osi.glr->glExt.glVertexStream1sATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1svATI (GLenum stream, const GLshort *coords) {
  osi.glr->glExt.glVertexStream1svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream1iATI (GLenum stream, GLint x) {
  osi.glr->glExt.glVertexStream1iATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1ivATI (GLenum stream, const GLint *coords) {
  osi.glr->glExt.glVertexStream1ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream1fATI (GLenum stream, GLfloat x) {
  osi.glr->glExt.glVertexStream1fATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1fvATI (GLenum stream, const GLfloat *coords) {
  osi.glr->glExt.glVertexStream1fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream1dATI (GLenum stream, GLdouble x) {
  osi.glr->glExt.glVertexStream1dATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1dvATI (GLenum stream, const GLdouble *coords) {
  osi.glr->glExt.glVertexStream1dvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2sATI (GLenum stream, GLshort x, GLshort y) {
  osi.glr->glExt.glVertexStream2sATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2svATI (GLenum stream, const GLshort *coords) {
  osi.glr->glExt.glVertexStream2svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2iATI (GLenum stream, GLint x, GLint y) {
  osi.glr->glExt.glVertexStream2iATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2ivATI (GLenum stream, const GLint *coords) {
  osi.glr->glExt.glVertexStream2ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2fATI (GLenum stream, GLfloat x, GLfloat y) {
  osi.glr->glExt.glVertexStream2fATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2fvATI (GLenum stream, const GLfloat *coords) {
  osi.glr->glExt.glVertexStream2fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2dATI (GLenum stream, GLdouble x, GLdouble y) {
  osi.glr->glExt.glVertexStream2dATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2dvATI (GLenum stream, const GLdouble *coords) {
  osi.glr->glExt.glVertexStream2dvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3sATI (GLenum stream, GLshort x, GLshort y, GLshort z) {
  osi.glr->glExt.glVertexStream3sATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3svATI (GLenum stream, const GLshort *coords) {
  osi.glr->glExt.glVertexStream3svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3iATI (GLenum stream, GLint x, GLint y, GLint z) {
  osi.glr->glExt.glVertexStream3iATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3ivATI (GLenum stream, const GLint *coords) {
  osi.glr->glExt.glVertexStream3ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3fATI (GLenum stream, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glVertexStream3fATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3fvATI (GLenum stream, const GLfloat *coords) {
  osi.glr->glExt.glVertexStream3fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3dATI (GLenum stream, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glVertexStream3dATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3dvATI (GLenum stream, const GLdouble *coords) {
  osi.glr->glExt.glVertexStream3dvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4sATI (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w) {
  osi.glr->glExt.glVertexStream4sATI (stream,  x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4svATI (GLenum stream, const GLshort *coords) {
  osi.glr->glExt.glVertexStream4svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4iATI (GLenum stream, GLint x, GLint y, GLint z, GLint w) {
  osi.glr->glExt.glVertexStream4iATI (stream, x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4ivATI (GLenum stream, const GLint *coords) {
  osi.glr->glExt.glVertexStream4ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4fATI (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glVertexStream4fATI (stream, x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4fvATI (GLenum stream, const GLfloat *coords) {
  osi.glr->glExt.glVertexStream4fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4dATI (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glVertexStream4dATI (stream, x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4dvATI (GLenum stream, const GLdouble *coords) {
  osi.glr->glExt.glVertexStream4dvATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3bATI (GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz) {
  osi.glr->glExt.glNormalStream3bATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3bvATI (GLenum stream, const GLbyte *coords) {
  osi.glr->glExt.glNormalStream3bvATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3sATI (GLenum stream, GLshort nx, GLshort ny, GLshort nz) {
  osi.glr->glExt.glNormalStream3sATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3svATI (GLenum stream, const GLshort *coords) {
  osi.glr->glExt.glNormalStream3svATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3iATI (GLenum stream, GLint nx, GLint ny, GLint nz) {
  osi.glr->glExt.glNormalStream3iATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3ivATI (GLenum stream, const GLint *coords) {
  osi.glr->glExt.glNormalStream3ivATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3fATI (GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz) {
  osi.glr->glExt.glNormalStream3fATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3fvATI (GLenum stream, const GLfloat *coords) {
  osi.glr->glExt.glNormalStream3fvATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3dATI (GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz) {
  osi.glr->glExt.glNormalStream3dATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3dvATI (GLenum stream, const GLdouble *coords) {
  osi.glr->glExt.glNormalStream3dvATI (stream, coords);}
inline GLAPI void APIENTRY glClientActiveVertexStreamATI (GLenum stream) {
  osi.glr->glExt.glClientActiveVertexStreamATI (stream);}
inline GLAPI void APIENTRY glVertexBlendEnviATI (GLenum pname, GLint param) {
  osi.glr->glExt.glVertexBlendEnviATI (pname, param);}
inline GLAPI void APIENTRY glVertexBlendEnvfATI (GLenum pname, GLfloat param) {
  osi.glr->glExt.glVertexBlendEnvfATI (pname, param);}
// GL_EXT_bindable_uniform
inline GLAPI void APIENTRY glUniformBufferEXT (GLuint program, GLint location, GLuint buffer) {
  osi.glr->glExt.glUniformBufferEXT (program, location, buffer);}
inline GLAPI GLint APIENTRY glGetUniformBufferSizeEXT (GLuint program, GLint location) {
  return osi.glr->glExt.glGetUniformBufferSizeEXT (program, location);}
inline GLAPI GLintptr APIENTRY glGetUniformOffsetEXT (GLuint program, GLint location) {
  return osi.glr->glExt.glGetUniformOffsetEXT (program, location);}
// GL_EXT_blend_color
inline GLAPI void APIENTRY glBlendColorEXT (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  osi.glr->glExt.glBlendColorEXT (red, green, blue,  alpha);}
// GL_EXT_blend_equation_separate
inline GLAPI void APIENTRY glBlendEquationSeparateEXT (GLenum modeRGB, GLenum modeAlpha) {
  osi.glr->glExt.glBlendEquationSeparateEXT (modeRGB, modeAlpha);}
// GL_EXT_blend_func_separate
inline GLAPI void APIENTRY glBlendFuncSeparateEXT (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
  osi.glr->glExt.glBlendFuncSeparateEXT (sfactorRGB, dfactorRGB, sfactorAlpha,  dfactorAlpha);}
// GL_EXT_blend_minmax
inline GLAPI void APIENTRY glBlendEquationEXT (GLenum mode) {
  osi.glr->glExt.glBlendEquationEXT (mode);}
// GL_EXT_color_subtable
inline GLAPI void APIENTRY glColorSubTableEXT (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glColorSubTableEXT (target, start, count, format, type, data);}
inline GLAPI void APIENTRY glCopyColorSubTableEXT (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyColorSubTableEXT (target, start, x, y, width);}
// GL_EXT_compiled_vertex_array
inline GLAPI void APIENTRY glLockArraysEXT (GLint first, GLsizei count) {
  osi.glr->glExt.glLockArraysEXT (first,  count);}
inline GLAPI void APIENTRY glUnlockArraysEXT (void) {
  osi.glr->glExt.glUnlockArraysEXT ();}
// GL_EXT_convolution
inline GLAPI void APIENTRY glConvolutionFilter1DEXT (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image) {
  osi.glr->glExt.glConvolutionFilter1DEXT (target, internalformat, width, format, type, image);}
inline GLAPI void APIENTRY glConvolutionFilter2DEXT (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image) {
  osi.glr->glExt.glConvolutionFilter2DEXT (target, internalformat, width, height, format, type, image);}
inline GLAPI void APIENTRY glConvolutionParameterfEXT (GLenum target, GLenum pname, GLfloat params) {
  osi.glr->glExt.glConvolutionParameterfEXT (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameterfvEXT (GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glConvolutionParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameteriEXT (GLenum target, GLenum pname, GLint params) {
  osi.glr->glExt.glConvolutionParameteriEXT (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameterivEXT (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glConvolutionParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glCopyConvolutionFilter1DEXT (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyConvolutionFilter1DEXT (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glCopyConvolutionFilter2DEXT (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyConvolutionFilter2DEXT (target, internalformat, x, y, width, height);}
inline GLAPI void APIENTRY glGetConvolutionFilterEXT (GLenum target, GLenum format, GLenum type, void *image) {
  osi.glr->glExt.glGetConvolutionFilterEXT (target, format, type, image);}
inline GLAPI void APIENTRY glGetConvolutionParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetConvolutionParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetConvolutionParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetConvolutionParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetSeparableFilterEXT (GLenum target, GLenum format, GLenum type, void *row, void *column, void *span) {
  osi.glr->glExt.glGetSeparableFilterEXT (target, format, type, row, column, span);}
inline GLAPI void APIENTRY glSeparableFilter2DEXT (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column) {
  osi.glr->glExt.glSeparableFilter2DEXT (target, internalformat, width, height, format, type, row, column);}
// GL_EXT_coordinate_frame
inline GLAPI void APIENTRY glTangent3bEXT (GLbyte tx, GLbyte ty, GLbyte tz) {
  osi.glr->glExt.glTangent3bEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3bvEXT (const GLbyte *v) {
  osi.glr->glExt.glTangent3bvEXT (v);}
inline GLAPI void APIENTRY glTangent3dEXT (GLdouble tx, GLdouble ty, GLdouble tz) {
  osi.glr->glExt.glTangent3dEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3dvEXT (const GLdouble *v) {
  osi.glr->glExt.glTangent3dvEXT (v);}
inline GLAPI void APIENTRY glTangent3fEXT (GLfloat tx, GLfloat ty, GLfloat tz) {
  osi.glr->glExt.glTangent3fEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3fvEXT (const GLfloat *v) {
  osi.glr->glExt.glTangent3fvEXT (v);}
inline GLAPI void APIENTRY glTangent3iEXT (GLint tx, GLint ty, GLint tz) {
  osi.glr->glExt.glTangent3iEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3ivEXT (const GLint *v) {
  osi.glr->glExt.glTangent3ivEXT (v);}
inline GLAPI void APIENTRY glTangent3sEXT (GLshort tx, GLshort ty, GLshort tz) {
  osi.glr->glExt.glTangent3sEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3svEXT (const GLshort *v) {
  osi.glr->glExt.glTangent3svEXT (v);}
inline GLAPI void APIENTRY glBinormal3bEXT (GLbyte bx, GLbyte by, GLbyte bz) {
  osi.glr->glExt.glBinormal3bEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3bvEXT (const GLbyte *v) {
  osi.glr->glExt.glBinormal3bvEXT (v);}
inline GLAPI void APIENTRY glBinormal3dEXT (GLdouble bx, GLdouble by, GLdouble bz) {
  osi.glr->glExt.glBinormal3dEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3dvEXT (const GLdouble *v) {
  osi.glr->glExt.glBinormal3dvEXT (v);}
inline GLAPI void APIENTRY glBinormal3fEXT (GLfloat bx, GLfloat by, GLfloat bz) {
  osi.glr->glExt.glBinormal3fEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3fvEXT (const GLfloat *v) {
  osi.glr->glExt.glBinormal3fvEXT (v);}
inline GLAPI void APIENTRY glBinormal3iEXT (GLint bx, GLint by, GLint bz) {
  osi.glr->glExt.glBinormal3iEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3ivEXT (const GLint *v) {
  osi.glr->glExt.glBinormal3ivEXT (v);}
inline GLAPI void APIENTRY glBinormal3sEXT (GLshort bx, GLshort by, GLshort bz) {
  osi.glr->glExt.glBinormal3sEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3svEXT (const GLshort *v) {
  osi.glr->glExt.glBinormal3svEXT (v);}
inline GLAPI void APIENTRY glTangentPointerEXT (GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glTangentPointerEXT (type, stride, pointer);}
inline GLAPI void APIENTRY glBinormalPointerEXT (GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glBinormalPointerEXT (type, stride, pointer);}
// GL_EXT_copy_texture
inline GLAPI void APIENTRY glCopyTexImage1DEXT (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
  osi.glr->glExt.glCopyTexImage1DEXT (target, level, internalformat, x, y, width, border);}
inline GLAPI void APIENTRY glCopyTexImage2DEXT (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
  osi.glr->glExt.glCopyTexImage2DEXT (target, level, internalformat, x, y, width, height, border);}
inline GLAPI void APIENTRY glCopyTexSubImage1DEXT (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyTexSubImage1DEXT (target, level, xoffset, x, y, width);}
inline GLAPI void APIENTRY glCopyTexSubImage2DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyTexSubImage2DEXT (target, level, xoffset, yoffset, x, y, width, height);}
inline GLAPI void APIENTRY glCopyTexSubImage3DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyTexSubImage3DEXT (target, level, xoffset, yoffset, zoffset, x, y, width, height);}
// GL_EXT_cull_vertex
inline GLAPI void APIENTRY glCullParameterdvEXT (GLenum pname, GLdouble *params) {
  osi.glr->glExt.glCullParameterdvEXT (pname, params);}
inline GLAPI void APIENTRY glCullParameterfvEXT (GLenum pname, GLfloat *params) {
  osi.glr->glExt.glCullParameterfvEXT (pname, params);}
// GL_EXT_debug_label
inline GLAPI void APIENTRY glLabelObjectEXT (GLenum type, GLuint object, GLsizei length, const GLchar *label) {
  osi.glr->glExt.glLabelObjectEXT (type, object, length, label);}
inline GLAPI void APIENTRY glGetObjectLabelEXT (GLenum type, GLuint object, GLsizei bufSize, GLsizei *length, GLchar *label) {
  osi.glr->glExt.glGetObjectLabelEXT (type, object, bufSize, length, label);}
// GL_EXT_debug_marker
inline GLAPI void APIENTRY glInsertEventMarkerEXT (GLsizei length, const GLchar *marker) {
  osi.glr->glExt.glInsertEventMarkerEXT (length, marker);}
inline GLAPI void APIENTRY glPushGroupMarkerEXT (GLsizei length, const GLchar *marker) {
  osi.glr->glExt.glPushGroupMarkerEXT (length, marker);}
inline GLAPI void APIENTRY glPopGroupMarkerEXT (void) {
  osi.glr->glExt.glPopGroupMarkerEXT ();}
// GL_EXT_depth_bounds_test
inline GLAPI void APIENTRY glDepthBoundsEXT (GLclampd zmin, GLclampd zmax) {
  osi.glr->glExt.glDepthBoundsEXT (zmin, zmax);}
// GL_EXT_direct_state_access
inline GLAPI void APIENTRY glMatrixLoadfEXT (GLenum mode, const GLfloat *m) {
  osi.glr->glExt.glMatrixLoadfEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixLoaddEXT (GLenum mode, const GLdouble *m) {
  osi.glr->glExt.glMatrixLoaddEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultfEXT (GLenum mode, const GLfloat *m) {
  osi.glr->glExt.glMatrixMultfEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultdEXT (GLenum mode, const GLdouble *m) {
  osi.glr->glExt.glMatrixMultdEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixLoadIdentityEXT (GLenum mode) {
  osi.glr->glExt.glMatrixLoadIdentityEXT (mode);}
inline GLAPI void APIENTRY glMatrixRotatefEXT (GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glMatrixRotatefEXT (mode, angle, x, y, z);}
inline GLAPI void APIENTRY glMatrixRotatedEXT (GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glMatrixRotatedEXT (mode, angle, x, y, z);}
inline GLAPI void APIENTRY glMatrixScalefEXT (GLenum mode, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glMatrixScalefEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixScaledEXT (GLenum mode, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glMatrixScaledEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixTranslatefEXT (GLenum mode, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glMatrixTranslatefEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixTranslatedEXT (GLenum mode, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glMatrixTranslatedEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixFrustumEXT (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
  osi.glr->glExt.glMatrixFrustumEXT (mode, left, right, bottom,  top, zNear, zFar);}
inline GLAPI void APIENTRY glMatrixOrthoEXT (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
  osi.glr->glExt.glMatrixOrthoEXT (mode, left, right, bottom, top, zNear, zFar);}
inline GLAPI void APIENTRY glMatrixPopEXT (GLenum mode) {
  osi.glr->glExt.glMatrixPopEXT (mode);}
inline GLAPI void APIENTRY glMatrixPushEXT (GLenum mode) {
  osi.glr->glExt.glMatrixPushEXT (mode);}
inline GLAPI void APIENTRY glClientAttribDefaultEXT (GLbitfield mask) {
  osi.glr->glExt.glClientAttribDefaultEXT (mask);}
inline GLAPI void APIENTRY glPushClientAttribDefaultEXT (GLbitfield mask) {
  osi.glr->glExt.glPushClientAttribDefaultEXT (mask);}
inline GLAPI void APIENTRY glTextureParameterfEXT (GLuint texture, GLenum target, GLenum pname, GLfloat param) {
  osi.glr->glExt.glTextureParameterfEXT (texture, target, pname, param);}
inline GLAPI void APIENTRY glTextureParameterfvEXT (GLuint texture, GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glTextureParameterfvEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glTextureParameteriEXT (GLuint texture, GLenum target, GLenum pname, GLint param) {
  osi.glr->glExt.glTextureParameteriEXT (texture, target, pname, param);}
inline GLAPI void APIENTRY glTextureParameterivEXT (GLuint texture, GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glTextureParameterivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glTextureImage1DEXT (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTextureImage1DEXT (texture, target, level, internalformat, width, border, format, type, pixels);}
inline GLAPI void APIENTRY glTextureImage2DEXT (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTextureImage2DEXT (texture, target, level, internalformat, width, height, border, format, type, pixels);}
inline GLAPI void APIENTRY glTextureSubImage1DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTextureSubImage1DEXT (texture, target, level, xoffset, width, format, type, pixels);}
inline GLAPI void APIENTRY glTextureSubImage2DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTextureSubImage2DEXT (texture, target, level, xoffset, yoffset, width, height, format,  type, pixels);}
inline GLAPI void APIENTRY glCopyTextureImage1DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
  osi.glr->glExt.glCopyTextureImage1DEXT (texture, target, level, internalformat, x, y, width, border);}
inline GLAPI void APIENTRY glCopyTextureImage2DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
  osi.glr->glExt.glCopyTextureImage2DEXT (texture, target, level, internalformat, x, y, width, height, border);}
inline GLAPI void APIENTRY glCopyTextureSubImage1DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyTextureSubImage1DEXT (texture, target, level, xoffset, x, y, width);}
inline GLAPI void APIENTRY glCopyTextureSubImage2DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyTextureSubImage2DEXT (texture, target, level, xoffset, yoffset, x, y, width, height);}
inline GLAPI void APIENTRY glGetTextureImageEXT (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void *pixels) {
  osi.glr->glExt.glGetTextureImageEXT (texture, target, level, format, type, pixels);}
inline GLAPI void APIENTRY glGetTextureParameterfvEXT (GLuint texture, GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetTextureParameterfvEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureParameterivEXT (GLuint texture, GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetTextureParameterivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureLevelParameterfvEXT (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetTextureLevelParameterfvEXT (texture,  target, level, pname, params);}
inline GLAPI void APIENTRY glGetTextureLevelParameterivEXT (GLuint texture, GLenum target, GLint level, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetTextureLevelParameterivEXT (texture, target, level, pname, params);}
inline GLAPI void APIENTRY glTextureImage3DEXT (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTextureImage3DEXT (texture, target, level, internalformat,  width, height, depth, border, format, type, pixels);}
inline GLAPI void APIENTRY glTextureSubImage3DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTextureSubImage3DEXT (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);}
inline GLAPI void APIENTRY glCopyTextureSubImage3DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyTextureSubImage3DEXT (texture, target, level, xoffset, yoffset, zoffset, x, y, width, height);}
inline GLAPI void APIENTRY glBindMultiTextureEXT (GLenum texunit, GLenum target, GLuint texture) {
  osi.glr->glExt.glBindMultiTextureEXT (texunit, target,  texture);}
inline GLAPI void APIENTRY glMultiTexCoordPointerEXT (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glMultiTexCoordPointerEXT (texunit, size, type, stride, pointer);}
inline GLAPI void APIENTRY glMultiTexEnvfEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat param) {
  osi.glr->glExt.glMultiTexEnvfEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexEnvfvEXT (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glMultiTexEnvfvEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexEnviEXT (GLenum texunit, GLenum target, GLenum pname, GLint param) {
  osi.glr->glExt.glMultiTexEnviEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexEnvivEXT (GLenum texunit, GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glMultiTexEnvivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexGendEXT (GLenum texunit, GLenum coord, GLenum pname, GLdouble param) {
  osi.glr->glExt.glMultiTexGendEXT (texunit, coord, pname, param);}
inline GLAPI void APIENTRY glMultiTexGendvEXT (GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params) {
  osi.glr->glExt.glMultiTexGendvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glMultiTexGenfEXT (GLenum texunit, GLenum coord, GLenum pname, GLfloat param) {
  osi.glr->glExt.glMultiTexGenfEXT (texunit, coord, pname, param);}
inline GLAPI void APIENTRY glMultiTexGenfvEXT (GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glMultiTexGenfvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glMultiTexGeniEXT (GLenum texunit, GLenum coord, GLenum pname, GLint param) {
  osi.glr->glExt.glMultiTexGeniEXT (texunit, coord, pname, param);}
inline GLAPI void APIENTRY glMultiTexGenivEXT (GLenum texunit, GLenum coord, GLenum pname, const GLint *params) {
  osi.glr->glExt.glMultiTexGenivEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexEnvfvEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMultiTexEnvfvEXT (texunit, target,  pname, params);}
inline GLAPI void APIENTRY glGetMultiTexEnvivEXT (GLenum texunit, GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMultiTexEnvivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexGendvEXT (GLenum texunit, GLenum coord, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetMultiTexGendvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexGenfvEXT (GLenum texunit, GLenum coord, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMultiTexGenfvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexGenivEXT (GLenum texunit, GLenum coord, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMultiTexGenivEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameteriEXT (GLenum texunit, GLenum target, GLenum pname, GLint param) {
  osi.glr->glExt.glMultiTexParameteriEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexParameterivEXT (GLenum texunit, GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glMultiTexParameterivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameterfEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat param) {
  osi.glr->glExt.glMultiTexParameterfEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexParameterfvEXT (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glMultiTexParameterfvEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glMultiTexImage1DEXT (texunit, target, level, internalformat, width, border, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glMultiTexImage2DEXT (texunit, target, level, internalformat, width, height, border, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexSubImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glMultiTexSubImage1DEXT (texunit, target, level, xoffset, width, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexSubImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glMultiTexSubImage2DEXT (texunit, target, level, xoffset, yoffset, width, height, format, type, pixels);}
inline GLAPI void APIENTRY glCopyMultiTexImage1DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
  osi.glr->glExt.glCopyMultiTexImage1DEXT (texunit, target, level, internalformat, x, y, width, border);}
inline GLAPI void APIENTRY glCopyMultiTexImage2DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
  osi.glr->glExt.glCopyMultiTexImage2DEXT (texunit, target, level, internalformat, x, y, width, height, border);}
inline GLAPI void APIENTRY glCopyMultiTexSubImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyMultiTexSubImage1DEXT (texunit, target, level, xoffset, x, y, width);}
inline GLAPI void APIENTRY glCopyMultiTexSubImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyMultiTexSubImage2DEXT (texunit, target, level, xoffset, yoffset, x, y, width, height);}
inline GLAPI void APIENTRY glGetMultiTexImageEXT (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void *pixels) {
  osi.glr->glExt.glGetMultiTexImageEXT (texunit, target, level, format, type, pixels);}
inline GLAPI void APIENTRY glGetMultiTexParameterfvEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMultiTexParameterfvEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexParameterivEXT (GLenum texunit, GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMultiTexParameterivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexLevelParameterfvEXT (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMultiTexLevelParameterfvEXT (texunit, target, level, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexLevelParameterivEXT (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMultiTexLevelParameterivEXT (texunit, target, level, pname, params);}
inline GLAPI void APIENTRY glMultiTexImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glMultiTexImage3DEXT (texunit, target, level, internalformat, width, height, depth, border, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexSubImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glMultiTexSubImage3DEXT (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);}
inline GLAPI void APIENTRY glCopyMultiTexSubImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  osi.glr->glExt.glCopyMultiTexSubImage3DEXT (texunit, target, level, xoffset,  yoffset, zoffset, x, y, width, height);}
inline GLAPI void APIENTRY glEnableClientStateIndexedEXT (GLenum array, GLuint index) {
  osi.glr->glExt.glEnableClientStateIndexedEXT (array, index);}
inline GLAPI void APIENTRY glDisableClientStateIndexedEXT (GLenum array, GLuint index) {
  osi.glr->glExt.glDisableClientStateIndexedEXT (array, index);}
inline GLAPI void APIENTRY glGetFloatIndexedvEXT (GLenum target, GLuint index, GLfloat *data) {
  osi.glr->glExt.glGetFloatIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glGetDoubleIndexedvEXT (GLenum target, GLuint index, GLdouble *data) {
  osi.glr->glExt.glGetDoubleIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glGetPointerIndexedvEXT (GLenum target, GLuint index, void **data) {
  osi.glr->glExt.glGetPointerIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glEnableIndexedEXT (GLenum target, GLuint index) {
  osi.glr->glExt.glEnableIndexedEXT (target, index);}
inline GLAPI void APIENTRY glDisableIndexedEXT (GLenum target, GLuint index) {
  osi.glr->glExt.glDisableIndexedEXT (target, index);}
inline GLAPI GLboolean APIENTRY glIsEnabledIndexedEXT (GLenum target, GLuint index) {
  return osi.glr->glExt.glIsEnabledIndexedEXT (target, index);}
inline GLAPI void APIENTRY glGetIntegerIndexedvEXT (GLenum target, GLuint index, GLint *data) {
  osi.glr->glExt.glGetIntegerIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glGetBooleanIndexedvEXT (GLenum target, GLuint index, GLboolean *data) {
  osi.glr->glExt.glGetBooleanIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glCompressedTextureImage3DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedTextureImage3DEXT (texture, target, level, internalformat, width, height, depth, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureImage2DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedTextureImage2DEXT (texture, target, level, internalformat, width, height, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureImage1DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedTextureImage1DEXT (texture, target, level, internalformat,  width, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureSubImage3DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedTextureSubImage3DEXT (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureSubImage2DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedTextureSubImage2DEXT (texture, target, level, xoffset, yoffset, width, height, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureSubImage1DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedTextureSubImage1DEXT (texture, target, level, xoffset, width, format, imageSize, bits);}
inline GLAPI void APIENTRY glGetCompressedTextureImageEXT (GLuint texture, GLenum target, GLint lod, void *img) {
  osi.glr->glExt.glGetCompressedTextureImageEXT (texture, target, lod, img);}
inline GLAPI void APIENTRY glCompressedMultiTexImage3DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedMultiTexImage3DEXT (texunit, target, level, internalformat, width, height, depth, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexImage2DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedMultiTexImage2DEXT (texunit, target, level, internalformat, width, height, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexImage1DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedMultiTexImage1DEXT (texunit, target, level, internalformat, width, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexSubImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedMultiTexSubImage3DEXT (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexSubImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedMultiTexSubImage2DEXT (texunit, target, level, xoffset, yoffset, width, height, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexSubImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits) {
  osi.glr->glExt.glCompressedMultiTexSubImage1DEXT (texunit, target, level, xoffset, width, format, imageSize, bits);}
inline GLAPI void APIENTRY glGetCompressedMultiTexImageEXT (GLenum texunit, GLenum target, GLint lod, void *img) {
  osi.glr->glExt.glGetCompressedMultiTexImageEXT (texunit, target, lod, img);}
inline GLAPI void APIENTRY glMatrixLoadTransposefEXT (GLenum mode, const GLfloat *m) {
  osi.glr->glExt.glMatrixLoadTransposefEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixLoadTransposedEXT (GLenum mode, const GLdouble *m) {
  osi.glr->glExt.glMatrixLoadTransposedEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultTransposefEXT (GLenum mode, const GLfloat *m) {
  osi.glr->glExt.glMatrixMultTransposefEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultTransposedEXT (GLenum mode, const GLdouble *m) {
  osi.glr->glExt.glMatrixMultTransposedEXT (mode, m);}
inline GLAPI void APIENTRY glNamedBufferDataEXT (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage) {
  osi.glr->glExt.glNamedBufferDataEXT (buffer, size, data, usage);}
inline GLAPI void APIENTRY glNamedBufferSubDataEXT (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data) {
  osi.glr->glExt.glNamedBufferSubDataEXT (buffer, offset, size, data);}
inline GLAPI void *APIENTRY glMapNamedBufferEXT (GLuint buffer, GLenum access) {
  return osi.glr->glExt.glMapNamedBufferEXT (buffer, access);}
inline GLAPI GLboolean APIENTRY glUnmapNamedBufferEXT (GLuint buffer) {
  return osi.glr->glExt.glUnmapNamedBufferEXT (buffer);}
inline GLAPI void APIENTRY glGetNamedBufferParameterivEXT (GLuint buffer, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetNamedBufferParameterivEXT (buffer, pname, params);}
inline GLAPI void APIENTRY glGetNamedBufferPointervEXT (GLuint buffer, GLenum pname, void **params) {
  osi.glr->glExt.glGetNamedBufferPointervEXT (buffer, pname, params);}
inline GLAPI void APIENTRY glGetNamedBufferSubDataEXT (GLuint buffer, GLintptr offset, GLsizeiptr size, void *data) {
  osi.glr->glExt.glGetNamedBufferSubDataEXT (buffer, offset, size, data);}
inline GLAPI void APIENTRY glProgramUniform1fEXT (GLuint program, GLint location, GLfloat v0) {
  osi.glr->glExt.glProgramUniform1fEXT (program, location,  v0);}
inline GLAPI void APIENTRY glProgramUniform2fEXT (GLuint program, GLint location, GLfloat v0, GLfloat v1) {
  osi.glr->glExt.glProgramUniform2fEXT (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform3fEXT (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  osi.glr->glExt.glProgramUniform3fEXT (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform4fEXT (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  osi.glr->glExt.glProgramUniform4fEXT (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform1iEXT (GLuint program, GLint location, GLint v0) {
  osi.glr->glExt.glProgramUniform1iEXT (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform2iEXT (GLuint program, GLint location, GLint v0, GLint v1) {
  osi.glr->glExt.glProgramUniform2iEXT (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform3iEXT (GLuint program, GLint location, GLint v0, GLint v1, GLint v2) {
  osi.glr->glExt.glProgramUniform3iEXT (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform4iEXT (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  osi.glr->glExt.glProgramUniform4iEXT (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform1fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform1fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform2fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform3fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  osi.glr->glExt.glProgramUniform4fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform1ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform2ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform3ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  osi.glr->glExt.glProgramUniform4ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix2fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix3fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix4fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix2x3fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix3x2fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix2x4fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix4x2fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix3x4fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  osi.glr->glExt.glProgramUniformMatrix4x3fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glTextureBufferEXT (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer) {
  osi.glr->glExt.glTextureBufferEXT (texture, target, internalformat, buffer);}
inline GLAPI void APIENTRY glMultiTexBufferEXT (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer) {
  osi.glr->glExt.glMultiTexBufferEXT (texunit, target, internalformat, buffer);}
inline GLAPI void APIENTRY glTextureParameterIivEXT (GLuint texture, GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glTextureParameterIivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glTextureParameterIuivEXT (GLuint texture, GLenum target, GLenum pname, const GLuint *params) {
  osi.glr->glExt.glTextureParameterIuivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureParameterIivEXT (GLuint texture, GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetTextureParameterIivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureParameterIuivEXT (GLuint texture, GLenum target, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetTextureParameterIuivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameterIivEXT (GLenum texunit, GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glMultiTexParameterIivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameterIuivEXT (GLenum texunit, GLenum target, GLenum pname, const GLuint *params) {
  osi.glr->glExt.glMultiTexParameterIuivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexParameterIivEXT (GLenum texunit, GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMultiTexParameterIivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexParameterIuivEXT (GLenum texunit, GLenum target, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetMultiTexParameterIuivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glProgramUniform1uiEXT (GLuint program, GLint location, GLuint v0) {
  osi.glr->glExt.glProgramUniform1uiEXT (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform2uiEXT (GLuint program, GLint location, GLuint v0, GLuint v1) {
  osi.glr->glExt.glProgramUniform2uiEXT (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform3uiEXT (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) {
  osi.glr->glExt.glProgramUniform3uiEXT (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform4uiEXT (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  osi.glr->glExt.glProgramUniform4uiEXT (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform1uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform1uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform2uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform3uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glProgramUniform4uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glNamedProgramLocalParameters4fvEXT (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat *params) {
  osi.glr->glExt.glNamedProgramLocalParameters4fvEXT (program, target, index, count, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4iEXT (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w) {
  osi.glr->glExt.glNamedProgramLocalParameterI4iEXT (program, target,  index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4ivEXT (GLuint program, GLenum target, GLuint index, const GLint *params) {
  osi.glr->glExt.glNamedProgramLocalParameterI4ivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParametersI4ivEXT (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint *params) {
  osi.glr->glExt.glNamedProgramLocalParametersI4ivEXT (program,  target,  index, count, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4uiEXT (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  osi.glr->glExt.glNamedProgramLocalParameterI4uiEXT (program, target, index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4uivEXT (GLuint program, GLenum target, GLuint index, const GLuint *params) {
  osi.glr->glExt.glNamedProgramLocalParameterI4uivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParametersI4uivEXT (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint *params) {
  osi.glr->glExt.glNamedProgramLocalParametersI4uivEXT (program, target, index, count, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterIivEXT (GLuint program, GLenum target, GLuint index, GLint *params) {
  osi.glr->glExt.glGetNamedProgramLocalParameterIivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterIuivEXT (GLuint program, GLenum target, GLuint index, GLuint *params) {
  osi.glr->glExt.glGetNamedProgramLocalParameterIuivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glEnableClientStateiEXT (GLenum array, GLuint index) {
  osi.glr->glExt.glEnableClientStateiEXT (array, index);}
inline GLAPI void APIENTRY glDisableClientStateiEXT (GLenum array, GLuint index) {
  osi.glr->glExt.glDisableClientStateiEXT (array, index);}
inline GLAPI void APIENTRY glGetFloati_vEXT (GLenum pname, GLuint index, GLfloat *params) {
  osi.glr->glExt.glGetFloati_vEXT (pname, index, params);}
inline GLAPI void APIENTRY glGetDoublei_vEXT (GLenum pname, GLuint index, GLdouble *params) {
  osi.glr->glExt.glGetDoublei_vEXT (pname, index, params);}
inline GLAPI void APIENTRY glGetPointeri_vEXT (GLenum pname, GLuint index, void **params) {
  osi.glr->glExt.glGetPointeri_vEXT (pname, index, params);}
inline GLAPI void APIENTRY glNamedProgramStringEXT (GLuint program, GLenum target, GLenum format, GLsizei len, const void *string) {
  osi.glr->glExt.glNamedProgramStringEXT (program, target, format, len, string);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4dEXT (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glNamedProgramLocalParameter4dEXT (program, target, index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4dvEXT (GLuint program, GLenum target, GLuint index, const GLdouble *params) {
  osi.glr->glExt.glNamedProgramLocalParameter4dvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4fEXT (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glNamedProgramLocalParameter4fEXT (program, target, index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4fvEXT (GLuint program, GLenum target, GLuint index, const GLfloat *params) {
  osi.glr->glExt.glNamedProgramLocalParameter4fvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterdvEXT (GLuint program, GLenum target, GLuint index, GLdouble *params) {
  osi.glr->glExt.glGetNamedProgramLocalParameterdvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterfvEXT (GLuint program, GLenum target, GLuint index, GLfloat *params) {
  osi.glr->glExt.glGetNamedProgramLocalParameterfvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramivEXT (GLuint program, GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetNamedProgramivEXT (program, target, pname, params);}
inline GLAPI void APIENTRY glGetNamedProgramStringEXT (GLuint program, GLenum target, GLenum pname, void *string) {
  osi.glr->glExt.glGetNamedProgramStringEXT (program, target, pname, string);}
inline GLAPI void APIENTRY glNamedRenderbufferStorageEXT (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glNamedRenderbufferStorageEXT (renderbuffer, internalformat, width, height);}
inline GLAPI void APIENTRY glGetNamedRenderbufferParameterivEXT (GLuint renderbuffer, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetNamedRenderbufferParameterivEXT (renderbuffer, pname, params);}
inline GLAPI void APIENTRY glNamedRenderbufferStorageMultisampleEXT (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glNamedRenderbufferStorageMultisampleEXT (renderbuffer, samples, internalformat, width, height);}
inline GLAPI void APIENTRY glNamedRenderbufferStorageMultisampleCoverageEXT (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glNamedRenderbufferStorageMultisampleCoverageEXT (renderbuffer, coverageSamples, colorSamples, internalformat, width, height);}
inline GLAPI GLenum APIENTRY glCheckNamedFramebufferStatusEXT (GLuint framebuffer, GLenum target) {
  return osi.glr->glExt.glCheckNamedFramebufferStatusEXT (framebuffer, target);}
inline GLAPI void APIENTRY glNamedFramebufferTexture1DEXT (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  osi.glr->glExt.glNamedFramebufferTexture1DEXT (framebuffer, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glNamedFramebufferTexture2DEXT (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  osi.glr->glExt.glNamedFramebufferTexture2DEXT (framebuffer, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glNamedFramebufferTexture3DEXT (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
  osi.glr->glExt.glNamedFramebufferTexture3DEXT (framebuffer, attachment, textarget, texture, level, zoffset);}
inline GLAPI void APIENTRY glNamedFramebufferRenderbufferEXT (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
  osi.glr->glExt.glNamedFramebufferRenderbufferEXT (framebuffer, attachment, renderbuffertarget,  renderbuffer);}
inline GLAPI void APIENTRY glGetNamedFramebufferAttachmentParameterivEXT (GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetNamedFramebufferAttachmentParameterivEXT (framebuffer, attachment, pname, params);}
inline GLAPI void APIENTRY glGenerateTextureMipmapEXT (GLuint texture, GLenum target) {
  osi.glr->glExt.glGenerateTextureMipmapEXT (texture, target);}
inline GLAPI void APIENTRY glGenerateMultiTexMipmapEXT (GLenum texunit, GLenum target) {
  osi.glr->glExt.glGenerateMultiTexMipmapEXT (texunit, target);}
inline GLAPI void APIENTRY glFramebufferDrawBufferEXT (GLuint framebuffer, GLenum mode) {
  osi.glr->glExt.glFramebufferDrawBufferEXT (framebuffer, mode);}
inline GLAPI void APIENTRY glFramebufferDrawBuffersEXT (GLuint framebuffer, GLsizei n, const GLenum *bufs) {
  osi.glr->glExt.glFramebufferDrawBuffersEXT (framebuffer, n, bufs);}
inline GLAPI void APIENTRY glFramebufferReadBufferEXT (GLuint framebuffer, GLenum mode) {
  osi.glr->glExt.glFramebufferReadBufferEXT (framebuffer,  mode);}
inline GLAPI void APIENTRY glGetFramebufferParameterivEXT (GLuint framebuffer, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFramebufferParameterivEXT (framebuffer, pname, params);}
inline GLAPI void APIENTRY glNamedCopyBufferSubDataEXT (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
  osi.glr->glExt.glNamedCopyBufferSubDataEXT (readBuffer, writeBuffer, readOffset, writeOffset, size);}
inline GLAPI void APIENTRY glNamedFramebufferTextureEXT (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level) {
  osi.glr->glExt.glNamedFramebufferTextureEXT (framebuffer, attachment, texture, level);}
inline GLAPI void APIENTRY glNamedFramebufferTextureLayerEXT (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  osi.glr->glExt.glNamedFramebufferTextureLayerEXT (framebuffer, attachment, texture, level, layer);}
inline GLAPI void APIENTRY glNamedFramebufferTextureFaceEXT (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face) {
  osi.glr->glExt.glNamedFramebufferTextureFaceEXT (framebuffer, attachment, texture, level, face);}
inline GLAPI void APIENTRY glTextureRenderbufferEXT (GLuint texture, GLenum target, GLuint renderbuffer) {
  osi.glr->glExt.glTextureRenderbufferEXT (texture, target, renderbuffer);}
inline GLAPI void APIENTRY glMultiTexRenderbufferEXT (GLenum texunit, GLenum target, GLuint renderbuffer) {
  osi.glr->glExt.glMultiTexRenderbufferEXT (texunit, target, renderbuffer);}
inline GLAPI void APIENTRY glVertexArrayVertexOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayVertexOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayColorOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayColorOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayEdgeFlagOffsetEXT (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayEdgeFlagOffsetEXT (vaobj, buffer, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayIndexOffsetEXT (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayIndexOffsetEXT (vaobj, buffer, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayNormalOffsetEXT (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayNormalOffsetEXT (vaobj, buffer, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayTexCoordOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayTexCoordOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayMultiTexCoordOffsetEXT (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayMultiTexCoordOffsetEXT ( vaobj, buffer, texunit, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayFogCoordOffsetEXT (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayFogCoordOffsetEXT (vaobj, buffer, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArraySecondaryColorOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArraySecondaryColorOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribOffsetEXT (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayVertexAttribOffsetEXT (vaobj, buffer, index, size, type, normalized, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribIOffsetEXT (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayVertexAttribIOffsetEXT (vaobj, buffer, index, size, type, stride, offset);}
inline GLAPI void APIENTRY glEnableVertexArrayEXT (GLuint vaobj, GLenum array) {
  osi.glr->glExt.glEnableVertexArrayEXT (vaobj, array);}
inline GLAPI void APIENTRY glDisableVertexArrayEXT (GLuint vaobj, GLenum array) {
  osi.glr->glExt.glDisableVertexArrayEXT (vaobj, array);}
inline GLAPI void APIENTRY glEnableVertexArrayAttribEXT (GLuint vaobj, GLuint index) {
  osi.glr->glExt.glEnableVertexArrayAttribEXT (vaobj, index);}
inline GLAPI void APIENTRY glDisableVertexArrayAttribEXT (GLuint vaobj, GLuint index) {
  osi.glr->glExt.glDisableVertexArrayAttribEXT (vaobj,  index);}
inline GLAPI void APIENTRY glGetVertexArrayIntegervEXT (GLuint vaobj, GLenum pname, GLint *param) {
  osi.glr->glExt.glGetVertexArrayIntegervEXT (vaobj, pname, param);}
inline GLAPI void APIENTRY glGetVertexArrayPointervEXT (GLuint vaobj, GLenum pname, void **param) {
  osi.glr->glExt.glGetVertexArrayPointervEXT (vaobj, pname, param);}
inline GLAPI void APIENTRY glGetVertexArrayIntegeri_vEXT (GLuint vaobj, GLuint index, GLenum pname, GLint *param) {
  osi.glr->glExt.glGetVertexArrayIntegeri_vEXT (vaobj, index, pname, param);}
inline GLAPI void APIENTRY glGetVertexArrayPointeri_vEXT (GLuint vaobj, GLuint index, GLenum pname, void **param) {
  osi.glr->glExt.glGetVertexArrayPointeri_vEXT (vaobj, index, pname, param);}
inline GLAPI void *APIENTRY glMapNamedBufferRangeEXT (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) {
  return osi.glr->glExt.glMapNamedBufferRangeEXT (buffer, offset, length, access);}
inline GLAPI void APIENTRY glFlushMappedNamedBufferRangeEXT (GLuint buffer, GLintptr offset, GLsizeiptr length) {
  osi.glr->glExt.glFlushMappedNamedBufferRangeEXT (buffer, offset, length);}
inline GLAPI void APIENTRY glNamedBufferStorageEXT (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags) {
  osi.glr->glExt.glNamedBufferStorageEXT (buffer, size, data, flags);}
inline GLAPI void APIENTRY glClearNamedBufferDataEXT (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glClearNamedBufferDataEXT (buffer, internalformat, format, type, data);}
inline GLAPI void APIENTRY glClearNamedBufferSubDataEXT (GLuint buffer, GLenum internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) {
  osi.glr->glExt.glClearNamedBufferSubDataEXT (buffer, internalformat, offset, size, format, type, data);}
inline GLAPI void APIENTRY glNamedFramebufferParameteriEXT (GLuint framebuffer, GLenum pname, GLint param) {
  osi.glr->glExt.glNamedFramebufferParameteriEXT (framebuffer, pname, param);}
inline GLAPI void APIENTRY glGetNamedFramebufferParameterivEXT (GLuint framebuffer, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetNamedFramebufferParameterivEXT (framebuffer, pname, params);}
inline GLAPI void APIENTRY glProgramUniform1dEXT (GLuint program, GLint location, GLdouble x) {
  osi.glr->glExt.glProgramUniform1dEXT (program, location, x);}
inline GLAPI void APIENTRY glProgramUniform2dEXT (GLuint program, GLint location, GLdouble x, GLdouble y) {
  osi.glr->glExt.glProgramUniform2dEXT (program, location, x, y);}
inline GLAPI void APIENTRY glProgramUniform3dEXT (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glProgramUniform3dEXT (program, location, x, y, z);}
inline GLAPI void APIENTRY glProgramUniform4dEXT (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glProgramUniform4dEXT (program, location, x, y, z, w);}
inline GLAPI void APIENTRY glProgramUniform1dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform1dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform2dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform3dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  osi.glr->glExt.glProgramUniform4dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix2dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix3dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix4dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix2x3dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix2x4dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix3x2dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix3x4dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix4x2dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  osi.glr->glExt.glProgramUniformMatrix4x3dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glTextureBufferRangeEXT (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  osi.glr->glExt.glTextureBufferRangeEXT (texture, target, internalformat, buffer, offset, size);}
inline GLAPI void APIENTRY glTextureStorage1DEXT (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) {
  osi.glr->glExt.glTextureStorage1DEXT (texture, target, levels, internalformat, width);}
inline GLAPI void APIENTRY glTextureStorage2DEXT (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glTextureStorage2DEXT (texture, target, levels, internalformat, width, height);}
inline GLAPI void APIENTRY glTextureStorage3DEXT (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
  osi.glr->glExt.glTextureStorage3DEXT (texture,  target, levels, internalformat, width, height, depth);}
inline GLAPI void APIENTRY glTextureStorage2DMultisampleEXT (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
  osi.glr->glExt.glTextureStorage2DMultisampleEXT (texture, target, samples, internalformat, width, height, fixedsamplelocations);}
inline GLAPI void APIENTRY glTextureStorage3DMultisampleEXT (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
  osi.glr->glExt.glTextureStorage3DMultisampleEXT (texture, target, samples, internalformat, width, height, depth, fixedsamplelocations);}
inline GLAPI void APIENTRY glVertexArrayBindVertexBufferEXT (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
  osi.glr->glExt.glVertexArrayBindVertexBufferEXT (vaobj, bindingindex, buffer, offset, stride);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribFormatEXT (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
  osi.glr->glExt.glVertexArrayVertexAttribFormatEXT (vaobj, attribindex, size, type, normalized, relativeoffset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribIFormatEXT (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  osi.glr->glExt.glVertexArrayVertexAttribIFormatEXT (vaobj, attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribLFormatEXT (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  osi.glr->glExt.glVertexArrayVertexAttribLFormatEXT (vaobj, attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribBindingEXT (GLuint vaobj, GLuint attribindex, GLuint bindingindex) {
  osi.glr->glExt.glVertexArrayVertexAttribBindingEXT (vaobj, attribindex, bindingindex);}
inline GLAPI void APIENTRY glVertexArrayVertexBindingDivisorEXT (GLuint vaobj, GLuint bindingindex, GLuint divisor) {
  osi.glr->glExt.glVertexArrayVertexBindingDivisorEXT (vaobj, bindingindex, divisor);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribLOffsetEXT (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  osi.glr->glExt.glVertexArrayVertexAttribLOffsetEXT (vaobj, buffer, index, size, type, stride, offset);}
inline GLAPI void APIENTRY glTexturePageCommitmentEXT (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean resident) {
  osi.glr->glExt.glTexturePageCommitmentEXT (texture, level, xoffset, yoffset, zoffset, width, height, depth, resident);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribDivisorEXT (GLuint vaobj, GLuint index, GLuint divisor) {
  osi.glr->glExt.glVertexArrayVertexAttribDivisorEXT (vaobj, index, divisor);}
// GL_EXT_draw_buffers2
inline GLAPI void APIENTRY glColorMaskIndexedEXT (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
  osi.glr->glExt.glColorMaskIndexedEXT (index, r, g, b, a);}
// GL_EXT_draw_instanced
inline GLAPI void APIENTRY glDrawArraysInstancedEXT (GLenum mode, GLint start, GLsizei count, GLsizei primcount) {
  osi.glr->glExt.glDrawArraysInstancedEXT (mode, start, count, primcount);}
inline GLAPI void APIENTRY glDrawElementsInstancedEXT (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount) {
  osi.glr->glExt.glDrawElementsInstancedEXT (mode, count, type, indices, primcount);}
// GL_EXT_draw_range_elements
inline GLAPI void APIENTRY glDrawRangeElementsEXT (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) {
  osi.glr->glExt.glDrawRangeElementsEXT (mode, start, end, count, type, indices);}
// GL_EXT_fog_coord
inline GLAPI void APIENTRY glFogCoordfEXT (GLfloat coord) {
  osi.glr->glExt.glFogCoordfEXT (coord);}
inline GLAPI void APIENTRY glFogCoordfvEXT (const GLfloat *coord) {
  osi.glr->glExt.glFogCoordfvEXT (coord);}
inline GLAPI void APIENTRY glFogCoorddEXT (GLdouble coord) {
  osi.glr->glExt.glFogCoorddEXT (coord);}
inline GLAPI void APIENTRY glFogCoorddvEXT (const GLdouble *coord) {
  osi.glr->glExt.glFogCoorddvEXT (coord);}
inline GLAPI void APIENTRY glFogCoordPointerEXT (GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glFogCoordPointerEXT (type, stride, pointer);}
// GL_EXT_framebuffer_blit
inline GLAPI void APIENTRY glBlitFramebufferEXT (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
  osi.glr->glExt.glBlitFramebufferEXT (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);}
// GL_EXT_framebuffer_multisample
inline GLAPI void APIENTRY glRenderbufferStorageMultisampleEXT (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glRenderbufferStorageMultisampleEXT (target, samples, internalformat, width, height);}
// GL_EXT_framebuffer_object
inline GLAPI GLboolean APIENTRY glIsRenderbufferEXT (GLuint renderbuffer) {
  return osi.glr->glExt.glIsRenderbufferEXT (renderbuffer);}
inline GLAPI void APIENTRY glBindRenderbufferEXT (GLenum target, GLuint renderbuffer) {
  osi.glr->glExt.glBindRenderbufferEXT (target, renderbuffer);}
inline GLAPI void APIENTRY glDeleteRenderbuffersEXT (GLsizei n, const GLuint *renderbuffers) {
  osi.glr->glExt.glDeleteRenderbuffersEXT (n, renderbuffers);}
inline GLAPI void APIENTRY glGenRenderbuffersEXT (GLsizei n, GLuint *renderbuffers) {
  osi.glr->glExt.glGenRenderbuffersEXT (n, renderbuffers);}
inline GLAPI void APIENTRY glRenderbufferStorageEXT (GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glRenderbufferStorageEXT (target, internalformat, width, height);}
inline GLAPI void APIENTRY glGetRenderbufferParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetRenderbufferParameterivEXT (target, pname, params);}
inline GLAPI GLboolean APIENTRY glIsFramebufferEXT (GLuint framebuffer) {
  return osi.glr->glExt.glIsFramebufferEXT (framebuffer);}
inline GLAPI void APIENTRY glBindFramebufferEXT (GLenum target, GLuint framebuffer) {
  osi.glr->glExt.glBindFramebufferEXT (target, framebuffer);}
inline GLAPI void APIENTRY glDeleteFramebuffersEXT (GLsizei n, const GLuint *framebuffers) {
  osi.glr->glExt.glDeleteFramebuffersEXT (n, framebuffers);}
inline GLAPI void APIENTRY glGenFramebuffersEXT (GLsizei n, GLuint *framebuffers) {
  osi.glr->glExt.glGenFramebuffersEXT (n, framebuffers);}
inline GLAPI GLenum APIENTRY glCheckFramebufferStatusEXT (GLenum target) {
  return osi.glr->glExt.glCheckFramebufferStatusEXT (target);}
inline GLAPI void APIENTRY glFramebufferTexture1DEXT (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  osi.glr->glExt.glFramebufferTexture1DEXT (target, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glFramebufferTexture2DEXT (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  osi.glr->glExt.glFramebufferTexture2DEXT (target, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glFramebufferTexture3DEXT (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
  osi.glr->glExt.glFramebufferTexture3DEXT (target, attachment, textarget, texture, level, zoffset);}
inline GLAPI void APIENTRY glFramebufferRenderbufferEXT (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
  osi.glr->glExt.glFramebufferRenderbufferEXT (target, attachment, renderbuffertarget, renderbuffer);}
inline GLAPI void APIENTRY glGetFramebufferAttachmentParameterivEXT (GLenum target, GLenum attachment, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFramebufferAttachmentParameterivEXT (target, attachment, pname, params);}
inline GLAPI void APIENTRY glGenerateMipmapEXT (GLenum target) {
  osi.glr->glExt.glGenerateMipmapEXT (target);}
// GL_EXT_geometry_shader4
inline GLAPI void APIENTRY glProgramParameteriEXT (GLuint program, GLenum pname, GLint value) {
  osi.glr->glExt.glProgramParameteriEXT (program, pname, value);}
// GL_EXT_gpu_program_parameters
inline GLAPI void APIENTRY glProgramEnvParameters4fvEXT (GLenum target, GLuint index, GLsizei count, const GLfloat *params) {
  osi.glr->glExt.glProgramEnvParameters4fvEXT (target, index, count, params);}
inline GLAPI void APIENTRY glProgramLocalParameters4fvEXT (GLenum target, GLuint index, GLsizei count, const GLfloat *params) {
  osi.glr->glExt.glProgramLocalParameters4fvEXT (target, index, count, params);}
// GL_EXT_gpu_shader4
inline GLAPI void APIENTRY glGetUniformuivEXT (GLuint program, GLint location, GLuint *params) {
  osi.glr->glExt.glGetUniformuivEXT (program, location, params);}
inline GLAPI void APIENTRY glBindFragDataLocationEXT (GLuint program, GLuint color, const GLchar *name) {
  osi.glr->glExt.glBindFragDataLocationEXT (program, color, name);}
inline GLAPI GLint APIENTRY glGetFragDataLocationEXT (GLuint program, const GLchar *name) {
  return osi.glr->glExt.glGetFragDataLocationEXT (program, name);}
inline GLAPI void APIENTRY glUniform1uiEXT (GLint location, GLuint v0) {
  osi.glr->glExt.glUniform1uiEXT (location, v0);}
inline GLAPI void APIENTRY glUniform2uiEXT (GLint location, GLuint v0, GLuint v1) {
  osi.glr->glExt.glUniform2uiEXT (location,  v0, v1);}
inline GLAPI void APIENTRY glUniform3uiEXT (GLint location, GLuint v0, GLuint v1, GLuint v2) {
  osi.glr->glExt.glUniform3uiEXT (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4uiEXT (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  osi.glr->glExt.glUniform4uiEXT (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1uivEXT (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform1uivEXT (location, count, value);}
inline GLAPI void APIENTRY glUniform2uivEXT (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform2uivEXT (location, count, value);}
inline GLAPI void APIENTRY glUniform3uivEXT (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform3uivEXT (location,  count, value);}
inline GLAPI void APIENTRY glUniform4uivEXT (GLint location, GLsizei count, const GLuint *value) {
  osi.glr->glExt.glUniform4uivEXT (location, count, value);}
// GL_EXT_histogram
inline GLAPI void APIENTRY glGetHistogramParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetHistogramParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetHistogramParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetHistogramParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmaxEXT (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values) {
  osi.glr->glExt.glGetMinmaxEXT (target, reset, format, type, values);}
inline GLAPI void APIENTRY glGetMinmaxParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMinmaxParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmaxParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMinmaxParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glHistogramEXT (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink) {
  osi.glr->glExt.glHistogramEXT (target, width, internalformat, sink);}
inline GLAPI void APIENTRY glMinmaxEXT (GLenum target, GLenum internalformat, GLboolean sink) {
  osi.glr->glExt.glMinmaxEXT (target, internalformat, sink);}
inline GLAPI void APIENTRY glResetHistogramEXT (GLenum target) {
  osi.glr->glExt.glResetHistogramEXT (target);}
inline GLAPI void APIENTRY glResetMinmaxEXT (GLenum target) {
  osi.glr->glExt.glResetMinmaxEXT (target);}
// GL_EXT_index_func
inline GLAPI void APIENTRY glIndexFuncEXT (GLenum func, GLclampf ref) {
  osi.glr->glExt.glIndexFuncEXT (func, ref);}
// GL_EXT_index_material
inline GLAPI void APIENTRY glIndexMaterialEXT (GLenum face, GLenum mode) {
  osi.glr->glExt.glIndexMaterialEXT (face, mode);}
// GL_EXT_light_texture
inline GLAPI void APIENTRY glApplyTextureEXT (GLenum mode) {
  osi.glr->glExt.glApplyTextureEXT (mode);}
inline GLAPI void APIENTRY glTextureLightEXT (GLenum pname) {
  osi.glr->glExt.glTextureLightEXT (pname);}
inline GLAPI void APIENTRY glTextureMaterialEXT (GLenum face, GLenum mode) {
  osi.glr->glExt.glTextureMaterialEXT (face, mode);}
// GL_EXT_multi_draw_arrays
inline GLAPI void APIENTRY glMultiDrawArraysEXT (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount) {
  osi.glr->glExt.glMultiDrawArraysEXT (mode, first, count, primcount);}
inline GLAPI void APIENTRY glMultiDrawElementsEXT (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount) {
  osi.glr->glExt.glMultiDrawElementsEXT (mode, count, type, indices, primcount);}
// GL_EXT_multisample
inline GLAPI void APIENTRY glSampleMaskEXT (GLclampf value, GLboolean invert) {
  osi.glr->glExt.glSampleMaskEXT (value, invert);}
inline GLAPI void APIENTRY glSamplePatternEXT (GLenum pattern) {
  osi.glr->glExt.glSamplePatternEXT (pattern);}
// GL_EXT_paletted_texture
inline GLAPI void APIENTRY glColorTableEXT (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void *table) {
  osi.glr->glExt.glColorTableEXT (target, internalFormat, width, format, type, table);}
inline GLAPI void APIENTRY glGetColorTableEXT (GLenum target, GLenum format, GLenum type, void *data) {
  osi.glr->glExt.glGetColorTableEXT (target, format, type, data);}
inline GLAPI void APIENTRY glGetColorTableParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetColorTableParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetColorTableParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetColorTableParameterfvEXT (target, pname, params);}
// GL_EXT_pixel_transform
inline GLAPI void APIENTRY glPixelTransformParameteriEXT (GLenum target, GLenum pname, GLint param) {
  osi.glr->glExt.glPixelTransformParameteriEXT (target, pname, param);}
inline GLAPI void APIENTRY glPixelTransformParameterfEXT (GLenum target, GLenum pname, GLfloat param) {
  osi.glr->glExt.glPixelTransformParameterfEXT (target, pname, param);}
inline GLAPI void APIENTRY glPixelTransformParameterivEXT (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glPixelTransformParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glPixelTransformParameterfvEXT (GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glPixelTransformParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetPixelTransformParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetPixelTransformParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetPixelTransformParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetPixelTransformParameterfvEXT (target, pname, params);}
// GL_EXT_point_parameters
inline GLAPI void APIENTRY glPointParameterfEXT (GLenum pname, GLfloat param) {
  osi.glr->glExt.glPointParameterfEXT (pname, param);}
inline GLAPI void APIENTRY glPointParameterfvEXT (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glPointParameterfvEXT (pname, params);}
// GL_EXT_polygon_offset
inline GLAPI void APIENTRY glPolygonOffsetEXT (GLfloat factor, GLfloat bias) {
  osi.glr->glExt.glPolygonOffsetEXT (factor, bias);}
// GL_EXT_provoking_vertex
inline GLAPI void APIENTRY glProvokingVertexEXT (GLenum mode) {
  osi.glr->glExt.glProvokingVertexEXT (mode);}
// GL_EXT_secondary_color
inline GLAPI void APIENTRY glSecondaryColor3bEXT (GLbyte red, GLbyte green, GLbyte blue) {
  osi.glr->glExt.glSecondaryColor3bEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3bvEXT (const GLbyte *v) {
  osi.glr->glExt.glSecondaryColor3bvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3dEXT (GLdouble red, GLdouble green, GLdouble blue) {
  osi.glr->glExt.glSecondaryColor3dEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3dvEXT (const GLdouble *v) {
  osi.glr->glExt.glSecondaryColor3dvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3fEXT (GLfloat red, GLfloat green, GLfloat blue) {
  osi.glr->glExt.glSecondaryColor3fEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3fvEXT (const GLfloat *v) {
  osi.glr->glExt.glSecondaryColor3fvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3iEXT (GLint red, GLint green, GLint blue) {
  osi.glr->glExt.glSecondaryColor3iEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3ivEXT (const GLint *v) {
  osi.glr->glExt.glSecondaryColor3ivEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3sEXT (GLshort red, GLshort green, GLshort blue) {
  osi.glr->glExt.glSecondaryColor3sEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3svEXT (const GLshort *v) {
  osi.glr->glExt.glSecondaryColor3svEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3ubEXT (GLubyte red, GLubyte green, GLubyte blue) {
  osi.glr->glExt.glSecondaryColor3ubEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3ubvEXT (const GLubyte *v) {
  osi.glr->glExt.glSecondaryColor3ubvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3uiEXT (GLuint red, GLuint green, GLuint blue) {
  osi.glr->glExt.glSecondaryColor3uiEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3uivEXT (const GLuint *v) {
  osi.glr->glExt.glSecondaryColor3uivEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3usEXT (GLushort red, GLushort green, GLushort blue) {
  osi.glr->glExt.glSecondaryColor3usEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3usvEXT (const GLushort *v) {
  osi.glr->glExt.glSecondaryColor3usvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColorPointerEXT (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glSecondaryColorPointerEXT (size, type, stride, pointer);}
// GL_EXT_separate_shader_objects
inline GLAPI void APIENTRY glUseShaderProgramEXT (GLenum type, GLuint program) {
  osi.glr->glExt.glUseShaderProgramEXT (type, program);}
inline GLAPI void APIENTRY glActiveProgramEXT (GLuint program) {
  osi.glr->glExt.glActiveProgramEXT (program);}
inline GLAPI GLuint APIENTRY glCreateShaderProgramEXT (GLenum type, const GLchar *string) {
  return osi.glr->glExt.glCreateShaderProgramEXT (type, string);}
// GL_EXT_shader_image_load_store
inline GLAPI void APIENTRY glBindImageTextureEXT (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format) {
  osi.glr->glExt.glBindImageTextureEXT (index, texture, level, layered, layer, access, format);}
inline GLAPI void APIENTRY glMemoryBarrierEXT (GLbitfield barriers) {
  osi.glr->glExt.glMemoryBarrierEXT (barriers);}
// GL_EXT_stencil_clear_tag
inline GLAPI void APIENTRY glStencilClearTagEXT (GLsizei stencilTagBits, GLuint stencilClearTag) {
  osi.glr->glExt.glStencilClearTagEXT (stencilTagBits, stencilClearTag);}
// GL_EXT_stencil_two_side
inline GLAPI void APIENTRY glActiveStencilFaceEXT (GLenum face) {
  osi.glr->glExt.glActiveStencilFaceEXT (face);}
// GL_EXT_subtexture
inline GLAPI void APIENTRY glTexSubImage1DEXT (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexSubImage1DEXT (target, level, xoffset, width, format, type, pixels);}
inline GLAPI void APIENTRY glTexSubImage2DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexSubImage2DEXT (target, level, xoffset, yoffset, width, height, format, type, pixels);}
// GL_EXT_texture3D
inline GLAPI void APIENTRY glTexImage3DEXT (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexImage3DEXT (target, level, internalformat, width, height, depth, border, format, type, pixels);}
inline GLAPI void APIENTRY glTexSubImage3DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexSubImage3DEXT (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);}
// GL_EXT_texture_buffer_object
inline GLAPI void APIENTRY glTexBufferEXT (GLenum target, GLenum internalformat, GLuint buffer) {
  osi.glr->glExt.glTexBufferEXT (target, internalformat, buffer);}
// GL_EXT_texture_integer
inline GLAPI void APIENTRY glTexParameterIivEXT (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glTexParameterIivEXT (target, pname, params);}
inline GLAPI void APIENTRY glTexParameterIuivEXT (GLenum target, GLenum pname, const GLuint *params) {
  osi.glr->glExt.glTexParameterIuivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIivEXT (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetTexParameterIivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIuivEXT (GLenum target, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetTexParameterIuivEXT (target, pname, params);}
inline GLAPI void APIENTRY glClearColorIiEXT (GLint red, GLint green, GLint blue, GLint alpha) {
  osi.glr->glExt.glClearColorIiEXT (red, green, blue, alpha);}
inline GLAPI void APIENTRY glClearColorIuiEXT (GLuint red, GLuint green, GLuint blue, GLuint alpha) {
  osi.glr->glExt.glClearColorIuiEXT (red, green, blue, alpha);}
// GL_EXT_texture_object
inline GLAPI GLboolean APIENTRY glAreTexturesResidentEXT (GLsizei n, const GLuint *textures, GLboolean *residences) {
  return osi.glr->glExt.glAreTexturesResidentEXT (n, textures, residences);}
inline GLAPI void APIENTRY glBindTextureEXT (GLenum target, GLuint texture) {
  osi.glr->glExt.glBindTextureEXT (target, texture);}
inline GLAPI void APIENTRY glDeleteTexturesEXT (GLsizei n, const GLuint *textures) {
  osi.glr->glExt.glDeleteTexturesEXT (n, textures);}
inline GLAPI void APIENTRY glGenTexturesEXT (GLsizei n, GLuint *textures) {
  osi.glr->glExt.glGenTexturesEXT (n, textures);}
inline GLAPI GLboolean APIENTRY glIsTextureEXT (GLuint texture) {
  return osi.glr->glExt.glIsTextureEXT (texture);}
inline GLAPI void APIENTRY glPrioritizeTexturesEXT (GLsizei n, const GLuint *textures, const GLclampf *priorities) {
  osi.glr->glExt.glPrioritizeTexturesEXT (n, textures, priorities);}
// GL_EXT_texture_perturb_normal
inline GLAPI void APIENTRY glTextureNormalEXT (GLenum mode) {
  osi.glr->glExt.glTextureNormalEXT (mode);}
// GL_EXT_timer_query
inline GLAPI void APIENTRY glGetQueryObjecti64vEXT (GLuint id, GLenum pname, GLint64 *params) {
  osi.glr->glExt.glGetQueryObjecti64vEXT (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectui64vEXT (GLuint id, GLenum pname, GLuint64 *params) {
  osi.glr->glExt.glGetQueryObjectui64vEXT (id, pname, params);}
// GL_EXT_transform_feedback
inline GLAPI void APIENTRY glBeginTransformFeedbackEXT (GLenum primitiveMode) {
  osi.glr->glExt.glBeginTransformFeedbackEXT (primitiveMode);}
inline GLAPI void APIENTRY glEndTransformFeedbackEXT (void) {
  osi.glr->glExt.glEndTransformFeedbackEXT ();}
inline GLAPI void APIENTRY glBindBufferRangeEXT (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  osi.glr->glExt.glBindBufferRangeEXT (target, index, buffer, offset, size);}
inline GLAPI void APIENTRY glBindBufferOffsetEXT (GLenum target, GLuint index, GLuint buffer, GLintptr offset) {
  osi.glr->glExt.glBindBufferOffsetEXT (target, index, buffer, offset);}
inline GLAPI void APIENTRY glBindBufferBaseEXT (GLenum target, GLuint index, GLuint buffer) {
  osi.glr->glExt.glBindBufferBaseEXT (target, index,  buffer);}
inline GLAPI void APIENTRY glTransformFeedbackVaryingsEXT (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode) {
  osi.glr->glExt.glTransformFeedbackVaryingsEXT (program, count, varyings, bufferMode);}
inline GLAPI void APIENTRY glGetTransformFeedbackVaryingEXT (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
  osi.glr->glExt.glGetTransformFeedbackVaryingEXT (program, index, bufSize, length, size, type, name);}
// GL_EXT_vertex_array
inline GLAPI void APIENTRY glArrayElementEXT (GLint i) {
  osi.glr->glExt.glArrayElementEXT (i);}
inline GLAPI void APIENTRY glColorPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  osi.glr->glExt.glColorPointerEXT (size, type, stride, count, pointer);}
inline GLAPI void APIENTRY glDrawArraysEXT (GLenum mode, GLint first, GLsizei count) {
  osi.glr->glExt.glDrawArraysEXT (mode, first, count);}
inline GLAPI void APIENTRY glEdgeFlagPointerEXT (GLsizei stride, GLsizei count, const GLboolean *pointer) {
  osi.glr->glExt.glEdgeFlagPointerEXT (stride, count, pointer);}
inline GLAPI void APIENTRY glGetPointervEXT (GLenum pname, void **params) {
  osi.glr->glExt.glGetPointervEXT (pname, params);}
inline GLAPI void APIENTRY glIndexPointerEXT (GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  osi.glr->glExt.glIndexPointerEXT (type, stride, count, pointer);}
inline GLAPI void APIENTRY glNormalPointerEXT (GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  osi.glr->glExt.glNormalPointerEXT (type, stride, count, pointer);}
inline GLAPI void APIENTRY glTexCoordPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  osi.glr->glExt.glTexCoordPointerEXT (size, type, stride, count, pointer);}
inline GLAPI void APIENTRY glVertexPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  osi.glr->glExt.glVertexPointerEXT (size, type, stride, count, pointer);}
// GL_EXT_vertex_attrib_64bit
inline GLAPI void APIENTRY glVertexAttribL1dEXT (GLuint index, GLdouble x) {
  osi.glr->glExt.glVertexAttribL1dEXT (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2dEXT (GLuint index, GLdouble x, GLdouble y) {
  osi.glr->glExt.glVertexAttribL2dEXT (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3dEXT (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glVertexAttribL3dEXT (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4dEXT (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glVertexAttribL4dEXT (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1dvEXT (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL1dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2dvEXT (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL2dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3dvEXT (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL3dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4dvEXT (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribL4dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribLPointerEXT (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexAttribLPointerEXT (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribLdvEXT (GLuint index, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetVertexAttribLdvEXT (index, pname, params);}
// GL_EXT_vertex_shader
inline GLAPI void APIENTRY glBeginVertexShaderEXT (void) {
  osi.glr->glExt.glBeginVertexShaderEXT ();}
inline GLAPI void APIENTRY glEndVertexShaderEXT (void) {
  osi.glr->glExt.glEndVertexShaderEXT ();}
inline GLAPI void APIENTRY glBindVertexShaderEXT (GLuint id) {
  osi.glr->glExt.glBindVertexShaderEXT (id);}
inline GLAPI GLuint APIENTRY glGenVertexShadersEXT (GLuint range) {
  return osi.glr->glExt.glGenVertexShadersEXT (range);}
inline GLAPI void APIENTRY glDeleteVertexShaderEXT (GLuint id) {
  osi.glr->glExt.glDeleteVertexShaderEXT (id);}
inline GLAPI void APIENTRY glShaderOp1EXT (GLenum op, GLuint res, GLuint arg1) {
  osi.glr->glExt.glShaderOp1EXT (op, res, arg1);}
inline GLAPI void APIENTRY glShaderOp2EXT (GLenum op, GLuint res, GLuint arg1, GLuint arg2) {
  osi.glr->glExt.glShaderOp2EXT (op, res, arg1, arg2);}
inline GLAPI void APIENTRY glShaderOp3EXT (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3) {
  osi.glr->glExt.glShaderOp3EXT (op, res, arg1, arg2, arg3);}
inline GLAPI void APIENTRY glSwizzleEXT (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW) {
  osi.glr->glExt.glSwizzleEXT (res, in, outX, outY, outZ, outW);}
inline GLAPI void APIENTRY glWriteMaskEXT (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW) {
  osi.glr->glExt.glWriteMaskEXT (res, in, outX, outY, outZ, outW);}
inline GLAPI void APIENTRY glInsertComponentEXT (GLuint res, GLuint src, GLuint num) {
  osi.glr->glExt.glInsertComponentEXT (res, src, num);}
inline GLAPI void APIENTRY glExtractComponentEXT (GLuint res, GLuint src, GLuint num) {
  osi.glr->glExt.glExtractComponentEXT (res, src, num);}
inline GLAPI GLuint APIENTRY glGenSymbolsEXT (GLenum datatype, GLenum storagetype, GLenum range, GLuint components) {
  return osi.glr->glExt.glGenSymbolsEXT (datatype, storagetype, range, components);}
inline GLAPI void APIENTRY glSetInvariantEXT (GLuint id, GLenum type, const void *addr) {
  osi.glr->glExt.glSetInvariantEXT (id, type, addr);}
inline GLAPI void APIENTRY glSetLocalConstantEXT (GLuint id, GLenum type, const void *addr) {
  osi.glr->glExt.glSetLocalConstantEXT (id, type, addr);}
inline GLAPI void APIENTRY glVariantbvEXT (GLuint id, const GLbyte *addr) {
  osi.glr->glExt.glVariantbvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantsvEXT (GLuint id, const GLshort *addr) {
  osi.glr->glExt.glVariantsvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantivEXT (GLuint id, const GLint *addr) {
  osi.glr->glExt.glVariantivEXT (id, addr);}
inline GLAPI void APIENTRY glVariantfvEXT (GLuint id, const GLfloat *addr) {
  osi.glr->glExt.glVariantfvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantdvEXT (GLuint id, const GLdouble *addr) {
  osi.glr->glExt.glVariantdvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantubvEXT (GLuint id, const GLubyte *addr) {
  osi.glr->glExt.glVariantubvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantusvEXT (GLuint id, const GLushort *addr) {
  osi.glr->glExt.glVariantusvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantuivEXT (GLuint id, const GLuint *addr) {
  osi.glr->glExt.glVariantuivEXT (id, addr);}
inline GLAPI void APIENTRY glVariantPointerEXT (GLuint id, GLenum type, GLuint stride, const void *addr) {
  osi.glr->glExt.glVariantPointerEXT (id, type, stride, addr);}
inline GLAPI void APIENTRY glEnableVariantClientStateEXT (GLuint id) {
  osi.glr->glExt.glEnableVariantClientStateEXT (id);}
inline GLAPI void APIENTRY glDisableVariantClientStateEXT (GLuint id) {
  osi.glr->glExt.glDisableVariantClientStateEXT (id);}
inline GLAPI GLuint APIENTRY glBindLightParameterEXT (GLenum light, GLenum value) {
  return osi.glr->glExt.glBindLightParameterEXT (light,  value);}
inline GLAPI GLuint APIENTRY glBindMaterialParameterEXT (GLenum face, GLenum value) {
  return osi.glr->glExt.glBindMaterialParameterEXT (face, value);}
inline GLAPI GLuint APIENTRY glBindTexGenParameterEXT (GLenum unit, GLenum coord, GLenum value) {
  return osi.glr->glExt.glBindTexGenParameterEXT (unit, coord,  value);}
inline GLAPI GLuint APIENTRY glBindTextureUnitParameterEXT (GLenum unit, GLenum value) {
  return osi.glr->glExt.glBindTextureUnitParameterEXT (unit, value);}
inline GLAPI GLuint APIENTRY glBindParameterEXT (GLenum value) {
  return osi.glr->glExt.glBindParameterEXT (value);}
inline GLAPI GLboolean APIENTRY glIsVariantEnabledEXT (GLuint id, GLenum cap) {
  return osi.glr->glExt.glIsVariantEnabledEXT (id, cap);}
inline GLAPI void APIENTRY glGetVariantBooleanvEXT (GLuint id, GLenum value, GLboolean *data) {
  osi.glr->glExt.glGetVariantBooleanvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetVariantIntegervEXT (GLuint id, GLenum value, GLint *data) {
  osi.glr->glExt.glGetVariantIntegervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetVariantFloatvEXT (GLuint id, GLenum value, GLfloat *data) {
  osi.glr->glExt.glGetVariantFloatvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetVariantPointervEXT (GLuint id, GLenum value, void **data) {
  osi.glr->glExt.glGetVariantPointervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetInvariantBooleanvEXT (GLuint id, GLenum value, GLboolean *data) {
  osi.glr->glExt.glGetInvariantBooleanvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetInvariantIntegervEXT (GLuint id, GLenum value, GLint *data) {
  osi.glr->glExt.glGetInvariantIntegervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetInvariantFloatvEXT (GLuint id, GLenum value, GLfloat *data) {
  osi.glr->glExt.glGetInvariantFloatvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetLocalConstantBooleanvEXT (GLuint id, GLenum value, GLboolean *data) {
  osi.glr->glExt.glGetLocalConstantBooleanvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetLocalConstantIntegervEXT (GLuint id, GLenum value, GLint *data) {
  osi.glr->glExt.glGetLocalConstantIntegervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetLocalConstantFloatvEXT (GLuint id, GLenum value, GLfloat *data) {
  osi.glr->glExt.glGetLocalConstantFloatvEXT (id, value, data);}
// GL_EXT_vertex_weighting
inline GLAPI void APIENTRY glVertexWeightfEXT (GLfloat weight) {
  osi.glr->glExt.glVertexWeightfEXT (weight);}
inline GLAPI void APIENTRY glVertexWeightfvEXT (const GLfloat *weight) {
  osi.glr->glExt.glVertexWeightfvEXT (weight);}
inline GLAPI void APIENTRY glVertexWeightPointerEXT (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexWeightPointerEXT (size, type, stride, pointer);}
// GL_EXT_x11_sync_object
inline GLAPI GLsync APIENTRY glImportSyncEXT (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags) {
  return osi.glr->glExt.glImportSyncEXT (external_sync_type, external_sync, flags);}
// GL_GREMEDY_frame_terminator
inline GLAPI void APIENTRY glFrameTerminatorGREMEDY (void) {
  osi.glr->glExt.glFrameTerminatorGREMEDY ();}
// GL_GREMEDY_string_marker
inline GLAPI void APIENTRY glStringMarkerGREMEDY (GLsizei len, const void *string) {
  osi.glr->glExt.glStringMarkerGREMEDY (len, string);}
// GL_HP_image_transform
inline GLAPI void APIENTRY glImageTransformParameteriHP (GLenum target, GLenum pname, GLint param) {
  osi.glr->glExt.glImageTransformParameteriHP (target, pname, param);}
inline GLAPI void APIENTRY glImageTransformParameterfHP (GLenum target, GLenum pname, GLfloat param) {
  osi.glr->glExt.glImageTransformParameterfHP (target, pname, param);}
inline GLAPI void APIENTRY glImageTransformParameterivHP (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glImageTransformParameterivHP (target, pname, params);}
inline GLAPI void APIENTRY glImageTransformParameterfvHP (GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glImageTransformParameterfvHP (target, pname, params);}
inline GLAPI void APIENTRY glGetImageTransformParameterivHP (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetImageTransformParameterivHP (target, pname, params);}
inline GLAPI void APIENTRY glGetImageTransformParameterfvHP (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetImageTransformParameterfvHP (target, pname, params);}
// GL_IBM_multimode_draw_arrays
inline GLAPI void APIENTRY glMultiModeDrawArraysIBM (const GLenum *mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride) {
  osi.glr->glExt.glMultiModeDrawArraysIBM (mode, first, count, primcount, modestride);}
inline GLAPI void APIENTRY glMultiModeDrawElementsIBM (const GLenum *mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount, GLint modestride) {
  osi.glr->glExt.glMultiModeDrawElementsIBM (mode, count, type, indices, primcount, modestride);}
// GL_IBM_static_data
inline GLAPI void APIENTRY glFlushStaticDataIBM (GLenum target) {
  osi.glr->glExt.glFlushStaticDataIBM (target);}
// GL_IBM_vertex_array_lists
inline GLAPI void APIENTRY glColorPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  osi.glr->glExt.glColorPointerListIBM (size, type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glSecondaryColorPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  osi.glr->glExt.glSecondaryColorPointerListIBM (size, type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glEdgeFlagPointerListIBM (GLint stride, const GLboolean **pointer, GLint ptrstride) {
  osi.glr->glExt.glEdgeFlagPointerListIBM (stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glFogCoordPointerListIBM (GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  osi.glr->glExt.glFogCoordPointerListIBM (type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glIndexPointerListIBM (GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  osi.glr->glExt.glIndexPointerListIBM (type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glNormalPointerListIBM (GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  osi.glr->glExt.glNormalPointerListIBM (type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glTexCoordPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  osi.glr->glExt.glTexCoordPointerListIBM (size, type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glVertexPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  osi.glr->glExt.glVertexPointerListIBM (size, type, stride, pointer, ptrstride);}
// GL_INGR_blend_func_separate
inline GLAPI void APIENTRY glBlendFuncSeparateINGR (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
  osi.glr->glExt.glBlendFuncSeparateINGR (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);}
// GL_INTEL_map_texture
inline GLAPI void APIENTRY glSyncTextureINTEL (GLuint texture) {
  osi.glr->glExt.glSyncTextureINTEL (texture);}
inline GLAPI void APIENTRY glUnmapTexture2DINTEL (GLuint texture, GLint level) {
  osi.glr->glExt.glUnmapTexture2DINTEL (texture, level);}
inline GLAPI void *APIENTRY glMapTexture2DINTEL (GLuint texture, GLint level, GLbitfield access, GLint *stride, GLenum *layout) {
  return osi.glr->glExt.glMapTexture2DINTEL (texture, level, access, stride, layout);}
// GL_INTEL_parallel_arrays
inline GLAPI void APIENTRY glVertexPointervINTEL (GLint size, GLenum type, const void **pointer) {
  osi.glr->glExt.glVertexPointervINTEL (size, type, pointer);}
inline GLAPI void APIENTRY glNormalPointervINTEL (GLenum type, const void **pointer) {
  osi.glr->glExt.glNormalPointervINTEL (type, pointer);}
inline GLAPI void APIENTRY glColorPointervINTEL (GLint size, GLenum type, const void **pointer) {
  osi.glr->glExt.glColorPointervINTEL (size, type, pointer);}
inline GLAPI void APIENTRY glTexCoordPointervINTEL (GLint size, GLenum type, const void **pointer) {
  osi.glr->glExt.glTexCoordPointervINTEL (size, type, pointer);}
// GL_INTEL_performance_query
inline GLAPI void APIENTRY glBeginPerfQueryINTEL (GLuint queryHandle) {
  osi.glr->glExt.glBeginPerfQueryINTEL (queryHandle);}
inline GLAPI void APIENTRY glCreatePerfQueryINTEL (GLuint queryId, GLuint *queryHandle) {
  osi.glr->glExt.glCreatePerfQueryINTEL (queryId, queryHandle);}
inline GLAPI void APIENTRY glDeletePerfQueryINTEL (GLuint queryHandle) {
  osi.glr->glExt.glDeletePerfQueryINTEL (queryHandle);}
inline GLAPI void APIENTRY glEndPerfQueryINTEL (GLuint queryHandle) {
  osi.glr->glExt.glEndPerfQueryINTEL (queryHandle);}
inline GLAPI void APIENTRY glGetFirstPerfQueryIdINTEL (GLuint *queryId) {
  osi.glr->glExt.glGetFirstPerfQueryIdINTEL (queryId);}
inline GLAPI void APIENTRY glGetNextPerfQueryIdINTEL (GLuint queryId, GLuint *nextQueryId) {
  osi.glr->glExt.glGetNextPerfQueryIdINTEL (queryId, nextQueryId);}
inline GLAPI void APIENTRY glGetPerfCounterInfoINTEL (GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar *counterName, GLuint counterDescLength, GLchar *counterDesc, GLuint *counterOffset, GLuint *counterDataSize, GLuint *counterTypeEnum, GLuint *counterDataTypeEnum, GLuint64 *rawCounterMaxValue) {
  osi.glr->glExt.glGetPerfCounterInfoINTEL (queryId, counterId, counterNameLength, counterName, counterDescLength, counterDesc, counterOffset, counterDataSize, counterTypeEnum, counterDataTypeEnum, rawCounterMaxValue);}
inline GLAPI void APIENTRY glGetPerfQueryDataINTEL (GLuint queryHandle, GLuint flags, GLsizei dataSize, GLvoid *data, GLuint *bytesWritten) {
  osi.glr->glExt.glGetPerfQueryDataINTEL (queryHandle, flags, dataSize, data, bytesWritten);}
inline GLAPI void APIENTRY glGetPerfQueryIdByNameINTEL (GLchar *queryName, GLuint *queryId) {
  osi.glr->glExt.glGetPerfQueryIdByNameINTEL (queryName, queryId);}
inline GLAPI void APIENTRY glGetPerfQueryInfoINTEL (GLuint queryId, GLuint queryNameLength, GLchar *queryName, GLuint *dataSize, GLuint *noCounters, GLuint *noInstances, GLuint *capsMask) {
  osi.glr->glExt.glGetPerfQueryInfoINTEL (queryId, queryNameLength, queryName, dataSize, noCounters, noInstances, capsMask);}
// GL_MESA_resize_buffers
inline GLAPI void APIENTRY glResizeBuffersMESA (void) {
  osi.glr->glExt.glResizeBuffersMESA ();}
// GL_MESA_window_pos
inline GLAPI void APIENTRY glWindowPos2dMESA (GLdouble x, GLdouble y) {
  osi.glr->glExt.glWindowPos2dMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2dvMESA (const GLdouble *v) {
  osi.glr->glExt.glWindowPos2dvMESA (v);}
inline GLAPI void APIENTRY glWindowPos2fMESA (GLfloat x, GLfloat y) {
  osi.glr->glExt.glWindowPos2fMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2fvMESA (const GLfloat *v) {
  osi.glr->glExt.glWindowPos2fvMESA (v);}
inline GLAPI void APIENTRY glWindowPos2iMESA (GLint x, GLint y) {
  osi.glr->glExt.glWindowPos2iMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2ivMESA (const GLint *v) {
  osi.glr->glExt.glWindowPos2ivMESA (v);}
inline GLAPI void APIENTRY glWindowPos2sMESA (GLshort x, GLshort y) {
  osi.glr->glExt.glWindowPos2sMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2svMESA (const GLshort *v) {
  osi.glr->glExt.glWindowPos2svMESA (v);}
inline GLAPI void APIENTRY glWindowPos3dMESA (GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glWindowPos3dMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3dvMESA (const GLdouble *v) {
  osi.glr->glExt.glWindowPos3dvMESA (v);}
inline GLAPI void APIENTRY glWindowPos3fMESA (GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glWindowPos3fMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3fvMESA (const GLfloat *v) {
  osi.glr->glExt.glWindowPos3fvMESA (v);}
inline GLAPI void APIENTRY glWindowPos3iMESA (GLint x, GLint y, GLint z) {
  osi.glr->glExt.glWindowPos3iMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3ivMESA (const GLint *v) {
  osi.glr->glExt.glWindowPos3ivMESA (v);}
inline GLAPI void APIENTRY glWindowPos3sMESA (GLshort x, GLshort y, GLshort z) {
  osi.glr->glExt.glWindowPos3sMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3svMESA (const GLshort *v) {
  osi.glr->glExt.glWindowPos3svMESA (v);}
inline GLAPI void APIENTRY glWindowPos4dMESA (GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glWindowPos4dMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4dvMESA (const GLdouble *v) {
  osi.glr->glExt.glWindowPos4dvMESA (v);}
inline GLAPI void APIENTRY glWindowPos4fMESA (GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glWindowPos4fMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4fvMESA (const GLfloat *v) {
  osi.glr->glExt.glWindowPos4fvMESA (v);}
inline GLAPI void APIENTRY glWindowPos4iMESA (GLint x, GLint y, GLint z, GLint w) {
  osi.glr->glExt.glWindowPos4iMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4ivMESA (const GLint *v) {
  osi.glr->glExt.glWindowPos4ivMESA (v);}
inline GLAPI void APIENTRY glWindowPos4sMESA (GLshort x, GLshort y, GLshort z, GLshort w) {
  osi.glr->glExt.glWindowPos4sMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4svMESA (const GLshort *v) {
  osi.glr->glExt.glWindowPos4svMESA (v);}
// GL_NVX_conditional_render
inline GLAPI void APIENTRY glBeginConditionalRenderNVX (GLuint id) {
  osi.glr->glExt.glBeginConditionalRenderNVX (id);}
inline GLAPI void APIENTRY glEndConditionalRenderNVX (void) {
  osi.glr->glExt.glEndConditionalRenderNVX ();}
// GL_NV_bindless_multi_draw_indirect
inline GLAPI void APIENTRY glMultiDrawArraysIndirectBindlessNV (GLenum mode, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount) {
  osi.glr->glExt.glMultiDrawArraysIndirectBindlessNV (mode, indirect, drawCount, stride, vertexBufferCount);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirectBindlessNV (GLenum mode, GLenum type, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount) {
  osi.glr->glExt.glMultiDrawElementsIndirectBindlessNV (mode, type, indirect, drawCount, stride, vertexBufferCount);}
// GL_NV_bindless_texture
inline GLAPI GLuint64 APIENTRY glGetTextureHandleNV (GLuint texture) {
  return osi.glr->glExt.glGetTextureHandleNV (texture);}
inline GLAPI GLuint64 APIENTRY glGetTextureSamplerHandleNV (GLuint texture, GLuint sampler) {
  return osi.glr->glExt.glGetTextureSamplerHandleNV (texture, sampler);}
inline GLAPI void APIENTRY glMakeTextureHandleResidentNV (GLuint64 handle) {
  osi.glr->glExt.glMakeTextureHandleResidentNV (handle);}
inline GLAPI void APIENTRY glMakeTextureHandleNonResidentNV (GLuint64 handle) {
  osi.glr->glExt.glMakeTextureHandleNonResidentNV (handle);}
inline GLAPI GLuint64 APIENTRY glGetImageHandleNV (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format) {
  return osi.glr->glExt.glGetImageHandleNV (texture, level, layered, layer, format);}
inline GLAPI void APIENTRY glMakeImageHandleResidentNV (GLuint64 handle, GLenum access) {
  osi.glr->glExt.glMakeImageHandleResidentNV (handle, access);}
inline GLAPI void APIENTRY glMakeImageHandleNonResidentNV (GLuint64 handle) {
  osi.glr->glExt.glMakeImageHandleNonResidentNV (handle);}
inline GLAPI void APIENTRY glUniformHandleui64NV (GLint location, GLuint64 value) {
  osi.glr->glExt.glUniformHandleui64NV (location, value);}
inline GLAPI void APIENTRY glUniformHandleui64vNV (GLint location, GLsizei count, const GLuint64 *value) {
  osi.glr->glExt.glUniformHandleui64vNV (location, count, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64NV (GLuint program, GLint location, GLuint64 value) {
  osi.glr->glExt.glProgramUniformHandleui64NV (program, location, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64 *values) {
  osi.glr->glExt.glProgramUniformHandleui64vNV (program, location, count, values);}
inline GLAPI GLboolean APIENTRY glIsTextureHandleResidentNV (GLuint64 handle) {
  return osi.glr->glExt.glIsTextureHandleResidentNV (handle);}
inline GLAPI GLboolean APIENTRY glIsImageHandleResidentNV (GLuint64 handle) {
  return osi.glr->glExt.glIsImageHandleResidentNV (handle);}
// GL_NV_blend_equation_advanced
inline GLAPI void APIENTRY glBlendParameteriNV (GLenum pname, GLint value) {
  osi.glr->glExt.glBlendParameteriNV (pname, value);}
inline GLAPI void APIENTRY glBlendBarrierNV (void) {
  osi.glr->glExt.glBlendBarrierNV ();}
// GL_NV_conditional_render
inline GLAPI void APIENTRY glBeginConditionalRenderNV (GLuint id, GLenum mode) {
  osi.glr->glExt.glBeginConditionalRenderNV (id, mode);}
inline GLAPI void APIENTRY glEndConditionalRenderNV (void) {
  osi.glr->glExt.glEndConditionalRenderNV ();}
// GL_NV_copy_image WGL_NV_copy_image GLX_NV_copy_image
inline GLAPI void APIENTRY glCopyImageSubDataNV (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
  osi.glr->glExt.glCopyImageSubDataNV (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);}
#ifdef OS_LINUX
inline void glXCopyImageSubDataNV (Display *dpy, GLXContext srcCtx, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLXContext dstCtx, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
  osi.glr->glExt. glXCopyImageSubDataNV (dpy, srcCtx, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstCtx, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);}
#endif /// OS_LINUX
#ifdef OS_WIN
inline BOOL WINAPI wglCopyImageSubDataNV (HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
  return osi.glr->glExt.wglCopyImageSubDataNV (hSrcRC, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, hDstRC, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);}
#endif /// OS_WIN
// GL_NV_depth_buffer_float
inline GLAPI void APIENTRY glDepthRangedNV (GLdouble zNear, GLdouble zFar) {
  osi.glr->glExt.glDepthRangedNV (zNear, zFar);}
inline GLAPI void APIENTRY glClearDepthdNV (GLdouble depth) {
  osi.glr->glExt.glClearDepthdNV (depth);}
inline GLAPI void APIENTRY glDepthBoundsdNV (GLdouble zmin, GLdouble zmax) {
  osi.glr->glExt.glDepthBoundsdNV (zmin, zmax);}
// GL_NV_draw_texture
inline GLAPI void APIENTRY glDrawTextureNV (GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1) {
  osi.glr->glExt.glDrawTextureNV (texture, sampler, x0, y0, x1, y1, z, s0, t0, s1, t1);}
// GL_NV_evaluators
inline GLAPI void APIENTRY glMapControlPointsNV (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void *points) {
  osi.glr->glExt.glMapControlPointsNV (target, index, type, ustride, vstride, uorder, vorder, packed, points);}
inline GLAPI void APIENTRY glMapParameterivNV (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glMapParameterivNV (target, pname, params);}
inline GLAPI void APIENTRY glMapParameterfvNV (GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glMapParameterfvNV (target, pname, params);}
inline GLAPI void APIENTRY glGetMapControlPointsNV (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void *points) {
  osi.glr->glExt.glGetMapControlPointsNV (target, index, type, ustride, vstride, packed, points);}
inline GLAPI void APIENTRY glGetMapParameterivNV (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMapParameterivNV (target, pname, params);}
inline GLAPI void APIENTRY glGetMapParameterfvNV (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMapParameterfvNV (target, pname, params);}
inline GLAPI void APIENTRY glGetMapAttribParameterivNV (GLenum target, GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetMapAttribParameterivNV (target, index, pname, params);}
inline GLAPI void APIENTRY glGetMapAttribParameterfvNV (GLenum target, GLuint index, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetMapAttribParameterfvNV (target, index, pname, params);}
inline GLAPI void APIENTRY glEvalMapsNV (GLenum target, GLenum mode) {
  osi.glr->glExt.glEvalMapsNV (target, mode);}
// GL_NV_explicit_multisample
inline GLAPI void APIENTRY glGetMultisamplefvNV (GLenum pname, GLuint index, GLfloat *val) {
  osi.glr->glExt.glGetMultisamplefvNV (pname, index, val);}
inline GLAPI void APIENTRY glSampleMaskIndexedNV (GLuint index, GLbitfield mask) {
  osi.glr->glExt.glSampleMaskIndexedNV (index, mask);}
inline GLAPI void APIENTRY glTexRenderbufferNV (GLenum target, GLuint renderbuffer) {
  osi.glr->glExt.glTexRenderbufferNV (target, renderbuffer);}
// GL_NV_fence
inline GLAPI void APIENTRY glDeleteFencesNV (GLsizei n, const GLuint *fences) {
  osi.glr->glExt.glDeleteFencesNV (n, fences);}
inline GLAPI void APIENTRY glGenFencesNV (GLsizei n, GLuint *fences) {
  osi.glr->glExt.glGenFencesNV (n, fences);}
inline GLAPI GLboolean APIENTRY glIsFenceNV (GLuint fence) {
  return osi.glr->glExt.glIsFenceNV (fence);}
inline GLAPI GLboolean APIENTRY glTestFenceNV (GLuint fence) {
  return osi.glr->glExt.glTestFenceNV (fence);}
inline GLAPI void APIENTRY glGetFenceivNV (GLuint fence, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFenceivNV (fence, pname, params);}
inline GLAPI void APIENTRY glFinishFenceNV (GLuint fence) {
  osi.glr->glExt.glFinishFenceNV (fence);}
inline GLAPI void APIENTRY glSetFenceNV (GLuint fence, GLenum condition) {
  osi.glr->glExt.glSetFenceNV (fence, condition);}
// GL_NV_fragment_program
inline GLAPI void APIENTRY glProgramNamedParameter4fNV (GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glProgramNamedParameter4fNV (id, len, name, x, y, z, w);}
inline GLAPI void APIENTRY glProgramNamedParameter4fvNV (GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v) {
  osi.glr->glExt.glProgramNamedParameter4fvNV (id, len, name, v);}
inline GLAPI void APIENTRY glProgramNamedParameter4dNV (GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glProgramNamedParameter4dNV (id, len, name, x, y, z, w);}
inline GLAPI void APIENTRY glProgramNamedParameter4dvNV (GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v) {
  osi.glr->glExt.glProgramNamedParameter4dvNV (id, len, name, v);}
inline GLAPI void APIENTRY glGetProgramNamedParameterfvNV (GLuint id, GLsizei len, const GLubyte *name, GLfloat *params) {
  osi.glr->glExt.glGetProgramNamedParameterfvNV (id, len, name, params);}
inline GLAPI void APIENTRY glGetProgramNamedParameterdvNV (GLuint id, GLsizei len, const GLubyte *name, GLdouble *params) {
  osi.glr->glExt.glGetProgramNamedParameterdvNV (id, len, name, params);}
// GL_NV_framebuffer_multisample_coverage
inline GLAPI void APIENTRY glRenderbufferStorageMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height) {
  osi.glr->glExt.glRenderbufferStorageMultisampleCoverageNV (target, coverageSamples, colorSamples, internalformat, width, height);}
// GL_NV_geometry_program4
inline GLAPI void APIENTRY glProgramVertexLimitNV (GLenum target, GLint limit) {
  osi.glr->glExt.glProgramVertexLimitNV (target, limit);}
inline GLAPI void APIENTRY glFramebufferTextureEXT (GLenum target, GLenum attachment, GLuint texture, GLint level) {
  osi.glr->glExt.glFramebufferTextureEXT (target, attachment, texture, level);}
inline GLAPI void APIENTRY glFramebufferTextureLayerEXT (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  osi.glr->glExt.glFramebufferTextureLayerEXT (target, attachment, texture, level, layer);}
inline GLAPI void APIENTRY glFramebufferTextureFaceEXT (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face) {
  osi.glr->glExt.glFramebufferTextureFaceEXT (target, attachment, texture, level, face);}
// GL_NV_gpu_program4
inline GLAPI void APIENTRY glProgramLocalParameterI4iNV (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w) {
  osi.glr->glExt.glProgramLocalParameterI4iNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameterI4ivNV (GLenum target, GLuint index, const GLint *params) {
  osi.glr->glExt.glProgramLocalParameterI4ivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParametersI4ivNV (GLenum target, GLuint index, GLsizei count, const GLint *params) {
  osi.glr->glExt.glProgramLocalParametersI4ivNV (target, index, count, params);}
inline GLAPI void APIENTRY glProgramLocalParameterI4uiNV (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  osi.glr->glExt.glProgramLocalParameterI4uiNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameterI4uivNV (GLenum target, GLuint index, const GLuint *params) {
  osi.glr->glExt.glProgramLocalParameterI4uivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParametersI4uivNV (GLenum target, GLuint index, GLsizei count, const GLuint *params) {
  osi.glr->glExt.glProgramLocalParametersI4uivNV (target, index, count, params);}
inline GLAPI void APIENTRY glProgramEnvParameterI4iNV (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w) {
  osi.glr->glExt.glProgramEnvParameterI4iNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameterI4ivNV (GLenum target, GLuint index, const GLint *params) {
  osi.glr->glExt.glProgramEnvParameterI4ivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramEnvParametersI4ivNV (GLenum target, GLuint index, GLsizei count, const GLint *params) {
  osi.glr->glExt.glProgramEnvParametersI4ivNV (target, index, count, params);}
inline GLAPI void APIENTRY glProgramEnvParameterI4uiNV (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  osi.glr->glExt.glProgramEnvParameterI4uiNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameterI4uivNV (GLenum target, GLuint index, const GLuint *params) {
  osi.glr->glExt.glProgramEnvParameterI4uivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramEnvParametersI4uivNV (GLenum target, GLuint index, GLsizei count, const GLuint *params) {
  osi.glr->glExt.glProgramEnvParametersI4uivNV (target, index, count, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterIivNV (GLenum target, GLuint index, GLint *params) {
  osi.glr->glExt.glGetProgramLocalParameterIivNV (target, index, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterIuivNV (GLenum target, GLuint index, GLuint *params) {
  osi.glr->glExt.glGetProgramLocalParameterIuivNV (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterIivNV (GLenum target, GLuint index, GLint *params) {
  osi.glr->glExt.glGetProgramEnvParameterIivNV (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterIuivNV (GLenum target, GLuint index, GLuint *params) {
  osi.glr->glExt.glGetProgramEnvParameterIuivNV (target, index, params);}
// GL_NV_gpu_program5
inline GLAPI void APIENTRY glProgramSubroutineParametersuivNV (GLenum target, GLsizei count, const GLuint *params) {
  osi.glr->glExt.glProgramSubroutineParametersuivNV (target, count, params);}
inline GLAPI void APIENTRY glGetProgramSubroutineParameteruivNV (GLenum target, GLuint index, GLuint *param) {
  osi.glr->glExt.glGetProgramSubroutineParameteruivNV (target, index, param);}
// GL_NV_half_float
inline GLAPI void APIENTRY glVertex2hNV (GLhalfNV x, GLhalfNV y) {
  osi.glr->glExt.glVertex2hNV (x, y);}
inline GLAPI void APIENTRY glVertex2hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glVertex2hvNV (v);}
inline GLAPI void APIENTRY glVertex3hNV (GLhalfNV x, GLhalfNV y, GLhalfNV z) {
  osi.glr->glExt.glVertex3hNV (x, y, z);}
inline GLAPI void APIENTRY glVertex3hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glVertex3hvNV (v);}
inline GLAPI void APIENTRY glVertex4hNV (GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w) {
  osi.glr->glExt.glVertex4hNV (x, y, z, w);}
inline GLAPI void APIENTRY glVertex4hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glVertex4hvNV (v);}
inline GLAPI void APIENTRY glNormal3hNV (GLhalfNV nx, GLhalfNV ny, GLhalfNV nz) {
  osi.glr->glExt.glNormal3hNV (nx, ny, nz);}
inline GLAPI void APIENTRY glNormal3hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glNormal3hvNV (v);}
inline GLAPI void APIENTRY glColor3hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue) {
  osi.glr->glExt.glColor3hNV (red, green, blue);}
inline GLAPI void APIENTRY glColor3hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glColor3hvNV (v);}
inline GLAPI void APIENTRY glColor4hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha) {
  osi.glr->glExt.glColor4hNV (red, green, blue, alpha);}
inline GLAPI void APIENTRY glColor4hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glColor4hvNV (v);}
inline GLAPI void APIENTRY glTexCoord1hNV (GLhalfNV s) {
  osi.glr->glExt.glTexCoord1hNV (s);}
inline GLAPI void APIENTRY glTexCoord1hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glTexCoord1hvNV (v);}
inline GLAPI void APIENTRY glTexCoord2hNV (GLhalfNV s, GLhalfNV t) {
  osi.glr->glExt.glTexCoord2hNV (s, t);}
inline GLAPI void APIENTRY glTexCoord2hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glTexCoord2hvNV (v);}
inline GLAPI void APIENTRY glTexCoord3hNV (GLhalfNV s, GLhalfNV t, GLhalfNV r) {
  osi.glr->glExt.glTexCoord3hNV (s, t, r);}
inline GLAPI void APIENTRY glTexCoord3hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glTexCoord3hvNV (v);}
inline GLAPI void APIENTRY glTexCoord4hNV (GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q) {
  osi.glr->glExt.glTexCoord4hNV (s, t, r, q);}
inline GLAPI void APIENTRY glTexCoord4hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glTexCoord4hvNV (v);}
inline GLAPI void APIENTRY glMultiTexCoord1hNV (GLenum target, GLhalfNV s) {
  osi.glr->glExt.glMultiTexCoord1hNV (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1hvNV (GLenum target, const GLhalfNV *v) {
  osi.glr->glExt.glMultiTexCoord1hvNV (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2hNV (GLenum target, GLhalfNV s, GLhalfNV t) {
  osi.glr->glExt.glMultiTexCoord2hNV (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2hvNV (GLenum target, const GLhalfNV *v) {
  osi.glr->glExt.glMultiTexCoord2hvNV (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3hNV (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r) {
  osi.glr->glExt.glMultiTexCoord3hNV (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3hvNV (GLenum target, const GLhalfNV *v) {
  osi.glr->glExt.glMultiTexCoord3hvNV (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4hNV (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q) {
  osi.glr->glExt.glMultiTexCoord4hNV (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4hvNV (GLenum target, const GLhalfNV *v) {
  osi.glr->glExt.glMultiTexCoord4hvNV (target, v);}
inline GLAPI void APIENTRY glFogCoordhNV (GLhalfNV fog) {
  osi.glr->glExt.glFogCoordhNV (fog);}
inline GLAPI void APIENTRY glFogCoordhvNV (const GLhalfNV *fog) {
  osi.glr->glExt.glFogCoordhvNV (fog);}
inline GLAPI void APIENTRY glSecondaryColor3hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue) {
  osi.glr->glExt.glSecondaryColor3hNV (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3hvNV (const GLhalfNV *v) {
  osi.glr->glExt.glSecondaryColor3hvNV (v);}
inline GLAPI void APIENTRY glVertexWeighthNV (GLhalfNV weight) {
  osi.glr->glExt.glVertexWeighthNV (weight);}
inline GLAPI void APIENTRY glVertexWeighthvNV (const GLhalfNV *weight) {
  osi.glr->glExt.glVertexWeighthvNV (weight);}
inline GLAPI void APIENTRY glVertexAttrib1hNV (GLuint index, GLhalfNV x) {
  osi.glr->glExt.glVertexAttrib1hNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1hvNV (GLuint index, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttrib1hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2hNV (GLuint index, GLhalfNV x, GLhalfNV y) {
  osi.glr->glExt.glVertexAttrib2hNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2hvNV (GLuint index, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttrib2hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3hNV (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z) {
  osi.glr->glExt.glVertexAttrib3hNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3hvNV (GLuint index, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttrib3hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4hNV (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w) {
  osi.glr->glExt.glVertexAttrib4hNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4hvNV (GLuint index, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttrib4hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribs1hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttribs1hvNV (index, n, v);}
inline GLAPI void APIENTRY glVertexAttribs2hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttribs2hvNV (index, n, v);}
inline GLAPI void APIENTRY glVertexAttribs3hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttribs3hvNV (index, n, v);}
inline GLAPI void APIENTRY glVertexAttribs4hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  osi.glr->glExt.glVertexAttribs4hvNV (index, n, v);}
// GL_NV_occlusion_query
inline GLAPI void APIENTRY glGenOcclusionQueriesNV (GLsizei n, GLuint *ids) {
  osi.glr->glExt.glGenOcclusionQueriesNV (n, ids);}
inline GLAPI void APIENTRY glDeleteOcclusionQueriesNV (GLsizei n, const GLuint *ids) {
  osi.glr->glExt.glDeleteOcclusionQueriesNV (n, ids);}
inline GLAPI GLboolean APIENTRY glIsOcclusionQueryNV (GLuint id) {
  return osi.glr->glExt.glIsOcclusionQueryNV (id);}
inline GLAPI void APIENTRY glBeginOcclusionQueryNV (GLuint id) {
  osi.glr->glExt.glBeginOcclusionQueryNV (id);}
inline GLAPI void APIENTRY glEndOcclusionQueryNV (void) {
  osi.glr->glExt.glEndOcclusionQueryNV ();}
inline GLAPI void APIENTRY glGetOcclusionQueryivNV (GLuint id, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetOcclusionQueryivNV (id, pname, params);}
inline GLAPI void APIENTRY glGetOcclusionQueryuivNV (GLuint id, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetOcclusionQueryuivNV (id, pname, params);}
// GL_NV_parameter_buffer_object
inline GLAPI void APIENTRY glProgramBufferParametersfvNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLfloat *params) {
  osi.glr->glExt.glProgramBufferParametersfvNV (target, bindingIndex, wordIndex, count, params);}
inline GLAPI void APIENTRY glProgramBufferParametersIivNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLint *params) {
  osi.glr->glExt.glProgramBufferParametersIivNV (target, bindingIndex, wordIndex, count, params);}
inline GLAPI void APIENTRY glProgramBufferParametersIuivNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLuint *params) {
  osi.glr->glExt.glProgramBufferParametersIuivNV (target, bindingIndex, wordIndex, count, params);}
// GL_NV_path_rendering
inline GLAPI GLuint APIENTRY glGenPathsNV (GLsizei range) {
  return osi.glr->glExt.glGenPathsNV (range);}
inline GLAPI void APIENTRY glDeletePathsNV (GLuint path, GLsizei range) {
  osi.glr->glExt.glDeletePathsNV (path, range);}
inline GLAPI GLboolean APIENTRY glIsPathNV (GLuint path) {
  return osi.glr->glExt.glIsPathNV (path);}
inline GLAPI void APIENTRY glPathCommandsNV (GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords) {
  osi.glr->glExt.glPathCommandsNV (path, numCommands, commands, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathCoordsNV (GLuint path, GLsizei numCoords, GLenum coordType, const void *coords) {
  osi.glr->glExt.glPathCoordsNV (path, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathSubCommandsNV (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords) {
  osi.glr->glExt.glPathSubCommandsNV (path, commandStart, commandsToDelete, numCommands, commands, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathSubCoordsNV (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void *coords) {
  osi.glr->glExt.glPathSubCoordsNV (path, coordStart, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathStringNV (GLuint path, GLenum format, GLsizei length, const void *pathString) {
  osi.glr->glExt.glPathStringNV (path, format, length, pathString);}
inline GLAPI void APIENTRY glPathGlyphsNV (GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const void *charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale) {
  osi.glr->glExt.glPathGlyphsNV (firstPathName, fontTarget, fontName, fontStyle, numGlyphs, type, charcodes, handleMissingGlyphs, pathParameterTemplate, emScale);}
inline GLAPI void APIENTRY glPathGlyphRangeNV (GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale) {
  osi.glr->glExt.glPathGlyphRangeNV (firstPathName, fontTarget, fontName, fontStyle, firstGlyph, numGlyphs, handleMissingGlyphs, pathParameterTemplate, emScale);}
inline GLAPI void APIENTRY glWeightPathsNV (GLuint resultPath, GLsizei numPaths, const GLuint *paths, const GLfloat *weights) {
  osi.glr->glExt.glWeightPathsNV (resultPath, numPaths, paths, weights);}
inline GLAPI void APIENTRY glCopyPathNV (GLuint resultPath, GLuint srcPath) {
  osi.glr->glExt.glCopyPathNV (resultPath, srcPath);}
inline GLAPI void APIENTRY glInterpolatePathsNV (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight) {
  osi.glr->glExt.glInterpolatePathsNV (resultPath, pathA, pathB, weight);}
inline GLAPI void APIENTRY glTransformPathNV (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat *transformValues) {
  osi.glr->glExt.glTransformPathNV (resultPath, srcPath, transformType, transformValues);}
inline GLAPI void APIENTRY glPathParameterivNV (GLuint path, GLenum pname, const GLint *value) {
  osi.glr->glExt.glPathParameterivNV (path, pname, value);}
inline GLAPI void APIENTRY glPathParameteriNV (GLuint path, GLenum pname, GLint value) {
  osi.glr->glExt.glPathParameteriNV (path, pname, value);}
inline GLAPI void APIENTRY glPathParameterfvNV (GLuint path, GLenum pname, const GLfloat *value) {
  osi.glr->glExt.glPathParameterfvNV (path, pname, value);}
inline GLAPI void APIENTRY glPathParameterfNV (GLuint path, GLenum pname, GLfloat value) {
  osi.glr->glExt.glPathParameterfNV (path, pname, value);}
inline GLAPI void APIENTRY glPathDashArrayNV (GLuint path, GLsizei dashCount, const GLfloat *dashArray) {
  osi.glr->glExt.glPathDashArrayNV (path, dashCount, dashArray);}
inline GLAPI void APIENTRY glPathStencilFuncNV (GLenum func, GLint ref, GLuint mask) {
  osi.glr->glExt.glPathStencilFuncNV (func, ref, mask);}
inline GLAPI void APIENTRY glPathStencilDepthOffsetNV (GLfloat factor, GLfloat units) {
  osi.glr->glExt.glPathStencilDepthOffsetNV (factor, units);}
inline GLAPI void APIENTRY glStencilFillPathNV (GLuint path, GLenum fillMode, GLuint mask) {
  osi.glr->glExt.glStencilFillPathNV (path, fillMode, mask);}
inline GLAPI void APIENTRY glStencilStrokePathNV (GLuint path, GLint reference, GLuint mask) {
  osi.glr->glExt.glStencilStrokePathNV (path, reference, mask);}
inline GLAPI void APIENTRY glStencilFillPathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues) {
  osi.glr->glExt.glStencilFillPathInstancedNV (numPaths, pathNameType, paths, pathBase, fillMode, mask, transformType, transformValues);}
inline GLAPI void APIENTRY glStencilStrokePathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues) {
  osi.glr->glExt.glStencilStrokePathInstancedNV (numPaths, pathNameType, paths, pathBase, reference, mask, transformType, transformValues);}
inline GLAPI void APIENTRY glPathCoverDepthFuncNV (GLenum func) {
  osi.glr->glExt.glPathCoverDepthFuncNV (func);}
inline GLAPI void APIENTRY glPathColorGenNV (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs) {
  osi.glr->glExt.glPathColorGenNV (color, genMode, colorFormat, coeffs);}
inline GLAPI void APIENTRY glPathTexGenNV (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat *coeffs) {
  osi.glr->glExt.glPathTexGenNV (texCoordSet, genMode, components, coeffs);}
inline GLAPI void APIENTRY glPathFogGenNV (GLenum genMode) {
  osi.glr->glExt.glPathFogGenNV (genMode);}
inline GLAPI void APIENTRY glCoverFillPathNV (GLuint path, GLenum coverMode) {
  osi.glr->glExt.glCoverFillPathNV (path, coverMode);}
inline GLAPI void APIENTRY glCoverStrokePathNV (GLuint path, GLenum coverMode) {
  osi.glr->glExt.glCoverStrokePathNV (path, coverMode);}
inline GLAPI void APIENTRY glCoverFillPathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues) {
  osi.glr->glExt.glCoverFillPathInstancedNV (numPaths, pathNameType, paths, pathBase, coverMode, transformType, transformValues);}
inline GLAPI void APIENTRY glCoverStrokePathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues) {
  osi.glr->glExt.glCoverStrokePathInstancedNV (numPaths, pathNameType, paths, pathBase, coverMode, transformType, transformValues);}
inline GLAPI void APIENTRY glGetPathParameterivNV (GLuint path, GLenum pname, GLint *value) {
  osi.glr->glExt.glGetPathParameterivNV (path, pname, value);}
inline GLAPI void APIENTRY glGetPathParameterfvNV (GLuint path, GLenum pname, GLfloat *value) {
  osi.glr->glExt.glGetPathParameterfvNV (path, pname, value);}
inline GLAPI void APIENTRY glGetPathCommandsNV (GLuint path, GLubyte *commands) {
  osi.glr->glExt.glGetPathCommandsNV (path, commands);}
inline GLAPI void APIENTRY glGetPathCoordsNV (GLuint path, GLfloat *coords) {
  osi.glr->glExt.glGetPathCoordsNV (path, coords);}
inline GLAPI void APIENTRY glGetPathDashArrayNV (GLuint path, GLfloat *dashArray) {
  osi.glr->glExt.glGetPathDashArrayNV (path, dashArray);}
inline GLAPI void APIENTRY glGetPathMetricsNV (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics) {
  osi.glr->glExt.glGetPathMetricsNV (metricQueryMask, numPaths, pathNameType, paths, pathBase, stride, metrics);}
inline GLAPI void APIENTRY glGetPathMetricRangeNV (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics) {
  osi.glr->glExt.glGetPathMetricRangeNV (metricQueryMask, firstPathName, numPaths, stride, metrics);}
inline GLAPI void APIENTRY glGetPathSpacingNV (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing) {
  osi.glr->glExt.glGetPathSpacingNV (pathListMode, numPaths, pathNameType, paths, pathBase, advanceScale, kerningScale, transformType, returnedSpacing);}
inline GLAPI void APIENTRY glGetPathColorGenivNV (GLenum color, GLenum pname, GLint *value) {
  osi.glr->glExt.glGetPathColorGenivNV (color, pname, value);}
inline GLAPI void APIENTRY glGetPathColorGenfvNV (GLenum color, GLenum pname, GLfloat *value) {
  osi.glr->glExt.glGetPathColorGenfvNV (color, pname, value);}
inline GLAPI void APIENTRY glGetPathTexGenivNV (GLenum texCoordSet, GLenum pname, GLint *value) {
  osi.glr->glExt.glGetPathTexGenivNV (texCoordSet, pname, value);}
inline GLAPI void APIENTRY glGetPathTexGenfvNV (GLenum texCoordSet, GLenum pname, GLfloat *value) {
  osi.glr->glExt.glGetPathTexGenfvNV (texCoordSet, pname, value);}
inline GLAPI GLboolean APIENTRY glIsPointInFillPathNV (GLuint path, GLuint mask, GLfloat x, GLfloat y) {
  return osi.glr->glExt.glIsPointInFillPathNV (path, mask, x, y);}
inline GLAPI GLboolean APIENTRY glIsPointInStrokePathNV (GLuint path, GLfloat x, GLfloat y) {
  return osi.glr->glExt.glIsPointInStrokePathNV (path, x, y);}
inline GLAPI GLfloat APIENTRY glGetPathLengthNV (GLuint path, GLsizei startSegment, GLsizei numSegments) {
  return osi.glr->glExt.glGetPathLengthNV (path, startSegment, numSegments);}
inline GLAPI GLboolean APIENTRY glPointAlongPathNV (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat *x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY) {
  return osi.glr->glExt.glPointAlongPathNV (path, startSegment, numSegments, distance, x, y, tangentX, tangentY);}
// GL_NV_pixel_data_range
inline GLAPI void APIENTRY glPixelDataRangeNV (GLenum target, GLsizei length, const void *pointer) {
  osi.glr->glExt.glPixelDataRangeNV (target, length, pointer);}
inline GLAPI void APIENTRY glFlushPixelDataRangeNV (GLenum target) {
  osi.glr->glExt.glFlushPixelDataRangeNV (target);}
// GL_NV_point_sprite
inline GLAPI void APIENTRY glPointParameteriNV (GLenum pname, GLint param) {
  osi.glr->glExt.glPointParameteriNV (pname, param);}
inline GLAPI void APIENTRY glPointParameterivNV (GLenum pname, const GLint *params) {
  osi.glr->glExt.glPointParameterivNV (pname, params);}
// GL_NV_present_video GLX_NV_present_video WGL_NV_present_video
inline GLAPI void APIENTRY glPresentFrameKeyedNV (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1) {
  osi.glr->glExt.glPresentFrameKeyedNV (video_slot, minPresentTime, beginPresentTimeId, presentDurationId, type, target0, fill0, key0, target1, fill1, key1);}
inline GLAPI void APIENTRY glPresentFrameDualFillNV (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3) {
  osi.glr->glExt.glPresentFrameDualFillNV (video_slot, minPresentTime, beginPresentTimeId, presentDurationId, type, target0, fill0, target1, fill1, target2, fill2, target3, fill3);}
inline GLAPI void APIENTRY glGetVideoivNV (GLuint video_slot, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVideoivNV (video_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideouivNV (GLuint video_slot, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetVideouivNV (video_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideoi64vNV (GLuint video_slot, GLenum pname, GLint64EXT *params) {
  osi.glr->glExt.glGetVideoi64vNV (video_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideoui64vNV (GLuint video_slot, GLenum pname, GLuint64EXT *params) {
  osi.glr->glExt.glGetVideoui64vNV (video_slot, pname, params);}
#ifdef OS_WIN
inline BOOL WINAPI wglGetPixelFormatAttribivARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues) {
  return osi.glr->glExt.wglGetPixelFormatAttribivARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);}
inline BOOL WINAPI wglGetPixelFormatAttribfvARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues) {
  return osi.glr->glExt.wglGetPixelFormatAttribfvARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);}
inline BOOL WINAPI wglChoosePixelFormatARB (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats) {
  return osi.glr->glExt.wglChoosePixelFormatARB (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);}
#endif /// OS_WIN
#ifdef OS_LINUX
inline unsigned int *glXEnumerateVideoDevicesNV (Display *dpy, int screen, int *nelements) {
  return osi.glr->glExt.glXEnumerateVideoDevicesNV (dpy, screen, nelements);}
inline int glXBindVideoDeviceNV (Display *dpy, unsigned int video_slot, unsigned int video_device, const int *attrib_list) {
  return osi.glr->glExt.glXBindVideoDeviceNV (dpy, video_slot, video_device, attrib_list);}
#endif /// OS_LINUX






// GL_NV_primitive_restart
inline GLAPI void APIENTRY glPrimitiveRestartNV (void) {
  osi.glr->glExt.glPrimitiveRestartNV ();}
inline GLAPI void APIENTRY glPrimitiveRestartIndexNV (GLuint index) {
  osi.glr->glExt.glPrimitiveRestartIndexNV (index);}
// GL_NV_register_combiners
inline GLAPI void APIENTRY glCombinerParameterfvNV (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glCombinerParameterfvNV (pname, params);}
inline GLAPI void APIENTRY glCombinerParameterfNV (GLenum pname, GLfloat param) {
  osi.glr->glExt.glCombinerParameterfNV (pname, param);}
inline GLAPI void APIENTRY glCombinerParameterivNV (GLenum pname, const GLint *params) {
  osi.glr->glExt.glCombinerParameterivNV (pname, params);}
inline GLAPI void APIENTRY glCombinerParameteriNV (GLenum pname, GLint param) {
  osi.glr->glExt.glCombinerParameteriNV (pname, param);}
inline GLAPI void APIENTRY glCombinerInputNV (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage) {
  osi.glr->glExt.glCombinerInputNV (stage, portion, variable, input, mapping, componentUsage);}
inline GLAPI void APIENTRY glCombinerOutputNV (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum) {
  osi.glr->glExt.glCombinerOutputNV (stage, portion, abOutput, cdOutput, sumOutput, scale, bias, abDotProduct, cdDotProduct, muxSum);}
inline GLAPI void APIENTRY glFinalCombinerInputNV (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage) {
  osi.glr->glExt.glFinalCombinerInputNV (variable, input, mapping, componentUsage);}
inline GLAPI void APIENTRY glGetCombinerInputParameterfvNV (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetCombinerInputParameterfvNV (stage, portion, variable, pname, params);}
inline GLAPI void APIENTRY glGetCombinerInputParameterivNV (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetCombinerInputParameterivNV (stage, portion, variable, pname, params);}
inline GLAPI void APIENTRY glGetCombinerOutputParameterfvNV (GLenum stage, GLenum portion, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetCombinerOutputParameterfvNV (stage, portion, pname, params);}
inline GLAPI void APIENTRY glGetCombinerOutputParameterivNV (GLenum stage, GLenum portion, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetCombinerOutputParameterivNV (stage, portion, pname, params);}
inline GLAPI void APIENTRY glGetFinalCombinerInputParameterfvNV (GLenum variable, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetFinalCombinerInputParameterfvNV (variable, pname, params);}
inline GLAPI void APIENTRY glGetFinalCombinerInputParameterivNV (GLenum variable, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFinalCombinerInputParameterivNV (variable, pname, params);}
// GL_NV_register_combiners2
inline GLAPI void APIENTRY glCombinerStageParameterfvNV (GLenum stage, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glCombinerStageParameterfvNV (stage, pname, params);}
inline GLAPI void APIENTRY glGetCombinerStageParameterfvNV (GLenum stage, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetCombinerStageParameterfvNV (stage, pname, params);}
// GL_NV_shader_buffer_load
inline GLAPI void APIENTRY glMakeBufferResidentNV (GLenum target, GLenum access) {
  osi.glr->glExt.glMakeBufferResidentNV (target, access);}
inline GLAPI void APIENTRY glMakeBufferNonResidentNV (GLenum target) {
  osi.glr->glExt.glMakeBufferNonResidentNV (target);}
inline GLAPI GLboolean APIENTRY glIsBufferResidentNV (GLenum target) {
  return osi.glr->glExt.glIsBufferResidentNV (target);}
inline GLAPI void APIENTRY glMakeNamedBufferResidentNV (GLuint buffer, GLenum access) {
  osi.glr->glExt.glMakeNamedBufferResidentNV (buffer, access);}
inline GLAPI void APIENTRY glMakeNamedBufferNonResidentNV (GLuint buffer) {
  osi.glr->glExt.glMakeNamedBufferNonResidentNV (buffer);}
inline GLAPI GLboolean APIENTRY glIsNamedBufferResidentNV (GLuint buffer) {
  return osi.glr->glExt.glIsNamedBufferResidentNV (buffer);}
inline GLAPI void APIENTRY glGetBufferParameterui64vNV (GLenum target, GLenum pname, GLuint64EXT *params) {
  osi.glr->glExt.glGetBufferParameterui64vNV (target, pname, params);}
inline GLAPI void APIENTRY glGetNamedBufferParameterui64vNV (GLuint buffer, GLenum pname, GLuint64EXT *params) {
  osi.glr->glExt.glGetNamedBufferParameterui64vNV (buffer, pname, params);}
inline GLAPI void APIENTRY glGetIntegerui64vNV (GLenum value, GLuint64EXT *result) {
  osi.glr->glExt.glGetIntegerui64vNV (value, result);}
inline GLAPI void APIENTRY glUniformui64NV (GLint location, GLuint64EXT value) {
  osi.glr->glExt.glUniformui64NV (location, value);}
inline GLAPI void APIENTRY glUniformui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glUniformui64vNV (location, count, value);}
inline GLAPI void APIENTRY glProgramUniformui64NV (GLuint program, GLint location, GLuint64EXT value) {
  osi.glr->glExt.glProgramUniformui64NV (program, location, value);}
inline GLAPI void APIENTRY glProgramUniformui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  osi.glr->glExt.glProgramUniformui64vNV (program, location, count, value);}
// GL_NV_texture_barrier
inline GLAPI void APIENTRY glTextureBarrierNV (void) {
  osi.glr->glExt.glTextureBarrierNV ();}
// GL_NV_texture_multisample
inline GLAPI void APIENTRY glTexImage2DMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations) {
  osi.glr->glExt.glTexImage2DMultisampleCoverageNV (target, coverageSamples, colorSamples, internalFormat, width, height, fixedSampleLocations);}
inline GLAPI void APIENTRY glTexImage3DMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations) {
  osi.glr->glExt.glTexImage3DMultisampleCoverageNV (target, coverageSamples, colorSamples, internalFormat, width, height, depth, fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage2DMultisampleNV (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations) {
  osi.glr->glExt.glTextureImage2DMultisampleNV (texture, target, samples, internalFormat, width, height,  fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage3DMultisampleNV (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations) {
  osi.glr->glExt.glTextureImage3DMultisampleNV (texture, target, samples, internalFormat, width, height, depth, fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage2DMultisampleCoverageNV (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations) {
  osi.glr->glExt.glTextureImage2DMultisampleCoverageNV (texture, target, coverageSamples, colorSamples, internalFormat, width, height, fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage3DMultisampleCoverageNV (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations) {
  osi.glr->glExt.glTextureImage3DMultisampleCoverageNV (texture, target, coverageSamples, colorSamples, internalFormat, width, height, depth, fixedSampleLocations);}
// GL_NV_transform_feedback
inline GLAPI void APIENTRY glBeginTransformFeedbackNV (GLenum primitiveMode) {
  osi.glr->glExt.glBeginTransformFeedbackNV (primitiveMode);}
inline GLAPI void APIENTRY glEndTransformFeedbackNV (void) {
  osi.glr->glExt.glEndTransformFeedbackNV ();}
inline GLAPI void APIENTRY glTransformFeedbackAttribsNV (GLuint count, const GLint *attribs, GLenum bufferMode) {
  osi.glr->glExt.glTransformFeedbackAttribsNV (count, attribs, bufferMode);}
inline GLAPI void APIENTRY glBindBufferRangeNV (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  osi.glr->glExt.glBindBufferRangeNV (target, index, buffer, offset, size);}
inline GLAPI void APIENTRY glBindBufferOffsetNV (GLenum target, GLuint index, GLuint buffer, GLintptr offset) {
  osi.glr->glExt.glBindBufferOffsetNV (target, index, buffer, offset);}
inline GLAPI void APIENTRY glBindBufferBaseNV (GLenum target, GLuint index, GLuint buffer) {
  osi.glr->glExt.glBindBufferBaseNV (target, index, buffer);}
inline GLAPI void APIENTRY glTransformFeedbackVaryingsNV (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode) {
  osi.glr->glExt.glTransformFeedbackVaryingsNV (program, count, locations, bufferMode);}
inline GLAPI void APIENTRY glActiveVaryingNV (GLuint program, const GLchar *name) {
  osi.glr->glExt.glActiveVaryingNV (program, name);}
inline GLAPI GLint APIENTRY glGetVaryingLocationNV (GLuint program, const GLchar *name) {
  return osi.glr->glExt.glGetVaryingLocationNV (program, name);}
inline GLAPI void APIENTRY glGetActiveVaryingNV (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
  osi.glr->glExt.glGetActiveVaryingNV (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glGetTransformFeedbackVaryingNV (GLuint program, GLuint index, GLint *location) {
  osi.glr->glExt.glGetTransformFeedbackVaryingNV (program, index, location);}
inline GLAPI void APIENTRY glTransformFeedbackStreamAttribsNV (GLsizei count, const GLint *attribs, GLsizei nbuffers, const GLint *bufstreams, GLenum bufferMode) {
  osi.glr->glExt.glTransformFeedbackStreamAttribsNV (count, attribs, nbuffers, bufstreams, bufferMode);}
// GL_NV_transform_feedback2
inline GLAPI void APIENTRY glBindTransformFeedbackNV (GLenum target, GLuint id) {
  osi.glr->glExt.glBindTransformFeedbackNV (target, id);}
inline GLAPI void APIENTRY glDeleteTransformFeedbacksNV (GLsizei n, const GLuint *ids) {
  osi.glr->glExt.glDeleteTransformFeedbacksNV (n, ids);}
inline GLAPI void APIENTRY glGenTransformFeedbacksNV (GLsizei n, GLuint *ids) {
  osi.glr->glExt.glGenTransformFeedbacksNV (n, ids);}
inline GLAPI GLboolean APIENTRY glIsTransformFeedbackNV (GLuint id) {
  return osi.glr->glExt.glIsTransformFeedbackNV (id);}
inline GLAPI void APIENTRY glPauseTransformFeedbackNV (void) {
  osi.glr->glExt.glPauseTransformFeedbackNV ();}
inline GLAPI void APIENTRY glResumeTransformFeedbackNV (void) {
  osi.glr->glExt.glResumeTransformFeedbackNV ();}
inline GLAPI void APIENTRY glDrawTransformFeedbackNV (GLenum mode, GLuint id) {
  osi.glr->glExt.glDrawTransformFeedbackNV (mode, id);}
// GL_NV_vdpau_interop
inline GLAPI void APIENTRY glVDPAUInitNV (const void *vdpDevice, const void *getProcAddress) {
  osi.glr->glExt.glVDPAUInitNV (vdpDevice, getProcAddress);}
inline GLAPI void APIENTRY glVDPAUFiniNV (void) {
  osi.glr->glExt.glVDPAUFiniNV ();}
inline GLAPI GLvdpauSurfaceNV APIENTRY glVDPAURegisterVideoSurfaceNV (const void *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames) {
  return osi.glr->glExt.glVDPAURegisterVideoSurfaceNV (vdpSurface, target, numTextureNames, textureNames);}
inline GLAPI GLvdpauSurfaceNV APIENTRY glVDPAURegisterOutputSurfaceNV (const void *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames) {
  return osi.glr->glExt.glVDPAURegisterOutputSurfaceNV (vdpSurface, target, numTextureNames, textureNames);}
inline GLAPI GLboolean APIENTRY glVDPAUIsSurfaceNV (GLvdpauSurfaceNV surface) {
  return osi.glr->glExt.glVDPAUIsSurfaceNV (surface);}
inline GLAPI void APIENTRY glVDPAUUnregisterSurfaceNV (GLvdpauSurfaceNV surface) {
  osi.glr->glExt.glVDPAUUnregisterSurfaceNV (surface);}
inline GLAPI void APIENTRY glVDPAUGetSurfaceivNV (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) {
  osi.glr->glExt.glVDPAUGetSurfaceivNV (surface, pname, bufSize, length, values);}
inline GLAPI void APIENTRY glVDPAUSurfaceAccessNV (GLvdpauSurfaceNV surface, GLenum access) {
  osi.glr->glExt.glVDPAUSurfaceAccessNV (surface, access);}
inline GLAPI void APIENTRY glVDPAUMapSurfacesNV (GLsizei numSurfaces, const GLvdpauSurfaceNV *surfaces) {
  osi.glr->glExt.glVDPAUMapSurfacesNV (numSurfaces, surfaces);}
inline GLAPI void APIENTRY glVDPAUUnmapSurfacesNV (GLsizei numSurface, const GLvdpauSurfaceNV *surfaces) {
  osi.glr->glExt.glVDPAUUnmapSurfacesNV (numSurface, surfaces);}
// GL_NV_vertex_array_range
inline GLAPI void APIENTRY glFlushVertexArrayRangeNV (void) {
  osi.glr->glExt.glFlushVertexArrayRangeNV ();}
inline GLAPI void APIENTRY glVertexArrayRangeNV (GLsizei length, const void *pointer) {
  osi.glr->glExt.glVertexArrayRangeNV (length, pointer);}
// GL_NV_vertex_attrib_integer_64bit
inline GLAPI void APIENTRY glVertexAttribL1i64NV (GLuint index, GLint64EXT x) {
  osi.glr->glExt.glVertexAttribL1i64NV (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2i64NV (GLuint index, GLint64EXT x, GLint64EXT y) {
  osi.glr->glExt.glVertexAttribL2i64NV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3i64NV (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z) {
  osi.glr->glExt.glVertexAttribL3i64NV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4i64NV (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w) {
  osi.glr->glExt.glVertexAttribL4i64NV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1i64vNV (GLuint index, const GLint64EXT *v) {
  osi.glr->glExt.glVertexAttribL1i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2i64vNV (GLuint index, const GLint64EXT *v) {
  osi.glr->glExt.glVertexAttribL2i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3i64vNV (GLuint index, const GLint64EXT *v) {
  osi.glr->glExt.glVertexAttribL3i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4i64vNV (GLuint index, const GLint64EXT *v) {
  osi.glr->glExt.glVertexAttribL4i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL1ui64NV (GLuint index, GLuint64EXT x) {
  osi.glr->glExt.glVertexAttribL1ui64NV (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y) {
  osi.glr->glExt.glVertexAttribL2ui64NV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z) {
  osi.glr->glExt.glVertexAttribL3ui64NV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w) {
  osi.glr->glExt.glVertexAttribL4ui64NV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1ui64vNV (GLuint index, const GLuint64EXT *v) {
  osi.glr->glExt.glVertexAttribL1ui64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2ui64vNV (GLuint index, const GLuint64EXT *v) {
  osi.glr->glExt.glVertexAttribL2ui64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3ui64vNV (GLuint index, const GLuint64EXT *v) {
  osi.glr->glExt.glVertexAttribL3ui64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4ui64vNV (GLuint index, const GLuint64EXT *v) {
  osi.glr->glExt.glVertexAttribL4ui64vNV (index, v);}
inline GLAPI void APIENTRY glGetVertexAttribLi64vNV (GLuint index, GLenum pname, GLint64EXT *params) {
  osi.glr->glExt.glGetVertexAttribLi64vNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribLui64vNV (GLuint index, GLenum pname, GLuint64EXT *params) {
  osi.glr->glExt.glGetVertexAttribLui64vNV (index, pname, params);}
inline GLAPI void APIENTRY glVertexAttribLFormatNV (GLuint index, GLint size, GLenum type, GLsizei stride) {
  osi.glr->glExt.glVertexAttribLFormatNV (index, size, type, stride);}
// GL_NV_vertex_buffer_unified_memory
inline GLAPI void APIENTRY glBufferAddressRangeNV (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length) {
  osi.glr->glExt.glBufferAddressRangeNV (pname, index, address, length);}
inline GLAPI void APIENTRY glVertexFormatNV (GLint size, GLenum type, GLsizei stride) {
  osi.glr->glExt.glVertexFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glNormalFormatNV (GLenum type, GLsizei stride) {
  osi.glr->glExt.glNormalFormatNV (type, stride);}
inline GLAPI void APIENTRY glColorFormatNV (GLint size, GLenum type, GLsizei stride) {
  osi.glr->glExt.glColorFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glIndexFormatNV (GLenum type, GLsizei stride) {
  osi.glr->glExt.glIndexFormatNV (type, stride);}
inline GLAPI void APIENTRY glTexCoordFormatNV (GLint size, GLenum type, GLsizei stride) {
  osi.glr->glExt.glTexCoordFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glEdgeFlagFormatNV (GLsizei stride) {
  osi.glr->glExt.glEdgeFlagFormatNV (stride);}
inline GLAPI void APIENTRY glSecondaryColorFormatNV (GLint size, GLenum type, GLsizei stride) {
  osi.glr->glExt.glSecondaryColorFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glFogCoordFormatNV (GLenum type, GLsizei stride) {
  osi.glr->glExt.glFogCoordFormatNV (type, stride);}
inline GLAPI void APIENTRY glVertexAttribFormatNV (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride) {
  osi.glr->glExt.glVertexAttribFormatNV (index, size, type, normalized, stride);}
inline GLAPI void APIENTRY glVertexAttribIFormatNV (GLuint index, GLint size, GLenum type, GLsizei stride) {
  osi.glr->glExt.glVertexAttribIFormatNV (index, size, type, stride);}
inline GLAPI void APIENTRY glGetIntegerui64i_vNV (GLenum value, GLuint index, GLuint64EXT *result) {
  osi.glr->glExt.glGetIntegerui64i_vNV (value, index, result);}
// GL_NV_vertex_program
inline GLAPI GLboolean APIENTRY glAreProgramsResidentNV (GLsizei n, const GLuint *programs, GLboolean *residences) {
  return osi.glr->glExt.glAreProgramsResidentNV (n, programs, residences);}
inline GLAPI void APIENTRY glBindProgramNV (GLenum target, GLuint id) {
  osi.glr->glExt.glBindProgramNV (target, id);}
inline GLAPI void APIENTRY glDeleteProgramsNV (GLsizei n, const GLuint *programs) {
  osi.glr->glExt.glDeleteProgramsNV (n, programs);}
inline GLAPI void APIENTRY glExecuteProgramNV (GLenum target, GLuint id, const GLfloat *params) {
  osi.glr->glExt.glExecuteProgramNV (target, id, params);}
inline GLAPI void APIENTRY glGenProgramsNV (GLsizei n, GLuint *programs) {
  osi.glr->glExt.glGenProgramsNV (n, programs);}
inline GLAPI void APIENTRY glGetProgramParameterdvNV (GLenum target, GLuint index, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetProgramParameterdvNV (target, index, pname, params);}
inline GLAPI void APIENTRY glGetProgramParameterfvNV (GLenum target, GLuint index, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetProgramParameterfvNV (target, index, pname, params);}
inline GLAPI void APIENTRY glGetProgramivNV (GLuint id, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetProgramivNV (id, pname, params);}
inline GLAPI void APIENTRY glGetProgramStringNV (GLuint id, GLenum pname, GLubyte *program) {
  osi.glr->glExt.glGetProgramStringNV (id, pname, program);}
inline GLAPI void APIENTRY glGetTrackMatrixivNV (GLenum target, GLuint address, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetTrackMatrixivNV (target, address, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribdvNV (GLuint index, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetVertexAttribdvNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribfvNV (GLuint index, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetVertexAttribfvNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribivNV (GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVertexAttribivNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribPointervNV (GLuint index, GLenum pname, void **pointer) {
  osi.glr->glExt.glGetVertexAttribPointervNV (index, pname, pointer);}
inline GLAPI GLboolean APIENTRY glIsProgramNV (GLuint id) {
  return osi.glr->glExt.glIsProgramNV (id);}
inline GLAPI void APIENTRY glLoadProgramNV (GLenum target, GLuint id, GLsizei len, const GLubyte *program) {
  osi.glr->glExt.glLoadProgramNV (target, id, len, program);}
inline GLAPI void APIENTRY glProgramParameter4dNV (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glProgramParameter4dNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramParameter4dvNV (GLenum target, GLuint index, const GLdouble *v) {
  osi.glr->glExt.glProgramParameter4dvNV (target, index, v);}
inline GLAPI void APIENTRY glProgramParameter4fNV (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glProgramParameter4fNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramParameter4fvNV (GLenum target, GLuint index, const GLfloat *v) {
  osi.glr->glExt.glProgramParameter4fvNV (target, index, v);}
inline GLAPI void APIENTRY glProgramParameters4dvNV (GLenum target, GLuint index, GLsizei count, const GLdouble *v) {
  osi.glr->glExt.glProgramParameters4dvNV (target, index, count, v);}
inline GLAPI void APIENTRY glProgramParameters4fvNV (GLenum target, GLuint index, GLsizei count, const GLfloat *v) {
  osi.glr->glExt.glProgramParameters4fvNV (target, index, count, v);}
inline GLAPI void APIENTRY glRequestResidentProgramsNV (GLsizei n, const GLuint *programs) {
  osi.glr->glExt.glRequestResidentProgramsNV (n, programs);}
inline GLAPI void APIENTRY glTrackMatrixNV (GLenum target, GLuint address, GLenum matrix, GLenum transform) {
  osi.glr->glExt.glTrackMatrixNV (target, address, matrix, transform);}
inline GLAPI void APIENTRY glVertexAttribPointerNV (GLuint index, GLint fsize, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexAttribPointerNV (index, fsize, type, stride, pointer);}
inline GLAPI void APIENTRY glVertexAttrib1dNV (GLuint index, GLdouble x) {
  osi.glr->glExt.glVertexAttrib1dNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1dvNV (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib1dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1fNV (GLuint index, GLfloat x) {
  osi.glr->glExt.glVertexAttrib1fNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1fvNV (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib1fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1sNV (GLuint index, GLshort x) {
  osi.glr->glExt.glVertexAttrib1sNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1svNV (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib1svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2dNV (GLuint index, GLdouble x, GLdouble y) {
  osi.glr->glExt.glVertexAttrib2dNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2dvNV (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib2dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2fNV (GLuint index, GLfloat x, GLfloat y) {
  osi.glr->glExt.glVertexAttrib2fNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2fvNV (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib2fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2sNV (GLuint index, GLshort x, GLshort y) {
  osi.glr->glExt.glVertexAttrib2sNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2svNV (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib2svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3dNV (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  osi.glr->glExt.glVertexAttrib3dNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3dvNV (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib3dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3fNV (GLuint index, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glVertexAttrib3fNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3fvNV (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib3fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3sNV (GLuint index, GLshort x, GLshort y, GLshort z) {
  osi.glr->glExt.glVertexAttrib3sNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3svNV (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib3svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4dNV (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  osi.glr->glExt.glVertexAttrib4dNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4dvNV (GLuint index, const GLdouble *v) {
  osi.glr->glExt.glVertexAttrib4dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4fNV (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glVertexAttrib4fNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4fvNV (GLuint index, const GLfloat *v) {
  osi.glr->glExt.glVertexAttrib4fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4sNV (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
  osi.glr->glExt.glVertexAttrib4sNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4svNV (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttrib4svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ubNV (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
  osi.glr->glExt.glVertexAttrib4ubNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4ubvNV (GLuint index, const GLubyte *v) {
  osi.glr->glExt.glVertexAttrib4ubvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribs1dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribs1dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs1fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  osi.glr->glExt.glVertexAttribs1fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs1svNV (GLuint index, GLsizei count, const GLshort *v) {
  osi.glr->glExt.glVertexAttribs1svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs2dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribs2dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs2fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  osi.glr->glExt.glVertexAttribs2fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs2svNV (GLuint index, GLsizei count, const GLshort *v) {
  osi.glr->glExt.glVertexAttribs2svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs3dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribs3dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs3fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  osi.glr->glExt.glVertexAttribs3fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs3svNV (GLuint index, GLsizei count, const GLshort *v) {
  osi.glr->glExt.glVertexAttribs3svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  osi.glr->glExt.glVertexAttribs4dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  osi.glr->glExt.glVertexAttribs4fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4svNV (GLuint index, GLsizei count, const GLshort *v) {
  osi.glr->glExt.glVertexAttribs4svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4ubvNV (GLuint index, GLsizei count, const GLubyte *v) {
  osi.glr->glExt.glVertexAttribs4ubvNV (index, count, v);}
// GL_NV_vertex_program4
inline GLAPI void APIENTRY glVertexAttribI1iEXT (GLuint index, GLint x) {
  osi.glr->glExt.glVertexAttribI1iEXT (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2iEXT (GLuint index, GLint x, GLint y) {
  osi.glr->glExt.glVertexAttribI2iEXT (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3iEXT (GLuint index, GLint x, GLint y, GLint z) {
  osi.glr->glExt.glVertexAttribI3iEXT (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4iEXT (GLuint index, GLint x, GLint y, GLint z, GLint w) {
  osi.glr->glExt.glVertexAttribI4iEXT (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1uiEXT (GLuint index, GLuint x) {
  osi.glr->glExt.glVertexAttribI1uiEXT (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2uiEXT (GLuint index, GLuint x, GLuint y) {
  osi.glr->glExt.glVertexAttribI2uiEXT (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3uiEXT (GLuint index, GLuint x, GLuint y, GLuint z) {
  osi.glr->glExt.glVertexAttribI3uiEXT (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4uiEXT (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  osi.glr->glExt.glVertexAttribI4uiEXT (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1ivEXT (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI1ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2ivEXT (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI2ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3ivEXT (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI3ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4ivEXT (GLuint index, const GLint *v) {
  osi.glr->glExt.glVertexAttribI4ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI1uivEXT (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI1uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2uivEXT (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI2uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3uivEXT (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI3uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4uivEXT (GLuint index, const GLuint *v) {
  osi.glr->glExt.glVertexAttribI4uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4bvEXT (GLuint index, const GLbyte *v) {
  osi.glr->glExt.glVertexAttribI4bvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4svEXT (GLuint index, const GLshort *v) {
  osi.glr->glExt.glVertexAttribI4svEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4ubvEXT (GLuint index, const GLubyte *v) {
  osi.glr->glExt.glVertexAttribI4ubvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4usvEXT (GLuint index, const GLushort *v) {
  osi.glr->glExt.glVertexAttribI4usvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribIPointerEXT (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  osi.glr->glExt.glVertexAttribIPointerEXT (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribIivEXT (GLuint index, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVertexAttribIivEXT (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribIuivEXT (GLuint index, GLenum pname, GLuint *params) {
  osi.glr->glExt.glGetVertexAttribIuivEXT (index, pname, params);}
// GL_NV_video_capture
inline GLAPI void APIENTRY glBeginVideoCaptureNV (GLuint video_capture_slot) {
  osi.glr->glExt.glBeginVideoCaptureNV (video_capture_slot);}
inline GLAPI void APIENTRY glBindVideoCaptureStreamBufferNV (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset) {
  osi.glr->glExt.glBindVideoCaptureStreamBufferNV (video_capture_slot, stream, frame_region, offset);}
inline GLAPI void APIENTRY glBindVideoCaptureStreamTextureNV (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture) {
  osi.glr->glExt.glBindVideoCaptureStreamTextureNV (video_capture_slot, stream, frame_region, target, texture);}
inline GLAPI void APIENTRY glEndVideoCaptureNV (GLuint video_capture_slot) {
  osi.glr->glExt.glEndVideoCaptureNV (video_capture_slot);}
inline GLAPI void APIENTRY glGetVideoCaptureivNV (GLuint video_capture_slot, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVideoCaptureivNV (video_capture_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideoCaptureStreamivNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetVideoCaptureStreamivNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glGetVideoCaptureStreamfvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetVideoCaptureStreamfvNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glGetVideoCaptureStreamdvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble *params) {
  osi.glr->glExt.glGetVideoCaptureStreamdvNV (video_capture_slot, stream, pname, params);}
inline GLAPI GLenum APIENTRY glVideoCaptureNV (GLuint video_capture_slot, GLuint *sequence_num, GLuint64EXT *capture_time) {
  return osi.glr->glExt.glVideoCaptureNV (video_capture_slot, sequence_num, capture_time);}
inline GLAPI void APIENTRY glVideoCaptureStreamParameterivNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint *params) {
  osi.glr->glExt.glVideoCaptureStreamParameterivNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glVideoCaptureStreamParameterfvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glVideoCaptureStreamParameterfvNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glVideoCaptureStreamParameterdvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble *params) {
  osi.glr->glExt.glVideoCaptureStreamParameterdvNV (video_capture_slot, stream, pname, params);}
#ifdef OS_WIN
/// WGL_NV_video_capture
inline BOOL WINAPI wglBindVideoCaptureDeviceNV (UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice) {
  return osi.glr->glExt.wglBindVideoCaptureDeviceNV (uVideoSlot, hDevice);}
inline UINT WINAPI wglEnumerateVideoCaptureDevicesNV (HDC hDc, HVIDEOINPUTDEVICENV *phDeviceList) {
  return osi.glr->glExt.wglEnumerateVideoCaptureDevicesNV (hDc, phDeviceList);}
inline BOOL WINAPI wglLockVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
  return osi.glr->glExt.wglLockVideoCaptureDeviceNV (hDc, hDevice);}
inline BOOL WINAPI wglQueryVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int *piValue) {
  return osi.glr->glExt.wglQueryVideoCaptureDeviceNV (hDc, hDevice, iAttribute, piValue);}
inline BOOL WINAPI wglReleaseVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
  return osi.glr->glExt.wglReleaseVideoCaptureDeviceNV (hDc, hDevice);}
#endif
// GL_PGI_misc_hints
inline GLAPI void APIENTRY glHintPGI (GLenum target, GLint mode) {
  osi.glr->glExt.glHintPGI (target, mode);}
// GL_SGIS_detail_texture
inline GLAPI void APIENTRY glDetailTexFuncSGIS (GLenum target, GLsizei n, const GLfloat *points) {
  osi.glr->glExt.glDetailTexFuncSGIS (target, n, points);}
inline GLAPI void APIENTRY glGetDetailTexFuncSGIS (GLenum target, GLfloat *points) {
  osi.glr->glExt.glGetDetailTexFuncSGIS (target, points);}
// GL_SGIS_fog_function
inline GLAPI void APIENTRY glFogFuncSGIS (GLsizei n, const GLfloat *points) {
  osi.glr->glExt.glFogFuncSGIS (n, points);}
inline GLAPI void APIENTRY glGetFogFuncSGIS (GLfloat *points) {
  osi.glr->glExt.glGetFogFuncSGIS (points);}
// GL_SGIS_multisample
inline GLAPI void APIENTRY glSampleMaskSGIS (GLclampf value, GLboolean invert) {
  osi.glr->glExt.glSampleMaskSGIS (value, invert);}
inline GLAPI void APIENTRY glSamplePatternSGIS (GLenum pattern) {
  osi.glr->glExt.glSamplePatternSGIS (pattern);}
// GL_SGIS_pixel_texture
inline GLAPI void APIENTRY glPixelTexGenParameteriSGIS (GLenum pname, GLint param) {
  osi.glr->glExt.glPixelTexGenParameteriSGIS (pname, param);}
inline GLAPI void APIENTRY glPixelTexGenParameterivSGIS (GLenum pname, const GLint *params) {
  osi.glr->glExt.glPixelTexGenParameterivSGIS (pname, params);}
inline GLAPI void APIENTRY glPixelTexGenParameterfSGIS (GLenum pname, GLfloat param) {
  osi.glr->glExt.glPixelTexGenParameterfSGIS (pname, param);}
inline GLAPI void APIENTRY glPixelTexGenParameterfvSGIS (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glPixelTexGenParameterfvSGIS (pname, params);}
inline GLAPI void APIENTRY glGetPixelTexGenParameterivSGIS (GLenum pname, GLint *params) {
  osi.glr->glExt.glGetPixelTexGenParameterivSGIS (pname, params);}
inline GLAPI void APIENTRY glGetPixelTexGenParameterfvSGIS (GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetPixelTexGenParameterfvSGIS (pname, params);}
// GL_SGIS_point_parameters
inline GLAPI void APIENTRY glPointParameterfSGIS (GLenum pname, GLfloat param) {
  osi.glr->glExt.glPointParameterfSGIS (pname, param);}
inline GLAPI void APIENTRY glPointParameterfvSGIS (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glPointParameterfvSGIS (pname, params);}
// GL_SGIS_sharpen_texture
inline GLAPI void APIENTRY glSharpenTexFuncSGIS (GLenum target, GLsizei n, const GLfloat *points) {
  osi.glr->glExt.glSharpenTexFuncSGIS (target, n, points);}
inline GLAPI void APIENTRY glGetSharpenTexFuncSGIS (GLenum target, GLfloat *points) {
  osi.glr->glExt.glGetSharpenTexFuncSGIS (target, points);}
// GL_SGIS_texture4D
inline GLAPI void APIENTRY glTexImage4DSGIS (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexImage4DSGIS (target, level, internalformat, width, height, depth, size4d, border, format, type, pixels);}
inline GLAPI void APIENTRY glTexSubImage4DSGIS (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const void *pixels) {
  osi.glr->glExt.glTexSubImage4DSGIS (target, level, xoffset, yoffset, zoffset, woffset, width, height, depth, size4d, format, type, pixels);}
// GL_SGIS_texture_color_mask
inline GLAPI void APIENTRY glTextureColorMaskSGIS (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
  osi.glr->glExt.glTextureColorMaskSGIS (red, green, blue, alpha);}
// GL_SGIS_texture_filter4
inline GLAPI void APIENTRY glGetTexFilterFuncSGIS (GLenum target, GLenum filter, GLfloat *weights) {
  osi.glr->glExt.glGetTexFilterFuncSGIS (target, filter, weights);}
inline GLAPI void APIENTRY glTexFilterFuncSGIS (GLenum target, GLenum filter, GLsizei n, const GLfloat *weights) {
  osi.glr->glExt.glTexFilterFuncSGIS (target, filter, n, weights);}
// GL_SGIX_async
inline GLAPI void APIENTRY glAsyncMarkerSGIX (GLuint marker) {
  osi.glr->glExt.glAsyncMarkerSGIX (marker);}
inline GLAPI GLint APIENTRY glFinishAsyncSGIX (GLuint *markerp) {
  return osi.glr->glExt.glFinishAsyncSGIX (markerp);}
inline GLAPI GLint APIENTRY glPollAsyncSGIX (GLuint *markerp) {
  return osi.glr->glExt.glPollAsyncSGIX (markerp);}
inline GLAPI GLuint APIENTRY glGenAsyncMarkersSGIX (GLsizei range) {
  return osi.glr->glExt.glGenAsyncMarkersSGIX (range);}
inline GLAPI void APIENTRY glDeleteAsyncMarkersSGIX (GLuint marker, GLsizei range) {
  osi.glr->glExt.glDeleteAsyncMarkersSGIX (marker, range);}
inline GLAPI GLboolean APIENTRY glIsAsyncMarkerSGIX (GLuint marker) {
  return osi.glr->glExt.glIsAsyncMarkerSGIX (marker);}
// GL_SGIX_flush_raster
inline GLAPI void APIENTRY glFlushRasterSGIX (void) {
  osi.glr->glExt.glFlushRasterSGIX ();}
// GL_SGIX_fragment_lighting
inline GLAPI void APIENTRY glFragmentLightfSGIX (GLenum light, GLenum pname, GLfloat param) {
  osi.glr->glExt.glFragmentLightfSGIX (light, pname, param);}
inline GLAPI void APIENTRY glFragmentLightfvSGIX (GLenum light, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glFragmentLightfvSGIX (light, pname, params);}
inline GLAPI void APIENTRY glFragmentLightiSGIX (GLenum light, GLenum pname, GLint param) {
  osi.glr->glExt.glFragmentLightiSGIX (light, pname, param);}
inline GLAPI void APIENTRY glFragmentLightivSGIX (GLenum light, GLenum pname, const GLint *params) {
  osi.glr->glExt.glFragmentLightivSGIX (light, pname, params);}
inline GLAPI void APIENTRY glFragmentLightModelfSGIX (GLenum pname, GLfloat param) {
  osi.glr->glExt.glFragmentLightModelfSGIX (pname, param);}
inline GLAPI void APIENTRY glFragmentLightModelfvSGIX (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glFragmentLightModelfvSGIX (pname, params);}
inline GLAPI void APIENTRY glFragmentLightModeliSGIX (GLenum pname, GLint param) {
  osi.glr->glExt.glFragmentLightModeliSGIX (pname, param);}
inline GLAPI void APIENTRY glFragmentLightModelivSGIX (GLenum pname, const GLint *params) {
  osi.glr->glExt.glFragmentLightModelivSGIX (pname, params);}
inline GLAPI void APIENTRY glFragmentMaterialfSGIX (GLenum face, GLenum pname, GLfloat param) {
  osi.glr->glExt.glFragmentMaterialfSGIX (face, pname, param);}
inline GLAPI void APIENTRY glFragmentMaterialfvSGIX (GLenum face, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glFragmentMaterialfvSGIX (face, pname, params);}
inline GLAPI void APIENTRY glFragmentMaterialiSGIX (GLenum face, GLenum pname, GLint param) {
  osi.glr->glExt.glFragmentMaterialiSGIX (face, pname, param);}
inline GLAPI void APIENTRY glFragmentMaterialivSGIX (GLenum face, GLenum pname, const GLint *params) {
  osi.glr->glExt.glFragmentMaterialivSGIX (face, pname, params);}
inline GLAPI void APIENTRY glGetFragmentLightfvSGIX (GLenum light, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetFragmentLightfvSGIX (light, pname, params);}
inline GLAPI void APIENTRY glGetFragmentLightivSGIX (GLenum light, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFragmentLightivSGIX (light, pname, params);}
inline GLAPI void APIENTRY glGetFragmentMaterialfvSGIX (GLenum face, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetFragmentMaterialfvSGIX (face, pname, params);}
inline GLAPI void APIENTRY glGetFragmentMaterialivSGIX (GLenum face, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetFragmentMaterialivSGIX (face, pname, params);}
inline GLAPI void APIENTRY glLightEnviSGIX (GLenum pname, GLint param) {
  osi.glr->glExt.glLightEnviSGIX (pname, param);}
// GL_SGIX_framezoom
inline GLAPI void APIENTRY glFrameZoomSGIX (GLint factor) {
  osi.glr->glExt.glFrameZoomSGIX (factor);}
// GL_SGIX_igloo_interface
inline GLAPI void APIENTRY glIglooInterfaceSGIX (GLenum pname, const void *params) {
  osi.glr->glExt.glIglooInterfaceSGIX (pname, params);}
// GL_SGIX_instruments
inline GLAPI GLint APIENTRY glGetInstrumentsSGIX (void) {
  return osi.glr->glExt.glGetInstrumentsSGIX ();}
inline GLAPI void APIENTRY glInstrumentsBufferSGIX (GLsizei size, GLint *buffer) {
  osi.glr->glExt.glInstrumentsBufferSGIX (size, buffer);}
inline GLAPI GLint APIENTRY glPollInstrumentsSGIX (GLint *marker_p) {
  return osi.glr->glExt.glPollInstrumentsSGIX (marker_p);}
inline GLAPI void APIENTRY glReadInstrumentsSGIX (GLint marker) {
  osi.glr->glExt.glReadInstrumentsSGIX (marker);}
inline GLAPI void APIENTRY glStartInstrumentsSGIX (void) {
  osi.glr->glExt.glStartInstrumentsSGIX ();}
inline GLAPI void APIENTRY glStopInstrumentsSGIX (GLint marker) {
  osi.glr->glExt.glStopInstrumentsSGIX (marker);}
// GL_SGIX_list_priority
inline GLAPI void APIENTRY glGetListParameterfvSGIX (GLuint list, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetListParameterfvSGIX (list, pname, params);}
inline GLAPI void APIENTRY glGetListParameterivSGIX (GLuint list, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetListParameterivSGIX (list, pname, params);}
inline GLAPI void APIENTRY glListParameterfSGIX (GLuint list, GLenum pname, GLfloat param) {
  osi.glr->glExt.glListParameterfSGIX (list, pname, param);}
inline GLAPI void APIENTRY glListParameterfvSGIX (GLuint list, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glListParameterfvSGIX (list, pname, params);}
inline GLAPI void APIENTRY glListParameteriSGIX (GLuint list, GLenum pname, GLint param) {
  osi.glr->glExt.glListParameteriSGIX (list, pname, param);}
inline GLAPI void APIENTRY glListParameterivSGIX (GLuint list, GLenum pname, const GLint *params) {
  osi.glr->glExt.glListParameterivSGIX (list, pname, params);}
// GL_SGIX_pixel_texture
inline GLAPI void APIENTRY glPixelTexGenSGIX (GLenum mode) {
  osi.glr->glExt.glPixelTexGenSGIX (mode);}
// GL_SGIX_polynomial_ffd
inline GLAPI void APIENTRY glDeformationMap3dSGIX (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points) {
  osi.glr->glExt.glDeformationMap3dSGIX (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, w1, w2, wstride, worder, points);}
inline GLAPI void APIENTRY glDeformationMap3fSGIX (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points) {
  osi.glr->glExt.glDeformationMap3fSGIX (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, w1, w2, wstride, worder, points);}
inline GLAPI void APIENTRY glDeformSGIX (GLbitfield mask) {
  osi.glr->glExt.glDeformSGIX (mask);}
inline GLAPI void APIENTRY glLoadIdentityDeformationMapSGIX (GLbitfield mask) {
  osi.glr->glExt.glLoadIdentityDeformationMapSGIX (mask);}
// GL_SGIX_reference_plane
inline GLAPI void APIENTRY glReferencePlaneSGIX (const GLdouble *equation) {
  osi.glr->glExt.glReferencePlaneSGIX (equation);}
// GL_SGIX_sprite
inline GLAPI void APIENTRY glSpriteParameterfSGIX (GLenum pname, GLfloat param) {
  osi.glr->glExt.glSpriteParameterfSGIX (pname, param);}
inline GLAPI void APIENTRY glSpriteParameterfvSGIX (GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glSpriteParameterfvSGIX (pname, params);}
inline GLAPI void APIENTRY glSpriteParameteriSGIX (GLenum pname, GLint param) {
  osi.glr->glExt.glSpriteParameteriSGIX (pname, param);}
inline GLAPI void APIENTRY glSpriteParameterivSGIX (GLenum pname, const GLint *params) {
  osi.glr->glExt.glSpriteParameterivSGIX (pname, params);}
// GL_SGIX_tag_sample_buffer
inline GLAPI void APIENTRY glTagSampleBufferSGIX (void) {
  osi.glr->glExt.glTagSampleBufferSGIX ();}
// GL_SGI_color_table
inline GLAPI void APIENTRY glColorTableSGI (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *table) {
  osi.glr->glExt.glColorTableSGI (target, internalformat, width, format, type, table);}
inline GLAPI void APIENTRY glColorTableParameterfvSGI (GLenum target, GLenum pname, const GLfloat *params) {
  osi.glr->glExt.glColorTableParameterfvSGI (target, pname, params);}
inline GLAPI void APIENTRY glColorTableParameterivSGI (GLenum target, GLenum pname, const GLint *params) {
  osi.glr->glExt.glColorTableParameterivSGI (target, pname, params);}
inline GLAPI void APIENTRY glCopyColorTableSGI (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  osi.glr->glExt.glCopyColorTableSGI (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glGetColorTableSGI (GLenum target, GLenum format, GLenum type, void *table) {
  osi.glr->glExt.glGetColorTableSGI (target, format, type, table);}
inline GLAPI void APIENTRY glGetColorTableParameterfvSGI (GLenum target, GLenum pname, GLfloat *params) {
  osi.glr->glExt.glGetColorTableParameterfvSGI (target, pname, params);}
inline GLAPI void APIENTRY glGetColorTableParameterivSGI (GLenum target, GLenum pname, GLint *params) {
  osi.glr->glExt.glGetColorTableParameterivSGI (target, pname, params);}
// GL_SUNX_constant_data
inline GLAPI void APIENTRY glFinishTextureSUNX (void) {
  osi.glr->glExt.glFinishTextureSUNX ();}
// GL_SUN_global_alpha
inline GLAPI void APIENTRY glGlobalAlphaFactorbSUN (GLbyte factor) {
  osi.glr->glExt.glGlobalAlphaFactorbSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorsSUN (GLshort factor) {
  osi.glr->glExt.glGlobalAlphaFactorsSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactoriSUN (GLint factor) {
  osi.glr->glExt.glGlobalAlphaFactoriSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorfSUN (GLfloat factor) {
  osi.glr->glExt.glGlobalAlphaFactorfSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactordSUN (GLdouble factor) {
  osi.glr->glExt.glGlobalAlphaFactordSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorubSUN (GLubyte factor) {
  osi.glr->glExt.glGlobalAlphaFactorubSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorusSUN (GLushort factor) {
  osi.glr->glExt.glGlobalAlphaFactorusSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactoruiSUN (GLuint factor) {
  osi.glr->glExt.glGlobalAlphaFactoruiSUN (factor);}
// GL_SUN_mesh_array
inline GLAPI void APIENTRY glDrawMeshArraysSUN (GLenum mode, GLint first, GLsizei count, GLsizei width) {
  osi.glr->glExt.glDrawMeshArraysSUN (mode, first, count, width);}
// GL_SUN_triangle_list
inline GLAPI void APIENTRY glReplacementCodeuiSUN (GLuint code) {
  osi.glr->glExt.glReplacementCodeuiSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeusSUN (GLushort code) {
  osi.glr->glExt.glReplacementCodeusSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeubSUN (GLubyte code) {
  osi.glr->glExt.glReplacementCodeubSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeuivSUN (const GLuint *code) {
  osi.glr->glExt.glReplacementCodeuivSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeusvSUN (const GLushort *code) {
  osi.glr->glExt.glReplacementCodeusvSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeubvSUN (const GLubyte *code) {
  osi.glr->glExt.glReplacementCodeubvSUN (code);}
inline GLAPI void APIENTRY glReplacementCodePointerSUN (GLenum type, GLsizei stride, const void **pointer) {
  osi.glr->glExt.glReplacementCodePointerSUN (type, stride, pointer);}
// GL_SUN_vertex
inline GLAPI void APIENTRY glColor4ubVertex2fSUN (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y) {
  osi.glr->glExt.glColor4ubVertex2fSUN (r, g, b, a, x, y);}
inline GLAPI void APIENTRY glColor4ubVertex2fvSUN (const GLubyte *c, const GLfloat *v) {
  osi.glr->glExt.glColor4ubVertex2fvSUN (c, v);}
inline GLAPI void APIENTRY glColor4ubVertex3fSUN (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glColor4ubVertex3fSUN (r, g, b, a, x, y, z);}
inline GLAPI void APIENTRY glColor4ubVertex3fvSUN (const GLubyte *c, const GLfloat *v) {
  osi.glr->glExt.glColor4ubVertex3fvSUN (c, v);}
inline GLAPI void APIENTRY glColor3fVertex3fSUN (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glColor3fVertex3fSUN (r, g, b, x, y, z);}
inline GLAPI void APIENTRY glColor3fVertex3fvSUN (const GLfloat *c, const GLfloat *v) {
  osi.glr->glExt.glColor3fVertex3fvSUN (c, v);}
inline GLAPI void APIENTRY glNormal3fVertex3fSUN (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glNormal3fVertex3fSUN (nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glNormal3fVertex3fvSUN (const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glNormal3fVertex3fvSUN (n, v);}
inline GLAPI void APIENTRY glColor4fNormal3fVertex3fSUN (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glColor4fNormal3fVertex3fSUN (r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glColor4fNormal3fVertex3fvSUN (const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glColor4fNormal3fVertex3fvSUN (c, n, v);}
inline GLAPI void APIENTRY glTexCoord2fVertex3fSUN (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glTexCoord2fVertex3fSUN (s, t, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fVertex3fvSUN (const GLfloat *tc, const GLfloat *v) {
  osi.glr->glExt.glTexCoord2fVertex3fvSUN (tc, v);}
inline GLAPI void APIENTRY glTexCoord4fVertex4fSUN (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glTexCoord4fVertex4fSUN (s, t, p, q, x, y, z, w);}
inline GLAPI void APIENTRY glTexCoord4fVertex4fvSUN (const GLfloat *tc, const GLfloat *v) {
  osi.glr->glExt.glTexCoord4fVertex4fvSUN (tc, v);}
inline GLAPI void APIENTRY glTexCoord2fColor4ubVertex3fSUN (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glTexCoord2fColor4ubVertex3fSUN (s, t, r, g, b, a, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fColor4ubVertex3fvSUN (const GLfloat *tc, const GLubyte *c, const GLfloat *v) {
  osi.glr->glExt.glTexCoord2fColor4ubVertex3fvSUN (tc, c, v);}
inline GLAPI void APIENTRY glTexCoord2fColor3fVertex3fSUN (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glTexCoord2fColor3fVertex3fSUN (s, t, r, g, b, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fColor3fVertex3fvSUN (const GLfloat *tc, const GLfloat *c, const GLfloat *v) {
  osi.glr->glExt.glTexCoord2fColor3fVertex3fvSUN (tc, c, v);}
inline GLAPI void APIENTRY glTexCoord2fNormal3fVertex3fSUN (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glTexCoord2fNormal3fVertex3fSUN (s, t, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fNormal3fVertex3fvSUN (const GLfloat *tc, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glTexCoord2fNormal3fVertex3fvSUN (tc, n, v);}
inline GLAPI void APIENTRY glTexCoord2fColor4fNormal3fVertex3fSUN (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glTexCoord2fColor4fNormal3fVertex3fSUN (s, t, r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fColor4fNormal3fVertex3fvSUN (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glTexCoord2fColor4fNormal3fVertex3fvSUN (tc, c, n, v);}
inline GLAPI void APIENTRY glTexCoord4fColor4fNormal3fVertex4fSUN (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  osi.glr->glExt.glTexCoord4fColor4fNormal3fVertex4fSUN (s, t, p, q, r, g, b, a, nx, ny, nz, x, y, z, w);}
inline GLAPI void APIENTRY glTexCoord4fColor4fNormal3fVertex4fvSUN (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glTexCoord4fColor4fNormal3fVertex4fvSUN (tc, c, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiVertex3fSUN (GLuint rc, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiVertex3fSUN (rc, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiVertex3fvSUN (const GLuint *rc, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiVertex3fvSUN (rc, v);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4ubVertex3fSUN (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiColor4ubVertex3fSUN (rc, r, g, b, a, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4ubVertex3fvSUN (const GLuint *rc, const GLubyte *c, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiColor4ubVertex3fvSUN (rc, c, v);}
inline GLAPI void APIENTRY glReplacementCodeuiColor3fVertex3fSUN (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiColor3fVertex3fSUN (rc, r, g, b, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiColor3fVertex3fvSUN (const GLuint *rc, const GLfloat *c, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiColor3fVertex3fvSUN (rc, c, v);}
inline GLAPI void APIENTRY glReplacementCodeuiNormal3fVertex3fSUN (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiNormal3fVertex3fSUN (rc, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiNormal3fVertex3fvSUN (rc, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4fNormal3fVertex3fSUN (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiColor4fNormal3fVertex3fSUN (rc, r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4fNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiColor4fNormal3fVertex3fvSUN (rc, c, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fVertex3fSUN (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiTexCoord2fVertex3fSUN (rc, s, t, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fVertex3fvSUN (const GLuint *rc, const GLfloat *tc, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiTexCoord2fVertex3fvSUN (rc, tc, v);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN (rc, s, t, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN (rc, tc, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  osi.glr->glExt.glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN (rc, s, t, r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  osi.glr->glExt.glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN (rc, tc, c, n, v);}





GlExt _glOTHERlistEmpty[]= {
  {"GL_ARB_imaging",              0},
  {"GL_ARB_bindless_texture",     0},
  {"GL_INGR_blend_func_separate", 0},
  {"GL_SGIX_fragment_lighting",   0},
  {"GL_SGIX_polynomial_ffd",      0},
  {"GL_SGIS_point_parameters",    0},
  {"GL_SGIX_igloo_interface",     0},
  {"WGL_NV_vertex_array_range",   0},
  {null, 0}
};

// ARB list ===================-------------------------------------------
GlExt _glARBlistEmpty[]= {
  {"GL_ARB_multitexture",            0}, // http://www.opengl.org/registry/specs/ARB/multitexture.txt
  {"GLX_ARB_get_proc_address",       0}, // http://www.opengl.org/registry/specs/ARB/get_proc_address.txt
  {"GL_ARB_transpose_matrix",        0}, // http://www.opengl.org/registry/specs/ARB/transpose_matrix.txt
  {"WGL_ARB_buffer_region",          0}, // http://www.opengl.org/registry/specs/ARB/wgl_buffer_region.txt
  {"GL_ARB_multisample",             0}, // GLX_ARB_multisample WGL_ARB_multisample http://www.opengl.org/registry/specs/ARB/multisample.txt
  {"GL_ARB_texture_env_add",         0}, // http://www.opengl.org/registry/specs/ARB/texture_env_add.txt
  {"GL_ARB_texture_cube_map",        0}, // http://www.opengl.org/registry/specs/ARB/texture_cube_map.txt
  {"WGL_ARB_extensions_string",      0}, // http://www.opengl.org/registry/specs/ARB/wgl_extensions_string.txt
  {"WGL_ARB_pixel_format",           0}, // http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
  {"WGL_ARB_make_current_read",      0}, // #10 http://www.opengl.org/registry/specs/ARB/wgl_make_current_read.txt
  {"WGL_ARB_pbuffer",                0}, // http://www.opengl.org/registry/specs/ARB/wgl_pbuffer.txt
  {"GL_ARB_texture_compression",     0}, // http://www.opengl.org/registry/specs/ARB/texture_compression.txt
  {"GL_ARB_texture_border_clamp",    0}, // http://www.opengl.org/registry/specs/ARB/texture_border_clamp.txt
  {"GL_ARB_point_parameters",        0}, // http://www.opengl.org/registry/specs/ARB/point_parameters.txt
  {"GL_ARB_vertex_blend",            0}, // http://www.opengl.org/registry/specs/ARB/vertex_blend.txt
  {"GL_ARB_matrix_palette",          0}, // http://www.opengl.org/registry/specs/ARB/matrix_palette.txt
  {"GL_ARB_texture_env_combine",     0}, // http://www.opengl.org/registry/specs/ARB/texture_env_combine.txt
  {"GL_ARB_texture_env_crossbar",    0}, // http://www.opengl.org/registry/specs/ARB/texture_env_crossbar.txt
  {"GL_ARB_texture_env_dot3",        0}, // http://www.opengl.org/registry/specs/ARB/texture_env_dot3.txt
  {"WGL_ARB_render_texture",         0}, // #20 http://www.opengl.org/registry/specs/ARB/wgl_render_texture.txt
  {"GL_ARB_texture_mirrored_repeat", 0}, // http://www.opengl.org/registry/specs/ARB/texture_mirrored_repeat.txt
  {"GL_ARB_depth_texture",           0}, // http://www.opengl.org/registry/specs/ARB/depth_texture.txt
  {"GL_ARB_shadow",                  0}, // http://www.opengl.org/registry/specs/ARB/shadow.txt
  {"GL_ARB_shadow_ambient",          0}, // http://www.opengl.org/registry/specs/ARB/shadow_ambient.txt
  {"GL_ARB_window_pos",              0}, // http://www.opengl.org/registry/specs/ARB/window_pos.txt
  {"GL_ARB_vertex_program",          0}, // http://www.opengl.org/registry/specs/ARB/vertex_program.txt
  {"GL_ARB_fragment_program",        0}, // http://www.opengl.org/registry/specs/ARB/fragment_program.txt
  {"GL_ARB_vertex_buffer_object",    0}, // GLX_ARB_vertex_buffer_object http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
  {"GL_ARB_occlusion_query",         0}, // http://www.opengl.org/registry/specs/ARB/occlusion_query.txt
  {"GL_ARB_shader_objects",          0}, // #30 http://www.opengl.org/registry/specs/ARB/shader_objects.txt
  {"GL_ARB_vertex_shader",           0}, // http://www.opengl.org/registry/specs/ARB/vertex_shader.txt
  {"GL_ARB_fragment_shader",           0}, // http://www.opengl.org/registry/specs/ARB/fragment_shader.txt
  {"GL_ARB_shading_language_100",      0}, // http://www.opengl.org/registry/specs/ARB/shading_language_100.txt
  {"GL_ARB_texture_non_power_of_two",  0}, // http://www.opengl.org/registry/specs/ARB/texture_non_power_of_two.txt
  {"GL_ARB_point_sprite",              0}, // http://www.opengl.org/registry/specs/ARB/point_sprite.txt
  {"GL_ARB_fragment_program_shadow",   0}, // http://www.opengl.org/registry/specs/ARB/fragment_program_shadow.txt
  {"GL_ARB_draw_buffers",              0}, // http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
  {"GL_ARB_texture_rectangle",         0}, // http://www.opengl.org/registry/specs/ARB/texture_rectangle.txt
  {"GL_ARB_color_buffer_float",        0}, // WGL_ARB_pixel_format_float GLX_ARB_fbconfig_float http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
  {"GL_ARB_half_float_pixel",          0}, // #40 http://www.opengl.org/registry/specs/ARB/half_float_pixel.txt
  {"GL_ARB_texture_float",             0}, // http://www.opengl.org/registry/specs/ARB/texture_float.txt
  {"GL_ARB_pixel_buffer_object",       0}, // http://www.opengl.org/registry/specs/ARB/pixel_buffer_object.txt
  {"GL_ARB_depth_buffer_float",        0}, // http://www.opengl.org/registry/specs/ARB/depth_buffer_float.txt
  {"GL_ARB_draw_instanced",            0}, // http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
  {"GL_ARB_framebuffer_object",        0}, // http://www.opengl.org/registry/specs/ARB/framebuffer_object.txt
  {"GL_ARB_framebuffer_sRGB",          0}, // GLX_ARB_framebuffer_sRGB WGL_ARB_framebuffer_sRGB http://www.opengl.org/registry/specs/ARB/framebuffer_sRGB.txt
  {"GL_ARB_geometry_shader4",          0}, // http://www.opengl.org/registry/specs/ARB/geometry_shader4.txt
  {"GL_ARB_half_float_vertex",         0}, // http://www.opengl.org/registry/specs/ARB/half_float_vertex.txt
  {"GL_ARB_instanced_arrays",          0}, // http://www.opengl.org/registry/specs/ARB/instanced_arrays.txt
  {"GL_ARB_map_buffer_range",          0}, // #50 http://www.opengl.org/registry/specs/ARB/map_buffer_range.txt
  {"GL_ARB_texture_buffer_object",     0}, // http://www.opengl.org/registry/specs/ARB/texture_buffer_object.txt
  {"GL_ARB_texture_compression_rgtc",  0}, // http://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
  {"GL_ARB_texture_rg",                0}, // http://www.opengl.org/registry/specs/ARB/texture_rg.txt
  {"GL_ARB_vertex_array_object",       0}, // http://www.opengl.org/registry/specs/ARB/vertex_array_object.txt
  {"WGL_ARB_create_context",           0}, // !!! WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
  {"GLX_ARB_create_context",           0}, // !!! GLX_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
  {"GL_ARB_uniform_buffer_object",     0}, // http://www.opengl.org/registry/specs/ARB/uniform_buffer_object.txt
  {"GL_ARB_compatibility",             0}, // http://www.opengl.org/registry/specs/ARB/compatibility.txt
  {"GL_ARB_copy_buffer",               0}, // http://www.opengl.org/registry/specs/ARB/copy_buffer.txt
  {"GL_ARB_shader_texture_lod",        0}, // #60 http://www.opengl.org/registry/specs/ARB/shader_texture_lod.txt
  {"GL_ARB_depth_clamp",               0}, // http://www.opengl.org/registry/specs/ARB/depth_clamp.txt
  {"GL_ARB_draw_elements_base_vertex", 0}, // http://www.opengl.org/registry/specs/ARB/draw_elements_base_vertex.txt
  {"GL_ARB_fragment_coord_conventions", 0}, // http://www.opengl.org/registry/specs/ARB/fragment_coord_conventions.txt
  {"GL_ARB_provoking_vertex",          0}, // http://www.opengl.org/registry/specs/ARB/provoking_vertex.txt
  {"GL_ARB_seamless_cube_map",         0}, // http://www.opengl.org/registry/specs/ARB/seamless_cube_map.txt
  {"GL_ARB_sync",                      0}, // http://www.opengl.org/registry/specs/ARB/sync.txt
  {"GL_ARB_texture_multisample",       0}, // http://www.opengl.org/registry/specs/ARB/texture_multisample.txt
  {"GL_ARB_vertex_array_bgra",         0}, // http://www.opengl.org/registry/specs/ARB/vertex_array_bgra.txt
  {"GL_ARB_draw_buffers_blend",        0}, // http://www.opengl.org/registry/specs/ARB/draw_buffers_blend.txt
  {"GL_ARB_sample_shading",            0}, // #70 http://www.opengl.org/registry/specs/ARB/sample_shading.txt
  {"GL_ARB_texture_cube_map_array",    0}, // http://www.opengl.org/registry/specs/ARB/texture_cube_map_array.txt
  {"GL_ARB_texture_gather",            0}, // http://www.opengl.org/registry/specs/ARB/texture_gather.txt
  {"GL_ARB_texture_query_lod",         0}, // http://www.opengl.org/registry/specs/ARB/texture_query_lod.txt
  {"WGL_ARB_create_context",           0}, // WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
  {"GLX_ARB_create_context",           0}, // http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
  {"GL_ARB_shading_language_include",  0}, // http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
  {"GL_ARB_texture_compression_bptc",  0}, // http://www.opengl.org/registry/specs/ARB/texture_compression_bptc.txt
  {"GL_ARB_blend_func_extended",       0}, // http://www.opengl.org/registry/specs/ARB/blend_func_extended.txt
  {"GL_ARB_explicit_attrib_location",  0}, // http://www.opengl.org/registry/specs/ARB/explicit_attrib_location.txt
  {"GL_ARB_occlusion_query2",          0}, // #80 http://www.opengl.org/registry/specs/ARB/occlusion_query2.txt
  {"GL_ARB_sampler_objects",           0}, // http://www.opengl.org/registry/specs/ARB/sampler_objects.txt
  {"GL_ARB_shader_bit_encoding",       0}, // http://www.opengl.org/registry/specs/ARB/shader_bit_encoding.txt
  {"GL_ARB_texture_rgb10_a2ui",        0}, // http://www.opengl.org/registry/specs/ARB/texture_rgb10_a2ui.txt
  {"GL_ARB_texture_swizzle",           0}, // http://www.opengl.org/registry/specs/ARB/texture_swizzle.txt
  {"GL_ARB_timer_query",               0}, // !! http://www.opengl.org/registry/specs/ARB/timer_query.txt
  {"GL_ARB_vertex_type_2_10_10_10_rev",  0}, // http://www.opengl.org/registry/specs/ARB/vertex_type_2_10_10_10_rev.txt
  {"GL_ARB_draw_indirect",               0}, // http://www.opengl.org/registry/specs/ARB/draw_indirect.txt
  {"GL_ARB_gpu_shader5",                 0}, // http://www.opengl.org/registry/specs/ARB/gpu_shader5.txt
  {"GL_ARB_gpu_shader_fp64",             0}, // http://www.opengl.org/registry/specs/ARB/gpu_shader_fp64.txt
  {"GL_ARB_shader_subroutine",           0}, // #90 http://www.opengl.org/registry/specs/ARB/shader_subroutine.txt
  {"GL_ARB_tessellation_shader",         0}, // http://www.opengl.org/registry/specs/ARB/tessellation_shader.txt
  {"GL_ARB_texture_buffer_object_rgb32", 0}, // http://www.opengl.org/registry/specs/ARB/texture_buffer_object_rgb32.txt
  {"GL_ARB_transform_feedback2",         0}, // http://www.opengl.org/registry/specs/ARB/transform_feedback2.txt
  {"GL_ARB_transform_feedback3",         0}, // http://www.opengl.org/registry/specs/ARB/transform_feedback3.txt
  {"GL_ARB_ES2_compatibility",           0}, // http://www.opengl.org/registry/specs/ARB/ES2_compatibility.txt
  {"GL_ARB_get_program_binary",          0}, // http://www.opengl.org/registry/specs/ARB/get_program_binary.txt
  {"GL_ARB_separate_shader_objects",     0}, // http://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt
  {"GL_ARB_shader_precision",            0}, // http://www.opengl.org/registry/specs/ARB/shader_precision.txt
  {"GL_ARB_vertex_attrib_64bit",         0}, // http://www.opengl.org/registry/specs/ARB/vertex_attrib_64bit.txt
  {"GL_ARB_viewport_array",                0}, // #100 http://www.opengl.org/registry/specs/ARB/viewport_array.txt
  {"GLX_ARB_create_context_robustness",    0}, // http://www.opengl.org/registry/specs/ARB/glx_create_context_robustness.txt
  {"WGL_ARB_create_context_robustness",    0}, // http://www.opengl.org/registry/specs/ARB/wgl_create_context_robustness.txt
  {"GL_ARB_cl_event",                      0}, // http://www.opengl.org/registry/specs/ARB/cl_event.txt
  {"GL_ARB_debug_output",                  0}, // http://www.opengl.org/registry/specs/ARB/debug_output.txt
  {"GL_ARB_robustness",                    0}, // http://www.opengl.org/registry/specs/ARB/robustness.txt
  {"GL_ARB_shader_stencil_export",         0}, // http://www.opengl.org/registry/specs/ARB/shader_stencil_export.txt
  {"GL_ARB_base_instance",                 0}, // http://www.opengl.org/registry/specs/ARB/base_instance.txt
  {"GL_ARB_shading_language_420pack",      0}, // http://www.opengl.org/registry/specs/ARB/shading_language_420pack.txt
  {"GL_ARB_transform_feedback_instanced",  0}, // http://www.opengl.org/registry/specs/ARB/transform_feedback_instanced.txt
  {"GL_ARB_compressed_texture_pixel_storage", 0}, // #110 http://www.opengl.org/registry/specs/ARB/compressed_texture_pixel_storage.txt
  {"GL_ARB_conservative_depth",            0}, // http://www.opengl.org/registry/specs/ARB/conservative_depth.txt
  {"GL_ARB_internalformat_query",          0}, // http://www.opengl.org/registry/specs/ARB/internalformat_query.txt
  {"GL_ARB_map_buffer_alignment",          0}, // http://www.opengl.org/registry/specs/ARB/map_buffer_alignment.txt
  {"GL_ARB_shader_atomic_counters",        0}, // http://www.opengl.org/registry/specs/ARB/shader_atomic_counters.txt
  {"GL_ARB_shader_image_load_store",       0}, // http://www.opengl.org/registry/specs/ARB/shader_image_load_store.txt
  {"GL_ARB_shading_language_packing",      0}, // http://www.opengl.org/registry/specs/ARB/shading_language_packing.txt
  {"GL_ARB_texture_storage",               0}, // http://www.opengl.org/registry/specs/ARB/texture_storage.txt
  {"GL_KHR_texture_compression_astc_hdr",  0}, // GL_KHR_texture_compression_astc_ldr http://www.opengl.org/registry/specs/KHR/texture_compression_astc_hdr.txt
  {"GL_KHR_debug",                         0}, // http://www.opengl.org/registry/specs/KHR/debug.txt
  {"GL_ARB_arrays_of_arrays",              0}, // #120 http://www.opengl.org/registry/specs/ARB/arrays_of_arrays.txt
  {"GL_ARB_clear_buffer_object",           0}, // http://www.opengl.org/registry/specs/ARB/clear_buffer_object.txt
  {"GL_ARB_compute_shader",                0}, // http://www.opengl.org/registry/specs/ARB/compute_shader.txt
  {"GL_ARB_copy_image",                    0}, // http://www.opengl.org/registry/specs/ARB/copy_image.txt
  {"GL_ARB_texture_view",                  0}, // http://www.opengl.org/registry/specs/ARB/texture_view.txt
  {"GL_ARB_vertex_attrib_binding",         0}, // http://www.opengl.org/registry/specs/ARB/vertex_attrib_binding.txt
  {"GL_ARB_robustness_application_isolation", 0}, // GL_ARB_robustness_share_group_isolation http://www.opengl.org/registry/specs/ARB/robustness_isolation.txt
  {"GL_ARB_ES3_compatibility",             0}, // http://www.opengl.org/registry/specs/ARB/ES3_compatibility.txt
  {"GL_ARB_explicit_uniform_location",     0}, // http://www.opengl.org/registry/specs/ARB/explicit_uniform_location.txt
  {"GL_ARB_fragment_layer_viewport",       0}, // http://www.opengl.org/registry/specs/ARB/fragment_layer_viewport.txt
  {"GL_ARB_framebuffer_no_attachments",    0}, // #130 http://www.opengl.org/registry/specs/ARB/framebuffer_no_attachments.txt
  {"GL_ARB_internalformat_query2",         0}, // http://www.opengl.org/registry/specs/ARB/internalformat_query2.txt
  {"GL_ARB_invalidate_subdata",            0}, // !! http://www.opengl.org/registry/specs/ARB/invalidate_subdata.txt
  {"GL_ARB_multi_draw_indirect",           0}, // !! http://www.opengl.org/registry/specs/ARB/multi_draw_indirect.txt
  {"GL_ARB_program_interface_query",       0}, // http://www.opengl.org/registry/specs/ARB/program_interface_query.txt
  {"GL_ARB_robust_buffer_access_behavior", 0}, // http://www.opengl.org/registry/specs/ARB/robust_buffer_access_behavior.txt
  {"GL_ARB_shader_image_size",             0}, // http://www.opengl.org/registry/specs/ARB/shader_image_size.txt
  {"GL_ARB_shader_storage_buffer_object",  0}, // http://www.opengl.org/registry/specs/ARB/shader_storage_buffer_object.txt
  {"GL_ARB_stencil_texturing",             0}, // http://www.opengl.org/registry/specs/ARB/stencil_texturing.txt
  {"GL_ARB_texture_buffer_range",          0}, // http://www.opengl.org/registry/specs/ARB/texture_buffer_range.txt
  {"GL_ARB_texture_query_levels",          0}, // #140 http://www.opengl.org/registry/specs/ARB/texture_query_levels.txt
  {"GL_ARB_texture_storage_multisample",   0}, // http://www.opengl.org/registry/specs/ARB/texture_storage_multisample.txt
  {"GLX_ARB_robustness_application_isolation", 0}, // GLX_ARB_robustness_share_group_isolation http://www.opengl.org/registry/specs/ARB/glx_robustness_isolation.txt
  {"WGL_ARB_robustness_application_isolation", 0}, // WGL_ARB_robustness_share_group_isolation http://www.opengl.org/registry/specs/ARB/wgl_robustness_isolation.txt
  {"GL_ARB_buffer_storage",                0}, // http://www.opengl.org/registry/specs/ARB/buffer_storage.txt
  {"GL_ARB_clear_texture",                 0}, // !! http://www.opengl.org/registry/specs/ARB/clear_texture.txt
  {"GL_ARB_enhanced_layouts",              0}, // has batman in overview! http://www.opengl.org/registry/specs/ARB/enhanced_layouts.txt
  {"GL_ARB_multi_bind",                    0}, // http://www.opengl.org/registry/specs/ARB/multi_bind.txt
  {"GL_ARB_query_buffer_object",           0}, // http://www.opengl.org/registry/specs/ARB/query_buffer_object.txt
  {"GL_ARB_texture_mirror_clamp_to_edge",  0}, // http://www.opengl.org/registry/specs/ARB/texture_mirror_clamp_to_edge.txt
  {"GL_ARB_texture_stencil8",              0}, // #150 http://www.opengl.org/registry/specs/ARB/texture_stencil8.txt
  {"GL_ARB_vertex_type_10f_11f_11f_rev",   0}, // http://www.opengl.org/registry/specs/ARB/vertex_type_10f_11f_11f_rev.txt
  {"GL_ARB_bindless_texture",              0}, // http://www.opengl.org/registry/specs/ARB/bindless_texture.txt
  {"GL_ARB_compute_variable_group_size",   0}, // http://www.opengl.org/registry/specs/ARB/compute_variable_group_size.txt
  {"GL_ARB_indirect_parameters",           0}, // http://www.opengl.org/registry/specs/ARB/indirect_parameters.txt
  {"GL_ARB_seamless_cubemap_per_texture",  0}, // http://www.opengl.org/registry/specs/ARB/seamless_cubemap_per_texture.txt
  {"GL_ARB_shader_draw_parameters",        0}, // shader stuff http://www.opengl.org/registry/specs/ARB/shader_draw_parameters.txt
  {"GL_ARB_shader_group_vote",             0}, // shader stuff http://www.opengl.org/registry/specs/ARB/shader_group_vote.txt
  {"GL_ARB_sparse_texture",                0}, // http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
  {"GL_ARB_ES3_1_compatibility",           0}, // http://www.opengl.org/registry/specs/ARB/ES3_1_compatibility.txt
  {"GL_ARB_clip_control",                  0}, // #160 http://www.opengl.org/registry/specs/ARB/clip_control.txt
  {"GL_ARB_conditional_render_inverted",   0}, // http://www.opengl.org/registry/specs/ARB/conditional_render_inverted.txt
  {"GL_ARB_cull_distance",                 0}, // http://www.opengl.org/registry/specs/ARB/cull_distance.txt
  {"GL_ARB_derivative_control",            0}, // http://www.opengl.org/registry/specs/ARB/derivative_control.txt
  {"GL_ARB_direct_state_access",           0}, // http://www.opengl.org/registry/specs/ARB/direct_state_access.txt
  {"GL_ARB_get_texture_sub_image",         0}, // http://www.opengl.org/registry/specs/ARB/get_texture_sub_image.txt
  {"GL_ARB_shader_texture_image_samples",  0}, // http://www.opengl.org/registry/specs/ARB/shader_texture_image_samples.txt
  {"GL_ARB_texture_barrier",               0}, // http://www.opengl.org/registry/specs/ARB/texture_barrier.txt
  {"GL_KHR_context_flush_control",         0}, // GLX_ARB_context_flush_control WGL_ARB_context_flush_control http://www.opengl.org/registry/specs/KHR/context_flush_control.txt
  {"GL_KHR_robust_buffer_access_behavior", 0}, // http://www.opengl.org/registry/specs/KHR/robust_buffer_access_behavior.txt
  {"GL_KHR_robustness",                    0}, // #170 http://www.opengl.org/registry/specs/KHR/robustness.txt
  {"GL_ARB_pipeline_statistics_query",     0}, // http://www.opengl.org/registry/specs/ARB/pipeline_statistics_query.txt
  {"GL_ARB_sparse_buffer",                 0}, // http://www.opengl.org/registry/specs/ARB/sparse_buffer.txt
  {"GL_ARB_transform_feedback_overflow_query", 0}, // #173 http://www.opengl.org/registry/specs/ARB/transform_feedback_overflow_query.txt
  {null, 0} // terminator
};


// EXT & vendor list ===================-------------------------------------------
GlExt _glEXTlistEmpty[]= {
  {"GL_EXT_abgr", 0},               // #1 http://www.opengl.org/registry/specs/EXT/abgr.txt
  {"GL_EXT_blend_color", 0},        // http://www.opengl.org/registry/specs/EXT/blend_color.txt
  {"GL_EXT_polygon_offset", 0},     // http://www.opengl.org/registry/specs/EXT/polygon_offset.txt
  {"GL_EXT_texture", 0},            // http://www.opengl.org/registry/specs/EXT/texture.txt
  {"", 0}, // 
  {"GL_EXT_texture3D", 0},          // http://www.opengl.org/registry/specs/EXT/texture3D.txt
  {"GL_SGIS_texture_filter4", 0},   // http://www.opengl.org/registry/specs/SGIS/texture_filter4.txt
  {"", 0}, // 
  {"GL_EXT_subtexture", 0},         // http://www.opengl.org/registry/specs/EXT/subtexture.txt
  {"GL_EXT_copy_texture", 0},       // #10 http://www.opengl.org/registry/specs/EXT/copy_texture.txt
  {"GL_EXT_histogram", 0},          // http://www.opengl.org/registry/specs/EXT/histogram.txt
  {"GL_EXT_convolution", 0},        // http://www.opengl.org/registry/specs/EXT/convolution.txt
  {"GL_SGI_color_matrix", 0},       // http://www.opengl.org/registry/specs/SGI/color_matrix.txt
  {"GL_SGI_color_table", 0},        // http://www.opengl.org/registry/specs/SGI/color_table.txt
  {"GL_SGIS_pixel_texture", 0},     // GL_SGIX_pixel_texture #15 & #15a http://www.opengl.org/registry/specs/SGIS/pixel_texture.txt http://www.opengl.org/registry/specs/SGIX/sgix_pixel_texture.txt
  {"GL_SGIS_texture4D", 0},         // http://www.opengl.org/registry/specs/SGIS/texture4D.txt
  {"GL_SGI_texture_color_table", 0}, // http://www.opengl.org/registry/specs/SGI/texture_color_table.txt
  {"GL_EXT_cmyka", 0},              // http://www.opengl.org/registry/specs/EXT/cmyka.txt
  {"", 0}, // 
  {"GL_EXT_texture_object", 0},     // #20 http://www.opengl.org/registry/specs/EXT/texture_object.txt
  {"GL_SGIS_detail_texture", 0},    // http://www.opengl.org/registry/specs/SGIS/detail_texture.txt
  {"GL_SGIS_sharpen_texture", 0},   // http://www.opengl.org/registry/specs/SGIS/sharpen_texture.txt
  {"GL_EXT_packed_pixels", 0},      // http://www.opengl.org/registry/specs/EXT/packed_pixels.txt
  {"GL_SGIS_texture_lod", 0},       // http://www.opengl.org/registry/specs/SGIS/texture_lod.txt
  {"GL_SGIS_multisample", 0},       // #25 GLX_SGIS_multisample http://www.opengl.org/registry/specs/SGIS/multisample.txt
  {"", 0}, // 
  {"GL_EXT_rescale_normal", 0},     // http://www.opengl.org/registry/specs/EXT/rescale_normal.txt
  {"GLX_EXT_visual_info", 0},       // http://www.opengl.org/registry/specs/EXT/visual_info.txt
  {"", 0}, // 
  {"GL_EXT_vertex_array", 0},       // #30 http://www.opengl.org/registry/specs/EXT/vertex_array.txt
  {"GL_EXT_misc_attribute", 0},     // http://www.opengl.org/registry/specs/EXT/misc_attribute.txt
  {"GL_SGIS_generate_mipmap", 0},   // http://www.opengl.org/registry/specs/SGIS/generate_mipmap.txt
  {"GL_SGIX_clipmap", 0},           // incomplete http://www.opengl.org/registry/specs/SGIX/clipmap.txt
  {"GL_SGIX_shadow", 0},            // incomplete http://www.opengl.org/registry/specs/SGIX/shadow.txt
  {"GL_SGIS_texture_edge_clamp", 0}, // http://www.opengl.org/registry/specs/SGIS/texture_edge_clamp.txt
  {"GL_SGIS_texture_border_clamp", 0}, // http://www.opengl.org/registry/specs/SGIS/texture_border_clamp.txt
  {"GL_EXT_blend_minmax", 0},       // http://www.opengl.org/registry/specs/EXT/blend_minmax.txt
  {"GL_EXT_blend_subtract", 0},     // http://www.opengl.org/registry/specs/EXT/blend_subtract.txt
  {"GL_EXT_blend_logic_op", 0},     // http://www.opengl.org/registry/specs/EXT/blend_logic_op.txt
  {"GLX_SGI_swap_control", 0},      // #40 http://www.opengl.org/registry/specs/SGI/swap_control.txt
  {"GLX_SGI_video_sync", 0},        // http://www.opengl.org/registry/specs/SGI/video_sync.txt
  {"GLX_SGI_make_current_read", 0}, // http://www.opengl.org/registry/specs/SGI/make_current_read.txt
  {"GLX_SGIX_video_source", 0},     // http://www.opengl.org/registry/specs/SGIX/video_source.txt
  {"GLX_EXT_visual_rating", 0},     // http://www.opengl.org/registry/specs/EXT/visual_rating.txt
  {"GL_SGIX_interlace", 0},         // http://www.opengl.org/registry/specs/SGIX/interlace.txt
  {"", 0}, // 
  {"GLX_EXT_import_context", 0},    // http://www.opengl.org/registry/specs/EXT/import_context.txt
  {"", 0}, // 
  {"GLX_SGIX_fbconfig", 0},         // http://www.opengl.org/registry/specs/SGIX/fbconfig.txt
  {"GLX_SGIX_pbuffer", 0},          // #50 http://www.opengl.org/registry/specs/SGIX/pbuffer.txt
  {"GL_SGIS_texture_select", 0},    // http://www.opengl.org/registry/specs/SGIS/texture_select.txt
  {"GL_SGIX_sprite", 0},            // http://www.opengl.org/registry/specs/SGIX/sprite.txt
  {"GL_SGIX_texture_multi_buffer", 0}, // http://www.opengl.org/registry/specs/SGIX/texture_multi_buffer.txt
  {"GL_EXT_point_parameters", 0},   // http://www.opengl.org/registry/specs/EXT/point_parameters.txt
  {"GL_SGIX_instruments", 0},       // http://www.opengl.org/registry/specs/SGIX/instruments.txt
  {"GL_SGIX_texture_scale_bias", 0}, // http://www.opengl.org/registry/specs/SGIX/texture_scale_bias.txt
  {"GL_SGIX_framezoom", 0},         // http://www.opengl.org/registry/specs/SGIX/framezoom.txt
  {"GL_SGIX_tag_sample_buffer", 0}, // http://www.opengl.org/registry/specs/SGIX/tag_sample_buffer.txt
  {"", 0}, // 
  {"GL_SGIX_reference_plane", 0},   // #60 http://www.opengl.org/registry/specs/SGIX/reference_plane.txt
  {"GL_SGIX_flush_raster", 0},      // http://www.opengl.org/registry/specs/SGIX/flush_raster.txt
  {"GLX_SGI_cushion", 0},           // http://www.opengl.org/registry/specs/SGI/cushion.txt
  {"GL_SGIX_depth_texture", 0},     // http://www.opengl.org/registry/specs/SGIX/depth_texture.txt
  {"GL_SGIS_fog_function", 0},      // http://www.opengl.org/registry/specs/SGIS/fog_func.txt
  {"GL_SGIX_fog_offset", 0},        // http://www.opengl.org/registry/specs/SGIX/fog_offset.txt
  {"GL_HP_image_transform", 0},     // http://www.opengl.org/registry/specs/HP/image_transform.txt
  {"GL_HP_convolution_border_modes", 0}, // http://www.opengl.org/registry/specs/HP/convolution_border_modes.txt
  {"", 0}, // 
  {"GL_SGIX_texture_add_env", 0},   // http://www.opengl.org/registry/specs/SGIX/texture_env_add.txt
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"GL_EXT_color_subtable", 0},     // #74 http://www.opengl.org/registry/specs/EXT/color_subtable.txt
  {"GLU_EXT_object_space_tess", 0}, // http://www.opengl.org/registry/specs/EXT/object_space_tess.txt
  {"GL_PGI_vertex_hints", 0},       // http://www.opengl.org/registry/specs/PGI/vertex_hints.txt
  {"GL_PGI_misc_hints", 0},         // http://www.opengl.org/registry/specs/PGI/misc_hints.txt
  {"GL_EXT_paletted_texture", 0},   // http://www.opengl.org/registry/specs/EXT/paletted_texture.txt
  {"GL_EXT_clip_volume_hint", 0},   // http://www.opengl.org/registry/specs/EXT/clip_volume_hint.txt
  {"GL_SGIX_list_priority", 0},     // #80 incomplete http://www.opengl.org/registry/specs/SGIX/list_priority.txt
  {"GL_SGIX_ir_instrument1", 0},    // http://www.opengl.org/registry/specs/SGIX/ir_instrument1.txt
  {"", 0}, // 
  {"GLX_SGIX_video_resize", 0},     // http://www.opengl.org/registry/specs/SGIX/video_resize.txt
  {"GL_SGIX_texture_lod_bias", 0},  // http://www.opengl.org/registry/specs/SGIX/texture_lod_bias.txt
  {"GLU_SGI_filter4_parameters", 0}, // incomplete http://www.opengl.org/registry/specs/SGI/filter4_parameters.txt
  {"GLX_SGIX_dm_buffer", 0},        // incomplete !!!GLX_SGIX_dmbuffer!!! http://www.opengl.org/registry/specs/SGIX/dmbuffer.txt
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"GL_SGIX_shadow_ambient", 0},      // #90 http://www.opengl.org/registry/specs/SGIX/shadow_ambient.txt
  {"GLX_SGIX_swap_group", 0},         // http://www.opengl.org/registry/specs/SGIX/swap_group.txt
  {"GLX_SGIX_swap_barrier", 0},       // http://www.opengl.org/registry/specs/SGIX/swap_barrier.txt
  {"GL_EXT_index_texture", 0},        // http://www.opengl.org/registry/specs/EXT/index_texture.txt
  {"GL_EXT_index_material", 0},       // http://www.opengl.org/registry/specs/EXT/index_material.txt
  {"GL_EXT_index_func", 0},           // http://www.opengl.org/registry/specs/EXT/index_func.txt
  {"GL_EXT_index_array_formats", 0},  // incomplete http://www.opengl.org/registry/specs/EXT/index_array_formats.txt
  {"GL_EXT_compiled_vertex_array", 0}, // incomplete http://www.opengl.org/registry/specs/EXT/compiled_vertex_array.txt
  {"GL_EXT_cull_vertex", 0},          // incomplete http://www.opengl.org/registry/specs/EXT/cull_vertex.txt
  {"", 0}, // 
  {"GLU_EXT_nurbs_tessellator", 0},   // #100 http://www.opengl.org/registry/specs/EXT/nurbs_tessellator.txt
  {"GL_SGIX_ycrcb", 0},               // incomplete http://www.opengl.org/registry/specs/SGIX/ycrcb.txt
  {"GL_EXT_fragment_lighting", 0},    // incomplete http://www.opengl.org/registry/specs/EXT/fragment_lighting.txt
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"GL_IBM_rasterpos_clip", 0},       // #110 http://www.opengl.org/registry/specs/IBM/rasterpos_clip.txt
  {"GL_HP_texture_lighting", 0},      // incomplete http://www.opengl.org/registry/specs/HP/texture_lighting.txt
  {"GL_EXT_draw_range_elements", 0},  // http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
  {"GL_WIN_phong_shading", 0},        // http://www.opengl.org/registry/specs/WIN/phong_shading.txt
  {"GL_WIN_specular_fog", 0},         // http://www.opengl.org/registry/specs/WIN/specular_fog.txt
  {"GL_SGIS_color_range", 0},         // GLX_SGIS_color_range http://www.opengl.org/registry/specs/SGIS/color_range.txt
  {"", 0}, // 
  {"GL_EXT_light_texture", 0},        // http://www.opengl.org/registry/specs/EXT/light_texture.txt
  {"", 0}, // 
  {"GL_SGIX_blend_alpha_minmax", 0},  // incomplete http://www.opengl.org/registry/specs/SGIX/blend_alpha_minmax.txt
  {"GL_EXT_scene_marker", 0},         // #120 GLX_EXT_scene_marker http://www.opengl.org/registry/specs/EXT/scene_marker.txt
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"GL_SGIX_pixel_texture_bits", 0},  // http://www.opengl.org/registry/specs/SGIX/pixel_texture_bits.txt
  {"", 0}, // 
  {"GL_EXT_bgra", 0},                 // http://www.opengl.org/registry/specs/EXT/bgra.txt
  {"", 0}, // #130
  {"", 0}, // 
  {"GL_SGIX_async", 0},               // #132 incomplete http://www.opengl.org/registry/specs/SGIX/async.txt
  {"GL_SGIX_async_pixel", 0},         // inc http://www.opengl.org/registry/specs/SGIX/async_pixel.txt
  {"GL_SGIX_async_histogram", 0},     // inc http://www.opengl.org/registry/specs/SGIX/async_histogram.txt
  {"GL_INTEL_texture_scissor", 0},    // http://www.opengl.org/registry/specs/INTEL/texture_scissor.txt
  {"GL_INTEL_parallel_arrays", 0},    // http://www.opengl.org/registry/specs/INTEL/parallel_arrays.txt
  {"GL_HP_occlusion_test", 0},        // inc http://www.opengl.org/registry/specs/HP/occlusion_test.txt
  {"GL_EXT_pixel_transform", 0},      // http://www.opengl.org/registry/specs/EXT/pixel_transform.txt
  {"GL_EXT_pixel_transform_color_table", 0}, // http://www.opengl.org/registry/specs/EXT/pixel_transform_color_table.txt
  {"", 0}, // #140
  {"GL_EXT_shared_texture_palette", 0}, // http://www.opengl.org/registry/specs/EXT/shared_texture_palette.txt
  {"GLX_SGIS_blended_overlay", 0},    // http://www.opengl.org/registry/specs/SGIS/blended_overlay.txt
  {"", 0}, // 
  {"GL_EXT_separate_specular_color", 0}, // http://www.opengl.org/registry/specs/EXT/separate_specular_color.txt
  {"GL_EXT_secondary_color", 0},      // http://www.opengl.org/registry/specs/EXT/secondary_color.txt
  {"GL_EXT_texture_env", 0},          // http://www.opengl.org/registry/specs/EXT/texture_env.txt
  {"GL_EXT_texture_perturb_normal", 0}, // http://www.opengl.org/registry/specs/EXT/texture_perturb_normal.txt
  {"GL_EXT_multi_draw_arrays", 0},    // GL_SUN_multi_draw_arrays http://www.opengl.org/registry/specs/EXT/multi_draw_arrays.txt
  {"GL_EXT_fog_coord", 0},            // http://www.opengl.org/registry/specs/EXT/fog_coord.txt
  {"", 0}, // #150
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"GL_REND_screen_coordinates", 0},  // inc http://www.opengl.org/registry/specs/REND/screen_coordinates.txt
  {"GL_EXT_coordinate_frame", 0},     // inc http://www.opengl.org/registry/specs/EXT/coordinate_frame.txt
  {"", 0}, // 
  {"GL_EXT_texture_env_combine", 0},  // http://www.opengl.org/registry/specs/EXT/texture_env_combine.txt
  {"GL_APPLE_specular_vector", 0},    // inc http://www.opengl.org/registry/specs/APPLE/specular_vector.txt
  {"GL_APPLE_transform_hint", 0},     // #160 inc http://www.opengl.org/registry/specs/APPLE/transform_hint.txt
  {"", 0}, // 
  {"", 0}, // 
  {"GL_SUNX_constant_data", 0},       // http://www.opengl.org/registry/specs/SUNX/constant_data.txt
  {"GL_SUN_global_alpha", 0},         // http://www.opengl.org/registry/specs/SUN/global_alpha.txt
  {"GL_SUN_triangle_list", 0},        // http://www.opengl.org/registry/specs/SUN/triangle_list.txt
  {"GL_SUN_vertex", 0},               // http://www.opengl.org/registry/specs/SUN/vertex.txt
  {"WGL_EXT_display_color_table", 0}, // inc http://www.opengl.org/registry/specs/EXT/wgl_display_color_table.txt
  {"WGL_EXT_extensions_string", 0},   // http://www.opengl.org/registry/specs/EXT/wgl_extensions_string.txt
  {"WGL_EXT_make_current_read", 0},   // http://www.opengl.org/registry/specs/EXT/wgl_make_current_read.txt
  {"WGL_EXT_pixel_format", 0},        // #170 http://www.opengl.org/registry/specs/EXT/wgl_pixel_format.txt
  {"WGL_EXT_pbuffer", 0},             // http://www.opengl.org/registry/specs/EXT/wgl_pbuffer.txt
  {"WGL_EXT_swap_control", 0},        // http://www.opengl.org/registry/specs/EXT/wgl_swap_control.txt
  {"GL_EXT_blend_func_separate", 0},  // http://www.opengl.org/registry/specs/EXT/blend_func_separate.txt
  {"GL_INGR_color_clamp", 0},         // http://www.opengl.org/registry/specs/INGR/color_clamp.txt
  {"GL_INGR_interlace_read", 0},      // http://www.opengl.org/registry/specs/INGR/interlace_read.txt
  {"GL_EXT_stencil_wrap", 0},         // http://www.opengl.org/registry/specs/EXT/stencil_wrap.txt
  {"WGL_EXT_depth_float", 0},         // http://www.opengl.org/registry/specs/EXT/wgl_depth_float.txt
  {"GL_EXT_422_pixels", 0},           // http://www.opengl.org/registry/specs/EXT/422_pixels.txt
  {"GL_NV_texgen_reflection", 0},     // http://www.opengl.org/registry/specs/NV/texgen_reflection.txt
  {"", 0}, // #180
  {"GL_SGIX_texture_range", 0},       // http://www.opengl.org/registry/specs/SGIX/texture_range.txt
  {"GL_SUN_convolution_border_modes", 0}, // http://www.opengl.org/registry/specs/SUN/convolution_border_modes.txt
  {"GLX_SUN_get_transparent_index", 0}, // http://www.opengl.org/registry/specs/SUN/get_transparent_index.txt
  {"", 0}, // 
  {"GL_EXT_texture_env_add", 0},      // http://www.opengl.org/registry/specs/EXT/texture_env_add.txt
  {"GL_EXT_texture_lod_bias", 0},     // http://www.opengl.org/registry/specs/EXT/texture_lod_bias.txt
  {"GL_EXT_texture_filter_anisotropic", 0}, // http://www.opengl.org/registry/specs/EXT/texture_filter_anisotropic.txt
  {"GL_EXT_vertex_weighting", 0},     // http://www.opengl.org/registry/specs/EXT/vertex_weighting.txt
  {"GL_NV_light_max_exponent", 0},    // http://www.opengl.org/registry/specs/NV/light_max_exponent.txt
  {"GL_NV_vertex_array_range", 0},    // #190 http://www.opengl.org/registry/specs/NV/vertex_array_range.txt
  {"GL_NV_register_combiners", 0},    // http://www.opengl.org/registry/specs/NV/register_combiners.txt
  {"GL_NV_fog_distance", 0},          // http://www.opengl.org/registry/specs/NV/fog_distance.txt
  {"GL_NV_texgen_emboss", 0},         // http://www.opengl.org/registry/specs/NV/texgen_emboss.txt
  {"GL_NV_blend_square", 0},          // http://www.opengl.org/registry/specs/NV/blend_square.txt
  {"GL_NV_texture_env_combine4", 0},  // http://www.opengl.org/registry/specs/NV/texture_env_combine4.txt
  {"GL_MESA_resize_buffers", 0},      // http://www.opengl.org/registry/specs/MESA/resize_buffers.txt
  {"GL_MESA_window_pos", 0},          // http://www.opengl.org/registry/specs/MESA/window_pos.txt
  {"GL_EXT_texture_compression_s3tc", 0}, // http://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt
  {"GL_IBM_cull_vertex", 0},          // http://www.opengl.org/registry/specs/IBM/cull_vertex.txt
  {"GL_IBM_multimode_draw_arrays", 0}, // #200 http://www.opengl.org/registry/specs/IBM/multimode_draw_arrays.txt
  {"GL_IBM_vertex_array_lists", 0},   // http://www.opengl.org/registry/specs/IBM/vertex_array_lists.txt
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"", 0}, // 
  {"GL_3DFX_texture_compression_FXT1", 0}, // http://www.opengl.org/registry/specs/3DFX/texture_compression_FXT1.txt
  {"GL_3DFX_multisample", 0},         // GLX_3DFX_multisample WGL_3DFX_multisample http://www.opengl.org/registry/specs/3DFX/3dfx_multisample.txt
  {"GL_3DFX_tbuffer", 0},             // http://www.opengl.org/registry/specs/3DFX/tbuffer.txt
  {"GL_EXT_multisample", 0},          // WGL_EXT_multisample http://www.opengl.org/registry/specs/EXT/wgl_multisample.txt
  {"GL_SGIX_vertex_preclip", 0},      // #210 GL_SGIX_vertex_preclip_hint http://www.opengl.org/registry/specs/SGIX/vertex_preclip.txt
  {"GL_SGIX_convolution_accuracy", 0}, // inc http://www.opengl.org/registry/specs/SGIX/convolution_accuracy.txt
  {"GL_SGIX_resample", 0},            // inc http://www.opengl.org/registry/specs/SGIX/resample.txt
  {"GL_SGIS_point_line_texgen", 0},   // http://www.opengl.org/registry/specs/SGIS/point_line_texgen.txt
  {"GL_SGIS_texture_color_mask", 0},  // http://www.opengl.org/registry/specs/SGIS/texture_color_mask.txt
  {"GLX_MESA_copy_sub_buffer", 0},    // http://www.opengl.org/registry/specs/MESA/copy_sub_buffer.txt
  {"GLX_MESA_pixmap_colormap", 0},    // http://www.opengl.org/registry/specs/MESA/pixmap_colormap.txt
  {"GLX_MESA_release_buffers", 0},    // http://www.opengl.org/registry/specs/MESA/release_buffers.txt
  {"GLX_MESA_set_3dfx_mode", 0},      // http://www.opengl.org/registry/specs/MESA/set_3dfx_mode.txt
  {"", 0}, // 
  {"GL_EXT_texture_env_dot3", 0},     // #220 http://www.opengl.org/registry/specs/EXT/texture_env_dot3.txt
  {"GL_ATI_texture_mirror_once", 0},  // http://www.opengl.org/registry/specs/ATI/texture_mirror_once.txt
  {"GL_NV_fence", 0},                 // http://www.opengl.org/registry/specs/NV/fence.txt
  {"GL_IBM_static_data", 0},          // http://www.opengl.org/registry/specs/IBM/static_data.txt
  {"GL_IBM_texture_mirrored_repeat", 0}, // http://www.opengl.org/registry/specs/IBM/texture_mirrored_repeat.txt
  {"GL_NV_evaluators", 0},            // http://www.opengl.org/registry/specs/NV/evaluators.txt
  {"GL_NV_packed_depth_stencil", 0},  // http://www.opengl.org/registry/specs/NV/packed_depth_stencil.txt
  {"GL_NV_register_combiners2", 0},   // http://www.opengl.org/registry/specs/NV/register_combiners2.txt
  {"GL_NV_texture_compression_vtc", 0}, // http://www.opengl.org/registry/specs/NV/texture_compression_vtc.txt
  {"GL_NV_texture_rectangle", 0},     // http://www.opengl.org/registry/specs/NV/texture_rectangle.txt
  {"GL_NV_texture_shader", 0},        // #230 http://www.opengl.org/registry/specs/NV/texture_shader.txt
  {"GL_NV_texture_shader2", 0},       // http://www.opengl.org/registry/specs/NV/texture_shader2.txt
  {"GL_NV_vertex_array_range2", 0},   // http://www.opengl.org/registry/specs/NV/vertex_array_range2.txt
  {"GL_NV_vertex_program", 0},        // http://www.opengl.org/registry/specs/NV/vertex_program.txt
  {"GLX_SGIX_visual_select_group", 0}, // http://www.opengl.org/registry/specs/SGIX/visual_select_group.txt
  {"GL_SGIX_texture_coordinate_clamp", 0}, // http://www.opengl.org/registry/specs/SGIX/texture_coordinate_clamp.txt
  {"", 0}, // 
  {"GLX_OML_swap_method", 0},         // http://www.opengl.org/registry/specs/OML/glx_swap_method.txt
  {"GLX_OML_sync_control", 0},        // http://www.opengl.org/registry/specs/OML/glx_sync_control.txt
  {"GL_OML_interlace", 0},            // http://www.opengl.org/registry/specs/OML/interlace.txt
  {"GL_OML_subsample", 0},            // #240 http://www.opengl.org/registry/specs/OML/subsample.txt
  {"GL_OML_resample", 0},             // http://www.opengl.org/registry/specs/OML/resample.txt
  {"WGL_OML_sync_control", 0},        // http://www.opengl.org/registry/specs/OML/wgl_sync_control.txt
  {"GL_NV_copy_depth_to_color", 0},   // http://www.opengl.org/registry/specs/NV/copy_depth_to_color.txt
  {"GL_ATI_envmap_bumpmap", 0},       // http://www.opengl.org/registry/specs/ATI/envmap_bumpmap.txt
  {"GL_ATI_fragment_shader", 0},      // http://www.opengl.org/registry/specs/ATI/fragment_shader.txt
  {"GL_ATI_pn_triangles", 0},         // http://www.opengl.org/registry/specs/ATI/pn_triangles.txt
  {"GL_ATI_vertex_array_object", 0},  // http://www.opengl.org/registry/specs/ATI/vertex_array_object.txt
  {"GL_EXT_vertex_shader", 0},        // http://www.opengl.org/registry/specs/EXT/vertex_shader.txt
  {"GL_ATI_vertex_streams", 0},       // http://www.opengl.org/registry/specs/ATI/vertex_streams.txt
  {"WGL_I3D_digital_video_control", 0}, // #250 http://www.opengl.org/registry/specs/I3D/wgl_digital_video_control.txt
  {"WGL_I3D_gamma", 0},               // http://www.opengl.org/registry/specs/I3D/wgl_gamma.txt
  {"WGL_I3D_genlock", 0},             // http://www.opengl.org/registry/specs/I3D/wgl_genlock.txt
  {"WGL_I3D_image_buffer", 0},        // http://www.opengl.org/registry/specs/I3D/wgl_image_buffer.txt
  {"WGL_I3D_swap_frame_lock", 0},     // http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_lock.txt
  {"WGL_I3D_swap_frame_usage", 0},    // http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_usage.txt
  {"GL_ATI_element_array", 0},        // http://www.opengl.org/registry/specs/ATI/element_array.txt
  {"GL_SUN_mesh_array", 0},           // http://www.opengl.org/registry/specs/SUN/mesh_array.txt
  {"GL_SUN_slice_accum", 0},          // http://www.opengl.org/registry/specs/SUN/slice_accum.txt
  {"GL_NV_multisample_filter_hint", 0}, // http://www.opengl.org/registry/specs/NV/multisample_filter_hint.txt
  {"GL_NV_depth_clamp", 0},           // #260 http://www.opengl.org/registry/specs/NV/depth_clamp.txt
  {"GL_NV_occlusion_query", 0},       // http://www.opengl.org/registry/specs/NV/occlusion_query.txt
  {"GL_NV_point_sprite", 0},          // http://www.opengl.org/registry/specs/NV/point_sprite.txt
  {"WGL_NV_render_depth_texture", 0}, // http://www.opengl.org/registry/specs/NV/render_depth_texture.txt
  {"WGL_NV_render_texture_rectangle", 0}, // http://www.opengl.org/registry/specs/NV/render_texture_rectangle.txt
  {"GL_NV_texture_shader3", 0},       // http://www.opengl.org/registry/specs/NV/texture_shader3.txt
  {"GL_NV_vertex_program1_1", 0},     // http://www.opengl.org/registry/specs/NV/texture_shader3.txt
  {"GL_EXT_shadow_funcs", 0},         // http://www.opengl.org/registry/specs/EXT/shadow_funcs.txt
  {"GL_EXT_stencil_two_side", 0},     // http://www.opengl.org/registry/specs/EXT/stencil_two_side.txt
  {"GL_ATI_text_fragment_shader", 0}, // http://www.opengl.org/registry/specs/ATI/text_fragment_shader.txt
  {"GL_APPLE_client_storage", 0},     // #270 http://www.opengl.org/registry/specs/APPLE/client_storage.txt
  {"GL_APPLE_element_array", 0},      // http://www.opengl.org/registry/specs/APPLE/element_array.txt
  {"GL_APPLE_fence", 0},              // http://www.opengl.org/registry/specs/APPLE/fence.txt
  {"GL_APPLE_vertex_array_object", 0}, // http://www.opengl.org/registry/specs/APPLE/vertex_array_object.txt
  {"GL_APPLE_vertex_array_range", 0}, // http://www.opengl.org/registry/specs/APPLE/vertex_array_range.txt
  {"GL_APPLE_ycbcr_422", 0},          // http://www.opengl.org/registry/specs/APPLE/ycbcr_422.txt
  {"GL_S3_s3tc", 0},                  // inc http://www.opengl.org/registry/specs/S3/s3tc.txt
  {"GL_ATI_draw_buffers", 0},         // http://www.opengl.org/registry/specs/ATI/draw_buffers.txt
  {"WGL_ATI_pixel_format_float", 0},  // http://www.opengl.org/registry/specs/ATI/pixel_format_float.txt
  {"GL_ATI_texture_env_combine3", 0}, // http://www.opengl.org/registry/specs/ATI/texture_env_combine3.txt
  {"GL_ATI_texture_float", 0},        // #280 http://www.opengl.org/registry/specs/ATI/texture_float.txt
  {"GL_NV_float_buffer", 0},          // WGL_NV_float_buffer GLX_NV_float_buffer http://www.opengl.org/registry/specs/NV/float_buffer.txt
  {"GL_NV_fragment_program", 0},      // http://www.opengl.org/registry/specs/NV/fragment_program.txt
  {"GL_NV_half_float", 0},            // http://www.opengl.org/registry/specs/NV/half_float.txt
  {"GL_NV_pixel_data_range", 0},      // http://www.opengl.org/registry/specs/NV/pixel_data_range.txt
  {"GL_NV_primitive_restart", 0},     // http://www.opengl.org/registry/specs/NV/primitive_restart.txt
  {"GL_NV_texture_expand_normal", 0}, // http://www.opengl.org/registry/specs/NV/texture_expand_normal.txt
  {"GL_NV_vertex_program2", 0},       // http://www.opengl.org/registry/specs/NV/vertex_program2.txt
  {"GL_ATI_map_object_buffer", 0},    // http://www.opengl.org/registry/specs/ATI/map_object_buffer.txt
  {"GL_ATI_separate_stencil", 0},     // http://www.opengl.org/registry/specs/ATI/separate_stencil.txt
  {"GL_ATI_vertex_attrib_array_object", 0}, // #290 http://www.opengl.org/registry/specs/ATI/vertex_attrib_array_object.txt
  {"GL_OES_byte_coordinates", 0},     // http://www.opengl.org/registry/specs/OES/OES_byte_coordinates.txt
  {"GL_OES_fixed_point", 0},          // http://www.opengl.org/registry/specs/OES/OES_fixed_point.txt
  {"GL_OES_single_precision", 0},     // http://www.opengl.org/registry/specs/OES/OES_single_precision.txt
  {"GL_OES_compressed_paletted_texture", 0}, // http://www.opengl.org/registry/specs/OES/OES_compressed_paletted_texture.txt
  {"GL_OES_read_format", 0},          // http://www.opengl.org/registry/specs/OES/OES_read_format.txt
  {"GL_OES_query_matrix", 0},         // http://www.opengl.org/registry/specs/OES/OES_query_matrix.txt
  {"GL_EXT_depth_bounds_test", 0},    // http://www.opengl.org/registry/specs/EXT/depth_bounds_test.txt
  {"GL_EXT_texture_mirror_clamp", 0}, // http://www.opengl.org/registry/specs/EXT/texture_mirror_clamp.txt
  {"GL_EXT_blend_equation_separate", 0}, // http://www.opengl.org/registry/specs/EXT/blend_equation_separate.txt
  {"GL_MESA_pack_invert", 0},         // #300 http://www.opengl.org/registry/specs/MESA/pack_invert.txt
  {"GL_MESA_ycbcr_texture", 0},       // http://www.opengl.org/registry/specs/MESA/ycbcr_texture.txt
  {"GL_EXT_pixel_buffer_object", 0},  // http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt
  {"GL_NV_fragment_program_option", 0}, // http://www.opengl.org/registry/specs/NV/fragment_program_option.txt
  {"GL_NV_fragment_program2", 0},     // http://www.opengl.org/registry/specs/NV/fragment_program2.txt
  {"GL_NV_vertex_program2_option", 0}, // http://www.opengl.org/registry/specs/NV/vertex_program2_option.txt
  {"GL_NV_vertex_program3", 0},       // http://www.opengl.org/registry/specs/NV/vertex_program3.txt
  {"GLX_SGIX_hyperpipe", 0},          // http://www.opengl.org/registry/specs/SGIX/hyperpipe_group.txt
  {"GLX_MESA_agp_offset", 0},         // http://www.opengl.org/registry/specs/MESA/agp_offset.txt
  {"GL_EXT_texture_compression_dxt1", 0}, // http://www.opengl.org/registry/specs/EXT/texture_compression_dxt1.txt
  {"GL_EXT_framebuffer_object", 0},   // #310 http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
  {"GL_GREMEDY_string_marker", 0},    // http://www.opengl.org/registry/specs/GREMEDY/string_marker.txt
  {"GL_EXT_packed_depth_stencil", 0}, // http://www.opengl.org/registry/specs/EXT/packed_depth_stencil.txt
  {"WGL_3DL_stereo_control", 0},      // http://www.opengl.org/registry/specs/3DL/stereo_control.txt
  {"GL_EXT_stencil_clear_tag", 0},    // http://www.opengl.org/registry/specs/EXT/stencil_clear_tag.txt
  {"GL_EXT_texture_sRGB", 0},         // http://www.opengl.org/registry/specs/EXT/texture_sRGB.txt
  {"GL_EXT_framebuffer_blit", 0},     // http://www.opengl.org/registry/specs/EXT/framebuffer_blit.txt
  {"GL_EXT_framebuffer_multisample", 0}, // http://www.opengl.org/registry/specs/EXT/framebuffer_multisample.txt
  {"GL_MESAX_texture_stack", 0},      // http://www.opengl.org/registry/specs/MESAX/texture_stack.txt
  {"GL_EXT_timer_query", 0},          // http://www.opengl.org/registry/specs/EXT/timer_query.txt
  {"GL_EXT_gpu_program_parameters", 0}, // #320 http://www.opengl.org/registry/specs/EXT/gpu_program_parameters.txt
  {"GL_APPLE_flush_buffer_range", 0}, // http://www.opengl.org/registry/specs/APPLE/flush_buffer_range.txt
  {"GL_NV_gpu_program4", 0},          // http://www.opengl.org/registry/specs/NV/gpu_program4.txt
  {"GL_NV_geometry_program4", 0},     // http://www.opengl.org/registry/specs/NV/geometry_program4.txt
  {"GL_EXT_geometry_shader4", 0},     // http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
  {"GL_NV_vertex_program4", 0},       // http://www.opengl.org/registry/specs/NV/vertex_program4.txt
  {"GL_EXT_gpu_shader4", 0},          // http://www.opengl.org/registry/specs/EXT/gpu_shader4.txt
  {"GL_EXT_draw_instanced", 0},       // http://www.opengl.org/registry/specs/EXT/draw_instanced.txt
  {"GL_EXT_packed_float", 0},         // WGL_EXT_pixel_format_packed_float GLX_EXT_fbconfig_packed_float http://www.opengl.org/registry/specs/EXT/packed_float.txt
  {"GL_EXT_texture_array", 0},        // http://www.opengl.org/registry/specs/EXT/texture_array.txt
  {"GL_EXT_texture_buffer_object", 0}, // #330 http://www.opengl.org/registry/specs/EXT/texture_buffer_object.txt
  {"GL_EXT_texture_compression_latc", 0}, // GL_NV_texture_compression_latc (legacy) http://www.opengl.org/registry/specs/EXT/texture_compression_latc.txt
  {"GL_EXT_texture_compression_rgtc", 0}, // http://www.opengl.org/registry/specs/EXT/texture_compression_rgtc.txt
  {"GL_EXT_texture_shared_exponent", 0}, // http://www.opengl.org/registry/specs/EXT/texture_shared_exponent.txt
  {"GL_NV_depth_buffer_float", 0},    // http://www.opengl.org/registry/specs/NV/depth_buffer_float.txt
  {"GL_NV_fragment_program4", 0},     // http://www.opengl.org/registry/specs/NV/fragment_program4.txt
  {"GL_NV_framebuffer_multisample_coverage", 0}, // http://www.opengl.org/registry/specs/NV/framebuffer_multisample_coverage.txt
  {"GL_EXT_framebuffer_sRGB", 0},     // GLX_EXT_framebuffer_sRGB WGL_EXT_framebuffer_sRGB http://www.opengl.org/registry/specs/EXT/framebuffer_sRGB.txt
  {"GL_NV_geometry_shader4", 0},      // http://www.opengl.org/registry/specs/NV/geometry_shader4.txt
  {"GL_NV_parameter_buffer_object", 0}, // http://www.opengl.org/registry/specs/NV/parameter_buffer_object.txt
  {"GL_EXT_draw_buffers2", 0},        // #340 http://www.opengl.org/registry/specs/EXT/draw_buffers2.txt
  {"GL_NV_transform_feedback", 0},    // http://www.opengl.org/registry/specs/NV/transform_feedback.txt
  {"GL_EXT_bindable_uniform", 0},     // http://www.opengl.org/registry/specs/EXT/bindable_uniform.txt
  {"GL_EXT_texture_integer", 0},      // http://www.opengl.org/registry/specs/EXT/texture_integer.txt
  {"GLX_EXT_texture_from_pixmap", 0}, // http://www.opengl.org/registry/specs/EXT/texture_from_pixmap.txt
  {"GL_GREMEDY_frame_terminator", 0}, // http://www.opengl.org/registry/specs/GREMEDY/frame_terminator.txt
  {"GL_NV_conditional_render", 0},    // http://www.opengl.org/registry/specs/NV/conditional_render.txt
  {"GL_NV_present_video", 0},         // GLX_NV_present_video WGL_NV_present_video http://www.opengl.org/registry/specs/NV/present_video.txt
  {"GLX_NV_video_out", 0},            // http://www.opengl.org/registry/specs/NV/glx_video_output.txt
  {"WGL_NV_video_output", 0},         // http://www.opengl.org/registry/specs/NV/wgl_video_output.txt
  {"GLX_NV_swap_group", 0},           // #350 http://www.opengl.org/registry/specs/NV/glx_swap_group.txt
  {"WGL_NV_swap_group", 0},           // http://www.opengl.org/registry/specs/NV/wgl_swap_group.txt
  {"GL_EXT_transform_feedback", 0},   // http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
  {"GL_EXT_direct_state_access", 0},  // http://www.opengl.org/registry/specs/EXT/direct_state_access.txt
  {"GL_EXT_vertex_array_bgra", 0},    // http://www.opengl.org/registry/specs/EXT/vertex_array_bgra.txt
  {"WGL_NV_gpu_affinity", 0},         // http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
  {"GL_EXT_texture_swizzle", 0},      // http://www.opengl.org/registry/specs/EXT/texture_swizzle.txt
  {"GL_NV_explicit_multisample", 0},  // http://www.opengl.org/registry/specs/NV/explicit_multisample.txt
  {"GL_NV_transform_feedback2", 0},   // http://www.opengl.org/registry/specs/NV/transform_feedback2.txt
  {"GL_ATI_meminfo", 0},              // http://www.opengl.org/registry/specs/ATI/meminfo.txt
  {"GL_AMD_performance_monitor", 0},  // #360 http://www.opengl.org/registry/specs/AMD/performance_monitor.txt
  {"WGL_AMD_gpu_association", 0},     // http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
  {"GL_AMD_texture_texture4", 0},     // http://www.opengl.org/registry/specs/AMD/texture_texture4.txt
  {"GL_AMD_vertex_shader_tessellator", 0}, // http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt
  {"GL_EXT_provoking_vertex", 0},     // http://www.opengl.org/registry/specs/EXT/provoking_vertex.txt
  {"GL_EXT_texture_snorm", 0},        // http://www.opengl.org/registry/specs/EXT/texture_snorm.txt
  {"GL_AMD_draw_buffers_blend", 0},   // http://www.opengl.org/registry/specs/AMD/draw_buffers_blend.txt
  {"GL_APPLE_texture_range", 0},      // http://www.opengl.org/registry/specs/APPLE/texture_range.txt
  {"GL_APPLE_float_pixels", 0},       // http://www.opengl.org/registry/specs/APPLE/float_pixels.txt
  {"GL_APPLE_vertex_program_evaluators", 0}, // http://www.opengl.org/registry/specs/APPLE/vertex_program_evaluators.txt
  {"GL_APPLE_aux_depth_stencil", 0},  // #370 http://www.opengl.org/registry/specs/APPLE/aux_depth_stencil.txt
  {"GL_APPLE_object_purgeable", 0},   // http://www.opengl.org/registry/specs/APPLE/object_purgeable.txt
  {"GL_APPLE_row_bytes", 0},          // http://www.opengl.org/registry/specs/APPLE/row_bytes.txt
  {"GL_APPLE_rgb_422", 0},            // http://www.opengl.org/registry/specs/APPLE/rgb_422.txt
  {"GL_NV_video_capture", 0},         // GLX_NV_video_capture WGL_NV_video_capture http://www.opengl.org/registry/specs/NV/video_capture.txt
  {"GLX_EXT_swap_control", 0},        // http://www.opengl.org/registry/specs/EXT/swap_control.txt
  {"GL_NV_copy_image", 0},            // WGL_NV_copy_image GLX_NV_copy_image http://www.opengl.org/registry/specs/NV/copy_image.txt
  {"GL_EXT_separate_shader_objects", 0}, // http://www.opengl.org/registry/specs/EXT/separate_shader_objects.txt
  {"GL_NV_parameter_buffer_object2", 0}, // http://www.opengl.org/registry/specs/NV/parameter_buffer_object2.txt
  {"GL_NV_shader_buffer_load", 0},    // http://www.opengl.org/registry/specs/NV/shader_buffer_load.txt
  {"GL_NV_vertex_buffer_unified_memory", 0}, // #380 http://www.opengl.org/registry/specs/NV/vertex_buffer_unified_memory.txt
  {"GL_NV_texture_barrier", 0},       // http://www.opengl.org/registry/specs/NV/texture_barrier.txt
  {"GL_AMD_shader_stencil_export", 0}, // http://www.opengl.org/registry/specs/AMD/shader_stencil_export.txt
  {"GL_AMD_seamless_cubemap_per_texture", 0}, // http://www.opengl.org/registry/specs/AMD/seamless_cubemap_per_texture.txt
  {"GLX_INTEL_swap_event", 0},        // http://www.opengl.org/registry/specs/INTEL/swap_event.txt
  {"GL_AMD_conservative_depth", 0},   // http://www.opengl.org/registry/specs/AMD/conservative_depth.txt
  {"GL_EXT_shader_image_load_store", 0}, // http://www.opengl.org/registry/specs/EXT/shader_image_load_store.txt
  {"GL_EXT_vertex_attrib_64bit", 0},  // http://www.opengl.org/registry/specs/EXT/vertex_attrib_64bit.txt
  {"GL_NV_gpu_program5", 0},          // GL_NV_gpu_program_fp64 http://www.opengl.org/registry/specs/NV/gpu_program5.txt
  {"GL_NV_gpu_shader5", 0},           // http://www.opengl.org/registry/specs/NV/gpu_shader5.txt
  {"GL_NV_shader_buffer_store", 0},   // #390 http://www.opengl.org/registry/specs/NV/shader_buffer_store.txt
  {"GL_NV_tessellation_program5", 0}, // http://www.opengl.org/registry/specs/NV/tessellation_program5.txt
  {"GL_NV_vertex_attrib_integer_64bit", 0}, // http://www.opengl.org/registry/specs/NV/vertex_attrib_integer_64bit.txt
  {"GL_NV_multisample_coverage", 0},  // GLX_NV_multisample_coverage WGL_NV_multisample_coverage http://www.opengl.org/registry/specs/NV/multisample_coverage.txt
  {"GL_AMD_name_gen_delete", 0},      // http://www.opengl.org/registry/specs/AMD/name_gen_delete.txt
  {"GL_AMD_debug_output", 0},         // http://www.opengl.org/registry/specs/AMD/debug_output.txt
  {"GL_NV_vdpau_interop", 0},         // http://www.opengl.org/registry/specs/NV/vdpau_interop.txt
  {"GL_AMD_transform_feedback3_lines_triangles", 0}, // http://www.opengl.org/registry/specs/AMD/transform_feedback3_lines_triangles.txt
  {"GLX_AMD_gpu_association", 0},     // http://www.opengl.org/registry/specs/AMD/glx_gpu_association.txt
  {"GLX_EXT_create_context_es_profile", 0}, // GLX_EXT_create_context_es2_profile http://www.opengl.org/registry/specs/EXT/glx_create_context_es2_profile.txt
  {"WGL_EXT_create_context_es_profile", 0}, // #400 WGL_EXT_create_context_es2_profile http://www.opengl.org/registry/specs/EXT/wgl_create_context_es2_profile.txt
  {"GL_AMD_depth_clamp_separate", 0}, // http://www.opengl.org/registry/specs/AMD/depth_clamp_separate.txt
  {"GL_EXT_texture_sRGB_decode", 0},  // http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
  {"GL_NV_texture_multisample", 0},   // GL_ missing in registry, asuming mistake http://www.opengl.org/registry/specs/NV/texture_multisample.txt
  {"GL_AMD_blend_minmax_factor", 0},  // http://www.opengl.org/registry/specs/AMD/blend_minmax_factor.txt
  {"GL_AMD_sample_positions", 0},     // http://www.opengl.org/registry/specs/AMD/sample_positions.txt
  {"GL_EXT_x11_sync_object", 0},      // http://www.opengl.org/registry/specs/EXT/x11_sync_object.txt
  {"WGL_NV_DX_interop", 0},           // http://www.opengl.org/registry/specs/NV/DX_interop.txt
  {"GL_AMD_multi_draw_indirect", 0},  // http://www.opengl.org/registry/specs/AMD/multi_draw_indirect.txt
  {"GL_EXT_framebuffer_multisample_blit_scaled", 0}, // http://www.opengl.org/registry/specs/EXT/framebuffer_multisample_blit_scaled.txt
  {"GL_NV_path_rendering", 0},        // #410 http://www.opengl.org/registry/specs/NV/path_rendering.txt
  {"GL_AMD_pinned_memory", 0},        // http://www.opengl.org/registry/specs/AMD/pinned_memory.txt
  {"WGL_NV_DX_interop2", 0},          // http://www.opengl.org/registry/specs/NV/DX_interop2.txt
  {"GL_AMD_stencil_operation_extended", 0}, // http://www.opengl.org/registry/specs/AMD/stencil_operation_extended.txt
  {"GLX_EXT_swap_control_tear", 0},   // http://www.opengl.org/registry/specs/EXT/glx_swap_control_tear.txt
  {"WGL_EXT_swap_control_tear", 0},   // http://www.opengl.org/registry/specs/EXT/wgl_swap_control_tear.txt
  {"GL_AMD_vertex_shader_viewport_index", 0}, // http://www.opengl.org/registry/specs/AMD/vertex_shader_viewport_index.txt
  {"GL_AMD_vertex_shader_layer", 0},  // http://www.opengl.org/registry/specs/AMD/vertex_shader_layer.txt
  {"GL_NV_bindless_texture", 0},      // http://www.opengl.org/registry/specs/NV/bindless_texture.txt
  {"GL_NV_shader_atomic_float", 0},   // http://www.opengl.org/registry/specs/NV/shader_atomic_float.txt
  {"GL_AMD_query_buffer_object", 0},  // #420 http://www.opengl.org/registry/specs/AMD/query_buffer_object.txt
  {"GL_NV_compute_program5", 0},      // http://www.opengl.org/registry/specs/NV/compute_program5.txt
  {"GL_NV_shader_storage_buffer_object", 0}, // http://www.opengl.org/registry/specs/NV/shader_storage_buffer_object.txt
  {"GL_NV_shader_atomic_counters", 0}, // http://www.opengl.org/registry/specs/NV/shader_atomic_counters.txt
  {"GL_NV_deep_texture3D", 0},        // http://www.opengl.org/registry/specs/NV/deep_texture3D.txt
  {"GL_NVX_conditional_render", 0},   // http://www.opengl.org/registry/specs/NVX/nvx_conditional_render.txt
  {"GL_AMD_sparse_texture", 0},       // http://www.opengl.org/registry/specs/AMD/sparse_texture.txt
  {"GLX_EXT_buffer_age", 0},          // http://www.opengl.org/registry/specs/EXT/glx_buffer_age.txt
  {"GL_AMD_shader_trinary_minmax", 0}, // http://www.opengl.org/registry/specs/AMD/shader_trinary_minmax.txt
  {"GL_INTEL_map_texture", 0},        // http://www.opengl.org/registry/specs/INTEL/map_texture.txt
  {"GL_NV_draw_texture", 0},          // #430 http://www.opengl.org/registry/specs/NV/draw_texture.txt
  {"GL_AMD_interleaved_elements", 0}, // http://www.opengl.org/registry/specs/AMD/interleaved_elements.txt
  {"GL_NV_bindless_multi_draw_indirect", 0}, // http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect.txt
  {"GL_NV_blend_equation_advanced", 0}, // GL_NV_blend_equation_advanced_coherent http://www.opengl.org/registry/specs/NV/blend_equation_advanced.txt
  {"GL_NV_gpu_program5_mem_extended", 0}, // http://www.opengl.org/registry/specs/NV/gpu_program5_mem_extended.txt
  {"GL_AMD_shader_atomic_counter_ops", 0}, // http://www.opengl.org/registry/specs/AMD/shader_atomic_counter_ops.txt
  {"WGL_NV_delay_before_swap", 0},    // http://www.opengl.org/registry/specs/NV/wgl_delay_before_swap.txt
  {"GL_EXT_shader_integer_mix", 0},   // http://www.opengl.org/registry/specs/EXT/shader_integer_mix.txt
  {"GL_NVX_gpu_memory_info", 0},      // http://www.opengl.org/registry/specs/NVX/gpu_memory_info.txt
  {"GL_EXT_debug_label", 0},          // http://www.opengl.org/registry/specs/EXT/EXT_debug_label.txt
  {"GL_EXT_debug_marker", 0},         // #440 http://www.opengl.org/registry/specs/EXT/EXT_debug_marker.txt
  {"GL_INTEL_fragment_shader_ordering", 0}, // http://www.opengl.org/registry/specs/INTEL/fragment_shader_ordering.txt
  {"GL_AMD_occlusion_query_event", 0}, // http://www.opengl.org/registry/specs/AMD/occlusion_query_event.txt
  {"GL_INTEL_performance_query", 0},  // http://www.opengl.org/registry/specs/INTEL/performance_query.txt
  {"GL_AMD_shader_stencil_value_export", 0}, // http://www.opengl.org/registry/specs/AMD/shader_stencil_value_export.txt
  {"GLX_NV_delay_before_swap", 0},    // http://www.opengl.org/registry/specs/NV/glx_delay_before_swap.txt
  {"GLX_MESA_query_renderer", 0},     // http://www.opengl.org/registry/specs/MESA/glx_query_renderer.txt
  {"GL_NV_shader_thread_group", 0},   // http://www.opengl.org/registry/specs/NV/shader_thread_group.txt
  {"GL_NV_shader_thread_shuffle", 0}, // http://www.opengl.org/registry/specs/NV/shader_thread_shuffle.txt
  {"GL_EXT_shader_image_load_formatted", 0}, // http://www.opengl.org/registry/specs/EXT/shader_image_load_formatted.txt
  {"GL_AMD_transform_feedback4", 0},  // #450 http://www.opengl.org/registry/specs/AMD/transform_feedback4.txt
  {"GL_AMD_gpu_shader_int64", 0},     // http://www.opengl.org/registry/specs/AMD/gpu_shader_int64.txt
  {"GLX_EXT_stereo_tree", 0},         // http://www.opengl.org/registry/specs/EXT/glx_stereo_tree.txt
  {"GL_AMD_gcn_shader", 0},           // http://www.opengl.org/registry/specs/AMD/gcn_shader.txt
  {"", 0}, // 
  {"GL_NV_shader_atomic_int64", 0},   // http://www.opengl.org/registry/specs/NV/shader_atomic_int64.txt
  {"GL_NV_bindless_multi_draw_indirect_count", 0}, // http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect_count.txt
  {"GLX_NV_copy_buffer", 0},          // http://www.opengl.org/registry/specs/NV/glx_copy_buffer.txt
  {"GL_KHR_blend_equation_advanced", 0}, // #458 GL_KHR_blend_equation_advanced_coherent http://www.opengl.org/registry/specs/KHR/blend_equation_advanced.txt
  {null, 0} // terminator
}; /// till #458








