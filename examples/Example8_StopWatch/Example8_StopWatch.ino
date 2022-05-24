
//      ******************************************************************
//      *                                                                *
//      *    Example shows an application that starts with the app's     *
//      *    main screen, then pressing the Menu button sets options     *
//      *                                                                *
//      *            S. Reifel & Co.                2/18/2022            *
//      *                                                                *
//      ******************************************************************

// The previous examples show sketches that first run by displaying a menu.  
// In some cases you want to start your application with its display, then 
// press a button to pull up its menu.  This sketch runs a Stopwatch.  The 
// stopwatch is displayed immediately when powered up.  At anytime the user  
// can press the "Menu" button to get a list of options.  

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


//
// values for stopwatchMode
//
const byte STOPWATCH_READY = 0;
const byte STOPWATCH_RUNNING = 1;
const byte STOPWATCH_STOPPED = 2;


//
// local vars
//
static byte displayDigitsFlg = 1;
static byte displayColorsFlg = 1;
static byte stopwatchMode;
static unsigned long stopwatchStartTime;
static unsigned long stopwatchCurrentTime;


//
// build a button for the Stopwatch display
//
BUTTON stopwatchButton;


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
// Notes on building the Main-menu:
//
// In this app, the first screen the user sees is not a menu, but the stopwatch.
// The user calls up the menu by pressing the "Menu" button on the Title Bar.  
// From the menu, they can return to the stopwatch by pressing the menu's "Back" 
// button.
//
// Normally the Main-menu does not show a "Back" button.  To enable the "Back" 
// button, the fourth field in the MENU_ITEM_TYPE_MAIN_MENU_HEADER line is set 
// to NULL.
// 


//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];


//
// main menu, NOTE HOW 4TH COLUMN IN FIRST LINE IS SET TO "NULL", THIS DISPLAYS THE "MENU" BUTTON
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,  "Stopwatch Settings",         MENU_COLUMNS_1,            NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Set number of digits",       commandSetDigits,          NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Set screen colors",          commandSetScreenColors,    NULL},
  {MENU_ITEM_TYPE_COMMAND,           "About",                      commandAbout,              NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,       "",                           NULL,                      NULL}
};



// ---------------------------------------------------------------------------------
//                             Stopwatch application
// ---------------------------------------------------------------------------------

//
// example of an application that's displayed first upon power up, then
// menus can be called from it
//
void loop()
{
  stopWatch();
}



//
// stopwatch application
//
void stopWatch(void)
{ 
  //
  // build the Stopwatch button
  //
  stopwatchButton = {"Start",    ui.displaySpaceCenterX, ui.displaySpaceBottomY-40,  150 , 50};
   
  //
  // fully redraw the stopwatch, including the title bar
  //
  drawStopwatch(true);
  
  //
  // check for touch events from the user, and update the stopwatch display if it's running
  //
  while(true)
  {
    //
    // get any new touch events on the LCD
    //
    ui.getTouchEvents();

 
    //
    // check if the Menu button was press
    //
    if (ui.checkForMenuButtonClicked())
    {
      //
      // the user pressed the Menu button, display the menu
      //
      ui.displayAndExecuteMenu(mainMenu);
      drawStopwatch(true);
    }


    //
    // check if the Stopwatch button was pressed
    //
    if (ui.checkForButtonClicked(stopwatchButton))
    {
      //
      // Stopwatch button was pressed, select the next stopwatch mode
      //
      switch(stopwatchMode)
      {
        case STOPWATCH_READY:
        {
          //
          // start the stopwatch
          //
          stopwatchStartTime = millis();
          stopwatchCurrentTime = millis();
          stopwatchMode = STOPWATCH_RUNNING;
          drawStopwatch(false);
          stopwatchButton.labelText = "Stop";         // change the label on the button to "Stop"
          ui.drawButton(stopwatchButton);
          break;
        }

        case STOPWATCH_RUNNING:
        {
          //
          // stop the stopwatch
          //
          stopwatchCurrentTime = millis();
          stopwatchMode = STOPWATCH_STOPPED;
          drawStopwatch(false);
          stopwatchButton.labelText = "Reset";        // change the label on the button to "Reset"
          ui.drawButton(stopwatchButton);
          break;
        }

        case STOPWATCH_STOPPED:
        {
          //
          // reset the stopwatch
          //
          stopwatchStartTime = 0;
          stopwatchCurrentTime = 0;
          stopwatchMode = STOPWATCH_READY;
          drawStopwatch(false);
          stopwatchButton.labelText = "Start";        // change the label on the button to "Start"
          ui.drawButton(stopwatchButton);
          break;
        }
      }
    }

        
    //
    // update the time shown on the stopwatch
    //
    if (stopwatchMode == STOPWATCH_RUNNING)
    {
      unsigned long timeNow = millis();
      if (timeNow > stopwatchCurrentTime + 47)
      {
        stopwatchCurrentTime = timeNow;
        drawStopwatch(false);
      }
    }
  }
}



//
// draw the stopwatch display
//  Enter:  redrawAllFlg = true if the display space should be cleared and everything redrawn
//
void drawStopwatch(boolean redrawAllFlg)
{
  byte hours;
  byte minutes;
  byte seconds;
  byte hundredths;
  byte tenths;
  long milliSeconds;
  char sBuf[15];
  char *sbufPntr;

  //
  // set the background color for the stopwatch
  //
  uint16_t backgroundColor = ui.lcdMakeColor(25, 50, 31);

  //
  // check if the stopwatch should be fully initialized and everything redrawn
  //
  if (redrawAllFlg)
  {
    //
    // draw the title bar, include the "Hamburger Menu" button on it
    //
    ui.drawTitleBarWithMenuButton("Stopwatch!");

    //
    // draw everything on the stopwatch display, including the button
    //
    ui.clearDisplaySpace(backgroundColor);
    stopwatchButton.labelText = "Start";
    ui.drawButton(stopwatchButton);

    //
    // initialize the stopwatch
    //
    stopwatchStartTime = 0;
    stopwatchCurrentTime = 0;
    stopwatchMode = STOPWATCH_READY;
  }


  //
  // compute the stopwatch time in Hours, Minutes, Seconds and Hundredths
  //
  milliSeconds = stopwatchCurrentTime - stopwatchStartTime;
  
  hours = milliSeconds / (60L * 60L * 1000L);
  milliSeconds -= (hours * (60L * 60L * 1000L));
  
  minutes = milliSeconds / (60L * 1000L);
  milliSeconds -= (minutes * (60L * 1000L));
  
  seconds = milliSeconds / (1000L);
  milliSeconds -= (seconds * 1000L);
  
  hundredths = milliSeconds / 10L;
  tenths = milliSeconds / 100L;
  
  sbufPntr = sBuf;


  //
  // build a single ASCII string formatted for the stopwatch's display
  //
  sbufPntr = timeDigits(sbufPntr, hours);
  *sbufPntr++ = ':';
  
  sbufPntr = timeDigits(sbufPntr, minutes);
  *sbufPntr++ = ':';
  
  sbufPntr = timeDigits(sbufPntr, seconds);
  *sbufPntr++ = ':';

  if (displayDigitsFlg == 1)
    sbufPntr = timeDigits(sbufPntr, hundredths);
  else
  {
    *sbufPntr++ = tenths + '0';
    *sbufPntr = 0;
  }


  //
  // pick a color to display the stopwatch digits
  //
  uint16_t textColor;
  if (displayColorsFlg == 0)
    textColor = ui.lcdMakeColor(5, 10, 5);
  else
    textColor = ui.lcdMakeColor(0, 0, 25);
  ui.lcdSetFontColor(textColor);

  
  //
  // blank the screen where the stopwatch's number are displayed
  //
  ui.lcdSetFont(Arial_20_Bold);
  int textWidth = ui.lcdStringWidthInPixels(sBuf);
  int textHeight = ui.lcdGetFontHeightWithoutDecenders();
  int textY = ui.displaySpaceCenterY-40;
  int textX = ui.displaySpaceCenterX - textWidth/2;
  ui.lcdSetCursorXY(textX, textY);
  ui.lcdDrawFilledRectangle(textX, textY, textWidth, textHeight, backgroundColor);


  //
  // display the string showing the stopwatch's time
  //
  ui.lcdPrint(sBuf);
}



//
// convert a number into a two digit string, pad with leading zeros
//
char* timeDigits(char *sBufPntr, byte n)
{
  *sBufPntr++ = (n / 10) + '0';
  *sBufPntr++ = (n % 10) + '0';
  *sBufPntr = 0;
  return(sBufPntr);
}


// ---------------------------------------------------------------------------------
//                            Commands executed from the menu
// ---------------------------------------------------------------------------------

//
// menu command to "Set display colors"
//
void commandSetDigits(void)
{  
  //
  // draw the title bar and clear the display space
  //
  ui.drawTitleBar("Set Stopwatch Precision");
  ui.clearDisplaySpace();

  //
  // build and display a selection box for setting the number of digits
  //
  SELECTION_BOX digitsSelectionBox;
  digitsSelectionBox.labelText = "Configure seconds";
  digitsSelectionBox.value = displayDigitsFlg;
  digitsSelectionBox.choice0Text = "Show tenths";
  digitsSelectionBox.choice1Text = "Show hundredths";
  digitsSelectionBox.choice2Text = "";
  digitsSelectionBox.choice3Text = "";
  digitsSelectionBox.centerX = ui.displaySpaceCenterX;
  digitsSelectionBox.centerY = 115;
  digitsSelectionBox.width = 280;
  digitsSelectionBox.height = 40;
  ui.drawSelectionBox(digitsSelectionBox);

  //
  // define and display an "OK" button
  //
  BUTTON okButton        = {"OK",    ui.displaySpaceCenterX,   ui.displaySpaceBottomY-35,  120 , 40};
  ui.drawButton(okButton);
  
  
  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the selection Box
    //
    ui.checkForSelectionBoxTouched(digitsSelectionBox);

    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // save the values set by the user to EEPROM, then return to the menu
      //
      displayDigitsFlg = digitsSelectionBox.value;
      return;
    }
  }
}



//
// settings menu command to "set the screen colors"
//
void commandSetScreenColors(void)
{  
  ui.drawTitleBar("Set Screen Colors");
  ui.clearDisplaySpace();

  //
  // use a Selection Boxes to select screen colors
  //
  SELECTION_BOX colorSelectionBox;
  colorSelectionBox.labelText = "Screen color palette";
  colorSelectionBox.value = displayColorsFlg;
  colorSelectionBox.choice0Text = "Gray";
  colorSelectionBox.choice1Text = "Blue";
  colorSelectionBox.choice2Text = "";
  colorSelectionBox.choice3Text = "";
  colorSelectionBox.centerX = ui.displaySpaceCenterX;
  colorSelectionBox.centerY = 115;
  colorSelectionBox.width = 280;
  colorSelectionBox.height = 40;
  ui.drawSelectionBox(colorSelectionBox);

  //
  // define and display an "OK" button
  //
  BUTTON okButton        = {"OK",    ui.displaySpaceCenterX,   ui.displaySpaceBottomY-35,  120 , 40};
  ui.drawButton(okButton);

  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events in the selection box
    //
    ui.checkForSelectionBoxTouched(colorSelectionBox);
 
    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      displayColorsFlg = colorSelectionBox.value;

      if (displayColorsFlg == 0)
        ui.setColorPaletteGray();
      else
        ui.setColorPaletteBlue();
      return;
    }
  }
}



//
// menu command to "Display the About Box"
//
void commandAbout(void)
{  
  //
  // clear the screen and draw title bar showing with the "Back" button
  //
  ui.drawTitleBarWithBackButton("About This Program");
  ui.clearDisplaySpace();
  
  //
  // show some info in the display space
  //
  int y = 70;
  int ySpacing = 17;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("Stopwatch!");

  y += ySpacing * 2;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("This is an example program for the Teensy");

  y += ySpacing;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("User Interface.  It shows how to add a");

  y += ySpacing;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("hamburger menu to an application.");

  y += ySpacing * 2;
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, y);
  ui.lcdPrintCentered("Copyright (c) 2019, S. Reifel & Company.");


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
