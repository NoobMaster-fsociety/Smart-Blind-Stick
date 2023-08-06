#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial gsmSerial(10, 11); // RX, TX pins

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  delay(1000);
  while (!gsmCheck()) {
    Serial.println("GSM module is not responding. Retrying...");
    delay(1000);
  }
  Serial.println("GSM module is responding");
}

void loop() {
  if (gsmCheck()) {
    sendSMS("+639473117641", "Check location, need assistance. Stay safe! Location: vip.sinotrack.com");
    Serial.println("SMS SENT!");
    delay(1000);
  } else {
    Serial.println("GSM module is not responding");
    delay(1000);
  }
}

void sendSMS(String phone_number, String message) {
  gsmSerial.println("AT+CMGF=1"); // set message format to text mode
  delay(1000);
  gsmSerial.print("AT+CMGS=\""); // start message sending
  gsmSerial.print(phone_number);
  gsmSerial.println("\"");
  delay(1000);
  gsmSerial.print(message);
  delay(1000);
  gsmSerial.write(26); // end message sending by sending Ctrl+Z
  delay(1000);
}

bool gsmCheck() {
  gsmSerial.println("AT");
  delay(1000);
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.indexOf("OK") != -1) {
      return true;
    }
  }
  return false;
}
