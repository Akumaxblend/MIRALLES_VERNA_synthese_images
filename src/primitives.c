#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "primitives.h"

void drawRacket(float width, float height, double RacketX, double RacketY){

	glBegin(GL_LINES);

	glVertex2f(RacketX-width/2, RacketY + height/2);
    glVertex2f(RacketX-width/2, RacketY-height/2);

	glVertex2f(RacketX + width/2, RacketY + height/2);
    glVertex2f(RacketX + width/2, RacketY-height/2);

    glVertex2f(RacketX-width/2, RacketY + height/2);
    glVertex2f(RacketX + width/2, RacketY + height/2);

    glVertex2f(RacketX-width/2, RacketY-height/2);
    glVertex2f(RacketX + width/2, RacketY-height/2);

	glEnd();
}