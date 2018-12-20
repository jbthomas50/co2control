#ifndef FAN_H
#define FAN_H

/**
 * 
 */
class Fan 
{
public:
   void off();
   void on();
   Fan(uint8_t pin);
   Fan();
   ~Fan();
private:
  
};

/**
 * 
 */
void Fan::off()
{

}

/**
 * 
 */
void Fan::on()
{

}

/**
 * 
 */
Fan::Fan(uint8_t pin)
{
  
}

/**
 * 
 */
Fan::Fan()
{
   this->off();
}

/**
 * 
 */
Fan::~Fan()
{
   this->off();
}

#endif /*FAN_H*/
