#include "filter_pipeline.h"
#include "filters/gaussian.h"
#include "filters/intensity_normalization.h"
#include "filters/sobel_edge_detection.h"
#include "pgm_parser.h"
#include "pgm_writer.h"
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " <input.pgm> <output.pgm> [--parallel]\n";
    return 1;
  }

  bool parallel = (argc >= 4 && std::string(argv[3]) == "--parallel");

  // load image from disk
  PGM_Parser parser(argv[1]);
  Image image = parser.parse();

  // build pipeline
  Pipeline pipeline;
  pipeline.addFilter(std::make_unique<GaussianBlur>())
      .addFilter(std::make_unique<SobelEdgeDetection>())
      .addFilter(std::make_unique<IntensityNormalization>());

  // run filters
  Image result =
      parallel ? pipeline.executeParallel(image) : pipeline.execute(image);

  // write result to disk
  PGMWriter writer(result);
  writer.write(argv[2]);

  std::cout << "Done (" << (parallel ? "parallel" : "sequential") << "). "
            << "Output written to " << argv[2] << "\n";
  return 0;
}
