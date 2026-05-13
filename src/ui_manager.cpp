#include "../include/ui_manager.h"
#include "../include/rendering.h"

void UIManager::initializeGrid(std::vector<Cell> &grid) {
  constexpr short cols {3}, rows {3};
  constexpr float cellW {180}, cellH {110};
  constexpr float spacing {10};

  constexpr float totalW {cols * cellW + (cols - 1) * spacing};

  constexpr float startX {(800 - totalW) / 2.0f};
  constexpr float startY {60};

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
  drawCenteredText(renderer, font, "Q para fechar",
                            topInstruction);

  for (size_t i = 0; i < grid.size(); i++) {
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

    drawCenteredText(renderer, font, opcoes[i], c.rect);
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &input.rect);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderRect(renderer, &input.rect);

  drawCenteredText(
      renderer, font, input.text.empty() ? "INSERIR STRING" : input.text,
      input.rect);

  SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
  SDL_RenderFillRect(renderer, &next.rect);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderRect(renderer, &next.rect);

  drawCenteredText(renderer, font, next.text, next.rect);
}

void UIManager::drawWaveformScreen(SDL_Renderer *renderer, TTF_Font *font,
                                   const std::string &waveformBits,
                                   const std::vector<std::string> &waveformNames) {
  auto drawWaveform = [&](const std::string &waveformName, float centerY) {
    if (waveformName == "NRZ-I") {
      drawNRZI(renderer, font, waveformBits, centerY);
    } else if (waveformName == "AMI") {
      drawAMI(renderer, font, waveformBits, centerY);
    } else if (waveformName == "Pseudoternario") {
      drawPseudoTernary(renderer, font, waveformBits, centerY);
    } else if (waveformName == "NRZ-L") {
      drawNRZL(renderer, font, waveformBits, centerY);
    } else if (waveformName == "Manchester") {
      drawManchester(renderer, font, waveformBits, centerY);
    } else if (waveformName == "Manchester Dif.") {
      drawManchesterDiff(renderer, font, waveformBits, centerY);
    }
  };

  if (waveformNames.size() >= 2) {
    SDL_FRect topLabel = {0, 50, 800, 24};
    SDL_FRect bottomLabel = {0, 300, 800, 24};

    drawCenteredText(renderer, font, waveformNames[0], topLabel);
    drawCenteredText(renderer, font, waveformNames[1], bottomLabel);

    drawWaveform(waveformNames[0], 180.0f);
    drawWaveform(waveformNames[1], 420.0f);
  } else if (waveformNames.size() == 1) {
    SDL_FRect topLabel = {0, 50, 800, 24};
    drawCenteredText(renderer, font, waveformNames[0], topLabel);
    drawWaveform(waveformNames[0], 300.0f);
  }

  SDL_FRect top = {0, 20, 800, 40};
  drawCenteredText(renderer, font, "Pressione ESC para voltar",
                            top);
}

std::string UIManager::getOpcao(int index) {
  if (index >= 0 && index < (int)opcoes.size())
    return opcoes[index];
  return "";
}
