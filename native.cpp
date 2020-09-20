/*
Copyright (C) 2013 - 2020 GEATEC engineering

This program is free software.
You can use, redistribute and/or modify it, but only under the terms stated in the QQuickLicence.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY, without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the QQuickLicence for details.

The QQuickLicense can be accessed at: http://www.qquick.org/license.html
__________________________________________________________________________


THIS PROGRAM IS FUNDAMENTALLY UNSUITABLE FOR CONTROLLING REAL SYSTEMS !!

__________________________________________________________________________

It is meant for training purposes only.

Removing this header ends your licence.
*/

#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

auto motorShield = Adafruit_MotorShield ();
auto mode = DOUBLE;

// ====== Create decl stepper

auto *declMotor = motorShield.getStepper (48, 2);


void forwardStepDecl () {  
  declMotor->onestep (FORWARD, mode);
}

void backwardStepDecl () {  
  declMotor->onestep (BACKWARD, mode);
}

auto declStepper = AccelStepper (forwardStepDecl, backwardStepDecl);

// ====== Create ra stepper

auto *raMotor = motorShield.getStepper (48, 1);

void forwardStepRa () {  
  raMotor->onestep (BACKWARD, mode);
}

void backwardStepRa () {  
  raMotor->onestep (FORWARD, mode);
}

auto raStepper = AccelStepper (forwardStepRa, backwardStepRa);

// ====== Standard Arduino callbacks

void setup () {
    pinMode (0, INPUT_PULLUP);
    pinMode (1, INPUT_PULLUP);
    pinMode (2, INPUT_PULLUP);
    pinMode (3, INPUT_PULLUP);
    
    pinMode (4, OUTPUT);
    
    motorShield.begin ();
    
    declStepper.setAcceleration (10);
    raStepper.setAcceleration (10);
}

void loop () {
    downButton = !digitalRead (1);
    upButton = !digitalRead (3);
    
    leftButton = !digitalRead (0);
    rightButton = !digitalRead (2);

    cycle ();

    digitalWrite (4, led);
    
    declStepper.setMaxSpeed (declSpeed);
    declStepper.move (declGoalDist);
    declStepper.run ();
    
    raStepper.setMaxSpeed (raSpeed);
    raStepper.move (raGoalDist);
    raStepper.run ();
    
}       
