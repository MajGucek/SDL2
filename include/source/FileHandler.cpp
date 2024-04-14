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
        return;
    }
    GameSave game_save;
    strcpy(game_save.name, player_name);
    game_save.hp = player_hp;
    game_save.level = level;
    game_save.score = score;

    save.write((char*)&game_save, sizeof(game_save));
}

GameLoad FileHandler::loadGame() {
    std::ifstream save("Save.bin", std::ios::binary);
    GameLoad load;
    GameSave game_save;
    if (!save.is_open()) {
        return load;
    }
    save.read((char*)&game_save, sizeof(game_save));

    std::string player_name;
    for (int i = 0; i < strlen(game_save.name); ++i) {
        player_name.push_back(game_save.name[i]);
    }

    load = {player_name, game_save.hp, game_save.level, game_save.score};
    return load;
}

void FileHandler::saveScore(const char* player_name, int score) {
    ScoreWrite s;
    strcpy(s.name, player_name);
    s.score = score;
    if (_scores_saved < 5) {
        std::ofstream leaderboard(
            "Leaderboard.bin", std::ios::binary | std::ios::app | std::ios::in);
        if (!leaderboard.is_open()) {
            std::cout << "error opening Leaderboard.bin!, write" << std::endl;
            return;
        }
        leaderboard.write((char*)&s, sizeof(s));
        ++_scores_saved;
    } else {
        std::ifstream leaderboard("Leaderboard.bin", std::ios::binary);
        ScoreWrite cur;
        std::list<ScoreWrite> scores = {s};
        while (leaderboard.read((char*)&cur, sizeof(cur))) {
            scores.push_back(cur);
        }
        auto min = std::min_element(scores.begin(), scores.end());
        scores.erase(min);
        std::ofstream lead("Leaderboard.bin", std::ios::binary);
        for (const auto& sc : scores) {
            lead.write((char*)&sc, sizeof(sc));
        }
    }
}

std::vector<Score> FileHandler::loadScores() {
    std::ifstream leaderboard("Leaderboard.bin", std::ios::binary);
    std::vector<Score> scores;
    if (!leaderboard.is_open()) {
        return scores;
    }
    ScoreWrite current;
    int count = 0;
    while (leaderboard.read((char*)&current, sizeof(current)) and count <= 5) {
        auto name = std::string(current.name);
        scores.push_back({name, current.score});
        ++count;
    }
    std::sort(scores.begin(), scores.end());
    return scores;
}

void FileHandler::saveResolution(int width, int height) {
    std::ofstream qol("QOL.bin", std::ios::binary);
    if (!qol.is_open()) {
        std::cout << "error opening QOL.bin, write mode" << std::endl;
        return;
    }
    qol.write((char*)&width, sizeof(width));
    qol.write((char*)&height, sizeof(height));
}

void FileHandler::savePlayerPos(int x, int y) { _player_pos.push_back({x, y}); }

void FileHandler::writePlayerPos() {
    std::ofstream player_pos("Replay.bin", std::ios::binary);
    if (!player_pos.is_open()) {
        std::cout << "error opening Replay.bin, write mode" << std::endl;
        return;
    }
    for (const auto& x : _player_pos) {
        player_pos.write((char*)&x, sizeof(x));
    }
}

std::vector<std::pair<int, int>> FileHandler::loadPlayerPos() {
    std::vector<std::pair<int, int>> pos;
    std::ifstream player_pos("Replay.bin", std::ios::binary);
    if (player_pos.is_open()) {
        std::pair<int, int> inst;
        while (player_pos.read((char*)&inst, sizeof(inst))) {
            pos.push_back(inst);
        }
    }
    return pos;
}

std::pair<int, int> FileHandler::loadResolution() {
    std::pair<int, int> res = {1920, 1080};
    std::ifstream qol("QOL.bin", std::ios::binary);
    if (!qol.is_open()) {
        return res;
    }
    qol.read((char*)&res.first, sizeof(res.first));
    qol.read((char*)&res.second, sizeof(res.second));
    return res;
}

bool ScoreWrite::operator<(const ScoreWrite& b) { return score < b.score; }

bool Score::operator<(const Score& b) { return score < b.score; }
