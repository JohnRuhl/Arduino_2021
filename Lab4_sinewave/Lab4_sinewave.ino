// This is for Lab4.  It generates a sine wave using the MCP4725.
#define MCP4725_ADDR 0x60   

// Libraries for the I2C bus (for both A/D and D/A)
#include <Wire.h>  //Include the Wire library to talk I2C

// Set up things for the MCP4725, 12-bit D/A
// Vout = A + B*sin(2*pi*f*t) = A + B*sin(w*t)
float V_fullscale = 5.16;  // Measure your +5V supply and enter the value here!
float A = 2.5;
float B = 1.0;  // Amplitude of wave in volts, between 0 and 2.5.
float f = 100.0; // Frequency in Hz.
float w = 2*3.1415*f;  // angular frequency
float Vout;

float t;  // time in seconds
float fullscale_fraction;
int code;   // output integer code for DAC
int bits_8;  // upper 8 bit code for DAC
int bits_4;  // lower 8 bit code for DAC


//---------------------------------------------------
void setup()
{
  Wire.begin();
  
  // Start the serial link, at 9600 bps.
  Serial.begin(115200);

  // Start up the I2C bus
  Wire.begin();

}

//---------------------------------------------------
void loop()
{
  // ------ Find the time, calculate the output voltage.
  t = micros()/1e6;   // time in seconds;  this overflows after about 70 minutes.
  Vout = A + B*sin(w*t);
  code = int((Vout/V_fullscale)*4095);

  // ------Unpack the code into the 8 most significant bits, the 4 least significant bits, then
  // ------send everything over the I2C bus to the MCP4715.
  bits_8 = code >> 4;
  bits_4 = (code & 15) << 4;
  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(64);                     // cmd to update the DAC
  Wire.write(bits_8);        // the 8 most significant bits...
  Wire.write(bits_4); // the 4 least significant bits...
  Wire.endTransmission();

  
}
