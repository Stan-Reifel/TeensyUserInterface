
//      ******************************************************************
//      *                                                                *
//      *   Example shows how to prompt the user for numbers and values  *
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
  // use a grayscale color palette
  //
  ui.setColorPaletteGray();
}


// ---------------------------------------------------------------------------------
//           Define the menus and top level loop, place menus after setup()
// ---------------------------------------------------------------------------------

//
// Notes on building the menu table:
//
// This example uses many "Command" button, exactly like we have used in the
// earlier examples.  Here we introduce a new menu item, the "Toggle" button.
//
// A MENU_ITEM_TYPE_TOGGLE is used somewhat like a Radio Button in a dialog 
// box.  It allows the user to select one of a fixed number of choices (such as  
// On / Off,   or   Red / Green / Blue).  Each time the user presses this button, 
// it alternates the selection (i.e. toggles between On and Off, or rotates between 
// Red, Green and Blue).  The third field in this entry points to a callback 
// function that alternates the value.
//
// The "Self Destruct" button in this example uses the Toggle feature.


//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];


//
// the main menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,  "Example Six - Prompt User for Infomation",  MENU_COLUMNS_2,              mainMenu},
  {MENU_ITEM_TYPE_COMMAND,           "Get an integer",                            commandGetAnInteger,         NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Get a float",                               commandGetAFloat,            NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Make a choice",                             commandMakeAChoice,          NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Save settings",                             commandGetValuesAndSave,     NULL},
  {MENU_ITEM_TYPE_TOGGLE,            "Self destruct",                             enableSelfDestructCallback,  NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,       "",                                          NULL,                        NULL}
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
// menu command that demonstrates how to prompt the user for an integer
//
static int xOffsetValue = 50;

void commandGetAnInteger(void)
{
  char sBuffer[25];
  
  //
  // draw the title bar and clear the screen
  //
  ui.drawTitleBar("Prompt User for an Integer");
  ui.clearDisplaySpace();
  
  //
  // set the size and initial value of the number box
  //
  const int numberBoxWidth = 200;
  const int numberBoxAndButtonsHeight = 35;

  //
  // define a Number Box so the user can select a numeric value, specify the initial value, 
  // max and min values, and step up/down amount
  //
  NUMBER_BOX my_NumberBox;
  my_NumberBox.labelText     = "Set X offset";
  my_NumberBox.value         = xOffsetValue;
  my_NumberBox.minimumValue  = -200;
  my_NumberBox.maximumValue  = 200;
  my_NumberBox.stepAmount    = 2;
  my_NumberBox.centerX       = ui.displaySpaceCenterX;
  my_NumberBox.centerY       = ui.displaySpaceCenterY - 20;
  my_NumberBox.width         = numberBoxWidth;
  my_NumberBox.height        = numberBoxAndButtonsHeight;
  ui.drawNumberBox(my_NumberBox);


  //
  // define and display "OK" and "Cancel" buttons
  //
  BUTTON okButton        = {"OK",      ui.displaySpaceCenterX-70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(okButton);

  BUTTON cancelButton    = {"Cancel",  ui.displaySpaceCenterX+70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(cancelButton);


  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the Number Box
    //
    ui.checkForNumberBoxTouched(my_NumberBox);
 
    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // user OK pressed, get the value from the Number Box and display it
      //
      xOffsetValue = my_NumberBox.value;
      sprintf(sBuffer, "X Offset = %d", xOffsetValue);

      ui.clearDisplaySpace();      
      ui.lcdSetCursorXY(ui.displaySpaceCenterX, ui.displaySpaceCenterY-10);
      ui.lcdPrintCentered(sBuffer);
      delay(1500);
      return;
    }

    //
    // check for touch events on the "Cancel" button
    //
    if (ui.checkForButtonClicked(cancelButton))
      return;
  }
}



//
// menu command that demonstrates how to prompt the user for an float
//
static float xScalerValue = 0.57;

void commandGetAFloat(void)
{
  char sBuffer[30];
  
  //
  // draw the title bar and clear the screen
  //
  ui.drawTitleBar("Prompt User for an float");
  ui.clearDisplaySpace();
  
  //
  // set the size and initial value of the number box
  //
  const int numberBoxWidth = 200;
  const int numberBoxAndButtonsHeight = 35;

  //
  // define a Number Box for Floats so the user can select a numeric value, specify 
  // the initial value, max and min values, and step up/down amount
  //
  NUMBER_BOX_FLOAT my_NumberBox;
  my_NumberBox.labelText           = "Set X scaler";
  my_NumberBox.value                = xScalerValue;
  my_NumberBox.minimumValue         = 0.0;
  my_NumberBox.maximumValue         = 1.0;
  my_NumberBox.stepAmount           = 0.01;
  my_NumberBox.digitsRightOfDecimal = 2;
  my_NumberBox.centerX              = ui.displaySpaceCenterX;
  my_NumberBox.centerY              = ui.displaySpaceCenterY - 20;;
  my_NumberBox.width                = numberBoxWidth;
  my_NumberBox.height               = numberBoxAndButtonsHeight;
  ui.drawNumberBox(my_NumberBox);


  //
  // define and display "OK" and "Cancel" buttons
  //
  BUTTON okButton        = {"OK",      ui.displaySpaceCenterX-70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(okButton);

  BUTTON cancelButton    = {"Cancel",  ui.displaySpaceCenterX+70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(cancelButton);


  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the Number Box
    //
    ui.checkForNumberBoxTouched(my_NumberBox);
 
    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // user OK pressed, get the value from the Number Box and display it
      //
      xScalerValue = my_NumberBox.value;
      sprintf(sBuffer, "X Offset = %4.2f", xScalerValue);

      ui.clearDisplaySpace();      
      ui.lcdSetCursorXY(ui.displaySpaceCenterX, ui.displaySpaceCenterY-10);
      ui.lcdPrintCentered(sBuffer);
      delay(1500);
      return;
    }

    //
    // check for touch events on the "Cancel" button
    //
    if (ui.checkForButtonClicked(cancelButton))
      return;
  }
}



//
// menu command that demonstrates how to prompt the user to select one of n things
//
static int powerSelection = 2;
static int colorSelection = 1;
static int safetySelection = 0;

void commandMakeAChoice(void)
{  
  ui.drawTitleBarWithBackButton("Using Selection Boxes");
  ui.clearDisplaySpace();

  //
  // define and display 3 selection boxes, one with 2 choice, one with 3 choice 
  // and one with 4
  //
  SELECTION_BOX powerSelectionBox;
  powerSelectionBox.labelText = "Laser power level";
  powerSelectionBox.value = powerSelection;
  powerSelectionBox.choice0Text = "Low";
  powerSelectionBox.choice1Text = "Medium";
  powerSelectionBox.choice2Text = "High";
  powerSelectionBox.choice3Text = "";
  powerSelectionBox.centerX = ui.displaySpaceCenterX;
  powerSelectionBox.centerY = 79;
  powerSelectionBox.width = 250;
  powerSelectionBox.height = 33;
  ui.drawSelectionBox(powerSelectionBox);

  SELECTION_BOX colorSelectionBox;
  colorSelectionBox.labelText = "Laser color";
  colorSelectionBox.value = colorSelection;
  colorSelectionBox.choice0Text = "Red";
  colorSelectionBox.choice1Text = "Green";
  colorSelectionBox.choice2Text = "Blue";
  colorSelectionBox.choice3Text = "White";
  colorSelectionBox.centerX = ui.displaySpaceCenterX;
  colorSelectionBox.centerY = 146;
  colorSelectionBox.width = 250;
  colorSelectionBox.height = 33;
  ui.drawSelectionBox(colorSelectionBox);

  SELECTION_BOX safetySelectionBox;
  safetySelectionBox.labelText = "Laser safety switch";
  safetySelectionBox.value = safetySelection;
  safetySelectionBox.choice0Text = "Off";
  safetySelectionBox.choice1Text = "On";
  safetySelectionBox.choice2Text = "";
  safetySelectionBox.choice3Text = "";
  safetySelectionBox.centerX = ui.displaySpaceCenterX;
  safetySelectionBox.centerY = 214;
  safetySelectionBox.width = 250;
  safetySelectionBox.height = 33;
  ui.drawSelectionBox(safetySelectionBox);

  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events in the selection boxes
    //
    ui.checkForSelectionBoxTouched(powerSelectionBox);
    ui.checkForSelectionBoxTouched(colorSelectionBox);
    ui.checkForSelectionBoxTouched(safetySelectionBox);

    //
    // when the "Back" button is pressed, save all the values selected
    // by the user
    //
    if (ui.checkForBackButtonClicked())
    {
      powerSelection = powerSelectionBox.value;
      colorSelection = colorSelectionBox.value;
      safetySelection = safetySelectionBox.value;
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


//
// storage locations in EEPROM for configuration values settable below
//
const int EEPROM_X_OFFSET = 0;                        // int requires 5 bytes of EEPROM storage
const int EEPROM_X_SCALER = EEPROM_X_OFFSET + 5;      // float requires 5 bytes of EEPROM storage
const int EEPROM_Y_OFFSET = EEPROM_X_SCALER + 5;      // int requires 5 bytes of EEPROM storage
const int EEPROM_Y_SCALER = EEPROM_Y_OFFSET + 5;      // float requires 5 bytes of EEPROM storage


//
// defaults for configuration values, these values are used if they have never been set before
//
const int   DEFAULT_X_OFFSET   = 50; 
const float DEFAULT_X_SCALER   = 0.57;
const int   DEFAULT_Y_OFFSET   = -51;
const float DEFAULT_Y_SCALER   = 0.85;


//
// menu command that demonstrates how to prompt the user for info then save it
//
void commandGetValuesAndSave(void)
{
  const int numberBoxWidth = 145;
  const int numberBoxHeight = 34;


  //
  // read initial values from the EEPROM, if the EEPROM has never been set, use default values
  //
  int initialXOffset = ui.readConfigurationInt(EEPROM_X_OFFSET, DEFAULT_X_OFFSET);
  float initialXScaler = ui.readConfigurationFloat(EEPROM_X_SCALER, DEFAULT_X_SCALER);
  int initialYOffset = ui.readConfigurationInt(EEPROM_Y_OFFSET, DEFAULT_Y_OFFSET);
  float initialYScaler = ui.readConfigurationFloat(EEPROM_Y_SCALER, DEFAULT_Y_SCALER);


  //
  // draw title bar without a "Back" button
  //
  ui.drawTitleBar("Calibrate X and Y Axes");
  ui.clearDisplaySpace();
  

  //
  // define and display number boxes for setting calibration constants
  //
  NUMBER_BOX XOffset_NumberBox;
  XOffset_NumberBox.labelText    = "Set X offset";
  XOffset_NumberBox.value        = initialXOffset;
  XOffset_NumberBox.minimumValue = -200;
  XOffset_NumberBox.maximumValue = 200;
  XOffset_NumberBox.stepAmount   = 2;
  XOffset_NumberBox.centerX      = ui.displaySpaceCenterX - 80;
  XOffset_NumberBox.centerY      = 83;
  XOffset_NumberBox.width        = numberBoxWidth;
  XOffset_NumberBox.height       = numberBoxHeight;
  ui.drawNumberBox(XOffset_NumberBox);

  NUMBER_BOX_FLOAT XScaler_NumberBox;
  XScaler_NumberBox.labelText            = "Set X scaler";
  XScaler_NumberBox.value                = initialXScaler;
  XScaler_NumberBox.minimumValue         = 0.0;
  XScaler_NumberBox.maximumValue         = 1.0;
  XScaler_NumberBox.stepAmount           = 0.01;
  XScaler_NumberBox.digitsRightOfDecimal = 2;
  XScaler_NumberBox.centerX              = ui.displaySpaceCenterX - 80;
  XScaler_NumberBox.centerY              = 157;
  XScaler_NumberBox.width                = numberBoxWidth;
  XScaler_NumberBox.height               = numberBoxHeight;
  ui.drawNumberBox(XScaler_NumberBox);


  NUMBER_BOX YOffset_NumberBox;
  YOffset_NumberBox.labelText    = "Set Y offset";
  YOffset_NumberBox.value        = initialYOffset;
  YOffset_NumberBox.minimumValue = -200;
  YOffset_NumberBox.maximumValue = 200;
  YOffset_NumberBox.stepAmount   = 2;
  YOffset_NumberBox.centerX      = ui.displaySpaceCenterX + 80;
  YOffset_NumberBox.centerY      = 83;
  YOffset_NumberBox.width        = numberBoxWidth;
  YOffset_NumberBox.height       = numberBoxHeight;
  ui.drawNumberBox(YOffset_NumberBox);

  NUMBER_BOX_FLOAT YScaler_NumberBox;
  YScaler_NumberBox.labelText            = "Set Y scaler";
  YScaler_NumberBox.value                = initialYScaler;
  YScaler_NumberBox.minimumValue         = 0.0;
  YScaler_NumberBox.maximumValue         = 1.0;
  YScaler_NumberBox.stepAmount           = 0.01;
  YScaler_NumberBox.digitsRightOfDecimal = 2;
  YScaler_NumberBox.centerX              = ui.displaySpaceCenterX + 80;
  YScaler_NumberBox.centerY              = 157;
  YScaler_NumberBox.width                = numberBoxWidth;
  YScaler_NumberBox.height               = numberBoxHeight;
  ui.drawNumberBox(YScaler_NumberBox);


  //
  // define and display "OK" and "Cancel" buttons
  //
  BUTTON okButton        = {"OK",     ui.displaySpaceCenterX-80,   ui.displaySpaceBottomY-27,  100 , numberBoxHeight};
  ui.drawButton(okButton);

  BUTTON cancelButton    = {"Cancel", ui.displaySpaceCenterX+80,   ui.displaySpaceBottomY-27,  100 , numberBoxHeight};
  ui.drawButton(cancelButton);


  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the Number Boxes
    //
    ui.checkForNumberBoxTouched(XOffset_NumberBox);
    ui.checkForNumberBoxTouched(XScaler_NumberBox);
    ui.checkForNumberBoxTouched(YOffset_NumberBox);
    ui.checkForNumberBoxTouched(YScaler_NumberBox);

    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // save the values set by the user to EEPROM, then return to the menu
      //
      ui.writeConfigurationInt(EEPROM_X_OFFSET, XOffset_NumberBox.value);
      ui.writeConfigurationFloat(EEPROM_X_SCALER, XScaler_NumberBox.value);
      ui.writeConfigurationInt(EEPROM_Y_OFFSET, YOffset_NumberBox.value);
      ui.writeConfigurationFloat(EEPROM_Y_SCALER, YScaler_NumberBox.value);
      return;
    }

    //
    // check for touch events on the "Cancel" button
    //
    if (ui.checkForButtonClicked(cancelButton))
      return;
  }
}
