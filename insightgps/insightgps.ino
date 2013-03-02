#include <TinyGPS.h>   //built using version 12
#include <LiquidCrystal.h>

//Version 7.5

LiquidCrystal vfd(10,11,2,3,4,5,6,7,8,9);
TinyGPS gps;

void setup(){
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  vfd.begin(20,2); 
  vfd.print("Version 7.5");
  vfd.setCursor(0,1);
  Serial.begin(4800);
digitalWrite(13,LOW);



}

void loop(){
while (Serial.available())
  {
    digitalWrite(13,HIGH);
    int c = Serial.read();
    //vfd.print(gps.satellites());   //this doesn't work
    if (gps.encode(c)) //if true, new data, so update
    {
      updateDisplay();
    }
    digitalWrite(13,LOW);
  }
}

void updateDisplay(){
float vbatt;
int year;
byte month, day, hour, minutes, second, hundredths;
unsigned long fix_age;
float flat, flon;
gps.f_get_position(&flat, &flon, &fix_age);
float alt = gps.f_altitude();  // +/- altitude in meters
float cou = gps.f_course();    // course in degrees
float mph = gps.f_speed_mph(); // speed in miles/hr
byte  sats= gps.satellites();
gps.crack_datetime(&year, &month, &day, &hour, &minutes, &second, &hundredths, &fix_age);

vbatt = (analogRead(0)*0.0048828125*3);
alt*=3.28084; //convert to feet

byte isSummer = 0;   //change to 1 for summer time, 0 for winter

switch (isSummer){
  case 0:
  if (hour>4)    hour-=6;
if (hour == 0) hour = 18;
if (hour == 1) hour = 19;     //piss-poor gmt to cst
if (hour == 2) hour = 20;
if (hour == 3) hour = 21;
if (hour == 4) hour = 22;
if (hour == 5) hour = 23;
  break;
  case 1:
if (hour>4)    hour-=5;
if (hour == 0) hour = 19;
if (hour == 1) hour = 20;     //piss-poor gmt to cdt
if (hour == 2) hour = 21;
if (hour == 3) hour = 22;
if (hour == 4) hour = 23;
}


if (mph<1)mph=0;
flon = abs(flon);

vfd.setCursor(0,0);
vfd.print(alt,0);
vfd.print(" ");
vfd.print(mph,0);
vfd.print(" ");
vfd.print(hour);
vfd.print(":");
if(minutes<10)vfd.print("0");
vfd.print(minutes);
vfd.print(" ");
if(mph==0)vfd.print("--");
if(mph>0)vfd.print(cou,0);
vfd.print(" ");
vfd.print(vbatt,3);
vfd.print("  ");

vfd.setCursor(0,1);
vfd.print(sats);
vfd.print(" ");
vfd.print(flat,5);
vfd.print(" ");
vfd.print(flon,5);
}
