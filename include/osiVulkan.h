#pragma once


namespace osiVk {

  void init(osinteraction *o);
  void close();

  // surface handling funcs

  bool createSurface(vkObject *vk, osiWindow *in_win);      // creates VkSurfaceKHR on the window (osiWindow::vkSurface)
  void destroySurface(vkObject *vk, osiWindow *in_win);     // destroys the VkSurfaceKHR on the window (osiWindow::vkSurface)
  bool recreateSurface(vkObject *vk, osiWindow *in_win);    // call to try recreate the surface after a VK_ERROR_SURFACE_LOST_KHR error on the swapchain
  bool chooseVisual(vkObject *in_vk, osiWindow *in_win);    // chooses a visual that works with the device
  
}; /// namespace osiVk





















