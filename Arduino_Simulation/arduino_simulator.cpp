#include "arduino_simulator.h"

long long arduino_simulation_time_counter = 0;
long long next_loop_time_counter = 0;

uint8_t Gpio_Mode[16];
uint8_t Gpio_Status[16];
uint8_t ADC_Input[8];
uint8_t PWM_Output[8];

Interrupt Interrups[2];


void pinMode(uint8_t pin, uint8_t mode)
{
    Gpio_Mode[pin] = OUTPUT;
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    Gpio_Status[pin] = val;
}

int digitalRead(uint8_t pin)
{
    return Gpio_Status[pin];
}

void analogWrite(uint8_t pin, int val)
{
    PWM_Output[pin] = val;
}

int analogRead(uint8_t pin)
{
    return ADC_Input[pin];
}

void analogReference(uint8_t mode)
{
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    return 0;
}

unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout)
{
    return 0;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
    return 0;
}

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
    Interrups[interruptNum].isr = userFunc;
    Interrups[interruptNum].mode = mode;
}

void detachInterrupt(uint8_t interruptNum)
{
    Interrups[interruptNum].isr = nullptr;    
}

unsigned long millis(void)
{
    return arduino_simulation_time_counter / 1000;
}

unsigned long micros(void)
{
    return arduino_simulation_time_counter;
}

void delay(unsigned long ms)
{
    next_loop_time_counter = arduino_simulation_time_counter + ms * 1000;
}

void delayMicroseconds(unsigned int us)
{
    next_loop_time_counter = arduino_simulation_time_counter + us * 1000;
}


void print(char* t) {std::cout << t ;}
void print(const char t[]) {std::cout << t ;}
void print(char t) {std::cout << t ;}
void print(unsigned char t, int) {std::cout << t ;}
void print(int t, int) {std::cout << t ;}
void print(unsigned int t, int) {std::cout << t ;}
void print(long t, int) {std::cout << t ;}
void print(unsigned long t, int) {std::cout << t ;}
void print(long long t, int) {std::cout << t ;}
void print(unsigned long long t, int) {std::cout << t ;}
void print(double t, int) {std::cout << t ;}

void println(char* t) {std::cout << t ;}
void println(const char t[]) {std::cout << t << "\n";}
void println(char t) {std::cout << t << "\n";}
void println(unsigned char t, int) {std::cout << t << "\n";}
void println(int t, int) {std::cout << t << "\n";}
void println(unsigned int t, int) {std::cout << t << "\n";}
void println(long t, int ) {std::cout << t << "\n";}
void println(unsigned long t, int) {std::cout << t << "\n";}
void println(long long t, int) {std::cout << t << "\n";}
void println(unsigned long long t, int) {std::cout << t << "\n";}
void println(double t, int ) {std::cout << t << "\n";}