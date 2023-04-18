#include "graph.hpp"

Graph::Graph(sf::RenderWindow &m_window, double max_x, double max_y, std::vector<sf::Color> col, std::vector<std::string> tit)
    : window{m_window}, max_x{max_x}, max_y{max_y}, x_exst{1}, y_exst{1} {

  if (!font.loadFromFile("arial.ttf")) {
    throw std::runtime_error{"cannot load font, please download arial.ttf"};
  }

  resize(max_x, max_y);
  for (long unsigned int i = 0; i < col.size() ; i++) {
    sf::VertexArray a;
    color.push_back(col[i]);
    point.push_back(a);
    point[i].setPrimitiveType(sf::LineStrip);
    add_to_legend(tit[i], i);
  }

  leg_box.setFillColor(sf::Color::Transparent);
  leg_box.setOutlineThickness(1);
  leg_box.setOutlineColor(sf::Color::Black);
}

double Graph::get_max_x() const { return max_x; }

double Graph::get_max_y() const { return max_y; }

void Graph::resize(double m_x, double m_y) {
  sf::Vector2u size = window.getSize();
  double o_l_margin = l_margin;

  this->set_RL_margin(m_x, m_y);

  double ext_ratio_x =
      (static_cast<double>(size.x) - r_margin - l_margin) / x_exst;
  double ext_ration_y =
      (static_cast<double>(size.y) - c_margin_bottom - c_margin_top) / y_exst;

  x_exst = static_cast<double>(size.x) - r_margin - l_margin;
  y_exst = static_cast<double>(size.y) - c_margin_bottom - c_margin_top;
  double x_ax_y = static_cast<double>(size.y) - c_margin_bottom;

  Axis x(m_x, x_exst, c_axis_width, c_axis_n_div, sf::Color::Black, l_margin,
         x_ax_y, Direction::x);
  Axis y(m_y, c_axis_width, y_exst, c_axis_n_div, sf::Color::Black, l_margin,
         c_y_ax_y, Direction::y);

  x_axis = x;
  y_axis = y;

  double x_ratio = max_x / m_x * ext_ratio_x;
  double y_ratio = max_y / m_y * ext_ration_y;

  for (auto &v : point)
    for (long unsigned i = 0; i < v.getVertexCount(); i++) {
      v[i].position.x = (v[i].position.x - o_l_margin) * x_ratio + l_margin;
      v[i].position.y =
          (v[i].position.y - static_cast<double>(window.getSize().y) +
           c_margin_bottom) *
              y_ratio +
          static_cast<double>(window.getSize().y) - c_margin_bottom;
    }
  max_x = m_x;
  max_y = m_y;
}

void Graph::set_RL_margin(double m_x, double m_y) {

  std::string s = std::to_string(m_y);
  s.erase(s.find_last_not_of('0') + 1, std::string::npos);
  s.erase(s.find_last_not_of('.') + 1, std::string::npos);

  l_margin = Axis::get_lab_offset(s, c_font_size) + c_margin_left +
             (c_div_length - 1) / 2 * c_axis_width + c_label_offset;

  if (std::round(m_y) == m_y && (int)m_y % c_axis_n_div != 0)
    l_margin = l_margin + c_font_size / 2;

  s = std::to_string(m_x);
  s.erase(s.find_last_not_of('0') + 1, std::string::npos);
  s.erase(s.find_last_not_of('.') + 1, std::string::npos);

  r_margin = Axis::get_lab_offset(s, c_font_size);
}

void Graph::add_point(sf::Vector2f n_point, int i) {
  double scale_x = x_exst / max_x;
  double scale_y = y_exst / max_y;
  n_point.x = n_point.x * scale_x + l_margin;
  n_point.y = static_cast<double>(window.getSize().y) - n_point.y * scale_y -
              c_margin_bottom;

  point[i - 1].append(n_point);
  point[i - 1][point[i - 1].getVertexCount() - 1].color = color[i - 1];
}

void Graph::add_to_legend(std::string str, int i) {

  sf::Vector2f lab_pos;
  lab_pos.x = window.getSize().x - c_legend_margin -
              c_legend_n_char * c_font_size * c_arial_ratio;
  lab_pos.y = c_legend_margin + i * (c_font_size + c_legend_int_line);

  sf::Vector2f rec_pos;
  rec_pos.x = lab_pos.x - c_label_offset - c_legend_square_l;
  rec_pos.y = lab_pos.y;

  leg_rec.push_back(
      sf::RectangleShape(sf::Vector2f(c_legend_square_l, c_legend_square_l)));
  leg_rec[i].setPosition(rec_pos);
  leg_rec[i].setFillColor(color[i]);

  sf::Text tex;
  if (str.size() > c_legend_n_char)
    str = str.erase(c_legend_n_char);
  tex.setString(str);
  tex.setCharacterSize(c_font_size);
  tex.setPosition(lab_pos);
  tex.setFillColor(sf::Color::Black);
  leg_label.push_back(tex);

  sf::Vector2f box_pos;
  sf::Vector2f box_size;

  box_pos.x = rec_pos.x - c_legend_margin / 2;
  box_pos.y = c_legend_margin / 2;

  box_size.x = c_legend_margin + c_legend_n_char * c_font_size * c_arial_ratio;
  box_size.y = c_legend_margin + leg_label.size() * c_font_size +
               (leg_label.size() - 1) * c_legend_int_line;

  leg_box.setPosition(box_pos);
  leg_box.setSize(box_size);
}

void Graph::draw() {
  x_axis.draw(window);
  y_axis.draw(window);
  for (auto &v : point)
    window.draw(v);
  for (long unsigned int i = 0; i < leg_label.size(); i++) {
    leg_label[i].setFont(font);
    window.draw(leg_label[i]);
    window.draw(leg_rec[i]);
  }
  window.draw(leg_box);
}