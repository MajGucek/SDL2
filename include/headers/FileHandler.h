#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

struct GameSave {
    char* name;
    int hp, level;
};

class FileHandler {
   private:
    FileHandler() = default;

   public:
    static FileHandler& getInstance();
    void saveGame(const char* player_name, int player_hp, int level);
    void saveResolution(int width, int height);
    std::pair<int, int> loadResolution();
    GameSave loadGame();
};