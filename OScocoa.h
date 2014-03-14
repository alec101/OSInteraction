#pragma once
#ifdef OS_MAC


class OSIcocoa {
  
  
  
  
public:
  
  void setProgramPath();              /// under mac is not certain that it is set, it seems
  bool createWindow(OSIWindow *w);
  
  void checkMSG();
  
  
  void getWindowSize(OSIWindow *w, int *dx, int *dy);
  void sleep(int ms);                 /// cocoa sleep function (in miliseconds)
  
  int passedTime(void);               /// this might be scraped
  
  void swapBuffers(OSIWindow *w);
  void makeCurrent(OSIWindow *w);
  bool displayName(unsigned long id, string8 *out);  // returns in out the name of the display
  
  OSIcocoa();
  ~OSIcocoa();
};

extern OSIcocoa cocoa;



/*
// MAC DELEGATE object //
extern "C" {
@interface MacDelegate: NSObject { // < NSApplicationDelegate > 
}
- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
@end

  
// MAC WINDOW object //

@interface MacGLWindow: NSWindow {
}
- (id) initWithContentRect: (NSRect)rect styleMask:(NSUInteger)wndStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferFlg;
- (void) windowDidResize: (NSNotification *)notification;
- (void) windowWillClose: (NSNotification *)notification;
@end
  
  
// MAC OPENGLVIEW object //

@interface MacGLview: NSOpenGLView {
}
- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
- (NSMenu *)menuForEvent: (NSEvent *)theEvent;
- (void) flagsChanged: (NSEvent *)theEvent;
- (void) keyDown:(NSEvent *)theEvent;
- (void) keyUp:(NSEvent *)theEvent;
- (void) mouseMoved:(NSEvent *)theEvent;
- (void) mouseDragged:(NSEvent *)theEvent;
- (void) rightMouseDragged:(NSEvent *)theEvent;
- (void) otherMouseDragged:(NSEvent *)theEvent;
- (void) mouseDown:(NSEvent *)theEvent;
- (void) mouseUp:(NSEvent *)theEvent;
- (void) rightMouseDown:(NSEvent *)theEvent;
- (void) rightMouseUp:(NSEvent *)theEvent;
- (void) otherMouseDown:(NSEvent *)theEvent;
- (void) otherMouseUp:(NSEvent *)theEvent;
@end
  
}
*/

#endif /// OS_MAC

