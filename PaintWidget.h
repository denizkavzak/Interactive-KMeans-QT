#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include "ChartView.h"

namespace Ui {
class PaintWidget;
}

class PaintWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PaintWidget(QWidget *parent = nullptr);

  int zoom() const;

public slots:
  void setZoom(int zoom);

private:
  int m_zoom;

  ChartView *chartView() const;

  Ui::PaintWidget *ui;
};

#endif // PAINTWIDGET_H
