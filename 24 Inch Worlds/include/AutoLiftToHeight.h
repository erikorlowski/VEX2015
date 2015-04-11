/*
 * AutoLiftToHeight.h
 *
 *  Created on: Mar 18, 2015
 *      Author: Erik
 */

#ifndef AUTOLIFTTOHEIGHT_H_
#define AUTOLIFTTOHEIGHT_H_

struct AutoLiftToHeight{

	int desiredHeight;
	int isFinished;
	Lift lift;
	int deadBand;

}typedef AutoLiftToHeight;

AutoLiftToHeight initAutoLiftToHeight(Lift lift, int height, int deadBand);
void autoLiftToHeight(AutoLiftToHeight *stepInfo);

#endif /* AUTOLIFTTOHEIGHT_H_ */
