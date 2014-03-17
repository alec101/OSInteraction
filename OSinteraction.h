#pragma once

// clang WARNING a[--n]= b[n] WILL NOT DECREASE n BEFORE b[n] ( so it will be a[n-1]= b[n]) !!!!!!!!!!!

/// the main defs that are used:
// OS_WIN
// OS_LINUX
// OS_MAC

// start with display.populate(&osi), to scan monitors/resolutions/sizes
// then a window can be created
// createGLWindow / primaryGlWindow/ etcWindow   MODE parameter:
// [mode 1]: windowed, supplied using size, center screen
// [mode 2]: fullscreen, changes resolution of selected monitor with selected sizes
// [mode 3]: fullscreen window on selected monitor
// [mode 4]: full Virtual Screen window, on ALL MONITORS, ignores monitor/size vars passed (can be null i guess)

// USAGE WARNING
// CREATE win[0], wich is the primary window FIRST
// DELETE the primary window LAST


// graphics cards are ignored atm; no usefull data that i can think of can be gathered; (windows)
// only the monitors that are attached to graphics cards are important - all data is gathered on them

//=============//
// LINUX STUFF //
//=============//

// sudo apt-get install mesa-common-dev    GL/gl.h GL/glx.h
// sudo apt-get install libglu1-mesa-dev   GL/glu.h
// or the freeglu one
// sudo apt-get install libx11-dev         for X11/Xlib.h  < probably this is already installed
// sudo apt-get install libxrandr-dev      used for resolution changes
// sudo apt-get install libxi-dev          NOT USED ATM 
// sudo apt-get install libc6-dev-i386     the 32-bit c libraries (only 64bit libs are in linux64)
// sudo apt-get install xxxxxxxxxxxx       the 64-bit c libraries (only 32bit libs are in linux32)





// 64 is too much...
#define MAX_WINDOWS 64



// -----------------============ WINDOW CLASS =============---------------------
class OSIWindow {
public:
  OSIWindow();
  ~OSIWindow();
  void delData();

  string name;              /// window name (program name)
  bool isCreated;           /// window has been created flag
  bool hasFocus;            /// has input focus (if no window has focus, program lost focus)
  short x0, y0, dx, dy;     /// window position & size
  int8 bpp;                 /// bits per pixel (if in fullscreen)
  short freq;               /// frequency (if used - fullscreen)
  int8 mode;                /// 1= windowed, 2= fullscreen, 3= fullscreen window, 4= full virtual desktop
  OSIMonitor *monitor;      /// on what monitor it is drawn

#ifdef OS_WIN
  HGLRC glRenderer;         /// permanent rendering context
  HDC hDC;                  /// private GDI device context
  HWND hWnd;                /// holds program window handle
  HINSTANCE hInstance;      /// holds the instance of the application ?? isn't this per window...
    
  static MSG msg;           /// windows message struct... this is just needed for checkMSG();
#endif ///OS_WIN

#ifdef OS_LINUX
  Window root;              /// root window (it is different on each monitor)
  static Display *dis;      /// display 'handle'. nowadays there is only 1 display, and 1 big (virtual) screen.
  Window win;               /// window 'handle' or watever
  XWindowAttributes gwa;    /// window attributes (size/etc)
  
  GLXContext glRenderer;        /// openGL renderer  <<<<<<<<<<<<<<<<<<<<<<<<<
  
  bool isMapped;            /// internal flag used when resolution is changing

  /// specific linux window propreties functions
  void setWMprop(string8 wmID, string8 wmProp, uint val1, uint val2= 0); /// documentation is @ end of OSinteraction.h
  void setWMstate(uint val, string8 prop1, string8 prop2= (cchar*)0); /// documentation is @ end of OSinteraction.h
 
#endif

#ifdef OS_MAC
  void *win;                 /// MacWindow
  void *view;                /// MacGLview
  void *glRenderer;          // not used/ view has the context. this needs further thinking
#endif
};




// -----------=========== OSINTERACTION CLASS ================------------------
class OSInteraction {
  

public:
  uint64 present;   /// present time, updated in checkMSG(), wich should be the first func in a main loop. present MUST BE UPDATED MANUALLY, each frame, if checkMSG() is not called
  OSInteraction();
  ~OSInteraction();
  void delData();   /// called by destructor
    
// primaryWindow must be expanded with a bool to be set to middle of screen, eventually to have image of the game
// or resize / standard buttons if windowed mode is used for the game
// frequency must be the same 
  bool primaryGLWindow(string name, int dx, int dy, int8 bpp, int8 mode, short freq= 0); // mode: 1= windowed, 2= fullscreen, 3= fullscreeen window(must research this one), 4= fullscreen virtual desktop (every monitor)
  bool primaryGLWindow();       // uses data from OSInteraction::ini

// [mode1]: windowed, using size, center screen [mode2] fullscreen [mode3] fullscreen window [mode4] full Virtual Screen window, on ALL MONITORS
  bool createGLWindow(OSIWindow *w, OSIMonitor *m, string name, int dx, int dy, int8 bpp, int8 mode, short freq= 0);


  bool killPrimaryGLWindow();         /// calls restoreResolution, if in fullscreen
  void swapPrimaryBuffers();          /// OS dependant
// opengl funcs 
  bool killGLWindow(OSIWindow *w);    /// kills a specific opengl window
  void swapBuffers(OSIWindow *w);     /// swap buffers of specific window
  
  bool glMakeCurrent(OSIWindow *w);   /// OS independant variant. Pass null, to unmake current
  bool glCreateRenderer(OSIWindow *w);
  bool glDestroyRenderer(OSIWindow *w); /// OS independant variant

// time functions
  void getNanosecs(uint64 *out);      /// performance timer in nanoseconds
  void getMicrosecs(uint64 *out);     /// performance timer in microseconds
  void getMillisecs(uint64 *out);     /// performance timer in miliseconds

// system events
  bool checkMSG();                    /// checks for OS messages, should be INCLUDED in the MAIN LOOP
  uint64 eventTime;                   /// each event/msg timestamp/ used internally, to timestamp different messages
  /// windows processMSG() is outside class, their callback sucks, as allways
  #ifdef OS_LINUX
  void processMSG(); // linux MESSAGE HANDLER variant -don't call it, use OS independent checkMSG()
  #endif /// OS_LINUX
  
  
  string path;                  /// program path

// all program windows win[0] should be primary (primWin)
  OSIWindow win[MAX_WINDOWS];   /// all windows, including primary window (win[0])
  OSIWindow *primWin;           /// win[0]; primary window

  #ifdef OS_WIN
private:
  LARGE_INTEGER timerFreq;
public:
  string getWinName(HWND h);
  OSIWindow *getWin(HWND h);          /// returns the OSIWindow that has the specified HWND
  #endif /// OS_WIN

  #ifdef OS_LINUX
  OSIWindow *getWin(Window *w);       /// returns the OSIWindow that has the specified Window *
  //void setFullScreen(OSIWindow *w, bool fullScreen);  /// sets _NET_WM_STATE_FULLSCREEN attribute for selected window
  //void sendWMProp(int wmID, int wmProp, bool activate); /// documentation is @ end of OSinteraction.h
  #endif /// OS_LINUX

  #ifdef OS_MAC
  mach_timebase_info_data_t machInfo; /// mac variant of a performance timer. this var holds cpu frequencies & stuff (similar to QuerryPerformance... in win)
  OSIWindow *getWin(void *w);         /// returns the OSIWindow that has the specified NSWindow *
  #endif /// OS_MAC
  
  
  OSIDisplay display;           /// display class, handles monitors, resolutions
  
// main flags - check these frequently (mainly after a call to checkMSG() )
  struct OSIFlags {
    bool haveFocus;       // program has FOCUS or is in the BACKGROUND
    bool minimized;       // program is minimized
    bool exit;            // system wants program to CLOSE
    bool keyPress;        // a keyboard key is pressed - autoupdates @ checkMSG()
    bool buttonPress;     // a mouse button is pressed - autoupdates @ checkMSG()
  } flags;

    
// nothing to do with this class:
  bool resizeGLScene(GLsizei dx, GLsizei dy);   // this is NOT OS DEPENDANT<------------ maybe needs to be placed in another class or something    
};

#ifdef OS_WIN
LRESULT CALLBACK processMSG(HWND hWnd, UINT m, WPARAM wParam, LPARAM lParam); // wndproc - use checkMSG() to check (it will call this one)
BOOL CALLBACK monitorData(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);  // not used atm
#endif /// OS_WIN

#ifdef OS_MAC
extern "C" void processMSG(void);   /// declared in OScocoa.mm
#endif

extern OSInteraction osi;   // only 1 global class





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
 * void setWMprop(string8 wmID, string8 wmProp, uint val1, uint val2= 0); /// documentation is @ end of OSinteraction.h
 * 
 *  this func uses XChangeProperty(...) with atom1 as wmID
 *                                           atom2 as wmProp
 *                                           val1, val2 for data[0], data[1]
 * 
 */






