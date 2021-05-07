#ifndef MPUSENSOR_H
#define MPUSENSOR_H

#include <Wire.h>
#include "Arduino.h"

#define ADDR 0x69

class MPU6050{
    //pin that controls the I2C address of the module
    int AD0Pin;
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    float gForceX;
    float gForceY;
    float igForceX;
    float igForceY;
    float angleX;
    float angleY;
    //Processes raw data to gForce
    void processData();
    //Getting data from module
    void getData();
    //Process gForce into Angle
    void processAngle();

    public:
    //Constructor 
    MPU6050(int AD0Pin);
    //initialize all MPU6050 Modules
    static void initialize();
    //set initial data for inidividual module
    void initializeData();

    //Print method for testing
    void print();
    //Getter for angle
    int getAngleX();
    //Getter for angle
    int getAngleY();
    //Getter for AD0Pin
    int getPin();
    //Update method to be called in loop
    void update();
};



#endif
