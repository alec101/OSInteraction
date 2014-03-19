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

#ifdef OS_MAC
/// all these are callback functions. any pad/stick/wheel added/button press / etc + some kind of egipteean hierogliph func, that should be cleaned (first one)
static CFMutableDictionaryRef hu_CreateDeviceMatchingDictionary(UInt32 inUsagePage, UInt32 inUsage);
static void HIDadded(void *, IOReturn , void *, IOHIDDeviceRef);
static void HIDremoved(void *, IOReturn , void *, IOHIDDeviceRef);
static void HIDchange(void *, IOReturn, void *, IOHIDValueRef);
//static void HIDchange(void *, IOReturn, void *, IOHIDReportType, uint32_t, uint8_t *, CFIndex);
#endif /// OS_MAC


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
  
  IOHIDManagerClose(manager, kIOHIDOptionsTypeNone); /// close the HID manager
  CFRelease(manager);                                /// delloc memory
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
  
  #ifdef OS_MAC
  
  in.manager= IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
  
  // COPY PASTE from apple 'documentation'
  
  
  /// create an array of matching dictionaries
  CFMutableArrayRef arr= CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
  
  /// create a device matching dictionary for joysticks
  CFDictionaryRef dic= hu_CreateDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
  CFArrayAppendValue(arr, dic);                 /// add it to the matching array
  CFRelease(dic);                               /// release it
  
  /// create a device matching dictionary for game pads
  dic= hu_CreateDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
  CFArrayAppendValue(arr, dic);                 /// add it to the matching array
  CFRelease(dic);                               /// release it
  
  /// create a device matching dictionary for gameWheels (hopefully)
  dic= hu_CreateDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Wheel);
  CFArrayAppendValue(arr, dic);                 /// add it to the matching array
  CFRelease(dic);                               /// release it
  
  
  /// set the created array of criterias to the manager
  IOHIDManagerSetDeviceMatchingMultiple(manager, arr);
  
  CFRelease(arr);                               // release array
  
  
  IOHIDManagerRegisterDeviceMatchingCallback(manager, HIDadded, NULL);  /// callback function for when a matching HID is added
  IOHIDManagerRegisterDeviceRemovalCallback(manager, HIDremoved, NULL); /// callback function for when a matching HID is removed
  
  // RUN LOOPS? MAYBE WON'T NEED THEM
  IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
  
  /// open the manager
  IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone); // option bits: kIOHIDOptionsTypeNone or kIOHIDOptionsTypeSeizeDevice
  
  
  /// register a callback function when a value from any registered HIDs change
  IOHIDManagerRegisterInputValueCallback(manager, HIDchange, NULL);  // this... works... if u have the enigma codebreaker (the one that didn't have the actual machine to crack the enigma code)
  //  IOHIDManagerRegisterInputReportCallback(manager, HIDchange, NULL);  // TRY REPORTS
  
  
  //continue from https://developer.apple.com/library/mac/technotes/tn2187/_index.html
  
  #endif /// OS_MAC
  
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














#ifdef OS_MAC // garbage, but this is MAC

/// NO DAMN CLUE HOW IT IS POSSIBLE TO CREATE SOMETHING LIKE THIS FOR 2 VARIABLES. ARE THEY PAYED FOR THE NUMBER OF CHARACTERS THEY TYPE?
// COPY-PASTE FROM APPLE 'DOCUMENTATION' ...lol

// function to create matching dictionary
static CFMutableDictionaryRef hu_CreateDeviceMatchingDictionary( UInt32 inUsagePage, UInt32 inUsage) {
 
  // create a dictionary to add usage page/usages to
  CFMutableDictionaryRef result= CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  // Add key for device type to refine the matching dictionary.
  CFNumberRef pageCFNumberRef= CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &inUsagePage);
  CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsagePageKey), pageCFNumberRef);
  CFRelease(pageCFNumberRef);
        
  // note: the usage is only valid if the usage page is also defined
  CFNumberRef usageCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &inUsage);
  CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsageKey), usageCFNumberRef);
  
  CFRelease(usageCFNumberRef);
  return result;
}   // hu_CreateDeviceMatchingDictionary


// this will be called when the HID Manager matches a new ( hot plugged ) HID device
static void HIDadded(void *inContext, IOReturn inResult, void *inSender, IOHIDDeviceRef inHIDDevice) {
  // inContext:   context from IOHIDManagerRegisterDeviceMatchingCallback
  // inResult:    the result of the matching operation
  // inSender:    the IOHIDManagerRef for the new device
  // inHIDDevice: the new HID device
  //IOHIDDeviceRef r; IOHIDDeviceRef
  
  printf("%s\n", __FUNCTION__);
} /// HIDadded


// this will be called when a HID device is removed ( unplugged )
static void HIDremoved(void *inContext, IOReturn inResult, void *inSender, IOHIDDeviceRef inHIDDevice) {
  // inContext:   context from IOHIDManagerRegisterDeviceMatchingCallback
  // inResult:    the result of the removing operation
  // inSender:    the IOHIDManagerRef for the device being removed
  // inHIDDevice: the removed HID device

  printf("%s\n", __FUNCTION__);
} /// HIDremoved

/// any value change from any HIDs -> this callback func
static void HIDchange(void *inContext, IOReturn inResult, void *inSender, IOHIDValueRef val) {
  // inContext:       context from IOHIDManagerRegisterInputValueCallback
  // IinResult:       completion result for the input value operation
  // inSender:        the IOHIDManagerRef
  // inIOHIDValueRef: the new element value
  printf("%s", __FUNCTION__);
  
  
  IOHIDElementRef elem= IOHIDValueGetElement(val);      /// get the 'element' thet of the value
  IOHIDDeviceRef device= IOHIDElementGetDevice(elem);   /// get the HID device that a element belongs to
  int v= IOHIDValueGetIntegerValue(val);                /// the actual value that changed
  
  
  CFTypeID id= IOHIDElementGetTypeID();                 // crap


  // IOHIDElementCookie represent a unique identifier for a HID element within a HID device.
  IOHIDElementCookie cookie= IOHIDElementGetCookie(elem);
  
  // return the collection type:
  //  kIOHIDElementTypeInput_Misc         = 1,
  //  kIOHIDElementTypeInput_Button       = 2,
  //  kIOHIDElementTypeInput_Axis         = 3,
  //  kIOHIDElementTypeInput_ScanCodes    = 4,
  //  kIOHIDElementTypeOutput             = 129,
  //  kIOHIDElementTypeFeature            = 257,
  //  kIOHIDElementTypeCollection         = 513
  IOHIDElementType tType = IOHIDElementGetType(elem);
  
  // If the HID element type is of type kIOHIDElementTypeCollection then
  // the collection type is one of:
  //  kIOHIDElementCollectionTypePhysical         = 0x00,
  //  kIOHIDElementCollectionTypeApplication      = 0x01,
  //  kIOHIDElementCollectionTypeLogical          = 0x02,
  //  kIOHIDElementCollectionTypeReport           = 0x03,
  //  kIOHIDElementCollectionTypeNamedArray       = 0x04,
  //  kIOHIDElementCollectionTypeUsageSwitch      = 0x05,
  //  kIOHIDElementCollectionTypeUsageModifier    = 0x06
  IOHIDElementCollectionType cType= IOHIDElementGetCollectionType(elem);
  
  // usage and usage pages are defined on the USB website at: <http://www.usb.org>
  uint32_t page = IOHIDElementGetUsagePage(elem);
  uint32_t usage = IOHIDElementGetUsage(elem);
  
  // Boolean properties
  Boolean isVirtual = IOHIDElementIsVirtual(elem);
  Boolean isRelative = IOHIDElementIsRelative(elem);
  Boolean isWrapping = IOHIDElementIsWrapping(elem);
  Boolean isArray = IOHIDElementIsArray(elem);
  Boolean isNonLinear = IOHIDElementIsNonLinear(elem);
  Boolean hasPreferred = IOHIDElementHasPreferredState(elem);
  Boolean hasNullState = IOHIDElementHasNullState(elem);
  
  // the HID element name
  CFStringRef name= IOHIDElementGetName(elem);
  char buffer[500];  
  
  if(name) {
    for(short b= 0; b<500; b++)
      buffer[b]= 0;
    CFStringGetCString(name, buffer, 499, CFStringGetSystemEncoding());
  }
  
  // element report information
  uint32_t reportID = IOHIDElementGetReportID(elem);
  uint32_t reportSize = IOHIDElementGetReportSize(elem);
  uint32_t reportCount = IOHIDElementGetReportCount(elem);
  
  // element unit & exponent
  uint32_t unit = IOHIDElementGetUnit(elem);
  uint32_t unitExp = IOHIDElementGetUnitExponent(elem);
  
  // logical & physical minimums & maximums
  CFIndex logicalMin = IOHIDElementGetLogicalMin(elem);
  CFIndex logicalMax = IOHIDElementGetLogicalMax(elem);
  CFIndex physicalMin = IOHIDElementGetPhysicalMin(elem);
  CFIndex physicalMax = IOHIDElementGetPhysicalMax(elem);
  
  
  
  //There are also functions to determine the device, parent, and child of a specified HID element:
  
  
  // return the collection element that a HID element belongs to (if any)
  IOHIDElementRef parent= IOHIDElementGetParent(elem);
  
  // return the child elements of a collection element (if any)
  CFArrayRef tCFArrayRef= IOHIDElementGetChildren(elem);
  
  
   go for struct for each joystick 
     each cookie = button or axis
     switch (cookie) {
         
     }
  //  printf(" typeID[%lu]", id);
  printf(" name[%s] cookie[%u] tType[%u] cType[%u] page[%ld] usage[%ld]\n", buffer, cookie, tType, cType, page, usage);
    
  //  printf(" element[%d] value[%d]\n", elem, v);
  
  /*
  switch(type) {
    case kIOHIDElementTypeInput_Misc:     /// gamepad axis go here, actually everything but buttons go here, ffs
      printf(" misc...\n");
      ///      IOHIDElement_GetDoubleProperty(element, CFSTR(kIOHIDElementCalibrationMaxKey), &calibrateMax);
      
      break;
    case kIOHIDElementTypeInput_Button:     /// some buttons go here (not the d-pad)
      printf(" button\n");
      break;
    case kIOHIDElementTypeInput_Axis:     /// full of NO DATA AT ALL HERE
      printf(" axis\n");
      break;
    default:
      printf(" other stuff\n");
  };
  */
  
  /// Returns the timestamp value associated with this HID value reference.
  //uint64_t IOHIDValueGetTimeStamp(IOHIDValueRef inIOHIDValueRef);
  
  /// Returns an integer representation for this HID value reference.
  //  CFIndex IOHIDValueGetIntegerValue(IOHIDValueRef inIOHIDValueRef);
  
  
} /// Handle_IOHIDInputValueCallback


/*
static void HIDchange(void *context, IOReturn result, void *sender, IOHIDReportType type, uint32_t reportID, uint8_t *report, CFIndex reportLength) {
  // context:       void * pointer to your data, often a pointer to an object.
  // result:        Completion result of desired operation.
  // refcon:        void * pointer to more data.
  // sender:        Interface instance sending the completion routine.
  // type:          The type of the report that was completed.
  // reportID:      The ID of the report that was completed.
  // report:        Pointer to the buffer containing the contents of the report.
  // reportLength:  Size of the buffer received upon completion.

  printf("%s", __FUNCTION__);
  printf(" type[%lu] reportID[%u] reportLength[%d] report[%s]\n", type, reportID, reportLength, report);
}
*/
#endif


/*
 Copyright (c) 2013 Alex Diener
 
 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 
 Alex Diener adiener@sacredsoftware.net
 */
/*
 
#include "gamepad/Gamepad.h"
#include "gamepad/Gamepad_private.h"
#include <IOKit/hid/IOHIDLib.h>
#include <limits.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

struct HIDGamepadAxis {
	IOHIDElementCookie cookie;
	CFIndex logicalMin;
	CFIndex logicalMax;
	bool hasNullState;
	bool isHatSwitch;
	bool isHatSwitchSecondAxis;
};

struct HIDGamepadButton {
	IOHIDElementCookie cookie;
};

struct Gamepad_devicePrivate {
	IOHIDDeviceRef deviceRef;
	struct HIDGamepadAxis * axisElements;
	struct HIDGamepadButton * buttonElements;
};

struct Gamepad_queuedEvent {
	unsigned int deviceID;
	enum Gamepad_eventType eventType;
	void * eventData;
};

static IOHIDManagerRef hidManager = NULL;
static struct Gamepad_device ** devices = NULL;
static unsigned int numDevices = 0;
static unsigned int nextDeviceID = 0;

static struct Gamepad_queuedEvent * inputEventQueue = NULL;
static size_t inputEventQueueSize = 0;
static size_t inputEventCount = 0;

static struct Gamepad_queuedEvent * deviceEventQueue = NULL;
static size_t deviceEventQueueSize = 0;
static size_t deviceEventCount = 0;

static void hatValueToXY(CFIndex value, CFIndex range, int * outX, int * outY) {
	if (value == range) {
		*outX = *outY = 0;
		
	} else {
		if (value > 0 && value < range / 2) {
			*outX = 1;
			
		} else if (value > range / 2) {
			*outX = -1;
			
		} else {
			*outX = 0;
		}
		
		if (value > range / 4 * 3 || value < range / 4) {
			*outY = -1;
			
		} else if (value > range / 4 && value < range / 4 * 3) {
			*outY = 1;
			
		} else {
			*outY = 0;
		}
	}
}

static void queueInputEvent(unsigned int deviceID, enum Gamepad_eventType eventType, void * eventData) {
	struct Gamepad_queuedEvent queuedEvent;
	
	queuedEvent.deviceID = deviceID;
	queuedEvent.eventType = eventType;
	queuedEvent.eventData = eventData;
	
	if (inputEventCount >= inputEventQueueSize) {
		inputEventQueueSize = inputEventQueueSize == 0 ? 1 : inputEventQueueSize * 2;
		inputEventQueue = realloc(inputEventQueue, sizeof(struct Gamepad_queuedEvent) * inputEventQueueSize);
	}
	inputEventQueue[inputEventCount++] = queuedEvent;
}

static void queueAxisEvent(struct Gamepad_device * device, double timestamp, unsigned int axisID, float value, float lastValue) {
	struct Gamepad_axisEvent * axisEvent;
	
	axisEvent = malloc(sizeof(struct Gamepad_axisEvent));
	axisEvent->device = device;
	axisEvent->timestamp = timestamp;
	axisEvent->axisID = axisID;
	axisEvent->value = value;
	axisEvent->lastValue = lastValue;
	
	queueInputEvent(device->deviceID, GAMEPAD_EVENT_AXIS_MOVED, axisEvent);
}

static void queueButtonEvent(struct Gamepad_device * device, double timestamp, unsigned int buttonID, bool down) {
	struct Gamepad_buttonEvent * buttonEvent;
	
	buttonEvent = malloc(sizeof(struct Gamepad_buttonEvent));
	buttonEvent->device = device;
	buttonEvent->timestamp = timestamp;
	buttonEvent->buttonID = buttonID;
	buttonEvent->down = down;
	
	queueInputEvent(device->deviceID, down ? GAMEPAD_EVENT_BUTTON_DOWN : GAMEPAD_EVENT_BUTTON_UP, buttonEvent);
}

static void onDeviceValueChanged(void * context, IOReturn result, void * sender, IOHIDValueRef value) {
	struct Gamepad_device * deviceRecord;
	struct Gamepad_devicePrivate * hidDeviceRecord;
	IOHIDElementRef element;
	IOHIDElementCookie cookie;
	unsigned int axisIndex, buttonIndex;
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	
	deviceRecord = context;
	hidDeviceRecord = deviceRecord->privateData;
	element = IOHIDValueGetElement(value);
	cookie = IOHIDElementGetCookie(element);
	
	for (axisIndex = 0; axisIndex < deviceRecord->numAxes; axisIndex++) {
		if (!hidDeviceRecord->axisElements[axisIndex].isHatSwitchSecondAxis &&
		    hidDeviceRecord->axisElements[axisIndex].cookie == cookie) {
			CFIndex integerValue;
			
			if (IOHIDValueGetLength(value) > 4) {
				// Workaround for a strange crash that occurs with PS3 controller; was getting lengths of 39 (!)
				continue;
			}
			integerValue = IOHIDValueGetIntegerValue(value);
			
			if (hidDeviceRecord->axisElements[axisIndex].isHatSwitch) {
				int x, y;
				
				// Fix for Saitek X52
				if (!hidDeviceRecord->axisElements[axisIndex].hasNullState) {
					if (integerValue < hidDeviceRecord->axisElements[axisIndex].logicalMin) {
						integerValue = hidDeviceRecord->axisElements[axisIndex].logicalMax - hidDeviceRecord->axisElements[axisIndex].logicalMin + 1;
					} else {
						integerValue--;
					}
				}
				
				hatValueToXY(integerValue, hidDeviceRecord->axisElements[axisIndex].logicalMax - hidDeviceRecord->axisElements[axisIndex].logicalMin + 1, &x, &y);
				
				if (x != deviceRecord->axisStates[axisIndex]) {
					queueAxisEvent(deviceRecord,
					               IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001,
					               axisIndex,
					               x,
					               deviceRecord->axisStates[axisIndex]);
					
					deviceRecord->axisStates[axisIndex] = x;
				}
				
				if (y != deviceRecord->axisStates[axisIndex + 1]) {
					queueAxisEvent(deviceRecord,
					               IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001,
					               axisIndex + 1,
					               y,
					               deviceRecord->axisStates[axisIndex + 1]);
					
					deviceRecord->axisStates[axisIndex + 1] = y;
				}
				
			} else {
				float floatValue;
				
				if (integerValue < hidDeviceRecord->axisElements[axisIndex].logicalMin) {
					hidDeviceRecord->axisElements[axisIndex].logicalMin = integerValue;
				}
				if (integerValue > hidDeviceRecord->axisElements[axisIndex].logicalMax) {
					hidDeviceRecord->axisElements[axisIndex].logicalMax = integerValue;
				}
				floatValue = (integerValue - hidDeviceRecord->axisElements[axisIndex].logicalMin) / (float) (hidDeviceRecord->axisElements[axisIndex].logicalMax - hidDeviceRecord->axisElements[axisIndex].logicalMin) * 2.0f - 1.0f;
				
				queueAxisEvent(deviceRecord,
				               IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001,
				               axisIndex,
				               floatValue,
				               deviceRecord->axisStates[axisIndex]);
				
				deviceRecord->axisStates[axisIndex] = floatValue;
			}
			
			return;
		}
	}
	
	for (buttonIndex = 0; buttonIndex < deviceRecord->numButtons; buttonIndex++) {
		if (hidDeviceRecord->buttonElements[buttonIndex].cookie == cookie) {
			bool down;
			
			down = IOHIDValueGetIntegerValue(value);
			queueButtonEvent(deviceRecord,
			                 IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001,
			                 buttonIndex,
			                 down);
			
			deviceRecord->buttonStates[buttonIndex] = down;
			
			return;
		}
	}
}

static int IOHIDDeviceGetIntProperty(IOHIDDeviceRef deviceRef, CFStringRef key) {
	CFTypeRef typeRef;
	int value;
	
	typeRef = IOHIDDeviceGetProperty(deviceRef, key);
	if (typeRef == NULL || CFGetTypeID(typeRef) != CFNumberGetTypeID()) {
		return 0;
	}
	
	CFNumberGetValue((CFNumberRef) typeRef, kCFNumberSInt32Type, &value);
	return value;
}

static int IOHIDDeviceGetVendorID(IOHIDDeviceRef deviceRef) {
	return IOHIDDeviceGetIntProperty(deviceRef, CFSTR(kIOHIDVendorIDKey));
}

static int IOHIDDeviceGetProductID(IOHIDDeviceRef deviceRef) {
	return IOHIDDeviceGetIntProperty(deviceRef, CFSTR(kIOHIDProductIDKey));
}

static void onDeviceMatched(void * context, IOReturn result, void * sender, IOHIDDeviceRef device) {
	CFArrayRef elements;
	CFIndex elementIndex;
	IOHIDElementRef element;
	CFStringRef cfProductName;
	struct Gamepad_device * deviceRecord;
	struct Gamepad_devicePrivate * hidDeviceRecord;
	IOHIDElementType type;
	char * description;
	struct Gamepad_queuedEvent queuedEvent;
	
	deviceRecord = malloc(sizeof(struct Gamepad_device));
	deviceRecord->deviceID = nextDeviceID++;
	deviceRecord->vendorID = IOHIDDeviceGetVendorID(device);
	deviceRecord->productID = IOHIDDeviceGetProductID(device);
	deviceRecord->numAxes = 0;
	deviceRecord->numButtons = 0;
	devices = realloc(devices, sizeof(struct Gamepad_device *) * (numDevices + 1));
	devices[numDevices++] = deviceRecord;
	
	hidDeviceRecord = malloc(sizeof(struct Gamepad_devicePrivate));
	hidDeviceRecord->deviceRef = device;
	hidDeviceRecord->axisElements = NULL;
	hidDeviceRecord->buttonElements = NULL;
	deviceRecord->privateData = hidDeviceRecord;
	
	cfProductName = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
	if (cfProductName == NULL || CFGetTypeID(cfProductName) != CFStringGetTypeID()) {
		description = malloc(strlen("[Unknown]" + 1));
		strcpy(description, "[Unknown]");
		
	} else {
		CFIndex length;
		
		CFStringGetBytes(cfProductName, CFRangeMake(0, CFStringGetLength(cfProductName)), kCFStringEncodingUTF8, '?', false, NULL, 100, &length);
		description = malloc(length + 1);
		CFStringGetBytes(cfProductName, CFRangeMake(0, CFStringGetLength(cfProductName)), kCFStringEncodingUTF8, '?', false, (UInt8 *) description, length + 1, NULL);
		description[length] = '\x00';
	}
	deviceRecord->description = description;
	
	elements = IOHIDDeviceCopyMatchingElements(device, NULL, kIOHIDOptionsTypeNone);
	for (elementIndex = 0; elementIndex < CFArrayGetCount(elements); elementIndex++) {
		element = (IOHIDElementRef) CFArrayGetValueAtIndex(elements, elementIndex);
		type = IOHIDElementGetType(element);
		
		// All of the axis elements I've ever detected have been kIOHIDElementTypeInput_Misc. kIOHIDElementTypeInput_Axis is only included for good faith...
		if (type == kIOHIDElementTypeInput_Misc ||
		    type == kIOHIDElementTypeInput_Axis) {
			
			hidDeviceRecord->axisElements = realloc(hidDeviceRecord->axisElements, sizeof(struct HIDGamepadAxis) * (deviceRecord->numAxes + 1));
			hidDeviceRecord->axisElements[deviceRecord->numAxes].cookie = IOHIDElementGetCookie(element);
			hidDeviceRecord->axisElements[deviceRecord->numAxes].logicalMin = IOHIDElementGetLogicalMin(element);
			hidDeviceRecord->axisElements[deviceRecord->numAxes].logicalMax = IOHIDElementGetLogicalMax(element);
			hidDeviceRecord->axisElements[deviceRecord->numAxes].hasNullState = !!IOHIDElementHasNullState(element);
			hidDeviceRecord->axisElements[deviceRecord->numAxes].isHatSwitch = IOHIDElementGetUsage(element) == kHIDUsage_GD_Hatswitch;
			hidDeviceRecord->axisElements[deviceRecord->numAxes].isHatSwitchSecondAxis = false;
			deviceRecord->numAxes++;
			
			if (hidDeviceRecord->axisElements[deviceRecord->numAxes - 1].isHatSwitch) {
				hidDeviceRecord->axisElements = realloc(hidDeviceRecord->axisElements, sizeof(struct HIDGamepadAxis) * (deviceRecord->numAxes + 1));
				hidDeviceRecord->axisElements[deviceRecord->numAxes].isHatSwitchSecondAxis = true;
				deviceRecord->numAxes++;
			}
			
		} else if (type == kIOHIDElementTypeInput_Button) {
			hidDeviceRecord->buttonElements = realloc(hidDeviceRecord->buttonElements, sizeof(struct HIDGamepadButton) * (deviceRecord->numButtons + 1));
			hidDeviceRecord->buttonElements[deviceRecord->numButtons].cookie = IOHIDElementGetCookie(element);
			deviceRecord->numButtons++;
		}
	}
	CFRelease(elements);
	
	deviceRecord->axisStates = calloc(sizeof(float), deviceRecord->numAxes);
	deviceRecord->buttonStates = calloc(sizeof(bool), deviceRecord->numButtons);
	
	IOHIDDeviceRegisterInputValueCallback(device, onDeviceValueChanged, deviceRecord);
	
	queuedEvent.deviceID = deviceRecord->deviceID;
	queuedEvent.eventType = GAMEPAD_EVENT_DEVICE_ATTACHED;
	queuedEvent.eventData = deviceRecord;
	
	if (deviceEventCount >= deviceEventQueueSize) {
		deviceEventQueueSize = deviceEventQueueSize == 0 ? 1 : deviceEventQueueSize * 2;
		deviceEventQueue = realloc(deviceEventQueue, sizeof(struct Gamepad_queuedEvent) * deviceEventQueueSize);
	}
	deviceEventQueue[deviceEventCount++] = queuedEvent;
}

static void disposeDevice(struct Gamepad_device * deviceRecord) {
	unsigned int inputEventIndex, deviceEventIndex;
	
	IOHIDDeviceRegisterInputValueCallback(((struct Gamepad_devicePrivate *) deviceRecord->privateData)->deviceRef, NULL, NULL);
	
	for (inputEventIndex = 0; inputEventIndex < inputEventCount; inputEventIndex++) {
		if (inputEventQueue[inputEventIndex].deviceID == deviceRecord->deviceID) {
			unsigned int inputEventIndex2;
			
			free(inputEventQueue[inputEventIndex].eventData);
			inputEventCount--;
			for (inputEventIndex2 = inputEventIndex; inputEventIndex2 < inputEventCount; inputEventIndex2++) {
				inputEventQueue[inputEventIndex2] = inputEventQueue[inputEventIndex2 + 1];
			}
			inputEventIndex--;
		}
	}
	
	for (deviceEventIndex = 0; deviceEventIndex < deviceEventCount; deviceEventIndex++) {
		if (deviceEventQueue[deviceEventIndex].deviceID == deviceRecord->deviceID) {
			unsigned int deviceEventIndex2;
			
			deviceEventCount--;
			for (deviceEventIndex2 = deviceEventIndex; deviceEventIndex2 < deviceEventCount; deviceEventIndex2++) {
				deviceEventQueue[deviceEventIndex2] = deviceEventQueue[deviceEventIndex2 + 1];
			}
			deviceEventIndex--;
		}
	}
	
	free(((struct Gamepad_devicePrivate *) deviceRecord->privateData)->axisElements);
	free(((struct Gamepad_devicePrivate *) deviceRecord->privateData)->buttonElements);
	free(deviceRecord->privateData);
	
	free((void *) deviceRecord->description);
	free(deviceRecord->axisStates);
	free(deviceRecord->buttonStates);
	
	free(deviceRecord);
}

static void onDeviceRemoved(void * context, IOReturn result, void * sender, IOHIDDeviceRef device) {
	unsigned int deviceIndex;
	
	for (deviceIndex = 0; deviceIndex < numDevices; deviceIndex++) {
		if (((struct Gamepad_devicePrivate *) devices[deviceIndex]->privateData)->deviceRef == device) {
			if (Gamepad_deviceRemoveCallback != NULL) {
				Gamepad_deviceRemoveCallback(devices[deviceIndex], Gamepad_deviceRemoveContext);
			}
			
			disposeDevice(devices[deviceIndex]);
			numDevices--;
			for (; deviceIndex < numDevices; deviceIndex++) {
				devices[deviceIndex] = devices[deviceIndex + 1];
			}
			return;
		}
	}
}

#define GAMEPAD_RUN_LOOP_MODE CFSTR("GamepadRunLoopMode")

void Gamepad_init() {
	if (hidManager == NULL) {
		CFStringRef keys[2];
		int value;
		CFNumberRef values[2];
		CFDictionaryRef dictionaries[3];
		CFArrayRef array;
		
		hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
		
		keys[0] = CFSTR(kIOHIDDeviceUsagePageKey);
		keys[1] = CFSTR(kIOHIDDeviceUsageKey);
		
		value = kHIDPage_GenericDesktop;
		values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		value = kHIDUsage_GD_Joystick;
		values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		dictionaries[0] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFRelease(values[0]);
		CFRelease(values[1]);
		
		value = kHIDPage_GenericDesktop;
		values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		value = kHIDUsage_GD_GamePad;
		values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		dictionaries[1] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFRelease(values[0]);
		CFRelease(values[1]);
		
		value = kHIDPage_GenericDesktop;
		values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		value = kHIDUsage_GD_MultiAxisController;
		values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		dictionaries[2] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFRelease(values[0]);
		CFRelease(values[1]);
		
		array = CFArrayCreate(kCFAllocatorDefault, (const void **) dictionaries, 3, &kCFTypeArrayCallBacks);
		CFRelease(dictionaries[0]);
		CFRelease(dictionaries[1]);
		CFRelease(dictionaries[2]);
		IOHIDManagerSetDeviceMatchingMultiple(hidManager, array);
		CFRelease(array);
		
		IOHIDManagerRegisterDeviceMatchingCallback(hidManager, onDeviceMatched, NULL);
		IOHIDManagerRegisterDeviceRemovalCallback(hidManager, onDeviceRemoved, NULL);
		
		IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
		
		// Force gamepads to be recognized immediately. The normal run loop mode takes a few frames,
		// but we can run one iteration with a custom mode to do it without a delay.
		IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), GAMEPAD_RUN_LOOP_MODE);
		CFRunLoopRunInMode(GAMEPAD_RUN_LOOP_MODE, 0, true);
		IOHIDManagerUnscheduleFromRunLoop(hidManager, CFRunLoopGetCurrent(), GAMEPAD_RUN_LOOP_MODE);
		
		IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	}
}

void Gamepad_shutdown() {
	if (hidManager != NULL) {
		unsigned int deviceIndex;
		
		IOHIDManagerUnscheduleFromRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
		
		for (deviceIndex = 0; deviceIndex < numDevices; deviceIndex++) {
			disposeDevice(devices[deviceIndex]);
		}
		free(devices);
		devices = NULL;
		numDevices = 0;
		
		IOHIDManagerClose(hidManager, 0);
		CFRelease(hidManager);
		hidManager = NULL;
	}
}

unsigned int Gamepad_numDevices() {
	return numDevices;
}

struct Gamepad_device * Gamepad_deviceAtIndex(unsigned int deviceIndex) {
	if (deviceIndex >= numDevices) {
		return NULL;
	}
	return devices[deviceIndex];
}

static void processQueuedEvent(struct Gamepad_queuedEvent event) {
	switch (event.eventType) {
		case GAMEPAD_EVENT_DEVICE_ATTACHED:
			if (Gamepad_deviceAttachCallback != NULL) {
				Gamepad_deviceAttachCallback(event.eventData, Gamepad_deviceAttachContext);
			}
			break;
			
		case GAMEPAD_EVENT_DEVICE_REMOVED:
			if (Gamepad_deviceRemoveCallback != NULL) {
				Gamepad_deviceRemoveCallback(event.eventData, Gamepad_deviceRemoveContext);
			}
			break;
			
		case GAMEPAD_EVENT_BUTTON_DOWN:
			if (Gamepad_buttonDownCallback != NULL) {
				struct Gamepad_buttonEvent * buttonEvent = event.eventData;
				Gamepad_buttonDownCallback(buttonEvent->device, buttonEvent->buttonID, buttonEvent->timestamp, Gamepad_buttonDownContext);
			}
			break;
			
		case GAMEPAD_EVENT_BUTTON_UP:
			if (Gamepad_buttonUpCallback != NULL) {
				struct Gamepad_buttonEvent * buttonEvent = event.eventData;
				Gamepad_buttonUpCallback(buttonEvent->device, buttonEvent->buttonID, buttonEvent->timestamp, Gamepad_buttonUpContext);
			}
			break;
			
		case GAMEPAD_EVENT_AXIS_MOVED:
			if (Gamepad_axisMoveCallback != NULL) {
				struct Gamepad_axisEvent * axisEvent = event.eventData;
				Gamepad_axisMoveCallback(axisEvent->device, axisEvent->axisID, axisEvent->value, axisEvent->lastValue, axisEvent->timestamp, Gamepad_axisMoveContext);
			}
			break;
	}
}

void Gamepad_detectDevices() {
	unsigned int eventIndex;
	
	if (hidManager == NULL) {
		return;
	}
	
	for (eventIndex = 0; eventIndex < deviceEventCount; eventIndex++) {
		processQueuedEvent(deviceEventQueue[eventIndex]);
	}
	deviceEventCount = 0;
}

void Gamepad_processEvents() {
	unsigned int eventIndex;
	static bool inProcessEvents;
	
	if (hidManager == NULL || inProcessEvents) {
		return;
	}
	
	inProcessEvents = true;
	for (eventIndex = 0; eventIndex < inputEventCount; eventIndex++) {
		processQueuedEvent(inputEventQueue[eventIndex]);
		free(inputEventQueue[eventIndex].eventData);
	}
	inputEventCount = 0;
	inProcessEvents = false;
}



*/






