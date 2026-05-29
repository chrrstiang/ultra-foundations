#include <vector>

class Image {

private:
  int height;
  int width;
  std::vector<float> pixelData;

public:
  Image(int height, int width, std::vector<float> pixelData)
      : height(height), width(width), pixelData(std::move(pixelData)) {}

  // Reads a pixel value at the specified row and column
  float at(int row, int col) { return pixelData[row * width + col]; }

  //
  void set(float value, int row, int col) {
    pixelData[row * width + col] = value;
  }
};