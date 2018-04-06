#ifndef INPUT_OUTPUT_INTERFACE_H_INCLUDED
#define INPUT_OUTPUT_INTERFACE_H_INCLUDED

#include "helper_functions.hpp"
#include "base_classes.hpp"
#include "global_constants.hpp"

class Output_Interface
{
public:
  Output_Interface() {}
  Output_Interface(float val) : value(val) {}
  virtual ~Output_Interface() {}

  virtual float get() const { return value; };
  virtual void set(float value) { this->value = value; }
protected:
  float value;
};

class Input_Interface : public Is_Processable
{
public:
  virtual void plugIn(const Output_Interface *input);
  virtual void plugIn(float dummy_value);
  virtual void unplug();
  bool isPluggedIn() const { return input != nullptr; }

  virtual void process() override {}
  virtual float get() const = 0;

protected:
  bool dummy_input;
  const Output_Interface *input = nullptr;
};

class Signal_Input : public Input_Interface
{
public:
  virtual float get() const override;
  virtual void setGain(float gain) { this->gain = clamp(gain); }
  virtual void setOffset(float offset) { this->offset = clamp(offset); }

private:
  float gain = 1.0, offset = 0.0;
};

class Gate_Input : public Input_Interface
{
public:
  float get() const override;

  void process() override;
  bool risingEdge();
  bool fallingEdge();

private:
  const float threshold = 0.5, hysteresis = 0.3;
  bool gate_is_high, gate_was_high, rising_edge, falling_edge;
};

class Signal_Input_Feedthrough : public Signal_Input
{
public:
  Output_Interface output;
  void process();
};

class Arduino_Pin : public Is_Processable
{
public:
  Arduino_Pin(int pin_num) : pin_number(pin_num) {}
protected:
  const int pin_number;
};

class Arduino_Input : public Arduino_Pin
{
public:
  Arduino_Input(int pin_number) : Arduino_Pin(pin_number) { pinMode(pin_number, INPUT); }
  Output_Interface output;
};

class Arduino_Output : public Arduino_Pin, public Output_Interface
{
public:
  Arduino_Output(int pin_number) : Arduino_Pin(pin_number) {pinMode(pin_number, OUTPUT); }

};

class Arduino_Digital_Input : public Arduino_Input
{
public:
  Arduino_Digital_Input(int pin_number) : Arduino_Input(pin_number) {}

  void process() override;
};

class Arduino_Analog_Input : public Arduino_Input
{
public:
  Arduino_Analog_Input(const int pin_number) : Arduino_Input(pin_number) {}
  void process() override;
};

class Arduino_Digital_Output : public Arduino_Output
{
public:
  Arduino_Digital_Output(int pin_number) : Arduino_Output(pin_number) {}
  Gate_Input input;
  void process() override;
};

class Arduino_Analog_Output : public Arduino_Output
{
public:
  Arduino_Analog_Output(int pin_number) : Arduino_Output(pin_number)
  {
    analogWriteFrequency(pin_number, pwm_dac_frequency);
  }

  Signal_Input input;
  void process() override;
};

#endif
