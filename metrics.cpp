#include "metrics.h"
#include <QString>

metrics::metrics()
{

}

// lambda function for euclidean distance of 2 2D points
auto euclidean = [] (QVector2D p1, QVector2D p2) {
  return sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) +
              (p1.y()-p2.y())*(p1.y()-p2.y()));
};

// lambda function for manhattan distance of 2 2D points
auto manhattan = [] (QVector2D p1, QVector2D p2) {
  return qAbs(p1.x()-p2.x()) +
      qAbs(p1.y()-p2.y());
};

// lambda function for euclidean distance of 2 2D points
auto euclidean3D = [] (QVector3D p1, QVector3D p2) {
  return sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) +
              (p1.y()-p2.y())*(p1.y()-p2.y()) +
              (p1.z()-p2.z())*(p1.z()-p2.z()));
};

// lambda function for manhattan distance of 2 2D points
auto manhattan3D = [] (QVector3D p1, QVector3D p2) {
  return qAbs(p1.x()-p2.x()) +
      qAbs(p1.y()-p2.y()) + qAbs(p1.z()-p2.z());
};

// lambda function for euclidean distance of 2 2D points
auto euclideanND = [] (QVector<float> p1, QVector<float> p2) {

  float result = 0;

  // for each dimension
  for (int i = 0; i < p1.size(); i++) {
    result += (p1[i] - p2[i])*(p1[i] - p2[i]);
  }

  return sqrt(result);
};

// lambda function for manhattan distance of 2 2D points
auto manhattanND = [] (QVector<float> p1, QVector<float> p2) {
  float result = 0;

  // for each dimension
  for (int i = 0; i < p1.size(); i++) {
    result += qAbs(p1[i] - p2[i]);
  }

  return result;
};


/**
 * @brief metrics::getDistance
 * @param p1
 * @param p2
 * @param metric
 * @return Distance between 2D points p1 and p2
 * with the metric as m_metric
 * Can easily be extended by adding other functions
 */
float metrics::getDistance(QVector2D p1, QVector2D p2, QString metric)
{
  if (metric == "manhattan") {
    return manhattan(p1, p2);
  } else { // m_metric == "euclidean"
    return euclidean(p1, p2);
  }
}

/**
 * @brief metrics::getDistance
 * @param p1
 * @param p2
 * @param metric
 * @return Distance between 3D points p1 and p2
 * with the metric as m_metric
 * m_metric can be euclidean or manhattan
 */
float metrics::getDistance(QVector3D p1, QVector3D p2, QString metric)
{
  if (metric == "manhattan") {
    return manhattan3D(p1, p2);
  } else { // m_metric == "euclidean"
    return euclidean3D(p1, p2);
  }
}

/**
 * @brief metrics::getDistance
 * @param binary1
 * @param binary2
 * @return Distance between two binary points
 * with Hamming distance metric in any dimension
 */
float metrics::getDistance(QString binary1, QString binary2)
{
  float distance = 0;
  // Cannot get distance if the length of strings are different
  if (binary1.size() == binary2.size()) {
    for (int i=0; i<binary1.size() ; i++) {
      if(binary1[i] != binary2[i]){ // Count not equal characters in same index
        distance += 1;
      }
    }
    return distance;
  }
  return -1;
}

/**
 * @brief metrics::getDistance
 * @param p1
 * @param p2
 * @param metric
 * @return Distance between ND (N>=3) points p1 and p2
 * with the metric as m_metric
 * m_metric can be euclidean or manhattan
 */
float metrics::getDistance(QVector<float> p1, QVector<float> p2, QString metric)
{
  if (metric == "manhattan") {
    return manhattanND(p1, p2);
  } else { // m_metric == "euclidean"
    return euclideanND(p1, p2);
  }
}



