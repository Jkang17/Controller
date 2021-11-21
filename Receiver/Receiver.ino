#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SWING_PIN       (0u)
#define ACCELERATE_PIN  (1u)
#define REVERSE_PIN     (2u)

#define L_REAR_MOTOR1   (3u)
#define L_REAR_MOTOR2   (4u)
#define R_REAR_MOTOR1   (5u)
#define R_REAR_MOTOR2   (6u)

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0u;
char text[] = "";                 //Saving the incoming data

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
	pinMode(L_REAR_MOTOR1, OUTPUT);
	pinMode(L_REAR_MOTOR2, OUTPUT);
	pinMode(R_REAR_MOTOR1, OUTPUT);
	pinMode(R_REAR_MOTOR2, OUTPUT);
	pinMode(SWING_PIN, OUTPUT);
	pinMode(ACCELERATE_PIN, OUTPUT);
	pinMode(REVERSE_PIN, OUTPUT);
	
	radio.begin();
	radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
	radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
	radio.startListening();              //This sets the module as receiver
}

void loop()
{
	if (radio.available())              //Looking for the data.
	{
    radio.read(&data, sizeof(Controller_t));    //Reading the data
    Serial.println(data.X_Axis_Positive);      // Leave for debugging purposes
    //Serial.println(data.X_Axis_Negative);
    //Serial.println(data.accelerateDigitalRead);

		switch(ControllerState)
		{
			case STANDBY:

				if (data.accelerateDigitalRead == HIGH)
				{
				  ControllerState = ACCELERATE;
				}

				else if (data.reverseDigitalRead == HIGH)
				{
					ControllerState = REVERSE;
				}
					     					
				else
				{
					digitalWrite(SWING_PIN, LOW);
				}

			break;

			case ACCELERATE:

				if (data.accelerateDigitalRead == HIGH)
				{
					
					if(data.X_Axis_Positive > 512)	/*Note: Turn Right*/
					{
				 			// turn servo right
            Serial.println("Right");
            digitalWrite(L_REAR_MOTOR1, HIGH);
            digitalWrite(L_REAR_MOTOR2, LOW);
            digitalWrite(R_REAR_MOTOR1, HIGH);
            digitalWrite(R_REAR_MOTOR2, LOW);
         	}
           
					else
					{

						Serial.println("Nothing");
					}
				}

				else
				{
					digitalWrite(L_REAR_MOTOR1, LOW);
					digitalWrite(L_REAR_MOTOR2, LOW);
					digitalWrite(R_REAR_MOTOR1, LOW);
					digitalWrite(R_REAR_MOTOR2, LOW);

					ControllerState = STANDBY;
				}
				
			break;

			case REVERSE:
			 	if (data.reverseDigitalRead == HIGH)
			 	{
			 		if(data.X_Axis_Positive < 512)
			 		{
			 			if (data.swingDigitalRead == HIGH)
  					{
  						digitalWrite(SWING_PIN, HIGH);
	 			    }	     
  					else
  					{
  						digitalWrite(SWING_PIN, LOW);
  					}
			 		}

				 	if(data.X_Axis_Negative < 450 ) //Note: Turns Right
   				{
   					/*Input the motor control here*/
   					if (data.swingDigitalRead == HIGH)
   					{
   						digitalWrite(SWING_PIN, HIGH);
						}	

   					else
   					{
   						digitalWrite(SWING_PIN, LOW);
   					}
   				}

 					if(data.X_Axis_Positive < 512 && data.X_Axis_Negative > 450)
  				{
						if (data.swingDigitalRead == HIGH)
						{
							digitalWrite(SWING_PIN, HIGH);
						}	     

						else
						{
							digitalWrite(SWING_PIN, LOW);
						}

					}

    			else
    			{
    				Serial.println(text);
    			}
				}

				else
				{
					ControllerState = STANDBY;
				}

			break;			

			default:
			break;
		}
	}

	delay(1);

}


void MoveForward()
{
	digitalWrite(L_REAR_MOTOR1, HIGH);
  digitalWrite(L_REAR_MOTOR2, LOW);
  digitalWrite(R_REAR_MOTOR1, HIGH);
  digitalWrite(R_REAR_MOTOR2, LOW);
}

void MoveBackward()
{
	digitalWrite(L_REAR_MOTOR1, LOW);
  digitalWrite(L_REAR_MOTOR2, HIGH);
  digitalWrite(R_REAR_MOTOR1, LOW);
  digitalWrite(R_REAR_MOTOR2, HIGH);
}

void MotorOff()
{
	digitalWrite(L_REAR_MOTOR1, LOW);
	digitalWrite(L_REAR_MOTOR2, LOW);
	digitalWrite(R_REAR_MOTOR1, LOW);
	digitalWrite(R_REAR_MOTOR2, LOW);
}