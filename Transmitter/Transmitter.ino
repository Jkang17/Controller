#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define SWING_PIN   0u
#define ACCELERATE_PIN 1u
#define REVERSE_PIN 2u
#define X_AXIS  A4
#define Y_AXIS  A5

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
  uint32_t X_Axis_Positive = 0u;
  uint32_t X_Axis_Negative = 0u;
}Controller_t;

Controller_t data;
ControllerState_t ControllerState = STANDBY;

void setup() 
{
  Serial.begin(9600);

  pinMode(SWING_PIN, INPUT);
  pinMode(ACCELERATE_PIN, INPUT);
  pinMode(REVERSE_PIN, INPUT);

  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}


void loop()
{
  data.X_Axis_Positive = analogRead(X_AXIS);
  data.X_Axis_Negative = analogRead(X_AXIS);
  data.swingDigitalRead = digitalRead(SWING_PIN);
  data.accelerateDigitalRead = digitalRead(ACCELERATE_PIN);
  data.reverseDigitalRead = digitalRead(REVERSE_PIN);
  
  switch(ControllerState)
  {
    case STANDBY:
      Serial.println("Standing by");
      if (data.accelerateDigitalRead == HIGH)
      {
        Serial.println("Changing State");
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
        Serial.println("Accelerating");
        Serial.println(data.X_Axis_Positive);
        Serial.println(data.X_Axis_Negative);
//         if(data.X_Axis_Negative < 450 ) //Note: Turns Left
//         {
//            data.swingDigitalRead = digitalRead(swingPin);
//            Serial.println(data.X_Axis_Positive);
//            Serial.println(data.X_Axis_Negative);
//         }
  
         // if(data.X_Axis_Negative > 512 ) //Note: Turns Right
         // {
         //   data.swingDigitalRead = digitalRead(swingPin);
         //   Serial.println(data.X_Axis_Positive);
         //   Serial.println(data.X_Axis_Negative);
         // }
  
         // if(data.X_Axis_Positive < 512 && data.X_Axis_Negative > 450)
         // {
         //   data.swingDigitalRead = digitalRead(swingPin);
         //   Serial.println(data.X_Axis_Positive);
         //   Serial.println(data.X_Axis_Negative);
         // } 
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
        Serial.println(data.X_Axis_Positive);
        Serial.println(data.X_Axis_Negative);
//         if(data.X_Axis_Negative < 450 ) //Note: Turns Left
//         {
//            data.swingDigitalRead = digitalRead(swingPin);
//            Serial.println(data.X_Axis_Positive);
//            Serial.println(data.X_Axis_Negative);
//         }
  
         // if(data.X_Axis_Negative > 512 ) //Note: Turns Right
         // {
         //   data.swingDigitalRead = digitalRead(swingPin);
         //   Serial.println(data.X_Axis_Positive);
         //   Serial.println(data.X_Axis_Negative);
         // }
  
         // if(data.X_Axis_Positive < 512 && data.X_Axis_Negative > 450)
         // {
         //   data.swingDigitalRead = digitalRead(swingPin);
         //   Serial.println(data.X_Axis_Positive);
         //   Serial.println(data.X_Axis_Negative);
         // } 
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
