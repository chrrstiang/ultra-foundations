#include "parallelizer.h"
#include <algorithm>
#include <future>
#include <thread>

/**
 * Divides the image into horizontal strips equal to the number of hardware
 * threads. Each strip is padded with ghost rows so border pixels have real
 * neighbor data during convolution. Strips are processed concurrently via
 * std::async, ghost rows are trimmed from results, and strips are recombined.
 *
 * Before slicing, a serial pass calls prepare() on each filter with the full
 * image. Filters that require global image state (e.g. IntensityNormalization)
 * override prepare() to compute and store that state, ensuring results are
 * consistent with sequential execution.
 */
Parallelizer::Parallelizer(std::vector<std::unique_ptr<Filter>> &filters,
                           Image image)
    : filters(filters), image(image) {}

Image Parallelizer::execute() {
  int height = image.getHeight();
  int numStrips = std::min((int)std::thread::hardware_concurrency(), height);
  if (numStrips < 1) numStrips = 1;

  // serial pass: allow each filter to compute global state from the full image
  for (auto &f : filters) {
    f->prepare(image);
  }

  int baseRows = height / numStrips;
  int remainder = height % numStrips;

  // compute strip boundaries [startRow, endRow)
  std::vector<std::pair<int, int>> strips;
  int row = 0;
  for (int i = 0; i < numStrips; i++) {
    int startRow = row;
    int endRow = startRow + baseRows + (i < remainder ? 1 : 0);
    strips.push_back({startRow, endRow});
    row = endRow;
  }

  // launch one async task per strip
  std::vector<std::future<Image>> futures;
  for (auto &[startRow, endRow] : strips) {
    int sliceStart = std::max(0, startRow - GHOST_ROWS);
    int sliceEnd = std::min(height, endRow + GHOST_ROWS);

    Image padded = image.slice(sliceStart, sliceEnd);

    futures.push_back(std::async(std::launch::async, [padded, &filters = this->filters]() mutable {
      Image current = padded;
      for (auto &f : filters) {
        current = f->apply(current);
      }
      return current;
    }));
  }

  // collect results, trim ghost rows, combine
  std::vector<Image> results;
  for (int i = 0; i < numStrips; i++) {
    Image filtered = futures[i].get();
    auto [startRow, endRow] = strips[i];

    int sliceStart = std::max(0, startRow - GHOST_ROWS);
    int trimTop = startRow - sliceStart;
    int trimBottom = trimTop + (endRow - startRow);

    results.push_back(filtered.slice(trimTop, trimBottom));
  }

  return Image::combine(results);
}
