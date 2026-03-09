#define OLC_PGE_APPLICATION

#include "../includes/engine.h"
#include "../includes/clipping.h"
#include "../includes/math_utils.h"
#include <algorithm>
#include <list>
#include <vector>

olcEngine3D::olcEngine3D() { sAppName = "3D Demo"; }

olc::Pixel olcEngine3D::GetColour(float lum) {
  int c = std::max(0, std::min(255, (int)(lum * 255.0f)));
  return olc::Pixel(c, c, c);
}

bool olcEngine3D::OnUserCreate() {
  meshCube.LoadFromObjectFile("assets/mountains.obj");
  matProj = Matrix_MakeProjection(
      90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);
  return true;
}

bool olcEngine3D::OnUserUpdate(float fElapsedTime) {
  vec3d vUp = {0, 1, 0};

  if (GetKey(olc::Key::UP).bHeld)
    vCamera.y += 8.0f * fElapsedTime;
  if (GetKey(olc::Key::DOWN).bHeld)
    vCamera.y -= 8.0f * fElapsedTime;
  if (GetKey(olc::Key::LEFT).bHeld)
    vCamera.x -= 8.0f * fElapsedTime;
  if (GetKey(olc::Key::RIGHT).bHeld)
    vCamera.x += 8.0f * fElapsedTime;

  vec3d vForward = Vector_Mul(vLookDir, 8.0f * fElapsedTime);
  if (GetKey(olc::Key::W).bHeld)
    vCamera = Vector_Add(vCamera, vForward);
  if (GetKey(olc::Key::S).bHeld)
    vCamera = Vector_Sub(vCamera, vForward);
  if (GetKey(olc::Key::A).bHeld)
    fYam -= 2.0f * fElapsedTime;
  if (GetKey(olc::Key::D).bHeld)
    fYam += 2.0f * fElapsedTime;

  Clear(olc::BLACK);

  mat4x4 matRotZ = Matrix_makeRotationZ(fTheta * 0.5f);
  mat4x4 matRotX = Matrix_makeRotationX(fTheta * 0.5f);
  mat4x4 matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 8.0f);
  mat4x4 matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX);
  matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

  vec3d vTarget = {0, 0, 1};
  mat4x4 matCameraRot = Matrix_makeRotationY(fYam);
  vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
  vTarget = Vector_Add(vCamera, vLookDir);

  mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);
  mat4x4 matView = Matrix_QuickInverse(matCamera);

  std::vector<triangle> vecTrianglesToRaster;

  for (auto tri : meshCube.tris) {
    triangle triTransformed, triViewed, triProjected;

    triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
    triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
    triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);
    triTransformed.t[0] = tri.t[0];
    triTransformed.t[1] = tri.t[1];
    triTransformed.t[2] = tri.t[2];

    vec3d line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
    vec3d line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);
    vec3d normal = Vector_CrossProduct(line1, line2);
    normal = Vector_Normalize(normal);

    vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);
    if (Vector_DotProduct(normal, vCameraRay) >= 0.0f)
      continue;

    vec3d light_direction = {0.0f, 1.0f, -1.0f};
    light_direction = Vector_Normalize(light_direction);
    float dp = std::max(0.1f, Vector_DotProduct(light_direction, normal));

    triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
    triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
    triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
    triViewed.t[0] = triTransformed.t[0];
    triViewed.t[1] = triTransformed.t[1];
    triViewed.t[2] = triTransformed.t[2];

    triangle clipped[2];
    int nClipped =
        Triangle_ClipAgainstPlane({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f},
                                  triViewed, clipped[0], clipped[1]);

    for (int n = 0; n < nClipped; n++) {
      triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
      triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
      triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);
      triProjected.t[0] = clipped[n].t[0];
      triProjected.t[1] = clipped[n].t[1];
      triProjected.t[2] = clipped[n].t[2];

      triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
      triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
      triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

      triProjected.p[0].x *= -1.0f;
      triProjected.p[0].y *= -1.0f;
      triProjected.p[1].x *= -1.0f;
      triProjected.p[1].y *= -1.0f;
      triProjected.p[2].x *= -1.0f;
      triProjected.p[2].y *= -1.0f;

      vec3d vOffsetView = {1, 1, 0};
      triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
      triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
      triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);

      triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
      triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
      triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
      triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
      triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
      triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

      triProjected.col = GetColour(dp);
      vecTrianglesToRaster.push_back(triProjected);
    }
  }

  std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(),
            [](triangle &t1, triangle &t2) {
              float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
              float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
              return z1 > z2;
            });

  for (auto &triToRaster : vecTrianglesToRaster) {
    triangle clipped[2];
    std::list<triangle> listTriangles;
    listTriangles.push_back(triToRaster);
    int nNewTriangles = 1;

    for (int p = 0; p < 4; p++) {
      int nTrisToAdd = 0;
      while (nNewTriangles > 0) {
        triangle test = listTriangles.front();
        listTriangles.pop_front();
        nNewTriangles--;

        switch (p) {
        case 0:
          nTrisToAdd =
              Triangle_ClipAgainstPlane({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                                        test, clipped[0], clipped[1]);
          break;
        case 1:
          nTrisToAdd = Triangle_ClipAgainstPlane(
              {0.0f, (float)ScreenHeight() - 1, 0.0f}, {0.0f, -1.0f, 0.0f},
              test, clipped[0], clipped[1]);
          break;
        case 2:
          nTrisToAdd =
              Triangle_ClipAgainstPlane({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
                                        test, clipped[0], clipped[1]);
          break;
        case 3:
          nTrisToAdd = Triangle_ClipAgainstPlane(
              {(float)ScreenWidth() - 1, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, test,
              clipped[0], clipped[1]);
          break;
        }
        for (int w = 0; w < nTrisToAdd; w++)
          listTriangles.push_back(clipped[w]);
      }
      nNewTriangles = listTriangles.size();
    }

    for (auto &t : listTriangles)
      FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y,
                   t.col);
  }

  return true;
}
