# TerabeeI2C
This library eases the use of the Terabee ToF sensors when using them in I2C mode.

## How-To use
---
 
 ### Basic use
 `Tearbee(int mode)` is the constructor in case you are using only one sensor.

 `int mode` defines in which range the sensor works. It takes **SHORT** or **LONG** to define the use of short or long range mode.

 To start using your sensor, use `begin()` in ***setup()***. It will start the sensor in its defined mode.

 Now in ***loop()*** or in any other method, of course, you can use `getDistance(uint16_t & mm)` to request the meassured distance in millimeter. Please be aware to use a variable in which you want to store your data.

 For Example:
 ```C++
 int distance = 0;

 sensor.getDistance(distance);

 Serial.println(distance);
 ````

See the example file to check out a full program.

---
### Advanced use
 
 To use multiple sensors over I2C they each have to have their unique address. Fortunatly we can change the adresses of the Terarangers in software instead of soldering jumpers on the pcb.
 
 Use `Tearbee(int addr, int mode)` where 
 
`mode` receives **LONG** or **SHORT**  and 

 `addr` receives a hex address.
 The standard address of the Terarangers is **0x31**.

 Please keep in mind to check the adresses of other devices you may use on the same I2C bus!

 To change the address of one sensor, use `setAddress(int newAddr)` by setting ```newAddr``` to an other HEX value than **0x31**, the standard address.

Use `resetAddress()` to reset the address to **0x31**.

## Examples

```C++
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
```