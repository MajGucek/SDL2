#include <FileHandler.h>

FileHandler& FileHandler::getInstance() {
    static FileHandler f;
    return f;
}

void FileHandler::saveGame(const char* player_name, int player_hp, int level,
                           int score) {
    std::ofstream save("Save.bin", std::ios::binary);
    if (!save.is_open()) {
        std::cout << "error opening Save.bin!, write" << std::endl;
        save.close();
        return;
    }
    GameSave game_save;
    strcpy(game_save.name, player_name);
    game_save.hp = player_hp;
    game_save.level = level;
    game_save.score = score;

    save.write((char*)&game_save, sizeof(game_save));
    save.close();
}

GameLoad FileHandler::loadGame() {
    std::ifstream save("Save.bin", std::ios::binary);
    GameLoad load;
    GameSave game_save;
    if (!save.is_open()) {
        save.close();
        return load;
    }
    save.read((char*)&game_save, sizeof(game_save));

    std::string player_name;
    for (int i = 0; i < strlen(game_save.name); ++i) {
        player_name.push_back(game_save.name[i]);
    }

    load = {player_name, game_save.hp, game_save.level, game_save.score};
    save.close();
    return load;
}

void FileHandler::saveScore(const char* player_name, int score) {
    std::ofstream leaderboard("Leaderboard.bin",
                              std::ios::binary | std::ios::app | std::ios::in);
    if (!leaderboard.is_open()) {
        std::cout << "error opening Leaderboard.bin!, write" << std::endl;
        leaderboard.close();
        return;
    }
    ScoreWrite s;
    strcpy(s.name, player_name);
    s.score = score;
    leaderboard.write((char*)&s, sizeof(s));
    leaderboard.close();
}

std::vector<Score> FileHandler::loadScores() {
    std::ifstream leaderboard("Leaderboard.bin", std::ios::binary);
    std::vector<Score> scores;
    if (!leaderboard.is_open()) {
        std::cout << "error opening Leaderboard.bin!, read" << std::endl;
        leaderboard.close();
        return scores;
    }
    ScoreWrite current;
    int count = 0;
    while (leaderboard.read((char*)&current, sizeof(current)) and count <= 5) {
        auto name = std::string(current.name);
        scores.push_back({name, current.score});
        ++count;
    }
    leaderboard.close();
    return scores;
}

void FileHandler::saveResolution(int width, int height) {
    std::ofstream qol("QOL.bin", std::ios::binary | std::ios::app);
    if (!qol.is_open()) {
        std::cout << "error opening QOL.bin, write mode" << std::endl;
        return;
    }
    qol.write((char*)&width, sizeof(width));
    qol.write((char*)&height, sizeof(height));
}

std::pair<int, int> FileHandler::loadResolution() {
    std::pair<int, int> res = {1920, 1080};
    std::ifstream qol("QOL.bin", std::ios::binary);
    if (!qol.is_open()) {
        std::cout << "error opening QOL.bin, read mode" << std::endl;
        return res;
    }
    qol.read((char*)&res.first, sizeof(res.first));
    qol.read((char*)&res.second, sizeof(res.second));
    return res;
}
