#include "helpers.h"

float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}
