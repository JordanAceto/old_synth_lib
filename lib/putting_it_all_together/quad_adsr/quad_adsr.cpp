#include "quad_adsr.hpp"

Quad_ADSR::Quad_ADSR()
{
  analogWriteRes(g_num_ADC_bits);
  analogReadResolution(g_num_DAC_bits);

  for (int channel = 0; channel < NUM_ADSR_CHANNELS; channel ++)
  {
    adsr[channel].gate_input.plugIn(&gate_in[channel].output);
    adsr[channel].trigger_input.plugIn(&trigger_in[channel].output);
    pwm_dac[channel].input.plugIn(&adsr[channel].output);
  }

  channel_cycle_in.plugIn(&channel_cycle_button.output);

  assignPotsToAllChannels();
}

void Quad_ADSR::setSampleRate(float sample_rate)
{
  for (auto &env : adsr)
    env.setSampleRate(sample_rate);
}

void Quad_ADSR::tick()
{
  for (int channel = 0; channel < NUM_ADSR_CHANNELS; channel ++)
  {
    gate_in[channel].process();
    trigger_in[channel].process();
    adsr[channel].tick();
  }
}

void Quad_ADSR::process()
{
  for (int input = 0; input < NUM_POTS; input++)
  {
    pot[input].process();
    input_feedthrough[input].process();
  }

  for (int channel = 0; channel < NUM_ADSR_CHANNELS; channel ++)
  {
    adsr[channel].process();
    pwm_dac[channel].process();
  }

  channel_cycle_button.process();
  channel_cycle_in.process();

  if (channel_cycle_in.risingEdge())
  {
    int new_channel = active_channel + 1;
    new_channel %= NUM_ADSR_CHANNELS;
    assignPotsTo(new_channel);
  }
}

void Quad_ADSR::assignPotsTo(int new_channel)
{
  active_channel = new_channel;

  for (int channel = 0; channel < NUM_ADSR_CHANNELS; channel++)
    for (int pot = 0; pot < NUM_POTS; pot++)
      adsr[channel].input[pot].unplug();

  for (auto &in : input_feedthrough)
    in.unplug();

  float attack_time_before_changing_channel = adsr[active_channel].input[ADSR_INPUT::ATTACK_TIME].get();
  input_feedthrough[ADSR_INPUT::ATTACK_TIME].plugIn(attack_time_before_changing_channel);

  float decay_time_before_changing_channel = adsr[active_channel].input[ADSR_INPUT::DECAY_TIME].get();
  input_feedthrough[ADSR_INPUT::DECAY_TIME].plugIn(decay_time_before_changing_channel);

  float sustain_level_before_changing_channel = adsr[active_channel].input[ADSR_INPUT::SUSTAIN_LEVEL].get();
  input_feedthrough[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(sustain_level_before_changing_channel);

  float release_time_before_changing_channel = adsr[active_channel].input[ADSR_INPUT::RELEASE_TIME].get();
  input_feedthrough[ADSR_INPUT::RELEASE_TIME].plugIn(release_time_before_changing_channel);

  input_feedthrough[ADSR_INPUT::ATTACK_TIME].plugIn(&pot[ATTACK_POT].output);
  input_feedthrough[ADSR_INPUT::DECAY_TIME].plugIn(&pot[DECAY_POT].output);
  input_feedthrough[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(&pot[SUSTAIN_POT].output);
  input_feedthrough[ADSR_INPUT::RELEASE_TIME].plugIn(&pot[RELEASE_POT].output);

  adsr[active_channel].input[ADSR_INPUT::ATTACK_TIME].plugIn(&input_feedthrough[ATTACK_POT].output);
  adsr[active_channel].input[ADSR_INPUT::DECAY_TIME].plugIn(&input_feedthrough[DECAY_POT].output);
  adsr[active_channel].input[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(&input_feedthrough[SUSTAIN_POT].output);
  adsr[active_channel].input[ADSR_INPUT::RELEASE_TIME].plugIn(&input_feedthrough[RELEASE_POT].output);
}

void Quad_ADSR::assignPotsToAllChannels()
{
  for (int channel = 0; channel < NUM_ADSR_CHANNELS; channel ++)
  {
    adsr[channel].input[ADSR_INPUT::ATTACK_TIME].plugIn(&pot[ATTACK_POT].output);
    adsr[channel].input[ADSR_INPUT::DECAY_TIME].plugIn(&pot[DECAY_POT].output);
    adsr[channel].input[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(&pot[SUSTAIN_POT].output);
    adsr[channel].input[ADSR_INPUT::RELEASE_TIME].plugIn(&pot[RELEASE_POT].output);
  }
}
