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

/**
 * @brief KMeansDialog::getNextStep
 * Triggers the stepUpdated signal when next step
 * button is clicked in dialog ui
 */
void KMeansDialog::getNextStep()
{
  qDebug() << "getNextStep in dialog";
  emit stepUpdated();
}

/**
 * @brief KMeansDialog::initializeClustering
 * Triggers the initializationSelected signal when initialize
 * button is clicked in dialog ui
 */
void KMeansDialog::initializeClustering()
{
  int k = ui->kSpinBox->value();
  int iter = ui->iterSpinBox->value();
  QString metric = getSelectedMetric();
  QString q = ui->initComboBox->itemText(ui->initComboBox->currentIndex());
  emit initializationSelected(k, metric, iter, q);
}

/**
 * @brief KMeansDialog::updateIterationStepLabel
 * @param newStep
 * Updates the label in dialog ui where the current
 * iteration step of clustering is shown
 */
void KMeansDialog::updateIterationStepLabel(int newStep)
{
  m_step = newStep;
  ui->stepNoLabel->setText(QString("%1").arg(m_step));
}

/**
 * @brief KMeansDialog::updatePointSize
 * Triggers the pointSizeUpdated signal when
 * the update point size button is clicked
 */
void KMeansDialog::updatePointSize()
{
  int pointSize = ui->pointSizeSpinBox->value();
  emit pointSizeUpdated(pointSize);
}

/**
 * @brief KMeansDialog::getPrevStep
 * Triggers the stepPrevUpdated signal when
 * the previous step button is clicked
 */
void KMeansDialog::getPrevStep()
{
  qDebug() << "getPrevStep in dialog";
  emit stepPrevUpdated();
}

/**
 * @brief KMeansDialog::importPoints
 * Triggers the importFileSelected signal when
 * the import points button is clicked
 */
void KMeansDialog::importPoints()
{
  emit importFileSelected();
}

/**
 * @brief KMeansDialog::updateImportedPointParameters
 * @param numOfPoint
 * @param dimension
 * @param min
 * @param max
 * Updates the spinboxes for number of points, dimension,
 * min value and max value in the dialog ui by using
 * the passed in parameters
 */
void KMeansDialog::updateImportedPointParameters(int numOfPoint, int dimension,
                                                 float min, float max)
{
  ui->NSpinBox->setValue(numOfPoint);
  ui->dimensionSpinBox->setValue(dimension);
  ui->minDoubleSpinBox->setValue(min);
  ui->maxDoubleSpinBox->setValue(max);
}

/**
 * @brief KMeansDialog::updatePointInfoLabel
 * @param s
 * Updates the text in the label indicating the
 * state of points. It can be "Points Generated"
 * or "Points Imported"
 */
void KMeansDialog::updatePointInfoLabel(QString s)
{
  ui->pointsInfoLabel->setText(s);
}
