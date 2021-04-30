#include "KMeansDialog.h"
#include "ui_KMeansDialog.h"

KMeansDialog::KMeansDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::KMeansDialog)
{
  ui->setupUi(this);


  connect(ui->generatePointsButton, &QPushButton::clicked,
          this, &KMeansDialog::applyGeneratePoints);

  connect(ui->clusterButton, &QPushButton::clicked,
          this, &KMeansDialog::applyClustering);

}

KMeansDialog::~KMeansDialog()
{
  delete ui;
}


void KMeansDialog::applyGeneratePoints()
{
  int noOfPoints = ui->NSpinBox->value();
  float min = ui->minDoubleSpinBox->value();
  float max = ui->maxDoubleSpinBox->value();

  emit pointsParametersChanged(noOfPoints, min, max);
}

void KMeansDialog::applyClustering()
{
  int k = ui->kSpinBox->value()-1;
  int iter = ui->iterSpinBox->value();
  QString metric = getSelectedMetric();

  emit clusteringParametersChanged(k, metric, iter);
}

QString KMeansDialog::getSelectedMetric()
{
  return ui->metricComboBox->itemText(ui->metricComboBox->currentIndex());
}
