#ifndef KMEANS_H
#define KMEANS_H

#include <QPoint>
#include <QVector>
#include <QVector2D>
#include <QColor>

class k_means
{
public:
  k_means();
  k_means(int num_points);
  k_means(int num_points, int k, float min, float max);
  k_means(QVector<QVector2D*> points, int k);

  k_means(QVector<QVector<float>*> points, int k); // for ND

  struct Cluster{
    QVector2D center;
    QVector<QVector2D> cluster_points;
    QColor* color;
  };

  struct ClusterND{
    QVector<float> center;
    QVector<QVector<float>> cluster_points;
    QColor* color;
  };

  struct ClusterColor{
    ClusterColor() { }

    // TODOO: It has 15 colors currently, update it to be continuous colors
    QVector<QColor> colorArray = {Qt::red, Qt::blue,
                                  Qt::magenta, Qt::green,
                                  Qt::gray, Qt::cyan, Qt::darkBlue, Qt::darkRed,
                                  Qt::darkGreen, Qt::darkCyan, Qt::darkYellow,
                                  Qt::darkMagenta, Qt::darkGray, Qt::yellow};
    // QColor a(255,0,255,0); // Can have random color but will it be distinctive?


    QColor* operator()(int i){
      // modulo for making repeated pattern when pattern image is smaller
      return &(colorArray[i % colorArray.size()]);
    }
  };

  // common getters
  int getNumOfPoints();
  int getK();
  QString getMetric();
  int getNumOfIter();
  bool isInitialized();
  int getDimension();

  // common setters
  void setNumOfIter(int numOfIterations);
  void setInitialized(bool);
  void setStep(int step);
  void setK(int k);
  void setMetric(QString metric);
  void setNoOfPoints(int num_points);
  void setDimension(int dim);

  //
  // 2D functions
  //

  // getters
  QVector<Cluster*> getClusters();
  QVector<QVector2D*> getAllPoints();
  QVector2D getCenters();
  QVector<QVector2D*> getPrevClusterCenters();

  // kmeans functions
  void clusterPoints(int num_iterations);
  void moveOneStep();
  void finalizeOneStep();

  // helper functions
  void setClusterCentersToPrev();
  void clearClusterPoints();
  void addCluster(Cluster* cluster);
  void addPoint(QVector2D* point);
  void updatePrevClusterCenters();
  void printClusters();

  //
  // functions for ND
  //

  // getters
  QVector<QVector<float>*> getAllPointsND();
  QVector<ClusterND*> getClustersND();
  bool isPointsSet();
  void setPointsSet(bool set);

  // kmeans functions
  void clusterPointsND(int num_iterations);
  void moveOneStepND();
  void finalizeOneStepND();

  // helper functions
  void setClusterCentersToPrevND();
  void clearClusterPointsND();
  void addClusterND(ClusterND* cluster);
  void addPointND(QVector<float>* point);
  void updatePrevClusterCentersND();
  void printClustersND();

private:
  void setPoints();
  void updateCenters();

  // for ND
  void setPointsND();
  void updateCentersND();

  QVector<QVector2D*> m_allPoints;
  int m_num_points;
  int m_k;
  QVector<Cluster*> m_clusters;
  QVector<Cluster*> m_PreviousClusters;
  QVector<QVector2D*> m_previousCenters;

  // for ND
  QVector<QVector<float>*> m_allPointsND;
  QVector<ClusterND*> m_clustersND;
  QVector<QVector<float>*> m_previousCentersND;

  QString m_metric;
  int m_step = 0;
  int m_iter = 0;
  bool m_initialized = false;
  int m_dim = 2;
  bool m_pointsSet = false;
};

#endif // KMEANS_H
