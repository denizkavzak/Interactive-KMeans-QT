#ifndef CHARTVIEWWIDGET_H
#define CHARTVIEWWIDGET_H

#include <QWidget>
#include "ChartView.h"

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

  //ZoomableChart *chart() const;
  ChartView *chartView() const;

public slots:
  void setZoom(int zoom);

private:

  int m_zoom;

  Ui::ChartViewWidget *ui;

};

#endif // CHARTVIEWWIDGET_H
