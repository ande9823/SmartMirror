#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
// include the library code:
//LCD display
#include <LiquidCrystal.h>
//8x8 Matix
#include <LedControlMS.h>
//Temp/Hum Sensor
#include <dht11.h>
#define DHT11PIN 7

//the initial wifi status
int status = WL_IDLE_STATUS;

//WiFiUDP object used for the communication
WiFiUDP Udp;

//-----
//your network name (SSID) and password (WPA)
char ssid[] = "AndroidAP";    //Name of the internet connection        
char pass[] = "sss22wbx"; //Password of the internet connection

//local port to listen on
int localPort = 3002;                               

//IP and port for the server
IPAddress serverIPAddress(192, 168, 43, 63); //IP address can be seen when running the node js server file (index.js) in the commando prompt. Write the IP address with comma's (,) instead of dot (.)
int serverPort = 3001;       
//-----


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//potentiometer
int potPin = A0;
int potValue;

//LED
int LEDPin = 6;

//Millis setup
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

const unsigned long millisDelay = 2000;  //the value is a number of milliseconds

dht11 DHT11;
bool writeTemp = true;

//Now we need a LedControl to work with.
//***** These pin numbers will probably not work with your hardware *****
//pin 10 is connected to the DataIn
//pin 8 is connected to the CLK
//pin 9 is connected to LOAD (CS)
//We have only a single MAX72XX.

#define NBR_MTX 5
LedControl lc=LedControl(10,8,9, NBR_MTX);

// we always wait a bit between updates of the display
String scrollString = "Setup    ";
int stringLength=scrollString.length();
char ch0, ch1, ch2, ch3, ch4;
int nextCharIndex=0;

//setup: runs only once
void setup() {
  //set pinModes
  pinMode(potPin, INPUT);
  pinMode(LEDPin, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //Setup Millis()
  startMillis = millis(); //initial start time
  
  //begin serial and await serial
  Serial.begin(9600);
  while (!Serial);

  //check the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    
    //don't continue
    while (true);
  }

  //attempt to connect to WiFi network
  while (status != WL_CONNECTED) {

    //connect to WPA/WPA2 network
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    //wait 10 seconds for connection
    delay(10000);
  }
  
  Serial.println("Connected to WiFi");
  
  //if you get a connection, report back via serial:
  Udp.begin(localPort);

  //Setup for LED matrices.
  for (int i=0; i< NBR_MTX; i++){
    lc.shutdown(i,false);
    //Set the brightness to a low value
    lc.setIntensity(i,1);
    //and clear the display 
    lc.clearDisplay(i);
  }
  delay(100);
  lc.clearAll();
}

//loop: runs forever
void loop() {
  readPotentiometer();

  //Temp/Hum Sensor
  if(writeTemp == true) {
    writeTemp = false;
    DisplayTemp();
  }
  
  //Listen for a UDP message
  listenForUDPMessage();
  delay(100);

  //Setup of chars for the 8x8 matrix for scrolling the text.
  lc.displayChar(0, lc.getCharArrayPosition(ch0));
  lc.displayChar(1, lc.getCharArrayPosition(ch1));
  lc.displayChar(2, lc.getCharArrayPosition(ch2));
  lc.displayChar(3, lc.getCharArrayPosition(ch3));
  lc.displayChar(4, lc.getCharArrayPosition(ch4));
  ch0=ch1;
  ch1=ch2;
  ch2=ch3;
  ch3=ch4;
  ch4=scrollString[nextCharIndex++];
  if (nextCharIndex>=stringLength) nextCharIndex=0;
  delay(300);
  lc.clearAll();
  delay(25);
}

//listens for incoming UDP messages
void listenForUDPMessage() {

  //on package received
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    
    //buffer for incoming packages
    char packetBuffer[256]; 
  
    //read the packet into packetBufffer
    int msgLength = Udp.read(packetBuffer, 255);
    if (msgLength > 0) {
      packetBuffer[msgLength] = NULL;
    }

    //print message from packet
    Serial.print("\nReceived message: ");
    Serial.println(packetBuffer);

    //lcd update (Need to do something with scrolling or set text on multiple lines)
    lcd.clear();
    lcd.write(packetBuffer);

    // we always wait a bit between updates of the display
    scrollString = String(packetBuffer);
    stringLength=scrollString.length();
    nextCharIndex=0;

    delay(100);
    lc.clearAll();
    ch0= scrollString[0];
    ch1= scrollString[1];
    ch2= scrollString[2];
    ch3= scrollString[3];
    ch4= scrollString[4];
    nextCharIndex=5;
    
    //convert message value to int
    int messageValueAsInt = atoi(packetBuffer);

    //set LED to received value
    setLEDTo(messageValueAsInt);

    //send acknowledgement message
    //sendUDPMessage(Udp.remoteIP(), Udp.remotePort(), "ARDUINO: message was received");
  }
}

//sends a message to the server (UDP)
void sendUDPMessage(IPAddress remoteIPAddress, int remoteport, String message) {
  Serial.println("sendUDPMessageToServer");

  //get message string length (+1 to store a null value indicating the end of the message)
  int messageLength = message.length() + 1;
  
  //create char array 
  char messageBuffer[messageLength];

  //copy string message to char array
  message.toCharArray(messageBuffer, messageLength);

  //send the packet to the server
  Udp.beginPacket(remoteIPAddress, remoteport);
  Udp.write(messageBuffer);
  Udp.endPacket(); 
}

void DisplayTemp(){
  startMillis = currentMillis;
  lcd.clear();
  int chk = DHT11.read(DHT11PIN);
  
  char humString[4];
  itoa(DHT11.humidity, humString, 8);
  //char humArray[32] = "Hummidity   (%): ";
  char humArray[32] = "Hum (%): ";
  strcat(humArray, humString);
  lcd.write(humArray);

  currentMillis = millis(); //milliseconds since the program started
  if (currentMillis - startMillis >= millisDelay){ //test whether the period has elapsed
    lcd.clear();
    
    char tempString[4];
    itoa(DHT11.temperature, tempString, 8);
    //char tempArray[32] = "Temperature   (C): ";
    char tempArray[32] = "Temp (C): ";
    strcat(tempArray, tempString);
    lcd.write(tempArray);

    startMillis = currentMillis; //Resets the delay time
  }

  //Uses an extra delay
  currentMillis = millis();
  if (currentMillis - startMillis >= millisDelay){ 
    writeTemp = true;
    startMillis = currentMillis;
  }
  char tempHumArray[64] = "";
  strcat(tempHumArray, humArray);
  strcat(tempHumArray, ",");
  strcat(tempHumArray, tempArray);
  
  sendUDPMessage(serverIPAddress, serverPort, String(tempHumArray));
}

void readPotentiometer() {
  int currentPotValue = analogRead(potPin);
  
  if(potValue != currentPotValue) {
    potValue = currentPotValue;
    
    Serial.print("\npot val : ");
    Serial.println(String(potValue));
    
    sendUDPMessage(serverIPAddress, serverPort, String(potValue));
  }
}

void setLEDTo(int LEDValue) {
  Serial.print("set LED to: ");
  Serial.println(LEDValue);
  digitalWrite(LEDPin, LEDValue);
}
