#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

using namespace QtDataVisualization;

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

  connect(m_kMeansDialog, &KMeansDialog::pointSizeUpdated,
          this, &MainWindow::updatePointSize);

  connect(m_kMeansDialog, &KMeansDialog::stepPrevUpdated,
          this, &MainWindow::getPrevStep);

  connect(m_kMeansDialog, &KMeansDialog::importFileSelected,
          this, &MainWindow::importPoints);

  //k_means m(20, 3, FLOAT_MIN, FLOAT_MAX);

  qDebug() << "Clustering started !";

  //m.clusterPoints(5);

  qDebug() << "Clustering ended !";

  //ui->ndViewWidget->paintPoints();

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
void MainWindow::generatePoints(int noOfPoints, float min, float max, int dim)
{
  if (!m_k_means.getAllPoints().empty()){
    QMessageBox msgBox;
    msgBox.setText("Points are already generated, open a new instance!");
    msgBox.exec();
  } else {
    m_k_means.setNoOfPoints(noOfPoints);
    m_k_means.setDimension(dim);
    initialization in;
    // Try using generateRandomPointsND
    if (dim == 2) {
      in.generateRandomPoints(min, max, m_k_means);
      ui->chartViewWidget->paintPoints(m_k_means.getAllPoints());
      m_kMeansDialog->updatePointInfoLabel("Points Generated");
    } else if (dim  == 3) {
      in.generateRandomPointsND(min, max, m_k_means);
      ui->ndViewWidget->addPoints(&m_k_means);
      m_kMeansDialog->updatePointInfoLabel("Points Generated");
      m_graph = new Q3DScatter();
      if (!m_graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
      } else {
        qDebug() << " Scatter graph is created ";
      }
      ui->scatter3DWidget->createContainer(*m_graph);
      ui->scatter3DWidget->paintPoints(m_k_means);
    }

  }
}

/**
 * @brief MainWindow::clusterPoints
 * @param k
 * @param metric
 * @param iter
 * Cluster points using given parameters
 */
void MainWindow::clusterPoints()
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
      back_clicked = false;
      m_k_means.clusterPoints(m_k_means.getNumOfIter());
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
      back_clicked = false;
      m_k_means.moveOneStep();
      // If this is the first step, assign prev clusters before update
      ui->chartViewWidget->getNextStep(m_k_means);
      m_k_means.finalizeOneStep();
      m_step += 1;
      m_kMeansDialog->updateIterationStepLabel(m_step);
    }
  }
}

void MainWindow::initializeClustering(int k, QString metric, int iter, QString initMethod)
{
  if (m_k_means.getAllPoints().empty() && m_k_means.getAllPointsND().empty()){
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
      bool flag = (m_k_means.getDimension()>=3);
      if (flag) {
        if (initMethod == "Random Sample") {
          in.initRandomSampleND(m_k_means);
        } else if (initMethod == "Random Real") {
          in.initRandomRealND(m_k_means);
        } else { // kmeans++
          in.initKMeansPpND(m_k_means);
        }
        //qDebug() << "clusters: " << m_k_means.getClustersND();
        //qDebug() << "clusters: " << m_k_means.getClustersND().at(0)->center;
        m_k_means.setInitialized(true);
        ui->scatter3DWidget->paintCenters(m_k_means);
      } else {
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
}

void MainWindow::updatePointSize(int pointSize)
{
  if (m_k_means.getAllPoints().isEmpty()) {
    QMessageBox msgBox;
    msgBox.setText("Generate points first!");
    msgBox.exec();
  } else {
    ui->chartViewWidget->updatePointSize(pointSize);
  }
}

void MainWindow::getPrevStep()
{
  QMessageBox msgBox;
  msgBox.setText("Not working properly yet!");
  msgBox.exec();
//  qDebug() << "getPrevStep in main window";
//  if (!m_k_means.isInitialized()){
//    QMessageBox msgBox;
//    msgBox.setText("Initialize clustering first!");
//    msgBox.exec();
//  }
//  else {
//    if (m_step == 0){
//      QMessageBox msgBox;
//      msgBox.setText("Clustering has not started yet, cannot go back!");
//      msgBox.exec();
//    } else if (m_step == 1){
//      QMessageBox msgBox;
//      msgBox.setText("Only one step is done, cannot go back!");
//      msgBox.exec();
//    } else{
//      if (!back_clicked){
//        //m_k_means.getPrevClusters();
//        back_clicked = true;
//        m_step -= 1;
//        m_k_means.setStep(m_step);
//        m_kMeansDialog->updateIterationStepLabel(m_step);
//        m_k_means.setClusterCentersToPrev();
//        m_k_means.moveOneStep();
//        ui->chartViewWidget->getPrevStep(m_k_means);
//      }else {
//        QMessageBox msgBox;
//        msgBox.setText("Already moved one back, cannot go more!");
//        msgBox.exec();
//      }
//    }
//  }
}

void MainWindow::zoomIn()
{
  qDebug() << "Zoomed in";
  ui->chartViewWidget->zoomIn();
}

void MainWindow::zoomOut()
{
  qDebug() << "Zoomed out";
  ui->chartViewWidget->zoomOut();
}

void MainWindow::zoomActualSize()
{
  qDebug() << "Zoom actual size";
  ui->chartViewWidget->zoomActualSize();
}

void MainWindow::importPoints()
{
  if (!m_k_means.getAllPoints().empty()){
    QMessageBox msgBox;
    msgBox.setText("Points are already generated, open a new instance!");
    msgBox.exec();
    return;
  } else {
    QString fileName = QFileDialog::getOpenFileName(this,
                    tr("Import points"), "./", tr("Text Files (*.txt)"));
    if (fileName.isEmpty()) {
      // No file was selected
      QMessageBox msgBox;
      msgBox.setText("No file selected!");
      msgBox.exec();
    } else {
      // Read the file with fileName and parse point
      QFile file(fileName);

      if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText("Problem with file read!");
        msgBox.exec();
        return;
      }
      QTextStream in(&file);
      qDebug() << " File is read, parsing";
      // Read first two lines
      // First line is numOfPoints
      QString line = in.readLine();
      int numOfPoints = line.toInt();
      m_k_means.setNoOfPoints(numOfPoints);
      qDebug() << " Num of Points " << numOfPoints;

      // Second line is dimension
      line = in.readLine();
      int dimension = line.toInt();
      m_k_means.setDimension(dimension);
      qDebug() << " Dimension " << dimension;

      // Find min and max just to update the spinboxes in ui
      float min = FLT_MAX;
      float max = FLT_MIN;

      // Rest: each line is a point vector in given dimension
      while (!in.atEnd()) {
        QVector<float> * p = new QVector<float>();
        line = in.readLine();
        // parse line by splitting for values in each dimension of points
        QStringList point = line.split(" ");
        for (QString s : point){
          p->append(s.toFloat());
          if (s.toFloat() < min) {
            min = s.toFloat();
          }
          if (s.toFloat() > max){
            max = s.toFloat();
          }
        }
        if (dimension == 2) {
          QVector2D *v = new QVector2D();
          v->setX(p->at(0));
          v->setY(p->at(1));
          m_k_means.addPoint(v);
        } else {
          m_k_means.addPointND(p);
        }
      }
      if (dimension == 2) {
        ui->chartViewWidget->paintPoints(m_k_means.getAllPoints());
      } else if (dimension == 3) {
        m_graph = new Q3DScatter();
        ui->scatter3DWidget->createContainer(*m_graph);
        ui->scatter3DWidget->paintPoints(m_k_means);
      }
      m_kMeansDialog->updatePointInfoLabel("Points Imported");
      m_kMeansDialog->updateImportedPointParameters(numOfPoints, dimension,
                                                         min, max);
    }
  }
}

