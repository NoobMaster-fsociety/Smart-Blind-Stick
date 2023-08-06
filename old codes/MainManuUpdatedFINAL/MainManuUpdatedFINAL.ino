#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>

// tanggal

#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>

#include "U8glib.h"
#include <ClickEncoder.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <Wire.h>              // include the Wire library for I2C communication
#include "RTClib.h"      // include the RTClib library for the DS3231 RTC module

// pang SD card at Speaker
// #define SD_ChipSelectPin 7
// #define Speaker 6
// TMRpcm tmrpcm;
//-----------------------------------Rotary
#define ENCODER_PINA A0
#define ENCODER_PINB A1
#define ENCODER_BTN A2

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



RTC_DS3231 rtc;     
SoftwareSerial sim800l(10, 11); // RX, TX
//--------------------------------------LDR----------------------------------//
const int ldr_pin = 31;
int m = 0;
int l = 0;
int r = 0;

//SIM SETUP
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+639473117641"; //-> change with your number

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST); // Fast I2C / TWI

// all the arrays below are generated from images using image2cpp website
// scroll down to see the actual code

// ' Day_Mode', 16x16px
const unsigned char bitmap__Day_Mode [] PROGMEM = {
	0x00, 0x00, 0x01, 0x80, 0x2b, 0xd4, 0x18, 0x18, 0x33, 0xcc, 0x07, 0xe0, 0x0f, 0xf0, 0x2f, 0xf4, 
	0x6f, 0xf6, 0x2f, 0xf4, 0x0f, 0xf0, 0x07, 0xe0, 0x33, 0xcc, 0x18, 0x18, 0x2b, 0xd4, 0x01, 0x80
};
// ' Night_Mode', 16x16px
const unsigned char bitmap__Night_Mode [] PROGMEM = {
	0x00, 0x05, 0x20, 0x02, 0x07, 0xe5, 0x4f, 0xf0, 0x1f, 0x0a, 0x3e, 0x00, 0x3c, 0x40, 0x3c, 0x04, 
	0x3c, 0x00, 0x3e, 0x00, 0x1f, 0x08, 0x0f, 0xf0, 0x27, 0xe0, 0x70, 0x02, 0x20, 0x08, 0x00, 0x00
};
// ' RainMode_Icon', 16x16px
const unsigned char bitmap__RainMode_Icon [] PROGMEM = {
	0x44, 0x14, 0x11, 0x80, 0x43, 0xca, 0x0f, 0xb0, 0x3f, 0xfc, 0x7f, 0xf6, 0x7f, 0xfe, 0x6f, 0xf4, 
	0x04, 0xa0, 0x00, 0x80, 0x50, 0x8a, 0x04, 0x80, 0x44, 0x88, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
};
// ' Settings_Icon', 16x16px
const unsigned char bitmap__Settings_Icon [] PROGMEM = {
	0x00, 0x00, 0x03, 0xc0, 0x1b, 0xd8, 0x3f, 0xfc, 0x3f, 0xfc, 0x1f, 0xf8, 0x3e, 0x7c, 0x7c, 0x3e, 
	0x7c, 0x3e, 0x3e, 0x7c, 0x1f, 0xf8, 0x3f, 0xfc, 0x3f, 0xfc, 0x1b, 0xd8, 0x03, 0xc0, 0x00, 0x00
};
// ' SOS_Icon', 16x16px
const unsigned char bitmap__SOS_Icon [] PROGMEM = {
	0x00, 0x00, 0x1c, 0x00, 0x36, 0x7c, 0x23, 0x02, 0x21, 0x71, 0x26, 0x09, 0x24, 0x65, 0x24, 0x15, 
	0x26, 0x15, 0x11, 0x01, 0x18, 0x98, 0x0c, 0x6c, 0x06, 0x04, 0x03, 0x04, 0x00, 0xfc, 0x00, 0x00
};
// ' TimeCheck_Icon', 16x16px
const unsigned char bitmap__TimeCheck_Icon [] PROGMEM = {
	0x07, 0xe0, 0x18, 0x18, 0x22, 0xa4, 0x50, 0x02, 0x48, 0x0a, 0x84, 0x01, 0xa3, 0x89, 0x82, 0x41, 
	0xa2, 0x49, 0x81, 0x81, 0xa0, 0x09, 0x50, 0x02, 0x47, 0xe2, 0x23, 0xc4, 0x18, 0x18, 0x07, 0xe0
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 384)
const unsigned char* bitmap_icons[6] = {
	bitmap__Day_Mode,
	bitmap__Night_Mode,
	bitmap__RainMode_Icon,
	bitmap__SOS_Icon,
	bitmap__Settings_Icon,
	bitmap__TimeCheck_Icon
};


// 'scrollbar_background', 8x64px
const unsigned char bitmap_scrollbar_background [] PROGMEM = {
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00
};

// 'item_sel_outline', 128x21px
const unsigned char bitmap_item_sel_outline [] PROGMEM = {
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0
};



// ------------------ end generated bitmaps from image2cpp ---------------------------------



const int NUM_ITEMS = 6; // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)
const int MAX_ITEM_LENGTH = 20; // maximum characters for the item name

char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {  // array with item names
  { "Day Mode" }, 
  { "Night Mode" }, 
  { "Rain Mode" }, 
  { "SOS Mode" },  
  { "Standby Mode"}, 
 };

const int NUM_ITEMS_SCREEN2 = 6; // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)
const int MAX_ITEM_LENGTH_SCREEN2 = 30; // maximum characters for the item name

char menu_items_SCREEN2 [NUM_ITEMS_SCREEN2] [MAX_ITEM_LENGTH_SCREEN2] = {  // array with item names
  { "Day Mode On!!" }, 
  { "Night Mode On!!" }, 
  { "Rain Mode On!!" }, 
  { "SOS Mode On!!" }, 
  { "Standby Mode!!" }, 
  { "Time Check!!" },  
 };

//boolean up = false;
//boolean down = false;
//boolean middle = false;

ClickEncoder *encoder;
int16_t lastValue;

//ClickEncoder encoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_BUTTON, 4);
volatile int value = 0;
volatile int last = 0;
volatile boolean up = false;
volatile boolean down = false;
volatile boolean middle = false;  // add middle button state variable


const int timerInterval = 500; //rotary

int item_selected = 0; // which item in the menu is selected

int item_sel_previous; // previous item - used in the menu screen to draw the item before the selected one
int item_sel_next; // next item - used in the menu screen to draw next item after the selected one

int current_screen = 0;   // 0 = menu, 1 = screenshot, 2 = qr

//variables for keeping track of the current and previous time for buttons
unsigned long currentMillisBtn = 0; //current time
unsigned long previousMillisBtn = 0; //previous time used to reset options
unsigned long previousMillisBtn2 = 0; //previous time used to reset selectedFunction if the modifications are not applied
int delayForResetAll = 1500;  //delay for holding the buttons to reset currentFunction and selectedFunction
int delayForResetSelectedFunction = 10000; //delay for reseting selectedFunction if it was not applied

// ------------------ end of Rotary and OLED Initilization ---------------------------------
#define SD_ChipSelectPin 7
TMRpcm tmrpcm;

void setup() {
  tmrpcm.speakerPin = 6;

  Serial.begin(9600);
  sim800l.begin(9600);
  u8g.setColorIndex(1);  // set the color to white


  //encoder = new ClickEncoder(A1, A0, A2);
  encoder->setAccelerationEnabled(false);
  encoder = new ClickEncoder(ENCODER_PINA, ENCODER_PINB, ENCODER_BTN);
  MsTimer2::set(50, timerIsr);
  MsTimer2::start();

 last = encoder->getValue();

  // RTC
  Wire.begin();          // initialize I2C communication
  rtc.begin();
  // Wait for SIM800L module to respond
  if (!checkSIM800L()) {
    Serial.println("SIM800L not responding.");
    while (1);
  }
  Serial.println("TYPE S TO SEND A MESSAGE OR D TO SEND AN ALERT");
  // Uncomment the line below to set the RTC to the current date and time
  //rtc.adjust(DateTime(__DATE__, __TIME__));
 
        // set the RTC module to the desired date and time
  
// ------------------ end of Rotary and OLED Initilization ---------------------------------  
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


bool sdCardCheck(){
  while (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return false;
  } 
  Serial.println("Working SD");
  tmrpcm.setVolume(5);
  return true;
}

void loop() {
s:

  sdCardCheck(); // check sd card

  smsCount = 0;
  process1();
  readRotaryEncoder(encoder);

  ClickEncoder::Button b = encoder->getButton();
if (b != ClickEncoder::Open) {
  switch (b) {
    case ClickEncoder::Clicked:
      middle = true;
      bool smsSent = false;
      if (item_selected == 0) {

          tmrpcm.play("day.wav");
          while (tmrpcm.isPlaying()); 

        while (item_selected == 0) {
          process1();  
          process2();
          daymode();

          delay(500);
    
          // goto s;
          if (encoder->getButton() == ClickEncoder::Clicked) {
            Serial.println("EXIT MENU");\
            item_selected=-1;
            goto s;
          }

          
        }

  
      } else if (item_selected == 1) {


        tmrpcm.play("night.wav");
        while (tmrpcm.isPlaying()); 
         
        while (item_selected == 1) {
          process1();
          process2();


          nightmode();
          delay(500);
          if (encoder->getButton() == ClickEncoder::Clicked) {
            Serial.println("EXIT MENU");
            item_selected=-1;
            goto s;
          }
        }
      } else if (item_selected == 2) {

        tmrpcm.play("rain.wav");
        while (tmrpcm.isPlaying()); 

        while (item_selected == 2) {
          process1();
          process2();


          // rainmode

          rainmode();
          delay(500);
          if (encoder->getButton() == ClickEncoder::Clicked) {
            Serial.println("EXIT MENU");
            item_selected=-1;
            goto s;
          }
        }
      } else if (item_selected == 3) {

          tmrpcm.play("sos.wav");
          while (tmrpcm.isPlaying()); 

        while (item_selected == 3) {
          process1();
          process2();



          sosmode();
          delay(500);
          if (encoder->getButton() == ClickEncoder::Clicked) {
            Serial.println("EXIT MENU");
            item_selected=-1;
            goto s;
          }
        }
      } else if (item_selected == 4) {

          tmrpcm.play("stand.wav");
          while (tmrpcm.isPlaying()); 

        while (item_selected == 4) {
          process1();
          process2();



          standbymode();
          delay(500);
          if (encoder->getButton() == ClickEncoder::Clicked) {
            Serial.println("EXIT MENU");
            item_selected=-1;
            goto s;
            
          }
        }
      } 
  }
}

  process2();
}

void readRotaryEncoder(ClickEncoder *encoder)
{
  value += encoder->getValue();
  
  if (value/2 > last) {
    last = value/2;
    down = true;
  }else   if (value/2 < last) {
    last = value/2;
    up = true;
  }
}

void timerIsr() {
  encoder->service();
  readRotaryEncoder(encoder);
}




void process1(){
  u8g.firstPage(); // required for page drawing mode for u8g library
  do {

    if (current_screen == 0) { // MENU SCREEN

      // selected item background
      u8g.drawBitmapP(0, 22, 128/8, 21, bitmap_item_sel_outline);

      // draw previous item as icon + label
      u8g.setFont(u8g_font_7x14);
      u8g.drawStr(25, 15, menu_items[item_sel_previous]); 
      u8g.drawBitmapP( 4, 2, 16/8, 16, bitmap_icons[item_sel_previous]);          

      // draw selected item as icon + label in bold font
      u8g.setFont(u8g_font_7x14B);    
      u8g.drawStr(25, 15+20+2, menu_items[item_selected]);   
      u8g.drawBitmapP( 4, 24, 16/8, 16, bitmap_icons[item_selected]);     

      // draw next item as icon + label
      u8g.setFont(u8g_font_7x14);     
      u8g.drawStr(25, 15+20+20+2+2, menu_items[item_sel_next]);   
      u8g.drawBitmapP( 4, 46, 16/8, 16, bitmap_icons[item_sel_next]);  

      // draw scrollbar background
      u8g.drawBitmapP(128-8, 0, 8/8, 64, bitmap_scrollbar_background);

      // draw scrollbar handle
      u8g.drawBox(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS); 

              

    } 
    else if (current_screen == 1) { // SCREENSHOTS SCREEN

        u8g.setFont(u8g_font_7x14B);     
        u8g.drawStr(20, 37, menu_items_SCREEN2[item_selected]);   
        
    } 
   

  } while ( u8g.nextPage() ); // required for page drawing mode with u8g library
}

void process2(){
  if (up && current_screen == 0) { // MENU SCREEN
      
      up = false;
    
        item_selected = item_selected - 1; // select previous item

        if (item_selected < 0) { // if first item was selected, jump to last item
            item_selected = NUM_ITEMS-1;
          }
      }

if (down && current_screen == 0){
        down = false;
       item_selected = item_selected + 1; // select next item
          if (item_selected >= NUM_ITEMS) { // last item was selected, jump to first menu item
            item_selected = 0;
            }
      }
        
if (middle){
     middle = false;
    if (current_screen == 0) {current_screen = 1;} //
    else if (current_screen == 1) {current_screen = 0;} // screenshots screen --> Normal Screen
    
  }   
        

  // set correct values for the previous and next items
  item_sel_previous = item_selected - 1;
  if (item_sel_previous < 0) {item_sel_previous = NUM_ITEMS - 1;} // previous item would be below first = make it the last
  item_sel_next = item_selected + 1;  
  if (item_sel_next >= NUM_ITEMS) {item_sel_next = 0;} // next item would be after last = make it the first
}

// Define the functions that correspond to the menu items
void daymode(){

    // tmrpcm.stopPlayback();  
    // tmrpcm.play("day.wav");

    Serial.println("You Selected Day!");
    MainUltraSonic();
    LEDHazzardfunctionOFF();
    LDRfunction();
    u8g.setFont(u8g_font_7x14B);     
    u8g.drawStr(20, 37, menu_items_SCREEN2[item_selected]);

}
void nightmode(){
  Serial.println("You Selected Night!");
  MainUltraSonic();
  LEDHazzardfunctionOFF();
  
  while (smsCount < 3) { // loop until 3 SMS messages have been sent
    sms("09473117641", "ALERT: Night Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
    delay(1000); // wait for 1 second between SMS messages
    smsCount++; // increment counter variable
  }
}
void rainmode(){
  Serial.println("You Selected Rain!");
  LEDHazzardfunctionON();
  
  while (smsCount < 3) { // loop until 3 SMS messages have been sent
    sms("09473117641", "ALERT: Rain Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
    delay(1000); // wait for 1 second between SMS messages
    smsCount++; // increment counter variable
  }
}
void sosmode(){
  Serial.println("You Selected SOS!");
  LEDHazzardfunctionON();
  sms("09473117641", "ALERT: SOS Mode was triggered! Location: vip.sinotrack.com"); //pa change nalang number
}
void standbymode(){
  Serial.println("You Selected Standby!");
  LEDHazzardfunctionOFF();
}
void timeCheckmode(){
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
    digitalWrite(LvibPin, HIGH);
    digitalWrite(MvibPin, HIGH);
    digitalWrite(RvibPin, HIGH);    

      // Obstacle is detected

      tmrpcm.play("obs.wav");

    }
  else{
    digitalWrite(LvibPin, LOW);
    digitalWrite(MvibPin, LOW);
    digitalWrite(RvibPin, LOW);
    tmrpcm.stopPlayback();
    // Obstacle is not detected
  }
}

void LDRfunction(){
   if( digitalRead( ldr_pin ) == 1){
      Serial.println("Dark Place"); // Voice wull notif that it is on the dark area
   }
   else{
       Serial.println("Light Place"); // Voice wull notif that it is on the ligher area
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

