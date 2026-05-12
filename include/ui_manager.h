#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include "types.h"

class UIManager {
private:
  std::vector<std::string> opcoes = {
      "NRZ-I",           "NRZ-L",   "AMI",     "Pseudoternario", "Manchester",
      "Manchester Dif.", "Opcao 1", "Opcao 2", "Opcao 3"};

public:
  void initializeGrid(std::vector<Cell> &grid);
  void drawMenuScreen(SDL_Renderer *renderer, TTF_Font *font,
                      const std::vector<Cell> &grid, const InputBox &input,
                      const Button &next);
  void drawWaveformScreen(SDL_Renderer *renderer, TTF_Font *font,
                          const std::string &waveformBits,
                          const std::string &waveformName);
  std::string getOpcao(int index);
};

#endif // UI_MANAGER_H
