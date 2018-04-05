#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "input_output_interface.hpp"
#include "helper_functions.hpp"

#include <Arduino.h>

/*
this was made to simulate the Jurgen Haible morphing LFO
that I liked, scans between triangle, sine, square, sample and hold and noise.
it is tuned to work for that and not necessarily as a general purpose scanner,
maybe it should just go with the LFO stuff and make a more generalized scanner later
*/
class Five_Input_Scanner : public Is_Processable
{
public:

  enum INPUTS
  {
    INPUT_1 = 0,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,

    NUM_INPUTS
  };

  Signal_Input signal_input[NUM_INPUTS];
  Signal_Input control_input;
  Output_Interface output;

  void process() override;

private:
  float trapezoid_1, trapezoid_2, trapezoid_3, trapezoid_4, trapezoid_5;

};

class Trapezoid_Generator : public Is_Processable
{
public:
  Trapezoid_Generator()
  {
    input.setGain(0.5);
    input.setOffset(0.5);  
  }

  void setCenter(float center) { this->center = center; }
  void setWidth(float width) { this->width = width; }
  void setSlope(float slope) { this->slope = slope; }

  Signal_Input input;
  Output_Interface output;
  void process() override
  {
    const float plateau_start = center - (0.5 * width);
    const float plateau_end = center + (0.5 * width);

    if (input.get() < plateau_start)
      output.set(clamp(input.get() * slope + (1 - slope * plateau_start), 0.0, 1.0));
    else if (plateau_start <= input.get() && input.get() < plateau_end)
      output.set(1.0);
    else if (input.get() >= plateau_end)
      output.set(clamp(input.get() * -slope + (1 + slope * plateau_end), 0.0, 1.0));
  }

private:
  float center, width, slope;
};

#endif
