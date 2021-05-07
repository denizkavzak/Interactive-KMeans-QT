#include "k_means.h"
#include <iostream>
#include <random>
#include <QMap>
#include <QtMath>
#include <QVector2D>
#include <QSet>
#include <QDebug>
#include <initialization.h>
#include <metrics.h>

/**
 * @brief k_means::k_means
 * Default constructor
 */
k_means::k_means()
{
  m_num_points = 1000;
  m_metric = "euclidean";
  m_k = 3;
  m_dim = 3;
}

/**
 * @brief k_means::k_means
 * @param num_points
 * Constructor for k-means class
 * taking only number of points
 */
k_means::k_means(int num_points){
  m_num_points = num_points;
  m_metric = "euclidean";
}

/**
 * @brief k_means::k_means
 * @param num_points: number of points to be generated randomly
 * @param k: number of clusters
 * Constructor for k-means class
 * Generates random points to fill m_num_points
 */
k_means::k_means(int num_points, int k, float min, float max)
{
  m_num_points = num_points;
  m_k = k;
  initialization in;
  in.generateRandomPoints(min, max, *this);
  m_metric = "euclidean";
}

/**
 * @brief k_means::k_means
 * @param points: points to be clustered
 * @param k: number of clusters
 * Constructor for k-means class with points dimension 2D
 */
k_means::k_means(QVector<QVector2D*> points, int k)
{
  m_allPoints = points;
  m_num_points = m_allPoints.size();
  m_k = k;
  m_metric = "euclidean";
}

/**
 * @brief k_means::k_means
 * @param points
 * @param k
 * Constructor for k-means class with points dimension >=3
 */
k_means::k_means(QVector<QVector<float> *> points, int k)
{
  m_allPointsND = points;
  m_num_points = m_allPointsND.size();
  m_k = k;
  m_metric = "euclidean";
}

/**
 * @brief k_means::printClusters
 * This function prints the clusters by printing the
 * number of points in the cluster, the center of the cluster,
 * and the points in the cluster
 */
void k_means::printClusters()
{
  qDebug() << "Clusters : ";
  int id = 0;
  for (k_means::Cluster* cluster : m_clusters) {
    qDebug() << "Cluster " << id << " - size: " << cluster->cluster_points.size();
    qDebug() << "Center: " << cluster->center.x() << ", "
             << cluster->center.y();
    for (QVector2D p : cluster->cluster_points) {
      qDebug() << p.x() << ", " << p.y();
    }
    qDebug() << " " ;
    id += 1;
  }
}

/**
 * @brief k_means::setK
 * @param k
 * Setter function for K attribute
 */
void k_means::setK(int k)
{
  m_k = k;
}

/**
 * @brief k_means::setMetric
 * @param metric
 * Setter for distance metric attribute
 */
void k_means::setMetric(QString metric)
{
  m_metric = metric;
}

/**
 * @brief k_means::setNoOfPoints
 * @param num_points
 * Setter for the number of points attribute
 */
void k_means::setNoOfPoints(int num_points)
{
  m_num_points = num_points;
}

/**
 * @brief k_means::setDimension
 * @param dim
 * Setter for the dimension attribute
 */
void k_means::setDimension(int dim)
{
  m_dim = dim;
}

/**
 * @brief k_means::getDimension
 * @return
 * Getter for the dimension attribute
 */
int k_means::getDimension()
{
  return m_dim;
}

/**
 * @brief k_means::getClusters
 * @return clusters
 * Getter for the clusters of 2D
 */
QVector<k_means::Cluster*> k_means::getClusters()
{
  return m_clusters;
}

/**
 * @brief k_means::getAllPoints
 * @return
 * Getter for all generated/imported points of 2D
 */
QVector<QVector2D*> k_means::getAllPoints()
{
  return m_allPoints;
}

/**
 * @brief k_means::getNumOfPoints
 * @return
 * Getter for the number of points attribute
 */
int k_means::getNumOfPoints()
{
  return m_num_points;
}

/**
 * @brief k_means::getK
 * @return
 * Getter for the K attribute
 */
int k_means::getK()
{
  return m_k;
}

/**
 * @brief k_means::getMetric
 * @return
 * Getter for the distance metric attribute
 */
QString k_means::getMetric()
{
  return m_metric;
}

///**
// * @brief k_means::getCenters
// * @return
// * Returns the centers of each cluster in a vector in 2D
// */
//QVector2D k_means::getCenters()
//{
//  QVector2D centers;
//  for (k_means::Cluster* cluster : m_clusters) {
//    centers += cluster->center;
//  }
//  return centers;
//}

void k_means::addCluster(k_means::Cluster *cluster)
{
  m_clusters += cluster;
}

/**
 * @brief k_means::addPoint
 * @param point
 * Add a point to the all points 2D vector
 */
void k_means::addPoint(QVector2D* point)
{
  m_allPoints += point;
}

/**
 * @brief k_means::setPoints
 * This function assigns points to the corresponding clusters
 * by using the distance and current cluster centers in 2D
 */
void k_means::setPoints()
{
  clearClusterPoints();
  metrics m;
  // Point assignment
  for (int i = 0; i < m_num_points; ++i) {
    // for each point:
    QVector2D* point = m_allPoints.at(i);
    float min = FLT_MAX;
    int min_ind = 0;
    int ind = 0;

    // find the nearest center point
    for (k_means::Cluster* cluster : m_clusters) {
      QVector2D center = cluster->center;
      float distance = m.getDistance(*point, center, m_metric);
      if (distance < min) {
        min = distance;
        min_ind = ind;
      }
      ind += 1;
    }
    // assign the point to that cluster
    m_clusters[min_ind]->cluster_points += *point;
  }
}

/**
 * @brief k_means::updateCenters
 * This function updates the cluster centers by using the mean of the
 * points in the cluster in 2D
 */
void k_means::updateCenters()
{
  // Cluster center update
  // for each cluster center 0...m_k
  int ind = 0;
  for (k_means::Cluster* cluster : m_clusters) {
    // new center = mean of all points assigned to that cluster
    if (cluster->cluster_points.size()!=0){ // check if cluster has any points
      float x = 0;
      float y = 0;
      for (QVector2D cluster_point : cluster->cluster_points) {
        x += cluster_point.x();
        y += cluster_point.y();
      }

      m_clusters[ind]->center = QVector2D(x/cluster->cluster_points.size(),
                                          y/cluster->cluster_points.size());

    }
    ind += 1;
  }
}

/**
 * @brief k_means::clusterPoints
 * @param num_iterations
 * This function implements a simple k-means clustering algorithm
 * using given number of iterations as termination criteria
 * Updates m_clusters with corresponding centers and clustered points
 * This function works for 2D data only
 */
void k_means::clusterPoints(int num_iterations)
{
  // TODO: check for edge cases - k=0, k=1, k>num_points
  // TODO: optimize code for better performance
  qDebug() << "num iter: " << num_iterations;
  qDebug() << " " ;
  qDebug() << "Iterations Start!" ;
  qDebug() << " " ;

  //m_step = 0;
  // loop in num_iterations
  for (int j = m_step; j < num_iterations; ++j) {
    moveOneStep();
    finalizeOneStep();
  }

  printClusteringCenterHistory();
}

/**
 * @brief k_means::moveOneStep
 * One step of setting points to their current clusters in 2D
 */
void k_means::moveOneStep()
{
  qDebug() << "move one step";
  // Point assignment
  setPoints();

  if (m_step != m_iter && m_back_clicked != true) {
    updateClusteringCenterHistory();
    printClusteringCenterHistory();
  }
  m_back_clicked = false;

  qDebug() << "Iteration : " << m_step;
  printClusters();
}

/**
 * @brief k_means::finalizeOneStep
 * Final part of one step where centers are updated in 2D
 */
void k_means::finalizeOneStep()
{
  //updatePrevClusterCenters();
  // Cluster center update
  // for each cluster center 0...m_k
  updateCenters();
  m_step += 1;
  if (m_step == m_iter) {
    // Final Point assignment
    qDebug() << "/------------Final Clusters---------------/: ";
    moveOneStep();
  }
}

/**
 * @brief k_means::setNumOfIter
 * @param numOfIterations
 * Setter for the number of iterations attribute
 */
void k_means::setNumOfIter(int numOfIterations)
{
  m_iter = numOfIterations;
}

/**
 * @brief k_means::getNumOfIter
 * @return
 * Getter for the number of iterations attribute
 */
int k_means::getNumOfIter()
{
  return m_iter;
}

/**
 * @brief k_means::setInitialized
 * @param init
 * Setter for the initialized flag attribute
 */
void k_means::setInitialized(bool init)
{
  m_initialized = init;
}

/**
 * @brief k_means::isInitialized
 * @return
 * Getter for the initialized flag attribute
 */
bool k_means::isInitialized()
{
  return m_initialized;
}

/**
 * @brief k_means::setStep
 * @param step
 * Setter for step attribute indicating the current
 * step (iteration) during clustering
 */
void k_means::setStep(int step)
{
  m_step = step;
}

/**
 * @brief k_means::clearClusterPoints
 * Clears the points assigned to each cluster
 * leaving only centers in each cluster in 2D
 */
void k_means::clearClusterPoints()
{
  for (k_means::Cluster* cluster : m_clusters) {
    cluster->cluster_points.clear();
  }
}

//
// ND Functions
//

/**
 * @brief k_means::addPointND
 * @param point
 * Add a point to the all points ND (N>=3) vector
 */
void k_means::addPointND(QVector<float> *point)
{
  m_allPointsND += point;
}

/**
 * @brief k_means::getAllPointsND
 * @return
 * Getter for all generated/imported points in ND (N>=3)
 */
QVector<QVector<float> *> k_means::getAllPointsND()
{
  return m_allPointsND;
}

/**
 * @brief k_means::printClustersND
 * Prints cluster information for ND (N>=3)
 */
void k_means::printClustersND()
{
  qDebug() << "Clusters : ";
  int id = 0;
  for (k_means::ClusterND* cluster : m_clustersND) {
    qDebug() << "Cluster " << id << " - size: " << cluster->cluster_points.size();
    qDebug() << "Center: " << cluster->center;
    for (QVector<float> p : cluster->cluster_points) {
      qDebug() << p;
    }
    qDebug() << " " ;
    id += 1;
  }
}

/**
 * @brief k_means::updateClusteringCenterHistoryND
 * Update clustering center history with current
 * cluster centers in current iteration for ND (N>=3)
 */
void k_means::updateClusteringCenterHistoryND()
{
  QVector<QVector<float>*> centers; // <numOfclusters<dimension
  for (int i = 0; i < m_clustersND.size(); i++) { // numOfClusters
    QVector<float>* p = new QVector<float>(m_clustersND.at(i)->center);
    centers.append(p);
  }
  m_clusteringCenterHistoryND->replace(m_step, centers);
}

/**
 * @brief k_means::printClusteringCenterHistoryND
 * Print cluster centers history for ND (N>=3)
 */
void k_means::printClusteringCenterHistoryND()
{
  qDebug() << "PREV HISTORY: ";
  for (int i = 0; i < m_clusteringCenterHistoryND->size(); i++) {
    qDebug() << "iteration " << i;
    for (int j = 0; j < m_clusteringCenterHistoryND->at(i).size(); j++) {
      qDebug() << *m_clusteringCenterHistoryND->at(i).at(j);
    }
  }
}

/**
 * @brief k_means::setClusterCentersToPrevStepInHistoryND
 * Sets current cluster centers to the
 * stored previous step cluster center history in ND (N>=3)
 * Used for back step feature during clustering
 */
void k_means::setClusterCentersToPrevStepInHistoryND()
{
  m_step -= 1;
  QVector<QVector<float>*> prev = m_clusteringCenterHistoryND->at(m_step - 1);
  for (int i = 0; i < m_clustersND.size(); i++) {
    QVector<float> p(*prev.at(i)); // get previous center point
    qDebug() << m_clustersND[i]->center  << " set to " << p;
    m_clustersND[i]->center = p;
  }
  m_back_clicked = true;
}

/**
 * @brief k_means::initClusterCentersHistoryND
 * Initialize cluster centers history by number of iterations for ND (N>=3)
 */
void k_means::initClusterCentersHistoryND()
{
  m_clusteringCenterHistoryND = new QVector<QVector<QVector<float>*>>(m_iter);
  printClusteringCenterHistoryND();
}

/**
 * @brief k_means::updateClusteringCenterHistory
 * Update clustering center history with current
 * cluster centers in current iteration
 * Used for back step feature during clustering in ND (N>=3)
 */
void k_means::updateClusteringCenterHistory()
{
  QVector<QVector2D*> centers;
  for (int i = 0; i < m_clusters.size(); i++) {
    QVector2D *p = new QVector2D(m_clusters.at(i)->center.x(),
                                 m_clusters.at(i)->center.y());
    centers.append(p);
  }
  m_clusteringCenterHistory->replace(m_step, centers);
}

/**
 * @brief k_means::printClusteringCenterHistory
 * Print cluster centers history
 */
void k_means::printClusteringCenterHistory()
{
  qDebug() << "PREV HISTORY: ";
  for (int i = 0; i < m_clusteringCenterHistory->size(); i++) {
    qDebug() << "iteration " << i;
    for (int j = 0; j < m_clusteringCenterHistory->at(i).size(); j++) {
      qDebug() << *m_clusteringCenterHistory->at(i).at(j);
    }
  }
}

/**
 * @brief k_means::setClusterCentersToPrevStepInHistory
 * Sets current cluster centers to the
 * stored previous cluster centers from the history.
 * Used for back step feature during clustering in 2D
 */
void k_means::setClusterCentersToPrevStepInHistory()
{
  m_step -= 1;
  QVector<QVector2D*> prev = m_clusteringCenterHistory->at(m_step - 1);
  for (int i = 0; i < m_clusters.size(); i++) {
    QVector2D p(prev.at(i)->x(), prev.at(i)->y());
    m_clusters[i]->center = p;
  }
  m_back_clicked = true;
}

/**
 * @brief k_means::initClusterCentersHistory
 * Initialize cluster centers history by number of iterations
 */
void k_means::initClusterCentersHistory()
{
  m_clusteringCenterHistory = new QVector<QVector<QVector2D*>>(m_iter);
}


/**
 * @brief k_means::getClustersND
 * @return
 * Getter for clusters in ND (N>=3)
 */
QVector<k_means::ClusterND*> k_means::getClustersND()
{
  return m_clustersND;
}

/**
 * @brief k_means::clusterPointsND
 * @param num_iterations
 * Kmeans clustering algorithm for ND points (N>=3)
 * Runs the clustering all at once without going step by step
 */
void k_means::clusterPointsND(int num_iterations)
{
  // TODO: check for edge cases - k=0, k=1, k>num_points
  // TODO: optimize code for better performance
  qDebug() << "num iter: " << num_iterations;
  qDebug() << " " ;
  qDebug() << "Iterations Start!" ;
  qDebug() << " " ;

  //m_step = 0;
  // loop in num_iterations
  for (int j = m_step; j < num_iterations; ++j) {
    moveOneStepND();
    finalizeOneStepND();
  }

  printClusteringCenterHistoryND();
}

/**
 * @brief k_means::moveOneStepND
 * One step of setting points to their current clusters for ND (D>=3)
 */
void k_means::moveOneStepND()
{
  qDebug() << "move one step";
  // Point assignment
  setPointsND();

  if (m_step != m_iter && m_back_clicked != true) {
    updateClusteringCenterHistoryND();
    printClusteringCenterHistoryND();
  }
  m_back_clicked = false;

  qDebug() << "Iteration : " << m_step;
  printClustersND();
}

/**
 * @brief k_means::finalizeOneStepND
 * Final part of one step where centers are updated in ND (N>=3)
 */
void k_means::finalizeOneStepND()
{
  // Cluster center update
  // for each cluster center 0...m_k
  updateCentersND();
  m_step += 1;
  if (m_step == m_iter) {
    // Final Point assignment
    qDebug() << "/------------Final Clusters---------------/: ";
    moveOneStepND();
  }
  m_pointsSet = true;
}

/**
 * @brief k_means::setPointsND
 * This function assigns points to the corresponding clusters
 * by using the distance and current cluster centers in ND (D>=3)
 */
void k_means::setPointsND()
{
  clearClusterPointsND();
  metrics m;
  // Point assignment
  for (int i = 0; i < m_num_points; ++i) {
    // for each point:
    QVector<float>* point = m_allPointsND.at(i);
    float min = FLT_MAX;
    int min_ind = 0;
    int ind = 0;

    // find the nearest center point
    for (k_means::ClusterND* cluster : m_clustersND) {
      QVector<float> center = cluster->center;
      float distance = m.getDistance(*point,center,m_metric);
      if (distance < min) {
        min = distance;
        min_ind = ind;
      }
      ind += 1;
    }
    // assign the point to that cluster
    m_clustersND[min_ind]->cluster_points += *point;
  }

}

/**
 * @brief k_means::clearClusterPointsND
 * Clears the points assigned to each cluster
 * leaving only centers in each cluster in ND (N>=3)
 */
void k_means::clearClusterPointsND()
{
  for (k_means::ClusterND* cluster : m_clustersND) {
    //qDebug() << "PREV CLUSTER CLEAR:" << cluster->cluster_points.size();
    cluster->cluster_points.clear();
    //qDebug() << "AFTER CLUSTER CLEAR:" << cluster->cluster_points.size();
  }
}

/**
 * @brief k_means::addClusterND
 * @param cluster
 * Adds a cluster to the clusters attribute in ND (N>=3)
 */
void k_means::addClusterND(k_means::ClusterND *cluster)
{
  m_clustersND += cluster;
}

/**
 * @brief k_means::updateCentersND
 * This function updates the cluster centers by using the mean of the
 * points in the cluster in ND (N>=3)
 */
void k_means::updateCentersND()
{
  // Cluster center update
  // for each cluster center 0...m_k
  int ind = 0;
  for (k_means::ClusterND* cluster : m_clustersND) {
    // new center = mean of all points assigned to that cluster
    if (cluster->cluster_points.size() != 0){ // check if cluster has any points
      QVector<float> p(m_dim, 0.0);
      for (QVector<float> cluster_point : cluster->cluster_points) {
        for (int i = 0; i < m_dim; i++) {
          p[i] += cluster_point[i];
        }
      }

      for (int i = 0; i < m_dim; i++) {
        p[i] = p[i] / cluster->cluster_points.size();
      }

      m_clustersND[ind]->center = p;
      //m_clustersND[ind]->cluster_points.clear();
    }
    ind += 1;
  }
}

/**
 * @brief k_means::isPointsSet
 * @return
 * Getter for the points set flag attribute
 * This attribute keeps track of initial points being set (assigned)
 * for 3D visualization to decide on adding more series to the graph
 */
bool k_means::isPointsSet()
{
  return m_pointsSet;
}

/**
 * @brief k_means::setPointsSet
 * @param set
 * Setter for the points set flag attribute
 */
void k_means::setPointsSet(bool set)
{
  m_pointsSet = set;
}
