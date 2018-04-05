#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "input_output_interface.hpp"
#include "utilities.hpp"
#include "helper_functions.hpp"

#include <Arduino.h>

class Trapezoid_Generator : public Is_Processable
{
public:
  Trapezoid_Generator();

  void setCenter(float center) { this->center = center; }
  void setWidth(float width) { this->width = width; }
  void setSlope(float slope) { this->slope = slope; }

  Signal_Input input;
  Output_Interface output;
  void process() override;

private:
  float center, width, slope;
};

class Five_Input_Scanner : public Is_Processable
{
public:
  Five_Input_Scanner();

  enum INPUTS
  {
    INPUT_1 = 0,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,

    NUM_INPUTS
  };

  Signal_Input_Feedthrough signal_input[NUM_INPUTS];
  Signal_Input_Feedthrough control_input;
  Output_Interface output;

  void process() override;

private:
  Mixer mixer;
  Trapezoid_Generator trapezoid[NUM_INPUTS];
};


#endif
