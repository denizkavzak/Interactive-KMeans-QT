#ifndef NDVIEWWIDGET_H
#define NDVIEWWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "k_means.h"

namespace Ui {
class NDViewWidget;
}

class NDViewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  explicit NDViewWidget(QWidget *parent = nullptr);
  ~NDViewWidget();

  void setPoints(QVector<float> points);
protected:
  void initializeGL() override;
  void paintGL() override;

public slots:
  void addPoints(k_means* k_m);

private:
  Ui::NDViewWidget *ui;
  QVector<float> m_points;
  QVector<float> m_colors;

  QOpenGLShaderProgram m_pointProgram;
  k_means* m_kmeans;

};

#endif // NDVIEWWIDGET_H
