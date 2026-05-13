#include "../include/rendering.h"

constexpr float START_X {50.0f};
constexpr float BIT_WIDTH {60.0f};
constexpr float BIT_AMPLITUDE {60.0f};

enum class SignalLevel { High, Zero, Low };

float levelY(SignalLevel level, float highY, float centerY, float lowY) {
  switch (level) {
  case SignalLevel::High:
    return highY;
  case SignalLevel::Zero:
    return centerY;
  case SignalLevel::Low:
    return lowY;
  }

  return centerY;
}

void drawWaveGrid(SDL_Renderer *renderer, const std::string &bits,
                  float startX, float bitWidth, float centerY, float highY,
                  float lowY) {
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

  for (size_t i = 0; i <= bits.size(); i++) {
    float x {startX + i * bitWidth};
    SDL_RenderLine(renderer, x, highY - 20, x, lowY + 20);
  }

  SDL_RenderLine(renderer, startX, centerY, startX + bits.size() * bitWidth,
                 centerY);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void drawBitLabels(SDL_Renderer *renderer, TTF_Font *font,
                   const std::string &bits, float startX, float bitWidth,
                   float bottomTextY) {
  float x {startX};

  for (size_t i = 0; i < bits.size(); i++) {
    SDL_FRect textRect = {x, bottomTextY, bitWidth, 30};
    std::string bitStr(1, bits[i]);
    drawCenteredText(renderer, font, bitStr, textRect);
    x += bitWidth;
  }
}

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &text) {
  SDL_Color color = {0, 0, 0, 255};
  SDL_Surface *surf {TTF_RenderText_Blended(font, text.c_str(), text.size(), color)};
  SDL_Texture *tex {SDL_CreateTextureFromSurface(renderer, surf)};
  SDL_DestroySurface(surf);
  return tex;
}

void drawCenteredText(SDL_Renderer *renderer, TTF_Font *font,
                               const std::string &text, SDL_FRect rect) {
  SDL_Texture *tex {renderText(renderer, font, text)};

  float w, h;
  SDL_GetTextureSize(tex, &w, &h);

  SDL_FRect dst = {rect.x + (rect.w - w) / 2.0f, rect.y + (rect.h - h) / 2.0f,
                   w, h};

  SDL_RenderTexture(renderer, tex, NULL, &dst);
  SDL_DestroyTexture(tex);
}

void drawWaveInterface(SDL_Renderer *renderer, TTF_Font *font,
                       const std::string &bits, float centerY) {
    float highY {centerY - BIT_AMPLITUDE};
    float lowY {centerY + BIT_AMPLITUDE};
    float bottomTextY = centerY + 100;

    drawWaveGrid(renderer, bits, START_X, BIT_WIDTH, centerY, highY, lowY);

    drawBitLabels(renderer, font, bits, START_X, BIT_WIDTH, bottomTextY);
}

void drawNRZL(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits, float centerY) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits, centerY);

  SignalLevel previousLevel = SignalLevel::Low;
  float highY {centerY - BIT_AMPLITUDE};
  float lowY {centerY + BIT_AMPLITUDE};

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = bits[i] == '1' ? SignalLevel::High
                                              : SignalLevel::Low;
    float previousY = levelY(previousLevel, highY, centerY, lowY);
    float currentY = levelY(currentLevel, highY, centerY, lowY);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawNRZI(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits, float centerY) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits, centerY);

  SignalLevel previousLevel = SignalLevel::Low;
  float highY {centerY - BIT_AMPLITUDE};
  float lowY {centerY + BIT_AMPLITUDE};

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = previousLevel;

    if (bits[i] == '1') {
      currentLevel = (previousLevel == SignalLevel::High) ? SignalLevel::Low
                                                           : SignalLevel::High;
    }

    float previousY = levelY(previousLevel, highY, centerY, lowY);
    float currentY = levelY(currentLevel, highY, centerY, lowY);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawAMI(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits,
             float centerY) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits, centerY);

  if (bits.empty()) {
    return;
  }

  SignalLevel previousLevel = SignalLevel::Zero;
  bool nextPulseHigh = true;
  float highY {centerY - BIT_AMPLITUDE};
  float lowY {centerY + BIT_AMPLITUDE};

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = SignalLevel::Zero;

    if (bits[i] == '1') {
      currentLevel = nextPulseHigh ? SignalLevel::High : SignalLevel::Low;
      nextPulseHigh = !nextPulseHigh;
    }

    float previousY = levelY(previousLevel, highY, centerY, lowY);
    float currentY = levelY(currentLevel, highY, centerY, lowY);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawPseudoTernary(SDL_Renderer *renderer, TTF_Font *font,
                       const std::string &bits, float centerY) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits, centerY);

  if (bits.empty()) {
    return;
  }

  SignalLevel previousLevel = SignalLevel::Zero;
  bool nextPulseHigh = true;
  float highY {centerY - BIT_AMPLITUDE};
  float lowY {centerY + BIT_AMPLITUDE};

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = SignalLevel::Zero;

    if (bits[i] == '0') {
      currentLevel = nextPulseHigh ? SignalLevel::High : SignalLevel::Low;
      nextPulseHigh = !nextPulseHigh;
    }

    float previousY = levelY(previousLevel, highY, centerY, lowY);
    float currentY = levelY(currentLevel, highY, centerY, lowY);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawManchester(SDL_Renderer *renderer, TTF_Font *font,
                    const std::string &bits, float centerY) {
    float x {START_X};

    drawWaveInterface(renderer, font, bits, centerY);

    if (bits.empty()) {
      return;
    }

    float highY {centerY - BIT_AMPLITUDE};
    float lowY {centerY + BIT_AMPLITUDE};

    SignalLevel previousLevel = bits[0] == '1' ? SignalLevel::Low : SignalLevel::High;

    for (size_t i = 0; i < bits.size(); i++) {
        SignalLevel currentLevel = bits[i] == '1' ? SignalLevel::High : SignalLevel::Low;

        SDL_RenderLine(renderer, x + BIT_WIDTH/2, lowY, x + BIT_WIDTH/2, highY);

        if (previousLevel == currentLevel) {
            SDL_RenderLine(renderer, x, lowY, x, highY);
        }

        if (currentLevel == SignalLevel::High) {
          SDL_RenderLine(renderer, x, lowY, x + BIT_WIDTH/2, lowY);
          SDL_RenderLine(renderer, x + BIT_WIDTH/2, highY, x + BIT_WIDTH, highY);
        }
        if (currentLevel == SignalLevel::Low) {
            SDL_RenderLine(renderer, x, highY, x + BIT_WIDTH/2, highY);
            SDL_RenderLine(renderer, x + BIT_WIDTH/2, lowY, x + BIT_WIDTH, lowY);
        }
        previousLevel = currentLevel;
        x += BIT_WIDTH;
    }
}

void drawManchesterDiff(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &bits, float centerY) {
    float x {START_X};

    drawWaveInterface(renderer, font, bits, centerY);

    if (bits.empty()) {
      return;
    }

    float highY {centerY - BIT_AMPLITUDE};
    float lowY {centerY + BIT_AMPLITUDE};

    SignalLevel previousLevel = bits[0] == '1' ? SignalLevel::Low : SignalLevel::High;

    for (size_t i = 0; i < bits.size(); i++) {
        SignalLevel currentLevel = bits[i] == '0' ? previousLevel : previousLevel == SignalLevel::High ? SignalLevel::Low : SignalLevel::High;

        SDL_RenderLine(renderer, x + BIT_WIDTH/2, lowY, x + BIT_WIDTH/2, highY);

        if (bits[i] == '0') {
          SDL_RenderLine(renderer, x, lowY, x, highY);
        }

        if (currentLevel == SignalLevel::High) {
          SDL_RenderLine(renderer, x, lowY, x + BIT_WIDTH/2, lowY);
          SDL_RenderLine(renderer, x + BIT_WIDTH/2, highY, x + BIT_WIDTH, highY);
        }

        if (currentLevel == SignalLevel::Low) {
          SDL_RenderLine(renderer, x, highY, x + BIT_WIDTH/2, highY);
          SDL_RenderLine(renderer, x + BIT_WIDTH/2, lowY, x + BIT_WIDTH, lowY);
        }

        previousLevel = currentLevel;
        x += BIT_WIDTH;
    }
}