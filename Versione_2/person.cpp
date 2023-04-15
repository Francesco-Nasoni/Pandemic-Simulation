#include "person.hpp"

int Person::get_social() const { return sociality; }

State Person::get_state() const { return state; }

void Person::set_state(State s) { state = s; }

void Person::set_social(int s) { sociality = s; }

Person::Person() { state = State::Susceptible; }