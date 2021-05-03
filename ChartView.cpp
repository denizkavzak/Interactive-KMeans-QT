#include "ChartView.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QWidget>
#include <QDebug>

struct ClusterColor{
  ClusterColor() { }

  // TODOO: It has 15 colors currently, update it to be continuous colors
  QVector<QColor> colorArray = {Qt::red, Qt::blue,
                                Qt::black, Qt::magenta, Qt::green,
                                Qt::gray, Qt::cyan, Qt::darkBlue, Qt::darkRed,
                                Qt::darkGreen, Qt::darkCyan, Qt::darkYellow,
                                Qt::darkMagenta, Qt::darkGray, Qt::yellow};
  // QColor a(255,0,255,0); // Can have random color but will it be distinctive?


  QColor operator()(int i){
    // modulo for making repeated pattern when pattern image is smaller
    return colorArray[i % colorArray.size()];
  }
};

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

void ChartView::paintCenters(k_means k_m)
{
  m_clusterCenterSeries.clear();
  ClusterColor c;
  int i = 0;
  QScatterSeries* clusterCenterSeries;
  for (k_means::Cluster* cluster : k_m.getClusters()) {
    clusterCenterSeries = new QScatterSeries();
    // Paint center with rectangle and bigger marker
    clusterCenterSeries->setName(QString("Center_%1").arg(i));
    clusterCenterSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    clusterCenterSeries->setMarkerSize(m_pointSize + 10.0);
    clusterCenterSeries->setColor(c.operator()(i));
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

void ChartView::setPointSize(int pointSize, int step)
{
  qDebug() << " check points " ;
  m_pointSize = pointSize;
  if (step == 0) {
    qDebug() << " in points " ;
    m_series->setMarkerSize(m_pointSize);
  }
  qDebug() << " check centers " ;
  if (!m_clusterCenterSeries.empty()) {
    qDebug() << " in centers " ;
    for (int i = 0; i<m_clusterCenterSeries.size(); i++) {
      m_clusterCenterSeries[i]->setMarkerSize(m_pointSize);
    }
  }
  qDebug() << " check clusters " ;
  if (!m_clusterSeries.empty()) {
    qDebug() << " in clusters " ;
    for (int i = 0; i<m_clusterSeries.size(); i++) {
      m_clusterSeries[i]->setMarkerSize(m_pointSize);
    }
  }
}

/**
 * @brief ChartView::paintClusters
 * @param k_m
 * Paint cluster points with the same color to distinguish between
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
  ClusterColor c;
  int i = 0;

  for (k_means::Cluster* cluster : clusters) {
    QScatterSeries* clusterSeries = new QScatterSeries();
    QScatterSeries* clusterCenterSeries = new QScatterSeries();

    clusterSeries->setName(QString("Cluster_%1").arg(i));

    // Paint points in the cluster
    clusterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    clusterSeries->setMarkerSize(m_pointSize);
    clusterSeries->setColor(c.operator()(i));

    for (QVector2D point : cluster->cluster_points) {
      clusterSeries->append(point.x(),point.y());
    }

    // Paint center with rectangle and bigger marker
    clusterCenterSeries->setName(QString("Center_%1").arg(i));
    clusterCenterSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    clusterCenterSeries->setMarkerSize(m_pointSize + 10.0);
    clusterCenterSeries->setColor(c.operator()(i));
    clusterCenterSeries->append(cluster->center.x(), cluster->center.y());

    i ++;

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

QScatterSeries *ChartView::getSeries()
{
  return m_series;
}
