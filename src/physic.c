#include "primitives.h"
#include <math.h>
#include <time.h>

void translateSections(SectionsTab * st, float d, int spawnLimit)
{
	for(int i = 0; i < st->sectionNumber ; i ++){
        st->tab[i].position += d;
		if(st->tab[i].position > 46 && st->nb_spawned < spawnLimit){
            st->tab[i].position = 46 - ((st->sectionNumber-1) * st->tab[0].length);
			st->nb_spawned ++;
		}
	}
}

void translateBall(Ball * b, float dx, float dy, float dz, float xlim, float ylim, float zlim)
{
	b->x += dx;
	b->y += dy;
	b->z += dz;
	if(b->vx >0 && b->x +b->radius > xlim) b->vx *= -1;
	if(b->vx <0 && b->x - b->radius < -xlim) b->vx *= -1;
	if(b->vy > 0 && b->y + b->radius > ylim) b->vy *= -1;
	if(b->vy < 0 && b->y -b->radius < -ylim) b->vy *= -1;
	if(b->vz > 0 && b->z + b->radius > zlim - 20){
		b->isAlive = false;
		b->lives --;
	} 
	if(b->vz < 0 && b->z -b->radius < -zlim) b->vz *= -1;
}

void translateBallOnRacket(Ball * b, Racket *r)
{
	b->vx = 0;
	b->vy = 0;
	b->vz = 0;
	b->x = r->racketx;
	b->y = r->rackety;
	b->z = r->racketz - 2*b->radius;
}

void translateRacket(Racket * r, float dz, float * extRacketPosition)
{
    if(r->racketz < 31 && dz > 0){
		r->racketz += dz;
	}
	if(dz < 0){
		r->racketz += dz;
	}
	*extRacketPosition = 30 - r->racketz;
}

void racketCollision(Racket *r, Ball * b)
{
	if(b->vz > 0 && b->z + b->radius > r->racketz){
        if(b->x + b->radius > (r->racketx - r->width/2) && b->x - b->radius < (r->racketx + r->width / 2) && b->y + b->radius > (r->rackety - r->height/2) && b->y - b->radius < (r->rackety + r->height/2)){
			b->vz *= -1;
			b->vx += (b->x - r->racketx) / (r->width /2) * 0.2;
			b->vy += (b->y - r->rackety) / (r->height /2) * 0.2;
		}
	}
}

void translateObstacle(Obstacle * o, float dz)
{
    o->z += dz;
}

void translateObstacles(ObstaclesTab * ot, float dz, int maxSpawned)
{
	for(int i = 0; i<ot->nb; i++){
		if(ot->tab[i].z < 31){ //On translate si l'obstacle n'a pas atteint la camera
			translateObstacle(&(ot->tab[i]), dz);
		}
		else if (ot->nb_spawned < maxSpawned){
            for(int i = 0; i< ot->nb - 1; i++){
                ot->tab[i] = ot->tab[i+1];
			}
			Obstacle tmp;
			initObstacle(&tmp, 15 - ot->nb * 15, 10, 5);
			ot->tab[ot->nb-1] = tmp;
			ot->nb_spawned ++;
		}
	}
}

void obstaclesCollision(Ball * b, ObstaclesTab *ot)
{
	for(int i = 0 ; i <= ot->nb ; i++){
		if(b->vz < 0 && b->z < ot->tab[i].z && ot->tab[i].z < 25){
            if((b->x + b->radius > ot->tab[i].x - ot->tab[i].width/2) && (b->x - b->radius < ot->tab[i].x + ot->tab[i].width/2) && (b->y + b->radius > ot->tab[i].y - ot->tab[i].height/2) && (b->y - b->radius < ot->tab[i].y + ot->tab[i].height/2)){
				b->vz *= -1;
				break;
			}
		}
	}
}

void bonusCollision(Bonus * bonus, Racket *r, Ball * ball)
{
	if(r->racketz > bonus->z - 1 && r->racketz < bonus->z + 1){
		if(r->racketx + r->width / 2 > bonus->x - 1 && r->racketx - r->width / 2 < bonus->x + 1){
			if(r->rackety + r->height / 2 > bonus->y - 1 && r->rackety - r->height / 2 < bonus->y + 1){
				if(strcmp(bonus->type, "life") == 0) ball->lives ++;
				else if (strcmp(bonus->type, "glue") == 0) ball->isAlive = false;
                if(rand() % 2 == 0){
					initBonus(bonus, "life", -30, bonus->vz, 5, 2.5);
				}
				else initBonus(bonus, "glue", -30, bonus->vz, 5, 2.5);
			}
		}
	}
}

void translateBonus(Bonus * b, float dz)
{
	b->z += dz;
	if(b->z > 26){
		if(rand() % 2 == 0){
			initBonus(b, "glue", -30, b->vz, 5, 2.5);
            }
            else initBonus(b, "life", -30, b->vz, 5, 2.5);
	}
}

bool racketWillCollide(Racket *r, ObstaclesTab *ot)// Fonction vérifiant si la raquette est suffisament proche d'un obstacle qui l'empecherait éventuellement d'avancer
{
	for(int i = 0 ; i <= ot->nb ; i++){
		if((r->racketz - 1 < ot->tab[i].z) && (r->racketz + 1 > ot->tab[i].z)){ //La deuxième condition dans le if permet de s'affranchir de l'obstacle dès qu'il est dépassé
            if((r->racketx + r->width/2 > ot->tab[i].x - ot->tab[i].width/2) && (r->racketx - r->width/2 < ot->tab[i].x + ot->tab[i].width/2) && (r->rackety + r->height/2 > ot->tab[i].y - ot->tab[i].height/2) && (r->rackety - r->height/2 < ot->tab[i].y + ot->tab[i].height/2)){
				return true;
			}
		}
	}return false;
}

bool victory(Racket *r, Obstacle *boss)
{
    // La victoire est activée si la raquette réussi à dépasser l'écriteau de fin de partie
	return(r->racketz < boss->z);
}
