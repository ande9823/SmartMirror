/*
//Mario's Ideas
//MAX7219 - using Led Control library to display 8x8   bitmap
#include <LedControl.h>

int DIN = 10;
int CS = 9;
int CLK   = 8;
LedControl lc=LedControl(DIN, CLK, CS,0);

int Cat[8] ={B10000111,B01101010,B11111110,B11101010,B11100111,B11000000,B10000000,B01111000};
byte Apple[8] ={B00111000,B01111100,B11111100,B11111101,B10111011,B10111100,B01111100,B00111000};

void   setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
}
   
void loop(){
  for(int i=0;i<8;i++) lc.setRow(0,i,Apple[i]);
  delay(5000);
   lc.clearDisplay(0);
  for(int i=0;i<8;i++) lc.setRow(0,i,Cat[i]);
  delay(5000);
   lc.clearDisplay(0);
}
*/

//We always have to include the library
#include <LedControlMS.h>

//Now we need a LedControl to work with.
//***** These pin numbers will probably not work with your hardware *****
//pin 10 is connected to the DataIn
//pin 8 is connected to the CLK
//pin 9 is connected to LOAD
//We have only a single MAX72XX.


//Number of matrices connected
#define NBR_MTX 5
LedControl lc=LedControl(10,8,9, NBR_MTX);


// we always wait a bit between updates of the display
String scrollString= "Setup    ";
int stringLength=scrollString.length();
char ch0, ch1, ch2, ch3;
int nextCharIndex=0;


void setup() {
   //The MAX72XX is in power-saving mode on startup,
   //we have to do a wakeup call
   
  Serial.begin (9600);
  Serial.println("Setup");
  Serial.println(scrollString);
  Serial.println(stringLength);

  for (int i=0; i< NBR_MTX; i++){
    lc.shutdown(i,false);
   //Set the brightness to a low value
    lc.setIntensity(i,1);
   //and clear the display 
    lc.clearDisplay(i);
  }
  delay(100);
  lc.clearAll();
  ch0= scrollString[0];
  ch1= scrollString[1];
  ch2= scrollString[2];
  ch3= scrollString[3];
  nextCharIndex=4;
}

void loop(){
  lc.displayChar(0, lc.getCharArrayPosition(ch0));
  lc.displayChar(1, lc.getCharArrayPosition(ch1));
  lc.displayChar(2, lc.getCharArrayPosition(ch2));
  lc.displayChar(3, lc.getCharArrayPosition(ch3));
  ch0=ch1;
  ch1=ch2;
  ch2=ch3;
  ch3=scrollString[nextCharIndex++];
  if (nextCharIndex>=stringLength) nextCharIndex=0;
  delay(300);
  lc.clearAll();
  delay(25);
}

/*
//We always have to include the library
#include <LedControlMS.h>

//Now we need a LedControl to work with.
//***** These pin numbers will probably not work with your hardware *****
//pin 10 is connected to the DataIn
//pin 8 is connected to the CLK
//pin 9 is connected to LOAD
//We have only a single MAX72XX.

#define NBR_MTX 2
LedControl lc=LedControl(10,8,9, NBR_MTX);


// we always wait a bit between updates of the display
String scrollString= "MAKESPACE MADRID    ";
int stringLength=scrollString.length();
char ch0, ch1, ch2, ch3;
int nextCharIndex=0;


void setup() {
   //The MAX72XX is in power-saving mode on startup,
   //we have to do a wakeup call
   
  Serial.begin (9600);
  
  for (int i=0; i< NBR_MTX; i++){
    lc.shutdown(i,false);
   //Set the brightness to a low value
    lc.setIntensity(i,1);
   //and clear the display 
    lc.clearDisplay(i);
  }
  delay(100);
  lc.clearAll();
  ch0= scrollString[0];
  ch1= scrollString[1];
  ch2= scrollString[2];
  ch3= scrollString[3];
  nextCharIndex=4;
}

void loop(){
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      String string = (Serial.readString());
      char charArray[64]; 
      string.toCharArray(charArray, 64);
      Serial.write(charArray);
      //DisplayString(string);
      String scrollString = string;
      int stringLength=scrollString.length();
    }
  }
  lc.clearAll();
  ch0= scrollString[0];
  ch1= scrollString[1];
  ch2= scrollString[2];
  ch3= scrollString[3];
  nextCharIndex=4;
  
  lc.displayChar(0, lc.getCharArrayPosition(ch0));
  lc.displayChar(1, lc.getCharArrayPosition(ch1));
  lc.displayChar(2, lc.getCharArrayPosition(ch2));
  lc.displayChar(3, lc.getCharArrayPosition(ch3));
  ch0=ch1;
  ch1=ch2;
  ch2=ch3;
  ch3=scrollString[nextCharIndex++];
  if (nextCharIndex>=stringLength) nextCharIndex=0;
  delay(300);
  lc.clearAll();
  delay(25);
    
    delay(1000);
  
}

void DisplayString(String string){
  String scrollString = string;
  int stringLength=scrollString.length();

  lc.clearAll();
  ch0= scrollString[0];
  ch1= scrollString[1];
  ch2= scrollString[2];
  ch3= scrollString[3];
  nextCharIndex=4;
  
  lc.displayChar(0, lc.getCharArrayPosition(ch0));
  lc.displayChar(1, lc.getCharArrayPosition(ch1));
  lc.displayChar(2, lc.getCharArrayPosition(ch2));
  lc.displayChar(3, lc.getCharArrayPosition(ch3));
  ch0=ch1;
  ch1=ch2;
  ch2=ch3;
  ch3=scrollString[nextCharIndex++];
  if (nextCharIndex>=stringLength) nextCharIndex=0;
  delay(300);
  lc.clearAll();
  delay(25);
}
*/
