#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "primitives.h"
#include "3D_tools.h"
#include "draw_scene.h"

#define SPEED 0.1
#define MAX_SECTION_NUMBER 20


/* Window properties */
static unsigned int WINDOW_WIDTH = 900;
static unsigned int WINDOW_HEIGHT = 500;
static const char WINDOW_TITLE[] = "Super projet Mirale Vernat";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */
static int flag_animate_rot_arm = 0;

float animTime = 1;

double RacketX = 0;
double RacketY = 0;
int fov = 60;
float racketDist = 5;
int sectionNumber = 10;
float speed = 0.1;
float racketSpeed = 1;
int movingRacket = -1;


//initialisation de la balle

Ball ball;
static const float GL_VIEW_SIZE = 2.;

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
			// case GLFW_KEY_R :
			// 	flag_animate_rot_arm = 1-flag_animate_rot_arm;
			// 	break;
			// case GLFW_KEY_T :
			// 	flag_animate_rot_scale = 1-flag_animate_rot_scale;
			// 	break;
			// case GLFW_KEY_KP_9 :
			// 	if(dist_zoom<100.0f) dist_zoom*=1.1;
			// 	printf("Zoom is %f\n",dist_zoom);
			// 	break;
			// case GLFW_KEY_KP_3 :
			// 	if(dist_zoom>1.0f) dist_zoom*=0.9;
			// 	printf("Zoom is %f\n",dist_zoom);
			// 	break;
			case GLFW_KEY_UP :
				if (phy>2) phy -= 2;
				printf("Phy %f\n",phy);
				break;
			case GLFW_KEY_DOWN :
				if (phy<=88.) phy += 2;
				printf("Phy %f\n",phy);
				break;
			case GLFW_KEY_LEFT :
				theta -= 5;
				break;
			case GLFW_KEY_RIGHT :
				theta += 5;
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
		ball.vz = -20;
		speed = SPEED;
	}

}


// void init(void) 
// {
//    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat mat_shininess[] = { 50.0 };
//    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
//    glClearColor (0.0, 0.0, 0.0, 0.0);
//    glShadeModel (GL_SMOOTH);

//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_DEPTH_TEST);
// }

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
	glEnable(GL_DEPTH_TEST);

	//Random seed initialisation

	srand(time(NULL));

	//initialisation des sections

	SectionsTab sections;
	initSectionsTab(&sections, sectionNumber);

	//initialisation de la raquette 

	Racket racket;
	initRacket(&racket, 1, 1, RacketX, RacketY, 30-racketDist);

	initBall(&ball, 0, 0, 0, 1, 1, -20, 0.25);


	//tests sur obstacles 

	// Obstacle obs;
	// initObstacle(&obs, 15.0, 10.0, 5.0);

	ObstaclesTab ot;
	initObstaclesTab(&ot, 10, 15, 10, 5);
	// Obstacle boss;
	// initObstacle(&boss, -MAX_SECTION_NUMBER * 15, 0, 0);
	// boss.x = 0;
	// boss.y = 0;
	// boss.height = 5;
	// boss.width = 10;
	// ot.tab[ot.nb] = boss;


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
		

		GLfloat light_position_racket[] = {racket.racketx,racket.rackety,racket.racketz,1.0};
		GLfloat light_position_ball[] = {ball.x, ball.y, ball.z, 1.0};

		GLfloat light_color_racket[] = {1.0,2.0,1.0};
		GLfloat light_color_ball[] = {3.0, 0.0, 3.0};
	


		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0,0.0,0.1,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable (GL_BLEND); 
   	 	glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();	

		
		//Light initialisation and positionning
		glLightfv(GL_LIGHT0, GL_POSITION, light_position_racket); 
		glLightfv(GL_LIGHT1, GL_POSITION, light_position_ball);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.3);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.3);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color_racket);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color_ball);
		//glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		

		/* Scene rendering */

		drawRacket(racket.width, racket.height, racket.racketx, racket.rackety, racket.racketz);
		drawBall(ball);	
		drawGUI(ball);
		
		glEnable(GL_LIGHTING);
		drawSections(10, sections, ball, racket);
		drawObstacles(ot);
		glDisable(GL_LIGHTING);
		
		
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */

		double elapsedTime = glfwGetTime() - startTime;

		animTime += elapsedTime * flag_animate_rot_arm;

		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			//glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}

		/* Animate scenery */

		
		obstaclesCollision(&ball, ot);
		racketCollision(racket, &ball);
		translateBall(&ball, ball.vx * elapsedTime, ball.vy * elapsedTime, ball.vz * elapsedTime, 5, 2.5, 50);
		translateRacket(&racket, -movingRacket * speed, &racketDist);

		if(!ball.isAlive){

			translateBallOnRacket(&ball, racket);
			speed = 0;
		}
		translateSections(&sections, animTime * speed, MAX_SECTION_NUMBER);
		translateObstacles(&ot, animTime*speed, MAX_SECTION_NUMBER);
	}

	glfwTerminate();
	return 0;
}

