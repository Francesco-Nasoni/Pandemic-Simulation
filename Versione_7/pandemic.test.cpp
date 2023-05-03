#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "header/doctest.h"

#include "header/graph.hpp"
#include "header/m_functions.hpp"
#include "header/pandemic.hpp"
#include "header/pandemicCM.hpp"

#include <chrono>
#include <iostream>

struct Timer {

  std::chrono::time_point<std::chrono::steady_clock> start;
  std::chrono::time_point<std::chrono::steady_clock> end;
  std::chrono::duration<double, std::milli> duration;

  Timer() { start = std::chrono::steady_clock::now(); };

  ~Timer() {
    end = std::chrono::steady_clock::now();
    duration = end - start;
    double time = duration.count();
    std::cout << time << "ms" << '\n';
  };
};

TEST_CASE("Pandemic throw test") {

  SUBCASE("N negative") { CHECK_THROWS(Pandemic{-100, 0.5, 0.5, 0.5, 4, 0.5}); }
  SUBCASE(" Invalid B") { CHECK_THROWS(Pandemic{100, 12, 0.5, 0.5, 4, 0.5}); }
  SUBCASE("Invalid Y") { CHECK_THROWS(Pandemic{100, 0.5, -12, 0.5, 4, 0.5}); }
  SUBCASE("Invalid R") { CHECK_THROWS(Pandemic{100, 0.5, 0.5, 25, 4, 0.5}); }
  SUBCASE("B,Y,R all zeros") { CHECK_THROWS(Pandemic{100, 0, 0, 0, 4, 0.5}); }
  SUBCASE("Invalid sociality") {
    CHECK_THROWS(Pandemic{100, 0.5, 0.5, 0.5, -9, 0.5});
  }

  SUBCASE("Test B=0") {
    Pandemic pan{1000, 0, 0.1, 0.007, 4, 0.05};
    int initial_infected = pan.get_infected();
    while (!pan.is_ended()) {
      pan.evolve();
    }
    CHECK(pan.get_dead() <= initial_infected);
    std::cout << "Finished B=0" << std::endl;
  }

  SUBCASE("Test Y=0") {
    Pandemic pan{1000, 0.05, 0, 0.007, 4, 0.05};
    while (!pan.is_ended()) {
      pan.evolve();
    }
    CHECK(pan.get_susceptible() == 0);
    CHECK(pan.get_dead() == 1000);
    std::cout << "Finished Y=0" << std::endl;
  }

  SUBCASE("Test R=0") {
    Pandemic pan{1000, 0.05, 0.20, 0, 4, 0.5};
    while (!pan.is_ended()) {
      pan.evolve();
    }
    CHECK(pan.get_susceptible() == 1000);
    CHECK(pan.get_dead() == 0);
    std::cout << "Finished R=0" << std::endl;
  }

  SUBCASE("Test R=0, Y=0") {
    Pandemic pan{1000, 0.1, 0, 0, 4, 0.5};
    while (!pan.is_ended()) {
      pan.evolve();
    }
    CHECK(pan.get_susceptible() == 0);
    CHECK(pan.get_infected() == 1000);
    CHECK(pan.get_dead() == 0);
    std::cout << "Finished R=0 and Y=0" << std::endl;
  }
}

TEST_CASE("PancdemicCM.cpp") {
  Pandemic p{100, 0.5, 0.5, 0.5, 4, 0.5};
  SUBCASE("Invalid vacc_B_effect") {
    CHECK_THROWS(PandemicCM{p, -1.2, 1.2, 1.2});
  }
  SUBCASE("Invalid vacc_y_effect") {
    CHECK_THROWS(PandemicCM{p, 1.2, -1.2, 1.2});
  }
  SUBCASE("Invalid vacc_R_effect") {
    CHECK_THROWS(PandemicCM{p, 1.2, 1.2, -1.2});
  }
}