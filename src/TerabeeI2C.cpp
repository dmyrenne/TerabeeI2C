#include <TerabeeI2C.h>

/*!
    @brief Creates new Terabee Object with default address 0x31
    @param mode set to SHORT or LONG
*/
Terabee::Terabee(int mode){
    _addr = DEFAULT_ADDR;
    _mode = mode;

    if(mode == SHORT) inter = 25;           //short mode interval
    else if(mode == LONG) inter = 50;       //long mode interval
}

/*!
    @brief Creates new Terabee Object
    @param addr address of the sensor
    @param mode set to SHORT or LONG
    @param cmd  
*/
Terabee::Terabee(int addr, int mode){
    _addr = addr;
    _mode = mode;

    if(mode == SHORT) inter = 25;           //short mode interval
    else if(mode == LONG) inter = 50;       //long mode interval
}

/*!
 @brief Starts communication with Sensor
 */
void Terabee::begin(){
    if(!begun){
        Wire.begin();
        begun = true;
    }
    Wire.beginTransmission(_addr);
  
    if(_mode == SHORT){
        byte distCmd[2] = {GET_DIST, SHORT};
        Wire.write(distCmd, 2);
    } 
    else if (_mode == LONG){
        byte distCmd[2] = {GET_DIST, LONG};
        Wire.write(distCmd, 2);
    }
    // else return;
    Wire.endTransmission();
    delay(25);                  // This delay is necessary to ensure mode is switched correctly
}

/*!
 @brief Requests new sensor data and converts to Millimeter
 @param[out] mm returns meassurement in millimeter 
 @return 1 - meassure out of range.      
         0 - CRC failed / sensor not connected.
 */
void Terabee::getDistance(uint16_t & mm){
    uint8_t buf[3];
    uint8_t CRC;

    Wire.beginTransmission(_addr);           // Transmit to Evo Mini (THIS IS THE I2C BASE ADDRESS, CHANGE HERE IN CASE IT IS DIFFERENT)
    Wire.write(GET_DIST);                        // Sends measure trigger byte
    Wire.endTransmission();                 // Stop transmitting

    Wire.requestFrom(_addr, 3);              // Read back three bytes from Evo Mini (THIS IS THE I2C BASE ADDRESS, CHANGE HERE IN CASE IT IS DIFFERENT)
    buf[0] = Wire.read();                   // First byte of distance
    buf[1] = Wire.read();                   // Second byte of distance
    buf[2] = Wire.read();                   // Byte of checksum

    
    CRC = crc8(buf, 2);                     // Save the "return" checksum in variable "CRC" to compare with the one sent by the TeraRanger

    if (CRC == buf[2]) {                    // If the function crc8 return the same checksum than the TeraRanger, then:
        mm = (buf[0]<<8) + buf[1];        // Calculate distance in mm
    }
    else {
        Serial.println("CRC error!");
        mm = 0;
    }
    delay(inter);                           // This delay is necessary to prevent reading too many times the same measurement (To be adapted depending on the sensor mode)
}

/*!
    @brief  Change I2C address of sensor. Make sure only one is connected at a time.
    @param  newAddr new address
    @return
 */
void Terabee::setAddress(int newAddr){

    Wire.beginTransmission(_addr);
    Wire.write(SET_ADDR);
    Wire.write(newAddr);
    Wire.endTransmission();
    delay(25);
    Wire.end();
    begun = false;

    _addr = newAddr;
    begin();
}

/*!
    @brief  resets I2C address of sensor to 0x31. Make sure only one is connected at a time.
 */
void Terabee::resetAddress(){

    Wire.beginTransmission(_addr);
    Wire.write(SET_ADDR);
    Wire.write(DEFAULT_ADDR);
    Wire.endTransmission();
    delay(25);
    Wire.end();
    begun = false;

    _addr = DEFAULT_ADDR;
    begin();
}

/*!
    @brief  ends the communication and frees up the used I2C pins.
 */
void Terabee::end(){
    Wire.end();
    begun = false;
}
/*!
    @brief  Calculate a Cyclic Redundancy Checks of 8 bits
    @param  p pointer to receive buffer
    @param  len number of bytes returned by the TeraRanger
    @return (crc & 0xFF) checksum calculated locally
 */
uint8_t Terabee::crc8(uint8_t *p, uint8_t len) {
  uint8_t i;
  uint8_t crc = 0x0;
  while (len--) {
    i = (crc ^ *p++) & 0xFF;
    crc = (crc_table[i] ^ (crc << 8)) & 0xFF;
  }
  return crc & 0xFF;
}