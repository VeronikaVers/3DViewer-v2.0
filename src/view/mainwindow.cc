#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(ControllerViewer *controller_viewer, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      controller_viewer_(controller_viewer) {
  ui->setupUi(this);
  setFixedSize(1100, 585);
  timer = new QTimer(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_open_file_clicked() {
  QString fileDialog = QFileDialog::getOpenFileName(
      this, "Выберите файл", QCoreApplication::applicationDirPath(),
      "Файлы OBJ (*.obj)");
  if (fileDialog != "") {
    controller_viewer_->Parser(fileDialog.toStdString());
    if (controller_viewer_->GetData().v_count > 0) {
      ui->spin_box_move_x->setValue(0.0);
      ui->spin_box_move_y->setValue(0.0);
      ui->spin_box_move_z->setValue(0.0);
      ui->spin_box_x_rot->setValue(0.0);
      ui->spin_box_y_rot->setValue(0.0);
      ui->spin_box_z_rot->setValue(0.0);
      ui->spin_box_scale->setValue(1.0);
      ui->open_gl_widget->SetObj().data = controller_viewer_->GetData();
      ui->name_file->setText(QFileInfo(fileDialog).fileName());
      ui->num_of_vertices->setText(
          QString::number(controller_viewer_->GetData().v_count / 3));
      ui->num_of_edges->setText(
          QString::number(controller_viewer_->GetData().f_count / 2));
    }
  }
}

void MainWindow::on_type_rojection_currentTextChanged(const QString &arg1) {
  if (arg1 == "central") {
    ui->open_gl_widget->SetObj().central = true;
    ui->open_gl_widget->SetObj().parallel = false;
  } else {
    ui->open_gl_widget->SetObj().central = false;
    ui->open_gl_widget->SetObj().parallel = true;
  }
}

void MainWindow::on_type_line_currentTextChanged(const QString &arg1) {
  if (arg1 == "solid") {
    ui->open_gl_widget->SetObj().dash = false;
  } else {
    ui->open_gl_widget->SetObj().dash = true;
  }
}

void MainWindow::on_type_vertex_currentTextChanged(const QString &arg1) {
  if (arg1 == "none") {
    ui->open_gl_widget->SetObj().circle = false;
    ui->open_gl_widget->SetObj().disable_points = true;
  } else if (arg1 == "circle") {
    ui->open_gl_widget->SetObj().circle = true;
    ui->open_gl_widget->SetObj().disable_points = false;
  } else {
    ui->open_gl_widget->SetObj().circle = false;
    ui->open_gl_widget->SetObj().disable_points = false;
  }
}

void MainWindow::on_spinBox_line_size_valueChanged(double arg1) {
  ui->open_gl_widget->SetObj().line_size = arg1;
}

void MainWindow::on_spinBox_vertex_size_valueChanged(double arg1) {
  if (ui->open_gl_widget->SetObj().disable_points == false) {
    ui->open_gl_widget->SetObj().vertex_size = arg1;
  }
}

void MainWindow::on_save_clicked() {
  QSettings settings("s21", "3D_viewer");
  settings.setValue("parallel", ui->open_gl_widget->GetObj().parallel);
  settings.setValue("central", ui->open_gl_widget->GetObj().central);
  settings.setValue("dash", ui->open_gl_widget->GetObj().dash);

  settings.setValue("disable_points",
                    ui->open_gl_widget->GetObj().disable_points);
  settings.setValue("circle", ui->open_gl_widget->GetObj().circle);
  settings.setValue("line_size", ui->open_gl_widget->GetObj().line_size);
  settings.setValue("vertex_size", ui->open_gl_widget->GetObj().vertex_size);
  settings.setValue("bac_color", ui->open_gl_widget->GetObj().bac_color_);
  settings.setValue("line_color", ui->open_gl_widget->GetObj().line_color_);
  settings.setValue("vertex_color", ui->open_gl_widget->GetObj().vertex_color_);
}

void MainWindow::on_load_clicked() {
  QSettings settings("s21", "3D_viewer");

  ui->open_gl_widget->SetObj().parallel =
      settings.value("parallel", true).toBool();
  ui->open_gl_widget->SetObj().central =
      settings.value("central", false).toBool();

  ui->open_gl_widget->SetObj().dash = settings.value("dash", false).toBool();
  if (ui->type_line->currentText() == "solid") {
    ui->open_gl_widget->SetObj().dash = false;
  } else {
    ui->open_gl_widget->SetObj().dash = true;
  }

  ui->open_gl_widget->SetObj().circle =
      settings.value("circle", false).toBool();
  ui->open_gl_widget->SetObj().disable_points =
      settings.value("disable_points", false).toBool();

  ui->open_gl_widget->SetObj().line_size =
      settings.value("line_size", 0.0).toDouble();
  ui->spinBox_line_size->setValue(ui->open_gl_widget->GetObj().line_size);

  ui->open_gl_widget->SetObj().vertex_size =
      settings.value("vertex_size", 0.0).toDouble();
  ui->spinBox_vertex_size->setValue(ui->open_gl_widget->GetObj().vertex_size);
}

void MainWindow::on_spin_box_scale_valueChanged(double arg1) {
  ui->spin_box_scale->setMinimum(0.1);
  double s = arg1 / ui->open_gl_widget->num_last_scale;
  controller_viewer_->ChangeScale(s, s, s);
  ui->open_gl_widget->num_last_scale = arg1;
  ui->open_gl_widget->SetObj().data = controller_viewer_->GetData();
  ui->open_gl_widget->update();
}

void MainWindow::on_spin_box_move_x_valueChanged(double x) {
  double scale_factor = x - ui->open_gl_widget->num_last_x_move;
  controller_viewer_->ChangeX(scale_factor *
                              ui->open_gl_widget->GetObj().data.max_coord);
  ui->open_gl_widget->num_last_x_move = x;
  ui->open_gl_widget->SetObj().data = controller_viewer_->GetData();
  ui->open_gl_widget->update();
}

void MainWindow::on_spin_box_move_y_valueChanged(double y) {
  double scale_factor = y - ui->open_gl_widget->num_last_y_move;
  controller_viewer_->ChangeY(scale_factor *
                              ui->open_gl_widget->GetObj().data.max_coord);
  ui->open_gl_widget->num_last_y_move = y;
  ui->open_gl_widget->SetObj().data = controller_viewer_->GetData();
  ui->open_gl_widget->update();
}

void MainWindow::on_spin_box_move_z_valueChanged(double z) {
  double scale_factor = z - ui->open_gl_widget->num_last_z_move;
  controller_viewer_->ChangeZ(scale_factor *
                              ui->open_gl_widget->GetObj().data.max_coord);
  ui->open_gl_widget->num_last_z_move = z;
  ui->open_gl_widget->SetObj().data = controller_viewer_->GetData();
  ui->open_gl_widget->update();
}

void MainWindow::on_spin_box_x_rot_valueChanged(int arg1) {
  controller_viewer_->RotateOX(arg1 - ui->open_gl_widget->num_last_x_rot);
  ui->open_gl_widget->num_last_x_rot = arg1;
  QObject::connect(ui->spin_box_x_rot, SIGNAL(valueChanged(int)),
                   ui->dial_x_rot, SLOT(setValue(int)));
  ui->open_gl_widget->update();
}

void MainWindow::on_dial_x_rot_valueChanged(int value) {
  ui->open_gl_widget->xRot = value;
  ui->dial_x_rot->setSliderPosition(value);
  QObject::connect(ui->dial_x_rot, SIGNAL(valueChanged(int)),
                   ui->spin_box_x_rot, SLOT(setValue(int)));
}

void MainWindow::on_spin_box_y_rot_valueChanged(int arg1) {
  controller_viewer_->RotateOY(arg1 - ui->open_gl_widget->num_last_y_rot);
  ui->open_gl_widget->num_last_y_rot = arg1;
  QObject::connect(ui->spin_box_y_rot, SIGNAL(valueChanged(int)),
                   ui->dial_y_rot, SLOT(setValue(int)));
  update();
}

void MainWindow::on_dial_y_rot_valueChanged(int value) {
  ui->open_gl_widget->yRot = value;
  ui->dial_y_rot->setSliderPosition(value);
  QObject::connect(ui->dial_y_rot, SIGNAL(valueChanged(int)),
                   ui->spin_box_y_rot, SLOT(setValue(int)));
}

void MainWindow::on_spin_box_z_rot_valueChanged(int arg1) {
  controller_viewer_->RotateOZ(arg1 - ui->open_gl_widget->num_last_z_rot);
  ui->open_gl_widget->num_last_z_rot = arg1;
  QObject::connect(ui->spin_box_z_rot, SIGNAL(valueChanged(int)),
                   ui->dial_z_rot, SLOT(setValue(int)));
  ui->open_gl_widget->update();
}

void MainWindow::on_dial_z_rot_valueChanged(int value) {
  ui->open_gl_widget->zRot = value;
  ui->dial_z_rot->setSliderPosition(value);
  QObject::connect(ui->dial_z_rot, SIGNAL(valueChanged(int)),
                   ui->spin_box_z_rot, SLOT(setValue(int)));
}

void MainWindow::on_pushButton_color_lines_clicked() {
  QColor color_line_ =
      QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (color_line_.isValid()) {
    QString prev_style(
        "border-radius: 3px;\nborder: 1px solid rgb(110, 105, 157);\n");
    ui->pushButton_color_lines->setStyleSheet(
        QString(prev_style + "background-color: %1").arg(color_line_.name()));
    ui->open_gl_widget->set_line_color(color_line_);
    update();
  }
}

void MainWindow::on_pushButton_color_vertex_clicked() {
  QColor color_vertex_ =
      QColorDialog::getColor(Qt::red, this, "Choose a color");
  if (color_vertex_.isValid()) {
    QString prev_style(
        "border-radius: 3px;\nborder: 1px solid rgb(110, 105, 157);\n");
    ui->pushButton_color_vertex->setStyleSheet(
        QString(prev_style + "background-color: %1").arg(color_vertex_.name()));
    ui->open_gl_widget->set_vertex_color(color_vertex_);
    update();
  }
}

void MainWindow::on_pushButton_bac_color_clicked() {
  QColor color_bac_ = QColorDialog::getColor(Qt::black, this, "Choose a color");
  if (color_bac_.isValid()) {
    QString prev_style(
        "border-radius: 3px;\nborder: 1px solid rgb(110, 105, 157);\n");
    ui->pushButton_bac_color->setStyleSheet(
        QString(prev_style + "background-color: %1").arg(color_bac_.name()));
    ui->open_gl_widget->set_bac_color(color_bac_);
    update();
  }
}

void MainWindow::on_pushButton_save_image_clicked() {
  QString f_name = QFileDialog::getSaveFileName(
      this, "Save image", "", "BMP Image (*.bmp);; JPEG Image (*.jpeg)");
  QImage img = ui->open_gl_widget->GetFrame();
  img.save(f_name);
}

void MainWindow::on_pb_gif_clicked() {
  gif = new QGifImage;
  frame = new QImage;
  timer = new QTimer(this);
  time = 0;
  connect(timer, SIGNAL(timeout()), this, SLOT(SaveGIF()));
  timer->start(100);
}

void MainWindow::SaveGIF() {
  time++;
  *frame = ui->open_gl_widget->grabFramebuffer();
  gif->addFrame(*frame);
  if (time == 50) {
    timer->stop();
    gifSavePath = NULL;
    gifSavePath = QFileDialog::getSaveFileName(this, NULL, NULL, "GIF (*.gif)");
    if (!gifSavePath.isNull()) gif->save(gifSavePath);
  }
}

}  // namespace s21
