// This is for Lab5.  
// Measure transistor beta.

#define MCP4725_ADDR 0x60   

// Libraries for the I2C bus (for both A/D and D/A)
#include <Wire.h>  //Include the Wire library to talk I2C
#include <Adafruit_ADS1X15.h>   // the library for the A/D card

// Things you will need to change.
float V_DAC = 5;
float V_fullscale = 5.16;  // Measure your +5V supply and enter the value here!

// You may need to change this if you don't have 100Ohm resistors.
float R = 100; // Ohms;  we're going to assume both your 100 Ohm resistors are this value.

// Set up things for the MCP4725, 12-bit D/A

int code;   // output integer code for DAC
int bits_8;  // upper 8 bit code for DAC
int bits_4;  // lower 8 bit code for DAC

float delaytime = 1000; // milliseconds delay between readings

// Set up ADS1115 A/D stuff
// We're going to set the preamp gain of the ADS1115 to 16, get a lot more resolution, below.
// We're also going to do a DIFFERENTIAL measurement across each resistor.  This 
// is what allows us to used gain=16, because the voltage across the 100 Ohm resistors is small.
Adafruit_ADS1115 ads1115;  // construct an ads1115 object at address 0x48
float ADCVoltsPerBit = 0.256/(32767);  // conversion from bits to volts for GAIN_SIXTEEN.
float V01, V23;  // input voltage, after converting from bits
int InA01, InA23;  // input bits for ADS1115
float  I_C, I_B, beta;

//---------------------------------------------------
void setup()
{
  Wire.begin();
  ads1115.begin();
  ads1115.setGain(GAIN_SIXTEEN); // sets gain=16, range to run from 0 to +/- 0.256V

  // Start the serial link, at 9600 bps.
  Serial.begin(9600);
  Serial.println("# Lab5_MeasureBeta_16bit");
  
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
  
  // Uncomment to read in voltage from ADS1115
  InA01 = ads1115.readADC_Differential_0_1();  
  InA23 = ads1115.readADC_Differential_2_3();
  V01 = ADCVoltsPerBit*InA01;
  V23 = ADCVoltsPerBit*InA23;

  // Calculate I_C, I_B and beta here.
  I_C = V01/R;
  I_B = V23/R;
  beta = I_C/I_B;
 
  // ------Print the ADS1115 info
  
  //Serial.println(T_room-gain*(Vdiode-Voffset),3);
  Serial.print(V_DAC);
  Serial.print("\t");
  Serial.print(V01,7);
  Serial.print("\t");
  Serial.print(V23,7);
  Serial.print("\t");
  Serial.print(1000*I_C,7);
  Serial.print("\t");
  Serial.print(1000*I_B,7);
  Serial.print("\t");
  Serial.print(beta);
  Serial.println("");

}
