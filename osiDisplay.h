#pragma once

// check osinteraction.h for more linux / other os info & how to initialize & use everything

struct osiResolution; // SELFNOTE: decl might work here cuz they are defined in same cpp
struct osiMonitor;
struct osiGPU;
class osinteraction;
class osiWindow;
class osiRenderer;



// ---------------==============DISPLAY CLASS=================--------------- //
///--------------------------------------------------------------------------///
class osiDisplay {
  friend class osinteraction;

public:

  // call this at PROGRAM START
  void populate(osinteraction *t);       // call this once to populate everything
  
  int16 nrMonitors;                     /// nr of active monitors connected to the system
  int32 vx0, vy0, vdx, vdy;             /// VIRTUAL DESKTOP size (all monitors are placed inside this virtual desktop/ fullscreen virtual desktop mode, uses these)

  int16 nrGPUs;                         /// nr of GPU's on the current machine - IF THIS IS 0, THERE WAS NO WAY TO AQUIRE THIS DATA
  osiGPU *GPU;                          /// array with all GPU's on the current machine

  osiMonitor *monitor;                  /// all monitors database
  osiMonitor *primary;                  /// pointer to the primary monitor


  //MULTI MONITOR HANDLERS

  bool changeRes(osiMonitor *m, int32 dx, int32 dy, int8 bpp, int16 freq= 0); // change specific monitor resolution (this is actually the main resolution change func)
  void restoreRes(osiMonitor *m);       /// restores original resolution of a specific monitor
  void restoreAllRes();                 /// restores all original resolutions
  
  //primary monitor; use these for sigle monitor resolution change

  bool changePrimary(int32 dx, int32 dy, int8 bpp, int16 freq= 0);      /// change primary display& primary monitor resolution (calls changeRes, nothing more)
  void restorePrimary();

  bool isRendererInUse(uint glRender);  /// WIP check if the renderer is in use on other windows (if not in use anymore, it can be destroyed)
 
  osiDisplay();
  ~osiDisplay();
  void delData();                       // called by destroyer -standard

  #ifdef OS_LINUX
  int _top, _bottom, _left, _right;     /// [internal] used for _NET_WM_FULLSCREEN_MONITORS. check populate(), end of linux part
  #endif /// OS_LINUX
};


// --------------======== osiResolution =======-----------------------
struct osiResolution {
  int32 dx, dy;               /// resolution size
  int32 nrFreq;               /// nr of frequencies supported
  int16 *freq;                /// list of supported frequencies in this resolution
  /// bpp is ignored ATM, as 32bpp is default, and i don't think anything else will be used
  
  osiResolution();            /// set everything to 0 (needed)
  ~osiResolution();           /// calls delData()
  void delData();             /// clears data/ deallocs everything

  // private stuff from here 

  #ifdef OS_LINUX
  Rotation _rotation;         /// [internal] X if it is used...
  RRMode *_resID;             /// [internal] it is tied with frequency (RRMode[nrFreq])
  #endif /// OS_LINUX

  #ifdef OS_MAC
  uint32 *_id;                /// [internal] resolution id (mac) - tied with frequency (id[nrFreq])
  //possible that a CGDisplayModeRef must be used 
  #endif /// OS_MAC
  
};


// -------------========= osiMonitor =======------------------
struct osiMonitor {

  Str8 name;                /// monitor name (product description or something that can identify it)

  int32 x0, y0;             /// position on the VIRTUAL DESKTOP
  int32 dx, dy;             /// current size (resolution size)
  
  bool primary;             /// is it the primary display 
  osiGPU *GPU;              /// on what GPU is attached
  osiWindow *win;           /// the window that is on this monitor (if there is one)
  

  int16 nrRes;              /// nr of resolutions monitor can handle
  osiResolution *res;       /// all resolutions the display supports (res[nrRes])
  
  // the next vars are kinda internal stuff

  bool inOriginal;          /// monitor is in original resolution (false= in program resolution)
  osiResolution original;   /// original resolution @ program start (freq[0] is ID, not hertz)
  osiResolution progRes;    /// program resolution. original&program used to detect a resolution CHANGE, and ignore multiple resolution changes if already in requested resolution (freq[0] is ID, not hertz)
  osiRenderer *glr;

  #ifdef OS_LINUX
  int _XineramaID;          /// [internal] used only for _NET_WM_FULLSCREEN_MONITORS, it is found in display.populate() @ end of linux part
  #endif /// OS_LINUX
    
  osiMonitor();
  ~osiMonitor();
  void delData();

  // nothing to bother from here on

  #ifdef OS_WIN
  Str8 _id;                 /// [internal] win- display ID
  //Str8 name;                /// [internal] display's card name
  Str8 _monitorID;          /// [internal] monitor id (NOT USED FOR ANYTHING?... wincrap rulz)
  Str8 _monitorName;        /// [internal] monitor description (did not find any use for it ina ANY windows function)
  // if a monitor is set to duplicate another monitor, windows returns only one display,
  // with combined resolution options, and monitorID+monitorName for each. Can't do anything with any of them, so im not storing them anywhere.
  friend LRESULT CALLBACK _processMSG(HWND hWnd, UINT m, WPARAM wParam, LPARAM lParam);
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  int _screen;              /// [internal] monitor id (number)
  Window _root;             /// [internal] root window of screen (monitor)
  RROutput _outID;          /// [internal] xrandr output (phisical out that a monitor can be attached to; this output holds connected monitor info/supported modes too)
  RRCrtc _crtcID;           /// [internal] xrandr crtc (some internal graphics card thingie that handles pixels sent to outputs->monitors)
  osiMonitor *_right;       /// [internal] points to a monitor next to this one, to the right, or NULL
  osiMonitor *_bottom;      /// [internal] points to a monitor next to this one, to the bottom, or NULL
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  uint _id;                 /// [internal] quartz monitor id
  #endif /// OS_MAC
private:
  friend class osiDisplay;
  friend void getMonitorPos(osiMonitor *m);
  friend void updateVirtualDesktop();
  friend bool doChange(osiMonitor *, osiResolution *, int8, int16);

  friend void _populateGrCards(osiDisplay *);

  int32 _y0;                /// not changed, os specific, monitor position on the y axis
};



struct osiGPU {
  Str8 name;
  int32 ram;
  int32 clock;

  bool primary;

  int16 nrMonitors;         /// nr monitors attached
  osiMonitor **monitor;     /// array with all attached monitors; [nrMonitors] in size

  osiGPU() { ram= clock= 0; nrMonitors= 0; monitor= null; primary= false; }
  void delData() { if(nrMonitors) { delete[] monitor; nrMonitors= 0; monitor= null; } primary= false; name.delData(); ram= clock= 0; }
  ~osiGPU() { delData(); }
};
