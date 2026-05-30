#include "intensity_normalization.h"

/**
 * Uses the minimum and maximum intensity values of the
 * image to average out the pixel intensities of the image.
 * If prepare() was called with the full image beforehand (e.g. during parallel
 * execution), the pre-computed global min/max is used instead of computing
 * local values from the strip, ensuring correct normalization across all
 * strips.
 */
void IntensityNormalization::prepare(const Image &image) {
  globalMin = image.at(0, 0);
  globalMax = image.at(0, 0);

  for (int r = 0; r < image.getHeight(); r++) {
    for (int c = 0; c < image.getWidth(); c++) {
      float val = image.at(r, c);
      if (val < globalMin)
        globalMin = val;
      if (val > globalMax)
        globalMax = val;
    }
  }

  prepared = true;
}

Image IntensityNormalization::apply(Image image) {
  float min = prepared ? globalMin : image.at(0, 0);
  float max = prepared ? globalMax : image.at(0, 0);

  if (!prepared) {
    for (int r = 0; r < image.getHeight(); r++) {
      for (int c = 0; c < image.getWidth(); c++) {
        float val = image.at(r, c);
        if (val < min)
          min = val;
        if (val > max)
          max = val;
      }
    }
  }

  if (min == max)
    return image;

  for (int r = 0; r < image.getHeight(); r++) {
    for (int c = 0; c < image.getWidth(); c++) {
      float normalized = (image.at(r, c) - min) / (max - min) * 255.0f;
      image.set(normalized, r, c);
    }
  }

  return image;
}
