//      ******************************************************************
//      *                                                                *
//      *                      Teensy User Interface                     *
//      *                                                                *
//      *            Stan Reifel                      3/3/2019           *
//      *               Copyright (c) S. Reifel & Co, 2019               *
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

//
// The TeensyUserInterface library uses two other libraries which must be installed:
//    ILI9341_t3          - Driver for the LCD screen
//    XPT2046_Touchscreen - Driver for the touch screen
//

//
// This user interface works with Teensy applications having a 2.8" 320x240 ILI9341 
// LCD touch screen display.  It has only been tested with a Teensy 3.6 but will
// likely work with other Teensys that support the libraries listed above.  The display 
// is divided into two sections:  along the top is a Title Bar and below that is the 
// "Display Space".  The Display Space is where menus, message boxes, configuration 
// screens, along with the application's main display are shown.
//
// The heart of this user interface are menus.  Menus are display as rows and columns of
// touch sensitive buttons.  There are three types of menu buttons: Commands, Toggles 
// and Sub Menus.  Menus are displayed by creating a "Menu Table", then calling: 
// displayAndExecuteMenu()
//
// A typical main menu table might look like this:
//
//  MENU_ITEM mainMenu[] = {
//    {MENU_ITEM_TYPE_MAIN_MENU_HEADER, "My Application Menu",  MENU_COLUMNS_1,       mainMenu},
//    {MENU_ITEM_TYPE_COMMAND,          "Turn on motor",        turnOnMotorCommand,   NULL},
//    {MENU_ITEM_TYPE_COMMAND,          "Turn off motor",       turnOffMotorCommand,  NULL},
//    {MENU_ITEM_TYPE_TOGGLE,           "LED",                  enableLEDCallback,    NULL},
//    {MENU_ITEM_TYPE_SUB_MENU,         "Settings",             NULL,                 settingsMenu},
//    {MENU_ITEM_TYPE_END_OF_MENU,      "",                     NULL,                 NULL}
//  };
//
// The first line in the table always defines what type of menu it is, either a Main 
// Menu, or a Sub Menu.  The table's last line marks the end of the table.  In between 
// are menu items, one line for each button that's displayed on the menu.
//
// A menu can have as many buttons as you like, simply by adding more entries to the menu 
// table.  Buttons are sized such that they always fill the screen, adding more causes
// them to be shorter in height.  In many cases it is advantageous to arrange a menu's 
// buttons in two or more columns.  Columns of buttons often look better, are easier to  
// touch, and you can fit more on a single screen.
//
// The buttons on a menu can be arranged in 1, 2, 3 or 4 columns.  The number of columns 
// is set in the third field of the menu table's first line by inserting one of these 
// values:
//           MENU_COLUMNS_1, MENU_COLUMNS_2, MENU_COLUMNS_3, or MENU_COLUMNS_4

//
// There are 3 different types of buttons that can be added to a menu table:
//
// Command:  A MENU_ITEM_TYPE_COMMAND indicates that a function will be executed  
// when this menu button is pressed.  In the second field is the text displayed
// on the button.  A pointer to the function is entered into the third field of
// the table.
//
// Toggle:  A MENU_ITEM_TYPE_TOGGLE is used somewhat like a Radio Button in a dialog 
// box.  It allows the user to select one of a fixed number of choices (such as  
// On / Off,   or   Red / Green / Blue).  Each time the user presses this button, 
// it alternates the selection (i.e. toggles between On and Off, or rotates between 
// Red, Green and Blue).  The third field in this entry points to a callback 
// function that alternates the value.
//
// Sub Menu:  A MENU_ITEM_TYPE_SUB_MENU is used to select a different menu.  For 
// example, a main menu might reference a "Settings" sub menu. The fourth field  
// in this entry points to another menu table where the sub menu is defined.
//
// As described above, the first line in a menu table indicates the type of menu.  
// There are 2 types:
//
// Main Menu:  A MENU_ITEM_TYPE_MAIN_MENU_HEADER in the first line of the table  
// specifies that it is a main menu (a main menu may have child menus, but no 
// parent menus).  The three other columns in this line define more about the menu.  
// The second field contains text printed on the LCD's Title Bar when the menu is
// displayed.  The next column sets how the buttons are arranged on the screen.  
// Legal values for this field are: MENU_COLUMNS_1, MENU_COLUMNS_2, MENU_COLUMNS_3,  
// or MENU_COLUMNS_4.  The fourth field controls if a "Back" button is displayed on 
// the Title Bar for this menu.  Assigning the fourth field "mainMenu" (or what 
// every you named this menu table) will NOT show the "Back" button.  Alternately 
// if the fourth field is set to NULL, the "Back" button is included.  In this 
// case pressing "Back" will exit all menus, returning to the main application.
//
// Sub Menu:  A MENU_ITEM_TYPE_SUB_MENU_HEADER in the first line specifies that the 
// table defines a sub menu.  Sub menus are menus that are called from a main menu, 
// or another sub menu.  The fourth field in this line is a pointer back to the 
// parent menu table (typically this would be the main menu).  This is used to 
// reselect the parent menu when the user presses the "Back" button while the sub 
// menu is displayed.
//
// Typically applications have one or more menus, each having several buttons.  Each
// of these buttons invokes a separate "Command" that is written by the application's 
// developer.  These command may directly do something or display something; or they 
// might prompt the user for more information.
//
// To help the developer create screens that prompt the user for more information,
// this user interface include three touch sensitive widgets that can be easily  
// added to custom screens.  They are:
//
// Buttons:  Buttons are often used to exit screens with labels such as "OK" or   
// "Cancel", but can also be used to execute other commands such as "Start" or "Stop".
//
// Number Boxes:  Number boxes allow the user to enter a number by pressing Up and 
// Down.  Numbers can be integers or floats.
//
// Selection Boxes:  Selection boxes present choices to the user to pick between, 
// such as On / Off; or Low / Medium / High
//
// In addition to these widgets, there are many primitive functions for drawing to 
// the LCD display.  They include functions for drawing text, lines, circles, 
// rectangles...  For drawing text there are many font sizes to choose from.  The  
// Arial fonts that look best are: 8, 8Bold, 9, 9Bold, 12Bold, 13
//
//
// Hooking up the Teensy 3.6 to the LCD Touchscreen:
//    Teensy VIN        LCD VCC
//    Teensy GND        LCD GND
//    Teensy D10        LCD CS
//    Teensy 3.3V       LCD RESET
//    Teensy D9         LCD DC
//    Teensy D11        LCD SDI
//    Teensy D13        LCD CLK
//    Teensy 3.3V       LCD LED BACKLIGHT
//    Teensy D12        LCD SDO
//    Teensy D13        TOUCH CLK
//    Teensy D8         TOUCH CS 
//    Teensy D11        TOUCH DIN
//    Teensy D12        TOUCH D0
//

// ---------------------------------------------------------------------------------

#include <EEPROM.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include "TeensyUserInterface.h"


//
// pin assignments, note: the LCD also uses hardware SPI with MOSI pin 11, MISO pin 12, SCK pin 13
//
const byte LCD_DC_PIN = 9;
const byte LCD_CS_PIN = 10;
const byte TOUCH_CS_PIN = 8;


//
// create the LCD and Touch objects
//
ILI9341_t3 lcd = ILI9341_t3(LCD_CS_PIN, LCD_DC_PIN);
XPT2046_Touchscreen ts(TOUCH_CS_PIN);

//
// the size of features for drawing the user interface
//
const int titleBarHeight = 34;
const int arrowWidth = 8;


// ---------------------------------------------------------------------------------
//                       Setup functions for the User Interface
// ---------------------------------------------------------------------------------

//
// constructor for the TeensyUserInterface class
//
TeensyUserInterface::TeensyUserInterface(void)
{
}



//
// initialize the UI, display hardware and touchscreen hardware
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//          font -> the font typeface to load, ei: Arial_10
//
void TeensyUserInterface::begin(int lcdOrientation, const ui_font &font)
{
  //
  // initialize the LCD and touch screen hardware
  //
  lcdInitialize(lcdOrientation, font);
  touchScreenInitialize(lcdOrientation);

  //
  // initialize the user interface
  //
  displaySpaceWidth = lcdWidth - 2;
  displaySpaceHeight = lcdHeight - titleBarHeight - 1;
  displaySpaceLeftX = 1;
  displaySpaceRightX = displaySpaceLeftX + displaySpaceWidth - 1;
  displaySpaceTopY = titleBarHeight;
  displaySpaceBottomY = displaySpaceTopY + displaySpaceHeight - 1;
  displaySpaceCenterX = displaySpaceLeftX + displaySpaceWidth/2;
  displaySpaceCenterY = displaySpaceTopY + displaySpaceHeight/2;

  //
  // set some default colors and fonts for the UI
  //
  setColorPaletteBlue();
  setTitleBarFont(font);
  setMenuFont(font);
}



//
// set color palette to Blue
//
void TeensyUserInterface::setColorPaletteBlue(void)
{
  setTitleBarColors(LCD_BLUE, LCD_WHITE, LCD_DARKBLUE, 0x8C5F);
  setMenuColors(LCD_BLACK, LCD_BLUE, 0x8C5F, LCD_LIGHTBLUE, LCD_WHITE);
}



//
// set color palette to Gray
//
void TeensyUserInterface::setColorPaletteGray(void)
{
  uint16_t red;
  uint16_t green;
  uint16_t blue;

  red = 11; green = 22; blue = 11-5;  uint16_t titleBarColor                   = lcdMakeColor(red, green, blue);
  red = 9; green = 18; blue = 9-4;    uint16_t titleBarBackButtonColor         = lcdMakeColor(red, green, blue);
  red = 16; green = 32; blue = 16-5;  uint16_t titleBarBackButtonSelectedColor = lcdMakeColor(red, green, blue);
                                      uint16_t titleBarTextColor               = LCD_WHITE;
                                      uint16_t menuBackgroundColor             = LCD_BLACK;
  red = 9; green = 18; blue = 9-4;    uint16_t menuButtonColor                 = lcdMakeColor(red, green, blue);
                                      uint16_t menuButtonSelectedColor         = titleBarBackButtonSelectedColor;
  red = 12; green = 24; blue = 12-4;  uint16_t menuButtonFrameColor            = lcdMakeColor(red, green, blue);
                                      uint16_t menuButtonTextColor             = LCD_WHITE;
                                      
  setTitleBarColors(titleBarColor, titleBarTextColor, titleBarBackButtonColor, titleBarBackButtonSelectedColor);
  setMenuColors(menuBackgroundColor, menuButtonColor, menuButtonSelectedColor, menuButtonFrameColor, menuButtonTextColor);
}


// ---------------------------------------------------------------------------------
//                                   Menu functions  
// ---------------------------------------------------------------------------------

//
// set the menu colors
//  Enter:  _menuBackgroundColor = RGB565 color for background of the menu
//          _menuButtonColor = RGB565 color for menu buttons
//          _menuButtonSelectedColor = RGB565 color for menu buttons that are selected
//          _menuButtonFrameColor = RGB565 color for highlight around the menu buttons
//          _menuButtonTextColor = RGB565 color for text of the buttons
//
void TeensyUserInterface::setMenuColors(uint16_t _menuBackgroundColor, uint16_t _menuButtonColor,
  uint16_t _menuButtonSelectedColor, uint16_t _menuButtonFrameColor, uint16_t _menuButtonTextColor)
{
    menuBackgroundColor = _menuBackgroundColor;
    menuButtonColor = _menuButtonColor;
    menuButtonSelectedColor = _menuButtonSelectedColor;
    menuButtonFrameColor = _menuButtonFrameColor;
    menuButtonTextColor = _menuButtonTextColor;
}



//
// set the menu font
//  Enter:  font -> the font to use when drawing the title bar
//          ie: Arial_8, Arial_8_Bold, Arial_9, Arial_9_Bold, Arial_10, Arial_10_Bold, Arial_11, Arial_11_Bold,
//              Arial_12, 13, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60, 72, 96
//
void TeensyUserInterface::setMenuFont(const ui_font &font)
{
  menuButtonFont = &font;
}



//
// display the top level menu, then execute the commands selected by the user
//  Enter:  menu -> the menu to display
//
void TeensyUserInterface::displayAndExecuteMenu(MENU_ITEM *menu)
{
  int menuIdx;
  MENU_ITEM *parentMenu;
  
  //
  // display the top level menu
  //
  selectAndDrawMenu(menu);
 
  //
  // check for screen touches and execute menu commands
  //
  while(true)
  {
    //
    // get new button events
    //
    getTouchEvents();
    if (touchEventType == TOUCH_NO_EVENT)
      continue;

    //
    // check if user has pressed the menu's "Back" button on the title bar
    //
    if (checkForBackButtonClicked())
    {
      //
      // the menu's Back button pushed, get this menu's type
      //
      int menuItemType = currentMenuTable[0].MenuItemType;

      //
      // if this is a sub menu, select this menu's parent menu
      //
      if (menuItemType == MENU_ITEM_TYPE_SUB_MENU_HEADER)
      {
        parentMenu = currentMenuTable[0].MenuItemSubMenu;
        selectAndDrawMenu(parentMenu);
        continue;
      }

      //
      // if this is the Main menu, exit the menus and return to the app
      //
      if (menuItemType == MENU_ITEM_TYPE_MAIN_MENU_HEADER)
      {
        parentMenu = currentMenuTable[0].MenuItemSubMenu;
        if (parentMenu == NULL)
          return;
        continue;
      }
    }

    //
    // check if user has pressed one of the menu's buttons
    //
    menuIdx = findMenuButtonForTouchEvent();
    if (menuIdx > 0)
    {
      if (touchEventType == TOUCH_PUSHED_EVENT)
        drawMenuItem(menuIdx, true);

      if (touchEventType == TOUCH_RELEASED_EVENT)
      {
        drawMenuItem(menuIdx, false);
        executeMenuItem(menuIdx);
      }
    }
  }
}



//
// execute the given menu item
//  Enter:  menuIdx = index into the menu table of the menu item to execute
//
void TeensyUserInterface::executeMenuItem(int menuIdx)
{
  MENU_ITEM *subMenu;

  //
  // determine the type of menu entry, then execute it
  //
  switch(currentMenuTable[menuIdx].MenuItemType)
  {    
    //
    // select a "Sub menu" from the current menu
    //
    case MENU_ITEM_TYPE_SUB_MENU:
    {
      subMenu = currentMenuTable[menuIdx].MenuItemSubMenu;
      selectAndDrawMenu(subMenu);
      break;
    }
    
    //
    // execute the menu item's function
    //
    case MENU_ITEM_TYPE_COMMAND:
    {
      //
      // execute the menu item's function
      //
      (currentMenuTable[menuIdx].MenuItemFunction)();
      
      //
      // display the menu again
      //
      selectAndDrawMenu(currentMenuTable);
      break;
    }
    
    //
    // toggle the menu item, then redisplay
    //
    case MENU_ITEM_TYPE_TOGGLE:
    {
      toggleSelectNextStateFlg = true;
      (currentMenuTable[menuIdx].MenuItemFunction)();
      drawMenuItem(menuIdx, false);
    }
  }
}



//
// select and display a menu or submenu
//  Enter:  menu -> the menu to display
//
void TeensyUserInterface::selectAndDrawMenu(MENU_ITEM *menu)
{   
  //
  // remember the currently selected menu
  //
  currentMenuTable = menu;

  //
  // draw the title bar, decide if should include the "Back" button (don't show the 
  // Back button if this is the main menu and it links back to itself)
  //
  if ((currentMenuTable[0].MenuItemType == MENU_ITEM_TYPE_MAIN_MENU_HEADER) &&
    (currentMenuTable[0].MenuItemSubMenu != NULL))
    drawTitleBar(currentMenuTable[0].MenuItemText);
  else
    drawTitleBarWithBackButton(currentMenuTable[0].MenuItemText);
   
  //
  // clear the display space
  //
  clearDisplaySpace();

  //
  // draw all buttons in the menu
  //
  drawMenu();
}



//
// display the currently selected menu
//  Enter:  currentMenuTable -> the menu to display
//
void TeensyUserInterface::drawMenu(void)
{ 
  int menuIdx = 1;
 
  //
  // loop through the menu, drawing each button
  //
  while(true)
  {
    if(currentMenuTable[menuIdx].MenuItemType == MENU_ITEM_TYPE_END_OF_MENU)
      break;

    drawMenuItem(menuIdx, false);
    menuIdx++;
  }
}



//
// draw one button on the menu
//  Enter:  menuIdx = the index into the menu table of the button to draw
//          buttonSelectedFlg = true if should show the button selected
//
void TeensyUserInterface::drawMenuItem(int menuIdx, boolean buttonSelectedFlg)
{
  int buttonX;
  int buttonY;
  int buttonWidth;
  int buttonHeight;
 
  //
  // determine the coordinates and size of the button, then draw it
  //
  getMenuButtonSizeAndLocation(menuIdx, &buttonX, &buttonY, &buttonWidth, &buttonHeight);

  //
  // get the button text
  //
  const char *menuItemText = currentMenuTable[menuIdx].MenuItemText;
    
  //
  // determine the menu entry type and draw it
  //
  switch(currentMenuTable[menuIdx].MenuItemType)
  {    
    //
    // display a "sub menu" button
    //    
    case MENU_ITEM_TYPE_SUB_MENU:
    {
      //
      // draw the button
      //
      drawButton(menuItemText, buttonSelectedFlg, buttonX, buttonY, buttonWidth, buttonHeight);

      //
      // draw a triangle showing the button connects to a submenu
      //
      int arrowX = buttonX + buttonWidth - 18;
      int arrowCenterY = buttonY + buttonHeight / 2;
      lcdDrawFilledTriangle(arrowX,               arrowCenterY - arrowWidth/2,
                            arrowX + arrowWidth,  arrowCenterY,
                            arrowX,               arrowCenterY + arrowWidth/2,
                            menuButtonTextColor);
      break;
    }

    //
    // display a "command" button
    //
    case MENU_ITEM_TYPE_COMMAND:
    {
      drawButton(menuItemText, buttonSelectedFlg, buttonX, buttonY, buttonWidth, buttonHeight);
      break;
    }

    //
    // display a "toggle" button
    //    
    case MENU_ITEM_TYPE_TOGGLE:
    {
      //
      // execute the callback fuction to get the toggle button's text
      //
      toggleSelectNextStateFlg = false;
      (currentMenuTable[menuIdx].MenuItemFunction)();


      //
      // build the text to display on the button
      //
      char s[100];
      strcpy(s, menuItemText);
      strcat(s, ":  ");
      strcat(s, toggleText);
      drawButton(s, buttonSelectedFlg, buttonX, buttonY, buttonWidth, buttonHeight);
    }
  }
}



//
// find a menu button given LCD coords
//  Enter:  touchEventX, touchEventY = screen coordinates where touch event occurred
//  Exit:   index of menu entry returned if found, else 0
//
int TeensyUserInterface::findMenuButtonForTouchEvent(void)
{
  int menuIdx = 1;
  int buttonX, buttonY;
  int buttonWidth, buttonHeight;
 
  //
  // loop through the menu, testing each button location
  //
  while(true)
  {
    //
    // check if at end of table, indicating that a matching button wasn't found
    //
    if(currentMenuTable[menuIdx].MenuItemType == MENU_ITEM_TYPE_END_OF_MENU)
      return(0);

    //
    // check if the given coords were inside this button
    //
    getMenuButtonSizeAndLocation(menuIdx, &buttonX, &buttonY, &buttonWidth, &buttonHeight);

    if ((touchEventX >= buttonX) && (touchEventX <= buttonX + buttonWidth - 1))
    {
      if ((touchEventY >= buttonY) && (touchEventY <= buttonY + buttonHeight - 1))
        return(menuIdx);
    }
    menuIdx++;
  }
}



//
// get the XY coords and size of a menu button 
//  Enter:  menuIdx = index into the menu table of the button to get coords for
//          buttonX, buttonY -> storage to return XY coords of the button
//          buttonWidth, buttonHeight -> storage to return size of the button
//
void TeensyUserInterface::getMenuButtonSizeAndLocation(int menuIdx, int *buttonX, int *buttonY, 
  int *buttonWidth, int *buttonHeight)
{
  int menuButtonNumber = menuIdx - 1;
  int buttonCountOnThisRow;
  
  //
  // count the total number of buttons
  //
  int idx = 1;
  int buttonCount = 0;
  while(true)
  {
    if(currentMenuTable[idx].MenuItemType == MENU_ITEM_TYPE_END_OF_MENU)
      break;

    idx++;
    buttonCount++;
  }

  //
  // determine the number of rows and columns of buttons
  //
  int columnsOfButtons = (int) currentMenuTable[0].MenuItemFunction;
  if ((columnsOfButtons < 1) || (columnsOfButtons > 4))
    columnsOfButtons = 1;
    
  int rowsOfButtons = (buttonCount + columnsOfButtons - 1) / columnsOfButtons;

  //
  // determine the width of the buttons
  //
  int paddingOnTheSidesOfButtons = 10;
  int paddingBetweenButtons = 10;
  *buttonWidth = (displaySpaceWidth - (paddingOnTheSidesOfButtons*2) - (paddingBetweenButtons*(columnsOfButtons-1))) / columnsOfButtons;
  paddingOnTheSidesOfButtons = (displaySpaceWidth - (*buttonWidth * columnsOfButtons) - (paddingBetweenButtons*(columnsOfButtons-1))) / 2;

  //
  // determine the height of the buttons
  //
  int paddingOnTopAndBottomOfButtons = 10;
  *buttonHeight = (displaySpaceHeight - (paddingOnTopAndBottomOfButtons*2) - (paddingBetweenButtons*(rowsOfButtons-1))) / rowsOfButtons;
  paddingOnTopAndBottomOfButtons = (displaySpaceHeight - (*buttonHeight * rowsOfButtons) - (paddingBetweenButtons*(rowsOfButtons-1))) / 2;

  //
  // determine the row and column of this button
  //
  int buttonRow = menuButtonNumber / columnsOfButtons;
  int buttonColumn = menuButtonNumber - (buttonRow * columnsOfButtons);

  //
  // determine the XY coords of the button's upper left corner
  //
  if ((buttonRow != rowsOfButtons - 1) || (buttonCount % columnsOfButtons == 0))
    buttonCountOnThisRow = columnsOfButtons;
  else
    buttonCountOnThisRow = buttonCount % columnsOfButtons;

  int leftMostButtonX = displaySpaceLeftX + (displaySpaceWidth - (*buttonWidth * buttonCountOnThisRow) - (paddingBetweenButtons*(buttonCountOnThisRow-1))) / 2;
  
  *buttonX = leftMostButtonX + (*buttonWidth + paddingBetweenButtons) * buttonColumn;
  *buttonY = displaySpaceTopY + paddingOnTopAndBottomOfButtons + (*buttonHeight + paddingBetweenButtons) * buttonRow;
}


// ---------------------------------------------------------------------------------
//                                 Title bar functions  
// ---------------------------------------------------------------------------------

const int TITLE_BAR_BUTTON_TYPE_NONE  = 0;
const int TITLE_BAR_BUTTON_TYPE_BACK  = 1;
const int TITLE_BAR_BUTTON_TYPE_MENU  = 2;


//
// set the title bar colors
//  Enter:  _titleBarColor = RGB565 color for the title bar background
//          _titleBarTextColor = RGB565 color for the title bar's text
//          _titleBarBackButtonColor = RGB565 color for the back button on the title bar
//
void TeensyUserInterface::setTitleBarColors(uint16_t _titleBarColor, uint16_t _titleBarTextColor, 
  uint16_t _titleBarBackButtonColor, uint16_t _titleBarBackButtonSelectedColor)
{
    titleBarColor = _titleBarColor;
    titleBarTextColor = _titleBarTextColor;
    titleBarBackButtonColor = _titleBarBackButtonColor;
    titleBarBackButtonSelectedColor = _titleBarBackButtonSelectedColor;
}



//
// set the title bar font
//  Enter:  font -> the font to use when drawing the title bar
//          ie: Arial_8, Arial_8_Bold, Arial_9, Arial_9_Bold, Arial_10, Arial_10_Bold, Arial_11, Arial_11_Bold,
//              Arial_12, 13, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60, 72, 96
//
void TeensyUserInterface::setTitleBarFont(const ui_font &font)
{
  titleBarFont = &font;
}


//
// draw the title bar (without the back or hamburger button)
//
void TeensyUserInterface::drawTitleBar(const char *titleBarText)
{
  drawTitleBar(titleBarText, TITLE_BAR_BUTTON_TYPE_NONE);
}


//
// draw the title bar with the back button
//
void TeensyUserInterface::drawTitleBarWithBackButton(const char *titleBarText)
{
  drawTitleBar(titleBarText, TITLE_BAR_BUTTON_TYPE_BACK);
}


//
// draw the title bar with the Menu button
//
void TeensyUserInterface::drawTitleBarWithMenuButton(const char *titleBarText)
{
  drawTitleBar(titleBarText, TITLE_BAR_BUTTON_TYPE_MENU);
}


//
// draw the title bar above the Display Space
//
void TeensyUserInterface::drawTitleBar(const char *titleBarText, int buttonType)
{
  //
  // remember if the title bar includes a button
  //
  buttonTypeOnTitleBar = buttonType;
  
  //
  // draw the "bar" of the title bar along the top of the screen
  //
  lcdDrawFilledRectangle(0, 0, lcdWidth, titleBarHeight, titleBarColor);

  //
  // draw the text on the title bar, first figure out how it will fit best
  //
  lcdSetFont(*titleBarFont);
  int titleBarTextX = (lcdWidth / 2) - (lcdStringWidthInPixels(titleBarText) / 2);
  if (titleBarTextX < 2) titleBarTextX = 2;

  //
  // check space is needed on the title bar for the Back button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_BACK)
  {
    int backButtonHeight;
    int backButtonWidth;
    int backButtonX;
    int backButtonY;

    getBackButtonSizeAndLocation(&backButtonX, &backButtonY, &backButtonWidth, &backButtonHeight);
    if (titleBarTextX < backButtonX + backButtonWidth + 6) 
      titleBarTextX = backButtonX + backButtonWidth + 6;
  }

  //
  // check space is needed on the title bar for the Menu button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_MENU)
  {
  int menuButtonHeight;
  int menuButtonWidth;
  int menuButtonX;
  int menuButtonY;
  
  getMenuButtonSizeAndLocation(&menuButtonX, &menuButtonY, &menuButtonWidth, &menuButtonHeight);
    if (titleBarTextX < menuButtonX + menuButtonWidth + 6) 
      titleBarTextX = menuButtonX + menuButtonWidth + 6;
  }

  //
  // draw the title bar text
  //
  lcdSetFont(*titleBarFont);
  lcdSetFontColor(titleBarTextColor);
  int titleBarTextY = (titleBarHeight / 2) - (lcdGetFontHeightWithoutDecenders() / 2);
  lcdSetCursorXY(titleBarTextX, titleBarTextY);
  lcdPrint(titleBarText); 

  //
  // optionally draw the Back button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_BACK)
    drawTitleBarBackButton(false);

  //
  // optionally draw the Hamburger button
  //
  if (buttonType == TITLE_BAR_BUTTON_TYPE_MENU)
    drawTitleBarMenuButton(false);
}



//
// draw the "Back" button on the title bar
//  Enter:  buttonSelectedFlg = true if should show the button as "pressed"
//
void TeensyUserInterface::drawTitleBarBackButton(boolean buttonSelectedFlg)
{
  //
  // get the location of the Back button
  //
  int backButtonHeight;
  int backButtonWidth;
  int backButtonX;
  int backButtonY;
  getBackButtonSizeAndLocation(&backButtonX, &backButtonY, &backButtonWidth, &backButtonHeight);
  int backButtonRadius = backButtonHeight / 2;

  //
  // draw the button as a rounded rect
  //
  uint16_t buttonColor;
  if (buttonSelectedFlg)
    buttonColor = titleBarBackButtonSelectedColor;
  else
    buttonColor = titleBarBackButtonColor;
  
  lcdDrawFilledRoundedRectangle(backButtonX, backButtonY, backButtonWidth, backButtonHeight, 
    backButtonRadius, buttonColor);

  //
  // place the text "Back" on the button
  //
  lcdSetFont(*titleBarFont);
  lcdSetFontColor(titleBarTextColor);
  lcdSetCursorXY(backButtonX + backButtonRadius + arrowWidth*2 - 2, backButtonY + backButtonHeight/2 - lcdGetFontHeightWithoutDecenders()/2 - 1);
  lcdPrint("Back");

  //
  // draw a triangle showing button goes back to previous menu
  //
  int arrowX = backButtonX + backButtonRadius - 2;
  int arrowCenterY = backButtonY + backButtonRadius - 1;
  lcdDrawFilledTriangle(arrowX,              arrowCenterY,
                        arrowX + arrowWidth, arrowCenterY - arrowWidth/2,
                        arrowX + arrowWidth, arrowCenterY + arrowWidth/2,
                        menuButtonTextColor);
}



//
// draw the Menu button on the title bar
//  Enter:  buttonSelectedFlg = true if should show the button as "pressed"
//
void TeensyUserInterface::drawTitleBarMenuButton(boolean buttonSelectedFlg)
{
  //
  // get the location of the Menu button
  //
  int menuButtonHeight;
  int menuButtonWidth;
  int menuButtonX;
  int menuButtonY;
  getMenuButtonSizeAndLocation(&menuButtonX, &menuButtonY, &menuButtonWidth, &menuButtonHeight);
  int menuButtonRadius = menuButtonHeight / 4;


  //
  // draw the button as a rounded rect
  //
  uint16_t buttonColor;
  if (buttonSelectedFlg)
    buttonColor = titleBarBackButtonSelectedColor;
  else
    buttonColor = titleBarBackButtonColor;
  
  lcdDrawFilledRoundedRectangle(menuButtonX, menuButtonY, menuButtonWidth, menuButtonHeight, 
    menuButtonRadius, buttonColor);

  //
  // draw a triangle showing button goes back to previous menu
  //
  int menuLinesWidth = menuButtonWidth / 2;
  int menuLinesLeftX = menuButtonX + menuButtonWidth/2 - menuLinesWidth/2;
  int menuLinesMiddleY =  menuButtonY + menuButtonHeight/2 - 1;
  
  lcdDrawFilledRectangle(menuLinesLeftX, menuLinesMiddleY, menuLinesWidth, 2, menuButtonTextColor);
  lcdDrawFilledRectangle(menuLinesLeftX, menuLinesMiddleY - 6, menuLinesWidth, 2, menuButtonTextColor);
  lcdDrawFilledRectangle(menuLinesLeftX, menuLinesMiddleY + 6, menuLinesWidth, 2, menuButtonTextColor);
}



//
// check if user has touched and released the title bar's Back button, this also highlights
// the button when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Exit:   true returned if user has touched and released the Back button, else false
//
boolean TeensyUserInterface::checkForBackButtonClicked(void)
{
  int X1;
  int Y1;
  int buttonWidth;
  int buttonHeight;

  //
  // check if a Back button is displayed on the title bar
  //
  if (buttonTypeOnTitleBar == TITLE_BAR_BUTTON_TYPE_BACK)
  {
    //
    // return if there is No Event
    //
    if (touchEventType == TOUCH_NO_EVENT)
      return(false);
      
    //
    // get the coordinates of the Back button
    //
    getBackButtonSizeAndLocation(&X1, &Y1, &buttonWidth, &buttonHeight);
    int X2 = X1 + buttonWidth - 1;
    int Y2 = Y1 + buttonHeight - 1;
    
    //
    // check if most recent event was: this button "PUSHED"
    //
    if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarBackButton(true);
      return(false);
    }
    
    //
    // check if most recent event was: this button "RELEASED"
    //
    if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarBackButton(false);
      return(true);
    }
  }
  
  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user has touched and released the title bar's Menu button, this also highlights
// the button when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Exit:   true returned if user has touched and released the Menu button, else false
//
boolean TeensyUserInterface::checkForMenuButtonClicked(void)
{
  int X1;
  int Y1;
  int buttonWidth;
  int buttonHeight;

  //
  // check if a Menu button is displayed on the title bar
  //
  if (buttonTypeOnTitleBar == TITLE_BAR_BUTTON_TYPE_MENU)
  {
    //
    // return if there is No Event
    //
    if (touchEventType == TOUCH_NO_EVENT)
      return(false);
      
    //
    // get the coordinates of the Menu button
    //
    getMenuButtonSizeAndLocation(&X1, &Y1, &buttonWidth, &buttonHeight);
    int X2 = X1 + buttonWidth - 1;
    int Y2 = Y1 + buttonHeight - 1;
    
    //
    // check if most recent event was: this button "PUSHED"
    //
    if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarMenuButton(true);
      return(false);
    }
    
    //
    // check if most recent event was: this button "RELEASED"
    //
    if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
    {
      drawTitleBarMenuButton(false);
      return(true);
    }
  }
  
  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// get the XY coords and size of a title bar "Back" button 
//  Enter:  buttonX, buttonY -> storage to return XY coords of the button
//          buttonWidth, buttonHeight -> storage to return size of the button
//
void TeensyUserInterface::getBackButtonSizeAndLocation(int *buttonX, int *buttonY, 
  int *buttonWidth, int *buttonHeight)
{
  lcdSetFont(*titleBarFont);

  const char *backButtonText = "Back";
  *buttonHeight = titleBarHeight - 6;
  int backButtonRadius = *buttonHeight / 2;
  *buttonWidth = backButtonRadius + arrowWidth*2 + lcdStringWidthInPixels(backButtonText) + backButtonRadius - 3;
  *buttonX = 4;
  *buttonY = (titleBarHeight - *buttonHeight) / 2;
}



//
// get the XY coords and size of a title bar "Menu" button 
//  Enter:  buttonX, buttonY -> storage to return XY coords of the button
//          buttonWidth, buttonHeight -> storage to return size of the button
//
void TeensyUserInterface::getMenuButtonSizeAndLocation(int *buttonX, int *buttonY, int *buttonWidth, int *buttonHeight)
{
  *buttonHeight = titleBarHeight - 6;
  *buttonWidth = (*buttonHeight * 18) / 10;
  *buttonX = 4;
  *buttonY = (titleBarHeight - *buttonHeight) / 2;
}

// ---------------------------------------------------------------------------------
//                               Display space functions  
// ---------------------------------------------------------------------------------

//
// clear the screen's "display space" using the menu's background color and draw a frame around it 
//
void TeensyUserInterface::clearDisplaySpace(void)
{
  clearDisplaySpace(menuBackgroundColor);
}

//
// clear the screen's "display space" using the given background color and draw a frame around it
//  Enter:  backgroundColor = color to fill the display space
//
void TeensyUserInterface::clearDisplaySpace(uint16_t backgroundColor)
{  
  //
  // draw the frame
  //
  lcdDrawLine(displaySpaceRightX+1, displaySpaceTopY,         displaySpaceRightX+1, displaySpaceBottomY+1,   titleBarColor);
  lcdDrawLine(displaySpaceRightX+1, displaySpaceBottomY+1,    displaySpaceLeftX-1,  displaySpaceBottomY+1,   titleBarColor);
  lcdDrawLine(displaySpaceLeftX-1,  displaySpaceBottomY+1,    displaySpaceLeftX-1,  displaySpaceTopY,        titleBarColor);

  //
  // blank the display space
  //
  lcdDrawFilledRectangle(displaySpaceLeftX, displaySpaceTopY, displaySpaceWidth, displaySpaceHeight, backgroundColor);
}


// ---------------------------------------------------------------------------------
//                                  Button functions  
// ---------------------------------------------------------------------------------

//
// draw a rectangular button using the colors and font defined for the menu
//  Enter:  uiButton -> the specifications for the button to draw
//
void TeensyUserInterface::drawButton(BUTTON &uiButton)
{
  int buttonX = uiButton.centerX - uiButton.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButton.centerY - uiButton.height/2;
  if (buttonY < 0) buttonY = 0;
  
  drawButton(uiButton.labelText, buttonX, buttonY, uiButton.width, uiButton.height, menuButtonColor, 
    menuButtonFrameColor, menuButtonTextColor, *menuButtonFont);
}

//
// draw a rectangular button using the colors and font defined for the menu
//  Enter:  uiButton -> the specifications for the button to draw
//          showButtonTouchedFlg = true to draw button showing it's being touched, false to draw normal
//
void TeensyUserInterface::drawButton(BUTTON &uiButton, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;

  int buttonX = uiButton.centerX - uiButton.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButton.centerY - uiButton.height/2;
  if (buttonY < 0) buttonY = 0;

  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;
  
  drawButton(uiButton.labelText, buttonX, buttonY, uiButton.width, uiButton.height, buttonColor, 
    menuButtonFrameColor, menuButtonTextColor, *menuButtonFont);
}

//
// draw a rectangular button with extended options for setting color and font
//  Enter:  uiButton -> the specifications for the button to draw
//
void TeensyUserInterface::drawButton(BUTTON_EXTENDED &uiButtonExt)
{
  int buttonX = uiButtonExt.centerX - uiButtonExt.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButtonExt.centerY - uiButtonExt.height/2;
  if (buttonY < 0) buttonY = 0;
  
  drawButton(uiButtonExt.labelText, buttonX, buttonY, uiButtonExt.width, uiButtonExt.height, 
    uiButtonExt.buttonColor, uiButtonExt.buttonFrameColor, uiButtonExt.buttonTextColor, uiButtonExt.buttonFont);
}

//
// draw a rectangular button with extended options for setting color and font
//  Enter:  uiButton -> the specifications for the button to draw
//          showButtonTouchedFlg = true to draw button showing it's being touched, false to draw normal
//
void TeensyUserInterface::drawButton(BUTTON_EXTENDED &uiButtonExt, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;

  int buttonX = uiButtonExt.centerX - uiButtonExt.width/2;
  if (buttonX < 0) buttonX = 0;
  int buttonY = uiButtonExt.centerY - uiButtonExt.height/2;
  if (buttonY < 0) buttonY = 0;

  if (showButtonTouchedFlg)
    buttonColor = uiButtonExt.buttonSelectedColor;
  else
    buttonColor = uiButtonExt.buttonColor;
  
  drawButton(uiButtonExt.labelText, buttonX, buttonY, uiButtonExt.width, uiButtonExt.height, 
    buttonColor, uiButtonExt.buttonFrameColor, uiButtonExt.buttonTextColor, uiButtonExt.buttonFont);
}

//
// draw a rectangular button using the colors and font defined for the menu
//  Enter:  labelText -> text to display on the button's face
//          showButtonTouchedFlg = true to draw button showing it's being touched, false to draw normal
//          buttonX, buttonY = screen coords for the button's upper left corner
//          buttonWidth, buttonHeight = size of the button
//
void TeensyUserInterface::drawButton(const char *labelText, boolean showButtonTouchedFlg, 
  int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
  uint16_t buttonColor;

  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;

  drawButton(labelText, buttonX, buttonY, buttonWidth, buttonHeight, buttonColor, 
    menuButtonFrameColor, menuButtonTextColor, *menuButtonFont);
}

//
// draw a rectangular button
//  Enter:  labelText -> text to display on the button's face
//          buttonX, buttonY = screen coords for the button's upper left corner
//          buttonWidth, buttonHeight = size of the button
//          buttonColor = color of the button's face
//          buttonFrameColor = color to make the button look raised
//          buttonTextColor = color for the button's text
//          buttonFont -> font for the button's text
//
void TeensyUserInterface::drawButton(const char *labelText, int buttonX, int buttonY, int buttonWidth, 
  int buttonHeight, uint16_t buttonColor, uint16_t buttonFrameColor, uint16_t buttonTextColor, 
  const ui_font &buttonFont)
{
  //
  // draw the button's face with raised edges
  //
  lcdDrawLine(buttonX,  buttonY + buttonHeight-1,   buttonX,                  buttonY,   buttonFrameColor);
  lcdDrawLine(buttonX,  buttonY,                    buttonX + buttonWidth-1,  buttonY,   buttonFrameColor);
  lcdDrawFilledRectangle(buttonX+1, buttonY+1, buttonWidth-1, buttonHeight-1, buttonColor);

  //
  // draw the text on the button
  //
  lcdSetFont(buttonFont);
  lcdSetCursorXY(buttonX + buttonWidth/2, buttonY + (buttonHeight / 2) - (lcdGetFontHeightWithoutDecenders()/2));  
  lcdSetFontColor(buttonTextColor);
  lcdPrintCentered(labelText);
}



//
// check if user has touched and released the given button, this also highlights the button
// when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonClicked(BUTTON &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(false);
  }
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(true);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}

//
// check if user has touched and released the given button, this also highlights the button
// when the user first touches it
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonClicked(BUTTON_EXTENDED &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(false);
  }
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(true);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user is holding down the given button and it's now "Auto repeating"
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if there's an Auto Repeat event for this button, else false
//
boolean TeensyUserInterface::checkForButtonAutoRepeat(BUTTON &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
  
  //
  // check if most recent event was: this button "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
    return(true);
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}

//
// check if user is holding down the given button and it's now "Auto repeating"
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if there's an Auto Repeat event for this button, else false
//
boolean TeensyUserInterface::checkForButtonAutoRepeat(BUTTON_EXTENDED &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
  
  //
  // check if most recent event was: this button "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
    return(true);
  
  //
  // check if most recent event was: this button "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user has just touched the given button, this happens once each time a button is
// pressed, in most cases using checkForButtonClicked() is preferred
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonFirstTouched(BUTTON &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
 
  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// check if user has just touched the given button, this happens once each time a button is
// pressed, in most cases using checkForButtonClicked() is preferred
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  uiButton -> the button to test
//  Exit:   true returned if user has touched and released this button, else false
//
boolean TeensyUserInterface::checkForButtonFirstTouched(BUTTON_EXTENDED &uiButton)
{
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  int X1 = uiButton.centerX - uiButton.width/2;
  int Y1 = uiButton.centerY - uiButton.height/2;
  int X2 = X1 + uiButton.width - 1;
  int Y2 = Y1 + uiButton.height - 1;
  
  //
  // check if most recent event was: this button "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawButton(uiButton, true);
    return(true);
  }
 
  //
  // most recent event didn't match anything here
  //
  return(false);
}


// ---------------------------------------------------------------------------------
//                                 Number Box functions  
// ---------------------------------------------------------------------------------

//
// draw a Number Box
//  Enter:  numberBox -> the specifications of the Number Box to draw
//
void TeensyUserInterface::drawNumberBox(NUMBER_BOX &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);

  //
  // the frame of the Number Box
  //
  int overallWidth = downButtonX + buttonWidth - upButtonX;
  int rightMostX = upButtonX + overallWidth;
  lcdDrawFilledRectangle(upButtonX,    topY,              overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY + height-3,   overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(numberX-3,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(downButtonX,  topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(rightMostX-3, topY,              3,            height, menuButtonColor);

  //
  // draw a highlight around the button making it look raised
  //
  lcdDrawLine(upButtonX-1,  topY + height-1,    upButtonX-1,   topY-1,   menuButtonFrameColor);
  lcdDrawLine(upButtonX-1,  topY-1,             rightMostX-1,  topY-1,   menuButtonFrameColor);

  //
  // draw the Up and Down buttons
  //
  drawUpButtonInNumberBox(numberBox, false);
  drawDownButtonInNumberBox(numberBox, false);

  //
  // fill in the number
  //
  drawNumberInNumberBox(numberBox);

  //
  // draw optional text above the Number Box
  //
  if (numberBox.labelText[0] != 0)
  {
    int textHeight = lcdGetFontHeightWithDecentersAndLineSpacing();
    lcdSetCursorXY(numberX + numberWidth/2, topY - ((textHeight * 16) / 10));
    lcdPrintCentered(numberBox.labelText);
  }
}



//
// check if user is touching the Number Box, pressing Up or Down
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  numberBox -> the Number Box to check
//  Exit:   true returned if the Number Box's Value changed
//
boolean TeensyUserInterface::checkForNumberBoxTouched(NUMBER_BOX &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  //
  // get the coordinates of the DOWN button
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  int X1 = downButtonX;
  int Y1 = topY;
  int X2 = downButtonX + buttonWidth - 1;
  int Y2 = topY + height - 1;
  
  //
  // Down button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBox(numberBox, true);
    updateNumberBoxNumber(numberBox, -numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Down button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    int stepSize = numberBox.stepAmount * ((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumber(numberBox, -stepSize);
    return(true);
  }
  
  //
  // Down button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBox(numberBox, false);
    return(false);
  }


  //
  // get the coordinates of the UP button
  //
  X1 = upButtonX;
  X2 = upButtonX + buttonWidth - 1;
  
  //
  // Up button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBox(numberBox, true);
    updateNumberBoxNumber(numberBox, numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Up button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    int stepSize = numberBox.stepAmount * ((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumber(numberBox, stepSize);
    return(true);
  }
  
  //
  // Up button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBox(numberBox, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// draw Down button in the number box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawDownButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "down" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;

  lcdDrawFilledRectangle(downButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = downButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX - arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX,                   arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// draw Up button in the number box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawUpButtonInNumberBox(NUMBER_BOX &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "up" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;
     
  lcdDrawFilledRectangle(upButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = upButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX,                   arrowCenterY - arrowHalfWidth,
                        arrowCenterX - arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// update the Number Box's Value
//  Enter:  numberBox -> the specifications of the Number Box
//          stepAmount = signed amount to changes the Value by
//  Exit:   true returned if the value changed, else false
//
boolean TeensyUserInterface::updateNumberBoxNumber(NUMBER_BOX &numberBox, int stepAmount)
{
  //
  // check if Adding
  //
  if (stepAmount > 0)
  {
    int newValue = numberBox.value + stepAmount;
    if (newValue > numberBox.maximumValue)
      newValue = numberBox.maximumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBox(numberBox);
    return(true);
  }

  //
  // check if Subtracting
  //
  if (stepAmount < 0)
  {
    int newValue = numberBox.value + stepAmount;
    if (newValue < numberBox.minimumValue)
      newValue = numberBox.minimumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBox(numberBox);
    return(true);
  }

  return(false);
}



//
// draw the number field in the number box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawNumberInNumberBox(NUMBER_BOX &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinates(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);

  //
  // first blank then number field
  //
  lcdSetFont(*menuButtonFont);
  lcdSetFontColor(menuButtonTextColor);
  int fontHeight = lcdGetFontHeightWithoutDecenders();
  int textY = numberBox.centerY -  fontHeight/2;
  lcdDrawFilledRectangle(numberX+3, textY, numberWidth-6, fontHeight + 1, menuBackgroundColor);

  //
  // draw the number
  //
  lcdSetCursorXY(numberX + numberWidth/2, textY);
  lcdPrintCentered(numberBox.value);
}



//
// get LCD coordinates and sizes about the given Number Box
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::getNumberBoxCoordinates(NUMBER_BOX &numberBox, int *downButtonX, int *numberX, 
  int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height)
{
  *buttonWidth = (numberBox.height * 16) / 10;
  if (*buttonWidth > (numberBox.width * 4) / 10)
    *buttonWidth = (numberBox.width * 4) / 10;
  if (*buttonWidth < 30)
    *buttonWidth = 30;

  *numberWidth = numberBox.width - (2 * *buttonWidth);
  if (*numberWidth < 30)
    *numberWidth = 30;

  *upButtonX = numberBox.centerX - numberBox.width/2;
  *numberX = *upButtonX + *buttonWidth;
  *downButtonX = *numberX + *numberWidth;

  *topY = numberBox.centerY - numberBox.height/2;
  *height = numberBox.height;
}



//
// draw a Number Box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box to draw
//
void TeensyUserInterface::drawNumberBox(NUMBER_BOX_FLOAT &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, 
    &buttonWidth, &numberWidth, &height);

  //
  // the frame of the Number Box
  //
  int overallWidth = downButtonX + buttonWidth - upButtonX;
  int rightMostX = upButtonX + overallWidth;
  lcdDrawFilledRectangle(upButtonX,    topY,              overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY + height-3,   overallWidth, 3,      menuButtonColor);
  lcdDrawFilledRectangle(upButtonX,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(numberX-3,    topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(downButtonX,  topY,              3,            height, menuButtonColor);
  lcdDrawFilledRectangle(rightMostX-3, topY,              3,            height, menuButtonColor);

  //
  // draw a highlight around the button making it look raised
  //
  lcdDrawLine(upButtonX-1,  topY + height-1,    upButtonX-1,   topY-1,   menuButtonFrameColor);
  lcdDrawLine(upButtonX-1,  topY-1,             rightMostX-1,  topY-1,   menuButtonFrameColor);

  //
  // draw the Up and Down buttons
  //
  drawUpButtonInNumberBoxFloat(numberBox, false);
  drawDownButtonInNumberBoxFloat(numberBox, false);

  //
  // fill in the number
  //
  drawNumberInNumberBoxFloat(numberBox);

  //
  // draw the text above the Number Box
  //
  if (numberBox.labelText[0] != 0)
  {
    int textHeight = lcdGetFontHeightWithDecentersAndLineSpacing();
    lcdSetCursorXY(numberX + numberWidth/2, topY - ((textHeight * 16) / 10));
    lcdPrintCentered(numberBox.labelText);
  }
}



//
// check if user is touching the Number Box, pressing Up or Down (FLOAT)
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  numberBox -> the Number Box to check
//  Exit:   true returned if the Number Box's Value changed
//
boolean TeensyUserInterface::checkForNumberBoxTouched(NUMBER_BOX_FLOAT &numberBox)
{
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;
  
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  //
  // get the coordinates of the DOWN button
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  int X1 = downButtonX;
  int Y1 = topY;
  int X2 = downButtonX + buttonWidth - 1;
  int Y2 = topY + height - 1;
  
  //
  // Down button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBoxFloat(numberBox, true);
    updateNumberBoxNumberFloat(numberBox, -numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Down button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    float stepSize = numberBox.stepAmount * (float)((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumberFloat(numberBox, -stepSize);
    return(true);
  }
  
  //
  // Down button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawDownButtonInNumberBoxFloat(numberBox, false);
    return(false);
  }


  //
  // get the coordinates of the UP button
  //
  X1 = upButtonX;
  X2 = upButtonX + buttonWidth - 1;
  
  //
  // Up button: check if justed "PUSHED"
  //
  if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBoxFloat(numberBox, true);
    updateNumberBoxNumberFloat(numberBox, numberBox.stepAmount);
    numberBoxRepeatCount = 0;
    return(true);
  }
  
  //
  // Up button: check if "REPEATING"
  //
  if (checkForTouchEventInRect(TOUCH_REPEAT_EVENT, X1, Y1, X2, Y2))
  {
    numberBoxRepeatCount++;
    float stepSize = numberBox.stepAmount * (float)((numberBoxRepeatCount / 16) + 1);
    updateNumberBoxNumberFloat(numberBox, stepSize);
    return(true);
  }
  
  //
  // Up button: check if "RELEASED"
  //
  if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT, X1, Y1, X2, Y2))
  {
    drawUpButtonInNumberBoxFloat(numberBox, false);
    return(false);
  }

  //
  // most recent event didn't match anything here
  //
  return(false);
}



//
// draw Down button in the number box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//          showButtonTouchedFlg = true if button should be highlighted
//
void TeensyUserInterface::drawDownButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "down" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;

  lcdDrawFilledRectangle(downButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = downButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX - arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY - arrowHalfWidth,
                        arrowCenterX,                   arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// draw Up button in the number box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawUpButtonInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox, boolean showButtonTouchedFlg)
{
  uint16_t buttonColor;
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);
  
  //
  // draw the "up" button
  //
  if (showButtonTouchedFlg)
    buttonColor = menuButtonSelectedColor;
  else
    buttonColor = menuButtonColor;
     
  lcdDrawFilledRectangle(upButtonX+3, topY+3, buttonWidth-6, height-6, buttonColor);
  
  int arrowCenterX = upButtonX + buttonWidth/2;
  int arrowCenterY = numberBox.centerY;
  int arrowHalfWidth = 5;
  lcdDrawFilledTriangle(arrowCenterX,                   arrowCenterY - arrowHalfWidth,
                        arrowCenterX - arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        arrowCenterX + arrowHalfWidth,  arrowCenterY + arrowHalfWidth,
                        menuButtonTextColor);
}



//
// update the Number Box's Value (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//          stepAmount = signed amount to changes the Value by
//  Exit:   true returned if the value changed, else false
//
boolean TeensyUserInterface::updateNumberBoxNumberFloat(NUMBER_BOX_FLOAT &numberBox, float stepAmount)
{
  //
  // check if Adding
  //
  if (stepAmount > 0)
  {
    float newValue = numberBox.value + stepAmount;
    if (newValue > numberBox.maximumValue)
      newValue = numberBox.maximumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBoxFloat(numberBox);
    return(true);
  }

  //
  // check if Subtracting
  //
  if (stepAmount < 0)
  {
    float newValue = numberBox.value + stepAmount;
    if (newValue < numberBox.minimumValue)
      newValue = numberBox.minimumValue;
    if (numberBox.value == newValue)
      return(false);
    numberBox.value = newValue;
    drawNumberInNumberBoxFloat(numberBox);
    return(true);
  }

  return(false);
}



//
// draw the number field in the number box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::drawNumberInNumberBoxFloat(NUMBER_BOX_FLOAT &numberBox)
{
  char stringBuffer[20];
  int downButtonX;
  int numberX;
  int upButtonX;
  int topY;
  int buttonWidth;
  int numberWidth;
  int height;

  //
  // get the coordinates of this Number Box
  //
  getNumberBoxCoordinatesFloat(numberBox, &downButtonX, &numberX, &upButtonX, &topY, &buttonWidth, &numberWidth, &height);

  //
  // first blank then number field
  //
  lcdSetFont(*menuButtonFont);
  lcdSetFontColor(menuButtonTextColor);
  int fontHeight = lcdGetFontHeightWithoutDecenders();
  int textY = numberBox.centerY -  fontHeight/2;
  lcdDrawFilledRectangle(numberX+3, textY, numberWidth-6, fontHeight + 1, menuBackgroundColor);

  //
  // draw the number
  //
  dtostrf(numberBox.value, 1, numberBox.digitsRightOfDecimal, stringBuffer);
  lcdSetCursorXY(numberX + numberWidth/2, textY);
  lcdPrintCentered(stringBuffer);
}



//
// get LCD coordinates and sizes about the given Number Box (FLOAT)
//  Enter:  numberBox -> the specifications of the Number Box
//
void TeensyUserInterface::getNumberBoxCoordinatesFloat(NUMBER_BOX_FLOAT &numberBox, int *downButtonX, int *numberX, 
  int *upButtonX, int *topY, int *buttonWidth, int *numberWidth, int *height)
{
  *buttonWidth = (numberBox.height * 16) / 10;
  if (*buttonWidth > (numberBox.width * 4) / 10)
    *buttonWidth = (numberBox.width * 4) / 10;
  if (*buttonWidth < 30)
    *buttonWidth = 30;

  *numberWidth = numberBox.width - (2 * *buttonWidth);
  if (*numberWidth < 30)
    *numberWidth = 30;

  *upButtonX = numberBox.centerX - numberBox.width/2;
  *numberX = *upButtonX + *buttonWidth;
  *downButtonX = *numberX + *numberWidth;

  *topY = numberBox.centerY - numberBox.height/2;
  *height = numberBox.height;
}


// ---------------------------------------------------------------------------------
//                               Selection Box functions  
// ---------------------------------------------------------------------------------

//
// draw a Selection Box
//  Enter:  selectionBox -> the specifications of the Selection Box to draw
//
void TeensyUserInterface::drawSelectionBox(SELECTION_BOX &selectionBox)
{
  int X, Y;
  int width, height;
  
  //
  // determine how many cells in the selection box & coordinates of the first one
  //
  int numberOfCells = countSelectionBoxChoices(selectionBox);
  getCoordsOfSelectionBoxCell(selectionBox, 0, &X, &Y, &width, &height);
  
  //
  // draw a box to contain call the cells
  //
  int overallWidth = (width * numberOfCells) + 2;
  lcdDrawRectangle(X-1,  Y-1,  overallWidth, height+2,  menuButtonColor);

  //
  // draw each cell
  //
  for (int cellNumber = 0; cellNumber < numberOfCells; cellNumber++)
    drawSelectionBoxCell(selectionBox, cellNumber, false);
  
  //
  // draw optional text above the Selection Box
  //
  if (selectionBox.labelText[0] != 0)
  {
    int textHeight = lcdGetFontHeightWithDecentersAndLineSpacing();
    lcdSetCursorXY(X-1 + overallWidth/2, Y - ((textHeight * 16) / 10));
    lcdPrintCentered(selectionBox.labelText);
  }
}



//
// check if user is touching the Selection Box, pressing one of the choices
// Note: getTouchEvents() must be called at the top of the loop that calls this function
//  Enter:  selectionBox -> the selection Box to check
//  Exit:   true returned if the Selection Box's Value changed
//
boolean TeensyUserInterface::checkForSelectionBoxTouched(SELECTION_BOX &selectionBox)
{
  int X, Y;
  int width, height;
  
  //
  // return if there is No Event
  //
  if (touchEventType == TOUCH_NO_EVENT)
    return(false);

  //
  // loop through each of the choices in the Selection BO
  //
  int numberOfCells = countSelectionBoxChoices(selectionBox);
  for (int cellNumber = 0; cellNumber < numberOfCells; cellNumber++)
  {
    //
    // check this cell to see if it matches the current event
    //
    getCoordsOfSelectionBoxCell(selectionBox, cellNumber, &X, &Y, &width, &height);
    if (checkForTouchEventInRect(TOUCH_PUSHED_EVENT,   X, Y,   X + width-1, Y + height-1))
    {
      //
      // select the touched cell and redraw all to show it highlighted
      //
      int oldSelectionValue = selectionBox.value;
      selectionBox.value = cellNumber;
      for (int cell = 0; cell < numberOfCells; cell++)
        drawSelectionBoxCell(selectionBox, cell, true);

      //
      // return "true" if the selection changed
      //
      if (oldSelectionValue != selectionBox.value)
         return(true);
      else
        return(false);
    }

    if (checkForTouchEventInRect(TOUCH_RELEASED_EVENT,   X, Y,   X + width-1, Y + height-1))
    {
      //
      // Selection Box is no longer being touched, draw all call untouched
      //
      for (int cell = 0; cell < numberOfCells; cell++)
        drawSelectionBoxCell(selectionBox, cell, false);
      return(false);
    }
  }

  //
  // no match found for the current events
  //
  return(false);
}
  


//
// draw one cell of the Selection Box
//  Enter:  selectionBox -> Selection Box to draw
//          cellNumber = cell number to draw (0 - 3)
//          showButtonTouchedFlg = true if cell should be highlighted
//
void TeensyUserInterface::drawSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, boolean showButtonTouchedFlg)
{
  int X, Y;
  int width, height;
  uint16_t cellColor;
  
  getCoordsOfSelectionBoxCell(selectionBox, cellNumber, &X, &Y, &width, &height);

  //
  // choose the color to fill the cell with
  //
  if ((cellNumber == selectionBox.value) && (showButtonTouchedFlg))
    cellColor = menuButtonSelectedColor;
  else if (cellNumber == selectionBox.value)
    cellColor = menuButtonColor;
  else
    cellColor = menuBackgroundColor;

  //
  // draw a border around the cell and fill the interior
  //
  lcdDrawRectangle(X, Y, width, height, menuButtonColor);
  lcdDrawFilledRectangle(X+1, Y+1, width-2, height-2, cellColor);

  //
  // draw the cell's text
  //
  const char *cellLabel;
  if      (cellNumber == 0) cellLabel = selectionBox.choice0Text;
  else if (cellNumber == 1) cellLabel = selectionBox.choice1Text;
  else if (cellNumber == 2) cellLabel = selectionBox.choice2Text;
  else if (cellNumber == 3) cellLabel = selectionBox.choice3Text;
  else cellLabel = "Undefined";

  lcdSetFont(*menuButtonFont);
  lcdSetFontColor(menuButtonTextColor);
  int fontHeight = lcdGetFontHeightWithoutDecenders();
  int textY = selectionBox.centerY - fontHeight/2;
  lcdSetCursorXY(X + width/2, textY);
  lcdPrintCentered(cellLabel);
}



//
// get the coordinates of one cell in the selection box
//  Enter:  selectionBox -> the Selection Box
//          cellNumber = cell number (0 - 3)
//          X, Y -> storage to return upper left corner of cell
//          width, height -> storage to return size of the cell
//
void TeensyUserInterface::getCoordsOfSelectionBoxCell(SELECTION_BOX &selectionBox, int cellNumber, int *X, 
  int *Y, int *width, int *height)
{
  int numberOfCells = countSelectionBoxChoices(selectionBox);
  int cellWidth = (selectionBox.width - 3) / numberOfCells;
  int overallWidth = cellWidth * numberOfCells;

  *Y = selectionBox.centerY - (selectionBox.height-3)/2;
  *height = selectionBox.height-3;
  *width = cellWidth;
 
  *X = (selectionBox.centerX - overallWidth/2) + (cellNumber * cellWidth);
}



//
// count the number of selection choices in a Selection Box
//  Enter:  selectionBox -> the Selection Box
//  Exit:   number of choices returned (1 - 4)
//
int TeensyUserInterface::countSelectionBoxChoices(SELECTION_BOX &selectionBox)
{
  if (selectionBox.choice1Text[0] == 0)
    return(1);

  if (selectionBox.choice2Text[0] == 0)
    return(2);

  if (selectionBox.choice3Text[0] == 0)
    return(3);

  return(4);
}


// ---------------------------------------------------------------------------------
//                                Touch screen functions  
// ---------------------------------------------------------------------------------

//
// values for: touchState
//
const byte WAITING_FOR_TOUCH_DOWN_STATE                 = 0;
const byte CONFIRM_TOUCH_DOWN_STATE                     = 1;
const byte WAITING_FOR_TOUCH_UP_STATE                   = 2;
const byte WAITING_FOR_TOUCH_UP_AFTER_AUTO_REPEAT_STATE = 3;
const byte CONFIRM_TOUCH_UP_STATE                       = 4;


//
// delay periods for dealing with touches in milliseconds
//
const long TOUCH_DEBOUNCE_PERIOD = 30;
const long TOUCH_AUTO_REPEAT_DELAY = 800;
const long TOUCH_AUTO_REPEAT_RATE = 120;


// ---------------------------------------------------------------------------------

//
// initialize the touch screen
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//
void TeensyUserInterface::touchScreenInitialize(int lcdOrientation)
{
  ts.begin();
  ts.setRotation((lcdOrientation + 2) % 4);
  setDefaultTouchScreenCalibrationConstants(lcdOrientation);
  touchState = WAITING_FOR_TOUCH_DOWN_STATE;
}



//
// check if the most recent touch event happened inside the given rectangle
//  Enter:  eventType = the type of event to look for (ie TOUCH_PUSHED_EVENT, TOUCH_RELEASED_EVENT, TOUCH_REPEAT_EVENT)
//          rectX1, rectY1 = upper left corner of the test area
//          rectX2, rectY2 = lower right corner of the test area
//  Exit:   true returned if most recent touch event matches the given parameters
//
boolean TeensyUserInterface::checkForTouchEventInRect(int eventType, int rectX1, int rectY1, int rectX2, int rectY2)
{
  //
  // check if the most recent event matches what we are looking for
  //
  if (eventType != touchEventType)
    return(false);

  //
  // check if the most recent event happened inside the rectangle we are looking for
  //
  if ((touchEventX >= rectX1) && (touchEventX <= rectX2))
  {
    if ((touchEventY >= rectY1) && (touchEventY <= rectY2))
      return(true);
  }
  return(false);
}



//
// check touch screen for new events
//  Exit:   touchEventType = touch event type, TOUCH_NO_EVENT if no event
//          touchEventX, touchEventY = LCD coordinates of touch event 
//
void TeensyUserInterface::getTouchEvents(void)
{
  boolean currentlyTouched;
  int currentTouchX;
  int currentTouchY;
  unsigned long currentTime = millis();
  static unsigned long touchEventStartTime;
  static int recordedTouchX;
  static int recordedTouchY;

  touchEventType = TOUCH_NO_EVENT;                          // assume there will be no touch event

  //
  // check if anything is touched now
  //
  currentlyTouched = getTouchScreenCoords(&currentTouchX, &currentTouchY);

  //
  // select the current touch state
  //
  switch(touchState)
  {
    //
    // check if waiting for the screen to be touched
    //
    case WAITING_FOR_TOUCH_DOWN_STATE:
    {
      if (currentlyTouched)
      {
         touchState = CONFIRM_TOUCH_DOWN_STATE;             // screen is touched, start timer to confirm touch
         touchEventStartTime = currentTime;
      }
      return;
     } 


    //
    // we think the screen has a new touch, let's double check
    //
    case CONFIRM_TOUCH_DOWN_STATE:
    {
      if (currentTime < (touchEventStartTime + TOUCH_DEBOUNCE_PERIOD))
        return;                                             // wait until debounce period complete
  
      //
      // check if the screen didn't stay touched
      //
      if (!currentlyTouched)
      {
        touchState = WAITING_FOR_TOUCH_DOWN_STATE;          // false alarm, don't report the touch
        return;
      }

      //
      // the screen has been touched
      //
      recordedTouchX = currentTouchX;                       // save coordinates where the touch occurred
      recordedTouchY = currentTouchY;
      
      touchEventStartTime = currentTime;                    // start auto repeat timer
      touchState = WAITING_FOR_TOUCH_UP_STATE;  

      touchEventX = recordedTouchX;                         // return "screen has been pressed" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_PUSHED_EVENT; 
      return;
    }


    //
    // check if waiting for the touch to be released
    //      
    case WAITING_FOR_TOUCH_UP_STATE:
    {
      //
      // screen has been continuously touched, check if no longer touched
      //
      if (!currentlyTouched)
      {
        touchState = CONFIRM_TOUCH_UP_STATE;                // not touched now, go debounce the released
        return;
      }

      //
      // still touched, check if time to auto repeat
      //
      if (currentTime < (touchEventStartTime + TOUCH_AUTO_REPEAT_DELAY))
        return;                                             // no, return no event
                                           
      touchEventStartTime = currentTime;                    // yes auto repeat, reset auto repeat timer
      touchState = WAITING_FOR_TOUCH_UP_AFTER_AUTO_REPEAT_STATE;

      touchEventX = recordedTouchX;                         // return "touch is auto repeating" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_REPEAT_EVENT;
      return; 
    }
     

    //
    // check if auto repeat has started, waiting more repeats
    //
    case WAITING_FOR_TOUCH_UP_AFTER_AUTO_REPEAT_STATE:
    {
      //
      // screen has been continuously touched & auto repeat started, check if still touched
      //
      if (!currentlyTouched)
      {
        touchState = CONFIRM_TOUCH_UP_STATE;                // not touched now, wait to debounce
        return;
      }
 
      //
      // still touched, check if time to auto repeat
      //
      if (currentTime < (touchEventStartTime + TOUCH_AUTO_REPEAT_RATE))
        return;                        // no, return no event
                                           
      touchEventStartTime = currentTime;                    // yes, reset auto repeat timer

      touchEventX = recordedTouchX;                         // return "auto repeat" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_REPEAT_EVENT;
      return;
    }
  

    //
    // touch has been released, verify that it stays released for a period of time
    //
    case CONFIRM_TOUCH_UP_STATE:
    {
      //
      // screen should not be touched, reset debounce time if it is
      //
      if (currentlyTouched)
      {
        touchEventStartTime = currentTime;                  // touched again, reset timer & return no event
        return;
      }
      
      //
      // check if debounce period has elapsed
      //
      if (currentTime < (touchEventStartTime + TOUCH_DEBOUNCE_PERIOD))
        return;                                             // delay period not up, return no event
  
      //
      // touch has been released now
      //
      touchState = WAITING_FOR_TOUCH_DOWN_STATE;            // the touch cycle of events is now complete

      touchEventX = recordedTouchX;                         // return touch "Released" event
      touchEventY = recordedTouchY;
      touchEventType = TOUCH_RELEASED_EVENT;
      return;
    }
  }
}



//
// set default calibration constants for converting to LCD coordinates
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//
void TeensyUserInterface::setDefaultTouchScreenCalibrationConstants(int lcdOrientation)
{
  switch(lcdOrientation)
  {
    case LCD_ORIENTATION_PORTRAIT_4PIN_TOP:
    {
      setTouchScreenCalibrationConstants(20 - 4, 14.90,    17, 11.07);
      break;
    }
    
    case LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT:
    {
      setTouchScreenCalibrationConstants(17, 11.07,    20, 14.90);
      break;
    }
    
    case LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM:
    {
     setTouchScreenCalibrationConstants(20, 14.90,     35, 11.07);
     break;
    }
    
    case LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT:
    default:
    {
      setTouchScreenCalibrationConstants(35, 11.06,    19, 14.84);
      break;
    }
  }
}



//
// set the touch screen calibration constants used for converting from
// touch coordinates to LCD coordinates
//  Enter:  tsToLCDOffsetX = touch screen X offset calibration constant
//          tsToLCDScalerX = touch screen X scaler calibration constant
//          tsToLCDOffsetY = touch screen Y offset calibration constant
//          tsToLCDScalerY = touch screen Y scaler calibration constant
//
void TeensyUserInterface::setTouchScreenCalibrationConstants(int tsToLCDOffsetX, 
  float tsToLCDScalerX, int tsToLCDOffsetY, float tsToLCDScalerY)
{
  touchScreenToLCDOffsetX = tsToLCDOffsetX;
  touchScreenToLCDScalerX = tsToLCDScalerX;
  touchScreenToLCDOffsetY = tsToLCDOffsetY;
  touchScreenToLCDScalerY = tsToLCDScalerY;
}



//
// get the XY values of where to touch screen is being touched (in LCD space)
//  Enter:  xLCD, yLCD -> storage to return X and Y coordinates
//  Exit:   true returned if touch screen is currently being touch, else false
//
boolean TeensyUserInterface::getTouchScreenCoords(int *xLCD, int *yLCD)
{
  int xRaw;
  int yRaw;

  //
  // check if the screen is currently being touched
  //
  if (getRAWTouchScreenCoords(&xRaw, &yRaw) == false)
    return(false);

  //
  // convert the coordinates into LCD space
  //
  int x = (int)((float)xRaw / touchScreenToLCDScalerX) - touchScreenToLCDOffsetX;
  *xLCD = constrain(x, 0, lcdWidth - 1);

  int y = (int)((float)yRaw / touchScreenToLCDScalerY) - touchScreenToLCDOffsetY;
  *yLCD = constrain(y, 0, lcdHeight - 1);

  return(true);
}



//
// get the raw XY values of where to touch screen is being touched (in touch screen space)
//  Enter:  xRaw, yRaw -> storage to return X and Y raw coordinates
//  Exit:   true returned if touch screen is currently being touch, else false
//
boolean TeensyUserInterface::getRAWTouchScreenCoords(int *xRaw, int *yRaw)
{
  //
  // check if the screen is currently being touched
  //
  if (ts.touched() == false)
    return(false);

  //
  // get the raw coordinates and return them
  //
  TS_Point rawTouchPoint = ts.getPoint();

  *xRaw = rawTouchPoint.x;
  *yRaw = rawTouchPoint.y;
  return(true);
}


// ---------------------------------------------------------------------------------
//                                    LCD functions  
// ---------------------------------------------------------------------------------

//
// initialize the LCD display
//  Enter:  lcdOrientation = LCD_ORIENTATION_PORTRAIT_4PIN_TOP, LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT
//                           LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT
//          font -> the font typeface to load, ei: Arial_10
//
void TeensyUserInterface::lcdInitialize(int lcdOrientation, const ui_font &font)
{
  lcd.begin();
  lcd.setRotation(lcdOrientation);
  lcdClearScreen(LCD_BLACK);
  lcdSetFontColor(LCD_WHITE);  
  lcdSetFont(font);
  lcdSetCursorXY(0, 0);

  lcdWidth = lcd.width();
  lcdHeight = lcd.height();
}



//
// fill the entire lcd screen with the given color
//  Enter:  color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdClearScreen(uint16_t color)
{
  lcd.fillScreen(color);
}



//
// draw one pixel with the given coords and color
//  Enter:  x, y = coords of the pixel to draw
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawPixel(int x, int y, uint16_t color)
{
  lcd.drawPixel(x, y, color);
}



//
// draw a line with the given coords and color
//  Enter:  x1, y1 = first endpoint of line
//          x2, y2 = second endpoint of line
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawLine(int x1, int y1, int x2, int y2, uint16_t color)
{
  lcd.drawLine(x1, y1, x2, y2, color);
}



//
// draw a horizontal line with the given coords, length and color
//  Enter:  x, y = first endpoint of line
//          length = length of line
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawHorizontalLine(int x, int y, int length, uint16_t color)
{
  lcd.drawFastHLine(x, y, length, color);
}



//
// draw a vertical line with the given coords, length and color
//  Enter: x, y = first endpoint of line
//          length = length of line
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawVerticalLine(int x, int y, int length, uint16_t color)
{
  lcd.drawFastVLine(x, y, length, color);
}



//
// draw rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawRectangle(int x, int y, int width, int height, uint16_t color)
{
  lcd.drawRect(x, y, width, height, color);
}



//
// draw rounded rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          radius = radius of the corners
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color)
{
  lcd.drawRoundRect(x, y, width, height, radius, color);
}



//
// draw a circle at the given coords, radius and color
//  Enter:  x0, y0 = endpoint 0 of the triangle
//          x1, y1 = endpoint 1 of the triangle
//          x2, y2 = endpoint 2 of the triangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{
  lcd.drawTriangle(x0, y0, x1, y1, x2, y2, color);
}



//
// draw a triangle at the given coords and color
//  Enter:  x, y = upper left corner of rect
//          radius = radius of the circle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawCircle(int x, int y, int radius, uint16_t color)
{
  lcd.drawCircle(x, y, radius, color);
}



//
// draw a filled rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledRectangle(int x, int y, int width, int height, uint16_t color)
{
  lcd.fillRect(x, y, width, height, color);
}



//
// draw a filled rounded rectangle at the given coords, length, width and color
//  Enter:  x, y = upper left corner of rect
//          width = width of rectangle
//          height = height of rectangle
//          radius = radius of the corners
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledRoundedRectangle(int x, int y, int width, int height, int radius, uint16_t color)
{
  lcd.fillRoundRect(x, y, width, height, radius, color);
}



//
// draw a circle at the given coords, radius and color
//  Enter:  x0, y0 = endpoint 0 of the triangle
//          x1, y1 = endpoint 1 of the triangle
//          x2, y2 = endpoint 2 of the triangle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{
  lcd.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}



//
// draw a filled circle at the given coords, radius and color
//  Enter:  x, y = upper left corner of rect
//          radius = radius of the circle
//          color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdDrawFilledCircle(int x, int y, int radius, uint16_t color)
{
  lcd.fillCircle(x, y, radius, color);
}



//
// set the text font for the "print" functions
//  Enter:  font -> the font typeface to load
//          ie: Arial_8, Arial_8_Bold, Arial_9, Arial_9_Bold, Arial_10, Arial_10_Bold, Arial_11, Arial_11_Bold,
//              Arial_12, 13, 14, 16, 18, 20, 24, 28, 32, 40, 48, 60, 72, 96
//
void TeensyUserInterface::lcdSetFont(const ui_font &font)
{
  lcd.setFont(font);
  currentFont = &font;
}



//
// set the foreground color for the "print" functions
//  Enter:  color = 16 bit color, bit format: rrrrrggggggbbbbb
//
void TeensyUserInterface::lcdSetFontColor(uint16_t color)
{
  lcd.setTextColor(color);
}



//
// print a string to the LCD display
//  Enter:  s -> a null terminated string 
//
void TeensyUserInterface::lcdPrint(char *s)
{
  lcd.print(s);
}

void TeensyUserInterface::lcdPrint(const char *s)
{
  lcd.print(s);
}



//
// print a signed int at location of the cursor
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrint(int n)
{
  char stringBuffer[14];
  
  itoa(n, stringBuffer, 10);
  lcdPrint(stringBuffer);
}



//
// print a float or double at location of the cursor
//  Enter:  n = signed number to print 
//          digitsRightOfDecimal = number of digits to display right of decimal point (optional)
//
void TeensyUserInterface::lcdPrint(double n, int digitsRightOfDecimal)
{
  char stringBuffer[40];

  dtostrf(n, 1, digitsRightOfDecimal, stringBuffer);
  lcdPrint(stringBuffer);
}



//
// print a string to the LCD, right justified at the cursor
//  Enter:  s -> string to print 
//
void TeensyUserInterface::lcdPrintRightJustified(char *s)
{
  int cursorX;
  int cursorY;

  lcdGetCursorXY(&cursorX, &cursorY);           // get the current cursor position
  int stringWidth = lcdStringWidthInPixels(s);  // get the width of the string to print
  cursorX = cursorX - stringWidth;              // determine new X coord for cursor to right justify

  if (cursorX < 0)                              // set the new cursor position
    cursorX = 0;
  lcdSetCursorXY(cursorX, cursorY);
  
  lcdPrint(s);                                  // print the string
}

void TeensyUserInterface::lcdPrintRightJustified(const char *s)
{
  lcdPrintRightJustified((char *)s);
}



//
// print a signed int on the LCD, right justify at the cursor 
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrintRightJustified(int n)
{
  char stringBuffer[14];
  
  itoa(n, stringBuffer, 10);
  lcdPrintRightJustified(stringBuffer);
}



//
// print a float on the LCD, right justify at the cursor 
//  Enter:  n = signed number to print 
//          digitsRightOfDecimal = number of digits to display right of decimal point (optional)
//
void TeensyUserInterface::lcdPrintRightJustified(double n, int digitsRightOfDecimal)
{
  char stringBuffer[40];

  dtostrf(n, 1, digitsRightOfDecimal, stringBuffer);
  lcdPrintRightJustified(stringBuffer);
}



//
// print a string to the LCD, centered side-to-side at the cursor 
//  Enter:  s -> string to print 
//
void TeensyUserInterface::lcdPrintCentered(char *s)
{
  int cursorX;
  int cursorY;

  lcdGetCursorXY(&cursorX, &cursorY);           // get the current cursor position
  int stringWidth = lcdStringWidthInPixels(s);  // get the width of the string to print
  cursorX = cursorX - stringWidth/2;            // determine new X coord for cursor to center

  if (cursorX < 0)                              // set the new cursor position
    cursorX = 0;
  lcdSetCursorXY(cursorX, cursorY);
  
  lcdPrint(s);                                  // print the string
}

void TeensyUserInterface::lcdPrintCentered(const char *s)
{
  lcdPrintCentered((char *) s);
}



//
// print a signed int to the LCD, centered side-to-side at the cursor
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrintCentered(int n)
{
  char stringBuffer[14];
  
  itoa(n, stringBuffer, 10);
  lcdPrintCentered(stringBuffer);
}



//
// print a float to the LCD, centered side-to-side at the cursor
//          digitsRightOfDecimal = number of digits to display right of decimal point (optional)
//  Enter:  n = signed number to print 
//
void TeensyUserInterface::lcdPrintCentered(double n, int digitsRightOfDecimal)
{
  char stringBuffer[40];

  dtostrf(n, 1, digitsRightOfDecimal, stringBuffer);
  lcdPrintCentered(stringBuffer);
}



//
// print one ASCII charater to the LCD, at location of the cursor
//  Enter:  c = character to display
//
void TeensyUserInterface::lcdPrintCharacter(byte character)
{
  lcd.drawFontChar(character);
}



//
// get the width of a string in pixels
//
int TeensyUserInterface::lcdStringWidthInPixels(char *s)
{
  return(lcd.strPixelLen(s));
}

int TeensyUserInterface::lcdStringWidthInPixels(const char *s)
{
  return(lcd.strPixelLen((char *) s));
}



//
// get the height of the selected font in pixels, excluding decenders
//
int TeensyUserInterface::lcdGetFontHeightWithoutDecenders(void)
{
  return(currentFont->cap_height);
}



//
// get the height of the selected font in pixels, including decenders & line spacing
//
int TeensyUserInterface::lcdGetFontHeightWithDecentersAndLineSpacing(void)
{
  return(currentFont->line_space);
}



//
// set the cursor coords in pixels
//  Enter:  x  0 = left most pixel
//          y  0 = left most pixel
//
void TeensyUserInterface::lcdSetCursorXY(int x, int y)
{
  if ((x < 0) || (x >= lcdWidth)) return;
  if ((y < 0) || (y >= lcdHeight)) return;
  
  lcd.setCursor(x, y);
}



//
// get the cursor coords in pixels
//  Enter:  x -> storage to return X,  0 = left most pixel
//          y -> storage to return Y,  0 = left most pixel
//
void TeensyUserInterface::lcdGetCursorXY(int *x, int *y)
{
  int16_t x1;
  int16_t y1;

  lcd.getCursor(&x1, &y1);
  *x = x1;
  *y = y1;
}



//
// make a RGB565 color
//  Enter:  red (0 to 31)
//          green (0 to 63) Note: green value 0x20 has same intensity as blue value 0x10
//          blue (0 to 31)
//  Exit:   16 bit color returned, bit format: rrrrrggggggbbbbb
//
uint16_t TeensyUserInterface::lcdMakeColor(int red, int green, int blue)
{
  return((red << 11) + (green << 5) + (blue));
}


// ---------------------------------------------------------------------------------
//                                   EEPROM functions
// ---------------------------------------------------------------------------------


//
// write a configuration byte (8 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 8 bit value to write to EEPROM
//          note: 2 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationByte(int EEPromAddress, byte value)
{
  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);
	
  EEPROM.write(EEPromAddress + 1, value);
}



//
// read a configuration byte (8 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 2 bytes of EEPROM space are used 
//  Exit:   byte value from EEPROM (or default value) returned
//
byte TeensyUserInterface::readConfigurationByte(int EEPromAddress, byte defaultValue)
{
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);
	
    return(EEPROM.read(EEPromAddress + 1));
}



//
// write a configuration short (16 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 16 bit value to write to EEPROM
//          note: 3 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationShort(int EEPromAddress, short value)
{
  byte *dataPntr;

  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);

	dataPntr = (byte*) (&value);
  
  EEPROM.write(EEPromAddress + 1, dataPntr[0]);
  EEPROM.write(EEPromAddress + 2, dataPntr[1]);
}



//
// read a configuration short (16 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 3 bytes of EEPROM space are used 
//  Exit:   short value from EEPROM (or default value) returned
//
short TeensyUserInterface::readConfigurationShort(int EEPromAddress, short defaultValue)
{
  short value;
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);

  dataPntr = (byte*) (&value);

  dataPntr[0] = EEPROM.read(EEPromAddress + 1);
  dataPntr[1] = EEPROM.read(EEPromAddress + 2);  
  return(value);
}



//
// write a configuration int (32 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 32 bit value to write to EEPROM
//          note: 5 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationInt(int EEPromAddress, int value)
{
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);

  dataPntr = (byte*) (&value);
  
  EEPROM.write(EEPromAddress + 1, dataPntr[0]);
  EEPROM.write(EEPromAddress + 2, dataPntr[1]);
  EEPROM.write(EEPromAddress + 3, dataPntr[2]);
  EEPROM.write(EEPromAddress + 4, dataPntr[3]);
}



//
// read a configuration int (32 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 5 bytes of EEPROM space are used 
//  Exit:   long value from EEPROM (or default value) returned
//
int TeensyUserInterface::readConfigurationInt(int EEPromAddress, int defaultValue)
{
  int value;
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);

  dataPntr = (byte*) (&value);

  dataPntr[0] = EEPROM.read(EEPromAddress + 1);
  dataPntr[1] = EEPROM.read(EEPromAddress + 2);
  dataPntr[2] = EEPROM.read(EEPromAddress + 3);
  dataPntr[3] = EEPROM.read(EEPromAddress + 4);
  return(value);
}



//
// write a configuration float (32 bit) to the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to write 
//          value = 32 bit float to write to EEPROM
//          note: 5 bytes of EEPROM space are used 
//
void TeensyUserInterface::writeConfigurationFloat(int EEPromAddress, float value)
{
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    EEPROM.write(EEPromAddress, 0);

  dataPntr = (byte*) (&value);
  
  EEPROM.write(EEPromAddress + 1, dataPntr[0]);
  EEPROM.write(EEPromAddress + 2, dataPntr[1]);
  EEPROM.write(EEPromAddress + 3, dataPntr[2]);
  EEPROM.write(EEPromAddress + 4, dataPntr[3]);
}



//
// read a configuration float (32 bit) from the EEPROM
//  Enter:  EEPromAddress = address in EEPROM to read from 
//          defaultValue = default value to return if value has never been 
//            written to the EEPROM
//          note: 5 bytes of EEPROM space are used 
//  Exit:   float value from EEPROM (or default value) returned
//
float TeensyUserInterface::readConfigurationFloat(int EEPromAddress, float defaultValue)
{
  float value;
  byte *dataPntr;
  
  if (EEPROM.read(EEPromAddress) == 0xff)
    return(defaultValue);

  dataPntr = (byte*) (&value);

  dataPntr[0] = EEPROM.read(EEPromAddress + 1);
  dataPntr[1] = EEPROM.read(EEPromAddress + 2);
  dataPntr[2] = EEPROM.read(EEPromAddress + 3);
  dataPntr[3] = EEPROM.read(EEPromAddress + 4);
  return(value);
}


// -------------------------------------- End --------------------------------------
