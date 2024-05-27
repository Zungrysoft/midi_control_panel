#include "helpers.h"

float clamp(float n, float min, float max) {
  if (n < min) {
    return min;
  }

  if (n > max) {
    return max;
  }

  return n;
}

float map(float n, float start1, float stop1, float start2, float stop2, bool clampOutput) {
  const float newval = (n - start1) / (stop1 - start1) * (stop2 - start2) + start2;

  if (!clampOutput) {
    return newval;
  }

  return clamp(newval, start2, stop2);
}

float map(float n, float start1, float stop1, float start2, float stop2) {
  return map(n, start1, stop1, start2, stop2, false);
}
