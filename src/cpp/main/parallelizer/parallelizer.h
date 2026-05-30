#pragma once
#include "../filters/filter.h"
#include "../image/image.h"
#include <memory>
#include <vector>

class Parallelizer {
public:
  Parallelizer(std::vector<std::unique_ptr<Filter>> &filters, Image image);
  Image execute();

private:
  std::vector<std::unique_ptr<Filter>> &filters;
  Image image;
  static const int GHOST_ROWS = 1;
};
