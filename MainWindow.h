#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KMeansDialog.h>
#include "k_means.h"
#include <initialization.h>
#include <limits>
#include <QtDataVisualization/Q3DScatter>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void generatePoints(int noOfPoints, float min, float max, int dim);
  void clusterPoints();
  void getNextStep();
  void initializeClustering(int k, QString, int, QString);
  void updatePointSize(int pointSize);
  void getPrevStep();
  void play(int ms_value);
  void stop();

  void zoomIn();
  void zoomOut();
  void zoomActualSize();
  void importPoints();

private:
  Ui::MainWindow *ui;
  KMeansDialog *m_kMeansDialog;
  k_means m_k_means;
  int m_step;
  bool m_back_clicked = false;
  QtDataVisualization::Q3DScatter* m_graph;
  QTimer* m_timer;
  bool m_playing = false;
};

#endif // MAINWINDOW_H
