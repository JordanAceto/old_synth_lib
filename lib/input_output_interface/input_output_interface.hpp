#ifndef INPUT_OUTPUT_INTERFACE_H_INCLUDED
#define INPUT_OUTPUT_INTERFACE_H_INCLUDED

#include "helper_functions.hpp"
#include "base_classes.hpp"
#include "global_constants.hpp"

#include <Encoder.h>

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
  Input_Interface();
  Input_Interface(float initial_value);

  virtual void plugIn(const Output_Interface *input);
  virtual void plugIn(float dummy_value);
  virtual void unplug();

  virtual void process() override {}
  virtual float get() const = 0;

protected:
  const Output_Interface *input;
  Output_Interface *dummy_input;
};

class Signal_Input : public Input_Interface
{
public:
  Signal_Input() {}
  Signal_Input(float initial_value) : Input_Interface(initial_value) {}
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
  bool isPluggedIn() const { return input != nullptr; }

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

class Rotary_Encoder : public Is_Processable
{
public:
  Rotary_Encoder(int pin1, int pin2) : encoder(pin1, pin2) {}

  void process() override
  {
    if (encoder.read() > max_counts)
      encoder.write(max_counts);
    if (encoder.read() < -max_counts)
      encoder.write(-max_counts);

    output.set(encoder.read() / max_counts);
  }

  void set(float new_value)
  {
    encoder.write(new_value * max_counts);
  }

  Output_Interface output;
private:
  const float max_counts = 500.0;
  Encoder encoder;

};

#endif
