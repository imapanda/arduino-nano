/*
  C.BRESSY
  24.04.2022

  Using 7 segments displays (common cathode) on Arduino Nano with logic gate for multiplexing.
  Those 2 x 7segments displays will be used to display temperature & humidity from DHT11 sensor
*/

#include <dht.h>
#include <Ticker.h>  //Ticker Library

const unsigned int DELAY = 50;

// GPIO Pinout for multiplexing displays, this is done with hardware logic gates.
// This allows us to save gpio pins for other uses.
const unsigned int MULTIPLEXING_PIN = 5;

// GPIO Pinout for 7 segment display
const unsigned int SEGMENT_PINS[] = {12, 11, 10, 9, 8, 7, 6, 5 };   // { a b c d e f g . )
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


// Temperature DHT11 settings
const unsigned int DHT11_PIN 16 // For dht.h

Ticker myClock;
dht DHT;
float temp = 0;

// Settings for ticker
int show_clock=1;

void updateTempAndDisplay() {
int chk = DHT.read11(DHT11_PIN);
// DHT.temperature
// DHT.humidity

  
  
  show_clock = ms<500;
  
  
  if(show_clock) {  // Display temp for 1/2 second
    sprintf(s, "%02d", 33); // print to string
    for (int digit = 0; digit < NUM_DIGIT; digit++) {
      chip_disable();
      digit_select(digit);
      x = s[NUM_DIGIT - 1 - digit];
      //Serial.println(x);
      set_data(x);
      write_disable();
      delayMicroseconds(50);      // pauses for 50 microseconds
      write_enable();
      digit_unselect();
      chip_enable();

      delayMicroseconds(50);      // pauses for 50 microseconds
    }

  
  }
  else {  // Display temp for 1/2 second
    sprintf(s, "%02d", 99); // print to string
    for (int digit = 0; digit < NUM_DIGIT; digit++) {
      chip_disable();
      digit_select(digit);
      x = s[NUM_DIGIT - 1 - digit];
      //Serial.println(x);
      set_data(x);
      write_disable();
      delayMicroseconds(50);      // pauses for 50 microseconds
      write_enable();
      digit_unselect();
      chip_enable();

      delayMicroseconds(50);      // pauses for 50 microseconds
    }
  }


  //always update hhmmss
  ms+=displayPeriodMS;
  if(ms>=1000) {
    ms-=1000;
    ss++;
    if (ss > 59) {
      ss = 0;
      mm++;
    }
    if (mm > 59) {
      mm = 0;
      hh++;
    }
    if (hh > 23) {
      hh = 0;
    }
  
  }
 
  return;
}




void displayDigit(int digit)
{
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

  for (int n = 0; n < 10; n++) {    // display digits 0 - 9 and decimal point
    displayDigit(n);
    for (int i = 0; i < 200; i++) {
      digitalWrite(MULTIPLEXING_PIN, HIGH);
      delay(1);
      digitalWrite(MULTIPLEXING_PIN, LOW);
      delay(1);
    }
  }



  digitalWrite(MULTIPLEXING_PIN, HIGH);
  //  digitalWrite(SEGMENT_A, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_A, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_B, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_B, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_C, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_C, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_D, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_D, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_E, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_E, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_F, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_F, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_G, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_G, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(MULTIPLEXING_PIN, LOW);
  //  digitalWrite(SEGMENT_A, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_A, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_B, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_B, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_C, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_C, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_D, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_D, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_E, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_E, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_F, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_F, LOW);
  //  delay(DELAY);
  //
  //  digitalWrite(SEGMENT_G, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(DELAY);
  //  digitalWrite(SEGMENT_G, LOW);
  //  delay(DELAY);

  //  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(100);                       // wait for a second
  //  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //  delay(100);                       // wait for a second
}
