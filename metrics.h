#ifndef METRICS_H
#define METRICS_H

#include <QVector2D>
#include <QVector3D>

class metrics
{
public:
  metrics();

  float getDistance(QVector2D p1, QVector2D p2, QString metric);
  float getDistance(QVector3D p1, QVector3D p2, QString metric);
  float getDistance(QString binary1, QString binary2, QString metric);

};

#endif // METRICS_H
