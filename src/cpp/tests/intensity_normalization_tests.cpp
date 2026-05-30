#include "../main/filters/intensity_normalization.h"
#include <gtest/gtest.h>
#include <vector>

/** Test Cases:
 *
 * IntensityNormalization__Test:
 * - FullRange [success]: pixels already spanning [0,255] remain unchanged
 * - ScalesUp [success]: low-range pixels are scaled up to [0,255]
 * - ScalesDown [success]: high-range pixels are scaled down to [0,255]
 * - FlatImage [edge]: all pixels equal returns image unchanged (no division by zero)
 * - SinglePixel [edge]: 1x1 image with flat value returns as-is
 * - MinMapsToZero [success]: minimum pixel in result is 0
 * - MaxMapsTo255 [success]: maximum pixel in result is 255
 * - NegativePixelValues [success]: range [-50, 50] — min maps to 0, max maps to 255
 * - PrepareUsesGlobalRange [success]: prepare() on wide-range image, apply() on sub-image uses global range
 */

static Image make_image(int height, int width, std::vector<float> pixels) {
  return Image(height, width, pixels);
}

// --- IntensityNormalization__Test ---

TEST(IntensityNormalization__Test, FullRange) {
  IntensityNormalization filter;
  Image img = make_image(2, 2, {0.0f, 255.0f, 128.0f, 64.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 0), 0.0f);
  EXPECT_FLOAT_EQ(result.at(0, 1), 255.0f);
}

TEST(IntensityNormalization__Test, ScalesUp) {
  IntensityNormalization filter;
  // pixels in [0, 1] range — should scale to [0, 255]
  Image img = make_image(1, 3, {0.0f, 0.5f, 1.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 0), 0.0f);
  EXPECT_FLOAT_EQ(result.at(0, 2), 255.0f);
  EXPECT_FLOAT_EQ(result.at(0, 1), 127.5f);
}

TEST(IntensityNormalization__Test, ScalesDown) {
  IntensityNormalization filter;
  // pixels in [100, 200] — min maps to 0, max maps to 255
  Image img = make_image(1, 3, {100.0f, 150.0f, 200.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 0), 0.0f);
  EXPECT_FLOAT_EQ(result.at(0, 2), 255.0f);
  EXPECT_FLOAT_EQ(result.at(0, 1), 127.5f);
}

TEST(IntensityNormalization__Test, FlatImage) {
  IntensityNormalization filter;
  Image img = make_image(2, 2, {128.0f, 128.0f, 128.0f, 128.0f});
  Image result = filter.apply(img);

  // all pixels should remain unchanged
  EXPECT_FLOAT_EQ(result.at(0, 0), 128.0f);
  EXPECT_FLOAT_EQ(result.at(1, 1), 128.0f);
}

TEST(IntensityNormalization__Test, SinglePixel) {
  IntensityNormalization filter;
  Image img = make_image(1, 1, {77.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 0), 77.0f);
}

TEST(IntensityNormalization__Test, MinMapsToZero) {
  IntensityNormalization filter;
  Image img = make_image(1, 4, {10.0f, 50.0f, 90.0f, 200.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 0), 0.0f);
}

TEST(IntensityNormalization__Test, MaxMapsTo255) {
  IntensityNormalization filter;
  Image img = make_image(1, 4, {10.0f, 50.0f, 90.0f, 200.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 3), 255.0f);
}

TEST(IntensityNormalization__Test, NegativePixelValues) {
  IntensityNormalization filter;
  // range [-50, 50] — min maps to 0, max maps to 255, midpoint to 127.5
  Image img = make_image(1, 3, {-50.0f, 0.0f, 50.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 0), 0.0f);
  EXPECT_FLOAT_EQ(result.at(0, 2), 255.0f);
  EXPECT_FLOAT_EQ(result.at(0, 1), 127.5f);
}

TEST(IntensityNormalization__Test, PrepareUsesGlobalRange) {
  // prepare() with wide-range image [0, 200]; apply() on sub-image with only [50, 150]
  // result should use global range [0,200], not local [50,150]
  IntensityNormalization filter;

  Image fullImg = make_image(1, 3, {0.0f, 100.0f, 200.0f});
  filter.prepare(fullImg);

  // sub-image only has values in [50, 150]
  Image subImg = make_image(1, 3, {50.0f, 100.0f, 150.0f});
  Image result = filter.apply(subImg);

  // using global range [0, 200]: (50-0)/200*255=63.75, (100-0)/200*255=127.5, (150-0)/200*255=191.25
  EXPECT_FLOAT_EQ(result.at(0, 0), 63.75f);
  EXPECT_FLOAT_EQ(result.at(0, 1), 127.5f);
  EXPECT_FLOAT_EQ(result.at(0, 2), 191.25f);
}
