#include "KMeansDialog.h"
#include "ui_KMeansDialog.h"

KMeansDialog::KMeansDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::KMeansDialog)
{
  ui->setupUi(this);

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
  int k = ui->kSpinBox->value();
  int iter = ui->iterSpinBox->value();
  QString metric = getSelectedMetric();

  emit clusteringParametersChanged(k, metric, iter);
}

QString KMeansDialog::getSelectedMetric()
{
  QString metric = ui->metricComboBox->itemText(ui->metricComboBox->currentIndex());
  if (metric == "Euclidean (L2)") {
    return QString("euclidean");
  }
  return QString("euclidean");
}
