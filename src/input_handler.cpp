#include "../include/input_handler.h"
#include "../include/geometry.h"

#include <algorithm>

void InputHandler::handleMenuInput(SDL_Event &e, std::vector<Cell> &grid,
                                   InputBox &input,
                                   std::vector<int> &selecionados) {
  if (e.type == SDL_EVENT_MOUSE_MOTION) {
    float mx {e.motion.x};
    float my {e.motion.y};

    for (auto &c : grid)
      c.hover = pontoNoRetangulo(mx, my, c.rect);
  }

  if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    float mx {e.button.x};
    float my {e.button.y};

    for (size_t i = 0; i < grid.size(); i++) {
      if (pontoNoRetangulo(mx, my, grid[i].rect)) {
        auto it = std::find(selecionados.begin(), selecionados.end(), i);

        if (it != selecionados.end()) {
          grid[i].selecionado = false;
          selecionados.erase(it);
        } else if (selecionados.size() < 2) {
          grid[i].selecionado = true;
          selecionados.push_back(i);
        }

        break;
      }
    }

    input.ativo = pontoNoRetangulo(mx, my, input.rect);
  }

  if (e.type == SDL_EVENT_TEXT_INPUT && input.ativo) {
    if ((e.text.text[0] == '0' || e.text.text[0] == '1') && input.text.length() < 12) {
      input.text += e.text.text;
    }
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
