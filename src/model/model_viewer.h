#ifndef CPP4_3DVIEWER_V2_0_2_MODEL_H_
#define CPP4_3DVIEWER_V2_0_2_MODEL_H_

#include <cmath>
#include <fstream>
#include <string>
#include <vector>

namespace s21 {

struct Data {
  int v_count{};
  int f_count{};
  double max_coord{};
  double max_min_xyz[3][2]{};
  std::vector<float> vertexes{};
  std::vector<unsigned int> facets{};
};

class ModelViewer {
 public:
  ModelViewer() = default;
  ~ModelViewer() = default;
  void Parser(const std::string &path);
  Data &GetData() { return data_; }
  void MoveX(double a);
  void MoveY(double a);
  void MoveZ(double a);
  void RotationByOX(double angle);
  void RotationByOY(double angle);
  void RotationByOZ(double angle);
  void Scale(double X, double Y, double Z);

 private:
  Data data_;
  void ParserLine(const std::string &str, std::vector<std::string> &elements);
  void PushFacets(std::vector<std::string> &elements);
  void Clear() noexcept;
  void MaxMin(Data &data_, const size_t &i);
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_2_MODEL_H_
