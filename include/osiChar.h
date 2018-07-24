#pragma once


// -these constants are for in.k.charTyped: stream with unicode characters / string manipulation characters
// -keys that have same character meaning, are combined:
//    keypad enter and enter have the same Kch_enter constant
//    keypad down arrow and normal down arrow have the same Kch_down constant ETC.

#define Kch_enter     '\n'
#define Kch_backSpace '\b'
#define Kch_delete    0xFFFF0000
#define Kch_home      0xFFFF0001
#define Kch_end       0xFFFF0002
#define Kch_pgUp      0xFFFF0003
#define Kch_pgDown    0xFFFF0004
#define Kch_left      0xFFFF0005
#define Kch_up        0xFFFF0006
#define Kch_right     0xFFFF0007
#define Kch_down      0xFFFF0008

#define Kch_cut       0xFFFF0010      // ctrl+ x or ctrl+ delete
#define Kch_copy      0xFFFF0011      // ctrl+ c or ctrl+ insert
#define Kch_paste     0xFFFF0012      // ctrl+ v or shift+ insert
  
#define Kch_selHome   0xFFFF0020      // selection change: shift+ home
#define Kch_selEnd    0xFFFF0021      // selection change: shift+ end
#define Kch_selPgUp   0xFFFF0022      // selection change: shift+ pgUp
#define Kch_selPgDown 0xFFFF0023      // selection change: shift+ pgDown
#define Kch_selLeft   0xFFFF0024      // selection change: shift+ left
#define Kch_selUp     0xFFFF0025      // selection change: shift+ up
#define Kch_selRight  0xFFFF0026      // selection change: shift+ right
#define Kch_selDown   0xFFFF0027      // selection change: shift+ down


// THE OLD MANIP CHAR, BEFORE THE char/manip MERGE:
/*
#define Kch_enter     '\n'
#define Kch_backSpace '\b'
#define Kch_delete    0x10 0xFFFF0000
#define Kch_home      0x11
#define Kch_end       0x12
#define Kch_pgUp      0x13
#define Kch_pgDown    0x14
#define Kch_left      0x15
#define Kch_up        0x16
#define Kch_right     0x17
#define Kch_down      0x18

#define Kch_cut       0x20      // ctrl+ x or ctrl+ delete
#define Kch_copy      0x21      // ctrl+ c or ctrl+ insert
#define Kch_paste     0x22      // ctrl+ v or shift+ insert
  
#define Kch_selHome   0x30      // selection change: shift+ home
#define Kch_selEnd    0x31      // selection change: shift+ end
#define Kch_selPgUp   0x32      // selection change: shift+ pgUp
#define Kch_selPgDown 0x33      // selection change: shift+ pgDown
#define Kch_selLeft   0x34      // selection change: shift+ left
#define Kch_selUp     0x35      // selection change: shift+ up
#define Kch_selRight  0x36      // selection change: shift+ right
#define Kch_selDown   0x37      // selection change: shift+ down
*/





