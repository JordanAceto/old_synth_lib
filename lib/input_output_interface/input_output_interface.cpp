#include "input_output_interface.hpp"

void Input_Interface::plugIn(const Output_Interface *input)
{
  this->input = input;
  dummy_input = false;
}

void Input_Interface::plugIn(float dummy_value)
 {
  input = new Output_Interface(dummy_value);
  dummy_input = true;
}

void Input_Interface::unplug()
{
   if (dummy_input)
     delete input;
   input = nullptr;
}

float Signal_Input::get() const
{
  if (isPluggedIn())
    return input->get() * gain + offset;
  else
    return 0.0;
}

float Gate_Input::get() const
{
  if (isPluggedIn())
    return gate_is_high ? 1.0 : 0.0;
  else
    return 0.0;
}

void Gate_Input::process()
{
  gate_was_high = gate_is_high;

  if (isPluggedIn())
  {
    if (input->get() > threshold + hysteresis)
      gate_is_high = true;
    else if (input->get() < threshold - hysteresis)
      gate_is_high = false;

    rising_edge = !gate_was_high && gate_is_high;

    falling_edge = gate_was_high && !gate_is_high;
  }
}

bool Gate_Input::risingEdge()
{
  return isPluggedIn() ? rising_edge : false;
}

bool Gate_Input::fallingEdge()
{
  return isPluggedIn() ? falling_edge : false;
}

void Signal_Input_Feedthrough::process()
{
  if (isPluggedIn())
    output.set(get());
}

void Arduino_Digital_Input::process()
{
  output.set(digitalRead(pin_number) ? 1.0 : 0.0);
}

void Arduino_Analog_Input::process()
{
  output.set(analogRead(pin_number) / (float)(1 << (num_ADC_bits - 1)) - 1.0);
}

void Arduino_Digital_Output::process()
{
  if (input.isPluggedIn())
  {
    input.process();
    digitalWrite(pin_number, input.get());
  }
}

void Arduino_Analog_Output::process()
{
  if (input.isPluggedIn())
    analogWrite(pin_number, (input.get() + 1.0) * float(1 << (num_DAC_bits - 1)));
}
