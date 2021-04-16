#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
  Q_OBJECT
public:
  explicit ChartView(QWidget *parent = nullptr);
  void paintPoints(QVector<QVector2D> points);
  QScatterSeries* getSeries();
private:
  QScatterSeries *m_series;

};

#endif // CHARTVIEW_H
