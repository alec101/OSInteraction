#pragma once

// mode variable for each HID (human input device)
// keyboard: [MODE1]: OS events (default)  [MODE2]: manual update() NA in linux             [MODE3]: directinput            [MODE4]: windows raw data(NA)
// mouse:    [MODE1]: OS events (default)  [MODE2]: manual update() using different funcs   [MODE3]: direct input (win)
// joystick:  j[0-7]: sys driver           j[8-15]: directinput / NA / NA                    j[16-19]: xinput/ NA / NA
// gamepad:  gp[0-7]: sys driver          gp[8-15]: directinput / NA / NA                   gp[16-19]: xinput/ NA / NA
// gamewheel:gw[0-7]: sys driver          gw[8-15]: directinput / NA / NA                   gw[16-19]: xinput/ NA / NA

// [internal workings]: joysticks/ gamepads/ gamewheels share direct input & xinput drivers & os drivers

// in keyboard [MODE 2&3] keystats when pressed have eiter 129 or 128 value test with key[]& 0x80 (it might be good to make this to all modes)
// in keyboard [MODE 1] windows doesnt send keyup if both rshift and lshift are pressed (same with alt and control), NOT RELIABLE FOR those keys
// in keyboard [MODE 2] update is made when a key is pressed, based in a windows message. it could be changed, just comment in the processMSG() the lines to update the keyboard&update manually
// in keyboard [MODE 3] update must be done manually in a loop (with update())

// *more USAGE stuff*
// 1. should check joysticks/gamepads/gamewheels mode. if it's 0, there is no HID present - activating it does nothing
// 2. every joystick/gamepad/gamewheel must be activated first to signal osiInput::update() to update it's values
//    simply call j/gp/gw[n].activate()

// GAMEPAD BUTTON PRESSUREs 20 - 23 are for dPad button pressures

// TODO is on the cpp file

#define MAX_KEYBOARD_KEYS 256
#define MAX_MOUSE_BUTTONS 16
#define MAX_JOYSTICK_BUTTONS 32
#define MAX_KEYS_LOGGED 16
#define MAX_JOYSTICKS 20          /// nr of maximum joysticks/gamepads/gamewheels, NOT JUST JOYSTICKS


// --------------============= MOUSE CLASS ============--------------
///==================================================================
class osiMouse {
public:
  // USAGE / settings
  
  int8_t mode;            // [MODE 1]: OS events(default) - works on every OS
                          // [MODE 2]: manual update() using different funcs (can't make it work under linux/mac, but still researching ways...)
                          // [MODE 3]: win(direct input) / linux(n/a) / mac(n/a)
  
  // position

  int x, y;               // current mouse position
  //int vx, vy;             // current mouse position on the virtual desktop *TO BE OR NOT TO BE: x&y would be inside window coords... things might get messier, tho
  int oldx, oldy;         // old mouse position (can be usefull) sincel last in.update() call
  int dx, dy;             // mouse delta movement values, since last in.update() call
  
  // wheel

  // WHEELS CAN HAVE EXACT AXIS VALUES... <<< look into this some more
  int wheel;              // wheel delta rotation in units since last in.update() call
  
  // buttons

  struct osiMouseButton {
    uint64_t lastDT;        // last button press delta time in milisecs
    uint64_t lastTimeStart; // last button press start time
    uint64_t lastTimeEnded; // last button press end time

    bool down;              // button is currently pressed
    uint64_t timeStart;     // time @ button down start

    osiMouseButton(): lastDT(0), lastTimeStart(0), lastTimeEnded(0), down(false), timeStart(0) {};
  } but[MAX_MOUSE_BUTTONS];

  // funcs

  bool init(int8_t mode);   // can init mouse with this function (usually is best to call in.init(..) instead of this)
  void update();            // if not using mode 1, update mouse values with this
  
  bool activate();          // activates the mouse after an alt-tab or on app start
  bool unactivate();        // called if any init needs to be done when the application loses focus
  
  bool grab();              // set the mouse can only move in the window boundaries
  bool ungrab();            // set the mouse to roam the plains free
  
  void setPos(int, int);    // sets the mouse position
  
  
  void resetButtons();      // resets all buttons in case of alt-tab or something similar

  // constructors / destructors

  osiMouse();
  ~osiMouse();
  void delData();
  
  int _twheel; //  to be changed to the new mac processMSG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< (to private)
private:
  bool _bActive, _bGrabbed;
  
  friend bool _processMSG(void);
  friend class osiInput;
  friend class osinteraction;
  #ifdef OS_WIN
  friend LRESULT CALLBACK _processMSG(HWND, UINT, WPARAM, LPARAM);

  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 _diDevice;
  DIMOUSESTATE2 _diStats;
  #endif /// USING DIRECT INPUT

  #endif /// OS_WIN
};



// --------------============= KEYBOARD CLASS ============--------------
///=====================================================================
class osiKeyboard {
public:
  int8_t mode;                     // [MODE1]: OS events (default)  [MODE2]: manual update()  [MODE3]: directinput  [MODE4]: windows raw data
  

  /// use Input::Kv structure if you need to find a certain key. EX: in.k.key[Kv.enter] is the enter key status (pressed or not pressed)
  uint8_t *key;                         // all keys button status - it points to buffer1/buffer2. there is a clever swap between the two buffers, so there is no copying involved
  uint64_t keyTime[MAX_KEYBOARD_KEYS];  // time @ key started to be pressed
  uint8_t *lastCheck;                   // holds what the last time the keys were checked button press info - points to 1 of the buffers
  bool capsLock, scrollLock, numLock;   // the 3 toggle locks <<< there are other 'locks'... on foreign keyboards
  bool insertLock;                      // insert is basically working like any 'lock' key
  
  struct osiKeyLog {
    int32_t code;                       // scan code of the keyboard key (Input::Kv structure has all the keyboard key codes for each os)
    bool checked;                       // checked & lastKey[] used for mortal kombat style of keyboard check
    uint64_t timeDown, timeUp, timeDT;  // when the key was pressed & released and for how int32 it was pressed (timeDT) (timeUp & timeDT can be 0, indicating the key is still down)
    
    osiKeyLog(const osiKeyLog &o): code(o.code), checked(o.checked), timeDown(o.timeDown), timeUp(o.timeUp), timeDT(o.timeDT) {};
    osiKeyLog(): code(0), checked(false), timeDown(0), timeUp(0), timeDT(0) {}
  }lastKey[MAX_KEYS_LOGGED];      // history of keys pressed - using this history, it is possible to make a Mortal Kombat combo check - like game (it has every needed variable & time for each key press&release)

  // character input/ character manipulation keys (enter/arrows/del/etc)
  
  class chTyped:public segData {  // uses the segment chainlist class(segList.cpp/h), check constructor in Keyboard()
  public:
    uint32_t c;                   // character typed (unicode); call getChar() to get the first character typed (it removes it from the list too)
    uint64_t time;                // time when the character was typed
  };

  /// in charTyped.nrNodes / manipTyped.nrNodes is the nr of chars waiting to be processed (they get auto-del after 1-2 secs if not processed)
  segList charTyped;              // list with chars typed. charTyped::nrNodes has nr of chars waiting to be 'read'. dimensions: [size:32, unitsize sizeof(chTyped)];
  segList manipTyped;             // list with string manip chars (arrow keys, backspace, del, enter, etc)

  /// the main functions to call to get a char / string manip char
  uint32_t getChar();             // returns a character typed @ keyboard or null if nothing is typed. (call it until it returns 0, or for each charTyped.nrNodes)
  uint32_t getManip();            // returns a str manip key press. (call it until it returns 0, or for each manipTyped.nrNodes)
  void clearTypedBuffer();        // clears all character buffers, ususally called when switching to a new/ existing input box / control
  
  // funcs
  
  void update();                // just call in.update(); MAIN LOOP FUNC: updates keys pressed array (key[] / lastCheck[])  (maybe toggle the locks? - N/A ATM)
  
  void resetButtons();          // call after losing program focus / need to reset all keys / fixes history of pressed keys too (lastkey[])
  
  bool activate();              // if any init is needed on alt-tab or program start, this func does it (in.init() activates everything) / on some systems this does nothing
  bool unactivate();            // called after app lose focus / on some systems this does nothing
  bool grab();                  // DANGER!! grabs exclusive control of the keyboard, if program halts or keyboard is not handled corectly, the whole system can lose the keyboard. AVOID!
  bool ungrab();                // ungrab the keyboard - better to just avoid grabbing
  
  void updateLocks();           // updates all the locks (caps, num, scroll) - autocalled by system event handler, but can be called when manually updating keyboard
  // <<< ON SOME KEYBOARDS THIS MUST BE UPDATED TO HANDLE SPECIAL LOCK KEYS >>>
  
  // --- NOTHING TO BOTHER from this point on (usually) ---
  bool init(int8_t mode= 1);                  // see 'mode' var; can be used to initialize direct input, otherwize, use Input::init()
  void _log(const osiKeyLog &);               // [internal] just puts the last key in the last key-history (it logs imediatly when a key is down)
  void _addChar(uint32_t c, uint64_t *time);  // [internal] used in WM_CHAR message... nothing to bother
  void _addManip(uint32_t c, uint64_t *time); // [internal] string manipulation keys - enter/del/arrow keys/etc
  void _doManip();                            // [internal] OSchar.cpp. checks if current keys pressed form a char manip, if they do, calls addManip() 
  inline void swapBuffers();                  // swaps what key and lastKey point to (so no copying is involved)

  osiKeyboard();
  ~osiKeyboard();
  void delData();               /// standard dealloc func / called by destroyer
  
private:
  bool _bActive, _bGrabbed;
    
  friend class osiInput;

  uint8_t _buffer1[MAX_KEYBOARD_KEYS], _buffer2[MAX_KEYBOARD_KEYS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved

  #ifdef OS_WIN  
  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 _diDevice;
  #endif /// USING_DIRECTINPUT
  #endif /// OS_WIN

// TESTING 
  int16_t getFirstKey();
  void printPressed();
  
// to be or not to be - THIS REALLY SEEMS ARE USELESS (31.01.2014) maybe if extending to ps4/xbone...
//  uint repeat[256];             /// how many times a character was repeated
//  inline int getRepeat(int key) { uint t= repeat[key]; repeat[key]= 0; return t; }
// these might be useless ^^^
// /TESTING
};


struct osiButLog {
  uint8_t but;                        /// button number
  bool checked;                     /// this is just a helper flag that can be messed with; always starts on false when a new key is added (osi doesn't use it for anything)
  uint64_t timeDown, timeUp, timeDT;  /// timeDown: when button was pressed; timeUp:  when button was released (0= button is STILL pressed); timeDT: how much time was pressed (time delta)
  
  osiButLog(const osiButLog &o): but(o.but), checked(o.checked), timeDown(o.timeDown), timeUp(o.timeUp), timeDT(o.timeDT) {};
  osiButLog():but(0), checked(false), timeDown(0), timeUp(0), timeDT(0) {}
};








class osiGamePad;
class osiGameWheel;

// --------------============= JOYSTICK CLASS ============--------------
///=====================================================================
class osiJoystick {
public:
  
  // CONFIGURATION
  
  int8_t mode;                    // [MODE0]: disabled, can check (and should) against this
                                  // [MODE1]: OS native
                                  // [MODE2]: win(directinput) / linux(n/a) / mac(n/a)
                                  // [MODE3]: win(xinput)      / linux(n/a) / mac(n/a)
  str8 name;                      // joystick name (product name)
  int16_t maxButtons;             // nr of buttons the gameWheel has
  

  // AXIS

  int32_t x, y;         // [-32767 0 +32767] main stick x and y axis
  int32_t x2, y2;       // [-32767 0 +32767] second stick x and y axis (these are reserved as i don't think these are used atm)
  int32_t throttle;     // [-32767 0 +32767] 3rd axis usually throttle
  int32_t rudder;       // [-32767 0 +32767] 4th axis usually rudder
  int32_t u, v;         // [-32767 0 +32767] fifth/ sixth axis (reserved, i guess, they might be used by some sticks tho)
  int32_t pov;          // [-1null 0 +35999] POV angle (atm it's in degrees multiplied by 100 so it's range is from 0 to 35900)
  
  // BUTTONS state / history / everything
  
  uint8_t *but;                               // buttons state
  uint8_t *butPrev;                           // holds the previous button state
  int32_t butPressure[MAX_JOYSTICK_BUTTONS];  // [0 +65534] button pressure (how hard a button is pressed) buttons 20-23 are pressure on the pov
  uint64_t butTime[MAX_JOYSTICK_BUTTONS];     // time @ key started to be pressed
  osiButLog butLog[MAX_KEYS_LOGGED];          // history of pressed/depressed buttons

  // FUNCTIONS

  bool activate();                // !! activates stick, signaling to update internal values & grabs exclusive control of stick
  bool deactivate();              // deactivates & ungrabs exclusive control of stick
  bool isActive() { return _bActive; }
  
  bool grab();                    // exclusive control of the device (if possible)
  bool ungrab();                  // lose exclusive control of the device
  
  void resetButtons();            // clears all button buffers & resets logged buttons (used in case of alt-tab or something similar)
  void resetAxis();               // clears all stick axis values

  void update();                  // MAIN UPDATE FUNC (for every type of stick/pad/wheel) (calling Input::update() calls this too, if stick is active)

  osiJoystick();
  ~osiJoystick();
  void delData();

  // private data from here on  
private:
  friend class osiInput;
  friend class osinteraction;
  
  bool _bActive, _bGrabbed;       // internal flags

  osiGamePad *_gp;                // linked gamepad - each stick has a coresponding gamepad that uses the same 'driver'
  osiGameWheel *_gw;              // linked gamewheel - each stick has a coresponding gamewheel that uses the same 'driver'
  uint8_t _buffer1[MAX_JOYSTICK_BUTTONS], _buffer2[MAX_JOYSTICK_BUTTONS];   /// used for the but / butPrev. buffers are swapped with pointers, so no copying is involved
  inline void _swapBuffers();     // [internal] swaps button buffers
  void _log(const osiButLog &);   // [internal] just puts the last button in the last button-history (it logs imediatly when a button is down)

  /// OS specific stuff
  #ifdef OS_WIN
  int16_t _id;                    // windows id (THIS MIGHT BE UNIVERSAL)
  
  #ifdef USING_DIRECTINPUT        // primary
  friend BOOL CALLBACK _diDevCallback(LPCDIDEVICEINSTANCE, LPVOID);
  LPDIRECTINPUTDEVICE8 _diDevice;
  //LPCDIDEVICEINSTANCE diID;       // ID of the device; if a new one is plugged, it will differ from current IDs
  GUID _diID;                     // ID of the device; if a new one is plugged, it will differ from current IDs
  DIJOYSTATE2 _diStats;
  #endif /// USING_DIRECTINPUT

  #ifdef USING_XINPUT             // secondary, probly main joysticks are using direct input
  #endif /// USING_XINPUT

  #endif /// OS_WIN

  #ifdef OS_LINUX
  int32_t _jsFile;                // opened /dev/input/jsNNN  file
  int16_t _jsID;                  // /dev/input/jsNNN    NNN= id
  int32_t _eventFile;             // opened /dev/input/eventNNN eventFile
  int16_t _eventID;               // /dev/input/eventNNN NNN= eventID
  #endif
  
  #ifdef OS_MAC
  //friend void _HIDchange(void *, IOReturn, void *, IOHIDValueRef);
  
public:
  /// this struct is used to transfer data from HID callback funcs (that can write at any moment anythinhg - dangerous)
  struct _CallbackTame {
    int32_t x, y, x2, y2, throttle, rudder, u, v, pov;
    uint8_t but[MAX_JOYSTICK_BUTTONS];
    int32_t butPressure[MAX_JOYSTICK_BUTTONS];
    std::mutex mutex;             /// object lock system
    _CallbackTame() { delData(); }
    void delData() {
      x= y= x2= y2= throttle= rudder= u= v= 0;
      pov= -1;
      for(int a= 0; a< MAX_JOYSTICK_BUTTONS; a++) butPressure[a]= but[a]= 0;
    }
  } _cbTame;
private:
  #endif
};







// --------------============= GAMEPAD CLASS ============--------------
///====================================================================
class osiGamePad {
  friend class osiInput;
  friend class osiJoystick;
public:
  
  // CONFIGURATION
  
  int8_t mode;            // [MODE0]: disabled, can check against this
                          // [MODE1]: OS native
                          // [MODE2]: win(directinput) / linux(n/a) / mac(n/a)
                          // [MODE3]: win(xinput)      / linux(n/a) / mac(n/a)
  str8 name;              // gamepad name (product name)
  int16_t type;           // 0= ps3 compatible; 1= xbox compatible - COULD BE CHANGED by user in-game, and it will work to update the right axis!!!
  int16_t maxButtons;     // nr of buttons the gamePad has
  
  // AXIS
  
  int32_t lx, ly;   // [-32767 0 +32767] stick 1 axis position (left)
  int32_t rx, ry;   // [-32767 0 +32767] stick 2 axis position (right)
  int32_t lt, rt;   // [0 +65534]        left and right triggers
  int32_t u, v;     // [-32767 0 +32767] extra axis - updated but usually not used... usually... can't know what pad they make
  int32_t pov;      // [-1null 0 +35900] POV angle (multiplied by 100, so 35,900(max)= 359 degrees) (-1 usually, if not pressed)

  // BUTTONS state / history

  uint8_t *but;                               // buttons state
  uint8_t *butPrev;                           // holds previous buttons state
  int32_t butPressure[MAX_JOYSTICK_BUTTONS];  // [0 +65534] button pressure (how hard a button is pressed) buttons 20-23 are pressure on the pov
  uint64_t butTime[MAX_JOYSTICK_BUTTONS];     // time @ key started to be pressed
  osiButLog butLog[MAX_KEYS_LOGGED];          // history of pressed/depressed buttons

  // functions

  bool activate() { return _j->activate(); }     // !! activates pad, signaling to update it's data values& grabbing exclusive usage
  bool deactivate() { return _j->deactivate(); } // deactivates & ungrabs device

  void resetButtons();                  // clears all button buffers & resets logged buttons (used in case of alt-tab or something similar)
  void resetAxis();                     // resets all gamepad axis to 0 / null position
  void update() { _j->update(); }       // can be called, to manually update the gamepad variables
  bool grab() { return _j->grab(); }    // exclusive control of the device (if possible)
  bool ungrab() { return _j->ungrab(); }// lose exclusive control of the device

  osiGamePad();
  ~osiGamePad();
  void delData();
  
// internals from here on
private:
  osiJoystick *_j;                      // linked Joystick class
  void _log(const osiButLog &);         // [internal] just puts the last button in the last button-history (it logs imediatly when a button is down)
  uint8_t _buffer1[MAX_JOYSTICK_BUTTONS], _buffer2[MAX_JOYSTICK_BUTTONS];   // used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved
  inline void _swapBuffers();
  
  #ifdef OS_MAC
  //friend void HIDchange(void *, IOReturn, void *, IOHIDValueRef);
  #endif 
};






// --------------============= GAMEWHEEL CLASS ============--------------
///======================================================================
class osiGameWheel {
  friend class osiInput;
  friend class osiJoystick;
public:
  
  // CONFIGURATION
  
  int16_t mode;                   // [MODE0]: disabled, can check against this
                                  // [MODE1]: OS native
                                  // [MODE2]: win(directinput) / linux(n/a) / mac(n/a)
                                  // [MODE3]: win(xinput)      / linux(n/a) / mac(n/a)
  str8 name;                      // wheel name (product name)
  int16_t  maxButtons;            // nr of buttons the gameWheel has
  
  // AXIS
  
  int32_t wheel;                  // [-32767 0 +32767] the wheel
  int32_t a1, a2, a3, a4, a5;     // [-32767 0 +32767] different axis (more work must be done here, i think)
  // a pov??                       // THIS NEEDS MORE WORK <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // BUTTONS state / history

  uint8_t *but;                             // buttons state
  uint8_t *butPrev;                         // holds previous buttons state
  int32_t butPressure[MAX_JOYSTICK_BUTTONS];// [0 +65534] button pressure (how hard a button is pressed) buttons 20-23 are pressure on the pov
  uint64_t butTime[MAX_JOYSTICK_BUTTONS];   // time @ key started to be pressed
  osiButLog butLog[MAX_KEYS_LOGGED];        // history of pressed buttons

  // functions

  bool activate() { return _j->activate(); }     // !! activates wheel, signaling to update it's data values & grabbing exclusive usage
  bool deactivate() { return _j->deactivate(); } // deactivates & ungrabs device

  void resetButtons();                  // clears all button buffers & resets history (used in case of alt-tab or something similar)
  void resetAxis();                     // resets all gamewheel axis to 0 / null position
  void update() { _j->update(); }       // updates internal vals; calling Input::update() is better, but each stick can be updated manually
  bool grab() { return _j->grab(); }    // exclusive control of the device (if possible)
  bool ungrab() { return _j->ungrab(); }// lose exclusive control of the device
  

  osiGameWheel();
  ~osiGameWheel();
  void delData();
  
private:
  osiJoystick *_j;                // linked Joystick class
  uint8_t _buffer1[MAX_JOYSTICK_BUTTONS], _buffer2[MAX_JOYSTICK_BUTTONS];  // used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved

  void _log(const osiButLog &);   // [internal] puts the last button in the last button-history (it logs imediatly when a button is down)
  inline void _swapBuffers();
  
  #ifdef OS_MAC
  //friend void _HIDchange(void *, IOReturn, void *, IOHIDValueRef);
  #endif 

};



// -these are key codes; they are updated with _Kv.populate()- it is auto-called in in.populate()
// -they are OS independant, and if the user switches a keyboard/ system changes keyboard locals
//    another call to in.populate() should update the key code vals - THESE CODES ARE THE SAME ON EVERY KEYBOARD ON EARTHl
//    SO THIS IS NOT TRUE (i think) - problem is that a few keys might be on a different FIZICAL position (european keyboards usually)
// -v comes from 'variable'
// -only the most inportant keyboard keys are in this struct.
//    (there is no 'play' button on some crappy keyboard manufacturer)
// -on some keyboards, SOME of these keys MIGHT NOT EXIST!!!!
struct _Kv {
  uint8_t esc, enter, kpenter,
          tab, space, backspace,
          insert, del, home, end, pgup, pgdown,
          rshift, lshift, rctrl, lctrl, ralt, lalt,
          f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
          left, up, right, down,
          prtsc, pause, capslock, scrolllock, numlock,
          q, w, e, r, t, y, u, i, o, p,
          a, s, d, f, g, h, j, k, l,
          z, x, c, v, b, n, m,
          n1, n2, n3, n4, n5, n6, n7, n8, n9, n0,
          kp1, kp2, kp3, kp4, kp5, kp6, kp7, kp8, kp9, kp0,
          minus, equal, backslash, lbracket, rbracket, semicolon, apostrophe, // should apostrophe be quote?
          comma, dot, slash, grave, kpslash, kpmultiply, kpminus, kpplus, kpequal, kpdel, // dot= period
          lOS, rOS, menu; /// left win key, right win key, propreties key
  void populate();  /// updates all these vars. SOURCE CODE IN "OSchar.cpp"
};







// ================================================================== //
// --------------============= INPUT CLASS ============-------------- //
// ================================================================== //

#ifdef USING_DIRECTINPUT
BOOL CALLBACK _diDevCallback(LPCDIDEVICEINSTANCE, LPVOID); // no, i did not create this crap, this is direct input 'callback' func
#endif

class osiInput {
public:

  std::mutex mutex;               // when reading from different threads, this is the locking mutex that will lock every class that osiInput handles
  osiMouse m;                     // more than 1 mouse?
  osiKeyboard k;                  // more than 1 keyboard?
  osiJoystick j[MAX_JOYSTICKS];   //  j[0-7]= OS driver;  j[8-15] XINPUT;  j[16-19] DIRECT INPUT
  osiGamePad gp[MAX_JOYSTICKS];   // gp[0-7]= OS driver; gp[8-15] XINPUT; gp[16-19] DIRECT INPUT
  osiGameWheel gw[MAX_JOYSTICKS]; // gw[0-7]= OS driver; gw[8-15] XINPUT; gw[16-19] DIRECT INPUT
  _Kv Kv;                         // struct with most inportant keycodes: in.k.key[Kv.space] is possible. OS independant/ if keyboard is changed in any way, just call Kv.populate()
  
  // each driver type name (EX: j[0].mode==1 -> system default driver j[8].mode== 2 -> Direct Input handled)
  str8 mode1Name;             // under all systems, this is 'System Handled' or 'System Default' <<<<<<<<<<<<<<<<<<< CHOSE A GOOD NAME
  str8 mode2Name;             // under windows, this should be 'DirectInput', under the others 'Not Used'
  str8 mode3Name;             // under windows, this should be 'XInput', under the others 'Not Used'
  
  struct InputNumbers {
    int16_t jFound;           // nr of joysticks found on system in total (for win, os+directinput+xinput)
    int16_t gpFound;          // nr of gamepads found on system
    int16_t gwFound;          // nr of gamewheels found on system

    int16_t jOS;              // (max 8) nr of normal driver joysticks found
    int16_t gpOS;             // (max 8) nr of normal driver joysticks found
    int16_t gwOS;             // (max 8) nr of normal driver joysticks found

    int16_t jT2;              // (max 8) nr of directinput joysticks found  (nothing in linux/mac, but the code will compile)
    int16_t gpT2;             // (max 8) nr of directinput gamepads found   (nothing in linux/mac, but the code will compile)
    int16_t gwT2;             // (max 8) nr of directinput gamewheels found (nothing in linux/mac, but the code will compile)

    int16_t jT3;              // (max 4) nr of xinput joysticks found  (nothing in linux/mac, but the code will compile)
    int16_t gpT3;             // (max 4) nr of xinput gamepads found   (nothing in linux/mac, but the code will compile)
    int16_t gwT3;             // (max 4) nr of xinput gamewheels found (nothing in linux/mac, but the code will compile)
  } nr;                       // all different numbers of HID found

  inline osiJoystick  *getT2j (int16_t nr) { return  &j[8+ nr]; }   // [win type2 = direct input] [linux= nothig] [mac= nothig]
  inline osiGamePad   *getT2gp(int16_t nr) { return &gp[8+ nr]; }   // [win type2 = direct input] [linux= nothig] [mac= nothig]
  inline osiGameWheel *getT2gw(int16_t nr) { return &gw[8+ nr]; }   // [win type2 = direct input] [linux= nothig] [mac= nothig]

  inline osiJoystick  *getT3j (int16_t nr) { return  &j[16+ nr]; }  // [win type3= xinput] [linux= nothig] [mac= nothig]
  inline osiGamePad   *getT3gp(int16_t nr) { return &gp[16+ nr]; }  // [win type3= xinput] [linux= nothig] [mac= nothig]
  inline osiGameWheel *getT3gw(int16_t nr) { return &gw[16+ nr]; }  // [win type3= xinput] [linux= nothig] [mac= nothig]

  // functions
  
  bool init(int mMode= 1, int kMode= 1);        // must be called after a main window is created (see 'mode' variable for mouse & keyboard for more customization) - locks in.mutex
  void populate(bool scanMouseKeyboard= false); // searches for joysticks / other HIDs - locks in.mutex
  void update();                                // update everything (mouse& keyboard& sticks& pads& wheels) - locks in.mutex
  void resetPressedButtons();                   // in case of ALT-TAB, all buttons/timers must be reset, to avoid bugs!!!
  void resetAxis();                             // resets all HID axis (sticks/pads/wheels)

  osiInput();
  ~osiInput();
  void delData();
  
  // private (internal) stuff from here on
private:
  friend class osinteraction;
  friend class osiMouse;
  friend class osiKeyboard;

  uint64_t _lastPopulate;

  #ifdef OS_WIN
  #ifdef USING_DIRECTINPUT
  friend BOOL CALLBACK _diDevCallback(LPCDIDEVICEINSTANCE, LPVOID);
  LPDIRECTINPUT8 _dInput;
  #endif
  #endif /// OS_WIN

  #ifdef OS_LINUX
  // linux keysyms handling (keysyms make windows look good ffs)
  // these funcs are in <OSchar.cpp>, at the end of the file !!!!!!!!!!!!!
  void _keysym2unicode(KeySym *, uint32_t *ret);  // converts keysym to unicode (no checks, use getUnicode)
  void _getUnicode(KeySym *, uint32_t *ret);      // converts keysym to unicode, verifies that the character is valid
  #endif /// OS_LINUX

  #ifdef OS_MAC // MAC MESS <<<--------------- NOTHING TO BOTHER HERE ------
  // nothing to bother here, all internal vars
  void *_manager;        // [internal] 'manager' that handles all HID devices (this one is set to handle sticks/pads/wheels only)
  #endif // END MAC MESS <<<-----------------------------------------------

// TESTING
  void vibrate();
// TESTING ^^^^^^^^

};


extern osiInput in;  // only 1 global class


#define mPos(_x, _y, _dx, _dy) ((in.m.x>= (_x)) && (in.m.x<= ((_x)+ (_dx))) && (in.m.y>= (_y)) && (in.m.y<= ((_y)+ (_dy))))



