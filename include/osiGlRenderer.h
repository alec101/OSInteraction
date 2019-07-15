#pragma once
#ifdef OSI_USE_OPENGL
#include "osiRenderer.h"


///=========================================================///
// GLRENDERER class - manages OpenGL contexts and extensions //
///=========================================================///



struct osiGlExtFuncs;

class osiGlRenderer: public osiRenderer {
public:

  bool isActive;           // a simple check to see if it is already active when calling glMakeCurrent() is WAAAY faster than switching contexts... 
  //osiMonitor *monitor;    /// the monitor on which the renderer was created. assigning a renderer to render to a different monitor that is handled by a different GPU, can be bad
  // osiGPU ?!!?!?!? this should be a thing i think

  // renderer / graphics card parameters

  str8 glVersion;             /// OpenGL version utf-8 format
  int glVerMajor, glVerMinor; /// OpenGL version integer format
  str8 glVendor;              /// this can be used for various checks, as it never changes
  str8 glRenderer;            /// this can be used for various checks, as it never changes
  int max3Dtexture;           /// maximum 3D texture size this renderer can operate
  GLint maxTexelUnits;        /// maximum texel units this renderer can operate
  int maxTextureAnisotropy;   /// maximum texture anisotropy (if extension is present)

  // OpenGL extensions - these are defined in osiGlExt.cpp - by default, all extensions are auto-checked and aquired
  
  void checkExt();                     // checks all all extension availability and populates glExtList[] array - MUST BE CALLED BEFORE aquiring extension functions
  void getExtFuncs();                 /// gets all oGL extensions funcs for this renderer; renderer MUST be active-> glMakeCurrent (no problems if called multiple times)
  bool isExtensionARB(const char *);  /// returns the availability of selected ARB extension
  bool isExtensionEXT(const char *);  /// returns the availability of selected EXT extension (vendor extensions too - NV/ATI/etc)
  bool isExtensionOTHER(const char *);/// returns the availability of selected extension that is not catalogued in ARB or EXT (there's a few)
  osiGlExt *glARBlist;                /// all openGL ARB extensions list and their avaibility on this renderer
  osiGlExt *glEXTlist;                /// all openGL EXT(and the rest) extensions list and their avaibility on this renderer
  osiGlExt *glOTHERlist;              /// all openGL extensions that are not listed anywhere

  // ogl context and a struct with all extension funcs

  #ifdef OS_WIN
  HGLRC glContext;         // ze thing needed - destructor deletes oGL render context if it is created
  osiGlExtFuncs glExt;    /// each renderer has it's own oGL extensions functions, under windows
  #endif /// OS_WIN

  #ifdef OS_LINUX
  GLXContext glContext;    // oGL rendering context
  osiGlExtFuncs &glExt;   /// reference to a global struct that holds all extension functions
  #endif /// OS_LINUX

  #ifdef OS_MAC
  void *glContext;        /// NSOpenGLContext object (which wraps a CGL renderer which wraps the context and all these are wrapped by osiRenderer. Do you even wrap?)
  osiGlExtFuncs &glExt;   /// NOT USED
  #endif

  // constructor, destructor

  osiGlRenderer();
  ~osiGlRenderer();
  void delData();

private:
  
  //friend bool _getContextFuncs(osiWindow *, osiGlRenderer *);
  bool _bContextFuncsGot; // wip

  #ifdef OS_MAC
  friend class osiCocoa;
  void *_pixelFormat;     /// NSOpenGLPixelFomat object
  #endif
};


// VERTEX ARRAY OBJECT CLASS that creates vertex arrays on all renderers - VAOs are not context shared
///===================================================================================================///
// this class is very slow to initialize the VAOs, but it's ok when binding the VAOs,
// therefore, if the VAO is constant/static/nothing is changing in it's attribs (enable/disable of vertex arrays, pointers, etc),
// this class is ok to use and it _handles multiple contexts_

extern int _VAOrenderer;
GLAPI void APIENTRY glBindVertexArray (GLuint array);

class glVAO {
public:
  unsigned int *id;

  void genArray();           /// same as glGenVertexArrays(..) but for one array only - VERY SLOW - switches contexts if there is more than one renderer active but returns to curent context
  void delArray();           /// same as glDeleteVertexArrays(..) but deletes all arrays in this object - VERY SLOW - switches contexts if there is more than one renderer active but returns to curent context

  void bindAndVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);  /// VERY SLOW - switches contexts if there is more than one renderer active but returns to curent context
  void bindAndVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer); /// VERY SLOW - switches contexts if there is more than one renderer active but returns to curent context
  void enableVertexAttribArray(GLuint index);   // VERY SLOW - same as glEnableVertexAttribArray but it enables on all the contexts
  void disableVertexAttribArray(GLuint index);  // VERY SLOW - avoid using - use only the enabler on init - if you keep enabling/disabling the speed decrease is very big on multiple contexts
  //void bindBuffer(GLenum target, GLuint buffer);  // VERY SLOW - use this on VAO init, to bind the buffers on all contexts (for the VAO)
  
  // this is the func that is important:
  inline void bind() { glBindVertexArray(id[_VAOrenderer]); }                // fast func - binds VAO, depending on current renderer

  glVAO() { id= NULL; }
  ~glVAO() { delData(); }
  void delData() { if(id) delete[] id; id= NULL; }
};


extern osiGlRenderer *_glr; // CURRENTLY ACTIVE ogl RENDERER - same as [osi.glr]

#include "osiGlExt.h"


#endif /// OSI_USE_OPENGL












