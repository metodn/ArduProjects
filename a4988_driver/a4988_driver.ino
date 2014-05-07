#include <Arduino.h>
//#http://www.geeetech.com/wiki/index.php/StepStick_A4988_Stepper_Driver_Module
//http://miscsolutions.wordpress.com/2011/10/16/five-things-i-never-use-in-arduino-projects/
/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/

unsigned long timeOfLastEvent = 0;
unsigned long currentMillis = 0;
unsigned long timeOfLastEvent1 = 0;
unsigned long timeOfLastEvent2 = 0;

int stepperPin = 3;
int dirPin = 4;
int enaPin = 5;

int ledPin = 13;
bool mstate = LOW;  //motor state low or hih
int i;
int a;

void setup() {
 pinMode(stepperPin, OUTPUT);
 pinMode(dirPin, OUTPUT);
 pinMode(ledPin, OUTPUT);
 pinMode(enaPin, OUTPUT);
 digitalWrite(enaPin, LOW);  //Low for enable stepper
//   Serial.begin(9600);
  Serial.begin(115200);
}


 void step(boolean dir,int steps){
 digitalWrite(dirPin,dir);
 delay(50);
 for(int i=0;i<steps;i++){
   digitalWrite(stepperPin, HIGH);
   delayMicroseconds(800);
   digitalWrite(stepperPin, LOW);
   delayMicroseconds(800);
 }
}
//digitalWrite(pinnum, !digitalRead(pinnum));
void loop(){
    unsigned long currentMillis = millis(); //delay replacement
    if (currentMillis - timeOfLastEvent1 > 100){
//    if( i%10==0) {
        //Serial.println(i);
    //}

       digitalWrite(ledPin,HIGH);
       Serial.print(mstate);
       Serial.print("-tick\n\r ");
       mstate=!mstate;
       digitalWrite(stepperPin,mstate);
       timeOfLastEvent1 = currentMillis;  //update timer with last event
    }

     digitalWrite(ledPin,LOW);


}
