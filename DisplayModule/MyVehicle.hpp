#ifndef __MYVEHICLE_HPP__
#define __MYVEHICLE_HPP__


#include "Vehicle.hpp"
#include <Laser.h>
#include <SM.h>

class MyVehicle : public Vehicle
{
public:
	//double LaserX[361];
	//double LaserY[361];
	PM* PMSMPtr;
	MyVehicle(PM* PMPtr);
	virtual void draw();
	void drawLaser();
};

#endif