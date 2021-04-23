// Example for ILI9341 Touchscreen-TFT
// uGUI setup for static and dynamic memory allocation

//  #define RA8875_TFT

#include "SPI.h"
#include <UGUI.h>
#include "UGUI_Test.h"
#include "fonts.h"

#ifndef RA8875_TFT
  #ifdef __arm__
    #include <ILI9341_t3.h>
  #else
    #include "Adafruit_GFX.h"
    #include "Adafruit_ILI9341.h"
  #endif
  #include <XPT2046_Touchscreen.h>
#elif defined RA8875_TFT
  #include <RA8875.h>
/*
  //include used fonts
  #include"fonts/font_8x12.c"
  #include"fonts/font_10x16.c"
  #include"fonts/font_12x20.c"
  //#include"fonts/font_14x22.c"
  #include"fonts/font_16x26.c"
*/
#else
  #error "No supported TFT"
#endif

#ifdef RA8875_TFT
  #ifdef __arm__
  RA8875 tft = RA8875(TFT_CS, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
  #else
  RA8875 tft = RA8875(2,3);
  #endif
#else
  #ifdef __arm__
  // For optimized ILI9341_t3 library
    ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
  #else
    Adafruit_ILI9341 tft = Adafruit_ILI9341(2,3,4);
  #endif
  XPT2046_Touchscreen ts(TS_CS_PIN, TOUCH_IRQ);
#endif

#ifdef RA8875_TFT
  TS_point_t p;
  uint8_t tft_brightness = 100;
#else
  TS_Point p;
#endif

  int touch;
  int point_x_min, point_x_max, point_y_min, point_y_max;


#ifndef USE_STATIC_MEMORY

UGUI  gui = UGUI(4, 320, 240);       // 4 windows, dimension 320x240

#else

UG_GUI mygui;
UGUI gui = UGUI(&mygui);
UG_WINDOW mainWindow;
UG_OBJECT  obj_buf_wnd_main[OBJ_BUF_WND_MAIN];
UG_BUTTON btnControl;
UG_BUTTON btnPrint;
UG_CHECKBOX chbControl;
UG_PROGRESSBAR pgrControl;
UG_RADIOBUTTON rabControl;

#endif

const char TitleText[] = "Hallo";
const char Str1[] = "Hello World";
const char Button1Txt[] = "Btn 1";
const char Button2Txt[] = "Btn 2";
const char Chb1Txt[] = "Chb 1";

//*****************************************************************************************//
void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("start setup");
  point_x_min = 420;
  point_x_max = 3600;
  point_y_min = 300;
  point_y_max = 3730;
  
#ifdef RA8875_TFT
//  tft.touchSetBorder (point_x_min, point_x_max, point_y_min, point_y_max);
  tft.begin(RA8875_800x480);//initialize library
  tft.useINT(TOUCH_IRQ);//We use generic int helper for Internal Resistive Touch
  tft.touchBegin();//enable Touch support!
//  tft.enableISR(true);
  tft.setRotation(1);
#else
  pinMode(TOUCH_IRQ,INPUT_PULLUP);
  tft.begin();
  ts.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
#endif

Serial.println("gui setup");
#ifndef USE_STATIC_MEMORY
  gui.UG_Init(setPixel);
#else
  gui.UG_Init(&mygui, setPixel, 320, 240);
//  gui.UG_SelectGUI(&mygui);
#endif
Serial.println("gui init done");
  gui.UG_DriverRegister(DRIVER_DRAW_LINE, (void*)HW_DrawLine);
  gui.UG_DriverRegister(DRIVER_FILL_FRAME, (void*)HW_FillFrame);
  gui.UG_DriverRegister(DRIVER_FILL_AREA, (void*)HW_FillFrame);
  gui.UG_DriverRegister(DRIVER_FILL_SCREEN, (void*)HW_FillScreen);
  gui.UG_FontSelect (&FONT_8X12);
Serial.println("FontSelect done");
  gui.UG_PutString(100,100,(char*)Str1);
Serial.println("PutString done");
  delay(1000);
  setupWindows();
Serial.println("setupWindows done");

#ifndef USE_STATIC_MEMORY
  gui.UG_WindowShow(gui.window[WND_MAIN]);
#else
  gui.UG_WindowShow(&mainWindow);
#endif

Serial.println("show mainWindow done");
  Serial.println("setup done");
}

//*****************************************************************************************//
void loop() {

  static uint32_t timeOld;
  static uint32_t counter1 = 0;
  static size_t counter2 = 0;
  static UG_U8 counterProgressbar = 90;
  
  uint32_t timeNow = millis();
  if ((timeNow - timeOld) >= 50) {
    timeOld = timeNow;
    counter1++;
    counter2++;
    
    getTouch();
    
    if (counter1 == 20) {
#ifndef USE_STATIC_MEMORY
      gui.UG_ButtonSetText(gui.window[WND_MAIN], BTN_ID_CONTROL, (char*)TitleText);
      gui.UG_ButtonSetHSpace(gui.window[WND_MAIN], BTN_ID_CONTROL,1);
      //gui.UG_ButtonHide(gui.window[WND_MAIN], BTN_ID_CONTROL);
#else
      gui.UG_ButtonSetText(&mainWindow, BTN_ID_CONTROL, (char*)TitleText);
      gui.UG_ButtonSetHSpace(&mainWindow, BTN_ID_CONTROL,1);
#endif      
      if (counterProgressbar <=100) {
#ifndef USE_STATIC_MEMORY
      gui.UG_ProgressbarSetPercent(gui.window[WND_MAIN], PRB_ID_COUNT, counterProgressbar);
#else
	  gui.UG_ProgressbarSetPercent(&mainWindow, PRB_ID_COUNT, counterProgressbar);
#endif
      counterProgressbar++;
      if (counterProgressbar == 101) counterProgressbar = 0;
      }
    }
    else if (counter1 == 40) {
      if (counter2 >= 100) {
        
      //gui.UG_ButtonDelete(gui.window[WND_MAIN], BTN_ID_CONTROL);
      }
#ifndef USE_STATIC_MEMORY
      gui.UG_ButtonSetText(gui.window[WND_MAIN], BTN_ID_CONTROL, (char*)TitleText);
      gui.UG_ButtonSetHSpace(gui.window[WND_MAIN], BTN_ID_CONTROL,3);
      //gui.UG_ButtonShow(gui.window[WND_MAIN], BTN_ID_CONTROL);
#else
      gui.UG_ButtonSetText(&mainWindow, BTN_ID_CONTROL, (char*)TitleText);
      gui.UG_ButtonSetHSpace(&mainWindow, BTN_ID_CONTROL,3);
#endif
      counter1 = 0;
    }
    gui.UG_Update();
  }
}

//*****************************************************************************************//
void setPixel(UG_S16 x,UG_S16 y,UG_COLOR c) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  tft.drawPixel(x, y, c);
}

/*****************************************************************************************
//
//  Callback function will be called if there is a touch event
//
******************************************************************************************/
void* mainWindowCallback (UG_MESSAGE* msg) {

UG_OBJECT* obj;

obj =  (UG_OBJECT*) *(int*)msg->src;
  if (msg->type == MSG_TYPE_OBJECT) {
      if (msg->id == OBJ_TYPE_BUTTON) {

        if (obj->id == BTN_ID_PRINT) {
          if (msg->event == OBJ_EVENT_CLICKED) {
#ifndef USE_STATIC_MEMORY
            if (gui.UG_ButtonGetHSpace(gui.window[WND_MAIN],BTN_ID_PRINT) ==1) {
            gui.UG_ButtonSetHSpace(gui.window[WND_MAIN], BTN_ID_PRINT,3);
            }else{
              gui.UG_ButtonSetHSpace(gui.window[WND_MAIN], BTN_ID_PRINT,1);
            }
#else
            if (gui.UG_ButtonGetHSpace(&mainWindow,BTN_ID_PRINT) ==1) {
            gui.UG_ButtonSetHSpace(&mainWindow, BTN_ID_PRINT,3);
            }else{
              gui.UG_ButtonSetHSpace(&mainWindow, BTN_ID_PRINT,1);
            }
#endif
          }
        }
      }
  }
  return NULL;
}

//*****************************************************************************************//
void setupWindows (void) {
#ifndef USE_STATIC_MEMORY
  gui.window[WND_MAIN] = gui.UG_WindowCreate(OBJ_BUF_WND_MAIN, mainWindowCallback);    // create windows with n objects
  gui.UG_WindowSetTitleText(gui.window[WND_MAIN], (char*)TitleText);
  gui.UG_WindowSetStyle(gui.window[WND_MAIN], WND_STYLE_2D | WND_STYLE_SHOW_TITLE);
//  gui.UG_WindowResize(gui.window[WND_MAIN], 10,20,300,200);
  
  gui.UG_ButtonCreate(gui.window[WND_MAIN], BTN_ID_CONTROL, 30,30,120,50);				// x/y coords of start and endpoint
  gui.UG_ButtonSetText(gui.window[WND_MAIN], BTN_ID_CONTROL, (char*)TitleText);
  gui.UG_ButtonSetStyle(gui.window[WND_MAIN], BTN_ID_CONTROL, BTN_STYLE_2D | BTN_STYLE_USE_ALTERNATE_COLORS);
  gui.UG_ButtonSetAlternateBackColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_GREEN);
  gui.UG_ButtonSetBackColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_PALE_GREEN);
  
  gui.UG_ButtonCreate(gui.window[WND_MAIN], BTN_ID_PRINT, 30,60,20,20, UG_REL);			// x/y coords of startpoint and width/height of object
  gui.UG_ButtonSetText(gui.window[WND_MAIN], BTN_ID_PRINT, (char*)Button2Txt);
  gui.UG_ButtonSetBackColor(gui.window[WND_MAIN], BTN_ID_PRINT, C_PALE_GREEN);
  
  gui.UG_CheckboxCreate(gui.window[WND_MAIN], 1, 30,90,49,109);
  gui.UG_CheckboxSetBackColor(gui.window[WND_MAIN], 1, C_GREEN);
  gui.UG_CheckboxSetAlternateBackColor(gui.window[WND_MAIN], 1, C_PALE_GREEN);
  gui.UG_CheckboxSetStyle(gui.window[WND_MAIN], 1, CHB_STYLE_2D | CHB_STYLE_USE_ALTERNATE_COLORS);

  gui.UG_TextboxCreate(gui.window[WND_MAIN], 1, 30, 115, 100,20, UG_REL);
  gui.UG_TextboxSetText(gui.window[WND_MAIN], 1, (char*)Str1);

  gui.UG_ProgressbarCreate(gui.window[WND_MAIN], 1, 30, 200, 200, 15, UG_REL);
  gui.UG_ProgressbarSetForeColor(gui.window[WND_MAIN], 1, C_GREEN);
  gui.UG_ProgressbarSet2dBorderColor(gui.window[WND_MAIN], 1, C_RED);
  gui.UG_ProgressbarSetBackColor(gui.window[WND_MAIN], 1, C_GRAY);
//  gui.UG_ProgressbarSetStyle(gui.window[WND_MAIN], 1, PGR_STYLE_2D);

  gui.UG_RadiobuttonCreate(gui.window[WND_MAIN], 1, 100, 90, 15);
//gui.UG_Update();
#else

  gui.UG_WindowCreate(&mainWindow, obj_buf_wnd_main, OBJ_BUF_WND_MAIN, mainWindowCallback);    // create window
  gui.UG_WindowSetTitleText(&mainWindow, (char*)TitleText);
  gui.UG_WindowSetStyle(&mainWindow, WND_STYLE_2D | WND_STYLE_SHOW_TITLE);
//  gui.UG_WindowResize(mainWindow, 10,20,300,200);

  gui.UG_ButtonCreate(&mainWindow, &btnControl, BTN_ID_CONTROL, 30,30,120,50);
  gui.UG_ButtonSetText(&mainWindow, BTN_ID_CONTROL, (char*)TitleText);
  
  gui.UG_ButtonCreate(&mainWindow, &btnPrint, BTN_ID_PRINT, 30,60,120,20, UG_REL);
  gui.UG_ButtonSetText(&mainWindow, BTN_ID_PRINT, (char*)Button2Txt);
  
  gui.UG_CheckboxCreate(&mainWindow, &chbControl, CHB_ID_CONTROL, 30,90,100,20,UG_REL);
  
  gui.UG_ProgressbarCreate(&mainWindow, &pgrControl, PRB_ID_COUNT, 30, 200, 200, 15, UG_REL);
  
  gui.UG_RadiobuttonCreate(&mainWindow, &rabControl, RAB_ID_1, 100, 90, 15);

#endif
}

/***********************************************************************/
void getTouch (void) {

  if (ts.touched() ) {
    p = ts.getPoint();
    touch = 1;
    p.x = map(p.x,point_x_min,point_x_max,0,TOUCH_X_DIM);
    p.y = map(p.y,point_y_min,point_y_max,0,TOUCH_Y_DIM);
    gui.UG_TouchUpdate (p.x, p.y, TOUCH_STATE_PRESSED);
  }else{
    touch = 0;
    gui.UG_TouchUpdate( -1, -1, TOUCH_STATE_RELEASED);
  }

}
/***********************************************************************/
UG_RESULT HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c ) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  tft.drawLine( x1, y1, x2, y2, c);
  return UG_RESULT_OK;
}

/***********************************************************************/
UG_RESULT HW_FillScreen ( UG_COLOR c) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
#ifdef RA8875_TFT
  tft.clearScreen(c);
#else
  tft.fillScreen(c);
#endif
  return UG_RESULT_OK;
}

/***********************************************************************/
UG_RESULT HW_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c ) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  if ( (x2 == x1) || (y2 == y1) ) return UG_RESULT_OK;
  x2 = x2 - x1;
  if (x2 < 0) x2 = -x2;
  x2++;
  y2 = y2 - y1;
  if (y2 < 0) y2 = -y2;
  y2++;
  tft.fillRect(x1, y1, x2, y2, c);
  return UG_RESULT_OK;
}

/***********************************************************************/
UG_RESULT HW_FillRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c )  {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  if ( (x2 == x1) || (y2 == y1) ) return UG_RESULT_OK;
  x2 = x2 - x1;
  if (x2 < 0) x2 = -x2;
  x2++;
  y2 = y2 - y1;
  if (y2 < 0) y2 = -y2;
  y2++;
  tft.fillRoundRect(x1, y1, x2, y2, r, c);  
  return UG_RESULT_OK;
}

/***********************************************************************/
UG_RESULT HW_DrawFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c ) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  if ( (x2 == x1) || (y2 == y1) ) return UG_RESULT_OK;
  x2 = x2 - x1;
  if (x2 < 0) x2 = -x2;
  x2++;
  y2 = y2 - y1;
  if (y2 < 0) y2 = -y2;
  y2++;
  tft.drawRect( x1, y1, x2, y2, c);
  return UG_RESULT_OK;
}

/***********************************************************************/
UG_RESULT HW_DrawRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c ) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  if ( (x2 == x1) || (y2 == y1) ) return UG_RESULT_OK;
  x2 = x2 - x1;
  if (x2 < 0) x2 = -x2;
  x2++;
  y2 = y2 - y1;
  if (y2 < 0) y2 = -y2;
  y2++;
  tft.drawRoundRect( x1, y1, x2, y2, r, c);
  return UG_RESULT_OK;
}

/***********************************************************************/
UG_RESULT HW_DrawCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c ) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  tft.drawCircle( x0, y0, r, c);
  return UG_RESULT_OK;
}

/***********************************************************************/
UG_RESULT HW_FillCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c ) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  tft.fillCircle( x0, y0, r, c);
  return UG_RESULT_OK;
}
