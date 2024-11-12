#ifndef CPP4_3DVIEWER_V2_0_2_CONTROLLER_H_
#define CPP4_3DVIEWER_V2_0_2_CONTROLLER_H_

#include "../model/model_viewer.h"

namespace s21 {

class ControllerViewer {
 public:
  ControllerViewer(ModelViewer *model) : model_(model) {}
  ~ControllerViewer() = default;
  void Parser(const std::string &path) { model_->Parser(path); }
  Data GetData() { return model_->GetData(); }

  void ChangeScale(double a, double b, double c) { model_->Scale(a, b, c); }
  void ChangeX(double a) { model_->MoveX(a); }
  void ChangeY(double a) { model_->MoveY(a); }
  void ChangeZ(double a) { model_->MoveZ(a); }
  void RotateOX(double a) { model_->RotationByOX(a); }
  void RotateOY(double a) { model_->RotationByOY(a); }
  void RotateOZ(double a) { model_->RotationByOZ(a); }

 private:
  ModelViewer *model_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_2_CONTROLLER_H_
