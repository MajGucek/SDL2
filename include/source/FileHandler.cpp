#include <FileHandler.h>

FileHandler& FileHandler::getInstance() {
    static FileHandler f;
    return f;
}

void FileHandler::saveGame(const char* player_name, int player_hp, int level) {
    std::ofstream save("Save.bin", std::ios::binary | std::ios::app);
    if (!save.is_open()) {
        std::cout << "error opening Save.bin!, write" << std::endl;
        return;
    }
    char* name = new char[21];
    strncpy(name, player_name, 21);
    GameSave game_save = {name, player_hp, level};
    save.write((char*)&game_save, sizeof(game_save));
    delete[] name;
}

GameSave FileHandler::loadGame() {
    std::ifstream save("Save.bin", std::ios::binary);
    GameSave ret = {"", 100, 0};
    if (!save.is_open()) {
        std::cout << "error opening Save.bin!, read" << std::endl;
        return ret;
    }
    save.read((char*)&ret, sizeof(ret));
    return ret;
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
