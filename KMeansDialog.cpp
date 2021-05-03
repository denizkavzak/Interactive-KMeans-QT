#include "KMeansDialog.h"
#include "ui_KMeansDialog.h"
#include <QDebug>

KMeansDialog::KMeansDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::KMeansDialog), m_step(0)
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
  int dim = ui->dimensionSpinBox->value();

  emit pointsParametersChanged(noOfPoints, min, max, dim);
}

/**
 * @brief KMeansDialog::applyClustering
 * Takes the k, iteration and metric values
 * from ui and clusters the points accordingly
 */
void KMeansDialog::applyClustering()
{
//  int k = ui->kSpinBox->value();
//  int iter = ui->iterSpinBox->value();
//  QString metric = getSelectedMetric();

  emit clusteringParametersChanged();
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

void KMeansDialog::getNextStep()
{
  qDebug() << "getNextStep in dialog";
  emit stepUpdated();
}

void KMeansDialog::initializeClustering()
{
  int k = ui->kSpinBox->value();
  int iter = ui->iterSpinBox->value();
  QString metric = getSelectedMetric();
  QString q = ui->initComboBox->itemText(ui->initComboBox->currentIndex());
  emit initializationSelected(k, metric, iter, q);
}

void KMeansDialog::updateIterationStepLabel(int newStep)
{
  m_step = newStep;
  ui->stepNoLabel->setText(QString("%1").arg(m_step));
}

void KMeansDialog::updatePointSize()
{
  int pointSize = ui->pointSizeSpinBox->value();
  emit pointSizeUpdated(pointSize);
}

void KMeansDialog::getPrevStep()
{
  qDebug() << "getPrevStep in dialog";
  emit stepPrevUpdated();
}
