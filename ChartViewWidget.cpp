#include "ChartViewWidget.h"
#include "ui_ChartViewWidget.h"
#include <QDebug>
#include <QMessageBox>

ChartViewWidget::ChartViewWidget(QWidget *parent) :
  QWidget(parent), m_zoom(1), m_scrolling(false),
  ui(new Ui::ChartViewWidget), m_step(0), m_pointSize(15.0)
{
  ui->setupUi(this);

  //m_ChartView = (new ChartView(this));
  ui->chartView->setRubberBand( QChartView::HorizontalRubberBand );

  //connect( c->axisX( ), SIGNAL(rangeChanged(qreal, qreal)), this,
  //  SLOT(on_rangeChanged(qreal,qreal)) );
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

void ChartViewWidget::paintPoints(QVector<QVector2D*> points)
{
  ui->chartView->paintPoints(points);
}

void ChartViewWidget::paintClusters(k_means k_m)
{
  qDebug() << "PAINT";
  m_step = k_m.getNumOfIter();
  ui->chartView->paintClusters(k_m.getClusters());
}

void ChartViewWidget::paintCenters(k_means k_m)
{
  ui->chartView->paintCenters(k_m);
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

void ChartViewWidget::getNextStep(k_means &k_m)
{
  qDebug() << " getNextStep in chartview widget" ;
  m_step += 1;
  ui->chartView->paintClusters(k_m.getClusters());
}

void ChartViewWidget::getPrevStep(k_means &k_m)
{
  //qDebug() << k_m.getPrevClusters().size();
  m_step -= 1;
  ui->chartView->paintClusters(k_m.getClusters());
}

/**
 * @brief ChartViewWidget::updatePointSize
 * @param k_m
 * @param pointSize
 * Called when user wants to change the point size in the chart
 */
void ChartViewWidget::updatePointSize(int pointSize)
{
  // Currently resizing will work before clustering starts
  if (m_step == 0) {
    // Only attempt to draw if size is different than current size
    if (pointSize != m_pointSize) {
      m_pointSize = pointSize;
      ui->chartView->setPointSize(pointSize, m_step);
    }
  } else {
    QMessageBox msgBox;
    msgBox.setText("Currently point size can only be changed "
                   "before clustering starts!");
    msgBox.exec();
  }
}
