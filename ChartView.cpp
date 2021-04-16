#include "ChartView.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QWidget>

ChartView::ChartView(QWidget *parent) :
  QChartView(new QChart(), parent)
{
  m_series = new QScatterSeries();
  m_series->setName("points");
  m_series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  m_series->setMarkerSize(15.0);

}

void ChartView::paintPoints(QVector<QVector2D> points)
{
  for (QVector2D point : points) {
    m_series->append(point.x(),point.y());
  }

  setRenderHint(QPainter::Antialiasing);
  chart()->addSeries(m_series);

  chart()->setTitle("Points");
  chart()->createDefaultAxes();
  chart()->setDropShadowEnabled(false);

  chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

}

QScatterSeries *ChartView::getSeries()
{
  return m_series;
}
