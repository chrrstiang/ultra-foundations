#pragma once
#include "filter.h"

class SobelEdgeDetection : public Filter {
public:
  Image apply(Image image) override;
};
