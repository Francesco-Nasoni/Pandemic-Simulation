#ifndef PERSON_HPP
#define PERSON_HPP

enum class State { Susceptible, Infected, Dead };

class Person {
private:
  State state;
  int sociality;

public:
  int get_social() const;
  State get_state() const;
  void set_state(State);
  void set_social(int);
  Person();
};

#endif