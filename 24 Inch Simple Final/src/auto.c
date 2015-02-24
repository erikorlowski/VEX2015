/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
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
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT autoLimitED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT autoLimitED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
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
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */

//UTILITIES
int autoLimit(int value, int autoMax, int min)
{
	if(value > autoMax)
	{
		return autoMax;
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

int autoMax(int num1, int num2)
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

double autoAbs(double num)
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
int autoLiftSafety(int desiredSpeed, int potValue, int autoMax)
{
	printf("Value: %d", potValue);

	if(potValue < FIRST_STAGE_LEFT_MIN_HEIGHT)
	{
		puts("Lower autoLimit");
		return autoLimit(desiredSpeed, 127, 0);
	}
	else if(potValue > FIRST_STAGE_LEFT_SLOW_HEIGHT && potValue < autoMax)
	{
		puts("Slow");
		return autoLimit(desiredSpeed, 40, -127);
	}
	else if(potValue > autoMax)
	{
		puts("Upper autoLimit");
		printf("Returns: %d", autoLimit(127, 0, -127));
		return autoLimit(desiredSpeed, 0, -127);
	}
	else
	{
		return desiredSpeed;
	}
}

/*void liftMasterSlave(int desiredSpeed, int *(pLeftSide), int *(pRightSide), double kP, int leftPot, int rightPot)
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
}*/

void autoHolonomicDrive(int x, int y, int rotation)
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

	frontLeft = autoLimit(frontLeft, 127, -127);
	frontRight = autoLimit(frontRight, 127, -127);
	rearLeft = autoLimit(rearLeft, 127, -127);
	rearRight = autoLimit(rearRight, 127, -127);

	motorSet(FRONT_LEFT_DRIVE, frontLeft);
	motorSet(FRONT_RIGHT_DRIVE, frontRight);
	motorSet(REAR_LEFT_DRIVE, rearLeft);
	motorSet(REAR_RIGHT_DRIVE, rearRight);
}

void autoLiftFirstStage(int speed, int overrideSafeties, int leftPotValue, int rightPotValue)
{
	int leftSpeed = speed;
	int rightSpeed = speed;

	/*leftMasterSlave(speed, &leftSpeed, &rightSpeed, .003,
			leftPotValue, rightPotValue);*/

	leftSpeed = autoLimit(leftSpeed, 127, -127);
	rightSpeed = autoLimit(rightSpeed, 127, -127);

	if(!overrideSafeties)
	{
		int potValue = leftPotValue;//autoMax(leftPotValue, rightPotValue);

		leftSpeed = autoLiftSafety(leftSpeed, potValue, 10000);
		rightSpeed = autoLiftSafety(rightSpeed, potValue, 10000);
	}

	leftSpeed = FIRST_STAGE_LIFT_LEFT_INVERTED ? -leftSpeed : leftSpeed;
	rightSpeed = FIRST_STAGE_LIFT_RIGHT_INVERTED ? -rightSpeed : rightSpeed;

	motorSet(FIRST_STAGE_LIFT_LEFT, leftSpeed);
	motorSet(FIRST_STAGE_LIFT_RIGHT, rightSpeed);
}

void autoLiftSecondStage(int speed, int overrideSafeties, int potValue)
{
	int leftSpeed = autoLimit(speed, 127, -127);
	int rightSpeed = autoLimit(speed, 127, -127);

	if(!overrideSafeties)
	{
		leftSpeed = autoLiftSafety(leftSpeed, potValue, FIRST_STAGE_LEFT_MAX_HEIGHT);
		rightSpeed = autoLiftSafety(rightSpeed, potValue, FIRST_STAGE_LEFT_MAX_HEIGHT);
	}

	leftSpeed = SECOND_STAGE_LIFT_LEFT_INVERTED ? -leftSpeed : leftSpeed;
	rightSpeed = SECOND_STAGE_LIFT_RIGHT_INVERTED ? -rightSpeed : rightSpeed;

	motorSet(SECOND_STAGE_LIFT_LEFT, leftSpeed);
	motorSet(SECOND_STAGE_LIFT_RIGHT, rightSpeed);
}

void autoPickup(int speed)
{
	int theSpeed = PICKUP_MOTOR_INVERTED ? -speed : speed;

	theSpeed = autoLimit(theSpeed, 127, -127);

	motorSet(PICKUP_MOTOR, theSpeed);
}

const int RED_ONE_LOW_TWO_LINE = 0;
int autonMode = 0;

void autonInit()
{
	//Auton init code goes here
}

void autonomous()
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
				autoHolonomicDrive(-50,0,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1400)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			case(2): //Put preloaded cube onto the low goal

				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				autoPickup(-100);

				if(elapsedTime > 500)
				{
					autoPickup(0);
					step++;
				}
				break;

			case(3): //Strafe to lineup with cubes
				autoHolonomicDrive(50,0,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 3400)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			case(4): //Backup to give distance from the wall
				autoHolonomicDrive(0,-40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 200)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			case(5): //Turn 180 degrees
				autoHolonomicDrive(0,0,40);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 4700)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			case(6): //Drive to first cube
				autoHolonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1500)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			case(7): //autoPickup first cube
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				autoPickup(100);

				if(elapsedTime > 1300)
				{
					autoPickup(0);
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
				autoHolonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1500)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			case(10): //autoPickup second cube
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				autoPickup(100);

				if(elapsedTime > 700)
				{
					autoPickup(0);
					step++;
				}
				break;

			case(11): //Drive forward to lineup with medium post
					//TODO tune
				autoHolonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 1000)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			case(12): //Turn 90 degrees
					//TODO Tune
				autoHolonomicDrive(0,0,-40);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				if(elapsedTime > 2400)
				{
					autoHolonomicDrive(0,0,0);
					step++;
				}
				break;

			/*case(13): //Drive forward to medium post while raising lift
					//TODO tune
				const int FIRST_STAGE_HEIGHT = 500;
				const int SECOND_STAGE_HEIGHT = 1000;

				autoHolonomicDrive(0,40,0);
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				//NOTE: Setting the lift to 10 after result is achieved
				//is to stall the lift and keep it at the desired height
				//chanfe this if needed

				if(elapsedTime > 1000 && firstStageLeftPotValue > FIRST_STAGE_HEIGHT &&
						firstStageRightPotValue > FIRST_STAGE_HEIGHT &&
						secondStagePotValue > SECOND_STAGE_HEIGHT)
				{
					autoHolonomicDrive(0,0,0);
					autoLiftFirstStage(10,0,firstStageLeftPotValue,firstStageRightPotValue);
					autoLiftSecondStage(10,0,secondStagePotValue);
					step++;
				}
				else
				{
					if(elapsedTime > 1500)
					{
						autoHolonomicDrive(0,0,0);
					}
					else
					{
						autoHolonomicDrive(0,40,0);
					}

					if(firstStageLeftPotValue > FIRST_STAGE_HEIGHT
							&& firstStageRightPotValue > FIRST_STAGE_HEIGHT)
					{
						autoLiftFirstStage(10,0,firstStageLeftPotValue,firstStageRightPotValue);
					}
					else
					{
						autoLiftFirstStage(55,0,firstStageLeftPotValue,firstStageRightPotValue);
					}

					if(secondStagePotValue > SECOND_STAGE_HEIGHT)
					{
						autoLiftSecondStage(10,0,secondStagePotValue);
					}
					else
					{
						autoLiftSecondStage(55,0,secondStagePotValue);
					}
				}
				break;

			case(14): //Score Cubes
					//TODO Tune
				printf("Step: %d Elapsed Time: %d", step, elapsedTime);

				autoPickup(-100);

				if(elapsedTime > 700)
				{
					autoPickup(0);
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
					autoLiftFirstStage(0,0,firstStageLeftPotValue,firstStageRightPotValue);
					autoLiftSecondStage(0,0,secondStagePotValue);
					step++;
				}
				else
				{
					if(firstStageLeftPotValue < FIRST_STAGE_HEIGHT
						&& firstStageRightPotValue < FIRST_STAGE_HEIGHT)
					{
						autoLiftFirstStage(0,0,firstStageLeftPotValue,firstStageRightPotValue);
					}
					else
					{
						autoLiftFirstStage(-35,0,firstStageLeftPotValue,firstStageRightPotValue);
					}

					if(secondStagePotValue > SECOND_STAGE_HEIGHT)
					{
						autoLiftSecondStage(0,0,secondStagePotValue);
					}
					else
					{
						autoLiftSecondStage(-35,0,secondStagePotValue);
					}
				}
				break;*/

			default:
				autoHolonomicDrive(0,0,0);
				autoPickup(0);
				isAuto = 0;
				puts("Autonomous Finished");
				break;
		}
	}


		delay(20);
	}
}
