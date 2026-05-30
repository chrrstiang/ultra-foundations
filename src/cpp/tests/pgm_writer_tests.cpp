#include "../main/pgm_writer.h"
#include "../main/pgm_parser.h"
#include <gtest/gtest.h>
#include <fstream>
#include <vector>

/** Test Cases:
 *
 * PGMWriter__Test:
 * - CreatesFile: write() produces a file at the given path
 * - InvalidPath: write() to a bad path throws runtime_error
 * - CorrectHeader: written file has valid P5 header with correct dimensions
 * - PixelValuesRoundTrip: pixel data written and re-parsed matches original
 * - SinglePixel: 1x1 image writes and reads back correctly
 */

// --- PGMWriter__Test ---

TEST(PGMWriter__Test, CreatesFile) {
  Image img(1, 1, {128.0f});
  PGMWriter writer(img);
  writer.write("/tmp/pgmwriter_creates.pgm");

  std::ifstream f("/tmp/pgmwriter_creates.pgm");
  EXPECT_TRUE(f.good());
}

TEST(PGMWriter__Test, InvalidPath) {
  Image img(1, 1, {128.0f});
  PGMWriter writer(img);
  EXPECT_THROW(writer.write("/nonexistent/path/out.pgm"), std::runtime_error);
}

TEST(PGMWriter__Test, PixelValuesRoundTrip) {
  std::vector<float> pixels = {0.0f, 64.0f, 128.0f, 255.0f};
  Image img(2, 2, pixels);

  PGMWriter writer(img);
  writer.write("/tmp/pgmwriter_roundtrip.pgm");

  PGM_Parser parser("/tmp/pgmwriter_roundtrip.pgm");
  Image result = parser.parse();

  EXPECT_FLOAT_EQ(result.at(0, 0), 0.0f);
  EXPECT_FLOAT_EQ(result.at(0, 1), 64.0f);
  EXPECT_FLOAT_EQ(result.at(1, 0), 128.0f);
  EXPECT_FLOAT_EQ(result.at(1, 1), 255.0f);
}

TEST(PGMWriter__Test, SinglePixel) {
  Image img(1, 1, {200.0f});
  PGMWriter writer(img);
  writer.write("/tmp/pgmwriter_single.pgm");

  PGM_Parser parser("/tmp/pgmwriter_single.pgm");
  Image result = parser.parse();

  EXPECT_FLOAT_EQ(result.at(0, 0), 200.0f);
}

TEST(PGMWriter__Test, CorrectHeader) {
  Image img(3, 5, std::vector<float>(15, 0.0f));
  PGMWriter writer(img);
  writer.write("/tmp/pgmwriter_header.pgm");

  std::ifstream f("/tmp/pgmwriter_header.pgm");
  std::string magic, maxval;
  int width, height;
  f >> magic >> width >> height >> maxval;

  EXPECT_EQ(magic, "P5");
  EXPECT_EQ(width, 5);
  EXPECT_EQ(height, 3);
  EXPECT_EQ(maxval, "255");
}
