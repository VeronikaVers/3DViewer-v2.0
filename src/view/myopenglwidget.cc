#include "myopenglwidget.h"
// #include "mainwindow.h"

namespace s21 {

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
  obj.bac_color_ = Qt::black;
  obj.line_color_ = Qt::white;
  obj.vertex_color_ = Qt::white;
}

MyOpenGLWidget::~MyOpenGLWidget() {}

void MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void MyOpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyOpenGLWidget::paintGL() {
  glClearColor(obj.bac_color_.redF(), obj.bac_color_.greenF(),
               obj.bac_color_.blueF(), obj.bac_color_.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (obj.parallel == true) {
    ProjectionParallel();
  } else if (obj.central == true) {
    ProjectionCenter();
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(xRot, 1.0, 0.0, 0.0);
  glRotatef(yRot, 0.0, 1.0, 0.0);
  glRotatef(zRot, 0.0, 0.0, 1.0);
  PaintObj();
}

void MyOpenGLWidget::PaintObj() {
  if (!obj.data.vertexes.empty() || !obj.data.facets.empty()) {
    if (obj.dash == true) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 10);
    } else {
      glDisable(GL_LINE_STIPPLE);
    }

    if (obj.circle == true) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }

    glLineWidth(obj.line_size);
    glPointSize(obj.vertex_size);
    glVertexPointer(3, GL_FLOAT, 0, &obj.data.vertexes[0]);
    glColor3d(obj.line_color_.redF(), obj.line_color_.greenF(),
              obj.line_color_.blueF());
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawElements(GL_LINES, obj.data.f_count, GL_UNSIGNED_INT,
                   &obj.data.facets[0]);
    glColor3d(obj.vertex_color_.redF(), obj.vertex_color_.greenF(),
              obj.vertex_color_.blueF());

    if (obj.disable_points == false) {
      glDrawArrays(GL_POINTS, 0, obj.data.v_count / 3);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    update();
  }
}

void MyOpenGLWidget::ProjectionParallel() {
  glOrtho(obj.data.max_min_xyz[0][1] * 1.5f, obj.data.max_min_xyz[0][0] * 1.5f,
          obj.data.max_min_xyz[1][1] * 1.5f, obj.data.max_min_xyz[1][0] * 1.5f,
          obj.data.max_min_xyz[2][1] * 3.5f, obj.data.max_min_xyz[2][0] * 3.5f);
  glTranslatef(0, 0, -0.5);
}

void MyOpenGLWidget::ProjectionCenter() {
  float fov = 30.0 * M_PI / 180;
  float heapHeight = obj.data.max_min_xyz[1][0] / (2 * tan(fov / 2));
  glFrustum(obj.data.max_min_xyz[0][1], obj.data.max_min_xyz[0][0],
            obj.data.max_min_xyz[1][1], obj.data.max_min_xyz[1][0], heapHeight,
            obj.data.max_min_xyz[2][0] * 100);
  glTranslatef(0, 0, -heapHeight * 3);
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* mo) { isClicking = true; }

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent* mo) { isClicking = false; }

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* mo) {
  mDelta = mo->pos() - mPos;
  mPos = mo->pos();
  if (isClicking) {
    xRot += 1 / M_PI * mDelta.y();
    yRot += 1 / M_PI * mDelta.x();
  }
  update();
  mDelta = QPoint();
}

void MyOpenGLWidget::set_line_color(const QColor& color) noexcept {
  obj.line_color_ = color;
}

void MyOpenGLWidget::set_vertex_color(const QColor& color) noexcept {
  obj.vertex_color_ = color;
}

void MyOpenGLWidget::set_bac_color(const QColor& color) noexcept {
  obj.bac_color_ = color;
}

QImage MyOpenGLWidget::GetFrame() noexcept { return grabFramebuffer(); }

}  // namespace s21
