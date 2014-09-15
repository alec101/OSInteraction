#pragma once

/// the main defs that are used:
// OS_WIN
// OS_LINUX
// OS_MAC

// start with display.populate(&osi), to scan monitors/resolutions/sizes
// then a window can be created
// createGLWindow / primaryGlWindow/ etc
// Window   MODE parameter:
// [mode 1]: windowed, supplied using size, center screen
// [mode 2]: fullscreen, changes resolution of selected monitor with selected sizes
// [mode 3]: fullscreen window on selected monitor
// [mode 4]: full Virtual Screen window, on ALL MONITORS, ignores monitor/size vars passed (can be null i guess)

// USAGE WARNING
// CREATE win[0], wich is the primary window FIRST
// DELETE the primary window LAST <<< or better, don't destroy anything, as they are auto-destroyed on program exit


// graphics cards are ignored atm; no usefull data that i can think of can be gathered; (windows)
//   only the monitors that are attached to graphics cards are important - all data is gathered on them

///===================///
// COMPILING / LINKING //
///===================///

// put [..] [../source] [../../!utilClasses] in your include directories

// ---===NEEDED LIBRARIES ===---

// LINUX libraries: [GL] [GLU] [Xrandr] [Xinerama]   ([Xi] is scraped)
// WIN libs: [opengl32] [glu32] 
//           [winmm]:            crude windows joystick support
//           [dinput8] [dxguid]: if using direct input (+ #define USING_DIRECTINPUT)
//           [xinput]:           xinput 1.3, from dxsdk, maybe a path to it, too (+ #define USING_XINPUT)
// MAC frameworks: [-framework Opengl]: opengl library, basically
//                 [-framework cocoa]: macOSX api
//                 [-framework IOKit]: some monitor functions use this lib

// WIN: there are #pragma comments, for windows libs... but they work only for the visual c++ compiler

// MAC: just place "-framework Opengl -framework cocoa -framework IOKit" in additional linker commands


///===========///
// LINUX STUFF //
///===========///

// sudo apt-get install mesa-common-dev    GL/gl.h GL/glx.h
// sudo apt-get install libglu1-mesa-dev   GL/glu.h
// or the freeglu one
// sudo apt-get install libx11-dev         for X11/Xlib.h  < probably this is already installed
// sudo apt-get install libxrandr-dev      used for resolution changes
// sudo apt-get install libxi-dev          NOT USED ATM 
// sudo apt-get install libc6-dev-i386     the 32-bit c libraries (only 64bit libs are in linux64)
// sudo apt-get install xxxxxxxxxxxx       the 64-bit c libraries (only 32bit libs are in linux32)

// CASE SENSITIVE FILENAMES!!!! (linux)


 

#pragma once

// You can start the program with this macro; starting with main() in windows signals to create a console
#define osiMain \
#ifdef OS_WIN \
int WinMain(_In_  HINSTANCE hInstance, _In_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_  int nCmdShow) { \
#else \
int main(int argc, char *argv[], char *envp[]) { \
#endif 


#ifdef _WIN32
#define OS_WIN
#elif defined __linux__
#define OS_LINUX
#elif defined __APPLE__
#define OS_MAC
#endif

#ifdef OS_WIN
#define _CRT_SECURE_NO_WARNINGS
#define USING_DIRECTINPUT
#define USING_XINPUT

#define _WIN32_WINNT 0x05010000
#define WINVER 0x0501
#include <SDKDDKVer.h>

#include <Windows.h>
#include <process.h>
#include <mmsystem.h>
#ifdef USING_DIRECTINPUT
  #define DIRECTINPUT_VERSION 0x0800
  #include <dinput.h>
#endif
#ifdef USING_XINPUT
  #include <../../dxSDK2010j/Include/XInput.h>
#endif
#endif /// OS_WIN


#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// OpenGL headers

#ifdef OS_WIN
//#include <GL/gl.h>
#include "gl.h"
#include <GL/glu.h>
#define WGL_WGLEXT_PROTOTYPES 1
#include "wglext.h"
#endif /// OS_WIN

#ifdef OS_LINUX
#include <GL/gl.h>
#include <GL/glu.h>
#define GLX_GLXEXT_PROTOTYPES 1
#include "glxext.h"
#endif /// OS_LINUX

#ifdef OS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
// there's a corearb header, too NEEDS CHECKING
#endif /// OS_MAC
#define GL_GLEXT_PROTOTYPES
#include "glext.h"        // OpenGL extensions header file (OS independant ones)

#ifdef OS_LINUX
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
//#include <X11/extensions/XInput.h>        // too old stuff, maybe go thru it again, but...
//#include <X11/extensions/XInput2.h>       // too old stuff, maybe go thru it again, but...
//#include <locale.h> printf won't work without locale, to print unicode chars...

#include <GL/glx.h>
#endif /// OS_LINUX

#ifdef OS_MAC
#include <unistd.h>
#include <mach/mach.h>                  // high resolution clock funcs
#include <mach/mach_time.h>             // high resolution clock funcs
#include <CoreGraphics/CoreGraphics.h>  // trying to pinpoint quartz
//#include <IOKit/hid/IOHIDKeys.h>        // human interface devices (joysticks/gamepads/gamewheels)
#include <IOKit/hid/IOHIDLib.h>         // human interface devices (joysticks/gamepads/gamewheels)
#endif /// OS_MAC


#include "typeShortcuts.h"
#include "stringClass32.h"
#include "stringClass8.h"
#include "errorHandling.h"
#include "chainList.h"
#include "segList.h"

typedef string8 string;         // <<-- string set is here; can be utf-8 / utf-32

#include "osiDisplay.h"
#include "osiGlExt.h"
#include "osiRenderer.h"
#include "osiChar.h"
#include "osiInput.h"
#include "osiCocoa.h"











// 64 may be too much i think...
#define MAX_WINDOWS 64

class osiRenderer;

// -----------------============ WINDOW CLASS =============---------------------
class osiWindow {
  friend class osinteraction;
public:
  osiWindow();
  ~osiWindow();
  void delData();

  string name;              /// window name (program name)
  bool isCreated;           /// window has been created flag
  bool hasFocus;            /// has input focus (if no window has focus, program lost focus)
  short x0, y0, dx, dy;     /// window position & size
  int8 bpp;                 /// bits per pixel (if in fullscreen)
  short freq;               /// frequency (if used - fullscreen)
  int8 mode;                /// 1= windowed, 2= fullscreen, 3= fullscreen window, 4= full virtual desktop
  osiMonitor *monitor;      /// on what monitor it is drawn
  osiRenderer *glr;         /// the oGL renderer assigned to this window

  // internal data from here

  #ifdef OS_WIN
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
  
  bool isMapped;            // internal flag used when resolution is changing <<NOT USED ANYMORE I THINK>>

  /// specific linux window propreties functions
  void setWMprop(string8 wmID, string8 wmProp, uint val1, uint val2= 0); /// documentation is @ end of osinteraction.h
  void setWMstate(uint val, string8 prop1, string8 prop2= (cchar*)0); /// documentation is @ end of osinteraction.h
  #endif

  #ifdef OS_MAC
  void *win;                 /// MacWindow
  void *view;                /// MacGLview
  #endif
};





///=============================================================================///
// -----------=========== osiNTERACTION CLASS ================------------------ //
///=============================================================================///

class osinteraction {
public:
  string path;                        /// program path
  osiDisplay display;                 /// display class, handles monitors, resolutions
  osiWindow win[MAX_WINDOWS];         /// all windows; win[0] is the primary window, it should be created first
  osiWindow *primWin;                 /// win[0]; primary window
  uint64 present;   /// present time, updated in checkMSG(), wich should be the first func in a main loop. present MUST BE UPDATED MANUALLY, each frame, if checkMSG() is not called
  uint64 eventTime;                   /// each event/msg timestamp/ used internally, to timestamp different messages

  // main flags - check these frequently (mainly after a call to checkMSG() )
  struct osiFlags {
    bool haveFocus;                    // program has FOCUS or is in the BACKGROUND
    bool minimized;                    // program is minimized
    bool exit;                         // system wants program to CLOSE
    bool keyPress;                     // a keyboard key is pressed - autoupdates @ checkMSG()
    bool buttonPress;                  // a mouse button is pressed - autoupdates @ checkMSG()
  } flags;

  
  osinteraction();                    /// lots of inits go here. check cpp file
  ~osinteraction();
  void delData();                     /// called by destructor

  // SYSTEM EVENTS HANDLER: call this in MAIN PROGRAM LOOP
  
  bool checkMSG();                    /// checks for OS messages, should be INCLUDED in the MAIN LOOP

  void startThread(void (void *));    /// start / create a new thread
  
  // openGL window creation / deletion funcs:
  
  // createGLWindow is the main function to use
  // [mode1]: windowed, using size, center screen [mode2] fullscreen [mode3] fullscreen window [mode4] full Virtual Screen window, on all monitors
  bool createGLWindow(osiWindow *w, osiMonitor *m, string name, int dx, int dy, int8 bpp, int8 mode, short freq= 0, bool dblBuffer= true);
  bool killGLWindow(osiWindow *w);    /// destroys a specific opengl window
  
  // next funcs call createGLWindow / killGLWindow; they might make life easier, but nothing more
  
  ///frequency must be the same for all windows...
  bool primaryGLWindow(string name, int dx, int dy, int8 bpp, int8 mode, short freq= 0); // mode: 1= windowed, 2= fullscreen, 3= fullscreeen window(must research this one), 4= fullscreen virtual desktop (every monitor)
  bool primaryGLWindow();             /// creates a basic window, fullscreen
  bool killPrimaryGLWindow();         /// calls restoreResolution, if in fullscreen
  void setProgramIcon(string file);   /// sets program icon 

  // very useful functions that will work on all OSes
  
  void getNanosecs(uint64 *out);      /// performance timer in nanoseconds
  void getMicrosecs(uint64 *out);     /// performance timer in microseconds
  void getMillisecs(uint64 *out);     /// performance timer in miliseconds
  void getClocks(uint64 *out);        /// performance timer in raw clocks     N/A LINUX... trying to make it work
  void clocks2nanosecs(uint64 *out);  /// convert raw clocks to nanoseconds   N/A LINUX... trying to make it work
  void clocks2microsecs(uint64 *out); /// convert raw clocks to microseconds  N/A LINUX... trying to make it work
  void clocks2millisecs(uint64 *out); /// convert raw clocks to milliseconds  N/A LINUX... trying to make it work
  void exit(int retVal);              /// restores all monitor resolutions & exits. call this instead of _exit() or exit() func

  // opengl funcs

  void swapBuffers(osiWindow *w);     /// swap buffers of specific window
  void swapPrimaryBuffers();          /// calls swapBuffers, but for primary window (this makes life a little easier)
  bool glMakeCurrent(osiWindow *w);   /// OS independant variant. Pass null, to unmake current
  
  chainList glRenderers;
  osiRenderer *glr;                   /// this will allways point to the current active glRenderer;
  osiRenderer *assignRenderer(osiWindow *w); /// [internal]create or assign a renderer to selected window; returns pointer to the renderer or null if failed, somehow
  void delRenderer(osiRenderer *);    /// deletes the specified renderer and makes shure that windows and monitors that used it, know about it

  // internals from here on; nothing to bother
private:
  #ifdef OS_WIN

  LARGE_INTEGER timerFreq;

  string getWinName(HWND h);
  osiWindow *getWin(HWND h);          /// [internal] returns the osiWindow that has the specified HWND
  friend LRESULT CALLBACK processMSG(HWND, UINT, WPARAM, LPARAM);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  osiWindow *getWin(Window *w);       /// [internal] returns the osiWindow that has the specified Window *
  //void setFullScreen(osiWindow *w, bool fullScreen);  /// sets _NET_WM_STATE_FULLSCREEN attribute for selected window
  //void sendWMProp(int wmID, int wmProp, bool activate); /// documentation is @ end of osinteraction.h
  #endif /// OS_LINUX

  #ifdef OS_MAC
  mach_timebase_info_data_t machInfo; /// [internal] mac variant of a performance timer. this var holds cpu frequencies & stuff (similar to QuerryPerformance... in win)
public:
  osiWindow *getWin(void *w);         /// [internal] returns the osiWindow that has the specified NSWindow *
private:
  #endif /// OS_MAC

  /// windows processMSG() is outside class
  #ifdef OS_LINUX
  void processMSG(); // linux MESSAGE HANDLER variant -don't call it, use OS independent checkMSG()
  #endif /// OS_LINUX

  
  // nothing to do with this class:
public:
  bool resizeGLScene(GLsizei dx, GLsizei dy);   // this is NOT OS DEPENDANT<------------ maybe needs to be placed in another class or something    
};

#ifdef OS_WIN
LRESULT CALLBACK processMSG(HWND hWnd, UINT m, WPARAM wParam, LPARAM lParam); // wndproc - use checkMSG() to check (it will call this one)
BOOL CALLBACK monitorData(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);  // not used atm
#endif /// OS_WIN

#ifdef OS_MAC
extern "C" void processMSG(void);   /// declared in OScocoa.mm
#endif


extern osinteraction osi;   // only 1 global class
extern ErrorHandling error;










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






