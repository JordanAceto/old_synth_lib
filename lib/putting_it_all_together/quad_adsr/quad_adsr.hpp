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

#define POT_1 A9
#define POT_2 A8
#define POT_3 A7
#define POT_4 A6

#define GATE_1 24
#define GATE_2 25
#define GATE_3 26
#define GATE_4 27

#define TRIG_1 28
#define TRIG_2 29
#define TRIG_3 30
#define TRIG_4 31

#define DAC_1 A21
#define DAC_2 A22
#define DAC_3 4
#define DAC_4 5

#define PUSH_BUTTON 13



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

  void assignPotsTo(int new_channel);
  void assignPotsToAllChannels();

private:
  int active_channel, last_channel;

  Arduino_Digital_Input channel_cycle_button = Arduino_Digital_Input(PUSH_BUTTON);
  Gate_Input channel_cycle_in;

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
    Arduino_Analog_Input(POT_1),
    Arduino_Analog_Input(POT_2),
    Arduino_Analog_Input(POT_3),
    Arduino_Analog_Input(POT_4)
  };

  Signal_Input_Feedthrough_With_Delayed_Plug_In input_feedthrough[NUM_POTS];

  Arduino_Digital_Input gate_in[NUM_ADSR_CHANNELS] =
  {
    Arduino_Digital_Input(GATE_1),
    Arduino_Digital_Input(GATE_2),
    Arduino_Digital_Input(GATE_3),
    Arduino_Digital_Input(GATE_4)
  };

  Arduino_Digital_Input trigger_in[NUM_ADSR_CHANNELS] =
  {
    Arduino_Digital_Input(TRIG_1),
    Arduino_Digital_Input(TRIG_2),
    Arduino_Digital_Input(TRIG_3),
    Arduino_Digital_Input(TRIG_4)
  };

  Arduino_Analog_Output pwm_dac[NUM_ADSR_CHANNELS] =
  {
    Arduino_Analog_Output(DAC_1),
    Arduino_Analog_Output(DAC_2),
    Arduino_Analog_Output(DAC_3),
    Arduino_Analog_Output(DAC_4),
  };

  ADSR adsr[NUM_ADSR_CHANNELS];

};

#endif
