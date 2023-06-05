#pragma once

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NB_SEG_CIRCLE 64

/* Camera parameters and functions */
static const float Z_NEAR = 0.1f;
static const float Z_FAR = 1000.f;
extern float theta;      // Angle between x axis and viewpoint
extern float phy  ;      // Angle between z axis and viewpoint
extern float dist_zoom ; // Distance between origin and viewpoint

void setCamera();

/* Draw cannonic objet functions */
void drawSquare();

void drawCircle();

void drawCone();

void drawSphere();

void drawOrigin();

void drawCylinder();

void drawGlue();

void drawLife();

void drawRectangle(const float scale_h, const float scale_w);

void drawZero();
void drawOne();
void drawTwo();
void drawThree();
void drawFour();
void drawFive();
void drawSix();
void drawSeven();
void drawEight();
void drawNine();

void drawNumber(const int n);
/* Small tools */
float toRad(float deg);
