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

/**
 * @brief metrics::getDistance
 * @param p1
 * @param p2
 * @param metric
 * @return Distance between 2D points p1 and p2
 * with the metric as m_metric
 */
float metrics::getDistance(QVector2D p1, QVector2D p2, QString metric)
{
  if (metric == "manhattan") {
    return manhattan(p1, p2);
    // TODO: Implement hamming for binary features
//  } else if (m_metric == "hamming") {
//    return euclidean(p1, p2);
  } else { // m_metric == "euclidean"
    return euclidean(p1, p2);
  }
}

float metrics::getDistance(QVector3D p1, QVector3D p2, QString metric)
{
  if (metric == "manhattan") {
    return manhattan3D(p1, p2);
    // TODO: Implement hamming for binary features
//  } else if (m_metric == "hamming") {
//    return euclidean(p1, p2);
  } else { // m_metric == "euclidean"
    return euclidean3D(p1, p2);
  }
}

