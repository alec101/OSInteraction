#include "osinteraction.h"


int searchARB(osiRenderer *, cchar *);
int searchEXT(osiRenderer *, cchar *);
int searchOTHER(osiRenderer *, cchar *);

//template<class T> extern bool getGlProc(cchar *, T&);
extern bool getGlProc(cchar *, void **); /// [kinda internal]OpenGL func pointer retriever; pass the name of the function, and the pointer to aquire the address
extern void getVERfuncs(osiRenderer *, int, int);
extern void getARBfuncs(osiRenderer *);
extern void getEXTfuncs(osiRenderer *);
extern void getOTHERfuncs(osiRenderer *);
void *glExtNULL(void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *);






/// [internal]
osiRenderer *_createRenderer(osiWindow *w, int major= 3, int minor= 2, void *shareGrp= null, bool compatibility= false, bool debug= false) {
  osiRenderer *r= new osiRenderer;
  bool chatty= true;
  #ifdef OS_WIN

  /*
  /// get crucial context creation functions by creating a temporary old-type context
  HGLRC tmp= wglCreateContext(w->_hDC);
  wglMakeCurrent(w->_hDC, tmp);
  
  
  //r->_getContextFuncs(); <<<<<<<<<<<<<<<<<<<<<


  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(tmp);

  
  // requested version < 3.0
  if(major< 3) {
    r->glContext= wglCreateContext(w->_hDC);

  // requested version > 3.0
  } else {

    int attr[]= { 
      WGL_CONTEXT_MAJOR_VERSION_ARB,   major,
      WGL_CONTEXT_MINOR_VERSION_ARB,   minor, 
      WGL_CONTEXT_PROFILE_MASK_ARB,    compatibility? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB: WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      /// this is crappy, imho... if u want gl4.0, just ask for gl4.0, don't exclude forward deprecated stuff, if u ask for 3
      //WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
      /// usually more checks / debug info / BUT SLOWER - USE FOR DEBUG only; check https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt - 
      debug? WGL_CONTEXT_FLAGS_ARB: 0, debug? WGL_CONTEXT_DEBUG_BIT_ARB: 0, // THIS ONE MUST BE LAST, as the int list must end in 0
      0
    };

    wglCreateContextAttribsARB(w->_hDC, shareGrp? *(HGLRC *)shareGrp: 0, attr);

  }
  */



  r->glContext= wglCreateContext(w->_hDC);


  if(chatty) printf("Renderer created: context[%d] win hDC[%d]\n", (int)r->glContext, (int)w->_hDC);
  
  #endif /// OS_WIN

  #ifdef OS_LINUX
  r->glContext= glXCreateContext(w->_dis, w->_vi, NULL, GL_TRUE);
  
/* The last parameter decides if direct rendering is enabled. If you want
   to send the graphical output via network, you have to set it to GL_FALSE.
   If your application puts its output to the computer you are sitting in
   front of, use GL_TRUE. Note that some capabilities like vertex buffer objects
   can only be used with a direct gl context (GL_TRUE). */
          
  #endif /// OS_LINUX

  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC


    


  if(!r->glContext) {
    delete r;
    return null;
  }
  w->glr= r;
  w->monitor->glr= r;
  osi.glRenderers.add(r);
  return r;
}






/// create or assign a renderer to selected window; returns pointer to the renderer or null if failed, somehow
osiRenderer *osinteraction::assignRenderer(osiWindow *w) {
  // make shure w and w's monitor are not null
  if(!w) return null;
  if(!w->monitor) return null;

  osiRenderer *r;                  /// tmp var

  /// if there are existing renderers created, check if the requested window can use one of them
  if(glRenderers.nrNodes) {

    /// check if any renderer is created on current monitor (if there is one, assign it to w)
    if(w->monitor->glr)
      return w->glr= w->monitor->glr;

    // quote from MSDN:
    //  "It need not be the same hdc that was passed to wglCreateContext when hglrc was created,
    //  but it must be on the same device and have the same pixel format."
    // if this is true, BINGO! a test can be done to see if a renderer can be activated on a window; if it can, no need for a new one!
    // ofc, this is not true, as a context can work on any gr card, FFS!!!

    glMakeCurrent(null);


    // THIS IS NOT WORKING, it will make the renderer on grcard 0 always active, no other renderers will be created
    /// check if it is possible to activate an existing renderer to this window
    /*
    for(r= (osiRenderer *)glRenderers.first; r; r= (osiRenderer *)r->next) {
      w->glr= r;
      if(glMakeCurrent(w))           /// try to make it current
        return w->monitor->glr= r;    // successfull made current
      w->glr= null;
    }
    */
    // NOT WORKING ^^^



    // other tests can be done... need the new computer to test every possibility and OS

    // more tests to be placed here ^^^
  } /// if there are renderers already created

  if(!_createRenderer(w)) return null;         /// reached this point -> create a new renderer
  
  return w->glr;
}





/// deletes the specified renderer and makes shure that windows and monitors that used it, know about it
void osinteraction::delRenderer(osiRenderer *r) {

  /// if any window uses this renderer, set it to null
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(win[a].glr== r)
      win[a].glr= null;
  
  /// if any monitor uses this renderer, set it to null
  for(short a= 0; a< display.nrMonitors; a++)
    if(display.monitor[a].glr== r)
      display.monitor[a].glr= null;


  /// if the selected renderer is the one that has to be deleted, make shure everything is ok
  if(glr== r) {
    glr= null;
    glMakeCurrent(null);
  }
  
  glRenderers.del(r);
}
















///=================///
// osiRENDERER class //
///=================///


osiRenderer *_glr; // CURRENTLY ACTIVE RENDERER, same as [osi.glr]


osiRenderer::osiRenderer()
#ifndef OS_WIN
: glExt( ::glExt)        /// set the reference to the global struct that holds all the functions 
#endif
  {
  glContext= null;
  isActive= false;
  glVerMajor= glVerMinor= 0;
  maxTexelUnits= 0;
  max3Dtexture= 0;

  #ifdef OS_WIN
  ;
  #else /// OS_MAC & OS_LINUX

  #endif

  short a;
  /// initial ARB list
  for(a= 0; _glARBlistEmpty[a].desc!= null; a++);     /// find out how big the list is
  glARBlist= new GlExt[a+ 1];
  for(a= 0; _glARBlistEmpty[a].desc!= null; a++)
    glARBlist[a]= _glARBlistEmpty[a];

  /// initial EXT list
  for(a= 0; _glEXTlistEmpty[a].desc!= null; a++);     /// find out how big the list is
  glEXTlist= new GlExt[a+ 1];
  for(a= 0; _glEXTlistEmpty[a].desc!= null; a++)
    glEXTlist[a]= _glEXTlistEmpty[a];

  /// initial OTHER list
  for(a= 0; _glOTHERlistEmpty[a].desc!= null; a++);   /// find out how big the list is
  glOTHERlist= new GlExt[a+ 1];
  for(a= 0; _glOTHERlistEmpty[a].desc!= null; a++)
    glOTHERlist[a]= _glOTHERlistEmpty[a];

  /// set all glExt funcs to point to the glExtNULL func (defined in osiGlExt.cpp)
  void **p= (void **)&glExt.glDrawRangeElements;      // glDrawRangeElements must be the first func in the structure
  for(int a= 0; a< ((sizeof(glExt)- sizeof(char))/ sizeof(void *)); a++) {
    *p= (void*)&glExtNULL;
    p++;
  }
}


osiRenderer::~osiRenderer() {
  delData();
}


void osiRenderer::delData() {
  isActive= false;

  if(glARBlist) 
    delete[] glARBlist;
  glARBlist= null;

  if(glEXTlist)
    delete[] glEXTlist;
  glEXTlist= null;

  if(glOTHERlist)
    delete[] glOTHERlist;
  glOTHERlist= null;
}


// DEFAULT NULL PROC - this is the default function all ext funcs are assigned to
/// big stack for arguments, to avoid memory corruption - hopefully no function has more than 24* sizeof(void *) argument size...
void *glExtNULL(void *a, void *b, void *c, void *d, void *e, void *f, void *g, void *h, void *i, void *j, void *k, void *l, void *m, void *n, void *o, void *p, void *q, void *r, void *s, void *t, void *u, void *v, void *x, void *y, void *z) {     /// it has a big stack for arguments
  // this can be further customized to pop an error, or mark an error somewhere
  printf("glExtNULL func called!!! - a oGL func that the system could not aquire a pointer too, was called\n");
  return null;
}



/* THIS GOES, IT SEEMS
template<class T> void setGlProcNULL(T& address) {
  address= (T)&glExtNULL;
}
*/

void osiRenderer::_getContextFuncs() {
  
  // more cleanup must be made. the nvidia/ati extensions don't work (or work for only 1 card)
  
  int major;
  char buf[128];
  cchar *ext;
  glGetIntegerv(GL_MAJOR_VERSION, &major);          /// oGL major version
  
  
  if(major< 3) {
    ext= (cchar *)glGetString(GL_EXTENSIONS);       /// oGL extensions string
    cchar *p= ext;
    while(1) {
      
      /// parse 1 extension at a time
      for(short a= 0; a< 128; a++) {
        if(*p== ' ' || *p== '\0') {                 /// extension delimiter or end of string
          buf[a]= 0;
          break;
        }

        buf[a]= *p;
        p++;
      } /// for each character in current ext string


      /// WGL_ARB_create_context #55 / WGL_ARB_create_context_profile #74 http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
      if(!string8::strcmp(buf, glARBlist[54].desc) || !string8::strcmp(buf, "WGL_ARB_create_context_profile"))
      { glARBlist[54].avaible= 1; glARBlist[73].avaible= 1; }
      /// GLX_ARB_create_context #56 / GLX_ARB_create_context_profile #75 
      if(!string8::strcmp(buf, glARBlist[55].desc) || !string8::strcmp(buf, "GLX_ARB_create_context_profile"))
      { glARBlist[55].avaible= 1; glARBlist[74].avaible= 1; }
      /// WGL_NV_gpu_affinity #355 http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
      if(!string8::strcmp(buf, glEXTlist[354].desc))
        glEXTlist[354].avaible= 1;
      /// WGL_AMD_gpu_association #361 http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
      if(!string8::strcmp(buf, glEXTlist[360].desc))
        glEXTlist[360].avaible= 1;
      /// GLX_AMD_gpu_association #398 http://www.opengl.org/registry/specs/AMD/glx_gpu_association.txt
      if(!string8::strcmp(buf, glEXTlist[397].desc))
        glEXTlist[397].avaible= 1;
    } /// pass thru all extensions


  } else {
    getGlProc("glGetStringi", (void**)&glExt.glGetStringi);      /// make shure this func is avaible

    int max;
    glGetIntegerv(GL_NUM_EXTENSIONS, &max);

    for(short a= 0; a< max; a++) {
      ext= (cchar *)glGetStringi(GL_EXTENSIONS, a);

      /// WGL_ARB_create_context / WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
      if(!string8::strcmp(ext, glARBlist[54].desc) || !string8::strcmp(ext, "WGL_ARB_create_context_profile"))
      { glARBlist[54].avaible= 1; glARBlist[73].avaible= 1; }
      /// GLX_ARB_create_context / GLX_ARB_create_context_profile 
      if(!string8::strcmp(ext, glARBlist[55].desc) || !string8::strcmp(ext, "GLX_ARB_create_context_profile"))
      { glARBlist[55].avaible= 1; glARBlist[74].avaible= 1; }
      /// WGL_NV_gpu_affinity http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
      if(!string8::strcmp(ext, glEXTlist[354].desc))
        glEXTlist[354].avaible= 1;
      /// WGL_AMD_gpu_association http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
      if(!string8::strcmp(ext, glEXTlist[360].desc))
        glEXTlist[360].avaible= 1;
      /// GLX_AMD_gpu_association
      if(!string8::strcmp(ext, glEXTlist[397].desc))
        glEXTlist[397].avaible= 1;
    }
  }

  #ifdef OS_WIN
  if(glARBlist[54].avaible)        /// #55 #74 !!! WGL_ARB_create_context !!! WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    getGlProc("wglCreateContextAttribsARB", glExt.wglCreateContextAttribsARB);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(glARBlist[55].avaible)       /// #56 #75 GLX_ARB_create_context !!! GLX_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
    getGlProc("glXCreateContextAttribsARB", (void **)&glXCreateContextAttribsARB);
  #endif /// OS_LINUX

  #ifdef OS_WIN
  if(glEXTlist[354].avaible) {    /// #355 WGL_NV_gpu_affinity http://www.opengl.org/registry/specs/NV/gpu_affinity.txt
    getGlProc("wglEnumGpusNV", glExt.wglEnumGpusNV);
    getGlProc("wglEnumGpuDevicesNV", glExt.wglEnumGpuDevicesNV);
    getGlProc("wglCreateAffinityDCNV", glExt.wglCreateAffinityDCNV);
    getGlProc("wglEnumGpusFromAffinityDCNV", glExt.wglEnumGpusFromAffinityDCNV);
    getGlProc("wglDeleteDCNV", glExt.wglDeleteDCNV);
  }
  #endif

  #ifdef OS_WIN
  if(glEXTlist[360].avaible) {    /// #361 WGL_AMD_gpu_association http://www.opengl.org/registry/specs/AMD/wgl_gpu_association.txt
    getGlProc("wglGetGPUIDsAMD", glExt.wglGetGPUIDsAMD);
    getGlProc("wglGetGPUInfoAMD", glExt.wglGetGPUInfoAMD);
    getGlProc("wglGetContextGPUIDAMD", glExt.wglGetContextGPUIDAMD);
    getGlProc("wglCreateAssociatedContextAMD", glExt.wglCreateAssociatedContextAMD);
    getGlProc("wglCreateAssociatedContextAttribsAMD", glExt.wglCreateAssociatedContextAttribsAMD);
    getGlProc("wglDeleteAssociatedContextAMD", glExt.wglDeleteAssociatedContextAMD);
    getGlProc("wglMakeAssociatedContextCurrentAMD", glExt.wglMakeAssociatedContextCurrentAMD);
    getGlProc("wglGetCurrentAssociatedContextAMD", glExt.wglGetCurrentAssociatedContextAMD);
    getGlProc("wglBlitContextFramebufferAMD", glExt.wglBlitContextFramebufferAMD);
  }
  #endif
  #ifdef OS_LINUX
  if(glEXTlist[397].avaible) {
    // MISSING funcs <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  }
  #endif
  

}




void osiRenderer::checkExt() {
  
  // more cleanup must be made (nv/ati ext funcs that should determine gpu affinity don't work...)
  
  #ifdef OS_MAC
  return;     // under mac, all extensions are avaible
  #endif
  bool chatty= true;
  char buf[128];      /// hopefully extension names won't be bigger than 128... 
  cchar *ext= null;

  /// basic renderer characteristics
  glVendor= (cchar *)glGetString(GL_VENDOR);                    /// graphics card vendor string
  glRenderer= (cchar *)glGetString(GL_RENDERER);                /// oGL 'renderer' string
  glVersion= (cchar *)glGetString(GL_VERSION);                  /// oGL version in a string format
  glGetIntegerv(GL_MAJOR_VERSION, &glVerMajor);                 /// oGL major version
  glGetIntegerv(GL_MINOR_VERSION, &glVerMinor);                 /// oGL minor version
  glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTexelUnits);      /// maximum texel units
  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &max3Dtexture);     /// maximum 3D texture size
  if(chatty) printf("context[%lld]\n", (long long)(this->glContext));
  //if(chatty) printf("GL extensions: %s\n", glGetString(GL_EXTENSIONS));
  if(chatty) printf("Vendor Name: %s\n", glVendor.d);
  if(chatty) printf("Graphic Card: %s\n", glRenderer.d);
  if(chatty) printf("GL Version: %s\n", glVersion.d);
  if(chatty) printf("Texel units avaible: %d\n", maxTexelUnits);
  if(chatty) printf("Maximum 3d texture size %dx%dx%d\n", max3Dtexture, max3Dtexture, max3Dtexture);
  if(chatty) printf("\nTesting GL Extensions...\n");

  /// OpenGL 2.x and less
  if(glVerMajor< 3) {
    /// there are 2 extensions for windows and linux, but they do the same thing: a big string with all supported extensions
    ext= (cchar *)glGetString(GL_EXTENSIONS);
    cchar *p= ext;

    if(!p) return;  // fail to aquire the string... very unlikely

    while(1) {

      /// parse 1 extension at a time
      for(short a= 0; a< 128; a++) {
        if(*p== ' ' || *p== '\0') {  /// extension delimiter or end of string
          buf[a]= 0;
          break;
        }

        buf[a]= *p;
        p++;
      } /// for each character in current ext string

      /// search the ARB list
      int i= searchARB(this, buf);
      if(i != -1) {
        glARBlist[i].avaible= true;           /// found it in ARB

      /// search EXT list
      } else {
        i= searchEXT(this, buf);
        if(i != -1) {
          glEXTlist[i].avaible= true;         /// found it in EXT

        /// search OTHER list
        } else {
          i= searchOTHER(this, buf);
          if(i != -1) {
            glOTHERlist[i].avaible= true;     /// found it in OTHER

          /// not found in any list
          } else
            if(chatty) printf("UNKNOWN extension: [%s]\n", buf);
        }
      }

      if(*p== '\0')
        return;                               /// reached the end
      else
        p++;                                  /// pass the ' ' character, next extension
    } /// for each extension string

  /// OpenGL 3.x and over
  } else {
    getGlProc("glGetStringi", (void **)&glExt.glGetStringi);      /// make shure this func is avaible

    int max, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &max);

    for(short a= 0; a< max; a++) {
      ext= (cchar *)glGetStringi(GL_EXTENSIONS, a);

      /// search ARB list
      i= searchARB(this, ext);
      if(i != -1) {
        glARBlist[i].avaible= true;           /// found it in ARB

      /// search EXT list
      } else {
        i= searchEXT(this, ext);
        if(i != -1) {
          glEXTlist[i].avaible= true;         /// found it in EXT list

        /// search OTHER list
        } else {
          i= searchOTHER(this, ext);
          if(i != -1) {
            glOTHERlist[i].avaible= true;     /// found it in OTHER list

          /// not found
          } else
            ; // if(chatty) printf("UNKNOWN extension: [%s] \n", ext); <<<<<<<<<<<<<<<
        }
      }
      
    } /// pass thru all extension strings

  } /// lower than gl3.x or over gl3.x

  if(chatty) printf("WGL_AMD_gpu_association is %s\n", glEXTlist[360].avaible? "AVAIBLE": "NOT avaible");
  if(chatty) printf("WGL_NV_gpu_affinity is %s\n", glEXTlist[354].avaible? "AVAIBLE": "NOT avaible");
}



/// this aquires all extension funcs
void osiRenderer::getExtFuncs() {
  if(glExt.initialized) return;             /// if functions already aquired, just return
  
  getVERfuncs(this, glVerMajor, glVerMinor);/// gl1.2 to current
  getARBfuncs(this);                        /// all ARB or KHR funcs
  getEXTfuncs(this);                        /// all EXT and vendor funcs
  getOTHERfuncs(this);                      /// all funcs not in ARB or EXT

  glExt.initialized= true;                  /// mark glExt funcs as aquired
}




/// [internal] searches for specified extension's array index, returns -1 if not found 
int searchARB(osiRenderer *r, cchar *s) {
  for(short a= 0; r->glARBlist[a].desc!= null; a++)   /// pass thru r's ARB extensions
    if(!string8::strcmp(r->glARBlist[a].desc, s))     /// found
      return a;

  if(!string8::strcmp("GLX_ARB_multisample", s)) return 4;
  if(!string8::strcmp("WGL_ARB_multisample", s)) return 4;
  if(!string8::strcmp("GLX_ARB_vertex_buffer_object", s)) return 27;
  if(!string8::strcmp("WGL_ARB_pixel_format_float", s)) return 38;
  if(!string8::strcmp("GLX_ARB_fbconfig_float", s)) return 38;
  if(!string8::strcmp("GLX_ARB_framebuffer_sRGB", s)) return 45;
  if(!string8::strcmp("WGL_ARB_framebuffer_sRGB", s)) return 45;
  //if(!string8::strcmp("WGL_ARB_create_context_profile", s)) return 54;
  //if(!string8::strcmp("GLX_ARB_create_context_profile", s)) return 55;
  if(!string8::strcmp("GL_ARB_robustness_share_group_isolation", s)) return 125;
  if(!string8::strcmp("GLX_ARB_robustness_share_group_isolation", s)) return 141;
  if(!string8::strcmp("WGL_ARB_robustness_share_group_isolation", s)) return 142;
  if(!string8::strcmp("GLX_ARB_context_flush_control", s)) return 167;
  if(!string8::strcmp("WGL_ARB_context_flush_control", s)) return 167;

  return -1;                                          /// not found (if reached this point)
}

/// [internal] searches for specified extension's array index, returns -1 if not found
int searchEXT(osiRenderer *r, cchar *s) {
  for(short a= 0; r->glEXTlist[a].desc!= null; a++)   /// pass thru r's EXT extensions
    if(!string8::strcmp(r->glEXTlist[a].desc, s))     /// found
      return a;

  if(!string8::strcmp("GL_SGIX_pixel_texture", s)) return 14;
  if(!string8::strcmp("GLX_SGIS_multisample", s)) return 24;
  if(!string8::strcmp("GLX_SGIX_dmbuffer", s)) return 85;
  if(!string8::strcmp("GLX_SGIS_color_range", s)) return 114;
  if(!string8::strcmp("GLX_EXT_scene_marker", s)) return 119;
  if(!string8::strcmp("GL_SUN_multi_draw_arrays", s)) return 147;
  if(!string8::strcmp("GLX_3DFX_multisample", s)) return 206;
  if(!string8::strcmp("WGL_3DFX_multisample", s)) return 206;
  if(!string8::strcmp("WGL_EXT_multisample", s)) return 208;
  if(!string8::strcmp("GL_SGIX_vertex_preclip_hint", s)) return 209;
  if(!string8::strcmp("WGL_NV_float_buffer", s)) return 280;
  if(!string8::strcmp("GLX_NV_float_buffer", s)) return 280;
  if(!string8::strcmp("WGL_EXT_pixel_format_packed_float", s)) return 327;
  if(!string8::strcmp("GLX_EXT_fbconfig_packed_float", s)) return 327;
  if(!string8::strcmp("GL_NV_texture_compression_latc", s)) return 330;
  if(!string8::strcmp("GLX_EXT_framebuffer_sRGB", s)) return 336;
  if(!string8::strcmp("WGL_EXT_framebuffer_sRGB", s)) return 336;
  if(!string8::strcmp("GLX_NV_present_video", s)) return 346;
  if(!string8::strcmp("WGL_NV_present_video", s)) return 346;
  if(!string8::strcmp("GLX_NV_video_capture", s)) return 373;
  if(!string8::strcmp("WGL_NV_video_capture", s)) return 373;
  if(!string8::strcmp("WGL_NV_copy_image", s)) return 375;
  if(!string8::strcmp("GLX_NV_copy_image", s)) return 375;
  if(!string8::strcmp("GL_NV_gpu_program_fp64", s)) return 387;
  if(!string8::strcmp("GLX_NV_multisample_coverage", s)) return 392;
  if(!string8::strcmp("WGL_NV_multisample_coverage", s)) return 392;
  if(!string8::strcmp("GLX_EXT_create_context_es2_profile", s)) return 398;
  if(!string8::strcmp("WGL_EXT_create_context_es2_profile", s)) return 399;
  if(!string8::strcmp("GL_NV_blend_equation_advanced_coherent", s)) return 432;
  if(!string8::strcmp("GL_KHR_blend_equation_advanced_coherent", s)) return 457;

  return -1;
}

/// [internal] searches for specified extension's array index, returns -1 if not found
int searchOTHER(osiRenderer *r, cchar *s) {
  for(short a= 0; r->glOTHERlist[a].desc!= null; a++) /// pass thru r's OTHER extensions
    if(!string8::strcmp(r->glOTHERlist[a].desc, s))     /// found
      return a;

  return -1;                                          /// not found (if reached this point)
}












