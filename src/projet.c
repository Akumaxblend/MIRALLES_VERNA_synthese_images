#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "primitives.h"
#include "physic.h"
#include "3D_tools.h"
#include "draw_scene.h"

#define SPEED 0.3
#define RACKET_SPEED 1
#define MAX_SECTION_NUMBER 20


/* Window properties */
static unsigned int WINDOW_WIDTH = 900;
static unsigned int WINDOW_HEIGHT = 500;
static const char WINDOW_TITLE[] = "Super projet Mirale Vernat";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */

float animTime = 1;

double RacketX = 0;
double RacketY = 0;
int fov = 60;
float racketDist = 5;
int sectionNumber = 10;
float speed = 0.1;
float racketSpeed = 1;
int movingRacket = -1;

// Déclaration des acteurs
Ball ball;
SectionsTab sections;
Racket racket;
Bonus bonus;
ObstaclesTab ot;

//Déclaration des variables d'illumination

GLfloat light_color_racket[] = {1.0,2.0,1.0};
GLfloat light_color_ball[] = {3.0, 0.0, 3.0};

GLfloat light_position_racket[] = {0,0,25, 1.0};
GLfloat light_position_ball[] = {0, 0, 25, 1.0};

static const float GL_VIEW_SIZE = 2.;

void initActors(){

	//Random seed initialisation
	srand(time(NULL));

	//initialisation de la balle
	initBall(&ball, 0, 0, 0, 0.0, 0.0, 0.0, 0.25);

	//initialisation des sections
	initSectionsTab(&sections, sectionNumber);

	//initialisation de la raquette 
	initRacket(&racket, 1, 1, RacketX, RacketY, 30-racketDist);

	//initialisation du bonus
	initBonus(&bonus, "life", 20, speed, 5, 2.5);

	//initialisation des obstacles
	initObstaclesTab(&ot, 10, 15, 10, 5);

}

void drawUnlitScene(){

	drawRacket(racket.width, racket.height, racket.racketx, racket.rackety, racket.racketz);
	drawBall(ball);	
	drawGUI(ball);
	drawBonus(bonus);

	
}

void drawLitScene(){

	 //On active le lighting de la scene pour les sections et les obstacles uniquement
	drawSections(10, sections, ball, racket);
	drawObstacles(ot);
}

void initLighting(){

	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_racket); 
	glLightfv(GL_LIGHT1, GL_POSITION, light_position_ball);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.3);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.3);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color_racket);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color_ball);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void updateLighting(Ball ball, Racket racket){

	light_position_racket[0] = racket.racketx;
	light_position_racket[1] = racket.rackety;
	light_position_racket[2] = racket.racketz;

	light_position_ball[0] = ball.x;
	light_position_ball[1] = ball.y;
	light_position_ball[2] = ball.z;

}

void testCollisions(){

	obstaclesCollision(&ball, ot);
	racketCollision(racket, &ball);
	bonusCollision(&bonus, racket, &ball);
}

void translateActors(float time){


	translateBall(&ball, ball.vx * animTime, ball.vy * animTime, ball.vz * animTime, 5, 2.5, 50);
	translateRacket(&racket, -movingRacket * racketSpeed, &racketDist);
	
	if(!ball.isAlive){
		translateBallOnRacket(&ball, racket);
		speed = 0;
		racketSpeed = 0;
	}

	translateSections(&sections, animTime * speed, MAX_SECTION_NUMBER);
	translateObstacles(&ot, animTime*speed, MAX_SECTION_NUMBER);
	translateBonus(&bonus, 2 * speed * animTime);
}
/* Error handling function */
void onError(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / (float) height;

	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,aspectRatio,Z_NEAR,Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}


void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_A :
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_L :
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case GLFW_KEY_P :
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &RacketX, &RacketY);

	 if (aspectRatio < 1) { //Conversion adéquate pour le repère affichage
            RacketX = (GL_VIEW_SIZE*RacketX/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2)*(racketDist*tan(toRad(fov)/2.0));
			RacketY = ((GL_VIEW_SIZE/2 - GL_VIEW_SIZE*RacketY/(float)WINDOW_HEIGHT)/aspectRatio)*(racketDist*tan(toRad(fov)/2.0));            
        } 
        else {
            RacketX = ((GL_VIEW_SIZE*RacketX/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2)*aspectRatio)*(racketDist*tan(toRad(fov)/2.0));
			RacketY = (GL_VIEW_SIZE/2 - GL_VIEW_SIZE*RacketY/(float)WINDOW_HEIGHT)*(racketDist*tan(toRad(fov)/2.0));
            
        }

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        
		if(movingRacket == -1){
			movingRacket = 1;
		}
    }
	else movingRacket = -1;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !ball.isAlive){

		ball.isAlive = true;
		ball.vz = -2;
		speed = SPEED;
		racketSpeed = RACKET_SPEED;
	}

}

int main(int argc, char** argv)
{
	/* GLFW initialisation */

	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);
	glPointSize(5.0);

	//On initialise les acteurs déclarés plus tôt
	initActors();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && ball.lives > 0)
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		//On update les coordonnées de la raquette

		if(RacketX + racket.width/2 < 5 && RacketX - racket.width / 2 > -5){
		racket.racketx = RacketX;
		}
		if(RacketY + racket.height / 2 < 2.5 && RacketY - racket.height/2 > -2.5){
			racket.rackety = RacketY;
		}

		//On active le lighting
		initLighting();

		//On met à jour la position du lighting
		updateLighting(ball, racket);

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0,0.0,0.1,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable (GL_BLEND); 
   	 	glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();	
		
		
		/* Scene rendering */
		
		glDisable(GL_LIGHTING);
		drawUnlitScene();
		glEnable(GL_LIGHTING);
		drawLitScene();
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */

		double elapsedTime = glfwGetTime() - startTime;

		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			//glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}

		/* Animate scenery */

		//On test d'éventuelles collisions
		testCollisions();

		//On déplace les différents acteurs dans la scène en fonction du facteur voulu
		translateActors(animTime);
	}

	glfwTerminate();
	return 0;
}

