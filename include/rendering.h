#ifndef RENDERING_H
#define RENDERING_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &text);

void drawCenteredText(SDL_Renderer *renderer, TTF_Font *font,
                               const std::string &text, SDL_FRect rect);

void drawNRZL(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits, float centerY);

void drawNRZI(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits, float centerY);

void drawAMI(SDL_Renderer *renderer, TTF_Font *font,
             const std::string &bits, float centerY);

void drawPseudoTernary(SDL_Renderer *renderer, TTF_Font *font,
                       const std::string &bits, float centerY);

void drawManchester(SDL_Renderer *renderer, TTF_Font *font,
                    const std::string &bits, float centerY);

void drawManchesterDiff(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &bits, float centerY);

#endif // RENDERING_H
