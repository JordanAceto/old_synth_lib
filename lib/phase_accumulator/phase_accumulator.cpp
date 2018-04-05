#include "phase_accumulator.hpp"

void Phase_Accumulator::setSampleRate(float sr)
{
    sample_rate = sr;
    nyquist = sample_rate / 2.0;
 }

void Phase_Accumulator::setFrequency(float f)
{
    frequency = clamp(f, float(0.0), nyquist);
    increment = calcIncrement();
}

void Phase_Accumulator::tick()
{
    last_accumulator = accumulator;
    accumulator += increment;
}

bool Phase_Accumulator::cycleHasCompleted() const
{
    bool accumulator_has_rolled_over = accumulator < last_accumulator;

    if (accumulator_has_rolled_over)
        last_accumulator = 0;

    return accumulator_has_rolled_over;
}

uint32_t Phase_Accumulator::calcIncrement() const
{
    return (frequency * two_to_the_N) / sample_rate;
}
