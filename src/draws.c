#include "draws.h"


void drawUnlitScene(Game *game)
{
    drawRacket(game->racket.width, game->racket.height, game->racket.racketx, game->racket.rackety, game->racket.racketz);
    drawBall(&game->ball);	
    drawGUI(&game->ball);
    drawBonus(&game->bonus);
}

void drawLitScene(Game *game)
{
	 //On active le lighting de la scene pour les sections et les obstacles uniquement
	drawSections(10, &game->sections/*, ball, racket*/);
	drawObstacles(&game->ot);
}

void drawGame(Game *game)
{
    glDisable(GL_LIGHTING);
    drawUnlitScene(game);
    glEnable(GL_LIGHTING);
    drawLitScene(game);
}


