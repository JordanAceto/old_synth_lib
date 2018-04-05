#ifndef CLOCK_GENERATOR_H_INCLUDED
#define CLOCK_GENERATOR_H_INCLUDED

#include "phase_accumulator.hpp"
#include "input_output_interface.hpp"

class Clock_Generator : public Phase_Accumulator, public Is_Processable
{
public:
  bool gateIsHigh() const;

  void setGateLength(float percent);

  void process() override;

  Output_Interface output;

private:
  float gate_length;

};


#endif
