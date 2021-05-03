#ifndef SCATTERWIDGET3D_H
#define SCATTERWIDGET3D_H

#include <QWidget>

namespace Ui {
class ScatterWidget3D;
}

class ScatterWidget3D : public QWidget
{
  Q_OBJECT

public:
  explicit ScatterWidget3D(QWidget *parent = nullptr);
  ~ScatterWidget3D();

private:
  Ui::ScatterWidget3D *ui;
};

#endif // SCATTERWIDGET3D_H
