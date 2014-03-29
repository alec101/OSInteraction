#pragma once

// check OSInteraction.h / pch.h / for more linux / other os info & how to initialize & use everything



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
  friend class OSInteraction;
public:
// call this at PROGRAM START
  void populate(OSInteraction *t);       // call this once to populate everything
  
  short nrMonitors;                     /// nr of active monitors connected to the system
  short nrCards;                        /// WIP (nothing atm)
  short vx0, vy0, vdx, vdy;             /// VIRTUAL DESKTOP size (all monitors are placed inside this virtual desktop/ fullscreen virtual desktop mode, uses these)

  OSIMonitor *monitor;                  /// all monitors database
  OSIMonitor *primary;                  /// pointer to the primary monitor

  chainList renderer;   // WIP (nothing atm)
  Renderer rData;       // WIP (nothing atm)


//MULTI MONITOR HANDLERS
  bool changeRes(OSIWindow *w, OSIMonitor *m, short dx, short dy, int8 bpp, short freq= 0); // change specific monitor resolution (this is actually the main resolution change func)
  void restoreRes(OSIWindow *w, OSIMonitor *m);   /// restores original resolution of a specific monitor
  void restoreAllRes();                           /// restores all original resolutions
  
  //primary monitor; use these for sigle monitor resolution change
  bool changePrimary(short dx, short dy, int8 bpp, short freq= 0);      /// change primary display& primary monitor resolution (calls changeRes, nothing more)
  void restorePrimary();

  bool isRendererInUse(uint glRender);  /// WIP check if the renderer is in use on other windows (if not in use anymore, it can be destroyed)
 
  OSIDisplay();
  ~OSIDisplay();
  void delData();                       // called by destroyer -standard

  #ifdef OS_LINUX
  int top, bottom, left, right;         /// [internal] used for _NET_WM_FULLSCREEN_MONITORS. check populate(), end of linux part
  #endif /// OS_LINUX
};


// --------------======== OSIResolution =======-----------------------
struct OSIResolution {
  int dx, dy;                 /// resolution size
  int nrFreq;                 /// nr of frequencies supported
  short *freq;                /// list of supported frequencies in this resolution
  /// bpp is ignored ATM, as 32bpp is default, and i don't think anything else will be used
  
  OSIResolution();            /// set everything to 0 (needed)
  ~OSIResolution();           /// calls delData()
  void delData();             /// clears data/ deallocs everything

// private data from here  
  #ifdef OS_LINUX
  Rotation rotation;          /// [internal] X if it is used...
  RRMode *resID;              /// [internal] it is tied with frequency (RRMode[nrFreq])
  #endif /// OS_LINUX

  #ifdef OS_MAC
  uint *id;                   /// [internal] resolution id (mac) - tied with frequency (id[nrFreq])
  //possible that a CGDisplayModeRef must be used 
  #endif /// OS_MAC
  
};


// -------------========= OSIMonitor =======------------------
struct OSIMonitor {
  string name;              /// monitor name (product description or something that can identify it)

  int x0, y0;               /// position on the VIRTUAL DESKTOP
  int dx, dy;               /// current size (resolution size)
  
  bool primary;             /// is it the primary display 
  
  short nrRes;              /// nr of resolutions monitor can handle
  OSIResolution *res;       /// all resolutions the display supports (res[nrRes])
  
  // the next vars are kinda internal stuff
  bool inOriginal;          /// monitor is in original resolution (false= in program resolution)
  OSIResolution original;   /// original resolution @ program start (freq[0] is ID, not hertz)
  OSIResolution progRes;    /// program resolution. original&program used to detect a resolution CHANGE, and ignore multiple resolution changes if already in requested resolution (freq[0] is ID, not hertz)

// WIP
// opengl renderer; only 1 per graphics card must be created,
// when glCreateContext is called. so that func must be expanded/clever
  bool rendererCreated;     /// WIP renderer created flag.
  #ifdef OS_WIN
  HGLRC glRenderer;         /// oGL rendering context      <<<<<THIS NEEDS A PROPER CODE POSITION>>>>> here might be too obscured
  #endif /// OS_WIN

  #ifdef OS_LINUX
  GLXContext glRenderer;    /// oGL rendering context      <<<<<THIS NEEDS A PROPER CODE POSITION>>>>> here might be too obscured
  int XineramaID;           /// [internal] used only for _NET_WM_FULLSCREEN_MONITORS, it is found in display.populate() @ end of linux part
  #endif /// OS_LINUX

  #ifdef OS_MAC
  void *glRenderer;
  #endif /// OS_MAC
// more thinking about the renderer ^^^

// /WIP
  
  OSIMonitor();
  ~OSIMonitor();
  void delData();

// nothing to bother from here on
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
  #endif /// OS_MAC
};




