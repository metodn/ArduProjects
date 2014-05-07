// for the desperates..
//#include <JeeLib.h>
#include "pitches.h"
#include <avr/sleep.h>

int LEDs[] = {1, 1, 1, 1,         1, 0, 1, 0, 1, 0, 1, 0, 1, 0};         
//-------------------------------------------------- melodija 
int melody1[] = {
     NB5,0,NB5,0,NB5,NA5,NB5,NB5,NB5,NB5,NB5,NB5,NC6,NB5,NA5,
     NB5,0,NB5,0,NB5,NA5,NFS5,NFS5,NFS5,NG5,NG5,NG5,NGS5,NGS5,NGS5,NA5,NAS5,
     NB5,0,NB5,0,NB5,NA5,NB5,NB5,NB5,NB5,NB5,NB5,NC6,NB5,NA5,
     NB5,0,NB5,0,NB5,NA5,NFS5,NFS5,NFS5,NG5,NG5,NG5,NGS5,NGS5,NGS5,NA5,NAS5
               };


int noteDurations1[] = {
                       8,4,8,4,8,8,8,16,16,8,8,8,8,8,8, // 15
                       8,4,8,4,8,8,8,16,16,8,16,16,8,16,16,8,8, // 17
                       8,4,8,4,8,8,8,16,16,8,8,8,8,8,8, // 15
                       8,4,8,4,8,8,8,16,16,8,16,16,8,16,16,8,8 // 17
                      };

//----------------------------------------------------------jingle bells
int melodyb[] = {
                NG4,NE5,ND5,NC5,NG4,NG4,NE5,ND5,NC5,NA4,
                NA4,NF5,NE5,ND5,NB4,NG5,NG5,NF5,ND5,NE5,
                NG4,NE5,ND5,NC5,NG4,NG4,NE5,ND5,NC5,NA4,
                NA4,NF5,NE5,ND5,NG5,NG5,NG5,NG5,NG5,NA5,NG5,NF5,ND5,NC5,NG5,
                NE5,NE5,NE5,NE5,NE5,NE5,NE5,NG5,NC5,ND5,NE5,
                NF5,NF5,NF5,NF5,NF5,NF5,NE5,NE5,NE5,NE5,NE5,ND5,ND5,NE5,ND5,NG5,
                NE5,NE5,NE5,NE5,NE5,NE5,NE5,NG5,NC5,ND5,NE5,
                NF5,NF5,NF5,NF5,NF5,NF5,NE5,NE5,NE5,NE5,NG5,NG5,NF5,ND5,NC5,
               };

  int melody[] = {
                NG4,NE5,ND5,NC5,NG4,NG4,NE5,
               };
               

int noteDurations[] = {
                       8,8,8,8,2,8,8,8,8,2, // 10
                       8,8,8,8,2,8,8,8,8,2, // 10
                       8,8,8,8,2,8,8,8,8,2, // 10
                       8,8,8,8,8,8,8,16,16,8,8,8,8,4,4, // 15
                       8,8,4,8,8,4,8,8,8,8,2, // 11
                       8,8,8,16,16,8,8,8,16,16,8,8,8,8,4,4, // 16
                       8,8,4,8,8,4,8,8,8,8,2, // 11
                       8,8,8,16,16,8,8,8,16,16,8,8,8,8,2, // 15
                       };
//-----------------------------------                       
int melody3[] ={
NE5,NF5,NG5,ND6,0,NC6,NC6,NB5,NG5,NF5,0,NF5,NF5,NE5,NC5,NF5,NE5,ND5,NC5,NE5,ND5,0,NE5,NF5,NG5,ND6,0,NC6,NC6,NB5,NG5,NF5,0,NF5,NF5,NE5,NC5,NF5,NE5,ND5,NC5,NE5,ND5,0,
0,NC5,NE5,NG5,NG5,NE5,ND5,NE5,ND5,0,NC5,NE5,NG5,NG5,NE5,ND5,0,NE5,0,NC5,NE5,NG5,NG5,NE5,ND5,NE5,ND5,0,NC5,NE5,NG5,NG5,NA5,NE5,0,NE5,
0,NC5,NE5,NG5,NG5,NE5,ND5,NE5,ND5,0,NC5,NE5,NG5,NG5,NE5,ND5,0,NE5,0,NC5,NE5,NG5,NG5,NE5,ND5,NE5,ND5,NF5,0,NE5,0,NC5,NF5,0,NE5,0,NC5,
NE5,NE5,NE5,0,NE5,0,ND5,NE5,NE5,NE5,0,NE5,0,ND5,NE5,NE5,NE5,0,NE5,0,ND5,ND5,NB4,NB4,0,NC5,0,NG5,
NE5,NE5,NE5,0,NE5,0,ND5,NE5,NE5,NE5,0,NE5,0,ND5,NE5,NE5,NE5,0,NE5,0,ND5,ND5,
NE5,NF5,NG5,ND6,0,NC6,NC6,NB5,NG5,NF5,0,NF5,NF5,NE5,NC5,NF5,NE5,ND5,NC5,NE5,ND5,0,NE5,NF5,NG5,ND6,0,NC6,NC6,NB5,NG5,NF5,0,NF5,NF5,NE5,NC5,NF5,NE5,ND5,NC5,NE5,ND5,0,
NE5,NF5,NE5,ND5,ND5,NC5,NC5,NC5,ND5,ND5,NE5,0,NE5,NF5,NE5,ND5,ND5,NC5,NC5,NC5,NG5,ND5,0,
NE5,NF5,NE5,ND5,ND5,NC5
              };

int noteDurations3[] = {
               8,8,8,4,8,4,8,8,8,4,8,4,8,8,8,4,4,8,8,8,2,8,8,8,8,4,8,4,8,8,8,4,8,4,8,8,8,4,4,8,8,8,2,8,
               8,8,8,8,8,16,8,16,8,8,8,8,8,8,16,8,16,8,8,8,8,8,8,16,8,16,8,8,8,8,8,8,16,8,16,8,
               8,8,8,8,8,16,8,16,8,8,8,8,8,8,16,8,16,8,8,8,8,8,8,16,8,16,8,8,16,8,16,8,8,16,8,16,8,
               4,4,8,16,8,16,8,4,4,8,16,8,16,8,4,4,8,16,8,16,8,4,4,8,16,8,16,8,
               4,4,8,16,8,16,8,4,4,8,16,8,16,8,4,4,8,16,8,16,8,2,
               8,8,8,4,8,4,8,8,8,4,8,4,8,8,8,4,4,8,8,8,2,8,8,8,8,4,8,4,8,8,8,4,8,4,8,8,8,4,4,8,8,8,2,8,
               8,8,8,8,4,4,8,8,8,4,2,8,8,8,8,8,4,4,8,8,8,2,8, 
               8,8,8,8,4,1
               };
//-------------------------------
int ledPin = 4;
int speakerPin = 3;
int switchPin = 2;
 
volatile byte PlayCount = 0;
int after=0;
//*
//* SETUP
//*
void setup() {
  Serial.println("setup "); 
  pinMode (ledPin, OUTPUT); 
  pinMode (speakerPin, OUTPUT);
  pinMode (switchPin, INPUT);
  
 // attachInterrupt(0, sw0, RISING );
  
  Serial.begin(9600);
  
  attachInterrupt(0, wakeUp, LOW);
}


void wakeUp(){    //isr za tipko  DONTUSER SERIAL>PRINT
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 20ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 20) 
  {
     PlayCount++;
  }
  last_interrupt_time = interrupt_time;
  
} 

void sleepNow()         
{
    Serial.println("sleep now "); 
    Serial.print("PLAY before sleep: ");  
    Serial.println(PlayCount); 
    Serial.flush();
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();          // enables the sleep bit in the mcucr register
 //   attachInterrupt(0,wakeUp, LOW); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW 
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP


    //******************* resume after sleep //                             


 //   sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
                                                       
 //   detachInterrupt(0);      // disables interrupt 0 on pin 2 so the 
                             // wakeUpNow code will not be executed 
                             // during normal running time.
    Serial.flush();                             
        Serial.println(); 
    Serial.print("PLAY on wake up: ");  
    Serial.println(PlayCount); 
}



inline void ring(){
   Serial.println("ring "); 
   while( PlayCount >=1){
    digitalWrite(ledPin, HIGH);
 
    int mSize=sizeof(melody)/2; // size of array in number of bytes
    Serial.println(PlayCount); 
    Serial.println("Play Music"); 
   
    for (int thisNote = 0; thisNote < mSize;  thisNote++) {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(speakerPin, melody[thisNote],noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;      
      delay(pauseBetweenNotes);      
    }
    Serial.println(" Music end.");  
    after=1;   //set after ring indicator switch    
    PlayCount--;
   
  }//endif
  if (after == 1){
   // afterRing();
    after=0;
   
   }
  digitalWrite(ledPin, LOW);

}

inline void afterRing(){
  Serial.println("after ring");
  Serial.print("PLAY: ");  
  Serial.println(PlayCount);  
  for (int j =0 ; j <3 ; j++){
    //if (PlayCount>=1) break; 
    digitalWrite(ledPin, HIGH);
    tone(speakerPin, 500+(j*50),100);
    delay(500-(j*20));
    digitalWrite(ledPin, LOW);
    delay(1300);
    

    }
   tone(speakerPin, 2500,5000);

}
//*
//* LOOP
//*

void loop() {
  Serial.println("loop "); 
  Serial.print("PLAY loop: ");  
  Serial.println(PlayCount); 
  ring(); 
  sleepNow();

}
