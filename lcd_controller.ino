/***************************************************************************************
    Name    : LCD Button Shield Menu
    Author  : Paul Siewert
    Created : June 14, 2016
    Last Modified: June 14, 2016
    Version : 1.0
    Notes   : This code is for use with an Arduino Uno and LCD/button shield. The
              intent is for anyone to use this program to give them a starting
              program with a fully functional menu with minimal modifications
              required by the user.
    License : This program is free software. You can redistribute it and/or modify
              it under the terms of the GNU General Public License as published by
              the Free Software Foundation, either version 3 of the License, or
              (at your option) any later version.
              This program is distributed in the hope that it will be useful,
              but WITHOUT ANY WARRANTY; without even the implied warranty of
              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
              GNU General Public License for more details.
 ***************************************************************************************/
/*
   This program is designed to get you as close as possible to a finished menu for the standard Arduino Uno LCD/button shield. The only required modifications
   are to add as menu items to the master menu (menuItems array) and then modify/adjust the void functions below for each of those selections.
*/

// You can have up to 10 menu items in the menuItems[] array below without having to change the base programming at all. Name them however you'd like. Beyond 10 items, you will have to add additional "cases" in the switch/case
// section of the operateMainMenu() function below. You will also have to add additional void functions (i.e. menuItem11, menuItem12, etc.) to the program.
String menuItems[] = {"TIME", "VENTILE 1", "VENTILE 2", "MANUAL", "ADD VENTILE", "SETUP"};

// Navigation button variables
int readKey;

// Menu control variables
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = 0;

// Creates 3 custom characters for the menu display
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};
#include <DS3231.h>
#include <Wire.h>
DS3231  rtc(SDA, SCL);
#include <LiquidCrystal.h>
// Setting the LCD shields pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  
 //create array which is for 2 ventile with 1 temporary values 
int led=0; //declare pin 2 is led
char displayArray[16];
        typedef struct ventile {
        char sH;   
        char sM;
        char fH;
        char fM;
        } ventile_t;
        
ventile_t ventiles[2];

void setup() {
        // trying for start end hours
      //  ventiles[0].sH = 8;
      //  ventiles[0].sM = 30;
      //  ventiles[0].fH = 8;
      //  ventiles[0].fM = 34;
  pinMode(0, OUTPUT);

  // Initializes and clears the LCD screen
  rtc.begin(); // Initialize the rtc object
  //The following lines can be uncommented to set the date and time
//    rtc.setDOW(SATURDAY);     // Set Day-of-Week to SUNDAY
//    rtc.setTime(14, 55, 0);     // Set the time to 12:00:00 (24hr format)
//    rtc.setDate(11, 5, 2019);   // Set the date to January 1st, 2014
  
  lcd.begin(16, 2);
  lcd.clear();

  // Creates the byte for the 3 custom characters
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);

}
void blinkLed(){
   // TODO for all ventiles

int r = rtc.getTime().hour*60 +rtc.getTime().min;
int s = ventiles[0].sH*60 + ventiles[0].sM;
int f = ventiles[0].fH*60 + ventiles[0].fM;

//    lcd.setCursor(0, 0);
//     sprintf(displayArray, "R%d S%d F%d",  rtc.getTime().min, ventiles[0].sM, ventiles[0].fM);
//    lcd.print(displayArray);
//    lcd.setCursor(0, 1);
//     sprintf(displayArray, "R%d S%d F%d",  rtc.getTime().hour, ventiles[0].sH, ventiles[0].fH);
//    lcd.print(displayArray);
//    lcd.setCursor(0, 0);
//     sprintf(displayArray, "R%d S%d",  r, s);
//    lcd.print(displayArray);
//    lcd.setCursor(0, 1);
//     sprintf(displayArray, "F%d",  f);
//    lcd.print(displayArray);

//   if (
//    ventiles[0].sH <= rtc.getTime().hour && rtc.getTime().hour <= ventiles[0].fH
//   &&
//   ventiles[0].sM <= rtc.getTime().min && rtc.getTime().min < ventiles[0].fM) {
    if (s<r&&r<f) {
      digitalWrite(led, HIGH);
   } else {
      digitalWrite(led, LOW);
    }

}

void loop() {
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
  blinkLed();
}

// This function will generate the 2 menu items that can fit on the screen. They will change as you scroll through your menu. Up and down arrows will indicate your current menu position.
void mainMenuDraw() {
  Serial.print(menuPage);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

// When called, this function will erase the current cursor and redraw it based on the cursorPosition and menuPage variables.
void drawCursor() {
  for (int x = 0; x < 2; x++) {     // Erases current cursor
    lcd.setCursor(0, x);
    lcd.print(" ");
  }

  // The menu is set up to be progressive (menuPage 0 = Item 1 & Item 2, menuPage 1 = Item 2 & Item 3, menuPage 2 = Item 3 & Item 4), so
  // in order to determine where the cursor should be you need to see if you are at an odd or even menu page and an odd or even cursor position.
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is even and the cursor position is even that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is even and the cursor position is odd that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is odd and the cursor position is even that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is odd and the cursor position is odd that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}


void operateMainMenu() {
  int activeButton = 0;
  while (activeButton == 0) {
    blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 1:  // This case will execute if the "forward" button is pressed
        button = 0;
        switch (cursorPosition) { // The case that is selected here is dependent on which menu page you are on and where the cursor is.
          case 0:
            menuItem1();
            break;
          case 1:
            menuItem2();
            break;
          case 2:
            menuItem3();
            break;
          case 3:
            menuItem4();
            break;
          case 4:
            menuItem5();
            break;
          case 5:
            menuItem6();
            break;
          case 6:
            menuItem7();
            break;
          case 7:
            menuItem8();
            break;
          case 8:
            menuItem9();
            break;
          case 9:
            menuItem10();
            break;
        }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}

// This function is called whenever a button press is evaluated. The LCD shield works by observing a voltage drop across the buttons all hooked up to A0.
int evaluateButton(int x) {
  int result = 0;
  if (x < 50) {
    result = 1; // right
  } else if (x < 195) {
    result = 2; // up
  } else if (x < 380) {
    result = 3; // down
  } else if (x < 500) {
    result = 4; // left
  } else if (x < 790) {
    result = 5; // select
  }
  return result;
}

// If there are common usage instructions on more than 1 of your menu items you can call this function from the sub
// menus to make things a little more simplified. If you don't have common instructions or verbage on multiple menus
// I would just delete this void. You must also delete the drawInstructions()function calls from your sub menu functions.
void drawInstructions() {
  lcd.setCursor(0, 1); // Set cursor to the bottom line
  lcd.print("Use ");
  lcd.print(byte(1)); // Up arrow
  lcd.print("/");
  lcd.print(byte(2)); // Down arrow
  lcd.print(" buttons");
}

void menuItem1() { // Function executes when you select the 1st item from main menu
  int activeButton = 0;
  while (activeButton == 0) {
             blinkLed();
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("Time:  ");
             lcd.print(rtc.getTimeStr());
             
             lcd.setCursor(0,1);
             lcd.print("Date: ");
             lcd.print(rtc.getDateStr());
             
             delay(1000); 
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

char incr(char *number, char mod) {
  *number = (*number + 1) % mod;
}

char decr(char *number, char mod) {
  if(*number == 0) 
  {*number = mod-1;}
  else { *number = (*number - 1) % mod; }
}

void subMenuVentile(char ventileSelected) {
int activeButton = 0;
      int selectTime = 0;
      lcd.clear();
ventile_t temporaryVentile;

//memcpy (&ventiles[0], &temporaryVentile, sizeof(ventile_t));
temporaryVentile.sH = ventiles[ventileSelected].sH;
temporaryVentile.sM = ventiles[ventileSelected].sM;
temporaryVentile.fH = ventiles[ventileSelected].fH;
temporaryVentile.fM = ventiles[ventileSelected].fM;

  while (activeButton == 0) {
      blinkLed();
      lcd.setCursor(0, 0);
      
      char saved;
      if (temporaryVentile.sH == ventiles[ventileSelected].sH
       && temporaryVentile.sM == ventiles[ventileSelected].sM
       && temporaryVentile.fH == ventiles[ventileSelected].fH
       && temporaryVentile.fM == ventiles[ventileSelected].fM) {
        saved = 'y';
      } else {
        saved = 'n';
      }
      
      sprintf(displayArray, "S%02d:%02d F%02d:%02d V%d S%c", temporaryVentile.sH, temporaryVentile.sM, temporaryVentile.fH,
              temporaryVentile.fM, ventileSelected, saved);
      lcd.print(displayArray);
      lcd.setCursor(0, 1);
      lcd.print(selectTime);
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 1:
      selectTime = (selectTime + 1 )%4;
      delay(100);    
      break;
      case 2: 
        switch(selectTime){
          case 0:
            incr(&temporaryVentile.sH, 24);
           break;
           case 1:
            incr(&temporaryVentile.sM, 60);
           break;
          case 2:
            incr(&temporaryVentile.fH, 24);
           break;
          case 3:
            incr(&temporaryVentile.fM, 60);
           break;
          }
      break;
      case 3:
      switch(selectTime){
          case 0:
            decr(&temporaryVentile.sH, 24);
           break;
           case 1:
            decr(&temporaryVentile.sM, 60);
           break;
          case 2:
            decr(&temporaryVentile.fH, 24);
           break;
          case 3:
            decr(&temporaryVentile.fM, 60);
           break;
          }     
      break;
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
      case 5: // select button makes it works
        ventiles[ventileSelected].sH = temporaryVentile.sH;
        ventiles[ventileSelected].sM = temporaryVentile.sM;
        ventiles[ventileSelected].fH = temporaryVentile.fH;
        ventiles[ventileSelected].fM = temporaryVentile.fM;
        break;
    }
  }
}

void menuItem2() { // Function executes when you select the 2nd item from main menu
  subMenuVentile(0);
}

void menuItem3() { // Function executes when you select the 3rd item from main menu
  subMenuVentile(1);
}

void menuItem4() { // Function executes when you select the 4th item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 4");

  while (activeButton == 0) {
      blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem5() { // Function executes when you select the 5th item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 5");

  while (activeButton == 0) {  blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem6() { // Function executes when you select the 6th item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 6");

  while (activeButton == 0) {  blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem7() { // Function executes when you select the 7th item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 7");

  while (activeButton == 0) {   blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem8() { // Function executes when you select the 8th item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 8");

  while (activeButton == 0) {   blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem9() { // Function executes when you select the 9th item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 9");

  while (activeButton == 0) {   blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem10() { // Function executes when you select the 10th item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 10");

  while (activeButton == 0) {  blinkLed();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
}
