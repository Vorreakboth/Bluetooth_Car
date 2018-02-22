#include "Define.h"
#include "myClass.h"
#include "Bluetooth.h"
#include "Objavoid.h"

float KP = 0.2; //0.8 -> 0.2
float KD = 100; //100 -> 120

int MaxSpd = 255;
int MSPD = MaxSpd; //max speed 230 -> 
int BSPD = MSPD; //base speed 200 <- 
int LSPD = MSPD-100; //low speed 100 -> 

char inputSet;
char inputChar;
char road;

MOTOR_Initialize motorR(in1,in2), motorL(in3,in4); //initial in1, in2, enable of motor right or left

float SR[5];
float EYE;
float lastEYE;
char lstErr;

void setup()
{
 Serial.begin(9600);
 //pinMode
 pinMode(A0, INPUT);
 pinMode(A1, INPUT);
 pinMode(A2, INPUT);
 pinMode(A3, INPUT);
 pinMode(A4, INPUT);
 for (int i = 2 ; i <= 10 ; i++)
    pinMode(i, OUTPUT);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 
 int mode, count = 0;
 
 motorL.start();
 motorR.start();
 
 mode = 0;
 while(mode != 1){
    getSR();
    if(SR[0]<550 && SR[1]<550 && SR[2]<550 && SR[3]<550 && SR[4]<550){
      road = 'w';
      mode = 1;
      Blue(true);
    }
    else if(SR[0]>550 && SR[1]>550 && SR[2]>550 && SR[3]>550 && SR[4]>550){
      road = 'b';
      mode = 1;    
      Yellow(true);
    }
 }
 delay(100);
 
 Blue(false);
 Yellow(false);
 
 mode = 0;
 while(mode != 1){
    getSR();
    if((SR[0]<550 && SR[1]<550 && SR[2]>550 && SR[3]<550 && SR[4]<550) || (SR[0]>550 && SR[1]>550 && SR[2]<550 && SR[3]>550 && SR[4]>550)){
      inputSet = 'X';
      mode = 1;
      Blue(true);
    }
    else if((SR[0]<550 && SR[1]<550 && SR[2]<550 && SR[3]<550 && SR[4]<550) || (SR[0]>550 && SR[1]>550 && SR[2]>550 && SR[3]>550 && SR[4]>550)){
      count++;
      if(count > 20000){
        inputSet = 'Z';
        mode = 1;
        Blue(true);
        Yellow(true);
      }
    }
    else if(Serial.available()){
      inputSet = 'x';
      mode = 1;
      Yellow(true);
    }
    else
      count = 0;
 }
 delay(100);
 
 Blue(false);
 Yellow(false);
}

float eye() {
  float duration, distance = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

void avoid() {
  speed('R', 100);
  delay(500);
  speed('R', 0);
  delay(100);
  speed('a',200);
}

void getSR(){
  SR[0] = analogRead(A0);
  SR[1] = analogRead(A1);
  SR[2] = analogRead(A2);
  SR[3] = analogRead(A3);
  SR[4] = analogRead(A4);
}

boolean notDetect(){
  switch(road){
    case 'w': return (SR[0]<550 && SR[1]<550 && SR[2]<550 && SR[3]<550 && SR[4]<550);break;
    case 'b': return (SR[0]>550 && SR[1]>550 && SR[2]>550 && SR[3]>550 && SR[4]>550);break;
  }
}

float getErr(){
  float p=0.0, s=0.0;
  switch(road){
    case 'w': if(SR[0]>550) lstErr='r';//White Road
              if(SR[4]>550) lstErr='l';//White Road
              break;
    case 'b': if(SR[0]<550) lstErr='r';//Black Road
              if(SR[4]<550) lstErr='l';//Black Road
              break;
  }
  
  p = SR[0]*100 + SR[1]*200 + SR[2]*500 + SR[3]*800 + SR[4]*900;
  s = SR[0] + SR[1] + SR[2] + SR[3] + SR[4];
  return p/s;
}

void loop()
{
  float err = 0, O = 0;
  float pe = 0;
  int spL = 0, spR = 0;
  int c = 0;
  
  do{
    if(Serial.available()){
            inputChar = Serial.read(); //read the input   
            if(inputChar == 'x' || inputChar == 'X'){
              inputSet = inputChar;
              motorL.start();
              motorR.start();
              Stop();
              KP = 0.2;
              KD = 100;
            }
        if(inputChar == 'O' || inputChar == 'o') KP += 1;
        if(inputChar == 'U' || inputChar == 'u') KP -= 1;
        if(inputChar == 'W' || inputChar == 'w') KD += 20;
        if(inputChar == 'P' || inputChar == 'p') KD -= 20;
    }
    
    switch(inputSet){
        case 'Z': if(Serial.available())
                    inputSet = 'x';
                  EYE = eye();
                  lastEYE = EYE;
                  if(EYE > 0 && EYE <= 20)
                    speed('R', 255);
                  else if(EYE > 20 && EYE <= 25)
                    speed('L', 255);
                  else
                    speed(' ', 200);
                  break;
        case 'X': if(Serial.available())
                    inputSet = 'x';
                  getSR();
                  err = getErr() - SETPOINT;
                  
                  if(notDetect()){  
                    Blue(false);Yellow(true);
                    if(lstErr=='r'){
                      motorL.reverse(LSPD);
                      motorR.forward(MSPD);
                    }
                    if(lstErr=='l'){
                      motorL.forward(MSPD);
                      motorR.reverse(LSPD);
                    }
                  }
                  else{
                    Yellow(false);Blue(true);
                    O = KP * err + KD * (err - pe);
                    pe = err;
                    switch(road){
                      case 'w': spL = BSPD + (int)O;
                                spR = BSPD - (int)O;
                                break;
                      case 'b': spL = BSPD - (int)O;
                                spR = BSPD + (int)O;
                                break;
                    }
                    spL = spL>MSPD?MSPD:spL;
                    spR = spR>MSPD?MSPD:spR;
                    
                    if(spR<0){
                      if(spR<(LSPD*-1)) spR = LSPD*-1;
                      Blue(false);motorR.reverse(spR*-1);
                    }
                    else{
                      Blue(true);motorR.forward(spR);
                    }
                    
                    if(spL<0){
                      if(spL<(LSPD*-1)) spL = LSPD*-1;
                      Blue(false);motorL.reverse(spL*-1);
                    }
                    else{
                      Blue(true);motorL.forward(spL);
                    }
                  }

                  if (millis() > 1000)
                  switch(road){
                    case 'w': if(SR[0] > 550 && SR[4] > 550){
                                c++;
                                if(c>150){
                                  Blue(false);
                                  Yellow(false);
                                  motorL.stop();
                                  motorR.stop();
                                  exit(0);
                                }
                              }
                              else{
                                c=0;
                              }
                              break;
                    case 'b': if(SR[0] < 550 && SR[4] < 550){
                                c++;
                                if(c>150){
                                  Blue(false);
                                  Yellow(false);
                                  motorL.stop();
                                  motorR.stop();
                                  exit(0);
                                }
                              }
                              else{
                                c=0;
                              }
                    break;
                  }
                  break;
        case 'x': switch(inputChar){
                    case 'F': Blue(true);Yellow(false);Forward();break;
                    case 'B': Yellow(true);Blue(false);Back();break;
                    case 'L': Blue(true);Yellow(false);Left();break;
                    case 'R': Yellow(false);Blue(true);Right();break;
                    case 'S': Blue(false);Yellow(false);Stop();break;
                    case '0': Speed = 0;break;
                    case '1': Speed = 50;break;
                    case '2': Speed = 75;break;
                    case '3': Speed = 100;break;
                    case '4': Speed = 125;break;
                    case '5': Speed = 150;break;
                    case '6': Speed = 175;break;
                    case '7': Speed = 200;break;
                    case '8': Speed = 225;break;
                    case '9': Speed = 255;break;
                    case 'q': Speed = 255;break;
                    case 'V': Horn(true);break;
                    case 'v': Horn(false);break;
                    case 'W': Blue(true);break;
                    case 'w': Blue(false);break;
                    case 'U': Yellow(true);break;
                    case 'u': Yellow(false);break;
                    //case 'P': break;
                    //case 'p': break;
                    //case 'O': break;
                    //case 'o': break;
                    case 'D': Speed = 0;Blue(false);Yellow(false);Horn(false);break;
                  }
                  break;
    }
    
  }while(1);
}
