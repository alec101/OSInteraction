#include "osinteraction.h"
#include "util/typeShortcuts.h"


namespace osiVk {




// init

void init(osinteraction *o) {
  o->display.populate(true);
}

// close everything Vulkan-related

void close(vkObject *in_vk) {
  for(int a= 0; a< OSI_MAX_WINDOWS; a++)
    if(osi.win[a].vkSurface) {
      in_vk->DestroySurfaceKHR(*in_vk, (VkSurfaceKHR)osi.win[a].vkSurface, *in_vk);
      osi.win[a].vkSurface= null;
    }
}


///=====================///
// VkSurfaceKHR handling //
///=====================///
  
bool createSurface(vkObject *in_vk, osiWindow *in_w) {
  if(in_w== NULL) { error.detail("window parameter is NULL", __FUNCTION__); return false; }
  if(in_w->vkSurface) //{ error.detail("window has already a surface", __FUNCTION__); return false; }
    return true;  // window has already a surface, no need to create another
  

  #ifdef OS_WIN
  if(in_vk->CreateWin32SurfaceKHR== null) { error.detail("vkCreateWin32Surface not avaible. vk_KHR_win32_surface must be enabled/ avaible", __FUNCTION__); return false; }

  VkWin32SurfaceCreateInfoKHR s;
  
  s.sType= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  s.pNext= null;
  s.flags= 0;
  s.hinstance= in_w->_hInstance;
  s.hwnd= in_w->_hWnd;
  
  if(in_vk->CreateWin32SurfaceKHR(*in_vk, &s, *in_vk, (VkSurfaceKHR *)&in_w->vkSurface)!= VK_SUCCESS) {
    error.detail("vkCreateWin32SurfaceKHR failed", __FUNCTION__);
    return false;
  }
  #endif /// OS_WIN
   
  #ifdef OS_LINUX
  makeme
  error.makeme(__FUNCTION__);
  #endif /// OS_LINUX


  #ifdef OS_MAC
  makeme
  error.makeme(__FUNCTION__);
  #endif /// OS_MAC

  return true;
}


void destroySurface(vkObject *in_vk, osiWindow *in_win) {
  if(in_win== null) { error.detail("in_window parameter null", __FUNCTION__); return; }
  if(in_vk->DestroySurfaceKHR== null) { error.detail("vk.DestroySurfaceKHR is not avaible. enable extension if not enabled.", __FUNCTION__); return; }

  if(in_win->vkSurface)
    in_vk->DestroySurfaceKHR(*in_vk, (VkSurfaceKHR)in_win->vkSurface, *in_vk);
  in_win->vkSurface= null;
}


bool recreateSurface(vkObject *in_vk, osiWindow *in_win) {
  destroySurface(in_vk, in_win);
  return createSurface(in_vk, in_win);
}




} // osiVk
























// VULKAN GETS HANDLED BY VKO



//#define OSI_VK_LINK_INSTANCE_FUNC(instance, func) func= (PFN_##func)vkGetInstanceProcAddr(instance, #func)
//#define OSI_VK_LINK_DEVICE_FUNC(device, func) func= (PFN_##func)vkGetDeviceProcAddr(device, #func)
/*
#define OSI_VK_LINK_INSTANCE_FUNC(instance, func) in_f->func= (PFN_vk##func)in_f->GetInstanceProcAddr(instance, "vk"#func)


#define OSI_VK_LINK_DEVICE_FUNC(func) if(in_d== null) in_f->func= (PFN_vk##func)in_f->GetInstanceProcAddr(in_i, "vk"#func); \
                                                 else in_f->func= (PFN_vk##func)in_f->GetDeviceProcAddr(in_d, "vk"#func)
                                                 */



  //chainList _validationLayers;      // [_ValidationLayer:chainData]

// init Vulkan



  /*
  // required OSI extensions - without these, OSI will not run properly

  /// on vulkan 1.0, OSI needs some extra extensions if possible
  if( (VK_VERSION_MAJOR(osi.vkApiVersion)== 1) && (VK_VERSION_MINOR(osi.vkApiVersion)== 0) ) {
    if(osi.settings.vulkan.extensions.instance.deprecated.vk_KHR_get_physical_device_properties2.isAvaible)
      osi.settings.vulkan.extensions.instance.deprecated.vk_KHR_get_physical_device_properties2.enable= true;
  }

  if(osi.settings.vulkan.extensions.instance.vk_KHR_surface.isAvaible)
    osi.settings.vulkan.extensions.instance.vk_KHR_surface.enable= true;

  #ifdef OS_WIN
  if(osi.settings.vulkan.extensions.instance.vk_KHR_win32_surface.isAvaible)
    osi.settings.vulkan.extensions.instance.vk_KHR_win32_surface.enable= true;
  #endif


  #ifdef OS_LINUX
  if(osi.settings.vulkan.extensions.instance.vk_KHR_xlib_surface.isAvaible)
    osi.settings.vulkan.extensions.instance.vk_KHR_xlib_surface.enable= true;
  if(osi.settings.vulkan.extensions.instance.vk_EXT_acquire_xlib_display.isAvaible)
    osi.settings.vulkan.extensions.instance.vk_EXT_acquire_xlib_display.enable= true;
  if(osi.settings.vulkan.extensions.instance.vk_EXT_direct_mode_display.isAvaible)
    osi.settings.vulkan.extensions.instance.vk_EXT_direct_mode_display.enable= true;
  if(osi.settings.vulkan.extensions.instance.vk_KHR_display.isAvaible)
    osi.settings.vulkan.extensions.instance.vk_KHR_display.enable= true;
  #endif

  #ifdef OS_MAC
  if(osi.settings.vulkan.extensions.instance.vk_MVK_macos_surface.isAvaible)
    osi.settings.vulkan.extensions.instance.vk_MVK_macos_surface.enable= true;
  #endif
  */



/* VULKAN GETS HANDLED BY VKO

// creates or assigns an existing renderer to the window. it's used for vkCreateWindow, 
// the vulkan device is created using osi.settings.vulkan
// <out_r> will have the renderer assigned
bool _assignRenderer(osiWindow *in_w, VkDevice in_device, osiVkRenderer **out_r) {
  
#ifndef OS_MAC
//  make testchamber funcion in mac
    //then see to install moltenvk, sdk too, try at least see if stuff starts or works...
    //with such wrap under wrap under wrap it might not, let's face it...
    //who knows...
#endif

  // make sure w and w's monitor are not null
  if(!in_w) return false;
  if(!in_w->monitor) return false;

  osiVkRenderer *r= null;
  bool createRenderer= false;

  /// create new / assign existing renderer based on current settings
  if(osi.settings.vulkan.customRenderer) {                // using provided custom renderer
    r= osi.settings.vulkan.customRenderer;

  } else if(osi.settings.rendererCreation== osiSettings::onePerGPU) {      // one renderer per GPU

    osiVkRenderer *p= (osiVkRenderer *)osi.vkRenderers.first;
    for(;p; p= (osiVkRenderer *)p->next)
      if(p->GPU== in_w->monitor->GPU)
        break;

    /// found an already created renderer?
    if(p)
      r= p;
    else
      createRenderer= true;

  } else if(osi.settings.rendererCreation== osiSettings::onePerMonitor) {  // one renderer per monitor
    osiVkRenderer *p= (osiVkRenderer *)osi.vkRenderers.first;
    for(;p; p= (osiVkRenderer *)p->next)
      if(p->monitor== in_w->monitor)
        break;

    if(p)
      r= p;
    else
      createRenderer= true;
      

  } else if(osi.settings.rendererCreation== osiSettings::onePerWindow) {      // one renderer per window
    if(in_w->renderer)
      if(in_w->renderer->type== 1)
        r= (osiVkRenderer *)in_w->renderer;

    if(r== null)
      createRenderer= true;

  } else if(osi.settings.rendererCreation== osiSettings::onlyOne) {         // work with only one renderer (default)
    r= (osiVkRenderer *)osi.vkRenderers.first;
    if(r== null)
      createRenderer= true;
  }
  //else if(settings.renderer.noAutocreateRenderer)     // do not create any renderer
    //; // do nothing

  // create a renderer if it is needed
  if(createRenderer) {
    //r= new osiVkRenderer;
    //osi.vkRenderers.add(r);

    uint nrQueues= 0;
    osiVkQueue *vkQueues= null;

    if(in_device== null)
      //if(!_createDevice((VkPhysicalDevice)in_w->monitor->GPU->vkGPU, &in_device, &r->nrQueues, &r->vkQueues))
      if(!_createDevice((VkPhysicalDevice)in_w->monitor->GPU->vkGPU, &in_device, &nrQueues, &vkQueues))
        return false;
    if(!osi.vkCreateRenderer(&r, in_device, (VkPhysicalDevice)in_w->monitor->GPU->vkGPU))
      return false;

    r->vkQueues= vkQueues;
    r->nrQueues= nrQueues;
    _populateVkQueue(r);
  }

  if(r== null) return false;

  // link all the variables
  r->GPU= in_w->monitor->GPU;
  r->monitor= in_w->monitor;
  in_w->renderer= r;
  in_w->monitor->renderer= r;
  //vkCreateSurface(in_w);      /// make sure the window has a vkSurface

  // return value
  if(out_r)
    *out_r= r;

  return true;  
}
*/














