#include "ScatterWidget3D.h"
#include "ui_ScatterWidget3D.h"

ScatterWidget3D::ScatterWidget3D(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ScatterWidget3D)
{
  ui->setupUi(this);
}

ScatterWidget3D::~ScatterWidget3D()
{
  delete ui;
}
