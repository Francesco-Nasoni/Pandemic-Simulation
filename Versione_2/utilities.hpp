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

#define FIXED_FLOAT(x) std::fixed << std::setprecision(x)
#define SPACE(x) std::left << std::setw(x)

namespace ut{
    void render(sf::RenderWindow &window, Graph &graph);
    void addPoint(Graph &graph, Pandemic const &p, int d);
    void procesEvent(sf::RenderWindow &window, sf ::Event const &event);
    void write(std::ostream &os, Pandemic const &sample, int d);
    void print(Pandemic const &s1, Pandemic const &s2, int d1, int d2);
}

#endif