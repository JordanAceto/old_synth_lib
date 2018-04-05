#ifndef MINI_SYNTH_H_INCLUDED
#define MINI_SYNTH_H_INCLUDED

#include "synth_library.hpp"
#include "global_constants.hpp"

#include <Arduino.h>


class Mini_Synth : public Is_Tickable, public Is_Processable
{
public:
  Mini_Synth()
  {
    analogWriteRes(num_ADC_bits);
    analogReadResolution(num_DAC_bits);

    setSampleRate(sample_rate);

    adsr.input[ADSR_INPUT::ATTACK_TIME].plugIn(&pot[POT_1].output);
    adsr.input[ADSR_INPUT::DECAY_TIME].plugIn(&pot[POT_2].output);
    adsr.input[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(&pot[POT_3].output);
    adsr.input[ADSR_INPUT::RELEASE_TIME].plugIn(&pot[POT_2].output);
    adsr.input[ADSR_INPUT::OVERALL_TIME].plugIn(&pot[POT_4].output);

    adsr.gate_input.plugIn(&lfo.output[LFO::SQUARE]);

    pwm_dac[BUILT_IN_DAC_1].input.plugIn(&lfo.output[LFO::TRIANGLE]);
    //pwm_dac[PULSE_1].input.plugIn(&pot[POT_1].output);
    pwm_dac[BUILT_IN_DAC_2].input.plugIn(&lfo.output[LFO::SINE]);
    pwm_dac[BUILT_IN_DAC_2].input.plugIn(&adsr.output);

    lfo.setFrequency(0.5);
    //lfo.frequency_input.plugIn(&pot[POT_4].output);

  }

  void setSampleRate(float sample_rate)
  {
    adsr.setSampleRate(sample_rate);
    lfo.setSampleRate(sample_rate);
  }

  void tick() override
  {
    adsr.tick();
    lfo.tick();
    lfo.setFrequency(1.0); // just for testing
  }

  void process() override
  {
    for (auto &p : pot)
      p.process();

    for (auto &dac : pwm_dac)
      dac.process();

    adsr.process();
    lfo.process();
  }


private:
  enum POTENTIOMETER
  {
    POT_1 = 0,
    POT_2,
    POT_3,
    POT_4,

    NUM_POTS
  };

  Arduino_Analog_Input pot[NUM_POTS] =
  {
    Arduino_Analog_Input(A9),
    Arduino_Analog_Input(A8),
    Arduino_Analog_Input(A7),
    Arduino_Analog_Input(A6)
  };

  enum DAC_OUPUTS
  {
    PULSE_1 = 0,
    PULSE_2,
    VCO_1_LEV,
    VCO_2_LEV,
    NOISE_LEV,
    EXT_LEV,
    VCF_Q,
    NOISE,

    BUILT_IN_DAC_1,
    BUILT_IN_DAC_2,

    NUM_DAC_OUTPUTS
  };

  Arduino_Analog_Output pwm_dac[NUM_DAC_OUTPUTS] =
  {
    Arduino_Analog_Output(2),
    Arduino_Analog_Output(3),
    Arduino_Analog_Output(4),
    Arduino_Analog_Output(5),
    Arduino_Analog_Output(6),
    Arduino_Analog_Output(7),
    Arduino_Analog_Output(8),
    Arduino_Analog_Output(9),

    Arduino_Analog_Output(A21),
    Arduino_Analog_Output(A22)
  };

  ADSR adsr;
  LFO lfo;

};

#endif
