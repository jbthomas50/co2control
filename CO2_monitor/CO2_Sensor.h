#ifndef CO2_SENSOR_H
#define CO2_SENSOR_H

#include <SoftwareSerial.h>
/**
 * 
 */

#define bufferMax 7

class CO2_Sensor
{
public:
  int getCO2();
  void fill_buffer();
  void read();
  void begin();
  CO2_Sensor(SoftwareSerial * CO2_serial);
private:
  uint8_t buffer[10];
  uint8_t ind;
  uint8_t index;
  int co2;
  int multiplier; // 1 for 2% = 20,000 PPM, 10 for 20% = 200,000, 100 for 100%
  int format_output();
  SoftwareSerial *CO2_serial;
};

/**
 * Returns the CO2 in PPM
 */
int CO2_Sensor::getCO2()
{
  return (co2 * multiplier);
}

/**
 * Reads the CO2 Sensor and fills the buffer
 */
void CO2_Sensor::fill_buffer()
{                               // Fill buffer with sensor ascii data
  ind = 0;
  while(buffer[ind-1] != 0x0A)
  {                             // Read sensor and fill buffer up to 0XA = CR
    if((*CO2_serial).available())
    {
      buffer[ind] = (*CO2_serial).read();
      ind++;   
    } 
  }                             // buffer() now filled with sensor ascii data
                                //   ind contains the number of characters loaded into buffer up to 0xA =  CR
  ind = ind - 2;                // decrement buffer to exactly match last numerical character
}

/**
 * 
 */
void CO2_Sensor::read()
{
  for(int i = 0; i < bufferMax; i++)
  {
    buffer[i] = ' ';
  }
  this->CO2_serial->println("Z");          // send Mode Z output for filtered C02 reading
  this->fill_buffer();
  index = 0;
  this->format_output();
}

/**
 * 
 */
void CO2_Sensor::begin()
{
  this->multiplier = 100;
  this->co2 = 0;
  this->ind = 0;
  this->index = 0;
  this->CO2_serial->begin(9600);
  this->CO2_serial->println("K 2");            // set polling mode
}

/**
 * 
 */
int CO2_Sensor::format_output()
{                                                // read buffer, extract 6 ASCII chars, convert to PPM and print
  co2 = buffer[bufferMax-index++]-0x30;
  co2 += (buffer[bufferMax-index++]-0x30)*10;
  co2 += (buffer[bufferMax-index++]-0x30)*100;
  co2 += (buffer[bufferMax-index++]-0x30)*1000;
  co2 += (buffer[bufferMax-index++]-0x30)*10000;
}

/**
 * 
 */
CO2_Sensor::CO2_Sensor(SoftwareSerial *CO2_serial)
{
  this->CO2_serial = CO2_serial;
  this->begin();
}

#endif /*CO2_SENSOR_H*/
