/*
 * Pot.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#ifndef POT_H_
#define POT_H_

/**
 * The data structure for a potentiometer. This structure and its associated
 * functions will handle any mechanical inversions of the sensor.
 */
struct Pot{

	int channel;
	int inverted;

}typedef Pot;

Pot initPot(int channel, int inverted);
int getPot(Pot pot);

#endif /* POT_H_ */
