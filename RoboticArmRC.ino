#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>



Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

RF24 radio(5, 6);

const byte address[6] = "00002";

#define SERVOMIN 150
#define SERVOMAX 600

int pos0 = 102;
int pos180 = 512;

char empty[3] = {0};
int emptyC[6] = {0};

const uint8_t s0 = 6;
const uint8_t s1 = 7;
const uint8_t s2 = 8;
const uint8_t s3 = 10;
const uint8_t s4 = 12;
const uint8_t s5 = 14;

int control[7];

int stage = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  pwm.begin();
  pwm.setPWMFreq(60);

  yield();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(stage == 0){
      setServoStart();
      stage = 1;
  }else if(stage == 1){
    if(radio.available()){
      char text[32] = "";
      radio.read(&text, sizeof(text));;
      if(text){
        char* pch = strtok(text, ":");
        int i = 0;
        while(pch != NULL && i < 8){
          control[i] = atoi(pch);
          i++;
          pch = strtok(NULL, ":");
        }
//        control[0] = roundUp(control[0]);
//        control[1] = roundUp(control[1]);
//        control[2] = roundUp(control[2]);
//        control[3] = roundUp(control[3]);
//        control[4] = roundUp(control[4]);
//        control[5] = roundUp(control[5]);
        setServo(s0, control[0]);
        setServo(s1, control[1]);
        setServo(s2, control[3]);
        setServo(s3, control[5]);
        setServo(s4, control[4]);
        setServo(s5, control[6]);
//      
        
      }
      printCont();
    }
    
  }



   
}

void initial(int value){
    //setServo(7, value);
  setServo(12, value);
  setServo(14, value);

}

void setServoStart(){
    setServo(s0, 90);
    setServo(s1, 90);
    setServo(s2, 90);
    setServo(s3, 90);
    setServo(s4, 90);
    setServo(s5, 0);
}

void setServo(int servo, int angle){
  int duty;
  if(servo == 14 || servo == 10){
    duty = map(angle, 140, 0, pos0, pos180);
  }else{
    duty = map(angle, 0, 140, pos0, pos180);
  }
 
  //Serial.print(duty);
  pwm.setPWM(servo, 0, duty);
}

int roundUp(int n){
  return(n+4)/5*5;
}

void printCont(){
  for(int i = 0; i < 7; i++){
    Serial.print(control[i]);
    Serial.print(" ");
  }
  Serial.println();
}
