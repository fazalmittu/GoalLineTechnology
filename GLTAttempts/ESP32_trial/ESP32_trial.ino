#include <Gsender_32.h>

/* Arduino core for ESP32 WiFi chip SMTP Gmail sender
   by jubaid hossain
   4.9.2018
   you can copy and paste full code
*/
#include <WiFi.h>
#include "Gsender_32.h"
#include <WiFiClientSecure.h>
//#include "soc/soc.h"`
//#include "soc/rtc_cntl_reg.h"`

#pragma region Globals
const char* ssid = "EKONKAR-Guest";                           // WIFI network name
const char* password = "abcabcabcd";                       // WIFI network password
uint8_t connection_state = 0;                    // Connected to WIFI or not
uint16_t reconnect_interval = 10000; // If not connected wait time to try again
#pragma endregion Globals

//String address[] = {"npanair@gmail.com"};

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
  Serial.println("TRYING TO CONNECT");
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
    Serial.println("HELLO WORLD");
  } else {
    WiFi.begin(ssid, password);
    Serial.println(ssid);
    Serial.println("ELSE STATEMENT");
    Serial.println("HELLOOO I CHANGED");
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

void Awaits()
{
  uint32_t ts = millis();
  while (!connection_state)
  {
    delay(200);
    if (millis() > (ts + reconnect_interval) && !connection_state) {
      Serial.println("TRYING TO CONNECT");
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void setup()
{
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  Serial.println("SERIAL MONITOR OPEN");
  connection_state = WiFiConnect();
  Serial.println("I CONNECTED");
  if (!connection_state) // if not connected to WIFI
  {
    Serial.println("I AM AWAITING CONNECTION");
    Awaits(); // constantly trying to connect
  }
  Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
  String subject = "ESP32 Test!";
  if (gsender->Subject(subject)->Send("fazalmittu1800@gmail.com", "ESP32 Test Email!"))
  {
    Serial.println("Message send.");
  }
  else
  {
    Serial.print("Error sending message: ");
    Serial.println(gsender->getError());
  }  
}

void loop() {}
