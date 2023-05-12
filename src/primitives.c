#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "primitives.h"
#include "3D_tools.h"


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

void drawSection(float width, float height, float length, float position){

	glColor3f(0.1,0.1,0.1);
    glBegin(GL_POLYGON);
		glVertex3f(-width/2,-height/2,position);
		glVertex3f(-width/2,-height/2,position-length);
		glVertex3f(width/2,-height/2,position-length);
		glVertex3f(width/2,-height/2,position);
	glEnd();

	glColor3f(0.5,0.5,0.5);
    glBegin(GL_POLYGON);
		glVertex3f(-width/2,height/2,position);
		glVertex3f(-width/2,height/2,position-length);
		glVertex3f(width/2,height/2,position-length);
		glVertex3f(width/2,height/2,position);
	glEnd();

	glColor3f(0.3,0.3,0.3);
    glBegin(GL_POLYGON);
		glVertex3f(-width/2,-height/2,position);
		glVertex3f(-width/2,-height/2,position-length);
		glVertex3f(-width/2,height/2,position-length);
		glVertex3f(-width/2,height/2,position);
	glEnd();

	glColor3f(0.3,0.3,0.3);
    glBegin(GL_POLYGON);
		glVertex3f(width/2,-height/2,position);
		glVertex3f(width/2,-height/2,position-length);
		glVertex3f(width/2,height/2,position-length);
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

void drawSections(SectionsTab st){

	for(int i = 0 ; i < st.sectionNumber ; i++){

		drawSection(st.tab[i].width, st.tab[i].height, st.tab[i].length, st.tab[i].position);
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

	//if(b->vz > 0 && b->z + b->radius > zlim) b->vz *= -1;
	if(b->vz < 0 && b->z -b->radius < -zlim) b->vz *= -1;

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

		if(b->x > (r.racketx - r.width/2) && b->x < (r.racketx + r.width / 2) && b->y > (r.rackety - r.height/2) && b->y < (r.rackety + r.height/2)){

			b->vz *= -1;
		}
	}
}




