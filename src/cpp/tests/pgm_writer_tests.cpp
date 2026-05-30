#include "../main/pgm_writer.h"
#include "../main/pgm_parser.h"
#include <gtest/gtest.h>
#include <fstream>
#include <vector>

/** Test Cases:
 *
 * PGMWriter__Test:
 * - CreatesFile [success]: write() produces a file at the given path
 * - InvalidPath [failure]: write() to a bad path throws runtime_error
 * - CorrectHeader [success]: written file has valid P5 header with correct dimensions
 * - PixelValuesRoundTrip [success]: pixel data written and re-parsed matches original
 * - SinglePixel [success]: 1x1 image writes and reads back correctly
 * - PixelOverflowTruncates [edge]: float value > 255 truncates via unsigned char cast
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

TEST(PGMWriter__Test, PixelOverflowTruncates) {
  // 300.0f cast to unsigned char wraps: 300 % 256 = 44
  // this documents and verifies the truncation behavior for out-of-range pixels
  Image img(1, 1, {300.0f});
  PGMWriter writer(img);
  writer.write("/tmp/pgmwriter_overflow.pgm");

  std::ifstream f("/tmp/pgmwriter_overflow.pgm", std::ios::binary);
  std::string magic;
  int w, h, maxval;
  f >> magic >> w >> h >> maxval;
  f.ignore(1); // skip the newline after maxval

  unsigned char byte = 0;
  f.read(reinterpret_cast<char *>(&byte), 1);
  // 300 % 256 = 44: cast through int first to get well-defined truncation
  EXPECT_EQ(byte, static_cast<unsigned char>(static_cast<int>(300.0f))); // 44
}
