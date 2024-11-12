#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::ModelViewer model;
  s21::ControllerViewer controller(&model);
  s21::MainWindow w(&controller);
  w.show();
  return a.exec();
}
