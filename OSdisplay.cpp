#include "pch.h"

//#include <stdlib.h>
//#include <unistd.h>

#ifdef OS_MAC
#include <CoreGraphics/CGDirectDisplay.h>
//#include <CoreGraphics.framework/headers/CGDirectDisplay.h>
#endif /// OS_MAC

/*
#ifdef OS_WIN
#include <Windows.h>
#endif

#include "OSinteraction.h"
#include "OSdisplay.h"
*/


// [MAC]: -restoring monitor resolutions is done with a single function for all monitors. _It is possible to restore resolutions for each monitor, if NEEDED_
//        - alt-tabbing sets fullscreen windows to the back; the other option would be to autohide - it would be easy to set (in cocoa.mm cocoa.createWindow() )
//          there is a single line that is commented. This mode seems pretty nice, tho; the back window can be hidden with right-click on it's taskbar icon...
/* TODO:
 *
 * WIN: to use progRes
 * WIN: move getMonitorPos in populate(). it's win only, and small, used only in populate() anyways
 * xrand has some event handling, must be checked
 *  
 * LINUX: primary monitor: who the heck is it?
 * 
 

 
stuff to KEEP AN EYE ON:
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

#ifdef OS_WIN
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#endif /// OS_WIN

extern OSInteraction osi;
extern ErrorHandling error;

// ------Resolution struct------ //
// ============================= //

OSIResolution::OSIResolution() {
  dx= dy= 0;
  nrFreq= 0;
  freq= null;
  #ifdef OS_LINUX
  resID= null;
  rotation= 0;
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  id= null;
  #endif ///OS_MAC
}

OSIResolution::~OSIResolution() {
  delData();
}

void OSIResolution::delData() {
  dx= dy= nrFreq= 0;
  if(freq) {
    delete[] freq;
    freq= null;
  }
  
  #ifdef OS_LINUX
  if(resID) {
    delete[] resID;
    resID= null;
  }
  rotation= 0;
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  if(id) {
    delete[] id;
    id= null;
  }
  #endif /// OS_MAC
}

// ------OSI Monitor struct------ //
// ============================== //

OSIMonitor::OSIMonitor() {
  nrRes= 0;
  res= null;
  primary= false;
  glRenderer= null;
  inOriginal= true;     /// start in original resolution... right?

/// original & program resolutions
  original.nrFreq= 1;
  original.freq= new short[1];
  original.dx= 0;
  original.dy= 0;
  original.freq[0]= 0;

  progRes.nrFreq= 1;
  progRes.freq= new short[1];
  progRes.dx= 0;
  progRes.dy= 0;
  progRes.freq[0]= 0;

  #ifdef OS_LINUX
  original.resID= new RRMode[1];
  original.resID[0]= 0;
  progRes.resID= new RRMode[1];
  progRes.resID[0]= 0;
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  original.id= new uint[1];
  original.id[0]= 0;
  progRes.id= new uint[1];
  progRes.id[0]= 0;
  #endif /// OS_MAC
}

OSIMonitor::~OSIMonitor() {
  delData();
}

void OSIMonitor::delData() {
  if(nrRes) {
    delete[] res;
    res= null;
    nrRes= 0;
  }
  #ifdef OS_WIN
  id= "";
  name= "";
  #endif /// OS_WIN
  primary= false;

  original.dx= 0;
  original.dy= 0;
  original.freq[0]= 0;

  progRes.dx= 0;
  progRes.dy= 0;
  progRes.freq[0]= 0;
  
  #ifdef OS_MAC
  this->name= "";
  #endif ///OS_MAC
}



// SUBJECT OF DELETION
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


// WIP vvvvvvvvvv

// Renderer CHAINLIST //
// ------------------ //
Renderer::Renderer() {
  glRenderer= null;
}

Renderer::~Renderer() {
  delData();
}


void Renderer::delData() {
  /*
  if(glRenderer) {
    osi.glMakeCurrent(null, null);
    osi.glDeleteContext(glRenderer);
    //wglMakeCurrent(null, null);
    //wglDeleteContext(glRenderer);
    glRenderer= null;
  }
   */

}
// WIP ^^^^^^^^^^^^^^^^^^


// -----------============= DISPLAY CLASS =============------------------

OSIDisplay::OSIDisplay() {
  nrCards= 0;
  nrMonitors= 0;
  monitor= null;
  primary= null;
  vdx= vdy= vx0= vy0= 0;
}

OSIDisplay::~OSIDisplay() {
  delData();
}

void OSIDisplay::delData() {
  if(nrMonitors) {
    delete[] monitor;
    monitor= null;
    nrMonitors= 0;
  }
  primary= null;
  nrCards= 0;
  vdx= vdy= vx0= vy0= 0;
}



bool OSIDisplay::changePrimary(short dx, short dy, int8 bpp, short freq) {      // change primary display& primary monitor resolution
  return changeRes(osi.primWin, primary, dx, dy, bpp, freq);
}



///----------------------------------------------------------------------///
// --------------->>>>>>>>>>>>> CHANGERES <<<<<<<<<<<<<<----------------- //
///----------------------------------------------------------------------///
bool OSIDisplay::changeRes(OSIWindow *w, OSIMonitor *m, short dx, short dy, int8 bpp, short freq) {
  bool chatty= true;
  
  // THIS FUNCTION MIGHT CHANGE, so it will be only 1 code with multiple, per OS res change calls
  ///^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  
  #ifdef OS_WIN
  DEVMODE dm;
  memset(&dm, 0, sizeof(dm));
  dm.dmSize= sizeof(dm);
  dm.dmPelsWidth= dx;           /// selected screen width
  dm.dmPelsHeight= dy;          /// selected screen height
  dm.dmBitsPerPel= bpp;         /// selected bits per pixel
  if(freq)
    dm.dmDisplayFrequency= freq;
  dm.dmFields= DM_BITSPERPEL| DM_PELSWIDTH| DM_PELSHEIGHT;

  /*
  MAKE ME SAFE, AS IN, WHAT IS IN LINUX VARIANT. multiple(fast) resolution changes
  can fry the monitor
  */




  /// try to set selected mode and get results.  NOTE: CDS_FULLSCREEN gets rid of start bar
  if(ChangeDisplaySettingsEx(m->id, &dm, NULL, CDS_FULLSCREEN, NULL)!= DISP_CHANGE_SUCCESSFUL) {
    error.simple("This full screen is mode not supported");
    return false;
  }
  return true;
  #endif /// OS_WIN

  #ifdef OS_LINUX

  /// search in data for requested resolution (hope populate() was called first)
  OSIResolution *r= getResolution(dx, dy, m);
  
  if(r == null) {
    error.simple("OSI:changeRes: Can't find requested resolution size");
    return false;
  }
  if(chatty) printf("requested resolution change: %dx%d %dHz\n", r->dx, r->dy, freq);
  
  short f= 0;
  Status s;
  if(freq) f= getFreq(freq, r);

  if(f== -1) {
    error.simple("OSI:changeRes: Requested frequency not found");
    return false;
  }

// THERE HAS TO BE ALL THE CHECKS IN HERE, cuz changing 10 times the resolution
// in 1 second (wich can freaqing happen, due to some crazy request), might
// blow up your monitor

  /// check if the ORIGINAL RESOLUTION was requested
  if(!freq)                               /// frequency might be ignored (0)
    f= m->original.freq[0];

  if((m->original.dx== dx) && (m->original.dy== dy) && (m->original.freq[0]== f)) {
    if(m->inOriginal) {                   // already in original res?
      if(chatty) printf("changeRes: IGNORE: already in original resolution\n");
      return true;
    }

    restoreRes(w, m);
    return true;
  } /// if original resolution is requested

  /// check if changing TO PROGRAM RESOLUTION (from original probly, an alt-tab or something)
  if(!freq)                               /// frequency might be ignored (0)
      f= m->progRes.freq[0];

  if((m->progRes.dx== dx) && (m->progRes.dy== dy) && (m->progRes.freq[0]== f)) {
    if(m->inOriginal) {
      XRRScreenResources *scr= XRRGetScreenResources(w->dis, w->win);
      XRRCrtcInfo *crtc= XRRGetCrtcInfo(w->dis, scr, m->crtcID);
      if(chatty) printf("m->outID:%lu crtc->noutputs:%d crtc->outputs[0]:%lu\n", m->outID, crtc->noutput, crtc->outputs[0]);
      
      
      s= XRRSetCrtcConfig(w->dis, scr,    /// display, screen resources (virtual desktop configuration)
                          m->crtcID,      /// crt that will change resolution
                          CurrentTime,
                          m->x0, m->y0,   /// monitor position on virtual screen
                          r->resID[f],
                          r->rotation,
                          crtc->outputs,  /// outputs(+monitors) that will change resolution
                          crtc->noutput); /// will change res on all (noutput) duplicate monitors

      XRROutputInfo *out= XRRGetOutputInfo(w->dis, scr, crtc->outputs[0]);
      XRRSetScreenSize(w->dis, m->root, dx, dy, out->mm_width, out->mm_height);
      XRRFreeOutputInfo(out);
  
      XRRFreeCrtcInfo(crtc);
      XRRFreeScreenResources(scr);

      if(s!= Success) {
        error.simple("OSdisplay::changeRes: can't change to requested resolution");
        return false;
      }
      
      // TESTS
      
      
      /*
      sleep(3);
      XRRPanning p= {0};                            // try DISABLE panning
      
      scr= XRRGetScreenResources(w->dis, m->root);
      XRRSetPanning(w->dis, scr, m->crtcID, &p);
      XRRFreeScreenResources(scr);
      */
      // END TESTS

      w->bpp= bpp;
      w->dx= dx;
      w->dy= dy;
      w->freq= r->freq[f];
      m->inOriginal= false;

      return true;
    } /// if in original resolution
    if(chatty) printf("changeRes: IGNORE: already in program resolution\n");
    return true; /// if already in program resolution, just return
  } /// if progRes is requested

/// it's not origRes requested, nor progRes requested, it's a NEW resolution
  /// if frequency is not supplied
  if(!freq) {
    f= getFreq(60, r);        // try 60hz
    if(f== -1)
      f= getFreq(59, r);      // try 59hz
    if(f== -1)
      f= 0;                   // just get the first freq in list (there has to be 1)
  }
  
  XRRScreenResources *scr= XRRGetScreenResources(w->dis, m->root);
  XRRCrtcInfo *crtc= XRRGetCrtcInfo(w->dis, scr, m->crtcID);
  
  
  //XRRFreeScreenResources(scr);
  //*scr= XRRGetScreenResources(w->dis, m->root);
  
  if(chatty) printf("m->outID:%lu crtc->noutputs:%d crtc->outputs[0]:%lu\n", m->outID, crtc->noutput, crtc->outputs[0]);
  s= XRRSetCrtcConfig(w->dis, scr,         /// server connection, screen resources (virtual desktop)
                      m->crtcID,           /// crt that will change the res
                      CurrentTime,         /// time
                      m->x0, m->y0,        /// monitor position
                      r->resID[f],         /// resolution id (paired with frequency)
                      r->rotation,         /// rotation
//    &m->outID, 1);
                      crtc->outputs,       /// outputs that will change (duplicate monitors all will change res)
                      crtc->noutput);      /// nr monitors that will change res
  
  XRROutputInfo *out= XRRGetOutputInfo(w->dis, scr, crtc->outputs[0]);        // MIGHT WORK WITH ONLY 1 MONITOR. not gonna recalculate the whole virtual screen size ... 
  XRRSetScreenSize(w->dis, m->root, dx, dy, out->mm_width, out->mm_height);
  XRRFreeOutputInfo(out);

  XRRFreeCrtcInfo(crtc);
  XRRFreeScreenResources(scr);
  
  if(s!= Success) {
    error.simple("OSdisplay::changeRes: can't change to requested resolution");
    return false;
  }

  // TESTS


  /*
  sleep(3);
  XRRPanning p= {0};                            // try DISABLE panning
  scr= XRRGetScreenResources(w->dis, m->root);
  XRRSetPanning(w->dis, scr, m->crtcID, &p);
  XRRFreeScreenResources(scr);
  */
  
  // END TESTS
  
  m->progRes.dx= dx;
  m->progRes.dy= dy;
  m->progRes.resID[0]= r->resID[0];
  m->progRes.freq[0]= f;

  w->bpp= bpp;
  w->dx= dx;
  w->dy= dy;
  w->freq= r->freq[f];

  m->inOriginal= false;
  
  return true;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  
  /// search in data for requested resolution (hope populate() was called first)
  OSIResolution *r= getResolution(dx, dy, m);
  
  if(r == null) {
    error.simple("OSI:changeRes: Can't find requested resolution size");
    return false;
  }
  if(chatty) printf("requested resolution change: %dx%d %dHz\n", r->dx, r->dy, freq);
  
  short f= 0;
  //uint reqID;
  //Status s;
  if(freq) f= getFreq(freq, r);
  
  if(f== -1) {
    error.simple("OSI:changeRes: Requested frequency not found");
    return false;
  }
  
  // THERE HAS TO BE ALL THE CHECKS IN HERE, cuz changing 10 times the resolution
  // in 1 second (wich can freaqing happen, due to some crazy request), might
  // blow up your monitor
  
  /// check if the ORIGINAL RESOLUTION was requested
  if(!freq)                               /// frequency might be ignored (0)
    f= m->original.freq[0];
  
  if((m->original.dx== dx) && (m->original.dy== dy) && (m->original.freq[0]== f)) {
    if(m->inOriginal) {                   // already in original res?
      if(chatty) printf("changeRes: IGNORE: already in original resolution\n");
      return true;
    }
    
    restoreRes(w, m);
    return true;
  } /// if original resolution is requested
  
  /// check if changing TO PROGRAM RESOLUTION (from original probly, an alt-tab or something)
  if(!freq)                               /// frequency might be ignored (0)
    f= m->progRes.freq[0];
  
  if((m->progRes.dx== dx) && (m->progRes.dy== dy) && (m->progRes.freq[0]== f)) {
    if(m->inOriginal) {
      
      // IFDEF OS_MAC <<<
      /// get resolution with specified ID
      CFArrayRef modes= CGDisplayCopyAllDisplayModes(m->id, NULL);
      const void *resid= CFArrayGetValueAtIndex(modes, r->id[f]);
      
      /// change resolution <<<<
      if(CGDisplaySetDisplayMode(m->id, (CGDisplayModeRef)resid, null) != kCGErrorSuccess) {
        CFRelease(modes);
        error.simple("OSdisplay::changeRes: can't change to requested resolution");
        return false;
      }
      
      CFRelease(modes);
      // ENDIF /// OS_MAC <<<
      
      w->bpp= bpp;
      w->dx= dx;
      w->dy= dy;
      w->freq= r->freq[f];
      m->inOriginal= false;
      
      return true;
    } /// if in original resolution
    if(chatty) printf("changeRes: IGNORE: already in program resolution\n");
    return true; /// if already in program resolution, just return
  } /// if progRes is requested
  
  
  /// it's not origRes requested, nor progRes requested, it's a NEW resolution
  /// if frequency is not supplied
  if(!freq) {
    f= getFreq(60, r);        // try 60hz
    if(f== -1)
      f= getFreq(59, r);      // try 59hz
    if(f== -1)
      f= 0;                   // just get the first freq in list (there has to be 1)
  }
  
  // IFDEF OS_MAC <<<
  /// get resolution with specified ID
  CFArrayRef modes= CGDisplayCopyAllDisplayModes(m->id, NULL);
  const void *resid= CFArrayGetValueAtIndex(modes, r->id[f]);
  
  /// change resolution <<<<
  if(CGDisplaySetDisplayMode(m->id, (CGDisplayModeRef)resid, null) != kCGErrorSuccess) {
    CFRelease(modes);
    error.simple("OSdisplay::changeRes: can't change to requested resolution");
    return false;
  }
  
  CFRelease(modes);
  // ENDIF /// OS_MAC <<<

  m->progRes.dx= dx;
  m->progRes.dy= dy;
  m->progRes.id[0]= r->id[0]; // more OS specific, but doable
  m->progRes.freq[0]= f;
  
  w->bpp= bpp;
  w->dx= dx;
  w->dy= dy;
  w->freq= r->freq[f];
  
  m->inOriginal= false;
  
  return true;
  
  
  
  #endif /// OS_MAC
}

// 
void OSIDisplay::restorePrimary() {
  restoreRes(osi.primWin, osi.primWin->monitor);
}


void OSIDisplay::restoreAllRes() {
  for(short a= 0; a< nrMonitors; a++)
    restoreRes(osi.primWin, &monitor[a]);
}

///----------------------------------------------------------------------///
// --------------->>>>>>>>>>>>> RESTORERES <<<<<<<<<<<<<<---------------- //
///----------------------------------------------------------------------///

void OSIDisplay::restoreRes(OSIWindow *w, OSIMonitor *m) {
  bool chatty= true;
  
  #ifdef OS_WIN
  if(m->inOriginal)
    return;

  ChangeDisplaySettingsEx(m->id, NULL, NULL, NULL, NULL);
  ShowCursor(TRUE);
  m->inOriginal= true;            // set 'in original resolution' FLAG
  #endif

  #ifdef OS_LINUX
  if(m->inOriginal)
    return;
  XRRScreenResources *scr= XRRGetScreenResources(w->dis, m->root);
  XRRCrtcInfo *crtc= XRRGetCrtcInfo(w->dis, scr, m->crtcID);
  
  XRRModeInfo *mode= this->getMode(scr, m->original.resID[0]);
  printf("RESTORE RESOLUTION\n");
  printf("crtc: x[%d] y[%d] dx[%d] dy[%d] mode[%d] noutputs[%d] output1[%d]\n", crtc->x, crtc->y, crtc->width, crtc->height, crtc->mode, crtc->noutput, crtc->outputs[0]);
  printf("req:  x[%d] y[%d] dx[%d] dy[%d] mode[%d]\n", m->x0, m->y0, mode->width, mode->height, mode->id);
  
  // TESTS
  
  XRRPanning p= {0};                            // try DISABLE panning
  scr= XRRGetScreenResources(w->dis, m->root);
  XRRSetPanning(w->dis, scr, m->crtcID, &p);
  XRRFreeScreenResources(scr);
  sleep(3);
  
  XRROutputInfo *out= XRRGetOutputInfo(w->dis, scr, crtc->outputs[0]);
  XRRSetScreenSize(w->dis, m->root, m->original.dx, m->original.dy, out->mm_width, out->mm_height);
  XRRFreeOutputInfo(out);
  
  // END TESTS
  
  if(XRRSetCrtcConfig(w->dis, scr,
                      m->crtcID,
                      CurrentTime,
                      m->x0, m->y0,
                      m->original.resID[0],
                      m->original.rotation,
                      crtc->outputs,
                      crtc->noutput)!= Success) {
    error.simple("OSIDisplay::restoreRes: cannot change back to original resolution.");
    
    XRRFreeCrtcInfo(crtc);
    XRRFreeScreenResources(scr);

    return;
  }
  

  XRRFreeCrtcInfo(crtc);
  XRRFreeScreenResources(scr);
  
  m->inOriginal= true;
  
  return;
  #endif /// OS_LINUX

  #ifdef OS_MAC
  if(m->inOriginal)
    return;
   
  if(chatty) printf("RESTORE RESOLUTION\n");
  
  // IFDEF OS_MAC <<<
  /// this restores resolutions for ALL monitors
  CGRestorePermanentDisplayConfiguration();
  
  /// set inOriginal flag for all monitors, as they all get in original resolution
  for(short a= 0; a< MAX_WINDOWS; a++)
    if(osi.win[a].isCreated) {
      osi.win[a].monitor->inOriginal= true;
      CGDisplayShowCursor(m->id);   /// show mouse cursor? this need a little bit of further thinking      
    }

  // PER MONITOR RESTORE... needs more work (in display.populate() ). probly keep the CGDisplayModeRef of the original resolution somewhere
  /*
  /// change resolution <<<<
  if(CGDisplaySetDisplayMode(m->id, (CGDisplayModeRef)resid, null) != kCGErrorSuccess)
    error.simple("OSIDisplay::restoreRes: cannot change back to original resolution."); /// just an error, i guess, let the program try to continue functioning

  CGDisplayShowCursor(m->id);   /// show mouse cursor? this need a little bit of further thinking
  m->inOriginal= true;
  */
  // ENDIF /// OS_MAC <<<
  
  #endif /// OS_MAC
} // OSIDisplay::restoreRes





///---------------------------------------------------------------------///
// -------------->>>>>>>>>>>>>>> POPULATE <<<<<<<<<<<<<<<--------------- //
///---------------------------------------------------------------------///
void OSIDisplay::populate(OSInteraction *t) {
  bool chatty= true;   // this is used for debug: prints stuff it finds to terminal
  delData();
  #ifdef OS_WIN

  int a, b, n, tx, ty;//, m;
  OSIResolution *p;
  string s;
  
  bool found;
  DISPLAY_DEVICE dd= { 0 };
  DEVMODE dm= { 0 };

  dm.dmSize= sizeof(dm);
  dd.cb= sizeof(dd);

/// virtual screen size
  vx0= GetSystemMetrics(SM_XVIRTUALSCREEN);
  vy0= GetSystemMetrics(SM_YVIRTUALSCREEN);
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
    error.simple("OSIDisplay::populate: can't find any displays");
    return;
  }

  monitor= new OSIMonitor[nrMonitors];

// loop thru all displays < START <--------------------------------------------------------
  for(short d= 0; EnumDisplayDevices(null, d, &dd, null); d++) {    /// for each display
    if(!(dd.StateFlags& DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))        /// should be attached to desktop ...
      continue;

    monitor[d].id= dd.DeviceName;
    monitor[d].name= dd.DeviceString;

    if(chatty) printf("%s (%s)", monitor[d].id, monitor[d].name);

    if(dd.StateFlags& DISPLAY_DEVICE_PRIMARY_DEVICE) {
      monitor[d].primary= true;
      primary= &monitor[d];
      if(chatty) printf(" primary");
    }
    printf("\n");

/// original monitor settings
    if(EnumDisplaySettings(monitor[d].id, ENUM_CURRENT_SETTINGS, &dm)) {
      monitor[d].original.dx= dm.dmPelsWidth;
      monitor[d].original.dy= dm.dmPelsHeight;
      monitor[d].original.freq[0]= (short)dm.dmDisplayFrequency;
    }

/// position on VIRTUAL DESKTOP		<--- if there's a need to find this position after a resolution change, a new func might be needed
/// to get these vars & some rethinking
    monitor[d].x0= dm.dmPosition.x;
    monitor[d].y0= dm.dmPosition.y;

    if(EnumDisplayDevices(monitor[d].id, 0, &dd, null)) {
      monitor[d].monitorID= dd.DeviceName;                  // currently i cant find any use for this
      monitor[d].monitorName= dd.DeviceString;
    }

    if(chatty) printf("%s (%s)\n", monitor[d].monitorID, monitor[d].monitorName);

/// windows vomit array size
    for(a= 0, n= 0; EnumDisplaySettings(monitor[d].id, a, &dm) != 0; a++)
      n++;

    if(!n) {
      error.simple("OSIDisplay::populate: can't find any display resolutions");
      continue;
    }

    OSIResolution *tmp= new OSIResolution[n+ 1]; /// +1 a safety, this list will be wiped anyways


/// compute how many resolutions the monitor can handle
    monitor[d].nrRes= 0;
    for(a= 0; EnumDisplaySettings(monitor[d].id, a, &dm) != 0; a++) {
      if(dm.dmBitsPerPel< 16) continue;     /// res should support at least 16 bpp

        found= false;

        for(b= 0; b< monitor[d].nrRes; b++)
          if( (dm.dmPelsWidth == tmp[b].dx) && (dm.dmPelsHeight == tmp[b].dy) )		/// already in the list
            found= true;

        if(!found){                         /// if not found in tmp list it means it's a new resolution
          tmp[monitor[d].nrRes].dx= dm.dmPelsWidth;
          tmp[monitor[d].nrRes].dy= dm.dmPelsHeight;
          monitor[d].nrRes++;
        }
      }
      n= monitor[d].nrRes;                  /// n= name shortcut

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
    monitor[d].res= new OSIResolution[n];
    p= monitor[d].res;

    for(a= 0; a< n; a++) {
      p[a].dx= tmp[a].dx;
      p[a].dy= tmp[a].dy;
    }

    short tf[256];
    for(a= 0; a< 256; a++)
      tf[a]= 0;

/// compute the number of frequencies per resolution (res[].nrFreq)
    for(a= 0; EnumDisplaySettings(monitor[d].id, a, &dm) != 0; a++) {
      if(dm.dmDisplayFlags== DM_INTERLACED)	continue;               /// ignore interlaced resolutions
      if(dm.dmDefaultSource) continue;                              /// have no clue what this is
      if(dm.dmBitsPerPel != 32) continue;                           /// only 32bpp

      for(b= 0; b< n; b++)                                          /// search for resolution & increase it's nr of frequencies
        if((dm.dmPelsWidth == p[b].dx) && (dm.dmPelsHeight == p[b].dy))
          p[b].nrFreq++;
    }

/// alloc the list per resolution (got the max numbers)
    for(a= 0; a< n; a++) {
      p[a].freq= new short[p[a].nrFreq];
      for(b= 0; b< p[a].nrFreq; b++)
        p[a].freq[b]= 0;				/// fill all frequencies with 0, this is used in next part
    }

/// filling all frequencies with only a pass thru all windows big mess table
/// the less times messing with win table stuff, the quicker (else can get really slow)
    for(a= 0; EnumDisplaySettings(monitor[d].id, a, &dm) != 0; a++) {
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
        printf("%dx%d ", p[a].dx, monitor[d].res[a].dy , monitor[d].res[a].nrFreq);
        for(b= 0; b< p[a].nrFreq; b++)
          printf("%d ", p[a].freq[b]);
        printf("\n");
      }

    delete[] tmp;
  } /// displays loop

// IT'S OVERRRRR ... 


  #endif /// OS_WIN


  #ifdef OS_LINUX
//  XRRScreenSize *xrrs; // OLD CODE

  primary monitor!!!
  
  short a, b, c, d, e;              /// all used in loops...
  OSIResolution *p;
  
  // NEW CODE MUST BE MADE
  // XRRScreenResources *scr;       // moved to display class as a member
  XRROutputInfo *out, *out2;
  XRRCrtcInfo *crtc;
  XRRScreenResources *scr;
  
  scr= XRRGetScreenResourcesCurrent(t->primWin->dis, DefaultRootWindow(t->primWin->dis));
  if(chatty) printf("Screen info: outputs= %d; modes= %d; crtcs= %d\n",scr->noutput, scr->nmode, scr->ncrtc);
  
/// virtual desktop size
  vx0= 0; vy0= 0;
  vdx= scr->modes[0].width;
  vdy= scr->modes[0].height;
  if(chatty) printf("virtual desktop size [%dx%d]\n", vdx, vdy);

/// find the number of connected monitors  
  for(a= 0; a< scr->noutput; a++) {
     out= XRRGetOutputInfo(t->primWin->dis, scr, scr->outputs[a]);
     if(chatty) printf("output %d: %s %s (crtc%d)\n", a, out->name, (out->connection== RR_Connected)? "active": "no connection", out->crtc);
     
     if(out->connection== RR_Connected)
       nrMonitors++;
     XRRFreeOutputInfo(out);
  }

/// create & start populating monitor structure
  monitor= new OSIMonitor[nrMonitors];

  for(a= 0, b= 0; a< scr->noutput; a++) { /// for each output (empty outputs will be ignored)
    out= XRRGetOutputInfo(t->primWin->dis, scr, scr->outputs[a]);
    /// if there is nothing connected to this output, skip it
    if(out->connection!= RR_Connected) {
      XRRFreeOutputInfo(out);
      continue;
    } 
    
    /// crtc that handles this output (output that IS connected to a monitor)
    crtc= XRRGetCrtcInfo(t->primWin->dis, scr, out->crtc);
    /// can ge usefull data from a crtc
    if(chatty) {
      printf("out%d: is on crtc%d\n", scr->outputs[a], out->crtc);
      printf("crtc%lu: has %d outputs:", out->crtc, crtc->noutput);
      for(short z= 0; z< crtc->noutput; z++)
        printf(" %lu", crtc->outputs[z]);
      printf("\n");
      
    }
    monitor[b].screen= DefaultScreen(t->primWin->dis);;  // can it be possible anymore to be a different value???
    monitor[b].root= RootWindow(t->primWin->dis, monitor[b].screen);
    monitor[b].outID= scr->outputs[a];
    monitor[b].crtcID= out->crtc;
    monitor[b].x0= crtc->x;
    monitor[b].y0= crtc->y;
    monitor[b].original.dx= crtc->width;
    monitor[b].original.dy= crtc->height;
    monitor[b].original.resID[0]= crtc->mode; /// this is the only use
    monitor[b].original.rotation= crtc->rotation;
    
    
    if(chatty) printf("monitor position %d,%d crtc[%d] out[%d]\n", monitor[b].x0, monitor[b].y0, monitor[b].crtcID, monitor[b].outID);
    

// ***********************************************************
// MUST TEST IF RANDR AUTOMATICALLY LOWERS THE POSSIBLE RESOLUTIONS
// FOR MONITORS SET ON DUPLICATE. CHANSES ARE, IT DOESN'T
// ***********************************************************
    

/// find the number of resolutions the monitor supports (this is not easy cuz of duplicate monitors)
    
    /// if there are multiple monitors on same crtc, they are set on DUPLICATE (mirror or watever the OS names them).
    /// each resolution MUST be avaible on all monitors in this case
    /// ex: if monitor1 can't support a resolution that monitor2 can, that resolution must go from both lists
    bool found, found2;
    RRMode *tmp= new RRMode[out->nmode];      // temporary array, will be populated with resolutions all monitors support
    short tmpSize= 0;
    
    for(c= 0; c< out->nmode; c++) {     // for each res (resolution==mode)
      found= true;                            /// start with true(res is found), just mark it as false on the way
      
      if(crtc->noutput> 1)
        for(d= 0; d< crtc->noutput; d++) {    // for each out that is duplicating
          if(monitor[b].outID == crtc->outputs[d]) continue; /// don't test against itself

          out2= XRRGetOutputInfo(t->primWin->dis, scr, crtc->outputs[d]);
        
          found2= false;                      /// start with false(res not found in out2), mark as true if found
        
          for(e= 0; e< out2->nmode; e++)      // for each mode in out2
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
    monitor[b].res= new OSIResolution[tmp2Size];
    
    for(c= 0; c< monitor[b].nrRes; c++) {       // for each res the monitor supports
      /// find the id in all the screen modes (hopefully this is the place all modes are dumped)
      i= getMode(scr, tmp2[c]);
      
      /// finally populate the rest of the stuff
      monitor[b].res[c].dx= i->width;
      monitor[b].res[c].dy= i->height;
      monitor[b].res[c].rotation= RR_Rotate_0;
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
      monitor[b].res[c].resID= new RRMode[e];
      /// populate both frequencies & resolutionIDs
      e= 0;
      for(d= 0; d< tmpSize; d++) {
        j= getMode(scr, tmp[d]);
        if((i->width== j->width) && (i->height== j->height)) {
          monitor[b].res[c].freq[e]= (j->dotClock? (j->dotClock/ (j->hTotal* j->vTotal)): 0);
          monitor[b].res[c].resID[e++]= j->id;
        }
      } 
    } /// for each resolution
    
    delete[] tmp;
    delete[] tmp2;
    
    b++;
    XRRFreeCrtcInfo(crtc);
    XRRFreeOutputInfo(out);
  } /// for each output
  
  XRRFreeScreenResources(scr);
  
  if(chatty)
    for(a= 0; a< nrMonitors; a++)
      for(b= 0; b< monitor[a].nrRes; b++) {
        printf("[%dx%d]", monitor[a].res[b].dx, monitor[a].res[b].dy);
        for(c= 0; c< monitor[a].res[b].nrFreq; c++) 
          printf(" %d[id%lu]", monitor[a].res[b].freq[c], monitor[a].res[b].resID[c]);
        printf("\n");
      }
  
  // IT'S OVERRRRR ... 
  
  // OLD CODE FROM HERE
/*
  // Display should be on the OSIMonitor struct
  // and they must be 'found', i think. further research needed


/// determine the number of monitors attached
  nrMonitors= XScreenCount(t->primWin->display);
  printf("test %d\n", (XScreenOfDisplay(t->primWin->display, 1))->width);
  
  if(chatty) printf("found %d monitors\n", nrMonitors);
  
/// populate monitors structure
  monitor= new OSIMonitor[nrMonitors];

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
    monitor[a].res= new OSIResolution[monitor[a].nrRes];

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

  int a, b, c, d, tx, ty;
  CGRect r;
  OSIResolution *p;
  string s;
  uint32_t n, n2;
  const void *resid;
  uint32_t flags;
  bool found;
  char buf[512];
  
  /// find the number of displays on the system
  CGDirectDisplayID *dis= new CGDirectDisplayID[MAX_WINDOWS];
  
  CGGetActiveDisplayList(MAX_WINDOWS, dis, &n);
  
  if(!n) {
    error.simple("OSIDisplay::populate: can't find any displays");
    return;
  }
  
  nrMonitors= (short)n;
  
  monitor= new OSIMonitor[nrMonitors];
  
  if(chatty) printf("found %d monitors", nrMonitors);
  
  
// loop thru all displays < START <--------------------------------------------------------
  
  for(d= 0; d< nrMonitors; d++) {                     /// for each monitor
    
    /// monitor id(mac) & name
    monitor[d].id= dis[d];
    
    if(!cocoa.displayName(dis[d], &monitor[d].name))
      monitor[d].name= "Unknown Monitor";
    
    /// is it the primary monitor?
    if(CGDisplayIsMain(dis[d])) {
      monitor[d].primary= true;
      primary= &monitor[d];
    }
    
    /// monitor position on virtual desktop
    r= CGDisplayBounds(dis[d]);
    monitor[d].x0= r.origin.x;
    monitor[d].y0= r.origin.y;
   
    
    /// original monitor settings
    monitor[d].original.dx= (int)CGDisplayPixelsWide(dis[d]);
    monitor[d].original.dy= (int)CGDisplayPixelsHigh(dis[d]);
    
    //monitor[d].original.freq[0]= (short)dm.dmDisplayFrequency;
      
    if(chatty) printf("monitor %d (%s):\n", d, monitor[d].name.d);
    if(chatty) printf("  id[%d] position[%dx%d] original res[%dx%d]\n", monitor[d].id, monitor[d].x0, monitor[d].y0, monitor[d].original.dx, monitor[d].original.dy);
    
// MAC 10.5 required
    // double CGDisplayRotation(dis[d]); this is macOS 10.5 onwards... <<<<<<<<<<<<<<<<<<<<<<<<<
    // ROTATION???
    
    
    
// MAC 10.6 required
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
        CFStringGetCString(st, buf, 512, CFStringGetSystemEncoding());
      
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
    OSIResolution *tmp= new OSIResolution[n2+ 1]; /// +1 a safety, this list will be wiped anyways
    monitor[d].nrRes= 0;
    s= IO32BitDirectPixels;          /// 32bit... apple stores a 32bit resolution type as a string...
    
    for(a= 0; a< n2; a++) {          /// for each vommit element
      resid= CFArrayGetValueAtIndex(modes, a);
      
      /// interlaced modes are out of the picture
      flags= CGDisplayModeGetIOFlags((CGDisplayModeRef)resid);
      if(flags& kDisplayModeInterlacedFlag) continue;
      
      /// only 32bit resolutions ... SUBJECT OF INTESIVE THINKING... remove 16 bits?! <<<<<<<<<<<<<
      CFStringRef st= CGDisplayModeCopyPixelEncoding((CGDisplayModeRef)resid);
      CFStringGetCString(st, buf, 512, CFStringGetSystemEncoding());
      
      if(s!= buf) {
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
    monitor[d].res= new OSIResolution[n];
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
      CFStringGetCString(st, buf, 512, CFStringGetSystemEncoding());
      if(s!= buf) {
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
      p[a].id= new uint[p[a].nrFreq];
      for(b= 0; b< p[a].nrFreq; b++)
        p[a].id[b]= 0;          /// fill IDs with 0, this is used in the next part (hopefully they won't make a 0 id)
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
      CFStringGetCString(st, buf, 512, CFStringGetSystemEncoding());

      if(s!= buf) {
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
            if(!p[b].id[c]) {
              p[b].freq[c]= (short)freq;
              p[b].id[c]= idt;
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
            uint idt= p[a].id[b];
            tx= p[a].freq[b];
            p[a].freq[b]= p[a].freq[c];
            p[a].id[b]= p[a].id[c];
            p[a].freq[c]= (short)tx;
            p[a].id[c]= idt;
          }
    
    if(chatty)
      for(a= 0; a< n; a++) {
        printf("%dx%d ", p[a].dx, p[a].dy);
        for(b= 0; b< p[a].nrFreq; b++)
          printf("%d[ID%u] ", p[a].freq[b], p[a].id[b]);
        printf("\n");
      }
    
    CFRelease(modes);
    delete[] tmp;
  } /// displays loop
  
  /// virtual desktop size
  vx0= primary->x0;
  vy0= primary->y0;
  vdx= primary->original.dx;
  vdy= primary->original.dy;
  for(a= 0; a< nrMonitors; a++) {
    if(monitor[a].x0< vx0)                              /// <<
      vx0= monitor[a].x0;
    if(monitor[a].y0< vy0)                              /// ^^
      vy0= monitor[a].y0;
    if(monitor[a].x0+ monitor[a].original.dx- 1> vdx)   /// >>
      vdx= monitor[a].x0+ monitor[a].original.dx- 1;
    if(monitor[a].y0+ monitor[a].original.dy- 1> vdy)   /// vv
      vdy= monitor[a].y0+ monitor[a].original.dy- 1;
  }
  
  delete[] dis;
  
// IT'S OVERRRRR ... 
  #endif /// OS_MAC
}

// this is good only for windows... it might just be moeved in populate()
void OSIDisplay::getMonitorPos(OSIMonitor *m) {
  #ifdef OS_WIN
  DEVMODE dm= { 0 };
  dm.dmSize= sizeof(dm);

  if(EnumDisplaySettings(m->id, ENUM_CURRENT_SETTINGS, &dm)) {
    m->x0= dm.dmPosition.x;     /// position on VIRTUAL DESKTOP  <--- if there's a need to find this position after a resolution change, a new func might be needed to get these vars & some rethinking
    m->y0= dm.dmPosition.y;     /// position on VIRTUAL DESKTOP
  }
  #endif /// OS_WIN

  #ifdef OS_LINUX
  // function useless in LINUX
  #endif
  
  #ifdef OS_MAC
  // function useless in MAC
  #endif /// OS_MAC

}


#ifdef OS_LINUX
XRRModeInfo *OSIDisplay::getMode(XRRScreenResources* s, RRMode id) {
  for(short a= 0; a< s->nmode; a++)
    if(s->modes[a].id== id)
      return &s->modes[a];
  return null;
}
#endif /// OS_LINUX



OSIResolution *OSIDisplay::getResolution(int dx, int dy, OSIMonitor *gr) {
  for(short a= 0; a< gr->nrRes; a++) {
    OSIResolution *p= &gr->res[a];
    if((dx == p->dx) && (dy == p->dy))
      return p;
  }

  return null;
}


short OSIDisplay::getFreq(short freq, OSIResolution *r) {
  for(short a= 0; a< r->nrFreq; a++)
    if(freq == r->freq[a])
      return a;

  return -1;
}



