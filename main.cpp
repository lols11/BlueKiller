//BlueKiller -  2ch NodeMCU Bluetooth Jammer 
//Based on  https://github.com/cropinghigh/morgenstop
//FOR EDUCATIONAL PURPOSES ONLY!!
#include <Arduino.h>
#include "RF24.h"


RF24 radio(D1, D8, 16000000);
RF24 radio2(D4, D2, 16000000);

#define LED D0 // Led in NodeMCU at pin GPIO16

uint8_t ch = 0; // Start with channel 0

void setup()
{
  pinMode(LED, OUTPUT);
  //Serial.begin(115200);

  for (int i = 0; i <= 2; i++) //Wakeup info via LED
  {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
  }

  // delay(10000); Delay time to connect to serial port for debug

  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MAX, false);
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_DISABLED);
  //radio.printDetails();

  radio2.begin();
  radio2.setAutoAck(false);
  radio2.setPALevel(RF24_PA_MAX, false);
  radio2.setDataRate(RF24_2MBPS);
  radio2.setCRCLength(RF24_CRC_DISABLED);
  //radio2.printDetails();

  radio.startConstCarrier(RF24_PA_MAX, ch);
  radio2.startConstCarrier(RF24_PA_MAX, ch + 40);
  
  for (int i = 0; i <= 4; i++) //Ready info via LED
  {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
  }
}

void loop()
{
  uint32_t randVal = random(); // Randomize time and frequency shift to get better spectrum spread
  uint32_t del = 10;
  switch (randVal & 0b11100)
  {
  case 0b00000:
    del = 3;
    break;
  case 0b00100:
    del = 4;
    break;
  case 0b01000:
    del = 5;
    break;
  case 0b01100:
    del = 6;
    break;
  case 0b10000:
    del = 6;
    break;
  case 0b10100:
    del = 9;
    break;
  case 0b11000:
    del = 1;
    break;
  case 0b11100:
    del = 11;
    break;
  }
  delayMicroseconds(del);

  // Set radio to channels between 0 and 39
  radio.setChannel(ch);
  // Set radio2 to channels between 40 and 80
  radio2.setChannel((ch + 40) % 41 + 40);

  ch += (randVal & 0b11);
  if (ch > 39)
  {
    ch = 0;
  }
}
