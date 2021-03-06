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

#ifdef OSI_USE_OPENGL
#define USING_OPENGL 1    // << enables OpenGL error handling
#endif

//#ifdef OSI_USE_VKO
//#define USING_VULKAN 1
//#endif


class ErrorHandling {
public:
  
  // USAGE FLAGS
  
  bool useConsoleFlag;    // use the ConsoleClass (only)
  bool useWindowsFlag;    // use OS windows (MessageBox/ XBlaBla/ etc)

  // main call funcs
  
  void simple(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL);  // exitFunc: func to call before exit program
  void detail(const char *txt, const char *func, int line= -1, bool exit= false, void (*exitFunc)(void)= NULL);
  void makeme(const char *func, int line= -1, bool exit= false, void (*exitFunc)(void)= NULL);
  void alloc(const char *func= NULL, int line= -1, bool exit= false, void (*exitFunc)(void)= NULL);

  // these funcs will force a type of window/ print to something
  
  void window(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL);
  inline void console(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL) { _FUNCconsole(txt, exit, exitFunc); }
  void terminal(const char *txt, bool exit= false, void (*exitFunc)(void)= NULL);

  

  #ifdef USING_DIRECTINPUT
  void dinput(int64_t nr);   // direct input error nr as text (msgbox etc)
  #endif
  
  #ifdef USING_OPENGL
  void glFlushErrors();
  int glError(const char *text= NULL, bool exit= false);   /// returns the error nr or 0, and prints with simple() func the error, IF there is one; text is used for additional text to print
  #endif /// OPENGL

  #ifdef VK_VERSION_1_0
  // returns false on error, true on success
  inline bool vkCheck(VkResult in_err, const char *in_extraText= NULL, bool in_exit= false, void (*in_exitFunc)(void)= NULL) {
    if(in_err) { vkPrint(in_err, in_extraText, in_exit, in_exitFunc); return false; } else return true;
  }

  inline void vkPrint(VkResult in_err, const char *in_extraText= NULL, bool in_exit= false, void (*in_exitFunc)(void)= NULL) {
    if(!in_err) return;
    str8 s("Vulkan Error: ");
    s+= vkStrResult(in_err);
    
    if(in_extraText)
      s+= " ", s+= in_extraText;
    simple(s, in_exit, in_exitFunc);
  }

  inline void vkWindow(const char *in_text, const char *in_vkoErrText, VkResult in_res, bool in_exit= false, void (*in_exitFunc)(void)= NULL) {
    window(str8().f("%s\nVKO error: %s\nvkResult: %s", (in_text? in_text: ""), (in_vkoErrText? in_vkoErrText: ""), vkStrResult(in_res)), in_exit, in_exitFunc);
  }

  inline void vkSimple(const char *in_text, const char *in_vkoErrText, VkResult in_res, bool in_exit= false, void (*in_exitFunc)(void)= NULL) {
    simple(str8().f("%s\nVKO error: %s\nvkResult: %s", (in_text? in_text: ""), (in_vkoErrText? in_vkoErrText: ""), vkStrResult(in_res)), in_exit, in_exitFunc);
  }

  inline const char *vkStrResult(VkResult in_r) {
    if(in_r== VK_SUCCESS) return "VK_SUCCESS";
    else if(in_r== VK_NOT_READY)    return "VK_NOT_READY";
    else if(in_r== VK_TIMEOUT)      return "VK_TIMEOUT";
    else if(in_r== VK_EVENT_SET)    return "VK_EVENT_SET";
    else if(in_r== VK_EVENT_RESET)  return "VK_EVENT_RESET";
    else if(in_r== VK_INCOMPLETE)   return "VK_INCOMPLETE";
    else if(in_r== VK_ERROR_OUT_OF_HOST_MEMORY)     return "VK_ERROR_OUT_OF_HOST_MEMORY";
    else if(in_r== VK_ERROR_OUT_OF_DEVICE_MEMORY)   return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    else if(in_r== VK_ERROR_INITIALIZATION_FAILED)  return "VK_ERROR_INITIALIZATION_FAILED";
    else if(in_r== VK_ERROR_DEVICE_LOST)            return "VK_ERROR_DEVICE_LOST";
    else if(in_r== VK_ERROR_MEMORY_MAP_FAILED)      return "VK_ERROR_MEMORY_MAP_FAILED";
    else if(in_r== VK_ERROR_LAYER_NOT_PRESENT)      return "VK_ERROR_LAYER_NOT_PRESENT";
    else if(in_r== VK_ERROR_EXTENSION_NOT_PRESENT)  return "VK_ERROR_EXTENSION_NOT_PRESENT";
    else if(in_r== VK_ERROR_FEATURE_NOT_PRESENT)    return "VK_ERROR_FEATURE_NOT_PRESENT";
    else if(in_r== VK_ERROR_INCOMPATIBLE_DRIVER)    return "VK_ERROR_INCOMPATIBLE_DRIVER";
    else if(in_r== VK_ERROR_TOO_MANY_OBJECTS)       return "VK_ERROR_TOO_MANY_OBJECTS";
    else if(in_r== VK_ERROR_FORMAT_NOT_SUPPORTED)   return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    else if(in_r== VK_ERROR_FRAGMENTED_POOL)        return "VK_ERROR_FRAGMENTED_POOL";
    else if(in_r== VK_ERROR_OUT_OF_POOL_MEMORY)     return "VK_ERROR_OUT_OF_POOL_MEMORY";
    else if(in_r== VK_ERROR_INVALID_EXTERNAL_HANDLE) return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    else if(in_r== VK_ERROR_SURFACE_LOST_KHR)       return "VK_ERROR_SURFACE_LOST_KHR";
    else if(in_r== VK_ERROR_NATIVE_WINDOW_IN_USE_KHR) return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    else if(in_r== VK_SUBOPTIMAL_KHR)               return "VK_SUBOPTIMAL_KHR";
    else if(in_r== VK_ERROR_OUT_OF_DATE_KHR)        return "VK_ERROR_OUT_OF_DATE_KHR";
    else if(in_r== VK_ERROR_INCOMPATIBLE_DISPLAY_KHR) return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    else if(in_r== VK_ERROR_VALIDATION_FAILED_EXT)  return "VK_ERROR_VALIDATION_FAILED_EXT";
    else if(in_r== VK_ERROR_INVALID_SHADER_NV)      return "VK_ERROR_INVALID_SHADER_NV";
    else if(in_r== VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT) return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    else if(in_r== VK_ERROR_FRAGMENTATION_EXT)      return "VK_ERROR_FRAGMENTATION_EXT";
    else if(in_r== VK_ERROR_NOT_PERMITTED_EXT)      return "VK_ERROR_NOT_PERMITTED_EXT";
    else return "Unknown error";
  }


  #endif

  ErrorHandling();
  ~ErrorHandling();
  void delData();
  
private:
  #ifdef __linux__
  void messageBox(const char *text);   // linux specific messageBox window
  #endif /// OS_LINUX
};



extern ErrorHandling error; // only 1 global class




