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
  void paintPoints(QVector<QVector2D*> points);
  QScatterSeries* getSeries();
  void paintClusters(QVector<k_means::Cluster*> clusters);
  void paintCenters(k_means k_m);
  void setPointSize(int pointSize, int step);

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
  QScatterSeries *m_series;
  QVector<QScatterSeries*> m_clusterSeries;
  QVector<QScatterSeries*> m_clusterCenterSeries;
  QSizeF m_minSize;
  QSizeF m_maxSize;
  int m_pointSize;
  QPointF m_lastMousePos;

};

#endif // CHARTVIEW_H
