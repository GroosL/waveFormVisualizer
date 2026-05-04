#include "../include/input_handler.h"
#include "../include/geometry.h"

void InputHandler::handleMenuInput(SDL_Event &e, std::vector<Cell> &grid,
                                   InputBox &input, int &selecionadoIndex) {
  if (e.type == SDL_EVENT_MOUSE_MOTION) {
    float mx = e.motion.x;
    float my = e.motion.y;

    for (auto &c : grid)
      c.hover = pontoNoRetangulo(mx, my, c.rect);
  }

  if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    float mx = e.button.x;
    float my = e.button.y;

    for (int i = 0; i < (int)grid.size(); i++) {
      if (pontoNoRetangulo(mx, my, grid[i].rect)) {
        selecionadoIndex = i;
        for (auto &c : grid)
          c.selecionado = false;
        grid[i].selecionado = true;
      }
    }

    input.ativo = pontoNoRetangulo(mx, my, input.rect);
  }

  if (e.type == SDL_EVENT_TEXT_INPUT && input.ativo) {
    input.text += e.text.text;
  }

  if (e.type == SDL_EVENT_KEY_DOWN && input.ativo) {
    if (e.key.key == SDLK_BACKSPACE && !input.text.empty()) {
      input.text.pop_back();
    }
  }
}

void InputHandler::handleWaveformInput(SDL_Event &e, Screen &screen) {
  if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
    screen = SCREEN_MENU;
  }
}
