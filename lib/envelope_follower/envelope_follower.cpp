#include "envelope_follower.hpp"

void Envelope_Follower::tick()
{
  if (fwr.output.get() > peak)
    peak = fwr.output.get();
  else
    peak = peak > 0.0 ? peak - 0.005 : 0.0;

  fwr.output.set(peak);

  lpf.tick();
}

void Envelope_Follower::process()
{
  if (fwr.input.isPluggedIn())
    fwr.process();

  if (decay_control.isPluggedIn())
    decay_control.process();

  lpf.process();

  output.set(lpf.output.get());
}
