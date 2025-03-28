// Oct. 08 2020
// Author: Thuy Nguyen


#include <DHT.h>
#include "global_vars.h"
#include "mydevices.h"
#include <ESP8266WiFi.h>
#include "pin_define.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define CAYENNE_PRINT Serial

const char* ssid = "VNNO";
const char* password = "MyWifiPassword"; // WIFI_PW;

#define MAX_SUPPLY_VOLT   16.157    // volt: 10K(9910)+39K(38610) --> 3.3*(9910+38610)/9910 = 16.1570131181 V 

DHT dht(PIN_SS_DHT, DHT11,15);
WiFiClient client;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov");

bool needUploadCloud = false;
bool cloudUploaded = false;

// blink without delay:
const long blinkInterval = 500;      // interval at which to blink (milliseconds)
unsigned long previousMillis = 0;    // will store last time LED was updated
int ledState = LOW;             // ledState used to set the LED

unsigned long currentMillis = millis();

void WIFI_Connect(){
  Serial.println();
  Serial.println("MAC: " + WiFi.macAddress());
  Serial.println("Connecting to " + String(ssid));
  
  WiFi.begin(ssid, password);

  bool ledStatus = false;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    ledStatus = !ledStatus;
    digitalWrite(PIN_LED, !ledStatus);
    if(debugCounter++ > 80)
    {
      debugCounter = 0;
      Serial.println("!");
    }
  }

  delay(500);
  Serial.println("Connected to wifi");
  Serial.print("Status: ");   Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");       Serial.println(WiFi.localIP());
  Serial.print("Subnet: ");   Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");  Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
  Serial.println();
  delay(1000);

  Serial.println("Cayenne connecting...");
  Cayenne.begin(dv_username, dv_password, dv_clientID, ssid, password);
  Serial.println("Cayenne connected");
  delay(1000);    
}

void setup() {
  pinMode(PIN_SS_BIKE_STILL, INPUT);
  pinMode(PIN_SS_BATT_STILL, INPUT);
  pinMode(PIN_IN_SIGNAL_LEFT, INPUT);
  pinMode(PIN_IN_SIGNAL_RIGHT, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_SS_BIKE_STILL), detectBikeMove, RISING);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_OUT_SIGNAL_LEFT, OUTPUT);
  pinMode(PIN_OUT_SIGNAL_RIGHT, OUTPUT);
  //pinMode(PIN_AC_POWER_RADIO, OUTPUT);

  //Serial.begin(19200);
  Serial.begin(19200, SERIAL_8N1, SERIAL_TX_ONLY);
  delay(1000);
//  dht.begin();

  WIFI_Connect();

  timeClient.begin(); // Initialize a NTPClient to get time
// Set offset time in seconds to adjust for your timezone, ex.: GMT +1 = 3600, GMT +8 = 28800, GMT -1 = -3600, GMT 0 = 0
  timeClient.setTimeOffset(3600); // Norway GMT + 1
}

void loop() {
//  updateHumidTempe();
  getServerTime();
  blinkSignal();

  minutesRuntime = millis() / 1000;
  minutesBikeNotStill = (millis() - millisBikeNotStill) / 60000;

  if(WiFi.status() == WL_DISCONNECTED){
    Serial.println("WiFi connection lost! Reconnecting...");
    WiFi.disconnect();
    WIFI_Connect();
  }
  else{
    Cayenne.loop();
    if(!cloudUploaded && needUploadCloud == true)
    {
      if(cayenneCounter++ > CH_HUMIDITY) // last channel
        cayenneCounter = 0;
      cloudUploaded = true;
    }

  }
}

ICACHE_RAM_ATTR void detectBikeMove() {
  millisBikeNotStill = millis();
  Serial.println("BIKE MOVE DETECTED!!!");

  writeCayenneDigitalStates(CH_BIKE_STILL, true);
}

void blinkSignal(){
  currentMillis = millis();

  if (currentMillis - previousMillis >= blinkInterval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    bool inLeft = digitalRead(PIN_IN_SIGNAL_LEFT);
    bool inRight = digitalRead(PIN_IN_SIGNAL_RIGHT);

    if(inLeft)
      digitalWrite(PIN_OUT_SIGNAL_LEFT, ledState);

    if(inRight)
      digitalWrite(PIN_OUT_SIGNAL_RIGHT, ledState);
  }
}

void getServerTime(){
  Serial.println();
  timeClient.update();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();
  
  if((minutes % 1) == 0) // to send every 1 minutes
    needUploadCloud = true;
  else
  {
    cloudUploaded = false;
    needUploadCloud = false;
  }
  
  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
//  String formattedDate = timeClient.getFormattedDate();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);
  Serial.print(" - ");
//  Serial.println(formattedDate);
  Serial.println();
}

bool updateHumidTempe(){
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");

    humidity = -100;
    temp = -100;
    return false;
  }

  return true;
}

void updateSensors(){
  ssBatteryVoltRaw = analogRead(PIN_SS_SUPPLY_VOLT);
  ssBatteryVolt = MAX_SUPPLY_VOLT * ssBatteryVoltRaw / 1023;

  Serial.println("0. Battery volt.:  " + String(ssBatteryVolt) + " - " + String(ssBatteryVoltRaw));
  Serial.println("1. Temperature:    " + String(temp) + " deg C");
  Serial.println("2. Humidity:       " + String(humidity) + " %");
  Serial.println();
}
