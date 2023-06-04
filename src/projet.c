#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "3D_tools.h"
#include "draw_scene.h"
#include "primitives.h"
#include "physic.h"

#define SPEED 0.3
#define MAX_SECTION_NUMBER 10
#define RACKET_SPEED 0.3

/* Window properties */
static unsigned int WINDOW_WIDTH = 900;
static unsigned int WINDOW_HEIGHT = 500;

static const char WINDOW_TITLE[] = "The IMAC light corridor - MIRALLES VERNA";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

Ball ball;
SectionsTab sections;
Racket racket;
Bonus bonus;
Obstacle boss; //Le paneau de fin : il a la classe obstacle mais ne sert que de trigger avec la raquette
ObstaclesTab ot;
Menu menu_debut;
Menu menu_fin;

double RacketX = 0;
double RacketY = 0;
float racketDist = 5;
int sectionNumber = 10;
float speed = SPEED;
float racketSpeed = 1;
int movingRacket = -1;
float animTime = 1;

GLfloat light_color_racket[] = {1.0,2.0,1.0};
GLfloat light_color_ball[] = {3.0, 0.0, 3.0};
GLfloat light_position_racket[] = {0,0,25, 1.0};
GLfloat light_position_ball[] = {0, 0, 25, 1.0};

/* IHM flag */

int fov = 60;
static int focus = 0;
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

void initActors()
{
	//Random seed initialisation
	srand(time(NULL));
	//initialisation de la balle
	initBall(&ball, 0, 0, 0, 0.0, 0.0, 0.0, 0.25);
	ball.isAlive = false;
	//initialisation des sections
	initSectionsTab(&sections, sectionNumber);
	//initialisation de la raquette 
	initRacket(&racket, 1, 1, RacketX, RacketY, 30-racketDist);
	//initialisation du bonus
	initBonus(&bonus, "life", 20, speed, 5, 2.5);
	//initialisation des obstacles
	initObstaclesTab(&ot, 10, 15, 10, 5);
	initObstacle(&boss, 46-MAX_SECTION_NUMBER*15 - 10 * 15, 0.01,0.01); //On initialise le boss à la distance voulue pour la victoire
	boss.width = 10; //On se débarasse du facteur aléatoire généré par l'initialisation
	boss.height = 5;
    //attention les dimensions des menus sont à revoir car beaucoup trop grandes si on veut y mettre des textures
    //initialisation du menu de debut
    initMenu(&menu_debut, 100, 100, "debut");
    menu_debut.on = true;
    //initialisation du menu de fin
    initMenu(&menu_fin, 100, 100, "fin_defaite");
    menu_fin.on = false;
}

void initLighting()
{
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

void updateLighting(Ball ball, Racket racket)
{
	light_position_racket[0] = racket.racketx;
	light_position_racket[1] = racket.rackety;
	light_position_racket[2] = racket.racketz;

	light_position_ball[0] = ball.x;
	light_position_ball[1] = ball.y;
	light_position_ball[2] = ball.z;
}

void updateRacket(Racket *racket)
{
    if(RacketX + racket->width/2 < 5 && RacketX - racket->width / 2 > -5){
        racket->racketx = RacketX;
    }
    if(RacketY + racket->height / 2 < 2.5 && RacketY - racket->height/2 > -2.5){
        racket->rackety = RacketY;
    }
}

void testCollisions()
{
	obstaclesCollision(&ball, ot);
	racketCollision(racket, &ball);
	bonusCollision(&bonus, racket, &ball);
}

void translateActors(float time)
{
	translateBall(&ball, ball.vx * animTime, ball.vy * animTime, ball.vz * animTime, 5, 2.5, 50);
	if(racketWillCollide(&racket, &ot)) movingRacket = 1; //On teste si la raquette va entrer en collision avec un mur ; si oui, alors on la force à reculer. Sinon, elle avance
	translateRacket(&racket, movingRacket * racketSpeed, &racketDist);
	if(!ball.isAlive){
		translateBallOnRacket(&ball, racket);
		speed = 0;
		racketSpeed = 0;
	}
	translateSections(&sections, animTime * speed, MAX_SECTION_NUMBER);
	translateObstacles(&ot, animTime*speed, MAX_SECTION_NUMBER);
	translateObstacle(&boss, animTime*speed); //Puisque le boss (écriteau de fin) n'est pas inclu dans le tableau d'obstacles on le bouge indépendament
	translateBonus(&bonus, 2 * speed * animTime);
}

void getCoords(double *x, double *y, double dist) {
    if (aspectRatio < 1 && dist > 0) {
        *x = (GL_VIEW_SIZE*(*x)/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2)*(dist*tan(toRad(fov)/2.0));
        *y = ((GL_VIEW_SIZE/2 - GL_VIEW_SIZE*(*y)/(float)WINDOW_HEIGHT)/aspectRatio)*(dist*tan(toRad(fov)/2.0));
    }
    else if (dist > 0) {
        *x = ((GL_VIEW_SIZE*(*x)/(float)WINDOW_WIDTH - GL_VIEW_SIZE/2)*aspectRatio)*(dist*tan(toRad(fov)/2.0));
        *y = (GL_VIEW_SIZE/2 - GL_VIEW_SIZE*(*y)/(float)WINDOW_HEIGHT)*(dist*tan(toRad(fov)/2.0));
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &RacketX, &RacketY);
    getCoords(&RacketX, &RacketY, racketDist);
}

void drawUnlitScene()
{
    drawRacket(racket.width, racket.height, racket.racketx, racket.rackety, racket.racketz);
    drawBall(ball);	
    drawGUI(ball);
    drawBonus(bonus);
	drawEnd(&boss); //L'ecriteau de fin, dessiné sans lumière pour avoir un rendu uni
}

void drawLitScene()
{
	 //On active le lighting de la scene pour les sections et les obstacles uniquement
	drawSections(10, sections, ball, racket);
	drawObstacles(ot);
}

void drawGame()
{
    glDisable(GL_LIGHTING);
    drawUnlitScene();
    glEnable(GL_LIGHTING);
    drawLitScene();
}

void replayGame()
{
	racketDist = 5; //On réinitialise la distance à la camera, sinon on respawn là où on s'était arreté
    initActors();
    menu_debut.on = false;
    initLighting();
	updateRacket(&racket);
    //On met à jour la position du lighting
    updateLighting(ball, racket);
    drawGame();
    testCollisions();
    translateActors(animTime);
}

void playGame()
{
    //On active le lighting
    initLighting();
    //On update les coordonnées de la raquette
    updateRacket(&racket);
    //On met à jour la position du lighting
    updateLighting(ball, racket);
    drawGame();
    //On test d'éventuelles collisions
    testCollisions();
    //On déplace les différents acteurs dans la scène en fonction du facteur voulu
    translateActors(animTime);
}


void pauseGame() 
{
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(-0.1,-0.1, 29);
        glVertex3f(-0.1,0.1, 29);
        glVertex3f(0.1,0.1, 29);
        glVertex3f(0.1,-0.1, 29);
    glEnd();
    glDisable(GL_DEPTH_TEST);
    //On active le lighting
    initLighting();
    drawGame();
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        
		if(movingRacket == 1){
			movingRacket = -2; //Facteur d'inversion de la vitesse : la raquette avance 2 fois plus vite qu'elle ne recule (l'axe de la profondeur est orienté vers nous)
		}
    }
	else movingRacket = 1;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !ball.isAlive && !menu_debut.on){

		ball.isAlive = true;
		ball.vz = -2;
		speed = SPEED;
		racketSpeed = RACKET_SPEED;
	}

    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);
    getCoords(&x, &y, 30);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && menu_debut.on && inButton(menu_debut.play, x, y)) {
            menu_debut.on = false;
            focus = 3;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && menu_debut.on && inButton(menu_debut.quit, x, y)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && menu_fin.on && inButton(menu_fin.play, x, y)) {
        menu_fin.on = false;
        focus = 4;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && menu_fin.on && inButton(menu_fin.quit, x, y)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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
	initActors(WINDOW_WIDTH, WINDOW_HEIGHT);

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
        
        switch(focus) {
            case 0 : 
                glDisable(GL_LIGHTING);
                drawMenu(menu_debut);
                break;
            case 1 :
                glDisable(GL_LIGHTING);
                glDisable(GL_DEPTH_TEST);
                drawMenu(menu_fin);
                break;
            case 3 :
                playGame();
                break;
            case 4 :
                replayGame();
                focus = 3;
                break;
            case 5 :
                pauseGame();
                break;
        }

        if (ball.lives == 0) {
            menu_fin.on = true;
            focus = 1;
        }
		if(victory(&racket, &boss)){ //Si la raquette dépasse la position de l'écriteau de fin alors c'est gagné ! 
			menu_fin.type = "fin_victoire"; //On change le type du menu pour qu'il affiche "gagné"
			menu_fin.on = true;
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
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}

		/* Animate scenery */
	}

	glfwTerminate();
	return 0;
}

