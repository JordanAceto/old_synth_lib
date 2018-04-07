#ifndef ADSR_H_INCLUDED
#define ADSR_H_INCLUDED

#include "phase_accumulator.hpp"
#include "inputs_and_outputs.hpp"
#include "helper_functions.hpp"
#include "base_classes.hpp"
#include "utilities.hpp"

#include <Arduino.h>

namespace ADSR_INPUT
{
enum ADSR_INPUT
{
  ATTACK_TIME = 0,
  DECAY_TIME,
  SUSTAIN_LEVEL,
  RELEASE_TIME,
  OVERALL_TIME,

  NUM_INPUTS
};
}


class ADSR_Core : public Is_Tickable, public Is_Processable
{
public:
  ADSR_Core();

  void setSampleRate(float sr) override { accumulator.setSampleRate(sr); }

  enum ADSR_STATE
  {
    ATTACK = 0,
    DECAY,
    SUSTAIN,
    RELEASE,
    AT_REST
  };

  void set(ADSR_INPUT::ADSR_INPUT input, float value);

  void tick() override;
  void process() override;

  void gateOn();
  void gateOff();

  float getCurrentSample() const { return current_sample; }

  bool stateIs(ADSR_STATE s) const { return state == s; }
  ADSR_STATE getState()      const { return state; }

  void printAttackTable() const;
  void printDecayTable() const;

private:
  ADSR_STATE state;

  float attack_time, decay_time, sustain_level, release_time,
        value_when_gate_on_recieved, value_when_gate_off_recieved,
        current_sample;

  bool currentStateIsTickable() const;

  float periodOf(ADSR_STATE) const;

  void advanceState();

  Phase_Accumulator accumulator;

  static const int table_length = 256;

  float attack_table[table_length];
  void fillAttackTable();

  float decay_table[table_length];
  void fillDecayTable();
};

class ADSR : public Is_Tickable, public Is_Processable
{
public:
  void setSampleRate(float sample_rate) override { core.setSampleRate(sample_rate); }

  void set(ADSR_INPUT::ADSR_INPUT input, float value) { core.set(input, value); }

  Signal_Input input[ADSR_INPUT::NUM_INPUTS];
  Gate_Input gate_input;
  Gate_Input trigger_input;
  Output_Interface output;

  void tick() override;
  void process() override;

private:
  ADSR_Core core;
  float overall_time = 10.0;
};


#endif
