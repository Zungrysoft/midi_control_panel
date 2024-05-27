#ifndef HELPERS_H
#define HELPERS_H

float clamp(float n, float min, float max);
float map(float n, float start1, float stop1, float start2, float stop2, bool clampOutput);
float map(float n, float start1, float stop1, float start2, float stop2);

#endif