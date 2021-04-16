#include "k_means.h"
#include <iostream>
#include <random>
#include <QMap>
#include <QtMath>
#include <QVector2D>

constexpr int FLOAT_MIN = 0;
constexpr int FLOAT_MAX = 100;

/**
 * @brief k_means::k_means
 * @param num_points: number of points to be generated randomly
 * @param k: number of clusters
 * Constructor for k-means class
 * Generates random points to fill m_num_points
 */
k_means::k_means(int num_points, int k)
{
  m_num_points = num_points;
  m_k = k;
  generateRandomPoints();
}

/**
 * @brief k_means::k_means
 * @param points: points to be clustered
 * @param k: number of clusters
 * Constructor for k-means class
 */
k_means::k_means(QVector<QVector2D> points, int k)
{
  m_allPoints = points;
  m_num_points = m_allPoints.size();
  m_k = k;
}

/**
 * @brief k_means::generateRandomPoints
 * This function generates random m_num_points float points between
 * FLOAT_MIN and FLOAT_MAX
 */
void k_means::generateRandomPoints()
{
  for (int i=0; i< m_num_points; ++i) {
    //QPoint point = QPoint(rand() % 10, rand() % 10);
    float p1 = FLOAT_MIN + (float)(rand()) /
        ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
    float p2 = FLOAT_MIN + (float)(rand()) /
        ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
    QVector2D point = QVector2D(p1, p2);
    m_allPoints += point;
  }

  for (QVector2D p : m_allPoints) {
      std::cout<< p.x() << ", " << p.y() << std::endl;
  }
}

/**
 * @brief k_means::generateNormalDistributionPoints
 * This function generates random m_num_points float points between
 * FLOAT_MIN and FLOAT_MAX, with a normal distribution
 */
void k_means::generateNormalDistributionPoints()
{
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(FLOAT_MIN, FLOAT_MAX);

  for (int i=0; i< m_num_points; ++i) {
    QVector2D point = QVector2D(distr(eng), distr(eng));
    //QPoint point = QPoint(rand() % 10, rand() % 10);
    m_allPoints += point;
  }
}

/**
 * @brief k_means::getDistance
 * @param p1
 * @param p2
 * @return euclidean distance between 2D points p1 and p2
 */
float k_means::getDistance(QVector2D p1, QVector2D p2)
{
  return sqrt((p1.x() - p2.x())*(p1.x() - p2.x()) +
              (p1.y() - p2.y())*(p1.y() - p2.y()));
}

/**
 * @brief k_means::clusterPoints
 * @param num_iterations
 * @return clusters with corresponding centers and clustered points
 * This function implements a simple k-means clustering algorithm
 * using given number of iterations as termination criteria
 */
QVector<k_means::Cluster> k_means::clusterPoints(int num_iterations)
{
  QVector<k_means::Cluster> clusters;

  // Initialize clusters
  // Choose m_k centers randomly
  for (int i=0; i< m_k; ++i) {
    int c = rand() % m_num_points;
    QVector2D center = m_allPoints.at(c);
    // Create cluster
    k_means::Cluster cluster;
    cluster.center = center;
    clusters += cluster;
  }

  // loop in num_iterations
  for (int j=0; j<num_iterations; ++j) {

    // Point assignment
    for (int i=0; i<m_num_points ; ++i) {
      // for each point:
      QVector2D point = m_allPoints.at(i);
      float min = FLT_MAX;
      int min_ind = 0;
      int ind = 0;

      // find the nearest center point
      for (k_means::Cluster cluster : clusters) {
        QVector2D center = cluster.center;
        float distance = getDistance(point,center);
        if (distance<min) {
          min = distance;
          min_ind = ind;
        }
        ind += 1;
      }
      // assign the point to that cluster
      clusters[min_ind].cluster_points += point;
    }

    std::cout<< std::endl << "Iteration : " << j << std::endl;
    std::cout<< "Clusters : " << std::endl;
    for (k_means::Cluster cluster : clusters) {
      std::cout<< "Cluster : " << cluster.cluster_points.size() << std::endl;
      std::cout<< "Center: " << cluster.center.x() << ", "
               << cluster.center.y() << std::endl;
      for (QVector2D p : cluster.cluster_points) {
          std::cout<< p.x() << ", " << p.y() << std::endl;
      }
    }

    // Cluster center update
    // for each cluster center 0...m_k
    int ind = 0;
    for (k_means::Cluster cluster : clusters) {
      // new center = mean of all points assigned to that cluster

      float x = 0;
      float y = 0;
      for (QVector2D cluster_point : cluster.cluster_points) {
        x += cluster_point.x();
        y += cluster_point.y();
      }

      clusters[ind].center = QVector2D(x/cluster.cluster_points.size(),
                              y/cluster.cluster_points.size());

      clusters[ind].cluster_points.clear();
      ind += 1;
    }

  }

  // Final Point assignment
  for (int i=0; i<m_num_points ; ++i) {
    // for each point:
    QVector2D point = m_allPoints.at(i);
    float min = FLT_MAX;
    int min_ind = 0;
    int ind = 0;

    // find the nearest center point
    for (k_means::Cluster cluster : clusters) {
      QVector2D center = cluster.center;
      float distance = getDistance(point,center);
      if (distance<min) {
        min = distance;
        min_ind = ind;
      }
      ind += 1;
    }
    // assign the point to that cluster
    clusters[min_ind].cluster_points += point;
  }

  std::cout<< "/------------Final Clusters---------------/: " << std::endl;
  for (k_means::Cluster cluster : clusters) {
    std::cout<< "Cluster : " << cluster.cluster_points.size() << std::endl;
    std::cout<< "Center: " << cluster.center.x() << ", "
             << cluster.center.y() << std::endl;
    for (QVector2D p : cluster.cluster_points) {
        std::cout<< p.x() << ", " << p.y() << std::endl;
    }
  }
  return clusters;
}
