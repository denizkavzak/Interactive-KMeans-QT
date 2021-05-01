#include "ChartViewWidget.h"
#include "ui_ChartViewWidget.h"

ChartViewWidget::ChartViewWidget(QWidget *parent) :
  QWidget(parent), m_zoom(1),
  ui(new Ui::ChartViewWidget)
{
  ui->setupUi(this);
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

/**
 * @brief ChartViewWidget::setZoom
 * @param zoom
 * A public slot used to change the zoom level of the pixel buffer
 */
void ChartViewWidget::setZoom(int zoom)
{
  // Only update on change
  if(m_zoom != zoom)
  {
    // Ensure zoom is 1 or greater
    m_zoom = qMax(1, zoom);

    // Notify layouts that size has changed
    updateGeometry();

    // Force widget to re-evaluate size
    adjustSize();
  }
}
