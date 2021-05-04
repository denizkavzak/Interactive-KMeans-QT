#ifndef SCATTER3DWIDGET_H
#define SCATTER3DWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include "Scatter3DVis.h"

namespace Ui {
class Scatter3DWidget;
}

class Scatter3DWidget : public QWidget //QOpenGLWidget
{
  Q_OBJECT

public:
  explicit Scatter3DWidget(QWidget *parent = nullptr);
                           //Qt::WindowFlags f = Qt::WindowFlags());
  ~Scatter3DWidget();

  void paintPoints();
  void createContainer(Q3DScatter& graph);

private:
  Ui::Scatter3DWidget *ui;
  Scatter3DVis *m_scatter3Dvis;
  QWidget *m_container;
};

#endif // SCATTER3DWIDGET_H
