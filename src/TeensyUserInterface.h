//      ******************************************************************
//      *                                                                *
//      *            Header file for TeensyUserInterface.cpp             *
//      *                                                                *
//      *              Copyright (c) S. Reifel & Co,  2014               *
//      *                                                                *
//      ******************************************************************


// MIT License
// 
// Copyright (c) 2014 Stanley Reifel & Co.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef TeensyUserInterface_h
#define TeensyUserInterface_h

#include <ILI9341_t3.h>


//
// lcd display screen orientations
//
const int LCD_ORIENTATION_PORTRAIT_4PIN_TOP = 0;
const int LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT = 1;
const int LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM = 2;
const int LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT = 3;


// 
// 16 bit colors in rgb 565 format
//
const uint16_t LCD_BLACK =       0x0000; 
const uint16_t LCD_NAVY =        0x000F;
const uint16_t LCD_DARKGREEN =   0x03E0;
const uint16_t LCD_DARKCYAN =    0x03EF;
const uint16_t LCD_MAROON =      0x7800;
const uint16_t LCD_PURPLE =      0x780F;
const uint16_t LCD_OLIVE =       0x7BE0;
const uint16_t LCD_LIGHTGREY =   0xC618;
const uint16_t LCD_DARKGREY =    0x7BEF;
const uint16_t LCD_BLUE =        0x001F;
const uint16_t LCD_LIGHTBLUE =   0x5ADF;
const uint16_t LCD_DARKBLUE =    0x0016;
const uint16_t LCD_GREEN =       0x07E0; 
const uint16_t LCD_CYAN =        0x07FF;
const uint16_t LCD_RED =         0xF800;
const uint16_t LCD_MAGENTA =     0xF81F;
const uint16_t LCD_YELLOW =      0xFFE0;
const uint16_t LCD_WHITE =       0xFFFF;
const uint16_t LCD_ORANGE =      0xFD20;
const uint16_t LCD_GREENYELLOW = 0xAFE5;


//
// datatype for fonts
//
typedef ILI9341_t3_font_t ui_font;


//
// definition of a Button, the menu's colors and font are used 
//
typedef struct 
{
  const char *labelText;
  int centerX;
  int centerY;
  int width;
  int height;
} BUTTON;


//
// definition of a Button with extended options
//
typedef struct 
{
  const char *labelText;
  int centerX;
  int centerY;
  int width;
  int height;
  uint16_t buttonColor;
  uint16_t buttonSelectedColor;
  uint16_t buttonFrameColor;
  uint16_t buttonTextColor;
  const ui_font &buttonFont;
} BUTTON_EXTENDED;


//
// definition of a integer Number Box 
//
typedef struct 
{
  const char *labelText;
  int value;
  int minimumValue;
  int maximumValue;
  int stepAmount;
  int centerX;
  int centerY;
  int width;
  int height;
} NUMBER_BOX;


//
// definition of a float Number Box 
//
typedef struct 
{
  const char *labelText;
  float value;
  float minimumValue;
  float maximumValue;
  float stepAmount;
  int digitsRightOfDecimal;
  int centerX;
  int centerY;
  int width;
  int height;
} NUMBER_BOX_FLOAT;


//
// definition of a Selection Box 
//
typedef struct 
{
  const char *labelText;
  int value;
  const char *choice0Text;
  const char *choice1Text;
  const char *choice2Text;
  const char *choice3Text;
  int centerX;
  int centerY;
  int width;
  int height;
} SELECTION_BOX;


//
// definition of an entry in menu's table
//
typedef struct _MENU_ITEM
{
  byte MenuItemType;
  const char *MenuItemText;
  void (*MenuItemFunction)();
  _MENU_ITEM *MenuItemSubMenu;
} MENU_ITEM;


//
// types of entries in a menu table
//
const byte MENU_ITEM_TYPE_MAIN_MENU_HEADER = 0;
const byte MENU_ITEM_TYPE_SUB_MENU_HEADER  = 1;
const byte MENU_ITEM_TYPE_SUB_MENU         = 2;
const byte MENU_ITEM_TYPE_COMMAND          = 3;
const byte MENU_ITEM_TYPE_TOGGLE           = 4;
const byte MENU_ITEM_TYPE_END_OF_MENU      = 5;


//
// number of columns of buttons in a menu
//
#define MENU_COLUMNS_1  ((void (*)()) 1)
#define MENU_COLUMNS_2  ((void (*)()) 2)
#define MENU_COLUMNS_3  ((void (*)()) 3)
#define MENU_COLUMNS_4  ((void (*)()) 4)


//
// types of touch events
//
const int TOUCH_NO_EVENT         = 0;       // no event from the touch screen (nothing being touched)
const int TOUCH_PUSHED_EVENT     = 1;       // touch screen just touched
const int TOUCH_RELEASED_EVENT   = 2;       // touch screen just released
const int TOUCH_REPEAT_EVENT     = 3;       // touch screen touched and event repeating


//
// the TeensyUserInterface class
//
class TeensyUserInterface
{
  public:
    //
    // public member variables
    //
    int lcdWidth;
    int lcdHeight;
    int displaySpaceWidth;
    int displaySpaceHeight;
    int displaySpaceLeftX;
    int displaySpaceRightX;
    int displaySpaceTopY;
    int displaySpaceBottomY;
    int displaySpaceCenterX;
    int displaySpaceCenterY;
    int touchEventType;
    int touchEventX;
    int touchEventY;
    byte toggleSelectNextStateFlg;
    const char *toggleText;


    //
    // public functions
    //
    TeensyUserInterface(void);
    void begin(int lcdOrientation, const ui_font &font);
    void setColorPaletteBlue(void);
    void setColorPaletteGray(void);

    void setMenuColors(uint16_t _menuBackgroundColor, uint16_t _menuButtonColor, uint16_t _menuButtonSelectedColor, uint16_t _menuButtonFrameColor, uint16_t _menuButtonTextColor);
    void setMenuFont(const ui_font &font);
    void displayAndExecuteMenu(MENU_ITEM *menu);
 
    void setTitleBarColors(uint16_t _titleBarColor, uint16_t _titleBarTextColor, uint16_t _titleBarBackButtonColor, uint16_t _titleBarBackButtonSelectedColor);
    void setTitleBarFont(const ui_font &font);
    void drawTitleBar(const char *titleBarText);
    void drawTitleBarWithBackButton(const char *titleBarText);
    void drawTitleBarWithMenuButton(const char *titleBarText);
    boolean checkForBackButtonClicked(void);
    boolean checkForMenuButtonClicked(void);
    void clearDisplaySpace(void);
    void clearDisplaySpace(uint16_t backgroundColor);

    void drawButton(BUTTON &uiButton);
    void drawButton(BUTTON &uiButton, boolean buttonSelectedFlg);
    void drawButton(BUTTON_EXTENDED &uiButtonExt);
    void drawButton(BUTTON_EXTENDED &uiButtonExt, boolean buttonSelectedFlg);
    void drawButton(const char *buttonText, boolean buttonSelectedFlg, int buttonX, int buttonY, int buttonWidth, int buttonHeight);
    void drawButton(const char *buttonText, int buttonX, int buttonY, int buttonWidth, int buttonHeight, uint16_t buttonColor, uint16_t buttonFrameColor, uint16_t buttonTextColor, const ui_font &buttonFont);
    boolean checkForButtonClicked(BUTTON &uiButton);
    boolean checkForButtonClicked(BUTTON_EXTENDED &uiButton);
    boolean checkForButtonAutoRepeat(BUTTON &uiButton);
    boolean checkForButtonAutoRepeat(BUTTON_EXTENDED &uiButton);
    boolean checkForButtonFirstTouched(BUTTON &uiButton);
    boolean checkForButtonFirstTouched(BUTTON_EXTENDED &uiButton);

    void drawNumberBox(NUMBER_BOX &numberBox);
    void drawNumberBox(NUMBER_BOX_FLOAT &numberBox);
    boolean checkForNumberBoxTouched(NUMBER_BOX &numberBox);
    boolean checkForNumberBoxTouched(NUMBER_BOX_FLOAT &numberBox);

    void drawSelectionBox(SELECTION_BOX &selectionBox);
    boolean checkForSelectionBoxTouched(SELECTION_BOX &selectionBox);

    boolean checkForTouchEventInRect(int eventType, int rectX1, int rectY1, int rectX2, int rectY2);
    void getTouchEvents(void);
    void setDefaultTouchScreenCalibrationConstants(int lcdOrientation);
    void setTouchScreenCalibrationConstants(int tsToLCDOffsetX, float tsToLCDScalerX, int tsToLCDOffsetY, float tsToLCDScalerY);
    boolean getTouchScreenCoords(int *xLCD, int *yLCD);

    void lcdClearScreen(uint16_t color);
    void lcdDrawPixel(int x, int y, uint16_t color);
    void lcdDrawLine(int x1, int y1, int x2, int y2, uint16_t color);
    void lcdDrawHorizontalLine(int x, int y, int length, uint16_t color);
    void lcdDrawVerticalLine(int x, int y, int length, uint16_t color);
    void lcdDrawRectangle(int x, int y, int width, int height, uint16_t color);
    void lcdDrawRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color);
    void lcdDrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void lcdDrawCircle(int x, int y, int radius, uint16_t color);
    void lcdDrawFilledRectangle(int x, int y, int width, int height, uint16_t color);
    void lcdDrawFilledRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color);
    void lcdDrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void lcdDrawFilledCircle(int x, int y, int radius, uint16_t color);
    void lcdSetFont(const ui_font &font);
    void lcdSetFontColor(uint16_t color);
    void lcdPrint(char *s);
    void lcdPrint(const char *s);
    void lcdPrint(int n);
    void lcdPrint(double n, int digitsRightOfDecimal = 5);
    void lcdPrintRightJustified(char *s);
    void lcdPrintRightJustified(const char *s);
    void lcdPrintRightJustified(int n);
    void lcdPrintRightJustified(double n, int digitsRightOfDecimal = 5);
    void lcdPrintCentered(char *s);
    void lcdPrintCentered(const char *s);
    void lcdPrintCentered(int n);
    void lcdPrintCentered(double n, int digitsRightOfDecimal = 5);
    void lcdPrintCharacter(byte character);
    int lcdStringWidthInPixels(char *s);
    int lcdStringWidthInPixels(const char *s);
    int lcdGetFontHeightWithoutDecenders(void);
    int lcdGetFontHeightWithDecentersAndLineSpacing(void);
    void lcdSetCursorXY(int x, int y);
    void lcdGetCursorXY(int *x, int *y);
    uint16_t lcdMakeColor(int red, int green, int blue);

    void writeConfigurationByte(int EEPromAddress, byte value);
    byte readConfigurationByte(int EEPromAddress, byte defaultValue);
    void writeConfigurationShort(int EEPromAddress, short value);
    short readConfigurationShort(int EEPromAddress, short defaultValue);
    void writeConfigurationInt(int EEPromAddress, int value);
    int readConfigurationInt(int EEPromAddress, int defaultValue);
    void writeConfigurationFloat(int EEPromAddress, float value);
    float readConfigurationFloat(int EEPromAddress, float defaultValue);



  private:
    //
    // private member variables
    //
    MENU_ITEM *currentMenuTable;
    const ui_font *currentFont;

    uint16_t titleBarColor;
    uint16_t titleBarTextColor;
    uint16_t titleBarBackButtonColor;
    uint16_t titleBarBackButtonSelectedColor;
    const ui_font *titleBarFont;
    int buttonTypeOnTitleBar;

    uint16_t menuBackgroundColor;
    uint16_t menuButtonColor;
    uint16_t menuButtonSelectedColor;
    uint16_t menuButtonFrameColor;
    uint16_t menuButtonTextColor;
    const ui_font *menuButtonFont;

    int numberBoxRepeatCount;

    int touchScreenToLCDOffsetX;
    float touchScreenToLCDScalerX;
    int touchScreenToLCDOffsetY;
    float touchScreenToLCDScalerY;
    int touchState;


    //
    // private functions
    //
    void executeMenuItem(int menuIdx);
    void selectAndDrawMenu(MENU_ITEM *menu);
    void drawMenu(void);
    void drawMenuItem(int menuIdx, boolean buttonSelectedFlg);
    int findMenuButtonForTouchEvent(void);
    void getMenuButtonSizeAndLocation(int menuButtonNumber, int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight);

    void drawTitleBar(const char *titleBarText, int buttonType);
    void drawTitleBarBackButton(boolean buttonSelectedFlg);
    void drawTitleBarMenuButton(boolean buttonSelectedFlg);
    void getBackButtonSizeAndLocation(int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight);
    void getMenuButtonSizeAndLocation(int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight);

    void drawDownButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg);
    void drawUpButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg);
    boolean updateNumberBoxNumber(NUMBER_BOX &numberBox, int stepAmount);
    void drawNumberInNumberBox(NUMBER_BOX &numberBox);
    void getNumberBoxCoordinates(NUMBER_BOX &numberBox, int *downButtonX, int *numberX, int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height);
    void drawDownButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg);
    void drawUpButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg);
    boolean updateNumberBoxNumberFloat(NUMBER_BOX_FLOAT &numberBox, float stepAmount);
    void drawNumberInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox);
    void getNumberBoxCoordinatesFloat(NUMBER_BOX_FLOAT &numberBox, int *downButtonX, int *numberX, int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height);

    void drawSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, boolean showButtonTouchedFlg);
    void getCoordsOfSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, int *X, int *Y, int *width, int *height);
    int countSelectionBoxChoices(SELECTION_BOX &selectionBox);

    void touchScreenInitialize(int lcdOrientation);
    boolean getRAWTouchScreenCoords(int *xRaw, int *yRaw);
     
    void lcdInitialize(int lcdOrientation, const ui_font &font);
};

// ------------------------------------ End ---------------------------------
#endif
