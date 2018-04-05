#include "clock_generator.hpp"

void Clock_Generator::process()
{
  output.set(gateIsHigh() ? 1.0 : 0.0);
}

bool Clock_Generator::gateIsHigh() const
{
  float distance_travelled = getAccumulator() / float(0xFFFFFFFF) * 100.0;
  return distance_travelled <= gate_length;
}

void Clock_Generator::setGateLength(float percent)
{
  gate_length = clamp(percent, 1.0, 99.0);
}
