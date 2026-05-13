#include "../include/ui_manager.h"
#include "../include/rendering.h"

void UIManager::initializeGrid(std::vector<Cell> &grid) {
  int cols = 3, rows = 3;
  float cellW = 180, cellH = 110;
  float spacing = 10;

  float totalW = cols * cellW + (cols - 1) * spacing;

  float startX = (800 - totalW) / 2.0f;
  float startY = 60;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      SDL_FRect r = {startX + x * (cellW + spacing),
                     startY + y * (cellH + spacing), cellW, cellH};
      grid.push_back({r});
    }
  }
}

void UIManager::drawMenuScreen(SDL_Renderer *renderer, TTF_Font *font,
                               const std::vector<Cell> &grid,
                               const InputBox &input, const Button &next) {
  SDL_FRect topInstruction = {0, 12, 800, 32};
  desenharTextoCentralizado(renderer, font, "Q para fechar",
                            topInstruction);

  for (int i = 0; i < (int)grid.size(); i++) {
    auto &c = grid[i];

    if (c.selecionado)
      SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
    else if (c.hover)
      SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    else
      SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);

    SDL_RenderFillRect(renderer, &c.rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &c.rect);

    desenharTextoCentralizado(renderer, font, opcoes[i], c.rect);
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &input.rect);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderRect(renderer, &input.rect);

  desenharTextoCentralizado(
      renderer, font, input.text.empty() ? "INSERIR STRING" : input.text,
      input.rect);

  SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
  SDL_RenderFillRect(renderer, &next.rect);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderRect(renderer, &next.rect);

  desenharTextoCentralizado(renderer, font, next.text, next.rect);
}

void UIManager::drawWaveformScreen(SDL_Renderer *renderer, TTF_Font *font,
                                   const std::string &waveformBits,
                                   const std::string &waveformName) {
  if (waveformName == "NRZ-I") {
    drawNRZI(renderer, font, waveformBits);
  } else if (waveformName == "AMI") {
    drawAMI(renderer, font, waveformBits);
  } else if (waveformName == "Pseudoternario") {
    drawPseudoTernary(renderer, font, waveformBits);
  } else if (waveformName == "NRZ-L") {
    drawNRZL(renderer, font, waveformBits);
  } else if (waveformName == "Manchester") {
    drawManchester(renderer, font, waveformBits);
  } else if (waveformName == "Manchester Dif.") {
      drawManchesterDiff(renderer, font, waveformBits);
  }

  SDL_FRect top = {0, 20, 800, 40};
  std::string title = waveformName.empty() ? "NRZ-L" : waveformName;
  desenharTextoCentralizado(renderer, font, title + " (ESC para voltar)", top);
}

std::string UIManager::getOpcao(int index) {
  if (index >= 0 && index < (int)opcoes.size())
    return opcoes[index];
  return "";
}
