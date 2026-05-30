#pragma once
#include <vector>

class Image {
public:
  Image(int height, int width, std::vector<float> pixelData);
  float at(int row, int col) const;
  void set(float value, int row, int col);
  int getHeight() const;
  int getWidth() const;
  Image slice(int startRow, int endRow);
  static Image combine(std::vector<Image> slices);

private:
  int height;
  int width;
  std::vector<float> pixelData;
};
