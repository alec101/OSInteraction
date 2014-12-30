#pragma once

/*
USAGE ==========================================================================
  * just use error.simple() to print an error message * - fastest way

  useConsoleFlag:					use the ConsoleClass, it wont create any window
  useWindowsFlag:					use OS windows (MessageBox/ XBlaBla/ etc)
  both flags are false (DEFAULT): print to OS console / terminal / watever - just printf something
  
  using window(...) will force a OS window only msg
  using console(...) will force a ConsoleClass only print
  using terminal(...) will force a OS terminal/ console / watever printf only
 ===============================================================================
*/



//OPEN GL SPECIFIC ERRORZ:
//glErrNr-GL_INVALID_ENUM+GLENUM

#define USING_OPENGL

class ErrorHandling {
  #ifdef __linux__
  void messageBox(cchar *text);   // linux specific messageBox window
  #endif /// OS_LINUX
    
public:
// USAGE FLAGS
  bool useConsoleFlag;    // use the ConsoleClass (only)
  bool useWindowsFlag;    // use OS windows (MessageBox/ XBlaBla/ etc)

// main call funcs
  void simple(cchar *txt, bool exit= false, void (*exitFunc)(void)= null);  // exitFunc: func to call before exit program
  
// these funcs will force a type of window/ print to something
  void window(cchar *txt, bool exit= false, void (*exitFunc)(void)= null);
  void console(cchar *txt, bool exit= false, void (*exitFunc)(void)= null);
  void terminal(cchar *txt, bool exit= false, void (*exitFunc)(void)= null);
  
  #ifdef USING_DIRECTINPUT
  void dinput(int32 nr);   // direct input error nr as text (msgbox etc)
  #endif
  
  #ifdef USING_OPENGL
  inline void glFlushErrors() { glGetError(); }
  int glError(cchar *text= null);     /// returns the error nr or 0, and prints with simple() func the error, IF there is one; text is used for additional text to print
  #endif /// OPENGL

  ErrorHandling();
  ~ErrorHandling();
  void delData();
};

extern ErrorHandling error; // only 1 global class




