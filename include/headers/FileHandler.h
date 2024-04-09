#pragma once

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
    void saveGame(char player_name[21], int player_hp, int level);
    void saveResolution(int width, int height);
    std::pair<int, int> loadResolution();
    GameSave loadGame();
};