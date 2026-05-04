#ifndef TYPES_H
#define TYPES_H

#include <SDL3/SDL.h>
#include <string>

struct Cell {
  SDL_FRect rect;
  bool hover{false};
  bool selecionado{false};
};

struct Button {
  SDL_FRect rect;
  std::string text;
};

struct InputBox {
  SDL_FRect rect;
  std::string text;
  bool ativo{false};
};

enum Screen { SCREEN_MENU, SCREEN_WAVEFORM };

#endif // TYPES_H
