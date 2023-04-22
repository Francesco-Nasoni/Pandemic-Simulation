#include "axis.hpp"

Axis::Axis(double max, double length, double width, int nDiv, sf::Color color,
           double x, double y, Direction d)
    : color{color}, x{x}, y{y}, max{max}, length{length}, nDiv{nDiv}, dir{d} {
  if (!font.loadFromFile("arial.ttf")) {
    throw std::runtime_error{"cannot load font, please download arial.ttf"};
  }

  line.setSize(sf::Vector2f(length, width));
  line.setPosition(sf::Vector2f(x, y));
  line.setFillColor(color);

  div.reserve(nDiv);

  for (int i = 0; i <= nDiv; i++) {
    sf::Text tex;
    sf::RectangleShape rec;
    std::string str = std::to_string(i * max / nDiv);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);

    double l;
    sf::Vector2f rec_dim;
    sf::Vector2f rec_pos;
    sf::Vector2f lab_pos;

    if (dir == Direction::x) {
      l = length / static_cast<double>(nDiv);
      rec_dim = sf::Vector2f(width, c_div_length * width);
      rec_pos = sf::Vector2f(i * l + x, y - (c_div_length - 1) / 2 * width);
      lab_pos = sf::Vector2f(i * l + x - get_lab_offset(str, c_font_size) / 2 +
                                 width / 2,
                             y + c_div_length + c_label_offset);
    } else {
      l = width / static_cast<double>(nDiv);
      rec_dim = sf::Vector2f(c_div_length * length, length);
      rec_pos =
          sf::Vector2f(x - (c_div_length - 1) / 2 * length, -i * l + y + width);
      lab_pos =
          sf::Vector2f(x - c_label_offset - (c_div_length - 1) / 2 * length -
                           get_lab_offset(str, c_font_size),
                       -i * l + y + width - c_font_size / 2);
    }

    rec.setSize(rec_dim);
    rec.setPosition(rec_pos);
    rec.setFillColor(color);

    tex.setString(str);
    tex.setCharacterSize(c_font_size);
    tex.setPosition(lab_pos);
    tex.setFillColor(color);

    div.push_back(rec);
    label.push_back(tex);
  }
}

Axis::Axis() {}

double Axis::get_lab_offset(std::string s, double f) {
  return s.length() * f * c_arial_ratio;
}

void Axis::draw(sf::RenderWindow &window) {
  window.draw(line);
  for (auto &v : label) {
    v.setFont(font);
    window.draw(v);
  }
  for (auto &v : div)
    window.draw(v);
}
