#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include "input_output_interface.hpp"
#include "base_classes.hpp"
#include "helper_functions.hpp"

class Inverter : public Is_Processable
{
public:
  Signal_Input input;
  Output_Interface output;

  void process() override;
};

class Half_Wave_Rectifier : public Is_Processable
{
public:
  Signal_Input input;
  Output_Interface output;

  void process() override;
};

class Full_Wave_Rectifier : public Is_Processable
{
public:
  Signal_Input input;
  Output_Interface output;

  void process() override;
};

class Bipolar_To_Unipolar_Converter : public Is_Processable
{
public:
  Signal_Input input;
  Output_Interface output;

  void process() override;
};

class Unipolar_To_Bipolar_Converter : public Is_Processable
{
public:
  Signal_Input input;
  Output_Interface output;

  void process() override;
};

class Mixer : public Is_Processable
{
private:
  static const int num_inputs = 5;

public:
  enum Inputs
  {
    INPUT_1 = 0,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5
  };

  Signal_Input input[num_inputs];
  Output_Interface output;

  void process() override;
};

class Sample_And_Hold : public Is_Processable
{
public:
  Signal_Input signal_input;
  Gate_Input trigger_input;
  Output_Interface output;

  void process() override;
};

#endif
