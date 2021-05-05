#include "Scatter3DVis.h"

Scatter3DVis::Scatter3DVis(Q3DScatter *scatter) :
  m_graph(scatter), m_pointSize(20.0f), m_numOfPoints(900)
{
  init();
}

void Scatter3DVis::init()
{
  m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
  QFont font = m_graph->activeTheme()->font();
  font.setPointSize(m_pointSize);
  m_graph->activeTheme()->setFont(font);
  m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
  m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

  // Add one series that will store all points
  QScatterDataProxy *proxy = new QScatterDataProxy;
  QScatter3DSeries *series = new QScatter3DSeries(proxy);
  series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
  //series->setMeshSmooth(m_smooth);
  m_graph->addSeries(series);

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

void Scatter3DVis::addDataCenters(k_means &k_m)
{
  qDebug() << "Inside add centers";
  int ind = 1;
  for (k_means::ClusterND* cluster : k_m.getClustersND()) {
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(1); // We only have k centers
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    qDebug() << "Inside loop";
    ptrToDataArray->setPosition(QVector3D(cluster->center.at(0) ,
                                          cluster->center.at(1),
                                          cluster->center.at(2)));

    ptrToDataArray++;

    qDebug() << QVector3D(cluster->center.at(0) ,
                          cluster->center.at(1),
                          cluster->center.at(2));

    qDebug() << "from cluster " << *(cluster->color);
    // ind starts from 1 since all points are still in seriesList().at(0)
    //m_graph->seriesList().at(0)->dataProxy()->removeItems();
    m_graph->seriesList().at(ind)->dataProxy()->resetArray(dataArray);
    m_graph->seriesList().at(ind)->setBaseColor(*(cluster->color));
    qDebug() << "series color" << m_graph->seriesList().at(ind)->baseColor();
    ind++;
  }
}

void Scatter3DVis::addSeriesForEachCluster(k_means &k_m)
{
  // First series will be for all points, rest will be for clusters
  for (int i=0; i < k_m.getK(); i++)
  {
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    //series->setMeshSmooth(m_smooth);
    m_graph->addSeries(series);
  }
}
