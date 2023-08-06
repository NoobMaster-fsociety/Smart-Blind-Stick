#include <SoftwareSerial.h>

// Configure the SIM900A software serial port
SoftwareSerial sim900a(18, 19);  // RX, TX

void setup() {
  Serial.begin(9600);         // Start the hardware serial port
  sim900a.begin(9600);        // Start the software serial port for SIM900A
}

void loop() {
  // Check if there is any text message available
  if (sim900a.available()) {
    String message = sim900a.readString();  // Read the incoming message
    Serial.println("Received message: " + message);
  }

  // Send a test message to a phone number
  sim900a.println("AT+CMGF=1");  // Set the SMS mode to text mode
  delay(1000);
  sim900a.println("AT+CMGS=\"+639951594526\"");  // Replace with your own phone number
  delay(1000);
  sim900a.println("Alert: SOS Mode Activated.");  // Replace with your own message
  delay(1000);
  sim900a.write(26);  // Send the Ctrl+Z character to indicate the end of the message
  delay(5000);

  

}