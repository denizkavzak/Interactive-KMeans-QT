#ifndef SCATTER3DVIS_H
#define SCATTER3DVIS_H

#include <QtDataVisualization/Q3DScatter>
#include <QtMath>
#include <QDebug>
#include "k_means.h"
#include "InputHandlerPanning.h"

using namespace QtDataVisualization;

class Scatter3DVis : public QObject
{
  Q_OBJECT
public:  
  explicit Scatter3DVis(Q3DScatter *scatter);

  void addData();
  Q3DScatter* getGraph();
  void addData(k_means &k_m);
  void addDataCenters(k_means &k_m);
  void addSeriesForEachCluster(k_means &k_m);
  void addSeriesForEachClusterPoints(k_means &k_m);
  void updateSeriesForEachCluster(k_means &k_m);
  void clearAllPointsSeriesFromGraph();
  void updatePointSize(int pointSize);

private:
  Q3DScatter *m_graph;
  float m_pointSize;
  int m_numOfPoints;

  void init();
  InputHandlerPanning m_inputHandlerPanning;
  bool m_autoAdjust;

};


#endif // SCATTER3DVIS_H
