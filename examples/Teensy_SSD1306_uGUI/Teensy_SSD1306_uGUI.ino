/*
 Name:		Teensy_SSD1306_uGUI.ino
 Created:	22.04.2021 10:01:56
 Author:	Andi2
*/

// Example for Teensy and SSD1306 Monocrome-OLED

// uGUI setup for dynamic memory allocation

#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Teensy_SSD1306_uGUI.h"
#include <UGUI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_DC     9
#define OLED_CS     10
#define OLED_RESET  8

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
	&SPI, OLED_DC, OLED_RESET, OLED_CS);

UGUI  gui = UGUI(4, 128, 64);		// max 4 windows, screen size 128x64

const char TitleText[] = "Hallo";
const char Str1[] = "Hello World";
const char Button1Txt[] = "Btn 1";
const char Button2Txt[] = "Btn 2";
const char Chb1Txt[] = "Chb 1";


/***********************************************************************/
void setup() {
	Serial.begin(115200);
	display.begin(SSD1306_SWITCHCAPVCC);
	display.clearDisplay(); // Clear display buffer
	display.display();

	gui.UG_Init(setPixel);
	gui.UG_FontSelect(&FONT_6X8);
	gui.UG_PutString(10, 10, (char*)Str1);
	display.display();
	delay(2000);
	setupWindows();
	gui.UG_WindowShow(gui.window[WND_MAIN]);
	gui.UG_TouchUpdate(-1, -1, TOUCH_STATE_RELEASED);
	gui.UG_Update();
	display.display();
}

/***********************************************************************/
void loop() {
	static elapsedMillis updateGUI;
	static elapsedMillis increasePRB;
	static uint8_t counterProgressbar = 0;

	if (increasePRB >= 700) {
		increasePRB -= 700;

		counterProgressbar++;
		if (counterProgressbar <= 100) {
			gui.UG_ProgressbarSetPercent(gui.window[WND_MAIN], PRB_ID_1, counterProgressbar);
		}
		if (counterProgressbar > 100) {
			counterProgressbar = 0;
			gui.UG_ProgressbarSetPercent(gui.window[WND_MAIN], PRB_ID_1, counterProgressbar);
			if (gui.UG_ProgressbarGetForeColor(gui.window[WND_MAIN], PRB_ID_1) == C_WHITE) {
				gui.UG_ProgressbarSetForeColor(gui.window[WND_MAIN], PRB_ID_1, C_BLACK);
				gui.UG_ProgressbarSetBackColor(gui.window[WND_MAIN], PRB_ID_1, C_WHITE);
			}
			else {
				gui.UG_ProgressbarSetForeColor(gui.window[WND_MAIN], PRB_ID_1, C_WHITE);
				gui.UG_ProgressbarSetBackColor(gui.window[WND_MAIN], PRB_ID_1, C_BLACK);
			}
		}
	}
	
	if (updateGUI >= 50) {
		updateGUI -= 50;
		gui.UG_Update();
		display.display();
	}
}

/***********************************************************************/
void setPixel(UG_S16 x, UG_S16 y, UG_COLOR c) {

	if (c) display.drawPixel(x, y, 1);
	else display.drawPixel(x, y, 0);
}

/***********************************************************************/
void* mainWindowCallback(UG_MESSAGE* msg) {

	return NULL;
}

/***********************************************************************/
void setupWindows(void) {

	gui.window[WND_MAIN] = gui.UG_WindowCreate(OBJ_BUF_WND_MAIN, mainWindowCallback);    // create windows with n objects
	gui.UG_WindowSetTitleTextFont(gui.window[WND_MAIN], &FONT_6X8);
	gui.UG_WindowSetTitleText(gui.window[WND_MAIN], (char*)Str1);
	gui.UG_WindowSetTitleTextColor(gui.window[WND_MAIN], C_BLACK);
	gui.UG_WindowSetTitleTextAlignment(gui.window[WND_MAIN], ALIGN_CENTER);
	gui.UG_WindowSetStyle(gui.window[WND_MAIN], WND_STYLE_2D | WND_STYLE_SHOW_TITLE);
	gui.UG_WindowSetBackColor(gui.window[WND_MAIN], C_BLACK);
	
	int windowWidth = gui.UG_WindowGetInnerWidth(gui.window[WND_MAIN]);
	int windowHeight = gui.UG_WindowGetInnerHeight(gui.window[WND_MAIN]);
	gui.UG_ProgressbarCreate(gui.window[WND_MAIN], PRB_ID_1, 5, windowHeight - 10, windowWidth - 5, windowHeight - 2);
	gui.UG_ProgressbarSetForeColor(gui.window[WND_MAIN], PRB_ID_1, C_WHITE);
	gui.UG_ProgressbarSet2dBorderColor(gui.window[WND_MAIN], PRB_ID_1, C_WHITE);
	gui.UG_ProgressbarSetBackColor(gui.window[WND_MAIN], PRB_ID_1, C_BLACK);
//	gui.UG_ProgressbarSetStyle(gui.window[WND_MAIN], PRB_ID_1, PRB_STYLE_3D);

	gui.UG_ButtonCreate(gui.window[WND_MAIN], BTN_ID_CONTROL, 5, 5, 45, 15, UG_REL);
	gui.UG_ButtonSetText(gui.window[WND_MAIN], BTN_ID_CONTROL, (char*)Button1Txt);
	gui.UG_ButtonSetFont(gui.window[WND_MAIN], BTN_ID_CONTROL, &FONT_6X8);
	gui.UG_ButtonSetForeColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_WHITE);
	gui.UG_ButtonSetStyle(gui.window[WND_MAIN], BTN_ID_CONTROL, BTN_STYLE_2D | BTN_STYLE_USE_ALTERNATE_COLORS);
	gui.UG_ButtonSetAlternateBackColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_WHITE);
	gui.UG_ButtonSetBackColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_BLACK);
	gui.UG_ButtonSetHSpace(gui.window[WND_MAIN], BTN_ID_PRINT, 0);
	gui.UG_ButtonSetAlternateForeColor(gui.window[WND_MAIN], BTN_ID_CONTROL, C_WHITE);

}

