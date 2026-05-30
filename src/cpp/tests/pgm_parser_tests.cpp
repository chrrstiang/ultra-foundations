#include "../main/pgm_parser.h"
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

/** Test Cases:
 *
 * FileHandling__Test:
 * - InvalidPath [failure]: nonexistent file throws runtime_error
 * - EmptyFile [failure]: file exists but is empty throws runtime_error
 *
 * Parse__Test:
 * - PixelValues [success]: known byte values are parsed correctly
 * - CommentsInHeader [success]: a comment line before dimensions is skipped
 * - SinglePixel [success]: 1x1 image parses correctly
 * - DimensionsCorrect [success]: parsed image has correct width and height
 * - MultipleComments [edge]: multiple comment lines before each header field are all skipped
 *
 * Image__Test:
 * - AtRowCol [success]: at() maps row/col to correct pixel
 * - SetRowCol [success]: set() updates correct pixel
 */

// Writes a minimal valid P5 PGM file and returns the path.
static std::string write_temp_pgm(const std::string &path, int w, int h,
                                  const std::vector<unsigned char> &pixels,
                                  bool with_comment = false) {
  std::ofstream f(path, std::ios::binary);
  f << "P5\n";
  if (with_comment) {
    f << "# this is a comment\n";
  }
  f << w << " " << h << "\n255\n";
  f.write(reinterpret_cast<const char *>(pixels.data()), pixels.size());
  return path;
}

// --- FileHandling__Test ---

TEST(FileHandling__Test, InvalidPath) {
  PGM_Parser parser("/nonexistent/path/file.pgm");
  EXPECT_THROW(parser.parse(), std::runtime_error);
}

TEST(FileHandling__Test, EmptyFile) {
  // create an empty file then try to parse it
  { std::ofstream f("/tmp/test_empty.pgm"); }
  PGM_Parser parser("/tmp/test_empty.pgm");
  EXPECT_THROW(parser.parse(), std::runtime_error);
}

// --- Parse__Test ---

TEST(Parse__Test, PixelValues) {
  // 3x2 image with known pixel values
  std::vector<unsigned char> pixels = {10, 20, 30, 40, 50, 60};
  write_temp_pgm("/tmp/test_pixel_values.pgm", 3, 2, pixels);

  PGM_Parser parser("/tmp/test_pixel_values.pgm");
  Image img = parser.parse();

  EXPECT_EQ(img.at(0, 0), 10.0f);
  EXPECT_EQ(img.at(0, 1), 20.0f);
  EXPECT_EQ(img.at(0, 2), 30.0f);
  EXPECT_EQ(img.at(1, 0), 40.0f);
  EXPECT_EQ(img.at(1, 1), 50.0f);
  EXPECT_EQ(img.at(1, 2), 60.0f);
}

TEST(Parse__Test, CommentsInHeader) {
  std::vector<unsigned char> pixels = {128};
  write_temp_pgm("/tmp/test_comments.pgm", 1, 1, pixels, true);

  PGM_Parser parser("/tmp/test_comments.pgm");
  Image img = parser.parse();

  EXPECT_EQ(img.at(0, 0), 128.0f);
}

TEST(Parse__Test, SinglePixel) {
  std::vector<unsigned char> pixels = {255};
  write_temp_pgm("/tmp/test_single.pgm", 1, 1, pixels);

  PGM_Parser parser("/tmp/test_single.pgm");
  Image img = parser.parse();

  EXPECT_EQ(img.at(0, 0), 255.0f);
}

TEST(Parse__Test, DimensionsCorrect) {
  std::vector<unsigned char> pixels(3 * 4, 0);
  write_temp_pgm("/tmp/test_dims.pgm", 4, 3, pixels);

  PGM_Parser parser("/tmp/test_dims.pgm");
  Image img = parser.parse();

  EXPECT_EQ(img.getWidth(), 4);
  EXPECT_EQ(img.getHeight(), 3);
}

TEST(Parse__Test, MultipleComments) {
  // write a PGM file with multiple comment lines before each header field
  {
    std::ofstream f("/tmp/test_multicomment.pgm", std::ios::binary);
    f << "P5\n";
    f << "# comment 1\n";
    f << "# comment 2\n";
    f << "2 2\n";
    f << "# comment before maxval\n";
    f << "255\n";
    unsigned char pixels[] = {10, 20, 30, 40};
    f.write(reinterpret_cast<const char *>(pixels), 4);
  }

  PGM_Parser parser("/tmp/test_multicomment.pgm");
  Image img = parser.parse();

  EXPECT_EQ(img.at(0, 0), 10.0f);
  EXPECT_EQ(img.at(0, 1), 20.0f);
  EXPECT_EQ(img.at(1, 0), 30.0f);
  EXPECT_EQ(img.at(1, 1), 40.0f);
}

// --- Image__Test ---

TEST(Image__Test, AtRowCol) {
  // 2x3 image: verify row-major indexing
  std::vector<unsigned char> pixels = {1, 2, 3, 4, 5, 6};
  write_temp_pgm("/tmp/test_at.pgm", 3, 2, pixels);

  PGM_Parser parser("/tmp/test_at.pgm");
  Image img = parser.parse();

  // row 0: pixels 1,2,3 — row 1: pixels 4,5,6
  EXPECT_EQ(img.at(0, 0), 1.0f);
  EXPECT_EQ(img.at(0, 2), 3.0f);
  EXPECT_EQ(img.at(1, 0), 4.0f);
  EXPECT_EQ(img.at(1, 2), 6.0f);
}

TEST(Image__Test, SetRowCol) {
  std::vector<unsigned char> pixels = {0, 0, 0, 0};
  write_temp_pgm("/tmp/test_set.pgm", 2, 2, pixels);

  PGM_Parser parser("/tmp/test_set.pgm");
  Image img = parser.parse();

  img.set(99.0f, 1, 1);
  EXPECT_EQ(img.at(1, 1), 99.0f);
  // other pixels unchanged
  EXPECT_EQ(img.at(0, 0), 0.0f);
}
