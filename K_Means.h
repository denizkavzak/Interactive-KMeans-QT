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

  k_means(QVector<QVector<float>*> points, int k); // for ND

  void printClusters();
  void setK(int k);
  void setMetric(QString metric);
  void setNoOfPoints(int num_points);

  struct Cluster{
    QVector2D center;
    QVector<QVector2D> cluster_points;
  };

  struct ClusterND{
    QVector<float> center;
    QVector<QVector<float>> cluster_points;
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

  // functions for ND
  void addPointND(QVector<float*> point); // for ND
  void generateRandomPointsND(float min, float max);
  void generateNormalDistributionPointsND(float min, float max);


private:
  void initializeCenters();

  void setPoints();
  void updateCenters();

  QVector<QVector2D*> m_allPoints;
  int m_num_points;
  int m_k;
  QVector<Cluster*> m_clusters;
  QVector<Cluster*> m_PreviousClusters;
  QVector<QVector2D*> m_previousCenters;

  QVector<QVector<float>*> m_allPointsND; // for ND

  QString m_metric;
  int m_step = 0;
  int m_iter = 0;
  bool m_initialized = false;
  int m_dim = 2;
};

#endif // KMEANS_H
