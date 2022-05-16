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

boolean _add3_S0(bool _a3, bool _a2, bool _a1, bool _a0){
  // S0 = A3'A2'A0 + A2A1A0' + A3A0'
  return digitalRead(INPUT_PINS[7]);
}
boolean _add3_S1(bool _a3, bool _a2, bool _a1, bool _a0){
  // S1 = A1A0 + A3A0' + A2'A1A0'
  return digitalRead(INPUT_PINS[6]);
}
boolean _add3_S2(bool _a3, bool _a2, bool _a1, bool _a0){
  // S2 = A2A1'A0' + A3A0
  return digitalRead(INPUT_PINS[5]);
}
boolean _add3_S3(bool _a3, bool _a2, bool _a1, bool _a0){
  // S3 = A3 + A2A0 + A2A1
  return digitalRead(INPUT_PINS[4]);
}

boolean * add3(bool _a3, bool _a2, bool _a1, bool _a0){
  boolean add3_val[] = {
    _add3_S3(_a3,_a2,_a1,_a0),
    _add3_S2(_a3,_a2,_a1,_a0),
    _add3_S1(_a3,_a2,_a1,_a0),
    _add3_S0(_a3,_a2,_a1,_a0)
    };
  return add3_val;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setup begin.");
  
  for (int i = 0; i < 8; i++) {
    pinMode(INPUT_PINS[i], INPUT);
  }
  
  // TODO : setup MCP23008
//  for (int i = 0; i < 8; i++) {
//    pinMode(OUTPUT_PINS[i], INPUT);
//  }
  Serial.println("Setup done.");
}
int count = 0;
void loop() {
  Serial.print(count);
  Serial.print(" - ");
  Serial.print(add3(INPUT_PINS[3],INPUT_PINS[2],INPUT_PINS[1],INPUT_PINS[0])[0]);
  Serial.print(add3(INPUT_PINS[3],INPUT_PINS[2],INPUT_PINS[1],INPUT_PINS[0])[1]);
  Serial.print(add3(INPUT_PINS[3],INPUT_PINS[2],INPUT_PINS[1],INPUT_PINS[0])[2]);
  Serial.print(add3(INPUT_PINS[3],INPUT_PINS[2],INPUT_PINS[1],INPUT_PINS[0])[3]);
  Serial.print(" - ");
  boolean tmp_result[4] = {add3(INPUT_PINS[3],INPUT_PINS[2],INPUT_PINS[1],INPUT_PINS[0])};
  int i = 0;
  for(i = 0; i < 4; i++){
    Serial.print(i);
    Serial.print(":");
    Serial.print(tmp_result[i]);
    Serial.print("; ");
  }
  Serial.print("-- ");
  Serial.print(digitalRead(INPUT_PINS[7]));
  Serial.print(digitalRead(INPUT_PINS[6]));
  Serial.print(digitalRead(INPUT_PINS[5]));
  Serial.println(digitalRead(INPUT_PINS[4]));
  delay(200);
  count++;
}
