#include "sobel_edge_detection.h"
#include <algorithm>
#include <cmath>

/**
 * Applies the Sobel edge detection operator to the image.
 * Computes horizontal (Gx) and vertical (Gy) gradients using 3x3 kernels,
 * then combines them as magnitude = sqrt(Gx^2 + Gy^2). Border pixels
 * are clamped to the edge. Output values are clamped to [0, 255].
 */
Image SobelEdgeDetection::apply(Image image) {
  // Sobel kernels
  const float Kx[3][3] = {
      {-1.0f, 0.0f, 1.0f}, {-2.0f, 0.0f, 2.0f}, {-1.0f, 0.0f, 1.0f}};
  const float Ky[3][3] = {
      {-1.0f, -2.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 1.0f}};

  int height = image.getHeight();
  int width = image.getWidth();

  Image result = image;

  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      float gx = 0.0f;
      float gy = 0.0f;

      for (int kr = -1; kr <= 1; kr++) {
        for (int kc = -1; kc <= 1; kc++) {
          // clamp to border
          int sr = std::max(0, std::min(height - 1, r + kr));
          int sc = std::max(0, std::min(width - 1, c + kc));
          float val = image.at(sr, sc);
          gx += Kx[kr + 1][kc + 1] * val;
          gy += Ky[kr + 1][kc + 1] * val;
        }
      }

      float magnitude = std::sqrt((gx * gx) + (gy * gy));
      // clamp to [0, 255]
      magnitude = std::min(magnitude, 255.0f);
      result.set(magnitude, r, c);
    }
  }

  return result;
}
