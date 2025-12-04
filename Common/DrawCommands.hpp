#ifndef DRAW_CMD_HPP
#define DRAW_CMD_HPP

#include <string>

namespace MenuCommands {
enum class MeshingApproach {
  None,
  PCA,
  PCAInv,
};

enum class PostProcessingApproach {
  None,
  Smooth,
  Remesh,
};

struct CreateSceneCMD {
  struct Transformation {
    float scale = 0;
    float rotation = 0;
  };

  Transformation transformation;
  MeshingApproach MeshApproach;
  PostProcessingApproach PostApproach;
  std::string objPath;
};
}; // namespace MenuCommands

#endif
