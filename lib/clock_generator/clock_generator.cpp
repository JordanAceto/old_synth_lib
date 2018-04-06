#include "clock_generator.hpp"

Clock_Generator::Clock_Generator()
{
  frequency_input.setGain(0.5);
  frequency_input.setOffset(0.5);

  gate_length_input.setGain(0.5);
  gate_length_input.setOffset(0.5);
}

void Clock_Generator::setSampleRate(float sample_rate)
{
  core.setSampleRate(sample_rate);
}

void Clock_Generator::tick()
{
  core.tick();
}

void Clock_Generator::process()
{
  setGateLength(gate_length_input.get() * 100.0);
  core.setFrequency(frequency_input.get() * 300 + 0.01);
  output.set(gateIsHigh() ? 1.0 : 0.0);
}

bool Clock_Generator::gateIsHigh() const
{
  float distance_travelled = core.getAccumulator() / float(0xFFFFFFFF) * 100.0;
  return distance_travelled <= gate_length;
}

void Clock_Generator::setGateLength(float percent)
{
  gate_length = clamp(percent, 1.0, 99.0);
}
