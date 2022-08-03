#include "Arduino.h"
#include "ledRGB.h"
#include<cmath>

#define MODE1_OFF 0
#define MODE2_ONE_COLOR 1
#define MODE3_FADE 2

bool ledRGB::COMMON_ANODE = true;
bool ledRGB::COMMON_CATHODE = false;

ledRGB::ledRGB(int _redPin, int _greenPin, int _bluePin, bool common, int _tickTime) {
    
    //Turn of diode at start
    turnOff();

    //Save RGB pins
    redPin = _redPin;
    greenPin = _greenPin;
    bluePin = _bluePin;

    //Pin configuration
    pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);

    //If common anode - True, if common cathode - False
    commonAnode = common;

    //Reset tick time
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

//Tick for diode color update
void ledRGB::tick()
{
    if(!mode)
    {
        unsigned long actualTime = millis();
        if((actualTime - lastTickTime > tickTime)||(actualTime < lastTickTime))
        {
            lastTickTime = actualTime;

            switch (mode)
            {
                case MODE2_ONE_COLOR:
                    break;
                case MODE3_FADE:
                    calculateFadeColor();
                    updateColor();
                    break;
                case MODE1_OFF:
                    break;
            }
            
        }

    }

}

//Turn of diode - mode 1
void ledRGB::turnOff()
{
    mode = MODE1_OFF;
    setColor(0,0,0);
}

//Set color of diode - mode 2
void ledRGB::color(int red, int green, int blue)
{
    mode = MODE2_ONE_COLOR;
    setColor(red, green, blue);
    updateColor();

}

//Smooth transition from the current color to the given - mode 3
void ledRGB::fade(int red, int green, int blue, unsigned int time)
{
    mode = MODE3_FADE;

    fadeDone = false;

    setTarget(red,green,blue);

    if (time < tickTime)
    {
        time = tickTime;
    }
    //Calculates how much to increment every color in each tick
    float steps = (float)time/(float)tickTime;
    redCorrector = (targetRedValue-actualRedValue)/steps;
    greenCorrector = (targetGreenValue-actualGreenValue)/steps;
    blueCorrector = (targetBlueValue-actualBlueValue)/steps;
}

int ledRGB::getMode()
{
    return mode;
}

bool ledRGB::getFadeDone()
{
    return fadeDone;
}

void ledRGB::setColor(int red, int green, int blue)
{
    if (red > 255) red = 255;
    if (red < 0) red = 0;

    if (green > 255) green = 255;
    if (green < 0) green = 0;

    if (blue > 255) blue = 255;
    if (blue < 0) blue = 0;


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

void ledRGB::setTarget(int red, int green, int blue)
{
    if (red > 255) red = 255;
    if (red < 0) red = 0;

    if (green > 255) green = 255;
    if (green < 0) green = 0;

    if (blue > 255) blue = 255;
    if (blue < 0) blue = 0;

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


void ledRGB::updateColor()
{
    analogWrite(redPin,(int)actualRedValue);
    analogWrite(greenPin,(int)actualGreenValue);
    analogWrite(bluePin,(int)actualBlueValue);

}

void ledRGB::calculateFadeColor()
{
    if(targetRedValue == actualRedValue 
    && targetGreenValue == actualGreenValue
    && targetBlueValue == actualBlueValue)
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


