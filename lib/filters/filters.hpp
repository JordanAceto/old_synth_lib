#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "input_output_interface.hpp"
#include "base_classes.hpp"

class One_Pole_Lowpass_Filter : public Is_Tickable, public Is_Processable
{
public:
  Signal_Input input;
  Signal_Input cutoff_input;
  Output_Interface output;

  void tick() override;
  void process() override;

private:
  float sample, cutoff;
};

class Four_Pole_Lowpass_Filter : public Is_Tickable, public Is_Processable
{
public:
  Four_Pole_Lowpass_Filter()
  {
    stage[0].input.plugIn(&input.output);

    for (int i = 1; i < 4; i ++)
      stage[i].input.plugIn(&stage[i - 1].output);

    for (auto &s : stage)
      s.cutoff_input.plugIn(&cutoff_input.output);
  }

  void tick() override;
  void process() override;

  Signal_Input_Feedthrough input;
  Signal_Input_Feedthrough cutoff_input;
  Output_Interface output;

private:
  One_Pole_Lowpass_Filter stage[4];
};

#endif
