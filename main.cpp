#include <QCoreApplication>
#include "k_means.h"
#include <iostream>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  k_means m(20, 2);

  std::cout << "Clustering started !" << std::endl;

  m.clusterPoints(5);

  std::cout << "Clustering ended !" << std::endl;

  // Can access clusters by using m.getClusters()

  return a.exec();
}
