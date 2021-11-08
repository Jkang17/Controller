#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
int led_pin = 3;

typedef struct
{
  uint32_t X_Axis_Positive = 0u;
  uint32_t X_Axis_Negative = 0u;
  uint32_t Y_Axis_Positive = 0u;
  uint32_t Y_Axis_Negative = 0u;
}Analog_t;

Analog_t analog;

void setup() 
{
	pinMode(led_pin, OUTPUT);
	Serial.begin(9600);
	radio.begin();
	radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
	radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
	radio.startListening();              //This sets the module as receiver
}


void loop()
{
	if (radio.available())              //Looking for the data.
	{
		char text[100] = "";                 //Saving the incoming data
		radio.read(&text, sizeof(text));    //Reading the data
		radio.read(&analog.X_Axis_Positive, sizeof(analog.X_Axis_Positive));    //Reading the data
    radio.read(&analog.X_Axis_Negative, sizeof(analog.X_Axis_Negative));
		Serial.println(analog.X_Axis_Positive);
		Serial.println(analog.X_Axis_Negative);
		if(analog.X_Axis_Positive < 512)
		{
			digitalWrite(led_pin, HIGH);
			Serial.println(text);
		}

		else
		{
			digitalWrite(led_pin, LOW);
			Serial.println(text);}
		}
	delay(1);
}
