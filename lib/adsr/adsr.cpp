#include "adsr.hpp"

ADSR_Core::ADSR_Core() : state(AT_REST)
{
  fillAttackTable();
  fillDecayTable();
}

void ADSR_Core::set(ADSR_INPUT::ADSR_INPUT input, float value)
{
  switch (input)
  {
    case ADSR_INPUT::ATTACK_TIME   : attack_time   = clamp(value, 0.001, 60.0); break;
    case ADSR_INPUT::DECAY_TIME    : decay_time    = clamp(value, 0.001, 60.0); break;
    case ADSR_INPUT::SUSTAIN_LEVEL : sustain_level = clamp(value, 0.0, 1.0);    break;
    case ADSR_INPUT::RELEASE_TIME  : release_time  = clamp(value, 0.001, 60.0); break;
    default                        : return;
  }
}

void ADSR_Core::tick()
{
  if (currentStateIsTickable())
  {
    accumulator.setFrequency(1.0 / (periodOf(state)));
    accumulator.tick();

    if (accumulator.cycleHasCompleted())
    {
      accumulator.reset();
      advanceState();
    }
  }
}

void ADSR_Core::process()
{
  const int lut_index = accumulator.getAccumulator() >> 24;

  float coefficient = 1.0, sample = 0.0, offset = 0.0;

  switch (state)
  {
    case ATTACK :
      coefficient = 1.0 - value_when_gate_on_recieved;
      sample = attack_table[lut_index];
      offset = value_when_gate_on_recieved;
      break;

    case DECAY :
      coefficient = 1.0 - sustain_level;
      sample = decay_table[lut_index];
      offset = sustain_level;
      break;

    case SUSTAIN :
      sample = sustain_level;
      break;

    case RELEASE :
      coefficient = value_when_gate_off_recieved;
      sample = decay_table[lut_index];
      break;

    case AT_REST :
      break;
  }

   current_sample = coefficient * sample + offset;
}

void ADSR_Core::gateOn()
{
  if (state != ATTACK)
  {
    value_when_gate_on_recieved = current_sample;
    accumulator.reset();
    state = ATTACK;
  }
}

void ADSR_Core::gateOff()
{
  value_when_gate_off_recieved = current_sample;
  accumulator.reset();
  state = RELEASE;
}

bool ADSR_Core::currentStateIsTickable() const
{
  return state == ATTACK || state == DECAY || state == RELEASE;
}

float ADSR_Core::periodOf(ADSR_STATE state) const
{
  switch (state)
  {
    case ATTACK  : return attack_time;
    case DECAY   : return decay_time;
    case SUSTAIN : return INFINITY;
    case RELEASE : return release_time;
    case AT_REST : return INFINITY;
    default      : return 0.001;
  }
}

void ADSR_Core::advanceState()
{
  switch (state)
  {
    case ATTACK  : state = DECAY;   break;
    case DECAY   : state = SUSTAIN; break;
    case SUSTAIN : state = RELEASE; break;
    case RELEASE : state = AT_REST; break;
    case AT_REST : state = ATTACK;  break;
    default      : state = AT_REST; break;
  }
}

void ADSR_Core::fillAttackTable()
{
  const float target = 1.3;
  const float decay_factor = 0.0057;

  attack_table[0] = 0;

  for (int i = 1; i < table_length; i ++)
    attack_table[i] = attack_table[i - 1] + (target - attack_table[i - 1]) * decay_factor;
}

void ADSR_Core::fillDecayTable()
{
  const float target = 1.0;
  const float decay_factor = 0.03;

  decay_table[0] = 0;

  for (int i = 1; i < table_length; i ++)
    decay_table[i] = decay_table[i - 1] + (target - decay_table[i - 1]) * decay_factor;

  for (int i = 0; i < table_length; i ++)
    decay_table[i] = 1.0 - decay_table[i];
}

void ADSR_Core::printAttackTable() const
{
  for (int i = 1; i < table_length + 1; i ++)
  {
    Serial.print(attack_table[i - 1]);
    Serial.print(", ");
    if (i % 16 == 0)
      Serial.println();
  }
}

void ADSR_Core::printDecayTable() const
{
  for (int i = 1; table_length + 1; i ++)
  {
    Serial.print(decay_table[i - 1]);
    Serial.print(", ");
    if (i % 16 == 0)
      Serial.println();
  }
}

void ADSR::tick()
{
  gate_input.process();
  trigger_input.process();

  if (gate_input.risingEdge() || trigger_input.risingEdge())
    core.gateOn();

  if (gate_input.fallingEdge())
    core.gateOff();

  core.tick();
}

void ADSR::process()
{
  core.set(ADSR_INPUT::ATTACK_TIME, mapInput(input[ADSR_INPUT::ATTACK_TIME].get(), 0.001, 10.0));
  core.set(ADSR_INPUT::DECAY_TIME, mapInput(input[ADSR_INPUT::DECAY_TIME].get(), 0.001, 10.0));
  core.set(ADSR_INPUT::SUSTAIN_LEVEL, mapInput(input[ADSR_INPUT::SUSTAIN_LEVEL].get(), 0.0, 1.0));
  core.set(ADSR_INPUT::RELEASE_TIME, mapInput(input[ADSR_INPUT::RELEASE_TIME].get(), 0.001, 10.0));

  overall_time = input[ADSR_INPUT::OVERALL_TIME].get() * 19.0 + 1.0;

  core.process();
  output.set(core.getCurrentSample());
}
