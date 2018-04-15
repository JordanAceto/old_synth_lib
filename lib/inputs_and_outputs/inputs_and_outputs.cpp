#include "inputs_and_outputs.hpp"

Input_Interface::Input_Interface()
{
  dummy_input = new Output_Interface(0.0);
  this->input = dummy_input;
}

Input_Interface::Input_Interface(float initial_value)
{
  dummy_input = new Output_Interface(initial_value);
  this->input = dummy_input;
}

void Input_Interface::plugIn(const Output_Interface *input)
{
  this->input = input;
}

void Input_Interface::plugIn(float dummy_value)
 {
   dummy_input->set(dummy_value);
   this->input = dummy_input;
}

void Input_Interface::unplug()
{
  dummy_input->set(input->get());
  this->input = dummy_input;
}

float Signal_Input::get() const
{
  return input->get() * gain + offset;
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
  output.set(get());
}

void Signal_Input_Feedthrough_With_Delayed_Plug_In::plugIn(const Output_Interface *input)
{
  this->input_to_plug_in = input;
  input_caught_up_with_dummy_value = false;
  input_started_too_high = false;
  input_started_too_low = false;

  if (input_to_plug_in->get() > dummy_input->get())
  {
    input_started_too_high = true;
  }
  else if (input_to_plug_in->get() < dummy_input->get())
  {
    input_started_too_low = true;
  }
  else
  {
      this->input = input_to_plug_in;
      input_caught_up_with_dummy_value = true;
      input_started_too_high = false;
      input_started_too_low = false;
  }
}

void Signal_Input_Feedthrough_With_Delayed_Plug_In::process()
{
  if (!input_caught_up_with_dummy_value)
  {
    if (input_started_too_high && (input_to_plug_in->get() <= dummy_input->get()))
    {
        this->input = input_to_plug_in;
        input_caught_up_with_dummy_value = true;
        input_started_too_high = false;
        input_started_too_low = false;
    }

    if (input_started_too_low && (input_to_plug_in->get() >= dummy_input->get()))
    {
        this->input = input_to_plug_in;
        input_caught_up_with_dummy_value = true;
        input_started_too_high = false;
        input_started_too_low = false;
    }
  }

  Signal_Input_Feedthrough::process();
}

void Arduino_Digital_Input::process()
{
  output.set(digitalRead(pin_number) ? 1.0 : 0.0);
}

void Arduino_Analog_Input::process()
{
  output.set(analogRead(pin_number) / (float)(1 << (g_num_ADC_bits - 1)) - 1.0);
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
  analogWrite(pin_number, (input.get() + 1.0) * float(1 << (g_num_DAC_bits - 1)));
}
