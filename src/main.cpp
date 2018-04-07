#include <Arduino.h>

#include "synth_library.hpp"
#include "fx_mod_section/fx_mod_section.hpp"

const uint32_t sample_period_in_micros = 1000000.0 / sample_rate;
uint32_t last_tick;

One_Pole_Lowpass lpf;

Mono_VCF_Control vcf_controller;

Rotary_Encoder encoder(15, 14);

LFO lfo;

ADSR adsr;

Arduino_Analog_Input pot1(A9);
Arduino_Analog_Input pot2(A8);

Arduino_Analog_Output dac1(A21);
Arduino_Analog_Output dac2(A22);

void setup()
{
  //Serial.begin(38400); while (!Serial) {}
  analogWriteRes(num_ADC_bits);
  analogReadResolution(num_DAC_bits);

  lpf.setSampleRate(sample_rate);
  lfo.setSampleRate(sample_rate);
  adsr.setSampleRate(sample_rate);

  //lfo.gate_length_input.plugIn(&pot1.output);

  adsr.gate_input.plugIn(&lfo.output[LFO::SQUARE]);

  //adsr.input[ADSR_INPUT::ATTACK_TIME].plugIn(&pot1.output);
  adsr.input[ADSR_INPUT::DECAY_TIME].plugIn(&pot2.output);
  adsr.input[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(0.0);
  adsr.input[ADSR_INPUT::RELEASE_TIME].plugIn(&pot2.output);

  lfo.frequency_input.plugIn(&encoder.output);

  dac1.input.plugIn(&lfo.output[LFO::SQUARE]);
  //dac1.input.plugIn(&adsr.output);
  //dac1.input.plugIn(&trap.output);

  lpf.cutoff_input.plugIn(&pot1.output);
  lpf.input.plugIn(&lfo.output[LFO::SQUARE]);
  dac2.input.plugIn(&lpf.output);
  //dac2.input.plugIn(&encoder.output);
}

void loop()
{
  if (micros() - last_tick > sample_period_in_micros)
  {
    last_tick = micros();

    encoder.process();

    lpf.tick();
    lpf.process();

    lfo.tick();
    lfo.process();

    adsr.tick();
    adsr.process();

    pot1.process();
    pot2.process();

    dac1.process();
    dac2.process();
  }
}
