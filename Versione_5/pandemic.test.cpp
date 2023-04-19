#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "graph.hpp"
#include "m_functions.hpp"
#include "pandemic.hpp"
#include "pandemicCM.hpp"

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
      pan = pan.evolve();
    }
    CHECK(pan.get_dead() <= initial_infected);
    std::cout << "Finished B=0" << std::endl;
  }

  SUBCASE("Test Y=0") {
    Pandemic pan{1000, 0.05, 0, 0.007, 4, 0.05};
    while (!pan.is_ended()) {
      pan = pan.evolve();
    }
    CHECK(pan.get_susceptible() == 0);
    CHECK(pan.get_dead() == 1000);
    std::cout << "Finished Y=0" << std::endl;
  }

  SUBCASE("Test R=0") {
    Pandemic pan{1000, 0.05, 0.20, 0, 4, 0.5};
    while (!pan.is_ended()) {
      pan = pan.evolve();
    }
    CHECK(pan.get_susceptible() == 1000);
    CHECK(pan.get_dead() == 0);
    std::cout << "Finished R=0" << std::endl;
  }

  SUBCASE("Test R=0, Y=0") {
    Pandemic pan{1000, 0.1, 0, 0, 4, 0.5};
    while (!pan.is_ended()) {
      pan = pan.evolve();
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

TEST_CASE("Performances of pandemic") {

  auto desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation", sf::Style::Close};
  window.setPosition(sf::Vector2i(0, 0));

  SUBCASE("Performnce of printing on console") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    {
      Timer timer;
      mf::print(sample, sample, 10, 10, 0);
      std::string str(40, '=');
      std::cout << str << '\n';
      std::cout << "PERFORMANCESC WITH N=10K\n";
      std::cout << str;

      std::cout << "\nTime for print is about ";
    }
  }

  SUBCASE("Performnce of printing on file") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    std::ofstream file{"Data/Result.test.txt", std::ofstream::trunc};
    {
      Timer timer;
      mf::write(file, sample, 0);
      std::cout << "\nTime for write on the file is about ";
    }
  }

  SUBCASE("performance of evolve in the beginning") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    std::cout << "\nTime for evolve in the beginningis is about ";
    {
      Timer timer;
      sample = sample.evolve();
    }
  }

  SUBCASE("performance of evolve some where in the middle") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    std::cout << "\nTime for evolve near the pick is about ";
    for (int i = 0; i < 70; i++) {
      sample = sample.evolve();
    }
    {
      Timer timer;
      sample = sample.evolve();
    }
  }

  SUBCASE("Graph performance in the beginning") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);

    std::vector<std::string> leg_titles{"Susceptible", "Infected", "Dead"};
    std::vector<sf::Color> colors{sf::Color::Blue, sf::Color::Red,
                                  sf::Color::Green};
    Graph graph(window, 50., 10000, colors, leg_titles);
    mf::render(window, graph);

    std::cout << "\nTime for update the graph in the begining is about ";
    {
      Timer timer;
      mf::add_point(graph, sample, 0);
      mf::render(window, graph);
    }
  }

  window.clear(sf::Color::White);

  SUBCASE("Graph performance") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);

    std::vector<std::string> leg_titles{"Susceptible", "Infected", "Dead"};
    std::vector<sf::Color> colors{sf::Color::Blue, sf::Color::Red,
                                  sf::Color::Green};
    Graph graph(window, 50., 10000, colors, leg_titles);

    int i = 0;
    while (sample.get_infected() > 0) {
      sample = sample.evolve();
      mf::add_point(graph, sample, i);
      i++;
    }
    mf::render(window, graph);
    std::cout << "\nTime for update the graph in the end is about ";
    {
      Timer timer;
      mf::add_point(graph, sample, i);
      mf::render(window, graph);
    }
  }
  window.close();
}

TEST_CASE("Performances of pandemicCM") {

  SUBCASE("performance of evolveCM in a quarantine near the peak") {
    Pandemic pan(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    PandemicCM sampleCM(pan, 0.8, 1.2, 0.8);
    for (int i = 0; i < 85; i++) {
      sampleCM = sampleCM.evolveCM();
    }
    sampleCM.toggle_quar();
    std::cout << "\nTime for evolveCM in a quarantine near the peak is about ";
    {
      Timer timer;
      sampleCM = sampleCM.evolveCM();
    }
  }

  SUBCASE("performance of evolveCM with vaccination near the peak") {
    Pandemic pan(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    PandemicCM sampleCM(pan, 0.8, 1.2, 0.8);
    for (int i = 0; i < 85; i++) {
      sampleCM = sampleCM.evolveCM();
    }
    sampleCM.toggle_vacc_2();
    std::cout << "\nTime for evolveCM with vaccination near the pick is about ";
    {
      Timer timer;
      sampleCM = sampleCM.evolveCM();
    }
  }
}