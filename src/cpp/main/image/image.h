#pragma once
#include <vector>

class Image {
public:
  Image(int height, int width, std::vector<float> pixelData);
  [[nodiscard]] float at(int row, int col) const;
  void set(float value, int row, int col);
  [[nodiscard]] int getHeight() const;
  [[nodiscard]] int getWidth() const;
  [[nodiscard]] Image slice(int startRow, int endRow) const;
  static Image combine(std::vector<Image> slices);

private:
  int height;
  int width;
  std::vector<float> pixelData;
};
