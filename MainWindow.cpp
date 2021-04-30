#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "k_means.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_kMeansDialog(new KMeansDialog(this))
{
  ui->setupUi(this);

  // Connect Edit > Draw Line... to show dialog
  connect(ui->actionK_Means, &QAction::triggered, m_kMeansDialog,
          &KMeansDialog::show);

  connect(m_kMeansDialog, &KMeansDialog::pointsParametersChanged,
          this, &MainWindow::generatePoints);

  connect(m_kMeansDialog, &KMeansDialog::clusteringParametersChanged,
          this, &MainWindow::clusterPoints);


  k_means m(40, 4);

  qDebug() << "Clustering started !";

  m.clusterPoints(2);

  qDebug() << "Clustering ended !";

  // Can access clusters by using m.getClusters()

  ui->graphicsView->paintPoints(m.getAllPoints());

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::generatePoints(int noOfPoints, float min, float max)
{

}

void MainWindow::clusterPoints(int k, QString metric)
{

}
