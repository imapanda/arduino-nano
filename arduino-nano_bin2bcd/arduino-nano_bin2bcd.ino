/*
 * 
 * C.BRESSY - P.BRESSY
 * 2022.05.16
 * 
 * Simulation of binary to BCD using Arduino Nano using MCP23008 gpio extender.
 * Tables from https://tableconvert.com/ <3
 * 
 * Interrupts on GPIO moved => re-calculate output pins?
 * 
 * Truth table of add3 module:
 * | VALUE_BIN | A3 | A2 | A1 | A0 | VALUE_BCD | S3 | S2 | S1 | S0 | CONVERTED |
 * |-----------|----|----|----|----|-----------|----|----|----|----|-----------|
 * | 0         | 0  | 0  | 0  | 0  | 0         | 0  | 0  | 0  | 0  | 0         |
 * | 1         | 0  | 0  | 0  | 1  | 1         | 0  | 0  | 0  | 1  | 1         |
 * | 2         | 0  | 0  | 1  | 0  | 2         | 0  | 0  | 1  | 0  | 2         |
 * | 3         | 0  | 0  | 1  | 1  | 3         | 0  | 0  | 1  | 1  | 3         |
 * | 4         | 0  | 1  | 0  | 0  | 4         | 0  | 1  | 0  | 0  | 4         |
 * | 5         | 0  | 1  | 0  | 1  | 5         | 1  | 0  | 0  | 0  | 8         |
 * | 6         | 0  | 1  | 1  | 0  | 6         | 1  | 0  | 0  | 1  | 9         |
 * | 7         | 0  | 1  | 1  | 1  | 7         | 1  | 0  | 1  | 0  | 10        |
 * | 8         | 1  | 0  | 0  | 0  | 8         | 1  | 0  | 1  | 1  | 11        |
 * | 9         | 1  | 0  | 0  | 1  | 9         | 1  | 1  | 0  | 0  | 12        |
 * | 10        | 1  | 0  | 1  | 0  | X         | X  | X  | X  | X  | -         |
 * | 11        | 1  | 0  | 1  | 1  | X         | X  | X  | X  | X  | -         |
 * | 12        | 1  | 1  | 0  | 0  | X         | X  | X  | X  | X  | -         |
 * | 13        | 1  | 1  | 0  | 1  | X         | X  | X  | X  | X  | -         |
 * | 14        | 1  | 1  | 1  | 0  | X         | X  | X  | X  | X  | -         |
 * | 15        | 1  | 1  | 1  | 1  | X         | X  | X  | X  | X  | -         |
 * 
 * 
 * --> add3 module S0 :
 * | S0_A3A2/A1A0 | 00 | 01 | 11 | 10 |
 * |-----------|----|----|----|----|
 * | 00        | 0  | 1  | 1  | 0  |
 * | 01        | 0  | 0  | 0  | 1  |
 * | 11        | X  | X  | X  | X  |
 * | 10        | 1  | 0  | X  | X  |
 * S0 = A3'A2'A0 + A2A1A0' + A3A0'
 * 
 * 
 * --> add3 module S1 : 
 * | S1_A3A2/A1A0 | 00 | 01 | 11 | 10 |
 * |--------------|----|----|----|----|
 * | 00           | 0  | 0  | 1  | 1  |
 * | 01           | 0  | 0  | 1  | 0  |
 * | 11           | X  | X  | X  | X  |
 * | 10           | 1  | 0  | X  | X  |
 * S1 = A1A0 + A3A0' + A2'A1A0'
 * 
 * 
 * --> add3 module S2 :
 * | S2_A3A2/A1A0 | 00 | 01 | 11 | 10 |
 * |--------------|----|----|----|----|
 * | 00           | 0  | 0  | 0  | 0  |
 * | 01           | 1  | 0  | 0  | 0  |
 * | 11           | X  | X  | X  | X  |
 * | 10           | 0  | 1  | X  | X  |
 * S2 = A2A1'A0' + A3A0
 * 
 * 
 * --> add3 module S3 :
 * | S3_A3A2/A1A0 | 00 | 01 | 11 | 10 |
 * |--------------|----|----|----|----|
 * | 00           | 0  | 0  | 0  | 0  |
 * | 01           | 0  | 1  | 1  | 1  |
 * | 11           | X  | X  | X  | X  |
 * | 10           | 1  | 1  | X  | X  |
 * S3 = A3 + A2A0 + A2A1
 * 
 */

const uint8_t INPUT_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9 };   // IN_7 to IN_0

// 8 input pins (counting from 0 to 255)
boolean IN_VALUES[8];

// 10 output pins 2/4 BCD (0-2) + 2x4 BCD (0-9) :
boolean OUT_VALUES[10];

boolean _add3_S0(bool A0, bool A1, bool A2, bool A3){
  return 0;  // default when result of truth table is X
}
boolean _add3_S1(bool A0, bool A1, bool A2, bool A3){
  return 0;  // default when result of truth table is X
}
boolean _add3_S2(bool A0, bool A1, bool A2, bool A3){
  return 0;  // default when result of truth table is X
}
boolean _add3_S3(bool A0, bool A1, bool A2, bool A3){
  return 0;  // default when result of truth table is X
}

boolean * add3(bool A0, bool A1, bool A2, bool A3){
  boolean add3_val[] = {0,0,0,0};  // outputs
  return add3_val;  // default when result of truth table is X
}

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(INPUT_PINS[i], INPUT);
  }
  
  // TODO : setup MCP23008
//  for (int i = 0; i < 8; i++) {
//    pinMode(OUTPUT_PINS[i], INPUT);
//  }
}

void loop() {}
