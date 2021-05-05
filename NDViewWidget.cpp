#include "NDViewWidget.h"
#include "ui_NDViewWidget.h"
#include <QtDataVisualization/Q3DScatter>
#include <QMessageBox>
#include <QtMath>
#include <QTimer>
#include <chrono>
#include <random>
#include <k_means.h>
#include <initialization.h>

using namespace QtDataVisualization;

NDViewWidget::NDViewWidget(QWidget *parent) :
  QOpenGLWidget(parent),
  ui(new Ui::NDViewWidget)
{
  ui->setupUi(this);

  initialization in;
//  k_means* k_m = new k_means();
//  in.generateRandomPointsND(0, 1, *k_m);

//  for (int i = 0; i<  k_m->getAllPointsND().size(); i++) {
//     QVector<float> *p = k_m->getAllPointsND().at(i);
//     m_points.append({p->at(0), p->at(1), p->at(2)});
//     //qDebug() << "added point 1" << *p;

//     // Re-map positions to [0, 1] and use as color
//     float r = (p->at(0) + 1.0f)/2.0;
//     float g = (p->at(1) + 1.0f)/2.0;
//     float b = (p->at(2) + 1.0f)/2.0;

//     m_colors.append({r, g, b});
//  }
}

NDViewWidget::~NDViewWidget()
{
  delete ui;
}

void NDViewWidget::setPoints(QVector<float> points)
{
  m_points = points;
}

void NDViewWidget::addPoints(k_means* k_m)
{
  m_kmeans = k_m;
  for (QVector<float>* p : k_m->getAllPointsND()) {
    m_points += {p->at(0), p->at(1), p->at(2)};
  }
}

void NDViewWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(0.25, 0.25, 0.25, 1.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_POINT_SMOOTH);

  m_pointProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
    "attribute highp vec4 vertex;\n"
    "attribute mediump vec4 color;\n"
    "varying mediump vec4 vColor;\n"
    "uniform highp mat4 matrix;\n"
    "void main(void)\n"
    "{\n"
    "   gl_Position = matrix * vertex;\n"
    //" vcolor = (vertex + vec4(1.0))/2 "
    "   vColor = color;\n"
    "   gl_PointSize = 8.0;\n"
    "}");

  m_pointProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
    "varying mediump vec4 vColor;\n"
    "void main(void)\n"
    "{\n"
    "   gl_FragColor = vColor;\n"
    "}");

  m_pointProgram.link();
}

void NDViewWidget::paintGL()
{
  QOpenGLShaderProgram program;
  program.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                  "attribute highp vec4 vertex;\n"
                                  "uniform highp mat4 matrix;\n"
                                  "void main(void)\n"
                                  "{\n"
                                  "   gl_Position = matrix * vertex;\n"
                                  "}");
  program.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                  "uniform mediump vec4 color;\n"
                                  "void main(void)\n"
                                  "{\n"
                                  "   gl_FragColor = color;\n"
                                  "}");
  program.link();
  program.bind();
  int vertexLocation = program.attributeLocation("vertex");
  int matrixLocation = program.uniformLocation("matrix");
  int colorLocation = program.uniformLocation("color");
  //  static GLfloat const triangleVertices[] = {
  //       60.0f,  10.0f,  0.0f,
  //       110.0f, 110.0f, 0.0f,
  //       10.0f,  110.0f, 0.0f
  //   };
  //   auto pentagon = createPolygon(60, 60, 0, 50, 5);
  //   QColor color(0, 255, 0, 255);
  QMatrix4x4 pmvMatrix;
  // Pushing the near clip further increases depth precision
  pmvMatrix.perspective(40.0, float(width())/height(), 0.5f, 10000.0f);
  //pmvMatrix.lookAt({60, 60, 998}, {60, 60, 0}, {0, 1, 0}); // view transformation
  pmvMatrix.lookAt({0, 0, 5}, {0, 0, 0}, {0, 1, 0});
  //pmvMatrix.rotate(m_turntableAngle, {0.0f, 1.0f, 0.0f});

  // left right bottom top nearPlane farPlane
  // pmvMatrix.ortho(rect()); // the center is upper left
  // pmvMatrix.ortho(10, 110, 110, 10, -1, 1);
  // pmvMatrix.ortho(-width()/2.0, width()/2.0, -height()/2.0,
  // height()/2.0, -1, 1); // projection

  program.setUniformValue(matrixLocation, pmvMatrix);

  m_pointProgram.bind();
  m_pointProgram.enableAttributeArray("vertex");
  m_pointProgram.enableAttributeArray("color");

  m_pointProgram.setUniformValue("matrix", pmvMatrix);
  m_pointProgram.setAttributeArray("vertex", m_points.constData(), 3);
  m_pointProgram.setAttributeArray("color", m_colors.constData(), 3);

  glDrawArrays(GL_POINTS, 0, m_points.count()/3);

  m_pointProgram.disableAttributeArray("vertex");
  m_pointProgram.disableAttributeArray("color");

  program.bind();

  program.enableAttributeArray(vertexLocation);

  program.setUniformValue(matrixLocation, pmvMatrix);

  // Draw green pentagon
//  auto pentagon = createPolygon(60, 60, 0, 50, 5);
//  program.setAttributeArray(vertexLocation, pentagon.constData(), 3);
//  program.setUniformValue(colorLocation, QColor(0, 255, 0, 255));
//  glDrawArrays(GL_POLYGON, 0, pentagon.count()/3);

//  // Draw red pentagon
//  auto octagon = createPolygon(60, 60, -0.5f, 60, 8);
//  program.setAttributeArray(vertexLocation, octagon.constData(), 3);
//  program.setUniformValue(colorLocation, QColor(255, 0, 0, 255));
//  glDrawArrays(GL_POLYGON, 0, octagon.count()/3);

  //
  // Draw axes
  //
  GLfloat const axisWidth = 8.0;
  GLfloat const axisLength = 500.0;
  GLfloat const xAxis[] = {0.0f, 0.0f, 0.0f, // Vertex 1
                           axisLength, 0.0f, 0.0f}; // Vertex 2

  glEnable(GL_LINE_SMOOTH);
  glLineWidth(axisWidth);

  program.setAttributeArray("vertex", xAxis, 3);
  program.setUniformValue("color", QColor(255, 0, 0, 255));
  glDrawArrays(GL_LINES, 0, 2);


  GLfloat const yAxis[] = {0.0f, 0.0f, 0.0f, // Vertex 1
                           0.0f, axisLength, 0.0f}; // Vertex 2

  program.setAttributeArray("vertex", yAxis, 3);
  program.setUniformValue("color", QColor(0, 255, 0, 255));
  glDrawArrays(GL_LINES, 0, 2);


  GLfloat const zAxis[] = {0.0f, 0.0f, 0.0f, // Vertex 1
                           0.0f, 0.0f, axisLength}; // Vertex 2

  glLineWidth(10.0);

  program.setAttributeArray("vertex", zAxis, 3);
  program.setUniformValue("color", QColor(0, 0, 255, 255));
  glDrawArrays(GL_LINES, 0, 2);


  program.disableAttributeArray(vertexLocation);
}
