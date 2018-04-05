#include "multipliers.hpp"

void VCA::process()
{
  output.set(clamp(control_input.get(), 0.0, 1.0) * signal_input.get());
}

void Ring_Mod::process()
{
  output.set(x_input.get() * y_input.get());
}
