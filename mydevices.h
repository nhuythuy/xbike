#include "global_vars.h"
#include <CayenneMQTTESP8266.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char dv_username[] = "3541f5b0-d9b3-11ea-883c-638d8ce4c23d";
char dv_password[] = "0573b7bfc25b7afb4042b3bb93ed8f16a6dd6fc2";
char dv_clientID[] = "d175a430-d9b4-11ea-b767-3f1a8f1211ba";


#define CH_BATT_VOLTAGE                 1
#define CH_MINUTES_DOOR_MAIN_OPENED     2
#define CH_MINUTES_DOOR_BASEMENT_OPENED 3
#define CH_TEMPERATURE                  4
#define CH_HUMIDITY                     5
#define CH_RUNTIME                      6
#define CH_SECONDS_MOTION_DETECTED      7

// digital states
#define CH_DOOR_MAIN            10
#define CH_DOOR_BASEMENT        11

int cayenneCounter = 0;

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(CH_BATT_VOLTAGE){
  delay(1000);
  Cayenne.virtualWrite(CH_BATT_VOLTAGE, ssBatteryVolt, "batt", "V");
}

CAYENNE_OUT(CH_MINUTES_DOOR_MAIN_OPENED){
  delay(1000);
  Cayenne.virtualWrite(CH_MINUTES_DOOR_MAIN_OPENED, minutesDoorMainOpened, "counter");
}

CAYENNE_OUT(CH_RUNTIME){
  delay(1000);
  Cayenne.virtualWrite(CH_RUNTIME, runtimeMinutes, "counter");
}

CAYENNE_OUT(CH_TEMPERATURE){
  delay(1000);
  Cayenne.celsiusWrite(CH_TEMPERATURE, temp);
}

CAYENNE_OUT(CH_HUMIDITY){
  delay(1000);
  Cayenne.virtualWrite(CH_HUMIDITY, humidity, "rel_hum", "p");
}

void writeCayenneDigitalStates(int channelId, int value){
  delay(1000);
  Cayenne.virtualWrite(channelId, value, "digital_sensor", "d");
}
