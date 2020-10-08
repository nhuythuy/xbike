#ifndef GLOBAL_VARS
#define GLOBAL_VARS

long globalState = 0;
int debugCounter = 0;

// sensors
float humidity = 0.0;
float temp = 0.0;

int ssBatteryVoltRaw = 0;
float ssBatteryVolt = 0;
bool ssDoorMain = 0;



int timeDoorMainOpened = 0;
int minutesDoorMainOpened = 0;

int runtimeMinutes = 0;

#endif
