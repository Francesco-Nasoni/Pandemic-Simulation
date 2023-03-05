#ifndef PERSON_HPP
#define PERSON_HPP

enum class State : char { Susceptible, Infected, Dead };

class Person {

private:
  State state;
  int sociality;

public:
  int getSocial() const;
  State getState() const;
  void setState(State);
  void setSocial(int);
  Person();
};

#endif