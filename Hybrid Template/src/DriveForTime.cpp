/*
 * DriveForTime.cpp
 *
 *  Created on: Feb 21, 2015
 *      Author: Erik
 */

#include "AutoClasses.hpp"

class DriveForTime : public Step{

public:
	void start();
	void excectute();
	bool isFinished();
	void kill();
};
