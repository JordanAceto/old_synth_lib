#include "quad_adsr.hpp"

Quad_ADSR::Quad_ADSR()
{
  analogWriteRes(num_ADC_bits);
  analogReadResolution(num_DAC_bits);

  setSampleRate(sample_rate);

  for (int i = 0; i < NUM_ADSR_CHANNELS; i ++)
  {
    adsr[i].gate_input.plugIn(&gate_in[i].output);
    adsr[i].trigger_input.plugIn(&trigger_in[i].output);
    pwm_dac[i].input.plugIn(&adsr[i].output);
  }
}

void Quad_ADSR::setSampleRate(float sample_rate)
{
  for (auto &env : adsr)
    env.setSampleRate(sample_rate);
}

void Quad_ADSR::tick()
{
  for (auto &env : adsr)
    env.tick();
}

void Quad_ADSR::process()
{
  for (int i = 0; i < NUM_ADSR_CHANNELS; i ++)
  {
    pot[i].process();
    pwm_dac[i].process();
    adsr[i].process();
  }
}

void Quad_ADSR::assignPotsTo(ADSR_CHANNEL new_channel)
{
  last_channel = active_channel;
  active_channel = new_channel;

  adsr[last_channel].input[ADSR_INPUT::ATTACK_TIME].unplug();
  adsr[last_channel].input[ADSR_INPUT::DECAY_TIME].unplug();
  adsr[last_channel].input[ADSR_INPUT::SUSTAIN_LEVEL].unplug();
  adsr[last_channel].input[ADSR_INPUT::RELEASE_TIME].unplug();

  adsr[active_channel].input[ADSR_INPUT::ATTACK_TIME].plugIn(&pot[ATTACK_POT].output);
  adsr[active_channel].input[ADSR_INPUT::DECAY_TIME].plugIn(&pot[DECAY_POT].output);
  adsr[active_channel].input[ADSR_INPUT::SUSTAIN_LEVEL].plugIn(&pot[SUSTAIN_POT].output);
  adsr[active_channel].input[ADSR_INPUT::RELEASE_TIME].plugIn(&pot[RELEASE_POT].output);
}
