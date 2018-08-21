#include <Sim800l.h> // https://github.com/cristiansteib/Sim800l
#include <SoftwareSerial.h>

#define CW  0
#define CCW 1

#define MOTOR_A 0
#define MOTOR_B 1

Sim800l Sim800l;  //to declare the library
String text;     // to storage the text of the sms
uint8_t index = 1;   // to indicate the message to read.
char timp;
char stare;
String s_timp;
uint8_t i;


const byte PWMA = 3;  // PWM control (speed) for motor A
const byte PWMB = 5; // PWM control (speed) for motor B
const byte DIRA = 12; // Direction control for motor A
const byte DIRB = 9; // Direction control for motor B


void setup(){
  
  Serial.begin(9600); // only for debug the results .
  pinMode(LED_PIN, OUTPUT); 
  Sim800l.begin(); // initializate the library. 
  Sim800l.delAllSms();   
  Serial.println(text);
  setupArdumoto();

}

void loop(){ 

   digitalWrite(LED_PIN, LOW);
  
  text=Sim800l.readSms(index);
  
  if (text.indexOf("OK")!=-1) {
    
      text.toUpperCase();
      Serial.println(text);

      if(text.indexOf("F") != -1) {

          digitalWrite(LED_PIN, HIGH);
          
          index = text.indexOf("F");
          stare = text[index];
          timp = text[index+1];
          Serial.print(stare);
          Serial.println(timp);
          
          s_timp = timp;
          s_timp = s_timp  + "000";

          driveArdumoto(MOTOR_A, CW, 127);  // Motor A at max speed.
          driveArdumoto(MOTOR_B, CW, 127);  // Motor B at max speed.
          
          //Serial.println(s_timp);
          delay(s_timp.toInt());
          Serial.println("done_delay");

          stopArdumoto(MOTOR_A);
          stopArdumoto(MOTOR_B);

          digitalWrite(LED_PIN, LOW);
      }

      else if(text.indexOf("S") != -1) {

          index = text.indexOf("S");
          stare = text[index];
          timp = text[index+1];
          Serial.print(stare);
          Serial.print(timp);

          s_timp = timp;

          driveArdumoto(MOTOR_A, CCW, 127);  // Motor A at max speed.
          driveArdumoto(MOTOR_B, CCW, 127);  // Motor B at max speed.
         
          for(i = 0; i<= s_timp.toInt(); i++) 
          {
            digitalWrite(LED_PIN, HIGH);
            delay(500);
            digitalWrite(LED_PIN, LOW);
            delay(500);
            
          }

          stopArdumoto(MOTOR_A);
          stopArdumoto(MOTOR_B);

          digitalWrite(LED_PIN, LOW);
          
      }

            else{
             Serial.println("Not Compatible ...sorry.. :D");
           }
		   
      Sim800l.delAllSms();
      index = 1;
      
    } 
}


void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}