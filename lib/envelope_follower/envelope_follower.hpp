#ifndef ENVELOPE_FOLLOWER_H_INCLUDED
#define ENVELOPE_FOLLOWER_H_INCLUDED

#include "input_output_interface.hpp"
#include "base_classes.hpp"
#include "filters.hpp"
#include "utilities.hpp"

class Envelope_Follower : public Is_Tickable, public Is_Processable
{
public:
  Envelope_Follower()
  {
    lpf.input.plugIn(&fwr.output);
    lpf.cutoff_input.plugIn(&decay_control.output);
    decay_control.setGain(0.03);
  }

  void setSampleRate(float sample_rate)
  {
    Is_Tickable::setSampleRate(sample_rate);
    lpf.setSampleRate(sample_rate);
  }

  Full_Wave_Rectifier fwr;
  Signal_Input_Feedthrough decay_control;
  Output_Interface output;

  void tick() override;
  void process() override;

private:
  One_Pole_Lowpass_Filter lpf;

  float peak;
};


#endif
