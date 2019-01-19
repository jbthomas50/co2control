#ifndef FAN_H
#define FAN_H

/**
 * 
 */
class Fan 
{
public:
  bool isOn();
  void off();
  void on();
  Fan(uint8_t pin);
  ~Fan();
private:
  uint8_t pin;
  bool status;
};

/**
 * Tells if Fan is on or off
 */
bool Fan::isOn(){
  return this->status;
}

/**
 * Turn Fan off
 */
void Fan::off()
{
  digitalWrite(this->pin, LOW);
  this->status = false;
}

/**
 * Turn Fan on
 */
void Fan::on()
{
  digitalWrite(this->pin, HIGH);
  this->status = true;
}

/**
 * Non-default constructor
 */
Fan::Fan(uint8_t pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
  this->off();
}

/**
 * Makes sure that the Fan is off
 */
Fan::~Fan()
{
   this->off();
}

#endif /*FAN_H*/
