/*
#include "osinteraction.h"
#include "util/typeShortcuts.h"
#include "osiVkRenderer.h"











osiVkRenderer::osiVkRenderer() {
  type= 1;
  monitor= null;
  GPU= null;
  vkDevice= null;
  vkGPU= null;

  nrQueues= 0;
  vkQueues= null;
}


osiVkRenderer::~osiVkRenderer() {
  // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkDestroyDevice.html
  //The lifetime of each of these objects is bound by the lifetime of the VkDevice object.
  //  Therefore, to avoid resource leaks, it is critical that an application explicitly free all of these resources
  //  prior to calling vkDestroyDevice.


  // vvv well these crash, so i guess the instance closes first or something like that
  //vk.DeviceWaitIdle(vkDevice);
  //vk.DestroyDevice(vkDevice, osi.settings.vulkan.memAllocCallback);

  // THESE ARE IN vkClose()
  if(vkQueues)
    delete[] vkQueues;
}













bool osiVkRenderer::vkCreateRenderer(osiVkRenderer **out_renderer, VkDevice in_device, VkPhysicalDevice in_physicalDevice) {
  if(out_renderer== null) { error.simple("osi::vkCreateRenderer(): out_renderer is null. aborting."); return false; }
  if(in_device== null) { error.simple("osi::vkCreateRenderer(): in_device is null. aborting."); *out_renderer= null; return false; }
  // in_physicalDevice could be null and i don't think that should be an error, it's up to the user. osiVkRenderer::GPU will be null

  *out_renderer= new osiVkRenderer;
  if(*out_renderer== null) { error.detail("Alloc failed", __FUNCTION__, __LINE__); return false; }
  osi.vkRenderers.add(*out_renderer);
  
  (*out_renderer)->GPU= null;
  (*out_renderer)->vkGPU= in_physicalDevice;
  (*out_renderer)->vkDevice= in_device;

  osiVk::_linkDeviceFuncs(&(*out_renderer)->vk, osi.vkInstance, (*out_renderer)->vkDevice);
  osiVk::_linkInstanceFuncs(&(*out_renderer)->vk, osi.vkInstance);
  osiVk::_populateVkQueue(*out_renderer);

  /// find out what osiGPU this is tied to
  for(int a= 0; a< osi.display.nrGPUs; a++)
    if(osi.display.GPU[a].vkGPU== in_physicalDevice) {
      (*out_renderer)->GPU= &osi.display.GPU[a];
      break;
    }

  return true;
}






osiVkRenderer *osiVkRenderer::vkCreateRendererMon(osiMonitor *in_m, VkDevice in_device) {
  if(!in_m) { error.detail("in_monitor is null", __FUNCTION__); return null; }
  if(!in_m->GPU) { error.detail("in_monitor's GPU is null", __FUNCTION__); return null; }
  if(!in_m->GPU->vkGPU) { error.detail("in_monitor's GPU's vkGPU is null", __FUNCTION__); return null; }

  osiVkRenderer *r= new osiVkRenderer;
  r->monitor= in_m;
  r->GPU= in_m->GPU;
  r->vkGPU= (VkPhysicalDevice)in_m->GPU->vkGPU;
  
  if(in_device)
    r->vkDevice= in_device;
  else
    if(!osiVk::_createDevice(r->vkGPU, &r->vkDevice, &r->nrQueues, &r->vkQueues)) { delete r; return null; }

  osiVk::_linkDeviceFuncs(&r->vk, osi.vkInstance, r->vkDevice);
  osiVk::_linkInstanceFuncs(&r->vk, osi.vkInstance);

  osiVk::_populateVkQueue(r);

  if(in_m->renderer== null)
    in_m->renderer= r;

  osi.vkRenderers.add(r);
  
  return r;
}


osiVkRenderer *osiVkRenderer::vkCreateRendererWin(osiWindow *in_w, VkDevice in_device) {
  osiVkRenderer *r= vkCreateRendererMon(in_w->monitor, in_device);
  if(r) {
    if(in_w->renderer== NULL)
      in_w->renderer= r;
    osiVk::createSurface(in_w);
  }
  return r;
}


osiVkRenderer *osiVkRenderer::vkAssignRenderer(osiWindow *in_w, VkDevice in_device) {
  osiVkRenderer *r;
  osiVk::_assignRenderer(in_w, in_device, &r);

  if(r) {
    if(in_w->renderer== NULL)
      in_w->renderer= r;
    osiVk::createSurface(in_w);
  }
  return r;
}




bool osiVkRenderer::vkLinkRendererToWindow(osiWindow *in_window, osiVkRenderer *in_renderer) {

  if(!in_window) { error.detail("window parameter null", __FUNCTION__); return false; }
  if(!in_renderer) { error.detail("renderer parameter null", __FUNCTION__); return false; }

  
  if(in_window->renderer== NULL)
    in_window->renderer= in_renderer;
  osiVk::createSurface(in_window);

  in_renderer->monitor= in_window->monitor;
  return true;
}






void osiVkRenderer::vkDelRenderer(osiVkRenderer *in_r) {
  /// if any window uses this renderer, set it to null
  for(short a= 0; a< OSI_MAX_WINDOWS; a++)
    if(osi.win[a].renderer== in_r)
      osi.win[a].renderer= null;
  
  /// if any monitor uses this renderer, set it to null
  for(short a= 0; a< osi.display.nrMonitors; a++)
    if(osi.display.monitor[a].renderer== in_r)
      osi.display.monitor[a].renderer= null;
  
  if(in_r->vkDevice) {
    if(in_r->vk.DeviceWaitIdle)
      in_r->vk.DeviceWaitIdle(in_r->vkDevice);
    else if(::vk.DeviceWaitIdle)
      ::vk.DeviceWaitIdle(in_r->vkDevice);

    if(in_r->vk.DestroyDevice)
      in_r->vk.DestroyDevice(in_r->vkDevice, osi.settings.vulkan.memAllocCallback);
    else if(::vk.DestroyDevice)
      ::vk.DestroyDevice(in_r->vkDevice, osi.settings.vulkan.memAllocCallback);

    in_r->vkDevice= null;
  }

  osi.vkRenderers.del(in_r);
}







*/





