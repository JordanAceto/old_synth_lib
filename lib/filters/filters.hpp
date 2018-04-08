#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "inputs_and_outputs.hpp"
#include "base_classes.hpp"

#include <math.h>

class One_Pole_Lowpass_Core : public Is_Tickable
{
public:

  void setCutoffFrequency(float Fc) // I'm unsure of the correctness of this
  {
    Fc = Fc / sample_rate;
    b1 = exp(-2.0 * g_pi * Fc);
    a0 = 1.0 - b1;
  }

  void tick() override { z1 += (input - z1) * a0; }

  void loadInput(float input) { this->input = input; }
  float getOutput() const { return z1; }

private:
  float input, a0, b1, z1;
};

class One_Pole_Lowpass : public Is_Tickable, public Is_Processable, public Has_Frequency_Range
{
public:
  Signal_Input input;
  Signal_Input cutoff_input;
  Output_Interface output;

  void setSampleRate(float sample_rate) override { core.setSampleRate(sample_rate); }

  void tick() override
  {
    core.loadInput(input.get());
    core.tick();
    output.set(core.getOutput());
  }

  void process() override
  {
    core.setCutoffFrequency(mapInput(cutoff_input.get(), F1, F2));
  }

private:
  One_Pole_Lowpass_Core core;
};

class Four_Pole_Lowpass : public Is_Tickable, public Is_Processable, public Has_Frequency_Range
{
public:
  Four_Pole_Lowpass();

  void setFrequencyRange(float F1, float F2) override
  {
    for (auto &s : stage)
      s.setFrequencyRange(F1, F2);
  }

  void tick() override;
  void process() override;

  Signal_Input_Feedthrough input;
  Signal_Input_Feedthrough cutoff_input;
  Output_Interface output;

private:
  One_Pole_Lowpass stage[4];
};

#endif
