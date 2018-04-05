#ifndef MINI_SYNTH_H_INCLUDED
#define MINI_SYNTH_H_INCLUDED

#include "synth_library.hpp"
#include "global_constants.hpp"

#include <Arduino.h>

class Quad_ADSR : public Is_Tickable, public Is_Processable
{
public:
  Quad_ADSR();

  void setSampleRate(float sample_rate) override;

  void tick() override;

  void process() override;

  enum ADSR_CHANNEL
  {
    ADSR_1 = 0,
    ADSR_2,
    ADSR_3,
    ADSR_4,

    NUM_ADSR_CHANNELS
  };

  void assignPotsTo(ADSR_CHANNEL new_channel);

private:
  int active_channel, last_channel;

  enum POTENTIOMETER
  {
    ATTACK_POT = 0,
    DECAY_POT,
    SUSTAIN_POT,
    RELEASE_POT,

    NUM_POTS
  };

  Arduino_Analog_Input pot[NUM_POTS] =
  {
    Arduino_Analog_Input(A9),
    Arduino_Analog_Input(A8),
    Arduino_Analog_Input(A7),
    Arduino_Analog_Input(A6)
  };

  Arduino_Digital_Input gate_in[NUM_ADSR_CHANNELS] =
  {
    Arduino_Digital_Input(9),
    Arduino_Digital_Input(10),
    Arduino_Digital_Input(11),
    Arduino_Digital_Input(12)
  };

  Arduino_Digital_Input trigger_in[NUM_ADSR_CHANNELS] =
  {
    Arduino_Digital_Input(16),
    Arduino_Digital_Input(15),
    Arduino_Digital_Input(14),
    Arduino_Digital_Input(13)
  };

  Arduino_Analog_Output pwm_dac[NUM_ADSR_CHANNELS] =
  {
    Arduino_Analog_Output(3),
    Arduino_Analog_Output(4),
    Arduino_Analog_Output(5),
    Arduino_Analog_Output(6),
  };

  ADSR adsr[NUM_ADSR_CHANNELS];

};

#endif
