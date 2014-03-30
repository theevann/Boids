#pragma once
#include "C:\Users\Evann\Documents\Mon travail\NSU\Info\OpenGl3\const.h"

class Boid
{// friend Boids;
public:
	double x,y,z;
	double vx,vy,vz;
	double ax,ay,az;

	Boid();
	Boid(double x0,double y0,double z0);
	void print();
	void deplace(double dt = PAS_DE_TEMPS);
	~Boid(void);
};


