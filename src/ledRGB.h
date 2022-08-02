#ifndef ledRGB_h
#define ledRGB_h

#include "Arduino.h"

class ledRGB
{

public:
    ledRGB(int redPin, int greenPin, int bluePin, bool common, int _tickTime);

    void tick();

    void setColor(int red, int green, int blue);
    void setBrightness(int _brightness);
    void changeColor(int red, int green, int blue, int time);
    

	static bool COMMON_ANODE;
	static bool COMMON_CATHODE;

private:

    void calculateActualColor();
    void updateColor();

    int redPin;
    int greenPin;
    int bluePin;

    bool commonAnode;

    float actualRedValue;
    float actualGreenValue;
    float actualBlueValue;
    int brightness;

    float redCorrector;
    float greenCorrector;
    float blueCorrector;

    float targetRedValue;
    float targetGreenValue;
    float targetBlueValue;

    unsigned long lastTickTime;
    int tickTime;
	
};

#endif