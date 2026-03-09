#pragma once
#include "olcPixelGameEngine.h"

struct vec3d {
  float x = 0, y = 0, z = 0, w = 1;
};

struct vec2d {
  float u = 0, v = 0;
};

struct triangle {
  vec3d p[3];
  vec2d t[3];
  olc::Pixel col;
};

struct mat4x4 {
  float m[4][4] = {0};
};
