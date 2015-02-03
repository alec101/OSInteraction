#include "osinteraction.h"
#include "util/typeShortcuts.h"
/* MIGHT TRY TO MAKE THIS NOT NECESARY A PART OF OSI
#include "glext.h"

#ifdef OS_WIN
#include "wglext.h"
#endif

#ifdef OS_LINUX
#include "glxext.h"
#endif


#include "osiRenderer.h"
#include "osiGlExt.h"
*/

/// one single struct that holds all the gl extensions, for LINUX and MAC
/// under windows, each renderer must have it's own functions

#ifndef OS_WIN
GlExtFuncs glExt;
#endif /// OS_MAC or OS_LINUX



bool getGlProc(cchar *name, void **addr) {
  bool chatty= true;
  if(!addr) return false;
  
  #ifdef OS_WIN
  *addr= (void *)wglGetProcAddress(name);

  // wgl can fail for old funcs, and GetProcAddress must be used instead https://www.opengl.org/wiki/Load_OpenGL_Functions
  if(*addr== 0 ||
    (*addr== (void*)0x1) || (*addr== (void*)0x2) || (&addr== (void*)0x3) ||
    (*addr== (void*)-1) )
  {
    HMODULE module= LoadLibraryA("opengl32.dll");
    *addr= (void *)GetProcAddress(module, name);
  }
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  *addr= (void *)glXGetProcAddressARB((const GLubyte *)name);
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  //needed?
  #endif /// OS_MAC
    
  return (*addr? true: false);
}

#define GETGLPROCMACRO(_x_x) getGlProc(#_x_x,  (void **)&r->glExt. _x_x  );




///=============================///
// OpenGL version funcs aquiring //
///=============================///

void getVERfuncs(osiRenderer *r, int major, int minor) {
  //getGlProc("glTexImage3DEXT", r->glExt.glTexImage3D);
  
  #ifndef OS_MAC
  
  #ifdef OS_LINUX
  // linux GLX versions //    SCRAPE INCOMING
  ///==================///
  /*
  /// GLX_VERSION_1_3
  GETGLPROCMACRO(glXGetFBConfigs)   //getGlProc("glXGetFBConfigs", r->glExt.glXGetFBConfigs); // << what should look like
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
  */

  /// GLX_VERSION_1_4
  //GETGLPROCMACRO(glXGetProcAddress)
  #endif /// OS_LINUX

  if(major< 1)
    return;

  /// OpenGL 1.2 funcs =================------------------------------
  if(major== 1 && minor< 2)
    return;
  #ifdef GL_VERSION_1_2
  GETGLPROCMACRO(glDrawRangeElements) // getGlProc("glDrawRangeElements", r->glExt.glDrawRangeElements); // <<< what should look like
  GETGLPROCMACRO(glTexImage3D)
  GETGLPROCMACRO(glTexSubImage3D)
  GETGLPROCMACRO(glCopyTexSubImage3D)
  #endif

  /// OpenGL 1.3 funcs =================------------------------------
  if(major== 1 && minor< 3)
    return;
  #ifdef GL_VERSION_1_3
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
  #endif

  /// OpenGL 1.4 funcs =================------------------------------
  if(major== 1 && minor< 4)
    return;
  #ifdef GL_VERSION_1_4
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
  #endif

  /// OpenGL 1.5 funcs =================------------------------------
  if(major== 1 && minor< 5)
    return;
  #ifdef GL_VERSION_1_5
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
  #endif

  /// OpenGL 2.0 funcs =================------------------------------
  if(major< 2)
    return;
  #ifdef GL_VERSION_2_0
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
  #endif

  /// OpenGL 2.1 funcs =================------------------------------
  if(major== 2 && minor< 1)
    return;
  #ifdef GL_VERSION_2_1
  GETGLPROCMACRO(glUniformMatrix2x3fv)
  GETGLPROCMACRO(glUniformMatrix3x2fv)
  GETGLPROCMACRO(glUniformMatrix2x4fv)
  GETGLPROCMACRO(glUniformMatrix4x2fv)
  GETGLPROCMACRO(glUniformMatrix3x4fv)
  GETGLPROCMACRO(glUniformMatrix4x3fv)
  #endif

  /// OpenGL 3.0 funcs =================------------------------------
  if(major< 3)
    return;
  #ifdef GL_VERSION_3_0
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
  #endif

  /// OpenGL 3.1 funcs =================------------------------------
  if(major== 3 && minor< 1)
    return;
  #ifdef GL_VERSION_3_1
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
  #endif

  /// OpenGL 3.2 funcs =================------------------------------
  if(major== 3 && minor< 2)
    return;
  #ifdef GL_VERSION_3_2
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
  #endif

  /// OpenGL 3.3 funcs =================------------------------------
  if(major== 3 && minor< 3)
    return;
  #ifdef GL_VERSION_3_3
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
  #endif

  /// OpenGL 4.0 funcs =================------------------------------
  if(major< 4)
    return;
  #ifdef GL_VERSION_4_0
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
  #endif

  /// OpenGL 4.1 funcs =================------------------------------
  if(major== 4 && minor< 1)
    return;
  #ifdef GL_VERSION_4_1
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
  #endif

  /// OpenGL 4.2 funcs =================------------------------------
  if(major== 4 && minor< 2)
    return;
  #ifdef GL_VERSION_4_2
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
  #endif

  /// OpenGL 4.3 funcs =================------------------------------
  if(major== 4 && minor< 3)
    return;
  #ifdef GL_VERSION_4_3
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
  #endif

  /// OpenGL 4.4 funcs =================------------------------------
  if(major== 4 && minor< 4)
    return;
  #ifdef GL_VERSION_4_4
  GETGLPROCMACRO(glBufferStorage)
  GETGLPROCMACRO(glClearTexImage)
  GETGLPROCMACRO(glClearTexSubImage)
  GETGLPROCMACRO(glBindBuffersBase)
  GETGLPROCMACRO(glBindBuffersRange)
  GETGLPROCMACRO(glBindTextures)
  GETGLPROCMACRO(glBindSamplers)
  GETGLPROCMACRO(glBindImageTextures)
  GETGLPROCMACRO(glBindVertexBuffers)
  #endif

  #endif /// OS_MAC ignore
}



///=================================///
// extensions not in ARB or EXT list //
///=================================///
void getOTHERfuncs(osiRenderer *r) {
  #ifndef OS_MAC
  #ifdef OS_WIN
  #ifdef GL_ARB_imaging
  if(r->glOTHERlist[0].avaible) {
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
  #endif
  #endif /// OS_WIN
  #ifdef GL_ARB_bindless_texture
  if(r->glOTHERlist[1].avaible) {    /// 
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
  #endif
  #ifdef GL_INGR_blend_func_separate    ///  ?
  if(r->glOTHERlist[2].avaible) {
    GETGLPROCMACRO(glBlendFuncSeparateINGR)
  }
  #endif
  #ifdef GL_SGIX_fragment_lighting    ///  ?
  if(r->glOTHERlist[3].avaible) {
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
  #endif
  #ifdef GL_SGIX_polynomial_ffd
  if(r->glOTHERlist[4].avaible) {    /// 
    GETGLPROCMACRO(glDeformationMap3dSGIX)
    GETGLPROCMACRO(glDeformationMap3fSGIX)
    GETGLPROCMACRO(glDeformSGIX)
    GETGLPROCMACRO(glLoadIdentityDeformationMapSGIX)
  }
  #endif
  #ifdef GL_SGIS_point_parameters
  if(r->glOTHERlist[5].avaible) {    /// 
    GETGLPROCMACRO(glPointParameterfSGIS)
    GETGLPROCMACRO(glPointParameterfvSGIS)
  }
  #endif
  #ifdef GL_SGIX_igloo_interface
  if(r->glOTHERlist[6].avaible) {    /// 
    GETGLPROCMACRO(glIglooInterfaceSGIX)

  }
  #endif
  #ifdef WGL_NV_vertex_array_range
  #ifdef OS_WIN
  if(r->glOTHERlist[7].avaible) {    /// 
    GETGLPROCMACRO(wglAllocateMemoryNV)
    GETGLPROCMACRO(wglFreeMemoryNV)
  }
  #endif

  #endif /// OS_WIN
  #endif /// OS_MAC does nothing
}



///=============================///
// ARB extensions funcs aquiring //
///=============================///
void getARBfuncs(osiRenderer *r) {
  #ifndef OS_MAC
  #ifdef GL_ARB_multitexture
  if(r->glARBlist[0].avaible) {             /// #1   http://www.opengl.org/registry/specs/ARB/multitexture.txt
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
  #endif
  #ifdef GLX_ARB_get_proc_address
  //if(r->glARBlist[1].avaible)       /// #2   http://www.opengl.org/registry/specs/ARB/get_proc_address.txt
    //GETGLPROCMACRO(glXGetProcAddressARB)
  #endif
  #ifdef GL_ARB_transpose_matrix
  if(r->glARBlist[2].avaible) {     /// #3   http://www.opengl.org/registry/specs/ARB/transpose_matrix.txt
    GETGLPROCMACRO(glLoadTransposeMatrixfARB)
    GETGLPROCMACRO(glLoadTransposeMatrixdARB)
    GETGLPROCMACRO(glMultTransposeMatrixfARB)
    GETGLPROCMACRO(glMultTransposeMatrixdARB)
  }
  #endif
  #ifdef WGL_ARB_buffer_region
  if(r->glARBlist[3].avaible) {     /// #4   http://www.opengl.org/registry/specs/ARB/wgl_buffer_region.txt
    GETGLPROCMACRO(wglCreateBufferRegionARB)
    GETGLPROCMACRO(wglDeleteBufferRegionARB)
    GETGLPROCMACRO(wglSaveBufferRegionARB)
    GETGLPROCMACRO(wglRestoreBufferRegionARB)
  }
  #endif
  #ifdef GL_ARB_multisample
  if(r->glARBlist[4].avaible)         /// #5  GLX_ARB_multisample WGL_ARB_multisample  http://www.opengl.org/registry/specs/ARB/multisample.txt
    GETGLPROCMACRO(glSampleCoverageARB)
  #endif
  #ifdef WGL_ARB_extensions_string
  if(r->glARBlist[7].avaible)         /// #8  http://www.opengl.org/registry/specs/ARB/wgl_extensions_string.txt
    GETGLPROCMACRO(wglGetExtensionsStringARB)
  #endif
  #ifdef WGL_ARB_pixel_format
  if(r->glARBlist[8].avaible) {       /// #9  http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
    GETGLPROCMACRO(wglGetPixelFormatAttribivARB)
    GETGLPROCMACRO(wglGetPixelFormatAttribfvARB)
    GETGLPROCMACRO(wglChoosePixelFormatARB)
  }
  #endif
  #ifdef WGL_ARB_make_current_read
  if(r->glARBlist[9].avaible) {       /// #10  http://www.opengl.org/registry/specs/ARB/wgl_make_current_read.txt
    GETGLPROCMACRO(wglMakeContextCurrentARB)
    GETGLPROCMACRO(wglGetCurrentReadDCARB)
  }
  #endif
  #ifdef WGL_ARB_pbuffer
  if(r->glARBlist[10].avaible) {      /// #11  http://www.opengl.org/registry/specs/ARB/wgl_pbuffer.txt
    GETGLPROCMACRO(wglCreatePbufferARB)
    GETGLPROCMACRO(wglGetPbufferDCARB)
    GETGLPROCMACRO(wglReleasePbufferDCARB)
    GETGLPROCMACRO(wglDestroyPbufferARB)
    GETGLPROCMACRO(wglQueryPbufferARB)
  }
  #endif
  #ifdef GL_ARB_texture_compression
  if(r->glARBlist[11].avaible) {      /// #12  http://www.opengl.org/registry/specs/ARB/texture_compression.txt
    GETGLPROCMACRO(glCompressedTexImage3DARB)
    GETGLPROCMACRO(glCompressedTexImage2DARB)
    GETGLPROCMACRO(glCompressedTexImage1DARB)
    GETGLPROCMACRO(glCompressedTexSubImage3DARB)
    GETGLPROCMACRO(glCompressedTexSubImage2DARB)
    GETGLPROCMACRO(glCompressedTexSubImage1DARB)
    GETGLPROCMACRO(glGetCompressedTexImageARB)
  }
  #endif
  #ifdef GL_ARB_point_parameters
  if(r->glARBlist[13].avaible) {      /// #14  http://www.opengl.org/registry/specs/ARB/point_parameters.txt
    GETGLPROCMACRO(glPointParameterfARB)
    GETGLPROCMACRO(glPointParameterfvARB)
  }
  #endif
  #ifdef GL_ARB_vertex_blend
  if(r->glARBlist[14].avaible) {      /// #15  http://www.opengl.org/registry/specs/ARB/vertex_blend.txt
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
  #endif
  #ifdef GL_ARB_matrix_palette
  if(r->glARBlist[15].avaible) {      /// #16  http://www.opengl.org/registry/specs/ARB/matrix_palette.txt
    GETGLPROCMACRO(glCurrentPaletteMatrixARB)
    GETGLPROCMACRO(glMatrixIndexubvARB)
    GETGLPROCMACRO(glMatrixIndexusvARB)
    GETGLPROCMACRO(glMatrixIndexuivARB)
    GETGLPROCMACRO(glMatrixIndexPointerARB)
  }
  #endif
  #ifdef WGL_ARB_render_texture
  if(r->glARBlist[19].avaible) {      /// #20  http://www.opengl.org/registry/specs/ARB/wgl_render_texture.txt
    GETGLPROCMACRO(wglBindTexImageARB)
    GETGLPROCMACRO(wglReleaseTexImageARB)
    GETGLPROCMACRO(wglSetPbufferAttribARB)
  }
  #endif
  #ifdef GL_ARB_window_pos
  if(r->glARBlist[24].avaible) {      /// #25  http://www.opengl.org/registry/specs/ARB/window_pos.txt
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
  #endif
  #ifdef GL_ARB_vertex_program
  if(r->glARBlist[25].avaible) {      /// #26  http://www.opengl.org/registry/specs/ARB/vertex_program.txt
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
  #endif
  #ifdef GL_ARB_fragment_program
  if(r->glARBlist[26].avaible) {      /// #27  http://www.opengl.org/registry/specs/ARB/fragment_program.txt
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
  #endif
  #ifdef GL_ARB_vertex_buffer_object
  if(r->glARBlist[27].avaible) {      /// #28  GLX_ARB_vertex_buffer_object http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
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
  #endif
  #ifdef GL_ARB_occlusion_query
  if(r->glARBlist[28].avaible) {      /// #29  http://www.opengl.org/registry/specs/ARB/occlusion_query.txt
    GETGLPROCMACRO(glGenQueriesARB)
    GETGLPROCMACRO(glDeleteQueriesARB)
    GETGLPROCMACRO(glIsQueryARB)
    GETGLPROCMACRO(glBeginQueryARB)
    GETGLPROCMACRO(glEndQueryARB)
    GETGLPROCMACRO(glGetQueryivARB)
    GETGLPROCMACRO(glGetQueryObjectivARB)
    GETGLPROCMACRO(glGetQueryObjectuivARB)
  }
  #endif
  #ifdef GL_ARB_shader_objects
  if(r->glARBlist[29].avaible) {      /// #30  http://www.opengl.org/registry/specs/ARB/shader_objects.txt
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
  #endif
  #ifdef GL_ARB_vertex_shader
  if(r->glARBlist[30].avaible){       /// #31  http://www.opengl.org/registry/specs/ARB/vertex_shader.txt
    GETGLPROCMACRO(glBindAttribLocationARB) // doc includes GL_ARB_vertex_program funcs...
    GETGLPROCMACRO(glGetActiveAttribARB)
    GETGLPROCMACRO(glGetAttribLocationARB)
  }
  #endif
  #ifdef GL_ARB_draw_buffers
  if(r->glARBlist[36].avaible)        /// #37  http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
    GETGLPROCMACRO(glDrawBuffersARB)
  #endif
  #ifdef GL_ARB_color_buffer_float
  if(r->glARBlist[38].avaible) {      /// #39  WGL_ARB_pixel_format_float GLX_ARB_fbconfig_float http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
    GETGLPROCMACRO(glClampColorARB)
    #ifdef OS_WIN
    GETGLPROCMACRO(wglGetPixelFormatAttribivARB)
    GETGLPROCMACRO(wglGetPixelFormatAttribfvARB)
    GETGLPROCMACRO(wglChoosePixelFormatARB)
    #endif /// OS_WIN
  }
  #endif
  #ifdef GL_ARB_draw_instanced
  if(r->glARBlist[43].avaible) {      /// #44  http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
    GETGLPROCMACRO(glDrawArraysInstancedARB)
    GETGLPROCMACRO(glDrawElementsInstancedARB)
  }
  #endif
  #ifdef GL_ARB_geometry_shader4
  if(r->glARBlist[46].avaible) {      /// #47  http://www.opengl.org/registry/specs/ARB/geometry_shader4.txt
    GETGLPROCMACRO(glProgramParameteriARB)
    GETGLPROCMACRO(glFramebufferTextureARB)
    GETGLPROCMACRO(glFramebufferTextureLayerARB)
    GETGLPROCMACRO(glFramebufferTextureFaceARB)
  }
  #endif
  #ifdef GL_ARB_instanced_arrays
  if(r->glARBlist[48].avaible)        /// #49  http://www.opengl.org/registry/specs/ARB/instanced_arrays.txt
    GETGLPROCMACRO(glVertexAttribDivisorARB)
  #endif
  #ifdef GL_ARB_texture_buffer_object
  if(r->glARBlist[50].avaible)        /// #51  http://www.opengl.org/registry/specs/ARB/texture_buffer_object.txt
    GETGLPROCMACRO(glTexBufferARB)
  #endif
  #ifdef WGL_ARB_create_context
  if(r->glARBlist[54].avaible)        /// #55 #74 !!!  !!! WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    GETGLPROCMACRO(wglCreateContextAttribsARB)
  #endif
  #ifdef GLX_ARB_create_context
  if(r->glARBlist[55].avaible)       /// #56 #75  !!! GLX_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
    GETGLPROCMACRO(glXCreateContextAttribsARB)
  #endif
  #ifdef GL_ARB_draw_buffers_blend
  if(r->glARBlist[68].avaible) {      /// #69  http://www.opengl.org/registry/specs/ARB/draw_buffers_blend.txt
    GETGLPROCMACRO(glBlendEquationiARB)
    GETGLPROCMACRO(glBlendEquationSeparateiARB)
    GETGLPROCMACRO(glBlendFunciARB)
    GETGLPROCMACRO(glBlendFuncSeparateiARB)
  }
  #endif
  #ifdef GL_ARB_sample_shading
  if(r->glARBlist[69].avaible)        /// #70  http://www.opengl.org/registry/specs/ARB/sample_shading.txt
    GETGLPROCMACRO(glMinSampleShadingARB)
  #endif
  #ifdef GL_ARB_shading_language_include
  if(r->glARBlist[75].avaible) {      /// #76  http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
    GETGLPROCMACRO(glNamedStringARB)
    GETGLPROCMACRO(glDeleteNamedStringARB)
    GETGLPROCMACRO(glCompileShaderIncludeARB)
    GETGLPROCMACRO(glIsNamedStringARB)
    GETGLPROCMACRO(glGetNamedStringARB)
    GETGLPROCMACRO(glGetNamedStringivARB)
  }
  #endif
  #ifdef GL_ARB_cl_event
  if(r->glARBlist[102].avaible)        /// #103  http://www.opengl.org/registry/specs/ARB/cl_event.txt
    GETGLPROCMACRO(glCreateSyncFromCLeventARB)
  #endif
  #ifdef GL_ARB_debug_output
  if(r->glARBlist[103].avaible) {      /// #104  http://www.opengl.org/registry/specs/ARB/debug_output.txt
    GETGLPROCMACRO(glDebugMessageControlARB)
    GETGLPROCMACRO(glDebugMessageInsertARB)
    GETGLPROCMACRO(glDebugMessageCallbackARB)
    GETGLPROCMACRO(glGetDebugMessageLogARB)
  }
  #endif
  #ifdef GL_ARB_robustness
  if(r->glARBlist[104].avaible) {      /// #105  http://www.opengl.org/registry/specs/ARB/robustness.txt
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
  #endif
  #ifdef GL_ARB_compute_variable_group_size
  if(r->glARBlist[152].avaible)        /// #153  http://www.opengl.org/registry/specs/ARB/compute_variable_group_size.txt
    GETGLPROCMACRO(glDispatchComputeGroupSizeARB)    /// shader stuff
  #endif
  #ifdef GL_ARB_indirect_parameters
  if(r->glARBlist[153].avaible) {      /// #154  http://www.opengl.org/registry/specs/ARB/indirect_parameters.txt
    GETGLPROCMACRO(glMultiDrawArraysIndirectCountARB)
    GETGLPROCMACRO(glMultiDrawElementsIndirectCountARB)
  }
  #endif
  #ifdef GL_ARB_sparse_texture
  if(r->glARBlist[157].avaible)        /// #158  http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
    GETGLPROCMACRO(glTexPageCommitmentARB)       /// texture memory allocation management
  #endif

  #endif /// OS_MAC ignores everything
}



///=============================///
// EXT and vendor funcs aquiring //
///=============================///
void getEXTfuncs(osiRenderer *r) {
  #ifndef OS_MAC
  
  #ifdef GL_EXT_blend_color
  if(r->glEXTlist[1].avaible) {    /// #2  http://www.opengl.org/registry/specs/EXT/blend_color.txt
    GETGLPROCMACRO(glBlendColorEXT)
  }
  #endif
  #ifdef GL_EXT_polygon_offset
  if(r->glEXTlist[2].avaible) {    /// #3  http://www.opengl.org/registry/specs/EXT/polygon_offset.txt
    GETGLPROCMACRO(glPolygonOffsetEXT)
  }
  #endif
  #ifdef GL_EXT_texture3D
  if(r->glEXTlist[5].avaible) {    /// #6  http://www.opengl.org/registry/specs/EXT/texture3D.txt
    GETGLPROCMACRO(glTexImage3DEXT)
    GETGLPROCMACRO(glTexSubImage3DEXT)
  }
  #endif
  #ifdef GL_SGIS_texture_filter4
  if(r->glEXTlist[6].avaible) {    /// #7  http://www.opengl.org/registry/specs/SGIS/texture_filter4.txt
    GETGLPROCMACRO(glGetTexFilterFuncSGIS)
    GETGLPROCMACRO(glTexFilterFuncSGIS)
  }
  #endif
  #ifdef GL_EXT_subtexture
  if(r->glEXTlist[8].avaible) {    /// #9  http://www.opengl.org/registry/specs/EXT/subtexture.txt
    GETGLPROCMACRO(glTexSubImage1DEXT)
    GETGLPROCMACRO(glTexSubImage2DEXT)
  }
  #endif
  #ifdef GL_EXT_copy_texture
  if(r->glEXTlist[9].avaible) {    /// #10  http://www.opengl.org/registry/specs/EXT/copy_texture.txt
    GETGLPROCMACRO(glCopyTexImage1DEXT)
    GETGLPROCMACRO(glCopyTexImage2DEXT)
    GETGLPROCMACRO(glCopyTexSubImage1DEXT)
    GETGLPROCMACRO(glCopyTexSubImage2DEXT)
    GETGLPROCMACRO(glCopyTexSubImage3DEXT)
  }
  #endif
  #ifdef GL_EXT_histogram
  if(r->glEXTlist[10].avaible) {    /// #11  http://www.opengl.org/registry/specs/EXT/histogram.txt
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
  #endif
  #ifdef GL_EXT_convolution
  if(r->glEXTlist[11].avaible) {    /// #12  http://www.opengl.org/registry/specs/EXT/convolution.txt
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
  #endif
  #ifdef GL_SGI_color_table
  if(r->glEXTlist[13].avaible) {    /// #14  http://www.opengl.org/registry/specs/SGI/color_table.txt
    GETGLPROCMACRO(glColorTableSGI)
    GETGLPROCMACRO(glColorTableParameterfvSGI)
    GETGLPROCMACRO(glColorTableParameterivSGI)
    GETGLPROCMACRO(glCopyColorTableSGI)
    GETGLPROCMACRO(glGetColorTableSGI)
    GETGLPROCMACRO(glGetColorTableParameterfvSGI)
    GETGLPROCMACRO(glGetColorTableParameterivSGI)
  }
  #endif
  #ifdef GL_SGIS_pixel_texture
  if(r->glEXTlist[14].avaible) {    /// #15 & #15a  GL_SGIX_pixel_texture http://www.opengl.org/registry/specs/SGIS/pixel_texture.txt http://www.opengl.org/registry/specs/SGIX/sgix_pixel_texture.txt
    GETGLPROCMACRO(glPixelTexGenParameteriSGIS)
    GETGLPROCMACRO(glPixelTexGenParameterivSGIS)
    GETGLPROCMACRO(glPixelTexGenParameterfSGIS)
    GETGLPROCMACRO(glPixelTexGenParameterfvSGIS)
    GETGLPROCMACRO(glGetPixelTexGenParameterivSGIS)
    GETGLPROCMACRO(glGetPixelTexGenParameterfvSGIS)
    GETGLPROCMACRO(glPixelTexGenSGIX) /// GL_SGIX_pixel_texture
  }
  #endif
  #ifdef GL_SGIS_texture4D
  if(r->glEXTlist[15].avaible) {    /// #16  http://www.opengl.org/registry/specs/SGIS/texture4D.txt
    GETGLPROCMACRO(glTexImage4DSGIS)
    GETGLPROCMACRO(glTexSubImage4DSGIS)
  }
  #endif
  #ifdef GL_EXT_texture_object
  if(r->glEXTlist[19].avaible) {    /// #20  http://www.opengl.org/registry/specs/EXT/texture_object.txt
    GETGLPROCMACRO(glAreTexturesResidentEXT)
    GETGLPROCMACRO(glBindTextureEXT)
    GETGLPROCMACRO(glDeleteTexturesEXT)
    GETGLPROCMACRO(glGenTexturesEXT)
    GETGLPROCMACRO(glIsTextureEXT)
    GETGLPROCMACRO(glPrioritizeTexturesEXT)
  }
  #endif
  #ifdef GL_SGIS_detail_texture
  if(r->glEXTlist[20].avaible) {    /// #21  http://www.opengl.org/registry/specs/SGIS/detail_texture.txt
    GETGLPROCMACRO(glDetailTexFuncSGIS)
    GETGLPROCMACRO(glGetDetailTexFuncSGIS)
  }
  #endif
  #ifdef GL_SGIS_sharpen_texture
  if(r->glEXTlist[21].avaible) {    /// #22  http://www.opengl.org/registry/specs/SGIS/sharpen_texture.txt
    GETGLPROCMACRO(glSharpenTexFuncSGIS)
    GETGLPROCMACRO(glGetSharpenTexFuncSGIS)
  }
  #endif
  #ifdef GL_SGIS_multisample
  if(r->glEXTlist[24].avaible) {    /// #25  GLX_SGIS_multisample http://www.opengl.org/registry/specs/SGIS/multisample.txt
    GETGLPROCMACRO(glSampleMaskSGIS)
    GETGLPROCMACRO(glSamplePatternSGIS)
  }
  #endif
  #ifdef GL_EXT_vertex_array
  if(r->glEXTlist[29].avaible) {    /// #30  http://www.opengl.org/registry/specs/EXT/vertex_array.txt
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
  #endif
  #ifdef GL_EXT_blend_minmax
  if(r->glEXTlist[36].avaible) {    /// #37  http://www.opengl.org/registry/specs/EXT/blend_minmax.txt
    GETGLPROCMACRO(glBlendEquationEXT)
  }
  #endif

  #ifdef OS_LINUX
  #ifdef GLX_SGI_swap_control
  if(r->glEXTlist[39].avaible) {    /// #40  http://www.opengl.org/registry/specs/SGI/swap_control.txt
    GETGLPROCMACRO(glXSwapIntervalSGI)
  }
  #endif
  #ifdef GLX_SGI_video_sync
  if(r->glEXTlist[40].avaible) {    /// #41  http://www.opengl.org/registry/specs/SGI/video_sync.txt
    GETGLPROCMACRO(glXGetVideoSyncSGI)
    GETGLPROCMACRO(glXWaitVideoSyncSGI)
  }
  #endif
  #ifdef GLX_SGI_make_current_read
  if(r->glEXTlist[41].avaible) {    /// #42  http://www.opengl.org/registry/specs/SGI/make_current_read.txt
    GETGLPROCMACRO(glXMakeCurrentReadSGI)
    GETGLPROCMACRO(glXGetCurrentReadDrawableSGI)
  }
  #endif
  #ifdef GLX_SGIX_video_source
  if(r->glEXTlist[42].avaible) {    /// #43  http://www.opengl.org/registry/specs/SGIX/video_source.txt
    #ifdef _VL_H
    GETGLPROCMACRO(glXCreateGLXVideoSourceSGIX)
    GETGLPROCMACRO(glXDestroyGLXVideoSourceSGIX)
    #endif
  }
  #endif
  #ifdef GLX_EXT_import_context
  if(r->glEXTlist[46].avaible) {    /// #47  http://www.opengl.org/registry/specs/EXT/import_context.txt
    GETGLPROCMACRO(glXGetCurrentDisplayEXT)
    GETGLPROCMACRO(glXQueryContextInfoEXT)
    GETGLPROCMACRO(glXGetContextIDEXT)
    GETGLPROCMACRO(glXImportContextEXT)
    GETGLPROCMACRO(glXFreeContextEXT)
  }
  #endif
  #ifdef GLX_SGIX_fbconfig
  if(r->glEXTlist[48].avaible) {    /// #49  http://www.opengl.org/registry/specs/SGIX/fbconfig.txt
    GETGLPROCMACRO(glXGetFBConfigAttribSGIX)
    GETGLPROCMACRO(glXChooseFBConfigSGIX)
    GETGLPROCMACRO(glXCreateGLXPixmapWithConfigSGIX)
    GETGLPROCMACRO(glXCreateContextWithConfigSGIX)
    GETGLPROCMACRO(glXGetVisualFromFBConfigSGIX)
    GETGLPROCMACRO(glXGetFBConfigFromVisualSGIX)
  }
  #endif
  #ifdef GLX_SGIX_pbuffer
  if(r->glEXTlist[49].avaible) {    /// #50  http://www.opengl.org/registry/specs/SGIX/pbuffer.txt
    GETGLPROCMACRO(glXCreateGLXPbufferSGIX)
    GETGLPROCMACRO(glXDestroyGLXPbufferSGIX)
    GETGLPROCMACRO(glXQueryGLXPbufferSGIX)
    GETGLPROCMACRO(glXSelectEventSGIX)
    GETGLPROCMACRO(glXGetSelectedEventSGIX)
  }
  #endif
  #endif /// OS_LINUX

  #ifdef GL_SGIX_sprite
  if(r->glEXTlist[51].avaible) {    /// #52  http://www.opengl.org/registry/specs/SGIX/sprite.txt
    GETGLPROCMACRO(glSpriteParameterfSGIX)
    GETGLPROCMACRO(glSpriteParameterfvSGIX)
    GETGLPROCMACRO(glSpriteParameteriSGIX)
    GETGLPROCMACRO(glSpriteParameterivSGIX)
  }
  #endif
  #ifdef GL_EXT_point_parameters
  if(r->glEXTlist[53].avaible) {    /// #54  http://www.opengl.org/registry/specs/EXT/point_parameters.txt
    GETGLPROCMACRO(glPointParameterfEXT)
    GETGLPROCMACRO(glPointParameterfvEXT)
  }
  #endif
  #ifdef GL_SGIX_instruments
  if(r->glEXTlist[54].avaible) {    /// #55  http://www.opengl.org/registry/specs/SGIX/instruments.txt
    GETGLPROCMACRO(glGetInstrumentsSGIX)
    GETGLPROCMACRO(glInstrumentsBufferSGIX)
    GETGLPROCMACRO(glPollInstrumentsSGIX)
    GETGLPROCMACRO(glReadInstrumentsSGIX)
    GETGLPROCMACRO(glStartInstrumentsSGIX)
    GETGLPROCMACRO(glStopInstrumentsSGIX)
  }
  #endif
  #ifdef GL_SGIX_framezoom
  if(r->glEXTlist[56].avaible) {    /// #57  http://www.opengl.org/registry/specs/SGIX/framezoom.txt
    GETGLPROCMACRO(glFrameZoomSGIX)
  }
  #endif
  #ifdef GL_SGIX_tag_sample_buffer
  if(r->glEXTlist[57].avaible) {    /// #58  http://www.opengl.org/registry/specs/SGIX/tag_sample_buffer.txt
    GETGLPROCMACRO(glTagSampleBufferSGIX)
  }
  #endif
  #ifdef GL_SGIX_reference_plane
  if(r->glEXTlist[59].avaible) {    /// #60  http://www.opengl.org/registry/specs/SGIX/reference_plane.txt
    GETGLPROCMACRO(glReferencePlaneSGIX)
  }
  #endif
  #ifdef GL_SGIX_flush_raster
  if(r->glEXTlist[60].avaible) {    /// #61  http://www.opengl.org/registry/specs/SGIX/flush_raster.txt
    GETGLPROCMACRO(glFlushRasterSGIX)
  }
  #endif
  #ifdef GLX_SGI_cushion
  if(r->glEXTlist[61].avaible) {    /// #62  http://www.opengl.org/registry/specs/SGI/cushion.txt
    GETGLPROCMACRO(glXCushionSGI)
  }
  #endif
  #ifdef GL_SGIS_fog_function
  if(r->glEXTlist[63].avaible) {    /// #64  http://www.opengl.org/registry/specs/SGIS/fog_func.txt
    GETGLPROCMACRO(glFogFuncSGIS)
    GETGLPROCMACRO(glGetFogFuncSGIS)
  }
  #endif
  #ifdef GL_HP_image_transform
  if(r->glEXTlist[65].avaible) {    /// #66  http://www.opengl.org/registry/specs/HP/image_transform.txt
    GETGLPROCMACRO(glImageTransformParameteriHP)
    GETGLPROCMACRO(glImageTransformParameterfHP)
    GETGLPROCMACRO(glImageTransformParameterivHP)
    GETGLPROCMACRO(glImageTransformParameterfvHP)
    GETGLPROCMACRO(glGetImageTransformParameterivHP)
    GETGLPROCMACRO(glGetImageTransformParameterfvHP)
  }
  #endif
  #ifdef GL_EXT_color_subtable
  if(r->glEXTlist[73].avaible) {    /// #74  http://www.opengl.org/registry/specs/EXT/color_subtable.txt
    GETGLPROCMACRO(glColorSubTableEXT)
    GETGLPROCMACRO(glCopyColorSubTableEXT)
  }
  #endif
  #ifdef GL_PGI_misc_hints
  if(r->glEXTlist[76].avaible) {    /// #77  http://www.opengl.org/registry/specs/PGI/misc_hints.txt
    GETGLPROCMACRO(glHintPGI)
  }
  #endif
  #ifdef GL_EXT_paletted_texture
  if(r->glEXTlist[77].avaible) {    /// #78  http://www.opengl.org/registry/specs/EXT/paletted_texture.txt
    GETGLPROCMACRO(glColorTableEXT)
    GETGLPROCMACRO(glGetColorTableEXT)
    GETGLPROCMACRO(glGetColorTableParameterivEXT)
    GETGLPROCMACRO(glGetColorTableParameterfvEXT)
  }
  #endif
  #ifdef GL_SGIX_list_priority
  if(r->glEXTlist[79].avaible) {    /// #80 incomplete!   http://www.opengl.org/registry/specs/SGIX/list_priority.txt
    GETGLPROCMACRO(glGetListParameterfvSGIX)
    GETGLPROCMACRO(glGetListParameterivSGIX)
    GETGLPROCMACRO(glListParameterfSGIX)
    GETGLPROCMACRO(glListParameterfvSGIX)
    GETGLPROCMACRO(glListParameteriSGIX)
    GETGLPROCMACRO(glListParameterivSGIX)
  }
  #endif
  #ifdef GLX_SGIX_video_resize
  if(r->glEXTlist[82].avaible) {    /// #83  http://www.opengl.org/registry/specs/SGIX/video_resize.txt
    GETGLPROCMACRO(glXBindChannelToWindowSGIX)
    GETGLPROCMACRO(glXChannelRectSGIX)
    GETGLPROCMACRO(glXQueryChannelRectSGIX)
    GETGLPROCMACRO(glXQueryChannelDeltasSGIX)
    GETGLPROCMACRO(glXChannelRectSyncSGIX)
  }
  #endif
  #ifdef GLX_SGIX_dm_buffer
  if(r->glEXTlist[85].avaible) {    /// #86  incomplete !!!GLX_SGIX_dmbuffer!!! http://www.opengl.org/registry/specs/SGIX/dmbuffer.txt
    #ifdef _DM_BUFFER_H_
    GETGLPROCMACRO(glXAssociateDMPbufferSGIX)
    #endif
  }
  #endif
  #ifdef GLX_SGIX_swap_group
  if(r->glEXTlist[90].avaible) {    /// #91  http://www.opengl.org/registry/specs/SGIX/swap_group.txt
    GETGLPROCMACRO(glXJoinSwapGroupSGIX)
  }
  #endif
  #ifdef GLX_SGIX_swap_barrier
  if(r->glEXTlist[91].avaible) {    /// #92  http://www.opengl.org/registry/specs/SGIX/swap_barrier.txt
    GETGLPROCMACRO(glXBindSwapBarrierSGIX)
    GETGLPROCMACRO(glXQueryMaxSwapBarriersSGIX)
  }
  #endif
  #ifdef GL_EXT_index_material
  if(r->glEXTlist[93].avaible) {    /// #94  http://www.opengl.org/registry/specs/EXT/index_material.txt
    GETGLPROCMACRO(glIndexMaterialEXT)
  }
  #endif
  #ifdef GL_EXT_index_func
  if(r->glEXTlist[94].avaible) {    /// #95  http://www.opengl.org/registry/specs/EXT/index_func.txt
    GETGLPROCMACRO(glIndexFuncEXT)
  }
  #endif
  #ifdef GL_EXT_compiled_vertex_array
  if(r->glEXTlist[96].avaible) {    /// #97 incomplete!   http://www.opengl.org/registry/specs/EXT/compiled_vertex_array.txt
    GETGLPROCMACRO(glLockArraysEXT)
    GETGLPROCMACRO(glUnlockArraysEXT)
  }
  #endif
  #ifdef GL_EXT_cull_vertex
  if(r->glEXTlist[97].avaible) {    /// #98 incomplete!  http://www.opengl.org/registry/specs/EXT/cull_vertex.txt
    GETGLPROCMACRO(glCullParameterdvEXT)
    GETGLPROCMACRO(glCullParameterfvEXT)
  }
  #endif
  #ifdef GLU_EXT_nurbs_tessellator
  if(r->glEXTlist[99].avaible) {    // ?? /// #100  http://www.opengl.org/registry/specs/EXT/nurbs_tessellator.txt
    /// #102 inc! GL_EXT_fragment_lighting http://www.opengl.org/registry/specs/EXT/fragment_lighting.txt
    // has funcs but none in glext.h ATM
    /// #112 GL_EXT_draw_range_elements http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
    GETGLPROCMACRO(glDrawRangeElementsEXT)
  }
  #endif
  #ifdef GL_EXT_light_texture
  if(r->glEXTlist[116].avaible) {    /// #117  http://www.opengl.org/registry/specs/EXT/light_texture.txt
    GETGLPROCMACRO(glApplyTextureEXT)
    GETGLPROCMACRO(glTextureLightEXT)
    GETGLPROCMACRO(glTextureMaterialEXT)
  }
  #endif
  #ifdef GL_EXT_scene_marker
  if(r->glEXTlist[119].avaible) {    /// #120  GLX_EXT_scene_marker http://www.opengl.org/registry/specs/EXT/scene_marker.txt
    // some funcs but not in glext.h ATM
  }
  #endif
  #ifdef GL_SGIX_async
  if(r->glEXTlist[131].avaible) {    /// #132 incomplete!  http://www.opengl.org/registry/specs/SGIX/async.txt
    GETGLPROCMACRO(glAsyncMarkerSGIX)
    GETGLPROCMACRO(glFinishAsyncSGIX)
    GETGLPROCMACRO(glPollAsyncSGIX)
    GETGLPROCMACRO(glGenAsyncMarkersSGIX)
    GETGLPROCMACRO(glDeleteAsyncMarkersSGIX)
    GETGLPROCMACRO(glIsAsyncMarkerSGIX)
  }
  #endif
  #ifdef GL_INTEL_parallel_arrays
  if(r->glEXTlist[135].avaible) {    /// #136  http://www.opengl.org/registry/specs/INTEL/parallel_arrays.txt
    GETGLPROCMACRO(glVertexPointervINTEL)
    GETGLPROCMACRO(glNormalPointervINTEL)
    GETGLPROCMACRO(glColorPointervINTEL)
    GETGLPROCMACRO(glTexCoordPointervINTEL)
  }
  #endif
  #ifdef GL_EXT_pixel_transform
  if(r->glEXTlist[137].avaible) {    /// #138  http://www.opengl.org/registry/specs/EXT/pixel_transform.txt
    GETGLPROCMACRO(glPixelTransformParameteriEXT)
    GETGLPROCMACRO(glPixelTransformParameterfEXT)
    GETGLPROCMACRO(glPixelTransformParameterivEXT)
    GETGLPROCMACRO(glPixelTransformParameterfvEXT)
    GETGLPROCMACRO(glGetPixelTransformParameterivEXT)
    GETGLPROCMACRO(glGetPixelTransformParameterfvEXT)
  }
  #endif
  #ifdef GL_EXT_secondary_color
  if(r->glEXTlist[144].avaible) {    /// #145  http://www.opengl.org/registry/specs/EXT/secondary_color.txt
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
  #endif
  #ifdef GL_EXT_texture_perturb_normal
  if(r->glEXTlist[146].avaible) {    /// #147  http://www.opengl.org/registry/specs/EXT/texture_perturb_normal.txt
    GETGLPROCMACRO(glTextureNormalEXT)
  }
  #endif
  #ifdef GL_EXT_multi_draw_arrays
  if(r->glEXTlist[147].avaible) {    /// #148  GL_SUN_multi_draw_arrays http://www.opengl.org/registry/specs/EXT/multi_draw_arrays.txt
    GETGLPROCMACRO(glMultiDrawArraysEXT)
    GETGLPROCMACRO(glMultiDrawElementsEXT)
  }
  #endif
  #ifdef GL_EXT_fog_coord
  if(r->glEXTlist[148].avaible) {    /// #149  http://www.opengl.org/registry/specs/EXT/fog_coord.txt
    GETGLPROCMACRO(glFogCoordfEXT)
    GETGLPROCMACRO(glFogCoordfvEXT)
    GETGLPROCMACRO(glFogCoorddEXT)
    GETGLPROCMACRO(glFogCoorddvEXT)
    GETGLPROCMACRO(glFogCoordPointerEXT)
  }
  #endif
  #ifdef GL_EXT_coordinate_frame
  if(r->glEXTlist[155].avaible) {    /// #156 inc!  http://www.opengl.org/registry/specs/EXT/coordinate_frame.txt
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
  #endif
  #ifdef GL_SUNX_constant_data
  if(r->glEXTlist[162].avaible) {    /// #163  http://www.opengl.org/registry/specs/SUNX/constant_data.txt
    GETGLPROCMACRO(glFinishTextureSUNX)
  }
  #endif
  #ifdef GL_SUN_global_alpha
  if(r->glEXTlist[163].avaible) {    /// #164  http://www.opengl.org/registry/specs/SUN/global_alpha.txt
    GETGLPROCMACRO(glGlobalAlphaFactorbSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorsSUN)
    GETGLPROCMACRO(glGlobalAlphaFactoriSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorfSUN)
    GETGLPROCMACRO(glGlobalAlphaFactordSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorubSUN)
    GETGLPROCMACRO(glGlobalAlphaFactorusSUN)
    GETGLPROCMACRO(glGlobalAlphaFactoruiSUN)
  }
  #endif
  #ifdef GL_SUN_triangle_list
  if(r->glEXTlist[164].avaible) {    /// #165  http://www.opengl.org/registry/specs/SUN/triangle_list.txt
    GETGLPROCMACRO(glReplacementCodeuiSUN)
    GETGLPROCMACRO(glReplacementCodeusSUN)
    GETGLPROCMACRO(glReplacementCodeubSUN)
    GETGLPROCMACRO(glReplacementCodeuivSUN)
    GETGLPROCMACRO(glReplacementCodeusvSUN)
    GETGLPROCMACRO(glReplacementCodeubvSUN)
    GETGLPROCMACRO(glReplacementCodePointerSUN)
  }
  #endif
  #ifdef GL_SUN_vertex
  if(r->glEXTlist[165].avaible) {    /// #166  http://www.opengl.org/registry/specs/SUN/vertex.txt
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
  #endif
  #ifdef OS_WIN
  #ifdef WGL_EXT_display_color_table
  if(r->glEXTlist[166].avaible) {    /// #167 inc!  http://www.opengl.org/registry/specs/EXT/wgl_display_color_table.txt
    GETGLPROCMACRO(wglCreateDisplayColorTableEXT)
    GETGLPROCMACRO(wglLoadDisplayColorTableEXT)
    GETGLPROCMACRO(wglBindDisplayColorTableEXT)
    GETGLPROCMACRO(wglDestroyDisplayColorTableEXT)
  }
  #endif
  #ifdef WGL_EXT_extensions_string
  if(r->glEXTlist[167].avaible) {    /// #168  http://www.opengl.org/registry/specs/EXT/wgl_extensions_string.txt
    GETGLPROCMACRO(wglGetExtensionsStringEXT)
  }
  #endif
  #ifdef WGL_EXT_make_current_read
  if(r->glEXTlist[168].avaible) {    /// #169  http://www.opengl.org/registry/specs/EXT/wgl_make_current_read.txt
    GETGLPROCMACRO(wglMakeContextCurrentEXT)
    GETGLPROCMACRO(wglGetCurrentReadDCEXT)
  }
  #endif
  #ifdef WGL_EXT_pixel_format
  if(r->glEXTlist[169].avaible) {    /// #170  http://www.opengl.org/registry/specs/EXT/wgl_pixel_format.txt
    GETGLPROCMACRO(wglGetPixelFormatAttribivEXT)
    GETGLPROCMACRO(wglGetPixelFormatAttribfvEXT)
    GETGLPROCMACRO(wglChoosePixelFormatEXT)
  }
  #endif
  #ifdef WGL_EXT_pbuffer
  if(r->glEXTlist[170].avaible) {    /// #171  http://www.opengl.org/registry/specs/EXT/wgl_pbuffer.txt
    GETGLPROCMACRO(wglCreatePbufferEXT)
    GETGLPROCMACRO(wglGetPbufferDCEXT)
    GETGLPROCMACRO(wglReleasePbufferDCEXT)
    GETGLPROCMACRO(wglDestroyPbufferEXT)
    GETGLPROCMACRO(wglQueryPbufferEXT)
  }
  #endif
  #ifdef WGL_EXT_swap_control
  if(r->glEXTlist[171].avaible) {    /// #172  http://www.opengl.org/registry/specs/EXT/wgl_swap_control.txt
    GETGLPROCMACRO(wglSwapIntervalEXT)
    GETGLPROCMACRO(wglGetSwapIntervalEXT)
  }
  #endif
  #endif /// OS_WIN
  #ifdef GL_EXT_blend_func_separate
  if(r->glEXTlist[172].avaible) {    /// #173  http://www.opengl.org/registry/specs/EXT/blend_func_separate.txt
    GETGLPROCMACRO(glBlendFuncSeparateEXT)
  }
  #endif
  #ifdef GLX_SUN_get_transparent_index
  if(r->glEXTlist[182].avaible) {    /// #183  http://www.opengl.org/registry/specs/SUN/get_transparent_index.txt
    GETGLPROCMACRO(glXGetTransparentIndexSUN)
  }
  #endif
  #ifdef GL_EXT_vertex_weighting
  if(r->glEXTlist[187].avaible) {    /// #188  http://www.opengl.org/registry/specs/EXT/vertex_weighting.txt
    GETGLPROCMACRO(glVertexWeightfEXT)
    GETGLPROCMACRO(glVertexWeightfvEXT)
    GETGLPROCMACRO(glVertexWeightPointerEXT)
  }
  #endif
  #ifdef GL_NV_vertex_array_range
  if(r->glEXTlist[189].avaible) {    /// #190  http://www.opengl.org/registry/specs/NV/vertex_array_range.txt
    GETGLPROCMACRO(glFlushVertexArrayRangeNV)
    GETGLPROCMACRO(glVertexArrayRangeNV)
  }
  #endif
  #ifdef GL_NV_register_combiners
  if(r->glEXTlist[190].avaible) {    /// #191  http://www.opengl.org/registry/specs/NV/register_combiners.txt
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
  #endif
  #ifdef GL_MESA_resize_buffers
  if(r->glEXTlist[195].avaible) {    /// #196  http://www.opengl.org/registry/specs/MESA/resize_buffers.txt
    GETGLPROCMACRO(glResizeBuffersMESA)
  }
  #endif
  #ifdef GL_MESA_window_pos
  if(r->glEXTlist[196].avaible) {    /// #197  http://www.opengl.org/registry/specs/MESA/window_pos.txt
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
  #endif
  #ifdef GL_IBM_multimode_draw_arrays
  if(r->glEXTlist[199].avaible) {    /// #200  http://www.opengl.org/registry/specs/IBM/multimode_draw_arrays.txt
    GETGLPROCMACRO(glMultiModeDrawArraysIBM)
    GETGLPROCMACRO(glMultiModeDrawElementsIBM)
  }
  #endif
  #ifdef GL_IBM_vertex_array_lists
  if(r->glEXTlist[200].avaible) {    /// #201  http://www.opengl.org/registry/specs/IBM/vertex_array_lists.txt
    GETGLPROCMACRO(glColorPointerListIBM)
    GETGLPROCMACRO(glSecondaryColorPointerListIBM)
    GETGLPROCMACRO(glEdgeFlagPointerListIBM)
    GETGLPROCMACRO(glFogCoordPointerListIBM)
    GETGLPROCMACRO(glIndexPointerListIBM)
    GETGLPROCMACRO(glNormalPointerListIBM)
    GETGLPROCMACRO(glTexCoordPointerListIBM)
    GETGLPROCMACRO(glVertexPointerListIBM)
  }
  #endif
  #ifdef GL_3DFX_tbuffer
  if(r->glEXTlist[207].avaible) {    /// #208  http://www.opengl.org/registry/specs/3DFX/tbuffer.txt
    GETGLPROCMACRO(glTbufferMask3DFX)
  }
  #endif
  #ifdef GL_EXT_multisample
  if(r->glEXTlist[208].avaible) {    /// #209  WGL_EXT_multisample http://www.opengl.org/registry/specs/EXT/wgl_multisample.txt
    GETGLPROCMACRO(glSampleMaskEXT)
    GETGLPROCMACRO(glSamplePatternEXT)
  }
  #endif
  #ifdef GL_SGIS_texture_color_mask
  if(r->glEXTlist[213].avaible) {    /// #214  http://www.opengl.org/registry/specs/SGIS/texture_color_mask.txt
    GETGLPROCMACRO(glTextureColorMaskSGIS)
  }
  #endif
  #ifdef GLX_MESA_copy_sub_buffer
  if(r->glEXTlist[214].avaible) {    /// #215  http://www.opengl.org/registry/specs/MESA/copy_sub_buffer.txt
    GETGLPROCMACRO(glXCopySubBufferMESA)
  }
  #endif
  #ifdef GLX_MESA_pixmap_colormap
  if(r->glEXTlist[215].avaible) {    /// #216  http://www.opengl.org/registry/specs/MESA/pixmap_colormap.txt
    GETGLPROCMACRO(glXCreateGLXPixmapMESA)
  }
  #endif
  #ifdef GLX_MESA_release_buffers
  if(r->glEXTlist[216].avaible) {    /// #217  http://www.opengl.org/registry/specs/MESA/release_buffers.txt
    GETGLPROCMACRO(glXReleaseBuffersMESA)
  }
  #endif
  #ifdef GLX_MESA_set_3dfx_mode
  if(r->glEXTlist[217].avaible) {    /// #218  http://www.opengl.org/registry/specs/MESA/set_3dfx_mode.txt
    GETGLPROCMACRO(glXSet3DfxModeMESA)
  }
  #endif
  #ifdef GL_NV_fence
  if(r->glEXTlist[221].avaible) {    /// #222  http://www.opengl.org/registry/specs/NV/fence.txt
    GETGLPROCMACRO(glDeleteFencesNV)
    GETGLPROCMACRO(glGenFencesNV)
    GETGLPROCMACRO(glIsFenceNV)
    GETGLPROCMACRO(glTestFenceNV)
    GETGLPROCMACRO(glGetFenceivNV)
    GETGLPROCMACRO(glFinishFenceNV)
    GETGLPROCMACRO(glSetFenceNV)
  }
  #endif
  #ifdef GL_IBM_static_data
  if(r->glEXTlist[222].avaible) {    /// #223  http://www.opengl.org/registry/specs/IBM/static_data.txt
    GETGLPROCMACRO(glFlushStaticDataIBM)
  }
  #endif
  #ifdef GL_NV_evaluators
  if(r->glEXTlist[224].avaible) {    /// #225  http://www.opengl.org/registry/specs/NV/evaluators.txt
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
  #endif
  #ifdef GL_NV_register_combiners2
  if(r->glEXTlist[226].avaible) {    /// #227  http://www.opengl.org/registry/specs/NV/register_combiners2.txt
    GETGLPROCMACRO(glCombinerStageParameterfvNV)
    GETGLPROCMACRO(glGetCombinerStageParameterfvNV)
  }
  #endif
  #ifdef GL_NV_vertex_program
  if(r->glEXTlist[232].avaible) {    /// #233  http://www.opengl.org/registry/specs/NV/vertex_program.txt
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
  #endif
  #ifdef GLX_OML_sync_control
  if(r->glEXTlist[237].avaible) {    /// #238  http://www.opengl.org/registry/specs/OML/glx_sync_control.txt
    GETGLPROCMACRO(glXGetSyncValuesOML)
    GETGLPROCMACRO(glXGetMscRateOML)
    GETGLPROCMACRO(glXSwapBuffersMscOML)
    GETGLPROCMACRO(glXWaitForMscOML)
    GETGLPROCMACRO(glXWaitForSbcOML)
  }
  #endif
  #ifdef WGL_OML_sync_control
  if(r->glEXTlist[241].avaible) {    /// #242  http://www.opengl.org/registry/specs/OML/wgl_sync_control.txt
    GETGLPROCMACRO(wglGetSyncValuesOML)
    GETGLPROCMACRO(wglGetMscRateOML)
    GETGLPROCMACRO(wglSwapBuffersMscOML)
    GETGLPROCMACRO(wglSwapLayerBuffersMscOML)
    GETGLPROCMACRO(wglWaitForMscOML)
    GETGLPROCMACRO(wglWaitForSbcOML)
  }
  #endif
  #ifdef GL_ATI_envmap_bumpmap
  if(r->glEXTlist[243].avaible) {    /// #244  http://www.opengl.org/registry/specs/ATI/envmap_bumpmap.txt
    GETGLPROCMACRO(glTexBumpParameterivATI)
    GETGLPROCMACRO(glTexBumpParameterfvATI)
    GETGLPROCMACRO(glGetTexBumpParameterivATI)
    GETGLPROCMACRO(glGetTexBumpParameterfvATI)
  }
  #endif
  #ifdef GL_ATI_fragment_shader
  if(r->glEXTlist[244].avaible) {    /// #245  http://www.opengl.org/registry/specs/ATI/fragment_shader.txt
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
  #endif
  #ifdef GL_ATI_pn_triangles
  if(r->glEXTlist[245].avaible) {    /// #246  http://www.opengl.org/registry/specs/ATI/pn_triangles.txt
    GETGLPROCMACRO(glPNTrianglesiATI)
    GETGLPROCMACRO(glPNTrianglesfATI)
  }
  #endif
  #ifdef GL_ATI_vertex_array_object
  if(r->glEXTlist[246].avaible) {    /// #247  http://www.opengl.org/registry/specs/ATI/vertex_array_object.txt
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
  #endif
  #ifdef GL_EXT_vertex_shader
  if(r->glEXTlist[247].avaible) {    /// #248  http://www.opengl.org/registry/specs/EXT/vertex_shader.txt
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
  #endif
  #ifdef GL_ATI_vertex_streams
  if(r->glEXTlist[248].avaible) {    /// #249  http://www.opengl.org/registry/specs/ATI/vertex_streams.txt
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
  #endif
  #ifdef WGL_I3D_digital_video_control
  if(r->glEXTlist[249].avaible) {    /// #250  http://www.opengl.org/registry/specs/I3D/wgl_digital_video_control.txt
    GETGLPROCMACRO(wglGetDigitalVideoParametersI3D)
    GETGLPROCMACRO(wglSetDigitalVideoParametersI3D)
  }
  #endif
  #ifdef WGL_I3D_gamma
  if(r->glEXTlist[250].avaible) {    /// #251  http://www.opengl.org/registry/specs/I3D/wgl_gamma.txt
    GETGLPROCMACRO(wglGetGammaTableParametersI3D)
    GETGLPROCMACRO(wglSetGammaTableParametersI3D)
    GETGLPROCMACRO(wglGetGammaTableI3D)
    GETGLPROCMACRO(wglSetGammaTableI3D)
  }
  #endif
  #ifdef WGL_I3D_genlock
  if(r->glEXTlist[251].avaible) {    /// #252  http://www.opengl.org/registry/specs/I3D/wgl_genlock.txt
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
  #endif
  #ifdef WGL_I3D_image_buffer
  if(r->glEXTlist[252].avaible) {    /// #253  http://www.opengl.org/registry/specs/I3D/wgl_image_buffer.txt
    GETGLPROCMACRO(wglCreateImageBufferI3D)
    GETGLPROCMACRO(wglDestroyImageBufferI3D)
    GETGLPROCMACRO(wglAssociateImageBufferEventsI3D)
    GETGLPROCMACRO(wglReleaseImageBufferEventsI3D)
  }
  #endif
  #ifdef WGL_I3D_swap_frame_lock
  if(r->glEXTlist[253].avaible) {    /// #254  http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_lock.txt
    GETGLPROCMACRO(wglEnableFrameLockI3D)
    GETGLPROCMACRO(wglDisableFrameLockI3D)
    GETGLPROCMACRO(wglIsEnabledFrameLockI3D)
    GETGLPROCMACRO(wglQueryFrameLockMasterI3D)
  }
  #endif
  #ifdef WGL_I3D_swap_frame_usage
  if(r->glEXTlist[254].avaible) {    /// #255  http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_usage.txt
    GETGLPROCMACRO(wglGetFrameUsageI3D)
    GETGLPROCMACRO(wglBeginFrameTrackingI3D)
    GETGLPROCMACRO(wglEndFrameTrackingI3D)
    GETGLPROCMACRO(wglQueryFrameTrackingI3D)
  }
  #endif
  #ifdef GL_ATI_element_array
  if(r->glEXTlist[255].avaible) {    /// #256  http://www.opengl.org/registry/specs/ATI/element_array.txt
    GETGLPROCMACRO(glElementPointerATI)
    GETGLPROCMACRO(glDrawElementArrayATI)
    GETGLPROCMACRO(glDrawRangeElementArrayATI)
  }
  #endif
  #ifdef GL_SUN_mesh_array
  if(r->glEXTlist[256].avaible) {    /// #257  http://www.opengl.org/registry/specs/SUN/mesh_array.txt
    GETGLPROCMACRO(glDrawMeshArraysSUN)
  }
  #endif
  #ifdef GL_NV_occlusion_query
  if(r->glEXTlist[260].avaible) {    /// #261  http://www.opengl.org/registry/specs/NV/occlusion_query.txt
    GETGLPROCMACRO(glGenOcclusionQueriesNV)
    GETGLPROCMACRO(glDeleteOcclusionQueriesNV)
    GETGLPROCMACRO(glIsOcclusionQueryNV)
    GETGLPROCMACRO(glBeginOcclusionQueryNV)
    GETGLPROCMACRO(glEndOcclusionQueryNV)
    GETGLPROCMACRO(glGetOcclusionQueryivNV)
    GETGLPROCMACRO(glGetOcclusionQueryuivNV)
  }
  #endif
  #ifdef GL_NV_point_sprite
  if(r->glEXTlist[261].avaible) {    /// #262  http://www.opengl.org/registry/specs/NV/point_sprite.txt
    GETGLPROCMACRO(glPointParameteriNV)
    GETGLPROCMACRO(glPointParameterivNV)
  }
  #endif
  #ifdef GL_EXT_stencil_two_side
  if(r->glEXTlist[267].avaible) {    /// #268  http://www.opengl.org/registry/specs/EXT/stencil_two_side.txt
    GETGLPROCMACRO(glActiveStencilFaceEXT)
  }
  #endif
  #ifdef GL_APPLE_element_array
  if(r->glEXTlist[270].avaible) {    /// #271  http://www.opengl.org/registry/specs/APPLE/element_array.txt
    GETGLPROCMACRO(glElementPointerAPPLE)
    GETGLPROCMACRO(glDrawElementArrayAPPLE)
    GETGLPROCMACRO(glDrawRangeElementArrayAPPLE)
    GETGLPROCMACRO(glMultiDrawElementArrayAPPLE)
    GETGLPROCMACRO(glMultiDrawRangeElementArrayAPPLE)
  }
  #endif
  #ifdef GL_APPLE_fence
  if(r->glEXTlist[271].avaible) {    /// #272  http://www.opengl.org/registry/specs/APPLE/fence.txt
    GETGLPROCMACRO(glGenFencesAPPLE)
    GETGLPROCMACRO(glDeleteFencesAPPLE)
    GETGLPROCMACRO(glSetFenceAPPLE)
    GETGLPROCMACRO(glIsFenceAPPLE)
    GETGLPROCMACRO(glTestFenceAPPLE)
    GETGLPROCMACRO(glFinishFenceAPPLE)
    GETGLPROCMACRO(glTestObjectAPPLE)
    GETGLPROCMACRO(glFinishObjectAPPLE)
  }
  #endif
  #ifdef GL_APPLE_vertex_array_object
  if(r->glEXTlist[272].avaible) {    /// #273  http://www.opengl.org/registry/specs/APPLE/vertex_array_object.txt
    GETGLPROCMACRO(glBindVertexArrayAPPLE)
    GETGLPROCMACRO(glDeleteVertexArraysAPPLE)
    GETGLPROCMACRO(glGenVertexArraysAPPLE)
    GETGLPROCMACRO(glIsVertexArrayAPPLE)
  }
  #endif
  #ifdef GL_APPLE_vertex_array_range
  if(r->glEXTlist[273].avaible) {    /// #274  http://www.opengl.org/registry/specs/APPLE/vertex_array_range.txt
    GETGLPROCMACRO(glVertexArrayRangeAPPLE)
    GETGLPROCMACRO(glFlushVertexArrayRangeAPPLE)
    GETGLPROCMACRO(glVertexArrayParameteriAPPLE)
  }
  #endif
  #ifdef GL_ATI_draw_buffers
  if(r->glEXTlist[276].avaible) {    /// #277  http://www.opengl.org/registry/specs/ATI/draw_buffers.txt
    GETGLPROCMACRO(glDrawBuffersATI)
  }
  #endif
  #ifdef GL_NV_fragment_program
  if(r->glEXTlist[281].avaible) {    /// #282  http://www.opengl.org/registry/specs/NV/fragment_program.txt
    GETGLPROCMACRO(glProgramNamedParameter4fNV)
    GETGLPROCMACRO(glProgramNamedParameter4fvNV)
    GETGLPROCMACRO(glProgramNamedParameter4dNV)
    GETGLPROCMACRO(glProgramNamedParameter4dvNV)
    GETGLPROCMACRO(glGetProgramNamedParameterfvNV)
    GETGLPROCMACRO(glGetProgramNamedParameterdvNV)
  }
  #endif
  #ifdef GL_NV_half_float
  if(r->glEXTlist[282].avaible) {    /// #283  http://www.opengl.org/registry/specs/NV/half_float.txt
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
  #endif
  #ifdef GL_NV_pixel_data_range
  if(r->glEXTlist[283].avaible) {    /// #284  http://www.opengl.org/registry/specs/NV/pixel_data_range.txt
    GETGLPROCMACRO(glPixelDataRangeNV)
    GETGLPROCMACRO(glFlushPixelDataRangeNV)
  }
  #endif
  #ifdef GL_NV_primitive_restart
  if(r->glEXTlist[284].avaible) {    /// #285  http://www.opengl.org/registry/specs/NV/primitive_restart.txt
    GETGLPROCMACRO(glPrimitiveRestartNV)
    GETGLPROCMACRO(glPrimitiveRestartIndexNV)
  }
  #endif
  #ifdef GL_ATI_map_object_buffer
  if(r->glEXTlist[287].avaible) {    /// #288  http://www.opengl.org/registry/specs/ATI/map_object_buffer.txt
    GETGLPROCMACRO(glMapObjectBufferATI)
    GETGLPROCMACRO(glUnmapObjectBufferATI)
  }
  #endif
  #ifdef GL_ATI_separate_stencil
  if(r->glEXTlist[288].avaible) {    /// #289  http://www.opengl.org/registry/specs/ATI/separate_stencil.txt
    GETGLPROCMACRO(glStencilOpSeparateATI)
    GETGLPROCMACRO(glStencilFuncSeparateATI)
  }
  #endif
  #ifdef GL_ATI_vertex_attrib_array_object
  if(r->glEXTlist[289].avaible) {    /// #290  http://www.opengl.org/registry/specs/ATI/vertex_attrib_array_object.txt
    GETGLPROCMACRO(glVertexAttribArrayObjectATI)
    GETGLPROCMACRO(glGetVertexAttribArrayObjectfvATI)
    GETGLPROCMACRO(glGetVertexAttribArrayObjectivATI)
  }
  #endif
  #ifdef GL_OES_byte_coordinates
  if(r->glEXTlist[290].avaible) {    /// #291  http://www.opengl.org/registry/specs/OES/OES_byte_coordinates.txt
    /* DISABLED UNTIL THEY FIGURE OUT HOW MANY PARAMETERS THEIR FUNCS HAVE
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
    */
  }
  #endif
  #ifdef GL_OES_fixed_point
  if(r->glEXTlist[291].avaible) {    /// #292  http://www.opengl.org/registry/specs/OES/OES_fixed_point.txt
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
  #endif
  #ifdef GL_OES_single_precision
  if(r->glEXTlist[292].avaible) {    /// #293  http://www.opengl.org/registry/specs/OES/OES_single_precision.txt
    GETGLPROCMACRO(glClearDepthfOES)
    GETGLPROCMACRO(glClipPlanefOES)
    GETGLPROCMACRO(glDepthRangefOES)
    GETGLPROCMACRO(glFrustumfOES)
    GETGLPROCMACRO(glGetClipPlanefOES)
    GETGLPROCMACRO(glOrthofOES)
  }
  #endif
  #ifdef GL_OES_query_matrix
  if(r->glEXTlist[295].avaible) {    /// #296  http://www.opengl.org/registry/specs/OES/OES_query_matrix.txt
    GETGLPROCMACRO(glQueryMatrixxOES)
  }
  #endif
  #ifdef GL_EXT_depth_bounds_test
  if(r->glEXTlist[296].avaible) {    /// #297  http://www.opengl.org/registry/specs/EXT/depth_bounds_test.txt
    GETGLPROCMACRO(glDepthBoundsEXT)
  }
  #endif
  #ifdef GL_EXT_blend_equation_separate
  if(r->glEXTlist[298].avaible) {    /// #299  http://www.opengl.org/registry/specs/EXT/blend_equation_separate.txt
    GETGLPROCMACRO(glBlendEquationSeparateEXT)
  }
  #endif
  #ifdef GLX_SGIX_hyperpipe
  if(r->glEXTlist[306].avaible) {    /// #307  http://www.opengl.org/registry/specs/SGIX/hyperpipe_group.txt
    GETGLPROCMACRO(glXQueryHyperpipeNetworkSGIX)
    GETGLPROCMACRO(glXHyperpipeConfigSGIX)
    GETGLPROCMACRO(glXQueryHyperpipeConfigSGIX)
    GETGLPROCMACRO(glXDestroyHyperpipeConfigSGIX)
    GETGLPROCMACRO(glXBindHyperpipeSGIX)
    GETGLPROCMACRO(glXQueryHyperpipeBestAttribSGIX)
    GETGLPROCMACRO(glXHyperpipeAttribSGIX)
    GETGLPROCMACRO(glXQueryHyperpipeAttribSGIX)
  }
  #endif
  #ifdef GLX_MESA_agp_offset
  if(r->glEXTlist[307].avaible) {    /// #308  http://www.opengl.org/registry/specs/MESA/agp_offset.txt
    GETGLPROCMACRO(glXGetAGPOffsetMESA)
  }
  #endif
  #ifdef GL_EXT_framebuffer_object
  if(r->glEXTlist[309].avaible) {    /// #310  http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
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
  #endif
  #ifdef GL_GREMEDY_string_marker
  if(r->glEXTlist[310].avaible) {    /// #311  http://www.opengl.org/registry/specs/GREMEDY/string_marker.txt
    GETGLPROCMACRO(glStringMarkerGREMEDY)
  }
  #endif
  #ifdef WGL_3DL_stereo_control
  if(r->glEXTlist[312].avaible) {    /// #313  http://www.opengl.org/registry/specs/3DL/stereo_control.txt
    GETGLPROCMACRO(wglSetStereoEmitterState3DL)
  }
  #endif
  #ifdef GL_EXT_stencil_clear_tag
  if(r->glEXTlist[313].avaible) {    /// #314  http://www.opengl.org/registry/specs/EXT/stencil_clear_tag.txt
    GETGLPROCMACRO(glStencilClearTagEXT)
  }
  #endif
  #ifdef GL_EXT_framebuffer_blit
  if(r->glEXTlist[315].avaible) {    /// #316  http://www.opengl.org/registry/specs/EXT/framebuffer_blit.txt
    GETGLPROCMACRO(glBlitFramebufferEXT)
  }
  #endif
  #ifdef GL_EXT_framebuffer_multisample
  if(r->glEXTlist[316].avaible) {    /// #317  http://www.opengl.org/registry/specs/EXT/framebuffer_multisample.txt
    GETGLPROCMACRO(glRenderbufferStorageMultisampleEXT)
  }
  #endif
  #ifdef GL_EXT_timer_query
  if(r->glEXTlist[318].avaible) {    /// #319  http://www.opengl.org/registry/specs/EXT/timer_query.txt
    GETGLPROCMACRO(glGetQueryObjecti64vEXT)
    GETGLPROCMACRO(glGetQueryObjectui64vEXT)
  }
  #endif
  #ifdef GL_EXT_gpu_program_parameters
  if(r->glEXTlist[319].avaible) {    /// #320  http://www.opengl.org/registry/specs/EXT/gpu_program_parameters.txt
    GETGLPROCMACRO(glProgramEnvParameters4fvEXT)
    GETGLPROCMACRO(glProgramLocalParameters4fvEXT)
  }
  #endif
  #ifdef GL_APPLE_flush_buffer_range
  if(r->glEXTlist[320].avaible) {    /// #321  http://www.opengl.org/registry/specs/APPLE/flush_buffer_range.txt
    GETGLPROCMACRO(glBufferParameteriAPPLE)
    GETGLPROCMACRO(glFlushMappedBufferRangeAPPLE)
  }
  #endif
  #ifdef GL_NV_gpu_program4
  if(r->glEXTlist[321].avaible) {    /// #322  http://www.opengl.org/registry/specs/NV/gpu_program4.txt
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
  #endif
  #ifdef GL_NV_geometry_program4
  if(r->glEXTlist[322].avaible) {    /// #323  http://www.opengl.org/registry/specs/NV/geometry_program4.txt
    GETGLPROCMACRO(glProgramVertexLimitNV)
    GETGLPROCMACRO(glFramebufferTextureEXT)
    GETGLPROCMACRO(glFramebufferTextureLayerEXT)
    GETGLPROCMACRO(glFramebufferTextureFaceEXT)
  }
  #endif
  #ifdef GL_EXT_geometry_shader4
  if(r->glEXTlist[323].avaible) {    /// #324  http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
    GETGLPROCMACRO(glProgramParameteriEXT)
  }
  #endif
  #ifdef GL_NV_vertex_program4
  if(r->glEXTlist[324].avaible) {    /// #325  http://www.opengl.org/registry/specs/NV/vertex_program4.txt
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
  #endif
  #ifdef GL_EXT_gpu_shader4
  if(r->glEXTlist[325].avaible) {    /// #326  http://www.opengl.org/registry/specs/EXT/gpu_shader4.txt
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
  #endif
  #ifdef GL_EXT_draw_instanced
  if(r->glEXTlist[326].avaible) {    /// #327  http://www.opengl.org/registry/specs/EXT/draw_instanced.txt
    GETGLPROCMACRO(glDrawArraysInstancedEXT)
    GETGLPROCMACRO(glDrawElementsInstancedEXT)
  }
  #endif
  #ifdef GL_EXT_texture_buffer_object
  if(r->glEXTlist[329].avaible) {    /// #330  http://www.opengl.org/registry/specs/EXT/texture_buffer_object.txt
    GETGLPROCMACRO(glTexBufferEXT)
  }
  #endif
  #ifdef GL_NV_depth_buffer_float
  if(r->glEXTlist[333].avaible) {    /// #334  http://www.opengl.org/registry/specs/NV/depth_buffer_float.txt
    GETGLPROCMACRO(glDepthRangedNV)
    GETGLPROCMACRO(glClearDepthdNV)
    GETGLPROCMACRO(glDepthBoundsdNV)
  }
  #endif
  #ifdef GL_NV_framebuffer_multisample_coverage
  if(r->glEXTlist[335].avaible) {    /// #336  http://www.opengl.org/registry/specs/NV/framebuffer_multisample_coverage.txt
    GETGLPROCMACRO(glRenderbufferStorageMultisampleCoverageNV)
  }
  #endif
  #ifdef GL_NV_parameter_buffer_object
  if(r->glEXTlist[338].avaible) {    /// #339  http://www.opengl.org/registry/specs/NV/parameter_buffer_object.txt
    GETGLPROCMACRO(glProgramBufferParametersfvNV)
    GETGLPROCMACRO(glProgramBufferParametersIivNV)
    GETGLPROCMACRO(glProgramBufferParametersIuivNV)
  }
  #endif
  #ifdef GL_EXT_draw_buffers2
  if(r->glEXTlist[339].avaible) {    /// #340  http://www.opengl.org/registry/specs/EXT/draw_buffers2.txt
    GETGLPROCMACRO(glColorMaskIndexedEXT)
  }
  #endif
  #ifdef GL_NV_transform_feedback
  if(r->glEXTlist[340].avaible) {    /// #341  http://www.opengl.org/registry/specs/NV/transform_feedback.txt
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
  #endif
  #ifdef GL_EXT_bindable_uniform
  if(r->glEXTlist[341].avaible) {    /// #342  http://www.opengl.org/registry/specs/EXT/bindable_uniform.txt
    GETGLPROCMACRO(glUniformBufferEXT)
    GETGLPROCMACRO(glGetUniformBufferSizeEXT)
    GETGLPROCMACRO(glGetUniformOffsetEXT)
  }
  #endif
  #ifdef GL_EXT_texture_integer
  if(r->glEXTlist[342].avaible) {    /// #343  http://www.opengl.org/registry/specs/EXT/texture_integer.txt
    GETGLPROCMACRO(glTexParameterIivEXT)
    GETGLPROCMACRO(glTexParameterIuivEXT)
    GETGLPROCMACRO(glGetTexParameterIivEXT)
    GETGLPROCMACRO(glGetTexParameterIuivEXT)
    GETGLPROCMACRO(glClearColorIiEXT)
    GETGLPROCMACRO(glClearColorIuiEXT)
  }
  #endif
  #ifdef GLX_EXT_texture_from_pixmap
  if(r->glEXTlist[343].avaible) {    /// #344  http://www.opengl.org/registry/specs/EXT/texture_from_pixmap.txt
    GETGLPROCMACRO(glXBindTexImageEXT)
    GETGLPROCMACRO(glXReleaseTexImageEXT)
  }
  #endif
  #ifdef GL_GREMEDY_frame_terminator
  if(r->glEXTlist[344].avaible) {    /// #345  http://www.opengl.org/registry/specs/GREMEDY/frame_terminator.txt
    GETGLPROCMACRO(glFrameTerminatorGREMEDY)
  }
  #endif
  #ifdef GL_NV_conditional_render
  if(r->glEXTlist[345].avaible) {    /// #346  http://www.opengl.org/registry/specs/NV/conditional_render.txt
    GETGLPROCMACRO(glBeginConditionalRenderNV)
    GETGLPROCMACRO(glEndConditionalRenderNV)
  }
  #endif
  #ifdef GL_NV_present_video
  if(r->glEXTlist[346].avaible) {    /// #347  GLX_NV_present_video WGL_NV_present_video http://www.opengl.org/registry/specs/NV/present_video.txt
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
  #endif
  #ifdef GLX_NV_video_out
  if(r->glEXTlist[347].avaible) {    /// #348  http://www.opengl.org/registry/specs/NV/glx_video_output.txt
    GETGLPROCMACRO(glXGetVideoDeviceNV)
    GETGLPROCMACRO(glXReleaseVideoDeviceNV)
    GETGLPROCMACRO(glXBindVideoImageNV)
    GETGLPROCMACRO(glXReleaseVideoImageNV)
    GETGLPROCMACRO(glXSendPbufferToVideoNV)
    GETGLPROCMACRO(glXGetVideoInfoNV)
  }
  #endif
  #ifdef WGL_NV_video_output
  if(r->glEXTlist[348].avaible) {    /// #349  http://www.opengl.org/registry/specs/NV/wgl_video_output.txt
    GETGLPROCMACRO(wglGetVideoDeviceNV)
    GETGLPROCMACRO(wglReleaseVideoDeviceNV)
    GETGLPROCMACRO(wglBindVideoImageNV)
    GETGLPROCMACRO(wglReleaseVideoImageNV)
    GETGLPROCMACRO(wglSendPbufferToVideoNV)
    GETGLPROCMACRO(wglGetVideoInfoNV)
  }
  #endif
  #ifdef GLX_NV_swap_group
  if(r->glEXTlist[349].avaible) {    /// #350  http://www.opengl.org/registry/specs/NV/glx_swap_group.txt
    GETGLPROCMACRO(glXJoinSwapGroupNV)
    GETGLPROCMACRO(glXBindSwapBarrierNV)
    GETGLPROCMACRO(glXQuerySwapGroupNV)
    GETGLPROCMACRO(glXQueryMaxSwapGroupsNV)
    GETGLPROCMACRO(glXQueryFrameCountNV)
    GETGLPROCMACRO(glXResetFrameCountNV)
  }
  #endif
  #ifdef WGL_NV_swap_group
  if(r->glEXTlist[350].avaible) {    /// #351  http://www.opengl.org/registry/specs/NV/wgl_swap_group.txt
    GETGLPROCMACRO(wglJoinSwapGroupNV)
    GETGLPROCMACRO(wglBindSwapBarrierNV)
    GETGLPROCMACRO(wglQuerySwapGroupNV)
    GETGLPROCMACRO(wglQueryMaxSwapGroupsNV)
    GETGLPROCMACRO(wglQueryFrameCountNV)
    GETGLPROCMACRO(wglResetFrameCountNV)
  }
  #endif
  #ifdef GL_EXT_transform_feedback
  if(r->glEXTlist[351].avaible) {    /// #352  http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
    GETGLPROCMACRO(glBeginTransformFeedbackEXT)
    GETGLPROCMACRO(glEndTransformFeedbackEXT)
    GETGLPROCMACRO(glBindBufferRangeEXT)
    GETGLPROCMACRO(glBindBufferOffsetEXT)
    GETGLPROCMACRO(glBindBufferBaseEXT)
    GETGLPROCMACRO(glTransformFeedbackVaryingsEXT)
    GETGLPROCMACRO(glGetTransformFeedbackVaryingEXT)
  }
  #endif
  #ifdef GL_EXT_direct_state_access
  if(r->glEXTlist[352].avaible) {    /// #353  http://www.opengl.org/registry/specs/EXT/direct_state_access.txt
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
  #endif
  #ifdef WGL_NV_gpu_affinity
  if(r->glEXTlist[354].avaible) {    /// #355  http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
    GETGLPROCMACRO(wglEnumGpusNV)
    GETGLPROCMACRO(wglEnumGpuDevicesNV)
    GETGLPROCMACRO(wglCreateAffinityDCNV)
    GETGLPROCMACRO(wglEnumGpusFromAffinityDCNV)
    GETGLPROCMACRO(wglDeleteDCNV)
  }
  #endif
  #ifdef GL_NV_explicit_multisample
  if(r->glEXTlist[356].avaible) {    /// #357  http://www.opengl.org/registry/specs/NV/explicit_multisample.txt
    GETGLPROCMACRO(glGetMultisamplefvNV)
    GETGLPROCMACRO(glSampleMaskIndexedNV)
    GETGLPROCMACRO(glTexRenderbufferNV)
  }
  #endif
  #ifdef GL_NV_transform_feedback2
  if(r->glEXTlist[357].avaible) {    /// #358  http://www.opengl.org/registry/specs/NV/transform_feedback2.txt
    GETGLPROCMACRO(glBindTransformFeedbackNV)
    GETGLPROCMACRO(glDeleteTransformFeedbacksNV)
    GETGLPROCMACRO(glGenTransformFeedbacksNV)
    GETGLPROCMACRO(glIsTransformFeedbackNV)
    GETGLPROCMACRO(glPauseTransformFeedbackNV)
    GETGLPROCMACRO(glResumeTransformFeedbackNV)
    GETGLPROCMACRO(glDrawTransformFeedbackNV)
  }
  #endif
  #ifdef GL_AMD_performance_monitor
  if(r->glEXTlist[359].avaible) {    /// #360  http://www.opengl.org/registry/specs/AMD/performance_monitor.txt
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
  #endif
  #ifdef WGL_AMD_gpu_association
  if(r->glEXTlist[360].avaible) {    /// #361  http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
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
  #ifdef GL_AMD_vertex_shader_tessellator
  if(r->glEXTlist[362].avaible) {    /// #363  http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt
    GETGLPROCMACRO(glTessellationFactorAMD)
    GETGLPROCMACRO(glTessellationModeAMD)
  }
  #endif
  #ifdef GL_EXT_provoking_vertex
  if(r->glEXTlist[363].avaible) {    /// #364  http://www.opengl.org/registry/specs/EXT/provoking_vertex.txt
    GETGLPROCMACRO(glProvokingVertexEXT)
  }
  #endif
  #ifdef GL_AMD_draw_buffers_blend
  if(r->glEXTlist[365].avaible) {    /// #366  http://www.opengl.org/registry/specs/AMD/draw_buffers_blend.txt
    GETGLPROCMACRO(glBlendFuncIndexedAMD)
    GETGLPROCMACRO(glBlendFuncSeparateIndexedAMD)
    GETGLPROCMACRO(glBlendEquationIndexedAMD)
    GETGLPROCMACRO(glBlendEquationSeparateIndexedAMD)
  }
  #endif
  #ifdef GL_APPLE_texture_range
  if(r->glEXTlist[366].avaible) {    /// #367  http://www.opengl.org/registry/specs/APPLE/texture_range.txt
    GETGLPROCMACRO(glTextureRangeAPPLE)
    GETGLPROCMACRO(glGetTexParameterPointervAPPLE)
  }
  #endif
  #ifdef GL_APPLE_vertex_program_evaluators
  if(r->glEXTlist[368].avaible) {    /// #369  http://www.opengl.org/registry/specs/APPLE/vertex_program_evaluators.txt
    GETGLPROCMACRO(glEnableVertexAttribAPPLE)
    GETGLPROCMACRO(glDisableVertexAttribAPPLE)
    GETGLPROCMACRO(glIsVertexAttribEnabledAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib1dAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib1fAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib2dAPPLE)
    GETGLPROCMACRO(glMapVertexAttrib2fAPPLE)
  }
  #endif
  #ifdef GL_APPLE_object_purgeable
  if(r->glEXTlist[370].avaible) {    /// #371  http://www.opengl.org/registry/specs/APPLE/object_purgeable.txt
    GETGLPROCMACRO(glObjectPurgeableAPPLE)
    GETGLPROCMACRO(glObjectUnpurgeableAPPLE)
    GETGLPROCMACRO(glGetObjectParameterivAPPLE)
  }
  #endif
  #ifdef GL_NV_video_capture
  if(r->glEXTlist[373].avaible) {    /// #374  GLX_NV_video_capture WGL_NV_video_capture http://www.opengl.org/registry/specs/NV/video_capture.txt
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
  #endif
  #ifdef GLX_EXT_swap_control
  if(r->glEXTlist[374].avaible) {    /// #375  http://www.opengl.org/registry/specs/EXT/swap_control.txt
    GETGLPROCMACRO(glXSwapIntervalEXT)
  }
  #endif
  #ifdef GL_NV_copy_image
  if(r->glEXTlist[375].avaible) {    /// #376  WGL_NV_copy_image GLX_NV_copy_image http://www.opengl.org/registry/specs/NV/copy_image.txt
    GETGLPROCMACRO(glCopyImageSubDataNV)
    #ifdef OS_LINUX
    GETGLPROCMACRO(glXCopyImageSubDataNV)
    #endif
    #ifdef OS_WIN
    GETGLPROCMACRO(wglCopyImageSubDataNV)
    #endif
  }
  #endif
  #ifdef GL_EXT_separate_shader_objects
  if(r->glEXTlist[376].avaible) {    /// #377  http://www.opengl.org/registry/specs/EXT/separate_shader_objects.txt
    GETGLPROCMACRO(glUseShaderProgramEXT)
    GETGLPROCMACRO(glActiveProgramEXT)
    GETGLPROCMACRO(glCreateShaderProgramEXT)
  }
  #endif
  #ifdef GL_NV_shader_buffer_load
  if(r->glEXTlist[378].avaible) {    /// #379  http://www.opengl.org/registry/specs/NV/shader_buffer_load.txt
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
  #endif
  #ifdef GL_NV_vertex_buffer_unified_memory
  if(r->glEXTlist[379].avaible) {    /// #380  http://www.opengl.org/registry/specs/NV/vertex_buffer_unified_memory.txt
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
  #endif
  #ifdef GL_NV_texture_barrier
  if(r->glEXTlist[380].avaible) {    /// #381  http://www.opengl.org/registry/specs/NV/texture_barrier.txt
    GETGLPROCMACRO(glTextureBarrierNV)
  }
  #endif
  #ifdef GL_EXT_shader_image_load_store
  if(r->glEXTlist[385].avaible) {    /// #386  http://www.opengl.org/registry/specs/EXT/shader_image_load_store.txt
    GETGLPROCMACRO(glBindImageTextureEXT)
    GETGLPROCMACRO(glMemoryBarrierEXT)
  }
  #endif
  #ifdef GL_EXT_vertex_attrib_64bit
  if(r->glEXTlist[386].avaible) {    /// #387  http://www.opengl.org/registry/specs/EXT/vertex_attrib_64bit.txt
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
  #endif
  #ifdef GL_NV_gpu_program5
  if(r->glEXTlist[387].avaible) {    /// #388  GL_NV_gpu_program_fp64 http://www.opengl.org/registry/specs/NV/gpu_program5.txt
    GETGLPROCMACRO(glProgramSubroutineParametersuivNV)
    GETGLPROCMACRO(glGetProgramSubroutineParameteruivNV)
  }
  #endif
  #ifdef GL_NV_vertex_attrib_integer_64bit
  if(r->glEXTlist[391].avaible) {    /// #392  http://www.opengl.org/registry/specs/NV/vertex_attrib_integer_64bit.txt
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
  #endif
  #ifdef GL_AMD_name_gen_delete
  if(r->glEXTlist[393].avaible) {    /// #394  http://www.opengl.org/registry/specs/AMD/name_gen_delete.txt
    GETGLPROCMACRO(glGenNamesAMD)
    GETGLPROCMACRO(glDeleteNamesAMD)
    GETGLPROCMACRO(glIsNameAMD)
  }
  #endif
  #ifdef GL_AMD_debug_output
  if(r->glEXTlist[394].avaible) {    /// #395  http://www.opengl.org/registry/specs/AMD/debug_output.txt
    GETGLPROCMACRO(glDebugMessageEnableAMD)
    GETGLPROCMACRO(glDebugMessageInsertAMD)
    GETGLPROCMACRO(glDebugMessageCallbackAMD)
    GETGLPROCMACRO(glGetDebugMessageLogAMD)
  }
  #endif
  #ifdef GL_NV_vdpau_interop
  if(r->glEXTlist[395].avaible) {    /// #396  http://www.opengl.org/registry/specs/NV/vdpau_interop.txt
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
  #endif
  #ifdef GL_NV_texture_multisample
  if(r->glEXTlist[402].avaible) {    /// #403  GL_ missing in registry, asuming mistake http://www.opengl.org/registry/specs/NV/texture_multisample.txt
    GETGLPROCMACRO(glTexImage2DMultisampleCoverageNV)
    GETGLPROCMACRO(glTexImage3DMultisampleCoverageNV)
    GETGLPROCMACRO(glTextureImage2DMultisampleNV)
    GETGLPROCMACRO(glTextureImage3DMultisampleNV)
    GETGLPROCMACRO(glTextureImage2DMultisampleCoverageNV)
    GETGLPROCMACRO(glTextureImage3DMultisampleCoverageNV)
  }
  #endif
  #ifdef GL_AMD_sample_positions
  if(r->glEXTlist[404].avaible) {    /// #405  http://www.opengl.org/registry/specs/AMD/sample_positions.txt
    GETGLPROCMACRO(glSetMultisamplefvAMD)
  }
  #endif
  #ifdef GL_EXT_x11_sync_object
  if(r->glEXTlist[405].avaible) {    /// #406  http://www.opengl.org/registry/specs/EXT/x11_sync_object.txt
    GETGLPROCMACRO(glImportSyncEXT)
  }
  #endif
  #ifdef WGL_NV_DX_interop
  if(r->glEXTlist[406].avaible) {    /// #407  http://www.opengl.org/registry/specs/NV/DX_interop.txt
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
  #ifdef GL_AMD_multi_draw_indirect
  if(r->glEXTlist[407].avaible) {    /// #408  http://www.opengl.org/registry/specs/AMD/multi_draw_indirect.txt
    GETGLPROCMACRO(glMultiDrawArraysIndirectAMD)
    GETGLPROCMACRO(glMultiDrawElementsIndirectAMD)
  }
  #endif
  #ifdef GL_NV_path_rendering
  if(r->glEXTlist[409].avaible) {    /// #410  http://www.opengl.org/registry/specs/NV/path_rendering.txt
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
  #endif
  #ifdef GL_AMD_stencil_operation_extended
  if(r->glEXTlist[412].avaible) {    /// #413  http://www.opengl.org/registry/specs/AMD/stencil_operation_extended.txt
    GETGLPROCMACRO(glStencilOpValueAMD)
  }
  #endif
  #ifdef GL_NV_bindless_texture
  if(r->glEXTlist[417].avaible) {    /// #418  http://www.opengl.org/registry/specs/NV/bindless_texture.txt
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
  #endif
  #ifdef GL_NVX_conditional_render
  if(r->glEXTlist[424].avaible) {    /// #425  http://www.opengl.org/registry/specs/NVX/nvx_conditional_render.txt
    GETGLPROCMACRO(glBeginConditionalRenderNVX)
    GETGLPROCMACRO(glEndConditionalRenderNVX)
  }
  #endif
  #ifdef GL_AMD_sparse_texture
  if(r->glEXTlist[425].avaible) {    /// #426  http://www.opengl.org/registry/specs/AMD/sparse_texture.txt
    GETGLPROCMACRO(glTexStorageSparseAMD)
    GETGLPROCMACRO(glTextureStorageSparseAMD)
  }
  #endif
  #ifdef GL_INTEL_map_texture
  if(r->glEXTlist[428].avaible) {    /// #429  http://www.opengl.org/registry/specs/INTEL/map_texture.txt
    GETGLPROCMACRO(glSyncTextureINTEL)
    GETGLPROCMACRO(glUnmapTexture2DINTEL)
    GETGLPROCMACRO(glMapTexture2DINTEL)
  }
  #endif
  #ifdef GL_NV_draw_texture
  if(r->glEXTlist[429].avaible) {    /// #430  http://www.opengl.org/registry/specs/NV/draw_texture.txt
    GETGLPROCMACRO(glDrawTextureNV)
  }
  #endif
  #ifdef GL_AMD_interleaved_elements
  if(r->glEXTlist[430].avaible) {    /// #431  http://www.opengl.org/registry/specs/AMD/interleaved_elements.txt
    GETGLPROCMACRO(glVertexAttribParameteriAMD)
  }
  #endif
  #ifdef GL_NV_bindless_multi_draw_indirect
  if(r->glEXTlist[431].avaible) {    /// #432  http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect.txt
    GETGLPROCMACRO(glMultiDrawArraysIndirectBindlessNV)
    GETGLPROCMACRO(glMultiDrawElementsIndirectBindlessNV)
  }
  #endif
  #ifdef GL_NV_blend_equation_advanced
  if(r->glEXTlist[432].avaible) {    /// #433  GL_NV_blend_equation_advanced_coherent http://www.opengl.org/registry/specs/NV/blend_equation_advanced.txt
    GETGLPROCMACRO(glBlendParameteriNV)
    GETGLPROCMACRO(glBlendBarrierNV)
  }
  #endif
  #ifdef WGL_NV_delay_before_swap
  if(r->glEXTlist[435].avaible) {    /// #436  http://www.opengl.org/registry/specs/NV/wgl_delay_before_swap.txt
    GETGLPROCMACRO(wglDelayBeforeSwapNV)
  }
  #endif
  #ifdef GL_EXT_debug_label
  if(r->glEXTlist[438].avaible) {    /// #439  http://www.opengl.org/registry/specs/EXT/EXT_debug_label.txt
    GETGLPROCMACRO(glLabelObjectEXT)
    GETGLPROCMACRO(glGetObjectLabelEXT)
  }
  #endif
  #ifdef GL_EXT_debug_marker
  if(r->glEXTlist[439].avaible) {    /// #440  http://www.opengl.org/registry/specs/EXT/EXT_debug_marker.txt
    GETGLPROCMACRO(glInsertEventMarkerEXT)
    GETGLPROCMACRO(glPushGroupMarkerEXT)
    GETGLPROCMACRO(glPopGroupMarkerEXT)
  }
  #endif
  #ifdef GL_AMD_occlusion_query_event
  if(r->glEXTlist[441].avaible) {    /// #442  http://www.opengl.org/registry/specs/AMD/occlusion_query_event.txt
    GETGLPROCMACRO(glQueryObjectParameteruiAMD)
  }
  #endif
  #ifdef GL_INTEL_performance_query
  if(r->glEXTlist[442].avaible) {    /// #443  http://www.opengl.org/registry/specs/INTEL/performance_query.txt
    /* DISABLED
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
    */
  }
  #endif
  #ifdef GL_AMD_gpu_shader_int64
  if(r->glEXTlist[450].avaible) {    /// #451  http://www.opengl.org/registry/specs/AMD/gpu_shader_int64.txt
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
  #endif

  #endif /// OS_MAC ignore
}










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
  {"WGL_ARB_create_context_profile",   0}, // http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
  {"GLX_ARB_create_context_profile",   0}, // http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
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








