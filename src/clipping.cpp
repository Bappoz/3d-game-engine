#include "../includes/clipping.h"
#include "../includes/math_utils.h"

vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart,
                            vec3d &lineEnd, float &t) {
  plane_n = Vector_Normalize(plane_n);
  float plane_d = -Vector_DotProduct(plane_n, plane_p);
  float ad = Vector_DotProduct(lineStart, plane_n);
  float bd = Vector_DotProduct(lineEnd, plane_n);
  t = (-plane_d - ad) / (bd - ad);
  vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
  vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);

  return Vector_Add(lineStart, lineToIntersect);
}

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri,
                              triangle &out_tri1, triangle &out_tri2) {
  plane_n = Vector_Normalize(plane_n);

  auto dist = [&](vec3d &p) {
    return plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z -
           Vector_DotProduct(plane_n, plane_p);
  };

  vec3d *inside_points[3];
  int nInsidePointCount = 0;
  vec3d *outside_points[3];
  int nOutsidePointCount = 0;
  vec2d *inside_tex[3];
  int nInsideTexCount = 0;
  vec2d *outside_tex[3];
  int nOutsideTexCount = 0;

  float d0 = dist(in_tri.p[0]);
  float d1 = dist(in_tri.p[1]);
  float d2 = dist(in_tri.p[2]);

  if (d0 >= 0) {
    inside_points[nInsidePointCount++] = &in_tri.p[0];
    inside_tex[nInsideTexCount++] = &in_tri.t[0];
  } else {
    outside_points[nOutsidePointCount++] = &in_tri.p[0];
    outside_tex[nOutsideTexCount++] = &in_tri.t[0];
  }
  if (d1 >= 0) {
    inside_points[nInsidePointCount++] = &in_tri.p[1];
    inside_tex[nInsideTexCount++] = &in_tri.t[1];
  } else {
    outside_points[nOutsidePointCount++] = &in_tri.p[1];
    outside_tex[nOutsideTexCount++] = &in_tri.t[1];
  }
  if (d2 >= 0) {
    inside_points[nInsidePointCount++] = &in_tri.p[2];
    inside_tex[nInsideTexCount++] = &in_tri.t[2];
  } else {
    outside_points[nOutsidePointCount++] = &in_tri.p[2];
    outside_tex[nOutsideTexCount++] = &in_tri.t[2];
  }

  if (nInsidePointCount == 0)
    return 0;

  if (nInsidePointCount == 3) {
    out_tri1 = in_tri;
    return 1;
  }

  if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
    out_tri1.col = in_tri.col;
    out_tri1.p[0] = *inside_points[0];
    out_tri1.t[0] = *inside_tex[0];

    float t;
    out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0],
                                          *outside_points[0], t);
    out_tri1.t[1].u =
        t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
    out_tri1.t[1].v =
        t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;

    out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0],
                                          *outside_points[1], t);
    out_tri1.t[2].u =
        t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
    out_tri1.t[2].v =
        t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
    return 1;
  }

  if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
    float t;
    out_tri1.col = in_tri.col;
    out_tri1.p[0] = *inside_points[0];
    out_tri1.t[0] = *inside_tex[0];
    out_tri1.p[1] = *inside_points[1];
    out_tri1.t[1] = *inside_tex[1];
    out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0],
                                          *outside_points[0], t);
    out_tri1.t[2].u =
        t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
    out_tri1.t[2].v =
        t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;

    out_tri2.col = in_tri.col;
    out_tri2.p[0] = *inside_points[1];
    out_tri2.t[0] = *inside_tex[1];
    out_tri2.p[1] = out_tri1.p[2];
    out_tri2.t[1] = out_tri1.t[2];
    out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1],
                                          *outside_points[0], t);
    out_tri2.t[2].u =
        t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
    out_tri2.t[2].v =
        t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
    return 2;
  }

  return 0;
}
