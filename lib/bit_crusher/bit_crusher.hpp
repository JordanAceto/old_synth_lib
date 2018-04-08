#ifndef BIT_CRUSHER_H_INCLUDED
#define BIT_CRUSHER_H_INCLUDED

#include "inputs_and_outputs.hpp"
#include "helper_functions.hpp"
#include "base_classes.hpp"
#include <stdint.h>

#include <Arduino.h>

class Bit_Crusher_Core : public Is_Processable
{
public:

  void loadSample(int32_t input) { this->input = clamp(input, -32768, 32767); }
  void loadCrushSignal(int32_t control) { this->control = control; }
  int32_t getOutput() const { return output; }


void process() override
{
  output = 0;
  bool is_negative = input < 0;

  for (int bit = 0; bit < num_bits; bit ++)
  {
    int32_t place_value = 1 << bit;

    int32_t local_input = input;

    if (is_negative)
      local_input = -local_input;

    int32_t bit_contrib = place_value & local_input;

    /** take the top 4 bits of the 16 bit control signal as the cutoff control, this will be between 0 - 16 */
    int32_t cutoff = control >> 12;

    /** take the bottom 12 bits as the fractional part of the control signal */
    int32_t fraction = control & 0x0FFF;

    /** and invert the fractional signal, so that bits are attenuated in the right way */
    fraction = 0x0FFF - fraction;

    /** bits below the cutoff are thrown away */
    if (bit < cutoff)
      bit_contrib = 0;
    /** the bit right on the cutoff number is attenuated by the inverted fractional control signal */
    else if (bit == cutoff)
      bit_contrib = (fraction * bit_contrib) >> 12;
    /** bits above the cutoff are let through unscathed */
    else if (bit > cutoff) {}
      // do nothing

    /** add up the contribution from each bit */
    output += bit_contrib;
  }

  if (is_negative)
    output = -output;
}

private:
  int32_t input, control, output;
  const int num_bits = 16;
};

class Bit_Crusher : public Is_Processable
{
public:
  Signal_Input input;
  Signal_Input crush_input;
  Output_Interface output;

  void process() override
  {
    core.loadSample(input.get() * 0x8000);
    core.loadCrushSignal(mapInput(crush_input.get(), 0, 0xE800));
    core.process();
    output.set(core.getOutput() / (float)0x8000);
  }

private:
  Bit_Crusher_Core core;
};


#endif
