#include "person.hpp"

int Person::getSocial() const { return sociality; }

State Person::getState() const { return state; }

void Person::setState(State s) { state = s; }

void Person::setSocial(int s) { sociality = s; }

Person::Person() { state = State::Susceptible; }