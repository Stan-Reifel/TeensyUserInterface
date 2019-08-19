
//      ******************************************************************
//      *                                                                *
//      *          Example to shows how to create a simple menu          *
//      *                                                                *
//      *            S. Reifel & Co.                8/8/2019             *
//      *                                                                *
//      ******************************************************************

// This sketch shows how to use the "Teensy User Interface" to create a simple menu
// with 3 commands.  Below are the hardware and software requirements needed to make
// this example work.
//
//
// HARDWARE:
// 
// This user interface works with a Teensy microcontroller (www.pjrc.com) and a 2.8" 
// 320x240 LCD touch screen display (the LCD must include a ILI9341 controller).  
// These displays can be purchased from PJRC:
//                https://www.pjrc.com/store/display_ili9341_touch.html
// or on eBay and Amazon.  
//
// To hookup the LCD to the Teensy, see:
// https://github.com/Stan-Reifel/TeensyUserInterface/blob/master/images/HookupGuide.png
//
//
// LIBRARIES:
//
// The TeensyUserInterface requires three libraries, all of which must be installed into
// the Arduino IDE.  To do so:  1) From the "Sketch" menu, select "Include Library", 
// then "Manage libraries".  2) In the "Filter your search" field, enter the library
// name as shown below.  3) Click "Install".
// 
//    TeensyUserInterface  - The Teensy User Interface
//    ILI9341_t3           - Driver for the LCD screen
//    XPT2046_Touchscreen  - Driver for the touch screen
//
// The author has only tested the TeensyUserInterface code with a Teensy 3.6 but it 
// will likely work with other Teensys that support these libraries.
// 
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
  // setup the LCD orientation, the default font and initialize the user interface
  //
  ui.begin(LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT, Arial_9_Bold);
}


// ---------------------------------------------------------------------------------
//           Define the menus and top level loop, place menus after setup()
// ---------------------------------------------------------------------------------

//
// Notes on building the menu table:
//
// The first line in the table always defines what type of menu it is, either a 
// Main Menu, or a Sub Menu.  The table's last line marks the end of the table.  
// In between are menu items, each line represents one menu choice the user will
// see displayed.
//
// There are three types of menu items: Commands, Toggles, and Sub Menus. In this
// sketch we are only going to explore "Commands".
//
// A MENU_ITEM_TYPE_COMMAND line in the table adds a button to the menu.  When 
// pressed, the specified function is executed.  In the second field is the text 
// displayed on the button.  A pointer to the function is entered in the third 
// field.  The last field should always be NULL.


//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];


//
// the main menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,   "Example One - Simple Menu",   MENU_COLUMNS_1,            mainMenu},
  {MENU_ITEM_TYPE_COMMAND,            "Count to ten",                commandCountToTen,         NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Gettsburg address",           commandGettsburgAddress,   NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Draw some shapes",            commandDrawShapes,         NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",                            NULL,                      NULL}
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
// menu command to "Count To Ten"
//
void commandCountToTen(void)
{  
  //
  // draw title bar showing at the top of the screen
  //
  ui.drawTitleBar("Count To Ten");

  //
  // count to ten, showing the digits on the screen, when done return to the menu
  //
  for(int i = 1; i <= 10; i++)
  {
    //
    // blank the screen below the Title Bar
    //
    ui.clearDisplaySpace();
    
    //
    // show the count number on the LCD screen
    //
    ui.lcdSetCursorXY(160, 130);
    ui.lcdPrintCentered(i);

    //
    // delay for a half second while showing the count value
    //
    delay(500);
  }
}



//
// menu command to "Show the Gettysburg Address"
//
void commandGettsburgAddress(void)
{  
  //
  // draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Gettsburg Address");

  //
  // blank the screen below the Title Bar
  //
  ui.clearDisplaySpace();
  
  //
  // print some text in the LCD's display space
  //
  ui.lcdSetCursorXY(160, 100);
  ui.lcdPrintCentered("Four score and seven years ago our fathers");
  ui.lcdSetCursorXY(160, 120);
  ui.lcdPrintCentered("brought forth on this continent, a new nation,");
  ui.lcdSetCursorXY(160, 140);
  ui.lcdPrintCentered("conceived in Liberty, and dedicated to the");
  ui.lcdSetCursorXY(160, 160);
  ui.lcdPrintCentered("proposition that all men are created equal.");


  //
  // wait for the user presses the "Back" button, then return to the main menu
  //
  while(true)
  {
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}



//
// menu command to "Draw shapes"
//
void commandDrawShapes(void)
{  
  //
  // draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Draw Shapes");

  //
  // blank the screen below the Title Bar
  //
  ui.clearDisplaySpace();
  
  //
  // print some random shapes in the LCD's display space
  //
  int radius = random(20, 50);
  int x = random(55, 260);
  int y = random(100, 140);
  uint16_t color = LCD_GREEN;
  ui.lcdDrawFilledCircle(x, y, radius, color);

  x = random(55, 260);
  y = random(100, 140);
  int w = random(20, 50);
  color = LCD_RED;
  ui.lcdDrawFilledTriangle(x, y,   x-w/2, y+w,   x+w/2, y+w,  color);

  x = random(55, 260);
  y = random(100, 140);
  w = random(20, 50);
  int h = random(20, 50);
  color = LCD_YELLOW;
  ui.lcdDrawFilledRectangle(x, y, w, h, color);


  //
  // wait for the user presses the "Back" button, then return to the main menu
  //
  while(true)
  {
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}
