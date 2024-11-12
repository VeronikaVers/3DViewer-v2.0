#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myopenglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(ControllerViewer *controller_viewer,
                      QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_open_file_clicked();

  void on_type_rojection_currentTextChanged(const QString &arg1);

  void on_type_line_currentTextChanged(const QString &arg1);
  void on_type_vertex_currentTextChanged(const QString &arg1);

  void on_spinBox_line_size_valueChanged(double arg1);
  void on_spinBox_vertex_size_valueChanged(double arg1);

  void on_pushButton_color_lines_clicked();
  void on_pushButton_color_vertex_clicked();
  void on_pushButton_bac_color_clicked();

  void on_save_clicked();
  void on_load_clicked();

  void on_spin_box_scale_valueChanged(double arg1);
  void on_spin_box_move_x_valueChanged(double с);
  void on_spin_box_move_y_valueChanged(double arg1);
  void on_spin_box_move_z_valueChanged(double z);
  void on_spin_box_x_rot_valueChanged(int arg1);
  void on_dial_x_rot_valueChanged(int value);
  void on_spin_box_y_rot_valueChanged(int arg1);
  void on_dial_y_rot_valueChanged(int value);
  void on_spin_box_z_rot_valueChanged(int arg1);
  void on_dial_z_rot_valueChanged(int value);

  void on_pushButton_save_image_clicked();
  void on_pb_gif_clicked();
  void SaveGIF();

 private:
  Ui::MainWindow *ui;
  ControllerViewer *controller_viewer_;

  QGifImage *gif;
  QImage *frame;
  QTimer *timer;
  QString gifSavePath;
  int time;
};

}  // namespace s21

#endif  // MAINWINDOW_H
