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

typedef struct str_sectionstab
{
    Section tab[10] ;
    int sectionNumber; //Nb effectif de sections dans le tableau
    int nb_spawned; //Nb de fois que les obstacles ont été reset, donc qu'ils ont dépassé la caméra avant d'ete respawn plus loin
}SectionsTab;

typedef struct str_ball
{
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

typedef struct str_racket
{
    float width;
    float height;

    float racketx;
    float rackety;
    float racketz;

}Racket;

typedef struct str_obstacle
{
    float width;
    float height;

    float vz;

    float x;
    float y;
    float z;
}Obstacle;

typedef struct str_obstacles_tab
{
    int nb;
    int nb_spawned;
    Obstacle tab[20];
}ObstaclesTab;

typedef struct str_bonus
{
    char * type; //"glue" ou "life"
    
    float x;
    float y;
    float z;

    float vz;
}Bonus;

typedef struct str_button
{
    float height;
    float width;
    float x;
    float y;
    float r, g, b;
    char *type;
}Button;

typedef struct str_menu
{
    float width;
    float height;
    Button play;
    Button quit;
    bool on;
    char *type;
}Menu;

typedef struct str_game
{
    Ball ball;
    Racket racket;
    Menu menu_debut;
    Menu menu_fin;
    SectionsTab sections;
    ObstaclesTab ot;
    Obstacle boss;
    Bonus bonus;
}Game;

//Dessine un carré de largeur width et de hauteur height, centré en (RacketX, RacketY)
void drawRacket(float width, float height, double RacketX, double RacketY, float d);
//Dessine une section selon les paramètres
void drawSection(int resolution, float width, float height, float length, float position/*, Ball b, Racket r*/);
//Dessine l'ensemble des sections d'un tableau 
void drawSections(int resolution, SectionsTab *st/*, Ball b, Racket r*/);
//Initialise les sections d'un tableau
void initSectionsTab(SectionsTab *st/*, int sectionNumber*/);
//Initialise les sections en leur assignant largeur hauteur et profondeur ainsi que position
void initSection(Section *s, float w, float h, float l, float p);
//Initialise la balle avec une certaine vitesse, position, etc
void initBall(Ball *b, float x, float y, float z, float vx, float vy, float vz, float r);
//Dessine la balle
void drawBall(Ball *b);
//Initialise la raquette 
void initRacket(Racket *r, float w, float h, float z);
//Initialise le bonus selon un type et des coordonnées aléatoires
void initBonus(Bonus *b, char * type, float z, float vz, float xlim, float ylim);
//Initialise un obstacle selon une taille et une position aléatoire
void initObstacle(Obstacle *o, float position, float xlim, float ylim);
//Dessine un obstacle textué ;)
void drawObstacle(Obstacle *o);
//Initialise les nb premiers ostacles d'un tableau
void initObstaclesTab(ObstaclesTab *ot, int nb, float origin, float xlim, float ylim);
//Dessine tous les obstacles d'un tableau
void drawObstacles(ObstaclesTab *ot);
//Calcule la distance entre deux points (inutilisée pour le moment)
float calculateDist(float x1,float y1,float z1,float x2,float y2,float z2);
//Affiche à l'écran le nombre de vies de la balle 
void drawGUI(Ball *b);
//Dessine le bonus
void drawBonus(Bonus *b);
void initMenu(Menu *menu, float width, float height, char *type);
void drawMenu(Menu *menu);
void initButton(Button *button, float x, float y, float height, float width, char *type);
void drawButton(Button *button);
bool inButton(Button *button, float x, float y);
void drawEnd(Obstacle *o);

#endif
