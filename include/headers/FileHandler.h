#pragma once

#include <fstream>
#include <iostream>
#include <string>

class FileHandler {
   private:
    FileHandler() = default;
    std::fstream _save;
    std::fstream _qol;

   public:
    static FileHandler& getInstance();
    void saveGame(int player_hp, int level);
    void saveResolution(int width, int height);
    std::pair<int, int> loadResolution();
    std::pair<int, int> loadGame();
};