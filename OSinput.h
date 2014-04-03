#pragma once

// mode variable for each HID (human input device)
// keyboard: [MODE1]: OS events (default)  [MODE2]: manual update() NA in linux             [MODE3]: directinput            [MODE4]: windows raw data(NA)
// mouse:    [MODE1]: OS events (default)  [MODE2]: manual update() using different funcs   [MODE3]: direct input (win)
// joystick:  j[0-7]: sys driver           j[8-15]: directinput / ? / ?                    j[16-19]: xinput/ ? / ?
// gamepad:  gp[0-7]: sys driver          gp[8-15]: directinput / ? / ?                   gp[16-19]: xinput/ ? / ?
// gamewheel:gw[0-7]: sys driver          gw[8-15]: directinput / ? / ?                   gw[16-19]: xinput/ ? / ?

// [internal workings]: joysticks/ gamepads/ gamewheels share direct input & xinput drivers & os drivers

// in keyboard [MODE 2&3] keystats when pressed have eiter 129 or 128 value test with key[]& 0x80 (it might be good to make this to all modes)
// in keyboard [MODE 1] windows doesnt send keyup if both rshift and lshift are pressed (same with alt and control), NOT RELIABLE FOR those keys
// in keyboard [MODE 2] update is made when a key is pressed, based in a windows message. it could be changed, just comment in the processMSG() the lines to update the keyboard&update manually
// in keyboard [MODE 3] update must be done manually in a loop (with update())

// *more USAGE stuff*
// -setting a gamepad/joy/etc as "inUse" or "active" (NOT INPLEMENTED)
//    will tell input.update() to update it. Else, update it manually
//    or just use nrJoy, nrBlabla??????????????????? this might be CUT

// GAMEPAD BUTTON PRESSUREs 20 - 23 are for dPad button pressures

// TODO is on the cpp file

#define MAX_KEYBOARD_KEYS 256
#define MAX_MOUSE_BUTTONS 16
#define MAX_JOYSTICK_BUTTONS 32
#define MAX_KEYS_LOGGED 16
#define MAX_JOYSTICKS 20          /// nr of maximum joysticks/gamepads/gamewheels, NOT JUST JOYSTICKS


// --------------============= MOUSE CLASS ============--------------
///==================================================================
class Mouse {
  friend class Input;
public:
// USAGE / settings
  
  short mode;              // [MODE 1]: OS events(default) - works on every OS
                           // [MODE 2]: manual update() using different funcs (can't make it work under linux/mac, but still researching ways...)
                           // [MODE 3]: win(direct input) / linux(n/a) / mac(n/a)
  
  bool useDelta;          /// use dx, dy (x,y, oldx, oldy are still updated)

/// position
  int x, y;               /// current mouse position
  int oldx, oldy;         /// old mouse position (can be usefull)
/// delta x,y since last read. when program reads them, either set them to 0 after each read, or use getDx() & getDy()
  int dx, dy;             /// read these vars, and set them to 0, or call getDx()/getDy(). they track all movement until program processes them
  inline int getDx() { int tx= dx; dx= 0; return tx; }          /// returns mouse delta movement on y axis
  inline int getDy() { int ty= dy; dy= 0; return ty; }          /// returns mouse delta movement on x axis

  // WHEELS CAN HAVE EXACT AXIS VALUES... <<< look into this some more
/// wheel
  int wheel;              /// wheel delta rotation in units; get nr units rotated then set <wheel> to 0; or use getWheelDu();
  inline int getWheelDu() { int t= wheel; wheel= 0; return t; } /// returns wheel delta units

/// buttons
  struct Button {
    uint64 lastDT;        /// last button press delta time in milisecs
    uint64 lastTimeStart; /// last button press start time
    uint64 lastTimeEnded; /// last button press end time

    bool down;            /// button is currently pressed
    uint64 timeStart;     /// time @ button down start

    Button(): lastDT(0), lastTimeStart(0), lastTimeEnded(0), down(false), timeStart(0) {};
  } b[MAX_MOUSE_BUTTONS];

  
  bool init(short mode);  /// can init mouse with this function (usually is best to call in.init(..) instead of this)
  void update();          /// if not using mode 1, update mouse values with this
  
  bool aquire();          /// exclusive control of the mouse (if possible)
  bool unaquire();        /// lose exclusive control of the mouse
  void resetButtons();    /// resets all buttons in case of alt-tab or something similar

  Mouse();
  ~Mouse();
  void delData();

private:  
  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 diDevice;
  DIMOUSESTATE2 diStats;
  #endif
};



// --------------============= KEYBOARD CLASS ============--------------
///=====================================================================
class Keyboard {
  friend class Input;
public:
  short mode;                   // [MODE1]: OS events (default)  [MODE2]: manual update()  [MODE3]: directinput  [MODE4]: windows raw data
  
  void update();                /// MAIN LOOP FUNC: updates keys pressed array (key[] / lastCheck[])  (maybe toggle the locks? - N/A ATM)

  /// use Input::Kv structure if you need to find a certain key. EX: in.k.key[Kv.enter] is the enter key status (pressed or not pressed)
  uchar *key;                   // all keys button status - it points to buffer1/buffer2. there is a clever swap between the two buffers, so there is no copying involved
  uint64 keyTime[256];          /// time @ key started to be pressed
  uchar *lastCheck;             /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  bool capsLock, scrollLock, numLock;     /// the 3 toggle locks <<< there are other 'locks'... on foreign keyboards
  
  
  struct KeyPressed {
    int code;                   /// scan code of the keyboard key (Input::Kv structure has all the keyboard key codes for each os)
    bool checked;               /// checked & lastKey[] used for mortal kombat style of keyboard check
    uint64 timeDown, timeUp, timeDT;   /// when the key was pressed & released and for how long it was pressed (timeDT) (timeUp & timeDT can be 0, indicating the key is still down)
    
    KeyPressed(const KeyPressed &o): code(o.code), checked(o.checked), timeDown(o.timeDown), timeUp(o.timeUp), timeDT(o.timeDT) {};
    KeyPressed(): code(0), checked(false), timeDown(0), timeUp(0), timeDT(0) {}
  }lastKey[MAX_KEYS_LOGGED];     /// history of keys pressed - using this history, it is possible to make a Mortal Kombat combo check - like game (it has every needed variable & time for each key press&release)

// character input/ character manipulation keys (enter/arrows/del/etc)
  class chTyped:public segData {/// uses the segment chainlist class(segList.cpp/h), check constructor in Keyboard()
  public:
    ulong c;                    /// character typed (unicode); call getChar() to get the first character typed (it removes it from the list too)
    uint64 time;                /// time when the character was typed
  };

  /// in charTyped.nrNodes / manipTyped.nrNodes is the nr of chars waiting to be processed (they get auto-del after 1-2 secs if not processed)
  segList charTyped;            /// list with chars typed. charTyped::nrNodes has nr of chars waiting to be 'read'. dimensions: [size:32, unitsize sizeof(chTyped)];
  segList manipTyped;           /// list with string manip chars (arrow keys, backspace, del, enter, etc)

  /// the main functions to call to get a char / string manip char
  ulong getChar();              /// returns a character typed @ keyboard or null if nothing is typed. (call it until it returns 0, or for each charTyped.nrNodes)
  ulong getManip();             /// returns a str manip key press. (call it until it returns 0, or for each manipTyped.nrNodes)
  void clearTypedBuffer();      /// clears all character buffers, ususally called when switching to a new/ existing input box / control
  
// useful functions
  void resetButtons();          /// call after losing program focus / need to reset all keys / fixes history of pressed keys too (lastkey[])
  bool aquire();                /// call after gaining focus (alt-tab...)
  bool unaquire();              /// call after losing focus (alt-tab...)
  void updateLocks();           /// updates all the locks (caps, num, scroll) - autocalled by system event handler, but can be called when manually updating keyboard
  // <<< ON SOME KEYBOARDS THIS MUST BE UPDATED TO HANDLE SPECIAL LOCK KEYS >>>
  
// --- NOTHING TO BOTHER from this point on (usually) ---
  bool init(short mode= 1);      // see 'mode' var; can be used to initialize direct input, otherwize, use Input::init()
  void log(const KeyPressed &); /// [internal] just puts the last key in the last key-history (it logs imediatly when a key is down)
  void addChar(ulong c, uint64 *time);  /// [internal] used in WM_CHAR message... nothing to bother
  void addManip(ulong c, uint64 *time); /// [internal] string manipulation keys - enter/del/arrow keys/etc
  void doManip();                       /// [internal] OSchar.cpp. checks if current keys pressed form a char manip, if they do, calls addManip() 
  inline void swapBuffers();    /// swaps what key and lastKey point to (so no copying is involved)

  Keyboard();
  ~Keyboard();
  void delData();               /// standard dealloc func / called by destroyer
  
private:
  uchar buffer1[MAX_KEYBOARD_KEYS], buffer2[MAX_KEYBOARD_KEYS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved
  
  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 diDevice;
  #endif /// USING_DIRECTINPUT

// TESTING 
  short getFirstKey();
  void printPressed();
  
// to be or not to be - THIS REALLY SEEMS ARE USELESS (31.01.2014) maybe if extending to ps4/xbone...
//  uint repeat[256];             /// how many times a character was repeated
//  inline int getRepeat(int key) { uint t= repeat[key]; repeat[key]= 0; return t; }
// these might be useless ^^^
// /TESTING
};



struct ButPressed {
  uchar b;                        /// button number
  bool checked;                   /// this is just a helper flag that can be messed with; always starts on false when a new key is added (osi doesn't use it for anything)
  uint64 timeDown, timeUp, timeDT;  /// timeDown: when button was pressed; timeUp:  when button was released (0= button is STILL pressed); timeDT: how much time was pressed (time delta)
  
  ButPressed(const ButPressed &o): b(o.b), checked(o.checked), timeDown(o.timeDown), timeUp(o.timeUp), timeDT(o.timeDT) {};
  ButPressed():b(0), checked(false), timeDown(0), timeUp(0), timeDT(0) {}
};

class GamePad;
class GameWheel;
// --------------============= JOYSTICK CLASS ============--------------
///=====================================================================
class Joystick {
  friend class Input;
  friend class OSInteraction;
public:
  
// CONFIGURATION
  
  short mode;                      // [MODE0]: disabled, can check against this
                                   // [MODE1]: OS native
                                   // [MODE2]: win(directinput) / linux(n/a) / mac(n/a)
                                   // [MODE3]: win(xinput)      / linux(n/a) / mac(n/a)
  string name;                    /// joystick name (product name)
  short maxButtons;               /// nr of buttons the gameWheel has

// AXIS
  
  long x, y;                      /// main stick x and y axis
  long x2, y2;                    /// second stick x and y axis (these are reserved as i don't think these are used atm)
  long throttle;                  /// 3rd axis usually throttle
  long rudder;                    /// 4th axis usually rudder
  long u, v;                      /// fifth/ sixth axis (reserved, i guess, they might be used by some sticks tho)
  long pov;                       /// POV angle (multiplied by 100, so 35,900(max)= 359 degrees)
  
// BUTTONS state / history / everything
  
  uchar *b;                             /// buttons state
  long bPressure[MAX_JOYSTICK_BUTTONS]; /// heck if k knew this existed... guess no game (that i played) uses it; now it's implemented!
  uint64 bTime[MAX_JOYSTICK_BUTTONS];   /// time @ key started to be pressed
  uchar *lastCheck;                     /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  ButPressed lastBut[MAX_KEYS_LOGGED];  /// history of pressed buttons

// FUNCTIONS

  void update();                  /// MAIN UPDATE FUNC (for every type of stick/pad/wheel)
  void aquire();                  /// exclusive control of the device (if possible)
  void unaquire();                /// lose exclusive control of the device
  void resetButtons();            /// clears all button buffers & resets logged buttons (used in case of alt-tab or something similar)

  Joystick();
  ~Joystick();
  void delData();

// private data from here on  
private:
  GamePad *_gp;                   /// linked gamepad - each stick has a coresponding gamepad that uses the same 'driver'
  GameWheel *_gw;                 /// linked gamewheel - each stick has a coresponding gamewheel that uses the same 'driver'
  uchar buffer1[MAX_JOYSTICK_BUTTONS], buffer2[MAX_JOYSTICK_BUTTONS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved
  inline void swapBuffers();      /// [internal] swaps button buffers
  void log(const ButPressed &);   /// [internal] just puts the last button in the last button-history (it logs imediatly when a button is down)
  
  /// OS specific stuff
  #ifdef OS_WIN
  short id;                        // windows id (THIS MIGHT BE UNIVERSAL)
  #endif /// OS_WIN

  #ifdef USING_DIRECTINPUT         // primary
  friend BOOL CALLBACK diDevCallback(LPCDIDEVICEINSTANCE, LPVOID);
  LPDIRECTINPUTDEVICE8 diDevice;
  LPCDIDEVICEINSTANCE diID;       /// ID of the device; if a new one is plugged, it will differ from current IDs
  DIJOYSTATE2 diStats;
  #endif /// USING_DIRECTINPUT

  #ifdef USING_XINPUT              // secondary, probly main joysticks are using direct input
  #endif /// USING_XINPUT

  #ifdef OS_LINUX
  int jsFile;                     /// opened /dev/input/jsNNN  file
  short jsID;                     /// /dev/input/jsNNN    NNN= id
  int eventFile;                  /// opened /dev/input/eventNNN eventFile
  short eventID;                  /// /dev/input/eventNNN NNN= eventID
  #endif
};



// --------------============= GAMEPAD CLASS ============--------------
///====================================================================
class GamePad {
  friend class Input;
  friend class Joystick;
public:
  
// CONFIGURATION
  
  short mode;                      // [MODE0]: disabled, can check against this
                                   // [MODE1]: OS native
                                   // [MODE2]: win(directinput) / linux(n/a) / mac(n/a)
                                   // [MODE3]: win(xinput)      / linux(n/a) / mac(n/a)
  string name;                    /// gamepad name (product name)
  short type;                     /// 0= ps3 compatible; 1= xbox compatible - COULD BE CHANGED by user in-game, and it will work to update the right axis!!!
  short maxButtons;               /// nr of buttons the gamePad has
  
// AXIS
  
  long lx, ly;                    /// stick 1 axis position (left)
  long rx, ry;                    /// stick 2 axis position (right)
  long lt, rt;                    /// left and right triggers
  long u, v;                      /// extra axis - updated but usually not used... usually... can't know what pad they make
  long pov;                       /// POV angle (multiplied by 100, so 35,900(max)= 359 degrees) (-1 usually, if not pressed)

// BUTTONS state / history

  uchar *b;                             /// buttons state
  long bPressure[MAX_JOYSTICK_BUTTONS]; /// heck if k knew this existed... guess no game (that i played) uses it; now it's implemented!
  uint64 bTime[MAX_JOYSTICK_BUTTONS];   /// time @ key started to be pressed
  uchar *lastCheck;                     /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  ButPressed lastBut[MAX_KEYS_LOGGED];  /// history of pressed buttons

// functions
  
  void resetButtons();                  /// clears all button buffers & resets logged buttons (used in case of alt-tab or something similar)
  void update() { _j->update(); }       /// can be called, to manually update the gamepad variables
  bool aquire() { _j->aquire(); }       /// exclusive control of the device (if possible)
  bool unaquire() { _j->unaquire(); }   /// lose exclusive control of the device

  GamePad();
  ~GamePad();
  void delData();
  
// internals from here on
private:
  Joystick *_j;                         /// linked Joystick class
  void log(const ButPressed &);         /// [internal] just puts the last button in the last button-history (it logs imediatly when a button is down)
  uchar buffer1[MAX_JOYSTICK_BUTTONS], buffer2[MAX_JOYSTICK_BUTTONS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved
  inline void swapBuffers();
};



// --------------============= GAMEWHEEL CLASS ============--------------
///======================================================================
class GameWheel {
  friend class Input;
  friend class Joystick;
public:
  
// CONFIGURATION
  
  short mode;                      // [MODE0]: disabled, can check against this
                                   // [MODE1]: OS native
                                   // [MODE2]: win(directinput) / linux(n/a) / mac(n/a)
                                   // [MODE3]: win(xinput)      / linux(n/a) / mac(n/a)
  string name;                    /// wheel name (product name)
  short  maxButtons;              /// nr of buttons the gameWheel has
  
// AXIS
  
  long wheel;                     /// the wheel
  long a1, a2, a3, a4, a5;        /// different axis/ pedals
  // a pov??                       // THIS NEEDS MORE WORK <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// BUTTONS state / history

  uchar *b;                             /// buttons state
  long bPressure[MAX_JOYSTICK_BUTTONS]; /// heck if k knew this existed... guess no game (that i played) uses it; now it's implemented!
  uint64 bTime[MAX_JOYSTICK_BUTTONS];   /// time @ key started to be pressed
  uchar *lastCheck;                     /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  ButPressed lastBut[MAX_KEYS_LOGGED];  /// history of pressed buttons

// functions

  void update() { _j->update(); }
  bool aquire() { _j->aquire(); }       /// exclusive control of the device (if possible)
  bool unaquire() { _j->unaquire(); }   /// lose exclusive control of the device
  void resetButtons();                  /// clears all button buffers & resets history (used in case of alt-tab or something similar)

  GameWheel();
  ~GameWheel();
  void delData();
  
private:
  Joystick *_j;                  /// linked Joystick class
  uchar buffer1[MAX_JOYSTICK_BUTTONS], buffer2[MAX_JOYSTICK_BUTTONS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved

  void log(const ButPressed &);  /// [internal] puts the last button in the last button-history (it logs imediatly when a button is down)
  inline void swapBuffers();
};



// -these are key codes; they are updated with _Kv.populate()- it is auto-called in in.populate()
// -they are OS independant, and if the user switches a keyboard/ system changes keyboard locals
//    another call to in.populate() should update the key code vals - THESE CODES ARE THE SAME ON EVERY KEYBOARD ON EARTHl
//    SO THIS IS NOT TRUE (i think) - problem is that a few keys might be on a different FIZICAL position (european keyboards usually)
// -v comes from 'variable'
/// -only the most inportant keyboard keys are in this struct.
///    (there is no 'play' button on some crappy keyboard manufacturer)
/// -on some keyboards, SOME of these keys MIGHT NOT EXIST!!!!
struct _Kv {
  uchar esc, enter, kpenter,
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
BOOL CALLBACK diDevCallback(LPCDIDEVICEINSTANCE, LPVOID); /// no, i did not create this crap, this is direct input 'callback' func
#endif

class Input {
  friend class OSInteraction;
  friend class Mouse;
  friend class Keyboard;
public:
  Mouse m;                      /// more than 1 mouse?
  Keyboard k;                   /// more than 1 keyboard?
  Joystick j[MAX_JOYSTICKS];    ///  j[0-7]= OS driver;  j[8-15] XINPUT;  j[16-19] DIRECT INPUT
  GamePad gp[MAX_JOYSTICKS];    /// gp[0-7]= OS driver; gp[8-15] XINPUT; gp[16-19] DIRECT INPUT
  GameWheel gw[MAX_JOYSTICKS];  /// gw[0-7]= OS driver; gw[8-15] XINPUT; gw[16-19] DIRECT INPUT
  _Kv Kv;                       /// struct with most inportant keycodes: in.k.key[Kv.space] is possible. OS independant/ if keyboard is changed in any way, just call Kv.populate()
  
  /// each driver type name (EX: j[0].mode==1 -> system default driver j[8].mode== 2 -> Direct Input handled)
  string mode1Name;             /// under all systems, this is 'System Handled' or 'System Default' <<<<<<<<<<<<<<<<<<< CHOSE A GOOD NAME
  string mode2Name;             /// under windows, this should be 'DirectInput', under the others 'Not Used'
  string mode3Name;             /// under windows, this should be 'XInput', under the others 'Not Used'
  
  struct InputNumbers {
    short jFound;             /// nr of joysticks found on system in total (for win, os+directinput+xinput)
    short gpFound;            /// nr of gamepads found on system
    short gwFound;            /// nr of gamewheels found on system

    short jOS;                /// (max 8) nr of normal driver joysticks found
    short gpOS;               /// (max 8) nr of normal driver joysticks found
    short gwOS;               /// (max 8) nr of normal driver joysticks found

    short jT2;                /// (max 8) nr of directinput joysticks found  (nothing in linux/mac, but the code will compile)
    short gpT2;               /// (max 8) nr of directinput gamepads found   (nothing in linux/mac, but the code will compile)
    short gwT2;               /// (max 8) nr of directinput gamewheels found (nothing in linux/mac, but the code will compile)

    short jT3;                /// (max 4) nr of xinput joysticks found  (nothing in linux/mac, but the code will compile)
    short gpT3;               /// (max 4) nr of xinput gamepads found   (nothing in linux/mac, but the code will compile)
    short gwT3;               /// (max 4) nr of xinput gamewheels found (nothing in linux/mac, but the code will compile)
  } nr;                       /// all different numbers of HID found

  inline Joystick  *getT2j (short nr) { return  &j[8+ nr]; } /// [win type2 = direct input] [linux= nothig] [mac= nothig]
  inline GamePad   *getT2gp(short nr) { return &gp[8+ nr]; } /// [win type2 = direct input] [linux= nothig] [mac= nothig]
  inline GameWheel *getT2gw(short nr) { return &gw[8+ nr]; } /// [win type2 = direct input] [linux= nothig] [mac= nothig]

  inline Joystick  *getT3j (short nr) { return  &j[16+ nr]; } /// [win type3= xinput] [linux= nothig] [mac= nothig]
  inline GamePad   *getT3gp(short nr) { return &gp[16+ nr]; } /// [win type3= xinput] [linux= nothig] [mac= nothig]
  inline GameWheel *getT3gw(short nr) { return &gw[16+ nr]; } /// [win type3= xinput] [linux= nothig] [mac= nothig]

// functions
  
  bool init(int mMode= 1, int kMode= 1);          // ? START (see 'mode' variable for mouse & keyboard for more customization)
  void populate(bool scanMouseKeyboard= false);   // WIP, this might have more use in the future - calls EVERY init ATM 
  void update();                                  // manually update everything (mouse& keyboard& sticks& pads& wheels)
  void resetPressedButtons(); /// in case of ALT-TAB, all buttons/timers must be reset, to avoid bugs!!!
  
  Input();
  ~Input();
  void delData();
  
// private (internal) stuff from here on
 private:
   uint64 lastPopulate;
   
  #ifdef USING_DIRECTINPUT
  friend BOOL CALLBACK diDevCallback(LPCDIDEVICEINSTANCE, LPVOID);
  LPDIRECTINPUT8 dInput;
  #endif

  #ifdef OS_LINUX
  // linux keysyms handling (keysyms make windows look good ffs)
  // these funcs are in <OSchar.cpp>, at the end of the file !!!!!!!!!!!!!
  void keysym2unicode(KeySym *, ulong *ret);  /// converts keysym to unicode (no checks, use getUnicode)
  void getUnicode(KeySym *, ulong *ret);      /// converts keysym to unicode, verifies that the character is valid
  #endif /// OS_LINUX

  #ifdef OS_MAC // MAC MESS <<<--------------- NOTHING TO BOTHER HERE ------
  /// nothing to bother here, all internal vars
  IOHIDManagerRef manager;        /// [internal] 'manager' that handles all HID devices (this one is set to handle sticks/pads/wheels only)
  #endif // END MAC MESS <<<-----------------------------------------------

// TESTING
  void vibrate();
// TESTING ^^^^^^^^

};


extern Input in;  // only 1 global class






