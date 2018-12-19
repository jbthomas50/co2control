#ifndef FAN_H
#define FAN_H

class Fan 
{
public:
   void off();
   void on();
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