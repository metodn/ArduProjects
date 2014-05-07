//http://www.instructables.com/id/Intermediate-Arduino-Inputs-and-Outputs/
//http://playground.arduino.cc/Code/Timer
#include <Arduino.h>
#include <PID_AutoTune_v0.h> //http://www.homebrewtalk.com/f235/pid-settings-arduino-based-hlt-328239/index2.html

#include <PID_v1.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <Time.h>

//==============PID+++++++++++++http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/
/*
*Tvoda med [80-99], ce preseze vrednost iklopi rele [VAROvalka]
*Tjedra med [26-92] npr 46 trajanja 6ur..  [PID]
*Lvoda=25-30l
*Ljedra =120 litov
* 4.5-5 kw grelc
*/
//delay replacment
unsigned long timeOfLastEvent = 0;
unsigned long timeOfLastEvent1 = 0;
//keyboard analog
unsigned int adc_key_in = 0;
#define btnNONE   0
#define btnUP     1
#define btnLEFT   2
#define btnSELECT 3
#define btnRIGHT  4
#define btnDOWN   5
#define btnESC    6
unsigned int ir = 0;

#define PotPin A1
#define KeyPin A0

#define RelayPin 3
//Define Variables we'll be connecting to
double Setpoint, Input, Output;

double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

//Specify the links and initial tuning parameters
//PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);               //DIRECT (like a car) or REVERSE (like a refrigerator)

int WindowSize = 30000;
unsigned long windowStartTime;
unsigned long t;

//==============ONEWIRE
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


//======LCD    l(rs, enable, d4, d5, d6, d7)
 LiquidCrystal lcd(7, 8, 9, 10, 5, 6);

void setup(void)
{
  // start serial port
//  Serial.begin(9600);
//  Serial.println("Dallas Temperature IC Control Library Demo");
    setTime(0,0,0,1,1,14); // set time to noon Jan 1 2011
  // Start up the library
  sensors.begin();


    /* LiquidCrystal display with: http://arduino.cc/en/Tutorial/LiquidCrystal
  LCD 4 (RS) to arduino pin 12
  LCD 5 (R/W) to ground (pin 14)
  LCD 7 (E) to arduino pin 11
  LCD DB4, DB5, DB6, DB7 on arduino pins 7, 8, 9, 10 */
//  LiquidCrystal(rs, rw, enable, d4, d5, d6, d7)
//LiquidCrystal(rs, enable, d4, d5, d6, d7)


    lcd.begin(20,4);

//  LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//  lcd.begin(16,2);


    lcd.clear();
    lcd.setCursor(1,3); // Position cursor on line x=1,y=3
    lcd.print("stating.."); // Print a message to the LCD

  pinMode(RelayPin, OUTPUT);

  windowStartTime = millis();
  //initialize the variables we're linked to

 Setpoint = 300.0;
  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}
int read_ADC_buttons(){
 adc_key_in = analogRead(0);      // read the value from the sensor
  //lcd.setCursor(16,0);
  //lcd.print(adc_key_in);
   if (adc_key_in > 1000) return btnNONE;
   if (adc_key_in < 40)  return btnNONE;

   if (adc_key_in < 67)  return btnESC;
   if (adc_key_in < 140)  return btnUP;
   if (adc_key_in < 250)  return btnRIGHT;
   if (adc_key_in < 295)  return btnSELECT;
   if (adc_key_in < 390)  return btnDOWN;
   if (adc_key_in < 540)  return btnLEFT;

 return btnNONE;  // when all others fail, return this...
}

void loop(void)
{
    unsigned long currentMillis = millis(); //delay replacement
    t++;
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
//  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
//  Serial.println("DONE");

  lcd.setCursor(0, 0);
  lcd.print("0:");
  double DS0=sensors.getTempCByIndex(0);
  lcd.print(DS0); // Why "byIndex"?

  lcd.setCursor(0, 1);
  lcd.print("1:");
  double DS1=sensors.getTempCByIndex(1);
  lcd.print(DS1); // Why "byIndex"?

  lcd.setCursor(0, 2);
  lcd.print("2:");
  double DS2=sensors.getTempCByIndex(2);
  lcd.print(DS2); // Why "byIndex"?
    // You can have more than one IC on the same bus.
    // 0 refers to the first IC on the wire

 //read keyboard
 if (currentMillis - timeOfLastEvent > 50){
    ir = read_ADC_buttons();
    lcd.setCursor(19,0);
    lcd.print(ir);

    int PotVal = analogRead(PotPin);
    Setpoint = map(  PotVal , 0,1023, 20, 99) * 10; //krat 10 zaradi osciliranje da se ga znebim
    timeOfLastEvent = currentMillis;  //update timer with last event
 }
//clock
if (currentMillis - timeOfLastEvent1 > 400){
    lcd.setCursor(13,0); //15
    lcd.print (hour()); //ure
    lcd.print (":");
    lcd.print (minute()); //ure
    lcd.print (":");
    lcd.print (second()); //min
//    lcd.print (":");
//    lcd.print (currentMillis/1000);   //sek
    timeOfLastEvent1 = currentMillis;  //update timer with last event
}



//if(t%5){
 //do sth every 5% tiemes
//}
  //Setpoint = 370;
  lcd.setCursor(17,1);
  lcd.print(Setpoint);

  Input = (int)(DS2*10);

  double gap = abs(Setpoint-Input); //distance away from setpoint
  if(gap<10)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }
//////////////////////compute
//myPID.SetMode(mode); //MANUAL AUTOMATIC
 myPID.SetSampleTime(400);


if (  myPID.Compute() ==1 ){
  lcd.setCursor(17, 2);
  lcd.print(t);
}

  lcd.setCursor(0, 3);
   lcd.print ((millis() - windowStartTime)/1000);
  lcd.print ("p");
   lcd.print (myPID.GetKp(),0);
  lcd.print ("i");
  lcd.print (myPID.GetKi(),1);
  lcd.print ("d");
  lcd.print (myPID.GetKd(),1);
  lcd.print ("d");
  lcd.print (myPID.GetDirection(),0);
  lcd.print ("g");
  lcd.print (gap,0);


  lcd.setCursor(7, 0);
  lcd.print ("|i");
  lcd.print (Input,0);

  lcd.setCursor(7, 1);
  lcd.print ("|o");
  lcd.print (Output,0);

  lcd.setCursor(7, 2);
  lcd.print ("|s");
  lcd.print (Setpoint,0);

  //lcd.print (windowStartTime);
  /************************************************
* turn the output pin on/off based on pid output
************************************************/
  if(millis() - windowStartTime>WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }


  if(Output > millis() - windowStartTime){
    digitalWrite(RelayPin,LOW);
  }  else {
    digitalWrite(RelayPin,HIGH);
  }

  // digitalWrite(6, HIGH);

}
