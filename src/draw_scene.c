#include "draw_scene.h"
#include "3D_tools.h"

void drawBase() {

    glColor3f(235/255.0,207/255.0, 52/255.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glScalef(3,3,3);
        drawCircle();
    glPopMatrix();
    glPushMatrix();
        glScalef(2,2,10);
        drawCone();
    glPopMatrix();
    
}

void drawArm() {

    glColor3f(245/255.0,164/255.0,66/255.0);

    glPushMatrix();
        glScalef(1.6,1.6,1.6);
        drawSphere();
    glPopMatrix();
    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(1,1,10);
        drawCone();
        glRotatef(180,1,0,0);
        drawCone();
    glPopMatrix();

}

void drawPan() {

    glColor3f(1,0,0);

    glBegin(GL_LINES);

    glVertex3f(0,0,0);
    glVertex3f(cos(2*M_PI / 3)* 3, sin(2*M_PI / 3)* 3, -5);

    glVertex3f(0,0,0);
    glVertex3f(cos(4 * M_PI / 3)* 3, sin(4 * M_PI / 3)* 3, -5);

    glVertex3f(0,0,0);
    glVertex3f(cos(6 * M_PI / 3) * 3, sin(6 * M_PI / 3) * 3, -5);

    glEnd();

    glColor3f(89/255.0,115/255.0,32/255.0);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
        glTranslatef(0,0,-5);
        glScalef(3,3,3);
        drawCircle();
    glPopMatrix();

}
