#include "scanner.hpp"

void Five_Input_Scanner::process()
{
  const float one_eighth = 1.0 / 8.0,
              three_eighths = 3.0 / 8.0,
              five_eighths = 5.0 / 8.0,
              seven_eighths = 7.0 / 8.0,
              slope = 1.0 / three_eighths;

  trapezoid_1 = clamp(control_input.get() * -2.0 - 1.0, 0.0, 0.8);

  if (control_input.get() < -seven_eighths || control_input.get() >= 0.0) // low dead zone
      trapezoid_2 = 0.0;
  else if (control_input.get() > -0.5 && control_input.get() < -three_eighths) // high dead zone
    trapezoid_2 = 1.0;
  else if (control_input.get() >= -three_eighths) // down ramp
    trapezoid_2 = control_input.get() * -slope;
  else if (control_input.get() >= -seven_eighths) // up ramp
    trapezoid_2 = (control_input.get() + seven_eighths) * slope;

  if (control_input.get() < -three_eighths || control_input.get() >= 0.5) // low dead zone
      trapezoid_3 = 0.0;
  else if (control_input.get() > 0.0 && control_input.get() < one_eighth) // high dead zone
    trapezoid_3 = 1.0;
  else if (control_input.get() >= one_eighth) // down ramp
    trapezoid_3 = control_input.get() * -slope + 1.33;
  else if (control_input.get() >= -three_eighths) // up ramp
    trapezoid_3 = (control_input.get() + three_eighths) * slope;

    if (control_input.get() < -one_eighth) // low dead zone
        trapezoid_4 = 0.0;
    else if (control_input.get() > 0.5 && control_input.get() < five_eighths) // high dead zone
      trapezoid_4 = 1.0;
    else if (control_input.get() >= five_eighths) // down ramp
      trapezoid_4 = control_input.get() * -slope + 2.675;
    else if (control_input.get() >= one_eighth) // up ramp
      trapezoid_4 = (control_input.get() - one_eighth) * slope;

    trapezoid_5 = clamp((control_input.get() - 0.5) * slope, 0.0, 1.0);


  signal_input[INPUT_1].setGain(trapezoid_1);
  signal_input[INPUT_2].setGain(trapezoid_2);
  signal_input[INPUT_3].setGain(trapezoid_3);
  signal_input[INPUT_4].setGain(trapezoid_4);
  signal_input[INPUT_5].setGain(trapezoid_5);

  output.set(signal_input[INPUT_1].get() + signal_input[INPUT_2].get() + signal_input[INPUT_3].get() + signal_input[INPUT_4].get() + signal_input[INPUT_5].get());
/*
  Serial.print("Input: ");
  Serial.print(control_input.get());
  Serial.print("    Output: ");
  Serial.println(trapezoid_5);
*/
}
