/*
   C.BRESSY

   Original code by Noriaki Mitsunaga

*/
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT        32
#define SCREEN_RESET_PIN     -1 // -1 if your screen doesn't have reset pin
#define SCREEN_I2C_ADDRESS 0x3C

Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN);
// 3 colors available in Adafruit_SSD1306.h
#define SSD1306_BLACK 0             // Draw 'off' pixels
#define SSD1306_WHITE 1             // Draw 'on' pixels
#define SSD1306_INVERSE 2           // Invert pixels


#define txtLINE0   0
#define txtLINE1   16
#define txtLINE2   30
#define txtLINE3   46

const int LCD_WIDTH = 128;
// const int LCD_HEIGHT = 64;
const int LCD_HEIGHT = 32;
const int SAMPLES = 100;
const int ad_sw = 3;                    // Analog 3 pin for switches
const int ad_ch0 = 4;                   // Analog 4 pin for channel 0
const int ad_ch1 = 5;                   // Analog 5 pin for channel 1
const unsigned long VREF[] = {49, 98, 244, 488, 976}; // reference voltage 5.0V ->  50 :   1V/div range (100mV/dot)
//                        -> 100 : 0.5V/div
//                        -> 250 : 0.2V/div
//                        -> 500 : 100mV/div
//                       -> 1000 :  50mV/div
const int MILLIVOL_per_dot[] = {100, 50, 20, 10, 5}; // mV/dot
const int MODE_ON = 0;
const int MODE_INV = 1;
const int MODE_OFF = 2;
const char *Modes[] = {"ON", "INV", "OFF"};
const int TRIG_AUTO = 0;
const int TRIG_NORM = 1;
const int TRIG_SCAN = 2;
const int TRIG_ONE  = 3;
const char *TRIG_Modes[] = {"Auto", "Norm", "Scan", "One"};
const int TRIG_E_UP = 0;
const int TRIG_E_DN = 1;
#define RATE_MIN 0
#define RATE_MAX 13
const char *Rates[] = {"F1-1", "F1-2 ", "F2  ", "5ms", "10ms", "20ms", "50ms", "0.1s", "0.2s", "0.5s", "1s", "2s", "5s", "10s"};
#define RANGE_MIN 0
#define RANGE_MAX 4
const char *Ranges[] = {" 1V ", "0.5V", "0.2V", "0.1V", "50mV"};
byte range0 = RANGE_MIN;
byte range1 = RANGE_MIN;
byte ch0_mode = 0, ch1_mode = 0, rate = 5;
byte trig_mode = TRIG_AUTO, trig_lv = 30, trig_edge = TRIG_E_UP, trig_ch = 0;
byte Start = 1, menu = 0;
short ch0_off = 0, ch1_off = 0;
unsigned long startMillis;
byte data[4][SAMPLES];                   // keep twice of the number of channels to make it a double buffer
byte sample = 0;                         // index for double buffer*/

void drawGrid() {
  for (int x = 0; x <= SAMPLES; x += 2) { // Horizontal Line
    for (int y = 0; y <= 60; y += 10) {
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_BLACK);
      // TODO
      //CheckSW();
    }
  }
  for (int x = 0; x <= SAMPLES; x += 10 ) { // Vertical Line
    for (int y = 0; y <= 60; y += 2) {
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_BLACK);
      // TODO
      //CheckSW();
    }
  }
}

void drawGrid(int x) {
  int y;
  if ((x % 2) == 0)
    for (y = 0; y <= 60; y += 10)
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_BLACK); //SSD1306_WHITE?
  if ((x % 10) == 0)
    for (y = 0; y <= 60; y += 2)
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_BLACK);
}

void setup() {
  Serial.begin(9600);

  // Init OLED screen
  if (!screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS))
    while (1);

  screen.setTextSize(1);  // Caracters size (1 to 3)
  screen.setCursor(0, 0);

  screen.setTextColor(SSD1306_WHITE);

  screen.println("github.com/imapanda");
  screen.println("2022.04.29");
  screen.display();

  delay(1000); //Wait 2 seconds for boot logo

  screen.clearDisplay();
  screen.setCursor(0, 0);

  drawGrid();
  //DrawText();
}


void loop() {
  // put your main code here, to run repeatedly:
  screen.clearDisplay();
  delay(1000);
  drawGrid();
  delay(1000);
}
