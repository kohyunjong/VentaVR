#include "Capture.h"

// public functions //

Capture::Capture()
{

}

int Capture::getClass()
{
	return mMyClass;
}

// protected functions //

void Capture::setClass(int myClass)
{
	mMyClass = myClass;
}