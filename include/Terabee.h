#ifndef Terabee_h
#define Terabee_h

#include <Arduino.h>
#include <crc.h>
#include <Wire.h>

#define GET_DIST        0x00
#define SET_ADDR        0xA2
#define LONG            0x03
#define SHORT           0x01
#define REQUEST         0x02
#define DEFAULT_ADDR    0x31

class Terabee{
    
    public:
        Terabee(int mode);
        Terabee(int addr, int mode);
        void begin();
        void getDistance(uint16_t & mm);
        void setAddress(int newAddr);
        void resetAddress();
        void end();

    protected:
        // Initialize variables
        boolean begun = false;
        int inter = 25; // Inter reading delay, replace by 50 if using long mode
        int _addr, _cmd, _mode;
        uint8_t crc8(uint8_t *p, uint8_t len);

};



#endif