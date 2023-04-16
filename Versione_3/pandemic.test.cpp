#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "graph.hpp"
#include "pandemic.hpp"
#include "utilities.hpp"

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

TEST_CASE("Performances of evolve") {

  auto desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation", sf::Style::Close};
  window.setPosition(sf::Vector2i(0, 0));

  SUBCASE("Performnce of printing on console") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    {
      Timer timer;
      ut::print(sample, sample, 10, 10);
      std::cout << "\nTime for print is about ";
    }
  }

  SUBCASE("Performnce of printing on file") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);
    std::ofstream file{"Data/Result.test.txt", std::ofstream::trunc};
    {
      Timer timer;
      ut::write(file, sample, 0);
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

    Graph graph(window, 50., 10000, 3);
    graph.set_color(sf::Color::Red, 2);
    graph.set_color(sf::Color::Green, 3);
    ut::render(window, graph);

    std::cout << "\nTime for update the graph in the begining is about ";
    {
      Timer timer;
      ut::add_point(graph, sample, 0);
      ut::render(window, graph);
    }
  }

  window.clear(sf::Color::White);

  SUBCASE("Graph performance") {
    Pandemic sample(10000, 0.05, 0.1, 0.007, 4, 0.5 / 100.);

    Graph graph(window, 50., 10000, 3);
    graph.set_color(sf::Color::Red, 2);
    graph.set_color(sf::Color::Green, 3);

    int i = 0;
    while (sample.get_infected() > 0) {
      sample = sample.evolve();
      ut::add_point(graph, sample, i);
      i++;
    }
    ut::render(window, graph);
    std::cout << "\nTime for update the graph in the end is about ";
    {
      Timer timer;
      ut::add_point(graph, sample, i);
      ut::render(window, graph);
    }
  }
  window.close();
}