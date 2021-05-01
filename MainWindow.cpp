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

/**
 * @brief MainWindow::generatePoints
 * @param noOfPoints
 * @param min
 * @param max
 * Generate points using given parameters
 */
void MainWindow::generatePoints(int noOfPoints, float min, float max)
{
  m_k_means.setNoOfPoints(noOfPoints);
  m_k_means.generateRandomPoints(min, max);
  //ui->chartViewWidget->chartView()->paintPoints(m_k_means.getAllPoints());
}

/**
 * @brief MainWindow::clusterPoints
 * @param k
 * @param metric
 * @param iter
 * Cluster points using given parameters
 */
void MainWindow::clusterPoints(int k, QString metric, int iter)
{
  m_k_means.setK(k);
  m_k_means.setMetric(metric);
  m_k_means.clusterPoints(iter);
 // ui->chartViewWidget->chartView()->paintClusters(m_k_means);
}

void MainWindow::zoomIn()
{
  qDebug() << "Zoomed in";
  setZoom(ui->chartViewWidget->zoom() * 2);
}

void MainWindow::zoomOut()
{
  qDebug() << "Zoomed out";
  setZoom(ui->chartViewWidget->zoom() * 0.5f);
}

void MainWindow::zoomActualSize()
{
  qDebug() << "Zoom actual size";
  setZoom(1);
}

void MainWindow::setZoom(int zoom)
{
 // ui->chartViewWidget->setZoom(zoom);

  //ui->actionActual_Size->setEnabled(ui->pixelWidget->zoom() != 1);
  //ui->actionZoom_Out->setEnabled(ui->pixelWidget->zoom() != 1);
}
