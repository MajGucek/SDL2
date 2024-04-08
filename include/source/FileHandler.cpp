#include <FileHandler.h>

FileHandler& FileHandler::getInstance() {
    static FileHandler f;
    return f;
}

void FileHandler::saveGame(int player_hp, int level) {
    _save.open("Save.bin", std::ios::binary | std::ios::app | std::ios::out);
    if (!_save.is_open()) {
        std::cout << "error opening Save.bin!" << std::endl;
        return;
    }
    _save.write((char*)&player_hp, sizeof(player_hp));
    _save.write((char*)&level, sizeof(level));
    _save.close();
}

std::pair<int, int> FileHandler::loadGame() {
    _save.open("Save.bin", std::ios::binary | std::ios::in);
    std::pair<int, int> ret = {100, 0};
    if (!_save.is_open()) {
        std::cout << "error opening Save.bin!" << std::endl;
        return ret;
    }
    _save.read((char*)&ret.first, sizeof(ret.first));
    _save.read((char*)&ret.second, sizeof(ret.second));
    _save.close();
    return ret;
}

void FileHandler::saveResolution(int width, int height) {
    _qol.open("QOL.bin", std::ios::binary | std::ios::app);
    if (!_qol.is_open()) {
        std::cout << "error opening QOL.bin" << std::endl;
        return;
    }
    _qol.write((char*)&width, sizeof(width));
    _qol.write((char*)&height, sizeof(height));
    _qol.close();
}

std::pair<int, int> FileHandler::loadResolution() {
    std::pair<int, int> res = {1920, 1080};
    _qol.open("QOL.bin", std::ios::binary | std::ios::in);
    if (!_qol.is_open()) {
        std::cout << "error opening QOL.bin" << std::endl;
        return res;
    }
    _qol.read((char*)&res.first, sizeof(res.first));
    _qol.read((char*)&res.second, sizeof(res.second));
    _qol.close();
    return res;
}
