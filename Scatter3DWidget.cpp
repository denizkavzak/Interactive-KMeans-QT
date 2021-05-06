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

void Scatter3DWidget::paintPoints(k_means &k_m)
{
  qDebug() << "inside scatter3Dwidget paint points";
  m_scatter3Dvis->addData(k_m);
}

void Scatter3DWidget::createContainer(Q3DScatter& graph, k_means &k_m)
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

void Scatter3DWidget::paintCentersInit(k_means &k_m)
{
  m_scatter3Dvis->addSeriesForEachCluster(k_m);
}

void Scatter3DWidget::clearAllPointsSeriesFromGraph()
{
  m_scatter3Dvis->clearAllPointsSeriesFromGraph();
}
