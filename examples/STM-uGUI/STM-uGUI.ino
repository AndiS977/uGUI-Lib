#include "SPI.h"
#include <UGUI.h>
//#include "fonts.h"
#include "STMuGUI.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS PB7

// for freeMemory()
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

//#include <Adafruit_GFX.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#define LED   PC13

#define SPI_CLOCK_DIVIDER SPI_CLOCK_DIV4
//#define SPI_CLOCK_TOUCH   SPI_CLOCK_DIV64

//Falls die Anzeige gedreht wird
#define ROTATION_NO 0
#define ROTATION_90 1
#define ROTATION_180 2
#define ROTATION_270 3

SPIClass TFTSPI();
//SPIClass TFTSPI(PA7, PA6, PA5);     //SPIClass TFTSPI(1);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

  int touch;
  CONFIG_t ee_config;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS_PIN);
  TS_Point p;
volatile size_t pointValid;

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
UG_RADIOBUTTON rabControl;
UG_TEXTBOX txbControl;
UG_PROGRESSBAR prbControl;
UG_TEXTBOX tempTextbox1;
UG_TEXTBOX tempTextbox2;
#endif

const char TitleText[] = "Hallo";
const char Str1[] = "Hello World";
const char Button1Txt[] = "Btn 1";
const char Button2Txt[] = "Btn 2";
const char Chb1Txt[] = "Chb 1";
const char fmStr[] = "Free Memory: ";

char temperaturStr[6] = "Wait";
char temperaturStr1[6] = "Wait";
float merketemperatur=0.0, merketemperatur1=0.0;
DeviceAddress insideThermometer, outsideThermometer;

/***********************************************************************/
void setup() {
  // put your setup code here, to run once:

  pinMode (LED, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  pinMode(TOUCH_IRQ,INPUT_PULLUP);
  tft.begin(18000000);
  ts.begin();
  p = ts.getPoint();
  DS18B20.begin();
  oneWire.reset_search();
  oneWire.search(insideThermometer);
  oneWire.search(outsideThermometer);
//  DS18B20.setCheckForConversion(false);
  DS18B20.setWaitForConversion(false);
  
  SPI.setClockDivider(SPI_CLOCK_DIVIDER);
  tft.setRotation( ROTATION_270);
  tft.fillScreen(ILI9341_BLACK);

  Serial.println("start setup");
  Serial.print(fmStr);Serial.println( freeMemory() );
  ee_config.point_x_min = 420;
  ee_config.point_x_max = 3600;
  ee_config.point_y_min = 300;
  ee_config.point_y_max = 3730;

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
//  gui.UG_FontSelect (&FONT_6X8);
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
  //gui.UG_WindowShow(mainWindow);
  Serial.println("setup done");
  Serial.print(fmStr);Serial.println( freeMemory() );

}

/***********************************************************************/
void loop() {
  // put your main code here, to run repeatedly:
  static uint32_t timeOld;
  static uint32_t counter1 = 0;
  static uint32_t counter500 = 0;
  static size_t counter2 = 0;
  static UG_U8 counterProgressbar = 90;
  static size_t getTempStatus;
  
  uint32_t timeNow = millis();
  if ((timeNow - timeOld) >= 50) {
    timeOld = timeNow;
    counter1++;
    counter2++;
    
    getTouch();
    
    if (++counter500 > 100) {
      if (!getTempStatus) {
        digitalWrite(LED, HIGH);
        Serial.print(F("Start Request Temp: "));Serial.println(millis());
        DS18B20.requestTemperaturesByAddress(insideThermometer);
        DS18B20.requestTemperaturesByAddress(outsideThermometer);
//        DS18B20.requestTemperatures();
        Serial.print(F("Stop Request Temp: "));Serial.println(millis());
        getTempStatus++;
      }else{
        if(counter500 > 110) {
          counter500 = 0;
          getTempStatus = 0;
          float temperatur = DS18B20.getTempC(insideThermometer);
          float temperatur1 = DS18B20.getTempC(outsideThermometer);
//          float temperatur = DS18B20.getTempCByIndex(0);
          Serial.println(F("Got Temp"));
          Serial.print(temperatur);Serial.print(F("  "));Serial.println(temperatur1);
//          if (temperatur <= -100.0) temperatur = -99.9;
//          if (temperatur1 <= -100.0) temperatur = -99.9;
//          if (temperatur >= -99.9) {
            if ( (merketemperatur != temperatur) || (merketemperatur1 != temperatur1) ) {
              //zeigeTemperatur(temperatur, temperatur1);
  Serial.print(F("Temperatur: ")); Serial.println(temperaturStr);
              dtostrf(temperatur, 2, 1, temperaturStr);
#ifndef USE_STATIC_MEMORY
              gui.UG_TextboxSetText(gui.window[WND_MAIN], 10, temperaturStr);
#else
              gui.UG_TextboxSetText(&mainWindow, 10, temperaturStr);
#endif
              dtostrf(temperatur1, 2, 1, temperaturStr1);
  Serial.print(F("Temperatur1: ")); Serial.println(temperaturStr1);
#ifndef USE_STATIC_MEMORY
              gui.UG_TextboxSetText(gui.window[WND_MAIN], 11, temperaturStr1);
#else
              gui.UG_TextboxSetText(&mainWindow, 11, temperaturStr1);
#endif
              merketemperatur=temperatur;
              merketemperatur1 = temperatur1;
            }
//          }
          digitalWrite(LED, LOW);
        }
      }
    }
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
      gui.UG_ProgressbarSetPercent(gui.window[WND_MAIN], 1, counterProgressbar);
#else
      gui.UG_ProgressbarSetPercent(&mainWindow, 1, counterProgressbar);
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
      Serial.print(fmStr);Serial.println( freeMemory() );
    }
    gui.UG_Update();
  }
}

/***********************************************************************/
void setPixel(UG_S16 x,UG_S16 y,UG_COLOR c) {

#ifdef USE_COLOR_RGB888
  c = tft.color565( (uint8_t)(c>>16), (uint8_t)(c>>8), (uint8_t)c );
#endif
  tft.drawPixel(x, y, c);
}

/***********************************************************************/
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

/***********************************************************************/
void setupWindows (void) {
#ifndef USE_STATIC_MEMORY
  Serial.print(fmStr);Serial.println( freeMemory() );
  gui.window[WND_MAIN] = gui.UG_WindowCreate(OBJ_BUF_WND_MAIN, mainWindowCallback);    // create windows with n objects
  gui.UG_WindowSetTitleText(gui.window[WND_MAIN], (char*)TitleText);
  gui.UG_WindowSetStyle(gui.window[WND_MAIN], WND_STYLE_2D | WND_STYLE_SHOW_TITLE);
  
//  gui.UG_WindowResize(gui.window[WND_MAIN], 10,20,300,200);
  
  gui.UG_ButtonCreate(gui.window[WND_MAIN], BTN_ID_CONTROL, 30,30,120,50);
  gui.UG_ButtonSetText(gui.window[WND_MAIN], BTN_ID_CONTROL, (char*)TitleText);
  gui.UG_ButtonSetStyle(gui.window[WND_MAIN], BTN_ID_CONTROL, BTN_STYLE_2D | BTN_STYLE_USE_ALTERNATE_COLORS);
  gui.UG_ButtonSetAlternateBackColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_BUTTON_PRESSED);
  gui.UG_ButtonSetBackColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_PALE_GREEN);
  
  gui.UG_ButtonCreate(gui.window[WND_MAIN], BTN_ID_PRINT, 30,60,20,20, UG_REL);
  gui.UG_ButtonSetText(gui.window[WND_MAIN], BTN_ID_PRINT, (char*)Button2Txt);
  gui.UG_ButtonSetBackColor(gui.window[WND_MAIN], BTN_ID_PRINT, C_PALE_GREEN);
  
  gui.UG_CheckboxCreate(gui.window[WND_MAIN], 1, 30,90,49,109);
  gui.UG_CheckboxSetBackColor(gui.window[WND_MAIN], 1, C_GREEN);
  gui.UG_CheckboxSetAlternateBackColor(gui.window[WND_MAIN], 1, C_PALE_GREEN);
  gui.UG_CheckboxSetStyle(gui.window[WND_MAIN], 1, CHB_STYLE_2D | CHB_STYLE_USE_ALTERNATE_COLORS);
//  gui.UG_CheckboxSetText(gui.window[WND_MAIN], 1, NULL);

  gui.UG_TextboxCreate(gui.window[WND_MAIN], 1, 30, 115, 100,20, UG_REL);
  gui.UG_TextboxSetText(gui.window[WND_MAIN], 1, (char*)Str1);

  gui.UG_ProgressbarCreate(gui.window[WND_MAIN], 1, 30, 200, 200, 15, UG_REL);
  gui.UG_ProgressbarSetForeColor(gui.window[WND_MAIN], 1, C_GREEN);
  gui.UG_ProgressbarSet2dBorderColor(gui.window[WND_MAIN], 1, C_RED);
  gui.UG_ProgressbarSetBackColor(gui.window[WND_MAIN], 1, C_GRAY);
//  gui.UG_ProgressbarSetStyle(gui.window[WND_MAIN], 1, BGR_STYLE_2D);

  gui.UG_RadiobuttonCreate(gui.window[WND_MAIN], 1, 100, 90, 15);
//gui.UG_Update();
#else
  Serial.print(fmStr);Serial.println( freeMemory() );
  gui.UG_WindowCreate(&mainWindow, obj_buf_wnd_main, OBJ_BUF_WND_MAIN, mainWindowCallback);    // create window
  gui.UG_WindowSetTitleText(&mainWindow, (char*)TitleText);
  gui.UG_WindowSetStyle(&mainWindow, WND_STYLE_2D | WND_STYLE_SHOW_TITLE);
  gui.UG_WindowSetTitleTextFont(&mainWindow, &FONT_12X20);
//  gui.UG_WindowResize(&mainWindow, 10,20,300,200);

  gui.UG_ButtonCreate(&mainWindow, &btnControl, BTN_ID_CONTROL, 30,30,120,50);
  gui.UG_ButtonSetText(&mainWindow, BTN_ID_CONTROL, (char*)TitleText);
  gui.UG_ButtonSetStyle(&mainWindow, BTN_ID_CONTROL, BTN_STYLE_2D | BTN_STYLE_USE_ALTERNATE_COLORS);
  gui.UG_ButtonSetAlternateBackColor(&mainWindow, BTN_ID_CONTROL, C_BUTTON_PRESSED);
  gui.UG_ButtonSetBackColor(&mainWindow, BTN_ID_CONTROL, C_PALE_GREEN);
  
  gui.UG_ButtonCreate(&mainWindow, &btnPrint, BTN_ID_PRINT, 30,60,20,20, UG_REL);
  gui.UG_ButtonSetText(&mainWindow, BTN_ID_PRINT, (char*)Button2Txt);
  gui.UG_ButtonSetBackColor(&mainWindow, BTN_ID_PRINT, C_PALE_GREEN);
  
  gui.UG_CheckboxCreate(&mainWindow, &chbControl, 1, 30,90,20,20,UG_REL);
  gui.UG_CheckboxSetBackColor(&mainWindow, 1, C_GREEN);
  gui.UG_CheckboxSetAlternateBackColor(&mainWindow, 1, C_PALE_GREEN);
  gui.UG_CheckboxSetStyle(&mainWindow, 1, CHB_STYLE_2D | CHB_STYLE_USE_ALTERNATE_COLORS);

  gui.UG_TextboxCreate(&mainWindow, &txbControl, 1, 30, 115, 100,20, UG_REL);
  gui.UG_TextboxSetText(&mainWindow, 1, (char*)Str1);

  gui.UG_ProgressbarCreate(&mainWindow, &prbControl, 1, 30, 160, 200, 15, UG_REL);
  gui.UG_ProgressbarSetForeColor(&mainWindow, 1, C_GREEN);
  gui.UG_ProgressbarSet2dBorderColor(&mainWindow, 1, C_RED);
  gui.UG_ProgressbarSetBackColor(&mainWindow, 1, C_GRAY);
//  gui.UG_ProgressbarSetStyle(&mainWindow, 1, BGR_STYLE_2D);

  gui.UG_RadiobuttonCreate(&mainWindow, &rabControl, 1, 100, 90, 15);

  gui.UG_TextboxCreate(&mainWindow, &tempTextbox1, 10, 160, 30, 100, 30, UG_REL);
  gui.UG_TextboxSetBackColor(&mainWindow, 10, C_LIGHT_GREEN);
  gui.UG_TextboxSetFont(&mainWindow, 10, &FONT_12X20);
  gui.UG_TextboxSetAlignment(&mainWindow, 10, ALIGN_CENTER);
  gui.UG_TextboxSetText(&mainWindow, 10, temperaturStr);

  gui.UG_TextboxCreate(&mainWindow, &tempTextbox2, 11, 160, 90, 100, 30, UG_REL);
  gui.UG_TextboxSetBackColor(&mainWindow, 11, C_LIGHT_GREEN);
  gui.UG_TextboxSetFont(&mainWindow, 11, &FONT_12X20);
  gui.UG_TextboxSetAlignment(&mainWindow, 11, ALIGN_CENTER);
  gui.UG_TextboxSetText(&mainWindow, 11, temperaturStr1);
  
#endif
}

/***********************************************************************/
void getTouch (void) {

  if (ts.touched() ) {
    p = ts.getPoint();
    touch = 1;
    p.x = map(p.x,ee_config.point_x_min,ee_config.point_x_max,0,TOUCH_X_DIM);
    p.y = map(p.y,ee_config.point_y_min,ee_config.point_y_max,0,TOUCH_Y_DIM);
    Serial.print(p.x,DEC); Serial.print(' ');Serial.println(p.y,DEC);
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
/*
  tft.drawFastVLine(x1, y1, y2, c);
  tft.drawFastVLine(x1+x2-1, y1, y2, c);
  tft.drawFastHLine(x1, y1, x2, c);
  tft.drawFastHLine(x1+y2-1, y1, x2, c);
*/
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

/***********************************************************************/
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
