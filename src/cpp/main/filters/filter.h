#pragma once
#include "../image.h"

class Filter {
public:
  virtual Image apply(Image image) = 0;
  virtual ~Filter() = default;
};
