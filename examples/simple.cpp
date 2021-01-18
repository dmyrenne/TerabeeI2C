#include <TerabeeI2C.h>

Terabee sensor(SHORT);
int distance = 0;

setup(){
    Serial.begin(115200);
    sensor.begin();
}

loop(){
    sensor.getDistance(distance);
    
    Serial.print("Distance is: ");
    Serial.print(distance);
    Serial.println(" mm");
}