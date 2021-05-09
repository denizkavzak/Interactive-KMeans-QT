#ifndef INPUTHANDLERPANNING_H
#define INPUTHANDLERPANNING_H

#include <QtDataVisualization/Q3DInputHandler>
#include <QtDataVisualization/QAbstract3DGraph>
#include <QtDataVisualization/QValue3DAxis>

using namespace QtDataVisualization;

class InputHandlerPanning : public Q3DInputHandler
{
  Q_OBJECT

  enum InputState {
    StateNormal = 0,
    StateDraggingX,
    StateDraggingZ,
    StateDraggingY
  };


public:
  explicit InputHandlerPanning(QAbstract3DGraph *graph, QObject *parent = nullptr);

  void setAxes(QValue3DAxis *axisX, QValue3DAxis *axisY, QValue3DAxis *axisZ);

  void setDragSpeedModifier(float modifier);

  virtual void mousePressEvent(QMouseEvent *event, const QPoint &mousePos) override;
  virtual void mouseMoveEvent(QMouseEvent *event, const QPoint &mousePos) override;
  virtual void mouseReleaseEvent(QMouseEvent *event, const QPoint &mousePos) override;

private:
  void handleElementSelected(QAbstract3DGraph::ElementType type);
  void handleAxisDragging();

private:
  bool m_mousePressed;
  InputState m_state;
  QValue3DAxis *m_axisX;
  QValue3DAxis *m_axisZ;
  QValue3DAxis *m_axisY;
  float m_speedModifier;

};

#endif // INPUTHANDLERPANNING_H
