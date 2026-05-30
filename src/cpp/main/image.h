#pragma once
#include <vector>

class Image {
public:
  Image(int height, int width, std::vector<float> pixelData);
  float at(int row, int col);
  void set(float value, int row, int col);
  int getHeight();
  int getWidth();
  Image slice(int startRow, int endRow);
  static Image combine(std::vector<Image> slices);

private:
  int height;
  int width;
  std::vector<float> pixelData;
};
