#include <AudioHandler.h>

std::map<std::string, std::string> AudioHelper::_audios;
std::map<std::string, std::string> AudioHelper::_songs;

AudioHandler& AudioHandler::initAudio() {
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
    return *this;
}

AudioHandler& AudioHandler::addAudio(std::string name, std::string file_path) {
    _audio_helper.addAudio(name, file_path);
    return *this;
}
AudioHandler& AudioHandler::addSong(std::string name, std::string file_path) {
    _audio_helper.addSong(name, file_path);
    return *this;
}
AudioHandler& AudioHandler::playAudio(std::string name) {
    _audio_helper.playAudio(name);
    return *this;
}
AudioHandler& AudioHandler::playSong(std::string name) {
    _audio_helper.playSong(name);
    return *this;
}
AudioHandler& AudioHandler::stopAudio() {
    _audio_helper.stopAudio();
    return *this;
}
AudioHandler& AudioHandler::stopSongs() {
    _audio_helper.stopMusic();
    return *this;
}

void AudioHelper::addAudio(std::string name, std::string file_path) {
    _audios.insert({name, file_path});
}
void AudioHelper::playAudio(std::string name) {
    auto file = _audios.find(name);
    if (file != _audios.end()) {
        Mix_Chunk* sfx = Mix_LoadWAV(file->second.c_str());
        if (sfx) {
            Mix_PlayChannel(-1, sfx, 0);
        } else {
            std::cout << "failed to load file!" << std::endl;
        }
    } else {
        std::cout << "file not located!" << std::endl;
    }
}

void AudioHelper::addSong(std::string name, std::string file_path) {
    _songs.insert({name, file_path});
}
void AudioHelper::playSong(std::string name) {
    auto file = _songs.find(name);
    if (file != _songs.end()) {
        Mix_Music* music = Mix_LoadMUS(file->second.c_str());
        if (music) {
            Mix_PlayMusic(music, 0);
        } else {
            std::cout << "failed to load file!" << std::endl;
        }
    } else {
        std::cout << "file not located!" << std::endl;
    }
}

void AudioHelper::stopAudio() { Mix_HaltChannel(-1); }
void AudioHelper::stopMusic() { Mix_HaltMusic(); }