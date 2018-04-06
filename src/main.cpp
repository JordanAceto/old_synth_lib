#include <Arduino.h>

#include "synth_library.hpp"
#include "mini_synth/mini_synth.hpp"
#include "fx_mod_section/fx_mod_section.hpp"

const uint32_t sample_period_in_micros = 1000000.0 / sample_rate;
uint32_t last_tick;

Five_Input_Scanner scanner;
LFO lfo;
White_Noise_Generator noise;

Trapezoid_Generator trap;

Arduino_Analog_Input pot(A8);
Arduino_Analog_Input pot2(A9);

Arduino_Analog_Output dac1(A21);
Arduino_Analog_Output dac2(A22);

void setup()
{
  //Serial.begin(38400); while (!Serial) {}
  analogWriteRes(num_ADC_bits);
  analogReadResolution(num_DAC_bits);

  lfo.setSampleRate(sample_rate);
  lfo.setFrequency(100.0);

  lfo.frequency_input.plugIn(&pot2.output);

  scanner.signal.input[Mixer::INPUT_1].plugIn(&lfo.output[LFO::TRIANGLE]);
  scanner.signal.input[Mixer::INPUT_2].plugIn(&lfo.output[LFO::SINE]);
  scanner.signal.input[Mixer::INPUT_3].plugIn(&lfo.output[LFO::SQUARE]);
  scanner.signal.input[Mixer::INPUT_4].plugIn(&lfo.output[LFO::RANDOM]);
  scanner.signal.input[Mixer::INPUT_5].plugIn(&noise.output);
  scanner.control_input.plugIn(&pot.output);
  //scanner.control_input.plugIn(&lfo.output[LFO::UP_SAW]);


  trap.input.plugIn(&lfo.output[LFO::UP_SAW]);
  trap.setCenter(0.5);
  trap.setWidth(0.03);
  trap.setSlope(10.0);


  dac1.input.plugIn(&lfo.output[LFO::SINE]);
  dac1.input.plugIn(&scanner.output);
  //dac1.input.plugIn(&trap.output);

  dac2.input.plugIn(&lfo.output[LFO::UP_SAW]);
}

void loop()
{
  if (micros() - last_tick > sample_period_in_micros)
  {
    last_tick = micros();

    trap.process();

    lfo.tick();
    lfo.process();
    noise.process();
    pot.process();
    pot2.process();
    scanner.process();
    dac1.process();
    dac2.process();
  }
}
