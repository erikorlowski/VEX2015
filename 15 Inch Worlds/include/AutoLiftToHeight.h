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

}typedef AutoLiftToHeight;

AutoLiftToHeight initAutoLiftToHeight(int height);
void autoLiftToHeight(AutoLiftToHeight *stepInfo);

#endif /* AUTOLIFTTOHEIGHT_H_ */
