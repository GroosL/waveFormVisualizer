#include "../include/geometry.h"

bool pontoNoRetangulo(float x, float y, const SDL_FRect &r) {
  return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}
