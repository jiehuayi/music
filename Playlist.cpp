#include "Playlist.hpp"

Playlist::Playlist() {
    // BASS_Init(-1, 44100, 0, nullptr, nullptr);
    _songs = {};
    _volume = 0.5;
    _path = std::string(getenv("HOME")) + "/Music/";
    _activeTrack = nullptr;
    _isTrackPlaying = false;
}

Playlist::Playlist(std::string path) {
    _songs = {};
    _volume = 0.5;
    _path = path;
    _activeTrack = nullptr;
    _isTrackPlaying = false;
}

Playlist::~Playlist() {
}

int Playlist::readPlaylist() {
    try {
        if (std::filesystem::exists(_path) && std::filesystem::is_directory(_path)) {
            for (const auto& entry : std::filesystem::directory_iterator(_path)) {
                std::filesystem::path ext = entry.path().extension();
                std::string extension = ext.u8string();
                if (extension == ".ogg" || extension == ".wav" || extension == ".mp3") {
                    _songs.push_back(entry.path());
                }
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }

    return READ_DIR_SUCCESS;
}

std::vector<std::string> Playlist::getPlaylistSongs() {
    std::vector<std::string> ret = {};

    for (const auto& path : _songs) {
        ret.push_back(path.filename().string());
    }

    return ret;
}

int Playlist::size() {
    return _songs.size();
}

std::string Playlist::path() {
    return _path;
}

void Playlist::incVolume() {
    setVolume(_volume + VOLUME_STEP);
}

void Playlist::decVolume() {
    setVolume(_volume - VOLUME_STEP);
}

void Playlist::setVolume(float volume) {
    _volume = std::fmax(std::fmin(1.0, volume), 0.0);

    if (_activeTrack) {
        _activeTrack->setVolume(_volume);
    }
}

float Playlist::getVolume() {
    return _volume;
}

void Playlist::play(int index) {
    std::filesystem::path selectedTrack = _songs[index];

    _isTrackPlaying = true;

    if (_activeTrack == nullptr || selectedTrack != _activeTrack->path()) {
        _activeTrack = std::make_shared<Track>(selectedTrack);
        _activeTrack->play();
        _activeTrack->setVolume(_volume);
    }  
}

void Playlist::trigger() {
    if (!_activeTrack) return;

    if (_isTrackPlaying) {
        _activeTrack->pause();
    } else {
        _activeTrack->play();
    }

    _isTrackPlaying = !_isTrackPlaying;
}

std::string Playlist::activeSongName() {
    if (_activeTrack) {
        return _activeTrack->name();
    }

    return "?";
}

bool Playlist::isPlaying() {
    return _isTrackPlaying;
}

std::vector<float> Playlist::getFFT() {
    if (_activeTrack) {
        return _activeTrack->getChannelFFT();
    }

    return std::vector<float>(BUFF_SZ, 0.0);
}

double Playlist::getDuration() {
    if (!_activeTrack) {
        return 0.0;
    }

    double ret = _activeTrack->getDuration();

    return ret < 0 ? 0.0 : ret;
}

double Playlist::getPosition() {
    if (!_activeTrack) {
        return 0.0;
    }

    double ret = _activeTrack->getPosition();

    return ret < 0 ? 0.0 : ret;
}
