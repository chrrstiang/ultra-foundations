#include "gaussian.h"
#include <cmath>

/**
 * Applies a 3x3 Gaussian blur kernel to the image to reduce noise and detail.
 * Each pixel is replaced by the weighted average of its neighbors, with
 * center pixels weighted more heavily. Border pixels are clamped to the edge.
 */
Image GaussianBlur::apply(Image image) {
  // 3x3 Gaussian kernel (sigma ~ 1.0), sum = 16
  const float kernel[3][3] = {
      {1.0f, 2.0f, 1.0f},
      {2.0f, 4.0f, 2.0f},
      {1.0f, 2.0f, 1.0f}
  };
  const float kernelSum = 16.0f;

  int height = image.getHeight();
  int width = image.getWidth();

  Image result = image;

  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      float sum = 0.0f;

      for (int kr = -1; kr <= 1; kr++) {
        for (int kc = -1; kc <= 1; kc++) {
          // clamp to border
          int sr = std::max(0, std::min(height - 1, r + kr));
          int sc = std::max(0, std::min(width - 1, c + kc));
          sum += kernel[kr + 1][kc + 1] * image.at(sr, sc);
        }
      }

      result.set(sum / kernelSum, r, c);
    }
  }

  return result;
}
