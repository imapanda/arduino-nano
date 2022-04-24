/*
  C.BRESSY
  24.04.2022

  Using 7 segments displays (common cathode) on Arduino Nano with logic gate for multiplexing.
*/

const unsigned int DELAY = 50;

// GPIO Pinout for multiplexing displays, this is done with hardware logic gates.
// This allows us to save gpio pins for other uses.
const unsigned int MULTIPLEXING_PIN = 5;

// GPIO Pinout for 7 segment display
const unsigned int SEGMENT_A = 12;
const unsigned int SEGMENT_B = 11;
const unsigned int SEGMENT_C = 10;
const unsigned int SEGMENT_D = 9;
const unsigned int SEGMENT_E = 8;
const unsigned int SEGMENT_F = 7;
const unsigned int SEGMENT_G = 6;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins as an output.
  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);
  
  pinMode(MULTIPLEXING_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(MULTIPLEXING_PIN, HIGH); 
  digitalWrite(SEGMENT_A, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_A, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_B, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_B, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_C, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_C, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_D, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_D, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_E, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_E, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_F, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_F, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_G, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_G, LOW);
  delay(DELAY);
  
  digitalWrite(MULTIPLEXING_PIN, LOW);
  digitalWrite(SEGMENT_A, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_A, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_B, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_B, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_C, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_C, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_D, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_D, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_E, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_E, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_F, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_F, LOW);
  delay(DELAY);

  digitalWrite(SEGMENT_G, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);
  digitalWrite(SEGMENT_G, LOW);
  delay(DELAY);
  
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(100);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  delay(100);                       // wait for a second
}
