#ifndef AXIS_HPP
#define AXIS_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <string>
#include <vector>

constexpr int c_div_rect_length = 7;
constexpr int c_font_size = 16;
constexpr double c_arial_ratio = 0.57;
constexpr int c_label_offset = 3;

enum class Direction : bool { x, y };

class Axis {
private:
  sf::RectangleShape line;
  std::vector<sf::RectangleShape> div_rect;
  std::vector<sf::Text> label;
  sf::Font font;

public:
  Axis(double max, double length, double width, int n_div, double x, double y,
       Direction direction);
  Axis();
  static double get_font_length(std::string, double);
  static void clear_string(std::string &str);
  void draw(sf::RenderWindow &);
};

#endif