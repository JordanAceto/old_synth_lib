#ifndef HELPER_FUNCTIONS_H_INCLUDED
#define HELPER_FUNCTIONS_H_INCLUDED

#include <Arduino.h>

template <typename T>
T absVal(T x) { return x > 0 ? x : -x; }
/*
template <typename T>
T clamp(T x, T min, T max) { return (x < min) ? min : (x > max) ? max : x; }
*/
int clamp(int x, int min, int max);
float clamp(float x, float min, float max);
double clamp(double x, double min, double max);

float clamp(float x);

float mapInput(float input, float output_for_neg_one, float output_for_pos_one);

float bipolarToUnipolar(float);
float unipolarToBipolar(float);

int scaleForDAC(float);

bool checkExpect(int expected, int actual, const char *fail_message);
bool checkExpect(double expected, double actual, const char *fail_message);


#endif
