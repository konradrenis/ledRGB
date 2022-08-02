#include "Arduino.h"
#include "ledRGB.h"

bool ledRGB::COMMON_ANODE = true;
bool ledRGB::COMMON_CATHODE = false;

ledRGB::ledRGB(int _redPin, int _greenPin, int _bluePin, bool common, int _tickTime) {
    
    redPin = _redPin;
    greenPin = _greenPin;
    bluePin = _bluePin;

    pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);

    commonAnode = common;

    lastTickTime = 0;

    if(_tickTime<8) 
    {
        tickTime = 8;
    }
    else if(_tickTime>300) 
    {
        tickTime = 300;
    }
    else
    {
    tickTime = _tickTime;
    }
}

void ledRGB::setColor(int red, int green, int blue)
{
    actualRedValue = (float)red;
    actualGreenValue = (float)green;
    actualBlueValue = (float)blue;
}

void ledRGB::changeColor(int red, int green, int blue, int time)
{
    targetRedValue = red;
    targetGreenValue = green;
    targetBlueValue = blue;

    if (time < tickTime)
    {
        time = tickTime;
    }

    float steps = (float)time/(float)tickTime;

    redCorrector = (targetRedValue-actualRedValue)/steps;
    greenCorrector = (targetGreenValue-actualGreenValue)/steps;
    blueCorrector = (targetBlueValue-actualBlueValue)/steps;
}

void ledRGB::setBrightness(int _brightness)
{
    brightness = _brightness;
}

void ledRGB::updateColor()
{
    analogWrite(redPin,(int)actualRedValue);
    analogWrite(greenPin,(int)actualGreenValue);
    analogWrite(bluePin,(int)actualBlueValue);

}

void ledRGB::calculateActualColor()
{
    if(targetRedValue == actualRedValue 
    || targetGreenValue == actualGreenValue
    || targetBlueValue == actualBlueValue)
    {
        return;
    }

    actualRedValue += redCorrector;
    actualGreenValue += greenCorrector;
    actualBlueValue += blueCorrector; 
}

//dodac dwa tryby: 1 - ustawianie jednego koloru, 2 - plynna zmiana koloru
void ledRGB::tick()
{
    unsigned long actualTime = millis();

    if((actualTime - lastTickTime > tickTime)||(actualTime < lastTickTime))
    {
        lastTickTime = actualTime;

        calculateActualColor();

        updateColor();
    }


}

