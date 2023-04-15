#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "graph.hpp"
#include "pandemicCM.hpp"
#include "utilities.hpp"

int main()
{

    int population_size = 10000; // unica riusata per ora
    double sociality = 4;
    double initial_infected = 0.5 / 100.;
    double B = 0.05;
    double Y = 0.1;
    double R = 0.007;
    double Musk_trigger = 0.2;
    double Quar_trigger = 0.13;
    int Vacc_trigger = 0;

    std::ofstream file{"Data/Result.txt", std::ofstream::trunc};
    std::ofstream fileCM{"Data/ResultCM.txt", std::ofstream::trunc};

    auto desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window{sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20), "Pandemic Similation",
                            sf::Style::Close};
    sf::RenderWindow window_CM{sf::VideoMode(desktop.width / 2, desktop.height / 2 - 20), "Pandemic Similation with CM",
                               sf::Style::Close};
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

    while (window.isOpen() || window_CM.isOpen() || !sample.is_ended() || !sample_CM.is_ended())
    {

        start = std::chrono::steady_clock::now();

        sf::Event event;
        while (window.pollEvent(event))
            ut::proces_event(window, event);

        while (window_CM.pollEvent(event))
            ut::proces_event(window_CM, event);

        if (!sample.is_ended())
        {
            day++;
            sample = sample.evolve();
            ut::write(file, sample, day);
        }
        if (!sample_CM.is_ended())
        {
            day_CM++;
            sample_CM = sample_CM.evolveCM(day_CM);
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