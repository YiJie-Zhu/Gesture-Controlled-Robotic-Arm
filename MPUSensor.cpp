#include "MPUSensor.h"
#include "Arduino.h"

MPU6050::MPU6050(int AD0Pin): AD0Pin{AD0Pin}{

}

void MPU6050::initialize(){

    Wire.beginTransmission(0x69); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
    Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
    Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
    Wire.endTransmission();  
    Wire.beginTransmission(0x69); //I2C address of the MPU
    Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
    Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
    Wire.endTransmission(); 
    Wire.beginTransmission(0x69); //I2C address of the MPU
    Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
    Wire.write(0b00000000); //Setting the accel to +/- 2g
    Wire.endTransmission(); 
}

void MPU6050::processData(){
    gForceX = accelX / 16384.0;
    gForceY = accelY / 16384.0; 
}

void MPU6050::getData(){
    Wire.beginTransmission(0x69); //I2C address of the MPU
    Wire.write(0x3B); //Starting register for Accel Readings
    Wire.endTransmission();
    Wire.requestFrom(0x69,6); //Request Accel Registers (3B - 40)
    while(Wire.available() < 6);
    accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
    accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
    accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
}

void MPU6050::initializeData(){
    MPU6050::getData();
    MPU6050::processData();
    igForceX = gForceX;
    igForceY = gForceY;
}

void MPU6050::processAngle(){
    angleX = map((int)((gForceX - igForceX) * 100), -100, 100, 0, 180);
    angleY = map((int)((gForceY - igForceY) * 100), -100, 100, 0, 180);
    if(angleX > 180){
        angleX = 180;
    }
    else if(angleX < 0){
        angleX = 0;
    }
    if(angleY > 180){
        angleY = 180;
    }
     else if (angleY < 0){
     angleY = 0;
    }
}

void MPU6050::update(){
    MPU6050::getData();
    MPU6050::processData();
    MPU6050::processAngle();
}

void MPU6050::print() {
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(angleX);  
  Serial.print(" Y=");
  Serial.print(angleY);
}

int MPU6050::getAngleX(){
    return (int)angleX;
}

int MPU6050::getAngleY(){
    return (int)angleY;
}

int MPU6050::getPin(){
  return AD0Pin;
}
