#pragma once
#include "image.h"
#include <fstream>
#include <string>

class PGM_Parser {
public:
  PGM_Parser(const std::string &filepath);
  Image parse();

private:
  std::ifstream file;
  void skip_comments();
};
