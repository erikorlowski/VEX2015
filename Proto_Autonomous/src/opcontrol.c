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

	float kP;
	float kI;
	float kD;
	float sumOfErrors;
	float lastError;
	long int lastTime;
	int max;
	int min;

}pidValues;

int pidController(pidValues *myPid, float setPoint, float proccessVariable)
{
	float p;
	float i;
	float d;
	float output;
	float error = setPoint - proccessVariable;

	printf("Error: %f\n", error);

	p = error * (*myPid).kP;
	(*myPid).sumOfErrors = (*myPid).sumOfErrors + (double) ((millis() - (*myPid).lastTime) * error);
	printf("Sum of Errors: %f", (*myPid).sumOfErrors);
	i = (*myPid).sumOfErrors * (*myPid).kI;
	d = (*myPid).kD * ((*myPid).lastError / ((double) (millis() - (*myPid).lastTime)));

	output = p + i + d;
	(*myPid).lastError = error;

	if(output > (*myPid).max)
	{
		output = (*myPid).max;
	}
	else if(output < (*myPid).min)
	{
		output = (*myPid).min;
	}

	if(output < 5.0 && output > -5.0)
	{
		//puts("Deadband");
		//output = 0;
	}

	return output;
}

void moveStraightForIME(int leftMotor, int rightMotor, int leftIME, int rightIME, float distance, int maxPower)
{
	int finished = 0;
	float leftToMove = distance, rightToMove = distance;
	float timeDif;
	int leftSpeed;
	int rightSpeed;
	float brakingDistance = 200;
	int leftMoved, rightMoved;
	float deadband = 25.0;
	int rampTime = 500;
	long int startTime = millis();

	imeReset(leftIME);
	imeReset(rightIME);
	motorSet(leftMotor, 0);
	motorSet(rightMotor, 0);

	while(!finished)
	{
		timeDif = millis() - startTime;
		imeGet(leftIME, &leftMoved);
		imeGet(rightIME, &rightMoved);

		leftMoved = (LEFT_IME_INVERTED) ? leftMoved * -1 : leftMoved;
		rightMoved = (RIGHT_IME_INVERTED) ? rightMoved * -1 : rightMoved;

		/*if(distance < 0)
		{
			leftMoved *= -1;
			rightMoved *= -1;
		}*/

		if(timeDif < rampTime && leftMoved < distance - brakingDistance && rightMoved < distance - brakingDistance)
		{
			puts("1\n");
			leftSpeed = (int) (((float) (timeDif) / (float) (rampTime)) * maxPower);
			rightSpeed = (int) (((float) (timeDif) / (float) (rampTime)) * maxPower);
		}
		else if(leftMoved > distance - brakingDistance || rightMoved > distance - brakingDistance)
		{
			puts("2\n");
			leftSpeed = (int) (((distance - leftMoved) / brakingDistance) * maxPower);
			rightSpeed = (int) (((distance - rightMoved) / brakingDistance) * maxPower);
		}
		else
		{
			puts("3\n");
			leftSpeed = maxPower;
			rightSpeed = maxPower;
		}

		if(leftSpeed > maxPower)
		{
			puts("4\n");
			leftSpeed = maxPower;
		}
		else if(leftSpeed * -1 > maxPower)
		{
			puts("4A\n");
			leftSpeed = maxPower * -1;
		}

		if(rightSpeed > maxPower)
		{
			puts("5\n");
			rightSpeed = maxPower;
		}
		else if(rightSpeed * -1 > maxPower)
		{
			puts("5A\n");
			rightSpeed = maxPower * -1;
		}

		if(abs(distance - leftMoved) < deadband && abs(distance - rightMoved) < deadband)
		{
			puts("7\n");
			finished = 1;
			leftSpeed = 0;
			rightSpeed = 0;
		}
		else
		{
			puts("8\n");
			printf("Left Moved: %d\n", leftMoved);
			printf("Right Moved: %d\n", rightMoved);
			printf("Left Speed: %d\n", leftSpeed);
			printf("Right Speed: %d\n\n", rightSpeed);
		}

		leftSpeed = (LEFT_MOTOR_INVERTED) ? leftSpeed * -1 : leftSpeed;
		rightSpeed = (RIGHT_MOTOR_INVERTED) ? rightSpeed * -1 : rightSpeed;

		motorSet(leftMotor, leftSpeed);
		motorSet(rightMotor, rightSpeed);

		delay(20);
	}
}

void turnForIME(int leftMotor, int rightMotor, int leftIME, int rightIME, float leftDistance, float rightDistance, int maxPower)
{
	int finished = 0;
	float leftToMove = leftDistance, rightToMove = rightDistance;
	float timeDif;
	int leftSpeed;
	int rightSpeed;
	float brakingDistance = 200;
	int leftMoved, rightMoved;
	float deadband = 25.0;
	int rampTime = 500;
	long int startTime = millis();

	imeReset(leftIME);
	imeReset(rightIME);
	motorSet(leftMotor, 0);
	motorSet(rightMotor, 0);

	while(!finished)
	{
		timeDif = millis() - startTime;
		imeGet(leftIME, &leftMoved);
		imeGet(rightIME, &rightMoved);

		leftMoved = (LEFT_IME_INVERTED) ? leftMoved * -1 : leftMoved;
		rightMoved = (RIGHT_IME_INVERTED) ? rightMoved * -1 : rightMoved;

		if(leftDistance < 0)
		{
			leftMoved *= -1;
		}

		if(rightDistance < 0)
		{
			rightMoved *= -1;
		}


		if(timeDif < rampTime && leftMoved < leftDistance - brakingDistance)
		{
			puts("1\n");
			leftSpeed = (int) (((float) (timeDif) / (float) (rampTime)) * maxPower);
		}
		else if(leftMoved > leftDistance - brakingDistance)
		{
			puts("2\n");
			leftSpeed = (int) (((leftDistance - leftMoved) / brakingDistance) * maxPower);
		}
		else
		{
			puts("3\n");
			leftSpeed = maxPower;
		}
		//END OF LEFT

		if(rightMoved < rightDistance - brakingDistance)
		{
			puts("1\n");
			rightSpeed = (int) (((float) (timeDif) / (float) (rampTime)) * maxPower);
		}
		else if(rightMoved > rightDistance - brakingDistance)
		{
			puts("2\n");
			rightSpeed = (int) (((rightDistance - rightMoved) / brakingDistance) * maxPower);
		}
		else
		{
			puts("3\n");
			rightSpeed = maxPower;
		}
		//END OF RIGHT

		if(leftSpeed > maxPower)
		{
			puts("4\n");
			leftSpeed = maxPower;
		}
		else if(leftSpeed * -1 > maxPower)
		{
			puts("4A\n");
			leftSpeed = maxPower * -1;
		}

		if(rightSpeed > maxPower)
		{
			puts("5\n");
			rightSpeed = maxPower;
		}
		else if(rightSpeed * -1 > maxPower)
		{
			puts("5A\n");
			rightSpeed = maxPower * -1;
		}

		/*if(leftDistance < 0)
		{
			puts("6\n");
			leftSpeed *= -1;
		}

		if(rightDistance < 0)
		{
			puts("6A\n");
			rightSpeed *= -1;
		}*/

		if(leftDistance - leftMoved < deadband && rightDistance - rightMoved < deadband)
		{
			puts("7\n");
			finished = 1;
			leftSpeed = 0;
			rightSpeed = 0;
		}
		else
		{
			puts("8\n");
			printf("Left Moved: %d\n", leftMoved);
			printf("Right Moved: %d\n", rightMoved);
			printf("Left Speed: %d\n", leftSpeed);
			printf("Right Speed: %d\n\n", rightSpeed);
		}
		//END OF LEFT

		leftSpeed = (LEFT_MOTOR_INVERTED) ? leftSpeed * -1 : leftSpeed;
		rightSpeed = (RIGHT_MOTOR_INVERTED) ? rightSpeed * -1 : rightSpeed;

		motorSet(leftMotor, leftSpeed);
		motorSet(rightMotor, rightSpeed);

		delay(20);
	}
}

void operatorControl()
{


	pidValues liftPid = {1, 0.000, 0.0, 0, 0, millis(), 127, -127};

	moveStraightForIME(LEFT_MOTOR, RIGHT_MOTOR, LEFT_IME, RIGHT_IME, -2000, 100);

	encoderReset(liftEnc);

	do
	{
		motorSet(LIFT_MOTOR, pidController(&liftPid, 50.0, encoderGet(liftEnc)));
		delay(20);

	}while(liftPid.lastError > 10);

	motorSet(LEFT_MOTOR, 0);

	turnForIME(LEFT_MOTOR, RIGHT_MOTOR, LEFT_IME, RIGHT_IME, 1000, -1000, 127);

	puts("Done");

	motorSet(LEFT_MOTOR, 0);
	motorSet(RIGHT_MOTOR, 0);
}
