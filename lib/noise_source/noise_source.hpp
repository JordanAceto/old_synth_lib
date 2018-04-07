#ifndef NOISE_SOURCE_H_INCLUDED
#define NOISE_SOURCE_H_INCLUDED

#include "inputs_and_outputs.hpp"

class White_Noise_Generator : public Is_Processable
{
public:
  void process() override { output.set((float)rand() / RAND_MAX * 2.0 - 1.0); }
  Output_Interface output;
};

#endif
