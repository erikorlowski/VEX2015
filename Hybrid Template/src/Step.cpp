/*
 * Step.cpp
 *
 *  Created on: Feb 21, 2015
 *      Author: Erik
 */

//#include "main.h"

class Step
{
	public:
		virtual void start();
		virtual void excectute();
		virtual bool isFinished();
		virtual void kill();
};
