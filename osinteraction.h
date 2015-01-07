#pragma once
// !!!
// any comment starting with '<<<' marks a setting that can / SHOULD be changed for your project
// keep glext.h wglext.h, glxext.h updated from https://www.opengl.org/registry/ , to be able to access the latest OpenGL extensions
// !!!


///===================///
// COMPILING / LINKING //
///===================///

// C++ standard must be set to C++11
// make shure osi header files are in [Additional include directories] (include search path)
// osi comes with some basic directx libs, but the path to them must be set somehow (in visual Studio it's "additional library directories")
// (Linux only) When using THREADS, '-pthreads' cmd option is a must for the linker/compiler (this is true for any program not only osi)

// ---===NEEDED LIBRARIES ===---

// LINUX libraries: [X11] [GL] [GLU] [Xrandr] [Xinerama]   ([Xi] is scraped)
// WIN libs: [opengl32] [glu32] 
//           if any dinput, xinput or direct3d are used, some directx sdk files (libs+includes) are provided, but directx sdk can be downloaded and used
//           [d3d9]:             [#define USING_DIRECT3D] must be set in osinteraction.h - used ONLY for GPU detection (hopefully oGL will have an extension for this, in the future)
//           [dinput8] [dxguid]: [#define USING_DIRECTINPUT] must be set in osinteration.h - used for direct input HIDs - joysticks gamepads etc
//           [xinput]:           [#define USING_XINPUT] must be set in osinteraction.h - used for xinput HIDs - probly only gamepads
//
//           the next libs should be auto-included, but here is the list in case something is missing:
//             kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
//
// MAC frameworks: [-framework Opengl]: opengl library, basically
//                 [-framework cocoa]: macOSX api
//                 [-framework IOKit]: some monitor functions use this lib

// WIN: there are #pragma comments, for windows libs... but they work only for the visual c++ compiler

// MAC: just place "-framework Opengl -framework cocoa -framework IOKit" in additional linker commands


///===========///
// LINUX STUFF //
///===========///

// Testing was done only in Ubuntu. (more linux systems must be tested!!!)

// sudo apt-get install mesa-common-dev    GL/gl.h GL/glx.h
// sudo apt-get install libglu1-mesa-dev   GL/glu.h
// or the freeglu one
// sudo apt-get install libx11-dev         for X11/Xlib.h  - probably this is already installed
// sudo apt-get install libxrandr-dev      used for resolution changes
// sudo apt-get install libxi-dev          NOT USED ATM (SCRAPED even)
// sudo apt-get install libc6-dev-i386     the 32-bit C libraries (only 64bit libs are in linux64)
// sudo apt-get install xxxxxxxxxxxx       the 64-bit C libraries (only 32bit libs are in linux32)
// sudo apt-get install libxinerama-dev    Xinerama header files, used to identify monitors



// linux uses case sensitive filenames, so 'Image.png' is different from 'image.png'



// SIMPLE USAGE info: //
///==================///
// start with display.populate(&osi), to scan monitors/resolutions/sizes
// then a window can be created
// createGLWindow / primaryGlWindow/ etc
// Window   MODE parameter:
// [mode 1]: windowed, supplied using size, center screen
// [mode 2]: fullscreen, changes resolution of selected monitor with selected sizes
// [mode 3]: fullscreen window on selected monitor
// [mode 4]: full Virtual Screen window, on ALL MONITORS, ignores monitor/size vars passed (can be null i guess)




#ifdef _WIN32
#define OS_WIN 1
#elif defined __linux__
#define OS_LINUX 1
#elif defined __APPLE__
#define OS_MAC 1
#endif


// WINDOWS SETTINGS / DIRECTORIES vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#ifdef OSI_RESOURCE_ONLY
#define IDI_LARGE 100
#define IDI_SMALL 101
#define OSI_ICON_LARGE "icon_64.ico"        // <<< name of the program icon here [WINDOWS ONLY]
#define OSI_ICON_SMALL "icon_64.ico"        // <<< name of the small program icon here [WINDOWS ONLY]

#else // if not windows resource include only

#ifdef OS_WIN

// the following can be turned off; just comment the lines; the whole DirectX part can be excluded from build;
#define USING_DIRECTINPUT                   // <<< [turn on / off] - dinput compatible HIDs
#define USING_XINPUT                        // <<< [turn on / off] - xinput compatible HIDs
#define USING_DIRECT3D                      // <<< [turn on / off] - used only for graphics card info (without this, you can't know what monitor is on what card)
// <<< download directx sdk: http://www.microsoft.com/en-us/download/confirmation.aspx?id=6812 >>>

// the following are directory locations; libraries MUST BE MANUALLY included, if not using visual studio (#pragma lib only works in vstudio)
#define XINPUTINCLUDE "directx/XInput.h"    // <<< xinput header file & directory location, if used - manually set this if using other
#define DINPUTINCLUDE "directx/dinput.h"    // <<< dinput header file & directory location, if used - manually set this if using other
#define D3DINCLUDE    "directx/d3d9.h"      // <<< direct3D9 header file & direcotry location, if used - manually set this if using other

#define XINPUTLIB32 "XInput_32.lib"         // <<< xinput 32bit library file & directory location, if used
#define XINPUTLIB64 "XInput_64.lib"         // <<< xinput 64bit library file & directory location, if used
#define DINPUTLIB32 "dinput8_32.lib"        // <<< dinput 32bit library file & directory location, if used
#define DINPUTLIB64 "dinput8_64.lib"        // <<< dinput 64bit library file & directory location, if used
#define DXGUIDLIB32 "dxguid_32.lib"         // <<< direct input guid 32bit library file & directory location, if dinput is used
#define DXGUIDLIB64 "dxguid_64.lib"         // <<< direct input guid 64bit library file & directory location, if dinput is used
#define D3D9LIB32   "d3d9_32.lib"           // <<< direct3D9 32bit library file & directory location, if it is used
#define D3D9LIB64   "d3d9_64.lib"           // <<< direct3D9 64bit library file & directory location, if it is used

// WINDOWS SETTINGS / DIRECTORIES ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x05010000
#define WINVER 0x0501
#include <SDKDDKVer.h>
#include <Windows.h>
#include <process.h>
#include <mmsystem.h>

#ifdef USING_DIRECTINPUT
  #define DIRECTINPUT_VERSION 0x0800
  #include DINPUTINCLUDE
  #pragma comment(lib, DINPUTLIB32)   /// if not using visual studio, this lib must be manually included, if using directinput
  #pragma comment(lib, DINPUTLIB64)   /// if not using visual studio, this lib must be manually included, if using directinput
  #pragma comment(lib, DXGUIDLIB32)   /// if not using visual studio, this lib must be manually included, if using directinput
  #pragma comment(lib, DXGUIDLIB64)   /// if not using visual studio, this lib must be manually included, if using directinput
#endif

#ifdef USING_XINPUT
  #include XINPUTINCLUDE
  #pragma comment(lib, XINPUTLIB64)   /// if not using visual studio, this lib must be manually included, if using xinput
  #pragma comment(lib, XINPUTLIB32)   /// if not using visual studio, this lib must be manually included, if using xinput
#endif

#ifdef USING_DIRECT3D
  #pragma comment(lib, D3D9LIB32)     /// if not using visual studio, this lib must be manually included, if using direct3d
  #pragma comment(lib, D3D9LIB64)     /// if not using visual studio, this lib must be manually included, if using direct3d
#endif /// USING_DIRECT3D

#endif /// OS_WIN



#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <math.h>


// OpenGL headers

#ifdef OS_WIN
//#include <GL/gl.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#pragma comment(lib, "opengl32")  /// if this pragma does not work, the library must be manually included
#pragma comment(lib, "glu32")     /// if this pragma does not work, the library must be manually included

//#define WGL_WGLEXT_PROTOTYPES 1
#include "openGL/wglext.h"        // <<< provided wglext.h - should be updated when a new version appears (and it can change monthly)
#endif /// OS_WIN

#ifdef OS_LINUX
#define GL_GLEXT_LEGACY 1
//#define GLX_GLXEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include "openGL/glxext.h"       // <<< provided glxext.h - should be updated when new a version appears (and it can change monthly)
#endif /// OS_LINUX

#ifdef OS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
// there's a corearb header, too NEEDS CHECKING
#endif /// OS_MAC

//#define GL_GLEXT_PROTOTYPES 1
#include "openGL/glext.h"        // <<< OpenGL extensions header file (OS independant ones) - should be updated when a new version appears (and it can change monthly)

// os specific

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
#include <pthread.h>
//#include <X11/extensions/XInput.h>        // too old stuff, maybe go thru it again, but...
//#include <X11/extensions/XInput2.h>       // too old stuff, maybe go thru it again, but...
//#include <locale.h> printf won't work without locale, to print unicode chars...
#endif /// OS_LINUX

#ifdef OS_MAC
#include <unistd.h>
#include <mach/mach.h>                  // high resolution clock funcs
#include <mach/mach_time.h>             // high resolution clock funcs
#include <CoreGraphics/CoreGraphics.h>  // trying to pinpoint quartz
//#include <IOKit/hid/IOHIDKeys.h>        // human interface devices (joysticks/gamepads/gamewheels)
#include <IOKit/hid/IOHIDLib.h>         // human interface devices (joysticks/gamepads/gamewheels)
#endif /// OS_MAC

// utility classes
#include "util/typeShortcuts.h"
#include "util/str32.h"
#include "util/str8.h"
#include "util/errorHandling.h"
#include "util/chainList.h"
#include "util/segList.h"
#include "util/filePNG.h"
#include "util/fileTGA.h"

//typedef Str8 string;         // <<< string set is here; can be utf-8 / utf-32 (as internal data)

// osi headers

#include "osiDisplay.h"
#include "osiGlExt.h"
#include "osiRenderer.h"
#include "osiChar.h"
#include "osiInput.h"
#include "osiCocoa.h"
#include "osiWindow.h"


// WIP vvv - works, but i don't like this
// You can start the program with this macro; starting with main() in windows signals to create a console
#define osiMain \
#ifdef OS_WIN \
int WinMain(_In_  HINSTANCE hInstance, _In_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_  int nCmdShow) { \
#else \
int main(int argc, char *argv[], char *envp[]) { \
#endif 
// WIP ^^^



class osiRenderer;




///================================================================================///
// -------------============ OSINTERACTION CLASS ================------------------ //
///================================================================================///

class osinteraction {
  Str8 _iconFile;
public:
  Str8 path;                          /// program path
  Str8 cmdLine;                       /// command line
  int argc;                           /// command line nr of arguments, same as the main(int argc..)
  char **argv;                        /// command line arguments list, same as the main(.. , char *argv[])

  osiDisplay display;                 /// display class, handles monitors, resolutions
  osiWindow win[MAX_WINDOWS];         /// all windows
  osiWindow *primWin;                 /// primary window - splash windows will not be marked as primary windows
  uint64 present;   /// present time, updated in checkMSG(), wich should be the first func in a main loop. present MUST BE UPDATED MANUALLY, each frame, if checkMSG() is not called
  uint64 eventTime;                   /// each event/msg timestamp/ used internally, to timestamp different messages

  // main flags - check these frequently (mainly after a call to checkMSG() )
  struct osiFlags {
    bool haveFocus;                    // program has FOCUS or is in the BACKGROUND
    bool minimized;                    // program is minimized
    bool exit;                         // system wants program to CLOSE
    bool keyPress;                     // a keyboard key is pressed - autoupdates @ checkMSG()
    bool buttonPress;                  // a mouse button is pressed - autoupdates @ checkMSG()
    bool HIDlostConnection;            // one of the HIDs (joysticks/gamepads/gamewheels) lost connection - a recheck of all active should be made / repopulate
    //bool sysHIDunplugged;              // system signaled that a HID was unplugged (there's no linux version, so a check to HIDlostConnection is better)
    bool windowResized;                // one of the windows was resized (usually need a gl aspect ratio recompute) - only for window mode 1
    //bool systemInSuspend;              // CAN'T FIND FOR LINUX the system entered a suspend mode - THIS IS VERY IMPORTANT TO CHECK nowadays - there HAS to be some kind of pause
  } flags;

  
  osinteraction();                    /// lots of inits go here. check cpp file
  ~osinteraction();
  void delData();                     /// called by destructor

  // SYSTEM EVENTS HANDLER: call this in MAIN PROGRAM LOOP
  
  bool checkMSG();                    /// checks for OS messages, should be INCLUDED in the MAIN LOOP. returns true if any msg was processed

  //void startThread(void (void *));   std::threads!!! /// start / create a new thread
  //void endThread(int status= 0);     std::threads!!! /// call it within the thread to end the thread
  
  // openGL window creation / deletion funcs:
  
  // createGLWindow is the main function to use
  // [mode1]: windowed, using size, center screen [mode2] fullscreen [mode3] fullscreen window [mode4] full Virtual Screen window, on all monitors
  bool createGLWindow(osiWindow *w, osiMonitor *m, cchar *name, int dx, int dy, int8 bpp, int8 mode, short freq= 0, bool dblBuffer= true, cchar *iconFile= null);
  bool killGLWindow(osiWindow *w);    /// destroys a specific opengl window
  
  // next funcs call createGLWindow / killGLWindow; they might make life easier, but nothing more
  
  ///frequency must be the same for all windows...
  bool primaryGLWindow(cchar *name, int dx, int dy, int8 bpp, int8 mode, int16 freq= 0); // mode: 1= windowed, 2= fullscreen, 3= fullscreeen window(must research this one), 4= fullscreen virtual desktop (every monitor)
  bool primaryGLWindow();             /// creates a basic window, fullscreen
  bool killPrimaryGLWindow();         /// calls restoreResolution, if in fullscreen
  void setProgramIcon(cchar *fileName);/// sets program icon - CALL BEFORE ANY WINDOW CREATION, or pass icon file to each window
  //void setWindowIcon(osiWindow *w, cchar *file);   /// sets a specific window icon 
  bool createSplashWindow(osiWindow *w, osiMonitor *m, cchar *file);
  
  

  // very useful functions that will work on all OSes
  
  void getNanosecs(uint64 *out);      /// performance timer in nanoseconds
  void getMicrosecs(uint64 *out);     /// performance timer in microseconds
  void getMillisecs(uint64 *out);     /// performance timer in miliseconds
  void sleep(uint64 millisecs);       /// sleeps for specified milliseconds
  void exit(int retVal= 0);           /// restores all monitor resolutions & exits. call this instead of _exit() or exit() func
  
  void getClocks(uint64 *out);        /// WIP performance timer in raw clocks     N/A LINUX... trying to make it work
  void clocks2nanosecs(uint64 *out);  /// WIP convert raw clocks to nanoseconds   N/A LINUX... trying to make it work
  void clocks2microsecs(uint64 *out); /// WIP convert raw clocks to microseconds  N/A LINUX... trying to make it work
  void clocks2millisecs(uint64 *out); /// WIP convert raw clocks to milliseconds  N/A LINUX... trying to make it work
  
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

  LARGE_INTEGER _timerFreq;

  char *_getWinName(HWND h);
  osiWindow *_getWin(HWND h);          /// [internal] returns the osiWindow that has the specified HWND
  friend LRESULT CALLBACK _processMSG(HWND, UINT, WPARAM, LPARAM);  /// windows processMSG() is outside class
  #endif /// OS_WIN

  #ifdef OS_LINUX
  osiWindow *_getWin(Window *w);      /// [internal] returns the osiWindow that has the specified X11 Window *
  //void setFullScreen(osiWindow *w, bool fullScreen);  /// sets _NET_WM_STATE_FULLSCREEN attribute for selected window
  //void sendWMProp(int wmID, int wmProp, bool activate); /// documentation is @ end of osinteraction.h
  bool _processMSG();                  // linux MESSAGE HANDLER variant -don't call it, use OS independent checkMSG()
  static Display *_dis;               /// display 'handle'. nowadays there is only 1 display, and 1 big (virtual) screen.

  #endif /// OS_LINUX

  #ifdef OS_MAC
  mach_timebase_info_data_t _machInfo; /// [internal] mac variant of a performance timer. this var holds cpu frequencies & stuff (similar to QuerryPerformance... in win)
public:
  osiWindow *_getWin(void *w);         /// [internal] returns the osiWindow that has the specified NSWindow *
private:
  #endif /// OS_MAC

  // friending funcs (happy happy joy joy)
  
  friend bool doChange(osiMonitor *, osiResolution *, int8, int16);
  friend void _populateGrCards(osiDisplay *);
  friend class osiDisplay;
  friend class osiMouse;
  friend class osiKeyboard;
  friend class osiJoystick;
  
  // nothing to do with this class:
public:
  bool resizeGLScene(GLsizei dx, GLsizei dy);   // this is NOT OS DEPENDANT<------------ maybe needs to be placed in another class or something    
};

#ifdef OS_WIN
LRESULT CALLBACK _processMSG(HWND hWnd, UINT m, WPARAM wParam, LPARAM lParam); // wndproc - use checkMSG() to check (it will call this one)
BOOL CALLBACK monitorData(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);  // not used atm
#endif /// OS_WIN

#ifdef OS_MAC
extern "C" void _processMSG(void);   /// declared in OScocoa.mm
#endif


extern osinteraction osi;   // only 1 global class
extern ErrorHandling error;


#endif /// OSI_RESOURCE_ONLY













