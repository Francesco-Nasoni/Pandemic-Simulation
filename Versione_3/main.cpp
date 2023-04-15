#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "graph.hpp"
#include "pandemicCM.hpp"
#include "utilities.hpp"

int main() {

  int population_size = 10000;

  bool auto_mode = false;
  double quar_trigger = 0.13;
  double quar_goal = 0.23;
  int quar_max_n = 5;
  int vacc_1_trigger = 0;
  int vacc_2_trigger = 0;

  int quar_count = 0;

  Pandemic sample;
  PandemicCM sample_CM;

  ut::configuration(sample, sample_CM, auto_mode, quar_trigger, quar_goal,
                    quar_max_n, vacc_1_trigger, vacc_2_trigger);

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

  while (window.isOpen() || window_CM.isOpen() || !sample.is_ended() ||
         !sample_CM.is_ended()) {

    start = std::chrono::steady_clock::now();

    sf::Event event;
    while (window.pollEvent(event))
      ut::proces_event(window, event, sample_CM, auto_mode);

    while (window_CM.pollEvent(event))
      ut::proces_event(window_CM, event, sample_CM, auto_mode);

    if (!sample.is_ended()) {
      day++;
      sample = sample.evolve();
      ut::write(file, sample, day);
    }
    if (!sample_CM.is_ended()) {
      if (auto_mode) {
        if (static_cast<double>(sample_CM.get_infected()) /
                    (static_cast<double>(sample_CM.get_susceptible()) +
                     static_cast<double>(sample_CM.get_infected())) >=
                quar_trigger &&
            quar_trigger != 0 && quar_count < quar_max_n) {
          sample_CM.toggle_quar();
        }
        if (day_CM >= vacc_1_trigger && vacc_1_trigger != 0) {
          sample_CM.toggle_vacc_1();
        }
        if (day_CM >= vacc_2_trigger && vacc_2_trigger != 0) {
          sample_CM.toggle_vacc_2();
        }
      }
      day_CM++;
      sample_CM = sample_CM.evolveCM();
      ut::write(fileCM, sample_CM, day_CM);
    }

    ut::print(sample, sample_CM, day, day_CM);

    ut::add_point(graph, sample, day);
    ut::add_point(graph_CM, sample_CM, day_CM);

    ut::render(window, graph);
    ut::render(window_CM, graph_CM);

    int wait;
    end = std::chrono::steady_clock::now();
    duration = end - start;
    wait = 100 - duration.count();
    if (wait > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(wait));
  }
  std::cout << '\n';
};