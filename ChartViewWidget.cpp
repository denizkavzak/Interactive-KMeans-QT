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

/**
 * @brief ChartViewWidget::paintPoints
 * @param points
 * Calls the paint points function from Chart View
 * to visualize all 2D points
 */
void ChartViewWidget::paintPoints(QVector<QVector2D*> points)
{
  ui->chartView->paintPoints(points);
}

/**
 * @brief ChartViewWidget::paintClusters
 * @param k_m
 * Calls the paint clusters function from Chart View
 * to visualize the clusters with all their points and centers
 * with different colors that are assigned to them
 */
void ChartViewWidget::paintClusters(k_means k_m)
{
  qDebug() << "PAINT";
  m_step = k_m.getNumOfIter();
  ui->chartView->paintClusters(k_m.getClusters());
}

/**
 * @brief ChartViewWidget::paintCenters
 * @param k_m
 * Calls the paint centers function from Chart View
 * to visualize the cluster centers with their asigned
 * color and rectangle shape
 */
void ChartViewWidget::paintCenters(k_means k_m)
{
  ui->chartView->paintCenters(k_m);
}

/**
 * @brief ChartViewWidget::zoomIn
 * Calls zoom in function from Chart View
 */
void ChartViewWidget::zoomIn()
{
  m_zoom = m_zoom *2;
  updateGeometry();
  adjustSize();
  ui->chartView->chart()->zoomIn();
}

/**
 * @brief ChartViewWidget::zoomOut
 * Calls zoom out function from Chart View
 */
void ChartViewWidget::zoomOut()
{
  m_zoom = m_zoom *0.5;
  updateGeometry();
  adjustSize();
  ui->chartView->chart()->zoomOut();
}

/**
 * @brief ChartViewWidget::zoomActualSize
 * Calls zoom actual size from Chart View
 */
void ChartViewWidget::zoomActualSize()
{
  m_zoom = 1;
  updateGeometry();
  adjustSize();
  ui->chartView->chart()->zoomReset();
}

/**
 * @brief ChartViewWidget::getNextStep
 * @param k_m
 * Calls paint cluster function from Chart view
 * so that it is triggered and called when
 * the user clicks on next step in clustering
 */
void ChartViewWidget::getNextStep(k_means &k_m)
{
  qDebug() << " getNextStep in chartview widget" ;
  m_step += 1;
  ui->chartView->paintClusters(k_m.getClusters());
}

/**
 * @brief ChartViewWidget::getPrevStep
 * @param k_m
 * Calls paint cluster sunction from Chart view
 * so that it is triggered and called when
 * the user clicks on previous step in clusterin
 */
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
