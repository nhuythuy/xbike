#ifndef GLOBAL_VARS
#define GLOBAL_VARS

long globalState = 0;
int debugCounter = 0;

// sensors
float humidity = 0.0;
float temp = 0.0;

int ssBatteryVoltRaw = 0;
float ssBatteryVolt = 0;

unsigned long millisBikeNotStill = 0;

int minutesRuntime = 0;
int minutesBikeNotStill = 0;


#endif
