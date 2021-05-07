#include "Scatter3DWidget.h"
#include "ui_Scatter3DWidget.h"
#include <QtDataVisualization/Q3DScatter>
#include <QMessageBox>
#include "Scatter3DVis.h"
#include <QDebug>
#include <QSize>
#include <QScreen>

using namespace QtDataVisualization;

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

void Scatter3DWidget::createContainer(Q3DScatter& graph)
{
  m_container = QWidget::createWindowContainer(&graph);

  QSize screenSize = graph.screen()->size();
  m_container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
  m_container->setMaximumSize(screenSize);
  m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_container->setFocusPolicy(Qt::StrongFocus);

  QWidget *widget = new QWidget;
  QHBoxLayout *hLayout = new QHBoxLayout(widget);
  QVBoxLayout *vLayout = new QVBoxLayout();
  hLayout->addWidget(m_container, 1);
  hLayout->addLayout(vLayout);

  m_scatter3Dvis = new Scatter3DVis(&graph);
  //paintPoints();
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

void Scatter3DWidget::getPrevStep(k_means &k_m)
{
  //m_step -= 1;
  paintClusters(k_m);
  paintCenters(k_m);
}
