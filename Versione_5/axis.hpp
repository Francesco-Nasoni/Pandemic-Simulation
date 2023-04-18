#ifndef AXIS_HPP
#define AXIS_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <string>
#include <vector>

constexpr int c_div_length = 7;
constexpr int c_font_size = 16;
constexpr double c_arial_ratio = 0.57;
constexpr int c_label_offset = 3;

enum class Direction : bool { x, y };

class Axis {
private:
  sf::RectangleShape line;
  sf::Color color;
  std::vector<sf::Text> label;
  std::vector<sf::RectangleShape> div;
  sf::Font font;
  double x;
  double y;
  double max;
  double length;
  int nDiv;
  Direction dir;

public:
  Axis(double max, double lenght, double width, int nDiv, sf::Color, double x,
       double y, Direction);
  Axis();
  static double get_lab_offset(std::string, double);
  void draw(sf::RenderWindow &);
};

#endif