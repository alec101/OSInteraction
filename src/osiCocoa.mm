#include "osinteraction.h"
#include "util/typeShortcuts.h"

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
 
 * there might be a set to make a window the principal 'responder' wich i belive will be the one to receive msg events. there's 2 levels of this, i think (before anyone processes even, possibly)
 
 */


//osiCocoa cocoa;
bool _osiCocoaSetTheIcon;

///-------------------///
// MAC DELEGATE object //
///-------------------///

@interface MacDelegate: NSObject // < NSApplicationDelegate >
{}

/* - (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication; */
@end

@implementation MacDelegate
// various 
- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
  return YES;
}

// this one might be wrongly placed. the MacGLWindow one is the right placement, i think...
// they are not called tho...

- (BOOL) canBecomeMainWindow:(NSApplication *)theApplication{
  #ifdef OSI_BE_CHATTY
  printf("%s\n", __FUNCTION__);
  #endif
  return YES;
}

@end



///-----------------///
// MAC WINDOW object //
///-----------------///

@interface osiMacWindow: NSWindow {
}

@end

@implementation osiMacWindow
/*
- (BOOL) canBecomeMainWindow:(NSApplication *)theApplication  {
  printf("%s\n", __FUNCTION__);
  return YES;
}
*/
- (BOOL) canBecomeKeyWindow {
  // printf("%s\n", __FUNCTION__);  // << this one is spammed... there might be a setCanBecome, so this is not spammed anymore
  return YES;
}

// extra window propreties in init func
- (id) initWithContentRect: (NSRect)rect styleMask:(NSUInteger)wndStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferFlg {
  [super initWithContentRect:rect styleMask:wndStyle backing:bufferingType defer:deferFlg];

  [[NSNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(windowDidResize:)
    name:NSWindowDidResizeNotification
    object:self];

  [[NSNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(windowDidMove:)
    name:NSWindowDidMoveNotification
    object:self];

  [[NSNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(windowWillClose:)
    name:NSWindowWillCloseNotification
    object:self];


  [[NSNotificationCenter defaultCenter]
   addObserver:self
   selector:@selector(windowDidResignKey:)
   name:NSWindowDidResignKeyNotification
   object:self];

  [[NSNotificationCenter defaultCenter]
   addObserver:self
   selector:@selector(windowDidBecomeKey:)
   name:NSWindowDidBecomeKeyNotification
   object:self];

  
  [self setAcceptsMouseMovedEvents:YES];
  
  [self setExcludedFromWindowsMenu:NO];

  //printf("%s\n", __FUNCTION__);
  return self;
}

// window resize
- (void) windowDidResize: (NSNotification *)notification {
  osiWindow *w;

  if((w= osi._getWin(self)))        /// safety check; if window was found
    if(w->mode== 1) {
      NSRect r= [self frame];
      w->dx= r.size.width;
      w->dy= r.size.height;
      osi.flags.windowResized= true;
    }
}

// window move
- (void) windowDidMove: (NSNotification *)notification {
  osiWindow *w;
  if((w= osi._getWin(self)))
    if(w->mode== 1) {
      NSRect r= [self frame];
      w->x0= r.origin.x;
      #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
      w->y0= r.origin.y;
      #endif
      #ifdef OSI_USE_ORIGIN_TOP_LEFT
      w->y0= osi.display.vyMax- (r.origin.y+ w->dy- 1);
      #endif
      osi.flags.windowMoved= true;
    }

}

// window close - PROGRAM EXIT
- (void) windowWillClose: (NSNotification *)notification {
  osi.flags.exit= true;
  [NSApp terminate:nil];    // <<<< program exit. is this ok? a close button is pressed... i guess program must exit...
}

// --------------------->>>> WINDOW FOCUS IN <<<<------------------------
- (void)windowDidBecomeKey:(NSNotification *)notification{
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  if(chatty) printf("%s\n", __FUNCTION__);
  #endif

  /// set the hasFocus flag of the window that become key to true
  osiWindow *w= osi._getWin(self);

  if(w)
    w->hasFocus= true;
  
  /// the program was not active/ just started?
  if(!osi.flags.haveFocus) {
    if([NSApp isActive]) {    /// app just became active
      osi.flags.haveFocus= true;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("program is ACTIVE\n");
      #endif
      
      /// loop thru all created windows
      for(int16 a= 0; a< MAX_WINDOWS; a++)
        if(osi.win[a].isCreated) {

          /// set all created windows below the current window, if the program just activated
          /// avoids to have some windows behind other applications, if this program just got active
          if(w)
            if(osi.win[a].mode== 1)
              if(osi.win[a]._win!= w->_win)
                [((osiMacWindow *)osi.win[a]._win) orderWindow:NSWindowBelow relativeTo: [((osiMacWindow *)w->_win) windowNumber]];
          
          // UNHIDE (maximize) is set in createWindow, easyer: [win setHidesOnDeactivate:YES];

          /// in full screen, change monitor resolution to 'program resolution'
          if(osi.win[a].mode== 2)
            osi.display.changeRes(osi.win[a].monitor, osi.win[a].dx, osi.win[a].dy, osi.win[a].freq);              
            
          /// in full screen & full screen window, set the windows level to top (shilding level)
          if(osi.win[a].mode== 2 || osi.win[a].mode== 3|| osi.win[a].mode== 4)
            [((osiMacWindow *)osi.win[a]._win) setLevel:CGShieldingWindowLevel()];

        } /// if the window is created
      
    } else {                  /// impossible...
      osi.flags.haveFocus= true;
      // but the program is NOT ACTIVE... so this is not possible
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("black hole sun!\n");
      #endif
    }
  } /// if osi.haveFocus is false

}


// ------------------>>>> WINDOW FOCUS OUT <<<<----------------------
- (void)windowDidResignKey:(NSNotification *)notification{
  #ifdef OSI_BE_CHATTY
  bool chatty= false;
  if(chatty) printf("%s\n", __FUNCTION__);
  #endif

  /// set the hasFocus flag of the window as false
  osiWindow *w= osi._getWin(self);
  if(w)
    w->hasFocus= false;

  if(![NSApp isActive]) {   /// the program is NOT active anymore
    #ifdef OSI_BE_CHATTY
    if(chatty) printf("app is NOT ACTIVE\n");
    #endif
    osi.flags.haveFocus= false;
    
    /// loop thru all created windows
    for(short a= 0; a< MAX_WINDOWS; a++)
      if(osi.win[a].isCreated) {
        
        /// restore original monitor resolution
        if(osi.win[a].mode== 2)
          osi.display.restoreRes(osi.win[a].monitor);
        
        /// lose the 'shielded' atribute (move to back)
        if(osi.win[a].mode== 2|| osi.win[a].mode== 3|| osi.win[a].mode== 4)
          [((osiMacWindow *)osi.win[a]._win) setLevel:kCGNormalWindowLevel- 1];
      } /// if this is a created window
  } /// if the program is not active anymore
}

@end



///---------------------///
// MAC OPENGLVIEW object //
///---------------------///

@interface osiMacGLview : NSOpenGLView {
}

- (void) drawRect: (NSRect) bounds;
@end

// not much to bother here
@implementation osiMacGLview

/*
- (BOOL) canBecomeKeyView {
  printf("%s\n", __FUNCTION__);
  return YES;
}
*/

/*
- (BOOL) wantsBestResolutionOpenGLSurface {
  // this might be overkill; rendering @ more than a pixel resolution... this might be way too gpu unfriendly; UNTESTED
  // see https://developer.apple.com/library/mac/documentation/graphicsimaging/conceptual/OpenGL-MacProgGuide/EnablingOpenGLforHighResolution/EnablingOpenGLforHighResolution.html#//apple_ref/doc/uid/TP40001987-CH1001-SW5
  return YES;
}
*/

-(void) drawRect: (NSRect) bounds {
  // printf("%s\n", __FUNCTION__);
}

-(void) prepareOpenGL {
  // printf("%s\n", __FUNCTION__);
}

-(NSMenu *)menuForEvent: (NSEvent *)theEvent {
  // printf("%s\n", __FUNCTION__);
  return [NSView defaultMenu];
}


///==========================================================================///
// ---------------------------- SYSTEM EVENTS ------------------------------- //
///==========================================================================///






// ########################### KEYBOARD EVENTS ############################## //

/* research:
NSUInteger flags = [theEvent modifierFlags] & NSDeviceIndependentModifierFlagsMask;
if( flags == NSCommandKeyMask ){
  ...
}
if(flags == NSCommandKeyMask+ NSControlKeyMask) if ⌘ and ⌃ should be pressed rather writing flags & NSCommandKeyMask && flags & NSControlKeyMask
*/


/*
// -----------============= caps/shift/ctrl/alt/etc =============------------ //
- (void) flagsChanged: (NSEvent *)theEvent {
  // find out all flagsChanged!!!!!!!!!!
  
  ulong flags= [theEvent modifierFlags];
  uint8 code= [theEvent keyCode];
  
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision
  
  if(chatty) printf("event!!\n");
  
  //MUST TEST THIS, it is INPOSSIBLE OTHERWISE...
  /// caps lock
  uint8 press= 0;
  if(code== in.Kv.capslock) {
    if(flags& NSAlphaShiftKeyMask) {
      press= 128;
      in.k.capsLock= true;  // MUST TEST
    } else {
      in.k.capsLock= false; // not shure about this

    }
    if(chatty) printf("capsLock[%s]\n", in.k.capsLock? "true": "false");
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
    if(chatty) printf("key PRESS code[0x%x] [flagsChanged]\n", code);
    
    /// log the key
    osiKeyboard::osiKeyLog k;
    k.code= code;
    k.checked= false;
    k.timeDown= osi.eventTime;
    k.timeUp= 0;
    k.timeDT= 0;
    in.k._log(k);
    /// set the key as pressed & other needed vars
    in.k.key[code]= 128;
    in.k.keyTime[code]= osi.eventTime;
    //in.k.repeat[code]= 128;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED
    
  // it's a KEY DEPRESS
  } else {
    osi.flags.keyPress= false;
    
    if(chatty) printf("key RELEASE code[0x%x] [flagsChanged]\n", code);
    
    
    
    /// log the key in history
    bool found= false;
    for(short a= 0; a< MAX_KEYS_LOGGED; a++)
      if(in.k.lastKey[a].code== code) {
        if(in.k.lastKey[a].timeUp) continue;
        in.k.lastKey[a].timeUp= osi.eventTime;
        in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
        found= true;
        break;
      }
    /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
    if(!found)  {
      osiKeyboard::osiKeyLog k;
      k.code= code;
      k.checked= false;
      k.timeUp= osi.eventTime;
      k.timeDown= k.timeUp- 1;            /// 1 milisecond before the keyup
      k.timeDT= 1;                        /// timeUp- timeDown
      in.k._log(k);
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
*/


// ---------------================ KEY DOWN =================---------------- //
- (void) keyDown:(NSEvent *)theEvent {
/*
  // string UCKeytranslate(b,a,c);<< this one is "very low". hopefully, everything is ok with what cocoa has as a basic.
  
  uint8 code= [theEvent keyCode];
  
  /// different vars that might be needed some day (atm, everything seems set with keyboard, tho):
  // uint32 flags= [theEvent modifierFlags];
  // int32 unicode;
  // NSString *chrs= [theEvent characters];
  // NSString *chrsNoMod= [theEvent charactersIgnoringModifiers];
  
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  osi.flags.keyPress= false;
  
  /// if it's not a repeat press event, start the keypress log
  if(![theEvent isARepeat]) {
    if(chatty) printf("Key PRESS: code[0x%x] [keyDown]\n", code);
    /// log the key
    osiKeyboard::osiKeyLog k;
    k.code= code;
    k.checked= false;
    k.timeDown= osi.eventTime;
    k.timeUp= 0;
    k.timeDT= 0;
    in.k._log(k);
    /// set the key as pressed & other needed vars
    in.k.key[code]= 128;
    in.k.keyTime[code]= osi.eventTime;
    //in.k.repeat[code]= 1;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED
  }
  
  in.k._checkKeyManip(code);     /// check if pressed keys form a manip char. if they do, manipChar is added to manip stream
  
  /// next line translates key(s) for inputText and doCommandBySelector (wich is disabled)
  */
  [self interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
}


/*
// ---------------================= KEY UP ==================-------------------
- (void) keyUp:(NSEvent *)theEvent {

  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  osi.flags.keyPress= false;
  
  uint8 code= [theEvent keyCode];
  
  if(chatty) printf("key RELEASE code[0x%x] [keyUp]\n", code);
  
  /// log the key in history
  bool found= false;
  for(short a= 0; a< MAX_KEYS_LOGGED; a++)
    if(in.k.lastKey[a].code== code) {
      if(in.k.lastKey[a].timeUp) continue;
      in.k.lastKey[a].timeUp= osi.eventTime;
      in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
      found= true;
      break;
    }
  /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
  if(!found)  {
    osiKeyboard::osiKeyLog k;
    k.code= code;
    k.checked= false;
    k.timeUp= osi.eventTime;
    k.timeDown= k.timeUp- 1;           /// 1 milisecond before the keyup
    k.timeDT= 1;                      /// timeUp- timeDown
    in.k._log(k);
  }
  
  /// set the key as pressed & other vars
  in.k.key[code]= 0;
  in.k.keyTime[code]= 0;
  //    in.k.repeat[code]= 0;     // MIGHT BE DELETED
}
*/

// -------------============== CHARACTER(s) INPUT ==============----------------
- (void)insertText:(id)string {
  uint32 unicode;

  for(short a= 0; a< [string length]; a++) {
    unicode= [string characterAtIndex: a];
    in.k._checkAndAddUnicode(unicode);      /// eventTime is already updated. insertText is RIGHT AFTER a keyDown event
  }
  
  //DISABLED [super insertText:string];  // have superclass insert it - THIS MAKES A BEEP if no one handles the text
}

// DISABLED
- (void)doCommandBySelector:(SEL)aSelector {
  // overided to disable command (key) beeps
}


///==========================================================================///
// ----------------------------- MOUSE EVENTS ------------------------------- //
///==========================================================================///
/*
// ---------------============== MOUSE MOVED ================-------------------
- (void) mouseMoved:(NSEvent *)theEvent {
  /// oldx&y, deltas, removed; now updated with each in.update() call
  in.m.x= theEvent.window.screen.frame.origin.x+ theEvent.window.frame.origin.x+ theEvent.locationInWindow.x;
  in.m.y= theEvent.window.screen.frame.origin.y+ theEvent.window.frame.origin.y+ theEvent.locationInWindow.y;
  
  if(chatty) printf("mouseMoved: x[%d] y[%d]\n", in.m.x, in.m.y);
}


// ---------------============ LEFT BUTTON DOWN =============-------------------
- (void) mouseDown:(NSEvent *)theEvent {
  osi.flags.buttonPress= true;
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  
  in.m.but[0].down= true;
  in.m.but[0].timeStart= osi.eventTime;
  if(chatty) printf("mouseDown (left button)\n");
}

// ---------------============= LEFT BUTTON UP ==============-------------------
- (void) mouseUp:(NSEvent *)theEvent {
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  osi.flags.buttonPress= false;      /// it's not accurate, needs further testing against other buttons, but...
  
  if(in.m.but[0].down) {                    /// an alt-bab might mess stuff...
    in.m.but[0].lastTimeStart= in.m.but[0].timeStart;
    in.m.but[0].lastTimeEnded= osi.eventTime;
    in.m.but[0].lastDT= in.m.but[0].lastTimeEnded- in.m.but[0].lastTimeStart;
  } else {                                /// an alt-tab? might mess stuff
    in.m.but[0].lastTimeEnded= osi.eventTime;
    in.m.but[0].lastTimeStart= osi.eventTime- 1;
    in.m.but[0].lastDT= 1;
  }
  in.m.but[0].down= false;
  if(chatty) printf("mouseUp (left button)\n");
}

// ---------------=========== RIGHT BUTTON DOWN =============-------------------
- (void) rightMouseDown:(NSEvent *)theEvent {
  osi.flags.buttonPress= true;
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  
  in.m.but[1].down= true;
  in.m.but[1].timeStart= osi.eventTime;
    if(chatty) printf("rightMouseDown\n");
}

// ---------------============ RIGHT BUTTON UP ==============-------------------
- (void) rightMouseUp:(NSEvent *)theEvent {
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  osi.flags.buttonPress= false;      /// it's not accurate, needs further testing against other buttons, but...
  
  if(in.m.but[1].down) {                    /// an alt-bab might mess stuff...
    in.m.but[1].lastTimeStart= in.m.but[1].timeStart;
    in.m.but[1].lastTimeEnded= osi.eventTime;
    in.m.but[1].lastDT= in.m.but[1].lastTimeEnded- in.m.but[1].lastTimeStart;
  } else {                                /// an alt-tab? might mess stuff
    in.m.but[1].lastTimeEnded= osi.eventTime;
    in.m.but[1].lastTimeStart= osi.eventTime- 1;
    in.m.but[1].lastDT= 1;
  }
  in.m.but[1].down= false;
  if(chatty) printf("rightMouseUp\n");
}

// ---------------========== 'OTHER' BUTTON DOWN ============-------------------
- (void) otherMouseDown:(NSEvent *)theEvent {
  int b= (int)theEvent.buttonNumber; /// this seems ok: 2= middle; 3, 4= extra butotns
  
  osi.flags.buttonPress= true;
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  
  in.m.but[b].down= true;
  in.m.but[b].timeStart= osi.eventTime;
  if(chatty) printf("otherMouseDown [%d]\n", b);
}

// ---------------=========== 'OTHER' BUTTON UP =============-------------------
- (void) otherMouseUp:(NSEvent *)theEvent {

  int b= (int)theEvent.buttonNumber; /// this seems ok. 2=middle; 3, 4= extra butotns
  osi.eventTime= osi.present/ 1000000;       /// event time; using osi.present time - not gonna call osi.getMillisecs for 100% precision  
  osi.flags.buttonPress= false;      /// it's not accurate, needs further testing against other buttons, but...
  
  if(in.m.but[b].down) {                    /// an alt-bab might mess stuff...
    in.m.but[b].lastTimeStart= in.m.but[b].timeStart; 
    in.m.but[b].lastTimeEnded= osi.eventTime;
    in.m.but[b].lastDT= in.m.but[b].lastTimeEnded- in.m.but[b].lastTimeStart;
  } else {                                /// an alt-tab? might mess stuff
    in.m.but[b].lastTimeEnded= osi.eventTime;
    in.m.but[b].lastTimeStart= osi.eventTime- 1;
    in.m.but[b].lastDT= 1;
  }
  in.m.but[b].down= false;
  if(chatty) printf("otherMouseUp [%d]\n", b);
}

// ---------------============== MOUSE WHEEL ================-------------------
- (void) scrollWheel:(NSEvent *)theEvent {
  if(theEvent.scrollingDeltaY> 0)
    in.m._twheel++;
  else
    in.m._twheel--;
  if(chatty) printf("scrollWheel [%d]\n", theEvent.scrollingDeltaY> 0? 1: -1);
}

// -= rest of mouse moving event types, will all call the main moving function =-
- (void) mouseDragged:(NSEvent *)theEvent {
  [self mouseMoved:theEvent];
}

- (void) rightMouseDragged:(NSEvent *)theEvent {
  [self mouseMoved:theEvent];
}

- (void) otherMouseDragged:(NSEvent *)theEvent {
  [self mouseMoved:theEvent];
}
*/
@end




///=================================================================///
// -------------------->>> PROCESS MSGS <<<------------------------- //
///=================================================================///
bool _processMSG(void) {
  #ifdef OSI_BE_CHATTY
  bool chatty= false;    /// debug texts
  #endif

  bool ret= false;      /// return value - true if a message was processed, false if no message was in queue

  NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

  /// set flags down
  osi.flags.windowResized= false;
  osi.flags.windowMoved= false;
  osi.flags.windowMinimized= false;

  osi.eventTime= osi.present/ 1000000;

  while(1) {

    // [pool release];
    // pool= [[NSAutoreleasePool alloc] init];

    NSEvent *event= [NSApp nextEventMatchingMask: NSAnyEventMask
           untilDate: [NSDate distantPast]
           inMode: NSDefaultRunLoopMode
           dequeue: YES];

    if(event== nil)
      break;

    

    ///==========================================================================///
    // ----------------------------- MOUSE EVENTS ------------------------------- //
    ///==========================================================================///

    // ---------------============== MOUSE MOVED ================-------------------
    // -= rest of mouse moving event types, will all call the main moving function =-
    if([event type]== NSMouseMoved ||
       [event type]== NSLeftMouseDragged ||
       [event type]== NSRightMouseDragged ||
       [event type]== NSOtherMouseDragged) {
      /// oldx&y, deltas, removed; now updated with each in.update() call
      in.m.x= event.window.screen.frame.origin.x+ event.window.frame.origin.x+ event.locationInWindow.x;
      #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
      in.m.y= event.window.screen.frame.origin.y+ event.window.frame.origin.y+ event.locationInWindow.y;
      #endif
      #ifdef OSI_USE_ORIGIN_TOP_LEFT
      in.m.y= event.window.screen.frame.origin.y+ event.window.frame.origin.y+ event.locationInWindow.y;
      in.m.y= osi.display.vyMax- in.m.y;
      #endif
      #ifdef OSI_BE_CHATTY
      // spam if(chatty) printf("mouseMoved: x[%d] y[%d]\n", in.m.x, in.m.y);
      #endif
    }
    // ---------------============ LEFT BUTTON DOWN =============-------------------
    else if([event type]== NSLeftMouseDown) {
      osi.flags.buttonPress= true;
  
      in.m.but[0].down= true;
      in.m.but[0].timeStart= osi.eventTime;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("mouseDown (left button)\n");
      #endif
    }
    // ---------------============= LEFT BUTTON UP ==============-------------------
    else if([event type]== NSLeftMouseUp) {
      osi.flags.buttonPress= false;      /// it's not accurate, needs further testing against other buttons, but...
  
      if(in.m.but[0].down) {                    /// an alt-bab might mess stuff...
        in.m.but[0].lastTimeStart= in.m.but[0].timeStart;
        in.m.but[0].lastTimeEnded= osi.eventTime;
        in.m.but[0].lastDT= in.m.but[0].lastTimeEnded- in.m.but[0].lastTimeStart;
      } else {                                  /// an alt-tab? might mess stuff
        in.m.but[0].lastTimeEnded= osi.eventTime;
        in.m.but[0].lastTimeStart= osi.eventTime- 1;
        in.m.but[0].lastDT= 1;
      }
      in.m.but[0].down= false;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("mouseUp (left button)\n");
      #endif
    }
    
      
    // ---------------=========== RIGHT BUTTON DOWN =============-------------------
    else if([event type]== NSRightMouseDown) {
      osi.flags.buttonPress= true;
  
      in.m.but[1].down= true;
      in.m.but[1].timeStart= osi.eventTime;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("rightMouseDown\n");
      #endif
    }
    // ---------------============ RIGHT BUTTON UP ==============-------------------
    else if([event type]== NSRightMouseUp) {
      osi.flags.buttonPress= false;      /// it's not accurate, needs further testing against other buttons, but...
  
      if(in.m.but[1].down) {                    /// an alt-bab might mess stuff...
        in.m.but[1].lastTimeStart= in.m.but[1].timeStart;
        in.m.but[1].lastTimeEnded= osi.eventTime;
        in.m.but[1].lastDT= in.m.but[1].lastTimeEnded- in.m.but[1].lastTimeStart;
      } else {                                /// an alt-tab? might mess stuff
        in.m.but[1].lastTimeEnded= osi.eventTime;
        in.m.but[1].lastTimeStart= osi.eventTime- 1;
        in.m.but[1].lastDT= 1;
      }
      in.m.but[1].down= false;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("rightMouseUp\n");
      #endif
    }

    // ---------------========== 'OTHER' BUTTON DOWN ============-------------------
    else if([event type]== NSOtherMouseDown) {
      int b= (int)event.buttonNumber; /// this seems ok: 2= middle; 3, 4= extra butotns
  
      osi.flags.buttonPress= true;
  
      in.m.but[b].down= true;
      in.m.but[b].timeStart= osi.eventTime;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("otherMouseDown [%d]\n", b);
      #endif
    }
    // ---------------=========== 'OTHER' BUTTON UP =============-------------------
    else if([event type]== NSOtherMouseUp) {
      int b= (int)event.buttonNumber; /// this seems ok. 2=middle; 3, 4= extra butotns
      osi.flags.buttonPress= false;      /// it's not accurate, needs further testing against other buttons, but...
  
      if(in.m.but[b].down) {                    /// an alt-bab might mess stuff...
        in.m.but[b].lastTimeStart= in.m.but[b].timeStart; 
        in.m.but[b].lastTimeEnded= osi.eventTime;
        in.m.but[b].lastDT= in.m.but[b].lastTimeEnded- in.m.but[b].lastTimeStart;
      } else {                                  /// an alt-tab? might mess stuff
        in.m.but[b].lastTimeEnded= osi.eventTime;
        in.m.but[b].lastTimeStart= osi.eventTime- 1;
        in.m.but[b].lastDT= 1;
      }
      in.m.but[b].down= false;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("otherMouseUp [%d]\n", b);
      #endif
    }
    // ---------------============== MOUSE WHEEL ================-------------------
    else if([event type]== NSScrollWheel) {
      if(event.scrollingDeltaY> 0)
        in.m._twheel++;
      else
        in.m._twheel--;
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("scrollWheel [%d]\n", event.scrollingDeltaY> 0? 1: -1);
      #endif
    }





    ///==========================================================================///
    // --------------------------- KEYBOARD EVENTS ------------------------------ //
    ///==========================================================================///

    // -----------============= caps/shift/ctrl/alt/etc =============------------ //
    else if([event type]== NSFlagsChanged) {
      // find out all flagsChanged!!!!!!!!!!
  
      ulong flags= [event modifierFlags];
      uint8 code= [event keyCode];
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("event!!\n");
      #endif
  
      //MUST TEST THIS, it is INPOSSIBLE OTHERWISE...
      /// caps lock
      uint8 press= 0;
      if(code== in.Kv.capslock) {
        if(flags& NSAlphaShiftKeyMask) {
          press= 128;
          in.k.capsLock= true;  // MUST TEST
        } else {
          in.k.capsLock= false; // not shure about this

        }
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("capsLock[%s]\n", in.k.capsLock? "true": "false");
        #endif
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
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("key PRESS code[0x%x] [flagsChanged]\n", code);
        #endif
    
        /// log the key
        osiKeyboard::osiKeyLog k;
        k.code= code;
        k.checked= false;
        k.timeDown= osi.eventTime;
        k.timeUp= 0;
        k.timeDT= 0;
        in.k._log(k);
        /// set the key as pressed & other needed vars
        in.k.key[code]= 128;
        in.k.keyTime[code]= osi.eventTime;
        //in.k.repeat[code]= 128;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED
    
      // it's a KEY DEPRESS
      } else {
        osi.flags.keyPress= false;
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("key RELEASE code[0x%x] [flagsChanged]\n", code);
        #endif
    
        /// log the key in history
        bool found= false;
        for(short a= 0; a< MAX_KEYS_LOGGED; a++)
          if(in.k.lastKey[a].code== code) {
            if(in.k.lastKey[a].timeUp) continue;
            in.k.lastKey[a].timeUp= osi.eventTime;
            in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
            found= true;
            break;
          }
        /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
        if(!found)  {
          osiKeyboard::osiKeyLog k;
          k.code= code;
          k.checked= false;
          k.timeUp= osi.eventTime;
          k.timeDown= k.timeUp- 1;            /// 1 milisecond before the keyup
          k.timeDT= 1;                        /// timeUp- timeDown
          in.k._log(k);
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
    // ---------------================ KEY DOWN =================---------------- //
    else if([event type]== NSKeyDown) {
      // string UCKeytranslate(b,a,c);<< this one is "very low". hopefully, everything is ok with what cocoa has as a basic.
  
      uint8 code= [event keyCode];
  
      /// different vars that might be needed some day (atm, everything seems set with keyboard, tho):
      // uint32 flags= [event modifierFlags];
      // int32 unicode;
      // NSString *chrs= [event characters];
      // NSString *chrsNoMod= [event charactersIgnoringModifiers];
  
      osi.flags.keyPress= true;
  
      /// if it's not a repeat press event, start the keypress log
      if(![event isARepeat]) {
        #ifdef OSI_BE_CHATTY
        if(chatty) printf("Key PRESS: code[0x%x] [keyDown]\n", code);
        #endif
        /// log the key
        osiKeyboard::osiKeyLog k;
        k.code= code;
        k.checked= false;
        k.timeDown= osi.eventTime;
        k.timeUp= 0;
        k.timeDT= 0;
        in.k._log(k);
        /// set the key as pressed & other needed vars
        in.k.key[code]= 128;
        in.k.keyTime[code]= osi.eventTime;
        //in.k.repeat[code]= 1;                  /// a new key press, sets repeat to 1  // MIGHT BE DELETED

        /// insert lock handling
        if(code== in.Kv.insert)
          in.k.insertLock= (in.k.insertLock? false: true);
      }
  
      in.k._checkKeyManip(code);     /// check if pressed keys form a manip char. if they do, manipChar is added to manip stream
    }
    // ---------------================= KEY UP ==================-------------------
    else if([event type]== NSKeyUp) {
      osi.flags.keyPress= false;
  
      uint8 code= [event keyCode];
      #ifdef OSI_BE_CHATTY
      if(chatty) printf("key RELEASE code[0x%x] [keyUp]\n", code);
      #endif
  
      /// log the key in history
      bool found= false;
      for(short a= 0; a< MAX_KEYS_LOGGED; a++)
        if(in.k.lastKey[a].code== code) {
          if(in.k.lastKey[a].timeUp) continue;
          in.k.lastKey[a].timeUp= osi.eventTime;
          in.k.lastKey[a].timeDT= in.k.lastKey[a].timeUp- in.k.lastKey[a].timeDown;
          found= true;
          break;
        }
      /// in case the key was not found in history, add a hist-log with insta-keydown-keyup
      if(!found)  {
        osiKeyboard::osiKeyLog k;
        k.code= code;
        k.checked= false;
        k.timeUp= osi.eventTime;
        k.timeDown= k.timeUp- 1;           /// 1 milisecond before the keyup
        k.timeDT= 1;                      /// timeUp- timeDown
        in.k._log(k);
      }
  
      /// set the key as pressed & other vars
      in.k.key[code]= 0;
      in.k.keyTime[code]= 0;
      //    in.k.repeat[code]= 0;     // MIGHT BE DELETED
      continue;
    }
/*
    else if([event type]== ) {
    }

    else if([event type]== ) {
    }

    else if([event type]== ) {
    }
*/



    
    [NSApp sendEvent: event];
    [NSApp updateWindows];
    
    ret= true;
	}
  
	[pool release];	
  return ret;
}













///--------------------===============-----------------///
// ---------=========== OScocoa CLASS =========-------- //
///--------------------===============-----------------///

//NSImage *_createNSImage(cchar *fileName);


osiCocoa::osiCocoa() {
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
  
  /// program icon - THIS IS KINDA SKETCHY, SETTING static TO FUNC BY FORCE
  osiWindow _tmp; _tmp.setIcon(OSI_PROGRAM_ICON);
  
  [NSApp finishLaunching]; // <<<<---- after finish launching settings from here 
  
  /// this seems to disable the crappy momentum scrolling. Thing is, there won't be any scrolling involved, tho
  NSDictionary *appDefaults = [NSDictionary dictionaryWithObject:@"NO" forKey:@"AppleMomentumScrollSupported"];
  [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];

  // this doesn't seem to work atm - SDL REF; maybe add these menu items after the window is created?
  /// Create the window menu
  NSMenu *windowMenu= [[NSMenu alloc] initWithTitle:@"Window"];
  [windowMenu addItemWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
  [windowMenu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
  [NSApp setWindowsMenu:windowMenu];
  [windowMenu release];
  

  [NSApp activateIgnoringOtherApps: YES];
  [pool release];
}

osiCocoa::~osiCocoa() {
}


// sets program path, and updates osi.path
void osiCocoa::setProgramPath() {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];

	NSString *path;
	path= [[NSBundle mainBundle] bundlePath];
	[[NSFileManager defaultManager] changeCurrentDirectoryPath: path];
  osi.path= [path UTF8String];
  
	[pool release];
}


///================================================================///
// -------------------->>> CREATE WINDOW <<<----------------------- //
///================================================================///
bool osiCocoa::createWindow(osiWindow *w, const char *iconFile) {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
  
  bool ret= true;
  
  osiMacWindow *win= NULL;

  // REMOVED 1
  //osiMacGLview *view= NULL;
  
  /// change monitor resolution
  if(w->mode== 2)
    osi.display.changeRes(w->monitor, w->dx, w->dy, w->freq);
  
  /// window size
  NSRect contRect;
  #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
  contRect= NSMakeRect(w->x0, w->y0, w->dx, w->dy);
  #endif
  #ifdef OSI_USE_ORIGIN_TOP_LEFT
  contRect= NSMakeRect(w->x0, osi.display.vyMax- (w->y0+ w->dy- 1), w->dx, w->dy);
  #endif

  /// window style
  uint winStyle= 0;
	if(w->mode== 1)                                 /// windowed style
    winStyle=
      NSTitledWindowMask|
      NSClosableWindowMask|
      NSMiniaturizableWindowMask|
      NSResizableWindowMask;
  if(w->mode== 3 || w->mode== 2 || w->mode== 4)   /// fullscreen / fullscreen window style / extended on all monitors
    winStyle=
    //      NSTitledWindowMask|
      NSClosableWindowMask|
      NSMiniaturizableWindowMask|
      NSBorderlessWindowMask;
  
  /// window alloc & init
  win= [osiMacWindow alloc];
  [win initWithContentRect: contRect
                 styleMask: winStyle
                   backing: NSBackingStoreBuffered
                     defer: NO];
  //screen:X]; /// a screen number can be placed here <<<
  
  // see http://www.mikeash.com/pyblog/nsopenglcontext-and-one-shot.html
  /// prevent window deletion when hidden
  [win setOneShot:NO];
  
  /* SCRAPE INCOMING
  /// pixel format
  // THIS MIGHT NEED MORE CUSTOMIZATION
  NSOpenGLPixelFormat *format;
  NSOpenGLPixelFormatAttribute formatAttrib[]= {
    NSOpenGLPFAWindow,
    NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)32,
    NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
    (dblBuffer? NSOpenGLPFADoubleBuffer: 0u),                  /// double buffer window
    0
  };
  
  format= [NSOpenGLPixelFormat alloc];
  [format initWithAttributes: formatAttrib];
  */





  /* REMOVED 2
  // assign/create a renderer
  osi.assignRenderer(w);
 
  /// OpenGL view alloc & init
  view= [osiMacGLview alloc];
  contRect= NSMakeRect(0, 0, w->dx, w->dy);
  [view initWithFrame: contRect pixelFormat: (NSOpenGLPixelFormat *)w->glr->_pixelFormat];
  //[view initWithFrame: contRect pixelFormat: format];
  
  /// set the view's renderer
  [view setOpenGLContext: (NSOpenGLContext *)w->glr->glContext];
  
  /// rest of window settings
  [win setContentView: view];
  [win makeFirstResponder: view];
  */





  /// windowed mode
  if(w->mode== 1) {
    [win setTitle:[NSString stringWithUTF8String: (cchar *)w->name.d]];
    [win setLevel:kCGNormalWindowLevel];
  /// fullscreen / fullscreen window
  } else if(w->mode== 2|| w->mode== 3|| w->mode== 4) {
    [win setLevel:CGShieldingWindowLevel()];
    [win setOpaque:YES];              /// no clue what this has in advantages, but documentation sets this; no other info found on oficial doc
    
    //        if(w->mode== 2)
    //      [win setHidesOnDeactivate:YES]; /// using this for fullscreen... TEST IF CHANGING RES IS OK WITH JUST MOVING THE WINDOW TO THE BACK
  }
  



  /* REMOVED 3
  /// set renderer's view (doc writes there have to be both way set)
  [(NSOpenGLContext *)w->glr->glContext setView: view];
  */
  



  //see https://developer.apple.com/library/mac/documentation/graphicsimaging/conceptual/OpenGL-MacProgGuide/EnablingOpenGLforHighResolution/EnablingOpenGLforHighResolution.html#//apple_ref/doc/uid/TP40001987-CH1001-SW5  
  
  /// osiWindow connection
  w->_win= win;


  // REMOVED 4
  //w->_view= view;

  w->isCreated= true;

  if(iconFile)
    w->setIcon(iconFile);
  
  [win makeKeyAndOrderFront: nil];
  
  /* this crashes cocoa, dunno why
  if(w== primWin)
    [win makeMainWindow];
   */
  
  //  [NSApp activateIgnoringOtherApps: YES]; // THIS WAS MOVED TO CONSTRUCTOR

  
  /* REMOVED 5
  osi.glMakeCurrent(w->glr, w);
  
  
  w->glr->checkExt();
  */
  // no need, right????? osi.getExtensions();              // WIP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  [pool release];

  return ret;
}






bool osiCocoa::glCreateWindow(osiWindow *w, const char *iconFile) {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
  NSRect contRect;
  
  osiMacGLview *view= NULL;

  // assign/create a renderer
  osi.glAssignRenderer(w);
 
  /// OpenGL view alloc & init
  view= [osiMacGLview alloc];
  contRect= NSMakeRect(0, 0, w->dx, w->dy);
  [view initWithFrame: contRect pixelFormat: (NSOpenGLPixelFormat *)((osiGlRenderer *)w->renderer)->_pixelFormat];
  //[view initWithFrame: contRect pixelFormat: format];
  
  /// set the view's renderer
  [view setOpenGLContext: (NSOpenGLContext *)((osiGlRenderer *)w->renderer)->glContext];
  
  /// rest of window settings
  [(osiMacWindow *)w->_win setContentView: view];
  [(osiMacWindow *)w->_win makeFirstResponder: view];


  
  /// set renderer's view (doc writes there have to be both way set)
  [(NSOpenGLContext *)((osiGlRenderer *)w->renderer)->glContext setView: view];
  
  w->_view= view;
  
  osi.glMakeCurrent((osiGlRenderer *)w->renderer, w);
  ((osiGlRenderer *)w->renderer)->checkExt();

  [pool release];
  
  return true;
}






// SPLASH WINDOW ======================------------------------
///============================================================

bool osiCocoa::createSplashWindow(osiWindow *w, uint8_t *bitmap, int dx, int dy, int bpp, int bpc) {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
  
  bool ret= true;
  
  osiMacWindow *win= NULL;
  osiMacGLview *view= NULL;      // THIS MIGHT NEED TO BE ANOTHER NORMAL NSView
  
  /// window size
  NSRect contRect;
  #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
  contRect= NSMakeRect(w->x0, w->y0, w->dx, w->dy);
  #endif
  contRect= NSMakeRect(w->x0, osi.display.vyMax- (w->y0+ w->dy- 1), w->dx, w->dy);
  #ifdef OSI_USE_ORIGIN_TOP_LEFT

  #endif

  /// window style
  uint winStyle= 
    NSBorderlessWindowMask|
    NSTexturedBackgroundWindowMask; // maybe

  /// window alloc & init
  win= [osiMacWindow alloc];
  [win initWithContentRect: contRect
                 styleMask: winStyle
                   backing: NSBackingStoreBuffered
                     defer: NO];
  //screen:X]; /// a screen number can be placed here <<<
  
  // see http://www.mikeash.com/pyblog/nsopenglcontext-and-one-shot.html
  /// prevent window deletion when hidden
  [win setOneShot:NO];

  /// OpenGL view alloc & init
  view= [osiMacGLview alloc];
  contRect= NSMakeRect(0, 0, w->dx, w->dy);
  [view initWithFrame: contRect];
  
  
  /// rest of window settings
  [win setContentView: view];
  [win makeFirstResponder: view];
  [win setTitle:[NSString stringWithUTF8String: (cchar *)w->name.d]];
  [win setLevel:kCGNormalWindowLevel];
  [win setBackgroundColor:[NSColor clearColor]];
  [win setOpaque:NO];

  //see https://developer.apple.com/library/mac/documentation/graphicsimaging/conceptual/OpenGL-MacProgGuide/EnablingOpenGLforHighResolution/EnablingOpenGLforHighResolution.html#//apple_ref/doc/uid/TP40001987-CH1001-SW5  

  /// osiWindow connection
  w->_win= win;
  w->_view= view;
  w->isCreated= true;
  
  /// window background image
  if(bitmap && dx && dy && bpp && bpc) {  
    /// prepare the loads of vars needed
    CGColorSpaceRef cspace= CGColorSpaceCreateDeviceRGB();
    CGDataProviderRef dProvider= CGDataProviderCreateWithData(NULL, bitmap, dx* dy* (bpp/ 8), NULL);
    CGImageRef cgdata= CGImageCreate(dx, dy, bpc, bpp, dx* (bpp/ 8), cspace, (bpp==24? kCGBitmapByteOrderDefault:kCGImageAlphaLast), dProvider, NULL, false, kCGRenderingIntentDefault);
    NSImage *nsi= [NSImage alloc];
    if(cgdata) [nsi initWithCGImage:cgdata size: NSZeroSize];
    else error.simple("osiCocoa::createSplashWindow():Can't create cgdata");
    
    // set window background image
    if(cgdata && nsi) {
      [view setWantsLayer:YES];
      [view layer].contents= nsi;
    } else error.simple("osiCocoa::createSplashWindow():nsi or cgdata failed");

    /// release vars allocated
    [nsi release];
    CGImageRelease(cgdata);
    CGDataProviderRelease(dProvider);
    CGColorSpaceRelease(cspace);
  }
  
  [win makeKeyAndOrderFront: nil];


  [pool release];

  return ret;
}


void osiCocoa::delWindow(osiWindow* w) {
  if(!w) return;
  
  if(w->_view)
    [(osiMacGLview *)w->_view release];
  w->_view= nil;
  
  if(w->_win)
    [(osiMacWindow *)w->_win release];
  w->_win= nil;
}



str8 osiCocoa::getCmdLine() {
  
  NSArray *args = [[NSProcessInfo processInfo] arguments];
  // use -objectAtIndex: to obtain an element of the array
  // and -count to obtain the number of elements in the array
  
  ulong n= [args count];
  str8 ret;
  for(int a= 0; a< n; a++) {
    NSString *s= [args objectAtIndex:a];

    if(a) ret+= " ";
    ret+= [s UTF8String];
  }
  
  return ret;
}


void osiCocoa::setIcon(uint8_t *bitmap, int dx, int dy, int bpp, int bpc) {
  /// prepare the loads of vars needed
  CGColorSpaceRef cspace= CGColorSpaceCreateDeviceRGB();
  CGDataProviderRef dProvider= CGDataProviderCreateWithData(NULL, bitmap, dx* dy* (bpp/ 8), NULL);
  CGImageRef cgdata= CGImageCreate(dx, dy, bpc, bpp, dx* (bpp/ 8), cspace, (bpp==24? kCGBitmapByteOrderDefault:kCGImageAlphaLast), dProvider, NULL, false, kCGRenderingIntentDefault);
  NSImage *nsi= [NSImage alloc];
  [nsi initWithCGImage:cgdata size: NSZeroSize];
  
  // change application icon
  [NSApp setApplicationIconImage:nsi];
  
  /// release vars allocated
  [nsi release];
  CGImageRelease(cgdata);
  CGDataProviderRelease(dProvider);
  CGColorSpaceRelease(cspace);
}



void osiCocoa::getWindowSize(osiWindow *w, int *dx, int *dy) {
  NSRect rect;
  rect= [(osiMacGLview *)w->_view frame];
  *dx= rect.size.width;
  *dy= rect.size.height;
}


void osiCocoa::setWindowSize(osiWindow *w, int dx, int dy) {
  NSRect rect;
  rect.size.width= dx;
  rect.size.height= dy;
  rect.origin.x= w->x0;
  #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
  rect.origin.y= (w->y0+ w->dy)- dy;
  #endif
  #ifdef OSI_USE_ORIGIN_TOP_LEFT
  rect.origin.y= osi.display.vyMax- (w->y0+ dy- 1);
  #endif
  [(osiMacWindow *)w->_win setFrame:rect display:false];
}


void osiCocoa::setWindowPos(osiWindow *w, int x, int y) {
  NSPoint p;
  p.x= (float)x;
  #ifdef OSI_USE_ORIGIN_BOTTOM_LEFT
  p.y= (float)(y- w->dy);
  #endif
  #ifdef OSI_USE_ORIGIN_TOP_LEFT
  p.y= (float)(osi.display.vyMax- (y+ w->dy- 1));
  #endif
  [(osiMacWindow *)w->_win setFrameOrigin: p];
}


void osiCocoa::setWindowName(osiWindow *w, cchar *name) {
  [((osiMacWindow *)w->_win) setTitle:[NSString stringWithUTF8String: (char *)w->name.d]];
}

void osiCocoa::setWindowHidden(osiWindow *w) {
  [(osiMacWindow *)w->_win makeKeyAndOrderFront:nil];
}

void osiCocoa::setWindowShown(osiWindow *w) {
  [(osiMacWindow *)w->_win orderOut:nil];
}








// OPENGL stuff ===========---------
///=================================

void osiCocoa::swapBuffers(osiWindow *w) {
  [[(osiMacGLview *)w->_view openGLContext] flushBuffer];
}


bool osiCocoa::makeCurrent(osiGlRenderer *r, osiWindow *w) {
  if(w && r) {
    NSOpenGLContext *c= (NSOpenGLContext *)r->glContext;
    osiMacGLview *v= (osiMacGLview *)w->_view;

    if([v openGLContext] != c)
      [v setOpenGLContext: c];

    if([c view] != v)
      [c setView: v];

    // method 1
    // [[v openGLContext] makeCurrentContext];

    // method 2
    if(!CGLSetCurrentContext((CGLContextObj)[c CGLContextObj])) return true;
    else                                                        return false;

    //[[(osiMacGLview *)w->_view openGLContext] makeCurrentContext]; // old way
  } else {
    CGLSetCurrentContext(NULL);
    return true;
  }
  return true;
  // [[(osiMacGLview *)w->_view openGLContext] makeCurrentContext]; // old way
}

/// creates a pixel format with current osi settings
bool osiCocoa::createPixelFormat(osiGlRenderer *r, uint32_t oglDisplayMask) {
  // https://developer.apple.com/library/mac/documentation/GraphicsImaging/Reference/CGL_OpenGL/index.html#//apple_ref/c/func/CGLChoosePixelFormat

  // pixel buffers: NSOpenGLPixelBuffer
  //https://developer.apple.com/library/mac/documentation/Cocoa/Reference/ApplicationKit/Classes/NSOpenGLPixelBuffer_Class/index.html#//apple_ref/occ/cl/NSOpenGLPixelBuffer



  // no values were played with, i don't have the necesary... mac. this emulator can do so much
  CGLPixelFormatAttribute attr[50]; int n= 0;
  
  attr[n++]= kCGLPFAMinimumPolicy;  /// buffer sizes MUST be at least what was supplied
  //attr[n++]= kCGLPFAMaximumPolicy;  /// buffer sizes MUST be no bigger then what was supplied
  //attr[n++]= kCGLPFAClosestPolicy;  /// buffer closest to the requested size is preferred

  // !!! if(osi.settings.renderer.debug) attr[n++]= kCGLPFAAllRenderers; < THIS OPTION LETS DEBUG RENDERERS TO BE USED, BUT HOW YOU KNOW A RENDERER IS IN DEBUG MODE?

  /// monitors that renderer must be able to work on
  if(oglDisplayMask) { attr[n++]= kCGLPFADisplayMask;  attr[n++]= (CGLPixelFormatAttribute)oglDisplayMask; }

  if(osi.settings.glRenderer.legacyCompatibility) { /// use of compatibility extension. this val can also be kCGLOGLPVersion_3_2_Core (probly the last ver)
    attr[n++]= kCGLPFAOpenGLProfile; attr[n++]= (CGLPixelFormatAttribute)kCGLOGLPVersion_Legacy; }
  
  if(osi.settings.pixelFormat.dblBuffer) attr[n++]= kCGLPFADoubleBuffer;    /// double buffer

  attr[n++]= kCGLPFANoRecovery;      /// Normally, if an accelerated renderer fails due to lack of resources, OpenGL automatically switches to another renderer

  if(osi.settings.pixelFormat.onlyAccelerated)
    attr[n++]= kCGLPFAAccelerated;     /// only hardware accelerated renderers are considered

  /// the red/green/blue sizes are not present on Macs, tweak must be made (if rgb vals are present, they are used to determine c size)
  int cSize= osi.settings.pixelFormat.redSize+ osi.settings.pixelFormat.greenSize+ osi.settings.pixelFormat.blueSize;
  if(osi.settings.pixelFormat.colorSize> cSize) cSize= osi.settings.pixelFormat.colorSize;
  attr[n++]= kCGLPFAColorSize;    attr[n++]= (CGLPixelFormatAttribute)cSize; /// color size

  attr[n++]= kCGLPFAAlphaSize;    attr[n++]= (CGLPixelFormatAttribute)osi.settings.pixelFormat.alphaSize;    /// alpha size
  attr[n++]= kCGLPFADepthSize;    attr[n++]= (CGLPixelFormatAttribute)osi.settings.pixelFormat.depthSize;    /// depth size
  attr[n++]= kCGLPFAStencilSize;  attr[n++]= (CGLPixelFormatAttribute)osi.settings.pixelFormat.stencilSize;  /// stencil size
  

  attr[n++]= kCGLPFASampleBuffers; attr[n++]= (CGLPixelFormatAttribute)osi.settings.pixelFormat.sampleBuffers; /// The number of multisample buffers. Typically, the value is 0 if no multisample buffer exists or 1.
  attr[n++]= kCGLPFASamples;       attr[n++]= (CGLPixelFormatAttribute)osi.settings.pixelFormat.samples;     /// The number of samples per multisample buffer
  //attr[n++]= kCGLPFASampleAlpha;    /// request alpha filtering when multisampling

  //attr[n++]= kCGLPFASupersample;    /// prefer supersampling
  //attr[n++]= kCGLPFARendererID;                /// select a renderer id, from vendor ids and such (nvidia ati soft). check web
  //attr[n++]= kCGLPFABackingStore;              /// asures a color back buffer (for swap buffers)
  //attr[n++]= kCGLPFADisplayMask; attr[n++]= 0i;/// on what displays this renderer can act (check quartz for numbers)
  //attr[n++]= kCGLPFAAllowOfflineRenderers;     /// renderers that are not connected to displays are considered
  //attr[n++]= kCGLPFAAcceleratedCompute;        /// only renderers that render to a hardware device that is capable of OpenCL processing are considered
  
  attr[n]= (CGLPixelFormatAttribute)0;
  
  /// core oGL pixel format object
  CGLPixelFormatObj pf;
  GLint npix;
  CGLChoosePixelFormat(attr, &pf, &npix);
  if(!pf) {
    error.simple("CreateRenderer: pixel format failed to create with specified attributes");
    return false;
  }
  
  /// osiRenderer NS pixelFormat object (do you even object? i heard you like to object the object in objec of object)
  NSOpenGLPixelFormat *pfNS= [NSOpenGLPixelFormat alloc];
  [pfNS initWithCGLPixelFormatObj:pf];

  r->_pixelFormat= pfNS;

  return true;
}


bool osiCocoa::createContext(osiGlRenderer *r, uint32_t oglDisplayMask) {

  /// create the pixel format that this renderer will function on
  if(!createPixelFormat(r, oglDisplayMask))
    return false;

  /// context sharing group
  CGLContextObj sh= nil;
  if(osi.settings.glRenderer.shareGroup)
    sh= (CGLContextObj)[(NSOpenGLContext *)osi.settings.glRenderer.shareGroup->glContext CGLContextObj];
  
  /// create the CGL context
  CGLContextObj cCGL;
  CGLPixelFormatObj pfCGL= (CGLPixelFormatObj)[(NSOpenGLPixelFormat *)r->_pixelFormat CGLPixelFormatObj];
  if(CGLCreateContext(pfCGL, sh, &cCGL)) {
    #ifdef OSI_BE_CHATTY
    printf("CGL CREATE CONTEXT FAILED !!!!!!!!!!!");
    #endif
    error.simple("CreateRenderer(): CGLCreateContext FAILED");
    delPixelFormat(r->_pixelFormat);
    r->_pixelFormat= NULL;
    return false;
  }
  
  /// create cocoa OpenGL renderer object
  NSOpenGLContext *context= [NSOpenGLContext alloc];
  [context initWithCGLContextObj:cCGL];
  r->glContext= context;
  
  return true;
}

   
void osiCocoa::delContext(void *context) {
  NSOpenGLContext *cNS= (NSOpenGLContext *)context;
  CGLContextObj cCGL= (CGLContextObj)[cNS CGLContextObj];
  CGLReleaseContext(cCGL);
  [cNS dealloc];
}

void osiCocoa::delPixelFormat(void *pf) {
  CGLReleasePixelFormat((CGLPixelFormatObj)[(NSOpenGLPixelFormat *)pf CGLPixelFormatObj]);
  [(NSOpenGLPixelFormat *)pf dealloc];
}

void osiCocoa::delNSImage(void *i) {
  [(NSImage *)i dealloc];
}

void osiCocoa::setRendererVertSync(osiGlRenderer *r, bool vertSync) {
  CGLContextObj cCGL= (CGLContextObj)[(NSOpenGLContext *)r->glContext CGLContextObj];
  GLint v= vertSync? 1: 0;
  CGLSetParameter(cCGL, kCGLCPSwapInterval, &v);
}





void osiCocoa::sleep(int ms) {
  if(ms> 0) {
    double sec;
    sec= (double)ms/ 1000.0;
    [NSThread sleepForTimeInterval: sec];
  }
}


// INTERNAL- monitor name - and ofc, a func just got deprecated !
bool osiCocoa::displayName(uint32 id, str8 *out) {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
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
  
  //[deviceInfo release];
  [pool release];
  return ret;
}


bool osiCocoa::displayGPU(uint32 id, str8 *out) {
  if(!out) { printf("osiCocoa::displayGPU(): null string. aborting."); return false; }

  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
  // !!!!! these seem to not be listed anywhere - must test
  // kIODisplayEDIDKey special key tied to the monitor
  // kIOFramebufferInfoKey - this might be it
  // but, there should be a 'location' of the monitor (probly the framebufferInfoKey)
  // !!!!!
  
  
  out->delData();
  
  NSDictionary *deviceInfo= (NSDictionary *)IODisplayCreateInfoDictionary(CGDisplayIOServicePort(id), kIODisplayOnlyPreferredName);
  NSString *locInfo= [deviceInfo objectForKey: [NSString stringWithUTF8String:kIODisplayLocationKey]];
  if(!locInfo) { [deviceInfo release]; return false; }
  
  /// out will hold the whole location info, which has monitor info too (i think) 
  *out= [locInfo UTF8String];
  if(!*out) { [pool release]; return false; }
  
  /// wipe the last 2 items in the location info, hopefully the remaining info is grcard info
  int n= 2;
  while(n) {
    if(Str::utf8to32(out->pointUnicode(out->nrUnicodes- 1))== '/')
      n--;
    
    out->operator-=(1);
  }
  
  //[locInfo release];
  //[deviceInfo release];
  [pool release];
  return true;
}


/// pastebin=clipboard - copy/paste operations
void osiCocoa::setPastebin(uint8_t *in_text) {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];

  NSString *nss= [NSString stringWithUTF8String:(const char *)in_text];
  NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
  
  // MIGHT BE NEEDED, NOT SURE [pasteBoard declareTypes:[NSArray arrayWithObjects:NSStringPboardType, nil] owner:nil];
  [pasteBoard clearContents];
  [pasteBoard setString:nss forType:NSStringPboardType];

  [pool release];
}


/// pastebin=clipboard - copy/paste operations
void osiCocoa::getPastebin(uint8_t **out_text) {
  NSAutoreleasePool *pool= [[NSAutoreleasePool alloc] init];
  *out_text= nil;

  NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
  cchar *buf= [[pasteBoard stringForType:NSStringPboardType] UTF8String];
  
  if(buf) {
    *out_text= new uint8[Str::strlen8(buf)];
    Str::strcpy8((char *)*out_text, buf);
  }

  [pool release];
}






#endif /// OS_MAC
















