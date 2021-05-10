#include "Scatter3DVis.h"

Scatter3DVis::Scatter3DVis(Q3DScatter *scatter) :
  m_graph(scatter), m_pointSize(20.0f), m_numOfPoints(900),
  m_inputHandlerPanning(new InputHandlerPanning(m_graph)), m_autoAdjust(false)
{
  init();
}

/**
 * @brief Scatter3DVis::init
 * Initialize the graph by assigning initial parameters and add a single series
 * to the graph series, which will be used to store all the generated/imported
 * points later by using addData.
 */
void Scatter3DVis::init()
{
  m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
  m_graph->activeTheme()->setLabelBorderEnabled(true);
  QFont font = m_graph->activeTheme()->font();
  font.setPointSize(m_pointSize);
  m_graph->activeTheme()->setFont(font);
  m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
  m_graph->scene()->activeCamera()->setCameraPreset(
        Q3DCamera::CameraPresetFront);

  m_graph->setActiveInputHandler(m_inputHandlerPanning);
  m_inputHandlerPanning->setAxes(m_graph->axisX(), m_graph->axisZ(),
                                 m_graph->axisY());

  // Add one series that will store all points
  QScatterDataProxy *proxy = new QScatterDataProxy;
  QScatter3DSeries *series = new QScatter3DSeries(proxy);
  series->setItemLabelFormat(
        QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
  //series->setMeshSmooth(m_smooth);
  m_graph->addSeries(series);
}

/**
 * @brief Scatter3DVis::getGraph
 * @return
 * Getter for 3DScatter graph
 */
Q3DScatter* Scatter3DVis::getGraph()
{
  return m_graph;
}

/**
 * @brief Scatter3DVis::addData
 * @param k_m
 * This function adds all the points from the generated/imported data
 * into the graph by appending all to the index 0 series in the
 * graph series.
 */
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


/**
 * @brief Scatter3DVis::addDataCenters
 * @param k_m
 * Sets cluster centers in graph using respective series for each
 * all points are in seriesList().at(0)
 * All cluster centers are from index 1....k
 */
void Scatter3DVis::addDataCenters(k_means &k_m)
{
  int ind = 1;
  for (k_means::ClusterND* cluster : k_m.getClustersND()) {
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(1);
    QScatterDataItem *ptrToDataArray = &dataArray->first();

    ptrToDataArray->setPosition(QVector3D(cluster->center.at(0),
                                          cluster->center.at(1),
                                          cluster->center.at(2)));

    ptrToDataArray++;

    qDebug() << QVector3D(cluster->center.at(0) ,
                          cluster->center.at(1),
                          cluster->center.at(2));

    // ind starts from 1 since all points are still in seriesList().at(0)
    //m_graph->seriesList().at(0)->dataProxy()->removeItems();
    m_graph->seriesList().at(ind)->dataProxy()->resetArray(dataArray);
    m_graph->seriesList().at(ind)->setBaseColor(*(cluster->color));
    m_graph->seriesList().at(ind)->setMesh(QAbstract3DSeries::MeshCube);

    ind++;
  }
}

/**
 * @brief Scatter3DVis::addSeriesForEachCluster
 * @param k_m
 * Add series for each cluster center to the graph (k series for k centers)
 */
void Scatter3DVis::addSeriesForEachCluster(k_means &k_m)
{
  // First series will be for all points, rest will be for clusters
  for (int i=0; i < k_m.getK(); i++)
  {
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(
          QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    //series->setMeshSmooth(m_smooth);
    m_graph->addSeries(series);
  }
}

/**
 * @brief Scatter3DVis::addSeriesForEachClusterPoints
 * @param k_m
 * Add series for points of each cluster (k series for k set of points)
 */
void Scatter3DVis::addSeriesForEachClusterPoints(k_means &k_m)
{
  // First series will be for all points, next k will be for cluster centers
  // Then next k will be for the actual points of each cluster
  // Cluster center in series index i's points will be in series index k+i+1
  // There will be in total 2k + 1 series
  for (int i=0; i < k_m.getK(); i++)
  {
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(
          QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    //series->setMeshSmooth(m_smooth);
    m_graph->addSeries(series);
  }
}

/**
 * @brief Scatter3DVis::updateSeriesForEachCluster
 * @param k_m
 * Add cluster points into the respective series in the graph
 * Index 0 holds all the points
 * Indexes 1....k holds the cluster centers
 * Indexes k+1......2k+1 holds the corresponding cluster points
 * This function need to be called together with
 * addDataCenters to update both centers and respective points
 * in the graph
 */
void Scatter3DVis::updateSeriesForEachCluster(k_means &k_m)
{
  // First series will be for all points, next k will be for cluster centers
  // Then next k will be for the actual points of each cluster
  // Cluster center in series index i's points will be in series index k+i+1
  // There will be in total 2k + 1 series
  qDebug() << "Inside update series for each cluster";
  int ind = 1;

  for (k_means::ClusterND* cluster : k_m.getClustersND()) {

    if (cluster->cluster_points.size() != 0) {
      m_graph->seriesList().at(0)->setVisible(false);

      QScatterDataArray *dataArray = new QScatterDataArray;
      dataArray->resize(cluster->cluster_points.size()); // size of cluster pts
      QScatterDataItem *ptrToDataArray = &dataArray->first();

      for (QVector<float> point : cluster->cluster_points) {

        ptrToDataArray->setPosition(QVector3D(point.at(0),
                                              point.at(1),
                                              point.at(2)));
        ptrToDataArray++;
      }

      m_graph->seriesList().at(ind + k_m.getK())->dataProxy()->resetArray(
            dataArray);
      m_graph->seriesList().at(ind + k_m.getK())->setBaseColor(
            *(cluster->color));

      ind++;
    }
  }
}

/**
 * @brief Scatter3DVis::clearAllPointsSeriesFromGraph
 * This function clears all points that initially
 * generated/imported from the graph, which are hold at
 * index 0 of series of the graph.
 */
void Scatter3DVis::clearAllPointsSeriesFromGraph()
{
  m_graph->seriesList().at(0)->setVisible(false);
}

/**
 * @brief Scatter3DVis::updatePointSize
 * @param pointSize
 * Updates the point size in the graph
 * pointSize being 0 means the size of points will be automatically
 * determined by the graph itself. Any value taken from the spinbox from
 * ui and passed into this function is divided by 100, assuming the
 * maximum selectable size is 100. The reason is the scatter graph
 * sizes are between 0.0f and 1.0f.
 */
void Scatter3DVis::updatePointSize(int pointSize)
{
  for (int i = 0; i < m_graph->seriesList().size(); i++) {
    m_graph->seriesList().at(i)->setItemSize(float(pointSize) / 100.0f);
  }
}

void Scatter3DVis::clearAll()
{
  m_graph->seriesList().clear();
}

