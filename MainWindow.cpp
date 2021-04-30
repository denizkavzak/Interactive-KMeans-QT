#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

//constexpr int FLOAT_MIN = 0;
//constexpr int FLOAT_MAX = 100;

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

  //k_means m(20, 3, FLOAT_MIN, FLOAT_MAX);

  qDebug() << "Clustering started !";

  //m.clusterPoints(5);

  qDebug() << "Clustering ended !";

  // Can access clusters by using m.getClusters()

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::generatePoints(int noOfPoints, float min, float max)
{
  m_k_means.setNoOfPoints(noOfPoints);
  m_k_means.generateRandomPoints(min, max);
  ui->graphicsView->paintPoints(m_k_means.getAllPoints());
}

void MainWindow::clusterPoints(int k, QString metric, int iter)
{
  // TODO: FIX: k needs to be -1 in the loops
  m_k_means.setK(k);
  m_k_means.setMetric(metric);
  m_k_means.clusterPoints(iter);
}
