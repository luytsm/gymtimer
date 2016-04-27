#include "Arduino.h"

int numeral[10]= { 
B11111100, //0
B01100000, //1
B11011010, //2
B11110010, //3
B01100110, //4
B10110110, //5
B00111110, //6
B11100000, //7
B11111110, //8
B11100110, //9
};

const int segmentPins[8] = {8, 7, A2, 6, 5, 4, A1, A0};
const int digitCount = 4;
const int digitPins[digitCount] = {A3, A4, A5, 9};
const int startStopPin = 2;
const int resetPin = 3;

long startTime, elapsedTime;
int numbers[digitCount] = {0, 1, 2, 3}; 
boolean running;

void showDigit (int number, int digit)
{
    digitalWrite(digitPins[digit], HIGH);
    for (int segment= 0; segment < 8; segment++)
    {
        boolean isBitSet= bitRead(numeral[number], segment);
        digitalWrite(segmentPins[segment], isBitSet);
    }
    digitalWrite(digitPins[digit], LOW);
    delay(3);
    digitalWrite(digitPins[digit], HIGH);
}

void setMiddleDP()
{
    digitalWrite(digitPins[2], HIGH);
    digitalWrite(segmentPins[0], 1);
    digitalWrite(digitPins[2], LOW);
}

void startStop()
{
    Serial.println("Start Or Stop");
    if (running)
    {
        running = false;
    }
    else
    {
        running = true;
        startTime = 0;
    }
}

void reset()
{
    Serial.println("Reset");
    if (!running) 
    {
        elapsedTime = 0; 
    }
}

void updateTime()
{
    elapsedTime = (millis() - startTime) / 1000;
    numbers[3] = elapsedTime % 10;
    numbers[2] = (elapsedTime % 60) / 10;
    numbers[1] = (elapsedTime / 60) % 10;
    numbers[0] = (elapsedTime / 60) / 10;
}

void printTime()
{
    for (int i = 0; i < digitCount; i ++)
        showDigit(numbers[i], i);
}

void run()
{
    if (running)
    {
        updateTime();
    }
    printTime();
    
}

void setup()
{
    Serial.begin(9600);
    for (int i=0; i < 8; i++)
        pinMode(segmentPins[i], OUTPUT); 

    for (int i=0; i < digitCount; i++)
        pinMode(digitPins[i], OUTPUT);

    pinMode(startStopPin, INPUT); 
    pinMode(resetPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(startStopPin), startStop, FALLING);
    attachInterrupt(digitalPinToInterrupt(resetPin), reset, FALLING);
    running = false;
}

void loop()
{
    run();
}
