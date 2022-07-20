/*
 * 20.07.2022 - C.BRESSY
 * 
 * Using MCP56P to generate audio waves
 * 
 * Pins used on arduino => MCP56 :
 *  - D13 (SCK)   =>  7 (Data)
 *  - D11 (MOSI)  =>  6 (LE)
 *  - D10 (SS)    =>  5 (CLK)
 */

#include <SPI.h>

#define SWITCHING_DELAY 10000

// DIGITAL INPUT CODES FOR PCM56P:
//  |-------------------------------  |---------------  |-------------------- |-------------------------  |
//  | DIGITAL INPUT                   | ANALOG OUTPUT   |                     |                           |
//  |-------------------------------  |---------------  |-------------------- |-------------------------  |
//  | Binary Two's complement (BTC)   | DAC Output      | Voltage, Vout Mode  | Current (mA), Iout Mode   |
//  |-------------------------------  |---------------  |-------------------- |-------------------------  |
//  | 7FFF Hex                        | + Full scale    | +2.999908           | -0.999970                 |
//  | 8000 Hex                        | - Full scale    | -3.000000           | +1.000000                 |
//  | 0000 Hex                        | Bipolar Zewo    | 0.000000            | 0.000000                  |
//  | FFFF Hex                        | Zero -1 LSB     | -0.000092           | +0.030500uA               |
//  |-------------------------------  |---------------  |-------------------- |-------------------------  |


//Mode      Clock Polarity (CPOL) Clock Phase (CPHA)  Output Edge Data Capture
//SPI_MODE0 0                     0                   Falling     Rising
//SPI_MODE1 0                     1                   Rising     Falling
//SPI_MODE2 1                     0                   Rising     Falling
//SPI_MODE3 1                     1                   Falling     Rising

const int chipSelectPin = 10;

// PCM56P is MSB first
// 15000000 = 15MHz
SPISettings settings(15000000, MSBFIRST, SPI_MODE0);

void setup() {
  pinMode(chipSelectPin, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Im ready.");
}

void loop() {
  int i = 0;
  int increments = 5000;
  for(i = -32768; i < 32768;){
    write_spi_value(i);
    delayMicroseconds(SWITCHING_DELAY);
    i = i + increments;
  }
  
  i = 0;
  for(i = -32768; i < 32768;){
    write_spi_value(i * (-1));
    delayMicroseconds(SWITCHING_DELAY);
    i = i + increments;
    Serial.println(i);
  }

  
//  write_spi_value(0);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0x7FFF);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0x33FF);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0x0000);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0xCCCC);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0x8000);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0xCCCC);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0x0000);
//  delayMicroseconds(SWITCHING_DELAY);
//  write_spi_value(0x33FF);
}

bool write_spi_value(int value){
  // take the SS pin low to select the chip:
  SPI.beginTransaction(settings);
  digitalWrite (chipSelectPin, LOW);

  SPI.transfer((char)(value>>8));  // LSB
  SPI.transfer((char)(value));         // MSB first
//  SPI.transfer(value);

  
  // take the SS pin high to unselect the chip:
  digitalWrite (chipSelectPin, HIGH);
  SPI.endTransaction();
}
