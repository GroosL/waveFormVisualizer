#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL3/SDL.h>
#include <vector>
#include "types.h"

class InputHandler {
public:
  void handleMenuInput(SDL_Event &e, std::vector<Cell> &grid,
                       InputBox &input, int &selecionadoIndex);

  void handleWaveformInput(SDL_Event &e, Screen &screen);
};

#endif // INPUT_HANDLER_H
