#include "InputHandlerPanning.h"
#include <QtMath>


InputHandlerPanning::InputHandlerPanning(QAbstract3DGraph *graph,
                                         QObject *parent) :
  Q3DInputHandler(parent),
  m_mousePressed(false),
  m_state(StateNormal),
  m_axisX(0),
  m_axisZ(0),
  m_axisY(0),
  m_speedModifier(15.0f)
{
  // Connect to the item selection signal from graph
  connect(graph, &QAbstract3DGraph::selectedElementChanged, this,
          &InputHandlerPanning::handleElementSelected);
}

void InputHandlerPanning::setAxes(QValue3DAxis *axisX, QValue3DAxis *axisY, QValue3DAxis *axisZ)
{
  m_axisX = axisX;
  m_axisY = axisY;
  m_axisZ = axisZ;
}

void InputHandlerPanning::setDragSpeedModifier(float modifier)
{
  m_speedModifier = modifier;
}

void InputHandlerPanning::mousePressEvent(QMouseEvent *event, const QPoint &mousePos)
{
  Q3DInputHandler::mousePressEvent(event, mousePos);
  if (event->button() == Qt::LeftButton) {
    m_mousePressed = true;
  }
}

void InputHandlerPanning::mouseMoveEvent(QMouseEvent *event, const QPoint &mousePos)
{
  // Check if we're trying to drag axis label
  if (m_mousePressed && m_state != StateNormal) {
    setPreviousInputPos(inputPosition());
    setInputPosition(mousePos);
    handleAxisDragging();
  } else {
    Q3DInputHandler::mouseMoveEvent(event, mousePos);
  }
}

void InputHandlerPanning::mouseReleaseEvent(QMouseEvent *event, const QPoint &mousePos)
{
  Q3DInputHandler::mouseReleaseEvent(event, mousePos);
  m_mousePressed = false;
  m_state = StateNormal;
}

void InputHandlerPanning::handleElementSelected(QAbstract3DGraph::ElementType type)
{
  switch (type) {
  case QAbstract3DGraph::ElementAxisXLabel:
    m_state = StateDraggingX;
    break;
  case QAbstract3DGraph::ElementAxisZLabel:
    m_state = StateDraggingZ;
    break;
  default:
    m_state = StateNormal;
    break;
  }
}

void InputHandlerPanning::handleAxisDragging()
{
  float distance = 0.0f;

  //! [6]
  // Get scene orientation from active camera
  float xRotation = scene()->activeCamera()->xRotation();
  float yRotation = scene()->activeCamera()->yRotation();
  //! [6]

  //! [7]
  // Calculate directional drag multipliers based on rotation
  float xMulX = qCos(qDegreesToRadians(xRotation));
  float xMulY = qSin(qDegreesToRadians(xRotation));
  float zMulX = qSin(qDegreesToRadians(xRotation));
  float zMulY = qCos(qDegreesToRadians(xRotation));
  //! [7]

  //! [8]
  // Get the drag amount
  QPoint move = inputPosition() - previousInputPos();

  // Flip the effect of y movement if we're viewing from below
  float yMove = (yRotation < 0) ? -move.y() : move.y();
  //! [8]

  //! [9]
  // Adjust axes
  switch (m_state) {
  case StateDraggingX:
    distance = (move.x() * xMulX - yMove * xMulY) / m_speedModifier;
    m_axisX->setRange(m_axisX->min() - distance, m_axisX->max() - distance);
    break;
  case StateDraggingZ:
    distance = (move.x() * zMulX + yMove * zMulY) / m_speedModifier;
    m_axisZ->setRange(m_axisZ->min() + distance, m_axisZ->max() + distance);
    break;
  case StateDraggingY:
    distance = move.y() / m_speedModifier; // No need to use adjusted y move here
    m_axisY->setRange(m_axisY->min() + distance, m_axisY->max() + distance);
    break;
  default:
    break;
  }
  //! [9]
}


