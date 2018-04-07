#include "filters.hpp"

Four_Pole_Lowpass::Four_Pole_Lowpass()
{
  stage[0].input.plugIn(&input.output);

  for (int i = 1; i < 4; i ++)
    stage[i].input.plugIn(&stage[i - 1].output);

  for (auto &s : stage)
    s.cutoff_input.plugIn(&cutoff_input.output);
}

void Four_Pole_Lowpass::tick()
{
  for (auto &s : stage)
    s.tick();
}

void Four_Pole_Lowpass::process()
{
  input.process();
  cutoff_input.process();
  for (auto &s : stage)
    s.process();
  output.set(stage[3].output.get());
}
