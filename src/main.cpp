#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "types.h"
#include "ui_manager.h"
#include "input_handler.h"
#include "rendering.h"
#include "geometry.h"

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_Window *window =
      SDL_CreateWindow("Trabalho Comunicacao de Dados", 800, 600, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  TTF_Font *font = TTF_OpenFont("NotoSerif.ttf", 20);

  // Initialize UI components
  UIManager uiManager;
  InputHandler inputHandler;

  std::vector<Cell> grid;
  uiManager.initializeGrid(grid);

  float gridBottom = 60 + 3 * 110 + 2 * 10; // Based on grid calculation

  InputBox input = {{(800 - 300) / 2.0f, gridBottom + 40, 300, 40}, ""};
  Button next = {{(800 - 120) / 2.0f, gridBottom + 100, 120, 50}, "Proximo"};

  Screen screen = SCREEN_MENU;
  std::string waveformBits;

  SDL_StartTextInput(window);

  int selecionadoIndex = -1;
  bool running = true;

  while (running) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT)
        running = false;

      if (screen == SCREEN_MENU) {
        inputHandler.handleMenuInput(e, grid, input, selecionadoIndex);

        // Check Next button
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
          float mx = e.button.x;
          float my = e.button.y;

          if (pontoNoRetangulo(mx, my, next.rect)) {
            if (selecionadoIndex == 1) { // NRZ-L
              waveformBits = input.text;
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
      uiManager.drawWaveformScreen(renderer, font, waveformBits);
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
