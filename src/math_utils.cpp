#include "../includes/math_utils.h"
#include <cmath>

vec3d Vector_Add(vec3d &v1, vec3d &v2) {
  return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3d Vector_Sub(vec3d &v1, vec3d &v2) {
  return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3d Vector_Mul(vec3d &v1, float k) { return {v1.x * k, v1.y * k, v1.z * k}; }

vec3d Vector_Div(vec3d &v1, float k) { return {v1.x / k, v1.y / k, v1.z / k}; }

float Vector_DotProduct(vec3d &v1, vec3d &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vector_Length(vec3d &v) { return sqrtf(Vector_DotProduct(v, v)); }

vec3d Vector_Normalize(vec3d &v) {
  float l = Vector_Length(v);
  return {v.x / l, v.y / l, v.z / l};
}

vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2) {
  return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
          v1.x * v2.y - v1.y * v2.x};
}

mat4x4 Matrix_MakeIdentity() {
  mat4x4 m;
  m.m[0][0] = 1.0f;
  m.m[1][1] = 1.0f;
  m.m[2][2] = 1.0f;
  m.m[3][3] = 1.0f;
  return m;
}

mat4x4 Matrix_makeRotationX(float a) {
  mat4x4 m;
  m.m[0][0] = 1.0f;
  m.m[1][1] = cosf(a);
  m.m[1][2] = sinf(a);
  m.m[2][1] = -sinf(a);
  m.m[2][2] = cosf(a);
  m.m[3][3] = 1.0f;
  return m;
}

mat4x4 Matrix_makeRotationY(float a) {
  mat4x4 m;
  m.m[0][0] = cosf(a);
  m.m[0][2] = sinf(a);
  m.m[1][1] = 1.0f;
  m.m[2][0] = -sinf(a);
  m.m[2][2] = cosf(a);
  m.m[3][3] = 1.0f;
  return m;
}

mat4x4 Matrix_makeRotationZ(float a) {
  mat4x4 m;
  m.m[0][0] = cosf(a);
  m.m[0][1] = sinf(a);
  m.m[1][0] = -sinf(a);
  m.m[1][1] = cosf(a);
  m.m[2][2] = 1.0f;
  m.m[3][3] = 1.0f;
  return m;
}

mat4x4 Matrix_MakeTranslation(float x, float y, float z) {
  mat4x4 m = Matrix_MakeIdentity();
  m.m[3][0] = x;
  m.m[3][1] = y;
  m.m[3][2] = z;
  return m;
}

mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear,
                             float fFar) {
  float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * (float)M_PI);
  mat4x4 m;
  m.m[0][0] = fAspectRatio * fFovRad;
  m.m[1][1] = fFovRad;
  m.m[2][2] = fFar / (fFar - fNear);
  m.m[3][2] = (-fFar * fNear) / (fFar - fNear);
  m.m[2][3] = 1.0f;
  return m;
}

mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2) {
  mat4x4 matrix;
  for (int c = 0; c < 4; c++)
    for (int r = 0; r < 4; r++)
      matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] +
                       m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
  return matrix;
}

vec3d Matrix_MultiplyVector(mat4x4 &m, vec3d &i) {
  vec3d v;
  v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
  v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
  v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
  v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
  return v;
}

mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up) {
  vec3d newForward = Vector_Sub(target, pos);
  newForward = Vector_Normalize(newForward);

  vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
  vec3d newUp = Vector_Sub(up, a);
  newUp = Vector_Normalize(newUp);

  vec3d newRight = Vector_CrossProduct(newUp, newForward);

  mat4x4 m;
  m.m[0][0] = newRight.x;
  m.m[0][1] = newRight.y;
  m.m[0][2] = newRight.z;
  m.m[0][3] = 0.0f;
  m.m[1][0] = newUp.x;
  m.m[1][1] = newUp.y;
  m.m[1][2] = newUp.z;
  m.m[1][3] = 0.0f;
  m.m[2][0] = newForward.x;
  m.m[2][1] = newForward.y;
  m.m[2][2] = newForward.z;
  m.m[2][3] = 0.0f;
  m.m[3][0] = pos.x;
  m.m[3][1] = pos.y;
  m.m[3][2] = pos.z;
  m.m[3][3] = 1.0f;
  return m;
}

mat4x4 Matrix_QuickInverse(mat4x4 &m) {
  mat4x4 r;
  r.m[0][0] = m.m[0][0];
  r.m[0][1] = m.m[1][0];
  r.m[0][2] = m.m[2][0];
  r.m[0][3] = 0.0f;
  r.m[1][0] = m.m[0][1];
  r.m[1][1] = m.m[1][1];
  r.m[1][2] = m.m[2][1];
  r.m[1][3] = 0.0f;
  r.m[2][0] = m.m[0][2];
  r.m[2][1] = m.m[1][2];
  r.m[2][2] = m.m[2][2];
  r.m[2][3] = 0.0f;
  r.m[3][0] =
      -(m.m[3][0] * r.m[0][0] + m.m[3][1] * r.m[1][0] + m.m[3][2] * r.m[2][0]);
  r.m[3][1] =
      -(m.m[3][0] * r.m[0][1] + m.m[3][1] * r.m[1][1] + m.m[3][2] * r.m[2][1]);
  r.m[3][2] =
      -(m.m[3][0] * r.m[0][2] + m.m[3][1] * r.m[1][2] + m.m[3][2] * r.m[2][2]);
  r.m[3][3] = 1.0f;
  return r;
}
