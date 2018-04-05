#include "utilities.hpp"

void Inverter::process()
{
  output.set(-input.get());
}

void Half_Wave_Rectifier::process()
{
  output.set(clamp(input.get(), 0.0, 1.0));
}

void Full_Wave_Rectifier::process()
{
  output.set(absVal(input.get()));
}

void Bipolar_To_Unipolar_Converter::process()
{
  output.set((input.get() + 1.0) / 2.0);
}

void Unipolar_To_Bipolar_Converter::process()
{
  output.set((input.get() * 2.0) - 1.0);
}

void Mixer::process()
{
  float sum = 0;
  for (auto input_channel : input)
    sum += input_channel.get();

  output.set(clamp(sum));
}

void Sample_And_Hold::process()
{
  trigger_input.process();

  if (trigger_input.risingEdge())
    output.set(signal_input.get());
}
