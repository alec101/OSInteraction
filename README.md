It uses classes from https://github.com/alec101/-utilClasses 

The wiki has detailed information on everything (https://github.com/alec101/OSInteraction/wiki)

SOME features:
--------------
* When you write a program using OSi, the same code will run under Windows, Linux and Mac OSX, without changing any line of code. 
* One of the main features of OSi is to create any number of OpenGL windows, on any number of monitors. If the system has multiple graphics cards, all of them can be used in rendering. Each of these windows can be a full screen window, normal window or a window that spans on **all system monitors** (this last type of window uses only 1 graphics card to render with OpenGL - to use all graphics cards, 1 full screen window per monitor must be created). Today's games and applications do not use more than 1 graphics card usually and for sure will not work on all 3 big operating systems. Imagine a strategy game: on one monitor you have the map, on the other, building queues, factories, maybe a radar etc... don't know why so very few games use this kind of feature.
* Mouse/Keyboard/Joystick/ Gamepad (every kind) / GameWheel (all main HID's) support, for all OS-es, with almost all driver types (XInput, DirectInput, linux joystick.h method, etc)
* The Keyboard class was designed to be able to build Mortal-Kombat style games, if you wish - it has a history of the keys that were pressed and their time when they were pressed and depressed. It has support for text input and editing in unicode and many more things, about everything you need from a keyboard.
* OSdisplay class scans every monitor connected to the system, and stores it's capabilities - resolutions frequencies, etc. This data is easy to access, and is sorted and stored in the same manner under Windows/Linux/Mac.
* High performance timers for all OS-es (same function names, same variable outputs).
* OpenGL functions that have different usage mode/names under each OS, are nicely combined under 1 function. (ex: call glMakeCurrent(....), which knows to call wglMakeCurrent under WIN, glxMakeCurrent under LINUX, blaBlaBla under MAC)
* Tons of other nice stuff.


This is a setup for netbeans/xcode/visual studio/code blocks/codelite :
https://www.dropbox.com/s/gze6yepuhwn278k/OSInteraction.zip



check out OSinteraction.h for further compiling instructions & other info


Compiling on Windows:

    libraries: [opengl32] [glu32]: openGL libraries 
               [winmm]:            crude windows joystick support
               [dinput8] [dxguid]: if using direct input (+ #define USING_DIRECTINPUT)
               [xinput]:           xinput 1.3, from dxsdk (+ #define USING_XINPUT)

Compiling on Linux:

    libraries: [GL] [GLU] [Xrandr] [Xinerama]
 
Compiling on Mac:

    frameworks: [-framework Opengl]: opengl library, basically
                [-framework cocoa]:  macOSX api
                [-framework IOKit]:  some monitor functions use this lib
    just place "-framework Opengl -framework cocoa -framework IOKit" in additional linker commands


  _If you use `USING_DIRECTINPUT` or `USING_XINPUT` compiler directives, be sure to place these defines in both OSinteraction and your project, if you use OSi as a library. Basicaly just make sure every project has the same defines._

-
A nice way to arrange the source (this is how i got setup thigs):

devYourDir/!utilClasses: https://github.com/alec101/-utilClasses 

devYourDir/OSInteraction/source: this git



devYourDir/OSInteraction/visualC/projectFile - if using Visual Studio

devYourDir/OSInteraction/xcode/projectFile - if using XCode

devYourDir/OSInteraction/netbeans/projectFile - if using NetBeans

etc

and include directories: ".." "../source" "../../!utilClasses"



###Licence: 
http://unlicense.org/ - so you can do whatever you want to do with this code - copy, change, wipe the floor, etc without any kind of pressure. Hope it helps!



