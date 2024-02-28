#pragma once

#include <SDL2x64/SDL_mixer.h>

#include <iostream>
#include <map>
#include <string>
class AudioHandler {
   private:
    std::map<std::string, std::string> _audios;
    std::map<std::string, std::string> _songs;
    AudioHandler();
    void initAudio();
    void addSong(std::string name, std::string file_path);
    void addAudio(std::string name, std::string file_path);
    void playAudio(std::string name);
    void playSong(std::string name);
    void stopMusic();
    void stopAudio();

   public:
    static AudioHandler& getInstance();
    AudioHandler& playSFX(std::string name);
    AudioHandler& stopSFX();
};
