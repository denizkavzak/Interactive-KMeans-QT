#include "PaintWidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent), m_zoom(1)
, ui(new Ui::PaintWidget)
{

}

// Returns the current zoom amount
int PaintWidget::zoom() const
{
  return m_zoom;
}

//
// setZoom() is a public slot used to change the zoom level of the pixel buffer
//
void PaintWidget::setZoom(int zoom)
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

//namespace Ui {
//    class PaintWidget: public Ui_PaintWidget {};
//} // namespace Ui
