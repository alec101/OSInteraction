#include "pch.h"

/* TODO:
 *
 * program path. a string that initializes in OSI constructor (UNDER WIN&LINUX)
 *
 * mach_time, when does it start? more tests with it
 *
 * what happens when the time variables overflow? must do something about that (osi.getNano, etc)
 *
 * system to create a glRenderer for each graphic card (MUST install a second grcard on a computer)
 * gamepad vibration under directinput (it is possible) & force feedback (MUST HAVE A VIBRATION GAMEPAD FIRST...)
 * OSX emulator? to test stuf under osx?
 *
 * test keyboard locks under linux
 * test mouse grab under linux
 * test keyboard grab under linux (first make shure it is possible to exit program)
 *
 * in linux, if messing with visual variables, the black screen flash can go away
 *  therefore, in windows, pfd must be messed with to get rid of the black flash
 *
 * linux keboard manipChars
 *
 * - [win+linux] joystick, xbox controller, wheel (BUY THEM first)
 *
 * - [win] test japanese keyboard, see what chars shows... or print to a file at least
 * - [win] Kv structure
 * - [linux] test Kv structure that is actually working on a arabic (for example) keyboard
 * - [mac] test Kv structure on a complex language keyboards
 * - create a loading window, in the center of the screen? eventually to have image of the game
 *
 * - set an icon for the window;  [win] WHEN dealing with icons, must remember to develop WM_GETICON too
 * - [WIN] WM_SETFOCUS & WM_KILLFOCUS - develop these? WM_ACTIVATE handles focus change atm.
 * - system to create a glRenderer for each graphic card (MUST install a second grcard on a computer) (THIS IS A MUST)
 * - threads!
 * - if there is 1 glRenderer per window, glShareLists knows not to make duplicates if in the same graphics card??? THIS IS THE QUESTION.
 *
 * - Linux: test monitors on duplicate (on mirror, on whatever crap the os calls them)
 *
 * - window to change a monitor without problems (unplug?)
 *   but on multiple monitor mode, mark it as closed? something like this... notify the program somehow, so a rearrangement will be done

// CHANGE THE STYLE I PROGRAM, "void getBla(retval)", not "retval getBla();"
// faster this way ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*/


// LINUX WINDOW RESEARCH

// ONLY 1 DISPLAY MUST BE CREATED. it is 'the connection' to the 'server'(linux/computer, call it what u like)

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


#ifdef OS_WIN
  #pragma comment(lib, "opengl32")
  #pragma comment(lib, "glu32")
  #pragma comment(lib, "Winmm")               /// joystick api support
  #ifdef USING_DIRECTINPUT
    #pragma comment(lib, "dinput8")
    #pragma comment(lib, "dxguid")
  #endif
  #ifdef USING_XINPUT

// something must be done with this <<<<<<<<<<<<<<<<<<<<<<<<<<<
    #pragma comment(lib, "c:/alec/dxSDK2010j/lib/x64/xinput")
  #endif
#endif /// OS_WIN



OSInteraction osi;
ErrorHandling error;
Input in;



// ################
// Testing purposes
// ################


/*
double pov(double x, double y)
  // GOOD VARIANT

  if(y> 0) {
    if(x>= 0)
      ret= (atan(x/ y))* (180.0/ M_PI);
    else
      ret= (2* M_PI+ atan(x/ y))* (180.0/ M_PI);
  } else if(y< 0) {
    ret= ( M_PI+ atan(x/ y))* (180.0/ M_PI);
  } else if(y == 0) {

    if(x== 0)
      ret= -1;
    else if(x> 0)
      ret= 90;
    else if(x< 0)
      ret= 270;
  }
  return ret;
}
*/
/*
int getX(double a) {
  double x;
  x= 10.0* sin(a* (M_PI/ 180.0));

  return x;
}
int getY(double a) {

  double y;
  y= 10.0* cos(a* (M_PI/ 180.0));

  return y;
}
*/

void drawSomething();

int main() {
/*
  printf("Q1[% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f]\n", pov( 0,    100), pov( 5,    100), pov( 50,   100), pov( 100,  100), pov( 100,  50),  pov( 100,  5));
  printf("Q2[% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f]\n", pov( 100,  0),   pov( 100, -5),   pov( 100, -50),  pov( 100, -100), pov( 50,  -100), pov( 5,   -100));
  printf("Q3[% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f]\n", pov( 0,   -100), pov(-5,   -100), pov(-50,  -100), pov(-100, -100), pov(-100, -50),  pov(-100, -5));
  printf("Q4[% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f][% 6.1f]\n", pov(-100,  0),   pov(-100,  5),   pov(-100,  50),  pov(-100,  100), pov(-50,   100), pov(-5,    100));
*/
  /*
  for(short a= 0; a< 4; a++) {
    short b= a* 90;
    printf("Q%d % 4d[% 3d,% 3d] % 4d[% 3d,% 3d] % 4d[% 3d,% 3d] % 4d[% 3d,% 3d]\n", a, b+ 0, getX(b+ 0), getY(b+ 0), b+ 10, getX(b+ 10), getY(b+ 10), b+ 45, getX(b+ 45), getY(b+ 45), b+ 80, getX(b+ 80), getY(b+ 80));
  }
  getchar();
  return 0;
  */


/*
  // some string + time tests
  uint64 t1, t2;
  string s;

  osi.getNanosecs(&t1);

  uint b= 0;
  s= "繋がって or つながって繋がって or つながって繋がって or つながって繋がって or つながって繋がって or つながって繋がって or つながって繋がって or つながって繋がって or つながって繋がって or つながって繋がって or つながって";
  for(uint a= 0; a< 10000000; a++) {
    //s+= "繋がって or つながって";
    string s1= s[4];
    uint c= s.getChar(b++);
    if(b>100) b= 0;
    //printf("%d", b);
  }

  osi.getNanosecs(&t2);

  printf("elapsed time= %lu\n", t2- t1);
  printf("sizeof(bool)= %d\n", sizeof(bool));
  printf("t1= %lu, t2= %lu", t1, t2);
  getchar();
  // <end> string + time tests
*/


  error.useWindowsFlag= true;
  //error.window("test");
  
  osi.display.populate(&osi);     // check all monitors/ resolutions/ etc

  //osi.createGLWindow(&osi.win[0], &osi.display.monitor[0], "window 0", 400, 400, 32, 1);
  //osi.createGLWindow(&osi.win[0], &osi.display.monitor[0], "window 0", 1024, 768, 32, 2);
  //osi.createGLWindow(&osi.win[1], &osi.display.monitor[0], "window 2", 400, 400, 32, 3);
  osi.createGLWindow(&osi.win[0], &osi.display.monitor[0], "window 0", 400, 400, 32, 4);

  //osi.createGLWindow(&osi.win[2], &osi.display.monitor[1], "window 3", 400, 400, 32, 1);

  in.init(1, 1);	// mode 2= manual update() on each frame
  
  // !!!!!
  osi.flags.haveFocus= true;
  
  
  /*
  while(!osi.flags.haveFocus)
    osi.checkMSG();               // wait for window creation
*/
  #ifdef USING_DIRECTINPUT
  //in.getDIj(0)->init(3);
  in.getT2j(0)->init(3);
  in.gp[4].diDevice->Acquire();

  //in.vibrate();
  #endif /// USING_DIRECTINPUT


  #ifdef OS_LINUX
  //glViewport(0, 0, osi.lin.gwa[0].width, osi.lin.gwa[0].height);
  #endif

  while(1) {

    //in.m.update();   /// mouse update
    //in.k.update();   /// keyboard update
    //in.k.updateLocks();

    //in.j[0].update();

    //    in.update();

    if(osi.flags.exit)
      break;

    if(osi.flags.haveFocus)
      for(short a= 0; a< MAX_WINDOWS; a++)
        if(osi.win[a].isCreated) {
          if(osi.glMakeCurrent(&osi.win[a])) {
            osi.resizeGLScene(osi.win[a].dx, osi.win[a].dy);
            drawSomething();
            osi.swapBuffers(&osi.win[a]);
          }
    }
    
    if(in.k.key[in.Kv.esc] || (in.k.key[in.Kv.lalt] && in.k.key[in.Kv.f4]))
      osi.exit(0);

    #ifdef OS_WIN
    COORD pos= {0,0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    #else /// OS_LINUX & OS_MAC
    //printf("\x1b[H");      // should set cursor position to '0,0' or whatever home means
    #endif /// OS_LINUX & MAC
    /*
    ulong c= in.k.getChar();
    if(c) {
      string s(c);
      printf("%s ", s.d);
    }
    
    
    printf("mouse: %05dx %05dy %dl %dr %dm %dx1 %dx2 % dw %d %d %d %d \n", in.m.x, in.m.y, in.m.b[0].down, in.m.b[1].down, in.m.b[2].down, in.m.b[3].down, in.m.b[4].down, in.m.getWheelDu(), in.m.b[5].down, in.m.b[6].down, in.m.b[7].down, in.m.b[8].down);
    printf("last keyboard keys: %03d %03d %03d\n", in.k.lastKey[0].code, in.k.lastKey[1].code, in.k.lastKey[2].code);
    printf("nr: joysticks(%d) gamepads(%d) gamewheels(%d)\n", in.nr.jFound, in.nr.gpFound, in.nr.gwFound);
    short n= 0;

    printf("gamepad[%d] s1[% 6ld,% 6ld] s2[% 6ld,% 6ld] extra[% 6ld,% 6ld]\n", n, in.gp[n].lx, in.gp[n].ly, in.gp[n].rx, in. gp[n].ry, in.gp[n].u, in.gp[n].v);
    printf("gamepad[%d] lTrigger[% 6ld] rTrigger[% 6ld]\n", n, in.gp[n].lt, in.gp[n].rt);
    printf("gamepad[%d] b01[%d] b02[%d] b03[%d] b04[%d] b05[%d] b06[%d] b07[%d] b08[%d]\n", n, in.gp[n].b[0], in.gp[n].b[1], in.gp[n].b[2], in.gp[n].b[3], in.gp[n].b[4], in.gp[n].b[5], in.gp[n].b[6], in.gp[n].b[7]);
    printf("gamepad[%d] b09[%d] b10[%d] b11[%d] b12[%d] b13[%d] b14[%d] b15[%d] b16[%d]\n", n, in.gp[n].b[8], in.gp[n].b[9], in.gp[n].b[10], in.gp[n].b[11], in.gp[n].b[12], in.gp[n].b[13], in.gp[n].b[14], in.gp[n].b[15]);
    printf("gamepad[%d] pov[%05ld]\n", n, in.gp[n].pov);

    printf("typed char buffer[%d]:", in.k.charTyped.nrNodes);
    for(short a= 0; a< 32; a++) {
      ulong c= (in.k.charTyped.nrNodes> a)? ((Keyboard::chTyped*)in.k.charTyped.get(a))->c: L' ';
      printf("%lc", c);
      //printf("% c", (in.k.charTyped.nrNodes> a)? ((Keyboard::chTyped*)in.k.charTyped.get(a))->c: ' ');
    }
    
    printf("\n");
    //    printf("ZE QUESTION: [%d] [%d]\n", osi.display.monitor[0].glRenderer, osi.display.monitor[1].glRenderer);
    */
    
    #ifdef OS_LINUX
    /*
    for(short a= 0; a< 256; a++) {
      if(in.k.key[a])
        printf("keydown: keycode[%02d], keysym[%06lx]\n", a, XkbKeycodeToKeysym(osi.primWin->dis, a, null, 0));
    }
    */
    #endif /// OS_LINUX


    osi.checkMSG();		/// operating system messages handling
  }

  return 0;
}

void drawSomething() {
  glDrawBuffer( GL_BACK );
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1., 1., -1., 1., 1., 20.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

  glBegin(GL_QUADS);
    glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
    glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
    glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
    glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
  glEnd();

}
// ####################
// End testing purposes
// ####################



















OSInteraction::OSInteraction() {
  flags.exit= false;
  flags.haveFocus= false;
  flags.minimized= false;
  flags.buttonPress= false;
  flags.keyPress= false;

  primWin= &win[0];

  #ifdef OS_WIN
  QueryPerformanceFrequency(&timerFreq);     /// read cpu frequency. used for high performance timer (querryPerformanceBlaBla)
  #endif /// OS_WIN

  #ifdef OS_LINUX
  //  setlocale(LC_ALL, ""); // can't rely on setlocale. everything is different on each os. rely on StringClass32/8 and that's that.
//  setlocale(LC_CTYPE, "");
  primWin->dis= XOpenDisplay(null);          // DISPLAY CONNECTION TO XSERVER can be something like ":0.0" :displayNr.screenNr
  if(primWin->dis == NULL)
    error.simple("Cannot connect to X server\n", true); /// true= exit
  #endif /// OS_LINUX

  #ifdef OS_MAC
  mach_timebase_info(&machInfo);            /// read cpu frequency & stuff; used for high performance timer (mac variant, named mach)
  cocoa.setProgramPath();                   /// program path (osi.path stores the string afterwards)
  #endif /// OS_MAC
}

OSInteraction::~OSInteraction() {
  #ifdef OS_LINUX
// it seems system already destroys the display/windows and calling any of these causes segmentation error
// maybe put them in a program exit function that might be called by the program.
//
  // XSync(primWin->dis, False);
  // delData();
  // XCloseDisplay(primWin->dis);             // DISPLAY CONNECTION TERMINATION
  #endif
}

void OSInteraction::delData() {
  /// destroy every window. kinda useless...
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(win[a].isCreated) win[a].delData();
}


// -------------============== WINDOW CREATION ================----------------

// SIMPLE WINDOW CREATION FUNCS. they all call createGLWindow()

// create just a single 'primary' window on 'primary' monitor
bool OSInteraction::primaryGLWindow(string name, int dx, int dy, int8 bpp, int8 mode, short freq) {
  return createGLWindow(primWin, display.primary, name, dx, dy, bpp, mode, freq);
}

// create a fullscreen (mode 3) primary window
bool OSInteraction::primaryGLWindow() {
  win[0].mode= 3;
  win[0].name= "Primary Program Window";
  win[0].freq= 0;
  win[0].bpp= 32;

  return createGLWindow(&win[0], display.primary, win[0].name, win[0].dx, win[0].dy, win[0].bpp, win[0].mode, win[0].freq);
}

// MAIN CREATE WINDOW FUNC. has every customisation
bool OSInteraction::createGLWindow(OSIWindow *w, OSIMonitor *m, string name, int dx, int dy, int8 bpp, int8 mode, short freq) {
  string func= "OSInteraction::createGLWindow: ";
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

  #ifdef OS_WIN

  GLuint PixelFormat;   // Holds The Results After Searching For A Match
  WNDCLASS wc;          // Windows Class Structure
  DWORD dwExStyle;      // Window Extended Style
  DWORD dwStyle;        // Window Style
  RECT rect;            // Grabs Rectangle Upper Left / Lower Right Values

  /// fullscreen resolution change
  if(mode== 2) {
    if(!display.changeRes(w, m, w->dx, w->dy, w->bpp, w->freq)) {
      mode= 1;                            // if it fails, set mode to windowed <<--- ???
      w->mode= 1;
    }
    display.getMonitorPos(m);
    w->x0= m->x0;
    w->y0= m->y0;
  }

  rect.left=	 w->x0;
  rect.right=	 w->x0+ w->dx;
  rect.top=		 w->y0;
  rect.bottom= w->y0+ w->dy;

  w->hInstance = GetModuleHandle(NULL);                   // Grab An Instance For Our Window .... wonder what this means lol
  wc.style				 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc	 = (WNDPROC) processMSG;                // WndProc Handles Messages
  wc.cbClsExtra		 = 0;                                   // No Extra Window Data
  wc.cbWndExtra		 = 0;                                   // No Extra Window Data
  wc.hInstance		 = w->hInstance;                        // Set The Instance
  wc.hIcon				 = LoadIcon(NULL, IDI_WINLOGO);         // Load The Default Icon
  wc.hCursor			 = LoadCursor(NULL, IDC_ARROW);         // Load The Arrow Pointer
  wc.hbrBackground = NULL;                                // No Background Required For GL
  //wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));  // No Background Required For GL
  wc.lpszMenuName	 = NULL;                                // We Don't Want A Menu
  wc.lpszClassName = name;                                // Set The Class Name

  if (!RegisterClass(&wc)) {                        // Attempt To Register The Window Class
    error.simple(func+ "Failed to register wc");
    return false;
  }

  if(mode== 1) {                                    // windowed
    if(w == &win[0]) {                              /// if it's win[0], it's the primary window - if not it's a child window
      dwExStyle= WS_EX_APPWINDOW| WS_EX_WINDOWEDGE;
      dwStyle= WS_OVERLAPPEDWINDOW;
    } else {
      dwExStyle= WS_EX_TOOLWINDOW| WS_EX_WINDOWEDGE;
      dwStyle= WS_OVERLAPPEDWINDOW;//| WS_CHILD;
    }
  } else if(mode== 2) {                             // fullscreen
    if(w == &win[0]) {
      dwExStyle= WS_EX_APPWINDOW;
      dwStyle= WS_POPUP;
      ShowCursor(FALSE);
    } else {
      dwExStyle= 0;
      dwStyle= WS_POPUP| WS_CHILD;
      ShowCursor(FALSE);
    }
  } else if((mode== 3) || (mode== 4)) {             // fullscreen window
    if(w == &win[0]) {
      dwExStyle= WS_EX_APPWINDOW;
      dwStyle= WS_POPUP;
    }	else {
      dwExStyle= 0;
      dwStyle= WS_POPUP| WS_CHILD;
    }
  }

  AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

  // Create The Window
  if (!(w->hWnd= CreateWindowEx(dwExStyle,              // Extended Style For The Window
                name,                   // class name           <--- might want a different class name?
                name,                   // window title
                dwStyle |               // defined window style
                WS_CLIPSIBLINGS |       // Required Window Style
                WS_CLIPCHILDREN,        // Required Window Style
                w->x0, w->y0,           // Window Position
                rect.right- rect.left,  // dx
                rect.bottom- rect.top,  // dy
                win[0].hWnd,            // parent window
                NULL,                   // no menu
                w->hInstance,           // instance
                NULL)))                 // Dont Pass Anything To WM_CREATE
  {
    killGLWindow(w);                    // Reset The Display
    error.simple(func+ "Window creation error.");
    return false;
  }
  /// pixel format - MORE TESTING NEEDED HERE. screen blacks out on mode 3 - it shouldn't
  static PIXELFORMATDESCRIPTOR pfd;
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

  if (!(w->hDC= GetDC(w->hWnd))) {                  // try to get a Device Context?
    killGLWindow(w);
    error.simple(func+ "Can't create a GL DC");
    return false;
  }

  if (!(PixelFormat= ChoosePixelFormat(w->hDC, &pfd))) {  // Did Windows Find A Matching Pixel Format? I HOPE WE FUKIN DID rofl (all these checs.. some will never fail ffs)
    killGLWindow(w);
    error.simple(func+ "Can't aquire a PixelFormat");
    return false;
  }

  if(!SetPixelFormat(w->hDC, PixelFormat, &pfd)) {        // Are We Able To Set The Pixel Format?
    killGLWindow(w);
    error.simple(func+ "Can't set PixelFormat");
    return false;
  }

  if(!m->glRenderer) {      // THIS NEEDS INTENSIVE WORK&TESTING <<<<<<<<<<<<<< there must be only 1 renderer per graphics card
    if(!(m->glRenderer= wglCreateContext(w->hDC))) {      // Are We Able To Get A Rendering Context?
      killGLWindow(w);
      error.simple(func+ "Can't create GL RC");
      return false;
    }
    w->monitor= m;          /// point window's monitor to the primary monitor
  }

  w->glRenderer= m->glRenderer;

  // from help: wglMakeCurrent()
  //	All subsequent OpenGL calls made by the thread are drawn on the device identified by hdc.
  //	You can also use wglMakeCurrent to change the calling thread's current rendering context so it's no longer current.


  if(!wglMakeCurrent(w->hDC, m->glRenderer)) {          // Try To Activate The Rendering Context
    killGLWindow(w);
    error.simple(func+ "Can't activate GL RC");
    return false;
  }

  ShowWindow(w->hWnd, SW_SHOW);   /// Show The Window
  SetForegroundWindow(w->hWnd);   /// Slightly Higher Priority

  SetFocus(w->hWnd);              /// Sets Keyboard Focus To The Window

  w->isCreated= true;
  return true;
  #endif /// OS_WIN


  #ifdef OS_LINUX //                <---------------------------LINUX
  XVisualInfo *vi;
  Colormap cmap;
  XSetWindowAttributes swa;

  GLint att[]= {
                 GLX_RGBA,
                 GLX_RED_SIZE, 8,
                 GLX_GREEN_SIZE, 8,
                 GLX_BLUE_SIZE, 8,
                 GLX_ALPHA_SIZE, 8,
                 GLX_DEPTH_SIZE, 16,
                 GLX_DOUBLEBUFFER,
                 None };

  w->root= m->root;                                        // 'desktop window'
  w->dis= primWin->dis; // server connection, created in OSInteraction()

  if(mode == 2)
    if(!display.changeRes(w, m, dx, dy, bpp, freq)) {
      error.simple("osi:createGLwindow: cant change to selected resolution");
      w->dx= m->dx;
      w->dy= m->dy;
      mode= 3;
    }

  w->mode= mode;

  vi= glXChooseVisual(w->dis, m->screen, att);

  /* !!!!!!!!!!!!!!!!!!
  vi= DefaultVisual(display, 0);
  depth= DefaultDepth(display, 0);
  */


  if(vi == NULL)
    error.simple(func+ "no appropriate visual found\n", true);
  else // DELETE <--------------------------------
    printf("\n\tvisual %p selected\n", (void *)vi->visualid); // %p creates hexadecimal output like in glxinfo

  cmap= XCreateColormap(w->dis, w->root, vi->visual, AllocNone);
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

  w->win= XCreateWindow(w->dis, w->root,
                        w->x0, w->y0, w->dx, w->dy,     // position & size
                        0,                              // border size
                        vi->depth,                      // depth can be CopyFromParent
                        InputOutput,                    // InputOnly/ InputOutput/ CopyFromParent
                        vi->visual,                     // can be CopyFromParent
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
  XSetWMHints(primWin->dis, w->win, wh);
  XFree(wh);


// ^^^^^^^^ after testing, still dunno what the group does... auto it does nothing

  // _NET_WM_STATE with_NET_WM_STATE_ABOVE gain focus
  // _NET_WM_STATE with_NET_WM_STATE_BELOW lose focus

  if(w== primWin) {
    // ICON

  } else {
    /// rest of windows have no taskbar icons
    //XSetTransientForHint(primWin->dis, w->win, primWin->win); // it seems transientFor is the only thing needed (no taskbar icon)
    w->setWMstate(1, "_NET_WM_STATE_SKIP_TASKBAR");

    // ***NO ICON

    // ***NO TASKBAR
    // option 1:  _NET_WM_WINDOW_TYPE
    //   with  _NET_WM_WINDOW_TYPE_TOOLBAR or _NET_WM_WINDOW_TYPE_UTILITY or _NET_WM_WINDOW_TYPE_DIALOG
    // option 2:  _NET_WM_STATE
    //   with  _NET_WM_STATE_SKIP_TASKBAR

  }

  /// fullscreen mode linux specific msgs settings
  //XMoveWindow(w->dis, w->win, w->x0, w->y0);
  if((mode== 2) || (mode== 3))
    w->setWMstate(1, "_NET_WM_STATE_FULLSCREEN");
  if(mode== 4) {
    Atom fullmons = XInternAtom(w->dis, "_NET_WM_FULLSCREEN_MONITORS", False);
    XEvent xev;
    for(short a= 0; a< sizeof(xev); a++) ((char*)(&xev))[a]= 0;
    xev.type = ClientMessage;
    xev.xclient.window= w->win;
    xev.xclient.message_type = fullmons;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 0; // your topmost monitor number
    xev.xclient.data.l[1] = 0; // bottommost
    xev.xclient.data.l[2] = 0; // leftmost
    xev.xclient.data.l[3] = 0; // rightmost
    xev.xclient.data.l[4] = 0; // source indication
    
    WIP
      
    XSendEvent (w->dis, DefaultRootWindow(w->dis), False,
                    SubstructureRedirectMask | SubstructureNotifyMask, &xev);
  }
  
  w->setWMstate(1, "_NET_WM_STATE_ABOVE");

  XMapWindow(w->dis, w->win);

/* ----------================IMPORTANT===================---------------------
Update They say for multihead to work, you need to use _NET_WM_FULLSCREEN_MONITORS
property (see here). It's an array of 4 integers that should be set like this:

    Atom fullmons = XInternAtom(dis, "_NET_WM_FULLSCREEN_MONITORS", False);
    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = win;
    xev.xclient.message_type = fullmons;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 0; // your topmost monitor number
    xev.xclient.data.l[1] = 0; // bottommost
    xev.xclient.data.l[2] = 0; // leftmost
    xev.xclient.data.l[3] = 1; // rightmost
    xev.xclient.data.l[4] = 0; // source indication

    XSendEvent (dis, DefaultRootWindow(dis), False,
                    SubstructureRedirectMask | SubstructureNotifyMask, &xev);

With this, you should be able to set your fullscreen windows to occupy
a single monitor, the entire desktop, or (for more than 2 monitors) anything in between.

I have not checked this because I don't have a multihead system.
*/ //-----------===============IMPORTANT====================------------------

  XStoreName(w->dis, w->win, name);


  // NOTE: this func has shareLists in its params !!!!!!!!!!!!!!!!!!!!!
  if(win== primWin)
    w->glRenderer= glXCreateContext(w->dis, vi, NULL, GL_TRUE);
  else
    w->glRenderer= win[0].glRenderer;
  // SAME RENDERER!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ATM



/* The last parameter decides if direct rendering is enabled. If you want
   to send the graphical output via network, you have to set it to GL_FALSE.
   If your application puts its output to the computer you are sitting in
   front of, use GL_TRUE. Note that some capabilities like vertex buffer objects
   can only be used with a direct gl context (GL_TRUE). */

  glMakeCurrent(w);    // osi func

  glEnable(GL_DEPTH_TEST);

  ///  handle the close button WM
  if(w== primWin) {
    Atom wmDelete= XInternAtom(w->dis, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(w->dis, w->win, &wmDelete, 1);
  }

  w->monitor= m;
  w->isCreated= true;
  return true;

  #endif /// OS_LINUX

  #ifdef OS_MAC // <<<<<<<<<<<<<< MAC >>>>>>>>>>>>>>>
  
  /// window creation is in OScocoa.mm due to Abjective-C
  
  return cocoa.createWindow(w);  /// all window vars are set, just create the window.
  #endif /// OS_MAC

/// if program reached this point, there's no OS defined
  error.simple(func+ "no OS specified?");
  return false;
} // OSInteraction::createGLWindow END <<<







// -------------------============ GLWINDOW DELETION ==============-------------
bool OSInteraction::killPrimaryGLWindow() {
  return killGLWindow(&win[0]);
}


bool OSInteraction::killGLWindow(OSIWindow *w) {
  #ifdef OS_WIN
  if (w->mode== 2)
    display.restorePrimary();

  w->delData();

  return true;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  w->delData();
  return true;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  w->delData();
  return true;
  #endif

  return false;
} /// OSInteraction::killGLWindow






///--------------------------------------///
// OPENGL functions that are OS dependant //
///--------------------------------------///

// SWAP BUFFERS
void OSInteraction::swapPrimaryBuffers() {
  swapBuffers(&win[0]);
}


void OSInteraction::swapBuffers(OSIWindow *w) {
  #ifdef OS_WIN
  //SwapBuffers(w->hDC);      /// standard; the wgl one has more possibilities
  wglSwapLayerBuffers(w->hDC, WGL_SWAP_MAIN_PLANE);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  glXSwapBuffers(w->dis, w->win);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  cocoa.swapBuffers(w);
  #endif /// OS_MAC
} // OSInteraction::swapBuffers



bool OSInteraction::glMakeCurrent(OSIWindow *w) {
  #ifdef OS_WIN
  if(w)
    return wglMakeCurrent(w->hDC, w->glRenderer);
  else
    return wglMakeCurrent(null, null);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(w)
    return glXMakeCurrent(w->dis, w->win, w->glRenderer);
  else
    return true; //glXMakeCurrent(primWin->dis, None, NULL);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  if(w)
    cocoa.makeCurrent(w);
  return true;
  #endif /// OS_MAC
  
  return false;
} // OSInteraction::glMakeCurrent


bool OSInteraction::glCreateRenderer(OSIWindow *w) {
  return false;
//  glXCreateContext(w->display, w->
  // visual info!!!!
} // OSInteraction::glCreateRenderer


bool OSInteraction::glDestroyRenderer(OSIWindow *w) {
  #ifdef OS_WIN
  wglDeleteContext(w->glRenderer);
  return true;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  glXDestroyContext(w->dis, w->glRenderer);
  return true;
  #endif ///OS_LINUX

  #ifdef OS_MAC
  return true;  // nothing to 'destroy' in mac, it seems. renderers are always active i think.
  #endif /// OS_MAC

  return false;
} // OSInteraction::glDestroyRenderer










#ifdef OS_WIN
string OSInteraction::getWinName(HWND h) {
  for(int a= 0; a< MAX_WINDOWS; a++)
    if(win[a].hWnd==h)
      return win[a].name;
  return "unknown window";
}

OSIWindow *OSInteraction::getWin(HWND h) {
  for(int a= 0; a< MAX_WINDOWS; a++)
    if(win[a].hWnd==h)
      return &win[a];
  return null;
}
#endif /// OS_WIN

#ifdef OS_LINUX
OSIWindow *OSInteraction::getWin(Window *w) {
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(win[a].win== *w)
      return &win[a];
  return null;
}
#endif

#ifdef OS_MAC
OSIWindow *OSInteraction::getWin(void *w) {
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(win[a].win== w)
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
// there is no resize/move for windows, the close button wont work either, i think
///===================================================

  bool chatty= false;	// if used, prints msgs to terminal
  bool onlyHandled= true; /// used with chatty
  int mb= 0;

// mouse in [MODE 2], THE WHEEL IS NOT POSSIBLE TO READ with funcs, so events are the only way (decent way, anyways)
  if((in.m.mode== 2)&& osi.flags.haveFocus)
    if(m== WM_MOUSEWHEEL) {
      in.m.wheel+= (GET_WHEEL_DELTA_WPARAM(wParam)> 0)? 1: -1;
      if(chatty) printf("mouse: wheel rotated\n");
      goto ret;
    }


// mouse vals are handled here, in case mouse is set in [MODE 1]
  if((in.m.mode== 1)&& osi.flags.haveFocus)
    switch(m) {
      case WM_MOUSEMOVE: {                                        /// mouse movement
        in.m.oldx= in.m.x;
        in.m.oldy= in.m.y;
        in.m.x= ((int)(short)LOWORD(lParam));
        in.m.y= ((int)(short)HIWORD(lParam));
        if(in.m.useDelta) {                    ///in case mouse is set to use delta move values, not exact screen coords
          in.m.dx+= in.m.x- in.m.oldx;
          in.m.dy+= in.m.y- in.m.oldy;
        }
        goto ret;
        //return 0; // it is faster, but no windows move/resize!!!
      } case WM_SETCURSOR: { goto ret;                            /// to be or not to be

    // mouse buttons in MODE 1
      } case WM_LBUTTONDOWN: {                                    /// left mouse button
        osi.getMillisecs(&in.m.b[0].timeStart);
        in.m.b[0].down= true;
        if(chatty) printf("mouse: l button pressed\n");
        goto ret;
        //return 0;
      } case WM_LBUTTONUP: {
        in.m.b[0].lastTimeStart= in.m.b[0].timeStart;
        osi.getMillisecs(&in.m.b[0].lastTimeEnded);
        in.m.b[0].lastDT= in.m.b[0].lastTimeEnded- in.m.b[0].lastTimeStart;
        in.m.b[0].down= false;
        if(chatty) printf("mouse: l button released\n");
        goto ret;
        //return 0;
      } case WM_RBUTTONDOWN: {                                    /// right mouse button
        osi.getMillisecs(&in.m.b[1].timeStart);
        in.m.b[1].down= true;
        if(chatty) printf("mouse: r button pressed\n");
        goto ret;
        //return 0;
      } case WM_RBUTTONUP: {
        in.m.b[1].lastTimeStart= in.m.b[1].timeStart;
        osi.getMillisecs(&in.m.b[1].lastTimeEnded);
        in.m.b[1].lastDT= in.m.b[1].lastTimeEnded- in.m.b[1].lastTimeStart;
        in.m.b[1].down= false;
        if(chatty) printf("mouse: r button released\n");
        goto ret;
        //return 0;
      } case WM_MBUTTONDOWN: {                                    /// middle mouse button
        osi.getMillisecs(&in.m.b[2].timeStart);
        in.m.b[2].down= true;
        if(chatty) printf("mouse: m button pressed\n");
        goto ret;
        //return 0;
      } case WM_MBUTTONUP: {
        in.m.b[2].lastTimeStart= in.m.b[2].timeStart;
        osi.getMillisecs(&in.m.b[2].lastTimeEnded);
        in.m.b[2].lastDT= in.m.b[2].lastTimeEnded- in.m.b[2].lastTimeStart;
        in.m.b[2].down= false;
        if(chatty) printf("mouse: m button released\n");
        goto ret;
        //return 0;
      } case WM_MOUSEWHEEL: {                                     /// wheel
        in.m.wheel+= (GET_WHEEL_DELTA_WPARAM(wParam)> 0)? 1: -1;
        if(chatty) printf("mouse: wheel rotated\n");
        goto ret;
        //return 0;
      } case WM_XBUTTONDOWN: {                                    /// buttons 4 & 5
        if(GET_XBUTTON_WPARAM(wParam)== XBUTTON1) {
          osi.getMillisecs(&in.m.b[3].timeStart);
          in.m.b[3].down= true;
          if(chatty) printf("mouse: button 4 pressed\n");
          goto ret;
          //return 0;
        }
        if(GET_XBUTTON_WPARAM(wParam)== XBUTTON2) {
          osi.getMillisecs(&in.m.b[4].timeStart);
          in.m.b[4].down= true;
          if(chatty) printf("mouse: button 5 pressed\n");
          goto ret;
          //return 0;
        }
        break;
      } case WM_XBUTTONUP: {
        if(GET_XBUTTON_WPARAM(wParam)== XBUTTON1) {
          in.m.b[3].lastTimeStart= in.m.b[3].timeStart;
          in.m.b[3].lastTimeEnded= osi.eventTime;
          osi.getMillisecs(&in.m.b[3].lastTimeEnded);
          in.m.b[3].down= false;
          if(chatty) printf("mouse: button 4 released\n");
          goto ret;
          //return 0;
        }
        if(GET_XBUTTON_WPARAM(wParam)== XBUTTON2) {
          in.m.b[4].lastTimeStart= in.m.b[4].timeStart;
          osi.getMillisecs(&in.m.b[4].lastTimeEnded);
          in.m.b[4].lastDT= in.m.b[4].lastTimeEnded- in.m.b[4].lastTimeStart;
          in.m.b[4].down= false;
          if(chatty) printf("mouse: button 5 released\n");
          goto ret;
          //return 0;
        }
        break;
      }
    };


  // keyboard messages [MODE 1]
  if(in.k.mode== 1) {
    switch(m) {
      case WM_KEYDOWN:                                          // ***key PRESS***
      case WM_SYSKEYDOWN: {
        in.k.updateLocks();
        getMillisecs(&osi.eventTime);            /// using getTIMEXXX() funcs: can't rely on event time sent from system

        //int code= GETBYTE2UINT32(lParam);
        uint code= (uint)wParam;
        Keyboard::KeyPressed k;

        /// left/ right ALT/CONTROL/SHIFT distingush
        if(wParam== VK_SHIFT)   code= (GetKeyState(VK_RSHIFT)& 0x80)?   VK_RSHIFT:    VK_LSHIFT;
        if(wParam== VK_CONTROL) code= (GetKeyState(VK_RCONTROL)& 0x80)? VK_RCONTROL:  VK_LCONTROL;
        if(wParam== VK_MENU)    code= (GetKeyState(VK_RMENU)& 0x80)?    VK_RMENU:     VK_LMENU;

        if(chatty) printf("key PRESS code=%d \n", code);
        
        /// check if message is a <repeat key press>
        if(KF_REPEAT& HIWORD(lParam)) {
          if(!in.k.key[code]) {    /// in case shit happened (alt tab mess, some crappy windows message block dunno)
            /// log the key
            k.code= code;
            k.checked= false;
            k.timeDown= osi.eventTime;
            in.k.log(k);
            /// set the vars
            in.k.key[code]= 128;
            in.k.keyTime[code]= osi.eventTime;
            //in.k.repeat[code]= 1;    /// with the next repeat increase (next line), the key will be logged as repeated twice, wich is ok
          }
          //in.k.repeat[code]+= KF_REPEAT& HIWORD(lParam);   // THIS MIGHT BE DELETED <-------------------------------------------
          
          
          // here must be placed in.k.addManip();
          
          
          goto ret;
          //return 0;
        }
        /// log the key
        k.code= code;
        k.checked= false;
        k.timeDown= osi.eventTime;
        in.k.log(k);
        /// set the key as pressed & other needed vars
        in.k.key[code]= 128;
        in.k.keyTime[code]= osi.eventTime;
        //in.k.repeat[code]= 1;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED
        
        
        //        in.k.addManip(); here<<<<<<<<<<
        
        
        goto ret;
        //return 0;
      }
      case WM_KEYUP:                                        // ***key DEPRESS***
      case WM_SYSKEYUP: {
        in.k.updateLocks();
        getMillisecs(&osi.eventTime);            /// using getTIMEXXX() funcs: can't rely on event time sent from system
        //int code= GETBYTE2UINT32(lParam);
        uint code= (uint)wParam;
        if(wParam== VK_SHIFT)   code= in.k.key[VK_RSHIFT]?   VK_RSHIFT:   VK_LSHIFT;
        if(wParam== VK_CONTROL) code= in.k.key[VK_RCONTROL]? VK_RCONTROL:	VK_LCONTROL;
        if(wParam== VK_MENU)    code= in.k.key[VK_RMENU]?		 VK_RMENU:    VK_LMENU;

        if(chatty) printf("key RELEASE code=%d\n", code);

        /// log the key in history
        bool found= false;
        for(short a= 0; a< MAX_KEYS_LOGGED; a++)
          if(in.k.lastKey[a].code== code) {
            in.k.lastKey[a].timeUp= osi.eventTime;
            in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
            found= true;
            break;
          }
        /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
        if(!found)  {
          Keyboard::KeyPressed k;
          k.code= code;
          k.checked= false;
          k.timeDown= osi.eventTime- 1;   /// 1 milisecond before the keyup
          k.timeUp= osi.eventTime;
          k.timeDT= 1;                    /// timeUp- timeDown
          in.k.log(k);
        }

        /// set the key as pressed & other vars
        in.k.key[code]= 0;
        in.k.keyTime[code]= 0;
        // in.k.repeat[code]= 0;  // repeat[] is subject for deletion

        goto ret;
        //return 0;
      }
    }
  }
  if(in.k.mode== 2) {
    switch(m) {
      case WM_KEYDOWN:                    // key PRESS
      case WM_SYSKEYDOWN:
      case WM_KEYUP:                      // key PRESS
      case WM_SYSKEYUP: {
        in.k.updateLocks();               /// to be or not to be...  this should be just empty code here...
        in.k.update();
        if(chatty) printf("key update");
        goto ret;
      }
    }
  }


  /// ignore some calls to defwindowproc for speed - PROBLEM IS, no window MOVE/RESIZE, probly no close button either

/*
  if((m>= WM_MOUSEFIRST)&& (m<= WM_MOUSELAST))
    return 0;
  if((m>= WM_KEYFIRST)&& (m<= WM_KEYLAST))
    return 0;
*/


  // handled windows messages
  switch(m) {
    case WM_ACTIVATE: {
      if(wParam == 0) {
        osi.flags.haveFocus= false;
        // in case of alt-tab all current pressed buttons must be reset !!!!
        in.resetPressedButtons();
        in.m.unaquire();       /// direct input mouse unAquire <--- grab/aquire/blabla might be needed for other modes/ human input devices
        in.k.unaquire();
        
        // further unaquires must be placed here <<<<<<<<<<<<<<<<<<<<<<<<<

        //in.gp[4].unaquire();
        
        
      } else {
        osi.flags.haveFocus= true;
        in.m.aquire();         /// direct input mouse aquire
        in.k.aquire();
        // further aquires must be placed here <<<<<<<<<<<<<<<<<<<<<<<<<
        
        //in.gp[4].aquire();
      }
      if(chatty) printf("WM_ACTIVATE %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam);
      //goto ret;
      return 0;

    } case WM_CLOSE: {
      if(chatty) printf("WM_CLOSE %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam);
      osi.flags.exit= true;     /// main exit flag
      return 0;
    } case WM_CHAR: {
      if(chatty) printf("\n\n\n\n\n\nWM_CHAR %s %c\n", osi.getWinName(hWnd), wParam);
      osi.getMillisecs(&osi.eventTime);
      in.k.addChar(wParam, &osi.eventTime);
      return 0;
    } case WM_UNICHAR: {
      error.simple("WM_UNICHAR not tested");
      if(chatty) printf("\n\n\n\n\n\nWM_UNICHAR %s %c\n", osi.getWinName(hWnd), wParam);
      osi.getMillisecs(&osi.eventTime);
      in.k.addChar(wParam, &osi.eventTime);
      return 0;

/// system commands
    } case WM_SYSCOMMAND: {
      if(chatty) printf("WM_SYSCOMMAND %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam);

      switch (wParam)	{
        case SC_CLOSE: {
          if(chatty) printf("  SC_CLOSE %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam);
          osi.flags.exit= true;
          return 0;
        }
      }
    goto ret;
    //return 0;
    }
  }

/// unhandled frequent windows messages
  if(chatty&& !onlyHandled)
    switch(m) {
      case WM_ACTIVATEAPP: { printf("WM_ACTIVATEAPP %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam); goto ret;   /// lower level aplication focus, the WM_ACTIVATE one, i think is better, cuz is the last one that is sent
      } case WM_ERASEBKGND: { printf("WM_ERASEBKGND %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam); goto ret;
      } case WM_PAINT: { printf("WM_PAINT %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam); goto ret;
      } case WM_NCPAINT: { printf("WM_NCPAINT %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam); goto ret;
      } case WM_SETFOCUS: { printf("WM_SETFOCUS %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam); goto ret;       /// keyboard focus i think
      } case WM_KILLFOCUS: { printf("WM_KILLFOCUS %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam); goto ret;     /// keyboard focus i think
      } case WM_NCACTIVATE: { printf("WM_NCACTIVATE %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam); goto ret;
      } case WM_GETICON: { goto ret;              /// usually is used when alt-tabbing, gets an icon for the mini alt-tab list
        // WHEN dealing with icons, must remember to develop WM_GETICON too
      } case WM_IME_NOTIFY: {	goto ret;
      } case WM_IME_SETCONTEXT: {	goto ret;
      } case WM_NCHITTEST: { goto ret;            /// something to do with mouse placement
      } case WM_NCMOUSEMOVE: { goto ret;          /// non client area mouse coords (top title/ moving bar, is a non-client for example)
      } case WM_NCMOUSELEAVE: { goto ret;         /// non client area mouse leaving window
      }
    };

  if(chatty&& !onlyHandled)
    printf("UNKNOWN %s 0x%x %d %d\n", osi.getWinName(hWnd), m, wParam, lParam);
  /// this DefWindowProc() handles window movement & resize & rest... without this, moving is not working, for example
ret:
  return DefWindowProc(hWnd, m, wParam, lParam);
}

#endif /// OS_WIN



#ifdef OS_LINUX
void OSInteraction::processMSG()  {
  bool chatty= true;      // used for debugs, prints stuff in every message

  XEvent event;
  OSIWindow *w= null;

  while(XPending(primWin->dis)) {       /// while there are messages in queue, loop thru them
    XNextEvent(primWin->dis, &event);
    
// ########################### MOUSE EVENTS ################################# //
      
// ---------------============ MOUSE MOVEMENT ===============-------------------
    if(event.type == MotionNotify) { /// this is the first event handled, because it is spammed
      in.m.oldx= in.m.x;
      in.m.oldy= in.m.y;
      in.m.x= event.xmotion.x_root;
      in.m.y= event.xmotion.y_root;
      if(in.m.useDelta) {
        in.m.dx+= in.m.x- in.m.oldx;
        in.m.dy+= in.m.y- in.m.oldy;
      }
      continue;

// ---------------============ BUTTON PRESS =================-------------------
    } else if(event.type == ButtonPress) {
      flags.buttonPress= true;
      eventTime= event.xbutton.time;          /// compatible with getMillisecs()

      if(event.xbutton.button== 4) {          // wheel up
        in.m.wheel+= 1;
        continue;
      }
      if(event.xbutton.button== 5) {          // wheel down
        in.m.wheel-= 1;
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
      XkbLookupKeySym(primWin->dis, code, event.xkey.state, &mods, &ks);
      
      /* THERE ARE MODS THAT I CAN'T FIND WHAT THEY DO
      if(mods)
        error.console( (string8("").f("there are still mods not applied to key %d\n", code)).d)
      */

      /// if the keysym can be a character, update the keyboard char stream
      ulong ch;
      in.getUnicode(&ks, &ch);
      if(ch)
        in.k.addChar(ch, &eventTime);
      
      /// if this is a real key press, log it and set vars
      if(!repeat) {
        if(chatty) printf("key PRESS code=%d \n", code);
        Keyboard::KeyPressed k;

        /// log the key
        k.code= code;
        k.checked= false;
        k.timeDown= eventTime;
        in.k.log(k);
        /// set the key as pressed & other needed vars
        in.k.key[code]= 128;
        in.k.keyTime[code]= eventTime;
        // in.k.repeat[code]= 128;        /// a new key press, sets repeat to 128  // MIGHT BE DELETED
      } /// if it's not a repeat
      
      in.k.updateLocks();                 /// update keyboard leds (repeat or not)
      in.k.doManip();                     /// check & handle if pressed keys form a manip char
      
      continue;
      
// ---------------=============== KEY RELEASE ===============-------------------
    } else if(event.type == KeyRelease) {
      if(in.k.mode!= 1) continue;             /// only keyboard in [mode 1]
      
      /// check if this is a repeat message (xlib sends key releases for repeats too...)
      if(XPending(primWin->dis)) {
        XEvent next;
        XPeekEvent(primWin->dis, &next);
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
          in.k.lastKey[a].timeUp= eventTime;
          in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
          found= true;
          break;
        }
      
      /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
      if(!found)  {
        Keyboard::KeyPressed k;
        k.code= code;
        k.checked= false;
        k.timeUp= eventTime;
        k.timeDown= k.timeUp- 1;              /// 1 milisecond before the keyup
        k.timeDT= 1;                          /// timeUp- timeDown
        in.k.log(k);
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

      if(w->isMapped)
        XGetWindowAttributes(w->dis, w->win, &w->gwa); // update gwa window size / attributes

      continue;
//    } else if(event.type == NoExpose) {
//      continue;
            
// ---------------=========== MAP NOTIFY EVENT ==============-------------------
    } else if(event.type == MapNotify) {

      // IS ANYTHING HERE GOOD FOR ANYTHING????????????????
      // SUBJECT OF DELETION

      w= getWin(&event.xmap.event);
      w->isMapped= true;
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
      w->isMapped= false;
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
                display.changeRes(&win[a], win[a].monitor, win[a].dx, win[a].dy, win[a].bpp, win[a].freq);
              }
              /// set the window 'on top' of everything
              win[a].setWMstate(1, "_NET_WM_STATE_ABOVE");
              win[a].setWMstate(0, "_NET_WM_STATE_BELOW");
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
        if(XPending(primWin->dis)) {
          XEvent tmp;
          XPeekEvent(primWin->dis, &tmp);
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
              win[a].setWMstate(0, "_NET_WM_STATE_ABOVE");
              win[a].setWMstate(1, "_NET_WM_STATE_BELOW");
              // setFullScreen(&win[a], false); // THIS IS A POSIBILITY

              /// [mode 2] resolution change & window iconification
              if(win[a].mode== 2) {
                if(chatty) printf("Changing to original resolution for window [%d]\n", a);
                XIconifyWindow(win[0].dis, win[a].win, win[a].monitor->screen);
                display.restoreRes(&win[a], win[a].monitor);
                flags.minimized= true;
              }
            } /// if window is created
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
    }

    //} else
    //  XFlush(win[0].display); // why flush msgs? pass thru all, right?
  
  } /// while there are messages in queue
  
} // OSInteraction::processMSG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif /// OS_LINUX


#ifdef OS_MAC
// MAC variant of processMSG is in OScocoa.mm. there is Objective-C specific stuff
#endif /// OS_MAC



bool OSInteraction::checkMSG() {
  bool ret= false;

  #ifdef OS_WIN
  while(1)    // loop thru ALL msgs... i used to peek thru only 1 msg, that was baaad... biig LAG
    if(PeekMessage(&win[0].msg, NULL, 0, 0, PM_REMOVE)) {	// Is There A Message Waiting?
      //      eventTime= win[0].msg.time;
      TranslateMessage(&win[0].msg);
      DispatchMessage(&win[0].msg);

      ret= true;
    } else
      break;

  /// i think processMSG is called by WindProc() when DispatchMessage() is called (hopefully !)
  #endif /// OS_WIN

  #ifdef OS_LINUX
  processMSG();
  ret= true;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  processMSG();
  ret= true;
  #endif /// OS_MAC

  return ret;
} /// OSInteraction::checkMSG









// -----------============= WINDOW CLASS =============------------------
///=====================================================================
#ifdef OS_WIN
MSG OSIWindow::msg= {0};
#endif

#ifdef OS_LINUX
Display *OSIWindow::dis= null;
#endif

OSIWindow::OSIWindow() {
  isCreated= false;
  x0= y0= dx= dy= 0;
  freq= bpp= 0;

  monitor= null;

  #ifdef OS_WIN
  hDC= NULL;
  hInstance= NULL;
  hWnd= NULL;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  root= 0;              /// root window (desktop/server main window/ watever)
  win= 0;               /// window 'handle' or watever (per monitor)

  glRenderer= 0;        /// openGL renderer
  isMapped= false;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  win= null;
  view= null;
  glRenderer= null;   // n/u
  #endif /// OS_MAC
}

OSIWindow::~OSIWindow() {
  #ifdef OS_WIN
  delData();     // it seems system already destroys windows, and this causes segmentation error
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  //  delData();     // it seems system already destroys windows, and this causes segmentation error  
  #endif
  
  #ifdef OS_MAC
  // nothing to do atm
  #endif /// OS_MAC
}

void OSIWindow::delData() {
  #ifdef OS_WIN
  if(hDC) {
    if(!ReleaseDC(hWnd, hDC))
      error.simple("OSIWindow::delData: Release Device Context Failed.");
    hDC= NULL;
  }

  if(hWnd) {
    if(!DestroyWindow(hWnd))
      error.simple("OSIWindow::delData: Could Not Release hWnd.");
    hWnd= NULL;
  }

  if(name.len) {
    if(!UnregisterClass(name, hInstance))
      error.simple("OSIWindow::delData: Could Not Unregister Class.");
    hInstance= NULL;
    name.delData();
  }
  // do not return;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(isCreated) {
    osi.glMakeCurrent(null);
    //osi.glDestroyRenderer(this); // THIS NEEDS TO BE PUT WITH glRenderer STRUCT WIP
    //glXDestroyContext(display, glRenderer);

    if(mode == 2)
      osi.display.restoreRes(this, monitor);

    XDestroyWindow(dis, win);
    this->isMapped= false;
  }
  // do not return
  #endif

  #ifdef OS_MAC
  win= null;
  view= null;

  // do not return
  #endif


  this->isCreated= false;
  this->mode= 0;
  this->bpp= 0;
  this->dx= 0;
  this->dy= 0;
  this->freq= 0;
  this->glRenderer= 0;
  this->monitor= null;
  this->name.delData();
  this->x0= 0;
  this->y0= 0;
}



#ifdef OS_LINUX

void OSIWindow::setWMprop(string8 wmID, string8 wmProp, uint val1, uint val2) {
  Atom wm= XInternAtom(dis, wmID, False);
  Atom prop= XInternAtom(dis, wmProp, False);
  ulong data[5]= {val1, val2, 0, 0, 0};

  XChangeProperty(dis, win, wm, prop, 32, PropModeReplace, (cuchar*)data, 2);
}


void OSIWindow::setWMstate(uint val, string8 prop1, string8 prop2) {
  XEvent xev;
  for(short a= 0; a< sizeof(xev); a++) ((char*)&xev)[a]= 0;               /// clear xev
/// set vals
  xev.type= ClientMessage;
  xev.xclient.window= win;
  xev.xclient.message_type= XInternAtom(dis, "_NET_WM_STATE", False);
  xev.xclient.format= 32;
  xev.xclient.data.l[0]= val;
  xev.xclient.data.l[1]= XInternAtom(dis, prop1, False);
  if(prop2.d)
    xev.xclient.data.l[2]= XInternAtom(dis, prop2, False);
  xev.xclient.data.l[3]= 1; // 0= oldApp/ignore, 1= normal app, 2= pagers/other clients
/// send the message to root
  XSendEvent(dis, root, False, SubstructureRedirectMask| SubstructureNotifyMask, &xev);
}


/* TO BE DELETED <----------------------------------------------------------------------------------------------
 *
void OSIWindow::setFullscreen(bool b) {
/// it seems XChangeProperty is not enough??? maybe these msgs are new, not in the standard (DIDN'T TRY THO)

  Atom wm_state= XInternAtom(display, "_NET_WM_STATE", False);
  Atom fullscreen= XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

  XEvent xev;
  for(short a= 0; a< sizeof(xev); a++) ((char*)&xev)[a]= 0;
  xev.type= ClientMessage;
  xev.xclient.window= win;
  xev.xclient.message_type= wm_state;
  xev.xclient.format= 32;
  xev.xclient.data.l[0]= b? 1: 0;
  xev.xclient.data.l[1]= fullscreen;
  xev.xclient.data.l[2]= 0;
  XSendEvent(display, root, False, SubstructureRedirectMask| SubstructureNotifyMask, &xev);
}
*/

#endif /// OS_LINUX




// END OSIWindow class //
///------=======------///








void OSInteraction::getNanosecs(uint64 *out) {
  #ifdef OS_WIN
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  *out= t.QuadPart/ (timerFreq.QuadPart/ 1000000000);
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


void OSInteraction::getMicrosecs(uint64 *out) {
  #ifdef OS_WIN
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  *out= t.QuadPart/ (timerFreq.QuadPart/ 1000000);
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


void OSInteraction::getMillisecs(uint64 *out) {
  #ifdef OS_WIN
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  *out= t.QuadPart/ (timerFreq.QuadPart/ 1000);
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

void OSInteraction::exit(int retVal) {
  display.restoreAllRes();
  #ifdef OS_WIN
  exit(retVal);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  _exit(retVal);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  _exit(retVal);
  #endif /// OS_MAC
}










// NOT OS DEPENDANT pure gl stuff <----------------------------------------------------------
// ------------------------------------------------------------------------------------------

bool OSInteraction::resizeGLScene(GLsizei dx, GLsizei dy) {
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






