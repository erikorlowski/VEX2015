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
int limit(int value, int max, int min)
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

int max(int num1, int num2)
{
	if(num1 > num2)
	{
		return num1;
	}
	else
	{
		return num2;
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
int liftSafety(int desiredSpeed, int potValue, int max)
{
	printf("Value: %d", potValue);

	if(potValue < FIRST_STAGE_LEFT_MIN_HEIGHT)
	{
		puts("Lower Limit");
		return limit(desiredSpeed, 127, 0);
	}
	else if(potValue > FIRST_STAGE_LEFT_SLOW_HEIGHT && potValue < max)
	{
		puts("Slow");
		return limit(desiredSpeed, 40, -127);
	}
	else if(potValue > max)
	{
		puts("Upper Limit");
		printf("Returns: %d", limit(127, 0, -127));
		return limit(desiredSpeed, 0, -127);
	}
	else
	{
		return desiredSpeed;
	}
}

void liftMasterSlave(int desiredSpeed, int *(pLeftSide), int *(pRightSide), double kP, int leftPot, int rightPot)
{
	int error = rightPot - leftPot;

	if(error > 0)
	{
		*(pLeftSide) = desiredSpeed;
		*(pRightSide) = desiredSpeed - kP * error;
	}
	else
	{
		*(pLeftSide) = desiredSpeed - kP * error;
		*(pRightSide) = desiredSpeed;
	}
}

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

void liftFirstStage(int speed, int overrideSafeties, int leftPotValue, int rightPotValue)
{
	int leftSpeed = speed;
	int rightSpeed = speed;

	/*leftMasterSlave(speed, &leftSpeed, &rightSpeed, .003,
			leftPotValue, rightPotValue);*/

	leftSpeed = limit(leftSpeed, 127, -127);
	rightSpeed = limit(rightSpeed, 127, -127);

	if(!overrideSafeties)
	{
		int potValue = leftPotValue;//max(leftPotValue, rightPotValue);

		leftSpeed = liftSafety(leftSpeed, potValue, 10000);
		rightSpeed = liftSafety(rightSpeed, potValue, 10000);
	}

	leftSpeed = FIRST_STAGE_LIFT_LEFT_INVERTED ? -leftSpeed : leftSpeed;
	rightSpeed = FIRST_STAGE_LIFT_RIGHT_INVERTED ? -rightSpeed : rightSpeed;

	motorSet(FIRST_STAGE_LIFT_LEFT, leftSpeed);
	motorSet(FIRST_STAGE_LIFT_RIGHT, rightSpeed);
}

void liftSecondStage(int speed, int overrideSafeties, int potValue)
{
	int leftSpeed = limit(speed, 127, -127);
	int rightSpeed = limit(speed, 127, -127);

	if(!overrideSafeties)
	{
		leftSpeed = liftSafety(leftSpeed, potValue, FIRST_STAGE_LEFT_MAX_HEIGHT);
		rightSpeed = liftSafety(rightSpeed, potValue, FIRST_STAGE_LEFT_MAX_HEIGHT);
	}

	leftSpeed = SECOND_STAGE_LIFT_LEFT_INVERTED ? -leftSpeed : leftSpeed;
	rightSpeed = SECOND_STAGE_LIFT_RIGHT_INVERTED ? -rightSpeed : rightSpeed;

	motorSet(SECOND_STAGE_LIFT_LEFT, leftSpeed);
	motorSet(SECOND_STAGE_LIFT_RIGHT, rightSpeed);
}

void pickup(int speed)
{
	int theSpeed = PICKUP_MOTOR_INVERTED ? -speed : speed;

	theSpeed = limit(theSpeed, 127, -127);

	motorSet(PICKUP_MOTOR, theSpeed);
}

/*//Autonomous Variables
const int RED_ONE_LOW_TWO_LINE = 0;
int autonMode = 0;

void autonInit()
{
	//Auton init code goes here
}



void myAutonomous()
{
	int step = 1;
	int isAuto = 1;
	int lastStep = 0;
	int stepStartTime = millis();
	int elapsedTime = 0;
	int firstStageLeftPotValue;
	int firstStageRightPotValue;
	int secondStagePotValue;

	autonInit();

	while(isAuto)
	{

		firstStageLeftPotValue = FIRST_STAGE_POT_LEFT_INVERTED ?
			(4095 - analogRead(FIRST_STAGE_POT_LEFT)) :
			analogRead(FIRST_STAGE_POT_LEFT);
		firstStageRightPotValue = FIRST_STAGE_POT_RIGHT_INVERTED ?
			(4095 - analogRead(FIRST_STAGE_POT_RIGHT)) :
			analogRead(FIRST_STAGE_POT_RIGHT);
		secondStagePotValue = SECOND_STAGE_POT_INVERTED ?
			(4095 - analogRead(SECOND_STAGE_POT)) :
			analogRead(SECOND_STAGE_POT);

		if(lastStep != step)
		{
			stepStartTime = millis();
		}

		lastStep = step;

		elapsedTime = millis() - stepStartTime;

		if(autonMode == RED_ONE_LOW_TWO_LINE)
		{
			switch(step)
			{
			case(1): //Strafe to low goal
				holonomicDrive(-50,0,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1400)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			case(2): //Put preloaded cube onto the low goal

				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				pickup(-100);

				if(elapsedTime > 500)
				{
					pickup(0);
					step++;
				}
				break;

			case(3): //Strafe to lineup with cubes
				holonomicDrive(50,0,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 3400)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			case(4): //Backup to give distance from the wall
				holonomicDrive(0,-40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 200)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			case(5): //Turn 180 degrees
				holonomicDrive(0,0,40);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 4700)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			case(6): //Drive to first cube
				holonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1500)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			case(7): //Pickup first cube
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				pickup(100);

				if(elapsedTime > 1300)
				{
					pickup(0);
					step++;
				}
				break;

			case(8): //Wait to allow first cube to be taken off of robot
					//TODO delete
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 3000)
				{
					step++;
				}
				break;

			case(9): //Drive to second cube
				holonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1500)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			case(10): //Pickup second cube
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				pickup(100);

				if(elapsedTime > 700)
				{
					pickup(0);
					step++;
				}
				break;

			case(11): //Drive forward to lineup with medium post
					//TODO tune
				holonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1000)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			case(12): //Turn 90 degrees
					//TODO Tune
				holonomicDrive(0,0,-40);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 2400)
				{
					holonomicDrive(0,0,0);
					step++;
				}
				break;

			/*case(13): //Drive forward to medium post while raising lift
					//TODO tune
				const int FIRST_STAGE_HEIGHT = 500;
				const int SECOND_STAGE_HEIGHT = 1000;

				holonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				//NOTE: Setting the lift to 10 after result is achieved
				//is to stall the lift and keep it at the desired height
				//chanfe this if needed

				if(elapsedTime > 1000 && firstStageLeftPotValue > FIRST_STAGE_HEIGHT &&
						firstStageRightPotValue > FIRST_STAGE_HEIGHT &&
						secondStagePotValue > SECOND_STAGE_HEIGHT)
				{
					holonomicDrive(0,0,0);
					liftFirstStage(10,0,firstStageLeftPotValue,firstStageRightPotValue);
					liftSecondStage(10,0,secondStagePotValue);
					step++;
				}
				else
				{
					if(elapsedTime > 1500)
					{
						holonomicDrive(0,0,0);
					}
					else
					{
						holonomicDrive(0,40,0);
					}

					if(firstStageLeftPotValue > FIRST_STAGE_HEIGHT
							&& firstStageRightPotValue > FIRST_STAGE_HEIGHT)
					{
						liftFirstStage(10,0,firstStageLeftPotValue,firstStageRightPotValue);
					}
					else
					{
						liftFirstStage(55,0,firstStageLeftPotValue,firstStageRightPotValue);
					}

					if(secondStagePotValue > SECOND_STAGE_HEIGHT)
					{
						liftSecondStage(10,0,secondStagePotValue);
					}
					else
					{
						liftSecondStage(55,0,secondStagePotValue);
					}
				}
				break;

			case(14): //Score Cubes
					//TODO Tune
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				pickup(-100);

				if(elapsedTime > 700)
				{
					pickup(0);
					step++;
				}
				break;

			case(15): //Drive lift down
				//TODO tune
				const int FIRST_STAGE_HEIGHT = 100;
				const int SECOND_STAGE_HEIGHT = 100;

				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(firstStageLeftPotValue < FIRST_STAGE_HEIGHT &&
					firstStageRightPotValue < FIRST_STAGE_HEIGHT &&
					secondStagePotValue < SECOND_STAGE_HEIGHT)
				{
					liftFirstStage(0,0,firstStageLeftPotValue,firstStageRightPotValue);
					liftSecondStage(0,0,secondStagePotValue);
					step++;
				}
				else
				{
					if(firstStageLeftPotValue < FIRST_STAGE_HEIGHT
						&& firstStageRightPotValue < FIRST_STAGE_HEIGHT)
					{
						liftFirstStage(0,0,firstStageLeftPotValue,firstStageRightPotValue);
					}
					else
					{
						liftFirstStage(-35,0,firstStageLeftPotValue,firstStageRightPotValue);
					}

					if(secondStagePotValue > SECOND_STAGE_HEIGHT)
					{
						liftSecondStage(0,0,secondStagePotValue);
					}
					else
					{
						liftSecondStage(-35,0,secondStagePotValue);
					}
				}
				break;

			default:
				holonomicDrive(0,0,0);
				pickup(0);
				isAuto = 0;
				puts("Autonomous Finished");
				break;
		}
	}


		delay(20);
	}
}*/



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
	int overrideFirstStageLiftSafety = 0;
	const int STRAFE_SPEED = 50;
	int firstStageLeftPotValue;
	int firstStageRightPotValue;
	int secondStagePotValue;

	teleopInit();

	while(1)
	{
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

		firstStageLeftPotValue = FIRST_STAGE_POT_LEFT_INVERTED ?
				(4095 - analogRead(FIRST_STAGE_POT_LEFT)) :
				analogRead(FIRST_STAGE_POT_LEFT);
		firstStageRightPotValue = FIRST_STAGE_POT_RIGHT_INVERTED ?
						(4095 - analogRead(FIRST_STAGE_POT_RIGHT)) :
						analogRead(FIRST_STAGE_POT_RIGHT);
		secondStagePotValue = SECOND_STAGE_POT_INVERTED ?
						(4095 - analogRead(SECOND_STAGE_POT)) :
						analogRead(SECOND_STAGE_POT);

		if(joystickGetDigital(2, 7, JOY_UP))
		{
			firstStageLiftSpeed = FIRST_STAGE_SPEED;
		}
		else if(joystickGetDigital(2, 7, JOY_DOWN))
		{
			firstStageLiftSpeed = -FIRST_STAGE_SPEED;
		}
		else
		{
			firstStageLiftSpeed = 0;
		}

		if(joystickGetDigital(2,5,JOY_UP) && joystickGetDigital(2,5,JOY_DOWN))
		{
			overrideFirstStageLiftSafety = 1;
		}
		else
		{
			overrideFirstStageLiftSafety = 0;
		}

		liftFirstStage(firstStageLiftSpeed, overrideFirstStageLiftSafety,
				firstStageLeftPotValue, firstStageRightPotValue);

		if(joystickGetDigital(2, 8, JOY_UP))
		{
			secondStageLiftSpeed = SECOND_STAGE_SPEED;
		}
		else if(joystickGetDigital(2, 8, JOY_DOWN))
		{
			secondStageLiftSpeed = -SECOND_STAGE_SPEED;
		}
		else
		{
			secondStageLiftSpeed = 0;
		}

		liftSecondStage(secondStageLiftSpeed, overrideFirstStageLiftSafety,
				secondStagePotValue);

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

		printf("1_L: %d\n1_R: %d\n2: %d\n",
				firstStageLeftPotValue, firstStageRightPotValue,
				secondStagePotValue);

		delay(20);
	}
}

void operatorControl()
{
	//autonomous();
	teleop();
}
