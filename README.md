# Gesture-Controlled-Robotic-Arm
## Description

3D printer robotic arm. Joints controlled with servo motors. Arm/hand movement detected with MPU6050 accelerometer and flex sensor.

### List of Materials

* 4 MG996R Servo Motors
* 2 SG90 Servo Motors
* 1 PCA9685 Servfo Motor Driver
* 2 NRF24L01 RF Tranceiver Module
* 3 MPU6050 Accelerometers
* 1 ADA1070 Flex Sensor
* 2 Arduino Unos
* 4mm bolt and nut for claw
* 3 NPN transistors

## How it Works

Arm and hand orientation is calculated from the data transmitted from the MPU6050 accelerometers. The Accelerometers are connected to an Arduino through I2C. the AD0 pin of each sensor is pulled to high (3.3V) periodically to select it on the I2C bus. NPN tansistors were used to level shift 5V outputs of the Arduino to 3.3V for the MPU6050. 

MPU6050 can be controlled by using the library create for this project:

```
#include "MPUSensor.h"
```

Initialize an MPU6050 object with the pin number connected to its AD0 pin.

```
MPU6050 mpu1(2);
MPU6050 mpu2(3);
MPU6050 mpu3(4);
```

Data taken from the library functions can then be transmitted to the other Arduino using NRF24L01 RF Tranceivers











