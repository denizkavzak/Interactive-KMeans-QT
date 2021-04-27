#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <k_means.h>

class initialization
{
public:
  initialization();

  void initRandomReal(k_means& k_means);
  void initRandomSample(k_means& k_means);
  void initKMeansPp(k_means& k_means);

};

#endif // INITIALIZATION_H
