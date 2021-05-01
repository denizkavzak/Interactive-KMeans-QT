#include "ChartViewWidget.h"
#include "ui_ChartViewWidget.h"
#include <QDebug>

ChartViewWidget::ChartViewWidget(QWidget *parent) :
  QWidget(parent), m_zoom(1), m_scrolling(false),
  ui(new Ui::ChartViewWidget), m_step(0)
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

void ChartViewWidget::paintPoints(QVector<QVector2D> points)
{
  ui->chartView->paintPoints(points);
}

void ChartViewWidget::paintClusters(k_means k_m)
{
  qDebug() << "PAINT";
  ui->chartView->paintClusters(k_m);
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

void ChartViewWidget::getNextStep(k_means k_m)
{
  qDebug() << " getNextStep in chartview widget" ;
  m_step += 1;
  ui->chartView->paintClusters(k_m);
}

void ChartViewWidget::setScrolling(bool scrolling)
{
  m_scrolling = scrolling;
}

void ChartViewWidget::on_rangeChanged(qreal min, qreal max)
{
//  if (m_scrolling) return;
//  QChart          *c = ui->chartView->chart( );
//  QAbstractAxis   *x = c->axisX( );
//  qreal            avg = (min + max) / 2.0;
//  bool             range_fixed = false;


//  //    /*
//  //     * Make sure the new range is sane; fix if not.
//  //     */
//  //    if ((max - min) < 0.1) {    // Avoid overzooming
//  //        min = avg - 0.05;
//  //        max = avg + 0.05;
//  //        range_fixed = true;
//  //    }

//      if (min < 0.0) { min = 0.0; range_fixed = true; }

//      if (max > 4.0) { max = 4.0; range_fixed = true; }

//      if (range_fixed) {
//          x->setRange( min, max );    // will re-signal with the fixed range
//          return;
//      }

//      qreal    vis_width = c->plotArea( ).width( );
//      qreal    all_width = vis_width * (4.0 - 0.0) / (max - min);

  //    cerr << "range " << min << " ... " << max << " in " << vis_width << " pixels" << endl;
  //    cerr << "full width requires " << all_width << " pixels" << endl;;

//      if (max - min < 4.0) {
//  //        cerr << "set scroll parameters" << endl;
//          m_scrolling = true;
//          ui->chartScroll->setMaximum( all_width - vis_width );
//          qreal sv = min / (4.0 - 0.0) * all_width;
//          ui->chartScroll->setValue( sv );
//          m_scrolling = false;
//      } else {
//  //        cerr << "disable scroll bar" << endl;
//          m_scrolling = true;
//          ui->chartScroll->setMaximum( 0 );
//          ui->chartScroll->setValue( sv );
//          m_scrolling = false;
//      }

}

//void ChartViewWidget::on_rangeChanged( qreal min, qreal max )
//{
//    if (m_scrolling) return;  // Scrolling causes range changes, but we don't have to do anything.

//    QChart          *c = ui->chartView->chart( );
//    QAbstractAxis   *x = c->axisX( );
//    qreal            avg = (min + max) / 2.0;
//    bool             range_fixed = false;

//    /*
//     * Make sure the new range is sane; fix if not.
//     */
//    if ((max - min) < 0.1) {    // Avoid overzooming
//        min = avg - 0.05;
//        max = avg + 0.05;
//        range_fixed = true;
//    }

//    if (min < 0.0) { min = 0.0; range_fixed = true; }

//    if (max > 4.0) { max = 4.0; range_fixed = true; }

//    if (range_fixed) {
//        x->setRange( min, max );    // will re-signal with the fixed range
//        return;
//    }

//    qreal    vis_width = c->plotArea( ).width( );
//    qreal    all_width = vis_width * (4.0 - 0.0) / (max - min);

////    cerr << "range " << min << " ... " << max << " in " << vis_width << " pixels" << endl;
////    cerr << "full width requires " << all_width << " pixels" << endl;;

//    if (max - min < 4.0) {
////        cerr << "set scroll parameters" << endl;
//        m_scrolling = true;
//        ui->chartScroll->setMaximum( all_width - vis_width );
//        sv = min / (4.0 - 0.0) * all_width;
//        ui->chartScroll->setValue( sv );
//        m_scrolling = false;
//    } else {
////        cerr << "disable scroll bar" << endl;
//        m_scrolling = true;
//        ui->chartScroll->setMaximum( 0 );
//        ui->chartScroll->setValue( sv );
//        m_scrolling = false;
//    }
//}

