#include "../header/axis.hpp"

Axis::Axis(double max, double length, double width, int n_div, double x,
           double y, Direction direction) {
  if (!font.loadFromFile("arial.ttf")) {
    throw std::runtime_error{"cannot load font, please download arial.ttf"};
  }

  sf::Color color = sf::Color::Black;

  line.setPosition(sf::Vector2f(x, y));
  line.setFillColor(color);
  if (direction == Direction::x) {
    line.setSize(sf::Vector2f(length, width));
  } else {
    line.setSize(sf::Vector2f(width, length));
  }

  for (int i = 0; i <= n_div; i++) {
    sf::Text tex;
    sf::RectangleShape rect;
    std::string label_string = std::to_string(i * max / n_div);

    clear_string(label_string);

    sf::Vector2f rect_dim;
    sf::Vector2f rect_pos;
    sf::Vector2f lab_pos;

    double division_length = length / static_cast<double>(n_div);
    if (direction == Direction::x) {
      rect_dim = sf::Vector2f(width, c_div_rect_length * width);
      rect_pos = sf::Vector2f(i * division_length + x,
                              y - (c_div_rect_length - 1) / 2 * width);
      lab_pos = sf::Vector2f(
          i * division_length + x -
              get_font_length(label_string, c_font_size) / 2 + width / 2,
          y + c_div_rect_length + c_label_offset);
    } else {
      rect_dim = sf::Vector2f(c_div_rect_length * width, width);
      rect_pos = sf::Vector2f(x - (c_div_rect_length - 1) / 2 * width,
                              -i * division_length + y + length);
      lab_pos = sf::Vector2f(
          x - c_label_offset - (c_div_rect_length - 1) / 2 * width -
              get_font_length(label_string, c_font_size),
          -i * division_length + y + length - c_font_size / 2);
    }

    rect.setSize(rect_dim);
    rect.setPosition(rect_pos);
    rect.setFillColor(color);

    tex.setString(label_string);
    tex.setCharacterSize(c_font_size);
    tex.setPosition(lab_pos);
    tex.setFillColor(color);

    div_rect.push_back(rect);
    label.push_back(tex);
  }
}

Axis::Axis() {}

double Axis::get_font_length(std::string s, double f) {
  return s.length() * f * c_arial_ratio;
}

void Axis::clear_string(std::string &str) {
  str.erase(str.find_last_not_of('0') + 1, std::string::npos);
  str.erase(str.find_last_not_of('.') + 1, std::string::npos);
}

void Axis::draw(sf::RenderWindow &window) {
  window.draw(line);
  for (auto &v : label) {
    v.setFont(font);
    window.draw(v);
  }
  for (auto &v : div_rect)
    window.draw(v);
}
