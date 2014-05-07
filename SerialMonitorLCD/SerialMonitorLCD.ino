#include <Arduino.h>
#include <LiquidCrystal.h>



#ifdef DEBUG
#define DEBUG_PRINT(str) \
    Serial.print(" |"); \
    Serial.print(millis()); \
    Serial.print(": "); \
    Serial.print(__FUNCTION__); \
    Serial.print("() in "); \
    Serial.print(__FILE__); \
    Serial.print(':'); \
    Serial.print(__LINE__); \
    Serial.print(' ='); \
    Serial.println(str);

  #define DEBUG_PRINT2(str,desc) \
    Serial.print(desc); \
    Serial.print(': '); \
    Serial.print(millis()); \
    Serial.print(": "); \
    Serial.print(__FUNCTION__); \
    Serial.print("() in "); \
    Serial.print(__FILE__); \
    Serial.print(':'); \
    Serial.print(__LINE__); \
    Serial.print(' == '); \
    Serial.println(str)

  #define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
  #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINT2(x,desc)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x)
#endif

//BUILD options -> Cutom Variables -> ${UPLOAD_PORT) =com6

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


unsigned long sTime = 0;
unsigned long lTime = 0;
unsigned long eTime = 0;
unsigned long lastDebounceTime = 0;

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define LCDrow   2
#define LCDcol   16

char LCDHID[LCDrow][LCDcol]= {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

char LCDBUF[LCDrow+1][LCDcol]= { {90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90},
                                 {66,67,68,69,70,71,72,73,65,65,65,65,66,67,68,122},
                                 {48,49,50,50,51,51,52,65,65,65,65,65,65,65,66,122}};

                              //5,77,78,75,75,75,75,75,75,75,75,75,75,75,75,76}};

int byteRead;

char** LCDstr ;//= malloc(100);

//char* LCDcontent[]={"","",""};            //http://arduino.cc/en/Reference/string
//char* LCDcontent="                ";          // '\0' keep c string null treminated
//char* LCDcontent1="                ";
//char* LCDcontent2="                ";
String LCDcontent="                                ";
String LCDcontent1="";
String LCDcontent2="";

//String [] comPortList;

bool LCDredraw=0;

//////////////serial variables

    unsigned int numOfBytes=0;
    unsigned int secnumOfBytes=0;
    short row=0;
    short col=0;


//#define DEBUG1
//#define DEBUG

void setup()
{
     lcd.begin(LCDcol, LCDrow);              // start the library
     lcd.setCursor(0,0);
     lcd.print("Serial Monitor 57600  by Metod Novak from Slovenia and a1"); // print a simple message
     lcd.cursor();

     delay(500);
     lcd.clear();
     lcd.setCursor(0,0);

     Serial.begin(57600);

//    LCDstr= malloc(100);

}

//#############################################################################################
//
//  FUNCTIONS
//#############################################################################################
// read the buttons
int read_LCD_buttons()
{
   adc_key_in = analogRead(0);      // read the value from the sensor
   // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
   // we add approx 50 to those values and check to see if we are close
   if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
   // For V1.1 us this threshold
   if (adc_key_in < 50)   return btnRIGHT;
   if (adc_key_in < 250)  return btnUP;
   if (adc_key_in < 450)  return btnDOWN;
   if (adc_key_in < 650)  return btnLEFT;
   if (adc_key_in < 850)  return btnSELECT;

  //13  sensorVal = constrain(map(analogRead(A0),200,800,0,100),0,100                TOTDO  IDEA

   // For V1.0 comment the other threshold and use the one below:
  /*
   if (adc_key_in < 50)   return btnRIGHT;
   if (adc_key_in < 195)  return btnUP;
   if (adc_key_in < 380)  return btnDOWN;
   if (adc_key_in < 555)  return btnLEFT;
   if (adc_key_in < 790)  return btnSELECT;
  */


   return btnNONE;  // when all others fail, return this...
  }

void  LCDbuftest(){

    int r = 0;
    DEBUG_PRINT(sizeof(LCDstr));
    int isize = sizeof(LCDBUF[2]);
    DEBUG_PRINT2('size of isize',isize);
    DEBUG_PRINT(isize);
    for (int r=1 ; r<= LCDrow ; r++)     //xscni z prvo vrsto
    {
        for (int i=0 ; i<= LCDcol ; i++)
        {
#ifdef DEBUG
            Serial.print(i);            Serial.print("x");            Serial.print(r);            Serial.print("| =");            Serial.println(LCDBUF[r][i]);
#endif

            if (r==1) {   LCDcontent  = LCDcontent + LCDBUF[r][i];      }
            if (r==2) {   LCDcontent2 = LCDcontent2 + LCDBUF[r][i];     }
        }
    }

    lcd.setCursor(0,0);
    lcd.print(LCDcontent);
    lcd.setCursor(0,1);
    lcd.print(LCDcontent2);

    LCDcontent1 = LCDcontent + LCDcontent2;

}


//#############################################################################################
//
//  MAIN
//#############################################################################################


void loop()
{


      // when characters arrive over the serial port...
      if (Serial.available()) {
            numOfBytes=Serial.available();
        // wait a bit for the entire message to arrive
        // clear the screen

     //   lcd.scrollDisplayRight() ();
        char sread;

        // read all the available characters
        while (Serial.available() > 0) {
          // display each character to the LCD

                            //stevec da lahko ugotovim kdaj v 2 vrstico pisat
           secnumOfBytes++;
           col++;
           if( col % LCDcol == 0) { row++; if(row>=LCDrow) {row=0;}    lcd.setCursor(0,row);  }
            sread=Serial.read();
//            if (sread== '\n')                                                                                           //http://forum.arduino.cc/index.php/topic,48925.0.html
            if (sread== char(13) ) {      //'\r'                                                                                      //http://forum.arduino.cc/index.php/topic,48925.0.html
                lcd.setCursor(0,0);
                col=0;
                continue;   //skipwriting to lcd
            }
            if (sread== char(12) ) {      //FORM FEED V drugo vrstico
                lcd.setCursor(0,1);
                col=0;
                row=1;
                continue;   //skipwriting to lcd
            }

           lcd.write(sread);

        }

        delay(50);
    #ifdef DEBUG
        if (numOfBytes >0 ) { Serial.print(row); Serial.print("x"); Serial.print(col); Serial.print(" | "); Serial.print(secnumOfBytes);  Serial.print(" | "); Serial.print(numOfBytes);  Serial.print(" | "); Serial.print((col % LCDcol) );    Serial.print(" | ");  Serial.print(sread); Serial.print(" | ");  Serial.println(int(sread)) ; }
    #endif // DEBUG

      }

      //////////keboard send

      if ((millis() - lastDebounceTime) > 500)     //debounce de repeat
        {
            lcd_key = read_LCD_buttons();  // read the buttons
            lastDebounceTime = millis();
        }
        else
        {
            lcd_key=99;
        }


     switch (lcd_key)               // depending on which button was pushed, we perform an action
     {
       case btnRIGHT:
         {
            lcd.noBlink();
            //lcd.clear();
            //lcd.print("RIGHT ");

            Serial.write("RIGHT");


         break;
         }
       case btnLEFT:
         {
            lcd.blink();
            //lcd.clear();
            //lcd.print("LEFT   ");

            Serial.write("LEFT");

         break;
         }
       case btnUP:
         {
            lcd.cursor();
            //lcd.clear();
            //lcd.print("UP");

            Serial.write("U");


         break;
         }
       case btnDOWN:
         {
            lcd.noCursor();
            //lcd.clear();
            //lcd.print("DOWN  ");

            Serial.write("D");


         break;
         }
       case btnSELECT:
         {
            //lcd.clear();
            lcd.blink();
           // lcd.print("SELECT");

            Serial.write("S");


         break;
         }
         case btnNONE:
         {
    //     lcd.print("NONE  ");
    //     Serial.write("NONE");
         break;
         }
     }




}






void loop1()
{
//// http://arduino.cc/en/Tutorial/LiquidCrystalSerial    ///SERIAL TO LCD EXAMPLE

     if (Serial.available()) {
    //    // wait a bit for the entire message to arrive
    //    delay(100);
    //    // clear the screen
        //lcd.clear();
    //    // read all the available characters
        while (Serial.available() > 0) {
          // display each character to the LCD
    //      lcd.write(Serial.read());
            LCDcontent = LCDcontent + (char)Serial.read();

            for (int t=sizeof(LCDcontent) ; t <= (LCDrow*LCDcol) ; t++) {
                LCDcontent = LCDcontent + "0";
            }
            LCDredraw = 1;
        }


     }

    //Serial.flush();

    /* read the most recent byte */
    //  byteRead = Serial.read();


//    if(Serial.available() > 0)
//      {
//          content = Serial.readStringUntil('\n');
//
//      }
//  for (int i = sizeof(content); i <=16; i++){
//    content=content+" ";
//  }

    // Serial.readBytes(buffer, length)
    /*ECHO the value that was read, back to the serial port. */
    //lcd.clear();

#ifdef DEBUG
    Serial.println("DEBUG");
#endif


#ifdef DEBUG
    Serial.println(LCDcontent);
    Serial.println(LCDcontent2);
    Serial.println(LCDcontent1);
#endif
    lcd.clear();
    //for (int c=0; c<=LCDcol; c++){

if (LCDredraw==1){
    int r=0;
    for (int i=0 ; i<= (LCDrow*LCDcol) ; i++)     //xscni z prvo vrsto
    {
        if ( i%LCDcol == 0 ) {  lcd.setCursor(0,r); r++;}   //rowcounter
            //if ( char(LCDcontent[i]) <=33 )  LCDcontent[i]=char(65);
          lcd.write( LCDcontent[i] );
//lcd.scrollDisplayRight();
          Serial.print( LCDcontent[i] );
         // DEBUG_PRINT(r);


    }
    LCDredraw=0;
}
    LCDcontent="";
    LCDcontent2="";
    LCDcontent1="";


    //unsigned int tmpsize= sizeof(LCDBUF);
    //Serial.println(tmpsize);
    delay(100);

}
