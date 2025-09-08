#ifndef DRAW_CMD_HPP
#define DRAW_CMD_HPP

#include <string>

namespace MenuCommands {

struct CreateSceneCMD {
  struct Transformation {
    float scale = 0;
    float rotation = 0;
  };

  enum MeshingApproach {
    None,
    PCA,
    PCAInv,
    Smooth,
  };

  Transformation transformation;
  MeshingApproach approach;
  std::string objPath;
};
}; // namespace DrawMessages

#endif
