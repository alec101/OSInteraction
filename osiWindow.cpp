#include "osinteraction.h"



// -----------============= WINDOW CLASS =============------------------
///=====================================================================


#ifdef OS_WIN
MSG osiWindow::_msg= {0};
#endif

#ifdef OS_LINUX
Display *osinteraction::_dis= null;
Display *osiWindow::_dis= null;
#endif


osiWindow::osiWindow() {
  isCreated= false;
  x0= y0= dx= dy= 0;
  freq= bpp= 0;
  _isSplashWindow= false;
  
  monitor= null;
  glr= null;
  
  #ifdef OS_WIN
  _hDC= NULL;
  _hInstance= NULL;
  _hWnd= NULL;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  _root= 0;              /// root window (desktop/server main window/ watever)
  _win= 0;               /// window 'handle' or watever (per monitor)
  _vi= null;
  _isMapped= false;
  _img= null;            /// window background / mainly used on splash windows
  _gc= null;             /// graphics context used when drawing a window background
  _imgData= null;        /// background image pixel data
  #endif /// OS_LINUX

  #ifdef OS_MAC
  _win= null;
  _view= null;
  #endif /// OS_MAC
}


osiWindow::~osiWindow() {
  #ifdef OS_WIN
  delData();            // it seems system already destroys windows, and this causes segmentation error
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  //  delData();        // it seems system already destroys windows, and this causes segmentation error  
  if(_iconData) delete[] _iconData;
  if(_imgData)  delete[] _imgData;
  if(_img)      XFree(_img);
  if(_gc)       XFreeGC(_dis, _gc);
  #endif
  
  #ifdef OS_MAC
  // nothing to do atm
  #endif /// OS_MAC
}


void osiWindow::delData() {
  #ifdef OS_WIN
  if(_hDC) {
    ReleaseDC(_hWnd, _hDC);
    _hDC= NULL;
  }

  if(_hWnd) {
    DestroyWindow(_hWnd);
    _hWnd= NULL;
  }

  if(name.len) {
    UnregisterClass(name, _hInstance);   // << in case class name will differ in future
    _hInstance= NULL;
  }
  // do not return;
  #endif /// OS_WIN

  #ifdef OS_LINUX
  if(isCreated) {
    osi.glMakeCurrent(null);
    //osi.glDestroyRenderer(this); // THIS NEEDS TO BE PUT WITH glRenderer STRUCT WIP
    //glXDestroyContext(display, glRenderer);
    
    /// window background image + grContext free
    if(_imgData) {delete[] _imgData;   _imgData= null; }
    if(_img)     { XFree(_img);        _img= null; }
    if(_gc)      { XFreeGC(_dis, _gc); _gc= null; }
    
    if(mode == 2)
      osi.display.restoreRes(monitor);
    XUnmapWindow(_dis, _win);
    XDestroyWindow(_dis, _win);
    this->_isMapped= false;
  }
  // do not return
  #endif

  #ifdef OS_MAC
  _win= null;
  _view= null;

  // do not return
  #endif


  this->isCreated= false;
  this->mode= 0;
  this->bpp= 0;
  this->dx= 0;
  this->dy= 0;
  this->freq= 0;
  this->glr= null;
  this->monitor= null;
  this->name.delData();
  this->x0= 0;
  this->y0= 0;
}




#ifdef OS_LINUX

void osiWindow::_setWMprop(cchar *wmID, cchar *wmProp, uint32 val1, uint32 val2) {
  Atom wm= XInternAtom(_dis, wmID, False);
  Atom prop= XInternAtom(_dis, wmProp, False);
  
  /// note, the long is 64bit in linux64, and XChangeProperty actually wants 64bit ints (i think is a stoopid bug)
  ulong data[5]= {val1, val2, 0, 0, 0};

  XChangeProperty(_dis, _win, wm, prop, 32, PropModeReplace, (cuchar*)data, 2);
}


void osiWindow::_setWMstate(uint32 val, cchar *prop1, cchar *prop2) {
  XEvent xev;
  Str::memclr(&xev, sizeof(xev));
/// set vals
  xev.type= ClientMessage;
  xev.xclient.window= _win;
  xev.xclient.message_type= XInternAtom(_dis, "_NET_WM_STATE", False);
  xev.xclient.format= 32;
  xev.xclient.data.l[0]= val;
  xev.xclient.data.l[1]= XInternAtom(_dis, prop1, False);
  if(prop2)
    xev.xclient.data.l[2]= XInternAtom(_dis, prop2, False);
  xev.xclient.data.l[3]= 1; // 0= oldApp/ignore, 1= normal app, 2= pagers/other clients
/// send the message to root
  XSendEvent(_dis, _root, False, SubstructureRedirectMask| SubstructureNotifyMask, &xev);
}

#endif /// OS_LINUX



// funcs


bool osiWindow::setIcon(cchar *file) {
  if(!file) return false;
  
  /// s will hold the file extension to be loaded, in lowercase
  Str8 s("   ");
  int32 len= Str::strlen8(file);
  
  for(char a= 0; a< 3; a++)
    s.d[a]= file[len- 3+ a];

  s.lower();

  PNG png;
  TGA tga;

  uint8 *bitmap= null;
  int32 dx, dy;
  int8 depth;
  
  /// file extension== PNG
  if(s== "png") {
    png.load(file);
    
    if(png.err) return false;                                   /// error loading
    if(png.type!= IMG_RGB && png.type!= IMG_RGBA) return false; /// RGBA & RGB only

    depth= png.bpp/ png.bpc;
    dx= png.dx;
    dy= png.dy;
    bitmap= (uint8 *)png.bitmap;
    
  /// file extension== TGA
  } else if(s== "tga") {
    tga.load(file);
    
    if(tga.err) return false;                                   /// error loading
    if(tga.type!= IMG_RGB && tga.type!= IMG_RGBA) return false; /// RGBA & RGB only

    depth= tga.bpp/ tga.bpc;
    dx= tga.dx;
    dy= tga.dy;
    bitmap= (uint8 *)tga.bitmap;
    
  /// file extension== UNKNOWN
  } else return false;

  #ifdef OS_WIN
  /// RGB(A)->BGR(A) conversion
  for(int32 a= 0; a< dy; a++)
    for(int32 b= 0; b< dx; b++) {
      uint8 t= bitmap[a* dx* depth+ b* depth+ 0];
      bitmap[a* dx* depth+ b* depth+ 0]= bitmap[a* dx* depth+ b* depth+ 2];
      bitmap[a* dx* depth+ b* depth+ 2]= t;
    }

  HICON hIcon= CreateIcon(null, dx, dy, depth, 8, null, bitmap);
  SendMessage(primWin->_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

  // small icon???
  //hIconSm = LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
  //SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
  #endif

  #ifdef OS_LINUX
  
  /// there's a bug (imho) tied to the fact that 'long' in linux64 is 64bit; they ask for 32bit BGRA array of pixels
  /// but it is not working unless the array is 64bit on linux64
  /// this code hopefully works on both 32 and 64 bit Linux
  if(_iconData) delete[] _iconData;
  _iconData= new unsigned long[(dx* dy)+ 2];
  _iconData[0]= dx;                /// first arr index is image width
  _iconData[1]= dy;                /// second arr index is image height
  
  uint8 *dst= (uint8 *)_iconData; dst+= sizeof(unsigned long)* 2; /// from icon[2] onwards, is the actual image pixel data in BGRA 32bit
  uint8 *src= bitmap;
  
  /// convert from RGB(A) to BGRA
  for(int64 a= 0, n= dx* dy; a< n; a++, dst+= sizeof(unsigned long), src+= depth) {
    dst[0]= src[2];
    dst[1]= src[1];
    dst[2]= src[0];
    dst[3]= (depth== 3? 255: src[3]);
  }
  
  int length= 2+ dx* dy;      /// image size + 2 for width+ height
  
  Atom net_wm_icon = XInternAtom(_dis, "_NET_WM_ICON", False);
  Atom cardinal = XInternAtom(_dis, "CARDINAL", False);
  // PropModePrepend PropModeReplace PropModeAppend
  XChangeProperty(_dis, _win, net_wm_icon, cardinal, 32, PropModeReplace, (cuchar *)_iconData, length); /// http://standards.freedesktop.org/wm-spec/wm-spec-latest.html#idm140200472568384
  //XSync(_dis, False);
  
  
  // METHOD2:  MIGHT WORK IN THE FUTURE
  /* unfortunately, XSetStandardPropreties uses (i belive) _NET_WM_ICON, which wants int64's, and the bitmap normal int32 ARGB
   * therefore, it's bugged
  Pixmap iconPixmap= XCreatePixmapFromBitmapData(_dis, w->_root, ((char *)icon)+ 8, dx, dy, 0xFFFFFFFF, 0, 32);
  if(!iconPixmap) error.simple("XCreatePixmapFromBitmapData - error creating pixmap\n");
  XSetStandardProperties(_dis, w->_win, 0, 0, iconPixmap, null, 0, null);
  XSync(_dis, False);
  */
  
  
  /* getting icon sizes seem NOT TO WORK
  int nr= 0;
  XIconSize *sizes= XAllocIconSize();
  XGetIconSizes(_dis, primWin->_win, &sizes, &nr); // <<< nr is still 0
  XFree(sizes);
  */
  
  
  #endif

  #ifdef OS_MAC
  makeme
  #endif
  return true;
}


void osiWindow::setName(cchar *s) {
  if(!s) return;
  name= s;
  #ifdef OS_WIN
  makeme
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  XStoreName(_dis, _win, name);
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC
}


void osiWindow::show() {
  #ifdef OS_WIN
  makeme
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  XMapWindow(_dis, _win);
  move(x0, y0);
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC
}


void osiWindow::hide() {
  #ifdef OS_WIN
  makeme
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  XUnmapWindow(_dis, _win);
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC
}


void osiWindow::move(int x, int y) {
  #ifdef OS_WIN
  makeme
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  XMoveWindow(_dis, _win, x0, osi.display.vdy- (y0+ dy));
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC
}


void osiWindow::resize(int dx, int dy) {
  #ifdef OS_WIN
  makeme
  #endif /// OS_WIN
  
  #ifdef OS_LINUX
  XResizeWindow(_dis, _win, dx, dy);
  #endif /// OS_LINUX
  
  #ifdef OS_MAC
  makeme
  #endif /// OS_MAC
}









