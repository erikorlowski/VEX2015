/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
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

void changeSelection(int valueToChange, int * selection, int size)
{
	if(valueToChange == -1)
	{
		if((* selection) > 0)
		{
			(* selection) --;
		}
		else
		{
			(* selection) = size - 1;
		}
	}
	else if(valueToChange == 1)
		{
			if((* selection) < size - 1)
			{
				(* selection) ++;
			}
			else
			{
				(* selection) = 0;
			}
		}
}

void lcdModeSelect()
{
	int alliance;
	const int RED = 0;
	const int BLUE = 1;

	puts("6:55");
	int inModeSelection = 1;
	int step = 1;
	int lastButtonPress = 0;
	int newButtonPressed;
	int lastStep = 0;

	const char * selectionText[] = {"Nothing", "Something"};
	int size = 2;

	int selection = 0;

	while((isOnline() ? (!isAutonomous() && !isEnabled()
			&& inModeSelection) : inModeSelection))
	{
		printf("Step: %d\n", step);

		if(lastButtonPress != lcdReadButtons(uart1))
		{
			newButtonPressed = lcdReadButtons(uart1);
		}
		else
		{
			newButtonPressed = 0;
		}

		lastButtonPress = lcdReadButtons(uart1);

		switch(step)
		{
		case(1):
			puts("Setting Text");

			if(step != lastStep)
			{
				lcdSetText(uart1, 1, "    Alliance    ");
				lcdSetText(uart1, 2, "Red         Blue");
			}

			lastStep = step;

			if(newButtonPressed == 1)
			{
				alliance = RED;
				step ++;
			}

			if(newButtonPressed == 4)
			{
				alliance = BLUE;
				step ++;
			}

			break;

		case(2):
			lcdSetText(uart1, 1, "      Mode      ");
			lcdSetText(uart1, 2, selectionText[selection]);

			if(newButtonPressed == 1)
			{
				changeSelection(-1, &selection, size);
			}

			if(newButtonPressed == 4)
			{
				changeSelection(1, &selection, size);
			}

			if(newButtonPressed == 2)
			{
				step++;
			}
			break;

		default:
			lcdSetText(uart1, 1, "   Selection:   ");
			lcdPrint(uart1, 2, "%s  %s", (alliance ? "Blue" : "Red"),
					selectionText[selection]);

			delay(5000);

			lcdSetBacklight(uart1, false);

			inModeSelection = 0;

		}

		delay(20);
	}
}

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO()
{
	lcdInit(uart1);
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize()
{
	puts("Running");

	lcdSetBacklight(uart1, true);

	lcdModeSelect();
}
