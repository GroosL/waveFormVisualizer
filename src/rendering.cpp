#include "../include/rendering.h"

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

  for (int i = 0; i <= (int)bits.size(); i++) {
    float x = startX + i * bitWidth;
    SDL_RenderLine(renderer, x, highY - 20, x, lowY + 20);
  }

  SDL_RenderLine(renderer, startX, centerY, startX + bits.size() * bitWidth,
                 centerY);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void drawBitLabels(SDL_Renderer *renderer, TTF_Font *font,
                   const std::string &bits, float startX, float bitWidth,
                   float bottomTextY) {
  float x = startX;

  for (int i = 0; i < (int)bits.size(); i++) {
    SDL_FRect textRect = {x, bottomTextY, bitWidth, 30};
    std::string bitStr(1, bits[i]);
    desenharTextoCentralizado(renderer, font, bitStr, textRect);
    x += bitWidth;
  }
}

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &text) {
  SDL_Color color = {0, 0, 0, 255};
  SDL_Surface *surf =
      TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_DestroySurface(surf);
  return tex;
}

void desenharTextoCentralizado(SDL_Renderer *renderer, TTF_Font *font,
                               const std::string &text, SDL_FRect rect) {
  SDL_Texture *tex = renderText(renderer, font, text);

  float w, h;
  SDL_GetTextureSize(tex, &w, &h);

  SDL_FRect dst = {rect.x + (rect.w - w) / 2.0f, rect.y + (rect.h - h) / 2.0f,
                   w, h};

  SDL_RenderTexture(renderer, tex, NULL, &dst);
  SDL_DestroyTexture(tex);
}

void drawNRZL(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits) {
  float startX = 50;
  float centerY = 300;

  float bitWidth = 60;
  float highY = centerY - 60;
  float lowY = centerY + 60;
  float bottomTextY = centerY + 100;

  drawWaveGrid(renderer, bits, startX, bitWidth, centerY, highY, lowY);

  SignalLevel previousLevel = SignalLevel::Low;
  float x = startX;

  for (size_t i = 0; i < bits.size(); i++) {
    SignalLevel currentLevel = bits[i] == '1' ? SignalLevel::High
                                              : SignalLevel::Low;
    float previousY = levelY(previousLevel, highY, centerY, lowY);
    float currentY = levelY(currentLevel, highY, centerY, lowY);

    if (i == 0 || currentLevel != previousLevel) {
      SDL_RenderLine(renderer, x, previousY, x, currentY);
    }

    SDL_RenderLine(renderer, x, currentY, x + bitWidth, currentY);
    previousLevel = currentLevel;
    x += bitWidth;
  }

  drawBitLabels(renderer, font, bits, startX, bitWidth, bottomTextY);
}

void drawNRZI(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits) {
  float startX = 50;
  float centerY = 300;

  float bitWidth = 60;
  float highY = centerY - 60;
  float lowY = centerY + 60;
  float bottomTextY = centerY + 100;

  drawWaveGrid(renderer, bits, startX, bitWidth, centerY, highY, lowY);

  SignalLevel previousLevel = SignalLevel::Low;
  float x = startX;

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

    SDL_RenderLine(renderer, x, currentY, x + bitWidth, currentY);
    previousLevel = currentLevel;
    x += bitWidth;
  }

  drawBitLabels(renderer, font, bits, startX, bitWidth, bottomTextY);
}

void drawAMI(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits) {
  float startX = 50;
  float centerY = 300;

  float bitWidth = 60;
  float highY = centerY - 60;
  float lowY = centerY + 60;
  float bottomTextY = centerY + 100;

  drawWaveGrid(renderer, bits, startX, bitWidth, centerY, highY, lowY);

  SignalLevel previousLevel = SignalLevel::Zero;
  bool nextPulseHigh = true;
  float x = startX;

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

    SDL_RenderLine(renderer, x, currentY, x + bitWidth, currentY);
    previousLevel = currentLevel;
    x += bitWidth;
  }

  drawBitLabels(renderer, font, bits, startX, bitWidth, bottomTextY);
}

void drawPseudoTernary(SDL_Renderer *renderer, TTF_Font *font,
                       const std::string &bits) {
  float startX = 50;
  float centerY = 300;

  float bitWidth = 60;
  float highY = centerY - 60;
  float lowY = centerY + 60;
  float bottomTextY = centerY + 100;

  drawWaveGrid(renderer, bits, startX, bitWidth, centerY, highY, lowY);

  SignalLevel previousLevel = SignalLevel::Zero;
  bool nextPulseHigh = true;
  float x = startX;

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

    SDL_RenderLine(renderer, x, currentY, x + bitWidth, currentY);
    previousLevel = currentLevel;
    x += bitWidth;
  }

  drawBitLabels(renderer, font, bits, startX, bitWidth, bottomTextY);
}

void drawManchester(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits) {
    float startX = 50;
    float centerY = 300;

    float bitWidth = 60;
    float highY = centerY - 60;
    float lowY = centerY + 60;
    float bottomTextY = centerY + 100;

    drawWaveGrid(renderer, bits, startX, bitWidth, centerY, highY, lowY);

    SignalLevel previousLevel = bits[0] == '1' ? SignalLevel::Low : SignalLevel::High;
    float x = startX;

    for (size_t i = 0; i < bits.size(); i++) {
        SignalLevel currentLevel = bits[i] == '1' ? SignalLevel::High : SignalLevel::Low;

        SDL_RenderLine(renderer, x + bitWidth/2, lowY, x + bitWidth/2, highY);

        if (previousLevel == currentLevel) {
            SDL_RenderLine(renderer, x, lowY, x, highY);
        }

        if (currentLevel == SignalLevel::High) {
          SDL_RenderLine(renderer, x, lowY, x + bitWidth/2, lowY);
          SDL_RenderLine(renderer, x + bitWidth/2, highY, x + bitWidth, highY);
        }
        if (currentLevel == SignalLevel::Low) {
            SDL_RenderLine(renderer, x, highY, x + bitWidth/2, highY);
            SDL_RenderLine(renderer, x + bitWidth/2, lowY, x + bitWidth, lowY);
        }
        previousLevel = currentLevel;
        x += bitWidth;
    }

    drawBitLabels(renderer, font, bits, startX, bitWidth, bottomTextY);
}

void drawManchesterDiff(SDL_Renderer *renderer, TTF_Font *font, const std::string &bits) {
    float startX = 50;
    float centerY = 300;

    float bitWidth = 60;
    float highY = centerY - 60;
    float lowY = centerY + 60;
    float bottomTextY = centerY + 100;

    drawWaveGrid(renderer, bits, startX, bitWidth, centerY, highY, lowY);

    SignalLevel previousLevel = bits[0] == '1' ? SignalLevel::Low : SignalLevel::High;
    float x = startX;

    for (size_t i = 0; i < bits.size(); i++) {
        SignalLevel currentLevel = bits[i] == '0' ? previousLevel : previousLevel == SignalLevel::High ? SignalLevel::Low : SignalLevel::High;

        SDL_RenderLine(renderer, x + bitWidth/2, lowY, x + bitWidth/2, highY);

        if (currentLevel == SignalLevel::High) {
            SDL_RenderLine(renderer, x, lowY, x + bitWidth/2, lowY);
            SDL_RenderLine(renderer, x + bitWidth/2, highY, x + bitWidth, highY);
        }

        if (currentLevel == SignalLevel::Low) {
            SDL_RenderLine(renderer, x, highY, x + bitWidth/2, highY);
            SDL_RenderLine(renderer, x + bitWidth/2, lowY, x + bitWidth, lowY);
        }

        if (bits[i] == '0') {
            SDL_RenderLine(renderer, x, lowY, x, highY);
        }

        previousLevel = currentLevel;
        x += bitWidth;
    }

    drawBitLabels(renderer, font, bits, startX, bitWidth, bottomTextY);
}