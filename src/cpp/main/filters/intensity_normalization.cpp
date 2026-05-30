#include "intensity_normalization.h"

Image IntensityNormalization::apply(Image image) {
  float min = image.at(0, 0);
  float max = image.at(0, 0);

  for (int r = 0; r < image.getHeight(); r++) {
    for (int c = 0; c < image.getWidth(); c++) {
      float val = image.at(r, c);
      if (val < min) min = val;
      if (val > max) max = val;
    }
  }

  if (min == max) return image;

  for (int r = 0; r < image.getHeight(); r++) {
    for (int c = 0; c < image.getWidth(); c++) {
      float normalized = (image.at(r, c) - min) / (max - min) * 255.0f;
      image.set(normalized, r, c);
    }
  }

  return image;
}
