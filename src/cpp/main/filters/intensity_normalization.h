#pragma once
#include "filter.h"

class IntensityNormalization : public Filter {
public:
  Image apply(Image image) override;
};
