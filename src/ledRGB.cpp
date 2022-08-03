#include "Arduino.h"
#include "ledRGB.h"
#include<cmath>

#define MODE1_OFF 0
#define MODE2_ONE_COLOR 1
#define MODE3_FADE 2

bool ledRGB::COMMON_ANODE = true;
bool ledRGB::COMMON_CATHODE = false;

ledRGB::ledRGB(int _redPin, int _greenPin, int _bluePin, bool common, int _tickTime) {
    
    mode = MODE1_OFF;

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
    if(commonAnode)
    {
        actualRedValue = 255 - (float)red;
        actualGreenValue = 255 - (float)green;
        actualBlueValue = 255 - (float)blue;

    }
    else
    {
        actualRedValue = (float)red;
        actualGreenValue = (float)green;
        actualBlueValue = (float)blue;

    }
}

void ledRGB::color(int red, int green, int blue)
{
    mode = MODE2_ONE_COLOR;

    setColor(red, green, blue);

}

void ledRGB::setTarget(int red, int green, int blue)
{
    if(commonAnode)
    {
        targetRedValue = 255 - red;
        targetGreenValue = 255 - green;
        targetBlueValue = 255 - blue;
    }
    else
    {
        targetRedValue = red;
        targetGreenValue = green;
        targetBlueValue = blue;
    }
}

void ledRGB::fade(int red, int green, int blue, int time)
{
    mode = MODE3_FADE;

    fadeDone = false;

    setTarget(red,green,blue);

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

void ledRGB::turnOff()
{
    mode = MODE1_OFF;
    setColor(0,0,0);
}

void ledRGB::updateColor()
{
    analogWrite(redPin,(int)actualRedValue);
    analogWrite(greenPin,(int)actualGreenValue);
    analogWrite(bluePin,(int)actualBlueValue);

}

void ledRGB::calculateFadeColor()
{
    if(targetRedValue == actualRedValue 
    || targetGreenValue == actualGreenValue
    || targetBlueValue == actualBlueValue)
    {
        fadeDone = true;
        return;
    }

    actualRedValue += redCorrector;
    actualGreenValue += greenCorrector;
    actualBlueValue += blueCorrector; 

    if(abs(actualRedValue - targetRedValue)<1) actualRedValue = targetRedValue;
    if(abs(actualGreenValue - targetGreenValue)<1) actualGreenValue = targetGreenValue;
    if(abs(actualBlueValue - targetBlueValue)<1) actualBlueValue = targetBlueValue;

}

void ledRGB::tick()
{
    unsigned long actualTime = millis();

    if((actualTime - lastTickTime > tickTime)||(actualTime < lastTickTime))
    {
        lastTickTime = actualTime;

        switch (mode)
        {
            case MODE1_OFF:
                updateColor();
                break;
            case MODE2_ONE_COLOR:
                updateColor();
                break;
            case MODE3_FADE:

                calculateFadeColor();
                updateColor();
                break;
        }
        
    }

}

