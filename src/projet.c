#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "3D_tools.h"
#include "draw_scene.h"
#include "init.h"
#include "draws.h"
#define SPEED 0.3
#define RACKET_SPEED 1

/* Window properties */
static unsigned int WINDOW_WIDTH = 900;
static unsigned int WINDOW_HEIGHT = 500;

static const char WINDOW_TITLE[] = "Super projet Mirale Vernat";
static float aspectRatio = 1.0;
static const float GL_VIEW_SIZE = 2.;

Game game;    

GLfloat light_color_racket[] = {1.0,2.0,1.0};
GLfloat light_color_ball[] = {3.0, 0.0, 3.0};
GLfloat light_position_racket[] = {0,0,25, 1.0};
GLfloat light_position_ball[] = {0, 0, 25, 1.0};

double RacketX = 0;
double RacketY = 0;
float racketDist = 5;
float speed = SPEED;
int movingRacket = -1;
float racketSpeed = 1;
float animTime = 1;
int fov = 60;
static int focus = 0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

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

void getCoords(double *x, double *y, float dist) {
    if (aspectRatio < 1) {
        *x = (GL_VIEW_SIZE*(*x)/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2)*(dist*tan(toRad(fov)/2.0));
        *y = ((GL_VIEW_SIZE/2 - GL_VIEW_SIZE*(*y)/(float)WINDOW_HEIGHT)/aspectRatio)*(dist*tan(toRad(fov)/2.0));
    }
    else {
        *x = ((GL_VIEW_SIZE*(*x)/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2)*aspectRatio)*(dist*tan(toRad(fov)/2.0));
        *y = (GL_VIEW_SIZE/2 - GL_VIEW_SIZE*(*y)/(float)WINDOW_HEIGHT)*(dist*tan(toRad(fov)/2.0));
    }
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
            case GLFW_KEY_SPACE :
                if (focus == 3) {
                    focus = 5;
                }
                else {
                    focus = 3;
                }
                break;
			default: fprintf(stdout,"Touche non gérée (%d)\n",key);
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &RacketX, &RacketY);
    getCoords(&RacketX, &RacketY, racketDist);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        
		if(movingRacket == -1){
			movingRacket = 1;
		}
    }
	else movingRacket = -1;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !game.ball.isAlive && !game.menu_debut.on){

		game.ball.isAlive = true;
		game.ball.vz = -2;
		speed = SPEED;
		racketSpeed = RACKET_SPEED;
	}

    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);
    getCoords(&x, &y, 30);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && game.menu_debut.on && inButton(&game.menu_debut.play, x, y)) {
            game.menu_debut.on = false;
            focus = 3;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && game.menu_debut.on && inButton(&game.menu_debut.quit, x, y)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && game.menu_fin.on && inButton(&game.menu_fin.play, x, y)) {
        game.menu_fin.on = false;
        focus = 4;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && game.menu_fin.on && inButton(&game.menu_fin.quit, x, y)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char** argv)
{
    //déclaration des variables nécessaires
    
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
	initActors(&game, speed);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
        
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();
		
		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0,0.0,0.1,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable (GL_BLEND); 
   	 	glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();	
        //à mettre sous forme de fonction dans un fichier à part et ça fera l'affaire        
        switch(focus) {
            case 0 : 
                glDisable(GL_LIGHTING);
                drawMenu(&game.menu_debut);
                break;
            case 1 :
                glDisable(GL_LIGHTING);
                glDisable(GL_DEPTH_TEST);
                drawMenu(&game.menu_fin);
                break;
            case 3 :
                playGame(&game, light_position_racket, light_position_ball, light_color_racket, light_color_ball, animTime, movingRacket, &racketDist, speed, RacketX, RacketY, racketSpeed);
                break;
            case 4 :
                replayGame(&game, light_position_racket, light_position_ball, light_color_racket, light_color_ball, animTime, movingRacket, &racketDist, speed, RacketX, RacketY, racketSpeed);
                focus = 3;
                break;
            case 5 :
                pauseGame(&game, light_position_racket, light_position_ball, light_color_racket, light_color_ball);
                break;
        }
		
		/* Scene rendering */
        if (game.ball.lives == 0) {
            game.menu_fin.on = true;
            focus = 1;
        }
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
	}

	glfwTerminate();
	return 0;
}

