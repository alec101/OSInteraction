#pragma once

// check osinteraction.h for more linux / other os info & how to initialize & use everything

struct osiResolution;
struct osiMonitor;
struct osiGPU;
class osinteraction;
class osiWindow;



// ---------------==============DISPLAY CLASS=================--------------- //
///--------------------------------------------------------------------------///
class osiDisplay {
public:
  void populate(bool onlyVulkan= false); // this is auto-called on osi constructor; can be called multiple times to rescan the displays. after vulkan is initialized, it should be called to further populate everything vulkan can find. if <onlyVulkan> is true, only the vulkan part will populate
  
  int16_t nrMonitors;                   /// nr of active monitors connected to the system
  int32_t vx0, vy0;                     /// virtual desktop position - win+linux top-left origin, mac bottom-right origin
  int32_t vdx, vdy;                     /// VIRTUAL DESKTOP size (all monitors are placed inside this virtual desktop/ fullscreen virtual desktop mode, uses these)
  int32_t vyMax;                        /// virtual desktop y-axis max point (vy0+ vdy- 1)- used in TOP-BOTTOM origin point compute (a shortcut)

  int16_t nrGPUs;                       /// nr of GPU's on the current machine - IF THIS IS 0, THERE WAS NO WAY TO AQUIRE THIS DATA
  osiGPU *GPU;                          /// array with all GPU's on the current machine

  osiMonitor *monitor;                  /// array with all monitors - array size is nrMonitors
  osiMonitor *primary;                  /// pointer to the primary monitor

  bool bResCanBeChanged;                /// monitor resolution change is possible
  str8 bResCanBeChangedReason;          /// if monitor resolution change is not possible (reason)
  bool bGPUinfoAvaible;                 /// GPU information avaible
  str8 bGPUinfoAvaibleReason;           /// if no GPU info is avaible, the reason for it

  //MULTI MONITOR HANDLERS

  bool changeRes(osiMonitor *m, int32_t dx, int32_t dy, int16_t freq= 0); // change specific monitor resolution (this is actually the main resolution change func)
  void restoreRes(osiMonitor *m);       /// restores original resolution of a specific monitor
  void restoreAllRes();                 /// restores all original resolutions
  
  //primary monitor; use these for sigle monitor resolution change

  bool changePrimary(int32_t dx, int32_t dy, int16_t freq= 0);      /// change primary display& primary monitor resolution (calls changeRes, nothing more)
  void restorePrimary();

  osiDisplay();
  ~osiDisplay();
  void delData();                       // called by destroyer -standard

  #ifdef OS_LINUX
  int _top, _bottom, _left, _right;     /// [internal] used for _NET_WM_FULLSCREEN_MONITORS. check populate(), end of linux part
  #endif /// OS_LINUX

private:
  void _vkPopulate();
  friend class osinteraction;
};


// --------------======== osiResolution =======-----------------------
struct osiResolution {
  int32_t dx, dy;             // resolution size
  int32_t nrFreq;             // nr of frequencies supported
  int16_t *freq;              // list of supported frequencies in this resolution
  /// bpp is ignored ATM, as 32bpp is default, and i don't think anything else will be ever used
  
  osiResolution();            // set everything to 0 (needed)
  ~osiResolution();           // calls delData()
  void delData();             // clears data/ deallocs everything
  
  // private stuff from here 

  #ifdef OS_LINUX
  Rotation _rotation;         // [internal] X if it is used...
  RRMode *_resID;             // [internal] it is tied with frequency (RRMode[nrFreq])
  #endif /// OS_LINUX

  #ifdef OS_MAC
  uint32_t *_id;              /// [internal] resolution id (mac) - tied with frequency (id[nrFreq])
  //possible that a CGDisplayModeRef must be used 
  #endif /// OS_MAC
  
};


// -------------========= osiMonitor =======------------------
struct osiMonitor {

  str8 name;                // monitor name (product description or something that can identify it)

  int32_t x0, y0;           // position on the VIRTUAL DESKTOP
  int32_t dx, dy;           // current size)

  //int32_t resDx, resDy;     // real monitor resolution, unafected by scale - SCRAPE THAT, WHAT WOULD A PROGRAM BUILT WITH OSI NEED ANY SCALING OR SCALING INFO?
  //uint32_t scale;           // monitor scale

  bool primary;             // is it the primary display 
  osiGPU *GPU;              // on what GPU is attached
  osiWindow *win;           // the window that is on this monitor (if there is one)
  
  int16_t nrRes;            // nr of resolutions monitor can handle
  osiResolution *res;       // all resolutions the display supports (res[nrRes])

  // the next vars are kinda internal stuff

  bool inOriginal;          // monitor is in original resolution (false= in program resolution)
  osiResolution original;   // original resolution @ program start (freq[0] is ID, not hertz)
  osiResolution progRes;    // program resolution. original&program used to detect a resolution CHANGE, and ignore multiple resolution changes if already in requested resolution (freq[0] is ID, not hertz)
  void *renderer;           // first renderer that was created on this monitor

  osiMonitor();
  ~osiMonitor();
  void delData();

  // internals from here on - these are not made private, because they might be useful

  #ifdef OS_WIN
  str8 _id;                 // [internal] win- display ID
  //str8 name;                // [internal] display's card name
  str8 _monitorID;          // [internal] monitor id (NOT USED FOR ANYTHING?... wincrap rulz)
  str8 _monitorName;        // [internal] monitor description (did not find any use for it ina ANY windows function)
  //void *_hMonitor;          // JUST DISABLED, NO NEED; monitorData func can populate this
  // if a monitor is set to duplicate another monitor, windows returns only one display,
  // with combined resolution options, and monitorID+monitorName for each. Can't do anything with any of them, so im not storing them anywhere.
  //friend LRESULT CALLBACK _processMSG(HWND hWnd, UINT m, WPARAM wParam, LPARAM lParam);
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  int _screen;              // [internal] monitor id (number)
  Window _root;             // [internal] root window of screen (monitor)
  RROutput _outID;          // [internal] xrandr output (phisical out that a monitor can be attached to; this output holds connected monitor info/supported modes too)
  RRCrtc _crtcID;           // [internal] xrandr crtc (some internal graphics card thingie that handles pixels sent to outputs->monitors)
  int _XineramaID;          // [internal] used only for _NET_WM_FULLSCREEN_MONITORS, it is found in display.populate() @ end of linux part
  osiMonitor *_right;       // [internal] points to a monitor next to this one, to the right, or NULL
  osiMonitor *_bottom;      // [internal] points to a monitor next to this one, to the bottom, or NULL
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  unsigned int _id;         // [internal] quartz monitor id
  str8 _GPUinfo;            // [internal] GPU info string, should be unique for each GPU
  uint32_t _oglDisplayMask; // [internal] OpenGL Display Mask. each monitor have a place in this mask
  #endif /// OS_MAC

  int32_t _y0;              // not changed, os specific, monitor position on the y axis

private:
  bool _inProgRes;          // [internal] flag used for res changes

  friend class osiDisplay;
  friend void _osiGetMonitorPos(osiMonitor *m);
  friend void _osiUpdateVirtualDesktop();
  friend bool _osiDoChange(osiMonitor *, osiResolution *, int16_t);
  friend void _osiPopulateGrCards(osiDisplay *);
};



struct osiGPU {
  str8 name;                // GPU description

  bool primary;             // primary GPU has the primary monitor

  int16_t nrMonitors;       // nr monitors attached
  osiMonitor **monitor;     // array with all attached monitors; [nrMonitors] in size

  int32_t ram;      // WIP
  int32_t clock;    // WIP

  void *vkGPU;      // if vulkan is on the system, this is the link to the VkPhysicalDevice
  
  uint64_t LUID;    // LUID of the graphics card;

  osiGPU() { ram= clock= 0; nrMonitors= 0; monitor= NULL; primary= false; vkGPU= NULL; LUID= 0; }

  void delData() { if(nrMonitors) { delete[] monitor; nrMonitors= 0; monitor= NULL; } primary= false; name.delData(); ram= clock= 0; }

  ~osiGPU() { delData(); }
};
