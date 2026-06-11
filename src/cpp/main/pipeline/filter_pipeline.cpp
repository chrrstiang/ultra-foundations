#include "filter_pipeline.h"
#include "../parallelizer/parallelizer.h"

Pipeline::Pipeline() = default;

Pipeline::~Pipeline() = default;

Pipeline &Pipeline::addFilter(std::unique_ptr<Filter> filter) {
  filters.push_back(std::move(filter));
  return *this;
}

Image Pipeline::execute(Image current_image) {
  for (auto &ptr : filters) {
    current_image = ptr->apply(current_image);
  }
  return current_image;
}

Image Pipeline::executeParallel(Image image) {
  Parallelizer p(filters, std::move(image));
  return p.execute();
}
