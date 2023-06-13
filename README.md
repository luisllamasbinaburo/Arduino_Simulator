# Arduino Simulation
Arduino Simulator is a library that allows you to **test Arduino code written in C++ from your favorite IDE** such as Visual Studio or Visual Studio Code.

Let's say you have a machine that you want to put into production. This machine has a keypad, a user menu, an emergency stop button, an alarm speaker, sensors, and more. Arduino Simulator allows you **to simulate the behavior of the machine from your IDE**, use debugging features, and create automated tests to ensure that your code functions correctly.

Arduino Simulator enables the creation of **use cases** by defining a Schedule, which contains the actions that will occur at each moment.

For example:

```cpp
TEST_METHOD(MY_COOL_EXAMPLE_TEST)
{
   SIMULATION_STEP_US = 50;
   AT_MILLIS(2000, []() { digitalWrite(0, HIGH); });
   AT_MILLIS(5000, []() { digitalWrite(0, LOW); });
   AT_MILLIS(8000, []() { SIMULATE_INTERRUPT(0, HIGH); });
   AT_MILLIS(12000, []() { SAVE_STATE(); });

   EACH_MILLIS(1000, []() { digitalWrite(1, digitalRead(1) == LOW ? HIGH : LOW); });

   THEN([]() { analogWrite(0, 250); });

   for (size_t pwm = 250; pwm > 50; pwm -= 50)
   {
      THEN_AFTER_MILLIS(500, [pwm]() { ADC_Input[0] = pwm; });
   }

   simulate_seconds(50);

   Assert::IsTrue(Gpio_Status[0] == LOW);
   Assert::IsTrue(Gpio_Status[1] == LOW);
   Assert::IsTrue(Gpio_Status[2] == HIGH);
   Assert::IsTrue(PWM_Output[0] > 200);
   Assert::IsTrue(ADC_Input[0] <= 100);
}
```

You can **use the testing environment of your IDE** to generate various unit tests or complete use cases.

Another advantage of simulating your machine is that you can **use the debugging features of your IDE** to check the state of the machine.

# When to use it
## What it is
Arduino Simulator allows us to create unit tests and usage tests for our Arduino code. For it to be useful, your code must be testable. This means that your code should have abstractions for hardware operations, so that you can use mocks. If you're unsure how to do this, you can visit my blog, where we frequently discuss how to organize your code to make it modular, scalable, and testable.

## What it's not
It's not a complete simulator of Arduino code or hardware (and never will be). It does not provide precise emulation of all Arduino-specific components and functionalities. Of course, third party libraries for Arduino are not supported.

## How to use Arduino Simulator
Using Arduino Simulator is very simple:

- Clone the repository of the solution.
- In the Sketch.cpp file, place the code for your machine.
- Create one or multiple Schedules.
- Run the simulation.

## Simulated machine

Arduino Simulator employs a "virtual machine" consisting of 16 GPIOs, 8 PWM outputs, 8 ADC inputs, and 2 interrupts. However, it can be easily expanded by modifying the code.

You can access the variables of the simulated hardware using the following collections:

```cpp
Gpio_Mode[] and Gpio_Status
ADC_Input[]
PWM_Output[]
Interrupt[]
```

You can also simulate the occurrence of an interrupt with the function `SIMULATE_INTERRUPT(numInterrupt, status)`. It will execute the code you have defined in your Sketch using `attachInterrupt(...)`.

## Schedule
The Schedule is the plan of actions that will occur at each instant. To define it, we have the following functions:

```cpp
AT_MILLIS, AT_MICROS
THEN
THEN_AFTER_MILLIS, THEN_AFTER_MICROS
EACH_MILLIS, EACH_MICROS
```

You can create the Schedule in the main() function of the main.cpp file, although typically, if your IDE allows it, you would create multiple tests, each with their own actions.

##  Running simulation
To execute the simulation, you have the following functions:

```cpp
simulate_seconds()
simulate_milliseconds()
simulate_microseconds()
```
By default, the simulation runs every microsecond. In many cases, this time step is unnecessarily small. For example, if your code performs certain actions every second, simulating it every microsecond is a waste of time.

To address this, you can adjust the simulation step using the variable `SIMULATION_STEP_US`. For example, you can set it to 100 to run the simulation every 100 microseconds.

## Save states
Finally, you can also save states of the machine using the function SAVE_STATE, which creates a snapshot of the machine's state at a specific moment.