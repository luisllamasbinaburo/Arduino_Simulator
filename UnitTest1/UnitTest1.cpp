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

			AT_MILLIS(2000, []() {SIMULATE_INTERRUPT(1, HIGH); });
			simulate_seconds(5);

			Assert::IsTrue(Gpio_Status[1] == HIGH);
		}
	};
}
