// Module KY023 
int Xin= A0; 
// X Input Pin 
int Yin = A1; 
// Y Input Pin 
int KEYin = 3; 
// Push Button  
int buttonVal = 0;
void setup () 
{   
  pinMode (KEYin, INPUT);   
  Serial.begin (9600);  
} 

void loop () 
{   
  uint16_t xVal, yVal, buttonVal, counts = 0u;  

  xVal = analogRead (Xin);   
  yVal = analogRead (Yin);   
  buttonVal = digitalRead (KEYin);      
  Serial.print("X = ");   
  Serial.println (xVal);   
  Serial.print ("Y = ");   
  Serial.println (yVal);      
  Serial.print("Button is ");
  Serial.println(buttonVal);   

  
  if (buttonVal == LOW)
    {     
      Serial.println ("not pressed");   
    }   

  else
    { 
      Serial.println ("PRESSED");   
    }        


  delay (500); 
} 
