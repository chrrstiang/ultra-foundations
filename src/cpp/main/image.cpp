#include "image.h"

Image::Image(int height, int width, std::vector<float> pixelData)
    : height(height), width(width), pixelData(std::move(pixelData)) {}

float Image::at(int row, int col) { return pixelData[row * width + col]; }

void Image::set(float value, int row, int col) {
  pixelData[row * width + col] = value;
}

int Image::getHeight() { return height; }

int Image::getWidth() { return width; }
