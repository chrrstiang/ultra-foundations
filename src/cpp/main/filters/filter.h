#pragma once
#include "../image.h"

class Filter {
public:
  virtual Image apply(Image image) = 0;
  virtual void prepare(const Image &image) {}
  virtual ~Filter() = default;
};
