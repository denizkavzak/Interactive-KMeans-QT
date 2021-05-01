#include "ChartViewWidget.h"
#include "ui_ChartViewWidget.h"
#include <QDebug>

ChartViewWidget::ChartViewWidget(QWidget *parent) :
  QWidget(parent), m_zoom(1),
  ui(new Ui::ChartViewWidget)
{
  ui->setupUi(this);

  m_ChartView = (new ChartView(this));

}

ChartViewWidget::~ChartViewWidget()
{
  delete ui;
}

/**
 * @brief ChartViewWidget::zoom
 * @return
 * Returns the current zoom amount
 */
int ChartViewWidget::zoom() const
{
  return m_zoom;
}

void ChartViewWidget::paintPoints(QVector<QVector2D> points)
{
  ui->chartView->paintPoints(points);
}

void ChartViewWidget::paintClusters(k_means k_m)
{
  ui->chartView->paintClusters(k_m);
}

void ChartViewWidget::zoomIn()
{
  m_zoom = m_zoom *2;
  updateGeometry();
  adjustSize();
  ui->chartView->chart()->zoomIn();
}

void ChartViewWidget::zoomOut()
{
  m_zoom = m_zoom *0.5;
  updateGeometry();
  adjustSize();
  ui->chartView->chart()->zoomOut();
}

void ChartViewWidget::zoomActualSize()
{
  m_zoom = 1;
  updateGeometry();
  adjustSize();
  ui->chartView->chart()->zoomReset();
}
