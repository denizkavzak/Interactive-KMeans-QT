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

  void createContainer(Q3DScatter& graph);
  void paintPoints(k_means &k_m);
  void paintCenters(k_means &k_m);
  void paintClusters(k_means &k_m);
  void paintClustersInit(k_means &k_m);
  void paintCentersInit(k_means &k_m);
  void clearAllPointsSeriesFromGraph();
  void updatePointSize(int pointSize);
  void getPrevStep(k_means &k_m);
  int getSelectedPointID();
  int getManualInitCount();

public slots:
  void selectClusterCenter();
  void setClusterCenter(k_means &k_m, int ind);

signals:
  void clusterCenterSelected();

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void keyPressEvent(QKeyEvent *event) override;

private:
  Ui::Scatter3DWidget *ui;
  Scatter3DVis *m_scatter3Dvis;
  QWidget *m_container;
  int m_selectedPointID;
  QPointF m_lastMousePos;

  bool m_initialized = false;
  int m_manualInitCount = 0;
  //bool selected = false;
};

#endif // SCATTER3DWIDGET_H
