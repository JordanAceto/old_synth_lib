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

Four_Pole_Lowpass_Filter::Four_Pole_Lowpass_Filter()
{
  stage[0].input.plugIn(&input.output);

  for (int i = 1; i < 4; i ++)
    stage[i].input.plugIn(&stage[i - 1].output);

  for (auto &s : stage)
    s.cutoff_input.plugIn(&cutoff_input.output);
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
