#ifndef SCATTER3DVIS_H
#define SCATTER3DVIS_H

#include <QtDataVisualization/Q3DScatter>
#include <QtMath>
#include <QDebug>

using namespace QtDataVisualization;

class Scatter3DVis
{
public:  
  explicit Scatter3DVis(Q3DScatter *scatter);

  void addData();
  Q3DScatter* getGraph();

private:
  Q3DScatter *m_graph;
  float m_pointSize;
  int m_numOfPoints;

};


#endif // SCATTER3DVIS_H
