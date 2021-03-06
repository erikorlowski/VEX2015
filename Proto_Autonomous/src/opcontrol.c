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

}straightDrive;

typedef struct turnDrive
{

	float leftDistance;
	float rightDistance;
	int maxPower;
	float brakingDistance;
	float deadband;
	int rampTime;

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

void moveStraightForIME(drive myDrive, straightDrive values)
{
	//Defines variables for the function
	int finished = 0;
	float leftToMove = values.distance, rightToMove = values.distance;
	float timeDif;
	int leftSpeed;
	int rightSpeed;
	int leftMoved, rightMoved;

	//Defines the start time
	long int startTime = millis();

	//Resets IMEs and motors
	imeReset(myDrive.leftIME);
	imeReset(myDrive.rightIME);
	motorSet(myDrive.leftMotor, 0);
	motorSet(myDrive.rightMotor, 0);

	while(!finished)
	{
		//Updates time and IMEs
		timeDif = millis() - startTime;
		imeGet(myDrive.leftIME, &leftMoved);
		imeGet(myDrive.rightIME, &rightMoved);

		//Inverts the moved variables if the IME is inverted
		leftMoved = (LEFT_IME_INVERTED) ? leftMoved * -1 : leftMoved;
		rightMoved = (RIGHT_IME_INVERTED) ? rightMoved * -1 : rightMoved;

		/*if(distance < 0)
		{
			leftMoved *= -1;
			rightMoved *= -1;
		}*/

		//CASE 1
		//If the motors are accelerating and neither of the motors are in braking distance:
		if(timeDif < values.rampTime && leftMoved < values.distance - values.brakingDistance && rightMoved < values.distance - values.brakingDistance)
		{
			puts("1\n");

			//Speed scales up linearly to give max power at the end of the ramp up
			leftSpeed = (int) (((float) (timeDif) / (float) (values.rampTime)) * values.maxPower);
			rightSpeed = (int) (((float) (timeDif) / (float) (values.rampTime)) * values.maxPower);
		}

		//CASE 2
		//If either of the motors is within the braking distance:
		else if(leftMoved > values.distance - values.brakingDistance || rightMoved > values.distance - values.brakingDistance)
		{
			puts("2\n");

			//Slows down the motor linearly to go from max power to zero at the
			//end of the distance
			leftSpeed = (int) (((values.distance - leftMoved) / values.brakingDistance) * values.maxPower);
			rightSpeed = (int) (((values.distance - rightMoved) / values.brakingDistance) * values.maxPower);
		}

		//CASE 3
		//The wheels are not ramping up or down:
		else
		{
			puts("3\n");

			//Drive the wheels at max power
			leftSpeed = values.maxPower;
			rightSpeed = values.maxPower;
		}

		//CASE 4
		//Limits leftSpeed in the positive direction
		if(leftSpeed > values.maxPower)
		{
			puts("4\n");
			leftSpeed = values.maxPower;
		}
		//CASE 4A
		//Limits leftSpeed in the negative direction
		else if(leftSpeed * -1 > values.maxPower)
		{
			puts("4A\n");
			leftSpeed = values.maxPower * -1;
		}

		//CASE 5
		//Limits rightSpeed in the positive direction
		if(rightSpeed > values.maxPower)
		{
			puts("5\n");
			rightSpeed = values.maxPower;
		}
		//CASE 5A
		//Limits rightSpeed in the negative direction
		else if(rightSpeed * -1 > values.maxPower)
		{
			puts("5A\n");
			rightSpeed = values.maxPower * -1;
		}

		//CASE 7
		//If both wheels are within the deadband, stop them and finish the function
		if(abs(values.distance - leftMoved) < values.deadband && abs(values.distance - rightMoved) < values.deadband)
		{
			puts("7\n");
			finished = 1;
			leftSpeed = 0;
			rightSpeed = 0;
		}
		//CASE 8
		//If the deadband has not been reached, print the moved and speed values and continue
		else
		{
			puts("8\n");
			printf("Left Moved: %d\n", leftMoved);
			printf("Right Moved: %d\n", rightMoved);
			printf("Left Speed: %d\n", leftSpeed);
			printf("Right Speed: %d\n\n", rightSpeed);
		}

		//If the motors are inverted, invert the speeds
		leftSpeed = (LEFT_MOTOR_INVERTED) ? leftSpeed * -1 : leftSpeed;
		rightSpeed = (RIGHT_MOTOR_INVERTED) ? rightSpeed * -1 : rightSpeed;

		//Set the motors
		motorSet(myDrive.leftMotor, leftSpeed);
		motorSet(myDrive.rightMotor, rightSpeed);

		//Delay to avoid hogging the CPU
		delay(20);
	}
}

void turnForIME(drive myDrive, turnDrive values)
{
	//Defines variables for the function
	int finished = 0;
	float leftToMove = values.leftDistance, rightToMove = values.rightDistance;
	float timeDif;
	int leftSpeed;
	int rightSpeed;
	int leftMoved, rightMoved;

	//Defines the start time for the function
	long int startTime = millis();

	//Resets IMEs and motors
	imeReset(myDrive.leftIME);
	imeReset(myDrive.rightIME);
	motorSet(myDrive.leftMotor, 0);
	motorSet(myDrive.rightMotor, 0);

	while(!finished)
	{
		//Gets the elapsed time and IME values
		timeDif = millis() - startTime;
		imeGet(myDrive.leftIME, &leftMoved);
		imeGet(myDrive.rightIME, &rightMoved);

		//If the IMEs are inverted, invert their moved variables
		leftMoved = (LEFT_IME_INVERTED) ? leftMoved * -1 : leftMoved;
		rightMoved = (RIGHT_IME_INVERTED) ? rightMoved * -1 : rightMoved;

		//If a wheel is moving backwards, invert its moved variable
		if(values.leftDistance < 0)
		{
			leftMoved *= -1;
		}

		if(values.rightDistance < 0)
		{
			rightMoved *= -1;
		}

		//CASE 1
		//If the left motor is accelerating and is not within the braking distance:
		if(timeDif < values.rampTime && leftMoved < values.leftDistance - values.brakingDistance)
		{
			puts("1\n");
			leftSpeed = (int) (((float) (timeDif) / (float) (values.rampTime)) * values.maxPower);
		}
		//CASE 2
		//If the left motor is within the braking distance, slow it down
		else if(leftMoved > values.leftDistance - values.brakingDistance)
		{
			puts("2\n");
			leftSpeed = (int) (((values.leftDistance - leftMoved) / values.brakingDistance) * values.maxPower);
		}
		//CASE 3
		//If the left motor is not slowing down or speeding up, run it at max power
		else
		{
			puts("3\n");
			leftSpeed = values.maxPower;
		}
		//END OF LEFT

		//CASE 1
		//If the right motor is accelerating and is not within the braking distance:
		if(rightMoved < values.rightDistance - values.brakingDistance)
		{
			puts("1\n");
			rightSpeed = (int) (((float) (timeDif) / (float) (values.rampTime)) * values.maxPower);
		}
		//CASE 2
		//If the right motor is within the braking distance, slow it down
		else if(rightMoved > values.rightDistance - values.brakingDistance)
		{
			puts("2\n");
			rightSpeed = (int) (((values.rightDistance - rightMoved) / values.brakingDistance) * values.maxPower);
		}
		//CASE 3
		//If the right motor is not slowing down or speeding up, run it at max power
		else
		{
			puts("3\n");
			rightSpeed = values.maxPower;
		}
		//END OF RIGHT

		//CASE 4
		//Limits the left speed in the positive direction
		if(leftSpeed > values.maxPower)
		{
			puts("4\n");
			leftSpeed = values.maxPower;
		}
		//CASE 4A
		//Limits the left speed in the negative direction
		else if(leftSpeed * -1 > values.maxPower)
		{
			puts("4A\n");
			leftSpeed = values.maxPower * -1;
		}

		//CASE 5
		//Limits the right speed in the positive direction
		if(rightSpeed > values.maxPower)
		{
			puts("5\n");
			rightSpeed = values.maxPower;
		}
		//CASE 5A
		//Limits the right speed in the negative direction
		else if(rightSpeed * -1 > values.maxPower)
		{
			puts("5A\n");
			rightSpeed = values.maxPower * -1;
		}

		//CASE 7
		//If both motors are within the deadband, stop the function
		if(values.leftDistance - leftMoved < values.deadband && values.rightDistance - rightMoved < values.deadband)
		{
			puts("7\n");
			finished = 1;
			leftSpeed = 0;
			rightSpeed = 0;
		}
		//CASE 8
		//If the deadband has not been reached, print the moved and speed values and
		//continue
		else
		{
			puts("8\n");
			printf("Left Moved: %d\n", leftMoved);
			printf("Right Moved: %d\n", rightMoved);
			printf("Left Speed: %d\n", leftSpeed);
			printf("Right Speed: %d\n\n", rightSpeed);
		}

		//If the motors are inverted, invert the speed
		leftSpeed = (LEFT_MOTOR_INVERTED) ? leftSpeed * -1 : leftSpeed;
		rightSpeed = (RIGHT_MOTOR_INVERTED) ? rightSpeed * -1 : rightSpeed;

		//Set the motors
		motorSet(myDrive.leftMotor, leftSpeed);
		motorSet(myDrive.rightMotor, rightSpeed);

		//Avoid hogging the CPU
		delay(20);
	}
}

void operatorControl()
{
	drive myDrive = {LEFT_MOTOR, RIGHT_MOTOR, LEFT_IME, RIGHT_IME};
	straightDrive step1 = {-2000, 100, 200, 25, 500};
	turnDrive step2 = {1000, -1000, 127, 200, 25, 500};
	pidValues liftPid = {1, 0.000, 0.0, 0, 0, millis(), 127, -127};

	moveStraightForIME(myDrive, step1);

	encoderReset(liftEnc);

	do
	{
		motorSet(LIFT_MOTOR, pidController(&liftPid, 50.0, encoderGet(liftEnc)));
		delay(20);

	}while(liftPid.lastError > 10);

	motorSet(LEFT_MOTOR, 0);

	turnForIME(myDrive, step2);

	puts("Done");

	motorSet(LEFT_MOTOR, 0);
	motorSet(RIGHT_MOTOR, 0);
}
