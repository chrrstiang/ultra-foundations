#include "bridge_NativeFilterBridge.h"
#include "filters/gaussian.h"
#include "filters/intensity_normalization.h"
#include "filters/sobel_edge_detection.h"
#include "image/image.h"
#include "pipeline/filter_pipeline.h"
#include <jni.h>
#include <memory>
#include <vector>

static const int FILTER_GAUSSIAN = 0;
static const int FILTER_SOBEL = 1;
static const int FILTER_NORMALIZE = 2;

static std::unique_ptr<Pipeline> g_pipeline;

/**
 * Configures the filter pipeline with the given filter IDs. Must be called
 * once before processImage to set which filters are applied to every frame.
 * Filter IDs: 0=GaussianBlur, 1=SobelEdgeDetection, 2=IntensityNormalization
 */
JNIEXPORT void JNICALL Java_bridge_NativeFilterBridge_configureFilters(
    JNIEnv *env, jobject obj, jintArray filterIds) {
  jsize count = env->GetArrayLength(filterIds);
  jint *ids = env->GetIntArrayElements(filterIds, nullptr);

  g_pipeline = std::make_unique<Pipeline>();
  for (jsize i = 0; i < count; i++) {
    if (ids[i] == FILTER_GAUSSIAN) {
      g_pipeline->addFilter(std::make_unique<GaussianBlur>());
    } else if (ids[i] == FILTER_SOBEL) {
      g_pipeline->addFilter(std::make_unique<SobelEdgeDetection>());
    } else if (ids[i] == FILTER_NORMALIZE) {
      g_pipeline->addFilter(std::make_unique<IntensityNormalization>());
    }
  }

  env->ReleaseIntArrayElements(filterIds, ids, JNI_ABORT);
}

/**
 * Takes the byte array (image) provided from the java client side, and adds
 * filters, ultimately returning the byte array after applying filters.
 */
JNIEXPORT jfloatArray JNICALL Java_bridge_NativeFilterBridge_processImage(
    JNIEnv *env, jobject obj, jfloatArray inputArray) {
  jsize length = env->GetArrayLength(inputArray);
  jfloat *elements = env->GetFloatArrayElements(inputArray, nullptr);

  std::vector<float> pixels(elements, elements + length);
  env->ReleaseFloatArrayElements(inputArray, elements, JNI_ABORT);

  if (g_pipeline) {
    Image image(1, length, pixels);
    Image result = g_pipeline->execute(image);
    for (jsize i = 0; i < length; i++) {
      pixels[i] = result.at(0, i);
    }
  }

  jfloatArray output = env->NewFloatArray(length);
  env->SetFloatArrayRegion(output, 0, length, pixels.data());
  return output;
}
