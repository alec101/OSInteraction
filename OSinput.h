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
#define MAX_KEYS_LOGGED 8
#define MAX_JOYSTICKS 20          /// nr of maximum joysticks/gamepads/gamewheels, NOT JUST JOYSTICKS


// --------------============= MOUSE CLASS ============--------------
class Mouse {
public:
// USAGE / settings
  short mode;       //  [MODE 1]: OS events(default)  [MODE 2]: manual update() using different funcs   [MODE 3]: direct input (win)
  bool useDelta;    // use dx, dy (x,y, oldx, oldy are still updated)

/// position
  int x, y;
  int oldx, oldy;
/// delta x,y since last read. when program reads them, either set them to 0 after each read, or use getDx() & getDy()
  int dx, dy;         /// read these vars, and set them to 0, or call getDx()/getDy(). they track all movement until program processes them
  inline int getDx() { int tx= dx; dx= 0; return tx; }          /// returns mouse delta movement on y axis
  inline int getDy() { int ty= dy; dy= 0; return ty; }          /// returns mouse delta movement on x axis

/// wheel
  int wheel;        /// wheel delta rotation in units; get nr units rotated then set <wheel> to 0; or use getWheelDu();
  inline int getWheelDu() { int t= wheel; wheel= 0; return t; } /// returns wheel delta units

/// buttons
  struct Button {
    uint64 lastDT;          /// last button press delta time in milisecs
    uint64 lastTimeStart;   /// last button press start time
    uint64 lastTimeEnded;   /// last button press end time

    bool down;              /// button is currently pressed
    uint64 timeStart;       /// time @ button down start

    Button();
  } b[MAX_MOUSE_BUTTONS];

  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 diDevice;
  DIMOUSESTATE2 diStats;
  #endif

  bool aquire();
  bool unaquire();
  void resetButtons();

  void update();
  bool init(short mode);

  Mouse();
  ~Mouse();
  void delData();
};



// --------------============= KEYBOARD CLASS ============--------------
class Keyboard {
  uchar buffer1[MAX_KEYBOARD_KEYS], buffer2[MAX_KEYBOARD_KEYS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved

//  ulong charBuffer[256];        /// character buffer - used for input - strings / single chars
//  short nrCharsInBuffer;        /// nr of chars in charBuffer[]
//  ulong manipBuffer[256];       /// string manip char buffer - arrow keys, delete backspace, enter etc
//  short nrManipsInBuffer;       /// nr of chars in manipBuffer[]

public:
  bool init(short mode= 1);     // [MODE1]: OS events (default)  [MODE2]: manual update()  [MODE3]: directinput  [MODE4]: windows raw data
  void update();                /// MAIN LOOP FUNC: updates keys pressed array (key[] / lastCheck[])  (maybe toggle the locks? - N/A ATM)

  short mode;                   // [MODE1]: OS events (default)  [MODE2]: manual update()  [MODE3]: directinput  [MODE4]: windows raw data
  uchar *key;                   // all keys button status - it points to buffer1/buffer2. there is a clever swap between the two buffers, so there is no copying involved
  uint64 keyTime[256];          /// time @ key started to be pressed
  uchar *lastCheck;             /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  bool capsLock, scrollLock, numLock;     /// the 3 toggle locks

  struct KeyPressed {
    int code;                   /// scan code of the keyboard key. hopefully, these codes are os independant
    bool checked;               /// checked & lastKey[] used for mortal kombat style of keyboard check (3 buffer keys for slow pc/ unhandled drop in framerate)
    uint64 timeDown, timeUp, timeDT;   /// if a key is down, it has a time& timeDown is not 0 (same with timeUp) timeDT= time delta
    KeyPressed(const KeyPressed &o): code(o.code), checked(o.checked), timeDown(o.timeDown), timeUp(o.timeUp), timeDT(o.timeDT) {};
    KeyPressed() {code= 0; checked= false; timeDown= timeUp= timeDT= 0; }
  }lastKey[MAX_KEYS_LOGGED];     /// history of keys pressed


// to be or not to be - THIS REALLY SEEMS ARE USELESS (31.01.2014)
//  uint repeat[256];             /// how many times a character was repeated (usually this is done with WM_CHAR. if linux dont have a WM_CHAR, it might be used)
//  inline int getRepeat(int key) { uint t= repeat[key]; repeat[key]= 0; return t; }
// these might be useless ^^^



// character input/ character manipulation keys (enter/arrows/del/etc)
  class chTyped:public segData {/// uses the segment chainlist, check constructor in Keyboard()
  public:
    ulong c;
    uint64 time;
  };

// in charTyped.nrNodes / manipTyped.nrNodes is the nr of chars waiting to be processed (they get auto-del after 1-2 secs if not processed)
  segList charTyped;            /// list with chars typed. charTyped::nrNodes has nr of chars waiting to be 'read'. dimensions: [size:32, unitsize sizeof(chTyped)];
  segList manipTyped;           /// list with string manip chars (arrow keys, backspace, del, enter, etc)

// the main functions to call to get a char / string manip char
  ulong getChar();              /// returns a character typed @ keyboard or null or nothing is typed. (call it until it returns 0, or for each charTyped.nrNodes)
  ulong getManip();             /// returns a str manip key press. call it until (call it until it returns 0, or for each charTyped.nrNodes)
  void clearTypedBuffer();      /// clear both typed chars buffers (might want to clear them before a new input box, dunno) CLEAR THEM ON ALT-TAB TOO!!!!!!!!


// --- nothing to bother from this point on (usually) ---
  void addChar(ulong c, uint64 *time);  /// internal. used in WM_CHAR message... nothing to bother
  void addManip(ulong c, uint64 *time); /// internal. string manipulation keys - enter/del/arrow keys/etc
  void doManip();                       /// internal, OSchar.cpp. checks if current keys pressed form a char manip, if they do, calls addManip() 

  inline void swapBuffers();    /// swaps what key and lastKey point to (so no copying is involved)


/// more keyboard funcs, usually nothing to bother here, these are being used by OSInteraction
  bool aquire();                /// call after gaining focus (alt-tab...)
  bool unaquire();              /// call after losing focus (alt-tab...)
  void resetButtons();          /// call after losing focus too... if a key is pushed while alt-tabbing or somthing else, all keys that are pressed get messed up
  void updateLocks();           /// updates all the locks (caps, num, scroll)

  void log(const KeyPressed &); /// used internally, just puts the last key in the last key-history (it logs imediatly when a key is down)
  //void logd(const KeyDown &);   /// used internally, puts keydown in keydown history


  // TESTING
  short getFirstKey();
  void printPressed();


  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 diDevice;
  #endif /// USING_DIRECTINPUT

  Keyboard();
  ~Keyboard();
  void delData();
};



struct ButPressed {
  uchar b;                        /// button number
  bool checked;                   /// checked & lastKey[] used for mortal kombat style of keyboard check (3 buffer keys for slow pc/ unhandled drop in framerate)
  uint64 timeDown, timeUp, timeDT;  /// if a key is down, it has a time& timeDown is not 0 (same with timeUp) timeDT= time delta
  
  ButPressed(const ButPressed &o): checked(o.checked), timeDown(o.timeDown), timeUp(o.timeUp), timeDT(o.timeDT) {};
  ButPressed() {checked= false; timeDown= timeUp= timeDT= 0; }
};


// --------------============= JOYSTICK CLASS ============--------------
class Joystick {
  uchar buffer1[MAX_JOYSTICK_BUTTONS], buffer2[MAX_JOYSTICK_BUTTONS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved
  inline void swapBuffers();
public:
  short mode;   // [MODE 1]: sys default  [MODE 2]: xinput/ linux bla/mac bla [MODE 3]: direct input/ linux bla/mac bla
  string name;                     /// joystick name (product name)
  long x, y;                       /// main stick x and y axis
  long throttle;                   /// 3rd axis usually throttle
  long rudder;                     /// 4th axis usually rudder
  long u, v;                       /// fifth/ sixth axis
  long pov;                        /// POV angle (multiplied by 100, so 35,900(max)= 359 degrees)

/// buttons state / history / everything
  uchar *b;                             /// buttons state
  long bPressure[MAX_JOYSTICK_BUTTONS]; /// heck if k knew this existed... guess no game (that i played) uses it; now it's implemented!
  uint64 bTime[MAX_JOYSTICK_BUTTONS];   /// time @ key started to be pressed
  uchar *lastCheck;                     /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  ButPressed lastBut[MAX_KEYS_LOGGED];  /// history of pressed buttons

/// OS specific stuff
  #ifdef OS_WIN
  short id;                         // windows id (THIS MIGHT BE UNIVERSAL)
  #endif /// OS_WIN

  #ifdef USING_DIRECTINPUT            // primary
  LPDIRECTINPUTDEVICE8 diDevice;
  DIJOYSTATE2 diStats;
  #endif /// USING_DIRECTINPUT

  #ifdef USING_XINPUT                 // secondary, probly main joysticks are using direct input
  #endif /// USING_XINPUT

  #ifdef OS_LINUX
  int file;                           /// driver 'file'
  short id;                           /// /dev/input/js[X]
  #endif

// functions

  bool init(short mode);  // [MODE 1]: sys default  [MODE 2]: xinput/ linux bla/mac bla [MODE 3]: direct input/ linux bla/mac bla
  void aquire();
  void unaquire();

  void update();                    /// main update func

  void log(const ButPressed &);     /// used internally, just puts the last button in the last button-history (it logs imediatly when a button is down)

  Joystick();
  ~Joystick();
  void delData();
};



// --------------============= GAMEPAD CLASS ============--------------
class GamePad {
  uchar buffer1[MAX_JOYSTICK_BUTTONS], buffer2[MAX_JOYSTICK_BUTTONS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved
  inline void swapBuffers();
public:
  short mode;                     // [MODE1]: xinput/os variant 1 [MODE2]: directinput/os variant2 [MODE3] ????  [MODE 0]= not used not init NA
  string name;                   /// gamepad name (product name)

  long lx, ly;                   /// stick 1 axis position (left)
  long rx, ry;                   /// stick 2 axis position (right)
  long lt, rt;                   /// left and right triggers
  long u, v;                     /// extra axis - updated but usually not used... usually... can't know what pad they make
  long pov;                      /// POV angle (multiplied by 100, so 35,900(max)= 359 degrees) (-1 usually, if not pressed)

/// buttons state / history
  uchar *b;                             /// buttons state
  long bPressure[MAX_JOYSTICK_BUTTONS]; /// heck if k knew this existed... guess no game (that i played) uses it; now it's implemented!
  uint64 bTime[MAX_JOYSTICK_BUTTONS];   /// time @ key started to be pressed
  uchar *lastCheck;                     /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  ButPressed lastBut[MAX_KEYS_LOGGED];  /// history of pressed buttons


/// OS/ driver specific stuff
  #ifdef USING_XINPUT
  #endif /// USING_XINPUT

  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 diDevice;
  DIJOYSTATE2 diStats;
  LPDIRECTINPUTEFFECT vibration;
  #endif /// USING_DIRECTINPUT

  #ifdef OS_LINUX
  int file;                           /// driver 'file'
  short id;                           /// /dev/input/js[X] (shared between j/
  #endif

// functions

  bool init(short mode);        // [MODE1]: xinput/os variant 1 [MODE2]: directinput/os variant2 [MODE3] ????  [MODE 0]= not used not init NA
  bool aquire();
  bool unaquire();

  void update();

  void log(const ButPressed &);     /// used internally, just puts the last button in the last button-history (it logs imediatly when a button is down)

  GamePad();
  ~GamePad();
  void delData();
};



// --------------============= GAMEWHEEL CLASS ============--------------
class GameWheel {
  uchar buffer1[MAX_JOYSTICK_BUTTONS], buffer2[MAX_JOYSTICK_BUTTONS];   /// used for the key / lastCheck. buffers are swapped with pointers, so no copying is involved
  inline void swapBuffers();
public:
  short mode;                   // [MODE1]: xinput/os variant 1 [MODE2]: directinput/os variant2 [MODE3] ????
  string name;                 /// wheel name (product name)

  long wheel;                  /// the wheel
  long a1, a2, a3, a4, a5;     /// different axis/ pedals
  // a pov??                    // THIS NEEDS MORE WORK <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/// buttons state / history
  uchar *b;                             /// buttons state
  long bPressure[MAX_JOYSTICK_BUTTONS]; /// heck if k knew this existed... guess no game (that i played) uses it; now it's implemented!
  uint64 bTime[MAX_JOYSTICK_BUTTONS];   /// time @ key started to be pressed
  uchar *lastCheck;                     /// holds what the last time the keys were checked button press info - points to 1 of the buffers
  ButPressed lastBut[MAX_KEYS_LOGGED];  /// history of pressed buttons

/// OS / driver specific stuff
  #ifdef USING_DIRECTINPUT
  LPDIRECTINPUTDEVICE8 diDevice;
  DIJOYSTATE2 diStats;
  #endif /// USING_DIRECTINPUT

  #ifdef OS_LINUX
  int file;
  short id;                           /// /dev/input/js[X] (shared between j/
  #endif

// functions

  bool init(short mode);        // [MODE1]: xinput/os variant 1 [MODE2]: directinput/os variant2 [MODE3] ????
  bool aquire();
  bool unaquire();

  void update();

  void log(const ButPressed &);     /// used internally, just puts the last button in the last button-history (it logs imediatly when a button is down)

  GameWheel();
  ~GameWheel();
  void delData();
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

public:
  Mouse m;                      /// more than 1 mouse?
  Keyboard k;                   /// more than 1 keyboard?
  Joystick j[MAX_JOYSTICKS];    ///  j[0-7]= OS driver;  j[8-15] XINPUT;  j[16-19] DIRECT INPUT
  GamePad gp[MAX_JOYSTICKS];    /// gp[0-7]= OS driver; gp[8-15] XINPUT; gp[16-19] DIRECT INPUT
  GameWheel gw[MAX_JOYSTICKS];  /// gw[0-7]= OS driver; gw[8-15] XINPUT; gw[16-19] DIRECT INPUT
  _Kv Kv;                       /// struct with most inportant keycodes: in.k.key[Kv.space] is possible. OS independant/ if keyboard is changed in any way, just call Kv.populate()
  
  


  bool init(int mMode= 1, int kMode= 1);          // ? START
  void populate(bool scanMouseKeyboard= false);   // ? calls EVERY init ATM
  void update();

  void resetPressedButtons(); /// in case of ALT-TAB, all buttons/timers must be reset, to avoid bugs!!!

  struct InputNumbers {
    short jFound;             /// nr of joysticks found on system in total (for win, os+directinput+xinput)
    short gpFound;            /// nr of gamepads found on system
    short gwFound;            /// nr of gamewheels found on system

    short jOS;                /// (max 16) nr of normal driver joysticks found
    short gpOS;
    short gwOS;

    short jT2;                /// (max 12)	nr of directinput joysticks found (nothing in linux, but the code will compile)
    short gpT2;               /// (max 12)	nr of directinput gamepads found (nothing in linux, but the code will compile)
    short gwT2;               /// (max 12)	nr of directinput gamewheels found (nothing in linux, but the code will compile)

    short jT3;                /// (max 4)		nr of xinput joysticks found (nothing in linux, but the code will compile)
    short gpT3;               /// (max 4)		nr of xinput gamepads found (nothing in linux, but the code will compile)
    short gwT3;               /// (max 4)		nr of xinput gamewheels found (nothing in linux, but the code will compile)
  } nr;                       /// all different numbers of HID found

  inline Joystick  *getT2j (short nr) { return  &j[8+ nr]; } /// [win type2 = direct input] [linux= nothig] [mac= nothig]
  inline GamePad   *getT2gp(short nr) { return &gp[8+ nr]; } /// [win type2 = direct input] [linux= nothig] [mac= nothig]
  inline GameWheel *getT2gw(short nr) { return &gw[8+ nr]; } /// [win type2 = direct input] [linux= nothig] [mac= nothig]

  inline Joystick  *getT3j (short nr) { return  &j[16+ nr]; } /// [win type3= xinput] [linux= nothig] [mac= nothig]
  inline GamePad   *getT3gp(short nr) { return &gp[16+ nr]; } /// [win type3= xinput] [linux= nothig] [mac= nothig]
  inline GameWheel *getT3gw(short nr) { return &gw[16+ nr]; } /// [win type3= xinput] [linux= nothig] [mac= nothig]

  #ifdef USING_DIRECTINPUT
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


  Input();
  ~Input();
  void delData();
};


extern Input in;  // only 1 global class






