#ifndef SOLENOID_H
#define SOLENOID_H

class Solenoid 
{
public:
   void open();
   void close();
   Solenoid();
   Solenoid(int outputPin);
   ~Solenoid();
private:
  int outputPin;
};

/**
 * 
 */
void Solenoid::open()
{

}

/**
 * 
 */
void Solenoid::close()
{

}

/**
 * 
 */
Solenoid::Solenoid()
{
  outputPin = 0;
  this->close();
}

/**
 * 
 */
Solenoid::Solenoid(int outputPin)
{
  this->outputPin = outputPin;
  this->close;
}

/**
 * 
 */
Solenoid::~Solenoid()
{
   this->close();
}

#endif /*SOLENOID_H*/
