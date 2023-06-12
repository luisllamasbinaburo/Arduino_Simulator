#include <iostream>

#include "arduino_simulator.h"
#include "sketch.h"

struct ScheduleTask
{
	int micros;

	int interval;
	std::function<void()> action;
};

struct SaveState
{
	int micros;

	State state;
};


static std::vector<ScheduleTask> schedule;
static std::vector<SaveState> SaveStates;

using namespace std;

extern long long arduino_simulation_time_counter;
extern long long next_loop_time_counter;

long long SIMULATION_STEP_US = 1;


long last_added;
void EACH_MICROSECONDS(long interval, std::function<void()> action)
{
	auto task = ScheduleTask{ 0, interval, action };
	schedule.push_back(task);
}

void EACH_MILLIS(long interval, std::function<void()> action)
{
	auto task = ScheduleTask{ 0, interval * 1000, action };
	schedule.push_back(task);
}

void EACH_MICROSECONDS(long start, long interval, std::function<void()> action)
{
	auto task = ScheduleTask{ start, interval, action };
	schedule.push_back(task);
}

void EACH_MILLIS(long start, long interval, std::function<void()> action)
{
	auto task = ScheduleTask{ start * 1000, interval * 1000, action };
	schedule.push_back(task);
}

void THEN(std::function<void()> action)
{
	auto task = ScheduleTask{ last_added, 0, action };
	schedule.push_back(task);
}

void THEN_AFTER_MICROSECONDS(long micros, std::function<void()> action)
{
	last_added = last_added + micros;
	THEN(action);
}

void THEN_AFTER_MILLIS(long millis, std::function<void()> action)
{
	last_added = last_added + millis * 1000;
	THEN(action);
}

void AT_MICROSECONDS(long micros, std::function<void()> action)
{
	last_added = micros;
	auto task = ScheduleTask{ micros, 0, action };
	schedule.push_back(task);
}

void AT_MILLIS(long millis, std::function<void()> action)
{
	auto task = ScheduleTask{ millis * 1000, 0, action };
	schedule.push_back(task);
}

void init()
{
	arduino_simulation_time_counter = 0;
	next_loop_time_counter = 0;

	for (size_t i = 0; i < 16; i++)
	{
		Gpio_Mode[i] = OUTPUT;
		Gpio_Status[i] = 0;
	}

	for (size_t i = 0; i < 8; i++)
	{
		ADC_Input[i] = 0;
		PWM_Output[i] = 0;
	}
}

static void SAVE_STATE()
{
	State state;
	SaveState savedState;

	for (size_t i = 0; i < 16; i++)
	{
		state.Gpio_Mode[i] = Gpio_Mode[i];
		state.Gpio_Status[i] = Gpio_Status[i];
	}
	for (size_t i = 0; i < 8; i++)
	{
		state.ADC_Input[i] = ADC_Input[i];
		state.PWM_Output[i] = PWM_Output[i];
	}

	savedState.micros = arduino_simulation_time_counter;
	savedState.state = state;


	SaveStates.push_back(savedState);
}

static void SIMULATE_INTERRUPT(int interruptNum, int value)
{
	auto interrupt = Interrups[interruptNum];

	if(interrupt.isr != nullptr)
	{
		if(interrupt.mode == CHANGE && interrupt.prev_value != value) interrupt.isr();
		if(interrupt.mode == RISING && interrupt.prev_value == LOW && value == HIGH) interrupt.isr();
		if(interrupt.mode == FALLING && interrupt.prev_value == HIGH && value == LOW) interrupt.isr();

		interrupt.prev_value = value;
	}	
}

static int simulate_microseconds(long microseconds)
{
	init();

	cout << "********** simulation start **********\n\n";
	setup();

	while (arduino_simulation_time_counter < microseconds)
	{
		auto counter = schedule.size();
		for (auto i = 0; i < counter; i++)
		{
			if (schedule[i].micros <= arduino_simulation_time_counter)
			{
				schedule[i].action();
				if (schedule[i].interval > 0)
				{
					schedule[i].micros += schedule[i].interval;
				}
				else
				{
					schedule.erase(schedule.begin() + i);
					counter--;
				}
			}
		}

		if (arduino_simulation_time_counter >= next_loop_time_counter) loop();
		arduino_simulation_time_counter += SIMULATION_STEP_US;
	}
		
	cout << "\n********** simulation end **********";
	return 0;
}

static int simulate_milliseconds(long milliseconds)
{
	return simulate_microseconds(milliseconds * 1000);
}

static int simulate_seconds(long seconds)
{
	return simulate_microseconds(seconds * 1000000);
}

int main()
{
	SIMULATION_STEP_US = 100;

	EACH_MILLIS(1000, []() { 
		digitalWrite(0, digitalRead(0) == LOW ? HIGH : LOW); 
		auto message = digitalRead(0) == LOW ? "Settin LOW\n" : "Seetting HIGH\n";
		cout << message;
	});

	simulate_seconds(5);
}
