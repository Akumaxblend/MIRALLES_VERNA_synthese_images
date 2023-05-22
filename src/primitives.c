#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "primitives.h"
#include "3D_tools.h"
#include <time.h>

float calculateDist(float x1,float y1,float z1,float x2,float y2,float z2){

	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
}

float illuminationFactor(Ball b, Racket r, float x, float y, float z){

	float distance1 = calculateDist(b.x, b.y, b.z, x, y, z);
	float distance2 = calculateDist(r.racketx, r.rackety, r.racketz, x, y, z);

	return 1/distance1 + 1/distance2;
}

void drawRacket(float width, float height, double RacketX, double RacketY, float d){

	glColor3f(1.0,1.0,1.0);

	glBegin(GL_LINES);

	glVertex3f(RacketX-width/2, RacketY + height/2, d);
    glVertex3f(RacketX-width/2, RacketY-height/2, d);

	glVertex3f(RacketX + width/2, RacketY + height/2, d);
    glVertex3f(RacketX + width/2, RacketY-height/2, d);

    glVertex3f(RacketX-width/2, RacketY + height/2, d);
    glVertex3f(RacketX + width/2, RacketY + height/2, d);

    glVertex3f(RacketX-width/2, RacketY-height/2, d);
    glVertex3f(RacketX + width/2, RacketY-height/2, d);

	glEnd();
}

void drawSection(float width, float height, float length, float position, Ball b, Racket r){

	float illum;

	glColor3f(0.1,0.1,0.1);
	
    glBegin(GL_POLYGON);

		glNormal3f(0.0,1.0,0.0);
		glVertex3f(-width/2,-height/2,position);
		glNormal3f(0.0,1.0,0.0);
		glVertex3f(-width/2,-height/2,position-length);
		glNormal3f(0.0,1.0,0.0);
		glVertex3f(width/2,-height/2,position-length);
		glNormal3f(0.0,1.0,0.0);
		glVertex3f(width/2,-height/2,position);
	glEnd();

    glBegin(GL_POLYGON);
	glColor3f(0.1,0.1,0.1);

		glNormal3f(0.0,-1.0,0.0);
		glVertex3f(-width/2,height/2,position);
		glNormal3f(0.0,-1.0,0.0);
		glVertex3f(-width/2,height/2,position-length);
		glNormal3f(0.0,-1.0,0.0);
		glVertex3f(width/2,height/2,position-length);
		glNormal3f(0.0,-1.0,0.0);
		glVertex3f(width/2,height/2,position);

	glEnd();

	glColor3f(0.3,0.3,0.3);
    glBegin(GL_POLYGON);
		glNormal3f(1.0,0.0,0.0);
		glVertex3f(-width/2,-height/2,position);
		glNormal3f(1.0,0.0,0.0);
		glVertex3f(-width/2,-height/2,position-length);
		glNormal3f(1.0,0.0,0.0);
		glVertex3f(-width/2,height/2,position-length);
		glNormal3f(1.0,0.0,0.0);
		glVertex3f(-width/2,height/2,position);
	glEnd();

	glColor3f(0.3,0.3,0.3);
    glBegin(GL_POLYGON);
		glNormal3f(-1.0,0.0,0.0);
		glVertex3f(width/2,-height/2,position);
		glNormal3f(-1.0,0.0,0.0);
		glVertex3f(width/2,-height/2,position-length);
		glNormal3f(-1.0,0.0,0.0);
		glVertex3f(width/2,height/2,position-length);
		glNormal3f(-1.0,0.0,0.0);
		glVertex3f(width/2,height/2,position);
	glEnd();

}

void initSection(Section * s, float w, float h, float l, float p){

	s->width = w;
	s->height = h;
	s->length = l;
	s->position = p;
}

void initSectionsTab(SectionsTab * st, int sectionNumber){

	st->sectionNumber = sectionNumber;
	for(int i = 0 ; i < sectionNumber ; i ++){

		Section tmp;
		initSection(&tmp, 10, 5, 15, 30 - 15 * i);
		st->tab[i] = tmp;
	}
}

void drawSections(SectionsTab st, Ball b, Racket r){

	for(int i = 0 ; i < st.sectionNumber ; i++){

		drawSection(st.tab[i].width, st.tab[i].height, st.tab[i].length, st.tab[i].position, b, r);
	}
}

void translateSections(SectionsTab * st, float d){

	for(int i = 0; i < st->sectionNumber ; i ++){

		st->tab[i].position += d;
		if(st->tab[i].position > 50){
			st->tab[i].position = 50 - ((st->sectionNumber-1) * 15);
		}
	}
}

void initBall(Ball * b, float x, float y, float z, float vx, float vy, float vz, float r){

	b->radius = r;
	b->x = x;
	b->y = y;
	b->z = z;
	b->vx = vx;
	b->vy = vy;
	b->vz = vz;
	b->isAlive = true;

}

void drawBall(Ball b){

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(b.x, b.y, b.z);
		glScalef(b.radius, b.radius, b.radius);
		glColor3f(1.,0,0);
		drawSphere();
	glPopMatrix();

}

void translateBall(Ball * b, float dx, float dy, float dz, float xlim, float ylim, float zlim){

	b->x += dx;
	b->y += dy;
	b->z += dz;

	if(b->vx >0 && b->x +b->radius > xlim) b->vx *= -1;
	if(b->vx <0 && b->x - b->radius < -xlim) b->vx *= -1;

	if(b->vy > 0 && b->y + b->radius > ylim) b->vy *= -1;
	if(b->vy < 0 && b->y -b->radius < -ylim) b->vy *= -1;

	if(b->vz > 0 && b->z + b->radius > zlim - 20) b->isAlive = false;
	if(b->vz < 0 && b->z -b->radius < -zlim) b->vz *= -1;

}

void translateBallOnRacket(Ball * b, Racket r){


	if(!b->isAlive){

		b->vx = 0;
		b->vy = 0;
		b->vz = 0;

		b->x = r.racketx;
		b->y = r.rackety;
		b->z = r.racketz;
	}
}

void initRacket(Racket * r, float w, float h, float x, float y, float z){

	r->width = w;
	r->height = h;
	r->racketx = x;
	r->rackety = y;
	r->racketz = z;
}


void translateRacket(Racket * r, float dz, float * extRacketPosition){

	if(r->racketz < 25 && dz > 0){
		r->racketz += dz;
	}

	if(r->racketz > 0 && dz < 0){
		r->racketz += dz;
	}

	*extRacketPosition = 30 - r->racketz;
}

void racketCollision(Racket r, Ball * b){

	if(b->vz > 0 && b->z + b->radius > r.racketz){

		if(b->x + b->radius > (r.racketx - r.width/2) && b->x - b->radius < (r.racketx + r.width / 2) && b->y + b->radius > (r.rackety - r.height/2) && b->y - b->radius < (r.rackety + r.height/2)){

			b->vz *= -1;
		}
	}
}

void initObstacle(Obstacle * o, float position, float xlim, float ylim){

	o->height = (float)rand()/(float)(RAND_MAX/(xlim/4.0)) + ylim/4.0;
	o->width = (float)rand()/(float)(RAND_MAX/(ylim/4.0)) + xlim/4.0;

	o->x =(float)rand()/(float)(RAND_MAX/(xlim - o->width/2.0)) - xlim/2 + o->width/2.0;
	o->y = (float)rand()/(float)(RAND_MAX/(ylim - o->height/2.0)) - ylim/2 + o->height/2.0;

	o->z = position;
}

void drawObstacle(Obstacle o){

	//glDepthMask(false);
	glColor4f(0.1,0.1,0.8,0.5);
    glBegin(GL_POLYGON);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(o.x - o.width/2, o.y - o.height/2, o.z);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(o.x + o.width/2, o.y - o.height/2, o.z);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(o.x + o.width/2, o.y + o.height/2, o.z);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(o.x - o.width/2, o.y + o.height/2, o.z);
	glEnd();
	//glDepthMask(true);
}

void initObstaclesTab(ObstaclesTab * ot, int nb,float origin, float xlim, float ylim){

	ot->nb = nb;
	for(int i = 0; i <nb; i++){

		Obstacle tmp;
		ot->tab[i] = tmp;
		initObstacle(&(ot->tab[i]), origin - i * 15, xlim, ylim);
	}
}

void drawObstacles(ObstaclesTab ot){

	for(int i = ot.nb-1; i>=0 ; i--){ //On dessine en décrémentant pour garder la transparence comme il faut

		drawObstacle(ot.tab[i]);
	}
}

void translateObstacle(Obstacle * o, float dz){

	o->z += dz;
}

void translateObstacles(ObstaclesTab * ot, float dz){

	for(int i = 0; i<ot->nb; i++){

		if(ot->tab[i].z < 25){ //On translate si l'obstacle n'a pas atteint la camera

			translateObstacle(&(ot->tab[i]), dz);
		}

		else{

			for(int i = 0; i< ot->nb - 1; i++){

				ot->tab[i] = ot->tab[i+1];
			}

			Obstacle tmp;

			initObstacle(&tmp, 15 - ot->nb * 15, 10, 5);
			ot->tab[ot->nb-1] = tmp;
		}
	}
}

void obstaclesCollision(Ball * b, ObstaclesTab ot){

	for(int i = 0 ; i < ot.nb ; i++){

		if(b->vz < 0 && b->z < ot.tab[i].z){

			if((b->x + b->radius > ot.tab[i].x - ot.tab[i].width/2) && (b->x - b->radius < ot.tab[i].x + ot.tab[i].width/2) && (b->y + b->radius > ot.tab[i].y - ot.tab[i].height/2) && (b->y - b->radius < ot.tab[i].y + ot.tab[i].height/2)){

				b->vz *= -1;
				break;
			}
		}
	}
}



