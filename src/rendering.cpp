#include "../include/rendering.h"

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

  float totalWidth = bits.size() * bitWidth;

  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

  for (int i = 0; i <= (int)bits.size(); i++) {
    float x = startX + i * bitWidth;
    SDL_RenderLine(renderer, x, highY - 20, x, lowY + 20);
  }

  SDL_RenderLine(renderer, startX, centerY, startX + totalWidth, centerY);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  float x = startX;

  for (int i = 0; i < (int)bits.size(); i++) {
    char b = bits[i];
    float y = (b == '1') ? highY : lowY;

    float nextY = y;
    if (i + 1 < (int)bits.size()) {
      nextY = (bits[i + 1] == '1') ? highY : lowY;
    }

    SDL_RenderLine(renderer, x, y, x + bitWidth, y);

    if (i + 1 < (int)bits.size() && y != nextY) {
      SDL_RenderLine(renderer, x + bitWidth, y, x + bitWidth, nextY);
    }

    SDL_FRect textRect = {x, bottomTextY, bitWidth, 30};
    std::string bitStr(1, b);
    desenharTextoCentralizado(renderer, font, bitStr, textRect);

    x += bitWidth;
  }
}
