#include <WiFi.h>
#include "Gsender_32.h"
#include <WiFiClientSecure.h>

#pragma region Globals
const char* ssid = "*********";                           
const char* password = "*********";                       
uint8_t connection_state = 0;                    
uint16_t reconnect_interval = 10000;             
#pragma endregion Globals


uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr) {
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  } else {
    WiFi.begin(ssid, password);
    Serial.println(ssid);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50)
  {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void Awaits() {
  uint32_t ts = millis();
  while (!connection_state)
  {
    delay(200);
    if (millis() > (ts + reconnect_interval) && !connection_state) {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void email_send() {
  connection_state = WiFiConnect();
  if (!connection_state) 
  {
    Awaits(); 
  }
  Gsender *gsender = Gsender::Instance();    
  String subject = "HI REFEREE!";
  if (gsender->Subject(subject)->Send("**********@vtext.com", "GOAL SCORED - GLT SYSTEM"))
  {
    Serial.println("Message sent.");
  }
  else
  {
    Serial.print("Error sending message: ");
    Serial.println(gsender->getError());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(22, INPUT);
}


void loop() {
  if (digitalRead(22) == HIGH) {
    email_send();
  }

}
