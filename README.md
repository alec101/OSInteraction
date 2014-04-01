It uses classes from https://github.com/alec101/-utilClasses 

check out the wiki (https://github.com/alec101/OSInteraction/wiki) for more info about this project

This is a setup for netbeans/xcode/visual studio/code blocks/codelite :
https://www.dropbox.com/s/gze6yepuhwn278k/OSInteraction.zip



check out pch.h for further compiling instructions & other info


Compiling on Windows:

1. Put OS_WIN as a preprocesor definition ( _CRT_SECURE_NO_WARNINGS is a nice addition, too)
2. WIN libs: [opengl32] [glu32]: openGL libraries 
             [winmm]:            crude windows joystick support
             [dinput8] [dxguid]: if using direct input (+ #define USING_DIRECTINPUT)
             [xinput]:           xinput 1.3, from dxsdk, maybe a path to it, too (+ #define USING_XINPUT)

Compiling on Linux:

1. Put OS_LINUX as a preprocesor definition (needs -DOS_LINUX if i remember correctly)
2. libraries: [GL] [GLU] [Xrandr] [Xinerama]
 
Compiling on Mac:

1. Put OS_MAC as a preprocessor definition ( -DOS_MAC )
2. frameworks: [-framework Opengl]: opengl library, basically
               [-framework cocoa]:  macOSX api
               [-framework IOKit]:  some monitor functions use this lib
   just place "-framework Opengl -framework cocoa -framework IOKit" in additional linker commands





A nice way to arrange the source (this is how i got setup thigs):

devYourDir/!utilClasses: https://github.com/alec101/-utilClasses 

devYourDir/OSInteraction/source: this git



devYourDir/OSInteraction/visualC/projectFile - if using Visual Studio

devYourDir/OSInteraction/xcode/projectFile - if using XCode

devYourDir/OSInteraction/netbeans/projectFile - if using NetBeans

etc

and include directories: ".." "../source" "../../!utilClasses"
