#ifndef SOLENOID_H
#define SOLENOID_H

class Solenoid 
{
public:
   void open();
   void close();
   Solenoid();
   ~Solenoid();
private:

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
   this->close();
}

/**
 * 
 */
Solenoid::~Solenoid()
{
   this->close();
}

#endif /*SOLENOID_H*/