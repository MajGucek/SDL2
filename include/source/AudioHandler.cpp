#include <AudioHandler.h>

AudioHandler::AudioHandler() {
    initAudio();
    addSong("background_music", "audio/background_music.mp3");
    addAudio("bonk", "audio/bonk.mp3");
    addAudio("boom", "audio/boom.mp3");
    addSong("circus_clown", "audio/circus_clown.mp3");
    addAudio("death", "audio/death.mp3");
    addAudio("he_hell_naw", "audio/he_hell_naw.mp3");
    addAudio("hell_naw", "audio/hell_naw.mp3");
    addAudio("metal_pipe", "audio/metal_pipe.mp3");
    addAudio("money", "audio/money.mp3");
    addAudio("nerd", "audio/nerd.mp3");
    addSong("piano", "audio/piano.mp3");
    addSong("posnia", "audio/posnia.mp3");
    addAudio("shine", "audio/shine.mp3");
    addSong("startup", "audio/startup.mp3");
    addAudio("usb", "audio/usb.mp3");
    addAudio("usb_out", "audio/usb_out.mp3");
    addAudio("waterphone", "audio/waterphone.mp3");
    addAudio("wha_wha", "audio/wha_wha.mp3");
    addAudio("yawn", "audio/yawn.mp3");
    addAudio("you_died", "audio/you_died.mp3");
    addAudio("zombie_hit", "audio/zombie_hit.mp3");
}

void AudioHandler::initAudio() {
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
}

void AudioHandler::addAudio(std::string name, std::string file_path) {
    _audios.insert({name, file_path});
}

void AudioHandler::addSong(std::string name, std::string file_path) {
    _songs.insert({name, file_path});
}

//
// --- PUBLIC --- //
//

AudioHandler& AudioHandler::getInstance() {
    static AudioHandler audio;
    return audio;
}

void AudioHandler::playAudio(std::string name) {
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

void AudioHandler::playSong(std::string name) {
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

AudioHandler& AudioHandler::playSFX(std::string name) {
    if (_audios.count(name) > 0) {
        // je audio
        playAudio(name);
    } else if (_songs.count(name) > 0) {
        // je song
        playSong(name);
    } else {
        std::cout << "invalid name!" << std::endl;
    }
    return *this;
}

AudioHandler& AudioHandler::stopSFX() {
    stopAudio();
    stopMusic();
    return *this;
}

void AudioHandler::stopAudio() { Mix_HaltChannel(-1); }
void AudioHandler::stopMusic() { Mix_HaltMusic(); }