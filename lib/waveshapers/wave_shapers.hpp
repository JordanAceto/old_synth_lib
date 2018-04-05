#ifndef WAVE_SHAPERS_H_INCLUDED
#define WAVE_SHAPERS_H_INCLUDED

#include "phase_accumulator.hpp"

#include <math.h> // for sine

class Waveshaper_Interface
{
public:
  virtual ~Waveshaper_Interface() {}
  virtual float process(const Phase_Accumulator &core) const = 0;
};

class Sine_Shaper : public Waveshaper_Interface
{
public:
    Sine_Shaper() { fillLut(); }

    virtual float process(const Phase_Accumulator &core) const override
    {
      return lut[core.getAccumulator() >> 24];
    }

private:
    static const int table_length = 256;
    float lut[table_length];

    void fillLut()
    {
        const float pi = 3.1415927;
        for (int i = 0; i < table_length; i ++)
            lut[i] = sin(2.0 * pi * i / table_length);
    }
};

class Naive_Triangle_Shaper : public Waveshaper_Interface
{
  virtual float process(const Phase_Accumulator &core) const override
  {
    const uint32_t quarter_scale = 0x40000000,
                   half_scale    = 0x80000000,
                   full_scale    = 0xFFFFFFFF;

    uint32_t phase_shifted_core = core.getAccumulator() + quarter_scale;

    if (phase_shifted_core < half_scale)
      return phase_shifted_core / float(quarter_scale) - 1.0;
    else
      return (full_scale - phase_shifted_core) / float(quarter_scale) - 1.0;
  }
};

class Naive_Up_Saw_Shaper : public Waveshaper_Interface
{
  virtual float process(const Phase_Accumulator &core) const override
  {
    return core.getAccumulator() / float(0x80000000) - 1.0;
  }
};

class Naive_Down_Saw_Shaper : public Waveshaper_Interface
{
  virtual float process(const Phase_Accumulator &core) const override
  {
    return (0xFFFFFFFF - core.getAccumulator()) / float(0x80000000) - 1.0;
  }
};

class Naive_Square_Shaper : public Waveshaper_Interface
{
  virtual float process(const Phase_Accumulator &core) const override
  {
    return core.getAccumulator() < 0x80000000 ? 1.0 : -1.0;
  }
};

class Random_Shaper : public Waveshaper_Interface
{
  virtual float process(const Phase_Accumulator &core) const override
  {
    if (core.cycleHasCompleted())
        sample = (float)rand() / RAND_MAX * 2.0 - 1.0;
    return sample;
  }

private:
  mutable float sample;
};


#endif
