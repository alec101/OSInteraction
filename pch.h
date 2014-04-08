// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

// CASE SENSITIVE FILENAMES!!!! (linux)

// put [..] [../source] [../../!utilClasses] in your include directories

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
 
// LINUX: add those libs in project libraries used, and to get the actual libs used:
// sudo apt-get install libx11-dev         for X11/Xlib.h  < probably this is already installed
// sudo apt-get install mesa-common-dev    GL/glx.h - openGL linux specific
// sudo apt-get install libglu1-mesa-dev   GL/glu.h - openGL simple utils (glut should be avoided)
//   or the freeglu one
// sudo apt-get install libxrandr-dev      used for resolution changes
// sudo apt-get install libxinerama-dev    used just for monitor numbers on _NET_WM_FULLSCREEN_MONITORS. maybe there's another way to find these numbers...
// !NOT USED! sudo apt-get install libxi-dev          !NOT USED! (might be used for joysticks but it is too old)

#pragma once

#ifdef OS_WIN
#define USING_DIRECTINPUT 1
#define USING_XINPUT
//#define _WIN32_WINNT 0x05010000
//#define WINVER 0x0501

#include "targetver.h"
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


#ifdef OS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else /// OS_WIN + OS_LINUX
#include <GL/gl.h>
#include <GL/glu.h>
#endif 

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

#include "OSdisplay.h"
#include "OSinteraction.h"
#include "OSchar.h"
#include "OSinput.h"
#include "OScocoa.h"






