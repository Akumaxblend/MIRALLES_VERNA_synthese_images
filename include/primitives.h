#ifndef PRIMITIVES_H
#define PRIMITIVES_H

typedef struct str_section
{

    float width;
    float height; 
    float length;
    float position;

}Section;

typedef struct str_sectionstab{

    Section tab [10];
    int sectionNumber;

}SectionsTab;

//Dessine un carré de largeur width et de hauteur height, centré en (RacketX, RacketY)
void drawRacket(float width, float height, double RacketX, double RacketY, float d);

void drawSection(float width, float height, float length, float position);

void drawSections(SectionsTab st);

void initSectionsTab(SectionsTab st, int sectionNumber);

void translateSections(SectionsTab st, float d);

void initSection(Section s, float w, float h, float l, float p);

#endif