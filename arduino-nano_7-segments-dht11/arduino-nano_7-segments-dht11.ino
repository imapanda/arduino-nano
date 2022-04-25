/*
  C.BRESSY & P.BRESSY
  24.04.2022

  Using 7 segments displays (common cathode) on Arduino Nano with logic gate for multiplexing.
  Those 2 x 7segments displays will be used to display temperature & humidity from DHT11 sensor
*/

#include <dht.h>
#include "Ticker.h" // See https://github.com/sstaub/Ticker for more infos

const unsigned int DISPLAY_TICKS = 2;  // 25 images secondes = 40ms = 1/25; 100hz = 10ms
const unsigned int DHT_11_READ_TIMING = 4000;  // Delay between each DHT11 reads
const unsigned int DISPLAY_MODE_INTERVAL = 2000;  // Change display screen timer

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

void update_dht11_values(void);
void update_counter_value(void);
void update_display(void);
void displayTemp(void);
void displayHumidity(void);

// DHT11 settings
const unsigned int DHT11_PIN = 4;       // For dht.h
const unsigned int DHT11_LED_PIN = 3;   // For dht11 led on ticker
const unsigned int DISPLAY_LED_PIN = 2;

Ticker dht11_updater   (update_dht11_values, DHT_11_READ_TIMING);       //calls function update_dht11_values() every DHT_11_READ_TIMING seconds, internal resolution is micros, running endless
Ticker counter_updater (update_counter_value, DISPLAY_MODE_INTERVAL);   // Change display screen timer
Ticker display_update (update_display, DISPLAY_TICKS);                  //25 images secondes = 40ms = 1/25; 100hz = 10ms

dht DHT;
float temp = 0;

typedef enum {  // This is used for displaying tens and units each at a time
  E_DIZ = 0,
  E_UNIT
} eDigit;

eDigit digit = E_DIZ;  // On rythm with the call every DISPLAY_TICKS is change state

unsigned int counter = 0;
unsigned int temperature_value = 0;
unsigned int humidity_value = 0;


void (*functions[2])(void) = {displayTemp, displayHumidity};  // Array of pointers to functions


void displayDigit(int digit) {
  // Write values of digit using segCode onto SEGMENT_PINS
  int i;
  for (i = 0; i < 8; i++) {
    digitalWrite(SEGMENT_PINS[i], segCode[digit][i]);
  }
  return;
}


void update_counter_value(void) {
  //  counter++;
  //  if (counter > 1) {
  //    counter = 0;
  //  }
  //  Serial.println(counter);
  //
  // trick 1 "little player" :
  counter = 1 - counter;  // alternate between 0 and 1 values
  return;
}


void update_dht11_values(void) {

  //Light up temp reading led
  digitalWrite(DHT11_LED_PIN, HIGH);

  // Display "-" on display (digit one) while reading :
  int i;
  for (i = 0; i < 8; i++) { // clearing
    digitalWrite(SEGMENT_PINS[i], 0);
  }
  digitalWrite(6, HIGH);
  

  DHT.read11(DHT11_PIN);
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature);
  
  // DHT11 has this limit, it can read temperature and humidity only by integer value.
  // If you need better precision up to 0.1° then use DHT22 which is more advanced and expensive of course.
  Serial.println("C  ");
  //int chk = DHT.read11(DHT11_PIN);

  delay(200); // used to simulate more dht11 latency
  digitalWrite(DHT11_LED_PIN, LOW);

  return;
}

void displayTemp(void) {

  digitalWrite(DISPLAY_LED_PIN, HIGH);
  if (digit == E_DIZ) {  // Enum tens mode
    displayDigit(DHT.temperature / 10);
    digitalWrite(MULTIPLEXING_PIN, HIGH);
    digit = E_UNIT;
  } else {  // Enum units mode
    displayDigit((int)(DHT.temperature + .5) % 10); // (int)(g+0.5);// Converts float to int. +.5 works if g is positive only (for rounding value), -0.5 if negative
    digitalWrite(MULTIPLEXING_PIN, LOW);
    digit = E_DIZ;
  }

  return;
}

void displayHumidity(void) {

  digitalWrite(DISPLAY_LED_PIN, LOW);
  if (digit == E_DIZ) {  // Enum tens mode
    displayDigit(DHT.humidity / 10);
    digitalWrite(MULTIPLEXING_PIN, HIGH);
    digit = E_UNIT;
  } else {  // Enum units mode
    displayDigit((int)(DHT.humidity + .5) % 10);
    digitalWrite(MULTIPLEXING_PIN, LOW);
    digit = E_DIZ;
  }

  return;
}


// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);

  // initialize digital pins as outputs.
  for (int i = 0; i < 8; i++) {
    pinMode(SEGMENT_PINS[i], OUTPUT);
  }

  pinMode(MULTIPLEXING_PIN, OUTPUT);

  pinMode(DHT11_PIN, INPUT);
  pinMode(DHT11_LED_PIN, OUTPUT);
  pinMode(DISPLAY_LED_PIN, OUTPUT);

  delay(1234); // Simulates boots time

  dht11_updater.start();
  counter_updater.start();
  display_update.start();
  
  return;
}


void update_display(void) {
  functions[counter & 1]();  // switch between each of the 2 functions of the array. Easy to add/remove functions from the array.
  
  return;
}


void loop() {

  dht11_updater.update();  // It will check the Ticker, and if necessary, will run the callback.
  counter_updater.update();
  display_update.update();

  return;
}
