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

  void paintPoints(QVector<QVector2D> points);
  void paintClusters(k_means k_m);

public slots:
  void zoomIn();
  void zoomOut();
  void zoomActualSize();

  void getNextStep(k_means k_m);

  void setScrolling(bool scroll);
  void on_rangeChanged(qreal horizontalMin, qreal horizontal_max);

signals:
  void zoomChanged(int);

private:

  int m_zoom;
  bool m_scrolling;

  Ui::ChartViewWidget *ui;

  int m_step;

  //ChartView* m_ChartView;

};

#endif // CHARTVIEWWIDGET_H
