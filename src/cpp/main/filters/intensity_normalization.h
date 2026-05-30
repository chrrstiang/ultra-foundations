#pragma once
#include "filter.h"

class IntensityNormalization : public Filter {
public:
  Image apply(Image image) override;
  void prepare(const Image &image) override;

private:
  float globalMin = 0.0f;
  float globalMax = 0.0f;
  bool prepared = false;
};
