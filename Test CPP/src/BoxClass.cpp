/*
 * Box.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: Erik
 */

class Box
{
	int length, width, height;

public:
	Box initBox(int, int, int);
	int getVolume();
	Box(int length, int width, int height)
	{
		this->length = length;
		this->width = width;
		this->height = height;
	}
};

Box Box::initBox(int length, int width, int height)
{
	return Box(length, width, height);
}

int Box::getVolume()
{
	return length * width * height;
}
