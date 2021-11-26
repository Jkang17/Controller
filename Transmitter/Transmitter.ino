#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SWING_PIN   (6u)
#define ACCELERATE_PIN (8u)
#define REVERSE_PIN (7u)
#define X_AXIS  A1
#define Y_AXIS  A0

RF24 radio(9, 10); // CE, CSN         
const byte address[6] = "00001";     /*Byte of array representing the address. This is the address where we will send the data. 
                                    This should be same on the receiving    side.*/
typedef enum
{
  STANDBY = 0,
  ACCELERATE = 1,
  SWING = 2,
  REVERSE = 3,
}ControllerState_t;

typedef struct
{
  bool swingDigitalRead = 0u;
  bool accelerateDigitalRead = 0u;
  bool reverseDigitalRead = 0u;
  uint32_t xAxis = 0u;
}Controller_t;

Controller_t data;
ControllerState_t ControllerState = STANDBY;

void setup() 
{
  Serial.begin(9600);

  pinMode(SWING_PIN, INPUT);
  pinMode(4, OUTPUT);
  pinMode(ACCELERATE_PIN, INPUT);
  pinMode(REVERSE_PIN, INPUT);

  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}


void loop()
{
  digitalWrite(4, HIGH);
  data.xAxis = analogRead(X_AXIS);
  data.swingDigitalRead = digitalRead(SWING_PIN);
  data.accelerateDigitalRead = digitalRead(ACCELERATE_PIN);
  data.reverseDigitalRead = digitalRead(REVERSE_PIN);
  Serial.println(data.accelerateDigitalRead);
  switch(ControllerState)
  {
    case STANDBY:
      Serial.println(data.xAxis);
      if (data.accelerateDigitalRead == HIGH)   //Test D4 to output a logic signal for D8 to read.
      {
//        Serial.println("Changing State");
        ControllerState = ACCELERATE;
      }
      
       else if (data.reverseDigitalRead == HIGH)
       {
         ControllerState = REVERSE;  
       }
     
       else 
      {
        data.accelerateDigitalRead = 0u;
        data.reverseDigitalRead = 0u;
        ControllerState = STANDBY;
      }
      break;
  
    case ACCELERATE:      /*Note: Adjust below commented code to accomodate for X axis*/
      if (data.accelerateDigitalRead == HIGH)
      {
//        Serial.println("Accelerating");
        Serial.println(data.xAxis);
        ControllerState = ACCELERATE;
      }
  
      else
      {
        data.accelerateDigitalRead = 0u;
        data.reverseDigitalRead = 0u;
        ControllerState = STANDBY;
      }
      break;

      case REVERSE:      /*Note: Adjust below commented code to accomodate for X axis*/
      if (data.reverseDigitalRead == HIGH)
      {
        Serial.println("Reversing");
        Serial.println(data.xAxis); 
        ControllerState = REVERSE;
      }
  
      else
      {
        data.accelerateDigitalRead = 0u;
        data.reverseDigitalRead = 0u;
        ControllerState = STANDBY;
      }
      break;
      
      default:
      break;
    } 
  radio.write(&data, sizeof(Controller_t));  //Sending the message to receiver 
  delay(1);
}
