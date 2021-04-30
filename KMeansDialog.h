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

private slots:
  QString getSelectedMetric();

signals:
  void pointsParametersChanged(int noOfPoints, float min, float max);
  void clusteringParametersChanged(int k, QString metric);

private:
  Ui::KMeansDialog *ui;
};

#endif // KMEANSDIALOG_H
