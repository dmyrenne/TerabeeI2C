#ifndef Terabee_H
#define Terabee_H

#include <Arduino.h>
#include <crc.h>
#include <Chrono.h>

class Terabee{
    public:
        Terabee(uint8_t s, uint32_t b = 115200, uint8_t p = 1, boolean r = true);
        Terabee();
        // ~Terabee();
        bool begin(uint8_t s, uint32_t b = 115200); 
        uint8_t setPixelMode(uint8_t p);
        bool setLongRange(boolean r);
        bool update();
        void getDistance(uint16_t & w);     //1PX Mode
        void getDistance(uint16_t & w, uint16_t & x);     //2PX Mode
        void getDistance(uint16_t & w, uint16_t & x, uint16_t & y, uint16_t & z);      //4PX Mode
        void button(uint16_t min, uint16_t max);
        bool wasPressed();
        uint8_t Framereceived[10];// The variable "Framereceived[]" will contain the frame sent by the TeraRanger
        

    protected:
        const byte pixelmode[3][4]  =   {
            {0x00,0x21,0x01,0xBC},
            {0x00,0x21,0x03,0xB2},
            {0x00,0x21,0x02,0xB5}
        };

        const byte rangemode[2][4]    =   {
            {0x00,0x61,0x01,0xE7},
            {0x00,0x61,0x03,0xE9}
        };
        const byte PRINTOUT_BINARY[4]             = {0x00,0x11,0x02,0x4C};
        const byte PRINTOUT_TEXT[4]               = {0x00,0x11,0x01,0x45};
        //const int BUFFER_LENGTH = 10;
        uint8_t _pixel;
        boolean _setRange;
        uint8_t _serial;
        uint32_t _baud;
        bool begun;
        //uint16_t w, x, y, z;
        uint16_t _minDist, _maxDist;
        uint8_t ind;// The variable "ind" will contain the number of actual bytes in the frame to treat in the main loop
        
        //IntervalTimer c;

        uint8_t crc8(uint8_t *crcP, uint8_t len);
        void setPrintout();
        void setTimer(int t);
        void updateSerial1(bool & u);
        void updateSerial2(bool & u);
        void updateSerial3(bool & u);
        void updateSerial4(bool & u);

};

#endif