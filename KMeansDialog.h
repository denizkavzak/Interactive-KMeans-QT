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

signals:
  void pointsParametersChanged(int, float, float);
  void clusteringParametersChanged(int, QString, int);

private:
  Ui::KMeansDialog *ui;
};

#endif // KMEANSDIALOG_H
