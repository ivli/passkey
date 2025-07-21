#include <EEPROM.h>
#include "Keyboard.h"

static char password[256] = {0};

const int address = 0x0;
byte length = 0;
bool pass = 0;



void setup() {
  // open the serial port:
  Serial.begin(9600);
  // initialize control over the keyboard:
  Keyboard.begin();  
  pinMode(LED_BUILTIN, OUTPUT);
  length = EEPROM.read(address);

  if (!length) {
    Serial.write("No password programmed\n");
  } else {
    for (int i = 0 ; i < length; ++i)
      password[i] = EEPROM.read(address + 1 + i);
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
    enum command_t{nop_e, erase_e, programm_e} command = nop_e;

    char buf[1024]={0};
    unsigned size=0;

    while (Serial.available() > 0) {
      buf[size++] = Serial.read();
    }
    
    Serial.write(buf, size);
    
    if (1 == size) {
      if (buf[0] == 'e')
        command = erase_e;
    } else if (size > 2 && buf[0] == 'p' && buf[1] == '=') {
      for (unsigned i=0; i < size; ++i)
        password[i] = buf[i+2];

      command = programm_e;  
    }

    switch (command) {
      case erase_e: {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.write("Erasing eprom...");

        for (int i = 0 ; i < EEPROM.length(); i++)
          EEPROM.write(i, 0);
        
        digitalWrite(LED_BUILTIN, LOW);
        Serial.write("Done");
      }; break;
    case programm_e: 
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.write("Programming...");
        Serial.write(buf+2, size - 2);

        EEPROM.write(0, size - 2);        
        for (int i = 0 ; i < size - 2 ; i++)
          EEPROM.write(i+1, buf[i+2]);
        
        digitalWrite(LED_BUILTIN, LOW);
        Serial.write("Done");
    case nop_e: break;
    }

  }

}
