#include "initialization.h"
#include <QSet>
#include <QDebug>
#include <QtMath>

constexpr int FLOAT_MIN = 0;
constexpr int FLOAT_MAX = 100;


initialization::initialization()
{

}

// lambda function for euclidean distance of 2 2D points
auto euclidean = [] (QVector2D p1, QVector2D p2) {
  return sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) +
              (p1.y()-p2.y())*(p1.y()-p2.y()));
};

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

/**
 * @brief initialization::initKMeansPp
 * @param k_means
 * This function implements the center initialization
 * algorithm from Kmeans++ using distance between points and
 * candidate centers trying to maximize the distance
 */
void initialization::initKMeansPp(k_means &k_means)
{
  int c = rand() % k_means.getNumOfPoints();
  QVector2D center = k_means.getAllPoints().at(c);

  qDebug() << "CENTER: " << center;
  // Create cluster
  k_means::Cluster cluster;
  cluster.center = center;
  k_means.addCluster(cluster);

  // for each point get distance between center and that point:
  float min;
  int min_ind;
  float sum;
  QVector<float> d = getPairwiseDistances(k_means, center, k_means.getMetric(),
                                          min, min_ind, sum);
  qDebug() << "d is found";
  for (int i = 0; i < k_means.getK(); ++i) {
    QVector<float> prob = getProbs(d, sum);
    qDebug() << "probs: " << prob;
    int ind = choseWithProb(prob);
    qDebug() << "selected ind: " << ind;
    // Get new center point
    center = k_means.getAllPoints().at(ind);
    k_means::Cluster cluster; // TODO: create constructor taking center
    cluster.center = center;
    k_means.addCluster(cluster);

    // Get distances between all points and the new center
    QVector<float> d2 = getPairwiseDistances(k_means, center, "euclidean",
                                            min, min_ind, sum);

    d = choseMin(d, d2); // select the min (worse) distances for next iteration
  }
}

/**
 * @brief initialization::getPairwiseDistances
 * @param k_means
 * @param center : center point
 * @param metric : metric for the distance calculation, i.e. "euclidean"
 * @param min : passed param to return minimum distance value
 * @param min_ind : passed param to return minimum distance index
 * @param sum : passed param to return the sum of squared distances
 * This function calculates the distances between all points in the
 * k_means object and given center.
 * @return
 */
QVector<float> initialization::getPairwiseDistances(k_means &k_means,
                                                    QVector2D center,
                                                    QString metric, float &min,
                                                    int &min_ind, float &sum)
{
  // Create an array to store the distances between center and all points
  QVector<float> d(k_means.getNumOfPoints());
  min = FLOAT_MAX;
  min_ind = 0;
  sum = 0;

  for (int i = 0; i < k_means.getNumOfPoints(); ++i) {
    QVector2D point = k_means.getAllPoints().at(i);

    if (metric == "euclidean"){
      float distance = euclidean(center,point);
      if (distance < min){
        min = distance;
        min_ind = i;
      }
      d[i] = distance;
      sum += distance*distance;
    }
  }
  return d;
}

/**
 * @brief initialization::choseMin
 * @param d
 * @param d2
 * @return Takes in two float vectors, compares elementwise,
 * creates and returns a vector with the minimum value for each
 * element index
 */
QVector<float> initialization::choseMin(QVector<float> d, QVector<float> d2)
{
  QVector<float> min(d.size());
  for (int i = 0; i< d.size(); i++) {
    min[i] = qMin(d[i], d2[i]);
  }
  return min;
}

/**
 * @brief initialization::getProbs
 * @param d
 * @param sum
 * @return Given distances vector and sum of squares of distances,
 * calculates the probability value for each element and returns
 * the probability vector
 */
QVector<float> initialization::getProbs(QVector<float> d, float sum)
{
  QVector<float> probs(d.size());
  for (int i = 0; i< d.size(); i++) {
    probs[i] = (d[i] * d[i]) / sum;
  }
  return probs;
}

int initialization::choseWithProb(const QVector<float> prob)
{
  QVector<float> sorted = prob;
  std::sort(sorted.begin(), sorted.end());

  qDebug() << "SORTED: " << sorted;

  float rand_x = (float) rand()/RAND_MAX;
  float cum = 0;
  float selected_prob = 0;

  for (int i = 0; i< prob.size(); i++){
    cum += sorted[i];

    if (rand_x < cum) {
      selected_prob = sorted[i];
      break;
    }
  }

  int ind = prob.indexOf(selected_prob);
  int last_ind = prob.lastIndexOf(selected_prob);
  // If there are more than one points with same probability
  // break the tie by randomly selecting one of them
  if (ind != last_ind){
    ind = (rand() % last_ind) + ind;
  }

  return ind;
}



