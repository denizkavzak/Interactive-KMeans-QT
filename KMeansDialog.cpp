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

/**
 * @brief KMeansDialog::applyGeneratePoints
 * Takes the noOfPoints, min and max values
 * from the ui and generates points
 */
void KMeansDialog::applyGeneratePoints()
{
  int noOfPoints = ui->NSpinBox->value();
  float min = ui->minDoubleSpinBox->value();
  float max = ui->maxDoubleSpinBox->value();

  emit pointsParametersChanged(noOfPoints, min, max);
}

/**
 * @brief KMeansDialog::applyClustering
 * Takes the k, iteration and metric values
 * from ui and clusters the points accordingly
 */
void KMeansDialog::applyClustering()
{
  int k = ui->kSpinBox->value();
  int iter = ui->iterSpinBox->value();
  QString metric = getSelectedMetric();

  emit clusteringParametersChanged(k, metric, iter);
}

/**
 * @brief KMeansDialog::getSelectedMetric
 * Take selected distance metric from ui
 * and convert it to simpler text and return
 * as QString
 * @return selected metric as QString
 */
QString KMeansDialog::getSelectedMetric()
{
  QString metric = ui->metricComboBox->itemText(ui->metricComboBox->currentIndex());
  if (metric == "Manhattan (L1)") {
    return QString("manhattan");
  } else if (metric == "Hamming") {
    return QString("hamming");
  } else {
    return QString("euclidean"); // default is "Euclidean (L2)"
  }
}