#ifndef MULTIPLIERS_H_INCLUDED
#define MULTIPLIERS_H_INCLUDED

#include "inputs_and_outputs.hpp"

class Two_Quadrant_Multiplier : public Is_Processable
{
public:
  Signal_Input signal_input;
  Signal_Input control_input;
  Output_Interface output;
  void process() override;
};

class Four_Quadrant_Multiplier : public Is_Processable
{
public:
  Signal_Input x_input;
  Signal_Input y_input;
  Output_Interface output;
  void process() override;
};

#endif
