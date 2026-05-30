#include "../main/parallelizer/parallelizer.h"
#include "../main/filters/gaussian.h"
#include "../main/filters/intensity_normalization.h"
#include "../main/filters/sobel_edge_detection.h"
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <vector>

/** Test Cases:
 *
 * Parallelizer__Test:
 * - OutputDimensionsMatch [success]: result has same height/width as input
 * - FlatImageUnchanged [success]: flat image through GaussianBlur stays flat
 * - SameResultAsSequential [success]: parallel GaussianBlur matches sequential
 * - SingleRowImage [edge]: 1-row image doesn't crash, returns valid result
 * - NonDivisibleHeight [edge]: height not evenly divisible by thread count produces
 *   correct total height and correct pixel values
 * - SeamRowAccuracy [edge]: pixel values at strip boundary rows match sequential
 * - IntensityNormalizationMatchesSequential [success]: parallel pipeline with
 *   IntensityNormalization matches sequential, verifying prepare() global min/max
 */

static std::vector<std::unique_ptr<Filter>> make_gaussian_filters() {
  std::vector<std::unique_ptr<Filter>> filters;
  filters.push_back(std::make_unique<GaussianBlur>());
  return filters;
}

// --- Parallelizer__Test ---

TEST(Parallelizer__Test, OutputDimensionsMatch) {
  auto filters = make_gaussian_filters();
  Image img(8, 6, std::vector<float>(48, 100.0f));
  Parallelizer p(filters, img);
  Image result = p.execute();

  EXPECT_EQ(result.getHeight(), 8);
  EXPECT_EQ(result.getWidth(), 6);
}

TEST(Parallelizer__Test, FlatImageUnchanged) {
  auto filters = make_gaussian_filters();
  Image img(6, 6, std::vector<float>(36, 128.0f));
  Parallelizer p(filters, img);
  Image result = p.execute();

  for (int r = 0; r < 6; r++) {
    for (int c = 0; c < 6; c++) {
      EXPECT_FLOAT_EQ(result.at(r, c), 128.0f);
    }
  }
}

TEST(Parallelizer__Test, SameResultAsSequential) {
  std::vector<float> pixels;
  for (int i = 0; i < 36; i++) pixels.push_back(static_cast<float>(i * 5));

  auto seq_filters = make_gaussian_filters();
  auto par_filters = make_gaussian_filters();

  Image img(6, 6, pixels);

  // sequential
  Image seq = seq_filters[0]->apply(img);

  // parallel
  Parallelizer p(par_filters, img);
  Image par = p.execute();

  for (int r = 0; r < 6; r++) {
    for (int c = 0; c < 6; c++) {
      EXPECT_FLOAT_EQ(par.at(r, c), seq.at(r, c));
    }
  }
}

TEST(Parallelizer__Test, NonDivisibleHeight) {
  // 7 rows — not evenly divisible by most thread counts
  // regardless of how strips are divided, total height must be preserved
  // and all pixel values must match sequential
  auto seq_filters = make_gaussian_filters();
  auto par_filters = make_gaussian_filters();

  std::vector<float> pixels;
  for (int i = 0; i < 7 * 5; i++) pixels.push_back(static_cast<float>(i * 3));
  Image img(7, 5, pixels);

  Image seq = seq_filters[0]->apply(img);

  Parallelizer p(par_filters, img);
  Image par = p.execute();

  EXPECT_EQ(par.getHeight(), 7);
  EXPECT_EQ(par.getWidth(), 5);
  for (int r = 0; r < 7; r++) {
    for (int c = 0; c < 5; c++) {
      EXPECT_FLOAT_EQ(par.at(r, c), seq.at(r, c));
    }
  }
}

TEST(Parallelizer__Test, SeamRowAccuracy) {
  // Use a gradient image so seam rows have distinct, non-trivial values.
  // After Gaussian blur, seam rows must match sequential — this verifies
  // that ghost rows were applied correctly and trimmed correctly.
  auto seq_filters = make_gaussian_filters();
  auto par_filters = make_gaussian_filters();

  // 8x8 image with a horizontal gradient — each row has a different base value
  std::vector<float> pixels;
  for (int r = 0; r < 8; r++)
    for (int c = 0; c < 8; c++)
      pixels.push_back(static_cast<float>(r * 30 + c * 5));
  Image img(8, 8, pixels);

  Image seq = seq_filters[0]->apply(img);

  Parallelizer p(par_filters, img);
  Image par = p.execute();

  // check every row, with focused attention on likely seam positions
  int numStrips = std::min((int)std::thread::hardware_concurrency(), 8);
  if (numStrips < 1) numStrips = 1;
  int seamRow = 8 / numStrips; // first seam between strip 0 and strip 1

  for (int c = 0; c < 8; c++) {
    EXPECT_FLOAT_EQ(par.at(seamRow - 1, c), seq.at(seamRow - 1, c))
        << "mismatch at seam boundary (row above seam)";
    EXPECT_FLOAT_EQ(par.at(seamRow, c), seq.at(seamRow, c))
        << "mismatch at seam boundary (row below seam)";
  }
}

TEST(Parallelizer__Test, SingleRowImage) {
  auto filters = make_gaussian_filters();
  Image img(1, 4, {10.0f, 20.0f, 30.0f, 40.0f});
  Parallelizer p(filters, img);

  EXPECT_NO_THROW({
    Image result = p.execute();
    EXPECT_EQ(result.getHeight(), 1);
    EXPECT_EQ(result.getWidth(), 4);
  });
}

TEST(Parallelizer__Test, IntensityNormalizationMatchesSequential) {
  // build a gradient image with a wide range so normalization is non-trivial
  std::vector<float> pixels;
  for (int i = 0; i < 8 * 6; i++) pixels.push_back(static_cast<float>(i * 4));
  Image img(8, 6, pixels);

  // sequential: apply intensity normalization directly
  IntensityNormalization seqFilter;
  Image seq = seqFilter.apply(img);

  // parallel: IntensityNormalization via Parallelizer (prepare() called first)
  std::vector<std::unique_ptr<Filter>> parFilters;
  parFilters.push_back(std::make_unique<IntensityNormalization>());
  Parallelizer p(parFilters, img);
  Image par = p.execute();

  EXPECT_EQ(par.getHeight(), seq.getHeight());
  EXPECT_EQ(par.getWidth(), seq.getWidth());
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 6; c++) {
      EXPECT_FLOAT_EQ(par.at(r, c), seq.at(r, c))
          << "mismatch at row=" << r << " col=" << c;
    }
  }
}
