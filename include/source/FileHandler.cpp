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
    std::pair<int, int> ret;
    if (!_save.is_open()) {
        std::cout << "error opening Save.bin!" << std::endl;
        throw 1;
    }
    _save.read((char*)&ret.first, sizeof(ret.first));
    _save.read((char*)&ret.second, sizeof(ret.second));
    return ret;
}
