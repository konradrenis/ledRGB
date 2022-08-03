#ifndef ledRGB_h
#define ledRGB_h

#include "Arduino.h"

class ledRGB
{

//Functions
public:
    ledRGB(int redPin, int greenPin, int bluePin, bool common, int _tickTime);

    void tick();
    void color(int red, int green, int blue);
    void fade(int red, int green, int blue,unsigned int time);
    void turnOff();
    int getMode();
    bool getFadeDone();

private:
    void calculateFadeColor();
    void updateColor();
    void setTarget(int red, int green, int blue);
    void setColor(int red, int green, int blue);

//Variables
public:
	static bool COMMON_ANODE;
	static bool COMMON_CATHODE;
    
private:
    int mode;

    bool commonAnode;

    bool fadeDone = false;

    int redPin;
    int greenPin;
    int bluePin;

    float actualRedValue;
    float actualGreenValue;
    float actualBlueValue;

    float targetRedValue;
    float targetGreenValue;
    float targetBlueValue;

    float redCorrector;
    float greenCorrector;
    float blueCorrector;

    unsigned long lastTickTime;
    unsigned int tickTime;
	
};

#endif