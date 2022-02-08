int pin1 = 2;
int pin2 = 10;

#define IO_REG_TYPE      uint8_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

  volatile IO_REG_TYPE * pin1_register;
  volatile IO_REG_TYPE * pin2_register;
  IO_REG_TYPE            pin1_bitmask;
  IO_REG_TYPE            pin2_bitmask;
  uint8_t                state;
  volatile int32_t                position;


void setup()
{
  Serial.begin(115200);
  
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);

  pin1_register = PIN_TO_BASEREG(pin1);
  pin1_bitmask = PIN_TO_BITMASK(pin1);
  pin2_register = PIN_TO_BASEREG(pin2);
  pin2_bitmask = PIN_TO_BITMASK(pin2);
  position = 0;

  delayMicroseconds(2000);
  uint8_t s = 0;
  if (DIRECT_PIN_READ(pin1_register, pin1_bitmask)) s |= 1;
  if (DIRECT_PIN_READ(pin2_register, pin2_bitmask)) s |= 2;
  state = s;

  EICRB = (EICRB & 0xFC) | 0x01;
  EIMSK |= 0x10;
}

void loop()
{
  // Nice clean business logic, using well-named functions,
  // implementation detail is hidden
  if (triggered ())
    Serial.println(read_position());  // respond
}

int32_t read_position()
{
  noInterrupts(); // read position in a critical section
  int32_t pos = position;
  interrupts();
  return pos ;
}

bool triggered ()
{
  if (Serial.available())
  {
    Serial.read() ;
    return true ;
  }
  else
    return false ;
}

ISR(INT0_vect) { encoderUpdate(); }
ISR(INT1_vect) { encoderUpdate(); }
ISR(INT4_vect) { encoderUpdate(); }

void encoderUpdate()
{
    uint8_t p1val = DIRECT_PIN_READ(pin1_register, pin1_bitmask);
    uint8_t p2val = DIRECT_PIN_READ(pin2_register, pin2_bitmask);
    uint8_t s = state & 3;
    if (p1val) s |= 4;
    if (p2val) s |= 8;
    state = (s >> 2);
    switch (s) {
      case 1: case 7: case 8: case 14: case 3: case 12:
        position++;
        return;
      case 2: case 4: case 11: case 13: case 6: case 9:
        position--;
        return;
    }
}
