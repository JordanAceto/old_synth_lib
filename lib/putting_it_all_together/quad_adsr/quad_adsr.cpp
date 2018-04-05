#include "quad_adsr.hpp"

Quad_ADSR::Quad_ADSR()
{
  analogWriteRes(num_ADC_bits);
  analogReadResolution(num_DAC_bits);

  for (int channel = 0; channel < NUM_ADSR_CHANNELS; channel ++)
  {
    adsr[channel].gate_input.plugIn(&gate_in[channel].output);
    adsr[channel].trigger_input.plugIn(&trigger_in[channel].output);
    pwm_dac[channel].input.plugIn(&adsr[channel].output);
  }
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
  for (int channel = 0; channel < NUM_ADSR_CHANNELS; channel ++)
  {
    pot[channel].process();
    pwm_dac[channel].process();
    adsr[channel].process();
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
