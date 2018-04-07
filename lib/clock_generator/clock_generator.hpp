#ifndef CLOCK_GENERATOR_H_INCLUDED
#define CLOCK_GENERATOR_H_INCLUDED

#include "phase_accumulator.hpp"
#include "inputs_and_outputs.hpp"

class Clock_Generator : public Is_Tickable, public Is_Processable
{
public:
  Clock_Generator();

  void setSampleRate(float sample_rate) override;
  void tick() override;
  void process() override;

  bool gateIsHigh() const;
  void setGateLength(float percent);

  Signal_Input frequency_input;
  Signal_Input gate_length_input;
  Output_Interface output;
private:
  Phase_Accumulator core;
  float gate_length;

};


#endif
