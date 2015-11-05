#include "osinteraction.h"
#include "util/typeShortcuts.h"

#ifdef OS_LINUX
#include <X11/extensions/Xinerama.h>
#endif

#ifdef USING_DIRECT3D
#include <d3d9.h>
#endif

#ifdef OS_MAC
/// cocoa uses these, unfortunately...
#undef uint
#undef ushort
#undef uint64
#undef uint32
#undef uint16
#undef word
#undef byte
#include <unistd.h>

#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFStringEncodingExt.h>
#include <CoreGraphics/CoreGraphics.h>  // trying to pinpoint quartz
#include <CoreGraphics/CGDirectDisplay.h>

//#include <CoreGraphics.framework/headers/CGDirectDisplay.h>
#endif /// OS_MAC


/* TODO:
 * [all]: possible bug: res change with only frequency change
 * [linux] restore mouse cursor for change res... ?
 * xrand has some event handling, must be checked
 */


// [MAC]: -restoring monitor resolutions is done with a single function for all monitors. _It is possible to restore resolutions for each monitor, if NEEDED_
//        - alt-tabbing sets fullscreen windows to the back; the other option would be to autohide - it would be easy to set (in cocoa.mm cocoa.createWindow() )
//          there is a single line that is commented. This mode seems pretty nice, tho; the back window can be hidden with right-click on it's taskbar icon...
 

/* stuff to KEEP AN EYE ON:
 * "horizontal span" nvidia/ati specific opengl extension
 *
 * AMD's EyeFinity, or more specifically SLS (Single Large Surface), creates a large,
 *    spanned resolution across three and up to six monitors
 * nVidia's approach to this is called "Surround" and it also creates a large,
 *    spanned resolution; however, it only supports three monitors.
 *
 *
 * Xdmx     - linux extension. i think used for rendering on other pcs then show the result on another
 * chromium - opengl extension. same as xdmx (different rendering for opengl)
 *
 */



// small internal funcs i think i will not add to the class anymore; scrap everything private ???
// check end of file for source
void updateVirtualDesktop(); 
void getMonitorPos(osiMonitor *m); // SCRAPED I THINK (it still helped, tho, many months later, so do not be hasty in deleting stuff)
osiResolution *getResolution(int dx, int dy, osiMonitor *gr);
int16 getFreq(int16 freq, osiResolution *r);

#ifdef OS_LINUX
XRRModeInfo *getMode(XRRScreenResources* s, RRMode id);
bool _xrrAvaible= false;
bool _xiAvaible= false;
int _xrrMajor, _xrrMinor;
#endif /// OS_LINUX
  
  
extern osinteraction osi;
extern ErrorHandling error;

// ------Resolution struct------ //
// ============================= //

osiResolution::osiResolution() {
  dx= dy= 0;
  nrFreq= 0;
  freq= null;
  #ifdef OS_LINUX
  _resID= null;
  _rotation= 0;
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  _id= null;
  #endif ///OS_MAC
}

osiResolution::~osiResolution() {
  delData();
}

void osiResolution::delData() {
  dx= dy= nrFreq= 0;
  if(freq) {
    delete[] freq;
    freq= null;
  }

  #ifdef OS_LINUX
  if(_resID) {
    delete[] _resID;
    _resID= null;
  }
  _rotation= 0;
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  if(_id) {
    delete[] _id;
    _id= null;
  }
  #endif /// OS_MAC
}

// ------osi Monitor struct------ //
// ============================== //

osiMonitor::osiMonitor() {
  nrRes= 0;
  res= null;
  primary= false;
  glr= null;
  inOriginal= true;     /// start in original resolution... right?
  _inProgRes= false;
  x0= y0= _y0= dx= dy= 0;
  win= null;

  /// original & program resolutions
  original.nrFreq= 1;
  original.freq= new int16[1];
  original.dx= 0;
  original.dy= 0;
  original.freq[0]= 0;

  progRes.nrFreq= 1;
  progRes.freq= new int16[1];
  progRes.dx= 0;
  progRes.dy= 0;
  progRes.freq[0]= 0;

  #ifdef OS_LINUX
  original._resID= new RRMode[1];
  original._resID[0]= 0;
  progRes._resID= new RRMode[1];
  progRes._resID[0]= 0;
  _bottom= null;
  _right= null;
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  original._id= new uint[1];
  original._id[0]= 0;
  progRes._id= new uint[1];
  progRes._id[0]= 0;
  _oglDisplayMask= 0;
  #endif /// OS_MAC
}

osiMonitor::~osiMonitor() {
  delData();
}

void osiMonitor::delData() {
  if(nrRes) {
    delete[] res;
    res= null;
    nrRes= 0;
  }
  name= "";
  x0= y0= _y0= dx= dy= 0;  /// current position & size;
  primary= false;
  win= null;

  original.dx= 0;
  original.dy= 0;
  original.freq[0]= 0;

  progRes.dx= 0;
  progRes.dy= 0;
  progRes.freq[0]= 0;
  
  #ifdef OS_WIN
  _id= "";
  #endif /// OS_WIN

  #ifdef OS_LINUX
  _bottom= null;
  _right= null;
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  _oglDisplayMask= 0;
  #endif ///OS_MAC
}



// SUBJECT OF DELETION - it helped many months later, so don't be hasty in deleting obsolete stuff
#ifdef OS_WIN
BOOL CALLBACK monitorData(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
// MIGHT BE USELESS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  MONITORINFOEX hmon;
  hmon.cbSize= sizeof(hmon);
  GetMonitorInfo(hMonitor, &hmon);
  printf("%s ", hmon.szDevice);
  if(hmon.dwFlags& MONITORINFOF_PRIMARY)
    printf("primary monitor\n");
  printf("\n");
  return true;
}
#endif ///OS_WIN







///======================================================================
// -----------============= DISPLAY CLASS =============------------------
///======================================================================

osiDisplay::osiDisplay() {
  nrMonitors= 0;
  monitor= null;
  primary= null;
  nrGPUs= 0;
  GPU= null;
  bResCanBeChanged= false;
  bGPUinfoAvaible= false;
  cchar buf[]= "osiDisplay.populate() was not called";
  bResCanBeChangedReason= buf;
  bGPUinfoAvaibleReason= buf;
  
  vdx= vdy= vx0= vy0= 0;
}

osiDisplay::~osiDisplay() {
  delData();
}

void osiDisplay::delData() {
  if(nrMonitors) {
    delete[] monitor;
    monitor= null;
  }

  if(nrGPUs) {
    delete[] GPU;
    GPU= null;
  }

  nrMonitors= 0;
  nrGPUs= 0;

  primary= null;
  vdx= vdy= vx0= vy0= 0;
}



bool osiDisplay::changePrimary(int32 dx, int32 dy, int16 freq) {      // change primary display& primary monitor resolution
  return changeRes(primary, dx, dy, freq);
}



///----------------------------------------------------------------------///
// --------------->>>>>>>>>>>>> CHANGERES <<<<<<<<<<<<<<----------------- //
///----------------------------------------------------------------------///

// doChange is doing the actual resolution change, whitout any checks. safety/ rest of handling is in osiDisplay::changeRes
// doChange is private, no need to put it in osiDisplay, as calling it won't do anything
bool doChange(osiMonitor *m, osiResolution *r, int16 freq);

bool osiDisplay::changeRes(osiMonitor *m, int32 dx, int32 dy, int16 freq) {
  bool chatty= false;
  if(!bResCanBeChanged) return false;   /// there can be reasons that res change is not possible. check bResCanBeChangedReason text
  
  // THERE HAS TO BE ALL THE CHECKS IN HERE, cuz changing 10 times the resolution
  // in 1 second (wich can freaqing happen, due to some crazy request), might
  // blow up your monitor

  // this program should NOT handle monitor plug & plays; you set the monitors before you use osi. ... right?
  
  
  // LINUX research: http://cgit.freedesktop.org/xorg/app/xrandr/tree/xrandr.c 
  //                 http://cgit.freedesktop.org/xorg/proto/randrproto/tree/randrproto.txt
  //                 * gamma seems nighmarish to set, unfortunately. There is a function in xrandr.c (line 1031)
  //                 * a 'revert' function @ line 1631 (uses functions: line[1597], line[1506], line[1532])
  //                 * hope there are no problems with 'primary' monitor, and needs to be set manually...
  //                 * XGrabServer, might be needed when res changing! (line[1683])


  /// search in data for requested resolution (hope populate() was called first)
  osiResolution *r= getResolution(dx, dy, m);
  
  if(r == null) {
    error.simple("osi:changeRes: Can't find requested resolution size");
    return false;
  }
  if(chatty) printf("requested RESOLUTION CHANGE: %dx%d %dHz (%s)\n", r->dx, r->dy, freq, m->name.d);
  
  /// if a frequency is provided, get it's ID from databanks
  int16 fID= 0, tfreq;                       /// this will help with frequencyes
  
  /// in case a frequency is supplied
  if(freq)
    fID= getFreq(freq, r);
  /// if frequency is not supplied, use defaults
  else {
    fID= getFreq(60, r);        // try 60hz
    if(fID== -1)
      fID= getFreq(59, r);      // try 59hz
    if(fID== -1)
      fID= 0;                   // just get the first freq in list (there has to be 1)
  }
  
  if(fID== -1) {               /// getFreq returns -1 if none found
    error.simple("osi:changeRes: Requested frequency not found");
    return false;
  }
  
  // check if the ORIGINAL RESOLUTION was requested <<<==================================================================
  if(!freq)                               /// frequency might be ignored (0) - A BUG CAN HAPPEN HERE: A RES CHANGE WITH ONLY FREQ CHANGE HAPPENS
    tfreq= m->original.freq[0];
  
  if((m->original.dx== dx) && (m->original.dy== dy) && (m->original.freq[0]== tfreq)) {
    if(!m->inOriginal) {                   // already in original res?
      if(chatty) printf("changeRes: IGNORE: already in original resolution\n");
      return true;
    }
    
    restoreRes(m);                       /// this will handle window configuration changes too

    return true;
  } /// if original resolution is requested
  
  
  // check if changing TO PROGRAM RESOLUTION (from original res, an alt-tab or something) <<<==========================
  if(!freq)                               /// frequency might be ignored (0)
    tfreq= m->progRes.freq[0];
  
  if((m->progRes.dx== dx) && (m->progRes.dy== dy) && (m->progRes.freq[0]== tfreq)) {
    
    /// if the monitor is not in original, the monitor is already in progRes, and there won't be any res change
    if(m->inOriginal) {
      /// per OS resolution change
      bool b;
      #ifdef OS_WIN
      b= doChange(m, r, freq);
      #else /// OS_LINUX & OS_MAC
      b= doChange(m, r, fID);
      #endif /// OS_LINUX & OS_MAC 

      if(!b) { // could not change resolution
        printf("\nERROR 1\n\n");
        restoreAllRes();
        return false;
      }
      
      /// adjust current window& monitor variables
      m->dx= dx;
      m->dy= dy;
      m->inOriginal= false;
      m->_inProgRes= true;
      
      /// update monitor's window (if there is one)
      if(m->win) {
        m->win->freq= r->freq[fID];
        if(m->win->mode== 2 || m->win->mode== 3) {
          m->win->dx= dx;
          m->win->dy= dy;
        }
      }
              
      /// all window positions can change after 1 monitor resolution change
      for(int16 a= 0; a< nrMonitors; a++)
        if(monitor[a].win) 
          if(monitor[a].win->mode== 2 || monitor[a].win->mode== 3) {
            monitor[a].win->x0= monitor[a].x0;
            monitor[a].win->y0= monitor[a].y0;
          }

      return true;
    } /// if in original resolution
    
    if(chatty) printf("changeRes: IGNORE: already in program resolution\n");
    return true; /// if already in program resolution, just return
  } /// if progRes is requested
  

  // it's not origRes requested, nor progRes requested, it's a NEW resolution <<<==================================
  
  // RESOLUTION CHANGE
  bool b;
  #ifdef OS_WIN
  b= doChange(m, r, freq);
  #else /// OS_LINUX & OS_MAC
  b= doChange(m, r, fID);
  #endif /// OS_LINUX & OS_MAC 

  if(!b) { // could not change resolution
    restoreAllRes();
    return false;
  }
  
  #ifdef OS_LINUX
  m->progRes._resID[0]= r->_resID[fID];
  #endif /// OS_LINUX

  #ifdef OS_MAC
  m->progRes._id[0]= r->_id[fID];
  #endif /// OS_MAC

  /// adjust current window& monitor variables
  m->dx= dx;
  m->dy= dy;
  m->progRes.dx= dx;        /// this is the point progRes is (re)initialized
  m->progRes.dy= dy;
  m->progRes.freq[0]= r->freq[fID];
  m->inOriginal= false;

  /// update monitor's window (if there is one)
  if(m->win) {
    m->win->freq= r->freq[fID];
    if(m->win->mode== 2 || m->win->mode== 3) {
      m->win->dx= dx;
      m->win->dy= dy;
    }
  }

  /// all window positions can change after 1 monitor resolution change
  for(int16 a= 0; a< nrMonitors; a++)
    if(monitor[a].win) 
      if(monitor[a].win->mode== 2 || monitor[a].win->mode== 3) {
        monitor[a].win->x0= monitor[a].x0;
        monitor[a].win->y0= monitor[a].y0;
      }

  return true;
}

// restores primary monitor resolution (usualy used in case only 1 window is created)
void osiDisplay::restorePrimary() {
  restoreRes(/*osi.primWin, */osi.primWin->monitor);
}

// restores all monitors to their original resolution
void osiDisplay::restoreAllRes() {
  for(short a= 0; a< nrMonitors; a++)
    if(!monitor[a].inOriginal)
      restoreRes(/*osi.primWin, */&monitor[a]);
}

///----------------------------------------------------------------------///
// --------------->>>>>>>>>>>>> RESTORERES <<<<<<<<<<<<<<---------------- //
///----------------------------------------------------------------------///

void osiDisplay::restoreRes(osiMonitor *m) {
  bool chatty= false;
  if(!bResCanBeChanged) return;
  
  if(m->inOriginal)
    return;
  if(chatty) printf("RESTORE MONITOR RESOLUTION [%s] dx[%d] dx[%d] freq[%d]\n", m->name.d, m->original.dx, m->original.dy, m->original.freq[0]);
  
  #ifdef OS_WIN
  ChangeDisplaySettingsEx(m->_id, NULL, NULL, NULL, NULL);
  ShowCursor(TRUE);   // this needs to be replaced with a in.m.showCursor() <<<<<<<<<<<<<<<<<<<<<<<<<<<<
  #endif

  #ifdef OS_LINUX
  if(!doChange(m, &m->original, 0)) {
    error.simple("ERROR: can't change back to original monitor resolution");
    osi.flags.exit= true;                   // EXIT PROGRAM IF CAN'T CHANGE BACK RESOLUTION... can't do much at this point
  }
  // show cursor here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  #endif /// OS_LINUX

  #ifdef OS_MAC
  /// this restores resolutions for ALL monitors
  CGRestorePermanentDisplayConfiguration();
  CGDisplayShowCursor(m->_id);   /// show mouse cursor? this need a little bit of further thinking      
  // in.m.showCursor(true);   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  /// set attribs for all monitors
  for(short a= 0; a< nrMonitors; a++) {
    osi.display.monitor[a].dx= osi.display.monitor[a].original.dx;
    osi.display.monitor[a].dy= osi.display.monitor[a].original.dy;
    osi.display.monitor[a].inOriginal= true;
  }

  
  /* WHY CHANGE WIN SIZE? WIN SIZE DOESN'T ACTUALLY CHANGE !!!
  /// set attribs for all windows
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(osi.win[a].isCreated) {
      osi.win[a].freq= osi.win[a].monitor->original.freq[0];
      if(osi.win[a].mode== 2 || osi.win[a].mode== 3) {
        osi.win[a].dx= osi.win[a].monitor->original.dx;
        osi.win[a].dy= osi.win[a].monitor->original.dy;
      }
    }
   */
  // do not return

  // PER MONITOR RESTORE... needs more work (in display.populate() ). probly keep the CGDisplayModeRef of the original resolution somewhere
  /* code not 100% done, but it might be never used...
  /// change resolution <<<<
  if(CGDisplaySetDisplayMode(m->id, (CGDisplayModeRef)resid, null) != kCGErrorSuccess)
    error.simple("osiDisplay::restoreRes: cannot change back to original resolution."); /// just an error, i guess, let the program try to continue functioning

  CGDisplayShowCursor(m->id);   // show mouse cursor? this need a little bit of further thinking <<<<<<<<<<<<<<<<<<<<<<< replace with osi one
   
  /// current monitor configuration adjust
  m->dx= m->original.dx;
  m->dy= m->original.dy;
  m->inOriginal= true;
  */
  
  #endif /// OS_MAC

  /// current monitor configuration adjust
  m->dx= m->original.dx;
  m->dy= m->original.dy;
  m->inOriginal= true;

  /// update monitors positions after resolution change
  for(short a= 0; a< osi.display.nrMonitors; a++)
    getMonitorPos(&osi.display.monitor[a]);

  /* WINDOW'S SIZE DOESN'T CHANGE! AND IT CAN BE USED TO RESTORE FROM A FOCUS OUT
  /// update monitor's window (if there is one)
  if(m->win) {
    //m->win->bpp= bpp;
    m->win->freq= m->original.freq[0];
    if(m->win->mode== 2 || m->win->mode== 3) {
      m->win->dx= m->dx;
      m->win->dy= m->dy;
    }
  }
  */
  
   // WHY CHANGE? THEY MINIMIZE / SET TO BACKGROUND - they don't change!
  /// all window positions can change after 1 monitor resolution change
  /*
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(osi.win[a].isCreated)
      if(osi.win[a].mode== 2 || osi.win[a].mode== 3) {
        osi.win[a].x0= osi.win[a].monitor->x0;
        osi.win[a].y0= osi.win[a].monitor->y0;
        //osi.win[a].move(osi.win[a].x0, osi.win[a].y0); // IT WON'T WORK, RIGHT?
      }
  */
  
} // osiDisplay::restoreRes



///-----------------------------------------------------------------------------///
// ----->>>>> doChange - actual per OS steps to change the resolution <<<<<----- //
///-----------------------------------------------------------------------------///

bool doChange(osiMonitor *m, osiResolution *r, int16 freq) {
  bool chatty= false;
  #ifdef OS_WIN
  DEVMODE dm;
  for(short a= 0; a< sizeof(DEVMODE); a++) ((int8 *)&dm)[a]= 0;
  dm.dmSize= sizeof(dm);
  dm.dmPelsWidth= r->dx;            /// selected screen width
  dm.dmPelsHeight= r->dy;           /// selected screen height
  dm.dmBitsPerPel= 32;              /// selected bits per pixel
  if(freq)
    dm.dmDisplayFrequency= freq;
  dm.dmFields= DM_BITSPERPEL| DM_PELSWIDTH| DM_PELSHEIGHT;

  /// try to set selected mode and get results.  NOTE: CDS_FULLSCREEN gets rid of start bar
  if(ChangeDisplaySettingsEx(m->_id, &dm, NULL, CDS_FULLSCREEN, NULL)!= DISP_CHANGE_SUCCESSFUL) {
    error.simple("OSdisplay::changeRes: can't change to requested resolution");
    return false;
  }

  /// update monitors positions after resolution change
  for(short a= 0; a< osi.display.nrMonitors; a++)
    getMonitorPos(&osi.display.monitor[a]);
  #endif /// OS_WIN

  #ifdef OS_LINUX
  /* Xrandr research:
   * 
   * 1. X11 server grab is a must, else bad things happen
   * 2. EVERY crtc must be disabled, prior to resolution change (im shure there's a way to disable some, but it might be more of a headache)
   * 3. don't forget to set the primary output back, after every crtc is re-enabled (forgot this and weird things happened)
   * 
   * 
   * specification:
   * http://cgit.freedesktop.org/xorg/proto/randrproto/tree/randrproto.txt
   * source code:
   * http://cgit.freedesktop.org/xorg/app/xrandr/tree/xrandr.c?id=9887ed4989e0abd48004598be0eb5cb06fa40bd1
   * 
  */
  if(!_xrrAvaible) return false;
  
  bool change= true;                        // actually do change resolution (DEBUG)
  bool grab= true;                          // grab the server <- there are apps that need to be on hold for this
  
  if(grab)
    XGrabServer(osi._dis);                  // GRAB SERVER
  
  XRRScreenResources *scr= XRRGetScreenResources(osi._dis, m->_root);
  XRRCrtcInfo *crtc;
  Status s;
  
  /// the monitor resolution is getting bigger/smaller; the next 2 vars hold the delta for each axis
  int changex= r->dx- m->dx;               /// delta x axis change
  int changey= r->dy- m->dy;               /// delta y axis change
  
  /// temporary vars that will hold what outputs each crtc was pumping data to
  int *nouts= new int[osi.display.nrMonitors];
  RROutput **outs = new RROutput*[osi.display.nrMonitors];
  
  for(short a= 0; a< osi.display.nrMonitors; a++) { /// for each monitor
    crtc= XRRGetCrtcInfo(osi._dis, scr, osi.display.monitor[a]._crtcID);
    /// remember what outputs this crtc was pumping information
    nouts[a]= crtc->noutput;
    outs[a]= new RROutput[nouts[a]];
    for(short b= 0; b< nouts[a]; b++) {
      outs[a][b]= crtc->outputs[b];
      if(chatty) printf("storing info: crtc[%lu] out[a%d][b%d/%d]= %lu\n", osi.display.monitor[a]._crtcID, a, b+ 1, nouts[a], outs[a][b]);
    }
    XRRFreeCrtcInfo(crtc);
  } /// for each monitor
  XRRFreeScreenResources(scr);
  
  
  /// disable all crtcs (basically disabling all monitors) - needed for virtual desktop resize
  for(short a= 0; a< osi.display.nrMonitors; a++) {
    scr= XRRGetScreenResources(osi._dis, m->_root);
    crtc= XRRGetCrtcInfo(osi._dis, scr, osi.display.monitor[a]._crtcID);
    /// actual disable
    if(chatty) printf("disabling crtc[%lu] [%s]\n", osi.display.monitor[a]._crtcID, osi.display.monitor[a].name.d);
    if(change)  // DEBUG
    XRRSetCrtcConfig(osi._dis, scr, osi.display.monitor[a]._crtcID,
                     CurrentTime, 0, 0, None, RR_Rotate_0, NULL, 0);
    
    //osi.sleep(500); // SLEEP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
    XRRFreeCrtcInfo(crtc);
    XRRFreeScreenResources(scr);
  }
  
  //osi.sleep(100);
  
  
  // VIRTUAL DESKTOP RESIZE+ EACH MONITOR PosiTION & PANNING
  // 9 monitors, arranged in a 3x3 grid:
  
  // situation 1:    situation 2:    situation 3:
  // +---+---+---+   +---+---+---+   +---+---+---+
  // | m | X | X |   |   | m | X |   |   |   |   |
  // +---+---+---+   +---+---+---+   +---+---+---+
  // | X |   |   |   |   | X |   |   | m | X | X |  ETC.
  // +---+---+---+   +---+---+---+   +---+---+---+
  // | X |   |   |   |   | X |   |   | X |   |   |
  // +---+---+---+   +---+---+---+   +---+---+---+
  // 
  // m= monitor that changed resolution
  // X= all monitors that need to change position (the rest will change if other monitors resolution change)
  
  // posibility 2: in case none on the left on m, move all monitors on the bottom-left(in case none above them, in that case algorithm stops on that branch), same on the right...
  //               in case none on bottom, move all monitors on the right-bottom, same on top.
  //               this will need a pointers to monitors on the left & top
  
  // * IF THIS ALGORITHM IS NOT ENOUGH, WELL, JUST MANUALLY CHANGE
  //   THE DANG MONITORS, IN LINUX 'CONTROL PANEL' AND THAT'S THAT!

  
  
  /// update all [monitors positions] on the [right] and [bottom] of current monitor that just changed resolution
  osiMonitor *m2;
  int tmpx= m->dx;
  int tmpy= m->dy;
  m->dx= r->dx;
  m->dy= r->dy;
  
  /// pass thru all monitors on the right
  m2= m->_right;
  while(m2) {
    m2->x0+= changex;                   /// right monitor x0 position adjust
    m2= m2->_right;
  }
  
  /// pass thru all monitors below m
  m2= m->_bottom;
  while(m2) {
    m2->_y0+= changey;                   /// bottom monitor y0 position adjust
    m2= m2->_bottom;
  }
  
  /// change the virtual desktop size
  updateVirtualDesktop();               /// updates all virtual desktop vars (atm not mm)
  // this can be avoided, and changex & changey used to update virtual desktop!!!
  if(chatty) printf("virtual desktop UPDATE: x[%d], y[%d] (monitor change delta x[%d], y[%d])\n", osi.display.vdx, osi.display.vdy, changex, changey);
  
  if(change)
  XRRSetScreenSize(osi._dis, DefaultRootWindow(osi._dis), osi.display.vdx, osi.display.vdy,
              DisplayWidthMM(m->win->_dis, m->_screen), DisplayHeightMM(m->win->_dis, m->_screen)); // the size in mm is kinda hard to compute, but doable... it might be NOT NEEDED
  
  m->_root= DefaultRootWindow(osi._dis);
  
  /// a 100 milisecs sleep between monitor re-activation? can't hurt
  osi.sleep(100); // PAUSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
  
  
  /// set panning for all monitors
  /* THIS MIGHT NOT BE NEEDED AT ALL, as all monitors are disabled & re-enabled with updated positions
  XRRPanning p;
  for(short a= 0; a< nrMonitors; a++) { /// for each monitor
    p.left= monitor[a].x0;
    p.top= monitor[a].y0;
    p.width= monitor[a].dx;
    p.height= monitor[a].dy;
  
    //XRRSetPanning(osi._dis, scr, monitor[a].crtcID, &p);
  } /// for each monitor
  */
  
  osiResolution *r2;                              /// temporary, used in next loop
  int16 id;                                       /// temporary, used in next loop
  bool primaryActivated= false;
  
  /// will pass thru all monitors twice. first time to activate primary monitor first, then the rest of monitors
  for(short pass= 0; pass< 2; pass++) {                // pass thru all monitors twice
    for(short a= 0; a< osi.display.nrMonitors; a++) {  // for each monitor
      
      if(pass== 0 && primaryActivated) continue;  /// if in pass 0 and primary monitor was activated, continue till pass 2
      
      /// in pass 0, find primary monitor (pos 0, 0)
      if(pass== 0 && (!primaryActivated)) {
        if(osi.display.monitor[a].primary) {
          if(chatty) printf("activating primary monitor first\n");
          primaryActivated= true;
        } else continue;                          /// skip until primary monitor is found
      }
      /// skip if in pass 1 and this is the primary monitor (already activated)
      if((pass== 1) && osi.display.monitor[a].primary) continue;
        
      scr= XRRGetScreenResources(osi._dis, m->_root);
      crtc= XRRGetCrtcInfo(osi._dis, scr, osi.display.monitor[a]._crtcID);
      
      if(chatty) printf("monitor%d[%s]:", a, osi.display.monitor[a].name.d);
      if(m== &osi.display.monitor[a]) {           /// current monitor that is changing resolution
        r2= r;
        id= freq;                                 /// id is supplied
        if(chatty) printf(" requested resChange x[%d], y[%d] id[%d] crtc[%lu] nouts[%d] out1[%lu]\n",
                          r2->dx, r2->dy, id, osi.display.monitor[a]._crtcID, nouts[a], outs[a][0]);
      } else {                                    /// rest of monitors
        if(osi.display.monitor[a].inOriginal) {
          r2= &osi.display.monitor[a].original;   /// if in original resolution
          if(chatty) printf(" other monitor in origRes");
        } else {
          r2= &osi.display.monitor[a].progRes;    /// if in program resolution resolution
          if(chatty) printf(" other monitor in progRes");
        }
        id= 0;                                    /// first resolution in progRes or originalRes
        if(chatty) printf(" x[%d], y[%d] id[%d] crtc[%lu] nouts[%d] out1[%lu]\n",
                          r2->dx, r2->dy, id, osi.display.monitor[a]._crtcID, nouts[a], outs[a][0]);
      }
    
      if(chatty) printf("monitor position: x[%d], y[%d]\n", osi.display.monitor[a].x0, osi.display.monitor[a]._y0);
      s= Success;
    
      if(change) // DEBUG
      s= XRRSetCrtcConfig(osi._dis, scr,              /// server connection, screen resources (virtual desktop)
                          osi.display.monitor[a]._crtcID, /// crt that will change the res
                          CurrentTime,                    /// time
                          osi.display.monitor[a].x0,      /// x monitor position on virtual desktop
                          osi.display.monitor[a]._y0,     /// y monitor position on virtual desktop (the one not changed by coordonate unification)
                          r2->_resID[id],                 /// resolution id (paired with frequency)
                          r2->_rotation,                  /// rotation
                          outs[a],                        /// outputs that will be 'pumped data to'
                          nouts[a]);                      /// number of outputs (monitors) that will change res
      
              
      //osi.sleep(50); // SLEEP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
      XRRFreeCrtcInfo(crtc);
      XRRFreeScreenResources(scr);
    
      /// quit program, if there is an error; if this function fails, monitors should be restored manually by user
      if(s!= Success) {
        for(short a= 0; a< osi.display.nrMonitors; a++)
          delete[] outs[a];
        
        delete[] outs;
        delete[] nouts;
        
        m->dx= tmpx;
        m->dy= tmpy;
      
        if(chatty) printf("error: XRRSetCrtcConfig not sucessful\n");
        error.simple("doChange: Critical error while changing monitor resolution"); // , true); DISABLED QUIT, do something in other funcs
        return false;
      } /// if changing res isn't successful
    } /// for each monitor -> reactivate all
  } /// for each monitor -> 2 steps (step 1 for primary monitor, step 2 for rest of monitors)
  
  
  XRRSetOutputPrimary(osi._dis, osi.display.primary->_root, osi.display.primary->_outID);

  if(grab)
    XUngrabServer(osi._dis);               // UNGRAB SERVER

  for(short a= 0; a< osi.display.nrMonitors; a++) {
    delete[] outs[a];
    getMonitorPos(&osi.display.monitor[a]); /// updates y0, from _y0
  }
        
  delete[] outs;
  delete[] nouts;
  
  
  /// after everything is done, a 1 sec sleep? too much?
  /// on slower systems this could cause problems if the sleep is really needed...
  osi.sleep(1000);    /// a sleep IS required. there's the need to wait for X11 to update, else the window position is wrong
  
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  /// get resolution with specified ID
  CFArrayRef modes= CGDisplayCopyAllDisplayModes(m->_id, NULL);
  const void *resid= CFArrayGetValueAtIndex(modes, r->_id[freq]);
  
  /// change resolution <<<<
  if(CGDisplaySetDisplayMode(m->_id, (CGDisplayModeRef)resid, null) != kCGErrorSuccess) {
    CFRelease(modes);
    error.simple("OSdisplay::changeRes: can't change to requested resolution");
    return false;
  }
  CFRelease(modes);
  
  /// update monitors positions after resolution change
  for(short a= 0; a< osi.display.nrMonitors; a++)
    getMonitorPos(&osi.display.monitor[a]);
  #endif /// OS_MAC

  return true;
}






















///---------------------------------------------------------------------///
// -------------->>>>>>>>>>>>>>> POPULATE <<<<<<<<<<<<<<<--------------- //
///---------------------------------------------------------------------///

void _populateGrCards(osiDisplay *);

void osiDisplay::populate() {
  bool chatty= false;
  delData();
  
  #ifdef OS_WIN
  /// resolution changes are no problem under windows
  bResCanBeChanged= true;
  bResCanBeChangedReason= "";
  
  int a, b, n, tx, ty;//, m;
  osiResolution *p;
  str8 s;
  
  bool found;
  DISPLAY_DEVICE dd= { 0 };
  DEVMODE dm= { 0 };

  dm.dmSize= sizeof(dm);
  dd.cb= sizeof(dd);

  /// virtual screen size
  vx0= GetSystemMetrics(SM_XVIRTUALSCREEN);
  vy0= -GetSystemMetrics(SM_YVIRTUALSCREEN);    /// coordonate unification
  vdx= GetSystemMetrics(SM_CXVIRTUALSCREEN);
  vdy= GetSystemMetrics(SM_CYVIRTUALSCREEN);

  // EnumDisplayDevices	DISPLAY_DEVICE		gr card(s) info

  /// find the number of displays on the system
  for(a= 0; EnumDisplayDevices(null, a, &dd, null); a++) {
    if(!(dd.StateFlags& DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))        /// should be attached to desktop ...
      continue;
    nrMonitors++;

  }
  if(!nrMonitors) {
    error.simple("osiDisplay::populate: can't find any displays");
    return;
  }

  monitor= new osiMonitor[nrMonitors];
  
  // loop thru all displays < START <--------------------------------------------------------
  for(int32 d= 0, id= -1; EnumDisplayDevices(null, d, &dd, null); d++) {    /// for each display ADAPTER
    if(!(dd.StateFlags& DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))        /// should be attached to desktop ...
      continue;
    
    id++; // [id] must be incremented ONLY with a valid monitor

    monitor[id]._id= dd.DeviceName;           // <<<<<<<<<<<<<<<<<<<
    monitor[id].name= dd.DeviceString;       // <<<<<<<<<<<<<<<<<<<
    //printf("DEVICE ID: %s\n", dd.DeviceID);
    if(chatty) printf("%s (%s)", monitor[id]._id.d, monitor[id].name.d);

    if(dd.StateFlags& DISPLAY_DEVICE_PRIMARY_DEVICE) {
      monitor[id].primary= true;
      primary= &monitor[id];
      if(chatty) printf(" primary");
    }
    if(chatty) printf("\n");
    
    /// original monitor settings
    if(EnumDisplaySettings(monitor[id]._id, ENUM_CURRENT_SETTINGS, &dm)) {
      monitor[id].original.dx= dm.dmPelsWidth;
      monitor[id].original.dy= dm.dmPelsHeight;
      monitor[id].dx= dm.dmPelsWidth;            /// current resolution dx
      monitor[id].dy= dm.dmPelsHeight;           /// current resolution dy
      monitor[id].original.freq[0]= (int16)dm.dmDisplayFrequency;
    }

    /// position on VIRTUAL DESKTOP		<--- if there's a need to find this position after a resolution change, a new func might be needed
    /// to get these vars & some rethinking
    monitor[id].x0= dm.dmPosition.x;
    monitor[id]._y0= dm.dmPosition.y;
    monitor[id].y0= vdy- dm.dmPosition.y- dm.dmPelsHeight;   /// coordonate unification
    
    for(a= 0; EnumDisplayDevices(monitor[id]._id, a, &dd, null); a++) {
      if(chatty) printf("found %d ID[%s]\nName[%s]\nString[%s]\n", a, dd.DeviceID, dd.DeviceName, dd.DeviceString);
      if(!(dd.StateFlags& DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) printf("!!not attached!!\n");

    }

    if(EnumDisplayDevices(monitor[id]._id, 0, &dd, null)) {     /// >>> FOR EACH MONITOR ON THAT DISPLAY ADAPTER <<<<
      monitor[id]._monitorID= dd.DeviceName;                  // currently i cant find any use for this
      monitor[id]._monitorName= dd.DeviceString;            //<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }

    if(chatty) printf("%s (%s)\n", monitor[id]._monitorID.d, monitor[id]._monitorName.d);
    if(chatty) printf("Monitor metrics: x0[%d] y0[%d] dx[%d] dy[%d]\n", monitor[id].x0, monitor[id].y0, monitor[id].dx, monitor[id].dy);

    /// windows vomit array size
    for(a= 0, n= 0; EnumDisplaySettings(monitor[id]._id, a, &dm) != 0; a++)
      n++;

    if(!n) {
      error.simple("osiDisplay::populate: can't find any display resolutions");
      continue;
    }

    osiResolution *tmp= new osiResolution[n+ 1]; /// +1 a safety, this list will be wiped anyways


    /// compute how many resolutions the monitor can handle
    monitor[id].nrRes= 0;
    for(a= 0; EnumDisplaySettings(monitor[id]._id, a, &dm) != 0; a++) {
      if(dm.dmBitsPerPel< 16) continue;     /// res should support at least 16 bpp

      found= false;

      for(b= 0; b< monitor[id].nrRes; b++)
        if( (dm.dmPelsWidth == tmp[b].dx) && (dm.dmPelsHeight == tmp[b].dy) )		/// already in the list
          found= true;

      if(!found){                         /// if not found in tmp list it means it's a new resolution
        tmp[monitor[id].nrRes].dx= dm.dmPelsWidth;
        tmp[monitor[id].nrRes].dy= dm.dmPelsHeight;
        monitor[id].nrRes++;
      }
    }
    n= monitor[id].nrRes;                 /// n= name shortcut

    /// width sort
    for(a= 0; a< n; a++)
      for(b= a; b< n; b++)
        if(tmp[a].dx< tmp[b].dx){
          tx= tmp[a].dx;				ty= tmp[a].dy;
          tmp[a].dx= tmp[b].dx;	tmp[a].dy= tmp[b].dy;
          tmp[b].dx= tx;				tmp[b].dy= ty;
        }
    /// height sort
    for(a= 0; a< n; a++)
      for(b= 0; b< n; b++)
        if(tmp[a].dx == tmp[b].dx)
          if(tmp[a].dy> tmp[b].dy) {
            tx= tmp[a].dx;				ty= tmp[a].dy;
            tmp[a].dx= tmp[b].dx;	tmp[a].dy= tmp[b].dy;
            tmp[b].dx= tx;				tmp[b].dy= ty;
          }

    /// resolution list build
    monitor[id].res= new osiResolution[n];
    p= monitor[id].res;

    for(a= 0; a< n; a++) {
      p[a].dx= tmp[a].dx;
      p[a].dy= tmp[a].dy;
    }

    int16 tf[256];
    for(a= 0; a< 256; a++)
      tf[a]= 0;
    
    /// compute the number of frequencies per resolution (res[].nrFreq)
    for(a= 0; EnumDisplaySettings(monitor[id]._id, a, &dm) != 0; a++) {
      if(dm.dmDisplayFlags== DM_INTERLACED)	continue;               /// ignore interlaced resolutions
      if(dm.dmDefaultSource) continue;                              /// have no clue what this is
      if(dm.dmBitsPerPel != 32) continue;                           /// only 32bpp

      for(b= 0; b< n; b++)                                          /// search for resolution & increase it's nr of frequencies
        if((dm.dmPelsWidth == p[b].dx) && (dm.dmPelsHeight == p[b].dy))
          p[b].nrFreq++;
    }

    /// alloc the list per resolution (got the max numbers)
    for(a= 0; a< n; a++) {
      p[a].freq= new int16[p[a].nrFreq];
      for(b= 0; b< p[a].nrFreq; b++)
        p[a].freq[b]= 0;				/// fill all frequencies with 0, this is used in next part
    }
    
    /// filling all frequencies with only a pass thru all windows big mess table
    /// the less times messing with win table stuff, the quicker (else can get really slow)
    for(a= 0; EnumDisplaySettings(monitor[id]._id, a, &dm) != 0; a++) {
      if(dm.dmDisplayFlags== DM_INTERLACED)	continue;               /// ignore interlaced resolutions
      if(dm.dmDefaultSource) continue;                              /// have no clue what this is
      if(dm.dmBitsPerPel != 32) continue;                           /// only 32bpp (could be 16, doesn't matter)

      /// this supposed to find the first 0 frequency in the res[] list and update it with the one found ... hope this mess works...
      for(b= 0; b< n; b++)
        if((dm.dmPelsWidth == p[b].dx) && (dm.dmPelsHeight == p[b].dy))
          for(short c= 0; c< p[b].nrFreq; c++)
            if(!p[b].freq[c]) {
              p[b].freq[c]= (short)dm.dmDisplayFrequency;
              break;
            }
    }

    /// sorting & cleaning duplicates
    for(a= 0; a< n; a++)                            /// a= each resolution
      for(b= 0; b< p[a].nrFreq- 1; b++)             /// b= each frequency
        for(short c= b+ 1; c< p[a].nrFreq; c++) {   /// c= each frequency (not b) & (until b)

          if(p[a].freq[b] == p[a].freq[c])          /// duplicate elimintation (just sets it to 0)
            p[a].freq[c]= 0;

          if(p[a].freq[b]< p[a].freq[c]) {          /// sorting
            tx= p[a].freq[b];
            p[a].freq[b]= p[a].freq[c];
            p[a].freq[c]= tx;
          }
        }

    if(chatty)
      for(a= 0; a< n; a++) {
        printf("%dx%d %d", p[a].dx, monitor[id].res[a].dy , monitor[id].res[a].nrFreq);
        for(b= 0; b< p[a].nrFreq; b++)
          printf("%d ", p[a].freq[b]);
        printf("\n");
      }

    delete[] tmp;
  } /// displays loop

  /// grcards populate: end of proc->call _grCardPopulate()
  // IT'S OVERRRRR ... 


  #endif /// OS_WIN


  #ifdef OS_LINUX
  // RESEARCH:  
  // RR_DoubleScan is the oposite of RR_Interlace !!!!!!!!!!!!
  
  short a, b, c, d, e;              /// all used in loops...
  osiResolution *p;
  
  /// check XRandR extension
  _xrrAvaible= true;
  int evBase= -1, err= -1;
  
  if(!XRRQueryExtension(osi._dis, &evBase, &err))
    _xrrAvaible= false;
  
  if(_xrrAvaible) 
    if(!XRRQueryVersion(osi._dis, &_xrrMajor, &_xrrMinor))
      _xrrAvaible= false;                            /// can't check version- something wrong
  
  if(_xrrAvaible)
    if(_xrrMajor< 1 || (_xrrMajor== 1 && _xrrMinor< 3))
      _xrrAvaible= false;                            /// work with v1.3+
  if(_xrrAvaible) {
    bResCanBeChanged= true;
    bResCanBeChangedReason= "";
  }
  
  
  /// check Xinerama extension
  _xiAvaible= true;
  int xiMajor, xiMinor;
  
  if(!XineramaQueryExtension(osi._dis, &xiMajor, &xiMinor)) 
    _xiAvaible= false;
  
  if(_xiAvaible)
    if(!XineramaIsActive(osi._dis))
      _xiAvaible= false;
  
  
  // POPULATE VIA XRANDR ============---------------------
  if(_xrrAvaible) {
    
    if(chatty) printf("XRandR: version[%d.%d] querry[%d] err[%d]\n", _xrrMajor, _xrrMinor, evBase, err);
    
    XRROutputInfo *out, *out2;
    XRRCrtcInfo *crtc;
    XRRScreenResources *scr;

    /// X11 screen info
    scr= XRRGetScreenResourcesCurrent(osi._dis, DefaultRootWindow(osi._dis));
    if(chatty) printf("Screen info: outputs= %d; modes= %d; crtcs= %d\n",scr->noutput, scr->nmode, scr->ncrtc);

    /// find the number of connected monitors  
    for(a= 0; a< scr->noutput; a++) {
       out= XRRGetOutputInfo(osi._dis, scr, scr->outputs[a]);
       if(chatty) printf("output %d: %s %s (crtc%lu)\n", a, out->name, (out->connection== RR_Connected)? "active": "no connection", out->crtc);

       if(out->connection== RR_Connected)
         nrMonitors++;
       XRRFreeOutputInfo(out);
    }

    /// create & start populating monitor structure
    monitor= new osiMonitor[nrMonitors];

    for(a= 0, b= 0; a< scr->noutput; a++) { /// for each output (empty outputs will be ignored)
      out= XRRGetOutputInfo(osi._dis, scr, scr->outputs[a]);
      /// if there is nothing connected to this output, skip it
      if(out->connection!= RR_Connected) {
        XRRFreeOutputInfo(out);
        continue;
      } 

      /// crtc that handles this output (output that IS connected to a monitor)
      crtc= XRRGetCrtcInfo(osi._dis, scr, out->crtc);
      /// can ge usefull data from a crtc
      if(chatty) {
        printf("out%lu: is on crtc%lu\n", scr->outputs[a], out->crtc);
        printf("crtc%lu: has %d outputs:", out->crtc, crtc->noutput);
        for(short z= 0; z< crtc->noutput; z++)
          printf(" %lu", crtc->outputs[z]);
        printf("\n");
      }


      /// populate monitor, lots of stuff found at this point
      monitor[b]._screen= DefaultScreen(osi._dis);   // can it be possible anymore to be a different value???
      monitor[b]._root= RootWindow(osi._dis, monitor[b]._screen);
      monitor[b]._outID= scr->outputs[a];
      monitor[b]._crtcID= out->crtc;
      monitor[b].x0= crtc->x;
      monitor[b]._y0= crtc->y;
      monitor[b].name= out->name;
      monitor[b].original.dx= crtc->width;
      monitor[b].original.dy= crtc->height;
      monitor[b].original._resID[0]= crtc->mode; /// this is the only use
      monitor[b].original._rotation= crtc->rotation;
      /// original monitor frequency is filled in further down, when the frequencies are calculated


      if((crtc->x== 0) && (crtc->y== 0)) {      /// primary monitor is in position 0, 0
        // there's XRRGetOutputPrimary() if 0,0 is a bad ideea
        monitor[b].primary= true;
        primary= &monitor[b];
      }
      monitor[b].dx= crtc->width;
      monitor[b].dy= crtc->height;

      if(chatty) printf("monitor [%s] position %d,%d crtc[%lu] out[%lu]\n", monitor[b].name.d, monitor[b].x0, monitor[b].y0, monitor[b]._crtcID, monitor[b]._outID);

      /* crtc transform tests. SEEMS NOTHING USEFULL IS HERE (not going into zooms and scales)
      XRRCrtcTransformAttributes *attr;
      XRRGetCrtcTransform(osi.primWin->dis, out->crtc, &attr);

      for(int x= 0; x< 3; x++) {
        for(int y= 0; y< 3; y++)
          printf("%d ", attr->pendingTransform.matrix[x][y]);
        printf("\n");
      }

      printf("current filter: %s\n", attr->currentFilter);
      printf("current nr of parameters: %d\n", attr->currentNparams);
      for(int x= 0; x< attr->currentNparams; x++)
        printf("param%d: %d\n", x, attr->currentParams[x]);

      getchar();
      _exit(1);  
       */

      // ***********************************************************
      // MUST TEST IF RANDR AUTOMATICALLY LOWERS THE POSSIBLE RESOLUTIONS
      // FOR MONITORS SET ON DUPLICATE. CHANSES ARE, IT DOESN'T
      // it doesn't. NOW WHAT?!
      // posibilities:
      // 1. find out what resID's monitors share (there are clear id's that both share, from tests
      //    populate both only with shared ID's
      //    a bool to mark a monitor is on duplicate (maybe for both, so further searches for the duplicated monitor is made)
      //    change res for both in case bDuplicate is true
      // 2. well... just live it like this... change a res for a monitor... 
      
      // ***********************************************************


      /// find the number of resolutions the monitor supports (this is not easy cuz of duplicate monitors)

      /// if there are multiple monitors on same crtc, they are set on DUPLICATE (mirror or watever the OS names them).
      /// each resolution MUST be avaible on all monitors in this case
      /// ex: if monitor1 can't support a resolution that monitor2 can, that resolution must go from both lists
      bool found, found2;
      RRMode *tmp= new RRMode[out->nmode];      // temporary array, will be populated with resolutions all monitors support
      short tmpSize= 0;

      for(c= 0; c< out->nmode; c++) {            // for each res (resolution==mode)
        found= true;                            /// start with true(res is found), just mark it as false on the way

        if(crtc->noutput> 1)
          for(d= 0; d< crtc->noutput; d++) {    // for each out that is duplicating
            if(monitor[b]._outID == crtc->outputs[d]) continue; /// don't test against itself

            out2= XRRGetOutputInfo(osi._dis, scr, crtc->outputs[d]);

            found2= false;                      /// start with false(res not found in out2), mark as true if found

            for(e= 0; e< out2->nmode; e++)       // for each mode in out2
              if(out->modes[c]== out2->modes[e])
                found2= true;

            /// if found2 was not marked as true, then the mode is not in out2
            if(!found2) {
              found= false;                     // mark mode as NOT FOUND in all outputs
              XRRFreeOutputInfo(out2);
              break;                            /// can actually break, as further tests are futile
            }
            XRRFreeOutputInfo(out2);
          }

        if(found)                               // if it's still true, it's a valid monitor mode
          tmp[tmpSize++]= out->modes[c];        /// temporary matrix update
      } /// for each res [c]

      /// find the real number of resolutions (based on size only)
      RRMode *tmp2= new RRMode[tmpSize];        // temporary array, will be populated without duplicate sizes in tmp
      short tmp2Size= 0;
      XRRModeInfo *i, *j;

      for(c= 0; c< tmpSize; c++) {              // for each element in tmp (eliminate duplicate sizes)
        if(c!= 0) {
          /// if last res size was the same as this one, skip it
          i= getMode(scr, tmp[c]);
          j= getMode(scr, tmp2[tmp2Size- 1]);
          if((i->height == j->height) && (i->width  == j->width))
            continue;
        }
        tmp2[tmp2Size++]= tmp[c];
      } /// for each element in tmp
      if(chatty) printf("found %d unique res\n", tmp2Size);

      /// create & populate res structure 
      monitor[b].nrRes= tmp2Size;
      monitor[b].res= new osiResolution[tmp2Size];

      for(c= 0; c< monitor[b].nrRes; c++) {       // for each res the monitor supports
        /// find the id in all the screen modes (hopefully this is the place all modes are dumped)
        i= getMode(scr, tmp2[c]);

        /// finally populate the rest of the stuff
        monitor[b].res[c].dx= i->width;
        monitor[b].res[c].dy= i->height;
        monitor[b].res[c]._rotation= RR_Rotate_0;
      }

      /// populate frequencies and modeIDs  (
      // tmp holds modeIDs that are good for all duplicating monitors
      // tmp2 holds what tmp holds, without duplicate sizes (each res size is unique)

      for(c= 0; c< monitor[b].nrRes; c++) {   // for each resolution

        /// find out how many frequencies this resolution has
        e= 0;
        i= getMode(scr, tmp2[c]);
        for(d= 0; d< tmpSize; d++) {
          j= getMode(scr, tmp[d]);
          if((i->width== j->width) && (i->height== j->height))
            e++;
        }
        /// got the number of freq/resIDs (in e)
        monitor[b].res[c].nrFreq= e;
        monitor[b].res[c].freq= new short[e];
        monitor[b].res[c]._resID= new RRMode[e];
        /// populate both frequencies & resolutionIDs
        e= 0;
        for(d= 0; d< tmpSize; d++) {
          j= getMode(scr, tmp[d]);
          if((i->width== j->width) && (i->height== j->height)) {
            monitor[b].res[c].freq[e]= (j->dotClock? (j->dotClock/ (j->hTotal* j->vTotal)): 0);
            
            // original monitor frequency
            if(monitor[b].original._resID[0]== j->id)
              monitor[b].original.freq[0]= monitor[b].res[c].freq[e];
            
            monitor[b].res[c]._resID[e++]= j->id;
          }
        } 
      } /// for each resolution

      delete[] tmp;
      delete[] tmp2;

      b++;
      XRRFreeCrtcInfo(crtc);
      XRRFreeOutputInfo(out);
    } /// for each output


    // find monitors that are glued on the right & bottom for each monitor (will help ennourmously on resolution change)
    int mx, my;                                             /// middle point on x and y
    osiMonitor *m1, *m2;                                    /// name shortcuts

    for(a= 0; a< nrMonitors; a++) {
      m1= &monitor[a];                                      /// name shortcut for monitor[a]

      /// pass thru all monitors again, to find glued ones
      for(b= 0; b< nrMonitors; b++) {
        m2= &monitor[b];                                    /// m2 will be checked against m1
        if(m1== m2) continue;                               /// same monitor, skip

        /// check if m2 is glued on the right of m1
        my= m2->_y0+ (m2->original.dy/ 2);

        if((my> m1->_y0) && (my< (m1->_y0+ m1->original.dy)))/// if it is approx on the same x axis
          if(m2->x0== (m1->x0+ m1->original.dx))            /// if it is glued on the right
            m1->_right= m2;

        /// check if m2 is glued on the bottom of m1
        mx= m2->x0+ (m2->original.dx/ 2);

        if((mx> m1->x0) && (mx< (m1->x0+ m1->original.dx))) /// if it is approx on the same y axis
          if(m2->_y0== (m1->_y0+ m1->original.dy))          /// if it is glued on the bottom
            m1->_bottom= m2;
      } /// for each monitor again
    } /// for each monitor
    
    


    XRRFreeScreenResources(scr);

    if(chatty)
      for(a= 0; a< nrMonitors; a++)
        for(b= 0; b< monitor[a].nrRes; b++) {
          printf("[%dx%d]", monitor[a].res[b].dx, monitor[a].res[b].dy);
          for(c= 0; c< monitor[a].res[b].nrFreq; c++) 
            printf(" %d[id%lu]", monitor[a].res[b].freq[c], monitor[a].res[b]._resID[c]);
          printf("\n");
        }

  } /// XRandR avaible
  
  
  
  // XInerama is used only for a display ID wich is used in WM_FULLSCREENMONITORS... that's all...
  
  /// if xinerama is not present... at least do a sketchy list of IDs
  /// i found that xinerama has the exact oposite order for the monitors as XRandr
  /// (i might be wrong, but if it is not installed, this is a dud anyway)
  for(a= nrMonitors- 1; a>= 0; a--)
    monitor[a]._XineramaID= a;
  
    
  /// try to get the XineramaID for the monitor (this is the only thing Xinerama is used for)
  int dummy1, dummy2, heads;
  
  if(!XineramaQueryExtension(osi._dis, &dummy1, &dummy2)) {
    error.console("No Xinerama extension");
    _populateGrCards(this);
    return;
  }
  
  if(!XineramaIsActive(osi._dis)) {
    error.console("Xinerama not active");
    _populateGrCards(this);
    return;
  }

  XineramaScreenInfo *xi= XineramaQueryScreens(osi._dis, &heads);
  
  for (a= 0; a< heads; a++) {
    if(chatty) printf("XINERAMA: monitor[%d/%d]: position[%dx %dy] size[%dx %dy]\n", a+ 1, heads, xi[a].x_org, xi[a].y_org, xi[a].width, xi[a].height);
    for(b= 0; b< nrMonitors; b++) {
      if(xi[a].x_org== monitor[b].x0 && xi[a].y_org== monitor[b]._y0) {
        monitor[b]._XineramaID= a;
        if(chatty) printf("monitor[%d] xineramaID[%d]\n", b, monitor[b]._XineramaID);
      }
    }
  }
  
  XFree(xi);
  
  updateVirtualDesktop();               /// update virtual desktop size

  /// coordonate unification (must be placed after the virtual desktop size is found)
  for(a= 0; a< nrMonitors; a++)
    monitor[a].y0= vdy- monitor[a]._y0- monitor[a].dy;
  

  if(chatty) printf("virtual desktop size [%dx%d]\n", vdx, vdy);
  
  // IT'S OVERRRRR ... 
  
  // OLD CODE FROM HERE
  /*
  // Display should be on the osiMonitor struct
  // and they must be 'found', i think. further research needed


  /// determine the number of monitors attached
  nrMonitors= XScreenCount(t->primWin->display);
  printf("test %d\n", (XScreenOfDisplay(t->primWin->display, 1))->width);
  
  if(chatty) printf("found %d monitors\n", nrMonitors);
  
  /// populate monitors structure
  monitor= new osiMonitor[nrMonitors];

  for(short a= 0; a< nrMonitors; a++) {             // for each monitor (screen)
    monitor[a].screen= a;
    monitor[a].root= RootWindow(t->primWin->display, a);
    getMonitorPos(&monitor[a]);
    if(chatty) printf("monitor %d: root %lx mon pos NOT DONE\n", a, monitor[a].root);

    printf("mon1 %dx%d\n", XDisplayWidth(t->primWin->display, 0), XDisplayHeight(t->primWin->display, 0));
    
    xrrs= XRRSizes(t->primWin->display, a, (int*)&monitor[a].nrRes);
    
    
      
      
    if(!monitor[a].nrRes) {
      error.simple("strange error, found no resolutions for monitor");
      continue;
    }


    

    /// populate resolutions structure for each monitor
    monitor[a].res= new osiResolution[monitor[a].nrRes];

    p= monitor[a].res;

    for(short b= 0; b< monitor[a].nrRes; b++) {           // for each resolution
      p[b].id= b;                                                 /// set res ID
      p[b].dx= xrrs[b].width;
      p[b].dy= xrrs[b].height;
      if(chatty) printf("%dx%d\n", xrrs[b].width, xrrs[b].height);
      
      
      
      /// populate frequencies for each resolution
      short *rates;
      rates= XRRRates(t->primWin->display, a, b, &p[b].nrFreq);

      if(p[b].nrFreq)
        p[b].freq= new short[p[b].nrFreq];

      for(short c= 0; c < p[b].nrFreq; c++)                // for each frequency
        p[b].freq[c]= rates[c];
    } /// resolution loop

    /// store original monitor configuration
    monitor[a].conf= XRRGetScreenInfo(t->primWin->display, monitor[a].root);
    monitor[a].original.id= XRRConfigCurrentConfiguration(monitor[a].conf, &monitor[a].original.rotation); /// store size id+ rotation (i think it has to store the rotation, it has no sense else)
    monitor[a].original.dx= monitor[a].res[monitor[a].original.id].dx;

    monitor[a].original.dy= monitor[a].res[monitor[a].original.id].dy;
    monitor[a].original.freq[0]= XRRConfigCurrentRate(monitor[a].conf);

  } /// monitor loop

  /// defaul monitor
  int m= XDefaultScreen(t->primWin->display);
  primary= &monitor[m];
  monitor[m].primary= true;
  */
  #endif /// OS_LINUX

  #ifdef OS_MAC
  /// tmp vars used
  int a, b, c, d, tx, ty;
  CGRect r;
  osiResolution *p;
  str8 s;
  uint32_t n, n2;
  const void *resid;
  uint32_t flags;
  bool found;
  uint8 buf[512];
  
  /// find the number of displays on the system
  CGDirectDisplayID *dis= new CGDirectDisplayID[MAX_WINDOWS];
  
  CGGetOnlineDisplayList(MAX_WINDOWS, dis, &n);
  
  if(!n) {
    error.simple("osiDisplay::populate: can't find any displays");
    return;
  }
  
  nrMonitors= (short)n;
  
  monitor= new osiMonitor[nrMonitors];
  
  if(chatty) printf("found %d monitors\n", nrMonitors);
  
  // loop thru all displays < START <--------------------------------------------------------
  for(d= 0; d< nrMonitors; d++) {                     /// for each monitor
    
    /// monitor id(mac) & name
    monitor[d]._id= dis[d];
    
    if(!cocoa.displayName(dis[d], &monitor[d].name))
      monitor[d].name= "Unknown Monitor";
    
    /// is it the primary monitor?
    if(CGDisplayIsMain(dis[d])) {
      monitor[d].primary= true;
      primary= &monitor[d];
    }

    /// openGL mask
    monitor[d]._oglDisplayMask= CGDisplayIDToOpenGLDisplayMask(monitor[d]._id);

    /// monitor position on virtual desktop
    r= CGDisplayBounds(dis[d]);
    monitor[d].x0= r.origin.x;
    monitor[d].y0= r.origin.y;
    monitor[d]._y0= r.origin.y;

    /// original monitor settings
    monitor[d].original.dx= (int)CGDisplayPixelsWide(dis[d]);
    monitor[d].original.dy= (int)CGDisplayPixelsHigh(dis[d]);
    
    /// current monitor resolution size
    monitor[d].dx= (int)CGDisplayPixelsWide(dis[d]);
    monitor[d].dy= (int)CGDisplayPixelsHigh(dis[d]);
    
    //monitor[d].original.freq[0]= (short)dm.dmDisplayFrequency;
      
    if(chatty) printf("monitor %d (%s):\n", d, monitor[d].name.d);
    if(chatty) printf("  id[%d] position[%dx%d] original res[%dx%d]\n", monitor[d]._id, monitor[d].x0, monitor[d].y0, monitor[d].original.dx, monitor[d].original.dy);
    
    // * MAC 10.5 required *
    // double CGDisplayRotation(dis[d]); this is macOS 10.5 onwards... <<<<<<<<<<<<<<<<<<<<<<<<<
    // ROTATION???
    
    // * MAC 10.6 required *
    //double freq= CGDisplayModeGetRefreshRate((CGDisplayModeRef)resid);
    //p2.id= CGDisplayModeGetIODisplayModeID((CGDisplayModeRef)resid);

    
    //ORIGINAL RESOLUTION!!!
    //    CGDisplayModeRef org= CGDisplayCopyDisplayMode(dis[d]); // help says it must be deallocated with CGDisplayModeRelease

    
    CFArrayRef modes= CGDisplayCopyAllDisplayModes(dis[d], NULL);
    n2= (uint)CFArrayGetCount(modes);
        
    if(chatty) printf("  vomit array is %d big\n", n2);
    
    if(chatty)
      for(a= 0; a< n2; a++) {
      
        resid= CFArrayGetValueAtIndex(modes, a);
        tx=        (int)CGDisplayModeGetWidth(          (CGDisplayModeRef)resid);
        ty=        (int)CGDisplayModeGetHeight(         (CGDisplayModeRef)resid);
        double freq=    CGDisplayModeGetRefreshRate(    (CGDisplayModeRef)resid);
        uint idt=       CGDisplayModeGetIODisplayModeID((CGDisplayModeRef)resid);
        uint32_t flags= CGDisplayModeGetIOFlags(        (CGDisplayModeRef)resid);
        CFStringRef st= CGDisplayModeCopyPixelEncoding( (CGDisplayModeRef)resid);
        CFStringGetCString(st, (char *)buf, 512, CFStringGetSystemEncoding());
      
        printf("   %02d [%dx%d] id[%d] freq[%f] flags[%x] pixel[%s]\n", a, tx, ty, idt, freq, flags, buf);

        if(flags& kDisplayModeSafetyFlags)            printf("SafetyFlags ");
        if(flags& kDisplayModeAlwaysShowFlag)         printf("AlwaysShow ");
        if(flags& kDisplayModeNeverShowFlag)          printf("NeverShow ");
        if(flags& kDisplayModeNotResizeFlag)          printf("NotResize ");
        if(flags& kDisplayModeRequiresPanFlag)        printf("RequiresPan ");
        if(flags& kDisplayModeInterlacedFlag)         printf("Interlaced ");
        if(flags& kDisplayModeSimulscanFlag)          printf("Simulscan ");
        if(flags& kDisplayModeBuiltInFlag)            printf("BuiltIn ");
        if(flags& kDisplayModeNotPresetFlag)          printf("NotPreset ");
        if(flags& kDisplayModeStretchedFlag)          printf("Stretched ");
        if(flags& kDisplayModeNotGraphicsQualityFlag) printf("NotGraphicsQuality ");
        if(flags& kDisplayModeValidateAgainstDisplay) printf("ValidateAgainst ");
        if(flags& kDisplayModeTelevisionFlag)         printf("Television ");
        if(flags& kDisplayModeValidForMirroringFlag)  printf("ValidForMirroring ");
        if(flags& kDisplayModeAcceleratorBackedFlag)  printf("AcceleratorBacked ");
        if(flags& kDisplayModeValidForHiResFlag)      printf("ValidForHiRes ");
        if(flags& kDisplayModeValidForAirPlayFlag)    printf("ValidForAirPlay ");
        if(flags& kDisplayModeValidFlag)              printf("Valid ");
        if(flags& kDisplayModeSafeFlag)               printf("Safe ");
        if(flags& kDisplayModeDefaultFlag)            printf("Default ");
        printf("\n");
        
        CFRelease(st); /// "caller is responsible for releasing the string"
      }

    
    /// compute how many resolutions the monitor can handle
    osiResolution *tmp= new osiResolution[n2+ 1]; /// +1 a safety, this list will be wiped anyways
    monitor[d].nrRes= 0;
    s= IO32BitDirectPixels;          /// 32bit... apple stores a 32bit resolution type as a string...
    
    for(a= 0; a< n2; a++) {          /// for each vommit element
      resid= CFArrayGetValueAtIndex(modes, a);
      
      /// interlaced modes are out of the picture
      flags= CGDisplayModeGetIOFlags((CGDisplayModeRef)resid);
      if(flags& kDisplayModeInterlacedFlag) continue;
      
      /// only 32bit resolutions ... SUBJECT OF INTESIVE THINKING... remove 16 bits?! <<<<<<<<<<<<<
      CFStringRef st= CGDisplayModeCopyPixelEncoding((CGDisplayModeRef)resid);
      CFStringGetCString(st, (char *)buf, 512, CFStringGetSystemEncoding());
      
      if(s!= (char *)buf) {
        CFRelease(st);
        continue;
      }

      tx= (int)CGDisplayModeGetWidth((CGDisplayModeRef)resid);
      ty= (int)CGDisplayModeGetHeight((CGDisplayModeRef)resid);

      found= false;

      for(b= 0; b< monitor[d].nrRes; b++)
        if( (tx == tmp[b].dx) && (ty == tmp[b].dy) )     /// already in the list
          found= true;

      if(!found){                         /// if not found in tmp list it means it's a new resolution
        tmp[monitor[d].nrRes].dx= tx;
        tmp[monitor[d].nrRes].dy= ty;
        monitor[d].nrRes++;
      }
      CFRelease(st);
    } /// for each vommit element
    
    n= monitor[d].nrRes;                  /// n= name shortcut
    
    if(chatty) printf("found %d unique resolutions\n", n);
    
    /// width sort
    for(a= 0; a< n; a++)
      for(b= a; b< n; b++)
        if(tmp[a].dx< tmp[b].dx){
          tx= tmp[a].dx;				ty= tmp[a].dy;
          tmp[a].dx= tmp[b].dx;	tmp[a].dy= tmp[b].dy;
          tmp[b].dx= tx;				tmp[b].dy= ty;
        }
    /// height sort
    for(a= 0; a< n; a++)
      for(b= 0; b< n; b++)
        if(tmp[a].dx == tmp[b].dx)
          if(tmp[a].dy> tmp[b].dy) {
            tx= tmp[a].dx;				ty= tmp[a].dy;
            tmp[a].dx= tmp[b].dx;	tmp[a].dy= tmp[b].dy;
            tmp[b].dx= tx;				tmp[b].dy= ty;
          }

    /// resolution list build
    monitor[d].res= new osiResolution[n];
    p= monitor[d].res;

    for(a= 0; a< n; a++) {
      p[a].dx= tmp[a].dx;
      p[a].dy= tmp[a].dy;
    }

    /// compute the number of frequencies per resolution (res[].nrFreq)
    for(a= 0; a< n2 != 0; a++) {
      resid= CFArrayGetValueAtIndex(modes, a);
      
      /// interlaced modes are out of the picture
      flags= CGDisplayModeGetIOFlags((CGDisplayModeRef)resid);
      if(flags& kDisplayModeInterlacedFlag) continue;
      
      /// only 32bit resolutions ... SUBJECT OF INTESIVE THINKING... remove 16 bits?! <<<<<<<<<<<<<
      CFStringRef st= CGDisplayModeCopyPixelEncoding((CGDisplayModeRef)resid);
      CFStringGetCString(st, (char *)buf, 512, CFStringGetSystemEncoding());
      if(s!= (char *)buf) {
        CFRelease(st);
        continue;
      }

      tx= (int)CGDisplayModeGetWidth((CGDisplayModeRef)resid);
      ty= (int)CGDisplayModeGetHeight((CGDisplayModeRef)resid);
      
      for(b= 0; b< n; b++)                                          /// search for resolution & increase it's nr of frequencies
        if((tx== p[b].dx)&& (ty== p[b].dy))
          p[b].nrFreq++;
      
      CFRelease(st);
    }

    /// alloc the list per resolution (got the max numbers)
    for(a= 0; a< n; a++) {
      p[a].freq= new short[p[a].nrFreq];
      p[a]._id= new uint[p[a].nrFreq];
      for(b= 0; b< p[a].nrFreq; b++)
        p[a]._id[b]= 0;          /// fill IDs with 0, this is used in the next part (hopefully they won't make a 0 id)
    }

    /// filling all frequencies with only a pass thru all windows big mess table
    /// the less times messing with win table stuff, the quicker (else can get really slow)
    for(a= 0; a< n2; a++) {
      resid= CFArrayGetValueAtIndex(modes, a);
      
      /// interlaced modes are out of the picture
      flags= CGDisplayModeGetIOFlags((CGDisplayModeRef)resid);
      if(flags& kDisplayModeInterlacedFlag) continue;
      
      /// only 32bit resolutions ... SUBJECT OF INTESIVE THINKING... remove 16 bits?! <<<<<<<<<<<<<
      CFStringRef st= CGDisplayModeCopyPixelEncoding((CGDisplayModeRef)resid);
      CFStringGetCString(st, (char *)buf, 512, CFStringGetSystemEncoding());

      if(s!= (char *)buf) {
        CFRelease(st);
        continue;
      }
      
      tx= (int)CGDisplayModeGetWidth((CGDisplayModeRef)resid);
      ty= (int)CGDisplayModeGetHeight((CGDisplayModeRef)resid);
      double freq= CGDisplayModeGetRefreshRate((CGDisplayModeRef)resid);
      //uint idt= CGDisplayModeGetIODisplayModeID((CGDisplayModeRef)resid);
      uint idt= a; // isn't this really the resID? IO ID might be for an even higher(that it seems it won't be used) id

      /// this supposed to find the first 0 id in the res[] list and update it with the one found ... hope this mess works...
      for(b= 0; b< n; b++)
        if((tx== p[b].dx)&& (ty== p[b].dy))
          for(c= 0; c< p[b].nrFreq; c++)
            if(!p[b]._id[c]) {
              p[b].freq[c]= (short)freq;
              p[b]._id[c]= idt;
              break;
            }
      CFRelease(st);
    }

    
    // THIS IS USELESS (at first glance, mac sorts stuff) ... but it might sort the wrong way
    /// frequency sort
    for(a= 0; a< n; a++)                            /// a= each resolution
      for(b= 0; b< p[a].nrFreq; b++)                /// b= each frequency
        for(c= b+ 1; c< p[a].nrFreq; c++)           /// c= each frequency (not b) & (until last frequency)
          if(p[a].freq[b]< p[a].freq[c]) {          /// sorting. i think this is already done in mac
            /// swap
            uint idt= p[a]._id[b];
            tx= p[a].freq[b];
            p[a].freq[b]= p[a].freq[c];
            p[a]._id[b]= p[a]._id[c];
            p[a].freq[c]= (short)tx;
            p[a]._id[c]= idt;
          }
    
    if(chatty)
      for(a= 0; a< n; a++) {
        printf("%dx%d ", p[a].dx, p[a].dy);
        for(b= 0; b< p[a].nrFreq; b++)
          printf("%d[ID%u] ", p[a].freq[b], p[a]._id[b]);
        printf("\n");
      }
    
    CFRelease(modes);
    delete[] tmp;
  } /// displays loop
  
  updateVirtualDesktop();
  
  delete[] dis;
  
// IT'S OVERRRRR ... 
  #endif /// OS_MAC

  _populateGrCards(this);
}



void _populateGrCards(osiDisplay *display) {
  bool chatty= false;
  
  #ifdef OS_WIN

  #ifndef USINGDIRECT3D
  display->bGPUinfoAvaible= false;
  display->bGPUinfoAvaibleReason= "Program not using Direct3D";    /// D3D is only used to querry cards, nothing else (oGL is missing this feature, unfortunately)
  #endif

  #ifdef USING_DIRECT3D
  if(chatty) printf("Direct3D GPU detection:\n");
  display->bGPUinfoAvaible= true;
  display->bGPUinfoAvaibleReason= "";
  
  D3DADAPTER_IDENTIFIER9 *disList= null;    /// this will store GPU/adapter/display information (term is foggy for microsoft, it's just one of these)
  IDirect3D9 *d3d;                          /// pointer to the direct3d object

  d3d= Direct3DCreate9(D3D_SDK_VERSION);    /// direct3d 'creation' (probly this just returns a pointer to the d3d main object)
  

  /// find out how many 'displays' are on the system
  int nrDis= d3d->GetAdapterCount();
  if(!nrDis) { error.console("osiDisplay::populateGrCards(): Direct3D found 0 displays"); return; }

  // populate disList
  disList= new D3DADAPTER_IDENTIFIER9[nrDis];
  
  D3DCAPS9 caps;
  int *masterAdapterID= new int[nrDis];
  
  for(short a= 0; a< nrDis; a++) {
    d3d->GetAdapterIdentifier(a, null, &disList[a]);
    d3d->GetDeviceCaps(a, D3DDEVTYPE_HAL, &caps);
    // monitors that are part of a gr card have THE SAME masterAdapterOrdinal. MasterAdapterOrdinal can be any number, as i saw id1 was just gone once
    masterAdapterID[a]= caps.MasterAdapterOrdinal;
  }

 
  // find out how many GPUs are on the system
  int *gpuID= new int[nrDis];       /// this list stores id's of grcards found (yes, it's very cryptic, but everything in windows is)

  /// the first card
  int nrCards= 1;
  gpuID[0]= masterAdapterID[0];

  /// search the rest of 'displays' to see if any belong where... (/cry)
  for(short a= 1; a< nrDis; a++) {
    bool found= false;
    for(short b= 0; b< nrCards; b++)
      if(masterAdapterID[a]== gpuID[b]) found= true;

    if(!found)  // a new id is found, belonging to another card
      gpuID[nrCards++]= masterAdapterID[a];
  }

  // populate osiDisplay::GPU
  display->nrGPUs= nrCards;                 /// number of graphics cards on the system
  display->GPU= new osiGPU[nrCards];

  /// populate osiDisplay's GPU array
  for(short a= 0; a< display->nrGPUs; a++) {
    int c;
    for(c= 0; c< nrDis; c++)
      if(masterAdapterID[c]== gpuID[a])
        break;

    display->GPU[a].name= disList[c].Description;
    if(chatty) printf("Found GPU [%d]: %s\n", a, display->GPU[a].name.d);
  }
    
  /// populate osiDisplay::monitor->GPU - each monitor belongs on what GPU
  for(short a= 0; a< nrDis; a++) {              /// pass thru all d3d's 'displays'

    /// [a] coresponds to what osiDisplay::GPU [n]
    short n;
    for(n= 0; n< nrCards; n++)
      if(masterAdapterID[a]== gpuID[n])
        break;

    HMONITOR hmon= d3d->GetAdapterMonitor(a);   /// windows HMONITOR handle
    MONITORINFOEX mon;                          /// monitor info struct
    mon.cbSize= sizeof(mon);                    /// mark struct's size (windows standard recognition procedure)
    GetMonitorInfo(hmon, &mon);                 /// populate mon

    /// search thru all osiDisplay's monitors, to find this [mon] (identify by position on virtual desktop)
    for(short b= 0; b< display->nrMonitors; b++)
      if(display->monitor[b].x0== mon.rcMonitor.left)
        if(display->monitor[b]._y0== mon.rcMonitor.top)
          if(display->monitor[b].x0+ display->monitor[b].dx== mon.rcMonitor.right)
            if(display->monitor[b]._y0+ display->monitor[b].dy== mon.rcMonitor.bottom) {
              display->monitor[b].GPU= &display->GPU[n];    // MONITOR FOUND - assign that monitor to this GPU
              if(chatty) printf("osiDisplay::monitor[%d] belongs to GPU[%d]: %s\n", b, n, display->GPU[n].name.d);
            }
    
    /* // debug tests - unfortunately, for same model cards, these descriptions are the same, so nothing can be counted on
    printf("=========================================\n");
    printf("Description [%s]\n", disList[a].Description);
    printf("DeviceId [%d]\n", disList[a].DeviceId);
    printf("DeviceIdentifier [%u][%u][%u][data4notprinted]\n", disList[a].DeviceIdentifier.Data1, disList[a].DeviceIdentifier.Data2, disList[a].DeviceIdentifier.Data3);
    printf("DeviceName [%s]\n", disList[a].DeviceName);
    printf("Driver [%s]\n", disList[a].Driver);
    printf("DriverVersion [%lld]\n", disList[a].DriverVersion);
    printf("Revision [%u]\n", disList[a].Revision);
    printf("SubSysId [%u]\n", disList[a].SubSysId);
    printf("VendorId [%u]\n", disList[a].VendorId);
    printf("WHQLLevel [%u]\n", disList[a].WHQLLevel);
    printf("=========================================\n");
    */
  } /// for each d3d 'display'


  /// populate each osiDisplay::GPU[].monitor - array of monitors that belong to this GPU
  for(short a= 0; a< display->nrGPUs; a++) {

    /// find the number of monitors that belong to this GPU[a]
    for(short b= 0; b< display->nrMonitors; b++)
      if(display->monitor[b].GPU== &display->GPU[a])
        display->GPU[a].nrMonitors++;

    if(display->GPU[a].nrMonitors) {
      display->GPU[a].monitor= new osiMonitor*[display->GPU[a].nrMonitors];

      short c= 0;
      for(short b= 0; b< display->nrMonitors; b++) 
        if(display->monitor[b].GPU== &display->GPU[a])
          display->GPU[a].monitor[c++]= &display->monitor[b];
    } /// if there are monitors attached to this GPU
  }

  display->primary->GPU->primary= true;   /// set primary monitor's GPU as the primary grCard too

  /// memory deallocation
  if(gpuID) delete[] gpuID;
  if(masterAdapterID) delete[] masterAdapterID;
  if(disList) delete[] disList;
  d3d->Release();

  #endif /// USING_DIRECT3D
  #endif /// OS_WIN

  #ifdef OS_LINUX

  if(!_xrrAvaible) {
    display->bGPUinfoAvaible= false;
    display->bGPUinfoAvaibleReason= "XRandR extension needed for GPU info";
    return;
  }
  if(_xrrMajor< 1 || (_xrrMajor== 1 && _xrrMinor< 4)) {
    display->bGPUinfoAvaible= false;
    display->bGPUinfoAvaibleReason.f("XRandr v1.4 needed. Current version is %d.%d", _xrrMajor, _xrrMinor);
    return;
  }
         
  
  XRRScreenResources *scr=     XRRGetScreenResources  (osi._dis, XDefaultRootWindow(osi._dis));
  XRRProviderResources *cards= XRRGetProviderResources(osi._dis, XDefaultRootWindow(osi._dis));

  /// graphics cards list populate
  display->nrGPUs= cards->nproviders;
  display->GPU= new osiGPU[display->nrGPUs];

  /// loop thru all cards
  for(int a= 0; a< cards->nproviders; a++) {
    XRRProviderInfo *cardInfo= XRRGetProviderInfo(osi._dis, scr, cards->providers[a]);

    // TRY TO POPULATE MORE CARD INFO (MEMORY - FREQ ETC)

    display->GPU[a].name= cardInfo->name;
    

    /// check what monitor belongs on what card
    /// find the number of monitors that belong to this card
    for(int b= 0; b< cardInfo->noutputs; b++)
      for(int c= 0; c< display->nrMonitors; c++)
        if(display->monitor[c]._outID== cardInfo->outputs[b])
          display->GPU[a].nrMonitors++;

    /// populate GPU <-> monitor info
    if(display->GPU[a].nrMonitors) {
      display->GPU[a].monitor= new osiMonitor*[display->GPU[a].nrMonitors];

      int id= 0;
      for(int b= 0; b< cardInfo->noutputs; b++)
        for(int c= 0; c< display->nrMonitors; c++)
          if(display->monitor[c]._outID== cardInfo->outputs[b]) {
            display->GPU[a].monitor[id++]= &display->monitor[c];
            display->monitor[c].GPU= &display->GPU[a];
          }
    } /// there are monitors on this GPU
    if(chatty) printf("Found GPU[%s]: monitors attached[%d]\n", display->GPU[a].name.d, display->GPU[a].nrMonitors);

    XRRFreeProviderInfo(cardInfo);        
  }
  
  display->primary->GPU->primary= true; /// primary monitor -> primary GPU
  
  XRRFreeProviderResources(cards);
  XRRFreeScreenResources(scr);
  #endif

  #ifdef OS_MAC
  /*
  #include <CoreFoundation/CoreFoundation.h>
  #include <Cocoa/Cocoa.h>
  #include <IOKit/IOKitLib.h>

  int main(int argc, const char * argv[])
  {

  while (1) {

      // Get dictionary of all the PCI Devicces
      CFMutableDictionaryRef matchDict = IOServiceMatching(kIOAcceleratorClassName);

      // Create an iterator
      io_iterator_t iterator;

      if (IOServiceGetMatchingServices(kIOMasterPortDefault, matchDict, &iterator) == kIOReturnSuccess) {
          // Iterator for devices found
          io_registry_entry_t regEntry;

          while ((regEntry = IOIteratorNext(iterator))) {
              // Put this services object into a dictionary object.
              CFMutableDictionaryRef serviceDictionary;
              if (IORegistryEntryCreateCFProperties(regEntry, &serviceDictionary, kCFAllocatorDefault, kNilOptions) != kIOReturnSuccess) {
                  // Service dictionary creation failed.
                  IOObjectRelease(regEntry);
                  continue;
              }

              CFMutableDictionaryRef perf_properties = (CFMutableDictionaryRef) CFDictionaryGetValue( serviceDictionary, CFSTR("PerformanceStatistics") );
              if (perf_properties) {

                  static ssize_t gpuCoreUse=0;
                  static ssize_t freeVramCount=0;
                  static ssize_t usedVramCount=0;

                  const void* gpuCoreUtilization = CFDictionaryGetValue(perf_properties, CFSTR("GPU Core Utilization"));
                  const void* freeVram = CFDictionaryGetValue(perf_properties, CFSTR("vramFreeBytes"));
                  const void* usedVram = CFDictionaryGetValue(perf_properties, CFSTR("vramUsedBytes"));
                  if (gpuCoreUtilization && freeVram && usedVram) {
                      CFNumberGetValue( (CFNumberRef) gpuCoreUtilization, kCFNumberSInt64Type, &gpuCoreUse);
                      CFNumberGetValue( (CFNumberRef) freeVram, kCFNumberSInt64Type, &freeVramCount);
                      CFNumberGetValue( (CFNumberRef) usedVram, kCFNumberSInt64Type, &usedVramCount);
                      NSLog(@"GPU: %.3f%% VRAM: %.3f%%",gpuCoreUse/(double)10000000,usedVramCount/(double)(freeVramCount+usedVramCount)*100.0);
                  }
              }

              CFRelease(serviceDictionary);
              IOObjectRelease(regEntry);
          }
          IOObjectRelease(iterator);
      }

     sleep(1);
  }
  return 0;
  }
  */
  
  if(chatty) printf("searching for GPU(s)...");
  display->nrGPUs= 0;
  if(!display->nrMonitors) return;
  
  str8 *s= new str8[display->nrMonitors];
  
  /// populate each monitor's gpuInfo  
  for(int a= 0; a< display->nrMonitors; a++)
    cocoa.displayGPU(display->monitor[a]._id, &display->monitor[a]._GPUinfo);
  
  /// pass thru all monitor's GPUinfo, see how many are unique = NR GPU FOUND
  for(int a= 0; a< display->nrMonitors; a++) {
    bool found= false;
    for(int b= 0; b< display->nrGPUs; b++)
      if(s[b]== display->monitor[a]._GPUinfo)
        found= true;
    
    if(!found)
      s[display->nrGPUs++]= display->monitor[a]._GPUinfo;
  }
  if(chatty) printf("found %d GPU(s)\n", display->nrGPUs);
  
  /// populate display's GPU array
  display->GPU= new osiGPU[display->nrGPUs];
  for(int a= 0; a< display->nrGPUs; a++) {
    display->GPU[a].name= s[a];
    if(chatty) printf(" GPU[%d]: [%s]\n", a, display->GPU[a].name.d);
    // more gpu info could be populated here
  }
  
  /// nr monitors on each graphics card
  for(int a= 0; a< display->nrGPUs; a++)
    for(int b= 0; b< display->nrMonitors; b++)
      if(display->monitor[b]._GPUinfo== display->GPU[a].name)
        display->GPU[a].nrMonitors++;
  
  /// populate each GPU's list of monitors that are attached to it
  for(int a= 0; a< display->nrGPUs; a++)
    if(display->GPU[a].nrMonitors) {
      display->GPU[a].monitor= new osiMonitor*[display->GPU[a].nrMonitors];
      for(int b= 0, c= 0; b< display->nrMonitors; b++)
        if(display->monitor[b]._GPUinfo== display->GPU[a].name) {
          display->GPU[a].monitor[c++]= &display->monitor[b];
          display->monitor[b].GPU= &display->GPU[a];
        }
    }
  
  display->primary->GPU->primary= true; /// set primary monitor's GPU as primary grCard
  delete[] s;
  #endif
}










// internal funcs, that are not in header file, as they got no use elsewhere
// -----------------------------------------------------------------------------

// after each resolution change, this shuld be called; it works in populate() too
void getMonitorPos(osiMonitor *m) {
  #ifdef OS_WIN
  DEVMODE dm= { 0 };
  dm.dmSize= sizeof(dm);

  // a new call to remake it's internal vomit array might be needed <<<<<<<<<<<<<<<<<<<<<<
  if(EnumDisplaySettings(m->_id, ENUM_CURRENT_SETTINGS, &dm)) {
    m->x0= dm.dmPosition.x;               /// position on VIRTUAL DESKTOP
    m->_y0= dm.dmPosition.y;              /// position on VIRTUAL DESKTOP
    m->y0= osi.display.vdy- m->_y0- m->dy;
  }
  #endif /// OS_WIN

  #ifdef OS_LINUX
  m->y0= osi.display.vdy- m->_y0- m->dy;  /// coordonate unification - y0 position
  #endif
  
  #ifdef OS_MAC
  CGRect r;
  r= CGDisplayBounds(m->_id);
  m->x0= r.origin.x;
  m->_y0= r.origin.y;
  m->y0= m->_y0;                          /// macs have the right origin *MUST TEST*
  #endif /// OS_MAC
}


#ifdef OS_LINUX
XRRModeInfo *getMode(XRRScreenResources* s, RRMode id) {
  for(short a= 0; a< s->nmode; a++)
    if(s->modes[a].id== id)
      return &s->modes[a];
  return null;
}
#endif /// OS_LINUX


osiResolution *getResolution(int dx, int dy, osiMonitor *gr) {
  for(short a= 0; a< gr->nrRes; a++) {
    osiResolution *p= &gr->res[a];
    if((dx == p->dx) && (dy == p->dy))
      return p;
  }

  return null;
}


int16 getFreq(int16 freq, osiResolution *r) {
  for(short a= 0; a< r->nrFreq; a++)
    if(freq == r->freq[a])
      return a;

  return -1;
}


/// virtual desktop resize
void updateVirtualDesktop() {
  osi.display.vx0= osi.display.primary->x0;
  osi.display.vy0= osi.display.primary->_y0;
  osi.display.vdx= osi.display.primary->dx;
  osi.display.vdy= osi.display.primary->dy;
  
  #ifdef OS_LINUX
  /// linux has _NET_WM_FULLSCREEN_MONITORS (http://standards.freedesktop.org/wm-spec/wm-spec-latest.html)
  /// top, bottom, left, right monitors are all Xinerama monitor IDs, and must be found here
  osi.display._left= osi.display._top= osi.display.primary->_XineramaID;
  osi.display._right= osi.display._bottom= osi.display.primary->_XineramaID;
  #endif 
  
  for(short a= 0; a< osi.display.nrMonitors; a++) {                              /// for each monitor
    if(osi.display.monitor[a].x0< osi.display.vx0) {                             /// <<
      osi.display.vx0= osi.display.monitor[a].x0;
      #ifdef OS_LINUX
      osi.display._left= osi.display.monitor[a]._XineramaID;
      #endif
    }
    if(osi.display.monitor[a]._y0< osi.display.vy0) {                            /// ^^
      osi.display.vy0= osi.display.monitor[a]._y0;
      #ifdef OS_LINUX
      osi.display._top= osi.display.monitor[a]._XineramaID;
      #endif
    }
    if(osi.display.monitor[a].x0+ osi.display.monitor[a].dx> osi.display.vdx) {  /// >>
      osi.display.vdx= osi.display.monitor[a].x0+ osi.display.monitor[a].dx;
      #ifdef OS_LINUX
      osi.display._right= osi.display.monitor[a]._XineramaID;
      #endif
    }
    if(osi.display.monitor[a]._y0+ osi.display.monitor[a].dy> osi.display.vdy) { /// vv
      osi.display.vdy= osi.display.monitor[a]._y0+ osi.display.monitor[a].dy;
      #ifdef OS_LINUX
      osi.display._bottom= osi.display.monitor[a]._XineramaID;
      #endif
    }
  } /// for each monitor

  #ifndef OS_MAC /// LINUX + WIN
  /// coordonate unification - switch vdy and vy0
  /* THIS IS WRONG - DELETE INCOMING
  int t= osi.display.vdy;
  osi.display.vdy= osi.display.vy0;
  osi.display.vy0= t;
  */
  #endif /// OS_WIN + OS_LINUX

  /// vdx & vdy currently hold an end coordonate- they need to be delta x & y
  osi.display.vdx-= osi.display.vx0;
  osi.display.vdy-= osi.display.vy0;
}





// THIS CODE CREATES TEMPORARY WINDOWS/CONTEXTS TO GET GL EXTENSIONS THAT MIGHT DETECT GRAPHICS CARDS
// BUT ATM, THERE IS NO GL EXTENSION THAT CAN HELP IN IDENTIFIEING GPUs
// IF IN THE FUTURE THERE WILL BE ONE... 
/*
class _TMP_WINDOW{
public:
  cchar *wname;
  HWND hWnd;
  WNDCLASS wc;
  HINSTANCE hInstance;
  HDC hDC;
  GLuint pixelFormat;
  PIXELFORMATDESCRIPTOR pfd;
  DWORD style, exStyle;

  bool createWindow(osiMonitor *);

  _TMP_WINDOW();
  ~_TMP_WINDOW() { delData(); }
  void delData();
};


_TMP_WINDOW::_TMP_WINDOW():wname("tmp") {
  hWnd= 0;
  hInstance= 0;
  hDC= 0;
  pixelFormat= 0;
  // exStyle= WS_EX_APPWINDOW| WS_EX_WINDOWEDGE;
  style= WS_OVERLAPPEDWINDOW| WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  exStyle= 0; // WS_EX_APPWINDOW; // 0 might not work
  //style= WS_POPUP| WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  for(int a= 0; a< sizeof(wc); a++) ((char *)&wc)[a]= 0;  /// wipe [wc] ... lol
  wc.style=         CS_HREDRAW| CS_VREDRAW| CS_OWNDC;     /// Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc=   (WNDPROC)processMSG;                  /// processMSG handles messages
  wc.hInstance=     0;                                    /// set the aquired instance
  wc.hIcon=         LoadIcon(NULL, IDI_WINLOGO);          /// load default icon
  wc.hCursor=       LoadCursor(NULL, IDC_ARROW);          /// load arrow pointer
  wc.lpszClassName= wname;                                /// class name... dunno for shure what this is


  exStyle= WS_EX_APPWINDOW| WS_EX_WINDOWEDGE;
  style= WS_OVERLAPPEDWINDOW;

  wc.style				 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  /// Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc	 = (WNDPROC) processMSG;                // processMSG handles messages
  wc.cbClsExtra		 = 0;                                   /// no extra
  wc.cbWndExtra		 = 0;                                   /// no extra
  wc.hInstance		 = 0;                       /// set the aquired instance
  wc.hIcon				 = LoadIcon(NULL, IDI_WINLOGO);         // load default icon <<<<<<<<<<<<<<<<<<<<<< ICON WORKS MUST BE MADE
  wc.hCursor			 = LoadCursor(NULL, IDC_ARROW);         /// load arrow pointer
  wc.hbrBackground = NULL;                                /// no backgraound required when using opengl
  wc.lpszMenuName	 = NULL;                                /// no menus
  wc.lpszClassName = wname;                               /// class name... dunno for shure what this is



}


void _TMP_WINDOW::delData() {
  if(hDC) ReleaseDC(hWnd, hDC);
  if(hWnd) DestroyWindow(hWnd);
  UnregisterClass(wname, hInstance);
}


bool _TMP_WINDOW::createWindow(osiMonitor *m) {
  bool err= false;

  hInstance= GetModuleHandle(NULL);            /// grab an instance for window
  wc.hInstance= hInstance;
  if(!RegisterClass(&wc)) err= true;
  //if(!(hWnd= CreateWindowEx(exStyle, wname, wname, style, m->x0, m->y0, 100, 100, null, null, hInstance, null))) err= true;

  if (!(hWnd= CreateWindowEx(
                exStyle,              // Extended Style For The Window
                wname,                   // class name           <--- might want a different class name?
                wname,                   /// window title
                style |               /// defined window style
                WS_CLIPSIBLINGS |       /// Required Window Style ?? not shure
                WS_CLIPCHILDREN,        /// Required Window Style ?? not shure
                m->x0, m->y0, /// window position (coord unification fixed)
                100, // rect.right- rect.left,  /// dx
                100, //rect.bottom- rect.top,  /// dy
                NULL,           /// parent window
                NULL,                   /// no menu
                hInstance,          /// instance
                NULL)))                 /// don't pass anything to WM_CREATE
  {
    error.window("Window creation error.");
    return false;
  }



  if(!(hDC= GetDC(hWnd))) err= true;

  /// pixel format
  int pixelf= GetPixelFormat(hDC); 
  DescribePixelFormat(hDC, pixelf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  pfd.dwFlags= pfd.dwFlags| PFD_DRAW_TO_WINDOW| PFD_DRAW_TO_BITMAP| PFD_SUPPORT_OPENGL| PFD_STEREO_DONTCARE| PFD_DOUBLEBUFFER;
  if(!(pixelFormat= ChoosePixelFormat(hDC, &pfd))) err= true;
  if(!SetPixelFormat(hDC, pixelFormat, &pfd)) err= true;
  
  ShowWindow(hWnd, SW_SHOW);
  SetForegroundWindow(hWnd);  /// Slightly Higher Priority
  SetFocus(hWnd);             /// Sets Keyboard Focus To The Window

  if(err) return false;
  return true;
}


template<class T> extern bool getGlProc(cchar *, T&);   /// defined in osiGlExt.cpp

void _populateGrCards(osiDisplay *display) {
  bool chatty= false;

  #ifdef OS_WIN
  
    bool err= false;
  bool nvAvaible= false, amdAvaible= false;
  _TMP_WINDOW w;

  if(!w.createWindow(display->primary)) error.window("dummy window failed");
  
  /// oGL context creation
  HGLRC tmp= wglCreateContext(w.hDC);
  wglMakeCurrent(w.hDC, tmp);



  int major;
  char buf[128];
  cchar *ext;
  glGetIntegerv(GL_MAJOR_VERSION, &major);         /// oGL major version


  if(major< 3) {
    ext= (cchar *)glGetString(GL_EXTENSIONS);            /// oGL extensions string
    cchar *p= ext;
    while(1) {
      
      /// parse 1 extension at a time
      for(short a= 0; a< 128; a++) {
        if(*p== ' ' || *p== '\0') {  /// extension delimiter or end of string
          buf[a]= 0;
          break;
        }

        buf[a]= *p;
        p++;
      } /// for each character in current ext string

      if(!string8::strcmp(buf, "WGL_NV_gpu_affinity"))
        nvAvaible= true;
      if(!string8::strcmp(buf, "WGL_AMD_gpu_association"))
        amdAvaible= true;

      if(*p== '\0')
        return;                               /// reached the end
      else
        p++;                                  /// pass ' ' (space) for next extension
    } /// for each extension
  } else {
    PFNGLGETSTRINGIPROC glGetStringi_tmp;
    getGlProc("glGetStringi", glGetStringi_tmp);      /// make shure this func is avaible

    int max;
    glGetIntegerv(GL_NUM_EXTENSIONS, &max);

    for(short a= 0; a< max; a++) {
      ext= (cchar *)glGetStringi_tmp(GL_EXTENSIONS, a);

      if(!string8::strcmp(ext, "WGL_NV_gpu_affinity"))
        nvAvaible= true;
      if(!string8::strcmp(ext, "WGL_AMD_gpu_association"))
        amdAvaible= true;
    }
  }

  /// nvidia extension
  PFNWGLENUMGPUSNVPROC wglEnumGpusNV_tmp= null;
  PFNWGLENUMGPUDEVICESNVPROC wglEnumGpuDevicesNV_tmp= null;
  PFNWGLCREATEAFFINITYDCNVPROC wglCreateAffinityDCNV_tmp= null;
  PFNWGLENUMGPUSFROMAFFINITYDCNVPROC wglEnumGpusFromAffinityDCNV_tmp= null;
  PFNWGLDELETEDCNVPROC wglDeleteDCNV_tmp= null;
  /// amd extension
  PFNWGLGETGPUIDSAMDPROC wglGetGPUIDsAMD_tmp= null;
  PFNWGLGETGPUINFOAMDPROC wglGetGPUInfoAMD_tmp= null;
  PFNWGLGETCONTEXTGPUIDAMDPROC wglGetContextGPUIDAMD_tmp= null;
  //PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC wglCreateAssociatedContextAMD_tmp= null;
  //PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC wglCreateAssociatedContextAttribsAMD_tmp= null;
  //PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC wglDeleteAssociatedContextAMD_tmp= null;
  //PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC wglMakeAssociatedContextCurrentAMD_tmp= null;
  //PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC wglGetCurrentAssociatedContextAMD_tmp= null;
  //PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC wglBlitContextFramebufferAMD_tmp= null;


  if(nvAvaible) {
    getGlProc("wglEnumGpusNV", wglEnumGpusNV_tmp);
    getGlProc("wglEnumGpuDevicesNV", wglEnumGpuDevicesNV_tmp);
    getGlProc("wglCreateAffinityDCNV", wglCreateAffinityDCNV_tmp);
    getGlProc("wglEnumGpusFromAffinityDCNV", wglEnumGpusFromAffinityDCNV_tmp);
    getGlProc("wglDeleteDCNV", wglDeleteDCNV_tmp);
  }
  if(amdAvaible) { // only used funcs are aquired
    getGlProc("wglGetGPUIDsAMD", wglGetGPUIDsAMD_tmp);
    getGlProc("wglGetGPUInfoAMD", wglGetGPUInfoAMD_tmp);
    getGlProc("wglGetContextGPUIDAMD", wglGetContextGPUIDAMD_tmp);
    //getGlProc("wglCreateAssociatedContextAMD", wglCreateAssociatedContextAMD_tmp);
    //getGlProc("wglCreateAssociatedContextAttribsAMD", wglCreateAssociatedContextAttribsAMD_tmp);
    //getGlProc("wglDeleteAssociatedContextAMD", wglDeleteAssociatedContextAMD_tmp);
    //getGlProc("wglMakeAssociatedContextCurrentAMD", wglMakeAssociatedContextCurrentAMD_tmp);
    //getGlProc("wglGetCurrentAssociatedContextAMD", wglGetCurrentAssociatedContextAMD_tmp);
    //getGlProc("wglBlitContextFramebufferAMD", wglBlitContextFramebufferAMD_tmp);
  }

  
  if(!amdAvaible && !nvAvaible) {
    if(chatty) printf("WARNING: neither WGL_NV_gpu_affinity or WGL_AMD_gpu_association is avaible\n");
  }

  /// favour amd extension, as there is a linux version too
  if(amdAvaible) {

    display->nrGPUs= wglGetGPUIDsAMD_tmp(0, 0);               /// number of gpus on the system
    if(display->nrGPUs) {
      display->GPU= new osiGPU[display->nrGPUs];              /// create list of them

      uint *cards= new uint[display->nrGPUs];                 /// this will hold amd's internal gpu ids
      wglGetGPUIDsAMD_tmp(display->nrGPUs, cards);

      if(chatty) printf("Found %d GPUs:\n", display->nrGPUs);

      /// gather data on each card
      for(short a= 0; a< display->nrGPUs; a++) {
        // WGL_GPU_OPENGL_VERSION_STRING_AMD - highest supported OpenGL version string (GL_UNSIGNED_BYTE string)
        // WGL_GPU_RENDERER_STRING_AMD - returns name of the GPU (GL_UNSIGNED_BYTE string)
        // WGL_GPU_FASTEST_TARGET_GPUS_AMD
        // WGL_GPU_RAM_AMD - the amount of RAM available to GPU in MB
        // WGL_GPU_CLOCK_AMD - the GPU clock speed in MHz
        // WGL_GPU_NUM_PIPES_AMD - the nubmer of 3D pipes
        // WGL_GPU_NUM_SIMD_AMD - the number of SIMD ALU units in each shader pipe
        // WGL_GPU_NUM_RB_AMD - the number of render backends
        // WGL_GPU_NUM_SPI_AMD - the number of shader parameter interpolaters
        char buf[256];
        wglGetGPUInfoAMD_tmp(cards[a], WGL_GPU_RENDERER_STRING_AMD, GL_UNSIGNED_BYTE, 256, buf);
        display->GPU[a].id= buf;
        wglGetGPUInfoAMD_tmp(cards[a], WGL_GPU_RAM_AMD, GL_INT, 1, &display->GPU[a].ram);
        wglGetGPUInfoAMD_tmp(cards[a], WGL_GPU_CLOCK_AMD, GL_INT, 1, &display->GPU[a].clock);
        if(chatty) printf("GPU[%d]: id[%s] mem[%d] clock[%d]", a, display->GPU[a].id.d, display->GPU[a].ram, display->GPU[a].clock);
      } /// for each gr card

      /// try to determine which monitor belongs to which gpu
      _TMP_WINDOW test;
      
      for(short a= 0; a< display->nrMonitors; a++) {
        test.createWindow(&display->monitor[a]);          /// create dummy window
        HGLRC testRC= wglCreateContext(test.hDC);         /// create dummy context

        wglMakeCurrent(test.hDC, testRC);
        int id= wglGetContextGPUIDAMD_tmp(testRC);        /// check on what GPU id was this context created

        /// pass thru all grcards, to match the found id, and assign the monitor to that card
        for(short b= 0; b< display->nrGPUs; b++)
          if(cards[b]== id) {
            display->monitor[a].GPU= &display->GPU[b];
            display->GPU[a].nrMonitors++;
            if(chatty) printf("Monitor [%d] belongs to GPU [%d]\n", a, b);
          }

        wglDeleteContext(testRC);                         /// delete dummy context
        test.delData();                                   /// delete dummy window
      } /// for each monitor


      /// pass thru all cards, assigning each card the monitors that are attached to it
      for(short a= 0; a< display->nrGPUs; a++) {
        if(display->GPU[a].nrMonitors)
          display->GPU[a].monitor= new osiMonitor*[display->GPU[a].nrMonitors];
        /// pass thru all monitors, to see if any are part of this GPU
        for(short b= 0, c= 0; b< display->nrMonitors; b++)
          if(display->monitor[b].GPU== &display->GPU[a])
            display->GPU[a].monitor[c++]= &display->monitor[b];
      }


     if(cards) delete[] cards;

    }

  } else if(nvAvaible) {



  }



  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(tmp);


  #endif /// OS_WIN
}

THIS IS NOT WORKING. IF ANY OPENGL EXTENSION IS AVAIBLE IN THE FUTURE, THIS CODE WILL WORK FOR THAT SAID EXTENSION
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
*/













