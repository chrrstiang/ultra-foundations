#include "pgm_parser.h"
#include <vector>

PGM_Parser::PGM_Parser(const std::string &filepath)
    : file(filepath, std::ios::binary) {}

void PGM_Parser::skip_comments() {
  while (file >> std::ws && file.peek() == '#') {
    std::string dummy;
    std::getline(file, dummy);
  }
}

/**
 * parses the PGM file and returns an Image object corresponding to the PGM
 * file
 */
Image PGM_Parser::parse() {
  if (!file) {
    throw std::runtime_error("Cannot open file.");
  }

  std::string magic;
  if (!(file >> magic)) {
    throw std::runtime_error("Failed to read PGM header: file is empty or unreadable.");
  }

  int width;
  int height;
  std::vector<float> pixelData;

  skip_comments();
  file >> width;

  skip_comments();
  file >> height;

  skip_comments();
  int maxVal;
  file >> maxVal;
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  pixelData.resize(width * height);

  if (magic == "P2") {
    for (int i = 0; i < width * height; i++) {
      int val;
      file >> val;
      pixelData[i] = static_cast<float>(val);
    }
  } else if (magic == "P5") {
    for (int i = 0; i < width * height; i++) {
      unsigned char byte;
      file.read(reinterpret_cast<char *>(&byte), 1);
      pixelData[i] = static_cast<float>(byte);
    }
  }

  return Image(height, width, pixelData);
}
