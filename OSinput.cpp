#include "OSInteraction.h"

extern bool chatty;

#ifdef OS_LINUX
//#include <linux/joystick.h>   // it's not x stuff... lots of crap added, keyboard/mouse, that is not needed. IT'S POSSIBLE TO AVOID THIS HEADER, only some function definitions are needed.
#include <fcntl.h>
#include <sys/ioctl.h>

// avoid including joystick.h, wich includes tons of unneeded stuff
typedef unsigned char __u8;
#define JS_EVENT_BUTTON 0x01                              // button pressed/released
#define JS_EVENT_AXIS   0x02                              // joystick moved
#define JS_EVENT_INIT   0x80
#define JSIOCGVERSION		_IOR('j', 0x01, __u8)             // get driver version
#define JSIOCGAXES      _IOR('j', 0x11, __u8)             // get number of axes
#define JSIOCGBUTTONS   _IOR('j', 0x12, __u8)             // get number of buttons
#define JSIOCGNAME(len) _IOC(_IOC_READ, 'j', 0x13, len)   // get identifier string
#define EVIOCGNAME(len)		_IOC(_IOC_READ, 'E', 0x06, len) // get device name
struct js_event {
  unsigned int time;    // event timestamp in milliseconds
  short value;          // value
  unsigned char type;   // event type
  unsigned char number;	// axis/button number
};
#endif /// OS_LINUX

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

// private funcs
void checkGamepadType(GamePad *p);  // it is found in the gamepad area, at the end
  
/* TODO:
 *
 * buttons to be on 32 bit integers? 1<< a is button a? good for copy-ing, 
 *
 * [win][mac] gamepad unification - check linux version
 *
 * [all]: pov must be in degrees * 100
 *
 * every COORDONATE UNIFICATION: x0,y0 -> left,bottom (as in OpenGL, MAC.  NOT AS IN: windows, (i think linux too)
      this includes mouseWheel (+y means UP, as in the real cood system)
 * 
 * could set, that if program chooses Direct Input mode, under linux/mac to choose MODE 1 <<<<<<<<<
 *   or basically, if a mode is unavaible, go for mode 1 - maybe m.update() / k.update() to simply return under linux/mac (if cant make mode 2/3 work
 * 
 * 
 * i think every time variable should be int64... dunno for shure. nanosecs are int64, mili= nano* 1,000,000... still way more data to hold in a int64
 *    they may be a little slower... dunno, but not by much
 *
 * buy a ffeedback joystick
 * buy a wheel
 * 
 * [linux] Input::populate() joystick name might be null
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
  #ifdef USING_DIRECTINPUT // direct input vibration
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
    long hr= 0;
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
        //hr= j[8].diDevice->CreateEffect( GUID_ConstantForce, &eff, &j[8].vibration, NULL );
        //^^ further thinkin needed. gp vibrate? how about the stick? so same vibrate for all, right?



        

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
  #endif /// USING_DIRECTINPUT


}


#ifdef USING_DIRECTINPUT
BOOL CALLBACK EnumEffectsCallback(LPCDIEFFECTINFO di, LPVOID pvRef) {
    //DInputFFB * ffb = (DInputFFB*) pvRef;


    // Pointer to calling device
    //ffb->ffbcaps.ConstantForce = DIEFT_GETTYPE(di->dwEffType) == DIEFT_CONSTANTFORCE;
    //ffb->ffbcaps.PeriodicForce = DIEFT_GETTYPE(di->dwEffType) == DIEFT_PERIODIC;
  
    printf(" Effect '%s'. IsConstant = %d, IsPeriodic = %d", di->tszName, DIEFT_GETTYPE(di->dwEffType) == DIEFT_CONSTANTFORCE, DIEFT_GETTYPE(di->dwEffType) == DIEFT_PERIODIC);

    return DIENUM_CONTINUE;
}
#endif

/// end testing
















#ifdef OS_MAC
/// all these are callback functions. any pad/stick/wheel added/button press / etc + some kind of egipteean hierogliph func, that should be cleaned (first one)
static CFMutableDictionaryRef hu_CreateDeviceMatchingDictionary(UInt32 inUsagePage, UInt32 inUsage);
static void HIDadded(void *, IOReturn , void *, IOHIDDeviceRef);
static void HIDremoved(void *, IOReturn , void *, IOHIDDeviceRef);
void HIDchange(void *, IOReturn, void *, IOHIDValueRef);
//static void HIDchange(void *, IOReturn, void *, IOHIDReportType, uint32_t, uint8_t *, CFIndex);
#endif /// OS_MAC




// ------------============= INPUT CLASS ===========--------------------
// =====================================================================

Input::Input() {
  nr.jFound= nr.gpFound= nr.gwFound= 0;
  
  nr.jOS= nr.gpOS= nr.gwOS= 0;
  nr.jT2= nr.gpT2= nr.gwT2= 0;
  nr.jT3= nr.gpT3= nr.gwT3= 0;
  
  /// links between each stick/pad/wheel; the real difference is how they use what they read from the driver
  for(short a= 0; a< 20; a++) {
    j[a]._gp= &gp[a];
    j[a]._gw= &gw[a];
    gp[a]._j= &j[a];
    gw[a]._j= &j[a];
  }

  mode1Name= "System Default";        /// 'driver' name / description

  #ifdef OS_WIN
  mode2Name= "Direct Input";
  mode3Name= "XInput";
  #endif /// OS_WIN

  #ifdef OS_LINUX
  mode2Name= mode3Name= "Not Used";
  #endif /// OS_LINUX

  #ifdef OS_MAC
  mode2Name= mode3Name= "Not Used";
  k.numLock= true;    /// macs don't handle num locks. this will always be on
  #endif /// OS_MAC

  #ifdef USING_DIRECTINPUT
  dInput= null;
  #endif /// USING_DIRECTINPUT

}


Input::~Input() {
  #ifdef USING_DIRECTINPUT
  if(dInput)
    dInput->Release();
  dInput= null;
  #endif

  #ifdef OS_MAC
  IOHIDManagerClose(manager, kIOHIDOptionsTypeNone); /// close the HID manager
  CFRelease(manager);                                /// delloc memory
  #endif /// OS_MAC
}


void Input::delData() {
  m.delData();
  k.delData();
  short a;
  for(a= 0; a< MAX_JOYSTICKS; a++)
    j[a].delData();
  for(a= 0; a< MAX_JOYSTICKS; a++)
    gp[a].delData();
  for(a= 0; a< MAX_JOYSTICKS; a++)
    gw[a].delData();

  nr.jFound= nr.gpFound= nr.gwFound= 0;
  nr.jOS= nr.gpOS= nr.gwOS= 0;
  nr.jT2= nr.gpT2= nr.gwT2= 0;
  nr.jT3= nr.gpT3= nr.gwT3= 0;
}


// ############ Input::init() - MUST call @ PROGRAM START ##############
/// set mouse & keyboard mode with this func too (better leave on default)
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

  #ifdef OS_MAC
  
  in.manager= IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);

  // https://developer.apple.com/library/mac/technotes/tn2187/_index.html
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
  
  /// create a device matching dictionary for whatever multi axis controller
  dic= hu_CreateDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);
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
  
  //continue from 
  
  #endif /// OS_MAC
  
  return true;
}



///=============================================================================
// ########################### INPUT POPULATE ##################################
///=============================================================================

/// this func can be called any time to scan for joysticks/pads/wheels; >>> note, it has a big drag of 5-10 millisecs!!! <<<
bool jConnected[8];
void Input::populate(bool scanMouseKeyboard) {
  /// debug
  uint64 start, end;
  bool timer= false;

  lastPopulate= osi.present;

  // DO NOT SCAN FOR MOUSE & KEYBOARD EACH SECOND !!!
  if(scanMouseKeyboard) {

    // ---=== MOUSE SCAN ===---
    if(m.mode== 3) {
      m.delData();
      if(!m.init(3))        /// try to use direct input
        m.init(2);          /// else use defaul windows functions
    } else
      m.init(m.mode);

    // ---=== KEYBOARD SCAN ===---
    if(k.mode== 3) {
      k.delData();
      if(!k.init(3))
        k.init(2);
    }
  } /// scanMouseKeyboard


  /// all joys/gp/gw will have same directinput/xinput/os drivers

  // ------------============ MODE 1 JOYSTICKS ===============------------
  #ifdef OS_WIN
  
  /* CRASHES TO DESKTOP, USES DIRECT INPUT, BUGS IN READING = SCRAP

  /// if system reports 0 sticks, and currently no OS sticks are in use, just skip searching;
  if(joyGetNumDevs()<= 0)
    if(!nr.jOS)
      goto skipWinOSsearch;
  
  JOYCAPS caps;
  JOYINFOEX jinfo;
  jinfo.dwSize= sizeof(JOYINFOEX);

  for(short a= 0; a< 8; a++)
    if(joyGetPosEx(a, &jinfo) == JOYERR_NOERROR) {
      if(!j[a].mode) {                /// a new joystick got plugged in
        /// get important data from joyGetDevCaps
        for(short b= 0; b< sizeof(caps); b++) ((char *)&caps)[b]= 0;      /// clear caps
        
        joyGetDevCaps(a, &caps, sizeof(caps));
        
        j[a].name= (caps.szPname[0]==0)? "Unknown device": caps.szPname;
        gp[a].name= j[a].name;
        gw[a].name= j[a].name;

        j[a].maxButtons= caps.wMaxButtons;
        gp[a].maxButtons= caps.wMaxButtons;
        gw[a].maxButtons= caps.wMaxButtons;
        
        /// set needed data
        j[a].id= a;
        j[a].mode= 1;
        gp[a].mode= 1;
        gw[a].mode= 1;

        /// update numbers
        nr.jOS++;    nr.gpOS++;    nr.gwOS++;
        nr.jFound++; nr.gpFound++; nr.gwFound++;
        if(chatty) printf("OS joystick[%d] %s CONNECTED\n", a, j[a].name.d);
      }
    } else if(j[a].mode) {            /// joystick got unplugged
      /// clear all data
      j[a].delData();
      gp[a].delData();
      gw[a].delData();
      /// update numbers
      nr.jOS--;    nr.gpOS--;    nr.gwOS--;
      nr.jFound--; nr.gpFound--; nr.gwFound--;
      if(chatty) printf("OS joystick[%d] %s DISCONNECTED\n", a, j[a].name.d);
    }
  
skipWinOSsearch:
*/ // SCRAP ^^^^^^^^^^^^^^^^^^^^^^^^^
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(timer) osi.getNanosecs(&start);
  // xlib xinput research: ... after some time, found nothing. TOO OLD LIB?
  // this function takes 5-10 mil nanosecs (5-10 millisecs), which is HUGE
  // linux [MODE 1] using "linux/joystick.h". system driver
  
  int f;
  int version, axes= 0, buttons= 0;
  char name[128];
  //short id= 0;
  bool addEventFile= false;
  bool found;
  static string s1("/dev/input/js");
  static string s2("/dev/input/event");
  string s3;
  
  /// searching for 32 js[X] files
  for(short a= 0; a< 32; a++) {
    
    /// this limit to 8 can be changed if neccesary... can't predict what will happen in 10-20 years....
    if(nr.jOS== 8) {
      error.console("OSInput::init: Maximum number of jSticks/gPads/gWheels reached (where did you plug more than 8?)");
      break;
    }
    
    /// check if this id is already in use by some joystick struct
    found= false;
    for(short b= 0; b< 8; b++) {
      if(j[b].jsID== a)
        found= true;
    }
    if(found) continue;                         /// if found, this file id is already open
    
    /// if this id was not found in currently opened joysticks, check if exists this '/dev/input/js[a]' file
    name[0]= f= version= axes= buttons= 0;
    s3.f("%s%d", s1.d, a);                       /// '/dev/input/js[a]'
    
    f= open(s3, O_RDONLY| O_NONBLOCK);
    if(f== -1) continue;                        /// the file does not exist (do not break for!)
    
    ioctl(f, JSIOCGVERSION, &version);          /// JSIOCG version, i think - driver version or something
    // FURTHER TESTS MUST BE DONE WITH VERSION. it must be over 1.0 (>>> every jsiocversion i read is 0... <<<)
    ioctl(f, JSIOCGAXES, &axes);                /// number of axis this stick has
    ioctl(f, JSIOCGBUTTONS, &buttons);          /// number of buttons this stick has
    ioctl(f, JSIOCGNAME(sizeof(name)), &name);  /// stick name or product name
    
    /// check if the joystick is valid. I found out that there are mouses that report as joysticks...
    ///    but they report incredible amounts of axes and buttons...
    ///    still, can't rely only on this, FURTHER CHECKS SHOULD BE MADE  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if((axes> 32) || (buttons> MAX_JOYSTICK_BUTTONS)|| axes<= 0|| buttons<= 0) {
      close(f);
      continue;
    }
    
    /// search for an unused joystick struct
    for(short b= 0; b< 8; b++)
      if(!j[b].mode) {
        /// all joysticks/gamepads/gamewheels share the same file (driver). 
        j[b].mode= gp[b].mode= gw[b].mode= 1;   /// mode 1 sticks = system handled (the only type atm in linux)
        j[b].jsFile= f;                         /// joystick class handles this
        j[b].jsID= a;                           /// joystick class handles this
        j[b].name= gp[b].name= gw[b].name= name;/// stick name / product name

        /*
        joystick name! IT HAS TO HAVE A NAME, if name== null? make it "Unknown" , 
        but IT WON'T MATCH IN EVENT FILE NAME READ, more thinking to this
        maybe set name, after the event match
        */
        
        j[b].maxButtons= 
          gp[b].maxButtons= 
          gw[b].maxButtons= (short)buttons;     /// nr of buttons the stick has
        
        addEventFile= true;                     /// set flag to search for it's event file too
    
        /// better to have lots of vars. The other option would be having complicated search algorithms just to find out how many joysticks found that use driver N
        ///   the purpose is to have the same code run on every OS... game searches for driver type 1 (os driver), type 2(in win is directinput) etc...
        nr.jFound++;  nr.gpFound++; nr.gwFound++; 
        nr.jOS++;     nr.gpOS++;    nr.gwOS++;
        
        checkGamepadType(&gp[b]);               /// check if it is ps3/ xbone compatible
        
        if(chatty) printf("joystick[%d] %s Axes: %d Buttons: %d Version: %d CONNECTED\n", b, name, axes, buttons, version);
        break;
      }
    
  }
  
  /// if no event file needs to be found, just return
  if(!addEventFile) {
    if(timer) osi.getNanosecs(&end);
    if(timer) printf("linux joystick scan: %llu nanosecs\n", end- start);

    return;
  }
  
  // event files asociated with each stick
  
  for(short a= 0; a< 32; a++) {                     /// search thru event0-> event31
    /// search thru all joysticks for this id
    found= false;
    for(short b= 0; b< 8; b++) {
      if(j[b].eventID== a)
        found= true;
    }
    if(found) continue;                             /// if found, this file is already open

    /// try to open this file & read what joystick it belongs to    
    f= version= axes= buttons= name[0]= 0;
    s3.f("%s%d", s2.d, a);                           /// '/dev/input/event[a]'
    
    f= open(s3, O_RDONLY| O_NONBLOCK);
    if(f== -1) continue;                            /// file not found- next!
    
    ioctl(f, EVIOCGNAME(sizeof(name)), &name);      /// read joystick name this event file belongs to
    
    /// if it has no name, skip this; sticks return a name
    if(!name[0]) {
      close(f);
      continue;
    }
    
    /// search for a joystick struct that has no 'event' file & check if name matches with 'event' file's joystick name
    found= false;
    for(short b= 0; b< 8; b++)
      if(j[b].jsFile!= -1)                          /// joystick struct must have a js file
        if((j[b].eventFile== -1) && (j[b].name== name)) {
          j[b].eventFile= f;
          j[b].eventID= a;
          if(chatty) printf("event file: %s%d belongs to joystick %d\n", s2.d, a, b);
          found= true;
          break;
        }
    
    /// no match found? close the file; must be part of something else
    if(!found)
      close(f);
  } /// for each possible 'event' file

  if(timer) osi.getNanosecs(&end);
  if(timer) printf("linux joystick scan: %llu nanosecs\n", end- start);
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  /// mac uses calback functions. check HIDadded() / HIDremoved() at the end of this file
  #endif /// OS_MAC


  // ------------============ MODE 2 JOYSTICKS ===============------------
  #ifdef USING_DIRECTINPUT
  if(timer) osi.getNanosecs(&start);
  /// jConnected helps to check for disconnected joysticks; start with false, and each connected stick must mark 'true'
  /// if a stick doesn't mark jConnected[id] as true, it is DISCONNECTED
  for(short a= 0; a< 8; a++)
    jConnected[a]= false;

  dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, diDevCallback, NULL, DIEDFL_ATTACHEDONLY);

  for(short a= 0; a< 8; a++) {
    if(j[a+ 8].mode&& !jConnected[a]) {     // a joystick was disconnected
      if(chatty) printf("DirectInput: hid[%d] %s DISCONNECTED\n", a+ 8, j[a+ 8].name.d);
      /// clear all axis / buttons data / dinput driver
      j[a+ 8].delData();
      gp[a+ 8].delData();
      gw[a+ 8].delData();
      /// update numbers
      in.nr.jT2--;    in.nr.gpT2--;    in.nr.gwT2--;
      in.nr.jFound--; in.nr.gpFound--; in.nr.gwFound--;
    }
  }


  if(timer) osi.getNanosecs(&end);
  if(timer) printf("direct input joystick check: %llu nanosecs %llu millisecs\n", end- start, (end- start)/ 1000000);
  /// direct input 'plugged' state is in diDevCallback(..)
  /// direct input 'unplugged' state is set in Joystick::update(); if it can't read from device, calls delData()

  #endif /// USING_DIRECTINPUT

  // ------------============ MODE 3 JOYSTICKS ===============------------

  #ifdef USING_XINPUT
  if(timer) osi.getNanosecs(&start);
  
  
  XINPUT_STATE state;
  
  for(short a= 0; a< 4; a++ ) { /// XUSER_MAX_COUNT = max xbones, but 4 is max design set too in osi
    /// Simply get the state of the controller from XInput.  
    if(XInputGetState(a, &state)== ERROR_SUCCESS) {
      if(!j[16+ a].mode) {          /// new stick found
        j[16+ a].id= a;

        /// set mode to XInput (3)
        j[16+ a].mode= 3;
        gp[16+ a].mode= 3;
        gw[16+ a].mode= 3;
        // NOT SHURE THIS IS OK... <<<<<<<<< i can't find a way to find stick's name... this is XInput, tho, and only x-box pads are handled, right?
        j[16+ a].name= "Microsoft X-Box 360 pad";
        gp[16+ a].name= j[16+ a].name;
        gw[16+ a].name= j[16+ a].name;

        /// update sticks numbers
        nr.jT3++;    nr.gpT3++;    nr.gwT3++;
        nr.jFound++; nr.gpFound++; nr.gwFound++;

        if(chatty) printf("XInput controller %d CONNECTED\n", a);
      }
    } else {
      if(j[16+ a].mode) {           /// error reading & stick is enabled= stick disconnected
        /// clear axis / buttons data / rest of stuff
        j[16+ a].delData();
        gp[16+ a].delData();
        gw[16+ a].delData();
        /// update sticks numbers
        nr.jT3--;    nr.gpT3--;    nr.gwT3--;
        nr.jFound--; nr.gpFound--; nr.gwFound--;
        if(chatty) printf("XInput controller %d DISCONNECTED\n", a);
      }
    }
  } /// for each possible xinput slot
  if(timer) osi.getNanosecs(&end);
  if(timer) printf("xinput joystick check: %llu nanosecs\n", end- start);
  
  #endif /// USING_XINPUT
  
}


#ifdef USING_DIRECTINPUT
BOOL CALLBACK diDevCallback(LPCDIDEVICEINSTANCE inst, LPVOID extra) {
  /// can't handle more than 8 direct input sticks (don't think this will change very soon)
  if(in.nr.jT2== 8)
    return DIENUM_CONTINUE;

  /// check if this device is already active
  for(short a= 8; a< 16; a++) {
    if(in.j[a].diID== inst->guidInstance) {
      jConnected[a- 8]= true;           /// this helps with joystick disconnection(see more in Input::populate())
      return DIENUM_CONTINUE;
    }
  }
  
  /// find a free joystick class
  short id;
  for(id= 8; id< 16; id++)
    if(!in.j[id].mode)
      break;

  jConnected[id- 8]= true;              /// this helps with joystick disconnection(see more in Input::populate())

  /// create the direct input device
  bool fail= false;
  if(in.dInput->CreateDevice(inst->guidInstance, &in.j[id].diDevice, NULL)                     != DI_OK) fail= true;
  if(in.j[id].diDevice->SetDataFormat(&c_dfDIJoystick2)                                        != DI_OK) fail= true;
  if(in.j[id].diDevice->SetCooperativeLevel(osi.win[0].hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND)!= DI_OK) fail= true;

  if(fail) {
    error.console("diDevCallback(): couldn't add the new device");
    if(in.j[id].diDevice)
      in.j[id].diDevice->Release();
    in.j[id].diDevice= null;
    return DIENUM_CONTINUE;
  }

  in.j[id].diID= inst->guidInstance;                  /// this is used to distinguish between new sticks and already in use sticks

  /// device capabilities 
  // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.didevcaps(v=vs.85).aspx
  DIDEVCAPS caps;
  caps.dwSize= sizeof(DIDEVCAPS);
  in.j[id].diDevice->GetCapabilities(&caps);
  //(FORCE FEEDBACK FLAG? maybe?)<<<<<<<<<

  in.j[id].maxButtons= (short)caps.dwButtons; /// number of buttons device has
  in.gp[id].maxButtons= in.j[id].maxButtons;
  in.gw[id].maxButtons= in.j[id].maxButtons;

  in.j[id].mode= 2;                           /// set it's mode to DirectInput mode
  in.gp[id].mode= 2;
  in.gw[id].mode= 2;
  
  in.j[id].name= inst->tszProductName;        /// stick name
  in.gp[id].name= in.j[id].name;
  in.gw[id].name= in.j[id].name;
  
  in.nr.jT2++;                                /// update sticks numbers
  in.nr.gpT2++;
  in.nr.gwT3++;
  in.nr.jFound++;
  in.nr.gpFound++;
  in.nr.gwFound++;

  checkGamepadType(&in.gp[id]);               /// check if pad is ps3 compatible or xbone compatible

  if(chatty) printf("DirectInput: found joystick[%d]: %s axes[%d] buttons[%d]\n", id, in.j[id].name.d, caps.dwAxes, caps.dwButtons);

  return DIENUM_CONTINUE;         /// DIENUM_CONTINUE to continue enumerating devices; else it will stop enumerating
}
#endif /// USING_DIRECTINPUT




///=============================================================================
// ######################### MAIN INPUT UPDATE #################################
///=============================================================================
void Input::update() {
  uint64 start, end;    /// debug
  bool timer= false;    /// debug
  if(timer) osi.getNanosecs(&start);
  
  /// scan for joysticks every second (? maybe 2 secs or 3?)
  // DISABLED ATM. can be enabled under linux (must check timer times)
  #ifdef OS_LINUX
  //if(osi.present- lastPopulate> 1000000000)
    //populate();
  #endif /// OS_LINUX
  
  /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  check linux+mac joysticks. buttons & everything
  carefull with axis!! -x and -y must be as in opengl
  buttons 7& 8 must be back(select) / start
  check timers 
   */
  
  

  /// update mouse
  if(m.mode!= 1)
    m.update();
  
  /// update keyboard
  if(k.mode!= 1)
    k.update();
  
  
  /// update system handled sticks/pads/wheels
  if(nr.jOS)
    for(short a= 0; a< 8; a++)
      if(j[a].active)
        j[a].update();
  
  /// update [type 2] sticks/pads/wheels
  if(nr.jT2)
    for(short a= 8; a< 16; a++)
      if(j[a].active)
        j[a].update();
  

  
  /// update [type 3] sticks/pads/wheels
  if(nr.jT3)
    for(short a= 16; a< 20; a++)
      if(j[a].active)
        j[a].update();

  if(timer) osi.getNanosecs(&end);
  if(timer) printf("Input::update() timer: %llu nanosecs\n", end- start); // it's nothing
  
}


// resets all buttons for all devices- usually called on alt/tab - cmd/tab or something similar
void Input::resetPressedButtons() {
  m.resetButtons();
  k.resetButtons();
  for(short a= 0; a< MAX_JOYSTICKS; a++) {
    j[a].resetButtons();
    gp[a].resetButtons();
    gw[a].resetButtons();
  }
}








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

  

// ########################## MOUSE UPDATE ##############################
// when not using default MODE 1, call this func to update mouse vars - or better, call Input::update()
void Mouse::update() {
  if(!osi.flags.haveFocus)
    return;

  uint64 present= osi.present/ 1000000;

/// os events: nothing to update, atm (i cant think of anything anyways)
  if(mode== 1) {

  

/// manual update mode
  } else if(mode== 2) {
    #ifdef OS_WIN

    /// mouse position
    POINT p;
    GetCursorPos(&p);
    oldx= x;
    oldy= y;
    x= p.x;
    y= p.y;
    if(useDelta) {
      dx= x- oldx;      // REMOVED +=, if update() is done; it would not help with anything how the deltas were done
      dy= y- oldy;
    }
    
    /// mouse wheel
    
    // well, shit! gotta use window events, unfortunatly. 
    // NO WIN FUNC AVAIBLE TO READ THE WHEEL IN ANY WAY. the only way is too complicated, with hooks to dll-s... /cry

    /// mouse buttons
    
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

    
    
  } else if(mode== 3) {

    /// direct input
    #ifdef USING_DIRECTINPUT           /// skip some checks. only mode 1 works atm in linux
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
    #endif /// USING_DIRECTINPUT
  } /// pass thru all mouse modes
}


/// grab exclusive control of the mouse (if possible)
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

/// release exclusive control of the mouse
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


/// clears button states, usefull when alt-tabbing, so things don't get messed up
void Mouse::resetButtons() {
  for(short a= 0; a< MAX_MOUSE_BUTTONS; a++) {
    if(b[a].down) {
      b[a].lastTimeStart= b[a].timeStart;
      b[a].lastTimeEnded= osi.present/ 1000000;
      b[a].lastDT= b[a].lastTimeEnded- b[a].lastTimeStart;
      b[a].down= false;
    }
  }
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


// could be called, but using in.init() is better, as it inits every device
bool Keyboard::init(short mode) {
  this->mode= mode;
  if(mode== 1)
    return true;
  if(mode== 2)
    return true;
  if(mode== 3) {



    // MOVE ALL THIS? SCRAP INIT????? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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



// ############################ KEYBOARD UPDATE func ####################################
// if not using MODE 1, call this (autocalled in Input::update() )
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
    diDevice->GetDeviceState(MAX_KEYBOARD_KEYS, key);
    /// do not return
    #endif
  }
  
  
  #ifdef OS_WIN // i can't think of a way atm, to use this in linux. the only mode is [mode 1]
  
  uint64 presentMilli= osi.present/ 1000000;              /// present time in milliseconds

  /// check if a key just started to be pressed or a key was depressed - might be a little time consuming
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
    if((key[a]& 0x80)&& !(lastCheck[a]& 0x80) ) {         // key press start
      keyTime[a]= presentMilli;
      /// log the key in history
      KeyPressed k;
      k.code= a;
      k.checked= false;
      k.timeDown= presentMilli;
      k.timeUp= 0;
      k.timeDT= 0;

      log(k);
    }
    else if((lastCheck[a]& 0x80)&& !(key[a]& 0x80)) {     // key depressed
      bool found= false;
      for(short b= 0; b< MAX_KEYS_LOGGED; b++)
        if(lastKey[b].code== a) {
          if(lastKey[b].timeUp) continue;           /// skip if this key is already released

          lastKey[b].timeUp= presentMilli;
          lastKey[b].timeDT= presentMilli- lastKey[b].timeDown;
          lastKey[b].checked= false;
          found= true;

          break;
        }
        // this code can go away, as buttons & history are reset on app focus lost
        //  but first some debugs can be done i think, with these 1 millisec logs
      if(!found) {                                  /// failsafe - normally it is found (alt-tab happens...)
        KeyPressed k;
        k.code= a;
        k.checked= false;
        k.timeDown= presentMilli- 1;
        k.timeUp= presentMilli;
        k.timeDT= 1;
        log(k);                                     /// put it in history buffer
      }
    }
  #endif /// OS_WIN
}


// updates keyboard lock states (numlock/capslock/scrolllock)
void Keyboard::updateLocks() {
  #ifdef OS_WIN
  capsLock=   (GetKeyState(VK_CAPITAL)& 0x01) == 1;
  numLock=    (GetKeyState(VK_NUMLOCK)& 0x01) == 1;
  scrollLock= (GetKeyState(VK_SCROLL)&  0x01) == 1;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  uint n;
  XkbGetIndicatorState(osi.primWin->dis, XkbUseCoreKbd, &n);
  capsLock=   (n& 0x01);
  numLock=    (n& 0x02);
  scrollLock= (n& 0x04);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  // makeme - better to have this!!!  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  #endif /// OS_MAC
}


// grab exclusive control of the keyboard (if possible)
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

// ungrab exclusive control of the keyboard
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



// clears all character buffers, ususally called when switching to a new/ existing input box / control
void Keyboard::clearTypedBuffer() {
  while(charTyped.first)
    charTyped.del(charTyped.first);
  while(manipTyped.first)
    manipTyped.del(manipTyped.first);
}


// clears buffers and resets all logged keys, usually called when alt-tabbing (losing focus)
void Keyboard::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++) {
    buffer1[a]= buffer2[a]= 0;
    keyTime[a]= 0;
  }

  /// reset logged keys
  for(short a= 0; a< MAX_KEYS_LOGGED; a++) {
    /// if dela time pressed was not computed, it still waiting for key release
    if(!lastKey[a].timeDT) {
      lastKey[a].timeUp= osi.present/ 1000000;
      lastKey[a].timeDT= lastKey[a].timeUp- lastKey[a].timeDown;
    }
  }
}



// returns a character that user has typed (from a buffer wich has 1 second lifetime)
ulong Keyboard::getChar() {
  if(!charTyped.nrNodes)
    return 0;

  /// clear old typed characters (must have stayed in buffer longer than 1 sec)
  uint64 present= osi.present/ 1000000;
  
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
  uint64 present= osi.present/ 1000000;
  
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

// [internal]
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


// [internal] identical as addChar...
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


// might never be used... gets the first key that is down... not much uses for this other than checking if any key is pressed
short Keyboard::getFirstKey() {
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
    if(key[a])
      return a;

  return -1;	/// fail
}


/// internal stuff
void Keyboard::swapBuffers() {
  lastCheck= key;
  key= (key== buffer1)? buffer2: buffer1;
}


// [internal] logs a key to histoty of keys that were pressed
void Keyboard::log(const Keyboard::KeyPressed &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastKey[a]= lastKey[a- 1];
  lastKey[0]= k;
}



// debugging
void Keyboard::printPressed() {
  if((mode== 2) || (mode== 3))
    for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
      if(key[a]& 0x80)
        printf(" kc%d(%d)", a, key[a]);
}









///========================================================================///
// ------------============= JOYSTICK CLASS ===========-------------------- //
///========================================================================///

Joystick::Joystick() {
  mode= 0;                    /// mode set to 0 = DISABLED
  _gp= null;                  /// must start null
  _gw= null;                  /// must start null

  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  diDevice= null;
  diID= {0};
  #endif
  #endif

  #ifdef OS_LINUX
  jsFile= -1;
  eventFile= -1;
  jsID= -1;
  eventID= -1;
  #endif /// OS_LINUX

  delData();
}


Joystick::~Joystick() {
  delData();
}


void Joystick::delData() {
  mode= 0;
  name.delData();
  

  x= y= x2= y2= throttle= rudder= u= v= 0;
  pov= -1;
  
  b= buffer1;
  lastCheck= buffer2;

  resetButtons();

  #ifdef OS_WIN
  id= -1;
  #ifdef USING_DIRECTINPUT
  if(diDevice) {
    diDevice->Unacquire();
    diDevice->Release();
    diDevice= null;
  }
  diID= { 0 };
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

  #ifdef OS_LINUX
  /// close driver files if currently opened
  if(jsFile!= -1)
    close(jsFile);
  if(eventFile!= -1)
    close(eventFile);

  jsFile= -1;
  jsID= -1;
  eventFile= -1;
  eventID= -1;
  #endif /// OS_LINUX
  deactivate();
}



// ############### JOYSTICK UPDATE #################
/// handles pads/wheels too. might be a good ideea to call this func if others are called, dunno (or just remove update() from gp/gw)

void Joystick::update() {
  if(!active) return;                             /// a stick/pad/wheel must be marked as active, for updating to take place
                                                  /// this must be used, as updating same device with mutiple drives, will create a bad mess

                                                  // if player 1 selects device0 with xinput
                                                  // and player 2 selects device0 with directinput??? what then? some tests must be done somehow

  uint64 presentMilli= osi.present/ 1000000;      /// present time in milliseconds
  ButPressed blog;
  bool found;
  
  #ifdef OS_MAC
  /// mac uses events for hid input change, and lastCheck must be updated once per call to Input::update()
  /// there can be multiple updates per frame, or none, when using events. Therefore swapBuffers must be done here
  for(short a= 0; a< maxButtons; a++) {
    lastCheck[a]= b[a];
    _gp->lastCheck[a]= _gp->b[a];
    _gw->lastCheck[a]= _gw->b[a];
  }
  #endif /// OS_MAC

  // -----------============ MODE 1 JOYSTICKS ============------------
  if(mode== 1) {
    #ifdef OS_WIN
    // CRASHES TO DESKTOP, BUGGY READING, USES DIRECT INPUT ANYWAYS  >>> SCRAP <<<
    /*
    /// read values from OS
    JOYINFOEX jinfo;
    //for(short a= 0; a< sizeof(jinfo); a++) ((char *)&jinfo)[a]= 0;    /// wipe jinfo
    jinfo.dwSize= sizeof(JOYINFOEX);
    RAWINPUT *r;
    
    if(joyGetPosEx(id, &jinfo)!= JOYERR_NOERROR) {
      if(chatty) printf("OS: hid[%s] DISCONNECTED\n", name.d? name.d: "unknown");
      /// clear all axis / buttons data / dinput driver
      delData();
      _gp->delData();
      _gw->delData();
      /// update numbers
      in.nr.jOS--;    in.nr.gpOS--;    in.nr.gwOS--;
      in.nr.jFound--; in.nr.gpFound--; in.nr.gwFound--;
      return;
    }

    if(jinfo.dwXpos> 32768) jinfo.dwXpos--;
    if(jinfo.dwYpos> 32768) jinfo.dwYpos--;
    if(jinfo.dwZpos> 32768) jinfo.dwZpos--;
    if(jinfo.dwRpos> 32768) jinfo.dwRpos--;
    if(jinfo.dwUpos> 32768) jinfo.dwUpos--;
    if(jinfo.dwVpos> 32768) jinfo.dwVpos--;
    
    // ---=== JOYSTICK axis ===---
    x=        jinfo.dwXpos- 32767;                    /// main stick X axis
    y=        -(long)jinfo.dwYpos+ 32767;             /// main stick Y axis
    throttle= jinfo.dwZpos;                           /// throttle? need a joystick...
    rudder=   jinfo.dwRpos;                           /// rudder
    pov=      (jinfo.dwPOV== 65535)? -1: jinfo.dwPOV; /// pov in degrees* 100
    v=        jinfo.dwVpos- 32767;                    /// extra axis 6
    u=        -(long)jinfo.dwUpos+ 32767;             /// extra axis 5
    

    // ---=== GAMEPAD axis ===---
    _gp->lx=    x;       /// left stick X
    _gp->ly=    y;      /// left stick Y
    _gp->pov=   pov;        /// pov (0- 359 degrees * 100, -1 neutral)
    
    if(_gp->type== 0) {              // ps3 compatible
      _gp->rx= throttle- 32767;        /// right stick X
      _gp->ry= -rudder+32767;       /// right stick Y
      _gp->lt= 32767- v;        /// left trigger
      _gp->rt= 32767+ u;        /// right trigger
    } else if(_gp->type== 1) {       // xbone compatible
      _gp->rx= v;        /// right stick X
      _gp->ry= -u;       /// right stick Y
      _gp->lt= throttle;        /// left trigger
      _gp->rt= rudder;        /// right trigger
    }

    // ---=== GAMEWHEEL axis ===---
    _gw->wheel= jinfo.dwXpos;     /// wheel position
    _gw->a1=    jinfo.dwYpos;     /// other axis 1
    _gw->a2=    jinfo.dwZpos;     /// other axis 2
    _gw->a3=    jinfo.dwRpos;     /// other axis 3
    _gw->a4=    jinfo.dwPOV;      /// other axis 4
    _gw->a5=    jinfo.dwUpos;     /// other axis 5

    // ---=== stick/pad/wheel BUTTONS ===---
    swapBuffers();                    /// lastCheck will hold the previous buttons states
    _gp->swapBuffers();
    _gw->swapBuffers();

    for(uchar a= 0; a< 32; a++) {     /// for each possible button
      // can't find out what gamepad this is under this driver

      b[a]= (jinfo.dwButtons>> a)& 1;   /// current button state
      _gp->b[a]= b[a];
      _gw->b[a]= b[a];
      // --------------============== BUTTON PRESS ================-----------------
      if(b[a]&& !lastCheck[a]) {
        /// mark button press time
        bTime[a]= presentMilli;
        _gp->bTime[a]= bTime[a];
        _gw->bTime[a]= bTime[a];
        /// log the button in history
        blog.b= a;
        blog.checked= false;
        blog.timeDown= presentMilli;
        blog.timeUp= 0;
        blog.timeDT= 0;
        log(blog);
        _gw->log(blog);
        _gp->log(blog);
        if(chatty) printf("OS: hid[%s] button PRESS nr[%d]\n", name.d, a);
      
      // --------------============= BUTTON RELEASE ===============-----------------
      } else if(lastCheck[a]&& !b[a]) {

        /// search thru history for the button, to mark the time it got released
        found= false;
        for(short b= 0; b< MAX_KEYS_LOGGED; b++)
          if(lastBut[b].b== a) {
            if(lastBut[b].timeUp) continue;    /// skip if button is already released

            lastBut[b].timeUp= presentMilli;
            lastBut[b].timeDT= presentMilli- lastBut[b].timeDown;
            lastBut[b].checked= false;

            /// gamepad button nr could have a different number, but it does not matter
            _gp->lastBut[b].timeUp= presentMilli;
            _gp->lastBut[b].timeDT= lastBut[b].timeDT;
            _gp->lastBut[b].checked= false;
  
            _gw->lastBut[b].timeUp= presentMilli;
            _gw->lastBut[b].timeDT= lastBut[b].timeDT;
            _gw->lastBut[b].checked= false;

            found= true;
            break;
          }

        // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
        if(!found) {                      /// failsafe - normally it is found (but things can happen ... alt-tab?)
          // some debug stuff can be done here, tho
          blog.b= a;
          blog.checked= false;
          blog.timeDown= presentMilli- 1; /// mark it as insta down-up
          blog.timeUp= presentMilli;
          blog.timeDT= 1;
          log(blog);                      /// put it in history buffer
          _gw->log(blog);
          _gp->log(blog);
        } /// failsafe check
        
        if(chatty) printf("OS: hid[%s] button RELEASE nr[%d]\n", name.d, a);
        
      } /// if button press or release
    } /// for each button
    */ // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ SCRAP ^^^^^^^^^^^^^^^^^^^^

    #endif /// OS_WIN

  
    #ifdef OS_LINUX
    /// [MODE 1] joysticks/ gpads/ gwheels
    int n= -1, nev;
    js_event ev[64];
  
ReadAgain:
    /// read as many events as possible in 1 go
    n= read(jsFile, ev, sizeof(ev));

    if(n== -1) {                            /// no event happened / joystick unplugged

      // check if the joystick was UNPLUGGED (read sets errno on EBADF: file does not exist anymore)
      //printf("e[%d]", errno);
      if(errno== ENODEV) {
        if(chatty) printf("joystick %s REMOVED\n", name.d);

        /// set stick as DISABLED & close all opened driver files
        delData();
        _gp->delData();
        _gw->delData();

        /// update sticks numbers
        in.nr.jFound--; in.nr.gpFound--; in.nr.gwFound--;
        in.nr.jOS--;    in.nr.gpOS--;    in.nr.gwOS--;
      } /// if the file is not found (stick disconnected)
      return;
    } /// if nothing read

    nev= n/ sizeof(js_event);               /// nr of events read
    
    for(short a= 0; a< nev; a++) {           // for each event
      //if(ev[a].type& JS_EVENT_INIT)
        //continue;
      //ev[a].type^= JS_EVENT_INIT;
      
      // --------------============= BUTTON EVENT ===============---------------
      if(ev[a].type& JS_EVENT_BUTTON) {
        
        /*
        * gamepad buttons are messy. there should be an order in buttons, done by osi, i think, for GAMEPAD UNIFICATION
        *   the first 10 buttons are on all gamepads
        *   the rest of buttons shuld be 'extra' buttons, (including the xbox button), and arranged after button 10
        *   there has to be a variable that holds the total number of buttons the stick has
        *   thrustmaster pad's extra buttons are on 7-> n
        *   xbox 'xbox button' is on 9 ...
        */
        
        short but, extra;
        but= ev[a].number;
        /// gamepad buttons unification. extra buttons (including xbox main button are set after button 10)
        
        /// ps3 compatible pad
        if(in.gp[a].type== 0) {
          extra= _gp->maxButtons- 10; /// normal ps3 has 10 buttons. the rest are extra, on modified ps3 pads
          
          if(extra && (ev[a].number>= 6) && (ev[a].number< 6+ extra))
            but+= 4;                      /// is it an extra button? -> moved on position 10+
          else if(extra && (ev[a].number>= 6+ extra))
            but-= extra;                  /// all buttons above extra buttons, moved back on 6+
          
        /// xbox compatible pad
        } else if(in.gp[a].type== 1) {
          // EXTRA BUTTONS NOT HANDLED ON XBOX controller <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
          extra= _gp->maxButtons- 11; /// normal xbone has 11 buttons. rest are extra, but i have no clue where they would go... don't have such pad
          
          
          if(ev[a].number== 8)
            but= 10;                      /// is it the xbox button? -> move it on position 10, with the extra buttons
          else if(ev[a].number>=9 && ev[a].number<= 10)
            but-= 1;                      /// move start & back to positions 8 & 9
        }

        /// swapbuffers won't work. just update lastCheck[]
        lastCheck[ev[a].number]= b[ev[a].number];
        _gp->lastCheck[but]= _gp->b[but];
        _gw->lastCheck[ev[a].number]= b[ev[a].number];
        
        /// update button state
        b[ev[a].number]=  (uchar)ev[a].value;
        _gp->b[but]= (uchar)ev[a].value;
        _gw->b[ev[a].number]= (uchar)ev[a].value;
        
        /// update history
        if(ev[a].value== 1) {                      // button PRESS
          bTime[ev[a].number]= ev[a].time;
          _gp->bTime[but]= ev[a].time;
          _gw->bTime[ev[a].number]= ev[a].time;
          /// put the button in history
          blog.b= ev[a].number;
          blog.checked= false;
          blog.timeDown= bTime[but];
          blog.timeUp= 0;
          blog.timeDT= 0;
          log(blog);
          _gw->log(blog);
          blog.b= but;
          _gp->log(blog);
          if(chatty) printf("hid[%s] button PRESS nr[%d] arranged[%d]\n", name.d, ev[a].number, but);
        } else if(ev[a].value== 0) {               // button RELEASE
          /// search thru history for the button, to mark the time it got released
          found= false;
          for(short b= 0; b< MAX_KEYS_LOGGED; b++)
            if(lastBut[b].b== ev[a].number) {
              if(lastBut[b].timeUp) continue;    /// skip if this tested button is already released

              lastBut[b].timeUp= ev[a].time;
              lastBut[b].timeDT= ev[a].time- lastBut[b].timeDown;
              lastBut[b].checked= false;

              /// gamepad button nr could have a different number, but it does not matter
              _gp->lastBut[b].timeUp= ev[a].time;
              _gp->lastBut[b].timeDT= lastBut[b].timeDT;
              _gp->lastBut[b].checked= false;

              _gw->lastBut[b].timeUp= ev[a].time;
              _gw->lastBut[b].timeDT= lastBut[b].timeDT;
              _gw->lastBut[b].checked= false;

              found= true;
              break;
            }
        
          // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
          if(!found) {                      /// failsafe - normally it is found (but things can happen ... alt-tab?)
            // some debug stuff can be done here, tho
            blog.b= ev[a].number;
            blog.checked= false;
            blog.timeDown= ev[a].time- 1;   /// mark it as insta down-up
            blog.timeUp= ev[a].time;
            blog.timeDT= 1;
            log(blog);                      /// put it in history buffer
            _gw->log(blog);
            blog.b= but;
            _gp->log(blog);
          } /// failsafe
        
          if(chatty) printf("hid[%s] button RELEASE nr[%d] arranged[%d]\n", name.d, ev[a].number, but);
        } /// if button press/release

      // --------------============== AXIS EVENT ================---------------
      } else if(ev[a].type& JS_EVENT_AXIS) {
        /// axis order...
        
        //printf("axis %d: [%d]\n", ev[a].number, ev[a].value);
        //continue;
        
        // possible to make a[MAX_AXIS] and x/y/rudder/etc would be refrences to a[]
        switch (ev[a].number) {
                                            // [JOYSTICK]  / [GAMEPAD]   / [GAMEWHEEL]
          case 0:                           // [X axis?]   / [l stick X] / [wheel???]
            x= ev[a].value;
            _gp->lx= ev[a].value;
            _gw->a1= ev[a].value;
            break;
          case 1:                           // [Y axis?]   / [l stick Y] / [wheel???]
            y= -ev[a].value;
            _gp->ly= -ev[a].value;
            _gw->a2= ev[a].value;
            break;
          case 2:                           // [Throttle?] / [r stick X] / [wheel???]
            throttle= ev[a].value;
            if(_gp->type== 0)
              _gp->rx= ev[a].value;        /// ps3 pad   [right stick]
            else
              _gp->lt= 32767+ ev[a].value; /// xbone pad [left trigger]
            
            _gw->a3= ev[a].value;
            break;
          case 3:                           // [extra1 X?] / [l trigger] / [wheel???]
            x2= ev[a].value;
            if(_gp->type== 0)
              _gp->lt= 32767- ev[a].value; /// ps3   [left trigger]
            else
              _gp->rx= ev[a].value;        /// xbone [right stick X]
            _gw->a5= ev[a].value;
            break;
          case 4:                           // [extra1 Y?] / [r trigger] / [wheel???]
            y2= ev[a].value;
            if(_gp->type== 0)
              _gp->rt= 32767- ev[a].value; /// ps3   [right trigger]
            else
              _gp->ry= -ev[a].value;       /// xbone [right stick Y]
            // gw <<<<<<<<<<<<<<<<<<<<<<<<<<
            break;
          case 5:                           // [Rudder?]   / [rStick Y]  / [wheel???]
            rudder= ev[a].value;
            if(_gp->type== 0)
              _gp->ry= -ev[a].value;       /// ps3   [right stick Y]
            else
              _gp->rt= 32767+ ev[a].value; /// xbone [right trigger]
            _gw->a4= ev[a].value;
            break;
          case 6:                         // [POV X?]    / [POV X]     / [wheel???]
          case 7:                         // [POV Y?]    / [POV Y]     / [wheel???]
            long tx, ty;          // they gotta be integers for exact 0 degrees or 90 degrees, else there are problems
            double tpov;
            
            /// get axis from current pov position (wich is in degrees)
            tpov= pov;
            tx= ty= 0;
            
            if(pov!= -1) {           /// ... only if it's not on -1 position (nothing selected)
              tx= (double)(32767.0* sin(pov* (M_PI/ 180.0)));
              ty= (double)(32767.0* cos(pov* (M_PI/ 180.0)));
            }
            
            /// update from event
            if(ev[a].number== 6)          /// x axis event
              tx= ev[a].value;
            else                          /// y axis event
              ty= -ev[a].value;
            
            /// find pov in degrees; there have to be checks for each quadrant, unfortunatelly (bad for speed)
            if(ty> 0) {
              if(tx>= 0)
                tpov= (double) ((atan(tx/ ty))* (180.0/ M_PI));
              else
                tpov= (double) ((2* M_PI+ atan(tx/ ty))* (180.0/ M_PI));
            } else if(ty< 0) {
              tpov= (double) ((M_PI+ atan(tx/ ty))* (180.0/ M_PI));
              
            } else if(ty == 0) {
              if(tx== 0)
                tpov= -1;
              else if(tx> 0)
                tpov= 90;
              else if(tx< 0)
                tpov= 270;
            }
            
            /// pov found @ this point
            pov= tpov;
            _gp->pov= pov;
            // gw is not set<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            
            break;
          case 8:                         // [u axis]    / [u axis]    / [u axis]
            
            u= ev[a].value;
            _gp->u= ev[a].value;
            break;
          case 9:                         // [v axis]    / [v axis]    / [v axis]
            v= ev[a].value;
            _gp->v= ev[a].value;
            break;
          default:
            printf("unhandled axis event!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }
      } else
        printf("unhandled other joystick event!!!!!!!!!!!!!!!!!!!!!\n");
      
        
    } /// for each event
    
    // did it actually read 64 events? read another 64 then...
    if(nev == 64)
      goto ReadAgain;
    
    #endif /// OS_LINUX

    #ifdef OS_MAC
    /// macs use callback functions. check HIDchange() at the end of this file
    #endif /// OS_MAC


  // -----------============ MODE 2 JOYSTICKS ============------------
  } else if(mode== 2) {       // win(DirectInput) linux(n/a) mac(n/a)
    
    #ifdef USING_DIRECTINPUT
    if(diDevice->GetDeviceState(sizeof(DIJOYSTATE2), (LPVOID)&diStats)== DIERR_INPUTLOST) {   /// device DISCONNECTED
      if(chatty) printf("DirectInput: hid[%s] DISCONNECTED\n", name.d);
      /// clear all axis / buttons data / dinput driver
      delData();
      _gp->delData();
      _gw->delData();
      /// update numbers
      in.nr.jT2--;    in.nr.gpT2--;    in.nr.gwT2--;
      in.nr.jFound--; in.nr.gpFound--; in.nr.gwFound--;
      return;
    }

    /// adjust axis to osi standard (-32767 / +32767)
    if(diStats.lX> 32768) diStats.lX--; diStats.lX= diStats.lX- 32767;
    if(diStats.lY> 32768) diStats.lY--; diStats.lY= diStats.lY- 32767;
    if(diStats.lZ> 32768) diStats.lZ--; diStats.lZ= diStats.lZ- 32767;
    if(diStats.lRx> 32768) diStats.lRx--; diStats.lRx= diStats.lRx- 32767;
    if(diStats.lRy> 32768) diStats.lRy--; diStats.lRy= diStats.lRy- 32767;
    if(diStats.lRz> 32768) diStats.lRz--; diStats.lRz= diStats.lRz- 32767;
    if(diStats.rglSlider[0]> 32768) diStats.rglSlider[0]--; diStats.rglSlider[0]= diStats.rglSlider[0]- 32767;
    if(diStats.rglSlider[1]> 32768) diStats.rglSlider[1]--; diStats.rglSlider[1]= diStats.rglSlider[1]- 32767;

    // ---=== JOYSTICK axis ===---
    x=          diStats.lX;           /// main stick X
    y=          -diStats.lY;          /// main stick Y
    throttle=   diStats.lZ;           /// throttle
    rudder=     diStats.lRz;          /// rudder
    pov=        diStats.rgdwPOV[0];   /// pov
    u=          diStats.lRx;          /// extra axis 1
    v=          -diStats.lRy;         /// extra axis 2

    // ---=== GAMEPAD axis ===---
    _gp->lx=    diStats.lX;           /// left stick X
    _gp->ly=    -diStats.lY;          /// left stick Y
    _gp->pov=   diStats.rgdwPOV[0];   /// pov (0- 359 degrees * 100, -1 neutral)

    if(_gp->type== 0) {                // ps3 compatible
      _gp->rx= diStats.lZ;            /// right stick X
      _gp->ry= -diStats.lRz;          /// right stick Y
      _gp->lt= 32767- diStats.lRx;           /// left trigger
      _gp->rt= 32767- diStats.lRy;           /// right trigger
    } else if(_gp->type== 1) {         // xbone compatible
      _gp->rx= diStats.lRx;           /// right stick X
      _gp->ry= -diStats.lRy;          /// right stick Y
      _gp->lt= -diStats.lZ;           /// left trigger
      // this is probably a Direct Input BUG; they meant diStats.lRz; as it is now, 2 axis are fused into 1... wich is GREAT!
      _gp->rt= -diStats.lZ;  /// right trigger
    }

    // ---=== GAMEWHEEL axis ===---
    _gw->wheel= diStats.lX;           /// wheel position
    _gw->a1=    diStats.lY;           /// other axis 1
    _gw->a2=    diStats.lZ;           /// other axis 2
    _gw->a3=    diStats.lRx;          /// other axis 3
    _gw->a4=    diStats.lRy;          /// other axis 4
    _gw->a5=    diStats.lRz;          /// other axis 5

    // ---=== stick/pad/wheel BUTTONS ===---
    uchar but, extra;
    swapBuffers();                      /// lastCheck will hold the previous buttons states
    _gp->swapBuffers();
    _gw->swapBuffers();

    // >> something wrong with all buttons from dinput&OS drivers
    // >> test if mode 1 reads ok the xbox controller... if not, scrape, check what WM_messages there are...

    for(uchar a= 0; a< maxButtons; a++) {     /// for each possible button
      /*
        * gamepad buttons are messy. there should be an order in buttons, done by osi, i think, for GAMEPAD UNIFICATION
        *   the first 10 buttons are on all gamepads
        *   the rest of buttons shuld be 'extra' buttons, (including the xbox button), and arranged after button 9
        *   there has to be a variable that holds the total number of buttons the stick has
        *   thrustmaster pad's extra buttons are on 6-> n
        *   xbox 'xbox button' is on 8; it is changed on 10, and butotns 9 & 10 moved back 1 position
        */
      
      /// gamepad buttons unification. extra buttons (including xbox main button are set after button 9)
      but= a;    
      // ps3 compatible pad
      if(_gp->type== 0) {
        extra= _gp->maxButtons- 10;     /// gamepads have 10 normal buttons. the rest are marked as extra, and moved on button 10+
          
        if(extra && (a>= 6) && (a< 6+ extra))
          but+= 4;                      /// is it an extra button? -> moved on position 11+
        else if(extra && (a>= 6+ extra))
          but-= extra;                  /// all buttons above extra buttons, moved back on 7+

      // xbox compatible pad
      } else if(_gp->type== 1) {
        /// direct input i don't think will ever see extra buttons... i got 1 extra and it's nowhere to be found

      } /// check type of gamepad

      /// update current button state
      b[a]= diStats.rgbButtons[a]? 1: 0;
      _gp->b[but]= b[a];
      _gw->b[a]= b[a];

      // --------------============== BUTTON PRESS ================-----------------
      if(b[a]&& !lastCheck[a]) {
        /// mark button press time
        bTime[a]= presentMilli;
        _gp->bTime[but]= bTime[a];
        _gw->bTime[a]= bTime[a];
        /// log the button in history
        blog.b= a;
        blog.checked= false;
        blog.timeDown= presentMilli;
        blog.timeUp= 0;
        blog.timeDT= 0;
        log(blog);
        _gw->log(blog);
        blog.b= but;
        _gp->log(blog);
        if(chatty) printf("DirectInput: hid[%s] button PRESS nr[%d] arranged[%d]\n", name.d, a, but);
      
      // --------------============= BUTTON RELEASE ===============-----------------
      } else if(lastCheck[a]&& !b[a]) {

        /// search thru history for the button, to mark the time it got released
        found= false;
        for(short b= 0; b< MAX_KEYS_LOGGED; b++)
          if(lastBut[b].b== a) {
            if(lastBut[b].timeUp) continue;    /// skip if this tested button is already released

            lastBut[b].timeUp= presentMilli;
            lastBut[b].timeDT= presentMilli- lastBut[b].timeDown;
            lastBut[b].checked= false;

            /// gamepad button nr could have a different number, but it does not matter
            _gp->lastBut[b].timeUp= presentMilli;
            _gp->lastBut[b].timeDT= lastBut[b].timeDT;
            _gp->lastBut[b].checked= false;
  
            _gw->lastBut[b].timeUp= presentMilli;
            _gw->lastBut[b].timeDT= lastBut[b].timeDT;
            _gw->lastBut[b].checked= false;

            found= true;
            break;
          }
        
        // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
        if(!found) {                      /// failsafe - normally it is found (but things can happen ... alt-tab?)
          // some debug stuff can be done here, tho
          blog.b= a;
          blog.checked= false;
          blog.timeDown= presentMilli- 1; /// mark it as insta down-up
          blog.timeUp= presentMilli;
          blog.timeDT= 1;
          log(blog);                      /// put it in history buffer
          _gw->log(blog);
          blog.b= but;
          _gp->log(blog);
        } /// failsafe

        if(chatty) printf("DirectInput: hid[%s] button RELEASE nr[%d] arranged[%d]\n", name.d, a, but);

      } /// button press or release
    } /// for each button
     

    #endif /// USING_DIRECTINPUT

  // -----------============ MODE 3 JOYSTICKS ============------------
  } else if(mode== 3) {       // win(XInput) linux(n/a) mac(n/a)
    
    #ifdef USING_XINPUT
    XINPUT_STATE xi;
    for(short a= 0; a< sizeof(xi); a++) ((char *)&xi)[a]= 0;    /// zero memory
    XInputGetState(id, &xi);                                    /// read data from driver (update xi)

    /// finding pov in degrees* 100
    long tpov;
    if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_UP) {
      if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_RIGHT)
        tpov= 4500;
      else if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_LEFT)
        tpov= 31500;
      else
        tpov= 0;
    } else if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_DOWN) {
      if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_RIGHT)
        tpov= 13500;
      else if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_LEFT)
        tpov= 22500;
      else
        tpov= 18000;
    } else if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_RIGHT)
      tpov= 9000;
    else if(xi.Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_LEFT)
      tpov= 27000;
    else
      tpov= -1;

    /// adjust all axis to fit osi standard (-32767 +32767, or 0-65534 for rudder& throttle)
    if(xi.Gamepad.sThumbLX< 0) xi.Gamepad.sThumbLX++;
    if(xi.Gamepad.sThumbLY< 0) xi.Gamepad.sThumbLY++;
    if(xi.Gamepad.sThumbRX< 0) xi.Gamepad.sThumbRX++;
    if(xi.Gamepad.sThumbRY< 0) xi.Gamepad.sThumbRY++;
    xi.Gamepad.bLeftTrigger= (xi.Gamepad.bLeftTrigger* 65534)/ 255;
    xi.Gamepad.bRightTrigger= (xi.Gamepad.bRightTrigger* 65534)/ 255;  

    // ---=== JOYSTICK axis ===---
    x=        xi.Gamepad.sThumbLX;        /// main stick X
    y=        xi.Gamepad.sThumbLY;        /// main stick Y
    throttle= xi.Gamepad.bRightTrigger;   /// throttle
    rudder=   xi.Gamepad.bLeftTrigger;    /// rudder
    pov=      tpov;                       /// pov
    u=        xi.Gamepad.sThumbRX;        /// extra axis 1
    v=        xi.Gamepad.sThumbRY;        /// extra axis 2
    
    // ---=== GAMEPAD axis ===---
    /// windows uses 32768 units for negative axis, and 32767 units for positive axis... (0 is considered center in osi)
    _gp->lx=  xi.Gamepad.sThumbLX;        /// left stick X
    _gp->ly=  xi.Gamepad.sThumbLY;        /// left stick Y
    _gp->rx=  xi.Gamepad.sThumbRX;        /// right stick X
    _gp->ry=  xi.Gamepad.sThumbRY;        /// right stick Y
    _gp->lt=  xi.Gamepad.bLeftTrigger;    /// left trigger
    _gp->rt=  xi.Gamepad.bRightTrigger;   /// right trigger
    _gp->pov= tpov;                       /// pov (0- 359 degrees * 100, -1 neutral)

    // ---=== GAMEWHEEL axis ===---
    _gw->wheel= xi.Gamepad.sThumbLX;      /// wheel position
    _gw->a1=    xi.Gamepad.sThumbLY;      /// other axis 1
    _gw->a2=    xi.Gamepad.sThumbRX;      /// other axis 2
    _gw->a3=    xi.Gamepad.sThumbRY;      /// other axis 3
    _gw->a4=    xi.Gamepad.bLeftTrigger;  /// other axis 4
    _gw->a5=    xi.Gamepad.bRightTrigger; /// other axis 5

    // ---=== stick/pad/wheel BUTTONS ===---

    /// swapbuffers: lastCheck becomes previous button state
    swapBuffers();
    _gp->swapBuffers();
    _gw->swapBuffers();

    /// XInput button order is all ver the place...
    _gp->b[0]= _gw->b[0]= b[0]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_A)>> 12;
    _gp->b[1]= _gw->b[1]= b[1]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_B)>> 13;
    _gp->b[2]= _gw->b[2]= b[2]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_X)>> 14;
    _gp->b[3]= _gw->b[3]= b[3]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_Y)>> 15;
    _gp->b[4]= _gw->b[4]= b[4]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_LEFT_SHOULDER)>> 8;
    _gp->b[5]= _gw->b[5]= b[5]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_RIGHT_SHOULDER)>> 9;
    _gp->b[6]= _gw->b[6]= b[6]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_BACK)>> 5;
    _gp->b[7]= _gw->b[7]= b[7]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_START)>> 4;
    _gp->b[8]= _gw->b[8]= b[8]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_LEFT_THUMB)>> 6;
    _gp->b[9]= _gw->b[9]= b[9]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_RIGHT_THUMB)>> 7;

    /// check if it is a new press/ release & do button history
    for(short a= 0; a< 10; a++) {              /// for each possible xpad button (i got 1 extra, but heck if i know how to check it)
      if(lastCheck[a]&& !b[a]) {                // button just got ---=== PRESSED ===---
        /// mark button press time
        _gp->bTime[a]= _gw->bTime[a]= bTime[a]= presentMilli;
        /// log the button in history
        blog.b= (uchar)a;
        blog.checked= false;
        blog.timeDown= presentMilli;
        blog.timeUp= 0;
        blog.timeDT= 0;
        log(blog);
        _gw->log(blog);
        _gp->log(blog);
        if(chatty) printf("XInput: hid[%s] button PRESS nr[%d]\n", name.d, a);
      } else if(b[a]&& !lastCheck[a]) {         // button just got ---=== RELEASED ===---
        /// search thru history for the button, to mark the time it got released
        found= false;
        for(short b= 0; b< MAX_KEYS_LOGGED; b++) 
          if(lastBut[b].b== a) {
            if(lastBut[b].timeUp) continue;    /// skip if button is already released

            lastBut[b].timeUp= presentMilli;
            lastBut[b].timeDT= presentMilli- lastBut[b].timeDown;
            lastBut[b].checked= false;

            _gp->lastBut[b].timeUp= presentMilli;
            _gp->lastBut[b].timeDT= lastBut[b].timeDT;
            _gp->lastBut[b].checked= false;
  
            _gw->lastBut[b].timeUp= presentMilli;
            _gw->lastBut[b].timeDT= lastBut[b].timeDT;
            _gw->lastBut[b].checked= false;

            found= true;
            break;
          }

        // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
        if(!found) {                      /// failsafe - normally it is found (but things can happen ... alt-tab?)
          // some debug stuff can be done here, tho
          blog.b= (uchar)a;
          blog.checked= false;
          blog.timeDown= presentMilli- 1; /// mark it as insta down-up
          blog.timeUp= presentMilli;
          blog.timeDT= 1;
          log(blog);                      /// put it in history buffer
          _gw->log(blog);
          _gp->log(blog);
        } /// failsafe

        if(chatty) printf("XInput: hid[%s] button RELEASE nr[%d]\n", name.d, a);

      } /// if press or depress
    } /// for each possible xpad button

    #endif /// USING_XINPUT
  } /// pass thru all modes
}


void Joystick::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    buffer1[a]= buffer2[a]= 0;
    bPressure[a]= 0;
    bTime[a]= 0;
  }

  /// reset logged buttons
  for(short a= 0; a< MAX_KEYS_LOGGED; a++) {
    lastBut[a].checked= true;                       /// mark all history as checked, so buttons get ignored
    if(lastBut[a].timeDown&& !lastBut[a].timeUp) {  /// button waiting dor depress
      lastBut[a].timeUp= osi.present/ 1000000;
      lastBut[a].timeDT= lastBut[a].timeUp- lastBut[a].timeDown;
    }
  }
}

// grab exclusive control of the joystick (if possible)
void Joystick::aquire() {
  if(mode== 1) {
    return;
  }

  if(mode== 2) {
    #ifdef USING_DIRECTINPUT
    if(diDevice) diDevice->Acquire();
    #endif /// USING_DIRECTINPUT
    return;
  }

  if(mode== 3) {
    #ifdef USING_XINPUT
    XInputEnable(TRUE);
    #endif /// USING_XINPUT
    return;
  }
}

// ungrab exclusive control of the joystick
void Joystick::unaquire() {
  if(mode== 1) {
    return;
  }

  if(mode== 2) {
    #ifdef USING_DIRECTINPUT
    if(diDevice) diDevice->Unacquire();
    #endif /// USING_DIRECTINPUT
    return;
  }

  if(mode== 3) {
    #ifdef USING_XINPUT
    XInputEnable(FALSE);
    #endif /// USING_XINPUT
    return;
  }
}


/// swap button buffers
void Joystick::swapBuffers() {
  lastCheck= b;
  b= (b== buffer1)? buffer2: buffer1;
}


void Joystick::log(const ButPressed &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastBut[a]= lastBut[a- 1];
  lastBut[0]= k;
}







// ------------============= GAMEPAD CLASS ===========--------------------
// ========================================================================

GamePad::GamePad() {
  _j= null;
  delData();
}

GamePad::~GamePad() {
}

void GamePad::delData() {
  mode= 0;                          // 0= DISABLED
  type= 0;                          // 0= ps3; 1= xbone - changing this in game would work
  if(_j) _j->deactivate();

  name.delData();
  
  /// clear axis
  lx= ly= 0;
  rx= ry= 0;
  lt= rt= 0;
  u= v= 0;
  pov= -1;

  b= buffer1;
  lastCheck= buffer2;

  resetButtons();
}


// clears all button buffers / resets logged buttons - called by in.resetPressedButtons() - when alt-tab / something similar
void GamePad::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    buffer1[a]= buffer2[a]= 0;
    bPressure[a]= 0;
    bTime[a]= 0;
  }

  /// reset logged buttons
  for(short a= 0; a< MAX_KEYS_LOGGED; a++) {
    lastBut[a].checked= true;                       /// mark all history as checked, so buttons get ignored
    if(lastBut[a].timeDown&& !lastBut[a].timeUp) {  /// button waiting dor depress
      lastBut[a].timeUp= osi.present/ 1000000;
      lastBut[a].timeDT= lastBut[a].timeUp- lastBut[a].timeDown;
    }
  }
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



void checkGamepadType(GamePad *p) {
  /// ps3 compatible devices won't be searched; they default on type 0
  p->type= 0;

  #ifdef OS_WIN
  if(p->name== "Microsoft X-Box 360 pad")
    p->type= 1;
  else if(p->name== "XBOX 360 For Windows (Controller)")
    p->type= 1;
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  if(p->name== "Microsoft X-Box 360 pad")
    p->type= 1;
  #endif /// OS_LINU
  
  #ifdef OS_MAC
  #endif /// OS_MAC

  // well, this list must be updated i guess; per OS, too
}




///========================================================================///
// ------------============= GAMEWHEEL CLASS ===========------------------- //
///========================================================================///

GameWheel::GameWheel() {
  _j= null;
  delData();
}

GameWheel::~GameWheel() {
}

void GameWheel::delData() {
  mode= 0;
  name.delData();
  if(_j) _j->deactivate();

  /// clear axis
  wheel= 0;
  a1= a2= a3= a4= a5= 0;      // THIS NEEDS MORE WORK
  // pov starts on -1, off state
  
  b= buffer1;
  lastCheck= buffer2;

  resetButtons();
}


// clears button buffer states / resets logged buttons history
void GameWheel::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    buffer1[a]= buffer2[a]= 0;
    bPressure[a]= 0;
    bTime[a]= 0;
  }

  /// reset logged buttons
  for(short a= 0; a< MAX_KEYS_LOGGED; a++) {
    lastBut[a].checked= true;                       /// mark all history as checked, so buttons get ignored
    if(lastBut[a].timeDown&& !lastBut[a].timeUp) {  /// button waiting dor depress
      lastBut[a].timeUp= osi.present/ 1000000;
      lastBut[a].timeDT= lastBut[a].timeUp- lastBut[a].timeDown;
    }
  }
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







// MAC specific fuctions. 

#ifdef OS_MAC 

///----------------///
// HIDDriver STRUCT //
///----------------///

/// part of HIDDriver structure. each 'element' can be a button or axis in a stick/pad/wheel
struct HIDElement {
  // SUBJECT OF DELETION  
  char type;                    /// 1= axis, 2= button THIS MIGHT GO AWAY/ usagePage IS A BETTER 'TYPE'
  
  short id;                     /// button or axis number (0 - max buttons / 0 - max axis)
  long usage, usagePage;        /// [most inportant] characteristics about this element (is it an x axis, is it a button etc)
  long logicalMin;              /// minimum value it can have
  long logicalMax;              /// maximum value it can have
  bool hasNULLstate;

  bool isHat;                   /// is it a hat switch / dPad
  bool hatMultiAxis;            /// if the hat has 2 axis (there are some sticks that have complex hats)
  bool hatAxis1;                /// [hat multi axis only] if it is the first axis (x)
  bool hatAxis2;                /// [hat multi axis only] if it is the second axis (y)
  HIDElement(): type(0), id(0), logicalMin(0), logicalMax(0), hasNULLstate(false), isHat(false), hatMultiAxis(false), hatAxis1(false), hatAxis2(false) {}
};

/// macs lack a proper joystick api; the next struct is a helper to 'decode' the mess that is almost raw reading from a HID device
struct HIDDriver {              /// [internal]
  bool inUse;                   /// is this in use?
  IOHIDDeviceRef device;        /// coresponding 'device' that this stick/pad/wheel is tied to
  short nrButtons;              /// number of buttons that this stick/pad/wheel has
  short nrAxis;                 /// number of axis this stick/pad/wheel has
  HIDElement *elem;             /// array of elements the device has
  
  /// standard constructor/destructor/delData(); everything will be set to 0 and memory will be auto-deallocated if allocated
  HIDDriver(): inUse(false), device(null), nrButtons(0), nrAxis(0), elem(0) {}
  void delData() { if(elem) delete[] elem; elem= null; nrButtons= nrAxis= 0; inUse= false; device= null; }
  ~HIDDriver() { delData(); }
} driver[MAX_JOYSTICKS];        /// [internal]




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
} /// hu_CreateDeviceMatchingDictionary



// CALLBACK FUNCTION: when a device is plugged in ---------------------------------
///================================================================================
static void HIDadded(void *context, IOReturn result, void *sender, IOHIDDeviceRef device) {
  // context:   context from IOHIDManagerRegisterDeviceMatchingCallback
  // result:    the result of the matching operation
  // sender:    the IOHIDManagerRef for the new device
  // device: the new HID device

  // IOHIDDeviceRegisterInputValueCallback(device, HIDchange, &driver[a]); this could further optimize some code, but very little, by passing &driver[a] after it is created...
  
  if(chatty) printf("%s\n", __FUNCTION__);

  /// find the first non-in-use joystick
  short a;
  for(a= 0; a< MAX_JOYSTICKS; a++)
    if(!driver[a].inUse)
      break;
  
  // start to 'populate' vars inside the helper struct

  /// sticks/pads/wheels 'numbers'
  in.nr.jFound++;  in.nr.jOS++;
  in.nr.gpFound++; in.nr.gpOS++;
  in.nr.gwFound++; in.nr.gwOS++;
  
  /// rest of vars
  driver[a].inUse= true;  
  driver[a].device= device;
  in.j[a].mode= 1;              /// set it's mode to 1. in macs i think only mode 1 will be avaible... there is no freaking support for HIDs
  in.gp[a].mode= 1;
  in.gw[a].mode= 1;
  
  /// stick/pad/wheel name (product name) ... this should be easy, right?...
  CFStringRef name;
  name= (CFStringRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));

  if(name && (CFGetTypeID(name)== CFStringGetTypeID())) {
    CFIndex length;
		
		CFStringGetBytes(name, CFRangeMake(0, CFStringGetLength(name)), kCFStringEncodingUTF8, '?', false, NULL, 100, &length);
    
    in.j[a].name.d= new char[length+ 1];
                             
		CFStringGetBytes(name, CFRangeMake(0, CFStringGetLength(name)), kCFStringEncodingUTF8, '?', false, (UInt8 *) in.j[a].name.d, length+ 1, NULL);
		in.j[a].name.d[length]= 0;  /// terminator
    in.j[a].name.updateLen();
    
    in.gp[a].name= in.j[a].name;
    in.gw[a].name= in.j[a].name;
  } else {
    in.j[a].name= "Unknown";
    in.gp[a].name= in.j[a].name;
    in.gw[a].name= in.j[a].name;
  }
  checkGamepadType(&in.gp[a]);
  
  /// not used ATM:
  // IOHIDDeviceGetVendorID(device);
  // IOHIDDeviceGetProductID(device);
  
  
  CFArrayRef elems;               /// array with all elements
	IOHIDElementRef elem;           /// 1 element
	IOHIDElementType type;          /// button / axis
	IOHIDElementCookie cookie;      /// cookies are element IDs basically. they point to first element as 1, not as 0, tho!
  /// get all elements the device has
	elems= IOHIDDeviceCopyMatchingElements(device, NULL, kIOHIDOptionsTypeNone);
  
  driver[a].elem= new HIDElement[CFArrayGetCount(elems)];
  
  /// populate driver struct
  short c= 0, d= 0; /// c will hold counter for axis, d for buttons
  for(short b= 0; b< CFArrayGetCount(elems); b++) {
		elem= (IOHIDElementRef)CFArrayGetValueAtIndex(elems, b);
		type= IOHIDElementGetType(elem);
    cookie= IOHIDElementGetCookie(elem); /// cookies represent a unique identifier for a HID element within a HID device.

    /// unfortunately type for axis is screwed... some axis are put into Misc by mac HIDapi...
		if(type== kIOHIDElementTypeInput_Misc || type== kIOHIDElementTypeInput_Axis) {
      driver[a].elem[b].type= 1;
      driver[a].elem[b].id= c++;
      driver[a].elem[b].usagePage=    IOHIDElementGetUsagePage(elem);
      driver[a].elem[b].usage=        IOHIDElementGetUsage(elem);
      driver[a].elem[b].logicalMin=   IOHIDElementGetLogicalMin(elem);
			driver[a].elem[b].logicalMax=   IOHIDElementGetLogicalMax(elem);
			driver[a].elem[b].hasNULLstate= IOHIDElementHasNullState(elem);
			driver[a].elem[b].isHat=        IOHIDElementGetUsage(elem)== kHIDUsage_GD_Hatswitch;
      
      /// if this is a hat / dPad, check if it is a multi-axis hat
      if(driver[a].elem[b].isHat) {
        if(b!= 0)                                     /// if it's the first element, this will avoid a nice crash
          if(driver[a].elem[b- 1].isHat) {            /// if the prev element is a hat / dPad too, then this is a multi-axis hat / dPad
            driver[a].elem[b- 1].hatMultiAxis= true;  /// set previous element as a multiAxis
            driver[a].elem[b- 1].hatAxis1= true;      /// set previous element as axis 1
            driver[a].elem[b].hatMultiAxis= true;     /// set current element as a multiAxis
            driver[a].elem[b].hatAxis2= true;         /// set current element as axis 2
          }
      } /// if this is a hat

    /// buttons
    } else if(type== kIOHIDElementTypeInput_Button) {
      driver[a].elem[b].type= 2;
      driver[a].elem[b].id= d++;
      driver[a].nrButtons++;
    }
    if(chatty) printf("element[%d]: cookie[%d] id[%d] type[%d] min[%ld] max[%ld] hasNULL[%d] isHat[%d]\n", b, cookie, driver[a].elem[b].id, driver[a].elem[b].type, driver[a].elem[b].logicalMin, driver[a].elem[b].logicalMax, driver[a].elem[b].hasNULLstate, driver[a].elem[b].isHat);

	} /// for each element
  
  in.j[a].maxButtons= driver[a].nrButtons;
  in.gp[a].maxButtons= driver[a].nrButtons;
  in.gw[a].maxButtons= driver[a].nrButtons;

	CFRelease(elems);     /// release elements array
	if(chatty) printf("device[%s] nrButtons[%d] nrAxis[%d]\n", in.j[a].name.d, driver[a].nrButtons, driver[a].nrAxis);
  

} /// HIDadded




// CALLBACK FUNCTION: when a device is removed ------------------------------------
///================================================================================
static void HIDremoved(void *context, IOReturn result, void *sender, IOHIDDeviceRef device) {
  // inContext:   context from IOHIDManagerRegisterDeviceMatchingCallback
  // inResult:    the result of the removing operation
  // inSender:    the IOHIDManagerRef for the device being removed
  // inHIDDevice: the removed HID device

  if(chatty) printf("%s", __FUNCTION__);

  /// find removed device in 'driver' struct
  short a;
  for(a= 0; a< MAX_JOYSTICKS; a++)
    if(driver[a].device== device)
      break;
  
  if(a== MAX_JOYSTICKS) {     /// if not found... well... something is wrong...
    error.simple("HIDremoved: can't find the requested device");
    return;
  }
  
  /// sticks/pads/wheels 'numbers'
  in.nr.jFound--;  in.nr.jOS--;
  in.nr.gpFound--; in.nr.gpOS--;
  in.nr.gwFound--; in.nr.gwOS--;

  in.j[a].mode= in.gp[a].mode= in.gw[a].mode= 0; /// mode 0 = DISABLED
  in.j[a].name.delData();
  in.gp[a].name.delData();
  in.gw[a].name.delData();
  
  driver[a].delData();
  if(chatty) printf(" helper cleared\n");
} /// HIDremoved





// CALLBACK FUNCTION: any value in a device (axis/button) has changed -------------
///================================================================================
void HIDchange(void *inContext, IOReturn inResult, void *inSender, IOHIDValueRef val) {
  // inContext:       context from IOHIDManagerRegisterInputValueCallback
  // IinResult:       completion result for the input value operation
  // inSender:        the IOHIDManagerRef
  // inIOHIDValueRef: the new element value
  
  // REMEMBER: this function must be optimized for speed, as it is called many times over;
  
  // further testing: it seems there is d-pad button pressure measurements... same with most of buttons !!!!
  
  IOHIDElementRef elem= IOHIDValueGetElement(val);        /// get the 'element' thet of the value
  IOHIDDeviceRef device= IOHIDElementGetDevice(elem);     /// get the HID device that a element belongs to
  IOHIDElementCookie cookie= IOHIDElementGetCookie(elem)- 1; /// cookies represent a unique identifier for a HID element (also first element they point to is 1, and the list starts with 0)

  /// find the stick/pad/wheel this change belongs to
  short a;
  for(a= 0; a< in.nr.jOS; a++)
    if(driver[a].device== device)         /// found it
      break;
  
  HIDElement *e= &driver[a].elem[cookie]; /// name shortcut
  
  if(!e->type) return;                    /// only type1(axis) and type2(butons) are handled. rest, return quick
    
  /// value translation to -32767 : +32767
  long amin= e->logicalMin< 0? -e->logicalMin: e->logicalMin; /// faster than calling abs()
  long amax= e->logicalMax< 0? -e->logicalMax: e->logicalMax; /// faster than calling abs()

  if((e->type== 1) && amin+ amax == 0)    /// quick way to eliminate trash (dunno what other 'axis' that mac reports are) / would be div by zero error too
    return;
  
  long v= IOHIDValueGetIntegerValue(val); /// the actual value that changed
  //  double v2= IOHIDValueGetScaledValue(val, kIOHIDValueScaleTypeCalibrated); /// it could be used
  //  double v3= IOHIDValueGetScaledValue(val, kIOHIDValueScaleTypePhysical);   /// i saw there are not big changes from v

  /// Returns the timestamp value associated with this HID value reference.
  /// !!! these timestamps are using mach too, so they are reliable !!!
  uint64_t time= IOHIDValueGetTimeStamp(val)/ 1000000;    /// convert to millisecs too (from nanosecs)
  
  if(chatty)  printf("%s", in.j[a].name.d);

  if(e->usagePage== kHIDPage_GenericDesktop) {            // ---=== axis ===---
    long t= (((v+ amin)* 65534)/ (amin+ amax))- 32767;   /// value scaled to min[-32767] max[+32767], 65535 total possible units (65534+ unit 0)
    if(t> -150&& t< 150) t= 0;                           /// this is due to bug in mac HID api. center position is not centered.
    
    switch(e->usage) {
      case kHIDUsage_GD_X:                       // [X axis?]   / [l stick X] / [wheel???]
        in.j[a].x= t;
        in.gp[a].lx= in.j[a].x;
        in.gw[a].wheel= in.j[a].x;

        if(chatty) printf(" lStick[%ld]x\n", in.gp[a].lx);
        return;
      case kHIDUsage_GD_Y:                       // [Y axis?]   / [l stick Y] / [wheel???]
        in.j[a].y= -t;
        in.gp[a].ly= -t;
        in.gw[a].a1= t;
        if(chatty) printf(" lStick[%ld]y\n", in.gp[a].ly);
        return;
      case kHIDUsage_GD_Z:                       // [Throttle?] / [r stick X] / [wheel???]
        in.j[a].throttle= 32767- t;
        if(in.gp[a].type== 0)      in.gp[a].rx= t;        /// ps3 compatible
        else if(in.gp[a].type== 1) in.gp[a].lt= 32767- t; /// xpad compatible
        in.gw[a].a2= in.j[a].throttle;
        if(chatty) printf(" rStick[%ld]x\n", in.gp[a].rx);
        return;
      case kHIDUsage_GD_Rx:                      // [extra1 X?] / [l trigger] / [wheel???]
        in.j[a].v= t;
        if(in.gp[a].type== 0)      in.gp[a].lt= 32767- t; /// ps3 compatible
        else if(in.gp[a].type== 1) in.gp[a].rx= t;        /// xpad compatible
        in.gw[a].a4= t;
        if(chatty) printf(" lTrigger[%ld]\n", in.gp[a].lt);
        return;
      case kHIDUsage_GD_Ry:                      // [extra1 Y?] / [r trigger] / [wheel???]
        in.j[a].u= -t;
        if(in.gp[a].type== 0)      in.gp[a].rt= 32767- t; /// ps3  compatible
        else if(in.gp[a].type== 1) in.gp[a].ry= -t;       /// xpad compatible
        in.gw[a].a5= t;
        if(chatty) printf(" rTrigger[%ld]\n", in.gp[a].rt);
        return;
      case kHIDUsage_GD_Rz:                      // [Rudder?]   / [rStick Y]  / [wheel???]
        in.j[a].rudder= 32767- t;
        if(in.gp[a].type== 0) in.gp[a].ry= -t;            /// ps3 compatible
        else if(in.gp[a].type== 1) in.gp[a].rt= 32767- t; /// xpad compatible
        in.gw[a].a3= t;
        if(chatty) printf(" rStick[%ld]y\n", in.gp[a].ry);
        return;
      case kHIDUsage_GD_Hatswitch:
        if(!e->hatMultiAxis) {                   // gamePad dPads have only 1 axis
          if(v> e->logicalMax|| v< e->logicalMin) {
            in.gp[a].pov= -1;
            in.j[a].pov= -1;
            // game wheels can have povs, i guess, dunno
          } else {
            in.gp[a].pov= (360/ (e->logicalMax+ 1))* v; /// in degrees
            in.j[a].pov= in.gp[a].pov;
            // game wheels can have povs, i guess, dunno
          }
          if(chatty) printf(" pov[%ld]\n", in.gp[a].pov);
        } else {                                 // multi-axis hat switch. CAN'T TEST THESE AS I DO NOT HAVE ONE
          //if(chatty) printf(" multi-axis hat switch not handled ATM... gotta buy one first!\n");
          // THIS CODE IS NOT TESTED, as i have no joystick with 2 axis hat switch
          if(e->hasNULLstate)
            if((v< e->logicalMin) && (v> e->logicalMax)) {
              in.j[a].pov= -1;
              in.gp[a].pov= -1;
              if(chatty) printf(" pov[%ld]\n", in.gp[a].pov);
              return;
              // GAMEWHEEL NOT DONE <<<<<<<<<<<<<<<
            }
            
          long x, y;                        /// they gotta be ints for exact 0 degrees or 90 degrees, else there are problems
          double pov;
          
          /// get axis from current pov position (wich is in degrees)
          pov= in.j[a].pov;
          x= y= 0;
          
          if(in.j[a].pov!= -1) {            /// ... only if it's not on -1 position (nothing selected)
            x= (double)(32767.0* sin(in.j[a].pov* (M_PI/ 180.0)));
            y= (double)(32767.0* cos(in.j[a].pov* (M_PI/ 180.0)));
          }
          
          /// update from changed value (v)
          long t= (((v+ amin)* 65534)/ (amin+ amax))- 32767;
          if(e->hatAxis1)                   /// this is x axis
            x= (t> -150 && t< 150)? 0: t;
          else                              /// else is y axis
            y= (t> -150 && t< 150)? 0: -t;  /// 'up' y axis, is negative; this needs to be changed 
          
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
          in.j[a].pov= pov;
          in.gp[a].pov= pov;
          // gw is not set<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
          
          if(chatty) printf(" pov[%ld]\n", in.gp[a].pov);
        } /// if multi-axis hat switch / pov / dPad / watever other name they come up with
        return;
      default:
        if(chatty) printf(" unhandled generic desktop element usage[%lx]\n", e->usage);
    } /// axis usage switch

  } else if(e->type== 2) {                                // ---=== button ===---
    /// ^^^ have to use the 'type' variable, wich is the only place it actually works... 

    /// SWAP BUTTON BUFFERS is done in Joystick::update(), for OS_MAC. It must be done once per frame, or per call to Input::update()
    /// OS_MAC usses events, therefore it can update sticks multiple times per frame (in between calls to Input::update(),
    ///   so swapbuffers must be handled differently
    
    /// gamepad button unification
    short but, extra;
    but= e->id;
    // ps3 compatible pad
    if(in.gp[a].type== 0) {
      extra= in.gp[a].maxButtons- 10;   /// gamepads have 10 normal buttons. the rest are marked as extra, and moved on button 10+
      
      if(extra && (e->id>= 6) && (e->id< 6+ extra))
        but+= 4;                        /// is it an extra button? -> moved on position 11+
      else if(extra && (e->id>= 6+ extra))
        but-= extra;                    /// all buttons above extra buttons, moved back on 7+
      
    // xbox compatible pad
    } else if(in.gp[a].type== 1) {
      // EXTRA BUTTONS NOT HANDLED ON XBOX controller <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      extra= in.gp[a].maxButtons- 11; /// normal xbone has 11 buttons. rest are extra, but i have no clue where they would go... don't have such pad
          
      if(ev[a].number== 8)
        but= 10;                      /// is it the xbox button? -> move it on position 10, with the extra buttons
      else if(ev[a].number>=9 && ev[a].number<= 10)
        but-= 1;                      /// move start & back to positions 8 & 9
    }
    
    in.j[a].b[e->id]= (uchar)v;
    in.gp[a].b[but]= (uchar)v;
    in.gw[a].b[e->id]= (uchar)v;
    
    ButPressed blog;
    
    if(v== 1) {                                           // PRESS
      in.j[a].bTime[e->id]=  time;
      in.gp[a].bTime[but]= time;
      in.gw[a].bTime[e->id]= time;
      blog.checked= false;
      blog.timeDown= time;
      blog.timeUp= 0;
      blog.timeDT= 0;
      blog.b= e->id;
      in.j[a].log(blog);
      in.gw[a].log(blog);
      blog.b= but;
      in.gp[a].log(blog);

      if(chatty) printf(" button PRESS [%d] arranged[%d]\n", e->id, but);
      
    } else if(v== 0) {                                    // RELEASE
      /// search thru history for the button, to mark the time it got released
      bool found= false;
      for(short b= 0; b< MAX_KEYS_LOGGED; b++)
        if(in.j[a].lastBut[b].b== e->id) {
          if(in.j[a].lastBut[b].timeUp) continue;    /// skip if this tested button is already released
          
          in.j[a].lastBut[b].timeUp= time;
          in.j[a].lastBut[b].timeDT= time- in.j[a].lastBut[b].timeDown;
          in.j[a].lastBut[b].checked= false;
          
          /// gamepad button nr could have a different number, but it does not matter
          in.gp[a].lastBut[b].timeUp= time;
          in.gp[a].lastBut[b].timeDT= in.gp[a].lastBut[b].timeDT;
          in.gp[a].lastBut[b].checked= false;
          
          in.gw[a].lastBut[b].timeUp= time;
          in.gw[a].lastBut[b].timeDT= in.gw[a].lastBut[b].timeDT;
          in.gw[a].lastBut[b].checked= false;
          
          found= true;
          break;
        }
      
      // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
      if(!found) {                          /// failsafe - normally it is found (but things can happen ... alt-tab?)
        // some debug stuff can be done here, tho
        blog.b= e->id;
        blog.checked= false;
        blog.timeDown= time- 1;             /// mark it as insta down-up
        blog.timeUp= time;
        blog.timeDT= 1;
        in.j[a].log(blog);                  /// put it in history buffer
        in.gw[a].log(blog);
        blog.b= but;
        in.gp[a].log(blog);
      } /// failsafe
      
      if(chatty) printf(" button RELEASE [%d] arranged[%d]\n", e->id, but);
    } /// if press / release
    
  } else if(e->usagePage== kHIDPage_VendorDefinedStart) { // ---=== button pressure / other vendor specifics ===---
    long t;
    if(e->usage>= 0x20 && e->usage<= 0x2B)/// button pressure range
      t= (((v+ amin)* 65534)/ (amin+ amax))- 32767;     /// value scaled to min[-32767] max[+32767], 65535 total possible units (65534+ unit 0)    
    switch(e->usage) {


        //        GAMEPAD BUTTON UNIFICATION CODE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

      // ---=== button pressures ===---
      case 0x20:                          /// gamepad button pov right
        in.j[a].bPressure[20]= in.gp[a].bPressure[20]= in.gw[a].bPressure[20]= t+ 32767;
        if(chatty) printf(" but 20 pressure[%ld]\n", in.gp[a].bPressure[20]);
        return;
      case 0x21:                          /// gamepad button pov left
        in.j[a].bPressure[21]= in.gp[a].bPressure[21]= in.gw[a].bPressure[21]= t+ 32767;
        if(chatty) printf(" but 21 pressure[%ld]\n", in.gp[a].bPressure[21]);
        return;
      case 0x22:                          /// gamepad button pov up
        in.j[a].bPressure[22]= in.gp[a].bPressure[22]= in.gw[a].bPressure[22]= t+ 32767;
        if(chatty) printf(" but 22 pressure[%ld]\n", in.gp[a].bPressure[22]);
        return;
      case 0x23:                          /// gamepad button pov down
        in.j[a].bPressure[23]= in.gp[a].bPressure[23]= in.gw[a].bPressure[23]= t+ 32767;
        if(chatty) printf(" but 23 pressure[%ld]\n", in.gp[a].bPressure[23]);
        return;
      case 0x24:                          /// gamepad button 3
        in.j[a].bPressure[3]= in.gp[a].bPressure[3]= in.gw[a].bPressure[3]= t+ 32767;
        if(chatty) printf(" but 3 pressure[%ld]\n", in.gp[a].bPressure[3]);
        return;
      case 0x25:                          /// gamepad button 2
        in.j[a].bPressure[2]= in.gp[a].bPressure[2]= in.gw[a].bPressure[2]= t+ 32767;
        if(chatty) printf(" but 2 pressure[%ld]\n", in.gp[a].bPressure[2]);
        return;
      case 0x26:                          /// gamepad button 1
        in.j[a].bPressure[1]= in.gp[a].bPressure[1]= in.gw[a].bPressure[1]= t+ 32767;
        if(chatty) printf(" but 1 pressure[%ld]\n", in.gp[a].bPressure[1]);
        return;
      case 0x27:                          /// gamepad button 0
        in.j[a].bPressure[0]= in.gp[a].bPressure[0]= in.gw[a].bPressure[0]= t+ 32767;
        if(chatty) printf(" but 0 pressure[%ld]\n", in.gp[a].bPressure[0]);
        return;
      case 0x28:                          /// gamepad button 4
        in.j[a].bPressure[4]= in.gp[a].bPressure[4]= in.gw[a].bPressure[4]= t+ 32767;
        if(chatty) printf(" but 4 pressure[%ld]\n", in.gp[a].bPressure[4]);
        return;
      case 0x29:                          /// gamepad button 5
        in.j[a].bPressure[5]= in.gp[a].bPressure[5]= in.gw[a].bPressure[5]= t+ 32767;
        if(chatty) printf(" but 5 pressure[%ld]\n", in.gp[a].bPressure[5]);        
        return;
      case 0x2A:                          /// gamepad button EXTRA 1
        in.j[a].bPressure[6]= in.gp[a].bPressure[6+ 4]= in.gw[a].bPressure[6]= t+ 32767;
        if(chatty) printf(" but 6 pressure[%ld]\n", in.gp[a].bPressure[6+ 4]);
        return;
      case 0x2B:                          /// gamepad button EXTRA 2
        in.j[a].bPressure[7]= in.gp[a].bPressure[7+ 4]= in.gw[a].bPressure[7]= t+ 32767;
        if(chatty) printf(" but 7 pressure[%ld]\n", in.gp[a].bPressure[7+ 4]);
        return;
      default:
        if(chatty) printf(" unhandled vendor specific element usage[%lx]\n", e->usage);
    }
    
  } else if(e->usagePage== kHIDPage_Button) {             // ---=== ofc, this page doesn't work for game HIDs ===---
    /// this page is NOT USED (for gamepads at least, so i guess it is not used for any game HID)
    if(chatty) printf(" unhandled usagePage == button\n");

    
  } else
    if(chatty) printf(" unhandled HID element: usagePage[%lx] usage[%lx]\n", e->usagePage, e->usage);
  
  

  /* SOME DOCUMENTATION:
  // usage and usage pages are defined on the USB website at: <http://www.usb.org> [couldn't find them, ofc. use IOHIDUsageTables.h]
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
  
  /// no clue what these are  
  // element report information
  uint32_t reportID = IOHIDElementGetReportID(elem);
  uint32_t reportSize = IOHIDElementGetReportSize(elem);
  uint32_t reportCount = IOHIDElementGetReportCount(elem);
  
  // element unit & exponent
  uint32_t unit = IOHIDElementGetUnit(elem);
  uint32_t unitExp = IOHIDElementGetUnitExponent(elem);

  //There are also functions to determine the device, parent, and child of a specified HID element:
  
  // return the collection element that a HID element belongs to (if any)
  IOHIDElementRef parent= IOHIDElementGetParent(elem);
  
  // return the child elements of a collection element (if any)
  CFArrayRef tCFArrayRef= IOHIDElementGetChildren(elem);
   
   // check more on this (another guy that digged thru this nightmare):
   // http://sacredsoftware.net/svn/misc/StemLibProjects/gamepad/trunk/source/gamepad/Gamepad_macosx.c
*/
    
} /// Handle_IOHIDInputValueCallback


/* NOT USING, can't find no documentation about reports ATM
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







