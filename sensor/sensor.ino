/*
 Sensor

 Purpose: start acquiring data on trigger and send it to a remote
 location.

 */

#include <SPI.h>
#include <Ethernet.h>

byte MAC[] = {
  0x90, 0xA2, 0xDA, 0x0F, 0x1C, 0x12};
const IPAddress IP(192, 168, 254, 3);
const int PORT = 23;
EthernetServer server(PORT);

#define TRIG 2
int DATA_PINS[] = {
  4,5,6,7};

unsigned int range = 0;


void setup()
{
  pinMode(TRIG, INPUT);
  for (int i=0;i<4;i++)
    pinMode(DATA_PINS[i], INPUT);

  Ethernet.begin(MAC, IP);
  server.begin();
}

void loop(){
}

byte getrange()
{
  byte ret = 0;
  for (int i=0;i<4;i++) {
    byte temp = digitalRead(DATA_PINS[i]);
    ret |= temp << i;
  }
  range = ret;
  return ret;
}

