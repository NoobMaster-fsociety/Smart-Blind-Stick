#include <U8glib.h>
#include <SoftwareSerial.h>
#include <Wire.h>              // include the Wire library for I2C communication
#include "RTClib.h"      // include the RTClib library for the DS3231 RTC module
#include "DFRobotDFPlayerMini.h"
#include <String.h>

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
const int trigPinM = 38; //CHANGE PIN PARA SA SD CARD TO
const int echoPinM = 39; //CHANGE PIN PARA SA SD CARD TO
const int trigPinL = 36; //CHANGE PIN PARA SA SD CARD TO
const int echoPinL = 37;
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
// Configure the SIM900A software serial port
SoftwareSerial sim900a(18, 19);  // RX, TX

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
  // u8g.setRot180(); // Rotate the display 180 degrees

  
  Serial.begin(115200);

  // RTC
  Wire.begin();          // initialize I2C communication
  rtc.begin();  
  sim900a.begin(9600);        // Start the software serial port for SIM900A
  delay(1000);
  // while (!gsmCheck()) {
  //   Serial.println("GSM module is not responding. Retrying...");
  //   delay(1000);
  // }
  // Serial.println("GSM module is responding");


  

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
 bool checkBright = false;
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

    checkBright = false;
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
  delay(3000);
    
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
      digitalWrite(LvibPin, LOW);
      digitalWrite(MvibPin, LOW);
      digitalWrite(RvibPin, LOW);
      if (sim900a.available()) {
     String message = sim900a.readString();  // Read the incoming message
     Serial.println("Received message: " + message);
  }
    }
  } while (u8g.nextPage());
}

void daymode() {
  Serial.println("Daymode Selected!");
  MainUltraSonic();
  LEDHazzardfunctionOFF();
  ldrnight();
  // ldrday();
  // if (!checkOnce) {
  //   checkOnce = LDRfunction();
  // }
  
}
void nightmode() {
Serial.println("Nightmode Selected!");
MainUltraSonic();
  LEDHazzardfunctionON();
  ldrday();
}
void rainmode() {
Serial.println("Rainmode Selected!");
LEDHazzardfunctionON();
  
  sendsmsrain();
  digitalWrite(LvibPin, LOW);
  digitalWrite(MvibPin, LOW);
  digitalWrite(RvibPin, LOW);

  myDFPlayer.play(11);  //No mode is seleted
  delay(3000);

  
}
void SOSmode() {
Serial.println("SOSmode Selected!");
LEDHazzardfunctionON();
  sendsmssos();
  myDFPlayer.play(10);  //No mode is seleted
  delay(3000);  
  digitalWrite(LvibPin, LOW);
  digitalWrite(MvibPin, LOW);
  digitalWrite(RvibPin, LOW);
  
}
void timecheckmode() {
  Serial.println("TimeCheckmode Selected!");
  //rtc.adjust(DateTime(2023, 4, 19, 21, 30, 0)); // CHANGE IN TIMECHECK
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
long RightUltrasonic(){
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

   long distancesR = durationM * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("R-Distance: ");
  Serial.println(distancesR);  

  return distancesR;
}

long MidUltrasonic(){
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

  long distancesM = durationM * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("M-Distance: ");
  Serial.println(distancesM);   

  return distancesM;
}

long LeftUltrasonic(){
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

  long distancesL = durationL * 0.034 / 2;
  Serial.print("L-Distance: ");
  Serial.println(distancesL); 

  return distancesL;
}

void MainUltraSonic(){
  // RightUltrasonic();
  // MidUltrasonic();
  // LeftUltrasonic();

// DITO NTYO ADJUST YUNG DISTANCE SHIT
 if(RightUltrasonic() <= 60 || MidUltrasonic() <= 60 || LeftUltrasonic() <= 60){


    digitalWrite(LvibPin, HIGH);
    digitalWrite(MvibPin, HIGH);
    digitalWrite(RvibPin, HIGH);    

// DITO NTYO ADJUST YUNG DISTANCE SHIT PARA MAG OBSTACLE SHIT DETECTED SHIT
    if (MidUltrasonic() <= 40){
      Serial.println("MAY OBSTACLES");
      myDFPlayer.play(5);  //Obstacle is detected
      delay(6000); 
    }
    }
  else{
    digitalWrite(LvibPin, LOW);
    digitalWrite(MvibPin, LOW);
    digitalWrite(RvibPin, LOW);

  }

}

bool LDRfunction(){
   if( digitalRead( ldr_pin ) == HIGH){
      Serial.println("Light Place"); // Voice wull notif that it is on the dark area
      myDFPlayer.play(8);  //Light place
      delay(6000); 
   }
   else{
       Serial.println("Dark Place"); // Voice wull notif that it is on the ligher area
       myDFPlayer.play(7);  //Light place
      delay(6000);
      
   }

   return true;
}

void ldrday(){
    if( digitalRead( ldr_pin ) == LOW){
      Serial.println("LIGHT PLACE"); // Voice wull notif that it is on the dark area
      if (!checkBright)      
        {
          myDFPlayer.play(7);  //Dark Place
          delay(6000); 

          checkBright = true;
        }      
   }
}

void ldrnight(){
    if( digitalRead( ldr_pin ) == HIGH){
      Serial.println("DARK Place"); // Voice wull notif that it is on the dark area

            if (!checkBright)      
        {
            myDFPlayer.play(8);  //Dark Place
      delay(6000); 

          checkBright = true;
        }    

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
// // Function to send an SMS message
// void sendSMS(String phone_number, String message) {
//   gsmSerial.println("AT+CMGF=1"); // set message format to text mode
//   delay(1000);
//   gsmSerial.print("AT+CMGS=\""); // start message sending
//   gsmSerial.print(phone_number);
//   gsmSerial.println("\"");
//   delay(1000);
//   gsmSerial.print(message);
//   delay(1000);
//   gsmSerial.write(26); // end message sending by sending Ctrl+Z
//   delay(1000);
// }

// // Function to check if SIM800L module is responding
// bool gsmCheck() {
//   gsmSerial.println("AT");
//   delay(1000);
//   if (gsmSerial.available()) {
//     String response = gsmSerial.readString();
//     if (response.indexOf("OK") != -1) {
//       return true;
//     }
//   }
//   return false;
// }
void sendsmsnight(){
// Send a test message to a phone number
  sim900a.println("AT+CMGF=1");  // Set the SMS mode to text mode
  delay(1000);
  sim900a.println("AT+CMGS=\"+639951594526\"");  // Replace with your own phone number
  delay(1000);
  sim900a.println("Alert: Nightmode Activated");  // Replace with your own message
  delay(1000);
  sim900a.write(26);  // Send the Ctrl+Z character to indicate the end of the message
  delay(1000);

  Serial.println("Test message sent.");
  delay(1000);  
}
void sendsmsrain(){
// Send a test message to a phone number
  sim900a.println("AT+CMGF=1");  // Set the SMS mode to text mode
  delay(1000);
  sim900a.println("AT+CMGS=\"+639951594526\"");  // Replace with your own phone number
  delay(1000);
  sim900a.println("Alert: Rain Mode Activated.");  // Replace with your own message
  delay(1000);
  sim900a.write(26);  // Send the Ctrl+Z character to indicate the end of the message
  delay(1000);

  Serial.println("Test message sent.");
  delay(1000);  
}
void sendsmssos(){
// Send a test message to a phone number
  sim900a.println("AT+CMGF=1");  // Set the SMS mode to text mode
  delay(1000);
  sim900a.println("AT+CMGS=\"+639951594526\"");  // Replace with your own phone number
  delay(1000);
  sim900a.println("SOS Activated check location.");  // Replace with your own message
  delay(1000);
  sim900a.write(26);  // Send the Ctrl+Z character to indicate the end of the message
  delay(1000);

  Serial.println("Test message sent.");
  delay(1000);    
}
