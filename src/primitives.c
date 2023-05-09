#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "primitives.h"

void drawRacket(float width, float height, double RacketX, double RacketY, float d){

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

    glBegin(GL_TRIANGLE_FAN);
		glVertex3f(height/2,-width/2,position);
		glVertex3f(height/2,-width/2,position-length);
		glVertex3f(height/2,width/2,position-length);
		glVertex3f(height/2,width/2,position);
	glEnd();

    glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-height/2,-width/2,position);
		glVertex3f(-height/2,-width/2,position-length);
		glVertex3f(-height/2,width/2,position-length);
		glVertex3f(-height/2,width/2,position);
	glEnd();

    glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-height/2,-width/2,position);
		glVertex3f(-height/2,-width/2,position-length);
		glVertex3f(height/2,-width/2,position-length);
		glVertex3f(height/2,-width/2,position);
	glEnd();

    glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-height/2,width/2,position);
		glVertex3f(-height/2,width/2,position-length);
		glVertex3f(height/2,width/2,position-length);
		glVertex3f(height/2,width/2,position);
	glEnd();

}