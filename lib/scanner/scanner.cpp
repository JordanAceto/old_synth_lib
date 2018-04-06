#include "scanner.hpp"

Trapezoid_Generator::Trapezoid_Generator()
{
  input.setGain(0.5);
  input.setOffset(0.5);
}

void Trapezoid_Generator::process()
{
  const float plateau_start = center - (0.5 * width);
  const float plateau_end = center + (0.5 * width);

  if (input.get() < plateau_start)
    output.set(clamp(input.get() * slope + (1 - slope * plateau_start), 0.0, 1.0));
  else if (plateau_start <= input.get() && input.get() < plateau_end)
    output.set(1.0);
  else if (input.get() >= plateau_end)
    output.set(clamp(input.get() * -slope + (1 + slope * plateau_end), 0.0, 1.0));
}

Five_Input_Scanner::Five_Input_Scanner()
{
  for (int i = 0; i < NUM_INPUTS; i ++)
  {
    trapezoid[i].input.plugIn(&control_input.output);
    trapezoid[i].setWidth(1.0 / (2.5 * NUM_INPUTS));
    trapezoid[i].setSlope(1.0 + NUM_INPUTS);
    trapezoid[i].setCenter((float)i / (NUM_INPUTS - 1.0));
  }
}

void Five_Input_Scanner::process()
{
  control_input.process();

  for (int i = 0; i < NUM_INPUTS; i ++)
  {
    trapezoid[i].process();
    signal.input[i].setGain(trapezoid[i].output.get());
  }

  signal.process();
  output.set(signal.output.get());
}
