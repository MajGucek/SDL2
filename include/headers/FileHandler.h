#pragma once

#include <CollisionHandler.h>
#include <EnemyHandler.h>

#include <fstream>
#include <string>

class FileHandler {
   private:
    FileHandler() = default;
    std::fstream _save;

   public:
    FileHandler& getInstance();
    void saveGame(int player_hp, int level);
    std::pair<int, int> loadGame();
};