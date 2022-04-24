/*
  C.BRESSY
  24.04.2022

  Using 7 segments displays (common cathode) on Arduino Nano with logic gate for multiplexing.
  Those 2 x 7segments displays will be used to display temperature & humidity from DHT11 sensor
*/

#include <dht.h>
#include "Ticker.h" // See https://github.com/sstaub/Ticker

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

void update_dht11_values();
void update_counter_value();

// Temperature DHT11 settings
const unsigned int DHT11_PIN = 16; // For dht.h
const unsigned int DHT11_LED_PIN = 3; // For dht11 led on ticker

Ticker dht11_updater(update_dht11_values, 3000);  //calls function update_dht11_values() every 2 seconds, internal resolution is micros, running endless
Ticker counter_updater(update_counter_value, 1000);  //calls function update_dht11_values() every 2 seconds, internal resolution is micros, running endless
dht DHT;
float temp = 0;

// Settings for ticker
int show_clock = 1;

unsigned int counter = 0;


void displayDigit(int digit) {
  // Write values of digit using segCode onto SEGMENT_PINS
  for (int i = 0; i < 8; i++) {
    digitalWrite(SEGMENT_PINS[i], segCode[digit][i]);
  }
}


void update_counter_value(){
  counter++;
  if (counter > 99) {
    counter = 0;
  }
  Serial.println(counter);
}


void update_dht11_values() {
  //update_counter_value();
  
  //int chk = DHT.read11(DHT11_PIN);
  digitalWrite(DHT11_LED_PIN, HIGH);
  delay(7);
  digitalWrite(DHT11_LED_PIN, LOW);
  return;
}


// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);
  
  // initialize digital pins as an output.
  for (int i = 0; i < 8; i++) {
    pinMode(SEGMENT_PINS[i], OUTPUT);
  }

  pinMode(MULTIPLEXING_PIN, OUTPUT);
  
  pinMode(DHT11_PIN, INPUT);
  pinMode(DHT11_LED_PIN, OUTPUT);
  
  delay(1111);
  
  dht11_updater.start();
  counter_updater.start();
}

// the loop function runs over and over again forever
void loop() {
  
  dht11_updater.update();  // It will check the Ticker, and if necessary, will run the callback.
  counter_updater.update();


//  for (int i = 0; i < 1000 / DELAY / 2; i++) { // loops 10 times per second
    displayDigit(counter / 10);
    digitalWrite(MULTIPLEXING_PIN, HIGH);
    delay(DELAY);
    displayDigit(counter % 10);
    digitalWrite(MULTIPLEXING_PIN, LOW);
    delay(DELAY);
//  }


}
