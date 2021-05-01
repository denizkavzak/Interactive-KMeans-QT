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

signals:
  void pointsParametersChanged(int, float, float);
  void clusteringParametersChanged(int, QString, int);
  void stepUpdated();
  void initializationSelected(int, QString, int, QString);

private:
  Ui::KMeansDialog *ui;
  int m_step;
};

#endif // KMEANSDIALOG_H
