/*
  C.BRESSY
  24.04.2022

  Using 7 segments displays (common cathode) on Arduino Nano with logic gate for multiplexing.
*/

const unsigned int DELAY = 75;

// GPIO Pinout for multiplexing displays, this is done with hardware logic gates.
// This allows us to save gpio pins for other uses.
const unsigned int MULTIPLEXING_PIN = 5;

// GPIO Pinout for 7 segment display
const unsigned int SEGMENT_PINS[] = {12, 11, 10, 9, 8, 7, 6, 5 };   // { a b c d e f g . )

unsigned int counter = 0;

const byte segCode[11][8] = {
  //  a  b  c  d  e  f  g  .
  { 1, 1, 1, 1, 1, 1, 0, 0},  // 0
  { 0, 1, 1, 0, 0, 0, 0, 0},  // 1
  { 1, 1, 0, 1, 1, 0, 1, 0},  // 2
  { 1, 1, 1, 1, 0, 0, 1, 0},  // 3
  { 0, 1, 1, 0, 0, 1, 1, 0},  // 4
  { 1, 0, 1, 1, 0, 1, 1, 0},  // 5
  { 1, 0, 1, 1, 1, 1, 1, 0},  // 6
  { 1, 1, 1, 0, 0, 0, 0, 0},  // 7
  { 1, 1, 1, 1, 1, 1, 1, 0},  // 8
  { 1, 1, 1, 1, 0, 1, 1, 0},  // 9
  { 0, 0, 0, 0, 0, 0, 0, 1}   // .
};

void displayDigit(int digit) {
  // Write values of digit using segCode onto SEGMENT_PINS
  for (int i = 0; i < 8; i++) {
    digitalWrite(SEGMENT_PINS[i], segCode[digit][i]);
  }
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins as an output.
  for (int i = 0; i < 8; i++) {
    pinMode(SEGMENT_PINS[i], OUTPUT);
  }
  
  pinMode(MULTIPLEXING_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  // loop every 1 second approx.
  // 1 second = 1,000,000 microseconds
  // 1 second = delay(1000)
  
  
  for (int i = 0; i < 1000/DELAY/2; i++) { // loops 10 times per second
    displayDigit(counter/10);
    digitalWrite(MULTIPLEXING_PIN, HIGH);
    delay(DELAY);
    displayDigit(counter%10);
    digitalWrite(MULTIPLEXING_PIN, LOW);
    delay(DELAY);
  }
  
  //delay(1000);
  
  counter++;
  if(counter > 99){counter=0;}
}
