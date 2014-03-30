#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <math.h>
#include <iostream>

#include "C:\Users\Evann\Desktop\Boids.h"

void dessinerRepere(unsigned int echelle = 1)
{
    glPushMatrix();
    glScalef(echelle,echelle,echelle);
    glBegin(GL_LINES);
    glColor3ub(255,0,0);
    glVertex3d(0,0,0);
    glVertex3d(1,0,0);
    glColor3ub(0,255,0);
    glVertex3d(0,0,0);
    glVertex3d(0,1,0);
    glColor3ub(0,0,255);
    glVertex3d(0,0,0);
    glVertex3d(0,0,1);
    glEnd();
    glPopMatrix();
}

void drawBoid(Boid &, GLUquadric *);
void drawGoal(Boids & b, GLUquadric * params);
void Dessiner(Boids &, double *);

double lastTime = 0;
double newTime = 0;
double intervalle = 50;
int nbImageSec = 30 ;
bool pause = false;

int main(int argc, char *argv[])
{
    SDL_Event event;
    double cam[3] = {240,240,240};
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("BOIDS", NULL);
    SDL_SetVideoMode(1200, 900, 32, SDL_OPENGL);
    //SDL_EnableKeyRepeat(10,10);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)1200/900,1,1000);
    glEnable(GL_DEPTH_TEST);

    Boids test(220,220,220, NOMBRE_BOIDS);

    Dessiner(test,cam);

    lastTime = SDL_GetTicks();

    for (;;)
    {
        SDL_PollEvent(&event);
        int startTime = SDL_GetTicks();

        switch(event.type)
        {
            case SDL_QUIT:
            exit(0);
            break;

            case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_q:
                    cam[0]  += 5;
                break;

                case SDLK_a:
                    cam[0]  -= 5;
                break;

                case SDLK_w:
                    cam[1]  += 5;
                break;

                case SDLK_s:
                    cam[1]  -= 5;
                break;

                case SDLK_e:
                    cam[2]  += 5;
                break;

                case SDLK_d:
                    cam[2]  -= 5;
                break;

                case SDLK_SPACE:
                    pause = !pause;
                break;

                case SDLK_BACKSPACE:
                    test.xa = 0;
                    test.ya = 0;
                    test.za = 0;
                break;

                case SDLK_DELETE:
                    test.genereArrive();
                break;

                case SDLK_PLUS:
                    if(intervalle>1)
                        intervalle--;
                break;
                case SDLK_MINUS:
                        intervalle++;
                break;
                case SDLK_UP:
                    cam[0]--;
                break;
                case SDLK_DOWN:
                    cam[0]++;
                break;
                case SDLK_LEFT:
                if ((event.key.keysym.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
                {
                    cam[0]+=0.1;
                }
                else
                {
                    cam[0]+=10;
                }
                break;
                case SDLK_RIGHT:
                if ((event.key.keysym.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
                {
                    cam[0]-=0.1;
                }
                else
                {
                    cam[0]-=10;
                }
                break;
            }
            break;

        }
        Dessiner(test, cam);

        int timeElapsed = SDL_GetTicks()-startTime;
        if(1000/nbImageSec>timeElapsed)
            SDL_Delay(1000/nbImageSec-timeElapsed);//*/
    }
    return 0;
}

void Dessiner(Boids & b, double * cam)
{
    newTime = SDL_GetTicks();

    if(newTime - lastTime < intervalle)
    { return;}
     lastTime = newTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(cam[0],cam[1],cam[2],0,0,0,0,0,1);
    //gluLookAt(3,4,2,0,0,0,0,0,1);

    glColor3ub(0,0,0);
    GLUquadric * params = gluNewQuadric();
    gluQuadricDrawStyle(params, GLU_FILL);

    if(!pause){
        b.updateAcceleration();
        b.updateVitesse();
    }

    for(int i = 0 ; i < b.nb_boid ; i++)
    {
        if(!pause)
            b.tab[i].deplace();
        drawBoid(b.tab[i], params);
    }


    drawGoal(b, params);

    gluDeleteQuadric(params);
/*
    glRotated(angleX,1,0,0);
    glRotated(angleZ,0,0,1);
*/
    glBegin(GL_QUADS);
    glColor3ub(255,0,0); //face rouge
    glVertex3d(LargeurX,LargeurY,LargeurZ);
    glVertex3d(LargeurX,LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,LargeurY,LargeurZ);
    glEnd();
/*
    glBegin(GL_QUADS);
    glColor3ub(0,255,0); //face verte
    glVertex3d(LargeurX,-LargeurY,LargeurZ);
    glVertex3d(LargeurX,-LargeurY,-LargeurZ);
    glVertex3d(LargeurX,LargeurY,-LargeurZ);
    glVertex3d(LargeurX,LargeurY,LargeurZ);
    glEnd();
*/
     glBegin(GL_QUADS);
    glColor3ub(0,0,255); //face bleu
    glVertex3d(LargeurX,-LargeurY,LargeurZ);
    glVertex3d(LargeurX,LargeurY,LargeurZ);
    glVertex3d(-LargeurX,LargeurY,LargeurZ);
    glVertex3d(-LargeurX,-LargeurY,LargeurZ);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0,255,255); //face cyan
    glVertex3d(LargeurX,-LargeurY,LargeurZ);
    glVertex3d(LargeurX,-LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,-LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,-LargeurY,LargeurZ);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,0,255); //face magenta
    glVertex3d(LargeurX,-LargeurY,-LargeurZ);
    glVertex3d(LargeurX,LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,-LargeurY,-LargeurZ);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,0); //face verte
    glVertex3d(-LargeurX,-LargeurY,LargeurZ);
    glVertex3d(-LargeurX,-LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,LargeurY,-LargeurZ);
    glVertex3d(-LargeurX,LargeurY,LargeurZ);
    glEnd();

/*
    glRotated(-angleZ,0,0,1);
    glRotated(-angleX,1,0,0);
*/
    glDisable(GL_DEPTH_TEST);

    glBegin(GL_LINES);
    glColor3ub(255,0,0); //face rouge
    glVertex3d(0,0,0);
    glVertex3d(2,0,0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,255,0); //face rouge
    glVertex3d(0,0,0);
    glVertex3d(0,2,0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,255); //face rouge
    glVertex3d(0,0,0);
    glVertex3d(0,0,2);
    glEnd();

    glEnable(GL_DEPTH_TEST);
//*/

    glFlush();
    SDL_GL_SwapBuffers();
}

void drawBoid(Boid & b, GLUquadric * params){
    glPushMatrix();
    glTranslated(b.x,b.y,b.z);
/*
    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3d(0,0,0);
    glVertex3d(b.vx*10,b.vy*10,b.vz*10);
    glEnd();
*/
    glRotated(2*atan(b.vy/(b.vx+sqrt(b.vx*b.vx+b.vy*b.vy)))*180/3.14159,0,0,1);
    glRotated(acos(b.vz/NORME)*180/3.14159,0,1,0);
    gluCylinder(params,1,0,2,25,1);
    glTranslated(0,0,-1);
    gluCylinder(params,0,1,1,25,1);
    glPopMatrix();
}


void drawGoal(Boids & b, GLUquadric * params){
    glPushMatrix();
    gluQuadricDrawStyle(params, GLU_FILL);
    glColor3ub(126,126,126);
    glTranslated(b.xa,b.ya,b.za);
    gluSphere(params,2,20,20);
    glPopMatrix();
}
