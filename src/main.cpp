#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "types.h"
#include "ui_manager.h"
#include "input_handler.h"
#include "geometry.h"

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  int w = 800;
  int h = 600;

  SDL_Window *window =
      SDL_CreateWindow("Trabalho Comunicacao de Dados", w, h, 0); // O tamanho pra caber 16 bits tem q ser 1060x600
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  TTF_Font *font = TTF_OpenFont("NotoSerif.ttf", 20);

  // Initialize UI components
  UIManager uiManager;
  InputHandler inputHandler;

  std::vector<Cell> grid;
  uiManager.initializeGrid(grid);

  float gridBottom = h * 0.68333f; // Based on grid calculation

  InputBox input = {{(w * 0.625f) / 2.0f, gridBottom + h * 0.06666f, w * 0.375f, 40}, ""};
  Button next = {{(w * 0.85f) / 2.0f, gridBottom + h * 0.16666f, w * 0.15f, 50}, "Proximo"};

  Screen screen = SCREEN_MENU;
  std::string waveformBits;
  std::string selectedWaveform;

  SDL_StartTextInput(window);

  int selecionadoIndex = -1;
  bool running = true;

  while (running) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
        running = false;

      if (screen == SCREEN_MENU) {
        inputHandler.handleMenuInput(e, grid, input, selecionadoIndex);

        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RETURN) {
          waveformBits = input.text;
          selectedWaveform = uiManager.getOpcao(selecionadoIndex);
          screen = SCREEN_WAVEFORM;
        }

        // Check Next button
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
          float mx = e.button.x;
          float my = e.button.y;

          if (pontoNoRetangulo(mx, my, next.rect)) {
            if (selecionadoIndex >= 0 && selecionadoIndex <= 5) {
              waveformBits = input.text;
              selectedWaveform = uiManager.getOpcao(selecionadoIndex);
              screen = SCREEN_WAVEFORM;
            }
          }
        }
      } else if (screen == SCREEN_WAVEFORM) {
        inputHandler.handleWaveformInput(e, screen);
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (screen == SCREEN_MENU) {
      uiManager.drawMenuScreen(renderer, font, grid, input, next);
    } else if (screen == SCREEN_WAVEFORM) {
      uiManager.drawWaveformScreen(renderer, font, waveformBits,
                                   selectedWaveform);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_StopTextInput(window);
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();
  return 0;
}
