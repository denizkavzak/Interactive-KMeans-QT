#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KMeansDialog.h>
#include "k_means.h"

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
  void generatePoints(int noOfPoints, float min, float max);
  void clusterPoints(int k, QString metric, int iter);

  void zoomIn();
  void zoomOut();
  void zoomActualSize();

private:
  Ui::MainWindow *ui;
  KMeansDialog *m_kMeansDialog;
  k_means m_k_means;
};

#endif // MAINWINDOW_H
