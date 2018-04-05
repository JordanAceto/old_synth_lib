#include "helper_functions.hpp"
/*
int absVal(int x)
{
    return x > 0 ? x : -x;
}
*/
int clamp(int x, int minimum, int maximum)
{
    return (x < minimum) ? minimum : (x > maximum) ? maximum : x;
}

float clamp(float x, float minimum = -1.0, float maximum = 1.0)
{
    return (x < minimum) ? minimum : (x > maximum) ? maximum : x;
}

float clamp(float x)
{
  return (x < -1.0) ? -1.0 : (x > 1.0) ? 1.0 : x;
}

void swap(int &x, int &y)
{
    int temp = x;
    x = y;
    y = temp;
}

float bipolarToUnipolar(float x)
{
  return (x + 1.0) / 2.0;
}

float unipolarToBipolar(float x)
{
  return x * 2 - 1.0;
}

int scaleForDAC(float x)
{
  return (x + 1.0) * float(1 << 11);
}
