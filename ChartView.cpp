#include "ChartView.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QWidget>
#include <QDebug>

ChartView::ChartView(QWidget *parent) :
  QChartView(new QChart(), parent)
{
  m_pointSize = 15;
  m_series = new QScatterSeries();
  m_series->setName("points");
  m_series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  m_series->setMarkerSize(m_pointSize);

  //chart()->zoomIn();
  //setRubberBand(QChartView::RectangleRubberBand);
  //setDragMode(QChartView::ScrollHandDrag);
  //setDragMode(QChartView::NoDrag);
  //  setRubberBand(QChartView::NoRubberBand);
  //  setDragMode(QChartView::ScrollHandDrag);
}

/**
 * @brief ChartView::paintPoints
 * @param points
 * Paints/Visualizes all the 2D points passed into the function
 * in the chart graph
 */
void ChartView::paintPoints(QVector<QVector2D*> points)
{
  for (QVector2D* point : points) {
    m_series->append(point->x(),point->y());
  }

  setRenderHint(QPainter::Antialiasing);
  chart()->addSeries(m_series);

  chart()->setTitle("Points");
  chart()->createDefaultAxes();
  chart()->setDropShadowEnabled(false);

  chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
}

/**
 * @brief ChartView::paintCenters
 * @param k_m
 * Paints/Visualizes all the 2D cluster centers taken from the passed
 * kmeans object. The centers are drawn with a rectangle marker
 * and bigger than the actual data points in the chart graph
 */
void ChartView::paintCenters(k_means k_m)
{
  m_clusterCenterSeries.clear();
  int i = 0;
  QScatterSeries* clusterCenterSeries;
  for (k_means::Cluster* cluster : k_m.getClusters()) {
    clusterCenterSeries = new QScatterSeries();
    // Paint center with rectangle and bigger marker
    clusterCenterSeries->setName(QString("Center_%1").arg(i));
    clusterCenterSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    clusterCenterSeries->setMarkerSize(m_pointSize + 10.0);
    clusterCenterSeries->setColor(*(cluster->color)); // c.operator()(i));
    //*(cluster->color) = c.operator()(i);
    clusterCenterSeries->append(cluster->center.x(), cluster->center.y());
    i ++;

    m_clusterCenterSeries += clusterCenterSeries;
    chart()->addSeries(clusterCenterSeries);
  }

  setRenderHint(QPainter::Antialiasing);
  //chart()->addSeries(m_series);

  chart()->setTitle("Points");
  chart()->createDefaultAxes();
  chart()->setDropShadowEnabled(false);

  chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

}

/**
 * @brief ChartView::setPointSize
 * @param pointSize
 * @param step
 * Sets the point size for the 2D data points in the chart graph
 */
void ChartView::setPointSize(int pointSize, int step)
{
  m_pointSize = pointSize;
  if (step == 0) {
    m_series->setMarkerSize(m_pointSize);
  }
  if (!m_clusterCenterSeries.empty()) { // centers are painted
    for (int i = 0; i < m_clusterCenterSeries.size(); i++) {
      m_clusterCenterSeries[i]->setMarkerSize(m_pointSize + 10.0);
    }
  }
  if (!m_clusterSeries.empty()) {
    for (int i = 0; i < m_clusterSeries.size(); i++) {
      m_clusterSeries[i]->setMarkerSize(m_pointSize);
    }
  }
}

/**
 * @brief ChartView::paintClusters
 * @param k_m
 * Paint 2D cluster points with the same color to distinguish between
 * different clusters.
 * Paints all the points with their respective colors
 * determined by the cluster they belong to.
 * Center of clusters are also painted in different
 * color and shape. Points are circle, centers are rectangle
 */
void ChartView::paintClusters(QVector<k_means::Cluster*> clusters)
{
  m_clusterCenterSeries.clear();
  m_clusterSeries.clear();
  //qDebug() << "IN PAINTING";
  // Clear chart first
  chart()->removeAllSeries();
  int i = 0;

  for (k_means::Cluster* cluster : clusters) {
    QScatterSeries* clusterSeries = new QScatterSeries();
    QScatterSeries* clusterCenterSeries = new QScatterSeries();

    clusterSeries->setName(QString("Cluster_%1").arg(i));

    // Paint points in the cluster
    clusterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    clusterSeries->setMarkerSize(m_pointSize);
    clusterSeries->setColor(*(cluster->color));

    for (QVector2D point : cluster->cluster_points) {
      clusterSeries->append(point.x(),point.y());
    }

    // Paint center with rectangle and bigger marker
    clusterCenterSeries->setName(QString("Center_%1").arg(i));
    clusterCenterSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    clusterCenterSeries->setMarkerSize(m_pointSize + 10.0);
    clusterCenterSeries->setColor(*(cluster->color));
    clusterCenterSeries->append(cluster->center.x(), cluster->center.y());

    i ++;

    // First the cluster points are added, then the cluster center is added
    // cluster points of cluster 0 is in index 0
    // center of cluster 0 is in index 1
    // cluster points of cluster 1 is in index 2
    // center of cluster 1 is in index 3
    chart()->addSeries(clusterSeries);
    chart()->addSeries(clusterCenterSeries);
    m_clusterSeries += clusterSeries;
    m_clusterCenterSeries += clusterCenterSeries;
  }

  setRenderHint(QPainter::Antialiasing);
  //chart()->addSeries(m_series);

  chart()->setTitle("Points");
  chart()->createDefaultAxes();
  chart()->setDropShadowEnabled(false);

  chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

}

//void ChartView::updateRange()
//{
//  m_minSize = chart()->minimumSize();
//  m_maxSize = chart()->maximumSize();

//  emit rangeChanged(m_minSize.width(), m_maxSize.width());
//}

/**
 * @brief ChartView::getSeries
 * @return
 * Getter for the series of the chart graph
 */
QScatterSeries *ChartView::getSeries()
{
  return m_series;
}
