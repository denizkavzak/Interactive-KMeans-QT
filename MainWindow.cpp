#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include "initialization.h"
#include <QMessageBox>

//constexpr int FLOAT_MIN = 0;
//constexpr int FLOAT_MAX = 100;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_kMeansDialog(new KMeansDialog(this)), m_step(0)
{
  ui->setupUi(this);

  // Connect Edit > Draw Line... to show dialog
  connect(ui->actionK_Means, &QAction::triggered, m_kMeansDialog,
          &KMeansDialog::show);

  connect(m_kMeansDialog, &KMeansDialog::pointsParametersChanged,
          this, &MainWindow::generatePoints);

  connect(m_kMeansDialog, &KMeansDialog::clusteringParametersChanged,
          this, &MainWindow::clusterPoints);

  connect(m_kMeansDialog, &KMeansDialog::stepUpdated,
          this, &MainWindow::getNextStep);

  connect(m_kMeansDialog, &KMeansDialog::initializationSelected,
          this, &MainWindow::initializeClustering);

  //  connect(m_kMeansDialog, &KMeansDialog::clusteringParametersChanged,
  //          this, &MainWindow::getNextStep);

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
  if (!m_k_means.getAllPoints().empty()){
    QMessageBox msgBox;
    msgBox.setText("Points are already generated, open a new instance!");
    msgBox.exec();
  } else {
    m_k_means.setNoOfPoints(noOfPoints);
    m_k_means.generateRandomPoints(min, max);
    ui->chartViewWidget->paintPoints(m_k_means.getAllPoints());
  }
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
  if (!m_k_means.isInitialized()) {
    QMessageBox msgBox;
    msgBox.setText("Initialize clustering first!");
    msgBox.exec();
  } else {
    if (m_step == m_k_means.getNumOfIter()){
      QMessageBox msgBox;
      msgBox.setText("Clustering is finalized!");
      msgBox.exec();
    } else {
      m_k_means.setNumOfIter(iter);
      m_k_means.setK(k);
      m_k_means.setMetric(metric);
      m_k_means.clusterPoints(iter);
      m_step = m_k_means.getNumOfIter();
      m_kMeansDialog->updateIterationStepLabel(m_step);
      ui->chartViewWidget->paintClusters(m_k_means);
      ui->chartViewWidget->update();
    }
  }
}

void MainWindow::getNextStep()
{
  qDebug() << "getNextStep in mainwindow";
  if (!m_k_means.isInitialized()){
    QMessageBox msgBox;
    msgBox.setText("Initialize clustering first!");
    msgBox.exec();
  }
  else {
    if (m_step == m_k_means.getNumOfIter()){
      QMessageBox msgBox;
      msgBox.setText("Clustering is finalized!");
      msgBox.exec();
    } else{
      m_k_means.moveOneStep();
      m_step += 1;
      m_kMeansDialog->updateIterationStepLabel(m_step);
      ui->chartViewWidget->getNextStep(m_k_means);
    }
  }
}

void MainWindow::initializeClustering(int k, QString metric, int iter, QString initMethod)
{
  if (m_k_means.getAllPoints().empty()){
    QMessageBox msgBox;
    msgBox.setText("Generate points first!");
    msgBox.exec();
  } else {

    if (m_k_means.isInitialized()) {
      QMessageBox msgBox;
      msgBox.setText("Clustering is already initialized!");
      msgBox.exec();
    }else {
      initialization in;
      m_k_means.setNumOfIter(iter);
      m_k_means.setK(k);
      m_k_means.setMetric(metric);

      if (initMethod == "Random Sample") {
        in.initRandomSample(m_k_means);
      } else if (initMethod == "Random Real") {
        in.initRandomReal(m_k_means);
      } else { // kmeans++
        in.initKMeansPp(m_k_means);
      }
      m_k_means.setInitialized(true);
      ui->chartViewWidget->paintCenters(m_k_means);
      ui->chartViewWidget->update();
    }
  }
}

void MainWindow::zoomIn()
{
  qDebug() << "Zoomed in";
  ui->chartViewWidget->zoomIn();
  //setZoom(ui->chartViewWidget->zoom() * 2);
}

void MainWindow::zoomOut()
{
  qDebug() << "Zoomed out";
  ui->chartViewWidget->zoomOut();
  //setZoom(ui->chartViewWidget->zoom() * 0.5f);
}

void MainWindow::zoomActualSize()
{
  qDebug() << "Zoom actual size";
  ui->chartViewWidget->zoomActualSize();
  //setZoom(1);
}

