#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <thread>

#include "header/graph.hpp"
#include "header/m_functions.hpp"
#include "header/pandemic.hpp"
#include "header/pandemicCM.hpp"

int main() {

  typedef std::chrono::time_point<std::chrono::steady_clock> ch_steady_clock;

  Pandemic sample;
  PandemicCM sample_CM;
  int pop_size;
  bool auto_mode;
  double quar_trigger;
  double quar_goal;
  int quar_max_n;
  int vacc_1_trigger_day;
  int vacc_2_trigger_day;
  int quar_count;

  quar_count = 0;
  try {
    mf::read_from_config(sample, sample_CM, pop_size, auto_mode, quar_trigger,
                         quar_goal, quar_max_n, vacc_1_trigger_day,
                         vacc_2_trigger_day);
  } catch (std::runtime_error &e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    exit(0);
  }

  std::ofstream file{"Result.txt", std::ofstream::trunc};
  std::ofstream fileCM{"ResultCM.txt", std::ofstream::trunc};
  mf::write(file, sample, 0);
  mf::write(fileCM, sample_CM, 0);

  auto const desktop = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation", sf::Style::Close};
  sf::RenderWindow window_CM{
      sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20),
      "Pandemic Similation with CM", sf::Style::Close};
  window.setPosition(sf::Vector2i(0, 0));
  window_CM.setPosition(sf::Vector2i(desktop.width / 2, 0));

  std::vector<std::string> leg_titles{"Susceptible", "Infected", "Dead"};
  std::vector<sf::Color> colors{sf::Color::Blue, sf::Color::Red,
                                sf::Color::Green};
  Graph graph(window, 50., pop_size, colors, leg_titles);
  Graph graph_CM(window_CM, 50., pop_size, colors, leg_titles);

  mf::print(sample, sample_CM, 0, 0, 0);

  for (int day = 1, day_CM = 1; window.isOpen() || window_CM.isOpen() ||
                                !sample.is_ended() || !sample_CM.is_ended();) {

    ch_steady_clock start = std::chrono::steady_clock::now();

    sf::Event event;
    while (window.pollEvent(event))
      mf::process_event(window, event, sample_CM, auto_mode, quar_count);
    while (window_CM.pollEvent(event))
      mf::process_event(window_CM, event, sample_CM, auto_mode, quar_count);

    if (!sample.is_ended()) {
      day++;
      sample.evolve();
      mf::write(file, sample, day);
    }

    if (!sample_CM.is_ended()) {
      if (auto_mode) {
        double ISI = static_cast<double>(sample_CM.get_infected()) /
                     static_cast<double>(sample_CM.get_susceptible() +
                                         sample_CM.get_infected());
        if (!sample_CM.get_quar() && ISI >= quar_trigger && quar_trigger != 0 &&
            quar_count < quar_max_n) {
          sample_CM.toggle_quar();
          quar_count++;
        } else if (sample_CM.get_quar() && ISI <= quar_goal) {
          sample_CM.toggle_quar();
        }

        if (day_CM >= vacc_1_trigger_day && vacc_1_trigger_day != 0) {
          sample_CM.toggle_vacc_1();
        }
        if (day_CM >= vacc_2_trigger_day && vacc_2_trigger_day != 0) {
          sample_CM.toggle_vacc_2();
        }
      }
      day_CM++;
      sample_CM.evolveCM();
      mf::write(fileCM, sample_CM, day_CM);
    }

    mf::print(sample, sample_CM, day, day_CM, quar_count);

    mf::add_point(graph, sample, day);
    mf::add_point(graph_CM, sample_CM, day_CM);
    mf::render(window, graph);
    mf::render(window_CM, graph_CM);

    int wait;
    ch_steady_clock end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    wait = 100 - duration.count();
    if (wait > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(wait));
  }
  std::cout << '\n';
};