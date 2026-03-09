#pragma once

#include "types.h"

vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart,
                            vec3d &lineEnd, float &t);

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri,
                              triangle &out_tri1, triangle &out_tri2);
