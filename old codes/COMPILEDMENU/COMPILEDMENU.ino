#include <U8glib.h>
#include <SoftwareSerial.h>
#include <Wire.h>              // include the Wire library for I2C communication
#include "RTClib.h"      // include the RTClib library for the DS3231 RTC module
#include "DFRobotDFPlayerMini.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
int button1 = 9; // button 1 is connected to digital pin SET 42
int button2 = 6; // button 2 is connected to digital pin SET 43
int button3 = 8; // button 3 is connected to digital pin SET 44
int button4 = 9; // button 4 is connected to digital pin SET 45
int button5 = 7; // button 5 is connected to digital pin SET 46
int buttonPressed = 0; // variable to store which button is pressed

//--------------------------------------SONAR----------------------------------//
#define RvibPin 2
#define MvibPin 3
#define LvibPin 4
const int trigPinR = 48;
const int echoPinR = 49;
const int trigPinM = 50; //CHANGE PIN PARA SA SD CARD TO
const int echoPinM = 51; //CHANGE PIN PARA SA SD CARD TO
const int trigPinL = 52; //CHANGE PIN PARA SA SD CARD TO
const int echoPinL = 53;
int durationL;
long distanceL;
int durationM;
long distanceM;
int durationR;
long distanceR;
bool smsSent;
int smsCount = 0; // initialize counter variable to 0

//--------------------------------------LDR----------------------------------//
const int ldr_pin = 31;
int m = 0;
int l = 0;
int r = 0;

//SIM SETUP
SoftwareSerial sim800l(10, 11); // RX, TX
int _timeout;
String _buffer;
String number = "+639473117641"; //-> change with your number
RTC_DS3231 rtc;    



// ------------------------------------- SD CARD ------------------------------------- //
#define RX 10
#define TX 11
SoftwareSerial mySoftwareSerial(RX, TX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  u8g.setRot180(); // Rotate the display 180 degrees
  Serial.begin(9600);
  sim800l.begin(9600);
  // RTC
  Wire.begin();          // initialize I2C communication
  rtc.begin();

  // serial com for DFplayer mp3 mini
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  // Wait for SIM800L module to respond
  if (!checkSIM800L()) {
    Serial.println("SIM800L not responding.");
    while (1);
  }
  Serial.println("TYPE S TO SEND A MESSAGE OR D TO SEND AN ALERT");  \

  // check if SD card is working
  while (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.

    u8g.drawStr((128 - u8g.getStrWidth("SD card fail")) / 2, 30, "SD card fail");
    Serial.println("SD card fail");
    u8g.nextPage();
    
  }
 Serial.println("Working SD Card");
  myDFPlayer.volume(30);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
 
//--------------------------------------SONAR----------------------------------//
  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinR, INPUT); // Sets the echoPin as an Input
  pinMode(RvibPin, OUTPUT);  
  pinMode(trigPinM, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinM, INPUT); // Sets the echoPin as an Input
  pinMode(MvibPin, OUTPUT);
  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinL, INPUT); // Sets the echoPin as an Input
  pinMode(LvibPin, OUTPUT);

//--------------------------------------LDR----------------------------------//
  pinMode(ldr_pin,INPUT);

//--------------------------------------LED----------------------------------//
        for (int LEDpin=22; LEDpin<31; LEDpin++){
    pinMode(LEDpin, OUTPUT);

  }
}

void loop() {
  s:
  buttonPressed=0;
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x10);
    if (digitalRead(button1) == LOW) {
      buttonPressed = 1;

      myDFPlayer.play(1); //Daymode is selected

      while(buttonPressed == 1){
        u8g.drawStr((128 - u8g.getStrWidth("Daymode Selected!")) / 2, 30, "Daymode Selected!");
        Serial.println("Daymode Selected!");
        u8g.nextPage();
        daymode();
                
        if (digitalRead(button5) == LOW){
          goto s;
        }                
      }
    } else if (digitalRead(button2) == LOW) {
      buttonPressed = 2;

      myDFPlayer.play(2);  //Nightmode is selected

      while(buttonPressed == 2){
      u8g.drawStr((128 - u8g.getStrWidth("Nightmode Selected!")) / 2, 30, "Nightmode Selected!");
      Serial.println("Nightmode Selected!");
      u8g.nextPage();
      nightmode();

      if (digitalRead(button5) == LOW){
          goto s;
        }      
      }
    } else if (digitalRead(button3) == LOW) {
      buttonPressed = 3;

      myDFPlayer.play(3);  //Rainmode is selected

      while(buttonPressed == 3){
      u8g.drawStr((128 - u8g.getStrWidth("Rainmode Selected!")) / 2, 30, "Rainmode Selected!");
      Serial.println("Rainmode Selected!");
      u8g.nextPage();
      rainmode();

      if (digitalRead(button5) == LOW){
          goto s;
        }     
      }
    } else if (digitalRead(button4) == LOW) {
      buttonPressed = 4;

      myDFPlayer.play(4);  //SOS is selected

      while(buttonPressed == 4){
      u8g.drawStr((128 - u8g.getStrWidth("SOSmode Selected!")) / 2, 30, "SOSmode Selected!");
      Serial.println("SOSmode Selected!");
      u8g.nextPage();
      SOSmode();

      if (digitalRead(button5) == LOW){
          goto s;
        }    
      }       
    } 
    else if (digitalRead(button1) == LOW) && (digitalRead(button2) == LOW) {
      buttonPressed = 6;

      myDFPlayer.play(6);  //Timecheck is detected

      u8g.drawStr((128 - u8g.getStrWidth("Time Check Mode!")) / 2, 30, "Time Check Mode!");
      Serial.println("Time Check Mode!");
      u8g.nextPage();
      timecheckmode();
      delay(1000);
      goto s;      
    } 
    else {
      u8g.drawStr((128 - u8g.getStrWidth("NO MODE SELECTED")) / 2, 30, "NO MODE SELECTED");
      Serial.println("NO MODE SELECTED!");

      myDFPlayer.play(9);  //No mode is seleted

    }
  } while (u8g.nextPage());
}


void daymode() {
Serial.println("Daymode Selected!");
MainUltraSonic();
LEDHazzardfunctionOFF();
LDRfunction();
}
void nightmode() {
Serial.println("Nightmode Selected!");
MainUltraSonic();
  LEDHazzardfunctionOFF();
  
  while (smsCount < 3) { // loop until 3 SMS messages have been sent
    sms("09473117641", "ALERT: Night Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
    delay(1000); // wait for 1 second between SMS messages
    smsCount++; // increment counter variable
  }
}
void rainmode() {
Serial.println("Rainmode Selected!");
LEDHazzardfunctionON();
  
  while (smsCount < 3) { // loop until 3 SMS messages have been sent
    sms("09473117641", "ALERT: Rain Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
    delay(1000); // wait for 1 second between SMS messages
    smsCount++; // increment counter variable
  }
}
void SOSmode() {
Serial.println("SOSmode Selected!");
LEDHazzardfunctionON();
  sms("09473117641", "ALERT: SOS Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
}
void timecheckmode() {
  Serial.println("TimeCheckmode Selected!");
  rtc.adjust(DateTime(2023, 4, 19, 16, 30, 0));
  DateTime now = rtc.now();
  Serial.println("You Selected Time Check!");
  
    // get the current date and time from the RTC module
  Serial.print("Current date and time: ");
   

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);    // wait for one second before printing the next value
}


// FUNCTIONS OF DAYMODE
void RightUltrasonic(){
// Clears the trigPin
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
   durationR = pulseIn(echoPinR, HIGH);
  // Calculating the distance
   distanceR = durationR * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("R-Distance: ");
  Serial.println(distanceR);  
}
void MidUltrasonic(){
  // Clears the trigPin
  digitalWrite(trigPinM, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinM, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinM, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationM = pulseIn(echoPinM, HIGH);
  // Calculating the distance
  distanceM = durationM * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("M-Distance: ");
  Serial.println(distanceM);     
}

void LeftUltrasonic(){
  // Clears the trigPin
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationL = pulseIn(echoPinL, HIGH);
  // Calculating the distance
  distanceL = durationL * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("L-Distance: ");
  Serial.println(distanceL);  
}

void MainUltraSonic(){
  RightUltrasonic();
  MidUltrasonic();
  LeftUltrasonic();

 if(distanceR <= 50 || distanceM <= 50 || distanceL <= 50){

    myDFPlayer.play(5);  //Obstacle is detected

    digitalWrite(LvibPin, HIGH);
    digitalWrite(MvibPin, HIGH);
    digitalWrite(RvibPin, HIGH);    


    }
  else{
    digitalWrite(LvibPin, LOW);
    digitalWrite(MvibPin, LOW);
    digitalWrite(RvibPin, LOW);

  }
}

void LDRfunction(){
   if( digitalRead( ldr_pin ) == 1){
      Serial.println("Dark Place"); // Voice wull notif that it is on the dark area
      myDFPlayer.play(8);  //Darkplace place

   }
   else{
       Serial.println("Light Place"); // Voice wull notif that it is on the ligher area
       myDFPlayer.play(7);  //Light place
   }
}

void LEDHazzardfunctionOFF(){
  
  for (int i=22; i<31; i++){
  digitalWrite(i, LOW);
}
}
void LEDHazzardfunctionON(){
  
  int t=15;
  for (int i=22; i<31; i++){
  digitalWrite(i, HIGH);
  delay(t);
  digitalWrite(i+1, HIGH);
  delay(t);
  digitalWrite(i+2, HIGH);
  delay(t);
  digitalWrite(i, LOW);
  delay(t);
  digitalWrite(i+2, LOW);
}

for(int i=30; i>21; i--){
  digitalWrite(i, HIGH);
  delay(t);
  digitalWrite(i-1, HIGH);
  delay(t);
  digitalWrite(i-2, HIGH);
  delay(t);
  digitalWrite(i, LOW);
  delay(t);
  digitalWrite(i-1, LOW);
}
}
// Function to send an SMS message
void sms(String phone_number, String message) {
  // Send AT command to set the SMS mode to text
  sim800l.println("AT+CMGF=1");

  delay(1000); // Wait for SIM800L module to respond

  // Send AT command to set the phone number to send the message to
  sim800l.print("AT+CMGS=\"");
  sim800l.print(phone_number);
  sim800l.println("\"");

  delay(150); // Wait for SIM800L module to respond

  // Send the message
  sim800l.print(message);

  delay(200); // Wait for SIM800L module to respond

  // Send Ctrl+Z to end the message
  sim800l.write(26);

  delay(500); // Wait for SIM800L module to respond
  Serial.println("SMS SENT!");
}

// Function to check if SIM800L module is responding
bool checkSIM800L() {
  sim800l.println("AT");
  delay(1000);
  if (sim800l.available()) {
    String response = sim800l.readString();
    if (response.indexOf("OK") != -1) {
      return true;
    }
  }
  return false;
}
















