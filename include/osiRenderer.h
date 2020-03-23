#pragma once


class osiRenderer: public chainData {
public:
  int8_t type;  // 0= opengl, 1= vulkan   // VULKAN SCRAPED- VKO handles everything - each vulkan window will still have one for INFO

  osiMonitor *monitor;      // the monitor on which the renderer was created. assigning a renderer to render to a different monitor that is handled by a different GPU, can be bad
  osiGPU *GPU;              // GPU it belongs to
};



















