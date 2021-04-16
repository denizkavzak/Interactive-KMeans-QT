#include <QCoreApplication>
#include "k_means.h"
#include <iostream>
#include <QDebug>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  k_means m(20, 2);

  qDebug() << "Clustering started !";

  m.clusterPoints(5);

  qDebug() << "Clustering ended !";

  // Can access clusters by using m.getClusters()

  return a.exec();
}
