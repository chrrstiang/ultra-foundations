#include "../main/filters/sobel_edge_detection.h"
#include <gtest/gtest.h>
#include <vector>

/** Test Cases:
 *
 * SobelEdgeDetection__Test:
 * - FlatImageZeroEdges: uniform image produces zero gradient everywhere
 * - OutputDimensionsMatch: result has same width and height as input
 * - SharpEdgeProducesHighMagnitude: hard black-to-white edge yields high values
 * - OutputClamped: no pixel exceeds 255
 * - SinglePixel: 1x1 image returns zero (no neighbors to form gradient)
 */

static Image make_image(int height, int width, std::vector<float> pixels) {
  return Image(height, width, pixels);
}

// --- SobelEdgeDetection__Test ---

TEST(SobelEdgeDetection__Test, FlatImageZeroEdges) {
  SobelEdgeDetection filter;
  Image img = make_image(3, 3, std::vector<float>(9, 128.0f));
  Image result = filter.apply(img);

  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      EXPECT_FLOAT_EQ(result.at(r, c), 0.0f);
    }
  }
}

TEST(SobelEdgeDetection__Test, OutputDimensionsMatch) {
  SobelEdgeDetection filter;
  Image img = make_image(4, 5, std::vector<float>(20, 50.0f));
  Image result = filter.apply(img);

  EXPECT_EQ(result.getHeight(), 4);
  EXPECT_EQ(result.getWidth(), 5);
}

TEST(SobelEdgeDetection__Test, SharpEdgeProducesHighMagnitude) {
  SobelEdgeDetection filter;
  // vertical edge: left half black, right half white
  Image img = make_image(3, 4, {
        0.0f,   0.0f, 255.0f, 255.0f,
        0.0f,   0.0f, 255.0f, 255.0f,
        0.0f,   0.0f, 255.0f, 255.0f
  });
  Image result = filter.apply(img);

  // pixels at the edge boundary should have high gradient
  EXPECT_GT(result.at(1, 1), 100.0f);
  EXPECT_GT(result.at(1, 2), 100.0f);
}

TEST(SobelEdgeDetection__Test, OutputClamped) {
  SobelEdgeDetection filter;
  Image img = make_image(3, 3, {
        0.0f,   0.0f, 255.0f,
        0.0f,   0.0f, 255.0f,
        0.0f,   0.0f, 255.0f
  });
  Image result = filter.apply(img);

  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      EXPECT_LE(result.at(r, c), 255.0f);
      EXPECT_GE(result.at(r, c), 0.0f);
    }
  }
}

TEST(SobelEdgeDetection__Test, SinglePixel) {
  SobelEdgeDetection filter;
  Image img = make_image(1, 1, {200.0f});
  Image result = filter.apply(img);

  // single pixel has no neighbors, gradient is zero
  EXPECT_FLOAT_EQ(result.at(0, 0), 0.0f);
}
