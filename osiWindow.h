#pragma once

/// BUG: [linux64] icon data is 64bit ints, it should be 32, keep an eye if this bug gets fixed (on X11's part)



// 64 may be too much i think...
#define MAX_WINDOWS 64


class osiRenderer;


///==========================================================================///
// -----------------============ WINDOW CLASS =============------------------ //
///==========================================================================///

class osiWindow {
  bool _isSplashWindow;
  friend class osinteraction;
public:
  osiWindow();
  ~osiWindow();
  void delData();

  Str8 name;                /// window name (program name)
  bool isCreated;           /// window has been created flag
  bool hasFocus;            /// has input focus (if no window has focus, program lost focus)
  int32 x0, y0, dx, dy;     /// window position & size
  int8 bpp;                 /// bits per pixel (if in fullscreen)
  int16 freq;               /// frequency (if used - fullscreen)
  int8 mode;                /// 1= windowed, 2= fullscreen, 3= fullscreen window, 4= full virtual desktop
  osiMonitor *monitor;      /// on what monitor it is drawn
  osiRenderer *glr;         /// the oGL renderer assigned to this window

  // funcs
  
  void setName(cchar *);
  bool setIcon(cchar *);        /// sets an icon from specified image file name (png/tga so far); in linux, icon must be set before the window is shown
  void show();
  void hide();
  void move(int x, int y);      /// in virtual desktop coordinates
  void resize(int dx, int dy);
  
  // internal data from here

  #ifdef OS_WIN
  HDC _hDC;                  /// private GDI device context
  HWND _hWnd;                /// holds program window handle
  HINSTANCE _hInstance;      /// holds the instance of the application ?? isn't this per window...
    
  static MSG _msg;           /// windows message struct... this is just needed for checkMSG();
  #endif ///OS_WIN

  #ifdef OS_LINUX
  Window _root;              /// root window (it is different on each monitor)
  static Display *_dis;      /// display 'handle'. nowadays there is only 1 display, and 1 big (virtual) screen.
  Window _win;               /// window 'handle' or watever
  XWindowAttributes _gwa;    /// window attributes (size/etc)
  XVisualInfo *_vi;
  GC _gc;                    /// background - gr context
  XImage *_img;              /// background - X11 image
  uint8 *_imgData;           /// background - image pixel data
  ulong *_iconData;          /// NEEDS TO BE ULONG (64bit for linux64) due to a bug - it might be fixed in the future
  
  bool _isMapped;            // internal flag used when resolution is changing <<NOT USED ANYMORE I THINK>>

  /// specific linux window propreties functions
  void _setWMprop(cchar *wmID, cchar *wmProp, uint32 val1, uint32 val2= 0); /// documentation is @ end of osinteraction.h
  void _setWMstate(uint32 val, cchar *prop1, cchar *prop2= (cchar*)0);      /// documentation is @ end of osinteraction.h
  #endif

  #ifdef OS_MAC
  void *_win;                 /// MacWindow
  void *_view;                /// MacGLview
  #endif
};