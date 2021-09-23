/*
 * This program sets a digital pin high and then low, repeatedly.
 * Or, uses PWM.
 * We'll also use this to drive the gate of a transistor, 
 * with a lightbulb or heater or other device on the collector.
 */

// constants won't change. Used here to set a pin number:
const int OutPin =  6; // the number of the LED pin
const long interval = 1000;           // interval at which to blink (milliseconds)

//  When using PWM, this sets the brightness or speed.
int intensity = 255;  // 0 for "off", 255 for "maximum"

// Variables will change:
int State = LOW;             // ledState used to set the LED

void setup() {
  // set the digital pin as output:
  pinMode(OutPin, OUTPUT);
}

void loop() {
  
  // if the LED is off turn it on and vice-versa:
  if (State == LOW) {
    State = HIGH;
  } else {
    State = LOW;
  }

  // USE THIS FIRST
  // set the LED with the ledState of the variable:
  digitalWrite(OutPin, State);

  //USE THIS SECOND, to do PWM;  comment out the digitalWrite above, and uncomment this.
  //analogWrite(OutPin,intensity);

  delay(interval);
  
}
