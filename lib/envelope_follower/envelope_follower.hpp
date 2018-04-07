#ifndef ENVELOPE_FOLLOWER_H_INCLUDED
#define ENVELOPE_FOLLOWER_H_INCLUDED

#include "input_output_interface.hpp"
#include "base_classes.hpp"
#include "filters.hpp"
#include "utilities.hpp"

class Envelope_Follower : public Is_Tickable, public Is_Processable
{
public:
  Envelope_Follower();

  void setSampleRate(float sample_rate);

  Full_Wave_Rectifier fwr;
  Signal_Input_Feedthrough decay_control;
  Output_Interface output;

  void tick() override;
  void process() override;

private:
  One_Pole_Lowpass lpf;

  float peak;
};


#endif
