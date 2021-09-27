// This is for Lab5.  It steps up an output voltage using the MCP4725, and reads in two differential 
// signals in from the ADS1115, in order to get I_diode and V_diode. 

#define MCP4725_ADDR 0x60   

// Libraries for the I2C bus (for both A/D and D/A)
#include <Wire.h>  //Include the Wire library to talk I2C
#include <Adafruit_ADS1X15.h>   // the library for the A/D card

// Things you need to measure and change.
float V_fullscale = 5.16;  // ***MEASURE THE VOLTAGE OF YOUR +5V ARDUINO SUPPLY AND PUT THE VALUE HERE!
float R = 1000;  // Ohms  **** ENTER THE MEASURED VALUE OF YOUR RESISTOR HERE ***

// Set up things for the MCP4725, 12-bit D/A
// Vout = 0, x, 2x, 3x... up to Vmax, where x = Vstep is the stepsize.
// Reset Vout to zero when it gets above Vmax
float V_DAC;  // the variable that holds the value being sent out.
float Vmax = 4.0;  // the maximum voltage to send out.
float Vstep = 0.2; // the voltage step size.
float Vstart = 0.0;  // the voltage to start at.
float delaytime = 1000; // milliseconds at each step
// These are needed to send the value to the DAC
int code;   // output integer code for DAC
int bits_8;  // upper 8 bit code for DAC
int bits_4;  // lower 8 bit code for DAC

// Set up ADS1115 A/D stuff
Adafruit_ADS1115 ads1115; //(0x48); // construct an ads1115 object at address 0x48
float ADCVoltsPerBit = 6.144/(32767);  // conversion from bits to volts
float V0, V1, Vdiode, I;  // input voltage, after converting from bits
int InA0, InA1, InA2, InA3;  // input bits for ADS1115

//---------------------------------------------------
void setup()
{
  Wire.begin();
  ads1115.begin();

  // If we want better resolution at the ADS1115, we can increase its preamp gain.
  // Adjust both here, and above where ADCVoltsPerBit is set.
  //ads1115.setGain(GAIN_TWO); // sets gain=2, range to run from 0 to +/- 2.048V

  // Start the serial link, at 9600 bps.
  Serial.begin(9600);
  Serial.println("# Lab5_DiodeMeas_IV_16bit");

  V_DAC = Vstart;  // Set the start voltage, for first time through.

}

//---------------------------------------------------
void loop()
{

  // Value to be sent to DAC
  code = int((V_DAC/V_fullscale)*4095);

  // ------Unpack the code into the 8 most significant bits, the 4 least significant bits, then
  // ------send everything over the I2C bus to the MCP4715.
  bits_8 = code >> 4;
  bits_4 = (code & 15) << 4;
  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(64);                     // cmd to update the DAC
  Wire.write(bits_8);        // the 8 most significant bits...
  Wire.write(bits_4); // the 4 least significant bits...
  Wire.endTransmission();

  // Wait for a bit for things to settle
  delay(delaytime);
  
  // Read in A0 and A1
  // Top of R is wired to A0
  // Bottom of R, top of diode, wired to A1
  // Bottom of diode is ground, and we'll assume that's 0V.
  InA0 = ads1115.readADC_SingleEnded(0);  
  InA1 = ads1115.readADC_SingleEnded(1);

  V0 = ADCVoltsPerBit*InA0;
  V1 = ADCVoltsPerBit*InA1;

  I = (V0 - V1) /R;
  Vdiode = V1;
 

  // ------Print the ADS1115 info
  Serial.print(V_DAC);  
  Serial.print("\t");
  Serial.print(V0,4);
  Serial.print("\t");
  Serial.print(Vdiode,4);  
  Serial.print("\t");
  Serial.println(I,9);
  
  V_DAC = V_DAC + Vstep;
  if (V_DAC > Vmax) V_DAC = 0;
}
