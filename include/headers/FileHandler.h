#pragma once

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#define name_size 21

struct GameSave {
    char name[name_size];
    int hp, level, score;
};

struct GameLoad {
    std::string name;
    int hp, level, score;
};

struct ScoreWrite {
    char name[name_size];
    int score;
    bool operator<(const ScoreWrite& b);
};
struct Score {
    std::string name;
    int score;
    bool operator<(const Score& b);
};

class FileHandler {
   private:
    FileHandler() = default;
    std::vector<std::pair<int, int>> _player_pos;
    int _scores_saved = 0;

   public:
    static FileHandler& getInstance();
    void saveGame(const char* player_name, int player_hp, int level, int score);
    void saveScore(const char* player_name, int score);
    void saveResolution(int width, int height);
    void savePlayerPos(int x, int y);
    void writePlayerPos();
    std::vector<std::pair<int, int>> loadPlayerPos();
    std::pair<int, int> loadResolution();
    GameLoad loadGame();
    std::vector<Score> loadScores();
};