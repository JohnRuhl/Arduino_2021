// This is for Lab7.  
// Monitor a photodiode

#include <Adafruit_ADS1015.h>   // the library for the A/D card


// Set up ADS1115 A/D stuff
Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 object at address 0x48
float ADCVoltsPerBit = 6.1447/(32767);  // conversion from bits to volts for GAIN_TWOTHIRDS (default).
float V0, V1;  // input voltage, after converting from bits
int InA0, InA1;  // input bits for ADS1115
const int LEDPin = 6;
const int timer = 500;
float t1, t2;
int State = LOW;
int delaytime = 10;

//---------------------------------------------------
void setup()
{
  Wire.begin();
  ads1115.begin();
  ads1115.setGain(GAIN_TWOTHIRDS); // sets gain=2, range to run from 0 to +/- 2.048V

  // Start the serial link, at 9600 bps.
  Serial.begin(115200);
  Serial.println("# Lab7_photocell");
  t1 = millis();
  digitalWrite(LEDPin,LOW);
}

//---------------------------------------------------
void loop()
{
  //  Make the LED turn on and off ever "timer" milliseconds
  t2 = millis();
  if (t2-t1 > timer) {
    if (State == LOW) {
      State = HIGH;
    } else {
      State = LOW;
    }
    t1=t2;
    digitalWrite(LEDPin,State);
  }
 
  
  // Read in voltages from ADS1115
  InA0 = ads1115.readADC_SingleEnded(0);
  V0 = ADCVoltsPerBit*InA0;
  InA1 = ads1115.readADC_SingleEnded(1);
  V1 = ADCVoltsPerBit*InA1;

  Serial.println(V0-V1,4);

  // Wait for a bit (slow down sampling)
  delay(delaytime);

}
