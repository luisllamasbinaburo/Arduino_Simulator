#include "sketch.h"

void setup()
{
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    attachInterrupt(0, []() {digitalWrite(1, HIGH); }, CHANGE);

    println("Started");
}

int counter = 0;
void loop()
{   
    counter++;    

    print(counter);
    print(": ");
    println(millis());

    delay(1000); 
}