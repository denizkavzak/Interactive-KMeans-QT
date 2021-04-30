#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KMeansDialog.h>

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
  void clusterPoints(int k, QString metric);

private:
  Ui::MainWindow *ui;

  KMeansDialog *m_kMeansDialog;
};

#endif // MAINWINDOW_H
