#ifndef CO2_SENSOR_H
#define CO2_SENSOR_H

#include <SoftwareSerial.h>
/**
 * 
 */
class CO2_Sensor
{
public:
  void print();
  int getCO2();
  void fill_buffer();
  int format_output();
  void read();
  void begin(uint32_t baud);
  CO2_Sensor(SoftwareSerial * CO2_serial, uint32_t baud);
  CO2_Sensor();
private:
  uint8_t buffer[25];
  uint8_t ind;
  uint8_t index;
  int co2;
  double multiplier; // 1 for 2% = 20,000 PPM, 10 for 20% = 200,000
  SoftwareSerial *CO2_serial;
};

/**
 * 
 */
void CO2_Sensor::print()
{
  for(int i = 0; i < this->ind; i++)
  {
    this->index = 0;
    this->format_output();
    Serial.print(" Raw PPM        ");
    index = 8;
    this->format_output();
    Serial.println(" Filtered PPM\n\n");
  }
}

/**
 * Returns the CO2 in PPM
 */
int CO2_Sensor::getCO2()
{
  return (this->co2 * this->multiplier);
}

/**
 * 
 */
int CO2_Sensor::format_output()
{
  co2 = buffer[15-index]-0x30;
  co2 = co2+((buffer[14-index]-0x30)*10);
  co2 +=(buffer[13-index]-0x30)*100;
  co2 +=(buffer[12-index]-0x30)*1000;
  co2 +=(buffer[11-index]-0x30)*10000;
  Serial.print("\n CO2 = ");
  Serial.print(co2*multiplier,0);
// Serial.print(" PPM,");
//    Serial.print("\n");
  delay(200);
}

/**
 * Reads the CO2 Sensor and fills the buffer
 */
void CO2_Sensor::fill_buffer()
{
  // Fill buffer with sensor ascii data
  this->ind = 0;
  while(this->buffer[ind-1] != 0x0A)
  {  // Read sensor and fill buffer up to 0XA = CR
    if(this->CO2_serial->available())
    {
      buffer[this->ind] = this->CO2_serial->read();
      this->ind++;
    } 
  }
  // buffer() now filled with sensor ascii data
  // ind contains the number of characters loaded into buffer up to 0xA =  CR
  ind = ind -2; // decrement buffer to exactly match last numerical character
}

/**
 * 
 */
void CO2_Sensor::read()
{
  this->fill_buffer();
}

/**
 * 
 */
void CO2_Sensor::begin(uint32_t baud)
{
  this->multiplier = 1;
  this->co2 = 0;
  this->ind = 0;
  this->index = 0;
  this->CO2_serial->begin(baud);
}

/**
 * 
 */
CO2_Sensor::CO2_Sensor(SoftwareSerial *CO2_serial, uint32_t baud)
{
  this->CO2_serial = CO2_serial;
  this->begin(baud);
}

/**
 * 
 */
CO2_Sensor::CO2_Sensor()
{
  
}


#endif /*CO2_SENSOR_H*/
