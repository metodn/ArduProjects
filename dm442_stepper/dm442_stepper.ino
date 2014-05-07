// ARDUINO PRO Mini
#include <Arduino.h>
#include <EEPROM.h>

int EEsize = 1024;
int zz;
const int PinPUL =  3;
const int PinDIR =  4;
const int PinENA =  5;


const int PinSW0 =  7;
const int PinSW1 =  8;

#define PinLED0   9
#define PinLED1   2

unsigned long sTime = 0;
unsigned long lTime = 0;
unsigned long eTime = 0;
unsigned long korakov = 0;     //counter
unsigned long sKorakov = 0;    //start
unsigned long lKorakov = 0;   //lap
unsigned long eKorakov = 0;  //end
float KS = 0; //korakov na sek

#define PULREV   400   ///PREPISI iz  pulse /rev  koliko pulzov je potrebno za 1 obrat
unsigned int pulrev = PULREV;
unsigned int fak_kor_za_izr= 1;   //na krmnlinuku je nastavljano 400
#define defaultdelay 34  // :34[ms] 151.33spd: 351.63cm

int sensorReading = 500;
int motorSpeed = 5;
bool ms = 1 ;                //manual control of speed with potentiometer
unsigned int step_delay_us = 1000;
int smotorSpeed = 0;
int lmotorSpeed = 0;
int emotorSpeed = 0;
short motdif = 0;

boolean LED0ST = 0;
boolean LED1ST = 0;
boolean BUT0ST = 0;
boolean BUT1ST = 0;
boolean BUTPROCESS =0;

void setup() {
  // set the digital pin as output:
  pinMode(PinPUL, OUTPUT);
  pinMode(PinDIR, OUTPUT);
  pinMode(PinENA, OUTPUT);
  pinMode(PinLED0, OUTPUT);
  pinMode(PinLED1, OUTPUT);

  pinMode(PinSW0, INPUT);
  pinMode(PinSW1, INPUT);
  Serial.begin(57600);
  sTime = millis();
  sKorakov = 0;
  motorSpeed    = EEPROM.read(1);
  step_delay_us = EEPROM.read(2);
}

//########################################################################################
//http://arduinobasics.blogspot.com/search/label/Serial%20Monitor
//http://stackoverflow.com/questions/5697047/convert-serial-read-into-a-useable-string-using-arduino
//http://forum.arduino.cc/index.php/topic,48925.0.html
// http://tronixstuff.com/2011/03/16/tutorial-your-arduinos-inbuilt-eeprom/
//########################################################################################
void one_step_function ( int stepper, int dir, int step_delay, String _direction )
{
      if (_direction == "forward")
      digitalWrite(dir, HIGH);
      if (_direction == "backward")
       digitalWrite(dir, LOW);

    digitalWrite(stepper, LOW);
    //delay(step_delay );
    delayMicroseconds(step_delay*step_delay_us);
    digitalWrite(stepper, HIGH);
    //delay(step_delay );
    delayMicroseconds(step_delay*step_delay_us);
    digitalWrite(stepper, LOW);
}


void blink(int ino, int cnt, int wait){

        for (int i=0; i < cnt; i++){
            digitalWrite (ino, HIGH);
            delay(wait);
            digitalWrite (ino, LOW);
            delay(wait);
          }
}


float obseg(float diameter){

    return  2*3.14159*(diameter/2);   //v milimoetih  60/1,7*160   60sekind /cas 1 obrata * obseg
}
//  hitrostkms(51, 18, 7.5);
// d je premer zunajega kolesa  v [mm]
// i je razmerje 1:18
// k kot 1 koraka na motorja 7.5
//kns korakov na sekundo
float hitrostkms(float d, float i, float k, int kns){

    float k1 = 400.0/k;       //st korakov potrebnih za obrat motorja   HP je bil 360
    // Serial.println( k1,5 );
    float i1 = k1 * i;        //korak motorja potrebnih za en obrat pogonske gredi
   //  Serial.println( i1,4 );
    float o = d * 3.14;      //obseg
    //  Serial.println( o,3 );
    float ks = k/ i;             // kotna stopinja na gnani gerdi
  //     Serial.println( ks,3 );
    float kgg = o/i1;               // kot gnane gredi za 1 korak motorja
  //     Serial.println( kgg,4 );
    float kms = i1/o;            // hitrost vrtenja cm/minuto
  //     Serial.println( kms,3 );

    return (kns * kgg) * 10;   //v [cm/min] *10 da iz mm dobimo cm
}



void loop()
{


//http://www.robotc.net/wiki/ARDUINO_328_Functions_Strings
    String content = "";
    //char* content ="";
    //char conte[100];
    char character;

    while(Serial.available())
    {
        character = Serial.read();
        content.concat(character);
        //content=content+character;
    }
    //content.toCharArray(conte,sizeof(conte)));
    //http://arduino.cc/en/Tutorial/StringComparisonOperators

    if (content != "")
    {
        Serial.print("-");
        //Serial.print(conte);
          Serial.print(content);
        Serial.print("-");

    }
    if (content.compareTo("LEFT") ==0  || content.compareTo("L") ==0){
        Serial.print("EE-R");
        ms=1;
        motorSpeed    = EEPROM.read(1);
        step_delay_us = EEPROM.read(2);
        Serial.print(motorSpeed);

    }
    if (content.compareTo("RIGHT") ==0  || content.compareTo("R") ==0){
        Serial.print("EE-S");
        EEPROM.write(1,motorSpeed);
        EEPROM.write(2,step_delay_us);
        Serial.print( EEPROM.read(1) );  Serial.print( char(12) );
        Serial.print( EEPROM.read(2) );
        ms=0;

    }

    if (content.compareTo("U") ==0){
        step_delay_us=step_delay_us-100;
        if (step_delay_us <100 ) step_delay_us=100;
        ms=1;                    //set the wariable that defines manual operation
        Serial.print( char(12) );  //send newline
        Serial.print(step_delay_us);

    }

    if (content.compareTo("D") ==0){
        step_delay_us=step_delay_us+100;
        ms=1;
        Serial.print( char(12) );
        Serial.print(step_delay_us);

    }

    if (content.compareTo("S") ==0){      //start stop button
        Serial.print("STOP");
        BUT0ST=!BUT0ST;
        if (BUT0ST==0)  pulrev=PULREV;
        if (BUT0ST==1)  pulrev=0;
        ms=0;


    }

//        EEPROM.write(1,motorSpeed);
//        zz = EEPROM.read(1);
//        Serial.print(zz);Serial.print(" <val");

#ifdef debug3


    BUT0ST = digitalRead(PinSW0);
    BUT1ST = digitalRead(PinSW1);
    if ( BUT0ST == LOW )   { LED0ST != LED0ST;   Serial.print("SW0 ");     Serial.print(BUT0ST); Serial.print(LED0ST);  BUTPROCESS=1; digitalWrite(9, HIGH);}
    if ( BUT1ST == LOW )   { LED1ST != LED1ST;   Serial.print("SW1 ");     Serial.print(BUT1ST); Serial.print(LED1ST);  digitalWrite(9, LOW);}
    digitalWrite (PinLED0,  LED0ST);
    digitalWrite (PinLED1,  LED1ST);
#endif // debug3
 //
  //  blink (PinLED1, 3, 100 );


    unsigned long currentMillis = millis();
    //Serial.println("Begin");
    digitalWrite(PinLED0,LOW);
    if (pulrev==0) {delay(100); } //safety za stop funckijo na ne zafloda serijskega vodila
    for (unsigned long i =1 ;i <=pulrev ;i++){ // 400 korakov je en obrat
//      if (i%40-motorSpeed) {
//            Serial.print( char(12) );
//             Serial.print(".");
//        }

      if (i%10 == 0 && ms==0){     //vsk modul deset ali pa ce ni manual spped
                sensorReading = analogRead(A1);
                motorSpeed = map(sensorReading, 1, 1023, 1, 50);  // tukaj je bila liita sa nastavi hitrost of 2 do 100
                lmotorSpeed = emotorSpeed;
                emotorSpeed = motorSpeed;
                motdif= emotorSpeed-lmotorSpeed;
                if (motdif>=1){
                    Serial.println("                                ");
                    Serial.print( char(12) );
                    Serial.print( motorSpeed);


                }

#ifdef debug3
          if (i%(50-motorSpeed) == 1) {Serial.print(" delay: ");  Serial.print(motorSpeed); }    //izpisujemo relativno na hitrost motorja
#endif // debug3
       }
      one_step_function ( PinPUL, PinDIR, motorSpeed, "forward" );             //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      korakov = korakov + 1;

    }

    digitalWrite(PinLED0,HIGH);
//    blink (PinLED0, 2, 100 );
//    blink (10, 4, 100 );
    lTime = eTime;
    lKorakov =eKorakov;

    eTime = millis();
    eKorakov =korakov;
  // if ( millis()/1000
  //  Serial.print(  stikala() );
     float t1kroga = (eTime-lTime)/1000.0;// cas enga koraga v [ms]
     float st_obr_v_1_min = 60.0/t1kroga; // kolikokrat se bo obrnil v 1 minuti
    //Serial.print("T:");   Serial.print(t1kroga,1 ); Serial.print("s]");
    Serial.print("uS:");   Serial.print((step_delay_us/1000) ); Serial.print("]");
#ifdef debug3
    Serial.print("Ko:");  Serial.print( korakov/1000.0,1 );
#endif // debug3
    //float dt = motorSpeed/1000.0 ;   ///MOOGOCE DELJKENO 2
    Serial.print("de:");  Serial.print( motorSpeed); Serial.print("ms]");

    float cvs = (float)(eTime-sTime)/1000;                  //  Cas V SKUNDAH
    float kns = korakov / cvs; //stveliko korakov deljeno skupni cas Korakv Na Sekundo

#ifdef debug3
    Serial.print( kns,1 );  Serial.print("[kor/s] ");
#endif // debug3
    //float hitr = hitrostkms(51.0, 1.0, 1.8, kns);
    //Serial.print( hitr );

    float obs = obseg(75.0) * fak_kor_za_izr;  //premer v mm
    Serial.print( char(12) );
    Serial.print("sp:");
    unsigned int stobr= korakov/(eKorakov-lKorakov);    //stevliko obratov od zacetka
    Serial.print( (obs*st_obr_v_1_min)/10,1 );  Serial.print("cm/m#");       ///   dolzina JE obseg *stevilo obratov v 1 minuti (preracunano -nak  koliko porabi za 1 obrat)
    Serial.println(stobr);
#ifdef debug3
    Serial.print(" |obs: ");   Serial.print(obs);   Serial.print(" |st.obrv1min: ");   Serial.print(st_obr_v_1_min);   Serial.print(" |obr.st: ");    Serial.println(stobr);
#endif // debug3


// TODO (Stanley#1#): Dodaj parsenje serijskih komand ...
//Dodaj tudi zapiosvanje v eeprom
//DODAJ STOP/START TIPKO


//DISABLE
//    digitalWrite(PinENA, LOW);
//    digitalWrite(PinDIR, LOW);
//    digitalWrite(PinENA, LOW);

}

