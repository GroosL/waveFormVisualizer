#ifndef RENDERING_H
#define RENDERING_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &text);

void desenharTextoCentralizado(SDL_Renderer *renderer, TTF_Font *font,
                               const std::string &text, SDL_FRect rect);

void drawNRZL(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits);

void drawNRZI(SDL_Renderer *renderer, TTF_Font *font,
              const std::string &bits);

void drawAMI(SDL_Renderer *renderer, TTF_Font *font,
             const std::string &bits);

void drawPseudoTernary(SDL_Renderer *renderer, TTF_Font *font,
                       const std::string &bits);

void drawManchester(SDL_Renderer *renderer, TTF_Font *font,
                    const std::string &bits);

#endif // RENDERING_H
