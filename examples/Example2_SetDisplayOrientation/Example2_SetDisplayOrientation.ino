
//      ******************************************************************
//      *                                                                *
//      *     Example to shows how to set the display's orientation      *
//      *             choose between Portrait and Landscape              *
//      *                                                                *
//      *            S. Reifel & Co.                8/8/2019             *
//      *                                                                *
//      ******************************************************************

// These are the hardware and software requirements needed to make this example work:
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
  // this is where you set the display's orientation, choosing between Portrait 
  // and Landscape modes by uncommenting the one you want
  //
  ui.begin(LCD_ORIENTATION_PORTRAIT_4PIN_TOP, Arial_9_Bold);
//  ui.begin(LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT, Arial_9_Bold);
//  ui.begin(LCD_ORIENTATION_PORTRAIT_4PIN_BOTTOM, Arial_9_Bold);
//  ui.begin(LCD_ORIENTATION_LANDSCAPE_4PIN_RIGHT, Arial_9_Bold);
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
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,   "Example Two - Portrait Mode",   MENU_COLUMNS_1,               mainMenu},
  {MENU_ITEM_TYPE_COMMAND,            "Count to ten",                  commandCountToTen,            NULL},
  {MENU_ITEM_TYPE_COMMAND,            "First law of robotics",         commandFirstLawOfRobotics,    NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Draw one hundred dots",         commandDraw100Dots,           NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",                              NULL,                         NULL}
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
  // draw the digits using a larger font
  //
  ui.lcdSetFont(Arial_60_Bold);

  
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
    ui.lcdSetCursorXY(ui.displaySpaceCenterX, ui.displaySpaceCenterY - 30);
    ui.lcdPrintCentered(i);

    //
    // delay for a quarter second while showing the count value
    //
    delay(250);
  }
}



//
// menu command to "First Law Of Robotics"
//
void commandFirstLawOfRobotics(void)
{  
  //
  // draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("First Law of Robotics");

  //
  // blank the screen below the Title Bar
  //
  ui.clearDisplaySpace();

  //
  // set the font for drawing the text
  //
  ui.lcdSetFont(Arial_12);
  
  //
  // print some text in the LCD's display space
  //
  int y = ui.displaySpaceCenterY - (2 * 18);
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("A robot may not injure a");

  y += 18;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("human being or, through");

  y += 18;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered(" inaction, allow a human");
  
  y += 18;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("being to come to harm.");


  //
  // wait for the user to press the "Back" button, then return to the main menu
  //
  while(true)
  {
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}



//
// menu command to "Draw dots"
//
void commandDraw100Dots(void)
{  
  //
  // draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("Draw Dots");

  //
  // blank the screen below the Title Bar
  //
  ui.clearDisplaySpace();
  
  //
  // draw 100 random circles
  //
  for (int i = 0; i < 100; i++)
  {
    int radius = 20;
    int x = random(25, ui.displaySpaceWidth - 25);
    int y = random(25, ui.displaySpaceHeight - 25);
    uint16_t color = random(0, 0xffff);
    ui.lcdDrawFilledCircle(ui.displaySpaceLeftX + x, ui.displaySpaceTopY + y, radius, color);
  }

  //
  // wait for the user to press the "Back" button, then return to the main menu
  //
  while(true)
  {
    ui.getTouchEvents();

    if (ui.checkForBackButtonClicked())
      return;
  }
}
