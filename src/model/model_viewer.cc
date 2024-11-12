#include "model_viewer.h"

namespace s21 {

void ModelViewer::Parser(const std::string &path) {
  Clear();
  std::ifstream file;
  file.open(path.c_str(), std::ios::in | std::ios::binary);
  if (file.is_open()) {
    std::string str;
    std::vector<std::string> elements;
    while (std::getline(file, str)) {
      str += '\0';
      ParserLine(str, elements);
      if (!elements.empty()) {
        PushFacets(elements);
      }
    }
    data_.v_count = data_.vertexes.size();
  }
  file.close();
}

void ModelViewer::PushFacets(std::vector<std::string> &elements) {
  std::vector<std::string>::iterator itr(elements.begin());
  int f = 0;
  while (itr != elements.end()) {
    f = std::stoi(*itr);
    if (f < 0) {
      f = data_.vertexes.size() / 3 + f + 1;
    }
    if (itr == elements.begin()) {
      data_.facets.push_back(f - 1);
      data_.f_count += 1;
    } else {
      data_.facets.push_back(f - 1);
      data_.facets.push_back(f - 1);
      data_.f_count += 2;
    }
    itr++;
  }
  f = std::stoi(*elements.begin());
  if (f < 0) {
    f = data_.vertexes.size() / 3 + f + 1;
  }
  data_.facets.push_back(f - 1);
  data_.f_count += 1;
  elements.clear();
}

void ModelViewer::ParserLine(const std::string &str,
                             std::vector<std::string> &elements) {
  std::string number;
  for (size_t i = 0; i < str.size(); i++) {
    if (i > 1 && (str[0] == 'v' || str[0] == 'f') && str[1] == ' ') {
      if (str[i] != ' ' && str[i] != '\0' && str[i] != '\n' && str[i] != '\r') {
        number += str[i];
      } else if (!number.empty()) {
        if (str[0] == 'v') {
          data_.vertexes.push_back(std::stod(number));
          if (i % 3 == 0) {
            MaxMin(data_, 0);
          } else if (i % 3 == 1) {
            MaxMin(data_, 1);
          } else if (i % 3 == 2) {
            MaxMin(data_, 2);
          }
          if (data_.max_coord < data_.vertexes.back()) {
            data_.max_coord = data_.vertexes.back();
          }
        } else if (str[0] == 'f') {
          if (number.find('/') == std::string::npos) {
            elements.push_back(number);
          } else {
            elements.push_back(number.erase(number.find_first_of('/')));
          }
        }
        number.clear();
      }
    }
  }
}

void ModelViewer::MaxMin(Data &data_, const size_t &i) {
  if (data_.vertexes.back() > data_.max_min_xyz[i][0]) {
    data_.max_min_xyz[i][0] = data_.vertexes.back();
  } else if (data_.vertexes.back() < data_.max_min_xyz[i][1]) {
    data_.max_min_xyz[i][1] = data_.vertexes.back();
  }
}

void ModelViewer::Clear() noexcept {
  data_.v_count = 0;
  data_.f_count = 0;
  data_.max_coord = 0.0;
  for (int i = 0; i < 3; i++) {
    data_.max_min_xyz[i][0] = 0;
    data_.max_min_xyz[i][1] = 0;
  }
  data_.facets.clear();
  data_.vertexes.clear();
}

void ModelViewer::Scale(double X, double Y, double Z) {
  for (size_t i = 0; i < data_.vertexes.size(); i += 3) {
    data_.vertexes[i] *= X;
    data_.vertexes[i + 1] *= Y;
    data_.vertexes[i + 2] *= Z;
  }
}

void ModelViewer::MoveX(double a) {
  for (size_t i = 0; i < data_.vertexes.size(); i += 3) data_.vertexes[i] += a;
}

void ModelViewer::MoveY(double a) {
  for (size_t i = 0; i < data_.vertexes.size(); i += 3)
    data_.vertexes[i + 1] += a;
}

void ModelViewer::MoveZ(double a) {
  for (size_t i = 0; i < data_.vertexes.size(); i += 3)
    data_.vertexes[i + 2] += a;
}

void ModelViewer::RotationByOX(double angle) {
  for (size_t i = 0; i < data_.vertexes.size(); i += 3) {
    double y_0 = data_.vertexes[i + 1];
    double z_0 = data_.vertexes[i + 2];
    data_.vertexes[i + 1] = std::cos(angle) * y_0 - std::sin(angle) * z_0;
    data_.vertexes[i + 2] = std::sin(angle) * y_0 + std::cos(angle) * z_0;
  }
}

void ModelViewer::RotationByOY(double angle) {
  for (size_t i = 0; i < data_.vertexes.size(); i += 3) {
    double x_0 = data_.vertexes[i];
    double z_0 = data_.vertexes[i + 2];
    data_.vertexes[i] = std::sin(angle) * z_0 + std::cos(angle) * x_0;
    data_.vertexes[i + 2] = std::cos(angle) * z_0 - std::sin(angle) * x_0;
  }
}

void ModelViewer::RotationByOZ(double angle) {
  for (size_t i = 0; i < data_.vertexes.size(); i += 3) {
    double x_0 = data_.vertexes[i];
    double y_0 = data_.vertexes[i + 1];
    data_.vertexes[i] = std::cos(angle) * x_0 - std::sin(angle) * y_0;
    data_.vertexes[i + 1] = std::sin(angle) * x_0 + std::cos(angle) * y_0;
  }
}

}  // namespace s21
