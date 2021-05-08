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

  // Cluster struct for 2D points
  struct Cluster{
    QVector2D center;
    QVector<QVector2D> cluster_points;
    QColor* color;
  };

  // Cluster strcut for ND points (N>=3)
  struct ClusterND{
    QVector<float> center;
    QVector<QVector<float>> cluster_points;
    QColor* color;
  };

  /**
  // Struct to assign a different color for each cluster
  // Currently it has 14 colors,
  // can be extended using random colors in next version
  **/
  struct ClusterColor{
    ClusterColor() { }

    // TODO: It has 14 colors currently, update it to be continuous colors
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
  bool isConverged();

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

  // kmeans functions
  void clusterPoints(int num_iterations);
  void moveOneStep();
  void finalizeOneStep();

  // helper functions
  void clearClusterPoints();
  void addCluster(Cluster* cluster);
  void addPoint(QVector2D* point);
  void printClusters();
  void updateCenters();

  // back step functions
  void updateClusteringCenterHistory();
  void printClusteringCenterHistory();
  void setClusterCentersToPrevStepInHistory();
  void initClusterCentersHistory();

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
  void setPoints();
  void setPointsND();
  void updateCentersND();

  // helper functions
  void clearClusterPointsND();
  void addClusterND(ClusterND* cluster);
  void addPointND(QVector<float>* point);
  void printClustersND();

  // back step functions
  void updateClusteringCenterHistoryND();
  void printClusteringCenterHistoryND();
  void setClusterCentersToPrevStepInHistoryND();
  void initClusterCentersHistoryND();

  // for measuring performance
  void detectConvergence();
  void calculateEnergy();

private:

  QVector<QVector2D*> m_allPoints;
  int m_num_points;
  int m_k;
  QVector<Cluster*> m_clusters;

  // for ND
  QVector<QVector<float>*> m_allPointsND;
  QVector<ClusterND*> m_clustersND;
  QVector<QVector<QVector2D*>> *m_clusteringCenterHistory; // store all clustering history
  QVector<QVector<QVector<float>>> *m_clusteringCenterHistoryND; // store all clustering history
  // <#iteration<#cluster<N dimensional point vector
  // each iteration < each cluster < center point > >
  QString m_metric;
  int m_step = 0;
  int m_iter = 0;
  bool m_initialized = false;
  int m_dim = 2;
  bool m_pointsSet = false;
  bool m_back_clicked = false;
  bool m_converged = false;
};

#endif // KMEANS_H
