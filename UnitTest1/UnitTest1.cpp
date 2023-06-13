#include "pch.h"
#include "CppUnitTest.h"

#include "../Arduino_Simulation/main.cpp"
#include "../Arduino_Simulation/arduino_simulator.cpp"
#include "../Arduino_Simulation/sketch.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:		
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


		TEST_METHOD(LOG_TEST)
		{
			SIMULATION_STEP_US = 100;
			simulate_seconds(5);

			Logger::WriteMessage(digitalRead(0) == LOW ? "LOW" : "HIGH");
		}

		TEST_METHOD(AT_MILLIS_TEST)
		{
			SIMULATION_STEP_US = 100;
			AT_MILLIS(2000, []() { digitalWrite(0, HIGH); });

			simulate_seconds(5);

			Assert::IsTrue(Gpio_Status[0] == HIGH);
		}

		TEST_METHOD(EACH_MILLIS_TEST)
		{
			SIMULATION_STEP_US = 100;
			EACH_MILLIS(1000, []() { digitalWrite(0, digitalRead(0) == LOW ? HIGH : LOW); });

			simulate_seconds(5);

			Assert::IsTrue(Gpio_Status[0] == HIGH);
		}

		TEST_METHOD(INTERRUPT_TEST)
		{
			SIMULATION_STEP_US = 100;	

			AT_MILLIS(2000, []() {SIMULATE_INTERRUPT(0, HIGH); });
			simulate_seconds(5);

			Assert::IsTrue(Gpio_Status[2] == HIGH);
		}

		TEST_METHOD(SAVE_STATE_TEST)
		{
			SIMULATION_STEP_US = 100;	
		
			AT_MILLIS(1000, []() { digitalWrite(0, HIGH); });
			AT_MILLIS(2000, []() { SAVE_STATE(); });
			AT_MILLIS(3000, []() { digitalWrite(0, LOW); });
			
			simulate_seconds(5);

			Assert::IsTrue(SaveStates[0].state.Gpio_Status[0] == HIGH);
			Assert::IsTrue(Gpio_Status[0] == LOW);
		}
	};
}
