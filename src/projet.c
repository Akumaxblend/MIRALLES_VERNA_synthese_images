// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>
// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include "primitives.h"
// #include "3D_tools.h"

// /* Window properties */
// static const unsigned int WINDOW_WIDTH = 800;
// static const unsigned int WINDOW_HEIGHT = 600;
// static const char WINDOW_TITLE[] = "Super Projet Mirale Vernat";
// static float aspectRatio = 1.0;

// /*Variables definition*/

// double RacketX = 0;
// double RacketY = 0;

// int effectiveWidth = WINDOW_WIDTH;
// int effectiveHeight = WINDOW_HEIGHT;


// /* Minimal time wanted between two images */
// static const double FRAMERATE_IN_SECONDS = 1. / 30.;

// /* Virtual windows space */
// // Space is defined in interval -1 and 1 on x and y axes
// static const float GL_VIEW_SIZE = 2.;

// /* Error handling function */
// void onError(int error, const char* description)
// {
// 	fprintf(stderr, "GLFW Error: %s\n", description);
// }

// void onWindowResized(GLFWwindow* window, int width, int height)
// {
// 	aspectRatio = (float) width / (float) height;

// 	glViewport(0, 0, width, height);
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();

// 	if( aspectRatio > 1)
// 	{
// 		gluOrtho2D(
// 		-GL_VIEW_SIZE / 2. * aspectRatio, GL_VIEW_SIZE / 2. * aspectRatio,
// 		-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
// 	}
// 	else
// 	{
// 		gluOrtho2D(
// 		-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
// 		-GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio);
// 	}
// 	glMatrixMode(GL_MODELVIEW);

// }

// void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
// 	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
// 		glfwSetWindowShouldClose(window, GLFW_TRUE);
// 	}
// }
// static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
// {
// 	glfwGetCursorPos(window, &RacketX, &RacketY);

// 	 if (aspectRatio < 1) { //Conversion adéquate pour le repère affichage
//             RacketX = GL_VIEW_SIZE*RacketX/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2;
// 			RacketY = (GL_VIEW_SIZE/2 - GL_VIEW_SIZE*RacketY/(float)WINDOW_HEIGHT)/aspectRatio;            
//         } 
//         else {
//             RacketX = (GL_VIEW_SIZE*RacketX/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2)*aspectRatio;
// 			RacketY = (GL_VIEW_SIZE/2 - GL_VIEW_SIZE*RacketY/(float)WINDOW_HEIGHT);
            
//         }
// }

// int main(int argc, char** argv) 
// {
// 	/* GLFW initialisation */
// 	GLFWwindow* window;
// 	if (!glfwInit()) return -1;

// 	/* Callback to a function if an error is rised by GLFW */
// 	glfwSetErrorCallback(onError);

// 	/* Create a windowed mode window and its OpenGL context */
// 	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
// 	if (!window)
// 	{
// 		// If no context created : exit !
// 		glfwTerminate();
// 		return -1;
// 	}

// 	/* Make the window's context current */
// 	glfwMakeContextCurrent(window);

// 	glfwSetWindowSizeCallback(window,onWindowResized);
// 	glfwSetKeyCallback(window, onKey);
// 	glfwSetCursorPosCallback(window, cursor_position_callback);

// 	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

// 	glPointSize(4.0);

// 	/* Loop until the user closes the window */
// 	while (!glfwWindowShouldClose(window))
// 	{
// 		/* Get time (in second) at loop beginning */
// 		double startTime = glfwGetTime();

// 		/* Render here */

// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		glMatrixMode(GL_MODELVIEW);
// 		glLoadIdentity();
// 		setCamera();


// 		drawRacket(0.1,0.1, RacketX, RacketY);
// 		drawSection();

// 		/* Swap front and back buffers */
// 		glfwSwapBuffers(window);

// 		/* Poll for and process events */
// 		glfwPollEvents();

// 		/* Elapsed time computation from loop begining */
// 		double elapsedTime = glfwGetTime() - startTime;
// 		/* If to few time is spend vs our wanted FPS, we wait */
// 		if(elapsedTime < FRAMERATE_IN_SECONDS) 
// 		{
// 			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
// 		}
// 	}

// 	glfwTerminate();
// 	return 0;
// }

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "primitives.h"
#include "3D_tools.h"
#include "draw_scene.h"


/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "TD04 Ex01";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */
static int flag_animate_rot_scale = 0;
static int flag_animate_rot_arm = 0;

float animTime = 1;

double RacketX = 0;
double RacketY = 0;

/* Error handling function */
void onError(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,aspectRatio,Z_NEAR,Z_FAR);
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
			case GLFW_KEY_R :
				flag_animate_rot_arm = 1-flag_animate_rot_arm;
				break;
			case GLFW_KEY_T :
				flag_animate_rot_scale = 1-flag_animate_rot_scale;
				break;
			case GLFW_KEY_KP_9 :
				if(dist_zoom<100.0f) dist_zoom*=1.1;
				printf("Zoom is %f\n",dist_zoom);
				break;
			case GLFW_KEY_KP_3 :
				if(dist_zoom>1.0f) dist_zoom*=0.9;
				printf("Zoom is %f\n",dist_zoom);
				break;
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

int main(int argc, char** argv)
{
	/* GLFW initialisation */
	int angle = 30;

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

	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	glPointSize(5.0);
	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.2,0.0,0.0,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();	
		 

		/* Scene rendering */

		drawOrigin();
		drawSection(15.0,10.0,15.0,30);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */

		double elapsedTime = glfwGetTime() - startTime;

		animTime += elapsedTime * flag_animate_rot_arm;
		printf ("%f \n",animTime);
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}

		/* Animate scenery */
	}

	glfwTerminate();
	return 0;
}

