#pragma once
#include "Boid.h"

class Boids
{
	public:
	Boid * tab;
	int nb_boid;
	double xg,yg,zg,xa,ya,za;

	double distance(Boid & b1, Boid & b2);
	Boids(double x,double y, double z ,int n);
	void initPosition();
	void barycentre();
	void genereArrive();
	void vecteurCohesion (Boid & b, double *);
	void vecteurSeparation(Boid & b, double *);
	void vecteurAlignement (Boid & b, double *);
	void updateVitesse(int dt = PAS_DE_TEMPS);
    void updateAcceleration();
	~Boids(void);
};

