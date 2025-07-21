A simplistic string manager. The whole idea is snatched from <a href=https://en.wikipedia.org/wiki/BadUSB> badUSB </a>. 
I use 5$ cheap clone of "Leonardo" but I guess any ATMEGA32U4 based Arduino should work.    
Upon insertion into USB socket the device pretends to be a keyboard and sends a sequence of symbols stored in EEPROM.
Actually it not tecessarily have to be a password or command it could be any string instead.
Reprogramming is done via Serial connectivity: 
  sending just 'e' would erase entire EEPROM; 
  sending 'p=string' would store all symbols after 'p=' in EEPROM. String lenght is limited to 256 symbols but upon necessity it can be easily stretched depenfing on EERROM size. 

<img width="640" alt="image" src="https://github.com/user-attachments/assets/2f80a15a-c5e9-4fce-881d-9e1507aedbaf" />

That is.
