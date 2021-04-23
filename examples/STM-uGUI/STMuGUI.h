#ifndef STMuGUI_H
#define STMuGUI_H

#define DEBUG 0
#define SERIALDEBUG 1

/*
config for ILI9341:
uncomment #define ILI9341_TFT           in ugui_config.h
dimension is 320x240

config for RA8875:
comment out #define ILI9341_TFT         in ugui_config.h
set TFT_Y_RESOLUTION                    in ugui_config.h

*/

#ifdef RA8875_TFT
  #ifndef TFT_Y_RESOLUTION
    #error "TFT_Y_RESOLUTION not defined"
  #else
  #if (TFT_Y_RESOLUTION == 240)
    #define LCD_X_DIM 320 //320 //480 //480 //800
    #define LCD_Y_DIM 240 //240 //272 //320 //480
  #elif (TFT_Y_RESOLUTION == 272)
    #define LCD_X_DIM 480 //320 //480 //480 //800
    #define LCD_Y_DIM 272 //240 //272 //320 //480
  #elif (TFT_Y_RESOLUTION == 320)
    #define LCD_X_DIM 480 //320 //480 //480 //800
    #define LCD_Y_DIM 320 //240 //272 //320 //480
  #elif (TFT_Y_RESOLUTION == 480)
    #define LCD_X_DIM 800 //320 //480 //480 //800
    #define LCD_Y_DIM 480 //240 //272 //320 //480
  #else
    #error "TFT_Y_RESOLUTION not supported!"
  #endif
  #endif
  #define TOUCH_X_DIM 800
  #define TOUCH_Y_DIM 480
#else
  #define LCD_X_DIM 320
  #define LCD_Y_DIM 240
  #define TOUCH_X_DIM 320
  #define TOUCH_Y_DIM 240
#endif

#ifdef RA8875_TFT
  #define DISABLE_TOUCH_CAL
#endif

#define TFT_CS      PA4

#ifdef RA8875_TFT
#define TFT_RST     20
#else
#define TFT_DC      PA3
#define TFT_RST    PA0  // 255 = unused, connect to 3.3V
#endif

#define TFT_MOSI    PA7
#define TFT_SCLK    PA5
#define TFT_MISO    PA6

#ifndef RA8875_TFT
#define TS_CS_PIN  PA1
#endif

#define TOUCH_IRQ PA2
#define TS_IRQ      PA2

#define POINT_DEFAULT 500
#define POINT_X_MIN 260
//#define POINT_X_MIN 240
#define POINT_X_MAX 3690
//#define POINT_X_MAX 3400
#define POINT_Y_MIN 300
#define POINT_Y_MAX 3700
//#define POINT_Y_MAX 3300

#define WND_MAIN 0
#define OBJ_BUF_WND_MAIN 100
#define WND_TEMP_SET 1
#define OBJ_BUF_WND_TEMP_SET  15

//main window
#define BTN_ID_CONTROL  6
#define BTN_ID_PRINT    7

#define C_BUTTON_PRESSED  C_GREEN

typedef void (*voidFunct) (void);

struct CONFIG_t {
  uint16_t point_x_min;
  uint16_t point_x_max;
  uint16_t point_y_min;
  uint16_t point_y_max;
};

#ifdef RA8875_TFT
struct TS_point_t {
  uint16_t x;
  uint16_t y;
  uint16_t z;
};
#endif

#endif
