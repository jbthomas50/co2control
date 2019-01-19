#ifndef FANS_H
#define FANS_H

/**
 * 
 */
class Fans
{
public:
  void off();
<<<<<<< HEAD
  void on();
  Fan(uint8_t pin);
  ~Fan();
=======
  void on(uint8_t &num);
  Fans(uint8_t fan0, uint8_t fan1, uint8_t fan2, uint8_t fan3);
  ~Fans();
>>>>>>> 8675fdd19bc9d8d15756ea2e47ad80f041634607
private:
  uint8_t fanPins[4];
};

/**
 * Turn Fan off
 */
void Fans::off()
{
  for(uint8_t i = 0; i < 4; i++)
  {
    digitalWrite(fanPins[i], LOW);
  }
}

/**
 * Turn Fan on
 */
void Fans::on(uint8_t &num)
{
  for(uint8_t i = 0; i < 4; i++)
  {
    if(i < num)
      digitalWrite(fanPins[i], HIGH);
    else
      digitalWrite(fanPins[i], LOW);
  }
}

/**
 * Non-default constructor
 */
Fans::Fans(uint8_t fan0, uint8_t fan1, uint8_t fan2, uint8_t fan3)
{
  fanPins[0] = fan0;
  fanPins[1] = fan1;
  fanPins[2] = fan2;
  fanPins[3] = fan3;
  for(uint8_t i = 0; i < 4; i++)
  {
    pinMode(fanPins[i], OUTPUT);
    digitalWrite(fanPins[i], LOW); //make sure the fan is of initially
  }
}

/**
 * Makes sure that the Fan is off
 */
Fans::~Fans()
{
   this->off();
}

#endif /*FANS_H*/
