#include <U8glib.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
int button1 = 42; // button 1 is connected to digital pin SET 42
int button2 = 43; // button 2 is connected to digital pin SET 43
int button3 = 44; // button 3 is connected to digital pin SET 44
int button4 = 45; // button 4 is connected to digital pin SET 45
int button5 = 46; // button 5 is connected to digital pin SET 46
int buttonPressed = 0; // variable to store which button is pressed

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  u8g.setRot180(); // Rotate the display 180 degrees
  Serial.begin(9600);
}

void loop() {
  s:
  buttonPressed=0;
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x10);
    if (digitalRead(button1) == LOW) {
      buttonPressed = 1;
      while(buttonPressed == 1){
        u8g.drawStr((128 - u8g.getStrWidth("Daymode Selected!")) / 2, 30, "Daymode Selected!");
        Serial.println("Daymode Selected!");
        u8g.nextPage();
                
        if (digitalRead(button5) == LOW){
          goto s;
        }                
      }
    } else if (digitalRead(button2) == LOW) {
      buttonPressed = 2;
      while(buttonPressed == 2){
      u8g.drawStr((128 - u8g.getStrWidth("Nightmode Selected!")) / 2, 30, "Nightmode Selected!");
      Serial.println("Nightmode Selected!");
      u8g.nextPage();

      if (digitalRead(button5) == LOW){
          goto s;
        }      
      }
    } else if (digitalRead(button3) == LOW) {
      buttonPressed = 3;
      while(buttonPressed == 3){
      u8g.drawStr((128 - u8g.getStrWidth("Rainmode Selected!")) / 2, 30, "Rainmode Selected!");
      Serial.println("Rainmode Selected!");
      u8g.nextPage();

      if (digitalRead(button5) == LOW){
          goto s;
        }     
      }
    } else if (digitalRead(button4) == LOW) {
      buttonPressed = 4;
      while(buttonPressed == 4){
      u8g.drawStr((128 - u8g.getStrWidth("SOSmode Selected!")) / 2, 30, "SOSmode Selected!");
      Serial.println("SOSmode Selected!");
      u8g.nextPage();

      if (digitalRead(button5) == LOW){
          goto s;
        }    
      }       
    } 
    else {
      u8g.drawStr((128 - u8g.getStrWidth("NO MODE SELECTED")) / 2, 30, "NO MODE SELECTED");
      Serial.println("NO MODE SELECTED!");
    }
  } while (u8g.nextPage());
}
