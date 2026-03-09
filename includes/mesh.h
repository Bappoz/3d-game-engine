#pragma once

#include "types.h"
#include <string>
#include <vector>

struct mesh {
  std::vector<triangle> tris;
  bool LoadFromObjectFile(const std::string &sFilename);
};
