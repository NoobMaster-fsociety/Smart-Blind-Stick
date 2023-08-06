#include <U8glib.h>
#include <SoftwareSerial.h>
#include <Wire.h>              // include the Wire library for I2C communication
#include "RTClib.h"      // include the RTClib library for the DS3231 RTC module
#include "DFRobotDFPlayerMini.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
int button1 = 42; // button 1 is connected to digital pin SET 42
int button2 = 43; // button 2 is connected to digital pin SET 43
int button3 = 44; // button 3 is connected to digital pin SET 44
int button4 = 45; // button 4 is connected to digital pin SET 45
int button5 = 46; // button 5 is connected to digital pin SET 46
int buttonPressed = 0; // variable to store which button is pressed

//--------------------------------------SONAR----------------------------------//
#define RvibPin 13
#define MvibPin 9 // palitan ng ibang pin
#define LvibPin 12
const int trigPinR = 34;
const int echoPinR = 35;
const int trigPinM = 36; //CHANGE PIN PARA SA SD CARD TO
const int echoPinM = 37; //CHANGE PIN PARA SA SD CARD TO
const int trigPinL = 38; //CHANGE PIN PARA SA SD CARD TO
const int echoPinL = 39;
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
SoftwareSerial sim800l(5, 6); // RX, TX

RTC_DS3231 rtc;   

// -------------------------------- SD CARD READER ------------------------------ //
#define TX 10
#define RX 11

SoftwareSerial mySoftwareSerial(TX, RX); // TX, RX
DFRobotDFPlayerMini myDFPlayer;

bool checkOnce = false;

void setup() {
  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  u8g.setRot180(); // Rotate the display 180 degrees

  
  Serial.begin(115200);

  sim800l.begin(115200);
  // RTC
  Wire.begin();          // initialize I2C communication
  rtc.begin();  
  // Wait for SIM800L module to respond
  while (!checkSIM800L()) {
    Serial.println("SIM800L not responding.");
}
Serial.println("TYPE S TO SEND A MESSAGE OR D TO SEND AN ALERT");


  

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


// --------------------------------------- SD CARD READER -------------------------------- //
mySoftwareSerial.begin(9600);
  while (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("SD CARD FAIL"));    
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  
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
  delay(3000);
  
  u8g.firstPage();
  do {
    u8g.drawStr((128 - u8g.getStrWidth("Daymode Selected!")) / 2, 30, "Daymode Selected!");
  } while ( u8g.nextPage() );
  checkOnce = false;
  while(buttonPressed == 1){
    Serial.println("Daymode Selected!");
    daymode();
    if (digitalRead(button5) == LOW){

      myDFPlayer.play(9);  //No mode is seleted
      delay(3000);

      goto s;
    } 
    while (digitalRead(button1) == LOW){
      Serial.println("TimeCheck!");
      timecheckmode();
    }                
  }
}

  else if (digitalRead(button2) == LOW) {
  buttonPressed = 2;
  
myDFPlayer.play(2);  //Nightmode is selected  
  
  u8g.firstPage();
  do {
    u8g.drawStr((128 - u8g.getStrWidth("Nightmode Selected!")) / 2, 30, "Nightmode Selected!");
  } while ( u8g.nextPage() );
  
  while(buttonPressed == 2){
    Serial.println("Nightmode Selected!");
    nightmode();
    if (digitalRead(button5) == LOW){

      myDFPlayer.play(9);  //No mode is seleted
      delay(3000);


      goto s;
    }    
    while (digitalRead(button2) == LOW){
      Serial.println("TimeCheck!");
      timecheckmode();
    }              
  }
} else if (digitalRead(button3) == LOW) {
  buttonPressed = 3;

  myDFPlayer.play(3);  //Rainmode is selected  
  
  u8g.firstPage();
  do {
    u8g.drawStr((128 - u8g.getStrWidth("Rainmode Selected!")) / 2, 30, "Rainmode Selected!");
  } while ( u8g.nextPage() );
  
  while(buttonPressed == 3){
    Serial.println("Rainmode Selected!");
    rainmode();
    if (digitalRead(button5) == LOW){

  myDFPlayer.play(9);  //No mode is seleted
  delay(3000);
      
      
      goto s;
    }    
    while (digitalRead(button3) == LOW){
      Serial.println("TimeCheck!");
      timecheckmode();
    }              
  }
} else if (digitalRead(button4) == LOW) {
  buttonPressed = 4;
  
myDFPlayer.play(4);  //SOS is selected  
  
  u8g.firstPage();
  do {
    u8g.drawStr((128 - u8g.getStrWidth("SOSmode Selected!")) / 2, 30, "SOSmode Selected!");
  } while ( u8g.nextPage() );
  
  while(buttonPressed == 4){
    Serial.println("SOSmode Selected!");
    SOSmode();
    if (digitalRead(button5) == LOW){

      myDFPlayer.play(9);  //No mode is seleted
      delay(3000);
      
      goto s;
    }   
    while (digitalRead(button4) == LOW){
      Serial.println("TimeCheck!");
      timecheckmode();
    }               
  }
}

    else {
            
//  myDFPlayer.play(9);
// delay 
       
      u8g.drawStr((128 - u8g.getStrWidth("NO MODE SELECTED")) / 2, 30, "NO MODE SELECTED");
      Serial.println("NO MODE SELECTED!");
    }
  } while (u8g.nextPage());
}

void daymode() {
  Serial.println("Daymode Selected!");
  MainUltraSonic();
  LEDHazzardfunctionOFF();


  if (!checkOnce) {
    checkOnce = LDRfunction();
  }
  
}
void nightmode() {
Serial.println("Nightmode Selected!");
MainUltraSonic();
  LEDHazzardfunctionOFF();
  
  while (smsCount < 10) { // loop until 3 SMS messages have been sent
    sms("09951594526", "ALERT: Night Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
    delay(1000); // wait for 1 second between SMS messages
    smsCount++; // increment counter variable
  }
}
void rainmode() {
Serial.println("Rainmode Selected!");
LEDHazzardfunctionON();
  
  while (smsCount < 3) { // loop until 3 SMS messages have been sent
    sms("09951594526", "ALERT: Rain Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
    delay(1000); // wait for 1 second between SMS messages
    smsCount++; // increment counter variable
  }
}
void SOSmode() {
Serial.println("SOSmode Selected!");
LEDHazzardfunctionON();
  sms("09951594526", "ALERT: SOS Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number

  myDFPlayer.play(10);  //No mode is seleted
  delay(3000);  
  
}
void timecheckmode() {
  Serial.println("TimeCheckmode Selected!");
  rtc.adjust(DateTime(2023, 4, 19, 18, 30, 0));
  DateTime now = rtc.now();
  Serial.println("You Selected Time Check!");
  
    // get the current date and time from the RTC module
  Serial.print("Current date and time: ");
  
  if (now.hour() >= 8 && now.hour() < 9) {
    myDFPlayer.playMp3Folder(1); //8am
    delay(6000);
  }

  if (now.hour() >= 9 && now.hour() < 10) {
    myDFPlayer.playMp3Folder(2); //9am
    delay(6000);
  }

  if (now.hour() >= 10 && now.hour() < 11) {
    myDFPlayer.playMp3Folder(3); //10am
    delay(6000);
  }

  if (now.hour() >= 11 && now.hour() < 12) {
    myDFPlayer.playMp3Folder(4); //11am
    delay(6000);
  }

  if (now.hour() >= 12 && now.hour() < 1+12) {
    myDFPlayer.playMp3Folder(5); //12pm
    delay(6000);
  }

  if (now.hour() >= 1+12 && now.hour() < 2+12) {
    myDFPlayer.playMp3Folder(6); //1pm
    delay(6000);
  }

  if (now.hour() >= 2+12 && now.hour() < 3+12) {
    myDFPlayer.playMp3Folder(7); //2pm
    delay(6000);
  }

  if (now.hour() >= 3+12 && now.hour() < 4+12) {
    myDFPlayer.playMp3Folder(8); //3pm
    delay(6000);
  }

  if (now.hour() >= 4+12 && now.hour() < 5+12) {
    myDFPlayer.playMp3Folder(9); //4pm
    delay(6000);
  }

  if (now.hour() >= 5+12 && now.hour() < 6+12) {
    myDFPlayer.playMp3Folder(10); //5pm
    delay(6000);
  }

  if (now.hour() >= 6+12 && now.hour() < 7+12) {
    myDFPlayer.playMp3Folder(11); //6pm
    delay(6000);
  }

  if (now.hour() >= 7+12 && now.hour() < 8+12) {
    myDFPlayer.playMp3Folder(12); //7pm
    delay(6000);
  }

  if (now.hour() >= 8+12 && now.hour() < 9+12) {
    myDFPlayer.playMp3Folder(13); //8pm
    delay(6000);
  }

  if (now.hour() >= 9+12 && now.hour() < 10+12) {
    myDFPlayer.playMp3Folder(14); //9pm
    delay(6000);
  }
  

  
   
  //  if (now.hour() >= 8) {
  // if ((now.hour() - 8) % 1 == 0) {
  //   // Code to be executed every hour starting at 8am
  // }
// }





//if (now.hour() <= 8){ }


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

  //delay(1000);    // wait for one second before printing the next value
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


    digitalWrite(LvibPin, HIGH);
    digitalWrite(MvibPin, HIGH);
    digitalWrite(RvibPin, HIGH);    

    myDFPlayer.play(5);  //Obstacle is detected
    delay(6000);

    }
  else{
    digitalWrite(LvibPin, LOW);
    digitalWrite(MvibPin, LOW);
    digitalWrite(RvibPin, LOW);

  }
}

bool LDRfunction(){
   if( digitalRead( ldr_pin ) == 1){
      Serial.println("Dark Place"); // Voice wull notif that it is on the dark area
      myDFPlayer.play(8);  //Darkplace place
      delay(6000); 
   }
   else{
       Serial.println("Light Place"); // Voice wull notif that it is on the ligher area
       myDFPlayer.play(7);  //Light place
      delay(6000);
      
   }

   return true;
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
