#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "graph.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "pandemicCM.hpp"

#define SPACE std::left << std::setw(8)

struct Samples {
  Pandemic normal;
  PandemicCM c_measure;
};

namespace ut {
void configuration(Pandemic &p1, PandemicCM &p2, int &p_s, bool &a_m,
                   double &q_t, double &q_g, int &q_m_n, int &v_1_t,
                   int &v_2_t);
void render(sf::RenderWindow &window, Graph &graph);
void add_point(Graph &graph, Pandemic const &p, int d);
void proces_event(sf::RenderWindow &window, sf ::Event const &event,
                  PandemicCM &p, bool auto_mode);
void write(std::ostream &os, Pandemic const &sample, int d);
void print(Pandemic const &s1, Pandemic const &s2, int d1, int d2, int q_c);
} // namespace ut

#endif