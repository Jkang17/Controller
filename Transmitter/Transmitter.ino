#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*Note: Use these definitions to define what analog values should be on those inputs.
#define FORWARD ("Input some ADC value here for moving forward")
#define STOP ("Input some ADC value here for stopping")
#define REVERSE ("Input some ADC value here for reversing.")
#define TURNING ("Input some ADC value here for moving TURNING")
#define SWING   ("Input some ADC value here for swinging.")
*/

RF24 radio(9, 10); // CE, CSN         
const byte address[6] = "00001";     /*Byte of array representing the address. This is the address where we will send the data. 
                                    This should be same on the receiving    side.*/
uint8_t swingPin = 2u;
uint8_t acceleratePin = 3u;
uint8_t reversePin = 4u;
uint8_t brakePin = 5u;

typedef enum
{
  STANDBY = 0,
  ACCELERATE = 1,
  SWING = 3,
  REVERSE = 4,
  STOP = 5,
  TURNING = 6,
  SLEEP = 7
}ControllerState_t;

typedef struct
{
  uint8_t swingDigitalRead = 0u;
  uint32_t accelerateDigitalRead = 0u;
  uint32_t reverseDigitalRead = 0u;
  uint32_t brakeDigitalRead = 0u;
}Digital_t;


typedef struct
{
  uint32_t swingAnalogRead = 0u;
  uint32_t accelerateAnalogRead = 0u;
  uint32_t reverseAnalogRead = 0u;
  uint32_t brakeAnalogPin = 0u;
}Analog_t;

Digital_t digital;
Analog_t analog;
ControllerState_t ControllerState = ACCELERATE;

void setup() 
{
  Serial.begin(9600);
  pinMode(swingPin, INPUT);

  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}


void loop()
{

digital.swingDigitalRead = digitalRead(swingPin);
digital.accelerateDigitalRead = digitalRead(swingPin);
digital.reverseDigitalRead = digitalRead(swingPin);
digital.brakeDigitalRead = digitalRead(swingPin);

switch(ControllerState)
{
  case STANDBY:
    ControllerState = ACCELERATE;
    break;

  case ACCELERATE:      /*Note: */
    if(digital.accelerateDigitalRead == HIGH) // Note: Add ADC values for joystick control.
    {
      digital.accelerateDigitalRead = digitalRead(swingPin); 
      Serial.println(digital.accelerateDigitalRead);
      const char text[] = "Accelerating";
      Serial.println(text);
      radio.write(&text, sizeof(text));                  //Sending the message to receiver
    }

    else
    {
      digital.accelerateDigitalRead = digitalRead(swingPin);
      Serial.println(digital.accelerateDigitalRead);
      const char text[] = "Not Accelerating";
      Serial.println(text);
      radio.write(&text, sizeof(text));
    
    }
    ControllerState = ACCELERATE;
    break;

  // case SWING:
  //   if(digital.accelerateDigitalRead == HIGH)
  //   {
  //     digital.accelerateDigitalRead = digitalRead(swingPin); 
  //     Serial.println(digital.accelerateDigitalRead);
  //     const char text[] = "Swing";
  //     Serial.println(text);
  //     radio.write(&text, sizeof(text));                  //Sending the message to receiver
  //   }

  //   else
  //   {
  //     digital.accelerateDigitalRead = digitalRead(swingPin);
  //     if(digital.accelerateDigitalRead == LOW)
  //     {
  //       Serial.println(digital.accelerateDigitalRead);
  //       const char text[] = "No Swing";
  //       Serial.println(text);
  //       radio.write(&text, sizeof(text));
  //     } //Sending the message to receiver 
  //   }
  //   break;

  //   case REVERSE:
  //     if(digital.accelerateDigitalRead == HIGH)
  //     {
  //       digital.accelerateDigitalRead = digitalRead(swingPin); 
  //       Serial.println(digital.accelerateDigitalRead);
  //       const char text[] = "Reverse";
  //       Serial.println(text);
  //       radio.write(&text, sizeof(text));                  //Sending the message to receiver
  //     }

  //     else
  //     {
  //       digital.accelerateDigitalRead = digitalRead(swingPin);
  //       if(digital.accelerateDigitalRead == LOW)
  //       {
  //         Serial.println(digital.accelerateDigitalRead);
  //         const char text[] = "No Reverse";
  //         Serial.println(text);
  //         radio.write(&text, sizeof(text));
  //       } //Sending the message to receiver 
  //     }
      break;

    default:
    break;
  } 

  radio.write(&digital.accelerateDigitalRead, sizeof(digital.accelerateDigitalRead));  //Sending the message to receiver 
  delay(1);
}
