#include "initialization.h"
#include <QSet>
#include <QDebug>

constexpr int FLOAT_MIN = 0;
constexpr int FLOAT_MAX = 100;

initialization::initialization()
{

}

/**
 * @brief initialization::initRandomReal
 * @param k_means
 * This function initializes cluster centers
 * using randomly selected points from the
 * same space as the points to be clustered
 */
void initialization::initRandomReal(k_means &k_means)
{
  for (int i = 0; i < k_means.getK(); ++i) {
    float p1 = FLOAT_MIN + (float)(rand()) /
        ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
    float p2 = FLOAT_MIN + (float)(rand()) /
        ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
    QVector2D center = QVector2D(p1, p2);
    qDebug() << "CENTER: " << center;
    k_means::Cluster cluster;
    cluster.center = center;
    k_means.addCluster(cluster);
  }
}

/**
 * @brief initialization::initRandomSample
 * @param k_means
 * This function initializes cluster centers
 * using randomly selected points from the
 * all points for m_k klusters
 */
void initialization::initRandomSample(k_means &k_means)
{
  // Choose m_k centers randomly
  QSet<int> set;
  for (int i = 0; i < k_means.getK(); ++i) {
    int c = rand() % k_means.getNumOfPoints();
    // TODO: FIX: This would cause an infinite loop in case where k > num_points
    while (set.contains(c)) {   // Make sure no two selected points are the same
      c = rand() % k_means.getNumOfPoints();
    }
    set.insert(c);
    QVector2D center = k_means.getAllPoints().at(c);
    qDebug() << "CENTER: " << center;
    // Create cluster
    k_means::Cluster cluster;
    cluster.center = center;
    k_means.addCluster(cluster);
  }
}

