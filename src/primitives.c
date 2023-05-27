#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "primitives.h"
#include "3D_tools.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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

void drawSection(int resolution, float width, float height, float length, float position, Ball b, Racket r){

	// int textX = 0;
	// int textY = 0;
	// int textN = 0;

	// unsigned char* loaded;
	// loaded = stbi_load("images/mini.jpg",&textX, &textY, &textN, 0);
	// GLuint texture_walls;
	// glGenTextures(1, &texture_walls);
	// glBindTexture(GL_TEXTURE_2D, texture_walls);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textX, textY, 0, GL_RGB, GL_UNSIGNED_BYTE, loaded);

	// glEnable(GL_TEXTURE_2D);


	GLfloat diffuse_vect[] = {0.7, 0.7, 0.7, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_vect);
	
		for(int i = 0 ; i < resolution ; i++){

			for(int j = 0 ; j < resolution ; j++){
				glBegin(GL_POLYGON);
				glNormal3f(0.0,1.0,0.0);
				//glTexCoord2f(0,0);
				glVertex3f(-width/2 + j* width/resolution,-height/2, position - i*length / resolution);
				glNormal3f(0.0,1.0,0.0);
				//glTexCoord2f(1,0);
				glVertex3f(-width/2 + j*width/resolution,-height/2,position - (i+1)*length/resolution);
				glNormal3f(0.0,1.0,0.0);
				//glTexCoord2f(1,1);
				glVertex3f(-width/2 + (j+1) * width / resolution,-height/2,position - (i+1)*length/resolution);
				glNormal3f(0.0,1.0,0.0);
				//glTexCoord2f(0,1);
				glVertex3f(-width/2 + (j+1) * width / resolution,-height/2, position - i*length / resolution);
				glEnd();

			}
		}
	
	for(int i = 0 ; i < resolution ; i++){

			for(int j = 0 ; j < resolution ; j++){

				glBegin(GL_POLYGON);

					glNormal3f(0.0,-1.0,0.0);
					glVertex3f(-width/2 + j* width/resolution,height/2,position - i*length / resolution);
					glNormal3f(0.0,-1.0,0.0);
					glVertex3f(-width/2 + j* width/resolution,height/2,position - (i+1)*length/resolution);
					glNormal3f(0.0,-1.0,0.0);
					glVertex3f(-width/2 + (j+1) * width / resolution,height/2,position - (i+1)*length/resolution);
					glNormal3f(0.0,-1.0,0.0);
					glVertex3f(-width/2 + (j+1) * width / resolution,height/2,position - i*length / resolution);

				glEnd();

			}

	}

    
	for(int i = 0 ; i < resolution ; i++){

			for(int j = 0 ; j < resolution ; j++){
				glBegin(GL_POLYGON);
					glNormal3f(1.0,0.0,0.0);
					glVertex3f(-width/2,-height/2 + i*height/resolution,position-j*length/resolution);
					glNormal3f(1.0,0.0,0.0);
					glVertex3f(-width/2,-height/2 + i*height/resolution,position-(j+1)*length/resolution);
					glNormal3f(1.0,0.0,0.0);
					glVertex3f(-width/2,-height/2 + (i+1) * height / resolution,position-(j+1)*length/resolution);
					glNormal3f(1.0,0.0,0.0);
					glVertex3f(-width/2,-height/2 + (i+1) * height / resolution,position-j*length/resolution);
				glEnd();
			}		
	}
	
	for(int i = 0 ; i < resolution ; i++){

			for(int j = 0 ; j < resolution ; j++){
				glBegin(GL_POLYGON);
					glNormal3f(-1.0,0.0,0.0);
					glVertex3f(width/2,-height/2 + i*height/resolution,position-j*length/resolution);
					//glNormal3f(-1.0,0.0,0.0);
					glVertex3f(width/2,-height/2 + i*height/resolution,position-(j+1)*length/resolution);
					//glNormal3f(-1.0,0.0,0.0);
					glVertex3f(width/2,-height/2 + (i+1) * height / resolution,position-(j+1)*length/resolution);
					//glNormal3f(-1.0,0.0,0.0);
					glVertex3f(width/2,-height/2 + (i+1) * height / resolution,position-j*length/resolution);
				glEnd();
			}
	}
	// glDisable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, 0);
	// glDeleteTextures(1, &texture_walls);
	// stbi_image_free(loaded);

}

void initSection(Section * s, float w, float h, float l, float p){

	s->width = w;
	s->height = h;
	s->length = l;
	s->position = p;
}

void initSectionsTab(SectionsTab * st, int sectionNumber){

	st->sectionNumber = sectionNumber;
	st->nb_spawned = sectionNumber;
	for(int i = 0 ; i < sectionNumber ; i ++){

		Section tmp;
		initSection(&tmp, 10, 5, 15, 30 - 15 * i);
		st->tab[i] = tmp;
	}
}

void drawSections(int resolution, SectionsTab st, Ball b, Racket r){

	for(int i = 0 ; i < st.sectionNumber ; i++){

		drawSection(resolution, st.tab[i].width, st.tab[i].height, st.tab[i].length, st.tab[i].position, b, r);
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
	b->isAlive = false;
	b->lives = 5;

}

void drawBall(Ball b){

	glMatrixMode(GL_MODELVIEW);
	GLfloat diffuse_vect[] = {0.0, 1.0, 0, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_vect);
	glColor3f(1.0,0.0,1.0);
	glPushMatrix();
		glTranslatef(b.x, b.y, b.z);
		glScalef(b.radius, b.radius, b.radius);
		drawSphere();
	glPopMatrix();

}

void initRacket(Racket * r, float w, float h, float x, float y, float z){

	r->width = w;
	r->height = h;
	r->racketx = x;
	r->rackety = y;
	r->racketz = z;
}

void initBonus(Bonus * b, char * type, float z, float vz, float xlim, float ylim){

	b->type = type;
	b->z = z;
	b->vz = vz;

	b->x = rand() % (2 * (int)xlim - 2) - xlim + 1;
	b->y = rand() % (2 * (int)ylim - 2) - ylim + 1;

}

void initObstacle(Obstacle * o, float position, float xlim, float ylim){

	o->height = (float)rand()/(float)(RAND_MAX/(xlim/4.0)) + ylim/4.0;
	o->width = (float)rand()/(float)(RAND_MAX/(ylim/4.0)) + xlim/4.0;

	o->x =(float)rand()/(float)(RAND_MAX/(xlim - o->width/2.0)) - xlim/2 + o->width/2.0;
	o->y = (float)rand()/(float)(RAND_MAX/(ylim - o->height/2.0)) - ylim/2 + o->height/2.0;

	o->z = position;
}

void initObstaclesTab(ObstaclesTab * ot, int nb,float origin, float xlim, float ylim){

	ot->nb = nb;
	ot->nb_spawned = nb;
	for(int i = 0; i <nb; i++){

		Obstacle tmp;
		ot->tab[i] = tmp;
		initObstacle(&(ot->tab[i]), origin - i * 15, xlim, ylim);
	}
}
void drawObstacle(Obstacle o){

	//glDepthMask(false);

	int textX = 0;
	int textY = 0;
	int textN = 0;

	unsigned char* loaded;
	loaded = stbi_load("images/stone.png",&textX, &textY, &textN, 0);
	GLuint texture_walls;
	glGenTextures(1, &texture_walls);
	glBindTexture(GL_TEXTURE_2D, texture_walls);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textX, textY, 0, GL_RGBA, GL_UNSIGNED_BYTE, loaded);

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0,1.0,1.0,0.95);
	GLfloat diffuse_vect[] = {1.0, 1.0, 1.0, 0.8};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_vect);
    glBegin(GL_POLYGON);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0,0);
		glVertex3f(o.x - o.width/2, o.y - o.height/2, o.z);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(1,0);
		glVertex3f(o.x + o.width/2, o.y - o.height/2, o.z);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(o.x + o.width/2, o.y + o.height/2, o.z);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0,1.0);
		glVertex3f(o.x - o.width/2, o.y + o.height/2, o.z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texture_walls);
	stbi_image_free(loaded);
	//glDepthMask(true);
}

void drawObstacles(ObstaclesTab ot){

	for(int i = ot.nb-1; i>=0 ; i--){ //On dessine en décrémentant pour garder la transparence comme il faut

		drawObstacle(ot.tab[i]);
	}
}

void drawGUI(Ball b){

	for(int i = 0; i< b.lives ; i++){

		glPushMatrix();
			glTranslatef(-0.8 + i*0.15, -0.5, 29);
			glScalef(0.05,0.05,0.05);
			glColor3f(0,1.0,0);
			drawSphere();
		glPopMatrix();
	}
}

void drawBonus(Bonus b){

	if(strcmp(b.type, "life") == 0){

		glPushMatrix();
			glTranslatef(b.x, b.y, b.z);
			glScalef(0.15,0.15,0.15);
			glColor3f(0,1.0,0);
			drawSphere();
		glPopMatrix();
	}
	else if (strcmp(b.type, "glue") == 0){
		glPushMatrix();
			glTranslatef(b.x, b.y, b.z);
			glScalef(0.15,0.15,0.15);
			glColor3f(1.0,1.0,1.0);
			drawSphere();
		glPopMatrix();
	}
}


