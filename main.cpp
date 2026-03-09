#include "includes/engine.h"

int main() {
  olcEngine3D demo;
  if (demo.Construct(1366, 768, 1, 1))
    demo.Start();
  return 0;
}
