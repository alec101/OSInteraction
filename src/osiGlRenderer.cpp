#include "osinteraction.h"
// not part of coreARB exts that are used in this file:
//  - GL_EXT_texture_filter_anisotropic
//#define OSI_USE_OPENGL_EXOTIC_EXT 1

#ifdef OSI_BE_CHATTY
#include <stdio.h>
#endif

#ifdef OSI_USE_OPENGL
#include "util/typeShortcuts.h"
        
using namespace Str;

int _osiSearchARB(osiGlRenderer *, cchar *);
int _osiSearchEXT(osiGlRenderer *, cchar *);
int _osiSearchOTHER(osiGlRenderer *, cchar *);

//template<class T> extern bool getGlProc(cchar *, T&);
extern bool _osiGetGlProc(const char *, void **); /// [kinda internal]OpenGL func pointer retriever; pass the name of the function, and the pointer to aquire the address
extern void _osiGetVERfuncs(osiGlRenderer *, int, int);
extern void _osiGetARBfuncs(osiGlRenderer *);
extern void _osiGetEXTfuncs(osiGlRenderer *);
extern void _osiGetOTHERfuncs(osiGlRenderer *);
//void *_osiGlExtNULL(void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *);

// internal stuff
bool _osiGetContextFuncs(osiMonitor *, osiGlRenderer *);
bool _osiCreateFrontBuffer(osiWindow *, osiGlRenderer *);
bool _osiCreateContext(osiWindow *, osiGlRenderer *);

#ifdef OS_WIN
osiWindow *_osiCreateTmpWin(osiMonitor *m);
extern LRESULT CALLBACK _processMSG(HWND hWnd, UINT m, WPARAM wParam, LPARAM lParam);
#endif /// OS_WIN







// OSI RENDERER CLASS

/// create or assign a renderer to selected window; returns pointer to the renderer or null if failed, somehow
osiGlRenderer *osinteraction::glAssignRenderer(osiWindow *w) {
  //   nope. better:
  //   0. _getContextFuncs(osiMonitor *) - always on a monitor, or else this would end up in a constructor, always on a dummy window
  //   1. _createFrontBuffer(window, renderer);
  //   2. _createContext(window, renderer);     (maybe not window, dunno)
  //   "PIXEL FORMATS CAN BE SET ONLY ONCE ON A WINDOW."
  //   there's a RedrawWindow or updateWindow, if stuff don't work

  

  // make shure w and w's monitor are not null
  if(!w) return null;
  if(!w->monitor) return null;

  bool chatty= false;
  bool rendererAllocated= false;
  
  osiGlRenderer *r= null;      /// the returned renderer;

  /// create new / assign existing renderer based on current settings
  if(settings.customRenderer) {                // using provided custom renderer
    r= (osiGlRenderer *)settings.customRenderer;
    if(!_osiGetContextFuncs(w->monitor, r)) { error.simple("Cannot get context funcs"); goto Failed; }

  } else if(settings.rendererCreation== osiSettings::onePerGPU) {      // one renderer per GPU
    osiGlRenderer *p= (osiGlRenderer *)glRenderers.first;
    for(;p; p= (osiGlRenderer *)p->next)
      if(p->monitor->GPU== w->monitor->GPU)
        break;
    /// fount an already created renderer?
    if(p)
      r= p;
    else {
      r= new osiGlRenderer;
      if(!_osiGetContextFuncs(w->monitor, r)) { error.simple("Cannot get context funcs"); goto Failed; }
      rendererAllocated= true;
    }

  } else if(settings.rendererCreation== osiSettings::onePerMonitor) {  // one renderer per monitor
    osiGlRenderer *p= (osiGlRenderer *)glRenderers.first;
    for(;p; p= (osiGlRenderer *)p->next)
      if(p->monitor== w->monitor)
        break;
    if(p)
      r= p;
    else {
      r= new osiGlRenderer;
      if(!_osiGetContextFuncs(w->monitor, r)) { error.simple("Cannot get context funcs"); goto Failed; }
      rendererAllocated= true;
    }

  } else if(settings.rendererCreation== osiSettings::onePerWindow) {      // one renderer per window

    if(w->renderer)
      if(w->renderer->type== 0)
        r= (osiGlRenderer *)w->renderer;

    if(!r) {
      r= new osiGlRenderer;
      
      if(!_osiGetContextFuncs(w->monitor, r)) { error.simple("Cannot get context funcs"); goto Failed; }
      rendererAllocated= true;
    }

  } else if(settings.rendererCreation== osiSettings::onlyOne) {
    r= (osiGlRenderer *)osi.glRenderers.first;
    if(r== null) {
      r= new osiGlRenderer;
      if(!_osiGetContextFuncs(w->monitor, r)) { error.simple("cannot get context funcs"); goto Failed; }
      rendererAllocated= true;
    }


  }
  //else if(settings.renderer.noAutocreateRenderer)     // do not create any renderer
    //; // do nothing
  
  #ifdef OS_WIN
  /// create a front buffer for the window
  if(!_osiCreateFrontBuffer(w, r)) { error.simple("Cannot create a front buffer for the window"); goto Failed; }
  #endif

  /// create a context for the renderer, if this is a newly created renderer
  if(rendererAllocated)
    if(!_osiCreateContext(w, r)) { error.simple("Cannot create a context for the renderer"); goto Failed; }

  // success
  w->renderer= r;
  w->monitor->renderer= r;
  r->monitor= w->monitor;
  r->GPU= r->monitor->GPU;

  osi.glRenderers.add(r);
  return r;

Failed:
  if(rendererAllocated)
    delete r;
  w->delData();
  return null;
}


/// create a custom renderer, that will surely work on selected monitor
osiGlRenderer *osinteraction::glCreateRendererMon(osiMonitor *m) {
  if(!m) return null;
  str8 func= "osi::createRendererMon() ";
  osiGlRenderer *r= new osiGlRenderer;
  
  #ifndef OS_MAC // OS_WIN+ OS_LINUX
  _osiGetContextFuncs(m, r);
  osiWindow w;
  w.monitor= m;

  #ifdef OS_WIN
  w.monitor= m;

  WNDCLASS wc;          /// Windows Class Structure
  DWORD dwExStyle;      /// Window Extended Style
  DWORD dwStyle;        /// Window Style
  RECT rect;            /// Grabs Rectangle Upper Left / Lower Right Values

  w.x0= m->y0;
  w.y0= m->y0;
  w.dx= 50;
  w.dy= 50;
  w.name= "_glTmpWindow";

  rect.left=   w.x0;
  rect.right=  w.x0+ 50;
  #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
  rect.top=    osi.display.vyMax- (w.y0+ w.dy- 1);
  #endif
  #ifdef OSI_USE_ORIGIN_TOP_LEFT
  rect.top=    w.y0;
  #endif
  rect.bottom= rect.top- 50;

  w._hInstance=     (uint64_t)GetModuleHandle(NULL);     /// grab an instance for window
  Str::memclr(&wc, sizeof(wc));
  wc.style=         CS_HREDRAW| CS_VREDRAW| CS_OWNDC;    /// Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc=   (WNDPROC) _processMSG;                // _processMSG handles messages
  wc.hInstance=     (HINSTANCE)w._hInstance;             /// set the aquired instance
  wc.hIcon=         LoadIcon(NULL, IDI_WINLOGO);         /// load default icon
  wc.hCursor=       LoadCursor(NULL, IDC_ARROW);         /// load arrow pointer
  wc.lpszClassName= w.name;                              /// class name... dunno for shure what this is

  if (!RegisterClass(&wc)) { error.simple(func+ "tmpWindow: Failed to register wc"); return null; }

  dwExStyle= WS_EX_TOOLWINDOW| WS_EX_WINDOWEDGE;
  dwStyle= WS_OVERLAPPEDWINDOW| WS_CLIPSIBLINGS| WS_CLIPCHILDREN;                 /// WS_CHILD is out of the equation (these are windows in windows)

  AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

  // Create The Window
  if (!(w._hWnd= (uint64_t)CreateWindowEx(dwExStyle, w.name, w.name, dwStyle, rect.left, rect.top, rect.right- rect.left, rect.bottom- rect.top, NULL, NULL, (HINSTANCE)w._hInstance, NULL))) {
    error.simple(func+ "tmp window: Window creation error."); goto Failed; }
  if (!(w._hDC= (uint64_t)GetDC((HWND)w._hWnd))) { error.simple(func+ "tmpWindow: Can't create a GL DC"); goto Failed; }
  #endif /// OS_WIN


  if(!_osiCreateFrontBuffer(&w, r)) { error.simple(func+ "front buffer creation failed"); goto Failed; }
  if(!_osiCreateContext(&w, r)) { error.simple(func+ "context creation failed"); goto Failed; }
  osi.glMakeCurrent(r, &w);
  r->checkExt();
  r->getExtFuncs();
  osi.glMakeCurrent(null, null);

  w.delData();
  #endif

  #ifdef OS_MAC
  if(!cocoa.createContext(r, m->_oglDisplayMask)) { error.simple(func+ "create context failed"); goto Failed; }
  #endif

  osi.glRenderers.add(r);

  return r;

Failed:
  delete r;
  return null;
}

/// create a custom renderer, that will be asigned to specified window
osiGlRenderer *osinteraction::glCreateRendererWin(osiWindow *w) {

  error.simple("makeme");
  return null;
}



/// deletes the specified renderer and makes shure that windows and monitors that used it, know about it
void osinteraction::glDelRenderer(osiGlRenderer *r) {

  /// if any window uses this renderer, set it to null
  for(short a= 0; a< OSI_MAX_WINDOWS; a++)
    if(win[a].renderer== r)
      win[a].renderer= null;
  
  /// if any monitor uses this renderer, set it to null
  for(short a= 0; a< display.nrMonitors; a++)
    if(display.monitor[a].renderer== r)
      display.monitor[a].renderer= null;


  /// if the selected renderer is the one that has to be deleted, make shure everything is ok
  if(glr== r) {
    glr= null;
    glMakeCurrent(null, null);
  }
  
  glRenderers.del(r);
}



// DEFAULT NULL PROC - this is the default function all ext funcs are assigned to
/// big stack for arguments, to avoid memory corruption - hopefully no function has more than 24* sizeof(void *) argument size...
void *_osiGlExtNULL(void *a, void *b, void *c, void *d, void *e, void *f, void *g, void *h, void *i, void *j, void *k, void *l, void *m, void *n, void *o, void *p, void *q, void *r, void *s, void *t, void *u, void *v, void *x, void *y, void *z) {     /// it has a big stack for arguments
  // this can be further customized to pop an error, or mark an error somewhere
  #ifdef OSI_BE_CHATTY
  printf("_osiGlExtNULL func called!!! - a oGL func that the system could not aquire a pointer to, was called\n");
  #endif
  return null;
}



/* THIS GOES, IT SEEMS
template<class T> void setGlProcNULL(T& address) {
  address= (T)&glExtNULL;
}
*/






osiWindow *_osiCreateTmpWin(osiMonitor *m) {
  osiWindow *w= new osiWindow;
  w->x0= m->y0+ 10;
  w->y0= m->y0+ 10;
  w->dx= 50;
  w->dy= 50;
  w->name= "_osiGlTmpWindow";
  w->mode= 1;
  w->monitor= m;
  
  #ifdef OS_WIN
  GLuint PixelFormat;   // Holds The Results After Searching For A Match
  WNDCLASS wc;          // Windows Class Structure
  DWORD dwExStyle;      // Window Extended Style
  DWORD dwStyle;        // Window Style
  RECT rect;            // Grabs Rectangle Upper Left / Lower Right Values

  rect.left=   w->x0;
  rect.right=  w->x0+ 50;
  #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
  rect.top=    osi.display.vyMax- (w->y0+ w->dy- 1);
  #endif
  #ifdef OSI_USE_ORIGIN_TOP_LEFT
  rect.top=    w->y0;
  #endif
  rect.bottom= rect.top- 50;

  w->_hInstance=    (uint64_t)GetModuleHandle(NULL);      /// grab an instance for window
  wc.style=         CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   /// Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc=   (WNDPROC) _processMSG;                 // _processMSG handles messages
  wc.cbClsExtra=    0;                                    /// no extra
  wc.cbWndExtra=    0;                                    /// no extra
  wc.hInstance=     (HINSTANCE)w->_hInstance;             /// set the aquired instance
  wc.hIcon=         LoadIcon(NULL, IDI_WINLOGO);          // load default icon <<<<<<<<<<<<<<<<<<<<<< ICON WORKS MUST BE MADE
  wc.hCursor=       LoadCursor(NULL, IDC_ARROW);          /// load arrow pointer
  wc.hbrBackground= NULL;                                 /// no backgraound required when using opengl
  wc.lpszMenuName=  NULL;                                 /// no menus
  wc.lpszClassName= w->name;                              /// class name... dunno for shure what this is

  if (!RegisterClass(&wc)) {                        /// register the window class
    error.simple("tmp window: Failed to register wc");
    delete w;
    return null;
  }

  dwExStyle= WS_EX_TOOLWINDOW| WS_EX_WINDOWEDGE;
  dwStyle= WS_OVERLAPPEDWINDOW;                 /// WS_CHILD is out of the equation (these are windows in windows)

  AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

  // Create The Window
  if (!(w->_hWnd= (uint64_t)CreateWindowEx(
                dwExStyle,                 // Extended Style For The Window
                w->name,                   // class name           <--- might want a different class name?
                w->name,                  /// window title
                dwStyle |                 /// defined window style
                WS_CLIPSIBLINGS |         /// Required Window Style ?? not shure
                WS_CLIPCHILDREN,          /// Required Window Style ?? not shure
                w->x0,
                #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
                osi.display.vyMax- (w->y0+ w->dy- 1),
                #endif
                #ifdef OSI_USE_ORIGIN_TOP_LEFT
                w->y0,
                #endif
                rect.right- rect.left,    /// dx
                rect.bottom- rect.top,    /// dy
                NULL,                     /// parent window
                NULL,                     /// no menu
                (HINSTANCE)w->_hInstance, /// instance
                NULL)))                   /// don't pass anything to WM_CREATE
  {
    error.simple("tmp window: Window creation error.");
    delete w;
    return null;
  }

  if (!(w->_hDC= (uint64_t)GetDC((HWND)w->_hWnd))) {       /// get a device context
    error.simple("tmpWindow: Can't create a GL DC");
    delete w;
    return null;
  }

  /// pixel format - MORE TESTING NEEDED HERE. screen blacks out on mode 3 - it shouldn't
  static PIXELFORMATDESCRIPTOR pfd;

  /// get the current pixel format index  
  int pixelf= GetPixelFormat((HDC)w->_hDC); 

  /// obtain a detailed description of that pixel format  
  DescribePixelFormat((HDC)w->_hDC, pixelf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  pfd.dwFlags= pfd.dwFlags| PFD_DRAW_TO_WINDOW| PFD_DRAW_TO_BITMAP| PFD_SUPPORT_OPENGL| PFD_STEREO_DONTCARE| PFD_DOUBLEBUFFER; //| PFD_SWAP_EXCHANGE;//| PFD_NEED_PALETTE;

  if (!(PixelFormat= ChoosePixelFormat((HDC)w->_hDC, &pfd))) {  /// lots of checks, don't think any needed
    error.simple("tmpWindow: Can't aquire a PixelFormat");
    delete w;
    return null;
  }

  if(!SetPixelFormat((HDC)w->_hDC, PixelFormat, &pfd)) {        /// lots of checks, don't think any needed
    error.simple("tmpWindow: Can't set PixelFormat");
    delete w;
    return null;
  }

  /// create a temporary legacy context that will be used to aquire the func pointers
  osiGlRenderer *r= new osiGlRenderer;
  r->glContext= wglCreateContext((HDC)w->_hDC);
  if(!r->glContext) {
    error.simple("tmpWindow: Can't create temporary context");
    delete r;
    delete w;
    return null;
  }
  w->renderer= r;

  return w;
  #endif /// OS_WIN

  
  #ifdef OS_LINUX
  Colormap cmap;
  XSetWindowAttributes swa;
  
  w->_root= m->_root;                                        // 'desktop window'
  w->_dis= osi._dis;                                        // XServer connection

  
  GLXFBConfig *fbc= null;   // window front buffer  
  int nfbc;                 // nr of compatible front buffers returned

  int attr[]= {
    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RED_SIZE,      8,
    GLX_GREEN_SIZE,    8,
    GLX_BLUE_SIZE,     8,
    GLX_ALPHA_SIZE,    8,
    GLX_DEPTH_SIZE,    16,
    GLX_STENCIL_SIZE,  8,
    0  /// terminator
  };
  
  fbc= glXChooseFBConfig(osi._dis, w->monitor->_screen, attr, &nfbc);
  if(!fbc || !nfbc) { error.simple("Failed to retrieve any framebuffer configs with current settings"); delete w; return null; }

  if(w->_vi) XFree(w->_vi);
  w->_vi= null;
  w->_fbID= 0;

  /// find a FB that can have a visual - not all can have visuals
  for(int a= 0; a< nfbc; a++) {
    w->_vi= glXGetVisualFromFBConfig(osi._dis, fbc[a]);
    if(!w->_vi) continue;
    w->_fbID= fbc[a];
    break;
  }
  XFree(fbc);

  if((!w->_vi)|| (!w->_fbID)) { error.simple("No visual/front buffer config found with current settings"); delete w; return null; }
  
  /// color map (not shure what's this doing anymore)
  cmap= XCreateColormap(w->_dis, w->_root, w->_vi->visual, AllocNone);
  swa.colormap= cmap;
  
  swa.event_mask= ExposureMask|                       // redraw events
                  StructureNotifyMask;                // probly the default, a parent must have this I THINK
                  //EnterWindowMask| LeaveWindowMask|   // mouse enters window &other stuff like this
                  //FocusChangeMask|                    // gain/lose focus
                  //KeyPressMask| KeyReleaseMask|       // keyboard
                  //ButtonPressMask| ButtonReleaseMask| // mouse buttons
                  //PointerMotionMask;                  // mouse motion
    
  w->_win= XCreateWindow(w->_dis, w->_root,
                         w->x0,
                         #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
                         osi.display.vyMax- (w->y0+ w->dy- 1),
                         #endif
                         #ifdef OSI_USE_ORIGIN_TOP_LEFT
                         w->y0,
                         #endif
                         w->dx, w->dy,                   // size
                         0,                              // border size
                         w->_vi->depth,                  // depth can be CopyFromParent
                         InputOutput,                    // InputOnly/ InputOutput/ CopyFromParent
                         w->_vi->visual,                 // can be CopyFromParent
                         CWColormap| CWEventMask,        // tied with &swa
                         &swa);                          //


  w->_setWMstate(1, "_NET_WM_STATE_SKIP_TASKBAR");
  XStoreName(w->_dis, w->_win, w->name);         /// window name (top bar description/name)

  w->isCreated= true;
  
  /// create a temporary legacy context that will be used to aquire the func pointers
  osiGlRenderer *r= new osiGlRenderer;
  r->glContext= glXCreateContext(w->_dis, w->_vi, null, True);
  #endif ///OS_LINUX

  #ifndef OS_MAC
  if(!r->glContext) {
    error.simple("tmpWindow: Can't create temporary context");
    delete r;
    delete w;
    return null;
  }
  w->renderer= r;

  return w;
  #endif
  return null;    /// macs will return null
}


void _osiDelTmpWin(osiWindow *w) {
  if(w->renderer) { delete w->renderer; w->renderer= null; }
  w->delData();
  delete w;
}

/*
void osiGlRenderer::_copyContextFuncs(osiGlRenderer *r) {
  if(!r) return;
  #ifdef OS_WIN

  #endif
}
*/






bool _osiGetContextFuncs(osiMonitor *m, osiGlRenderer *r) {
  #ifdef OS_MAC
  // maybe some kind of querry...
  return true;
  #else // OS_WIN + OS_LINUX
  
  if(!r) return false;

  bool ret= true;     /// the return value, this will be used for various checks

  int major, minor;

  uint8 buf[128];
  cuint8 *ext;
  cuint8 *p= null;

  // set the current renderer to r, and get crucial functions - these funcs MUST be avaible
  _glr= r;

  
  

  if(!m) return false;
  /// under windows, a temporary window must be created
  osiWindow *t= _osiCreateTmpWin(m);
  if(!osi.glMakeCurrent((osiGlRenderer *)t->renderer, t)) error.simple("test1");

  /// get crucial funcs for the temporary context of the window
  ret= _osiGetGlProc("glGetString", (void **)&((osiGlRenderer *)t->renderer)->glExt.glGetString);
  if(!ret) { error.simple("_getContextFuncs(): Cannot get glGetString"); return false; }
  ret= _osiGetGlProc("glGetIntegerv", (void **)&((osiGlRenderer *)t->renderer)->glExt.glGetIntegerv);
  if(!ret) { error.simple("_getContextFuncs(): Cannot get glGetIntegerv"); return false; }
  ret= _osiGetGlProc("glGetError", (void **)&((osiGlRenderer *)t->renderer)->glExt.glGetError);
  if(!ret) { error.simple("_getContextFuncs(): Cannot get glGetError"); return false; }

  /// get the same critical funcs for the r renderer - t and r are on the same monitor, they share the same gr card contexts, same pointers
  ret= _osiGetGlProc("glGetString", (void **)&r->glExt.glGetString);
  if(!ret) { error.simple("_getContextFuncs(): Cannot get glGetString"); return false; }
  ret= _osiGetGlProc("glGetIntegerv", (void **)&r->glExt.glGetIntegerv);
  if(!ret) { error.simple("_getContextFuncs(): Cannot get glGetIntegerv"); return false; }
  ret= _osiGetGlProc("glGetError", (void **)&r->glExt.glGetError);
  if(!ret) { error.simple("_getContextFuncs(): Cannot get glGetError"); return false; }

  
  
  
  
  #ifdef OS_WIN
  ret= _osiGetGlProc("wglGetExtensionsStringARB", (void **)&r->glExt.wglGetExtensionsStringARB);      /// make sure this func is available
  //if(!ret) { error.simple("_getContextFuncs(): Cannot get wglGetExtensionsStringARB"); return false; }
  #endif /// OS_WIN

  
  osi.glGetVersion(&major, &minor);

  /// 2 ways to get extensions, 1 for ogl less than 3, another for ogl 3 and above
  // ogl <3 ===---
  if(major< 3) {
    ext= (cuint8 *)glGetString(GL_EXTENSIONS);       /// oGL extensions string
    p= ext;

    while(p) {
      /// parse 1 extension at a time
      for(int a= 0; a< 128; a++) {
        if(*p== ' ' || *p== '\0') {                 /// extension delimiter or end of string
          buf[a]= 0;
          break;
        }

        buf[a]= *p;
        p++;
      } /// for each character in current ext string

      /// WGL_ARB_create_context #55 / WGL_ARB_create_context_profile #74 http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
      if(!strcmp8((cchar *)buf, r->glARBlist[54].desc) || !strcmp8((cchar *)buf, "WGL_ARB_create_context_profile"))
      { r->glARBlist[54].avaible= 1; r->glARBlist[73].avaible= 1; }
      /// WGL_ARB_pixel_format #9 https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
      if(!strcmp8((cchar *)buf, r->glARBlist[8].desc))
      { r->glARBlist[8].avaible= 1;}
      /// GLX_ARB_create_context #56 / GLX_ARB_create_context_profile #75 
      if(!strcmp8((cchar *)buf, r->glARBlist[55].desc) || !strcmp8((cchar *)buf, "GLX_ARB_create_context_profile"))
      { r->glARBlist[55].avaible= 1; r->glARBlist[74].avaible= 1; }

      if(*p== '\0') break;        /// reached the end
      else          p++;          /// pass the ' ' character, next extension
    } /// pass thru all extensions


  // ogl 3> ===---
  } else {
    
    ret= _osiGetGlProc("glGetStringi", (void**)&r->glExt.glGetStringi);      /// make shure this func is avaible
    if(!ret) { error.simple("_getContextFuncs(): Cannot get glGetStringi"); return false; }
    
    GLenum error;

    int max;
    glGetIntegerv(GL_NUM_EXTENSIONS, &max);
    
    error = glGetError();
    #ifdef OSI_BE_CHATTY
    if (error != GL_NO_ERROR) printf("OpenGL Error: %u\n", error);
    #endif
 
    
    for(int a= 0; a< max; a++) {
      ext= (cuint8 *)(((PFNGLGETSTRINGIPROC)r->glExt.glGetStringi)(GL_EXTENSIONS, a));
      error = glGetError();
      #ifdef OSI_BE_CHATTY
      if (error != GL_NO_ERROR) printf("OpenGL Error: %u\n", error);
      #endif

      /// WGL_ARB_create_context / WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
      if(!strcmp8((cchar *)ext, r->glARBlist[54].desc) || !strcmp8((cchar *)ext, "WGL_ARB_create_context_profile"))
      { r->glARBlist[54].avaible= 1; r->glARBlist[73].avaible= 1; }
      /// WGL_ARB_pixel_format #9 https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
      if(!strcmp8((cchar *)ext, r->glARBlist[8].desc))
      { r->glARBlist[8].avaible= 1;}
      /// GLX_ARB_create_context / GLX_ARB_create_context_profile 
      if(!strcmp8((cchar *)ext, r->glARBlist[55].desc) || !strcmp8((cchar *)ext, "GLX_ARB_create_context_profile"))
      { r->glARBlist[55].avaible= 1; r->glARBlist[74].avaible= 1; }
    }
  } // ogl <3 or 3>


  // OS specific extensions parse
  ext= null;

  #ifdef OS_WIN
  if(r->glExt.wglGetExtensionsStringARB)
    ext= (cuint8 *)((PFNWGLGETEXTENSIONSSTRINGARBPROC)r->glExt.wglGetExtensionsStringARB)(wglGetCurrentDC());
  #endif

  #ifdef OS_LINUX
  ext= (cuint8 *)glXQueryExtensionsString(osi._dis, (m? m->_screen: 0));
  #endif

  p= ext;     /// p can be null, in this case, the next loop won't parse nothin

  while(p) {
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
    if(!strcmp8((cchar *)buf, r->glARBlist[54].desc) || !strcmp8((cchar *)buf, "WGL_ARB_create_context_profile"))
    { r->glARBlist[54].avaible= 1; r->glARBlist[73].avaible= 1; }
    /// WGL_ARB_pixel_format #9 https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
    if(!strcmp8((cchar *)buf, r->glARBlist[8].desc))
    { r->glARBlist[8].avaible= 1;}
    /// GLX_ARB_create_context #56 / GLX_ARB_create_context_profile #75 
    if(!strcmp8((cchar *)buf, r->glARBlist[55].desc) || !strcmp8((cchar *)buf, "GLX_ARB_create_context_profile"))
    { r->glARBlist[55].avaible= 1; r->glARBlist[74].avaible= 1; }

    if(*p== '\0') break;        /// reached the end
    else          p++;          /// pass the ' ' character, next extension
  } /// pass thru each extension in string




  // all the extension strings were parsed, get func pointers, if they're avaible
  #ifdef OS_WIN
  if(r->glARBlist[54].avaible)        /// #55 #74 !!! WGL_ARB_create_context !!! WGL_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    _osiGetGlProc("wglCreateContextAttribsARB", (void **)&r->glExt.wglCreateContextAttribsARB);
  if(r->glARBlist[8].avaible) {
    _osiGetGlProc("wglGetPixelFormatAttribivARB", (void **)&r->glExt.wglGetPixelFormatAttribivARB);
    _osiGetGlProc("wglGetPixelFormatAttribfvARB", (void **)&r->glExt.wglGetPixelFormatAttribfvARB);
    _osiGetGlProc("wglChoosePixelFormatARB", (void **)&r->glExt.wglChoosePixelFormatARB);
  }
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(r->glARBlist[55].avaible)       /// #56 #75 GLX_ARB_create_context !!! GLX_ARB_create_context_profile http://www.opengl.org/registry/specs/ARB/glx_create_context.txt
    _osiGetGlProc("glXCreateContextAttribsARB", (void **)&r->glExt.glXCreateContextAttribsARB);
  #endif /// OS_LINUX

  osi.glMakeCurrent(null, null);
  _osiDelTmpWin(t);

  #endif  /// OS_WIN + OS_LINUX
  return true;
}



/// [internal] create window front buffer - after the renderer is allocated / context funcs are got
/// under windows, this is called after the window is created, under linux before the window is created... NEAT!!!
bool _osiCreateFrontBuffer(osiWindow *w, osiGlRenderer *r) {
  #ifdef OS_WIN
  /// pixel format - MORE TESTING NEEDED HERE. screen blacks out on mode 3 - it shouldn't
  //static PIXELFORMATDESCRIPTOR pfd;
  
  /// get the current pixel format index  
  //int pixelf= GetPixelFormat(w->_hDC); 

  /// obtain a detailed description of that pixel format  
  //DescribePixelFormat(w->_hDC, pixelf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  /*
  if(chatty) {
    printf("Current pixel format descriptor:\n");
    printf("  [nVersion= %d] [iPixelType= %d] [cColorBits= %d]\n", pfd.nVersion, pfd.iPixelType, pfd.cColorBits);
    printf("  [cRedBits= %d] [cRedShift= %d] [cGreenBits= %d]\n", pfd.cRedBits, pfd.cRedShift, pfd.cGreenBits);
    printf("  [cGreenShift= %d] [cBlueBits= %d] [cBlueShift= %d]\n", pfd.cGreenShift, pfd.cBlueBits, pfd.cBlueShift);
    printf("  [cAlphaBits= %d] [cAlphaShift= %d] [cAccumBits= %d]\n", pfd.cAlphaBits, pfd.cAlphaShift, pfd.cAccumBits);
    printf("  [cAccumRedBits= %d] [cAccumGreenBits= %d] [cAccumBlueBits= %d]\n", pfd.cAccumRedBits, pfd.cAccumGreenBits, pfd.cAccumBlueBits);
    printf("  [cAccumAlphaBits= %d] [cDepthBits= %d [cStencilBits= %d]\n", pfd.cAccumAlphaBits, pfd.cDepthBits, pfd.cStencilBits);
    printf("  [cAuxBuffers= %d] [iLayerType= %d] [bReserved= %d]\n", pfd.cAuxBuffers, pfd.iLayerType, pfd.bReserved);
    printf("  [dwLayerMask= %d] [dwVisibleMask= %d] [dwDamageMask= %d]\n", pfd.dwLayerMask, pfd.dwVisibleMask, pfd.dwDamageMask);
  }
  
  /// setup pixel format
  pfd.dwFlags= pfd.dwFlags| PFD_DRAW_TO_WINDOW| PFD_DRAW_TO_BITMAP| PFD_SUPPORT_OPENGL| PFD_STEREO_DONTCARE; //| PFD_SWAP_EXCHANGE;//| PFD_NEED_PALETTE;

  if(osi.settings.frontBuffer.dblBuffer) pfd.dwFlags|= PFD_DOUBLEBUFFER;
  pfd.cRedBits=     osi.settings.frontBuffer.redSize;
  pfd.cGreenBits=   osi.settings.frontBuffer.greenSize;
  pfd.cBlueBits=    osi.settings.frontBuffer.blueSize;
  pfd.cAlphaBits=   osi.settings.frontBuffer.alphaSize;
  pfd.cDepthBits=   osi.settings.frontBuffer.depthSize;
  pfd.cStencilBits= osi.settings.frontBuffer.stencilSize;
  */


  // front buffer populate from osi.settings
  int pfAtr[50]; int n= 0;
  pfAtr[n++]= WGL_SUPPORT_OPENGL_ARB; pfAtr[n++]= GL_TRUE;
  pfAtr[n++]= WGL_PIXEL_TYPE_ARB; pfAtr[n++]= WGL_TYPE_RGBA_ARB;
  
  if(osi.settings.pixelFormat.renderOnWindow)  { pfAtr[n++]= WGL_DRAW_TO_WINDOW_ARB; pfAtr[n++]= GL_TRUE; }
  if(osi.settings.pixelFormat.renderOnBitmap)  { pfAtr[n++]= WGL_DRAW_TO_BITMAP_ARB; pfAtr[n++]= GL_TRUE; }
  if(osi.settings.pixelFormat.renderOnPBuffer) { pfAtr[n++]= WGL_DRAW_TO_PBUFFER_ARB; pfAtr[n++]= GL_TRUE; }

  pfAtr[n++]= WGL_DOUBLE_BUFFER_ARB;           pfAtr[n++]= (osi.settings.pixelFormat.dblBuffer? GL_TRUE: GL_FALSE);

  /// WGL_NO_ACCELERATION_ARB - only software / WGL_GENERIC_ACCELERATION_ARB MCD? driver / WGL_FULL_ACCELERATION_ARB ICD driver
  if(osi.settings.pixelFormat.onlyAccelerated) { pfAtr[n++]= WGL_ACCELERATION_ARB; pfAtr[n++]= WGL_FULL_ACCELERATION_ARB; }
  if(osi.settings.pixelFormat.transparent)     {
    pfAtr[n++]= WGL_TRANSPARENT_ARB; pfAtr[n++]= GL_TRUE;
    pfAtr[n++]= WGL_TRANSPARENT_RED_VALUE_ARB;   pfAtr[n++]= osi.settings.pixelFormat.redSize;
    pfAtr[n++]= WGL_TRANSPARENT_GREEN_VALUE_ARB; pfAtr[n++]= osi.settings.pixelFormat.greenSize;
    pfAtr[n++]= WGL_TRANSPARENT_BLUE_VALUE_ARB;  pfAtr[n++]= osi.settings.pixelFormat.blueSize;
  }

  pfAtr[n++]= WGL_COLOR_BITS_ARB; pfAtr[n++]= osi.settings.pixelFormat.colorSize;
  pfAtr[n++]= WGL_RED_BITS_ARB;   pfAtr[n++]= osi.settings.pixelFormat.redSize;
  pfAtr[n++]= WGL_GREEN_BITS_ARB; pfAtr[n++]= osi.settings.pixelFormat.blueSize;
  pfAtr[n++]= WGL_BLUE_BITS_ARB;  pfAtr[n++]= osi.settings.pixelFormat.greenSize;
  pfAtr[n++]= WGL_ALPHA_BITS_ARB; pfAtr[n++]= osi.settings.pixelFormat.alphaSize;

  pfAtr[n++]= WGL_DEPTH_BITS_ARB;   pfAtr[n++]= osi.settings.pixelFormat.depthSize;
  pfAtr[n++]= WGL_STENCIL_BITS_ARB; pfAtr[n++]= osi.settings.pixelFormat.stencilSize;

  /// MSAA antialiasing must be enabled on the front buffers
  if(osi.settings.pixelFormat.sampleBuffers) { 
    pfAtr[n++]= WGL_SAMPLE_BUFFERS_ARB; pfAtr[n++]= 1;
    pfAtr[n++]= WGL_SAMPLES_ARB; pfAtr[n++]= osi.settings.pixelFormat.samples;
  }
  
  pfAtr[n++]= 0;  /// terminator
  
  /// an array of pfID's can be made, and further choose from it. ATM just one ID is requested
  int pfID= 0;
  UINT retNrFormats;
  
  if(!((PFNWGLCHOOSEPIXELFORMATARBPROC)r->glExt.wglChoosePixelFormatARB)((HDC)w->_hDC, pfAtr, null, 1, &pfID, &retNrFormats)) {
    // try WinGDI ChoosePixelFormat can be done here - this func calls wglChoosePixelFormatARB, nowadays
    /*    
    if (!(PixelFormat= ChoosePixelFormat(w->_hDC, &pfd))) {  /// lots of checks, don't think any needed
      killGLWindow(w);
      error.simple(func+ "Can't aquire a PixelFormat");
      return false;
    }
    if(!SetPixelFormat(w->_hDC, PixelFormat, &pfd)) {        /// lots of checks, don't think any needed
      killGLWindow(w);
      error.simple(func+ "Can't set PixelFormat");
      return false;
    }
    */
    return false;
  }

  PIXELFORMATDESCRIPTOR pfd;
  DescribePixelFormat((HDC)w->_hDC, pfID, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  // Setting the pixel format of a window more than once can lead to significant complications
  //   for the Window Manager and for multithread applications, so it is not allowed.
  if(!SetPixelFormat((HDC)w->_hDC, pfID, &pfd)) return false;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  GLXFBConfig *fbc= null;
  int nfbc;

  int attr[50]; int n= 0;
  
  attr[n++]= GLX_RENDER_TYPE;   attr[n++]= GLX_RGBA_BIT;
  attr[n++]= GLX_X_VISUAL_TYPE; attr[n++]= GLX_TRUE_COLOR;
  attr[n++]= GLX_DRAWABLE_TYPE; attr[n++]= (osi.settings.pixelFormat.renderOnWindow? GLX_WINDOW_BIT: 0)|
                                           (osi.settings.pixelFormat.renderOnBitmap? GLX_PIXMAP_BIT: 0)|
                                           (osi.settings.pixelFormat.renderOnPBuffer? GLX_PBUFFER_BIT: 0);
  attr[n++]= GLX_DOUBLEBUFFER;  attr[n++]= (osi.settings.pixelFormat.dblBuffer? True: False);
  
  // osi.settings.frontBuffer.onlyAccelerated - CANNOT FIND CORESPONDING LINUX ATTR

  // osi.settings.frontBuffer.colorSize - CANNOT FIND CORESPONDING LINUX ATTR
  attr[n++]= GLX_RED_SIZE;     attr[n++]= osi.settings.pixelFormat.redSize;
  attr[n++]= GLX_GREEN_SIZE;   attr[n++]= osi.settings.pixelFormat.blueSize;
  attr[n++]= GLX_BLUE_SIZE;    attr[n++]= osi.settings.pixelFormat.greenSize;
  attr[n++]= GLX_ALPHA_SIZE;   attr[n++]= osi.settings.pixelFormat.alphaSize;
  attr[n++]= GLX_DEPTH_SIZE;   attr[n++]= osi.settings.pixelFormat.depthSize;
  attr[n++]= GLX_STENCIL_SIZE; attr[n++]= osi.settings.pixelFormat.stencilSize;

  if(osi.settings.pixelFormat.transparent)     {
    attr[n++]= GLX_TRANSPARENT_TYPE;        attr[n++]= GLX_TRANSPARENT_RGB;
    attr[n++]= GLX_TRANSPARENT_RED_VALUE;   attr[n++]= osi.settings.pixelFormat.redSize;
    attr[n++]= GLX_TRANSPARENT_GREEN_VALUE; attr[n++]= osi.settings.pixelFormat.greenSize;
    attr[n++]= GLX_TRANSPARENT_BLUE_VALUE;  attr[n++]= osi.settings.pixelFormat.blueSize;
  }

  /// MSAA antialiasing must be enabled on the front buffers
  if(osi.settings.pixelFormat.sampleBuffers) { // GLX_ARB_multisample
    attr[n++]= GLX_SAMPLE_BUFFERS_ARB; attr[n++]= 1;
    attr[n++]= GLX_SAMPLES_ARB;        attr[n++]= osi.settings.pixelFormat.samples;
  }

  // GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB - GLX_ARB_framebuffer_sRGB

  attr[n++]= 0;  /// terminator
  

  fbc= glXChooseFBConfig(osi._dis, w->monitor->_screen, attr, &nfbc);
  if(!fbc || !nfbc) {
    error.simple("Failed to retrieve any framebuffer configs with current settings");
    return false;
  }

  if(w->_vi) XFree(w->_vi);
  w->_vi= null;
  w->_fbID= 0;

  /// find a FB that can have a visual - not all can have visuals
  for(int a= 0; a< nfbc; a++) {
    w->_vi= glXGetVisualFromFBConfig(osi._dis, fbc[a]);
    if(!w->_vi) continue;
    w->_fbID= fbc[a];
    break;
  }
  XFree(fbc);

  if((!w->_vi)|| (!w->_fbID)) { error.simple("No visual/front buffer config found with current settings"); return false; }
  #endif /// OS_LINUX

  #ifdef OS_MAC
  // nothing to do - pixel format is tied to the renderer, windows are created using the renderer's pixel format
  #endif /// OS_MAC

  return true;
}


/// [internal] create configured context - after renderer is allocated / context funcs are got / front buffer is created
bool _osiCreateContext(osiWindow *w, osiGlRenderer *r) {
  bool chatty= true;
  
  #ifdef OS_WIN

  // requested version < 3.0
  if(osi.settings.glRenderer.minVerMajor< 3) {
    r->glContext= wglCreateContext((HDC)w->_hDC);
    if(osi.settings.glRenderer.shareGroup)
      wglShareLists(osi.settings.glRenderer.shareGroup->glContext, r->glContext);
    
  // requested version > 3.0
  } else {
    // https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    int attr[20]; int n= 0;
    if(osi.settings.glRenderer.minVerMajor && osi.settings.glRenderer.minVerMinor) {
      attr[n++]= WGL_CONTEXT_MAJOR_VERSION_ARB; attr[n++]= osi.settings.glRenderer.minVerMajor;
      attr[n++]= WGL_CONTEXT_MINOR_VERSION_ARB; attr[n++]= osi.settings.glRenderer.minVerMinor;
    }
    if(osi.settings.glRenderer.legacyCompatibility) {
      attr[n++]= WGL_CONTEXT_PROFILE_MASK_ARB;  attr[n++]= WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    } else {
      attr[n++]= WGL_CONTEXT_PROFILE_MASK_ARB;  attr[n++]= WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
      /// not entirely shure what exactly this does, it removes compatibility extension, for shure, but what else will it remove in the future?
      attr[n++]= WGL_CONTEXT_FLAGS_ARB;         attr[n++]= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
    }
    /// usually more checks / debug info / BUT SLOWER - USE FOR DEBUG only; check 
    if(osi.settings.glRenderer.debugRenderer) {
      attr[n++]= WGL_CONTEXT_FLAGS_ARB; attr[n++]= WGL_CONTEXT_DEBUG_BIT_ARB; }
    attr[n]= 0;
        
    r->glContext= ((PFNWGLCREATECONTEXTATTRIBSARBPROC)r->glExt.wglCreateContextAttribsARB)((HDC)w->_hDC, (osi.settings.glRenderer.shareGroup? osi.settings.glRenderer.shareGroup->glContext: NULL), attr);
    if(!r->glContext) return false;
  }
  #endif /// OS_WIN

  #ifdef OS_LINUX
  // ogl <3
  if(osi.settings.glRenderer.minVerMajor< 3) {
    r->glContext= glXCreateContext(w->_dis, w->_vi, (osi.settings.glRenderer.shareGroup? osi.settings.glRenderer.shareGroup->glContext: NULL), GL_TRUE);

  // ogl 3>
  } else {
    // https://www.opengl.org/registry/specs/ARB/glx_create_context.txt
    int attr[10]; int n= 0;
    if(osi.settings.glRenderer.minVerMajor && osi.settings.glRenderer.minVerMinor) {
      attr[n++]= GLX_CONTEXT_MAJOR_VERSION_ARB; attr[n++]= osi.settings.glRenderer.minVerMajor;
      attr[n++]= GLX_CONTEXT_MINOR_VERSION_ARB; attr[n++]= osi.settings.glRenderer.minVerMinor;
    }
    if(osi.settings.glRenderer.legacyCompatibility) {
      attr[n++]= GLX_CONTEXT_PROFILE_MASK_ARB;  attr[n++]= GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    } else {
      attr[n++]= GLX_CONTEXT_PROFILE_MASK_ARB;  attr[n++]= GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
      /// not entirely shure what exactly this does, it removes compatibility extension, for shure, but what else will it remove in the future?
      attr[n++]= GLX_CONTEXT_FLAGS_ARB;         attr[n++]= GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
    }
    /// usually more checks / debug info / BUT SLOWER - USE FOR DEBUG only;
    if(osi.settings.glRenderer.debugRenderer) {
      attr[n++]= GLX_CONTEXT_FLAGS_ARB; attr[n++]= GLX_CONTEXT_DEBUG_BIT_ARB;
    }
    attr[n]= 0;

    r->glContext= ((PFNGLXCREATECONTEXTATTRIBSARBPROC)r->glExt.glXCreateContextAttribsARB)(osi._dis, w->_fbID, (osi.settings.glRenderer.shareGroup? osi.settings.glRenderer.shareGroup->glContext: NULL), True, attr);
  }
  #endif /// OS_LINUX


  #ifdef OS_MAC
  uint32_t oglDisplayMask= 0;
  if(w) {
    if(w->mode== 4) {
      for(int a= 0; a< osi.display.nrMonitors; a++)
        oglDisplayMask|= osi.display.monitor[a]._oglDisplayMask;
    } else if(osi.settings.rendererCreation== osiSettings::onePerGPU) {
      for(int a= 0; a< w->monitor->GPU->nrMonitors; a++)
        oglDisplayMask|= w->monitor->GPU->monitor[a]->_oglDisplayMask;
    } else if(osi.settings.rendererCreation== osiSettings::onePerMonitor) {
      oglDisplayMask|= w->monitor->_oglDisplayMask;
    } else if(osi.settings.rendererCreation== osiSettings::onlyOne) {
    }
  }
  
  cocoa.createContext(r, oglDisplayMask);   /// (osiGlRenderer, on what monitors should work)
  #endif /// OS_MAC

  return true;
}




















///=================///
// osiGLRENDERER class //
///=================///


osiGlRenderer *_glr; // CURRENTLY ACTIVE RENDERER, same as [osi.glr]


osiGlRenderer::osiGlRenderer()
#ifndef OS_WIN
: glExt( ::glExt)        /// set the reference to the global struct that holds all the functions 
#endif
  {
  type= 0;
  glContext= null;
  isActive= false;
  glVerMajor= glVerMinor= 0;
  maxTexelUnits= 0;
  max3Dtexture= 0;
  monitor= null;
  maxTextureAnisotropy= 0;
  GPU= null;

  #ifdef OS_WIN
  _bContextFuncsGot= false;
  #else /// OS_MAC & OS_LINUX

  #endif

  #ifdef OS_MAC
  _pixelFormat= null;
  #endif /// OS_MAC
    
  int16 a;
  /// initial ARB list
  for(a= 0; _glARBlistEmpty[a].desc!= null; a++);     /// find out how big the list is
  glARBlist= new osiGlExt[a+ 1];
  for(a= 0; _glARBlistEmpty[a].desc!= null; a++)
    glARBlist[a]= _glARBlistEmpty[a];

  /// initial EXT list
  for(a= 0; _glEXTlistEmpty[a].desc!= null; a++);     /// find out how big the list is
  glEXTlist= new osiGlExt[a+ 1];
  for(a= 0; _glEXTlistEmpty[a].desc!= null; a++)
    glEXTlist[a]= _glEXTlistEmpty[a];
  
  /// initial OTHER list
  for(a= 0; _glOTHERlistEmpty[a].desc!= null; a++);   /// find out how big the list is
  glOTHERlist= new osiGlExt[a+ 1];
  for(a= 0; _glOTHERlistEmpty[a].desc!= null; a++)
    glOTHERlist[a]= _glOTHERlistEmpty[a];

  #ifndef OS_MAC // OS_WIN + OS_LINUX
  
  /// clear the whole extensions struct
  uint8 *p= (uint8 *)&glExt;
  for(size_t a= sizeof(glExt); a> 0; a--)
    *p= null, p++;

  /* (REMOVED - set all glExt funcs to point to the glExtNULL func (defined in osiGlExt.cpp)) - NOT SAFE
  void **p= (void **)&glExt._start;      // _start must be the first func in the structure
  for(int a= 0; a< ((sizeof(glExt)- sizeof(int8))/ sizeof(void *)); a++) {
    *p= (void*)&_osiGlExtNULL;
    p++;
  }
  */

  #endif /// OS_WIN + OS_LINUX
}


osiGlRenderer::~osiGlRenderer() {
  delData();
  
  
  #ifdef OS_MAC  
  if(_pixelFormat)
    cocoa.delPixelFormat(_pixelFormat);
  _pixelFormat= null;
  #endif
}


void osiGlRenderer::delData() {
  isActive= false;
  monitor= null;

  if(glARBlist) 
    delete[] glARBlist;
  glARBlist= null;

  if(glEXTlist)
    delete[] glEXTlist;
  glEXTlist= null;

  if(glOTHERlist)
    delete[] glOTHERlist;
  glOTHERlist= null;

  if(glContext) {
    #ifdef OS_WIN
    wglDeleteContext(glContext);
    glContext= 0;
    #endif
    #ifdef OS_LINUX
    glXDestroyContext(osi._dis, glContext);
    glContext= 0;
    #endif
    #ifdef OS_MAC
    cocoa.delContext(glContext);
    cocoa.delPixelFormat(_pixelFormat);
    glContext= null;
    _pixelFormat= null;
    #endif
  }
}



void _osiParseBigExtString(osiGlRenderer *, cchar *);  /// used for next func
void _osiParseExtString(osiGlRenderer *, cchar *);     /// used for next func

void osiGlRenderer::checkExt() {
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  #endif

  bool thisIsMac= false;
  #ifdef OS_MAC
  thisIsMac= true;
  #endif 
  
  cuint8 *ext= null;

  
  /// basic renderer characteristics
  glVendor= (cchar *)glGetString(GL_VENDOR);                    /// graphics card vendor string
  glRenderer= (cchar *)glGetString(GL_RENDERER);                /// oGL 'renderer' string
  glVersion= (cchar *)glGetString(GL_VERSION);                  /// oGL version in a string format
  #ifdef OS_MAC
  CGLGetVersion(&glVerMajor, &glVerMinor);
  #else 
  glGetIntegerv(GL_MAJOR_VERSION, &glVerMajor);                 /// oGL major version
  glGetIntegerv(GL_MINOR_VERSION, &glVerMinor);                 /// oGL minor version
  #endif
  
  #if(GL_EXT_texture_filter_anisotropic== 1)
  glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxTextureAnisotropy); /// maximum texture anisotropy
  #endif

  // deprecated glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTexelUnits);      /// maximum texel units
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexelUnits);      /// maximum texel units
  
  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max3Dtexture);     /// maximum 3D texture size

  #ifdef OSI_BE_CHATTY
  if(chatty) {
    printf("context[%lld]\n", (long long)(this->glContext));
    //printf("GL extensions: %s\n", glGetString(GL_EXTENSIONS));
    printf("Vendor Name: %s\n", glVendor.d);
    printf("Graphic Card: %s\n", glRenderer.d);
    printf("GL Version: %s\n", glVersion.d);
    printf("Texel units avaible: %d\n", maxTexelUnits);
    printf("Maximum 3d texture size %dx%dx%d\n", max3Dtexture, max3Dtexture, max3Dtexture);
    printf("\nTesting GL Extensions...\n");
  }
  #endif

  /// OpenGL 2.x and less
  if(glVerMajor< 3 || thisIsMac) {
    /// there are 2 extensions for windows and linux, but they do the same thing: a big string with all supported extensions
    ext= (cuint8 *)glGetString(GL_EXTENSIONS);
    _osiParseBigExtString(this, (cchar *)ext);

  /// OpenGL 3.x and over
  } else {
    #ifndef OS_MAC
    _osiGetGlProc("glGetStringi", (void **)&glExt.glGetStringi);      /// make shure this func is avaible

    int max;
    glGetIntegerv(GL_NUM_EXTENSIONS, &max);

    for(short a= 0; a< max; a++) {
      ext= (cuint8 *)((PFNGLGETSTRINGIPROC)glExt.glGetStringi)(GL_EXTENSIONS, a);
      _osiParseExtString(this, (cchar *)ext);
    } /// pass thru all extension strings
    #endif /// linux + win

  } /// lower than gl3.x or over gl3.x

  /// 'some' wgl and glx extensions are in the normal extension string. the rest must be aquired with separate funcs
  #ifdef OS_WIN
  _osiGetGlProc("wglGetExtensionsStringARB", (void **)&glExt.wglGetExtensionsStringARB);      /// make shure this func is avaible

  if(glExt.wglGetExtensionsStringARB) {
    ext= (cuint8 *)((PFNWGLGETEXTENSIONSSTRINGARBPROC)glExt.wglGetExtensionsStringARB)(wglGetCurrentDC());
    _osiParseBigExtString(this, (cchar *)ext);
  }
  #endif /// OS_WIN

  #ifdef OS_LINUX
  ext= (cuint8 *)glXQueryExtensionsString(osi._dis, (monitor? monitor->_screen: 0));
  _osiParseBigExtString(this, (cchar *)ext);
  #endif

}


void _osiParseExtString(osiGlRenderer *r, cchar *ext) {
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  #endif

  /// search the ARB list
  int i= _osiSearchARB(r, ext);
  if(i != -1) {
    r->glARBlist[i].avaible= true;           /// found it in ARB
    
  /// search EXT list
  } else {
    i= _osiSearchEXT(r, ext);
    if(i != -1) {
      r->glEXTlist[i].avaible= true;         /// found it in EXT

    /// search OTHER list
    } else {
      i= _osiSearchOTHER(r, ext);
      if(i != -1) {
        r->glOTHERlist[i].avaible= true;     /// found it in OTHER

      /// not found in any list
      }
      #ifdef OSI_BE_CHATTY
      else if(chatty) printf("UNKNOWN extension: [%s]\n", ext);
      #endif
    }
  }
}


void _osiParseBigExtString(osiGlRenderer *r, cchar *ext) {
  if(!ext) return;
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  #endif
  uint8 buf[128];      /// hopefully extension names won't be bigger than 128... 
  uint8 *p= (uint8 *)ext;

  while(1) {
  
    /// parse 1 extension at a time
    for(int16 a= 0; a< 128; a++) {
      if(*p== ' ' || *p== '\0') {  /// extension delimiter or end of string
        buf[a]= 0;
        break;
      }

      buf[a]= *p;
      p++;
    } /// for each character in current ext string

    _osiParseExtString(r, (cchar *)buf);

    if(*p== '\0')
      return;                               /// reached the end
    else
      p++;                                  /// pass the ' ' character, next extension
  } /// for each extension string
}







/// this aquires all extension funcs
void osiGlRenderer::getExtFuncs() {
  if(glExt.initialized) return;             /// if functions already aquired, just return
  
  _osiGetVERfuncs(this, glVerMajor, glVerMinor);/// gl1.0 to current
  _osiGetARBfuncs(this);                        /// all ARB or KHR funcs
  _osiGetEXTfuncs(this);                        /// all EXT and vendor funcs
  _osiGetOTHERfuncs(this);                      /// all funcs not in ARB or EXT

  glExt.initialized= true;                  /// mark glExt funcs as aquired
}


bool osiGlRenderer::isExtensionARB(cchar *ext) {
  int n= _osiSearchARB(this, ext);
  if(n== -1) return false;
  else       return glARBlist[n].avaible;
}


bool osiGlRenderer::isExtensionEXT(cchar *ext) {
  int n= _osiSearchEXT(this, ext);
  if(n== -1) return false;
  else       return glEXTlist[n].avaible;
}


bool osiGlRenderer::isExtensionOTHER(cchar *ext) {
  int n= _osiSearchOTHER(this, ext);
  if(n== -1) return false;
  else       return glOTHERlist[n].avaible;
}








/// [internal] searches for specified extension's array index, returns -1 if not found 
int _osiSearchARB(osiGlRenderer *r, cchar *s) {
  for(short a= 0; r->glARBlist[a].desc!= null; a++)   /// pass thru r's ARB extensions
    if(!strcmp8(r->glARBlist[a].desc, s))     /// found
      return a;

  if(!strcmp8("GLX_ARB_multisample", s)) return 4;
  if(!strcmp8("WGL_ARB_multisample", s)) return 4;
  if(!strcmp8("GLX_ARB_vertex_buffer_object", s)) return 27;
  if(!strcmp8("WGL_ARB_pixel_format_float", s)) return 38;
  if(!strcmp8("GLX_ARB_fbconfig_float", s)) return 38;
  if(!strcmp8("GLX_ARB_framebuffer_sRGB", s)) return 45;
  if(!strcmp8("WGL_ARB_framebuffer_sRGB", s)) return 45;
  //if(!strcmp8("WGL_ARB_create_context_profile", s)) return 54;
  //if(!strcmp8("GLX_ARB_create_context_profile", s)) return 55;
  if(!strcmp8("GL_ARB_robustness_share_group_isolation", s)) return 125;
  if(!strcmp8("GLX_ARB_robustness_share_group_isolation", s)) return 141;
  if(!strcmp8("WGL_ARB_robustness_share_group_isolation", s)) return 142;
  if(!strcmp8("GLX_ARB_context_flush_control", s)) return 167;
  if(!strcmp8("WGL_ARB_context_flush_control", s)) return 167;

  return -1;                                          /// not found (if reached this point)
}

/// [internal] searches for specified extension's array index, returns -1 if not found
int _osiSearchEXT(osiGlRenderer *r, cchar *s) {
  for(short a= 0; r->glEXTlist[a].desc!= null; a++)   /// pass thru r's EXT extensions
    if(!strcmp8(r->glEXTlist[a].desc, s))     /// found
      return a;

  if(!strcmp8("GL_SGIX_pixel_texture", s)) return 14;
  if(!strcmp8("GLX_SGIS_multisample", s)) return 24;
  if(!strcmp8("GLX_SGIX_dmbuffer", s)) return 85;
  if(!strcmp8("GLX_SGIS_color_range", s)) return 114;
  if(!strcmp8("GLX_EXT_scene_marker", s)) return 119;
  if(!strcmp8("GL_SUN_multi_draw_arrays", s)) return 147;
  if(!strcmp8("GLX_3DFX_multisample", s)) return 206;
  if(!strcmp8("WGL_3DFX_multisample", s)) return 206;
  if(!strcmp8("WGL_EXT_multisample", s)) return 208;
  if(!strcmp8("GL_SGIX_vertex_preclip_hint", s)) return 209;
  if(!strcmp8("WGL_NV_float_buffer", s)) return 280;
  if(!strcmp8("GLX_NV_float_buffer", s)) return 280;
  if(!strcmp8("WGL_EXT_pixel_format_packed_float", s)) return 327;
  if(!strcmp8("GLX_EXT_fbconfig_packed_float", s)) return 327;
  if(!strcmp8("GL_NV_texture_compression_latc", s)) return 330;
  if(!strcmp8("GLX_EXT_framebuffer_sRGB", s)) return 336;
  if(!strcmp8("WGL_EXT_framebuffer_sRGB", s)) return 336;
  if(!strcmp8("GLX_NV_present_video", s)) return 346;
  if(!strcmp8("WGL_NV_present_video", s)) return 346;
  if(!strcmp8("GLX_NV_video_capture", s)) return 373;
  if(!strcmp8("WGL_NV_video_capture", s)) return 373;
  if(!strcmp8("WGL_NV_copy_image", s)) return 375;
  if(!strcmp8("GLX_NV_copy_image", s)) return 375;
  if(!strcmp8("GL_NV_gpu_program_fp64", s)) return 387;
  if(!strcmp8("GLX_NV_multisample_coverage", s)) return 392;
  if(!strcmp8("WGL_NV_multisample_coverage", s)) return 392;
  if(!strcmp8("GLX_EXT_create_context_es2_profile", s)) return 398;
  if(!strcmp8("WGL_EXT_create_context_es2_profile", s)) return 399;
  if(!strcmp8("GL_NV_blend_equation_advanced_coherent", s)) return 432;
  if(!strcmp8("GL_KHR_blend_equation_advanced_coherent", s)) return 457;

  return -1;
}

/// [internal] searches for specified extension's array index, returns -1 if not found
int _osiSearchOTHER(osiGlRenderer *r, cchar *s) {
  for(short a= 0; r->glOTHERlist[a].desc!= null; a++) /// pass thru r's OTHER extensions
    if(!strcmp8(r->glOTHERlist[a].desc, s))           /// found
      return a;

  return -1;                                          /// not found (if reached this point)
}










// VAO helper class functions ==========--------------

int _VAOnrenderers= 0;
int _VAOrenderer= 0;


void _osiCountRenderers() {
  _VAOnrenderers= 0;
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first;
  while(p) {
    _VAOnrenderers++;
    p= (osiGlRenderer *)p->next;
  }
}

void glVAO::genArray() {
  /// count the number of renderers at current time
  if(!_VAOnrenderers) {
    _osiCountRenderers();
    if(!_VAOnrenderers)   /// if no renderer is created, return
      return;
  }
  id= new uint[_VAOnrenderers];
  if(!id) { error.simple("Memory allocation failed"); return; }

  /// pass thru all renderers and create a VAO on each
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first,   *currentR= osi.glr;
  osiWindow *w= null,                                     *currentW= osi.glrWin;

  for(int a= 0; a< _VAOnrenderers; a++) {
    /// search for the window that has this renderer
    for(int b= 0; b< OSI_MAX_WINDOWS; b++)
      if(osi.win[b].renderer== p) {
        w= &osi.win[b];
        break;
      }

    /// generate the VAO for this renderer
    if(w) {
      if((osi.glr!= p) || (osi.glrWin!= w))
        osi.glMakeCurrent(p, w);
      glGenVertexArrays(1, id+ a);
      p= (osiGlRenderer *)p->next;
    }
  } /// for each renderer

  /// return to the previous active renderer & window if any changed
  if((osi.glr!= currentR) || (osi.glrWin!= currentW))
    osi.glMakeCurrent(currentR, currentW);
}


void glVAO::delArray() {
  /// pass thru all renderers and create a VAO on each
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first,   *currentR= osi.glr;
  osiWindow *w= null,                                     *currentW= osi.glrWin;

  for(int a= 0; a< _VAOnrenderers; a++) {
    /// search for the window that has this renderer
    for(int b= 0; b< OSI_MAX_WINDOWS; b++)
      if(osi.win[b].renderer== p) {
        w= &osi.win[b];
        break;
      }

    /// generate the VAO for this renderer
    if(w) {
      if((osi.glr!= p) || (osi.glrWin!= w))
        osi.glMakeCurrent(p, w);
      glDeleteVertexArrays(1, id+ a);
      p= (osiGlRenderer *)p->next;
    }
  } /// for each renderer

  /// return to the previous active renderer & window if any changed
  if((osi.glr!= currentR) || (osi.glrWin!= currentW))
    osi.glMakeCurrent(currentR, currentW);
  delData();
}


void glVAO::bindAndVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
  /// pass thru all renderers and create a VAO on each
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first,   *currentR= osi.glr;
  osiWindow *w= null,                                     *currentW= osi.glrWin;

  for(int a= 0; a< _VAOnrenderers; a++) {
    /// search for the window that has this renderer
    for(int b= 0; b< OSI_MAX_WINDOWS; b++)
      if(osi.win[b].renderer== p) {
        w= &osi.win[b];
        break;
      }

    /// generate the VAO for this renderer
    if(w) {
      if((osi.glr!= p) || (osi.glrWin!= w))
        osi.glMakeCurrent(p, w);
      glBindVertexArray(id[a]);
      glVertexAttribIPointer(index, size, type, stride, pointer);
      p= (osiGlRenderer *)p->next;
    }
  } /// for each renderer

  /// return to the previous active renderer & window if any changed
  if((osi.glr!= currentR) || (osi.glrWin!= currentW))
    osi.glMakeCurrent(currentR, currentW);
}


void glVAO::bindAndVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
  /// pass thru all renderers and create a VAO on each
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first,   *currentR= osi.glr;
  osiWindow *w= null,                                     *currentW= osi.glrWin;

  for(int a= 0; a< _VAOnrenderers; a++) {
    /// search for the window that has this renderer
    for(int b= 0; b< OSI_MAX_WINDOWS; b++)
      if(osi.win[b].renderer== p) {
        w= &osi.win[b];
        break;
      }

    /// generate the VAO for this renderer
    if(w) {
      if((osi.glr!= p) || (osi.glrWin!= w))
        osi.glMakeCurrent(p, w);
      glBindVertexArray(id[a]);
      glVertexAttribPointer(index, size, type, normalized, stride, pointer);
      p= (osiGlRenderer *)p->next;
    }
  } /// for each renderer

  /// return to the previous active renderer & window if any changed
  if((osi.glr!= currentR) || (osi.glrWin!= currentW))
    osi.glMakeCurrent(currentR, currentW);
}

void glVAO::enableVertexAttribArray(GLuint n) {
  /// pass thru all renderers and create a VAO on each
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first,   *currentR= osi.glr;
  osiWindow *w= null,                                     *currentW= osi.glrWin;

  for(int a= 0; a< _VAOnrenderers; a++) {
    /// search for the window that has this renderer
    for(int b= 0; b< OSI_MAX_WINDOWS; b++)
      if(osi.win[b].renderer== p) {
        w= &osi.win[b];
        break;
      }

    /// generate the VAO for this renderer
    if(w) {
      if((osi.glr!= p) || (osi.glrWin!= w))
        osi.glMakeCurrent(p, w);
      glBindVertexArray(id[a]);
      glEnableVertexAttribArray(n);
      p= (osiGlRenderer *)p->next;
    }
  } /// for each renderer

  /// return to the previous active renderer & window if any changed
  if((osi.glr!= currentR) || (osi.glrWin!= currentW))
    osi.glMakeCurrent(currentR, currentW);
}


void glVAO::disableVertexAttribArray(GLuint n) {
  /// pass thru all renderers and create a VAO on each
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first,   *currentR= osi.glr;
  osiWindow *w= null,                                     *currentW= osi.glrWin;

  for(int a= 0; a< _VAOnrenderers; a++) {
    /// search for the window that has this renderer
    for(int b= 0; b< OSI_MAX_WINDOWS; b++)
      if(osi.win[b].renderer== p) {
        w= &osi.win[b];
        break;
      }

    /// generate the VAO for this renderer
    if(w) {
      if((osi.glr!= p) || (osi.glrWin!= w))
        osi.glMakeCurrent(p, w);
      glBindVertexArray(id[a]);
      glDisableVertexAttribArray(n);
      p= (osiGlRenderer *)p->next;
    }
  } /// for each renderer

  /// return to the previous active renderer & window if any changed
  if((osi.glr!= currentR) || (osi.glrWin!= currentW))
    osi.glMakeCurrent(currentR, currentW);
}


/* might be needed, but not sure
void glVAO::bindBuffer(GLenum target, GLuint buffer) {
  /// pass thru all renderers and create a VAO on each
  osiGlRenderer *p= (osiGlRenderer *)osi.glRenderers.first,   *currentR= osi.glr;
  osiWindow *w= null,                                     *currentW= osi.glrWin;

  for(int a= 0; a< _VAOnrenderers; a++) {
    /// search for the window that has this renderer
    for(int b= 0; b< MAX_WINDOWS; b++)
      if(osi.win[b].glr== p) {
        w= &osi.win[b];
        break;
      }

    /// generate the VAO for this renderer
    if(w) {
      if((osi.glr!= p) || (osi.glrWin!= w))
        osi.glMakeCurrent(p, w);
      glBindVertexArray(id[a]);
      glBindBuffer(target, buffer);
      p= (osiGlRenderer *)p->next;
    }
  } /// for each renderer

  /// return to the previous active renderer & window if any changed
  if((osi.glr!= currentR) || (osi.glrWin!= currentW))
    osi.glMakeCurrent(currentR, currentW);
}
*/

#endif /// OSI_USE_OPENGL






















