#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "pandemicCM.hpp"
#include "utilities.hpp"
#include "graph.hpp"

int main() {
  int population_size = 10000;
  double sociality = 4;
  double initial_infected = 5 / 100.;
  double B = 0.4;
  double Y = 0.1;
  double R = 0.007;
  double Musk_trigger = 0.2;
  double Quar_trigger = 0.13;
  int Vacc_trigger = 0;

  std::ofstream file{"Data/Result.txt", std::ofstream::trunc};
  std::ofstream fileCM{"Data/ResultCM.txt", std::ofstream::trunc};

  auto desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation", sf::Style::Close};
  sf::RenderWindow window_CM{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation with CM", sf::Style::Close};
  window.setPosition(sf::Vector2i(0, 0));
  window_CM.setPosition(sf::Vector2i(desktop.width / 2, 0));

  Pandemic sample(population_size, B, Y, R, sociality, initial_infected);
  PandemicCM sample_CM(sample, Musk_trigger, Quar_trigger, Vacc_trigger);
  sample_CM.setMusk_B_effect(0.8);
  sample_CM.setVacc_B_effect(0.8);
  sample_CM.setVacc_Y_effect(1.2);
  sample_CM.setVacc_R_effect(0.80);
  sample_CM.setQuar_goal(0.04);

  Graph graph(window, 50., population_size, 3);
  graph.setColor(sf::Color::Red, 2);
  graph.setColor(sf::Color::Green, 3);

  Graph graph_CM(window_CM, 50., population_size, 3);
  graph_CM.setColor(sf::Color::Red, 2);
  graph_CM.setColor(sf::Color::Green, 3);

  int day = 0;
  int day_CM = 0;

  ut::write(file, sample, 0);
  ut::write(fileCM, sample, 0);
  ut::print(sample, sample_CM, 0, 0);

  std::chrono::time_point<std::chrono::steady_clock> start;
  std::chrono::time_point<std::chrono::steady_clock> end;
  std::chrono::duration<double, std::milli> duration;

  while (window.isOpen() || window_CM.isOpen() || !sample.isEnded() ||
         !sample_CM.isEnded()) {
    
    start = std::chrono::steady_clock::now();

    sf::Event event;
    while (window.pollEvent(event))
      ut::procesEvent(window, event);

    while (window_CM.pollEvent(event))
      ut::procesEvent(window_CM, event);

    if (!sample.isEnded()) {
      day++;
      sample = sample.evolve();
      ut::write(file, sample, day);
    }
    if (!sample_CM.isEnded()) {
      day_CM++;
      sample_CM = sample_CM.evolveCM(day_CM);
      ut::write(fileCM, sample_CM, day_CM);
    }
    
    ut::print(sample, sample_CM, day, day_CM);

    ut::addPoint(graph, sample, day);
    ut::addPoint(graph_CM, sample_CM, day_CM);

    ut::render(window, graph);
    ut::render(window_CM, graph_CM);

    int wait_t;
    end = std::chrono::steady_clock::now();
    duration = end - start;
    wait_t = 100 - duration.count();
    if (wait_t >0)
      std::this_thread::sleep_for(std::chrono::milliseconds(wait_t));
  }
  std::cout << '\n';
};