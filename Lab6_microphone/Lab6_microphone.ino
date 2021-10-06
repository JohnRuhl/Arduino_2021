// This is for Lab6.  It reads in A0 from the on-board ADC.
// This is for the microphone reading part of the lab.
//

int A0pin = 0; // select the input pin for the potentiometer
int Bits0 = 0;  // variable to store the value coming from the sensor
float ADCVoltsPerBit = 5.17/1024;  //10 bit A/D, and my +5V rail is actually at 5.17V.
float V0;
unsigned long t=0;  // allows for big integers

void setup()
{
  // Start the serial link, at 115200 bps for fast printing of A/D values
  Serial.begin(115200);
  Serial.println("# Lab6_microphone");
}

//---------------------------------------------------
void loop()
{
  // ------ Find the time, calculate the output voltage.
  t = millis();   // time in seconds;  this overflows after about 70 minutes.
  //-------Read in the on-board ADC channel.
  Bits0 = analogRead(A0pin);
  V0 = ADCVoltsPerBit*Bits0;
  //Serial.print(t); // Uncommenting this and using the serial monitor allows you to count the number of samples in 10ms.
  //Serial.print("\t");
  Serial.println(V0);  

  //delay(1);  //  Comment this out to get faster sampling.
} 
