// This is for Lab5.  
// Use a diode as a thermometer.

#define MCP4725_ADDR 0x60   

// Libraries for the I2C bus (for both A/D and D/A)
#include <Wire.h>  //Include the Wire library to talk I2C
#include <Adafruit_ADS1X15.h>   // the library for the A/D card

// These are things you adjust to make your thermometer work well, and to calibrate it.
// We print the quantity (T_room + gain*(V_diode - Voffset)).
float Voffset = 0.6; // Volts offset, so we can see fluctuations better on serial plotter.
float gain = 500;   //  delta_K/delta_V for your diode
float T_room = 300; //  Kelvin

// Set up things for the MCP4725, 12-bit D/A
float V_DAC = 4.0;    // We're going to just put out a constant voltage.  You could just use the +5V supply.
float V_fullscale = 5.16;  // Measure your +5V supply and enter the value here!
int code;   // output integer code for DAC
int bits_8;  // upper 8 bit code for DAC
int bits_4;  // lower 8 bit code for DAC

float delaytime = 10; // milliseconds delay between readings

// Set up ADS1115 A/D stuff
// We're going to set the pre=amp gain of the ADS1115 to 4, to get a little more resolution, below.
Adafruit_ADS1115 ads1115; // construct an ads1115 object at address 0x48
float ADCVoltsPerBit = 1.024/(32767);  // conversion from bits to volts for GAIN_FOUR.
float Vdiode;  // input voltage, after converting from bits
int InA1;  // input bits for ADS1115


//---------------------------------------------------
void setup()
{
  Wire.begin();
  ads1115.begin();
  ads1115.setGain(GAIN_FOUR); // sets gain=4, 15-bit range to run from 0 to 1.024V

  // Start the serial link, at 9600 bps.
  Serial.begin(9600);
  Serial.println("# Lab5_DiodeThermometer_16bit");
  
  // ------Unpack the code into the 8 most significant bits, the 4 least significant bits, then
  // ------send everything over the I2C bus to the MCP4715.
  code = int((V_DAC/V_fullscale)*4095);
  bits_8 = code >> 4;
  bits_4 = (code & 15) << 4;
  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(64);                     // cmd to update the DAC
  Wire.write(bits_8);        // the 8 most significant bits...
  Wire.write(bits_4); // the 4 least significant bits...
  Wire.endTransmission();
  
}

//---------------------------------------------------
void loop()
{
  // Wait for a bit (slow down sampling)
  delay(delaytime);
  
  // Read in voltage from ADS1115
  InA1 = ads1115.readADC_SingleEnded(1);
  Vdiode = ADCVoltsPerBit*InA1;
 
  // ------Print the calibrated temperature.
  // 
  // The idea here is to roughly calibrate dVdiode/dT for your diode, and call that the "gain", 
  // and then use T = T_room + (dV/dT)*(V_diode - V_offset)
  // where V_offset is the voltage you read for the diode when it is at room temperature.
  //
  // Start by adjusting Voffset until the output on the serial plotter is about 
  //  equal to your estimated T_room.  Then adjust gain until the readout rises to a reasonable temperature
  //  for your skin, when yuu hold the diode between your thumb and finger.
  //  I've put in values for Voffset and gain that are a little off for my setup, so you'll probably have
  //  to adjust them to make the readout reasonable.
  
  Serial.println(T_room - gain*(Vdiode-Voffset),3);

}
