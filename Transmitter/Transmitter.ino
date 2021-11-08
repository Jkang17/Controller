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
#define X_AXIS  A4
#define Y_AXIS  A5

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
  uint32_t X_Axis_Positive = 0u;
  uint32_t X_Axis_Negative = 0u;
  uint32_t Y_Axis_Positive = 0u;
  uint32_t Y_Axis_Negative = 0u;
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

  case ACCELERATE:      /*Note: Adjust below commented code to accomodate for X axis*/
  
    analog.X_Axis_Positive = analogRead(X_AXIS);
    analog.X_Axis_Negative = analogRead(X_AXIS);
    // Serial.println(analog.X_Axis_Positive);
    // Serial.println(analog.X_Axis_Negative);
    
    if(analog.X_Axis_Negative < 450 ) //Note: Turns Left
    {
      // digital.accelerateDigitalRead = digitalRead(swingPin); 
      // Serial.println(digital.accelerateDigitalRead);
      Serial.println(analog.X_Axis_Positive);
      Serial.println(analog.X_Axis_Negative);
      const char text[] = "Turn Left";
      Serial.println(text);
      radio.write(&text, sizeof(text));                  //Sending the message to receiver
    }

    if(analog.X_Axis_Negative > 512 ) //Note: Turns Right
    {
      // digital.accelerateDigitalRead = digitalRead(swingPin); 
      // Serial.println(digital.accelerateDigitalRead);
      /*Note: if(button_pressed)
              {
                Proceed to accelerate while turning.
                digital.accelerateDigitalRead = digitalRead(swingPin); 
                Serial.println(digital.accelerateDigitalRead);
              }
      */
      Serial.println(analog.X_Axis_Positive);
      Serial.println(analog.X_Axis_Negative);
      const char text[] = "Turn Right";
      Serial.println(text);
      radio.write(&text, sizeof(text));                  //Sending the message to receiver
    }

    if(analog.X_Axis_Positive < 512 && analog.X_Axis_Negative > 450)
    {
      // digital.accelerateDigitalRead = digitalRead(swingPin);
      // Serial.println(digital.accelerateDigitalRead);
      Serial.println(analog.X_Axis_Positive);
      Serial.println(analog.X_Axis_Negative);
      const char text[] = "Standing";
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

  radio.write(&analog.X_Axis_Positive , sizeof(analog.X_Axis_Positive ));  //Sending the message to receiver 
  radio.write(&analog.X_Axis_Negative , sizeof(analog.X_Axis_Negative )); 
  delay(1);
}
