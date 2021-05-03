#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <k_means.h>
#include <QVector>

class initialization
{

private:
  QVector<float> getPairwiseDistances(k_means &k_means, QVector2D center,
                               QString metric, float &min, int &min_ind,
                              float &sum);
  QVector<float> getProbs(QVector<float>, float sum);
  int choseWithProb(QVector<float> prob);
  QVector<float> choseMin(QVector<float> d, QVector<float> d2);

public:
  initialization();

  void initRandomReal(k_means& k_means);
  void initRandomSample(k_means& k_means);
  void initKMeansPp(k_means& k_means);

  void generateRandomPoints(float min, float max, k_means& k_means);
  void generateNormalDistributionPoints(float min, float max, k_means& k_means);

  void generateRandomPointsND(float min, float max, k_means& k_means);
  void generateNormalDistributionPointsND(float min, float max, k_means& k_means);

};

#endif // INITIALIZATION_H
