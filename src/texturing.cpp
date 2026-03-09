#include "../libs/olcPixelGameEngine.h"

void TexturedTriangle(int x1, int y1, float u1, float v1, int x2, int y2,
                      float u2, float v2, int x3, int y3, float u3, float v3,
                      olc::Sprite *sprite, olc::PixelGameEngine *pge) {
  if (y2 < y1) {
    std::swap(y1, y2);
    std::swap(x1, x2);
    std::swap(u1, u2);
    std::swap(v1, v2);
  }

  if (y3 < y1) {
    std::swap(y1, y3);
    std::swap(x1, x3);
    std::swap(u1, u3);
    std::swap(v1, v3);
  }

  if (y3 < y2) {
    std::swap(y2, y3);
    std::swap(x2, x3);
    std::swap(u2, u3);
    std::swap(v2, v3);
  }

  int dx1 = x2 - x1;
  int dy1 = y2 - y1;
  float du1 = u2 - u1;
  float dv1 = v2 - v1;

  int dx2 = x3 - x1;
  int dy2 = y3 - y1;
  float du2 = u3 - u1;
  float dv2 = v3 - v1;

  float tex_u, tex_v;

  float dax_step = 0, dbx_step = 0, du1_step = 0, dv1_step = 0, du2_step = 0,
        dv2_step = 0;

  if (dy1)
    dax_step = dx1 / (float)abs(dy1);
  if (dy2)
    dbx_step = dx2 / (float)abs(dy2);

  if (dy1)
    du1_step = du1 / (float)abs(dy1);
  if (dy1)
    dv1_step = dv1 / (float)abs(dy1);

  if (dy2)
    du2_step = du2 / (float)abs(dy2);
  if (dy2)
    dv2_step = dv2 / (float)abs(dy2);

  if (dy1) {
    for (int i = y1; i <= y2; i++) {
      int ax = x1 + (float)(i - y1) * dax_step;
      int bx = x1 + (float)(i - y1) * dbx_step;

      float tex_su = u1 + (float)(i - y1) * du1_step;
      float tex_sv = v1 + (float)(i - y1) * dv1_step;

      float tex_eu = u1 + (float)(i - y1) * du2_step;
      float tex_ev = v1 + (float)(i - y1) * dv2_step;

      if (ax > bx) {
        std::swap(ax, bx);
        std::swap(tex_su, tex_eu);
        std::swap(tex_sv, tex_ev);
      }

      tex_u = tex_su;
      tex_v = tex_sv;

      float tstep = 1.0f / (float)(bx - ax);
      float t = 0.0f;

      for (int j = ax; j < bx; j++) {
        int tex_x = (1.0f - t) * tex_su + t * tex_eu;
        int tex_y = (1.0f - t) * tex_sv + t * tex_ev;
        pge->Draw(j, i, sprite->GetPixel(tex_x, tex_y));
        t += tstep;
      }
    }

    dy1 = y3 - y2;
    dx1 = x3 - x2;
    du1 = v3 - v2;
    dv1 = u3 - u2;

    if (dy1)
      dax_step = dx1 / (float)abs(dy1);
    if (dy2)
      dbx_step = dx2 / (float)abs(dy2);

    du1_step = 0;
    dv1_step = 0;
    if (dy1)
      dv1_step = dv1 / (float)abs(dy1);
    if (dy1)
      du1_step = du1 / (float)abs(dy1);

    for (int i = y2; i <= y3; i++) {
      int ax = x2 + (float)(i - y2) * dax_step;
      int bx = x1 + (float)(i - y1) * dbx_step;

      float tex_su = u2 + (float)(i - y1) * du1_step;
      float tex_sv = v2 + (float)(i - y1) * dv1_step;

      float tex_eu = u1 + (float)(i - y1) * du2_step;
      float tex_ev = v1 + (float)(i - y1) * dv2_step;

      if (ax > bx) {
        std::swap(ax, bx);
        std::swap(tex_su, tex_eu);
        std::swap(tex_sv, tex_ev);
      }

      tex_u = tex_su;
      tex_v = tex_sv;

      float tstep = 1.0f / (float)(bx - ax);
      float t = 0.0f;

      for (int j = ax; j < bx; j++) {
        int tex_x = (1.0f - t) * tex_su + t * tex_eu;
        int tex_y = (1.0f - t) * tex_sv + t * tex_ev;
        pge->Draw(j, i, sprite->GetPixel(tex_x, tex_y));
        t += tstep;
      }
    }
  }
}
