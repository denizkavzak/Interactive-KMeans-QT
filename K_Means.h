#ifndef KMEANS_H
#define KMEANS_H

#include <QPoint>
#include <QVector>
#include <QVector2D>

class k_means
{
public:
  k_means();
  k_means(int num_points);
  k_means(int num_points, int k, float min, float max);
  k_means(QVector<QVector2D*> points, int k);

  void printClusters();
  void setK(int k);
  void setMetric(QString metric);
  void setNoOfPoints(int num_points);

  struct Cluster{
    QVector2D center;
    QVector<QVector2D> cluster_points;
  };

  void clusterPoints(int num_iterations);
  QVector<Cluster*> getClusters();
  QVector<QVector2D*> getAllPoints();
  int getNumOfPoints();
  int getK();
  QString getMetric();
  QVector2D getCenters();
  void addCluster(Cluster* cluster);
  void addPoint(QVector2D* point);
  void generateRandomPoints(float min, float max);
  void generateNormalDistributionPoints(float min, float max);
  void moveOneStep();
  void finalizeOneStep();
  void setNumOfIter(int numOfIterations);
  int getNumOfIter();
  void setInitialized(bool);
  bool isInitialized();
  QVector<QVector2D*> getPrevClusterCenters();
  void updatePrevClusterCenters();
  void setClusterCentersToPrev();
  void setStep(int step);

  void clearClusterPoints();
private:
  void initializeCenters();
  float getDistance(QVector2D p1, QVector2D p2);
  void setPoints();
  void updateCenters();

  QVector<QVector2D*> m_allPoints;
  int m_num_points;
  int m_k;
  QVector<Cluster*> m_clusters;
  QVector<Cluster*> m_PreviousClusters;
  QVector<QVector2D*> m_previousCenters;
  QString m_metric;
  int m_step = 0;
  int m_iter = 0;
  bool m_initialized = false;
};

#endif // KMEANS_H
