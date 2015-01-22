
The wiki has detailed information on everything (https://github.com/alec101/OSInteraction/wiki)

SOME features:
--------------
* When you write a program using OSI, the same code will run under Windows, Linux and MacOS, without changing any line of code. 
* One of the main features of OSI is to create any number of OpenGL windows, on any number of monitors. If the system has multiple graphics cards, all of them can be used in rendering. Each of these windows can be a full screen window, normal window or a window that spans on **all system monitors** (this last type of window uses only 1 graphics card to render with OpenGL - to use all graphics cards, 1 full screen window per monitor must be created). Today's games and applications do not use more than 1 graphics card usually and for sure will not work on all 3 big operating systems. Imagine a strategy game: on one monitor you have the map, on the other, building queues, factories, maybe a radar etc... don't know why so very few games use this kind of feature.
* Mouse/Keyboard/Joystick/ Gamepad (every kind) / GameWheel (all main HID's) support, for all OS-es, with almost all driver types (XInput, DirectInput, linux joystick.h method, etc)
* The Keyboard class was designed to be able to build Mortal-Kombat style games, if you wish - it has a history of the keys that were pressed and their time when they were pressed and depressed. It has support for text input and editing in unicode and many more things, about everything you need from a keyboard.
* osiDisplay class scans every monitor connected to the system, and stores it's capabilities - resolutions frequencies, etc. This data is easy to access, and is sorted and stored in the same manner under Windows/Linux/Mac.
* High performance timers for all OS-es (same function names, same variable outputs).
* OpenGL functions that have different usage mode/names under each OS, are nicely combined under 1 function. (ex: call glMakeCurrent(....), which knows to call wglMakeCurrent under WIN, glxMakeCurrent under LINUX, blaBlaBla under MAC)
* tired of setting the locale to utf8/blabla, that just doesn't work on all systems? or just doesn't work, period? use supplied str8 class that uses only UTF-8 strings, or the UTF-32 class str32. Str namespace have functions that apply to UTF-8 / UTF-32 strings
* Tons of other nice stuff.

-
USAGE example:
--------------

    #include "osinteraction.h"                    // the only thing needed to be included

    main() {
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
    }


-
Needs tons of bug squishing - all this was done only by me (alec.paunescu@gmail.com), so i expect tons of bugs that i missed

-
Check out OSinteraction.h for further compiling instructions & other info

-
Needed libraries to compile:

* LINUX libraries: [X11] [GL] [GLU] [Xrandr] [Xinerama]
* WIN   libraries: [opengl32] [glu32] [d3d9] [dinput8] [dxguid] [xinput]
* MAC  frameworks: [-framework Opengl] [-framework cocoa] [-framework IOKit] [-framework CoreFoundation]

-
Detail library explanation:

    LINUX libs: [X11]      - libX base - don't leave home without it
                [GL] [GLU] - OpenGL libraries
                [Xrandr]   - used for monitor / GPU info / monitor resoulution changes
                [Xinerama] - used for monitor position info only

    WIN libs: [opengl32] [glu32] - OpenGL libraries

          if any dinput, xinput or direct3d are used, some directx sdk files (libs+includes) are provided, but directx sdk can be downloaded and used instead
          [d3d9]:             [#define USING_DIRECT3D] must be set in osinteraction.h - used ONLY for GPU detection (hopefully oGL will have an extension for this, in the future)
          [dinput8] [dxguid]: [#define USING_DIRECTINPUT] must be set in osinteration.h - used for direct input HIDs - joysticks gamepads etc
          [xinput]:           [#define USING_XINPUT] must be set in osinteraction.h - used for xinput HIDs - probly only gamepads
          
          the next libs should be auto-included, but here is the list in case something is missing:
          
            kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)


    MAC frameworks: [-framework Opengl]: opengl library, basically
                    [-framework cocoa]: macOSX api
                    [-framework IOKit]: some monitor functions use this lib
                    [-framework CoreFoundation]: used for simple error message boxes only


-
How to get the neccesary libs under Linux / Ubuntu:

* sudo apt-get install mesa-common-dev    GL/gl.h GL/glx.h
* sudo apt-get install libglu1-mesa-dev   GL/glu.h
* or the freeglu one
* sudo apt-get install libx11-dev         for X11/Xlib.h  - probably this is already installed
* sudo apt-get install libxrandr-dev      used for resolution changes
* sudo apt-get install libc6-dev-i386     the 32-bit C libraries (only 64bit libs are in linux64)
* sudo apt-get install xxxxxxxxxxxx       the 64-bit C libraries (only 32bit libs are in linux32)
* sudo apt-get install libxinerama-dev    Xinerama header files, used to identify monitors


-
###Licence: 
http://unlicense.org/ - so you can do whatever you want to do with this code - copy, change, wipe the floor, etc without any kind of pressure. Hope it helps!



