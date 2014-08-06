/*
 Sensor

 Purpose: start acquiring data on trigger and send it to a remote
 location.

 */

#include <SPI.h>
#include <Ethernet.h>

/* Ethernet settings */
byte MAC[] = {
  0x90, 0xA2, 0xDA, 0x0F, 0x1C, 0x12};
const IPAddress IP(192, 168, 254, 3);
const int PORT = 23;
EthernetServer server(PORT);

/* Pins for range, measurement and trigger signal */
#define TRIG 0 /* interrupt 0 = pin 2 */
#define TRIGP 2
#define SIGNAL A5 /* analog pin 5 = pin  14 */
int DATA_PINS[] = {
  4,5,6,7};
unsigned int range = 0;
volatile int trigd = 0;

byte getrange();
float getsignal();
void trigger();

void setup()
{
  pinMode(TRIGP, INPUT);
  digitalWrite(TRIGP, LOW);  /* Pull the pin down - testing */
  /* Set signal and range pins to input */
  pinMode(SIGNAL, INPUT);
  for (int i=0;i<4;i++)
    pinMode(DATA_PINS[i], INPUT);

  Serial.begin(9600);
  Ethernet.begin(MAC, IP);
  server.begin();
  attachInterrupt(TRIG, trigger, RISING); /* interrupt on rising edge */
}

void loop()
{
  if (trigd){
    /* Reset trigger signal and report a trigger to serial */
    trigd = 0;
    Serial.println(-1);
  }
  getrange();
  Serial.println(getsignal());
  delay(100);
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

float getsignal()
{
  int signal = analogRead(SIGNAL);
  return signal / 1023.0 * range;
}

void trigger()
{
  trigd = 1;
}

