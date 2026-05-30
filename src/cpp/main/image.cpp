#include "image.h"
#include <stdexcept>

Image::Image(int height, int width, std::vector<float> pixelData)
    : height(height), width(width), pixelData(std::move(pixelData)) {}

float Image::at(int row, int col) const { return pixelData[row * width + col]; }

void Image::set(float value, int row, int col) {
  pixelData[row * width + col] = value;
}

int Image::getHeight() const { return height; }

int Image::getWidth() const { return width; }

Image Image::slice(int startRow, int endRow) {
  if (startRow < 0)
    throw std::invalid_argument("startRow must be >= 0");
  if (endRow > height)
    throw std::invalid_argument("endRow must be <= image height");
  if (startRow >= endRow)
    throw std::invalid_argument("startRow must be < endRow");

  int newHeight = endRow - startRow;
  std::vector<float> sliceData(newHeight * width);
  for (int r = startRow; r < endRow; r++) {
    for (int c = 0; c < width; c++) {
      sliceData[(r - startRow) * width + c] = at(r, c);
    }
  }
  return Image(newHeight, width, sliceData);
}

Image Image::combine(std::vector<Image> slices) {
  if (slices.empty())
    throw std::invalid_argument("Cannot combine an empty list of images");

  int totalHeight = 0;
  int width = slices[0].getWidth();
  for (auto &s : slices) totalHeight += s.getHeight();

  std::vector<float> combined(totalHeight * width);
  int rowOffset = 0;
  for (auto &s : slices) {
    for (int r = 0; r < s.getHeight(); r++) {
      for (int c = 0; c < width; c++) {
        combined[rowOffset * width + c] = s.at(r, c);
      }
      rowOffset++;
    }
  }
  return Image(totalHeight, width, combined);
}
