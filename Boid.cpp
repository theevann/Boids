#include "Boid.h"
#include <iostream>

Boid::Boid()
{
}

Boid::Boid(double x0,double y0,double z0): x(x0), y(y0), z(z0),vx(0),vy(0),vz(0),ax(0),ay(0),az(0)
{
}


Boid::~Boid()
{
}

void Boid::deplace (double dt){
	x+=vx*dt;
	y+=vy*dt;
	z+=vz*dt;
}

void Boid::print(){
std::cout<<x<<" "<<y<<" "<<z<<std::endl;
//std::cout<<"Vitesse "<<vx<<" "<<vy<<" "<<vz<<std::endl;
}
