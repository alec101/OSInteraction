#pragma once
// At the back of the file, there's a very useful list with all the OpenGL extensions, a small description and their internet link for full documentation

// Windows MSDN says that function pointers are not guaranteed to work for different contexts, therefore, each renderer
//   is created with it's own list of extension pointers. Everything is done (in osi) so the function names is the same, but
//   under the hood they point to the currently active renderer's extension functions (this suposed to be Microsoft's job imho)


// Research:
/*
 * windows has gl.h on v1.1
 * linux has gl.h on v1.3, has GL_ARB_imaging extension, too, and some others
*/


// #define OSI_USING_GLEXT_DEFINES 1 // this is another way to set the ogl funcs, with defines, not tested, tho


/// each oGL extension will have one of these structs (there is an array with all of them)
struct GlExt {
  const char *desc;      /// extension string, case sensitive, exact match!
  bool avaible;     /// if this extension is avaible or not;
  GlExt() { desc= NULL; avaible= false; }
  GlExt(const char *s, bool b): desc(s), avaible(b) {}
};

extern GlExt _glARBlistEmpty[];   /// [internal] each renderer will create it's starting glARBlist from this
extern GlExt _glEXTlistEmpty[];   /// [internal] each renderer will create it's starting glEXTlist from this
extern GlExt _glOTHERlistEmpty[]; /// [internal] each renderer will create it's starting glOTHERlist from this
//extern bool getGlProc(cchar *, void **); /// [kinda internal]OpenGL func pointer retriever; pass the name of the function, and the pointer to aquire the address


///====================================================///
// all openGL extension functions, under this structure //
///====================================================///

struct GlExtFuncs {
  char initialized;           /// if the extension funcs have been aquired

  GlExtFuncs() { initialized= false; }
  
  // per OpenGL version functions list ==================------------------------------------
  ///==================================------------------------------------------------------
#ifndef OS_MAC
  /// OpenGL 1.2 funcs =================------------------------------
  PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
  PFNGLTEXIMAGE3DPROC        glTexImage3D;
  PFNGLTEXSUBIMAGE3DPROC     glTexSubImage3D;
  PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;

  /// OpenGL 1.3 funcs =================------------------------------
  PFNGLACTIVETEXTUREPROC        glActiveTexture;
  PFNGLSAMPLECOVERAGEPROC       glSampleCoverage;
  PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
  PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
  PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
  PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
  PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
  PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
  PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
  #ifdef OS_WIN /// ofc, these funcs dont have pointers under linux, dunno why
  PFNGLCLIENTACTIVETEXTUREPROC  glClientActiveTexture;
  PFNGLMULTITEXCOORD1DPROC      glMultiTexCoord1d;
  PFNGLMULTITEXCOORD1DVPROC     glMultiTexCoord1dv;
  PFNGLMULTITEXCOORD1FPROC      glMultiTexCoord1f;
  PFNGLMULTITEXCOORD1FVPROC     glMultiTexCoord1fv;
  PFNGLMULTITEXCOORD1IPROC      glMultiTexCoord1i;
  PFNGLMULTITEXCOORD1IVPROC     glMultiTexCoord1iv;
  PFNGLMULTITEXCOORD1SPROC      glMultiTexCoord1s;
  PFNGLMULTITEXCOORD1SVPROC     glMultiTexCoord1sv;
  PFNGLMULTITEXCOORD2DPROC      glMultiTexCoord2d;
  PFNGLMULTITEXCOORD2DVPROC     glMultiTexCoord2dv;
  PFNGLMULTITEXCOORD2FPROC      glMultiTexCoord2f;
  PFNGLMULTITEXCOORD2FVPROC     glMultiTexCoord2fv;
  PFNGLMULTITEXCOORD2IPROC      glMultiTexCoord2i;
  PFNGLMULTITEXCOORD2IVPROC     glMultiTexCoord2iv;
  PFNGLMULTITEXCOORD2SPROC      glMultiTexCoord2s;
  PFNGLMULTITEXCOORD2SVPROC     glMultiTexCoord2sv;
  PFNGLMULTITEXCOORD3DPROC      glMultiTexCoord3d;
  PFNGLMULTITEXCOORD3DVPROC     glMultiTexCoord3dv;
  PFNGLMULTITEXCOORD3FPROC      glMultiTexCoord3f;
  PFNGLMULTITEXCOORD3FVPROC     glMultiTexCoord3fv;
  PFNGLMULTITEXCOORD3IPROC      glMultiTexCoord3i;
  PFNGLMULTITEXCOORD3IVPROC     glMultiTexCoord3iv;
  PFNGLMULTITEXCOORD3SPROC      glMultiTexCoord3s;
  PFNGLMULTITEXCOORD3SVPROC     glMultiTexCoord3sv;
  PFNGLMULTITEXCOORD4DPROC      glMultiTexCoord4d;
  PFNGLMULTITEXCOORD4DVPROC     glMultiTexCoord4dv;
  PFNGLMULTITEXCOORD4FPROC      glMultiTexCoord4f;
  PFNGLMULTITEXCOORD4FVPROC     glMultiTexCoord4fv;
  PFNGLMULTITEXCOORD4IPROC      glMultiTexCoord4i;
  PFNGLMULTITEXCOORD4IVPROC     glMultiTexCoord4iv;
  PFNGLMULTITEXCOORD4SPROC      glMultiTexCoord4s;
  PFNGLMULTITEXCOORD4SVPROC     glMultiTexCoord4sv;
  PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
  PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
  PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
  PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;
  #endif /// OS_WIN

  /// OpenGL 1.4 funcs =================------------------------------
  PFNGLBLENDFUNCSEPARATEPROC  glBlendFuncSeparate;
  PFNGLMULTIDRAWARRAYSPROC    glMultiDrawArrays;
  PFNGLMULTIDRAWELEMENTSPROC  glMultiDrawElements;
  PFNGLPOINTPARAMETERFPROC    glPointParameterf;
  PFNGLPOINTPARAMETERFVPROC   glPointParameterfv;
  PFNGLPOINTPARAMETERIPROC    glPointParameteri;
  PFNGLPOINTPARAMETERIVPROC   glPointParameteriv;
  PFNGLFOGCOORDFPROC          glFogCoordf;
  PFNGLFOGCOORDFVPROC         glFogCoordfv;
  PFNGLFOGCOORDDPROC          glFogCoordd;
  PFNGLFOGCOORDDVPROC         glFogCoorddv;
  PFNGLFOGCOORDPOINTERPROC    glFogCoordPointer;
  PFNGLSECONDARYCOLOR3BPROC   glSecondaryColor3b;
  PFNGLSECONDARYCOLOR3BVPROC  glSecondaryColor3bv;
  PFNGLSECONDARYCOLOR3DPROC   glSecondaryColor3d;
  PFNGLSECONDARYCOLOR3DVPROC  glSecondaryColor3dv;
  PFNGLSECONDARYCOLOR3FPROC   glSecondaryColor3f;
  PFNGLSECONDARYCOLOR3FVPROC  glSecondaryColor3fv;
  PFNGLSECONDARYCOLOR3IPROC   glSecondaryColor3i;
  PFNGLSECONDARYCOLOR3IVPROC  glSecondaryColor3iv;
  PFNGLSECONDARYCOLOR3SPROC   glSecondaryColor3s;
  PFNGLSECONDARYCOLOR3SVPROC  glSecondaryColor3sv;
  PFNGLSECONDARYCOLOR3UBPROC  glSecondaryColor3ub;
  PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv;
  PFNGLSECONDARYCOLOR3UIPROC  glSecondaryColor3ui;
  PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv;
  PFNGLSECONDARYCOLOR3USPROC  glSecondaryColor3us;
  PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv;
  PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer;
  PFNGLWINDOWPOS2DPROC        glWindowPos2d;
  PFNGLWINDOWPOS2DVPROC       glWindowPos2dv;
  PFNGLWINDOWPOS2FPROC        glWindowPos2f;
  PFNGLWINDOWPOS2FVPROC       glWindowPos2fv;
  PFNGLWINDOWPOS2IPROC        glWindowPos2i;
  PFNGLWINDOWPOS2IVPROC       glWindowPos2iv;
  PFNGLWINDOWPOS2SPROC        glWindowPos2s;
  PFNGLWINDOWPOS2SVPROC       glWindowPos2sv;
  PFNGLWINDOWPOS3DPROC        glWindowPos3d;
  PFNGLWINDOWPOS3DVPROC       glWindowPos3dv;
  PFNGLWINDOWPOS3FPROC        glWindowPos3f;
  PFNGLWINDOWPOS3FVPROC       glWindowPos3fv;
  PFNGLWINDOWPOS3IPROC        glWindowPos3i;
  PFNGLWINDOWPOS3IVPROC       glWindowPos3iv;
  PFNGLWINDOWPOS3SPROC        glWindowPos3s;
  PFNGLWINDOWPOS3SVPROC       glWindowPos3sv;
  PFNGLBLENDCOLORPROC         glBlendColor;
  PFNGLBLENDEQUATIONPROC      glBlendEquation;

  /// OpenGL 1.5 funcs =================------------------------------
  PFNGLGENQUERIESPROC        glGenQueries;
  PFNGLDELETEQUERIESPROC     glDeleteQueries;
  PFNGLISQUERYPROC           glIsQuery;
  PFNGLBEGINQUERYPROC        glBeginQuery;
  PFNGLENDQUERYPROC          glEndQuery;
  PFNGLGETQUERYIVPROC        glGetQueryiv;
  PFNGLGETQUERYOBJECTIVPROC  glGetQueryObjectiv;
  PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;
  PFNGLBINDBUFFERPROC        glBindBuffer;
  PFNGLDELETEBUFFERSPROC     glDeleteBuffers;
  PFNGLGENBUFFERSPROC        glGenBuffers;
  PFNGLISBUFFERPROC          glIsBuffer;
  PFNGLBUFFERDATAPROC        glBufferData;
  PFNGLBUFFERSUBDATAPROC     glBufferSubData;
  PFNGLGETBUFFERSUBDATAPROC  glGetBufferSubData;
  PFNGLMAPBUFFERPROC         glMapBuffer;
  PFNGLUNMAPBUFFERPROC       glUnmapBuffer;
  PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
  PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;

  /// OpenGL 2.0 funcs =================------------------------------
  PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
  PFNGLDRAWBUFFERSPROC          glDrawBuffers;
  PFNGLSTENCILOPSEPARATEPROC    glStencilOpSeparate;
  PFNGLSTENCILFUNCSEPARATEPROC  glStencilFuncSeparate;
  PFNGLSTENCILMASKSEPARATEPROC  glStencilMaskSeparate;
  PFNGLATTACHSHADERPROC         glAttachShader;
  PFNGLBINDATTRIBLOCATIONPROC   glBindAttribLocation;
  PFNGLCOMPILESHADERPROC        glCompileShader;
  PFNGLCREATEPROGRAMPROC        glCreateProgram;
  PFNGLCREATESHADERPROC         glCreateShader;
  PFNGLDELETEPROGRAMPROC        glDeleteProgram;
  PFNGLDELETESHADERPROC         glDeleteShader;
  PFNGLDETACHSHADERPROC         glDetachShader;
  PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
  PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
  PFNGLGETACTIVEATTRIBPROC      glGetActiveAttrib;
  PFNGLGETACTIVEUNIFORMPROC     glGetActiveUniform;
  PFNGLGETATTACHEDSHADERSPROC   glGetAttachedShaders;
  PFNGLGETATTRIBLOCATIONPROC    glGetAttribLocation;
  PFNGLGETPROGRAMIVPROC         glGetProgramiv;
  PFNGLGETPROGRAMINFOLOGPROC    glGetProgramInfoLog;
  PFNGLGETSHADERIVPROC          glGetShaderiv;
  PFNGLGETSHADERINFOLOGPROC     glGetShaderInfoLog;
  PFNGLGETSHADERSOURCEPROC      glGetShaderSource;
  PFNGLGETUNIFORMLOCATIONPROC   glGetUniformLocation;
  PFNGLGETUNIFORMFVPROC         glGetUniformfv;
  PFNGLGETUNIFORMIVPROC         glGetUniformiv;
  PFNGLGETVERTEXATTRIBDVPROC    glGetVertexAttribdv;
  PFNGLGETVERTEXATTRIBFVPROC    glGetVertexAttribfv;
  PFNGLGETVERTEXATTRIBIVPROC    glGetVertexAttribiv;
  PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
  PFNGLISPROGRAMPROC            glIsProgram;
  PFNGLISSHADERPROC             glIsShader;
  PFNGLLINKPROGRAMPROC          glLinkProgram;
  PFNGLSHADERSOURCEPROC         glShaderSource;
  PFNGLUSEPROGRAMPROC           glUseProgram;
  PFNGLUNIFORM1FPROC            glUniform1f;
  PFNGLUNIFORM2FPROC            glUniform2f;
  PFNGLUNIFORM3FPROC            glUniform3f;
  PFNGLUNIFORM4FPROC            glUniform4f;
  PFNGLUNIFORM1IPROC            glUniform1i;
  PFNGLUNIFORM2IPROC            glUniform2i;
  PFNGLUNIFORM3IPROC            glUniform3i;
  PFNGLUNIFORM4IPROC            glUniform4i;
  PFNGLUNIFORM1FVPROC           glUniform1fv;
  PFNGLUNIFORM2FVPROC           glUniform2fv;
  PFNGLUNIFORM3FVPROC           glUniform3fv;
  PFNGLUNIFORM4FVPROC           glUniform4fv;
  PFNGLUNIFORM1IVPROC           glUniform1iv;
  PFNGLUNIFORM2IVPROC           glUniform2iv;
  PFNGLUNIFORM3IVPROC           glUniform3iv;
  PFNGLUNIFORM4IVPROC           glUniform4iv;
  PFNGLUNIFORMMATRIX2FVPROC     glUniformMatrix2fv;
  PFNGLUNIFORMMATRIX3FVPROC     glUniformMatrix3fv;
  PFNGLUNIFORMMATRIX4FVPROC     glUniformMatrix4fv;
  PFNGLVALIDATEPROGRAMPROC      glValidateProgram;
  PFNGLVERTEXATTRIB1DPROC       glVertexAttrib1d;
  PFNGLVERTEXATTRIB1DVPROC      glVertexAttrib1dv;
  PFNGLVERTEXATTRIB1FPROC       glVertexAttrib1f;
  PFNGLVERTEXATTRIB1FVPROC      glVertexAttrib1fv;
  PFNGLVERTEXATTRIB1SPROC       glVertexAttrib1s;
  PFNGLVERTEXATTRIB1SVPROC      glVertexAttrib1sv;
  PFNGLVERTEXATTRIB2DPROC       glVertexAttrib2d;
  PFNGLVERTEXATTRIB2DVPROC      glVertexAttrib2dv;
  PFNGLVERTEXATTRIB2FPROC       glVertexAttrib2f;
  PFNGLVERTEXATTRIB2FVPROC      glVertexAttrib2fv;
  PFNGLVERTEXATTRIB2SPROC       glVertexAttrib2s;
  PFNGLVERTEXATTRIB2SVPROC      glVertexAttrib2sv;
  PFNGLVERTEXATTRIB3DPROC       glVertexAttrib3d;
  PFNGLVERTEXATTRIB3DVPROC      glVertexAttrib3dv;
  PFNGLVERTEXATTRIB3FPROC       glVertexAttrib3f;
  PFNGLVERTEXATTRIB3FVPROC      glVertexAttrib3fv;
  PFNGLVERTEXATTRIB3SPROC       glVertexAttrib3s;
  PFNGLVERTEXATTRIB3SVPROC      glVertexAttrib3sv;
  PFNGLVERTEXATTRIB4NBVPROC     glVertexAttrib4Nbv;
  PFNGLVERTEXATTRIB4NIVPROC     glVertexAttrib4Niv;
  PFNGLVERTEXATTRIB4NSVPROC     glVertexAttrib4Nsv;
  PFNGLVERTEXATTRIB4NUBPROC     glVertexAttrib4Nub;
  PFNGLVERTEXATTRIB4NUBVPROC    glVertexAttrib4Nubv;
  PFNGLVERTEXATTRIB4NUIVPROC    glVertexAttrib4Nuiv;
  PFNGLVERTEXATTRIB4NUSVPROC    glVertexAttrib4Nusv;
  PFNGLVERTEXATTRIB4BVPROC      glVertexAttrib4bv;
  PFNGLVERTEXATTRIB4DPROC       glVertexAttrib4d;
  PFNGLVERTEXATTRIB4DVPROC      glVertexAttrib4dv;
  PFNGLVERTEXATTRIB4FPROC       glVertexAttrib4f;
  PFNGLVERTEXATTRIB4FVPROC      glVertexAttrib4fv;
  PFNGLVERTEXATTRIB4IVPROC      glVertexAttrib4iv;
  PFNGLVERTEXATTRIB4SPROC       glVertexAttrib4s;
  PFNGLVERTEXATTRIB4SVPROC      glVertexAttrib4sv;
  PFNGLVERTEXATTRIB4UBVPROC     glVertexAttrib4ubv;
  PFNGLVERTEXATTRIB4UIVPROC     glVertexAttrib4uiv;
  PFNGLVERTEXATTRIB4USVPROC     glVertexAttrib4usv;
  PFNGLVERTEXATTRIBPOINTERPROC  glVertexAttribPointer;

  /// OpenGL 2.1 funcs =================------------------------------
  PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
  PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
  PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
  PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
  PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
  PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;

  /// OpenGL 3.0 funcs =================------------------------------
  PFNGLCOLORMASKIPROC              glColorMaski;
  PFNGLGETBOOLEANI_VPROC           glGetBooleani_v;
  PFNGLGETINTEGERI_VPROC           glGetIntegeri_v;
  PFNGLENABLEIPROC                 glEnablei;
  PFNGLDISABLEIPROC                glDisablei;
  PFNGLISENABLEDIPROC              glIsEnabledi;
  PFNGLBEGINTRANSFORMFEEDBACKPROC  glBeginTransformFeedback;
  PFNGLENDTRANSFORMFEEDBACKPROC    glEndTransformFeedback;
  PFNGLBINDBUFFERRANGEPROC         glBindBufferRange;
  PFNGLBINDBUFFERBASEPROC          glBindBufferBase;
  PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
  PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
  PFNGLCLAMPCOLORPROC              glClampColor;
  PFNGLBEGINCONDITIONALRENDERPROC  glBeginConditionalRender;
  PFNGLENDCONDITIONALRENDERPROC    glEndConditionalRender;
  PFNGLVERTEXATTRIBIPOINTERPROC    glVertexAttribIPointer;
  PFNGLGETVERTEXATTRIBIIVPROC      glGetVertexAttribIiv;
  PFNGLGETVERTEXATTRIBIUIVPROC     glGetVertexAttribIuiv;
  PFNGLVERTEXATTRIBI1IPROC         glVertexAttribI1i;
  PFNGLVERTEXATTRIBI2IPROC         glVertexAttribI2i;
  PFNGLVERTEXATTRIBI3IPROC         glVertexAttribI3i;
  PFNGLVERTEXATTRIBI4IPROC         glVertexAttribI4i;
  PFNGLVERTEXATTRIBI1UIPROC        glVertexAttribI1ui;
  PFNGLVERTEXATTRIBI2UIPROC        glVertexAttribI2ui;
  PFNGLVERTEXATTRIBI3UIPROC        glVertexAttribI3ui;
  PFNGLVERTEXATTRIBI4UIPROC        glVertexAttribI4ui;
  PFNGLVERTEXATTRIBI1IVPROC        glVertexAttribI1iv;
  PFNGLVERTEXATTRIBI2IVPROC        glVertexAttribI2iv;
  PFNGLVERTEXATTRIBI3IVPROC        glVertexAttribI3iv;
  PFNGLVERTEXATTRIBI4IVPROC        glVertexAttribI4iv;
  PFNGLVERTEXATTRIBI1UIVPROC       glVertexAttribI1uiv;
  PFNGLVERTEXATTRIBI2UIVPROC       glVertexAttribI2uiv;
  PFNGLVERTEXATTRIBI3UIVPROC       glVertexAttribI3uiv;
  PFNGLVERTEXATTRIBI4UIVPROC       glVertexAttribI4uiv;
  PFNGLVERTEXATTRIBI4BVPROC        glVertexAttribI4bv;
  PFNGLVERTEXATTRIBI4SVPROC        glVertexAttribI4sv;
  PFNGLVERTEXATTRIBI4UBVPROC       glVertexAttribI4ubv;
  PFNGLVERTEXATTRIBI4USVPROC       glVertexAttribI4usv;
  PFNGLGETUNIFORMUIVPROC           glGetUniformuiv;
  PFNGLBINDFRAGDATALOCATIONPROC    glBindFragDataLocation;
  PFNGLGETFRAGDATALOCATIONPROC     glGetFragDataLocation;
  PFNGLUNIFORM1UIPROC              glUniform1ui;
  PFNGLUNIFORM2UIPROC              glUniform2ui;
  PFNGLUNIFORM3UIPROC              glUniform3ui;
  PFNGLUNIFORM4UIPROC              glUniform4ui;
  PFNGLUNIFORM1UIVPROC             glUniform1uiv;
  PFNGLUNIFORM2UIVPROC             glUniform2uiv;
  PFNGLUNIFORM3UIVPROC             glUniform3uiv;
  PFNGLUNIFORM4UIVPROC             glUniform4uiv;
  PFNGLTEXPARAMETERIIVPROC         glTexParameterIiv;
  PFNGLTEXPARAMETERIUIVPROC        glTexParameterIuiv;
  PFNGLGETTEXPARAMETERIIVPROC      glGetTexParameterIiv;
  PFNGLGETTEXPARAMETERIUIVPROC     glGetTexParameterIuiv;
  PFNGLCLEARBUFFERIVPROC           glClearBufferiv;
  PFNGLCLEARBUFFERUIVPROC          glClearBufferuiv;
  PFNGLCLEARBUFFERFVPROC           glClearBufferfv;
  PFNGLCLEARBUFFERFIPROC           glClearBufferfi;
  PFNGLGETSTRINGIPROC              glGetStringi;
  PFNGLISRENDERBUFFERPROC          glIsRenderbuffer;
  PFNGLBINDRENDERBUFFERPROC        glBindRenderbuffer;
  PFNGLDELETERENDERBUFFERSPROC     glDeleteRenderbuffers;
  PFNGLGENRENDERBUFFERSPROC        glGenRenderbuffers;
  PFNGLRENDERBUFFERSTORAGEPROC     glRenderbufferStorage;
  PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
  PFNGLISFRAMEBUFFERPROC           glIsFramebuffer;
  PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer;
  PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers;
  PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers;
  PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus;
  PFNGLFRAMEBUFFERTEXTURE1DPROC    glFramebufferTexture1D;
  PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D;
  PFNGLFRAMEBUFFERTEXTURE3DPROC    glFramebufferTexture3D;
  PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
  PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
  PFNGLGENERATEMIPMAPPROC          glGenerateMipmap;
  PFNGLBLITFRAMEBUFFERPROC         glBlitFramebuffer;
  PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
  PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;
  PFNGLMAPBUFFERRANGEPROC          glMapBufferRange;
  PFNGLFLUSHMAPPEDBUFFERRANGEPROC  glFlushMappedBufferRange;
  PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
  PFNGLDELETEVERTEXARRAYSPROC      glDeleteVertexArrays;
  PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
  PFNGLISVERTEXARRAYPROC           glIsVertexArray;

  /// OpenGL 3.1 funcs =================------------------------------
  PFNGLDRAWARRAYSINSTANCEDPROC     glDrawArraysInstanced;
  PFNGLDRAWELEMENTSINSTANCEDPROC   glDrawElementsInstanced;
  PFNGLTEXBUFFERPROC               glTexBuffer;
  PFNGLPRIMITIVERESTARTINDEXPROC   glPrimitiveRestartIndex;
  PFNGLCOPYBUFFERSUBDATAPROC       glCopyBufferSubData;
  PFNGLGETUNIFORMINDICESPROC       glGetUniformIndices;
  PFNGLGETACTIVEUNIFORMSIVPROC     glGetActiveUniformsiv;
  PFNGLGETACTIVEUNIFORMNAMEPROC    glGetActiveUniformName;
  PFNGLGETUNIFORMBLOCKINDEXPROC    glGetUniformBlockIndex;
  PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
  PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
  PFNGLUNIFORMBLOCKBINDINGPROC     glUniformBlockBinding;

  /// OpenGL 3.2 funcs =================------------------------------
  PFNGLDRAWELEMENTSBASEVERTEXPROC      glDrawElementsBaseVertex;
  PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex;
  PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
  PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex;
  PFNGLPROVOKINGVERTEXPROC             glProvokingVertex;
  PFNGLFENCESYNCPROC                   glFenceSync;
  PFNGLISSYNCPROC                      glIsSync;
  PFNGLDELETESYNCPROC                  glDeleteSync;
  PFNGLCLIENTWAITSYNCPROC              glClientWaitSync;
  PFNGLWAITSYNCPROC                    glWaitSync;
  PFNGLGETINTEGER64VPROC               glGetInteger64v;
  PFNGLGETSYNCIVPROC                   glGetSynciv;
  PFNGLGETINTEGER64I_VPROC             glGetInteger64i_v;
  PFNGLGETBUFFERPARAMETERI64VPROC      glGetBufferParameteri64v;
  PFNGLFRAMEBUFFERTEXTUREPROC          glFramebufferTexture;
  PFNGLTEXIMAGE2DMULTISAMPLEPROC       glTexImage2DMultisample;
  PFNGLTEXIMAGE3DMULTISAMPLEPROC       glTexImage3DMultisample;
  PFNGLGETMULTISAMPLEFVPROC            glGetMultisamplefv;
  PFNGLSAMPLEMASKIPROC                 glSampleMaski;

  /// OpenGL 3.3 funcs =================------------------------------
  PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
  PFNGLGETFRAGDATAINDEXPROC         glGetFragDataIndex;
  PFNGLGENSAMPLERSPROC              glGenSamplers;
  PFNGLDELETESAMPLERSPROC           glDeleteSamplers;
  PFNGLISSAMPLERPROC                glIsSampler;
  PFNGLBINDSAMPLERPROC              glBindSampler;
  PFNGLSAMPLERPARAMETERIPROC        glSamplerParameteri;
  PFNGLSAMPLERPARAMETERIVPROC       glSamplerParameteriv;
  PFNGLSAMPLERPARAMETERFPROC        glSamplerParameterf;
  PFNGLSAMPLERPARAMETERFVPROC       glSamplerParameterfv;
  PFNGLSAMPLERPARAMETERIIVPROC      glSamplerParameterIiv;
  PFNGLSAMPLERPARAMETERIUIVPROC     glSamplerParameterIuiv;
  PFNGLGETSAMPLERPARAMETERIVPROC    glGetSamplerParameteriv;
  PFNGLGETSAMPLERPARAMETERIIVPROC   glGetSamplerParameterIiv;
  PFNGLGETSAMPLERPARAMETERFVPROC    glGetSamplerParameterfv;
  PFNGLGETSAMPLERPARAMETERIUIVPROC  glGetSamplerParameterIuiv;
  PFNGLQUERYCOUNTERPROC             glQueryCounter;
  PFNGLGETQUERYOBJECTI64VPROC       glGetQueryObjecti64v;
  PFNGLGETQUERYOBJECTUI64VPROC      glGetQueryObjectui64v;
  PFNGLVERTEXATTRIBDIVISORPROC      glVertexAttribDivisor;
  PFNGLVERTEXATTRIBP1UIPROC         glVertexAttribP1ui;
  PFNGLVERTEXATTRIBP1UIVPROC        glVertexAttribP1uiv;
  PFNGLVERTEXATTRIBP2UIPROC         glVertexAttribP2ui;
  PFNGLVERTEXATTRIBP2UIVPROC        glVertexAttribP2uiv;
  PFNGLVERTEXATTRIBP3UIPROC         glVertexAttribP3ui;
  PFNGLVERTEXATTRIBP3UIVPROC        glVertexAttribP3uiv;
  PFNGLVERTEXATTRIBP4UIPROC         glVertexAttribP4ui;
  PFNGLVERTEXATTRIBP4UIVPROC        glVertexAttribP4uiv;
  PFNGLVERTEXP2UIPROC               glVertexP2ui;
  PFNGLVERTEXP2UIVPROC              glVertexP2uiv;
  PFNGLVERTEXP3UIPROC               glVertexP3ui;
  PFNGLVERTEXP3UIVPROC              glVertexP3uiv;
  PFNGLVERTEXP4UIPROC               glVertexP4ui;
  PFNGLVERTEXP4UIVPROC              glVertexP4uiv;
  PFNGLTEXCOORDP1UIPROC             glTexCoordP1ui;
  PFNGLTEXCOORDP1UIVPROC            glTexCoordP1uiv;
  PFNGLTEXCOORDP2UIPROC             glTexCoordP2ui;
  PFNGLTEXCOORDP2UIVPROC            glTexCoordP2uiv;
  PFNGLTEXCOORDP3UIPROC             glTexCoordP3ui;
  PFNGLTEXCOORDP3UIVPROC            glTexCoordP3uiv;
  PFNGLTEXCOORDP4UIPROC             glTexCoordP4ui;
  PFNGLTEXCOORDP4UIVPROC            glTexCoordP4uiv;
  PFNGLMULTITEXCOORDP1UIPROC        glMultiTexCoordP1ui;
  PFNGLMULTITEXCOORDP1UIVPROC       glMultiTexCoordP1uiv;
  PFNGLMULTITEXCOORDP2UIPROC        glMultiTexCoordP2ui;
  PFNGLMULTITEXCOORDP2UIVPROC       glMultiTexCoordP2uiv;
  PFNGLMULTITEXCOORDP3UIPROC        glMultiTexCoordP3ui;
  PFNGLMULTITEXCOORDP3UIVPROC       glMultiTexCoordP3uiv;
  PFNGLMULTITEXCOORDP4UIPROC        glMultiTexCoordP4ui;
  PFNGLMULTITEXCOORDP4UIVPROC       glMultiTexCoordP4uiv;
  PFNGLNORMALP3UIPROC               glNormalP3ui;
  PFNGLNORMALP3UIVPROC              glNormalP3uiv;
  PFNGLCOLORP3UIPROC                glColorP3ui;
  PFNGLCOLORP3UIVPROC               glColorP3uiv;
  PFNGLCOLORP4UIPROC                glColorP4ui;
  PFNGLCOLORP4UIVPROC               glColorP4uiv;
  PFNGLSECONDARYCOLORP3UIPROC       glSecondaryColorP3ui;
  PFNGLSECONDARYCOLORP3UIVPROC      glSecondaryColorP3uiv;

  /// OpenGL 4.0 funcs =================------------------------------
  PFNGLMINSAMPLESHADINGPROC glMinSampleShading;
  PFNGLBLENDEQUATIONIPROC glBlendEquationi;
  PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei;
  PFNGLBLENDFUNCIPROC glBlendFunci;
  PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei;
  PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect;
  PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;
  PFNGLUNIFORM1DPROC glUniform1d;
  PFNGLUNIFORM2DPROC glUniform2d;
  PFNGLUNIFORM3DPROC glUniform3d;
  PFNGLUNIFORM4DPROC glUniform4d;
  PFNGLUNIFORM1DVPROC glUniform1dv;
  PFNGLUNIFORM2DVPROC glUniform2dv;
  PFNGLUNIFORM3DVPROC glUniform3dv;
  PFNGLUNIFORM4DVPROC glUniform4dv;
  PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv;
  PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv;
  PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;
  PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv;
  PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv;
  PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv;
  PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv;
  PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv;
  PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv;
  PFNGLGETUNIFORMDVPROC glGetUniformdv;
  PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation;
  PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex;
  PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv;
  PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
  PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName;
  PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv;
  PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv;
  PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv;
  PFNGLPATCHPARAMETERIPROC glPatchParameteri;
  PFNGLPATCHPARAMETERFVPROC glPatchParameterfv;
  PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
  PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks;
  PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
  PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback;
  PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback;
  PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback;
  PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;
  PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
  PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed;
  PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed;
  PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv;

  /// OpenGL 4.1 funcs =================------------------------------
  PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
  PFNGLSHADERBINARYPROC glShaderBinary;
  PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
  PFNGLDEPTHRANGEFPROC glDepthRangef;
  PFNGLCLEARDEPTHFPROC glClearDepthf;
  PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
  PFNGLPROGRAMBINARYPROC glProgramBinary;
  PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
  PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
  PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram;
  PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
  PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
  PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
  PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
  PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
  PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
  PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
  PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
  PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
  PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
  PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
  PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
  PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
  PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv;
  PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
  PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
  PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
  PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
  PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d;
  PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
  PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
  PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv;
  PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
  PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
  PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
  PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
  PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d;
  PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
  PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
  PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
  PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;
  PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
  PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;
  PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
  PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d;
  PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;
  PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;
  PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;
  PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
  PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
  PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
  PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
  PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
  PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;
  PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
  PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
  PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
  PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
  PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
  PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
  PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
  PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
  PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
  PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
  PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
  PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
  PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
  PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;
  PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d;
  PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d;
  PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d;
  PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d;
  PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv;
  PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv;
  PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv;
  PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv;
  PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;
  PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv;
  PFNGLVIEWPORTARRAYVPROC glViewportArrayv;
  PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf;
  PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
  PFNGLSCISSORARRAYVPROC glScissorArrayv;
  PFNGLSCISSORINDEXEDPROC glScissorIndexed;
  PFNGLSCISSORINDEXEDVPROC glScissorIndexedv;
  PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv;
  PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
  PFNGLGETFLOATI_VPROC glGetFloati_v;
  PFNGLGETDOUBLEI_VPROC glGetDoublei_v;

  /// OpenGL 4.2 funcs =================------------------------------
  PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance;
  PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance;
  PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;
  PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;
  PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv;
  PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
  PFNGLMEMORYBARRIERPROC glMemoryBarrier;
  PFNGLTEXSTORAGE1DPROC glTexStorage1D;
  PFNGLTEXSTORAGE2DPROC glTexStorage2D;
  PFNGLTEXSTORAGE3DPROC glTexStorage3D;
  PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced;
  PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced;

  /// OpenGL 4.3 funcs =================------------------------------
  PFNGLCLEARBUFFERDATAPROC glClearBufferData;
  PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData;
  PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
  PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect;
  PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData;
  PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri;
  PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv;
  PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v;
  PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage;
  PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage;
  PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData;
  PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData;
  PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer;
  PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer;
  PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect;
  PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect;
  PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv;
  PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex;
  PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName;
  PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv;
  PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation;
  PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;
  PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding;
  PFNGLTEXBUFFERRANGEPROC glTexBufferRange;
  PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample;
  PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample;
  PFNGLTEXTUREVIEWPROC glTextureView;
  PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer;
  PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat;
  PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat;
  PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat;
  PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding;
  PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor;
  PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl;
  PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert;
  PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
  PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog;
  PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup;
  PFNGLPOPDEBUGGROUPPROC glPopDebugGroup;
  PFNGLOBJECTLABELPROC glObjectLabel;
  PFNGLGETOBJECTLABELPROC glGetObjectLabel;
  PFNGLOBJECTPTRLABELPROC glObjectPtrLabel;
  PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel;
    
  /// OpenGL 4.4 funcs =================------------------------------
  PFNGLBUFFERSTORAGEPROC glBufferStorage;
  PFNGLCLEARTEXIMAGEPROC glClearTexImage;
  PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage;
  PFNGLBINDBUFFERSBASEPROC glBindBuffersBase;
  PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange;
  PFNGLBINDTEXTURESPROC glBindTextures;
  PFNGLBINDSAMPLERSPROC glBindSamplers;
  PFNGLBINDIMAGETEXTURESPROC glBindImageTextures;
  PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers;


  ///===============================================///
  // CORE or other extensions not in ARB or EXT list //
  ///===============================================///

  #ifdef OS_WIN
  /// GL_ARB_imaging - no PFNPROCs in linux... ffs they just work without them
  PFNGLCOLORTABLEPROC glColorTable;
  PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv;
  PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv;
  PFNGLCOPYCOLORTABLEPROC glCopyColorTable;
  PFNGLGETCOLORTABLEPROC glGetColorTable;
  PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;
  PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
  PFNGLCOLORSUBTABLEPROC glColorSubTable;
  PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable;
  PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D;
  PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D;
  PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf;
  PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv;
  PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri;
  PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv;
  PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D;
  PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D;
  PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter;
  PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv;
  PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv;
  PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter;
  PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D;
  PFNGLGETHISTOGRAMPROC glGetHistogram;
  PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv;
  PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv;
  PFNGLGETMINMAXPROC glGetMinmax;
  PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv;
  PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv;
  PFNGLHISTOGRAMPROC glHistogram;
  PFNGLMINMAXPROC glMinmax;
  PFNGLRESETHISTOGRAMPROC glResetHistogram;
  PFNGLRESETMINMAXPROC glResetMinmax;
  #endif /// OS_WIN

  /// GL_ARB_bindless_texture
  PFNGLGETTEXTUREHANDLEARBPROC glGetTextureHandleARB;
  PFNGLGETTEXTURESAMPLERHANDLEARBPROC glGetTextureSamplerHandleARB;
  PFNGLMAKETEXTUREHANDLERESIDENTARBPROC glMakeTextureHandleResidentARB;
  PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC glMakeTextureHandleNonResidentARB;
  PFNGLGETIMAGEHANDLEARBPROC glGetImageHandleARB;
  PFNGLMAKEIMAGEHANDLERESIDENTARBPROC glMakeImageHandleResidentARB;
  PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC glMakeImageHandleNonResidentARB;
  PFNGLUNIFORMHANDLEUI64ARBPROC glUniformHandleui64ARB;
  PFNGLUNIFORMHANDLEUI64VARBPROC glUniformHandleui64vARB;
  PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC glProgramUniformHandleui64ARB;
  PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC glProgramUniformHandleui64vARB;
  PFNGLISTEXTUREHANDLERESIDENTARBPROC glIsTextureHandleResidentARB;
  PFNGLISIMAGEHANDLERESIDENTARBPROC glIsImageHandleResidentARB;
  PFNGLVERTEXATTRIBL1UI64ARBPROC glVertexAttribL1ui64ARB;
  PFNGLVERTEXATTRIBL1UI64VARBPROC glVertexAttribL1ui64vARB;
  PFNGLGETVERTEXATTRIBLUI64VARBPROC glGetVertexAttribLui64vARB;

  /// GL_INGR_blend_func_separate
  PFNGLBLENDFUNCSEPARATEINGRPROC glBlendFuncSeparateINGR;
  /// GL_SGIX_fragment_lighting
  PFNGLFRAGMENTCOLORMATERIALSGIXPROC glFragmentColorMaterialSGIX;
  PFNGLFRAGMENTLIGHTFSGIXPROC glFragmentLightfSGIX;
  PFNGLFRAGMENTLIGHTFVSGIXPROC glFragmentLightfvSGIX;
  PFNGLFRAGMENTLIGHTISGIXPROC glFragmentLightiSGIX;
  PFNGLFRAGMENTLIGHTIVSGIXPROC glFragmentLightivSGIX;
  PFNGLFRAGMENTLIGHTMODELFSGIXPROC glFragmentLightModelfSGIX;
  PFNGLFRAGMENTLIGHTMODELFVSGIXPROC glFragmentLightModelfvSGIX;
  PFNGLFRAGMENTLIGHTMODELISGIXPROC glFragmentLightModeliSGIX;
  PFNGLFRAGMENTLIGHTMODELIVSGIXPROC glFragmentLightModelivSGIX;
  PFNGLFRAGMENTMATERIALFSGIXPROC glFragmentMaterialfSGIX;
  PFNGLFRAGMENTMATERIALFVSGIXPROC glFragmentMaterialfvSGIX;
  PFNGLFRAGMENTMATERIALISGIXPROC glFragmentMaterialiSGIX;
  PFNGLFRAGMENTMATERIALIVSGIXPROC glFragmentMaterialivSGIX;
  PFNGLGETFRAGMENTLIGHTFVSGIXPROC glGetFragmentLightfvSGIX;
  PFNGLGETFRAGMENTLIGHTIVSGIXPROC glGetFragmentLightivSGIX;
  PFNGLGETFRAGMENTMATERIALFVSGIXPROC glGetFragmentMaterialfvSGIX;
  PFNGLGETFRAGMENTMATERIALIVSGIXPROC glGetFragmentMaterialivSGIX;
  PFNGLLIGHTENVISGIXPROC glLightEnviSGIX;

  /// GL_SGIX_polynomial_ffd
  PFNGLDEFORMATIONMAP3DSGIXPROC glDeformationMap3dSGIX;
  PFNGLDEFORMATIONMAP3FSGIXPROC glDeformationMap3fSGIX;
  PFNGLDEFORMSGIXPROC glDeformSGIX;
  PFNGLLOADIDENTITYDEFORMATIONMAPSGIXPROC glLoadIdentityDeformationMapSGIX;

  /// GL_SGIS_point_parameters
  PFNGLPOINTPARAMETERFSGISPROC glPointParameterfSGIS;
  PFNGLPOINTPARAMETERFVSGISPROC glPointParameterfvSGIS;

  /// GL_SGIX_igloo_interface
  PFNGLIGLOOINTERFACESGIXPROC glIglooInterfaceSGIX;

  #ifdef OS_WIN
  /// WGL_NV_vertex_array_range
  PFNWGLALLOCATEMEMORYNVPROC wglAllocateMemoryNV;
  PFNWGLFREEMEMORYNVPROC wglFreeMemoryNV;
  #endif



  

  #ifdef OS_LINUX
  ///==================///
  // linux GLX versions //
  ///==================///
  /*
  /// GLX_VERSION_1_3
  PFNGLXGETFBCONFIGSPROC glXGetFBConfigs;
  PFNGLXCHOOSEFBCONFIGPROC glXChooseFBConfig;
  PFNGLXGETFBCONFIGATTRIBPROC glXGetFBConfigAttrib;
  PFNGLXGETVISUALFROMFBCONFIGPROC glXGetVisualFromFBConfig;
  PFNGLXCREATEWINDOWPROC glXCreateWindow;
  PFNGLXDESTROYWINDOWPROC glXDestroyWindow;
  PFNGLXCREATEPIXMAPPROC glXCreatePixmap;
  PFNGLXDESTROYPIXMAPPROC glXDestroyPixmap;
  PFNGLXCREATEPBUFFERPROC glXCreatePbuffer;
  PFNGLXDESTROYPBUFFERPROC glXDestroyPbuffer;
  PFNGLXQUERYDRAWABLEPROC glXQueryDrawable;
  PFNGLXCREATENEWCONTEXTPROC glXCreateNewContext;
  PFNGLXMAKECONTEXTCURRENTPROC glXMakeContextCurrent;
  PFNGLXGETCURRENTREADDRAWABLEPROC glXGetCurrentReadDrawable;
  PFNGLXQUERYCONTEXTPROC glXQueryContext;
  PFNGLXSELECTEVENTPROC glXSelectEvent;
  PFNGLXGETSELECTEDEVENTPROC glXGetSelectedEvent;
  */
  /// GLX_VERSION_1_4
  //PFNGLXGETPROCADDRESSPROC glXGetProcAddress;
  #endif /// OS_LINUX



  
  // per ARB extension functions list ==================------------------------------------
  ///=================================------------------------------------------------------

  /// #1 GL_ARB_multitexture  http://www.opengl.org/registry/specs/ARB/multitexture.txt
  PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB;
  PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
  PFNGLMULTITEXCOORD1DARBPROC     glMultiTexCoord1dARB;
  PFNGLMULTITEXCOORD1DVARBPROC    glMultiTexCoord1dvARB;
  PFNGLMULTITEXCOORD1FARBPROC     glMultiTexCoord1fARB;
  PFNGLMULTITEXCOORD1FVARBPROC    glMultiTexCoord1fvARB;
  PFNGLMULTITEXCOORD1IARBPROC     glMultiTexCoord1iARB;
  PFNGLMULTITEXCOORD1IVARBPROC    glMultiTexCoord1ivARB;
  PFNGLMULTITEXCOORD1SARBPROC     glMultiTexCoord1sARB;
  PFNGLMULTITEXCOORD1SVARBPROC    glMultiTexCoord1svARB;
  PFNGLMULTITEXCOORD2DARBPROC     glMultiTexCoord2dARB;
  PFNGLMULTITEXCOORD2DVARBPROC    glMultiTexCoord2dvARB;
  PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB;
  PFNGLMULTITEXCOORD2FVARBPROC    glMultiTexCoord2fvARB;
  PFNGLMULTITEXCOORD2IARBPROC     glMultiTexCoord2iARB;
  PFNGLMULTITEXCOORD2IVARBPROC    glMultiTexCoord2ivARB;
  PFNGLMULTITEXCOORD2SARBPROC     glMultiTexCoord2sARB;
  PFNGLMULTITEXCOORD2SVARBPROC    glMultiTexCoord2svARB;
  PFNGLMULTITEXCOORD3DARBPROC     glMultiTexCoord3dARB;
  PFNGLMULTITEXCOORD3DVARBPROC    glMultiTexCoord3dvARB;
  PFNGLMULTITEXCOORD3FARBPROC     glMultiTexCoord3fARB;
  PFNGLMULTITEXCOORD3FVARBPROC    glMultiTexCoord3fvARB;
  PFNGLMULTITEXCOORD3IARBPROC     glMultiTexCoord3iARB;
  PFNGLMULTITEXCOORD3IVARBPROC    glMultiTexCoord3ivARB;
  PFNGLMULTITEXCOORD3SARBPROC     glMultiTexCoord3sARB;
  PFNGLMULTITEXCOORD3SVARBPROC    glMultiTexCoord3svARB;
  PFNGLMULTITEXCOORD4DARBPROC     glMultiTexCoord4dARB;
  PFNGLMULTITEXCOORD4DVARBPROC    glMultiTexCoord4dvARB;
  PFNGLMULTITEXCOORD4FARBPROC     glMultiTexCoord4fARB;
  PFNGLMULTITEXCOORD4FVARBPROC    glMultiTexCoord4fvARB;
  PFNGLMULTITEXCOORD4IARBPROC     glMultiTexCoord4iARB;
  PFNGLMULTITEXCOORD4IVARBPROC    glMultiTexCoord4ivARB;
  PFNGLMULTITEXCOORD4SARBPROC     glMultiTexCoord4sARB;
  PFNGLMULTITEXCOORD4SVARBPROC    glMultiTexCoord4svARB;
  #ifdef OS_LINUX
  /// #2 GLX_ARB_get_proc_address  http://www.opengl.org/registry/specs/ARB/get_proc_address.txt
  //PFNGLXGETPROCADDRESSARBPROC glXGetProcAddressARB;
  #endif /// OS_LINUX
  /// #3 GL_ARB_transpose_matrix  http://www.opengl.org/registry/specs/ARB/transpose_matrix.txt
  PFNGLLOADTRANSPOSEMATRIXFARBPROC glLoadTransposeMatrixfARB;
  PFNGLLOADTRANSPOSEMATRIXDARBPROC glLoadTransposeMatrixdARB;
  PFNGLMULTTRANSPOSEMATRIXFARBPROC glMultTransposeMatrixfARB;
  PFNGLMULTTRANSPOSEMATRIXDARBPROC glMultTransposeMatrixdARB;
  #ifdef OS_WIN
  /// #4 WGL_ARB_buffer_region  http://www.opengl.org/registry/specs/ARB/wgl_buffer_region.txt
  PFNWGLCREATEBUFFERREGIONARBPROC   wglCreateBufferRegionARB;
  PFNWGLDELETEBUFFERREGIONARBPROC   wglDeleteBufferRegionARB;
  PFNWGLSAVEBUFFERREGIONARBPROC     wglSaveBufferRegionARB;
  PFNWGLRESTOREBUFFERREGIONARBPROC  wglRestoreBufferRegionARB;
  #endif /// OS_WIN
  /// #5 GL_ARB_multisample GLX_ARB_multisample WGL_ARB_multisample  http://www.opengl.org/registry/specs/ARB/multisample.txt
  PFNGLSAMPLECOVERAGEARBPROC glSampleCoverageARB;
  #ifdef OS_WIN
  /// #8 WGL_ARB_extensions_string http://www.opengl.org/registry/specs/ARB/wgl_extensions_string.txt
  PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
  /// #9 WGL_ARB_pixel_format http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
  //PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
  //PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
  //PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
  /// #10 WGL_ARB_make_current_read http://www.opengl.org/registry/specs/ARB/wgl_make_current_read.txt
  PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
  PFNWGLGETCURRENTREADDCARBPROC   wglGetCurrentReadDCARB;
  /// #11 WGL_ARB_pbuffer http://www.opengl.org/registry/specs/ARB/wgl_pbuffer.txt
  PFNWGLCREATEPBUFFERARBPROC    wglCreatePbufferARB;
  PFNWGLGETPBUFFERDCARBPROC     wglGetPbufferDCARB;
  PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
  PFNWGLDESTROYPBUFFERARBPROC   wglDestroyPbufferARB;
  PFNWGLQUERYPBUFFERARBPROC     wglQueryPbufferARB;
  #endif /// OS_WIN
  /// #12 GL_ARB_texture_compression http://www.opengl.org/registry/specs/ARB/texture_compression.txt
  PFNGLCOMPRESSEDTEXIMAGE3DARBPROC    glCompressedTexImage3DARB;
  PFNGLCOMPRESSEDTEXIMAGE2DARBPROC    glCompressedTexImage2DARB;
  PFNGLCOMPRESSEDTEXIMAGE1DARBPROC    glCompressedTexImage1DARB;
  PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3DARB;
  PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB;
  PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1DARB;
  PFNGLGETCOMPRESSEDTEXIMAGEARBPROC   glGetCompressedTexImageARB;
  /// #14 GL_ARB_point_parameters http://www.opengl.org/registry/specs/ARB/point_parameters.txt
  PFNGLPOINTPARAMETERFARBPROC   glPointParameterfARB;
  PFNGLPOINTPARAMETERFVARBPROC  glPointParameterfvARB;
  /// #15 GL_ARB_vertex_blend http://www.opengl.org/registry/specs/ARB/vertex_blend.txt
  PFNGLWEIGHTBVARBPROC      glWeightbvARB;
  PFNGLWEIGHTSVARBPROC      glWeightsvARB;
  PFNGLWEIGHTIVARBPROC      glWeightivARB;
  PFNGLWEIGHTFVARBPROC      glWeightfvARB;
  PFNGLWEIGHTDVARBPROC      glWeightdvARB;
  PFNGLWEIGHTUBVARBPROC     glWeightubvARB;
  PFNGLWEIGHTUSVARBPROC     glWeightusvARB;
  PFNGLWEIGHTUIVARBPROC     glWeightuivARB;
  PFNGLWEIGHTPOINTERARBPROC glWeightPointerARB;
  PFNGLVERTEXBLENDARBPROC   glVertexBlendARB;
  /// #16 GL_ARB_matrix_palette http://www.opengl.org/registry/specs/ARB/matrix_palette.txt
  PFNGLCURRENTPALETTEMATRIXARBPROC  glCurrentPaletteMatrixARB;
  PFNGLMATRIXINDEXUBVARBPROC        glMatrixIndexubvARB;
  PFNGLMATRIXINDEXUSVARBPROC        glMatrixIndexusvARB;
  PFNGLMATRIXINDEXUIVARBPROC        glMatrixIndexuivARB;
  PFNGLMATRIXINDEXPOINTERARBPROC    glMatrixIndexPointerARB;
  #ifdef OS_WIN
  /// #20 WGL_ARB_render_texture http://www.opengl.org/registry/specs/ARB/wgl_render_texture.txt
  PFNWGLBINDTEXIMAGEARBPROC     wglBindTexImageARB;
  PFNWGLRELEASETEXIMAGEARBPROC  wglReleaseTexImageARB;
  PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;
  #endif /// OS_WIN
  /// #25 GL_ARB_window_pos http://www.opengl.org/registry/specs/ARB/window_pos.txt
  PFNGLWINDOWPOS2DARBPROC  glWindowPos2dARB;
  PFNGLWINDOWPOS2DVARBPROC glWindowPos2dvARB;
  PFNGLWINDOWPOS2FARBPROC  glWindowPos2fARB;
  PFNGLWINDOWPOS2FVARBPROC glWindowPos2fvARB;
  PFNGLWINDOWPOS2IARBPROC  glWindowPos2iARB;
  PFNGLWINDOWPOS2IVARBPROC glWindowPos2ivARB;
  PFNGLWINDOWPOS2SARBPROC  glWindowPos2sARB;
  PFNGLWINDOWPOS2SVARBPROC glWindowPos2svARB;
  PFNGLWINDOWPOS3DARBPROC  glWindowPos3dARB;
  PFNGLWINDOWPOS3DVARBPROC glWindowPos3dvARB;
  PFNGLWINDOWPOS3FARBPROC  glWindowPos3fARB;
  PFNGLWINDOWPOS3FVARBPROC glWindowPos3fvARB;
  PFNGLWINDOWPOS3IARBPROC  glWindowPos3iARB;
  PFNGLWINDOWPOS3IVARBPROC glWindowPos3ivARB;
  PFNGLWINDOWPOS3SARBPROC  glWindowPos3sARB;
  PFNGLWINDOWPOS3SVARBPROC glWindowPos3svARB;
  /// #26 GL_ARB_vertex_program http://www.opengl.org/registry/specs/ARB/vertex_program.txt
  PFNGLVERTEXATTRIB1DARBPROC      glVertexAttrib1dARB;
  PFNGLVERTEXATTRIB1DVARBPROC     glVertexAttrib1dvARB;
  PFNGLVERTEXATTRIB1FARBPROC      glVertexAttrib1fARB;
  PFNGLVERTEXATTRIB1FVARBPROC     glVertexAttrib1fvARB;
  PFNGLVERTEXATTRIB1SARBPROC      glVertexAttrib1sARB;
  PFNGLVERTEXATTRIB1SVARBPROC     glVertexAttrib1svARB;
  PFNGLVERTEXATTRIB2DARBPROC      glVertexAttrib2dARB;
  PFNGLVERTEXATTRIB2DVARBPROC     glVertexAttrib2dvARB;
  PFNGLVERTEXATTRIB2FARBPROC      glVertexAttrib2fARB;
  PFNGLVERTEXATTRIB2FVARBPROC     glVertexAttrib2fvARB;
  PFNGLVERTEXATTRIB2SARBPROC      glVertexAttrib2sARB;
  PFNGLVERTEXATTRIB2SVARBPROC     glVertexAttrib2svARB;
  PFNGLVERTEXATTRIB3DARBPROC      glVertexAttrib3dARB;
  PFNGLVERTEXATTRIB3DVARBPROC     glVertexAttrib3dvARB;
  PFNGLVERTEXATTRIB3FARBPROC      glVertexAttrib3fARB;
  PFNGLVERTEXATTRIB3FVARBPROC     glVertexAttrib3fvARB;
  PFNGLVERTEXATTRIB3SARBPROC      glVertexAttrib3sARB;
  PFNGLVERTEXATTRIB3SVARBPROC     glVertexAttrib3svARB;
  PFNGLVERTEXATTRIB4NBVARBPROC    glVertexAttrib4NbvARB;
  PFNGLVERTEXATTRIB4NIVARBPROC    glVertexAttrib4NivARB;
  PFNGLVERTEXATTRIB4NSVARBPROC    glVertexAttrib4NsvARB;
  PFNGLVERTEXATTRIB4NUBARBPROC    glVertexAttrib4NubARB;
  PFNGLVERTEXATTRIB4NUBVARBPROC   glVertexAttrib4NubvARB;
  PFNGLVERTEXATTRIB4NUIVARBPROC   glVertexAttrib4NuivARB;
  PFNGLVERTEXATTRIB4NUSVARBPROC   glVertexAttrib4NusvARB;
  PFNGLVERTEXATTRIB4BVARBPROC     glVertexAttrib4bvARB;
  PFNGLVERTEXATTRIB4DARBPROC      glVertexAttrib4dARB;
  PFNGLVERTEXATTRIB4DVARBPROC     glVertexAttrib4dvARB;
  PFNGLVERTEXATTRIB4FARBPROC      glVertexAttrib4fARB;
  PFNGLVERTEXATTRIB4FVARBPROC     glVertexAttrib4fvARB;
  PFNGLVERTEXATTRIB4IVARBPROC     glVertexAttrib4ivARB;
  PFNGLVERTEXATTRIB4SARBPROC      glVertexAttrib4sARB;
  PFNGLVERTEXATTRIB4SVARBPROC     glVertexAttrib4svARB;
  PFNGLVERTEXATTRIB4UBVARBPROC    glVertexAttrib4ubvARB;
  PFNGLVERTEXATTRIB4UIVARBPROC    glVertexAttrib4uivARB;
  PFNGLVERTEXATTRIB4USVARBPROC    glVertexAttrib4usvARB;
  PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
  PFNGLENABLEVERTEXATTRIBARRAYARBPROC   glEnableVertexAttribArrayARB;
  PFNGLDISABLEVERTEXATTRIBARRAYARBPROC  glDisableVertexAttribArrayARB;
  PFNGLGETVERTEXATTRIBDVARBPROC         glGetVertexAttribdvARB;
  PFNGLGETVERTEXATTRIBFVARBPROC         glGetVertexAttribfvARB;
  PFNGLGETVERTEXATTRIBIVARBPROC         glGetVertexAttribivARB;
  PFNGLGETVERTEXATTRIBPOINTERVARBPROC   glGetVertexAttribPointervARB;
  // doc enumerates functions from GL_ARB_fragment_program here, too...
  /// #27 GL_ARB_fragment_program http://www.opengl.org/registry/specs/ARB/fragment_program.txt
  PFNGLPROGRAMSTRINGARBPROC               glProgramStringARB;
  PFNGLBINDPROGRAMARBPROC                 glBindProgramARB;
  PFNGLDELETEPROGRAMSARBPROC              glDeleteProgramsARB;
  PFNGLGENPROGRAMSARBPROC                 glGenProgramsARB;
  PFNGLPROGRAMENVPARAMETER4DARBPROC       glProgramEnvParameter4dARB;
  PFNGLPROGRAMENVPARAMETER4DVARBPROC      glProgramEnvParameter4dvARB;
  PFNGLPROGRAMENVPARAMETER4FARBPROC       glProgramEnvParameter4fARB;
  PFNGLPROGRAMENVPARAMETER4FVARBPROC      glProgramEnvParameter4fvARB;
  PFNGLPROGRAMLOCALPARAMETER4DARBPROC     glProgramLocalParameter4dARB;
  PFNGLPROGRAMLOCALPARAMETER4DVARBPROC    glProgramLocalParameter4dvARB;
  PFNGLPROGRAMLOCALPARAMETER4FARBPROC     glProgramLocalParameter4fARB;
  PFNGLPROGRAMLOCALPARAMETER4FVARBPROC    glProgramLocalParameter4fvARB;
  PFNGLGETPROGRAMENVPARAMETERDVARBPROC    glGetProgramEnvParameterdvARB;
  PFNGLGETPROGRAMENVPARAMETERFVARBPROC    glGetProgramEnvParameterfvARB;
  PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC  glGetProgramLocalParameterdvARB;
  PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC  glGetProgramLocalParameterfvARB;
  PFNGLGETPROGRAMIVARBPROC                glGetProgramivARB;
  PFNGLGETPROGRAMSTRINGARBPROC            glGetProgramStringARB;
  PFNGLISPROGRAMARBPROC                   glIsProgramARB;
  /// #28 GL_ARB_vertex_buffer_object GLX_ARB_vertex_buffer_object http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
  PFNGLBINDBUFFERARBPROC            glBindBufferARB;
  PFNGLDELETEBUFFERSARBPROC         glDeleteBuffersARB;
  PFNGLGENBUFFERSARBPROC            glGenBuffersARB;
  PFNGLISBUFFERARBPROC              glIsBufferARB;
  PFNGLBUFFERDATAARBPROC            glBufferDataARB;
  PFNGLBUFFERSUBDATAARBPROC         glBufferSubDataARB;
  PFNGLGETBUFFERSUBDATAARBPROC      glGetBufferSubDataARB;
  PFNGLMAPBUFFERARBPROC             glMapBufferARB;
  PFNGLUNMAPBUFFERARBPROC           glUnmapBufferARB;
  PFNGLGETBUFFERPARAMETERIVARBPROC  glGetBufferParameterivARB;
  PFNGLGETBUFFERPOINTERVARBPROC     glGetBufferPointervARB;
  /// #29 GL_ARB_occlusion_query http://www.opengl.org/registry/specs/ARB/occlusion_query.txt
  PFNGLGENQUERIESARBPROC        glGenQueriesARB;
  PFNGLDELETEQUERIESARBPROC     glDeleteQueriesARB;
  PFNGLISQUERYARBPROC           glIsQueryARB;
  PFNGLBEGINQUERYARBPROC        glBeginQueryARB;
  PFNGLENDQUERYARBPROC          glEndQueryARB;
  PFNGLGETQUERYIVARBPROC        glGetQueryivARB;
  PFNGLGETQUERYOBJECTIVARBPROC  glGetQueryObjectivARB;
  PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB;
  /// #30 GL_ARB_shader_objects http://www.opengl.org/registry/specs/ARB/shader_objects.txt
  PFNGLDELETEOBJECTARBPROC        glDeleteObjectARB;
  PFNGLGETHANDLEARBPROC           glGetHandleARB;
  PFNGLDETACHOBJECTARBPROC        glDetachObjectARB;
  PFNGLCREATESHADEROBJECTARBPROC  glCreateShaderObjectARB;
  PFNGLSHADERSOURCEARBPROC        glShaderSourceARB;
  PFNGLCOMPILESHADERARBPROC       glCompileShaderARB;
  PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
  PFNGLATTACHOBJECTARBPROC        glAttachObjectARB;
  PFNGLLINKPROGRAMARBPROC         glLinkProgramARB;
  PFNGLUSEPROGRAMOBJECTARBPROC    glUseProgramObjectARB;
  PFNGLVALIDATEPROGRAMARBPROC     glValidateProgramARB;
  PFNGLUNIFORM1FARBPROC           glUniform1fARB;
  PFNGLUNIFORM2FARBPROC           glUniform2fARB;
  PFNGLUNIFORM3FARBPROC           glUniform3fARB;
  PFNGLUNIFORM4FARBPROC           glUniform4fARB;
  PFNGLUNIFORM1IARBPROC           glUniform1iARB;
  PFNGLUNIFORM2IARBPROC           glUniform2iARB;
  PFNGLUNIFORM3IARBPROC           glUniform3iARB;
  PFNGLUNIFORM4IARBPROC           glUniform4iARB;
  PFNGLUNIFORM1FVARBPROC          glUniform1fvARB;
  PFNGLUNIFORM2FVARBPROC          glUniform2fvARB;
  PFNGLUNIFORM3FVARBPROC          glUniform3fvARB;
  PFNGLUNIFORM4FVARBPROC          glUniform4fvARB;
  PFNGLUNIFORM1IVARBPROC          glUniform1ivARB;
  PFNGLUNIFORM2IVARBPROC          glUniform2ivARB;
  PFNGLUNIFORM3IVARBPROC          glUniform3ivARB;
  PFNGLUNIFORM4IVARBPROC          glUniform4ivARB;
  PFNGLUNIFORMMATRIX2FVARBPROC    glUniformMatrix2fvARB;
  PFNGLUNIFORMMATRIX3FVARBPROC    glUniformMatrix3fvARB;
  PFNGLUNIFORMMATRIX4FVARBPROC    glUniformMatrix4fvARB;
  PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
  PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
  PFNGLGETINFOLOGARBPROC          glGetInfoLogARB;
  PFNGLGETATTACHEDOBJECTSARBPROC  glGetAttachedObjectsARB;
  PFNGLGETUNIFORMLOCATIONARBPROC  glGetUniformLocationARB;
  PFNGLGETACTIVEUNIFORMARBPROC    glGetActiveUniformARB;
  PFNGLGETUNIFORMFVARBPROC        glGetUniformfvARB;
  PFNGLGETUNIFORMIVARBPROC        glGetUniformivARB;
  PFNGLGETSHADERSOURCEARBPROC     glGetShaderSourceARB;
  /// #31 GL_ARB_vertex_shader http://www.opengl.org/registry/specs/ARB/vertex_shader.txt
  // doc includes GL_ARB_vertex_program funcs...
  PFNGLBINDATTRIBLOCATIONARBPROC  glBindAttribLocationARB;
  PFNGLGETACTIVEATTRIBARBPROC     glGetActiveAttribARB;
  PFNGLGETATTRIBLOCATIONARBPROC   glGetAttribLocationARB;
  /// #37 GL_ARB_draw_buffers http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
  PFNGLDRAWBUFFERSARBPROC glDrawBuffersARB;
  /// #39 GL_ARB_color_buffer_float WGL_ARB_pixel_format_float GLX_ARB_fbconfig_float http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
  PFNGLCLAMPCOLORARBPROC glClampColorARB;
  #ifdef OS_WIN
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
  #endif /// OS_WIN
  /// #44 GL_ARB_draw_instanced http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
  PFNGLDRAWARRAYSINSTANCEDARBPROC   glDrawArraysInstancedARB;
  PFNGLDRAWELEMENTSINSTANCEDARBPROC glDrawElementsInstancedARB;
  /// #47 GL_ARB_geometry_shader4 http://www.opengl.org/registry/specs/ARB/geometry_shader4.txt
  PFNGLPROGRAMPARAMETERIARBPROC       glProgramParameteriARB;
  PFNGLFRAMEBUFFERTEXTUREARBPROC      glFramebufferTextureARB;
  PFNGLFRAMEBUFFERTEXTURELAYERARBPROC glFramebufferTextureLayerARB;
  PFNGLFRAMEBUFFERTEXTUREFACEARBPROC  glFramebufferTextureFaceARB;
  /// #49 GL_ARB_instanced_arrays http://www.opengl.org/registry/specs/ARB/instanced_arrays.txt
  /// multiple draws of the same object, but a translate or something is still a mistery for me
  PFNGLVERTEXATTRIBDIVISORARBPROC            glVertexAttribDivisorARB;
  /// #51 GL_ARB_texture_buffer_object http://www.opengl.org/registry/specs/ARB/texture_buffer_object.txt
  PFNGLTEXBUFFERARBPROC glTexBufferARB;
  /// #55 #74 !!! WGL_ARB_create_context !!! WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
  #ifdef OS_WIN
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
  #endif /// OS_WIN
  #ifdef OS_LINUX
  /// #56 #75 GLX_ARB_create_context !!! GLX_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
  PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;
  #endif /// OS_LINUX
  /// #69 GL_ARB_draw_buffers_blend http://www.opengl.org/registry/specs/ARB/draw_buffers_blend.txt
  PFNGLBLENDEQUATIONIARBPROC          glBlendEquationiARB;
  PFNGLBLENDEQUATIONSEPARATEIARBPROC  glBlendEquationSeparateiARB;
  PFNGLBLENDFUNCIARBPROC              glBlendFunciARB;
  PFNGLBLENDFUNCSEPARATEIARBPROC      glBlendFuncSeparateiARB;
  /// #70 GL_ARB_sample_shading http://www.opengl.org/registry/specs/ARB/sample_shading.txt
  PFNGLMINSAMPLESHADINGARBPROC glMinSampleShadingARB;
  /// #76 GL_ARB_shading_language_include http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
  PFNGLNAMEDSTRINGARBPROC           glNamedStringARB;
  PFNGLDELETENAMEDSTRINGARBPROC     glDeleteNamedStringARB;
  PFNGLCOMPILESHADERINCLUDEARBPROC  glCompileShaderIncludeARB;
  PFNGLISNAMEDSTRINGARBPROC         glIsNamedStringARB;
  PFNGLGETNAMEDSTRINGARBPROC        glGetNamedStringARB;
  PFNGLGETNAMEDSTRINGIVARBPROC      glGetNamedStringivARB;
  /// #103 GL_ARB_cl_event http://www.opengl.org/registry/specs/ARB/cl_event.txt
  PFNGLCREATESYNCFROMCLEVENTARBPROC glCreateSyncFromCLeventARB;
  /// #104 GL_ARB_debug_output http://www.opengl.org/registry/specs/ARB/debug_output.txt
  PFNGLDEBUGMESSAGECONTROLARBPROC   glDebugMessageControlARB;
  PFNGLDEBUGMESSAGEINSERTARBPROC    glDebugMessageInsertARB;
  PFNGLDEBUGMESSAGECALLBACKARBPROC  glDebugMessageCallbackARB;
  PFNGLGETDEBUGMESSAGELOGARBPROC    glGetDebugMessageLogARB;
  /// #105 GL_ARB_robustness http://www.opengl.org/registry/specs/ARB/robustness.txt
  PFNGLGETGRAPHICSRESETSTATUSARBPROC  glGetGraphicsResetStatusARB;
  PFNGLGETNMAPDVARBPROC               glGetnMapdvARB;
  PFNGLGETNMAPFVARBPROC               glGetnMapfvARB;
  PFNGLGETNMAPIVARBPROC               glGetnMapivARB;
  PFNGLGETNPIXELMAPFVARBPROC          glGetnPixelMapfvARB;
  PFNGLGETNPIXELMAPUIVARBPROC         glGetnPixelMapuivARB;
  PFNGLGETNPIXELMAPUSVARBPROC         glGetnPixelMapusvARB;
  PFNGLGETNPOLYGONSTIPPLEARBPROC      glGetnPolygonStippleARB;
  PFNGLGETNTEXIMAGEARBPROC            glGetnTexImageARB;
  PFNGLREADNPIXELSARBPROC             glReadnPixelsARB;
  PFNGLGETNCOLORTABLEARBPROC          glGetnColorTableARB;
  PFNGLGETNCONVOLUTIONFILTERARBPROC   glGetnConvolutionFilterARB;
  PFNGLGETNSEPARABLEFILTERARBPROC     glGetnSeparableFilterARB;
  PFNGLGETNHISTOGRAMARBPROC           glGetnHistogramARB;
  PFNGLGETNMINMAXARBPROC              glGetnMinmaxARB;
  PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC  glGetnCompressedTexImageARB;
  PFNGLGETNUNIFORMFVARBPROC           glGetnUniformfvARB;
  PFNGLGETNUNIFORMIVARBPROC           glGetnUniformivARB;
  PFNGLGETNUNIFORMUIVARBPROC          glGetnUniformuivARB;
  PFNGLGETNUNIFORMDVARBPROC           glGetnUniformdvARB;
  /// #153 GL_ARB_compute_variable_group_size http://www.opengl.org/registry/specs/ARB/compute_variable_group_size.txt
  PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC glDispatchComputeGroupSizeARB;    /// shader stuff
  /// #154 GL_ARB_indirect_parameters http://www.opengl.org/registry/specs/ARB/indirect_parameters.txt
  PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC    glMultiDrawArraysIndirectCountARB;
  PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC  glMultiDrawElementsIndirectCountARB;
  /// #158 GL_ARB_sparse_texture http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
  PFNGLTEXPAGECOMMITMENTARBPROC glTexPageCommitmentARB;         /// texture memory allocation management

  /* !!! vvv current glext.h don't have these vvv !!!
  -------========================================---------

  /// #159 GL_ARB_ES3_1_compatibility http://www.opengl.org/registry/specs/ARB/ES3_1_compatibility.txt
   ??? glMemoryBarrierByRegion /// OpenGL ES stuff, for easier porting
  /// #160 GL_ARB_clip_control http://www.opengl.org/registry/specs/ARB/clip_control.txt
   ??? glClipControl           /// Direct3D's coordinate system & improve precision of the Z coord mapping
  /// #164 GL_ARB_direct_state_access http://www.opengl.org/registry/specs/ARB/direct_state_access.txt
  glCreateTransformFeedbacks      /// no need to bind an object to a context to modify it
  glTransformFeedbackBufferBase
  glTransformFeedbackBufferRange
  glGetTransformFeedbackiv
  glGetTransformFeedbacki_v
  glGetTransformFeedbacki64_v
  glCreateBuffers
  glNamedBufferStorage
  glNamedBufferData
  glNamedBufferSubData
  glCopyNamedBufferSubData
  glClearNamedBufferData
  glClearNamedBufferSubData
  glMapNamedBuffer
  glMapNamedBufferRange
  glUnmapNamedBuffer
  glFlushMappedNamedBufferRange
  glGetNamedBufferParameteriv
  glGetNamedBufferParameteri64v
  glGetNamedBufferPointerv
  glGetNamedBufferSubData
  glCreateFramebuffers
  glNamedFramebufferRenderbuffer
  glNamedFramebufferParameteri
  glNamedFramebufferTexture
  glNamedFramebufferTextureLayer
  glNamedFramebufferDrawBuffer
  glNamedFramebufferDrawBuffers
  glNamedFramebufferReadBuffer
  glInvalidateNamedFramebufferData
  glInvalidateNamedFramebufferSubData
  glClearNamedFramebufferiv
  glClearNamedFramebufferuiv
  glClearNamedFramebufferfv
  glClearNamedFramebufferfi
  glBlitNamedFramebuffer
  glCheckNamedFramebufferStatus
  glGetNamedFramebufferParameteriv
  glGetNamedFramebufferAttachmentParameteriv
  glCreateRenderbuffers
  glNamedRenderbufferStorage
  glNamedRenderbufferStorageMultisample
  glGetNamedRenderbufferParameteriv
  glCreateTextures
  glTextureBuffer
  glTextureBufferRange
  glTextureStorage1D
  glTextureStorage2D
  glTextureStorage3D
  glTextureStorage2DMultisample
  glTextureStorage3DMultisample
  glTextureSubImage1D
  glTextureSubImage2D
  glTextureSubImage3D
  glCompressedTextureSubImage1D
  glCompressedTextureSubImage2D
  glCompressedTextureSubImage3D
  glCopyTextureSubImage1D
  glCopyTextureSubImage2D
  glCopyTextureSubImage3D
  glTextureParameterf
  glTextureParameterfv
  glTextureParameteri
  glTextureParameterIiv
  glTextureParameterIuiv
  glTextureParameteriv
  glGenerateTextureMipmap
  glBindTextureUnit
  glGetTextureImage
  glGetCompressedTextureImage
  glGetTextureLevelParameterfv
  glGetTextureLevelParameteriv
  glGetTextureParameterfv
  glGetTextureParameterIiv
  glGetTextureParameterIuiv
  glGetTextureParameteriv
  glCreateVertexArrays
  glDisableVertexArrayAttrib
  glEnableVertexArrayAttrib
  glVertexArrayElementBuffer
  glVertexArrayVertexBuffer
  glVertexArrayVertexBuffers
  glVertexArrayAttribFormat
  glVertexArrayAttribIFormat
  glVertexArrayAttribLFormat
  glVertexArrayAttribBinding
  glVertexArrayBindingDivisor
  glGetVertexArrayiv
  glGetVertexArrayIndexediv
  glGetVertexArrayIndexed64iv
  glCreateSamplers
  glCreateProgramPipelines
  glCreateQueries
  
  /// #165 GL_ARB_get_texture_sub_image http://www.opengl.org/registry/specs/ARB/get_texture_sub_image.txt
  glGetTextureSubImage            /// funcs to get sub-regions of textures
  glGetCompressedTextureSubImage
  /// #167 GL_ARB_texture_barrier http://www.opengl.org/registry/specs/ARB/texture_barrier.txt
  glTextureBarrier /// relaxes restrictions on rendering to a bound tex & mechanism to avoid read-after-write hazards.
  /// #170 GL_KHR_robustness http://www.opengl.org/registry/specs/KHR/robustness.txt
  // These funcs are already in GL_ARB_robustness ...
  glGetGraphicsResetStatus      /// fast check of the context status (crashed, reset etc)
  glReadnPixels                 /// safe checks, if the gr card accessed out of bounds stuff
  glGetnUniformfv
  glGetnUniformiv
  glGetnUniformuiv
  /// #172 GL_ARB_sparse_buffer http://www.opengl.org/registry/specs/ARB/sparse_buffer.txt
  /// ' create partially populated textures that would over-subscribe available graphics memory if made fully resident' ... or something, as Butthead would say
  glBufferPageCommitmentARB
 
  !!! ^^^ current glext.h don't have these ^^^ !!! */  









  // per EXT/vendor extension functions list ==================------------------------------------
  ///========================================------------------------------------------------------

// EXT & vendor list ===================-------------------------------------------
  /// GL_EXT_blend_color         // #2 http://www.opengl.org/registry/specs/EXT/blend_color.txt
  PFNGLBLENDCOLOREXTPROC glBlendColorEXT;
  /// GL_EXT_polygon_offset      // #3 http://www.opengl.org/registry/specs/EXT/polygon_offset.txt
  PFNGLPOLYGONOFFSETEXTPROC glPolygonOffsetEXT;
  /// GL_EXT_texture3D           // #6 http://www.opengl.org/registry/specs/EXT/texture3D.txt
  PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
  PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT;
  /// GL_SGIS_texture_filter4    // #7 http://www.opengl.org/registry/specs/SGIS/texture_filter4.txt
  PFNGLGETTEXFILTERFUNCSGISPROC glGetTexFilterFuncSGIS;
  PFNGLTEXFILTERFUNCSGISPROC glTexFilterFuncSGIS;
  /// GL_EXT_subtexture          // #9 http://www.opengl.org/registry/specs/EXT/subtexture.txt
  PFNGLTEXSUBIMAGE1DEXTPROC glTexSubImage1DEXT;
  PFNGLTEXSUBIMAGE2DEXTPROC glTexSubImage2DEXT;
  /// GL_EXT_copy_texture        // #10 http://www.opengl.org/registry/specs/EXT/copy_texture.txt
  PFNGLCOPYTEXIMAGE1DEXTPROC glCopyTexImage1DEXT;
  PFNGLCOPYTEXIMAGE2DEXTPROC glCopyTexImage2DEXT;
  PFNGLCOPYTEXSUBIMAGE1DEXTPROC glCopyTexSubImage1DEXT;
  PFNGLCOPYTEXSUBIMAGE2DEXTPROC glCopyTexSubImage2DEXT;
  PFNGLCOPYTEXSUBIMAGE3DEXTPROC glCopyTexSubImage3DEXT;
  /// GL_EXT_histogram           // #11 http://www.opengl.org/registry/specs/EXT/histogram.txt
  PFNGLGETHISTOGRAMEXTPROC glGetHistogramEXT;
  PFNGLGETHISTOGRAMPARAMETERFVEXTPROC glGetHistogramParameterfvEXT;
  PFNGLGETHISTOGRAMPARAMETERIVEXTPROC glGetHistogramParameterivEXT;
  PFNGLGETMINMAXEXTPROC glGetMinmaxEXT;
  PFNGLGETMINMAXPARAMETERFVEXTPROC glGetMinmaxParameterfvEXT;
  PFNGLGETMINMAXPARAMETERIVEXTPROC glGetMinmaxParameterivEXT;
  PFNGLHISTOGRAMEXTPROC glHistogramEXT;
  PFNGLMINMAXEXTPROC glMinmaxEXT;
  PFNGLRESETHISTOGRAMEXTPROC glResetHistogramEXT;
  PFNGLRESETMINMAXEXTPROC glResetMinmaxEXT;
  /// GL_EXT_convolution         // #12 http://www.opengl.org/registry/specs/EXT/convolution.txt
  PFNGLCONVOLUTIONFILTER1DEXTPROC glConvolutionFilter1DEXT;
  PFNGLCONVOLUTIONFILTER2DEXTPROC glConvolutionFilter2DEXT;
  PFNGLCONVOLUTIONPARAMETERFEXTPROC glConvolutionParameterfEXT;
  PFNGLCONVOLUTIONPARAMETERFVEXTPROC glConvolutionParameterfvEXT;
  PFNGLCONVOLUTIONPARAMETERIEXTPROC glConvolutionParameteriEXT;
  PFNGLCONVOLUTIONPARAMETERIVEXTPROC glConvolutionParameterivEXT;
  PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC glCopyConvolutionFilter1DEXT;
  PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC glCopyConvolutionFilter2DEXT;
  PFNGLGETCONVOLUTIONFILTEREXTPROC glGetConvolutionFilterEXT;
  PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC glGetConvolutionParameterfvEXT;
  PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC glGetConvolutionParameterivEXT;
  PFNGLGETSEPARABLEFILTEREXTPROC glGetSeparableFilterEXT;
  PFNGLSEPARABLEFILTER2DEXTPROC glSeparableFilter2DEXT;
  /// GL_SGI_color_table         // #14 http://www.opengl.org/registry/specs/SGI/color_table.txt
  PFNGLCOLORTABLESGIPROC glColorTableSGI;
  PFNGLCOLORTABLEPARAMETERFVSGIPROC glColorTableParameterfvSGI;
  PFNGLCOLORTABLEPARAMETERIVSGIPROC glColorTableParameterivSGI;
  PFNGLCOPYCOLORTABLESGIPROC glCopyColorTableSGI;
  PFNGLGETCOLORTABLESGIPROC glGetColorTableSGI;
  PFNGLGETCOLORTABLEPARAMETERFVSGIPROC glGetColorTableParameterfvSGI;
  PFNGLGETCOLORTABLEPARAMETERIVSGIPROC glGetColorTableParameterivSGI;
  /// GL_SGIS_pixel_texture      // GL_SGIX_pixel_texture #15 & #15a http://www.opengl.org/registry/specs/SGIS/pixel_texture.txt http://www.opengl.org/registry/specs/SGIX/sgix_pixel_texture.txt
  PFNGLPIXELTEXGENPARAMETERISGISPROC glPixelTexGenParameteriSGIS;
  PFNGLPIXELTEXGENPARAMETERIVSGISPROC glPixelTexGenParameterivSGIS;
  PFNGLPIXELTEXGENPARAMETERFSGISPROC glPixelTexGenParameterfSGIS;
  PFNGLPIXELTEXGENPARAMETERFVSGISPROC glPixelTexGenParameterfvSGIS;
  PFNGLGETPIXELTEXGENPARAMETERIVSGISPROC glGetPixelTexGenParameterivSGIS;
  PFNGLGETPIXELTEXGENPARAMETERFVSGISPROC glGetPixelTexGenParameterfvSGIS;
  PFNGLPIXELTEXGENSGIXPROC glPixelTexGenSGIX; /// GL_SGIX_pixel_texture
  /// GL_SGIS_texture4D          // #16 http://www.opengl.org/registry/specs/SGIS/texture4D.txt
  PFNGLTEXIMAGE4DSGISPROC glTexImage4DSGIS;
  PFNGLTEXSUBIMAGE4DSGISPROC glTexSubImage4DSGIS;
  /// GL_EXT_texture_object      // #20 http://www.opengl.org/registry/specs/EXT/texture_object.txt
  PFNGLARETEXTURESRESIDENTEXTPROC glAreTexturesResidentEXT;
  PFNGLBINDTEXTUREEXTPROC glBindTextureEXT;
  PFNGLDELETETEXTURESEXTPROC glDeleteTexturesEXT;
  PFNGLGENTEXTURESEXTPROC glGenTexturesEXT;
  PFNGLISTEXTUREEXTPROC glIsTextureEXT;
  PFNGLPRIORITIZETEXTURESEXTPROC glPrioritizeTexturesEXT;
  /// GL_SGIS_detail_texture     // #21 http://www.opengl.org/registry/specs/SGIS/detail_texture.txt
  PFNGLDETAILTEXFUNCSGISPROC glDetailTexFuncSGIS;
  PFNGLGETDETAILTEXFUNCSGISPROC glGetDetailTexFuncSGIS;
  /// GL_SGIS_sharpen_texture    // #22 http://www.opengl.org/registry/specs/SGIS/sharpen_texture.txt
  PFNGLSHARPENTEXFUNCSGISPROC glSharpenTexFuncSGIS;
  PFNGLGETSHARPENTEXFUNCSGISPROC glGetSharpenTexFuncSGIS;
  /// GL_SGIS_multisample        // #25 GLX_SGIS_multisample http://www.opengl.org/registry/specs/SGIS/multisample.txt
  PFNGLSAMPLEMASKSGISPROC glSampleMaskSGIS;
  PFNGLSAMPLEPATTERNSGISPROC glSamplePatternSGIS;
  /// GL_EXT_vertex_array        // #30 http://www.opengl.org/registry/specs/EXT/vertex_array.txt
  PFNGLARRAYELEMENTEXTPROC glArrayElementEXT;
  PFNGLCOLORPOINTEREXTPROC glColorPointerEXT;
  PFNGLDRAWARRAYSEXTPROC glDrawArraysEXT;
  PFNGLEDGEFLAGPOINTEREXTPROC glEdgeFlagPointerEXT;
  PFNGLGETPOINTERVEXTPROC glGetPointervEXT;
  PFNGLINDEXPOINTEREXTPROC glIndexPointerEXT;
  PFNGLNORMALPOINTEREXTPROC glNormalPointerEXT;
  PFNGLTEXCOORDPOINTEREXTPROC glTexCoordPointerEXT;
  PFNGLVERTEXPOINTEREXTPROC glVertexPointerEXT;
  /// GL_EXT_blend_minmax        // #37 http://www.opengl.org/registry/specs/EXT/blend_minmax.txt
  PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;
  #ifdef OS_LINUX
  /// GLX_SGI_swap_control       // #40 http://www.opengl.org/registry/specs/SGI/swap_control.txt
  PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI;
  /// GLX_SGI_video_sync         // #41 http://www.opengl.org/registry/specs/SGI/video_sync.txt
  PFNGLXGETVIDEOSYNCSGIPROC glXGetVideoSyncSGI;
  PFNGLXWAITVIDEOSYNCSGIPROC glXWaitVideoSyncSGI;
  /// GLX_SGI_make_current_read  // #42 http://www.opengl.org/registry/specs/SGI/make_current_read.txt
  PFNGLXMAKECURRENTREADSGIPROC glXMakeCurrentReadSGI;
  PFNGLXGETCURRENTREADDRAWABLESGIPROC glXGetCurrentReadDrawableSGI;
  /// GLX_SGIX_video_source      // #43 http://www.opengl.org/registry/specs/SGIX/video_source.txt
  #ifdef _VL_H
  PFNGLXCREATEGLXVIDEOSOURCESGIXPROC glXCreateGLXVideoSourceSGIX;
  PFNGLXDESTROYGLXVIDEOSOURCESGIXPROC glXDestroyGLXVideoSourceSGIX;
  #endif /// _VL_H - dunno what this is...
  /// GLX_EXT_import_context     // #47 http://www.opengl.org/registry/specs/EXT/import_context.txt
  PFNGLXGETCURRENTDISPLAYEXTPROC glXGetCurrentDisplayEXT;
  PFNGLXQUERYCONTEXTINFOEXTPROC glXQueryContextInfoEXT;
  PFNGLXGETCONTEXTIDEXTPROC glXGetContextIDEXT;
  PFNGLXIMPORTCONTEXTEXTPROC glXImportContextEXT;
  PFNGLXFREECONTEXTEXTPROC glXFreeContextEXT;
  /// GLX_SGIX_fbconfig          // #49 http://www.opengl.org/registry/specs/SGIX/fbconfig.txt
  PFNGLXGETFBCONFIGATTRIBSGIXPROC glXGetFBConfigAttribSGIX;
  PFNGLXCHOOSEFBCONFIGSGIXPROC glXChooseFBConfigSGIX;
  PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC glXCreateGLXPixmapWithConfigSGIX;
  PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC glXCreateContextWithConfigSGIX;
  PFNGLXGETVISUALFROMFBCONFIGSGIXPROC glXGetVisualFromFBConfigSGIX;
  PFNGLXGETFBCONFIGFROMVISUALSGIXPROC glXGetFBConfigFromVisualSGIX;
  /// GLX_SGIX_pbuffer           // #50 http://www.opengl.org/registry/specs/SGIX/pbuffer.txt
  PFNGLXCREATEGLXPBUFFERSGIXPROC glXCreateGLXPbufferSGIX;
  PFNGLXDESTROYGLXPBUFFERSGIXPROC glXDestroyGLXPbufferSGIX;
  PFNGLXQUERYGLXPBUFFERSGIXPROC glXQueryGLXPbufferSGIX;
  PFNGLXSELECTEVENTSGIXPROC glXSelectEventSGIX;
  PFNGLXGETSELECTEDEVENTSGIXPROC glXGetSelectedEventSGIX;
  #endif
  /// GL_SGIX_sprite             // #52 http://www.opengl.org/registry/specs/SGIX/sprite.txt
  PFNGLSPRITEPARAMETERFSGIXPROC glSpriteParameterfSGIX;
  PFNGLSPRITEPARAMETERFVSGIXPROC glSpriteParameterfvSGIX;
  PFNGLSPRITEPARAMETERISGIXPROC glSpriteParameteriSGIX;
  PFNGLSPRITEPARAMETERIVSGIXPROC glSpriteParameterivSGIX;
  /// GL_EXT_point_parameters    // #54 http://www.opengl.org/registry/specs/EXT/point_parameters.txt
  PFNGLPOINTPARAMETERFEXTPROC glPointParameterfEXT;
  PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvEXT;
  /// GL_SGIX_instruments        // #55 http://www.opengl.org/registry/specs/SGIX/instruments.txt
  PFNGLGETINSTRUMENTSSGIXPROC glGetInstrumentsSGIX;
  PFNGLINSTRUMENTSBUFFERSGIXPROC glInstrumentsBufferSGIX;
  PFNGLPOLLINSTRUMENTSSGIXPROC glPollInstrumentsSGIX;
  PFNGLREADINSTRUMENTSSGIXPROC glReadInstrumentsSGIX;
  PFNGLSTARTINSTRUMENTSSGIXPROC glStartInstrumentsSGIX;
  PFNGLSTOPINSTRUMENTSSGIXPROC glStopInstrumentsSGIX;
  /// GL_SGIX_framezoom          // #57 http://www.opengl.org/registry/specs/SGIX/framezoom.txt
  PFNGLFRAMEZOOMSGIXPROC glFrameZoomSGIX;
  /// GL_SGIX_tag_sample_buffer  // #58 http://www.opengl.org/registry/specs/SGIX/tag_sample_buffer.txt
  PFNGLTAGSAMPLEBUFFERSGIXPROC glTagSampleBufferSGIX;
  /// GL_SGIX_reference_plane    // #60 http://www.opengl.org/registry/specs/SGIX/reference_plane.txt
  PFNGLREFERENCEPLANESGIXPROC glReferencePlaneSGIX;
  /// GL_SGIX_flush_raster       // #61 http://www.opengl.org/registry/specs/SGIX/flush_raster.txt
  PFNGLFLUSHRASTERSGIXPROC glFlushRasterSGIX;
  #ifdef OS_LINUX
  /// GLX_SGI_cushion            // #62 http://www.opengl.org/registry/specs/SGI/cushion.txt
  PFNGLXCUSHIONSGIPROC glXCushionSGI;
  #endif
  /// GL_SGIS_fog_function       // #64 http://www.opengl.org/registry/specs/SGIS/fog_func.txt
  PFNGLFOGFUNCSGISPROC glFogFuncSGIS;
  PFNGLGETFOGFUNCSGISPROC glGetFogFuncSGIS;
  /// GL_HP_image_transform      // #66 http://www.opengl.org/registry/specs/HP/image_transform.txt
  PFNGLIMAGETRANSFORMPARAMETERIHPPROC glImageTransformParameteriHP;
  PFNGLIMAGETRANSFORMPARAMETERFHPPROC glImageTransformParameterfHP;
  PFNGLIMAGETRANSFORMPARAMETERIVHPPROC glImageTransformParameterivHP;
  PFNGLIMAGETRANSFORMPARAMETERFVHPPROC glImageTransformParameterfvHP;
  PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC glGetImageTransformParameterivHP;
  PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC glGetImageTransformParameterfvHP;
  /// GL_EXT_color_subtable      // #74 http://www.opengl.org/registry/specs/EXT/color_subtable.txt
  PFNGLCOLORSUBTABLEEXTPROC glColorSubTableEXT;
  PFNGLCOPYCOLORSUBTABLEEXTPROC glCopyColorSubTableEXT;
  /// GL_PGI_misc_hints          // #77 http://www.opengl.org/registry/specs/PGI/misc_hints.txt
  PFNGLHINTPGIPROC glHintPGI;
  /// GL_EXT_paletted_texture    // #78 http://www.opengl.org/registry/specs/EXT/paletted_texture.txt
  PFNGLCOLORTABLEEXTPROC glColorTableEXT;
  PFNGLGETCOLORTABLEEXTPROC glGetColorTableEXT;
  PFNGLGETCOLORTABLEPARAMETERIVEXTPROC glGetColorTableParameterivEXT;
  PFNGLGETCOLORTABLEPARAMETERFVEXTPROC glGetColorTableParameterfvEXT;
  /// GL_SGIX_list_priority      // #80 incomplete http://www.opengl.org/registry/specs/SGIX/list_priority.txt
  PFNGLGETLISTPARAMETERFVSGIXPROC glGetListParameterfvSGIX;
  PFNGLGETLISTPARAMETERIVSGIXPROC glGetListParameterivSGIX;
  PFNGLLISTPARAMETERFSGIXPROC glListParameterfSGIX;
  PFNGLLISTPARAMETERFVSGIXPROC glListParameterfvSGIX;
  PFNGLLISTPARAMETERISGIXPROC glListParameteriSGIX;
  PFNGLLISTPARAMETERIVSGIXPROC glListParameterivSGIX;
  #ifdef OS_LINUX
  /// GLX_SGIX_video_resize      // #83 http://www.opengl.org/registry/specs/SGIX/video_resize.txt
  PFNGLXBINDCHANNELTOWINDOWSGIXPROC glXBindChannelToWindowSGIX;
  PFNGLXCHANNELRECTSGIXPROC glXChannelRectSGIX;
  PFNGLXQUERYCHANNELRECTSGIXPROC glXQueryChannelRectSGIX;
  PFNGLXQUERYCHANNELDELTASSGIXPROC glXQueryChannelDeltasSGIX;
  PFNGLXCHANNELRECTSYNCSGIXPROC glXChannelRectSyncSGIX;
  /// GLX_SGIX_dm_buffer         // #86 incomplete !!!GLX_SGIX_dmbuffer!!! http://www.opengl.org/registry/specs/SGIX/dmbuffer.txt
  #ifdef _DM_BUFFER_H_
  PFNGLXASSOCIATEDMPBUFFERSGIXPROC glXAssociateDMPbufferSGIX;
  #endif /// _DM_BUFFER_H_ - no clue 'bout this header
  /// GLX_SGIX_swap_group          // #91 http://www.opengl.org/registry/specs/SGIX/swap_group.txt
  PFNGLXJOINSWAPGROUPSGIXPROC glXJoinSwapGroupSGIX;
  /// GLX_SGIX_swap_barrier        // #92 http://www.opengl.org/registry/specs/SGIX/swap_barrier.txt
  PFNGLXBINDSWAPBARRIERSGIXPROC glXBindSwapBarrierSGIX;
  PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC glXQueryMaxSwapBarriersSGIX;
  #endif
  /// GL_EXT_index_material        // #94 http://www.opengl.org/registry/specs/EXT/index_material.txt
  PFNGLINDEXMATERIALEXTPROC glIndexMaterialEXT;
  /// GL_EXT_index_func            // #95 http://www.opengl.org/registry/specs/EXT/index_func.txt
  PFNGLINDEXFUNCEXTPROC glIndexFuncEXT;
  /// GL_EXT_compiled_vertex_array  // #97 incomplete http://www.opengl.org/registry/specs/EXT/compiled_vertex_array.txt
  PFNGLLOCKARRAYSEXTPROC glLockArraysEXT;
  PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT;
  /// GL_EXT_cull_vertex           // #98 incomplete http://www.opengl.org/registry/specs/EXT/cull_vertex.txt
  PFNGLCULLPARAMETERDVEXTPROC glCullParameterdvEXT;
  PFNGLCULLPARAMETERFVEXTPROC glCullParameterfvEXT;
  // ?? /// GLU_EXT_nurbs_tessellator    // #100 http://www.opengl.org/registry/specs/EXT/nurbs_tessellator.txt
  /// GL_EXT_fragment_lighting     // #102 inc http://www.opengl.org/registry/specs/EXT/fragment_lighting.txt
  // has funcs but none in glext.h ATM
  /// GL_EXT_draw_range_elements   // #112 http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
  PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT;
  /// GL_EXT_light_texture         // #117 http://www.opengl.org/registry/specs/EXT/light_texture.txt
  PFNGLAPPLYTEXTUREEXTPROC glApplyTextureEXT;
  PFNGLTEXTURELIGHTEXTPROC glTextureLightEXT;
  PFNGLTEXTUREMATERIALEXTPROC glTextureMaterialEXT;
  /// GL_EXT_scene_marker          // #120 GLX_EXT_scene_marker http://www.opengl.org/registry/specs/EXT/scene_marker.txt
  // some funcs but not in glext.h ATM
  /// GL_SGIX_async                // #132 incomplete http://www.opengl.org/registry/specs/SGIX/async.txt
  PFNGLASYNCMARKERSGIXPROC glAsyncMarkerSGIX;
  PFNGLFINISHASYNCSGIXPROC glFinishAsyncSGIX;
  PFNGLPOLLASYNCSGIXPROC glPollAsyncSGIX;
  PFNGLGENASYNCMARKERSSGIXPROC glGenAsyncMarkersSGIX;
  PFNGLDELETEASYNCMARKERSSGIXPROC glDeleteAsyncMarkersSGIX;
  PFNGLISASYNCMARKERSGIXPROC glIsAsyncMarkerSGIX;
  /// GL_INTEL_parallel_arrays     // #136 http://www.opengl.org/registry/specs/INTEL/parallel_arrays.txt
  PFNGLVERTEXPOINTERVINTELPROC glVertexPointervINTEL;
  PFNGLNORMALPOINTERVINTELPROC glNormalPointervINTEL;
  PFNGLCOLORPOINTERVINTELPROC glColorPointervINTEL;
  PFNGLTEXCOORDPOINTERVINTELPROC glTexCoordPointervINTEL;
  /// GL_EXT_pixel_transform       // #138 http://www.opengl.org/registry/specs/EXT/pixel_transform.txt
  PFNGLPIXELTRANSFORMPARAMETERIEXTPROC glPixelTransformParameteriEXT;
  PFNGLPIXELTRANSFORMPARAMETERFEXTPROC glPixelTransformParameterfEXT;
  PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC glPixelTransformParameterivEXT;
  PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC glPixelTransformParameterfvEXT;
  PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC glGetPixelTransformParameterivEXT;
  PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC glGetPixelTransformParameterfvEXT;
  /// GL_EXT_secondary_color       // #145 http://www.opengl.org/registry/specs/EXT/secondary_color.txt
  PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT;
  PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT;
  PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT;
  PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT;
  PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT;
  PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT;
  PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT;
  PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT;
  PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT;
  PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT;
  PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT;
  PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT;
  PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT;
  PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT;
  PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT;
  PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT;
  PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT;
  /// GL_EXT_texture_perturb_normal  // #147 http://www.opengl.org/registry/specs/EXT/texture_perturb_normal.txt
  PFNGLTEXTURENORMALEXTPROC glTextureNormalEXT;
  /// GL_EXT_multi_draw_arrays     // GL_SUN_multi_draw_arrays #148 http://www.opengl.org/registry/specs/EXT/multi_draw_arrays.txt
  PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArraysEXT;
  PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElementsEXT;
  /// GL_EXT_fog_coord             // #149 http://www.opengl.org/registry/specs/EXT/fog_coord.txt
  PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
  PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT;
  PFNGLFOGCOORDDEXTPROC glFogCoorddEXT;
  PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT;
  PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT;
  /// GL_EXT_coordinate_frame      // #156 inc http://www.opengl.org/registry/specs/EXT/coordinate_frame.txt
  PFNGLTANGENT3BEXTPROC glTangent3bEXT;
  PFNGLTANGENT3BVEXTPROC glTangent3bvEXT;
  PFNGLTANGENT3DEXTPROC glTangent3dEXT;
  PFNGLTANGENT3DVEXTPROC glTangent3dvEXT;
  PFNGLTANGENT3FEXTPROC glTangent3fEXT;
  PFNGLTANGENT3FVEXTPROC glTangent3fvEXT;
  PFNGLTANGENT3IEXTPROC glTangent3iEXT;
  PFNGLTANGENT3IVEXTPROC glTangent3ivEXT;
  PFNGLTANGENT3SEXTPROC glTangent3sEXT;
  PFNGLTANGENT3SVEXTPROC glTangent3svEXT;
  PFNGLBINORMAL3BEXTPROC glBinormal3bEXT;
  PFNGLBINORMAL3BVEXTPROC glBinormal3bvEXT;
  PFNGLBINORMAL3DEXTPROC glBinormal3dEXT;
  PFNGLBINORMAL3DVEXTPROC glBinormal3dvEXT;
  PFNGLBINORMAL3FEXTPROC glBinormal3fEXT;
  PFNGLBINORMAL3FVEXTPROC glBinormal3fvEXT;
  PFNGLBINORMAL3IEXTPROC glBinormal3iEXT;
  PFNGLBINORMAL3IVEXTPROC glBinormal3ivEXT;
  PFNGLBINORMAL3SEXTPROC glBinormal3sEXT;
  PFNGLBINORMAL3SVEXTPROC glBinormal3svEXT;
  PFNGLTANGENTPOINTEREXTPROC glTangentPointerEXT;
  PFNGLBINORMALPOINTEREXTPROC glBinormalPointerEXT;
  /// GL_SUNX_constant_data        // #163 http://www.opengl.org/registry/specs/SUNX/constant_data.txt
  PFNGLFINISHTEXTURESUNXPROC glFinishTextureSUNX;
  /// GL_SUN_global_alpha          // #164 http://www.opengl.org/registry/specs/SUN/global_alpha.txt
  PFNGLGLOBALALPHAFACTORBSUNPROC glGlobalAlphaFactorbSUN;
  PFNGLGLOBALALPHAFACTORSSUNPROC glGlobalAlphaFactorsSUN;
  PFNGLGLOBALALPHAFACTORISUNPROC glGlobalAlphaFactoriSUN;
  PFNGLGLOBALALPHAFACTORFSUNPROC glGlobalAlphaFactorfSUN;
  PFNGLGLOBALALPHAFACTORDSUNPROC glGlobalAlphaFactordSUN;
  PFNGLGLOBALALPHAFACTORUBSUNPROC glGlobalAlphaFactorubSUN;
  PFNGLGLOBALALPHAFACTORUSSUNPROC glGlobalAlphaFactorusSUN;
  PFNGLGLOBALALPHAFACTORUISUNPROC glGlobalAlphaFactoruiSUN;
  /// GL_SUN_triangle_list         // #165 http://www.opengl.org/registry/specs/SUN/triangle_list.txt
  PFNGLREPLACEMENTCODEUISUNPROC glReplacementCodeuiSUN;
  PFNGLREPLACEMENTCODEUSSUNPROC glReplacementCodeusSUN;
  PFNGLREPLACEMENTCODEUBSUNPROC glReplacementCodeubSUN;
  PFNGLREPLACEMENTCODEUIVSUNPROC glReplacementCodeuivSUN;
  PFNGLREPLACEMENTCODEUSVSUNPROC glReplacementCodeusvSUN;
  PFNGLREPLACEMENTCODEUBVSUNPROC glReplacementCodeubvSUN;
  PFNGLREPLACEMENTCODEPOINTERSUNPROC glReplacementCodePointerSUN;
  /// GL_SUN_vertex                // #166 http://www.opengl.org/registry/specs/SUN/vertex.txt
  PFNGLCOLOR4UBVERTEX2FSUNPROC glColor4ubVertex2fSUN;
  PFNGLCOLOR4UBVERTEX2FVSUNPROC glColor4ubVertex2fvSUN;
  PFNGLCOLOR4UBVERTEX3FSUNPROC glColor4ubVertex3fSUN;
  PFNGLCOLOR4UBVERTEX3FVSUNPROC glColor4ubVertex3fvSUN;
  PFNGLCOLOR3FVERTEX3FSUNPROC glColor3fVertex3fSUN;
  PFNGLCOLOR3FVERTEX3FVSUNPROC glColor3fVertex3fvSUN;
  PFNGLNORMAL3FVERTEX3FSUNPROC glNormal3fVertex3fSUN;
  PFNGLNORMAL3FVERTEX3FVSUNPROC glNormal3fVertex3fvSUN;
  PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC glColor4fNormal3fVertex3fSUN;
  PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC glColor4fNormal3fVertex3fvSUN;
  PFNGLTEXCOORD2FVERTEX3FSUNPROC glTexCoord2fVertex3fSUN;
  PFNGLTEXCOORD2FVERTEX3FVSUNPROC glTexCoord2fVertex3fvSUN;
  PFNGLTEXCOORD4FVERTEX4FSUNPROC glTexCoord4fVertex4fSUN;
  PFNGLTEXCOORD4FVERTEX4FVSUNPROC glTexCoord4fVertex4fvSUN;
  PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC glTexCoord2fColor4ubVertex3fSUN;
  PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC glTexCoord2fColor4ubVertex3fvSUN;
  PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC glTexCoord2fColor3fVertex3fSUN;
  PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC glTexCoord2fColor3fVertex3fvSUN;
  PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC glTexCoord2fNormal3fVertex3fSUN;
  PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC glTexCoord2fNormal3fVertex3fvSUN;
  PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC glTexCoord2fColor4fNormal3fVertex3fSUN;
  PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC glTexCoord2fColor4fNormal3fVertex3fvSUN;
  PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC glTexCoord4fColor4fNormal3fVertex4fSUN;
  PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC glTexCoord4fColor4fNormal3fVertex4fvSUN;
  PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC glReplacementCodeuiVertex3fSUN;
  PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC glReplacementCodeuiVertex3fvSUN;
  PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC glReplacementCodeuiColor4ubVertex3fSUN;
  PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC glReplacementCodeuiColor4ubVertex3fvSUN;
  PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC glReplacementCodeuiColor3fVertex3fSUN;
  PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC glReplacementCodeuiColor3fVertex3fvSUN;
  PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC glReplacementCodeuiNormal3fVertex3fSUN;
  PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiNormal3fVertex3fvSUN;
  PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fSUN;
  PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fvSUN;
  PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC glReplacementCodeuiTexCoord2fVertex3fSUN;
  PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC glReplacementCodeuiTexCoord2fVertex3fvSUN;
  PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN;
  PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN;
  PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN;
  PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN;
  #ifdef OS_WIN
  /// WGL_EXT_display_color_table  // #167 inc http://www.opengl.org/registry/specs/EXT/wgl_display_color_table.txt
  PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC wglCreateDisplayColorTableEXT;
  PFNWGLLOADDISPLAYCOLORTABLEEXTPROC wglLoadDisplayColorTableEXT;
  PFNWGLBINDDISPLAYCOLORTABLEEXTPROC wglBindDisplayColorTableEXT;
  PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC wglDestroyDisplayColorTableEXT;
  /// WGL_EXT_extensions_string    // #168 http://www.opengl.org/registry/specs/EXT/wgl_extensions_string.txt
  PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;
  /// WGL_EXT_make_current_read    // #169 http://www.opengl.org/registry/specs/EXT/wgl_make_current_read.txt
  PFNWGLMAKECONTEXTCURRENTEXTPROC wglMakeContextCurrentEXT;
  PFNWGLGETCURRENTREADDCEXTPROC wglGetCurrentReadDCEXT;
  /// WGL_EXT_pixel_format         // #170 http://www.opengl.org/registry/specs/EXT/wgl_pixel_format.txt
  PFNWGLGETPIXELFORMATATTRIBIVEXTPROC wglGetPixelFormatAttribivEXT;
  PFNWGLGETPIXELFORMATATTRIBFVEXTPROC wglGetPixelFormatAttribfvEXT;
  PFNWGLCHOOSEPIXELFORMATEXTPROC wglChoosePixelFormatEXT;
  /// WGL_EXT_pbuffer              // #171 http://www.opengl.org/registry/specs/EXT/wgl_pbuffer.txt
  PFNWGLCREATEPBUFFEREXTPROC wglCreatePbufferEXT;
  PFNWGLGETPBUFFERDCEXTPROC wglGetPbufferDCEXT;
  PFNWGLRELEASEPBUFFERDCEXTPROC wglReleasePbufferDCEXT;
  PFNWGLDESTROYPBUFFEREXTPROC wglDestroyPbufferEXT;
  PFNWGLQUERYPBUFFEREXTPROC wglQueryPbufferEXT;
  /// WGL_EXT_swap_control         // #172 http://www.opengl.org/registry/specs/EXT/wgl_swap_control.txt
  PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
  PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
  #endif
  /// GL_EXT_blend_func_separate   // #173 http://www.opengl.org/registry/specs/EXT/blend_func_separate.txt
  PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT;
  #ifdef OS_LINUX
  /// GLX_SUN_get_transparent_index  // #183 http://www.opengl.org/registry/specs/SUN/get_transparent_index.txt
  PFNGLXGETTRANSPARENTINDEXSUNPROC glXGetTransparentIndexSUN;
  #endif
  /// GL_EXT_vertex_weighting      // #188 http://www.opengl.org/registry/specs/EXT/vertex_weighting.txt
  PFNGLVERTEXWEIGHTFEXTPROC glVertexWeightfEXT;
  PFNGLVERTEXWEIGHTFVEXTPROC glVertexWeightfvEXT;
  PFNGLVERTEXWEIGHTPOINTEREXTPROC glVertexWeightPointerEXT;
  /// GL_NV_vertex_array_range     // #190 http://www.opengl.org/registry/specs/NV/vertex_array_range.txt
  PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV;
  PFNGLVERTEXARRAYRANGENVPROC glVertexArrayRangeNV;
  /// GL_NV_register_combiners     // #191 http://www.opengl.org/registry/specs/NV/register_combiners.txt
  PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV;
  PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV;
  PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV;
  PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV;
  PFNGLCOMBINERINPUTNVPROC glCombinerInputNV;
  PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV;
  PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV;
  PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC glGetCombinerInputParameterfvNV;
  PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC glGetCombinerInputParameterivNV;
  PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC glGetCombinerOutputParameterfvNV;
  PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC glGetCombinerOutputParameterivNV;
  PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV;
  PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV;
  /// GL_MESA_resize_buffers       // #196 http://www.opengl.org/registry/specs/MESA/resize_buffers.txt
  PFNGLRESIZEBUFFERSMESAPROC glResizeBuffersMESA;
  /// GL_MESA_window_pos           // #197 http://www.opengl.org/registry/specs/MESA/window_pos.txt
  PFNGLWINDOWPOS2DMESAPROC glWindowPos2dMESA;
  PFNGLWINDOWPOS2DVMESAPROC glWindowPos2dvMESA;
  PFNGLWINDOWPOS2FMESAPROC glWindowPos2fMESA;
  PFNGLWINDOWPOS2FVMESAPROC glWindowPos2fvMESA;
  PFNGLWINDOWPOS2IMESAPROC glWindowPos2iMESA;
  PFNGLWINDOWPOS2IVMESAPROC glWindowPos2ivMESA;
  PFNGLWINDOWPOS2SMESAPROC glWindowPos2sMESA;
  PFNGLWINDOWPOS2SVMESAPROC glWindowPos2svMESA;
  PFNGLWINDOWPOS3DMESAPROC glWindowPos3dMESA;
  PFNGLWINDOWPOS3DVMESAPROC glWindowPos3dvMESA;
  PFNGLWINDOWPOS3FMESAPROC glWindowPos3fMESA;
  PFNGLWINDOWPOS3FVMESAPROC glWindowPos3fvMESA;
  PFNGLWINDOWPOS3IMESAPROC glWindowPos3iMESA;
  PFNGLWINDOWPOS3IVMESAPROC glWindowPos3ivMESA;
  PFNGLWINDOWPOS3SMESAPROC glWindowPos3sMESA;
  PFNGLWINDOWPOS3SVMESAPROC glWindowPos3svMESA;
  PFNGLWINDOWPOS4DMESAPROC glWindowPos4dMESA;
  PFNGLWINDOWPOS4DVMESAPROC glWindowPos4dvMESA;
  PFNGLWINDOWPOS4FMESAPROC glWindowPos4fMESA;
  PFNGLWINDOWPOS4FVMESAPROC glWindowPos4fvMESA;
  PFNGLWINDOWPOS4IMESAPROC glWindowPos4iMESA;
  PFNGLWINDOWPOS4IVMESAPROC glWindowPos4ivMESA;
  PFNGLWINDOWPOS4SMESAPROC glWindowPos4sMESA;
  PFNGLWINDOWPOS4SVMESAPROC glWindowPos4svMESA;
  /// GL_IBM_multimode_draw_arrays  // #200 http://www.opengl.org/registry/specs/IBM/multimode_draw_arrays.txt
  PFNGLMULTIMODEDRAWARRAYSIBMPROC glMultiModeDrawArraysIBM;
  PFNGLMULTIMODEDRAWELEMENTSIBMPROC glMultiModeDrawElementsIBM;
  /// GL_IBM_vertex_array_lists    // #201 http://www.opengl.org/registry/specs/IBM/vertex_array_lists.txt
  PFNGLCOLORPOINTERLISTIBMPROC glColorPointerListIBM;
  PFNGLSECONDARYCOLORPOINTERLISTIBMPROC glSecondaryColorPointerListIBM;
  PFNGLEDGEFLAGPOINTERLISTIBMPROC glEdgeFlagPointerListIBM;
  PFNGLFOGCOORDPOINTERLISTIBMPROC glFogCoordPointerListIBM;
  PFNGLINDEXPOINTERLISTIBMPROC glIndexPointerListIBM;
  PFNGLNORMALPOINTERLISTIBMPROC glNormalPointerListIBM;
  PFNGLTEXCOORDPOINTERLISTIBMPROC glTexCoordPointerListIBM;
  PFNGLVERTEXPOINTERLISTIBMPROC glVertexPointerListIBM;
  /// GL_3DFX_tbuffer              // #208 http://www.opengl.org/registry/specs/3DFX/tbuffer.txt
  PFNGLTBUFFERMASK3DFXPROC glTbufferMask3DFX;
  /// GL_EXT_multisample           // WGL_EXT_multisample #209 http://www.opengl.org/registry/specs/EXT/wgl_multisample.txt
  PFNGLSAMPLEMASKEXTPROC glSampleMaskEXT;
  PFNGLSAMPLEPATTERNEXTPROC glSamplePatternEXT;
  /// GL_SGIS_texture_color_mask   // #214 http://www.opengl.org/registry/specs/SGIS/texture_color_mask.txt
  PFNGLTEXTURECOLORMASKSGISPROC glTextureColorMaskSGIS;
  #ifdef OS_LINUX
  /// GLX_MESA_copy_sub_buffer     // #215 http://www.opengl.org/registry/specs/MESA/copy_sub_buffer.txt
  PFNGLXCOPYSUBBUFFERMESAPROC glXCopySubBufferMESA;
  /// GLX_MESA_pixmap_colormap     // #216 http://www.opengl.org/registry/specs/MESA/pixmap_colormap.txt
  PFNGLXCREATEGLXPIXMAPMESAPROC glXCreateGLXPixmapMESA;
  /// GLX_MESA_release_buffers     // #217 http://www.opengl.org/registry/specs/MESA/release_buffers.txt
  PFNGLXRELEASEBUFFERSMESAPROC glXReleaseBuffersMESA;
  /// GLX_MESA_set_3dfx_mode       // #218 http://www.opengl.org/registry/specs/MESA/set_3dfx_mode.txt
  PFNGLXSET3DFXMODEMESAPROC glXSet3DfxModeMESA;
  #endif
  /// GL_NV_fence                  // #222 http://www.opengl.org/registry/specs/NV/fence.txt
  PFNGLDELETEFENCESNVPROC glDeleteFencesNV;
  PFNGLGENFENCESNVPROC glGenFencesNV;
  PFNGLISFENCENVPROC glIsFenceNV;
  PFNGLTESTFENCENVPROC glTestFenceNV;
  PFNGLGETFENCEIVNVPROC glGetFenceivNV;
  PFNGLFINISHFENCENVPROC glFinishFenceNV;
  PFNGLSETFENCENVPROC glSetFenceNV;
  /// GL_IBM_static_data           // #223 http://www.opengl.org/registry/specs/IBM/static_data.txt
  PFNGLFLUSHSTATICDATAIBMPROC glFlushStaticDataIBM;
  /// GL_NV_evaluators             // #225 http://www.opengl.org/registry/specs/NV/evaluators.txt
  PFNGLMAPCONTROLPOINTSNVPROC glMapControlPointsNV;
  PFNGLMAPPARAMETERIVNVPROC glMapParameterivNV;
  PFNGLMAPPARAMETERFVNVPROC glMapParameterfvNV;
  PFNGLGETMAPCONTROLPOINTSNVPROC glGetMapControlPointsNV;
  PFNGLGETMAPPARAMETERIVNVPROC glGetMapParameterivNV;
  PFNGLGETMAPPARAMETERFVNVPROC glGetMapParameterfvNV;
  PFNGLGETMAPATTRIBPARAMETERIVNVPROC glGetMapAttribParameterivNV;
  PFNGLGETMAPATTRIBPARAMETERFVNVPROC glGetMapAttribParameterfvNV;
  PFNGLEVALMAPSNVPROC glEvalMapsNV;
  /// GL_NV_register_combiners2    // #227 http://www.opengl.org/registry/specs/NV/register_combiners2.txt
  PFNGLCOMBINERSTAGEPARAMETERFVNVPROC glCombinerStageParameterfvNV;
  PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC glGetCombinerStageParameterfvNV;
  /// GL_NV_vertex_program         // #233 http://www.opengl.org/registry/specs/NV/vertex_program.txt
  PFNGLAREPROGRAMSRESIDENTNVPROC glAreProgramsResidentNV;
  PFNGLBINDPROGRAMNVPROC glBindProgramNV;
  PFNGLDELETEPROGRAMSNVPROC glDeleteProgramsNV;
  PFNGLEXECUTEPROGRAMNVPROC glExecuteProgramNV;
  PFNGLGENPROGRAMSNVPROC glGenProgramsNV;
  PFNGLGETPROGRAMPARAMETERDVNVPROC glGetProgramParameterdvNV;
  PFNGLGETPROGRAMPARAMETERFVNVPROC glGetProgramParameterfvNV;
  PFNGLGETPROGRAMIVNVPROC glGetProgramivNV;
  PFNGLGETPROGRAMSTRINGNVPROC glGetProgramStringNV;
  PFNGLGETTRACKMATRIXIVNVPROC glGetTrackMatrixivNV;
  PFNGLGETVERTEXATTRIBDVNVPROC glGetVertexAttribdvNV;
  PFNGLGETVERTEXATTRIBFVNVPROC glGetVertexAttribfvNV;
  PFNGLGETVERTEXATTRIBIVNVPROC glGetVertexAttribivNV;
  PFNGLGETVERTEXATTRIBPOINTERVNVPROC glGetVertexAttribPointervNV;
  PFNGLISPROGRAMNVPROC glIsProgramNV;
  PFNGLLOADPROGRAMNVPROC glLoadProgramNV;
  PFNGLPROGRAMPARAMETER4DNVPROC glProgramParameter4dNV;
  PFNGLPROGRAMPARAMETER4DVNVPROC glProgramParameter4dvNV;
  PFNGLPROGRAMPARAMETER4FNVPROC glProgramParameter4fNV;
  PFNGLPROGRAMPARAMETER4FVNVPROC glProgramParameter4fvNV;
  PFNGLPROGRAMPARAMETERS4DVNVPROC glProgramParameters4dvNV;
  PFNGLPROGRAMPARAMETERS4FVNVPROC glProgramParameters4fvNV;
  PFNGLREQUESTRESIDENTPROGRAMSNVPROC glRequestResidentProgramsNV;
  PFNGLTRACKMATRIXNVPROC glTrackMatrixNV;
  PFNGLVERTEXATTRIBPOINTERNVPROC glVertexAttribPointerNV;
  PFNGLVERTEXATTRIB1DNVPROC glVertexAttrib1dNV;
  PFNGLVERTEXATTRIB1DVNVPROC glVertexAttrib1dvNV;
  PFNGLVERTEXATTRIB1FNVPROC glVertexAttrib1fNV;
  PFNGLVERTEXATTRIB1FVNVPROC glVertexAttrib1fvNV;
  PFNGLVERTEXATTRIB1SNVPROC glVertexAttrib1sNV;
  PFNGLVERTEXATTRIB1SVNVPROC glVertexAttrib1svNV;
  PFNGLVERTEXATTRIB2DNVPROC glVertexAttrib2dNV;
  PFNGLVERTEXATTRIB2DVNVPROC glVertexAttrib2dvNV;
  PFNGLVERTEXATTRIB2FNVPROC glVertexAttrib2fNV;
  PFNGLVERTEXATTRIB2FVNVPROC glVertexAttrib2fvNV;
  PFNGLVERTEXATTRIB2SNVPROC glVertexAttrib2sNV;
  PFNGLVERTEXATTRIB2SVNVPROC glVertexAttrib2svNV;
  PFNGLVERTEXATTRIB3DNVPROC glVertexAttrib3dNV;
  PFNGLVERTEXATTRIB3DVNVPROC glVertexAttrib3dvNV;
  PFNGLVERTEXATTRIB3FNVPROC glVertexAttrib3fNV;
  PFNGLVERTEXATTRIB3FVNVPROC glVertexAttrib3fvNV;
  PFNGLVERTEXATTRIB3SNVPROC glVertexAttrib3sNV;
  PFNGLVERTEXATTRIB3SVNVPROC glVertexAttrib3svNV;
  PFNGLVERTEXATTRIB4DNVPROC glVertexAttrib4dNV;
  PFNGLVERTEXATTRIB4DVNVPROC glVertexAttrib4dvNV;
  PFNGLVERTEXATTRIB4FNVPROC glVertexAttrib4fNV;
  PFNGLVERTEXATTRIB4FVNVPROC glVertexAttrib4fvNV;
  PFNGLVERTEXATTRIB4SNVPROC glVertexAttrib4sNV;
  PFNGLVERTEXATTRIB4SVNVPROC glVertexAttrib4svNV;
  PFNGLVERTEXATTRIB4UBNVPROC glVertexAttrib4ubNV;
  PFNGLVERTEXATTRIB4UBVNVPROC glVertexAttrib4ubvNV;
  PFNGLVERTEXATTRIBS1DVNVPROC glVertexAttribs1dvNV;
  PFNGLVERTEXATTRIBS1FVNVPROC glVertexAttribs1fvNV;
  PFNGLVERTEXATTRIBS1SVNVPROC glVertexAttribs1svNV;
  PFNGLVERTEXATTRIBS2DVNVPROC glVertexAttribs2dvNV;
  PFNGLVERTEXATTRIBS2FVNVPROC glVertexAttribs2fvNV;
  PFNGLVERTEXATTRIBS2SVNVPROC glVertexAttribs2svNV;
  PFNGLVERTEXATTRIBS3DVNVPROC glVertexAttribs3dvNV;
  PFNGLVERTEXATTRIBS3FVNVPROC glVertexAttribs3fvNV;
  PFNGLVERTEXATTRIBS3SVNVPROC glVertexAttribs3svNV;
  PFNGLVERTEXATTRIBS4DVNVPROC glVertexAttribs4dvNV;
  PFNGLVERTEXATTRIBS4FVNVPROC glVertexAttribs4fvNV;
  PFNGLVERTEXATTRIBS4SVNVPROC glVertexAttribs4svNV;
  PFNGLVERTEXATTRIBS4UBVNVPROC glVertexAttribs4ubvNV;
  #ifdef OS_LINUX
  /// GLX_OML_sync_control         // #238 http://www.opengl.org/registry/specs/OML/glx_sync_control.txt
  PFNGLXGETSYNCVALUESOMLPROC glXGetSyncValuesOML;
  PFNGLXGETMSCRATEOMLPROC glXGetMscRateOML;
  PFNGLXSWAPBUFFERSMSCOMLPROC glXSwapBuffersMscOML;
  PFNGLXWAITFORMSCOMLPROC glXWaitForMscOML;
  PFNGLXWAITFORSBCOMLPROC glXWaitForSbcOML;
  #endif
  #ifdef OS_WIN
  /// WGL_OML_sync_control         // #242 http://www.opengl.org/registry/specs/OML/wgl_sync_control.txt
  PFNWGLGETSYNCVALUESOMLPROC wglGetSyncValuesOML;
  PFNWGLGETMSCRATEOMLPROC wglGetMscRateOML;
  PFNWGLSWAPBUFFERSMSCOMLPROC wglSwapBuffersMscOML;
  PFNWGLSWAPLAYERBUFFERSMSCOMLPROC wglSwapLayerBuffersMscOML;
  PFNWGLWAITFORMSCOMLPROC wglWaitForMscOML;
  PFNWGLWAITFORSBCOMLPROC wglWaitForSbcOML;
  #endif
  /// GL_ATI_envmap_bumpmap        // #244 http://www.opengl.org/registry/specs/ATI/envmap_bumpmap.txt
  PFNGLTEXBUMPPARAMETERIVATIPROC glTexBumpParameterivATI;
  PFNGLTEXBUMPPARAMETERFVATIPROC glTexBumpParameterfvATI;
  PFNGLGETTEXBUMPPARAMETERIVATIPROC glGetTexBumpParameterivATI;
  PFNGLGETTEXBUMPPARAMETERFVATIPROC glGetTexBumpParameterfvATI;
  /// GL_ATI_fragment_shader       // #245 http://www.opengl.org/registry/specs/ATI/fragment_shader.txt
  PFNGLGENFRAGMENTSHADERSATIPROC glGenFragmentShadersATI;
  PFNGLBINDFRAGMENTSHADERATIPROC glBindFragmentShaderATI;
  PFNGLDELETEFRAGMENTSHADERATIPROC glDeleteFragmentShaderATI;
  PFNGLBEGINFRAGMENTSHADERATIPROC glBeginFragmentShaderATI;
  PFNGLENDFRAGMENTSHADERATIPROC glEndFragmentShaderATI;
  PFNGLPASSTEXCOORDATIPROC glPassTexCoordATI;
  PFNGLSAMPLEMAPATIPROC glSampleMapATI;
  PFNGLCOLORFRAGMENTOP1ATIPROC glColorFragmentOp1ATI;
  PFNGLCOLORFRAGMENTOP2ATIPROC glColorFragmentOp2ATI;
  PFNGLCOLORFRAGMENTOP3ATIPROC glColorFragmentOp3ATI;
  PFNGLALPHAFRAGMENTOP1ATIPROC glAlphaFragmentOp1ATI;
  PFNGLALPHAFRAGMENTOP2ATIPROC glAlphaFragmentOp2ATI;
  PFNGLALPHAFRAGMENTOP3ATIPROC glAlphaFragmentOp3ATI;
  PFNGLSETFRAGMENTSHADERCONSTANTATIPROC glSetFragmentShaderConstantATI;
  /// GL_ATI_pn_triangles          // #246 http://www.opengl.org/registry/specs/ATI/pn_triangles.txt
  PFNGLPNTRIANGLESIATIPROC glPNTrianglesiATI;
  PFNGLPNTRIANGLESFATIPROC glPNTrianglesfATI;
  /// GL_ATI_vertex_array_object   // #247 http://www.opengl.org/registry/specs/ATI/vertex_array_object.txt
  PFNGLNEWOBJECTBUFFERATIPROC glNewObjectBufferATI;
  PFNGLISOBJECTBUFFERATIPROC glIsObjectBufferATI;
  PFNGLUPDATEOBJECTBUFFERATIPROC glUpdateObjectBufferATI;
  PFNGLGETOBJECTBUFFERFVATIPROC glGetObjectBufferfvATI;
  PFNGLGETOBJECTBUFFERIVATIPROC glGetObjectBufferivATI;
  PFNGLFREEOBJECTBUFFERATIPROC glFreeObjectBufferATI;
  PFNGLARRAYOBJECTATIPROC glArrayObjectATI;
  PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectfvATI;
  PFNGLGETARRAYOBJECTIVATIPROC glGetArrayObjectivATI;
  PFNGLVARIANTARRAYOBJECTATIPROC glVariantArrayObjectATI;
  PFNGLGETVARIANTARRAYOBJECTFVATIPROC glGetVariantArrayObjectfvATI;
  PFNGLGETVARIANTARRAYOBJECTIVATIPROC glGetVariantArrayObjectivATI;
  /// GL_EXT_vertex_shader         // #248 http://www.opengl.org/registry/specs/EXT/vertex_shader.txt
  PFNGLBEGINVERTEXSHADEREXTPROC glBeginVertexShaderEXT;
  PFNGLENDVERTEXSHADEREXTPROC glEndVertexShaderEXT;
  PFNGLBINDVERTEXSHADEREXTPROC glBindVertexShaderEXT;
  PFNGLGENVERTEXSHADERSEXTPROC glGenVertexShadersEXT;
  PFNGLDELETEVERTEXSHADEREXTPROC glDeleteVertexShaderEXT;
  PFNGLSHADEROP1EXTPROC glShaderOp1EXT;
  PFNGLSHADEROP2EXTPROC glShaderOp2EXT;
  PFNGLSHADEROP3EXTPROC glShaderOp3EXT;
  PFNGLSWIZZLEEXTPROC glSwizzleEXT;
  PFNGLWRITEMASKEXTPROC glWriteMaskEXT;
  PFNGLINSERTCOMPONENTEXTPROC glInsertComponentEXT;
  PFNGLEXTRACTCOMPONENTEXTPROC glExtractComponentEXT;
  PFNGLGENSYMBOLSEXTPROC glGenSymbolsEXT;
  PFNGLSETINVARIANTEXTPROC glSetInvariantEXT;
  PFNGLSETLOCALCONSTANTEXTPROC glSetLocalConstantEXT;
  PFNGLVARIANTBVEXTPROC glVariantbvEXT;
  PFNGLVARIANTSVEXTPROC glVariantsvEXT;
  PFNGLVARIANTIVEXTPROC glVariantivEXT;
  PFNGLVARIANTFVEXTPROC glVariantfvEXT;
  PFNGLVARIANTDVEXTPROC glVariantdvEXT;
  PFNGLVARIANTUBVEXTPROC glVariantubvEXT;
  PFNGLVARIANTUSVEXTPROC glVariantusvEXT;
  PFNGLVARIANTUIVEXTPROC glVariantuivEXT;
  PFNGLVARIANTPOINTEREXTPROC glVariantPointerEXT;
  PFNGLENABLEVARIANTCLIENTSTATEEXTPROC glEnableVariantClientStateEXT;
  PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC glDisableVariantClientStateEXT;
  PFNGLBINDLIGHTPARAMETEREXTPROC glBindLightParameterEXT;
  PFNGLBINDMATERIALPARAMETEREXTPROC glBindMaterialParameterEXT;
  PFNGLBINDTEXGENPARAMETEREXTPROC glBindTexGenParameterEXT;
  PFNGLBINDTEXTUREUNITPARAMETEREXTPROC glBindTextureUnitParameterEXT;
  PFNGLBINDPARAMETEREXTPROC glBindParameterEXT;
  PFNGLISVARIANTENABLEDEXTPROC glIsVariantEnabledEXT;
  PFNGLGETVARIANTBOOLEANVEXTPROC glGetVariantBooleanvEXT;
  PFNGLGETVARIANTINTEGERVEXTPROC glGetVariantIntegervEXT;
  PFNGLGETVARIANTFLOATVEXTPROC glGetVariantFloatvEXT;
  PFNGLGETVARIANTPOINTERVEXTPROC glGetVariantPointervEXT;
  PFNGLGETINVARIANTBOOLEANVEXTPROC glGetInvariantBooleanvEXT;
  PFNGLGETINVARIANTINTEGERVEXTPROC glGetInvariantIntegervEXT;
  PFNGLGETINVARIANTFLOATVEXTPROC glGetInvariantFloatvEXT;
  PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC glGetLocalConstantBooleanvEXT;
  PFNGLGETLOCALCONSTANTINTEGERVEXTPROC glGetLocalConstantIntegervEXT;
  PFNGLGETLOCALCONSTANTFLOATVEXTPROC glGetLocalConstantFloatvEXT;
  /// GL_ATI_vertex_streams        // #249 http://www.opengl.org/registry/specs/ATI/vertex_streams.txt
  PFNGLVERTEXSTREAM1SATIPROC glVertexStream1sATI;
  PFNGLVERTEXSTREAM1SVATIPROC glVertexStream1svATI;
  PFNGLVERTEXSTREAM1IATIPROC glVertexStream1iATI;
  PFNGLVERTEXSTREAM1IVATIPROC glVertexStream1ivATI;
  PFNGLVERTEXSTREAM1FATIPROC glVertexStream1fATI;
  PFNGLVERTEXSTREAM1FVATIPROC glVertexStream1fvATI;
  PFNGLVERTEXSTREAM1DATIPROC glVertexStream1dATI;
  PFNGLVERTEXSTREAM1DVATIPROC glVertexStream1dvATI;
  PFNGLVERTEXSTREAM2SATIPROC glVertexStream2sATI;
  PFNGLVERTEXSTREAM2SVATIPROC glVertexStream2svATI;
  PFNGLVERTEXSTREAM2IATIPROC glVertexStream2iATI;
  PFNGLVERTEXSTREAM2IVATIPROC glVertexStream2ivATI;
  PFNGLVERTEXSTREAM2FATIPROC glVertexStream2fATI;
  PFNGLVERTEXSTREAM2FVATIPROC glVertexStream2fvATI;
  PFNGLVERTEXSTREAM2DATIPROC glVertexStream2dATI;
  PFNGLVERTEXSTREAM2DVATIPROC glVertexStream2dvATI;
  PFNGLVERTEXSTREAM3SATIPROC glVertexStream3sATI;
  PFNGLVERTEXSTREAM3SVATIPROC glVertexStream3svATI;
  PFNGLVERTEXSTREAM3IATIPROC glVertexStream3iATI;
  PFNGLVERTEXSTREAM3IVATIPROC glVertexStream3ivATI;
  PFNGLVERTEXSTREAM3FATIPROC glVertexStream3fATI;
  PFNGLVERTEXSTREAM3FVATIPROC glVertexStream3fvATI;
  PFNGLVERTEXSTREAM3DATIPROC glVertexStream3dATI;
  PFNGLVERTEXSTREAM3DVATIPROC glVertexStream3dvATI;
  PFNGLVERTEXSTREAM4SATIPROC glVertexStream4sATI;
  PFNGLVERTEXSTREAM4SVATIPROC glVertexStream4svATI;
  PFNGLVERTEXSTREAM4IATIPROC glVertexStream4iATI;
  PFNGLVERTEXSTREAM4IVATIPROC glVertexStream4ivATI;
  PFNGLVERTEXSTREAM4FATIPROC glVertexStream4fATI;
  PFNGLVERTEXSTREAM4FVATIPROC glVertexStream4fvATI;
  PFNGLVERTEXSTREAM4DATIPROC glVertexStream4dATI;
  PFNGLVERTEXSTREAM4DVATIPROC glVertexStream4dvATI;
  PFNGLNORMALSTREAM3BATIPROC glNormalStream3bATI;
  PFNGLNORMALSTREAM3BVATIPROC glNormalStream3bvATI;
  PFNGLNORMALSTREAM3SATIPROC glNormalStream3sATI;
  PFNGLNORMALSTREAM3SVATIPROC glNormalStream3svATI;
  PFNGLNORMALSTREAM3IATIPROC glNormalStream3iATI;
  PFNGLNORMALSTREAM3IVATIPROC glNormalStream3ivATI;
  PFNGLNORMALSTREAM3FATIPROC glNormalStream3fATI;
  PFNGLNORMALSTREAM3FVATIPROC glNormalStream3fvATI;
  PFNGLNORMALSTREAM3DATIPROC glNormalStream3dATI;
  PFNGLNORMALSTREAM3DVATIPROC glNormalStream3dvATI;
  PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC glClientActiveVertexStreamATI;
  PFNGLVERTEXBLENDENVIATIPROC glVertexBlendEnviATI;
  PFNGLVERTEXBLENDENVFATIPROC glVertexBlendEnvfATI;
  #ifdef OS_WIN
  /// WGL_I3D_digital_video_control  // #250 http://www.opengl.org/registry/specs/I3D/wgl_digital_video_control.txt
  PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC wglGetDigitalVideoParametersI3D;
  PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC wglSetDigitalVideoParametersI3D;
  /// WGL_I3D_gamma                // #251 http://www.opengl.org/registry/specs/I3D/wgl_gamma.txt
  PFNWGLGETGAMMATABLEPARAMETERSI3DPROC wglGetGammaTableParametersI3D;
  PFNWGLSETGAMMATABLEPARAMETERSI3DPROC wglSetGammaTableParametersI3D;
  PFNWGLGETGAMMATABLEI3DPROC wglGetGammaTableI3D;
  PFNWGLSETGAMMATABLEI3DPROC wglSetGammaTableI3D;
  /// WGL_I3D_genlock              // #252 http://www.opengl.org/registry/specs/I3D/wgl_genlock.txt
  PFNWGLENABLEGENLOCKI3DPROC wglEnableGenlockI3D;
  PFNWGLDISABLEGENLOCKI3DPROC wglDisableGenlockI3D;
  PFNWGLISENABLEDGENLOCKI3DPROC wglIsEnabledGenlockI3D;
  PFNWGLGENLOCKSOURCEI3DPROC wglGenlockSourceI3D;
  PFNWGLGETGENLOCKSOURCEI3DPROC wglGetGenlockSourceI3D;
  PFNWGLGENLOCKSOURCEEDGEI3DPROC wglGenlockSourceEdgeI3D;
  PFNWGLGETGENLOCKSOURCEEDGEI3DPROC wglGetGenlockSourceEdgeI3D;
  PFNWGLGENLOCKSAMPLERATEI3DPROC wglGenlockSampleRateI3D;
  PFNWGLGETGENLOCKSAMPLERATEI3DPROC wglGetGenlockSampleRateI3D;
  PFNWGLGENLOCKSOURCEDELAYI3DPROC wglGenlockSourceDelayI3D;
  PFNWGLGETGENLOCKSOURCEDELAYI3DPROC wglGetGenlockSourceDelayI3D;
  PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC wglQueryGenlockMaxSourceDelayI3D;
  /// WGL_I3D_image_buffer         // #253 http://www.opengl.org/registry/specs/I3D/wgl_image_buffer.txt
  PFNWGLCREATEIMAGEBUFFERI3DPROC wglCreateImageBufferI3D;
  PFNWGLDESTROYIMAGEBUFFERI3DPROC wglDestroyImageBufferI3D;
  PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC wglAssociateImageBufferEventsI3D;
  PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC wglReleaseImageBufferEventsI3D;
  /// WGL_I3D_swap_frame_lock      // #254 http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_lock.txt
  PFNWGLENABLEFRAMELOCKI3DPROC wglEnableFrameLockI3D;
  PFNWGLDISABLEFRAMELOCKI3DPROC wglDisableFrameLockI3D;
  PFNWGLISENABLEDFRAMELOCKI3DPROC wglIsEnabledFrameLockI3D;
  PFNWGLQUERYFRAMELOCKMASTERI3DPROC wglQueryFrameLockMasterI3D;
  /// WGL_I3D_swap_frame_usage     // #255 http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_usage.txt
  PFNWGLGETFRAMEUSAGEI3DPROC wglGetFrameUsageI3D;
  PFNWGLBEGINFRAMETRACKINGI3DPROC wglBeginFrameTrackingI3D;
  PFNWGLENDFRAMETRACKINGI3DPROC wglEndFrameTrackingI3D;
  PFNWGLQUERYFRAMETRACKINGI3DPROC wglQueryFrameTrackingI3D;
  #endif
  /// GL_ATI_element_array         // #256 http://www.opengl.org/registry/specs/ATI/element_array.txt
  PFNGLELEMENTPOINTERATIPROC glElementPointerATI;
  PFNGLDRAWELEMENTARRAYATIPROC glDrawElementArrayATI;
  PFNGLDRAWRANGEELEMENTARRAYATIPROC glDrawRangeElementArrayATI;
  /// GL_SUN_mesh_array            // #257 http://www.opengl.org/registry/specs/SUN/mesh_array.txt
  PFNGLDRAWMESHARRAYSSUNPROC glDrawMeshArraysSUN;
  /// GL_NV_occlusion_query        // #261 http://www.opengl.org/registry/specs/NV/occlusion_query.txt
  PFNGLGENOCCLUSIONQUERIESNVPROC glGenOcclusionQueriesNV;
  PFNGLDELETEOCCLUSIONQUERIESNVPROC glDeleteOcclusionQueriesNV;
  PFNGLISOCCLUSIONQUERYNVPROC glIsOcclusionQueryNV;
  PFNGLBEGINOCCLUSIONQUERYNVPROC glBeginOcclusionQueryNV;
  PFNGLENDOCCLUSIONQUERYNVPROC glEndOcclusionQueryNV;
  PFNGLGETOCCLUSIONQUERYIVNVPROC glGetOcclusionQueryivNV;
  PFNGLGETOCCLUSIONQUERYUIVNVPROC glGetOcclusionQueryuivNV;
  /// GL_NV_point_sprite           // #262 http://www.opengl.org/registry/specs/NV/point_sprite.txt
  PFNGLPOINTPARAMETERINVPROC glPointParameteriNV;
  PFNGLPOINTPARAMETERIVNVPROC glPointParameterivNV;
  /// GL_EXT_stencil_two_side      // #268 http://www.opengl.org/registry/specs/EXT/stencil_two_side.txt
  PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT;
  /// GL_APPLE_element_array       // #271 http://www.opengl.org/registry/specs/APPLE/element_array.txt
  PFNGLELEMENTPOINTERAPPLEPROC glElementPointerAPPLE;
  PFNGLDRAWELEMENTARRAYAPPLEPROC glDrawElementArrayAPPLE;
  PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC glDrawRangeElementArrayAPPLE;
  PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC glMultiDrawElementArrayAPPLE;
  PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC glMultiDrawRangeElementArrayAPPLE;
  /// GL_APPLE_fence               // #272 http://www.opengl.org/registry/specs/APPLE/fence.txt
  PFNGLGENFENCESAPPLEPROC glGenFencesAPPLE;
  PFNGLDELETEFENCESAPPLEPROC glDeleteFencesAPPLE;
  PFNGLSETFENCEAPPLEPROC glSetFenceAPPLE;
  PFNGLISFENCEAPPLEPROC glIsFenceAPPLE;
  PFNGLTESTFENCEAPPLEPROC glTestFenceAPPLE;
  PFNGLFINISHFENCEAPPLEPROC glFinishFenceAPPLE;
  PFNGLTESTOBJECTAPPLEPROC glTestObjectAPPLE;
  PFNGLFINISHOBJECTAPPLEPROC glFinishObjectAPPLE;
  /// GL_APPLE_vertex_array_object  // #273 http://www.opengl.org/registry/specs/APPLE/vertex_array_object.txt
  PFNGLBINDVERTEXARRAYAPPLEPROC glBindVertexArrayAPPLE;
  PFNGLDELETEVERTEXARRAYSAPPLEPROC glDeleteVertexArraysAPPLE;
  PFNGLGENVERTEXARRAYSAPPLEPROC glGenVertexArraysAPPLE;
  PFNGLISVERTEXARRAYAPPLEPROC glIsVertexArrayAPPLE;
  /// GL_APPLE_vertex_array_range  // #274 http://www.opengl.org/registry/specs/APPLE/vertex_array_range.txt
  PFNGLVERTEXARRAYRANGEAPPLEPROC glVertexArrayRangeAPPLE;
  PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC glFlushVertexArrayRangeAPPLE;
  PFNGLVERTEXARRAYPARAMETERIAPPLEPROC glVertexArrayParameteriAPPLE;
  /// GL_ATI_draw_buffers          // #277 http://www.opengl.org/registry/specs/ATI/draw_buffers.txt
  PFNGLDRAWBUFFERSATIPROC glDrawBuffersATI;
  /// GL_NV_fragment_program       // #282 http://www.opengl.org/registry/specs/NV/fragment_program.txt
  PFNGLPROGRAMNAMEDPARAMETER4FNVPROC glProgramNamedParameter4fNV;
  PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC glProgramNamedParameter4fvNV;
  PFNGLPROGRAMNAMEDPARAMETER4DNVPROC glProgramNamedParameter4dNV;
  PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC glProgramNamedParameter4dvNV;
  PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC glGetProgramNamedParameterfvNV;
  PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC glGetProgramNamedParameterdvNV;
  /// GL_NV_half_float             // #283 http://www.opengl.org/registry/specs/NV/half_float.txt
  PFNGLVERTEX2HNVPROC glVertex2hNV;
  PFNGLVERTEX2HVNVPROC glVertex2hvNV;
  PFNGLVERTEX3HNVPROC glVertex3hNV;
  PFNGLVERTEX3HVNVPROC glVertex3hvNV;
  PFNGLVERTEX4HNVPROC glVertex4hNV;
  PFNGLVERTEX4HVNVPROC glVertex4hvNV;
  PFNGLNORMAL3HNVPROC glNormal3hNV;
  PFNGLNORMAL3HVNVPROC glNormal3hvNV;
  PFNGLCOLOR3HNVPROC glColor3hNV;
  PFNGLCOLOR3HVNVPROC glColor3hvNV;
  PFNGLCOLOR4HNVPROC glColor4hNV;
  PFNGLCOLOR4HVNVPROC glColor4hvNV;
  PFNGLTEXCOORD1HNVPROC glTexCoord1hNV;
  PFNGLTEXCOORD1HVNVPROC glTexCoord1hvNV;
  PFNGLTEXCOORD2HNVPROC glTexCoord2hNV;
  PFNGLTEXCOORD2HVNVPROC glTexCoord2hvNV;
  PFNGLTEXCOORD3HNVPROC glTexCoord3hNV;
  PFNGLTEXCOORD3HVNVPROC glTexCoord3hvNV;
  PFNGLTEXCOORD4HNVPROC glTexCoord4hNV;
  PFNGLTEXCOORD4HVNVPROC glTexCoord4hvNV;
  PFNGLMULTITEXCOORD1HNVPROC glMultiTexCoord1hNV;
  PFNGLMULTITEXCOORD1HVNVPROC glMultiTexCoord1hvNV;
  PFNGLMULTITEXCOORD2HNVPROC glMultiTexCoord2hNV;
  PFNGLMULTITEXCOORD2HVNVPROC glMultiTexCoord2hvNV;
  PFNGLMULTITEXCOORD3HNVPROC glMultiTexCoord3hNV;
  PFNGLMULTITEXCOORD3HVNVPROC glMultiTexCoord3hvNV;
  PFNGLMULTITEXCOORD4HNVPROC glMultiTexCoord4hNV;
  PFNGLMULTITEXCOORD4HVNVPROC glMultiTexCoord4hvNV;
  PFNGLFOGCOORDHNVPROC glFogCoordhNV;
  PFNGLFOGCOORDHVNVPROC glFogCoordhvNV;
  PFNGLSECONDARYCOLOR3HNVPROC glSecondaryColor3hNV;
  PFNGLSECONDARYCOLOR3HVNVPROC glSecondaryColor3hvNV;
  PFNGLVERTEXWEIGHTHNVPROC glVertexWeighthNV;
  PFNGLVERTEXWEIGHTHVNVPROC glVertexWeighthvNV;
  PFNGLVERTEXATTRIB1HNVPROC glVertexAttrib1hNV;
  PFNGLVERTEXATTRIB1HVNVPROC glVertexAttrib1hvNV;
  PFNGLVERTEXATTRIB2HNVPROC glVertexAttrib2hNV;
  PFNGLVERTEXATTRIB2HVNVPROC glVertexAttrib2hvNV;
  PFNGLVERTEXATTRIB3HNVPROC glVertexAttrib3hNV;
  PFNGLVERTEXATTRIB3HVNVPROC glVertexAttrib3hvNV;
  PFNGLVERTEXATTRIB4HNVPROC glVertexAttrib4hNV;
  PFNGLVERTEXATTRIB4HVNVPROC glVertexAttrib4hvNV;
  PFNGLVERTEXATTRIBS1HVNVPROC glVertexAttribs1hvNV;
  PFNGLVERTEXATTRIBS2HVNVPROC glVertexAttribs2hvNV;
  PFNGLVERTEXATTRIBS3HVNVPROC glVertexAttribs3hvNV;
  PFNGLVERTEXATTRIBS4HVNVPROC glVertexAttribs4hvNV;
  /// GL_NV_pixel_data_range       // #284 http://www.opengl.org/registry/specs/NV/pixel_data_range.txt
  PFNGLPIXELDATARANGENVPROC glPixelDataRangeNV;
  PFNGLFLUSHPIXELDATARANGENVPROC glFlushPixelDataRangeNV;
  /// GL_NV_primitive_restart      // #285 http://www.opengl.org/registry/specs/NV/primitive_restart.txt
  PFNGLPRIMITIVERESTARTNVPROC glPrimitiveRestartNV;
  PFNGLPRIMITIVERESTARTINDEXNVPROC glPrimitiveRestartIndexNV;
  /// GL_ATI_map_object_buffer     // #288 http://www.opengl.org/registry/specs/ATI/map_object_buffer.txt
  PFNGLMAPOBJECTBUFFERATIPROC glMapObjectBufferATI;
  PFNGLUNMAPOBJECTBUFFERATIPROC glUnmapObjectBufferATI;
  /// GL_ATI_separate_stencil      // #289 http://www.opengl.org/registry/specs/ATI/separate_stencil.txt
  PFNGLSTENCILOPSEPARATEATIPROC glStencilOpSeparateATI;
  PFNGLSTENCILFUNCSEPARATEATIPROC glStencilFuncSeparateATI;
  /// GL_ATI_vertex_attrib_array_object  // #290 http://www.opengl.org/registry/specs/ATI/vertex_attrib_array_object.txt
  PFNGLVERTEXATTRIBARRAYOBJECTATIPROC glVertexAttribArrayObjectATI;
  PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC glGetVertexAttribArrayObjectfvATI;
  PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC glGetVertexAttribArrayObjectivATI;
  /// GL_OES_byte_coordinates      // #291 http://www.opengl.org/registry/specs/OES/OES_byte_coordinates.txt
  PFNGLMULTITEXCOORD1BOESPROC glMultiTexCoord1bOES;
  PFNGLMULTITEXCOORD1BVOESPROC glMultiTexCoord1bvOES;
  PFNGLMULTITEXCOORD2BOESPROC glMultiTexCoord2bOES;
  PFNGLMULTITEXCOORD2BVOESPROC glMultiTexCoord2bvOES;
  PFNGLMULTITEXCOORD3BOESPROC glMultiTexCoord3bOES;
  PFNGLMULTITEXCOORD3BVOESPROC glMultiTexCoord3bvOES;
  PFNGLMULTITEXCOORD4BOESPROC glMultiTexCoord4bOES;
  PFNGLMULTITEXCOORD4BVOESPROC glMultiTexCoord4bvOES;
  PFNGLTEXCOORD1BOESPROC glTexCoord1bOES;
  PFNGLTEXCOORD1BVOESPROC glTexCoord1bvOES;
  PFNGLTEXCOORD2BOESPROC glTexCoord2bOES;
  PFNGLTEXCOORD2BVOESPROC glTexCoord2bvOES;
  PFNGLTEXCOORD3BOESPROC glTexCoord3bOES;
  PFNGLTEXCOORD3BVOESPROC glTexCoord3bvOES;
  PFNGLTEXCOORD4BOESPROC glTexCoord4bOES;
  PFNGLTEXCOORD4BVOESPROC glTexCoord4bvOES;
  PFNGLVERTEX2BOESPROC glVertex2bOES;
  PFNGLVERTEX2BVOESPROC glVertex2bvOES;
  PFNGLVERTEX3BOESPROC glVertex3bOES;
  PFNGLVERTEX3BVOESPROC glVertex3bvOES;
  PFNGLVERTEX4BOESPROC glVertex4bOES;
  PFNGLVERTEX4BVOESPROC glVertex4bvOES;
  /// GL_OES_fixed_point           // #292 http://www.opengl.org/registry/specs/OES/OES_fixed_point.txt
  PFNGLALPHAFUNCXOESPROC glAlphaFuncxOES;
  PFNGLCLEARCOLORXOESPROC glClearColorxOES;
  PFNGLCLEARDEPTHXOESPROC glClearDepthxOES;
  PFNGLCLIPPLANEXOESPROC glClipPlanexOES;
  PFNGLCOLOR4XOESPROC glColor4xOES;
  PFNGLDEPTHRANGEXOESPROC glDepthRangexOES;
  PFNGLFOGXOESPROC glFogxOES;
  PFNGLFOGXVOESPROC glFogxvOES;
  PFNGLFRUSTUMXOESPROC glFrustumxOES;
  PFNGLGETCLIPPLANEXOESPROC glGetClipPlanexOES;
  PFNGLGETFIXEDVOESPROC glGetFixedvOES;
  PFNGLGETTEXENVXVOESPROC glGetTexEnvxvOES;
  PFNGLGETTEXPARAMETERXVOESPROC glGetTexParameterxvOES;
  PFNGLLIGHTMODELXOESPROC glLightModelxOES;
  PFNGLLIGHTMODELXVOESPROC glLightModelxvOES;
  PFNGLLIGHTXOESPROC glLightxOES;
  PFNGLLIGHTXVOESPROC glLightxvOES;
  PFNGLLINEWIDTHXOESPROC glLineWidthxOES;
  PFNGLLOADMATRIXXOESPROC glLoadMatrixxOES;
  PFNGLMATERIALXOESPROC glMaterialxOES;
  PFNGLMATERIALXVOESPROC glMaterialxvOES;
  PFNGLMULTMATRIXXOESPROC glMultMatrixxOES;
  PFNGLMULTITEXCOORD4XOESPROC glMultiTexCoord4xOES;
  PFNGLNORMAL3XOESPROC glNormal3xOES;
  PFNGLORTHOXOESPROC glOrthoxOES;
  PFNGLPOINTPARAMETERXVOESPROC glPointParameterxvOES;
  PFNGLPOINTSIZEXOESPROC glPointSizexOES;
  PFNGLPOLYGONOFFSETXOESPROC glPolygonOffsetxOES;
  PFNGLROTATEXOESPROC glRotatexOES;
  PFNGLSAMPLECOVERAGEOESPROC glSampleCoverageOES;
  PFNGLSCALEXOESPROC glScalexOES;
  PFNGLTEXENVXOESPROC glTexEnvxOES;
  PFNGLTEXENVXVOESPROC glTexEnvxvOES;
  PFNGLTEXPARAMETERXOESPROC glTexParameterxOES;
  PFNGLTEXPARAMETERXVOESPROC glTexParameterxvOES;
  PFNGLTRANSLATEXOESPROC glTranslatexOES;
  PFNGLACCUMXOESPROC glAccumxOES;
  PFNGLBITMAPXOESPROC glBitmapxOES;
  PFNGLBLENDCOLORXOESPROC glBlendColorxOES;
  PFNGLCLEARACCUMXOESPROC glClearAccumxOES;
  PFNGLCOLOR3XOESPROC glColor3xOES;
  PFNGLCOLOR3XVOESPROC glColor3xvOES;
  PFNGLCOLOR4XVOESPROC glColor4xvOES;
  PFNGLCONVOLUTIONPARAMETERXOESPROC glConvolutionParameterxOES;
  PFNGLCONVOLUTIONPARAMETERXVOESPROC glConvolutionParameterxvOES;
  PFNGLEVALCOORD1XOESPROC glEvalCoord1xOES;
  PFNGLEVALCOORD1XVOESPROC glEvalCoord1xvOES;
  PFNGLEVALCOORD2XOESPROC glEvalCoord2xOES;
  PFNGLEVALCOORD2XVOESPROC glEvalCoord2xvOES;
  PFNGLFEEDBACKBUFFERXOESPROC glFeedbackBufferxOES;
  PFNGLGETCONVOLUTIONPARAMETERXVOESPROC glGetConvolutionParameterxvOES;
  PFNGLGETHISTOGRAMPARAMETERXVOESPROC glGetHistogramParameterxvOES;
  PFNGLGETLIGHTXOESPROC glGetLightxOES;
  PFNGLGETMAPXVOESPROC glGetMapxvOES;
  PFNGLGETMATERIALXOESPROC glGetMaterialxOES;
  PFNGLGETPIXELMAPXVPROC glGetPixelMapxv;
  PFNGLGETTEXGENXVOESPROC glGetTexGenxvOES;
  PFNGLGETTEXLEVELPARAMETERXVOESPROC glGetTexLevelParameterxvOES;
  PFNGLINDEXXOESPROC glIndexxOES;
  PFNGLINDEXXVOESPROC glIndexxvOES;
  PFNGLLOADTRANSPOSEMATRIXXOESPROC glLoadTransposeMatrixxOES;
  PFNGLMAP1XOESPROC glMap1xOES;
  PFNGLMAP2XOESPROC glMap2xOES;
  PFNGLMAPGRID1XOESPROC glMapGrid1xOES;
  PFNGLMAPGRID2XOESPROC glMapGrid2xOES;
  PFNGLMULTTRANSPOSEMATRIXXOESPROC glMultTransposeMatrixxOES;
  PFNGLMULTITEXCOORD1XOESPROC glMultiTexCoord1xOES;
  PFNGLMULTITEXCOORD1XVOESPROC glMultiTexCoord1xvOES;
  PFNGLMULTITEXCOORD2XOESPROC glMultiTexCoord2xOES;
  PFNGLMULTITEXCOORD2XVOESPROC glMultiTexCoord2xvOES;
  PFNGLMULTITEXCOORD3XOESPROC glMultiTexCoord3xOES;
  PFNGLMULTITEXCOORD3XVOESPROC glMultiTexCoord3xvOES;
  PFNGLMULTITEXCOORD4XVOESPROC glMultiTexCoord4xvOES;
  PFNGLNORMAL3XVOESPROC glNormal3xvOES;
  PFNGLPASSTHROUGHXOESPROC glPassThroughxOES;
  PFNGLPIXELMAPXPROC glPixelMapx;
  PFNGLPIXELSTOREXPROC glPixelStorex;
  PFNGLPIXELTRANSFERXOESPROC glPixelTransferxOES;
  PFNGLPIXELZOOMXOESPROC glPixelZoomxOES;
  PFNGLPRIORITIZETEXTURESXOESPROC glPrioritizeTexturesxOES;
  PFNGLRASTERPOS2XOESPROC glRasterPos2xOES;
  PFNGLRASTERPOS2XVOESPROC glRasterPos2xvOES;
  PFNGLRASTERPOS3XOESPROC glRasterPos3xOES;
  PFNGLRASTERPOS3XVOESPROC glRasterPos3xvOES;
  PFNGLRASTERPOS4XOESPROC glRasterPos4xOES;
  PFNGLRASTERPOS4XVOESPROC glRasterPos4xvOES;
  PFNGLRECTXOESPROC glRectxOES;
  PFNGLRECTXVOESPROC glRectxvOES;
  PFNGLTEXCOORD1XOESPROC glTexCoord1xOES;
  PFNGLTEXCOORD1XVOESPROC glTexCoord1xvOES;
  PFNGLTEXCOORD2XOESPROC glTexCoord2xOES;
  PFNGLTEXCOORD2XVOESPROC glTexCoord2xvOES;
  PFNGLTEXCOORD3XOESPROC glTexCoord3xOES;
  PFNGLTEXCOORD3XVOESPROC glTexCoord3xvOES;
  PFNGLTEXCOORD4XOESPROC glTexCoord4xOES;
  PFNGLTEXCOORD4XVOESPROC glTexCoord4xvOES;
  PFNGLTEXGENXOESPROC glTexGenxOES;
  PFNGLTEXGENXVOESPROC glTexGenxvOES;
  PFNGLVERTEX2XOESPROC glVertex2xOES;
  PFNGLVERTEX2XVOESPROC glVertex2xvOES;
  PFNGLVERTEX3XOESPROC glVertex3xOES;
  PFNGLVERTEX3XVOESPROC glVertex3xvOES;
  PFNGLVERTEX4XOESPROC glVertex4xOES;
  PFNGLVERTEX4XVOESPROC glVertex4xvOES;
  /// GL_OES_single_precision      // #293 http://www.opengl.org/registry/specs/OES/OES_single_precision.txt
  PFNGLCLEARDEPTHFOESPROC glClearDepthfOES;
  PFNGLCLIPPLANEFOESPROC glClipPlanefOES;
  PFNGLDEPTHRANGEFOESPROC glDepthRangefOES;
  PFNGLFRUSTUMFOESPROC glFrustumfOES;
  PFNGLGETCLIPPLANEFOESPROC glGetClipPlanefOES;
  PFNGLORTHOFOESPROC glOrthofOES;
  /// GL_OES_query_matrix          // #296 http://www.opengl.org/registry/specs/OES/OES_query_matrix.txt
  PFNGLQUERYMATRIXXOESPROC glQueryMatrixxOES;
  /// GL_EXT_depth_bounds_test     // #297 http://www.opengl.org/registry/specs/EXT/depth_bounds_test.txt
  PFNGLDEPTHBOUNDSEXTPROC glDepthBoundsEXT;
  /// GL_EXT_blend_equation_separate  // #299 http://www.opengl.org/registry/specs/EXT/blend_equation_separate.txt
  PFNGLBLENDEQUATIONSEPARATEEXTPROC glBlendEquationSeparateEXT;
  #ifdef OS_LINUX
  /// GLX_SGIX_hyperpipe           // #307 http://www.opengl.org/registry/specs/SGIX/hyperpipe_group.txt
  PFNGLXQUERYHYPERPIPENETWORKSGIXPROC glXQueryHyperpipeNetworkSGIX;
  PFNGLXHYPERPIPECONFIGSGIXPROC glXHyperpipeConfigSGIX;
  PFNGLXQUERYHYPERPIPECONFIGSGIXPROC glXQueryHyperpipeConfigSGIX;
  PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC glXDestroyHyperpipeConfigSGIX;
  PFNGLXBINDHYPERPIPESGIXPROC glXBindHyperpipeSGIX;
  PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC glXQueryHyperpipeBestAttribSGIX;
  PFNGLXHYPERPIPEATTRIBSGIXPROC glXHyperpipeAttribSGIX;
  PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC glXQueryHyperpipeAttribSGIX;
  /// GLX_MESA_agp_offset          // #308 http://www.opengl.org/registry/specs/MESA/agp_offset.txt
  PFNGLXGETAGPOFFSETMESAPROC glXGetAGPOffsetMESA;
  #endif
  /// GL_EXT_framebuffer_object    // #310 http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
  PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
  PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
  PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
  PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
  PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
  PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
  PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
  PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
  PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
  PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
  PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
  PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
  PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
  PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
  PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
  PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
  PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;
  /// GL_GREMEDY_string_marker     // #311 http://www.opengl.org/registry/specs/GREMEDY/string_marker.txt
  PFNGLSTRINGMARKERGREMEDYPROC glStringMarkerGREMEDY;
  #ifdef OS_WIN
  /// WGL_3DL_stereo_control       // #313 http://www.opengl.org/registry/specs/3DL/stereo_control.txt
  PFNWGLSETSTEREOEMITTERSTATE3DLPROC wglSetStereoEmitterState3DL;
  #endif
  /// GL_EXT_stencil_clear_tag     // #314 http://www.opengl.org/registry/specs/EXT/stencil_clear_tag.txt
  PFNGLSTENCILCLEARTAGEXTPROC glStencilClearTagEXT;
  /// GL_EXT_framebuffer_blit      // #316 http://www.opengl.org/registry/specs/EXT/framebuffer_blit.txt
  PFNGLBLITFRAMEBUFFEREXTPROC glBlitFramebufferEXT;
  /// GL_EXT_framebuffer_multisample  // #317 http://www.opengl.org/registry/specs/EXT/framebuffer_multisample.txt
  PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT;
  /// GL_EXT_timer_query           // #319 http://www.opengl.org/registry/specs/EXT/timer_query.txt
  PFNGLGETQUERYOBJECTI64VEXTPROC glGetQueryObjecti64vEXT;
  PFNGLGETQUERYOBJECTUI64VEXTPROC glGetQueryObjectui64vEXT;
  /// GL_EXT_gpu_program_parameters  // #320 http://www.opengl.org/registry/specs/EXT/gpu_program_parameters.txt
  PFNGLPROGRAMENVPARAMETERS4FVEXTPROC glProgramEnvParameters4fvEXT;
  PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC glProgramLocalParameters4fvEXT;
  /// GL_APPLE_flush_buffer_range  // #321 http://www.opengl.org/registry/specs/APPLE/flush_buffer_range.txt
  PFNGLBUFFERPARAMETERIAPPLEPROC glBufferParameteriAPPLE;
  PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC glFlushMappedBufferRangeAPPLE;
  /// GL_NV_gpu_program4           // #322 http://www.opengl.org/registry/specs/NV/gpu_program4.txt
  PFNGLPROGRAMLOCALPARAMETERI4INVPROC glProgramLocalParameterI4iNV;
  PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC glProgramLocalParameterI4ivNV;
  PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC glProgramLocalParametersI4ivNV;
  PFNGLPROGRAMLOCALPARAMETERI4UINVPROC glProgramLocalParameterI4uiNV;
  PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC glProgramLocalParameterI4uivNV;
  PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC glProgramLocalParametersI4uivNV;
  PFNGLPROGRAMENVPARAMETERI4INVPROC glProgramEnvParameterI4iNV;
  PFNGLPROGRAMENVPARAMETERI4IVNVPROC glProgramEnvParameterI4ivNV;
  PFNGLPROGRAMENVPARAMETERSI4IVNVPROC glProgramEnvParametersI4ivNV;
  PFNGLPROGRAMENVPARAMETERI4UINVPROC glProgramEnvParameterI4uiNV;
  PFNGLPROGRAMENVPARAMETERI4UIVNVPROC glProgramEnvParameterI4uivNV;
  PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC glProgramEnvParametersI4uivNV;
  PFNGLGETPROGRAMLOCALPARAMETERIIVNVPROC glGetProgramLocalParameterIivNV;
  PFNGLGETPROGRAMLOCALPARAMETERIUIVNVPROC glGetProgramLocalParameterIuivNV;
  PFNGLGETPROGRAMENVPARAMETERIIVNVPROC glGetProgramEnvParameterIivNV;
  PFNGLGETPROGRAMENVPARAMETERIUIVNVPROC glGetProgramEnvParameterIuivNV;
  /// GL_NV_geometry_program4      // #323 http://www.opengl.org/registry/specs/NV/geometry_program4.txt
  PFNGLPROGRAMVERTEXLIMITNVPROC glProgramVertexLimitNV;
  PFNGLFRAMEBUFFERTEXTUREEXTPROC glFramebufferTextureEXT;
  PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC glFramebufferTextureLayerEXT;
  PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC glFramebufferTextureFaceEXT;
  /// GL_EXT_geometry_shader4      // #324 http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
  PFNGLPROGRAMPARAMETERIEXTPROC glProgramParameteriEXT;
  /// GL_NV_vertex_program4        // #325 http://www.opengl.org/registry/specs/NV/vertex_program4.txt
  PFNGLVERTEXATTRIBI1IEXTPROC glVertexAttribI1iEXT;
  PFNGLVERTEXATTRIBI2IEXTPROC glVertexAttribI2iEXT;
  PFNGLVERTEXATTRIBI3IEXTPROC glVertexAttribI3iEXT;
  PFNGLVERTEXATTRIBI4IEXTPROC glVertexAttribI4iEXT;
  PFNGLVERTEXATTRIBI1UIEXTPROC glVertexAttribI1uiEXT;
  PFNGLVERTEXATTRIBI2UIEXTPROC glVertexAttribI2uiEXT;
  PFNGLVERTEXATTRIBI3UIEXTPROC glVertexAttribI3uiEXT;
  PFNGLVERTEXATTRIBI4UIEXTPROC glVertexAttribI4uiEXT;
  PFNGLVERTEXATTRIBI1IVEXTPROC glVertexAttribI1ivEXT;
  PFNGLVERTEXATTRIBI2IVEXTPROC glVertexAttribI2ivEXT;
  PFNGLVERTEXATTRIBI3IVEXTPROC glVertexAttribI3ivEXT;
  PFNGLVERTEXATTRIBI4IVEXTPROC glVertexAttribI4ivEXT;
  PFNGLVERTEXATTRIBI1UIVEXTPROC glVertexAttribI1uivEXT;
  PFNGLVERTEXATTRIBI2UIVEXTPROC glVertexAttribI2uivEXT;
  PFNGLVERTEXATTRIBI3UIVEXTPROC glVertexAttribI3uivEXT;
  PFNGLVERTEXATTRIBI4UIVEXTPROC glVertexAttribI4uivEXT;
  PFNGLVERTEXATTRIBI4BVEXTPROC glVertexAttribI4bvEXT;
  PFNGLVERTEXATTRIBI4SVEXTPROC glVertexAttribI4svEXT;
  PFNGLVERTEXATTRIBI4UBVEXTPROC glVertexAttribI4ubvEXT;
  PFNGLVERTEXATTRIBI4USVEXTPROC glVertexAttribI4usvEXT;
  PFNGLVERTEXATTRIBIPOINTEREXTPROC glVertexAttribIPointerEXT;
  PFNGLGETVERTEXATTRIBIIVEXTPROC glGetVertexAttribIivEXT;
  PFNGLGETVERTEXATTRIBIUIVEXTPROC glGetVertexAttribIuivEXT;
  /// GL_EXT_gpu_shader4           // #326 http://www.opengl.org/registry/specs/EXT/gpu_shader4.txt
  PFNGLGETUNIFORMUIVEXTPROC glGetUniformuivEXT;
  PFNGLBINDFRAGDATALOCATIONEXTPROC glBindFragDataLocationEXT;
  PFNGLGETFRAGDATALOCATIONEXTPROC glGetFragDataLocationEXT;
  PFNGLUNIFORM1UIEXTPROC glUniform1uiEXT;
  PFNGLUNIFORM2UIEXTPROC glUniform2uiEXT;
  PFNGLUNIFORM3UIEXTPROC glUniform3uiEXT;
  PFNGLUNIFORM4UIEXTPROC glUniform4uiEXT;
  PFNGLUNIFORM1UIVEXTPROC glUniform1uivEXT;
  PFNGLUNIFORM2UIVEXTPROC glUniform2uivEXT;
  PFNGLUNIFORM3UIVEXTPROC glUniform3uivEXT;
  PFNGLUNIFORM4UIVEXTPROC glUniform4uivEXT;
  /// GL_EXT_draw_instanced        // #327 http://www.opengl.org/registry/specs/EXT/draw_instanced.txt
  PFNGLDRAWARRAYSINSTANCEDEXTPROC glDrawArraysInstancedEXT;
  PFNGLDRAWELEMENTSINSTANCEDEXTPROC glDrawElementsInstancedEXT;
  /// GL_EXT_texture_buffer_object  // #330 http://www.opengl.org/registry/specs/EXT/texture_buffer_object.txt
  PFNGLTEXBUFFEREXTPROC glTexBufferEXT;
  /// GL_NV_depth_buffer_float     // #334 http://www.opengl.org/registry/specs/NV/depth_buffer_float.txt
  PFNGLDEPTHRANGEDNVPROC glDepthRangedNV;
  PFNGLCLEARDEPTHDNVPROC glClearDepthdNV;
  PFNGLDEPTHBOUNDSDNVPROC glDepthBoundsdNV;
  /// GL_NV_framebuffer_multisample_coverage  // #336 http://www.opengl.org/registry/specs/NV/framebuffer_multisample_coverage.txt
  PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC glRenderbufferStorageMultisampleCoverageNV;
  /// GL_NV_parameter_buffer_object // #339 http://www.opengl.org/registry/specs/NV/parameter_buffer_object.txt
  PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC glProgramBufferParametersfvNV;
  PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC glProgramBufferParametersIivNV;
  PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC glProgramBufferParametersIuivNV;
  /// GL_EXT_draw_buffers2         // #340 http://www.opengl.org/registry/specs/EXT/draw_buffers2.txt
  PFNGLCOLORMASKINDEXEDEXTPROC glColorMaskIndexedEXT;
  /// GL_NV_transform_feedback     // #341 http://www.opengl.org/registry/specs/NV/transform_feedback.txt
  PFNGLBEGINTRANSFORMFEEDBACKNVPROC glBeginTransformFeedbackNV;
  PFNGLENDTRANSFORMFEEDBACKNVPROC glEndTransformFeedbackNV;
  PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC glTransformFeedbackAttribsNV;
  PFNGLBINDBUFFERRANGENVPROC glBindBufferRangeNV;
  PFNGLBINDBUFFEROFFSETNVPROC glBindBufferOffsetNV;
  PFNGLBINDBUFFERBASENVPROC glBindBufferBaseNV;
  PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC glTransformFeedbackVaryingsNV;
  PFNGLACTIVEVARYINGNVPROC glActiveVaryingNV;
  PFNGLGETVARYINGLOCATIONNVPROC glGetVaryingLocationNV;
  PFNGLGETACTIVEVARYINGNVPROC glGetActiveVaryingNV;
  PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC glGetTransformFeedbackVaryingNV;
  PFNGLTRANSFORMFEEDBACKSTREAMATTRIBSNVPROC glTransformFeedbackStreamAttribsNV;
  /// GL_EXT_bindable_uniform      // #342 http://www.opengl.org/registry/specs/EXT/bindable_uniform.txt
  PFNGLUNIFORMBUFFEREXTPROC glUniformBufferEXT;
  PFNGLGETUNIFORMBUFFERSIZEEXTPROC glGetUniformBufferSizeEXT;
  PFNGLGETUNIFORMOFFSETEXTPROC glGetUniformOffsetEXT;
  /// GL_EXT_texture_integer       // #343 http://www.opengl.org/registry/specs/EXT/texture_integer.txt
  PFNGLTEXPARAMETERIIVEXTPROC glTexParameterIivEXT;
  PFNGLTEXPARAMETERIUIVEXTPROC glTexParameterIuivEXT;
  PFNGLGETTEXPARAMETERIIVEXTPROC glGetTexParameterIivEXT;
  PFNGLGETTEXPARAMETERIUIVEXTPROC glGetTexParameterIuivEXT;
  PFNGLCLEARCOLORIIEXTPROC glClearColorIiEXT;
  PFNGLCLEARCOLORIUIEXTPROC glClearColorIuiEXT;
  #ifdef OS_LINUX
  /// GLX_EXT_texture_from_pixmap  // #344 http://www.opengl.org/registry/specs/EXT/texture_from_pixmap.txt
  PFNGLXBINDTEXIMAGEEXTPROC glXBindTexImageEXT;
  PFNGLXRELEASETEXIMAGEEXTPROC glXReleaseTexImageEXT;
  #endif
  /// GL_GREMEDY_frame_terminator  // #345 http://www.opengl.org/registry/specs/GREMEDY/frame_terminator.txt
  PFNGLFRAMETERMINATORGREMEDYPROC glFrameTerminatorGREMEDY;
  /// GL_NV_conditional_render     // #346 http://www.opengl.org/registry/specs/NV/conditional_render.txt
  PFNGLBEGINCONDITIONALRENDERNVPROC glBeginConditionalRenderNV;
  PFNGLENDCONDITIONALRENDERNVPROC glEndConditionalRenderNV;
  /// GL_NV_present_video          // #347 GLX_NV_present_video WGL_NV_present_video http://www.opengl.org/registry/specs/NV/present_video.txt
  PFNGLPRESENTFRAMEKEYEDNVPROC glPresentFrameKeyedNV;
  PFNGLPRESENTFRAMEDUALFILLNVPROC glPresentFrameDualFillNV;
  PFNGLGETVIDEOIVNVPROC glGetVideoivNV;
  PFNGLGETVIDEOUIVNVPROC glGetVideouivNV;
  PFNGLGETVIDEOI64VNVPROC glGetVideoi64vNV;
  PFNGLGETVIDEOUI64VNVPROC glGetVideoui64vNV;
  #ifdef OS_LINUX
  PFNGLXENUMERATEVIDEODEVICESNVPROC glXEnumerateVideoDevicesNV;
  PFNGLXBINDVIDEODEVICENVPROC glXBindVideoDeviceNV;
  #endif /// OS_LINUX
  #ifdef OS_WIN
  PFNWGLENUMERATEVIDEODEVICESNVPROC wglEnumerateVideoDevicesNV;
  PFNWGLBINDVIDEODEVICENVPROC wglBindVideoDeviceNV;
  PFNWGLQUERYCURRENTCONTEXTNVPROC wglQueryCurrentContextNV;
  #endif /// OS_WIN
  #ifdef OS_LINUX
  /// GLX_NV_video_out             // #348 http://www.opengl.org/registry/specs/NV/glx_video_output.txt
  PFNGLXGETVIDEODEVICENVPROC glXGetVideoDeviceNV;
  PFNGLXRELEASEVIDEODEVICENVPROC glXReleaseVideoDeviceNV;
  PFNGLXBINDVIDEOIMAGENVPROC glXBindVideoImageNV;
  PFNGLXRELEASEVIDEOIMAGENVPROC glXReleaseVideoImageNV;
  PFNGLXSENDPBUFFERTOVIDEONVPROC glXSendPbufferToVideoNV;
  PFNGLXGETVIDEOINFONVPROC glXGetVideoInfoNV;
  #endif
  #ifdef OS_WIN
  /// WGL_NV_video_output          // #349 http://www.opengl.org/registry/specs/NV/wgl_video_output.txt
  PFNWGLGETVIDEODEVICENVPROC wglGetVideoDeviceNV;
  PFNWGLRELEASEVIDEODEVICENVPROC wglReleaseVideoDeviceNV;
  PFNWGLBINDVIDEOIMAGENVPROC wglBindVideoImageNV;
  PFNWGLRELEASEVIDEOIMAGENVPROC wglReleaseVideoImageNV;
  PFNWGLSENDPBUFFERTOVIDEONVPROC wglSendPbufferToVideoNV;
  PFNWGLGETVIDEOINFONVPROC wglGetVideoInfoNV;
  #endif
  #ifdef OS_LINUX
  /// GLX_NV_swap_group            // #350 http://www.opengl.org/registry/specs/NV/glx_swap_group.txt
  PFNGLXJOINSWAPGROUPNVPROC glXJoinSwapGroupNV;
  PFNGLXBINDSWAPBARRIERNVPROC glXBindSwapBarrierNV;
  PFNGLXQUERYSWAPGROUPNVPROC glXQuerySwapGroupNV;
  PFNGLXQUERYMAXSWAPGROUPSNVPROC glXQueryMaxSwapGroupsNV;
  PFNGLXQUERYFRAMECOUNTNVPROC glXQueryFrameCountNV;
  PFNGLXRESETFRAMECOUNTNVPROC glXResetFrameCountNV;
  #endif
  #ifdef OS_WIN
  /// WGL_NV_swap_group            // #351 http://www.opengl.org/registry/specs/NV/wgl_swap_group.txt
  PFNWGLJOINSWAPGROUPNVPROC wglJoinSwapGroupNV;
  PFNWGLBINDSWAPBARRIERNVPROC wglBindSwapBarrierNV;
  PFNWGLQUERYSWAPGROUPNVPROC wglQuerySwapGroupNV;
  PFNWGLQUERYMAXSWAPGROUPSNVPROC wglQueryMaxSwapGroupsNV;
  PFNWGLQUERYFRAMECOUNTNVPROC wglQueryFrameCountNV;
  PFNWGLRESETFRAMECOUNTNVPROC wglResetFrameCountNV;
  #endif
  /// GL_EXT_transform_feedback    // #352 http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
  PFNGLBEGINTRANSFORMFEEDBACKEXTPROC glBeginTransformFeedbackEXT;
  PFNGLENDTRANSFORMFEEDBACKEXTPROC glEndTransformFeedbackEXT;
  PFNGLBINDBUFFERRANGEEXTPROC glBindBufferRangeEXT;
  PFNGLBINDBUFFEROFFSETEXTPROC glBindBufferOffsetEXT;
  PFNGLBINDBUFFERBASEEXTPROC glBindBufferBaseEXT;
  PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC glTransformFeedbackVaryingsEXT;
  PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC glGetTransformFeedbackVaryingEXT;
  /// GL_EXT_direct_state_access   // #353 http://www.opengl.org/registry/specs/EXT/direct_state_access.txt
  PFNGLMATRIXLOADFEXTPROC glMatrixLoadfEXT;
  PFNGLMATRIXLOADDEXTPROC glMatrixLoaddEXT;
  PFNGLMATRIXMULTFEXTPROC glMatrixMultfEXT;
  PFNGLMATRIXMULTDEXTPROC glMatrixMultdEXT;
  PFNGLMATRIXLOADIDENTITYEXTPROC glMatrixLoadIdentityEXT;
  PFNGLMATRIXROTATEFEXTPROC glMatrixRotatefEXT;
  PFNGLMATRIXROTATEDEXTPROC glMatrixRotatedEXT;
  PFNGLMATRIXSCALEFEXTPROC glMatrixScalefEXT;
  PFNGLMATRIXSCALEDEXTPROC glMatrixScaledEXT;
  PFNGLMATRIXTRANSLATEFEXTPROC glMatrixTranslatefEXT;
  PFNGLMATRIXTRANSLATEDEXTPROC glMatrixTranslatedEXT;
  PFNGLMATRIXFRUSTUMEXTPROC glMatrixFrustumEXT;
  PFNGLMATRIXORTHOEXTPROC glMatrixOrthoEXT;
  PFNGLMATRIXPOPEXTPROC glMatrixPopEXT;
  PFNGLMATRIXPUSHEXTPROC glMatrixPushEXT;
  PFNGLCLIENTATTRIBDEFAULTEXTPROC glClientAttribDefaultEXT;
  PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC glPushClientAttribDefaultEXT;
  PFNGLTEXTUREPARAMETERFEXTPROC glTextureParameterfEXT;
  PFNGLTEXTUREPARAMETERFVEXTPROC glTextureParameterfvEXT;
  PFNGLTEXTUREPARAMETERIEXTPROC glTextureParameteriEXT;
  PFNGLTEXTUREPARAMETERIVEXTPROC glTextureParameterivEXT;
  PFNGLTEXTUREIMAGE1DEXTPROC glTextureImage1DEXT;
  PFNGLTEXTUREIMAGE2DEXTPROC glTextureImage2DEXT;
  PFNGLTEXTURESUBIMAGE1DEXTPROC glTextureSubImage1DEXT;
  PFNGLTEXTURESUBIMAGE2DEXTPROC glTextureSubImage2DEXT;
  PFNGLCOPYTEXTUREIMAGE1DEXTPROC glCopyTextureImage1DEXT;
  PFNGLCOPYTEXTUREIMAGE2DEXTPROC glCopyTextureImage2DEXT;
  PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC glCopyTextureSubImage1DEXT;
  PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC glCopyTextureSubImage2DEXT;
  PFNGLGETTEXTUREIMAGEEXTPROC glGetTextureImageEXT;
  PFNGLGETTEXTUREPARAMETERFVEXTPROC glGetTextureParameterfvEXT;
  PFNGLGETTEXTUREPARAMETERIVEXTPROC glGetTextureParameterivEXT;
  PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC glGetTextureLevelParameterfvEXT;
  PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC glGetTextureLevelParameterivEXT;
  PFNGLTEXTUREIMAGE3DEXTPROC glTextureImage3DEXT;
  PFNGLTEXTURESUBIMAGE3DEXTPROC glTextureSubImage3DEXT;
  PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC glCopyTextureSubImage3DEXT;
  PFNGLBINDMULTITEXTUREEXTPROC glBindMultiTextureEXT;
  PFNGLMULTITEXCOORDPOINTEREXTPROC glMultiTexCoordPointerEXT;
  PFNGLMULTITEXENVFEXTPROC glMultiTexEnvfEXT;
  PFNGLMULTITEXENVFVEXTPROC glMultiTexEnvfvEXT;
  PFNGLMULTITEXENVIEXTPROC glMultiTexEnviEXT;
  PFNGLMULTITEXENVIVEXTPROC glMultiTexEnvivEXT;
  PFNGLMULTITEXGENDEXTPROC glMultiTexGendEXT;
  PFNGLMULTITEXGENDVEXTPROC glMultiTexGendvEXT;
  PFNGLMULTITEXGENFEXTPROC glMultiTexGenfEXT;
  PFNGLMULTITEXGENFVEXTPROC glMultiTexGenfvEXT;
  PFNGLMULTITEXGENIEXTPROC glMultiTexGeniEXT;
  PFNGLMULTITEXGENIVEXTPROC glMultiTexGenivEXT;
  PFNGLGETMULTITEXENVFVEXTPROC glGetMultiTexEnvfvEXT;
  PFNGLGETMULTITEXENVIVEXTPROC glGetMultiTexEnvivEXT;
  PFNGLGETMULTITEXGENDVEXTPROC glGetMultiTexGendvEXT;
  PFNGLGETMULTITEXGENFVEXTPROC glGetMultiTexGenfvEXT;
  PFNGLGETMULTITEXGENIVEXTPROC glGetMultiTexGenivEXT;
  PFNGLMULTITEXPARAMETERIEXTPROC glMultiTexParameteriEXT;
  PFNGLMULTITEXPARAMETERIVEXTPROC glMultiTexParameterivEXT;
  PFNGLMULTITEXPARAMETERFEXTPROC glMultiTexParameterfEXT;
  PFNGLMULTITEXPARAMETERFVEXTPROC glMultiTexParameterfvEXT;
  PFNGLMULTITEXIMAGE1DEXTPROC glMultiTexImage1DEXT;
  PFNGLMULTITEXIMAGE2DEXTPROC glMultiTexImage2DEXT;
  PFNGLMULTITEXSUBIMAGE1DEXTPROC glMultiTexSubImage1DEXT;
  PFNGLMULTITEXSUBIMAGE2DEXTPROC glMultiTexSubImage2DEXT;
  PFNGLCOPYMULTITEXIMAGE1DEXTPROC glCopyMultiTexImage1DEXT;
  PFNGLCOPYMULTITEXIMAGE2DEXTPROC glCopyMultiTexImage2DEXT;
  PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC glCopyMultiTexSubImage1DEXT;
  PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC glCopyMultiTexSubImage2DEXT;
  PFNGLGETMULTITEXIMAGEEXTPROC glGetMultiTexImageEXT;
  PFNGLGETMULTITEXPARAMETERFVEXTPROC glGetMultiTexParameterfvEXT;
  PFNGLGETMULTITEXPARAMETERIVEXTPROC glGetMultiTexParameterivEXT;
  PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC glGetMultiTexLevelParameterfvEXT;
  PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC glGetMultiTexLevelParameterivEXT;
  PFNGLMULTITEXIMAGE3DEXTPROC glMultiTexImage3DEXT;
  PFNGLMULTITEXSUBIMAGE3DEXTPROC glMultiTexSubImage3DEXT;
  PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC glCopyMultiTexSubImage3DEXT;
  PFNGLENABLECLIENTSTATEINDEXEDEXTPROC glEnableClientStateIndexedEXT;
  PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC glDisableClientStateIndexedEXT;
  PFNGLGETFLOATINDEXEDVEXTPROC glGetFloatIndexedvEXT;
  PFNGLGETDOUBLEINDEXEDVEXTPROC glGetDoubleIndexedvEXT;
  PFNGLGETPOINTERINDEXEDVEXTPROC glGetPointerIndexedvEXT;
  PFNGLENABLEINDEXEDEXTPROC glEnableIndexedEXT;
  PFNGLDISABLEINDEXEDEXTPROC glDisableIndexedEXT;
  PFNGLISENABLEDINDEXEDEXTPROC glIsEnabledIndexedEXT;
  PFNGLGETINTEGERINDEXEDVEXTPROC glGetIntegerIndexedvEXT;
  PFNGLGETBOOLEANINDEXEDVEXTPROC glGetBooleanIndexedvEXT;
  PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC glCompressedTextureImage3DEXT;
  PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC glCompressedTextureImage2DEXT;
  PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC glCompressedTextureImage1DEXT;
  PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC glCompressedTextureSubImage3DEXT;
  PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC glCompressedTextureSubImage2DEXT;
  PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC glCompressedTextureSubImage1DEXT;
  PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC glGetCompressedTextureImageEXT;
  PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC glCompressedMultiTexImage3DEXT;
  PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC glCompressedMultiTexImage2DEXT;
  PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC glCompressedMultiTexImage1DEXT;
  PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC glCompressedMultiTexSubImage3DEXT;
  PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC glCompressedMultiTexSubImage2DEXT;
  PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC glCompressedMultiTexSubImage1DEXT;
  PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC glGetCompressedMultiTexImageEXT;
  PFNGLMATRIXLOADTRANSPOSEFEXTPROC glMatrixLoadTransposefEXT;
  PFNGLMATRIXLOADTRANSPOSEDEXTPROC glMatrixLoadTransposedEXT;
  PFNGLMATRIXMULTTRANSPOSEFEXTPROC glMatrixMultTransposefEXT;
  PFNGLMATRIXMULTTRANSPOSEDEXTPROC glMatrixMultTransposedEXT;
  PFNGLNAMEDBUFFERDATAEXTPROC glNamedBufferDataEXT;
  PFNGLNAMEDBUFFERSUBDATAEXTPROC glNamedBufferSubDataEXT;
  PFNGLMAPNAMEDBUFFEREXTPROC glMapNamedBufferEXT;
  PFNGLUNMAPNAMEDBUFFEREXTPROC glUnmapNamedBufferEXT;
  PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC glGetNamedBufferParameterivEXT;
  PFNGLGETNAMEDBUFFERPOINTERVEXTPROC glGetNamedBufferPointervEXT;
  PFNGLGETNAMEDBUFFERSUBDATAEXTPROC glGetNamedBufferSubDataEXT;
  PFNGLPROGRAMUNIFORM1FEXTPROC glProgramUniform1fEXT;
  PFNGLPROGRAMUNIFORM2FEXTPROC glProgramUniform2fEXT;
  PFNGLPROGRAMUNIFORM3FEXTPROC glProgramUniform3fEXT;
  PFNGLPROGRAMUNIFORM4FEXTPROC glProgramUniform4fEXT;
  PFNGLPROGRAMUNIFORM1IEXTPROC glProgramUniform1iEXT;
  PFNGLPROGRAMUNIFORM2IEXTPROC glProgramUniform2iEXT;
  PFNGLPROGRAMUNIFORM3IEXTPROC glProgramUniform3iEXT;
  PFNGLPROGRAMUNIFORM4IEXTPROC glProgramUniform4iEXT;
  PFNGLPROGRAMUNIFORM1FVEXTPROC glProgramUniform1fvEXT;
  PFNGLPROGRAMUNIFORM2FVEXTPROC glProgramUniform2fvEXT;
  PFNGLPROGRAMUNIFORM3FVEXTPROC glProgramUniform3fvEXT;
  PFNGLPROGRAMUNIFORM4FVEXTPROC glProgramUniform4fvEXT;
  PFNGLPROGRAMUNIFORM1IVEXTPROC glProgramUniform1ivEXT;
  PFNGLPROGRAMUNIFORM2IVEXTPROC glProgramUniform2ivEXT;
  PFNGLPROGRAMUNIFORM3IVEXTPROC glProgramUniform3ivEXT;
  PFNGLPROGRAMUNIFORM4IVEXTPROC glProgramUniform4ivEXT;
  PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC glProgramUniformMatrix2fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC glProgramUniformMatrix3fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC glProgramUniformMatrix4fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC glProgramUniformMatrix2x3fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC glProgramUniformMatrix3x2fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC glProgramUniformMatrix2x4fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC glProgramUniformMatrix4x2fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC glProgramUniformMatrix3x4fvEXT;
  PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC glProgramUniformMatrix4x3fvEXT;
  PFNGLTEXTUREBUFFEREXTPROC glTextureBufferEXT;
  PFNGLMULTITEXBUFFEREXTPROC glMultiTexBufferEXT;
  PFNGLTEXTUREPARAMETERIIVEXTPROC glTextureParameterIivEXT;
  PFNGLTEXTUREPARAMETERIUIVEXTPROC glTextureParameterIuivEXT;
  PFNGLGETTEXTUREPARAMETERIIVEXTPROC glGetTextureParameterIivEXT;
  PFNGLGETTEXTUREPARAMETERIUIVEXTPROC glGetTextureParameterIuivEXT;
  PFNGLMULTITEXPARAMETERIIVEXTPROC glMultiTexParameterIivEXT;
  PFNGLMULTITEXPARAMETERIUIVEXTPROC glMultiTexParameterIuivEXT;
  PFNGLGETMULTITEXPARAMETERIIVEXTPROC glGetMultiTexParameterIivEXT;
  PFNGLGETMULTITEXPARAMETERIUIVEXTPROC glGetMultiTexParameterIuivEXT;
  PFNGLPROGRAMUNIFORM1UIEXTPROC glProgramUniform1uiEXT;
  PFNGLPROGRAMUNIFORM2UIEXTPROC glProgramUniform2uiEXT;
  PFNGLPROGRAMUNIFORM3UIEXTPROC glProgramUniform3uiEXT;
  PFNGLPROGRAMUNIFORM4UIEXTPROC glProgramUniform4uiEXT;
  PFNGLPROGRAMUNIFORM1UIVEXTPROC glProgramUniform1uivEXT;
  PFNGLPROGRAMUNIFORM2UIVEXTPROC glProgramUniform2uivEXT;
  PFNGLPROGRAMUNIFORM3UIVEXTPROC glProgramUniform3uivEXT;
  PFNGLPROGRAMUNIFORM4UIVEXTPROC glProgramUniform4uivEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC glNamedProgramLocalParameters4fvEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC glNamedProgramLocalParameterI4iEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC glNamedProgramLocalParameterI4ivEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC glNamedProgramLocalParametersI4ivEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC glNamedProgramLocalParameterI4uiEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC glNamedProgramLocalParameterI4uivEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC glNamedProgramLocalParametersI4uivEXT;
  PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC glGetNamedProgramLocalParameterIivEXT;
  PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC glGetNamedProgramLocalParameterIuivEXT;
  PFNGLENABLECLIENTSTATEIEXTPROC glEnableClientStateiEXT;
  PFNGLDISABLECLIENTSTATEIEXTPROC glDisableClientStateiEXT;
  PFNGLGETFLOATI_VEXTPROC glGetFloati_vEXT;
  PFNGLGETDOUBLEI_VEXTPROC glGetDoublei_vEXT;
  PFNGLGETPOINTERI_VEXTPROC glGetPointeri_vEXT;
  PFNGLNAMEDPROGRAMSTRINGEXTPROC glNamedProgramStringEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC glNamedProgramLocalParameter4dEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC glNamedProgramLocalParameter4dvEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC glNamedProgramLocalParameter4fEXT;
  PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC glNamedProgramLocalParameter4fvEXT;
  PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC glGetNamedProgramLocalParameterdvEXT;
  PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC glGetNamedProgramLocalParameterfvEXT;
  PFNGLGETNAMEDPROGRAMIVEXTPROC glGetNamedProgramivEXT;
  PFNGLGETNAMEDPROGRAMSTRINGEXTPROC glGetNamedProgramStringEXT;
  PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC glNamedRenderbufferStorageEXT;
  PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC glGetNamedRenderbufferParameterivEXT;
  PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glNamedRenderbufferStorageMultisampleEXT;
  PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC glNamedRenderbufferStorageMultisampleCoverageEXT;
  PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC glCheckNamedFramebufferStatusEXT;
  PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC glNamedFramebufferTexture1DEXT;
  PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC glNamedFramebufferTexture2DEXT;
  PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC glNamedFramebufferTexture3DEXT;
  PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC glNamedFramebufferRenderbufferEXT;
  PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetNamedFramebufferAttachmentParameterivEXT;
  PFNGLGENERATETEXTUREMIPMAPEXTPROC glGenerateTextureMipmapEXT;
  PFNGLGENERATEMULTITEXMIPMAPEXTPROC glGenerateMultiTexMipmapEXT;
  PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC glFramebufferDrawBufferEXT;
  PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC glFramebufferDrawBuffersEXT;
  PFNGLFRAMEBUFFERREADBUFFEREXTPROC glFramebufferReadBufferEXT;
  PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC glGetFramebufferParameterivEXT;
  PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC glNamedCopyBufferSubDataEXT;
  PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC glNamedFramebufferTextureEXT;
  PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC glNamedFramebufferTextureLayerEXT;
  PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC glNamedFramebufferTextureFaceEXT;
  PFNGLTEXTURERENDERBUFFEREXTPROC glTextureRenderbufferEXT;
  PFNGLMULTITEXRENDERBUFFEREXTPROC glMultiTexRenderbufferEXT;
  PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC glVertexArrayVertexOffsetEXT;
  PFNGLVERTEXARRAYCOLOROFFSETEXTPROC glVertexArrayColorOffsetEXT;
  PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC glVertexArrayEdgeFlagOffsetEXT;
  PFNGLVERTEXARRAYINDEXOFFSETEXTPROC glVertexArrayIndexOffsetEXT;
  PFNGLVERTEXARRAYNORMALOFFSETEXTPROC glVertexArrayNormalOffsetEXT;
  PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC glVertexArrayTexCoordOffsetEXT;
  PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC glVertexArrayMultiTexCoordOffsetEXT;
  PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC glVertexArrayFogCoordOffsetEXT;
  PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC glVertexArraySecondaryColorOffsetEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC glVertexArrayVertexAttribOffsetEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC glVertexArrayVertexAttribIOffsetEXT;
  PFNGLENABLEVERTEXARRAYEXTPROC glEnableVertexArrayEXT;
  PFNGLDISABLEVERTEXARRAYEXTPROC glDisableVertexArrayEXT;
  PFNGLENABLEVERTEXARRAYATTRIBEXTPROC glEnableVertexArrayAttribEXT;
  PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC glDisableVertexArrayAttribEXT;
  PFNGLGETVERTEXARRAYINTEGERVEXTPROC glGetVertexArrayIntegervEXT;
  PFNGLGETVERTEXARRAYPOINTERVEXTPROC glGetVertexArrayPointervEXT;
  PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC glGetVertexArrayIntegeri_vEXT;
  PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC glGetVertexArrayPointeri_vEXT;
  PFNGLMAPNAMEDBUFFERRANGEEXTPROC glMapNamedBufferRangeEXT;
  PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC glFlushMappedNamedBufferRangeEXT;
  PFNGLNAMEDBUFFERSTORAGEEXTPROC glNamedBufferStorageEXT;
  PFNGLCLEARNAMEDBUFFERDATAEXTPROC glClearNamedBufferDataEXT;
  PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC glClearNamedBufferSubDataEXT;
  PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC glNamedFramebufferParameteriEXT;
  PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC glGetNamedFramebufferParameterivEXT;
  PFNGLPROGRAMUNIFORM1DEXTPROC glProgramUniform1dEXT;
  PFNGLPROGRAMUNIFORM2DEXTPROC glProgramUniform2dEXT;
  PFNGLPROGRAMUNIFORM3DEXTPROC glProgramUniform3dEXT;
  PFNGLPROGRAMUNIFORM4DEXTPROC glProgramUniform4dEXT;
  PFNGLPROGRAMUNIFORM1DVEXTPROC glProgramUniform1dvEXT;
  PFNGLPROGRAMUNIFORM2DVEXTPROC glProgramUniform2dvEXT;
  PFNGLPROGRAMUNIFORM3DVEXTPROC glProgramUniform3dvEXT;
  PFNGLPROGRAMUNIFORM4DVEXTPROC glProgramUniform4dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC glProgramUniformMatrix2dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC glProgramUniformMatrix3dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC glProgramUniformMatrix4dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC glProgramUniformMatrix2x3dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC glProgramUniformMatrix2x4dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC glProgramUniformMatrix3x2dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC glProgramUniformMatrix3x4dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC glProgramUniformMatrix4x2dvEXT;
  PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC glProgramUniformMatrix4x3dvEXT;
  PFNGLTEXTUREBUFFERRANGEEXTPROC glTextureBufferRangeEXT;
  PFNGLTEXTURESTORAGE1DEXTPROC glTextureStorage1DEXT;
  PFNGLTEXTURESTORAGE2DEXTPROC glTextureStorage2DEXT;
  PFNGLTEXTURESTORAGE3DEXTPROC glTextureStorage3DEXT;
  PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC glTextureStorage2DMultisampleEXT;
  PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC glTextureStorage3DMultisampleEXT;
  PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC glVertexArrayBindVertexBufferEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXTPROC glVertexArrayVertexAttribFormatEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXTPROC glVertexArrayVertexAttribIFormatEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXTPROC glVertexArrayVertexAttribLFormatEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXTPROC glVertexArrayVertexAttribBindingEXT;
  PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXTPROC glVertexArrayVertexBindingDivisorEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC glVertexArrayVertexAttribLOffsetEXT;
  PFNGLTEXTUREPAGECOMMITMENTEXTPROC glTexturePageCommitmentEXT;
  PFNGLVERTEXARRAYVERTEXATTRIBDIVISOREXTPROC glVertexArrayVertexAttribDivisorEXT;
  #ifdef OS_WIN
  /// WGL_NV_gpu_affinity          // #355 http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
  PFNWGLENUMGPUSNVPROC wglEnumGpusNV;
  PFNWGLENUMGPUDEVICESNVPROC wglEnumGpuDevicesNV;
  PFNWGLCREATEAFFINITYDCNVPROC wglCreateAffinityDCNV;
  PFNWGLENUMGPUSFROMAFFINITYDCNVPROC wglEnumGpusFromAffinityDCNV;
  PFNWGLDELETEDCNVPROC wglDeleteDCNV;
  #endif
  /// NV_explicit_multisample      // #357 http://www.opengl.org/registry/specs/NV/explicit_multisample.txt
  PFNGLGETMULTISAMPLEFVNVPROC glGetMultisamplefvNV;
  PFNGLSAMPLEMASKINDEXEDNVPROC glSampleMaskIndexedNV;
  PFNGLTEXRENDERBUFFERNVPROC glTexRenderbufferNV;
  /// GL_NV_transform_feedback2    // #358 http://www.opengl.org/registry/specs/NV/transform_feedback2.txt
  PFNGLBINDTRANSFORMFEEDBACKNVPROC glBindTransformFeedbackNV;
  PFNGLDELETETRANSFORMFEEDBACKSNVPROC glDeleteTransformFeedbacksNV;
  PFNGLGENTRANSFORMFEEDBACKSNVPROC glGenTransformFeedbacksNV;
  PFNGLISTRANSFORMFEEDBACKNVPROC glIsTransformFeedbackNV;
  PFNGLPAUSETRANSFORMFEEDBACKNVPROC glPauseTransformFeedbackNV;
  PFNGLRESUMETRANSFORMFEEDBACKNVPROC glResumeTransformFeedbackNV;
  PFNGLDRAWTRANSFORMFEEDBACKNVPROC glDrawTransformFeedbackNV;
  /// GL_AMD_performance_monitor   // #360 http://www.opengl.org/registry/specs/AMD/performance_monitor.txt
  PFNGLGETPERFMONITORGROUPSAMDPROC glGetPerfMonitorGroupsAMD;
  PFNGLGETPERFMONITORCOUNTERSAMDPROC glGetPerfMonitorCountersAMD;
  PFNGLGETPERFMONITORGROUPSTRINGAMDPROC glGetPerfMonitorGroupStringAMD;
  PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC glGetPerfMonitorCounterStringAMD;
  PFNGLGETPERFMONITORCOUNTERINFOAMDPROC glGetPerfMonitorCounterInfoAMD;
  PFNGLGENPERFMONITORSAMDPROC glGenPerfMonitorsAMD;
  PFNGLDELETEPERFMONITORSAMDPROC glDeletePerfMonitorsAMD;
  PFNGLSELECTPERFMONITORCOUNTERSAMDPROC glSelectPerfMonitorCountersAMD;
  PFNGLBEGINPERFMONITORAMDPROC glBeginPerfMonitorAMD;
  PFNGLENDPERFMONITORAMDPROC glEndPerfMonitorAMD;
  PFNGLGETPERFMONITORCOUNTERDATAAMDPROC glGetPerfMonitorCounterDataAMD;
  #ifdef OS_WIN
  /// WGL_AMD_gpu_association      // #361 http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
  PFNWGLGETGPUIDSAMDPROC wglGetGPUIDsAMD;
  PFNWGLGETGPUINFOAMDPROC wglGetGPUInfoAMD;
  PFNWGLGETCONTEXTGPUIDAMDPROC wglGetContextGPUIDAMD;
  PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC wglCreateAssociatedContextAMD;
  PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC wglCreateAssociatedContextAttribsAMD;
  PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC wglDeleteAssociatedContextAMD;
  PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC wglMakeAssociatedContextCurrentAMD;
  PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC wglGetCurrentAssociatedContextAMD;
  PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC wglBlitContextFramebufferAMD;
  #endif
  /// GL_AMD_vertex_shader_tessellator  // #363 http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt
  PFNGLTESSELLATIONFACTORAMDPROC glTessellationFactorAMD;
  PFNGLTESSELLATIONMODEAMDPROC glTessellationModeAMD;
  /// GL_EXT_provoking_vertex      // #364 http://www.opengl.org/registry/specs/EXT/provoking_vertex.txt
  PFNGLPROVOKINGVERTEXEXTPROC glProvokingVertexEXT;
  /// GL_AMD_draw_buffers_blend    // #366 http://www.opengl.org/registry/specs/AMD/draw_buffers_blend.txt
  PFNGLBLENDFUNCINDEXEDAMDPROC glBlendFuncIndexedAMD;
  PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC glBlendFuncSeparateIndexedAMD;
  PFNGLBLENDEQUATIONINDEXEDAMDPROC glBlendEquationIndexedAMD;
  PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC glBlendEquationSeparateIndexedAMD;
  /// GL_APPLE_texture_range       // #367 http://www.opengl.org/registry/specs/APPLE/texture_range.txt
  PFNGLTEXTURERANGEAPPLEPROC glTextureRangeAPPLE;
  PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC glGetTexParameterPointervAPPLE;
  /// GL_APPLE_vertex_program_evaluators  // #369 http://www.opengl.org/registry/specs/APPLE/vertex_program_evaluators.txt
  PFNGLENABLEVERTEXATTRIBAPPLEPROC glEnableVertexAttribAPPLE;
  PFNGLDISABLEVERTEXATTRIBAPPLEPROC glDisableVertexAttribAPPLE;
  PFNGLISVERTEXATTRIBENABLEDAPPLEPROC glIsVertexAttribEnabledAPPLE;
  PFNGLMAPVERTEXATTRIB1DAPPLEPROC glMapVertexAttrib1dAPPLE;
  PFNGLMAPVERTEXATTRIB1FAPPLEPROC glMapVertexAttrib1fAPPLE;
  PFNGLMAPVERTEXATTRIB2DAPPLEPROC glMapVertexAttrib2dAPPLE;
  PFNGLMAPVERTEXATTRIB2FAPPLEPROC glMapVertexAttrib2fAPPLE;
  /// GL_APPLE_object_purgeable    // #371 http://www.opengl.org/registry/specs/APPLE/object_purgeable.txt
  PFNGLOBJECTPURGEABLEAPPLEPROC glObjectPurgeableAPPLE;
  PFNGLOBJECTUNPURGEABLEAPPLEPROC glObjectUnpurgeableAPPLE;
  PFNGLGETOBJECTPARAMETERIVAPPLEPROC glGetObjectParameterivAPPLE;
  /// GL_NV_video_capture          // #374 GLX_NV_video_capture WGL_NV_video_capture http://www.opengl.org/registry/specs/NV/video_capture.txt
  PFNGLBEGINVIDEOCAPTURENVPROC glBeginVideoCaptureNV;
  PFNGLBINDVIDEOCAPTURESTREAMBUFFERNVPROC glBindVideoCaptureStreamBufferNV;
  PFNGLBINDVIDEOCAPTURESTREAMTEXTURENVPROC glBindVideoCaptureStreamTextureNV;
  PFNGLENDVIDEOCAPTURENVPROC glEndVideoCaptureNV;
  PFNGLGETVIDEOCAPTUREIVNVPROC glGetVideoCaptureivNV;
  PFNGLGETVIDEOCAPTURESTREAMIVNVPROC glGetVideoCaptureStreamivNV;
  PFNGLGETVIDEOCAPTURESTREAMFVNVPROC glGetVideoCaptureStreamfvNV;
  PFNGLGETVIDEOCAPTURESTREAMDVNVPROC glGetVideoCaptureStreamdvNV;
  PFNGLVIDEOCAPTURENVPROC glVideoCaptureNV;
  PFNGLVIDEOCAPTURESTREAMPARAMETERIVNVPROC glVideoCaptureStreamParameterivNV;
  PFNGLVIDEOCAPTURESTREAMPARAMETERFVNVPROC glVideoCaptureStreamParameterfvNV;
  PFNGLVIDEOCAPTURESTREAMPARAMETERDVNVPROC glVideoCaptureStreamParameterdvNV;
  #ifdef OS_LINUX
  PFNGLXBINDVIDEOCAPTUREDEVICENVPROC glXBindVideoCaptureDeviceNV;
  PFNGLXENUMERATEVIDEOCAPTUREDEVICESNVPROC glXEnumerateVideoCaptureDevicesNV;
  PFNGLXLOCKVIDEOCAPTUREDEVICENVPROC glXLockVideoCaptureDeviceNV;
  PFNGLXQUERYVIDEOCAPTUREDEVICENVPROC glXQueryVideoCaptureDeviceNV;
  PFNGLXRELEASEVIDEOCAPTUREDEVICENVPROC glXReleaseVideoCaptureDeviceNV;
  #endif /// OS_LINUX
  #ifdef OS_WIN
  PFNWGLBINDVIDEOCAPTUREDEVICENVPROC wglBindVideoCaptureDeviceNV;
  PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC wglEnumerateVideoCaptureDevicesNV;
  PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC wglLockVideoCaptureDeviceNV;
  PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC wglQueryVideoCaptureDeviceNV;
  PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC wglReleaseVideoCaptureDeviceNV;
  #endif
  #ifdef OS_LINUX
  /// GLX_EXT_swap_control         // #375 http://www.opengl.org/registry/specs/EXT/swap_control.txt
  PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;
  #endif
  /// GL_NV_copy_image             // #376 WGL_NV_copy_image GLX_NV_copy_image http://www.opengl.org/registry/specs/NV/copy_image.txt
  PFNGLCOPYIMAGESUBDATANVPROC glCopyImageSubDataNV;
  #ifdef OS_LINUX
  PFNGLXCOPYIMAGESUBDATANVPROC glXCopyImageSubDataNV;
  #endif
  #ifdef OS_WIN
  PFNWGLCOPYIMAGESUBDATANVPROC wglCopyImageSubDataNV;
  #endif
  /// GL_EXT_separate_shader_objects  // #377 http://www.opengl.org/registry/specs/EXT/separate_shader_objects.txt
  PFNGLUSESHADERPROGRAMEXTPROC glUseShaderProgramEXT;
  PFNGLACTIVEPROGRAMEXTPROC glActiveProgramEXT;
  PFNGLCREATESHADERPROGRAMEXTPROC glCreateShaderProgramEXT;
  /// GL_NV_shader_buffer_load     // #379 http://www.opengl.org/registry/specs/NV/shader_buffer_load.txt
  PFNGLMAKEBUFFERRESIDENTNVPROC glMakeBufferResidentNV;
  PFNGLMAKEBUFFERNONRESIDENTNVPROC glMakeBufferNonResidentNV;
  PFNGLISBUFFERRESIDENTNVPROC glIsBufferResidentNV;
  PFNGLMAKENAMEDBUFFERRESIDENTNVPROC glMakeNamedBufferResidentNV;
  PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC glMakeNamedBufferNonResidentNV;
  PFNGLISNAMEDBUFFERRESIDENTNVPROC glIsNamedBufferResidentNV;
  PFNGLGETBUFFERPARAMETERUI64VNVPROC glGetBufferParameterui64vNV;
  PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC glGetNamedBufferParameterui64vNV;
  PFNGLGETINTEGERUI64VNVPROC glGetIntegerui64vNV;
  PFNGLUNIFORMUI64NVPROC glUniformui64NV;
  PFNGLUNIFORMUI64VNVPROC glUniformui64vNV;
  PFNGLPROGRAMUNIFORMUI64NVPROC glProgramUniformui64NV;
  PFNGLPROGRAMUNIFORMUI64VNVPROC glProgramUniformui64vNV;
  /// GL_NV_vertex_buffer_unified_memory  // #380 http://www.opengl.org/registry/specs/NV/vertex_buffer_unified_memory.txt
  PFNGLBUFFERADDRESSRANGENVPROC glBufferAddressRangeNV;
  PFNGLVERTEXFORMATNVPROC glVertexFormatNV;
  PFNGLNORMALFORMATNVPROC glNormalFormatNV;
  PFNGLCOLORFORMATNVPROC glColorFormatNV;
  PFNGLINDEXFORMATNVPROC glIndexFormatNV;
  PFNGLTEXCOORDFORMATNVPROC glTexCoordFormatNV;
  PFNGLEDGEFLAGFORMATNVPROC glEdgeFlagFormatNV;
  PFNGLSECONDARYCOLORFORMATNVPROC glSecondaryColorFormatNV;
  PFNGLFOGCOORDFORMATNVPROC glFogCoordFormatNV;
  PFNGLVERTEXATTRIBFORMATNVPROC glVertexAttribFormatNV;
  PFNGLVERTEXATTRIBIFORMATNVPROC glVertexAttribIFormatNV;
  PFNGLGETINTEGERUI64I_VNVPROC glGetIntegerui64i_vNV;
  /// GL_NV_texture_barrier        // #381 http://www.opengl.org/registry/specs/NV/texture_barrier.txt
  PFNGLTEXTUREBARRIERNVPROC glTextureBarrierNV;
  /// GL_EXT_shader_image_load_store // #386 http://www.opengl.org/registry/specs/EXT/shader_image_load_store.txt
  PFNGLBINDIMAGETEXTUREEXTPROC glBindImageTextureEXT;
  PFNGLMEMORYBARRIEREXTPROC glMemoryBarrierEXT;
  /// GL_EXT_vertex_attrib_64bit   // #387 http://www.opengl.org/registry/specs/EXT/vertex_attrib_64bit.txt
  PFNGLVERTEXATTRIBL1DEXTPROC glVertexAttribL1dEXT;
  PFNGLVERTEXATTRIBL2DEXTPROC glVertexAttribL2dEXT;
  PFNGLVERTEXATTRIBL3DEXTPROC glVertexAttribL3dEXT;
  PFNGLVERTEXATTRIBL4DEXTPROC glVertexAttribL4dEXT;
  PFNGLVERTEXATTRIBL1DVEXTPROC glVertexAttribL1dvEXT;
  PFNGLVERTEXATTRIBL2DVEXTPROC glVertexAttribL2dvEXT;
  PFNGLVERTEXATTRIBL3DVEXTPROC glVertexAttribL3dvEXT;
  PFNGLVERTEXATTRIBL4DVEXTPROC glVertexAttribL4dvEXT;
  PFNGLVERTEXATTRIBLPOINTEREXTPROC glVertexAttribLPointerEXT;
  PFNGLGETVERTEXATTRIBLDVEXTPROC glGetVertexAttribLdvEXT;
  /// GL_NV_gpu_program5           // #388 GL_NV_gpu_program_fp64 http://www.opengl.org/registry/specs/NV/gpu_program5.txt
  PFNGLPROGRAMSUBROUTINEPARAMETERSUIVNVPROC glProgramSubroutineParametersuivNV;
  PFNGLGETPROGRAMSUBROUTINEPARAMETERUIVNVPROC glGetProgramSubroutineParameteruivNV;
  /// GL_NV_vertex_attrib_integer_64bit  // #392 http://www.opengl.org/registry/specs/NV/vertex_attrib_integer_64bit.txt
  PFNGLVERTEXATTRIBL1I64NVPROC glVertexAttribL1i64NV;
  PFNGLVERTEXATTRIBL2I64NVPROC glVertexAttribL2i64NV;
  PFNGLVERTEXATTRIBL3I64NVPROC glVertexAttribL3i64NV;
  PFNGLVERTEXATTRIBL4I64NVPROC glVertexAttribL4i64NV;
  PFNGLVERTEXATTRIBL1I64VNVPROC glVertexAttribL1i64vNV;
  PFNGLVERTEXATTRIBL2I64VNVPROC glVertexAttribL2i64vNV;
  PFNGLVERTEXATTRIBL3I64VNVPROC glVertexAttribL3i64vNV;
  PFNGLVERTEXATTRIBL4I64VNVPROC glVertexAttribL4i64vNV;
  PFNGLVERTEXATTRIBL1UI64NVPROC glVertexAttribL1ui64NV;
  PFNGLVERTEXATTRIBL2UI64NVPROC glVertexAttribL2ui64NV;
  PFNGLVERTEXATTRIBL3UI64NVPROC glVertexAttribL3ui64NV;
  PFNGLVERTEXATTRIBL4UI64NVPROC glVertexAttribL4ui64NV;
  PFNGLVERTEXATTRIBL1UI64VNVPROC glVertexAttribL1ui64vNV;
  PFNGLVERTEXATTRIBL2UI64VNVPROC glVertexAttribL2ui64vNV;
  PFNGLVERTEXATTRIBL3UI64VNVPROC glVertexAttribL3ui64vNV;
  PFNGLVERTEXATTRIBL4UI64VNVPROC glVertexAttribL4ui64vNV;
  PFNGLGETVERTEXATTRIBLI64VNVPROC glGetVertexAttribLi64vNV;
  PFNGLGETVERTEXATTRIBLUI64VNVPROC glGetVertexAttribLui64vNV;
  PFNGLVERTEXATTRIBLFORMATNVPROC glVertexAttribLFormatNV;
  /// GL_AMD_name_gen_delete       // #394 http://www.opengl.org/registry/specs/AMD/name_gen_delete.txt
  PFNGLGENNAMESAMDPROC glGenNamesAMD;
  PFNGLDELETENAMESAMDPROC glDeleteNamesAMD;
  PFNGLISNAMEAMDPROC glIsNameAMD;
  /// GL_AMD_debug_output          // #395 http://www.opengl.org/registry/specs/AMD/debug_output.txt
  PFNGLDEBUGMESSAGEENABLEAMDPROC glDebugMessageEnableAMD;
  PFNGLDEBUGMESSAGEINSERTAMDPROC glDebugMessageInsertAMD;
  PFNGLDEBUGMESSAGECALLBACKAMDPROC glDebugMessageCallbackAMD;
  PFNGLGETDEBUGMESSAGELOGAMDPROC glGetDebugMessageLogAMD;
  /// GL_NV_vdpau_interop          // #396 http://www.opengl.org/registry/specs/NV/vdpau_interop.txt
  PFNGLVDPAUINITNVPROC glVDPAUInitNV;
  PFNGLVDPAUFININVPROC glVDPAUFiniNV;
  PFNGLVDPAUREGISTERVIDEOSURFACENVPROC glVDPAURegisterVideoSurfaceNV;
  PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC glVDPAURegisterOutputSurfaceNV;
  PFNGLVDPAUISSURFACENVPROC glVDPAUIsSurfaceNV;
  PFNGLVDPAUUNREGISTERSURFACENVPROC glVDPAUUnregisterSurfaceNV;
  PFNGLVDPAUGETSURFACEIVNVPROC glVDPAUGetSurfaceivNV;
  PFNGLVDPAUSURFACEACCESSNVPROC glVDPAUSurfaceAccessNV;
  PFNGLVDPAUMAPSURFACESNVPROC glVDPAUMapSurfacesNV;
  PFNGLVDPAUUNMAPSURFACESNVPROC glVDPAUUnmapSurfacesNV;
  /// GL_NV_texture_multisample    // #403 GL_ missing in registry, asuming mistake http://www.opengl.org/registry/specs/NV/texture_multisample.txt
  PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC glTexImage2DMultisampleCoverageNV;
  PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC glTexImage3DMultisampleCoverageNV;
  PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC glTextureImage2DMultisampleNV;
  PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC glTextureImage3DMultisampleNV;
  PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC glTextureImage2DMultisampleCoverageNV;
  PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC glTextureImage3DMultisampleCoverageNV;
  /// GL_AMD_sample_positions      // #405 http://www.opengl.org/registry/specs/AMD/sample_positions.txt
  PFNGLSETMULTISAMPLEFVAMDPROC glSetMultisamplefvAMD;
  /// GL_EXT_x11_sync_object       // #406 http://www.opengl.org/registry/specs/EXT/x11_sync_object.txt
  PFNGLIMPORTSYNCEXTPROC glImportSyncEXT;
  #ifdef OS_WIN
  /// WGL_NV_DX_interop            // #407 http://www.opengl.org/registry/specs/NV/DX_interop.txt
  PFNWGLDXSETRESOURCESHAREHANDLENVPROC wglDXSetResourceShareHandleNV;
  PFNWGLDXOPENDEVICENVPROC wglDXOpenDeviceNV;
  PFNWGLDXCLOSEDEVICENVPROC wglDXCloseDeviceNV;
  PFNWGLDXREGISTEROBJECTNVPROC wglDXRegisterObjectNV;
  PFNWGLDXUNREGISTEROBJECTNVPROC wglDXUnregisterObjectNV;
  PFNWGLDXOBJECTACCESSNVPROC wglDXObjectAccessNV;
  PFNWGLDXLOCKOBJECTSNVPROC wglDXLockObjectsNV;
  PFNWGLDXUNLOCKOBJECTSNVPROC wglDXUnlockObjectsNV;
  #endif
  /// GL_AMD_multi_draw_indirect   // #408 http://www.opengl.org/registry/specs/AMD/multi_draw_indirect.txt
  PFNGLMULTIDRAWARRAYSINDIRECTAMDPROC glMultiDrawArraysIndirectAMD;
  PFNGLMULTIDRAWELEMENTSINDIRECTAMDPROC glMultiDrawElementsIndirectAMD;
  /// GL_NV_path_rendering         // #410 http://www.opengl.org/registry/specs/NV/path_rendering.txt
  PFNGLGENPATHSNVPROC glGenPathsNV;
  PFNGLDELETEPATHSNVPROC glDeletePathsNV;
  PFNGLISPATHNVPROC glIsPathNV;
  PFNGLPATHCOMMANDSNVPROC glPathCommandsNV;
  PFNGLPATHCOORDSNVPROC glPathCoordsNV;
  PFNGLPATHSUBCOMMANDSNVPROC glPathSubCommandsNV;
  PFNGLPATHSUBCOORDSNVPROC glPathSubCoordsNV;
  PFNGLPATHSTRINGNVPROC glPathStringNV;
  PFNGLPATHGLYPHSNVPROC glPathGlyphsNV;
  PFNGLPATHGLYPHRANGENVPROC glPathGlyphRangeNV;
  PFNGLWEIGHTPATHSNVPROC glWeightPathsNV;
  PFNGLCOPYPATHNVPROC glCopyPathNV;
  PFNGLINTERPOLATEPATHSNVPROC glInterpolatePathsNV;
  PFNGLTRANSFORMPATHNVPROC glTransformPathNV;
  PFNGLPATHPARAMETERIVNVPROC glPathParameterivNV;
  PFNGLPATHPARAMETERINVPROC glPathParameteriNV;
  PFNGLPATHPARAMETERFVNVPROC glPathParameterfvNV;
  PFNGLPATHPARAMETERFNVPROC glPathParameterfNV;
  PFNGLPATHDASHARRAYNVPROC glPathDashArrayNV;
  PFNGLPATHSTENCILFUNCNVPROC glPathStencilFuncNV;
  PFNGLPATHSTENCILDEPTHOFFSETNVPROC glPathStencilDepthOffsetNV;
  PFNGLSTENCILFILLPATHNVPROC glStencilFillPathNV;
  PFNGLSTENCILSTROKEPATHNVPROC glStencilStrokePathNV;
  PFNGLSTENCILFILLPATHINSTANCEDNVPROC glStencilFillPathInstancedNV;
  PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC glStencilStrokePathInstancedNV;
  PFNGLPATHCOVERDEPTHFUNCNVPROC glPathCoverDepthFuncNV;
  PFNGLPATHCOLORGENNVPROC glPathColorGenNV;
  PFNGLPATHTEXGENNVPROC glPathTexGenNV;
  PFNGLPATHFOGGENNVPROC glPathFogGenNV;
  PFNGLCOVERFILLPATHNVPROC glCoverFillPathNV;
  PFNGLCOVERSTROKEPATHNVPROC glCoverStrokePathNV;
  PFNGLCOVERFILLPATHINSTANCEDNVPROC glCoverFillPathInstancedNV;
  PFNGLCOVERSTROKEPATHINSTANCEDNVPROC glCoverStrokePathInstancedNV;
  PFNGLGETPATHPARAMETERIVNVPROC glGetPathParameterivNV;
  PFNGLGETPATHPARAMETERFVNVPROC glGetPathParameterfvNV;
  PFNGLGETPATHCOMMANDSNVPROC glGetPathCommandsNV;
  PFNGLGETPATHCOORDSNVPROC glGetPathCoordsNV;
  PFNGLGETPATHDASHARRAYNVPROC glGetPathDashArrayNV;
  PFNGLGETPATHMETRICSNVPROC glGetPathMetricsNV;
  PFNGLGETPATHMETRICRANGENVPROC glGetPathMetricRangeNV;
  PFNGLGETPATHSPACINGNVPROC glGetPathSpacingNV;
  PFNGLGETPATHCOLORGENIVNVPROC glGetPathColorGenivNV;
  PFNGLGETPATHCOLORGENFVNVPROC glGetPathColorGenfvNV;
  PFNGLGETPATHTEXGENIVNVPROC glGetPathTexGenivNV;
  PFNGLGETPATHTEXGENFVNVPROC glGetPathTexGenfvNV;
  PFNGLISPOINTINFILLPATHNVPROC glIsPointInFillPathNV;
  PFNGLISPOINTINSTROKEPATHNVPROC glIsPointInStrokePathNV;
  PFNGLGETPATHLENGTHNVPROC glGetPathLengthNV;
  PFNGLPOINTALONGPATHNVPROC glPointAlongPathNV;
  /// GL_AMD_stencil_operation_extended  // #413 http://www.opengl.org/registry/specs/AMD/stencil_operation_extended.txt
  PFNGLSTENCILOPVALUEAMDPROC glStencilOpValueAMD;
  /// GL_NV_bindless_texture       // #418 http://www.opengl.org/registry/specs/NV/bindless_texture.txt
  PFNGLGETTEXTUREHANDLENVPROC glGetTextureHandleNV;
  PFNGLGETTEXTURESAMPLERHANDLENVPROC glGetTextureSamplerHandleNV;
  PFNGLMAKETEXTUREHANDLERESIDENTNVPROC glMakeTextureHandleResidentNV;
  PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC glMakeTextureHandleNonResidentNV;
  PFNGLGETIMAGEHANDLENVPROC glGetImageHandleNV;
  PFNGLMAKEIMAGEHANDLERESIDENTNVPROC glMakeImageHandleResidentNV;
  PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC glMakeImageHandleNonResidentNV;
  PFNGLUNIFORMHANDLEUI64NVPROC glUniformHandleui64NV;
  PFNGLUNIFORMHANDLEUI64VNVPROC glUniformHandleui64vNV;
  PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC glProgramUniformHandleui64NV;
  PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC glProgramUniformHandleui64vNV;
  PFNGLISTEXTUREHANDLERESIDENTNVPROC glIsTextureHandleResidentNV;
  PFNGLISIMAGEHANDLERESIDENTNVPROC glIsImageHandleResidentNV;
  /// GL_NVX_conditional_render    // #425 http://www.opengl.org/registry/specs/NVX/nvx_conditional_render.txt
  PFNGLBEGINCONDITIONALRENDERNVXPROC glBeginConditionalRenderNVX;
  PFNGLENDCONDITIONALRENDERNVXPROC glEndConditionalRenderNVX;
  /// GL_AMD_sparse_texture        // #426 http://www.opengl.org/registry/specs/AMD/sparse_texture.txt
  PFNGLTEXSTORAGESPARSEAMDPROC glTexStorageSparseAMD;
  PFNGLTEXTURESTORAGESPARSEAMDPROC glTextureStorageSparseAMD;
  /// GL_INTEL_map_texture         // #429 http://www.opengl.org/registry/specs/INTEL/map_texture.txt
  PFNGLSYNCTEXTUREINTELPROC glSyncTextureINTEL;
  PFNGLUNMAPTEXTURE2DINTELPROC glUnmapTexture2DINTEL;
  PFNGLMAPTEXTURE2DINTELPROC glMapTexture2DINTEL;
  /// GL_NV_draw_texture           // #430 http://www.opengl.org/registry/specs/NV/draw_texture.txt
  PFNGLDRAWTEXTURENVPROC glDrawTextureNV;
  /// GL_AMD_interleaved_elements  // #431 http://www.opengl.org/registry/specs/AMD/interleaved_elements.txt
  PFNGLVERTEXATTRIBPARAMETERIAMDPROC glVertexAttribParameteriAMD;
  /// GL_NV_bindless_multi_draw_indirect  // #432 http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect.txt
  PFNGLMULTIDRAWARRAYSINDIRECTBINDLESSNVPROC glMultiDrawArraysIndirectBindlessNV;
  PFNGLMULTIDRAWELEMENTSINDIRECTBINDLESSNVPROC glMultiDrawElementsIndirectBindlessNV;
  /// GL_NV_blend_equation_advanced  // #433 GL_NV_blend_equation_advanced_coherent http://www.opengl.org/registry/specs/NV/blend_equation_advanced.txt
  PFNGLBLENDPARAMETERINVPROC glBlendParameteriNV;
  PFNGLBLENDBARRIERNVPROC glBlendBarrierNV;
  #ifdef OS_WIN
  /// WGL_NV_delay_before_swap     // #436 http://www.opengl.org/registry/specs/NV/wgl_delay_before_swap.txt
  PFNWGLDELAYBEFORESWAPNVPROC wglDelayBeforeSwapNV;
  #endif
  /// GL_EXT_debug_label           // #439 http://www.opengl.org/registry/specs/EXT/EXT_debug_label.txt
  PFNGLLABELOBJECTEXTPROC glLabelObjectEXT;
  PFNGLGETOBJECTLABELEXTPROC glGetObjectLabelEXT;
  /// GL_EXT_debug_marker          // #440 http://www.opengl.org/registry/specs/EXT/EXT_debug_marker.txt
  PFNGLINSERTEVENTMARKEREXTPROC glInsertEventMarkerEXT;
  PFNGLPUSHGROUPMARKEREXTPROC glPushGroupMarkerEXT;
  PFNGLPOPGROUPMARKEREXTPROC glPopGroupMarkerEXT;
  /// GL_AMD_occlusion_query_event // #422 http://www.opengl.org/registry/specs/AMD/occlusion_query_event.txt
  PFNGLQUERYOBJECTPARAMETERUIAMDPROC glQueryObjectParameteruiAMD;
  /// GL_INTEL_performance_query   // #443 http://www.opengl.org/registry/specs/INTEL/performance_query.txt
  PFNGLBEGINPERFQUERYINTELPROC glBeginPerfQueryINTEL;
  PFNGLCREATEPERFQUERYINTELPROC glCreatePerfQueryINTEL;
  PFNGLDELETEPERFQUERYINTELPROC glDeletePerfQueryINTEL;
  PFNGLENDPERFQUERYINTELPROC glEndPerfQueryINTEL;
  PFNGLGETFIRSTPERFQUERYIDINTELPROC glGetFirstPerfQueryIdINTEL;
  PFNGLGETNEXTPERFQUERYIDINTELPROC glGetNextPerfQueryIdINTEL;
  PFNGLGETPERFCOUNTERINFOINTELPROC glGetPerfCounterInfoINTEL;
  PFNGLGETPERFQUERYDATAINTELPROC glGetPerfQueryDataINTEL;
  PFNGLGETPERFQUERYIDBYNAMEINTELPROC glGetPerfQueryIdByNameINTEL;
  PFNGLGETPERFQUERYINFOINTELPROC glGetPerfQueryInfoINTEL;
  /// GL_AMD_gpu_shader_int64      // #451 http://www.opengl.org/registry/specs/AMD/gpu_shader_int64.txt
  PFNGLUNIFORM1I64NVPROC glUniform1i64NV;
  PFNGLUNIFORM2I64NVPROC glUniform2i64NV;
  PFNGLUNIFORM3I64NVPROC glUniform3i64NV;
  PFNGLUNIFORM4I64NVPROC glUniform4i64NV;
  PFNGLUNIFORM1I64VNVPROC glUniform1i64vNV;
  PFNGLUNIFORM2I64VNVPROC glUniform2i64vNV;
  PFNGLUNIFORM3I64VNVPROC glUniform3i64vNV;
  PFNGLUNIFORM4I64VNVPROC glUniform4i64vNV;
  PFNGLUNIFORM1UI64NVPROC glUniform1ui64NV;
  PFNGLUNIFORM2UI64NVPROC glUniform2ui64NV;
  PFNGLUNIFORM3UI64NVPROC glUniform3ui64NV;
  PFNGLUNIFORM4UI64NVPROC glUniform4ui64NV;
  PFNGLUNIFORM1UI64VNVPROC glUniform1ui64vNV;
  PFNGLUNIFORM2UI64VNVPROC glUniform2ui64vNV;
  PFNGLUNIFORM3UI64VNVPROC glUniform3ui64vNV;
  PFNGLUNIFORM4UI64VNVPROC glUniform4ui64vNV;
  PFNGLGETUNIFORMI64VNVPROC glGetUniformi64vNV;
  PFNGLGETUNIFORMUI64VNVPROC glGetUniformui64vNV;
  PFNGLPROGRAMUNIFORM1I64NVPROC glProgramUniform1i64NV;
  PFNGLPROGRAMUNIFORM2I64NVPROC glProgramUniform2i64NV;
  PFNGLPROGRAMUNIFORM3I64NVPROC glProgramUniform3i64NV;
  PFNGLPROGRAMUNIFORM4I64NVPROC glProgramUniform4i64NV;
  PFNGLPROGRAMUNIFORM1I64VNVPROC glProgramUniform1i64vNV;
  PFNGLPROGRAMUNIFORM2I64VNVPROC glProgramUniform2i64vNV;
  PFNGLPROGRAMUNIFORM3I64VNVPROC glProgramUniform3i64vNV;
  PFNGLPROGRAMUNIFORM4I64VNVPROC glProgramUniform4i64vNV;
  PFNGLPROGRAMUNIFORM1UI64NVPROC glProgramUniform1ui64NV;
  PFNGLPROGRAMUNIFORM2UI64NVPROC glProgramUniform2ui64NV;
  PFNGLPROGRAMUNIFORM3UI64NVPROC glProgramUniform3ui64NV;
  PFNGLPROGRAMUNIFORM4UI64NVPROC glProgramUniform4ui64NV;
  PFNGLPROGRAMUNIFORM1UI64VNVPROC glProgramUniform1ui64vNV;
  PFNGLPROGRAMUNIFORM2UI64VNVPROC glProgramUniform2ui64vNV;
  PFNGLPROGRAMUNIFORM3UI64VNVPROC glProgramUniform3ui64vNV;
  PFNGLPROGRAMUNIFORM4UI64VNVPROC glProgramUniform4ui64vNV;
  #endif /// OS_WIN + OS_LINUX
}; 

typedef GlExtFuncs glExtFuncs;

#ifndef OS_WIN
extern GlExtFuncs glExt;        /// one set of function pointers for MAC and LINUX; windows have functions for each renderer
#endif /// OS_LINUX or OS_MAC







///==========================///
// OPENGL EXTENSION FUNCTIONS //
///==========================///
/// all inline, each will call current renderer's extension function

// this one is used to get extension strings in openGL 3.x+
//inline GLAPI const GLubyte *APIENTRY glGetStringi (GLenum name, GLuint index) {
//  return osi.glr->glExt.glGetStringi(name, index); }











#ifndef OS_MAC // OS_WIN + OS_LINUX only

///==========================///
// MODE 1: using inline funcs //
///==========================///


#ifndef OSI_USING_GLEXT_DEFINES

///====================///
// OpenGL version funcs //
///====================///

#include "osiRenderer.h"




/// OpenGL 1.2 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawRangeElements (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) {
            _glr->glExt.glDrawRangeElements (       mode,        start,        end,         count,        type,             indices); }
inline GLAPI void APIENTRY glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexImage3D (target, level, internalformat, width, height, depth, border, format, type, pixels); }
inline GLAPI void APIENTRY glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexSubImage3D (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels); }
inline GLAPI void APIENTRY glCopyTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyTexSubImage3D (target, level, xoffset, yoffset, zoffset, x, y, width, height); }

/// OpenGL 1.3 funcs =================------------------------------
inline GLAPI void APIENTRY glActiveTexture (GLenum texture) {
  _glr->glExt.glActiveTexture (texture); }
inline GLAPI void APIENTRY glSampleCoverage (GLfloat value, GLboolean invert) {
  _glr->glExt.glSampleCoverage (value, invert); }
inline GLAPI void APIENTRY glCompressedTexImage3D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexImage3D (target, level, internalformat, width, height, depth, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexImage2D (target, level, internalformat, width, height, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexImage1D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexImage1D (target, level, internalformat, width, border, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexSubImage3D (target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexSubImage2D (target, level, xoffset, yoffset, width, height, format, imageSize, data); }
inline GLAPI void APIENTRY glCompressedTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexSubImage1D (target, level, xoffset, width, format, imageSize, data); }
inline GLAPI void APIENTRY glGetCompressedTexImage (GLenum target, GLint level, void *img) {
  _glr->glExt.glGetCompressedTexImage (target, level, img); }
#ifdef OS_WIN
inline GLAPI void APIENTRY glClientActiveTexture (GLenum texture) {
  _glr->glExt.glClientActiveTexture (texture); }
inline GLAPI void APIENTRY glMultiTexCoord1d (GLenum target, GLdouble s) {
  _glr->glExt.glMultiTexCoord1d (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1dv (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord1dv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord1f (GLenum target, GLfloat s) {
  _glr->glExt.glMultiTexCoord1f (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1fv (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord1fv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord1i (GLenum target, GLint s) {
  _glr->glExt.glMultiTexCoord1i (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1iv (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord1iv (target, v); } 
inline GLAPI void APIENTRY glMultiTexCoord1s (GLenum target, GLshort s) {
  _glr->glExt.glMultiTexCoord1s (target, s); }
inline GLAPI void APIENTRY glMultiTexCoord1sv (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord1sv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2d (GLenum target, GLdouble s, GLdouble t) {
  _glr->glExt.glMultiTexCoord2d (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2dv (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord2dv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2f (GLenum target, GLfloat s, GLfloat t) {
  _glr->glExt.glMultiTexCoord2f (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2fv (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord2fv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2i (GLenum target, GLint s, GLint t) {
  _glr->glExt.glMultiTexCoord2i (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2iv (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord2iv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord2s (GLenum target, GLshort s, GLshort t) {
  _glr->glExt.glMultiTexCoord2s (target, s, t); }
inline GLAPI void APIENTRY glMultiTexCoord2sv (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord2sv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord3d (GLenum target, GLdouble s, GLdouble t, GLdouble r) {
  _glr->glExt.glMultiTexCoord3d (target, s, t, r); }
inline GLAPI void APIENTRY glMultiTexCoord3dv (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord3dv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord3f (GLenum target, GLfloat s, GLfloat t, GLfloat r) {
  _glr->glExt.glMultiTexCoord3f (target, s, t, r); }
inline GLAPI void APIENTRY glMultiTexCoord3fv (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord3fv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord3i (GLenum target, GLint s, GLint t, GLint r) {
  _glr->glExt.glMultiTexCoord3i (target, s, t, r); }
inline GLAPI void APIENTRY glMultiTexCoord3iv (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord3iv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3s (GLenum target, GLshort s, GLshort t, GLshort r) {
  _glr->glExt.glMultiTexCoord3s (target,  s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3sv (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord3sv (target, v); }
inline GLAPI void APIENTRY glMultiTexCoord4d (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q) {
  _glr->glExt.glMultiTexCoord4d (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4dv (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord4dv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
  _glr->glExt.glMultiTexCoord4f (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4fv (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord4fv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4i (GLenum target, GLint s, GLint t, GLint r, GLint q) {
  _glr->glExt.glMultiTexCoord4i (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4iv (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord4iv (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4s (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q) {
  _glr->glExt.glMultiTexCoord4s (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4sv (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord4sv (target, v);}
inline GLAPI void APIENTRY glLoadTransposeMatrixf (const GLfloat *m) {
  _glr->glExt.glLoadTransposeMatrixf (m);}
inline GLAPI void APIENTRY glLoadTransposeMatrixd (const GLdouble *m) {
  _glr->glExt.glLoadTransposeMatrixd (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixf (const GLfloat *m) {
  _glr->glExt.glMultTransposeMatrixf (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixd (const GLdouble *m) {
  _glr->glExt.glMultTransposeMatrixd (m);}
#endif /// OS_WIN

/// OpenGL 1.4 funcs =================------------------------------
inline GLAPI void APIENTRY glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
  _glr->glExt.glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);}
inline GLAPI void APIENTRY glMultiDrawArrays (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) {
  _glr->glExt.glMultiDrawArrays (mode, first, count, drawcount);}
inline GLAPI void APIENTRY glMultiDrawElements (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount) {
  _glr->glExt.glMultiDrawElements (mode, count, type, indices, drawcount);}
inline GLAPI void APIENTRY glPointParameterf (GLenum pname, GLfloat param) {
  _glr->glExt.glPointParameterf (pname, param);}
inline GLAPI void APIENTRY glPointParameterfv (GLenum pname, const GLfloat *params) {
  _glr->glExt.glPointParameterfv (pname, params);}
inline GLAPI void APIENTRY glPointParameteri (GLenum pname, GLint param) {
  _glr->glExt.glPointParameteri (pname, param);}
inline GLAPI void APIENTRY glPointParameteriv (GLenum pname, const GLint *params) {
  _glr->glExt.glPointParameteriv (pname, params);}
inline GLAPI void APIENTRY glFogCoordf (GLfloat coord) {
  _glr->glExt.glFogCoordf (coord);}
inline GLAPI void APIENTRY glFogCoordfv (const GLfloat *coord) {
  _glr->glExt.glFogCoordfv (coord);}
inline GLAPI void APIENTRY glFogCoordd (GLdouble coord) {
  _glr->glExt.glFogCoordd (coord);}
inline GLAPI void APIENTRY glFogCoorddv (const GLdouble *coord) {
  _glr->glExt.glFogCoorddv (coord);}
inline GLAPI void APIENTRY glFogCoordPointer (GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glFogCoordPointer (type, stride, pointer);}
inline GLAPI void APIENTRY glSecondaryColor3b (GLbyte red, GLbyte green, GLbyte blue) {
  _glr->glExt.glSecondaryColor3b (red, green,  blue);}
inline GLAPI void APIENTRY glSecondaryColor3bv (const GLbyte *v) {
  _glr->glExt.glSecondaryColor3bv (v);}
inline GLAPI void APIENTRY glSecondaryColor3d (GLdouble red, GLdouble green, GLdouble blue) {
  _glr->glExt.glSecondaryColor3d (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3dv (const GLdouble *v) {
  _glr->glExt.glSecondaryColor3dv (v);}
inline GLAPI void APIENTRY glSecondaryColor3f (GLfloat red, GLfloat green, GLfloat blue) {
  _glr->glExt.glSecondaryColor3f (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3fv (const GLfloat *v) {
  _glr->glExt.glSecondaryColor3fv (v);}
inline GLAPI void APIENTRY glSecondaryColor3i (GLint red, GLint green, GLint blue) {
  _glr->glExt.glSecondaryColor3i (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3iv (const GLint *v) {
  _glr->glExt.glSecondaryColor3iv (v);}
inline GLAPI void APIENTRY glSecondaryColor3s (GLshort red, GLshort green, GLshort blue) {
  _glr->glExt.glSecondaryColor3s (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3sv (const GLshort *v) {
  _glr->glExt.glSecondaryColor3sv (v);}
inline GLAPI void APIENTRY glSecondaryColor3ub (GLubyte red, GLubyte green, GLubyte blue) {
  _glr->glExt.glSecondaryColor3ub (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3ubv (const GLubyte *v) {
  _glr->glExt.glSecondaryColor3ubv (v);}
inline GLAPI void APIENTRY glSecondaryColor3ui (GLuint red, GLuint green, GLuint blue) {
  _glr->glExt.glSecondaryColor3ui (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3uiv (const GLuint *v) {
  _glr->glExt.glSecondaryColor3uiv (v);}
inline GLAPI void APIENTRY glSecondaryColor3us (GLushort red, GLushort green, GLushort blue) {
  _glr->glExt.glSecondaryColor3us (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3usv (const GLushort *v) {
  _glr->glExt.glSecondaryColor3usv (v);}
inline GLAPI void APIENTRY glSecondaryColorPointer (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glSecondaryColorPointer (size, type, stride, pointer);}
inline GLAPI void APIENTRY glWindowPos2d (GLdouble x, GLdouble y) {
  _glr->glExt.glWindowPos2d (x, y);}
inline GLAPI void APIENTRY glWindowPos2dv (const GLdouble *v) {
  _glr->glExt.glWindowPos2dv (v);}
inline GLAPI void APIENTRY glWindowPos2f (GLfloat x, GLfloat y) {
  _glr->glExt.glWindowPos2f (x, y); }
inline GLAPI void APIENTRY glWindowPos2fv (const GLfloat *v) {
  _glr->glExt.glWindowPos2fv (v);}
inline GLAPI void APIENTRY glWindowPos2i (GLint x, GLint y) {
  _glr->glExt.glWindowPos2i (x, y);}
inline GLAPI void APIENTRY glWindowPos2iv (const GLint *v) {
  _glr->glExt.glWindowPos2iv (v);}
inline GLAPI void APIENTRY glWindowPos2s (GLshort x, GLshort y) {
  _glr->glExt.glWindowPos2s (x, y);}
inline GLAPI void APIENTRY glWindowPos2sv (const GLshort *v) {
  _glr->glExt.glWindowPos2sv (v);}
inline GLAPI void APIENTRY glWindowPos3d (GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glWindowPos3d (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3dv (const GLdouble *v) {
  _glr->glExt.glWindowPos3dv (v);}
inline GLAPI void APIENTRY glWindowPos3f (GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glWindowPos3f (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3fv (const GLfloat *v) {
  _glr->glExt.glWindowPos3fv (v);}
inline GLAPI void APIENTRY glWindowPos3i (GLint x, GLint y, GLint z) {
  _glr->glExt.glWindowPos3i (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3iv (const GLint *v) {
  _glr->glExt.glWindowPos3iv (v);}
inline GLAPI void APIENTRY glWindowPos3s (GLshort x, GLshort y, GLshort z) {
  _glr->glExt.glWindowPos3s (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3sv (const GLshort *v) {
  _glr->glExt.glWindowPos3sv (v);}
inline GLAPI void APIENTRY glBlendColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  _glr->glExt.glBlendColor (red, green, blue, alpha);}
inline GLAPI void APIENTRY glBlendEquation (GLenum mode) {
  _glr->glExt.glBlendEquation (mode);}

/// OpenGL 1.5 funcs =================------------------------------
inline GLAPI void APIENTRY glGenQueries (GLsizei n, GLuint *ids) {
  _glr->glExt.glGenQueries (n, ids);}
inline GLAPI void APIENTRY glDeleteQueries (GLsizei n, const GLuint *ids) {
  _glr->glExt.glDeleteQueries (n, ids);}
inline GLAPI GLboolean APIENTRY glIsQuery (GLuint id) {
  return _glr->glExt.glIsQuery (id);}
inline GLAPI void APIENTRY glBeginQuery (GLenum target, GLuint id) {
  _glr->glExt.glBeginQuery (target, id);}
inline GLAPI void APIENTRY glEndQuery (GLenum target) {
  _glr->glExt.glEndQuery (target);}
inline GLAPI void APIENTRY glGetQueryiv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetQueryiv (target, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectiv (GLuint id, GLenum pname, GLint *params) {
  _glr->glExt.glGetQueryObjectiv (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectuiv (GLuint id, GLenum pname, GLuint *params) {
  _glr->glExt.glGetQueryObjectuiv (id, pname, params);}
inline GLAPI void APIENTRY glBindBuffer (GLenum target, GLuint buffer) {
  _glr->glExt.glBindBuffer (target, buffer);}
inline GLAPI void APIENTRY glDeleteBuffers (GLsizei n, const GLuint *buffers) {
  _glr->glExt.glDeleteBuffers (n, buffers);}
inline GLAPI void APIENTRY glGenBuffers (GLsizei n, GLuint *buffers) {
  _glr->glExt.glGenBuffers (n, buffers);}
inline GLAPI GLboolean APIENTRY glIsBuffer (GLuint buffer) {
  return _glr->glExt.glIsBuffer (buffer);}
inline GLAPI void APIENTRY glBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
  _glr->glExt.glBufferData (target, size, data, usage);}
inline GLAPI void APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void *data) {
  _glr->glExt.glBufferSubData (target, offset, size, data);}
inline GLAPI void APIENTRY glGetBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, void *data) {
  _glr->glExt.glGetBufferSubData (target, offset, size, data);}
inline GLAPI void *APIENTRY glMapBuffer (GLenum target, GLenum access) {
  return _glr->glExt.glMapBuffer (target, access);}
inline GLAPI GLboolean APIENTRY glUnmapBuffer (GLenum target) {
  return _glr->glExt.glUnmapBuffer (target);}
inline GLAPI void APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetBufferParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetBufferPointerv (GLenum target, GLenum pname, void **params) {
  _glr->glExt.glGetBufferPointerv (target, pname, params);}

/// OpenGL 2.0 funcs =================------------------------------
inline GLAPI void APIENTRY glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha) {
  _glr->glExt.glBlendEquationSeparate (modeRGB, modeAlpha);}
inline GLAPI void APIENTRY glDrawBuffers (GLsizei n, const GLenum *bufs) {
  _glr->glExt.glDrawBuffers (n, bufs);}
inline GLAPI void APIENTRY glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
  _glr->glExt.glStencilOpSeparate (face, sfail, dpfail, dppass);}
inline GLAPI void APIENTRY glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask) {
  _glr->glExt.glStencilFuncSeparate (face, func, ref, mask);}
inline GLAPI void APIENTRY glStencilMaskSeparate (GLenum face, GLuint mask) {
  _glr->glExt.glStencilMaskSeparate (face, mask);}
inline GLAPI void APIENTRY glAttachShader (GLuint program, GLuint shader) {
  _glr->glExt.glAttachShader (program, shader);}
inline GLAPI void APIENTRY glBindAttribLocation (GLuint program, GLuint index, const GLchar *name) {
  _glr->glExt.glBindAttribLocation (program, index, name);}
inline GLAPI void APIENTRY glCompileShader (GLuint shader) {
  _glr->glExt.glCompileShader (shader);}
inline GLAPI GLuint APIENTRY glCreateProgram (void) {
  return _glr->glExt.glCreateProgram ();}
inline GLAPI GLuint APIENTRY glCreateShader (GLenum type) {
  return _glr->glExt.glCreateShader (type);}
inline GLAPI void APIENTRY glDeleteProgram (GLuint program) {
  _glr->glExt.glDeleteProgram (program);}
inline GLAPI void APIENTRY glDeleteShader (GLuint shader) {
  _glr->glExt.glDeleteShader (shader);}
inline GLAPI void APIENTRY glDetachShader (GLuint program, GLuint shader) {
  _glr->glExt.glDetachShader (program, shader);}
inline GLAPI void APIENTRY glDisableVertexAttribArray (GLuint index) {
  _glr->glExt.glDisableVertexAttribArray (index);}
inline GLAPI void APIENTRY glEnableVertexAttribArray (GLuint index) {
  _glr->glExt.glEnableVertexAttribArray (index);}
inline GLAPI void APIENTRY glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
  _glr->glExt.glGetActiveAttrib (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
  _glr->glExt.glGetActiveUniform (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) {
  _glr->glExt.glGetAttachedShaders (program, maxCount, count, shaders);}
inline GLAPI GLint APIENTRY glGetAttribLocation (GLuint program, const GLchar *name) {
  return _glr->glExt.glGetAttribLocation (program, name);}
inline GLAPI void APIENTRY glGetProgramiv (GLuint program, GLenum pname, GLint *params) {
  _glr->glExt.glGetProgramiv (program, pname, params);}
inline GLAPI void APIENTRY glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
  _glr->glExt.glGetProgramInfoLog (program, bufSize, length, infoLog);}
inline GLAPI void APIENTRY glGetShaderiv (GLuint shader, GLenum pname, GLint *params) {
  _glr->glExt.glGetShaderiv (shader, pname, params);}
inline GLAPI void APIENTRY glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
  _glr->glExt.glGetShaderInfoLog (shader, bufSize, length, infoLog);}
inline GLAPI void APIENTRY glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) {
  _glr->glExt.glGetShaderSource (shader, bufSize, length, source);}
inline GLAPI GLint APIENTRY glGetUniformLocation (GLuint program, const GLchar *name) {
  return _glr->glExt.glGetUniformLocation (program, name);}
inline GLAPI void APIENTRY glGetUniformfv (GLuint program, GLint location, GLfloat *params) {
  _glr->glExt.glGetUniformfv (program, location, params);}
inline GLAPI void APIENTRY glGetUniformiv (GLuint program, GLint location, GLint *params) {
  _glr->glExt.glGetUniformiv (program, location, params);}
inline GLAPI void APIENTRY glGetVertexAttribdv (GLuint index, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetVertexAttribdv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetVertexAttribfv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetVertexAttribiv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribPointerv (GLuint index, GLenum pname, void **pointer) {
  _glr->glExt.glGetVertexAttribPointerv (index, pname, pointer);}
inline GLAPI GLboolean APIENTRY glIsProgram (GLuint program) {
  return _glr->glExt.glIsProgram (program);}
inline GLAPI GLboolean APIENTRY glIsShader (GLuint shader) {
  return _glr->glExt.glIsShader (shader);}
inline GLAPI void APIENTRY glLinkProgram (GLuint program) {
  _glr->glExt.glLinkProgram (program);}
inline GLAPI void APIENTRY glShaderSource (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) {
  _glr->glExt.glShaderSource (shader, count, string, length);}
inline GLAPI void APIENTRY glUseProgram (GLuint program) {
  _glr->glExt.glUseProgram (program);}
inline GLAPI void APIENTRY glUniform1f (GLint location, GLfloat v0) {
  _glr->glExt.glUniform1f (location, v0);}
inline GLAPI void APIENTRY glUniform2f (GLint location, GLfloat v0, GLfloat v1) {
  _glr->glExt.glUniform2f (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  _glr->glExt.glUniform3f (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  _glr->glExt.glUniform4f (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1i (GLint location, GLint v0) {
  _glr->glExt.glUniform1i (location, v0);}
inline GLAPI void APIENTRY glUniform2i (GLint location, GLint v0, GLint v1) {
  _glr->glExt.glUniform2i (location,  v0,  v1);}
inline GLAPI void APIENTRY glUniform3i (GLint location, GLint v0, GLint v1, GLint v2) {
  _glr->glExt.glUniform3i (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  _glr->glExt.glUniform4i (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1fv (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform1fv (location, count, value);}
inline GLAPI void APIENTRY glUniform2fv (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform2fv (location, count, value);}
inline GLAPI void APIENTRY glUniform3fv (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform3fv (location, count, value);}
inline GLAPI void APIENTRY glUniform4fv (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform4fv (location, count, value);}
inline GLAPI void APIENTRY glUniform1iv (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform1iv (location, count, value);}
inline GLAPI void APIENTRY glUniform2iv (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform2iv (location, count, value);}
inline GLAPI void APIENTRY glUniform3iv (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform3iv (location, count, value);}
inline GLAPI void APIENTRY glUniform4iv (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform4iv (location, count, value);}
inline GLAPI void APIENTRY glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix2fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix3fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix4fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glValidateProgram (GLuint program) {
  _glr->glExt.glValidateProgram (program);}
inline GLAPI void APIENTRY glVertexAttrib1d (GLuint index, GLdouble x) {
  _glr->glExt.glVertexAttrib1d (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib1dv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1f (GLuint index, GLfloat x) {
  _glr->glExt.glVertexAttrib1f (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1fv (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib1fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1s (GLuint index, GLshort x) {
  _glr->glExt.glVertexAttrib1s (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1sv (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib1sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2d (GLuint index, GLdouble x, GLdouble y) {
  _glr->glExt.glVertexAttrib2d (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib2dv (index,  v);}
inline GLAPI void APIENTRY glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y) {
  _glr->glExt.glVertexAttrib2f (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2fv (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib2fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2s (GLuint index, GLshort x, GLshort y) {
  _glr->glExt.glVertexAttrib2s (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2sv (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib2sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3d (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glVertexAttrib3d (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib3dv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glVertexAttrib3f (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3fv (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib3fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3s (GLuint index, GLshort x, GLshort y, GLshort z) {
  _glr->glExt.glVertexAttrib3s (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3sv (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib3sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nbv (GLuint index, const GLbyte *v) {
  _glr->glExt.glVertexAttrib4Nbv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Niv (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttrib4Niv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nsv (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib4Nsv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nub (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
  _glr->glExt.glVertexAttrib4Nub (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4Nubv (GLuint index, const GLubyte *v) {
  _glr->glExt.glVertexAttrib4Nubv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nuiv (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttrib4Nuiv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4Nusv (GLuint index, const GLushort *v) {
  _glr->glExt.glVertexAttrib4Nusv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4bv (GLuint index, const GLbyte *v) {
  _glr->glExt.glVertexAttrib4bv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4d (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glVertexAttrib4d (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib4dv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glVertexAttrib4f (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4fv (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib4fv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4iv (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttrib4iv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4s (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
  _glr->glExt.glVertexAttrib4s (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4sv (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib4sv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ubv (GLuint index, const GLubyte *v) {
  _glr->glExt.glVertexAttrib4ubv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4uiv (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttrib4uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4usv (GLuint index, const GLushort *v) {
  _glr->glExt.glVertexAttrib4usv (index, v);}
inline GLAPI void APIENTRY glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexAttribPointer (index, size, type, normalized, stride, pointer);}

/// OpenGL 2.1 funcs =================------------------------------
inline GLAPI void APIENTRY glUniformMatrix2x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix2x3fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix3x2fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix2x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix2x4fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix4x2fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix3x4fv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix4x3fv (location, count, transpose, value);}

/// OpenGL 3.0 funcs =================------------------------------
inline GLAPI void APIENTRY glColorMaski (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
  _glr->glExt.glColorMaski (index, r, g, b, a);}
inline GLAPI void APIENTRY glGetBooleani_v (GLenum target, GLuint index, GLboolean *data) {
  _glr->glExt.glGetBooleani_v (target, index, data);}
inline GLAPI void APIENTRY glGetIntegeri_v (GLenum target, GLuint index, GLint *data) {
  _glr->glExt.glGetIntegeri_v (target, index, data);}
inline GLAPI void APIENTRY glEnablei (GLenum target, GLuint index) {
  _glr->glExt.glEnablei (target, index);}
inline GLAPI void APIENTRY glDisablei (GLenum target, GLuint index) {
  _glr->glExt.glDisablei (target, index);}
inline GLAPI GLboolean APIENTRY glIsEnabledi (GLenum target, GLuint index) {
  return _glr->glExt.glIsEnabledi (target, index);}
inline GLAPI void APIENTRY glBeginTransformFeedback (GLenum primitiveMode) {
  _glr->glExt.glBeginTransformFeedback (primitiveMode);}
inline GLAPI void APIENTRY glEndTransformFeedback (void) {
  _glr->glExt.glEndTransformFeedback ();}
inline GLAPI void APIENTRY glBindBufferRange (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  _glr->glExt.glBindBufferRange (target, index, buffer,  offset, size);}
inline GLAPI void APIENTRY glBindBufferBase (GLenum target, GLuint index, GLuint buffer) {
  _glr->glExt.glBindBufferBase (target, index, buffer);}
inline GLAPI void APIENTRY glTransformFeedbackVaryings (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode) {
  _glr->glExt.glTransformFeedbackVaryings (program, count, varyings, bufferMode);}
inline GLAPI void APIENTRY glGetTransformFeedbackVarying (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
  _glr->glExt.glGetTransformFeedbackVarying (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glClampColor (GLenum target, GLenum clamp) {
  _glr->glExt.glClampColor (target, clamp);}
inline GLAPI void APIENTRY glBeginConditionalRender (GLuint id, GLenum mode) {
  _glr->glExt.glBeginConditionalRender (id, mode);}
inline GLAPI void APIENTRY glEndConditionalRender (void) {
  _glr->glExt.glEndConditionalRender ();}
inline GLAPI void APIENTRY glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexAttribIPointer (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribIiv (GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetVertexAttribIiv (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribIuiv (GLuint index, GLenum pname, GLuint *params) {
  _glr->glExt.glGetVertexAttribIuiv (index, pname, params);}
inline GLAPI void APIENTRY glVertexAttribI1i (GLuint index, GLint x) {
  _glr->glExt.glVertexAttribI1i (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2i (GLuint index, GLint x, GLint y) {
  _glr->glExt.glVertexAttribI2i (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3i (GLuint index, GLint x, GLint y, GLint z) {
  _glr->glExt.glVertexAttribI3i (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4i (GLuint index, GLint x, GLint y, GLint z, GLint w) {
  _glr->glExt.glVertexAttribI4i (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1ui (GLuint index, GLuint x) {
  _glr->glExt.glVertexAttribI1ui (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2ui (GLuint index, GLuint x, GLuint y) {
  _glr->glExt.glVertexAttribI2ui (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3ui (GLuint index, GLuint x, GLuint y, GLuint z) {
  _glr->glExt.glVertexAttribI3ui (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4ui (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  _glr->glExt.glVertexAttribI4ui (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1iv (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI1iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2iv (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI2iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3iv (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI3iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4iv (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI4iv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI1uiv (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI1uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2uiv (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI2uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3uiv (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI3uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4uiv (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI4uiv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4bv (GLuint index, const GLbyte *v) {
  _glr->glExt.glVertexAttribI4bv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4sv (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttribI4sv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4ubv (GLuint index, const GLubyte *v) {
  _glr->glExt.glVertexAttribI4ubv (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4usv (GLuint index, const GLushort *v) {
  _glr->glExt.glVertexAttribI4usv (index, v);}
inline GLAPI void APIENTRY glGetUniformuiv (GLuint program, GLint location, GLuint *params) {
  _glr->glExt.glGetUniformuiv (program, location, params);}
inline GLAPI void APIENTRY glBindFragDataLocation (GLuint program, GLuint color, const GLchar *name) {
  _glr->glExt.glBindFragDataLocation (program, color, name);}
inline GLAPI GLint APIENTRY glGetFragDataLocation (GLuint program, const GLchar *name) {
  return _glr->glExt.glGetFragDataLocation (program, name);}
inline GLAPI void APIENTRY glUniform1ui (GLint location, GLuint v0) {
  _glr->glExt.glUniform1ui (location, v0);}
inline GLAPI void APIENTRY glUniform2ui (GLint location, GLuint v0, GLuint v1) {
  _glr->glExt.glUniform2ui (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2) {
  _glr->glExt.glUniform3ui (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  _glr->glExt.glUniform4ui (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1uiv (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform1uiv (location, count, value);}
inline GLAPI void APIENTRY glUniform2uiv (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform2uiv (location, count, value);}
inline GLAPI void APIENTRY glUniform3uiv (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform3uiv (location,  count, value);}
inline GLAPI void APIENTRY glUniform4uiv (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform4uiv (location, count, value);}
inline GLAPI void APIENTRY glTexParameterIiv (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glTexParameterIiv (target, pname, params);}
inline GLAPI void APIENTRY glTexParameterIuiv (GLenum target, GLenum pname, const GLuint *params) {
  _glr->glExt.glTexParameterIuiv (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIiv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetTexParameterIiv (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIuiv (GLenum target, GLenum pname, GLuint *params) {
  _glr->glExt.glGetTexParameterIuiv (target, pname, params);}
inline GLAPI void APIENTRY glClearBufferiv (GLenum buffer, GLint drawbuffer, const GLint *value) {
  _glr->glExt.glClearBufferiv (buffer, drawbuffer, value);}
inline GLAPI void APIENTRY glClearBufferuiv (GLenum buffer, GLint drawbuffer, const GLuint *value) {
  _glr->glExt.glClearBufferuiv (buffer, drawbuffer, value);}
inline GLAPI void APIENTRY glClearBufferfv (GLenum buffer, GLint drawbuffer, const GLfloat *value) {
  _glr->glExt.glClearBufferfv (buffer, drawbuffer, value);}
inline GLAPI void APIENTRY glClearBufferfi (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
  _glr->glExt.glClearBufferfi (buffer, drawbuffer, depth, stencil);}
inline GLAPI const GLubyte *APIENTRY glGetStringi (GLenum name, GLuint index) {
  return _glr->glExt.glGetStringi (name, index);}
inline GLAPI GLboolean APIENTRY glIsRenderbuffer (GLuint renderbuffer) {
  return _glr->glExt.glIsRenderbuffer (renderbuffer);}
inline GLAPI void APIENTRY glBindRenderbuffer (GLenum target, GLuint renderbuffer) {
  _glr->glExt.glBindRenderbuffer (target, renderbuffer);}
inline GLAPI void APIENTRY glDeleteRenderbuffers (GLsizei n, const GLuint *renderbuffers) {
  _glr->glExt.glDeleteRenderbuffers (n, renderbuffers);}
inline GLAPI void APIENTRY glGenRenderbuffers (GLsizei n, GLuint *renderbuffers) {
  _glr->glExt.glGenRenderbuffers (n, renderbuffers);}
inline GLAPI void APIENTRY glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glRenderbufferStorage (target, internalformat, width, height);}
inline GLAPI void APIENTRY glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetRenderbufferParameteriv (target, pname, params);}
inline GLAPI GLboolean APIENTRY glIsFramebuffer (GLuint framebuffer) {
  return _glr->glExt.glIsFramebuffer (framebuffer);}
inline GLAPI void APIENTRY glBindFramebuffer (GLenum target, GLuint framebuffer) {
  _glr->glExt.glBindFramebuffer (target, framebuffer);}
inline GLAPI void APIENTRY glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers) {
  _glr->glExt.glDeleteFramebuffers (n, framebuffers);}
inline GLAPI void APIENTRY glGenFramebuffers (GLsizei n, GLuint *framebuffers) {
  _glr->glExt.glGenFramebuffers (n, framebuffers);}
inline GLAPI GLenum APIENTRY glCheckFramebufferStatus (GLenum target) {
  return _glr->glExt.glCheckFramebufferStatus (target);}
inline GLAPI void APIENTRY glFramebufferTexture1D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  _glr->glExt.glFramebufferTexture1D (target, attachment, textarget,  texture,  level);}
inline GLAPI void APIENTRY glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  _glr->glExt.glFramebufferTexture2D (target, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glFramebufferTexture3D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
  _glr->glExt.glFramebufferTexture3D (target, attachment,  textarget, texture, level, zoffset);}
inline GLAPI void APIENTRY glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
  _glr->glExt.glFramebufferRenderbuffer (target, attachment, renderbuffertarget, renderbuffer);}
inline GLAPI void APIENTRY glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params) {
  _glr->glExt.glGetFramebufferAttachmentParameteriv (target, attachment, pname, params);}
inline GLAPI void APIENTRY glGenerateMipmap (GLenum target) {
  _glr->glExt.glGenerateMipmap (target);}
inline GLAPI void APIENTRY glBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
  _glr->glExt.glBlitFramebuffer (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);}
inline GLAPI void APIENTRY glRenderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glRenderbufferStorageMultisample (target, samples, internalformat, width, height);}
inline GLAPI void APIENTRY glFramebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  _glr->glExt.glFramebufferTextureLayer (target, attachment, texture, level, layer);}
inline GLAPI void *APIENTRY glMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
  return _glr->glExt.glMapBufferRange (target, offset, length, access);}
inline GLAPI void APIENTRY glFlushMappedBufferRange (GLenum target, GLintptr offset, GLsizeiptr length) {
  _glr->glExt.glFlushMappedBufferRange (target, offset, length);}
inline GLAPI void APIENTRY glBindVertexArray (GLuint array) {
  _glr->glExt.glBindVertexArray (array);}
inline GLAPI void APIENTRY glDeleteVertexArrays (GLsizei n, const GLuint *arrays) {
  _glr->glExt.glDeleteVertexArrays (n, arrays);}
inline GLAPI void APIENTRY glGenVertexArrays (GLsizei n, GLuint *arrays) {
  _glr->glExt.glGenVertexArrays (n, arrays);}
inline GLAPI GLboolean APIENTRY glIsVertexArray (GLuint array) {
  return _glr->glExt.glIsVertexArray (array);}

/// OpenGL 3.1 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount) {
  _glr->glExt.glDrawArraysInstanced (mode, first, count, instancecount);}
inline GLAPI void APIENTRY glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) {
  _glr->glExt.glDrawElementsInstanced (mode, count, type, indices, instancecount);}
inline GLAPI void APIENTRY glTexBuffer (GLenum target, GLenum internalformat, GLuint buffer) {
  _glr->glExt.glTexBuffer (target, internalformat, buffer);}
inline GLAPI void APIENTRY glPrimitiveRestartIndex (GLuint index) {
  _glr->glExt.glPrimitiveRestartIndex (index);}
inline GLAPI void APIENTRY glCopyBufferSubData (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
  _glr->glExt.glCopyBufferSubData (readTarget, writeTarget, readOffset, writeOffset, size);}
inline GLAPI void APIENTRY glGetUniformIndices (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices) {
  _glr->glExt.glGetUniformIndices (program, uniformCount, uniformNames, uniformIndices);}
inline GLAPI void APIENTRY glGetActiveUniformsiv (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) {
  _glr->glExt.glGetActiveUniformsiv (program, uniformCount, uniformIndices, pname, params);}
inline GLAPI void APIENTRY glGetActiveUniformName (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName) {
  _glr->glExt.glGetActiveUniformName (program, uniformIndex, bufSize, length, uniformName);}
inline GLAPI GLuint APIENTRY glGetUniformBlockIndex (GLuint program, const GLchar *uniformBlockName) {
  return _glr->glExt.glGetUniformBlockIndex (program, uniformBlockName);}
inline GLAPI void APIENTRY glGetActiveUniformBlockiv (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) {
  _glr->glExt.glGetActiveUniformBlockiv (program, uniformBlockIndex, pname, params);}
inline GLAPI void APIENTRY glGetActiveUniformBlockName (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) {
  _glr->glExt.glGetActiveUniformBlockName (program, uniformBlockIndex, bufSize, length, uniformBlockName);}
inline GLAPI void APIENTRY glUniformBlockBinding (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
  _glr->glExt.glUniformBlockBinding (program, uniformBlockIndex, uniformBlockBinding);}

/// OpenGL 3.2 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawElementsBaseVertex (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) {
  _glr->glExt.glDrawElementsBaseVertex (mode, count, type, indices, basevertex);}
inline GLAPI void APIENTRY glDrawRangeElementsBaseVertex (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex) {
  _glr->glExt.glDrawRangeElementsBaseVertex (mode, start, end, count, type, indices, basevertex);}
inline GLAPI void APIENTRY glDrawElementsInstancedBaseVertex (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex) {
  _glr->glExt.glDrawElementsInstancedBaseVertex (mode, count, type, indices, instancecount, basevertex);}
inline GLAPI void APIENTRY glMultiDrawElementsBaseVertex (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex) {
  _glr->glExt.glMultiDrawElementsBaseVertex (mode, count, type, indices, drawcount, basevertex);}
inline GLAPI void APIENTRY glProvokingVertex (GLenum mode) {
  _glr->glExt.glProvokingVertex (mode);}
inline GLAPI GLsync APIENTRY glFenceSync (GLenum condition, GLbitfield flags) {
  return _glr->glExt.glFenceSync (condition, flags);}
inline GLAPI GLboolean APIENTRY glIsSync (GLsync sync) {
  return _glr->glExt.glIsSync (sync);}
inline GLAPI void APIENTRY glDeleteSync (GLsync sync) {
  _glr->glExt.glDeleteSync (sync);}
inline GLAPI GLenum APIENTRY glClientWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout) {
  return _glr->glExt.glClientWaitSync (sync, flags, timeout);}
inline GLAPI void APIENTRY glWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout) {
  _glr->glExt.glWaitSync (sync, flags, timeout);}
inline GLAPI void APIENTRY glGetInteger64v (GLenum pname, GLint64 *data) {
  _glr->glExt.glGetInteger64v (pname, data);}
inline GLAPI void APIENTRY glGetSynciv (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) {
  _glr->glExt.glGetSynciv (sync, pname, bufSize, length, values);}
inline GLAPI void APIENTRY glGetInteger64i_v (GLenum target, GLuint index, GLint64 *data) {
  _glr->glExt.glGetInteger64i_v (target, index, data);}
inline GLAPI void APIENTRY glGetBufferParameteri64v (GLenum target, GLenum pname, GLint64 *params) {
  _glr->glExt.glGetBufferParameteri64v (target, pname, params);}
inline GLAPI void APIENTRY glFramebufferTexture (GLenum target, GLenum attachment, GLuint texture, GLint level) {
  _glr->glExt.glFramebufferTexture (target, attachment, texture, level);}
inline GLAPI void APIENTRY glTexImage2DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
  _glr->glExt.glTexImage2DMultisample (target, samples, internalformat, width, height, fixedsamplelocations);}
inline GLAPI void APIENTRY glTexImage3DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
  _glr->glExt.glTexImage3DMultisample (target, samples, internalformat, width, height, depth, fixedsamplelocations);}
inline GLAPI void APIENTRY glGetMultisamplefv (GLenum pname, GLuint index, GLfloat *val) {
  _glr->glExt.glGetMultisamplefv (pname, index, val);}
inline GLAPI void APIENTRY glSampleMaski (GLuint maskNumber, GLbitfield mask) {
  _glr->glExt.glSampleMaski (maskNumber, mask);}

/// OpenGL 3.3 funcs =================------------------------------
inline GLAPI void APIENTRY glBindFragDataLocationIndexed (GLuint program, GLuint colorNumber, GLuint index, const GLchar *name) {
  _glr->glExt.glBindFragDataLocationIndexed (program, colorNumber, index, name);}
inline GLAPI GLint APIENTRY glGetFragDataIndex (GLuint program, const GLchar *name) {
  return _glr->glExt.glGetFragDataIndex (program, name);}
inline GLAPI void APIENTRY glGenSamplers (GLsizei count, GLuint *samplers) {
  _glr->glExt.glGenSamplers (count, samplers);}
inline GLAPI void APIENTRY glDeleteSamplers (GLsizei count, const GLuint *samplers) {
  _glr->glExt.glDeleteSamplers (count, samplers);}
inline GLAPI GLboolean APIENTRY glIsSampler (GLuint sampler) {
  return _glr->glExt.glIsSampler (sampler);}
inline GLAPI void APIENTRY glBindSampler (GLuint unit, GLuint sampler) {
  _glr->glExt.glBindSampler (unit, sampler);}
inline GLAPI void APIENTRY glSamplerParameteri (GLuint sampler, GLenum pname, GLint param) {
  _glr->glExt.glSamplerParameteri (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameteriv (GLuint sampler, GLenum pname, const GLint *param) {
  _glr->glExt.glSamplerParameteriv (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterf (GLuint sampler, GLenum pname, GLfloat param) {
  _glr->glExt.glSamplerParameterf (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterfv (GLuint sampler, GLenum pname, const GLfloat *param) {
  _glr->glExt.glSamplerParameterfv (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterIiv (GLuint sampler, GLenum pname, const GLint *param) {
  _glr->glExt.glSamplerParameterIiv (sampler, pname, param);}
inline GLAPI void APIENTRY glSamplerParameterIuiv (GLuint sampler, GLenum pname, const GLuint *param) {
  _glr->glExt.glSamplerParameterIuiv (sampler, pname, param);}
inline GLAPI void APIENTRY glGetSamplerParameteriv (GLuint sampler, GLenum pname, GLint *params) {
  _glr->glExt.glGetSamplerParameteriv (sampler, pname, params);}
inline GLAPI void APIENTRY glGetSamplerParameterIiv (GLuint sampler, GLenum pname, GLint *params) {
  _glr->glExt.glGetSamplerParameterIiv (sampler, pname, params);}
inline GLAPI void APIENTRY glGetSamplerParameterfv (GLuint sampler, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetSamplerParameterfv (sampler, pname, params);}
inline GLAPI void APIENTRY glGetSamplerParameterIuiv (GLuint sampler, GLenum pname, GLuint *params) {
  _glr->glExt.glGetSamplerParameterIuiv (sampler, pname, params);}
inline GLAPI void APIENTRY glQueryCounter (GLuint id, GLenum target) {
  _glr->glExt.glQueryCounter (id, target);}
inline GLAPI void APIENTRY glGetQueryObjecti64v (GLuint id, GLenum pname, GLint64 *params) {
  _glr->glExt.glGetQueryObjecti64v (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectui64v (GLuint id, GLenum pname, GLuint64 *params) {
  _glr->glExt.glGetQueryObjectui64v (id, pname, params);}
inline GLAPI void APIENTRY glVertexAttribDivisor (GLuint index, GLuint divisor) {
  _glr->glExt.glVertexAttribDivisor (index, divisor);}
inline GLAPI void APIENTRY glVertexAttribP1ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  _glr->glExt.glVertexAttribP1ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP1uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  _glr->glExt.glVertexAttribP1uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP2ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  _glr->glExt.glVertexAttribP2ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP2uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  _glr->glExt.glVertexAttribP2uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP3ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  _glr->glExt.glVertexAttribP3ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP3uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  _glr->glExt.glVertexAttribP3uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP4ui (GLuint index, GLenum type, GLboolean normalized, GLuint value) {
  _glr->glExt.glVertexAttribP4ui (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexAttribP4uiv (GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
  _glr->glExt.glVertexAttribP4uiv (index, type, normalized, value);}
inline GLAPI void APIENTRY glVertexP2ui (GLenum type, GLuint value) {
  _glr->glExt.glVertexP2ui (type, value);}
inline GLAPI void APIENTRY glVertexP2uiv (GLenum type, const GLuint *value) {
  _glr->glExt.glVertexP2uiv (type, value);}
inline GLAPI void APIENTRY glVertexP3ui (GLenum type, GLuint value) {
  _glr->glExt.glVertexP3ui (type, value);}
inline GLAPI void APIENTRY glVertexP3uiv (GLenum type, const GLuint *value) {
  _glr->glExt.glVertexP3uiv (type, value);}
inline GLAPI void APIENTRY glVertexP4ui (GLenum type, GLuint value) {
  _glr->glExt.glVertexP4ui (type, value);}
inline GLAPI void APIENTRY glVertexP4uiv (GLenum type, const GLuint *value) {
  _glr->glExt.glVertexP4uiv (type, value);}
inline GLAPI void APIENTRY glTexCoordP1ui (GLenum type, GLuint coords) {
  _glr->glExt.glTexCoordP1ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP1uiv (GLenum type, const GLuint *coords) {
  _glr->glExt.glTexCoordP1uiv (type, coords);}
inline GLAPI void APIENTRY glTexCoordP2ui (GLenum type, GLuint coords) {
  _glr->glExt.glTexCoordP2ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP2uiv (GLenum type, const GLuint *coords) {
  _glr->glExt.glTexCoordP2uiv (type, coords);}
inline GLAPI void APIENTRY glTexCoordP3ui (GLenum type, GLuint coords) {
  _glr->glExt.glTexCoordP3ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP3uiv (GLenum type, const GLuint *coords) {
  _glr->glExt.glTexCoordP3uiv (type, coords);}
inline GLAPI void APIENTRY glTexCoordP4ui (GLenum type, GLuint coords) {
  _glr->glExt.glTexCoordP4ui (type, coords);}
inline GLAPI void APIENTRY glTexCoordP4uiv (GLenum type, const GLuint *coords) {
  _glr->glExt.glTexCoordP4uiv (type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP1ui (GLenum texture, GLenum type, GLuint coords) {
  _glr->glExt.glMultiTexCoordP1ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP1uiv (GLenum texture, GLenum type, const GLuint *coords) {
  _glr->glExt.glMultiTexCoordP1uiv (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP2ui (GLenum texture, GLenum type, GLuint coords) {
  _glr->glExt.glMultiTexCoordP2ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP2uiv (GLenum texture, GLenum type, const GLuint *coords) {
  _glr->glExt.glMultiTexCoordP2uiv (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP3ui (GLenum texture, GLenum type, GLuint coords) {
  _glr->glExt.glMultiTexCoordP3ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP3uiv (GLenum texture, GLenum type, const GLuint *coords) {
  _glr->glExt.glMultiTexCoordP3uiv (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP4ui (GLenum texture, GLenum type, GLuint coords) {
  _glr->glExt.glMultiTexCoordP4ui (texture, type, coords);}
inline GLAPI void APIENTRY glMultiTexCoordP4uiv (GLenum texture, GLenum type, const GLuint *coords) {
  _glr->glExt.glMultiTexCoordP4uiv (texture, type, coords);}
inline GLAPI void APIENTRY glNormalP3ui (GLenum type, GLuint coords) {
  _glr->glExt.glNormalP3ui (type, coords);}
inline GLAPI void APIENTRY glNormalP3uiv (GLenum type, const GLuint *coords) {
  _glr->glExt.glNormalP3uiv (type, coords);}
inline GLAPI void APIENTRY glColorP3ui (GLenum type, GLuint color) {
  _glr->glExt.glColorP3ui (type, color);}
inline GLAPI void APIENTRY glColorP3uiv (GLenum type, const GLuint *color) {
  _glr->glExt.glColorP3uiv (type, color);}
inline GLAPI void APIENTRY glColorP4ui (GLenum type, GLuint color) {
  _glr->glExt.glColorP4ui (type, color);}
inline GLAPI void APIENTRY glColorP4uiv (GLenum type, const GLuint *color) {
  _glr->glExt.glColorP4uiv (type, color);}
inline GLAPI void APIENTRY glSecondaryColorP3ui (GLenum type, GLuint color) {
  _glr->glExt.glSecondaryColorP3ui (type,  color);}
inline GLAPI void APIENTRY glSecondaryColorP3uiv (GLenum type, const GLuint *color) {
  _glr->glExt.glSecondaryColorP3uiv (type, color);}

/// OpenGL 4.0 funcs =================------------------------------
inline GLAPI void APIENTRY glMinSampleShading (GLfloat value) {
  _glr->glExt.glMinSampleShading (value);}
inline GLAPI void APIENTRY glBlendEquationi (GLuint buf, GLenum mode) {
  _glr->glExt.glBlendEquationi (buf, mode);}
inline GLAPI void APIENTRY glBlendEquationSeparatei (GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
  _glr->glExt.glBlendEquationSeparatei (buf, modeRGB, modeAlpha);}
inline GLAPI void APIENTRY glBlendFunci (GLuint buf, GLenum src, GLenum dst) {
  _glr->glExt.glBlendFunci (buf, src, dst);}
inline GLAPI void APIENTRY glBlendFuncSeparatei (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  _glr->glExt.glBlendFuncSeparatei (buf, srcRGB, dstRGB, srcAlpha, dstAlpha);}
inline GLAPI void APIENTRY glDrawArraysIndirect (GLenum mode, const void *indirect) {
  _glr->glExt.glDrawArraysIndirect (mode, indirect);}
inline GLAPI void APIENTRY glDrawElementsIndirect (GLenum mode, GLenum type, const void *indirect) {
  _glr->glExt.glDrawElementsIndirect (mode, type, indirect);}
inline GLAPI void APIENTRY glUniform1d (GLint location, GLdouble x) {
  _glr->glExt.glUniform1d (location, x);}
inline GLAPI void APIENTRY glUniform2d (GLint location, GLdouble x, GLdouble y) {
  _glr->glExt.glUniform2d (location, x, y);}
inline GLAPI void APIENTRY glUniform3d (GLint location, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glUniform3d (location, x, y, z);}
inline GLAPI void APIENTRY glUniform4d (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glUniform4d (location, x, y, z, w);}
inline GLAPI void APIENTRY glUniform1dv (GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glUniform1dv (location, count, value);}
inline GLAPI void APIENTRY glUniform2dv (GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glUniform2dv (location, count, value);}
inline GLAPI void APIENTRY glUniform3dv (GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glUniform3dv (location, count, value);}
inline GLAPI void APIENTRY glUniform4dv (GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glUniform4dv (location, count, value);}
inline GLAPI void APIENTRY glUniformMatrix2dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix2dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix3dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix4dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix2x3dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix2x3dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix2x4dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix2x4dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x2dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix3x2dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3x4dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix3x4dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x2dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix4x2dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4x3dv (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glUniformMatrix4x3dv (location, count, transpose, value);}
inline GLAPI void APIENTRY glGetUniformdv (GLuint program, GLint location, GLdouble *params) {
  _glr->glExt.glGetUniformdv (program, location, params);}
inline GLAPI GLint APIENTRY glGetSubroutineUniformLocation (GLuint program, GLenum shadertype, const GLchar *name) {
  return _glr->glExt.glGetSubroutineUniformLocation (program, shadertype, name);}
inline GLAPI GLuint APIENTRY glGetSubroutineIndex (GLuint program, GLenum shadertype, const GLchar *name) {
  return _glr->glExt.glGetSubroutineIndex (program, shadertype, name);}
inline GLAPI void APIENTRY glGetActiveSubroutineUniformiv (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values) {
  _glr->glExt.glGetActiveSubroutineUniformiv (program, shadertype, index, pname, values);}
inline GLAPI void APIENTRY glGetActiveSubroutineUniformName (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name) {
  _glr->glExt.glGetActiveSubroutineUniformName (program, shadertype, index, bufsize, length, name);}
inline GLAPI void APIENTRY glGetActiveSubroutineName (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name) {
  _glr->glExt.glGetActiveSubroutineName (program, shadertype, index, bufsize, length, name);}
inline GLAPI void APIENTRY glUniformSubroutinesuiv (GLenum shadertype, GLsizei count, const GLuint *indices) {
  _glr->glExt.glUniformSubroutinesuiv (shadertype, count, indices);}
inline GLAPI void APIENTRY glGetUniformSubroutineuiv (GLenum shadertype, GLint location, GLuint *params) {
  _glr->glExt.glGetUniformSubroutineuiv (shadertype, location, params);}
inline GLAPI void APIENTRY glGetProgramStageiv (GLuint program, GLenum shadertype, GLenum pname, GLint *values) {
  _glr->glExt.glGetProgramStageiv (program, shadertype, pname, values);}
inline GLAPI void APIENTRY glPatchParameteri (GLenum pname, GLint value) {
  _glr->glExt.glPatchParameteri (pname, value);}
inline GLAPI void APIENTRY glPatchParameterfv (GLenum pname, const GLfloat *values) {
  _glr->glExt.glPatchParameterfv (pname, values);}
inline GLAPI void APIENTRY glBindTransformFeedback (GLenum target, GLuint id) {
  _glr->glExt.glBindTransformFeedback (target, id);}
inline GLAPI void APIENTRY glDeleteTransformFeedbacks (GLsizei n, const GLuint *ids) {
  _glr->glExt.glDeleteTransformFeedbacks (n, ids);}
inline GLAPI void APIENTRY glGenTransformFeedbacks (GLsizei n, GLuint *ids) {
  _glr->glExt.glGenTransformFeedbacks (n, ids);}
inline GLAPI GLboolean APIENTRY glIsTransformFeedback (GLuint id) {
  return _glr->glExt.glIsTransformFeedback (id);}
inline GLAPI void APIENTRY glPauseTransformFeedback (void) {
  _glr->glExt.glPauseTransformFeedback ();}
inline GLAPI void APIENTRY glResumeTransformFeedback (void) {
  _glr->glExt.glResumeTransformFeedback ();}
inline GLAPI void APIENTRY glDrawTransformFeedback (GLenum mode, GLuint id) {
  _glr->glExt.glDrawTransformFeedback (mode, id);}
inline GLAPI void APIENTRY glDrawTransformFeedbackStream (GLenum mode, GLuint id, GLuint stream) {
  _glr->glExt.glDrawTransformFeedbackStream (mode, id, stream);}
inline GLAPI void APIENTRY glBeginQueryIndexed (GLenum target, GLuint index, GLuint id) {
  _glr->glExt.glBeginQueryIndexed (target, index, id);}
inline GLAPI void APIENTRY glEndQueryIndexed (GLenum target, GLuint index) {
  _glr->glExt.glEndQueryIndexed (target, index);}
inline GLAPI void APIENTRY glGetQueryIndexediv (GLenum target, GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetQueryIndexediv (target, index, pname, params);}

/// OpenGL 4.1 funcs =================------------------------------
inline GLAPI void APIENTRY glReleaseShaderCompiler (void) {
  _glr->glExt.glReleaseShaderCompiler ();}
inline GLAPI void APIENTRY glShaderBinary (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length) {
  _glr->glExt.glShaderBinary (count, shaders, binaryformat, binary, length);}
inline GLAPI void APIENTRY glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) {
  _glr->glExt.glGetShaderPrecisionFormat (shadertype, precisiontype, range, precision);}
inline GLAPI void APIENTRY glDepthRangef (GLfloat n, GLfloat f) {
  _glr->glExt.glDepthRangef (n, f);}
inline GLAPI void APIENTRY glClearDepthf (GLfloat d) {
  _glr->glExt.glClearDepthf (d);}
inline GLAPI void APIENTRY glGetProgramBinary (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) {
  _glr->glExt.glGetProgramBinary (program, bufSize, length, binaryFormat, binary);}
inline GLAPI void APIENTRY glProgramBinary (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length) {
  _glr->glExt.glProgramBinary (program, binaryFormat, binary, length);}
inline GLAPI void APIENTRY glProgramParameteri (GLuint program, GLenum pname, GLint value) {
  _glr->glExt.glProgramParameteri (program, pname, value);}
inline GLAPI void APIENTRY glUseProgramStages (GLuint pipeline, GLbitfield stages, GLuint program) {
  _glr->glExt.glUseProgramStages (pipeline, stages, program);}
inline GLAPI void APIENTRY glActiveShaderProgram (GLuint pipeline, GLuint program) {
  _glr->glExt.glActiveShaderProgram (pipeline, program);}
inline GLAPI GLuint APIENTRY glCreateShaderProgramv (GLenum type, GLsizei count, const GLchar *const*strings) {
  return _glr->glExt.glCreateShaderProgramv (type, count, strings);}
inline GLAPI void APIENTRY glBindProgramPipeline (GLuint pipeline) {
  _glr->glExt.glBindProgramPipeline (pipeline);}
inline GLAPI void APIENTRY glDeleteProgramPipelines (GLsizei n, const GLuint *pipelines) {
  _glr->glExt.glDeleteProgramPipelines (n, pipelines);}
inline GLAPI void APIENTRY glGenProgramPipelines (GLsizei n, GLuint *pipelines) {
  _glr->glExt.glGenProgramPipelines (n, pipelines);}
inline GLAPI GLboolean APIENTRY glIsProgramPipeline (GLuint pipeline) {
  return _glr->glExt.glIsProgramPipeline (pipeline);}
inline GLAPI void APIENTRY glGetProgramPipelineiv (GLuint pipeline, GLenum pname, GLint *params) {
  _glr->glExt.glGetProgramPipelineiv (pipeline, pname, params);}
inline GLAPI void APIENTRY glProgramUniform1i (GLuint program, GLint location, GLint v0) {
  _glr->glExt.glProgramUniform1i (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform1iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform1iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1f (GLuint program, GLint location, GLfloat v0) {
  _glr->glExt.glProgramUniform1f (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform1fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform1fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1d (GLuint program, GLint location, GLdouble v0) {
  _glr->glExt.glProgramUniform1d (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform1dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform1dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1ui (GLuint program, GLint location, GLuint v0) {
  _glr->glExt.glProgramUniform1ui (program, location,  v0);}
inline GLAPI void APIENTRY glProgramUniform1uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform1uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2i (GLuint program, GLint location, GLint v0, GLint v1) {
  _glr->glExt.glProgramUniform2i (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform2iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2f (GLuint program, GLint location, GLfloat v0, GLfloat v1) {
  _glr->glExt.glProgramUniform2f (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform2fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2d (GLuint program, GLint location, GLdouble v0, GLdouble v1) {
  _glr->glExt.glProgramUniform2d (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform2dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2ui (GLuint program, GLint location, GLuint v0, GLuint v1) {
  _glr->glExt.glProgramUniform2ui (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform2uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform2uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3i (GLuint program, GLint location, GLint v0, GLint v1, GLint v2) {
  _glr->glExt.glProgramUniform3i (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform3iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  _glr->glExt.glProgramUniform3f (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform3fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3d (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2) {
  _glr->glExt.glProgramUniform3d (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform3dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3ui (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) {
  _glr->glExt.glProgramUniform3ui (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform3uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform3uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4i (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  _glr->glExt.glProgramUniform4i (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4iv (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform4iv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  _glr->glExt.glProgramUniform4f (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4fv (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform4fv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4d (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) {
  _glr->glExt.glProgramUniform4d (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4dv (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform4dv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4ui (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  _glr->glExt.glProgramUniform4ui (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform4uiv (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform4uiv (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix2fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix3fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix4fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix2dv (program,  location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix3dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix4dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix2x3fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix3x2fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix2x4fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix4x2fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix3x4fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix4x3fv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix2x3dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix3x2dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix2x4dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix4x2dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix3x4dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3dv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix4x3dv (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glValidateProgramPipeline (GLuint pipeline) {
  _glr->glExt.glValidateProgramPipeline (pipeline);}
inline GLAPI void APIENTRY glGetProgramPipelineInfoLog (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
  _glr->glExt.glGetProgramPipelineInfoLog (pipeline, bufSize, length, infoLog);}
inline GLAPI void APIENTRY glVertexAttribL1d (GLuint index, GLdouble x) {
  _glr->glExt.glVertexAttribL1d (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2d (GLuint index, GLdouble x, GLdouble y) {
  _glr->glExt.glVertexAttribL2d (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3d (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glVertexAttribL3d (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4d (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glVertexAttribL4d (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL1dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL2dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL3dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4dv (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL4dv (index, v);}
inline GLAPI void APIENTRY glVertexAttribLPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexAttribLPointer (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribLdv (GLuint index, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetVertexAttribLdv (index, pname, params);}
inline GLAPI void APIENTRY glViewportArrayv (GLuint first, GLsizei count, const GLfloat *v) {
  _glr->glExt.glViewportArrayv (first, count, v);}
inline GLAPI void APIENTRY glViewportIndexedf (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
  _glr->glExt.glViewportIndexedf (index, x, y, w, h);}
inline GLAPI void APIENTRY glViewportIndexedfv (GLuint index, const GLfloat *v) {
  _glr->glExt.glViewportIndexedfv (index, v);}
inline GLAPI void APIENTRY glScissorArrayv (GLuint first, GLsizei count, const GLint *v) {
  _glr->glExt.glScissorArrayv (first, count, v);}
inline GLAPI void APIENTRY glScissorIndexed (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height) {
  _glr->glExt.glScissorIndexed (index, left, bottom,  width, height);}
inline GLAPI void APIENTRY glScissorIndexedv (GLuint index, const GLint *v) {
  _glr->glExt.glScissorIndexedv (index, v);}
inline GLAPI void APIENTRY glDepthRangeArrayv (GLuint first, GLsizei count, const GLdouble *v) {
  _glr->glExt.glDepthRangeArrayv (first, count, v);}
inline GLAPI void APIENTRY glDepthRangeIndexed (GLuint index, GLdouble n, GLdouble f) {
  _glr->glExt.glDepthRangeIndexed (index, n, f);}
inline GLAPI void APIENTRY glGetFloati_v (GLenum target, GLuint index, GLfloat *data) {
  _glr->glExt.glGetFloati_v (target, index, data);}
inline GLAPI void APIENTRY glGetDoublei_v (GLenum target, GLuint index, GLdouble *data) {
  _glr->glExt.glGetDoublei_v (target, index, data);}

/// OpenGL 4.2 funcs =================------------------------------
inline GLAPI void APIENTRY glDrawArraysInstancedBaseInstance (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance) {
  _glr->glExt.glDrawArraysInstancedBaseInstance (mode, first, count, instancecount, baseinstance);}
inline GLAPI void APIENTRY glDrawElementsInstancedBaseInstance (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance) {
  _glr->glExt.glDrawElementsInstancedBaseInstance (mode, count, type, indices,  instancecount, baseinstance);}
inline GLAPI void APIENTRY glDrawElementsInstancedBaseVertexBaseInstance (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance) {
  _glr->glExt.glDrawElementsInstancedBaseVertexBaseInstance (mode, count, type, indices, instancecount, basevertex, baseinstance);}
inline GLAPI void APIENTRY glGetInternalformativ (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params) {
  _glr->glExt.glGetInternalformativ (target, internalformat, pname, bufSize, params);}
inline GLAPI void APIENTRY glGetActiveAtomicCounterBufferiv (GLuint program, GLuint bufferIndex, GLenum pname, GLint *params) {
  _glr->glExt.glGetActiveAtomicCounterBufferiv (program, bufferIndex, pname, params);}
inline GLAPI void APIENTRY glBindImageTexture (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) {
  _glr->glExt.glBindImageTexture (unit, texture, level, layered, layer, access, format);}
inline GLAPI void APIENTRY glMemoryBarrier (GLbitfield barriers) {
  _glr->glExt.glMemoryBarrier (barriers);}
inline GLAPI void APIENTRY glTexStorage1D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) {
  _glr->glExt.glTexStorage1D (target, levels, internalformat, width);}
inline GLAPI void APIENTRY glTexStorage2D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glTexStorage2D (target, levels, internalformat, width, height);}
inline GLAPI void APIENTRY glTexStorage3D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
  _glr->glExt.glTexStorage3D (target, levels, internalformat, width, height, depth);}
inline GLAPI void APIENTRY glDrawTransformFeedbackInstanced (GLenum mode, GLuint id, GLsizei instancecount) {
  _glr->glExt.glDrawTransformFeedbackInstanced (mode, id, instancecount);}
inline GLAPI void APIENTRY glDrawTransformFeedbackStreamInstanced (GLenum mode, GLuint id, GLuint stream, GLsizei instancecount) {
  _glr->glExt.glDrawTransformFeedbackStreamInstanced (mode, id, stream, instancecount);}

/// OpenGL 4.3 funcs =================------------------------------
inline GLAPI void APIENTRY glClearBufferData (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glClearBufferData (target, internalformat, format, type, data);}
inline GLAPI void APIENTRY glClearBufferSubData (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glClearBufferSubData (target, internalformat, offset, size, format, type, data);}
inline GLAPI void APIENTRY glDispatchCompute (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) {
  _glr->glExt.glDispatchCompute (num_groups_x, num_groups_y, num_groups_z);}
inline GLAPI void APIENTRY glDispatchComputeIndirect (GLintptr indirect) {
  _glr->glExt.glDispatchComputeIndirect (indirect);}
inline GLAPI void APIENTRY glCopyImageSubData (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) {
  _glr->glExt.glCopyImageSubData (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);}
inline GLAPI void APIENTRY glFramebufferParameteri (GLenum target, GLenum pname, GLint param) {
  _glr->glExt.glFramebufferParameteri (target, pname, param);}
inline GLAPI void APIENTRY glGetFramebufferParameteriv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetFramebufferParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetInternalformati64v (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params) {
  _glr->glExt.glGetInternalformati64v (target, internalformat, pname, bufSize, params);}
inline GLAPI void APIENTRY glInvalidateTexSubImage (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth) {
  _glr->glExt.glInvalidateTexSubImage (texture, level, xoffset, yoffset,  zoffset,  width, height, depth);}
inline GLAPI void APIENTRY glInvalidateTexImage (GLuint texture, GLint level) {
  _glr->glExt.glInvalidateTexImage (texture, level);}
inline GLAPI void APIENTRY glInvalidateBufferSubData (GLuint buffer, GLintptr offset, GLsizeiptr length) {
  _glr->glExt.glInvalidateBufferSubData (buffer, offset, length);}
inline GLAPI void APIENTRY glInvalidateBufferData (GLuint buffer) {
  _glr->glExt.glInvalidateBufferData (buffer);}
inline GLAPI void APIENTRY glInvalidateFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments) {
  _glr->glExt.glInvalidateFramebuffer (target, numAttachments, attachments);}
inline GLAPI void APIENTRY glInvalidateSubFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glInvalidateSubFramebuffer (target, numAttachments, attachments, x, y, width, height);}
inline GLAPI void APIENTRY glMultiDrawArraysIndirect (GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride) {
  _glr->glExt.glMultiDrawArraysIndirect (mode, indirect, drawcount, stride);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirect (GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride) {
  _glr->glExt.glMultiDrawElementsIndirect (mode, type, indirect, drawcount, stride);}
inline GLAPI void APIENTRY glGetProgramInterfaceiv (GLuint program, GLenum programInterface, GLenum pname, GLint *params) {
  _glr->glExt.glGetProgramInterfaceiv (program, programInterface, pname, params);}
inline GLAPI GLuint APIENTRY glGetProgramResourceIndex (GLuint program, GLenum programInterface, const GLchar *name) {
  return _glr->glExt.glGetProgramResourceIndex (program, programInterface, name);}
inline GLAPI void APIENTRY glGetProgramResourceName (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name) {
  _glr->glExt.glGetProgramResourceName (program, programInterface, index, bufSize, length, name);}
inline GLAPI void APIENTRY glGetProgramResourceiv (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params) {
  _glr->glExt.glGetProgramResourceiv (program, programInterface, index, propCount, props, bufSize, length, params);}
inline GLAPI GLint APIENTRY glGetProgramResourceLocation (GLuint program, GLenum programInterface, const GLchar *name) {
  return _glr->glExt.glGetProgramResourceLocation (program, programInterface, name);}
inline GLAPI GLint APIENTRY glGetProgramResourceLocationIndex (GLuint program, GLenum programInterface, const GLchar *name) {
  return _glr->glExt.glGetProgramResourceLocationIndex (program, programInterface, name);}
inline GLAPI void APIENTRY glShaderStorageBlockBinding (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding) {
  _glr->glExt.glShaderStorageBlockBinding (program, storageBlockIndex, storageBlockBinding);}
inline GLAPI void APIENTRY glTexBufferRange (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  _glr->glExt.glTexBufferRange (target, internalformat, buffer, offset, size);}
inline GLAPI void APIENTRY glTexStorage2DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
  _glr->glExt.glTexStorage2DMultisample (target, samples, internalformat, width, height, fixedsamplelocations);}
inline GLAPI void APIENTRY glTexStorage3DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
  _glr->glExt.glTexStorage3DMultisample (target, samples, internalformat, width, height, depth, fixedsamplelocations);}
inline GLAPI void APIENTRY glTextureView (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers) {
  _glr->glExt.glTextureView (texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);}
inline GLAPI void APIENTRY glBindVertexBuffer (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
  _glr->glExt.glBindVertexBuffer (bindingindex, buffer, offset, stride);}
inline GLAPI void APIENTRY glVertexAttribFormat (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
  _glr->glExt.glVertexAttribFormat (attribindex, size, type, normalized, relativeoffset);}
inline GLAPI void APIENTRY glVertexAttribIFormat (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  _glr->glExt.glVertexAttribIFormat (attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexAttribLFormat (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  _glr->glExt.glVertexAttribLFormat (attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexAttribBinding (GLuint attribindex, GLuint bindingindex) {
  _glr->glExt.glVertexAttribBinding (attribindex, bindingindex);}
inline GLAPI void APIENTRY glVertexBindingDivisor (GLuint bindingindex, GLuint divisor) {
  _glr->glExt.glVertexBindingDivisor (bindingindex, divisor);}
inline GLAPI void APIENTRY glDebugMessageControl (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) {
  _glr->glExt.glDebugMessageControl (source, type, severity, count, ids, enabled);}
inline GLAPI void APIENTRY glDebugMessageInsert (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf) {
  _glr->glExt.glDebugMessageInsert (source, type, id, severity, length, buf);}
inline GLAPI void APIENTRY glDebugMessageCallback (GLDEBUGPROC callback, const void *userParam) {
  _glr->glExt.glDebugMessageCallback (callback, userParam);}
inline GLAPI GLuint APIENTRY glGetDebugMessageLog (GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog) {
  return _glr->glExt.glGetDebugMessageLog (count, bufSize, sources, types, ids, severities, lengths, messageLog);}
inline GLAPI void APIENTRY glPushDebugGroup (GLenum source, GLuint id, GLsizei length, const GLchar *message) {
  _glr->glExt.glPushDebugGroup (source, id, length, message);}
inline GLAPI void APIENTRY glPopDebugGroup (void) {
  _glr->glExt.glPopDebugGroup ();}
inline GLAPI void APIENTRY glObjectLabel (GLenum identifier, GLuint name, GLsizei length, const GLchar *label) {
  _glr->glExt.glObjectLabel (identifier, name, length, label);}
inline GLAPI void APIENTRY glGetObjectLabel (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label) {
  _glr->glExt.glGetObjectLabel (identifier, name, bufSize, length, label);}
inline GLAPI void APIENTRY glObjectPtrLabel (const void *ptr, GLsizei length, const GLchar *label) {
  _glr->glExt.glObjectPtrLabel (ptr, length, label);}
inline GLAPI void APIENTRY glGetObjectPtrLabel (const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label) {
  _glr->glExt.glGetObjectPtrLabel (ptr, bufSize, length, label);}

/// OpenGL 4.4 funcs =================------------------------------
inline GLAPI void APIENTRY glBufferStorage (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags) {
  _glr->glExt.glBufferStorage (target, size, data, flags);}
inline GLAPI void APIENTRY glClearTexImage (GLuint texture, GLint level, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glClearTexImage (texture, level, format, type, data);}
inline GLAPI void APIENTRY glClearTexSubImage (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glClearTexSubImage (texture, level, xoffset, yoffset, zoffset, width, height, depth, format,  type, data);}
inline GLAPI void APIENTRY glBindBuffersBase (GLenum target, GLuint first, GLsizei count, const GLuint *buffers) {
  _glr->glExt.glBindBuffersBase (target, first, count, buffers);}
inline GLAPI void APIENTRY glBindBuffersRange (GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes) {
  _glr->glExt.glBindBuffersRange (target, first, count, buffers, offsets, sizes);}
inline GLAPI void APIENTRY glBindTextures (GLuint first, GLsizei count, const GLuint *textures) {
  _glr->glExt.glBindTextures (first, count, textures);}
inline GLAPI void APIENTRY glBindSamplers (GLuint first, GLsizei count, const GLuint *samplers) {
  _glr->glExt.glBindSamplers (first, count, samplers);}
inline GLAPI void APIENTRY glBindImageTextures (GLuint first, GLsizei count, const GLuint *textures) {
  _glr->glExt.glBindImageTextures (first, count, textures);}
inline GLAPI void APIENTRY glBindVertexBuffers (GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides) {
  _glr->glExt.glBindVertexBuffers (first, count, buffers, offsets, strides);}









///==============///
// ARB extensions //
///==============///

// GL_ARB_bindless_texture core 4.4, it is in glext.h, not in OpenGL registry pages tho...
inline GLAPI GLuint64 APIENTRY glGetTextureHandleARB (GLuint texture) {
  return _glr->glExt.glGetTextureHandleARB (texture);}
inline GLAPI GLuint64 APIENTRY glGetTextureSamplerHandleARB (GLuint texture, GLuint sampler) {
  return _glr->glExt.glGetTextureSamplerHandleARB (texture, sampler);}
inline GLAPI void APIENTRY glMakeTextureHandleResidentARB (GLuint64 handle) {
  _glr->glExt.glMakeTextureHandleResidentARB (handle);}
inline GLAPI void APIENTRY glMakeTextureHandleNonResidentARB (GLuint64 handle) {
  _glr->glExt.glMakeTextureHandleNonResidentARB (handle);}
inline GLAPI GLuint64 APIENTRY glGetImageHandleARB (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format) {
  return _glr->glExt.glGetImageHandleARB (texture, level, layered, layer,  format);}
inline GLAPI void APIENTRY glMakeImageHandleResidentARB (GLuint64 handle, GLenum access) {
  _glr->glExt.glMakeImageHandleResidentARB (handle, access);}
inline GLAPI void APIENTRY glMakeImageHandleNonResidentARB (GLuint64 handle) {
  _glr->glExt.glMakeImageHandleNonResidentARB (handle);}
inline GLAPI void APIENTRY glUniformHandleui64ARB (GLint location, GLuint64 value) {
  _glr->glExt.glUniformHandleui64ARB (location, value);}
inline GLAPI void APIENTRY glUniformHandleui64vARB (GLint location, GLsizei count, const GLuint64 *value) {
  _glr->glExt.glUniformHandleui64vARB (location, count, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64ARB (GLuint program, GLint location, GLuint64 value) {
  _glr->glExt.glProgramUniformHandleui64ARB (program, location, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64vARB (GLuint program, GLint location, GLsizei count, const GLuint64 *values) {
  _glr->glExt.glProgramUniformHandleui64vARB (program, location, count, values);}
inline GLAPI GLboolean APIENTRY glIsTextureHandleResidentARB (GLuint64 handle) {
  return _glr->glExt.glIsTextureHandleResidentARB (handle);}
inline GLAPI GLboolean APIENTRY glIsImageHandleResidentARB (GLuint64 handle) {
  return _glr->glExt.glIsImageHandleResidentARB (handle);}
inline GLAPI void APIENTRY glVertexAttribL1ui64ARB (GLuint index, GLuint64EXT x) {
  _glr->glExt.glVertexAttribL1ui64ARB (index, x);}
inline GLAPI void APIENTRY glVertexAttribL1ui64vARB (GLuint index, const GLuint64EXT *v) {
  _glr->glExt.glVertexAttribL1ui64vARB (index, v);}
inline GLAPI void APIENTRY glGetVertexAttribLui64vARB (GLuint index, GLenum pname, GLuint64EXT *params) {
  _glr->glExt.glGetVertexAttribLui64vARB (index, pname, params);}
#ifdef OS_LINUX
// GLX_ARB_get_proc_address
// hope this func just works as it is...
//inline __GLXextFuncPtr glXGetProcAddressARB (const GLubyte *procName) {
  //return _glr->glExt.glXGetProcAddressARB (procName);}
#endif /// OS_LINUX
// GL_ARB_cl_event
inline GLAPI GLsync APIENTRY glCreateSyncFromCLeventARB (struct _cl_context *context, struct _cl_event *event, GLbitfield flags) {
  return _glr->glExt.glCreateSyncFromCLeventARB (context, event, flags);}
// GL_ARB_color_buffer_float
inline GLAPI void APIENTRY glClampColorARB (GLenum target, GLenum clamp) {
  _glr->glExt.glClampColorARB (target, clamp);}

#ifdef OS_WIN
//inline BOOL WINAPI wglGetPixelFormatAttribivARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues) {
//  return _glr->glExt.wglGetPixelFormatAttribivARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);}
//inline BOOL WINAPI wglGetPixelFormatAttribfvARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues) {
//  return _glr->glExt.wglGetPixelFormatAttribfvARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);}
//inline BOOL WINAPI wglChoosePixelFormatARB (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats) {
//  return _glr->glExt.wglChoosePixelFormatARB (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);}
#endif /// OS_WIN











// #153 GL_ARB_compute_variable_group_size
inline GLAPI void APIENTRY glDispatchComputeGroupSizeARB (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z) {
  _glr->glExt.glDispatchComputeGroupSizeARB (num_groups_x, num_groups_y, num_groups_z, group_size_x, group_size_y, group_size_z);}
// #104 GL_ARB_debug_output
inline GLAPI void APIENTRY glDebugMessageControlARB (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) {
  _glr->glExt.glDebugMessageControlARB (source, type, severity, count, ids, enabled);}
inline GLAPI void APIENTRY glDebugMessageInsertARB (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf) {
  _glr->glExt.glDebugMessageInsertARB (source, type, id, severity, length, buf);}
inline GLAPI void APIENTRY glDebugMessageCallbackARB (GLDEBUGPROCARB callback, const void *userParam) {
  _glr->glExt.glDebugMessageCallbackARB (callback, userParam);}
inline GLAPI GLuint APIENTRY glGetDebugMessageLogARB (GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog) {
  return _glr->glExt.glGetDebugMessageLogARB (count, bufSize, sources, types, ids, severities, lengths, messageLog);}
// #037 GL_ARB_draw_buffers
inline GLAPI void APIENTRY glDrawBuffersARB (GLsizei n, const GLenum *bufs) {
  _glr->glExt.glDrawBuffersARB (n, bufs);}
// #069 GL_ARB_draw_buffers_blend
inline GLAPI void APIENTRY glBlendEquationiARB (GLuint buf, GLenum mode) {
  _glr->glExt.glBlendEquationiARB (buf,  mode);}
inline GLAPI void APIENTRY glBlendEquationSeparateiARB (GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
  _glr->glExt.glBlendEquationSeparateiARB (buf, modeRGB, modeAlpha);}
inline GLAPI void APIENTRY glBlendFunciARB (GLuint buf, GLenum src, GLenum dst) {
  _glr->glExt.glBlendFunciARB (buf, src, dst);}
inline GLAPI void APIENTRY glBlendFuncSeparateiARB (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  _glr->glExt.glBlendFuncSeparateiARB (buf, srcRGB, dstRGB, srcAlpha, dstAlpha);}
// #044 GL_ARB_draw_instanced
inline GLAPI void APIENTRY glDrawArraysInstancedARB (GLenum mode, GLint first, GLsizei count, GLsizei primcount) {
  _glr->glExt.glDrawArraysInstancedARB (mode, first, count, primcount);}
inline GLAPI void APIENTRY glDrawElementsInstancedARB (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount) {
  _glr->glExt.glDrawElementsInstancedARB (mode, count, type, indices, primcount);}
// #027 GL_ARB_fragment_program
inline GLAPI void APIENTRY glProgramStringARB (GLenum target, GLenum format, GLsizei len, const void *string) {
  _glr->glExt.glProgramStringARB (target, format, len, string);}
inline GLAPI void APIENTRY glBindProgramARB (GLenum target, GLuint program) {
  _glr->glExt.glBindProgramARB (target, program);}
inline GLAPI void APIENTRY glDeleteProgramsARB (GLsizei n, const GLuint *programs) {
  _glr->glExt.glDeleteProgramsARB (n, programs);}
inline GLAPI void APIENTRY glGenProgramsARB (GLsizei n, GLuint *programs) {
  _glr->glExt.glGenProgramsARB (n, programs);}
inline GLAPI void APIENTRY glProgramEnvParameter4dARB (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glProgramEnvParameter4dARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameter4dvARB (GLenum target, GLuint index, const GLdouble *params) {
  _glr->glExt.glProgramEnvParameter4dvARB (target, index, params);}
inline GLAPI void APIENTRY glProgramEnvParameter4fARB (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glProgramEnvParameter4fARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameter4fvARB (GLenum target, GLuint index, const GLfloat *params) {
  _glr->glExt.glProgramEnvParameter4fvARB (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParameter4dARB (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glProgramLocalParameter4dARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameter4dvARB (GLenum target, GLuint index, const GLdouble *params) {
  _glr->glExt.glProgramLocalParameter4dvARB (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParameter4fARB (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glProgramLocalParameter4fARB (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameter4fvARB (GLenum target, GLuint index, const GLfloat *params) {
  _glr->glExt.glProgramLocalParameter4fvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterdvARB (GLenum target, GLuint index, GLdouble *params) {
  _glr->glExt.glGetProgramEnvParameterdvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterfvARB (GLenum target, GLuint index, GLfloat *params) {
  _glr->glExt.glGetProgramEnvParameterfvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterdvARB (GLenum target, GLuint index, GLdouble *params) {
  _glr->glExt.glGetProgramLocalParameterdvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterfvARB (GLenum target, GLuint index, GLfloat *params) {
  _glr->glExt.glGetProgramLocalParameterfvARB (target, index, params);}
inline GLAPI void APIENTRY glGetProgramivARB (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetProgramivARB (target, pname, params);}
inline GLAPI void APIENTRY glGetProgramStringARB (GLenum target, GLenum pname, void *string) {
  _glr->glExt.glGetProgramStringARB (target, pname, string);}
inline GLAPI GLboolean APIENTRY glIsProgramARB (GLuint program) {
  return _glr->glExt.glIsProgramARB (program);}
// #047 GL_ARB_geometry_shader4
inline GLAPI void APIENTRY glProgramParameteriARB (GLuint program, GLenum pname, GLint value) {
  _glr->glExt.glProgramParameteriARB (program, pname, value);}
inline GLAPI void APIENTRY glFramebufferTextureARB (GLenum target, GLenum attachment, GLuint texture, GLint level) {
  _glr->glExt.glFramebufferTextureARB (target, attachment, texture, level);}
inline GLAPI void APIENTRY glFramebufferTextureLayerARB (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  _glr->glExt.glFramebufferTextureLayerARB (target, attachment, texture, level, layer);}
inline GLAPI void APIENTRY glFramebufferTextureFaceARB (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face) {
  _glr->glExt.glFramebufferTextureFaceARB (target, attachment, texture, level, face);}
#ifdef OS_WIN
// # GL_ARB_imaging - not in openGL registry list
inline GLAPI void APIENTRY glColorTable (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *table) {
  _glr->glExt.glColorTable (target, internalformat, width, format, type, table);}
inline GLAPI void APIENTRY glColorTableParameterfv (GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glColorTableParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glColorTableParameteriv (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glColorTableParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glCopyColorTable (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyColorTable (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glGetColorTable (GLenum target, GLenum format, GLenum type, void *table) {
  _glr->glExt.glGetColorTable (target, format, type, table);}
inline GLAPI void APIENTRY glGetColorTableParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetColorTableParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetColorTableParameteriv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetColorTableParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glColorSubTable (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glColorSubTable (target, start, count, format, type, data);}
inline GLAPI void APIENTRY glCopyColorSubTable (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyColorSubTable (target, start, x, y, width);}
inline GLAPI void APIENTRY glConvolutionFilter1D (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image) {
  _glr->glExt.glConvolutionFilter1D (target, internalformat, width, format, type, image);}
inline GLAPI void APIENTRY glConvolutionFilter2D (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image) {
  _glr->glExt.glConvolutionFilter2D (target, internalformat, width, height, format, type, image);}
inline GLAPI void APIENTRY glConvolutionParameterf (GLenum target, GLenum pname, GLfloat params) {
  _glr->glExt.glConvolutionParameterf (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameterfv (GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glConvolutionParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameteri (GLenum target, GLenum pname, GLint params) {
  _glr->glExt.glConvolutionParameteri (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameteriv (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glConvolutionParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glCopyConvolutionFilter1D (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyConvolutionFilter1D (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glCopyConvolutionFilter2D (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyConvolutionFilter2D (target, internalformat, x, y, width, height);}
inline GLAPI void APIENTRY glGetConvolutionFilter (GLenum target, GLenum format, GLenum type, void *image) {
  _glr->glExt.glGetConvolutionFilter (target, format, type, image);}
inline GLAPI void APIENTRY glGetConvolutionParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetConvolutionParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetConvolutionParameteriv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetConvolutionParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetSeparableFilter (GLenum target, GLenum format, GLenum type, void *row, void *column, void *span) {
  _glr->glExt.glGetSeparableFilter (target, format, type, row, column, span);}
inline GLAPI void APIENTRY glSeparableFilter2D (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column) {
  _glr->glExt.glSeparableFilter2D (target, internalformat, width, height, format, type, row, column);}
inline GLAPI void APIENTRY glGetHistogram (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values) {
  _glr->glExt.glGetHistogram (target, reset, format, type, values);}
inline GLAPI void APIENTRY glGetHistogramParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetHistogramParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetHistogramParameteriv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetHistogramParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmax (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values) {
  _glr->glExt.glGetMinmax (target, reset, format, type, values);}
inline GLAPI void APIENTRY glGetMinmaxParameterfv (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMinmaxParameterfv (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmaxParameteriv (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetMinmaxParameteriv (target, pname, params);}
inline GLAPI void APIENTRY glHistogram (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink) {
  _glr->glExt.glHistogram (target, width, internalformat, sink);}
inline GLAPI void APIENTRY glMinmax (GLenum target, GLenum internalformat, GLboolean sink) {
  _glr->glExt.glMinmax (target, internalformat, sink);}
inline GLAPI void APIENTRY glResetHistogram (GLenum target) {
  _glr->glExt.glResetHistogram (target);}
inline GLAPI void APIENTRY glResetMinmax (GLenum target) {
  _glr->glExt.glResetMinmax (target);}
#endif /// OS_WIN
// #154 GL_ARB_indirect_parameters
inline GLAPI void APIENTRY glMultiDrawArraysIndirectCountARB (GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
  _glr->glExt.glMultiDrawArraysIndirectCountARB (mode, indirect, drawcount, maxdrawcount, stride);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirectCountARB (GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
  _glr->glExt.glMultiDrawElementsIndirectCountARB (mode, type, indirect, drawcount, maxdrawcount, stride);}
// #049 GL_ARB_instanced_arrays
inline GLAPI void APIENTRY glVertexAttribDivisorARB (GLuint index, GLuint divisor) {
  _glr->glExt.glVertexAttribDivisorARB (index, divisor);}
// #016 GL_ARB_matrix_palette
inline GLAPI void APIENTRY glCurrentPaletteMatrixARB (GLint index) {
  _glr->glExt.glCurrentPaletteMatrixARB (index);}
inline GLAPI void APIENTRY glMatrixIndexubvARB (GLint size, const GLubyte *indices) {
  _glr->glExt.glMatrixIndexubvARB (size, indices);}
inline GLAPI void APIENTRY glMatrixIndexusvARB (GLint size, const GLushort *indices) {
  _glr->glExt.glMatrixIndexusvARB (size, indices);}
inline GLAPI void APIENTRY glMatrixIndexuivARB (GLint size, const GLuint *indices) {
  _glr->glExt.glMatrixIndexuivARB (size, indices);}
inline GLAPI void APIENTRY glMatrixIndexPointerARB (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glMatrixIndexPointerARB (size,  type, stride, pointer);}
// #005 GL_ARB_multisample
inline GLAPI void APIENTRY glSampleCoverageARB (GLfloat value, GLboolean invert) {
  _glr->glExt.glSampleCoverageARB (value, invert);}
// #001 GL_ARB_multitexture
inline GLAPI void APIENTRY glActiveTextureARB (GLenum texture) {
  _glr->glExt.glActiveTextureARB (texture);}
inline GLAPI void APIENTRY glClientActiveTextureARB (GLenum texture) {
  _glr->glExt.glClientActiveTextureARB (texture);}
inline GLAPI void APIENTRY glMultiTexCoord1dARB (GLenum target, GLdouble s) {
  _glr->glExt.glMultiTexCoord1dARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1dvARB (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord1dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord1fARB (GLenum target, GLfloat s) {
  _glr->glExt.glMultiTexCoord1fARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1fvARB (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord1fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord1iARB (GLenum target, GLint s) {
  _glr->glExt.glMultiTexCoord1iARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1ivARB (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord1ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord1sARB (GLenum target, GLshort s) {
  _glr->glExt.glMultiTexCoord1sARB (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1svARB (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord1svARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2dARB (GLenum target, GLdouble s, GLdouble t) {
  _glr->glExt.glMultiTexCoord2dARB (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2dvARB (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord2dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2fARB (GLenum target, GLfloat s, GLfloat t) {
  _glr->glExt.glMultiTexCoord2fARB (target,  s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2fvARB (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord2fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2iARB (GLenum target, GLint s, GLint t) {
  _glr->glExt.glMultiTexCoord2iARB (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2ivARB (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord2ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2sARB (GLenum target, GLshort s, GLshort t) {
  _glr->glExt.glMultiTexCoord2sARB (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2svARB (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord2svARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r) {
  _glr->glExt.glMultiTexCoord3dARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3dvARB (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord3dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r) {
  _glr->glExt.glMultiTexCoord3fARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3fvARB (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord3fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3iARB (GLenum target, GLint s, GLint t, GLint r) {
  _glr->glExt.glMultiTexCoord3iARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3ivARB (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord3ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3sARB (GLenum target, GLshort s, GLshort t, GLshort r) {
  _glr->glExt.glMultiTexCoord3sARB (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3svARB (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord3svARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q) {
  _glr->glExt.glMultiTexCoord4dARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4dvARB (GLenum target, const GLdouble *v) {
  _glr->glExt.glMultiTexCoord4dvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
  _glr->glExt.glMultiTexCoord4fARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4fvARB (GLenum target, const GLfloat *v) {
  _glr->glExt.glMultiTexCoord4fvARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4iARB (GLenum target, GLint s, GLint t, GLint r, GLint q) {
  _glr->glExt.glMultiTexCoord4iARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4ivARB (GLenum target, const GLint *v) {
  _glr->glExt.glMultiTexCoord4ivARB (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4sARB (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q) {
  _glr->glExt.glMultiTexCoord4sARB (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4svARB (GLenum target, const GLshort *v) {
  _glr->glExt.glMultiTexCoord4svARB (target, v);}
// #029 GL_ARB_occlusion_query
inline GLAPI void APIENTRY glGenQueriesARB (GLsizei n, GLuint *ids) {
  _glr->glExt.glGenQueriesARB (n, ids);}
inline GLAPI void APIENTRY glDeleteQueriesARB (GLsizei n, const GLuint *ids) {
  _glr->glExt.glDeleteQueriesARB (n, ids);}
inline GLAPI GLboolean APIENTRY glIsQueryARB (GLuint id) {
  return _glr->glExt.glIsQueryARB (id);}
inline GLAPI void APIENTRY glBeginQueryARB (GLenum target, GLuint id) {
  _glr->glExt.glBeginQueryARB (target,  id);}
inline GLAPI void APIENTRY glEndQueryARB (GLenum target) {
  _glr->glExt.glEndQueryARB (target);}
inline GLAPI void APIENTRY glGetQueryivARB (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetQueryivARB (target, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectivARB (GLuint id, GLenum pname, GLint *params) {
  _glr->glExt.glGetQueryObjectivARB (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectuivARB (GLuint id, GLenum pname, GLuint *params) {
  _glr->glExt.glGetQueryObjectuivARB (id, pname, params);}
// #014 GL_ARB_point_parameters
inline GLAPI void APIENTRY glPointParameterfARB (GLenum pname, GLfloat param) {
  _glr->glExt.glPointParameterfARB (pname, param);}
inline GLAPI void APIENTRY glPointParameterfvARB (GLenum pname, const GLfloat *params) {
  _glr->glExt.glPointParameterfvARB (pname, params);}
// #105 GL_ARB_robustness
inline GLAPI GLenum APIENTRY glGetGraphicsResetStatusARB (void) {
  return _glr->glExt.glGetGraphicsResetStatusARB ();}
inline GLAPI void APIENTRY glGetnTexImageARB (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *img) {
  _glr->glExt.glGetnTexImageARB (target, level, format, type, bufSize, img);}
inline GLAPI void APIENTRY glReadnPixelsARB (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data) {
  _glr->glExt.glReadnPixelsARB (x, y, width, height, format, type, bufSize, data);}
inline GLAPI void APIENTRY glGetnCompressedTexImageARB (GLenum target, GLint lod, GLsizei bufSize, void *img) {
  _glr->glExt.glGetnCompressedTexImageARB (target, lod, bufSize, img);}
inline GLAPI void APIENTRY glGetnUniformfvARB (GLuint program, GLint location, GLsizei bufSize, GLfloat *params) {
  _glr->glExt.glGetnUniformfvARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnUniformivARB (GLuint program, GLint location, GLsizei bufSize, GLint *params) {
  _glr->glExt.glGetnUniformivARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnUniformuivARB (GLuint program, GLint location, GLsizei bufSize, GLuint *params) {
  _glr->glExt.glGetnUniformuivARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnUniformdvARB (GLuint program, GLint location, GLsizei bufSize, GLdouble *params) {
  _glr->glExt.glGetnUniformdvARB (program, location, bufSize, params);}
inline GLAPI void APIENTRY glGetnMapdvARB (GLenum target, GLenum query, GLsizei bufSize, GLdouble *v) {
  _glr->glExt.glGetnMapdvARB (target, query, bufSize, v);}
inline GLAPI void APIENTRY glGetnMapfvARB (GLenum target, GLenum query, GLsizei bufSize, GLfloat *v) {
  _glr->glExt.glGetnMapfvARB (target, query, bufSize, v);}
inline GLAPI void APIENTRY glGetnMapivARB (GLenum target, GLenum query, GLsizei bufSize, GLint *v) {
  _glr->glExt.glGetnMapivARB (target, query, bufSize, v);}
inline GLAPI void APIENTRY glGetnPixelMapfvARB (GLenum map, GLsizei bufSize, GLfloat *values) {
  _glr->glExt.glGetnPixelMapfvARB (map, bufSize, values);}
inline GLAPI void APIENTRY glGetnPixelMapuivARB (GLenum map, GLsizei bufSize, GLuint *values) {
  _glr->glExt.glGetnPixelMapuivARB (map, bufSize, values);}
inline GLAPI void APIENTRY glGetnPixelMapusvARB (GLenum map, GLsizei bufSize, GLushort *values) {
  _glr->glExt.glGetnPixelMapusvARB (map, bufSize, values);}
inline GLAPI void APIENTRY glGetnPolygonStippleARB (GLsizei bufSize, GLubyte *pattern) {
  _glr->glExt.glGetnPolygonStippleARB (bufSize, pattern);}
inline GLAPI void APIENTRY glGetnColorTableARB (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table) {
  _glr->glExt.glGetnColorTableARB (target, format, type, bufSize, table);}
inline GLAPI void APIENTRY glGetnConvolutionFilterARB (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image) {
  _glr->glExt.glGetnConvolutionFilterARB (target, format, type, bufSize, image);}
inline GLAPI void APIENTRY glGetnSeparableFilterARB (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span) {
  _glr->glExt.glGetnSeparableFilterARB (target, format, type, rowBufSize, row, columnBufSize, column, span);}
inline GLAPI void APIENTRY glGetnHistogramARB (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) {
  _glr->glExt.glGetnHistogramARB (target, reset, format, type, bufSize, values);}
inline GLAPI void APIENTRY glGetnMinmaxARB (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) {
  _glr->glExt.glGetnMinmaxARB (target, reset, format, type, bufSize, values);}
// #070 GL_ARB_sample_shading
inline GLAPI void APIENTRY glMinSampleShadingARB (GLfloat value) {
  _glr->glExt.glMinSampleShadingARB (value);}
// #030 GL_ARB_shader_objects
inline GLAPI void APIENTRY glDeleteObjectARB (GLhandleARB obj) {
  _glr->glExt.glDeleteObjectARB (obj);}
inline GLAPI GLhandleARB APIENTRY glGetHandleARB (GLenum pname) {
  return _glr->glExt.glGetHandleARB (pname);}
inline GLAPI void APIENTRY glDetachObjectARB (GLhandleARB containerObj, GLhandleARB attachedObj) {
  _glr->glExt.glDetachObjectARB (containerObj, attachedObj);}
inline GLAPI GLhandleARB APIENTRY glCreateShaderObjectARB (GLenum shaderType) {
  return _glr->glExt.glCreateShaderObjectARB (shaderType);}
inline GLAPI void APIENTRY glShaderSourceARB (GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length) {
  _glr->glExt.glShaderSourceARB (shaderObj, count, string, length);}
inline GLAPI void APIENTRY glCompileShaderARB (GLhandleARB shaderObj) {
  _glr->glExt.glCompileShaderARB (shaderObj);}
inline GLAPI GLhandleARB APIENTRY glCreateProgramObjectARB (void) {
  return _glr->glExt.glCreateProgramObjectARB ();}
inline GLAPI void APIENTRY glAttachObjectARB (GLhandleARB containerObj, GLhandleARB obj) {
  _glr->glExt.glAttachObjectARB (containerObj, obj);}
inline GLAPI void APIENTRY glLinkProgramARB (GLhandleARB programObj) {
  _glr->glExt.glLinkProgramARB (programObj);}
inline GLAPI void APIENTRY glUseProgramObjectARB (GLhandleARB programObj) {
  _glr->glExt.glUseProgramObjectARB (programObj);}
inline GLAPI void APIENTRY glValidateProgramARB (GLhandleARB programObj) {
  _glr->glExt.glValidateProgramARB (programObj);}
inline GLAPI void APIENTRY glUniform1fARB (GLint location, GLfloat v0) {
  _glr->glExt.glUniform1fARB (location, v0);}
inline GLAPI void APIENTRY glUniform2fARB (GLint location, GLfloat v0, GLfloat v1) {
  _glr->glExt.glUniform2fARB (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3fARB (GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  _glr->glExt.glUniform3fARB (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4fARB (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  _glr->glExt.glUniform4fARB (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1iARB (GLint location, GLint v0) {
  _glr->glExt.glUniform1iARB (location, v0);}
inline GLAPI void APIENTRY glUniform2iARB (GLint location, GLint v0, GLint v1) {
  _glr->glExt.glUniform2iARB (location, v0, v1);}
inline GLAPI void APIENTRY glUniform3iARB (GLint location, GLint v0, GLint v1, GLint v2) {
  _glr->glExt.glUniform3iARB (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4iARB (GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  _glr->glExt.glUniform4iARB (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1fvARB (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform1fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform2fvARB (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform2fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform3fvARB (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform3fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform4fvARB (GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glUniform4fvARB (location, count, value);}
inline GLAPI void APIENTRY glUniform1ivARB (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform1ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniform2ivARB (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform2ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniform3ivARB (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform3ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniform4ivARB (GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glUniform4ivARB (location, count, value);}
inline GLAPI void APIENTRY glUniformMatrix2fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix2fvARB (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix3fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix3fvARB (location, count, transpose, value);}
inline GLAPI void APIENTRY glUniformMatrix4fvARB (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glUniformMatrix4fvARB (location, count, transpose, value);}
inline GLAPI void APIENTRY glGetObjectParameterfvARB (GLhandleARB obj, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetObjectParameterfvARB (obj, pname, params);}
inline GLAPI void APIENTRY glGetObjectParameterivARB (GLhandleARB obj, GLenum pname, GLint *params) {
  _glr->glExt.glGetObjectParameterivARB (obj, pname, params);}
inline GLAPI void APIENTRY glGetInfoLogARB (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog) {
  _glr->glExt.glGetInfoLogARB (obj, maxLength, length, infoLog);}
inline GLAPI void APIENTRY glGetAttachedObjectsARB (GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj) {
  _glr->glExt.glGetAttachedObjectsARB (containerObj, maxCount, count, obj);}
inline GLAPI GLint APIENTRY glGetUniformLocationARB (GLhandleARB programObj, const GLcharARB *name) {
  return _glr->glExt.glGetUniformLocationARB (programObj, name);}
inline GLAPI void APIENTRY glGetActiveUniformARB (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name) {
  _glr->glExt.glGetActiveUniformARB (programObj, index, maxLength, length, size, type, name);}
inline GLAPI void APIENTRY glGetUniformfvARB (GLhandleARB programObj, GLint location, GLfloat *params) {
  _glr->glExt.glGetUniformfvARB (programObj, location, params);}
inline GLAPI void APIENTRY glGetUniformivARB (GLhandleARB programObj, GLint location, GLint *params) {
  _glr->glExt.glGetUniformivARB (programObj, location, params);}
inline GLAPI void APIENTRY glGetShaderSourceARB (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source) {
  _glr->glExt.glGetShaderSourceARB (obj, maxLength, length, source);}
// #076 GL_ARB_shading_language_include
inline GLAPI void APIENTRY glNamedStringARB (GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string) {
  _glr->glExt.glNamedStringARB (type, namelen, name, stringlen, string);}
inline GLAPI void APIENTRY glDeleteNamedStringARB (GLint namelen, const GLchar *name) {
  _glr->glExt.glDeleteNamedStringARB (namelen, name);}
inline GLAPI void APIENTRY glCompileShaderIncludeARB (GLuint shader, GLsizei count, const GLchar *const*path, const GLint *length) {
  _glr->glExt.glCompileShaderIncludeARB (shader, count, path, length);}
inline GLAPI GLboolean APIENTRY glIsNamedStringARB (GLint namelen, const GLchar *name) {
  return _glr->glExt.glIsNamedStringARB (namelen, name);}
inline GLAPI void APIENTRY glGetNamedStringARB (GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string) {
  _glr->glExt.glGetNamedStringARB (namelen, name, bufSize, stringlen, string);}
inline GLAPI void APIENTRY glGetNamedStringivARB (GLint namelen, const GLchar *name, GLenum pname, GLint *params) {
  _glr->glExt.glGetNamedStringivARB (namelen, name, pname, params);}
// #158 GL_ARB_sparse_texture
inline GLAPI void APIENTRY glTexPageCommitmentARB (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean resident) {
  _glr->glExt.glTexPageCommitmentARB (target, level, xoffset, yoffset, zoffset, width, height, depth, resident);}
// #092 GL_ARB_texture_buffer_object
inline GLAPI void APIENTRY glTexBufferARB (GLenum target, GLenum internalformat, GLuint buffer) {
  _glr->glExt.glTexBufferARB (target, internalformat, buffer);}
// #012 GL_ARB_texture_compression
inline GLAPI void APIENTRY glCompressedTexImage3DARB (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexImage3DARB (target, level, internalformat, width, height, depth, border, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexImage2DARB (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexImage2DARB (target, level, internalformat, width, height, border, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexImage1DARB (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexImage1DARB (target, level, internalformat, width, border, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexSubImage3DARB (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexSubImage3DARB (target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexSubImage2DARB (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexSubImage2DARB (target, level, xoffset, yoffset, width, height, format, imageSize, data);}
inline GLAPI void APIENTRY glCompressedTexSubImage1DARB (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) {
  _glr->glExt.glCompressedTexSubImage1DARB (target, level, xoffset, width, format, imageSize, data);}
inline GLAPI void APIENTRY glGetCompressedTexImageARB (GLenum target, GLint level, void *img) {
  _glr->glExt.glGetCompressedTexImageARB (target, level, img);}
// #003 GL_ARB_transpose_matrix
inline GLAPI void APIENTRY glLoadTransposeMatrixfARB (const GLfloat *m) {
  _glr->glExt.glLoadTransposeMatrixfARB (m);}
inline GLAPI void APIENTRY glLoadTransposeMatrixdARB (const GLdouble *m) {
  _glr->glExt.glLoadTransposeMatrixdARB (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixfARB (const GLfloat *m) {
  _glr->glExt.glMultTransposeMatrixfARB (m);}
inline GLAPI void APIENTRY glMultTransposeMatrixdARB (const GLdouble *m) {
  _glr->glExt.glMultTransposeMatrixdARB (m);}
// #015 GL_ARB_vertex_blend
inline GLAPI void APIENTRY glWeightbvARB (GLint size, const GLbyte *weights) {
  _glr->glExt.glWeightbvARB (size, weights);}
inline GLAPI void APIENTRY glWeightsvARB (GLint size, const GLshort *weights) {
  _glr->glExt.glWeightsvARB (size, weights);}
inline GLAPI void APIENTRY glWeightivARB (GLint size, const GLint *weights) {
  _glr->glExt.glWeightivARB (size, weights);}
inline GLAPI void APIENTRY glWeightfvARB (GLint size, const GLfloat *weights) {
  _glr->glExt.glWeightfvARB (size, weights);}
inline GLAPI void APIENTRY glWeightdvARB (GLint size, const GLdouble *weights) {
  _glr->glExt.glWeightdvARB (size, weights);}
inline GLAPI void APIENTRY glWeightubvARB (GLint size, const GLubyte *weights) {
  _glr->glExt.glWeightubvARB (size, weights);}
inline GLAPI void APIENTRY glWeightusvARB (GLint size, const GLushort *weights) {
  _glr->glExt.glWeightusvARB (size, weights);}
inline GLAPI void APIENTRY glWeightuivARB (GLint size, const GLuint *weights) {
  _glr->glExt.glWeightuivARB (size, weights);}
inline GLAPI void APIENTRY glWeightPointerARB (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glWeightPointerARB (size, type, stride, pointer);}
inline GLAPI void APIENTRY glVertexBlendARB (GLint count) {
  _glr->glExt.glVertexBlendARB (count);}
// #028 GL_ARB_vertex_buffer_object
inline GLAPI void APIENTRY glBindBufferARB (GLenum target, GLuint buffer) {
  _glr->glExt.glBindBufferARB (target, buffer);}
inline GLAPI void APIENTRY glDeleteBuffersARB (GLsizei n, const GLuint *buffers) {
  _glr->glExt.glDeleteBuffersARB (n, buffers);}
inline GLAPI void APIENTRY glGenBuffersARB (GLsizei n, GLuint *buffers) {
  _glr->glExt.glGenBuffersARB (n, buffers);}
inline GLAPI GLboolean APIENTRY glIsBufferARB (GLuint buffer) {
  return _glr->glExt.glIsBufferARB (buffer);}
inline GLAPI void APIENTRY glBufferDataARB (GLenum target, GLsizeiptrARB size, const void *data, GLenum usage) {
  _glr->glExt.glBufferDataARB (target, size, data, usage);}
inline GLAPI void APIENTRY glBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void *data) {
  _glr->glExt.glBufferSubDataARB (target, offset, size, data);}
inline GLAPI void APIENTRY glGetBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, void *data) {
  _glr->glExt.glGetBufferSubDataARB (target, offset, size, data);}
inline GLAPI void *APIENTRY glMapBufferARB (GLenum target, GLenum access) {
  return _glr->glExt.glMapBufferARB (target, access);}
inline GLAPI GLboolean APIENTRY glUnmapBufferARB (GLenum target) {
  return _glr->glExt.glUnmapBufferARB (target);}
inline GLAPI void APIENTRY glGetBufferParameterivARB (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetBufferParameterivARB (target, pname, params);}
inline GLAPI void APIENTRY glGetBufferPointervARB (GLenum target, GLenum pname, void **params) {
  _glr->glExt.glGetBufferPointervARB (target, pname, params);}
// #026 GL_ARB_vertex_program
inline GLAPI void APIENTRY glVertexAttrib1dARB (GLuint index, GLdouble x) {
  _glr->glExt.glVertexAttrib1dARB (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1dvARB (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib1dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1fARB (GLuint index, GLfloat x) {
  _glr->glExt.glVertexAttrib1fARB (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1fvARB (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib1fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1sARB (GLuint index, GLshort x) {
  _glr->glExt.glVertexAttrib1sARB (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1svARB (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib1svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2dARB (GLuint index, GLdouble x, GLdouble y) {
  _glr->glExt.glVertexAttrib2dARB (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2dvARB (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib2dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2fARB (GLuint index, GLfloat x, GLfloat y) {
  _glr->glExt.glVertexAttrib2fARB (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2fvARB (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib2fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2sARB (GLuint index, GLshort x, GLshort y) {
  _glr->glExt.glVertexAttrib2sARB (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2svARB (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib2svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3dARB (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glVertexAttrib3dARB (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3dvARB (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib3dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3fARB (GLuint index, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glVertexAttrib3fARB (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3fvARB (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib3fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3sARB (GLuint index, GLshort x, GLshort y, GLshort z) {
  _glr->glExt.glVertexAttrib3sARB (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3svARB (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib3svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NbvARB (GLuint index, const GLbyte *v) {
  _glr->glExt.glVertexAttrib4NbvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NivARB (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttrib4NivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NsvARB (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib4NsvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NubARB (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
  _glr->glExt.glVertexAttrib4NubARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4NubvARB (GLuint index, const GLubyte *v) {
  _glr->glExt.glVertexAttrib4NubvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NuivARB (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttrib4NuivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4NusvARB (GLuint index, const GLushort *v) {
  _glr->glExt.glVertexAttrib4NusvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4bvARB (GLuint index, const GLbyte *v) {
  _glr->glExt.glVertexAttrib4bvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4dARB (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glVertexAttrib4dARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4dvARB (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib4dvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4fARB (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glVertexAttrib4fARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4fvARB (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib4fvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ivARB (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttrib4ivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4sARB (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
  _glr->glExt.glVertexAttrib4sARB (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4svARB (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib4svARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ubvARB (GLuint index, const GLubyte *v) {
  _glr->glExt.glVertexAttrib4ubvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4uivARB (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttrib4uivARB (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4usvARB (GLuint index, const GLushort *v) {
  _glr->glExt.glVertexAttrib4usvARB (index, v);}
inline GLAPI void APIENTRY glVertexAttribPointerARB (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexAttribPointerARB (index, size, type, normalized, stride, pointer);}
inline GLAPI void APIENTRY glEnableVertexAttribArrayARB (GLuint index) {
  _glr->glExt.glEnableVertexAttribArrayARB (index);}
inline GLAPI void APIENTRY glDisableVertexAttribArrayARB (GLuint index) {
  _glr->glExt.glDisableVertexAttribArrayARB (index);}
inline GLAPI void APIENTRY glGetVertexAttribdvARB (GLuint index, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetVertexAttribdvARB (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribfvARB (GLuint index, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetVertexAttribfvARB (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribivARB (GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetVertexAttribivARB (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribPointervARB (GLuint index, GLenum pname, void **pointer) {
  _glr->glExt.glGetVertexAttribPointervARB (index, pname, pointer);}
// #031 GL_ARB_vertex_shader
inline GLAPI void APIENTRY glBindAttribLocationARB (GLhandleARB programObj, GLuint index, const GLcharARB *name) {
  _glr->glExt.glBindAttribLocationARB (programObj, index, name);}
inline GLAPI void APIENTRY glGetActiveAttribARB (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name) {
  _glr->glExt.glGetActiveAttribARB (programObj, index, maxLength, length, size, type, name);}
inline GLAPI GLint APIENTRY glGetAttribLocationARB (GLhandleARB programObj, const GLcharARB *name) {
  return _glr->glExt.glGetAttribLocationARB (programObj, name);}
// #025 GL_ARB_window_pos
inline GLAPI void APIENTRY glWindowPos2dARB (GLdouble x, GLdouble y) {
  _glr->glExt.glWindowPos2dARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2dvARB (const GLdouble *v) {
  _glr->glExt.glWindowPos2dvARB (v);}
inline GLAPI void APIENTRY glWindowPos2fARB (GLfloat x, GLfloat y) {
  _glr->glExt.glWindowPos2fARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2fvARB (const GLfloat *v) {
  _glr->glExt.glWindowPos2fvARB (v);}
inline GLAPI void APIENTRY glWindowPos2iARB (GLint x, GLint y) {
  _glr->glExt.glWindowPos2iARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2ivARB (const GLint *v) {
  _glr->glExt.glWindowPos2ivARB (v);}
inline GLAPI void APIENTRY glWindowPos2sARB (GLshort x, GLshort y) {
  _glr->glExt.glWindowPos2sARB (x, y);}
inline GLAPI void APIENTRY glWindowPos2svARB (const GLshort *v) {
  _glr->glExt.glWindowPos2svARB (v);}
inline GLAPI void APIENTRY glWindowPos3dARB (GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glWindowPos3dARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3dvARB (const GLdouble *v) {
  _glr->glExt.glWindowPos3dvARB (v);}
inline GLAPI void APIENTRY glWindowPos3fARB (GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glWindowPos3fARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3fvARB (const GLfloat *v) {
  _glr->glExt.glWindowPos3fvARB (v);}
inline GLAPI void APIENTRY glWindowPos3iARB (GLint x, GLint y, GLint z) {
  _glr->glExt.glWindowPos3iARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3ivARB (const GLint *v) {
  _glr->glExt.glWindowPos3ivARB (v);}
inline GLAPI void APIENTRY glWindowPos3sARB (GLshort x, GLshort y, GLshort z) {
  _glr->glExt.glWindowPos3sARB (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3svARB (const GLshort *v) {
  _glr->glExt.glWindowPos3svARB (v);}






///==============///
// WGL extensions //
///==============///


#ifdef OS_WIN

// WGL_ARB_buffer_region
inline HANDLE WINAPI wglCreateBufferRegionARB (HDC hDC, int iLayerPlane, UINT uType) {
  return _glr->glExt.wglCreateBufferRegionARB (hDC, iLayerPlane, uType);}
inline VOID WINAPI wglDeleteBufferRegionARB (HANDLE hRegion) {
  _glr->glExt.wglDeleteBufferRegionARB (hRegion);}
inline BOOL WINAPI wglSaveBufferRegionARB (HANDLE hRegion, int x, int y, int width, int height) {
  return _glr->glExt.wglSaveBufferRegionARB (hRegion, x, y, width, height);}
inline BOOL WINAPI wglRestoreBufferRegionARB (HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc) {
  return _glr->glExt.wglRestoreBufferRegionARB (hRegion, x, y, width, height, xSrc, ySrc);}

// WGL_ARB_create_context
inline HGLRC WINAPI wglCreateContextAttribsARB (HDC hDC, HGLRC hShareContext, const int *attribList) {
  return _glr->glExt.wglCreateContextAttribsARB (hDC, hShareContext, attribList);}

// WGL_ARB_extensions_string
inline const char *WINAPI wglGetExtensionsStringARB (HDC hdc) {
  return _glr->glExt.wglGetExtensionsStringARB (hdc);}

// WGL_ARB_make_current_read
inline BOOL WINAPI wglMakeContextCurrentARB (HDC hDrawDC, HDC hReadDC, HGLRC hglrc) {
  return _glr->glExt.wglMakeContextCurrentARB (hDrawDC, hReadDC, hglrc);}
inline HDC WINAPI wglGetCurrentReadDCARB (void) {
  return _glr->glExt.wglGetCurrentReadDCARB ();}

// WGL_ARB_pbuffer
inline HPBUFFERARB WINAPI wglCreatePbufferARB (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList) {
  return _glr->glExt.wglCreatePbufferARB (hDC, iPixelFormat, iWidth, iHeight, piAttribList);}
inline HDC WINAPI wglGetPbufferDCARB (HPBUFFERARB hPbuffer) {
  return _glr->glExt.wglGetPbufferDCARB (hPbuffer);}
inline int WINAPI wglReleasePbufferDCARB (HPBUFFERARB hPbuffer, HDC hDC) {
  return _glr->glExt.wglReleasePbufferDCARB (hPbuffer, hDC);}
inline BOOL WINAPI wglDestroyPbufferARB (HPBUFFERARB hPbuffer) {
  return _glr->glExt.wglDestroyPbufferARB (hPbuffer);}
inline BOOL WINAPI wglQueryPbufferARB (HPBUFFERARB hPbuffer, int iAttribute, int *piValue) {
  return _glr->glExt.wglQueryPbufferARB (hPbuffer, iAttribute, piValue);}

// WGL_ARB_pixel_format
inline BOOL WINAPI wglGetPixelFormatAttribivARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues) {
  return _glr->glExt.wglGetPixelFormatAttribivARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);}
inline BOOL WINAPI wglGetPixelFormatAttribfvARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues) {
  return _glr->glExt.wglGetPixelFormatAttribfvARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);}
inline BOOL WINAPI wglChoosePixelFormatARB (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats) {
  return _glr->glExt.wglChoosePixelFormatARB (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);}

// WGL_ARB_render_texture
inline BOOL WINAPI wglBindTexImageARB (HPBUFFERARB hPbuffer, int iBuffer) {
  return _glr->glExt.wglBindTexImageARB (hPbuffer, iBuffer);}
inline BOOL WINAPI wglReleaseTexImageARB (HPBUFFERARB hPbuffer, int iBuffer) {
  return _glr->glExt.wglReleaseTexImageARB (hPbuffer, iBuffer);}
inline BOOL WINAPI wglSetPbufferAttribARB (HPBUFFERARB hPbuffer, const int *piAttribList) {
  return _glr->glExt.wglSetPbufferAttribARB (hPbuffer, piAttribList);}

// WGL_3DL_stereo_control
inline BOOL WINAPI wglSetStereoEmitterState3DL (HDC hDC, UINT uState) {
  return _glr->glExt.wglSetStereoEmitterState3DL (hDC, uState);}

// WGL_AMD_gpu_association
inline UINT WINAPI wglGetGPUIDsAMD (UINT maxCount, UINT *ids) {
  return _glr->glExt.wglGetGPUIDsAMD (maxCount, ids);}
inline INT WINAPI wglGetGPUInfoAMD (UINT id, int property, GLenum dataType, UINT size, void *data) {
  return _glr->glExt.wglGetGPUInfoAMD (id, property, dataType, size, data);}
inline UINT WINAPI wglGetContextGPUIDAMD (HGLRC hglrc) {
  return _glr->glExt.wglGetContextGPUIDAMD (hglrc);}
inline HGLRC WINAPI wglCreateAssociatedContextAMD (UINT id) {
  return _glr->glExt.wglCreateAssociatedContextAMD (id);}
inline HGLRC WINAPI wglCreateAssociatedContextAttribsAMD (UINT id, HGLRC hShareContext, const int *attribList) {
  return _glr->glExt.wglCreateAssociatedContextAttribsAMD (id, hShareContext, attribList);}
inline BOOL WINAPI wglDeleteAssociatedContextAMD (HGLRC hglrc) {
  return _glr->glExt.wglDeleteAssociatedContextAMD (hglrc);}
inline BOOL WINAPI wglMakeAssociatedContextCurrentAMD (HGLRC hglrc) {
  return _glr->glExt.wglMakeAssociatedContextCurrentAMD (hglrc);}
inline HGLRC WINAPI wglGetCurrentAssociatedContextAMD (void) {
  return _glr->glExt.wglGetCurrentAssociatedContextAMD ();}
inline VOID WINAPI wglBlitContextFramebufferAMD (HGLRC dstCtx, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
  _glr->glExt.wglBlitContextFramebufferAMD (dstCtx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);}

// WGL_EXT_display_color_table
inline GLboolean WINAPI wglCreateDisplayColorTableEXT (GLushort id) {
  return _glr->glExt.wglCreateDisplayColorTableEXT (id);}
inline GLboolean WINAPI wglLoadDisplayColorTableEXT (const GLushort *table, GLuint length) {
  return _glr->glExt.wglLoadDisplayColorTableEXT (table, length);}
inline GLboolean WINAPI wglBindDisplayColorTableEXT (GLushort id) {
  return _glr->glExt.wglBindDisplayColorTableEXT (id);}
inline VOID WINAPI wglDestroyDisplayColorTableEXT (GLushort id) {
  _glr->glExt.wglDestroyDisplayColorTableEXT (id);}

// WGL_EXT_extensions_string
inline const char *WINAPI wglGetExtensionsStringEXT (void) {
  return _glr->glExt.wglGetExtensionsStringEXT ();}

// WGL_EXT_make_current_read
inline BOOL WINAPI wglMakeContextCurrentEXT (HDC hDrawDC, HDC hReadDC, HGLRC hglrc) {
  return _glr->glExt.wglMakeContextCurrentEXT (hDrawDC, hReadDC, hglrc);}
inline HDC WINAPI wglGetCurrentReadDCEXT (void) {
  return _glr->glExt.wglGetCurrentReadDCEXT ();}

// WGL_EXT_pbuffer
inline HPBUFFEREXT WINAPI wglCreatePbufferEXT (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList) {
  return _glr->glExt.wglCreatePbufferEXT (hDC, iPixelFormat, iWidth, iHeight, piAttribList);}
inline HDC WINAPI wglGetPbufferDCEXT (HPBUFFEREXT hPbuffer) {
  return _glr->glExt.wglGetPbufferDCEXT (hPbuffer);}
inline int WINAPI wglReleasePbufferDCEXT (HPBUFFEREXT hPbuffer, HDC hDC) {
  return _glr->glExt.wglReleasePbufferDCEXT (hPbuffer, hDC);}
inline BOOL WINAPI wglDestroyPbufferEXT (HPBUFFEREXT hPbuffer) {
  return _glr->glExt.wglDestroyPbufferEXT (hPbuffer);}
inline BOOL WINAPI wglQueryPbufferEXT (HPBUFFEREXT hPbuffer, int iAttribute, int *piValue) {
  return _glr->glExt.wglQueryPbufferEXT (hPbuffer, iAttribute, piValue);}

// WGL_EXT_pixel_format
inline BOOL WINAPI wglGetPixelFormatAttribivEXT (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues) {
  return _glr->glExt.wglGetPixelFormatAttribivEXT (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);}
inline BOOL WINAPI wglGetPixelFormatAttribfvEXT (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues) {
  return _glr->glExt.wglGetPixelFormatAttribfvEXT (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);}
inline BOOL WINAPI wglChoosePixelFormatEXT (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats) {
  return _glr->glExt.wglChoosePixelFormatEXT (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);}

// WGL_EXT_swap_control
inline BOOL WINAPI wglSwapIntervalEXT (int interval) {
  return _glr->glExt.wglSwapIntervalEXT (interval);}
inline int WINAPI wglGetSwapIntervalEXT (void) {
  return _glr->glExt.wglGetSwapIntervalEXT ();}

// WGL_I3D_digital_video_control
inline BOOL WINAPI wglGetDigitalVideoParametersI3D (HDC hDC, int iAttribute, int *piValue) {
  return _glr->glExt.wglGetDigitalVideoParametersI3D (hDC, iAttribute, piValue);}
inline BOOL WINAPI wglSetDigitalVideoParametersI3D (HDC hDC, int iAttribute, const int *piValue) {
  return _glr->glExt.wglSetDigitalVideoParametersI3D (hDC, iAttribute, piValue);}

// WGL_I3D_gamma
inline BOOL WINAPI wglGetGammaTableParametersI3D (HDC hDC, int iAttribute, int *piValue) {
  return _glr->glExt.wglGetGammaTableParametersI3D (hDC, iAttribute, piValue);}
inline BOOL WINAPI wglSetGammaTableParametersI3D (HDC hDC, int iAttribute, const int *piValue) {
  return _glr->glExt.wglSetGammaTableParametersI3D (hDC, iAttribute, piValue);}
inline BOOL WINAPI wglGetGammaTableI3D (HDC hDC, int iEntries, USHORT *puRed, USHORT *puGreen, USHORT *puBlue) {
  return _glr->glExt.wglGetGammaTableI3D (hDC, iEntries, puRed, puGreen, puBlue);}
inline BOOL WINAPI wglSetGammaTableI3D (HDC hDC, int iEntries, const USHORT *puRed, const USHORT *puGreen, const USHORT *puBlue) {
  return _glr->glExt.wglSetGammaTableI3D (hDC, iEntries, puRed, puGreen, puBlue);}

// WGL_I3D_genlock
inline BOOL WINAPI wglEnableGenlockI3D (HDC hDC) {
  return _glr->glExt.wglEnableGenlockI3D (hDC);}
inline BOOL WINAPI wglDisableGenlockI3D (HDC hDC) {
  return _glr->glExt.wglDisableGenlockI3D (hDC);}
inline BOOL WINAPI wglIsEnabledGenlockI3D (HDC hDC, BOOL *pFlag) {
  return _glr->glExt.wglIsEnabledGenlockI3D (hDC, pFlag);}
inline BOOL WINAPI wglGenlockSourceI3D (HDC hDC, UINT uSource) {
  return _glr->glExt.wglGenlockSourceI3D (hDC, uSource);}
inline BOOL WINAPI wglGetGenlockSourceI3D (HDC hDC, UINT *uSource) {
  return _glr->glExt.wglGetGenlockSourceI3D (hDC, uSource);}
inline BOOL WINAPI wglGenlockSourceEdgeI3D (HDC hDC, UINT uEdge) {
  return _glr->glExt.wglGenlockSourceEdgeI3D (hDC, uEdge);}
inline BOOL WINAPI wglGetGenlockSourceEdgeI3D (HDC hDC, UINT *uEdge) {
  return _glr->glExt.wglGetGenlockSourceEdgeI3D (hDC, uEdge);}
inline BOOL WINAPI wglGenlockSampleRateI3D (HDC hDC, UINT uRate) {
  return _glr->glExt.wglGenlockSampleRateI3D (hDC, uRate);}
inline BOOL WINAPI wglGetGenlockSampleRateI3D (HDC hDC, UINT *uRate) {
  return _glr->glExt.wglGetGenlockSampleRateI3D (hDC, uRate);}
inline BOOL WINAPI wglGenlockSourceDelayI3D (HDC hDC, UINT uDelay) {
  return _glr->glExt.wglGenlockSourceDelayI3D (hDC, uDelay);}
inline BOOL WINAPI wglGetGenlockSourceDelayI3D (HDC hDC, UINT *uDelay) {
  return _glr->glExt.wglGetGenlockSourceDelayI3D (hDC, uDelay);}
inline BOOL WINAPI wglQueryGenlockMaxSourceDelayI3D (HDC hDC, UINT *uMaxLineDelay, UINT *uMaxPixelDelay) {
  return _glr->glExt.wglQueryGenlockMaxSourceDelayI3D (hDC, uMaxLineDelay, uMaxPixelDelay);}

// WGL_I3D_image_buffer
inline LPVOID WINAPI wglCreateImageBufferI3D (HDC hDC, DWORD dwSize, UINT uFlags) {
  return _glr->glExt.wglCreateImageBufferI3D (hDC, dwSize, uFlags);}
inline BOOL WINAPI wglDestroyImageBufferI3D (HDC hDC, LPVOID pAddress) {
  return _glr->glExt.wglDestroyImageBufferI3D (hDC, pAddress);}
inline BOOL WINAPI wglAssociateImageBufferEventsI3D (HDC hDC, const HANDLE *pEvent, const LPVOID *pAddress, const DWORD *pSize, UINT count) {
  return _glr->glExt.wglAssociateImageBufferEventsI3D (hDC, pEvent, pAddress, pSize, count);}
inline BOOL WINAPI wglReleaseImageBufferEventsI3D (HDC hDC, const LPVOID *pAddress, UINT count) {
  return _glr->glExt.wglReleaseImageBufferEventsI3D (hDC, pAddress, count);}

// WGL_I3D_swap_frame_lock
inline BOOL WINAPI wglEnableFrameLockI3D (void) {
  return _glr->glExt.wglEnableFrameLockI3D ();}
inline BOOL WINAPI wglDisableFrameLockI3D (void) {
  return _glr->glExt.wglDisableFrameLockI3D ();}
inline BOOL WINAPI wglIsEnabledFrameLockI3D (BOOL *pFlag) {
  return _glr->glExt.wglIsEnabledFrameLockI3D (pFlag);}
inline BOOL WINAPI wglQueryFrameLockMasterI3D (BOOL *pFlag) {
  return _glr->glExt.wglQueryFrameLockMasterI3D (pFlag);}

// WGL_I3D_swap_frame_usage
inline BOOL WINAPI wglGetFrameUsageI3D (float *pUsage) {
  return _glr->glExt.wglGetFrameUsageI3D (pUsage);}
inline BOOL WINAPI wglBeginFrameTrackingI3D (void) {
  return _glr->glExt.wglBeginFrameTrackingI3D ();}
inline BOOL WINAPI wglEndFrameTrackingI3D (void) {
  return _glr->glExt.wglEndFrameTrackingI3D ();}
inline BOOL WINAPI wglQueryFrameTrackingI3D (DWORD *pFrameCount, DWORD *pMissedFrames, float *pLastMissedUsage) {
  return _glr->glExt.wglQueryFrameTrackingI3D (pFrameCount, pMissedFrames, pLastMissedUsage);}

// WGL_NV_DX_interop
inline BOOL WINAPI wglDXSetResourceShareHandleNV (void *dxObject, HANDLE shareHandle) {
  return _glr->glExt.wglDXSetResourceShareHandleNV (dxObject, shareHandle);}
inline HANDLE WINAPI wglDXOpenDeviceNV (void *dxDevice) {
  return _glr->glExt.wglDXOpenDeviceNV (dxDevice);}
inline BOOL WINAPI wglDXCloseDeviceNV (HANDLE hDevice) {
  return _glr->glExt.wglDXCloseDeviceNV (hDevice);}
inline HANDLE WINAPI wglDXRegisterObjectNV (HANDLE hDevice, void *dxObject, GLuint name, GLenum type, GLenum access) {
  return _glr->glExt.wglDXRegisterObjectNV (hDevice, dxObject, name, type, access);}
inline BOOL WINAPI wglDXUnregisterObjectNV (HANDLE hDevice, HANDLE hObject) {
  return _glr->glExt.wglDXUnregisterObjectNV (hDevice, hObject);}
inline BOOL WINAPI wglDXObjectAccessNV (HANDLE hObject, GLenum access) {
  return _glr->glExt.wglDXObjectAccessNV (hObject, access);}
inline BOOL WINAPI wglDXLockObjectsNV (HANDLE hDevice, GLint count, HANDLE *hObjects) {
  return _glr->glExt.wglDXLockObjectsNV (hDevice, count, hObjects);}
inline BOOL WINAPI wglDXUnlockObjectsNV (HANDLE hDevice, GLint count, HANDLE *hObjects) {
  return _glr->glExt.wglDXUnlockObjectsNV (hDevice, count, hObjects);}

// WGL_NV_delay_before_swap
inline BOOL WINAPI wglDelayBeforeSwapNV (HDC hDC, GLfloat seconds) {
  return _glr->glExt.wglDelayBeforeSwapNV (hDC, seconds);}

// WGL_NV_gpu_affinity
inline BOOL WINAPI wglEnumGpusNV (UINT iGpuIndex, HGPUNV *phGpu) {
  return _glr->glExt.wglEnumGpusNV (iGpuIndex, phGpu);}
inline BOOL WINAPI wglEnumGpuDevicesNV (HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice) {
  return _glr->glExt.wglEnumGpuDevicesNV (hGpu, iDeviceIndex, lpGpuDevice);}
inline HDC WINAPI wglCreateAffinityDCNV (const HGPUNV *phGpuList) {
  return _glr->glExt.wglCreateAffinityDCNV (phGpuList);}
inline BOOL WINAPI wglEnumGpusFromAffinityDCNV (HDC hAffinityDC, UINT iGpuIndex, HGPUNV *hGpu) {
  return _glr->glExt.wglEnumGpusFromAffinityDCNV (hAffinityDC, iGpuIndex, hGpu);}
inline BOOL WINAPI wglDeleteDCNV (HDC hdc) {
  return _glr->glExt.wglDeleteDCNV (hdc);}

// WGL_NV_present_video
inline int WINAPI wglEnumerateVideoDevicesNV (HDC hDC, HVIDEOOUTPUTDEVICENV *phDeviceList) {
  return _glr->glExt.wglEnumerateVideoDevicesNV (hDC, phDeviceList);}
inline BOOL WINAPI wglBindVideoDeviceNV (HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int *piAttribList) {
  return _glr->glExt.wglBindVideoDeviceNV (hDC, uVideoSlot, hVideoDevice, piAttribList);}
inline BOOL WINAPI wglQueryCurrentContextNV (int iAttribute, int *piValue) {
  return _glr->glExt.wglQueryCurrentContextNV (iAttribute, piValue);}

// WGL_NV_swap_group
inline BOOL WINAPI wglJoinSwapGroupNV (HDC hDC, GLuint group) {
  return _glr->glExt.wglJoinSwapGroupNV (hDC, group);}
inline BOOL WINAPI wglBindSwapBarrierNV (GLuint group, GLuint barrier) {
  return _glr->glExt.wglBindSwapBarrierNV (group, barrier);}
inline BOOL WINAPI wglQuerySwapGroupNV (HDC hDC, GLuint *group, GLuint *barrier) {
  return _glr->glExt.wglQuerySwapGroupNV (hDC, group, barrier);}
inline BOOL WINAPI wglQueryMaxSwapGroupsNV (HDC hDC, GLuint *maxGroups, GLuint *maxBarriers) {
  return _glr->glExt.wglQueryMaxSwapGroupsNV (hDC, maxGroups, maxBarriers);}
inline BOOL WINAPI wglQueryFrameCountNV (HDC hDC, GLuint *count) {
  return _glr->glExt.wglQueryFrameCountNV (hDC, count);}
inline BOOL WINAPI wglResetFrameCountNV (HDC hDC) {
  return _glr->glExt.wglResetFrameCountNV (hDC);}

// WGL_NV_vertex_array_range
inline void *WINAPI wglAllocateMemoryNV (GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority) {
  return _glr->glExt.wglAllocateMemoryNV (size, readfreq, writefreq, priority);}
inline void WINAPI wglFreeMemoryNV (void *pointer) {
  _glr->glExt.wglFreeMemoryNV (pointer);}

// WGL_NV_video_capture
inline BOOL WINAPI wglBindVideoCaptureDeviceNV (UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice) {
  return _glr->glExt.wglBindVideoCaptureDeviceNV (uVideoSlot, hDevice);}
inline UINT WINAPI wglEnumerateVideoCaptureDevicesNV (HDC hDc, HVIDEOINPUTDEVICENV *phDeviceList) {
  return _glr->glExt.wglEnumerateVideoCaptureDevicesNV (hDc, phDeviceList);}
inline BOOL WINAPI wglLockVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
  return _glr->glExt.wglLockVideoCaptureDeviceNV (hDc, hDevice);}
inline BOOL WINAPI wglQueryVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int *piValue) {
  return _glr->glExt.wglQueryVideoCaptureDeviceNV (hDc, hDevice, iAttribute, piValue);}
inline BOOL WINAPI wglReleaseVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
  return _glr->glExt.wglReleaseVideoCaptureDeviceNV (hDc, hDevice);}

// WGL_NV_video_output
inline BOOL WINAPI wglGetVideoDeviceNV (HDC hDC, int numDevices, HPVIDEODEV *hVideoDevice) {
  return _glr->glExt.wglGetVideoDeviceNV (hDC, numDevices, hVideoDevice);}
inline BOOL WINAPI wglReleaseVideoDeviceNV (HPVIDEODEV hVideoDevice) {
  return _glr->glExt.wglReleaseVideoDeviceNV (hVideoDevice);}
inline BOOL WINAPI wglBindVideoImageNV (HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer) {
  return _glr->glExt.wglBindVideoImageNV (hVideoDevice, hPbuffer, iVideoBuffer);}
inline BOOL WINAPI wglReleaseVideoImageNV (HPBUFFERARB hPbuffer, int iVideoBuffer) {
  return _glr->glExt.wglReleaseVideoImageNV (hPbuffer, iVideoBuffer);}
inline BOOL WINAPI wglSendPbufferToVideoNV (HPBUFFERARB hPbuffer, int iBufferType, unsigned long *pulCounterPbuffer, BOOL bBlock) {
  return _glr->glExt.wglSendPbufferToVideoNV (hPbuffer, iBufferType, pulCounterPbuffer, bBlock);}
inline BOOL WINAPI wglGetVideoInfoNV (HPVIDEODEV hpVideoDevice, unsigned long *pulCounterOutputPbuffer, unsigned long *pulCounterOutputVideo) {
  return _glr->glExt.wglGetVideoInfoNV (hpVideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);}

// WGL_OML_sync_control
inline BOOL WINAPI wglGetSyncValuesOML (HDC hdc, INT64 *ust, INT64 *msc, INT64 *sbc) {
  return _glr->glExt.wglGetSyncValuesOML (hdc, ust, msc, sbc);}
inline BOOL WINAPI wglGetMscRateOML (HDC hdc, INT32 *numerator, INT32 *denominator) {
  return _glr->glExt.wglGetMscRateOML (hdc, numerator, denominator);}
inline INT64 WINAPI wglSwapBuffersMscOML (HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder) {
  return _glr->glExt.wglSwapBuffersMscOML (hdc, target_msc, divisor, remainder);}
inline INT64 WINAPI wglSwapLayerBuffersMscOML (HDC hdc, int fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder) {
  return _glr->glExt.wglSwapLayerBuffersMscOML (hdc, fuPlanes, target_msc, divisor, remainder);}
inline BOOL WINAPI wglWaitForMscOML (HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64 *ust, INT64 *msc, INT64 *sbc) {
  return _glr->glExt.wglWaitForMscOML (hdc, target_msc, divisor, remainder, ust, msc, sbc);}
inline BOOL WINAPI wglWaitForSbcOML (HDC hdc, INT64 target_sbc, INT64 *ust, INT64 *msc, INT64 *sbc) {
  return _glr->glExt.wglWaitForSbcOML (hdc, target_sbc, ust, msc, sbc); }

#endif /// OS_WIN







///==============///
// GLX extensions //
///==============///

#ifdef OS_LINUX
/*
// GLX_VERSION_1_3
inline GLXFBConfig *glXGetFBConfigs (Display *dpy, int screen, int *nelements) {
  return _glr->glExt.glXGetFBConfigs (dpy, screen, nelements);}
inline GLXFBConfig *glXChooseFBConfig (Display *dpy, int screen, const int *attrib_list, int *nelements) {
  return _glr->glExt.glXChooseFBConfig (dpy, screen, attrib_list, nelements);}
inline int glXGetFBConfigAttrib (Display *dpy, GLXFBConfig config, int attribute, int *value) {
  return _glr->glExt.glXGetFBConfigAttrib (dpy, config, attribute, value);}
inline XVisualInfo *glXGetVisualFromFBConfig (Display *dpy, GLXFBConfig config) {
  return _glr->glExt.glXGetVisualFromFBConfig (dpy, config);}
inline GLXWindow glXCreateWindow (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list) {
  return _glr->glExt.glXCreateWindow (dpy, config, win, attrib_list);}
inline void glXDestroyWindow (Display *dpy, GLXWindow win) {
  _glr->glExt.glXDestroyWindow (dpy, win);}
inline GLXPixmap glXCreatePixmap (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list) {
  return _glr->glExt.glXCreatePixmap (dpy, config, pixmap, attrib_list);}
inline void glXDestroyPixmap (Display *dpy, GLXPixmap pixmap) {
  _glr->glExt.glXDestroyPixmap (dpy, pixmap);}
inline GLXPbuffer glXCreatePbuffer (Display *dpy, GLXFBConfig config, const int *attrib_list) {
  return _glr->glExt.glXCreatePbuffer (dpy, config, attrib_list);}
inline void glXDestroyPbuffer (Display *dpy, GLXPbuffer pbuf) {
  _glr->glExt.glXDestroyPbuffer (dpy, pbuf);}
inline void glXQueryDrawable (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value) {
  _glr->glExt.glXQueryDrawable (dpy, draw, attribute, value);}
inline GLXContext glXCreateNewContext (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct) {
  return _glr->glExt.glXCreateNewContext (dpy, config, render_type, share_list, direct);}
inline Bool glXMakeContextCurrent (Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
  return _glr->glExt.glXMakeContextCurrent (dpy, draw, read, ctx);}
inline GLXDrawable glXGetCurrentReadDrawable (void) {
  return _glr->glExt.glXGetCurrentReadDrawable ();}
inline int glXQueryContext (Display *dpy, GLXContext ctx, int attribute, int *value) {
  return _glr->glExt.glXQueryContext (dpy, ctx, attribute, value);}
inline void glXSelectEvent (Display *dpy, GLXDrawable draw, unsigned long event_mask) {
  _glr->glExt.glXSelectEvent (dpy, draw, event_mask);}
inline void glXGetSelectedEvent (Display *dpy, GLXDrawable draw, unsigned long *event_mask) {
  _glr->glExt.glXGetSelectedEvent (dpy, draw, event_mask);}

// GLX_VERSION_1_4
//inline __GLXextFuncPtr glXGetProcAddress (const GLubyte *procName) {
  //return _glr->glExt.glXGetProcAddress (procName);}
*/

// GLX_ARB_create_context
inline GLXContext glXCreateContextAttribsARB (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list) {
  return _glr->glExt.glXCreateContextAttribsARB (dpy, config, share_context, direct, attrib_list);}

// GLX_ARB_get_proc_address
//inline __GLXextFuncPtr glXGetProcAddressARB (const GLubyte *procName) {
  //return _glr->glExt.glXGetProcAddressARB (procName);}

// GLX_EXT_import_context
inline Display *glXGetCurrentDisplayEXT (void) {
  return _glr->glExt.glXGetCurrentDisplayEXT ();}
inline int glXQueryContextInfoEXT (Display *dpy, GLXContext context, int attribute, int *value) {
  return _glr->glExt.glXQueryContextInfoEXT (dpy, context, attribute, value);}
inline GLXContextID glXGetContextIDEXT (const GLXContext context) {
  return _glr->glExt.glXGetContextIDEXT (context);}
inline GLXContext glXImportContextEXT (Display *dpy, GLXContextID contextID) {
  return _glr->glExt.glXImportContextEXT (dpy, contextID);}
inline void glXFreeContextEXT (Display *dpy, GLXContext context) {
  _glr->glExt.glXFreeContextEXT (dpy, context);}

// GLX_EXT_swap_control
inline void glXSwapIntervalEXT (Display *dpy, GLXDrawable drawable, int interval) {
  _glr->glExt.glXSwapIntervalEXT (dpy, drawable, interval);}

// GLX_EXT_texture_from_pixmap
inline void glXBindTexImageEXT (Display *dpy, GLXDrawable drawable, int buffer, const int *attrib_list) {
  _glr->glExt.glXBindTexImageEXT (dpy, drawable, buffer, attrib_list);}
inline void glXReleaseTexImageEXT (Display *dpy, GLXDrawable drawable, int buffer) {
  _glr->glExt.glXReleaseTexImageEXT (dpy, drawable, buffer);}

// GLX_MESA_agp_offset
inline unsigned int glXGetAGPOffsetMESA (const void *pointer) {
  return _glr->glExt.glXGetAGPOffsetMESA (pointer);}

// GLX_MESA_copy_sub_buffer
inline void glXCopySubBufferMESA (Display *dpy, GLXDrawable drawable, int x, int y, int width, int height) {
  _glr->glExt.glXCopySubBufferMESA (dpy, drawable, x, y, width, height);}

// GLX_MESA_pixmap_colormap
inline GLXPixmap glXCreateGLXPixmapMESA (Display *dpy, XVisualInfo *visual, Pixmap pixmap, Colormap cmap) {
  return _glr->glExt.glXCreateGLXPixmapMESA (dpy, visual, pixmap, cmap);}

// GLX_MESA_release_buffers
inline Bool glXReleaseBuffersMESA (Display *dpy, GLXDrawable drawable) {
  return _glr->glExt.glXReleaseBuffersMESA (dpy, drawable);}

// GLX_MESA_set_3dfx_mode
inline Bool glXSet3DfxModeMESA (int mode) {
  return _glr->glExt.glXSet3DfxModeMESA (mode);}

// GLX_NV_copy_image
inline void glXCopyImageSubDataNV (Display *dpy, GLXContext srcCtx, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLXContext dstCtx, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
  _glr->glExt.glXCopyImageSubDataNV (dpy, srcCtx, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstCtx, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);}

// GLX_NV_present_video
inline unsigned int *glXEnumerateVideoDevicesNV (Display *dpy, int screen, int *nelements) {
  return _glr->glExt.glXEnumerateVideoDevicesNV (dpy, screen, nelements);}
inline int glXBindVideoDeviceNV (Display *dpy, unsigned int video_slot, unsigned int video_device, const int *attrib_list) {
  return _glr->glExt.glXBindVideoDeviceNV (dpy, video_slot, video_device, attrib_list);}

// GLX_NV_swap_group
inline Bool glXJoinSwapGroupNV (Display *dpy, GLXDrawable drawable, GLuint group) {
  return _glr->glExt.glXJoinSwapGroupNV (dpy, drawable, group);}
inline Bool glXBindSwapBarrierNV (Display *dpy, GLuint group, GLuint barrier) {
  return _glr->glExt.glXBindSwapBarrierNV (dpy, group, barrier);}
inline Bool glXQuerySwapGroupNV (Display *dpy, GLXDrawable drawable, GLuint *group, GLuint *barrier) {
  return _glr->glExt.glXQuerySwapGroupNV (dpy, drawable, group, barrier);}
inline Bool glXQueryMaxSwapGroupsNV (Display *dpy, int screen, GLuint *maxGroups, GLuint *maxBarriers) {
  return _glr->glExt.glXQueryMaxSwapGroupsNV (dpy, screen, maxGroups, maxBarriers);}
inline Bool glXQueryFrameCountNV (Display *dpy, int screen, GLuint *count) {
  return _glr->glExt.glXQueryFrameCountNV (dpy, screen, count);}
inline Bool glXResetFrameCountNV (Display *dpy, int screen) {
  return _glr->glExt.glXResetFrameCountNV (dpy, screen);}

// GLX_NV_video_capture
inline int glXBindVideoCaptureDeviceNV (Display *dpy, unsigned int video_capture_slot, GLXVideoCaptureDeviceNV device) {
  return _glr->glExt.glXBindVideoCaptureDeviceNV (dpy, video_capture_slot, device);}
inline GLXVideoCaptureDeviceNV *glXEnumerateVideoCaptureDevicesNV (Display *dpy, int screen, int *nelements) {
  return _glr->glExt.glXEnumerateVideoCaptureDevicesNV (dpy, screen, nelements);}
inline void glXLockVideoCaptureDeviceNV (Display *dpy, GLXVideoCaptureDeviceNV device) {
  _glr->glExt.glXLockVideoCaptureDeviceNV (dpy, device);}
inline int glXQueryVideoCaptureDeviceNV (Display *dpy, GLXVideoCaptureDeviceNV device, int attribute, int *value) {
  return _glr->glExt.glXQueryVideoCaptureDeviceNV (dpy, device, attribute, value);}
inline void glXReleaseVideoCaptureDeviceNV (Display *dpy, GLXVideoCaptureDeviceNV device) {
  _glr->glExt.glXReleaseVideoCaptureDeviceNV (dpy, device);}

// GLX_NV_video_output
inline int glXGetVideoDeviceNV (Display *dpy, int screen, int numVideoDevices, GLXVideoDeviceNV *pVideoDevice) {
  return _glr->glExt.glXGetVideoDeviceNV (dpy, screen, numVideoDevices, pVideoDevice);}
inline int glXReleaseVideoDeviceNV (Display *dpy, int screen, GLXVideoDeviceNV VideoDevice) {
  return _glr->glExt.glXReleaseVideoDeviceNV (dpy, screen, VideoDevice);}
inline int glXBindVideoImageNV (Display *dpy, GLXVideoDeviceNV VideoDevice, GLXPbuffer pbuf, int iVideoBuffer) {
  return _glr->glExt.glXBindVideoImageNV (dpy, VideoDevice, pbuf, iVideoBuffer);}
inline int glXReleaseVideoImageNV (Display *dpy, GLXPbuffer pbuf) {
  return _glr->glExt.glXReleaseVideoImageNV (dpy, pbuf);}
inline int glXSendPbufferToVideoNV (Display *dpy, GLXPbuffer pbuf, int iBufferType, unsigned long *pulCounterPbuffer, GLboolean bBlock) {
  return _glr->glExt.glXSendPbufferToVideoNV (dpy, pbuf, iBufferType, pulCounterPbuffer, bBlock);}
inline int glXGetVideoInfoNV (Display *dpy, int screen, GLXVideoDeviceNV VideoDevice, unsigned long *pulCounterOutputPbuffer, unsigned long *pulCounterOutputVideo) {
  return _glr->glExt.glXGetVideoInfoNV (dpy, screen, VideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);}

// GLX_OML_sync_control
inline Bool glXGetSyncValuesOML (Display *dpy, GLXDrawable drawable, int64_t *ust, int64_t *msc, int64_t *sbc) {
  return _glr->glExt.glXGetSyncValuesOML (dpy, drawable, ust, msc, sbc);}
inline Bool glXGetMscRateOML (Display *dpy, GLXDrawable drawable, int32_t *numerator, int32_t *denominator) {
  return _glr->glExt.glXGetMscRateOML (dpy, drawable, numerator, denominator);}
inline int64_t glXSwapBuffersMscOML (Display *dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder) {
  return _glr->glExt.glXSwapBuffersMscOML (dpy, drawable, target_msc, divisor, remainder);}
inline Bool glXWaitForMscOML (Display *dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder, int64_t *ust, int64_t *msc, int64_t *sbc) {
  return _glr->glExt.glXWaitForMscOML (dpy, drawable, target_msc, divisor, remainder, ust, msc, sbc);}
inline Bool glXWaitForSbcOML (Display *dpy, GLXDrawable drawable, int64_t target_sbc, int64_t *ust, int64_t *msc, int64_t *sbc) {
  return _glr->glExt.glXWaitForSbcOML (dpy, drawable, target_sbc, ust, msc, sbc);}

// GLX_SGIX_dmbuffer
#ifdef _DM_BUFFER_H_
inline Bool glXAssociateDMPbufferSGIX (Display *dpy, GLXPbufferSGIX pbuffer, DMparams *params, DMbuffer dmbuffer) {
  return _glr->glExt.glXAssociateDMPbufferSGIX (dpy, pbuffer, params, dmbuffer);}
#endif // _DM_BUFFER_H included

// GLX_SGIX_fbconfig
inline int glXGetFBConfigAttribSGIX (Display *dpy, GLXFBConfigSGIX config, int attribute, int *value) {
  return _glr->glExt.glXGetFBConfigAttribSGIX (dpy, config, attribute, value);}
inline GLXFBConfigSGIX *glXChooseFBConfigSGIX (Display *dpy, int screen, int *attrib_list, int *nelements) {
  return _glr->glExt.glXChooseFBConfigSGIX (dpy, screen, attrib_list, nelements);}
inline GLXPixmap glXCreateGLXPixmapWithConfigSGIX (Display *dpy, GLXFBConfigSGIX config, Pixmap pixmap) {
  return _glr->glExt.glXCreateGLXPixmapWithConfigSGIX (dpy, config, pixmap);}
inline GLXContext glXCreateContextWithConfigSGIX (Display *dpy, GLXFBConfigSGIX config, int render_type, GLXContext share_list, Bool direct) {
  return _glr->glExt.glXCreateContextWithConfigSGIX (dpy, config, render_type, share_list, direct);}
inline XVisualInfo *glXGetVisualFromFBConfigSGIX (Display *dpy, GLXFBConfigSGIX config) {
  return _glr->glExt.glXGetVisualFromFBConfigSGIX (dpy, config);}
inline GLXFBConfigSGIX glXGetFBConfigFromVisualSGIX (Display *dpy, XVisualInfo *vis) {
  return _glr->glExt.glXGetFBConfigFromVisualSGIX (dpy, vis);}

// GLX_SGIX_hyperpipe
inline GLXHyperpipeNetworkSGIX *glXQueryHyperpipeNetworkSGIX (Display *dpy, int *npipes) {
  return _glr->glExt.glXQueryHyperpipeNetworkSGIX (dpy, npipes);}
inline int glXHyperpipeConfigSGIX (Display *dpy, int networkId, int npipes, GLXHyperpipeConfigSGIX *cfg, int *hpId) {
  return _glr->glExt.glXHyperpipeConfigSGIX (dpy, networkId, npipes, cfg, hpId);}
inline GLXHyperpipeConfigSGIX *glXQueryHyperpipeConfigSGIX (Display *dpy, int hpId, int *npipes) {
  return _glr->glExt.glXQueryHyperpipeConfigSGIX (dpy, hpId, npipes);}
inline int glXDestroyHyperpipeConfigSGIX (Display *dpy, int hpId) {
  return _glr->glExt.glXDestroyHyperpipeConfigSGIX (dpy, hpId);}
inline int glXBindHyperpipeSGIX (Display *dpy, int hpId) {
  return _glr->glExt.glXBindHyperpipeSGIX (dpy, hpId);}
inline int glXQueryHyperpipeBestAttribSGIX (Display *dpy, int timeSlice, int attrib, int size, void *attribList, void *returnAttribList) {
  return _glr->glExt.glXQueryHyperpipeBestAttribSGIX (dpy, timeSlice, attrib, size, attribList, returnAttribList);}
inline int glXHyperpipeAttribSGIX (Display *dpy, int timeSlice, int attrib, int size, void *attribList) {
  return _glr->glExt.glXHyperpipeAttribSGIX (dpy, timeSlice, attrib, size, attribList);}
inline int glXQueryHyperpipeAttribSGIX (Display *dpy, int timeSlice, int attrib, int size, void *returnAttribList) {
  return _glr->glExt.glXQueryHyperpipeAttribSGIX (dpy, timeSlice, attrib, size, returnAttribList);}

// GLX_SGIX_pbuffer
inline GLXPbufferSGIX glXCreateGLXPbufferSGIX (Display *dpy, GLXFBConfigSGIX config, unsigned int width, unsigned int height, int *attrib_list) {
  return _glr->glExt.glXCreateGLXPbufferSGIX (dpy, config, width, height, attrib_list);}
inline void glXDestroyGLXPbufferSGIX (Display *dpy, GLXPbufferSGIX pbuf) {
  _glr->glExt.glXDestroyGLXPbufferSGIX (dpy, pbuf);}
inline int glXQueryGLXPbufferSGIX (Display *dpy, GLXPbufferSGIX pbuf, int attribute, unsigned int *value) {
  return _glr->glExt.glXQueryGLXPbufferSGIX (dpy, pbuf, attribute, value);}
inline void glXSelectEventSGIX (Display *dpy, GLXDrawable drawable, unsigned long mask) {
  _glr->glExt.glXSelectEventSGIX (dpy, drawable, mask);}
inline void glXGetSelectedEventSGIX (Display *dpy, GLXDrawable drawable, unsigned long *mask) {
  _glr->glExt.glXGetSelectedEventSGIX (dpy, drawable, mask);}

// GLX_SGIX_swap_barrier
inline void glXBindSwapBarrierSGIX (Display *dpy, GLXDrawable drawable, int barrier) {
  _glr->glExt.glXBindSwapBarrierSGIX (dpy, drawable, barrier);}
inline Bool glXQueryMaxSwapBarriersSGIX (Display *dpy, int screen, int *max) {
  return _glr->glExt.glXQueryMaxSwapBarriersSGIX (dpy, screen, max);}

// GLX_SGIX_swap_group
inline void glXJoinSwapGroupSGIX (Display *dpy, GLXDrawable drawable, GLXDrawable member) {
  _glr->glExt.glXJoinSwapGroupSGIX (dpy, drawable, member);}

// GLX_SGIX_video_resize
inline int glXBindChannelToWindowSGIX (Display *display, int screen, int channel, Window window) {
  return _glr->glExt.glXBindChannelToWindowSGIX (display, screen, channel, window);}
inline int glXChannelRectSGIX (Display *display, int screen, int channel, int x, int y, int w, int h) {
  return _glr->glExt.glXChannelRectSGIX (display, screen, channel, x, y, w, h);}
inline int glXQueryChannelRectSGIX (Display *display, int screen, int channel, int *dx, int *dy, int *dw, int *dh) {
  return _glr->glExt.glXQueryChannelRectSGIX (display, screen, channel, dx, dy, dw, dh);}
inline int glXQueryChannelDeltasSGIX (Display *display, int screen, int channel, int *x, int *y, int *w, int *h) {
  return _glr->glExt.glXQueryChannelDeltasSGIX (display, screen, channel, x, y, w, h);}
inline int glXChannelRectSyncSGIX (Display *display, int screen, int channel, GLenum synctype) {
  return _glr->glExt.glXChannelRectSyncSGIX (display, screen, channel, synctype);}

// GLX_SGIX_video_source
#ifdef _VL_H
inline GLXVideoSourceSGIX glXCreateGLXVideoSourceSGIX (Display *display, int screen, VLServer server, VLPath path, int nodeClass, VLNode drainNode) {
  return _glr->glExt.glXCreateGLXVideoSourceSGIX (display, screen, server, path, nodeClass, drainNode);}
inline void glXDestroyGLXVideoSourceSGIX (Display *dpy, GLXVideoSourceSGIX glxvideosource) {
  _glr->glExt.glXDestroyGLXVideoSourceSGIX (dpy, glxvideosource);}
#endif /// _VL_H included

// GLX_SGI_cushion
inline void glXCushionSGI (Display *dpy, Window window, float cushion) {
  _glr->glExt.glXCushionSGI (dpy, window, cushion);}

// GLX_SGI_make_current_read
inline Bool glXMakeCurrentReadSGI (Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
  return _glr->glExt.glXMakeCurrentReadSGI (dpy, draw, read, ctx);}
inline GLXDrawable glXGetCurrentReadDrawableSGI (void) {
  return _glr->glExt.glXGetCurrentReadDrawableSGI ();}

// GLX_SGI_swap_control
inline int glXSwapIntervalSGI (int interval) {
  return _glr->glExt.glXSwapIntervalSGI (interval);}

// GLX_SGI_video_sync
inline int glXGetVideoSyncSGI (unsigned int *count) {
  return _glr->glExt.glXGetVideoSyncSGI (count);}
inline int glXWaitVideoSyncSGI (int divisor, int remainder, unsigned int *count) {
  return _glr->glExt.glXWaitVideoSyncSGI (divisor, remainder, count);}

// GLX_SUN_get_transparent_index
inline Status glXGetTransparentIndexSUN (Display *dpy, Window overlay, Window underlay, long *pTransparentIndex) {
  return _glr->glExt.glXGetTransparentIndexSUN (dpy, overlay, underlay, pTransparentIndex);}

#endif /// OS_LINUX








///=======================///
// EXT & vendor extensions //
///=======================///


// #291 GL_OES_byte_coordinates
inline GLAPI void APIENTRY glMultiTexCoord1bOES (GLenum texture, GLbyte s) {
  _glr->glExt.glMultiTexCoord1bOES (texture, s);}
inline GLAPI void APIENTRY glMultiTexCoord1bvOES (GLenum texture, const GLbyte *coords) {
  _glr->glExt.glMultiTexCoord1bvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord2bOES (GLenum texture, GLbyte s, GLbyte t) {
  _glr->glExt.glMultiTexCoord2bOES (texture, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2bvOES (GLenum texture, const GLbyte *coords) {
  _glr->glExt.glMultiTexCoord2bvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord3bOES (GLenum texture, GLbyte s, GLbyte t, GLbyte r) {
  _glr->glExt.glMultiTexCoord3bOES (texture, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3bvOES (GLenum texture, const GLbyte *coords) {
  _glr->glExt.glMultiTexCoord3bvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord4bOES (GLenum texture, GLbyte s, GLbyte t, GLbyte r, GLbyte q) {
  _glr->glExt.glMultiTexCoord4bOES (texture, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4bvOES (GLenum texture, const GLbyte *coords) {
  _glr->glExt.glMultiTexCoord4bvOES (texture, coords);}
inline GLAPI void APIENTRY glTexCoord1bOES (GLbyte s) {
  _glr->glExt.glTexCoord1bOES (s);}
inline GLAPI void APIENTRY glTexCoord1bvOES (const GLbyte *coords) {
  _glr->glExt.glTexCoord1bvOES (coords);}
inline GLAPI void APIENTRY glTexCoord2bOES (GLbyte s, GLbyte t) {
  _glr->glExt.glTexCoord2bOES (s, t);}
inline GLAPI void APIENTRY glTexCoord2bvOES (const GLbyte *coords) {
  _glr->glExt.glTexCoord2bvOES (coords);}
inline GLAPI void APIENTRY glTexCoord3bOES (GLbyte s, GLbyte t, GLbyte r) {
  _glr->glExt.glTexCoord3bOES (s, t, r);}
inline GLAPI void APIENTRY glTexCoord3bvOES (const GLbyte *coords) {
  _glr->glExt.glTexCoord3bvOES (coords);}
inline GLAPI void APIENTRY glTexCoord4bOES (GLbyte s, GLbyte t, GLbyte r, GLbyte q) {
  _glr->glExt.glTexCoord4bOES (s, t, r,  q);}
inline GLAPI void APIENTRY glTexCoord4bvOES (const GLbyte *coords) {
  _glr->glExt.glTexCoord4bvOES (coords);}
inline GLAPI void APIENTRY glVertex2bOES (GLbyte x) {
  _glr->glExt.glVertex2bOES (x);}
inline GLAPI void APIENTRY glVertex2bvOES (const GLbyte *coords) {
  _glr->glExt.glVertex2bvOES (coords);}
inline GLAPI void APIENTRY glVertex3bOES (GLbyte x, GLbyte y) {
  _glr->glExt.glVertex3bOES (x, y);}
inline GLAPI void APIENTRY glVertex3bvOES (const GLbyte *coords) {
  _glr->glExt.glVertex3bvOES (coords);}
inline GLAPI void APIENTRY glVertex4bOES (GLbyte x, GLbyte y, GLbyte z) {
  _glr->glExt.glVertex4bOES (x, y, z);}
inline GLAPI void APIENTRY glVertex4bvOES (const GLbyte *coords) {
  _glr->glExt.glVertex4bvOES (coords);}
// #292 GL_OES_fixed_point
inline GLAPI void APIENTRY glAlphaFuncxOES (GLenum func, GLfixed ref) {
  _glr->glExt.glAlphaFuncxOES (func, ref);}
inline GLAPI void APIENTRY glClearColorxOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  _glr->glExt.glClearColorxOES (red, green, blue, alpha);}
inline GLAPI void APIENTRY glClearDepthxOES (GLfixed depth) {
  _glr->glExt.glClearDepthxOES (depth);}
inline GLAPI void APIENTRY glClipPlanexOES (GLenum plane, const GLfixed *equation) {
  _glr->glExt.glClipPlanexOES (plane, equation);}
inline GLAPI void APIENTRY glColor4xOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  _glr->glExt.glColor4xOES (red, green, blue, alpha);}
inline GLAPI void APIENTRY glDepthRangexOES (GLfixed n, GLfixed f) {
  _glr->glExt.glDepthRangexOES (n, f);}
inline GLAPI void APIENTRY glFogxOES (GLenum pname, GLfixed param) {
  _glr->glExt.glFogxOES (pname, param);}
inline GLAPI void APIENTRY glFogxvOES (GLenum pname, const GLfixed *param) {
  _glr->glExt.glFogxvOES (pname, param);}
inline GLAPI void APIENTRY glFrustumxOES (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f) {
  _glr->glExt.glFrustumxOES (l, r, b, t, n, f);}
inline GLAPI void APIENTRY glGetClipPlanexOES (GLenum plane, GLfixed *equation) {
  _glr->glExt.glGetClipPlanexOES (plane, equation);}
inline GLAPI void APIENTRY glGetFixedvOES (GLenum pname, GLfixed *params) {
  _glr->glExt.glGetFixedvOES (pname, params);}
inline GLAPI void APIENTRY glGetTexEnvxvOES (GLenum target, GLenum pname, GLfixed *params) {
  _glr->glExt.glGetTexEnvxvOES (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterxvOES (GLenum target, GLenum pname, GLfixed *params) {
  _glr->glExt.glGetTexParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glLightModelxOES (GLenum pname, GLfixed param) {
  _glr->glExt.glLightModelxOES (pname, param);}
inline GLAPI void APIENTRY glLightModelxvOES (GLenum pname, const GLfixed *param) {
  _glr->glExt.glLightModelxvOES (pname, param);}
inline GLAPI void APIENTRY glLightxOES (GLenum light, GLenum pname, GLfixed param) {
  _glr->glExt.glLightxOES (light, pname, param);}
inline GLAPI void APIENTRY glLightxvOES (GLenum light, GLenum pname, const GLfixed *params) {
  _glr->glExt.glLightxvOES (light, pname, params);}
inline GLAPI void APIENTRY glLineWidthxOES (GLfixed width) {
  _glr->glExt.glLineWidthxOES (width);}
inline GLAPI void APIENTRY glLoadMatrixxOES (const GLfixed *m) {
  _glr->glExt.glLoadMatrixxOES (m);}
inline GLAPI void APIENTRY glMaterialxOES (GLenum face, GLenum pname, GLfixed param) {
  _glr->glExt.glMaterialxOES (face, pname, param);}
inline GLAPI void APIENTRY glMaterialxvOES (GLenum face, GLenum pname, const GLfixed *param) {
  _glr->glExt.glMaterialxvOES (face, pname, param);}
inline GLAPI void APIENTRY glMultMatrixxOES (const GLfixed *m) {
  _glr->glExt.glMultMatrixxOES (m);}
inline GLAPI void APIENTRY glMultiTexCoord4xOES (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q) {
  _glr->glExt.glMultiTexCoord4xOES (texture, s, t, r, q);}
inline GLAPI void APIENTRY glNormal3xOES (GLfixed nx, GLfixed ny, GLfixed nz) {
  _glr->glExt.glNormal3xOES (nx, ny, nz);}
inline GLAPI void APIENTRY glOrthoxOES (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f) {
  _glr->glExt.glOrthoxOES (l, r, b, t, n, f);}
inline GLAPI void APIENTRY glPointParameterxvOES (GLenum pname, const GLfixed *params) {
  _glr->glExt.glPointParameterxvOES (pname, params);}
inline GLAPI void APIENTRY glPointSizexOES (GLfixed size) {
  _glr->glExt.glPointSizexOES (size);}
inline GLAPI void APIENTRY glPolygonOffsetxOES (GLfixed factor, GLfixed units) {
  _glr->glExt.glPolygonOffsetxOES (factor, units);}
inline GLAPI void APIENTRY glRotatexOES (GLfixed angle, GLfixed x, GLfixed y, GLfixed z) {
  _glr->glExt.glRotatexOES (angle, x, y, z);}
inline GLAPI void APIENTRY glSampleCoverageOES (GLfixed value, GLboolean invert) { 
  _glr->glExt.glSampleCoverageOES (value, invert);}
inline GLAPI void APIENTRY glScalexOES (GLfixed x, GLfixed y, GLfixed z) {
  _glr->glExt.glScalexOES (x, y, z);}
inline GLAPI void APIENTRY glTexEnvxOES (GLenum target, GLenum pname, GLfixed param) {
  _glr->glExt.glTexEnvxOES (target, pname, param);}
inline GLAPI void APIENTRY glTexEnvxvOES (GLenum target, GLenum pname, const GLfixed *params) {
  _glr->glExt.glTexEnvxvOES (target, pname, params);}
inline GLAPI void APIENTRY glTexParameterxOES (GLenum target, GLenum pname, GLfixed param) {
  _glr->glExt.glTexParameterxOES (target, pname, param);}
inline GLAPI void APIENTRY glTexParameterxvOES (GLenum target, GLenum pname, const GLfixed *params) {
  _glr->glExt.glTexParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glTranslatexOES (GLfixed x, GLfixed y, GLfixed z) {
  _glr->glExt.glTranslatexOES (x, y, z);}
inline GLAPI void APIENTRY glAccumxOES (GLenum op, GLfixed value) {
  _glr->glExt.glAccumxOES (op, value);}
inline GLAPI void APIENTRY glBitmapxOES (GLsizei width, GLsizei height, GLfixed xorig, GLfixed yorig, GLfixed xmove, GLfixed ymove, const GLubyte *bitmap) {
  _glr->glExt.glBitmapxOES (width, height, xorig, yorig, xmove, ymove, bitmap);}
inline GLAPI void APIENTRY glBlendColorxOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  _glr->glExt.glBlendColorxOES (red, green, blue, alpha);}
inline GLAPI void APIENTRY glClearAccumxOES (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {
  _glr->glExt.glClearAccumxOES (red, green, blue,  alpha);}
inline GLAPI void APIENTRY glColor3xOES (GLfixed red, GLfixed green, GLfixed blue) {
  _glr->glExt.glColor3xOES (red, green,  blue);}
inline GLAPI void APIENTRY glColor3xvOES (const GLfixed *components) {
  _glr->glExt.glColor3xvOES (components);}
inline GLAPI void APIENTRY glColor4xvOES (const GLfixed *components) {
  _glr->glExt.glColor4xvOES (components);}
inline GLAPI void APIENTRY glConvolutionParameterxOES (GLenum target, GLenum pname, GLfixed param) {
  _glr->glExt.glConvolutionParameterxOES (target, pname,  param);}
inline GLAPI void APIENTRY glConvolutionParameterxvOES (GLenum target, GLenum pname, const GLfixed *params) {
  _glr->glExt.glConvolutionParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glEvalCoord1xOES (GLfixed u) {
  _glr->glExt.glEvalCoord1xOES (u);}
inline GLAPI void APIENTRY glEvalCoord1xvOES (const GLfixed *coords) {
  _glr->glExt.glEvalCoord1xvOES (coords);}
inline GLAPI void APIENTRY glEvalCoord2xOES (GLfixed u, GLfixed v) {
  _glr->glExt.glEvalCoord2xOES (u, v);}
inline GLAPI void APIENTRY glEvalCoord2xvOES (const GLfixed *coords) {
  _glr->glExt.glEvalCoord2xvOES (coords);}
inline GLAPI void APIENTRY glFeedbackBufferxOES (GLsizei n, GLenum type, const GLfixed *buffer) {
  _glr->glExt.glFeedbackBufferxOES (n, type, buffer);}
inline GLAPI void APIENTRY glGetConvolutionParameterxvOES (GLenum target, GLenum pname, GLfixed *params) {
  _glr->glExt.glGetConvolutionParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glGetHistogramParameterxvOES (GLenum target, GLenum pname, GLfixed *params) {
  _glr->glExt.glGetHistogramParameterxvOES (target, pname, params);}
inline GLAPI void APIENTRY glGetLightxOES (GLenum light, GLenum pname, GLfixed *params) {
  _glr->glExt.glGetLightxOES (light, pname, params);}
inline GLAPI void APIENTRY glGetMapxvOES (GLenum target, GLenum query, GLfixed *v) {
  _glr->glExt.glGetMapxvOES (target, query, v);}
inline GLAPI void APIENTRY glGetMaterialxOES (GLenum face, GLenum pname, GLfixed param) {
  _glr->glExt.glGetMaterialxOES (face, pname, param);}
inline GLAPI void APIENTRY glGetPixelMapxv (GLenum map, GLint size, GLfixed *values) {
  _glr->glExt.glGetPixelMapxv (map, size, values);}
inline GLAPI void APIENTRY glGetTexGenxvOES (GLenum coord, GLenum pname, GLfixed *params) {
  _glr->glExt.glGetTexGenxvOES (coord, pname, params);}
inline GLAPI void APIENTRY glGetTexLevelParameterxvOES (GLenum target, GLint level, GLenum pname, GLfixed *params) {
  _glr->glExt.glGetTexLevelParameterxvOES (target, level, pname, params);}
inline GLAPI void APIENTRY glIndexxOES (GLfixed component) {
  _glr->glExt.glIndexxOES (component);}
inline GLAPI void APIENTRY glIndexxvOES (const GLfixed *component) {
  _glr->glExt.glIndexxvOES (component);}
inline GLAPI void APIENTRY glLoadTransposeMatrixxOES (const GLfixed *m) {
  _glr->glExt.glLoadTransposeMatrixxOES (m);}
inline GLAPI void APIENTRY glMap1xOES (GLenum target, GLfixed u1, GLfixed u2, GLint stride, GLint order, GLfixed points) {
  _glr->glExt.glMap1xOES (target, u1, u2, stride, order, points);}
inline GLAPI void APIENTRY glMap2xOES (GLenum target, GLfixed u1, GLfixed u2, GLint ustride, GLint uorder, GLfixed v1, GLfixed v2, GLint vstride, GLint vorder, GLfixed points) {
  _glr->glExt.glMap2xOES (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
inline GLAPI void APIENTRY glMapGrid1xOES (GLint n, GLfixed u1, GLfixed u2) {
  _glr->glExt.glMapGrid1xOES (n, u1, u2);}
inline GLAPI void APIENTRY glMapGrid2xOES (GLint n, GLfixed u1, GLfixed u2, GLfixed v1, GLfixed v2) {
  _glr->glExt.glMapGrid2xOES (n, u1, u2, v1, v2);}
inline GLAPI void APIENTRY glMultTransposeMatrixxOES (const GLfixed *m) {
  _glr->glExt.glMultTransposeMatrixxOES (m);}
inline GLAPI void APIENTRY glMultiTexCoord1xOES (GLenum texture, GLfixed s) {
  _glr->glExt.glMultiTexCoord1xOES (texture, s);}
inline GLAPI void APIENTRY glMultiTexCoord1xvOES (GLenum texture, const GLfixed *coords) {
  _glr->glExt.glMultiTexCoord1xvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord2xOES (GLenum texture, GLfixed s, GLfixed t) {
  _glr->glExt.glMultiTexCoord2xOES (texture, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2xvOES (GLenum texture, const GLfixed *coords) {
  _glr->glExt.glMultiTexCoord2xvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord3xOES (GLenum texture, GLfixed s, GLfixed t, GLfixed r) {
  _glr->glExt.glMultiTexCoord3xOES (texture, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3xvOES (GLenum texture, const GLfixed *coords) {
  _glr->glExt.glMultiTexCoord3xvOES (texture, coords);}
inline GLAPI void APIENTRY glMultiTexCoord4xvOES (GLenum texture, const GLfixed *coords) {
  _glr->glExt.glMultiTexCoord4xvOES (texture, coords);}
inline GLAPI void APIENTRY glNormal3xvOES (const GLfixed *coords) {
  _glr->glExt.glNormal3xvOES (coords);}
inline GLAPI void APIENTRY glPassThroughxOES (GLfixed token) {
  _glr->glExt.glPassThroughxOES (token);}
inline GLAPI void APIENTRY glPixelMapx (GLenum map, GLint size, const GLfixed *values) {
  _glr->glExt.glPixelMapx (map, size, values);}
inline GLAPI void APIENTRY glPixelStorex (GLenum pname, GLfixed param) {
  _glr->glExt.glPixelStorex (pname, param);}
inline GLAPI void APIENTRY glPixelTransferxOES (GLenum pname, GLfixed param) {
  _glr->glExt.glPixelTransferxOES (pname, param);}
inline GLAPI void APIENTRY glPixelZoomxOES (GLfixed xfactor, GLfixed yfactor) {
  _glr->glExt.glPixelZoomxOES (xfactor, yfactor);}
inline GLAPI void APIENTRY glPrioritizeTexturesxOES (GLsizei n, const GLuint *textures, const GLfixed *priorities) {
  _glr->glExt.glPrioritizeTexturesxOES (n, textures, priorities);}
inline GLAPI void APIENTRY glRasterPos2xOES (GLfixed x, GLfixed y) {
  _glr->glExt.glRasterPos2xOES (x, y);}
inline GLAPI void APIENTRY glRasterPos2xvOES (const GLfixed *coords) {
  _glr->glExt.glRasterPos2xvOES (coords);}
inline GLAPI void APIENTRY glRasterPos3xOES (GLfixed x, GLfixed y, GLfixed z) {
  _glr->glExt.glRasterPos3xOES (x, y, z);}
inline GLAPI void APIENTRY glRasterPos3xvOES (const GLfixed *coords) {
  _glr->glExt.glRasterPos3xvOES (coords);}
inline GLAPI void APIENTRY glRasterPos4xOES (GLfixed x, GLfixed y, GLfixed z, GLfixed w) {
  _glr->glExt.glRasterPos4xOES (x, y, z, w);}
inline GLAPI void APIENTRY glRasterPos4xvOES (const GLfixed *coords) {
  _glr->glExt.glRasterPos4xvOES (coords);}
inline GLAPI void APIENTRY glRectxOES (GLfixed x1, GLfixed y1, GLfixed x2, GLfixed y2) {
  _glr->glExt.glRectxOES (x1, y1, x2, y2);}
inline GLAPI void APIENTRY glRectxvOES (const GLfixed *v1, const GLfixed *v2) {
  _glr->glExt.glRectxvOES (v1, v2);}
inline GLAPI void APIENTRY glTexCoord1xOES (GLfixed s) {
  _glr->glExt.glTexCoord1xOES (s);}
inline GLAPI void APIENTRY glTexCoord1xvOES (const GLfixed *coords) {
  _glr->glExt.glTexCoord1xvOES (coords);}
inline GLAPI void APIENTRY glTexCoord2xOES (GLfixed s, GLfixed t) {
  _glr->glExt.glTexCoord2xOES (s, t);}
inline GLAPI void APIENTRY glTexCoord2xvOES (const GLfixed *coords) {
  _glr->glExt.glTexCoord2xvOES (coords);}
inline GLAPI void APIENTRY glTexCoord3xOES (GLfixed s, GLfixed t, GLfixed r) {
  _glr->glExt.glTexCoord3xOES (s, t, r);}
inline GLAPI void APIENTRY glTexCoord3xvOES (const GLfixed *coords) {
  _glr->glExt.glTexCoord3xvOES (coords);}
inline GLAPI void APIENTRY glTexCoord4xOES (GLfixed s, GLfixed t, GLfixed r, GLfixed q) {
  _glr->glExt.glTexCoord4xOES (s, t, r, q);}
inline GLAPI void APIENTRY glTexCoord4xvOES (const GLfixed *coords) {
  _glr->glExt.glTexCoord4xvOES (coords);}
inline GLAPI void APIENTRY glTexGenxOES (GLenum coord, GLenum pname, GLfixed param) {
  _glr->glExt.glTexGenxOES (coord, pname,  param);}
inline GLAPI void APIENTRY glTexGenxvOES (GLenum coord, GLenum pname, const GLfixed *params) {
  _glr->glExt.glTexGenxvOES (coord, pname, params);}
inline GLAPI void APIENTRY glVertex2xOES (GLfixed x) {
  _glr->glExt.glVertex2xOES (x);}
inline GLAPI void APIENTRY glVertex2xvOES (const GLfixed *coords) {
  _glr->glExt.glVertex2xvOES (coords);}
inline GLAPI void APIENTRY glVertex3xOES (GLfixed x, GLfixed y) {
  _glr->glExt.glVertex3xOES (x, y);}
inline GLAPI void APIENTRY glVertex3xvOES (const GLfixed *coords) {
  _glr->glExt.glVertex3xvOES (coords);}
inline GLAPI void APIENTRY glVertex4xOES (GLfixed x, GLfixed y, GLfixed z) {
  _glr->glExt.glVertex4xOES (x, y, z);}
inline GLAPI void APIENTRY glVertex4xvOES (const GLfixed *coords) {
  _glr->glExt.glVertex4xvOES (coords);}
// #296 GL_OES_query_matrix
inline GLAPI GLbitfield APIENTRY glQueryMatrixxOES (GLfixed *mantissa, GLint *exponent) {
  return _glr->glExt.glQueryMatrixxOES (mantissa, exponent);}
// #293 GL_OES_single_precision
inline GLAPI void APIENTRY glClearDepthfOES (GLclampf depth) {
  _glr->glExt.glClearDepthfOES (depth);}
inline GLAPI void APIENTRY glClipPlanefOES (GLenum plane, const GLfloat *equation) {
  _glr->glExt.glClipPlanefOES (plane, equation);}
inline GLAPI void APIENTRY glDepthRangefOES (GLclampf n, GLclampf f) {
  _glr->glExt.glDepthRangefOES (n, f);}
inline GLAPI void APIENTRY glFrustumfOES (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f) {
  _glr->glExt.glFrustumfOES (l, r, b, t, n, f);}
inline GLAPI void APIENTRY glGetClipPlanefOES (GLenum plane, GLfloat *equation) {
  _glr->glExt.glGetClipPlanefOES (plane, equation);}
inline GLAPI void APIENTRY glOrthofOES (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f) {
  _glr->glExt.glOrthofOES (l, r, b, t, n, f);}
// #208 GL_3DFX_tbuffer
inline GLAPI void APIENTRY glTbufferMask3DFX (GLuint mask) {
  _glr->glExt.glTbufferMask3DFX (mask);}
// #395 GL_AMD_debug_output
inline GLAPI void APIENTRY glDebugMessageEnableAMD (GLenum category, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) {
  _glr->glExt.glDebugMessageEnableAMD (category, severity, count, ids, enabled);}
inline GLAPI void APIENTRY glDebugMessageInsertAMD (GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar *buf) {
  _glr->glExt.glDebugMessageInsertAMD (category, severity, id, length, buf);}
inline GLAPI void APIENTRY glDebugMessageCallbackAMD (GLDEBUGPROCAMD callback, void *userParam) {
  _glr->glExt.glDebugMessageCallbackAMD (callback, userParam);}
inline GLAPI GLuint APIENTRY glGetDebugMessageLogAMD (GLuint count, GLsizei bufsize, GLenum *categories, GLuint *severities, GLuint *ids, GLsizei *lengths, GLchar *message) {
  return _glr->glExt.glGetDebugMessageLogAMD (count, bufsize, categories, severities, ids, lengths, message);}
// #366 GL_AMD_draw_buffers_blend
inline GLAPI void APIENTRY glBlendFuncIndexedAMD (GLuint buf, GLenum src, GLenum dst) {
  _glr->glExt.glBlendFuncIndexedAMD (buf, src, dst);}
inline GLAPI void APIENTRY glBlendFuncSeparateIndexedAMD (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  _glr->glExt.glBlendFuncSeparateIndexedAMD (buf, srcRGB, dstRGB, srcAlpha, dstAlpha);}
inline GLAPI void APIENTRY glBlendEquationIndexedAMD (GLuint buf, GLenum mode) {
  _glr->glExt.glBlendEquationIndexedAMD (buf, mode);}
inline GLAPI void APIENTRY glBlendEquationSeparateIndexedAMD (GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
  _glr->glExt.glBlendEquationSeparateIndexedAMD (buf, modeRGB, modeAlpha);}
// #451 GL_AMD_gpu_shader_int64
inline GLAPI void APIENTRY glUniform1i64NV (GLint location, GLint64EXT x) {
  _glr->glExt.glUniform1i64NV (location, x);}
inline GLAPI void APIENTRY glUniform2i64NV (GLint location, GLint64EXT x, GLint64EXT y) {
  _glr->glExt.glUniform2i64NV (location, x, y);}
inline GLAPI void APIENTRY glUniform3i64NV (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z) {
  _glr->glExt.glUniform3i64NV (location, x, y, z);}
inline GLAPI void APIENTRY glUniform4i64NV (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w) {
  _glr->glExt.glUniform4i64NV (location, x, y, z, w);}
inline GLAPI void APIENTRY glUniform1i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glUniform1i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform2i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glUniform2i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform3i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glUniform3i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform4i64vNV (GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glUniform4i64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform1ui64NV (GLint location, GLuint64EXT x) {
  _glr->glExt.glUniform1ui64NV (location, x);}
inline GLAPI void APIENTRY glUniform2ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y) {
  _glr->glExt.glUniform2ui64NV (location, x, y);}
inline GLAPI void APIENTRY glUniform3ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z) {
  _glr->glExt.glUniform3ui64NV (location, x, y, z);}
inline GLAPI void APIENTRY glUniform4ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w) {
  _glr->glExt.glUniform4ui64NV (location, x, y, z, w);}
inline GLAPI void APIENTRY glUniform1ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glUniform1ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform2ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glUniform2ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform3ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glUniform3ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glUniform4ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glUniform4ui64vNV (location, count, value);}
inline GLAPI void APIENTRY glGetUniformi64vNV (GLuint program, GLint location, GLint64EXT *params) {
  _glr->glExt.glGetUniformi64vNV (program, location, params);}
inline GLAPI void APIENTRY glGetUniformui64vNV (GLuint program, GLint location, GLuint64EXT *params) {
  _glr->glExt.glGetUniformui64vNV (program, location, params);}
inline GLAPI void APIENTRY glProgramUniform1i64NV (GLuint program, GLint location, GLint64EXT x) {
  _glr->glExt.glProgramUniform1i64NV (program, location, x);}
inline GLAPI void APIENTRY glProgramUniform2i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y) {
  _glr->glExt.glProgramUniform2i64NV (program, location, x, y);}
inline GLAPI void APIENTRY glProgramUniform3i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z) {
  _glr->glExt.glProgramUniform3i64NV (program, location,  x, y, z);}
inline GLAPI void APIENTRY glProgramUniform4i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w) {
  _glr->glExt.glProgramUniform4i64NV (program, location, x, y, z, w);}
inline GLAPI void APIENTRY glProgramUniform1i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glProgramUniform1i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glProgramUniform2i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glProgramUniform3i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value) {
  _glr->glExt.glProgramUniform4i64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1ui64NV (GLuint program, GLint location, GLuint64EXT x) {
  _glr->glExt.glProgramUniform1ui64NV (program, location, x);}
inline GLAPI void APIENTRY glProgramUniform2ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y) {
  _glr->glExt.glProgramUniform2ui64NV (program, location, x, y);}
inline GLAPI void APIENTRY glProgramUniform3ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z) {
  _glr->glExt.glProgramUniform3ui64NV (program, location, x, y, z);}
inline GLAPI void APIENTRY glProgramUniform4ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w) {
  _glr->glExt.glProgramUniform4ui64NV (program, location, x, y, z, w);}
inline GLAPI void APIENTRY glProgramUniform1ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glProgramUniform1ui64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glProgramUniform2ui64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glProgramUniform3ui64vNV (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glProgramUniform4ui64vNV (program, location, count, value);}
// #431 GL_AMD_interleaved_elements
inline GLAPI void APIENTRY glVertexAttribParameteriAMD (GLuint index, GLenum pname, GLint param) {
  _glr->glExt.glVertexAttribParameteriAMD (index, pname, param);}
// #408 GL_AMD_multi_draw_indirect
inline GLAPI void APIENTRY glMultiDrawArraysIndirectAMD (GLenum mode, const void *indirect, GLsizei primcount, GLsizei stride) {
  _glr->glExt.glMultiDrawArraysIndirectAMD (mode, indirect, primcount, stride);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirectAMD (GLenum mode, GLenum type, const void *indirect, GLsizei primcount, GLsizei stride) {
  _glr->glExt.glMultiDrawElementsIndirectAMD (mode, type, indirect, primcount, stride);}
// #394 GL_AMD_name_gen_delete
inline GLAPI void APIENTRY glGenNamesAMD (GLenum identifier, GLuint num, GLuint *names) {
  _glr->glExt.glGenNamesAMD (identifier, num, names);}
inline GLAPI void APIENTRY glDeleteNamesAMD (GLenum identifier, GLuint num, const GLuint *names) {
  _glr->glExt.glDeleteNamesAMD (identifier, num, names);}
inline GLAPI GLboolean APIENTRY glIsNameAMD (GLenum identifier, GLuint name) {
  return _glr->glExt.glIsNameAMD (identifier, name);}
// #442 GL_AMD_occlusion_query_event
inline GLAPI void APIENTRY glQueryObjectParameteruiAMD (GLenum target, GLuint id, GLenum pname, GLuint param) {
  _glr->glExt.glQueryObjectParameteruiAMD (target, id, pname, param);}
// #360 GL_AMD_performance_monitor
inline GLAPI void APIENTRY glGetPerfMonitorGroupsAMD (GLint *numGroups, GLsizei groupsSize, GLuint *groups) {
  _glr->glExt.glGetPerfMonitorGroupsAMD (numGroups, groupsSize, groups);}
inline GLAPI void APIENTRY glGetPerfMonitorCountersAMD (GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei counterSize, GLuint *counters) {
  _glr->glExt.glGetPerfMonitorCountersAMD (group, numCounters, maxActiveCounters, counterSize, counters);}
inline GLAPI void APIENTRY glGetPerfMonitorGroupStringAMD (GLuint group, GLsizei bufSize, GLsizei *length, GLchar *groupString) {
  _glr->glExt.glGetPerfMonitorGroupStringAMD (group, bufSize, length, groupString);}
inline GLAPI void APIENTRY glGetPerfMonitorCounterStringAMD (GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, GLchar *counterString) {
  _glr->glExt.glGetPerfMonitorCounterStringAMD (group, counter, bufSize, length, counterString);}
inline GLAPI void APIENTRY glGetPerfMonitorCounterInfoAMD (GLuint group, GLuint counter, GLenum pname, void *data) {
  _glr->glExt.glGetPerfMonitorCounterInfoAMD (group, counter, pname, data);}
inline GLAPI void APIENTRY glGenPerfMonitorsAMD (GLsizei n, GLuint *monitors) {
  _glr->glExt.glGenPerfMonitorsAMD (n, monitors);}
inline GLAPI void APIENTRY glDeletePerfMonitorsAMD (GLsizei n, GLuint *monitors) {
  _glr->glExt.glDeletePerfMonitorsAMD (n, monitors);}
inline GLAPI void APIENTRY glSelectPerfMonitorCountersAMD (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *counterList) {
  _glr->glExt.glSelectPerfMonitorCountersAMD (monitor, enable, group, numCounters, counterList);}
inline GLAPI void APIENTRY glBeginPerfMonitorAMD (GLuint monitor) {
  _glr->glExt.glBeginPerfMonitorAMD (monitor);}
inline GLAPI void APIENTRY glEndPerfMonitorAMD (GLuint monitor) {
  _glr->glExt.glEndPerfMonitorAMD (monitor);}
inline GLAPI void APIENTRY glGetPerfMonitorCounterDataAMD (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten) {
  _glr->glExt.glGetPerfMonitorCounterDataAMD (monitor, pname, dataSize, data, bytesWritten);}
// #405 GL_AMD_sample_positions
inline GLAPI void APIENTRY glSetMultisamplefvAMD (GLenum pname, GLuint index, const GLfloat *val) {
  _glr->glExt.glSetMultisamplefvAMD (pname, index, val);}
// #426 GL_AMD_sparse_texture
inline GLAPI void APIENTRY glTexStorageSparseAMD (GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags) {
  _glr->glExt.glTexStorageSparseAMD (target, internalFormat, width, height, depth, layers, flags);}
inline GLAPI void APIENTRY glTextureStorageSparseAMD (GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags) {
  _glr->glExt.glTextureStorageSparseAMD (texture, target, internalFormat, width, height, depth, layers, flags);}
// #413 GL_AMD_stencil_operation_extended
inline GLAPI void APIENTRY glStencilOpValueAMD (GLenum face, GLuint value) {
  _glr->glExt.glStencilOpValueAMD (face, value);}
// #363 GL_AMD_vertex_shader_tessellator
inline GLAPI void APIENTRY glTessellationFactorAMD (GLfloat factor) {
  _glr->glExt.glTessellationFactorAMD (factor);}
inline GLAPI void APIENTRY glTessellationModeAMD (GLenum mode) {
  _glr->glExt.glTessellationModeAMD (mode);}
// #271 GL_APPLE_element_array
inline GLAPI void APIENTRY glElementPointerAPPLE (GLenum type, const void *pointer) {
  _glr->glExt.glElementPointerAPPLE (type, pointer);}
inline GLAPI void APIENTRY glDrawElementArrayAPPLE (GLenum mode, GLint first, GLsizei count) {
  _glr->glExt.glDrawElementArrayAPPLE (mode, first, count);}
inline GLAPI void APIENTRY glDrawRangeElementArrayAPPLE (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count) {
  _glr->glExt.glDrawRangeElementArrayAPPLE (mode, start, end, first, count);}
inline GLAPI void APIENTRY glMultiDrawElementArrayAPPLE (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount) {
  _glr->glExt.glMultiDrawElementArrayAPPLE (mode, first, count, primcount);}
inline GLAPI void APIENTRY glMultiDrawRangeElementArrayAPPLE (GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount) {
  _glr->glExt.glMultiDrawRangeElementArrayAPPLE (mode, start, end, first, count, primcount);}
// #272 GL_APPLE_fence
inline GLAPI void APIENTRY glGenFencesAPPLE (GLsizei n, GLuint *fences) {
  _glr->glExt.glGenFencesAPPLE (n, fences);}
inline GLAPI void APIENTRY glDeleteFencesAPPLE (GLsizei n, const GLuint *fences) {
  _glr->glExt.glDeleteFencesAPPLE (n, fences);}
inline GLAPI void APIENTRY glSetFenceAPPLE (GLuint fence) {
  _glr->glExt.glSetFenceAPPLE (fence);}
inline GLAPI GLboolean APIENTRY glIsFenceAPPLE (GLuint fence) {
  return _glr->glExt.glIsFenceAPPLE (fence);}
inline GLAPI GLboolean APIENTRY glTestFenceAPPLE (GLuint fence) {
  return _glr->glExt.glTestFenceAPPLE (fence);}
inline GLAPI void APIENTRY glFinishFenceAPPLE (GLuint fence) {
  _glr->glExt.glFinishFenceAPPLE (fence);}
inline GLAPI GLboolean APIENTRY glTestObjectAPPLE (GLenum object, GLuint name) {
  return _glr->glExt.glTestObjectAPPLE (object, name);}
inline GLAPI void APIENTRY glFinishObjectAPPLE (GLenum object, GLint name) {
  _glr->glExt.glFinishObjectAPPLE (object, name);}
// #321 GL_APPLE_flush_buffer_range
inline GLAPI void APIENTRY glBufferParameteriAPPLE (GLenum target, GLenum pname, GLint param) {
  _glr->glExt.glBufferParameteriAPPLE (target, pname, param);}
inline GLAPI void APIENTRY glFlushMappedBufferRangeAPPLE (GLenum target, GLintptr offset, GLsizeiptr size) {
  _glr->glExt.glFlushMappedBufferRangeAPPLE (target, offset, size);}
// #371 GL_APPLE_object_purgeable
inline GLAPI GLenum APIENTRY glObjectPurgeableAPPLE (GLenum objectType, GLuint name, GLenum option) {
  return _glr->glExt.glObjectPurgeableAPPLE (objectType, name, option);}
inline GLAPI GLenum APIENTRY glObjectUnpurgeableAPPLE (GLenum objectType, GLuint name, GLenum option) {
  return _glr->glExt.glObjectUnpurgeableAPPLE (objectType, name, option);}
inline GLAPI void APIENTRY glGetObjectParameterivAPPLE (GLenum objectType, GLuint name, GLenum pname, GLint *params) {
  _glr->glExt.glGetObjectParameterivAPPLE (objectType, name, pname, params);}
// #367 GL_APPLE_texture_range
inline GLAPI void APIENTRY glTextureRangeAPPLE (GLenum target, GLsizei length, const void *pointer) {
  _glr->glExt.glTextureRangeAPPLE (target, length, pointer);}
inline GLAPI void APIENTRY glGetTexParameterPointervAPPLE (GLenum target, GLenum pname, void **params) {
  _glr->glExt.glGetTexParameterPointervAPPLE (target, pname, params);}
// #273 GL_APPLE_vertex_array_object
inline GLAPI void APIENTRY glBindVertexArrayAPPLE (GLuint array) {
  _glr->glExt.glBindVertexArrayAPPLE (array);}
inline GLAPI void APIENTRY glDeleteVertexArraysAPPLE (GLsizei n, const GLuint *arrays) {
  _glr->glExt.glDeleteVertexArraysAPPLE (n, arrays);}
inline GLAPI void APIENTRY glGenVertexArraysAPPLE (GLsizei n, GLuint *arrays) {
  _glr->glExt.glGenVertexArraysAPPLE (n, arrays);}
inline GLAPI GLboolean APIENTRY glIsVertexArrayAPPLE (GLuint array) {
  return _glr->glExt.glIsVertexArrayAPPLE (array);}
// #274 GL_APPLE_vertex_array_range
inline GLAPI void APIENTRY glVertexArrayRangeAPPLE (GLsizei length, void *pointer) {
  _glr->glExt.glVertexArrayRangeAPPLE (length, pointer);}
inline GLAPI void APIENTRY glFlushVertexArrayRangeAPPLE (GLsizei length, void *pointer) {
  _glr->glExt.glFlushVertexArrayRangeAPPLE (length, pointer);}
inline GLAPI void APIENTRY glVertexArrayParameteriAPPLE (GLenum pname, GLint param) {
  _glr->glExt.glVertexArrayParameteriAPPLE (pname, param);}
// #369 GL_APPLE_vertex_program_evaluators
inline GLAPI void APIENTRY glEnableVertexAttribAPPLE (GLuint index, GLenum pname) {
  _glr->glExt.glEnableVertexAttribAPPLE (index, pname);}
inline GLAPI void APIENTRY glDisableVertexAttribAPPLE (GLuint index, GLenum pname) {
  _glr->glExt.glDisableVertexAttribAPPLE (index, pname);}
inline GLAPI GLboolean APIENTRY glIsVertexAttribEnabledAPPLE (GLuint index, GLenum pname) {
  return _glr->glExt.glIsVertexAttribEnabledAPPLE (index, pname);}
inline GLAPI void APIENTRY glMapVertexAttrib1dAPPLE (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points) {
  _glr->glExt.glMapVertexAttrib1dAPPLE (index, size, u1, u2, stride, order, points);}
inline GLAPI void APIENTRY glMapVertexAttrib1fAPPLE (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points) {
  _glr->glExt.glMapVertexAttrib1fAPPLE (index, size, u1, u2, stride, order, points);}
inline GLAPI void APIENTRY glMapVertexAttrib2dAPPLE (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points) {
  _glr->glExt.glMapVertexAttrib2dAPPLE (index, size, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
inline GLAPI void APIENTRY glMapVertexAttrib2fAPPLE (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points) {
  _glr->glExt.glMapVertexAttrib2fAPPLE (index, size, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
// #277 GL_ATI_draw_buffers
inline GLAPI void APIENTRY glDrawBuffersATI (GLsizei n, const GLenum *bufs) {
  _glr->glExt.glDrawBuffersATI (n, bufs);}
// #256 GL_ATI_element_array
inline GLAPI void APIENTRY glElementPointerATI (GLenum type, const void *pointer) {
  _glr->glExt.glElementPointerATI (type, pointer);}
inline GLAPI void APIENTRY glDrawElementArrayATI (GLenum mode, GLsizei count) {
  _glr->glExt.glDrawElementArrayATI (mode, count);}
inline GLAPI void APIENTRY glDrawRangeElementArrayATI (GLenum mode, GLuint start, GLuint end, GLsizei count) {
  _glr->glExt.glDrawRangeElementArrayATI (mode, start,  end,  count);}
// #244 GL_ATI_envmap_bumpmap
inline GLAPI void APIENTRY glTexBumpParameterivATI (GLenum pname, const GLint *param) {
  _glr->glExt.glTexBumpParameterivATI (pname, param);}
inline GLAPI void APIENTRY glTexBumpParameterfvATI (GLenum pname, const GLfloat *param) {
  _glr->glExt.glTexBumpParameterfvATI (pname, param);}
inline GLAPI void APIENTRY glGetTexBumpParameterivATI (GLenum pname, GLint *param) {
  _glr->glExt.glGetTexBumpParameterivATI (pname, param);}
inline GLAPI void APIENTRY glGetTexBumpParameterfvATI (GLenum pname, GLfloat *param) {
  _glr->glExt.glGetTexBumpParameterfvATI (pname, param);}
// #245 GL_ATI_fragment_shader
inline GLAPI GLuint APIENTRY glGenFragmentShadersATI (GLuint range) {
  return _glr->glExt.glGenFragmentShadersATI (range);}
inline GLAPI void APIENTRY glBindFragmentShaderATI (GLuint id) {
  _glr->glExt.glBindFragmentShaderATI (id);}
inline GLAPI void APIENTRY glDeleteFragmentShaderATI (GLuint id) {
  _glr->glExt.glDeleteFragmentShaderATI (id);}
inline GLAPI void APIENTRY glBeginFragmentShaderATI (void) {
  _glr->glExt.glBeginFragmentShaderATI ();}
inline GLAPI void APIENTRY glEndFragmentShaderATI (void) {
  _glr->glExt.glEndFragmentShaderATI ();}
inline GLAPI void APIENTRY glPassTexCoordATI (GLuint dst, GLuint coord, GLenum swizzle) {
  _glr->glExt.glPassTexCoordATI (dst, coord, swizzle);}
inline GLAPI void APIENTRY glSampleMapATI (GLuint dst, GLuint interp, GLenum swizzle) {
  _glr->glExt.glSampleMapATI (dst, interp, swizzle);}
inline GLAPI void APIENTRY glColorFragmentOp1ATI (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod) {
  _glr->glExt.glColorFragmentOp1ATI (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod);}
inline GLAPI void APIENTRY glColorFragmentOp2ATI (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod) {
  _glr->glExt.glColorFragmentOp2ATI (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod);}
inline GLAPI void APIENTRY glColorFragmentOp3ATI (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod) {
  _glr->glExt.glColorFragmentOp3ATI (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod);}
inline GLAPI void APIENTRY glAlphaFragmentOp1ATI (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod) {
  _glr->glExt.glAlphaFragmentOp1ATI (op, dst, dstMod, arg1, arg1Rep, arg1Mod);}
inline GLAPI void APIENTRY glAlphaFragmentOp2ATI (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod) {
  _glr->glExt.glAlphaFragmentOp2ATI (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod);}
inline GLAPI void APIENTRY glAlphaFragmentOp3ATI (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod) {
  _glr->glExt.glAlphaFragmentOp3ATI (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod);}
inline GLAPI void APIENTRY glSetFragmentShaderConstantATI (GLuint dst, const GLfloat *value) {
  _glr->glExt.glSetFragmentShaderConstantATI (dst, value);}
// #288 GL_ATI_map_object_buffer
inline GLAPI void *APIENTRY glMapObjectBufferATI (GLuint buffer) {
  return _glr->glExt.glMapObjectBufferATI (buffer);}
inline GLAPI void APIENTRY glUnmapObjectBufferATI (GLuint buffer) {
  _glr->glExt.glUnmapObjectBufferATI (buffer);}
// #246 GL_ATI_pn_triangles
inline GLAPI void APIENTRY glPNTrianglesiATI (GLenum pname, GLint param) {
  _glr->glExt.glPNTrianglesiATI (pname, param);}
inline GLAPI void APIENTRY glPNTrianglesfATI (GLenum pname, GLfloat param) {
  _glr->glExt.glPNTrianglesfATI (pname, param);}
// #289 GL_ATI_separate_stencil
inline GLAPI void APIENTRY glStencilOpSeparateATI (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
  _glr->glExt.glStencilOpSeparateATI (face, sfail, dpfail, dppass);}
inline GLAPI void APIENTRY glStencilFuncSeparateATI (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask) {
  _glr->glExt.glStencilFuncSeparateATI (frontfunc, backfunc, ref, mask);}
// #247 GL_ATI_vertex_array_object
inline GLAPI GLuint APIENTRY glNewObjectBufferATI (GLsizei size, const void *pointer, GLenum usage) {
  return _glr->glExt.glNewObjectBufferATI (size, pointer,  usage);}
inline GLAPI GLboolean APIENTRY glIsObjectBufferATI (GLuint buffer) {
  return _glr->glExt.glIsObjectBufferATI (buffer);}
inline GLAPI void APIENTRY glUpdateObjectBufferATI (GLuint buffer, GLuint offset, GLsizei size, const void *pointer, GLenum preserve) {
  _glr->glExt.glUpdateObjectBufferATI (buffer, offset, size, pointer,  preserve);}
inline GLAPI void APIENTRY glGetObjectBufferfvATI (GLuint buffer, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetObjectBufferfvATI (buffer, pname, params);}
inline GLAPI void APIENTRY glGetObjectBufferivATI (GLuint buffer, GLenum pname, GLint *params) {
  _glr->glExt.glGetObjectBufferivATI (buffer, pname, params);}
inline GLAPI void APIENTRY glFreeObjectBufferATI (GLuint buffer) {
  _glr->glExt.glFreeObjectBufferATI (buffer);}
inline GLAPI void APIENTRY glArrayObjectATI (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset) {
  _glr->glExt.glArrayObjectATI (array, size, type, stride, buffer, offset);}
inline GLAPI void APIENTRY glGetArrayObjectfvATI (GLenum array, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetArrayObjectfvATI (array, pname, params);}
inline GLAPI void APIENTRY glGetArrayObjectivATI (GLenum array, GLenum pname, GLint *params) {
  _glr->glExt.glGetArrayObjectivATI (array, pname, params);}
inline GLAPI void APIENTRY glVariantArrayObjectATI (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset) {
  _glr->glExt.glVariantArrayObjectATI (id, type, stride, buffer, offset);}
inline GLAPI void APIENTRY glGetVariantArrayObjectfvATI (GLuint id, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetVariantArrayObjectfvATI (id, pname, params);}
inline GLAPI void APIENTRY glGetVariantArrayObjectivATI (GLuint id, GLenum pname, GLint *params) {
  _glr->glExt.glGetVariantArrayObjectivATI (id, pname, params);}
// #290 GL_ATI_vertex_attrib_array_object
inline GLAPI void APIENTRY glVertexAttribArrayObjectATI (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset) {
  _glr->glExt.glVertexAttribArrayObjectATI (index, size, type, normalized, stride, buffer, offset);}
inline GLAPI void APIENTRY glGetVertexAttribArrayObjectfvATI (GLuint index, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetVertexAttribArrayObjectfvATI (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribArrayObjectivATI (GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetVertexAttribArrayObjectivATI (index, pname, params);}
// #249 GL_ATI_vertex_streams
inline GLAPI void APIENTRY glVertexStream1sATI (GLenum stream, GLshort x) {
  _glr->glExt.glVertexStream1sATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1svATI (GLenum stream, const GLshort *coords) {
  _glr->glExt.glVertexStream1svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream1iATI (GLenum stream, GLint x) {
  _glr->glExt.glVertexStream1iATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1ivATI (GLenum stream, const GLint *coords) {
  _glr->glExt.glVertexStream1ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream1fATI (GLenum stream, GLfloat x) {
  _glr->glExt.glVertexStream1fATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1fvATI (GLenum stream, const GLfloat *coords) {
  _glr->glExt.glVertexStream1fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream1dATI (GLenum stream, GLdouble x) {
  _glr->glExt.glVertexStream1dATI (stream, x);}
inline GLAPI void APIENTRY glVertexStream1dvATI (GLenum stream, const GLdouble *coords) {
  _glr->glExt.glVertexStream1dvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2sATI (GLenum stream, GLshort x, GLshort y) {
  _glr->glExt.glVertexStream2sATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2svATI (GLenum stream, const GLshort *coords) {
  _glr->glExt.glVertexStream2svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2iATI (GLenum stream, GLint x, GLint y) {
  _glr->glExt.glVertexStream2iATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2ivATI (GLenum stream, const GLint *coords) {
  _glr->glExt.glVertexStream2ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2fATI (GLenum stream, GLfloat x, GLfloat y) {
  _glr->glExt.glVertexStream2fATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2fvATI (GLenum stream, const GLfloat *coords) {
  _glr->glExt.glVertexStream2fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream2dATI (GLenum stream, GLdouble x, GLdouble y) {
  _glr->glExt.glVertexStream2dATI (stream, x, y);}
inline GLAPI void APIENTRY glVertexStream2dvATI (GLenum stream, const GLdouble *coords) {
  _glr->glExt.glVertexStream2dvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3sATI (GLenum stream, GLshort x, GLshort y, GLshort z) {
  _glr->glExt.glVertexStream3sATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3svATI (GLenum stream, const GLshort *coords) {
  _glr->glExt.glVertexStream3svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3iATI (GLenum stream, GLint x, GLint y, GLint z) {
  _glr->glExt.glVertexStream3iATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3ivATI (GLenum stream, const GLint *coords) {
  _glr->glExt.glVertexStream3ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3fATI (GLenum stream, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glVertexStream3fATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3fvATI (GLenum stream, const GLfloat *coords) {
  _glr->glExt.glVertexStream3fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream3dATI (GLenum stream, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glVertexStream3dATI (stream, x, y, z);}
inline GLAPI void APIENTRY glVertexStream3dvATI (GLenum stream, const GLdouble *coords) {
  _glr->glExt.glVertexStream3dvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4sATI (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w) {
  _glr->glExt.glVertexStream4sATI (stream,  x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4svATI (GLenum stream, const GLshort *coords) {
  _glr->glExt.glVertexStream4svATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4iATI (GLenum stream, GLint x, GLint y, GLint z, GLint w) {
  _glr->glExt.glVertexStream4iATI (stream, x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4ivATI (GLenum stream, const GLint *coords) {
  _glr->glExt.glVertexStream4ivATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4fATI (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glVertexStream4fATI (stream, x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4fvATI (GLenum stream, const GLfloat *coords) {
  _glr->glExt.glVertexStream4fvATI (stream, coords);}
inline GLAPI void APIENTRY glVertexStream4dATI (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glVertexStream4dATI (stream, x, y, z, w);}
inline GLAPI void APIENTRY glVertexStream4dvATI (GLenum stream, const GLdouble *coords) {
  _glr->glExt.glVertexStream4dvATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3bATI (GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz) {
  _glr->glExt.glNormalStream3bATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3bvATI (GLenum stream, const GLbyte *coords) {
  _glr->glExt.glNormalStream3bvATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3sATI (GLenum stream, GLshort nx, GLshort ny, GLshort nz) {
  _glr->glExt.glNormalStream3sATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3svATI (GLenum stream, const GLshort *coords) {
  _glr->glExt.glNormalStream3svATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3iATI (GLenum stream, GLint nx, GLint ny, GLint nz) {
  _glr->glExt.glNormalStream3iATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3ivATI (GLenum stream, const GLint *coords) {
  _glr->glExt.glNormalStream3ivATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3fATI (GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz) {
  _glr->glExt.glNormalStream3fATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3fvATI (GLenum stream, const GLfloat *coords) {
  _glr->glExt.glNormalStream3fvATI (stream, coords);}
inline GLAPI void APIENTRY glNormalStream3dATI (GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz) {
  _glr->glExt.glNormalStream3dATI (stream, nx, ny, nz);}
inline GLAPI void APIENTRY glNormalStream3dvATI (GLenum stream, const GLdouble *coords) {
  _glr->glExt.glNormalStream3dvATI (stream, coords);}
inline GLAPI void APIENTRY glClientActiveVertexStreamATI (GLenum stream) {
  _glr->glExt.glClientActiveVertexStreamATI (stream);}
inline GLAPI void APIENTRY glVertexBlendEnviATI (GLenum pname, GLint param) {
  _glr->glExt.glVertexBlendEnviATI (pname, param);}
inline GLAPI void APIENTRY glVertexBlendEnvfATI (GLenum pname, GLfloat param) {
  _glr->glExt.glVertexBlendEnvfATI (pname, param);}
// #342 GL_EXT_bindable_uniform
inline GLAPI void APIENTRY glUniformBufferEXT (GLuint program, GLint location, GLuint buffer) {
  _glr->glExt.glUniformBufferEXT (program, location, buffer);}
inline GLAPI GLint APIENTRY glGetUniformBufferSizeEXT (GLuint program, GLint location) {
  return _glr->glExt.glGetUniformBufferSizeEXT (program, location);}
inline GLAPI GLintptr APIENTRY glGetUniformOffsetEXT (GLuint program, GLint location) {
  return _glr->glExt.glGetUniformOffsetEXT (program, location);}
// #002 GL_EXT_blend_color
inline GLAPI void APIENTRY glBlendColorEXT (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  _glr->glExt.glBlendColorEXT (red, green, blue,  alpha);}
// #299 GL_EXT_blend_equation_separate
inline GLAPI void APIENTRY glBlendEquationSeparateEXT (GLenum modeRGB, GLenum modeAlpha) {
  _glr->glExt.glBlendEquationSeparateEXT (modeRGB, modeAlpha);}
// #173 GL_EXT_blend_func_separate
inline GLAPI void APIENTRY glBlendFuncSeparateEXT (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
  _glr->glExt.glBlendFuncSeparateEXT (sfactorRGB, dfactorRGB, sfactorAlpha,  dfactorAlpha);}
// #037 GL_EXT_blend_minmax
inline GLAPI void APIENTRY glBlendEquationEXT (GLenum mode) {
  _glr->glExt.glBlendEquationEXT (mode);}
// #074 GL_EXT_color_subtable
inline GLAPI void APIENTRY glColorSubTableEXT (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glColorSubTableEXT (target, start, count, format, type, data);}
inline GLAPI void APIENTRY glCopyColorSubTableEXT (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyColorSubTableEXT (target, start, x, y, width);}
// #097 GL_EXT_compiled_vertex_array
inline GLAPI void APIENTRY glLockArraysEXT (GLint first, GLsizei count) {
  _glr->glExt.glLockArraysEXT (first,  count);}
inline GLAPI void APIENTRY glUnlockArraysEXT (void) {
  _glr->glExt.glUnlockArraysEXT ();}
// #012 GL_EXT_convolution
inline GLAPI void APIENTRY glConvolutionFilter1DEXT (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image) {
  _glr->glExt.glConvolutionFilter1DEXT (target, internalformat, width, format, type, image);}
inline GLAPI void APIENTRY glConvolutionFilter2DEXT (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image) {
  _glr->glExt.glConvolutionFilter2DEXT (target, internalformat, width, height, format, type, image);}
inline GLAPI void APIENTRY glConvolutionParameterfEXT (GLenum target, GLenum pname, GLfloat params) {
  _glr->glExt.glConvolutionParameterfEXT (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameterfvEXT (GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glConvolutionParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameteriEXT (GLenum target, GLenum pname, GLint params) {
  _glr->glExt.glConvolutionParameteriEXT (target, pname, params);}
inline GLAPI void APIENTRY glConvolutionParameterivEXT (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glConvolutionParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glCopyConvolutionFilter1DEXT (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyConvolutionFilter1DEXT (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glCopyConvolutionFilter2DEXT (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyConvolutionFilter2DEXT (target, internalformat, x, y, width, height);}
inline GLAPI void APIENTRY glGetConvolutionFilterEXT (GLenum target, GLenum format, GLenum type, void *image) {
  _glr->glExt.glGetConvolutionFilterEXT (target, format, type, image);}
inline GLAPI void APIENTRY glGetConvolutionParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetConvolutionParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetConvolutionParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetConvolutionParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetSeparableFilterEXT (GLenum target, GLenum format, GLenum type, void *row, void *column, void *span) {
  _glr->glExt.glGetSeparableFilterEXT (target, format, type, row, column, span);}
inline GLAPI void APIENTRY glSeparableFilter2DEXT (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column) {
  _glr->glExt.glSeparableFilter2DEXT (target, internalformat, width, height, format, type, row, column);}
// #156 GL_EXT_coordinate_frame
inline GLAPI void APIENTRY glTangent3bEXT (GLbyte tx, GLbyte ty, GLbyte tz) {
  _glr->glExt.glTangent3bEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3bvEXT (const GLbyte *v) {
  _glr->glExt.glTangent3bvEXT (v);}
inline GLAPI void APIENTRY glTangent3dEXT (GLdouble tx, GLdouble ty, GLdouble tz) {
  _glr->glExt.glTangent3dEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3dvEXT (const GLdouble *v) {
  _glr->glExt.glTangent3dvEXT (v);}
inline GLAPI void APIENTRY glTangent3fEXT (GLfloat tx, GLfloat ty, GLfloat tz) {
  _glr->glExt.glTangent3fEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3fvEXT (const GLfloat *v) {
  _glr->glExt.glTangent3fvEXT (v);}
inline GLAPI void APIENTRY glTangent3iEXT (GLint tx, GLint ty, GLint tz) {
  _glr->glExt.glTangent3iEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3ivEXT (const GLint *v) {
  _glr->glExt.glTangent3ivEXT (v);}
inline GLAPI void APIENTRY glTangent3sEXT (GLshort tx, GLshort ty, GLshort tz) {
  _glr->glExt.glTangent3sEXT (tx, ty, tz);}
inline GLAPI void APIENTRY glTangent3svEXT (const GLshort *v) {
  _glr->glExt.glTangent3svEXT (v);}
inline GLAPI void APIENTRY glBinormal3bEXT (GLbyte bx, GLbyte by, GLbyte bz) {
  _glr->glExt.glBinormal3bEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3bvEXT (const GLbyte *v) {
  _glr->glExt.glBinormal3bvEXT (v);}
inline GLAPI void APIENTRY glBinormal3dEXT (GLdouble bx, GLdouble by, GLdouble bz) {
  _glr->glExt.glBinormal3dEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3dvEXT (const GLdouble *v) {
  _glr->glExt.glBinormal3dvEXT (v);}
inline GLAPI void APIENTRY glBinormal3fEXT (GLfloat bx, GLfloat by, GLfloat bz) {
  _glr->glExt.glBinormal3fEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3fvEXT (const GLfloat *v) {
  _glr->glExt.glBinormal3fvEXT (v);}
inline GLAPI void APIENTRY glBinormal3iEXT (GLint bx, GLint by, GLint bz) {
  _glr->glExt.glBinormal3iEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3ivEXT (const GLint *v) {
  _glr->glExt.glBinormal3ivEXT (v);}
inline GLAPI void APIENTRY glBinormal3sEXT (GLshort bx, GLshort by, GLshort bz) {
  _glr->glExt.glBinormal3sEXT (bx, by, bz);}
inline GLAPI void APIENTRY glBinormal3svEXT (const GLshort *v) {
  _glr->glExt.glBinormal3svEXT (v);}
inline GLAPI void APIENTRY glTangentPointerEXT (GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glTangentPointerEXT (type, stride, pointer);}
inline GLAPI void APIENTRY glBinormalPointerEXT (GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glBinormalPointerEXT (type, stride, pointer);}
// #010 GL_EXT_copy_texture
inline GLAPI void APIENTRY glCopyTexImage1DEXT (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
  _glr->glExt.glCopyTexImage1DEXT (target, level, internalformat, x, y, width, border);}
inline GLAPI void APIENTRY glCopyTexImage2DEXT (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
  _glr->glExt.glCopyTexImage2DEXT (target, level, internalformat, x, y, width, height, border);}
inline GLAPI void APIENTRY glCopyTexSubImage1DEXT (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyTexSubImage1DEXT (target, level, xoffset, x, y, width);}
inline GLAPI void APIENTRY glCopyTexSubImage2DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyTexSubImage2DEXT (target, level, xoffset, yoffset, x, y, width, height);}
inline GLAPI void APIENTRY glCopyTexSubImage3DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyTexSubImage3DEXT (target, level, xoffset, yoffset, zoffset, x, y, width, height);}
// #098 GL_EXT_cull_vertex
inline GLAPI void APIENTRY glCullParameterdvEXT (GLenum pname, GLdouble *params) {
  _glr->glExt.glCullParameterdvEXT (pname, params);}
inline GLAPI void APIENTRY glCullParameterfvEXT (GLenum pname, GLfloat *params) {
  _glr->glExt.glCullParameterfvEXT (pname, params);}
// #439 GL_EXT_debug_label
inline GLAPI void APIENTRY glLabelObjectEXT (GLenum type, GLuint object, GLsizei length, const GLchar *label) {
  _glr->glExt.glLabelObjectEXT (type, object, length, label);}
inline GLAPI void APIENTRY glGetObjectLabelEXT (GLenum type, GLuint object, GLsizei bufSize, GLsizei *length, GLchar *label) {
  _glr->glExt.glGetObjectLabelEXT (type, object, bufSize, length, label);}
// #440 GL_EXT_debug_marker
inline GLAPI void APIENTRY glInsertEventMarkerEXT (GLsizei length, const GLchar *marker) {
  _glr->glExt.glInsertEventMarkerEXT (length, marker);}
inline GLAPI void APIENTRY glPushGroupMarkerEXT (GLsizei length, const GLchar *marker) {
  _glr->glExt.glPushGroupMarkerEXT (length, marker);}
inline GLAPI void APIENTRY glPopGroupMarkerEXT (void) {
  _glr->glExt.glPopGroupMarkerEXT ();}
// #297 GL_EXT_depth_bounds_test
inline GLAPI void APIENTRY glDepthBoundsEXT (GLclampd zmin, GLclampd zmax) {
  _glr->glExt.glDepthBoundsEXT (zmin, zmax);}
// #353 GL_EXT_direct_state_access
inline GLAPI void APIENTRY glMatrixLoadfEXT (GLenum mode, const GLfloat *m) {
  _glr->glExt.glMatrixLoadfEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixLoaddEXT (GLenum mode, const GLdouble *m) {
  _glr->glExt.glMatrixLoaddEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultfEXT (GLenum mode, const GLfloat *m) {
  _glr->glExt.glMatrixMultfEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultdEXT (GLenum mode, const GLdouble *m) {
  _glr->glExt.glMatrixMultdEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixLoadIdentityEXT (GLenum mode) {
  _glr->glExt.glMatrixLoadIdentityEXT (mode);}
inline GLAPI void APIENTRY glMatrixRotatefEXT (GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glMatrixRotatefEXT (mode, angle, x, y, z);}
inline GLAPI void APIENTRY glMatrixRotatedEXT (GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glMatrixRotatedEXT (mode, angle, x, y, z);}
inline GLAPI void APIENTRY glMatrixScalefEXT (GLenum mode, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glMatrixScalefEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixScaledEXT (GLenum mode, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glMatrixScaledEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixTranslatefEXT (GLenum mode, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glMatrixTranslatefEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixTranslatedEXT (GLenum mode, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glMatrixTranslatedEXT (mode, x, y, z);}
inline GLAPI void APIENTRY glMatrixFrustumEXT (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
  _glr->glExt.glMatrixFrustumEXT (mode, left, right, bottom,  top, zNear, zFar);}
inline GLAPI void APIENTRY glMatrixOrthoEXT (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
  _glr->glExt.glMatrixOrthoEXT (mode, left, right, bottom, top, zNear, zFar);}
inline GLAPI void APIENTRY glMatrixPopEXT (GLenum mode) {
  _glr->glExt.glMatrixPopEXT (mode);}
inline GLAPI void APIENTRY glMatrixPushEXT (GLenum mode) {
  _glr->glExt.glMatrixPushEXT (mode);}
inline GLAPI void APIENTRY glClientAttribDefaultEXT (GLbitfield mask) {
  _glr->glExt.glClientAttribDefaultEXT (mask);}
inline GLAPI void APIENTRY glPushClientAttribDefaultEXT (GLbitfield mask) {
  _glr->glExt.glPushClientAttribDefaultEXT (mask);}
inline GLAPI void APIENTRY glTextureParameterfEXT (GLuint texture, GLenum target, GLenum pname, GLfloat param) {
  _glr->glExt.glTextureParameterfEXT (texture, target, pname, param);}
inline GLAPI void APIENTRY glTextureParameterfvEXT (GLuint texture, GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glTextureParameterfvEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glTextureParameteriEXT (GLuint texture, GLenum target, GLenum pname, GLint param) {
  _glr->glExt.glTextureParameteriEXT (texture, target, pname, param);}
inline GLAPI void APIENTRY glTextureParameterivEXT (GLuint texture, GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glTextureParameterivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glTextureImage1DEXT (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTextureImage1DEXT (texture, target, level, internalformat, width, border, format, type, pixels);}
inline GLAPI void APIENTRY glTextureImage2DEXT (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTextureImage2DEXT (texture, target, level, internalformat, width, height, border, format, type, pixels);}
inline GLAPI void APIENTRY glTextureSubImage1DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTextureSubImage1DEXT (texture, target, level, xoffset, width, format, type, pixels);}
inline GLAPI void APIENTRY glTextureSubImage2DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTextureSubImage2DEXT (texture, target, level, xoffset, yoffset, width, height, format,  type, pixels);}
inline GLAPI void APIENTRY glCopyTextureImage1DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
  _glr->glExt.glCopyTextureImage1DEXT (texture, target, level, internalformat, x, y, width, border);}
inline GLAPI void APIENTRY glCopyTextureImage2DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
  _glr->glExt.glCopyTextureImage2DEXT (texture, target, level, internalformat, x, y, width, height, border);}
inline GLAPI void APIENTRY glCopyTextureSubImage1DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyTextureSubImage1DEXT (texture, target, level, xoffset, x, y, width);}
inline GLAPI void APIENTRY glCopyTextureSubImage2DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyTextureSubImage2DEXT (texture, target, level, xoffset, yoffset, x, y, width, height);}
inline GLAPI void APIENTRY glGetTextureImageEXT (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void *pixels) {
  _glr->glExt.glGetTextureImageEXT (texture, target, level, format, type, pixels);}
inline GLAPI void APIENTRY glGetTextureParameterfvEXT (GLuint texture, GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetTextureParameterfvEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureParameterivEXT (GLuint texture, GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetTextureParameterivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureLevelParameterfvEXT (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetTextureLevelParameterfvEXT (texture,  target, level, pname, params);}
inline GLAPI void APIENTRY glGetTextureLevelParameterivEXT (GLuint texture, GLenum target, GLint level, GLenum pname, GLint *params) {
  _glr->glExt.glGetTextureLevelParameterivEXT (texture, target, level, pname, params);}
inline GLAPI void APIENTRY glTextureImage3DEXT (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTextureImage3DEXT (texture, target, level, internalformat,  width, height, depth, border, format, type, pixels);}
inline GLAPI void APIENTRY glTextureSubImage3DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTextureSubImage3DEXT (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);}
inline GLAPI void APIENTRY glCopyTextureSubImage3DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyTextureSubImage3DEXT (texture, target, level, xoffset, yoffset, zoffset, x, y, width, height);}
inline GLAPI void APIENTRY glBindMultiTextureEXT (GLenum texunit, GLenum target, GLuint texture) {
  _glr->glExt.glBindMultiTextureEXT (texunit, target,  texture);}
inline GLAPI void APIENTRY glMultiTexCoordPointerEXT (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glMultiTexCoordPointerEXT (texunit, size, type, stride, pointer);}
inline GLAPI void APIENTRY glMultiTexEnvfEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat param) {
  _glr->glExt.glMultiTexEnvfEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexEnvfvEXT (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glMultiTexEnvfvEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexEnviEXT (GLenum texunit, GLenum target, GLenum pname, GLint param) {
  _glr->glExt.glMultiTexEnviEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexEnvivEXT (GLenum texunit, GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glMultiTexEnvivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexGendEXT (GLenum texunit, GLenum coord, GLenum pname, GLdouble param) {
  _glr->glExt.glMultiTexGendEXT (texunit, coord, pname, param);}
inline GLAPI void APIENTRY glMultiTexGendvEXT (GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params) {
  _glr->glExt.glMultiTexGendvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glMultiTexGenfEXT (GLenum texunit, GLenum coord, GLenum pname, GLfloat param) {
  _glr->glExt.glMultiTexGenfEXT (texunit, coord, pname, param);}
inline GLAPI void APIENTRY glMultiTexGenfvEXT (GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params) {
  _glr->glExt.glMultiTexGenfvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glMultiTexGeniEXT (GLenum texunit, GLenum coord, GLenum pname, GLint param) {
  _glr->glExt.glMultiTexGeniEXT (texunit, coord, pname, param);}
inline GLAPI void APIENTRY glMultiTexGenivEXT (GLenum texunit, GLenum coord, GLenum pname, const GLint *params) {
  _glr->glExt.glMultiTexGenivEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexEnvfvEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMultiTexEnvfvEXT (texunit, target,  pname, params);}
inline GLAPI void APIENTRY glGetMultiTexEnvivEXT (GLenum texunit, GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetMultiTexEnvivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexGendvEXT (GLenum texunit, GLenum coord, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetMultiTexGendvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexGenfvEXT (GLenum texunit, GLenum coord, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMultiTexGenfvEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexGenivEXT (GLenum texunit, GLenum coord, GLenum pname, GLint *params) {
  _glr->glExt.glGetMultiTexGenivEXT (texunit, coord, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameteriEXT (GLenum texunit, GLenum target, GLenum pname, GLint param) {
  _glr->glExt.glMultiTexParameteriEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexParameterivEXT (GLenum texunit, GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glMultiTexParameterivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameterfEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat param) {
  _glr->glExt.glMultiTexParameterfEXT (texunit, target, pname, param);}
inline GLAPI void APIENTRY glMultiTexParameterfvEXT (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glMultiTexParameterfvEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glMultiTexImage1DEXT (texunit, target, level, internalformat, width, border, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glMultiTexImage2DEXT (texunit, target, level, internalformat, width, height, border, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexSubImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glMultiTexSubImage1DEXT (texunit, target, level, xoffset, width, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexSubImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glMultiTexSubImage2DEXT (texunit, target, level, xoffset, yoffset, width, height, format, type, pixels);}
inline GLAPI void APIENTRY glCopyMultiTexImage1DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
  _glr->glExt.glCopyMultiTexImage1DEXT (texunit, target, level, internalformat, x, y, width, border);}
inline GLAPI void APIENTRY glCopyMultiTexImage2DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
  _glr->glExt.glCopyMultiTexImage2DEXT (texunit, target, level, internalformat, x, y, width, height, border);}
inline GLAPI void APIENTRY glCopyMultiTexSubImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyMultiTexSubImage1DEXT (texunit, target, level, xoffset, x, y, width);}
inline GLAPI void APIENTRY glCopyMultiTexSubImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyMultiTexSubImage2DEXT (texunit, target, level, xoffset, yoffset, x, y, width, height);}
inline GLAPI void APIENTRY glGetMultiTexImageEXT (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void *pixels) {
  _glr->glExt.glGetMultiTexImageEXT (texunit, target, level, format, type, pixels);}
inline GLAPI void APIENTRY glGetMultiTexParameterfvEXT (GLenum texunit, GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMultiTexParameterfvEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexParameterivEXT (GLenum texunit, GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetMultiTexParameterivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexLevelParameterfvEXT (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMultiTexLevelParameterfvEXT (texunit, target, level, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexLevelParameterivEXT (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint *params) {
  _glr->glExt.glGetMultiTexLevelParameterivEXT (texunit, target, level, pname, params);}
inline GLAPI void APIENTRY glMultiTexImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glMultiTexImage3DEXT (texunit, target, level, internalformat, width, height, depth, border, format, type, pixels);}
inline GLAPI void APIENTRY glMultiTexSubImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glMultiTexSubImage3DEXT (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);}
inline GLAPI void APIENTRY glCopyMultiTexSubImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
  _glr->glExt.glCopyMultiTexSubImage3DEXT (texunit, target, level, xoffset,  yoffset, zoffset, x, y, width, height);}
inline GLAPI void APIENTRY glEnableClientStateIndexedEXT (GLenum array, GLuint index) {
  _glr->glExt.glEnableClientStateIndexedEXT (array, index);}
inline GLAPI void APIENTRY glDisableClientStateIndexedEXT (GLenum array, GLuint index) {
  _glr->glExt.glDisableClientStateIndexedEXT (array, index);}
inline GLAPI void APIENTRY glGetFloatIndexedvEXT (GLenum target, GLuint index, GLfloat *data) {
  _glr->glExt.glGetFloatIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glGetDoubleIndexedvEXT (GLenum target, GLuint index, GLdouble *data) {
  _glr->glExt.glGetDoubleIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glGetPointerIndexedvEXT (GLenum target, GLuint index, void **data) {
  _glr->glExt.glGetPointerIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glEnableIndexedEXT (GLenum target, GLuint index) {
  _glr->glExt.glEnableIndexedEXT (target, index);}
inline GLAPI void APIENTRY glDisableIndexedEXT (GLenum target, GLuint index) {
  _glr->glExt.glDisableIndexedEXT (target, index);}
inline GLAPI GLboolean APIENTRY glIsEnabledIndexedEXT (GLenum target, GLuint index) {
  return _glr->glExt.glIsEnabledIndexedEXT (target, index);}
inline GLAPI void APIENTRY glGetIntegerIndexedvEXT (GLenum target, GLuint index, GLint *data) {
  _glr->glExt.glGetIntegerIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glGetBooleanIndexedvEXT (GLenum target, GLuint index, GLboolean *data) {
  _glr->glExt.glGetBooleanIndexedvEXT (target, index, data);}
inline GLAPI void APIENTRY glCompressedTextureImage3DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedTextureImage3DEXT (texture, target, level, internalformat, width, height, depth, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureImage2DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedTextureImage2DEXT (texture, target, level, internalformat, width, height, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureImage1DEXT (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedTextureImage1DEXT (texture, target, level, internalformat,  width, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureSubImage3DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedTextureSubImage3DEXT (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureSubImage2DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedTextureSubImage2DEXT (texture, target, level, xoffset, yoffset, width, height, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedTextureSubImage1DEXT (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedTextureSubImage1DEXT (texture, target, level, xoffset, width, format, imageSize, bits);}
inline GLAPI void APIENTRY glGetCompressedTextureImageEXT (GLuint texture, GLenum target, GLint lod, void *img) {
  _glr->glExt.glGetCompressedTextureImageEXT (texture, target, lod, img);}
inline GLAPI void APIENTRY glCompressedMultiTexImage3DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedMultiTexImage3DEXT (texunit, target, level, internalformat, width, height, depth, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexImage2DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedMultiTexImage2DEXT (texunit, target, level, internalformat, width, height, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexImage1DEXT (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedMultiTexImage1DEXT (texunit, target, level, internalformat, width, border, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexSubImage3DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedMultiTexSubImage3DEXT (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexSubImage2DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedMultiTexSubImage2DEXT (texunit, target, level, xoffset, yoffset, width, height, format, imageSize, bits);}
inline GLAPI void APIENTRY glCompressedMultiTexSubImage1DEXT (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits) {
  _glr->glExt.glCompressedMultiTexSubImage1DEXT (texunit, target, level, xoffset, width, format, imageSize, bits);}
inline GLAPI void APIENTRY glGetCompressedMultiTexImageEXT (GLenum texunit, GLenum target, GLint lod, void *img) {
  _glr->glExt.glGetCompressedMultiTexImageEXT (texunit, target, lod, img);}
inline GLAPI void APIENTRY glMatrixLoadTransposefEXT (GLenum mode, const GLfloat *m) {
  _glr->glExt.glMatrixLoadTransposefEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixLoadTransposedEXT (GLenum mode, const GLdouble *m) {
  _glr->glExt.glMatrixLoadTransposedEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultTransposefEXT (GLenum mode, const GLfloat *m) {
  _glr->glExt.glMatrixMultTransposefEXT (mode, m);}
inline GLAPI void APIENTRY glMatrixMultTransposedEXT (GLenum mode, const GLdouble *m) {
  _glr->glExt.glMatrixMultTransposedEXT (mode, m);}
inline GLAPI void APIENTRY glNamedBufferDataEXT (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage) {
  _glr->glExt.glNamedBufferDataEXT (buffer, size, data, usage);}
inline GLAPI void APIENTRY glNamedBufferSubDataEXT (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data) {
  _glr->glExt.glNamedBufferSubDataEXT (buffer, offset, size, data);}
inline GLAPI void *APIENTRY glMapNamedBufferEXT (GLuint buffer, GLenum access) {
  return _glr->glExt.glMapNamedBufferEXT (buffer, access);}
inline GLAPI GLboolean APIENTRY glUnmapNamedBufferEXT (GLuint buffer) {
  return _glr->glExt.glUnmapNamedBufferEXT (buffer);}
inline GLAPI void APIENTRY glGetNamedBufferParameterivEXT (GLuint buffer, GLenum pname, GLint *params) {
  _glr->glExt.glGetNamedBufferParameterivEXT (buffer, pname, params);}
inline GLAPI void APIENTRY glGetNamedBufferPointervEXT (GLuint buffer, GLenum pname, void **params) {
  _glr->glExt.glGetNamedBufferPointervEXT (buffer, pname, params);}
inline GLAPI void APIENTRY glGetNamedBufferSubDataEXT (GLuint buffer, GLintptr offset, GLsizeiptr size, void *data) {
  _glr->glExt.glGetNamedBufferSubDataEXT (buffer, offset, size, data);}
inline GLAPI void APIENTRY glProgramUniform1fEXT (GLuint program, GLint location, GLfloat v0) {
  _glr->glExt.glProgramUniform1fEXT (program, location,  v0);}
inline GLAPI void APIENTRY glProgramUniform2fEXT (GLuint program, GLint location, GLfloat v0, GLfloat v1) {
  _glr->glExt.glProgramUniform2fEXT (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform3fEXT (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
  _glr->glExt.glProgramUniform3fEXT (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform4fEXT (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  _glr->glExt.glProgramUniform4fEXT (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform1iEXT (GLuint program, GLint location, GLint v0) {
  _glr->glExt.glProgramUniform1iEXT (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform2iEXT (GLuint program, GLint location, GLint v0, GLint v1) {
  _glr->glExt.glProgramUniform2iEXT (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform3iEXT (GLuint program, GLint location, GLint v0, GLint v1, GLint v2) {
  _glr->glExt.glProgramUniform3iEXT (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform4iEXT (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
  _glr->glExt.glProgramUniform4iEXT (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform1fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform1fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform2fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform3fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4fvEXT (GLuint program, GLint location, GLsizei count, const GLfloat *value) {
  _glr->glExt.glProgramUniform4fvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform1ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform1ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform2ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform3ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4ivEXT (GLuint program, GLint location, GLsizei count, const GLint *value) {
  _glr->glExt.glProgramUniform4ivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix2fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix3fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix4fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix2x3fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix3x2fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix2x4fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix4x2fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix3x4fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3fvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
  _glr->glExt.glProgramUniformMatrix4x3fvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glTextureBufferEXT (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer) {
  _glr->glExt.glTextureBufferEXT (texture, target, internalformat, buffer);}
inline GLAPI void APIENTRY glMultiTexBufferEXT (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer) {
  _glr->glExt.glMultiTexBufferEXT (texunit, target, internalformat, buffer);}
inline GLAPI void APIENTRY glTextureParameterIivEXT (GLuint texture, GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glTextureParameterIivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glTextureParameterIuivEXT (GLuint texture, GLenum target, GLenum pname, const GLuint *params) {
  _glr->glExt.glTextureParameterIuivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureParameterIivEXT (GLuint texture, GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetTextureParameterIivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glGetTextureParameterIuivEXT (GLuint texture, GLenum target, GLenum pname, GLuint *params) {
  _glr->glExt.glGetTextureParameterIuivEXT (texture, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameterIivEXT (GLenum texunit, GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glMultiTexParameterIivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glMultiTexParameterIuivEXT (GLenum texunit, GLenum target, GLenum pname, const GLuint *params) {
  _glr->glExt.glMultiTexParameterIuivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexParameterIivEXT (GLenum texunit, GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetMultiTexParameterIivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glGetMultiTexParameterIuivEXT (GLenum texunit, GLenum target, GLenum pname, GLuint *params) {
  _glr->glExt.glGetMultiTexParameterIuivEXT (texunit, target, pname, params);}
inline GLAPI void APIENTRY glProgramUniform1uiEXT (GLuint program, GLint location, GLuint v0) {
  _glr->glExt.glProgramUniform1uiEXT (program, location, v0);}
inline GLAPI void APIENTRY glProgramUniform2uiEXT (GLuint program, GLint location, GLuint v0, GLuint v1) {
  _glr->glExt.glProgramUniform2uiEXT (program, location, v0, v1);}
inline GLAPI void APIENTRY glProgramUniform3uiEXT (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) {
  _glr->glExt.glProgramUniform3uiEXT (program, location, v0, v1, v2);}
inline GLAPI void APIENTRY glProgramUniform4uiEXT (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  _glr->glExt.glProgramUniform4uiEXT (program, location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glProgramUniform1uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform1uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform2uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform3uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4uivEXT (GLuint program, GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glProgramUniform4uivEXT (program, location, count, value);}
inline GLAPI void APIENTRY glNamedProgramLocalParameters4fvEXT (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat *params) {
  _glr->glExt.glNamedProgramLocalParameters4fvEXT (program, target, index, count, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4iEXT (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w) {
  _glr->glExt.glNamedProgramLocalParameterI4iEXT (program, target,  index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4ivEXT (GLuint program, GLenum target, GLuint index, const GLint *params) {
  _glr->glExt.glNamedProgramLocalParameterI4ivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParametersI4ivEXT (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint *params) {
  _glr->glExt.glNamedProgramLocalParametersI4ivEXT (program,  target,  index, count, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4uiEXT (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  _glr->glExt.glNamedProgramLocalParameterI4uiEXT (program, target, index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameterI4uivEXT (GLuint program, GLenum target, GLuint index, const GLuint *params) {
  _glr->glExt.glNamedProgramLocalParameterI4uivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParametersI4uivEXT (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint *params) {
  _glr->glExt.glNamedProgramLocalParametersI4uivEXT (program, target, index, count, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterIivEXT (GLuint program, GLenum target, GLuint index, GLint *params) {
  _glr->glExt.glGetNamedProgramLocalParameterIivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterIuivEXT (GLuint program, GLenum target, GLuint index, GLuint *params) {
  _glr->glExt.glGetNamedProgramLocalParameterIuivEXT (program, target, index, params);}
inline GLAPI void APIENTRY glEnableClientStateiEXT (GLenum array, GLuint index) {
  _glr->glExt.glEnableClientStateiEXT (array, index);}
inline GLAPI void APIENTRY glDisableClientStateiEXT (GLenum array, GLuint index) {
  _glr->glExt.glDisableClientStateiEXT (array, index);}
inline GLAPI void APIENTRY glGetFloati_vEXT (GLenum pname, GLuint index, GLfloat *params) {
  _glr->glExt.glGetFloati_vEXT (pname, index, params);}
inline GLAPI void APIENTRY glGetDoublei_vEXT (GLenum pname, GLuint index, GLdouble *params) {
  _glr->glExt.glGetDoublei_vEXT (pname, index, params);}
inline GLAPI void APIENTRY glGetPointeri_vEXT (GLenum pname, GLuint index, void **params) {
  _glr->glExt.glGetPointeri_vEXT (pname, index, params);}
inline GLAPI void APIENTRY glNamedProgramStringEXT (GLuint program, GLenum target, GLenum format, GLsizei len, const void *string) {
  _glr->glExt.glNamedProgramStringEXT (program, target, format, len, string);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4dEXT (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glNamedProgramLocalParameter4dEXT (program, target, index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4dvEXT (GLuint program, GLenum target, GLuint index, const GLdouble *params) {
  _glr->glExt.glNamedProgramLocalParameter4dvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4fEXT (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glNamedProgramLocalParameter4fEXT (program, target, index, x, y, z, w);}
inline GLAPI void APIENTRY glNamedProgramLocalParameter4fvEXT (GLuint program, GLenum target, GLuint index, const GLfloat *params) {
  _glr->glExt.glNamedProgramLocalParameter4fvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterdvEXT (GLuint program, GLenum target, GLuint index, GLdouble *params) {
  _glr->glExt.glGetNamedProgramLocalParameterdvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramLocalParameterfvEXT (GLuint program, GLenum target, GLuint index, GLfloat *params) {
  _glr->glExt.glGetNamedProgramLocalParameterfvEXT (program, target, index, params);}
inline GLAPI void APIENTRY glGetNamedProgramivEXT (GLuint program, GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetNamedProgramivEXT (program, target, pname, params);}
inline GLAPI void APIENTRY glGetNamedProgramStringEXT (GLuint program, GLenum target, GLenum pname, void *string) {
  _glr->glExt.glGetNamedProgramStringEXT (program, target, pname, string);}
inline GLAPI void APIENTRY glNamedRenderbufferStorageEXT (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glNamedRenderbufferStorageEXT (renderbuffer, internalformat, width, height);}
inline GLAPI void APIENTRY glGetNamedRenderbufferParameterivEXT (GLuint renderbuffer, GLenum pname, GLint *params) {
  _glr->glExt.glGetNamedRenderbufferParameterivEXT (renderbuffer, pname, params);}
inline GLAPI void APIENTRY glNamedRenderbufferStorageMultisampleEXT (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glNamedRenderbufferStorageMultisampleEXT (renderbuffer, samples, internalformat, width, height);}
inline GLAPI void APIENTRY glNamedRenderbufferStorageMultisampleCoverageEXT (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glNamedRenderbufferStorageMultisampleCoverageEXT (renderbuffer, coverageSamples, colorSamples, internalformat, width, height);}
inline GLAPI GLenum APIENTRY glCheckNamedFramebufferStatusEXT (GLuint framebuffer, GLenum target) {
  return _glr->glExt.glCheckNamedFramebufferStatusEXT (framebuffer, target);}
inline GLAPI void APIENTRY glNamedFramebufferTexture1DEXT (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  _glr->glExt.glNamedFramebufferTexture1DEXT (framebuffer, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glNamedFramebufferTexture2DEXT (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  _glr->glExt.glNamedFramebufferTexture2DEXT (framebuffer, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glNamedFramebufferTexture3DEXT (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
  _glr->glExt.glNamedFramebufferTexture3DEXT (framebuffer, attachment, textarget, texture, level, zoffset);}
inline GLAPI void APIENTRY glNamedFramebufferRenderbufferEXT (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
  _glr->glExt.glNamedFramebufferRenderbufferEXT (framebuffer, attachment, renderbuffertarget,  renderbuffer);}
inline GLAPI void APIENTRY glGetNamedFramebufferAttachmentParameterivEXT (GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params) {
  _glr->glExt.glGetNamedFramebufferAttachmentParameterivEXT (framebuffer, attachment, pname, params);}
inline GLAPI void APIENTRY glGenerateTextureMipmapEXT (GLuint texture, GLenum target) {
  _glr->glExt.glGenerateTextureMipmapEXT (texture, target);}
inline GLAPI void APIENTRY glGenerateMultiTexMipmapEXT (GLenum texunit, GLenum target) {
  _glr->glExt.glGenerateMultiTexMipmapEXT (texunit, target);}
inline GLAPI void APIENTRY glFramebufferDrawBufferEXT (GLuint framebuffer, GLenum mode) {
  _glr->glExt.glFramebufferDrawBufferEXT (framebuffer, mode);}
inline GLAPI void APIENTRY glFramebufferDrawBuffersEXT (GLuint framebuffer, GLsizei n, const GLenum *bufs) {
  _glr->glExt.glFramebufferDrawBuffersEXT (framebuffer, n, bufs);}
inline GLAPI void APIENTRY glFramebufferReadBufferEXT (GLuint framebuffer, GLenum mode) {
  _glr->glExt.glFramebufferReadBufferEXT (framebuffer,  mode);}
inline GLAPI void APIENTRY glGetFramebufferParameterivEXT (GLuint framebuffer, GLenum pname, GLint *params) {
  _glr->glExt.glGetFramebufferParameterivEXT (framebuffer, pname, params);}
inline GLAPI void APIENTRY glNamedCopyBufferSubDataEXT (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
  _glr->glExt.glNamedCopyBufferSubDataEXT (readBuffer, writeBuffer, readOffset, writeOffset, size);}
inline GLAPI void APIENTRY glNamedFramebufferTextureEXT (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level) {
  _glr->glExt.glNamedFramebufferTextureEXT (framebuffer, attachment, texture, level);}
inline GLAPI void APIENTRY glNamedFramebufferTextureLayerEXT (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  _glr->glExt.glNamedFramebufferTextureLayerEXT (framebuffer, attachment, texture, level, layer);}
inline GLAPI void APIENTRY glNamedFramebufferTextureFaceEXT (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face) {
  _glr->glExt.glNamedFramebufferTextureFaceEXT (framebuffer, attachment, texture, level, face);}
inline GLAPI void APIENTRY glTextureRenderbufferEXT (GLuint texture, GLenum target, GLuint renderbuffer) {
  _glr->glExt.glTextureRenderbufferEXT (texture, target, renderbuffer);}
inline GLAPI void APIENTRY glMultiTexRenderbufferEXT (GLenum texunit, GLenum target, GLuint renderbuffer) {
  _glr->glExt.glMultiTexRenderbufferEXT (texunit, target, renderbuffer);}
inline GLAPI void APIENTRY glVertexArrayVertexOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayVertexOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayColorOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayColorOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayEdgeFlagOffsetEXT (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayEdgeFlagOffsetEXT (vaobj, buffer, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayIndexOffsetEXT (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayIndexOffsetEXT (vaobj, buffer, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayNormalOffsetEXT (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayNormalOffsetEXT (vaobj, buffer, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayTexCoordOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayTexCoordOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayMultiTexCoordOffsetEXT (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayMultiTexCoordOffsetEXT ( vaobj, buffer, texunit, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayFogCoordOffsetEXT (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayFogCoordOffsetEXT (vaobj, buffer, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArraySecondaryColorOffsetEXT (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArraySecondaryColorOffsetEXT (vaobj, buffer, size, type, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribOffsetEXT (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayVertexAttribOffsetEXT (vaobj, buffer, index, size, type, normalized, stride, offset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribIOffsetEXT (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayVertexAttribIOffsetEXT (vaobj, buffer, index, size, type, stride, offset);}
inline GLAPI void APIENTRY glEnableVertexArrayEXT (GLuint vaobj, GLenum array) {
  _glr->glExt.glEnableVertexArrayEXT (vaobj, array);}
inline GLAPI void APIENTRY glDisableVertexArrayEXT (GLuint vaobj, GLenum array) {
  _glr->glExt.glDisableVertexArrayEXT (vaobj, array);}
inline GLAPI void APIENTRY glEnableVertexArrayAttribEXT (GLuint vaobj, GLuint index) {
  _glr->glExt.glEnableVertexArrayAttribEXT (vaobj, index);}
inline GLAPI void APIENTRY glDisableVertexArrayAttribEXT (GLuint vaobj, GLuint index) {
  _glr->glExt.glDisableVertexArrayAttribEXT (vaobj,  index);}
inline GLAPI void APIENTRY glGetVertexArrayIntegervEXT (GLuint vaobj, GLenum pname, GLint *param) {
  _glr->glExt.glGetVertexArrayIntegervEXT (vaobj, pname, param);}
inline GLAPI void APIENTRY glGetVertexArrayPointervEXT (GLuint vaobj, GLenum pname, void **param) {
  _glr->glExt.glGetVertexArrayPointervEXT (vaobj, pname, param);}
inline GLAPI void APIENTRY glGetVertexArrayIntegeri_vEXT (GLuint vaobj, GLuint index, GLenum pname, GLint *param) {
  _glr->glExt.glGetVertexArrayIntegeri_vEXT (vaobj, index, pname, param);}
inline GLAPI void APIENTRY glGetVertexArrayPointeri_vEXT (GLuint vaobj, GLuint index, GLenum pname, void **param) {
  _glr->glExt.glGetVertexArrayPointeri_vEXT (vaobj, index, pname, param);}
inline GLAPI void *APIENTRY glMapNamedBufferRangeEXT (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) {
  return _glr->glExt.glMapNamedBufferRangeEXT (buffer, offset, length, access);}
inline GLAPI void APIENTRY glFlushMappedNamedBufferRangeEXT (GLuint buffer, GLintptr offset, GLsizeiptr length) {
  _glr->glExt.glFlushMappedNamedBufferRangeEXT (buffer, offset, length);}
inline GLAPI void APIENTRY glNamedBufferStorageEXT (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags) {
  _glr->glExt.glNamedBufferStorageEXT (buffer, size, data, flags);}
inline GLAPI void APIENTRY glClearNamedBufferDataEXT (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glClearNamedBufferDataEXT (buffer, internalformat, format, type, data);}
inline GLAPI void APIENTRY glClearNamedBufferSubDataEXT (GLuint buffer, GLenum internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) {
  _glr->glExt.glClearNamedBufferSubDataEXT (buffer, internalformat, offset, size, format, type, data);}
inline GLAPI void APIENTRY glNamedFramebufferParameteriEXT (GLuint framebuffer, GLenum pname, GLint param) {
  _glr->glExt.glNamedFramebufferParameteriEXT (framebuffer, pname, param);}
inline GLAPI void APIENTRY glGetNamedFramebufferParameterivEXT (GLuint framebuffer, GLenum pname, GLint *params) {
  _glr->glExt.glGetNamedFramebufferParameterivEXT (framebuffer, pname, params);}
inline GLAPI void APIENTRY glProgramUniform1dEXT (GLuint program, GLint location, GLdouble x) {
  _glr->glExt.glProgramUniform1dEXT (program, location, x);}
inline GLAPI void APIENTRY glProgramUniform2dEXT (GLuint program, GLint location, GLdouble x, GLdouble y) {
  _glr->glExt.glProgramUniform2dEXT (program, location, x, y);}
inline GLAPI void APIENTRY glProgramUniform3dEXT (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glProgramUniform3dEXT (program, location, x, y, z);}
inline GLAPI void APIENTRY glProgramUniform4dEXT (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glProgramUniform4dEXT (program, location, x, y, z, w);}
inline GLAPI void APIENTRY glProgramUniform1dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform1dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform2dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform2dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform3dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform3dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniform4dvEXT (GLuint program, GLint location, GLsizei count, const GLdouble *value) {
  _glr->glExt.glProgramUniform4dvEXT (program, location, count, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix2dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix3dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix4dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x3dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix2x3dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix2x4dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix2x4dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x2dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix3x2dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix3x4dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix3x4dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x2dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix4x2dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glProgramUniformMatrix4x3dvEXT (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
  _glr->glExt.glProgramUniformMatrix4x3dvEXT (program, location, count, transpose, value);}
inline GLAPI void APIENTRY glTextureBufferRangeEXT (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  _glr->glExt.glTextureBufferRangeEXT (texture, target, internalformat, buffer, offset, size);}
inline GLAPI void APIENTRY glTextureStorage1DEXT (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) {
  _glr->glExt.glTextureStorage1DEXT (texture, target, levels, internalformat, width);}
inline GLAPI void APIENTRY glTextureStorage2DEXT (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glTextureStorage2DEXT (texture, target, levels, internalformat, width, height);}
inline GLAPI void APIENTRY glTextureStorage3DEXT (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
  _glr->glExt.glTextureStorage3DEXT (texture,  target, levels, internalformat, width, height, depth);}
inline GLAPI void APIENTRY glTextureStorage2DMultisampleEXT (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
  _glr->glExt.glTextureStorage2DMultisampleEXT (texture, target, samples, internalformat, width, height, fixedsamplelocations);}
inline GLAPI void APIENTRY glTextureStorage3DMultisampleEXT (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
  _glr->glExt.glTextureStorage3DMultisampleEXT (texture, target, samples, internalformat, width, height, depth, fixedsamplelocations);}
inline GLAPI void APIENTRY glVertexArrayBindVertexBufferEXT (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
  _glr->glExt.glVertexArrayBindVertexBufferEXT (vaobj, bindingindex, buffer, offset, stride);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribFormatEXT (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
  _glr->glExt.glVertexArrayVertexAttribFormatEXT (vaobj, attribindex, size, type, normalized, relativeoffset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribIFormatEXT (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  _glr->glExt.glVertexArrayVertexAttribIFormatEXT (vaobj, attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribLFormatEXT (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
  _glr->glExt.glVertexArrayVertexAttribLFormatEXT (vaobj, attribindex, size, type, relativeoffset);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribBindingEXT (GLuint vaobj, GLuint attribindex, GLuint bindingindex) {
  _glr->glExt.glVertexArrayVertexAttribBindingEXT (vaobj, attribindex, bindingindex);}
inline GLAPI void APIENTRY glVertexArrayVertexBindingDivisorEXT (GLuint vaobj, GLuint bindingindex, GLuint divisor) {
  _glr->glExt.glVertexArrayVertexBindingDivisorEXT (vaobj, bindingindex, divisor);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribLOffsetEXT (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset) {
  _glr->glExt.glVertexArrayVertexAttribLOffsetEXT (vaobj, buffer, index, size, type, stride, offset);}
inline GLAPI void APIENTRY glTexturePageCommitmentEXT (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean resident) {
  _glr->glExt.glTexturePageCommitmentEXT (texture, level, xoffset, yoffset, zoffset, width, height, depth, resident);}
inline GLAPI void APIENTRY glVertexArrayVertexAttribDivisorEXT (GLuint vaobj, GLuint index, GLuint divisor) {
  _glr->glExt.glVertexArrayVertexAttribDivisorEXT (vaobj, index, divisor);}
// #340 GL_EXT_draw_buffers2
inline GLAPI void APIENTRY glColorMaskIndexedEXT (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
  _glr->glExt.glColorMaskIndexedEXT (index, r, g, b, a);}
// #327 GL_EXT_draw_instanced
inline GLAPI void APIENTRY glDrawArraysInstancedEXT (GLenum mode, GLint start, GLsizei count, GLsizei primcount) {
  _glr->glExt.glDrawArraysInstancedEXT (mode, start, count, primcount);}
inline GLAPI void APIENTRY glDrawElementsInstancedEXT (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount) {
  _glr->glExt.glDrawElementsInstancedEXT (mode, count, type, indices, primcount);}
// #112 GL_EXT_draw_range_elements
inline GLAPI void APIENTRY glDrawRangeElementsEXT (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) {
  _glr->glExt.glDrawRangeElementsEXT (mode, start, end, count, type, indices);}
// #149 GL_EXT_fog_coord
inline GLAPI void APIENTRY glFogCoordfEXT (GLfloat coord) {
  _glr->glExt.glFogCoordfEXT (coord);}
inline GLAPI void APIENTRY glFogCoordfvEXT (const GLfloat *coord) {
  _glr->glExt.glFogCoordfvEXT (coord);}
inline GLAPI void APIENTRY glFogCoorddEXT (GLdouble coord) {
  _glr->glExt.glFogCoorddEXT (coord);}
inline GLAPI void APIENTRY glFogCoorddvEXT (const GLdouble *coord) {
  _glr->glExt.glFogCoorddvEXT (coord);}
inline GLAPI void APIENTRY glFogCoordPointerEXT (GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glFogCoordPointerEXT (type, stride, pointer);}
// #316 GL_EXT_framebuffer_blit
inline GLAPI void APIENTRY glBlitFramebufferEXT (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
  _glr->glExt.glBlitFramebufferEXT (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);}
// #409 GL_EXT_framebuffer_multisample
inline GLAPI void APIENTRY glRenderbufferStorageMultisampleEXT (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glRenderbufferStorageMultisampleEXT (target, samples, internalformat, width, height);}
// #310 GL_EXT_framebuffer_object
inline GLAPI GLboolean APIENTRY glIsRenderbufferEXT (GLuint renderbuffer) {
  return _glr->glExt.glIsRenderbufferEXT (renderbuffer);}
inline GLAPI void APIENTRY glBindRenderbufferEXT (GLenum target, GLuint renderbuffer) {
  _glr->glExt.glBindRenderbufferEXT (target, renderbuffer);}
inline GLAPI void APIENTRY glDeleteRenderbuffersEXT (GLsizei n, const GLuint *renderbuffers) {
  _glr->glExt.glDeleteRenderbuffersEXT (n, renderbuffers);}
inline GLAPI void APIENTRY glGenRenderbuffersEXT (GLsizei n, GLuint *renderbuffers) {
  _glr->glExt.glGenRenderbuffersEXT (n, renderbuffers);}
inline GLAPI void APIENTRY glRenderbufferStorageEXT (GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glRenderbufferStorageEXT (target, internalformat, width, height);}
inline GLAPI void APIENTRY glGetRenderbufferParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetRenderbufferParameterivEXT (target, pname, params);}
inline GLAPI GLboolean APIENTRY glIsFramebufferEXT (GLuint framebuffer) {
  return _glr->glExt.glIsFramebufferEXT (framebuffer);}
inline GLAPI void APIENTRY glBindFramebufferEXT (GLenum target, GLuint framebuffer) {
  _glr->glExt.glBindFramebufferEXT (target, framebuffer);}
inline GLAPI void APIENTRY glDeleteFramebuffersEXT (GLsizei n, const GLuint *framebuffers) {
  _glr->glExt.glDeleteFramebuffersEXT (n, framebuffers);}
inline GLAPI void APIENTRY glGenFramebuffersEXT (GLsizei n, GLuint *framebuffers) {
  _glr->glExt.glGenFramebuffersEXT (n, framebuffers);}
inline GLAPI GLenum APIENTRY glCheckFramebufferStatusEXT (GLenum target) {
  return _glr->glExt.glCheckFramebufferStatusEXT (target);}
inline GLAPI void APIENTRY glFramebufferTexture1DEXT (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  _glr->glExt.glFramebufferTexture1DEXT (target, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glFramebufferTexture2DEXT (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
  _glr->glExt.glFramebufferTexture2DEXT (target, attachment, textarget, texture, level);}
inline GLAPI void APIENTRY glFramebufferTexture3DEXT (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
  _glr->glExt.glFramebufferTexture3DEXT (target, attachment, textarget, texture, level, zoffset);}
inline GLAPI void APIENTRY glFramebufferRenderbufferEXT (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
  _glr->glExt.glFramebufferRenderbufferEXT (target, attachment, renderbuffertarget, renderbuffer);}
inline GLAPI void APIENTRY glGetFramebufferAttachmentParameterivEXT (GLenum target, GLenum attachment, GLenum pname, GLint *params) {
  _glr->glExt.glGetFramebufferAttachmentParameterivEXT (target, attachment, pname, params);}
inline GLAPI void APIENTRY glGenerateMipmapEXT (GLenum target) {
  _glr->glExt.glGenerateMipmapEXT (target);}
// #324 GL_EXT_geometry_shader4
inline GLAPI void APIENTRY glProgramParameteriEXT (GLuint program, GLenum pname, GLint value) {
  _glr->glExt.glProgramParameteriEXT (program, pname, value);}
// #320 GL_EXT_gpu_program_parameters
inline GLAPI void APIENTRY glProgramEnvParameters4fvEXT (GLenum target, GLuint index, GLsizei count, const GLfloat *params) {
  _glr->glExt.glProgramEnvParameters4fvEXT (target, index, count, params);}
inline GLAPI void APIENTRY glProgramLocalParameters4fvEXT (GLenum target, GLuint index, GLsizei count, const GLfloat *params) {
  _glr->glExt.glProgramLocalParameters4fvEXT (target, index, count, params);}
// #326 GL_EXT_gpu_shader4
inline GLAPI void APIENTRY glGetUniformuivEXT (GLuint program, GLint location, GLuint *params) {
  _glr->glExt.glGetUniformuivEXT (program, location, params);}
inline GLAPI void APIENTRY glBindFragDataLocationEXT (GLuint program, GLuint color, const GLchar *name) {
  _glr->glExt.glBindFragDataLocationEXT (program, color, name);}
inline GLAPI GLint APIENTRY glGetFragDataLocationEXT (GLuint program, const GLchar *name) {
  return _glr->glExt.glGetFragDataLocationEXT (program, name);}
inline GLAPI void APIENTRY glUniform1uiEXT (GLint location, GLuint v0) {
  _glr->glExt.glUniform1uiEXT (location, v0);}
inline GLAPI void APIENTRY glUniform2uiEXT (GLint location, GLuint v0, GLuint v1) {
  _glr->glExt.glUniform2uiEXT (location,  v0, v1);}
inline GLAPI void APIENTRY glUniform3uiEXT (GLint location, GLuint v0, GLuint v1, GLuint v2) {
  _glr->glExt.glUniform3uiEXT (location, v0, v1, v2);}
inline GLAPI void APIENTRY glUniform4uiEXT (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
  _glr->glExt.glUniform4uiEXT (location, v0, v1, v2, v3);}
inline GLAPI void APIENTRY glUniform1uivEXT (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform1uivEXT (location, count, value);}
inline GLAPI void APIENTRY glUniform2uivEXT (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform2uivEXT (location, count, value);}
inline GLAPI void APIENTRY glUniform3uivEXT (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform3uivEXT (location,  count, value);}
inline GLAPI void APIENTRY glUniform4uivEXT (GLint location, GLsizei count, const GLuint *value) {
  _glr->glExt.glUniform4uivEXT (location, count, value);}
// #011 GL_EXT_histogram
inline GLAPI void APIENTRY glGetHistogramParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetHistogramParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetHistogramParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetHistogramParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmaxEXT (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values) {
  _glr->glExt.glGetMinmaxEXT (target, reset, format, type, values);}
inline GLAPI void APIENTRY glGetMinmaxParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMinmaxParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetMinmaxParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetMinmaxParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glHistogramEXT (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink) {
  _glr->glExt.glHistogramEXT (target, width, internalformat, sink);}
inline GLAPI void APIENTRY glMinmaxEXT (GLenum target, GLenum internalformat, GLboolean sink) {
  _glr->glExt.glMinmaxEXT (target, internalformat, sink);}
inline GLAPI void APIENTRY glResetHistogramEXT (GLenum target) {
  _glr->glExt.glResetHistogramEXT (target);}
inline GLAPI void APIENTRY glResetMinmaxEXT (GLenum target) {
  _glr->glExt.glResetMinmaxEXT (target);}
// #095 GL_EXT_index_func
inline GLAPI void APIENTRY glIndexFuncEXT (GLenum func, GLclampf ref) {
  _glr->glExt.glIndexFuncEXT (func, ref);}
// #094 GL_EXT_index_material
inline GLAPI void APIENTRY glIndexMaterialEXT (GLenum face, GLenum mode) {
  _glr->glExt.glIndexMaterialEXT (face, mode);}
// #117 GL_EXT_light_texture
inline GLAPI void APIENTRY glApplyTextureEXT (GLenum mode) {
  _glr->glExt.glApplyTextureEXT (mode);}
inline GLAPI void APIENTRY glTextureLightEXT (GLenum pname) {
  _glr->glExt.glTextureLightEXT (pname);}
inline GLAPI void APIENTRY glTextureMaterialEXT (GLenum face, GLenum mode) {
  _glr->glExt.glTextureMaterialEXT (face, mode);}
// #148 GL_EXT_multi_draw_arrays
inline GLAPI void APIENTRY glMultiDrawArraysEXT (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount) {
  _glr->glExt.glMultiDrawArraysEXT (mode, first, count, primcount);}
inline GLAPI void APIENTRY glMultiDrawElementsEXT (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount) {
  _glr->glExt.glMultiDrawElementsEXT (mode, count, type, indices, primcount);}
// #209 GL_EXT_multisample
inline GLAPI void APIENTRY glSampleMaskEXT (GLclampf value, GLboolean invert) {
  _glr->glExt.glSampleMaskEXT (value, invert);}
inline GLAPI void APIENTRY glSamplePatternEXT (GLenum pattern) {
  _glr->glExt.glSamplePatternEXT (pattern);}
// #078 GL_EXT_paletted_texture
inline GLAPI void APIENTRY glColorTableEXT (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void *table) {
  _glr->glExt.glColorTableEXT (target, internalFormat, width, format, type, table);}
inline GLAPI void APIENTRY glGetColorTableEXT (GLenum target, GLenum format, GLenum type, void *data) {
  _glr->glExt.glGetColorTableEXT (target, format, type, data);}
inline GLAPI void APIENTRY glGetColorTableParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetColorTableParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetColorTableParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetColorTableParameterfvEXT (target, pname, params);}
// #138 GL_EXT_pixel_transform
inline GLAPI void APIENTRY glPixelTransformParameteriEXT (GLenum target, GLenum pname, GLint param) {
  _glr->glExt.glPixelTransformParameteriEXT (target, pname, param);}
inline GLAPI void APIENTRY glPixelTransformParameterfEXT (GLenum target, GLenum pname, GLfloat param) {
  _glr->glExt.glPixelTransformParameterfEXT (target, pname, param);}
inline GLAPI void APIENTRY glPixelTransformParameterivEXT (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glPixelTransformParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glPixelTransformParameterfvEXT (GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glPixelTransformParameterfvEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetPixelTransformParameterivEXT (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetPixelTransformParameterivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetPixelTransformParameterfvEXT (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetPixelTransformParameterfvEXT (target, pname, params);}
// #054 GL_EXT_point_parameters
inline GLAPI void APIENTRY glPointParameterfEXT (GLenum pname, GLfloat param) {
  _glr->glExt.glPointParameterfEXT (pname, param);}
inline GLAPI void APIENTRY glPointParameterfvEXT (GLenum pname, const GLfloat *params) {
  _glr->glExt.glPointParameterfvEXT (pname, params);}
// #003 GL_EXT_polygon_offset
inline GLAPI void APIENTRY glPolygonOffsetEXT (GLfloat factor, GLfloat bias) {
  _glr->glExt.glPolygonOffsetEXT (factor, bias);}
// #364 GL_EXT_provoking_vertex
inline GLAPI void APIENTRY glProvokingVertexEXT (GLenum mode) {
  _glr->glExt.glProvokingVertexEXT (mode);}
// #145 GL_EXT_secondary_color
inline GLAPI void APIENTRY glSecondaryColor3bEXT (GLbyte red, GLbyte green, GLbyte blue) {
  _glr->glExt.glSecondaryColor3bEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3bvEXT (const GLbyte *v) {
  _glr->glExt.glSecondaryColor3bvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3dEXT (GLdouble red, GLdouble green, GLdouble blue) {
  _glr->glExt.glSecondaryColor3dEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3dvEXT (const GLdouble *v) {
  _glr->glExt.glSecondaryColor3dvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3fEXT (GLfloat red, GLfloat green, GLfloat blue) {
  _glr->glExt.glSecondaryColor3fEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3fvEXT (const GLfloat *v) {
  _glr->glExt.glSecondaryColor3fvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3iEXT (GLint red, GLint green, GLint blue) {
  _glr->glExt.glSecondaryColor3iEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3ivEXT (const GLint *v) {
  _glr->glExt.glSecondaryColor3ivEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3sEXT (GLshort red, GLshort green, GLshort blue) {
  _glr->glExt.glSecondaryColor3sEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3svEXT (const GLshort *v) {
  _glr->glExt.glSecondaryColor3svEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3ubEXT (GLubyte red, GLubyte green, GLubyte blue) {
  _glr->glExt.glSecondaryColor3ubEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3ubvEXT (const GLubyte *v) {
  _glr->glExt.glSecondaryColor3ubvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3uiEXT (GLuint red, GLuint green, GLuint blue) {
  _glr->glExt.glSecondaryColor3uiEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3uivEXT (const GLuint *v) {
  _glr->glExt.glSecondaryColor3uivEXT (v);}
inline GLAPI void APIENTRY glSecondaryColor3usEXT (GLushort red, GLushort green, GLushort blue) {
  _glr->glExt.glSecondaryColor3usEXT (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3usvEXT (const GLushort *v) {
  _glr->glExt.glSecondaryColor3usvEXT (v);}
inline GLAPI void APIENTRY glSecondaryColorPointerEXT (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glSecondaryColorPointerEXT (size, type, stride, pointer);}
// #377 GL_EXT_separate_shader_objects
inline GLAPI void APIENTRY glUseShaderProgramEXT (GLenum type, GLuint program) {
  _glr->glExt.glUseShaderProgramEXT (type, program);}
inline GLAPI void APIENTRY glActiveProgramEXT (GLuint program) {
  _glr->glExt.glActiveProgramEXT (program);}
inline GLAPI GLuint APIENTRY glCreateShaderProgramEXT (GLenum type, const GLchar *string) {
  return _glr->glExt.glCreateShaderProgramEXT (type, string);}
// #386 GL_EXT_shader_image_load_store
inline GLAPI void APIENTRY glBindImageTextureEXT (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format) {
  _glr->glExt.glBindImageTextureEXT (index, texture, level, layered, layer, access, format);}
inline GLAPI void APIENTRY glMemoryBarrierEXT (GLbitfield barriers) {
  _glr->glExt.glMemoryBarrierEXT (barriers);}
// #314 GL_EXT_stencil_clear_tag
inline GLAPI void APIENTRY glStencilClearTagEXT (GLsizei stencilTagBits, GLuint stencilClearTag) {
  _glr->glExt.glStencilClearTagEXT (stencilTagBits, stencilClearTag);}
// #268 GL_EXT_stencil_two_side
inline GLAPI void APIENTRY glActiveStencilFaceEXT (GLenum face) {
  _glr->glExt.glActiveStencilFaceEXT (face);}
// #009 GL_EXT_subtexture
inline GLAPI void APIENTRY glTexSubImage1DEXT (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexSubImage1DEXT (target, level, xoffset, width, format, type, pixels);}
inline GLAPI void APIENTRY glTexSubImage2DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexSubImage2DEXT (target, level, xoffset, yoffset, width, height, format, type, pixels);}
// #006 GL_EXT_texture3D
inline GLAPI void APIENTRY glTexImage3DEXT (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexImage3DEXT (target, level, internalformat, width, height, depth, border, format, type, pixels);}
inline GLAPI void APIENTRY glTexSubImage3DEXT (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexSubImage3DEXT (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);}
// #330 GL_EXT_texture_buffer_object
inline GLAPI void APIENTRY glTexBufferEXT (GLenum target, GLenum internalformat, GLuint buffer) {
  _glr->glExt.glTexBufferEXT (target, internalformat, buffer);}
// #343 GL_EXT_texture_integer
inline GLAPI void APIENTRY glTexParameterIivEXT (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glTexParameterIivEXT (target, pname, params);}
inline GLAPI void APIENTRY glTexParameterIuivEXT (GLenum target, GLenum pname, const GLuint *params) {
  _glr->glExt.glTexParameterIuivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIivEXT (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetTexParameterIivEXT (target, pname, params);}
inline GLAPI void APIENTRY glGetTexParameterIuivEXT (GLenum target, GLenum pname, GLuint *params) {
  _glr->glExt.glGetTexParameterIuivEXT (target, pname, params);}
inline GLAPI void APIENTRY glClearColorIiEXT (GLint red, GLint green, GLint blue, GLint alpha) {
  _glr->glExt.glClearColorIiEXT (red, green, blue, alpha);}
inline GLAPI void APIENTRY glClearColorIuiEXT (GLuint red, GLuint green, GLuint blue, GLuint alpha) {
  _glr->glExt.glClearColorIuiEXT (red, green, blue, alpha);}
// #020 GL_EXT_texture_object
inline GLAPI GLboolean APIENTRY glAreTexturesResidentEXT (GLsizei n, const GLuint *textures, GLboolean *residences) {
  return _glr->glExt.glAreTexturesResidentEXT (n, textures, residences);}
inline GLAPI void APIENTRY glBindTextureEXT (GLenum target, GLuint texture) {
  _glr->glExt.glBindTextureEXT (target, texture);}
inline GLAPI void APIENTRY glDeleteTexturesEXT (GLsizei n, const GLuint *textures) {
  _glr->glExt.glDeleteTexturesEXT (n, textures);}
inline GLAPI void APIENTRY glGenTexturesEXT (GLsizei n, GLuint *textures) {
  _glr->glExt.glGenTexturesEXT (n, textures);}
inline GLAPI GLboolean APIENTRY glIsTextureEXT (GLuint texture) {
  return _glr->glExt.glIsTextureEXT (texture);}
inline GLAPI void APIENTRY glPrioritizeTexturesEXT (GLsizei n, const GLuint *textures, const GLclampf *priorities) {
  _glr->glExt.glPrioritizeTexturesEXT (n, textures, priorities);}
// #147 GL_EXT_texture_perturb_normal
inline GLAPI void APIENTRY glTextureNormalEXT (GLenum mode) {
  _glr->glExt.glTextureNormalEXT (mode);}
// #319 GL_EXT_timer_query
inline GLAPI void APIENTRY glGetQueryObjecti64vEXT (GLuint id, GLenum pname, GLint64 *params) {
  _glr->glExt.glGetQueryObjecti64vEXT (id, pname, params);}
inline GLAPI void APIENTRY glGetQueryObjectui64vEXT (GLuint id, GLenum pname, GLuint64 *params) {
  _glr->glExt.glGetQueryObjectui64vEXT (id, pname, params);}
// #352 GL_EXT_transform_feedback
inline GLAPI void APIENTRY glBeginTransformFeedbackEXT (GLenum primitiveMode) {
  _glr->glExt.glBeginTransformFeedbackEXT (primitiveMode);}
inline GLAPI void APIENTRY glEndTransformFeedbackEXT (void) {
  _glr->glExt.glEndTransformFeedbackEXT ();}
inline GLAPI void APIENTRY glBindBufferRangeEXT (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  _glr->glExt.glBindBufferRangeEXT (target, index, buffer, offset, size);}
inline GLAPI void APIENTRY glBindBufferOffsetEXT (GLenum target, GLuint index, GLuint buffer, GLintptr offset) {
  _glr->glExt.glBindBufferOffsetEXT (target, index, buffer, offset);}
inline GLAPI void APIENTRY glBindBufferBaseEXT (GLenum target, GLuint index, GLuint buffer) {
  _glr->glExt.glBindBufferBaseEXT (target, index,  buffer);}
inline GLAPI void APIENTRY glTransformFeedbackVaryingsEXT (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode) {
  _glr->glExt.glTransformFeedbackVaryingsEXT (program, count, varyings, bufferMode);}
inline GLAPI void APIENTRY glGetTransformFeedbackVaryingEXT (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
  _glr->glExt.glGetTransformFeedbackVaryingEXT (program, index, bufSize, length, size, type, name);}
// #030 GL_EXT_vertex_array
inline GLAPI void APIENTRY glArrayElementEXT (GLint i) {
  _glr->glExt.glArrayElementEXT (i);}
inline GLAPI void APIENTRY glColorPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  _glr->glExt.glColorPointerEXT (size, type, stride, count, pointer);}
inline GLAPI void APIENTRY glDrawArraysEXT (GLenum mode, GLint first, GLsizei count) {
  _glr->glExt.glDrawArraysEXT (mode, first, count);}
inline GLAPI void APIENTRY glEdgeFlagPointerEXT (GLsizei stride, GLsizei count, const GLboolean *pointer) {
  _glr->glExt.glEdgeFlagPointerEXT (stride, count, pointer);}
inline GLAPI void APIENTRY glGetPointervEXT (GLenum pname, void **params) {
  _glr->glExt.glGetPointervEXT (pname, params);}
inline GLAPI void APIENTRY glIndexPointerEXT (GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  _glr->glExt.glIndexPointerEXT (type, stride, count, pointer);}
inline GLAPI void APIENTRY glNormalPointerEXT (GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  _glr->glExt.glNormalPointerEXT (type, stride, count, pointer);}
inline GLAPI void APIENTRY glTexCoordPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  _glr->glExt.glTexCoordPointerEXT (size, type, stride, count, pointer);}
inline GLAPI void APIENTRY glVertexPointerEXT (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer) {
  _glr->glExt.glVertexPointerEXT (size, type, stride, count, pointer);}
// #387 GL_EXT_vertex_attrib_64bit
inline GLAPI void APIENTRY glVertexAttribL1dEXT (GLuint index, GLdouble x) {
  _glr->glExt.glVertexAttribL1dEXT (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2dEXT (GLuint index, GLdouble x, GLdouble y) {
  _glr->glExt.glVertexAttribL2dEXT (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3dEXT (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glVertexAttribL3dEXT (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4dEXT (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glVertexAttribL4dEXT (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1dvEXT (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL1dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2dvEXT (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL2dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3dvEXT (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL3dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4dvEXT (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttribL4dvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribLPointerEXT (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexAttribLPointerEXT (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribLdvEXT (GLuint index, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetVertexAttribLdvEXT (index, pname, params);}
// #248 GL_EXT_vertex_shader
inline GLAPI void APIENTRY glBeginVertexShaderEXT (void) {
  _glr->glExt.glBeginVertexShaderEXT ();}
inline GLAPI void APIENTRY glEndVertexShaderEXT (void) {
  _glr->glExt.glEndVertexShaderEXT ();}
inline GLAPI void APIENTRY glBindVertexShaderEXT (GLuint id) {
  _glr->glExt.glBindVertexShaderEXT (id);}
inline GLAPI GLuint APIENTRY glGenVertexShadersEXT (GLuint range) {
  return _glr->glExt.glGenVertexShadersEXT (range);}
inline GLAPI void APIENTRY glDeleteVertexShaderEXT (GLuint id) {
  _glr->glExt.glDeleteVertexShaderEXT (id);}
inline GLAPI void APIENTRY glShaderOp1EXT (GLenum op, GLuint res, GLuint arg1) {
  _glr->glExt.glShaderOp1EXT (op, res, arg1);}
inline GLAPI void APIENTRY glShaderOp2EXT (GLenum op, GLuint res, GLuint arg1, GLuint arg2) {
  _glr->glExt.glShaderOp2EXT (op, res, arg1, arg2);}
inline GLAPI void APIENTRY glShaderOp3EXT (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3) {
  _glr->glExt.glShaderOp3EXT (op, res, arg1, arg2, arg3);}
inline GLAPI void APIENTRY glSwizzleEXT (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW) {
  _glr->glExt.glSwizzleEXT (res, in, outX, outY, outZ, outW);}
inline GLAPI void APIENTRY glWriteMaskEXT (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW) {
  _glr->glExt.glWriteMaskEXT (res, in, outX, outY, outZ, outW);}
inline GLAPI void APIENTRY glInsertComponentEXT (GLuint res, GLuint src, GLuint num) {
  _glr->glExt.glInsertComponentEXT (res, src, num);}
inline GLAPI void APIENTRY glExtractComponentEXT (GLuint res, GLuint src, GLuint num) {
  _glr->glExt.glExtractComponentEXT (res, src, num);}
inline GLAPI GLuint APIENTRY glGenSymbolsEXT (GLenum datatype, GLenum storagetype, GLenum range, GLuint components) {
  return _glr->glExt.glGenSymbolsEXT (datatype, storagetype, range, components);}
inline GLAPI void APIENTRY glSetInvariantEXT (GLuint id, GLenum type, const void *addr) {
  _glr->glExt.glSetInvariantEXT (id, type, addr);}
inline GLAPI void APIENTRY glSetLocalConstantEXT (GLuint id, GLenum type, const void *addr) {
  _glr->glExt.glSetLocalConstantEXT (id, type, addr);}
inline GLAPI void APIENTRY glVariantbvEXT (GLuint id, const GLbyte *addr) {
  _glr->glExt.glVariantbvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantsvEXT (GLuint id, const GLshort *addr) {
  _glr->glExt.glVariantsvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantivEXT (GLuint id, const GLint *addr) {
  _glr->glExt.glVariantivEXT (id, addr);}
inline GLAPI void APIENTRY glVariantfvEXT (GLuint id, const GLfloat *addr) {
  _glr->glExt.glVariantfvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantdvEXT (GLuint id, const GLdouble *addr) {
  _glr->glExt.glVariantdvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantubvEXT (GLuint id, const GLubyte *addr) {
  _glr->glExt.glVariantubvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantusvEXT (GLuint id, const GLushort *addr) {
  _glr->glExt.glVariantusvEXT (id, addr);}
inline GLAPI void APIENTRY glVariantuivEXT (GLuint id, const GLuint *addr) {
  _glr->glExt.glVariantuivEXT (id, addr);}
inline GLAPI void APIENTRY glVariantPointerEXT (GLuint id, GLenum type, GLuint stride, const void *addr) {
  _glr->glExt.glVariantPointerEXT (id, type, stride, addr);}
inline GLAPI void APIENTRY glEnableVariantClientStateEXT (GLuint id) {
  _glr->glExt.glEnableVariantClientStateEXT (id);}
inline GLAPI void APIENTRY glDisableVariantClientStateEXT (GLuint id) {
  _glr->glExt.glDisableVariantClientStateEXT (id);}
inline GLAPI GLuint APIENTRY glBindLightParameterEXT (GLenum light, GLenum value) {
  return _glr->glExt.glBindLightParameterEXT (light,  value);}
inline GLAPI GLuint APIENTRY glBindMaterialParameterEXT (GLenum face, GLenum value) {
  return _glr->glExt.glBindMaterialParameterEXT (face, value);}
inline GLAPI GLuint APIENTRY glBindTexGenParameterEXT (GLenum unit, GLenum coord, GLenum value) {
  return _glr->glExt.glBindTexGenParameterEXT (unit, coord,  value);}
inline GLAPI GLuint APIENTRY glBindTextureUnitParameterEXT (GLenum unit, GLenum value) {
  return _glr->glExt.glBindTextureUnitParameterEXT (unit, value);}
inline GLAPI GLuint APIENTRY glBindParameterEXT (GLenum value) {
  return _glr->glExt.glBindParameterEXT (value);}
inline GLAPI GLboolean APIENTRY glIsVariantEnabledEXT (GLuint id, GLenum cap) {
  return _glr->glExt.glIsVariantEnabledEXT (id, cap);}
inline GLAPI void APIENTRY glGetVariantBooleanvEXT (GLuint id, GLenum value, GLboolean *data) {
  _glr->glExt.glGetVariantBooleanvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetVariantIntegervEXT (GLuint id, GLenum value, GLint *data) {
  _glr->glExt.glGetVariantIntegervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetVariantFloatvEXT (GLuint id, GLenum value, GLfloat *data) {
  _glr->glExt.glGetVariantFloatvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetVariantPointervEXT (GLuint id, GLenum value, void **data) {
  _glr->glExt.glGetVariantPointervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetInvariantBooleanvEXT (GLuint id, GLenum value, GLboolean *data) {
  _glr->glExt.glGetInvariantBooleanvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetInvariantIntegervEXT (GLuint id, GLenum value, GLint *data) {
  _glr->glExt.glGetInvariantIntegervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetInvariantFloatvEXT (GLuint id, GLenum value, GLfloat *data) {
  _glr->glExt.glGetInvariantFloatvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetLocalConstantBooleanvEXT (GLuint id, GLenum value, GLboolean *data) {
  _glr->glExt.glGetLocalConstantBooleanvEXT (id, value, data);}
inline GLAPI void APIENTRY glGetLocalConstantIntegervEXT (GLuint id, GLenum value, GLint *data) {
  _glr->glExt.glGetLocalConstantIntegervEXT (id, value, data);}
inline GLAPI void APIENTRY glGetLocalConstantFloatvEXT (GLuint id, GLenum value, GLfloat *data) {
  _glr->glExt.glGetLocalConstantFloatvEXT (id, value, data);}
// #188 GL_EXT_vertex_weighting
inline GLAPI void APIENTRY glVertexWeightfEXT (GLfloat weight) {
  _glr->glExt.glVertexWeightfEXT (weight);}
inline GLAPI void APIENTRY glVertexWeightfvEXT (const GLfloat *weight) {
  _glr->glExt.glVertexWeightfvEXT (weight);}
inline GLAPI void APIENTRY glVertexWeightPointerEXT (GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexWeightPointerEXT (size, type, stride, pointer);}
// #406 GL_EXT_x11_sync_object
inline GLAPI GLsync APIENTRY glImportSyncEXT (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags) {
  return _glr->glExt.glImportSyncEXT (external_sync_type, external_sync, flags);}
// #345 GL_GREMEDY_frame_terminator
inline GLAPI void APIENTRY glFrameTerminatorGREMEDY (void) {
  _glr->glExt.glFrameTerminatorGREMEDY ();}
// #311 GL_GREMEDY_string_marker
inline GLAPI void APIENTRY glStringMarkerGREMEDY (GLsizei len, const void *string) {
  _glr->glExt.glStringMarkerGREMEDY (len, string);}
// #066 GL_HP_image_transform
inline GLAPI void APIENTRY glImageTransformParameteriHP (GLenum target, GLenum pname, GLint param) {
  _glr->glExt.glImageTransformParameteriHP (target, pname, param);}
inline GLAPI void APIENTRY glImageTransformParameterfHP (GLenum target, GLenum pname, GLfloat param) {
  _glr->glExt.glImageTransformParameterfHP (target, pname, param);}
inline GLAPI void APIENTRY glImageTransformParameterivHP (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glImageTransformParameterivHP (target, pname, params);}
inline GLAPI void APIENTRY glImageTransformParameterfvHP (GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glImageTransformParameterfvHP (target, pname, params);}
inline GLAPI void APIENTRY glGetImageTransformParameterivHP (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetImageTransformParameterivHP (target, pname, params);}
inline GLAPI void APIENTRY glGetImageTransformParameterfvHP (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetImageTransformParameterfvHP (target, pname, params);}
// #200 GL_IBM_multimode_draw_arrays
inline GLAPI void APIENTRY glMultiModeDrawArraysIBM (const GLenum *mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride) {
  _glr->glExt.glMultiModeDrawArraysIBM (mode, first, count, primcount, modestride);}
inline GLAPI void APIENTRY glMultiModeDrawElementsIBM (const GLenum *mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount, GLint modestride) {
  _glr->glExt.glMultiModeDrawElementsIBM (mode, count, type, indices, primcount, modestride);}
// #223 GL_IBM_static_data
inline GLAPI void APIENTRY glFlushStaticDataIBM (GLenum target) {
  _glr->glExt.glFlushStaticDataIBM (target);}
// #201 GL_IBM_vertex_array_lists
inline GLAPI void APIENTRY glColorPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  _glr->glExt.glColorPointerListIBM (size, type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glSecondaryColorPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  _glr->glExt.glSecondaryColorPointerListIBM (size, type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glEdgeFlagPointerListIBM (GLint stride, const GLboolean **pointer, GLint ptrstride) {
  _glr->glExt.glEdgeFlagPointerListIBM (stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glFogCoordPointerListIBM (GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  _glr->glExt.glFogCoordPointerListIBM (type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glIndexPointerListIBM (GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  _glr->glExt.glIndexPointerListIBM (type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glNormalPointerListIBM (GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  _glr->glExt.glNormalPointerListIBM (type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glTexCoordPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  _glr->glExt.glTexCoordPointerListIBM (size, type, stride, pointer, ptrstride);}
inline GLAPI void APIENTRY glVertexPointerListIBM (GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride) {
  _glr->glExt.glVertexPointerListIBM (size, type, stride, pointer, ptrstride);}
// GL_INGR_blend_func_separate - not in any list
inline GLAPI void APIENTRY glBlendFuncSeparateINGR (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
  _glr->glExt.glBlendFuncSeparateINGR (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);}
// #429 GL_INTEL_map_texture
inline GLAPI void APIENTRY glSyncTextureINTEL (GLuint texture) {
  _glr->glExt.glSyncTextureINTEL (texture);}
inline GLAPI void APIENTRY glUnmapTexture2DINTEL (GLuint texture, GLint level) {
  _glr->glExt.glUnmapTexture2DINTEL (texture, level);}
inline GLAPI void *APIENTRY glMapTexture2DINTEL (GLuint texture, GLint level, GLbitfield access, GLint *stride, GLenum *layout) {
  return _glr->glExt.glMapTexture2DINTEL (texture, level, access, stride, layout);}
// #136 GL_INTEL_parallel_arrays
inline GLAPI void APIENTRY glVertexPointervINTEL (GLint size, GLenum type, const void **pointer) {
  _glr->glExt.glVertexPointervINTEL (size, type, pointer);}
inline GLAPI void APIENTRY glNormalPointervINTEL (GLenum type, const void **pointer) {
  _glr->glExt.glNormalPointervINTEL (type, pointer);}
inline GLAPI void APIENTRY glColorPointervINTEL (GLint size, GLenum type, const void **pointer) {
  _glr->glExt.glColorPointervINTEL (size, type, pointer);}
inline GLAPI void APIENTRY glTexCoordPointervINTEL (GLint size, GLenum type, const void **pointer) {
  _glr->glExt.glTexCoordPointervINTEL (size, type, pointer);}
// #443 GL_INTEL_performance_query
inline GLAPI void APIENTRY glBeginPerfQueryINTEL (GLuint queryHandle) {
  _glr->glExt.glBeginPerfQueryINTEL (queryHandle);}
inline GLAPI void APIENTRY glCreatePerfQueryINTEL (GLuint queryId, GLuint *queryHandle) {
  _glr->glExt.glCreatePerfQueryINTEL (queryId, queryHandle);}
inline GLAPI void APIENTRY glDeletePerfQueryINTEL (GLuint queryHandle) {
  _glr->glExt.glDeletePerfQueryINTEL (queryHandle);}
inline GLAPI void APIENTRY glEndPerfQueryINTEL (GLuint queryHandle) {
  _glr->glExt.glEndPerfQueryINTEL (queryHandle);}
inline GLAPI void APIENTRY glGetFirstPerfQueryIdINTEL (GLuint *queryId) {
  _glr->glExt.glGetFirstPerfQueryIdINTEL (queryId);}
inline GLAPI void APIENTRY glGetNextPerfQueryIdINTEL (GLuint queryId, GLuint *nextQueryId) {
  _glr->glExt.glGetNextPerfQueryIdINTEL (queryId, nextQueryId);}
inline GLAPI void APIENTRY glGetPerfCounterInfoINTEL (GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar *counterName, GLuint counterDescLength, GLchar *counterDesc, GLuint *counterOffset, GLuint *counterDataSize, GLuint *counterTypeEnum, GLuint *counterDataTypeEnum, GLuint64 *rawCounterMaxValue) {
  _glr->glExt.glGetPerfCounterInfoINTEL (queryId, counterId, counterNameLength, counterName, counterDescLength, counterDesc, counterOffset, counterDataSize, counterTypeEnum, counterDataTypeEnum, rawCounterMaxValue);}
inline GLAPI void APIENTRY glGetPerfQueryDataINTEL (GLuint queryHandle, GLuint flags, GLsizei dataSize, GLvoid *data, GLuint *bytesWritten) {
  _glr->glExt.glGetPerfQueryDataINTEL (queryHandle, flags, dataSize, data, bytesWritten);}
inline GLAPI void APIENTRY glGetPerfQueryIdByNameINTEL (GLchar *queryName, GLuint *queryId) {
  _glr->glExt.glGetPerfQueryIdByNameINTEL (queryName, queryId);}
inline GLAPI void APIENTRY glGetPerfQueryInfoINTEL (GLuint queryId, GLuint queryNameLength, GLchar *queryName, GLuint *dataSize, GLuint *noCounters, GLuint *noInstances, GLuint *capsMask) {
  _glr->glExt.glGetPerfQueryInfoINTEL (queryId, queryNameLength, queryName, dataSize, noCounters, noInstances, capsMask);}
// #196 GL_MESA_resize_buffers
inline GLAPI void APIENTRY glResizeBuffersMESA (void) {
  _glr->glExt.glResizeBuffersMESA ();}
// #197 GL_MESA_window_pos
inline GLAPI void APIENTRY glWindowPos2dMESA (GLdouble x, GLdouble y) {
  _glr->glExt.glWindowPos2dMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2dvMESA (const GLdouble *v) {
  _glr->glExt.glWindowPos2dvMESA (v);}
inline GLAPI void APIENTRY glWindowPos2fMESA (GLfloat x, GLfloat y) {
  _glr->glExt.glWindowPos2fMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2fvMESA (const GLfloat *v) {
  _glr->glExt.glWindowPos2fvMESA (v);}
inline GLAPI void APIENTRY glWindowPos2iMESA (GLint x, GLint y) {
  _glr->glExt.glWindowPos2iMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2ivMESA (const GLint *v) {
  _glr->glExt.glWindowPos2ivMESA (v);}
inline GLAPI void APIENTRY glWindowPos2sMESA (GLshort x, GLshort y) {
  _glr->glExt.glWindowPos2sMESA (x, y);}
inline GLAPI void APIENTRY glWindowPos2svMESA (const GLshort *v) {
  _glr->glExt.glWindowPos2svMESA (v);}
inline GLAPI void APIENTRY glWindowPos3dMESA (GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glWindowPos3dMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3dvMESA (const GLdouble *v) {
  _glr->glExt.glWindowPos3dvMESA (v);}
inline GLAPI void APIENTRY glWindowPos3fMESA (GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glWindowPos3fMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3fvMESA (const GLfloat *v) {
  _glr->glExt.glWindowPos3fvMESA (v);}
inline GLAPI void APIENTRY glWindowPos3iMESA (GLint x, GLint y, GLint z) {
  _glr->glExt.glWindowPos3iMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3ivMESA (const GLint *v) {
  _glr->glExt.glWindowPos3ivMESA (v);}
inline GLAPI void APIENTRY glWindowPos3sMESA (GLshort x, GLshort y, GLshort z) {
  _glr->glExt.glWindowPos3sMESA (x, y, z);}
inline GLAPI void APIENTRY glWindowPos3svMESA (const GLshort *v) {
  _glr->glExt.glWindowPos3svMESA (v);}
inline GLAPI void APIENTRY glWindowPos4dMESA (GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glWindowPos4dMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4dvMESA (const GLdouble *v) {
  _glr->glExt.glWindowPos4dvMESA (v);}
inline GLAPI void APIENTRY glWindowPos4fMESA (GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glWindowPos4fMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4fvMESA (const GLfloat *v) {
  _glr->glExt.glWindowPos4fvMESA (v);}
inline GLAPI void APIENTRY glWindowPos4iMESA (GLint x, GLint y, GLint z, GLint w) {
  _glr->glExt.glWindowPos4iMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4ivMESA (const GLint *v) {
  _glr->glExt.glWindowPos4ivMESA (v);}
inline GLAPI void APIENTRY glWindowPos4sMESA (GLshort x, GLshort y, GLshort z, GLshort w) {
  _glr->glExt.glWindowPos4sMESA (x, y, z, w);}
inline GLAPI void APIENTRY glWindowPos4svMESA (const GLshort *v) {
  _glr->glExt.glWindowPos4svMESA (v);}
// #425 GL_NVX_conditional_render
inline GLAPI void APIENTRY glBeginConditionalRenderNVX (GLuint id) {
  _glr->glExt.glBeginConditionalRenderNVX (id);}
inline GLAPI void APIENTRY glEndConditionalRenderNVX (void) {
  _glr->glExt.glEndConditionalRenderNVX ();}
// #432 GL_NV_bindless_multi_draw_indirect
inline GLAPI void APIENTRY glMultiDrawArraysIndirectBindlessNV (GLenum mode, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount) {
  _glr->glExt.glMultiDrawArraysIndirectBindlessNV (mode, indirect, drawCount, stride, vertexBufferCount);}
inline GLAPI void APIENTRY glMultiDrawElementsIndirectBindlessNV (GLenum mode, GLenum type, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount) {
  _glr->glExt.glMultiDrawElementsIndirectBindlessNV (mode, type, indirect, drawCount, stride, vertexBufferCount);}
// #418 GL_NV_bindless_texture
inline GLAPI GLuint64 APIENTRY glGetTextureHandleNV (GLuint texture) {
  return _glr->glExt.glGetTextureHandleNV (texture);}
inline GLAPI GLuint64 APIENTRY glGetTextureSamplerHandleNV (GLuint texture, GLuint sampler) {
  return _glr->glExt.glGetTextureSamplerHandleNV (texture, sampler);}
inline GLAPI void APIENTRY glMakeTextureHandleResidentNV (GLuint64 handle) {
  _glr->glExt.glMakeTextureHandleResidentNV (handle);}
inline GLAPI void APIENTRY glMakeTextureHandleNonResidentNV (GLuint64 handle) {
  _glr->glExt.glMakeTextureHandleNonResidentNV (handle);}
inline GLAPI GLuint64 APIENTRY glGetImageHandleNV (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format) {
  return _glr->glExt.glGetImageHandleNV (texture, level, layered, layer, format);}
inline GLAPI void APIENTRY glMakeImageHandleResidentNV (GLuint64 handle, GLenum access) {
  _glr->glExt.glMakeImageHandleResidentNV (handle, access);}
inline GLAPI void APIENTRY glMakeImageHandleNonResidentNV (GLuint64 handle) {
  _glr->glExt.glMakeImageHandleNonResidentNV (handle);}
inline GLAPI void APIENTRY glUniformHandleui64NV (GLint location, GLuint64 value) {
  _glr->glExt.glUniformHandleui64NV (location, value);}
inline GLAPI void APIENTRY glUniformHandleui64vNV (GLint location, GLsizei count, const GLuint64 *value) {
  _glr->glExt.glUniformHandleui64vNV (location, count, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64NV (GLuint program, GLint location, GLuint64 value) {
  _glr->glExt.glProgramUniformHandleui64NV (program, location, value);}
inline GLAPI void APIENTRY glProgramUniformHandleui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64 *values) {
  _glr->glExt.glProgramUniformHandleui64vNV (program, location, count, values);}
inline GLAPI GLboolean APIENTRY glIsTextureHandleResidentNV (GLuint64 handle) {
  return _glr->glExt.glIsTextureHandleResidentNV (handle);}
inline GLAPI GLboolean APIENTRY glIsImageHandleResidentNV (GLuint64 handle) {
  return _glr->glExt.glIsImageHandleResidentNV (handle);}
// #433 GL_NV_blend_equation_advanced
inline GLAPI void APIENTRY glBlendParameteriNV (GLenum pname, GLint value) {
  _glr->glExt.glBlendParameteriNV (pname, value);}
inline GLAPI void APIENTRY glBlendBarrierNV (void) {
  _glr->glExt.glBlendBarrierNV ();}
// #346 GL_NV_conditional_render
inline GLAPI void APIENTRY glBeginConditionalRenderNV (GLuint id, GLenum mode) {
  _glr->glExt.glBeginConditionalRenderNV (id, mode);}
inline GLAPI void APIENTRY glEndConditionalRenderNV (void) {
  _glr->glExt.glEndConditionalRenderNV ();}
// #376 GL_NV_copy_image WGL_NV_copy_image GLX_NV_copy_image
inline GLAPI void APIENTRY glCopyImageSubDataNV (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
  _glr->glExt.glCopyImageSubDataNV (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);}
#ifdef OS_WIN
inline BOOL WINAPI wglCopyImageSubDataNV (HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
  return _glr->glExt.wglCopyImageSubDataNV (hSrcRC, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, hDstRC, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);}
#endif /// OS_WIN
// #334 GL_NV_depth_buffer_float
inline GLAPI void APIENTRY glDepthRangedNV (GLdouble zNear, GLdouble zFar) {
  _glr->glExt.glDepthRangedNV (zNear, zFar);}
inline GLAPI void APIENTRY glClearDepthdNV (GLdouble depth) {
  _glr->glExt.glClearDepthdNV (depth);}
inline GLAPI void APIENTRY glDepthBoundsdNV (GLdouble zmin, GLdouble zmax) {
  _glr->glExt.glDepthBoundsdNV (zmin, zmax);}
// #430 GL_NV_draw_texture
inline GLAPI void APIENTRY glDrawTextureNV (GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1) {
  _glr->glExt.glDrawTextureNV (texture, sampler, x0, y0, x1, y1, z, s0, t0, s1, t1);}
// #225 GL_NV_evaluators
inline GLAPI void APIENTRY glMapControlPointsNV (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void *points) {
  _glr->glExt.glMapControlPointsNV (target, index, type, ustride, vstride, uorder, vorder, packed, points);}
inline GLAPI void APIENTRY glMapParameterivNV (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glMapParameterivNV (target, pname, params);}
inline GLAPI void APIENTRY glMapParameterfvNV (GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glMapParameterfvNV (target, pname, params);}
inline GLAPI void APIENTRY glGetMapControlPointsNV (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void *points) {
  _glr->glExt.glGetMapControlPointsNV (target, index, type, ustride, vstride, packed, points);}
inline GLAPI void APIENTRY glGetMapParameterivNV (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetMapParameterivNV (target, pname, params);}
inline GLAPI void APIENTRY glGetMapParameterfvNV (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMapParameterfvNV (target, pname, params);}
inline GLAPI void APIENTRY glGetMapAttribParameterivNV (GLenum target, GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetMapAttribParameterivNV (target, index, pname, params);}
inline GLAPI void APIENTRY glGetMapAttribParameterfvNV (GLenum target, GLuint index, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetMapAttribParameterfvNV (target, index, pname, params);}
inline GLAPI void APIENTRY glEvalMapsNV (GLenum target, GLenum mode) {
  _glr->glExt.glEvalMapsNV (target, mode);}
// #357 GL_NV_explicit_multisample
inline GLAPI void APIENTRY glGetMultisamplefvNV (GLenum pname, GLuint index, GLfloat *val) {
  _glr->glExt.glGetMultisamplefvNV (pname, index, val);}
inline GLAPI void APIENTRY glSampleMaskIndexedNV (GLuint index, GLbitfield mask) {
  _glr->glExt.glSampleMaskIndexedNV (index, mask);}
inline GLAPI void APIENTRY glTexRenderbufferNV (GLenum target, GLuint renderbuffer) {
  _glr->glExt.glTexRenderbufferNV (target, renderbuffer);}
// #222 GL_NV_fence
inline GLAPI void APIENTRY glDeleteFencesNV (GLsizei n, const GLuint *fences) {
  _glr->glExt.glDeleteFencesNV (n, fences);}
inline GLAPI void APIENTRY glGenFencesNV (GLsizei n, GLuint *fences) {
  _glr->glExt.glGenFencesNV (n, fences);}
inline GLAPI GLboolean APIENTRY glIsFenceNV (GLuint fence) {
  return _glr->glExt.glIsFenceNV (fence);}
inline GLAPI GLboolean APIENTRY glTestFenceNV (GLuint fence) {
  return _glr->glExt.glTestFenceNV (fence);}
inline GLAPI void APIENTRY glGetFenceivNV (GLuint fence, GLenum pname, GLint *params) {
  _glr->glExt.glGetFenceivNV (fence, pname, params);}
inline GLAPI void APIENTRY glFinishFenceNV (GLuint fence) {
  _glr->glExt.glFinishFenceNV (fence);}
inline GLAPI void APIENTRY glSetFenceNV (GLuint fence, GLenum condition) {
  _glr->glExt.glSetFenceNV (fence, condition);}
// #282 GL_NV_fragment_program
inline GLAPI void APIENTRY glProgramNamedParameter4fNV (GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glProgramNamedParameter4fNV (id, len, name, x, y, z, w);}
inline GLAPI void APIENTRY glProgramNamedParameter4fvNV (GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v) {
  _glr->glExt.glProgramNamedParameter4fvNV (id, len, name, v);}
inline GLAPI void APIENTRY glProgramNamedParameter4dNV (GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glProgramNamedParameter4dNV (id, len, name, x, y, z, w);}
inline GLAPI void APIENTRY glProgramNamedParameter4dvNV (GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v) {
  _glr->glExt.glProgramNamedParameter4dvNV (id, len, name, v);}
inline GLAPI void APIENTRY glGetProgramNamedParameterfvNV (GLuint id, GLsizei len, const GLubyte *name, GLfloat *params) {
  _glr->glExt.glGetProgramNamedParameterfvNV (id, len, name, params);}
inline GLAPI void APIENTRY glGetProgramNamedParameterdvNV (GLuint id, GLsizei len, const GLubyte *name, GLdouble *params) {
  _glr->glExt.glGetProgramNamedParameterdvNV (id, len, name, params);}
// #336 GL_NV_framebuffer_multisample_coverage
inline GLAPI void APIENTRY glRenderbufferStorageMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height) {
  _glr->glExt.glRenderbufferStorageMultisampleCoverageNV (target, coverageSamples, colorSamples, internalformat, width, height);}
// #323 GL_NV_geometry_program4
inline GLAPI void APIENTRY glProgramVertexLimitNV (GLenum target, GLint limit) {
  _glr->glExt.glProgramVertexLimitNV (target, limit);}
inline GLAPI void APIENTRY glFramebufferTextureEXT (GLenum target, GLenum attachment, GLuint texture, GLint level) {
  _glr->glExt.glFramebufferTextureEXT (target, attachment, texture, level);}
inline GLAPI void APIENTRY glFramebufferTextureLayerEXT (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
  _glr->glExt.glFramebufferTextureLayerEXT (target, attachment, texture, level, layer);}
inline GLAPI void APIENTRY glFramebufferTextureFaceEXT (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face) {
  _glr->glExt.glFramebufferTextureFaceEXT (target, attachment, texture, level, face);}
// #322 GL_NV_gpu_program4
inline GLAPI void APIENTRY glProgramLocalParameterI4iNV (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w) {
  _glr->glExt.glProgramLocalParameterI4iNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameterI4ivNV (GLenum target, GLuint index, const GLint *params) {
  _glr->glExt.glProgramLocalParameterI4ivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParametersI4ivNV (GLenum target, GLuint index, GLsizei count, const GLint *params) {
  _glr->glExt.glProgramLocalParametersI4ivNV (target, index, count, params);}
inline GLAPI void APIENTRY glProgramLocalParameterI4uiNV (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  _glr->glExt.glProgramLocalParameterI4uiNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramLocalParameterI4uivNV (GLenum target, GLuint index, const GLuint *params) {
  _glr->glExt.glProgramLocalParameterI4uivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramLocalParametersI4uivNV (GLenum target, GLuint index, GLsizei count, const GLuint *params) {
  _glr->glExt.glProgramLocalParametersI4uivNV (target, index, count, params);}
inline GLAPI void APIENTRY glProgramEnvParameterI4iNV (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w) {
  _glr->glExt.glProgramEnvParameterI4iNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameterI4ivNV (GLenum target, GLuint index, const GLint *params) {
  _glr->glExt.glProgramEnvParameterI4ivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramEnvParametersI4ivNV (GLenum target, GLuint index, GLsizei count, const GLint *params) {
  _glr->glExt.glProgramEnvParametersI4ivNV (target, index, count, params);}
inline GLAPI void APIENTRY glProgramEnvParameterI4uiNV (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  _glr->glExt.glProgramEnvParameterI4uiNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramEnvParameterI4uivNV (GLenum target, GLuint index, const GLuint *params) {
  _glr->glExt.glProgramEnvParameterI4uivNV (target, index, params);}
inline GLAPI void APIENTRY glProgramEnvParametersI4uivNV (GLenum target, GLuint index, GLsizei count, const GLuint *params) {
  _glr->glExt.glProgramEnvParametersI4uivNV (target, index, count, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterIivNV (GLenum target, GLuint index, GLint *params) {
  _glr->glExt.glGetProgramLocalParameterIivNV (target, index, params);}
inline GLAPI void APIENTRY glGetProgramLocalParameterIuivNV (GLenum target, GLuint index, GLuint *params) {
  _glr->glExt.glGetProgramLocalParameterIuivNV (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterIivNV (GLenum target, GLuint index, GLint *params) {
  _glr->glExt.glGetProgramEnvParameterIivNV (target, index, params);}
inline GLAPI void APIENTRY glGetProgramEnvParameterIuivNV (GLenum target, GLuint index, GLuint *params) {
  _glr->glExt.glGetProgramEnvParameterIuivNV (target, index, params);}
// #388 GL_NV_gpu_program5
inline GLAPI void APIENTRY glProgramSubroutineParametersuivNV (GLenum target, GLsizei count, const GLuint *params) {
  _glr->glExt.glProgramSubroutineParametersuivNV (target, count, params);}
inline GLAPI void APIENTRY glGetProgramSubroutineParameteruivNV (GLenum target, GLuint index, GLuint *param) {
  _glr->glExt.glGetProgramSubroutineParameteruivNV (target, index, param);}
// #283 GL_NV_half_float
inline GLAPI void APIENTRY glVertex2hNV (GLhalfNV x, GLhalfNV y) {
  _glr->glExt.glVertex2hNV (x, y);}
inline GLAPI void APIENTRY glVertex2hvNV (const GLhalfNV *v) {
  _glr->glExt.glVertex2hvNV (v);}
inline GLAPI void APIENTRY glVertex3hNV (GLhalfNV x, GLhalfNV y, GLhalfNV z) {
  _glr->glExt.glVertex3hNV (x, y, z);}
inline GLAPI void APIENTRY glVertex3hvNV (const GLhalfNV *v) {
  _glr->glExt.glVertex3hvNV (v);}
inline GLAPI void APIENTRY glVertex4hNV (GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w) {
  _glr->glExt.glVertex4hNV (x, y, z, w);}
inline GLAPI void APIENTRY glVertex4hvNV (const GLhalfNV *v) {
  _glr->glExt.glVertex4hvNV (v);}
inline GLAPI void APIENTRY glNormal3hNV (GLhalfNV nx, GLhalfNV ny, GLhalfNV nz) {
  _glr->glExt.glNormal3hNV (nx, ny, nz);}
inline GLAPI void APIENTRY glNormal3hvNV (const GLhalfNV *v) {
  _glr->glExt.glNormal3hvNV (v);}
inline GLAPI void APIENTRY glColor3hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue) {
  _glr->glExt.glColor3hNV (red, green, blue);}
inline GLAPI void APIENTRY glColor3hvNV (const GLhalfNV *v) {
  _glr->glExt.glColor3hvNV (v);}
inline GLAPI void APIENTRY glColor4hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha) {
  _glr->glExt.glColor4hNV (red, green, blue, alpha);}
inline GLAPI void APIENTRY glColor4hvNV (const GLhalfNV *v) {
  _glr->glExt.glColor4hvNV (v);}
inline GLAPI void APIENTRY glTexCoord1hNV (GLhalfNV s) {
  _glr->glExt.glTexCoord1hNV (s);}
inline GLAPI void APIENTRY glTexCoord1hvNV (const GLhalfNV *v) {
  _glr->glExt.glTexCoord1hvNV (v);}
inline GLAPI void APIENTRY glTexCoord2hNV (GLhalfNV s, GLhalfNV t) {
  _glr->glExt.glTexCoord2hNV (s, t);}
inline GLAPI void APIENTRY glTexCoord2hvNV (const GLhalfNV *v) {
  _glr->glExt.glTexCoord2hvNV (v);}
inline GLAPI void APIENTRY glTexCoord3hNV (GLhalfNV s, GLhalfNV t, GLhalfNV r) {
  _glr->glExt.glTexCoord3hNV (s, t, r);}
inline GLAPI void APIENTRY glTexCoord3hvNV (const GLhalfNV *v) {
  _glr->glExt.glTexCoord3hvNV (v);}
inline GLAPI void APIENTRY glTexCoord4hNV (GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q) {
  _glr->glExt.glTexCoord4hNV (s, t, r, q);}
inline GLAPI void APIENTRY glTexCoord4hvNV (const GLhalfNV *v) {
  _glr->glExt.glTexCoord4hvNV (v);}
inline GLAPI void APIENTRY glMultiTexCoord1hNV (GLenum target, GLhalfNV s) {
  _glr->glExt.glMultiTexCoord1hNV (target, s);}
inline GLAPI void APIENTRY glMultiTexCoord1hvNV (GLenum target, const GLhalfNV *v) {
  _glr->glExt.glMultiTexCoord1hvNV (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord2hNV (GLenum target, GLhalfNV s, GLhalfNV t) {
  _glr->glExt.glMultiTexCoord2hNV (target, s, t);}
inline GLAPI void APIENTRY glMultiTexCoord2hvNV (GLenum target, const GLhalfNV *v) {
  _glr->glExt.glMultiTexCoord2hvNV (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord3hNV (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r) {
  _glr->glExt.glMultiTexCoord3hNV (target, s, t, r);}
inline GLAPI void APIENTRY glMultiTexCoord3hvNV (GLenum target, const GLhalfNV *v) {
  _glr->glExt.glMultiTexCoord3hvNV (target, v);}
inline GLAPI void APIENTRY glMultiTexCoord4hNV (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q) {
  _glr->glExt.glMultiTexCoord4hNV (target, s, t, r, q);}
inline GLAPI void APIENTRY glMultiTexCoord4hvNV (GLenum target, const GLhalfNV *v) {
  _glr->glExt.glMultiTexCoord4hvNV (target, v);}
inline GLAPI void APIENTRY glFogCoordhNV (GLhalfNV fog) {
  _glr->glExt.glFogCoordhNV (fog);}
inline GLAPI void APIENTRY glFogCoordhvNV (const GLhalfNV *fog) {
  _glr->glExt.glFogCoordhvNV (fog);}
inline GLAPI void APIENTRY glSecondaryColor3hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue) {
  _glr->glExt.glSecondaryColor3hNV (red, green, blue);}
inline GLAPI void APIENTRY glSecondaryColor3hvNV (const GLhalfNV *v) {
  _glr->glExt.glSecondaryColor3hvNV (v);}
inline GLAPI void APIENTRY glVertexWeighthNV (GLhalfNV weight) {
  _glr->glExt.glVertexWeighthNV (weight);}
inline GLAPI void APIENTRY glVertexWeighthvNV (const GLhalfNV *weight) {
  _glr->glExt.glVertexWeighthvNV (weight);}
inline GLAPI void APIENTRY glVertexAttrib1hNV (GLuint index, GLhalfNV x) {
  _glr->glExt.glVertexAttrib1hNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1hvNV (GLuint index, const GLhalfNV *v) {
  _glr->glExt.glVertexAttrib1hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2hNV (GLuint index, GLhalfNV x, GLhalfNV y) {
  _glr->glExt.glVertexAttrib2hNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2hvNV (GLuint index, const GLhalfNV *v) {
  _glr->glExt.glVertexAttrib2hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3hNV (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z) {
  _glr->glExt.glVertexAttrib3hNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3hvNV (GLuint index, const GLhalfNV *v) {
  _glr->glExt.glVertexAttrib3hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4hNV (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w) {
  _glr->glExt.glVertexAttrib4hNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4hvNV (GLuint index, const GLhalfNV *v) {
  _glr->glExt.glVertexAttrib4hvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribs1hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  _glr->glExt.glVertexAttribs1hvNV (index, n, v);}
inline GLAPI void APIENTRY glVertexAttribs2hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  _glr->glExt.glVertexAttribs2hvNV (index, n, v);}
inline GLAPI void APIENTRY glVertexAttribs3hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  _glr->glExt.glVertexAttribs3hvNV (index, n, v);}
inline GLAPI void APIENTRY glVertexAttribs4hvNV (GLuint index, GLsizei n, const GLhalfNV *v) {
  _glr->glExt.glVertexAttribs4hvNV (index, n, v);}
// #261 GL_NV_occlusion_query
inline GLAPI void APIENTRY glGenOcclusionQueriesNV (GLsizei n, GLuint *ids) {
  _glr->glExt.glGenOcclusionQueriesNV (n, ids);}
inline GLAPI void APIENTRY glDeleteOcclusionQueriesNV (GLsizei n, const GLuint *ids) {
  _glr->glExt.glDeleteOcclusionQueriesNV (n, ids);}
inline GLAPI GLboolean APIENTRY glIsOcclusionQueryNV (GLuint id) {
  return _glr->glExt.glIsOcclusionQueryNV (id);}
inline GLAPI void APIENTRY glBeginOcclusionQueryNV (GLuint id) {
  _glr->glExt.glBeginOcclusionQueryNV (id);}
inline GLAPI void APIENTRY glEndOcclusionQueryNV (void) {
  _glr->glExt.glEndOcclusionQueryNV ();}
inline GLAPI void APIENTRY glGetOcclusionQueryivNV (GLuint id, GLenum pname, GLint *params) {
  _glr->glExt.glGetOcclusionQueryivNV (id, pname, params);}
inline GLAPI void APIENTRY glGetOcclusionQueryuivNV (GLuint id, GLenum pname, GLuint *params) {
  _glr->glExt.glGetOcclusionQueryuivNV (id, pname, params);}
// #339 GL_NV_parameter_buffer_object
inline GLAPI void APIENTRY glProgramBufferParametersfvNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLfloat *params) {
  _glr->glExt.glProgramBufferParametersfvNV (target, bindingIndex, wordIndex, count, params);}
inline GLAPI void APIENTRY glProgramBufferParametersIivNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLint *params) {
  _glr->glExt.glProgramBufferParametersIivNV (target, bindingIndex, wordIndex, count, params);}
inline GLAPI void APIENTRY glProgramBufferParametersIuivNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLuint *params) {
  _glr->glExt.glProgramBufferParametersIuivNV (target, bindingIndex, wordIndex, count, params);}
// #410 GL_NV_path_rendering
inline GLAPI GLuint APIENTRY glGenPathsNV (GLsizei range) {
  return _glr->glExt.glGenPathsNV (range);}
inline GLAPI void APIENTRY glDeletePathsNV (GLuint path, GLsizei range) {
  _glr->glExt.glDeletePathsNV (path, range);}
inline GLAPI GLboolean APIENTRY glIsPathNV (GLuint path) {
  return _glr->glExt.glIsPathNV (path);}
inline GLAPI void APIENTRY glPathCommandsNV (GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords) {
  _glr->glExt.glPathCommandsNV (path, numCommands, commands, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathCoordsNV (GLuint path, GLsizei numCoords, GLenum coordType, const void *coords) {
  _glr->glExt.glPathCoordsNV (path, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathSubCommandsNV (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords) {
  _glr->glExt.glPathSubCommandsNV (path, commandStart, commandsToDelete, numCommands, commands, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathSubCoordsNV (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void *coords) {
  _glr->glExt.glPathSubCoordsNV (path, coordStart, numCoords, coordType, coords);}
inline GLAPI void APIENTRY glPathStringNV (GLuint path, GLenum format, GLsizei length, const void *pathString) {
  _glr->glExt.glPathStringNV (path, format, length, pathString);}
inline GLAPI void APIENTRY glPathGlyphsNV (GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const void *charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale) {
  _glr->glExt.glPathGlyphsNV (firstPathName, fontTarget, fontName, fontStyle, numGlyphs, type, charcodes, handleMissingGlyphs, pathParameterTemplate, emScale);}
inline GLAPI void APIENTRY glPathGlyphRangeNV (GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale) {
  _glr->glExt.glPathGlyphRangeNV (firstPathName, fontTarget, fontName, fontStyle, firstGlyph, numGlyphs, handleMissingGlyphs, pathParameterTemplate, emScale);}
inline GLAPI void APIENTRY glWeightPathsNV (GLuint resultPath, GLsizei numPaths, const GLuint *paths, const GLfloat *weights) {
  _glr->glExt.glWeightPathsNV (resultPath, numPaths, paths, weights);}
inline GLAPI void APIENTRY glCopyPathNV (GLuint resultPath, GLuint srcPath) {
  _glr->glExt.glCopyPathNV (resultPath, srcPath);}
inline GLAPI void APIENTRY glInterpolatePathsNV (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight) {
  _glr->glExt.glInterpolatePathsNV (resultPath, pathA, pathB, weight);}
inline GLAPI void APIENTRY glTransformPathNV (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat *transformValues) {
  _glr->glExt.glTransformPathNV (resultPath, srcPath, transformType, transformValues);}
inline GLAPI void APIENTRY glPathParameterivNV (GLuint path, GLenum pname, const GLint *value) {
  _glr->glExt.glPathParameterivNV (path, pname, value);}
inline GLAPI void APIENTRY glPathParameteriNV (GLuint path, GLenum pname, GLint value) {
  _glr->glExt.glPathParameteriNV (path, pname, value);}
inline GLAPI void APIENTRY glPathParameterfvNV (GLuint path, GLenum pname, const GLfloat *value) {
  _glr->glExt.glPathParameterfvNV (path, pname, value);}
inline GLAPI void APIENTRY glPathParameterfNV (GLuint path, GLenum pname, GLfloat value) {
  _glr->glExt.glPathParameterfNV (path, pname, value);}
inline GLAPI void APIENTRY glPathDashArrayNV (GLuint path, GLsizei dashCount, const GLfloat *dashArray) {
  _glr->glExt.glPathDashArrayNV (path, dashCount, dashArray);}
inline GLAPI void APIENTRY glPathStencilFuncNV (GLenum func, GLint ref, GLuint mask) {
  _glr->glExt.glPathStencilFuncNV (func, ref, mask);}
inline GLAPI void APIENTRY glPathStencilDepthOffsetNV (GLfloat factor, GLfloat units) {
  _glr->glExt.glPathStencilDepthOffsetNV (factor, units);}
inline GLAPI void APIENTRY glStencilFillPathNV (GLuint path, GLenum fillMode, GLuint mask) {
  _glr->glExt.glStencilFillPathNV (path, fillMode, mask);}
inline GLAPI void APIENTRY glStencilStrokePathNV (GLuint path, GLint reference, GLuint mask) {
  _glr->glExt.glStencilStrokePathNV (path, reference, mask);}
inline GLAPI void APIENTRY glStencilFillPathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues) {
  _glr->glExt.glStencilFillPathInstancedNV (numPaths, pathNameType, paths, pathBase, fillMode, mask, transformType, transformValues);}
inline GLAPI void APIENTRY glStencilStrokePathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues) {
  _glr->glExt.glStencilStrokePathInstancedNV (numPaths, pathNameType, paths, pathBase, reference, mask, transformType, transformValues);}
inline GLAPI void APIENTRY glPathCoverDepthFuncNV (GLenum func) {
  _glr->glExt.glPathCoverDepthFuncNV (func);}
inline GLAPI void APIENTRY glPathColorGenNV (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs) {
  _glr->glExt.glPathColorGenNV (color, genMode, colorFormat, coeffs);}
inline GLAPI void APIENTRY glPathTexGenNV (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat *coeffs) {
  _glr->glExt.glPathTexGenNV (texCoordSet, genMode, components, coeffs);}
inline GLAPI void APIENTRY glPathFogGenNV (GLenum genMode) {
  _glr->glExt.glPathFogGenNV (genMode);}
inline GLAPI void APIENTRY glCoverFillPathNV (GLuint path, GLenum coverMode) {
  _glr->glExt.glCoverFillPathNV (path, coverMode);}
inline GLAPI void APIENTRY glCoverStrokePathNV (GLuint path, GLenum coverMode) {
  _glr->glExt.glCoverStrokePathNV (path, coverMode);}
inline GLAPI void APIENTRY glCoverFillPathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues) {
  _glr->glExt.glCoverFillPathInstancedNV (numPaths, pathNameType, paths, pathBase, coverMode, transformType, transformValues);}
inline GLAPI void APIENTRY glCoverStrokePathInstancedNV (GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues) {
  _glr->glExt.glCoverStrokePathInstancedNV (numPaths, pathNameType, paths, pathBase, coverMode, transformType, transformValues);}
inline GLAPI void APIENTRY glGetPathParameterivNV (GLuint path, GLenum pname, GLint *value) {
  _glr->glExt.glGetPathParameterivNV (path, pname, value);}
inline GLAPI void APIENTRY glGetPathParameterfvNV (GLuint path, GLenum pname, GLfloat *value) {
  _glr->glExt.glGetPathParameterfvNV (path, pname, value);}
inline GLAPI void APIENTRY glGetPathCommandsNV (GLuint path, GLubyte *commands) {
  _glr->glExt.glGetPathCommandsNV (path, commands);}
inline GLAPI void APIENTRY glGetPathCoordsNV (GLuint path, GLfloat *coords) {
  _glr->glExt.glGetPathCoordsNV (path, coords);}
inline GLAPI void APIENTRY glGetPathDashArrayNV (GLuint path, GLfloat *dashArray) {
  _glr->glExt.glGetPathDashArrayNV (path, dashArray);}
inline GLAPI void APIENTRY glGetPathMetricsNV (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics) {
  _glr->glExt.glGetPathMetricsNV (metricQueryMask, numPaths, pathNameType, paths, pathBase, stride, metrics);}
inline GLAPI void APIENTRY glGetPathMetricRangeNV (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics) {
  _glr->glExt.glGetPathMetricRangeNV (metricQueryMask, firstPathName, numPaths, stride, metrics);}
inline GLAPI void APIENTRY glGetPathSpacingNV (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing) {
  _glr->glExt.glGetPathSpacingNV (pathListMode, numPaths, pathNameType, paths, pathBase, advanceScale, kerningScale, transformType, returnedSpacing);}
inline GLAPI void APIENTRY glGetPathColorGenivNV (GLenum color, GLenum pname, GLint *value) {
  _glr->glExt.glGetPathColorGenivNV (color, pname, value);}
inline GLAPI void APIENTRY glGetPathColorGenfvNV (GLenum color, GLenum pname, GLfloat *value) {
  _glr->glExt.glGetPathColorGenfvNV (color, pname, value);}
inline GLAPI void APIENTRY glGetPathTexGenivNV (GLenum texCoordSet, GLenum pname, GLint *value) {
  _glr->glExt.glGetPathTexGenivNV (texCoordSet, pname, value);}
inline GLAPI void APIENTRY glGetPathTexGenfvNV (GLenum texCoordSet, GLenum pname, GLfloat *value) {
  _glr->glExt.glGetPathTexGenfvNV (texCoordSet, pname, value);}
inline GLAPI GLboolean APIENTRY glIsPointInFillPathNV (GLuint path, GLuint mask, GLfloat x, GLfloat y) {
  return _glr->glExt.glIsPointInFillPathNV (path, mask, x, y);}
inline GLAPI GLboolean APIENTRY glIsPointInStrokePathNV (GLuint path, GLfloat x, GLfloat y) {
  return _glr->glExt.glIsPointInStrokePathNV (path, x, y);}
inline GLAPI GLfloat APIENTRY glGetPathLengthNV (GLuint path, GLsizei startSegment, GLsizei numSegments) {
  return _glr->glExt.glGetPathLengthNV (path, startSegment, numSegments);}
inline GLAPI GLboolean APIENTRY glPointAlongPathNV (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat *x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY) {
  return _glr->glExt.glPointAlongPathNV (path, startSegment, numSegments, distance, x, y, tangentX, tangentY);}
// #284 GL_NV_pixel_data_range
inline GLAPI void APIENTRY glPixelDataRangeNV (GLenum target, GLsizei length, const void *pointer) {
  _glr->glExt.glPixelDataRangeNV (target, length, pointer);}
inline GLAPI void APIENTRY glFlushPixelDataRangeNV (GLenum target) {
  _glr->glExt.glFlushPixelDataRangeNV (target);}
// #262 GL_NV_point_sprite
inline GLAPI void APIENTRY glPointParameteriNV (GLenum pname, GLint param) {
  _glr->glExt.glPointParameteriNV (pname, param);}
inline GLAPI void APIENTRY glPointParameterivNV (GLenum pname, const GLint *params) {
  _glr->glExt.glPointParameterivNV (pname, params);}
// #347 GL_NV_present_video GLX_NV_present_video WGL_NV_present_video
inline GLAPI void APIENTRY glPresentFrameKeyedNV (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1) {
  _glr->glExt.glPresentFrameKeyedNV (video_slot, minPresentTime, beginPresentTimeId, presentDurationId, type, target0, fill0, key0, target1, fill1, key1);}
inline GLAPI void APIENTRY glPresentFrameDualFillNV (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3) {
  _glr->glExt.glPresentFrameDualFillNV (video_slot, minPresentTime, beginPresentTimeId, presentDurationId, type, target0, fill0, target1, fill1, target2, fill2, target3, fill3);}
inline GLAPI void APIENTRY glGetVideoivNV (GLuint video_slot, GLenum pname, GLint *params) {
  _glr->glExt.glGetVideoivNV (video_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideouivNV (GLuint video_slot, GLenum pname, GLuint *params) {
  _glr->glExt.glGetVideouivNV (video_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideoi64vNV (GLuint video_slot, GLenum pname, GLint64EXT *params) {
  _glr->glExt.glGetVideoi64vNV (video_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideoui64vNV (GLuint video_slot, GLenum pname, GLuint64EXT *params) {
  _glr->glExt.glGetVideoui64vNV (video_slot, pname, params);}






// #285 GL_NV_primitive_restart
inline GLAPI void APIENTRY glPrimitiveRestartNV (void) {
  _glr->glExt.glPrimitiveRestartNV ();}
inline GLAPI void APIENTRY glPrimitiveRestartIndexNV (GLuint index) {
  _glr->glExt.glPrimitiveRestartIndexNV (index);}
// #191 GL_NV_register_combiners
inline GLAPI void APIENTRY glCombinerParameterfvNV (GLenum pname, const GLfloat *params) {
  _glr->glExt.glCombinerParameterfvNV (pname, params);}
inline GLAPI void APIENTRY glCombinerParameterfNV (GLenum pname, GLfloat param) {
  _glr->glExt.glCombinerParameterfNV (pname, param);}
inline GLAPI void APIENTRY glCombinerParameterivNV (GLenum pname, const GLint *params) {
  _glr->glExt.glCombinerParameterivNV (pname, params);}
inline GLAPI void APIENTRY glCombinerParameteriNV (GLenum pname, GLint param) {
  _glr->glExt.glCombinerParameteriNV (pname, param);}
inline GLAPI void APIENTRY glCombinerInputNV (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage) {
  _glr->glExt.glCombinerInputNV (stage, portion, variable, input, mapping, componentUsage);}
inline GLAPI void APIENTRY glCombinerOutputNV (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum) {
  _glr->glExt.glCombinerOutputNV (stage, portion, abOutput, cdOutput, sumOutput, scale, bias, abDotProduct, cdDotProduct, muxSum);}
inline GLAPI void APIENTRY glFinalCombinerInputNV (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage) {
  _glr->glExt.glFinalCombinerInputNV (variable, input, mapping, componentUsage);}
inline GLAPI void APIENTRY glGetCombinerInputParameterfvNV (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetCombinerInputParameterfvNV (stage, portion, variable, pname, params);}
inline GLAPI void APIENTRY glGetCombinerInputParameterivNV (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params) {
  _glr->glExt.glGetCombinerInputParameterivNV (stage, portion, variable, pname, params);}
inline GLAPI void APIENTRY glGetCombinerOutputParameterfvNV (GLenum stage, GLenum portion, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetCombinerOutputParameterfvNV (stage, portion, pname, params);}
inline GLAPI void APIENTRY glGetCombinerOutputParameterivNV (GLenum stage, GLenum portion, GLenum pname, GLint *params) {
  _glr->glExt.glGetCombinerOutputParameterivNV (stage, portion, pname, params);}
inline GLAPI void APIENTRY glGetFinalCombinerInputParameterfvNV (GLenum variable, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetFinalCombinerInputParameterfvNV (variable, pname, params);}
inline GLAPI void APIENTRY glGetFinalCombinerInputParameterivNV (GLenum variable, GLenum pname, GLint *params) {
  _glr->glExt.glGetFinalCombinerInputParameterivNV (variable, pname, params);}
// #227 GL_NV_register_combiners2
inline GLAPI void APIENTRY glCombinerStageParameterfvNV (GLenum stage, GLenum pname, const GLfloat *params) {
  _glr->glExt.glCombinerStageParameterfvNV (stage, pname, params);}
inline GLAPI void APIENTRY glGetCombinerStageParameterfvNV (GLenum stage, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetCombinerStageParameterfvNV (stage, pname, params);}
// #379 GL_NV_shader_buffer_load
inline GLAPI void APIENTRY glMakeBufferResidentNV (GLenum target, GLenum access) {
  _glr->glExt.glMakeBufferResidentNV (target, access);}
inline GLAPI void APIENTRY glMakeBufferNonResidentNV (GLenum target) {
  _glr->glExt.glMakeBufferNonResidentNV (target);}
inline GLAPI GLboolean APIENTRY glIsBufferResidentNV (GLenum target) {
  return _glr->glExt.glIsBufferResidentNV (target);}
inline GLAPI void APIENTRY glMakeNamedBufferResidentNV (GLuint buffer, GLenum access) {
  _glr->glExt.glMakeNamedBufferResidentNV (buffer, access);}
inline GLAPI void APIENTRY glMakeNamedBufferNonResidentNV (GLuint buffer) {
  _glr->glExt.glMakeNamedBufferNonResidentNV (buffer);}
inline GLAPI GLboolean APIENTRY glIsNamedBufferResidentNV (GLuint buffer) {
  return _glr->glExt.glIsNamedBufferResidentNV (buffer);}
inline GLAPI void APIENTRY glGetBufferParameterui64vNV (GLenum target, GLenum pname, GLuint64EXT *params) {
  _glr->glExt.glGetBufferParameterui64vNV (target, pname, params);}
inline GLAPI void APIENTRY glGetNamedBufferParameterui64vNV (GLuint buffer, GLenum pname, GLuint64EXT *params) {
  _glr->glExt.glGetNamedBufferParameterui64vNV (buffer, pname, params);}
inline GLAPI void APIENTRY glGetIntegerui64vNV (GLenum value, GLuint64EXT *result) {
  _glr->glExt.glGetIntegerui64vNV (value, result);}
inline GLAPI void APIENTRY glUniformui64NV (GLint location, GLuint64EXT value) {
  _glr->glExt.glUniformui64NV (location, value);}
inline GLAPI void APIENTRY glUniformui64vNV (GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glUniformui64vNV (location, count, value);}
inline GLAPI void APIENTRY glProgramUniformui64NV (GLuint program, GLint location, GLuint64EXT value) {
  _glr->glExt.glProgramUniformui64NV (program, location, value);}
inline GLAPI void APIENTRY glProgramUniformui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value) {
  _glr->glExt.glProgramUniformui64vNV (program, location, count, value);}
// #381 GL_NV_texture_barrier
inline GLAPI void APIENTRY glTextureBarrierNV (void) {
  _glr->glExt.glTextureBarrierNV ();}
// #403 GL_NV_texture_multisample
inline GLAPI void APIENTRY glTexImage2DMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations) {
  _glr->glExt.glTexImage2DMultisampleCoverageNV (target, coverageSamples, colorSamples, internalFormat, width, height, fixedSampleLocations);}
inline GLAPI void APIENTRY glTexImage3DMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations) {
  _glr->glExt.glTexImage3DMultisampleCoverageNV (target, coverageSamples, colorSamples, internalFormat, width, height, depth, fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage2DMultisampleNV (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations) {
  _glr->glExt.glTextureImage2DMultisampleNV (texture, target, samples, internalFormat, width, height,  fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage3DMultisampleNV (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations) {
  _glr->glExt.glTextureImage3DMultisampleNV (texture, target, samples, internalFormat, width, height, depth, fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage2DMultisampleCoverageNV (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations) {
  _glr->glExt.glTextureImage2DMultisampleCoverageNV (texture, target, coverageSamples, colorSamples, internalFormat, width, height, fixedSampleLocations);}
inline GLAPI void APIENTRY glTextureImage3DMultisampleCoverageNV (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations) {
  _glr->glExt.glTextureImage3DMultisampleCoverageNV (texture, target, coverageSamples, colorSamples, internalFormat, width, height, depth, fixedSampleLocations);}
// #341 GL_NV_transform_feedback
inline GLAPI void APIENTRY glBeginTransformFeedbackNV (GLenum primitiveMode) {
  _glr->glExt.glBeginTransformFeedbackNV (primitiveMode);}
inline GLAPI void APIENTRY glEndTransformFeedbackNV (void) {
  _glr->glExt.glEndTransformFeedbackNV ();}
inline GLAPI void APIENTRY glTransformFeedbackAttribsNV (GLuint count, const GLint *attribs, GLenum bufferMode) {
  _glr->glExt.glTransformFeedbackAttribsNV (count, attribs, bufferMode);}
inline GLAPI void APIENTRY glBindBufferRangeNV (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
  _glr->glExt.glBindBufferRangeNV (target, index, buffer, offset, size);}
inline GLAPI void APIENTRY glBindBufferOffsetNV (GLenum target, GLuint index, GLuint buffer, GLintptr offset) {
  _glr->glExt.glBindBufferOffsetNV (target, index, buffer, offset);}
inline GLAPI void APIENTRY glBindBufferBaseNV (GLenum target, GLuint index, GLuint buffer) {
  _glr->glExt.glBindBufferBaseNV (target, index, buffer);}
inline GLAPI void APIENTRY glTransformFeedbackVaryingsNV (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode) {
  _glr->glExt.glTransformFeedbackVaryingsNV (program, count, locations, bufferMode);}
inline GLAPI void APIENTRY glActiveVaryingNV (GLuint program, const GLchar *name) {
  _glr->glExt.glActiveVaryingNV (program, name);}
inline GLAPI GLint APIENTRY glGetVaryingLocationNV (GLuint program, const GLchar *name) {
  return _glr->glExt.glGetVaryingLocationNV (program, name);}
inline GLAPI void APIENTRY glGetActiveVaryingNV (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
  _glr->glExt.glGetActiveVaryingNV (program, index, bufSize, length, size, type, name);}
inline GLAPI void APIENTRY glGetTransformFeedbackVaryingNV (GLuint program, GLuint index, GLint *location) {
  _glr->glExt.glGetTransformFeedbackVaryingNV (program, index, location);}
inline GLAPI void APIENTRY glTransformFeedbackStreamAttribsNV (GLsizei count, const GLint *attribs, GLsizei nbuffers, const GLint *bufstreams, GLenum bufferMode) {
  _glr->glExt.glTransformFeedbackStreamAttribsNV (count, attribs, nbuffers, bufstreams, bufferMode);}
// #358 GL_NV_transform_feedback2
inline GLAPI void APIENTRY glBindTransformFeedbackNV (GLenum target, GLuint id) {
  _glr->glExt.glBindTransformFeedbackNV (target, id);}
inline GLAPI void APIENTRY glDeleteTransformFeedbacksNV (GLsizei n, const GLuint *ids) {
  _glr->glExt.glDeleteTransformFeedbacksNV (n, ids);}
inline GLAPI void APIENTRY glGenTransformFeedbacksNV (GLsizei n, GLuint *ids) {
  _glr->glExt.glGenTransformFeedbacksNV (n, ids);}
inline GLAPI GLboolean APIENTRY glIsTransformFeedbackNV (GLuint id) {
  return _glr->glExt.glIsTransformFeedbackNV (id);}
inline GLAPI void APIENTRY glPauseTransformFeedbackNV (void) {
  _glr->glExt.glPauseTransformFeedbackNV ();}
inline GLAPI void APIENTRY glResumeTransformFeedbackNV (void) {
  _glr->glExt.glResumeTransformFeedbackNV ();}
inline GLAPI void APIENTRY glDrawTransformFeedbackNV (GLenum mode, GLuint id) {
  _glr->glExt.glDrawTransformFeedbackNV (mode, id);}
// #396 GL_NV_vdpau_interop
inline GLAPI void APIENTRY glVDPAUInitNV (const void *vdpDevice, const void *getProcAddress) {
  _glr->glExt.glVDPAUInitNV (vdpDevice, getProcAddress);}
inline GLAPI void APIENTRY glVDPAUFiniNV (void) {
  _glr->glExt.glVDPAUFiniNV ();}
inline GLAPI GLvdpauSurfaceNV APIENTRY glVDPAURegisterVideoSurfaceNV (const void *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames) {
  return _glr->glExt.glVDPAURegisterVideoSurfaceNV (vdpSurface, target, numTextureNames, textureNames);}
inline GLAPI GLvdpauSurfaceNV APIENTRY glVDPAURegisterOutputSurfaceNV (const void *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames) {
  return _glr->glExt.glVDPAURegisterOutputSurfaceNV (vdpSurface, target, numTextureNames, textureNames);}
inline GLAPI GLboolean APIENTRY glVDPAUIsSurfaceNV (GLvdpauSurfaceNV surface) {
  return _glr->glExt.glVDPAUIsSurfaceNV (surface);}
inline GLAPI void APIENTRY glVDPAUUnregisterSurfaceNV (GLvdpauSurfaceNV surface) {
  _glr->glExt.glVDPAUUnregisterSurfaceNV (surface);}
inline GLAPI void APIENTRY glVDPAUGetSurfaceivNV (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) {
  _glr->glExt.glVDPAUGetSurfaceivNV (surface, pname, bufSize, length, values);}
inline GLAPI void APIENTRY glVDPAUSurfaceAccessNV (GLvdpauSurfaceNV surface, GLenum access) {
  _glr->glExt.glVDPAUSurfaceAccessNV (surface, access);}
inline GLAPI void APIENTRY glVDPAUMapSurfacesNV (GLsizei numSurfaces, const GLvdpauSurfaceNV *surfaces) {
  _glr->glExt.glVDPAUMapSurfacesNV (numSurfaces, surfaces);}
inline GLAPI void APIENTRY glVDPAUUnmapSurfacesNV (GLsizei numSurface, const GLvdpauSurfaceNV *surfaces) {
  _glr->glExt.glVDPAUUnmapSurfacesNV (numSurface, surfaces);}
// #190 GL_NV_vertex_array_range
inline GLAPI void APIENTRY glFlushVertexArrayRangeNV (void) {
  _glr->glExt.glFlushVertexArrayRangeNV ();}
inline GLAPI void APIENTRY glVertexArrayRangeNV (GLsizei length, const void *pointer) {
  _glr->glExt.glVertexArrayRangeNV (length, pointer);}
// #392 GL_NV_vertex_attrib_integer_64bit
inline GLAPI void APIENTRY glVertexAttribL1i64NV (GLuint index, GLint64EXT x) {
  _glr->glExt.glVertexAttribL1i64NV (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2i64NV (GLuint index, GLint64EXT x, GLint64EXT y) {
  _glr->glExt.glVertexAttribL2i64NV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3i64NV (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z) {
  _glr->glExt.glVertexAttribL3i64NV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4i64NV (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w) {
  _glr->glExt.glVertexAttribL4i64NV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1i64vNV (GLuint index, const GLint64EXT *v) {
  _glr->glExt.glVertexAttribL1i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2i64vNV (GLuint index, const GLint64EXT *v) {
  _glr->glExt.glVertexAttribL2i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3i64vNV (GLuint index, const GLint64EXT *v) {
  _glr->glExt.glVertexAttribL3i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4i64vNV (GLuint index, const GLint64EXT *v) {
  _glr->glExt.glVertexAttribL4i64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL1ui64NV (GLuint index, GLuint64EXT x) {
  _glr->glExt.glVertexAttribL1ui64NV (index, x);}
inline GLAPI void APIENTRY glVertexAttribL2ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y) {
  _glr->glExt.glVertexAttribL2ui64NV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribL3ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z) {
  _glr->glExt.glVertexAttribL3ui64NV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribL4ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w) {
  _glr->glExt.glVertexAttribL4ui64NV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribL1ui64vNV (GLuint index, const GLuint64EXT *v) {
  _glr->glExt.glVertexAttribL1ui64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL2ui64vNV (GLuint index, const GLuint64EXT *v) {
  _glr->glExt.glVertexAttribL2ui64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL3ui64vNV (GLuint index, const GLuint64EXT *v) {
  _glr->glExt.glVertexAttribL3ui64vNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribL4ui64vNV (GLuint index, const GLuint64EXT *v) {
  _glr->glExt.glVertexAttribL4ui64vNV (index, v);}
inline GLAPI void APIENTRY glGetVertexAttribLi64vNV (GLuint index, GLenum pname, GLint64EXT *params) {
  _glr->glExt.glGetVertexAttribLi64vNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribLui64vNV (GLuint index, GLenum pname, GLuint64EXT *params) {
  _glr->glExt.glGetVertexAttribLui64vNV (index, pname, params);}
inline GLAPI void APIENTRY glVertexAttribLFormatNV (GLuint index, GLint size, GLenum type, GLsizei stride) {
  _glr->glExt.glVertexAttribLFormatNV (index, size, type, stride);}
// #380 GL_NV_vertex_buffer_unified_memory
inline GLAPI void APIENTRY glBufferAddressRangeNV (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length) {
  _glr->glExt.glBufferAddressRangeNV (pname, index, address, length);}
inline GLAPI void APIENTRY glVertexFormatNV (GLint size, GLenum type, GLsizei stride) {
  _glr->glExt.glVertexFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glNormalFormatNV (GLenum type, GLsizei stride) {
  _glr->glExt.glNormalFormatNV (type, stride);}
inline GLAPI void APIENTRY glColorFormatNV (GLint size, GLenum type, GLsizei stride) {
  _glr->glExt.glColorFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glIndexFormatNV (GLenum type, GLsizei stride) {
  _glr->glExt.glIndexFormatNV (type, stride);}
inline GLAPI void APIENTRY glTexCoordFormatNV (GLint size, GLenum type, GLsizei stride) {
  _glr->glExt.glTexCoordFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glEdgeFlagFormatNV (GLsizei stride) {
  _glr->glExt.glEdgeFlagFormatNV (stride);}
inline GLAPI void APIENTRY glSecondaryColorFormatNV (GLint size, GLenum type, GLsizei stride) {
  _glr->glExt.glSecondaryColorFormatNV (size, type, stride);}
inline GLAPI void APIENTRY glFogCoordFormatNV (GLenum type, GLsizei stride) {
  _glr->glExt.glFogCoordFormatNV (type, stride);}
inline GLAPI void APIENTRY glVertexAttribFormatNV (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride) {
  _glr->glExt.glVertexAttribFormatNV (index, size, type, normalized, stride);}
inline GLAPI void APIENTRY glVertexAttribIFormatNV (GLuint index, GLint size, GLenum type, GLsizei stride) {
  _glr->glExt.glVertexAttribIFormatNV (index, size, type, stride);}
inline GLAPI void APIENTRY glGetIntegerui64i_vNV (GLenum value, GLuint index, GLuint64EXT *result) {
  _glr->glExt.glGetIntegerui64i_vNV (value, index, result);}
// #233 GL_NV_vertex_program
inline GLAPI GLboolean APIENTRY glAreProgramsResidentNV (GLsizei n, const GLuint *programs, GLboolean *residences) {
  return _glr->glExt.glAreProgramsResidentNV (n, programs, residences);}
inline GLAPI void APIENTRY glBindProgramNV (GLenum target, GLuint id) {
  _glr->glExt.glBindProgramNV (target, id);}
inline GLAPI void APIENTRY glDeleteProgramsNV (GLsizei n, const GLuint *programs) {
  _glr->glExt.glDeleteProgramsNV (n, programs);}
inline GLAPI void APIENTRY glExecuteProgramNV (GLenum target, GLuint id, const GLfloat *params) {
  _glr->glExt.glExecuteProgramNV (target, id, params);}
inline GLAPI void APIENTRY glGenProgramsNV (GLsizei n, GLuint *programs) {
  _glr->glExt.glGenProgramsNV (n, programs);}
inline GLAPI void APIENTRY glGetProgramParameterdvNV (GLenum target, GLuint index, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetProgramParameterdvNV (target, index, pname, params);}
inline GLAPI void APIENTRY glGetProgramParameterfvNV (GLenum target, GLuint index, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetProgramParameterfvNV (target, index, pname, params);}
inline GLAPI void APIENTRY glGetProgramivNV (GLuint id, GLenum pname, GLint *params) {
  _glr->glExt.glGetProgramivNV (id, pname, params);}
inline GLAPI void APIENTRY glGetProgramStringNV (GLuint id, GLenum pname, GLubyte *program) {
  _glr->glExt.glGetProgramStringNV (id, pname, program);}
inline GLAPI void APIENTRY glGetTrackMatrixivNV (GLenum target, GLuint address, GLenum pname, GLint *params) {
  _glr->glExt.glGetTrackMatrixivNV (target, address, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribdvNV (GLuint index, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetVertexAttribdvNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribfvNV (GLuint index, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetVertexAttribfvNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribivNV (GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetVertexAttribivNV (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribPointervNV (GLuint index, GLenum pname, void **pointer) {
  _glr->glExt.glGetVertexAttribPointervNV (index, pname, pointer);}
inline GLAPI GLboolean APIENTRY glIsProgramNV (GLuint id) {
  return _glr->glExt.glIsProgramNV (id);}
inline GLAPI void APIENTRY glLoadProgramNV (GLenum target, GLuint id, GLsizei len, const GLubyte *program) {
  _glr->glExt.glLoadProgramNV (target, id, len, program);}
inline GLAPI void APIENTRY glProgramParameter4dNV (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glProgramParameter4dNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramParameter4dvNV (GLenum target, GLuint index, const GLdouble *v) {
  _glr->glExt.glProgramParameter4dvNV (target, index, v);}
inline GLAPI void APIENTRY glProgramParameter4fNV (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glProgramParameter4fNV (target, index, x, y, z, w);}
inline GLAPI void APIENTRY glProgramParameter4fvNV (GLenum target, GLuint index, const GLfloat *v) {
  _glr->glExt.glProgramParameter4fvNV (target, index, v);}
inline GLAPI void APIENTRY glProgramParameters4dvNV (GLenum target, GLuint index, GLsizei count, const GLdouble *v) {
  _glr->glExt.glProgramParameters4dvNV (target, index, count, v);}
inline GLAPI void APIENTRY glProgramParameters4fvNV (GLenum target, GLuint index, GLsizei count, const GLfloat *v) {
  _glr->glExt.glProgramParameters4fvNV (target, index, count, v);}
inline GLAPI void APIENTRY glRequestResidentProgramsNV (GLsizei n, const GLuint *programs) {
  _glr->glExt.glRequestResidentProgramsNV (n, programs);}
inline GLAPI void APIENTRY glTrackMatrixNV (GLenum target, GLuint address, GLenum matrix, GLenum transform) {
  _glr->glExt.glTrackMatrixNV (target, address, matrix, transform);}
inline GLAPI void APIENTRY glVertexAttribPointerNV (GLuint index, GLint fsize, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexAttribPointerNV (index, fsize, type, stride, pointer);}
inline GLAPI void APIENTRY glVertexAttrib1dNV (GLuint index, GLdouble x) {
  _glr->glExt.glVertexAttrib1dNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1dvNV (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib1dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1fNV (GLuint index, GLfloat x) {
  _glr->glExt.glVertexAttrib1fNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1fvNV (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib1fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib1sNV (GLuint index, GLshort x) {
  _glr->glExt.glVertexAttrib1sNV (index, x);}
inline GLAPI void APIENTRY glVertexAttrib1svNV (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib1svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2dNV (GLuint index, GLdouble x, GLdouble y) {
  _glr->glExt.glVertexAttrib2dNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2dvNV (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib2dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2fNV (GLuint index, GLfloat x, GLfloat y) {
  _glr->glExt.glVertexAttrib2fNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2fvNV (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib2fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib2sNV (GLuint index, GLshort x, GLshort y) {
  _glr->glExt.glVertexAttrib2sNV (index, x, y);}
inline GLAPI void APIENTRY glVertexAttrib2svNV (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib2svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3dNV (GLuint index, GLdouble x, GLdouble y, GLdouble z) {
  _glr->glExt.glVertexAttrib3dNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3dvNV (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib3dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3fNV (GLuint index, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glVertexAttrib3fNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3fvNV (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib3fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib3sNV (GLuint index, GLshort x, GLshort y, GLshort z) {
  _glr->glExt.glVertexAttrib3sNV (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttrib3svNV (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib3svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4dNV (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
  _glr->glExt.glVertexAttrib4dNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4dvNV (GLuint index, const GLdouble *v) {
  _glr->glExt.glVertexAttrib4dvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4fNV (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glVertexAttrib4fNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4fvNV (GLuint index, const GLfloat *v) {
  _glr->glExt.glVertexAttrib4fvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4sNV (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
  _glr->glExt.glVertexAttrib4sNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4svNV (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttrib4svNV (index, v);}
inline GLAPI void APIENTRY glVertexAttrib4ubNV (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
  _glr->glExt.glVertexAttrib4ubNV (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttrib4ubvNV (GLuint index, const GLubyte *v) {
  _glr->glExt.glVertexAttrib4ubvNV (index, v);}
inline GLAPI void APIENTRY glVertexAttribs1dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  _glr->glExt.glVertexAttribs1dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs1fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  _glr->glExt.glVertexAttribs1fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs1svNV (GLuint index, GLsizei count, const GLshort *v) {
  _glr->glExt.glVertexAttribs1svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs2dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  _glr->glExt.glVertexAttribs2dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs2fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  _glr->glExt.glVertexAttribs2fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs2svNV (GLuint index, GLsizei count, const GLshort *v) {
  _glr->glExt.glVertexAttribs2svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs3dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  _glr->glExt.glVertexAttribs3dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs3fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  _glr->glExt.glVertexAttribs3fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs3svNV (GLuint index, GLsizei count, const GLshort *v) {
  _glr->glExt.glVertexAttribs3svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4dvNV (GLuint index, GLsizei count, const GLdouble *v) {
  _glr->glExt.glVertexAttribs4dvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4fvNV (GLuint index, GLsizei count, const GLfloat *v) {
  _glr->glExt.glVertexAttribs4fvNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4svNV (GLuint index, GLsizei count, const GLshort *v) {
  _glr->glExt.glVertexAttribs4svNV (index, count, v);}
inline GLAPI void APIENTRY glVertexAttribs4ubvNV (GLuint index, GLsizei count, const GLubyte *v) {
  _glr->glExt.glVertexAttribs4ubvNV (index, count, v);}
// #325 GL_NV_vertex_program4
inline GLAPI void APIENTRY glVertexAttribI1iEXT (GLuint index, GLint x) {
  _glr->glExt.glVertexAttribI1iEXT (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2iEXT (GLuint index, GLint x, GLint y) {
  _glr->glExt.glVertexAttribI2iEXT (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3iEXT (GLuint index, GLint x, GLint y, GLint z) {
  _glr->glExt.glVertexAttribI3iEXT (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4iEXT (GLuint index, GLint x, GLint y, GLint z, GLint w) {
  _glr->glExt.glVertexAttribI4iEXT (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1uiEXT (GLuint index, GLuint x) {
  _glr->glExt.glVertexAttribI1uiEXT (index, x);}
inline GLAPI void APIENTRY glVertexAttribI2uiEXT (GLuint index, GLuint x, GLuint y) {
  _glr->glExt.glVertexAttribI2uiEXT (index, x, y);}
inline GLAPI void APIENTRY glVertexAttribI3uiEXT (GLuint index, GLuint x, GLuint y, GLuint z) {
  _glr->glExt.glVertexAttribI3uiEXT (index, x, y, z);}
inline GLAPI void APIENTRY glVertexAttribI4uiEXT (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
  _glr->glExt.glVertexAttribI4uiEXT (index, x, y, z, w);}
inline GLAPI void APIENTRY glVertexAttribI1ivEXT (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI1ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2ivEXT (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI2ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3ivEXT (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI3ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4ivEXT (GLuint index, const GLint *v) {
  _glr->glExt.glVertexAttribI4ivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI1uivEXT (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI1uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI2uivEXT (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI2uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI3uivEXT (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI3uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4uivEXT (GLuint index, const GLuint *v) {
  _glr->glExt.glVertexAttribI4uivEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4bvEXT (GLuint index, const GLbyte *v) {
  _glr->glExt.glVertexAttribI4bvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4svEXT (GLuint index, const GLshort *v) {
  _glr->glExt.glVertexAttribI4svEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4ubvEXT (GLuint index, const GLubyte *v) {
  _glr->glExt.glVertexAttribI4ubvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribI4usvEXT (GLuint index, const GLushort *v) {
  _glr->glExt.glVertexAttribI4usvEXT (index, v);}
inline GLAPI void APIENTRY glVertexAttribIPointerEXT (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  _glr->glExt.glVertexAttribIPointerEXT (index, size, type, stride, pointer);}
inline GLAPI void APIENTRY glGetVertexAttribIivEXT (GLuint index, GLenum pname, GLint *params) {
  _glr->glExt.glGetVertexAttribIivEXT (index, pname, params);}
inline GLAPI void APIENTRY glGetVertexAttribIuivEXT (GLuint index, GLenum pname, GLuint *params) {
  _glr->glExt.glGetVertexAttribIuivEXT (index, pname, params);}
// #374 GL_NV_video_capture
inline GLAPI void APIENTRY glBeginVideoCaptureNV (GLuint video_capture_slot) {
  _glr->glExt.glBeginVideoCaptureNV (video_capture_slot);}
inline GLAPI void APIENTRY glBindVideoCaptureStreamBufferNV (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset) {
  _glr->glExt.glBindVideoCaptureStreamBufferNV (video_capture_slot, stream, frame_region, offset);}
inline GLAPI void APIENTRY glBindVideoCaptureStreamTextureNV (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture) {
  _glr->glExt.glBindVideoCaptureStreamTextureNV (video_capture_slot, stream, frame_region, target, texture);}
inline GLAPI void APIENTRY glEndVideoCaptureNV (GLuint video_capture_slot) {
  _glr->glExt.glEndVideoCaptureNV (video_capture_slot);}
inline GLAPI void APIENTRY glGetVideoCaptureivNV (GLuint video_capture_slot, GLenum pname, GLint *params) {
  _glr->glExt.glGetVideoCaptureivNV (video_capture_slot, pname, params);}
inline GLAPI void APIENTRY glGetVideoCaptureStreamivNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint *params) {
  _glr->glExt.glGetVideoCaptureStreamivNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glGetVideoCaptureStreamfvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetVideoCaptureStreamfvNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glGetVideoCaptureStreamdvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble *params) {
  _glr->glExt.glGetVideoCaptureStreamdvNV (video_capture_slot, stream, pname, params);}
inline GLAPI GLenum APIENTRY glVideoCaptureNV (GLuint video_capture_slot, GLuint *sequence_num, GLuint64EXT *capture_time) {
  return _glr->glExt.glVideoCaptureNV (video_capture_slot, sequence_num, capture_time);}
inline GLAPI void APIENTRY glVideoCaptureStreamParameterivNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint *params) {
  _glr->glExt.glVideoCaptureStreamParameterivNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glVideoCaptureStreamParameterfvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat *params) {
  _glr->glExt.glVideoCaptureStreamParameterfvNV (video_capture_slot, stream, pname, params);}
inline GLAPI void APIENTRY glVideoCaptureStreamParameterdvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble *params) {
  _glr->glExt.glVideoCaptureStreamParameterdvNV (video_capture_slot, stream, pname, params);}
/*
#ifdef OS_WIN
/// WGL_NV_video_capture
inline BOOL WINAPI wglBindVideoCaptureDeviceNV (UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice) {
  return _glr->glExt.wglBindVideoCaptureDeviceNV (uVideoSlot, hDevice);}
inline UINT WINAPI wglEnumerateVideoCaptureDevicesNV (HDC hDc, HVIDEOINPUTDEVICENV *phDeviceList) {
  return _glr->glExt.wglEnumerateVideoCaptureDevicesNV (hDc, phDeviceList);}
inline BOOL WINAPI wglLockVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
  return _glr->glExt.wglLockVideoCaptureDeviceNV (hDc, hDevice);}
inline BOOL WINAPI wglQueryVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int *piValue) {
  return _glr->glExt.wglQueryVideoCaptureDeviceNV (hDc, hDevice, iAttribute, piValue);}
inline BOOL WINAPI wglReleaseVideoCaptureDeviceNV (HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
  return _glr->glExt.wglReleaseVideoCaptureDeviceNV (hDc, hDevice);}
#endif
*/
// #077 GL_PGI_misc_hints
inline GLAPI void APIENTRY glHintPGI (GLenum target, GLint mode) {
  _glr->glExt.glHintPGI (target, mode);}
// #021 GL_SGIS_detail_texture
inline GLAPI void APIENTRY glDetailTexFuncSGIS (GLenum target, GLsizei n, const GLfloat *points) {
  _glr->glExt.glDetailTexFuncSGIS (target, n, points);}
inline GLAPI void APIENTRY glGetDetailTexFuncSGIS (GLenum target, GLfloat *points) {
  _glr->glExt.glGetDetailTexFuncSGIS (target, points);}
// #064 GL_SGIS_fog_function
inline GLAPI void APIENTRY glFogFuncSGIS (GLsizei n, const GLfloat *points) {
  _glr->glExt.glFogFuncSGIS (n, points);}
inline GLAPI void APIENTRY glGetFogFuncSGIS (GLfloat *points) {
  _glr->glExt.glGetFogFuncSGIS (points);}
// #025 GL_SGIS_multisample
inline GLAPI void APIENTRY glSampleMaskSGIS (GLclampf value, GLboolean invert) {
  _glr->glExt.glSampleMaskSGIS (value, invert);}
inline GLAPI void APIENTRY glSamplePatternSGIS (GLenum pattern) {
  _glr->glExt.glSamplePatternSGIS (pattern);}
// #015 GL_SGIS_pixel_texture
inline GLAPI void APIENTRY glPixelTexGenParameteriSGIS (GLenum pname, GLint param) {
  _glr->glExt.glPixelTexGenParameteriSGIS (pname, param);}
inline GLAPI void APIENTRY glPixelTexGenParameterivSGIS (GLenum pname, const GLint *params) {
  _glr->glExt.glPixelTexGenParameterivSGIS (pname, params);}
inline GLAPI void APIENTRY glPixelTexGenParameterfSGIS (GLenum pname, GLfloat param) {
  _glr->glExt.glPixelTexGenParameterfSGIS (pname, param);}
inline GLAPI void APIENTRY glPixelTexGenParameterfvSGIS (GLenum pname, const GLfloat *params) {
  _glr->glExt.glPixelTexGenParameterfvSGIS (pname, params);}
inline GLAPI void APIENTRY glGetPixelTexGenParameterivSGIS (GLenum pname, GLint *params) {
  _glr->glExt.glGetPixelTexGenParameterivSGIS (pname, params);}
inline GLAPI void APIENTRY glGetPixelTexGenParameterfvSGIS (GLenum pname, GLfloat *params) {
  _glr->glExt.glGetPixelTexGenParameterfvSGIS (pname, params);}
// GL_SGIS_point_parameters -not in any list
inline GLAPI void APIENTRY glPointParameterfSGIS (GLenum pname, GLfloat param) {
  _glr->glExt.glPointParameterfSGIS (pname, param);}
inline GLAPI void APIENTRY glPointParameterfvSGIS (GLenum pname, const GLfloat *params) {
  _glr->glExt.glPointParameterfvSGIS (pname, params);}
// #022 GL_SGIS_sharpen_texture
inline GLAPI void APIENTRY glSharpenTexFuncSGIS (GLenum target, GLsizei n, const GLfloat *points) {
  _glr->glExt.glSharpenTexFuncSGIS (target, n, points);}
inline GLAPI void APIENTRY glGetSharpenTexFuncSGIS (GLenum target, GLfloat *points) {
  _glr->glExt.glGetSharpenTexFuncSGIS (target, points);}
// #016 GL_SGIS_texture4D
inline GLAPI void APIENTRY glTexImage4DSGIS (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexImage4DSGIS (target, level, internalformat, width, height, depth, size4d, border, format, type, pixels);}
inline GLAPI void APIENTRY glTexSubImage4DSGIS (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const void *pixels) {
  _glr->glExt.glTexSubImage4DSGIS (target, level, xoffset, yoffset, zoffset, woffset, width, height, depth, size4d, format, type, pixels);}
// #214 GL_SGIS_texture_color_mask
inline GLAPI void APIENTRY glTextureColorMaskSGIS (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
  _glr->glExt.glTextureColorMaskSGIS (red, green, blue, alpha);}
// #007 GL_SGIS_texture_filter4
inline GLAPI void APIENTRY glGetTexFilterFuncSGIS (GLenum target, GLenum filter, GLfloat *weights) {
  _glr->glExt.glGetTexFilterFuncSGIS (target, filter, weights);}
inline GLAPI void APIENTRY glTexFilterFuncSGIS (GLenum target, GLenum filter, GLsizei n, const GLfloat *weights) {
  _glr->glExt.glTexFilterFuncSGIS (target, filter, n, weights);}
// #132 GL_SGIX_async
inline GLAPI void APIENTRY glAsyncMarkerSGIX (GLuint marker) {
  _glr->glExt.glAsyncMarkerSGIX (marker);}
inline GLAPI GLint APIENTRY glFinishAsyncSGIX (GLuint *markerp) {
  return _glr->glExt.glFinishAsyncSGIX (markerp);}
inline GLAPI GLint APIENTRY glPollAsyncSGIX (GLuint *markerp) {
  return _glr->glExt.glPollAsyncSGIX (markerp);}
inline GLAPI GLuint APIENTRY glGenAsyncMarkersSGIX (GLsizei range) {
  return _glr->glExt.glGenAsyncMarkersSGIX (range);}
inline GLAPI void APIENTRY glDeleteAsyncMarkersSGIX (GLuint marker, GLsizei range) {
  _glr->glExt.glDeleteAsyncMarkersSGIX (marker, range);}
inline GLAPI GLboolean APIENTRY glIsAsyncMarkerSGIX (GLuint marker) {
  return _glr->glExt.glIsAsyncMarkerSGIX (marker);}
// #061 GL_SGIX_flush_raster
inline GLAPI void APIENTRY glFlushRasterSGIX (void) {
  _glr->glExt.glFlushRasterSGIX ();}
// GL_SGIX_fragment_lighting - not in any list
inline GLAPI void APIENTRY glFragmentLightfSGIX (GLenum light, GLenum pname, GLfloat param) {
  _glr->glExt.glFragmentLightfSGIX (light, pname, param);}
inline GLAPI void APIENTRY glFragmentLightfvSGIX (GLenum light, GLenum pname, const GLfloat *params) {
  _glr->glExt.glFragmentLightfvSGIX (light, pname, params);}
inline GLAPI void APIENTRY glFragmentLightiSGIX (GLenum light, GLenum pname, GLint param) {
  _glr->glExt.glFragmentLightiSGIX (light, pname, param);}
inline GLAPI void APIENTRY glFragmentLightivSGIX (GLenum light, GLenum pname, const GLint *params) {
  _glr->glExt.glFragmentLightivSGIX (light, pname, params);}
inline GLAPI void APIENTRY glFragmentLightModelfSGIX (GLenum pname, GLfloat param) {
  _glr->glExt.glFragmentLightModelfSGIX (pname, param);}
inline GLAPI void APIENTRY glFragmentLightModelfvSGIX (GLenum pname, const GLfloat *params) {
  _glr->glExt.glFragmentLightModelfvSGIX (pname, params);}
inline GLAPI void APIENTRY glFragmentLightModeliSGIX (GLenum pname, GLint param) {
  _glr->glExt.glFragmentLightModeliSGIX (pname, param);}
inline GLAPI void APIENTRY glFragmentLightModelivSGIX (GLenum pname, const GLint *params) {
  _glr->glExt.glFragmentLightModelivSGIX (pname, params);}
inline GLAPI void APIENTRY glFragmentMaterialfSGIX (GLenum face, GLenum pname, GLfloat param) {
  _glr->glExt.glFragmentMaterialfSGIX (face, pname, param);}
inline GLAPI void APIENTRY glFragmentMaterialfvSGIX (GLenum face, GLenum pname, const GLfloat *params) {
  _glr->glExt.glFragmentMaterialfvSGIX (face, pname, params);}
inline GLAPI void APIENTRY glFragmentMaterialiSGIX (GLenum face, GLenum pname, GLint param) {
  _glr->glExt.glFragmentMaterialiSGIX (face, pname, param);}
inline GLAPI void APIENTRY glFragmentMaterialivSGIX (GLenum face, GLenum pname, const GLint *params) {
  _glr->glExt.glFragmentMaterialivSGIX (face, pname, params);}
inline GLAPI void APIENTRY glGetFragmentLightfvSGIX (GLenum light, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetFragmentLightfvSGIX (light, pname, params);}
inline GLAPI void APIENTRY glGetFragmentLightivSGIX (GLenum light, GLenum pname, GLint *params) {
  _glr->glExt.glGetFragmentLightivSGIX (light, pname, params);}
inline GLAPI void APIENTRY glGetFragmentMaterialfvSGIX (GLenum face, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetFragmentMaterialfvSGIX (face, pname, params);}
inline GLAPI void APIENTRY glGetFragmentMaterialivSGIX (GLenum face, GLenum pname, GLint *params) {
  _glr->glExt.glGetFragmentMaterialivSGIX (face, pname, params);}
inline GLAPI void APIENTRY glLightEnviSGIX (GLenum pname, GLint param) {
  _glr->glExt.glLightEnviSGIX (pname, param);}
// #057 GL_SGIX_framezoom
inline GLAPI void APIENTRY glFrameZoomSGIX (GLint factor) {
  _glr->glExt.glFrameZoomSGIX (factor);}
// GL_SGIX_igloo_interface - not in any list
inline GLAPI void APIENTRY glIglooInterfaceSGIX (GLenum pname, const void *params) {
  _glr->glExt.glIglooInterfaceSGIX (pname, params);}
// #055 GL_SGIX_instruments
inline GLAPI GLint APIENTRY glGetInstrumentsSGIX (void) {
  return _glr->glExt.glGetInstrumentsSGIX ();}
inline GLAPI void APIENTRY glInstrumentsBufferSGIX (GLsizei size, GLint *buffer) {
  _glr->glExt.glInstrumentsBufferSGIX (size, buffer);}
inline GLAPI GLint APIENTRY glPollInstrumentsSGIX (GLint *marker_p) {
  return _glr->glExt.glPollInstrumentsSGIX (marker_p);}
inline GLAPI void APIENTRY glReadInstrumentsSGIX (GLint marker) {
  _glr->glExt.glReadInstrumentsSGIX (marker);}
inline GLAPI void APIENTRY glStartInstrumentsSGIX (void) {
  _glr->glExt.glStartInstrumentsSGIX ();}
inline GLAPI void APIENTRY glStopInstrumentsSGIX (GLint marker) {
  _glr->glExt.glStopInstrumentsSGIX (marker);}
// #080 GL_SGIX_list_priority
inline GLAPI void APIENTRY glGetListParameterfvSGIX (GLuint list, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetListParameterfvSGIX (list, pname, params);}
inline GLAPI void APIENTRY glGetListParameterivSGIX (GLuint list, GLenum pname, GLint *params) {
  _glr->glExt.glGetListParameterivSGIX (list, pname, params);}
inline GLAPI void APIENTRY glListParameterfSGIX (GLuint list, GLenum pname, GLfloat param) {
  _glr->glExt.glListParameterfSGIX (list, pname, param);}
inline GLAPI void APIENTRY glListParameterfvSGIX (GLuint list, GLenum pname, const GLfloat *params) {
  _glr->glExt.glListParameterfvSGIX (list, pname, params);}
inline GLAPI void APIENTRY glListParameteriSGIX (GLuint list, GLenum pname, GLint param) {
  _glr->glExt.glListParameteriSGIX (list, pname, param);}
inline GLAPI void APIENTRY glListParameterivSGIX (GLuint list, GLenum pname, const GLint *params) {
  _glr->glExt.glListParameterivSGIX (list, pname, params);}
// #015a GL_SGIX_pixel_texture
inline GLAPI void APIENTRY glPixelTexGenSGIX (GLenum mode) {
  _glr->glExt.glPixelTexGenSGIX (mode);}
// GL_SGIX_polynomial_ffd - not in any list
inline GLAPI void APIENTRY glDeformationMap3dSGIX (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points) {
  _glr->glExt.glDeformationMap3dSGIX (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, w1, w2, wstride, worder, points);}
inline GLAPI void APIENTRY glDeformationMap3fSGIX (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points) {
  _glr->glExt.glDeformationMap3fSGIX (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, w1, w2, wstride, worder, points);}
inline GLAPI void APIENTRY glDeformSGIX (GLbitfield mask) {
  _glr->glExt.glDeformSGIX (mask);}
inline GLAPI void APIENTRY glLoadIdentityDeformationMapSGIX (GLbitfield mask) {
  _glr->glExt.glLoadIdentityDeformationMapSGIX (mask);}
// #060 GL_SGIX_reference_plane
inline GLAPI void APIENTRY glReferencePlaneSGIX (const GLdouble *equation) {
  _glr->glExt.glReferencePlaneSGIX (equation);}
// #052 GL_SGIX_sprite
inline GLAPI void APIENTRY glSpriteParameterfSGIX (GLenum pname, GLfloat param) {
  _glr->glExt.glSpriteParameterfSGIX (pname, param);}
inline GLAPI void APIENTRY glSpriteParameterfvSGIX (GLenum pname, const GLfloat *params) {
  _glr->glExt.glSpriteParameterfvSGIX (pname, params);}
inline GLAPI void APIENTRY glSpriteParameteriSGIX (GLenum pname, GLint param) {
  _glr->glExt.glSpriteParameteriSGIX (pname, param);}
inline GLAPI void APIENTRY glSpriteParameterivSGIX (GLenum pname, const GLint *params) {
  _glr->glExt.glSpriteParameterivSGIX (pname, params);}
// #058 GL_SGIX_tag_sample_buffer
inline GLAPI void APIENTRY glTagSampleBufferSGIX (void) {
  _glr->glExt.glTagSampleBufferSGIX ();}
// #014 GL_SGI_color_table
inline GLAPI void APIENTRY glColorTableSGI (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *table) {
  _glr->glExt.glColorTableSGI (target, internalformat, width, format, type, table);}
inline GLAPI void APIENTRY glColorTableParameterfvSGI (GLenum target, GLenum pname, const GLfloat *params) {
  _glr->glExt.glColorTableParameterfvSGI (target, pname, params);}
inline GLAPI void APIENTRY glColorTableParameterivSGI (GLenum target, GLenum pname, const GLint *params) {
  _glr->glExt.glColorTableParameterivSGI (target, pname, params);}
inline GLAPI void APIENTRY glCopyColorTableSGI (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width) {
  _glr->glExt.glCopyColorTableSGI (target, internalformat, x, y, width);}
inline GLAPI void APIENTRY glGetColorTableSGI (GLenum target, GLenum format, GLenum type, void *table) {
  _glr->glExt.glGetColorTableSGI (target, format, type, table);}
inline GLAPI void APIENTRY glGetColorTableParameterfvSGI (GLenum target, GLenum pname, GLfloat *params) {
  _glr->glExt.glGetColorTableParameterfvSGI (target, pname, params);}
inline GLAPI void APIENTRY glGetColorTableParameterivSGI (GLenum target, GLenum pname, GLint *params) {
  _glr->glExt.glGetColorTableParameterivSGI (target, pname, params);}
// #163 GL_SUNX_constant_data
inline GLAPI void APIENTRY glFinishTextureSUNX (void) {
  _glr->glExt.glFinishTextureSUNX ();}
// #164 GL_SUN_global_alpha
inline GLAPI void APIENTRY glGlobalAlphaFactorbSUN (GLbyte factor) {
  _glr->glExt.glGlobalAlphaFactorbSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorsSUN (GLshort factor) {
  _glr->glExt.glGlobalAlphaFactorsSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactoriSUN (GLint factor) {
  _glr->glExt.glGlobalAlphaFactoriSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorfSUN (GLfloat factor) {
  _glr->glExt.glGlobalAlphaFactorfSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactordSUN (GLdouble factor) {
  _glr->glExt.glGlobalAlphaFactordSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorubSUN (GLubyte factor) {
  _glr->glExt.glGlobalAlphaFactorubSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactorusSUN (GLushort factor) {
  _glr->glExt.glGlobalAlphaFactorusSUN (factor);}
inline GLAPI void APIENTRY glGlobalAlphaFactoruiSUN (GLuint factor) {
  _glr->glExt.glGlobalAlphaFactoruiSUN (factor);}
// #257 GL_SUN_mesh_array
inline GLAPI void APIENTRY glDrawMeshArraysSUN (GLenum mode, GLint first, GLsizei count, GLsizei width) {
  _glr->glExt.glDrawMeshArraysSUN (mode, first, count, width);}
// #165 GL_SUN_triangle_list
inline GLAPI void APIENTRY glReplacementCodeuiSUN (GLuint code) {
  _glr->glExt.glReplacementCodeuiSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeusSUN (GLushort code) {
  _glr->glExt.glReplacementCodeusSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeubSUN (GLubyte code) {
  _glr->glExt.glReplacementCodeubSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeuivSUN (const GLuint *code) {
  _glr->glExt.glReplacementCodeuivSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeusvSUN (const GLushort *code) {
  _glr->glExt.glReplacementCodeusvSUN (code);}
inline GLAPI void APIENTRY glReplacementCodeubvSUN (const GLubyte *code) {
  _glr->glExt.glReplacementCodeubvSUN (code);}
inline GLAPI void APIENTRY glReplacementCodePointerSUN (GLenum type, GLsizei stride, const void **pointer) {
  _glr->glExt.glReplacementCodePointerSUN (type, stride, pointer);}
// #166 GL_SUN_vertex
inline GLAPI void APIENTRY glColor4ubVertex2fSUN (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y) {
  _glr->glExt.glColor4ubVertex2fSUN (r, g, b, a, x, y);}
inline GLAPI void APIENTRY glColor4ubVertex2fvSUN (const GLubyte *c, const GLfloat *v) {
  _glr->glExt.glColor4ubVertex2fvSUN (c, v);}
inline GLAPI void APIENTRY glColor4ubVertex3fSUN (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glColor4ubVertex3fSUN (r, g, b, a, x, y, z);}
inline GLAPI void APIENTRY glColor4ubVertex3fvSUN (const GLubyte *c, const GLfloat *v) {
  _glr->glExt.glColor4ubVertex3fvSUN (c, v);}
inline GLAPI void APIENTRY glColor3fVertex3fSUN (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glColor3fVertex3fSUN (r, g, b, x, y, z);}
inline GLAPI void APIENTRY glColor3fVertex3fvSUN (const GLfloat *c, const GLfloat *v) {
  _glr->glExt.glColor3fVertex3fvSUN (c, v);}
inline GLAPI void APIENTRY glNormal3fVertex3fSUN (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glNormal3fVertex3fSUN (nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glNormal3fVertex3fvSUN (const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glNormal3fVertex3fvSUN (n, v);}
inline GLAPI void APIENTRY glColor4fNormal3fVertex3fSUN (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glColor4fNormal3fVertex3fSUN (r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glColor4fNormal3fVertex3fvSUN (const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glColor4fNormal3fVertex3fvSUN (c, n, v);}
inline GLAPI void APIENTRY glTexCoord2fVertex3fSUN (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glTexCoord2fVertex3fSUN (s, t, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fVertex3fvSUN (const GLfloat *tc, const GLfloat *v) {
  _glr->glExt.glTexCoord2fVertex3fvSUN (tc, v);}
inline GLAPI void APIENTRY glTexCoord4fVertex4fSUN (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glTexCoord4fVertex4fSUN (s, t, p, q, x, y, z, w);}
inline GLAPI void APIENTRY glTexCoord4fVertex4fvSUN (const GLfloat *tc, const GLfloat *v) {
  _glr->glExt.glTexCoord4fVertex4fvSUN (tc, v);}
inline GLAPI void APIENTRY glTexCoord2fColor4ubVertex3fSUN (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glTexCoord2fColor4ubVertex3fSUN (s, t, r, g, b, a, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fColor4ubVertex3fvSUN (const GLfloat *tc, const GLubyte *c, const GLfloat *v) {
  _glr->glExt.glTexCoord2fColor4ubVertex3fvSUN (tc, c, v);}
inline GLAPI void APIENTRY glTexCoord2fColor3fVertex3fSUN (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glTexCoord2fColor3fVertex3fSUN (s, t, r, g, b, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fColor3fVertex3fvSUN (const GLfloat *tc, const GLfloat *c, const GLfloat *v) {
  _glr->glExt.glTexCoord2fColor3fVertex3fvSUN (tc, c, v);}
inline GLAPI void APIENTRY glTexCoord2fNormal3fVertex3fSUN (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glTexCoord2fNormal3fVertex3fSUN (s, t, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fNormal3fVertex3fvSUN (const GLfloat *tc, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glTexCoord2fNormal3fVertex3fvSUN (tc, n, v);}
inline GLAPI void APIENTRY glTexCoord2fColor4fNormal3fVertex3fSUN (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glTexCoord2fColor4fNormal3fVertex3fSUN (s, t, r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glTexCoord2fColor4fNormal3fVertex3fvSUN (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glTexCoord2fColor4fNormal3fVertex3fvSUN (tc, c, n, v);}
inline GLAPI void APIENTRY glTexCoord4fColor4fNormal3fVertex4fSUN (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
  _glr->glExt.glTexCoord4fColor4fNormal3fVertex4fSUN (s, t, p, q, r, g, b, a, nx, ny, nz, x, y, z, w);}
inline GLAPI void APIENTRY glTexCoord4fColor4fNormal3fVertex4fvSUN (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glTexCoord4fColor4fNormal3fVertex4fvSUN (tc, c, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiVertex3fSUN (GLuint rc, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiVertex3fSUN (rc, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiVertex3fvSUN (const GLuint *rc, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiVertex3fvSUN (rc, v);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4ubVertex3fSUN (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiColor4ubVertex3fSUN (rc, r, g, b, a, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4ubVertex3fvSUN (const GLuint *rc, const GLubyte *c, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiColor4ubVertex3fvSUN (rc, c, v);}
inline GLAPI void APIENTRY glReplacementCodeuiColor3fVertex3fSUN (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiColor3fVertex3fSUN (rc, r, g, b, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiColor3fVertex3fvSUN (const GLuint *rc, const GLfloat *c, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiColor3fVertex3fvSUN (rc, c, v);}
inline GLAPI void APIENTRY glReplacementCodeuiNormal3fVertex3fSUN (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiNormal3fVertex3fSUN (rc, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiNormal3fVertex3fvSUN (rc, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4fNormal3fVertex3fSUN (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiColor4fNormal3fVertex3fSUN (rc, r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiColor4fNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiColor4fNormal3fVertex3fvSUN (rc, c, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fVertex3fSUN (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiTexCoord2fVertex3fSUN (rc, s, t, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fVertex3fvSUN (const GLuint *rc, const GLfloat *tc, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiTexCoord2fVertex3fvSUN (rc, tc, v);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN (rc, s, t, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN (rc, tc, n, v);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z) {
  _glr->glExt.glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN (rc, s, t, r, g, b, a, nx, ny, nz, x, y, z);}
inline GLAPI void APIENTRY glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN (const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v) {
  _glr->glExt.glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN (rc, tc, c, n, v);}

#endif /// not using OSI_USING_GLEXT_DEFINES



#endif /// OS_LINUX + OS_WIN









///=======================///
// MODE 2: using #define's //
///=======================///

#ifdef OSI_USING_GLEXT_DEFINES


///====================///
// OpenGL version funcs //
///====================///


/// OpenGL 1.2 funcs =================------------------------------
#define glDrawRangeElements _glr->glExt.glDrawRangeElements
#define glTexImage3D _glr->glExt.glTexImage3D
#define glTexSubImage3D _glr->glExt.glTexSubImage3D
#define glCopyTexSubImage3D _glr->glExt.glCopyTexSubImage3D

/// OpenGL 1.3 funcs =================------------------------------
#define glActiveTexture _glr->glExt.glActiveTexture
#define glSampleCoverage _glr->glExt.glSampleCoverage
#define glCompressedTexImage3D _glr->glExt.glCompressedTexImage3D
#define glCompressedTexImage2D _glr->glExt.glCompressedTexImage2D
#define glCompressedTexImage1D _glr->glExt.glCompressedTexImage1D
#define glCompressedTexSubImage3D _glr->glExt.glCompressedTexSubImage3D
#define glCompressedTexSubImage2D _glr->glExt.glCompressedTexSubImage2D
#define glCompressedTexSubImage1D _glr->glExt.glCompressedTexSubImage1D
#define glGetCompressedTexImage _glr->glExt.glGetCompressedTexImage
#ifdef OS_WIN
#define glClientActiveTexture _glr->glExt.glClientActiveTexture
#define glMultiTexCoord1d _glr->glExt.glMultiTexCoord1d
#define glMultiTexCoord1dv _glr->glExt.glMultiTexCoord1dv
#define glMultiTexCoord1f _glr->glExt.glMultiTexCoord1f
#define glMultiTexCoord1fv _glr->glExt.glMultiTexCoord1fv
#define glMultiTexCoord1i _glr->glExt.glMultiTexCoord1i
#define glMultiTexCoord1iv _glr->glExt.glMultiTexCoord1iv
#define glMultiTexCoord1s _glr->glExt.glMultiTexCoord1s
#define glMultiTexCoord1sv _glr->glExt.glMultiTexCoord1sv
#define glMultiTexCoord2d _glr->glExt.glMultiTexCoord2d
#define glMultiTexCoord2dv _glr->glExt.glMultiTexCoord2dv
#define glMultiTexCoord2f _glr->glExt.glMultiTexCoord2f
#define glMultiTexCoord2fv _glr->glExt.glMultiTexCoord2fv
#define glMultiTexCoord2i _glr->glExt.glMultiTexCoord2i
#define glMultiTexCoord2iv _glr->glExt.glMultiTexCoord2iv
#define glMultiTexCoord2s _glr->glExt.glMultiTexCoord2s
#define glMultiTexCoord2sv _glr->glExt.glMultiTexCoord2sv
#define glMultiTexCoord3d _glr->glExt.glMultiTexCoord3d
#define glMultiTexCoord3dv _glr->glExt.glMultiTexCoord3dv
#define glMultiTexCoord3f _glr->glExt.glMultiTexCoord3f
#define glMultiTexCoord3fv _glr->glExt.glMultiTexCoord3fv
#define glMultiTexCoord3i _glr->glExt.glMultiTexCoord3i
#define glMultiTexCoord3iv _glr->glExt.glMultiTexCoord3iv
#define glMultiTexCoord3s _glr->glExt.glMultiTexCoord3s
#define glMultiTexCoord3sv _glr->glExt.glMultiTexCoord3sv
#define glMultiTexCoord4d _glr->glExt.glMultiTexCoord4d
#define glMultiTexCoord4dv _glr->glExt.glMultiTexCoord4dv
#define glMultiTexCoord4f _glr->glExt.glMultiTexCoord4f
#define glMultiTexCoord4fv _glr->glExt.glMultiTexCoord4fv
#define glMultiTexCoord4i _glr->glExt.glMultiTexCoord4i
#define glMultiTexCoord4iv _glr->glExt.glMultiTexCoord4iv
#define glMultiTexCoord4s _glr->glExt.glMultiTexCoord4s
#define glMultiTexCoord4sv _glr->glExt.glMultiTexCoord4sv
#define glLoadTransposeMatrixf _glr->glExt.glLoadTransposeMatrixf
#define glLoadTransposeMatrixd _glr->glExt.glLoadTransposeMatrixd
#define glMultTransposeMatrixf _glr->glExt.glMultTransposeMatrixf
#define glMultTransposeMatrixd _glr->glExt.glMultTransposeMatrixd
#endif /// OS_WIN

/// OpenGL 1.4 funcs =================------------------------------
#define glBlendFuncSeparate _glr->glExt.glBlendFuncSeparate
#define glMultiDrawArrays _glr->glExt.glMultiDrawArrays
#define glMultiDrawElements _glr->glExt.glMultiDrawElements
#define glPointParameterf _glr->glExt.glPointParameterf
#define glPointParameterfv _glr->glExt.glPointParameterfv
#define glPointParameteri _glr->glExt.glPointParameteri
#define glPointParameteriv _glr->glExt.glPointParameteriv
#define glFogCoordf _glr->glExt.glFogCoordf
#define glFogCoordfv _glr->glExt.glFogCoordfv
#define glFogCoordd _glr->glExt.glFogCoordd
#define glFogCoorddv _glr->glExt.glFogCoorddv
#define glFogCoordPointer _glr->glExt.glFogCoordPointer
#define glSecondaryColor3b _glr->glExt.glSecondaryColor3b
#define glSecondaryColor3bv _glr->glExt.glSecondaryColor3bv
#define glSecondaryColor3d _glr->glExt.glSecondaryColor3d
#define glSecondaryColor3dv _glr->glExt.glSecondaryColor3dv
#define glSecondaryColor3f _glr->glExt.glSecondaryColor3f
#define glSecondaryColor3fv _glr->glExt.glSecondaryColor3fv
#define glSecondaryColor3i _glr->glExt.glSecondaryColor3i
#define glSecondaryColor3iv _glr->glExt.glSecondaryColor3iv
#define glSecondaryColor3s _glr->glExt.glSecondaryColor3s
#define glSecondaryColor3sv _glr->glExt.glSecondaryColor3sv
#define glSecondaryColor3ub _glr->glExt.glSecondaryColor3ub
#define glSecondaryColor3ubv _glr->glExt.glSecondaryColor3ubv
#define glSecondaryColor3ui _glr->glExt.glSecondaryColor3ui
#define glSecondaryColor3uiv _glr->glExt.glSecondaryColor3uiv
#define glSecondaryColor3us _glr->glExt.glSecondaryColor3us
#define glSecondaryColor3usv _glr->glExt.glSecondaryColor3usv
#define glSecondaryColorPointer _glr->glExt.glSecondaryColorPointer
#define glWindowPos2d _glr->glExt.glWindowPos2d
#define glWindowPos2dv _glr->glExt.glWindowPos2dv
#define glWindowPos2f _glr->glExt.glWindowPos2f
#define glWindowPos2fv _glr->glExt.glWindowPos2fv
#define glWindowPos2i _glr->glExt.glWindowPos2i
#define glWindowPos2iv _glr->glExt.glWindowPos2iv
#define glWindowPos2s _glr->glExt.glWindowPos2s
#define glWindowPos2sv _glr->glExt.glWindowPos2sv
#define glWindowPos3d _glr->glExt.glWindowPos3d
#define glWindowPos3dv _glr->glExt.glWindowPos3dv
#define glWindowPos3f _glr->glExt.glWindowPos3f
#define glWindowPos3fv _glr->glExt.glWindowPos3fv
#define glWindowPos3i _glr->glExt.glWindowPos3i
#define glWindowPos3iv _glr->glExt.glWindowPos3iv
#define glWindowPos3s _glr->glExt.glWindowPos3s
#define glWindowPos3sv _glr->glExt.glWindowPos3sv
#define glBlendColor _glr->glExt.glBlendColor
#define glBlendEquation _glr->glExt.glBlendEquation

/// OpenGL 1.5 funcs =================------------------------------
#define glGenQueries _glr->glExt.glGenQueries
#define glDeleteQueries _glr->glExt.glDeleteQueries
#define glIsQuery _glr->glExt.glIsQuery
#define glBeginQuery _glr->glExt.glBeginQuery
#define glEndQuery _glr->glExt.glEndQuery
#define glGetQueryiv _glr->glExt.glGetQueryiv
#define glGetQueryObjectiv _glr->glExt.glGetQueryObjectiv
#define glGetQueryObjectuiv _glr->glExt.glGetQueryObjectuiv
#define glBindBuffer _glr->glExt.glBindBuffer
#define glDeleteBuffers _glr->glExt.glDeleteBuffers
#define glGenBuffers _glr->glExt.glGenBuffers
#define glIsBuffer _glr->glExt.glIsBuffer
#define glBufferData _glr->glExt.glBufferData
#define glBufferSubData _glr->glExt.glBufferSubData
#define glGetBufferSubData _glr->glExt.glGetBufferSubData
#define glMapBuffer _glr->glExt.glMapBuffer
#define glUnmapBuffer _glr->glExt.glUnmapBuffer
#define glGetBufferParameteriv _glr->glExt.glGetBufferParameteriv
#define glGetBufferPointerv _glr->glExt.glGetBufferPointerv

/// OpenGL 2.0 funcs =================------------------------------
#define glBlendEquationSeparate _glr->glExt.glBlendEquationSeparate
#define glDrawBuffers _glr->glExt.glDrawBuffers
#define glStencilOpSeparate _glr->glExt.glStencilOpSeparate
#define glStencilFuncSeparate _glr->glExt.glStencilFuncSeparate
#define glStencilMaskSeparate _glr->glExt.glStencilMaskSeparate
#define glAttachShader _glr->glExt.glAttachShader
#define glBindAttribLocation _glr->glExt.glBindAttribLocation
#define glCompileShader _glr->glExt.glCompileShader
#define glCreateProgram _glr->glExt.glCreateProgram
#define glCreateShader _glr->glExt.glCreateShader
#define glDeleteProgram _glr->glExt.glDeleteProgram
#define glDeleteShader _glr->glExt.glDeleteShader
#define glDetachShader _glr->glExt.glDetachShader
#define glDisableVertexAttribArray _glr->glExt.glDisableVertexAttribArray
#define glEnableVertexAttribArray _glr->glExt.glEnableVertexAttribArray
#define glGetActiveAttrib _glr->glExt.glGetActiveAttrib
#define glGetActiveUniform _glr->glExt.glGetActiveUniform
#define glGetAttachedShaders _glr->glExt.glGetAttachedShaders
#define glGetAttribLocation _glr->glExt.glGetAttribLocation
#define glGetProgramiv _glr->glExt.glGetProgramiv
#define glGetProgramInfoLog _glr->glExt.glGetProgramInfoLog
#define glGetShaderiv _glr->glExt.glGetShaderiv
#define glGetShaderInfoLog _glr->glExt.glGetShaderInfoLog
#define glGetShaderSource _glr->glExt.glGetShaderSource
#define glGetUniformLocation _glr->glExt.glGetUniformLocation
#define glGetUniformfv _glr->glExt.glGetUniformfv
#define glGetUniformiv _glr->glExt.glGetUniformiv
#define glGetVertexAttribdv _glr->glExt.glGetVertexAttribdv
#define glGetVertexAttribfv _glr->glExt.glGetVertexAttribfv
#define glGetVertexAttribiv _glr->glExt.glGetVertexAttribiv
#define glGetVertexAttribPointerv _glr->glExt.glGetVertexAttribPointerv
#define glIsProgram _glr->glExt.glIsProgram
#define glIsShader _glr->glExt.glIsShader
#define glLinkProgram _glr->glExt.glLinkProgram
#define glShaderSource _glr->glExt.glShaderSource
#define glUseProgram _glr->glExt.glUseProgram
#define glUniform1f _glr->glExt.glUniform1f
#define glUniform2f _glr->glExt.glUniform2f
#define glUniform3f _glr->glExt.glUniform3f
#define glUniform4f _glr->glExt.glUniform4f
#define glUniform1i _glr->glExt.glUniform1i
#define glUniform2i _glr->glExt.glUniform2i
#define glUniform3i _glr->glExt.glUniform3i
#define glUniform4i _glr->glExt.glUniform4i
#define glUniform1fv _glr->glExt.glUniform1fv
#define glUniform2fv _glr->glExt.glUniform2fv
#define glUniform3fv _glr->glExt.glUniform3fv
#define glUniform4fv _glr->glExt.glUniform4fv
#define glUniform1iv _glr->glExt.glUniform1iv
#define glUniform2iv _glr->glExt.glUniform2iv
#define glUniform3iv _glr->glExt.glUniform3iv
#define glUniform4iv _glr->glExt.glUniform4iv
#define glUniformMatrix2fv _glr->glExt.glUniformMatrix2fv
#define glUniformMatrix3fv _glr->glExt.glUniformMatrix3fv
#define glUniformMatrix4fv _glr->glExt.glUniformMatrix4fv
#define glValidateProgram _glr->glExt.glValidateProgram
#define glVertexAttrib1d _glr->glExt.glVertexAttrib1d
#define glVertexAttrib1dv _glr->glExt.glVertexAttrib1dv
#define glVertexAttrib1f _glr->glExt.glVertexAttrib1f
#define glVertexAttrib1fv _glr->glExt.glVertexAttrib1fv
#define glVertexAttrib1s _glr->glExt.glVertexAttrib1s
#define glVertexAttrib1sv _glr->glExt.glVertexAttrib1sv
#define glVertexAttrib2d _glr->glExt.glVertexAttrib2d
#define glVertexAttrib2dv _glr->glExt.glVertexAttrib2dv
#define glVertexAttrib2f _glr->glExt.glVertexAttrib2f
#define glVertexAttrib2fv _glr->glExt.glVertexAttrib2fv
#define glVertexAttrib2s _glr->glExt.glVertexAttrib2s
#define glVertexAttrib2sv _glr->glExt.glVertexAttrib2sv
#define glVertexAttrib3d _glr->glExt.glVertexAttrib3d
#define glVertexAttrib3dv _glr->glExt.glVertexAttrib3dv
#define glVertexAttrib3f _glr->glExt.glVertexAttrib3f
#define glVertexAttrib3fv _glr->glExt.glVertexAttrib3fv
#define glVertexAttrib3s _glr->glExt.glVertexAttrib3s
#define glVertexAttrib3sv _glr->glExt.glVertexAttrib3sv
#define glVertexAttrib4Nbv _glr->glExt.glVertexAttrib4Nbv
#define glVertexAttrib4Niv _glr->glExt.glVertexAttrib4Niv
#define glVertexAttrib4Nsv _glr->glExt.glVertexAttrib4Nsv
#define glVertexAttrib4Nub _glr->glExt.glVertexAttrib4Nub
#define glVertexAttrib4Nubv _glr->glExt.glVertexAttrib4Nubv
#define glVertexAttrib4Nuiv _glr->glExt.glVertexAttrib4Nuiv
#define glVertexAttrib4Nusv _glr->glExt.glVertexAttrib4Nusv
#define glVertexAttrib4bv _glr->glExt.glVertexAttrib4bv
#define glVertexAttrib4d _glr->glExt.glVertexAttrib4d
#define glVertexAttrib4dv _glr->glExt.glVertexAttrib4dv
#define glVertexAttrib4f _glr->glExt.glVertexAttrib4f
#define glVertexAttrib4fv _glr->glExt.glVertexAttrib4fv
#define glVertexAttrib4iv _glr->glExt.glVertexAttrib4iv
#define glVertexAttrib4s _glr->glExt.glVertexAttrib4s
#define glVertexAttrib4sv _glr->glExt.glVertexAttrib4sv
#define glVertexAttrib4ubv _glr->glExt.glVertexAttrib4ubv
#define glVertexAttrib4uiv _glr->glExt.glVertexAttrib4uiv
#define glVertexAttrib4usv _glr->glExt.glVertexAttrib4usv
#define glVertexAttribPointer _glr->glExt.glVertexAttribPointer

/// OpenGL 2.1 funcs =================------------------------------
#define glUniformMatrix2x3fv _glr->glExt.glUniformMatrix2x3fv
#define glUniformMatrix3x2fv _glr->glExt.glUniformMatrix3x2fv
#define glUniformMatrix2x4fv _glr->glExt.glUniformMatrix2x4fv
#define glUniformMatrix4x2fv _glr->glExt.glUniformMatrix4x2fv
#define glUniformMatrix3x4fv _glr->glExt.glUniformMatrix3x4fv
#define glUniformMatrix4x3fv _glr->glExt.glUniformMatrix4x3fv

/// OpenGL 3.0 funcs =================------------------------------
#define glColorMaski _glr->glExt.glColorMaski
#define glGetBooleani_v _glr->glExt.glGetBooleani_v
#define glGetIntegeri_v _glr->glExt.glGetIntegeri_v
#define glEnablei _glr->glExt.glEnablei
#define glDisablei _glr->glExt.glDisablei
#define glIsEnabledi _glr->glExt.glIsEnabledi
#define glBeginTransformFeedback _glr->glExt.glBeginTransformFeedback
#define glEndTransformFeedback _glr->glExt.glEndTransformFeedback
#define glBindBufferRange _glr->glExt.glBindBufferRange
#define glBindBufferBase _glr->glExt.glBindBufferBase
#define glTransformFeedbackVaryings _glr->glExt.glTransformFeedbackVaryings
#define glGetTransformFeedbackVarying _glr->glExt.glGetTransformFeedbackVarying
#define glClampColor _glr->glExt.glClampColor
#define glBeginConditionalRender _glr->glExt.glBeginConditionalRender
#define glEndConditionalRender _glr->glExt.glEndConditionalRender
#define glVertexAttribIPointer _glr->glExt.glVertexAttribIPointer
#define glGetVertexAttribIiv _glr->glExt.glGetVertexAttribIiv
#define glGetVertexAttribIuiv _glr->glExt.glGetVertexAttribIuiv
#define glVertexAttribI1i _glr->glExt.glVertexAttribI1i
#define glVertexAttribI2i _glr->glExt.glVertexAttribI2i
#define glVertexAttribI3i _glr->glExt.glVertexAttribI3i
#define glVertexAttribI4i _glr->glExt.glVertexAttribI4i
#define glVertexAttribI1ui _glr->glExt.glVertexAttribI1ui
#define glVertexAttribI2ui _glr->glExt.glVertexAttribI2ui
#define glVertexAttribI3ui _glr->glExt.glVertexAttribI3ui
#define glVertexAttribI4ui _glr->glExt.glVertexAttribI4ui
#define glVertexAttribI1iv _glr->glExt.glVertexAttribI1iv
#define glVertexAttribI2iv _glr->glExt.glVertexAttribI2iv
#define glVertexAttribI3iv _glr->glExt.glVertexAttribI3iv
#define glVertexAttribI4iv _glr->glExt.glVertexAttribI4iv
#define glVertexAttribI1uiv _glr->glExt.glVertexAttribI1uiv
#define glVertexAttribI2uiv _glr->glExt.glVertexAttribI2uiv
#define glVertexAttribI3uiv _glr->glExt.glVertexAttribI3uiv
#define glVertexAttribI4uiv _glr->glExt.glVertexAttribI4uiv
#define glVertexAttribI4bv _glr->glExt.glVertexAttribI4bv
#define glVertexAttribI4sv _glr->glExt.glVertexAttribI4sv
#define glVertexAttribI4ubv _glr->glExt.glVertexAttribI4ubv
#define glVertexAttribI4usv _glr->glExt.glVertexAttribI4usv
#define glGetUniformuiv _glr->glExt.glGetUniformuiv
#define glBindFragDataLocation _glr->glExt.glBindFragDataLocation
#define glGetFragDataLocation return _glr->glExt.glGetFragDataLocation
#define glUniform1ui _glr->glExt.glUniform1ui
#define glUniform2ui _glr->glExt.glUniform2ui
#define glUniform3ui _glr->glExt.glUniform3ui
#define glUniform4ui _glr->glExt.glUniform4ui
#define glUniform1uiv _glr->glExt.glUniform1uiv
#define glUniform2uiv _glr->glExt.glUniform2uiv
#define glUniform3uiv _glr->glExt.glUniform3uiv
#define glUniform4uiv _glr->glExt.glUniform4uiv
#define glTexParameterIiv _glr->glExt.glTexParameterIiv
#define glTexParameterIuiv _glr->glExt.glTexParameterIuiv
#define glGetTexParameterIiv _glr->glExt.glGetTexParameterIiv
#define glGetTexParameterIuiv _glr->glExt.glGetTexParameterIuiv
#define glClearBufferiv _glr->glExt.glClearBufferiv
#define glClearBufferuiv _glr->glExt.glClearBufferuiv
#define glClearBufferfv _glr->glExt.glClearBufferfv
#define glClearBufferfi _glr->glExt.glClearBufferfi
#define glGetStringi _glr->glExt.glGetStringi
#define glIsRenderbuffer _glr->glExt.glIsRenderbuffer
#define glBindRenderbuffer _glr->glExt.glBindRenderbuffer
#define glDeleteRenderbuffers _glr->glExt.glDeleteRenderbuffers
#define glGenRenderbuffers _glr->glExt.glGenRenderbuffers
#define glRenderbufferStorage _glr->glExt.glRenderbufferStorage
#define glGetRenderbufferParameteriv _glr->glExt.glGetRenderbufferParameteriv
#define glIsFramebuffer _glr->glExt.glIsFramebuffer
#define glBindFramebuffer _glr->glExt.glBindFramebuffer
#define glDeleteFramebuffers _glr->glExt.glDeleteFramebuffers
#define glGenFramebuffers _glr->glExt.glGenFramebuffers
#define glCheckFramebufferStatus _glr->glExt.glCheckFramebufferStatus
#define glFramebufferTexture1D _glr->glExt.glFramebufferTexture1D
#define glFramebufferTexture2D _glr->glExt.glFramebufferTexture2D
#define glFramebufferTexture3D _glr->glExt.glFramebufferTexture3D
#define glFramebufferRenderbuffer _glr->glExt.glFramebufferRenderbuffer
#define glGetFramebufferAttachmentParameteriv _glr->glExt.glGetFramebufferAttachmentParameteriv
#define glGenerateMipmap _glr->glExt.glGenerateMipmap
#define glBlitFramebuffer _glr->glExt.glBlitFramebuffer
#define glRenderbufferStorageMultisample _glr->glExt.glRenderbufferStorageMultisample
#define glFramebufferTextureLayer _glr->glExt.glFramebufferTextureLayer
#define glMapBufferRange _glr->glExt.glMapBufferRange
#define glFlushMappedBufferRange _glr->glExt.glFlushMappedBufferRange
#define glBindVertexArray _glr->glExt.glBindVertexArray
#define glDeleteVertexArrays _glr->glExt.glDeleteVertexArrays
#define glGenVertexArrays _glr->glExt.glGenVertexArrays
#define glIsVertexArray _glr->glExt.glIsVertexArray

/// OpenGL 3.1 funcs =================------------------------------
#define glDrawArraysInstanced _glr->glExt.glDrawArraysInstanced
#define glDrawElementsInstanced _glr->glExt.glDrawElementsInstanced
#define glTexBuffer _glr->glExt.glTexBuffer
#define glPrimitiveRestartIndex _glr->glExt.glPrimitiveRestartIndex
#define glCopyBufferSubData _glr->glExt.glCopyBufferSubData
#define glGetUniformIndices _glr->glExt.glGetUniformIndices
#define glGetActiveUniformsiv _glr->glExt.glGetActiveUniformsiv
#define glGetActiveUniformName _glr->glExt.glGetActiveUniformName
#define glGetUniformBlockIndex return _glr->glExt.glGetUniformBlockIndex
#define glGetActiveUniformBlockiv _glr->glExt.glGetActiveUniformBlockiv
#define glGetActiveUniformBlockName _glr->glExt.glGetActiveUniformBlockName
#define glUniformBlockBinding _glr->glExt.glUniformBlockBinding

/// OpenGL 3.2 funcs =================------------------------------
#define glDrawElementsBaseVertex _glr->glExt.glDrawElementsBaseVertex
#define glDrawRangeElementsBaseVertex _glr->glExt.glDrawRangeElementsBaseVertex
#define glDrawElementsInstancedBaseVertex _glr->glExt.glDrawElementsInstancedBaseVertex
#define glMultiDrawElementsBaseVertex _glr->glExt.glMultiDrawElementsBaseVertex
#define glProvokingVertex _glr->glExt.glProvokingVertex
#define glFenceSync _glr->glExt.glFenceSync
#define glIsSync _glr->glExt.glIsSync
#define glDeleteSync _glr->glExt.glDeleteSync
#define glClientWaitSync _glr->glExt.glClientWaitSync
#define glWaitSync _glr->glExt.glWaitSync
#define glGetInteger64v _glr->glExt.glGetInteger64v
#define glGetSynciv _glr->glExt.glGetSynciv
#define glGetInteger64i_v _glr->glExt.glGetInteger64i_v
#define glGetBufferParameteri64v _glr->glExt.glGetBufferParameteri64v
#define glFramebufferTexture _glr->glExt.glFramebufferTexture
#define glTexImage2DMultisample _glr->glExt.glTexImage2DMultisample
#define glTexImage3DMultisample _glr->glExt.glTexImage3DMultisample
#define glGetMultisamplefv _glr->glExt.glGetMultisamplefv
#define glSampleMaski _glr->glExt.glSampleMaski

/// OpenGL 3.3 funcs =================------------------------------
#define glBindFragDataLocationIndexed _glr->glExt.glBindFragDataLocationIndexed
#define glGetFragDataIndex return _glr->glExt.glGetFragDataIndex
#define glGenSamplers _glr->glExt.glGenSamplers
#define glDeleteSamplers _glr->glExt.glDeleteSamplers
#define glIsSampler _glr->glExt.glIsSampler
#define glBindSampler _glr->glExt.glBindSampler
#define glSamplerParameteri _glr->glExt.glSamplerParameteri
#define glSamplerParameteriv _glr->glExt.glSamplerParameteriv
#define glSamplerParameterf _glr->glExt.glSamplerParameterf
#define glSamplerParameterfv _glr->glExt.glSamplerParameterfv
#define glSamplerParameterIiv _glr->glExt.glSamplerParameterIiv
#define glSamplerParameterIuiv _glr->glExt.glSamplerParameterIuiv
#define glGetSamplerParameteriv _glr->glExt.glGetSamplerParameteriv
#define glGetSamplerParameterIiv _glr->glExt.glGetSamplerParameterIiv
#define glGetSamplerParameterfv _glr->glExt.glGetSamplerParameterfv
#define glGetSamplerParameterIuiv _glr->glExt.glGetSamplerParameterIuiv
#define glQueryCounter _glr->glExt.glQueryCounter
#define glGetQueryObjecti64v _glr->glExt.glGetQueryObjecti64v
#define glGetQueryObjectui64v _glr->glExt.glGetQueryObjectui64v
#define glVertexAttribDivisor _glr->glExt.glVertexAttribDivisor
#define glVertexAttribP1ui _glr->glExt.glVertexAttribP1ui
#define glVertexAttribP1uiv _glr->glExt.glVertexAttribP1uiv
#define glVertexAttribP2ui _glr->glExt.glVertexAttribP2ui
#define glVertexAttribP2uiv _glr->glExt.glVertexAttribP2uiv
#define glVertexAttribP3ui _glr->glExt.glVertexAttribP3ui
#define glVertexAttribP3uiv _glr->glExt.glVertexAttribP3uiv
#define glVertexAttribP4ui _glr->glExt.glVertexAttribP4ui
#define glVertexAttribP4uiv _glr->glExt.glVertexAttribP4uiv
#define glVertexP2ui _glr->glExt.glVertexP2ui
#define glVertexP2uiv _glr->glExt.glVertexP2uiv
#define glVertexP3ui _glr->glExt.glVertexP3ui
#define glVertexP3uiv _glr->glExt.glVertexP3uiv
#define glVertexP4ui _glr->glExt.glVertexP4ui
#define glVertexP4uiv _glr->glExt.glVertexP4uiv
#define glTexCoordP1ui _glr->glExt.glTexCoordP1ui
#define glTexCoordP1uiv _glr->glExt.glTexCoordP1uiv
#define glTexCoordP2ui _glr->glExt.glTexCoordP2ui
#define glTexCoordP2uiv _glr->glExt.glTexCoordP2uiv
#define glTexCoordP3ui _glr->glExt.glTexCoordP3ui
#define glTexCoordP3uiv _glr->glExt.glTexCoordP3uiv
#define glTexCoordP4ui _glr->glExt.glTexCoordP4ui
#define glTexCoordP4uiv _glr->glExt.glTexCoordP4uiv
#define glMultiTexCoordP1ui _glr->glExt.glMultiTexCoordP1ui
#define glMultiTexCoordP1uiv _glr->glExt.glMultiTexCoordP1uiv
#define glMultiTexCoordP2ui _glr->glExt.glMultiTexCoordP2ui
#define glMultiTexCoordP2uiv _glr->glExt.glMultiTexCoordP2uiv
#define glMultiTexCoordP3ui _glr->glExt.glMultiTexCoordP3ui
#define glMultiTexCoordP3uiv _glr->glExt.glMultiTexCoordP3uiv
#define glMultiTexCoordP4ui _glr->glExt.glMultiTexCoordP4ui
#define glMultiTexCoordP4uiv _glr->glExt.glMultiTexCoordP4uiv
#define glNormalP3ui _glr->glExt.glNormalP3ui
#define glNormalP3uiv _glr->glExt.glNormalP3uiv
#define glColorP3ui _glr->glExt.glColorP3ui
#define glColorP3uiv _glr->glExt.glColorP3uiv
#define glColorP4ui _glr->glExt.glColorP4ui
#define glColorP4uiv _glr->glExt.glColorP4uiv
#define glSecondaryColorP3ui _glr->glExt.glSecondaryColorP3ui
#define glSecondaryColorP3uiv _glr->glExt.glSecondaryColorP3uiv

/// OpenGL 4.0 funcs =================------------------------------
#define glMinSampleShading _glr->glExt.glMinSampleShading
#define glBlendEquationi _glr->glExt.glBlendEquationi
#define glBlendEquationSeparatei _glr->glExt.glBlendEquationSeparatei
#define glBlendFunci _glr->glExt.glBlendFunci
#define glBlendFuncSeparatei _glr->glExt.glBlendFuncSeparatei
#define glDrawArraysIndirect _glr->glExt.glDrawArraysIndirect
#define glDrawElementsIndirect _glr->glExt.glDrawElementsIndirect
#define glUniform1d _glr->glExt.glUniform1d
#define glUniform2d _glr->glExt.glUniform2d
#define glUniform3d _glr->glExt.glUniform3d
#define glUniform4d _glr->glExt.glUniform4d
#define glUniform1dv _glr->glExt.glUniform1dv
#define glUniform2dv _glr->glExt.glUniform2dv
#define glUniform3dv _glr->glExt.glUniform3dv
#define glUniform4dv _glr->glExt.glUniform4dv
#define glUniformMatrix2dv _glr->glExt.glUniformMatrix2dv
#define glUniformMatrix3dv _glr->glExt.glUniformMatrix3dv
#define glUniformMatrix4dv _glr->glExt.glUniformMatrix4dv
#define glUniformMatrix2x3dv _glr->glExt.glUniformMatrix2x3dv
#define glUniformMatrix2x4dv _glr->glExt.glUniformMatrix2x4dv
#define glUniformMatrix3x2dv _glr->glExt.glUniformMatrix3x2dv
#define glUniformMatrix3x4dv _glr->glExt.glUniformMatrix3x4dv
#define glUniformMatrix4x2dv _glr->glExt.glUniformMatrix4x2dv
#define glUniformMatrix4x3dv _glr->glExt.glUniformMatrix4x3dv
#define glGetUniformdv _glr->glExt.glGetUniformdv
#define glGetSubroutineUniformLocation _glr->glExt.glGetSubroutineUniformLocation
#define glGetSubroutineIndex _glr->glExt.glGetSubroutineIndex
#define glGetActiveSubroutineUniformiv _glr->glExt.glGetActiveSubroutineUniformiv
#define glGetActiveSubroutineUniformName _glr->glExt.glGetActiveSubroutineUniformName
#define glGetActiveSubroutineName _glr->glExt.glGetActiveSubroutineName
#define glUniformSubroutinesuiv _glr->glExt.glUniformSubroutinesuiv
#define glGetUniformSubroutineuiv _glr->glExt.glGetUniformSubroutineuiv
#define glGetProgramStageiv _glr->glExt.glGetProgramStageiv
#define glPatchParameteri _glr->glExt.glPatchParameteri
#define glPatchParameterfv _glr->glExt.glPatchParameterfv
#define glBindTransformFeedback _glr->glExt.glBindTransformFeedback
#define glDeleteTransformFeedbacks _glr->glExt.glDeleteTransformFeedbacks
#define glGenTransformFeedbacks _glr->glExt.glGenTransformFeedbacks
#define glIsTransformFeedback _glr->glExt.glIsTransformFeedback
#define glPauseTransformFeedback _glr->glExt.glPauseTransformFeedback
#define glResumeTransformFeedback _glr->glExt.glResumeTransformFeedback
#define glDrawTransformFeedback _glr->glExt.glDrawTransformFeedback
#define glDrawTransformFeedbackStream _glr->glExt.glDrawTransformFeedbackStream
#define glBeginQueryIndexed _glr->glExt.glBeginQueryIndexed
#define glEndQueryIndexed _glr->glExt.glEndQueryIndexed
#define glGetQueryIndexediv _glr->glExt.glGetQueryIndexediv

/// OpenGL 4.1 funcs =================------------------------------
#define glReleaseShaderCompiler _glr->glExt.glReleaseShaderCompiler
#define glShaderBinary _glr->glExt.glShaderBinary
#define glGetShaderPrecisionFormat _glr->glExt.glGetShaderPrecisionFormat
#define glDepthRangef _glr->glExt.glDepthRangef
#define glClearDepthf _glr->glExt.glClearDepthf
#define glGetProgramBinary _glr->glExt.glGetProgramBinary
#define glProgramBinary _glr->glExt.glProgramBinary
#define glProgramParameteri _glr->glExt.glProgramParameteri
#define glUseProgramStages _glr->glExt.glUseProgramStages
#define glActiveShaderProgram _glr->glExt.glActiveShaderProgram
#define glCreateShaderProgramv _glr->glExt.glCreateShaderProgramv
#define glBindProgramPipeline _glr->glExt.glBindProgramPipeline
#define glDeleteProgramPipelines _glr->glExt.glDeleteProgramPipelines
#define glGenProgramPipelines _glr->glExt.glGenProgramPipelines
#define glIsProgramPipeline _glr->glExt.glIsProgramPipeline
#define glGetProgramPipelineiv _glr->glExt.glGetProgramPipelineiv
#define glProgramUniform1i _glr->glExt.glProgramUniform1i
#define glProgramUniform1iv _glr->glExt.glProgramUniform1iv
#define glProgramUniform1f _glr->glExt.glProgramUniform1f
#define glProgramUniform1fv _glr->glExt.glProgramUniform1fv
#define glProgramUniform1d _glr->glExt.glProgramUniform1d
#define glProgramUniform1dv _glr->glExt.glProgramUniform1dv
#define glProgramUniform1ui _glr->glExt.glProgramUniform1ui
#define glProgramUniform1uiv _glr->glExt.glProgramUniform1uiv
#define glProgramUniform2i _glr->glExt.glProgramUniform2i
#define glProgramUniform2iv _glr->glExt.glProgramUniform2iv
#define glProgramUniform2f _glr->glExt.glProgramUniform2f
#define glProgramUniform2fv _glr->glExt.glProgramUniform2fv
#define glProgramUniform2d _glr->glExt.glProgramUniform2d
#define glProgramUniform2dv _glr->glExt.glProgramUniform2dv
#define glProgramUniform2ui _glr->glExt.glProgramUniform2ui
#define glProgramUniform2uiv _glr->glExt.glProgramUniform2uiv
#define glProgramUniform3i _glr->glExt.glProgramUniform3i
#define glProgramUniform3iv _glr->glExt.glProgramUniform3iv
#define glProgramUniform3f _glr->glExt.glProgramUniform3f
#define glProgramUniform3fv _glr->glExt.glProgramUniform3fv
#define glProgramUniform3d _glr->glExt.glProgramUniform3d
#define glProgramUniform3dv _glr->glExt.glProgramUniform3dv
#define glProgramUniform3ui _glr->glExt.glProgramUniform3ui
#define glProgramUniform3uiv _glr->glExt.glProgramUniform3uiv
#define glProgramUniform4i _glr->glExt.glProgramUniform4i
#define glProgramUniform4iv _glr->glExt.glProgramUniform4iv
#define glProgramUniform4f _glr->glExt.glProgramUniform4f
#define glProgramUniform4fv _glr->glExt.glProgramUniform4fv
#define glProgramUniform4d _glr->glExt.glProgramUniform4d
#define glProgramUniform4dv _glr->glExt.glProgramUniform4dv
#define glProgramUniform4ui _glr->glExt.glProgramUniform4ui
#define glProgramUniform4uiv _glr->glExt.glProgramUniform4uiv
#define glProgramUniformMatrix2fv _glr->glExt.glProgramUniformMatrix2fv
#define glProgramUniformMatrix3fv _glr->glExt.glProgramUniformMatrix3fv
#define glProgramUniformMatrix4fv _glr->glExt.glProgramUniformMatrix4fv
#define glProgramUniformMatrix2dv _glr->glExt.glProgramUniformMatrix2dv
#define glProgramUniformMatrix3dv _glr->glExt.glProgramUniformMatrix3dv
#define glProgramUniformMatrix4dv _glr->glExt.glProgramUniformMatrix4dv
#define glProgramUniformMatrix2x3fv _glr->glExt.glProgramUniformMatrix2x3fv
#define glProgramUniformMatrix3x2fv _glr->glExt.glProgramUniformMatrix3x2fv
#define glProgramUniformMatrix2x4fv _glr->glExt.glProgramUniformMatrix2x4fv
#define glProgramUniformMatrix4x2fv _glr->glExt.glProgramUniformMatrix4x2fv
#define glProgramUniformMatrix3x4fv _glr->glExt.glProgramUniformMatrix3x4fv
#define glProgramUniformMatrix4x3fv _glr->glExt.glProgramUniformMatrix4x3fv
#define glProgramUniformMatrix2x3dv _glr->glExt.glProgramUniformMatrix2x3dv
#define glProgramUniformMatrix3x2dv _glr->glExt.glProgramUniformMatrix3x2dv
#define glProgramUniformMatrix2x4dv _glr->glExt.glProgramUniformMatrix2x4dv
#define glProgramUniformMatrix4x2dv _glr->glExt.glProgramUniformMatrix4x2dv
#define glProgramUniformMatrix3x4dv _glr->glExt.glProgramUniformMatrix3x4dv
#define glProgramUniformMatrix4x3dv _glr->glExt.glProgramUniformMatrix4x3dv
#define glValidateProgramPipeline _glr->glExt.glValidateProgramPipeline
#define glGetProgramPipelineInfoLog _glr->glExt.glGetProgramPipelineInfoLog
#define glVertexAttribL1d _glr->glExt.glVertexAttribL1d
#define glVertexAttribL2d _glr->glExt.glVertexAttribL2d
#define glVertexAttribL3d _glr->glExt.glVertexAttribL3d
#define glVertexAttribL4d _glr->glExt.glVertexAttribL4d
#define glVertexAttribL1dv _glr->glExt.glVertexAttribL1dv
#define glVertexAttribL2dv _glr->glExt.glVertexAttribL2dv
#define glVertexAttribL3dv _glr->glExt.glVertexAttribL3dv
#define glVertexAttribL4dv _glr->glExt.glVertexAttribL4dv
#define glVertexAttribLPointer _glr->glExt.glVertexAttribLPointer
#define glGetVertexAttribLdv _glr->glExt.glGetVertexAttribLdv
#define glViewportArrayv _glr->glExt.glViewportArrayv
#define glViewportIndexedf _glr->glExt.glViewportIndexedf
#define glViewportIndexedfv _glr->glExt.glViewportIndexedfv
#define glScissorArrayv _glr->glExt.glScissorArrayv
#define glScissorIndexed _glr->glExt.glScissorIndexed
#define glScissorIndexedv _glr->glExt.glScissorIndexedv
#define glDepthRangeArrayv _glr->glExt.glDepthRangeArrayv
#define glDepthRangeIndexed _glr->glExt.glDepthRangeIndexed
#define glGetFloati_v _glr->glExt.glGetFloati_v
#define glGetDoublei_v _glr->glExt.glGetDoublei_v

/// OpenGL 4.2 funcs =================------------------------------
#define glDrawArraysInstancedBaseInstance _glr->glExt.glDrawArraysInstancedBaseInstance
#define glDrawElementsInstancedBaseInstance _glr->glExt.glDrawElementsInstancedBaseInstance
#define glDrawElementsInstancedBaseVertexBaseInstance _glr->glExt.glDrawElementsInstancedBaseVertexBaseInstance
#define glGetInternalformativ _glr->glExt.glGetInternalformativ
#define glGetActiveAtomicCounterBufferiv _glr->glExt.glGetActiveAtomicCounterBufferiv
#define glBindImageTexture _glr->glExt.glBindImageTexture
#define glMemoryBarrier _glr->glExt.glMemoryBarrier
#define glTexStorage1D _glr->glExt.glTexStorage1D
#define glTexStorage2D _glr->glExt.glTexStorage2D
#define glTexStorage3D _glr->glExt.glTexStorage3D
#define glDrawTransformFeedbackInstanced _glr->glExt.glDrawTransformFeedbackInstanced
#define glDrawTransformFeedbackStreamInstanced _glr->glExt.glDrawTransformFeedbackStreamInstanced

/// OpenGL 4.3 funcs =================------------------------------
#define glClearBufferData _glr->glExt.glClearBufferData
#define glClearBufferSubData _glr->glExt.glClearBufferSubData
#define glDispatchCompute _glr->glExt.glDispatchCompute
#define glDispatchComputeIndirect _glr->glExt.glDispatchComputeIndirect
#define glCopyImageSubData _glr->glExt.glCopyImageSubData
#define glFramebufferParameteri _glr->glExt.glFramebufferParameteri
#define glGetFramebufferParameteriv _glr->glExt.glGetFramebufferParameteriv
#define glGetInternalformati64v _glr->glExt.glGetInternalformati64v
#define glInvalidateTexSubImage _glr->glExt.glInvalidateTexSubImage
#define glInvalidateTexImage _glr->glExt.glInvalidateTexImage
#define glInvalidateBufferSubData _glr->glExt.glInvalidateBufferSubData
#define glInvalidateBufferData _glr->glExt.glInvalidateBufferData
#define glInvalidateFramebuffer _glr->glExt.glInvalidateFramebuffer
#define glInvalidateSubFramebuffer _glr->glExt.glInvalidateSubFramebuffer
#define glMultiDrawArraysIndirect _glr->glExt.glMultiDrawArraysIndirect
#define glMultiDrawElementsIndirect _glr->glExt.glMultiDrawElementsIndirect
#define glGetProgramInterfaceiv _glr->glExt.glGetProgramInterfaceiv
#define glGetProgramResourceIndex _glr->glExt.glGetProgramResourceIndex
#define glGetProgramResourceName _glr->glExt.glGetProgramResourceName
#define glGetProgramResourceiv _glr->glExt.glGetProgramResourceiv
#define glGetProgramResourceLocation _glr->glExt.glGetProgramResourceLocation
#define glGetProgramResourceLocationIndex _glr->glExt.glGetProgramResourceLocationIndex
#define glShaderStorageBlockBinding _glr->glExt.glShaderStorageBlockBinding
#define glTexBufferRange _glr->glExt.glTexBufferRange
#define glTexStorage2DMultisample _glr->glExt.glTexStorage2DMultisample
#define glTexStorage3DMultisample _glr->glExt.glTexStorage3DMultisample
#define glTextureView _glr->glExt.glTextureView
#define glBindVertexBuffer _glr->glExt.glBindVertexBuffer
#define glVertexAttribFormat _glr->glExt.glVertexAttribFormat
#define glVertexAttribIFormat _glr->glExt.glVertexAttribIFormat
#define glVertexAttribLFormat _glr->glExt.glVertexAttribLFormat
#define glVertexAttribBinding _glr->glExt.glVertexAttribBinding
#define glVertexBindingDivisor _glr->glExt.glVertexBindingDivisor
#define glDebugMessageControl _glr->glExt.glDebugMessageControl
#define glDebugMessageInsert _glr->glExt.glDebugMessageInsert
#define glDebugMessageCallback _glr->glExt.glDebugMessageCallback
#define glGetDebugMessageLog _glr->glExt.glGetDebugMessageLog
#define glPushDebugGroup _glr->glExt.glPushDebugGroup
#define glPopDebugGroup _glr->glExt.glPopDebugGroup
#define glObjectLabel _glr->glExt.glObjectLabel
#define glGetObjectLabel _glr->glExt.glGetObjectLabel
#define glObjectPtrLabel _glr->glExt.glObjectPtrLabel
#define glGetObjectPtrLabel _glr->glExt.glGetObjectPtrLabel

/// OpenGL 4.4 funcs =================------------------------------
#define glBufferStorage _glr->glExt.glBufferStorage
#define glClearTexImage _glr->glExt.glClearTexImage
#define glClearTexSubImage _glr->glExt.glClearTexSubImage
#define glBindBuffersBase _glr->glExt.glBindBuffersBase
#define glBindBuffersRange _glr->glExt.glBindBuffersRange
#define glBindTextures _glr->glExt.glBindTextures
#define glBindSamplers _glr->glExt.glBindSamplers
#define glBindImageTextures _glr->glExt.glBindImageTextures
#define glBindVertexBuffers _glr->glExt.glBindVertexBuffers


#ifdef OS_LINUX
///==================///
// linux GLX versions //
///==================///

/*
/// GLX_VERSION_1_3
#define glXGetFBConfigs _glr->glExt.glXGetFBConfigs
#define glXChooseFBConfig _glr->glExt.glXChooseFBConfig
#define glXGetFBConfigAttrib _glr->glExt.glXGetFBConfigAttrib
#define glXGetVisualFromFBConfig _glr->glExt.glXGetVisualFromFBConfig
#define glXCreateWindow _glr->glExt.glXCreateWindow
#define glXDestroyWindow _glr->glExt.glXDestroyWindow
#define glXCreatePixmap _glr->glExt.glXCreatePixmap
#define glXDestroyPixmap _glr->glExt.glXDestroyPixmap
#define glXCreatePbuffer _glr->glExt.glXCreatePbuffer
#define glXDestroyPbuffer _glr->glExt.glXDestroyPbuffer
#define glXQueryDrawable _glr->glExt.glXQueryDrawable
#define glXCreateNewContext _glr->glExt.glXCreateNewContext
#define glXMakeContextCurrent _glr->glExt.glXMakeContextCurrent
#define glXGetCurrentReadDrawable _glr->glExt.glXGetCurrentReadDrawable
#define glXQueryContext _glr->glExt.glXQueryContext
#define glXSelectEvent _glr->glExt.glXSelectEvent
#define glXGetSelectedEvent _glr->glExt.glXGetSelectedEvent

/// GLX_VERSION_1_4
*/
#endif /// OS_LINUX








///==============///
// ARB extensions //
///==============///

// GL_ARB_bindless_texture core 4.4, it is in glext.h, not in OpenGL registry pages tho...
#define glGetTextureHandleARB _glr->glExt.glGetTextureHandleARB
#define glGetTextureSamplerHandleARB _glr->glExt.glGetTextureSamplerHandleARB
#define glMakeTextureHandleResidentARB _glr->glExt.glMakeTextureHandleResidentARB
#define glMakeTextureHandleNonResidentARB _glr->glExt.glMakeTextureHandleNonResidentARB
#define glGetImageHandleARB _glr->glExt.glGetImageHandleARB
#define glMakeImageHandleResidentARB _glr->glExt.glMakeImageHandleResidentARB
#define glMakeImageHandleNonResidentARB _glr->glExt.glMakeImageHandleNonResidentARB
#define glUniformHandleui64ARB _glr->glExt.glUniformHandleui64ARB
#define glUniformHandleui64vARB _glr->glExt.glUniformHandleui64vARB
#define glProgramUniformHandleui64ARB _glr->glExt.glProgramUniformHandleui64ARB
#define glProgramUniformHandleui64vARB _glr->glExt.glProgramUniformHandleui64vARB
#define glIsTextureHandleResidentARB _glr->glExt.glIsTextureHandleResidentARB
#define glIsImageHandleResidentARB _glr->glExt.glIsImageHandleResidentARB
#define glVertexAttribL1ui64ARB _glr->glExt.glVertexAttribL1ui64ARB
#define glVertexAttribL1ui64vARB _glr->glExt.glVertexAttribL1ui64vARB
#define glGetVertexAttribLui64vARB _glr->glExt.glGetVertexAttribLui64vARB
#ifdef OS_LINUX
// GLX_ARB_get_proc_address
// hope this func just works as it is...
//#define glXGetProcAddressARB _glr->glExt.glXGetProcAddressARB
#endif /// OS_LINUX
// GL_ARB_cl_event
#define glCreateSyncFromCLeventARB _glr->glExt.glCreateSyncFromCLeventARB
// GL_ARB_color_buffer_float
#define glClampColorARB _glr->glExt.glClampColorARB

#ifdef OS_WIN
//inline BOOL WINAPI wglGetPixelFormatAttribivARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues) {
//  return _glr->glExt.wglGetPixelFormatAttribivARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);}
//inline BOOL WINAPI wglGetPixelFormatAttribfvARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues) {
//  return _glr->glExt.wglGetPixelFormatAttribfvARB (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);}
//inline BOOL WINAPI wglChoosePixelFormatARB (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats) {
//  return _glr->glExt.wglChoosePixelFormatARB (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);}
#endif /// OS_WIN











// #153 GL_ARB_compute_variable_group_size
#define glDispatchComputeGroupSizeARB _glr->glExt.glDispatchComputeGroupSizeARB
// #104 GL_ARB_debug_output
#define glDebugMessageControlARB _glr->glExt.glDebugMessageControlARB
#define glDebugMessageInsertARB _glr->glExt.glDebugMessageInsertARB
#define glDebugMessageCallbackARB _glr->glExt.glDebugMessageCallbackARB
#define glGetDebugMessageLogARB _glr->glExt.glGetDebugMessageLogARB
// #037 GL_ARB_draw_buffers
#define glDrawBuffersARB _glr->glExt.glDrawBuffersARB
// #069 GL_ARB_draw_buffers_blend
#define glBlendEquationiARB _glr->glExt.glBlendEquationiARB
#define glBlendEquationSeparateiARB _glr->glExt.glBlendEquationSeparateiARB
#define glBlendFunciARB _glr->glExt.glBlendFunciARB
#define glBlendFuncSeparateiARB _glr->glExt.glBlendFuncSeparateiARB
// #044 GL_ARB_draw_instanced
#define glDrawArraysInstancedARB _glr->glExt.glDrawArraysInstancedARB
#define glDrawElementsInstancedARB _glr->glExt.glDrawElementsInstancedARB
// #027 GL_ARB_fragment_program
#define glProgramStringARB _glr->glExt.glProgramStringARB
#define glBindProgramARB _glr->glExt.glBindProgramARB
#define glDeleteProgramsARB _glr->glExt.glDeleteProgramsARB
#define glGenProgramsARB _glr->glExt.glGenProgramsARB
#define glProgramEnvParameter4dARB _glr->glExt.glProgramEnvParameter4dARB
#define glProgramEnvParameter4dvARB _glr->glExt.glProgramEnvParameter4dvARB
#define glProgramEnvParameter4fARB _glr->glExt.glProgramEnvParameter4fARB
#define glProgramEnvParameter4fvARB _glr->glExt.glProgramEnvParameter4fvARB
#define glProgramLocalParameter4dARB _glr->glExt.glProgramLocalParameter4dARB
#define glProgramLocalParameter4dvARB _glr->glExt.glProgramLocalParameter4dvARB
#define glProgramLocalParameter4fARB _glr->glExt.glProgramLocalParameter4fARB
#define glProgramLocalParameter4fvARB _glr->glExt.glProgramLocalParameter4fvARB
#define glGetProgramEnvParameterdvARB _glr->glExt.glGetProgramEnvParameterdvARB
#define glGetProgramEnvParameterfvARB _glr->glExt.glGetProgramEnvParameterfvARB
#define glGetProgramLocalParameterdvARB _glr->glExt.glGetProgramLocalParameterdvARB
#define glGetProgramLocalParameterfvARB _glr->glExt.glGetProgramLocalParameterfvARB
#define glGetProgramivARB _glr->glExt.glGetProgramivARB
#define glGetProgramStringARB _glr->glExt.glGetProgramStringARB
#define glIsProgramARB _glr->glExt.glIsProgramARB
// #047 GL_ARB_geometry_shader4
#define glProgramParameteriARB _glr->glExt.glProgramParameteriARB
#define glFramebufferTextureARB _glr->glExt.glFramebufferTextureARB
#define glFramebufferTextureLayerARB _glr->glExt.glFramebufferTextureLayerARB
#define glFramebufferTextureFaceARB _glr->glExt.glFramebufferTextureFaceARB
#ifdef OS_WIN
// # GL_ARB_imaging - not in openGL registry list
#define glColorTable _glr->glExt.glColorTable
#define glColorTableParameterfv _glr->glExt.glColorTableParameterfv
#define glColorTableParameteriv _glr->glExt.glColorTableParameteriv
#define glCopyColorTable _glr->glExt.glCopyColorTable
#define glGetColorTable _glr->glExt.glGetColorTable
#define glGetColorTableParameterfv _glr->glExt.glGetColorTableParameterfv
#define glGetColorTableParameteriv _glr->glExt.glGetColorTableParameteriv
#define glColorSubTable _glr->glExt.glColorSubTable
#define glCopyColorSubTable _glr->glExt.glCopyColorSubTable
#define glConvolutionFilter1D _glr->glExt.glConvolutionFilter1D
#define glConvolutionFilter2D _glr->glExt.glConvolutionFilter2D
#define glConvolutionParameterf _glr->glExt.glConvolutionParameterf
#define glConvolutionParameterfv _glr->glExt.glConvolutionParameterfv
#define glConvolutionParameteri _glr->glExt.glConvolutionParameteri
#define glConvolutionParameteriv _glr->glExt.glConvolutionParameteriv
#define glCopyConvolutionFilter1D _glr->glExt.glCopyConvolutionFilter1D
#define glCopyConvolutionFilter2D _glr->glExt.glCopyConvolutionFilter2D
#define glGetConvolutionFilter _glr->glExt.glGetConvolutionFilter
#define glGetConvolutionParameterfv _glr->glExt.glGetConvolutionParameterfv
#define glGetConvolutionParameteriv _glr->glExt.glGetConvolutionParameteriv
#define glGetSeparableFilter _glr->glExt.glGetSeparableFilter
#define glSeparableFilter2D _glr->glExt.glSeparableFilter2D
#define glGetHistogram _glr->glExt.glGetHistogram
#define glGetHistogramParameterfv _glr->glExt.glGetHistogramParameterfv
#define glGetHistogramParameteriv _glr->glExt.glGetHistogramParameteriv
#define glGetMinmax _glr->glExt.glGetMinmax
#define glGetMinmaxParameterfv _glr->glExt.glGetMinmaxParameterfv
#define glGetMinmaxParameteriv _glr->glExt.glGetMinmaxParameteriv
#define glHistogram _glr->glExt.glHistogram
#define glMinmax _glr->glExt.glMinmax
#define glResetHistogram _glr->glExt.glResetHistogram
#define glResetMinmax _glr->glExt.glResetMinmax
#endif /// OS_WIN
// #154 GL_ARB_indirect_parameters
#define glMultiDrawArraysIndirectCountARB _glr->glExt.glMultiDrawArraysIndirectCountARB
#define glMultiDrawElementsIndirectCountARB _glr->glExt.glMultiDrawElementsIndirectCountARB
// #049 GL_ARB_instanced_arrays
#define glVertexAttribDivisorARB _glr->glExt.glVertexAttribDivisorARB
// #016 GL_ARB_matrix_palette
#define glCurrentPaletteMatrixARB _glr->glExt.glCurrentPaletteMatrixARB
#define glMatrixIndexubvARB _glr->glExt.glMatrixIndexubvARB
#define glMatrixIndexusvARB _glr->glExt.glMatrixIndexusvARB
#define glMatrixIndexuivARB _glr->glExt.glMatrixIndexuivARB
#define glMatrixIndexPointerARB _glr->glExt.glMatrixIndexPointerARB
// #005 GL_ARB_multisample
#define glSampleCoverageARB _glr->glExt.glSampleCoverageARB
// #001 GL_ARB_multitexture
#define glActiveTextureARB _glr->glExt.glActiveTextureARB
#define glClientActiveTextureARB _glr->glExt.glClientActiveTextureARB
#define glMultiTexCoord1dARB _glr->glExt.glMultiTexCoord1dARB
#define glMultiTexCoord1dvARB _glr->glExt.glMultiTexCoord1dvARB
#define glMultiTexCoord1fARB _glr->glExt.glMultiTexCoord1fARB
#define glMultiTexCoord1fvARB _glr->glExt.glMultiTexCoord1fvARB
#define glMultiTexCoord1iARB _glr->glExt.glMultiTexCoord1iARB
#define glMultiTexCoord1ivARB _glr->glExt.glMultiTexCoord1ivARB
#define glMultiTexCoord1sARB _glr->glExt.glMultiTexCoord1sARB
#define glMultiTexCoord1svARB _glr->glExt.glMultiTexCoord1svARB
#define glMultiTexCoord2dARB _glr->glExt.glMultiTexCoord2dARB
#define glMultiTexCoord2dvARB _glr->glExt.glMultiTexCoord2dvARB
#define glMultiTexCoord2fARB _glr->glExt.glMultiTexCoord2fARB
#define glMultiTexCoord2fvARB _glr->glExt.glMultiTexCoord2fvARB
#define glMultiTexCoord2iARB _glr->glExt.glMultiTexCoord2iARB
#define glMultiTexCoord2ivARB _glr->glExt.glMultiTexCoord2ivARB
#define glMultiTexCoord2sARB _glr->glExt.glMultiTexCoord2sARB
#define glMultiTexCoord2svARB _glr->glExt.glMultiTexCoord2svARB
#define glMultiTexCoord3dARB _glr->glExt.glMultiTexCoord3dARB
#define glMultiTexCoord3dvARB _glr->glExt.glMultiTexCoord3dvARB
#define glMultiTexCoord3fARB _glr->glExt.glMultiTexCoord3fARB
#define glMultiTexCoord3fvARB _glr->glExt.glMultiTexCoord3fvARB
#define glMultiTexCoord3iARB _glr->glExt.glMultiTexCoord3iARB
#define glMultiTexCoord3ivARB _glr->glExt.glMultiTexCoord3ivARB
#define glMultiTexCoord3sARB _glr->glExt.glMultiTexCoord3sARB
#define glMultiTexCoord3svARB _glr->glExt.glMultiTexCoord3svARB
#define glMultiTexCoord4dARB _glr->glExt.glMultiTexCoord4dARB
#define glMultiTexCoord4dvARB _glr->glExt.glMultiTexCoord4dvARB
#define glMultiTexCoord4fARB _glr->glExt.glMultiTexCoord4fARB
#define glMultiTexCoord4fvARB _glr->glExt.glMultiTexCoord4fvARB
#define glMultiTexCoord4iARB _glr->glExt.glMultiTexCoord4iARB
#define glMultiTexCoord4ivARB _glr->glExt.glMultiTexCoord4ivARB
#define glMultiTexCoord4sARB _glr->glExt.glMultiTexCoord4sARB
#define glMultiTexCoord4svARB _glr->glExt.glMultiTexCoord4svARB
// #029 GL_ARB_occlusion_query
#define glGenQueriesARB _glr->glExt.glGenQueriesARB
#define glDeleteQueriesARB _glr->glExt.glDeleteQueriesARB
#define glIsQueryARB _glr->glExt.glIsQueryARB
#define glBeginQueryARB _glr->glExt.glBeginQueryARB
#define glEndQueryARB _glr->glExt.glEndQueryARB
#define glGetQueryivARB _glr->glExt.glGetQueryivARB
#define glGetQueryObjectivARB _glr->glExt.glGetQueryObjectivARB
#define glGetQueryObjectuivARB _glr->glExt.glGetQueryObjectuivARB
// #014 GL_ARB_point_parameters
#define glPointParameterfARB _glr->glExt.glPointParameterfARB
#define glPointParameterfvARB _glr->glExt.glPointParameterfvARB
// #105 GL_ARB_robustness
#define glGetGraphicsResetStatusARB _glr->glExt.glGetGraphicsResetStatusARB
#define glGetnTexImageARB _glr->glExt.glGetnTexImageARB
#define glReadnPixelsARB _glr->glExt.glReadnPixelsARB
#define glGetnCompressedTexImageARB _glr->glExt.glGetnCompressedTexImageARB
#define glGetnUniformfvARB _glr->glExt.glGetnUniformfvARB
#define glGetnUniformivARB _glr->glExt.glGetnUniformivARB
#define glGetnUniformuivARB _glr->glExt.glGetnUniformuivARB
#define glGetnUniformdvARB _glr->glExt.glGetnUniformdvARB
#define glGetnMapdvARB _glr->glExt.glGetnMapdvARB
#define glGetnMapfvARB _glr->glExt.glGetnMapfvARB
#define glGetnMapivARB _glr->glExt.glGetnMapivARB
#define glGetnPixelMapfvARB _glr->glExt.glGetnPixelMapfvARB
#define glGetnPixelMapuivARB _glr->glExt.glGetnPixelMapuivARB
#define glGetnPixelMapusvARB _glr->glExt.glGetnPixelMapusvARB
#define glGetnPolygonStippleARB _glr->glExt.glGetnPolygonStippleARB
#define glGetnColorTableARB _glr->glExt.glGetnColorTableARB
#define glGetnConvolutionFilterARB _glr->glExt.glGetnConvolutionFilterARB
#define glGetnSeparableFilterARB _glr->glExt.glGetnSeparableFilterARB
#define glGetnHistogramARB _glr->glExt.glGetnHistogramARB
#define glGetnMinmaxARB _glr->glExt.glGetnMinmaxARB
// #070 GL_ARB_sample_shading
#define glMinSampleShadingARB _glr->glExt.glMinSampleShadingARB
// #030 GL_ARB_shader_objects
#define glDeleteObjectARB _glr->glExt.glDeleteObjectARB
#define glGetHandleARB _glr->glExt.glGetHandleARB
#define glDetachObjectARB _glr->glExt.glDetachObjectARB
#define glCreateShaderObjectARB _glr->glExt.glCreateShaderObjectARB
#define glShaderSourceARB _glr->glExt.glShaderSourceARB
#define glCompileShaderARB _glr->glExt.glCompileShaderARB
#define glCreateProgramObjectARB _glr->glExt.glCreateProgramObjectARB
#define glAttachObjectARB _glr->glExt.glAttachObjectARB
#define glLinkProgramARB _glr->glExt.glLinkProgramARB
#define glUseProgramObjectARB _glr->glExt.glUseProgramObjectARB
#define glValidateProgramARB _glr->glExt.glValidateProgramARB
#define glUniform1fARB _glr->glExt.glUniform1fARB
#define glUniform2fARB _glr->glExt.glUniform2fARB
#define glUniform3fARB _glr->glExt.glUniform3fARB
#define glUniform4fARB _glr->glExt.glUniform4fARB
#define glUniform1iARB _glr->glExt.glUniform1iARB
#define glUniform2iARB _glr->glExt.glUniform2iARB
#define glUniform3iARB _glr->glExt.glUniform3iARB
#define glUniform4iARB _glr->glExt.glUniform4iARB
#define glUniform1fvARB _glr->glExt.glUniform1fvARB
#define glUniform2fvARB _glr->glExt.glUniform2fvARB
#define glUniform3fvARB _glr->glExt.glUniform3fvARB
#define glUniform4fvARB _glr->glExt.glUniform4fvARB
#define glUniform1ivARB _glr->glExt.glUniform1ivARB
#define glUniform2ivARB _glr->glExt.glUniform2ivARB
#define glUniform3ivARB _glr->glExt.glUniform3ivARB
#define glUniform4ivARB _glr->glExt.glUniform4ivARB
#define glUniformMatrix2fvARB _glr->glExt.glUniformMatrix2fvARB
#define glUniformMatrix3fvARB _glr->glExt.glUniformMatrix3fvARB
#define glUniformMatrix4fvARB _glr->glExt.glUniformMatrix4fvARB
#define glGetObjectParameterfvARB _glr->glExt.glGetObjectParameterfvARB
#define glGetObjectParameterivARB _glr->glExt.glGetObjectParameterivARB
#define glGetInfoLogARB _glr->glExt.glGetInfoLogARB
#define glGetAttachedObjectsARB _glr->glExt.glGetAttachedObjectsARB (containerObj, maxCount, count, obj);}
#define glGetUniformLocationARB _glr->glExt.glGetUniformLocationARB
#define glGetActiveUniformARB _glr->glExt.glGetActiveUniformARB
#define glGetUniformfvARB _glr->glExt.glGetUniformfvARB
#define glGetUniformivARB _glr->glExt.glGetUniformivARB
#define glGetShaderSourceARB _glr->glExt.glGetShaderSourceARB
// #076 GL_ARB_shading_language_include
#define glNamedStringARB _glr->glExt.glNamedStringARB
#define glDeleteNamedStringARB _glr->glExt.glDeleteNamedStringARB
#define glCompileShaderIncludeARB _glr->glExt.glCompileShaderIncludeARB
#define glIsNamedStringARB _glr->glExt.glIsNamedStringARB
#define glGetNamedStringARB _glr->glExt.glGetNamedStringARB
#define glGetNamedStringivARB _glr->glExt.glGetNamedStringivARB
// #158 GL_ARB_sparse_texture
#define glTexPageCommitmentARB _glr->glExt.glTexPageCommitmentARB
// #092 GL_ARB_texture_buffer_object
#define glTexBufferARB _glr->glExt.glTexBufferARB
// #012 GL_ARB_texture_compression
#define glCompressedTexImage3DARB _glr->glExt.glCompressedTexImage3DARB
#define glCompressedTexImage2DARB _glr->glExt.glCompressedTexImage2DARB
#define glCompressedTexImage1DARB _glr->glExt.glCompressedTexImage1DARB
#define glCompressedTexSubImage3DARB _glr->glExt.glCompressedTexSubImage3DARB
#define glCompressedTexSubImage2DARB _glr->glExt.glCompressedTexSubImage2DARB
#define glCompressedTexSubImage1DARB _glr->glExt.glCompressedTexSubImage1DARB
#define glGetCompressedTexImageARB _glr->glExt.glGetCompressedTexImageARB
// #003 GL_ARB_transpose_matrix
#define glLoadTransposeMatrixfARB _glr->glExt.glLoadTransposeMatrixfARB
#define glLoadTransposeMatrixdARB _glr->glExt.glLoadTransposeMatrixdARB
#define glMultTransposeMatrixfARB _glr->glExt.glMultTransposeMatrixfARB
#define glMultTransposeMatrixdARB _glr->glExt.glMultTransposeMatrixdARB
// #015 GL_ARB_vertex_blend
#define glWeightbvARB _glr->glExt.glWeightbvARB
#define glWeightsvARB _glr->glExt.glWeightsvARB
#define glWeightivARB _glr->glExt.glWeightivARB
#define glWeightfvARB _glr->glExt.glWeightfvARB
#define glWeightdvARB _glr->glExt.glWeightdvARB
#define glWeightubvARB _glr->glExt.glWeightubvARB
#define glWeightusvARB _glr->glExt.glWeightusvARB
#define glWeightuivARB _glr->glExt.glWeightuivARB
#define glWeightPointerARB _glr->glExt.glWeightPointerARB
#define glVertexBlendARB _glr->glExt.glVertexBlendARB
// #028 GL_ARB_vertex_buffer_object
#define glBindBufferARB _glr->glExt.glBindBufferARB
#define glDeleteBuffersARB _glr->glExt.glDeleteBuffersARB
#define glGenBuffersARB _glr->glExt.glGenBuffersARB
#define glIsBufferARB _glr->glExt.glIsBufferARB
#define glBufferDataARB _glr->glExt.glBufferDataARB
#define glBufferSubDataARB _glr->glExt.glBufferSubDataARB
#define glGetBufferSubDataARB _glr->glExt.glGetBufferSubDataARB
#define glMapBufferARB _glr->glExt.glMapBufferARB
#define glUnmapBufferARB _glr->glExt.glUnmapBufferARB
#define glGetBufferParameterivARB _glr->glExt.glGetBufferParameterivARB
#define glGetBufferPointervARB _glr->glExt.glGetBufferPointervARB
// #026 GL_ARB_vertex_program
#define glVertexAttrib1dARB _glr->glExt.glVertexAttrib1dARB
#define glVertexAttrib1dvARB _glr->glExt.glVertexAttrib1dvARB
#define glVertexAttrib1fARB _glr->glExt.glVertexAttrib1fARB
#define glVertexAttrib1fvARB _glr->glExt.glVertexAttrib1fvARB
#define glVertexAttrib1sARB _glr->glExt.glVertexAttrib1sARB
#define glVertexAttrib1svARB _glr->glExt.glVertexAttrib1svARB
#define glVertexAttrib2dARB _glr->glExt.glVertexAttrib2dARB
#define glVertexAttrib2dvARB _glr->glExt.glVertexAttrib2dvARB
#define glVertexAttrib2fARB _glr->glExt.glVertexAttrib2fARB
#define glVertexAttrib2fvARB _glr->glExt.glVertexAttrib2fvARB
#define glVertexAttrib2sARB _glr->glExt.glVertexAttrib2sARB
#define glVertexAttrib2svARB _glr->glExt.glVertexAttrib2svARB
#define glVertexAttrib3dARB _glr->glExt.glVertexAttrib3dARB
#define glVertexAttrib3dvARB _glr->glExt.glVertexAttrib3dvARB
#define glVertexAttrib3fARB _glr->glExt.glVertexAttrib3fARB
#define glVertexAttrib3fvARB _glr->glExt.glVertexAttrib3fvARB
#define glVertexAttrib3sARB _glr->glExt.glVertexAttrib3sARB
#define glVertexAttrib3svARB _glr->glExt.glVertexAttrib3svARB
#define glVertexAttrib4NbvARB _glr->glExt.glVertexAttrib4NbvARB
#define glVertexAttrib4NivARB _glr->glExt.glVertexAttrib4NivARB
#define glVertexAttrib4NsvARB _glr->glExt.glVertexAttrib4NsvARB
#define glVertexAttrib4NubARB _glr->glExt.glVertexAttrib4NubARB
#define glVertexAttrib4NubvARB _glr->glExt.glVertexAttrib4NubvARB
#define glVertexAttrib4NuivARB _glr->glExt.glVertexAttrib4NuivARB
#define glVertexAttrib4NusvARB _glr->glExt.glVertexAttrib4NusvARB
#define glVertexAttrib4bvARB _glr->glExt.glVertexAttrib4bvARB
#define glVertexAttrib4dARB _glr->glExt.glVertexAttrib4dARB
#define glVertexAttrib4dvARB _glr->glExt.glVertexAttrib4dvARB
#define glVertexAttrib4fARB _glr->glExt.glVertexAttrib4fARB
#define glVertexAttrib4fvARB _glr->glExt.glVertexAttrib4fvARB
#define glVertexAttrib4ivARB _glr->glExt.glVertexAttrib4ivARB
#define glVertexAttrib4sARB _glr->glExt.glVertexAttrib4sARB
#define glVertexAttrib4svARB _glr->glExt.glVertexAttrib4svARB
#define glVertexAttrib4ubvARB _glr->glExt.glVertexAttrib4ubvARB
#define glVertexAttrib4uivARB _glr->glExt.glVertexAttrib4uivARB
#define glVertexAttrib4usvARB _glr->glExt.glVertexAttrib4usvARB
#define glVertexAttribPointerARB _glr->glExt.glVertexAttribPointerARB
#define glEnableVertexAttribArrayARB _glr->glExt.glEnableVertexAttribArrayARB
#define glDisableVertexAttribArrayARB _glr->glExt.glDisableVertexAttribArrayARB
#define glGetVertexAttribdvARB _glr->glExt.glGetVertexAttribdvARB
#define glGetVertexAttribfvARB _glr->glExt.glGetVertexAttribfvARB
#define glGetVertexAttribivARB _glr->glExt.glGetVertexAttribivARB
#define glGetVertexAttribPointervARB _glr->glExt.glGetVertexAttribPointervARB
// #031 GL_ARB_vertex_shader
#define glBindAttribLocationARB _glr->glExt.glBindAttribLocationARB
#define glGetActiveAttribARB _glr->glExt.glGetActiveAttribARB
#define glGetAttribLocationARB _glr->glExt.glGetAttribLocationARB
// #025 GL_ARB_window_pos
#define glWindowPos2dARB _glr->glExt.glWindowPos2dARB
#define glWindowPos2dvARB _glr->glExt.glWindowPos2dvARB
#define glWindowPos2fARB _glr->glExt.glWindowPos2fARB
#define glWindowPos2fvARB _glr->glExt.glWindowPos2fvARB
#define glWindowPos2iARB _glr->glExt.glWindowPos2iARB
#define glWindowPos2ivARB _glr->glExt.glWindowPos2ivARB
#define glWindowPos2sARB _glr->glExt.glWindowPos2sARB
#define glWindowPos2svARB _glr->glExt.glWindowPos2svARB
#define glWindowPos3dARB _glr->glExt.glWindowPos3dARB
#define glWindowPos3dvARB _glr->glExt.glWindowPos3dvARB
#define glWindowPos3fARB _glr->glExt.glWindowPos3fARB
#define glWindowPos3fvARB _glr->glExt.glWindowPos3fvARB
#define glWindowPos3iARB _glr->glExt.glWindowPos3iARB
#define glWindowPos3ivARB _glr->glExt.glWindowPos3ivARB
#define glWindowPos3sARB _glr->glExt.glWindowPos3sARB
#define glWindowPos3svARB _glr->glExt.glWindowPos3svARB






///==============///
// WGL extensions //
///==============///


#ifdef OS_WIN
// WGL_ARB_buffer_region
#define wglCreateBufferRegionARB _glr->glExt.wglCreateBufferRegionARB
#define wglDeleteBufferRegionARB _glr->glExt.wglDeleteBufferRegionARB
#define wglSaveBufferRegionARB _glr->glExt.wglSaveBufferRegionARB
#define wglRestoreBufferRegionARB _glr->glExt.wglRestoreBufferRegionARB
// WGL_ARB_create_context
#define wglCreateContextAttribsARB _glr->glExt.wglCreateContextAttribsARB
// WGL_ARB_extensions_string
#define wglGetExtensionsStringARB _glr->glExt.wglGetExtensionsStringARB
// WGL_ARB_make_current_read
#define wglMakeContextCurrentARB _glr->glExt.wglMakeContextCurrentARB
#define wglGetCurrentReadDCARB _glr->glExt.wglGetCurrentReadDCARB
// WGL_ARB_pbuffer
#define wglCreatePbufferARB _glr->glExt.wglCreatePbufferARB
#define wglGetPbufferDCARB _glr->glExt.wglGetPbufferDCARB
#define wglReleasePbufferDCARB _glr->glExt.wglReleasePbufferDCARB
#define wglDestroyPbufferARB _glr->glExt.wglDestroyPbufferARB
#define wglQueryPbufferARB _glr->glExt.wglQueryPbufferARB
// WGL_ARB_pixel_format
#define wglGetPixelFormatAttribivARB _glr->glExt.wglGetPixelFormatAttribivARB
#define wglGetPixelFormatAttribfvARB _glr->glExt.wglGetPixelFormatAttribfvARB
#define wglChoosePixelFormatARB _glr->glExt.wglChoosePixelFormatARB
// WGL_ARB_render_texture
#define wglBindTexImageARB _glr->glExt.wglBindTexImageARB
#define wglReleaseTexImageARB _glr->glExt.wglReleaseTexImageARB
#define wglSetPbufferAttribARB _glr->glExt.wglSetPbufferAttribARB
// WGL_3DL_stereo_control
#define wglSetStereoEmitterState3DL _glr->glExt.wglSetStereoEmitterState3DL
// WGL_AMD_gpu_association
#define wglGetGPUIDsAMD _glr->glExt.wglGetGPUIDsAMD
#define wglGetGPUInfoAMD _glr->glExt.wglGetGPUInfoAMD
#define wglGetContextGPUIDAMD _glr->glExt.wglGetContextGPUIDAMD
#define wglCreateAssociatedContextAMD _glr->glExt.wglCreateAssociatedContextAMD
#define wglCreateAssociatedContextAttribsAMD _glr->glExt.wglCreateAssociatedContextAttribsAMD
#define wglDeleteAssociatedContextAMD _glr->glExt.wglDeleteAssociatedContextAMD
#define wglMakeAssociatedContextCurrentAMD _glr->glExt.wglMakeAssociatedContextCurrentAMD
#define wglGetCurrentAssociatedContextAMD _glr->glExt.wglGetCurrentAssociatedContextAMD
#define wglBlitContextFramebufferAMD _glr->glExt.wglBlitContextFramebufferAMD
// WGL_EXT_display_color_table
#define wglCreateDisplayColorTableEXT _glr->glExt.wglCreateDisplayColorTableEXT
#define wglLoadDisplayColorTableEXT _glr->glExt.wglLoadDisplayColorTableEXT
#define wglBindDisplayColorTableEXT _glr->glExt.wglBindDisplayColorTableEXT
#define wglDestroyDisplayColorTableEXT _glr->glExt.wglDestroyDisplayColorTableEXT
// WGL_EXT_extensions_string
#define wglGetExtensionsStringEXT _glr->glExt.wglGetExtensionsStringEXT
  // WGL_EXT_make_current_read
#define wglMakeContextCurrentEXT _glr->glExt.wglMakeContextCurrentEXT
#define wglGetCurrentReadDCEXT _glr->glExt.wglGetCurrentReadDCEXT
  // WGL_EXT_pbuffer
#define wglCreatePbufferEXT _glr->glExt.wglCreatePbufferEXT
#define wglGetPbufferDCEXT _glr->glExt.wglGetPbufferDCEXT
#define wglReleasePbufferDCEXT _glr->glExt.wglReleasePbufferDCEXT
#define wglDestroyPbufferEXT _glr->glExt.wglDestroyPbufferEXT
#define wglQueryPbufferEXT _glr->glExt.wglQueryPbufferEXT
  // WGL_EXT_pixel_format
#define wglGetPixelFormatAttribivEXT _glr->glExt.wglGetPixelFormatAttribivEXT
#define wglGetPixelFormatAttribfvEXT _glr->glExt.wglGetPixelFormatAttribfvEXT
#define wglChoosePixelFormatEXT _glr->glExt.wglChoosePixelFormatEXT
  // WGL_EXT_swap_control
#define wglSwapIntervalEXT _glr->glExt.wglSwapIntervalEXT
#define wglGetSwapIntervalEXT _glr->glExt.wglGetSwapIntervalEXT
  // WGL_I3D_digital_video_control
#define wglGetDigitalVideoParametersI3D _glr->glExt.wglGetDigitalVideoParametersI3D
#define wglSetDigitalVideoParametersI3D _glr->glExt.wglSetDigitalVideoParametersI3D
  // WGL_I3D_gamma
#define wglGetGammaTableParametersI3D _glr->glExt.wglGetGammaTableParametersI3D
#define wglSetGammaTableParametersI3D _glr->glExt.wglSetGammaTableParametersI3D
#define wglGetGammaTableI3D _glr->glExt.wglGetGammaTableI3D
#define wglSetGammaTableI3D _glr->glExt.wglSetGammaTableI3D
  // WGL_I3D_genlock
#define wglEnableGenlockI3D _glr->glExt.wglEnableGenlockI3D
#define wglDisableGenlockI3D _glr->glExt.wglDisableGenlockI3D
#define wglIsEnabledGenlockI3D _glr->glExt.wglIsEnabledGenlockI3D
#define wglGenlockSourceI3D _glr->glExt.wglGenlockSourceI3D
#define wglGetGenlockSourceI3D _glr->glExt.wglGetGenlockSourceI3D
#define wglGenlockSourceEdgeI3D _glr->glExt.wglGenlockSourceEdgeI3D
#define wglGetGenlockSourceEdgeI3D _glr->glExt.wglGetGenlockSourceEdgeI3D
#define wglGenlockSampleRateI3D _glr->glExt.wglGenlockSampleRateI3D
#define wglGetGenlockSampleRateI3D _glr->glExt.wglGetGenlockSampleRateI3D
#define wglGenlockSourceDelayI3D _glr->glExt.wglGenlockSourceDelayI3D
#define wglGetGenlockSourceDelayI3D _glr->glExt.wglGetGenlockSourceDelayI3D
#define wglQueryGenlockMaxSourceDelayI3D _glr->glExt.wglQueryGenlockMaxSourceDelayI3D
  // WGL_I3D_image_buffer
#define wglCreateImageBufferI3D _glr->glExt.wglCreateImageBufferI3D
#define wglDestroyImageBufferI3D _glr->glExt.wglDestroyImageBufferI3D
#define wglAssociateImageBufferEventsI3D _glr->glExt.wglAssociateImageBufferEventsI3D
#define wglReleaseImageBufferEventsI3D _glr->glExt.wglReleaseImageBufferEventsI3D
  // WGL_I3D_swap_frame_lock
#define wglEnableFrameLockI3D _glr->glExt.wglEnableFrameLockI3D
#define wglDisableFrameLockI3D _glr->glExt.wglDisableFrameLockI3D
#define wglIsEnabledFrameLockI3D _glr->glExt.wglIsEnabledFrameLockI3D
#define wglQueryFrameLockMasterI3D _glr->glExt.wglQueryFrameLockMasterI3D
  // WGL_I3D_swap_frame_usage
#define wglGetFrameUsageI3D _glr->glExt.wglGetFrameUsageI3D
#define wglBeginFrameTrackingI3D _glr->glExt.wglBeginFrameTrackingI3D
#define wglEndFrameTrackingI3D _glr->glExt.wglEndFrameTrackingI3D
#define wglQueryFrameTrackingI3D _glr->glExt.wglQueryFrameTrackingI3D
  // WGL_NV_DX_interop
#define wglDXSetResourceShareHandleNV _glr->glExt.wglDXSetResourceShareHandleNV
#define wglDXOpenDeviceNV _glr->glExt.wglDXOpenDeviceNV
#define wglDXCloseDeviceNV _glr->glExt.wglDXCloseDeviceNV
#define wglDXRegisterObjectNV _glr->glExt.wglDXRegisterObjectNV
#define wglDXUnregisterObjectNV _glr->glExt.wglDXUnregisterObjectNV
#define wglDXObjectAccessNV _glr->glExt.wglDXObjectAccessNV
#define wglDXLockObjectsNV _glr->glExt.wglDXLockObjectsNV
#define wglDXUnlockObjectsNV _glr->glExt.wglDXUnlockObjectsNV
  // WGL_NV_copy_image
#define wglCopyImageSubDataNV _glr->glExt.wglCopyImageSubDataNV
  // WGL_NV_delay_before_swap
#define wglDelayBeforeSwapNV _glr->glExt.wglDelayBeforeSwapNV
  // WGL_NV_gpu_affinity
#define wglEnumGpusNV _glr->glExt.wglEnumGpusNV
#define wglEnumGpuDevicesNV _glr->glExt.wglEnumGpuDevicesNV
#define wglCreateAffinityDCNV _glr->glExt.wglCreateAffinityDCNV
#define wglEnumGpusFromAffinityDCNV _glr->glExt.wglEnumGpusFromAffinityDCNV
#define wglDeleteDCNV _glr->glExt.wglDeleteDCNV
  // WGL_NV_present_video
#define wglEnumerateVideoDevicesNV _glr->glExt.wglEnumerateVideoDevicesNV
#define wglBindVideoDeviceNV _glr->glExt.wglBindVideoDeviceNV
#define wglQueryCurrentContextNV _glr->glExt.wglQueryCurrentContextNV
  // WGL_NV_swap_group
#define wglJoinSwapGroupNV _glr->glExt.wglJoinSwapGroupNV
#define wglBindSwapBarrierNV _glr->glExt.wglBindSwapBarrierNV
#define wglQuerySwapGroupNV _glr->glExt.wglQuerySwapGroupNV
#define wglQueryMaxSwapGroupsNV _glr->glExt.wglQueryMaxSwapGroupsNV
#define wglQueryFrameCountNV _glr->glExt.wglQueryFrameCountNV
#define wglResetFrameCountNV _glr->glExt.wglResetFrameCountNV
  // WGL_NV_vertex_array_range
#define wglAllocateMemoryNV _glr->glExt.wglAllocateMemoryNV
#define wglFreeMemoryNV _glr->glExt.wglFreeMemoryNV
  // WGL_NV_video_capture
#define wglBindVideoCaptureDeviceNV _glr->glExt.wglBindVideoCaptureDeviceNV
#define wglEnumerateVideoCaptureDevicesNV _glr->glExt.wglEnumerateVideoCaptureDevicesNV
#define wglLockVideoCaptureDeviceNV _glr->glExt.wglLockVideoCaptureDeviceNV
#define wglQueryVideoCaptureDeviceNV _glr->glExt.wglQueryVideoCaptureDeviceNV
#define wglReleaseVideoCaptureDeviceNV _glr->glExt.wglReleaseVideoCaptureDeviceNV
  // WGL_NV_video_output
#define wglGetVideoDeviceNV _glr->glExt.wglGetVideoDeviceNV
#define wglReleaseVideoDeviceNV _glr->glExt.wglReleaseVideoDeviceNV
#define wglBindVideoImageNV _glr->glExt.wglBindVideoImageNV
#define wglReleaseVideoImageNV _glr->glExt.wglReleaseVideoImageNV
#define wglSendPbufferToVideoNV _glr->glExt.wglSendPbufferToVideoNV
#define wglGetVideoInfoNV _glr->glExt.wglGetVideoInfoNV
  // WGL_OML_sync_control
#define wglGetSyncValuesOML _glr->glExt.wglGetSyncValuesOML
#define wglGetMscRateOML _glr->glExt.wglGetMscRateOML
#define wglSwapBuffersMscOML _glr->glExt.wglSwapBuffersMscOML
#define wglSwapLayerBuffersMscOML _glr->glExt.wglSwapLayerBuffersMscOML
#define wglWaitForMscOML _glr->glExt.wglWaitForMscOML
#define wglWaitForSbcOML _glr->glExt.wglWaitForSbcOML
#endif /// OS_WIN







///==============///
// GLX extensions //
///==============///

#ifdef OS_LINUX
/*
// GLX_VERSION_1_3
#define glXGetFBConfigs _glr->glExt.glXGetFBConfigs
#define glXChooseFBConfig _glr->glExt.glXChooseFBConfig
#define glXGetFBConfigAttrib _glr->glExt.glXGetFBConfigAttrib
#define glXGetVisualFromFBConfig _glr->glExt.glXGetVisualFromFBConfig
#define glXCreateWindow _glr->glExt.glXCreateWindow
#define glXDestroyWindow _glr->glExt.glXDestroyWindow
#define glXCreatePixmap _glr->glExt.glXCreatePixmap
#define glXDestroyPixmap _glr->glExt.glXDestroyPixmap
#define glXCreatePbuffer _glr->glExt.glXCreatePbuffer
#define glXDestroyPbuffer _glr->glExt.glXDestroyPbuffer
#define glXQueryDrawable _glr->glExt.glXQueryDrawable
#define glXCreateNewContext _glr->glExt.glXCreateNewContext
#define glXMakeContextCurrent _glr->glExt.glXMakeContextCurrent
#define glXGetCurrentReadDrawable _glr->glExt.glXGetCurrentReadDrawable
#define glXQueryContext _glr->glExt.glXQueryContext
#define glXSelectEvent _glr->glExt.glXSelectEvent
#define glXGetSelectedEvent _glr->glExt.glXGetSelectedEvent

// GLX_VERSION_1_4
//#define glXGetProcAddress _glr->glExt.glXGetProcAddress
*/

// GLX_ARB_create_context
#define glXCreateContextAttribsARB _glr->glExt.glXCreateContextAttribsARB
// GLX_ARB_get_proc_address
#define glXGetProcAddressARB _glr->glExt.glXGetProcAddressARB
// GLX_EXT_import_context
#define glXGetCurrentDisplayEXT _glr->glExt.glXGetCurrentDisplayEXT
#define glXQueryContextInfoEXT _glr->glExt.glXQueryContextInfoEXT
#define glXGetContextIDEXT _glr->glExt.glXGetContextIDEXT
#define glXImportContextEXT _glr->glExt.glXImportContextEXT
#define glXFreeContextEXT _glr->glExt.glXFreeContextEXT
// GLX_EXT_swap_control
#define glXSwapIntervalEXT _glr->glExt.glXSwapIntervalEXT
// GLX_EXT_texture_from_pixmap
#define glXBindTexImageEXT _glr->glExt.glXBindTexImageEXT
#define glXReleaseTexImageEXT _glr->glExt.glXReleaseTexImageEXT
// GLX_MESA_agp_offset
#define glXGetAGPOffsetMESA _glr->glExt.glXGetAGPOffsetMESA
// GLX_MESA_copy_sub_buffer
#define glXCopySubBufferMESA _glr->glExt.glXCopySubBufferMESA
// GLX_MESA_pixmap_colormap
#define glXCreateGLXPixmapMESA _glr->glExt.glXCreateGLXPixmapMESA
// GLX_MESA_release_buffers
#define glXReleaseBuffersMESA _glr->glExt.glXReleaseBuffersMESA
// GLX_MESA_set_3dfx_mode
#define glXSet3DfxModeMESA _glr->glExt.glXSet3DfxModeMESA
// GLX_NV_copy_image
#define glXCopyImageSubDataNV _glr->glExt.glXCopyImageSubDataNV
// GLX_NV_present_video
#define glXEnumerateVideoDevicesNV _glr->glExt.glXEnumerateVideoDevicesNV
#define glXBindVideoDeviceNV _glr->glExt.glXBindVideoDeviceNV
// GLX_NV_swap_group
#define glXJoinSwapGroupNV _glr->glExt.glXJoinSwapGroupNV
#define glXBindSwapBarrierNV _glr->glExt.glXBindSwapBarrierNV
#define glXQuerySwapGroupNV _glr->glExt.glXQuerySwapGroupNV
#define glXQueryMaxSwapGroupsNV _glr->glExt.glXQueryMaxSwapGroupsNV
#define glXQueryFrameCountNV _glr->glExt.glXQueryFrameCountNV
#define glXResetFrameCountNV _glr->glExt.glXResetFrameCountNV
// GLX_NV_video_capture
#define glXBindVideoCaptureDeviceNV _glr->glExt.glXBindVideoCaptureDeviceNV
#define glXEnumerateVideoCaptureDevicesNV _glr->glExt.glXEnumerateVideoCaptureDevicesNV
#define glXLockVideoCaptureDeviceNV _glr->glExt.glXLockVideoCaptureDeviceNV
#define glXQueryVideoCaptureDeviceNV _glr->glExt.glXQueryVideoCaptureDeviceNV
#define glXReleaseVideoCaptureDeviceNV _glr->glExt.glXReleaseVideoCaptureDeviceNV
// GLX_NV_video_output
#define glXGetVideoDeviceNV _glr->glExt.glXGetVideoDeviceNV
#define glXReleaseVideoDeviceNV _glr->glExt.glXReleaseVideoDeviceNV
#define glXBindVideoImageNV _glr->glExt.glXBindVideoImageNV
#define glXReleaseVideoImageNV _glr->glExt.glXReleaseVideoImageNV
#define glXSendPbufferToVideoNV _glr->glExt.glXSendPbufferToVideoNV
#define glXGetVideoInfoNV _glr->glExt.glXGetVideoInfoNV
// GLX_OML_sync_control
#define glXGetSyncValuesOML _glr->glExt.glXGetSyncValuesOML
#define glXGetMscRateOML _glr->glExt.glXGetMscRateOML
#define glXSwapBuffersMscOML _glr->glExt.glXSwapBuffersMscOML
#define glXWaitForMscOML _glr->glExt.glXWaitForMscOML
#define glXWaitForSbcOML _glr->glExt.glXWaitForSbcOML
// GLX_SGIX_dmbuffer
#define glXAssociateDMPbufferSGIX _glr->glExt.glXAssociateDMPbufferSGIX
// GLX_SGIX_fbconfig
#define glXGetFBConfigAttribSGIX _glr->glExt.glXGetFBConfigAttribSGIX
#define glXChooseFBConfigSGIX _glr->glExt.glXChooseFBConfigSGIX
#define glXCreateGLXPixmapWithConfigSGIX _glr->glExt.glXCreateGLXPixmapWithConfigSGIX
#define glXCreateContextWithConfigSGIX _glr->glExt.glXCreateContextWithConfigSGIX
#define glXGetVisualFromFBConfigSGIX _glr->glExt.glXGetVisualFromFBConfigSGIX
#define glXGetFBConfigFromVisualSGIX _glr->glExt.glXGetFBConfigFromVisualSGIX
// GLX_SGIX_hyperpipe
#define glXQueryHyperpipeNetworkSGIX _glr->glExt.glXQueryHyperpipeNetworkSGIX
#define glXHyperpipeConfigSGIX _glr->glExt.glXHyperpipeConfigSGIX
#define glXQueryHyperpipeConfigSGIX _glr->glExt.glXQueryHyperpipeConfigSGIX
#define glXDestroyHyperpipeConfigSGIX _glr->glExt.glXDestroyHyperpipeConfigSGIX
#define glXBindHyperpipeSGIX _glr->glExt.glXBindHyperpipeSGIX
#define glXQueryHyperpipeBestAttribSGIX _glr->glExt.glXQueryHyperpipeBestAttribSGIX
#define glXHyperpipeAttribSGIX _glr->glExt.glXHyperpipeAttribSGIX
#define glXQueryHyperpipeAttribSGIX _glr->glExt.glXQueryHyperpipeAttribSGIX
// GLX_SGIX_pbuffer
#define glXCreateGLXPbufferSGIX _glr->glExt.glXCreateGLXPbufferSGIX
#define glXDestroyGLXPbufferSGIX _glr->glExt.glXDestroyGLXPbufferSGIX
#define glXQueryGLXPbufferSGIX _glr->glExt.glXQueryGLXPbufferSGIX
#define glXSelectEventSGIX _glr->glExt.glXSelectEventSGIX
#define glXGetSelectedEventSGIX _glr->glExt.glXGetSelectedEventSGIX
// GLX_SGIX_swap_barrier
#define glXBindSwapBarrierSGIX _glr->glExt.glXBindSwapBarrierSGIX
#define glXQueryMaxSwapBarriersSGIX _glr->glExt.glXQueryMaxSwapBarriersSGIX
// GLX_SGIX_swap_group
#define glXJoinSwapGroupSGIX _glr->glExt.glXJoinSwapGroupSGIX
// GLX_SGIX_video_resize
#define glXBindChannelToWindowSGIX _glr->glExt.glXBindChannelToWindowSGIX
#define glXChannelRectSGIX _glr->glExt.glXChannelRectSGIX
#define glXQueryChannelRectSGIX _glr->glExt.glXQueryChannelRectSGIX
#define glXQueryChannelDeltasSGIX _glr->glExt.glXQueryChannelDeltasSGIX
#define glXChannelRectSyncSGIX _glr->glExt.glXChannelRectSyncSGIX
// GLX_SGIX_video_source
#define glXCreateGLXVideoSourceSGIX _glr->glExt.glXCreateGLXVideoSourceSGIX
#define glXDestroyGLXVideoSourceSGIX _glr->glExt.glXDestroyGLXVideoSourceSGIX
// GLX_SGI_cushion
#define glXCushionSGI _glr->glExt.glXCushionSGI
// GLX_SGI_make_current_read
#define glXMakeCurrentReadSGI _glr->glExt.glXMakeCurrentReadSGI
#define glXGetCurrentReadDrawableSGI _glr->glExt.glXGetCurrentReadDrawableSGI
// GLX_SGI_swap_control
#define glXSwapIntervalSGI _glr->glExt.glXSwapIntervalSGI
// GLX_SGI_video_sync
#define glXGetVideoSyncSGI _glr->glExt.glXGetVideoSyncSGI
#define glXWaitVideoSyncSGI _glr->glExt.glXWaitVideoSyncSGI
// GLX_SUN_get_transparent_index
#define glXGetTransparentIndexSUN _glr->glExt.glXGetTransparentIndexSUN

#endif /// OS_LINUX








///=======================///
// EXT & vendor extensions //
///=======================///


// #291 GL_OES_byte_coordinates
#define glMultiTexCoord1bOES _glr->glExt.glMultiTexCoord1bOES
#define glMultiTexCoord1bvOES _glr->glExt.glMultiTexCoord1bvOES
#define glMultiTexCoord2bOES _glr->glExt.glMultiTexCoord2bOES
#define glMultiTexCoord2bvOES _glr->glExt.glMultiTexCoord2bvOES
#define glMultiTexCoord3bOES _glr->glExt.glMultiTexCoord3bOES
#define glMultiTexCoord3bvOES _glr->glExt.glMultiTexCoord3bvOES
#define glMultiTexCoord4bOES _glr->glExt.glMultiTexCoord4bOES
#define glMultiTexCoord4bvOES _glr->glExt.glMultiTexCoord4bvOES
#define glTexCoord1bOES _glr->glExt.glTexCoord1bOES
#define glTexCoord1bvOES _glr->glExt.glTexCoord1bvOES
#define glTexCoord2bOES _glr->glExt.glTexCoord2bOES
#define glTexCoord2bvOES _glr->glExt.glTexCoord2bvOES
#define glTexCoord3bOES _glr->glExt.glTexCoord3bOES
#define glTexCoord3bvOES _glr->glExt.glTexCoord3bvOES
#define glTexCoord4bOES _glr->glExt.glTexCoord4bOES
#define glTexCoord4bvOES _glr->glExt.glTexCoord4bvOES
#define glVertex2bOES _glr->glExt.glVertex2bOES
#define glVertex2bvOES _glr->glExt.glVertex2bvOES
#define glVertex3bOES _glr->glExt.glVertex3bOES
#define glVertex3bvOES _glr->glExt.glVertex3bvOES
#define glVertex4bOES _glr->glExt.glVertex4bOES
#define glVertex4bvOES _glr->glExt.glVertex4bvOES
// #292 GL_OES_fixed_point
#define glAlphaFuncxOES _glr->glExt.glAlphaFuncxOES
#define glClearColorxOES _glr->glExt.glClearColorxOES
#define glClearDepthxOES _glr->glExt.glClearDepthxOES
#define glClipPlanexOES _glr->glExt.glClipPlanexOES
#define glColor4xOES _glr->glExt.glColor4xOES
#define glDepthRangexOES _glr->glExt.glDepthRangexOES
#define glFogxOES _glr->glExt.glFogxOES
#define glFogxvOES _glr->glExt.glFogxvOES
#define glFrustumxOES _glr->glExt.glFrustumxOES
#define glGetClipPlanexOES _glr->glExt.glGetClipPlanexOES
#define glGetFixedvOES _glr->glExt.glGetFixedvOES
#define glGetTexEnvxvOES _glr->glExt.glGetTexEnvxvOES
#define glGetTexParameterxvOES _glr->glExt.glGetTexParameterxvOES
#define glLightModelxOES _glr->glExt.glLightModelxOES
#define glLightModelxvOES _glr->glExt.glLightModelxvOES
#define glLightxOES _glr->glExt.glLightxOES
#define glLightxvOES _glr->glExt.glLightxvOES
#define glLineWidthxOES _glr->glExt.glLineWidthxOES
#define glLoadMatrixxOES _glr->glExt.glLoadMatrixxOES
#define glMaterialxOES _glr->glExt.glMaterialxOES
#define glMaterialxvOES _glr->glExt.glMaterialxvOES
#define glMultMatrixxOES _glr->glExt.glMultMatrixxOES
#define glMultiTexCoord4xOES _glr->glExt.glMultiTexCoord4xOES
#define glNormal3xOES _glr->glExt.glNormal3xOES
#define glOrthoxOES _glr->glExt.glOrthoxOES
#define glPointParameterxvOES _glr->glExt.glPointParameterxvOES
#define glPointSizexOES _glr->glExt.glPointSizexOES
#define glPolygonOffsetxOES _glr->glExt.glPolygonOffsetxOES
#define glRotatexOES _glr->glExt.glRotatexOES
#define glSampleCoverageOES _glr->glExt.glSampleCoverageOES
#define glScalexOES _glr->glExt.glScalexOES
#define glTexEnvxOES _glr->glExt.glTexEnvxOES
#define glTexEnvxvOES _glr->glExt.glTexEnvxvOES
#define glTexParameterxOES _glr->glExt.glTexParameterxOES
#define glTexParameterxvOES _glr->glExt.glTexParameterxvOES
#define glTranslatexOES _glr->glExt.glTranslatexOES
#define glAccumxOES _glr->glExt.glAccumxOES
#define glBitmapxOES _glr->glExt.glBitmapxOES
#define glBlendColorxOES _glr->glExt.glBlendColorxOES
#define glClearAccumxOES _glr->glExt.glClearAccumxOES
#define glColor3xOES _glr->glExt.glColor3xOES
#define glColor3xvOES _glr->glExt.glColor3xvOES
#define glColor4xvOES _glr->glExt.glColor4xvOES
#define glConvolutionParameterxOES _glr->glExt.glConvolutionParameterxOES
#define glConvolutionParameterxvOES _glr->glExt.glConvolutionParameterxvOES
#define glEvalCoord1xOES _glr->glExt.glEvalCoord1xOES
#define glEvalCoord1xvOES _glr->glExt.glEvalCoord1xvOES
#define glEvalCoord2xOES _glr->glExt.glEvalCoord2xOES
#define glEvalCoord2xvOES _glr->glExt.glEvalCoord2xvOES
#define glFeedbackBufferxOES _glr->glExt.glFeedbackBufferxOES
#define glGetConvolutionParameterxvOES _glr->glExt.glGetConvolutionParameterxvOES
#define glGetHistogramParameterxvOES _glr->glExt.glGetHistogramParameterxvOES
#define glGetLightxOES _glr->glExt.glGetLightxOES
#define glGetMapxvOES _glr->glExt.glGetMapxvOES
#define glGetMaterialxOES _glr->glExt.glGetMaterialxOES
#define glGetPixelMapxv _glr->glExt.glGetPixelMapxv
#define glGetTexGenxvOES _glr->glExt.glGetTexGenxvOES
#define glGetTexLevelParameterxvOES _glr->glExt.glGetTexLevelParameterxvOES
#define glIndexxOES _glr->glExt.glIndexxOES
#define glIndexxvOES _glr->glExt.glIndexxvOES
#define glLoadTransposeMatrixxOES _glr->glExt.glLoadTransposeMatrixxOES
#define glMap1xOES _glr->glExt.glMap1xOES
#define glMap2xOES _glr->glExt.glMap2xOES
#define glMapGrid1xOES _glr->glExt.glMapGrid1xOES
#define glMapGrid2xOES _glr->glExt.glMapGrid2xOES
#define glMultTransposeMatrixxOES _glr->glExt.glMultTransposeMatrixxOES
#define glMultiTexCoord1xOES _glr->glExt.glMultiTexCoord1xOES
#define glMultiTexCoord1xvOES _glr->glExt.glMultiTexCoord1xvOES
#define glMultiTexCoord2xOES _glr->glExt.glMultiTexCoord2xOES
#define glMultiTexCoord2xvOES _glr->glExt.glMultiTexCoord2xvOES
#define glMultiTexCoord3xOES _glr->glExt.glMultiTexCoord3xOES
#define glMultiTexCoord3xvOES _glr->glExt.glMultiTexCoord3xvOES
#define glMultiTexCoord4xvOES _glr->glExt.glMultiTexCoord4xvOES
#define glNormal3xvOES _glr->glExt.glNormal3xvOES
#define glPassThroughxOES _glr->glExt.glPassThroughxOES
#define glPixelMapx _glr->glExt.glPixelMapx
#define glPixelStorex _glr->glExt.glPixelStorex
#define glPixelTransferxOES _glr->glExt.glPixelTransferxOES
#define glPixelZoomxOES _glr->glExt.glPixelZoomxOES
#define glPrioritizeTexturesxOES _glr->glExt.glPrioritizeTexturesxOES
#define glRasterPos2xOES _glr->glExt.glRasterPos2xOES
#define glRasterPos2xvOES _glr->glExt.glRasterPos2xvOES
#define glRasterPos3xOES _glr->glExt.glRasterPos3xOES
#define glRasterPos3xvOES _glr->glExt.glRasterPos3xvOES
#define glRasterPos4xOES _glr->glExt.glRasterPos4xOES
#define glRasterPos4xvOES _glr->glExt.glRasterPos4xvOES
#define glRectxOES _glr->glExt.glRectxOES
#define glRectxvOES _glr->glExt.glRectxvOES
#define glTexCoord1xOES _glr->glExt.glTexCoord1xOES
#define glTexCoord1xvOES _glr->glExt.glTexCoord1xvOES
#define glTexCoord2xOES _glr->glExt.glTexCoord2xOES
#define glTexCoord2xvOES _glr->glExt.glTexCoord2xvOES
#define glTexCoord3xOES _glr->glExt.glTexCoord3xOES
#define glTexCoord3xvOES _glr->glExt.glTexCoord3xvOES
#define glTexCoord4xOES _glr->glExt.glTexCoord4xOES
#define glTexCoord4xvOES _glr->glExt.glTexCoord4xvOES
#define glTexGenxOES _glr->glExt.glTexGenxOES
#define glTexGenxvOES _glr->glExt.glTexGenxvOES
#define glVertex2xOES _glr->glExt.glVertex2xOES
#define glVertex2xvOES _glr->glExt.glVertex2xvOES
#define glVertex3xOES _glr->glExt.glVertex3xOES
#define glVertex3xvOES _glr->glExt.glVertex3xvOES
#define glVertex4xOES _glr->glExt.glVertex4xOES
#define glVertex4xvOES _glr->glExt.glVertex4xvOES
// #296 GL_OES_query_matrix
#define glQueryMatrixxOES _glr->glExt.glQueryMatrixxOES
// #293 GL_OES_single_precision
#define glClearDepthfOES _glr->glExt.glClearDepthfOES
#define glClipPlanefOES _glr->glExt.glClipPlanefOES
#define glDepthRangefOES _glr->glExt.glDepthRangefOES
#define glFrustumfOES _glr->glExt.glFrustumfOES
#define glGetClipPlanefOES _glr->glExt.glGetClipPlanefOES
#define glOrthofOES _glr->glExt.glOrthofOES
// #208 GL_3DFX_tbuffer
#define glTbufferMask3DFX _glr->glExt.glTbufferMask3DFX
// #395 GL_AMD_debug_output
#define glDebugMessageEnableAMD _glr->glExt.glDebugMessageEnableAMD
#define glDebugMessageInsertAMD _glr->glExt.glDebugMessageInsertAMD
#define glDebugMessageCallbackAMD _glr->glExt.glDebugMessageCallbackAMD
#define glGetDebugMessageLogAMD _glr->glExt.glGetDebugMessageLogAMD
// #366 GL_AMD_draw_buffers_blend
#define glBlendFuncIndexedAMD _glr->glExt.glBlendFuncIndexedAMD
#define glBlendFuncSeparateIndexedAMD _glr->glExt.glBlendFuncSeparateIndexedAMD
#define glBlendEquationIndexedAMD _glr->glExt.glBlendEquationIndexedAMD
#define glBlendEquationSeparateIndexedAMD _glr->glExt.glBlendEquationSeparateIndexedAMD
// #451 GL_AMD_gpu_shader_int64
#define glUniform1i64NV _glr->glExt.glUniform1i64NV
#define glUniform2i64NV _glr->glExt.glUniform2i64NV
#define glUniform3i64NV _glr->glExt.glUniform3i64NV
#define glUniform4i64NV _glr->glExt.glUniform4i64NV
#define glUniform1i64vNV _glr->glExt.glUniform1i64vNV
#define glUniform2i64vNV _glr->glExt.glUniform2i64vNV
#define glUniform3i64vNV _glr->glExt.glUniform3i64vNV
#define glUniform4i64vNV _glr->glExt.glUniform4i64vNV
#define glUniform1ui64NV _glr->glExt.glUniform1ui64NV
#define glUniform2ui64NV _glr->glExt.glUniform2ui64NV
#define glUniform3ui64NV _glr->glExt.glUniform3ui64NV
#define glUniform4ui64NV _glr->glExt.glUniform4ui64NV
#define glUniform1ui64vNV _glr->glExt.glUniform1ui64vNV
#define glUniform2ui64vNV _glr->glExt.glUniform2ui64vNV
#define glUniform3ui64vNV _glr->glExt.glUniform3ui64vNV
#define glUniform4ui64vNV _glr->glExt.glUniform4ui64vNV
#define glGetUniformi64vNV _glr->glExt.glGetUniformi64vNV
#define glGetUniformui64vNV _glr->glExt.glGetUniformui64vNV
#define glProgramUniform1i64NV _glr->glExt.glProgramUniform1i64NV
#define glProgramUniform2i64NV _glr->glExt.glProgramUniform2i64NV
#define glProgramUniform3i64NV _glr->glExt.glProgramUniform3i64NV
#define glProgramUniform4i64NV _glr->glExt.glProgramUniform4i64NV
#define glProgramUniform1i64vNV _glr->glExt.glProgramUniform1i64vNV
#define glProgramUniform2i64vNV _glr->glExt.glProgramUniform2i64vNV
#define glProgramUniform3i64vNV _glr->glExt.glProgramUniform3i64vNV
#define glProgramUniform4i64vNV _glr->glExt.glProgramUniform4i64vNV
#define glProgramUniform1ui64NV _glr->glExt.glProgramUniform1ui64NV
#define glProgramUniform2ui64NV _glr->glExt.glProgramUniform2ui64NV
#define glProgramUniform3ui64NV _glr->glExt.glProgramUniform3ui64NV
#define glProgramUniform4ui64NV _glr->glExt.glProgramUniform4ui64NV
#define glProgramUniform1ui64vNV _glr->glExt.glProgramUniform1ui64vNV
#define glProgramUniform2ui64vNV _glr->glExt.glProgramUniform2ui64vNV
#define glProgramUniform3ui64vNV _glr->glExt.glProgramUniform3ui64vNV
#define glProgramUniform4ui64vNV _glr->glExt.glProgramUniform4ui64vNV
// #431 GL_AMD_interleaved_elements
#define glVertexAttribParameteriAMD _glr->glExt.glVertexAttribParameteriAMD
// #408 GL_AMD_multi_draw_indirect
#define glMultiDrawArraysIndirectAMD _glr->glExt.glMultiDrawArraysIndirectAMD
#define glMultiDrawElementsIndirectAMD _glr->glExt.glMultiDrawElementsIndirectAMD
// #394 GL_AMD_name_gen_delete
#define glGenNamesAMD _glr->glExt.glGenNamesAMD
#define glDeleteNamesAMD _glr->glExt.glDeleteNamesAMD
#define glIsNameAMD _glr->glExt.glIsNameAMD
// #442 GL_AMD_occlusion_query_event
#define glQueryObjectParameteruiAMD _glr->glExt.glQueryObjectParameteruiAMD
// #360 GL_AMD_performance_monitor
#define glGetPerfMonitorGroupsAMD _glr->glExt.glGetPerfMonitorGroupsAMD
#define glGetPerfMonitorCountersAMD _glr->glExt.glGetPerfMonitorCountersAMD
#define glGetPerfMonitorGroupStringAMD _glr->glExt.glGetPerfMonitorGroupStringAMD
#define glGetPerfMonitorCounterStringAMD _glr->glExt.glGetPerfMonitorCounterStringAMD
#define glGetPerfMonitorCounterInfoAMD _glr->glExt.glGetPerfMonitorCounterInfoAMD
#define glGenPerfMonitorsAMD _glr->glExt.glGenPerfMonitorsAMD
#define glDeletePerfMonitorsAMD _glr->glExt.glDeletePerfMonitorsAMD
#define glSelectPerfMonitorCountersAMD _glr->glExt.glSelectPerfMonitorCountersAMD
#define glBeginPerfMonitorAMD _glr->glExt.glBeginPerfMonitorAMD
#define glEndPerfMonitorAMD _glr->glExt.glEndPerfMonitorAMD
#define glGetPerfMonitorCounterDataAMD _glr->glExt.glGetPerfMonitorCounterDataAMD
// #405 GL_AMD_sample_positions
#define glSetMultisamplefvAMD _glr->glExt.glSetMultisamplefvAMD
// #426 GL_AMD_sparse_texture
#define glTexStorageSparseAMD _glr->glExt.glTexStorageSparseAMD
#define glTextureStorageSparseAMD _glr->glExt.glTextureStorageSparseAMD
// #413 GL_AMD_stencil_operation_extended
#define glStencilOpValueAMD _glr->glExt.glStencilOpValueAMD
// #363 GL_AMD_vertex_shader_tessellator
#define glTessellationFactorAMD _glr->glExt.glTessellationFactorAMD
#define glTessellationModeAMD _glr->glExt.glTessellationModeAMD
// #271 GL_APPLE_element_array
#define glElementPointerAPPLE _glr->glExt.glElementPointerAPPLE
#define glDrawElementArrayAPPLE _glr->glExt.glDrawElementArrayAPPLE
#define glDrawRangeElementArrayAPPLE _glr->glExt.glDrawRangeElementArrayAPPLE
#define glMultiDrawElementArrayAPPLE _glr->glExt.glMultiDrawElementArrayAPPLE
#define glMultiDrawRangeElementArrayAPPLE _glr->glExt.glMultiDrawRangeElementArrayAPPLE
// #272 GL_APPLE_fence
#define glGenFencesAPPLE _glr->glExt.glGenFencesAPPLE
#define glDeleteFencesAPPLE _glr->glExt.glDeleteFencesAPPLE
#define glSetFenceAPPLE _glr->glExt.glSetFenceAPPLE
#define glIsFenceAPPLE _glr->glExt.glIsFenceAPPLE
#define glTestFenceAPPLE _glr->glExt.glTestFenceAPPLE
#define glFinishFenceAPPLE _glr->glExt.glFinishFenceAPPLE
#define glTestObjectAPPLE _glr->glExt.glTestObjectAPPLE
#define glFinishObjectAPPLE _glr->glExt.glFinishObjectAPPLE
// #321 GL_APPLE_flush_buffer_range
#define glBufferParameteriAPPLE _glr->glExt.glBufferParameteriAPPLE
#define glFlushMappedBufferRangeAPPLE _glr->glExt.glFlushMappedBufferRangeAPPLE
// #371 GL_APPLE_object_purgeable
#define glObjectPurgeableAPPLE _glr->glExt.glObjectPurgeableAPPLE
#define glObjectUnpurgeableAPPLE _glr->glExt.glObjectUnpurgeableAPPLE
#define glGetObjectParameterivAPPLE _glr->glExt.glGetObjectParameterivAPPLE
// #367 GL_APPLE_texture_range
#define glTextureRangeAPPLE _glr->glExt.glTextureRangeAPPLE
#define glGetTexParameterPointervAPPLE _glr->glExt.glGetTexParameterPointervAPPLE
// #273 GL_APPLE_vertex_array_object
#define glBindVertexArrayAPPLE _glr->glExt.glBindVertexArrayAPPLE
#define glDeleteVertexArraysAPPLE _glr->glExt.glDeleteVertexArraysAPPLE
#define glGenVertexArraysAPPLE _glr->glExt.glGenVertexArraysAPPLE
#define glIsVertexArrayAPPLE _glr->glExt.glIsVertexArrayAPPLE
// #274 GL_APPLE_vertex_array_range
#define glVertexArrayRangeAPPLE _glr->glExt.glVertexArrayRangeAPPLE
#define glFlushVertexArrayRangeAPPLE _glr->glExt.glFlushVertexArrayRangeAPPLE
#define glVertexArrayParameteriAPPLE _glr->glExt.glVertexArrayParameteriAPPLE
// #369 GL_APPLE_vertex_program_evaluators
#define glEnableVertexAttribAPPLE _glr->glExt.glEnableVertexAttribAPPLE
#define glDisableVertexAttribAPPLE _glr->glExt.glDisableVertexAttribAPPLE
#define glIsVertexAttribEnabledAPPLE _glr->glExt.glIsVertexAttribEnabledAPPLE
#define glMapVertexAttrib1dAPPLE _glr->glExt.glMapVertexAttrib1dAPPLE
#define glMapVertexAttrib1fAPPLE _glr->glExt.glMapVertexAttrib1fAPPLE
#define glMapVertexAttrib2dAPPLE _glr->glExt.glMapVertexAttrib2dAPPLE
#define glMapVertexAttrib2fAPPLE _glr->glExt.glMapVertexAttrib2fAPPLE
// #277 GL_ATI_draw_buffers
#define glDrawBuffersATI _glr->glExt.glDrawBuffersATI
// #256 GL_ATI_element_array
#define glElementPointerATI _glr->glExt.glElementPointerATI
#define glDrawElementArrayATI _glr->glExt.glDrawElementArrayATI
#define glDrawRangeElementArrayATI _glr->glExt.glDrawRangeElementArrayATI
// #244 GL_ATI_envmap_bumpmap
#define glTexBumpParameterivATI _glr->glExt.glTexBumpParameterivATI
#define glTexBumpParameterfvATI _glr->glExt.glTexBumpParameterfvATI
#define glGetTexBumpParameterivATI _glr->glExt.glGetTexBumpParameterivATI
#define glGetTexBumpParameterfvATI _glr->glExt.glGetTexBumpParameterfvATI
// #245 GL_ATI_fragment_shader
#define glGenFragmentShadersATI _glr->glExt.glGenFragmentShadersATI
#define glBindFragmentShaderATI _glr->glExt.glBindFragmentShaderATI
#define glDeleteFragmentShaderATI _glr->glExt.glDeleteFragmentShaderATI
#define glBeginFragmentShaderATI _glr->glExt.glBeginFragmentShaderATI
#define glEndFragmentShaderATI _glr->glExt.glEndFragmentShaderATI
#define glPassTexCoordATI _glr->glExt.glPassTexCoordATI
#define glSampleMapATI _glr->glExt.glSampleMapATI
#define glColorFragmentOp1ATI _glr->glExt.glColorFragmentOp1ATI
#define glColorFragmentOp2ATI _glr->glExt.glColorFragmentOp2ATI
#define glColorFragmentOp3ATI _glr->glExt.glColorFragmentOp3ATI
#define glAlphaFragmentOp1ATI _glr->glExt.glAlphaFragmentOp1ATI
#define glAlphaFragmentOp2ATI _glr->glExt.glAlphaFragmentOp2ATI
#define glAlphaFragmentOp3ATI _glr->glExt.glAlphaFragmentOp3ATI
#define glSetFragmentShaderConstantATI _glr->glExt.glSetFragmentShaderConstantATI
// #288 GL_ATI_map_object_buffer
#define glMapObjectBufferATI _glr->glExt.glMapObjectBufferATI
#define glUnmapObjectBufferATI _glr->glExt.glUnmapObjectBufferATI
// #246 GL_ATI_pn_triangles
#define glPNTrianglesiATI _glr->glExt.glPNTrianglesiATI
#define glPNTrianglesfATI _glr->glExt.glPNTrianglesfATI
// #289 GL_ATI_separate_stencil
#define glStencilOpSeparateATI _glr->glExt.glStencilOpSeparateATI
#define glStencilFuncSeparateATI _glr->glExt.glStencilFuncSeparateATI
// #247 GL_ATI_vertex_array_object
#define glNewObjectBufferATI _glr->glExt.glNewObjectBufferATI
#define glIsObjectBufferATI _glr->glExt.glIsObjectBufferATI
#define glUpdateObjectBufferATI _glr->glExt.glUpdateObjectBufferATI
#define glGetObjectBufferfvATI _glr->glExt.glGetObjectBufferfvATI
#define glGetObjectBufferivATI _glr->glExt.glGetObjectBufferivATI
#define glFreeObjectBufferATI _glr->glExt.glFreeObjectBufferATI
#define glArrayObjectATI _glr->glExt.glArrayObjectATI
#define glGetArrayObjectfvATI _glr->glExt.glGetArrayObjectfvATI
#define glGetArrayObjectivATI _glr->glExt.glGetArrayObjectivATI
#define glVariantArrayObjectATI _glr->glExt.glVariantArrayObjectATI
#define glGetVariantArrayObjectfvATI _glr->glExt.glGetVariantArrayObjectfvATI
#define glGetVariantArrayObjectivATI _glr->glExt.glGetVariantArrayObjectivATI
// #290 GL_ATI_vertex_attrib_array_object
#define glVertexAttribArrayObjectATI _glr->glExt.glVertexAttribArrayObjectATI
#define glGetVertexAttribArrayObjectfvATI _glr->glExt.glGetVertexAttribArrayObjectfvATI
#define glGetVertexAttribArrayObjectivATI _glr->glExt.glGetVertexAttribArrayObjectivATI
// #249 GL_ATI_vertex_streams
#define glVertexStream1sATI _glr->glExt.glVertexStream1sATI
#define glVertexStream1svATI _glr->glExt.glVertexStream1svATI
#define glVertexStream1iATI _glr->glExt.glVertexStream1iATI
#define glVertexStream1ivATI _glr->glExt.glVertexStream1ivATI
#define glVertexStream1fATI _glr->glExt.glVertexStream1fATI
#define glVertexStream1fvATI _glr->glExt.glVertexStream1fvATI
#define glVertexStream1dATI _glr->glExt.glVertexStream1dATI
#define glVertexStream1dvATI _glr->glExt.glVertexStream1dvATI
#define glVertexStream2sATI _glr->glExt.glVertexStream2sATI
#define glVertexStream2svATI _glr->glExt.glVertexStream2svATI
#define glVertexStream2iATI _glr->glExt.glVertexStream2iATI
#define glVertexStream2ivATI _glr->glExt.glVertexStream2ivATI
#define glVertexStream2fATI _glr->glExt.glVertexStream2fATI
#define glVertexStream2fvATI _glr->glExt.glVertexStream2fvATI
#define glVertexStream2dATI _glr->glExt.glVertexStream2dATI
#define glVertexStream2dvATI _glr->glExt.glVertexStream2dvATI
#define glVertexStream3sATI _glr->glExt.glVertexStream3sATI
#define glVertexStream3svATI _glr->glExt.glVertexStream3svATI
#define glVertexStream3iATI _glr->glExt.glVertexStream3iATI
#define glVertexStream3ivATI _glr->glExt.glVertexStream3ivATI
#define glVertexStream3fATI _glr->glExt.glVertexStream3fATI
#define glVertexStream3fvATI _glr->glExt.glVertexStream3fvATI
#define glVertexStream3dATI _glr->glExt.glVertexStream3dATI
#define glVertexStream3dvATI _glr->glExt.glVertexStream3dvATI
#define glVertexStream4sATI _glr->glExt.glVertexStream4sATI
#define glVertexStream4svATI _glr->glExt.glVertexStream4svATI
#define glVertexStream4iATI _glr->glExt.glVertexStream4iATI
#define glVertexStream4ivATI _glr->glExt.glVertexStream4ivATI
#define glVertexStream4fATI _glr->glExt.glVertexStream4fATI
#define glVertexStream4fvATI _glr->glExt.glVertexStream4fvATI
#define glVertexStream4dATI _glr->glExt.glVertexStream4dATI
#define glVertexStream4dvATI _glr->glExt.glVertexStream4dvATI
#define glNormalStream3bATI _glr->glExt.glNormalStream3bATI
#define glNormalStream3bvATI _glr->glExt.glNormalStream3bvATI
#define glNormalStream3sATI _glr->glExt.glNormalStream3sATI
#define glNormalStream3svATI _glr->glExt.glNormalStream3svATI
#define glNormalStream3iATI _glr->glExt.glNormalStream3iATI
#define glNormalStream3ivATI _glr->glExt.glNormalStream3ivATI
#define glNormalStream3fATI _glr->glExt.glNormalStream3fATI
#define glNormalStream3fvATI _glr->glExt.glNormalStream3fvATI
#define glNormalStream3dATI _glr->glExt.glNormalStream3dATI
#define glNormalStream3dvATI _glr->glExt.glNormalStream3dvATI
#define glClientActiveVertexStreamATI _glr->glExt.glClientActiveVertexStreamATI
#define glVertexBlendEnviATI _glr->glExt.glVertexBlendEnviATI
#define glVertexBlendEnvfATI _glr->glExt.glVertexBlendEnvfATI
// #342 GL_EXT_bindable_uniform
#define glUniformBufferEXT _glr->glExt.glUniformBufferEXT
#define glGetUniformBufferSizeEXT _glr->glExt.glGetUniformBufferSizeEXT
#define glGetUniformOffsetEXT _glr->glExt.glGetUniformOffsetEXT
// #002 GL_EXT_blend_color
#define glBlendColorEXT _glr->glExt.glBlendColorEXT
// #299 GL_EXT_blend_equation_separate
#define glBlendEquationSeparateEXT _glr->glExt.glBlendEquationSeparateEXT
// #173 GL_EXT_blend_func_separate
#define glBlendFuncSeparateEXT _glr->glExt.glBlendFuncSeparateEXT
// #037 GL_EXT_blend_minmax
#define glBlendEquationEXT _glr->glExt.glBlendEquationEXT
// #074 GL_EXT_color_subtable
#define glColorSubTableEXT _glr->glExt.glColorSubTableEXT
#define glCopyColorSubTableEXT _glr->glExt.glCopyColorSubTableEXT
// #097 GL_EXT_compiled_vertex_array
#define glLockArraysEXT _glr->glExt.glLockArraysEXT
#define glUnlockArraysEXT _glr->glExt.glUnlockArraysEXT
// #012 GL_EXT_convolution
#define glConvolutionFilter1DEXT _glr->glExt.glConvolutionFilter1DEXT
#define glConvolutionFilter2DEXT _glr->glExt.glConvolutionFilter2DEXT
#define glConvolutionParameterfEXT _glr->glExt.glConvolutionParameterfEXT
#define glConvolutionParameterfvEXT _glr->glExt.glConvolutionParameterfvEXT
#define glConvolutionParameteriEXT _glr->glExt.glConvolutionParameteriEXT
#define glConvolutionParameterivEXT _glr->glExt.glConvolutionParameterivEXT
#define glCopyConvolutionFilter1DEXT _glr->glExt.glCopyConvolutionFilter1DEXT
#define glCopyConvolutionFilter2DEXT _glr->glExt.glCopyConvolutionFilter2DEXT
#define glGetConvolutionFilterEXT _glr->glExt.glGetConvolutionFilterEXT
#define glGetConvolutionParameterfvEXT _glr->glExt.glGetConvolutionParameterfvEXT
#define glGetConvolutionParameterivEXT _glr->glExt.glGetConvolutionParameterivEXT
#define glGetSeparableFilterEXT _glr->glExt.glGetSeparableFilterEXT
#define glSeparableFilter2DEXT _glr->glExt.glSeparableFilter2DEXT
// #156 GL_EXT_coordinate_frame
#define glTangent3bEXT _glr->glExt.glTangent3bEXT
#define glTangent3bvEXT _glr->glExt.glTangent3bvEXT
#define glTangent3dEXT _glr->glExt.glTangent3dEXT
#define glTangent3dvEXT _glr->glExt.glTangent3dvEXT
#define glTangent3fEXT _glr->glExt.glTangent3fEXT
#define glTangent3fvEXT _glr->glExt.glTangent3fvEXT
#define glTangent3iEXT _glr->glExt.glTangent3iEXT
#define glTangent3ivEXT _glr->glExt.glTangent3ivEXT
#define glTangent3sEXT _glr->glExt.glTangent3sEXT
#define glTangent3svEXT _glr->glExt.glTangent3svEXT
#define glBinormal3bEXT _glr->glExt.glBinormal3bEXT
#define glBinormal3bvEXT _glr->glExt.glBinormal3bvEXT
#define glBinormal3dEXT _glr->glExt.glBinormal3dEXT
#define glBinormal3dvEXT _glr->glExt.glBinormal3dvEXT
#define glBinormal3fEXT _glr->glExt.glBinormal3fEXT
#define glBinormal3fvEXT _glr->glExt.glBinormal3fvEXT
#define glBinormal3iEXT _glr->glExt.glBinormal3iEXT
#define glBinormal3ivEXT _glr->glExt.glBinormal3ivEXT
#define glBinormal3sEXT _glr->glExt.glBinormal3sEXT
#define glBinormal3svEXT _glr->glExt.glBinormal3svEXT
#define glTangentPointerEXT _glr->glExt.glTangentPointerEXT
#define glBinormalPointerEXT _glr->glExt.glBinormalPointerEXT
// #010 GL_EXT_copy_texture
#define glCopyTexImage1DEXT _glr->glExt.glCopyTexImage1DEXT
#define glCopyTexImage2DEXT _glr->glExt.glCopyTexImage2DEXT
#define glCopyTexSubImage1DEXT _glr->glExt.glCopyTexSubImage1DEXT
#define glCopyTexSubImage2DEXT _glr->glExt.glCopyTexSubImage2DEXT
#define glCopyTexSubImage3DEXT _glr->glExt.glCopyTexSubImage3DEXT
// #098 GL_EXT_cull_vertex
#define glCullParameterdvEXT _glr->glExt.glCullParameterdvEXT
#define glCullParameterfvEXT _glr->glExt.glCullParameterfvEXT
// #439 GL_EXT_debug_label
#define glLabelObjectEXT _glr->glExt.glLabelObjectEXT
#define glGetObjectLabelEXT _glr->glExt.glGetObjectLabelEXT
// #440 GL_EXT_debug_marker
#define glInsertEventMarkerEXT _glr->glExt.glInsertEventMarkerEXT
#define glPushGroupMarkerEXT _glr->glExt.glPushGroupMarkerEXT
#define glPopGroupMarkerEXT _glr->glExt.glPopGroupMarkerEXT
// #297 GL_EXT_depth_bounds_test
#define glDepthBoundsEXT _glr->glExt.glDepthBoundsEXT
// #353 GL_EXT_direct_state_access
#define glMatrixLoadfEXT _glr->glExt.glMatrixLoadfEXT
#define glMatrixLoaddEXT _glr->glExt.glMatrixLoaddEXT
#define glMatrixMultfEXT _glr->glExt.glMatrixMultfEXT
#define glMatrixMultdEXT _glr->glExt.glMatrixMultdEXT
#define glMatrixLoadIdentityEXT _glr->glExt.glMatrixLoadIdentityEXT
#define glMatrixRotatefEXT _glr->glExt.glMatrixRotatefEXT
#define glMatrixRotatedEXT _glr->glExt.glMatrixRotatedEXT
#define glMatrixScalefEXT _glr->glExt.glMatrixScalefEXT
#define glMatrixScaledEXT _glr->glExt.glMatrixScaledEXT
#define glMatrixTranslatefEXT _glr->glExt.glMatrixTranslatefEXT
#define glMatrixTranslatedEXT _glr->glExt.glMatrixTranslatedEXT
#define glMatrixFrustumEXT _glr->glExt.glMatrixFrustumEXT
#define glMatrixOrthoEXT _glr->glExt.glMatrixOrthoEXT
#define glMatrixPopEXT _glr->glExt.glMatrixPopEXT
#define glMatrixPushEXT _glr->glExt.glMatrixPushEXT
#define glClientAttribDefaultEXT _glr->glExt.glClientAttribDefaultEXT
#define glPushClientAttribDefaultEXT _glr->glExt.glPushClientAttribDefaultEXT
#define glTextureParameterfEXT _glr->glExt.glTextureParameterfEXT
#define glTextureParameterfvEXT _glr->glExt.glTextureParameterfvEXT
#define glTextureParameteriEXT _glr->glExt.glTextureParameteriEXT
#define glTextureParameterivEXT _glr->glExt.glTextureParameterivEXT
#define glTextureImage1DEXT _glr->glExt.glTextureImage1DEXT
#define glTextureImage2DEXT _glr->glExt.glTextureImage2DEXT
#define glTextureSubImage1DEXT _glr->glExt.glTextureSubImage1DEXT
#define glTextureSubImage2DEXT _glr->glExt.glTextureSubImage2DEXT
#define glCopyTextureImage1DEXT _glr->glExt.glCopyTextureImage1DEXT
#define glCopyTextureImage2DEXT _glr->glExt.glCopyTextureImage2DEXT
#define glCopyTextureSubImage1DEXT _glr->glExt.glCopyTextureSubImage1DEXT
#define glCopyTextureSubImage2DEXT _glr->glExt.glCopyTextureSubImage2DEXT
#define glGetTextureImageEXT _glr->glExt.glGetTextureImageEXT
#define glGetTextureParameterfvEXT _glr->glExt.glGetTextureParameterfvEXT
#define glGetTextureParameterivEXT _glr->glExt.glGetTextureParameterivEXT
#define glGetTextureLevelParameterfvEXT _glr->glExt.glGetTextureLevelParameterfvEXT
#define glGetTextureLevelParameterivEXT _glr->glExt.glGetTextureLevelParameterivEXT
#define glTextureImage3DEXT _glr->glExt.glTextureImage3DEXT
#define glTextureSubImage3DEXT _glr->glExt.glTextureSubImage3DEXT
#define glCopyTextureSubImage3DEXT _glr->glExt.glCopyTextureSubImage3DEXT
#define glBindMultiTextureEXT _glr->glExt.glBindMultiTextureEXT
#define glMultiTexCoordPointerEXT _glr->glExt.glMultiTexCoordPointerEXT
#define glMultiTexEnvfEXT _glr->glExt.glMultiTexEnvfEXT
#define glMultiTexEnvfvEXT _glr->glExt.glMultiTexEnvfvEXT
#define glMultiTexEnviEXT _glr->glExt.glMultiTexEnviEXT
#define glMultiTexEnvivEXT _glr->glExt.glMultiTexEnvivEXT
#define glMultiTexGendEXT _glr->glExt.glMultiTexGendEXT
#define glMultiTexGendvEXT _glr->glExt.glMultiTexGendvEXT
#define glMultiTexGenfEXT _glr->glExt.glMultiTexGenfEXT
#define glMultiTexGenfvEXT _glr->glExt.glMultiTexGenfvEXT
#define glMultiTexGeniEXT _glr->glExt.glMultiTexGeniEXT
#define glMultiTexGenivEXT _glr->glExt.glMultiTexGenivEXT
#define glGetMultiTexEnvfvEXT _glr->glExt.glGetMultiTexEnvfvEXT
#define glGetMultiTexEnvivEXT _glr->glExt.glGetMultiTexEnvivEXT
#define glGetMultiTexGendvEXT _glr->glExt.glGetMultiTexGendvEXT
#define glGetMultiTexGenfvEXT _glr->glExt.glGetMultiTexGenfvEXT
#define glGetMultiTexGenivEXT _glr->glExt.glGetMultiTexGenivEXT
#define glMultiTexParameteriEXT _glr->glExt.glMultiTexParameteriEXT
#define glMultiTexParameterivEXT _glr->glExt.glMultiTexParameterivEXT
#define glMultiTexParameterfEXT _glr->glExt.glMultiTexParameterfEXT
#define glMultiTexParameterfvEXT _glr->glExt.glMultiTexParameterfvEXT
#define glMultiTexImage1DEXT _glr->glExt.glMultiTexImage1DEXT
#define glMultiTexImage2DEXT _glr->glExt.glMultiTexImage2DEXT
#define glMultiTexSubImage1DEXT _glr->glExt.glMultiTexSubImage1DEXT
#define glMultiTexSubImage2DEXT _glr->glExt.glMultiTexSubImage2DEXT
#define glCopyMultiTexImage1DEXT _glr->glExt.glCopyMultiTexImage1DEXT
#define glCopyMultiTexImage2DEXT _glr->glExt.glCopyMultiTexImage2DEXT
#define glCopyMultiTexSubImage1DEXT _glr->glExt.glCopyMultiTexSubImage1DEXT
#define glCopyMultiTexSubImage2DEXT _glr->glExt.glCopyMultiTexSubImage2DEXT
#define glGetMultiTexImageEXT _glr->glExt.glGetMultiTexImageEXT
#define glGetMultiTexParameterfvEXT _glr->glExt.glGetMultiTexParameterfvEXT
#define glGetMultiTexParameterivEXT _glr->glExt.glGetMultiTexParameterivEXT
#define glGetMultiTexLevelParameterfvEXT _glr->glExt.glGetMultiTexLevelParameterfvEXT
#define glGetMultiTexLevelParameterivEXT _glr->glExt.glGetMultiTexLevelParameterivEXT
#define glMultiTexImage3DEXT _glr->glExt.glMultiTexImage3DEXT
#define glMultiTexSubImage3DEXT _glr->glExt.glMultiTexSubImage3DEXT
#define glCopyMultiTexSubImage3DEXT _glr->glExt.glCopyMultiTexSubImage3DEXT
#define glEnableClientStateIndexedEXT _glr->glExt.glEnableClientStateIndexedEXT
#define glDisableClientStateIndexedEXT _glr->glExt.glDisableClientStateIndexedEXT
#define glGetFloatIndexedvEXT _glr->glExt.glGetFloatIndexedvEXT
#define glGetDoubleIndexedvEXT _glr->glExt.glGetDoubleIndexedvEXT
#define glGetPointerIndexedvEXT _glr->glExt.glGetPointerIndexedvEXT
#define glEnableIndexedEXT _glr->glExt.glEnableIndexedEXT
#define glDisableIndexedEXT _glr->glExt.glDisableIndexedEXT
#define glIsEnabledIndexedEXT _glr->glExt.glIsEnabledIndexedEXT
#define glGetIntegerIndexedvEXT _glr->glExt.glGetIntegerIndexedvEXT
#define glGetBooleanIndexedvEXT _glr->glExt.glGetBooleanIndexedvEXT
#define glCompressedTextureImage3DEXT _glr->glExt.glCompressedTextureImage3DEXT
#define glCompressedTextureImage2DEXT _glr->glExt.glCompressedTextureImage2DEXT
#define glCompressedTextureImage1DEXT _glr->glExt.glCompressedTextureImage1DEXT
#define glCompressedTextureSubImage3DEXT _glr->glExt.glCompressedTextureSubImage3DEXT
#define glCompressedTextureSubImage2DEXT _glr->glExt.glCompressedTextureSubImage2DEXT
#define glCompressedTextureSubImage1DEXT _glr->glExt.glCompressedTextureSubImage1DEXT
#define glGetCompressedTextureImageEXT _glr->glExt.glGetCompressedTextureImageEXT
#define glCompressedMultiTexImage3DEXT _glr->glExt.glCompressedMultiTexImage3DEXT
#define glCompressedMultiTexImage2DEXT _glr->glExt.glCompressedMultiTexImage2DEXT
#define glCompressedMultiTexImage1DEXT _glr->glExt.glCompressedMultiTexImage1DEXT
#define glCompressedMultiTexSubImage3DEXT _glr->glExt.glCompressedMultiTexSubImage3DEXT
#define glCompressedMultiTexSubImage2DEXT _glr->glExt.glCompressedMultiTexSubImage2DEXT
#define glCompressedMultiTexSubImage1DEXT _glr->glExt.glCompressedMultiTexSubImage1DEXT
#define glGetCompressedMultiTexImageEXT _glr->glExt.glGetCompressedMultiTexImageEXT
#define glMatrixLoadTransposefEXT _glr->glExt.glMatrixLoadTransposefEXT
#define glMatrixLoadTransposedEXT _glr->glExt.glMatrixLoadTransposedEXT
#define glMatrixMultTransposefEXT _glr->glExt.glMatrixMultTransposefEXT
#define glMatrixMultTransposedEXT _glr->glExt.glMatrixMultTransposedEXT
#define glNamedBufferDataEXT _glr->glExt.glNamedBufferDataEXT
#define glNamedBufferSubDataEXT _glr->glExt.glNamedBufferSubDataEXT
#define glMapNamedBufferEXT _glr->glExt.glMapNamedBufferEXT
#define glUnmapNamedBufferEXT _glr->glExt.glUnmapNamedBufferEXT
#define glGetNamedBufferParameterivEXT _glr->glExt.glGetNamedBufferParameterivEXT
#define glGetNamedBufferPointervEXT _glr->glExt.glGetNamedBufferPointervEXT
#define glGetNamedBufferSubDataEXT _glr->glExt.glGetNamedBufferSubDataEXT
#define glProgramUniform1fEXT _glr->glExt.glProgramUniform1fEXT
#define glProgramUniform2fEXT _glr->glExt.glProgramUniform2fEXT
#define glProgramUniform3fEXT _glr->glExt.glProgramUniform3fEXT
#define glProgramUniform4fEXT _glr->glExt.glProgramUniform4fEXT
#define glProgramUniform1iEXT _glr->glExt.glProgramUniform1iEXT
#define glProgramUniform2iEXT _glr->glExt.glProgramUniform2iEXT
#define glProgramUniform3iEXT _glr->glExt.glProgramUniform3iEXT
#define glProgramUniform4iEXT _glr->glExt.glProgramUniform4iEXT
#define glProgramUniform1fvEXT _glr->glExt.glProgramUniform1fvEXT
#define glProgramUniform2fvEXT _glr->glExt.glProgramUniform2fvEXT
#define glProgramUniform3fvEXT _glr->glExt.glProgramUniform3fvEXT
#define glProgramUniform4fvEXT _glr->glExt.glProgramUniform4fvEXT
#define glProgramUniform1ivEXT _glr->glExt.glProgramUniform1ivEXT
#define glProgramUniform2ivEXT _glr->glExt.glProgramUniform2ivEXT
#define glProgramUniform3ivEXT _glr->glExt.glProgramUniform3ivEXT
#define glProgramUniform4ivEXT _glr->glExt.glProgramUniform4ivEXT
#define glProgramUniformMatrix2fvEXT _glr->glExt.glProgramUniformMatrix2fvEXT
#define glProgramUniformMatrix3fvEXT _glr->glExt.glProgramUniformMatrix3fvEXT
#define glProgramUniformMatrix4fvEXT _glr->glExt.glProgramUniformMatrix4fvEXT
#define glProgramUniformMatrix2x3fvEXT _glr->glExt.glProgramUniformMatrix2x3fvEXT
#define glProgramUniformMatrix3x2fvEXT _glr->glExt.glProgramUniformMatrix3x2fvEXT
#define glProgramUniformMatrix2x4fvEXT _glr->glExt.glProgramUniformMatrix2x4fvEXT
#define glProgramUniformMatrix4x2fvEXT _glr->glExt.glProgramUniformMatrix4x2fvEXT
#define glProgramUniformMatrix3x4fvEXT _glr->glExt.glProgramUniformMatrix3x4fvEXT
#define glProgramUniformMatrix4x3fvEXT _glr->glExt.glProgramUniformMatrix4x3fvEXT
#define glTextureBufferEXT _glr->glExt.glTextureBufferEXT
#define glMultiTexBufferEXT _glr->glExt.glMultiTexBufferEXT
#define glTextureParameterIivEXT _glr->glExt.glTextureParameterIivEXT
#define glTextureParameterIuivEXT _glr->glExt.glTextureParameterIuivEXT
#define glGetTextureParameterIivEXT _glr->glExt.glGetTextureParameterIivEXT
#define glGetTextureParameterIuivEXT _glr->glExt.glGetTextureParameterIuivEXT
#define glMultiTexParameterIivEXT _glr->glExt.glMultiTexParameterIivEXT
#define glMultiTexParameterIuivEXT _glr->glExt.glMultiTexParameterIuivEXT
#define glGetMultiTexParameterIivEXT _glr->glExt.glGetMultiTexParameterIivEXT
#define glGetMultiTexParameterIuivEXT _glr->glExt.glGetMultiTexParameterIuivEXT
#define glProgramUniform1uiEXT _glr->glExt.glProgramUniform1uiEXT
#define glProgramUniform2uiEXT _glr->glExt.glProgramUniform2uiEXT
#define glProgramUniform3uiEXT _glr->glExt.glProgramUniform3uiEXT
#define glProgramUniform4uiEXT _glr->glExt.glProgramUniform4uiEXT
#define glProgramUniform1uivEXT _glr->glExt.glProgramUniform1uivEXT
#define glProgramUniform2uivEXT _glr->glExt.glProgramUniform2uivEXT
#define glProgramUniform3uivEXT _glr->glExt.glProgramUniform3uivEXT
#define glProgramUniform4uivEXT _glr->glExt.glProgramUniform4uivEXT
#define glNamedProgramLocalParameters4fvEXT _glr->glExt.glNamedProgramLocalParameters4fvEXT
#define glNamedProgramLocalParameterI4iEXT _glr->glExt.glNamedProgramLocalParameterI4iEXT
#define glNamedProgramLocalParameterI4ivEXT _glr->glExt.glNamedProgramLocalParameterI4ivEXT
#define glNamedProgramLocalParametersI4ivEXT _glr->glExt.glNamedProgramLocalParametersI4ivEXT
#define glNamedProgramLocalParameterI4uiEXT _glr->glExt.glNamedProgramLocalParameterI4uiEXT
#define glNamedProgramLocalParameterI4uivEXT _glr->glExt.glNamedProgramLocalParameterI4uivEXT
#define glNamedProgramLocalParametersI4uivEXT _glr->glExt.glNamedProgramLocalParametersI4uivEXT
#define glGetNamedProgramLocalParameterIivEXT _glr->glExt.glGetNamedProgramLocalParameterIivEXT
#define glGetNamedProgramLocalParameterIuivEXT _glr->glExt.glGetNamedProgramLocalParameterIuivEXT
#define glEnableClientStateiEXT _glr->glExt.glEnableClientStateiEXT
#define glDisableClientStateiEXT _glr->glExt.glDisableClientStateiEXT
#define glGetFloati_vEXT _glr->glExt.glGetFloati_vEXT
#define glGetDoublei_vEXT _glr->glExt.glGetDoublei_vEXT
#define glGetPointeri_vEXT _glr->glExt.glGetPointeri_vEXT
#define glNamedProgramStringEXT _glr->glExt.glNamedProgramStringEXT
#define glNamedProgramLocalParameter4dEXT _glr->glExt.glNamedProgramLocalParameter4dEXT
#define glNamedProgramLocalParameter4dvEXT _glr->glExt.glNamedProgramLocalParameter4dvEXT
#define glNamedProgramLocalParameter4fEXT _glr->glExt.glNamedProgramLocalParameter4fEXT
#define glNamedProgramLocalParameter4fvEXT _glr->glExt.glNamedProgramLocalParameter4fvEXT
#define glGetNamedProgramLocalParameterdvEXT _glr->glExt.glGetNamedProgramLocalParameterdvEXT
#define glGetNamedProgramLocalParameterfvEXT _glr->glExt.glGetNamedProgramLocalParameterfvEXT
#define glGetNamedProgramivEXT _glr->glExt.glGetNamedProgramivEXT
#define glGetNamedProgramStringEXT _glr->glExt.glGetNamedProgramStringEXT
#define glNamedRenderbufferStorageEXT _glr->glExt.glNamedRenderbufferStorageEXT
#define glGetNamedRenderbufferParameterivEXT _glr->glExt.glGetNamedRenderbufferParameterivEXT
#define glNamedRenderbufferStorageMultisampleEXT _glr->glExt.glNamedRenderbufferStorageMultisampleEXT
#define glNamedRenderbufferStorageMultisampleCoverageEXT _glr->glExt.glNamedRenderbufferStorageMultisampleCoverageEXT
#define glCheckNamedFramebufferStatusEXT _glr->glExt.glCheckNamedFramebufferStatusEXT
#define glNamedFramebufferTexture1DEXT _glr->glExt.glNamedFramebufferTexture1DEXT
#define glNamedFramebufferTexture2DEXT _glr->glExt.glNamedFramebufferTexture2DEXT
#define glNamedFramebufferTexture3DEXT _glr->glExt.glNamedFramebufferTexture3DEXT
#define glNamedFramebufferRenderbufferEXT _glr->glExt.glNamedFramebufferRenderbufferEXT
#define glGetNamedFramebufferAttachmentParameterivEXT _glr->glExt.glGetNamedFramebufferAttachmentParameterivEXT
#define glGenerateTextureMipmapEXT _glr->glExt.glGenerateTextureMipmapEXT
#define glGenerateMultiTexMipmapEXT _glr->glExt.glGenerateMultiTexMipmapEXT
#define glFramebufferDrawBufferEXT _glr->glExt.glFramebufferDrawBufferEXT
#define glFramebufferDrawBuffersEXT _glr->glExt.glFramebufferDrawBuffersEXT
#define glFramebufferReadBufferEXT _glr->glExt.glFramebufferReadBufferEXT
#define glGetFramebufferParameterivEXT _glr->glExt.glGetFramebufferParameterivEXT
#define glNamedCopyBufferSubDataEXT _glr->glExt.glNamedCopyBufferSubDataEXT
#define glNamedFramebufferTextureEXT _glr->glExt.glNamedFramebufferTextureEXT
#define glNamedFramebufferTextureLayerEXT _glr->glExt.glNamedFramebufferTextureLayerEXT
#define glNamedFramebufferTextureFaceEXT _glr->glExt.glNamedFramebufferTextureFaceEXT
#define glTextureRenderbufferEXT _glr->glExt.glTextureRenderbufferEXT
#define glMultiTexRenderbufferEXT _glr->glExt.glMultiTexRenderbufferEXT
#define glVertexArrayVertexOffsetEXT _glr->glExt.glVertexArrayVertexOffsetEXT
#define glVertexArrayColorOffsetEXT _glr->glExt.glVertexArrayColorOffsetEXT
#define glVertexArrayEdgeFlagOffsetEXT _glr->glExt.glVertexArrayEdgeFlagOffsetEXT
#define glVertexArrayIndexOffsetEXT _glr->glExt.glVertexArrayIndexOffsetEXT
#define glVertexArrayNormalOffsetEXT _glr->glExt.glVertexArrayNormalOffsetEXT
#define glVertexArrayTexCoordOffsetEXT _glr->glExt.glVertexArrayTexCoordOffsetEXT
#define glVertexArrayMultiTexCoordOffsetEXT _glr->glExt.glVertexArrayMultiTexCoordOffsetEXT
#define glVertexArrayFogCoordOffsetEXT _glr->glExt.glVertexArrayFogCoordOffsetEXT
#define glVertexArraySecondaryColorOffsetEXT _glr->glExt.glVertexArraySecondaryColorOffsetEXT
#define glVertexArrayVertexAttribOffsetEXT _glr->glExt.glVertexArrayVertexAttribOffsetEXT
#define glVertexArrayVertexAttribIOffsetEXT _glr->glExt.glVertexArrayVertexAttribIOffsetEXT
#define glEnableVertexArrayEXT _glr->glExt.glEnableVertexArrayEXT
#define glDisableVertexArrayEXT _glr->glExt.glDisableVertexArrayEXT
#define glEnableVertexArrayAttribEXT _glr->glExt.glEnableVertexArrayAttribEXT
#define glDisableVertexArrayAttribEXT _glr->glExt.glDisableVertexArrayAttribEXT
#define glGetVertexArrayIntegervEXT _glr->glExt.glGetVertexArrayIntegervEXT
#define glGetVertexArrayPointervEXT _glr->glExt.glGetVertexArrayPointervEXT
#define glGetVertexArrayIntegeri_vEXT _glr->glExt.glGetVertexArrayIntegeri_vEXT
#define glGetVertexArrayPointeri_vEXT _glr->glExt.glGetVertexArrayPointeri_vEXT
#define glMapNamedBufferRangeEXT _glr->glExt.glMapNamedBufferRangeEXT
#define glFlushMappedNamedBufferRangeEXT _glr->glExt.glFlushMappedNamedBufferRangeEXT
#define glNamedBufferStorageEXT _glr->glExt.glNamedBufferStorageEXT
#define glClearNamedBufferDataEXT _glr->glExt.glClearNamedBufferDataEXT
#define glClearNamedBufferSubDataEXT _glr->glExt.glClearNamedBufferSubDataEXT
#define glNamedFramebufferParameteriEXT _glr->glExt.glNamedFramebufferParameteriEXT
#define glGetNamedFramebufferParameterivEXT _glr->glExt.glGetNamedFramebufferParameterivEXT
#define glProgramUniform1dEXT _glr->glExt.glProgramUniform1dEXT
#define glProgramUniform2dEXT _glr->glExt.glProgramUniform2dEXT
#define glProgramUniform3dEXT _glr->glExt.glProgramUniform3dEXT
#define glProgramUniform4dEXT _glr->glExt.glProgramUniform4dEXT
#define glProgramUniform1dvEXT _glr->glExt.glProgramUniform1dvEXT
#define glProgramUniform2dvEXT _glr->glExt.glProgramUniform2dvEXT
#define glProgramUniform3dvEXT _glr->glExt.glProgramUniform3dvEXT
#define glProgramUniform4dvEXT _glr->glExt.glProgramUniform4dvEXT
#define glProgramUniformMatrix2dvEXT _glr->glExt.glProgramUniformMatrix2dvEXT
#define glProgramUniformMatrix3dvEXT _glr->glExt.glProgramUniformMatrix3dvEXT
#define glProgramUniformMatrix4dvEXT _glr->glExt.glProgramUniformMatrix4dvEXT
#define glProgramUniformMatrix2x3dvEXT _glr->glExt.glProgramUniformMatrix2x3dvEXT
#define glProgramUniformMatrix2x4dvEXT _glr->glExt.glProgramUniformMatrix2x4dvEXT
#define glProgramUniformMatrix3x2dvEXT _glr->glExt.glProgramUniformMatrix3x2dvEXT
#define glProgramUniformMatrix3x4dvEXT _glr->glExt.glProgramUniformMatrix3x4dvEXT
#define glProgramUniformMatrix4x2dvEXT _glr->glExt.glProgramUniformMatrix4x2dvEXT
#define glProgramUniformMatrix4x3dvEXT _glr->glExt.glProgramUniformMatrix4x3dvEXT
#define glTextureBufferRangeEXT _glr->glExt.glTextureBufferRangeEXT
#define glTextureStorage1DEXT _glr->glExt.glTextureStorage1DEXT
#define glTextureStorage2DEXT _glr->glExt.glTextureStorage2DEXT
#define glTextureStorage3DEXT _glr->glExt.glTextureStorage3DEXT
#define glTextureStorage2DMultisampleEXT _glr->glExt.glTextureStorage2DMultisampleEXT
#define glTextureStorage3DMultisampleEXT _glr->glExt.glTextureStorage3DMultisampleEXT
#define glVertexArrayBindVertexBufferEXT _glr->glExt.glVertexArrayBindVertexBufferEXT
#define glVertexArrayVertexAttribFormatEXT _glr->glExt.glVertexArrayVertexAttribFormatEXT
#define glVertexArrayVertexAttribIFormatEXT _glr->glExt.glVertexArrayVertexAttribIFormatEXT
#define glVertexArrayVertexAttribLFormatEXT _glr->glExt.glVertexArrayVertexAttribLFormatEXT
#define glVertexArrayVertexAttribBindingEXT _glr->glExt.glVertexArrayVertexAttribBindingEXT
#define glVertexArrayVertexBindingDivisorEXT _glr->glExt.glVertexArrayVertexBindingDivisorEXT
#define glVertexArrayVertexAttribLOffsetEXT _glr->glExt.glVertexArrayVertexAttribLOffsetEXT
#define glTexturePageCommitmentEXT _glr->glExt.glTexturePageCommitmentEXT
#define glVertexArrayVertexAttribDivisorEXT _glr->glExt.glVertexArrayVertexAttribDivisorEXT
// #340 GL_EXT_draw_buffers2
#define glColorMaskIndexedEXT _glr->glExt.glColorMaskIndexedEXT
// #327 GL_EXT_draw_instanced
#define glDrawArraysInstancedEXT _glr->glExt.glDrawArraysInstancedEXT
#define glDrawElementsInstancedEXT _glr->glExt.glDrawElementsInstancedEXT
// #112 GL_EXT_draw_range_elements
#define glDrawRangeElementsEXT _glr->glExt.glDrawRangeElementsEXT
// #149 GL_EXT_fog_coord
#define glFogCoordfEXT _glr->glExt.glFogCoordfEXT
#define glFogCoordfvEXT _glr->glExt.glFogCoordfvEXT
#define glFogCoorddEXT _glr->glExt.glFogCoorddEXT
#define glFogCoorddvEXT _glr->glExt.glFogCoorddvEXT
#define glFogCoordPointerEXT _glr->glExt.glFogCoordPointerEXT
// #316 GL_EXT_framebuffer_blit
#define glBlitFramebufferEXT _glr->glExt.glBlitFramebufferEXT
// #409 GL_EXT_framebuffer_multisample
#define glRenderbufferStorageMultisampleEXT _glr->glExt.glRenderbufferStorageMultisampleEXT
// #310 GL_EXT_framebuffer_object
#define glIsRenderbufferEXT _glr->glExt.glIsRenderbufferEXT
#define glBindRenderbufferEXT _glr->glExt.glBindRenderbufferEXT
#define glDeleteRenderbuffersEXT _glr->glExt.glDeleteRenderbuffersEXT
#define glGenRenderbuffersEXT _glr->glExt.glGenRenderbuffersEXT
#define glRenderbufferStorageEXT _glr->glExt.glRenderbufferStorageEXT
#define glGetRenderbufferParameterivEXT _glr->glExt.glGetRenderbufferParameterivEXT
#define glIsFramebufferEXT _glr->glExt.glIsFramebufferEXT
#define glBindFramebufferEXT _glr->glExt.glBindFramebufferEXT
#define glDeleteFramebuffersEXT _glr->glExt.glDeleteFramebuffersEXT
#define glGenFramebuffersEXT _glr->glExt.glGenFramebuffersEXT
#define glCheckFramebufferStatusEXT _glr->glExt.glCheckFramebufferStatusEXT
#define glFramebufferTexture1DEXT _glr->glExt.glFramebufferTexture1DEXT
#define glFramebufferTexture2DEXT _glr->glExt.glFramebufferTexture2DEXT
#define glFramebufferTexture3DEXT _glr->glExt.glFramebufferTexture3DEXT
#define glFramebufferRenderbufferEXT _glr->glExt.glFramebufferRenderbufferEXT
#define glGetFramebufferAttachmentParameterivEXT _glr->glExt.glGetFramebufferAttachmentParameterivEXT
#define glGenerateMipmapEXT _glr->glExt.glGenerateMipmapEXT
// #324 GL_EXT_geometry_shader4
#define glProgramParameteriEXT _glr->glExt.glProgramParameteriEXT
// #320 GL_EXT_gpu_program_parameters
#define glProgramEnvParameters4fvEXT _glr->glExt.glProgramEnvParameters4fvEXT
#define glProgramLocalParameters4fvEXT _glr->glExt.glProgramLocalParameters4fvEXT
// #326 GL_EXT_gpu_shader4
#define glGetUniformuivEXT _glr->glExt.glGetUniformuivEXT
#define glBindFragDataLocationEXT _glr->glExt.glBindFragDataLocationEXT
#define glGetFragDataLocationEXT _glr->glExt.glGetFragDataLocationEXT
#define glUniform1uiEXT _glr->glExt.glUniform1uiEXT
#define glUniform2uiEXT _glr->glExt.glUniform2uiEXT
#define glUniform3uiEXT _glr->glExt.glUniform3uiEXT
#define glUniform4uiEXT _glr->glExt.glUniform4uiEXT
#define glUniform1uivEXT _glr->glExt.glUniform1uivEXT
#define glUniform2uivEXT _glr->glExt.glUniform2uivEXT
#define glUniform3uivEXT _glr->glExt.glUniform3uivEXT
#define glUniform4uivEXT _glr->glExt.glUniform4uivEXT
// #011 GL_EXT_histogram
#define glGetHistogramParameterfvEXT _glr->glExt.glGetHistogramParameterfvEXT
#define glGetHistogramParameterivEXT _glr->glExt.glGetHistogramParameterivEXT
#define glGetMinmaxEXT _glr->glExt.glGetMinmaxEXT
#define glGetMinmaxParameterfvEXT _glr->glExt.glGetMinmaxParameterfvEXT
#define glGetMinmaxParameterivEXT _glr->glExt.glGetMinmaxParameterivEXT
#define glHistogramEXT _glr->glExt.glHistogramEXT
#define glMinmaxEXT _glr->glExt.glMinmaxEXT
#define glResetHistogramEXT _glr->glExt.glResetHistogramEXT
#define glResetMinmaxEXT _glr->glExt.glResetMinmaxEXT
// #095 GL_EXT_index_func
#define glIndexFuncEXT _glr->glExt.glIndexFuncEXT
// #094 GL_EXT_index_material
#define glIndexMaterialEXT _glr->glExt.glIndexMaterialEXT
// #117 GL_EXT_light_texture
#define glApplyTextureEXT _glr->glExt.glApplyTextureEXT
#define glTextureLightEXT _glr->glExt.glTextureLightEXT
#define glTextureMaterialEXT _glr->glExt.glTextureMaterialEXT
// #148 GL_EXT_multi_draw_arrays
#define glMultiDrawArraysEXT _glr->glExt.glMultiDrawArraysEXT
#define glMultiDrawElementsEXT _glr->glExt.glMultiDrawElementsEXT
// #209 GL_EXT_multisample
#define glSampleMaskEXT _glr->glExt.glSampleMaskEXT
#define glSamplePatternEXT _glr->glExt.glSamplePatternEXT
// #078 GL_EXT_paletted_texture
#define glColorTableEXT _glr->glExt.glColorTableEXT
#define glGetColorTableEXT _glr->glExt.glGetColorTableEXT
#define glGetColorTableParameterivEXT _glr->glExt.glGetColorTableParameterivEXT
#define glGetColorTableParameterfvEXT _glr->glExt.glGetColorTableParameterfvEXT
// #138 GL_EXT_pixel_transform
#define glPixelTransformParameteriEXT _glr->glExt.glPixelTransformParameteriEXT
#define glPixelTransformParameterfEXT _glr->glExt.glPixelTransformParameterfEXT
#define glPixelTransformParameterivEXT _glr->glExt.glPixelTransformParameterivEXT
#define glPixelTransformParameterfvEXT _glr->glExt.glPixelTransformParameterfvEXT
#define glGetPixelTransformParameterivEXT _glr->glExt.glGetPixelTransformParameterivEXT
#define glGetPixelTransformParameterfvEXT _glr->glExt.glGetPixelTransformParameterfvEXT
// #054 GL_EXT_point_parameters
#define glPointParameterfEXT _glr->glExt.glPointParameterfEXT
#define glPointParameterfvEXT _glr->glExt.glPointParameterfvEXT
// #003 GL_EXT_polygon_offset
#define glPolygonOffsetEXT _glr->glExt.glPolygonOffsetEXT
// #364 GL_EXT_provoking_vertex
#define glProvokingVertexEXT _glr->glExt.glProvokingVertexEXT
// #145 GL_EXT_secondary_color
#define glSecondaryColor3bEXT _glr->glExt.glSecondaryColor3bEXT
#define glSecondaryColor3bvEXT _glr->glExt.glSecondaryColor3bvEXT
#define glSecondaryColor3dEXT _glr->glExt.glSecondaryColor3dEXT
#define glSecondaryColor3dvEXT _glr->glExt.glSecondaryColor3dvEXT
#define glSecondaryColor3fEXT _glr->glExt.glSecondaryColor3fEXT
#define glSecondaryColor3fvEXT _glr->glExt.glSecondaryColor3fvEXT
#define glSecondaryColor3iEXT _glr->glExt.glSecondaryColor3iEXT
#define glSecondaryColor3ivEXT _glr->glExt.glSecondaryColor3ivEXT
#define glSecondaryColor3sEXT _glr->glExt.glSecondaryColor3sEXT
#define glSecondaryColor3svEXT _glr->glExt.glSecondaryColor3svEXT
#define glSecondaryColor3ubEXT _glr->glExt.glSecondaryColor3ubEXT
#define glSecondaryColor3ubvEXT _glr->glExt.glSecondaryColor3ubvEXT
#define glSecondaryColor3uiEXT _glr->glExt.glSecondaryColor3uiEXT
#define glSecondaryColor3uivEXT _glr->glExt.glSecondaryColor3uivEXT
#define glSecondaryColor3usEXT _glr->glExt.glSecondaryColor3usEXT
#define glSecondaryColor3usvEXT _glr->glExt.glSecondaryColor3usvEXT
#define glSecondaryColorPointerEXT _glr->glExt.glSecondaryColorPointerEXT
// #377 GL_EXT_separate_shader_objects
#define glUseShaderProgramEXT _glr->glExt.glUseShaderProgramEXT
#define glActiveProgramEXT _glr->glExt.glActiveProgramEXT
#define glCreateShaderProgramEXT _glr->glExt.glCreateShaderProgramEXT
// #386 GL_EXT_shader_image_load_store
#define glBindImageTextureEXT _glr->glExt.glBindImageTextureEXT
#define glMemoryBarrierEXT _glr->glExt.glMemoryBarrierEXT
// #314 GL_EXT_stencil_clear_tag
#define glStencilClearTagEXT _glr->glExt.glStencilClearTagEXT
// #268 GL_EXT_stencil_two_side
#define glActiveStencilFaceEXT _glr->glExt.glActiveStencilFaceEXT
// #009 GL_EXT_subtexture
#define glTexSubImage1DEXT _glr->glExt.glTexSubImage1DEXT
#define glTexSubImage2DEXT _glr->glExt.glTexSubImage2DEXT
// #006 GL_EXT_texture3D
#define glTexImage3DEXT _glr->glExt.glTexImage3DEXT
#define glTexSubImage3DEXT _glr->glExt.glTexSubImage3DEXT
// #330 GL_EXT_texture_buffer_object
#define glTexBufferEXT _glr->glExt.glTexBufferEXT
// #343 GL_EXT_texture_integer
#define glTexParameterIivEXT _glr->glExt.glTexParameterIivEXT
#define glTexParameterIuivEXT _glr->glExt.glTexParameterIuivEXT
#define glGetTexParameterIivEXT _glr->glExt.glGetTexParameterIivEXT
#define glGetTexParameterIuivEXT _glr->glExt.glGetTexParameterIuivEXT
#define glClearColorIiEXT _glr->glExt.glClearColorIiEXT
#define glClearColorIuiEXT _glr->glExt.glClearColorIuiEXT
// #020 GL_EXT_texture_object
#define glAreTexturesResidentEXT _glr->glExt.glAreTexturesResidentEXT
#define glBindTextureEXT _glr->glExt.glBindTextureEXT
#define glDeleteTexturesEXT _glr->glExt.glDeleteTexturesEXT
#define glGenTexturesEXT _glr->glExt.glGenTexturesEXT
#define glIsTextureEXT _glr->glExt.glIsTextureEXT
#define glPrioritizeTexturesEXT _glr->glExt.glPrioritizeTexturesEXT
// #147 GL_EXT_texture_perturb_normal
#define glTextureNormalEXT _glr->glExt.glTextureNormalEXT
// #319 GL_EXT_timer_query
#define glGetQueryObjecti64vEXT _glr->glExt.glGetQueryObjecti64vEXT
#define glGetQueryObjectui64vEXT _glr->glExt.glGetQueryObjectui64vEXT
// #352 GL_EXT_transform_feedback
#define glBeginTransformFeedbackEXT _glr->glExt.glBeginTransformFeedbackEXT
#define glEndTransformFeedbackEXT _glr->glExt.glEndTransformFeedbackEXT
#define glBindBufferRangeEXT _glr->glExt.glBindBufferRangeEXT
#define glBindBufferOffsetEXT _glr->glExt.glBindBufferOffsetEXT
#define glBindBufferBaseEXT _glr->glExt.glBindBufferBaseEXT
#define glTransformFeedbackVaryingsEXT _glr->glExt.glTransformFeedbackVaryingsEXT
#define glGetTransformFeedbackVaryingEXT _glr->glExt.glGetTransformFeedbackVaryingEXT
// #030 GL_EXT_vertex_array
#define glArrayElementEXT _glr->glExt.glArrayElementEXT
#define glColorPointerEXT _glr->glExt.glColorPointerEXT
#define glDrawArraysEXT _glr->glExt.glDrawArraysEXT
#define glEdgeFlagPointerEXT _glr->glExt.glEdgeFlagPointerEXT
#define glGetPointervEXT _glr->glExt.glGetPointervEXT
#define glIndexPointerEXT _glr->glExt.glIndexPointerEXT
#define glNormalPointerEXT _glr->glExt.glNormalPointerEXT
#define glTexCoordPointerEXT _glr->glExt.glTexCoordPointerEXT
#define glVertexPointerEXT _glr->glExt.glVertexPointerEXT
// #387 GL_EXT_vertex_attrib_64bit
#define glVertexAttribL1dEXT _glr->glExt.glVertexAttribL1dEXT
#define glVertexAttribL2dEXT _glr->glExt.glVertexAttribL2dEXT
#define glVertexAttribL3dEXT _glr->glExt.glVertexAttribL3dEXT
#define glVertexAttribL4dEXT _glr->glExt.glVertexAttribL4dEXT
#define glVertexAttribL1dvEXT _glr->glExt.glVertexAttribL1dvEXT
#define glVertexAttribL2dvEXT _glr->glExt.glVertexAttribL2dvEXT
#define glVertexAttribL3dvEXT _glr->glExt.glVertexAttribL3dvEXT
#define glVertexAttribL4dvEXT _glr->glExt.glVertexAttribL4dvEXT
#define glVertexAttribLPointerEXT _glr->glExt.glVertexAttribLPointerEXT
#define glGetVertexAttribLdvEXT _glr->glExt.glGetVertexAttribLdvEXT
// #248 GL_EXT_vertex_shader
#define glBeginVertexShaderEXT _glr->glExt.glBeginVertexShaderEXT
#define glEndVertexShaderEXT _glr->glExt.glEndVertexShaderEXT
#define glBindVertexShaderEXT _glr->glExt.glBindVertexShaderEXT
#define glGenVertexShadersEXT _glr->glExt.glGenVertexShadersEXT
#define glDeleteVertexShaderEXT _glr->glExt.glDeleteVertexShaderEXT
#define glShaderOp1EXT _glr->glExt.glShaderOp1EXT
#define glShaderOp2EXT _glr->glExt.glShaderOp2EXT
#define glShaderOp3EXT _glr->glExt.glShaderOp3EXT
#define glSwizzleEXT _glr->glExt.glSwizzleEXT
#define glWriteMaskEXT _glr->glExt.glWriteMaskEXT
#define glInsertComponentEXT _glr->glExt.glInsertComponentEXT
#define glExtractComponentEXT _glr->glExt.glExtractComponentEXT
#define glGenSymbolsEXT _glr->glExt.glGenSymbolsEXT
#define glSetInvariantEXT _glr->glExt.glSetInvariantEXT
#define glSetLocalConstantEXT _glr->glExt.glSetLocalConstantEXT
#define glVariantbvEXT _glr->glExt.glVariantbvEXT
#define glVariantsvEXT _glr->glExt.glVariantsvEXT
#define glVariantivEXT _glr->glExt.glVariantivEXT
#define glVariantfvEXT _glr->glExt.glVariantfvEXT
#define glVariantdvEXT _glr->glExt.glVariantdvEXT
#define glVariantubvEXT _glr->glExt.glVariantubvEXT
#define glVariantusvEXT _glr->glExt.glVariantusvEXT
#define glVariantuivEXT _glr->glExt.glVariantuivEXT
#define glVariantPointerEXT _glr->glExt.glVariantPointerEXT
#define glEnableVariantClientStateEXT _glr->glExt.glEnableVariantClientStateEXT
#define glDisableVariantClientStateEXT _glr->glExt.glDisableVariantClientStateEXT
#define glBindLightParameterEXT _glr->glExt.glBindLightParameterEXT
#define glBindMaterialParameterEXT _glr->glExt.glBindMaterialParameterEXT
#define glBindTexGenParameterEXT _glr->glExt.glBindTexGenParameterEXT
#define glBindTextureUnitParameterEXT _glr->glExt.glBindTextureUnitParameterEXT
#define glBindParameterEXT _glr->glExt.glBindParameterEXT
#define glIsVariantEnabledEXT _glr->glExt.glIsVariantEnabledEXT
#define glGetVariantBooleanvEXT _glr->glExt.glGetVariantBooleanvEXT
#define glGetVariantIntegervEXT _glr->glExt.glGetVariantIntegervEXT
#define glGetVariantFloatvEXT _glr->glExt.glGetVariantFloatvEXT
#define glGetVariantPointervEXT _glr->glExt.glGetVariantPointervEXT
#define glGetInvariantBooleanvEXT _glr->glExt.glGetInvariantBooleanvEXT
#define glGetInvariantIntegervEXT _glr->glExt.glGetInvariantIntegervEXT
#define glGetInvariantFloatvEXT _glr->glExt.glGetInvariantFloatvEXT
#define glGetLocalConstantBooleanvEXT _glr->glExt.glGetLocalConstantBooleanvEXT
#define glGetLocalConstantIntegervEXT _glr->glExt.glGetLocalConstantIntegervEXT
#define glGetLocalConstantFloatvEXT _glr->glExt.glGetLocalConstantFloatvEXT
// #188 GL_EXT_vertex_weighting
#define glVertexWeightfEXT _glr->glExt.glVertexWeightfEXT
#define glVertexWeightfvEXT _glr->glExt.glVertexWeightfvEXT
#define glVertexWeightPointerEXT _glr->glExt.glVertexWeightPointerEXT
// #406 GL_EXT_x11_sync_object
#define glImportSyncEXT _glr->glExt.glImportSyncEXT
// #345 GL_GREMEDY_frame_terminator
#define glFrameTerminatorGREMEDY _glr->glExt.glFrameTerminatorGREMEDY
// #311 GL_GREMEDY_string_marker
#define glStringMarkerGREMEDY _glr->glExt.glStringMarkerGREMEDY
// #066 GL_HP_image_transform
#define glImageTransformParameteriHP _glr->glExt.glImageTransformParameteriHP
#define glImageTransformParameterfHP _glr->glExt.glImageTransformParameterfHP
#define glImageTransformParameterivHP _glr->glExt.glImageTransformParameterivHP
#define glImageTransformParameterfvHP _glr->glExt.glImageTransformParameterfvHP
#define glGetImageTransformParameterivHP _glr->glExt.glGetImageTransformParameterivHP
#define glGetImageTransformParameterfvHP _glr->glExt.glGetImageTransformParameterfvHP
// #200 GL_IBM_multimode_draw_arrays
#define glMultiModeDrawArraysIBM _glr->glExt.glMultiModeDrawArraysIBM
#define glMultiModeDrawElementsIBM _glr->glExt.glMultiModeDrawElementsIBM
// #223 GL_IBM_static_data
#define glFlushStaticDataIBM _glr->glExt.glFlushStaticDataIBM
// #201 GL_IBM_vertex_array_lists
#define glColorPointerListIBM _glr->glExt.glColorPointerListIBM
#define glSecondaryColorPointerListIBM _glr->glExt.glSecondaryColorPointerListIBM
#define glEdgeFlagPointerListIBM _glr->glExt.glEdgeFlagPointerListIBM
#define glFogCoordPointerListIBM _glr->glExt.glFogCoordPointerListIBM
#define glIndexPointerListIBM _glr->glExt.glIndexPointerListIBM
#define glNormalPointerListIBM _glr->glExt.glNormalPointerListIBM
#define glTexCoordPointerListIBM _glr->glExt.glTexCoordPointerListIBM
#define glVertexPointerListIBM _glr->glExt.glVertexPointerListIBM
// GL_INGR_blend_func_separate - not in any list
#define glBlendFuncSeparateINGR _glr->glExt.glBlendFuncSeparateINGR
// #429 GL_INTEL_map_texture
#define glSyncTextureINTEL _glr->glExt.glSyncTextureINTEL
#define glUnmapTexture2DINTEL _glr->glExt.glUnmapTexture2DINTEL
#define glMapTexture2DINTEL _glr->glExt.glMapTexture2DINTEL
// #136 GL_INTEL_parallel_arrays
#define glVertexPointervINTEL _glr->glExt.glVertexPointervINTEL
#define glNormalPointervINTEL _glr->glExt.glNormalPointervINTEL
#define glColorPointervINTEL _glr->glExt.glColorPointervINTEL
#define glTexCoordPointervINTEL _glr->glExt.glTexCoordPointervINTEL
// #443 GL_INTEL_performance_query
#define glBeginPerfQueryINTEL _glr->glExt.glBeginPerfQueryINTEL
#define glCreatePerfQueryINTEL _glr->glExt.glCreatePerfQueryINTEL
#define glDeletePerfQueryINTEL _glr->glExt.glDeletePerfQueryINTEL
#define glEndPerfQueryINTEL _glr->glExt.glEndPerfQueryINTEL
#define glGetFirstPerfQueryIdINTEL _glr->glExt.glGetFirstPerfQueryIdINTEL
#define glGetNextPerfQueryIdINTEL _glr->glExt.glGetNextPerfQueryIdINTEL
#define glGetPerfCounterInfoINTEL _glr->glExt.glGetPerfCounterInfoINTEL
#define glGetPerfQueryDataINTEL _glr->glExt.glGetPerfQueryDataINTEL
#define glGetPerfQueryIdByNameINTEL _glr->glExt.glGetPerfQueryIdByNameINTEL
#define glGetPerfQueryInfoINTEL _glr->glExt.glGetPerfQueryInfoINTEL
// #196 GL_MESA_resize_buffers
#define glResizeBuffersMESA _glr->glExt.glResizeBuffersMESA
// #197 GL_MESA_window_pos
#define glWindowPos2dMESA _glr->glExt.glWindowPos2dMESA
#define glWindowPos2dvMESA _glr->glExt.glWindowPos2dvMESA
#define glWindowPos2fMESA _glr->glExt.glWindowPos2fMESA
#define glWindowPos2fvMESA _glr->glExt.glWindowPos2fvMESA
#define glWindowPos2iMESA _glr->glExt.glWindowPos2iMESA
#define glWindowPos2ivMESA _glr->glExt.glWindowPos2ivMESA
#define glWindowPos2sMESA _glr->glExt.glWindowPos2sMESA
#define glWindowPos2svMESA _glr->glExt.glWindowPos2svMESA
#define glWindowPos3dMESA _glr->glExt.glWindowPos3dMESA
#define glWindowPos3dvMESA _glr->glExt.glWindowPos3dvMESA
#define glWindowPos3fMESA _glr->glExt.glWindowPos3fMESA
#define glWindowPos3fvMESA _glr->glExt.glWindowPos3fvMESA
#define glWindowPos3iMESA _glr->glExt.glWindowPos3iMESA
#define glWindowPos3ivMESA _glr->glExt.glWindowPos3ivMESA
#define glWindowPos3sMESA _glr->glExt.glWindowPos3sMESA
#define glWindowPos3svMESA _glr->glExt.glWindowPos3svMESA
#define glWindowPos4dMESA _glr->glExt.glWindowPos4dMESA
#define glWindowPos4dvMESA _glr->glExt.glWindowPos4dvMESA
#define glWindowPos4fMESA _glr->glExt.glWindowPos4fMESA
#define glWindowPos4fvMESA _glr->glExt.glWindowPos4fvMESA
#define glWindowPos4iMESA _glr->glExt.glWindowPos4iMESA
#define glWindowPos4ivMESA _glr->glExt.glWindowPos4ivMESA
#define glWindowPos4sMESA _glr->glExt.glWindowPos4sMESA
#define glWindowPos4svMESA _glr->glExt.glWindowPos4svMESA
// #425 GL_NVX_conditional_render
#define glBeginConditionalRenderNVX _glr->glExt.glBeginConditionalRenderNVX
#define glEndConditionalRenderNVX _glr->glExt.glEndConditionalRenderNVX
// #432 GL_NV_bindless_multi_draw_indirect
#define glMultiDrawArraysIndirectBindlessNV _glr->glExt.glMultiDrawArraysIndirectBindlessNV
#define glMultiDrawElementsIndirectBindlessNV _glr->glExt.glMultiDrawElementsIndirectBindlessNV
// #418 GL_NV_bindless_texture
#define glGetTextureHandleNV _glr->glExt.glGetTextureHandleNV
#define glGetTextureSamplerHandleNV _glr->glExt.glGetTextureSamplerHandleNV
#define glMakeTextureHandleResidentNV _glr->glExt.glMakeTextureHandleResidentNV
#define glMakeTextureHandleNonResidentNV _glr->glExt.glMakeTextureHandleNonResidentNV
#define glGetImageHandleNV _glr->glExt.glGetImageHandleNV
#define glMakeImageHandleResidentNV _glr->glExt.glMakeImageHandleResidentNV
#define glMakeImageHandleNonResidentNV _glr->glExt.glMakeImageHandleNonResidentNV
#define glUniformHandleui64NV _glr->glExt.glUniformHandleui64NV
#define glUniformHandleui64vNV _glr->glExt.glUniformHandleui64vNV
#define glProgramUniformHandleui64NV _glr->glExt.glProgramUniformHandleui64NV
#define glProgramUniformHandleui64vNV _glr->glExt.glProgramUniformHandleui64vNV
#define glIsTextureHandleResidentNV _glr->glExt.glIsTextureHandleResidentNV
#define glIsImageHandleResidentNV _glr->glExt.glIsImageHandleResidentNV
// #433 GL_NV_blend_equation_advanced
#define glBlendParameteriNV _glr->glExt.glBlendParameteriNV
#define glBlendBarrierNV _glr->glExt.glBlendBarrierNV
// #346 GL_NV_conditional_render
#define glBeginConditionalRenderNV _glr->glExt.glBeginConditionalRenderNV
#define glEndConditionalRenderNV _glr->glExt.glEndConditionalRenderNV
// #376 GL_NV_copy_image WGL_NV_copy_image GLX_NV_copy_image
#define glCopyImageSubDataNV _glr->glExt.glCopyImageSubDataNV
#ifdef OS_LINUX
#define glXCopyImageSubDataNV _glr->glExt. glXCopyImageSubDataNV
#endif /// OS_LINUX
#ifdef OS_WIN
#define wglCopyImageSubDataNV _glr->glExt.wglCopyImageSubDataNV
#endif /// OS_WIN
// #334 GL_NV_depth_buffer_float
#define glDepthRangedNV _glr->glExt.glDepthRangedNV
#define glClearDepthdNV _glr->glExt.glClearDepthdNV
#define glDepthBoundsdNV _glr->glExt.glDepthBoundsdNV
// #430 GL_NV_draw_texture
#define glDrawTextureNV _glr->glExt.glDrawTextureNV
// #225 GL_NV_evaluators
#define glMapControlPointsNV _glr->glExt.glMapControlPointsNV
#define glMapParameterivNV _glr->glExt.glMapParameterivNV
#define glMapParameterfvNV _glr->glExt.glMapParameterfvNV
#define glGetMapControlPointsNV _glr->glExt.glGetMapControlPointsNV
#define glGetMapParameterivNV _glr->glExt.glGetMapParameterivNV
#define glGetMapParameterfvNV _glr->glExt.glGetMapParameterfvNV
#define glGetMapAttribParameterivNV _glr->glExt.glGetMapAttribParameterivNV
#define glGetMapAttribParameterfvNV _glr->glExt.glGetMapAttribParameterfvNV
#define glEvalMapsNV _glr->glExt.glEvalMapsNV
// #357 GL_NV_explicit_multisample
#define glGetMultisamplefvNV _glr->glExt.glGetMultisamplefvNV
#define glSampleMaskIndexedNV _glr->glExt.glSampleMaskIndexedNV
#define glTexRenderbufferNV _glr->glExt.glTexRenderbufferNV
// #222 GL_NV_fence
#define glDeleteFencesNV _glr->glExt.glDeleteFencesNV
#define glGenFencesNV _glr->glExt.glGenFencesNV
#define glIsFenceNV _glr->glExt.glIsFenceNV
#define glTestFenceNV _glr->glExt.glTestFenceNV
#define glGetFenceivNV _glr->glExt.glGetFenceivNV
#define glFinishFenceNV _glr->glExt.glFinishFenceNV
#define glSetFenceNV _glr->glExt.glSetFenceNV
// #282 GL_NV_fragment_program
#define glProgramNamedParameter4fNV _glr->glExt.glProgramNamedParameter4fNV
#define glProgramNamedParameter4fvNV _glr->glExt.glProgramNamedParameter4fvNV
#define glProgramNamedParameter4dNV _glr->glExt.glProgramNamedParameter4dNV
#define glProgramNamedParameter4dvNV _glr->glExt.glProgramNamedParameter4dvNV
#define glGetProgramNamedParameterfvNV _glr->glExt.glGetProgramNamedParameterfvNV
#define glGetProgramNamedParameterdvNV _glr->glExt.glGetProgramNamedParameterdvNV
// #336 GL_NV_framebuffer_multisample_coverage
#define glRenderbufferStorageMultisampleCoverageNV _glr->glExt.glRenderbufferStorageMultisampleCoverageNV
// #323 GL_NV_geometry_program4
#define glProgramVertexLimitNV _glr->glExt.glProgramVertexLimitNV
#define glFramebufferTextureEXT _glr->glExt.glFramebufferTextureEXT
#define glFramebufferTextureLayerEXT _glr->glExt.glFramebufferTextureLayerEXT
#define glFramebufferTextureFaceEXT _glr->glExt.glFramebufferTextureFaceEXT
// #322 GL_NV_gpu_program4
#define glProgramLocalParameterI4iNV _glr->glExt.glProgramLocalParameterI4iNV
#define glProgramLocalParameterI4ivNV _glr->glExt.glProgramLocalParameterI4ivNV
#define glProgramLocalParametersI4ivNV _glr->glExt.glProgramLocalParametersI4ivNV
#define glProgramLocalParameterI4uiNV _glr->glExt.glProgramLocalParameterI4uiNV
#define glProgramLocalParameterI4uivNV _glr->glExt.glProgramLocalParameterI4uivNV
#define glProgramLocalParametersI4uivNV _glr->glExt.glProgramLocalParametersI4uivNV
#define glProgramEnvParameterI4iNV _glr->glExt.glProgramEnvParameterI4iNV
#define glProgramEnvParameterI4ivNV _glr->glExt.glProgramEnvParameterI4ivNV
#define glProgramEnvParametersI4ivNV _glr->glExt.glProgramEnvParametersI4ivNV
#define glProgramEnvParameterI4uiNV _glr->glExt.glProgramEnvParameterI4uiNV
#define glProgramEnvParameterI4uivNV _glr->glExt.glProgramEnvParameterI4uivNV
#define glProgramEnvParametersI4uivNV _glr->glExt.glProgramEnvParametersI4uivNV
#define glGetProgramLocalParameterIivNV _glr->glExt.glGetProgramLocalParameterIivNV
#define glGetProgramLocalParameterIuivNV _glr->glExt.glGetProgramLocalParameterIuivNV
#define glGetProgramEnvParameterIivNV _glr->glExt.glGetProgramEnvParameterIivNV
#define glGetProgramEnvParameterIuivNV _glr->glExt.glGetProgramEnvParameterIuivNV
// #388 GL_NV_gpu_program5
#define glProgramSubroutineParametersuivNV _glr->glExt.glProgramSubroutineParametersuivNV
#define glGetProgramSubroutineParameteruivNV _glr->glExt.glGetProgramSubroutineParameteruivNV
// #283 GL_NV_half_float
#define glVertex2hNV _glr->glExt.glVertex2hNV
#define glVertex2hvNV _glr->glExt.glVertex2hvNV
#define glVertex3hNV _glr->glExt.glVertex3hNV
#define glVertex3hvNV _glr->glExt.glVertex3hvNV
#define glVertex4hNV _glr->glExt.glVertex4hNV
#define glVertex4hvNV _glr->glExt.glVertex4hvNV
#define glNormal3hNV _glr->glExt.glNormal3hNV
#define glNormal3hvNV _glr->glExt.glNormal3hvNV
#define glColor3hNV _glr->glExt.glColor3hNV
#define glColor3hvNV _glr->glExt.glColor3hvNV
#define glColor4hNV _glr->glExt.glColor4hNV
#define glColor4hvNV _glr->glExt.glColor4hvNV
#define glTexCoord1hNV _glr->glExt.glTexCoord1hNV
#define glTexCoord1hvNV _glr->glExt.glTexCoord1hvNV
#define glTexCoord2hNV _glr->glExt.glTexCoord2hNV
#define glTexCoord2hvNV _glr->glExt.glTexCoord2hvNV
#define glTexCoord3hNV _glr->glExt.glTexCoord3hNV
#define glTexCoord3hvNV _glr->glExt.glTexCoord3hvNV
#define glTexCoord4hNV _glr->glExt.glTexCoord4hNV
#define glTexCoord4hvNV _glr->glExt.glTexCoord4hvNV
#define glMultiTexCoord1hNV _glr->glExt.glMultiTexCoord1hNV
#define glMultiTexCoord1hvNV _glr->glExt.glMultiTexCoord1hvNV
#define glMultiTexCoord2hNV _glr->glExt.glMultiTexCoord2hNV
#define glMultiTexCoord2hvNV _glr->glExt.glMultiTexCoord2hvNV
#define glMultiTexCoord3hNV _glr->glExt.glMultiTexCoord3hNV
#define glMultiTexCoord3hvNV _glr->glExt.glMultiTexCoord3hvNV
#define glMultiTexCoord4hNV _glr->glExt.glMultiTexCoord4hNV
#define glMultiTexCoord4hvNV _glr->glExt.glMultiTexCoord4hvNV
#define glFogCoordhNV _glr->glExt.glFogCoordhNV
#define glFogCoordhvNV _glr->glExt.glFogCoordhvNV
#define glSecondaryColor3hNV _glr->glExt.glSecondaryColor3hNV
#define glSecondaryColor3hvNV _glr->glExt.glSecondaryColor3hvNV
#define glVertexWeighthNV _glr->glExt.glVertexWeighthNV
#define glVertexWeighthvNV _glr->glExt.glVertexWeighthvNV
#define glVertexAttrib1hNV _glr->glExt.glVertexAttrib1hNV
#define glVertexAttrib1hvNV _glr->glExt.glVertexAttrib1hvNV
#define glVertexAttrib2hNV _glr->glExt.glVertexAttrib2hNV
#define glVertexAttrib2hvNV _glr->glExt.glVertexAttrib2hvNV
#define glVertexAttrib3hNV _glr->glExt.glVertexAttrib3hNV
#define glVertexAttrib3hvNV _glr->glExt.glVertexAttrib3hvNV
#define glVertexAttrib4hNV _glr->glExt.glVertexAttrib4hNV
#define glVertexAttrib4hvNV _glr->glExt.glVertexAttrib4hvNV
#define glVertexAttribs1hvNV _glr->glExt.glVertexAttribs1hvNV
#define glVertexAttribs2hvNV _glr->glExt.glVertexAttribs2hvNV
#define glVertexAttribs3hvNV _glr->glExt.glVertexAttribs3hvNV
#define glVertexAttribs4hvNV _glr->glExt.glVertexAttribs4hvNV
// #261 GL_NV_occlusion_query
#define glGenOcclusionQueriesNV _glr->glExt.glGenOcclusionQueriesNV
#define glDeleteOcclusionQueriesNV _glr->glExt.glDeleteOcclusionQueriesNV
#define glIsOcclusionQueryNV _glr->glExt.glIsOcclusionQueryNV
#define glBeginOcclusionQueryNV _glr->glExt.glBeginOcclusionQueryNV
#define glEndOcclusionQueryNV _glr->glExt.glEndOcclusionQueryNV
#define glGetOcclusionQueryivNV _glr->glExt.glGetOcclusionQueryivNV
#define glGetOcclusionQueryuivNV _glr->glExt.glGetOcclusionQueryuivNV
// #339 GL_NV_parameter_buffer_object
#define glProgramBufferParametersfvNV _glr->glExt.glProgramBufferParametersfvNV
#define glProgramBufferParametersIivNV _glr->glExt.glProgramBufferParametersIivNV
#define glProgramBufferParametersIuivNV _glr->glExt.glProgramBufferParametersIuivNV
// #410 GL_NV_path_rendering
#define glGenPathsNV _glr->glExt.glGenPathsNV
#define glDeletePathsNV _glr->glExt.glDeletePathsNV
#define glIsPathNV _glr->glExt.glIsPathNV
#define glPathCommandsNV _glr->glExt.glPathCommandsNV
#define glPathCoordsNV _glr->glExt.glPathCoordsNV
#define glPathSubCommandsNV _glr->glExt.glPathSubCommandsNV
#define glPathSubCoordsNV _glr->glExt.glPathSubCoordsNV
#define glPathStringNV _glr->glExt.glPathStringNV
#define glPathGlyphsNV _glr->glExt.glPathGlyphsNV
#define glPathGlyphRangeNV _glr->glExt.glPathGlyphRangeNV
#define glWeightPathsNV _glr->glExt.glWeightPathsNV
#define glCopyPathNV _glr->glExt.glCopyPathNV
#define glInterpolatePathsNV _glr->glExt.glInterpolatePathsNV
#define glTransformPathNV _glr->glExt.glTransformPathNV
#define glPathParameterivNV _glr->glExt.glPathParameterivNV
#define glPathParameteriNV _glr->glExt.glPathParameteriNV
#define glPathParameterfvNV _glr->glExt.glPathParameterfvNV
#define glPathParameterfNV _glr->glExt.glPathParameterfNV
#define glPathDashArrayNV _glr->glExt.glPathDashArrayNV
#define glPathStencilFuncNV _glr->glExt.glPathStencilFuncNV
#define glPathStencilDepthOffsetNV _glr->glExt.glPathStencilDepthOffsetNV
#define glStencilFillPathNV _glr->glExt.glStencilFillPathNV
#define glStencilStrokePathNV _glr->glExt.glStencilStrokePathNV
#define glStencilFillPathInstancedNV _glr->glExt.glStencilFillPathInstancedNV
#define glStencilStrokePathInstancedNV _glr->glExt.glStencilStrokePathInstancedNV
#define glPathCoverDepthFuncNV _glr->glExt.glPathCoverDepthFuncNV
#define glPathColorGenNV _glr->glExt.glPathColorGenNV
#define glPathTexGenNV _glr->glExt.glPathTexGenNV
#define glPathFogGenNV _glr->glExt.glPathFogGenNV
#define glCoverFillPathNV _glr->glExt.glCoverFillPathNV
#define glCoverStrokePathNV _glr->glExt.glCoverStrokePathNV
#define glCoverFillPathInstancedNV _glr->glExt.glCoverFillPathInstancedNV
#define glCoverStrokePathInstancedNV _glr->glExt.glCoverStrokePathInstancedNV
#define glGetPathParameterivNV _glr->glExt.glGetPathParameterivNV
#define glGetPathParameterfvNV _glr->glExt.glGetPathParameterfvNV
#define glGetPathCommandsNV _glr->glExt.glGetPathCommandsNV
#define glGetPathCoordsNV _glr->glExt.glGetPathCoordsNV
#define glGetPathDashArrayNV _glr->glExt.glGetPathDashArrayNV
#define glGetPathMetricsNV _glr->glExt.glGetPathMetricsNV
#define glGetPathMetricRangeNV _glr->glExt.glGetPathMetricRangeNV
#define glGetPathSpacingNV _glr->glExt.glGetPathSpacingNV
#define glGetPathColorGenivNV _glr->glExt.glGetPathColorGenivNV
#define glGetPathColorGenfvNV _glr->glExt.glGetPathColorGenfvNV
#define glGetPathTexGenivNV _glr->glExt.glGetPathTexGenivNV
#define glGetPathTexGenfvNV _glr->glExt.glGetPathTexGenfvNV
#define glIsPointInFillPathNV _glr->glExt.glIsPointInFillPathNV
#define glIsPointInStrokePathNV _glr->glExt.glIsPointInStrokePathNV
#define glGetPathLengthNV _glr->glExt.glGetPathLengthNV
#define glPointAlongPathNV _glr->glExt.glPointAlongPathNV
// #284 GL_NV_pixel_data_range
#define glPixelDataRangeNV _glr->glExt.glPixelDataRangeNV
#define glFlushPixelDataRangeNV _glr->glExt.glFlushPixelDataRangeNV
// #262 GL_NV_point_sprite
#define glPointParameteriNV _glr->glExt.glPointParameteriNV
#define glPointParameterivNV _glr->glExt.glPointParameterivNV
// #347 GL_NV_present_video GLX_NV_present_video WGL_NV_present_video
#define glPresentFrameKeyedNV _glr->glExt.glPresentFrameKeyedNV
#define glPresentFrameDualFillNV _glr->glExt.glPresentFrameDualFillNV
#define glGetVideoivNV _glr->glExt.glGetVideoivNV
#define glGetVideouivNV _glr->glExt.glGetVideouivNV
#define glGetVideoi64vNV _glr->glExt.glGetVideoi64vNV
#define glGetVideoui64vNV _glr->glExt.glGetVideoui64vNV
#ifdef OS_WIN
#define wglGetPixelFormatAttribivARB _glr->glExt.wglGetPixelFormatAttribivARB
#define wglGetPixelFormatAttribfvARB _glr->glExt.wglGetPixelFormatAttribfvARB
#define wglChoosePixelFormatARB _glr->glExt.wglChoosePixelFormatARB
#endif /// OS_WIN
#ifdef OS_LINUX
#define glXEnumerateVideoDevicesNV _glr->glExt.glXEnumerateVideoDevicesNV
#define glXBindVideoDeviceNV _glr->glExt.glXBindVideoDeviceNV
#endif /// OS_LINUX
// #285 GL_NV_primitive_restart
#define glPrimitiveRestartNV _glr->glExt.glPrimitiveRestartNV
#define glPrimitiveRestartIndexNV _glr->glExt.glPrimitiveRestartIndexNV
// #191 GL_NV_register_combiners
#define glCombinerParameterfvNV _glr->glExt.glCombinerParameterfvNV
#define glCombinerParameterfNV _glr->glExt.glCombinerParameterfNV
#define glCombinerParameterivNV _glr->glExt.glCombinerParameterivNV
#define glCombinerParameteriNV _glr->glExt.glCombinerParameteriNV
#define glCombinerInputNV _glr->glExt.glCombinerInputNV
#define glCombinerOutputNV _glr->glExt.glCombinerOutputNV
#define glFinalCombinerInputNV _glr->glExt.glFinalCombinerInputNV
#define glGetCombinerInputParameterfvNV _glr->glExt.glGetCombinerInputParameterfvNV
#define glGetCombinerInputParameterivNV _glr->glExt.glGetCombinerInputParameterivNV
#define glGetCombinerOutputParameterfvNV _glr->glExt.glGetCombinerOutputParameterfvNV
#define glGetCombinerOutputParameterivNV _glr->glExt.glGetCombinerOutputParameterivNV
#define glGetFinalCombinerInputParameterfvNV _glr->glExt.glGetFinalCombinerInputParameterfvNV
#define glGetFinalCombinerInputParameterivNV _glr->glExt.glGetFinalCombinerInputParameterivNV
// #227 GL_NV_register_combiners2
#define glCombinerStageParameterfvNV _glr->glExt.glCombinerStageParameterfvNV
#define glGetCombinerStageParameterfvNV _glr->glExt.glGetCombinerStageParameterfvNV
// #379 GL_NV_shader_buffer_load
#define glMakeBufferResidentNV _glr->glExt.glMakeBufferResidentNV
#define glMakeBufferNonResidentNV _glr->glExt.glMakeBufferNonResidentNV
#define glIsBufferResidentNV _glr->glExt.glIsBufferResidentNV
#define glMakeNamedBufferResidentNV _glr->glExt.glMakeNamedBufferResidentNV
#define glMakeNamedBufferNonResidentNV _glr->glExt.glMakeNamedBufferNonResidentNV
#define glIsNamedBufferResidentNV _glr->glExt.glIsNamedBufferResidentNV
#define glGetBufferParameterui64vNV _glr->glExt.glGetBufferParameterui64vNV
#define glGetNamedBufferParameterui64vNV _glr->glExt.glGetNamedBufferParameterui64vNV
#define glGetIntegerui64vNV _glr->glExt.glGetIntegerui64vNV
#define glUniformui64NV _glr->glExt.glUniformui64NV
#define glUniformui64vNV _glr->glExt.glUniformui64vNV
#define glProgramUniformui64NV _glr->glExt.glProgramUniformui64NV
#define glProgramUniformui64vNV _glr->glExt.glProgramUniformui64vNV
// #381 GL_NV_texture_barrier
#define glTextureBarrierNV _glr->glExt.glTextureBarrierNV
// #403 GL_NV_texture_multisample
#define glTexImage2DMultisampleCoverageNV _glr->glExt.glTexImage2DMultisampleCoverageNV
#define glTexImage3DMultisampleCoverageNV _glr->glExt.glTexImage3DMultisampleCoverageNV
#define glTextureImage2DMultisampleNV _glr->glExt.glTextureImage2DMultisampleNV
#define glTextureImage3DMultisampleNV _glr->glExt.glTextureImage3DMultisampleNV
#define glTextureImage2DMultisampleCoverageNV _glr->glExt.glTextureImage2DMultisampleCoverageNV
#define glTextureImage3DMultisampleCoverageNV _glr->glExt.glTextureImage3DMultisampleCoverageNV
// #341 GL_NV_transform_feedback
#define glBeginTransformFeedbackNV _glr->glExt.glBeginTransformFeedbackNV
#define glEndTransformFeedbackNV _glr->glExt.glEndTransformFeedbackNV
#define glTransformFeedbackAttribsNV _glr->glExt.glTransformFeedbackAttribsNV
#define glBindBufferRangeNV _glr->glExt.glBindBufferRangeNV
#define glBindBufferOffsetNV _glr->glExt.glBindBufferOffsetNV
#define glBindBufferBaseNV _glr->glExt.glBindBufferBaseNV
#define glTransformFeedbackVaryingsNV _glr->glExt.glTransformFeedbackVaryingsNV
#define glActiveVaryingNV _glr->glExt.glActiveVaryingNV
#define glGetVaryingLocationNV _glr->glExt.glGetVaryingLocationNV
#define glGetActiveVaryingNV _glr->glExt.glGetActiveVaryingNV
#define glGetTransformFeedbackVaryingNV _glr->glExt.glGetTransformFeedbackVaryingNV
#define glTransformFeedbackStreamAttribsNV _glr->glExt.glTransformFeedbackStreamAttribsNV
// #358 GL_NV_transform_feedback2
#define glBindTransformFeedbackNV _glr->glExt.glBindTransformFeedbackNV
#define glDeleteTransformFeedbacksNV _glr->glExt.glDeleteTransformFeedbacksNV
#define glGenTransformFeedbacksNV _glr->glExt.glGenTransformFeedbacksNV
#define glIsTransformFeedbackNV _glr->glExt.glIsTransformFeedbackNV
#define glPauseTransformFeedbackNV _glr->glExt.glPauseTransformFeedbackNV
#define glResumeTransformFeedbackNV _glr->glExt.glResumeTransformFeedbackNV
#define glDrawTransformFeedbackNV _glr->glExt.glDrawTransformFeedbackNV
// #396 GL_NV_vdpau_interop
#define glVDPAUInitNV _glr->glExt.glVDPAUInitNV
#define glVDPAUFiniNV _glr->glExt.glVDPAUFiniNV
#define glVDPAURegisterVideoSurfaceNV _glr->glExt.glVDPAURegisterVideoSurfaceNV
#define glVDPAURegisterOutputSurfaceNV _glr->glExt.glVDPAURegisterOutputSurfaceNV
#define glVDPAUIsSurfaceNV _glr->glExt.glVDPAUIsSurfaceNV
#define glVDPAUUnregisterSurfaceNV _glr->glExt.glVDPAUUnregisterSurfaceNV
#define glVDPAUGetSurfaceivNV _glr->glExt.glVDPAUGetSurfaceivNV
#define glVDPAUSurfaceAccessNV _glr->glExt.glVDPAUSurfaceAccessNV
#define glVDPAUMapSurfacesNV _glr->glExt.glVDPAUMapSurfacesNV
#define glVDPAUUnmapSurfacesNV _glr->glExt.glVDPAUUnmapSurfacesNV
// #190 GL_NV_vertex_array_range
#define glFlushVertexArrayRangeNV _glr->glExt.glFlushVertexArrayRangeNV
#define glVertexArrayRangeNV _glr->glExt.glVertexArrayRangeNV
// #392 GL_NV_vertex_attrib_integer_64bit
#define glVertexAttribL1i64NV _glr->glExt.glVertexAttribL1i64NV
#define glVertexAttribL2i64NV _glr->glExt.glVertexAttribL2i64NV
#define glVertexAttribL3i64NV _glr->glExt.glVertexAttribL3i64NV
#define glVertexAttribL4i64NV _glr->glExt.glVertexAttribL4i64NV
#define glVertexAttribL1i64vNV _glr->glExt.glVertexAttribL1i64vNV
#define glVertexAttribL2i64vNV _glr->glExt.glVertexAttribL2i64vNV
#define glVertexAttribL3i64vNV _glr->glExt.glVertexAttribL3i64vNV
#define glVertexAttribL4i64vNV _glr->glExt.glVertexAttribL4i64vNV
#define glVertexAttribL1ui64NV _glr->glExt.glVertexAttribL1ui64NV
#define glVertexAttribL2ui64NV _glr->glExt.glVertexAttribL2ui64NV
#define glVertexAttribL3ui64NV _glr->glExt.glVertexAttribL3ui64NV
#define glVertexAttribL4ui64NV _glr->glExt.glVertexAttribL4ui64NV
#define glVertexAttribL1ui64vNV _glr->glExt.glVertexAttribL1ui64vNV
#define glVertexAttribL2ui64vNV _glr->glExt.glVertexAttribL2ui64vNV
#define glVertexAttribL3ui64vNV _glr->glExt.glVertexAttribL3ui64vNV
#define glVertexAttribL4ui64vNV _glr->glExt.glVertexAttribL4ui64vNV
#define glGetVertexAttribLi64vNV _glr->glExt.glGetVertexAttribLi64vNV
#define glGetVertexAttribLui64vNV _glr->glExt.glGetVertexAttribLui64vNV
#define glVertexAttribLFormatNV _glr->glExt.glVertexAttribLFormatNV
// #380 GL_NV_vertex_buffer_unified_memory
#define glBufferAddressRangeNV _glr->glExt.glBufferAddressRangeNV
#define glVertexFormatNV _glr->glExt.glVertexFormatNV
#define glNormalFormatNV _glr->glExt.glNormalFormatNV
#define glColorFormatNV _glr->glExt.glColorFormatNV
#define glIndexFormatNV _glr->glExt.glIndexFormatNV
#define glTexCoordFormatNV _glr->glExt.glTexCoordFormatNV
#define glEdgeFlagFormatNV _glr->glExt.glEdgeFlagFormatNV
#define glSecondaryColorFormatNV _glr->glExt.glSecondaryColorFormatNV
#define glFogCoordFormatNV _glr->glExt.glFogCoordFormatNV
#define glVertexAttribFormatNV _glr->glExt.glVertexAttribFormatNV
#define glVertexAttribIFormatNV _glr->glExt.glVertexAttribIFormatNV
#define glGetIntegerui64i_vNV _glr->glExt.glGetIntegerui64i_vNV
// #233 GL_NV_vertex_program
#define glAreProgramsResidentNV _glr->glExt.glAreProgramsResidentNV
#define glBindProgramNV _glr->glExt.glBindProgramNV
#define glDeleteProgramsNV _glr->glExt.glDeleteProgramsNV
#define glExecuteProgramNV _glr->glExt.glExecuteProgramNV
#define glGenProgramsNV _glr->glExt.glGenProgramsNV
#define glGetProgramParameterdvNV _glr->glExt.glGetProgramParameterdvNV
#define glGetProgramParameterfvNV _glr->glExt.glGetProgramParameterfvNV
#define glGetProgramivNV _glr->glExt.glGetProgramivNV
#define glGetProgramStringNV _glr->glExt.glGetProgramStringNV
#define glGetTrackMatrixivNV _glr->glExt.glGetTrackMatrixivNV
#define glGetVertexAttribdvNV _glr->glExt.glGetVertexAttribdvNV
#define glGetVertexAttribfvNV _glr->glExt.glGetVertexAttribfvNV
#define glGetVertexAttribivNV _glr->glExt.glGetVertexAttribivNV
#define glGetVertexAttribPointervNV _glr->glExt.glGetVertexAttribPointervNV
#define glIsProgramNV _glr->glExt.glIsProgramNV
#define glLoadProgramNV _glr->glExt.glLoadProgramNV
#define glProgramParameter4dNV _glr->glExt.glProgramParameter4dNV
#define glProgramParameter4dvNV _glr->glExt.glProgramParameter4dvNV
#define glProgramParameter4fNV _glr->glExt.glProgramParameter4fNV
#define glProgramParameter4fvNV _glr->glExt.glProgramParameter4fvNV
#define glProgramParameters4dvNV _glr->glExt.glProgramParameters4dvNV
#define glProgramParameters4fvNV _glr->glExt.glProgramParameters4fvNV
#define glRequestResidentProgramsNV _glr->glExt.glRequestResidentProgramsNV
#define glTrackMatrixNV _glr->glExt.glTrackMatrixNV
#define glVertexAttribPointerNV _glr->glExt.glVertexAttribPointerNV
#define glVertexAttrib1dNV _glr->glExt.glVertexAttrib1dNV
#define glVertexAttrib1dvNV _glr->glExt.glVertexAttrib1dvNV
#define glVertexAttrib1fNV _glr->glExt.glVertexAttrib1fNV
#define glVertexAttrib1fvNV _glr->glExt.glVertexAttrib1fvNV
#define glVertexAttrib1sNV _glr->glExt.glVertexAttrib1sNV
#define glVertexAttrib1svNV _glr->glExt.glVertexAttrib1svNV
#define glVertexAttrib2dNV _glr->glExt.glVertexAttrib2dNV
#define glVertexAttrib2dvNV _glr->glExt.glVertexAttrib2dvNV
#define glVertexAttrib2fNV _glr->glExt.glVertexAttrib2fNV
#define glVertexAttrib2fvNV _glr->glExt.glVertexAttrib2fvNV
#define glVertexAttrib2sNV _glr->glExt.glVertexAttrib2sNV
#define glVertexAttrib2svNV _glr->glExt.glVertexAttrib2svNV
#define glVertexAttrib3dNV _glr->glExt.glVertexAttrib3dNV
#define glVertexAttrib3dvNV _glr->glExt.glVertexAttrib3dvNV
#define glVertexAttrib3fNV _glr->glExt.glVertexAttrib3fNV
#define glVertexAttrib3fvNV _glr->glExt.glVertexAttrib3fvNV
#define glVertexAttrib3sNV _glr->glExt.glVertexAttrib3sNV
#define glVertexAttrib3svNV _glr->glExt.glVertexAttrib3svNV
#define glVertexAttrib4dNV _glr->glExt.glVertexAttrib4dNV
#define glVertexAttrib4dvNV _glr->glExt.glVertexAttrib4dvNV
#define glVertexAttrib4fNV _glr->glExt.glVertexAttrib4fNV
#define glVertexAttrib4fvNV _glr->glExt.glVertexAttrib4fvNV
#define glVertexAttrib4sNV _glr->glExt.glVertexAttrib4sNV
#define glVertexAttrib4svNV _glr->glExt.glVertexAttrib4svNV
#define glVertexAttrib4ubNV _glr->glExt.glVertexAttrib4ubNV
#define glVertexAttrib4ubvNV _glr->glExt.glVertexAttrib4ubvNV
#define glVertexAttribs1dvNV _glr->glExt.glVertexAttribs1dvNV
#define glVertexAttribs1fvNV _glr->glExt.glVertexAttribs1fvNV
#define glVertexAttribs1svNV _glr->glExt.glVertexAttribs1svNV
#define glVertexAttribs2dvNV _glr->glExt.glVertexAttribs2dvNV
#define glVertexAttribs2fvNV _glr->glExt.glVertexAttribs2fvNV
#define glVertexAttribs2svNV _glr->glExt.glVertexAttribs2svNV
#define glVertexAttribs3dvNV _glr->glExt.glVertexAttribs3dvNV
#define glVertexAttribs3fvNV _glr->glExt.glVertexAttribs3fvNV
#define glVertexAttribs3svNV _glr->glExt.glVertexAttribs3svNV
#define glVertexAttribs4dvNV _glr->glExt.glVertexAttribs4dvNV
#define glVertexAttribs4fvNV _glr->glExt.glVertexAttribs4fvNV
#define glVertexAttribs4svNV _glr->glExt.glVertexAttribs4svNV
#define glVertexAttribs4ubvNV _glr->glExt.glVertexAttribs4ubvNV
// #325 GL_NV_vertex_program4
#define glVertexAttribI1iEXT _glr->glExt.glVertexAttribI1iEXT
#define glVertexAttribI2iEXT _glr->glExt.glVertexAttribI2iEXT
#define glVertexAttribI3iEXT _glr->glExt.glVertexAttribI3iEXT
#define glVertexAttribI4iEXT _glr->glExt.glVertexAttribI4iEXT
#define glVertexAttribI1uiEXT _glr->glExt.glVertexAttribI1uiEXT
#define glVertexAttribI2uiEXT _glr->glExt.glVertexAttribI2uiEXT
#define glVertexAttribI3uiEXT _glr->glExt.glVertexAttribI3uiEXT
#define glVertexAttribI4uiEXT _glr->glExt.glVertexAttribI4uiEXT
#define glVertexAttribI1ivEXT _glr->glExt.glVertexAttribI1ivEXT
#define glVertexAttribI2ivEXT _glr->glExt.glVertexAttribI2ivEXT
#define glVertexAttribI3ivEXT _glr->glExt.glVertexAttribI3ivEXT
#define glVertexAttribI4ivEXT _glr->glExt.glVertexAttribI4ivEXT
#define glVertexAttribI1uivEXT _glr->glExt.glVertexAttribI1uivEXT
#define glVertexAttribI2uivEXT _glr->glExt.glVertexAttribI2uivEXT
#define glVertexAttribI3uivEXT _glr->glExt.glVertexAttribI3uivEXT
#define glVertexAttribI4uivEXT _glr->glExt.glVertexAttribI4uivEXT
#define glVertexAttribI4bvEXT _glr->glExt.glVertexAttribI4bvEXT
#define glVertexAttribI4svEXT _glr->glExt.glVertexAttribI4svEXT
#define glVertexAttribI4ubvEXT _glr->glExt.glVertexAttribI4ubvEXT
#define glVertexAttribI4usvEXT _glr->glExt.glVertexAttribI4usvEXT
#define glVertexAttribIPointerEXT _glr->glExt.glVertexAttribIPointerEXT
#define glGetVertexAttribIivEXT _glr->glExt.glGetVertexAttribIivEXT
#define glGetVertexAttribIuivEXT _glr->glExt.glGetVertexAttribIuivEXT
// #374 GL_NV_video_capture
#define glBeginVideoCaptureNV _glr->glExt.glBeginVideoCaptureNV
#define glBindVideoCaptureStreamBufferNV _glr->glExt.glBindVideoCaptureStreamBufferNV
#define glBindVideoCaptureStreamTextureNV _glr->glExt.glBindVideoCaptureStreamTextureNV
#define glEndVideoCaptureNV _glr->glExt.glEndVideoCaptureNV
#define glGetVideoCaptureivNV _glr->glExt.glGetVideoCaptureivNV
#define glGetVideoCaptureStreamivNV _glr->glExt.glGetVideoCaptureStreamivNV
#define glGetVideoCaptureStreamfvNV _glr->glExt.glGetVideoCaptureStreamfvNV
#define glGetVideoCaptureStreamdvNV _glr->glExt.glGetVideoCaptureStreamdvNV
#define glVideoCaptureNV _glr->glExt.glVideoCaptureNV
#define glVideoCaptureStreamParameterivNV _glr->glExt.glVideoCaptureStreamParameterivNV
#define glVideoCaptureStreamParameterfvNV _glr->glExt.glVideoCaptureStreamParameterfvNV
#define glVideoCaptureStreamParameterdvNV _glr->glExt.glVideoCaptureStreamParameterdvNV
#ifdef OS_WIN
/// WGL_NV_video_capture
#define wglBindVideoCaptureDeviceNV _glr->glExt.wglBindVideoCaptureDeviceNV
#define wglEnumerateVideoCaptureDevicesNV _glr->glExt.wglEnumerateVideoCaptureDevicesNV
#define wglLockVideoCaptureDeviceNV _glr->glExt.wglLockVideoCaptureDeviceNV
#define wglQueryVideoCaptureDeviceNV _glr->glExt.wglQueryVideoCaptureDeviceNV
#define wglReleaseVideoCaptureDeviceNV _glr->glExt.wglReleaseVideoCaptureDeviceNV
#endif
// #077 GL_PGI_misc_hints
#define glHintPGI _glr->glExt.glHintPGI
// #021 GL_SGIS_detail_texture
#define glDetailTexFuncSGIS _glr->glExt.glDetailTexFuncSGIS
#define glGetDetailTexFuncSGIS _glr->glExt.glGetDetailTexFuncSGIS
// #064 GL_SGIS_fog_function
#define glFogFuncSGIS _glr->glExt.glFogFuncSGIS
#define glGetFogFuncSGIS _glr->glExt.glGetFogFuncSGIS
// #025 GL_SGIS_multisample
#define glSampleMaskSGIS _glr->glExt.glSampleMaskSGIS
#define glSamplePatternSGIS _glr->glExt.glSamplePatternSGIS
// #015 GL_SGIS_pixel_texture
#define glPixelTexGenParameteriSGIS _glr->glExt.glPixelTexGenParameteriSGIS
#define glPixelTexGenParameterivSGIS _glr->glExt.glPixelTexGenParameterivSGIS
#define glPixelTexGenParameterfSGIS _glr->glExt.glPixelTexGenParameterfSGIS
#define glPixelTexGenParameterfvSGIS _glr->glExt.glPixelTexGenParameterfvSGIS
#define glGetPixelTexGenParameterivSGIS _glr->glExt.glGetPixelTexGenParameterivSGIS
#define glGetPixelTexGenParameterfvSGIS _glr->glExt.glGetPixelTexGenParameterfvSGIS
// GL_SGIS_point_parameters -not in any list
#define glPointParameterfSGIS _glr->glExt.glPointParameterfSGIS
#define glPointParameterfvSGIS _glr->glExt.glPointParameterfvSGIS
// #022 GL_SGIS_sharpen_texture
#define glSharpenTexFuncSGIS _glr->glExt.glSharpenTexFuncSGIS
#define glGetSharpenTexFuncSGIS _glr->glExt.glGetSharpenTexFuncSGIS
// #016 GL_SGIS_texture4D
#define glTexImage4DSGIS _glr->glExt.glTexImage4DSGIS
#define glTexSubImage4DSGIS _glr->glExt.glTexSubImage4DSGIS
// #214 GL_SGIS_texture_color_mask
#define glTextureColorMaskSGIS _glr->glExt.glTextureColorMaskSGIS
// #007 GL_SGIS_texture_filter4
#define glGetTexFilterFuncSGIS _glr->glExt.glGetTexFilterFuncSGIS
#define glTexFilterFuncSGIS _glr->glExt.glTexFilterFuncSGIS
// #132 GL_SGIX_async
#define glAsyncMarkerSGIX _glr->glExt.glAsyncMarkerSGIX
#define glFinishAsyncSGIX _glr->glExt.glFinishAsyncSGIX
#define glPollAsyncSGIX _glr->glExt.glPollAsyncSGIX
#define glGenAsyncMarkersSGIX _glr->glExt.glGenAsyncMarkersSGIX
#define glDeleteAsyncMarkersSGIX _glr->glExt.glDeleteAsyncMarkersSGIX
#define glIsAsyncMarkerSGIX _glr->glExt.glIsAsyncMarkerSGIX
// #061 GL_SGIX_flush_raster
#define glFlushRasterSGIX _glr->glExt.glFlushRasterSGIX
// GL_SGIX_fragment_lighting - not in any list
#define glFragmentLightfSGIX _glr->glExt.glFragmentLightfSGIX
#define glFragmentLightfvSGIX _glr->glExt.glFragmentLightfvSGIX
#define glFragmentLightiSGIX _glr->glExt.glFragmentLightiSGIX
#define glFragmentLightivSGIX _glr->glExt.glFragmentLightivSGIX
#define glFragmentLightModelfSGIX _glr->glExt.glFragmentLightModelfSGIX
#define glFragmentLightModelfvSGIX _glr->glExt.glFragmentLightModelfvSGIX
#define glFragmentLightModeliSGIX _glr->glExt.glFragmentLightModeliSGIX
#define glFragmentLightModelivSGIX _glr->glExt.glFragmentLightModelivSGIX
#define glFragmentMaterialfSGIX _glr->glExt.glFragmentMaterialfSGIX
#define glFragmentMaterialfvSGIX _glr->glExt.glFragmentMaterialfvSGIX
#define glFragmentMaterialiSGIX _glr->glExt.glFragmentMaterialiSGIX
#define glFragmentMaterialivSGIX _glr->glExt.glFragmentMaterialivSGIX
#define glGetFragmentLightfvSGIX _glr->glExt.glGetFragmentLightfvSGIX
#define glGetFragmentLightivSGIX _glr->glExt.glGetFragmentLightivSGIX
#define glGetFragmentMaterialfvSGIX _glr->glExt.glGetFragmentMaterialfvSGIX
#define glGetFragmentMaterialivSGIX _glr->glExt.glGetFragmentMaterialivSGIX
#define glLightEnviSGIX _glr->glExt.glLightEnviSGIX
// #057 GL_SGIX_framezoom
#define glFrameZoomSGIX _glr->glExt.glFrameZoomSGIX
// GL_SGIX_igloo_interface - not in any list
#define glIglooInterfaceSGIX _glr->glExt.glIglooInterfaceSGIX
// #055 GL_SGIX_instruments
#define glGetInstrumentsSGIX _glr->glExt.glGetInstrumentsSGIX
#define glInstrumentsBufferSGIX _glr->glExt.glInstrumentsBufferSGIX
#define glPollInstrumentsSGIX _glr->glExt.glPollInstrumentsSGIX
#define glReadInstrumentsSGIX _glr->glExt.glReadInstrumentsSGIX
#define glStartInstrumentsSGIX _glr->glExt.glStartInstrumentsSGIX
#define glStopInstrumentsSGIX _glr->glExt.glStopInstrumentsSGIX
// #080 GL_SGIX_list_priority
#define glGetListParameterfvSGIX _glr->glExt.glGetListParameterfvSGIX
#define glGetListParameterivSGIX _glr->glExt.glGetListParameterivSGIX
#define glListParameterfSGIX _glr->glExt.glListParameterfSGIX
#define glListParameterfvSGIX _glr->glExt.glListParameterfvSGIX
#define glListParameteriSGIX _glr->glExt.glListParameteriSGIX
#define glListParameterivSGIX _glr->glExt.glListParameterivSGIX
// #015a GL_SGIX_pixel_texture
#define glPixelTexGenSGIX _glr->glExt.glPixelTexGenSGIX
// GL_SGIX_polynomial_ffd - not in any list
#define glDeformationMap3dSGIX _glr->glExt.glDeformationMap3dSGIX
#define glDeformationMap3fSGIX _glr->glExt.glDeformationMap3fSGIX
#define glDeformSGIX _glr->glExt.glDeformSGIX (mask);}
#define glLoadIdentityDeformationMapSGIX _glr->glExt.glLoadIdentityDeformationMapSGIX
// #060 GL_SGIX_reference_plane
#define glReferencePlaneSGIX _glr->glExt.glReferencePlaneSGIX
// #052 GL_SGIX_sprite
#define glSpriteParameterfSGIX _glr->glExt.glSpriteParameterfSGIX
#define glSpriteParameterfvSGIX _glr->glExt.glSpriteParameterfvSGIX
#define glSpriteParameteriSGIX _glr->glExt.glSpriteParameteriSGIX
#define glSpriteParameterivSGIX _glr->glExt.glSpriteParameterivSGIX
// #058 GL_SGIX_tag_sample_buffer
#define glTagSampleBufferSGIX _glr->glExt.glTagSampleBufferSGIX
// #014 GL_SGI_color_table
#define glColorTableSGI _glr->glExt.glColorTableSGI
#define glColorTableParameterfvSGI _glr->glExt.glColorTableParameterfvSGI
#define glColorTableParameterivSGI _glr->glExt.glColorTableParameterivSGI
#define glCopyColorTableSGI _glr->glExt.glCopyColorTableSGI
#define glGetColorTableSGI _glr->glExt.glGetColorTableSGI
#define glGetColorTableParameterfvSGI _glr->glExt.glGetColorTableParameterfvSGI
#define glGetColorTableParameterivSGI _glr->glExt.glGetColorTableParameterivSGI
// #163 GL_SUNX_constant_data
#define glFinishTextureSUNX _glr->glExt.glFinishTextureSUNX
// #164 GL_SUN_global_alpha
#define glGlobalAlphaFactorbSUN _glr->glExt.glGlobalAlphaFactorbSUN
#define glGlobalAlphaFactorsSUN _glr->glExt.glGlobalAlphaFactorsSUN
#define glGlobalAlphaFactoriSUN _glr->glExt.glGlobalAlphaFactoriSUN
#define glGlobalAlphaFactorfSUN _glr->glExt.glGlobalAlphaFactorfSUN
#define glGlobalAlphaFactordSUN _glr->glExt.glGlobalAlphaFactordSUN
#define glGlobalAlphaFactorubSUN _glr->glExt.glGlobalAlphaFactorubSUN
#define glGlobalAlphaFactorusSUN _glr->glExt.glGlobalAlphaFactorusSUN
#define glGlobalAlphaFactoruiSUN _glr->glExt.glGlobalAlphaFactoruiSUN
// #257 GL_SUN_mesh_array
#define glDrawMeshArraysSUN _glr->glExt.glDrawMeshArraysSUN
// #165 GL_SUN_triangle_list
#define glReplacementCodeuiSUN _glr->glExt.glReplacementCodeuiSUN
#define glReplacementCodeusSUN _glr->glExt.glReplacementCodeusSUN
#define glReplacementCodeubSUN _glr->glExt.glReplacementCodeubSUN
#define glReplacementCodeuivSUN _glr->glExt.glReplacementCodeuivSUN
#define glReplacementCodeusvSUN _glr->glExt.glReplacementCodeusvSUN
#define glReplacementCodeubvSUN _glr->glExt.glReplacementCodeubvSUN
#define glReplacementCodePointerSUN _glr->glExt.glReplacementCodePointerSUN
// #166 GL_SUN_vertex
#define glColor4ubVertex2fSUN _glr->glExt.glColor4ubVertex2fSUN
#define glColor4ubVertex2fvSUN _glr->glExt.glColor4ubVertex2fvSUN
#define glColor4ubVertex3fSUN _glr->glExt.glColor4ubVertex3fSUN
#define glColor4ubVertex3fvSUN _glr->glExt.glColor4ubVertex3fvSUN
#define glColor3fVertex3fSUN _glr->glExt.glColor3fVertex3fSUN
#define glColor3fVertex3fvSUN _glr->glExt.glColor3fVertex3fvSUN
#define glNormal3fVertex3fSUN _glr->glExt.glNormal3fVertex3fSUN
#define glNormal3fVertex3fvSUN _glr->glExt.glNormal3fVertex3fvSUN
#define glColor4fNormal3fVertex3fSUN _glr->glExt.glColor4fNormal3fVertex3fSUN
#define glColor4fNormal3fVertex3fvSUN _glr->glExt.glColor4fNormal3fVertex3fvSUN
#define glTexCoord2fVertex3fSUN _glr->glExt.glTexCoord2fVertex3fSUN
#define glTexCoord2fVertex3fvSUN _glr->glExt.glTexCoord2fVertex3fvSUN
#define glTexCoord4fVertex4fSUN _glr->glExt.glTexCoord4fVertex4fSUN
#define glTexCoord4fVertex4fvSUN _glr->glExt.glTexCoord4fVertex4fvSUN
#define glTexCoord2fColor4ubVertex3fSUN _glr->glExt.glTexCoord2fColor4ubVertex3fSUN
#define glTexCoord2fColor4ubVertex3fvSUN _glr->glExt.glTexCoord2fColor4ubVertex3fvSUN
#define glTexCoord2fColor3fVertex3fSUN _glr->glExt.glTexCoord2fColor3fVertex3fSUN
#define glTexCoord2fColor3fVertex3fvSUN _glr->glExt.glTexCoord2fColor3fVertex3fvSUN
#define glTexCoord2fNormal3fVertex3fSUN _glr->glExt.glTexCoord2fNormal3fVertex3fSUN
#define glTexCoord2fNormal3fVertex3fvSUN _glr->glExt.glTexCoord2fNormal3fVertex3fvSUN
#define glTexCoord2fColor4fNormal3fVertex3fSUN _glr->glExt.glTexCoord2fColor4fNormal3fVertex3fSUN
#define glTexCoord2fColor4fNormal3fVertex3fvSUN _glr->glExt.glTexCoord2fColor4fNormal3fVertex3fvSUN
#define glTexCoord4fColor4fNormal3fVertex4fSUN _glr->glExt.glTexCoord4fColor4fNormal3fVertex4fSUN
#define glTexCoord4fColor4fNormal3fVertex4fvSUN _glr->glExt.glTexCoord4fColor4fNormal3fVertex4fvSUN
#define glReplacementCodeuiVertex3fSUN _glr->glExt.glReplacementCodeuiVertex3fSUN
#define glReplacementCodeuiVertex3fvSUN _glr->glExt.glReplacementCodeuiVertex3fvSUN
#define glReplacementCodeuiColor4ubVertex3fSUN _glr->glExt.glReplacementCodeuiColor4ubVertex3fSUN
#define glReplacementCodeuiColor4ubVertex3fvSUN _glr->glExt.glReplacementCodeuiColor4ubVertex3fvSUN
#define glReplacementCodeuiColor3fVertex3fSUN _glr->glExt.glReplacementCodeuiColor3fVertex3fSUN
#define glReplacementCodeuiColor3fVertex3fvSUN _glr->glExt.glReplacementCodeuiColor3fVertex3fvSUN
#define glReplacementCodeuiNormal3fVertex3fSUN _glr->glExt.glReplacementCodeuiNormal3fVertex3fSUN
#define glReplacementCodeuiNormal3fVertex3fvSUN _glr->glExt.glReplacementCodeuiNormal3fVertex3fvSUN
#define glReplacementCodeuiColor4fNormal3fVertex3fSUN _glr->glExt.glReplacementCodeuiColor4fNormal3fVertex3fSUN
#define glReplacementCodeuiColor4fNormal3fVertex3fvSUN _glr->glExt.glReplacementCodeuiColor4fNormal3fVertex3fvSUN
#define glReplacementCodeuiTexCoord2fVertex3fSUN _glr->glExt.glReplacementCodeuiTexCoord2fVertex3fSUN
#define glReplacementCodeuiTexCoord2fVertex3fvSUN _glr->glExt.glReplacementCodeuiTexCoord2fVertex3fvSUN
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN _glr->glExt.glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN _glr->glExt.glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN _glr->glExt.glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN _glr->glExt.glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN

#endif /// OSI_USING_GLEXT_DEFINES















///=========================================///
// CORE or not in OpenGL registry extensions //
///=========================================///
/* glOTHERlist
documentation for these is pretty scarce, but should function if the grCard support them
#001 GL_ARB_imaging 
#002 GL_ARB_bindless_texture
#003 GL_INGR_blend_func_separate
#004 GL_SGIX_fragment_lighting
#005 GL_SGIX_polynomial_ffd
#006 GL_SGIS_point_parameters
#007 GL_SGIX_igloo_interface
#008 WGL_NV_vertex_array_range
 */



///=============================================================================///
// ARB extension list =========================--------------------------------- //
///=============================================================================///

/* [extension ARB number] / [if it is a part of a core oGL number] / [small description] / [extension spec web page]
#001 GL_ARB_multitexture              [] http://www.opengl.org/registry/specs/ARB/multitexture.txt
#002 GLX_ARB_get_proc_address         [returns the addr of GLX and GL ext funcs] http://www.opengl.org/registry/specs/ARB/get_proc_address.txt
#003 GL_ARB_transpose_matrix          [C-style matrices (transposed)] http://www.opengl.org/registry/specs/ARB/transpose_matrix.txt
#004 WGL_ARB_buffer_region            [allows an area of an oGL window to be saved in off-screen mem] http://www.opengl.org/registry/specs/ARB/wgl_buffer_region.txt
#005 GL_ARB_multisample GLX_ARB_multisample WGL_ARB_multisample [Multisample antialiasing] http://www.opengl.org/registry/specs/ARB/multisample.txt
#006 GL_ARB_texture_env_add           [tex environment func ADD] http://www.opengl.org/registry/specs/ARB/texture_env_add.txt
#007 GL_ARB_texture_cube_map          [cube map textures] http://www.opengl.org/registry/specs/ARB/texture_cube_map.txt
#008 WGL_ARB_extensions_string        [apps can determine which WGL exts are supported] http://www.opengl.org/registry/specs/ARB/wgl_extensions_string.txt
#009 WGL_ARB_pixel_format             [pixel format funcs] http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
#010 WGL_ARB_make_current_read        [separate "read" and "draw" DC with the current context] http://www.opengl.org/registry/specs/ARB/wgl_make_current_read.txt
#011 WGL_ARB_pbuffer                  [defines pixel buffers] http://www.opengl.org/registry/specs/ARB/wgl_pbuffer.txt
#012 GL_ARB_texture_compression       [compressed textures framework (and some default compression modes)] http://www.opengl.org/registry/specs/ARB/texture_compression.txt
#013 GL_ARB_texture_border_clamp      [border clamp wrapping] http://www.opengl.org/registry/specs/ARB/texture_border_clamp.txt
#014 GL_ARB_point_parameters          [additional geometric characteristics of points] http://www.opengl.org/registry/specs/ARB/point_parameters.txt
#015 GL_ARB_vertex_blend              [vertex blend and weight funcs] http://www.opengl.org/registry/specs/ARB/vertex_blend.txt
#016 GL_ARB_matrix_palette            [extends vertex blend GL_ARB_vertex_blend] http://www.opengl.org/registry/specs/ARB/matrix_palette.txt
#017 GL_ARB_texture_env_combine       [new texture environment function] http://www.opengl.org/registry/specs/ARB/texture_env_combine.txt
#018 GL_ARB_texture_env_crossbar      [new texture environment function] http://www.opengl.org/registry/specs/ARB/texture_env_crossbar.txt
#019 GL_ARB_texture_env_dot3          [new operation to the texture combiner operations] http://www.opengl.org/registry/specs/ARB/texture_env_dot3.txt
#020 WGL_ARB_render_texture           [allows a color buffer to be used for both rendering and texturing] http://www.opengl.org/registry/specs/ARB/wgl_render_texture.txt
#021 GL_ARB_texture_mirrored_repeat   [tex wrap mode] http://www.opengl.org/registry/specs/ARB/texture_mirrored_repeat.txt
#022 GL_ARB_depth_texture             [depth texture format (ex: shadow casting)] http://www.opengl.org/registry/specs/ARB/depth_texture.txt
#023 GL_ARB_shadow                    [shadow maps] http://www.opengl.org/registry/specs/ARB/shadow.txt
#024 GL_ARB_shadow_ambient            [shadows, again!] http://www.opengl.org/registry/specs/ARB/shadow_ambient.txt
#025 GL_ARB_window_pos                [sets current raster pos in window coords] http://www.opengl.org/registry/specs/ARB/window_pos.txt
#026 GL_ARB_vertex_program            [vertex program!] http://www.opengl.org/registry/specs/ARB/vertex_program.txt
#027 GL_ARB_fragment_program          [fragment program!] http://www.opengl.org/registry/specs/ARB/fragment_program.txt
#028 GL_ARB_vertex_buffer_object GLX_ARB_vertex_buffer_object [VBO's !!!] http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
#029 GL_ARB_occlusion_query           [determine the visibility of an object] http://www.opengl.org/registry/specs/ARB/occlusion_query.txt
#030 GL_ARB_shader_objects            [shader objects!] http://www.opengl.org/registry/specs/ARB/shader_objects.txt
#031 GL_ARB_vertex_shader             [vertex shader] http://www.opengl.org/registry/specs/ARB/vertex_shader.txt
#032 GL_ARB_fragment_shader           [fragment shader] http://www.opengl.org/registry/specs/ARB/fragment_shader.txt
#033 GL_ARB_shading_language_100      [shading language 1.00] http://www.opengl.org/registry/specs/ARB/shading_language_100.txt
#034 GL_ARB_texture_non_power_of_two  [non-power of two textures] http://www.opengl.org/registry/specs/ARB/texture_non_power_of_two.txt
#035 GL_ARB_point_sprite              [point sprites] http://www.opengl.org/registry/specs/ARB/point_sprite.txt
#036 GL_ARB_fragment_program_shadow   [fragment program shadow options] http://www.opengl.org/registry/specs/ARB/fragment_program_shadow.txt
#037 GL_ARB_draw_buffers              [extends ARB_fragment_program and ARB_fragment_shader] http://www.opengl.org/registry/specs/ARB/draw_buffers.txt
#038 GL_ARB_texture_rectangle         [another non-power of two type of texture] http://www.opengl.org/registry/specs/ARB/texture_rectangle.txt
#039 GL_ARB_color_buffer_float WGL_ARB_pixel_format_float GLX_ARB_fbconfig_float [floating-point RGBA pixel format] http://www.opengl.org/registry/specs/ARB/color_buffer_float.txt
#040 GL_ARB_half_float_pixel          [half-precision floating-point for pixel data] http://www.opengl.org/registry/specs/ARB/half_float_pixel.txt
#041 GL_ARB_texture_float             [16&32bit floats for tex internal formats] http://www.opengl.org/registry/specs/ARB/texture_float.txt
#042 GL_ARB_pixel_buffer_object       [PBOs! accelerates work on pixels] http://www.opengl.org/registry/specs/ARB/pixel_buffer_object.txt
#043 GL_ARB_depth_buffer_float        [float32 for depth internal format] http://www.opengl.org/registry/specs/ARB/depth_buffer_float.txt
#044 GL_ARB_draw_instanced            [draw same object multiple times faster] http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
#045 3.0 GL_ARB_framebuffer_object    [fastest / best extension for 'texture rendering'] http://www.opengl.org/registry/specs/ARB/framebuffer_object.txt
#046 GL_ARB_framebuffer_sRGB GLX_ARB_framebuffer_sRGB WGL_ARB_framebuffer_sRGB [sRGB framebuffer update and blending] http://www.opengl.org/registry/specs/ARB/framebuffer_sRGB.txt
#047 GL_ARB_geometry_shader4          [a new shader type] http://www.opengl.org/registry/specs/ARB/geometry_shader4.txt
#048 GL_ARB_half_float_vertex         [float16 vertices] http://www.opengl.org/registry/specs/ARB/half_float_vertex.txt
#049 GL_ARB_instanced_arrays          [multiple draws of the same object] http://www.opengl.org/registry/specs/ARB/instanced_arrays.txt
#050 3.0 GL_ARB_map_buffer_range      [expands the buffer object API] http://www.opengl.org/registry/specs/ARB/map_buffer_range.txt
#051 GL_ARB_texture_buffer_object     [a new texture type] http://www.opengl.org/registry/specs/ARB/texture_buffer_object.txt
#052 GL_ARB_texture_compression_rgtc  [four new block-based texture compression formats] http://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
#053 GL_ARB_texture_rg                [1&2 component texture internal format] http://www.opengl.org/registry/specs/ARB/texture_rg.txt
#054 3.0 GL_ARB_vertex_array_object   [named vertex array objects... hmm] http://www.opengl.org/registry/specs/ARB/vertex_array_object.txt
#055 WGL_ARB_create_context WGL_ARB_create_context_profile [!!! advanced context creation] http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
#056 GLX_ARB_create_context GLX_ARB_create_context_profile [!!! advanced context creation] http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
#057 3.1 GL_ARB_uniform_buffer_object [GLSL uniforms known as a "uniform block"] http://www.opengl.org/registry/specs/ARB/uniform_buffer_object.txt
#058 GL_ARB_compatibility         !!! [this  enables pre 3.0 ogl deprecated stuff; ARB_create_context can set this extension off] http://www.opengl.org/registry/specs/ARB/compatibility.txt
#059 3.1 GL_ARB_copy_buffer           [accelerated copy from one buffer object to another] http://www.opengl.org/registry/specs/ARB/copy_buffer.txt
#060 GL_ARB_shader_texture_lod        [adds texture functions to the GLSL] http://www.opengl.org/registry/specs/ARB/shader_texture_lod.txt
#061 GL_ARB_depth_clamp               [near&far clipping stuff] http://www.opengl.org/registry/specs/ARB/depth_clamp.txt
#062 3.2 GL_ARB_draw_elements_base_vertex [draw elements extended] http://www.opengl.org/registry/specs/ARB/draw_elements_base_vertex.txt
#063 GL_ARB_fragment_coord_conventions [XY coords conventions] http://www.opengl.org/registry/specs/ARB/fragment_coord_conventions.txt
#064 3.2 GL_ARB_provoking_vertex      [first vertex has a special attr, check overview] http://www.opengl.org/registry/specs/ARB/provoking_vertex.txt
#065 GL_ARB_seamless_cube_map         [cube map stuff] http://www.opengl.org/registry/specs/ARB/seamless_cube_map.txt
#066 3.2 GL_ARB_sync                  [sync between the host CPU and the GPU] http://www.opengl.org/registry/specs/ARB/sync.txt
#067 3.2 GL_ARB_texture_multisample   [new types of "multisample textures"] http://www.opengl.org/registry/specs/ARB/texture_multisample.txt
#068 GL_ARB_vertex_array_bgra         [BGRA component ordering vertex attr] http://www.opengl.org/registry/specs/ARB/vertex_array_bgra.txt
#069 GL_ARB_draw_buffers_blend        [blend stuff for ARB_draw_buffers] http://www.opengl.org/registry/specs/ARB/draw_buffers_blend.txt
#070 GL_ARB_sample_shading            [GLSL stuff] http://www.opengl.org/registry/specs/ARB/sample_shading.txt
#071 GL_ARB_texture_cube_map_array    [array of cube map tex, i guess] http://www.opengl.org/registry/specs/ARB/texture_cube_map_array.txt
#072 GL_ARB_texture_gather            [GLSL tex funcs] http://www.opengl.org/registry/specs/ARB/texture_gather.txt
#073 GL_ARB_texture_query_lod         [fragment shader tex funcs] http://www.opengl.org/registry/specs/ARB/texture_query_lod.txt
#074 WGL_ARB_create_context WGL_ARB_create_context_profile [context creation attribs] http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
#075 GLX_ARB_create_context           [context creation attribs] http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
#076 GL_ARB_shading_language_include  [#include GLSL directive] http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
#077 GL_ARB_texture_compression_bptc  [BPTC and BPTC_FLOAT tex compression] http://www.opengl.org/registry/specs/ARB/texture_compression_bptc.txt
#078 3.3 GL_ARB_blend_func_extended   [new blending funcs] http://www.opengl.org/registry/specs/ARB/blend_func_extended.txt
#079 GL_ARB_explicit_attrib_location  ['pre-assign attrib locations to named vertex shader inputs..'] http://www.opengl.org/registry/specs/ARB/explicit_attrib_location.txt
#080 GL_ARB_occlusion_query2          [boolean occlusion] http://www.opengl.org/registry/specs/ARB/occlusion_query2.txt
#081 3.3 GL_ARB_sampler_objects       [(texture stuff, pretty good)A new object type is introduced, the sampler] http://www.opengl.org/registry/specs/ARB/sampler_objects.txt
#082 GL_ARB_shader_bit_encoding       [GLSL floating point funcs] http://www.opengl.org/registry/specs/ARB/shader_bit_encoding.txt
#083 GL_ARB_texture_rgb10_a2ui        [unsigned 10,10,10,10,2 int textures] http://www.opengl.org/registry/specs/ARB/texture_rgb10_a2ui.txt
#084 GL_ARB_texture_swizzle           [!!! tex swizzle] http://www.opengl.org/registry/specs/ARB/texture_swizzle.txt
#085 3.3 GL_ARB_timer_query           [accurate timer] http://www.opengl.org/registry/specs/ARB/timer_query.txt
#086 3.3 GL_ARB_vertex_type_2_10_10_10_rev [signed/unsigned 2.10.10.10 vert data format] http://www.opengl.org/registry/specs/ARB/vertex_type_2_10_10_10_rev.txt
#087 4.0 GL_ARB_draw_indirect         [supply args to DrawArraysInstanced / DrawElementsInstancedBaseVertex from buffer object memory] http://www.opengl.org/registry/specs/ARB/draw_indirect.txt
#088 GL_ARB_gpu_shader5               [GLSL new feats] http://www.opengl.org/registry/specs/ARB/gpu_shader5.txt
#089 4.0 GL_ARB_gpu_shader_fp64       [GLSL shaders can use double (float64)] http://www.opengl.org/registry/specs/ARB/gpu_shader_fp64.txt
#090 4.0 GL_ARB_shader_subroutine     [shader stuff -"indirect subroutine calls"] http://www.opengl.org/registry/specs/ARB/shader_subroutine.txt
#091 4.0 GL_ARB_tessellation_shader   [tesselation shader stuff] http://www.opengl.org/registry/specs/ARB/tessellation_shader.txt
#092 GL_ARB_texture_buffer_object_rgb32 [new buffer texture formats] http://www.opengl.org/registry/specs/ARB/texture_buffer_object_rgb32.txt
#093 4.0 GL_ARB_transform_feedback2   [trans feedback extended] http://www.opengl.org/registry/specs/ARB/transform_feedback2.txt
#094 4.0 GL_ARB_transform_feedback3   [trans feedback extended] http://www.opengl.org/registry/specs/ARB/transform_feedback3.txt
#095 4.1 GL_ARB_ES2_compatibility     [adds features from OpenGL ES 2.0] http://www.opengl.org/registry/specs/ARB/ES2_compatibility.txt
#096 4.1 GL_ARB_get_program_binary    [GLSL program important stuff] http://www.opengl.org/registry/specs/ARB/get_program_binary.txt
#097 GL_ARB_separate_shader_objects   [GLSL stuff] http://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt
#098 GL_ARB_shader_precision          [GLSL stuff] http://www.opengl.org/registry/specs/ARB/shader_precision.txt
#099 4.1 GL_ARB_vertex_attrib_64bit   [GLSL double (float64)] http://www.opengl.org/registry/specs/ARB/vertex_attrib_64bit.txt
#100 4.1 GL_ARB_viewport_array        [viewport/scissor stuff] http://www.opengl.org/registry/specs/ARB/viewport_array.txt
#101 GLX_ARB_create_context_robustness ['robust' contexts & more options] http://www.opengl.org/registry/specs/ARB/glx_create_context_robustness.txt
#102 WGL_ARB_create_context_robustness ['robust' contexts & more options] http://www.opengl.org/registry/specs/ARB/wgl_create_context_robustness.txt
#103 GL_ARB_cl_event                  [OpenGL sync objects linked to OpenCL] http://www.opengl.org/registry/specs/ARB/cl_event.txt
#104 GL_ARB_debug_output              [oGL debbugging & notifier funcs] http://www.opengl.org/registry/specs/ARB/debug_output.txt
#105 GL_ARB_robustness                ['robustness and security' for oGL contexts] http://www.opengl.org/registry/specs/ARB/robustness.txt
#106 GL_ARB_shader_stencil_export     [shader stencil stuff] http://www.opengl.org/registry/specs/ARB/shader_stencil_export.txt
#107 4.2 GL_ARB_base_instance         [DrawArrays more stuff] http://www.opengl.org/registry/specs/ARB/base_instance.txt
#108 GL_ARB_shading_language_420pack  [GLSL 4.20] http://www.opengl.org/registry/specs/ARB/shading_language_420pack.txt
#109 4.2 GL_ARB_transform_feedback_instanced [DrawTransformFeedback stuff] http://www.opengl.org/registry/specs/ARB/transform_feedback_instanced.txt
#110 GL_ARB_compressed_texture_pixel_storage [pixel store extension] http://www.opengl.org/registry/specs/ARB/compressed_texture_pixel_storage.txt
#111 GL_ARB_conservative_depth        [early depth test, can optimize stuff] http://www.opengl.org/registry/specs/ARB/conservative_depth.txt
#112 4.2 GL_ARB_internalformat_query  [more querry mechs (yes, the big robbots)] http://www.opengl.org/registry/specs/ARB/internalformat_query.txt
#113 GL_ARB_map_buffer_alignment      [map buffer works on 64b pointers] http://www.opengl.org/registry/specs/ARB/map_buffer_alignment.txt
#114 4.2 GL_ARB_shader_atomic_counters [GLSL atomic counters] http://www.opengl.org/registry/specs/ARB/shader_atomic_counters.txt
#115 GL_ARB_shader_image_load_store   [GLSL texture stuff, memory sync stuff] http://www.opengl.org/registry/specs/ARB/shader_image_load_store.txt
#116 GL_ARB_shading_language_packing  [GLSL convert funcs] http://www.opengl.org/registry/specs/ARB/shading_language_packing.txt
#117 GL_ARB_texture_storage           [each tex level characteristics & mods, can set IMMUTABLE to a tex] http://www.opengl.org/registry/specs/ARB/texture_storage.txt
#118 GL_KHR_texture_compression_astc_hdr GL_KHR_texture_compression_astc_ldr [ASTC tex compression] http://www.opengl.org/registry/specs/KHR/texture_compression_astc_hdr.txt
#119 4.3 GL_KHR_debug                 [oGL debug & notify funcs (seems to be like GL_ARB_debug_output)] http://www.opengl.org/registry/specs/KHR/debug.txt
#120 GL_ARB_arrays_of_arrays          [GLSL multi dimensional arrays (yeee!)] http://www.opengl.org/registry/specs/ARB/arrays_of_arrays.txt
#121 4.3 GL_ARB_clear_buffer_object       [adds a 'memset mechanism' to buffer objects] http://www.opengl.org/registry/specs/ARB/clear_buffer_object.txt
#122 4.3 GL_ARB_compute_shader        [! <funcs to use gr card for OTHER THAN DRAWING> !] http://www.opengl.org/registry/specs/ARB/compute_shader.txt
#123 4.3 GL_ARB_copy_image            [a memcpy for the gr card (fastest possible, no pipeline messings etc)] http://www.opengl.org/registry/specs/ARB/copy_image.txt
#124 4.3 GL_ARB_texture_view          [tex data can be 'view'ed in different ways] http://www.opengl.org/registry/specs/ARB/texture_view.txt
#125 4.3 GL_ARB_vertex_attrib_binding [more vertex attr bindings] http://www.opengl.org/registry/specs/ARB/vertex_attrib_binding.txt
#126 GL_ARB_robustness_application_isolation GL_ARB_robustness_share_group_isolation [context 'robustness' & more context attr] http://www.opengl.org/registry/specs/ARB/robustness_isolation.txt
#127 GL_ARB_ES3_compatibility         [adds oGL ES 3.0 stuff to oGL3.x] http://www.opengl.org/registry/specs/ARB/ES3_compatibility.txt
#128 GL_ARB_explicit_uniform_location [pre-assign uniform locations to unif vars] http://www.opengl.org/registry/specs/ARB/explicit_uniform_location.txt
#129 GL_ARB_fragment_layer_viewport   [adds gl_Layer and gl_ViewportIndex to frag shader] http://www.opengl.org/registry/specs/ARB/fragment_layer_viewport.txt
#130 4.3 GL_ARB_framebuffer_no_attachments [fb extra stuff, configs etc(lots)] http://www.opengl.org/registry/specs/ARB/framebuffer_no_attachments.txt
#131 GL_ARB_internalformat_query2     [more querryes] http://www.opengl.org/registry/specs/ARB/internalformat_query2.txt
#132 4.3 GL_ARB_invalidate_subdata    [mark contents of a tex or buffer as no longer needed] http://www.opengl.org/registry/specs/ARB/invalidate_subdata.txt
#133 4.3 GL_ARB_multi_draw_indirect   [!! mass drawing of stuff] http://www.opengl.org/registry/specs/ARB/multi_draw_indirect.txt
#134 4.3 GL_ARB_program_interface_query [! lots of more querryes] http://www.opengl.org/registry/specs/ARB/program_interface_query.txt
#135 GL_ARB_robust_buffer_access_behavior [behaivour of these 'robust' contexts] http://www.opengl.org/registry/specs/ARB/robust_buffer_access_behavior.txt
#136 GL_ARB_shader_image_size         [GLSL funcs allowing shaders to query the size of an img] http://www.opengl.org/registry/specs/ARB/shader_image_size.txt
#137 4.3 GL_ARB_shader_storage_buffer_object [shaders can do random access r/w and atomic mem ops on vars stored in a buffer object] http://www.opengl.org/registry/specs/ARB/shader_storage_buffer_object.txt
#138 GL_ARB_stencil_texturing         [resolves a depth/stencil problem] http://www.opengl.org/registry/specs/ARB/stencil_texturing.txt
#139 4.3 GL_ARB_texture_buffer_range  [extends ARB_texture_buffer_object] http://www.opengl.org/registry/specs/ARB/texture_buffer_range.txt
#140 GL_ARB_texture_query_levels      [GLSL texture querry funcs] http://www.opengl.org/registry/specs/ARB/texture_query_levels.txt
#141 4.3 GL_ARB_texture_storage_multisample [immutable storage for multisampled texs] http://www.opengl.org/registry/specs/ARB/texture_storage_multisample.txt
#142 GLX_ARB_robustness_application_isolation GLX_ARB_robustness_share_group_isolation ['robust' contexts further config] http://www.opengl.org/registry/specs/ARB/glx_robustness_isolation.txt
#143 WGL_ARB_robustness_application_isolation WGL_ARB_robustness_share_group_isolation ['robust' contexts further config] http://www.opengl.org/registry/specs/ARB/wgl_robustness_isolation.txt
#144 4.4 GL_ARB_buffer_storage        [immutable storage to buffer objects] http://www.opengl.org/registry/specs/ARB/buffer_storage.txt
#145 4.4 GL_ARB_clear_texture         [tex clearing (or set an initial val)] http://www.opengl.org/registry/specs/ARB/clear_texture.txt
#146 GL_ARB_enhanced_layouts          [has batman in overview!] http://www.opengl.org/registry/specs/ARB/enhanced_layouts.txt
#147 GL_ARB_multi_bind                [bind multiple obj in a singe call] http://www.opengl.org/registry/specs/ARB/multi_bind.txt
#148 GL_ARB_query_buffer_object       [querrys return in buf mem, so shaders can access without a trip to normal mem] http://www.opengl.org/registry/specs/ARB/query_buffer_object.txt
#149 GL_ARB_texture_mirror_clamp_to_edge [new texture mode, mirrors only once] http://www.opengl.org/registry/specs/ARB/texture_mirror_clamp_to_edge.txt
#150 ARB_texture_stencil8             [STENCIL_INDEX8 tex internal format] http://www.opengl.org/registry/specs/ARB/texture_stencil8.txt
#151 GL_ARB_vertex_type_10f_11f_11f_rev [new data format] http://www.opengl.org/registry/specs/ARB/vertex_type_10f_11f_11f_rev.txt
#152 4.4 GL_ARB_bindless_texture      [GLSL can access a tex without binding (faster)] http://www.opengl.org/registry/specs/ARB/bindless_texture.txt
#153 GL_ARB_compute_variable_group_size [shader stuff] http://www.opengl.org/registry/specs/ARB/compute_variable_group_size.txt
#154 GL_ARB_indirect_parameters       [!extra stuff for mass drawing] http://www.opengl.org/registry/specs/ARB/indirect_parameters.txt
#155 GL_ARB_seamless_cubemap_per_texture [cube map texture feature] http://www.opengl.org/registry/specs/ARB/seamless_cubemap_per_texture.txt
#156 GL_ARB_shader_draw_parameters    [shader stuff] http://www.opengl.org/registry/specs/ARB/shader_draw_parameters.txt
#157 GL_ARB_shader_group_vote         [shader stuff] http://www.opengl.org/registry/specs/ARB/shader_group_vote.txt
#158 GL_ARB_sparse_texture            [texture memory allocation management] http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
#159 GL_ARB_ES3_1_compatibility       [OpenGL ES stuff, for easier porting] http://www.opengl.org/registry/specs/ARB/ES3_1_compatibility.txt
#160 GL_ARB_clip_control              [Direct3D's coordinate system & improve precision of the Z coord mapping] http://www.opengl.org/registry/specs/ARB/clip_control.txt
#161 GL_ARB_conditional_render_inverted [new modes to BeginConditionalRender] http://www.opengl.org/registry/specs/ARB/conditional_render_inverted.txt
#162 GL_ARB_cull_distance             [new GLSL gl_CullDistance shader output] http://www.opengl.org/registry/specs/ARB/cull_distance.txt
#163 GL_ARB_derivative_control        ['This extension provides control over the spacial granularity at which the underlying implementation computes derivatives.' ... shoot me!] http://www.opengl.org/registry/specs/ARB/derivative_control.txt
#164 GL_ARB_direct_state_access       [no need to bind an object to a context to modify it(big ext)] http://www.opengl.org/registry/specs/ARB/direct_state_access.txt
#165 GL_ARB_get_texture_sub_image     [funcs to get sub-regions of textures] http://www.opengl.org/registry/specs/ARB/get_texture_sub_image.txt
#166 GL_ARB_shader_texture_image_samples [GLSL funcs allowing shaders to query the nr of samples of a tex] http://www.opengl.org/registry/specs/ARB/shader_texture_image_samples.txt
#167 GL_ARB_texture_barrier           [relaxes restrictions on rendering to a bound tex & mechanism to avoid read-after-write hazards] http://www.opengl.org/registry/specs/ARB/texture_barrier.txt
#168 GL_KHR_context_flush_control GLX_ARB_context_flush_control WGL_ARB_context_flush_control [!!! new context creation parameters (no more flushes after a context switch for example)] http://www.opengl.org/registry/specs/KHR/context_flush_control.txt
#169 GL_KHR_robust_buffer_access_behavior [no more crashes when an access of out of bonds buffer and array access] http://www.opengl.org/registry/specs/KHR/robust_buffer_access_behavior.txt
#170 GL_KHR_robustness                [fast check of the context status (crashed, reset etc)/ safe checks, if the gr card accessed out of bounds stuff] http://www.opengl.org/registry/specs/KHR/robustness.txt
#171 GL_ARB_pipeline_statistics_query [new query types that allow apps to get statistics information] http://www.opengl.org/registry/specs/ARB/pipeline_statistics_query.txt
#172 GL_ARB_sparse_buffer             ['create partially populated textures that would over-subscribe available graphics memory if made fully resident' ... or something, as Butthead would say] http://www.opengl.org/registry/specs/ARB/sparse_buffer.txt
#173 GL_ARB_transform_feedback_overflow_query [new queries to detect overflows of transform feedback buffers] http://www.opengl.org/registry/specs/ARB/transform_feedback_overflow_query.txt
*/

///============================================================================///
// <<<<<<<<<<<<<<<<<<<<<<<<< ARB STUFF ENDS HERE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
///============================================================================///






///=============================================================================///
// EXT extension list =========================--------------------------------- //
///=============================================================================///


/* [extension EXT number] / [small description] / [extension spec web page]
#001 GL_EXT_abgr               [] http://www.opengl.org/registry/specs/EXT/abgr.txt
#002 GL_EXT_blend_color        [] http://www.opengl.org/registry/specs/EXT/blend_color.txt
#003 GL_EXT_polygon_offset     [] http://www.opengl.org/registry/specs/EXT/polygon_offset.txt
#004 GL_EXT_texture            [] http://www.opengl.org/registry/specs/EXT/texture.txt
#006 GL_EXT_texture3D          [] http://www.opengl.org/registry/specs/EXT/texture3D.txt
#007 GL_SGIS_texture_filter4   [] http://www.opengl.org/registry/specs/SGIS/texture_filter4.txt
#009 GL_EXT_subtexture         [] http://www.opengl.org/registry/specs/EXT/subtexture.txt
#010 GL_EXT_copy_texture       [] http://www.opengl.org/registry/specs/EXT/copy_texture.txt
#011 GL_EXT_histogram          [] http://www.opengl.org/registry/specs/EXT/histogram.txt
#012 GL_EXT_convolution        [] http://www.opengl.org/registry/specs/EXT/convolution.txt
#013 GL_SGI_color_matrix       [] http://www.opengl.org/registry/specs/SGI/color_matrix.txt
#014 GL_SGI_color_table        [] http://www.opengl.org/registry/specs/SGI/color_table.txt
#015 GL_SGIS_pixel_texture     [] GL_SGIX_pixel_texture #15 & #15a http://www.opengl.org/registry/specs/SGIS/pixel_texture.txt http://www.opengl.org/registry/specs/SGIX/sgix_pixel_texture.txt
#016 GL_SGIS_texture4D         [] http://www.opengl.org/registry/specs/SGIS/texture4D.txt
#017 GL_SGI_texture_color_table [] http://www.opengl.org/registry/specs/SGI/texture_color_table.txt
#018 GL_EXT_cmyka              [] http://www.opengl.org/registry/specs/EXT/cmyka.txt
#020 GL_EXT_texture_object     [] http://www.opengl.org/registry/specs/EXT/texture_object.txt
#021 GL_SGIS_detail_texture    [] http://www.opengl.org/registry/specs/SGIS/detail_texture.txt
#022 GL_SGIS_sharpen_texture   [] http://www.opengl.org/registry/specs/SGIS/sharpen_texture.txt
#023 GL_EXT_packed_pixels      [] http://www.opengl.org/registry/specs/EXT/packed_pixels.txt
#024 GL_SGIS_texture_lod       [] http://www.opengl.org/registry/specs/SGIS/texture_lod.txt
#025 GL_SGIS_multisample       [] GLX_SGIS_multisample http://www.opengl.org/registry/specs/SGIS/multisample.txt
#027 GL_EXT_rescale_normal     [] http://www.opengl.org/registry/specs/EXT/rescale_normal.txt
#028 GLX_EXT_visual_info       [] http://www.opengl.org/registry/specs/EXT/visual_info.txt
#030 GL_EXT_vertex_array       [] http://www.opengl.org/registry/specs/EXT/vertex_array.txt
#031 GL_EXT_misc_attribute     [] http://www.opengl.org/registry/specs/EXT/misc_attribute.txt
#032 GL_SGIS_generate_mipmap   [] http://www.opengl.org/registry/specs/SGIS/generate_mipmap.txt
#033 GL_SGIX_clipmap           [incomplete] http://www.opengl.org/registry/specs/SGIX/clipmap.txt
#034 GL_SGIX_shadow            [incomplete] http://www.opengl.org/registry/specs/SGIX/shadow.txt
#035 GL_SGIS_texture_edge_clamp [] http://www.opengl.org/registry/specs/SGIS/texture_edge_clamp.txt
#036 GL_SGIS_texture_border_clamp [] http://www.opengl.org/registry/specs/SGIS/texture_border_clamp.txt
#037 GL_EXT_blend_minmax       [] http://www.opengl.org/registry/specs/EXT/blend_minmax.txt
#038 GL_EXT_blend_subtract     [] http://www.opengl.org/registry/specs/EXT/blend_subtract.txt
#039 GL_EXT_blend_logic_op     [] http://www.opengl.org/registry/specs/EXT/blend_logic_op.txt
#040 GLX_SGI_swap_control      [] http://www.opengl.org/registry/specs/SGI/swap_control.txt
#041 GLX_SGI_video_sync        [] http://www.opengl.org/registry/specs/SGI/video_sync.txt
#042 GLX_SGI_make_current_read [] http://www.opengl.org/registry/specs/SGI/make_current_read.txt
#043 GLX_SGIX_video_source     [] http://www.opengl.org/registry/specs/SGIX/video_source.txt
#044 GLX_EXT_visual_rating     [] http://www.opengl.org/registry/specs/EXT/visual_rating.txt
#045 GL_SGIX_interlace         [] http://www.opengl.org/registry/specs/SGIX/interlace.txt
#047 GLX_EXT_import_context    [] http://www.opengl.org/registry/specs/EXT/import_context.txt
#049 GLX_SGIX_fbconfig         [] http://www.opengl.org/registry/specs/SGIX/fbconfig.txt
#050 GLX_SGIX_pbuffer          [] http://www.opengl.org/registry/specs/SGIX/pbuffer.txt
#051 GL_SGIS_texture_select    [] http://www.opengl.org/registry/specs/SGIS/texture_select.txt
#052 GL_SGIX_sprite            [] http://www.opengl.org/registry/specs/SGIX/sprite.txt
#053 GL_SGIX_texture_multi_buffer [] http://www.opengl.org/registry/specs/SGIX/texture_multi_buffer.txt
#054 GL_EXT_point_parameters   [] http://www.opengl.org/registry/specs/EXT/point_parameters.txt
#055 GL_SGIX_instruments       [] http://www.opengl.org/registry/specs/SGIX/instruments.txt
#056 GL_SGIX_texture_scale_bias [] http://www.opengl.org/registry/specs/SGIX/texture_scale_bias.txt
#057 GL_SGIX_framezoom         [] http://www.opengl.org/registry/specs/SGIX/framezoom.txt
#058 GL_SGIX_tag_sample_buffer [] http://www.opengl.org/registry/specs/SGIX/tag_sample_buffer.txt
#060 GL_SGIX_reference_plane   [] http://www.opengl.org/registry/specs/SGIX/reference_plane.txt
#061 GL_SGIX_flush_raster      [] http://www.opengl.org/registry/specs/SGIX/flush_raster.txt
#062 GLX_SGI_cushion           [] http://www.opengl.org/registry/specs/SGI/cushion.txt
#063 GL_SGIX_depth_texture     [] http://www.opengl.org/registry/specs/SGIX/depth_texture.txt
#064 GL_SGIS_fog_function      [] http://www.opengl.org/registry/specs/SGIS/fog_func.txt
#065 GL_SGIX_fog_offset        [] http://www.opengl.org/registry/specs/SGIX/fog_offset.txt
#066 GL_HP_image_transform     [] http://www.opengl.org/registry/specs/HP/image_transform.txt
#067 GL_HP_convolution_border_modes [] http://www.opengl.org/registry/specs/HP/convolution_border_modes.txt
#069 GL_SGIX_texture_add_env   [] http://www.opengl.org/registry/specs/SGIX/texture_env_add.txt
#074 GL_EXT_color_subtable     [] http://www.opengl.org/registry/specs/EXT/color_subtable.txt
#075 GLU_EXT_object_space_tess [] http://www.opengl.org/registry/specs/EXT/object_space_tess.txt
#076 GL_PGI_vertex_hints       [] http://www.opengl.org/registry/specs/PGI/vertex_hints.txt
#077 GL_PGI_misc_hints         [] http://www.opengl.org/registry/specs/PGI/misc_hints.txt
#078 GL_EXT_paletted_texture   [] http://www.opengl.org/registry/specs/EXT/paletted_texture.txt
#079 GL_EXT_clip_volume_hint   [] http://www.opengl.org/registry/specs/EXT/clip_volume_hint.txt
#080 GL_SGIX_list_priority     [incomplete] http://www.opengl.org/registry/specs/SGIX/list_priority.txt
#081 GL_SGIX_ir_instrument1    [] http://www.opengl.org/registry/specs/SGIX/ir_instrument1.txt
#083 GLX_SGIX_video_resize     [] http://www.opengl.org/registry/specs/SGIX/video_resize.txt
#084 GL_SGIX_texture_lod_bias  [] http://www.opengl.org/registry/specs/SGIX/texture_lod_bias.txt
#085 GLU_SGI_filter4_parameters [incomplete] http://www.opengl.org/registry/specs/SGI/filter4_parameters.txt
#086 GLX_SGIX_dm_buffer        [incomplete] !!!GLX_SGIX_dmbuffer!!! http://www.opengl.org/registry/specs/SGIX/dmbuffer.txt
#090 GL_SGIX_shadow_ambient      [] http://www.opengl.org/registry/specs/SGIX/shadow_ambient.txt
#091 GLX_SGIX_swap_group         [] http://www.opengl.org/registry/specs/SGIX/swap_group.txt
#092 GLX_SGIX_swap_barrier       [] http://www.opengl.org/registry/specs/SGIX/swap_barrier.txt
#093 GL_EXT_index_texture        [] http://www.opengl.org/registry/specs/EXT/index_texture.txt
#094 GL_EXT_index_material       [] http://www.opengl.org/registry/specs/EXT/index_material.txt
#095 GL_EXT_index_func           [] http://www.opengl.org/registry/specs/EXT/index_func.txt
#096 GL_EXT_index_array_formats  [incomplete] http://www.opengl.org/registry/specs/EXT/index_array_formats.txt
#097 GL_EXT_compiled_vertex_array [incomplete] http://www.opengl.org/registry/specs/EXT/compiled_vertex_array.txt
#098 GL_EXT_cull_vertex          [incomplete] http://www.opengl.org/registry/specs/EXT/cull_vertex.txt
#100 GLU_EXT_nurbs_tessellator   [] #100 http://www.opengl.org/registry/specs/EXT/nurbs_tessellator.txt
#101 GL_SGIX_ycrcb               [incomplete] http://www.opengl.org/registry/specs/SGIX/ycrcb.txt
#102 GL_EXT_fragment_lighting    [incomplete] http://www.opengl.org/registry/specs/EXT/fragment_lighting.txt
#110 GL_IBM_rasterpos_clip       [] http://www.opengl.org/registry/specs/IBM/rasterpos_clip.txt
#111 GL_HP_texture_lighting      [incomplete] http://www.opengl.org/registry/specs/HP/texture_lighting.txt
#112 GL_EXT_draw_range_elements  [] http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
#113 GL_WIN_phong_shading        [] http://www.opengl.org/registry/specs/WIN/phong_shading.txt
#114 GL_WIN_specular_fog         [] http://www.opengl.org/registry/specs/WIN/specular_fog.txt
#115 GL_SGIS_color_range         [] GLX_SGIS_color_range http://www.opengl.org/registry/specs/SGIS/color_range.txt
#117 GL_EXT_light_texture        [] http://www.opengl.org/registry/specs/EXT/light_texture.txt
#119 GL_SGIX_blend_alpha_minmax  [incomplete] http://www.opengl.org/registry/specs/SGIX/blend_alpha_minmax.txt
#120 GL_EXT_scene_marker         [] GLX_EXT_scene_marker http://www.opengl.org/registry/specs/EXT/scene_marker.txt
#127 GL_SGIX_pixel_texture_bits  [] http://www.opengl.org/registry/specs/SGIX/pixel_texture_bits.txt
#129 GL_EXT_bgra                 [] http://www.opengl.org/registry/specs/EXT/bgra.txt
#132 GL_SGIX_async               [incomplete] http://www.opengl.org/registry/specs/SGIX/async.txt
#133 GL_SGIX_async_pixel         [inc] http://www.opengl.org/registry/specs/SGIX/async_pixel.txt
#134 GL_SGIX_async_histogram     [inc] http://www.opengl.org/registry/specs/SGIX/async_histogram.txt
#135 GL_INTEL_texture_scissor    [] http://www.opengl.org/registry/specs/INTEL/texture_scissor.txt
#136 GL_INTEL_parallel_arrays    [] http://www.opengl.org/registry/specs/INTEL/parallel_arrays.txt
#137 GL_HP_occlusion_test        inc[] http://www.opengl.org/registry/specs/HP/occlusion_test.txt
#138 GL_EXT_pixel_transform      [] http://www.opengl.org/registry/specs/EXT/pixel_transform.txt
#139 GL_EXT_pixel_transform_color_table [] http://www.opengl.org/registry/specs/EXT/pixel_transform_color_table.txt
#141 GL_EXT_shared_texture_palette [] http://www.opengl.org/registry/specs/EXT/shared_texture_palette.txt
#142 GLX_SGIS_blended_overlay    [] http://www.opengl.org/registry/specs/SGIS/blended_overlay.txt
#144 GL_EXT_separate_specular_color [] http://www.opengl.org/registry/specs/EXT/separate_specular_color.txt
#145 GL_EXT_secondary_color      [] http://www.opengl.org/registry/specs/EXT/secondary_color.txt
#146 GL_EXT_texture_env          [] http://www.opengl.org/registry/specs/EXT/texture_env.txt
#147 GL_EXT_texture_perturb_normal [] http://www.opengl.org/registry/specs/EXT/texture_perturb_normal.txt
#148 GL_EXT_multi_draw_arrays    [] GL_SUN_multi_draw_arrays http://www.opengl.org/registry/specs/EXT/multi_draw_arrays.txt
#149 GL_EXT_fog_coord            [] http://www.opengl.org/registry/specs/EXT/fog_coord.txt
#155 GL_REND_screen_coordinates  [inc] http://www.opengl.org/registry/specs/REND/screen_coordinates.txt
#156 GL_EXT_coordinate_frame     [inc] http://www.opengl.org/registry/specs/EXT/coordinate_frame.txt
#158 GL_EXT_texture_env_combine  [] http://www.opengl.org/registry/specs/EXT/texture_env_combine.txt
#159 GL_APPLE_specular_vector    [inc] http://www.opengl.org/registry/specs/APPLE/specular_vector.txt
#160 GL_APPLE_transform_hint     [inc] http://www.opengl.org/registry/specs/APPLE/transform_hint.txt
#163 GL_SUNX_constant_data       [] http://www.opengl.org/registry/specs/SUNX/constant_data.txt
#164 GL_SUN_global_alpha         [] http://www.opengl.org/registry/specs/SUN/global_alpha.txt
#165 GL_SUN_triangle_list        [] http://www.opengl.org/registry/specs/SUN/triangle_list.txt
#166 GL_SUN_vertex               [] http://www.opengl.org/registry/specs/SUN/vertex.txt
#167 WGL_EXT_display_color_table [inc] http://www.opengl.org/registry/specs/EXT/wgl_display_color_table.txt
#168 WGL_EXT_extensions_string   [] http://www.opengl.org/registry/specs/EXT/wgl_extensions_string.txt
#169 WGL_EXT_make_current_read   [] http://www.opengl.org/registry/specs/EXT/wgl_make_current_read.txt
#170 WGL_EXT_pixel_format        [] http://www.opengl.org/registry/specs/EXT/wgl_pixel_format.txt
#171 WGL_EXT_pbuffer             [] http://www.opengl.org/registry/specs/EXT/wgl_pbuffer.txt
#172 WGL_EXT_swap_control        [] http://www.opengl.org/registry/specs/EXT/wgl_swap_control.txt
#173 GL_EXT_blend_func_separate  [] http://www.opengl.org/registry/specs/EXT/blend_func_separate.txt
#174 GL_INGR_color_clamp         [] http://www.opengl.org/registry/specs/INGR/color_clamp.txt
#175 GL_INGR_interlace_read      [] http://www.opengl.org/registry/specs/INGR/interlace_read.txt
#176 GL_EXT_stencil_wrap         [] http://www.opengl.org/registry/specs/EXT/stencil_wrap.txt
#177 WGL_EXT_depth_float         [] http://www.opengl.org/registry/specs/EXT/wgl_depth_float.txt
#178 GL_EXT_422_pixels           [] http://www.opengl.org/registry/specs/EXT/422_pixels.txt
#179 GL_NV_texgen_reflection     [] http://www.opengl.org/registry/specs/NV/texgen_reflection.txt
#181 GL_SGIX_texture_range       [] http://www.opengl.org/registry/specs/SGIX/texture_range.txt
#182 GL_SUN_convolution_border_modes [] http://www.opengl.org/registry/specs/SUN/convolution_border_modes.txt
#183 GLX_SUN_get_transparent_index [] http://www.opengl.org/registry/specs/SUN/get_transparent_index.txt
#185 GL_EXT_texture_env_add      [] http://www.opengl.org/registry/specs/EXT/texture_env_add.txt
#186 GL_EXT_texture_lod_bias     [] http://www.opengl.org/registry/specs/EXT/texture_lod_bias.txt
#187 GL_EXT_texture_filter_anisotropic [] http://www.opengl.org/registry/specs/EXT/texture_filter_anisotropic.txt
#188 GL_EXT_vertex_weighting     [] http://www.opengl.org/registry/specs/EXT/vertex_weighting.txt
#189 GL_NV_light_max_exponent    [] http://www.opengl.org/registry/specs/NV/light_max_exponent.txt
#190 GL_NV_vertex_array_range    [] http://www.opengl.org/registry/specs/NV/vertex_array_range.txt
#191 GL_NV_register_combiners    [] http://www.opengl.org/registry/specs/NV/register_combiners.txt
#192 GL_NV_fog_distance          [] http://www.opengl.org/registry/specs/NV/fog_distance.txt
#193 GL_NV_texgen_emboss         [] http://www.opengl.org/registry/specs/NV/texgen_emboss.txt
#194 GL_NV_blend_square          [] http://www.opengl.org/registry/specs/NV/blend_square.txt
#195 GL_NV_texture_env_combine4  [] http://www.opengl.org/registry/specs/NV/texture_env_combine4.txt
#196 GL_MESA_resize_buffers      [] http://www.opengl.org/registry/specs/MESA/resize_buffers.txt
#197 GL_MESA_window_pos          [] http://www.opengl.org/registry/specs/MESA/window_pos.txt
#198 GL_EXT_texture_compression_s3tc [] http://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt
#199 GL_IBM_cull_vertex          [] http://www.opengl.org/registry/specs/IBM/cull_vertex.txt
#200 GL_IBM_multimode_draw_arrays [] http://www.opengl.org/registry/specs/IBM/multimode_draw_arrays.txt
#201 GL_IBM_vertex_array_lists   [] http://www.opengl.org/registry/specs/IBM/vertex_array_lists.txt
#206 GL_3DFX_texture_compression_FXT1 [] http://www.opengl.org/registry/specs/3DFX/texture_compression_FXT1.txt
#207 GL_3DFX_multisample         [] GLX_3DFX_multisample WGL_3DFX_multisample http://www.opengl.org/registry/specs/3DFX/3dfx_multisample.txt
#208 GL_3DFX_tbuffer             [] http://www.opengl.org/registry/specs/3DFX/tbuffer.txt
#209 GL_EXT_multisample          [] WGL_EXT_multisample http://www.opengl.org/registry/specs/EXT/wgl_multisample.txt
#210 GL_SGIX_vertex_preclip      [] GL_SGIX_vertex_preclip_hint http://www.opengl.org/registry/specs/SGIX/vertex_preclip.txt
#211 GL_SGIX_convolution_accuracy [inc] http://www.opengl.org/registry/specs/SGIX/convolution_accuracy.txt
#212 GL_SGIX_resample            [inc] http://www.opengl.org/registry/specs/SGIX/resample.txt
#213 GL_SGIS_point_line_texgen   [] http://www.opengl.org/registry/specs/SGIS/point_line_texgen.txt
#214 GL_SGIS_texture_color_mask  [] http://www.opengl.org/registry/specs/SGIS/texture_color_mask.txt
#215 GLX_MESA_copy_sub_buffer    [] http://www.opengl.org/registry/specs/MESA/copy_sub_buffer.txt
#216 GLX_MESA_pixmap_colormap    [] http://www.opengl.org/registry/specs/MESA/pixmap_colormap.txt
#217 GLX_MESA_release_buffers    [] http://www.opengl.org/registry/specs/MESA/release_buffers.txt
#218 GLX_MESA_set_3dfx_mode      [] http://www.opengl.org/registry/specs/MESA/set_3dfx_mode.txt
#220 GL_EXT_texture_env_dot3     [] http://www.opengl.org/registry/specs/EXT/texture_env_dot3.txt
#221 GL_ATI_texture_mirror_once  [] http://www.opengl.org/registry/specs/ATI/texture_mirror_once.txt
#222 GL_NV_fence                 [] http://www.opengl.org/registry/specs/NV/fence.txt
#223 GL_IBM_static_data          [] http://www.opengl.org/registry/specs/IBM/static_data.txt
#224 GL_IBM_texture_mirrored_repeat [] http://www.opengl.org/registry/specs/IBM/texture_mirrored_repeat.txt
#225 GL_NV_evaluators            [] http://www.opengl.org/registry/specs/NV/evaluators.txt
#226 GL_NV_packed_depth_stencil  [] http://www.opengl.org/registry/specs/NV/packed_depth_stencil.txt
#227 GL_NV_register_combiners2   [] http://www.opengl.org/registry/specs/NV/register_combiners2.txt
#228 GL_NV_texture_compression_vtc [] http://www.opengl.org/registry/specs/NV/texture_compression_vtc.txt
#229 GL_NV_texture_rectangle     [] http://www.opengl.org/registry/specs/NV/texture_rectangle.txt
#230 GL_NV_texture_shader        [] http://www.opengl.org/registry/specs/NV/texture_shader.txt
#231 GL_NV_texture_shader2       [] http://www.opengl.org/registry/specs/NV/texture_shader2.txt
#232 GL_NV_vertex_array_range2   [] http://www.opengl.org/registry/specs/NV/vertex_array_range2.txt
#233 GL_NV_vertex_program        [] http://www.opengl.org/registry/specs/NV/vertex_program.txt
#234 GLX_SGIX_visual_select_group [] http://www.opengl.org/registry/specs/SGIX/visual_select_group.txt
#235 GL_SGIX_texture_coordinate_clamp [] http://www.opengl.org/registry/specs/SGIX/texture_coordinate_clamp.txt
#237 GLX_OML_swap_method         [] http://www.opengl.org/registry/specs/OML/glx_swap_method.txt
#238 GLX_OML_sync_control        [] http://www.opengl.org/registry/specs/OML/glx_sync_control.txt
#239 GL_OML_interlace            [] http://www.opengl.org/registry/specs/OML/interlace.txt
#240 GL_OML_subsample            [] http://www.opengl.org/registry/specs/OML/subsample.txt
#241 GL_OML_resample             [] http://www.opengl.org/registry/specs/OML/resample.txt
#242 WGL_OML_sync_control        [] http://www.opengl.org/registry/specs/OML/wgl_sync_control.txt
#243 GL_NV_copy_depth_to_color   [] http://www.opengl.org/registry/specs/NV/copy_depth_to_color.txt
#244 GL_ATI_envmap_bumpmap       [] http://www.opengl.org/registry/specs/ATI/envmap_bumpmap.txt
#245 GL_ATI_fragment_shader      [] http://www.opengl.org/registry/specs/ATI/fragment_shader.txt
#246 GL_ATI_pn_triangles         [] http://www.opengl.org/registry/specs/ATI/pn_triangles.txt
#247 GL_ATI_vertex_array_object  [] http://www.opengl.org/registry/specs/ATI/vertex_array_object.txt
#248 GL_EXT_vertex_shader        [] http://www.opengl.org/registry/specs/EXT/vertex_shader.txt
#249 GL_ATI_vertex_streams       [] http://www.opengl.org/registry/specs/ATI/vertex_streams.txt
#250 WGL_I3D_digital_video_control [] http://www.opengl.org/registry/specs/I3D/wgl_digital_video_control.txt
#251 WGL_I3D_gamma               [] http://www.opengl.org/registry/specs/I3D/wgl_gamma.txt
#252 WGL_I3D_genlock             [] http://www.opengl.org/registry/specs/I3D/wgl_genlock.txt
#253 WGL_I3D_image_buffer        [] http://www.opengl.org/registry/specs/I3D/wgl_image_buffer.txt
#254 WGL_I3D_swap_frame_lock     [] http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_lock.txt
#255 WGL_I3D_swap_frame_usage    [] http://www.opengl.org/registry/specs/I3D/wgl_swap_frame_usage.txt
#256 GL_ATI_element_array        [] http://www.opengl.org/registry/specs/ATI/element_array.txt
#257 GL_SUN_mesh_array           [] http://www.opengl.org/registry/specs/SUN/mesh_array.txt
#258 GL_SUN_slice_accum          [] http://www.opengl.org/registry/specs/SUN/slice_accum.txt
#259 GL_NV_multisample_filter_hint [] http://www.opengl.org/registry/specs/NV/multisample_filter_hint.txt
#260 GL_NV_depth_clamp           [] http://www.opengl.org/registry/specs/NV/depth_clamp.txt
#261 GL_NV_occlusion_query       [] http://www.opengl.org/registry/specs/NV/occlusion_query.txt
#262 GL_NV_point_sprite          [] http://www.opengl.org/registry/specs/NV/point_sprite.txt
#263 WGL_NV_render_depth_texture [] http://www.opengl.org/registry/specs/NV/render_depth_texture.txt
#264 WGL_NV_render_texture_rectangle [] http://www.opengl.org/registry/specs/NV/render_texture_rectangle.txt
#265 GL_NV_texture_shader3       [] http://www.opengl.org/registry/specs/NV/texture_shader3.txt
#266 GL_NV_vertex_program1_1     [] http://www.opengl.org/registry/specs/NV/texture_shader3.txt
#267 GL_EXT_shadow_funcs         [] http://www.opengl.org/registry/specs/EXT/shadow_funcs.txt
#268 GL_EXT_stencil_two_side     [] http://www.opengl.org/registry/specs/EXT/stencil_two_side.txt
#269 GL_ATI_text_fragment_shader [] http://www.opengl.org/registry/specs/ATI/text_fragment_shader.txt
#270 GL_APPLE_client_storage     [] http://www.opengl.org/registry/specs/APPLE/client_storage.txt
#271 GL_APPLE_element_array      [] http://www.opengl.org/registry/specs/APPLE/element_array.txt
#272 GL_APPLE_fence              [] http://www.opengl.org/registry/specs/APPLE/fence.txt
#273 GL_APPLE_vertex_array_object [] http://www.opengl.org/registry/specs/APPLE/vertex_array_object.txt
#274 GL_APPLE_vertex_array_range [] http://www.opengl.org/registry/specs/APPLE/vertex_array_range.txt
#275 GL_APPLE_ycbcr_422          [] http://www.opengl.org/registry/specs/APPLE/ycbcr_422.txt
#276 GL_S3_s3tc                  [inc] http://www.opengl.org/registry/specs/S3/s3tc.txt
#277 GL_ATI_draw_buffers         [] http://www.opengl.org/registry/specs/ATI/draw_buffers.txt
#278 WGL_ATI_pixel_format_float  [] http://www.opengl.org/registry/specs/ATI/pixel_format_float.txt
#279 GL_ATI_texture_env_combine3 [] http://www.opengl.org/registry/specs/ATI/texture_env_combine3.txt
#280 GL_ATI_texture_float        [] http://www.opengl.org/registry/specs/ATI/texture_float.txt
#281 GL_NV_float_buffer          [] WGL_NV_float_buffer GLX_NV_float_buffer http://www.opengl.org/registry/specs/NV/float_buffer.txt
#282 GL_NV_fragment_program      [] http://www.opengl.org/registry/specs/NV/fragment_program.txt
#283 GL_NV_half_float            [] http://www.opengl.org/registry/specs/NV/half_float.txt
#284 GL_NV_pixel_data_range      [] http://www.opengl.org/registry/specs/NV/pixel_data_range.txt
#285 GL_NV_primitive_restart     [] http://www.opengl.org/registry/specs/NV/primitive_restart.txt
#286 GL_NV_texture_expand_normal [] http://www.opengl.org/registry/specs/NV/texture_expand_normal.txt
#287 GL_NV_vertex_program2       [] http://www.opengl.org/registry/specs/NV/vertex_program2.txt
#288 GL_ATI_map_object_buffer    [] http://www.opengl.org/registry/specs/ATI/map_object_buffer.txt
#289 GL_ATI_separate_stencil     [] http://www.opengl.org/registry/specs/ATI/separate_stencil.txt
#290 GL_ATI_vertex_attrib_array_object [] http://www.opengl.org/registry/specs/ATI/vertex_attrib_array_object.txt
#291 GL_OES_byte_coordinates     [] http://www.opengl.org/registry/specs/OES/OES_byte_coordinates.txt
#292 GL_OES_fixed_point          [] http://www.opengl.org/registry/specs/OES/OES_fixed_point.txt
#293 GL_OES_single_precision     [] http://www.opengl.org/registry/specs/OES/OES_single_precision.txt
#294 GL_OES_compressed_paletted_texture [] http://www.opengl.org/registry/specs/OES/OES_compressed_paletted_texture.txt
#295 GL_OES_read_format          [] http://www.opengl.org/registry/specs/OES/OES_read_format.txt
#296 GL_OES_query_matrix         [] http://www.opengl.org/registry/specs/OES/OES_query_matrix.txt
#297 GL_EXT_depth_bounds_test    [] http://www.opengl.org/registry/specs/EXT/depth_bounds_test.txt
#298 GL_EXT_texture_mirror_clamp [] http://www.opengl.org/registry/specs/EXT/texture_mirror_clamp.txt
#299 GL_EXT_blend_equation_separate [] http://www.opengl.org/registry/specs/EXT/blend_equation_separate.txt
#300 GL_MESA_pack_invert         [] http://www.opengl.org/registry/specs/MESA/pack_invert.txt
#301 GL_MESA_ycbcr_texture       [] http://www.opengl.org/registry/specs/MESA/ycbcr_texture.txt
#302 GL_EXT_pixel_buffer_object  [] http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt
#303 GL_NV_fragment_program_option [] http://www.opengl.org/registry/specs/NV/fragment_program_option.txt
#304 GL_NV_fragment_program2     [] http://www.opengl.org/registry/specs/NV/fragment_program2.txt
#305 GL_NV_vertex_program2_option [] http://www.opengl.org/registry/specs/NV/vertex_program2_option.txt
#306 GL_NV_vertex_program3       [] http://www.opengl.org/registry/specs/NV/vertex_program3.txt
#307 GLX_SGIX_hyperpipe          [] http://www.opengl.org/registry/specs/SGIX/hyperpipe_group.txt
#308 GLX_MESA_agp_offset         [] http://www.opengl.org/registry/specs/MESA/agp_offset.txt
#309 GL_EXT_texture_compression_dxt1 [] http://www.opengl.org/registry/specs/EXT/texture_compression_dxt1.txt
#310 GL_EXT_framebuffer_object   [] http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
#311 GL_GREMEDY_string_marker    [] http://www.opengl.org/registry/specs/GREMEDY/string_marker.txt
#312 GL_EXT_packed_depth_stencil [] http://www.opengl.org/registry/specs/EXT/packed_depth_stencil.txt
#313 WGL_3DL_stereo_control      [] http://www.opengl.org/registry/specs/3DL/stereo_control.txt
#314 GL_EXT_stencil_clear_tag    [] http://www.opengl.org/registry/specs/EXT/stencil_clear_tag.txt
#315 GL_EXT_texture_sRGB         [] http://www.opengl.org/registry/specs/EXT/texture_sRGB.txt
#316 GL_EXT_framebuffer_blit     [] http://www.opengl.org/registry/specs/EXT/framebuffer_blit.txt
#317 GL_EXT_framebuffer_multisample [] http://www.opengl.org/registry/specs/EXT/framebuffer_multisample.txt
#318 GL_MESAX_texture_stack      [] http://www.opengl.org/registry/specs/MESAX/texture_stack.txt
#319 GL_EXT_timer_query          [] http://www.opengl.org/registry/specs/EXT/timer_query.txt
#320 GL_EXT_gpu_program_parameters [] http://www.opengl.org/registry/specs/EXT/gpu_program_parameters.txt
#321 GL_APPLE_flush_buffer_range [] http://www.opengl.org/registry/specs/APPLE/flush_buffer_range.txt
#322 GL_NV_gpu_program4          [] http://www.opengl.org/registry/specs/NV/gpu_program4.txt
#323 GL_NV_geometry_program4     [] http://www.opengl.org/registry/specs/NV/geometry_program4.txt
#324 GL_EXT_geometry_shader4     [] http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
#325 GL_NV_vertex_program4       [] http://www.opengl.org/registry/specs/NV/vertex_program4.txt
#326 GL_EXT_gpu_shader4          [] http://www.opengl.org/registry/specs/EXT/gpu_shader4.txt
#327 GL_EXT_draw_instanced       [] http://www.opengl.org/registry/specs/EXT/draw_instanced.txt
#328 GL_EXT_packed_float         [] WGL_EXT_pixel_format_packed_float GLX_EXT_fbconfig_packed_float http://www.opengl.org/registry/specs/EXT/packed_float.txt
#329 GL_EXT_texture_array        [] http://www.opengl.org/registry/specs/EXT/texture_array.txt
#330 GL_EXT_texture_buffer_object [] http://www.opengl.org/registry/specs/EXT/texture_buffer_object.txt
#331 GL_EXT_texture_compression_latc [] GL_NV_texture_compression_latc (legacy) http://www.opengl.org/registry/specs/EXT/texture_compression_latc.txt
#332 GL_EXT_texture_compression_rgtc [] http://www.opengl.org/registry/specs/EXT/texture_compression_rgtc.txt
#333 GL_EXT_texture_shared_exponent [] http://www.opengl.org/registry/specs/EXT/texture_shared_exponent.txt
#334 GL_NV_depth_buffer_float    [] http://www.opengl.org/registry/specs/NV/depth_buffer_float.txt
#335 GL_NV_fragment_program4     [] http://www.opengl.org/registry/specs/NV/fragment_program4.txt
#336 GL_NV_framebuffer_multisample_coverage [] http://www.opengl.org/registry/specs/NV/framebuffer_multisample_coverage.txt
#337 GL_EXT_framebuffer_sRGB     [] GLX_EXT_framebuffer_sRGB WGL_EXT_framebuffer_sRGB http://www.opengl.org/registry/specs/EXT/framebuffer_sRGB.txt
#338 GL_NV_geometry_shader4      [] http://www.opengl.org/registry/specs/NV/geometry_shader4.txt
#339 GL_NV_parameter_buffer_object [] http://www.opengl.org/registry/specs/NV/parameter_buffer_object.txt
#340 GL_EXT_draw_buffers2        [] http://www.opengl.org/registry/specs/EXT/draw_buffers2.txt
#341 GL_NV_transform_feedback    [] http://www.opengl.org/registry/specs/NV/transform_feedback.txt
#342 GL_EXT_bindable_uniform     [] http://www.opengl.org/registry/specs/EXT/bindable_uniform.txt
#343 GL_EXT_texture_integer      [] http://www.opengl.org/registry/specs/EXT/texture_integer.txt
#344 GLX_EXT_texture_from_pixmap [] http://www.opengl.org/registry/specs/EXT/texture_from_pixmap.txt
#345 GL_GREMEDY_frame_terminator [] http://www.opengl.org/registry/specs/GREMEDY/frame_terminator.txt
#346 GL_NV_conditional_render    [] http://www.opengl.org/registry/specs/NV/conditional_render.txt
#347 GL_NV_present_video         [] GLX_NV_present_video WGL_NV_present_video http://www.opengl.org/registry/specs/NV/present_video.txt
#348 GLX_NV_video_out            [] http://www.opengl.org/registry/specs/NV/glx_video_output.txt
#349 WGL_NV_video_output         [] http://www.opengl.org/registry/specs/NV/wgl_video_output.txt
#350 GLX_NV_swap_group           [] http://www.opengl.org/registry/specs/NV/glx_swap_group.txt
#351 WGL_NV_swap_group           [] http://www.opengl.org/registry/specs/NV/wgl_swap_group.txt
#352 GL_EXT_transform_feedback   [] http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
#353 GL_EXT_direct_state_access  [] http://www.opengl.org/registry/specs/EXT/direct_state_access.txt
#354 GL_EXT_vertex_array_bgra    [] http://www.opengl.org/registry/specs/EXT/vertex_array_bgra.txt
#355 WGL_NV_gpu_affinity         [] http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
#356 GL_EXT_texture_swizzle      [] http://www.opengl.org/registry/specs/EXT/texture_swizzle.txt
#357 GL_NV_explicit_multisample  [] http://www.opengl.org/registry/specs/NV/explicit_multisample.txt
#358 GL_NV_transform_feedback2   [] http://www.opengl.org/registry/specs/NV/transform_feedback2.txt
#359 GL_ATI_meminfo              [] http://www.opengl.org/registry/specs/ATI/meminfo.txt
#360 GL_AMD_performance_monitor  [] http://www.opengl.org/registry/specs/AMD/performance_monitor.txt
#361 WGL_AMD_gpu_association     [] http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
#362 GL_AMD_texture_texture4     [] http://www.opengl.org/registry/specs/AMD/texture_texture4.txt
#363 GL_AMD_vertex_shader_tessellator [] http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt
#364 GL_EXT_provoking_vertex     [] http://www.opengl.org/registry/specs/EXT/provoking_vertex.txt
#365 GL_EXT_texture_snorm        [] http://www.opengl.org/registry/specs/EXT/texture_snorm.txt
#366 GL_AMD_draw_buffers_blend   [] http://www.opengl.org/registry/specs/AMD/draw_buffers_blend.txt
#367 GL_APPLE_texture_range      [] http://www.opengl.org/registry/specs/APPLE/texture_range.txt
#368 GL_APPLE_float_pixels       [] http://www.opengl.org/registry/specs/APPLE/float_pixels.txt
#369 GL_APPLE_vertex_program_evaluators [] http://www.opengl.org/registry/specs/APPLE/vertex_program_evaluators.txt
#370 GL_APPLE_aux_depth_stencil  [] http://www.opengl.org/registry/specs/APPLE/aux_depth_stencil.txt
#371 GL_APPLE_object_purgeable   [] http://www.opengl.org/registry/specs/APPLE/object_purgeable.txt
#372 GL_APPLE_row_bytes          [] http://www.opengl.org/registry/specs/APPLE/row_bytes.txt
#373 GL_APPLE_rgb_422            [] http://www.opengl.org/registry/specs/APPLE/rgb_422.txt
#374 GL_NV_video_capture         [] GLX_NV_video_capture WGL_NV_video_capture http://www.opengl.org/registry/specs/NV/video_capture.txt
#375 GLX_EXT_swap_control        [] http://www.opengl.org/registry/specs/EXT/swap_control.txt
#376 GL_NV_copy_image            [] WGL_NV_copy_image GLX_NV_copy_image http://www.opengl.org/registry/specs/NV/copy_image.txt
#377 GL_EXT_separate_shader_objects [] http://www.opengl.org/registry/specs/EXT/separate_shader_objects.txt
#378 GL_NV_parameter_buffer_object2 [] http://www.opengl.org/registry/specs/NV/parameter_buffer_object2.txt
#379 GL_NV_shader_buffer_load    [] http://www.opengl.org/registry/specs/NV/shader_buffer_load.txt
#380 GL_NV_vertex_buffer_unified_memory [] http://www.opengl.org/registry/specs/NV/vertex_buffer_unified_memory.txt
#381 GL_NV_texture_barrier       [] http://www.opengl.org/registry/specs/NV/texture_barrier.txt
#382 GL_AMD_shader_stencil_export [] http://www.opengl.org/registry/specs/AMD/shader_stencil_export.txt
#383 GL_AMD_seamless_cubemap_per_texture [] http://www.opengl.org/registry/specs/AMD/seamless_cubemap_per_texture.txt
#384 GLX_INTEL_swap_event        [] http://www.opengl.org/registry/specs/INTEL/swap_event.txt
#385 GL_AMD_conservative_depth   [] http://www.opengl.org/registry/specs/AMD/conservative_depth.txt
#386 GL_EXT_shader_image_load_store [] http://www.opengl.org/registry/specs/EXT/shader_image_load_store.txt
#387 GL_EXT_vertex_attrib_64bit  [] http://www.opengl.org/registry/specs/EXT/vertex_attrib_64bit.txt
#388 GL_NV_gpu_program5          [] GL_NV_gpu_program_fp64 http://www.opengl.org/registry/specs/NV/gpu_program5.txt
#389 GL_NV_gpu_shader5           [] http://www.opengl.org/registry/specs/NV/gpu_shader5.txt
#390 GL_NV_shader_buffer_store   [] http://www.opengl.org/registry/specs/NV/shader_buffer_store.txt
#391 GL_NV_tessellation_program5 [] http://www.opengl.org/registry/specs/NV/tessellation_program5.txt
#392 GL_NV_vertex_attrib_integer_64bit [] http://www.opengl.org/registry/specs/NV/vertex_attrib_integer_64bit.txt
#393 GL_NV_multisample_coverage  [] GLX_NV_multisample_coverage WGL_NV_multisample_coverage http://www.opengl.org/registry/specs/NV/multisample_coverage.txt
#394 GL_AMD_name_gen_delete      [] http://www.opengl.org/registry/specs/AMD/name_gen_delete.txt
#395 GL_AMD_debug_output         [] http://www.opengl.org/registry/specs/AMD/debug_output.txt
#396 GL_NV_vdpau_interop         [] http://www.opengl.org/registry/specs/NV/vdpau_interop.txt
#397 GL_AMD_transform_feedback3_lines_triangles [] http://www.opengl.org/registry/specs/AMD/transform_feedback3_lines_triangles.txt
#398 GLX_AMD_gpu_association     [] http://www.opengl.org/registry/specs/AMD/glx_gpu_association.txt
#399 GLX_EXT_create_context_es_profile [] GLX_EXT_create_context_es2_profile http://www.opengl.org/registry/specs/EXT/glx_create_context_es2_profile.txt
#400 WGL_EXT_create_context_es_profile [] WGL_EXT_create_context_es2_profile http://www.opengl.org/registry/specs/EXT/wgl_create_context_es2_profile.txt
#401 GL_AMD_depth_clamp_separate [] http://www.opengl.org/registry/specs/AMD/depth_clamp_separate.txt
#402 GL_EXT_texture_sRGB_decode  [] http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
#403 GL_NV_texture_multisample   [] GL_ missing in registry, asuming mistake http://www.opengl.org/registry/specs/NV/texture_multisample.txt
#404 GL_AMD_blend_minmax_factor  [] http://www.opengl.org/registry/specs/AMD/blend_minmax_factor.txt
#405 GL_AMD_sample_positions     [] http://www.opengl.org/registry/specs/AMD/sample_positions.txt
#406 GL_EXT_x11_sync_object      [] http://www.opengl.org/registry/specs/EXT/x11_sync_object.txt
#407 WGL_NV_DX_interop           [] http://www.opengl.org/registry/specs/NV/DX_interop.txt
#408 GL_AMD_multi_draw_indirect  [] http://www.opengl.org/registry/specs/AMD/multi_draw_indirect.txt
#409 GL_EXT_framebuffer_multisample_blit_scaled [] http://www.opengl.org/registry/specs/EXT/framebuffer_multisample_blit_scaled.txt
#410 GL_NV_path_rendering        [] http://www.opengl.org/registry/specs/NV/path_rendering.txt
#411 GL_AMD_pinned_memory        [] http://www.opengl.org/registry/specs/AMD/pinned_memory.txt
#412 WGL_NV_DX_interop2          [] http://www.opengl.org/registry/specs/NV/DX_interop2.txt
#413 GL_AMD_stencil_operation_extended [] http://www.opengl.org/registry/specs/AMD/stencil_operation_extended.txt
#414 GLX_EXT_swap_control_tear   [] http://www.opengl.org/registry/specs/EXT/glx_swap_control_tear.txt
#415 WGL_EXT_swap_control_tear   [] http://www.opengl.org/registry/specs/EXT/wgl_swap_control_tear.txt
#416 GL_AMD_vertex_shader_viewport_index [] http://www.opengl.org/registry/specs/AMD/vertex_shader_viewport_index.txt
#417 GL_AMD_vertex_shader_layer  [] http://www.opengl.org/registry/specs/AMD/vertex_shader_layer.txt
#418 GL_NV_bindless_texture      [] http://www.opengl.org/registry/specs/NV/bindless_texture.txt
#419 GL_NV_shader_atomic_float   [] http://www.opengl.org/registry/specs/NV/shader_atomic_float.txt
#420 GL_AMD_query_buffer_object  [] http://www.opengl.org/registry/specs/AMD/query_buffer_object.txt
#421 GL_NV_compute_program5      [] http://www.opengl.org/registry/specs/NV/compute_program5.txt
#422 GL_NV_shader_storage_buffer_object [] http://www.opengl.org/registry/specs/NV/shader_storage_buffer_object.txt
#423 GL_NV_shader_atomic_counters [] http://www.opengl.org/registry/specs/NV/shader_atomic_counters.txt
#424 GL_NV_deep_texture3D        [] http://www.opengl.org/registry/specs/NV/deep_texture3D.txt
#425 GL_NVX_conditional_render   [] http://www.opengl.org/registry/specs/NVX/nvx_conditional_render.txt
#426 GL_AMD_sparse_texture       [] http://www.opengl.org/registry/specs/AMD/sparse_texture.txt
#427 GLX_EXT_buffer_age          [] http://www.opengl.org/registry/specs/EXT/glx_buffer_age.txt
#428 GL_AMD_shader_trinary_minmax [] http://www.opengl.org/registry/specs/AMD/shader_trinary_minmax.txt
#429 GL_INTEL_map_texture        [] http://www.opengl.org/registry/specs/INTEL/map_texture.txt
#430 GL_NV_draw_texture          [] http://www.opengl.org/registry/specs/NV/draw_texture.txt
#431 GL_AMD_interleaved_elements [] http://www.opengl.org/registry/specs/AMD/interleaved_elements.txt
#432 GL_NV_bindless_multi_draw_indirect [] http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect.txt
#433 GL_NV_blend_equation_advanced [] GL_NV_blend_equation_advanced_coherent http://www.opengl.org/registry/specs/NV/blend_equation_advanced.txt
#434 GL_NV_gpu_program5_mem_extended [] http://www.opengl.org/registry/specs/NV/gpu_program5_mem_extended.txt
#435 GL_AMD_shader_atomic_counter_ops [] http://www.opengl.org/registry/specs/AMD/shader_atomic_counter_ops.txt
#436 WGL_NV_delay_before_swap    [] http://www.opengl.org/registry/specs/NV/wgl_delay_before_swap.txt
#437 GL_EXT_shader_integer_mix   [] http://www.opengl.org/registry/specs/EXT/shader_integer_mix.txt
#438 GL_NVX_gpu_memory_info      [] http://www.opengl.org/registry/specs/NVX/gpu_memory_info.txt
#439 GL_EXT_debug_label          [] http://www.opengl.org/registry/specs/EXT/EXT_debug_label.txt
#440 GL_EXT_debug_marker         [] http://www.opengl.org/registry/specs/EXT/EXT_debug_marker.txt
#441 GL_INTEL_fragment_shader_ordering [] http://www.opengl.org/registry/specs/INTEL/fragment_shader_ordering.txt
#442 GL_AMD_occlusion_query_event [] http://www.opengl.org/registry/specs/AMD/occlusion_query_event.txt
#443 GL_INTEL_performance_query  [] http://www.opengl.org/registry/specs/INTEL/performance_query.txt
#444 GL_AMD_shader_stencil_value_export [] http://www.opengl.org/registry/specs/AMD/shader_stencil_value_export.txt
#445 GLX_NV_delay_before_swap    [] http://www.opengl.org/registry/specs/NV/glx_delay_before_swap.txt
#446 GLX_MESA_query_renderer     [] http://www.opengl.org/registry/specs/MESA/glx_query_renderer.txt
#447 GL_NV_shader_thread_group   [] http://www.opengl.org/registry/specs/NV/shader_thread_group.txt
#448 GL_NV_shader_thread_shuffle [] http://www.opengl.org/registry/specs/NV/shader_thread_shuffle.txt
#449 GL_EXT_shader_image_load_formatted [] http://www.opengl.org/registry/specs/EXT/shader_image_load_formatted.txt
#450 GL_AMD_transform_feedback4  [] http://www.opengl.org/registry/specs/AMD/transform_feedback4.txt
#451 GL_AMD_gpu_shader_int64     [] http://www.opengl.org/registry/specs/AMD/gpu_shader_int64.txt
#452 GLX_EXT_stereo_tree         [] http://www.opengl.org/registry/specs/EXT/glx_stereo_tree.txt
#453 GL_AMD_gcn_shader           [] http://www.opengl.org/registry/specs/AMD/gcn_shader.txt
#455 GL_NV_shader_atomic_int64   [] http://www.opengl.org/registry/specs/NV/shader_atomic_int64.txt
#456 GL_NV_bindless_multi_draw_indirect_count [] http://www.opengl.org/registry/specs/NV/bindless_multi_draw_indirect_count.txt
#457 GLX_NV_copy_buffer          [] http://www.opengl.org/registry/specs/NV/glx_copy_buffer.txt
#458 GL_KHR_blend_equation_advanced [] GL_KHR_blend_equation_advanced_coherent http://www.opengl.org/registry/specs/KHR/blend_equation_advanced.txt
  

*/

















