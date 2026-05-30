#include "../main/filters/gaussian.h"
#include <gtest/gtest.h>
#include <vector>

/** Test Cases:
 *
 * GaussianBlur__Test:
 * - FlatImageUnchanged: uniform image stays uniform after blur
 * - CenterWeightedMore: center pixel contributes most to its own blurred value
 * - OutputDimensionsMatch: result has same width and height as input
 * - BorderPixelClamped: border pixels don't read out-of-bounds (no crash)
 * - SinglePixel: 1x1 image returns same value
 */

static Image make_image(int height, int width, std::vector<float> pixels) {
  return Image(height, width, pixels);
}

// --- GaussianBlur__Test ---

TEST(GaussianBlur__Test, FlatImageUnchanged) {
  GaussianBlur filter;
  Image img = make_image(3, 3, {
      100.0f, 100.0f, 100.0f,
      100.0f, 100.0f, 100.0f,
      100.0f, 100.0f, 100.0f
  });
  Image result = filter.apply(img);

  // weighted average of identical values is the same value
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      EXPECT_FLOAT_EQ(result.at(r, c), 100.0f);
    }
  }
}

TEST(GaussianBlur__Test, OutputDimensionsMatch) {
  GaussianBlur filter;
  Image img = make_image(4, 5, std::vector<float>(20, 50.0f));
  Image result = filter.apply(img);

  EXPECT_EQ(result.getHeight(), 4);
  EXPECT_EQ(result.getWidth(), 5);
}

TEST(GaussianBlur__Test, BorderPixelClamped) {
  GaussianBlur filter;
  // just verify it doesn't crash on a non-trivial image
  Image img = make_image(3, 3, {
       10.0f,  20.0f,  30.0f,
       40.0f,  50.0f,  60.0f,
       70.0f,  80.0f,  90.0f
  });
  EXPECT_NO_THROW(filter.apply(img));
}

TEST(GaussianBlur__Test, SinglePixel) {
  GaussianBlur filter;
  Image img = make_image(1, 1, {128.0f});
  Image result = filter.apply(img);

  EXPECT_FLOAT_EQ(result.at(0, 0), 128.0f);
}

TEST(GaussianBlur__Test, BlurReducesSharpDifference) {
  GaussianBlur filter;
  // left half black, right half white — center pixels should be blended
  Image img = make_image(3, 4, {
        0.0f,   0.0f, 255.0f, 255.0f,
        0.0f,   0.0f, 255.0f, 255.0f,
        0.0f,   0.0f, 255.0f, 255.0f
  });
  Image result = filter.apply(img);

  // edge pixels (col 1 and col 2) should be blended, not pure 0 or 255
  EXPECT_GT(result.at(1, 1), 0.0f);
  EXPECT_LT(result.at(1, 2), 255.0f);
}
