#ifndef PHYSIC_H
#define PHYSIC_H
#include "primitives.h"

//Applique un déplacement d à toutes les sections d'un tableau
void translateSections(SectionsTab * st, float d, int spawnLimit);
//Applique une translation sur la balle, sans que la balle ne puisse sortir des limites lim
void translateBall(Ball * b, float dx, float dy, float dz, float xlim, float ylim, float zlim);
//Fait coller la balle à la raquette au début et à la perte d'une vie ou récup bonus colle
void translateBallOnRacket(Ball * b, Racket r);
//Applique un déplacement sur la raquette
void translateRacket(Racket * r, float dz, float * extRacketPosition);
//Gere les collisions balle raquette
void racketCollision(Racket r, Ball * b);
//Applique un déplacement dz sur l'obstacle, le faisant réapparaitre plus loin si il dépasse la caméra
void translateObstacle(Obstacle * o, float dz);
//Applique un déplacement dz sur tous les obstacles d'un tableau, en faisant réapparaître maximum maxSpawned fois les obstacles
void translateObstacles(ObstaclesTab * ot, float dz,int maxSpawned);
//Gere les collisions balle - obstacles
void obstaclesCollision(Ball * b, ObstaclesTab ot);
//Gere la collision bonus - raquette (bonus récupéré ou non)
void bonusCollision(Bonus * bonus, Racket r, Ball * ball);
//Applique un déplacement dz sur le bonus
void translateBonus(Bonus * b, float dz);
//Verifie si la raquette entrera en collision avec un obstacle
bool racketWillCollide(Racket * r, ObstaclesTab * ot);

#endif
