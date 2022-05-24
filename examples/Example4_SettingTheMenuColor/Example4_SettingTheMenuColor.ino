
//      ******************************************************************
//      *                                                                *
//      *       Example shows how to set the colors and font used        *
//      *                          by the menus                          *
//      *                                                                *
//      *            S. Reifel & Co.                2/18/2022            *
//      *                                                                *
//      ******************************************************************

//
// DOCUMENTATION:
//
// Documentation for the "Teensy User Interface" library can be found at:
//    https://github.com/Stan-Reifel/TeensyUserInterface



// ***********************************************************************

#include <TeensyUserInterface.h>
#include <font_Arial.h>
#include <font_ArialBold.h>


//
// create the user interface object
//
TeensyUserInterface ui;



// ---------------------------------------------------------------------------------
//                                 Setup the hardware
// ---------------------------------------------------------------------------------

void setup() 
{
  //
  // pin numbers used in addition to the default SPI pins
  //
  const int LCD_CS_PIN = 10;
  const int LCD_DC_PIN = 9;
  const int TOUCH_CS_PIN = 8;
  
  //
  // setup the LCD orientation, the default font and initialize the user interface
  //
  ui.begin(LCD_CS_PIN, LCD_DC_PIN, TOUCH_CS_PIN, LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT, Arial_9_Bold);


  //
  // By default the menus use a blue color palette, however you can set the colors
  // to match your preference.  In this example we will build RGB color values for
  // each of the menu's attributes.
  //
  // The TeensyUserInterface has two built in color palettes, the default blue palette,
  // and a grayscale palette.  The author prefers the gray palette and it will be used
  // in future examples.  Selecting the gray palette is done with a single call to this 
  // function:
  //                            ui.setColorPaletteGray();
  //
  // Note about colors:  This LCD display uses a 16 bit color format.  The format is
  // referred to as RGB565, meaning 5 bits of red, 6 bits of green, and 5 bits of blue.
  // The function lcdMakeColor() can be used to build a color in the RGB565 format.  
  // Call it with a red value between 0-31, green between 0-63, and a blue value
  // between 0-31.

  //
  // build a red color palette
  //
  int16_t red;
  uint16_t green;
  uint16_t blue;
  
  red = 20; green = 0; blue = 0;      uint16_t titleBarColor                   = ui.lcdMakeColor(red, green, blue);
  red = 25; green = 4; blue = 2;      uint16_t titleBarBackButtonColor         = ui.lcdMakeColor(red, green, blue);
  red = 30; green = 6; blue = 3;      uint16_t titleBarBackButtonSelectedColor = ui.lcdMakeColor(red, green, blue);
                                      uint16_t titleBarTextColor               = LCD_WHITE;
                                      uint16_t menuBackgroundColor             = LCD_BLACK;
  red = 20; green = 0; blue = 0;      uint16_t menuButtonColor                 = ui.lcdMakeColor(red, green, blue);
                                      uint16_t menuButtonSelectedColor         = titleBarBackButtonSelectedColor;
  red = 22; green = 0; blue = 0;      uint16_t menuButtonFrameColor            = ui.lcdMakeColor(red, green, blue);
                                      uint16_t menuButtonTextColor             = LCD_WHITE;

  ui.setTitleBarColors(titleBarColor, titleBarTextColor, titleBarBackButtonColor, titleBarBackButtonSelectedColor);
  ui.setMenuColors(menuBackgroundColor, menuButtonColor, menuButtonSelectedColor, menuButtonFrameColor, menuButtonTextColor);


  //
  // set the menus to use a larger than the default font
  //
  ui.setTitleBarFont(Arial_13_Bold);
  ui.setMenuFont(Arial_13_Bold);
}


// ---------------------------------------------------------------------------------
//           Define the menus and top level loop, place menus after setup()
// ---------------------------------------------------------------------------------


//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];


//
// the main menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,  "Example Four - Setting Colors",            MENU_COLUMNS_1,     mainMenu},
  {MENU_ITEM_TYPE_COMMAND,           "Count to one",                             commandCountTo1,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to two",                             commandCountTo2,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to three",                           commandCountTo3,    NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,       "",                                         NULL,               NULL}
};



//
// display the menu, then execute commands selected by the user
//
void loop() 
{  
  ui.displayAndExecuteMenu(mainMenu);
}



// ---------------------------------------------------------------------------------
//                            Commands executed from the menu
// ---------------------------------------------------------------------------------

//
// menu command to "Count To 1"
//
void commandCountTo1(void)
{  
  ui.drawTitleBar("Count To One");
  showCount(1);
}


//
// menu command to "Count To 2"
//
void commandCountTo2(void)
{  
  ui.drawTitleBar("Count To Two");
  showCount(2);
}


//
// menu command to "Count To 3"
//
void commandCountTo3(void)
{  
  ui.drawTitleBar("Count To Three");
  showCount(3);
}



//
// display a screen that counts
//
void showCount(int countValue)
{
  //
  // draw the digits using a large font
  //
  ui.lcdSetFont(Arial_60_Bold);

  
  //
  // count to ten, showing the digits on the screen, when done return to the menu
  //
  for(int i = 1; i <= countValue; i++)
  {
    //
    // blank the screen below the Title Bar
    //
    ui.clearDisplaySpace();
    
    //
    // show the count number on the LCD screen
    //
    ui.lcdSetCursorXY(ui.displaySpaceCenterX, ui.displaySpaceCenterY - 30);
    ui.lcdPrintCentered(i);

    //
    // delay for a quarter second while showing the count value
    //
    delay(500);
  }
}
