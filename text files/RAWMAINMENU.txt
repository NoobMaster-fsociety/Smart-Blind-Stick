#include <U8glib.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
int button1 = 42; // button 1 is connected to digital pin 2
int button2 = 43; // button 2 is connected to digital pin 3
int button3 = 44; // button 3 is connected to digital pin 4
int button4 = 45; // button 4 is connected to digital pin 5
int button5 = 46; // button 5 is connected to digital pin 6
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
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x10);
    if (digitalRead(button1) == LOW) {
      buttonPressed = 1;
      while(buttonPressed == 1){
        u8g.drawStr((128 - u8g.getStrWidth("Daymode Selected!")) / 2, 30, "Daymode Selected!");
        Serial.println("Daymode Selected!");
        u8g.nextPage();
      }
    } else if (digitalRead(button2) == LOW) {
      buttonPressed = 2;
      u8g.drawStr((128 - u8g.getStrWidth("Nightmode Selected!")) / 2, 30, "Nightmode Selected!");
      Serial.println("Nightmode Selected!");
    } else if (digitalRead(button3) == LOW) {
      buttonPressed = 3;
      u8g.drawStr((128 - u8g.getStrWidth("Rainmode Selected!")) / 2, 30, "Rainmode Selected!");
      Serial.println("Rainmode Selected!");
    } else if (digitalRead(button4) == LOW) {
      buttonPressed = 4;
      u8g.drawStr((128 - u8g.getStrWidth("SOSmode Selected!")) / 2, 30, "SOSmode Selected!");
      Serial.println("SOSmode Selected!");
    } else if (digitalRead(button5) == LOW) {
      buttonPressed = 5;
      u8g.drawStr((128 - u8g.getStrWidth("Standbymode Selected!")) / 2, 30, "Standbymode Selected!");
      Serial.println("Standbymode Selected!");
    } else {
      u8g.drawStr((128 - u8g.getStrWidth("NO MODE SELECTED")) / 2, 30, "NO MODE SELECTED");
    }
  } while (u8g.nextPage());
}
