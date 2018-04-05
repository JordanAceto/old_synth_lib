#ifndef PHASE_ACCUMULATOR_H_INCLUDED
#define PHASE_ACCUMULATOR_H_INCLUDED

#include <stdint.h>

#include "helper_functions.hpp"
#include "base_classes.hpp"

class Phase_Accumulator : public Is_Tickable
{
public :
    Phase_Accumulator() :
        frequency(0),
        accumulator(0),
        last_accumulator(0),
        increment(0)
    {}

    virtual ~Phase_Accumulator() {}

    void setSampleRate(float sr) override;
    void setFrequency(float f);

    float getSampleRate() const { return sample_rate; }
    float getFrequency()  const { return frequency; }

    void tick() override;

    bool cycleHasCompleted() const;
    void reset() { accumulator = 0; }

    uint32_t getAccumulator() const { return accumulator; }

private :
    float nyquist, frequency;
    uint32_t accumulator;
    mutable uint32_t last_accumulator;
    uint32_t increment;
    static const uint32_t two_to_the_N = (1ULL << 32) - 1;

    uint32_t calcIncrement() const;
};

#endif // PHASE_ACCUMULATOR_H_INCLUDED
