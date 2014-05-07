#include <Arduino.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//howto  https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics

// Define variables

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int n = 1;

//Initialise the LCD
//LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

LiquidCrystal_I2C lcd(I2C_ADDR);  // Set the LCD I2C address

//LiquidCrystal_I2C lcd(0x38, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address

// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }

};

//void setup1()
//{
//   int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
//
//  // Switch on the backlight
//  pinMode ( BACKLIGHT_PIN, OUTPUT );
//  digitalWrite ( BACKLIGHT_PIN, HIGH );
//
//  lcd.begin(16,2);               // initialize the lcd
//
//   for ( int i = 0; i < charBitmapSize; i++ )
//   {
//      lcd.createChar ( i, (uint8_t *)charBitmap[i] );
//   }
//
//  lcd.home ();                   // go home
//  lcd.print("Hello, ARDUINO ");
//  lcd.setCursor ( 0, 1 );        // go to the next line
//  lcd.print (" FORUM - fm   ");
//  delay ( 1000 );
//}
//
//void loop1()
//{
//   lcd.home ();
//   // Do a little animation by writing to the same location
//   for ( int i = 0; i < 2; i++ )
//   {
//      for ( int j = 0; j < 16; j++ )
//      {
//         lcd.print (char(random(7)));
//      }
//      lcd.setCursor ( 0, 1 );
//   }
//   delay (200);
//}


/*
Tested on Arduino 1.0.3
UcontrolIt.TV
Bulding Block 1 - Sketch 1 Serial LCD

Test Sketch for the YwRobot Arduino serial LCD adapter - LCM1602 IIC V1

This may be the same as the Sainsmart I2C LCD2004 adapter but we havent tested it.

This example uses F Malpartida's ( http://www.electrofunltd.com/ )  NewLiquidCrystal
library available from:

https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads

You need to copy the new library to your Arduino libraries folder!

Mark Johnson
uControlIt.tv
December 2012

License: GNU General Public License

*/


void setup()
{
// Define LCD as 16 column x 2 rows
  lcd.begin (16,2);

// Switch on the backlight
//  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
//  lcd.setBacklight(HIGH);

// Goto first column (0 not 1!), first line (0 not 1!),
  lcd.setCursor ( 0, 0 );

// Print at cursor location
  lcd.print("UControlIt.TV");

// Go to first column (0 not 1!), second line (which is 1 not 2!)
  lcd.setCursor ( 0, 1 );

// Print at cursor location
  lcd.print("Count:");

}

void loop()
{
 lcd.setCursor ( 7, 1 );
 lcd.print(n);
 n=n+1;
 delay (1000);
}
