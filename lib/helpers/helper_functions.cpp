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

float mapInput(float input, float output_for_neg_one, float output_for_pos_one)
{
  static const float input_range = 2.0;
  static const float input_offset = -1.0;
  const float output_range = output_for_pos_one - output_for_neg_one;

  return (input - input_offset)  * output_range / input_range + output_for_neg_one;
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
