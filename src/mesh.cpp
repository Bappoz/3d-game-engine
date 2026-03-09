#include "../includes/mesh.h"
#include <fstream>
#include <sstream>

bool mesh::LoadFromObjectFile(const std::string &sFilename) {
  std::ifstream f(sFilename);
  if (!f.is_open())
    return false;

  std::vector<vec3d> verts;
  while (!f.eof()) {
    char line[128];
    f.getline(line, 128);
    std::stringstream s(line);
    char junk;

    if (line[0] == 'v') {
      vec3d v;
      s >> junk >> v.x >> v.y >> v.z;
      verts.push_back(v);
    }

    if (line[0] == 'f') {
      int idx[3];
      s >> junk >> idx[0] >> idx[1] >> idx[2];
      tris.push_back({verts[idx[0] - 1], verts[idx[1] - 1], verts[idx[2] - 1]});
    }
  }
  return true;
}
