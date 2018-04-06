#include "multipliers.hpp"

void Two_Quadrant_Multiplier::process()
{
  output.set(clamp(control_input.get(), 0.0, 1.0) * signal_input.get());
}

void Four_Quadrant_Multiplier::process()
{
  output.set(x_input.get() * y_input.get());
}
