# uGUI-Lib
uGUI Arduino Library

original see here: https://github.com/achimdoebler/UGUI


/* -------------------------------------------------------------------------------- */
/* -- µGUI - Generic GUI module (C)Achim Döbler, 2015                            -- */
/* -------------------------------------------------------------------------------- */
// µGUI is a generic GUI module for embedded systems.
// This is a free software that is open for education, research and commercial
// developments under license policy of following terms.
//
//  Copyright (C) 2015, Achim Döbler, all rights reserved.
//  URL: http://www.embeddedlightning.com/
//
// * The µGUI module is a free software and there is NO WARRANTY.
// * No restriction on use. You can use, modify and redistribute it for
//   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
// * Redistributions of source code must retain the above copyright notice.
//
//
// adapted as Arduino library by AndiS
// changed content:
//
//  - use of dynamic memory allocation or static memory allocation. Switch via #define USE_STATIC_MEMORY in ugui_conf.h
//    if you use static memory then you have to provide all memory locations via global variables as in the original library.
//  - checkboxes have no longer text attached (the size of checkboxes was dependend of used fontsize, this annoyed me)
//      so the size of checkboxes are free to choose. If you want text near a checkbox then add a textbox
//  - added UG_ButtonTouchEnable(), UG_ButtonTouchDisable()
//  - added UG_ButtonCreate, UG_TextboxCreate, UG_CheckboxCreate with relative (width,hight) parameters
//      e.g. gui.UG_ButtonCreate(*window, id, x_pos, y_pos, width, height, UG_REL);
//  - added Progressbar functions
//  - added Radiobutton functions
//  - added PREUPDATE, POSTPDATE events. You have to activate them in ugui_conf.h, BUT never tested
//
//  - added RA8875 functionality for accelerated text output. You should also define drivers for accelerated output.
//    Reason: RA8875 displays are very, yes, very slow to set single pixels, but can draw lines, rectangles etc. very fast.
//    Add RA8875 library from sumotoy.
//    To activate uncomment #define RA8875_TFT
/* -------------------------------------------------------------------------------- */

