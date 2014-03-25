#pragma once

/// the main defs that are used:
// OS_WIN
// OS_LINUX
// OS_MAC

// check OSInteraction.h for more linux / other os info & how to initialize & use everything

//#define MAX_WINDOWS 128



struct OSIResolution; // SELFNOTE: decl might work here cuz they are defined in same cpp
struct OSIMonitor;
class OSInteraction;
class OSIWindow;


// WIP (as nothing is used in this atm)
class Renderer: public chainData {
public:
  Renderer(); ~Renderer(); void delData();  // constructor, destructor
  #ifdef OS_WIN
  HGLRC glRenderer;     // ze thing needed - destructor deletes oGL render context if it is created
  #endif /// OS_WIN
  #ifdef OS_LINUX
  GLXContext glRenderer; // WIP
  #endif /// OS_LINUX
  #ifdef OS_MAC
  void *glRenderer;
  #endif
  bool isActive; // a simple check to see if it is already active when calling glMakeCurrent() is WAAAY faster then switching contexts... 
};
// END WIP


// ---------------==============DISPLAY CLASS=================--------------- //
///--------------------------------------------------------------------------///
class OSIDisplay {
public:
// call this at program start
  void populate(OSInteraction *t);       // call this once to populate everything

  void getMonitorPos(OSIMonitor *m);    /// sets m->x0 & m->y0, the monitor position on VIRTUAL DESKTOP (call this after a resolution change
  
  //void updateMonitorsPos();
  
  
  
  short nrMonitors;                     /// nr of active monitors connected to the system
  short nrCards;                        /// WIP (nothing atm)
  short vx0, vy0, vdx, vdy;             /// VIRTUAL DESKTOP size (all monitors are placed inside this virtual desktop/ fullscreen virtual desktop mode, uses these)

  OSIMonitor *monitor;                  /// all monitors database
  OSIMonitor *primary;                  /// pointer to the primary monitor

  chainList renderer;   // WIP (nothing atm)
  Renderer rData;       // WIP (nothing atm)

//primary monitor/gr card; use these for sigle monitor resolution change
  bool changePrimary(short dx, short dy, int8 bpp, short freq= 0);      /// change primary display& primary monitor resolution
  void restorePrimary();

//MULTI MONITOR HANDLERS
  bool changeRes(OSIWindow *w, OSIMonitor *m, short dx, short dy, int8 bpp, short freq= 0); // change specific monitor resolution (this is actually the main resolution change func)

  void restoreRes(OSIWindow *w, OSIMonitor *m);   /// restores original resolution of a specific monitor
  void restoreAllRes();                           /// restores all original resolutions

  bool isRendererInUse(uint glRender);            /// check if the renderer is in use on other windows (if not in use anymore, it can be destroyed)
  
/// mainly private stuff from here
  OSIResolution *getResolution(int dx, int dy, OSIMonitor *m);  /// searches *res[] data for requested resolution
  short getFreq(short freq, OSIResolution *r);    /// searches *res[] data for requested frequency
  
  #ifdef OS_LINUX
  //XRRScreenResources *scr;
  XRRModeInfo *getMode(XRRScreenResources *s, RRMode id); /// [internal] 
  #endif /// OS_LINUX

  OSIDisplay();
  ~OSIDisplay();
  void delData();             // called by destroyer -standard
};


// --------------======== OSIResolution =======-----------------------
struct OSIResolution {
  OSIResolution();            /// set everything to 0 is needed
  ~OSIResolution();           /// NOT USED ATM, SUBJECT OF DELETION
  void delData();             /// NOT USED ATM, SUBJECT OF DELETION
  
  #ifdef OS_LINUX
  Rotation rotation;          /// [internal] X if it is used...
  RRMode *resID;              /// [internal] it is tied with frequency (RRMode[nrFreq])
  #endif /// OS_LINUX

  #ifdef OS_MAC
  uint *id;                   /// [internal] resolution id (mac) - tied with frequency (id[nrFreq])
//possible that a CGDisplayModeRef must be used 
  #endif /// OS_MAC
  
  int dx, dy;                 /// resolution size
  int nrFreq;                 /// nr of frequencies supported
  short *freq;                /// list of supported frequencies in this resolution
};


// -------------========= OSIMonitor =======------------------
struct OSIMonitor {
  int x0, y0;               /// position on the VIRTUAL DESKTOP
  int dx, dy;               /// current size (resolution size)
  //string name/display there has to be a description/name, common for all OSes (same variable name, that can be used in the main program)
  string name;              // monitor name. this is a good candidate for the unified OS monitor name/description
  
  #ifdef OS_WIN
  string id;                /// [internal] win- display ID
  //string name;              /// [internal] display's card name
  string monitorID;         /// [internal] monitor id (NOT USED FOR ANYTHING?... wincrap rulz)
  string monitorName;       /// [internal] monitor description (did not find any use for it ina ANY windows function)
  // if a monitor is set to duplicate another monitor, windows returns only one display,
  // with combined resolution options, and monitorID+monitorName for each. Can't do anything with any of them, so im not storing them anywhere.
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  int screen;               /// [internal] monitor id (number)
  Window root;              /// [internal] root window of screen (monitor)
  RROutput outID;           /// [internal] xrandr output (phisical out that a monitor can be attached to; this output holds connected monitor info/supported modes too)
  RRCrtc crtcID;            /// [internal] xrandr crtc (some internal graphics card thingie that handles pixels sent to outputs->monitors)
  OSIMonitor *right;        /// [internal] points to a monitor next to this one, to the right, or NULL
  OSIMonitor *bottom;       /// [internal] points to a monitor next to this one, to the bottom, or NULL
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  uint id;                  /// [internal] quartz monitor id
  // name was here << name unification change
  #endif /// OS_MAC
  
  
// opengl renderer; only 1 per graphics card must be created,
// when glCreateContext is called. so that func must be expanded/clever
  bool rendererCreated;     /// renderer created flag.
  
  #ifdef OS_WIN
  HGLRC glRenderer;         /// oGL rendering context      <<<<<THIS NEEDS A PROPER CODE POSITION>>>>> here might be too obscured
  #endif /// OS_WIN

  #ifdef OS_LINUX
  GLXContext glRenderer;    /// oGL rendering context      <<<<<THIS NEEDS A PROPER CODE POSITION>>>>> here might be too obscured
  #endif /// OS_LINUX

  #ifdef OS_MAC
  void *glRenderer;
  #endif /// OS_MAC
// more thinking about the renderer ^^^
  
  
  bool primary;             /// is it the primary display
  short nrRes;              /// nr of resolutions monitor can handle
  OSIResolution *res;       /// all resolutions the display supports (res[nrRes])
  
  bool inOriginal;          /// monitor is in original resolution (false= in program resolution)
  OSIResolution original;   /// original resolution @ program start (freq[0] is ID, not hertz)
  OSIResolution progRes;    /// program resolution. original&program used to detect a resolution CHANGE, and ignore multiple resolution changes if already in requested resolution (freq[0] is ID, not hertz)
  
  OSIMonitor();
  ~OSIMonitor();
  void delData();
};




