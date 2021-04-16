#ifndef KMEANS_H
#define KMEANS_H

#include <QPoint>
#include <QVector>
#include <QVector2D>

class k_means
{
public:
  k_means(int num_points, int k);
  k_means(QVector<QVector2D> points, int k);

  struct Cluster{
    QVector2D center;
    QVector<QVector2D> cluster_points;
  };

  QVector<Cluster> clusterPoints(int num_iterations);

private:
  void generateRandomPoints();
  void generateNormalDistributionPoints();
  void initializeCenters();
  float getDistance(QVector2D p1, QVector2D p2);
  void setPoints();
  void updateCenters();
  void updatePoints();

  QVector<QVector2D> m_allPoints;
  int m_num_points;
  int m_k;

};

#endif // KMEANS_H
