// Shows how to run three Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library 
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support 
//   https://github.com/adafruit/AccelStepper

// This tutorial is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepper1 = AFMS.getStepper(48, 1);
Adafruit_StepperMotor *myStepper2 = AFMS.getStepper(48, 2);

auto mode = DOUBLE;

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  myStepper1->onestep(FORWARD, mode);
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, mode);
}
// wrappers for the second motor!
void forwardstep2() {  
  myStepper2->onestep(FORWARD, mode);
}
void backwardstep2() {  
  myStepper2->onestep(BACKWARD, mode);
}

// Now we'll wrap the 2 steppers in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

void setup()
{  
  AFMS.begin(); // Start the shield
  
  stepper1.setMaxSpeed(100.0);
  stepper1.setAcceleration(100.0);
  stepper1.moveTo(100000);
    
  stepper2.setMaxSpeed(100.0);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(100000);
}

void loop()
{
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
      stepper1.moveTo(-stepper1.currentPosition());

    if (stepper2.distanceToGo() == 0)
      stepper2.moveTo(-stepper2.currentPosition());

    stepper1.run();
    stepper2.run();
}
