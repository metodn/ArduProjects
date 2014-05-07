// ARDUINO PRO Mini
//http://www.instructables.com/id/Intermediate-Arduino-Inputs-and-Outputs/
#include <Arduino.h>
#include <EEPROM.h>


//delay replacment
unsigned long timeOfLastEvent = 0;
unsigned long utimeOfLastEvent1 = 0;
unsigned long timeOfLastEvent1 = 0;
unsigned long timeOfLastEvent2 = 0;
unsigned long timeOfLastEvent3 = 0;
unsigned long uPt = 0;  //   _____|--uPT---|___uDt__
unsigned long uDt = 0;  //
unsigned long uDTmp = 0;
unsigned long uPTmp =0;

unsigned long St=0 ;
int EEsize = 1024;
int zz;
const int PinPUL =  3; //step
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


bool mstate = LOW;  //motor state low or hih

#define FORWARD   1
#define BACKWARD  0
int direction = FORWARD;

#define RKROG  73.0000   //premer v mm
#define PULREV   800  ///PREPISI iz  pulse /rev  koliko pulzov je potrebno za 1 obrat
unsigned int pulrev = PULREV;   ///PULses per REVolution
unsigned int fak_kor_za_izr= 1;   //na krmnlinuku je nastavljano 400
#define defaultdelay 34  // :34[ms] 151.33spd: 351.63cm

unsigned int sensorReading = 500; //a1 ANALOG
unsigned int motorSpeed = 0;

unsigned long spd1;    //used for potenciometer
unsigned int spd=330; //mm/min
#define spdFACTOR 200   //offzet pri zapisovanju v eeprom 0-255   +200
unsigned int stepFACTOR=1;

bool ms = 0 ; ///manual control of speed with potentiometer
bool PULState = 0;
bool izpis_pul=0;
unsigned long step_delay_us ;

unsigned long PULScount;
unsigned long LastPULScount;

int smotorSpeed = 0;
int lmotorSpeed = 0;
int emotorSpeed = 0;
short motdif = 0;

boolean LED0ST = 0;
boolean LED1ST = 0;
boolean BUT0ST = 0;
boolean BUT1ST = 0;
boolean BUTPROCESS =0;

void izpis_eeprom(){
    Serial.print( "[" );
    Serial.print( EEPROM.read(1)+spdFACTOR );  Serial.print( "," );
    Serial.print( EEPROM.read(2) );    Serial.print( "," );
    Serial.print( EEPROM.read(3) );
    Serial.print( "]\r" );

}


void setup() {
    // set the digital pin as output:
    pinMode(PinPUL, OUTPUT);
    pinMode(PinDIR, OUTPUT);
    pinMode(PinENA, OUTPUT);
    pinMode(PinLED0, OUTPUT);
    pinMode(PinLED1, OUTPUT);

    pinMode(PinSW0, INPUT);
    pinMode(PinSW1, INPUT);
    digitalWrite(PinENA, LOW);   ///LOW for turning on Ad4988

    Serial.begin(115200);
    sTime = millis();
    sKorakov = 0;

    spd    = EEPROM.read(1)+spdFACTOR;
    step_delay_us = EEPROM.read(2);
    direction     = EEPROM.read(3);
    if (direction==FORWARD)  digitalWrite(PinDIR, HIGH);
    if (direction==BACKWARD)  digitalWrite(PinDIR, LOW);
    digitalWrite(PinDIR, direction);
    izpis_eeprom();
}
//########################################################################################
//http://arduinobasics.blogspot.com/search/label/Serial%20Monitor
//http://stackoverflow.com/questions/5697047/convert-serial-read-into-a-useable-string-using-arduino
//http://forum.arduino.cc/index.php/topic,48925.0.html
// http://tronixstuff.com/2011/03/16/tutorial-your-arduinos-inbuilt-eeprom/
//########################################################################################
void one_step_function2 ( int stepper, int dir, int step_delay, int _direction )
{
      if (_direction == FORWARD)
      digitalWrite(dir, HIGH); //forward
      if (_direction == BACKWARD)
       digitalWrite(dir, LOW); //backward

    digitalWrite(stepper, LOW);
    //delay(step_delay );
    delayMicroseconds(step_delay*step_delay_us);
    digitalWrite(stepper, HIGH);
    //delay(step_delay );
    delayMicroseconds(step_delay*step_delay_us);
    digitalWrite(stepper, LOW);
}

void one_step_function3 ( int stepper, int dir, int step_delay, int _direction )
{

//       digitalWrite(PinDIR, _direction); //backward

    unsigned long currentMillis = millis(); //delay replacement
    if (currentMillis - timeOfLastEvent1 > step_delay){

//       delayMicroseconds( step_delay_us ); //delay za toliko kolikor je POT1 nasavljen
//    for (unsigned int i=1 ; i<=10 ; i++){
            digitalWrite(stepper, HIGH);
            delayMicroseconds(step_delay_us);
            digitalWrite(stepper, LOW);
            PULScount++;
//    }

       timeOfLastEvent1 = currentMillis;  //update timer with last event
    }
}
bool one_step_function ( int stepper, int dir, unsigned long step_delay, int _direction )
{
    //http://playground.arduino.cc/Code/TimingRollover
    unsigned long currentMicros = micros();
    if (currentMicros - timeOfLastEvent1 > step_delay*100 ){ //*100//1000*step_delay_us  *step_delay_us

//            PULState=!PULState;
//            digitalWrite(stepper, PULState);
//!!betaaaaaaaaaaaaa
//            delayMicroseconds(step_delay*step_delay_us);
 //           digitalWrite(stepper, LOW);
            //digitalWrite(stepper, !digitalRead(stepper));
            digitalWrite(stepper, HIGH);
            delayMicroseconds(2);
            digitalWrite(stepper, LOW);
            PULScount++;

       timeOfLastEvent1 = currentMicros;  //update timer with last event
        //update timer with last event
     return true ;
    }
    return false ;
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
    //Serial.print(".");
   unsigned long currentMillis = millis(); //delay replacement
    digitalWrite(PinLED0,LOW);


    if (currentMillis - timeOfLastEvent3 > 20){
            timeOfLastEvent3 = currentMillis;
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
                //Serial.print(">");
                //Serial.print(conte);
                Serial.print(content);
                //Serial.print("<");
                //Serial.print("\r");
            }

            if (content.compareTo("RIGHT") ==0  || content.compareTo("R") ==0  ){
                Serial.print(" EE-READ-!! [");
                spd    = EEPROM.read(1)+spdFACTOR;             //v eeprom pise od 1 do 255!!!!!!!
                step_delay_us = EEPROM.read(2);
                direction     = EEPROM.read(3);
                if (direction==FORWARD)  digitalWrite(PinDIR, HIGH);
                if (direction==BACKWARD)  digitalWrite(PinDIR, LOW);
                izpis_eeprom();
                //ms=1;    //aktiviraj potenciometer
                Serial.print("]\r");
                 ms=!ms;
            }

            if (content.compareTo("LEFT") ==0  || content.compareTo("L") ==0   || content.compareTo("W") ==0){
                Serial.print(" EE-WRITE-!! [");
                EEPROM.write(1,spd-spdFACTOR);
                EEPROM.write(2,step_delay_us);
                EEPROM.write(3,direction);
                izpis_eeprom();
                //ms=0;
                Serial.print("]\r");
            }



            if (content.compareTo("D") ==0){    //DOWN
                //step_delay_us=step_delay_us-100;
                //if (step_delay_us <100 ) step_delay_us=100;
        //        ms=1;                    //set the wariable that defines manual operation
                spd=spd-1*stepFACTOR;
                if (spd <=10 ) spd=10;  //lIMIT NA DOL
                Serial.print( char(12) );  //send newline
                Serial.print(spd);
                Serial.print("\r");

            }

            if (content.compareTo("U") ==0){    //DOWN
                spd=spd+1*stepFACTOR;
                if (spd >=6000 ) spd=6000;
                Serial.print( char(12) );
                Serial.print(spd);
                Serial.print("\r");
            }

            if (content.compareTo("S") ==0){     //STOP //start stop button
                Serial.print("STOP");
                BUT0ST=!BUT0ST;
                if (BUT0ST==0)  pulrev=PULREV;
                if (BUT0ST==1)  pulrev=0;
        //        ms=0;

                //digitalWrite(PinENA, LOW);          //DISABLE MOTOR
                digitalWrite(PinENA, !digitalRead(PinENA));
                Serial.print("\r");
            }

            if (content.compareTo("I") ==0){     //DIRECTION
                if (direction == FORWARD){
                    direction = BACKWARD;
                    digitalWrite(PinDIR, LOW); //forward
                } else if(direction == BACKWARD) {
                    direction = FORWARD;
                    digitalWrite(PinDIR, HIGH); //backward
                }
                Serial.print("Direction ");
                Serial.print(direction);
                Serial.print("\r");
            }
            if (content.compareTo("O") ==0){
                izpis_eeprom();
            }

            if (content.compareTo("A") ==0){
                izpis_pul=1;
                }
            if (content.compareTo("P") ==0){
                 ms=!ms;
                }
             if (content.compareTo("F") ==0){    //Faaktor koraka
                //step_delay_us=step_delay_us-100;
                //if (step_delay_us <100 ) step_delay_us=100;
        //        ms=1;                    //set the wariable that defines manual operation

                if(stepFACTOR==1) {
                    stepFACTOR=10;
                }else{
                    stepFACTOR=1;
                }
                Serial.print(stepFACTOR);
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
    }//end delay replacment3 fro SERIAL READ

    if (ms==1 && currentMillis - timeOfLastEvent2 > 600   ){  //ce je branje pot vklopljeno
                analogReference(DEFAULT);
                sensorReading = analogRead(A1);
                     //map(value, fromLow, fromHigh, toLow, toHigh)
                spd1 = map(sensorReading, 0, 1023, 1, 5);  //  g, 1, 1023, 1, 50);  tukaj je bila liita sa nastavi hitrost of 2 do 100

                stepFACTOR=spd1*5;
                Serial.print("\r Sped factor==>");
                Serial.print( stepFACTOR);
                Serial.print("<");
               // Serial.print(stepFACTOR);
                Serial.print(" \r");
                timeOfLastEvent2 = currentMillis;  //update timer with last event
    }


//carculate motorspped (delay) => dt
//



    float motorSpeed = 10000.0/((spd/60.0)/(obseg(RKROG)/PULREV));  //za izracuna cm/min v delay[ms] -> kar definira 1000



        uDTmp = micros() - uDt ;  //zracunal razliko  iz presnega delaya
        uPt = micros();    //start timer for pt----
      bool ret = one_step_function ( PinPUL, PinDIR, motorSpeed, direction );                ///CALLL MOTOR
        uPTmp = micros() - uPt;

         if (ret==1){
            uDt = micros();   //start tirem for next dt _____
         }
            //int modu=1000-motorSpeed;
            //if (ret==1 && PULScount%modu==0 ){

        if (ret==1 &&  izpis_pul==1){
            Serial.print("\r mot speed:"); Serial.print(motorSpeed); Serial.print(" :spd ");Serial.print(spd);  Serial.print(" : \r");
            Serial.print(uPTmp); Serial.print("uPL |");
            Serial.print(uDTmp); Serial.print("uDE |");
            //Serial.print(PULScount%50);
            Serial.print("\r");
            izpis_pul=0;

        }
//        if (ret == true){
                //unsigned long StartTimer = micros();
//                Serial.print("\r------------:utimer:"); Serial.print( (St-micros())/1000000.0 );Serial.print("::\r");
//                St = micros();
//        }
       // }else{Serial.print("f");}

//       timeOfLastEvent1 = currentMillis;  //update timer with last event
//    }

//vsako sekundo na zaslon napisi podatke
    if (currentMillis - timeOfLastEvent > 1001){

       // unsigned long StartTimer = micros(); //START TIMER
        digitalWrite(PinLED0,HIGH);
        //lKorakov =eKorakov;
        //eKorakov =korakov;
      // if ( millis()/1000
      //  Serial.print(  stikala() );
//         float st_obr_v_1_min = 60.0/t1kroga; // kolikokrat se bo obrnil v 1 minuti
        //Serial.print("T:");   Serial.print(t1kroga,1 ); Serial.print("s]");
       /// Serial.print("uS:");   Serial.print((step_delay_us/1000) ); Serial.print("]");

    #ifdef debug3
        lTime = eTime;    //LAP TIMER
        eTime = millis();    //END TIMER
        float t1kroga = (eTime-lTime)/1000.0;// cas enga koraga v [ms]   (PONOVEM CAS MED ODCITOKOMA !!!!
        Serial.print("Ko:");  Serial.print( korakov/1000.0,1 );
    #endif // debug3
        //float dt = motorSpeed/1000.0 ;   ///MOOGOCE DELJKENO 2
        Serial.print("de:");  Serial.print( motorSpeed,0); Serial.print("ms]"); //Serial.print(t1kroga,1);

       //float cvs = (float)(eTime-sTime)/1000.0;                  //  Cas V SKUNDAH

///
///
//NASTAVITVE DRIVERJA
/// 400 Imp za  polni krog
///  DOLzina 1 koraka (CE JE CEL KROG 400 KORAKOV IN JE OBSEG\
///  obseg(75.0)/ koliko pulzov za 1 krog (400)  ali 4000


        unsigned int pulses = PULScount-LastPULScount;
        LastPULScount=PULScount;
        Serial.print("P:");   Serial.print(pulses ); Serial.print("");

    #ifdef debug3
         korakov = pulses;
         float kns = korakov / t1kroga; //stveliko korakov deljeno skupni cas Korakv Na Sekundo
        //Serial.print("/");   Serial.print( t1kroga,1 );
        Serial.print( kns,1 );      Serial.print("[kor/s] ");
    #endif // debug3
        //float hitr = hitrostkms(51.0, 1.0, 1.8, kns);
        //Serial.print( hitr );

        Serial.print( char(12) );
        Serial.print("    ");
        float obs = obseg(RKROG);  //premer v mm
        ///Serial.print(obs/400);  Serial.print("dozlina na 1 impilz v cm");
        float dol1imp=((obs/PULREV)*pulses*60)/10; /// NE VEM AKAJ JE TU /100
        Serial.print( dol1imp,1 );  Serial.print("mm/mi ");  //Serial.print( obs );
        Serial.print("\r");
//OLD        Serial.print( (obs*st_obr_v_1_min)/10,1 );  Serial.print("cm/m#");       ///   dolzina JE obseg *stevilo obratov v 1 minuti (preracunano -nak  koliko porabi za 1 obrat)
        //unsigned long stobr= korakov/(eKorakov-lKorakov);    //stevliko obratov od zacetka
        //Serial.println(stobr);
//            Serial.print (micros()-StartTimer );

//        int smm=330; //speed v mmm
//        float newcalc =  (smm/60) / (obs/PULREV);
//        float newcalc_delay = 1000/newcalc;
//        Serial.print(newcalc);
//        Serial.print("Pul/s]<-->ms:");
//        Serial.print(newcalc_delay);
//        Serial.print("\r");
    #ifdef debug3
        Serial.print(" |obs: ");   Serial.print(obs);   Serial.print(" |st.obrv1min: ");   Serial.print(st_obr_v_1_min);   Serial.print(" |obr.st: ");    Serial.println(stobr);
    #endif // debug3

     timeOfLastEvent = currentMillis;  //update timer with last event
    }

// TODO (Stanley#1#): Dodaj parsenje serijskih komand ...
//Dodaj tudi zapiosvanje v eeprom
//DODAJ STOP/START TIPKO


//DISABLE
//    digitalWrite(PinENA, LOW);
//    digitalWrite(PinDIR, LOW);
//    digitalWrite(PinENA, LOW);

}

