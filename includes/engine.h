#pragma once

#include "../libs/olcPixelGameEngine.h"
#include "mesh.h"
#include "types.h"

class olcEngine3D : public olc::PixelGameEngine {
public:
  olcEngine3D();
  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;

private:
  mesh meshCube;
  mat4x4 matProj;
  float fTheta = 0.0f;
  vec3d vCamera = {0.0f, 0.0f, 0.0f};
  vec3d vLookDir;
  float fYam = 0.0f;

  olc::Pixel GetColour(float lum);
};
