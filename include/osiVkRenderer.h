/*
#pragma once
#include "osiRenderer.h"




///========================================================///
// VKRENDERER class - manages Vulkan devices and extensions //
///========================================================///



// WIP _vkr: this is set by osi by default to the first created vulkan device. should be ignored if using multiple devices WIP
//osiVkRenderer _vkr;




class osiVkRenderer: public osiRenderer {
public:

  //_osiVkFuncs vk;               // vulkan functions, linked directly to the renderer's device

  VkPhysicalDevice vkGPU;       // vulkan GPU
  //VkDevice vkDevice;            // vulkan device
  //VkQueue vkQueue;              // vulkan queue

  //uint32_t nrQueues;
  //osiVk::osiVkQueue *vkQueues;

  // it's best you create your own vulkan device, but you can let osi to create it (using settings.vulkan)
  // if you want to let osi handle the device, just let <in_device> NULL
  static osiVkRenderer *vkCreateRendererMon(osiMonitor *, VkDevice in_device= NULL); 
  static osiVkRenderer *vkCreateRendererWin(osiWindow *, VkDevice in_device= NULL);
  static osiVkRenderer *vkAssignRenderer(osiWindow *, VkDevice in_device= NULL);

  static void vkDelRenderer(osiVkRenderer *);

  // creates a vulkan osi renderer from the specified already created vulkan device.
  // you must create the device yourself. This func is mainly used for other than drawing types of vulkan devices
  // it will link the vulkan funcs to it so it will be a ready to use osi renderer
  static bool vkCreateRenderer(osiVkRenderer **out_renderer, VkDevice in_device, VkPhysicalDevice in_physicalDevice);

  // makes sure there's a vkSurface created on that window
  static bool vkLinkRendererToWindow(osiWindow *in_window, osiVkRenderer *in_renderer);


  // constructor / destructor

  osiVkRenderer();
  ~osiVkRenderer();
  //void delData();


private:
  friend class osinteraction;
  friend void osiVk::_populateVkQueue(osiVkRenderer *);
};


*/








