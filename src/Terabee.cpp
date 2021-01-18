#include <Terabee.h>

/*!
  @brief   Terabee constructor with known settings.
  @param   s  Which Serial Port should be used (1-4).
  @param   b  Baudrate (default: 115200).
  @param   p  Pixelmode 1, 2 or 4 (default: 1px).
  @param   r  Long-Range true or false (default: true).
  @return  Terabee object with known Serial Port and Baudrate.
*/
Terabee::Terabee(uint8_t s, uint32_t b, uint8_t p, boolean r ) : begun(true) {
    _serial = s;
    _baud = b;
    _pixel = p;
    _setRange = r;
    
    begin(s, b); 
    setPixelMode(p);
    setLongRange(r);
    setPrintout();
}

Terabee::Terabee() : begun(false) {

}

// Terabee::~Terabee(){
//   if(_serial >= 1){
//     switch(_serial){
//       case 1:
//         Serial1.end();
//         break;
//       case 2:
//         Serial2.end();
//         break;
//       case 3:
//         Serial3.end();
//         break;
//       case 4:
//         Serial4.end();
//         break;
//     }
//   }
// }

/*!
  @brief Starts a new instance of Terabee
  @param s sets Serial port to use (1-4).
  @param b set baud rate (default 115200).
 */
bool Terabee::begin(uint8_t s, uint32_t b){
    _serial = s;
    _baud = b;
    begun = true;
    if(_serial >= 1){
      switch(_serial){
        case 1:
          Serial1.begin(_baud);
          break;
        case 2:
          Serial2.begin(_baud);
          break;
        case 3:
          Serial3.begin(_baud);
          break;
        case 4:
          Serial4.begin(_baud);
          break;
        default:
          begun = false;
          break;
      }
    }
    Serial.printf("Serial%i, has started!\r\n", _serial);
  return begun;
}

/*!
  @brief Sets 1, 2 or 4px mode
  @param p can be 1, 2 or 4
           which will return reading of 1,2 or 4 ranges
           where 4 will be slowest and 1 fastets reading
  @return pixel Mode (1, 2 or 4).
 */
uint8_t Terabee::setPixelMode(uint8_t p){
  _pixel = p;
  if(p<=2) p--;   //Makes sure Pixel 1 selects array field 0 etc
  if(p==4) p=2;   //Makes sure Pixel 4 selects array filed 2
  if(begun && _serial >= 1){
    switch(_serial){
      case 1:
        Serial1.write(pixelmode[p], 4);
        break;
      case 2:
        Serial2.write(pixelmode[p], 4);
        break;
      case 3:
        Serial3.write(pixelmode[p], 4);
        break;
      case 4:
        Serial4.write(pixelmode[p], 4);
        break;
      default:
        Serial.println("Please set value p to 1, 2 or 4.");
        break;
    }
  }
  Serial.printf("Serial%i, was set to Pixelmode %i!\r\n", _serial, _pixel);
  return _pixel;
}

/*!
  @brief Sets Long or short Range.
  @param r if True sets sensor to long range Mode.
           if False sets sensor to more precise short range mode.
  @return true if set to long range. Short range returns false.
 */
bool Terabee::setLongRange(boolean r){
  _setRange = r;
  byte mode;
  _setRange ? mode = 1 : mode = 0;
  if(begun && _serial >= 1){
    switch(_serial){
      case 1:
        Serial1.write(rangemode[mode], 4);
        break;
      case 2:
        Serial2.write(rangemode[mode], 4);
        break;
      case 3:
        Serial3.write(rangemode[mode], 4);
        break;
      case 4:
        Serial4.write(rangemode[mode], 4);
        break;
    }
  }
  Serial.printf("Serial%i, was set to Long Range: %i!\r\n", _serial, _setRange);
  return _setRange;
}

void Terabee::setPrintout(){
  if(begun && _serial >= 1){
    switch(_serial){
      case 1:
        Serial1.write(PRINTOUT_BINARY, 4);
        break;
      case 2:
        Serial2.write(PRINTOUT_BINARY, 4);
        break;
      case 3:
        Serial3.write(PRINTOUT_BINARY, 4);
        break;
      case 4:
        Serial4.write(PRINTOUT_BINARY, 4);
        break;
    }
  }
}

/*!
  @brief Needs to be run in loop(). Checks if any new meassurments are available.
  @return true if new data is available, else returns false.
 */
bool Terabee::update(){
    bool feedback;
    if(!begun) return false;
    switch(_serial){
        case 1:
            updateSerial1(feedback);
            break;
        case 2:
            updateSerial1(feedback);
            break;
        case 3:
            updateSerial1(feedback);
            break;
        case 4:
            updateSerial1(feedback);
            break;
        default:
            Serial.println("No Serial port defined. Did you use \"Terabee.begin()\"?");
            feedback = false;
            break;
    }
    return feedback;
}

void Terabee::updateSerial1(bool & u){
    if (Serial1.available() > 0) {
      // Serial.println("Serial available");   
      // Send data only when you receive data
      u = true;
      uint8_t inChar = Serial1.read();                     
      if (ind == 0) {
        if (inChar == 'T')
        {
          //Looking for frame start 'T'
          Framereceived[ind++] = inChar;
          Serial.println(inChar);
        }
        else u = false;
      }
      else if ((ind > 0) && (ind < 10))
      {
        //Gathering data
        Framereceived[ind++] = inChar;
        Serial.println(inChar);
      }
      //DEBUG
      Serial.printf("Serial%i, was updated!\r\n", _serial);
      Serial.print("Buffer IN: ");
      for(int i = 0; i <= 9; i++){
          Serial.printf(" %i |", Framereceived[i]);
      } 
      Serial.println();
    }
    else u = false;
    
}

void Terabee::updateSerial2(bool & u){
    if (Serial2.available() > 0) {
      // Send data only when you receive data
      uint8_t inChar = Serial2.read();                        
      if (ind == 0) {
        if (inChar == 'T')
        {
          //Looking for frame start 'T'
          Framereceived[ind++] = inChar;
        }
        else u = false;
      }
      else if ((ind > 0) && (ind < 10))
      {
        //Gathering data
        Framereceived[ind++] = inChar;
      }
    }
    Serial.printf("Serial%i, was updated!\r\n", _serial);
    u = true;
}

void Terabee::updateSerial3(bool & u){
    if (Serial3.available() > 0) {
      // Send data only when you receive data
      uint8_t inChar = Serial3.read();                        
      if (ind == 0) {
        if (inChar == 'T')
        {
          //Looking for frame start 'T'
          Framereceived[ind++] = inChar;
        }
        else u = false;
      }
      else if ((ind > 0) && (ind < 10))
      {
        //Gathering data
        Framereceived[ind++] = inChar;
      }
    }
    Serial.printf("Serial%i, was updated!\r\n", _serial);
    u = true;
}

void Terabee::updateSerial4(bool & u){
    if (Serial4.available() > 0) {
      // Send data only when you receive data
      uint8_t inChar = Serial4.read();                        
      if (ind == 0) {
        if (inChar == 'T')
        {
          //Looking for frame start 'T'
          Framereceived[ind++] = inChar;
        }
        else u = false;
      }
      else if ((ind > 0) && (ind < 10))
      {
        //Gathering data
        Framereceived[ind++] = inChar;
      }
    }
    Serial.printf("Serial%i, was updated!\r\n", _serial);
    u = true;
}

/*!
    @brief Returns the distance based on the pixel Setting set before.
    @param w distance in 1px mode
    @return distance in mm. If the CRC check fails, all ints are 0.
*/
void Terabee::getDistance(uint16_t & w){

    //If pixel was set to 1px
    if (ind == 4 && _pixel == 1){
      // Serial.print("Buffer OUT:");
      // for(int i = 0; i <= 9; i++){
      //     Serial.printf(" %i |", Framereceived[i]);
      // } 
      // Serial.println();
      if (crc8(Framereceived, 3) == Framereceived[3]) {
          //Convert bytes to distances
          w = (Framereceived[1]<<8) + Framereceived[2];

          ind = 0;
          Framereceived[0] = 0;
          Serial.printf("Serial%i, read %i!\r\n", _serial, w);
      }
    }
    
    //If CRC check fails
    else {
        w = 0;
        ind = 0;
        Framereceived[0] = 0;
        Serial.printf("CRC check failed!\r\n");
      }
}

/*!
    @brief Returns the distance based on the pixel Setting set before.
    @param w distance 1 in 2px mode
    @param x distance 2 in 2px mode
    @return distance in mm. If the CRC check fails, all ints are 0.
*/
void Terabee::getDistance(uint16_t & w, uint16_t & x){

    //If pixel was set to 2px
    if (ind == 6 && _pixel == 1){
      if (crc8(Framereceived, 5) == Framereceived[5]) {
        //Convert bytes to distances
        w = (Framereceived[1]<<8) + Framereceived[2];
        x = (Framereceived[3]<<8) + Framereceived[4];

        ind = 0;
        Framereceived[0] = 0;
      }
    }
    
    //If CRC check fails
    else
      {
        w = 0;
        x = 0;
        ind = 0;
        Framereceived[0] = 0;
      }
}

/*!
    @brief Returns the distance based on the pixel Setting set before.
    @param w distance 1 in 4px mode
    @param x distance 2 in 4px mode
    @param y distance 3 in 4px mode
    @param z distance 4 in 4px mode
    @return distance in mm. If the CRC check fails, all ints are 0.
*/
void Terabee::getDistance(uint16_t & w, uint16_t & x, uint16_t & y, uint16_t & z){

    //If pixel was set to 1px
    if (ind == 4 && _pixel == 0){
      if (crc8(Framereceived, 3) == Framereceived[3]) {
        //Convert bytes to distance
        w = (Framereceived[1]<<8) + Framereceived[2];
        x = w;
        y = w;
        z = w;

        ind = 0;
        Framereceived[0] = 0;
      }
    }

    //If pixel was set to 2px
    else if (ind == 6 && _pixel == 1){
      if (crc8(Framereceived, 5) == Framereceived[5]) {
        //Convert bytes to distances
        w = (Framereceived[1]<<8) + Framereceived[2];
        x = (Framereceived[3]<<8) + Framereceived[4];
        y = w;
        z = x;

        ind = 0;
        Framereceived[0] = 0;
      }
    }

    //If pixel was set to 4px
    else if (ind == 10 && _pixel == 2){
      if (crc8(Framereceived, 9) == Framereceived[9]) {
        //Convert bytes to distances
        w = (Framereceived[1]<<8) + Framereceived[2];
        x = (Framereceived[3]<<8) + Framereceived[4];
        y = (Framereceived[5]<<8) + Framereceived[6];
        z = (Framereceived[7]<<8) + Framereceived[8];

        ind = 0;
        Framereceived[0] = 0;
      }
    }

    //If CRC check fails
    else
      {
        w = 0;
        x = 0;
        y = 0;
        z = 0;
        ind = 0;
        Framereceived[0] = 0;
      }
}

/*!
  @brief Use the Sensor as a button. Set 
  @param min in mm. Sets minimal trigger distance. Every meassurment below will be neglected.
  @param max in mm. Sets maximal trigger distance. Every meassurment above will be neglected.
  @return True if a meassurment between min and max value is detected for the given time. Else it return false.
*/
void Terabee::button(uint16_t min, uint16_t max){ //@param time in ms. Set how long a meassurment has to be detected in between your values, before a button press is triggered. If not set, button triggers immediately
  _minDist = min;
  _maxDist = max;
  if(_pixel != 0) setPixelMode(1);  
}

bool Terabee::wasPressed(){
    uint16_t dist;
    getDistance(dist); 
    if(dist >= _minDist && dist <= _maxDist){
      return true;
    }
    else return false; 
}

/*!
  @brief Calculate a Cyclic Redundancy Checks of 8 bits
  @param crcP pointer to receive buffer
  @param len number of bytes returned by the TeraRanger
  @return (crc & 0xFF) checksum calculated locally
 */
uint8_t Terabee::crc8(uint8_t *crcP, uint8_t len) {
  uint8_t i;
  uint8_t crc = 0x0;
  while (len--) {
    i = (crc ^ *crcP++) & 0xFF;
    crc = (crc_table[i] ^ (crc << 8)) & 0xFF;
  }
  return crc & 0xFF;
}