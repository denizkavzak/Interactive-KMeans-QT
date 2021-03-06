#ifndef CHARTVIEWWIDGET_H
#define CHARTVIEWWIDGET_H

#include <QWidget>
#include "ChartView.h"
#include "k_means.h"

namespace Ui {
class ChartViewWidget;
}

class ChartViewWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ChartViewWidget(QWidget *parent = nullptr);
  ~ChartViewWidget();

  int zoom() const;

  void paintPoints(QVector<QVector2D*> points);
  void paintClusters(k_means k_m);
  void paintCenters(k_means k_m);
  void updatePointSize(int pointSize);
  void clearChart();

public slots:
  void zoomIn();
  void zoomOut();
  void zoomActualSize();

  void getNextStep(k_means &k_m);
  void getPrevStep(k_means &k_m);

private:
  Ui::ChartViewWidget *ui;

  int m_zoom;
  bool m_scrolling;
  int m_step;
  int m_pointSize;
};

#endif // CHARTVIEWWIDGET_H
