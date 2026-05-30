#pragma once
#include "image.h"
#include <string>

class PGMWriter {
public:
  PGMWriter(Image image);
  void write(const std::string &filepath);

private:
  Image image;
};
