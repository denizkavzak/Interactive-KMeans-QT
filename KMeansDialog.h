#ifndef KMEANSDIALOG_H
#define KMEANSDIALOG_H

#include <QDialog>

namespace Ui {
class KMeansDialog;
}

class KMeansDialog : public QDialog
{
  Q_OBJECT

public:
  explicit KMeansDialog(QWidget *parent = nullptr);
  ~KMeansDialog();

public slots:
  void applyGeneratePoints();
  void applyClustering();
  QString getSelectedMetric();
  void getNextStep();
  void initializeClustering();
  void updateIterationStepLabel(int newStep);
  void updatePointSize();
  void getPrevStep();
  void importPoints();
  void updateImportedPointParameters(int numOfPoint, int dimension,
                                     float min, float max);
  void updatePointInfoLabel(QString s);
  void updateConvergenceTimeLabel(int time);
  void updateConvergenceStepLabel(int step);
  void play();
  void stop();
  void reset();

  void getInitializationParameters(int&, QString&, int&);
  void getMinMax(float &min, float &max);

signals:
  void pointsParametersChanged(int, float, float, int);
  void clusteringParametersChanged();
  void stepUpdated();
  void stepPrevUpdated();
  void initializationSelected(int, QString, int, QString);
  void pointSizeUpdated(int);
  void importFileSelected();
  void playTriggered(int ms_value);
  void stopTriggered();
  void resetTriggered();

private:
  Ui::KMeansDialog *ui;
  int m_step;
};

#endif // KMEANSDIALOG_H
