#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "k_means.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  k_means m(100, 2);

  qDebug() << "Clustering started !";

  m.clusterPoints(5);

  qDebug() << "Clustering ended !";

  // Can access clusters by using m.getClusters()

  ui->graphicsView->paintPoints(m.getAllPoints());

}

MainWindow::~MainWindow()
{
  delete ui;
}
