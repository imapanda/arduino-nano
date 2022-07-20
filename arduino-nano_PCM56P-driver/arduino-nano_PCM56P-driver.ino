#include <SPI.h>

#define SWITCHING_DELAY 10

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
SPISettings settings(20000000, MSBFIRST, SPI_MODE1);

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
  SPI.beginTransaction(settings);

  // take the SS pin low to select the chip:
  digitalWrite (chipSelectPin, LOW);

  SPI.transfer(0x7F);
  SPI.transfer(0xFF);
  //SPI.transfer(10, 0x7AAA, SPI_CONTINUE);
  //SPI.transfer(result);


  // take the SS pin high to unselect the chip:
  digitalWrite (chipSelectPin, HIGH);

  SPI.endTransaction();

  delay(SWITCHING_DELAY);
  
  //------------------ another
  SPI.beginTransaction(settings);

  // take the SS pin low to select the chip:
  digitalWrite (chipSelectPin, LOW);

  SPI.transfer(0x2F);
  SPI.transfer(0x22);


  // take the SS pin high to unselect the chip:
  digitalWrite (chipSelectPin, HIGH);

  SPI.endTransaction();
  delay(SWITCHING_DELAY);

    //------------------ another
  SPI.beginTransaction(settings);

  // take the SS pin low to select the chip:
  digitalWrite (chipSelectPin, LOW);

  SPI.transfer(0x80);
  SPI.transfer(0x00);


  // take the SS pin high to unselect the chip:
  digitalWrite (chipSelectPin, HIGH);

  SPI.endTransaction();
  delay(SWITCHING_DELAY);
  //Serial.println("Im ok");
}
