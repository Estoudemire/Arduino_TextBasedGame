// !! Setting up the tft !! 
//Including necessary libraries 
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
//Defining pins for arduino mega
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4
byte button1 = 12; //pin for button 1
byte button2 = 13; 
//making tft object
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
int tftHeight = tft.width(); 
int tftWidth = tft.height();
//defining colors
#define	BLACK   0x0000
#define WHITE   0xFFFF
#define LBLUE   0xcede
#define GREEN   0x05a3
#define RED     0xf2eb
#define ORANGE  0xf54b
#define YELLOW  0xf76b
#define LGREEN  0x77ab
#define BLUE    0x5c5e
#define PURPLE  0xa2fe
#define PINK    0xf2fb
//including fonts
#include <Fonts/FreeMono9pt7b.h>
//Variables for text formatting 
byte yB = 83; //height for small text boxes
byte yS = tftHeight/2+18; // height for big text boxes

//Variables for the story
byte workDays = 0;
String gained = "So far, you've gained: ";
String visited = "So far, you've visited: ";
int numItems = 0; 


void setup() {
  //initalizing the tft & TFT setup
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif
  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
  tft.reset();
  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }
  tft.begin(identifier);

  //pin setup
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  //Text setup
  tft.setFont(&FreeMono9pt7b);    tft.setTextSize(1);   tft.setTextColor(WHITE);

  //Screen Setup
  tft.fillScreen(BLACK);
  tft.setRotation(1);
}

void loop() {

  // !! Day 1 !!
  byte startChoice = startSequence(); //Tells what choice the user makes on if they go to work or stay home
    if (startChoice == 1){
      goWork(); //runs the work sequence if user chooses to go to work
    } else if (startChoice ==2){
      byte stayHomeChoice = stayHome(); //gets the users choice for if they go downtown or to the city center
      if (stayHomeChoice == 1){ //choice to go to city center
        //city center
        byte cityChoice = cityCenter(); //gets the user choice if they want to go to the park or to the grocery store
        if (cityChoice == 1){
        // park
          park();
          gained = gained + "Balloons, "; //adds ballons to the string of objects the user has aquired
          visited = visited + "The Park, "; //adds the park to the string of places the user has visited 
        } else if (cityChoice == 2){
          // grocery Store
          groceryStore(); 
          gained = gained + "Cookies, Snacks and Drinks, "; //adds to the string of objects the user has aquired 
          visited = visited + "The Grocery Store, "; //adds to the string of places the user has visited
        }
      } else if (stayHomeChoice == 2){
      //downtown 
        byte downTownChoice = downTown(); //gets the user choice if they want to go to the bakery or to the party store
        if (downTownChoice == 1){
          //bakery
          bakery();
          gained = gained + "A Cake, Debt, ";//adds to the string of objects the user has aquired 
          visited = visited + "The Bakery, ";//adds to the string of places the user has visited
        } else if (downTownChoice == 2){
          //party Store
          partyStore();
          gained = gained + "A Seahorse, "; //adds to the string of objects the user has aquired 
          visited = visited + "The Party Store, ";//adds to the string of places the user has visited
        }
      }
    }
  //Ending day 1  
  background();
  textBoxS(); 
  type(F("After a long, long day of doing your best to prepare Calmantha's party, you head off to bed."), 0, yS);
  cont(); 
  textBoxB(); 
  type(gained, 0, yB);//types out the things the user has gained 
  cont(); 
  textBoxS();
  type(visited, 0, yS);//types out the list of places the user has visited 
  cont(); 
  textBoxS();
  type(F("End of day 1."), 0, yS);
  cont(); 

  // !! Day 2 !!
  startChoice = day2Start(); //runs the same as the previous day
    if (startChoice == 1){
      goWork(); //runs the work sequence if user chooses to go to work
      if (workDays == 2){
        gained = gained + "A Clownfish, "; 
        numItems = numItems + 1; 
        visited = visited + "Work, "; 
      }
    } else if (startChoice ==2){
      byte stayHomeChoice = stayHome(); //gets the users choice for if they go downtown or to the city center
      if (stayHomeChoice == 1){ //choice to go to city center
        //city center
        byte cityChoice = cityCenter(); //gets the user choice if they want to go to the park or to the grocery store
        if (cityChoice == 1){
        // park
          park();
          gained = gained + "Balloons, "; //adds ballons to the string of objects the user has aquired
          numItems = numItems + 1; 
          visited = visited + "The Park, "; //adds the park to the string of places the user has visited 
        } else if (cityChoice == 2){
          // grocery Store
          groceryStore(); 
          gained = gained + "Cookies, Snacks and Drinks, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Grocery Store, "; //adds to the string of places the user has visited
        }
      } else if (stayHomeChoice == 2){
      //downtown 
        byte downTownChoice = downTown(); //gets the user choice if they want to go to the bakery or to the party store
        if (downTownChoice == 1){
          //bakery
          bakery();
          gained = gained + "A Cake, Debt, ";//adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Bakery, ";//adds to the string of places the user has visited
        } else if (downTownChoice == 2){
          //party Store
          partyStore();
          gained = gained + "A Seahorse, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Party Store, ";//adds to the string of places the user has visited
        }
      }
    }
  //Ending day 2  
  background();
  textBoxS(); 
  type(F("After a long, long day of doing your best to prepare Calmantha's party, you head off to bed."), 0, yS);
  cont(); 
  textBoxB(); 
  type(gained, 0, yB);
  cont(); 
  textBoxS();
  type(visited, 0, yS);
  cont(); 
  textBoxS();
  type(F("End of day 2."), 0, yS);
  cont(); 

  // !! Day 3 !!
  startChoice = day3Start(); 
  if (startChoice == 1){
      goWork(); //runs the work sequence if user chooses to go to work
      if (workDays == 2){
        gained = gained + "A Clownfish, "; 
        numItems = numItems + 1; 
        visited = visited + "Work, "; 
      }
    } else if (startChoice ==2){
      byte stayHomeChoice = stayHome(); //gets the users choice for if they go downtown or to the city center
      if (stayHomeChoice == 1){ //choice to go to city center
        //city center
        byte cityChoice = cityCenter(); //gets the user choice if they want to go to the park or to the grocery store
        if (cityChoice == 1){
        // park
          park();
          gained = gained + "Balloons, "; //adds ballons to the string of objects the user has aquired
          numItems = numItems + 1; 
          visited = visited + "The Park, "; //adds the park to the string of places the user has visited 
        } else if (cityChoice == 2){
          // grocery Store
          groceryStore(); 
          gained = gained + "Cookies, Snacks and Drinks, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Grocery Store, "; //adds to the string of places the user has visited
        }
      } else if (stayHomeChoice == 2){
      //downtown 
        byte downTownChoice = downTown(); //gets the user choice if they want to go to the bakery or to the party store
        if (downTownChoice == 1){
          //bakery
          bakery();
          gained = gained + "A Cake, Debt, ";//adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Bakery, ";//adds to the string of places the user has visited
        } else if (downTownChoice == 2){
          //party Store
          partyStore();
          gained = gained + "A Seahorse, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Party Store, ";//adds to the string of places the user has visited
        }
      }
    }
  //Ending day 3  
  background();
  textBoxS(); 
  type(F("After a long, long day of doing your best to prepare Calmantha's party, you head off to bed."), 0, yS);
  cont(); 
  textBoxB(); 
  type(gained, 0, yB);
  cont(); 
  textBoxS();
  type(visited, 0, yS);
  cont(); 
  textBoxS();
  type(F("End of day 3."), 0, yS);
  cont();

  // !! Day 4 !!
  startChoice = day4Start(); 
  if (startChoice == 1){
      goWork(); //runs the work sequence if user chooses to go to work
      if (workDays == 2){
        gained = gained + "A Clownfish, "; 
        numItems = numItems + 1; 
        visited = visited + "Work, "; 
      }
    } else if (startChoice ==2){
      byte stayHomeChoice = stayHome(); //gets the users choice for if they go downtown or to the city center
      if (stayHomeChoice == 1){ //choice to go to city center
        //city center
        byte cityChoice = cityCenter(); //gets the user choice if they want to go to the park or to the grocery store
        if (cityChoice == 1){
        // park
          park();
          gained = gained + "Balloons, "; //adds ballons to the string of objects the user has aquired
          numItems = numItems + 1; 
          visited = visited + "The Park, "; //adds the park to the string of places the user has visited 
        } else if (cityChoice == 2){
          // grocery Store
          groceryStore(); 
          gained = gained + "Cookies, Snacks and Drinks, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Grocery Store, "; //adds to the string of places the user has visited
        }
      } else if (stayHomeChoice == 2){
      //downtown 
        byte downTownChoice = downTown(); //gets the user choice if they want to go to the bakery or to the party store
        if (downTownChoice == 1){
          //bakery
          bakery();
          gained = gained + "A Cake, Debt, ";//adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Bakery, ";//adds to the string of places the user has visited
        } else if (downTownChoice == 2){
          //party Store
          partyStore();
          gained = gained + "A Seahorse, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Party Store, ";//adds to the string of places the user has visited
        }
      }
    }
  //Ending day 4  
  background();
  textBoxS(); 
  type(F("After a long, long day of doing your best to prepare Calmantha's party, you head off to bed."), 0, yS);
  cont(); 
  textBoxB(); 
  type(gained, 0, yB);
  cont(); 
  textBoxS();
  type(visited, 0, yS);
  cont(); 
  textBoxS();
  type(F("End of day 4."), 0, yS);
  cont();

  // !! Day 5 !!
  startChoice = day5Start(); 
  if (startChoice == 1){
      goWork(); //runs the work sequence if user chooses to go to work
      if (workDays == 2){
        gained = gained + "A Clownfish, "; 
        numItems = numItems + 1; 
        visited = visited + "Work, "; 
      }
    } else if (startChoice ==2){
      byte stayHomeChoice = stayHome(); //gets the users choice for if they go downtown or to the city center
      if (stayHomeChoice == 1){ //choice to go to city center
        //city center
        byte cityChoice = cityCenter(); //gets the user choice if they want to go to the park or to the grocery store
        if (cityChoice == 1){
        // park
          park();
          gained = gained + "Balloons, "; //adds ballons to the string of objects the user has aquired
          numItems = numItems + 1; 
          visited = visited + "The Park, "; //adds the park to the string of places the user has visited 
        } else if (cityChoice == 2){
          // grocery Store
          groceryStore(); 
          gained = gained + "Cookies, Snacks and Drinks, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Grocery Store, "; //adds to the string of places the user has visited
        }
      } else if (stayHomeChoice == 2){
      //downtown 
        byte downTownChoice = downTown(); //gets the user choice if they want to go to the bakery or to the party store
        if (downTownChoice == 1){
          //bakery
          bakery();
          gained = gained + "A Cake, Debt, ";//adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Bakery, ";//adds to the string of places the user has visited
        } else if (downTownChoice == 2){
          //party Store
          partyStore();
          gained = gained + "A Seahorse, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Party Store, ";//adds to the string of places the user has visited
        }
      }
    }
  //Ending day 5  
  background();
  textBoxS(); 
  type(F("After a long, long day of doing your best to prepare Calmantha's party, you head off to bed."), 0, yS);
  cont(); 
  textBoxB(); 
  type(gained, 0, yB);
  cont(); 
  textBoxS();
  type(visited, 0, yS);
  cont(); 
  textBoxS();
  type(F("End of day 5."), 0, yS);
  cont();

  // !! Day 6 !!
  startChoice = day6Start(); 
  if (startChoice == 1){
      goWork(); //runs the work sequence if user chooses to go to work
      if (workDays == 2){
        gained = gained + "A Clownfish, "; 
        numItems = numItems + 1; 
        visited = visited + "Work, "; 
      }
    } else if (startChoice ==2){
      byte stayHomeChoice = stayHome(); //gets the users choice for if they go downtown or to the city center
      if (stayHomeChoice == 1){ //choice to go to city center
        //city center
        byte cityChoice = cityCenter(); //gets the user choice if they want to go to the park or to the grocery store
        if (cityChoice == 1){
        // park
          park();
          gained = gained + "Balloons, "; //adds ballons to the string of objects the user has aquired
          numItems = numItems + 1; 
          visited = visited + "The Park, "; //adds the park to the string of places the user has visited 
        } else if (cityChoice == 2){
          // grocery Store
          groceryStore(); 
          gained = gained + "Cookies, Snacks and Drinks, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Grocery Store, "; //adds to the string of places the user has visited
        }
      } else if (stayHomeChoice == 2){
      //downtown 
        byte downTownChoice = downTown(); //gets the user choice if they want to go to the bakery or to the party store
        if (downTownChoice == 1){
          //bakery
          bakery();
          gained = gained + "A Cake, Debt, ";//adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Bakery, ";//adds to the string of places the user has visited
        } else if (downTownChoice == 2){
          //party Store
          partyStore();
          gained = gained + "A Seahorse, "; //adds to the string of objects the user has aquired 
          numItems = numItems + 1;
          visited = visited + "The Party Store, ";//adds to the string of places the user has visited
        }
      }
    }
  //Ending day 6  
  background();
  textBoxS(); 
  type(F("After a long, long day of doing your best to prepare Calmantha's party, you head off to bed."), 0, yS);
  cont(); 
  textBoxB(); 
  type(gained, 0, yB);
  cont(); 
  textBoxS();
  type(visited, 0, yS);
  cont(); 
  textBoxS();
  type(F("End of day 6."), 0, yS);
  cont();

  //runs the party sequence once all days are completed
  if (numItems >= 3){
    partyGood(); //good ending if you've got at least 3 items for the party
  } else {
    partyBad(); //bad ending if you get less than 3 items for the party 
  }
    

}

byte day2Start(){
  background(); //resets the backgorund
  textBoxS(); //draws the textbox
  type(F("You wake up bright and early on day 2, ready to get to work preparing Clamantha's party. What do you do?\nButton 1 ==> Go to work\nButton 2 ==> Stay Home"), 0, yS);
  while (true){ //gets the users choice for work or stay home
    byte sensorVal = digitalRead(button1); //gets value of button 1
    byte sensorVal2 = digitalRead(button2); // gets valye of button 2
    if (sensorVal == LOW){
      return 1; //go to work
    } else if (sensorVal2 == LOW){
      return 2; // go home
    }
  }
}

byte day3Start(){ //same as day 2 start
  background(); 
  textBoxS(); 
  type(F("You wake up bright and early on day 3, ready to get to work preparing Clamantha's party. What do you do?\nButton 1 ==> Go to work\nButton 2 ==> Stay Home"), 0, yS);
  while (true){ 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    if (sensorVal == LOW){
      return 1; //go to work
    } else if (sensorVal2 == LOW){
      return 2; // go home
    }
  }
}

byte day4Start(){ //same as day 2 start 
  background(); 
  textBoxS(); 
  type(F("You wake up bright and early on day 4, ready to get to work preparing Clamantha's party. What do you do?\nButton 1 ==> Go to work\nButton 2 ==> Stay Home"), 0, yS);
  while (true){ 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    if (sensorVal == LOW){
      return 1; //go to work
    } else if (sensorVal2 == LOW){
      return 2; // go home
    }
  }
}

byte day5Start(){//same as day 2 start 
  background(); 
  textBoxS(); 
  type(F("You wake up bright and early on day 5, ready to get to work preparing Clamantha's party. What do you do?\nButton 1 ==> Go to work\nButton 2 ==> Stay Home"), 0, yS);
  while (true){ 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    if (sensorVal == LOW){
      return 1; //go to work
    } else if (sensorVal2 == LOW){
      return 2; // go home
    }
  }
}

byte day6Start(){//same as day 2 start 
  background(); 
  textBoxS(); 
  type(F("You wake up bright and early on day 6, ready to get to work preparing Clamantha's party. What do you do?\nButton 1 ==> Go to work\nButton 2 ==> Stay Home"), 0, yS);
  while (true){ 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    if (sensorVal == LOW){
      return 1; //go to work
    } else if (sensorVal2 == LOW){
      return 2; // go home
    }
  }
}
 
byte startSequence(){
  tft.fillScreen(BLACK);
  tft.setCursor(0, 90);    tft.setTextColor(WHITE);    tft.setTextSize(1);
  type(F("Welcome to our fish game! :)"), 0, 90);
  delay(1500); 
  type(F("When you see this green circle, it means you can press button 1 to continue!"), 0, 130);
  tft.fillCircle(310, 7, 6, GREEN);
  //only move on when button1 is pressed
  while (true){
    int sensorVal = digitalRead(12);
    int sensorVal2 = digitalRead(13);
    if (sensorVal == LOW) {
      tft.fillScreen(BLACK); 
      break;
    } else {
      
    }
  }

  type(F("TaDa!"), 0, 90);
  delay(1500); 
  tft.fillScreen(BLACK); 
  tft.setCursor(0, 90);
  tft.println("Now, Would you like to play?");
  tft.println(" ");
  tft.println("Button 1 ==> yes");
  tft.println("Button 2 ==> no") ;
  while (true){ 
    byte sensorVal = digitalRead(button1); //gets user's choice
    byte sensorVal2 = digitalRead(button2);
    
    if (sensorVal == LOW){  //if button 1 is pressed
      tft.fillScreen(BLACK); 
      tft.setCursor(0, 100);
      tft.println("Let's start!");
      delay(2000);
      break; //breaks loop to being game
    } else if (sensorVal2 == LOW){ //if user chooses not to play
      tft.fillScreen(BLACK); 
      tft.setCursor(0, 90);
      tft.println("If you don't want to play, I'm not sure why you're here...");
      tft.println(" ");
      tft.println("If you decide you want to play, then press the reset button :/");
      delay(10000);
      break; //breaks out of loop to let user choose again 
    }
  }

  //introduction sequence 
  tft.fillScreen(BLACK);    tft.setCursor(75, 120);    tft.setTextColor(WHITE);    tft.setTextSize(1);
  tft.print(F("You are a fish."));

  tft.fillCircle(310, 7, 6, GREEN);
  //only move on when button1 is pressed
  while (true){
    int sensorVal = digitalRead(12);
    int sensorVal2 = digitalRead(13);
    if (sensorVal == LOW) {
      tft.fillScreen(BLACK); 
      break;
    } else {
      
    }
  }

  background(); //introduction test
  delay(2000); 
  textBoxS(); 
  type(F("Like all fish, you live underwater. More specifically, you live in a nice neighborhood made of beautiful coral houses and long sea grass lawns with pretty kelp trees. "), 0, yS);
  cont();
  textBoxS(); 
  type(F("Of all your friends, your house is the nicest."), 0, yS);
  cont(); 
  textBoxB(); 
  type(F("That is why you are the default host for all your fish friend-group functions. This Sunday, your Best Fish Friend (BFF) Clamantha is having her 21st birthday party. "), 0, yB); 
  cont();
  textBoxS();
  type(F("You are determined to ensure that your BFF has the most fin-tastic, fun-filled party ever."), 0, yS);  
  cont(); 
  textBoxS(); 
  type(F("On your list of things to gather this week, you have:\nA Cake, Balloons, Snacks, and Drinks"), 0, yS); 
  cont(); 
  textBoxS(); 
  type(F("Bad news, you have no idea where to find any of these items. Good news: You have six days to figure it out."), 0, yS);
  cont(); 
  textBoxS(); 
  type(F("It is monday morning. What do you want to do?\n \nButton 1 ==> Go to work\nButton 2 ==> Stay Home (to shop or something)"), 0, yS); 

  while (true){ 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    if (sensorVal == LOW){
      return 1; //go to work
    } else if (sensorVal2 == LOW){
      return 2; // go home
    }
  }
}

void background(){
  //Draws the blue gradiant background
  for (int i = 0; i < tftHeight; i++) { //i represents y (height)
    int yBottom = tftHeight -i -1; //makes the lines start at the bottom
    uint8_t r, g, b; 
    r = 0; //red color
    g = map(yBottom, 0, tftHeight, 10, 60);   // green color
    b = map(yBottom, 0, tftHeight, 40, 140);  //blue color
    uint16_t color = tft.color565(r, g, b); //creating the color
    tft.drawFastHLine(0, i, tftWidth, color); //draws a line in that color
  }

  //Drawing the bubbles: 
  tft.fillCircle(30, 120, 5, LBLUE); //inside of the bubble
  tft.drawCircle(30, 120, 5, WHITE); //white outline
  tft.fillCircle(80, 60, 4, LBLUE); 
  tft.drawCircle(80, 60, 4, WHITE);
  tft.fillCircle(130, 25, 6, LBLUE); 
  tft.drawCircle(130, 25, 6, WHITE);
  tft.fillCircle(180, 100, 5, LBLUE); 
  tft.drawCircle(180, 100, 5, WHITE);
  tft.fillCircle(220, 35, 4, LBLUE); 
  tft.drawCircle(220, 35, 4, WHITE);
  tft.fillCircle(50, 20, 3, LBLUE); 
  tft.drawCircle(50, 20, 3, WHITE);
  tft.fillCircle(100, 10, 5, LBLUE); 
  tft.drawCircle(100, 10, 5, WHITE);
  tft.fillCircle(150, 70, 4, LBLUE); 
  tft.drawCircle(150, 70, 4, WHITE);
  tft.fillCircle(200, 15, 3, LBLUE); 
  tft.drawCircle(200, 15, 3, WHITE);
  tft.fillCircle(270, 40, 5, LBLUE); 
  tft.drawCircle(270, 40, 5, WHITE);
  tft.fillCircle(220, 60, 3, LBLUE); 
  tft.drawCircle(220, 60, 3, WHITE);
  tft.fillCircle(80, 110, 5, LBLUE); 
  tft.drawCircle(80, 110, 5, WHITE);
  tft.fillCircle(20, 70, 6, LBLUE); 
  tft.drawCircle(20, 70, 6, WHITE);
  tft.fillCircle(300, 100, 6, LBLUE); 
  tft.drawCircle(300, 100, 6, WHITE);
}

void type(String text, int x, int y) {
  String word = ""; // current word
  String line = ""; // the line to be printed
  int maxWidth = 250; 
  int characterWidth = 9; 
  int lineLength = 0; 
  int wordLength = 0; 
  int currentY = y; //y position of the cursor
  int lineHeight = 15; // Height of each line of text so we know how far to drop the y position for each new line
  
  for (int i = 0; i < text.length(); i++) { // loops through each character in the string
    char currentChar = text.charAt(i); // Get each character in the string

    if (currentChar != ' ' && currentChar != '\n') {//if the current character isn't a line or new space
      word = word + currentChar; // Adds character to the current word
      wordLength = word.length() * characterWidth; // lists how long the word is
    }

    if (currentChar == ' ') {//if the character is a space

      if (lineLength + wordLength > maxWidth) { //if the length of the line is more than the width after adding the word
        tft.setCursor(x, currentY); 
        tft.print(line); 
        currentY = currentY + lineHeight; // drops the y position of the cursor to start the new line
        line = ""; // resets the line that's going to be printed next
        lineLength = 0; // Resets the line length
      }
      line = line + word + ' '; //adds word to the line being printed
      lineLength = lineLength + wordLength + characterWidth;
      word = ""; // resets the next word to be added
      wordLength = 0; // ressets the wordLength for the next word
    }

    if (currentChar == '\n') { // if the character is a new line (\n)
      tft.setCursor(x, currentY); 
      tft.print(line); //print the line 

      currentY = currentY + lineHeight; 
      line = ""; 
      lineLength = 0; 
      word = "";
      wordLength = 0;
    }
  }

  //prints the last line
  if (line.length() > 0 || word.length() > 0) { //makes sure there's text to print
    line = line + word; 
    tft.setCursor(x, currentY); 
    tft.print(line); 
  }
}

void textBoxB(){
  tft.fillRect(0, tftHeight/2-50, tftWidth, tftHeight/2+50, BLACK); //black rextangle for text box
  tft.drawLine(0, tftHeight/2-50 + 1, tftWidth + 1, tftHeight/2-50 + 1, WHITE);//draws a line to border the box
}

void textBoxS(){
  tft.fillRect(0, tftHeight/2, tftWidth, tftHeight/2, BLACK); //draws a text box 
  tft.drawLine(0, tftHeight/2 + 1, tftWidth + 1, tftHeight/2 + 1, WHITE); //draws a border 
}

void cont(){
  //drawing the green circle that tells you to continue
  tft.fillCircle(310, 7, 6, GREEN);

  //only move on when button1 is pressed
  while (true){
    int sensorVal = digitalRead(12);
    int sensorVal2 = digitalRead(13);
    if (sensorVal == LOW) {
      background(); 
      break;
    } else {
      
    }
  }
}

byte goWork(){
  if (workDays == 0){  //the first time you go to work...
      textBoxS();
      type(F("You decide to head to work.  "), 0, yS);
      cont();
      textBoxS();
      type(F("Throughout the boring, monotonous tasks of your workday, you mention Clamantha’s party to one of your coworkers.  "), 0, yS);
      cont();
      textBoxS();
      type(F("'OH MY GOSH!' shouts your coworker, Shrimpina, who is far more excited about this party than she should be, considering she hasn’t even been invited.  "), 0, yS);
      cont();
      textBoxB();
      type(F("'You know what? My best friend’s cousin’s girlfriend’ sister in law’s older brother is a clown fish. He works parties, isn’t that crazy? Clown fish are like sooo rare these days,you know?'  "), 0, yB);
      cont();
      textBoxS();
      type(F("You just stare back at her. A clown fish? What kind of 21st birthday party would have a clown?  "), 0, yS);
      cont();
      textBoxS();
      type(F("Your coworker takes your silence as an invitation to keep talking.  "), 0, yS);
      cont();
      textBoxB();
      type(F(" 'Gosh, I would have so much fun at a party with a clown fish! Like, I bet if I threw a party and had a clown fish people would be talking about it for weeks. It’d be, like, starting a new trend for parties or something. ' "), 0, yB);
      cont();
      textBoxS();
      type(F("Hmm. This is the first time in several weeks that Shrimpina has been able to formulate a thought that is even remotely helpful to you.  "), 0, yS);
      cont();
      textBoxS();
      type(F("You do want this party to be the talk of the town. You might just have to find a way to get yourself a party-performing clown fish.  "), 0, yS);
      cont();
      textBoxS();
      type(F("But for now, the day is over. You’ve added 125 calms to your wallet.  "), 0, yS);
      cont();
      workDays += 1; //increase the counter for how many times you've been to work
  } else if (workDays == 1){//if you've been to work twice
    textBoxS();
    type(F("You decide to go to work again. Your coworker Shrimpina is waiting for you as soon as you clock in.  "), 0, yS);
    cont();
    textBoxS();
    type(F("She stands far too close to you.  "), 0, yS);
    cont();
    textBoxS();
    type(F("'So, how's that party coming?' Shrimpina asks.  "), 0, yS);
    cont();
    textBoxS();
    type(F("You sigh.  "), 0, yS);
    cont();
    textBoxS();
    type(F("You’ve considered her idea about the clown fish, and as unhelpful as Shrimpina is, this one time she’s produced a competent idea.  "), 0, yS);
    cont();
    textBoxS();
    type(F("You swallow your pride and politely ask where one could find a clown fish on such short notice.  "), 0, yS);
    cont();
    textBoxS();
    type(F("'Oh, well don’t worry about that!' Shrimpina says excitedly, placing her fin on your shoulder. "), 0, yS);
    cont();
    textBoxS();
    type(F("Gross. "), 0, yS);
    cont();
    textBoxS();
    type(F("'I don’t know if you remember but my best friend’s cousin’s girlfriend’ sister in law’s older brother is a clown fish– ' "), 0, yS);
    cont();
    textBoxS();
    type(F("You remember.  "), 0, yS);
    cont();
    textBoxS();
    type(F("Obviously.  "), 0, yS);
    cont();
    textBoxS();
    type(F("It’s been less than a week since you last spoke about this. Does she think you’re stupid?  "), 0, yS);
    cont();
    textBoxS();
    type(F("'Well, I know he’d be happy to help. His fee is usually only, like, 50 clams, but I bet you he’d drop the price to, like, 25 since you’re my friend.  "), 0, yS);
    cont();
    textBoxS();
    type(F("Especially if you invite us all to come.  "), 0, yS);
    cont();
    textBoxS();
    type(F("He’d be soooo grateful if me, and my cousin, and her girlfriend, and her sister in law could come to the party.'  "), 0, yS);
    cont();
    textBoxS();
    type(F("Right.  "), 0, yS);
    cont();
    textBoxS();
    type(F("Of course.  "), 0, yS);
    cont();
    textBoxS();
    type(F("Obviously, your coworker has been hanging around you to try and get an invite.  "), 0, yS);
    cont();
    textBoxS();
    type(F("But where else are you going to find a clown fish on such short notice with a fee of only 25 clams?  "), 0, yS);
    cont();
    textBoxS();
    type(F("You relent and invite her.  "), 0, yS);
    cont();
    textBoxS();
    type(F("And her cousin.  "), 0, yS);
    cont();
    textBoxS();
    type(F("And her cousin’s girlfriend.  "), 0, yS);
    cont();
    textBoxS();
    type(F("And the girlfriend’s sister in law.  "), 0, yS);
    cont();
    textBoxS();
    type(F("Shrimpina claps her fins together in excitement and chatters about her party dress for the rest of the workday.  "), 0, yS);
    cont();
    textBoxS();
    type(F("You've obtained: Clownfish!"), 0, yS);
    cont();
    textBoxS();
    type(F("By the time the workday ends, you'e finished all your work for the week. You don't have to clock in again until next week thankfully. You go home. "), 0, yS);
    cont();
    workDays+=1; //increase the counter for how many days you've been at work 
  } else { //if you've been to work more than twice 
    textBoxS();
    type(F("You've already been to work twice already. Your coworkers are confused as to why you keep showing up. "), 0, yS);
    cont();
  }
}

byte stayHome(){
  background(); //draws background
  textBoxS(); 
  type(F("You decide to stay home. Where do you go?\n \nButton 1 ==> City Center\nButton 2 ==> DownTown"), 0, yS);
  while (true){ //gets users chouce 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    
    if (sensorVal == LOW){ //if button 1 is pressed
      return 1; //city center
    } else if (sensorVal2 == LOW){//if button 2 is pressed
      return 2; // downtown
    }
  }
}

byte cityCenter(){
  background(); 
  textBoxS(); 
  type(F("You decide to go to the city center. Where do you go? \nButton 1 ==> Park \nButton 2 ==> Grocery Store"), 0, yS);
  while (true){ 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    
    if (sensorVal == LOW){//if button 1 is pressed 
      return 1; //park
    } else if (sensorVal2 == LOW){//if button 2 is pressed 
      return 2; // grocery store
    }
  }
}

byte downTown(){
  background(); 
  textBoxS(); 
  type(F("You decide to go to the Down Town. Where do you go?\n \nButton 1 ==> Bakery \nButton 2 ==> Party Store"), 0, yS);
  while (true){ 
    byte sensorVal = digitalRead(button1);
    byte sensorVal2 = digitalRead(button2);
    
    if (sensorVal == LOW){
      return 1; //Bakery
    } else if (sensorVal2 == LOW){
      return 2; // party store
    }
  }

}

byte park(){
    background(); 
    textBoxS();
    type(F("You decide to head to the park. "), 0, yS);
    cont();
    textBoxS();
    type(F("Thankfully, it’s a sunny day. "), 0, yS);
    cont();
    textBoxB();
    type(F("You follow a stone path that winds through the park, thinking over your party preparations and all the materials you’ve yet to find. This fish-city is massive, and you’re not sure how you’re supposed to locate all the items you still need. "), 0, yB);
    cont();
    textBoxB();
    type(F("As you’re lost in your thoughts, you pass by several different vendors standing by their little carts, attempting to sell you a multitude of different foods that you surely don’t need. "), 0, yB);
    cont();
    textBoxB();
    type(F("You've made the mistake of buying those fancy candied-nuts once, only to realize be met with the grand disappointment of realizing they will never taste as good as they smell. "), 0, yB);
    cont();
    textBoxS();
    type(F("You won’t be fooled again. "), 0, yS);
    cont();
    textBoxB();
    type(F("Off to the side, in a shadowy corner of the park, a vendor’s cart is illuminated by a hovering spotlight, giving the perfect view of dozen colorful balloons tied to its handle. "), 0, yB);
    cont();
    textBoxS();
    type(F("As you step closer to the cart, you find that the spotlight is attached to a scary-looking fish. "), 0, yS);
    cont();
    textBoxB();
    type(F("He looks like a fish you wouldn’t want to meet in a dark alleyway, but meeting him in a dark park will have to do. After all, you’ve sworn that Clamantha’s 21st ballin’ birthday bash will be the biggest, craziest rager of the year. "), 0, yB);
    cont();
    textBoxS();
    type(F("There simply have to be balloons. "), 0, yS);
    cont();
    textBoxS();
    type(F("As you approach the shadowy fish figure, he looks up to greet you. "), 0, yS);
    cont();
    textBoxS();
    type(F("'Name’s Fish. What do you want?' "), 0, yS);
    cont();
    textBoxB();
    type(F("You politely explain your situation, doing your best to avoid staring at his incredibly pointed teeth. For real, those things look like needles. How does even close his mouth? "), 0, yB);
    cont();
    textBoxS();
    type(F("Thankfully, Fish seems to be a decent guy. He can see that you’re desperate for balloons and offers you a twenty clam discount. "), 0, yS);
    cont();
    textBoxS();
    type(F("You pay the fish 34 clams and even invite him to your party. "), 0, yS);
    cont();
    textBoxS();
    type(F("You've obtained: Balloons !"), 0, yS);
    cont();
    textBoxS();
    type(F("It’s now completely dark outside. Scary. "), 0, yS);
    cont();
    textBoxS();
    type(F("You head home."), 0, yS);
    //gained = gained + "-Balloons\n";  
    cont();
}

byte groceryStore(){
    background(); //draws background
    textBoxS();
    type(F("When walking over to the grocery store, you see a swarm of small fins crowded in front of the store. You ignore it, totally not your business. "), 0, yS);
    cont();
    textBoxS();
    type(F("As you’re one foot into the store already, you hear an 'Excuse me!' from behind you. "), 0, yS);
    cont();
    textBoxS();
    type(F("You reluctantly turn around and see 13 Aquatic Scouts, looking at you desperately with a full table of Aquatic Scout cookies that still need to be sold. "), 0, yS);
    cont();
    textBoxS();
    type(F("ugh. "), 0, yS);
    cont();
    textBoxS();
    type(F("'How are you on this fin-tastic afternoon?' One of them asks excitedly. "), 0, yS);
    cont();
    textBoxS();
    type(F("Terrible now that you've been cornered by nine-year old fish. "), 0, yS);
    cont();
    textBoxS();
    type(F("The kid doesn't wait for you to answer, instead pointing to a table filled with mediocre cookies. "), 0, yS);
    cont();
    textBoxS();
    type(F("'We're selling Aquatic Scout Cookies!' The kid says brightly, 'We're trying to earn our Bass in Business pin!' "), 0, yS);
    cont();
    textBoxB();
    type(F("Whatever. You're about to tell the kids that you aren't intrested in their soliciting, and that trying to peddle cookies at a store front without a permit is illegal, but a thought strikes you. "), 0, yB);
    cont();
    textBoxS();
    type(F("Cookies could be just what you need for Clamantha's party. "), 0, yS);
    cont();
    textBoxS();
    type(F("You fake a smile and offer to buy ten boxes. "), 0, yS);
    cont();
    textBoxS();
    type(F("The scouts cheer, and unfortunately, it's kind of cute. "), 0, yS);
    cont();
    textBoxS();
    type(F("Whatever. Screw them and that Bass in Business badge. "), 0, yS);
    cont();
    textBoxS();
    type(F("You've obtained: Cookies!"), 0, yS); 
    cont();
    textBoxS();
    type(F("You hurry into the store before you can start becoming a better person. "), 0, yS);
    cont();
    textBoxS();
    type(F("Inside, you buy a couple other drinks and snacks before heading home. "), 0, yS);
    cont();
    textBoxS();
    type(F("You've obtained: Snacks and Drinks!"), 0, yS);
    cont();
    textBoxS();
    type(F("When you leave, thankfully the scouts are gone and you can live in peace. "), 0, yS);
    cont();
}

byte bakery(){
  background();
    textBoxS();
    type(F("You open the door to Sharkira's Butter Batch Bakery. The smell of freshly baked pastries invites you in. "), 0, yS);
    cont();
    textBoxS();
    type(F("Sharkira herself greets you with the biggest smile on her face, 'Welcome to my bakery! What brings you in today?' "), 0, yS);
    cont();
    textBoxS();
    type(F("The sun comes through the glass window and hits Sharkira's face in the most perfect way. "), 0, yS);
    cont();
    textBoxS();
    type(F("You speak before you can think: 'Woah, I’m sorry but like, like, like…' God, you're stuttering! Stop! "), 0, yS);
    cont();
    textBoxS();
    type(F("'Oh my goodness, is there something on my face?' "), 0, yS);
    cont();
    textBoxS();
    type(F("Sharkira runs to the closest mirror on the wall and you panic. There’s nothing on her face. "), 0, yS);
    cont();
    textBoxS();
    type(F("'Wait, no! There’s nothing on your face! There’s nothing wrong with you, I’m the problem!' "), 0, yS);
    cont();
    textBoxS();
    type(F("You're the problem?? Great. Now she's going to think you're crazy. "), 0, yS);
    cont();
    textBoxS();
    type(F("'Oh, no don't say that', Sharkira says with a smile. "), 0, yS);
    cont();
    textBoxS();
    type(F("You can’t focus on anything other than Sharkira's gorgeous fins, her gorgeous hair, her gorgeous features. "), 0, yS);
    cont();
    textBoxS();
    type(F("'Do you model?' "), 0, yS);
    cont();
    textBoxS();
    type(F("You totally did not mean to say that out loud. Carp "), 0, yS);
    cont();
    textBoxS();
    type(F("'Excuse me?' "), 0, yS);
    cont();
    textBoxS();
    type(F("'I’m so sorry. Do you have any birthday cakes?' Your cheeks are on fire. "), 0, yS);
    cont();
    textBoxS();
    type(F("Sharkira gracefully ignores your floundering, nodding, 'Yes! All freshly baked too. What kind of cake are you looking for?' "), 0, yS);
    cont();
    textBoxS();
    type(F("'I'm throwing a party for my BFF's birthday. I'll take the best you've got!' "), 0, yS);
    cont();
    textBoxS();
    type(F("'Oh that’s so lovely! My bestest design is this one on display!' "), 0, yS);
    cont();
    textBoxS();
    type(F("She points to a floral cake with gold flakes and diamonds on top. It’s stunning. "), 0, yS);
    cont();
    textBoxS();
    type(F("'It's 600 clams!' She says proudly. "), 0, yS);
    cont();
    textBoxS();
    type(F("600 clams? That's way out of your price range! You can't afford it! "), 0, yS);
    cont();
    textBoxS();
    type(F("'I'll take it!' "), 0, yS);
    cont();
    textBoxS();
    type(F("She’s so beautiful, you’ll do anything to support her bakery. Even if you end up in debt. "), 0, yS);
    cont();
    textBoxS();
    type(F("As she wraps up the cake for you, you somehow manage to stutter out an invitation to Clamantha's party. "), 0, yS);
    cont();
    textBoxS();
    type(F("She accepts, and you leave Sharkira's Butter Batch Bakery in debt but with a full and happy fish heart. "), 0, yS);
    cont();
    textBoxS();
    type(F("You've obtained: Cake!"), 0, yS);
    cont();
    textBoxS();
    type(F("You've obtained: Debt!"), 0, yS);
    cont();
}

byte partyStore(){
    background();
    textBoxS();
    type(F("You enter the empty, gray, dimly lit, slightly scary O-Fish-ial Party Supply Rental Store. "), 0, yS);
    cont();
    textBoxS();
    type(F("You expect to see shelves, or tables, or at least items on the floor. But there's nothing. "), 0, yS);
    cont();
    textBoxS();
    type(F("It's just a room. Concrete walls a floors. A single light hanging from a string above you, flickering in and out. "), 0, yS);
    cont();
    textBoxS();
    type(F("'Good afternoon! How can I help you today?' "), 0, yS);
    cont();
    textBoxB();
    type(F("You quickly turn around to find a store associate has magically appeared. He wears a striped shirt and a name pin that has the store's logo. It reads: Mackerelmore. "), 0, yB);
    cont();
    textBoxS();
    type(F("You don’t question it. You don't have time to quesion it. Clamantha's party is happening any day now. "), 0, yS);
    cont();
    textBoxS();
    type(F("You ask the store keeper if they have party supplies. "), 0, yS);
    cont();
    textBoxS();
    type(F("like "), 0, yS);
    cont();
    textBoxS();
    type(F("like in the name of the store."), 0, yS);
    cont();
    textBoxS();
    type(F("The store associate just randomly starts laughing. Like hysterically. You give them a puzzled look. What’s so funny? "), 0, yS);
    cont();
    textBoxS();
    type(F("'I’m sorry, did I say something wrong?', you ask. "), 0, yS);
    cont();
    textBoxS();
    type(F("'No you didn’t.' They say as they’re still laughing. But now they’re trying to conceal it. "), 0, yS);
    cont();
    textBoxS();
    type(F("Now you're starting to get annoyed.  "), 0, yS);
    cont();
    textBoxS();
    type(F("'So do you guys have any birthday party supplies or not?' you demand. 'And where is everything??' "), 0, yS);
    cont();
    textBoxB();
    type(F("Mackerelmore gives you a look that only customer service service workers can give.  He’s clearly at the end of his patience with the management of the O-Fish-ial Party Supply Rental Store. "), 0, yB);
    cont();
    textBoxS();
    type(F("'We're sold out.' He says. "), 0, yS);
    cont();
    textBoxS();
    type(F("'Of everything. They sold all of our stock. They sold the cash register. The shelves! For the love of Cod, they sold the carpet!' "), 0, yS);
    cont();
    textBoxS();
    type(F("‘Right.’ You answer awkwardly. You take a step back, put-off by the associate’s tirade against his, admittedly, terrible managers. "), 0, yS);
    cont();
    textBoxS();
    type(F("‘Wait, don’t go. We’ve got one thing left.’ "), 0, yS);
    cont();
    textBoxS();
    type(F("Mackerelmore grabs a rope behind him, pulling it forward to reveal a seahorse attached to the other end. It walks forward patiently and neighs at you. "), 0, yS);
    cont();
    textBoxS();
    type(F("‘Please, take my dearest seahorse away from here. He needs a good home. His name is Fin Diesel.’ "), 0, yS);
    cont();
    textBoxS();
    type(F("You’re in the process of shaking your head no, but Mackerelmore continues. ‘He’s a good party-horse. Your party-goers will love him!’  "), 0, yS);
    cont();
    textBoxS();
    type(F("You have to be kidding. What the fin is Clamantha  going to do with a seahorse? She’s turning 21, not 7. "), 0, yS);
    cont();
    textBoxS();
    type(F("‘It’s a grand deal. One seahorse for just 10 clams.’ The associate says, leading Fin Diesel closer to you.  "), 0, yS);
    cont();
    textBoxS();
    type(F("You can't deny a good deal. You take the seahorse. "), 0, yS);
    cont();
    textBoxS();
    type(F("It's almost as if there's someone out there, forcing you to buy things you don't need. Odd. "), 0, yS);
    cont();
    textBoxS();
    type(F("You take the seahorse's reins and lead him home. You aren't sure where to put him. He sleeps in the guest bedroom for now. "), 0, yS);
    cont();
    textBoxS();
    type(F("You've obtained: Seahorse!"), 0, yS);
    //gained = gained + "-A Seahorse\n";
    cont();
}

void partyBad(){
  textBoxS();
  type(F("Today’s the big day! Clamantha’s 21st birthday party! "), 0, yS);
  cont();
  textBoxS();
  type(F("However, you haven't managed to get everything you need for the party."), 0, yS);
  cont();
  textBoxB();
  type(F("You told everyone this was going to be the best part of the year, but without the materials you need, you know your party is going to be terrrible."), 0, yB);
  cont();
  textBoxS();
  type(F("You're too embarassed to show your face. You call off the party."), 0, yS);
  cont();
  textBoxS();
  type(F("Clamantha is so upset that her birthday is ruined. You hide in your house, too ashamed to see her."), 0, yS);
  cont();
  textBoxS();
  type(F("Hopefully she lets you try again next year."), 0, yS);
  cont();
  textBoxS();
  type(F("Bad Ending :("), 0, yS);
  cont();
}

void partyGood(){
  textBoxS();
    type(F("Today’s the big day! Clamantha’s 21st birthday party! "), 0, yS);
    cont();
    textBoxB();
    type(F("After running around these past few days, you’ve managed to get everything you need for the party. The only thing missing is the clownfish Shrimpina is supposed to bring. "), 0, yB);
    cont();
    textBoxS();
    type(F("You’ve spent all day cleaning your house, preparing for the event. "), 0, yS);
    cont();
    textBoxB();
    type(F("People start arriving at 8 pm. You’re ready, everything is ready. Shrimpina, her cousin, her cousin’s girfriend, and her best friend’s cousin’s girlfriend’s sister in law’s older brother, who’s acting as the clownfish, all show up. "), 0, yB);
    cont();
    textBoxS();
    type(F("'Heyyyyy!! We’re all here! Thank you so much for inviting all of us again! We’re gonna have so much fun tonight! Eeeee!' "), 0, yS);
    cont();
    textBoxS();
    type(F("Ugh. She’s so annoying. "), 0, yS);
    cont();
    textBoxS();
    type(F("You remember this is all for Clamantha. So you clam down and practice a few breathing exercises. "), 0, yS);
    cont();
    textBoxS();
    type(F("It’s 9 pm and you see Clamantha’s ride pull up. This is it, you tell everyone to hide and turn off all the lights. "), 0, yS);
    cont();
    tft.fillScreen(RED); //celebration!!
    tft.fillScreen(ORANGE);
    tft.fillScreen(YELLOW);
    tft.fillScreen(GREEN);
    tft.fillScreen(BLUE);
    tft.fillScreen(PURPLE);
    tft.fillScreen(PINK); 
    textBoxS();
    type(F("Clamantha opens the door and everyone yells, 'HAPPY 21ST CLAMANTHA!' "), 0, yS);
    cont();
    textBoxS();
    type(F("Clamantha’s face immediately lights up. This is the happiest you’ve seen her. This warms your little fish-heart. "), 0, yS);
    cont();
    textBoxS();
    type(F("After everyone greets her individually, she runs to find you and says: "), 0, yS);
    cont();
    textBoxS();
    type(F("'This is the best birthday ever. I love you so much. I’m so happy we’re BFFs. Thank you for everything.' "), 0, yS);
    cont();
    textBoxS();
    type(F("Good Ending! :)"), 0, yS);
    cont();
} 
