#include <Arduino.h>

#include "synth_library.hpp"
#include "fx_mod_section/fx_mod_section.hpp"

const uint32_t sample_period_in_micros = 1000000.0 / g_sample_rate;
uint32_t last_tick;

One_Pole_Lowpass lpf;

Envelope_Follower ef;

Bit_Crusher crusher;

Mono_VCF_Control vcf_controller;

LFO lfo;
LFO clock_gen;

Two_Quadrant_Multiplier vca;

ADSR adsr;

Arduino_Analog_Input pot1(A9);
Arduino_Analog_Input pot2(A8);
Arduino_Analog_Input pot3(A7);
Arduino_Analog_Input pot4(A6);

Rotary_Encoder encoder(15, 14);

Arduino_Analog_Output dac1(A21);
Arduino_Analog_Output dac2(A22);

void setup()
{
  //Serial.begin(38400); while (!Serial) {}



  analogWriteRes(g_num_ADC_bits);
  analogReadResolution(g_num_DAC_bits);

  vca.signal_input.plugIn(&lfo.output[LFO::SINE]);
  vca.control_input.plugIn(&adsr.output);

  ef.fwr.input.plugIn(&vca.output);
  ef.decay_control.plugIn(&pot2.output);

  crusher.input.plugIn(&vca.output);
  //crusher.input.plugIn(&pot2.output);
  crusher.crush_input.plugIn(&pot1.output);


  lpf.setSampleRate(g_sample_rate);
  lfo.setSampleRate(g_sample_rate);
  adsr.setSampleRate(g_sample_rate);
  clock_gen.setSampleRate(g_sample_rate);
  ef.setSampleRate(g_sample_rate);

  lfo.setFrequencyRange(80.0, 80.0);

  lpf.setFrequencyRange(20.0, 1000.00);

  clock_gen.setFrequencyRange(2.0, 2.0);

  adsr.gate_input.plugIn(&clock_gen.output[LFO::SQUARE]);

  adsr.input[ADSR_INPUT::ATTACK_TIME].plugIn(-0.999999);
  adsr.input[ADSR_INPUT::DECAY_TIME].plugIn(-0.95);
  adsr.input[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(0.0);
  adsr.input[ADSR_INPUT::RELEASE_TIME].plugIn(-0.9);

  lfo.frequency_input.plugIn(&encoder.output);

  dac1.input.plugIn(&ef.output);
  //dac1.input.plugIn(&adsr.output);
  //dac1.input.plugIn(&pot4.output);

  lpf.cutoff_input.plugIn(&pot4.output);
  lpf.input.plugIn(&crusher.output);
  dac2.input.plugIn(&adsr.output);
  //dac2.input.plugIn(&crusher.output);
}

void loop()
{
  if (micros() - last_tick > sample_period_in_micros)
  {
    last_tick = micros();



    lpf.tick();
    lpf.process();
    vca.process();

    crusher.process();

    lfo.tick();
    lfo.process();

    ef.tick();
    ef.process();

    clock_gen.tick();
    clock_gen.process();

    adsr.tick();
    adsr.process();

    pot1.process();
    pot2.process();
    pot3.process();
    pot4.process();

    encoder.process();

    dac1.process();
    dac2.process();
  }
}
