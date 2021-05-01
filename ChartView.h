#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <k_means.h>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
  Q_OBJECT

public:
  explicit ChartView(QWidget *parent = nullptr);
  void paintPoints(QVector<QVector2D> points);
  QScatterSeries* getSeries();
  void paintClusters(k_means k_m);

private:
  QScatterSeries *m_series;

};

#endif // CHARTVIEW_H
