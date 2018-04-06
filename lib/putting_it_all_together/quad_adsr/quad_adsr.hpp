#ifndef MINI_SYNTH_H_INCLUDED
#define MINI_SYNTH_H_INCLUDED

#include "synth_library.hpp"
#include "global_constants.hpp"

/*

Quad ADSR module to be installed on a teensy 3.2. If using a different
microcontroller change the input and output pins as needed.

Module consists of four individual ADSR's, each with their own gate and
trigger inputs and analog outputs, but only one set of four analog
attack/decay/sustain/release controls which are shared between them.

The controls can be shared between all four ADSR cores, or assigned to
individual ADSR's.

This is a work in progress. As of now, when you switch the controls from one
ADSR to another, the one you switched from remembers its last control settigns
and keeps them until the controls are reassigned to it. This is fine. The ADSR
you switch the controls to however immediately takes on the value of the
controls, which is less ideal. Better behavior would be to keep the value it
was until you adjust the controls enough to "catch up" to where it was. This
will require some thought, and will be useful/necessary for other modules as
well.

For now, this is here as an idea of one application of this library.

*/

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
  void assignPotsToAll();

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
