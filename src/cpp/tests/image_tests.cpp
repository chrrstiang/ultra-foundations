#include "../main/image.h"
#include <gtest/gtest.h>
#include <vector>

/** Test Cases:
 *
 * Image__Slice__Test:
 * - SliceReturnsCorrectRows: pixel values match the original rows
 * - SliceCorrectDimensions: height = endRow - startRow, width unchanged
 * - SliceSingleRow: slice of one row has height 1
 *
 * Image__Combine__Test:
 * - CombineReconstructsFullImage: two slices recombine to original
 * - CombineMultipleSlices: three slices recombine correctly
 * - CombinePreservesOrder: slices appear in the order provided
 */

// --- Image__Slice__Test ---

TEST(Image__Slice__Test, SliceReturnsCorrectRows) {
  // 4x3 image, rows with distinct values
  Image img(4, 3, {
      1, 2, 3,
      4, 5, 6,
      7, 8, 9,
      10, 11, 12
  });

  Image s = img.slice(1, 3); // rows 1 and 2
  EXPECT_FLOAT_EQ(s.at(0, 0), 4.0f);
  EXPECT_FLOAT_EQ(s.at(0, 1), 5.0f);
  EXPECT_FLOAT_EQ(s.at(0, 2), 6.0f);
  EXPECT_FLOAT_EQ(s.at(1, 0), 7.0f);
  EXPECT_FLOAT_EQ(s.at(1, 2), 9.0f);
}

TEST(Image__Slice__Test, SliceCorrectDimensions) {
  Image img(6, 4, std::vector<float>(24, 0.0f));
  Image s = img.slice(2, 5);
  EXPECT_EQ(s.getHeight(), 3);
  EXPECT_EQ(s.getWidth(), 4);
}

TEST(Image__Slice__Test, SliceSingleRow) {
  Image img(3, 2, {1, 2, 3, 4, 5, 6});
  Image s = img.slice(1, 2);
  EXPECT_EQ(s.getHeight(), 1);
  EXPECT_FLOAT_EQ(s.at(0, 0), 3.0f);
  EXPECT_FLOAT_EQ(s.at(0, 1), 4.0f);
}

// --- Image__Combine__Test ---

TEST(Image__Combine__Test, CombineReconstructsFullImage) {
  Image img(4, 2, {1, 2, 3, 4, 5, 6, 7, 8});
  Image top = img.slice(0, 2);
  Image bottom = img.slice(2, 4);
  Image result = Image::combine({top, bottom});

  EXPECT_EQ(result.getHeight(), 4);
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 2; c++) {
      EXPECT_FLOAT_EQ(result.at(r, c), img.at(r, c));
    }
  }
}

TEST(Image__Combine__Test, CombineMultipleSlices) {
  Image img(6, 2, {1,2, 3,4, 5,6, 7,8, 9,10, 11,12});
  Image s0 = img.slice(0, 2);
  Image s1 = img.slice(2, 4);
  Image s2 = img.slice(4, 6);
  Image result = Image::combine({s0, s1, s2});

  EXPECT_EQ(result.getHeight(), 6);
  for (int r = 0; r < 6; r++) {
    for (int c = 0; c < 2; c++) {
      EXPECT_FLOAT_EQ(result.at(r, c), img.at(r, c));
    }
  }
}

TEST(Image__Combine__Test, CombinePreservesOrder) {
  Image a(1, 2, {1.0f, 2.0f});
  Image b(1, 2, {3.0f, 4.0f});
  Image result = Image::combine({a, b});

  EXPECT_FLOAT_EQ(result.at(0, 0), 1.0f);
  EXPECT_FLOAT_EQ(result.at(1, 0), 3.0f);
}
