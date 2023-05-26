#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdbool.h>
#include <string.h>

typedef struct str_section
{

    float width;
    float height; 
    float length;
    float position; //Position z, permet de savoir que la prochaine section doit etre créée à position+length

}Section;

typedef struct str_sectionstab{

    Section tab[10] ;
    int sectionNumber; //Nb effectif de sections dans le tableau
    int nb_spawned; //Nb de fois que les obstacles ont été reset, donc qu'ils ont dépassé la caméra avant d'ete respawn plus loin

}SectionsTab;

typedef struct str_ball{

    float x;
    float y;
    float z;

    float vx;
    float vy;
    float vz;

    float radius;

    bool isAlive; //La balle est en mouvement = true, false sinon
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

    char * type; //"glue" ou "life"
    
    float x;
    float y;
    float z;

    float vz;
}Bonus;

//Dessine un carré de largeur width et de hauteur height, centré en (RacketX, RacketY)
void drawRacket(float width, float height, double RacketX, double RacketY, float d);

//Dessine une section selon les paramètres
void drawSection(int resolution, float width, float height, float length, float position, Ball b, Racket r);

//Dessine l'ensemble des sections d'un tableau 
void drawSections(int resolution, SectionsTab st, Ball b, Racket r);

//Initialise les sections d'un tableau
void initSectionsTab(SectionsTab * st, int sectionNumber);

//Applique un déplacement d à toutes les sections d'un tableau
void translateSections(SectionsTab * st, float d, int spawnLimit);

//Initialise les sections en leur assignant largeur hauteur et profondeur ainsi que position
void initSection(Section * s, float w, float h, float l, float p);

//Initialise la balle avec une certaine vitesse, position, etc
void initBall(Ball * b, float x, float y, float z, float vx, float vy, float vz, float r);

//Dessine la balle
void drawBall(Ball b);

//Applique une translation sur la balle, sans que la balle ne puisse sortir des limites lim
void translateBall(Ball * b, float dx, float dy, float dz, float xlim, float ylim, float zlim);

//Fait coller la balle à la raquette au début et à la perte d'une vie ou récup bonus colle
void translateBallOnRacket(Ball * b, Racket r);

//Initialise la raquette 
void initRacket(Racket * r, float w, float h, float x, float y, float z);

//Initialise le bonus selon un type et des coordonnées aléatoires
void initBonus(Bonus * b, char * type, float z, float vz, float xlim, float ylim);

//Applique un déplacement sur la raquette
void translateRacket(Racket * r, float dz, float * extRacketPosition);

//Gere les collisions balle raquette
void racketCollision(Racket r, Ball * b);

//Initialise un obstacle selon une taille et une position aléatoire
void initObstacle(Obstacle * o,float position, float xlim, float ylim);

//Dessine un obstacle textué ;)
void drawObstacle(Obstacle o);

//Initialise les nb premiers ostacles d'un tableau
void initObstaclesTab(ObstaclesTab * ot, int nb, float origin, float xlim, float ylim);

//Dessine tous les obstacles d'un tableau
void drawObstacles(ObstaclesTab ot);

//Applique un déplacement dz sur l'obstacle, le faisant réapparaitre plus loin si il dépasse la caméra
void translateObstacle(Obstacle * o, float dz);

//Applique un déplacement dz sur tous les obstacles d'un tableau, en faisant réapparaître maximum maxSpawned fois les obstacles
void translateObstacles(ObstaclesTab * ot, float dz,int maxSpawned);

//Gere les collisions balle - obstacles
void obstaclesCollision(Ball * b, ObstaclesTab ot);

//Calcule la distance entre deux points (inutilisée pour le moment)
float calculateDist(float x1,float y1,float z1,float x2,float y2,float z2);

//Affiche à l'écran le nombre de vies de la balle 
void drawGUI(Ball b);

//Dessine le bonus
void drawBonus(Bonus b);

//Applique un déplacement dz sur le bonus
void translateBonus(Bonus * b, float dz);

//Gere la collision bonus - raquette (bonus récupéré ou non)
void bonusCollision(Bonus * bonus, Racket r, Ball * ball);

#endif