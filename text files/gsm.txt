#include <SoftwareSerial.h>

// Create a software serial object to communicate with SIM800L module
SoftwareSerial sim800l(10, 11); // RX, TX

void setup() {
  // Start the serial communication between Arduino and SIM800L
  sim800l.begin(9600);
  Serial.begin(9600); // Start the serial communication between Arduino and serial monitor

  // Wait for SIM800L module to respond
  if (!checkSIM800L()) {
    Serial.println("SIM800L not responding.");
    while (1);
  }
  Serial.println("TYPE S TO SEND A MESSAGE OR D TO SEND AN ALERT");
}

void loop() {
  // Check if the user typed "s" or "d" in the serial monitor
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 's') {
      // Call the sms() function to send a text message
      sms("09473117641", "Hello from Arduino!");
    } else if (input == 'd') {
      // Call the sms() function to send an alert message
      sms("09473117641", "Hello Alert!");
    }
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

  delay(1000); // Wait for SIM800L module to respond

  // Send the message
  sim800l.print(message);

  delay(1000); // Wait for SIM800L module to respond

  // Send Ctrl+Z to end the message
  sim800l.write(26);

  delay(1000); // Wait for SIM800L module to respond
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
