#ifndef _FONTS_H_
#define _FONTS_H_

//#include "Arduino.h"
#include "ugui_config.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

//extern const ILI9341_t3_font_t Arial_8;

#ifdef USE_INTERNAL_FONTS

  extern const UG_FONT FONT_4X6;
  extern const UG_FONT FONT_5X8;
  extern const UG_FONT FONT_5X12;
  extern const UG_FONT FONT_6X8;
  extern const UG_FONT FONT_6X10;
  extern const UG_FONT FONT_7X12;
  extern const UG_FONT FONT_8X8;
  extern const UG_FONT FONT_8X12;
  extern const UG_FONT FONT_8X14;
  extern const UG_FONT FONT_10X16;
  extern const UG_FONT FONT_12X16;
  extern const UG_FONT FONT_12X20;
  extern const UG_FONT FONT_16X26;
  extern const UG_FONT FONT_22X36;
  extern const UG_FONT FONT_24X40;
  extern const UG_FONT FONT_32X53;

#else     // USE_RA8875_FONTS

  extern UG_FONT FONT_6X8;
  extern UG_FONT FONT_6X10;
  extern UG_FONT FONT_7X12;
  extern UG_FONT FONT_8X8;
  extern UG_FONT FONT_8X12;
  extern UG_FONT FONT_8X14;
  extern UG_FONT FONT_10X16;
  extern UG_FONT FONT_12X16;
  extern UG_FONT FONT_12X20;
  extern UG_FONT FONT_14X22;
  extern UG_FONT FONT_16X26;
  extern UG_FONT FONT_22X36;

#endif     // USE_INTERNAL_FONTS
/*
#ifdef __cplusplus
} // extern "C"
#endif 
*/
#endif

