
//      ******************************************************************
//      *                                                                *
//      *    Example shows how to create a menu one or more sub menus    *
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
// Notes on building the menu tables:
//
// Every application should have one Main Menu.  In some cases, you may have too many
// buttons to fit on a single screen.  The solution here is to use Sub Menus.  Sub Menus 
// are menus that are called from a main menu, or another sub menu.
//
// Sub menus tables are built the same way the Main Menu, except for changes in the first
// line.  The first field, of the first line, in a sub menu table is always set to:
//                         MENU_ITEM_TYPE_SUB_MENU_HEADER
// and the last field is set to the name of its parent menu (typically "mainMenu").  
//
// The process for adding a Sub Menu button to the main menu is very similar to adding a
// "Command" button, by inserting one line in the Main Menu table.  In this case that line  
// begins with MENU_ITEM_TYPE_SUB_MENU (in the first field).  The second field is the text
// that you want displayed on the button that invokes the sub menu.  The third field is 
// the number of columns (ie MENU_COLUMNS_1, MENU_COLUMNS_2...), and the the fourth field 
// points to the Sub Menu table.
//
// In this example, we will define a main menu having several commands, along with a 
// "Settings" sub menu.  The Settings menu will also have some commands, plus it will have
// a sub menu.  



//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];
extern MENU_ITEM settingsMenu[];
extern MENU_ITEM ridiculousMenu[];



//
// the main menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,  "Example Seven - Sub Menus",  MENU_COLUMNS_1,      mainMenu},
  {MENU_ITEM_TYPE_COMMAND,           "Count to two",               commandCountTo2,     NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to four",              commandCountTo4,     NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Count to six",               commandCountTo6,     NULL},
  {MENU_ITEM_TYPE_SUB_MENU,          "Settings",                   NULL,                settingsMenu},
  {MENU_ITEM_TYPE_END_OF_MENU,       "",                           NULL,                NULL}
};


//
// the Settings menu
//
MENU_ITEM settingsMenu[] = {
  {MENU_ITEM_TYPE_SUB_MENU_HEADER,    "Settings...",               MENU_COLUMNS_1,              mainMenu},
  {MENU_ITEM_TYPE_COMMAND,            "Set screen colors",         commandSetScreenColors,      NULL},
  {MENU_ITEM_TYPE_TOGGLE,             "Self destruct",             enableSelfDestructCallback,  NULL},
  {MENU_ITEM_TYPE_SUB_MENU,           "Ridiculous stuff",          NULL,                        ridiculousMenu},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",                          NULL,                        NULL}
};


//
// the Ridiculous menu
//
MENU_ITEM ridiculousMenu[] = {
  {MENU_ITEM_TYPE_SUB_MENU_HEADER,    "Ridiculous Stuff...",       MENU_COLUMNS_2,          settingsMenu},
  {MENU_ITEM_TYPE_COMMAND,            "Beer on the wall",          commandBeerOnTheWall,    NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Funny buttons",             commandFunnyButtons,     NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",                          NULL,                    NULL}
};




//
// display the menu, then execute commands selected by the user
//
void loop() 
{  
  ui.displayAndExecuteMenu(mainMenu);
}



// ---------------------------------------------------------------------------------
//                            Commands executed from the Main Menu
// ---------------------------------------------------------------------------------

//
// menu command to "Count To 2"
//
void commandCountTo2(void)
{  
  ui.drawTitleBar("Count To Two");
  showCount(2);
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


// ---------------------------------------------------------------------------------
//                 Commands executed from the "Settings" Sub Menu
// ---------------------------------------------------------------------------------

//
// settings menu command to set the screen colors
//
static int colorSelection = 1;

void commandSetScreenColors(void)
{  
  ui.drawTitleBarWithBackButton("Display Settings");
  ui.clearDisplaySpace();

  //
  // use a Selection Boxes to select screen colors
  //
  SELECTION_BOX colorSelectionBox;
  colorSelectionBox.labelText = "Screen color palette";
  colorSelectionBox.value = colorSelection;
  colorSelectionBox.choice0Text = "Gray";
  colorSelectionBox.choice1Text = "Blue";
  colorSelectionBox.choice2Text = "";
  colorSelectionBox.choice3Text = "";
  colorSelectionBox.centerX = ui.displaySpaceCenterX;
  colorSelectionBox.centerY = 146;
  colorSelectionBox.width = 250;
  colorSelectionBox.height = 33;
  ui.drawSelectionBox(colorSelectionBox);

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
    // when the "Back" button is pressed, save all the values selected
    // by the user
    //
    if (ui.checkForBackButtonClicked())
    {
      colorSelection = colorSelectionBox.value;

      if (colorSelection == 0)
        ui.setColorPaletteGray();
      else
        ui.setColorPaletteBlue();
      return;
    }
  }
}



//
// toggle used to enable / disable "Self Destruct Mode"
//
// Sometimes it is easier to use a single button to configure something, rather than
// having a separate screen with setting choices.  This can be done using a "Toggle"
// button.  
//
static byte enableSelfDestruct = false;

void enableSelfDestructCallback(void)
{
  //
  // check if menu is requesting that the state be changed (can have more than 2 states)
  //
  if (ui.toggleSelectNextStateFlg)
  {
    //
    // select the next state
    //
    enableSelfDestruct = !enableSelfDestruct;
  }
  
  //
  // here is where you update stuff depending on the choice made by the user
  // This assumes that you have wired a LED to pin 31
  //
  pinMode(31, OUTPUT);
  
  if (enableSelfDestruct)
    digitalWrite(31, HIGH);
  else
    digitalWrite(31, LOW);
 
  //
  // send back an indication of the current state, this text is written on the button
  //
  if(enableSelfDestruct)
    ui.toggleText = "On";
  else
    ui.toggleText = "Off";
}


// ---------------------------------------------------------------------------------
//                 Commands executed from the "Ridiculous" Sub Menu
// ---------------------------------------------------------------------------------

//
// "Beer on the wall command" command
//
static int initialBottlesOfBeer = 99;


void commandBeerOnTheWall(void)
{
  int bottlesOfBeer = initialBottlesOfBeer;
  int beerCountY = ui.displaySpaceTopY + 44;
 
  ui.drawTitleBar("Beer Ccounter");
  ui.clearDisplaySpace();
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, ui.displaySpaceTopY + 28);
  ui.lcdPrintCentered("Bottles of beer on the wall");
  ui.lcdSetCursorXY(ui.displaySpaceCenterX, beerCountY);
  ui.lcdPrintCentered(bottlesOfBeer);

  BUTTON okButton        = {"OK",             ui.displaySpaceCenterX-60,   ui.displaySpaceBottomY-37,  100,  35};
  BUTTON cancelButton    = {"Cancel",         ui.displaySpaceCenterX+60,   ui.displaySpaceBottomY-37,  100,  35};
  BUTTON countButton     = {"Take one down",  ui.displaySpaceCenterX,      ui.displaySpaceCenterY,     120,  45};

  ui.drawButton(okButton);
  ui.drawButton(cancelButton);
  ui.drawButton(countButton);

  while(true)
  {
    ui.getTouchEvents();

    if (ui.checkForMenuButtonClicked())
      return;

    if (ui.checkForButtonClicked(okButton))
    {
      initialBottlesOfBeer = bottlesOfBeer;
      return;
    }

    if (ui.checkForButtonClicked(cancelButton))
      return;

    if (ui.checkForButtonAutoRepeat(countButton))
    {
      bottlesOfBeer--;
      ui.lcdDrawFilledRectangle(ui.displaySpaceCenterX - 40, beerCountY,    80, 10, LCD_BLACK);
      ui.lcdSetCursorXY(ui.displaySpaceCenterX, beerCountY);
      ui.lcdPrintCentered(bottlesOfBeer);
    }
  }
}



//
// "Funny buttons" command
//
void commandFunnyButtons(void)
{   
  ui.drawTitleBar("Funny buttons");
  
  BUTTON_EXTENDED redButton   = {"Red",   90,                     80,     45,  25,   LCD_RED,   LCD_YELLOW,    LCD_ORANGE,    LCD_WHITE, Arial_8};
  BUTTON_EXTENDED blueButton  = {"Blue",  250,                    100,    90,  90,   LCD_BLUE,  LCD_LIGHTBLUE, LCD_LIGHTBLUE, LCD_NAVY,  Arial_32};
  BUTTON          okButton    = {"OK",    ui.displaySpaceCenterX, 200,    100, 35};

  ui.clearDisplaySpace(LCD_CYAN);
  
  ui.drawButton(redButton);
  ui.drawButton(blueButton);
  ui.drawButton(okButton);

  while(true)
  {
    ui.getTouchEvents();

    if (ui.checkForButtonClicked(redButton))
      ui.lcdDrawFilledCircle(50, 170, 30, LCD_RED);

    if (ui.checkForButtonClicked(blueButton))
      ui.lcdDrawFilledCircle(50, 170, 30, LCD_BLUE);

    if (ui.checkForButtonClicked(okButton))
      return;
  }
}
