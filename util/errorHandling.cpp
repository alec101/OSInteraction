

#ifdef _WIN32
#define OS_WIN
#define USING_DIRECTINPUT
#include <windows.h>

//#define USING_DIRECTINPUT     // << DISABLE / ENABLE


#ifdef USING_DIRECTINPUT
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#endif
#endif /// OS_WIN

#ifdef __linux__
#define OS_LINUX
#include <X11/Xlib.h>
#endif /// OS_LINUX

#ifdef __APPLE__
#define OS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif /// OS_APPLE

#define USING_OPENGL
#ifdef USING_OPENGL
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else /// OS_WIN + OS_LINUX
#include <GL/gl.h>
#include <GL/glu.h>
#endif 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "typeShortcuts.h"
#include "str8.h"
#include "errorHandling.h"


#ifdef USING_CONSOLE          // project defined
#include "Console.h"
extern Console console;
#endif


ErrorHandling::ErrorHandling() {
  useConsoleFlag= false;
  useWindowsFlag= true;
}

ErrorHandling::~ErrorHandling() {
  delData();
}

void ErrorHandling::delData() {
}


void ErrorHandling::simple(cchar *txt, bool exit, void (*exitFunc)(void)) {

// print to ConsoleClass
  if(useConsoleFlag) {
    console(txt, false);          /// if the error is critical, it will exit after it prints to terminal/window
    if(!exit)
      return;
  }

// use of a window
  if(useWindowsFlag) {
    if(exit || !useConsoleFlag)   /// if the ConsoleClass was used, it wont create a window unless the error is critical
      window(txt, exit, exitFunc);
    return;                       /// avoid printing to terminal
  }

// print to OS console/ terminal/ watever
  terminal(txt, exit, exitFunc);
}


void ErrorHandling::console(cchar *txt, bool exit, void (*exitFunc)(void)) {
#ifdef USING_CONSOLE
  console.print(txt);
  if(exit) {
    if(exitFunc)
      (*exitFunc)();
    ::exit(EXIT_FAILURE);
  }
  return;
#endif /// USING_CONSOLE
  printf("ConsoleClass not loaded, mate...\n");
  terminal(txt, exit, exitFunc);
}


void ErrorHandling::window(cchar *txt, bool exit, void (*exitFunc)(void)) {
  #ifdef OS_WIN
  MessageBox(null, txt, "PROGRAM ERROR", MB_OK);
  if(exit) {
    if(exitFunc)
      (*exitFunc)();
    ::exit(EXIT_FAILURE);
    //::PostQuitMessage(1); // ??????????
  }
  return;
  #endif /// OS_WIN


  #ifdef OS_LINUX
  messageBox(txt);
  if(exit) {
    if(exitFunc)
      (*exitFunc)();
    ::exit(EXIT_FAILURE);
  }

  return;
  #endif /// OS_LINUX


  #ifdef OS_MAC
  //CFStringRef s;
  CFStringRef s= CFStringCreateWithCString(null, txt, kCFStringEncodingUTF8);
  CFUserNotificationDisplayNotice(0, kCFUserNotificationPlainAlertLevel, NULL, NULL, NULL, CFSTR("PROGRAM ERROR"), s, CFSTR("OK"));
  CFRelease(s);
  
  if(exit) {
    if(exitFunc)
      (*exitFunc)();
    _exit(EXIT_FAILURE);
  }
  return;
  #endif ///OS_MAC

  printf("no OS defined\n");
  terminal(txt, exit, exitFunc);
}


void ErrorHandling::terminal(cchar *txt, bool exit, void (*exitFunc)(void)) {
  printf("%s\n", txt);
  if(exit) {
    if(exitFunc)
      (*exitFunc)();
    ::exit(EXIT_FAILURE);
  }
}


// linux message box window
#ifdef OS_LINUX
void ErrorHandling::messageBox(cchar *text) {
  Display *display;
  Visual *visual;
  int depth, x0, y0, dx, dy;

  Window win;
  XWindowAttributes desktop;  /// screen dimensions

  XSetWindowAttributes winAttr;

  XGCValues grValues;
  GC grContext;
  XEvent event;

  XFontStruct *font;
  XCharStruct tStr;
  int tx, ty, tDir, tAscent, tDescent;

// text position
  tx= 10;
  ty= 25;

// x window inits
  display= XOpenDisplay(NULL);
  visual= DefaultVisual(display, 0);
  depth= DefaultDepth(display, 0);

  winAttr.background_pixel= XWhitePixel(display, 0);

  font= XLoadQueryFont(display, "10x20");
  XGetWindowAttributes(display, RootWindow(display, 0), &desktop);
  XTextExtents(font, text, Str::strlen8(text), &tDir, &tAscent, &tDescent, &tStr);

// window size & position computations based on text & desktop size
  dx= tStr.width+ 20;
  dy= tStr.ascent+ tStr.descent+ 50;
  x0= (desktop.width- dx)/ 2;
  y0= (desktop.height- dy)/ 2;

// create the window
  win= XCreateWindow(display, XRootWindow(display, 0), 0, 0, dx, dy, 5, depth,
                               InputOutput, visual, CWBackPixel, &winAttr);
  XStoreName(display, win, "ERROR");
  XSelectInput(display, win, ExposureMask| StructureNotifyMask| SubstructureNotifyMask| KeyPressMask);

  grValues.font= font->fid;
  grValues.foreground= XBlackPixel(display, 0);
  grContext= XCreateGC(display, win, GCFont+ GCForeground, &grValues);

// this is very important - intercept the CLOSE BUTTON press
  Atom wmDelete= XInternAtom(display, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(display, win, &wmDelete, 1);

// show & move window to center
  XMapWindow(display, win);
  XMoveWindow(display, win, x0, y0);

  while (1) {
    XNextEvent(display, (XEvent *)&event);
    switch (event.type) {
      case Expose: {
        XDrawString(display, win, grContext, tx, ty, text, Str::strchars8(text));
        break;
      }
      case KeyPress: {            /// destroy window on keypress
        XDestroyWindow(display, win);
        XCloseDisplay(display);
        return;
      }
      case ClientMessage: {       // destroy window on close button press
/// this is pretty confusing, but this is how the WM_DELETE_WINDOW is handled
/// (ClientMessage+ that Atom protocol change)
        XDestroyWindow(display, win);
        XCloseDisplay(display);
        return;
      }
      case DestroyNotify: {
        printf("Destroy notify not handled\n");
        return;
      }
      break;
      default:
      break;
    } /// switch (event.type)
  }/// loop thru all avents
}
#endif /// OS_LINUX


#ifdef USING_DIRECTINPUT
void ErrorHandling::dinput(int32 n) {
  str8 s;
  switch(n) {
    case S_FALSE:                   { s= "S_FALSE: DI_BUFFEROVERFLOW: The device buffer overflowed and some input was lost.\nDI_NOEFFECT: The operation had no effect.\nDI_NOTATTACHED: The device exists but is not currently attached to the user's computer.\nDI_PROPNOEFFECT: The change in device properties had no effect."; break; }
    case DI_DOWNLOADSKIPPED:        { s= "DI_DOWNLOADSKIPPED: The parameters of the effect were successfully updated,\nbut the effect could not be downloaded because the associated device\nwas not acquired in exclusive mode."; break; }
    case DI_EFFECTRESTARTED:        { s= "The effect was stopped, the parameters were updated, and the effect was restarted."; break; }
    //case DI_NOEFFECT:               { s= "The operation had no effect. This value is equal to the S_FALSE standard COM return value. "; break; }
    //case DI_NOTATTACHED:            { s= "	 The device exists but is not currently attached to the user's computer. This value is equal to the S_FALSE standard COM return value. "; break; }
    case DI_OK:                     { s= "DI_OK: avoid to report an DI_OK ERROR FFS! The operation completed successfully."; break; }
    case DI_POLLEDDEVICE:           { s= "DI_POLLEDDEVICE: The device is a polled device. As a result, device buffering\ndoes not collect any data and event notifications is not signaled\nuntil the IDirectInputDevice8 Interface method is called."; break; }
    //case DI_PROPNOEFFECT:           { s= "	 The change in device properties had no effect. This value is equal to the S_FALSE standard COM return value. "; break; }
    case DI_SETTINGSNOTSAVED:       { s= "DI_SETTINGSNOTSAVED: The action map was applied to the device, but the settings could not be saved. "; break; }
    case DI_TRUNCATED:              { s= "DI_TRUNCATED: The parameters of the effect were successfully updated,\nbut some of them were beyond the capabilities of the device and were truncated to the nearest supported value. "; break; }
    case DI_TRUNCATEDANDRESTARTED:  { s= "DI_TRUNCATEDANDRESTARTED: Equal to DI_EFFECTRESTARTED | DI_TRUNCATED. "; break; }
    case DI_WRITEPROTECT:           { s= "DI_WRITEPROTECT: A SUCCESS code indicating that settings cannot be modified. "; break; }
    case DIERR_ACQUIRED:            { s= "DIERR_ACQUIRED: The operation cannot be performed while the device is acquired. "; break; }
    case DIERR_ALREADYINITIALIZED:  { s= "DIERR_ALREADYINITIALIZED: This object is already initialized "; break; }
    case DIERR_BADDRIVERVER:        { s= "DIERR_BADDRIVERVER: The object could not be created due to\nan incompatible driver version or mismatched or incomplete driver components. "; break; }
    case DIERR_BETADIRECTINPUTVERSION: { s= "DIERR_BETADIRECTINPUTVERSION: The application was written for\nan unsupported prerelease version of DirectInput. "; break; }
    case DIERR_DEVICEFULL:          { s= "DIERR_DEVICEFULL: The device is full. "; break; }
    case DIERR_DEVICENOTREG:        { s= "DIERR_DEVICENOTREG: The device or device instance is not registered with DirectInput."; break; }
    case DIERR_EFFECTPLAYING:       { s= "DIERR_EFFECTPLAYING: The parameters were updated in memory but were not downloaded to the device because\nthe device does not support updating an effect while it is still playing. "; break; }
    case DIERR_GENERIC:             { s= "DIERR_GENERIC: An undetermined error occurred inside the DirectInput subsystem."; break; }
    case DIERR_HANDLEEXISTS:        { s= "DIERR_HANDLEEXISTS: The device already has an event notification associated with it."; break; }
    case DIERR_HASEFFECTS:          { s= "DIERR_HASEFFECTS: The device cannot be reinitialized because effects are attached to it. "; break; }
    case DIERR_INCOMPLETEEFFECT:    { s= "DIERR_INCOMPLETEEFFECT: The effect could not be downloaded because essential information is missing.\nFor example, no axes have been associated with the effect."; break; }
    case DIERR_INPUTLOST:           { s= "DIERR_INPUTLOST: Access to the input device has been lost. It must be reacquired. "; break; }
    case DIERR_INVALIDPARAM:        { s= "DIERR_INVALIDPARAM: An invalid parameter was passed to the returning function,\nor the object was not in a state that permitted the function to be called."; break; }
    case DIERR_MAPFILEFAIL:         { s= "DIERR_MAPFILEFAIL: An error has occurred either reading the vendor-supplied\naction-mapping filefor the device or reading or writing the user configuration mapping file for the device. "; break; }
    case DIERR_MOREDATA:            { s= "DIERR_MOREDATA: Not all the requested information fit into the buffer. "; break; }
    case DIERR_NOAGGREGATION:       { s= "DIERR_NOAGGREGATION: This object does not support aggregation."; break; }
    case DIERR_NOINTERFACE:         { s= "DIERR_NOINTERFACE: The object does not support the specified interface."; break; }
    case DIERR_NOTACQUIRED:         { s= "DIERR_NOTACQUIRED: The operation cannot be performed unless the device is acquired. "; break; }
    case DIERR_NOTBUFFERED:         { s= "DIERR_NOTBUFFERED: The device is not buffered. Set the DIPROP_BUFFERSIZE property to enable buffering. "; break; }
    case DIERR_NOTDOWNLOADED:       { s= "DIERR_NOTDOWNLOADED: The effect is not downloaded. "; break; }
    case DIERR_NOTEXCLUSIVEACQUIRED:{ s= "DIERR_NOTEXCLUSIVEACQUIRED: The operation cannot be performed unless the device is acquired in DISCL_EXCLUSIVE mode. "; break; }
    case DIERR_NOTFOUND:            { s= "DIERR_NOTFOUND: The requested object does not exist. "; break; }
    case DIERR_NOTINITIALIZED:      { s= "DIERR_NOTINITIALIZED: This object has not been initialized. "; break; }
    //case DIERR_OBJECTNOTFOUND:      { s= "DIERR_OBJECTNOTFOUND: The requested object does not exist. "; break; }
    case DIERR_OLDDIRECTINPUTVERSION:{ s= "DIERR_OLDDIRECTINPUTVERSION: The application requires a newer version of DirectInput."; break; }
    //case DIERR_OTHERAPPHASPRIO:     { s= "DIERR_OTHERAPPHASPRIO: Another application has a higher priority level, preventing this call from succeeding. This value is equal to the E_ACCESSDENIED standard DirectInput return value. This error can be returned when an application has only foreground access to a device but is attempting to acquire the device while in the background. "; break; }
    case DIERR_OUTOFMEMORY:         { s= "DIERR_OUTOFMEMORY: The DirectInput subsystem could not allocate sufficient memory to complete the call.\nThis value is equal to the E_OUTOFMEMORY standard COM return value. "; break; }
    //case DIERR_READONLY:            { s= "	 The specified property cannot be changed. This value is equal to the E_ACCESSDENIED standard COM return value. "; break; }
    case DIERR_REPORTFULL:          { s= "DIERR_REPORTFULL: More information was requested to be sent than can be sent to the device. "; break; }
    case DIERR_UNPLUGGED:           { s= "DIERR_UNPLUGGED: The operation could not be completed because the device is not plugged in. "; break; }
    case DIERR_UNSUPPORTED:         { s= "DIERR_UNSUPPORTED: The function called is not supported at this time."; break; }
    case E_HANDLE:                  { s= "E_HANDLE: The HWND parameter is not a valid top-level window that belongs to the process. "; break; }
    case E_PENDING:                 { s= "E_PENDING: Data is not yet available. "; break; }
    case E_POINTER:                 { s= "E_POINTER: An invalid pointer, usually NULL, was passed as a parameter."; break; }
    default:
      s= "Unknown direct input error";
  }
  simple(s);
}
#endif /// USING_DIRECTINPUT



#ifdef USING_OPENGL
int ErrorHandling::glError(cchar *text) {
  int ret= glGetError();
  if(!ret) return 0;        // fast return if no error

  if(ret== GL_INVALID_ENUM)
    simple(text? str8(text)+ " GL ERROR: GL_INVALID_ENUM": "OpenGL ERROR: GL_INVALID_ENUM");
  else if(ret== GL_INVALID_VALUE)
    simple(text? str8(text)+ " GL ERROR: GL_INVALID_VALUE": "OpenGL ERROR: GL_INVALID_VALUE");
  else if(ret== GL_INVALID_OPERATION)
    simple(text? str8(text)+ " GL ERROR: GL_INVALID_OPERATION": "OpenGL ERROR: GL_INVALID_OPERATION");
  else if(ret== GL_OUT_OF_MEMORY)
    simple(text? str8(text)+ " GL ERROR: GL_OUT_OF_MEMORY": "OpenGL ERROR: GL_OUT_OF_MEMORY");
  else if(ret== GL_STACK_UNDERFLOW)
    simple(text? str8(text)+ " GL ERROR: GL_STACK_UNDERFLOW": "OpenGL ERROR: GL_STACK_UNDERFLOW");
  else if(ret== GL_STACK_OVERFLOW)
    simple(text? str8(text)+ " GL ERROR: GL_STACK_OVERFLOW": "OpenGL ERROR: GL_STACK_OVERFLOW");

  return ret;  
}
#endif /// USING_OPENGL








