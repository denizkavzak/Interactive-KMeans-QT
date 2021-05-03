#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KMeansDialog.h>
#include "k_means.h"
#include <initialization.h>

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

  void zoomIn();
  void zoomOut();
  void zoomActualSize();

private:
  Ui::MainWindow *ui;
  KMeansDialog *m_kMeansDialog;
  k_means m_k_means;
  int m_step;
  bool back_clicked = false;
};

#endif // MAINWINDOW_H
