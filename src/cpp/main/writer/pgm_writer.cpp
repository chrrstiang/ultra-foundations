#include "pgm_writer.h"
#include <fstream>
#include <stdexcept>

/**
 * Takes an Image and writes it to a file in P5 (binary) PGM format.
 * The header contains the magic number, width, height, and max value (255),
 * followed by raw pixel bytes derived from the image's pixel data.
 */
PGMWriter::PGMWriter(Image image) : image(std::move(image)) {}

void PGMWriter::write(const std::string &filepath) {
  std::ofstream file(filepath, std::ios::binary);

  if (!file) {
    throw std::runtime_error("Cannot open file for writing.");
  }

  int height = image.getHeight();
  int width = image.getWidth();

  file << "P5\n" << width << " " << height << "\n255\n";

  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      auto byte = static_cast<unsigned char>(image.at(r, c));
      file.write(reinterpret_cast<const char *>(&byte), 1);
    }
  }
}
