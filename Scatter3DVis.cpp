#include "Scatter3DVis.h"

Scatter3DVis::Scatter3DVis(Q3DScatter *scatter) :
  m_graph(scatter), m_pointSize(20.0f), m_numOfPoints(900)
{

  //! [0]
  m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
  QFont font = m_graph->activeTheme()->font();
  font.setPointSize(m_pointSize);
  m_graph->activeTheme()->setFont(font);
  m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
  m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
  //! [0]

  //! [2]
  QScatterDataProxy *proxy = new QScatterDataProxy;
  QScatter3DSeries *series = new QScatter3DSeries(proxy);
  series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
  //series->setMeshSmooth(m_smooth);
  m_graph->addSeries(series);
  //! [2]

  //! [3]
  //addData();
  //! [3]

}

void Scatter3DVis::addData()
{
  // Configure the axes according to the data
  //! [4]
  m_graph->axisX()->setTitle("X");
  m_graph->axisY()->setTitle("Y");
  m_graph->axisZ()->setTitle("Z");
  //! [4]
  //! [5]
  QScatterDataArray *dataArray = new QScatterDataArray;
  dataArray->resize(m_numOfPoints);
  QScatterDataItem *ptrToDataArray = &dataArray->first();
  //! [5]

#ifdef RANDOM_SCATTER
  for (int i = 0; i < m_itemCount; i++) {
      ptrToDataArray->setPosition(randVector());
      ptrToDataArray++;
  }
#else

  //! [6]
  float limit = qSqrt(m_numOfPoints) / 2.0f;
  for (float i = -limit; i < limit; i++) {
      for (float j = -limit; j < limit; j++) {
          ptrToDataArray->setPosition(QVector3D(i + 0.5f,
                                                qCos(qDegreesToRadians((i * j) / 0.75f)),
                                                j + 0.5f));
          ptrToDataArray++;
      }
  }
  //! [6]
#endif

  qDebug() << " In add data ";
  //! [7]
  m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
  //! [7]

  qDebug() << " end of add data ";
}

Q3DScatter *Scatter3DVis::getGraph()
{
  return m_graph;
}

void Scatter3DVis::addData(k_means &k_m)
{
  // Configure the axes according to the data
  m_graph->axisX()->setTitle("X");
  m_graph->axisY()->setTitle("Y");
  m_graph->axisZ()->setTitle("Z");

  QScatterDataArray *dataArray = new QScatterDataArray;
  dataArray->resize(m_numOfPoints);
  QScatterDataItem *ptrToDataArray = &dataArray->first();

  for (QVector<float> * point : k_m.getAllPointsND()){
    ptrToDataArray->setPosition(QVector3D(point->at(0), point->at(1),
                                          point->at(2)));
    ptrToDataArray++;
  }

  m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
}
