int motor1pin1 = 0;
int motor1pin2 = 1;

int motor2pin1 = 2;
int motor2pin2 = 3;

void setup() 
{
  // put your setup code here, to run once:
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

  //(Optional)
//  pinMode(9, OUTPUT); 
//  pinMode(10, OUTPUT);
  //(Optional)
}

void loop() 
{
  // put your main code here, to run repeatedly:

  //Controlling speed (0 = off and 255 = max speed):     
  //(Optional)
  analogWrite(9, 100); //ENA pin
  analogWrite(10, 1023); //ENB pin
  //(Optional)
  
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  delay(3000);

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(3000);
}