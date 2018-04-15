#include <Arduino.h>

#include "synth_library.hpp"
#include "fx_mod_section/fx_mod_section.hpp"
#include "quad_adsr/quad_adsr.hpp"

const uint32_t sample_period_in_micros = 1000000.0 / g_sample_rate;
uint32_t last_tick;

Quad_ADSR quad_adsr;

LFO clock_gen;
Arduino_Digital_Output clock_out(33);

Rotary_Encoder encoder(15, 14);

void setup()
{
  //Serial.begin(38400); while (!Serial) {}



  analogWriteRes(g_num_ADC_bits);
  analogReadResolution(g_num_DAC_bits);



  quad_adsr.setSampleRate(g_sample_rate);
  clock_gen.setSampleRate(g_sample_rate);

  clock_gen.setFrequencyRange(5.0, 5.0);
  clock_out.input.plugIn(&clock_gen.output[LFO::SQUARE]);



}

void loop()
{
  if (micros() - last_tick > sample_period_in_micros)
  {
    last_tick = micros();



    clock_gen.tick();
    clock_gen.process();
    clock_out.process();

    quad_adsr.tick();
    quad_adsr.process();

    encoder.process();

  }
}
