#include "pch.h"

#ifdef OS_MAC

/// cocoa uses these, unfortunately...
#undef uint
#undef ushort
#undef null
#undef uint64
#undef uint32
#undef uint16
#undef uint8
#undef word
#undef byte

#import <Cocoa/Cocoa.h>
//#import <Foundation/Foundation.h>
//#import <AppKit/AppKit.h>
#import <IOKit/graphics/IOGraphicsLib.h>
//#include <ApplicationServices.framework/Headers/ApplicationServices.h>
#include <CoreGraphics/CGDirectDisplay.h>

//#include <Core

// TODO: 
/*
 the name of the display must be found a different way. the current func was just deprecated...
 
 */


// COCOA RESEARCH
/*
 https://developer.apple.com/library/mac/samplecode/CocoaGL/Listings/GLCheck_c.html#//apple_ref/doc/uid/DTS10004501-GLCheck_c-DontLinkElementID_7
 void CheckOpenGLCaps (CGDisplayCount maxDspys,
                      GLCaps dCaps[],
                      CGDisplayCount * dCnt)
 check that function. osi.populate() can copy a lot from that


 * make a test to really see if the poll for msg is slow (can't rely on rumors)
   must implement the other (system call for a func) method


 * in case opera clears pins again...
 * http://stackoverflow.com/questions/4982656/programmatically-get-screen-size-in-mac-os-x
 * https://developer.apple.com/library/mac/documentation/Cocoa/Reference/ApplicationKit/Classes/NSScreen_Class/Reference/Reference.html
 * https://developer.apple.com/library/mac/documentation/GraphicsImaging/Conceptual/QuartzDisplayServicesConceptual/Articles/DisplayInfo.html#//apple_ref/doc/uid/TP40004272-SW1
 * 
 * quartz is the way for monitors/resolutions/etc. every function needed is there (it is NOT in objective-C)
 * 
 * NSOpenGLContext -> check for specific cocoa/gl funcs (agl is gone)
 * every 'method' is actually a c function wrapped in some shit, so do not despair!
 
 
 Note: Views that receive and edit text must conform to the NSTextInput protocol. Adopting this protocol allows a custom view to interact properly with the text input management system. The Application Kit classes NSText and NSTextView implement NSTextInput, so if you subclass these classes you get the protocol conformance “for free.“
 
 
 */

// direct call with extern "C"

OSIcocoa cocoa;

///-------------------///
// MAC DELEGATE object //
///-------------------///

@interface MacDelegate: NSObject // < NSApplicationDelegate >
{}

/* - (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication; */
@end

@implementation MacDelegate

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
  return YES;
}

- (BOOL) canBecomeMainWindow:(NSApplication *)theApplication{
  return YES;
}

@end



///-----------------///
// MAC WINDOW object //
///-----------------///

@interface MacGLWindow: NSWindow {
}

@end

@implementation MacGLWindow
- (id) initWithContentRect: (NSRect)rect styleMask:(NSUInteger)wndStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferFlg {
  [super initWithContentRect:rect styleMask:wndStyle backing:bufferingType defer:deferFlg];

  [[NSNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(windowDidResize:)
    name:NSWindowDidResizeNotification
    object:self];

  [[NSNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(windowWillClose:)
    name:NSWindowWillCloseNotification
    object:self];

  [self setAcceptsMouseMovedEvents:YES];
  
  [self setExcludedFromWindowsMenu:NO];

  printf("%s\n",__FUNCTION__);
  return self;
}

- (void) windowDidResize: (NSNotification *)notification {
}

- (void) windowWillClose: (NSNotification *)notification {
  osi.flags.exit= true;
  [NSApp terminate:nil];    // <<<< program exit. is this ok? a close button is pressed... i guess program must exit...
}

@end



///---------------------///
// MAC OPENGLVIEW object //
///---------------------///

@interface MacGLview : NSOpenGLView {
}

- (void) drawRect: (NSRect) bounds;
@end

@implementation MacGLview
-(void) drawRect: (NSRect) bounds {
  printf("%s\n",__FUNCTION__);
//  exposure=1;
}

-(void) prepareOpenGL {
  printf("%s\n",__FUNCTION__);
}

-(NSMenu *)menuForEvent: (NSEvent *)theEvent {
  printf("%s\n",__FUNCTION__);
  return [NSView defaultMenu];
}


///==========================================================================///
// ---------------------------- SYSTEM EVENTS ------------------------------- //
///==========================================================================///


// --------------------------- KEYBOARD EVENTS ------------------------------ //

/* research:
NSUInteger flags = [theEvent modifierFlags] & NSDeviceIndependentModifierFlagsMask;
if( flags == NSCommandKeyMask ){
  ...
}
if(flags == NSCommandKeyMask+ NSControlKeyMask) if ⌘ and ⌃ should be pressed rather writing flags & NSCommandKeyMask && flags & NSControlKeyMask
*/



// caps/shift/ctrl/alt/ these kind of keys have this func... who knows how to distinguish between a press and a depress...
- (void) flagsChanged: (NSEvent *)theEvent {
  // find out all flagsChanged!!!!!!!!!!
  
  bool chatty= true;
  ulong flags= [theEvent modifierFlags];
  uchar code= [theEvent keyCode];
  
  osi.getMillisecs(&osi.eventTime);
  printf("event!!\n");
  //MUST TEST THIS, it is INPOSSIBLE OTHERWISE...
  /// caps lock
  uchar press= 0;
  if(code== in.Kv.capslock) {
    if(flags& NSAlphaShiftKeyMask) {
      press= 128;
      in.k.capsLock= true;  // MUST TEST
    } else {
      in.k.capsLock= false; // not shure about this

    }
    printf("capsLock[%s]\n", in.k.capsLock? "true": "false");
  }
  // TEST ^^^ no clue if macs actually have a true 'lock'

  /// the others ...

  // MUST TEST
  else if((code== in.Kv.lshift || code== in.Kv.rshift) && (flags& NSShiftKeyMask))
    press= 128;
  else if((code== in.Kv.lctrl || code== in.Kv.rctrl) && (flags& NSControlKeyMask))
    press= 128;
  else if((code== in.Kv.lalt || code== in.Kv.ralt) && (flags& NSAlternateKeyMask))
    press= 128;
  else if((code== in.Kv.lOS || code== in.Kv.rOS) && (flags& NSCommandKeyMask))
    press= 128;
  
  // japanese kana & others... ffs    THIS IS A MESS, iCRAP(TM)
 
  
  // it's a KEY PRESS
  if(press) {
    osi.flags.keyPress= true;
    if(chatty) printf("key PRESS code[0x%x] \n", code);
    
    /// log the key
    Keyboard::KeyPressed k;
    k.code= code;
    k.checked= false;
    k.timeDown= osi.eventTime;
    in.k.log(k);
    /// set the key as pressed & other needed vars
    in.k.key[code]= 128;
    in.k.keyTime[code]= osi.eventTime;
    //in.k.repeat[code]= 128;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED
    
  // it's a KEY DEPRESS
  } else {
    osi.flags.keyPress= false;
    
    if(chatty) printf("key RELEASE code[0x%x]\n", code);
    
    
    
    /// log the key in history
    bool found= false;
    for(short a= 0; a< MAX_KEYS_LOGGED; a++)
      if(in.k.lastKey[a].code== code) {
        in.k.lastKey[a].timeUp= osi.eventTime;
        in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
        found= true;
        break;
      }
    /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
    if(!found)  {
      Keyboard::KeyPressed k;
      k.code= code;
      k.checked= false;
      k.timeUp= osi.eventTime;
      k.timeDown= k.timeUp- 1;            /// 1 milisecond before the keyup
      k.timeDT= 1;                        /// timeUp- timeDown
      in.k.log(k);
    }
    
    /// set the key as pressed & other vars
    in.k.key[code]= 0;
    in.k.keyTime[code]= 0;
    //    in.k.repeat[code]= 0;
    
  } /// key press/depress
  
  
  //in.k.key[code]= press;
  // MUST TEST ^^^
 
  // Other possible key masks
  // NSNumericPadKeyMask
  // NSHelpKeyMask
  // NSFunctionKeyMask
  // NSDeviceIndependentModifierFlagsMask
}

// ---------------================ KEY DOWN =================-------------------
- (void) keyDown:(NSEvent *)theEvent {
  // string UCKeytranslate(b,a,c);<< this one is "very low". hopefully, everything is ok with what cocoa has as a basic.
  
  bool chatty= true;
  uchar code= [theEvent keyCode];
  
  // ulong flags= [theEvent modifierFlags];
  // long unicode;
  // NSString *chrs= [theEvent characters];
  // NSString *chrsNoMod= [theEvent charactersIgnoringModifiers];

  osi.getMillisecs(&osi.eventTime);       /// event time. can't rely on what cocoa passes, it must match with osi.getMilisecs()
  
  osi.flags.keyPress= false;
  
  /// if it's not a repeat press event, start the keypress log
  if(![theEvent isARepeat]) {
    if(chatty) printf("Key pressed: code[0x%x]\n", code);
    /// log the key
    Keyboard::KeyPressed k;
    k.code= code;
    k.checked= false;
    k.timeDown= osi.eventTime;
    in.k.log(k);
    /// set the key as pressed & other needed vars
    in.k.key[code]= 128;
    in.k.keyTime[code]= osi.eventTime;
    //in.k.repeat[code]= 1;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED
  }
  
  in.k.doManip();     /// check if pressed keys form a manip char. if they do, manipChar is added to manip stream
  
  /// next line translates key(s) for inputText and doCommandBySelector (wich is disabled)
  [self interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
}

// ---------------================= KEY UP ==================-------------------
- (void) keyUp:(NSEvent *)theEvent {
  bool chatty= true;
  
  makeme
  osi.getMillisecs(&osi.eventTime);
  osi.flags.keyPress= false;
  
  uchar code= [theEvent keyCode];
  
  if(chatty) printf("key RELEASE code[0x%x]\n", code);
  
  /// log the key in history
  bool found= false;
  for(short a= 0; a< MAX_KEYS_LOGGED; a++)
    if(in.k.lastKey[a].code== code) {
      in.k.lastKey[a].timeUp= osi.eventTime;
      in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
      found= true;
      break;
    }
  /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
  if(!found)  {
    Keyboard::KeyPressed k;
    k.code= code;
    k.checked= false;
    k.timeUp= osi.eventTime;
    k.timeDown= k.timeUp- 1;           /// 1 milisecond before the keyup
    k.timeDT= 1;                      /// timeUp- timeDown
    in.k.log(k);
  }
  
  /// set the key as pressed & other vars
  in.k.key[code]= 0;
  in.k.keyTime[code]= 0;
  //    in.k.repeat[code]= 0;
  
  
  // OLD CRAP - DELETE <<<<<<<<<<<<<<<  
  //  chrs= [theEvent characters];
  //  if([chrs length]> 0) {
  //    int unicode;
  //    unicode= [chrs characterAtIndex:0];
  //	}
  //  chrsNoMod= [theEvent charactersIgnoringModifiers];
  //  if([chrsNoMod length]>0) {
  //    int unicode, fskey;
  //    unicode= [chrsNoMod characterAtIndex:0];
    /*
    fskey= YsMacUnicodeToFsKeyCode(unicode);

    if(fskey!=0) {
      fsKeyIsDown[fskey]= 0;
    }
    */
  //}
}

///==========================================================================///
// ----------------------------- MOUSE EVENTS ------------------------------- //
///==========================================================================///

// ---------------============== MOUSE MOVED ================-------------------
- (void) mouseMoved:(NSEvent *)theEvent {
  NSRect rect= [self frame];
  
  in.m.oldx= in.m.x;
  in.m.oldy= in.m.y;
  in.m.x= (int)[theEvent locationInWindow].x;
  in.m.y= rect.size.height- 1- (int)[theEvent locationInWindow].y;
  if(in.m.useDelta) {
    in.m.dx+= in.m.x- in.m.oldx;
    in.m.dy+= in.m.y- in.m.oldy;
  }
}


// ---------------============ LEFT BUTTON DOWN =============-------------------
- (void) mouseDown:(NSEvent *)theEvent {
  osi.flags.buttonPress= true;
  osi.getMillisecs(&osi.eventTime);
  
  in.m.b[0].down= true;
  in.m.b[0].timeStart= osi.eventTime;
}

// ---------------============= LEFT BUTTON UP ==============-------------------
- (void) mouseUp:(NSEvent *)theEvent {
  osi.getMillisecs(&osi.eventTime);
  osi.flags.buttonPress= false;
  
  if(in.m.b[0].down) {                    /// an alt-bab might mess stuff...
    in.m.b[0].lastTimeStart= in.m.b[0].timeStart;
    in.m.b[0].lastTimeEnded= osi.eventTime;
    in.m.b[0].lastDT= in.m.b[0].lastTimeEnded- in.m.b[0].lastTimeStart;
  } else {                                /// an alt-tab? might mess stuff
    in.m.b[0].lastTimeEnded= osi.eventTime;
    in.m.b[0].lastTimeStart= osi.eventTime- 1;
    in.m.b[0].lastDT= 1;
  }
  in.m.b[0].down= false;
}

// ---------------=========== RIGHT BUTTON DOWN =============-------------------
- (void) rightMouseDown:(NSEvent *)theEvent {
  osi.flags.buttonPress= true;
  osi.getMillisecs(&osi.eventTime);
  
  in.m.b[1].down= true;
  in.m.b[1].timeStart= osi.eventTime;
}

// ---------------============ RIGHT BUTTON UP ==============-------------------
- (void) rightMouseUp:(NSEvent *)theEvent {
  osi.getMillisecs(&osi.eventTime);
  osi.flags.buttonPress= false;
  
  if(in.m.b[1].down) {                    /// an alt-bab might mess stuff...
    in.m.b[1].lastTimeStart= in.m.b[1].timeStart;
    in.m.b[1].lastTimeEnded= osi.eventTime;
    in.m.b[1].lastDT= in.m.b[1].lastTimeEnded- in.m.b[1].lastTimeStart;
  } else {                                /// an alt-tab? might mess stuff
    in.m.b[1].lastTimeEnded= osi.eventTime;
    in.m.b[1].lastTimeStart= osi.eventTime- 1;
    in.m.b[1].lastDT= 1;
  }
  in.m.b[1].down= false;
}

// ---------------========== 'OTHER' BUTTON DOWN ============-------------------
- (void) otherMouseDown:(NSEvent *)theEvent {
  int b= (int)[theEvent buttonNumber];      // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  osi.flags.buttonPress= true;
  osi.getMillisecs(&osi.eventTime);
  
  in.m.b[b].down= true;
  in.m.b[b].timeStart= osi.eventTime;
}

// ---------------=========== 'OTHER' BUTTON UP =============-------------------
- (void) otherMouseUp:(NSEvent *)theEvent {
  int b= (int)[theEvent buttonNumber];      // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  osi.getMillisecs(&osi.eventTime);
  osi.flags.buttonPress= false;
  
  if(in.m.b[b].down) {                    /// an alt-bab might mess stuff...
    in.m.b[b].lastTimeStart= in.m.b[b].timeStart; 
    in.m.b[b].lastTimeEnded= osi.eventTime;
    in.m.b[b].lastDT= in.m.b[b].lastTimeEnded- in.m.b[b].lastTimeStart;
  } else {                                /// an alt-tab? might mess stuff
    in.m.b[b].lastTimeEnded= osi.eventTime;
    in.m.b[b].lastTimeStart= osi.eventTime- 1;
    in.m.b[b].lastDT= 1;
  }
  in.m.b[b].down= false;
}


- (void) mouseDragged:(NSEvent *)theEvent {
  [self mouseMoved:theEvent];
}

- (void) rightMouseDragged:(NSEvent *)theEvent {
  [self mouseMoved:theEvent];
}

- (void) otherMouseDragged:(NSEvent *)theEvent {
  [self mouseMoved:theEvent];
}



// keyboard character(s) input
- (void)insertText:(id)string {
  long unicode;
  
  for(short a= 0; a< [string length]; a++) {
    unicode= [string characterAtIndex: a];
    in.k.addChar(unicode, &osi.eventTime);      /// eventTime is already updated. insertText is RIGHT AFTER a keyDown event
  }

  //[super insertText:string];  // have superclass insert it - THIS MAKES A BEEP if no one handles the text
}


- (void)doCommandBySelector:(SEL)aSelector {
  // overided to disable command (key) beeps
}

@end








///-------------///
// OScocoa CLASS //
///-------------///


OSIcocoa::OSIcocoa() {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
  
  
  [NSApplication sharedApplication];  
  /// setting the application as a regular app, with taskbar icon&everything.
  /// by default the app has no taskbar icon& window menu

  /*
  // THIS WORKS, but it is deprecated :(
  ProcessSerialNumber psn;
  if (!GetCurrentProcess(&psn)) {
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
   SetFrontProcess(&psn);
  }
  */
  
  // this works, it seems
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
  
  
  /// setup the 'delegate' (this delegate has some app settings, but heck if i know why it exists)
  MacDelegate *delegate;
  delegate= [MacDelegate alloc];
  [delegate init];
  [NSApp setDelegate: delegate];
  
  // ICON MUST BE PLACED SOMEWHERE AROUND HERE or after finishLaunching();
  //myImage = [NSImage imageNamed: @"ChangedIcon"];
  //  [NSApp setApplicationIconImage: myImage];
  
  [NSApp finishLaunching]; // <<<<---- after finish launching settings from here 
  
  /// this seems to disable the crappy momentum scrolling. problem is, there won't be any scrolling, i think
  //  NSDictionary *appDefaults = [NSDictionary dictionaryWithObject:@"NO" forKey:@"AppleMomentumScrollSupported"];
  //  [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];

  // this doesn't seem to work atm - COPY PASTE FROM SDL
  /// Create the window menu
  NSMenu *windowMenu= [[NSMenu alloc] initWithTitle:@"Window"];
  [windowMenu addItemWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
  [windowMenu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
  [NSApp setWindowsMenu:windowMenu];
  [windowMenu release];
  

  [NSApp activateIgnoringOtherApps: YES];
  
  [pool release];
}

OSIcocoa::~OSIcocoa() {
}



// something like this must be called in OSI constructor <<<<<<<<<<<<<<<<<<<<<<<
void OSIcocoa::setProgramPath() {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];

  char buf[1024];
  getcwd(buf, 1023);
	printf("CWD(Initial): %s\n", buf);

	NSString *path;
	path= [[NSBundle mainBundle] bundlePath];
	printf("BundlePath:%s\n", [path UTF8String]);
  
	[[NSFileManager defaultManager] changeCurrentDirectoryPath: path];

	getcwd(buf, 1023);
	printf("CWD(Changed): %s\n", buf);
  
  osi.path= [path UTF8String];
  
	[pool release];
}




 
 /*
  NSMenu *appleMenu;
  NSMenu *serviceMenu;
  NSMenu *windowMenu;
  NSMenuItem *menuItem;
  
  if (NSApp == nil) {
    return;
  }
  
  // Create the main menu bar 
  [NSApp setMainMenu:[[NSMenu alloc] init]];
  
  // Create the application menu
  appName = GetApplicationName();
  appleMenu = [[NSMenu alloc] initWithTitle:@""];
  
  // Add menu items 
  title = [@"About " stringByAppendingString:appName];
  [appleMenu addItemWithTitle:title action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
  
  [appleMenu addItem:[NSMenuItem separatorItem]];
  
  [appleMenu addItemWithTitle:@"Preferences…" action:nil keyEquivalent:@","];
  
  [appleMenu addItem:[NSMenuItem separatorItem]];
  
  serviceMenu = [[NSMenu alloc] initWithTitle:@""];
  menuItem = (NSMenuItem *)[appleMenu addItemWithTitle:@"Services" action:nil keyEquivalent:@""];
  [menuItem setSubmenu:serviceMenu];
  
  [NSApp setServicesMenu:serviceMenu];
  [serviceMenu release];
  
  [appleMenu addItem:[NSMenuItem separatorItem]];
  
  title = [@"Hide " stringByAppendingString:appName];
  [appleMenu addItemWithTitle:title action:@selector(hide:) keyEquivalent:@"h"];
  
  menuItem = (NSMenuItem *)[appleMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"];
  [menuItem setKeyEquivalentModifierMask:(NSAlternateKeyMask|NSCommandKeyMask)];
  
  [appleMenu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];
  
  [appleMenu addItem:[NSMenuItem separatorItem]];
  
  title = [@"Quit " stringByAppendingString:appName];
  [appleMenu addItemWithTitle:title action:@selector(terminate:) keyEquivalent:@"q"];
  
  // Put menu into the menubar
  menuItem = [[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""];
  [menuItem setSubmenu:appleMenu];
  [[NSApp mainMenu] addItem:menuItem];
  [menuItem release];
  
  // Tell the application object that this is now the application menuß
  [NSApp setAppleMenu:appleMenu];
  [appleMenu release];
  
  
  // Create the window menu
  windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
  
  // Add menu items
  [windowMenu addItemWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
  
  [windowMenu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
  
  // Put menu into the menubar
  menuItem = [[NSMenuItem alloc] initWithTitle:@"Window" action:nil keyEquivalent:@""];
  [menuItem setSubmenu:windowMenu];
  [[NSApp mainMenu] addItem:menuItem];
  [menuItem release];
  
  // Tell the application object that this is now the window menu
  [NSApp setWindowsMenu:windowMenu];
  [windowMenu release];
}
*/

bool OSIcocoa::createWindow(OSIWindow *w) {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
  
  bool ret= true;
  
  MacGLWindow *win= NULL;
  MacGLview *view= NULL;
  
  /// window size
  NSRect contRect;
  contRect= NSMakeRect(w->monitor->x0+ w->x0, w->monitor->y0+ w->y0, w->dx, w->dy);
  
  /// window style
  uint winStyle;
	if(w->mode== 1)                     /// windowed style
    winStyle=
      NSTitledWindowMask|
      NSClosableWindowMask|
      NSMiniaturizableWindowMask|
      NSResizableWindowMask;
  if(w->mode== 3 || w->mode== 2)      /// fullscreen / fullscreen window style
    winStyle=
//      NSTitledWindowMask|
//      NSClosableWindowMask|
      NSMiniaturizableWindowMask|
      NSBorderlessWindowMask;
  
  /// window alloc & init
  win= [MacGLWindow alloc];
  [win
    initWithContentRect: contRect
    styleMask: winStyle
    backing: NSBackingStoreBuffered 
    defer: NO];
  
  // see http://www.mikeash.com/pyblog/nsopenglcontext-and-one-shot.html
  /// prevent window deletion when hidden
  [win setOneShot:NO];

  /// pixel format
  // THIS MIGHT NEED MORE CUSTOMIZATION
  NSOpenGLPixelFormat *format;
  NSOpenGLPixelFormatAttribute formatAttrib[]= {
    NSOpenGLPFAWindow,
    NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)32,
    NSOpenGLPFADoubleBuffer,                                  /// double buffer window
    0
  };
  
  format= [NSOpenGLPixelFormat alloc];
  [format initWithAttributes: formatAttrib];

	/// OpenGL view alloc & init
  view= [MacGLview alloc];
  contRect= NSMakeRect(0, 0, w->dx, w->dy);
  [view
    initWithFrame: contRect
    pixelFormat: format];
  
  /// rest of window settings
  [win setContentView: view];
  [win makeFirstResponder: view];
  [win setTitle:[NSString stringWithUTF8String: w->name.d]];
  [win makeKeyAndOrderFront: nil];
  
  if(w== &osi.win[0])
    [win makeMainWindow];

  //  [NSApp activateIgnoringOtherApps: YES]; // THIS WAS MOVED TO CONSTRUCTOR

  /// OSIWindow connection
  w->win= win;
  w->view= view;
  w->isCreated= true;
  
  [pool release];

  return ret;
}


void OSIcocoa::getWindowSize(OSIWindow *w, int *dx, int *dy) {
  NSRect rect;
  MacGLview *view= (MacGLview *)w->view;
  rect= [view frame];
  *dx= rect.size.width;
  *dy= rect.size.height;
}


void processMSG(void) {
  NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

  while(1) {
    [pool release];
    pool= [[NSAutoreleasePool alloc] init];
	
    NSEvent *event;
    event= [NSApp
           nextEventMatchingMask: NSAnyEventMask
           untilDate: [NSDate distantPast]
           inMode: NSDefaultRunLoopMode
           dequeue: YES];
    
    //    if([event type]== NSRightMouseDown) {
    //		  printf("R mouse down event\n");
    //    }
    
    if(event!= nil) {
      [NSApp sendEvent: event];
      [NSApp updateWindows];
    } else {
      break;
    }
	}
	[pool release];	
}

// TIME... ? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void OSIcocoa::sleep(int ms) {
  if(ms> 0) {
    double sec;
    sec= (double)ms/ 1000.0;
    [NSThread sleepForTimeInterval: sec];
  }
}

/*
int passedTime(void) {
  int ms;

  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];

  static NSTimeInterval last= 0.0;
  NSTimeInterval now;

  now= [[NSDate date] timeIntervalSince1970];

  NSTimeInterval passed;
  passed= now- last;
  ms= (int)(1000.0* passed);

  if(ms< 0)
    ms= 1;

  last= now;

  [pool release];	

  return ms;
}
*/


void OSIcocoa::swapBuffers(OSIWindow *w) {
  //MacGLview *view= (MacGLview *)w->view;
  //[[view openGLContext] flushBuffer];
  [[(MacGLview *)w->view openGLContext] flushBuffer];
}

void OSIcocoa::makeCurrent(OSIWindow *w) {
  //  (((MacGLview *)w->view).openGLContext.makeCurrentContext(); //<< something like this can be done
  [[(MacGLview *)w->view openGLContext] makeCurrentContext];
}


bool OSIcocoa::displayName(unsigned long id, string8 *out) {
  bool ret= false;
  out->delData();
  NSString *screenName= nil;
  
  NSDictionary *deviceInfo= (NSDictionary *)IODisplayCreateInfoDictionary(CGDisplayIOServicePort(id), kIODisplayOnlyPreferredName);
  NSDictionary *localizedNames= [deviceInfo objectForKey: [NSString stringWithUTF8String: kDisplayProductName]];

  if([localizedNames count]> 0) {
    screenName= [[localizedNames objectForKey: [[localizedNames allKeys] objectAtIndex: 0]] retain];
    *out= [screenName UTF8String];
    [screenName release];
    ret= true;
  }

  [deviceInfo release];
  return ret;
}




/* int main(int argc, char *argv[])
{
	YsTestApplicationPath();

	YsOpenWindow();

	printf("Going into the event loop\n");

	double angle;
	angle=0.0;
	while(1)
	{
		YsPollEvent();

		DrawTriangle(angle);
		angle=angle+0.05;

		YsSleep(20);
	}

	return 0;
	} */



//#define uint unsigned int
//#define ushort unsigned short
//#define null NULL




#endif /// OS_MAC