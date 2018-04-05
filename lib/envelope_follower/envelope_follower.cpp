#include "envelope_follower.hpp"


Envelope_Follower::Envelope_Follower()
{
  lpf.input.plugIn(&fwr.output);
  lpf.cutoff_input.plugIn(&decay_control.output);
  decay_control.setGain(0.03);
}

void Envelope_Follower::setSampleRate(float sample_rate)
{
  Is_Tickable::setSampleRate(sample_rate);
  lpf.setSampleRate(sample_rate);
}

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
