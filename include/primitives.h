#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdbool.h>
#include <string.h>

typedef struct str_section
{

    float width;
    float height; 
    float length;
    float position;

}Section;

typedef struct str_sectionstab{

    Section tab[10] ;
    int sectionNumber;
    int nb_spawned;

}SectionsTab;

typedef struct str_ball{

    float x;
    float y;
    float z;

    float vx;
    float vy;
    float vz;

    float radius;

    bool isAlive;
    int lives;

}Ball;

typedef struct str_racket{

    float width;
    float height;

    float racketx;
    float rackety;
    float racketz;

}Racket;

typedef struct str_obstacle{

    float width;
    float height;

    float vz;

    float x;
    float y;
    float z;

}Obstacle;

typedef struct str_obstacles_tab{

    int nb;
    int nb_spawned;
    Obstacle tab[20];

}ObstaclesTab;

typedef struct str_bonus{

    char * type;
    
    float x;
    float y;
    float z;

    float vz;
}Bonus;

//Dessine un carré de largeur width et de hauteur height, centré en (RacketX, RacketY)
void drawRacket(float width, float height, double RacketX, double RacketY, float d);

void drawSection(int resolution, float width, float height, float length, float position, Ball b, Racket r);

void drawSections(int resolution, SectionsTab st, Ball b, Racket r);

void initSectionsTab(SectionsTab * st, int sectionNumber);

void translateSections(SectionsTab * st, float d, int spawnLimit);

void initSection(Section * s, float w, float h, float l, float p);

void initBall(Ball * b, float x, float y, float z, float vx, float vy, float vz, float r);

void drawBall(Ball b);

void translateBall(Ball * b, float dx, float dy, float dz, float xlim, float ylim, float zlim);

void translateBallOnRacket(Ball * b, Racket r);

void initRacket(Racket * r, float w, float h, float x, float y, float z);

void initBonus(Bonus * b, char * type, float z, float vz, float xlim, float ylim);

void translateRacket(Racket * r, float dz, float * extRacketPosition);

void racketCollision(Racket r, Ball * b);

void initObstacle(Obstacle * o,float position, float xlim, float ylim);

void drawObstacle(Obstacle o);

void initObstaclesTab(ObstaclesTab * ot, int nb, float origin, float xlim, float ylim);

void drawObstacles(ObstaclesTab ot);

void translateObstacle(Obstacle * o, float dz);

void translateObstacles(ObstaclesTab * ot, float dz,int maxSpawned);

void obstaclesCollision(Ball * b, ObstaclesTab ot);

float calculateDist(float x1,float y1,float z1,float x2,float y2,float z2);

void drawGUI(Ball b);

void drawBonus(Bonus b);

void translateBonus(Bonus * b, float dz);

void bonusCollision(Bonus * bonus, Racket r, Ball * ball);

#endif