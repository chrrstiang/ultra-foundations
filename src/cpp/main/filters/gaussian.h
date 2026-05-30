#pragma once
#include "filter.h"

class GaussianBlur : public Filter {
public:
  Image apply(Image image) override;
};
