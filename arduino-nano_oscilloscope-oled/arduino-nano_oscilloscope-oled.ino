/*
   C.BRESSY

   Original code by Noriaki Mitsunaga

*/
#define MENU_BUTTONS


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

// Boot + status led
const uint8_t STATUS_LED_OK_PIN = 11;
const uint8_t STATUS_LED_KO_PIN = 12;
const uint8_t DEBUG_LED_PIN = 13;

#ifdef MENU_BUTTONS
// Menu buttons stuff
// Menu buttons pins
const uint8_t BTN_POS  = 8;
const uint8_t BTN_NEG  = 11;
const uint8_t BTN_MENU = 10;
// software debounce by using a timer
#include <Ticker.h>
volatile bool oldPushButtonState = false;

// set the debounce time here in [ms] ; 40 seems to be a good value :)
#define MAX_BOUND_DURATION_MS 40
#define DEBOUNCE_TIMER_INTERVAL_MS 1
#define MAX_BOUND_COUNT (MAX_BOUND_DURATION_MS/DEBOUNCE_TIMER_INTERVAL_MS)
// Push button debounce stuff
void update_debounceTimer();
Ticker debounceTimer(update_debounceTimer,   DEBOUNCE_TIMER_INTERVAL_MS, 0, MICROS_MICROS);

volatile int pushButtonStableLevelCount = 0;
volatile int pushButtonPushedCount = 0;
volatile int pushButtonReleasedCount = 0;
#endif


unsigned int counter = 0;

// PORT
#define txtLINE0   0
#define txtLINE1   8
#define txtLINE2   16
#define txtLINE3   24

const int SAMPLES = 100;
const int ad_sw = 3;                    // Analog 3 pin for switches

//Arduino nano analog pins : A0 to A7
const int ad_ch0 = A0;                   // Analog 1 pin for channel 0
const int ad_ch1 = A1;                   // Analog 2 pin for channel 1
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

void change_menu(){
  //called when menu button is pressed
  // We have 3 menus
  menu++;
  if(menu>2){menu=0;}  
}

#ifdef MENU_BUTTONS
bool getPushButtonState(void) {
  return digitalRead(BTN_MENU) == HIGH;
}

void update_debounceTimer() {

  digitalWrite(DEBUG_LED_PIN, HIGH);

  // read the current push button state
  bool newPushButtonState = getPushButtonState();

  // same state as previous => exit
  if (newPushButtonState == oldPushButtonState) {
    return;
  }

  // if old state is low (the user pushes the button)
  if (!oldPushButtonState) {

    if (newPushButtonState) { // button stills pushed
      pushButtonStableLevelCount++;
    }
    else { // button released
      pushButtonStableLevelCount = 0;
    }
    // button stills pushed enough longer to say "OK, no more bounce"
    if (pushButtonStableLevelCount >= MAX_BOUND_COUNT) {
      oldPushButtonState = true; // set internal state to PUSHED
      pushButtonPushedCount++;
      
      pushButtonStableLevelCount = 0;
    }
  }
  else {
    if (!newPushButtonState) { // button stills released
      pushButtonStableLevelCount++;
    }
    else { // button repushed
      pushButtonStableLevelCount = 0;
    }
    // button stills released enough longer to say "OK, no more bounce"
    if (pushButtonStableLevelCount >= MAX_BOUND_COUNT) {
      oldPushButtonState = false; // set internal state to RELEASED
      pushButtonReleasedCount++;
      change_menu();
      pushButtonStableLevelCount = 0;
    }
  }

  digitalWrite(DEBUG_LED_PIN, LOW);
}
#endif


void drawGrid() {
  // Draw a single pixel in white
  int16_t x, y;
  for (x = 0; x <= SAMPLES; x += 2) {  // Horizontal Line
    for (y = 0; y <= SCREEN_HEIGHT; y += 10) {
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_WHITE);
      // TODO
      //CheckSW();
      debounceTimer.update();
    }
  }
  for (x = 0; x <= SAMPLES; x += 10 ) {  // Vertical Line
    for (y = 0; y <= SCREEN_HEIGHT; y += 2) {
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_WHITE);
      // TODO
      //CheckSW();
      debounceTimer.update();
    }
  }
  screen.display(); // print what we've done
  return 0;

  /*display.clearDisplay(); // Clear display buffer

    for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
    }
    for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
    }*/
}


void drawGrid(int x) {
  int y;
  if ((x % 2) == 0)
    for (y = 0; y <= 60; y += 10)
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_BLACK); //SSD1306_WHITE?
  if ((x % 10) == 0)
    for (y = 0; y <= SCREEN_HEIGHT; y += 2)
      //GLCD.SetDot(x, y, BLACK);
      screen.drawPixel(x, y, SSD1306_BLACK);
  screen.display(); // print what we've done
  return 0;
}


void drawText() {
  digitalWrite(STATUS_LED_KO_PIN, HIGH);
  digitalWrite(STATUS_LED_OK_PIN, HIGH);
  //screen.FillRect(101,txtLINE0,28,64, WHITE);  // clear text area that will be drawn below
  screen.setTextColor(SSD1306_WHITE);  // Draw white text

  //Serial.println("[DrawText] - screen.fillRect");
  screen.fillRect(101, txtLINE0, 28, 32, SSD1306_BLACK);  // clear text area that will be drawn below
  //Serial.print("[DrawText] - switch");
  //Serial.println(menu);

  switch (menu) {
    case 0:
      //GLCD.GotoXY(SAMPLES + 1,txtLINE0);      // locate curser for printing text
      screen.setCursor(SAMPLES + 1, txtLINE0);  // locate curser for printing text
      //GLCD.Puts(Ranges[range0]);
      screen.print(Ranges[range0]);
      screen.setCursor(SAMPLES + 1, txtLINE1);  // locate curser for printing text
      screen.print(Ranges[range1]);
      screen.setCursor(SAMPLES + 1, txtLINE2);  // locate curser for printing text
      screen.print(Rates[rate]);
      screen.setCursor(SAMPLES + 1, txtLINE3);  // locate curser for printing text
      screen.print(TRIG_Modes[trig_mode]);
      break;
    case 1:
      screen.setCursor(SAMPLES + 1, txtLINE0);        // locate curser for printing text
      screen.print("OF1");
      screen.setCursor(SAMPLES + 1, txtLINE1);        // locate curser for printing text
      screen.print("OF2");
      screen.setCursor(SAMPLES + 1, txtLINE2);        // locate curser for printing text
      screen.print("Tlv");
      screen.setCursor(SAMPLES + 1, txtLINE3);        // locate curser for printing text
      screen.print(trig_edge == TRIG_E_UP ? "UP" : "DN");
      break;
    case 2:
      screen.setCursor(SAMPLES + 1, txtLINE0);        // locate curser for printing text
      screen.print(Modes[ch0_mode]);
      screen.setCursor(SAMPLES + 1, txtLINE1);        // locate curser for printing text
      screen.print(Modes[ch1_mode]);
      screen.setCursor(SAMPLES + 1, txtLINE2);        // locate curser for printing text
      screen.print(trig_ch == 0 ? "T:1" : "T:2");
      break;
  }
  screen.display(); // print what we've done
  digitalWrite(STATUS_LED_KO_PIN, LOW);
  digitalWrite(STATUS_LED_OK_PIN, HIGH);
  return 0;

}


void setup() {
  pinMode(STATUS_LED_OK_PIN, OUTPUT);
  pinMode(STATUS_LED_KO_PIN, OUTPUT);
  digitalWrite(STATUS_LED_KO_PIN, HIGH);

  pinMode(DEBUG_LED_PIN, OUTPUT);

  //Serial.begin(9600);
  Serial.begin(115200);

#ifdef MENU_BUTTONS
  // Setup of menu buttons
  pinMode(BTN_POS, INPUT);
  pinMode(BTN_NEG, INPUT);
  pinMode(BTN_MENU, INPUT);

  oldPushButtonState = getPushButtonState();
  //debounceTimer.attach(DEBOUNCE_TIMER_INTERVAL_MS / 1000., debounceHandler); // debounce period in [ms]
  debounceTimer.start();
#endif

  // Init OLED screen
  if (!screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS))
    while (1);
  screen.clearDisplay(); // use this line to remove Adafruit default logo

  screen.setTextSize(1);  // Caracters size (1 to 3)
  screen.setCursor(0, 0);

  screen.setTextColor(SSD1306_WHITE);

  screen.println("github.com/imapanda");
  screen.println("2022.04.30");
  screen.display();

  delay(1000); //Wait 1 second for boot logo

  screen.clearDisplay();
  screen.setCursor(0, 0);

  drawGrid();
  drawText();


  digitalWrite(STATUS_LED_KO_PIN, LOW);
  digitalWrite(STATUS_LED_OK_PIN, HIGH);
}


inline unsigned long adRead(byte ch, byte mode, int off) {
  unsigned long a = analogRead(ch);
  a = ((a + off) * VREF[ch == ad_ch0 ? range0 : range1] + 512) >> 10;
  a = a >= 61 ? 60 : a;
  if (mode == MODE_INV)
    return 60 - a;
  return a;
}


void clearAndDrawGraph() {
  int clear = 0;
  //Serial.print("[ClearAndDrawGraph]");
  //Serial.print(data[0][0]);
  //Serial.print(data[1][0]);
  //Serial.print(data[2][0]);
  //Serial.print(data[3][0]);
  //Serial.print("  -  ");
  //Serial.print(clear);
  //Serial.print("  -  ");
  //Serial.println(sample);
  int x;
  if (sample == 0)
    clear = 2;
#if 0
  for (x = 0; x < SAMPLES; x++) {
    //GLCD.SetDot(x, 60 - data[clear + 0][x], WHITE);
    screen.drawPixel(x, screen.height( ) - data[clear + 0][x], SSD1306_WHITE);
    screen.drawPixel(x, screen.height() - data[clear + 1][x], SSD1306_WHITE);
    screen.drawPixel(x, screen.height() - data[sample + 0][x], SSD1306_BLACK);
    screen.drawPixel(x, screen.height() - data[sample + 1][x], SSD1306_BLACK);
  }
#else
  for (x = 0; x < (SAMPLES - 1); x++) {
    //GLCD.DrawLine(x, 60 - data[clear + 0][x], x + 1, 60 - data[clear + 0][x + 1], WHITE);
    screen.drawLine(x, screen.height() - data[clear + 0][x], x + 1, screen.height() - data[clear + 0][x + 1], SSD1306_WHITE);
    screen.drawLine(x, screen.height() - data[clear + 1][x], x + 1, screen.height() - data[clear + 1][x + 1], SSD1306_WHITE);
    if (ch0_mode != MODE_OFF)
      screen.drawLine(x, screen.height() - data[sample + 0][x], x + 1, screen.height() - data[sample + 0][x + 1], SSD1306_BLACK);
    if (ch1_mode != MODE_OFF)
      screen.drawLine(x, screen.height() - data[sample + 1][x], x + 1, screen.height() - data[sample + 1][x + 1], SSD1306_BLACK);
    //TODO
    //CheckSW();
    debounceTimer.update();
  }
#endif
  screen.display(); // print what we've done
  return 0;
}


void clearAndDrawDot(int i) {
  int clear = 0;
  Serial.print("[clearAndDrawDot]");
  Serial.print(data[0][0]);
  Serial.print(data[1][0]);
  Serial.print(data[2][0]);
  Serial.print(data[3][0]);
  Serial.print("  -  ");
  Serial.print(clear);
  Serial.print("  -  ");
  Serial.println(sample);

  if (i <= 1)
    return;
  if (sample == 0)
    clear = 2;
#if 0
  for (int x = 0; x < SAMPLES; x++) {
    screen.drawPixel(x, screen.height() - data[clear + 0][x], SSD1306_WHITE);
    screen.drawPixel(x, screen.height() - data[clear + 1][x], SSD1306_WHITE);
    screen.drawPixel(x, screen.height() - data[sample + 0][x], SSD1306_BLACK);
    screen.drawPixel(x, screen.height() - data[sample + 1][x], SSD1306_BLACK);
  }
#else
  screen.drawLine(i - 1, screen.height() - data[clear + 0][i - 1], i, screen.height() - data[clear + 0][i], SSD1306_WHITE);
  screen.drawLine(i - 1, screen.height() - data[clear + 1][i - 1], i, screen.height() - data[clear + 1][i], SSD1306_WHITE);
  if (ch0_mode != MODE_OFF)
    screen.drawLine(i - 1, screen.height() - data[sample + 0][i - 1], i, screen.height() - data[sample + 0][i], SSD1306_BLACK);
  if (ch1_mode != MODE_OFF)
    screen.drawLine(i - 1, screen.height() - data[sample + 1][i - 1], i, screen.height() - data[sample + 1][i], SSD1306_BLACK);
#endif
  drawGrid(i);
  screen.display(); // print what we've done
  return 0;
}


void loop() {
  Serial.print(counter);
  Serial.print(" ");

  Serial.print(oldPushButtonState);
  Serial.print(" ");
  Serial.print(pushButtonStableLevelCount);
  Serial.print(" ");
  Serial.print(pushButtonPushedCount);
  Serial.print(" ");
  Serial.print(pushButtonReleasedCount);
  Serial.print(" Menu: ");
  Serial.print(menu);
  Serial.println(" ");
  //Serial.print(" ");


  debounceTimer.update();

  if (trig_mode != TRIG_SCAN) {

    unsigned long st = millis();
    byte oad;
    if (trig_ch == 0) {
      oad = adRead(ad_ch0, ch0_mode, ch0_off);
    }
    else {
      oad = adRead(ad_ch1, ch1_mode, ch1_off);
    }
    //Serial.print("oad: ");
    //Serial.println(oad);
    for (;;) {
      byte ad;
      if (trig_ch == 0)
        ad = adRead(ad_ch0, ch0_mode, ch0_off);
      else
        ad = adRead(ad_ch1, ch1_mode, ch1_off);
      //Serial.print("trig_edge == TRIG_E_UP ");
      //Serial.print(trig_edge == TRIG_E_UP);
      if (trig_edge == TRIG_E_UP) {
        if (ad >= trig_lv && ad > oad)
          break;
      } else {
        if (ad <= trig_lv && ad < oad)
          break;
      }
      oad = ad;
      //Serial.print("  FOR(;;) > oad");
      //Serial.print(oad);

      // TODO
      //CheckSW();
      debounceTimer.update();

      if (trig_mode == TRIG_SCAN)
        break;
      if (trig_mode == TRIG_AUTO && (millis() - st) > 100)
        break;
    }
  }
  //Serial.println("");
  //Serial.print("rate && start: ");
  //Serial.print(rate);
  //Serial.print(" - ");
  //Serial.println(Start);
  // sample and draw depending on the sampling rate
  if (rate <= 5 && Start) {
    if (sample == 0) {
      sample = 2;
    }
    else {
      sample = 0;
    }
    if (rate == 0) { // full speed, channel 0 only
      unsigned long st = millis();
      for (int i = 0; i < SAMPLES; i ++) {
        data[sample + 0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
      }
      for (int i = 0; i < SAMPLES; i ++)
        data[sample + 1][i] = 0;
      // Serial.println(millis()-st);
    } else if (rate == 1) { // full speed, channel 1 only
      unsigned long st = millis();
      for (int i = 0; i < SAMPLES; i ++) {
        data[sample + 1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      }
      for (int i = 0; i < SAMPLES; i ++)
        data[sample + 0][i] = 0;
      // Serial.println(millis()-st);
    } else if (rate == 2) { // full speed, dual channel
      unsigned long st = millis();
      for (int i = 0; i < SAMPLES; i ++) {
        data[sample + 0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
        data[sample + 1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      }
      // Serial.println(millis()-st);
    } else if (rate >= 3 && rate <= 5) { // .5ms, 1ms or 2ms sampling
      const unsigned long r_[] = {500, 1000, 2000};
      unsigned long st0 = millis();
      unsigned long st = micros();
      unsigned long r = r_[rate - 3];
      for (int i = 0; i < SAMPLES; i ++) {
        while ((st - micros()) < r) ;
        st += r;
        data[sample + 0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
        data[sample + 1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      }

      // Serial.println(millis()-st0);
    }
    clearAndDrawGraph();
    // TODO
    //CheckSW();
    debounceTimer.update();

    drawGrid();
    drawText();
  } else if (Start) { // 5ms - 500ms sampling
    // copy currently showing data to another
    if (sample == 0) {
      for (int i = 0; i < SAMPLES; i ++) {
        data[2][i] = data[0][i];
        data[3][i] = data[1][i];
      }
    } else {
      for (int i = 0; i < SAMPLES; i ++) {
        data[0][i] = data[2][i];
        data[1][i] = data[3][i];
      }
    }

    const unsigned long r_[] = {5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000};
    unsigned long st0 = millis();
    unsigned long st = micros();
    for (int i = 0; i < SAMPLES; i ++) {
      while ((st - micros()) < r_[rate - 6]) {  // here we wait
        //TODO
        //CheckSW();
        debounceTimer.update();

        if (rate < 6)
          break;
      }
      if (rate < 6) { // sampling rate has been changed
        //GLCD.ClearScreen();
        screen.clearDisplay();
        break;
      }
      st += r_[rate - 6];
      if (st - micros() > r_[rate - 6])
        st = micros(); // sampling rate has been changed to shorter interval
      if (!Start) {
        i --;
        continue;
      }
      data[sample + 0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
      data[sample + 1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      clearAndDrawDot(i);
    }
    // Serial.println(millis()-st0);
    drawGrid();
    drawText();
  } else {
    // TODO
    //CheckSW();
    debounceTimer.update();
  }

  if (trig_mode == TRIG_ONE) {
    Start = 0;
  }


  counter++;

}
