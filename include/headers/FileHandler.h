#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct GameSave {
    char name[21];
    int hp, level, score;
};

struct GameLoad {
    std::string name;
    int hp, level, score;
};

struct ScoreWrite {
    char name[21];
    int score;
};
struct Score {
    std::string name;
    int score;
};

class FileHandler {
   private:
    FileHandler() = default;
    const int _name_size = 21;

   public:
    static FileHandler& getInstance();
    void saveGame(const char* player_name, int player_hp, int level, int score);
    void saveScore(const char* player_name, int score);
    void saveResolution(int width, int height);
    std::pair<int, int> loadResolution();
    GameLoad loadGame();
    std::vector<Score> loadScores();
};