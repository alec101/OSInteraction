#include "vko/include/vkoPlatform.h"
#include "osinteraction.h"
#include "util/typeShortcuts.h"
#ifdef OSI_BE_CHATTY
#include <stdio.h>
#endif



#ifdef OSI_USE_VKO
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
  s.hinstance= (HINSTANCE)in_w->_hInstance;
  s.hwnd= (HWND)in_w->_hWnd;
  
  if(in_vk->CreateWin32SurfaceKHR(*in_vk, &s, *in_vk, (VkSurfaceKHR *)&in_w->vkSurface)!= VK_SUCCESS) {
    error.detail("vkCreateWin32SurfaceKHR failed", __FUNCTION__);
    return false;
  }
  #endif /// OS_WIN
   
  #ifdef OS_LINUX
  if(in_vk->CreateXlibSurfaceKHR== null) { error.detail("CreateXlibSurfaceKHR not avaible. vk_KHR_xlib_surface must be enabled/ avaible", __FUNCTION__); return false; }
  
  VkXlibSurfaceCreateInfoKHR s;
  s.sType= VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
  s.pNext= null;
  s.flags= 0;             // no flags as of 2021
  s.dpy= in_w->_dis;
  s.window= in_w->_win;
  
  if(in_vk->CreateXlibSurfaceKHR(*in_vk, &s, *in_vk, (VkSurfaceKHR *)&in_w->vkSurface)!= VK_SUCCESS) {
    error.detail("vkCreateXlibSurfaceKHR failed", __FUNCTION__);
    return false;
  }
  #endif /// OS_LINUX


  #ifdef OS_MAC
  makeme
  VK_EXT_metal_surface vs VK_MVK_macos_surface ??? i'm not sure if any difference - i'm guessing the metal surface is newer
  so might as well just go directly for a metal surface
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










/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xcomposite.h>

#include <stdio.h>

int main(int argc, char **argv)
{
  Display *dpy;
  XVisualInfo vinfo;
  int depth;
  XVisualInfo *visual_list;
  XVisualInfo visual_template;
  int nxvisuals;
  int i;
  XSetWindowAttributes attrs;
  Window parent;
  Visual *visual;

  dpy = XOpenDisplay(NULL);

  nxvisuals = 0;
  visual_template.screen = DefaultScreen(dpy);
  visual_list = XGetVisualInfo (dpy, VisualScreenMask, &visual_template, &nxvisuals);

  for (i = 0; i < nxvisuals; ++i)
    {
      printf("  %3d: visual 0x%lx class %d (%s) depth %d\n",
             i,
             visual_list[i].visualid,
             visual_list[i].class,
             visual_list[i].class == TrueColor ? "TrueColor" : "unknown",
             visual_list[i].depth);
    }


  if (!XMatchVisualInfo(dpy, XDefaultScreen(dpy), 32, TrueColor, &vinfo)) {
    fprintf(stderr, "no such visual\n");
    return 1;
  }

  printf("Matched visual 0x%lx class %d (%s) depth %d\n",
         vinfo.visualid,
         vinfo.class,
         vinfo.class == TrueColor ? "TrueColor" : "unknown",
         vinfo.depth);

  parent = XDefaultRootWindow(dpy);

  XSync(dpy, True);

  printf("creating RGBA child\n");

  visual = vinfo.visual;
  depth = vinfo.depth;

  attrs.colormap = XCreateColormap(dpy, XDefaultRootWindow(dpy), visual, AllocNone);
  attrs.background_pixel = 0;
  attrs.border_pixel = 0;

  XCreateWindow(dpy, parent, 10, 10, 150, 100, 0, depth, InputOutput,
                visual, CWBackPixel | CWColormap | CWBorderPixel, &attrs);

  XSync(dpy, True);

  printf("No error\n");

  return 0;
}

*/




#ifdef OS_LINUX
bool chooseVisual(vkObject *in_vk, osiWindow *in_win) {
  XVisualInfo viRequest;
  XVisualInfo *ret= null;                 // INIT 1
  int nrRet;
  const char *err= nullptr;
  int errL= 0;
  VkoQueue *q= nullptr;
  #ifdef OSI_BE_CHATTY
  bool chatty= true;
  #endif

  if(in_win->_vi) XFree(in_win->_vi);
  in_win->_vi= nullptr;

  //if(in_vk->GetPhysicalDeviceXlibPresentationSupportKHR== null) {
  //  err= "vk->GetPhysicalDeviceXlibPresentationSupportKHR() not avaible", errL= __LINE__; goto Exit;
  //}
  
  /// get the first graphics queue in Vko
  //for(uint32_t a= 0; a< in_vk->nrQueues; a++)
  //  if(in_vk->queue[a].typeFlags& VK_QUEUE_GRAPHICS_BIT)
  //   q= &in_vk->queue[a];
  
  //if(q== nullptr) {
  //  err= "No graphics queue setup before creating a window; one is needed to choose a proper visual", errL= __LINE__;
  //  goto Exit;
  //}
  
  // try 2 times to get a proper visual; once for 32bit visuals, once for 24bit visuals
  for(int d= 0; d< 2; d++) {
    if(ret) XFree(ret);
    ret= nullptr;
    nrRet= 0;
    
    /// request a visual supported by the current screen and has 24/32 bit depth
      //#define VisualNoMask            0x0
      //#define VisualIDMask            0x1
      //#define VisualScreenMask        0x2
      //#define VisualDepthMask         0x4
      //#define VisualClassMask         0x8
      //#define VisualRedMaskMask       0x10
      //#define VisualGreenMaskMask     0x20
      //#define VisualBlueMaskMask      0x40
      //#define VisualColormapSizeMask  0x80
      //#define VisualBitsPerRGBMask    0x100
      //#define VisualAllMask           0x1FF
    viRequest.screen= DefaultScreen(osi._dis);
    if     (d== 0) viRequest.depth= 24;
    else if(d== 1) viRequest.depth= 32;
    ret= XGetVisualInfo(osi._dis, VisualScreenMask| VisualDepthMask, &viRequest, &nrRet); // ALLOC 1
      
    // loop thru all visuals returned, find one that is accepted by vulkan
    for(int a= 0; a< nrRet; a++) {
      //if(in_vk->GetPhysicalDeviceXlibPresentationSupportKHR(*in_vk, q->family, osi._dis, ret[a].visualid)== VK_TRUE) {
        viRequest.visualid= ret[a].visualid;
        in_win->_vi= XGetVisualInfo(osi._dis, VisualIDMask, &viRequest, &nrRet);
        #ifdef OSI_BE_CHATTY
        if(chatty)
          printf("[%d] total visuals, choosing visual: [%d]depth [%d]index\n", nrRet, in_win->_vi->depth, a);
        #endif
        goto Exit;
      //}
    }
  }
  
Exit:
  if(ret) XFree(ret);     // DEALLOC 1

  if(err!= nullptr) {
    error.detail(err, __FUNCTION__, errL);
    
    error.window("STOP"); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,,
    
    return false;
  } else
    return true;

}
#endif /// OS_LINUX

} // osiVk

#endif /// OSI_USE_VKO






















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














