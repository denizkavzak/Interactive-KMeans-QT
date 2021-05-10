#include "initialization.h"
#include <QSet>
#include <QDebug>
#include <QtMath>
#include "metrics.h"
#include <k_means.h>
#include <random>
#include <QColor>

constexpr int FLOAT_MIN = 0;
constexpr int FLOAT_MAX = 100;

initialization::initialization()
{
  srand(time(nullptr));
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
  k_means::ClusterColor* color = new k_means::ClusterColor();
  for (int i = 0; i < k_means.getK(); ++i) {
    float p1 = FLOAT_MIN + (float)(rand()) /
        ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
    float p2 = FLOAT_MIN + (float)(rand()) /
        ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
    QVector2D center = QVector2D(p1, p2);
    qDebug() << "CENTER: " << center;
    k_means::Cluster* cluster = new k_means::Cluster();
    cluster->center = center;
    cluster->color = color->operator()(i);
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
  k_means::ClusterColor* color = new k_means::ClusterColor();
  // Choose m_k centers randomly
  QSet<int> set;
  for (int i = 0; i < k_means.getK(); ++i) {
    int c = rand() % k_means.getNumOfPoints();
    // since we prohibited cases k > number of points, no problem
    while (set.contains(c)) {   // Make sure no two selected points are the same
      c = rand() % k_means.getNumOfPoints();
    }
    set.insert(c);
    QVector2D* center = k_means.getAllPoints().at(c);
    qDebug() << "CENTER: " << *center;
    // Create cluster
    k_means::Cluster* cluster = new k_means::Cluster();
    cluster->center = *center;
    cluster->color = color->operator()(i);
    qDebug() << "Cluster color " << *(cluster->color);
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
  QSet<int> set;
  k_means::ClusterColor* color = new k_means::ClusterColor();
  int c = rand() % k_means.getNumOfPoints();
  set.insert(c);
  QVector2D* center = k_means.getAllPoints().at(c);

  qDebug() << "CENTER: " << *center;
  // Create cluster
  k_means::Cluster* cluster = new k_means::Cluster();
  cluster->center = *center;
  cluster->color = color->operator()(0);
  k_means.addCluster(cluster);

  // for each point get distance between center and that point:
  float min;
  int min_ind;
  float sum;
  QVector<float> d = getPairwiseDistances(k_means, *center, k_means.getMetric(),
                                          min, min_ind, sum);

  for (int i = 1; i < k_means.getK(); ++i) {
    QVector<float> prob = getProbs(d, sum);

    int ind = choseWithProb(prob);
    // since we prohibited cases k > number of points, no problem
    while (set.contains(ind)) {
      ind = choseWithProb(prob);
    }
    set.insert(ind);

    // Get new center point
    center = k_means.getAllPoints().at(ind);
    k_means::Cluster* cluster = new k_means::Cluster();
    cluster->center = *center;
    cluster->color = color->operator()(i);
    k_means.addCluster(cluster);

    // Get distances between all points and the new center
    QVector<float> d2 = getPairwiseDistances(k_means, *center,
                                             k_means.getMetric(),
                                            min, min_ind, sum);

    d = choseMin(d, d2); // select the min (worse) distances for next iteration
  }
}

/**
 * @brief initialization::generateRandomPoints
 * @param min
 * @param max
 * @param k_means
 * Generate points and initialize the points
 * in k_means object. Currently only supports
 * random number generation with normal distribution
 * m_num_points float points between
 * min and max
 */
void initialization::generateRandomPoints(float min, float max,
                                          k_means& k_means)
{
  for (int i = 0; i < k_means.getNumOfPoints(); ++i) {
    float p1 = min + (float)(rand()) /
        ((float)(RAND_MAX/(max - min)));
    float p2 = min + (float)(rand()) /
        ((float)(RAND_MAX/(max - min)));
    QVector2D* point = new QVector2D(p1, p2);
    k_means.addPoint(point);
  }

  qDebug() << "Generated Points:" ;
  for (QVector2D* p : k_means.getAllPoints()) {
    qDebug() << p->x() << ", " << p->y();
  }
  qDebug() << " " ;
}

/**
 * @brief initialization::generateNormalDistributionPoints
 * @param min
 * @param max
 * @param k_means
 * This function generates random m_num_points float points between
 * min and max, with a normal distribution
 */
void initialization::generateNormalDistributionPoints(float min, float max,
                                                      k_means& k_means)
{
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(min, max);

  for (int i = 0; i < k_means.getNumOfPoints(); ++i) {
    QVector2D* point = new QVector2D(distr(eng), distr(eng));
    k_means.addPoint(point);
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
 * k_means object and given center. Can easily be extended by
 * adding other functions in distance calculation
 * @return
 */
QVector<float> initialization::getPairwiseDistances(k_means &k_means,
                                                    QVector2D center,
                                                    QString metric, float &min,
                                                    int &min_ind, float &sum)
{
  metrics m;
  // Create an array to store the distances between center and all points
  QVector<float> d(k_means.getNumOfPoints());
  min = FLOAT_MAX;
  min_ind = 0;
  sum = 0;

  for (int i = 0; i < k_means.getNumOfPoints(); ++i) {
    QVector2D* point = k_means.getAllPoints().at(i);

    // These two metrics will return a float value
    if (metric == "euclidean" || metric == "manhattan"){
      float distance = m.getDistance(center, *point, metric);
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

/**
 * @brief initialization::choseWithProb
 * @param prob
 * @return
 * Using the given probabilites, randomly choses
 * one of the instances
 */
int initialization::choseWithProb(const QVector<float> prob)
{
  // Sorting is optional, commented out
  QVector<float> sorted = prob;
  // std::sort(sorted.begin(), sorted.end());
  // qDebug() << "SORTED: " << sorted;

  float rand_x = (float) rand()/RAND_MAX;
  float cum = 0;
  float selected_prob = 0;
  float selected_ind = 0;

  // Use CDF from PDF for random pick
  for (int i = 0; i< prob.size(); i++){
    cum += sorted[i];

    if (rand_x < cum) {
      selected_prob = sorted[i];
      selected_ind = i;
      break;
    }
  }

  // No need this part when it is not sorted
//  int ind = prob.indexOf(selected_prob);
//  int last_ind = prob.lastIndexOf(selected_prob);
//  // If there are more than one points with same probability
//  // break the tie by randomly selecting one of them
//  if (ind != last_ind){
//    ind = (rand() % last_ind) + ind;
//  }

  return selected_ind;
}

//
// ND functions
//

/**
 * @brief initialization::generateRandomPointsND
 * @param min
 * @param max
 * @param k_means
 * N dimensional version of generating random points
 */
void initialization::generateRandomPointsND(float min, float max,
                                            k_means& k_means)
{
  // for each point
  for (int i = 0; i < k_means.getNumOfPoints(); i++) {
    QVector<float>* point = new QVector<float>();
    // each dimension
    for (int i = 0; i < k_means.getDimension(); i++) {
      float p = min + (float)(rand()) /
          ((float)(RAND_MAX/(max - min)));
      point->append(p);
    }
    k_means.addPointND(point);
  }

  qDebug() << "Generated Points:" ;
  for (QVector<float>* p : k_means.getAllPointsND()) {
    qDebug() << *p;
  }
  qDebug() << " " ;
}

/**
 * @brief initialization::generateNormalDistributionPointsND
 * @param min
 * @param max
 * @param k_means
 * N dimensional version of generating normally distributed points
 */
void initialization::generateNormalDistributionPointsND(float min, float max,
                                                        k_means& k_means)
{
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(min, max);
  // for each point
  for (int i = 0; i < k_means.getNumOfPoints(); ++i) {
    QVector<float>* point = new QVector<float>();
    // for each dimension
    for (int j = 0; j < k_means.getDimension(); j++){
      point->append(distr(eng));
    }
    k_means.addPointND(point);
  }
}

/**
 * @brief initialization::initRandomRealND
 * @param k_means
 * N dimensional version of initRandomReal
 */
void initialization::initRandomRealND(k_means &k_means)
{
  k_means::ClusterColor* color = new k_means::ClusterColor();
  QVector<float> * center;
  for (int i = 0; i < k_means.getK(); ++i) {
    center = new QVector<float>();
    for (int j = 0; j< k_means.getDimension(); j++){
      float p = FLOAT_MIN + (float)(rand()) /
          ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
      center->append(p);
    }

    qDebug() << "CENTER: " << center;
    k_means::ClusterND* cluster = new k_means::ClusterND();
    cluster->center = *center;
    cluster->color = color->operator()(i);
    k_means.addClusterND(cluster);
  }
}

/**
 * @brief initialization::initRandomSampleND
 * @param k_means
 * N dimensional version of initRandomSample
 */
void initialization::initRandomSampleND(k_means &k_means)
{
  k_means::ClusterColor* color = new k_means::ClusterColor();
  // Choose m_k centers randomly
  QSet<int> set;
  for (int i = 0; i < k_means.getK(); ++i) {
    int c = rand() % k_means.getNumOfPoints();
    // since we prohibited cases k > number of points, no problem
    while (set.contains(c)) { // Make sure no two selected points are the same
      c = rand() % k_means.getNumOfPoints();
    }
    set.insert(c);
    QVector<float> * center = k_means.getAllPointsND().at(c);

    qDebug() << "CENTER: " << *center;
    // Create cluster
    k_means::ClusterND* cluster = new k_means::ClusterND();
    cluster->center = *center;
    cluster->color = color->operator()(i);
    k_means.addClusterND(cluster);
  }
}

/**
 * @brief initialization::initKMeansPpND
 * @param k_means
 * N dimensional version of kmeans++ initialization implementation
 */
void initialization::initKMeansPpND(k_means &k_means)
{
  QSet<int> set;
  k_means::ClusterColor* color = new k_means::ClusterColor();
  int c = rand() % k_means.getNumOfPoints();
  set.insert(c);
  QVector<float> *center = k_means.getAllPointsND().at(c);

  qDebug() << "CENTER: " << center;
  // Create cluster
  k_means::ClusterND* cluster = new k_means::ClusterND();
  cluster->center = *center;
  cluster->color = color->operator()(0);
  k_means.addClusterND(cluster);

  // for each point get distance between center and that point:
  float min;
  int min_ind;
  float sum;
  QVector<float> d = getPairwiseDistancesND(k_means, *center,
                                            k_means.getMetric(),
                                          min, min_ind, sum);

  for (int i = 1; i < k_means.getK(); ++i) {
    QVector<float> prob = getProbs(d, sum);

    int ind = choseWithProb(prob);
    // since we prohibited cases k > number of points, no problem
    while (set.contains(ind)) {
      ind = choseWithProb(prob);
    }
    set.insert(ind);

    // Get new center point
    center = k_means.getAllPointsND().at(ind);
    k_means::ClusterND* cluster = new k_means::ClusterND();
    cluster->center = *center;
    cluster->color = color->operator()(i);
    k_means.addClusterND(cluster);

    // Get distances between all points and the new center
    QVector<float> d2 = getPairwiseDistancesND(k_means, *center,
                                               k_means.getMetric(),
                                            min, min_ind, sum);

    d = choseMin(d, d2); // select the min (worse) distances for next iteration
  }
}

/**
 * @brief initialization::getPairwiseDistancesND
 * @param k_means
 * @param center
 * @param metric
 * @param min
 * @param min_ind
 * @param sum
 * @return
 * N dimensional version of the pairwise distance calculation function
 * Can easily be extended by
 * adding other functions in distance calculation
 */
QVector<float> initialization::getPairwiseDistancesND(k_means &k_means,
                                 QVector<float> center, QString metric,
                                     float &min, int &min_ind, float &sum)
{
  metrics m;
  // Create an array to store the distances between center and all points
  QVector<float> d(k_means.getNumOfPoints());
  min = FLOAT_MAX;
  min_ind = 0;
  sum = 0;

  for (int i = 0; i < k_means.getNumOfPoints(); ++i) {
    QVector<float>* point = k_means.getAllPointsND().at(i);

    // These two metrics will return a float value
    if (metric == "euclidean" || metric == "manhattan"){
      float distance = m.getDistance(center, *point, metric);
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


