#pragma once

#ifndef _SIMULATION_
#define _SIMULATION_

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <functional>

#define LOW  0x0
#define HIGH 0x1

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

extern long long arduino_simulation_time_counter;

struct Interrupt
{
	int prev_value;
	int mode;
	void (*isr)(void) = nullptr;
};

struct State
{
	uint8_t Gpio_Mode[16];
	uint8_t Gpio_Status[16];
	uint8_t ADC_Input[8];
	uint8_t PWM_Output[8];
	Interrupt Interrups[2];
};


extern uint8_t Gpio_Mode[16];
extern uint8_t Gpio_Status[16];
extern uint8_t ADC_Input[8];
extern uint8_t PWM_Output[8];
extern Interrupt Interrups[2];


extern void pinMode(uint8_t pin, uint8_t mode);
extern void digitalWrite(uint8_t pin, uint8_t val);
extern int digitalRead(uint8_t pin);
extern void analogWrite(uint8_t pin, int val);
extern int analogRead(uint8_t pin);
extern void analogReference(uint8_t mode);

extern unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
extern unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);
extern void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
extern uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

extern void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode);
extern void detachInterrupt(uint8_t interruptNum);

extern unsigned long millis(void);

extern unsigned long micros(void);
extern void delay(unsigned long ms);

extern void delayMicroseconds(unsigned int us);


extern void print(char* t);
extern void print(const char t[]);
extern void print(char t);
extern void print(unsigned char t, int = DEC);
extern void print(int t, int = DEC);
extern void print(unsigned int t, int = DEC);
extern void print(long t, int = DEC);
extern void print(unsigned long t, int = DEC);
extern void print(long long t, int = DEC);
extern void print(unsigned long long t, int = DEC);
extern void print(double t, int = 2);

extern void println(char* t);
extern void println(const char t[]);
extern void println(char t);
extern void println(unsigned char t, int = DEC);
extern void println(int t, int = DEC);
extern void println(unsigned int t, int = DEC);
extern void println(long t, int = DEC);
extern void println(unsigned long t, int = DEC);
extern void println(long long t, int = DEC);
extern void println(unsigned long long t, int = DEC);
extern void println(double t, int = 2);
#endif
