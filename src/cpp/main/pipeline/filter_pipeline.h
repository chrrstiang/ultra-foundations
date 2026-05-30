#pragma once
#include "../filters/filter.h"
#include "../image/image.h"
#include <memory>
#include <vector>

class Pipeline {
public:
  Pipeline();
  ~Pipeline();
  Pipeline &addFilter(std::unique_ptr<Filter> filter);
  Image execute(Image current_image);
  Image executeParallel(Image image);

private:
  std::vector<std::unique_ptr<Filter>> filters;
};
