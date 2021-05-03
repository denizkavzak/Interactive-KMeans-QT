#include "Scatter3DVis.h"

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

Scatter3DVis::Scatter3DVis()
{

}

Scatter3DVis::Scatter3DVis(QtDataVisualization::Q3DScatter *scatter)
    : m_graph(scatter),
      m_fontSize(40.0f),
      m_style(QtDataVisualization::QAbstract3DSeries::MeshSphere),
      m_smooth(true),
      m_itemCount(lowerNumberOfItems),
      m_curveDivider(lowerCurveDivider)
{

//  //! [0]
//  m_graph->activeTheme()->setType(QtDataVisualization::Q3DTheme::ThemeEbony);
//  QFont font = m_graph->activeTheme()->font();
//  font.setPointSize(m_fontSize);
//  m_graph->activeTheme()->setFont(font);
//  m_graph->setShadowQuality(QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftLow);
//  m_graph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetFront);
//  //! [0]

//  //! [2]
//  QtDataVisualization::QScatterDataProxy *proxy = new QtDataVisualization::QScatterDataProxy;
//  QtDataVisualization::QScatter3DSeries *series = new QtDataVisualization::QScatter3DSeries(proxy);
//  series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
//  series->setMeshSmooth(m_smooth);
//  m_graph->addSeries(series);
//  //! [2]

//  //! [3]
//  addData();
//  //! [3]

}

void Scatter3DVis::addData()
{


}
