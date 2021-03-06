/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

typedef struct pidValues
{
	//Defines the variables associated with a simple PID controller

	float kP;
	float kI;
	float kD;
	float sumOfErrors;
	float lastError;
	long int lastTime;
	int max;
	int min;

}pidValues;

typedef struct straightDrive
{

	float distance;
	int maxPower;
	float brakingDistance;
	float deadband;
	int rampTime;

	int finished;
	float leftToMove, rightToMove;
	float timeDif;
	int leftSpeed;
	int rightSpeed;
	int leftMoved, rightMoved;

	//Defines the start time
	long int startTime;

	int isFirstTime;

}straightDrive;

typedef struct turnDrive
{

	float leftDistance;
	float rightDistance;
	int maxPower;
	float brakingDistance;
	float deadband;
	int rampTime;

	//Defines variables for the function
	int finished;
	float leftToMove, rightToMove;
	float timeDif;
	int leftSpeed;
	int rightSpeed;
	int leftMoved, rightMoved;

	//Defines the start time for the function
	long int startTime;

	int isFirstTime;

}turnDrive;

typedef struct drive
{
	int leftMotor;
	int rightMotor;
	int leftIME;
	int rightIME;
}drive;

int pidController(pidValues *myPid, float setPoint, float proccessVariable)
{
	//Runs a single instance of a PID controller with values from a pidValues
	//struct and returns the output for the PID loop

	//Defines variables for the function, error is SP - PV
	float p;
	float i;
	float d;
	float output;
	float error = setPoint - proccessVariable;

	//Prints the error
	printf("Error: %f\n", error);

	//PROPORTIONAL
	p = error * (*myPid).kP;

	//INTEGRAL
	//Updates the sumOfErrors variable from the pidValues struct
	(*myPid).sumOfErrors = (*myPid).sumOfErrors + (double) ((millis() - (*myPid).lastTime) * error);
	//Prints the sum of errors
	printf("Sum of Errors: %f", (*myPid).sumOfErrors);
	//Calculates the I value
	i = (*myPid).sumOfErrors * (*myPid).kI;

	//DERIVATIVE
	//kD * change in error over change in time
	d = (*myPid).kD * ((*myPid).lastError / ((double) (millis() - (*myPid).lastTime)));

	//Calculates the output and updates the last error
	output = p + i + d;
	(*myPid).lastError = error;

	//Limits the output between min and max values
	if(output > (*myPid).max)
	{
		output = (*myPid).max;
	}
	else if(output < (*myPid).min)
	{
		output = (*myPid).min;
	}

	//Returns the controller's output
	return output;
}

void moveStraightForIME(drive myDrive, straightDrive *values)
{
	puts("Straight");

	if((*values).isFirstTime == 1)
	{
		//Resets IMEs and motors
		imeReset(myDrive.leftIME);
		imeReset(myDrive.rightIME);
		motorSet(myDrive.leftMotor, 0);
		motorSet(myDrive.rightMotor, 0);

		(*values).startTime = millis();

		(*values).leftToMove = (*values).distance;
		(*values).rightToMove = (*values).distance;

		(*values).isFirstTime = 0;
	}

	//Updates time and IMEs
	(*values).timeDif = millis() - (*values).startTime;
	imeGet(myDrive.leftIME, &((*values).leftMoved));
	imeGet(myDrive.rightIME, &((*values).rightMoved));

	//Inverts the moved variables if the IME is inverted
	(*values).leftMoved = (LEFT_IME_INVERTED) ? (*values).leftMoved * -1 : (*values).leftMoved;
	(*values).rightMoved = (RIGHT_IME_INVERTED) ? (*values).rightMoved * -1 : (*values).rightMoved;

	/*if(distance < 0)
	{
		leftMoved *= -1;
		rightMoved *= -1;
	}*/

	//CASE 1
	//If the motors are accelerating and neither of the motors are in braking distance:
	if((*values).timeDif < (*values).rampTime && (*values).leftMoved < (*values).distance - (*values).brakingDistance && (*values).rightMoved < (*values).distance - (*values).brakingDistance)
	{
		puts("1\n");

		//Speed scales up linearly to give max power at the end of the ramp up
		(*values).leftSpeed = (int) (((float) ((*values).timeDif) / (float) ((*values).rampTime)) * (*values).maxPower);
		(*values).rightSpeed = (int) (((float) ((*values).timeDif) / (float) ((*values).rampTime)) * (*values).maxPower);
	}

	//CASE 2
	//If either of the motors is within the braking distance:
	else if((*values).leftMoved > (*values).distance - (*values).brakingDistance || (*values).rightMoved > (*values).distance - (*values).brakingDistance)
	{
		puts("2\n");

		//Slows down the motor linearly to go from max power to zero at the
		//end of the distance
		(*values).leftSpeed = (int) ((((*values).distance - (*values).leftMoved) / (*values).brakingDistance) * (*values).maxPower);
		(*values).rightSpeed = (int) ((((*values).distance - (*values).rightMoved) / (*values).brakingDistance) * (*values).maxPower);
	}

	//CASE 3
	//The wheels are not ramping up or down:
	else
	{
		puts("3\n");
		//Drive the wheels at max power
		(*values).leftSpeed = (*values).maxPower;
		(*values).rightSpeed = (*values).maxPower;
	}

	//CASE 4
	//Limits leftSpeed in the positive direction
	if((*values).leftSpeed > (*values).maxPower)
	{
		puts("4\n");
		(*values).leftSpeed = (*values).maxPower;
	}
	//CASE 4A
	//Limits leftSpeed in the negative direction
	else if((*values).leftSpeed * -1 > (*values).maxPower)
	{
		puts("4A\n");
		(*values).leftSpeed = (*values).maxPower * -1;
	}
	//CASE 5
	//Limits rightSpeed in the positive direction
	if((*values).rightSpeed > (*values).maxPower)
	{
		puts("5\n");
		(*values).rightSpeed = (*values).maxPower;
	}
	//CASE 5A
	//Limits rightSpeed in the negative direction
	else if((*values).rightSpeed * -1 > (*values).maxPower)
	{
		puts("5A\n");
		(*values).rightSpeed = (*values).maxPower * -1;
	}

	//CASE 7
	//If both wheels are within the deadband, stop them and finish the function
	if(absoluteValue((*values).distance - (*values).leftMoved) < (*values).deadband && absoluteValue((*values).distance - (*values).rightMoved) < (*values).deadband)
	{
		puts("7\n");
		(*values).finished = 1;
		(*values).leftSpeed = 0;
		(*values).rightSpeed = 0;
	}
	//CASE 8
	//If the deadband has not been reached, print the moved and speed (*values) and continue
	else
	{
		puts("8\n");
		printf("Left Moved: %d\n", (*values).leftMoved);
		printf("Right Moved: %d\n", (*values).rightMoved);
		printf("Left Speed: %d\n", (*values).leftSpeed);
		printf("Right Speed: %d\n\n", (*values).rightSpeed);
	}

	//If the motors are inverted, invert the speeds
	(*values).leftSpeed = (LEFT_MOTOR_INVERTED) ? (*values).leftSpeed * -1 : (*values).leftSpeed;
	(*values).rightSpeed = (RIGHT_MOTOR_INVERTED) ? (*values).rightSpeed * -1 : (*values).rightSpeed;

	//Set the motors
	motorSet(myDrive.leftMotor, (*values).leftSpeed);
	motorSet(myDrive.rightMotor, (*values).rightSpeed);
}

void turnForIME(drive myDrive, turnDrive *values)
{
	puts("Turn");

	if((*values).isFirstTime == 1)
	{
		//Defines variables for the function
		(*values).leftToMove = (*values).leftDistance;
		(*values).rightToMove = (*values).rightDistance;

		//Defines the start time for the function
		(*values).startTime = millis();

		//Resets IMEs and motors
		imeReset(myDrive.leftIME);
		imeReset(myDrive.rightIME);
		motorSet(myDrive.leftMotor, 0);
		motorSet(myDrive.rightMotor, 0);

		(*values).isFirstTime = 0;
	}



	//Gets the elapsed time and IME (*values)
	(*values).timeDif = millis() - (*values).startTime;
	imeGet(myDrive.leftIME, &((*values).leftMoved));
	imeGet(myDrive.rightIME, &((*values).rightMoved));

	//If the IMEs are inverted, invert their moved variables
	(*values).leftMoved = (LEFT_IME_INVERTED) ? (*values).leftMoved * -1 : (*values).leftMoved;
	(*values).rightMoved = (RIGHT_IME_INVERTED) ? (*values).rightMoved * -1 : (*values).rightMoved;

	//If a wheel is moving backwards, invert its moved variable
	if((*values).leftDistance < 0)
	{
		(*values).leftMoved *= -1;
	}

	if((*values).rightDistance < 0)
	{
		(*values).rightMoved *= -1;
	}

	//CASE 1
	//If the left motor is accelerating and is not within the braking distance:
	if((*values).timeDif < (*values).rampTime && (*values).leftMoved < (*values).leftDistance - (*values).brakingDistance)
	{
		puts("1\n");
		(*values).leftSpeed = (int) (((float) ((*values).timeDif) / (float) ((*values).rampTime)) * (*values).maxPower);
	}
	//CASE 2
	//If the left motor is within the braking distance, slow it down
	else if((*values).leftMoved > (*values).leftDistance - (*values).brakingDistance)
	{
		puts("2\n");
		(*values).leftSpeed = (int) ((((*values).leftDistance - (*values).leftMoved) / (*values).brakingDistance) * (*values).maxPower);
	}
	//CASE 3
	//If the left motor is not slowing down or speeding up, run it at max power
	else
	{
		puts("3\n");
		(*values).leftSpeed = (*values).maxPower;
	}
	//END OF LEFT

	//CASE 1
	//If the right motor is accelerating and is not within the braking distance:
	if((*values).rightMoved < (*values).rightDistance - (*values).brakingDistance)
	{
		puts("1\n");
		(*values).rightSpeed = (int) (((float) ((*values).timeDif) / (float) ((*values).rampTime)) * (*values).maxPower);
	}
	//CASE 2
	//If the right motor is within the braking distance, slow it down
	else if((*values).rightMoved > (*values).rightDistance - (*values).brakingDistance)
	{
		puts("2\n");
		(*values).rightSpeed = (int) ((((*values).rightDistance - (*values).rightMoved) / (*values).brakingDistance) * (*values).maxPower);
	}
	//CASE 3
	//If the right motor is not slowing down or speeding up, run it at max power
	else
	{
		puts("3\n");
		(*values).rightSpeed = (*values).maxPower;
	}
	//END OF RIGHT

	//CASE 4
	//Limits the left speed in the positive direction
	if((*values).leftSpeed > (*values).maxPower)
	{
		puts("4\n");
		(*values).leftSpeed = (*values).maxPower;
	}
	//CASE 4A
	//Limits the left speed in the negative direction
	else if((*values).leftSpeed * -1 > (*values).maxPower)
	{
		puts("4A\n");
		(*values).leftSpeed = (*values).maxPower * -1;
	}

	//CASE 5
	//Limits the right speed in the positive direction
	if((*values).rightSpeed > (*values).maxPower)
	{
		puts("5\n");
		(*values).rightSpeed = (*values).maxPower;
	}
	//CASE 5A
	//Limits the right speed in the negative direction
	else if((*values).rightSpeed * -1 > (*values).maxPower)
	{
		puts("5A\n");
		(*values).rightSpeed = (*values).maxPower * -1;
	}

	//CASE 7
	//If both motors are within the deadband, stop the function
	if((*values).leftDistance - (*values).leftMoved < (*values).deadband && (*values).rightDistance - (*values).rightMoved < (*values).deadband)
	{
		puts("7\n");
		(*values).finished = 1;
		(*values).leftSpeed = 0;
		(*values).rightSpeed = 0;
	}
	//CASE 8
	//If the deadband has not been reached, print the moved and speed (*values) and
	//continue
	else
	{
		puts("8\n");
		printf("Left Moved: %d\n", (*values).leftMoved);
		printf("Right Moved: %d\n", (*values).rightMoved);
		printf("Left Speed: %d\n", (*values).leftSpeed);
		printf("Right Speed: %d\n\n", (*values).rightSpeed);
	}

	//If the motors are inverted, invert the speed
	(*values).leftSpeed = (LEFT_MOTOR_INVERTED) ? (*values).leftSpeed * -1 : (*values).leftSpeed;
	(*values).rightSpeed = (RIGHT_MOTOR_INVERTED) ? (*values).rightSpeed * -1 : (*values).rightSpeed;

	//Set the motors
	motorSet(myDrive.leftMotor, (*values).leftSpeed);
	motorSet(myDrive.rightMotor, (*values).rightSpeed);
}

void operatorControl()
{
	drive myDrive = {LEFT_MOTOR, RIGHT_MOTOR, LEFT_IME, RIGHT_IME};
	straightDrive step1 = {-2000, 100, 200, 25, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	turnDrive step2 = {3000, 0, 127, 200, 25, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};


	puts("First Move");

	while(step1.finished == 0)
	{
		moveStraightForIME(myDrive, &step1);
		delay(20);
	}

	puts("Second Move");

	encoderReset(liftEnc);
	pidValues liftPid = {1, 0.000, 0.0, 0, 0, millis(), 127, -127};

	do
	{
		puts("Iteration");

		turnForIME(myDrive, &step2);
		motorSet(RIGHT_MOTOR, pidController(&liftPid, 50.0, encoderGet(liftEnc)));
		delay(20);
	}while(step2.finished == 0 && liftPid.lastError > 10);

}
