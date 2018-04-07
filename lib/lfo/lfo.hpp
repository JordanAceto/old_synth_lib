#ifndef LFO_H_INCLUDED
#define LFO_H_INCLUDED

#include "phase_accumulator.hpp"
#include "wave_shapers.hpp"
#include "helper_functions.hpp"
#include "input_output_interface.hpp"
#include "base_classes.hpp"
#include "utilities.hpp"

class LFO : public Is_Tickable, public Is_Processable, public Has_Frequency_Range
{
public:
  LFO();
  ~LFO();

  void setSampleRate(float sample_rate) override;
  void tick() override;
  void process() override;

  enum LFO_Shape
  {
    SINE = 0,
    TRIANGLE,
    UP_SAW,
    DOWN_SAW,
    SQUARE,
    RANDOM,

    NUM_LFO_SHAPES
  };

  Signal_Input frequency_input;
  Gate_Input sync_input;

  Output_Interface output[NUM_LFO_SHAPES];

private:
  Phase_Accumulator core;
  Waveshaper_Interface *waveshape[NUM_LFO_SHAPES];
};


#endif
