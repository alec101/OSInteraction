#include "../../include/osinteraction.h"

int main()
{
  osi.createGLWindow(&osi.win[0],
    &osi.display.monitor[0],
    "Win 0",
    500, 500,
    1);

  in.init();

  while(1)
  {
    osi.checkMSG();
    in.update();
    
    if(in.k.key[in.Kv.esc] || osi.flags.exit)
      break;
  }

  return 0;
}
