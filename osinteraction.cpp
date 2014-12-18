#include "osinteraction.h"
//bool chatty= false;  /// used only for DEBUG


// Create a 'probe' context on each monitor, see what it (OpenGL) returns (a product ID or something)
//^^^^^^^^^^^^^^^

/* TODO:
 *  *** PRIORITY 0 *** - INSTALL 2 IDENTICAL GRCARDS, TO SEE IF THE STRING RETURNED BY WINDOWS IS THE SAME.
 *                       MUST TEST ON LINUX TOO, SO MAYBE AFTER LINUX IS INSTALLED
 * - forcing close bool - if not set to false by exit or something similar, destructors should skip deleting stuff and calling funcs, as it will crash
 * - [win/mac] primary window can be any window, it is set when the first is made. (not the splash screen tho).
 *         osi.primWin is way more important. any ref to osi.win[0] as the primary window must be scraped
 * - [mac] better glMakeCurrent func, with the coreGl mac stuff << MAX PRIORITY
 * -xx [mac] threads !!!SCRAPE?! why: std::threads adopted in c++11, which is already needed xx
 * - [mac] set an icon for the window;  [win] WHEN dealing with icons, must remember to develop WM_GETICON too
 *          linux exe's don't have icons :( - http://linuxcritic.wordpress.com/2010/04/07/anatomy-of-a-desktop-file/ 
 * - [win] gamepad vibration under directinput (it is possible) & force feedback (MUST HAVE A VIBRATION GAMEPAD FIRST...)
 * - [linux][mac] create a loading window, in the center of the screen? eventually to have image of the game
 * - more todo's in osiInput.cpp
 * - [linux][mac] program command line (argv / getCommandLine) - NOT EASY, unfortunately: either call a func after main, or 
 *         getCmdLine / http://stackoverflow.com/questions/2003608/linux-equivalent-of-getcommandline-and-commandlinetoargv
 *         on second thought, you can read on linux the cmd line from that file, so it's not that hard
 * LOWER PRIORITY:
 * - [all] HIDs: a bool for (_bGrabbed) is done for sticks... (partially, not fully done), maybe this bGrabbed should be a big important flag
 *               that osiInput checks when in.update() is called, for any HID... and if the OS has funcs for the current mode, use them
 *               dunno what to say... or if the app has focus, just silently use the grab/ungrab mechanism...
 *               this 'grabbing' concept is kinda fuzzy for me, so more thinking on the matter must be done, and all HIDs should use only 1 system
 *               now, there's grabbing for some hids, it's a mess
 * - [all] mouse 'grabbing' is not that easy. using the system 'grab' funcs will not work on multiple windows
 *         therefore, the mouse cursor must be 'kept' inside the bounds of the created windows...
 *         if the windows have a monitor gap between them, the cursor must jump over the gap. VERY HARD <<<<<
 * - [linux] XLock/UnlockDisplay() - keep an eye on, somewhere some locks might be needed? dunno VERY LOW PRIO
 * - [linux][mac] prevent screensaver/ monitor low power
 * - [linux][mac] what happens on sleep? should be handled like 'app lose focus', or better, another flag, as the app must pause or something (some dont pause on app focus)
 * - [linux][mac] windowfocus flag. it's there, but not updated at all
 * - [linux] test mouse grab
 * - [linux] test keyboard grab (first make shure it is possible to exit program)
 *
 * - [win][linux][mac] more messing with pixel formats, after a good testing 'chamber' in openGL is created
 * 
 * - [win][linux][mac] joystick, wheel (BUY THEM first)
 *
 * - [linux] test monitors on duplicate (on mirror, on whatever crap the os calls them)
 *
 * - window to change a monitor without problems (unplug?)
 *    but on multiple monitor mode, mark it as closed? something like this... notify the program somehow, so a rearrangement will be done
 *
 * - [all]  Libcmt.lib try eliminating printf? alternative : console to write to file; problem: format!!!!
 *
 * - keyboard mode 2 in is getting more and more useless and a big drag... and is not avaible in linux + mac
 * - what happens when the time variables overflow? must do something about that (osi.getNano, etc)
 * - [mac] errorHandling window
*/


// LINUX WINDOW RESEARCH

// ONLY 1 DISPLAY MUST BE CREATED. it is 'the connection' to the 'server'(linux/computer, call it what u like)
// ^^ must be tested, there's a possibility that for each gr card there's another display
// there's the posibility that on different linux systems, they use multiple 'screen(s)' and on others xinerama (with 1 screen)

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// >>> WHAT HAPPENS WHEN MULTIPLE GRAPHICS CARDS ARE PRESENT? SOME SAY THERE ARE MULTIPLE DISPLAY CONNECTIONS AVAIBLE <<<
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

/* -Two commonly met examples are Maximized and Shaded. A window manager may implement these as proper substates
 *  of NormalState and IconicState, or it may treat them as independent flags, allowing e.g. a maximized window
 *  to be iconified and to re-appear as maximized upon de-iconification.
 *
 * -Root Window Properties (and Related Messages)
 *  Whenever this spec speaks about “sending a message to the root window”, it is understood that the client
 *  is supposed to create a ClientMessage event with the specified contents and send it by using a SendEvent
 *  request with the following arguments:
 *
 *   destination     root
 *   propagate       False
 *   event-mask      (SubstructureNotify|SubstructureRedirect)
 *   event           the specified ClientMessage
 *
 *   _NET_SUPPORTED
 *      _NET_SUPPORTED, ATOM[]/32
 *
 *   This property MUST be set by the Window Manager to indicate which hints it supports.
 *   For example: considering _NET_WM_STATE both this atom and all supported states
 *   e.g. _NET_WM_STATE_MODAL, _NET_WM_STATE_STICKY, would be listed. This assumes
 *   that backwards incompatible changes will not be made to the hints (without being renamed).
 *
 *
  _NET_ACTIVE_WINDOW
      _NET_ACTIVE_WINDOW, WINDOW/32

 The window ID of the currently active window or None if no window has the focus. This is a read-only property
   set by the Window Manager. If a Client wants to activate another window, it MUST send a _NET_ACTIVE_WINDOW
   client message to the root window:
^
_NET_ACTIVE_WINDOW
  window  = window to activate
  message_type = _NET_ACTIVE_WINDOW
  format = 32
  data.l[0] = source indication
  data.l[1] = timestamp
  data.l[2] = requestor's currently active window, 0 if none
  other data.l[] elements = 0

 Source indication should be 1 when the request comes from an application, and 2 when it comes from a pager.
   Clients using older version of this spec use 0 as source indication, see the section called “Source indication
   in requests” for details. The timestamp is Client's last user activity timestamp (see _NET_WM_USER_TIME) at the
   time of the request, and the currently active window is the Client's active toplevel window, if any
   (the Window Manager may be e.g. more likely to obey the request if it will mean transferring focus
   from one active window to another).

 Depending on the information provided with the message, the Window Manager may decide to refuse the request
   (either completely ignore it, or e.g. use _NET_WM_STATE_DEMANDS_ATTENTION).
 *
 _NET_CLOSE_WINDOW
    _NET_CLOSE_WINDOW

 Pagers wanting to close a window MUST send a _NET_CLOSE_WINDOW client message request to the root window:
_NET_CLOSE_WINDOW
  window = window to close
  message_type = _NET_CLOSE_WINDOW
  format = 32
  data.l[0] = timestamp
  data.l[1] = source indication
  other data.l[] elements = 0

 The Window Manager MUST then attempt to close the window specified. See the section called “Source
   indication in requests” for details on the source indication.

 Rationale: A Window Manager might be more clever than the usual method (send WM_DELETE message if the protocol
   is selected, XKillClient otherwise). It might introduce a timeout, for example. Instead of duplicating
   the code, the Window Manager can easily do the job.

 *
 */

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



#define IDI_LARGE 100
#define IDI_SMALL 101
#define OSI_ICON_LARGE "icon_64.ico"
#define OSI_ICON_SMALL "icon_64.ico"

//IDI_PROJECTNAME	ICON	"icon_64.ico"

//IDI_SMALL	ICON	"small.ico"




// >>> OBJECTS CREATION <<< //
///========================///
osinteraction osi;
osiInput in;
ErrorHandling error;




void _parseCmdLine(osinteraction *o);

osinteraction::osinteraction() {
          
  flags.exit= false;
  flags.haveFocus= false;
  flags.minimized= false;
  flags.buttonPress= false;
  flags.keyPress= false;
  flags.systemInSuspend= false;

  primWin= null;
  
  glr= null;

  #ifdef OS_WIN
  QueryPerformanceFrequency(&_timerFreq);   /// read cpu frequency. used for high performance timer (querryPerformanceBlaBla)

  cmdLine= GetCommandLine();                /// full command line - argc / *argv[] are created from this few lines down

  /// 'path' string - program path
  char *buf= new char[2048];
  GetCurrentDirectory(2047, buf);
  path= buf;
  delete[] buf;
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
/// printf won't work without setlocale. but hopefully it won't be needed.
//    setlocale(LC_ALL, ""); // can't rely on setlocale. everything is different on each os. rely on StringClass32/8 and that's that.
//  setlocale(LC_CTYPE, "");
  
  if(!(_dis= XOpenDisplay(null)))          // DISPLAY CONNECTION TO XSERVER can be something like ":0.0" :displayNr.screenNr
    error.simple("Cannot connect to X server\n", true); /// true= exit
  
  /// 'path' string - program path
  char *buf= new char[512];
  getcwd(buf, 511);
  path= buf;
  delete[] buf;
  
  
  
  #endif /// OS_LINUX

  #ifdef OS_MAC
  mach_timebase_info(&machInfo);            /// read cpu frequency & stuff; used for high performance timer (mac variant, named mach)
  cocoa.setProgramPath();                   /// program path (osi.path stores the string afterwards)
  #endif /// OS_MAC


  _parseCmdLine(this);      /// creates argc & argv
  getNanosecs(&present);                    /// start with updated present time variable
}


osinteraction::~osinteraction() {
  if(argv) {
    for(int a= 0; a< argc; a++)
      if(argv[a]) delete[] argv[a];
    delete[] argv;
  }

  #ifdef OS_LINUX
// it seems system already destroys the display/windows and calling any of these causes segmentation error
// maybe put them in a program exit function that might be called by the program.
//
  // XSync(primWin->dis, False);
  // delData();
  // XCloseDisplay(primWin->dis);             // DISPLAY CONNECTION TERMINATION
  #endif
}


void osinteraction::delData() {
  /// destroy every window. kinda useless...
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(win[a].isCreated) win[a].delData();
}




void _parseCmdLine(osinteraction *o) {
  string32 s, s2;
  if(!o->cmdLine.d) return;
  s= o->cmdLine.convert32();
  string8 utf8conv;

  /// find argc
  o->argc= 1;    /// allways at least 1
  
  bool apostrophe= false, quote= false;
  for(int a= 0; a< o->cmdLine.len; a++) {
    ulong c= s.d[a];                /// c will hold the current character

    /// quotes - everything under will be 1 cmd argument
    if(c== '\'') {
      if(apostrophe) apostrophe= false;
      else           apostrophe= true;

    /// doublequotes - everything under will be 1 cmd argument
    } else if(c== '\"') {
      if(quote) quote= false;
      else      quote= true;

    /// if this is a whitespace, there is potential for another command, if not under quotations
    } else if(c== ' ' || c== 0 || c== '\n' || c== '\r' || c== '\t' || c== '\v' || c== '\b') {

      /// under quote/dblquote, this whitespace is ignored, until next quote/dblquote is reached
      if(apostrophe || quote) continue;

      /// if next character is still a whitespace, ignore and continue
      ulong n= s.d[a+ 1];
      if(n== ' ' || n== 0 || n== '\n' || n== '\r' || n== '\t' || n== '\v' || n== '\b') continue;

      /// reached this point= this is a cmd argument, increase argc
      o->argc++;
      if(c== 0) break;              /// safety feature, but it shouldn't happen
    }
  }

  /// populate argv
  o->argv= new char *[o->argc];

  int mark= 0;                  /// marks the beginning of the argument command (character number)
  int n= 0;                     /// current argument number (max = argc)
  apostrophe= quote= false;     /// if under a quote or doublequote, whitespaces are ignored

  for(int a= 0; a< s.len+ 1; a++) {
    ulong c= s.d[a];
    
    /// quotes - everything under will be 1 cmd argument
    if(c== '\'') {
      if(apostrophe) apostrophe= false;
      else           apostrophe= true;

    /// doublequotes - everything under will be 1 cmd argument
    } else if(c== '\"') {
      if(quote) quote= false;
      else      quote= true;
      
    /// if this is a whitespace, possible that a cmd argument end is reached, if not under quotations
    } else if(c== ' ' || c== 0 || c== '\n' || c== '\r' || c== '\t' || c== '\v' || c== '\b') {

      /// if mark is current char (a), and this is a whitespace, this whitespace is ignored, mark is increased
      if(mark== a) {
        mark++;
        continue;
      }

      /// under quote/dblquote, this whitespace is ignored, until next quote/dblquote is reached
      if(apostrophe || quote)         /// this whitespace is ignored if under a quote or double quote
        continue;

      // cmd argument fully found at this point, copy to argv[n]
      int len= a- mark;               /// length of the this cmd argument
      
      /// s2 will hold the cmd argument in utf32 format (easyer than utf8 to know how much mem to alloc / copy characters)
      s2.delData();
      s2.d= new ulong[len+ 1];
      s2.d[len]= 0;                   /// str terminator

      for(int d= 0; d< len; d++)
        s2.d[d]= s.d[d+ mark];

      /// convert from s2(utf-32) to argv[n] (utf-8)
      utf8conv= s2;
      o->argv[n]= new char[utf8conv.len+ 1];
      for(int d= 0; d< utf8conv.len+ 1; d++)
        (o->argv[n])[d]= utf8conv.d[d];

      n++;                            /// n is argv array index
      mark= a+ 1;                     /// mark the beggining of the next arg command (if any)

      if(c== 0) break;                /// reached end of string - parsing is done
    }
  } /// pass thru all characters in cmdLine
}





// -------------============== WINDOW CREATION ================----------------

// SIMPLE WINDOW CREATION FUNCS. they all call createGLWindow()

// create just a single 'primary' window on 'primary' monitor
bool osinteraction::primaryGLWindow(string name, int dx, int dy, int8 bpp, int8 mode, short freq) {
  if(primWin) return false;               /// primary window already created
  int a;
  for(a= 0; a< MAX_WINDOWS; a++)
    if(!win[a].isCreated)
      break;
  if(a== MAX_WINDOWS) return false;       /// weird case, but it can happen
  
  return createGLWindow(&win[a], display.primary, name, dx, dy, bpp, mode, freq);
}

// create a fullscreen (mode 3) primary window
bool osinteraction::primaryGLWindow() {
  if(primWin) return false;
  int a;
  for(a= 0; a< MAX_WINDOWS; a++) 
    if(!win[a].isCreated)
      break;
  if(a== MAX_WINDOWS) return false;       /// weird case, but it can happen
  
  win[a].mode= 3;
  win[a].name= "Primary Program Window";
  win[a].freq= 0;
  win[a].bpp= 32;
  
  return createGLWindow(&win[a], display.primary, win[a].name, win[a].dx, win[a].dy, win[a].bpp, win[a].mode, win[a].freq);
}


// MAIN CREATE WINDOW FUNC. has every customisation
bool osinteraction::createGLWindow(osiWindow *w, osiMonitor *m, string name, int dx, int dy, int8 bpp, int8 mode, short freq, bool dblBuffer) {
  bool chatty= false;

  string func= "osinteraction::createGLWindow: ";
  w->name= name;
  w->monitor= m;

  /// window position
  if(mode== 1) {                                    /// windowed position
    int tx= m->x0+ (m->original.dx/ 2)- (dx/ 2);
    int ty= m->y0+ (m->original.dy/ 2)- (dy/ 2);
    w->x0= (tx< m->x0)? m->x0: tx;
    w->y0= (ty< m->y0)? m->y0: ty;
  } else if((mode == 2) || (mode == 3)) {           /// fullscreen/ fullscreen window position
    w->x0= m->x0;
    w->y0= m->y0;
  } else if (mode== 4) {                            /// full virtual screen
    w->x0= display.vx0;
    w->y0= display.vy0;
  }
  /// fullscreen window size set to monitor resolution size - else use them passed vars
  if(mode == 3) {                                   /// fullscreen window
    w->dx= m->original.dx;
    w->dy= m->original.dy;
  } else if(mode == 4) {                            /// full virtual screen
    w->dx= display.vdx;
    w->dy= display.vdy;
  } else {
    w->dx= (dx> 1)? dx: 1;
    w->dy= (dy> 1)? dy: 1;
  }
  w->bpp= bpp;
  w->mode= mode;
  w->freq= freq;
  if(!primWin) primWin= w;                          /// if no primary window is set, this will be the primary
  
  #ifdef OS_WIN

  GLuint PixelFormat;   // Holds The Results After Searching For A Match
  WNDCLASS wc;          // Windows Class Structure
  DWORD dwExStyle;      // Window Extended Style
  DWORD dwStyle;        // Window Style
  RECT rect;            // Grabs Rectangle Upper Left / Lower Right Values

  /// fullscreen resolution change
  if(mode== 2) {
    if(!display.changeRes(m, w->dx, w->dy, w->bpp, w->freq)) {
      mode= 1;                            // if it fails, set mode to windowed <<--- ???
      w->mode= 1;
    }
    w->x0= m->x0;
    w->y0= m->y0;
  }

  rect.left=   w->x0;
  rect.right=  w->x0+ w->dx;
  rect.top=    display.vdy- display.vy0- (w->y0+ w->dy);  /// coordonate unification changed
  rect.bottom= display.vdy- display.vy0- w->y0;           /// coordonate unification changed

  w->_hInstance =   GetModuleHandle(NULL);                /// grab an instance for window
  wc.style=         CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   /// Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc=   (WNDPROC) processMSG;                 // processMSG handles messages
  wc.cbClsExtra=    0;                                    /// no extra
  wc.cbWndExtra=    0;                                    /// no extra
  wc.hInstance=     w->_hInstance;                        /// set the aquired instance
  wc.hIcon=         LoadIcon(NULL, IDI_WINLOGO);          // load default icon <<<<<<<<<<<<<<<<<<<<<< ICON WORKS MUST BE MADE
  wc.hCursor=       LoadCursor(NULL, IDC_ARROW);          /// load arrow pointer
  wc.hbrBackground= NULL;                                 /// no backgraound required when using opengl
  wc.lpszMenuName=  NULL;                                 /// no menus
  wc.lpszClassName= name;                                 /// class name... dunno for shure what this is

  if (!RegisterClass(&wc)) {                        /// register the window class
    error.simple(func+ "Failed to register wc");
    return false;
  }

  if(mode== 1) {                                    // windowed
    if(w == primWin) {                             /// if it's primWin, it's the primary window - if not it's a child window
      dwExStyle= WS_EX_APPWINDOW| WS_EX_WINDOWEDGE;
      dwStyle= WS_OVERLAPPEDWINDOW;
    } else {
      dwExStyle= WS_EX_TOOLWINDOW| WS_EX_WINDOWEDGE;
      dwStyle= WS_OVERLAPPEDWINDOW;                 /// WS_CHILD is out of the equation (these are windows in windows)
    }
  } else if(mode== 2) {                             // fullscreen
    if(w == primWin) {
      dwExStyle= WS_EX_APPWINDOW;
      dwStyle= WS_POPUP;
      ShowCursor(FALSE);
    } else {
      dwExStyle= 0;
      dwStyle= WS_POPUP;//WS_CHILD;//WS_OVERLAPPED;// WS_POPUP;//| WS_CHILD;  // <<< wschild???
      ShowCursor(FALSE);
    }
  } else if((mode== 3) || (mode== 4)) {             // fullscreen window
    if(w == primWin) {
      dwExStyle= WS_EX_APPWINDOW;
      dwStyle= WS_POPUP;
    }	else {
      dwExStyle= 0;
      dwStyle= WS_POPUP;//| WS_CHILD; // <<< wschild???
    }
  }

  AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

  // Create The Window
  if (!(w->_hWnd= CreateWindowEx(
                dwExStyle,              // Extended Style For The Window
                name,                   // class name           <--- might want a different class name?
                name,                   /// window title
                dwStyle |               /// defined window style
                WS_CLIPSIBLINGS |       /// Required Window Style ?? not shure
                WS_CLIPCHILDREN,        /// Required Window Style ?? not shure
                w->x0, (display.vdy- display.vy0)- (w->y0+ w->dy), /// window position (coord unification fixed)
                rect.right- rect.left,  /// dx
                rect.bottom- rect.top,  /// dy
                primWin->_hWnd,         /// parent window
                NULL,                   /// no menu
                w->_hInstance,          /// instance
                NULL)))                 /// don't pass anything to WM_CREATE
  {
    killGLWindow(w);                    // Reset The Display
    error.simple(func+ "Window creation error.");
    return false;
  }
  /// pixel format - MORE TESTING NEEDED HERE. screen blacks out on mode 3 - it shouldn't
  static PIXELFORMATDESCRIPTOR pfd;
  /*
  pfd.nSize= sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion= 1;
  pfd.dwFlags= PFD_DRAW_TO_WINDOW| PFD_DRAW_TO_BITMAP| PFD_SUPPORT_OPENGL| PFD_DOUBLEBUFFER| PFD_STEREO_DONTCARE; //| PFD_SWAP_EXCHANGE;//| PFD_NEED_PALETTE;
  pfd.iPixelType= PFD_TYPE_RGBA;
  pfd.cColorBits= w->bpp;
  pfd.cRedBits= 8;
  pfd.cRedShift= 0;
  pfd.cGreenBits= 8;
  pfd.cGreenShift= 0;
  pfd.cBlueBits= 8;
  pfd.cBlueShift= 0;
  pfd.cAlphaBits= 0;
  pfd.cAlphaShift= 0;
  pfd.cAccumBits= 32;
  pfd.cAccumRedBits= 8;
  pfd.cAccumGreenBits= 8;
  pfd.cAccumBlueBits= 8;
  pfd.cAccumAlphaBits= 8;
  pfd.cDepthBits= 32;
  pfd.cStencilBits= 8;
  pfd.cAuxBuffers= 0;
  pfd.iLayerType= PFD_MAIN_PLANE;
  pfd.bReserved= 0;
  pfd.dwLayerMask= 0;
  pfd.dwVisibleMask= 0;
  pfd.dwDamageMask= 0;
  */
  if (!(w->_hDC= GetDC(w->_hWnd))) {                /// get a device context
    killGLWindow(w);
    error.simple(func+ "Can't create a GL DC");
    return false;
  }
  /// get the current pixel format index  
  int pixelf= GetPixelFormat(w->_hDC); 
  if(chatty) printf("pixelf= %d\n", pixelf);
  /// obtain a detailed description of that pixel format  
  DescribePixelFormat(w->_hDC, pixelf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  // double buffer is needed, but causes screen flicker. must check with the other type of wgl_choosePixelFormat()
  pfd.dwFlags= pfd.dwFlags| PFD_DRAW_TO_WINDOW| PFD_DRAW_TO_BITMAP| PFD_SUPPORT_OPENGL| PFD_STEREO_DONTCARE; //| PFD_SWAP_EXCHANGE;//| PFD_NEED_PALETTE;
  if(dblBuffer)
    pfd.dwFlags|= PFD_DOUBLEBUFFER;

  if(chatty) {
    printf("Current pixel format descriptor:\n");
    printf("pfd.nVersion= %d\n", pfd.nVersion);
    //pfd.dwFlags= PFD_DRAW_TO_WINDOW| PFD_DRAW_TO_BITMAP| PFD_SUPPORT_OPENGL| PFD_DOUBLEBUFFER| PFD_STEREO_DONTCARE; //| PFD_SWAP_EXCHANGE;//| PFD_NEED_PALETTE;
    printf("pfd.iPixelType= %d\n", pfd.iPixelType);
    printf("pfd.cColorBits= %d\n", pfd.cColorBits);
    printf("pfd.cRedBits= %d\n", pfd.cRedBits);
    printf("pfd.cRedShift= %d\n", pfd.cRedShift);
    printf("pfd.cGreenBits= %d\n", pfd.cGreenBits);
    printf("pfd.cGreenShift= %d\n", pfd.cGreenShift);
    printf("pfd.cBlueBits= %d\n", pfd.cBlueBits);
    printf("pfd.cBlueShift= %d\n", pfd.cBlueShift);
    printf("pfd.cAlphaBits= %d\n", pfd.cAlphaBits);
    printf("pfd.cAlphaShift= %d\n", pfd.cAlphaShift);
    printf("pfd.cAccumBits= %d\n", pfd.cAccumBits);
    printf("pfd.cAccumRedBits= %d\n", pfd.cAccumRedBits);
    printf("pfd.cAccumGreenBits= %d\n", pfd.cAccumGreenBits);
    printf("pfd.cAccumBlueBits= %d\n", pfd.cAccumBlueBits);
    printf("pfd.cAccumAlphaBits= %d\n", pfd.cAccumAlphaBits);
    printf("pfd.cDepthBits= %d\n", pfd.cDepthBits);
    printf("pfd.cStencilBits= %d\n", pfd.cStencilBits);
    printf("pfd.cAuxBuffers= %d\n", pfd.cAuxBuffers);
    printf("pfd.iLayerType= %d\n", pfd.iLayerType);
    printf("pfd.bReserved= %d\n", pfd.bReserved);
    printf("pfd.dwLayerMask= %d\n", pfd.dwLayerMask);
    printf("pfd.dwVisibleMask= %d\n", pfd.dwVisibleMask);
    printf("pfd.dwDamageMask= %d\n", pfd.dwDamageMask);
  }


  // MORE TESTS NEEDED. it seems, when everything is 0, some 'default' current mode is in use; can't know for shure until using a more complex opengl scene


   // NEED TO USE THIS INSTEAD, i think
  /*
  BOOL wglChoosePixelFormatARB(   HDC hdc,
                                const int *piAttribIList,
                                const FLOAT *pfAttribFList,
                                UINT nMaxFormats,
                                int *piFormats,
                                UINT *nNumFormats);
  */


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


  ShowWindow(w->_hWnd, SW_SHOW);  /// Show The Window
  SetForegroundWindow(w->_hWnd);  /// Slightly Higher Priority

  SetFocus(w->_hWnd);             /// Sets Keyboard Focus To The Window

  //Sleep(500); //<><<<<<<<<<<<<<<<<<<<<>><><><

  if(!assignRenderer(w)) {
    osi.killGLWindow(w);
    error.simple("FATAL ERROR: Cannot create oGL renderer (context)");
    return false;
  }
  
  // from help: wglMakeCurrent()
  //	All subsequent OpenGL calls made by the thread are drawn on the device identified by hdc.
  //	You can also use wglMakeCurrent to change the calling thread's current rendering context so it's no longer current.


  if(!glMakeCurrent(w)) {
    killGLWindow(w);
    error.simple(func+ "Can't activate GL RC");
    return false;
  }



  w->isCreated= true;
  m->win= w;

  w->glr->checkExt();
  w->glr->getExtFuncs();          /// once a window is created, getExtensions() aquires oGL extensions functions

  return true;
  #endif /// OS_WIN

  
  #ifdef OS_LINUX //                <---------------------------LINUX

  Colormap cmap;
  XSetWindowAttributes swa;

  GLint att[]= { GLX_RGBA,
                 GLX_RED_SIZE, 8,
                 GLX_GREEN_SIZE, 8,
                 GLX_BLUE_SIZE, 8,
                 GLX_ALPHA_SIZE, 8,
                 GLX_DEPTH_SIZE, 16,
                 (dblBuffer? GLX_DOUBLEBUFFER: 0),
                 None };
  
  w->_root= m->_root;                                        // 'desktop window'
  w->_dis= _dis;                                           // XServer connection
  
  if(mode == 2)
    if(!display.changeRes(m, dx, dy, bpp, freq)) {
      error.simple("osi:createGLwindow: cant change to selected resolution");
      w->dx= m->dx;
      w->dy= m->dy;
      mode= 3;
    }

  w->mode= mode;

  w->_vi= glXChooseVisual(w->_dis, m->_screen, att);

  /* !!!!!!!!!!!!!!!!!!
  vi= DefaultVisual(display, 0);
  depth= DefaultDepth(display, 0);
  */


  if(w->_vi == NULL)
    error.simple(func+ "no appropriate visual found\n", true);
  else // DELETE <--------------------------------
    if(chatty) printf("\n\tvisual %p selected\n", (void *)w->_vi->visualid); // %p creates hexadecimal output like in glxinfo

  cmap= XCreateColormap(w->_dis, w->_root, w->_vi->visual, AllocNone);
  swa.colormap= cmap;


  // try only the parent to receive events!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //if(w== primWin)
    swa.event_mask= ExposureMask|

//                    ((w== primWin)? StructureNotifyMask: SubstructureNotifyMask)|
//                    SubstructureNotifyMask|   // send msgs only to parent
                    StructureNotifyMask|      // probly the default, a parent must have this I THINK

                    EnterWindowMask| LeaveWindowMask|     // mouse enters window &other stuff like this
                    FocusChangeMask|
                    KeyPressMask| KeyReleaseMask| ButtonPressMask| ButtonReleaseMask|
                    PointerMotionMask;
  //else
    //swa.event_mask= ExposureMask;

  swa.override_redirect= 0;                             // this is very hard to handle if true

  w->_win= XCreateWindow(w->_dis, w->_root,
                        w->x0, display.vdy- display.vy0- (w->y0+ w->dy), w->dx, w->dy,     // position & size (coord unification fixed)
                        0,                              // border size
                        w->_vi->depth,                  // depth can be CopyFromParent
                        InputOutput,                    // InputOnly/ InputOutput/ CopyFromParent
                        w->_vi->visual,                 // can be CopyFromParent
                        CWColormap| CWEventMask| CWOverrideRedirect,       // tied with &swa
                        &swa);                          //

  


  // XInput EXPERIMANTAL
  /*
  XIEventMask eventmask;
  unsigned char mask[1] = { 0 }; // the actual mask

  eventmask.deviceid = 2;
  eventmask.mask_len = sizeof(mask); // always in bytes
  eventmask.mask = mask;

  // now set the mask
  XISetMask(mask, XI_ButtonPress);
  XISetMask(mask, XI_Motion);
  //XISetMask(mask, XI_KeyPress);
  //XISetMask(mask, XI_);

  // select on the window
  XISelectEvents(w->dis, w->win, &eventmask, 1);
  */
// XInput EXPERIMANTAL ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



// tring to see what this group hint is

  XWMHints *wh= XAllocWMHints();
  wh->window_group= 1;
  wh->flags= WindowGroupHint;
  XSetWMHints(w->_dis, w->_win, wh);
  XFree(wh);


// ^^^^^^^^ after testing, still dunno what the group does... auto it does nothing

  // _NET_WM_STATE with_NET_WM_STATE_ABOVE gain focus
  // _NET_WM_STATE with_NET_WM_STATE_BELOW lose focus

  if(w== primWin) {
    // ICON

  } else {
    /// rest of windows have no taskbar icons
    //XSetTransientForHint(primWin->dis, w->win, primWin->win); // it seems transientFor is the only thing needed (no taskbar icon)
    w->_setWMstate(1, "_NET_WM_STATE_SKIP_TASKBAR");

    // ***NO ICON

    // ***NO TASKBAR
    // option 1:  _NET_WM_WINDOW_TYPE
    //   with  _NET_WM_WINDOW_TYPE_TOOLBAR or _NET_WM_WINDOW_TYPE_UTILITY or _NET_WM_WINDOW_TYPE_DIALOG
    // option 2:  _NET_WM_STATE
    //   with  _NET_WM_STATE_SKIP_TASKBAR

  }
  
  /// fullscreen mode linux specific msgs settings
  //XMoveWindow(w->dis, w->win, w->x0, w->y0);
  if((mode== 2) || (mode== 3) || (mode== 4))
    w->_setWMstate(1, "_NET_WM_STATE_FULLSCREEN");
  
  /// Fullscreen window on all monitors - MODE 4. Needs XInerama only for monitor IDs (wich sucks, as Xrandr should handle this little aspect)
  if(mode== 4) {
    Atom fullmons = XInternAtom(w->_dis, "_NET_WM_FULLSCREEN_MONITORS", False);
    XEvent xev;
    for(short a= 0; a< sizeof(xev); a++) ((char*)&xev)[a]= 0;
    xev.type = ClientMessage;
    xev.xclient.window= w->_win;
    xev.xclient.message_type = fullmons;
    xev.xclient.format = 32;
    
    /// the next values are found in OSdisplay.cpp, updateVirtualDesktop() (back of file)
    /// Xinerama IDs are found in display.populate(), linux part, at the end
    xev.xclient.data.l[0]= display._top;     /// topmost monitor number (Xinerama monitor ID)
    xev.xclient.data.l[1]= display._bottom;  /// bottommost
    xev.xclient.data.l[2]= display._left;    /// leftmost
    xev.xclient.data.l[3]= display._right;   /// rightmost
    printf("top[%d] left[%d] bottom[%d] right[%d]\n", display._top, display._left, display._bottom, display._right);
    xev.xclient.data.l[4]= 1;               /// source indication (1 for normal applications, 2 for pagers and other Clients that represent direct user actions)
    
    XSendEvent (w->_dis, DefaultRootWindow(w->_dis), False,
                    SubstructureRedirectMask | SubstructureNotifyMask, &xev);
  }
  
  if(w->mode!= 1)                           /// in all other modes but 1, make window 'on top'
    w->_setWMstate(1, "_NET_WM_STATE_ABOVE");
  
  XMapWindow(w->_dis, w->_win);               /// map window= finish creation/ show window
  
  if(w->mode== 1)                           /// MODE1 seems to need to move the window, x0&y0 in createwindow do nothing
    XMoveWindow(w->_dis, w->_win, w->x0, display.vdy- display.vy0- (w->y0+ w->dy));

  XStoreName(w->_dis, w->_win, name);         /// window name (top bar description/name)

  /// create / assign existing renderer 
  if(!assignRenderer(w)) {
    osi.killGLWindow(w);
    error.simple("FATAL ERROR: Cannot create oGL renderer (context)");
    return false;
  }
  
  glMakeCurrent(w);                         // osi variant. works on every OS

  ///  handle the close button WM
  if(w== primWin) {
    Atom wmDelete= XInternAtom(w->_dis, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(w->_dis, w->_win, &wmDelete, 1);
  }

  w->monitor= m;
  w->isCreated= true;
  m->win= w;
  
  w->glr->checkExt();           /// checks for extension avaibility on this oGL renderer
  w->glr->getExtFuncs();        /// once a window is created, getExtensions() aquires oGL extensions functions

  return true;

  #endif /// OS_LINUX

  #ifdef OS_MAC // <<<<<<<<<<<<<< MAC >>>>>>>>>>>>>>>
  
  /// window creation is in OScocoa.mm due to Abjective-C
  
  return cocoa.createWindow(w);  /// all window vars are set, just create the window.
  #endif /// OS_MAC
  
  /// if program reached this point, there's no OS defined
  error.simple(func+ "no OS specified?");
  return false;
} // osinteraction::createGLWindow END <<<





// ----------------============= GLWINDOW DELETION ==============-------------
bool osinteraction::killPrimaryGLWindow() {
  if(!primWin) return false;
  return killGLWindow(primWin);
}


bool osinteraction::killGLWindow(osiWindow *w) {
  bool newPrimWin= (w== primWin? true: false);
  
  if(w->mode== 2)
    display.restoreRes(w->monitor);

  if(w->monitor)
    w->monitor->win= null;

  w->delData();
  
  /// a new primary window is needed (primWin)
  if(newPrimWin)
    for(short a= 0; a< MAX_WINDOWS; a++)
      if(win[a].isCreated) {
        primWin= &win[a];
        break;
      }
      
  return true;
} /// osinteraction::killGLWindow




bool osinteraction::createSplashWindow(osiWindow *w, osiMonitor *m, cchar *file) {
  if((!w) || (!m) || (!file)) return false;

  // image loading
  /// s will hold the file extension to be loaded, in lowercase
  string8 s("   ");
  size_t len= string8::strlen(file);
  
  for(char a= 0; a< 3; a++)
    s.d[a]= file[len- 3+ a];

  s.lower();

  PNG png;
  TGA tga;

  uchar *bitmap= null;            /// bitmap will hold the image raw data (either kind of filetype, will be the same)
  ulong dx, dy;                   /// loaded image size
  uchar depth;                    /// byte depth - either 3 or 4 bytes (with or without alpha)
  uchar bpc;
  uchar bpp;

  /// check the file extension
  if(s== "png") {
    png.load(file);
    if(png.type!= IMG_RGB && png.type!= IMG_RGBA)
      return false;

    depth= png.bpp/ png.bpc;
    dx= png.dx;
    dy= png.dy;
    bitmap= (uchar *)png.bitmap;
    bpc= png.bpc;
    bpp= png.bpp;
  } else if(s== "tga") {
    tga.load(file);
    if(tga.type!= IMG_RGB && tga.type!= IMG_RGBA)
      return false;

    depth= tga.bpp/ tga.bpc;
    dx= tga.dx;
    dy= tga.dy;
    bitmap= (uchar *)tga.bitmap;
    bpc= tga.bpc;
    bpp= tga.bpp;
  }

  w->dx= (short)dx;
  w->dy= (short)dy;
  w->x0= m->x0+ ((m->dx- w->dx)/ 2);
  w->y0= m->y0+ ((m->dy- w->dy)/ 2);
  w->name= "Splash Window";

  #ifdef OS_WIN
  WNDCLASSEX wc;                    /// extended window class, used for window creation
  HDC memDC= null;                  /// hdc for the DIB bitmap
  HBITMAP memBM= null;              /// handle for the DIB bitmap

  w->_hInstance = GetModuleHandle(NULL);                  /// grab an instance for window

  for(int a= 0; a< sizeof(wc); a++) ((char *)&wc)[a]= 0;  /// memset(&wc, 0, sizeof(wc));
  wc.cbSize= sizeof(WNDCLASSEX);
  wc.hIconSm= LoadIcon(NULL, IDI_APPLICATION);
  wc.style= CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc= (WNDPROC)processMSG;
  wc.cbClsExtra= 0;
  wc.cbWndExtra= 0;
  wc.hInstance= w->_hInstance;
  wc.hIcon= LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor= LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground= 0; // (HBRUSH) CreatePatternBrush(bmp); - bmp would be created using CreateBitmap from bitmap pointer- this variant won't work with alpha i think
  wc.lpszClassName= w->name;

  if(!RegisterClassEx(&wc)) { error.simple("osi::createSplashWindow(): RegisterClassEx failed"); goto Fail; }
  
  long y0= display.vdy- display.vy0- (w->y0+ w->dy);              /// coordonate unification changed y0

  w->_hWnd= CreateWindowEx(WS_EX_LAYERED, w->name, w->name,       /// type & name
                           WS_VISIBLE| WS_POPUP,                  /// more attribs
                           w->x0, y0, w->dx, w->dy,               /// position & size
                           (primWin? primWin->_hWnd: NULL),       /// parent window
                           NULL, w->_hInstance, NULL);            /// menu/ hInstance/ val to be passed to the window creation- nothing important

  if(!w->_hWnd)                   { error.simple("osi::createSplashWindow(): CreateWindowEx() failed"); goto Fail; }
  if(!(w->_hDC= GetDC(w->_hWnd))) { error.simple("osi::createSplashWindow(): GetDC() failed");          goto Fail; }
  
  SetLayeredWindowAttributes(w->_hWnd, 0, 0, LWA_COLORKEY); // LWA_COLORKEY / LWA_ALPHA (this just changes global window alpha)

  /// create a bitmap - needs a hdc + HBITMAP handle
  memDC= CreateCompatibleDC(w->_hDC);

  uchar garbage= (dx% 4? 4- (dx% 4): 0);  /// bitmap (windows bmp's) scanlines must be divisible by 4, if they ain't, garbage is added at the end
  BITMAPINFO b;
  ZeroMemory(&b, sizeof(BITMAPINFO));
  b.bmiHeader.biSize= sizeof(BITMAPINFOHEADER);
  b.bmiHeader.biWidth= dx+ garbage;
  b.bmiHeader.biHeight= dy;
  b.bmiHeader.biPlanes= 1;                /// there are 4 planes, kinda, but msdn says only place 1 here... very professional
  b.bmiHeader.biBitCount= bpp;            /// four 8-bit components 
  b.bmiHeader.biCompression= BI_RGB;
  b.bmiHeader.biSizeImage= dx* dy* depth;

  uchar *p= null;                         /// this will point to the bitmap's internal data

  memBM= CreateDIBSection(memDC, &b, DIB_RGB_COLORS, (void **)&p, null, 0); /// CreateDIBitmap failed for me, unfortunately, probly there's a way
  if(!SelectObject(memDC, memBM))     { error.window("osi::createSplashWindow: SelectObject() failed"); goto Fail; }

  /// RGBA top to bottom-> BGRA bottom to top
  for(uint a= 0; a< dy; a++)
    for(uint b= 0; b< dx; b++) {
      uint pos1= (a* (dx+ garbage)* depth)+ (b* depth);
      uint pos2= ((dy- a- 1)* dx* depth)+   (b* depth);
      p[pos1+ 0]= bitmap[pos2+ 2];
      p[pos1+ 1]= bitmap[pos2+ 1];
      p[pos1+ 2]= bitmap[pos2+ 0];
      if(bpp== 32) p[pos1+ 3]= bitmap[pos2+ 3];
    }

  if(bpp== 32) {
    /// alpha blending / blend func
    BLENDFUNCTION blend;
    blend.BlendOp= AC_SRC_OVER;
    blend.BlendFlags= 0;
    blend.SourceConstantAlpha= 255;
    blend.AlphaFormat= AC_SRC_ALPHA;
    if(!GdiAlphaBlend(w->_hDC, 0, 0, w->dx, w->dy, memDC, 0, 0, dx, dy, blend)) { error.window("osi::createSplashWindow: GdiAlphaBlend failed"); goto Fail; }
  } else {
    if(!BitBlt(w->_hDC, 0, 0, dx, dy, memDC, 0, 0, SRCCOPY)) { error.simple("osi::createSplashWindow: BitBlt failed"); goto Fail; }
  }
  
  ShowWindow(w->_hWnd, SW_SHOW);
  //RedrawWindow(w->_hWnd, 0, 0, RDW_UPDATENOW);
  
  return true;

Fail:
  if(memBM) DeleteObject(memBM);
  if(memDC) DeleteDC(memDC);
  w->delData();
  return false;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  // makeme <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  
  
  
  
  #endif /// OS_LINUX

  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC

  return true;
}



void osinteraction::setProgramIcon(cchar *file) {
  if(!primWin) return;
  
  /// s will hold the file extension to be loaded, in lowercase
  string8 s("   ");
  size_t len= string8::strlen(file);
  
  for(char a= 0; a< 3; a++)
    s.d[a]= file[len- 3+ a];

  s.lower();

  PNG png;
  TGA tga;

  uchar *bitmap= null;
  ulong dx, dy;
  uchar depth;
  
  /// check the file extension
  if(s== "png") {
    png.load(file);
    if(png.type!= IMG_RGB && png.type!= IMG_RGBA)
      return;

    depth= png.bpp/ png.bpc;
    dx= png.dx;
    dy= png.dy;
    bitmap= (uchar *)png.bitmap;
  } else if(s== "tga") {
    tga.load(file);
    if(tga.type!= IMG_RGB && tga.type!= IMG_RGBA)
      return;

    depth= tga.bpp/ tga.bpc;
    dx= tga.dx;
    dy= tga.dy;
    bitmap= (uchar *)tga.bitmap;
  }

  #ifdef OS_WIN
  /// RGB(A)->BGR(A) conversion
  for(ulong a= 0; a< dy; a++)
    for(ulong b= 0; b< dx; b++) {
      uchar t= bitmap[a* dx* depth+ b* depth+ 0];
      bitmap[a* dx* depth+ b* depth+ 0]= bitmap[a* dx* depth+ b* depth+ 2];
      bitmap[a* dx* depth+ b* depth+ 2]= t;
    }

  HICON hIcon= CreateIcon(null, dx, dy, depth, 8, null, bitmap);
  SendMessage(primWin->_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

  // small icon???
  //hIconSm = LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
  //SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
  #endif

  #ifdef OS_LINUX
  
// WIP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  
  // research:
  /*
   * 
   * i think an .ico lib and .png lib must be in osi (maybe more img types, too... heck, why not osi to handle multiple img types...)
   * this will help for a splash screen image too
   * 
   * you can include something like #include "icon.bmp", and XReadBitmap funcs know how to handle this weird thing
   * 
   * 
   * interesting funcs
   * XCreatePixmapFrobBitmap (dunno exactly the name)
   * XSetWMIconName(display, win, &icon_name_property); <<< dunno about this name, must check when it is shown
   * 
   * */
    
  /* include the definition of the bitmap in our program. */
  //#include "icon.bmp";

  /* pointer to the WM hints structure. */
  
  // all this should work, as long as iconPixmap is populated with data somehow
  
  

  
  /* load the given bitmap data and create an X pixmap containing it. */
  // = XCreateBitmapFromData(display, win, icon_bitmap_bits, icon_bitmap_width, icon_bitmap_height);
  Pixmap iconPixmap= XCreatePixmapFromBitmapData(_dis, primWin->_win, (char *)bitmap, dx, dy, 0, 0, depth);
  
  if(!iconPixmap)
    error.simple("XCreatePixmapFromBitmapData - error creating pixmap\n");

  XWMHints* winHints= XAllocWMHints();
  winHints->flags= IconPixmapHint;                /// specify which size hints to fill in
  winHints->icon_pixmap= iconPixmap;              /// specify the actual Pixmap
  
  XSetWMHints(_dis, osi.primWin->_win, winHints); /// pass the hints to the window manager

  XFree(winHints);
    
    
// WIP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  #endif

  

  #ifdef OS_MAC
  makeme
  #endif
}








// THREADS =====================---------------------------
// scraped: std::thread is way easier, no special libraries/compiler options needed in linux

//void osinteraction::startThread(void func(void *)) {
//  #ifdef OS_WIN
  // CreateThread(..) option 1 - thing is, _beginthread is more advanced than these funcs, i think.
  // >> windows, it seems wants these funcs gone, and replaced in windows 8 with some 'windows store' compatible object which is crap <<
  // >> so support for xp / win 7 will go away? will see in time, what the heck they want <<

  // msdn:
  // Like the Win32 ExitThread API, _endthreadex does not close the thread handle. Therefore, when you use _beginthreadex and _endthreadex,
  // you must close the thread handle by calling the Win32 CloseHandle API.
//  _beginthread(func, 0, null);
//  #endif /// OS_WIN

//  #ifdef OS_LINUX
/* WARNING:
 * XInitThreads() function initializes Xlib support for concurrent threads.
 *   This function must be the first Xlib function a multi-threaded program calls,
 *   and it must complete before any other Xlib call is made. 
 * 
 * XLock/UnlockDisplay() - keep an eye on, something like this might be implemented for all os-es, for threads
 * 
 * std::threads works on every os... it's new.... 
 * 
 */
//  pthread_t thread;
//  pthread_attr_t attr;
//  pthread_attr_init(&attr);
//  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
//  pthread_create(&thread, &attr, (void *(*)(void *))func, null);
//  pthread_attr_destroy(&attr);
//  #endif /// OS_LINUX

//  #ifdef OS_MAC
//  mekeme
//  #endif /// OS_MAC
//}

//void osinteraction::endThread(int status) {
//  #ifdef OS_WIN
//  _endthread();       /// endthreadex() does not close the handle, beware, if used instead of this
//  #endif /// OS_WIN
//
//  #ifdef OS_LINUX
//  pthread_exit(&status);
//  #endif /// OS_LINUX
//
//  #ifdef OS_MAC
//  makeme
//  #endif /// OS_MAC
//}









#ifdef OS_WIN
string osinteraction::_getWinName(HWND h) {
  for(int a= 0; a< MAX_WINDOWS; a++)
    if(win[a]._hWnd== h)
      return win[a].name;
  return "unknown window";
}

osiWindow *osinteraction::_getWin(HWND h) {
  for(int a= 0; a< MAX_WINDOWS; a++)
    if(win[a]._hWnd== h)
      return &win[a];
  return null;
}
#endif /// OS_WIN

#ifdef OS_LINUX
osiWindow *osinteraction::getWin(Window *w) {
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(win[a]._win== *w)
      return &win[a];
  return null;
}
#endif

#ifdef OS_MAC
osiWindow *osinteraction::getWin(void *w) {
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(win[a]._win== w)
      return &win[a];
  return null;
}
#endif /// OS_MAC





// system EVENTS / MESSGES handler funcs
// -------------------------------------
#ifdef OS_WIN
LRESULT CALLBACK processMSG(HWND hWnd, UINT m, WPARAM wParam, LPARAM lParam) {

  ///===================================================
  // return 0 is faster than defWindowProc(what goto ret does)
  // it would work good for fullscreen/windowed fullscreen, but in windowed mode
  // there is no resize/move for windows, the close button won't work either, i think
  ///===================================================

  bool onlyHandled= true; /// used with chatty
  bool chatty= true;
  bool timeFunc= false;   /// measure the time this func takes to finish
  uint64 start, end;      /// used with timeFunc
  osiWindow *w;           /// window that received the message
//  WINDOWPOS *tw;          /// used for window position change messages

  if(timeFunc) osi.getNanosecs(&start);

  int mb= 0;

  // mouse messages ==============-------------------
  if((m>= WM_MOUSEFIRST)&& (m<= WM_MOUSELAST)) {
    if(!osi.flags.haveFocus)
      goto ret;

    /// mouse in [MODE 2], THE WHEEL IS NOT POSSIBLE TO READ with funcs, so events are the only way (decent way, anyways)
    if(in.m.mode== 2)
      if(m== WM_MOUSEWHEEL) {
        in.m.wheel+= (GET_WHEEL_DELTA_WPARAM(wParam)> 0)? 1: -1;
        //if(chatty) printf("mouse: wheel rotated\n");
        goto ret;
      }

    /// mouse in [MODE 1]- all values are handled from here on
    if(in.m.mode== 1) {
      if(m== WM_MOUSEMOVE) {
        /// removed oldx&y, dx&y, they are updated when in.update() is called; deltas are always on, now.
        /// these are inside window positions
        if(w= osi._getWin(hWnd)) {                /// i had an instance that a msg from an unknown window was sent, so safety checks must be made
          in.m.x= ((int)(short)LOWORD(lParam));   /// msdn says not to use loword; this is what GET_X_PARAM does
          in.m.y= ((int)(short)HIWORD(lParam));
          /// coordonate unification
          in.m.y= w->y0+ (w->dy- 1)- in.m.y;
          in.m.x= w->x0+ in.m.x;
        }
        
        goto ret; //return 0; // it is faster, but no windows move/resize!!!

      } else if(m== WM_SETCURSOR) {                                         /// to be or not to be
        goto ret;

      } else if(m== WM_LBUTTONDOWN) {                                       /// left mouse button
        osi.getMillisecs(&in.m.b[0].timeStart);
        in.m.b[0].down= true;

        //if(chatty) printf("mouse: l button pressed\n");
        goto ret; //return 0;

      } else if(m== WM_LBUTTONUP) {
        in.m.b[0].lastTimeStart= in.m.b[0].timeStart;
        osi.getMillisecs(&in.m.b[0].lastTimeEnded);
        in.m.b[0].lastDT= in.m.b[0].lastTimeEnded- in.m.b[0].lastTimeStart;
        in.m.b[0].down= false;

        //if(chatty) printf("mouse: l button released\n");
        goto ret; //return 0;

      } else if(m== WM_RBUTTONDOWN) {                                       /// right mouse button
        osi.getMillisecs(&in.m.b[1].timeStart);
        in.m.b[1].down= true;

        //if(chatty) printf("mouse: r button pressed\n");
        goto ret; //return 0;

      } else if(m== WM_RBUTTONUP) {
        in.m.b[1].lastTimeStart= in.m.b[1].timeStart;
        osi.getMillisecs(&in.m.b[1].lastTimeEnded);
        in.m.b[1].lastDT= in.m.b[1].lastTimeEnded- in.m.b[1].lastTimeStart;
        in.m.b[1].down= false;

        //if(chatty) printf("mouse: r button released\n");
        goto ret; //return 0;

      } else if(m== WM_MBUTTONDOWN) {                                       /// middle mouse button
        osi.getMillisecs(&in.m.b[2].timeStart);
        in.m.b[2].down= true;

        //if(chatty) printf("mouse: m button pressed\n");
        goto ret; //return 0;

      } else if(m== WM_MBUTTONUP) {
        in.m.b[2].lastTimeStart= in.m.b[2].timeStart;
        osi.getMillisecs(&in.m.b[2].lastTimeEnded);
        in.m.b[2].lastDT= in.m.b[2].lastTimeEnded- in.m.b[2].lastTimeStart;
        in.m.b[2].down= false;

        //if(chatty) printf("mouse: m button released\n");
        goto ret; //return 0;

      } else if(m== WM_MOUSEWHEEL) {                                        /// wheel
        in.m._twheel+= (GET_WHEEL_DELTA_WPARAM(wParam)> 0)? 1: -1;

        //if(chatty) printf("mouse: wheel rotated\n");
        goto ret; //return 0;

      } else if(m== WM_XBUTTONDOWN) {
        if(GET_XBUTTON_WPARAM(wParam)== XBUTTON1) {                         /// button 4 press
          osi.getMillisecs(&in.m.b[3].timeStart);
          in.m.b[3].down= true;

          //if(chatty) printf("mouse: button 4 pressed\n");
          goto ret; //return 0;
        } 
        if(GET_XBUTTON_WPARAM(wParam)== XBUTTON2) {                         /// button 5 press
          osi.getMillisecs(&in.m.b[4].timeStart);
          in.m.b[4].down= true;

          //if(chatty) printf("mouse: button 5 pressed\n");
          goto ret; //return 0;
        }
      } else if(m== WM_XBUTTONUP) {
        if(GET_XBUTTON_WPARAM(wParam)== XBUTTON1) {                         /// button 4 release
          in.m.b[3].lastTimeStart= in.m.b[3].timeStart;
          in.m.b[3].lastTimeEnded= osi.eventTime;
          osi.getMillisecs(&in.m.b[3].lastTimeEnded);
          in.m.b[3].down= false;
          //if(chatty) printf("mouse: button 4 released\n");
          goto ret; //return 0;

        } else if(GET_XBUTTON_WPARAM(wParam)== XBUTTON2) {                  /// button 5 release
          in.m.b[4].lastTimeStart= in.m.b[4].timeStart;
          osi.getMillisecs(&in.m.b[4].lastTimeEnded);
          in.m.b[4].lastDT= in.m.b[4].lastTimeEnded- in.m.b[4].lastTimeStart;
          in.m.b[4].down= false;
          //if(chatty) printf("mouse: button 5 released\n");
          goto ret; //return 0;
        }
      } else {
        if(chatty) printf("UNKNOWN MOUSE MESSAGE\n");
      } /// pass thru all possible mouse messages
    } /// if mouse is in [MODE 1]
  } /// if this is a mouse message

  // keyboard messages [MODE 1]
  if((m>= WM_KEYFIRST)&& (m<= WM_KEYLAST)) {

  if(in.k.mode== 1) {
    switch(m) {
      case WM_KEYDOWN:                                          // ***key PRESS***
      case WM_SYSKEYDOWN: {
        osi.getMillisecs(&osi.eventTime);     /// using getTIMEXXX() funcs: can't rely on event time sent from system
        in.k.updateLocks();
        osi.flags.keyPress= true;

        int code= GETBYTE2UINT32(lParam);
        uint vcode= (uint)wParam;
        osiKeyboard::KeyPressed k;

        /// left/ right ALT/CONTROL/SHIFT distingush
        if(wParam== VK_SHIFT)   code= (GetKeyState(VK_RSHIFT)& 0x80)?   in.Kv.rshift: in.Kv.lshift;
        if(wParam== VK_CONTROL) code= (GetKeyState(VK_RCONTROL)& 0x80)? in.Kv.rctrl:  in.Kv.lctrl;
        if(wParam== VK_MENU)    code= (GetKeyState(VK_RMENU)& 0x80)?    in.Kv.ralt:   in.Kv.lalt;
        //if(wParam== VK_RETURN)  code= (GetKeyState(VK_RETURN)& 0x80)?   in.Kv.enter:  in.Kv.kpenter; // makes no difference (win's fault)

        /// check if message is a <repeat key press>
        if(KF_REPEAT& HIWORD(lParam)) {
          if(!in.k.key[code]) {               /// in case shit happened (alt tab mess, some crappy windows message block dunno)
            /// log the key
            k.code= code;
            k.checked= false;
            k.timeDown= osi.eventTime;
            k.timeUp= 0;
            k.timeDT= 0;
            in.k._log(k);
            /// set the vars
            in.k.key[code]= 128;
            in.k.keyTime[code]= osi.eventTime;
            //in.k.repeat[code]= 1;           /// with the next repeat increase (next line), the key will be logged as repeated twice, wich is ok
          }
          //in.k.repeat[code]+= KF_REPEAT& HIWORD(lParam);   // THIS MIGHT BE DELETED <-------------------------------------------

          in.k._doManip();                         /// check if current pressed keys form a manip char
          //if(chatty) printf("key REPEAT code[0x%X] vcode[0x%X]\n", code, vcode);
          goto ret; //return 0;
        }
        /// log the key
        k.code= code;
        k.checked= false;
        k.timeDown= osi.eventTime;
        in.k._log(k);
        /// set the key as pressed & other needed vars
        in.k.key[code]= 128;
        in.k.keyTime[code]= osi.eventTime;
        //in.k.repeat[code]= 1;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED
        
        in.k._doManip();                          /// check if current pressed keys form a manip char
        //if(chatty) printf("key PRESS code[0x%X] vcode[0x%X]\n", code, vcode);
        goto ret; //return 0;
      }
      case WM_KEYUP:                              // <<< key DEPRESS >>>
      case WM_SYSKEYUP: {
        in.k.updateLocks();
        osi.flags.keyPress= false;

        osi.getMillisecs(&osi.eventTime);        /// using getTIMEXXX() funcs: can't rely on event time sent from system

        int code= GETBYTE2UINT32(lParam);        /// key code
        uint vcode= (uint)wParam;                /// windows virtual key code
        if(wParam== VK_SHIFT)   code= in.k.key[in.Kv.rshift]? in.Kv.rshift: in.Kv.lshift;
        if(wParam== VK_CONTROL) code= in.k.key[in.Kv.rctrl]?  in.Kv.rctrl:  in.Kv.lctrl;
        if(wParam== VK_MENU)    code= in.k.key[in.Kv.ralt]?		in.Kv.ralt:   in.Kv.lalt;
        //if(wParam== VK_RETURN)  code= in.k.key[VK_RETURN]?    in.Kv.enter:  in.Kv.kpenter;       // makes no difference (win's fault)

        //if(chatty) printf("key RELEASE code[0x%X] vcode[0x%X]\n", code, vcode);

        /// log the key in history
        bool found= false;
        for(short a= 0; a< MAX_KEYS_LOGGED; a++)
          if(in.k.lastKey[a].code== code) {
            if(in.k.lastKey[a].timeUp) continue;
            in.k.lastKey[a].timeUp= osi.eventTime;
            in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
            found= true;
            break;
          }
        /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
        if(!found)  {
          osiKeyboard::KeyPressed k;
          k.code= code;
          k.checked= false;
          k.timeDown= osi.eventTime- 1;   /// 1 milisecond before the keyup
          k.timeUp= osi.eventTime;
          k.timeDT= 1;                    /// timeUp- timeDown
          in.k._log(k);
        }

        /// set the key as pressed & other vars
        in.k.key[code]= 0;
        in.k.keyTime[code]= 0;
        // in.k.repeat[code]= 0;  // repeat[] is subject for deletion

        goto ret; //return 0;
      }
    } /// switch
  } /// if keyboard is in mode 1

  if(in.k.mode== 2) {
    switch(m) {
      case WM_KEYDOWN:                    // key PRESS
      case WM_SYSKEYDOWN:
      case WM_KEYUP:                      // key PRESS
      case WM_SYSKEYUP:
        in.k.updateLocks();               /// to be or not to be...  this should be just empty code here...
        in.k.update();
        //if(chatty) printf("key update");
        goto ret;
    } /// switch
  } /// if keyboard is in mode 2
  } /// if this is a keyboard message


  // handled windows messages
  switch(m) {
    case WM_ACTIVATEAPP:
      if(wParam== 1) {                     // ---=== application GAINS focus ===---
        /// if any HIDs are using exclusive mode, aquire it
        in.m.aquire();
        in.k.aquire();
        for(short a= 0; a< 20; a++)       /// pass thru all sticks
          if(in.j[a].active)
            in.j[a].aquire();

        /// change resolution in case of fullscreen
        for(short a= 0; a< MAX_WINDOWS; a++) 
          if(osi.win[a].isCreated)
            if(osi.win[a].mode== 2)
              osi.display.changeRes(osi.win[a].monitor, osi.win[a].dx, osi.win[a].dy, osi.win[a].bpp, osi.win[a].freq);

        /// show windows in case they are minimized
        for(short a= 0; a< MAX_WINDOWS; a++) 
          if(osi.win[a].isCreated)
            if(osi.win[a].mode== 2) {
              ShowWindow(osi.win[a]._hWnd, SW_RESTORE);
              osi.flags.minimized= false;
              MoveWindow(osi.win[a]._hWnd, osi.win[a].monitor->x0, osi.win[a].monitor->_y0, osi.win[a].dx, osi.win[a].dy, false);
              if(chatty) printf("window %d x0[%d] y0[%d] dx[%d] dy[%d]\n", a, osi.win[a].monitor->x0, osi.win[a].monitor->y0, osi.win[a].dx, osi.win[a].dy);
            }
        SetForegroundWindow(osi.primWin->_hWnd);
        osi.flags.haveFocus= true;        /// set flag, the last

      } else {                             // ---=== application LOSES focus ===---
        osi.flags.haveFocus= false;       /// set flag, first

        /// if any HIDs are using exclusive mode, unaquire it
        in.m.unaquire();
        in.k.unaquire();
        for(short a= 8; a< 20; a++)      /// direct input & xinput sticks unaquire
          if(in.j[a].active)
            in.j[a].unaquire();

        /// in case of alt-tab all current pressed buttons must be reset
        in.resetPressedButtons();

        /// minimize windows in case of fullscreen
        for(short a= 0; a< MAX_WINDOWS; a++) 
          if(osi.win[a].isCreated)
            if(osi.win[a].mode== 2) {
              if(&osi.win[a]== osi.primWin) /// main window gets minimized
                ShowWindow(osi.win[a]._hWnd, SW_MINIMIZE);
              else                          /// all other windows must be hidden, else they get minimized into a small box
                ShowWindow(osi.win[a]._hWnd, SW_HIDE);
              osi.flags.minimized= true;
            }

        /// change back original monitors resolutions in case of fullscreen
        for(short a= 0; a< MAX_WINDOWS; a++) 
          if(osi.win[a].isCreated)
            if(osi.win[a].mode== 2)
              osi.display.restoreRes(osi.win[a].monitor);

      } /// switch gain/lose focus

      if(chatty) printf("WM_ACTIVATEAPP %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam);
      goto ret;
    case WM_POWERBROADCAST:

      // ---=== system ENTERING suspend mode ===---
      if(wParam== PBT_APMSUSPEND) {
        osi.flags.systemInSuspend= true;

      // ---=== system EXITING suspend mode ===---
      } else if(wParam== PBT_APMRESUMESUSPEND || wParam== PBT_APMRESUMEAUTOMATIC || wParam== PBT_APMRESUMECRITICAL) {
        osi.flags.systemInSuspend= false;
      }
      goto ret;

    case WM_CLOSE:
      if(chatty) printf("WM_CLOSE %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam);
      osi.flags.exit= true;     /// main exit flag
      return 0;

    case WM_CHAR:
      if(chatty) printf("WM_CHAR %s %lc\n", osi._getWinName(hWnd).d, wParam);
      osi.getMillisecs(&osi.eventTime);
      in.k._addChar((ulong)wParam, &osi.eventTime);
      return 0;

    case WM_UNICHAR:
      error.console("WM_UNICHAR not tested");
      if(chatty) printf("WM_UNICHAR %s %lc\n", osi._getWinName(hWnd).d, wParam);
      osi.getMillisecs(&osi.eventTime);
      in.k._addChar((ulong)wParam, &osi.eventTime);
      return 0;

    case WM_DEVICECHANGE:
      in.populate();                        /// a call to in.populate to rescan for joysticks/gamepads/gamewheels
      goto ret;

    case WM_MOVE:
      /// hanles normal windows (no fullscreens)
      if(w= osi._getWin(hWnd))              /// safety check; 'unknown windows' msgs happened in Win7
        if(w->mode== 1) {
          w->x0= (int)(short)LOWORD(lParam);
          w->y0= (osi.display.vdy)- (int)(short)HIWORD(lParam)- w->dy;
        }
      goto ret;

    case WM_SIZE:
      if(wParam== SIZE_RESTORED) {      /// handling only window size change (there are minimize and maximize messages here)
        if(w= osi._getWin(hWnd))        /// safety check; 'unknown windows' msgs happened in Win7
          if(w->mode== 1) {
            w->dx= LOWORD(lParam);
            w->dy= HIWORD(lParam);
            osi.resizeGLScene(w->dx, w->dy);
          }
      }
      goto ret;




    case WM_SETFOCUS:         /// focus gained to a window
      if(w= osi._getWin(hWnd)) w->hasFocus= true;

      if(chatty) printf("WM_SETFOCUS %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam);
      goto ret;

    case WM_KILLFOCUS:        /// window lost focus
      if(w= osi._getWin(hWnd)) w->hasFocus= false;

      if(chatty) printf("WM_KILLFOCUS %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam);
      goto ret;

    // system commands
    case WM_SYSCOMMAND:
      if(chatty) printf("WM_SYSCOMMAND %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam);

      switch (wParam)	{
        case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				  return 0;                         /// prevent these from happening by not calling DefWinProc
        case SC_CLOSE: 
          if(chatty) printf("  SC_CLOSE %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam);
          osi.flags.exit= true;
          return 0;
      } /// switch the type of WM_SYSCOMMAND
    goto ret; //return 0;


    case WM_WINDOWPOSCHANGING:
      /*
      tw= (WINDOWPOS *)&lParam;
      w= osi.getWin(hWnd);
      /// hanles normal windows (no fullscreens)
      if(w)
      if(w->mode== 0) {
        w->x0= tw->x;
        w->y0= osi.display.vdy- tw->y- 1;
        w->dx= tw->cx;
        w->dy= tw->cy;
      }
      */
      goto ret;

    //case WM_PAINT:      // TEST
    //  return 0;
  } /// switch message

  /// unhandled frequent windows messages
  if(chatty&& !onlyHandled)
    switch(m) {
      case WM_ACTIVATE: if(chatty) printf("WM_ACTIVATE %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam); goto ret;
      case WM_ERASEBKGND: if(chatty) printf("WM_ERASEBKGND %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam); goto ret;
      case WM_PAINT: if(chatty) printf("WM_PAINT %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam); goto ret;
      case WM_NCPAINT: if(chatty) printf("WM_NCPAINT %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam); goto ret;
      case WM_NCACTIVATE: if(chatty) printf("WM_NCACTIVATE %s 0x%x %d %d\n", osi._getWinName(hWnd).d, m, wParam, lParam); goto ret;
      case WM_GETICON: if(chatty) printf("WM_GETICON\n"); goto ret;              /// usually is used when alt-tabbing, gets an icon for the mini alt-tab list
        // WHEN dealing with icons, must remember to develop WM_GETICON too
      case WM_IME_NOTIFY: if(chatty) printf("WM_IME_NOTIFY\n"); goto ret;
      case WM_IME_SETCONTEXT: if(chatty) printf("WM_IME_SETCONTEXT\n"); goto ret;
      case WM_NCHITTEST: goto ret;            /// something to do with mouse placement
      case WM_NCMOUSEMOVE: goto ret;          /// non client area mouse coords (top title/ moving bar, is a non-client for example)
      case WM_NCMOUSELEAVE: goto ret;         /// non client area mouse leaving window
    } /// switch

  if(chatty&& !onlyHandled)
    printf("UNKNOWN %s 0x%x %d %d\n", osi._getWinName(hWnd), m, wParam, lParam);
  /// this DefWindowProc() handles window movement & resize & rest... without this, moving is not working, for example
ret:
  if(timeFunc) { osi.getNanosecs(&end); printf("processMSG lag: %llu\n", end- start); }
  return DefWindowProc(hWnd, m, wParam, lParam);
}

#endif /// OS_WIN



#ifdef OS_LINUX
bool osinteraction::_processMSG()  {
  bool chatty= false;
  bool ret= false;                        /// return value. initially false - if a message is processed, ret= true;
  XEvent event;
  osiWindow *w= null;

  while(XPending(_dis)) {                 /// while there are messages in queue, loop thru them
    XNextEvent(_dis, &event);
    ret= true;                            /// if a message is processed, return value is true
    
// ########################### MOUSE EVENTS ################################# //
      
// ---------------============ MOUSE MOVEMENT ===============-------------------
    if(event.type == MotionNotify) { /// this is the first event handled, because it is spammed
      /// oldx&y, dx&y removed; now updated on each in.update() call 
      in.m.x= event.xmotion.x_root;
      in.m.y= osi.display.vdy- event.xmotion.y_root- 1; // test this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

      continue;

// ---------------============ BUTTON PRESS =================-------------------
    } else if(event.type == ButtonPress) {
      flags.buttonPress= true;
      eventTime= event.xbutton.time;          /// compatible with getMillisecs()

      if(event.xbutton.button== 4) {          // wheel up
        in.m._twheel+= 1;
        continue;
      }
      if(event.xbutton.button== 5) {          // wheel down
        in.m._twheel-= 1;
        continue;
      }

      short a;
      if(event.xbutton.button< 4) {           // first 3 buttons
        a= event.xbutton.button- 1;
        /// of course in windows there is another button order than in linux
        if(a== 1)
          a= 2;
        else if(a== 2)
          a= 1;
      } else                                  /// the rest of buttons are located after the mouse wheel
        a= event.xbutton.button- 3;

      in.m.b[a].down= true;
      in.m.b[a].timeStart= eventTime;
      continue;

// ---------------============ BUTTON RELEASE ===============-------------------
    } else if(event.type == ButtonRelease) {
      eventTime= event.xbutton.time;          /// compatible with getMillisecs()
      flags.buttonPress= false;

      /// ignore mouse wheel up msgs (linux sends them @ same time as button press)
      if((event.xbutton.button== 4) || (event.xbutton.button== 5))
        continue;

      short a;
      if(event.xbutton.button< 4) {           /// first 3 buttons?
        a= event.xbutton.button- 1;
        // of course in windows there is another button order than in linux
        if(a== 1)
          a= 2;
        else if(a== 2)
          a= 1;
      } else                                  /// the rest of buttons are located after the mouse wheel
        a= event.xbutton.button- 3;

      if(in.m.b[a].down) {                    /// an alt-bab might mess stuff...
        in.m.b[a].lastTimeStart= in.m.b[a].timeStart;
        in.m.b[a].lastTimeEnded= eventTime;
        in.m.b[a].lastDT= in.m.b[a].lastTimeEnded- in.m.b[a].lastTimeStart;
      } else {                                /// an alt-tab? might mess stuff
        in.m.b[a].lastTimeEnded= eventTime;
        in.m.b[a].lastTimeStart= eventTime- 1;
        in.m.b[a].lastDT= 1;
      }
      in.m.b[a].down= false;

      continue;

    // ############################ KEYBOARD EVENTS ############################# //

    // ---------------================ KEY PRESS ================-------------------
    } else if(event.type == KeyPress) {
      if(in.k.mode!= 1) continue;         /// only keyboard in [mode 1]
      
      flags.keyPress= true;               /// osi main flags
      eventTime= event.xkey.time;         /// compatible with getMillisecs()
      bool repeat= false;                 /// this will hold if it is a repeat character typed
      uint code= event.xkey.keycode;
      
      /// if the key is already down, this is a repeat message
      if(in.k.key[code]== 128)
        repeat= true;
      
      /// characters handling
      
      /// find the linux keysym
      KeySym ks;
      uint mods;
      XkbLookupKeySym(_dis, code, event.xkey.state, &mods, &ks);
      
      /* THERE ARE MODS THAT I CAN'T FIND WHAT THEY DO
      if(mods)
        error.console( (string8("").f("there are still mods not applied to key %d\n", code)).d)
      */

      /// if the keysym can be a character, update the keyboard char stream
      ulong ch;
      in._getUnicode(&ks, &ch);
      if(ch)
        in.k._addChar(ch, &eventTime);
      
      /// if this is a real key press, log it and set vars
      if(!repeat) {
        if(chatty) printf("key PRESS code=%d \n", code);
        osiKeyboard::KeyPressed k;

        /// log the key
        k.code= code;
        k.checked= false;
        k.timeDown= eventTime;
        k.timeUp= 0;
        k.timeDT= 0;
        in.k._log(k);
        /// set the key as pressed & other needed vars
        in.k.key[code]= 128;
        in.k.keyTime[code]= eventTime;
        // in.k.repeat[code]= 128;        /// a new key press, sets repeat to 128  // MIGHT BE DELETED
      } /// if it's not a repeat
      
      in.k.updateLocks();                 /// update keyboard leds (repeat or not)
      in.k._doManip();                     /// check & handle if pressed keys form a manip char
      
      continue;
      
// ---------------=============== KEY RELEASE ===============-------------------
    } else if(event.type == KeyRelease) {
      if(in.k.mode!= 1) continue;             /// only keyboard in [mode 1]
      
      /// check if this is a repeat message (xlib sends key releases for repeats too...)
      if(XPending(_dis)) {
        XEvent next;
        XPeekEvent(_dis, &next);
        if(next.type== KeyPress)
          if(next.xkey.keycode== event.xkey.keycode)
            if(next.xkey.time== event.xkey.time)
              continue;
      }
      
      eventTime= event.xkey.time;             /// compatible with getMillisecs()
      flags.keyPress= false;
      uint code= event.xkey.keycode;
      
      in.k.updateLocks();
      
      if(chatty) printf("key RELEASE code=%d\n", code);

      /// log the key in history
      bool found= false;
      for(short a= 0; a< MAX_KEYS_LOGGED; a++)
        if(in.k.lastKey[a].code== code) {
          if(in.k.lastKey[a].timeUp) continue;
          in.k.lastKey[a].timeUp= eventTime;
          in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
          found= true;
          break;
        }
      
      /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
      if(!found)  {
        osiKeyboard::KeyPressed k;
        k.code= code;
        k.checked= false;
        k.timeUp= eventTime;
        k.timeDown= k.timeUp- 1;              /// 1 milisecond before the keyup
        k.timeDT= 1;                          /// timeUp- timeDown
        in.k._log(k);
      }

      /// set the key as de-pressed & other vars
      in.k.key[code]= 0;
      in.k.keyTime[code]= 0;
      // in.k.repeat[code]= 0;  <<< DELETE ??? (not using)

      continue;



// ###################### WINDOW HANDLING / FOCUS ########################### //
      
// ---------------============== EXPOSE EVENT ===============-------------------
    } else if(event.type == Expose) {

      // IS ANYTHING HERE GOOD FOR ANYTHING????????????????
      // SUBJECT OF DELETION
      w= getWin(&event.xexpose.window);

      if(w->_isMapped)
        XGetWindowAttributes(_dis, w->_win, &w->_gwa); // update gwa window size / attributes

      continue;
//    } else if(event.type == NoExpose) {
//      continue;
            
// ---------------=========== MAP NOTIFY EVENT ==============-------------------
    } else if(event.type == MapNotify) {

      // IS ANYTHING HERE GOOD FOR ANYTHING????????????????
      // SUBJECT OF DELETION

      w= getWin(&event.xmap.event);
      w->_isMapped= true;
      /*
      printf("%s mapped\n", w->name.d);
      if(primWin->isMapped)
        for(short a= 1; a< MAX_WINDOWS; a++)
          if(win[a].isCreated && !win[a].isMapped) {
            XMapWindow(primWin->dis, win[a].win);
            printf("window nr %d mapped\n", a);
            win[a].isMapped= true;

            XStoreName(w->display, w->win, w->name);

          ///  handle the close button WM
            Atom wmDelete= XInternAtom(w->display, "WM_DELETE_WINDOW", True);
            XSetWMProtocols(w->display, w->win, &wmDelete, 1);
          }
  */
      continue;
      
// ---------------=========== UNMAP NOTIFY EVENT ============-------------------
    } else if(event.type == UnmapNotify) {

      // IS ANYTHING HERE GOOD FOR ANYTHING????????????????
      // SUBJECT OF DELETION

      w= getWin(&event.xexpose.window);
      w->_isMapped= false;
      if(chatty) printf("window UNmapped [%s]\n", w->name.d);
      continue;
      
// ---------------============ ENTER NOTIFY =================-------------------
/// mouse moves to a certain window... might be usefull in the future
    } else if(event.type == EnterNotify) {
      if((event.xcrossing.mode== NotifyGrab)|| (event.xcrossing.mode==NotifyUngrab)) /// ignore
        continue;

      if(event.xcrossing.mode== NotifyNormal)
        continue;

// ---------------============ LEAVE NOTIFY =================-------------------
/// mouse leaves a certain window... might be usefull in the future
    } else if(event.type == LeaveNotify) {
      if((event.xcrossing.mode== NotifyGrab)|| (event.xcrossing.mode==NotifyUngrab)) /// ignore grabs
        continue;

      if(event.xcrossing.mode== NotifyNormal)
        continue;

// ---------------=============== FOCUS IN ==================-------------------

    /// once a window is mapped, it is in the hands of WM (window manager)
    /// so use only window manager messages ("_NET_BLABLA")
    } else if(event.type == FocusIn) { // these are spammed
      if(chatty) printf("focusIn\n");

      if(event.xfocus.mode== NotifyNormal) {
        if(flags.haveFocus) {                    // ignore if already focused
          if(chatty) printf("FocusIn:IGNORED: xchange focus between internal program windows\n");
          continue;
        }

      if(chatty) printf("FocusIn:NotifyNormal, haveFocus=%d\n", flags.haveFocus);

        /// fullscreen
        if(primWin->mode== 2|| primWin->mode== 3|| primWin->mode== 4)
          for(short a= 0; a< MAX_WINDOWS; a++)  /// for each (created) window
            if(win[a].isCreated) {
              /// [mode 2] resolution change
              if(win[a].mode== 2) {
                if(chatty) printf("Changing resolution for window [%d]\n", a);
                display.changeRes(win[a].monitor, win[a].dx, win[a].dy, win[a].bpp, win[a].freq);
              }
              /// set the window 'on top' of everything
              win[a]._setWMstate(1, "_NET_WM_STATE_ABOVE");
              win[a]._setWMstate(0, "_NET_WM_STATE_BELOW");
              //setFullScreen(&win[a], true); // THIS IS A POSSIBILITY
            } /// is window created

        flags.minimized= false;
        flags.haveFocus= true;
      } /// NotifyNormal

      continue;

// ---------------=============== FOCUS OUT =================-------------------

    /// once a window is mapped, it is in the hands of WM (window manager)
    /// so use only window manager messages ("_NET_BLABLA")
    /// XIconify i think uses WM messages, so it works
    } else if(event.type == FocusOut) { // these are spammed
      if(chatty) printf("focusOut\n");

      if(event.xfocus.mode== NotifyNormal) {      /// it can be a grab or something
        /// if next event is a focus in, there's only a change of focus between program windows,
        /// so this focus is ignored (same as the next, cuz already have focus)
        if(XPending(_dis)) {
          XEvent tmp;
          XPeekEvent(_dis, &tmp);
          if((tmp.type == FocusIn)&& (tmp.xfocus.mode== NotifyNormal)) {
            if(chatty) printf("FocusOut:IGNORED: xchange focus between internal program windows\n");
            continue;
          }
        }

        if(chatty) printf("FocusOut:NotifyNormal, haveFocus=%d\n", flags.haveFocus);

        if(!flags.haveFocus)                      /// ignore if already not focused
          continue;

        /// fullscreen
        if(primWin->mode== 2|| primWin->mode== 3|| primWin->mode== 4)
          for(short a= 0; a< MAX_WINDOWS; a++)     /// for each (created) window
            if(win[a].isCreated) {
              /// set the window 'below' every other windows
              win[a]._setWMstate(0, "_NET_WM_STATE_ABOVE");
              win[a]._setWMstate(1, "_NET_WM_STATE_BELOW");
              // setFullScreen(&win[a], false); // THIS IS A PosiBILITY

              /// [mode 2] resolution change & window iconification
              if(win[a].mode== 2) {
                if(chatty) printf("Changing to original resolution for window [%d]\n", a);
                XIconifyWindow(_dis, win[a]._win, win[a].monitor->_screen);
                display.restoreRes(win[a].monitor);
                flags.minimized= true;
              }
            } /// if window is created
        
        /// clear every button / key buffer / fix history for every button
        in.k.clearTypedBuffer();
        in.resetPressedButtons();
        
        flags.haveFocus= false;
      } /// if FocusOut:NotifyNormal

      continue;

// ---------------============ CLOSE BUTTON PRESS ===========-------------------
    /// ... it is defined in createGLWindow() as a custom i guess
    /// i think this needs more testing, cuz it seems ALL client msgs are treated
    /// as the close message ATM
    } else if(event.type == ClientMessage) {
      flags.exit= true;
      continue;

// ---------------============== DESTROY NOTIFY =============-------------------
    } else if(event.type == DestroyNotify) {
      error.simple("destroy notify not handled");
      continue;

    } else if(event.type == CirculateNotify) {
      if(chatty) printf("circulate notify\n");
    } else
      if(chatty) printf("Unhandled unknown message\n");

    //} else
    //  XFlush(primWin->display); // why flush msgs? pass thru all, right?
  
  } /// while there are messages in queue
  return ret;
} // osinteraction::processMSG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif /// OS_LINUX


#ifdef OS_MAC
// MAC variant of processMSG is in OScocoa.mm. there is Objective-C specific stuff
#endif /// OS_MAC



bool osinteraction::checkMSG() {
  if(!primWin) return false;    /// if no primary window was created, no messages will be processed
  getNanosecs(&present);       // current time, or 'present' variable updated here <<<
  
  #ifdef OS_WIN
  bool ret= false;              /// return value, start false, if any msg is processed, ret= true
  while(1)    // loop thru ALL msgs... i used to peek thru only 1 msg, that was baaad... biig LAG
    if(PeekMessage(&primWin->_msg, NULL, 0, 0, PM_REMOVE)) {	// Is There A Message Waiting?
      // eventTime= primWin->_msg.time; // not reliable. 1 sec after getMillisecs(). this time is in the dang future
      TranslateMessage(&primWin->_msg);
      DispatchMessage(&primWin->_msg);

      ret= true;
    } else
      break;

  /// i think processMSG is called by WindProc() when DispatchMessage() is called (hopefully !)
  return ret;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  return _processMSG();
  #endif /// OS_LINUX

  #ifdef OS_MAC
  return _processMSG();
  #endif /// OS_MAC
} /// osinteraction::checkMSG









// -----------============= WINDOW CLASS =============------------------
///=====================================================================
#ifdef OS_WIN
MSG osiWindow::_msg= {0};
#endif

#ifdef OS_LINUX
Display *osinteraction::_dis= null;
Display *osiWindow::_dis= null;
#endif

osiWindow::osiWindow() {
  isCreated= false;
  x0= y0= dx= dy= 0;
  freq= bpp= 0;

  monitor= null;
  glr= null;
  
  #ifdef OS_WIN
  _hDC= NULL;
  _hInstance= NULL;
  _hWnd= NULL;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  _root= 0;              /// root window (desktop/server main window/ watever)
  _win= 0;               /// window 'handle' or watever (per monitor)
  _vi= null;
  _isMapped= false;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  _win= null;
  _view= null;
  #endif /// OS_MAC
}

osiWindow::~osiWindow() {
  #ifdef OS_WIN
  delData();            // it seems system already destroys windows, and this causes segmentation error
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  //  delData();        // it seems system already destroys windows, and this causes segmentation error  
  #endif
  
  #ifdef OS_MAC
  // nothing to do atm
  #endif /// OS_MAC
}

void osiWindow::delData() {
  #ifdef OS_WIN
  if(_hDC) {
    ReleaseDC(_hWnd, _hDC);
    _hDC= NULL;
  }

  if(_hWnd) {
    DestroyWindow(_hWnd);
    _hWnd= NULL;
  }

  if(name.len) {
    UnregisterClass(name, _hInstance);   // << in case class name will differ in future
    _hInstance= NULL;
  }
  // do not return;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(isCreated) {
    osi.glMakeCurrent(null);
    //osi.glDestroyRenderer(this); // THIS NEEDS TO BE PUT WITH glRenderer STRUCT WIP
    //glXDestroyContext(display, glRenderer);

    if(mode == 2)
      osi.display.restoreRes(monitor);

    XDestroyWindow(_dis, _win);
    this->_isMapped= false;
  }
  // do not return
  #endif

  #ifdef OS_MAC
  _win= null;
  _view= null;

  // do not return
  #endif


  this->isCreated= false;
  this->mode= 0;
  this->bpp= 0;
  this->dx= 0;
  this->dy= 0;
  this->freq= 0;
  this->glr= null;
  this->monitor= null;
  this->name.delData();
  this->x0= 0;
  this->y0= 0;
}



#ifdef OS_LINUX

void osiWindow::_setWMprop(string8 wmID, string8 wmProp, uint val1, uint val2) {
  Atom wm= XInternAtom(_dis, wmID, False);
  Atom prop= XInternAtom(_dis, wmProp, False);
  ulong data[5]= {val1, val2, 0, 0, 0};

  XChangeProperty(_dis, _win, wm, prop, 32, PropModeReplace, (cuchar*)data, 2);
}


void osiWindow::_setWMstate(uint val, string8 prop1, string8 prop2) {
  XEvent xev;
  for(short a= 0; a< sizeof(xev); a++) ((char*)&xev)[a]= 0;               /// clear xev
/// set vals
  xev.type= ClientMessage;
  xev.xclient.window= _win;
  xev.xclient.message_type= XInternAtom(_dis, "_NET_WM_STATE", False);
  xev.xclient.format= 32;
  xev.xclient.data.l[0]= val;
  xev.xclient.data.l[1]= XInternAtom(_dis, prop1, False);
  if(prop2.d)
    xev.xclient.data.l[2]= XInternAtom(_dis, prop2, False);
  xev.xclient.data.l[3]= 1; // 0= oldApp/ignore, 1= normal app, 2= pagers/other clients
/// send the message to root
  XSendEvent(_dis, _root, False, SubstructureRedirectMask| SubstructureNotifyMask, &xev);
}

#endif /// OS_LINUX




// END osiWindow class //
///------=======------///








void osinteraction::getNanosecs(uint64 *out) {
  #ifdef OS_WIN
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  /// "(t.QuadPart* 1000000000)/ timerFreq.QuadPart" would be faster& simpler,
  /// but unfortunately IT REACHES unsigned long long (uint64) LIMIT;
  /// a solution i found is to use 2 uint64 helper vars, each holding half the final result

  uint64 hi= t.QuadPart/ 10000000;
  uint64 lo= t.QuadPart- (hi* 10000000);
  lo= (lo* 1000000000)/ _timerFreq.QuadPart;
  hi= (hi* 1000000000)/ _timerFreq.QuadPart;
  
  *out= hi* 10000000+ lo;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  *out= (t.tv_sec* 1000000000)+ t.tv_nsec;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  *out= (mach_absolute_time()* machInfo.numer/ machInfo.denom);
  #endif /// OS_MAC
}


void osinteraction::getMicrosecs(uint64 *out) {
  #ifdef OS_WIN
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  /// "(t.QuadPart* 1000000)/ timerFreq.QuadPart" would be faster& simpler,
  /// but unfortunately this ALMOST reaches the maximum value unsigned long long (uint64) can hold;
  /// a solution i found is to use 2 uint64 helper vars, each holding half the final result

  uint64 hi= t.QuadPart/ 10000000;
  uint64 lo= t.QuadPart- (hi* 10000000);
  hi= (hi* 1000000)/ _timerFreq.QuadPart;
  lo= (lo* 1000000)/ _timerFreq.QuadPart;

  *out= hi* 10000000+ lo;

  #endif /// OS_WIN

  #ifdef OS_LINUX
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  *out= (t.tv_sec* 1000000)+ (t.tv_nsec/ 1000);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  *out= (mach_absolute_time()* machInfo.numer/ machInfo.denom)/ 1000;
  #endif /// OS_MAC
}


void osinteraction::getMillisecs(uint64 *out) {
  #ifdef OS_WIN
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  *out= (t.QuadPart* 1000)/ _timerFreq.QuadPart;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  *out= (t.tv_sec* 1000)+ (t.tv_nsec/ 1000000);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  *out= (mach_absolute_time()* machInfo.numer/ machInfo.denom)/ 1000000;
  #endif /// OS_MAC
}

// WIP - linux problems
void osinteraction::getClocks(uint64 *out) {
  #ifdef OS_WIN
  QueryPerformanceCounter((LARGE_INTEGER*)out);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  //clock_getthedamnclocks(); <<<<<<<<<<<<<<<<<<<<
  /*
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  *out= (t.tv_sec* 1000)+ (t.tv_nsec/ 1000000);
  */
  #endif /// OS_LINUX

  #ifdef OS_MAC
  *out= mach_absolute_time();
  #endif /// OS_MAC
}

// WIP - linux problems
void osinteraction::clocks2nanosecs(uint64 *out) {
  #ifdef OS_WIN
  /// there has to be a split because ((*out)* 1000000000)/ timerFreq.QuadPart reaches uint64 limit
  uint64 hi= *out/ 10000000;
  uint64 lo= *out- (hi* 10000000);
  lo= (lo* 1000000000)/ _timerFreq.QuadPart;
  hi= (hi* 1000000000)/ _timerFreq.QuadPart;
  
  *out= hi* 10000000+ lo;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  //<<< well, shait...
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  *out= (t.tv_sec* 1000000000)+ t.tv_nsec;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  *out= ((*out)* machInfo.numer/ machInfo.denom);
  #endif /// OS_MAC
}

// WIP - linux problems
void osinteraction::clocks2microsecs(uint64 *out) {
  #ifdef OS_WIN
  *out= (*out* 1000000)/ _timerFreq.QuadPart;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  //<<< well, shait...
  /*
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  *out= (t.tv_sec* 10000bla00000)+ t.tv_nsec;
  */
  #endif /// OS_LINUX

  #ifdef OS_MAC
  *out= ((*out)* machInfo.numer/ machInfo.denom)/ 1000;
  #endif /// OS_MAC
}

// WIP - linux problems
void osinteraction::clocks2millisecs(uint64 *out) {
  #ifdef OS_WIN
  *out= (*out* 1000)/ _timerFreq.QuadPart;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  //<<< well, shait...
  /*
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  *out= (t.tv_sec* 1000000bla000)+ t.tv_nsec;
  */
  #endif /// OS_LINUX

  #ifdef OS_MAC
  *out= ((*out)* machInfo.numer/ machInfo.denom)/ 1000000;
  #endif /// OS_MAC
}




void osinteraction::exit(int retVal) {
  display.restoreAllRes();
  #ifdef OS_WIN
  ::exit(retVal);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  _exit(retVal);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  _exit(retVal);
  #endif /// OS_MAC
}









///-------------=======================-------------///
// ============= OPENGL SPECIFIC STUFF ============= //
///-------------=======================-------------///






///--------------------------------------///
// OPENGL functions that are OS dependant //
///--------------------------------------///

// SWAP BUFFERS
void osinteraction::swapPrimaryBuffers() {
  if(primWin)
    swapBuffers(primWin);
}


void osinteraction::swapBuffers(osiWindow *w) {
  #ifdef OS_WIN
  //SwapBuffers(w->hDC);      /// standard; the wgl one has more possibilities
  wglSwapLayerBuffers(w->_hDC, WGL_SWAP_MAIN_PLANE);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  glXSwapBuffers(_dis, w->_win);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  cocoa.swapBuffers(w);
  #endif /// OS_MAC
} // osinteraction::swapBuffers



bool osinteraction::glMakeCurrent(osiWindow *w) {
  if(w) {
    if(glr) glr->isActive= false;     /// set not active flag for current renderer
    glr= w->glr; _glr= w->glr;
    if(glr) glr->isActive= true;      /// set active flag of new renderer
  } else {
    if(glr) glr->isActive= false;     /// set not active flag for current renderer
    glr= null; _glr= null;
  }

  #ifdef OS_WIN
  if(w) {
    if(w->glr)
      return wglMakeCurrent(w->_hDC, w->glr->glContext)? true: false;
  } else
    return wglMakeCurrent(null, null)? true: false;
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  
  if(w) {
    if(w->glr)
      return glXMakeCurrent(_dis, w->_win, w->glr->glContext);
    else
      return glXMakeCurrent(_dis, None, null);
  } else
    return glXMakeCurrent(_dis, None, NULL);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  if(w)
    cocoa.makeCurrent(w); // as it is, this is too simple <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< check CGL
    
    // this must be changed-> the glc (core)'s function has the possibility to makecurrent(null) and returns an error message if something went wrong
    
  return false; // <<<< better atm
  return true;
  #endif /// OS_MAC
  
  return false;
} // osinteraction::glMakeCurrent













// NOT OS DEPENDANT pure gl stuff <----------------------------------------------------------
// ------------------------------------------------------------------------------------------

bool osinteraction::resizeGLScene(GLsizei dx, GLsizei dy) {
  bool ret= true;
  if (dy==0)	dy= 1; /// prevent a divide by 0

  glViewport(0, 0, dx, dy);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)dx/ (GLfloat)dy, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  return ret;
}






