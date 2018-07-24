#pragma once

/*
USAGE ==========================================================================
  * just use error.simple() to print an error message * - fastest way

  useConsoleFlag:					use the ixConsole, it wont create any window
  useWindowsFlag:					use OS windows (MessageBox/ XBlaBla/ etc)
  both flags are false (DEFAULT): print to OS console / terminal / watever - just printf something
  if both flags are true, priority will be  ixConsole > OS window > OS terminal
  
  using window(...) will force a OS window only msg
  using console(...) will force a ixConsole only print - this class must be created before ixConsole
  using terminal(...) will force a OS terminal/ console / watever printf only
 ===============================================================================
*/

// 
#define __FUNC_LINE__ str8().f("f[%s] l[%d]", __FUNCTION__, __LINE__)
extern void (*(_FUNCconsole)) (const char *txt, bool exit, void (*exitFunc)(void));  // console print pointer. ixConsole sets this to it's own printing func, for example
#define errorMAKEME error.detail("makeme", __FUNCTION__, __LINE__, true)

#define USING_OPENGL 1    // << enables OpenGL error handling


class ErrorHandling {
public:
  
  // USAGE FLAGS
  
  bool useConsoleFlag;    // use the ConsoleClass (only)
  bool useWindowsFlag;    // use OS windows (MessageBox/ XBlaBla/ etc)

  // main call funcs
  
  void simple(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL);  // exitFunc: func to call before exit program
  void detail(const char *txt, const char *func, int line= -1, bool exit= false, void (*exitFunc)(void)= NULL);

  // these funcs will force a type of window/ print to something
  
  void window(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL);
  inline void console(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL) { _FUNCconsole(txt, exit, exitFunc); }
  void terminal(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL);

  

  #ifdef USING_DIRECTINPUT
  void dinput(int32_t nr);   // direct input error nr as text (msgbox etc)
  #endif
  
  #ifdef USING_OPENGL
  void glFlushErrors();
  int glError(const char *text= NULL, bool exit= false);   /// returns the error nr or 0, and prints with simple() func the error, IF there is one; text is used for additional text to print
  #endif /// OPENGL

  ErrorHandling();
  ~ErrorHandling();
  void delData();
  
private:
  #ifdef __linux__
  void messageBox(const char *text);   // linux specific messageBox window
  #endif /// OS_LINUX
};



extern ErrorHandling error; // only 1 global class




