#include <ChainableLED.h>
#include <Ultrasonic.h>

const int rightbuzzer = 6;
const int leftbuzzer = 2;
const int rightultrasonic = 7;
const int leftultrasonic = 8;

const int ArroundNumber = 5;
const int PermaRadius = 15;

const int PermaRaduisHit = 10;
const int MaxAlertHit = 50;

const int MaxAlert = 2;

const int Marge = 1;

int lastRight = 600;
int alertlevelRight;
int lastLeft = 600;
int alertlevelLeft;

Ultrasonic ultrasonicRight(rightultrasonic); //init right distance sensor
Ultrasonic ultrasonicLeft(leftultrasonic); //init left distance sensor

ChainableLED leds(4, 5, 2); //Leds port + number

void ledLight(int num, int red, int green, int blue) //Simplified led method
    {
        leds.setColorRGB(num, red, green, blue);
    }

void alert(int type, boolean Right){ //Leds color + Buzzer alarms
  if(Right==true){
    if(type==0){
      ledLight(0,255,0,0);
      digitalWrite(rightbuzzer, HIGH);
      delay(PermaRaduisHit);
      digitalWrite(rightbuzzer, LOW);
    } else if(type==1){
      ledLight(0,255,127,0);
      digitalWrite(rightbuzzer, HIGH);
      delay(MaxAlertHit);
      digitalWrite(rightbuzzer, LOW);
    }
  } else {
    if(type==0){
      ledLight(1,255,0,0);
      digitalWrite(leftbuzzer, HIGH);
      delay(PermaRaduisHit);
      digitalWrite(leftbuzzer, LOW);
    } else if(type==1){
      ledLight(1,255,127,0);
      digitalWrite(leftbuzzer, HIGH);
      delay(MaxAlertHit);
      digitalWrite(leftbuzzer, LOW);
    }
  }
}
//checking if value is within "ecart" number limit (affect sensibility)
boolean Arround(int value, int ecart, int tocompare){
  for(int i=value; i<=value+ecart; i++){
    if(i==tocompare){
      return true;
    }
  }
  for(int i=value; i>=value+ecart; i--){
    if(i==tocompare){
      return true;
    }
  }
  return false;
}

void setup() {
  //init buzzers
  pinMode(rightbuzzer, OUTPUT);
  pinMode(leftbuzzer, OUTPUT);
}

void loop() {
  //Right check
  int tempRight = ultrasonicRight.MeasureInCentimeters();
  if(Arround(lastRight, ArroundNumber,tempRight)==false){
    if(tempRight+Marge<lastRight){
      lastRight=tempRight;
      alertlevelRight+=1;
    }
    else{
      alertlevelRight=0;
      lastRight=600;
    }
  } else{
    if(tempRight+Marge<lastRight){
     tempRight=lastRight;
     alertlevelRight+=1;
    }
  }

  if(tempRight<=PermaRadius){
    alert(0, true);
    return;
  }
  if(alertlevelRight>MaxAlert){
    alert(1, true);
    return;
  }

  //-----------------------------

  //Left check
  int tempLeft = ultrasonicLeft.MeasureInCentimeters();
  if(Arround(lastLeft, ArroundNumber,tempLeft)==false){
    if(tempLeft+Marge<lastLeft){
      lastLeft=tempLeft;
      alertlevelLeft+=1;
    }
    else{
      alertlevelLeft=0;
      lastLeft=600;
    }
  } else{
    if(tempLeft+Marge<lastLeft){
     tempLeft=lastLeft;
     alertlevelLeft+=1;
    }
  }

  if(tempLeft<=PermaRadius){
    alert(0, false);
    return;
  }
  if(alertlevelLeft>MaxAlert){
    alert(1, false);
    return;
  }
  //---------------------------
  //Delay + Reset leds color
  delay(10);
  ledLight(0,0,255,0);
  ledLight(1,0,255,0);
}
