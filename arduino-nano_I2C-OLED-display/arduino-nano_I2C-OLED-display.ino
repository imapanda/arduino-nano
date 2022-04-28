// 2022.04.28 - C.BRESSY
// Testing OLED displays

// I2C working with 7 bits not 8 bits : 
//    - 0x27 = 00100111 => 7bits : 0010011 => 0x13
//    - 0x3f = 00111111 => 7bits : 0011111 => 0x1F
// Mine is 0x3C, you can use I2C finder on same repo : https://github.com/imapanda/arduino-nano/blob/main/arduino-nano_I2C-scanner/arduino-nano_I2C-scanner.ino

// 2 libraries available :
// - « U8G2 » from Olikraus : complete but hard
// - « SSD1306 » from Adafruit : easy

#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT        32
#define SCREEN_RESET_PIN     -1 // -1 if your screen doesn't have reset pin
#define SCREEN_I2C_ADDRESS 0x3C
//#define SCREEN_I2C_ADDRESS 0x3D

// &Wire => includes library « Wire » in instanciation, indicates we are working on « I2C » OLED 
Adafruit_SSD1306 ecranOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN);
Adafruit_SSD1306 ecranOLED_2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN);

// 3 colors available in Adafruit_SSD1306.h
#define SSD1306_BLACK 0             // Draw 'off' pixels
#define SSD1306_WHITE 1             // Draw 'on' pixels
#define SSD1306_INVERSE 2           // Invert pixels


void setup() {

    // Init OLED screen
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS))
    while(1);                               // Infinite loop if can't init screen
  // Init OLED screen 2
  //if(!ecranOLED_2.begin(SSD1306_SWITCHCAPVCC, 0x3D))
  //  while(1);                               // Infinite loop if can't init screen

  ecranOLED.clearDisplay();
  ecranOLED.display();
  
  ecranOLED_2.clearDisplay();
  ecranOLED_2.display();

  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(2);  // Caracters size (1 to 3)
  ecranOLED.setCursor(0, 0);

  ecranOLED.setTextColor(SSD1306_WHITE);

  ecranOLED.println("ecranOLED");

  ecranOLED.display();                            // Transfer buffer to screen
  delay(2000);
  
  ecranOLED_2.clearDisplay();
  ecranOLED_2.setTextSize(1);  // Caracters size (1 to 3)
  ecranOLED_2.setCursor(0, 0);

  ecranOLED_2.setTextColor(SSD1306_WHITE);

  ecranOLED_2.println("ecranOLED_2");

  ecranOLED_2.display();                            // Transfer buffer to screen
  delay(2000);
//  int16_t posX = 40;
//  int16_t posY = 20;
//  int16_t couleur = SSD1306_WHITE;
//  ecranOLED.drawPixel(posX, posY, couleur);

}

void loop() {}
