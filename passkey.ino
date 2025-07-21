#include <EEPROM.h>
#include "Keyboard.h"

static const size_t KBufSize = 256;
static const int KEepROMaddr = 0x0;
static const int KBlinkError = 4;
byte length = 0;
bool pass = 0;

static char password[KBufSize] = {0};

void blink_error()
{
  for (unsigned i = 0; i < KBlinkError; ++i) {
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void setup() {
   // open the serial port
  Serial.begin(9600);
   // initialize control over the keyboard
  Keyboard.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  length = EEPROM.read(KEepROMaddr);

  if (!length) {
    Serial.write("No password programmed\n");
  } else {
    if (length >= KBufSize) {
      blink_error();
      length = KBufSize - 1;
    }

    for (int i = 0 ; i < length; ++i)
      password[i] = EEPROM.read(KEepROMaddr + 1 + i);
  }
}

void loop() {
  delay(1000);

  if (!pass && length) {

    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)

    Keyboard.write(password, strlen(password));

    Serial.write("passw has been sent\n");
    delay(100);
    digitalWrite(LED_BUILTIN, LOW); 

    pass = true;
  } else {
    char buf[KBufSize]={0};
    unsigned size = 0;

    while (Serial.available() > 0) {
      buf[size++] = Serial.read();

      if (size == KBufSize) {
        Serial.write("passw too long\n");
        blink_error();
        return;
      }
    }
    
    if (1 == size) {
      if (buf[0] == 'e')
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.write("Erasing eprom...");

        for (uint16_t i = 0 ; i < EEPROM.length(); i++)
          EEPROM.write(i, 0);
        
        digitalWrite(LED_BUILTIN, LOW);
        Serial.write("Done");
    } else if (size > 2 && buf[0] == 'p' && buf[1] == '=') {

      for (uint16_t i = 0; i < size; ++i)
        password[i] = buf[i+2];

        digitalWrite(LED_BUILTIN, HIGH);
        Serial.write("Programming...");
        Serial.write(buf+2, size - 2);

        EEPROM.write(0, size - 2);        
        for (int i = 0 ; i < size - 2 ; i++)
          EEPROM.write(i+1, buf[i+2]);
        
        digitalWrite(LED_BUILTIN, LOW);
        Serial.write("Done");
    }
  }
}
