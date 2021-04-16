#include <QCoreApplication>
#include "k_means.h"
#include <iostream>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  k_means m(20, 2);
  QVector<k_means::Cluster> clusters;

  std::cout << "Clustering started !" << std::endl;

  clusters = m.clusterPoints(5);

  return a.exec();
}
