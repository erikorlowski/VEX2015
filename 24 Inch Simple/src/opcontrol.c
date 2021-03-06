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

//UTILITIES
double limit(double value, double max, double min)
{
	if(value > max)
	{
		return max;
	}
	else if(value < min)
	{
		return min;
	}
	else
	{
		return value;
	}
}

double absoluteValue(double num)
{
	if(num < 0)
	{
		return -num;
	}
	else
	{
		return num;
	}
}

//TEAM METHODS
void holonomicDrive(int x, int y, int rotation)
{
	int frontLeft;
	int frontRight;
	int rearLeft;
	int rearRight;

	frontLeft = y - x + rotation;
	frontRight = y + x - rotation;
	rearLeft = y + x + rotation;
	rearRight = y - x - rotation;

	if(FRONT_LEFT_DRIVE_INVERTED) frontLeft *= -1;
	if(FRONT_RIGHT_DRIVE_INVERTED) frontRight *= -1;
	if(REAR_LEFT_DRIVE_INVERTED) rearLeft *= -1;
	if(REAR_RIGHT_DRIVE_INVERTED) rearRight *= -1;

	frontLeft = limit(frontLeft, 127, -127);
	frontRight = limit(frontRight, 127, -127);
	rearLeft = limit(rearLeft, 127, -127);
	rearRight = limit(rearRight, 127, -127);

	motorSet(FRONT_LEFT_DRIVE, frontLeft);
	motorSet(FRONT_RIGHT_DRIVE, frontRight);
	motorSet(REAR_LEFT_DRIVE, rearLeft);
	motorSet(REAR_RIGHT_DRIVE, rearRight);
}

void liftFirstStage(int speed)
{
	int leftSpeed = FIRST_STAGE_LIFT_LEFT_INVERTED ? -speed : speed;
	int rightSpeed = FIRST_STAGE_LIFT_RIGHT_INVERTED ? -speed : speed;

	leftSpeed = limit(leftSpeed, 127, -127);
	rightSpeed = limit(rightSpeed, 127, -127);

	motorSet(FIRST_STAGE_LIFT_LEFT, leftSpeed);
	motorSet(FIRST_STAGE_LIFT_RIGHT, rightSpeed);
}

void liftSecondStage(int speed)
{
	int leftSpeed = SECOND_STAGE_LIFT_LEFT_INVERTED ? -speed : speed;
	int rightSpeed = SECOND_STAGE_LIFT_RIGHT_INVERTED ? -speed : speed;

	leftSpeed = limit(leftSpeed, 127, -127);
	rightSpeed = limit(rightSpeed, 127, -127);

	motorSet(SECOND_STAGE_LIFT_LEFT, leftSpeed);
	motorSet(SECOND_STAGE_LIFT_RIGHT, rightSpeed);
}

void pickup(int speed)
{
	int theSpeed = PICKUP_MOTOR_INVERTED ? -speed : speed;

	theSpeed = limit(theSpeed, 127, -127);

	motorSet(PICKUP_MOTOR, theSpeed);
}

void autonInit()
{
	//Auton init code goes here
}



void myAutonomous()
{
	int step = 1;
	int isAuto = 0;

	autonInit();

	while(isAuto)
	{
		//Code to happen in all steps goes here
		switch(step)
		{


		case(1):
			//Step 1 code goes here
			break;
		}

		delay(20);
	}
}



void teleopInit()
{

}

void teleop()
{
	int driveX;
	int driveY;
	int driveRotation;
	int firstStageLiftSpeed;
	const int FIRST_STAGE_SPEED = 55;
	int secondStageLiftSpeed;
	const int SECOND_STAGE_SPEED = 55;
	int pickupSpeed;
	const int PICKUP_RUN_SPEED = 127;
	const int STRAFE_SPEED = 50;

	teleopInit();

	while(1)
	{
		puts("Frame");
		driveX = -joystickGetAnalog(1, 4);
		driveY = joystickGetAnalog(1, 3);
		driveRotation = joystickGetAnalog(1, 1);

		if(joystickGetDigital(1,7,JOY_LEFT))
		{
			driveX = STRAFE_SPEED;
		}
		else if(joystickGetDigital(1,7,JOY_RIGHT))
		{
			driveX = -STRAFE_SPEED;
		}

		holonomicDrive(driveX, driveY, driveRotation);

		if(joystickGetDigital(1, 7, JOY_UP))
		{
			firstStageLiftSpeed = FIRST_STAGE_SPEED;
		}
		else if(joystickGetDigital(1, 7, JOY_DOWN))
		{
			firstStageLiftSpeed = -FIRST_STAGE_SPEED;
		}
		else
		{
			firstStageLiftSpeed = 0;
		}

		liftFirstStage(firstStageLiftSpeed);

		if(joystickGetDigital(1, 8, JOY_UP))
		{
			secondStageLiftSpeed = SECOND_STAGE_SPEED;
		}
		else if(joystickGetDigital(1, 8, JOY_DOWN))
		{
			secondStageLiftSpeed = -SECOND_STAGE_SPEED;
		}
		else
		{
			secondStageLiftSpeed = 0;
		}

		liftSecondStage(secondStageLiftSpeed);

		if(joystickGetDigital(1, 6, JOY_UP))
		{
			pickupSpeed = PICKUP_RUN_SPEED;
		}
		else if(joystickGetDigital(1, 6, JOY_DOWN))
		{
			pickupSpeed = -PICKUP_RUN_SPEED;
		}
		else
		{
			pickupSpeed = 0;
		}

		pickupSpeed = PICKUP_MOTOR_INVERTED ? -pickupSpeed : pickupSpeed;

		motorSet(PICKUP_MOTOR, pickupSpeed);

		delay(20);
	}
}

void operatorControl()
{
	//myAutonomous();
	teleop();
}
