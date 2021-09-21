
#include <Wire.h>  //Include the Wire library to talk I2C
#include <Adafruit_ADS1X15.h>   // the library for the A/D card
Adafruit_ADS1115 ads1115;//(0x48); // construct an ads1115 object at address 0x48

// For DAC
#define MCP4725_ADDR 0x60   
float V_out;
float delta_V = 1.0; // voltage increment each time through the loop.
int code1;
int Code_out;
int bits_4, bits_8;

//Variables needed for the onboard ADC
float Vmax = 5.12;   // This is the voltage of my Arduino +5V, measured with my multimeter.
float Arduino_VoltsPerBit = Vmax/1023;  // 2^10 - 1 
float Arduino_volts;
int Arduino_counts;


// Variables needed for the ADS1115
//  Calculate the conversion between bits and volts.
//  One bit is a sign bit, and 2^15 = 32768.
//  The default gain sets the full ADC range to 6.144 Volts,
//    even though we'll only use 0 to +5V.
float ADS1115_VoltsPerBit = 6.144/(32767);  // conversion from bits to volts
float ADS1115_volts;  // input voltage, after converting from bits
int ADS1115_counts;  // input bits (count) for ADS1115

//---------------------------------------------------
void setup()
{
  // Start the serial link, at 9600 bps.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Started");

  // Initialize DAC;
  Code_out = 0;
  V_out = 0;

  // Start up the I2C bus so we can talk to the ADS1115
  Wire.begin();  // Required for the DAC.
  ads1115.begin();
}

//---------------------------------------------------
void loop()
{
  // Write to DAC
  // First ensure that V_out is between 0 and Vmax
  code1 = (V_out/Vmax)*4095;
  
  Code_out = code1 % 4095;  // Modulus;  this gives us the remainder, in case our code is too large.

  // ------Unpack the code into the 8 most significant bits, the 4 least significant bits, then
  // ------send everything over the I2C bus to the MCP4715.
  bits_8 = Code_out >> 4;
  bits_4 = (Code_out & 15) << 4;
  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(64);            // cmd to update the DAC
  Wire.write(bits_8);        // the 8 most significant bits...
  Wire.write(bits_4);        // the 4 least significant bits...
  Wire.endTransmission();

  delay(10); // let it sit 10ms before reading with A/D's.

  
  //Read in the on-board ADC channel A0
  Arduino_counts = analogRead(A0);
  Arduino_volts = Arduino_VoltsPerBit*Arduino_counts;
  
  // Uncomment to read in voltage from ADS1115
  ADS1115_counts = ads1115.readADC_SingleEnded(0);  // Read channel 0
  ADS1115_volts = ADS1115_VoltsPerBit*ADS1115_counts;


  // Print the onboard ADC result to the Serial bus
  Serial.print(Arduino_counts);  // bits
  Serial.print("\t");
  Serial.print(Arduino_volts,6);   // voltage
  Serial.print("\t");

//  Print the ADS1115 info
  Serial.print(ADS1115_counts);
  Serial.print("\t");
  Serial.print(ADS1115_volts, 6);

  Serial.println("");  // line feed

  delay(2000);  // Change this to change the sampling rate

  V_out = V_out + delta_V;
  if (V_out > Vmax)
  {
    V_out = 0;
  }
  Code_out = int((V_out/Vmax)*4095);

}
