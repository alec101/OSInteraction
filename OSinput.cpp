#include "pch.h"

//#include <sys/types.h>
//#include <sys/stat.h>

//#include <X11/extensions/XInput.h>


/// if not using precompiled header use the following
/*
#include <math.h>
 
#ifdef USING_DIRECTINPUT
  #define DIRECTINPUT_VERSION 0x0800
  #include <dinput.h>
#endif
#ifdef USING_XINPUT
  // #include <c:\alec\dxSDK2010j\Include\XInput.h> // USE XINPUT 1.3
#endif
 
#ifdef OS_LINUX
#include <linux/joystick.h>   // it's not x stuff... lots of crap added, keyboard/mouse, that is not needed
#include <fcntl.h>
#endif

#include "../!utilClasses/typeShortcuts.h"
#include "../!utilClasses/stringClass.h"
#include "../!utilClasses/errorHandling.h"
#include "OSchar.h"
#include "OSinput.h"
*/



/* TODO:
 *
 * every COORDONATE UNIFICATION: x0,y0 -> left,bottom (as in OpenGL, MAC.  NOT AS IN: windows, (i think linux too)
      this includes mouseWheel (+y means UP, as in the real cood system)
 
 * 
 
 
 * joystick button history
 * 
 * name the TYPE2 / TYPE3 driver (osi must handle at least that)
 *
 * i think every time variable should be int64... dunno for shure. nanosecs are int64, mili= nano* 1,000,000... still way more data to hold in a int64
 *    they may be a little slower... dunno, but not by much
 *
 * buy a ffeedback joystick
 * buy a wheel
 * 
 */




/*	
    WM_POINTERDOWN <-- pens/watever? should be implemented?
    movement ID ? last movement/ current movement ID, so u know if blabla/ BUT, dont think it's worth it. too much precision for almost nothing
    even wheel ID, maybe a buffer of 4-8 commands, check if they were processed ?
*/

// makes a null cursor LINUX/QUAKE2 SOURCE
/*
static Cursor CreateNullCursor(Display *display, Window root)
{
    Pixmap cursormask; 
    XGCValues xgc;
    GC gc;
    XColor dummycolour;
    Cursor cursor;

    cursormask = XCreatePixmap(display, root, 1, 1, 1); // last var is depth
    xgc.function = GXclear;
    gc =  XCreateGC(display, cursormask, GCFunction, &xgc);
    XFillRectangle(display, cursormask, gc, 0, 0, 1, 1);
    dummycolour.pixel = 0;
    dummycolour.red = 0;
    dummycolour.flags = 04;
    cursor = XCreatePixmapCursor(display, cursormask, cursormask,
          &dummycolour,&dummycolour, 0,0);
    XFreePixmap(display,cursormask);
    XFreeGC(display,gc);
    return cursor;
}
*/


// Input in;   // PLACE IT HERE????????????????????????????



// EFFECTS TESTING ... must aquire a WORKING forcefeedback / vibration HID, first ffs!
// -----------------------------======================--------------------------------

void Input::vibrate() {
  #ifdef OS_WIN // direct input vibration
  /*
  HRESULT  hr;
  DIEFFECT diEffect;               // parameters for created effect

  DWORD    dwAxes[2] = { DIJOFS_X, DIJOFS_Y };
  LONG     lDirection[2] = { 18000, 0 };

  DIPERIODIC periodic;
  DICONSTANTFORCE constant;

  constant.lMagnitude= DI_FFNOMINALMAX;
  
  periodic.dwMagnitude= DI_FFNOMINALMAX;
  periodic.dwPeriod= 5000;
  periodic.dwPhase= 0;
  periodic.lOffset= 0;
  

  diEffect.dwSize          = sizeof(DIEFFECT); 
  
  diEffect.dwFlags         = DIEFF_POLAR | DIEFF_OBJECTOFFSETS; 
  diEffect.dwDuration      = (DWORD)(0.5 * DI_SECONDS);
  diEffect.dwSamplePeriod  = 5880;                 // = default 
  diEffect.dwGain          = DI_FFNOMINALMAX;   // No scaling
  diEffect.dwTriggerButton = DIEB_NOTRIGGER;    // Not a button response
  diEffect.dwTriggerRepeatInterval = 0;         // Not applicable
  diEffect.cAxes                   = 2; 
  diEffect.rgdwAxes                = &dwAxes[0]; 
  diEffect.rglDirection            = &lDirection[0]; 
  diEffect.lpEnvelope              = NULL; 
  */


  // wLeftMotorSpeed
  long hr;
        DWORD    rgdwAxes[2] = { DIJOFS_X, DIJOFS_Y };  // X- and y-axis
        LONG rglDirection[2] = { 0, 0 };

        DICONSTANTFORCE cf;
        DIEFFECT eff;

        ZeroMemory( &eff, sizeof( eff ) );
        eff.dwSize = sizeof( DIEFFECT );
        eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
        eff.dwDuration = INFINITE;
        eff.dwSamplePeriod = 0;
        eff.dwGain = DI_FFNOMINALMAX;
        eff.dwTriggerButton = DIEB_NOTRIGGER;
        eff.dwTriggerRepeatInterval = 0;
        eff.cAxes = 2; //device.ff.axisffbcount;
        eff.rgdwAxes = rgdwAxes;
        eff.rglDirection = rglDirection;
        eff.lpEnvelope = 0;
        eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
        eff.lpvTypeSpecificParams = &cf;
        eff.dwStartDelay = 0;


        // Create the prepared effect
        hr= gp[4].diDevice->CreateEffect( GUID_ConstantForce, &eff, &gp[4].vibration, NULL );
        

  //diEffect.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
  //diEffect.lpvTypeSpecificParams = &constant;  
  
        /*
  diEffect.cbTypeSpecificParams    = sizeof(DIPERIODIC);
  diEffect.lpvTypeSpecificParams   = &periodic;  
  

  hr= gp[4].diDevice->CreateEffect(GUID_SawtoothUp, &diEffect , &gp[4].vibration, NULL);

  gp[4].diDevice->SendForceFeedbackCommand(DISFFC_SETACTUATORSON);
  */
        if(hr!= DI_OK)
          error.dinput(hr);

  
//	lpdiEffect->Download();
  //gp[4].vibration->Start(1, null);
  //hr = g_lpdid->CreateEffect(GUID_ConstantForce, &diEffect, &lpdiEffect, NULL);
  #endif /// OS_WIN
}
/// end testing




// ------------============= INPUT CLASS ===========--------------------
// =====================================================================
Input::Input() {
  nr.jFound= nr.gpFound= nr.gwFound= 0;
  
  nr.jOS= nr.gpOS= nr.gwOS= 0;
  nr.jT2= nr.gpT2= nr.gwT2= 0;
  nr.jT3= nr.gpT3= nr.gwT3= 0;
  
  #ifdef USING_DIRECTINPUT
  m.diDevice= null;
  dInput= null;
  #endif /// USING_DIRECTINPUT
  
  #ifdef OS_MAC
  k.numLock= true;    /// macs don't handle num locks. this will always be on
  #endif /// OS_MAC
}

Input::~Input() {
  delData();
}

void Input::delData() {
  m.delData();
  k.delData();
  short a;
  for(a= 0; a< 20; a++)     // << ----------- FIXED VARS if nr of max controllers changes, must remember to change these too
    j[a].delData();
  for(a= 0; a< 20; a++)
    gp[a].delData();
  for(a= 0; a< 20; a++)
    gw[a].delData();

  nr.jFound= nr.gpFound= nr.gwFound= 0;
  nr.jOS= nr.gpOS= nr.gwOS= 0;
  nr.jT2= nr.gpT2= nr.gwT2= 0;
  nr.jT3= nr.gpT3= nr.gwT3= 0;

  #ifdef USING_DIRECTINPUT
  if(dInput)
    dInput->Release();
  dInput= null;
  #endif
}

bool Input::init(int mMode, int kMode) {
  m.mode= mMode;
  k.mode= kMode;
  #ifdef USING_DIRECTINPUT
  if(!dInput)
    if(DirectInput8Create(osi.win[0].hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL)!= DI_OK) {
      error.simple("Could not initialize Direct Input");
      return false;
    }
  #endif

  /// fill in all vars, find all xinput/directinput devices, etc
  populate(true);
  
  /// Kv struct has (almost) all keyboard keys. It has to 'populate' all vars @ start
  Kv.populate();

  #ifdef USING_DIRECTINPUT
  for(short a= 0; a< nr.jT2; a++) {
    getT2j(a)->init(2);     /// 1 init for all 3 HIDs (it is an option how to use the device data, as a wheel/gamepad/joystick)
    getT2gp(a)->mode= 2;
    getT2gw(a)->mode= 2;
  }
  #endif
  
  #ifdef USING_XINPUT
  for(short a= 0; a< nr.jT3; a++) {
    getT3j(a)->init(3);     /// 1 init for all 3 HIDs (it is an option how to use the device data, as a wheel/gamepad/joystick)
    getT3gp(a)->mode= 3;
    getT3gw(a)->mode= 3;
  }
  #endif /// USING_XINPUT


  return true;
}


void Input::populate(bool scanMouseKeyboard) {
  //bool chatty= true;
  if(scanMouseKeyboard) {

    // ---===MOUSE SCAN===---
    if(m.mode== 3) {
      m.delData();
      if(!m.init(3))        /// try to use direct input
        m.init(2);          /// else use defaul windows functions
    } else
      m.init(m.mode);

    // ---===KEYBOARD SCAN===---
    if(k.mode== 3) {
      k.delData();
      if(!k.init(3))
        k.init(2);
    }
  } /// scanMouseKeyboard

  // ---===JOYSTICK SCAN===---
  nr.jOS= 0;

  #ifdef OS_WIN
  int b= joyGetNumDevs();
  if(joyGetNumDevs()) {     /// if this func doesn't return a positive number, there is no driver installed
    change me (max 8 joysticks)
/// search normal driver joysticks
    JOYINFOEX jinfo;
    jinfo.dwSize= sizeof(JOYINFOEX);
    if(b> 16) b= 16;          // limit to 16 detectable joysticks
    for(uint a= 0; a< joyGetNumDevs(); a++)
      if(joyGetPosEx(a, &jinfo) != JOYERR_UNPLUGGED) {        /// if it's plugged in-> found a joystick
        j[nr.jOS].id= a;
        j[nr.jOS].mode= 1;
        nr.jOS++;
      }
  }
  #endif /// OS_WIN

  #ifdef USING_DIRECTINPUT
  dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, diDevCallback, NULL, DIEDFL_ATTACHEDONLY);
  #endif /// USING_DIRECTINPUT

  #ifdef OS_LINUX

  // xlib xinput ... after some research, found nothing
  /*
  int ndev;

  XDeviceInfo *dev= XListInputDevices(osi.primWin->dis, &ndev);
  
  for(short a= 0; a< ndev; a++) {
    printf("dev[%d] id[%d] [%s] use[%d]\n", a, dev[a].id, dev[a].name, dev[a].use);
  }
  */
  
  // linux [MODE 1] using "linux/joystick.h". system driver
  
  
  // NOT DONE, must use JSIOGCVERSION
  /*
    JSIOGCVERSION is a good way to check in run-time whether the running
      driver is 1.0+ and supports the event interface. If it is not, the
      IOCTL will fail. For a compile-time decision, you can test the
      JS_VERSION symbol
   
   * just set the joysticks/gpads/gwheels on mode 0, so they are disabled if OS is not using joy driver
  */
  
  
  
  
  
	//#ifdef JS_VERSION
	//#if JS_VERSION > 0xsomething
  
  int f;
  int version, axes= 0, buttons= 0;
  char name[128];
  short id= 0;
  string s= "/dev/input/js";
  
  /// searching for 32 js[X] files
  for(short a= 0; a< 32; a++) {
    /// this limit to 8 can be changed if neccesary... can't predict what will happen in 10-20 years....
    if(id== 8) {
      error.simple("OSInput::init: Maximum number of jSticks/gPads/gWheels reached (where did you plug more than 8?)");
      break;
    }
      
    f= version= axes= buttons=0;
    
    f= open(s+ (a+ 48ul), O_RDONLY| O_NONBLOCK);
    if(f== -1) continue;
    
    ioctl(f, JSIOCGAXES, &axes);
    ioctl(f, JSIOCGBUTTONS, &buttons);
    ioctl(f, JSIOCGVERSION, &version);
    ioctl(f, JSIOCGNAME(sizeof(name)), &name);

    /// check if the joystick is valid. I found out that there are mouses that report as joysticks...
    ///    but they report incredible amounts of axes and buttons...
    ///    still, can't rely only on this, FURTHER CHECKS SHOULD BE MADE  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if((axes> 32) || (buttons> MAX_JOYSTICK_BUTTONS)) {
      close(f);
      continue;
    }

    /// all joysticks/gamepads/gamewheels share the same file (driver). 
    j[id].mode= gp[id].mode= gw[id].mode= 1;
    j[id].id=   gp[id].id=   gw[id].id=   a;
    j[id].file= gp[id].file= gw[id].file= f;
    j[id].name= gp[id].name= gw[id].name= name;
    
    /// better to have lots of vars. The other option would be having complicated search algorithms just to find out how many joysticks found that use driver N
    ///   the purpose is to have the same code run on every OS... game searches for driver type 1 (os driver), type 2(in win is directinput) etc...
    nr.jFound++;  nr.gpFound++; nr.gwFound++; 
    nr.jOS++;     nr.gpOS++;    nr.gwOS++;
    
    id++;
    if(chatty) printf("Name: %s Axes: %d Buttons: %d Version: %d\n", name, axes, buttons, version);
  }
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  #endif /// OS_MAC
  
  /// all joys/gp/gw will have same directinput/xinput drivers
  // ---===GAMEPAD SCAN===---

  // ---===GAMEWHEEL SCAN===---


}

void Input::resetPressedButtons() {
  m.resetButtons();
  k.resetButtons();
}



void Input::update() {
  //  getDIgp(0)->update();
  // for (each gamepad if it is active, update?)
  // same for everything else
  // need an "active" or "inUse" bool
  
  // !!!!
  // types have changed, so same code can be used on every os
  // a type should have a name (direct input/xinput/os blabla), to pass to the program
  // !!!!
  
  #ifdef OS_LINUX
  /// [MODE 1] joysticks/ gpads/ gwheels
  int n= -1, nev;
  js_event ev[64];
  
  for(short a= 0; a< nr.jOS; a++) {       // for all active joysticks

ReadAgain:
    /// read as many events as possible in 1 go
    n= read(j[a].file, ev, sizeof(ev));
      
    if(n== -1)                            /// no event happened
      continue;

    nev= n/ sizeof(js_event);             /// nr of events read
      
    for(short b= 0; b< nev; b++) {         // for each event
      ev[b].type&= ~JS_EVENT_INIT;        /// disable the init flag... no use i can think of ATM
        
        
      if(ev[b].type == JS_EVENT_BUTTON) {  // ---=== button event ===---
        j[a].b[ev[b].number]=  (uchar)ev[b].value;
        gp[a].b[ev[b].number]= (uchar)ev[b].value;
        //gw[a].b[ev[b].number]= (uchar)ev[b].value;
        
        if(ev[b].value== 1) {             /// press
          j[a].bTime[ev[b].number]= ev[b].time;
          gp[a].bTime[ev[b].number]= ev[b].time;
          gw[a].bTime[ev[b].number]= ev[b].time;
        } else {                          /// release
          /// put the button in history
          ButPressed p;
          p.b= ev[b].number;
          p.checked= false;
          p.timeDown= j[a].bTime[ev[b].number];
          p.timeUp= ev[b].time;
          p.timeDT= p.timeUp- p.timeDown;
          
          j[a].log(p);
          gp[a].log(p);
          gw[a].log(p);
        }
          
      } /// button event
        
        
        
      if(ev[b].type== JS_EVENT_AXIS) {     // ---=== axis event ===---
        /// axis order...
        
        // possible to make a[MAX_AXIS] and x/y/rudder/etc would be refrences to a[]
        switch (ev[b].number) {
                                          // [JOYSTICK]  / [GAMEPAD]   / [GAMEWHEEL]
          case 0:                         // [X axis?]   / [l stick X] / [wheel???]
            gp[a].lx= ev[b].value;
            
          case 1:                         // [Y axis?]   / [l stick Y] / [wheel???]
            gp[a].ly= ev[b].value;
            
            break;
          case 2:                         // [Throttle?] / [r stick X] / [wheel???]
            gp[a].rx= ev[b].value;
            
            break;
          case 3:                         // [extra1 X?] / [l trigger] / [wheel???]
            gp[a].lt= ev[b].value;
            
            break;
          case 4:                         // [extra1 Y?] / [r trigger] / [wheel???]
            gp[a].rt= ev[b].value;
            
            break;
          case 5:                         // [Rudder?]   / [rStick Y]  / [wheel???]
            gp[a].ry= ev[b].value;
            
            break;
          case 6:                         // [POV X?]    / [POV X]     / [wheel???]
          case 7:                         // [POV Y?]    / [POV Y]     / [wheel???]
            long x, y;          // they gotta be integers for exact 0 degrees or 90 degrees, else there are problems
            double pov;
            
            /// get axis from current pov position (wich is in degrees)
            pov= j[a].pov;
            x= y= 0;
            
            if(j[a].pov!= -1) {           /// ... only if it's not on -1 position (nothing selected)
              x= (double)(32767.0* sin(j[a].pov* (M_PI/ 180.0)));
              y= (double)(32767.0* cos(j[a].pov* (M_PI/ 180.0)));
            }
            
            /// update from event
            if(ev[b].number== 6)          /// x axis event
              x= ev[b].value;
            else                          /// y axis event
              y= -ev[b].value;
            
            /// find pov in degrees; there have to be checks for each quadrant, unfortunatelly (bad for speed)
            if(y> 0) {
              if(x>= 0)
                pov= (double) ((atan(x/ y))* (180.0/ M_PI));
              else
                pov= (double) ((2* M_PI+ atan(x/ y))* (180.0/ M_PI));
            } else if(y< 0) {
              pov= (double) (( M_PI+ atan(x/ y))* (180.0/ M_PI));
              
            } else if(y == 0) {
              if(x== 0)
                pov= -1;
              else if(x> 0)
                pov= 90;
              else if(x< 0)
                pov= 270;
            }
            
            /// pov found @ this point
            j[a].pov= pov;
            gp[a].pov= pov;
            // gw is not set<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            
            break;
          case 8:                         // [u axis]    / [u axis]    / [u axis]
            //gp[a].u= ev[b].value;
            
            break;
          case 9:                         // [v axis]    / [v axis]    / [v axis]
            //gp[a].v= ev[b].value;
            
          case 10:
            gp[a].lx= ev[b].value;
          case 11:
            gp[a].ly= ev[b].value;
          case 12:
            gp[a].rx= ev[b].value;
          case 13:
            gp[a].ry= ev[b].value;
            
            break;
            
        }
      } /// axis event

    } /// for each event
      
    // did it actually read 64 events? read another 64 then...
    if(nev == 64)
      goto ReadAgain;
    
    
  } /// for all active joysticks
  
  #endif /// OS_LINUX

  #ifdef USING_XINPUT //          ---- msdn copy/paste ...
  DWORD dwResult;    
  for(DWORD i= 0; i< XUSER_MAX_COUNT; i++ ) {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    /// Simply get the state of the controller from XInput.  
    dwResult= XInputGetState(i, &state);

    if(dwResult == ERROR_SUCCESS)
      printf("XInput controller %d CONNECTED\n", i);
    else
      printf("XInput controller %d NOT connected\n", i);
  }
  #endif /// USING_XINPUT

  #ifdef USING_DIRECTINPUT
  #endif /// USING_DIRECTINPUT

}


#ifdef USING_DIRECTINPUT
BOOL CALLBACK diDevCallback(LPCDIDEVICEINSTANCE inst, LPVOID extra) {
  HRESULT hr;
  short n= in.nr.jDI;

  hr= in.dInput->CreateDevice(inst->guidInstance, &in.getDIj(n)->diDevice, NULL);
  if (FAILED(hr))
    return DIENUM_CONTINUE;

  in.getDIgp(n)->diDevice= in.getDIgw(n)->diDevice= in.getDIj(n)->diDevice;

  in.nr.jDI++;
  in.nr.gpDI++;
  in.nr.gwDI++;
  
  in.nr.jFound++;
  in.nr.gpFound++;
  in.nr.gwFound++;

  return DIENUM_CONTINUE;
}
#endif /// USING_DIRECTINPUT


// ------------============= MOUSE CLASS ===========--------------------
// =====================================================================

Mouse::Mouse() {
  mode= 1;
  useDelta= false;

  x= y= dx= dy= oldx= oldy= wheel= 0;
  #ifdef USING_DIRECTINPUT
  diDevice= null;
  diStats= {0};
  #endif /// USING_DIRECTINPUT
}

Mouse::~Mouse() {
  delData();
}

void Mouse::delData() {
  mode= 1;
  useDelta= false;
  x= y= dx= dy= oldx= oldy= wheel= 0;

  #ifdef USING_DIRECTINPUT
  if(diDevice) {
    diDevice->Unacquire();
    diDevice->Release();
    diDevice= null;
  }
  #endif /// USING_DIRECTINPUT

}

bool Mouse::init(short mode) {
  delData();
  this->mode= mode;
  if(mode== 1) {          /// no init required atm
    return true;
  }
  if(mode== 2) {          /// no init required atm
    return true;
  }

  if(mode== 3) {
    #ifdef USING_DIRECTINPUT
    long hr= 0;
    DIDATAFORMAT mouseformat;
    mouseformat= c_dfDIMouse;
    mouseformat.dwFlags= DIDF_RELAXIS; 

    hr= in.dInput->CreateDevice(GUID_SysMouse, &diDevice, NULL);

    hr= diDevice->SetDataFormat(&mouseformat);
    hr= diDevice->SetCooperativeLevel(osi.win[0].hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
    diDevice->Acquire();

    if(hr!= DI_OK) {
      error.simple("Could not initialize the mouse under DirectInput.");
      return false;
    }

    if(diDevice== NULL) {
      return false;
    }

    return true;		/// reached this point, returns a success
    #endif
    #ifndef USING_DIRECTINPUT
    error.simple("USING_DIRECTINPUT not defined; no code for direct input compiled");
    return false;
    #endif
  }
  return false;     /// this point is reaced-> fail
}

bool Mouse::aquire() {
  if(mode== 1) {
    #ifdef OS_LINUX
    XGrabPointer(osi.primWin->dis, osi.primWin->win,
                 True,              // send events or not
                 ButtonPressMask| ButtonReleaseMask| PointerMotionMask,
                 GrabModeSync,      // pointer - GrabMode[Async/Sync]
                 GrabModeSync,      // keyboard- GrabMode[Async/Sync]
                 osi.primWin->win,  // confine cursor to a window
                 None,              // mouse cursor (graphics)
                 CurrentTime);
    #endif /// OS_LINUX
    return true;
  }
  if(mode== 2)                    /// nothing to aquire
    return true;
  if(mode== 3) {
    #ifdef USING_DIRECTINPUT
    if(diDevice->Acquire()== DI_OK)
      return true;
    #endif
  }

  return false;
}

bool Mouse::unaquire() {
  if(mode== 1) {
    #ifdef OS_LINUX
    XUngrabPointer(osi.primWin->dis, CurrentTime);
    #endif
    return true;
  }
  if(mode== 2)
    return true;
  
  if(mode== 3) {
    #ifdef USING_DIRECTINPUT
    if(diDevice->Unacquire()== DI_OK)
      return true;
    #endif
  }
  return false;
}

void Mouse::resetButtons() {
  uint64 present; osi.getMillisecs(&present);
  for(short a= 0; a< MAX_MOUSE_BUTTONS; a++) {
    if(b[a].down) {
      b[a].lastTimeStart= b[a].timeStart;
      b[a].lastTimeEnded= present;
      b[a].lastDT= b[a].lastTimeEnded- b[a].lastTimeStart;
      b[a].timeStart= 0;
      b[a].down= false;
    }
  }
}

void Mouse::update() {
  if(!osi.flags.haveFocus)
    return;
/// os events: nothing to update, atm (i cant think of anything anyways)
  if(mode== 1) {

  }

/// manual update mode
  if(mode== 2) {
    #ifdef OS_WIN

    /// mouse position
    POINT p;
    GetCursorPos(&p);
    oldx= x;
    oldy= y;
    x= p.x;
    y= p.y;
    if(useDelta) {
      dx+= x- oldx;
      dy+= y- oldy;
    }
    
    /// mouse wheel
    
    // well, shit! gotta use window events, unfortunatly. 
    // NO WIN FUNC AVAIBLE TO READ THE WHEEL IN ANY WAY. the only way is too complicated, with hooks to dll-s... /cry

    /// mouse buttons
    uint64 present;
    osi.getMillisecs(&present);
    bool t;

    for(short a= 0; a< 5; a++) {                  /// for all mouse buttons (that windows knows ATM)
      if(a==0) t= GetAsyncKeyState(VK_LBUTTON)? true: false;        // LEFT BUTTON
      if(a==1) t= GetAsyncKeyState(VK_RBUTTON)? true: false;        // RIGHT BUTTON
      if(a==2) t= GetAsyncKeyState(VK_MBUTTON)? true: false;        // MIDDLE BUTTON
      if(a==3) t= GetAsyncKeyState(VK_XBUTTON1)? true: false;       // BUTTON 4
      if(a==4) t= GetAsyncKeyState(VK_XBUTTON2)? true: false;       // BUTTON 5

      if(b[a].down&& !t) {          /// button release
        b[a].lastTimeStart= b[a].timeStart;
        b[a].lastTimeEnded= present;
        b[a].lastDT= b[a].lastTimeEnded- b[a].lastTimeStart;
        b[a].down= t;
      }
      if((!b[a].down)&& t) {        /// button press started
        b[a].timeStart= present;
        b[a].down= t;
      }
    }

    #endif /// OS_WIN

    #ifdef OS_LINUX
    // cant find any funcs. events are the only option i can find
    //error.simple("Mouse cannot be manually updated under linux. use [MODE 1]");
    #endif /// OS_LINUX

    #ifdef OS_MAC       // <-----------------------------------
//    makeme
    #endif /// OS_MAC
  }

  /// direct input
  #ifdef USING_DIRECTINPUT           /// skip some checks. only mode 1 works atm in linux
  if(mode== 3) {
    
    diDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&diStats);
    /// mouse position
    oldx= x;
    oldy= y;
    x= diStats.lX;
    y= diStats.lY;
    if(useDelta) {
      dx+= x- oldx;
      dy+= y- oldy;
    }
    
    /// mouse wheel
    // THIS PART NEEDS TESTING, DUNNO WHAT DIRECT INPUT TRANMITS !!!!!!!!!!!!!!!  ALL DIRECT X PART, even. NEED A WORKING GLPRINT FUNC
    if(diStats.lZ)
      wheel++;
    else
      wheel--;

    /// mouse buttons
    bool t;
    int present= GetTickCount();
    for(short a= 0; a< 8; a++) {
      t= diStats.rgbButtons[a]? true: false;
      if((b[a].down)&& !t) {                  /// a button press ended
        b[a].lastTimeStart= b[a].timeStart;
        b[a].lastTimeEnded= present;
        b[a].lastDT= b[a].lastTimeEnded- b[a].lastTimeStart;
        b[a].down= t;
      }
      if((!b[a].down)&& t) {                  /// a button press starts
        b[a].timeStart= present;
        b[a].down= t;
      }
    }
  }
  #endif /// USING_DIRECTINPUT
}

Mouse::Button::Button() {
    lastDT= lastTimeStart= lastTimeEnded= 0;

    down= false;
    timeStart= 0;
}







// ------------============= KEYBOARD CLASS ===========--------------------
// ========================================================================

Keyboard::Keyboard():
charTyped(40, sizeof(chTyped)),
manipTyped(40, sizeof(chTyped)) {
  mode= 1;
  delData();
}

Keyboard::~Keyboard() {
//	delData();
  #ifdef USING_DIRECTINPUT
  if(diDevice) {
    diDevice->Unacquire();
    diDevice->Release();
    diDevice= null;
  }
  #endif
}

void Keyboard::delData() {
  key= buffer1;
  lastCheck= buffer2;
  capsLock= scrollLock= numLock= false;       /// the 3 toggle locks

  /// clear buffers
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++) {
    buffer1[a]= buffer2[a]= 0;
    keyTime[a]= 0;
    //    repeat[a]= 0;
  }
  clearTypedBuffer();                         /// character/manip streams too
  
  /// mark initial history keys as checked, so they get ignored
  for(short a= 0; a< MAX_KEYS_LOGGED; a++)
    lastKey[a].checked= true;
}



bool Keyboard::init(short mode) {
  this->mode= mode;
  if(mode== 1)
    return true;
  if(mode== 2)
    return true;
  if(mode== 3) {
    #ifdef USING_DIRECTINPUT
    long hr= 0;
        
    hr= in.dInput->CreateDevice(GUID_SysKeyboard, &diDevice, NULL);
    hr= diDevice->SetDataFormat(&c_dfDIKeyboard);
    hr= diDevice->SetCooperativeLevel(osi.win[0].hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
    diDevice->Acquire();

    if(hr!= DI_OK) {
      error.simple("Could not initialize the keyboard under DirectInput.");
      return false;
    }

    if(diDevice== NULL)
      return false;

    return true;
    #endif /// USING_DIRECTINPUT
  }
  return false;
}


short Keyboard::getFirstKey() {
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
    if(key[a])
      return a;

  return 0;	/// fail
}

// debugging
void Keyboard::printPressed() {
  if((mode== 2) || (mode== 3))
    for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
      if(key[a]& 0x80)
        printf(" kc%d(%d)", a, key[a]);
}


void Keyboard::update() {
  
  if(!osi.flags.haveFocus)
    return;
  if(mode== 1)    // nothing to update, >>>>>>...... hmm maybe the LOCK STATES....<<<<<<<<<<<
    return;
  
  if(mode== 2) {
    #ifdef OS_WIN
    swapBuffers();			/// ye, swap them buffers
    GetKeyboardState(key);
    /// do not return;
    #endif /// OS_WIN
  }

  if(mode== 3) {
    #ifdef USING_DIRECTINPUT
    swapBuffers();
    /*if(*/ diDevice->GetDeviceState(MAX_KEYBOARD_KEYS, key); //== DIERR_INPUTLOST) { setActive(); return; } // alien engine
    /// do not return
    #endif
  }
  
  
  #ifdef OS_WIN // i can't think of a way atm, to use this in linux. the only mode is [mode 1]
  
  uint64 present;
  osi.getMillisecs(&present);

  /// check if a key just started to be pressed or a key was depressed - might be a little time consuming
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
    if((key[a]& 0x80)&& !(lastCheck[a]& 0x80) ) {         // key press start
      keyTime[a]= present;
      KeyPressed k;
      k.code= a;
      k.checked= false;
      k.timeDown= present;
      log(k);
    }
    else if((lastCheck[a]& 0x80)&& !(key[a]& 0x80)) {     // key depressed
      bool found= false;
      for(short b= 0; b< MAX_KEYS_LOGGED; b++)
        if(lastKey[b].code== a) {
          lastKey[b].timeUp= present;
          lastKey[b].timeDT= present- lastKey[b].timeDown;
          found= true;
          break;
        }
      if(!found) {
        KeyPressed k;
        k.code= a;
        k.checked= false;
        k.timeDown= present- 1;
        k.timeUp= present;
        k.timeDT= 1;
        log(k);                                     /// put it in history buffer
      }
    }
  #endif /// OS_WIN
}


void Keyboard::swapBuffers() {
  lastCheck= key;
  key= (key== buffer1)? buffer2: buffer1;
}


bool Keyboard::aquire() {
  if(mode== 1) {
    #ifdef OS_LINUX
    XGrabKeyboard(osi.primWin->dis, osi.primWin->win,
                  True,           // continue to send events or not
                  GrabModeSync,   // pointer - GrabMode[Async/Sync]
                  GrabModeSync,   // keyboard- GrabMode[Async/Sync]
                  CurrentTime);
    #endif /// OS_LINUX
    return true;
  }
  if(mode== 2)                    /// nothing to aquire
    return true;
  if(mode== 3) {
    #ifdef USING_DIRECTINPUT
    if(diDevice->Acquire()== DI_OK)
      return true;
    #endif
  }
  return false;
}


bool Keyboard::unaquire() {
  if(mode== 1) {
    #ifdef OS_LINUX
    XUngrabKeyboard(osi.primWin->dis, CurrentTime);
    #endif /// OS_LINUX
    return true;
  }
  if(mode== 2)
    return true;
  if(mode== 3) {
    #ifdef USING_DIRECTINPUT
    if(diDevice->Unacquire()== DI_OK)
      return true;
    #endif
  }
  return false;
}


void Keyboard::resetButtons() {
  uint64 present;
  osi.getMillisecs(&present);// <<----------------- there should be only one present var, set in osi, so there is only 1 call to this func. many calls= time wasted

  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++) {
    if(key[a]) {
      KeyPressed k;
      k.code= a;
      k.checked= false;
      k.timeDown= keyTime[a];
      k.timeUp= present;
      k.timeDT= k.timeUp- k.timeDown;
      log(k);
    }
  }
}


void Keyboard::updateLocks() {
  #ifdef OS_WIN
  capsLock=   GetKeyState(VK_CAPITAL)& 0x01 == 1;
  numLock=    GetKeyState(VK_NUMLOCK)& 0x01 == 1;
  scrollLock= GetKeyState(VK_SCROLL)&  0x01 == 1;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  uint n;
  XkbGetIndicatorState(osi.primWin->dis, XkbUseCoreKbd, &n);
  capsLock=   (n& 0x01) == 1;
  numLock=    (n& 0x02) == 1;
  scrollLock= (n& 0x04) == 1;
  #endif /// OS_LINUX

  #ifdef OS_MAC
//  makeme
  #endif /// OS_MAC
}


void Keyboard::log(const Keyboard::KeyPressed &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastKey[a]= lastKey[a- 1];
  lastKey[0]= k;
}

/*
void Keyboard::logd(const Keyboard::KeyDown &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastKeyDown[a]= lastKeyDown[a- 1];
  lastKeyDown[0]= k;
}
*/

ulong Keyboard::getChar() {
  if(!charTyped.nrNodes)
    return 0;

  /// clear old typed characters (must have stayed in buffer longer than 1 sec)
  uint64 present;
  osi.getMillisecs(&present);
  
  while(charTyped.first)
    if( (present- ((chTyped*)charTyped.last)->time)> 1000)  // if character stayed in buffer longer than 1 second, delete. IS 1 SEC OK?????
      charTyped.del(charTyped.last);
    else                                                    /// the rest of chars couldn't have stayed in buffer longer than 1 sec if the last char didn't
      break;

  /// return the last character in buffer (usually there is only 1... maybe in cases of low framerate, loading stuff while typeing... etc)
  ulong ret= ((chTyped*)(charTyped.last))->c;
  charTyped.del(charTyped.last);                            /// del character from buffer
  return ret;
}


// basically same func as getChar, but for the other stream of string manipulation keys
ulong Keyboard::getManip() {
  if(!manipTyped.nrNodes)
    return 0;
  
  /// clear old typed characters (must have stayed in buffer longer than 1 sec)
  uint64 present;
  osi.getMillisecs(&present);
  
  while(manipTyped.first)
    if( (present- ((chTyped*)manipTyped.last)->time)> 1000) // if character stayed in buffer longer than 1 second, delete. IS 1 SEC OK?????
      manipTyped.del(manipTyped.last);
    else                                                    /// the rest of chars couldn't have stayed in buffer longer than 1 sec if the last char didn't
      break;

  /// return the last character in buffer (usually there is only 1... maybe in cases of low framerate, loading stuff while typeing... etc)
  ulong ret= ((chTyped*)(manipTyped.last))->c;
  manipTyped.del(manipTyped.last);                          /// del character from buffer
  return ret;
  
}


void Keyboard::addChar(ulong c, uint64 *time) {
  if(!c) return;

  /// clear old typed characters (must have stayed in buffer longer than 1 sec)
  while(charTyped.first)
    if( (*time- ((chTyped*)charTyped.last)->time)> 1000)  // if character stayed in buffer longer than 1 second, delete. IS 1 SEC OK?????
      charTyped.del(charTyped.last);
    else                                                    /// the rest of chars couldn't have stayed in buffer longer than 1 sec if the last char didn't
      break;

  /// add character to buffer
  chTyped *p= (chTyped*)charTyped.addFirst();
  p->c= c;
  p->time= *time;
}


// identical as addChar...
void Keyboard::addManip(ulong c, uint64 *time) {
  if(!c) return;

  /// clear old typed characters (must have stayed in buffer longer than 1 sec)
  while(manipTyped.first)
    if( (*time- ((chTyped*)manipTyped.last)->time)> 1000) // if character stayed in buffer longer than 1 second, delete. IS 1 SEC OK?????
      manipTyped.del(manipTyped.last);
    else                                                    /// the rest of chars couldn't have stayed in buffer longer than 1 sec if the last char didn't
      break;

  /// add character to buffer
  chTyped *p= (chTyped*)manipTyped.addFirst();
  p->c= c;
  p->time= *time;
}



void Keyboard::clearTypedBuffer() {
  while(charTyped.first)
    charTyped.del(charTyped.first);
  while(manipTyped.first)
    manipTyped.del(manipTyped.first);
}








// ------------============= JOYSTICK CLASS ===========--------------------
// ========================================================================

Joystick::Joystick() {
  mode= 0;

  #ifdef OS_LINUX
  file= -1;
  #endif /// OS_LINUX

  delData();
}

Joystick::~Joystick() {
  delData();
  #ifdef USING_DIRECTINPUT
  if(diDevice) {
    diDevice->Unacquire();
    diDevice->Release();
    diDevice= null;
  }
  #endif
}

void Joystick::delData() {
  mode= 0;

  x= y= throttle= rudder= u= v= pov= 0;

  b= buffer1;
  lastCheck= buffer2;

  /// clear buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    buffer1[a]= buffer2[a]= 0;
    bTime[a]= 0;
  }
  
  /// mark initial history buttons as checked, so they get ignored
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++)
    lastBut[a].checked= true;
  
  #ifdef OS_LINUX
  if(file!= -1)
    close(file);
  id= -1;
  name.delData();
  #endif /// OS_LINUX
}

void Joystick::swapBuffers() {
  lastCheck= b;
  b= (b== buffer1)? buffer2: buffer1;
}

void Joystick::log(const ButPressed &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastBut[a]= lastBut[a- 1];
  lastBut[0]= k;
}




// ############### JOYSTICK UPDATE #################
void Joystick::update() {
  // TYPE1 joysticks
  if(mode== 1) {
    #ifdef OS_WIN
    JOYINFOEX jinfo;
    jinfo.dwSize= sizeof(JOYINFOEX);
    joyGetPosEx(id, &jinfo);

    x= jinfo.dwXpos;                  /// main stick x axis
    y= jinfo.dwYpos;                  /// main stick y axis

    throttle= jinfo.dwZpos;           // throttle? need a joystick...
    rudder= jinfo.dwRpos;             /// rudder

    pov= jinfo.dwPOV;                 /// pov in degrees* 100
    
    /// buttons
    for(ulong a= 0; a< MAX_JOYSTICK_BUTTONS; a++)
      b[a]= jinfo.dwButtons& (1<< a);           // JOY_BUTTONXX vals hopefully wont change!!!!!

    u= jinfo.dwUpos;                  /// extra axis 5
    v= jinfo.dwVpos;                  /// extra axis 6
    #endif /// OS_WIN

    #ifdef OS_LINUX

/*
    js_event msg[64];
    int n= -1;
    
    for(short a= 0; a< 8; a++) {
      NOPE
      n= read(   &msg, sizeof(msg), 1, jf);
    
      for(short a= 0; a< 16; a++) {
        if(msg.value)
          printf("%ud", msg.value);
      }
    } /// OS_LINUX
  */  
    
    #endif /// OS_LINUX
  }
  
  // TYPE 2 JOYSTICKS
  #ifdef USING_DIRECTINPUT           /// skip some checks. only mode 1 works atm in linux
  if(mode== 2) {
  
    diDevice->GetDeviceState(sizeof(DIJOYSTATE2), (LPVOID)&diStats);
    /// left & right sticks
    x= diStats.lX;
    y= diStats.lY;
    throttle= diStats.lZ;
    rudder= diStats.lRz;
    /// POV...
    pov= diStats.rgdwPOV[0];
    /// buttons
    for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++)
      b[a]= diStats.rgbButtons[a];
    /// extra axis
    u= diStats.rglSlider[0];
    v= diStats.rglSlider[1];
  }
  #endif /// USING_DIRECTINPUT
  
  // TYPE 3 JOYSTICKS
  #ifdef USING_XINPUT           /// skip some checks. only mode 1 works atm in linux
  if(mode== 3) {
    makeme
  }
  #endif /// USING_XINPUT

}


bool Joystick::init(short mode) {
  this->mode= mode;

  if(mode== 1) {
    return true;
  }
  
  if(mode== 2) {
    #ifdef USING_DIRECTINPUT
    if(diDevice->SetDataFormat(&c_dfDIJoystick2)== DI_OK) {
      if(diDevice->SetCooperativeLevel(osi.win[0].hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND)!= DI_OK)
        error.simple("cant set cooperative level");
      return true;
    }
    #endif /// USING_DIRECTINPUT
  }
  
  if(mode== 3) {
    #ifdef USING_XINPUT
    #endif /// USING_XIMPUT
    return true;
  }

  return false;
}


#ifdef USING_DIRECTINPUT
BOOL CALLBACK EnumEffectsCallback(LPCDIEFFECTINFO di, LPVOID pvRef)
{
    //DInputFFB * ffb = (DInputFFB*) pvRef;


    // Pointer to calling device
    //ffb->ffbcaps.ConstantForce = DIEFT_GETTYPE(di->dwEffType) == DIEFT_CONSTANTFORCE;
    //ffb->ffbcaps.PeriodicForce = DIEFT_GETTYPE(di->dwEffType) == DIEFT_PERIODIC;
  
    printf(" Effect '%s'. IsConstant = %d, IsPeriodic = %d", di->tszName, DIEFT_GETTYPE(di->dwEffType) == DIEFT_CONSTANTFORCE, DIEFT_GETTYPE(di->dwEffType) == DIEFT_PERIODIC);

    return DIENUM_CONTINUE;
}
#endif






// ------------============= GAMEPAD CLASS ===========--------------------
// ========================================================================

GamePad::GamePad() {
  mode= 0;
  #ifdef USING_DIRECTINPUT
  vibration= null;
  #endif

  #ifdef OS_LINUX
  file= -1;
  #endif /// OS_LINUX
  delData();
}

GamePad::~GamePad() {
  #ifdef USING_DIRECTINPUT
  if(vibration) {
    vibration->Unload();
    vibration->Release();
    vibration= null;
  }
  /*
  if(diDevice) {
    diDevice->Unacquire();
    diDevice->Release();
    diDevice= null;
  }
  */
  #endif

  delData();
}

void GamePad::delData() {
  mode= 0;
  
  /// clear axis
  lx= ly= 0;
  rx= ry= 0;
  lt= rt= 0;
  u= v= 0;
  pov= 0;


  b= buffer1;
  lastCheck= buffer2;

  /// clear buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    buffer1[a]= buffer2[a]= 0;
    bTime[a]= 0;
  }
  
  /// mark initial history buttons as checked, so they get ignored
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++)
    lastBut[a].checked= true;
  
  #ifdef OS_LINUX
  if(file!= -1)
    close(file);
  id= -1;
  name.delData();
  #endif /// OS_LINUX
}

void GamePad::swapBuffers() {
  lastCheck= b;
  b= (b== buffer1)? buffer2: buffer1;
}

void GamePad::log(const ButPressed &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastBut[a]= lastBut[a- 1];
  lastBut[0]= k;
}




bool GamePad::aquire() {
  if(mode== 2) {
    #ifdef USING_DIRECTINPUT
    if(diDevice->Acquire()== DI_OK)
      return true;
    #endif
  }
  return false;
}


bool GamePad::unaquire() {
  if(mode== 2) {
    #ifdef USING_DIRECTINPUT
    if(diDevice->Unacquire()== DI_OK)
      return true;
    #endif
  }
  return false;
}


void GamePad::update() {
  
  if(mode== 1) {
    return;
  }
  
  #ifdef OS_WIN           /// skip some checks. only mode 1 works atm in linux
  if(mode== 2) {
  
  #ifdef USING_DIRECTINPUT
    diDevice->GetDeviceState(sizeof(DIJOYSTATE2), (LPVOID)&diStats);
    /// left & right sticks
    lx= diStats.lX;
    ly= diStats.lY;
    rx= diStats.lZ;
    ry= diStats.lRz;
    /// POV...
    pov= diStats.rgdwPOV[0];
    /// triggers
    lt= diStats.lRx;
    rt= diStats.lRy;
    /// buttons
    for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++)
      b[a]= diStats.rgbButtons[a];
    /// extra axis
    u= diStats.rglSlider[0];
    v= diStats.rglSlider[1];
    return;
  #endif /// USING_DIRECTINPUT
  
  }
  
  if(mode== 3) {
    return;
  }
  #endif /// OS_WIN
}


bool GamePad::init(short mode) {
  this->mode= mode;
  
  if(mode== 1) {
    return true;
  }
  
  if(mode== 2) {
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
    return true;	// joystick initializes, it is the same device
    /*
    if(diDevice->SetDataFormat(&c_dfDIJoystick2)== DI_OK) {
      diDevice->SetCooperativeLevel(osi.win[0].hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
      return true;
    }
    */
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN
  }
  if(mode== 3) {
    return true;
  }
  
  return false;
}







// ------------============= GAMEWHEEL CLASS ===========--------------------
// ========================================================================

GameWheel::GameWheel() {
  mode= 0;
  
  #ifdef OS_LINUX
  id= -1;
  file= null;
  #endif /// OS_LINUX
}

GameWheel::~GameWheel() {
  #ifdef USING_DIRECTINPUT
  /*
  if(diDevice) {
    diDevice->Unacquire();
    diDevice->Release();
    diDevice= null;
  }
  */
  #endif

  delData();
}

void GameWheel::delData() {
  mode= 0;
  
  /// clear axis
  wheel= 0;
  a1= a2= a3= a4= a5= 0;      // THIS NEEDS MORE WORK


  b= buffer1;
  lastCheck= buffer2;

  /// clear buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    buffer1[a]= buffer2[a]= 0;
    bTime[a]= 0;
  }
  
  /// mark initial history buttons as checked, so they get ignored
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++)
    lastBut[a].checked= true;
  
  #ifdef OS_LINUX
  if(file!= -1)
    close(file);
  id= -1;
  name.delData();
  #endif /// OS_LINUX
}

void GameWheel::swapBuffers() {
  lastCheck= b;
  b= (b== buffer1)? buffer2: buffer1;
}

void GameWheel::log(const ButPressed &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastBut[a]= lastBut[a- 1];
  lastBut[0]= k;
}





bool GameWheel::init(short mode) {
  this->mode= mode;
  
  if(mode== 1) {
    return true;
  }
  
  if(mode== 2) {
    #ifdef USING_DIRECTINPUT
    return true;		// joystick initializes it is the same device
    /*
    if(diDevice->SetDataFormat(&c_dfDIJoystick2)== DI_OK) {
      diDevice->SetCooperativeLevel(osi.win[0].hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
      return true;
    }
    */
    #endif /// USING_DIRECTINPUT
  }
  
  if(mode== 3) {
    #ifdef USING_XINPUT

    #endif /// USING_XINPUT
    return true;
  }

  return false;
}














