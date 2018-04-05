#ifndef FX_MOD_SECTION_H_INCLUDED
#define FX_MOD_SECTION_H_INCLUDED

#include "synth_library.hpp"
#include "global_constants.hpp"

class Mono_VCF_Control : public Is_Tickable, public Is_Processable
{
public:
  Mono_VCF_Control()
  {
    analogWriteRes(num_ADC_bits);
    analogReadResolution(num_DAC_bits);

    lfo.frequency_input.plugIn(&control_input[LFO_RATE].output);

    lfo_wave_scanner.signal_input[INPUT_1].plugIn(&lfo.output[LFO::TRIANGLE]);
    lfo_wave_scanner.signal_input[INPUT_2].plugIn(&lfo.output[LFO::SINE]);
    lfo_wave_scanner.signal_input[INPUT_3].plugIn(&lfo.output[LFO::SQUARE]);
    lfo_wave_scanner.signal_input[INPUT_4].plugIn(&lfo.output[LFO::RANDOM]);
    lfo_wave_scanner.signal_input[INPUT_5].plugIn(&noise.output);

    lfo_wave_scanner.control_input.plugIn(&control_input[LFO_WAVE].output);

    lfo_output_vca.signal_input.plugIn(&lfo_wave_scanner.output);
    lfo_output_vca.control_input.plugIn(&control_input[LFO_LEVEL].output);

    ef_input_vca.signal_input.plugIn(&control_input[EF_SIGNAL_INPUT].output);
    ef_input_vca.control_input.plugIn(&control_input[EF_SENSITIVITY].output);
    ef.fwr.input.plugIn(&ef_input_vca.output);
    ef.decay_control.plugIn(&control_input[EF_ATTACK].output);
    ef_attenuverter.x_input.plugIn(&ef.output);
    ef_attenuverter.y_input.plugIn(&control_input[EF_LEVEL].output);

    mixer.input[Mixer::INPUT_1].plugIn(&control_input[INITIAL_FREQUENCY].output);
    mixer.input[Mixer::INPUT_2].plugIn(&lfo_wave_Scanner.output);
    mixer.input[Mixer::INPUT_3].plugIn(&ef_attenuverter.output);

    output_dac[MAIN_CONTROL_DAC].input.plugIn(&mixer.output);
    output_dac[RESONANCE_DAC].input.plugIn(&control_input[RESONANCE].output);
    output_dac[WET_DRY_MIX_DAC].input.plugIn(&control_input[WET_DRY_MIX].output);
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
    for (auto &input : control_input)
      input.process();

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

private:
  Arduino_Analog_Input control_input[NUM_CONTROL_INPUTS] =
  {
    Arduino_Analog_Input(A0),
    Arduino_Analog_Input(A1),
    Arduino_Analog_Input(A2),
    Arduino_Analog_Input(A3),
    Arduino_Analog_Input(A4),
    Arduino_Analog_Input(A5),
    Arduino_Analog_Input(A6),
    Arduino_Analog_Input(A7),
    Arduino_Analog_Input(A8),
    Arduino_Analog_Input(A9)
  };

  LFO lfo;
  White_Noise_Generator noise;
  Five_Input_Scanner lfo_wave_scanner;
  Envelope_Follower ef;

  VCA lfo_output_vca;
  VCA ef_input_vca;
  Ring_Mod ef_attenuverter;

  Mixer mixer;

  enum OUTPUT_DAC
  {
    MAIN_CONTROL_DAC,
    RESONANCE_DAC,
    WET_DRY_MIX_DAC,

    NUM_OUTPUT_DACS
  };

  Arduino_Analog_Output output_dac[NUM_OUTPUT_DACS] =
  {
    Arduino_Analog_Output(A14),
    Arduino_Analog_Output(9),
    Arduino_Analog_Output(10)
  };
};

#endif
