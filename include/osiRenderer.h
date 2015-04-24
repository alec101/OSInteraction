#pragma once


///=======================================================///
// RENDERER class - manages OpenGL contexts and extensions //
///=======================================================///



struct GlExtFuncs;

class osiRenderer: public chainData {
public:

  bool isActive;           // a simple check to see if it is already active when calling glMakeCurrent() is WAAAY faster than switching contexts... 
  osiMonitor *monitor;    /// the monitor on which the renderer was created. assigning a renderer to render to a different monitor that is handled by a different GPU, can be bad

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
  GlExt *glARBlist;                   /// all openGL ARB extensions list and their avaibility on this renderer
  GlExt *glEXTlist;                   /// all openGL EXT(and the rest) extensions list and their avaibility on this renderer
  GlExt *glOTHERlist;                 /// all openGL extensions that are not listed anywhere

  // ogl context and a struct with all extension funcs

  #ifdef OS_WIN
  HGLRC glContext;         // ze thing needed - destructor deletes oGL render context if it is created
  GlExtFuncs glExt;       /// each renderer has it's own oGL extensions functions, under windows
  #endif /// OS_WIN

  #ifdef OS_LINUX
  GLXContext glContext;    // oGL rendering context
  GlExtFuncs &glExt;      /// reference to a global struct that holds all extension functions
  #endif /// OS_LINUX

  #ifdef OS_MAC
  void *glContext;        /// NSOpenGLContext object (which wraps a CGL renderer which wraps the context and all these are wrapped by osiRenderer. Do you even wrap?)
  GlExtFuncs &glExt;      /// NOT USED
  #endif

  // constructor, destructor

  osiRenderer();
  ~osiRenderer();
  void delData();

private:
  
  friend bool _getContextFuncs(osiWindow *, osiRenderer *);
  bool _bContextFuncsGot; // wip

  #ifdef OS_MAC
  friend class osiCocoa;
  void *_pixelFormat;     /// NSOpenGLPixelFomat object
  #endif
};

extern osiRenderer *_glr; // CURRENTLY ACTIVE RENDERER - same as [osi.glr]

#include "osiGlExt.h"













