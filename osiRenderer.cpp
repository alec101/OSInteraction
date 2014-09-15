#include "osinteraction.h"


int searchARB(osiRenderer *, cchar *);
int searchEXT(osiRenderer *, cchar *);
int searchOTHER(osiRenderer *, cchar *);

template<class T> extern bool getGlProc(cchar *, T&);
extern void getVERfuncs(osiRenderer *, int, int);
extern void getARBfuncs(osiRenderer *);
extern void getEXTfuncs(osiRenderer *);
extern void getOTHERfuncs(osiRenderer *);
void *glExtNULL(void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *);


  



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
  for(a= 0; _glARBlistEmpty[a].desc!= null; a++);   /// find out how big the list is
  glARBlist= new GlExt[a+ 1];
  for(a= 0; _glARBlistEmpty[a].desc!= null; a++)
    glARBlist[a]= _glARBlistEmpty[a];

  /// initial EXT list
  for(a= 0; _glEXTlistEmpty[a].desc!= null; a++);   /// find out how big the list is
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
  printf("glExtNULL func called!!!\n");
  return null;
}



/* THIS GOES, IT SEEMS
template<class T> void setGlProcNULL(T& address) {
  address= (T)&glExtNULL;
}
*/



void osiRenderer::checkExt() {
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
    getGlProc("glGetStringi", glExt.glGetStringi);      /// make shure this func is avaible

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
            if(chatty) printf("UNKNOWN extension: [%s] \n", ext);
        }
      }
      
    } /// pass thru all extension strings

  } /// lower than gl3.x or over gl3.x
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
  if(!string8::strcmp("WGL_ARB_create_context_profile", s)) return 54;
  if(!string8::strcmp("GLX_ARB_create_context_profile", s)) return 55;
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












