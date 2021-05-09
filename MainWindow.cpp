#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

using namespace QtDataVisualization;

auto start = std::chrono::high_resolution_clock::now();
auto end = std::chrono::high_resolution_clock::now();

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

  connect(m_kMeansDialog, &KMeansDialog::playTriggered,
          this, &MainWindow::play);

  connect(m_kMeansDialog, &KMeansDialog::stopTriggered,
          this, &MainWindow::stop);

  connect(ui->scatter3DWidget, &Scatter3DWidget::clusterCenterSelected,
          this, &MainWindow::selectClusterCenter);

  //k_means m(20, 3, FLOAT_MIN, FLOAT_MAX);
  qDebug() << "Clustering started !";

  //m.clusterPoints(5);

  qDebug() << "Clustering ended !";

  m_timer = new QTimer(this);
  m_timer->callOnTimeout(this, &MainWindow::getNextStep);

  //ui->ndViewWidget->paintPoints();

  // Can access clusters by using m.getClusters()

  //m_graph->;

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
 * In cases where dimension is 2 or 3, there is visualization available
 * and shown to user. In cases where N > 3 or N < 2, no visualization
 */
void MainWindow::generatePoints(int noOfPoints, float min, float max, int dim)
{
  // Cannot generate points for N <= 0
  if (noOfPoints <= 0) {
    QMessageBox msgBox;
    msgBox.setText("Cannot generate points N <=0 !");
    msgBox.exec();
  } else {
    if (!m_k_means.getAllPoints().empty() || !m_k_means.getAllPointsND().isEmpty()){
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
        //ui->ndViewWidget->addPoints(&m_k_means);
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
      } else { // No visualization available for N > 3 or N < 2
        in.generateRandomPointsND(min, max, m_k_means);
        m_kMeansDialog->updatePointInfoLabel("Points Generated");
      }
    }
  }
}

/**
 * @brief MainWindow::clusterPoints
 * @param k
 * @param metric
 * @param iter
 * Cluster points using given parameters
 * In cases where dimension is 2 or 3, there is visualization available
 * and shown to user. In cases where N > 3 or N < 2, no visualization
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
      if (m_k_means.getDimension() == 2) {
        m_back_clicked = false;
        m_k_means.clusterPoints(m_k_means.getNumOfIter());
        m_step = m_k_means.getNumOfIter();
        m_kMeansDialog->updateIterationStepLabel(m_step);
        ui->chartViewWidget->paintClusters(m_k_means);
        ui->chartViewWidget->update();
      } else if (m_k_means.getDimension() == 3) {
        m_back_clicked = false;
        m_k_means.clusterPointsND(m_k_means.getNumOfIter());
        m_step = m_k_means.getNumOfIter();
        m_kMeansDialog->updateIterationStepLabel(m_step);
        ui->scatter3DWidget->paintClustersInit(m_k_means); // initialize series
        ui->scatter3DWidget->clearAllPointsSeriesFromGraph();
        ui->scatter3DWidget->paintClusters(m_k_means); // draw series
        ui->scatter3DWidget->paintCenters(m_k_means); // draw new centers
      } else { // No visualization available for N > 3 or N < 2
        m_back_clicked = false;
        m_k_means.clusterPointsND(m_k_means.getNumOfIter());
        m_step = m_k_means.getNumOfIter();
        m_kMeansDialog->updateIterationStepLabel(m_step);
        m_k_means.printClustersND(); // Just printing the clusters
      }

    }
  }
}

/**
 * @brief MainWindow::getNextStep
 * Runs one iteration/step of k_means clustering algorithm.
 * In cases where dimension is 2 or 3, there is visualization available
 * and shown to user. In cases where N > 3 or N < 2, no visualization
 */
void MainWindow::getNextStep()
{ 
  qDebug() << "getNextStep in mainwindow";
  if (!m_k_means.isInitialized()){
    QMessageBox msgBox;
    msgBox.setText("Initialize clustering first!");
    msgBox.exec();
  }
  else {
    if (m_step == 0) { // get time to start measuring convergence speed
      start = std::chrono::high_resolution_clock::now();
    }
    // stop animation if it is still running in last iteration
    if ((m_step == m_k_means.getNumOfIter() - 1) && m_playing) {
      m_timer->stop();
      m_playing = false;
    }

    if (m_step == m_k_means.getNumOfIter()){
      QMessageBox msgBox;
      msgBox.setText("Clustering is finalized!");
      msgBox.exec();
    } else {
      if (m_k_means.getDimension() == 2) {
        m_back_clicked = false;
        m_k_means.moveOneStep();
        // If this is the first step, assign prev clusters before update
        m_k_means.finalizeOneStep();
        ui->chartViewWidget->getNextStep(m_k_means);
        m_step += 1;
        m_kMeansDialog->updateIterationStepLabel(m_step);
      } else if (m_k_means.getDimension() == 3) {
        m_back_clicked = false;
        m_k_means.moveOneStepND();
        if (!m_k_means.isPointsSet()) { // initialize if clustering just started
        // use another flag to test if points are set to any cluster yet
          ui->scatter3DWidget->paintClustersInit(m_k_means); // initialize series
        }
        m_k_means.finalizeOneStepND();
        //m_k_means.clusterPointsND(m_k_means.getNumOfIter());
        m_k_means.printClustersND();
        m_step += 1;
        m_kMeansDialog->updateIterationStepLabel(m_step);
        //ui->scatter3DWidget->clearAllPointsSeriesFromGraph(); // do not need to call it each time
        ui->scatter3DWidget->paintClusters(m_k_means); // draw series
        ui->scatter3DWidget->paintCenters(m_k_means); // draw new centers
      } else { // No visualization available for N > 3 or N < 2
        m_k_means.moveOneStepND();
        m_k_means.finalizeOneStepND();
        m_k_means.printClustersND(); // just printing the clusters
        m_step += 1;
        m_kMeansDialog->updateIterationStepLabel(m_step);
      }
      m_k_means.detectConvergence();
      // get time to start measuring convergence speed
      if (m_converged == false && m_k_means.isConverged()) {
        end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        qDebug() << "CONVERGED TIME : " << duration.count();
        m_kMeansDialog->updateConvergenceTimeLabel(duration.count());
        m_kMeansDialog->updateConvergenceStepLabel(m_step);
        m_converged = true;
        float energy = m_k_means.calculateEnergy(m_kMeansDialog->getSelectedMetric());
        qDebug() << "CONVERGENCE TOTAL ENERGY: " << energy;
        if (m_playing) {
          m_timer->stop();
          m_playing = false;
        }
      }
    }
  }
}


/**
 * @brief MainWindow::initializeClustering
 * @param k
 * @param metric
 * @param iter
 * @param initMethod
 * Initializes the necessary parameters for clustering
 */
void MainWindow::initializeClustering(int k, QString metric, int iter, QString initMethod)
{
  if (m_k_means.getAllPoints().empty() && m_k_means.getAllPointsND().empty()) {
    QMessageBox msgBox;
    msgBox.setText("Generate points first!");
    msgBox.exec();
  } else {

    if (m_k_means.isInitialized()) {
      QMessageBox msgBox;
      msgBox.setText("Clustering is already initialized!");
      msgBox.exec();
    } else {
      initialization in;
      m_k_means.setNumOfIter(iter);
      m_k_means.setK(k);
      m_k_means.setMetric(metric);

      if (m_k_means.getK() <= m_k_means.getNumOfPoints()) {
        // Cannot run for K <= 0
        if (m_k_means.getK() <= 0) {
          QMessageBox msgBox;
          msgBox.setText("Cannot cluster with K <= 0 !");
          msgBox.exec();
        } else { // If K >= 1
          if (m_k_means.getDimension() == 2) { // For 2D case
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
            m_k_means.initClusterCentersHistory();
            qDebug() << "size of clusters nd: " << m_k_means.getClusters().size();
            // For N>=3 and N=1 case
          } else { //(m_k_means.getDimension() >= 3 || m_k_means.getDimension() == 1) {
            if (initMethod == "Random Sample") {
              in.initRandomSampleND(m_k_means);
            } else if (initMethod == "Random Real") {
              in.initRandomRealND(m_k_means);
            } else { // kmeans++
              in.initKMeansPpND(m_k_means);
            }
            m_k_means.setInitialized(true);
            if (m_k_means.getDimension() == 3) { // visualization for 3D
              ui->scatter3DWidget->paintCentersInit(m_k_means);
              ui->scatter3DWidget->paintCenters(m_k_means);
            }
            m_k_means.initClusterCentersHistoryND();
            qDebug() << "after init history";
          }
        }
      } else {
        QMessageBox msgBox;
        msgBox.setText("Cannot initialize when K > number of points!");
        msgBox.exec();
      }

    }
  }
}

/**
 * @brief MainWindow::updatePointSize
 * @param pointSize
 * Updates point size in the graph for both 2D and 3D
 * visualizations using the passed value that is taken
 * from the spin box for point size in ui dialog
 */
void MainWindow::updatePointSize(int pointSize)
{
  if (m_k_means.getAllPoints().isEmpty() && m_k_means.getAllPointsND().isEmpty()) {
    QMessageBox msgBox;
    msgBox.setText("Generate points first!");
    msgBox.exec();
  } else {
    if (m_k_means.getDimension() == 2) {
      ui->chartViewWidget->updatePointSize(pointSize);
    } else if (m_k_means.getDimension() == 3) {
      ui->scatter3DWidget->updatePointSize(pointSize);
    } else {
      QMessageBox msgBox;
      msgBox.setText("Cannot visualize dimensions other than 2 and 3!");
      msgBox.exec();
    }
  }
}

/**
 * @brief MainWindow::getPrevStep
 * Reverts the clustering to one step backwards during step by step clustering
 * TODO: FIX: Does not work properly for 2D, 3D not implemented yet
 */
void MainWindow::getPrevStep()
{
  qDebug() << "getPrevStep in main window";
  if (!m_k_means.isInitialized()) {
    QMessageBox msgBox;
    msgBox.setText("Initialize clustering first!");
    msgBox.exec();
  } else {
    if (m_step == 0) {
      QMessageBox msgBox;
      msgBox.setText("Clustering has not started yet, cannot go back!");
      msgBox.exec();
    } else if (m_step == 1 || m_step == 2) {
      QMessageBox msgBox;
      msgBox.setText("Only one step is done, cannot go back!");
      msgBox.exec();
    } else {
      if (!m_back_clicked) {
        if (m_k_means.getDimension() == 2) {
          //m_k_means.getPrevClusters();
          m_back_clicked = true;
          m_step -= 1;
          m_k_means.setStep(m_step);
          m_kMeansDialog->updateIterationStepLabel(m_step);
          //m_k_means.setClusterCentersToPrev();
          qDebug() << "step in prev step in main: " << m_step;
          m_k_means.setClusterCentersToPrevStepInHistory();
          m_k_means.setPoints();
          m_k_means.updateCenters();
          ui->chartViewWidget->getPrevStep(m_k_means);
        } else { //if (m_k_means.getDimension() >= 3) {
          m_back_clicked = true;
          m_step -= 1;
          m_k_means.setStep(m_step);
          m_kMeansDialog->updateIterationStepLabel(m_step);
          //m_k_means.setClusterCentersToPrev();
          qDebug() << "step in prev step in main: " << m_step;
          m_k_means.setClusterCentersToPrevStepInHistoryND();
          m_k_means.setPointsND();
          m_k_means.updateCentersND();
          if (m_k_means.getDimension() == 3) { // visualize
            ui->scatter3DWidget->getPrevStep(m_k_means);
          }
        }
      } else {
        QMessageBox msgBox;
        msgBox.setText("Already moved one back, cannot go more!");
        msgBox.exec();
      }
    }
  }
}

/**
 * @brief MainWindow::play
 * @param ms_value
 * Starts the timer for animation
 */
void MainWindow::play(int ms_value)
{
  qDebug() << "Play called in mainwindow";
  if (m_k_means.getAllPoints().empty() && m_k_means.getAllPointsND().empty()) {
    QMessageBox msgBox;
    msgBox.setText("Generate points first!");
    msgBox.exec();
  } else if (!m_k_means.isInitialized()) {
    QMessageBox msgBox;
    msgBox.setText("Initialize clustering first!");
    msgBox.exec();
  } else {
    m_timer->start(ms_value);
    qDebug() << "Start timer";
    m_playing = true;
  }
}

/**
 * @brief MainWindow::stop
 * Stops the timer for animation
 */
void MainWindow::stop()
{
  m_timer->stop();
}

/**
 * @brief MainWindow::zoomIn
 * Zoom in for 2D view
 */
void MainWindow::zoomIn()
{
  qDebug() << "Zoomed in";
  ui->chartViewWidget->zoomIn();
}

/**
 * @brief MainWindow::zoomOut
 * Zoom out for 2D view
 */
void MainWindow::zoomOut()
{
  qDebug() << "Zoomed out";
  ui->chartViewWidget->zoomOut();
}

/**
 * @brief MainWindow::zoomActualSize
 * Zoom to actual size for 2D view
 */
void MainWindow::zoomActualSize()
{
  qDebug() << "Zoom actual size";
  ui->chartViewWidget->zoomActualSize();
}

/**
 * @brief MainWindow::importPoints
 * Imports points from a file by prompting user to select a file
 * from a file selector message box. First line of the file
 * denotes the number of points, second line denotes the dimension
 * of the points, each of the rest of the lines are the coordinates
 * of point vectors.
 */
void MainWindow::importPoints()
{
  if (!m_k_means.getAllPoints().empty() || !m_k_means.getAllPointsND().isEmpty()){
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
        } else { //if ((dimension >= 3 || dimension == 1)) {
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

/**
 * @brief MainWindow::selectClusterCenter
 * Slot that is triggered by the clusterCenterSelected signal in
 * Scatter3DWidget. This slot initializes parameters for the
 * initialization for clustering algorithm when it is first called.
 * Calls the setClusterCenter function from Scatter3DWidget instance
 * to add the selected point to the m_k_means attribute during manual
 * cluster center selection process
 */
void MainWindow::selectClusterCenter()
{
  qDebug() << " select cluster center in mainwindow ";
  if (ui->scatter3DWidget->getManualInitCount() == 0) {
    setParamsForManualInit();
  }
  ui->scatter3DWidget->setClusterCenter(m_k_means,
                                        ui->scatter3DWidget->getSelectedPointID());
}

/**
 * @brief MainWindow::setParamsForManualInit
 * Sets the initialization parameters of the m_k_means attribute
 * by taking the necessary values from the dialog ui
 */
void MainWindow::setParamsForManualInit()
{
  int k;
  QString metric;
  int iter;
  m_kMeansDialog->getInitializationParameters(k, metric, iter);
  m_k_means.setNumOfIter(iter);
  m_k_means.setK(k);
  m_k_means.setMetric(metric);
}

