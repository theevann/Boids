#include "Boids.h"
#include "Boid.h"
#include  "stdlib.h"
#include  "time.h"
#include <math.h>
#include <iostream>



Boids::Boids(double x,double y,double z,int n): xa(x), ya(y), za(z), nb_boid(n)
{
	tab = new Boid [n];
	initPosition();
}


Boids::~Boids()
{
    delete [] tab;
}

void Boids::initPosition(){
	srand((unsigned)time(NULL));
	for (int i = 0 ; i < nb_boid ; i++){
		double x0 = rand()%(LargeurX)-LargeurX/2;
		double y0 = rand()%(LargeurY)-LargeurY/2;
		double z0 = rand()%(LargeurZ)-LargeurZ/2;
		tab[i] = Boid(x0,y0,z0);
	}
}

void Boids::genereArrive()
{
    srand((unsigned)time(NULL));
    xa = rand()%(2*LargeurX)-LargeurX;
	ya = rand()%(2*LargeurY)-LargeurY;
	za = rand()%(2*LargeurZ)-LargeurZ;
}

void Boids::barycentre(){
	double xx =0,yy=0,zz=0;
	for (int i=0;i<nb_boid;i++){
        xx+= tab[i].x;
        yy+= tab[i].y;
        zz+= tab[i].z;
	}
	xg=xx/nb_boid;
	yg=yy/nb_boid;
	zg=zz/nb_boid;
}

void Boids::vecteurCohesion(Boid & b, double * vectorCohesion){
	vectorCohesion[0]=log(abs(xg-b.x)+1);
	vectorCohesion[1]= log(abs(yg-b.y)+1);
	vectorCohesion[2]= log(abs(zg-b.z)+1);
	double norme = sqrt(vectorCohesion[0]*vectorCohesion[0]+vectorCohesion[1]*vectorCohesion[1]+vectorCohesion[2]*vectorCohesion[2]);
	vectorCohesion[0] /= norme;
	vectorCohesion[1] /= norme;
	vectorCohesion[2] /= norme;
}


void Boids::vecteurSeparation(Boid & b, double * somme){
	somme[0] = 0;
    somme[1] = 0;
    somme[2] = 0;

        for(int i = 0 ; i < nb_boid ; i++){
            double dist = distance(tab[i],b);
            if(dist > 0.001 && dist < RAYON_INFL){
                somme[0] += ((b.x - tab[i].x)/(dist));
                somme[1] += ((b.y - tab[i].y)/(dist));
                somme[2] += ((b.z - tab[i].z)/(dist));
            }
        }

    double norme = sqrt((somme[0]*somme[0]+somme[1]*somme[1]+somme[2]*somme[2]));

	if(norme > 0.001){
		somme[0] = (somme[0]/(norme));
		somme[1] = (somme[1]/(norme));
		somme[2] = (somme[2]/(norme));
	}
}

double Boids::distance(Boid & b1, Boid & b2){
    return sqrt(((b1.x-b2.x)*(b1.x-b2.x)+(b1.y-b2.y)*(b1.y-b2.y)+(b1.z-b2.z)*(b1.z-b2.z)));
}


void Boids::vecteurAlignement(Boid & b, double * vectorAlignement){
	vectorAlignement[0] = (xa-b.x);
	vectorAlignement[1] = (ya-b.y);
	vectorAlignement[2] = (za-b.z);
	double norme = sqrt((vectorAlignement[0]*vectorAlignement[0]+vectorAlignement[1]*vectorAlignement[1]+vectorAlignement[2]*vectorAlignement[2]));
	vectorAlignement[0] /= norme;
	vectorAlignement[1] /= norme;
	vectorAlignement[2] /= norme;
}
/*
void Boids::updateVitesse(){
    double * vectorC = new double  [3];
    double * vectorA = new double  [3];
	double * vectorS = new double  [3];

	for (int i=0;i<nb_boid;i++){
		vecteurCohesion(tab[i], vectorC);
		vecteurAlignement(tab[i], vectorA);
		vecteurSeparation(tab[i], vectorS);

		tab[i].vx = C_COHES * vectorC[0] + C_SEP * vectorS[0] + C_ALIGN * vectorA[0];
		tab[i].vy = C_COHES * vectorC[1] + C_SEP * vectorS[1] + C_ALIGN * vectorA[1];
		tab[i].vz = C_COHES * vectorC[2] + C_SEP * vectorS[2] + C_ALIGN * vectorA[2];

        double norme = sqrt(tab[i].vx*tab[i].vx+tab[i].vy*tab[i].vy+tab[i].vz*tab[i].vz);

        tab[i].vx *= NORME/norme;
        tab[i].vy *=  NORME/norme;
        tab[i].vz *=  NORME/norme;
	}

	delete [] vectorA;
	delete [] vectorC;
	delete [] vectorS;
}
*/

void Boids::updateAcceleration(){
    double * vectorC = new double  [3];
    double * vectorA = new double  [3];
	double * vectorS = new double  [3];

	for (int i=0;i<nb_boid;i++){
		vecteurCohesion(tab[i], vectorC);
		vecteurAlignement(tab[i], vectorA);
		vecteurSeparation(tab[i], vectorS);

		tab[i].ax = C_COHES * vectorC[0] + C_SEP * vectorS[0] + C_ALIGN * vectorA[0];
		tab[i].ay = C_COHES * vectorC[1] + C_SEP * vectorS[1] + C_ALIGN * vectorA[1];
		tab[i].az = C_COHES * vectorC[2] + C_SEP * vectorS[2] + C_ALIGN * vectorA[2];

        double norme = sqrt(tab[i].ax*tab[i].ax+tab[i].ay*tab[i].ay+tab[i].az*tab[i].az);

        tab[i].ax /= (norme*10);
        tab[i].ay /= (norme*10);
        tab[i].az /= (norme*10);
	}

	delete [] vectorA;
	delete [] vectorC;
	delete [] vectorS;
}

void Boids::updateVitesse(int dt){

	for (int i=0;i<nb_boid;i++){

		tab[i].vx += tab[i].ax*dt;
		tab[i].vy += tab[i].ay*dt;
		tab[i].vz += tab[i].az*dt;

        double norme = sqrt(tab[i].vx*tab[i].vx+tab[i].vy*tab[i].vy+tab[i].vz*tab[i].vz);

        tab[i].vx *= NORME/norme;
        tab[i].vy *=  NORME/norme;
        tab[i].vz *=  NORME/norme;
	}

}
