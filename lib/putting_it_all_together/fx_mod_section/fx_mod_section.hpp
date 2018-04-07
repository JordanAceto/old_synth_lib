#ifndef FX_MOD_SECTION_H_INCLUDED
#define FX_MOD_SECTION_H_INCLUDED

#include "synth_library.hpp"
#include "global_constants.hpp"

/*

Unfinished concept for modulation controls for a digitally controlled analog
filter. This general setup is similar to what I've used in the past, but this
time implemented digitally using a teensy 3.6.

Basic idea: flexible LFO with various shapes and an envelope follower which can
sweep up and down.

The LFO concept is taken from Jurgen Haible's variable slope filter/phaser
project.

This is an unfinished work in progress.
For now, this is here as an idea of one application of this library.

*/

class Mono_VCF_Control : public Is_Tickable, public Is_Processable
{
public:
  Mono_VCF_Control() : encoder { {24, 25},  {26, 27}, {28, 28} }, // hardwired
                       output_dac { A14, 9, 10 }                  // io pins, eek
  {
    analogWriteRes(g_num_ADC_bits);
    analogReadResolution(g_num_DAC_bits);

    lfo_wave_scanner.signal.input[0].plugIn(&lfo.output[LFO::TRIANGLE]);
    lfo_wave_scanner.signal.input[1].plugIn(&lfo.output[LFO::SINE]);
    lfo_wave_scanner.signal.input[2].plugIn(&lfo.output[LFO::SQUARE]);
    lfo_wave_scanner.signal.input[3].plugIn(&lfo.output[LFO::RANDOM]);
    lfo_wave_scanner.signal.input[4].plugIn(&noise.output);

    lfo_output_vca.signal_input.plugIn(&lfo_wave_scanner.output);

    ef.fwr.input.plugIn(&ef_input_vca.output);
    ef_attenuverter.x_input.plugIn(&ef.output);

    mixer.input[Mixer::INPUT_2].plugIn(&lfo_wave_scanner.output);
    mixer.input[Mixer::INPUT_3].plugIn(&ef_attenuverter.output);

    output_dac[MAIN_CONTROL_DAC].input.plugIn(&mixer.output);
  }

  void setSampleRate(float sample_rate) override
  {
    lfo.setSampleRate(sample_rate);
    ef.setSampleRate(sample_rate);
  }

  void tick() override
  {
    lfo.tick();
    ef.tick();
  }

  void process() override
  {
    for (auto &enc : encoder)
      enc.process();

    for (auto &dac : output_dac)
      dac.process();

    lfo.process();
    noise.process();
    lfo_wave_scanner.process();
    lfo_output_vca.process();

    ef.process();
    ef_input_vca.process();
    ef_attenuverter.process();

    mixer.process();
  }

  enum CONTROL_INPUT
  {
    INITIAL_FREQUENCY = 0,
    RESONANCE,
    WET_DRY_MIX,

    LFO_RATE,
    LFO_WAVE,
    LFO_LEVEL,

    EF_SIGNAL_INPUT,

    EF_SENSITIVITY,
    EF_ATTACK,
    EF_LEVEL,

    NUM_CONTROL_INPUTS
  };

  enum CONTROL_PAGE
  {
    GLOBAL_SETTINGS,
    LFO_SETTINGS,
    EF_SETTINGS,

    NUM_PAGES
  };

  enum Encoders
  {
    ENCODER_1,
    ENCODER_2,
    ENCODER_3,

    NUM_ENCODERS
  };

  void setActivePage(CONTROL_PAGE page)
  {
    mixer.input[Mixer::INPUT_1].unplug();
    output_dac[RESONANCE_DAC].input.unplug();
    output_dac[WET_DRY_MIX_DAC].input.unplug();

    lfo.frequency_input.unplug();
    lfo_wave_scanner.control_input.unplug();
    lfo_output_vca.control_input.unplug();

    ef_input_vca.control_input.unplug();
    ef.decay_control.unplug();
    ef_attenuverter.y_input.unplug();

    switch (page)
    {
      case GLOBAL_SETTINGS :
        encoder[ENCODER_1].set(mixer.input[Mixer::INPUT_1].get());
        encoder[ENCODER_2].set(output_dac[RESONANCE_DAC].input.get());
        encoder[ENCODER_3].set(output_dac[WET_DRY_MIX].input.get());
        mixer.input[Mixer::INPUT_1].plugIn(&encoder[ENCODER_1].output);
        output_dac[RESONANCE_DAC].input.plugIn(&encoder[ENCODER_2].output);
        output_dac[WET_DRY_MIX_DAC].input.plugIn(&encoder[ENCODER_3].output);
        break;

      case LFO_SETTINGS :
        encoder[ENCODER_1].set(lfo.frequency_input.get());
        encoder[ENCODER_2].set(lfo_wave_scanner.control_input.get());
        encoder[ENCODER_3].set(lfo_output_vca.control_input.get());
        lfo.frequency_input.plugIn(&encoder[ENCODER_1].output);
        lfo_wave_scanner.control_input.plugIn(&encoder[ENCODER_2].output);
        lfo_output_vca.control_input.plugIn(&encoder[ENCODER_3].output);
        break;

      case EF_SETTINGS :
        encoder[ENCODER_1].set(ef_input_vca.control_input.get());
        encoder[ENCODER_2].set(ef.decay_control.get());
        encoder[ENCODER_3].set(ef_attenuverter.y_input.get());
        ef_input_vca.control_input.plugIn(&encoder[ENCODER_1].output);
        ef.decay_control.plugIn(&encoder[ENCODER_2].output);
        ef_attenuverter.y_input.plugIn(&encoder[ENCODER_3].output);
        break;

      default :
        break;
    }
  }

private:

  Rotary_Encoder encoder[NUM_ENCODERS];

  LFO lfo;
  White_Noise_Generator noise;
  Five_Input_Scanner lfo_wave_scanner;
  Envelope_Follower ef;

  Two_Quadrant_Multiplier lfo_output_vca;
  Two_Quadrant_Multiplier ef_input_vca;
  Four_Quadrant_Multiplier ef_attenuverter;

  Mixer mixer;

  enum OUTPUT_DAC
  {
    MAIN_CONTROL_DAC,
    RESONANCE_DAC,
    WET_DRY_MIX_DAC,

    NUM_OUTPUT_DACS
  };

  Arduino_Analog_Output output_dac[NUM_OUTPUT_DACS];
};

#endif
