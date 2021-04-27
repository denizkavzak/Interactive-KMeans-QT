#include "initialization.h"
#include <QSet>

initialization::initialization()
{

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
    // Create cluster
    k_means::Cluster cluster;
    cluster.center = center;
    k_means.addCluster(cluster);
  }
}
