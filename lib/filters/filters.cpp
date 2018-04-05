#include "filters.hpp"

void One_Pole_Lowpass_Filter::tick()
{
  sample += cutoff * (input.get() - sample);
}

void One_Pole_Lowpass_Filter::process()
{
  cutoff = clamp(cutoff_input.get(), 0.0001, 0.999999999);
  output.set(sample);
}

void Four_Pole_Lowpass_Filter::tick()
{
  for (auto &s : stage)
    s.tick();
}

void Four_Pole_Lowpass_Filter::process()
{
  input.process();
  cutoff_input.process();
  for (auto &s : stage)
    s.process();
  output.set(stage[3].output.get());
}
