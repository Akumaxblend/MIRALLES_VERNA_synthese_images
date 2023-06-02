#ifndef DRAWS_H
#define DRAWS_H
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <math.h>

#include "primitives.h"
#include "physic.h"

void drawUnlitScene(Game *game);
void drawLitScene(Game *game);
void drawGame(Game *game);

#endif
