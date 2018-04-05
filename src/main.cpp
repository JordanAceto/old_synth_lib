#include <Arduino.h>

#include "synth_library.hpp"
#include "mini_synth/mini_synth.hpp"

const uint32_t sample_period_in_micros = 1000000.0 / sample_rate;
uint32_t last_tick;

Five_Input_Scanner scanner;
LFO lfo;
White_Noise_Generator noise;

Arduino_Analog_Input pot(A8);

Arduino_Analog_Output dac(A21);

void setup()
{
  //Serial.begin(38400); while (!Serial) {}
  analogWriteRes(num_ADC_bits);
  analogReadResolution(num_DAC_bits);

  lfo.setSampleRate(sample_rate);
  lfo.setFrequency(100.0);

  scanner.signal_input[0].plugIn(&lfo.output[LFO::TRIANGLE]);
  scanner.signal_input[1].plugIn(&lfo.output[LFO::SINE]);
  scanner.signal_input[2].plugIn(&lfo.output[LFO::SQUARE]);
  scanner.signal_input[3].plugIn(&lfo.output[LFO::RANDOM]);
  scanner.signal_input[4].plugIn(&noise.output);
  scanner.control_input.plugIn(&pot.output);



  dac.input.plugIn(&lfo.output[LFO::SINE]);
  dac.input.plugIn(&scanner.output);
}

void loop()
{
  if (micros() - last_tick > sample_period_in_micros)
  {
    last_tick = micros();

    lfo.tick();
    lfo.process();
    noise.process();
    pot.process();
    scanner.process();
    dac.process();
  }
}
