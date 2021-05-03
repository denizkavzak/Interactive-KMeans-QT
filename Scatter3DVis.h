#ifndef SCATTER3DVIS_H
#define SCATTER3DVIS_H

#include <QVector3D>
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtGui/QFont>

class Scatter3DVis
{
public:
  Scatter3DVis();
  explicit Scatter3DVis(QtDataVisualization::Q3DScatter *scatter);

  void addData();
  void changeStyle();
  void changePresetCamera();
  void changeLabelStyle();
  void changeFont(const QFont &font);
  void changeFontSize(int fontsize);
  void setBackgroundEnabled(int enabled);
  void setGridEnabled(int enabled);
  void setSmoothDots(int smooth);
  void toggleItemCount();
  void start();

//public slots:
//    void changeStyle(int style);
//    void changeTheme(int theme);
//    void changeShadowQuality(int quality);
//    void shadowQualityUpdatedByVisual(QtDataVisualization::QAbstract3DGraph
//                                      ::ShadowQuality shadowQuality);

//signals:
//    void backgroundEnabledChanged(bool enabled);
//    void gridEnabledChanged(bool enabled);
//    void shadowQualityChanged(int quality);
//    void fontChanged(QFont font);



private:
    QVector3D randVector();
    QtDataVisualization::Q3DScatter *m_graph;
    int m_fontSize;
    QtDataVisualization::QAbstract3DSeries::Mesh m_style;
    bool m_smooth;
    int m_itemCount;
    float m_curveDivider;

};

#endif // SCATTER3DVIS_H
