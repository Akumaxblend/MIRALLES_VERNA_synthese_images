#include "3D_tools.h"

/* Camera parameters and functions */
float theta = -90.0f; // Angle between x axis and viewpoint
float phy = 0.1f; // Angle between z axis and viewpoint
float dist_zoom = 30.0f; // Distance between origin and viewpoint

void setCamera() {
	gluLookAt(dist_zoom*cos(toRad(theta))*sin(toRad(phy)),
			  dist_zoom*sin(toRad(theta))*sin(toRad(phy)),
			  dist_zoom*cos(toRad(phy)),
			  0.0,0.0,0.0,
			  0.0,0.0,1.0);
}

/* Convert degree to radians */
float toRad(float deg) {
	return deg*M_PI/180.0f;
}


void drawSquare() {
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,-0.5,0.0);
		glVertex3f(0.5,0.5,0.0);
		glVertex3f(-0.5,0.5,0.0);
	glEnd();
}

void drawCircle() {
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0,0.0,0.0);
		float step_rad = 2*M_PI/(float)NB_SEG_CIRCLE;
		for(int i=0;i<=NB_SEG_CIRCLE;i++) {
			glVertex3f(cos(i*step_rad),sin(i*step_rad),0.0f);
		}
	glEnd();
}

void drawCone() {
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0,0.0,1.0);
		float step_rad = 2*M_PI/(float)NB_SEG_CIRCLE;
		for(int i=0;i<=NB_SEG_CIRCLE;i++) {
			glVertex3f(cos(i*step_rad),sin(i*step_rad),0.0f);
		}
	glEnd();
}

void drawSphere() {
	gluSphere(gluNewQuadric(),1.0,NB_SEG_CIRCLE,NB_SEG_CIRCLE);
}

void drawOrigin(){

	glBegin(GL_LINES);

	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);

	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,1,0);

	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,1);

	glEnd();
}

void drawCylinder()
{
	drawCircle();
	glPushMatrix();
		glTranslatef(0,0,1.0);
		drawCircle();
	glPopMatrix();
	float step_rad = 2*M_PI/(float)NB_SEG_CIRCLE;
	for(int i = 0; i< NB_SEG_CIRCLE; i++)
	{
		glBegin(GL_POLYGON);
		glVertex3f(cos(i*step_rad), sin(i*step_rad),0);
		glVertex3f(cos(i*step_rad), sin(i*step_rad),1);
		glVertex3f(cos((i+1)*step_rad), sin((i+1)*step_rad),1);
		glVertex3f(cos((i+1)*step_rad), sin((i+1)*step_rad),0);
		glEnd();
	}
}

void drawGlue()
{
	glPushMatrix();
		glRotatef(90, -1.0,0,0.3);
		glColor3f(1.0,1.0,1.0);
		drawCylinder();
		glPushMatrix();
			glTranslatef(0,0,1.0);
			glScalef(0.9,0.9,4.0);
			glColor3f(1.0,1.0,0);
			drawCylinder();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,0,5.0);
			glScalef(1.0,1.0,0.5);
			glColor3f(1.0,1.0,1.0);
			drawCylinder();
		glPopMatrix();		
	glPopMatrix();
}

void drawLife()
{
	glColor3f(1.0,1.0,1.0);
	drawCylinder();
	glPushMatrix();
		glTranslatef(0,0,1.01);
		glPushMatrix();
			glScalef(0.3,0.9,1.0);
			glColor3f(0,1.0,0);
			drawSquare();
		glPopMatrix();
		glPushMatrix();
			glScalef(0.9,0.3,1.0);
			drawSquare();
		glPopMatrix();
	glPopMatrix();
}