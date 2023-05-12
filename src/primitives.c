#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "primitives.h"


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

void initSection(Section s, float w, float h, float l, float p){

	s.width = w;
	s.height = h;
	s.length = l;
	s.position = p;
}

void initSectionsTab(SectionsTab st, int sectionNumber){

	st.sectionNumber = sectionNumber;
	for(int i = 0 ; i < sectionNumber ; i ++){

		Section tmp;
		initSection(tmp, 15, 10, 15, 30 - 15 * i);
	}
}

void drawSections(SectionsTab st){

	for(int i = 0 ; i < st.sectionNumber ; i++){

		drawSection(st.tab[i].width, st.tab[i].height, st.tab[i].length, st.tab[i].position);
	}
}

void translateSections(SectionsTab st, float d){

	for(int i = 0; i < st.sectionNumber ; i ++){

		st.tab[i].position += d;
	}
}

