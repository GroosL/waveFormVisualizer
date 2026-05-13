#include "../include/rendering.h"

constexpr float START_X = 50.0f;
constexpr float CENTER_Y = 300.0f;
constexpr float BIT_WIDTH = 60.0f;
constexpr float BIT_AMPLITUDE = 60.0f;
constexpr float HIGH_Y = (CENTER_Y - BIT_AMPLITUDE);
constexpr float LOW_Y = (CENTER_Y + BIT_AMPLITUDE);

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

  return CENTER_Y;
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

void drawWaveInterface(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits) {
    float bottomTextY = CENTER_Y + 100;

    drawWaveGrid(renderer, bits, START_X, BIT_WIDTH, CENTER_Y, HIGH_Y, LOW_Y);

    drawBitLabels(renderer, font, bits, START_X, BIT_WIDTH, bottomTextY);
}

void drawNRZL(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits);

  SignalLevel previousLevel = SignalLevel::Low;

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = bits[i] == '1' ? SignalLevel::High
                                              : SignalLevel::Low;
    float previousY = levelY(previousLevel, HIGH_Y, CENTER_Y, LOW_Y);
    float currentY = levelY(currentLevel, HIGH_Y, CENTER_Y, LOW_Y);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawNRZI(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits);

  SignalLevel previousLevel = SignalLevel::Low;

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = previousLevel;

    if (bits[i] == '1') {
      currentLevel = (previousLevel == SignalLevel::High) ? SignalLevel::Low
                                                           : SignalLevel::High;
    }

    float previousY = levelY(previousLevel, HIGH_Y, CENTER_Y, LOW_Y);
    float currentY = levelY(currentLevel, HIGH_Y, CENTER_Y, LOW_Y);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawAMI(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits);

  SignalLevel previousLevel = SignalLevel::Zero;
  bool nextPulseHigh = true;

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = SignalLevel::Zero;

    if (bits[i] == '1') {
      currentLevel = nextPulseHigh ? SignalLevel::High : SignalLevel::Low;
      nextPulseHigh = !nextPulseHigh;
    }

    float previousY = levelY(previousLevel, HIGH_Y, CENTER_Y, LOW_Y);
    float currentY = levelY(currentLevel, HIGH_Y, CENTER_Y, LOW_Y);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawPseudoTernary(SDL_Renderer *renderer, TTF_Font *font,
                       const std::string &bits) {
  float x {START_X};

  drawWaveInterface(renderer, font, bits);

  SignalLevel previousLevel = SignalLevel::Zero;
  bool nextPulseHigh = true;

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = SignalLevel::Zero;

    if (bits[i] == '0') {
      currentLevel = nextPulseHigh ? SignalLevel::High : SignalLevel::Low;
      nextPulseHigh = !nextPulseHigh;
    }

    float previousY = levelY(previousLevel, HIGH_Y, CENTER_Y, LOW_Y);
    float currentY = levelY(currentLevel, HIGH_Y, CENTER_Y, LOW_Y);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + BIT_WIDTH, currentY);
    previousLevel = currentLevel;
    x += BIT_WIDTH;
  }
}

void drawManchester(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits) {
    float x {START_X};

    drawWaveInterface(renderer, font, bits);

    SignalLevel previousLevel = bits[0] == '1' ? SignalLevel::Low : SignalLevel::High;

    for (size_t i = 0; i < bits.size(); i++) {
        SignalLevel currentLevel = bits[i] == '1' ? SignalLevel::High : SignalLevel::Low;

        SDL_RenderLine(renderer, x + BIT_WIDTH/2, LOW_Y, x + BIT_WIDTH/2, HIGH_Y);

        if (previousLevel == currentLevel) {
            SDL_RenderLine(renderer, x, LOW_Y, x, HIGH_Y);
        }

        if (currentLevel == SignalLevel::High) {
          SDL_RenderLine(renderer, x, LOW_Y, x + BIT_WIDTH/2, LOW_Y);
          SDL_RenderLine(renderer, x + BIT_WIDTH/2, HIGH_Y, x + BIT_WIDTH, HIGH_Y);
        }
        if (currentLevel == SignalLevel::Low) {
            SDL_RenderLine(renderer, x, HIGH_Y, x + BIT_WIDTH/2, HIGH_Y);
            SDL_RenderLine(renderer, x + BIT_WIDTH/2, LOW_Y, x + BIT_WIDTH, LOW_Y);
        }
        previousLevel = currentLevel;
        x += BIT_WIDTH;
    }
}

void drawManchesterDiff(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits) {
    float x {START_X};

    drawWaveInterface(renderer, font, bits);

    SignalLevel previousLevel = bits[0] == '1' ? SignalLevel::Low : SignalLevel::High;

    for (size_t i = 0; i < bits.size(); i++) {
        SignalLevel currentLevel = bits[i] == '0' ? previousLevel : previousLevel == SignalLevel::High ? SignalLevel::Low : SignalLevel::High;

        SDL_RenderLine(renderer, x + BIT_WIDTH/2, LOW_Y, x + BIT_WIDTH/2, HIGH_Y);

        if (bits[i] == '0') {
            SDL_RenderLine(renderer, x, LOW_Y, x, HIGH_Y);
        }

        if (currentLevel == SignalLevel::High) {
            SDL_RenderLine(renderer, x, LOW_Y, x + BIT_WIDTH/2, LOW_Y);
            SDL_RenderLine(renderer, x + BIT_WIDTH/2, HIGH_Y, x + BIT_WIDTH, HIGH_Y);
        }

        if (currentLevel == SignalLevel::Low) {
            SDL_RenderLine(renderer, x, HIGH_Y, x + BIT_WIDTH/2, HIGH_Y);
            SDL_RenderLine(renderer, x + BIT_WIDTH/2, LOW_Y, x + BIT_WIDTH, LOW_Y);
        }

        previousLevel = currentLevel;
        x += BIT_WIDTH;
    }
}