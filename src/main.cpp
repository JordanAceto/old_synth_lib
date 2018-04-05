#include <Arduino.h>

#include "synth_library.hpp"
#include "mini_synth.hpp"

const uint32_t sample_period_in_micros = 1000000.0 / sample_rate;
uint32_t last_tick;

Mini_Synth mini_synth;

Arduino_Analog_Input pot1(A9);
Bipolar_To_Unipolar_Converter pot_converter;

Arduino_Digital_Input gate_in(A8);

Arduino_Analog_Output built_in_DAC(A14);

Arduino_Digital_Output clock_out(A0);

Four_Pole_Lowpass_Filter lpf;

LFO lfo;
Clock_Generator auto_gate;
ADSR adsr;
Sample_And_Hold s_and_h;
White_Noise_Generator noise;
VCA vca;
Ring_Mod ring_mod;

Envelope_Follower ef;

Mixer mixer;

Output_Interface fixed_output;
Output_Interface half_way;
Output_Interface minimum;

void setup()
{
  //Serial.begin(38400); while (!Serial) {}

  mini_synth.setSampleRate(sample_rate);

  analogWriteRes(num_ADC_bits);
  analogReadResolution(num_DAC_bits);
  pinMode(A0, OUTPUT);

  ef.setSampleRate(sample_rate);

  lfo.setSampleRate(sample_rate);
  lfo.setFrequency(15.0);

  auto_gate.setSampleRate(sample_rate);
  auto_gate.setFrequency(10.0);
  auto_gate.setGateLength(50.0);

  adsr.setSampleRate(sample_rate);

  fixed_output.set(-0.99);
  half_way.set(0.0);
  minimum.set(-1.0);

  adsr.input[ADSR_INPUT::ATTACK_TIME].setGain(0.05);

  //adsr.input[ADSR_INPUT::ATTACK_TIME].plugIn(&pot1.output);
  adsr.input[ADSR_INPUT::DECAY_TIME].plugIn(&pot1.output);
  adsr.input[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(&half_way);
  adsr.input[ADSR_INPUT::RELEASE_TIME].plugIn(&pot1.output);
  adsr.input[ADSR_INPUT::OVERALL_TIME].plugIn(&half_way);
/*
  adsr.input[ADSR::ATTACK_TIME].unplug();
  adsr.input[ADSR::DECAY_TIME].unplug();
  adsr.input[ADSR::SUSTAIN_LEVEL].unplug();
  adsr.input[ADSR::RELEASE_TIME].unplug();
*/
  adsr.gate_input.plugIn(&auto_gate.output);
  adsr.gate_input.unplug();
  adsr.trigger_input.plugIn(&lfo.output[LFO::SQUARE]);

  pot_converter.input.plugIn(&pot1.output);

  lpf.input.plugIn(&lfo.output[LFO::SQUARE]);
  lpf.cutoff_input.plugIn(&pot_converter.output);

  lfo.sync_input.plugIn(&auto_gate.output);
  lfo.sync_input.unplug();
  lfo.frequency_input.plugIn(&pot1.output);
  lfo.frequency_input.unplug();

  ef.fwr.input.plugIn(&vca.output);
  ef.fwr.input.unplug();
  ef.decay_control.plugIn(&pot_converter.output);

  s_and_h.signal_input.plugIn(&noise.output);
  s_and_h.trigger_input.plugIn(&auto_gate.output);

  vca.signal_input.plugIn(&lfo.output[LFO::SINE]);
  vca.control_input.plugIn(&adsr.output);

  ring_mod.x_input.plugIn(&lfo.output[LFO::UP_SAW]);
  ring_mod.y_input.plugIn(&pot1.output);

  mixer.input[Mixer::INPUT_1].plugIn(&lfo.output[LFO::SINE]);
  mixer.input[Mixer::INPUT_1].setGain(0.0);

  mixer.input[Mixer::INPUT_2].plugIn(&adsr.output);
  mixer.input[Mixer::INPUT_2].setGain(1.0);

  mixer.input[Mixer::INPUT_3].plugIn(&s_and_h.output);
  mixer.input[Mixer::INPUT_3].setGain(0.0);

  mixer.input[Mixer::INPUT_4].plugIn(&ef.output);
  mixer.input[Mixer::INPUT_4].setGain(0.0);

  mixer.input[Mixer::INPUT_5].plugIn(&vca.output);
  mixer.input[Mixer::INPUT_5].setGain(0.0);

  built_in_DAC.input.plugIn(&mixer.output);

  clock_out.input.plugIn(&auto_gate.output);


}

void loop()
{
  if (micros() - last_tick > sample_period_in_micros)
  {
    last_tick = micros();

    mini_synth.tick();
    mini_synth.process();

    //digitalWrite(A0, HIGH);
    //digitalWrite(A0, LOW);
/*
    ef.tick();
    lfo.tick();
    auto_gate.tick();
    adsr.tick();
    lpf.tick();

    static int counts;
    counts ++;
    if (counts % 4 == 0)
    {
      //digitalWrite(A0, HIGH);
      //digitalWrite(A0, LOW);

      pot1.process();
      pot_converter.process();
      gate_in.process();

      ef.process();

      lpf.process();

      lfo.process();

      vca.process();
      ring_mod.process();

      auto_gate.process();

      adsr.process();

      s_and_h.process();
      noise.process();

      mixer.process();

      built_in_DAC.process();
      clock_out.process();

    }
    */
  }
}
