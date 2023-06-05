#ifndef INIT_H
#define INIT_H
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "3D_tools.h"
#include "draws.h"

#define MAX_SECTION_NUMBER 20

void initActors(Game *game, float speed, float racketDist);
void testCollisions(Game *game);
void updateLighting(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball);
void updateRacket(Racket *racket, double RacketX, double RacketY);
void translateActors(Game *game, float time, int movingRacket, float *racketDist, float speed, float racketSpeed);
void replayGame(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball, GLfloat *light_color_racket, GLfloat *light_color_ball, float time, int movingRacket, float *racketDist, float speed, double RacketX, double RacketY, float racketSpeed);
void playGame(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball, GLfloat *light_color_racket, GLfloat *light_color_ball, float time, int movingRacket, float *racketDist, float speed, double RacketX, double RacketY, float racketSpeed);
void pauseGame(Game *game, GLfloat *light_position_racket, GLfloat *light_position_ball, GLfloat *light_color_racket, GLfloat *light_color_ball);

#endif
