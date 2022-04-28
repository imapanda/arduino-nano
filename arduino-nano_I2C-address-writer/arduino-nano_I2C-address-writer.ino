#include <Wire.h>

void setup() {
  Wire.begin(); // Enable I2C communication as master
  Serial.begin (9600); // Enable communication back to host PC at 19200 bps

  Wire.beginTransmission( 0x3C);
  int error = Wire.endTransmission();
  if( error != 0)
  {
    Serial.println( "The sensor is not at 0x3C");
  }
  else
  {
    Serial.println( "The sensor is found, changing I2C address - beginTransmission");
    Wire.beginTransmission( 0x3C);
    Serial.println( "write( 0x53);  // password register");
    Wire.write( 0x53);  // password register
    Serial.println( "write( 0xAA);  // password");
    Wire.write( 0xAA);  // password
    Serial.println( "endTransmission");
    Wire.endTransmission();
    Serial.println( "endTransmission done");
  
    delay(10);    // not described somewhere, just for safety
  
    
    Serial.println( "beginTransmission");
    Wire.beginTransmission( 0x3C);
    Serial.println( "write( 0x00);  // I2C address register");
    Wire.write( 0x00);  // I2C address register
    Serial.println( "write( 0x3D);  // new I2C address");
    Wire.write( 0x3D);  // new I2C address
    Serial.println( "endTransmission");
    Wire.endTransmission();
  }

}
void loop() {
  // Aucun code ici, car tout se passe dans la fonction setup !
}
