#include "Scatter3DWidget.h"
#include "ui_Scatter3DWidget.h"
#include <QtDataVisualization/Q3DScatter>
#include <QMessageBox>
#include "Scatter3DVis.h"
#include <QDebug>
#include <QSize>
#include <QScreen>

Scatter3DWidget::Scatter3DWidget(QWidget *parent) : //, Qt::WindowFlags f) :
  QWidget(parent),
  //QOpenGLWidget(parent, f),
  ui(new Ui::Scatter3DWidget)
{
  ui->setupUi(this);
}

Scatter3DWidget::~Scatter3DWidget()
{
  delete ui;
}

/**
 * @brief Scatter3DWidget::paintPoints
 * @param k_m
 * Calls the addData function from 3DVis class
 * to add all the generated/imported points
 * to the graph using the index 0 in series of
 * the graph
 */
void Scatter3DWidget::paintPoints(k_means &k_m)
{
  qDebug() << "inside scatter3Dwidget paint points";
  m_scatter3Dvis->addData(k_m);
}

void Scatter3DWidget::createContainer()
{
  m_graph = new Q3DScatter();

  if (!m_graph->hasContext()) {
    QMessageBox msgBox;
    msgBox.setText("Couldn't initialize the OpenGL context.");
    msgBox.exec();
    return;
  } else {
    qDebug() << " Scatter graph is created ";
  }

  m_container = QWidget::createWindowContainer(m_graph);

  QSize screenSize = m_graph->screen()->size();
  m_container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
  m_container->setMaximumSize(screenSize);
  m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_container->setFocusPolicy(Qt::StrongFocus);

  QWidget *widget = new QWidget;
  QHBoxLayout *hLayout = new QHBoxLayout(widget);
  QVBoxLayout *vLayout = new QVBoxLayout();
  hLayout->addWidget(m_container, 1);
  hLayout->addLayout(vLayout);

  m_scatter3Dvis = new Scatter3DVis(m_graph);

  widget->show();
}

/**
 * @brief Scatter3DWidget::paintCenters
 * @param k_m
 * Draw centers with current color and location
 */
void Scatter3DWidget::paintCenters(k_means &k_m)
{
  m_scatter3Dvis->addDataCenters(k_m);
}

/**
 * @brief Scatter3DWidget::paintClusters
 * @param k_m
 * Draw actual points with their current cluster color
 */
void Scatter3DWidget::paintClusters(k_means &k_m)
{
  m_scatter3Dvis->updateSeriesForEachCluster(k_m);
}

/**
 * @brief Scatter3DWidget::paintClustersInit
 * @param k_m
 * Need to add the series for points first in the
 * first clustering iteration.
 */
void Scatter3DWidget::paintClustersInit(k_means &k_m)
{
  m_scatter3Dvis->addSeriesForEachClusterPoints(k_m);
}

/**
 * @brief Scatter3DWidget::paintCentersInit
 * @param k_m
 * Calls the corresponding init centers function from
 * the 3DVis class to add k number of series to
 * the graph, which will be used to store the
 * cluster centers.
 */
void Scatter3DWidget::paintCentersInit(k_means &k_m)
{
  m_scatter3Dvis->addSeriesForEachCluster(k_m);
}

/**
 * @brief Scatter3DWidget::clearAllPointsSeriesFromGraph
 * Calls the corresponding function from the 3DVis class
 * to clear all generated/imported points from the
 * graph, which are being held in series index 0.
 */
void Scatter3DWidget::clearAllPointsSeriesFromGraph()
{
  m_scatter3Dvis->clearAllPointsSeriesFromGraph();
}

/**
 * @brief Scatter3DWidget::updatePointSize
 * @param pointSize
 * Function calls point update function from the 3DVis class
 * to update the point sizes in the graph using passed
 * pointSize parameter
 */
void Scatter3DWidget::updatePointSize(int pointSize)
{
  m_scatter3Dvis->updatePointSize(pointSize);
}

/**
 * @brief Scatter3DWidget::getPrevStep
 * @param k_m
 * Being called when previous step of clustering in iteration
 * is triggered. Simply paints the clusters and centers
 * of current step in the passed k_means instance
 */
void Scatter3DWidget::getPrevStep(k_means &k_m)
{
  paintClusters(k_m);
  paintCenters(k_m);
}

/**
 * @brief Scatter3DWidget::getSelectedPointID
 * @return selected point ID
 * Returns the selected point ID from the 3DScatter
 * which indicates the index in the series at
 * 0 index of the graph
 */
int Scatter3DWidget::getSelectedPointID()
{
  return m_selectedPointID;
}

/**
 * @brief Scatter3DWidget::getManualInitCount
 * @return manual init count
 * Returns the manual init count which keeps track of
 * the manually selected cluster centers.
 */
int Scatter3DWidget::getManualInitCount()
{
  return m_manualInitCount;
}

/**
 * @brief Scatter3DWidget::selectClusterCenter
 * Slot to trigger clusterCenterSelected signal
 * which is connected to MainWindow slot
 * selectClusterCenter. This is called when
 * the overriden left mouse click is triggered.
 */
void Scatter3DWidget::selectClusterCenter()
{
  qDebug() << "select cluster center scatter3dwidget" ;
  emit clusterCenterSelected();
}

/**
 * @brief Scatter3DWidget::setClusterCenter
 * @param k_m
 * @param ind
 * Adds a cluster with center determined by the passed index.
 * The parameter ind gives the index of the selected point
 * in all points series (series 0). Necessary initializations
 * are made when m_manualInitCount is 0. An error message
 * is popped up if all the cluster centers are already selected.
 */
void Scatter3DWidget::setClusterCenter(k_means &k_m, int ind)
{
  qDebug() << "set cluster center scatter3dwidget";
  if (m_manualInitCount != k_m.getK()) {
    k_means::ClusterColor* color = new k_means::ClusterColor();
    k_means::ClusterND *cluster = new k_means::ClusterND();
    // Series index 0 consists of all points
    QScatter3DSeries* series = m_scatter3Dvis->getGraph()->seriesList().at(0);
    // Use selected index of point in the series 0 to assign as cluster center
    QScatterDataItem dataItem = series->dataProxy()->array()->at(ind);

    QVector<float>* point = new QVector<float>();

    point->append(dataItem.x());
    point->append(dataItem.y());
    point->append(dataItem.z());
    qDebug() << m_manualInitCount;
    cluster->center = *point;
    qDebug() << "Selected center :" << cluster->center;
    cluster->color = color->operator()(m_manualInitCount);
    k_m.addClusterND(cluster);
    if (m_manualInitCount == 0) {
      paintCentersInit(k_m);
    }
    paintCenters(k_m);
    m_manualInitCount += 1;

    if (m_manualInitCount == k_m.getK()) {
      k_m.setInitialized(true);
      k_m.initClusterCentersHistoryND();
      qDebug() << "Initialized " ;
    }

  } else {
    QMessageBox msgBox;
    msgBox.setText("All clusters are already selected!");
    msgBox.exec();
  }
}

/**
 * @brief Scatter3DWidget::mousePressEvent
 * @param event
 * Mouse Press Event is overriden to allow user
 * select a point and add it as a cluster center
 * by clicking on left mouse button on Scatter3DWidget
 * instance on MainWindow.
 */
void Scatter3DWidget::mousePressEvent(QMouseEvent *event)
{
  qDebug() << "mouse press event scatter3dwidget";
  if (event->button() == Qt::LeftButton) {
    if (!m_initialized) {
      QScatter3DSeries* series = m_scatter3Dvis->getGraph()->seriesList().at(0);
      m_selectedPointID = series->selectedItem();
      qDebug() << "ID " << m_selectedPointID;
      selectClusterCenter();
    }
  }
}
