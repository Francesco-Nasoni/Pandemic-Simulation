#include "graph.hpp"

Graph::Graph(sf::RenderWindow &m_window, double max_x, double max_y, int n)
    : window{m_window}, max_x{max_x}, max_y{max_y}, x_exst{1}, y_exst{1} {

  if (!font.loadFromFile("arial.ttf")) {
    throw std::runtime_error{"cannot load font, please download arial.ttf"};
  }

  resize(max_x, max_y);
  for (int i = 0; i < n; i++) {
    sf::VertexArray a;
    color.push_back(sf::Color::Blue);
    point.push_back(a);
    point[i].setPrimitiveType(sf::LineStrip);
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
      (static_cast<double>(size.y) - BOTTOM_MARGIN - TOP_MARGIN) / y_exst;

  x_exst = static_cast<double>(size.x) - r_margin - l_margin;
  y_exst = static_cast<double>(size.y) - BOTTOM_MARGIN - TOP_MARGIN;
  double x_ax_y = static_cast<double>(size.y) - BOTTOM_MARGIN;

  Axis x(m_x, x_exst, WIDTH, N_DIV, sf::Color::Black, l_margin, x_ax_y,
         Direction::x);
  Axis y(m_y, WIDTH, y_exst, N_DIV, sf::Color::Black, l_margin, y_ax_y,
         Direction::y);

  x_axis = x;
  y_axis = y;

  double x_ratio = max_x / m_x * ext_ratio_x;
  double y_ratio = max_y / m_y * ext_ration_y;

  for (auto &v : point)
    for (long unsigned i = 0; i < v.getVertexCount(); i++) {
      v[i].position.x = (v[i].position.x - o_l_margin) * x_ratio + l_margin;
      v[i].position.y =
          (v[i].position.y - static_cast<double>(window.getSize().y) +
           BOTTOM_MARGIN) *
              y_ratio +
          static_cast<double>(window.getSize().y) - BOTTOM_MARGIN;
    }
  max_x = m_x;
  max_y = m_y;
}

void Graph::set_color(sf::Color c, int i) { color[i - 1] = c; }

void Graph::set_RL_margin(double m_x, double m_y) {

  std::string s = std::to_string(m_y);
  s.erase(s.find_last_not_of('0') + 1, std::string::npos);
  s.erase(s.find_last_not_of('.') + 1, std::string::npos);

  l_margin = Axis::get_lab_offset(s, FONT_SIZE) + LEFT_MARGIN +
             (DIV_LENGTH - 1) / 2 * WIDTH + LABEL_OFFSET;

  if (std::round(m_y) == m_y && (int)m_y % N_DIV != 0)
    l_margin = l_margin + FONT_SIZE / 2;

  s = std::to_string(m_x);
  s.erase(s.find_last_not_of('0') + 1, std::string::npos);
  s.erase(s.find_last_not_of('.') + 1, std::string::npos);

  r_margin = Axis::get_lab_offset(s, FONT_SIZE);
}

void Graph::add_point(sf::Vector2f n_point, int i) {
  double scale_x = x_exst / max_x;
  double scale_y = y_exst / max_y;
  n_point.x = n_point.x * scale_x + l_margin;
  n_point.y = static_cast<double>(window.getSize().y) - n_point.y * scale_y -
              BOTTOM_MARGIN;

  point[i - 1].append(n_point);
  point[i - 1][point[i - 1].getVertexCount() - 1].color = color[i - 1];
}

void Graph::add_to_legend(std::string str, int i) {

  i = i - 1;

  sf::Vector2f lab_pos;
  lab_pos.x = window.getSize().x - LEGEND_MARGIN -
              LEGEND_N_CHAR * FONT_SIZE * ARIAL_RATIO;
  lab_pos.y = LEGEND_MARGIN + i * (FONT_SIZE + LEGEND_INTL);

  sf::Vector2f rec_pos;
  rec_pos.x = lab_pos.x - LABEL_OFFSET - LEGEND_REC_L;
  rec_pos.y = lab_pos.y;

  leg_rec.push_back(
      sf::RectangleShape(sf::Vector2f(LEGEND_REC_L, LEGEND_REC_L)));
  leg_rec[i].setPosition(rec_pos);
  leg_rec[i].setFillColor(color[i]);

  sf::Text tex;
  if (str.size() > LEGEND_N_CHAR)
    str = str.erase(LEGEND_N_CHAR);
  tex.setString(str);
  tex.setCharacterSize(FONT_SIZE);
  tex.setPosition(lab_pos);
  tex.setFillColor(sf::Color::Black);
  leg_label.push_back(tex);

  sf::Vector2f box_pos;
  sf::Vector2f box_size;

  box_pos.x = rec_pos.x - LEGEND_MARGIN / 2;
  box_pos.y = LEGEND_MARGIN / 2;

  box_size.x = LEGEND_MARGIN + LEGEND_N_CHAR * FONT_SIZE * ARIAL_RATIO;
  box_size.y = LEGEND_MARGIN + leg_label.size() * FONT_SIZE +
               (leg_label.size() - 1) * LEGEND_INTL;

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