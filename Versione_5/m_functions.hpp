#ifndef MAIN_FUNCTION_HPP
#define MAIN_FUNCTION_HPP

#include "graph.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <thread>

#include "pandemicCM.hpp"

#define SPACE(x) std::left << std::setw(x)

namespace mf {
void read_from_config(Pandemic &sample, PandemicCM &sample_CM, int &pop_size,
                      bool &auto_mode, double &quar_goal, double &quar_trigger,
                      int &quar_max_n, int &vacc_1_trigger,
                      int &vacc_2_trigger);
void render(sf::RenderWindow &window, Graph &graph);
void add_point(Graph &graph, Pandemic const &p, int d);
void proces_event(sf::RenderWindow &window, sf ::Event const &event,
                  PandemicCM &p, bool auto_mode, int &q_c);
void write(std::ostream &os, Pandemic const &sample, int d);
void print(Pandemic const &s1, Pandemic const &s2, int d1, int d2, int q_c);
} // namespace mf

#endif