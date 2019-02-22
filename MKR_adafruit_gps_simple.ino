// GET $GPRMC LINE FROM GPS-module.
// USING: MKRGSM1400 & ADAFRUIT ULTIMATE GPS BREAKOUT v3
/*
 * GPS TEST
 * MKRGSM1400
 */
#include <Arduino.h>


#define SerialGPS Serial1  // RX&TX PINS
#define CHECK_INTERVAL 5  //seconds.

//VARS
unsigned long previousTest;
bool mGPS_got_line = false, mGPS_paused = false;
uint8_t mGPS_idx=0;
char mGPS_TempLine[120];

void setup() {
  Serial.begin(9600); //USB
  SerialGPS.begin(9600); //TEST GPS.

  while(!Serial) { ; } //WAIT FOR SERIAL USB.
  Serial.print("\nTEST SOMETHING\n\n");
  
}

void loop() {

if (millis() - previousTest >= CHECK_INTERVAL*1000UL) 
{
  previousTest = millis();
  Serial.print("\n\nGET NEW LINE\n\n");
  if (mGPS_paused) mGPS_paused = false; 
}
char mGPS = 0;
if ((SerialGPS.available() > 0) && !mGPS_got_line && !mGPS_paused) 
{
  mGPS = SerialGPS.read();
  //Serial.write(mGPS);   //DEBUG
  if (mGPS == '\n') 
  {
    mGPS_TempLine[mGPS_idx] = 0; mGPS_idx = 0; mGPS_got_line = true;
  } 
  else
  {
     mGPS_TempLine[mGPS_idx++] = mGPS;
     if (mGPS_idx >= 120) mGPS_idx = 119; //dont overflow.
  }
}
if (mGPS_got_line)
{
  if (strstr(mGPS_TempLine, "$GPRMC"))
  {
    Serial.print("-->");Serial.println(mGPS_TempLine);
    //DO SOMETHING WITH THE LINE.
    
    //done parsed.
    mGPS_paused = true; mGPS_got_line = false; //Reset to get a new after paused has been reset.
  }
  else
  {
    mGPS_got_line = false; //wrong line not GPRMC. get another.
  }
}

} //END LOOP
