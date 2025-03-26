#ifndef HELPERS_H
#define HELPERS_H

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh);

#endif