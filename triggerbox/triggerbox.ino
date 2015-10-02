/*
 Triggerbox

 Purpose:
 read a time interval from ethernet and send a trigger signal on
 a port at fixed time steps.

 */

#include <SPI.h>
#include <Ethernet.h>

const int TRIG = 2;
byte MAC[] = {0x90, 0xA2, 0xDA, 0x0F, 0x1C, 0x12};
const IPAddress IP(192, 168, 254, 3);
const int PORT = 23;
EthernetServer server(PORT);

unsigned long time;
unsigned long last_time;
unsigned int period;

void trigger();
void check_time();

static EthernetClient zero = EthernetClient();
void report(EthernetClient &client = zero);

void setup()
{
	pinMode(TRIG, OUTPUT); /* Set trigger pin to output     */
	digitalWrite(TRIG, LOW);
	Serial.begin(9600);		 /* Activate serial port          */
	Ethernet.begin(MAC, IP); /* Create ethernet socket server */
	server.begin();
	period = 1 * 1000; /* 1000 ms */
	report();
	/* Start the clock */
	time = millis();
	last_time = time;
}

void loop()
{
	EthernetClient client = server.available();
	if (client) {
		char cmd = client.read();
		if (cmd == 'P') {
			String period_str = NULL;
			period_str.reserve(10);
			while (client.available() > 0)
				period_str += client.read();
			period = period_str.toInt();
		}
		report(client);
	}
	check_time();
}

void report(EthernetClient &client)
{
	if (client) {
		/* F() stores string in flash memory */
		client.print(F("Triggering with period: "));
		client.print(period);
		client.println(F(" ms."));
		client.stop();
	} else {
		Serial.print(F("Triggering with period: "));
		Serial.print(period);
		Serial.println(F(" ms."));
	}
}

#define PULSE 500 /* microseconds */
void trigger()
{
	/* Send a PULSE ms trigger signal on pin TRIG */
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(PULSE);
	digitalWrite(TRIG, LOW);
	Serial.println(F("TRIGGERED"));
}

void check_time()
{
	/* Check if it is time for the trigger, and update clock */
	time = millis();
	if ((time - last_time) >= period) {
		last_time = time;
		trigger();
	}
}
