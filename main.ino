#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "MPUSensor.h"
#include <string.h>

#define MPUNUM 3

RF24 radio(7, 8);
const byte address[6] = "00002 ";

const int FLEX_PIN = A0;
const int minVal = 0;
const int maxVal = 1000;

char message[32] = {0};
char empty[32] = {0};
char number[16];

const char separator[] = ":";

MPU6050 mpu1(2);
MPU6050 mpu2(3);
MPU6050 mpu3(4);
MPU6050* mpuList[MPUNUM] = {&mpu1, &mpu2, &mpu3};


void sendMessage(char *message){
  radio.write(message, sizeof(message));
}


void ReadThing(){
    Wire.beginTransmission(0x69);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x69, 14, true);
  float AcX = Wire.read()<<8|Wire.read();
  float AcY = Wire.read()<<8|Wire.read();
  float AcZ = Wire.read()<<8|Wire.read();
  float Tmp = Wire.read()<<8|Wire.read();
  float GyX = Wire.read()<<8|Wire.read();
  float GyY = Wire.read()<<8|Wire.read();
  float GyZ = Wire.read()<<8|Wire.read();
}
void setUp() {
  Wire.begin();
  Wire.beginTransmission(0x69);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void setup() {
  // put your setup code here, to run once:
  //mpu6050 does not work unless I do a test read before, weird...
  ReadThing();
  setUp();
  delay(500);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  Wire.begin();
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  MPU6050::initialize();
  
  for(int i = 0; i < MPUNUM; i++){
    for(int j = 2; j < MPUNUM + 2; j ++){
      if(mpuList[i]->getPin() == j){
        digitalWrite(j, HIGH);
      }else{
        digitalWrite(j, LOW);
      }
    }
    mpuList[i]->initializeData();
  }
  
}

void loop() {
  // put your main code here, to run repeatedly
  strcpy(message, empty);
   for(int i = 0; i < MPUNUM; i++){
    for(int j = 2; j < MPUNUM + 2; j ++){
      if(mpuList[i]->getPin() == j){
        digitalWrite(j, HIGH);
      }else{
        digitalWrite(j, LOW);
      }
    }
    delay(50);
    mpuList[i]->update();
   // mpuList[i]->print();
    itoa(mpuList[i]->getAngleX(), number, 10);
    strcat(message, number);
    strcat(message, separator);
    itoa(mpuList[i]->getAngleY(), number, 10);
    strcat(message, number);
    strcat(message, separator);
  }
  int value = analogRead(FLEX_PIN);
  value = map(value, minVal, maxVal, 0, 158);
  value -= 10;
  if(value < 0){
    value = 0;
  }
  itoa(value, number, 10);
  strcat(message, number);
  Serial.println(message);
  const char text[] = "Hello World";
  radio.write(&message, sizeof(message));
}
