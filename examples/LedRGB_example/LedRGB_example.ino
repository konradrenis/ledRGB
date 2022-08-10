#include <ledRGB.h>


ledRGB led(2,3,4,ledRGB::COMMON_ANODE,8);
unsigned long lastTime = 0;
int stage = 1;

void setup() { 

}

void loop() {
  
  led.tick();

  unsigned long actualTime = millis();
  if (actualTime - lastTime > 5000)
  {
    lastTime = actualTime;
    switch (stage)
    {
      case 1:
        stage = 2;
        led.fade(255,0,0,4000);
        break;

      case 2:
        stage = 3;
        led.fade(255,255,0,4000);
        break;
      
      case 3:
        stage = 4;
        led.fade(0,255,0,4000);
        break;

      case 4:
        stage = 5;
        led.color(0,0,100);
        break;
      
      case 5:
        stage = 6;
        led.color(255,100,100);
        break;

      case 6:
        stage = 1;
        led.turnOff();
        break;
      
        
    }
    
  }
  

}
