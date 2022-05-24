
//      ******************************************************************
//      *                                                                *
//      *    Example shows how to create a menu with buttons arranged    *
//      *                           in columns                           *
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
}


// ---------------------------------------------------------------------------------
//           Define the menus and top level loop, place menus after setup()
// ---------------------------------------------------------------------------------

//
// Notes on building the menu table:
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
// So far all of our examples have placed the menu buttons in a single column.  In this
// example we will use two columns.


//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];


//
// the main menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,  "Example Three - Menus with Columns",   MENU_COLUMNS_2,     mainMenu},
  {MENU_ITEM_TYPE_COMMAND,           "Count to one",                         commandCountTo1,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to two",                         commandCountTo2,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to three",                       commandCountTo3,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to four",                        commandCountTo4,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to five",                        commandCountTo5,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to six",                         commandCountTo6,    NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,       "",                                     NULL,               NULL}
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
// menu command to "Count To 4"
//
void commandCountTo4(void)
{  
  ui.drawTitleBar("Count To Four");
  showCount(4);
}


//
// menu command to "Count To 5"
//
void commandCountTo5(void)
{  
  ui.drawTitleBar("Count To Five");
  showCount(5);
}


//
// menu command to "Count To 6"
//
void commandCountTo6(void)
{  
  ui.drawTitleBar("Count To Six");
  showCount(6);
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
    delay(350);
  }
}
