#include "bridge_NativeFilterBridge.h"
#include <jni.h>

/**
 *  Takes the byte array (image) provided from the java client side, and adds
 * filters, ultimately returning the byte array after applying filters.
 */
JNIEXPORT jfloatArray JNICALL Java_bridge_NativeFilterBridge_processImage(
    JNIEnv *env, jobject obj, jfloatArray inputArray) {
  jsize length = env->GetArrayLength(inputArray);
  jfloat *elements = env->GetFloatArrayElements(inputArray, nullptr);

  jfloatArray result = env->NewFloatArray(length);
  jfloat *resultElements = env->GetFloatArrayElements(result, nullptr);

  for (jsize i = 0; i < length; i++) {
    resultElements[i] = elements[i] * 2.0f;
  }

  env->ReleaseFloatArrayElements(inputArray, elements, JNI_ABORT);
  env->ReleaseFloatArrayElements(result, resultElements, 0);

  return result;
}