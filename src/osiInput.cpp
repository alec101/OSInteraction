#include "osinteraction.h"
#include "util/typeShortcuts.h"
#include <math.h>
#ifdef OSI_BE_CHATTY
#include <stdio.h>
#endif

#ifdef OSI_USING_DIRECTINPUT
  #ifndef DIRECTINPUT_VERSION
    #define DIRECTINPUT_VERSION 0x0800
  #endif
  #include OSI_DINPUTINCLUDE
#endif
#ifdef OSI_USING_XINPUT
  #include OSI_XINPUTINCLUDE
#endif



#ifdef OS_LINUX
#include <unistd.h>
#include <X11/XKBlib.h>
//#include <linux/joystick.h>   // it's not x stuff... lots of crap added, keyboard/mouse, that is not needed. IT'S POSSIBLE TO AVOID THIS HEADER, only some function definitions are needed.
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

// avoid including joystick.h, which includes tons of unneeded stuff
typedef uint8 __u8;
#define JS_EVENT_BUTTON 0x01                              // button pressed/released
#define JS_EVENT_AXIS   0x02                              // joystick moved
#define JS_EVENT_INIT   0x80
#define JSIOCGVERSION   _IOR('j', 0x01, __u8)             // get driver version
#define JSIOCGAXES      _IOR('j', 0x11, __u8)             // get number of axes
#define JSIOCGBUTTONS   _IOR('j', 0x12, __u8)             // get number of buttons
#define JSIOCGNAME(len) _IOC(_IOC_READ, 'j', 0x13, len)   // get identifier string
#define EVIOCGNAME(len) _IOC(_IOC_READ, 'E', 0x06, len)   // get device name
struct js_event {
  uint32 time;    // event timestamp in milliseconds
  int16 value;    // value
  uint8 type;     // event type
  uint8 number;   // axis/button number
};
#endif /// OS_LINUX

#ifdef OS_MAC
#undef uint
#include <IOKit/hid/IOHIDLib.h>         // human interface devices (joysticks/gamepads/gamewheels)
#include <CoreGraphics/CoreGraphics.h>  // trying to pinpoint quartz
#endif

#ifdef USING_DIRECTINPUT
BOOL CALLBACK _diDevCallback(LPCDIDEVICEINSTANCE, LPVOID); // this is direct input 'callback' func
#endif

// private funcs
void checkGamepadType(osiGamePad *p);  // it is found in the gamepad area, at the end
  
/* TODO:
 * 
 * [all] grab=/= aquire; there's active too... 
 *       grab for mouse= cursor can't get out of windows boundaries
 *       grab for keyboard= exclusive control DANGEROUS - maybe avoid it
 *       grab for sticks= this should be default, as only 1 program can read the stick
 *       aquired for mouse= active / check what init's must be done (set the grabbing, etc)
 *       aquired for keyboard= active / check what inits must be done (set the grabbing, etc)
 *       aquired for sticks= active / check what inits must be done (set the grabbing, etc)
 *    
 *       maybe aquire ---> activate??
 * 
 *       but, there should be either aquired or active. NOT BOTH
 * 
 * [all]more mouse funcs- showCursor, a grab function that doesn't allow for the cursor to exit the windows (this is not very easy, if the monitors are weirdly positioned, but it is usefull)
 *        but, the mouse position can be checked against some boxes, if not in any box (window/ desktop window) then move it back
 *
 * buy a ffeedback joystick
 * buy a wheel
 * 
 * [maybe] buttons to be on 32 bit integers? 1<< a is button a? good for copy-ing
 *         what happens with the keyboard? int256? int64 * 4? this needs further thinking
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

void osiInput::vibrate() {
  #ifdef OS_WIN
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
    int32 hr= 0;
        DWORD    rgdwAxes[2] = { (DWORD)DIJOFS_X, (DWORD)DIJOFS_Y };  // X- and y-axis
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
  #endif /// OS_WIN

}

#ifdef OS_WIN
#ifdef USING_DIRECTINPUT
BOOL CALLBACK EnumEffectsCallback(LPCDIEFFECTINFO di, LPVOID pvRef) {
    //DInputFFB * ffb = (DInputFFB*) pvRef;


    // Pointer to calling device
    //ffb->ffbcaps.ConstantForce = DIEFT_GETTYPE(di->dwEffType) == DIEFT_CONSTANTFORCE;
    //ffb->ffbcaps.PeriodicForce = DIEFT_GETTYPE(di->dwEffType) == DIEFT_PERIODIC;
  
    //printf(" Effect '%ls'. IsConstant = %d, IsPeriodic = %d", di->tszName, DIEFT_GETTYPE(di->dwEffType) == DIEFT_CONSTANTFORCE, DIEFT_GETTYPE(di->dwEffType) == DIEFT_PERIODIC);

    return DIENUM_CONTINUE;
}
#endif
#endif /// OS_WIN
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

osiInput::osiInput() {
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
  #ifdef USING_DIRECTINPUT
  _dInput= null;
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

  #ifdef OS_LINUX
  mode2Name= mode3Name= "Not Used";
  #endif /// OS_LINUX

  #ifdef OS_MAC
  mode2Name= mode3Name= "Not Used";
  k.numLock= true;    /// macs don't handle num locks. this will always be on
  #endif /// OS_MAC
}


osiInput::~osiInput() {
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(_dInput)
    _dInput->Release();
  _dInput= null;
  #endif
  #endif /// OS_WIN

  #ifdef OS_MAC
  IOHIDManagerClose((IOHIDManagerRef)_manager, kIOHIDOptionsTypeNone); /// close the HID manager
  CFRelease((IOHIDManagerRef)_manager);                                /// delloc memory
  #endif /// OS_MAC
}


void osiInput::delData() {
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
bool osiInput::init(int mMode, int kMode) {
  m.mode= (int8)mMode;
  k.mode= (int8)kMode;
  #ifdef OS_LINUX
  m.mode= k.mode= 1;
  #endif
  #ifdef OS_MAC
  m.mode= k.mode= 1;
  #endif
  k.activate();
  m.activate();
  
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(!_dInput)
    if(DirectInput8Create(osi.primWin->_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_dInput, NULL)!= DI_OK) {
      error.simple("Could not initialize Direct Input");
      return false;
    }
  #endif
  #endif /// OS_WIN

  /// fill in all vars, find all xinput/directinput devices, etc
  populate(true);

  /// Kv struct has (almost) all keyboard keys. It has to 'populate' all vars @ start
  Kv.populate();

  #ifdef OS_MAC
  
  in._manager= IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);

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
  IOHIDManagerSetDeviceMatchingMultiple((IOHIDManagerRef)_manager, arr);
  
  CFRelease(arr);                               // release array
  
  
  IOHIDManagerRegisterDeviceMatchingCallback((IOHIDManagerRef)_manager, HIDadded, NULL);  /// callback function for when a matching HID is added
  IOHIDManagerRegisterDeviceRemovalCallback((IOHIDManagerRef)_manager, HIDremoved, NULL); /// callback function for when a matching HID is removed
  
  // RUN LOOPS? MAYBE WON'T NEED THEM
  IOHIDManagerScheduleWithRunLoop((IOHIDManagerRef)_manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
  
  /// open the manager
  IOHIDManagerOpen((IOHIDManagerRef)_manager, kIOHIDOptionsTypeNone); // option bits: kIOHIDOptionsTypeNone or kIOHIDOptionsTypeSeizeDevice
  
  
  /// register a callback function when a value from any registered HIDs change
  IOHIDManagerRegisterInputValueCallback((IOHIDManagerRef)_manager, HIDchange, NULL);  // this... works... if u have the enigma codebreaker (the one that didn't have the actual machine to crack the enigma code)
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
bool _HIDdisconnected= false;
void osiInput::populate(bool scanMouseKeyboard) {
  #ifdef OSI_BE_CHATTY
  /// debug
  bool chatty= false;
  bool timer= false;
  #endif

  _lastPopulate= osi.present;

  // DO NOT SCAN FOR MOUSE & KEYBOARD EACH SECOND !!! - it can be done but it is extremly laggy
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

  #ifdef OSI_BE_CHATTY
  uint64 start, end;
  if(timer) osi.getNanosecs(&start);
  #endif

  // xlib xinput research: ... after some time, found nothing. TOO OLD LIB?
  
  // this function takes 5-10 mil nanosecs (5-10 millisecs), which is HUGE
  // linux [MODE 1] using "linux/joystick.h". system driver
  
  int f;
  int version, axes= 0, buttons= 0;
  uint8 name[256];
  //short id= 0;
  bool addEventFile= false;
  bool found;
  
  /// big speed increase can be made with static pointers "/dev/input/js  " (space for 2 decimals)
  /// and just put 2 numbers in there with nr/10, nr%10
  static str8 s1("/dev/input/js");
  static str8 s2("/dev/input/event");
  str8 s3;
  
  /// searching for 32 js[X] files
  for(short a= 0; a< 32; a++) {
    
    /// this limit to 8 can be changed if neccesary... can't predict what will happen in 10-20 years....
    if(nr.jOS== 8) {
      error.console("osinput::init: Maximum number of jSticks/gPads/gWheels reached (where did you plug more than 8?)", false, null);
      break;
    }
    
    /// check if this id is already in use by some joystick struct
    found= false;
    for(short b= 0; b< 8; b++) {
      if(j[b]._jsID== a)
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
        j[b]._jsFile= f;                        /// joystick class handles this
        j[b]._jsID= a;                          /// joystick class handles this
        j[b].name= gp[b].name= gw[b].name= (char *)name;/// stick name / product name

        /*
        joystick name! IT HAS TO HAVE A NAME, if name== null? make it "Unknown" , 
        but IT WON'T MATCH IN EVENT FILE NAME READ, more thinking to this
        maybe set name, after the event match
        */
        
        j[b].maxButtons= 
          gp[b].maxButtons= 
          gw[b].maxButtons= (int16)buttons;     /// nr of buttons the stick has
        
        addEventFile= true;                     /// set flag to search for it's event file too
    
        /// better to have lots of vars. The other option would be having complicated search algorithms just to find out how many joysticks found that use driver N
        ///   the purpose is to have the same code run on every OS... game searches for driver type 1 (os driver), type 2(in win is directinput) etc...
        nr.jFound++;  nr.gpFound++; nr.gwFound++; 
        nr.jOS++;     nr.gpOS++;    nr.gwOS++;
        
        checkGamepadType(&gp[b]);               /// check if it is ps3/ xbone compatible
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("joystick[%d] %s Axes: %d Buttons: %d Version: %d CONNECTED\n", b, name, axes, buttons, version);
        #endif
        break;
      }
    
  }
  
  /// if no event file needs to be found, just return
  if(!addEventFile) {
    #ifdef OSI_BE_CHATTY
    if(timer) osi.getNanosecs(&end);
    if(timer) printf("linux joystick scan: %llu nanosecs\n", (unsigned long long)(end- start));
    #endif
    return;
  }
  
  // event files asociated with each stick
  
  for(short a= 0; a< 32; a++) {                     /// search thru event0-> event31
    /// search thru all joysticks for this id
    found= false;
    for(short b= 0; b< 8; b++) {
      if(j[b]._eventID== a)
        found= true;
    }
    if(found) continue;                             /// if found, this file is already open

    /// try to open this file & read what joystick it belongs to    
    f= version= axes= buttons= name[0]= 0;
    s3.f("%s%d", s2.d, a);                          /// '/dev/input/event[a]'
    
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
      if(j[b]._jsFile!= -1)                          /// joystick struct must have a js file
        if((j[b]._eventFile== -1) && (j[b].name== (cchar *)name)) {
          j[b]._eventFile= f;
          j[b]._eventID= a;
          #ifdef OSI_BE_CHATTY
          if(chatty) printf("event file: %s%d belongs to joystick %d\n", s2.d, a, b);
          #endif
          found= true;
          break;
        }
    
    /// no match found? close the file; must be part of something else
    if(!found)
      close(f);
  } /// for each possible 'event' file

  #ifdef OSI_BE_CHATTY
  if(timer) osi.getNanosecs(&end);
  if(timer) printf("linux joystick scan: %llu nanosecs\n", (unsigned long long)(end- start));
  #endif

  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  /// mac uses calback functions. check HIDadded() / HIDremoved() at the end of this file
  #endif /// OS_MAC


  // ------------============ MODE 2 JOYSTICKS ===============------------
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  #ifdef OSI_BE_CHATTY
  if(timer) osi.getNanosecs(&start);
  #endif
  /// jConnected helps to check for disconnected joysticks; start with false, and each connected stick must mark 'true'
  /// if a stick doesn't mark jConnected[id] as true, it is DISCONNECTED
  for(short a= 0; a< 8; a++)
    jConnected[a]= false;

  _dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, _diDevCallback, NULL, DIEDFL_ATTACHEDONLY);

  for(short a= 0; a< 8; a++) {
    if(j[a+ 8].mode&& !jConnected[a]) {     // a joystick was disconnected
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("DirectInput: hid[%d] %s DISCONNECTED\n", a+ 8, j[a+ 8].name.d);
      #endif
      /// clear all axis / buttons data / dinput driver
      j[a+ 8].delData();
      gp[a+ 8].delData();
      gw[a+ 8].delData();
      /// update numbers
      in.nr.jT2--;    in.nr.gpT2--;    in.nr.gwT2--;
      in.nr.jFound--; in.nr.gpFound--; in.nr.gwFound--;
      _HIDdisconnected= true;
    }
  }

  #ifdef OSI_BE_CHATTY
  if(timer) osi.getNanosecs(&end);
  if(timer) printf("direct input joystick check: %llu nanosecs %llu millisecs\n", end- start, (end- start)/ 1000000);
  #endif

  /// direct input 'plugged' state is in diDevCallback(..)
  /// direct input 'unplugged' state is set in Joystick::update(); if it can't read from device, calls delData()

  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

  // ------------============ MODE 3 JOYSTICKS ===============------------

  #ifdef OS_WIN
  #ifdef USING_XINPUT

  #ifdef OSI_BE_CHATTY
  if(timer) osi.getNanosecs(&start);
  #endif
  
  XINPUT_STATE state;
  
  for(short a= 0; a< 4; a++ ) { /// XUSER_MAX_COUNT = max xbones, but 4 is max design set too in osi
    /// Simply get the state of the controller from XInput.  
    if(XInputGetState(a, &state)== ERROR_SUCCESS) {
      if(!j[16+ a].mode) {          /// new stick found
        j[16+ a]._id= a;

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
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("XInput controller %d CONNECTED\n", a);
        #endif
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
        _HIDdisconnected= true;
        
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("XInput controller %d DISCONNECTED\n", a);
        #endif
      }
    }
  } /// for each possible xinput slot

  #ifdef OSI_BE_CHATTY
  if(timer) osi.getNanosecs(&end);
  if(timer) printf("xinput joystick check: %llu nanosecs\n", end- start);
  #endif
  
  #endif /// USING_XINPUT
  #endif /// OS_WIN
}

#ifdef OS_WIN
#ifdef USING_DIRECTINPUT
BOOL CALLBACK _diDevCallback(LPCDIDEVICEINSTANCE inst, LPVOID extra) {
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  #endif

  /// can't handle more than 8 direct input sticks (don't think this will change very soon)
  if(in.nr.jT2== 8)
    return DIENUM_CONTINUE;

  /// check if this device is already active
  for(short a= 8; a< 16; a++) {
    if(in.j[a]._diID== inst->guidInstance) {
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
  if(in._dInput->CreateDevice(inst->guidInstance, &in.j[id]._diDevice, NULL)                       != DI_OK) fail= true;
  if(in.j[id]._diDevice->SetDataFormat(&c_dfDIJoystick2)                                           != DI_OK) fail= true;
  if(in.j[id]._diDevice->SetCooperativeLevel(osi.primWin->_hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND)!= DI_OK) fail= true;

  if(fail) {
    error.console("diDevCallback(): couldn't add the new device", false, null);
    if(in.j[id]._diDevice)
      in.j[id]._diDevice->Release();
    in.j[id]._diDevice= null;
    return DIENUM_CONTINUE;
  }

  in.j[id]._diID= inst->guidInstance;                  /// this is used to distinguish between new sticks and already in use sticks

  /// device capabilities 
  // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.didevcaps(v=vs.85).aspx
  DIDEVCAPS caps;
  caps.dwSize= sizeof(DIDEVCAPS);
  in.j[id]._diDevice->GetCapabilities(&caps);
  //(FORCE FEEDBACK FLAG? maybe?)<<<<<<<<<

  in.j[id].maxButtons= (int16)caps.dwButtons; /// number of buttons device has
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

  #ifdef OSI_BE_CHATTY
  if(chatty) printf("DirectInput: found joystick[%d]: %s axes[%d] buttons[%d]\n", id, in.j[id].name.d, (int)caps.dwAxes, (int)caps.dwButtons);
  #endif

  return DIENUM_CONTINUE;         /// DIENUM_CONTINUE to continue enumerating devices; else it will stop enumerating
}
#endif /// USING_DIRECTINPUT
#endif /// OS_WIN



///=============================================================================
// ######################### MAIN INPUT UPDATE #################################
///=============================================================================
void osiInput::update() {
  #ifdef OSI_BE_CHATTY
  uint64 start, end;    /// debug
  bool timer= false;    /// debug
  if(timer) osi.getNanosecs(&start);
  #endif
  
  /// set flags
  if(_HIDdisconnected) {
    _HIDdisconnected= false;
    osi.flags.HIDlostConnection= true;
  } else
    osi.flags.HIDlostConnection= false;
  
  /// scan for joysticks every second (? maybe 2 secs or 3?)
  // DISABLED ATM. can be enabled under linux (must check timer times)
  #ifdef OS_LINUX
  //if(osi.present- lastPopulate> 1000000000)
    //populate();
  #endif /// OS_LINUX
  
  /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  check linux+mac joysticks. buttons & everything
  carefull with axis!! -x and -y must be left and down
  buttons 7& 8 must be back(select) / start
  check timers 
   */
  
  

  /// update mouse
  m.update();
  
  /// update keyboard
  //if(k.mode!= 1)
  k.update();
  
  
  /// update system handled sticks/pads/wheels
  if(nr.jOS)
    for(short a= 0; a< 8; a++)
      if(j[a]._bActive)
        j[a].update();
  
  /// update [type 2] sticks/pads/wheels
  if(nr.jT2)
    for(short a= 8; a< 16; a++)
      if(j[a]._bActive)
        j[a].update();
  
  /// update [type 3] sticks/pads/wheels
  if(nr.jT3)
    for(short a= 16; a< 20; a++)
      if(j[a]._bActive)
        j[a].update();

  #ifdef OSI_BE_CHATTY
  if(timer) osi.getNanosecs(&end);
  if(timer) printf("Input::update() timer: %llu nanosecs\n", (unsigned long long)(end- start)); // it's nothing
  #endif
}


// resets all buttons for all devices- usually called on alt/tab - cmd/tab or something similar
void osiInput::resetPressedButtons() {
  m.resetButtons();
  k.resetButtons();
  for(short a= 0; a< MAX_JOYSTICKS; a++) {
    j[a].resetButtons();
    gp[a].resetButtons();
    gw[a].resetButtons();
  }
}

void osiInput::resetAxis() {
  for(int a= 0; a< MAX_JOYSTICKS; a++) {
    j[a].resetAxis();
    gp[a].resetAxis();
    gw[a].resetAxis();
  }
}







// ------------============= MOUSE CLASS ===========--------------------
// =====================================================================

osiMouse::osiMouse() {
  mode= 1;
  x= y= dx= dy= oldx= oldy= wheel= 0;
  _twheel= 0;      /// used with wheel position, internal
  _bActive= false;
  _bGrabbed= false;

  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  _diDevice= null;
  _diStats= {0};
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN
}


osiMouse::~osiMouse() {
  delData();
}


void osiMouse::delData() {
  mode= 1;
  _twheel= 0;
  x= y= dx= dy= oldx= oldy= wheel= 0;
  _bActive= _bGrabbed= false;
  
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(_diDevice) {
    _diDevice->Unacquire();
    _diDevice->Release();
    _diDevice= null;
  }
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

}


bool osiMouse::init(int8 mode) {
  delData();

  this->mode= mode;
  
  if(mode== 1) {          /// no init required atm
    return activate(); 
  }
  if(mode== 2) {          /// no init required atm
    return activate();
  }

  if(mode== 3) {
    #ifdef OS_WIN
    #ifdef USING_DIRECTINPUT
    // if(!primWin) return false;      /// primary window is needed for direct input
    int32 hr= 0;
    DIDATAFORMAT mouseformat;
    mouseformat= c_dfDIMouse;
    mouseformat.dwFlags= DIDF_RELAXIS; 

    hr= in._dInput->CreateDevice(GUID_SysMouse, &_diDevice, NULL);

    hr= _diDevice->SetDataFormat(&mouseformat);
    if(osi.primWin)
      hr= _diDevice->SetCooperativeLevel(osi.primWin->_hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
    _diDevice->Acquire();

    if(hr!= DI_OK) {
      error.simple("Could not initialize the mouse under DirectInput.");
      return false;
    }

    if(_diDevice== NULL) {
      return false;
    }

    return activate();  /// reached this point, returns a success
    #endif /// USING_DIRECTINPUT
    #endif /// OS_WIN
  }
  return false;     /// this point is reaced-> fail
}


bool osiMouse::activate() {
  if(!osi.primWin) { _bActive= false; return false; }
  _bActive= true;
  
  /// any init is done here
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(mode== 3)
    if(_diDevice->Acquire()!= DI_OK) {
      _bActive= false;
      return false;
    }
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

  /// if mouse is set to be grabbed, grab it here
  if(_bGrabbed)
    return grab();
  
  return true;
}


bool osiMouse::unactivate() {
  if(!osi.primWin) { _bActive= false; return false; }
  _bActive= false;  
  
  /// any uninit is done here
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(mode== 3)
    if(_diDevice->Unacquire()!= DI_OK)
      return false;
  #endif
  #endif /// OS_WIN

  
  /// if grabbed, ungrab it
  if(_bGrabbed)
    return ungrab();
  
  return true;
}

/// grab exclusive control of the mouse (if possible)
bool osiMouse::grab() {
  _bGrabbed= true;
  
  /* THIS GRABS ONLY FOR 1 WINDOW. A SPECIAL GRAB MUST BE DONE TO ALLOW THE MOUSE TO ROAM ALL THE WINDOWS
  #ifdef OS_LINUX
  if(mode== 1) {
    XGrabPointer(osi._dis, osi.primWin->_win,
                 True,              // send events or not
                 ButtonPressMask| ButtonReleaseMask| PointerMotionMask,
                 GrabModeSync,      // pointer - GrabMode[Async/Sync]
                 GrabModeSync,      // keyboard- GrabMode[Async/Sync]
                 osi.primWin->_win, // confine cursor to a window
                 None,              // mouse cursor (graphics)
                 CurrentTime);
    return true;
  }
  #endif /// OS_LINUX
  */

  return true;
}


/// release exclusive control of the mouse
bool osiMouse::ungrab() {
  _bGrabbed= false;
  
  /* THIS WORKS ONLY FOR 1 WINDOW
  #ifdef OS_LINUX
  if(mode== 1)
    XUngrabPointer(osi._dis, CurrentTime);
  #endif
  */
  return true;
}


void osiMouse::setPos(int in_x, int in_y) {
  #ifdef OS_WIN
    #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
    SetCursorPos(in_x, osi.display.vyMax- in_y);
    #endif
    #ifdef OSI_USE_ORIGIN_TOP_LEFT
    SetCursorPos(in_x, in_y);
    #endif
  #endif /// OS_WIN

  #ifdef OS_LINUX
    #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
    XWarpPointer(osi._dis, None, None, 0, 0, 0, 0, in_x, osi.display.vyMax- in_y);
    #endif
    #ifdef OSI_USE_ORIGIN_TOP_LEFT
    XWarpPointer(osi._dis, None, None, 0, 0, 0, 0, in_x, in_y);
    #endif
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
    CGPoint p;
    p.x= (float)in_x;
    #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
    p.y= (float)in_y;
    #endif
    #ifdef OSI_USE_ORIGIN_TOP_LEFT
    p.y= (float)(osi.display.vyMax- in_y);
    #endif
    CGWarpMouseCursorPosition(p);
  #endif /// OS_MAC
}





// ########################## MOUSE UPDATE ##############################
// when not using default MODE 1, call this func to update mouse vars - or better, call Input::update()
void osiMouse::update() {
  if(!osi.flags.haveFocus) return;
  if(!_bActive)            return;
  
  static int tx= 0, ty= 0;      /// temporary vars, used in mouse positioning and the wheel

  uint64 present= osi.present/ 1000000;

  if(mode== 1) {
    /// oldx&y and deltas must be done per in.update();
    /// REMEMBER: there can be multiple mouse move messages between in.update() calls
    oldx= tx;
    oldy= ty;
    tx= x;
    ty= y;
    dx= x- oldx;
    dy= y- oldy;
    wheel= _twheel;      /// twheel is updated using system messages, and set to 0 here, after it's read
    _twheel= 0;
  }
    
  /// manual update mode
  
  else if(mode== 2) {
    #ifdef OS_WIN
    /// mouse position
    POINT p;
    GetCursorPos(&p);
    oldx= x;
    oldy= y;
    x= p.x;
    #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT     /// coordonate unification
    y= osi.display.vyMax- p.y;
    #endif
    #ifdef OSI_USE_ORIGIN_TOP_LEFT        /// coordonate unification
    y= p.y;
    #endif
    dx= x- oldx;      /// replaced += with =; deltas are updated on each in.update() call
    dy= y- oldy;

    
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

      if(but[a].down&& !t) {          /// button release
        but[a].lastTimeStart= but[a].timeStart;
        but[a].lastTimeEnded= present;
        but[a].lastDT= but[a].lastTimeEnded- but[a].lastTimeStart;
        but[a].down= t;
      }
      if((!but[a].down)&& t) {        /// button press started
        but[a].timeStart= present;
        but[a].down= t;
      }
    }

    #endif /// OS_WIN

    #ifdef OS_LINUX
    // cant find any funcs. events are the only option i can find
    //error.simple("Mouse cannot be manually updated under linux. use [MODE 1]");
    #endif /// OS_LINUX

    #ifdef OS_MAC
    //    makeme .., very low priority tho
    #endif /// OS_MAC
  }
    
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT           /// skip some checks. only mode 1 works atm in linux
  else if(mode== 3) {

    /// direct input
    _diDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&_diStats);
    /// mouse position
    oldx= x;
    oldy= y;
    x= _diStats.lX;
    #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
    y= osi.display.vyMax- _diStats.lY;   /// coordonate unification
    #endif
    #ifdef OSI_USE_ORIGIN_TOP_LEFT
    y= _diStats.lY;
    #endif

    dx= x- oldx;      /// removed +=, made =; 
    dy= y- oldy;
    
    /// mouse wheel
    // THIS PART NEEDS TESTING, DUNNO WHAT DIRECT INPUT TRANMITS !!!!!!!!!!!!!!!  ALL DIRECT X PART, even. NEED A WORKING GLPRINT FUNC
    wheel= 0;
    if(_diStats.lZ)
      wheel++;
    else
      wheel--;

    /// mouse buttons
    bool t;
    
    for(short a= 0; a< 8; a++) {
      t= _diStats.rgbButtons[a]? true: false;
      if((but[a].down)&& !t) {                  /// a button press ended
        but[a].lastTimeStart= but[a].timeStart;
        but[a].lastTimeEnded= present;
        but[a].lastDT= but[a].lastTimeEnded- but[a].lastTimeStart;
        but[a].down= t;
      }
      if((!but[a].down)&& t) {                  /// a button press starts
        but[a].timeStart= present;
        but[a].down= t;
      }
    }
  } /// pass thru all mouse modes
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN
}



/// clears button states, usefull when alt-tabbing, so things don't get messed up
void osiMouse::resetButtons() {
  for(short a= 0; a< MAX_MOUSE_BUTTONS; a++) {
    if(but[a].down) {
      but[a].lastTimeStart= but[a].timeStart;
      but[a].lastTimeEnded= osi.present/ 1000000;
      but[a].lastDT= but[a].lastTimeEnded- but[a].lastTimeStart;
      but[a].down= false;
    }
  }
}




// ------------============= KEYBOARD CLASS ===========--------------------
// ========================================================================

osiKeyboard::osiKeyboard():
charTyped(40, sizeof(chTyped), true) {
//, manipTyped(40, sizeof(chTyped), true) {
  mode= 1;
  _bActive= _bGrabbed= false;
  delData();
}


osiKeyboard::~osiKeyboard() {
//	delData();
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(_diDevice) {
    _diDevice->Unacquire();
    _diDevice->Release();
    _diDevice= null;
  }
  #endif
  #endif /// OS_WIN
}


void osiKeyboard::delData() {
  _bActive= _bGrabbed= false;
  
  key= _buffer1;
  lastCheck= _buffer2;
  capsLock= scrollLock= numLock= false;       /// the 3 toggle locks
  insertLock= false;

  /// clear buffers
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++) {
    _buffer1[a]= _buffer2[a]= 0;
    keyTime[a]= 0;
    //    repeat[a]= 0;
  }
  clearTypedBuffer();                         /// character/manip streams too
  
  /// mark initial history keys as checked, so they get ignored
  for(short a= 0; a< MAX_KEYS_LOGGED; a++)
    lastKey[a].checked= true;
}


// could be called, but using in.init() is better, as it inits every device
bool osiKeyboard::init(int8 mode) {
  this->mode= mode;
  
  if(mode== 1)
    return activate();
  if(mode== 2)
    return activate();
  if(mode== 3) {
    // MOVE ALL THIS? SCRAP INIT????? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    #ifdef OS_WIN
    #ifdef USING_DIRECTINPUT
    if(!osi.primWin) return false;      /// a primary window must exist for directInput to work
    
    int32 hr= 0;
        
    hr= in._dInput->CreateDevice(GUID_SysKeyboard, &_diDevice, NULL);
    hr= _diDevice->SetDataFormat(&c_dfDIKeyboard);
    hr= _diDevice->SetCooperativeLevel(osi.primWin->_hWnd, DISCL_EXCLUSIVE| DISCL_FOREGROUND);
    _diDevice->Acquire();

    if(hr!= DI_OK) {
      error.simple("Could not initialize the keyboard under DirectInput.");
      return false;
    }

    if(_diDevice== NULL)
      return false;

    return activate();
    #endif /// USING_DIRECTINPUT
    #endif /// OS_WIN
  }
  return false;
}


bool osiKeyboard::activate() {
  _bActive= true;
  
  /// any init is done here
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(mode== 3)
    if(_diDevice->Acquire()!= DI_OK) {
      _bActive= false;
      return false;
    }
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

  /// if keyboard is set to be grabbed, grab it here
  if(_bGrabbed)
    return grab();
  
  return true;
}


bool osiKeyboard::unactivate() {
  if(!osi.primWin) { _bActive= false; return false; }
  _bActive= false;  
  
  /// any uninit is done here
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  if(mode== 3)
    if(_diDevice->Unacquire()!= DI_OK)
      return false;
  #endif
  #endif /// OS_WIN

  
  /// if grabbed, ungrab it
  if(_bGrabbed)
    return ungrab();
  
  return true;
}




// grab exclusive control of the keyboard (if possible)
bool osiKeyboard::grab() {
  if(!osi.primWin) { _bGrabbed= false; return false; }
  
  _bGrabbed= true;
  
  #ifdef OS_LINUX
  if(mode== 1) {
    XGrabKeyboard(osi._dis, osi.primWin->_win,
                  False,          // owner events (i think stops sending sys events)
                  GrabModeSync,   // pointer - GrabMode[Async/Sync]
                  GrabModeSync,   // keyboard- GrabMode[Async/Sync]
                  CurrentTime);
    
  }
  #endif /// OS_LINUX
  return true;
}

// ungrab exclusive control of the keyboard
bool osiKeyboard::ungrab() {
  _bGrabbed= false;
  
  #ifdef OS_LINUX
  if(mode== 1) {
    XUngrabKeyboard(osi._dis, CurrentTime);
    return true;
  }
  #endif /// OS_LINUX

  return true;
}







// ############################ KEYBOARD UPDATE func ####################################
// if not using MODE 1, call this (autocalled in Input::update() )
void osiKeyboard::update() {
  if(!osi.flags.haveFocus) return;
  if(!_bActive)            return;
  
  if(mode== 1)    // nothing to update, >>>>>>...... hmm maybe the LOCK STATES....<<<<<<<<<<<
    return;
  
  #ifdef OS_WIN
  else if(mode== 2) {
    swapBuffers();			/// ye, swap them buffers
    GetKeyboardState(key);
    /// do not return;
  }
  
  #ifdef USING_DIRECTINPUT
  else if(mode== 3) {
    swapBuffers();
    _diDevice->GetDeviceState(MAX_KEYBOARD_KEYS, key);
    /// do not return
  }
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN
  
  #ifdef OS_WIN // i can't think of a way atm, to use this in linux. the only mode is [mode 1]
  
  uint64 presentMilli= osi.present/ 1000000;              /// present time in milliseconds

  /// check if a key just started to be pressed or a key was depressed - might be a little time consuming
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
    if((key[a]& 0x80)&& !(lastCheck[a]& 0x80) ) {         // key press start
      keyTime[a]= presentMilli;
      /// log the key in history
      osiKeyLog k;
      k.code= a;
      k.checked= false;
      k.timeDown= presentMilli;
      k.timeUp= 0;
      k.timeDT= 0;

      _log(k);

      /// insert lock manage
      if(a== in.Kv.insert)
        in.k.insertLock= (in.k.insertLock? false: true);

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
        osiKeyLog k;
        k.code= a;
        k.checked= false;
        k.timeDown= presentMilli- 1;
        k.timeUp= presentMilli;
        k.timeDT= 1;
        _log(k);                                     /// put it in history buffer
      }
    }
  #endif /// OS_WIN
}


// updates keyboard lock states (numlock/capslock/scrolllock)
void osiKeyboard::updateLocks() {
  #ifdef OS_WIN
  capsLock=   (GetKeyState(VK_CAPITAL)& 0x01) == 1;
  numLock=    (GetKeyState(VK_NUMLOCK)& 0x01) == 1;
  scrollLock= (GetKeyState(VK_SCROLL)&  0x01) == 1;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  uint n;
  XkbGetIndicatorState(osi.primWin->_dis, XkbUseCoreKbd, &n);
  capsLock=   (n& 0x01);
  numLock=    (n& 0x02);
  scrollLock= (n& 0x04);
  #endif /// OS_LINUX

  #ifdef OS_MAC
  // makeme - better to have this!!!  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  #endif /// OS_MAC
}





// clears all character buffers, ususally called when switching to a new/ existing input box / control
void osiKeyboard::clearTypedBuffer() {
  while(charTyped.first)
    charTyped.del(charTyped.first);
  //while(manipTyped.first)
    //manipTyped.del(manipTyped.first);
}


// clears buffers and resets all logged keys, usually called when alt-tabbing (losing focus)
void osiKeyboard::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++) {
    _buffer1[a]= _buffer2[a]= 0;
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
uint32 osiKeyboard::getChar() {
  if(!charTyped.nrNodes)
    return 0;

  /// clear old typed characters (must have stayed in buffer longer than 1 sec)
  uint64 present= osi.present/ 1000000;
  
  while(charTyped.first)
    if( (present- ((chTyped*)charTyped.last)->time)> 1000)  // if character stayed in buffer longer than 1 second, delete. IS 1 SEC OK?????
      charTyped.del(charTyped.last);
    else                                                    /// the rest of chars couldn't have stayed in buffer longer than 1 sec if the last char didn't
      break;

  if(!charTyped.last) return 0;
  /// return the last character in buffer (usually there is only 1... maybe in cases of low framerate, loading stuff while typeing... etc)
  uint32 ret= ((chTyped*)(charTyped.last))->c;
  charTyped.del(charTyped.last);                            /// del character from buffer
  return ret;
}

/*
// basically same func as getChar, but for the other stream of string manipulation keys
uint32 osiKeyboard::getManip() {
  if(!manipTyped.nrNodes)
    return 0;
  
  /// clear old typed characters (must have stayed in buffer longer than 1 sec)
  uint64 present= osi.present/ 1000000;
  
  while(manipTyped.first)
    if( (present- ((chTyped*)manipTyped.last)->time)> 1000) // if character stayed in buffer longer than 1 second, delete. IS 1 SEC OK?????
      manipTyped.del(manipTyped.last);
    else                                                    /// the rest of chars couldn't have stayed in buffer longer than 1 sec if the last char didn't
      break;

  if(!manipTyped.last) return 0;
  /// return the last character in buffer (usually there is only 1... maybe in cases of low framerate, loading stuff while typeing... etc)
  uint32 ret= ((chTyped*)(manipTyped.last))->c;
  manipTyped.del(manipTyped.last);                          /// del character from buffer
  return ret;
  
}
*/

// [internal]
void osiKeyboard::_addChar(uint32_t c, uint64 *time) {
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

/*
// [internal] identical as addChar...
void osiKeyboard::_addManip(uint32 c, uint64 *time) {
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
*/

// might never be used... gets the first key that is down... not much uses for this other than checking if any key is pressed
int16 osiKeyboard::getFirstKey() {
  for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
    if(key[a])
      return a;

  return -1;	/// fail
}


/// internal stuff
void osiKeyboard::swapBuffers() {
  lastCheck= key;
  key= (key== _buffer1)? _buffer2: _buffer1;
}


// [internal] logs a key to histoty of keys that were pressed
void osiKeyboard::_log(const osiKeyboard::osiKeyLog &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    lastKey[a]= lastKey[a- 1];
  lastKey[0]= k;
}



// debugging
void osiKeyboard::printPressed() {
  if((mode== 2) || (mode== 3))
    for(short a= 0; a< MAX_KEYBOARD_KEYS; a++)
      if(key[a]& 0x80)
        printf(" kc%d(%d)", a, key[a]);
}









///========================================================================///
// ------------============= JOYSTICK CLASS ===========-------------------- //
///========================================================================///

osiJoystick::osiJoystick() {
  mode= 0;                    /// mode set to 0 = DISABLED
  _gp= null;                  /// must start null
  _gw= null;                  /// must start null
  _bActive= false;
  _bGrabbed= true;            /// default for sticks is in grabbed state
  
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  _diDevice= null;
  _diID= {0};
  #endif
  #endif

  #ifdef OS_LINUX
  _jsFile= -1;
  _eventFile= -1;
  _jsID= -1;
  _eventID= -1;
  #endif /// OS_LINUX

  delData();
}


osiJoystick::~osiJoystick() {
  delData();
}


void osiJoystick::delData() {
  mode= 0;
  name.delData();
  

  x= y= x2= y2= throttle= rudder= u= v= 0;
  pov= -1;
  
  but= _buffer1;
  butPrev= _buffer2;

  resetButtons();
  
  #ifdef OS_WIN
  _id= -1;
  #ifdef USING_DIRECTINPUT
  if(_diDevice) {
    _diDevice->Unacquire();
    _diDevice->Release();
    _diDevice= null;
  }
  _diID= { 0 };
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

  #ifdef OS_LINUX
  /// close driver files if currently opened
  if(_jsFile!= -1)
    close(_jsFile);
  if(_eventFile!= -1)
    close(_eventFile);

  _jsFile= -1;
  _jsID= -1;
  _eventFile= -1;
  _eventID= -1;
  #endif /// OS_LINUX
  deactivate();
}



// ############### JOYSTICK UPDATE #################
/// handles pads/wheels too. might be a good ideea to call this func if others are called, dunno (or just remove update() from gp/gw)

void osiJoystick::update() {
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  #endif

  if(!_bActive) return;                           /// a stick/pad/wheel must be marked as active, for updating to take place
                                                  /// this must be used, as updating same device with mutiple drives, will create a bad mess

                                                  // if player 1 selects device0 with xinput
                                                  // and player 2 selects device0 with directinput??? what then? some tests must be done somehow

  uint64 presentMilli= osi.present/ 1000000;      /// present time in milliseconds
  osiButLog blog;


  // -----------============ MODE 1 JOYSTICKS ============------------
  if(mode== 1) {
    #ifdef OS_WIN
    
    // CRASHES TO DESKTOP, BUGGY READING, USES DIRECT INPUT ANYWAYS  >>> SCRAP <<<
    /*
    bool found;
    
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
    y=        -(int32)jinfo.dwYpos+ 32767;            /// main stick Y axis
    throttle= jinfo.dwZpos;                           /// throttle? need a joystick...
    rudder=   jinfo.dwRpos;                           /// rudder
    pov=      (jinfo.dwPOV== 65535)? -1: jinfo.dwPOV; /// pov in degrees* 100
    v=        jinfo.dwVpos- 32767;                    /// extra axis 6
    u=        -(int32)jinfo.dwUpos+ 32767;            /// extra axis 5
    

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
    n= read(_jsFile, ev, sizeof(ev));

    if(n== -1) {                            /// no event happened / joystick unplugged

      // check if the joystick was UNPLUGGED (read sets errno on EBADF: file does not exist anymore)
      //printf("e[%d]", errno);
      //if(errno== ENODEV) {
        #ifdef OS_BE_CHATTY
        if(chatty) printf("joystick %s REMOVED\n", name.d);
        #endif

        /// set stick as DISABLED & close all opened driver files
        delData();
        _gp->delData();
        _gw->delData();

        /// update sticks numbers
        in.nr.jFound--; in.nr.gpFound--; in.nr.gwFound--;
        in.nr.jOS--;    in.nr.gpOS--;    in.nr.gwOS--;
        
        osi.flags.HIDlostConnection= true;
      //} /// if the file is not found (stick disconnected)
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
        
        int16 nbut, extra;
        nbut= ev[a].number;
        /// gamepad buttons unification. extra buttons (including xbox main button are set after button 10)
        
        /// ps3 compatible pad
        if(in.gp[a].type== 0) {
          extra= _gp->maxButtons- 10; /// normal ps3 has 10 buttons. the rest are extra, on modified ps3 pads
          
          /// extra buttons are located on position 6, and the last 4 buttons that are not extra are the last (bad positioning imho)
          if(extra && (ev[a].number>= 6) && (ev[a].number< 6+ extra))
            nbut+= 4;                      /// extra buttons moved on position 10+ (over the last 4 buttons)
          else if(extra && (ev[a].number>= 6+ extra))
            nbut-= extra;                  /// last 4 buttons moved back on position 6+
          
        /// xbox compatible pad
        } else if(in.gp[a].type== 1) {
          // EXTRA BUTTONS NOT HANDLED ON XBOX controller <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
          extra= _gp->maxButtons- 11; /// normal xbone has 11 buttons. rest are extra, but i have no clue where they would go... don't have such pad
          
          
          if(ev[a].number== 8)
            nbut= 10;                      /// is it the xbox button? -> move it on position 10, with the extra buttons
          else if(ev[a].number>=9 && ev[a].number<= 10)
            nbut-= 1;                      /// move start & back to positions 8 & 9
        }

        /// swapbuffers won't work. just update lastCheck[]
        butPrev[ev[a].number]= but[ev[a].number];
        _gp->butPrev[nbut]= _gp->but[nbut];
        _gw->butPrev[ev[a].number]= but[ev[a].number];
        
        /// update button state
        but[ev[a].number]=  (uint8)ev[a].value;
        _gp->but[nbut]= (uint8)ev[a].value;
        _gw->but[ev[a].number]= (uint8)ev[a].value;
        
        /// update history
        if(ev[a].value== 1) {                      // button PRESS
          butTime[ev[a].number]= ev[a].time;
          _gp->butTime[nbut]= ev[a].time;
          _gw->butTime[ev[a].number]= ev[a].time;
          /// put the button in history
          blog.but= ev[a].number;
          blog.checked= false;
          blog.timeDown= butTime[nbut];
          blog.timeUp= 0;
          blog.timeDT= 0;
          _log(blog);
          _gw->_log(blog);
          blog.but= nbut;
          _gp->_log(blog);

          #ifdef OSI_BE_CHATTY
          if(chatty) printf("hid[%s] button PRESS nr[%d] arranged[%d]\n", name.d, ev[a].number, nbut);
          #endif

        } else if(ev[a].value== 0) {               // button RELEASE
          /// search thru history for the button, to mark the time it got released
          found= false;
          for(short b= 0; b< MAX_KEYS_LOGGED; b++)
            if(butLog[b].but== ev[a].number) {
              if(butLog[b].timeUp) continue;    /// skip if this tested button is already released

              butLog[b].timeUp= ev[a].time;
              butLog[b].timeDT= ev[a].time- butLog[b].timeDown;
              butLog[b].checked= false;

              /// gamepad button nr could have a different number, but it does not matter
              _gp->butLog[b].timeUp= ev[a].time;
              _gp->butLog[b].timeDT= butLog[b].timeDT;
              _gp->butLog[b].checked= false;

              _gw->butLog[b].timeUp= ev[a].time;
              _gw->butLog[b].timeDT= butLog[b].timeDT;
              _gw->butLog[b].checked= false;

              found= true;
              break;
            }
        
          // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
          if(!found) {                      /// failsafe - normally it is found (but things can happen ... alt-tab?)
            // some debug stuff can be done here, tho
            blog.but= ev[a].number;
            blog.checked= false;
            blog.timeDown= ev[a].time- 1;   /// mark it as insta down-up
            blog.timeUp= ev[a].time;
            blog.timeDT= 1;
            _log(blog);                     /// put it in history buffer
            _gw->_log(blog);
            blog.but= nbut;
            _gp->_log(blog);
          } /// failsafe

          #ifdef OSI_BE_CHATTY
          if(chatty) printf("hid[%s] button RELEASE nr[%d] arranged[%d]\n", name.d, ev[a].number, nbut);
          #endif

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
            y= ev[a].value;
            _gp->ly= ev[a].value;
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
              _gp->ry= ev[a].value;        /// xbone [right stick Y]
            // gw <<<<<<<<<<<<<<<<<<<<<<<<<<
            break;
          case 5:                           // [Rudder?]   / [rStick Y]  / [wheel???]
            rudder= ev[a].value;
            if(_gp->type== 0)
              _gp->ry= ev[a].value;        /// ps3   [right stick Y]
            else
              _gp->rt= 32767+ ev[a].value; /// xbone [right trigger]
            _gw->a4= ev[a].value;
            break;
          case 6:                         // [POV X?]    / [POV X]     / [wheel???]
          case 7:                         // [POV Y?]    / [POV Y]     / [wheel???]
            int32 tx, ty;          // they gotta be integers for exact 0 degrees or 90 degrees, else there are problems
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
            #ifdef OSI_BE_CHATTY
            if(chatty) printf("unhandled axis event!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            #endif
            ;
        }
      }
      #ifdef OSI_BE_CHATTY
      else if(chatty) printf("unhandled other joystick event!!!!!!!!!!!!!!!!!!!!!\n");
      #endif
        
    } /// for each event
    
    // did it actually read 64 events? read another 64 then...
    if(nev == 64)
      goto ReadAgain;
    
    #endif /// OS_LINUX

    #ifdef OS_MAC

    /// lock _cbTame struct
    _cbTame.mutex.lock();

    // ---=== AXIS ===---

    x= _cbTame.x;
    _gp->lx= x;
    _gw->wheel= x;

    y= _cbTame.y;
    _gp->ly= y;
    _gw->a1= y;

    throttle= _cbTame.throttle;
         if(_gp->type== 0) _gp->rx= throttle;           /// ps3 compatible
    else if(_gp->type== 1) _gp->lt= throttle+ 32767;    /// xpad compatible
    _gw->a2= throttle;

    v= _cbTame.v;
         if(_gp->type== 0) _gp->lt= 32767- v;           /// ps3 compatible
    else if(_gp->type== 1) _gp->rx= v;                  /// xpad compatible
    _gw->a4= v;

    u= _cbTame.u;
         if(_gp->type== 0) _gp->rt= 32767- u;           /// ps3 compatible
    else if(_gp->type== 1) _gp->ry= u;                  /// xpad compatible
    _gw->a5= u;

    rudder= _cbTame.rudder;
         if(_gp->type== 0) _gp->ry= rudder;            /// ps3 compatible
    else if(_gp->type== 1) _gp->rt= rudder;             /// xpad compatible
    _gw->a3= rudder;

    pov= _cbTame.pov;
    _gp->pov= pov;
    //_gw->pov <<<

    // ---=== BUTTONS ===---
    
    _swapBuffers();
    _gp->_swapBuffers();
    _gw->_swapBuffers();


    int extra;        /// number of extra buttons on gamepad
    osiButLog blog;   /// used for button logging

         if(_gp->type== 0) extra= _gp->maxButtons- 10;  /// ps3 compatible - 10 normal buttons
    else if(_gp->type== 1) extra= _gp->maxButtons- 11;  /// xbox compatible - 11 normal buttons
    
    /// pass thru all HID's buttons
    for(int a= 0, b= 0; a< maxButtons; a++, b++) {
      
      but[a]= _cbTame.but[a];
      butPressure[a]= _cbTame.butPressure[a];
      _gw->but[a]= but[a];
      _gw->butPressure[a]= butPressure[a];

      /// ps3 compatible gamepad
      if(_gp->type== 0) {
        if(extra && a== 6)
          b+= 4;                          /// move extra buttons over 4 positions (last 4 buttons are not extra)
        else if(extra && (a== 6+ extra))
          b= 6;                      /// move last 4 buttons to position 6
        
      /// xbox compatible gamepad
      } else if(_gp->type== 1) {
             if(a== 8)  b= 10;    /// move button 8 on 10 (xbox button at the end)
        else if(a== 9)  b= 8;     /// move buttons 9 and 10 back one position
        else if(a== 11) b= 11;    /// exra buttons should be normal from here on
      }

      _gp->but[b]= but[a];
      _gp->butPressure[b]= butPressure[a];

      // button PRESS
      if((!butPrev[a]) && but[a]) {
        butTime[a]= presentMilli;
        _gp->butTime[b]= presentMilli;
        _gw->butTime[a]= presentMilli;

        blog.checked= false;
        blog.timeDown= presentMilli;
        blog.timeUp= blog.timeDT= 0;
        blog.but= a;
        _log(blog);
        _gw->_log(blog);
        blog.but= b;
        _gp->_log(blog);
        // if(chatty) printf("button PRESS - joystick/wheel[%d] gamepad[%d]\n", a, b);

      // button RELEASE
      } else if(butPrev[a] && (!but[a])) {
        bool found= false;
        for(int c= 0; c< MAX_KEYS_LOGGED; c++)
          if(butLog[c].but== a) {
            if(butLog[c].timeUp) continue;

            butLog[c].timeUp= presentMilli;
            butLog[c].timeDT= presentMilli- butLog[c].timeDown;
            butLog[c].checked= false;

            _gp->butLog[c].timeUp= presentMilli;
            _gp->butLog[c].timeDT= butLog[c].timeDT;
            _gp->butLog[c].checked= false;

            _gw->butLog[c].timeUp= presentMilli;
            _gw->butLog[c].timeDT= butLog[c].timeDT;
            _gw->butLog[c].checked= false;
            found= true;
            break;
          }

        /// if somehow the button was not in the logs
        if(!found) {                          /// failsafe - normally it is found (but things can happen ... alt-tab?)
          blog.but= a;
          blog.checked= false;
          blog.timeDown= presentMilli- 1;             /// mark it as insta down-up
          blog.timeUp= presentMilli;
          blog.timeDT= 1;
          _log(blog);                  /// put it in history buffer
          _gw->_log(blog);
          blog.but= b;
          _gp->_log(blog);
        } /// failsafe
      }

      // if(chatty) printf(" button RELEASE joystick/gamewheel[%d] gamepad[%d]\n", a, b);
    }
    
    /// pov pressure
    butPressure[20]= _cbTame.butPressure[20];
    butPressure[21]= _cbTame.butPressure[21];
    butPressure[22]= _cbTame.butPressure[22];
    butPressure[23]= _cbTame.butPressure[23];
    _gp->butPressure[20]= _gw->butPressure[20]= butPressure[20];
    _gp->butPressure[21]= _gw->butPressure[21]= butPressure[21];
    _gp->butPressure[22]= _gw->butPressure[22]= butPressure[22];
    _gp->butPressure[23]= _gw->butPressure[23]= butPressure[23];
    _cbTame.mutex.unlock();
    #endif /// OS_MAC
  }

  // -----------============ MODE 2 JOYSTICKS ============------------
  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  else if(mode== 2) {       // win(DirectInput) linux(n/a) mac(n/a)
    if(!_bGrabbed) return;
    if(!osi.flags.haveFocus) return;

    if(_diDevice->GetDeviceState(sizeof(DIJOYSTATE2), (LPVOID)&_diStats)== DIERR_INPUTLOST) {     /// device DISCONNECTED
      _diDevice->Acquire();

      /// after 1 aquire attempt, still not works, signal it disconnected
      if(_diDevice->GetDeviceState(sizeof(DIJOYSTATE2), (LPVOID)&_diStats)== DIERR_INPUTLOST) {   /// device DISCONNECTED
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("DirectInput: hid[%s] DISCONNECTED<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,\n", name.d);
        #endif
        /// clear all axis / buttons data / dinput driver
        delData();
        _gp->delData();
        _gw->delData();
        /// update numbers
        in.nr.jT2--;    in.nr.gpT2--;    in.nr.gwT2--;
        in.nr.jFound--; in.nr.gpFound--; in.nr.gwFound--;
        osi.flags.HIDlostConnection= true;
      
        return;
      }
    }

    /// adjust axis to osi standard (-32767 / +32767)
    _diStats.lX-=           (_diStats.lX>  32768?          32768: 32767);
    _diStats.lY-=           (_diStats.lY>  32768?          32768: 32767);
    _diStats.lZ-=           (_diStats.lZ>  32768?          32768: 32767);
    _diStats.lRx-=          (_diStats.lRx> 32768?          32768: 32767);
    _diStats.lRy-=          (_diStats.lRy> 32768?          32768: 32767);
    _diStats.lRz-=          (_diStats.lRz> 32768?          32768: 32767);
    _diStats.rglSlider[0]-= (_diStats.rglSlider[0]> 32768? 32768: 32767);
    _diStats.rglSlider[1]-= (_diStats.rglSlider[1]> 32768? 32768: 32767);

    // ---=== JOYSTICK axis ===---
    x=          _diStats.lX;           /// main stick X
    y=          _diStats.lY;           /// main stick Y
    throttle=   _diStats.lZ;           /// throttle
    rudder=     _diStats.lRz;          /// rudder
    pov=        _diStats.rgdwPOV[0];   /// pov
    u=          -_diStats.lRx;          /// extra axis 1
    v=          -_diStats.lRy;          /// extra axis 2

    // ---=== GAMEPAD axis ===---
    _gp->lx=    _diStats.lX;           /// left stick X
    _gp->ly=    -_diStats.lY;          /// left stick Y
    _gp->pov=   _diStats.rgdwPOV[0];   /// pov (0- 359 degrees * 100, -1 neutral)

    if(_gp->type== 0) {                // ps3 compatible
      _gp->rx= _diStats.lZ;            /// right stick X
      _gp->ry= -_diStats.lRz;          /// right stick Y
      _gp->lt= (32767- _diStats.lRx)/ 2;    /// left trigger
      _gp->rt= (32767- _diStats.lRy)/ 2;    /// right trigger
    } else if(_gp->type== 1) {         // xbone compatible
      _gp->rx= _diStats.lRx;           /// right stick X
      _gp->ry= -_diStats.lRy;          /// right stick Y
      _gp->lt= -_diStats.lZ;           /// left trigger
      // this is probably a Direct Input BUG; they meant diStats.lRz; as it is now, 2 axis are fused into 1... wich is GREAT!
      _gp->rt= -_diStats.lZ;  /// right trigger
    }

    // ---=== GAMEWHEEL axis ===---
    _gw->wheel= _diStats.lX;           /// wheel position
    _gw->a1=    _diStats.lY;           /// other axis 1
    _gw->a2=    _diStats.lZ;           /// other axis 2
    _gw->a3=    _diStats.lRx;          /// other axis 3
    _gw->a4=    _diStats.lRy;          /// other axis 4
    _gw->a5=    _diStats.lRz;          /// other axis 5

    // ---=== stick/pad/wheel BUTTONS ===---
    uint8 _but, extra;
    _swapBuffers();                      /// lastCheck will hold the previous buttons states
    _gp->_swapBuffers();
    _gw->_swapBuffers();

    // >> something wrong with all buttons from dinput&OS drivers
    // >> test if mode 1 reads ok the xbox controller... if not, scrape, check what WM_messages there are...

    for(int16 a= 0; a< maxButtons; a++) {     /// for each possible button
      /*
        * gamepad buttons are messy. there should be an order in buttons, done by osi, i think, for GAMEPAD UNIFICATION
        *   the first 10 buttons are on all gamepads
        *   the rest of buttons shuld be 'extra' buttons, (including the xbox button), and arranged after button 9
        *   there has to be a variable that holds the total number of buttons the stick has
        *   thrustmaster pad's extra buttons are on 6-> n
        *   xbox 'xbox button' is on 8; it is changed on 10, and butotns 9 & 10 moved back 1 position
        */
      
      /// gamepad buttons unification. extra buttons (including xbox main button are set after button 9)
      _but= (uint8)a;    
      // ps3 compatible pad
      if(_gp->type== 0) {
        extra= _gp->maxButtons- 10;      /// gamepads have 10 normal buttons. the rest are marked as extra, and moved on button 10+
          
        if(extra && (a>= 6) && (a< 6+ extra))
          _but+= 4;                      /// extra buttons moved on position 10+ (over the last 4 buttons)
        else if(extra && (a>= 6+ extra))
          _but-= extra;                  /// last 4 buttons moved on position 6+

      // xbox compatible pad
      } else if(_gp->type== 1) {
        /// direct input i don't think will ever see extra buttons... i got 1 extra and it's nowhere to be found

        /// the first 4 buttons are arranged as the ps3 pad
        if(a< 4) {
          if(a== 0)      _but= 1;
          else if(a== 1) _but= 2;
          else if(a== 2) _but= 0;
        }
      } /// check type of gamepad

      /// update current button state
      but[a]= _diStats.rgbButtons[a]? 1: 0;
      _gp->but[_but]= but[a];
      _gw->but[a]= but[a];

      // --------------============== BUTTON PRESS ================-----------------
      if(but[a]&& !butPrev[a]) {
        /// mark button press time
        butTime[a]= presentMilli;
        _gp->butTime[_but]= butTime[a];
        _gw->butTime[a]= butTime[a];
        /// log the button in history
        blog.but= (uint8)a;
        blog.checked= false;
        blog.timeDown= presentMilli;
        blog.timeUp= 0;
        blog.timeDT= 0;
        _log(blog);
        _gw->_log(blog);
        blog.but= _but;
        _gp->_log(blog);
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("DirectInput: hid[%s] button PRESS nr[%d] arranged[%d]\n", name.d, a, _but);
        #endif
      
      // --------------============= BUTTON RELEASE ===============-----------------
      } else if(butPrev[a]&& !but[a]) {

        /// search thru history for the button, to mark the time it got released
        found= false;
        for(short b= 0; b< MAX_KEYS_LOGGED; b++)
          if(butLog[b].but== a) {
            if(butLog[b].timeUp) continue;    /// skip if this tested button is already released

            butLog[b].timeUp= presentMilli;
            butLog[b].timeDT= presentMilli- butLog[b].timeDown;
            butLog[b].checked= false;

            /// gamepad button nr could have a different number, but it does not matter
            _gp->butLog[b].timeUp= presentMilli;
            _gp->butLog[b].timeDT= butLog[b].timeDT;
            _gp->butLog[b].checked= false;
  
            _gw->butLog[b].timeUp= presentMilli;
            _gw->butLog[b].timeDT= butLog[b].timeDT;
            _gw->butLog[b].checked= false;

            found= true;
            break;
          }
        
        // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
        if(!found) {                      /// failsafe - normally it is found (but things can happen ... alt-tab?)
          // some debug stuff can be done here, tho
          blog.but= (uint8)a;
          blog.checked= false;
          blog.timeDown= presentMilli- 1; /// mark it as insta down-up
          blog.timeUp= presentMilli;
          blog.timeDT= 1;
          _log(blog);                      /// put it in history buffer
          _gw->_log(blog);
          blog.but= _but;
          _gp->_log(blog);
        } /// failsafe

        #ifdef OSI_BE_CHATTY
        if(chatty) printf("DirectInput: hid[%s] button RELEASE nr[%d] arranged[%d]\n", name.d, a, _but);
        #endif

      } /// button press or release
    } /// for each button
     
  }
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

  // -----------============ MODE 3 JOYSTICKS ============------------
  #ifdef OS_WIN
  #ifdef USING_XINPUT
  else if(mode== 3) {       // win(XInput) linux(n/a) mac(n/a)
    XINPUT_STATE xi;

    Str::memclr(&xi, sizeof(xi));                             /// zero memory
    XInputGetState(_id, &xi);                                 /// read data from driver (update xi)

    /// finding pov in degrees* 100
    int32 tpov;
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
    uint16 _lt= (xi.Gamepad.bLeftTrigger*  65534)/ 255; /// temp vars used, as it gotta be converted to short
    uint16 _rt= (xi.Gamepad.bRightTrigger* 65534)/ 255; /// temp vars used, as it gotta be converted to short
     
    // ---=== JOYSTICK axis ===---
    x=        xi.Gamepad.sThumbLX;        /// main stick X
    y=        xi.Gamepad.sThumbLY;        /// main stick Y
    throttle= _rt;                        /// throttle
    rudder=   _lt;                        /// rudder
    pov=      tpov;                       /// pov
    u=        xi.Gamepad.sThumbRX;        /// extra axis 1
    v=        -xi.Gamepad.sThumbRY;        /// extra axis 2
    
    // ---=== GAMEPAD axis ===---
    /// windows uses 32768 units for negative axis, and 32767 units for positive axis... (0 is considered center in osi)
    _gp->lx=  xi.Gamepad.sThumbLX;        /// left stick X
    _gp->ly=  xi.Gamepad.sThumbLY;        /// left stick Y
    _gp->rx=  xi.Gamepad.sThumbRX;        /// right stick X
    _gp->ry=  xi.Gamepad.sThumbRY;        /// right stick Y
    _gp->lt=  _lt;                        /// left trigger
    _gp->rt=  _rt;                        /// right trigger
    _gp->pov= tpov;                       /// pov (0- 359 degrees * 100, -1 neutral)

    // ---=== GAMEWHEEL axis ===---
    _gw->wheel= xi.Gamepad.sThumbLX;      /// wheel position
    _gw->a1=    xi.Gamepad.sThumbLY;      /// other axis 1
    _gw->a2=    xi.Gamepad.sThumbRX;      /// other axis 2
    _gw->a3=    xi.Gamepad.sThumbRY;      /// other axis 3
    _gw->a4=    _lt;                      /// other axis 4
    _gw->a5=    _rt;                      /// other axis 5

    // ---=== stick/pad/wheel BUTTONS ===---

    /// swapbuffers: lastCheck becomes previous button state
    _swapBuffers();
    _gp->_swapBuffers();
    _gw->_swapBuffers();

    /// XInput button order is all 'ver the place...
    _gp->but[0]= _gw->but[0]= but[0]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_X)>> 14; // button unification change
    _gp->but[1]= _gw->but[1]= but[1]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_A)>> 12; // button unification change
    _gp->but[2]= _gw->but[2]= but[2]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_B)>> 13; // button unification change
    _gp->but[3]= _gw->but[3]= but[3]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_Y)>> 15; // button unification change

    _gp->but[4]= _gw->but[4]= but[4]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_LEFT_SHOULDER)>> 8;
    _gp->but[5]= _gw->but[5]= but[5]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_RIGHT_SHOULDER)>> 9;
    _gp->but[6]= _gw->but[6]= but[6]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_BACK)>> 5;
    _gp->but[7]= _gw->but[7]= but[7]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_START)>> 4;
    _gp->but[8]= _gw->but[8]= but[8]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_LEFT_THUMB)>> 6;
    _gp->but[9]= _gw->but[9]= but[9]= (xi.Gamepad.wButtons& XINPUT_GAMEPAD_RIGHT_THUMB)>> 7;

    /// check if it is a new press/ release & do button history
    for(short a= 0; a< 10; a++) {              /// for each possible xpad button (i got 1 extra, but heck if i know how to check it)
      if(butPrev[a]&& !but[a]) {                // button just got ---=== PRESSED ===---
        /// mark button press time
        _gp->butTime[a]= _gw->butTime[a]= butTime[a]= presentMilli;
        /// log the button in history
        blog.but= (uint8)a;
        blog.checked= false;
        blog.timeDown= presentMilli;
        blog.timeUp= 0;
        blog.timeDT= 0;
        _log(blog);
        _gw->_log(blog);
        _gp->_log(blog);

        #ifdef OSI_BE_CHATTY
        if(chatty) printf("XInput: hid[%s] button PRESS nr[%d]\n", name.d, a);
        #endif

      } else if(but[a]&& !butPrev[a]) {         // button just got ---=== RELEASED ===---
        /// search thru history for the button, to mark the time it got released
        found= false;
        for(short b= 0; b< MAX_KEYS_LOGGED; b++) 
          if(butLog[b].but== a) {
            if(butLog[b].timeUp) continue;    /// skip if button is already released

            butLog[b].timeUp= presentMilli;
            butLog[b].timeDT= presentMilli- butLog[b].timeDown;
            butLog[b].checked= false;

            _gp->butLog[b].timeUp= presentMilli;
            _gp->butLog[b].timeDT= butLog[b].timeDT;
            _gp->butLog[b].checked= false;
  
            _gw->butLog[b].timeUp= presentMilli;
            _gw->butLog[b].timeDT= butLog[b].timeDT;
            _gw->butLog[b].checked= false;

            found= true;
            break;
          }

        // THIS FAILSAFE CODE COULD GO AWAY vvvvvvvvvvvvvvvvv
        if(!found) {                      /// failsafe - normally it is found (but things can happen ... alt-tab?)
          // some debug stuff can be done here, tho
          blog.but= (uint8)a;
          blog.checked= false;
          blog.timeDown= presentMilli- 1; /// mark it as insta down-up
          blog.timeUp= presentMilli;
          blog.timeDT= 1;
          _log(blog);                      /// put it in history buffer
          _gw->_log(blog);
          _gp->_log(blog);
        } /// failsafe

        #ifdef OSI_BE_CHATTY
        if(chatty) printf("XInput: hid[%s] button RELEASE nr[%d]\n", name.d, a);
        #endif

      } /// if press or depress
    } /// for each possible xpad button
  } /// pass thru all modes
  #endif /// USING_XINPUT
  #endif /// OS_WIN
}


void osiJoystick::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    _buffer1[a]= _buffer2[a]= 0;
    butPressure[a]= 0;
    butTime[a]= 0;
    #ifdef OS_MAC
    _cbTame.butPressure[a]= _cbTame.but[a]= 0;
    #endif
  }

  /// reset logged buttons
  for(short a= 0; a< MAX_KEYS_LOGGED; a++) {
    butLog[a].checked= true;                       /// mark all history as checked, so buttons get ignored
    if(butLog[a].timeDown&& !butLog[a].timeUp) {  /// button waiting dor depress
      butLog[a].timeUp= osi.present/ 1000000;
      butLog[a].timeDT= butLog[a].timeUp- butLog[a].timeDown;
    }
  }
}

void osiJoystick::resetAxis() {
  x= y= x2= y2= throttle= rudder= u= v= 0;
  pov= -1;
  #ifdef OS_MAC
  _cbTame.x= _cbTame.y= _cbTame.x2= _cbTame.y2= _cbTame.throttle= _cbTame.rudder= _cbTame.u= _cbTame.v= 0;
  _cbTame.pov= -1;
  #endif
}


bool osiJoystick::activate() {
  if(!mode) { _bActive= false; return false; }
  _bActive= true;
  if(_bGrabbed)
    return grab();
  return true;
}


bool osiJoystick::deactivate() {
  if(!mode) return false;
  _bActive= false;
  if(_bGrabbed)
    return ungrab();
  return true;
}


// grab exclusive control of the joystick (if possible)
bool osiJoystick::grab() {
  if(!mode) return false;
  _bGrabbed= true;
  
  /// per mode grabbing funcs - if there are any
  #ifdef OS_WIN

  #ifdef USING_DIRECTINPUT
  if(mode== 2)
    if(_diDevice) _diDevice->Acquire();
  #endif /// USING_DIRECTINPUT

  #ifdef USING_XINPUT
  if(mode== 3)
    XInputEnable(TRUE);
  #endif /// USING_XINPUT

  #endif /// OS_WIN
  return true;
}

// ungrab exclusive control of the joystick
bool osiJoystick::ungrab() {
  _bGrabbed= false;
  if(!mode) return false;
  
  
  /// per mode ungrab funcs - if there are any
  #ifdef OS_WIN

  #ifdef USING_DIRECTINPUT
  if(mode== 2)
    if(_diDevice) _diDevice->Unacquire();
  #endif /// USING_DIRECTINPUT

  #ifdef USING_XINPUT
  if(mode== 3)
    XInputEnable(FALSE);
  #endif /// USING_XINPUT

  #endif /// OS_WIN
  return true;
}


/// swap button buffers
void osiJoystick::_swapBuffers() {
  butPrev= but;
  but= (but== _buffer1)? _buffer2: _buffer1;
}


void osiJoystick::_log(const osiButLog &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    butLog[a]= butLog[a- 1];
  butLog[0]= k;
}







// ------------============= GAMEPAD CLASS ===========--------------------
// ========================================================================

osiGamePad::osiGamePad() {
  _j= null;
  delData();
}

osiGamePad::~osiGamePad() {
}

void osiGamePad::delData() {
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

  but= _buffer1;
  butPrev= _buffer2;

  resetButtons();
}


// clears all button buffers / resets logged buttons - called by in.resetPressedButtons() - when alt-tab / something similar
void osiGamePad::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    _buffer1[a]= _buffer2[a]= 0;
    butPressure[a]= 0;
    butTime[a]= 0;
  }

  /// reset logged buttons
  for(short a= 0; a< MAX_KEYS_LOGGED; a++) {
    butLog[a].checked= true;                       /// mark all history as checked, so buttons get ignored
    if(butLog[a].timeDown&& !butLog[a].timeUp) {  /// button waiting dor depress
      butLog[a].timeUp= osi.present/ 1000000;
      butLog[a].timeDT= butLog[a].timeUp- butLog[a].timeDown;
    }
  }
}


void osiGamePad::resetAxis() {
  lx= ly= rx= ry= lt= rt= u= v= 0;
  pov= -1;
}


void osiGamePad::_swapBuffers() {
  butPrev= but;
  but= (but== _buffer1)? _buffer2: _buffer1;
}


void osiGamePad::_log(const osiButLog &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    butLog[a]= butLog[a- 1];
  butLog[0]= k;
}



void checkGamepadType(osiGamePad *p) {
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

osiGameWheel::osiGameWheel() {
  _j= null;
  delData();
}

osiGameWheel::~osiGameWheel() {
}

void osiGameWheel::delData() {
  mode= 0;
  name.delData();
  if(_j) _j->deactivate();

  /// clear axis
  wheel= 0;
  a1= a2= a3= a4= a5= 0;      // THIS NEEDS MORE WORK
  // pov starts on -1, off state
  
  but= _buffer1;
  butPrev= _buffer2;

  resetButtons();
}


// clears button buffer states / resets logged buttons history
void osiGameWheel::resetButtons() {
  /// clear all buffers
  for(short a= 0; a< MAX_JOYSTICK_BUTTONS; a++) {
    _buffer1[a]= _buffer2[a]= 0;
    butPressure[a]= 0;
    butTime[a]= 0;
  }

  /// reset logged buttons
  for(short a= 0; a< MAX_KEYS_LOGGED; a++) {
    butLog[a].checked= true;                       /// mark all history as checked, so buttons get ignored
    if(butLog[a].timeDown&& !butLog[a].timeUp) {  /// button waiting dor depress
      butLog[a].timeUp= osi.present/ 1000000;
      butLog[a].timeDT= butLog[a].timeUp- butLog[a].timeDown;
    }
  }
}


void osiGameWheel::resetAxis() {
  wheel= a1= a2= a3= a4= a5= 0;      // THIS NEEDS MORE WORK
}


void osiGameWheel::_swapBuffers() {
  butPrev= but;
  but= (but== _buffer1)? _buffer2: _buffer1;
}


void osiGameWheel::_log(const osiButLog &k) {
  for(short a= MAX_KEYS_LOGGED- 1; a> 0; a--)
    butLog[a]= butLog[a- 1];
  butLog[0]= k;
}







// MAC specific fuctions. 

#ifdef OS_MAC 

///----------------///
// HIDDriver STRUCT //
///----------------///

/// part of HIDDriver structure. each 'element' can be a button or axis in a stick/pad/wheel
struct _HIDElement {
  // SUBJECT OF DELETION  
  int8 type;                    /// 1= axis, 2= button THIS MIGHT GO AWAY/ usagePage IS A BETTER 'TYPE'
  
  int16 id;                     /// button or axis number (0 - max buttons / 0 - max axis)
  int32 usage, usagePage;        /// [most inportant] characteristics about this element (is it an x axis, is it a button etc)
  int32 logicalMin;              /// minimum value it can have
  int32 logicalMax;              /// maximum value it can have
  bool hasNULLstate;

  bool isHat;                   /// is it a hat switch / dPad
  bool hatMultiAxis;            /// if the hat has 2 axis (there are some sticks that have complex hats)
  bool hatAxis1;                /// [hat multi axis only] if it is the first axis (x)
  bool hatAxis2;                /// [hat multi axis only] if it is the second axis (y)
  _HIDElement(): type(0), id(0), logicalMin(0), logicalMax(0), hasNULLstate(false), isHat(false), hatMultiAxis(false), hatAxis1(false), hatAxis2(false) {}
};

/// macs lack a proper joystick api; the next struct is a helper to 'decode' the mess that is almost raw reading from a HID device
struct _HIDDriver {              /// [internal]
  bool inUse;                   /// is this in use?
  IOHIDDeviceRef device;        /// coresponding 'device' that this stick/pad/wheel is tied to
  int16 nrButtons;              /// number of buttons that this stick/pad/wheel has
  int16 nrAxis;                 /// number of axis this stick/pad/wheel has
  _HIDElement *elem;             /// array of elements the device has
  
  /// standard constructor/destructor/delData(); everything will be set to 0 and memory will be auto-deallocated if allocated
  _HIDDriver(): inUse(false), device(null), nrButtons(0), nrAxis(0), elem(0) {}
  void delData() { if(elem) delete[] elem; elem= null; nrButtons= nrAxis= 0; inUse= false; device= null; }
  ~_HIDDriver() { delData(); }
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
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  #endif

  // IOHIDDeviceRegisterInputValueCallback(device, HIDchange, &driver[a]); this could further optimize some code, but very little, by passing &driver[a] after it is created...
  #ifdef OSI_BE_CHATTY
  if(chatty) printf("%s\n", __FUNCTION__);
  #endif

  /// find the first non-in-use joystick
  int16 a;
  for(a= 0; a< MAX_JOYSTICKS; a++)
    if(!driver[a].inUse)
      break;

  /// lock cbtame struct - thread safety
  in.j[a]._cbTame.mutex.lock();


  // start to 'populate' vars inside the helper struct

  
  /// stick/pad/wheel name (product name) ... this should be easy, right?...
  CFStringRef name;
  name= (CFStringRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));

  if(name && (CFGetTypeID(name)== CFStringGetTypeID())) {
    CFIndex length;
		
		CFStringGetBytes(name, CFRangeMake(0, CFStringGetLength(name)), kCFStringEncodingUTF8, '?', false, NULL, 100, &length);
    
    in.j[a].name.d= (char *)new uint8[length+ 1];
                             
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
  
  driver[a].elem= new _HIDElement[CFArrayGetCount(elems)];
  
  /// populate driver struct
  int16 c= 0, d= 0; /// c will hold counter for axis, d for buttons
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
    #ifdef OSI_BE_CHATTY
    if(chatty) printf("element[%d]: cookie[%d] id[%d] type[%d] min[%ld] max[%ld] hasNULL[%d] isHat[%d]\n", b, cookie, driver[a].elem[b].id, driver[a].elem[b].type, (long)driver[a].elem[b].logicalMin, (long)driver[a].elem[b].logicalMax, driver[a].elem[b].hasNULLstate, driver[a].elem[b].isHat);
    #endif

	} /// for each element
  
  in.j[a].maxButtons= driver[a].nrButtons;
  in.gp[a].maxButtons= driver[a].nrButtons;
  in.gw[a].maxButtons= driver[a].nrButtons;

	CFRelease(elems);     /// release elements array

  #ifdef OSI_BE_CHATTY
	if(chatty) printf("device[%s] nrButtons[%d] nrAxis[%d]\n", in.j[a].name.d, driver[a].nrButtons, driver[a].nrAxis);
  #endif
  
  // THESE VARS NEED TO BE THE LAST TO BE WRITTEN TO. AND THE ORDER IS VERY IMPORTANT TO AVOID BUGS (mode = 1 before nrFounds)
  //  theory case: the callbug aaa callback func starts to write data to the joystick
  //               the application that uses osi is checking all joysticks - finds a mode 1 that is just being writtin to, but not all data is there
  //               ... bug asures (a write to data and a read from data happen in the same period of time)

  /// rest of vars
  driver[a].inUse= true;  
  driver[a].device= device;
  in.j[a].mode= 1;              /// set it's mode to 1. in macs i think only mode 1 will be avaible... there is no freaking support for HIDs
  in.gp[a].mode= 1;
  in.gw[a].mode= 1;

  /// sticks/pads/wheels 'numbers'
  in.nr.jFound++;  in.nr.jOS++;
  in.nr.gpFound++; in.nr.gpOS++;
  in.nr.gwFound++; in.nr.gwOS++;

  /// release grab of objects
  in.j[a]._cbTame.mutex.unlock();
} /// HIDadded




// CALLBACK FUNCTION: when a device is removed ------------------------------------
///================================================================================
static void HIDremoved(void *context, IOReturn result, void *sender, IOHIDDeviceRef device) {
  // inContext:   context from IOHIDManagerRegisterDeviceMatchingCallback
  // inResult:    the result of the removing operation
  // inSender:    the IOHIDManagerRef for the device being removed
  // inHIDDevice: the removed HID device

  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  if(chatty) printf("%s", __FUNCTION__);
  #endif

  /// find removed device in 'driver' struct
  int16 a;
  for(a= 0; a< MAX_JOYSTICKS; a++)
    if(driver[a].device== device)
      break;

  if(a== MAX_JOYSTICKS) {     /// if not found... well... something is wrong...
    error.simple("HIDremoved: can't find the requested device");
    return;
  }

  /// thread safety
  in.j[a]._cbTame.mutex.lock();

  // THE NEXT 4 CODE LINES ORDER IS IMPORTANT - IF A THREAD WRITES TO DATA, AND ANOTHER READS... (love them callbacks, right?)
  // so decrease the numbers, then set them to mode 0 (disabled)

  /// sticks/pads/wheels 'numbers'
  in.nr.gwFound--; in.nr.gwOS--;
  in.nr.gpFound--; in.nr.gpOS--;
  in.nr.jFound--;  in.nr.jOS--;

  in.j[a].mode= in.gp[a].mode= in.gw[a].mode= 0; /// mode 0 = DISABLED

  in.j[a].name.delData();
  in.gp[a].name.delData();
  in.gw[a].name.delData();
  _HIDdisconnected= true;
  
  driver[a].delData();

  #ifdef OSI_BE_CHATTY
  if(chatty) printf(" helper cleared\n");
  #endif

  //in.j[a]._cbTame._accessCB_HIDplug= false;

  /// release objects lock
  in.j[a]._cbTame.mutex.unlock();
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

  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  #endif
  

  IOHIDElementRef elem= IOHIDValueGetElement(val);        /// get the 'element' thet of the value
  IOHIDDeviceRef device= IOHIDElementGetDevice(elem);     /// get the HID device that a element belongs to
  uint32 cookie;  
  
  #ifdef __LP64__ // they did the cookie as int32 for 64bit MacOS, and void * for 32... INCREDIBLE!!!
  cookie= (uint32)IOHIDElementGetCookie(elem)- 1; /// cookies represent a unique identifier for a HID element (also first element they point to is 1, and the list starts with 0)
  #else
  IOHIDElementCookie _cook= IOHIDElementGetCookie(elem); /// cookies represent a unique identifier for a HID element (also first element they point to is 1, and the list starts with 0)
  cookie= *((uint8 *)_cook);
  cookie--;
  #endif
  

  /// find the stick/pad/wheel this change belongs to
  int16 a;
  for(a= 0; a< in.nr.jOS; a++)
    if(driver[a].device== device)   /// found it
      break;


  /// thread safety between osi-callbackFuncs
  in.j[a]._cbTame.mutex.lock();

  _HIDElement *e= &driver[a].elem[cookie]; /// name shortcut
  
  if(!e->type) { in.j[a]._cbTame.mutex.unlock(); return; }/// only type1(axis) and type2(butons) are handled. rest, return quick
    
  /// value translation to -32767 : +32767
  int32 amin= e->logicalMin< 0? -e->logicalMin: e->logicalMin; /// faster than calling abs()
  int32 amax= e->logicalMax< 0? -e->logicalMax: e->logicalMax; /// faster than calling abs()

  if((e->type== 1) && amin+ amax == 0)    /// quick way to eliminate trash (dunno what other 'axis' that mac reports are) / would be div by zero error too
    { in.j[a]._cbTame.mutex.unlock(); return; }
  
  int32 v= IOHIDValueGetIntegerValue(val); /// the actual value that changed
  //  double v2= IOHIDValueGetScaledValue(val, kIOHIDValueScaleTypeCalibrated); /// it could be used
  //  double v3= IOHIDValueGetScaledValue(val, kIOHIDValueScaleTypePhysical);   /// i saw there are not big changes from v

  /// Returns the timestamp value associated with this HID value reference.
  /// !!! these timestamps are using mach too, so they are reliable !!!
  uint64_t time= IOHIDValueGetTimeStamp(val)/ 1000000;    /// convert to millisecs too (from nanosecs)
  
  #ifdef OSI_BE_CHATTY
  if(chatty)  printf("HID callback[%s] ", in.j[a].name.d);
  #endif

  if(e->usagePage== kHIDPage_GenericDesktop) {            // ---=== axis ===---
    #ifdef OSI_BE_CHATTY
    if(chatty) printf(" axis change\n");
    #endif

    int32 t= (((v+ amin)* 65534)/ (amin+ amax))- 32767;   /// value scaled to min[-32767] max[+32767], 65535 total possible units (65534+ unit 0)
    if(t> -150&& t< 150) t= 0;                           /// this is due to bug in mac HID api. center position is not centered.
    
    // THROTTLE / RUDDLE could have a null position
    
    switch(e->usage) {
      case kHIDUsage_GD_X:  in.j[a]._cbTame.x= t;        in.j[a]._cbTame.mutex.unlock(); return; // [X axis]
      case kHIDUsage_GD_Y:  in.j[a]._cbTame.y= t;        in.j[a]._cbTame.mutex.unlock(); return; // [Y axis]
      case kHIDUsage_GD_Z:  in.j[a]._cbTame.throttle= t; in.j[a]._cbTame.mutex.unlock(); return; // [Throttle]
      case kHIDUsage_GD_Rx: in.j[a]._cbTame.v= t;        in.j[a]._cbTame.mutex.unlock(); return; // [extra1 X]
      case kHIDUsage_GD_Ry: in.j[a]._cbTame.u= t;        in.j[a]._cbTame.mutex.unlock(); return; // [extra1 Y]
      case kHIDUsage_GD_Rz: in.j[a]._cbTame.rudder= t;   in.j[a]._cbTame.mutex.unlock(); return; // [Rudder]

      case kHIDUsage_GD_Hatswitch:  // POV - here comes the pain
        if(!e->hatMultiAxis) {                   // gamePad dPads have only 1 axis
          if(v> e->logicalMax|| v< e->logicalMin) {
            in.j[a]._cbTame.pov= -1;
            // game wheels can have povs, i guess, dunno
          } else {
            in.j[a]._cbTame.pov= (360/ (e->logicalMax+ 1))* v; /// in degrees
          }
        } else {                                 // multi-axis hat switch. CAN'T TEST THESE AS I DO NOT HAVE ONE
          //if(chatty) printf(" multi-axis hat switch not handled ATM... gotta buy one first!\n");
          // THIS CODE IS NOT TESTED, as i have no joystick with 2 axis hat switch
          if(e->hasNULLstate)
            if((v< e->logicalMin) && (v> e->logicalMax)) {
              in.j[a]._cbTame.pov= -1;
              in.j[a]._cbTame.mutex.unlock();
              return;
            }
            
          int32 x, y;                        /// they gotta be ints for exact 0 degrees or 90 degrees, else there are problems
          double pov;
          
          /// get axis from current pov position (wich is in degrees)
          pov= in.j[a]._cbTame.pov;
          x= y= 0;
          
          if(in.j[a].pov!= -1) {            /// ... only if it's not on -1 position (nothing selected)
            x= (double)(32767.0* sin(in.j[a].pov* (M_PI/ 180.0)));
            y= (double)(32767.0* cos(in.j[a].pov* (M_PI/ 180.0)));
          }
          
          /// update from changed value (v)
          int32 t= (((v+ amin)* 65534)/ (amin+ amax))- 32767;
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
          in.j[a]._cbTame.pov= pov;
        } /// if multi-axis hat switch / pov / dPad / watever other name they come up with
        in.j[a]._cbTame.mutex.unlock();
        return;
      default:
        #ifdef OSI_BE_CHATTY
        if(chatty) printf(" unhandled generic desktop element usage[%x]\n", e->usage);
        #endif
        ;
    } /// axis usage switch

  } else if(e->type== 2) {                                // ---=== button ===---

    #ifdef OSI_BE_CHATTY
    if(chatty) printf(" button state/pressure change\n");
    #endif

    /// ^^^ have to use the 'type' variable, wich is the only place it actually works... 

    in.j[a]._cbTame.but[e->id]= (uint8)v;

  } else if(e->usagePage== kHIDPage_VendorDefinedStart) { // ---=== button pressure / other vendor specifics ===---
    int32 t;
    if(e->usage>= 0x20 && e->usage<= 0x2B)/// button pressure range
      t= (((v+ amin)* 65534)/ (amin+ amax))- 32767;     /// value scaled to min[-32767] max[+32767], 65535 total possible units (65534+ unit 0)    

    // ---=== button pressures ===---
    switch(e->usage) {
      case 0x20: in.j[a]._cbTame.butPressure[20]= t+ 32767; in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button pov right
      case 0x21: in.j[a]._cbTame.butPressure[21]= t+ 32767; in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button pov left
      case 0x22: in.j[a]._cbTame.butPressure[22]= t+ 32767; in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button pov up
      case 0x23: in.j[a]._cbTame.butPressure[23]= t+ 32767; in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button pov down
      case 0x24: in.j[a]._cbTame.butPressure[3]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button 3
      case 0x25: in.j[a]._cbTame.butPressure[2]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button 2
      case 0x26: in.j[a]._cbTame.butPressure[1]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button 1
      case 0x27: in.j[a]._cbTame.butPressure[0]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button 0
      case 0x28: in.j[a]._cbTame.butPressure[4]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button 4
      case 0x29: in.j[a]._cbTame.butPressure[5]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button 5
      case 0x2A: in.j[a]._cbTame.butPressure[6]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button EXTRA 1
      case 0x2B: in.j[a]._cbTame.butPressure[7]= t+ 32767;  in.j[a]._cbTame.mutex.unlock(); return; /// gamepad button EXTRA 2
      default:
        if(chatty) printf(" unhandled vendor specific element usage[%x]\n", e->usage);
        ;
    }
    
  } else if(e->usagePage== kHIDPage_Button) {             // ---=== ofc, this page doesn't work for game HIDs ===---
    /// this page is NOT USED (for gamepads at least, so i guess it is not used for any game HID)
    #ifdef OSI_BE_CHATTY
    if(chatty) printf(" unhandled usagePage == button\n");
    #endif

  }
  #ifdef OSI_BE_CHATTY
  else if(chatty) printf(" unhandled HID element: usagePage[%x] usage[%x]\n", e->usagePage, e->usage);
  #endif
  

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
  in.j[a]._cbTame.mutex.unlock();
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







