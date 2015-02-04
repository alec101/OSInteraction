#include "osinteraction.h"                    // the only thing needed to be included

int main() {
  osi.createGLWindow(&osi.win[0],             // creates an OpenGL window - first parameter is what window object to use (there are 64 pre-alocated)
                     &osi.display.monitor[0], // specify on what monitor to create the window
                     "Win 0",                 // window name
                     500, 500,                // window size OR if fullscreen, what resolution to change the monitor to
                     1);                      // window mode: 1-normal window; 2-fullscreen; 3-fullscreen window(ignores size); 4-fullscreen window on all monitors(also ignores size)

  in.init();                                  // initializes mouse / keyboard / rest of HIDs

  while(1) {                                  // a basic program loop

    osi.checkMSG();                           // checks system messages
    in.update();                              // updates HIDs (mouse / keyboard / activated joysticks

    if(in.k.key[in.Kv.esc] || osi.flags.exit) // if escape key is pressed or the system signaled the program to close, break from the loop
      break;
  }
  return 0;
}
