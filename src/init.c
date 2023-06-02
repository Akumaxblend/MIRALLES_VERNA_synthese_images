#include "init.h"

void initActors(Game *game, float speed)
{
	//Random seed initialisation
	srand(time(NULL));
	//initialisation de la balle
	initBall(&game->ball, 0, 0, 0, 0, 0, 0, 0.25);
	//initialisation des sections
	initSectionsTab(&game->sections);
	//initialisation de la raquette 
	initRacket(&(game->racket), 1, 1, 25);
	//initialisation du bonus
	initBonus(&game->bonus, "life", 20, speed, 5, 2.5);
	//initialisation des obstacles
	initObstaclesTab(&game->ot, 10, 15, 10, 5);
    //attention les dimensions des menus sont à revoir car beaucoup trop grandes si on veut y mettre des textures
    //initialisation du menu de debut
    initMenu(&game->menu_debut, 30, 25);
    game->menu_debut.on = true;
    //initialisation du menu de fin
    initMenu(&game->menu_fin, 30, 25);
    game->menu_fin.on = false;
}

void initLighting(GLfloat *light_position_racket, GLfloat *light_position_ball, GLfloat *light_color_racket, GLfloat *light_color_ball)
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

void testCollisions(Game *game)
{
	obstaclesCollision(&game->ball, &game->ot);
	racketCollision(&game->racket, &game->ball);
	bonusCollision(&game->bonus, &game->racket, &game->ball);
}

void updateLighting(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball)
{
	light_position_racket[0] = game->racket.racketx;
	light_position_racket[1] = game->racket.rackety;
	light_position_racket[2] = game->racket.racketz;

	light_position_ball[0] = game->ball.x;
	light_position_ball[1] = game->ball.y;
	light_position_ball[2] = game->ball.z;
}

void updateRacket(Racket *racket, double RacketX, double RacketY)
{
    if(RacketX + racket->width/2 < 5 && RacketX - racket->width / 2 > -5){
        racket->racketx = RacketX;
    }
    if(RacketY + racket->height / 2 < 2.5 && RacketY - racket->height/2 > -2.5){
        racket->rackety = RacketY;
    }
}

void translateActors(Game *game, float time, int movingRacket, float *racketDist, float speed, float racketSpeed)
{
	translateBall(&game->ball, game->ball.vx * time, game->ball.vy * time, game->ball.vz * time, 5, 2.5, 50);
	translateRacket(&game->racket, -movingRacket * racketSpeed, racketDist);
	if(!game->ball.isAlive){
		translateBallOnRacket(&game->ball, &game->racket);
		speed = 0;
		racketSpeed = 0;
	}
	translateSections(&game->sections, time * speed, MAX_SECTION_NUMBER);
	translateObstacles(&game->ot, time*speed, MAX_SECTION_NUMBER);
	translateBonus(&game->bonus, 2 * speed * time);
}

void replayGame(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball, GLfloat *light_color_racket, GLfloat *light_color_ball, float time, int movingRacket, float *racketDist, float speed, double RacketX, double RacketY, float racketSpeed)
{
    initActors(game, speed);
    game->menu_debut.on = false;
    initLighting(light_position_racket, light_position_ball, light_color_racket, light_color_ball);
    //On met à jour la position du lighting
    updateLighting(game, light_position_racket, light_position_ball);
    updateRacket(&game->racket, RacketX, RacketY);
    drawGame(game);
    testCollisions(game);
    translateActors(game, time, movingRacket, racketDist, speed, racketSpeed);
}

void playGame(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball, GLfloat *light_color_racket, GLfloat *light_color_ball, float time, int movingRacket, float *racketDist, float speed, double RacketX, double RacketY, float racketSpeed)
{
    //On active le lighting
    initLighting(light_position_racket, light_position_ball, light_color_racket, light_color_ball);
    //On update les coordonnées de la raquette
    updateRacket(&game->racket, RacketX, RacketY);
    //On met à jour la position du lighting
    updateLighting(game, light_position_racket, light_position_ball);
    drawGame(game);
    //On test d'éventuelles collisions
    testCollisions(game);
    //On déplace les différents acteurs dans la scène en fonction du facteur voulu
    translateActors(game, time, movingRacket, racketDist, speed, racketSpeed);
}

void pauseGame(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball, GLfloat *light_color_racket, GLfloat *light_color_ball) 
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
    initLighting(light_position_racket, light_position_ball, light_color_racket, light_color_ball);
    drawGame(game);
}
