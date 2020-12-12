#include <ESP8266WiFi.h>

// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
// IMPORTANT NOTE: Remove PIN_SS_DOOR_MAIN connector b4 uploading code to NodeMcu, uploading fail otherwise

// inputs
#define PIN_SS_SUPPLY_VOLT          A0 // battery voltage

#define PIN_SS_DHT                  D5 // DHT sensor pin
#define PIN_SS_BIKE_STILL           D9 // = RX, try use this as DI, together w.   Serial.begin(19200, SERIAL_8N1, SERIAL_TX_ONLY);
#define PIN_SS_BATT_STILL           D6

#define PIN_IN_SIGNAL_LEFT          D1
#define PIN_IN_SIGNAL_RIGHT         D2

// outputs
#define PIN_LED                     D4 // LED_BUILTIN = D4: same as built in LED GPIO2
#define PIN_OUT_SIGNAL_LEFT         D7
#define PIN_OUT_SIGNAL_RIGHT        D8
