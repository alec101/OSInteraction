#pragma once

/// BUG: [linux64] icon data is 64bit ints, it should be 32, keep an eye if this bug gets fixed (on X11's part)


// 64 may be too much i think...
#define OSI_MAX_WINDOWS 64


//class osiGlRenderer;
//class osiVkRenderer;


///==========================================================================///
// -----------------============ WINDOW CLASS =============------------------ //
///==========================================================================///

class osiWindow {
public:

  str8 name;                // window name (program name)
  bool isCreated;           // window has been created flag
  bool hasFocus;            // has input focus (if no window has focus, program lost focus)
  bool isMinimized;         // the window is minimized
  int32_t x0, y0, dx, dy;   // window position & size
  int16_t freq;             // frequency (if used - fullscreen)
  int8_t mode;              // 1= windowed, 2= fullscreen, 3= fullscreen window, 4= full virtual desktop
  osiMonitor *monitor;      // on what monitor it is drawn
  osiRenderer *renderer;    // renderer assigned to this window (vulkan or ogl)

  #ifdef OSI_USE_VKO
  void *vkSurface;          // VkSurfaceKHR tied to this window
  #endif

  // funcs
  
  void setName(const char *);
  bool setIcon(const char *);   // sets an icon from specified image file name (png/tga so far); in linux, icon must be set before the window is shown
  void show();
  void hide();
  void move(int x, int y);      // in virtual desktop coordinates
  void resize(int dx, int dy);
  
  // constructor / destructor
  
  osiWindow();
  ~osiWindow();
  void delData();

  // internal data from here

  #ifdef OS_WIN
  uint64_t _hDC;         // private GDI device context
  uint64_t _hWnd;        // holds program window handle
  uint64_t _hInstance;   // holds the instance of the application ?? isn't this per window...
  
  //static MSG _msg;       // windows message struct... this is just needed for checkMSG();

  uint64_t _imgDC;       // background - hdc for the DIB bitmap
  uint64_t _imgBM;       // background - handle for the DIB bitmap
  #endif ///OS_WIN

  #ifdef OS_LINUX
  Window _root;           // root window (it is different on each monitor)
  static Display *_dis;   // display 'handle'. nowadays there is only 1 display, and 1 big (virtual) screen.
  Window _win;            // window 'handle' or watever
  XWindowAttributes _gwa; // window attributes (size/etc)
  XVisualInfo *_vi;       // this can be extracted from _fbID, but only on newer versions
  #ifdef OSI_USE_OPENGL
  GLXFBConfig _fbID;      // front buffer ID
  #endif

  bool _isMapped;         // internal flag used when resolution is changing <<NOT USED ANYMORE I THINK>>
  uint64_t *_iconData;    // NEEDS TO BE ULONG (64bit for linux64) due to a bug - it might be fixed in the future

  GC _gc;                 // background - gr context
  XImage *_img;           // background - X11 image
  uint8_t *_imgData;      // background - image pixel data

  /// specific linux window propreties functions
  void _setWMprop(const char *wmID, const char *wmProp, uint32_t val1, uint32_t val2= 0); // documentation is @ end of osinteraction.h
  void _setWMstate(uint32_t val, const char *prop1, const char *prop2= (const char*)0);   // documentation is @ end of osinteraction.h
  void _setWMtype(const char *wmType);  // sets the _NET_WM_WINDOW_TYPE http://standards.freedesktop.org/wm-spec/latest/ar01s05.html
  #ifdef OSI_USE_OPENGL
  bool _glCreateFBandVisual();            // creates front buffer object and visual info (need monitor to point to a valid monitor)
  #endif
  #ifdef OSI_USE_VKO
  bool _vkCreateVisual();
  #endif
  #endif

  #ifdef OS_MAC
  void *_win;                 // MacWindow
  void *_view;                // MacGLview
  void *_img;                 // background image (splash windows)
  #endif
private:
  bool _isSplashWindow;
  friend class osinteraction;
};






/*
 * Linux window types:
 * 
 * Window type is set with _NET_WM_WINDOW_TYPE. It can be set to one of:
 * 
 * _NET_WM_WINDOW_TYPE_DESKTOP
 * _NET_WM_WINDOW_TYPE_DOCK
 * _NET_WM_WINDOW_TYPE_TOOLBAR
 * _NET_WM_WINDOW_TYPE_MENU
 * _NET_WM_WINDOW_TYPE_UTILITY
 * _NET_WM_WINDOW_TYPE_SPLASH
 * _NET_WM_WINDOW_TYPE_DIALOG
 * _NET_WM_WINDOW_TYPE_DROPDOWN_MENU
 * _NET_WM_WINDOW_TYPE_POPUP_MENU
 * _NET_WM_WINDOW_TYPE_TOOLTIP
 * _NET_WM_WINDOW_TYPE_NOTIFICATION
 * _NET_WM_WINDOW_TYPE_COMBO
 * _NET_WM_WINDOW_TYPE_DND
 * _NET_WM_WINDOW_TYPE_NORMAL
 * 
 * 
 * 
 */


// LINUX specific window proprety funcs
/*
 * ====================================================================================================
 * void setWMstate(uint val, string8 prop1, string8 prop2= (cchar*)0);
 *   val: 0= remove/unset property, 1= add/set property, 2= toggle property
 *   prop1 & 2 can be:
 *    "_NET_WM_STATE_MODAL"= indicates that this is a modal dialog box. If the WM_TRANSIENT_FOR
 *                           hint is set to another toplevel window, the dialog is modal for
 *                           that window; if WM_TRANSIENT_FOR is not set or set to the root window
 *                           the dialog is modal for its window group.
 *    "_NET_WM_STATE_STICKY"= indicates that the Window Manager SHOULD keep the window's position
 *                            fixed on the screen, even when the virtual desktop scrolls.
 *    "_NET_WM_STATE_MAXIMIZED_VERT"= indicates that the window is vertically maximized. (vert can be prop1, horiz prop2)
 *    "_NET_WM_STATE_MAXIMIZED_HORZ"= indicates that the window is horizontally maximized. (vert can be prop1, horiz prop2)
 *    "_NET_WM_STATE_SHADED"= indicates that the window is shaded.
 *    "_NET_WM_STATE_SKIP_TASKBAR"= indicates that the window should not be included on a taskbar.
 *                                  This hint should be requested by the application,
 *                                  i.e. it indicates that the window by nature is never in
 *                                  the taskbar. Applications should not set this hint if
 *                                  _NET_WM_WINDOW_TYPE already conveys the exact nature of the window.
 *    "_NET_WM_STATE_SKIP_PAGER"= indicates that the window should not be included on a Pager.
 *                                This hint should be requested by the application, i.e. it indicates
 *                                that the window by nature is never in the Pager. Applications should
 *                                not set this hint if _NET_WM_WINDOW_TYPE already conveys the exact nature of the window.
 *    "_NET_WM_STATE_HIDDEN"= should be set by the Window Manager to indicate that a window would not be
 *                            visible on the screen if its desktop/viewport were active and its coordinates
 *                            were within the screen bounds. The canonical example is that minimized windows
 *                            should be in the _NET_WM_STATE_HIDDEN state. Pagers and similar applications
 *                            should use _NET_WM_STATE_HIDDEN instead of WM_STATE to decide whether
 *                            to display a window in miniature representations of the windows on a desktop.
 *    "_NET_WM_STATE_FULLSCREEN"= indicates that the window should fill the entire screen and have
 *                                no window decorations. Additionally the Window Manager is responsible
 *                                for restoring the original geometry after a switch from fullscreen
 *                                back to normal window. For example, a presentation program would use this hint.
 *    "_NET_WM_STATE_ABOVE"= indicates that the window should be on top of most windows.
 *    "_NET_WM_STATE_BELOW"= indicates that the window should be below most windows.
 * 
 * _NET_WM_STATE_ABOVE and _NET_WM_STATE_BELOW are mainly meant for user preferences and should not be used
 * by applications e.g. for drawing attention to their dialogs (the Urgency hint should be used in that case,
 * see the section called “Urgency”).'
 * 
 *    "_NET_WM_STATE_DEMANDS_ATTENTION"= indicates that some action in or with the window happened.
 *                                       For example, it may be set by the Window Manager if the window
 *                                       requested activation but the Window Manager refused it, or the
 *                                       application may set it if it finished some work. This state may
 *                                       be set by both the Client and the Window Manager. It should be
 *                                       unset by the Window Manager when it decides the window got the
 *                                       required attention (usually, that it got activated).
 *    "_NET_WM_STATE_FOCUSED"= indicates whether the window's decorations are drawn in an active state.
 *                             Clients MUST regard it as a read-only hint. It cannot be set at map time
 *                             or changed via a _NET_WM_STATE client message. The window given by
 *                             _NET_ACTIVE_WINDOW will usually have this hint, but at times other
 *                             windows may as well, if they have a strong association with the active
 *                             window and will be considered as a unit with it by the user. Clients that
 *                             modify the appearance of internal elements when a toplevel has keyboard
 *                             focus SHOULD check for the availability of this state in _NET_SUPPORTED
 *                             and, if it is available, use it in preference to tracking focus via
 *                             FocusIn events. By doing so they will match the window decorations and
 *                             accurately reflect the intentions of the Window Manager.
 * 
 * 
 * ====================================================================================================
 * 
 * void setWMprop(string8 wmID, string8 wmProp, uint val1, uint val2= 0); /// documentation is @ end of osinteraction.h
 * 
 *  this func uses XChangeProperty(...) with atom1 as wmID
 *                                           atom2 as wmProp
 *                                           val1, val2 for data[0], data[1]
 * 
 */









