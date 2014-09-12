#pragma once


///=============================================================///
// RENDERER class - each grCard has one and should have ONLY one //
///=============================================================///

class osiRenderer: public chainData {
public:
  
  #ifdef OS_WIN
  HGLRC glContext;         // ze thing needed - destructor deletes oGL render context if it is created
  GlExtFuncs glExt;        /// each renderer has it's own oGL extensions functions, under windows
  #endif /// OS_WIN
  #ifdef OS_LINUX
  GLXContext glContext;    // oGL rendering context
  GlExtFuncs &glExt;    /// reference to a global struct that holds all extension functions
  #endif /// OS_LINUX
  #ifdef OS_MAC
  // ??? not used/ view has the context. this needs further thinking ???
  void *glContext;        /// reference to a global struct that holds all extension functions
  GlExtFuncs &glExt;
  #endif

  bool isActive;       // a simple check to see if it is already active when calling glMakeCurrent() is WAAAY faster than switching contexts... 

  // OpenGL extensions - these are defined in osiGlExt.cpp - by default, all extensions are auto-checked and aquired
  
  void checkExt();     // checks all all extension avaibility and populates glExtList[] array - MUST BE CALLED BEFORE aquiring extension functions
  void getExtFuncs(); /// gets all oGL extensions funcs for this renderer; renderer MUST be active-> glMakeCurrent (no problems if called multiple times)
  bool isExtensionARB(cchar *);
  bool isExtensionEXT(cchar *);


/* SUBJECT OF DELETION (just get all avaible extensions, right?)
  // OpenGL extensions functions aquiring - these are defined in osiGlExt.cpp

  void getAllAvaibleExtensions(); /// gets all oGL extensions funcs for this renderer; renderer MUST be active-> glMakeCurrent (no problems if called multiple times)
  void getVERfuncs(int, int);   /// specify OpenGL version (major, minor) to aquire all it's funcs
  void getVERfuncsAll();        /// aquires all funcs for all major OpenGL versions (that current renderer supports)
  void getVERfuncsUntil(int, int); /// aquires all funcs for all OpenGL versions until specified version (major, minor)
  void getARBfuncs(int);        /// makes specified ARB extension (#NUMBER) funcs avaible
  void getARBfuncs(cchar *);    /// makes specified ARB extension (glSTRING) funcs avaible
  void getARBfuncsAll();        /// aquires all ARB extension funcs (that current renderer supports)
  void getEXTfuncs(int);        /// makes specified EXT or vendor (#NUMBER) funcs avaible
  void getEXTfuncs(cchar *);    /// makes specified EXT or vendor (#glSTRING) funcs avaible
  void getEXTfuncsAll();        /// aquires all EXT and vendor extension funcs (that current renderer supports)
*/

  // renderer / graphics card parameters

  int max3Dtexture;       /// maximum 3D texture size this renderer can operate
  GLint maxTexelUnits;    /// maximum texel units this renderer can operate

  // these are populated when getExtensions() is called >>> ... not pretty <<<
  
  // RENDERER CHARACTERISTICS COULD BE IN A struct, IF TOO MANY

  string8 glVendor;       /// this can be used for various checks, as it never changes
  string8 glRenderer;     /// this can be used for various checks, as it never changes
  string8 glVersion;      /// OpenGL version
  int glVerMajor, glVerMinor; /// OpenGL version

  GlExt *glARBlist;       /// all openGL ARB extensions list and their avaibility on this renderer
  GlExt *glEXTlist;       /// all openGL EXT(and the rest) extensions list and their avaibility on this renderer
  GlExt *glOTHERlist;     /// all openGL extensions that are not listed anywhere

  /*
  int nrARB;
  int nrEXT;
  int nrOTHER;

  static int ARBlistSize; /// when adding more exts, update this var too
  static int EXTlistSize; /// when adding more exts, update this var too
  */

  // constructor, destructor

  osiRenderer();
  ~osiRenderer();
  void delData();
};

















