#pragma once

#ifdef OS_MAC

// this is a wrapper for Objective-C functions

class osiCocoa {
  
public:
  
  void setProgramPath();                /// under mac is not certain that it is set, it seems
  bool createWindow(osiWindow *w, const char *iconFile);
  bool createSplashWindow(osiWindow *w, uint8_t *bitmap, int dx, int dy, int bpp, int bpc);

  bool changeRes(osiWindow *w, osiMonitor *m, int32_t dx, int32_t dy, int8_t bpp, int16_t freq= 0);
  bool displayName(uint32_t id, str8 *out);    // returns in out the name of the display
  bool displayGPU(uint32_t id, str8 *out);    /// returns the gpu string (the only info i could find about the gpu)
    
  void setIcon(uint8_t *bitmap, int dx, int dy, int bpp, int bpc);
  str8 getCmdLine();
  void sleep(int ms);                 /// cocoa sleep function (in miliseconds)
  int passedTime(void);               /// this might be scraped
  void setPastebin(uint8_t *in_text);   /// pastebin=clipboard - copy/paste operations
  void getPastebin(uint8_t **out_text); /// pastebin=clipboard - copy/paste operations

  void getWindowSize(osiWindow *w, int32_t *dx, int32_t *dy);
  void setWindowSize(osiWindow *w, int dx, int dy);
  void setWindowPos(osiWindow *w, int x, int y);
  void setWindowName(osiWindow *w, const char *name);
  void setWindowHidden(osiWindow *w);
  void setWindowShown(osiWindow *w);
  void delWindow(osiWindow *w);
  
  // oGL related

  bool createPixelFormat(osiRenderer *, uint32_t oglDisplayMask);
  bool createContext(osiRenderer *r, uint32_t oglDisplayMask);

  void setRendererVertSync(osiRenderer *r, bool vertSync= true);
  void delContext(void *);          // dealloc memory
  void delPixelFormat(void *);      // dealloc memory
  void delNSImage(void *);          // dealloc memory
  void swapBuffers(osiWindow *w);
  bool makeCurrent(osiRenderer *, osiWindow *);
  
  osiCocoa();
  ~osiCocoa();
};

extern osiCocoa cocoa;

#endif /// OS_MAC

