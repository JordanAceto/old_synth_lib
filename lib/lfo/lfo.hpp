#ifndef LFO_H_INCLUDED
#define LFO_H_INCLUDED

#include "phase_accumulator.hpp"
#include "wave_shapers.hpp"
#include "helper_functions.hpp"
#include "input_output_interface.hpp"
#include "base_classes.hpp"
#include "utilities.hpp"

class LFO : public Phase_Accumulator, public Is_Processable
{
public:
  LFO();
  ~LFO();

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

  Signal_Input_Feedthrough frequency_input;
  Gate_Input sync_input;

  Output_Interface output[NUM_LFO_SHAPES];

private:
  Bipolar_To_Unipolar_Converter freq_input_scaler;
  Waveshaper_Interface *waveshape[NUM_LFO_SHAPES];
};


#endif
