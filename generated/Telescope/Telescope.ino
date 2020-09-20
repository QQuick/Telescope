// ======================== BEGIN OF GENERATED CODE ========================



// ====== BEGIN OF License COMMENT BLOCK, INCLUDE IN ANY COPY OF THIS GENERATED CODE AND DO NOT REMOVE ======
//
// I M P O R T A N T   S A F E T Y   N O T I C E
//
// THIS CODE IS INTENDED SOLELY FOR EDUCATIONAL PURPOSES AND IS FUNDAMENTALLY UNSUITABLE FOR CONTROLLING REAL SYSTEMS.
// IT IS STRICKTLY PROHIBITED TO USE THIS GENERATED CODE IN ANY SITUATION THAT ENTAILS RISK OF DAMAGE OR INJURIES.
//
// USE OF THIS CODE IS GOVERNED BY THE QQUICK LICENSE (WWW.QQUICK.ORG/LICENSE).
// YOUR LICENSE TO USE THIS GENERATED CODE AUTOMATICALLY ENDS IF YOU REMOVE OR LEAVE OUT THIS LICENSE COMMENT BLOCK OR THE CODE THAT GENERATED IT. 
//
// ====== END OF License COMMENT BLOCK, INCLUDE IN COPY OF THIS GENERATED CODE AND DO NOT REMOVE ======



// Generator: SimPyLC 3.7.04
// Generated: 2020-09-20 02:13:29.846575
// Target platform: Arduino



// ____________ General includes ____________

#include <math.h>



// ____________ Arduino macros ____________

#define getNowExact() micros ()
#define getNowInexact() millis ()



// ____________ General macros ____________

// Circuit operations

#define mark4(marker, trueValue, condition, falseValue) marker = (condition) ? (trueValue) : (falseValue)
#define mark3(marker, trueValue, condition) if (condition) marker = (trueValue)
#define mark2(marker, trueValue) marker = (trueValue)
#define mark1(marker) marker = True

#define trigger2(oneshot, condition) oneshot.value = oneshot.memo; oneshot.memo = (condition); oneshot.value = !oneshot.value and oneshot.memo
#define trigger1(oneshot) oneshot.value = !oneshot.memo; oneshot.memo = True
#define spiked1(oneshot) (oneshot.value)

#define latch2(latch, condition) if (condition) latch = True
#define latch1 (latch) latch = True

#define unlatch2(latch, condition) if (condition) latch = False
#define unlatch1 (latch) latch = False

#define set4(register, trueValue, condition, falseValue) register = (condition) ? (trueValue) : (falseValue)
#define set3(register, trueValue, condition) if (condition) register = (trueValue)
#define set2(register, trueValue) register = (trueValue)
#define set1(register) register = 1

#define reset2(timer, condition) if (condition) {timer.exact = nowExact; timer.inexact = nowInexact;}
#define reset1(timer) timer.exact = nowExact; timer.inexact = nowInexact
#define elapsed1(timer) ((nowInexact - timer.inexact) < 3.6e6 ? 1e-6 * (nowExact - timer.exact) : 1e-3 * (nowInexact - timer.inexact))

// Support operations

#define update()\
    thenExact = nowExact; nowExact = getNowExact(); period = 1e-6 * (nowExact - thenExact);\
    nowInexact = getNowInexact();\
    first = False;

// Types

#define False 0
#define True 1
#define Bool bool
#define UInt unsigned long
#define Int long
#define Float double
#define Marker int
#define Oneshot struct {int value; int memo;}
#define Latch int
#define Register double
#define Timer struct {unsigned long exact; unsigned long inexact;}

// Math operations

#define abs1(value) fabs (value)
#define max2(value0, value1) fmax (value0, value1)
#define min2(value0, value1) fmin (value0, value1)
#define limit3(value, aLimit0, aLimit1) min (max (value, aLimit0), aLimit1)  
#define limit2(value, aLimit) limit3 (value, -aLimit, aLimit)
#define digit2(value, index) getDigit (int (value), index)

// ____________ General functions ____________

int getDigit (int value, int index) {
    return (index == 0) ? value % 10 : getDigit (value / 10, --index);
}

// ____________ General variables ____________

UInt nowExact = 0;
UInt thenExact = 0;
UInt nowInexact = 0;
Float period = 1;
Bool first = True;



// ____________ PLC variables ____________

// ______ Module: control ______

// Page: Telescope control

// Group: Inputs

Marker downButton = False;
Marker upButton = False;
Register declActualPos = 0;
Marker leftButton = False;
Marker rightButton = False;
Register raActualPos = 0;

// Group: Outputs

Marker led = False;
Register declSpeed = 0;
Register declTargetPos = 0;
Register raSpeed = 0;
Register raTargetPos = 0;

// Group: System


// Group: Internals

Marker ledToggleCombi = False;
Oneshot ledToggle = {False, False};
Marker hemiToggleCombi = False;
Oneshot hemiToggle = {False, False};
Marker hemi = False;
Marker declDownCombi = False;
Marker declUpCombi = False;
Marker raLeftCombi = False;
Marker raRightCombi = False;

// Group: Constants

Register finity = 100000;
Register minSpeed = 0.1;
Register maxSpeed = 100;



// ____________ PLC cycle ____________

void cycle () {

	// ______ Module: control ______

	// ___ Sweep ___
	mark2 (ledToggleCombi, (downButton && upButton));
	mark2 (hemiToggleCombi, (leftButton && rightButton));
	mark2 (declDownCombi, (downButton && (!upButton)));
	mark2 (declUpCombi, ((!downButton) && upButton));
	mark2 (raLeftCombi, (leftButton && (!rightButton)));
	mark2 (raRightCombi, ((!leftButton) && rightButton));
	trigger2 (ledToggle, ledToggleCombi);
	mark3 (led, (!led), spiked1 (ledToggle));
	trigger2 (hemiToggle, hemiToggleCombi);
	mark3 (hemi, (!hemi), spiked1 (hemiToggle));
	set4 (declSpeed, maxSpeed, (declDownCombi || declUpCombi), 0);
	set4 (declTargetPos, (-finity), declDownCombi, finity);
	set3 (declTargetPos, declActualPos, (declSpeed < minSpeed));
	set4 (raSpeed, maxSpeed, (raLeftCombi || raRightCombi), 0);
	set4 (raTargetPos, (-finity), raLeftCombi, finity);
	set3 (raTargetPos, raActualPos, (raSpeed < minSpeed));

    // ______ System ______

    update ();
}



// ======================== END OF GENERATED CODE ========================

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

auto *declMotor = motorShield.getStepper (48, 1);

void forwardStepDecl () {  
  declMotor->onestep (FORWARD, mode);
}

void backwardStepDecl () {  
  declMotor->onestep (BACKWARD, mode);
}

auto declStepper = AccelStepper (forwardStepDecl, backwardStepDecl);

// ====== Create ra stepper

auto *raMotor = motorShield.getStepper (48, 2);

void forwardStepRa () {  
  raMotor->onestep (FORWARD, mode);
}

void backwardStepRa () {  
  raMotor->onestep (BACKWARD, mode);
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
    declStepper.moveTo (-declTargetPos);
    declStepper.run ();
    
    raStepper.setMaxSpeed (raSpeed);
    raStepper.moveTo (-raTargetPos);
    raStepper.run ();
    
}       
