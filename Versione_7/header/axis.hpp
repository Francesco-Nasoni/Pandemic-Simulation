#ifndef AXIS_HPP
#define AXIS_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <string>
#include <vector>

enum class Direction : bool { x, y };

constexpr double c_arial_ratio = 0.57;
constexpr int c_font_size = 16;

class Axis {
private:
  sf::RectangleShape line;
  std::vector<sf::RectangleShape> div_rect;
  std::vector<sf::Text> label;
  sf::Font font;

public:
  static constexpr int c_div_rect_length = 7;
  static constexpr int c_label_offset = 3;

  Axis(double max, double length, double width, int n_div, double x, double y,
       Direction direction);
  Axis();
  static double get_font_length(std::string, double);
  static void clear_string(std::string &str);
  void draw(sf::RenderWindow &);
};

#endif