#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define L_REAR_MOTOR1   (4u)
#define L_REAR_MOTOR2   (5u)
#define R_REAR_MOTOR1   (6u)
#define R_REAR_MOTOR2   (7u)

#define SWING_PIN       (0u)
#define SERVO_PIN				(1u)

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
					MotorOff();
					Swing();
				}

			break;

			case ACCELERATE:
				if (data.accelerateDigitalRead == HIGH)
				{
					MoveForward();
					if(data.X_Axis_Positive > 512)	/*Note: Turn Right*/
					{
				 			// turn servo right
						Swing();
            Serial.println("Right");
         	}
           
					else
					{
						Swing();
						Serial.println("Nothing");
					}
				}

				else
				{
					MotorOff();
					ControllerState = STANDBY;
				}
				
			break;

			case REVERSE:
			 	if (data.reverseDigitalRead == HIGH)
			 	{
			 		MoveBackward();
			 		if(data.X_Axis_Positive < 512)
			 		{
			 			Swing();
			 		}

				 	if(data.X_Axis_Negative < 450 ) //Note: Turns Right
   				{
   					/*Input the motor control here*/
   					Swing();
   				}

 					if(data.X_Axis_Positive < 512 && data.X_Axis_Negative > 450)
  				{
  					Swing();
					}

    			else
    			{
    				Serial.println(text);
    			}
				}

				else
				{
				  MotorOff();
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

void Swing()
{
	if (data.swingDigitalRead == HIGH)
	{
		digitalWrite(SWING_PIN, HIGH);
		digitalWrite(SWING_PIN, LOW);
  }	  

	else
	{
		digitalWrite(SWING_PIN, LOW);
	}
}
