#pragma once

#include <SDL2x64/SDL_mixer.h>

#include <iostream>
#include <map>
#include <string>
class AudioHelper {
   private:
    static std::map<std::string, std::string> _audios;
    static std::map<std::string, std::string> _songs;

   public:
    void addAudio(std::string name, std::string file_path);
    void playAudio(std::string name);
    void addSong(std::string name, std::string file_path);
    void playSong(std::string name);
    void stopAudio();
    void stopMusic();
};

class AudioHandler {
   private:
    AudioHelper _audio_helper;

   public:
    AudioHandler& initAudio();
    AudioHandler& addSong(std::string name, std::string file_path);
    AudioHandler& addAudio(std::string name, std::string file_path);
    AudioHandler& playSong(std::string name);
    AudioHandler& playAudio(std::string name);
    AudioHandler& stopAudio();
    AudioHandler& stopSongs();
};
