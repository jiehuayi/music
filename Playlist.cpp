#include "Playlist.hpp"

bool Playlist::operator==(const Playlist& other) const {
    return this->_path == other._path;
}

bool Playlist::operator!=(const Playlist& other) const {
    return this->_path != other._path;
}

Playlist::Playlist() {
    _path = std::string(getenv("HOME")) + "/Music/";
    init();
}

Playlist::Playlist(std::string path) {
    _path = path;
    init();
}

void Playlist::init() {
    _songs = {};
    _songsBase = {};
    _volume = 0.5;
    _activeTrack = nullptr;
    _isTrackPlaying = false;
}

Playlist::~Playlist() {
}

int Playlist::readPlaylist() {
    if (!std::filesystem::exists(_path)) {
        throw std::runtime_error("ERROR: no such directory");
    }

    if (!std::filesystem::is_directory(_path)) {
        throw std::runtime_error("ERROR: the given path is not a directory"); 
    }
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_path)) {
            std::filesystem::path ext = entry.path().extension();
            std::string extension = ext.u8string();
            if (extension == ".ogg" || extension == ".wav" || extension == ".mp3") {
                _songs.push_back(entry.path());
            }
        }
    } catch(std::exception& err) {
        throw std::runtime_error(err.what());
    }
    return READ_DIR_SUCCESS;
}

int Playlist::getSize() {
    return _songs.size();
}

std::string Playlist::getPath() {
    return _path;
}

std::vector<std::string> Playlist::getPlaylistSongs() {
    std::vector<std::string> ret = {};

    for (const auto& path : _songs) {
        ret.push_back(path.filename().string());
    }

    return ret;
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

    if (_activeTrack == nullptr || selectedTrack != _activeTrack->getPath()) {
        _activeTrack = std::make_shared<Track>(selectedTrack);
        _activeTrack->play();
        _activeTrack->setVolume(_volume);
    }  
}

void Playlist::trigger() {
    if (!_activeTrack) {
        return;
    }

    if (_isTrackPlaying) {
        _activeTrack->pause();
    } else {
        _activeTrack->play();
    }

    _isTrackPlaying = !_isTrackPlaying;
}

void Playlist::shuffle() {
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(_songs.begin(), _songs.end(), rng);
}

std::string Playlist::activeSongName() {
    if (_activeTrack) {
        return _activeTrack->getName();
    }

    return "?";
}

bool Playlist::isPlaying() {
    return _isTrackPlaying;
}

bool Playlist::isFinished() {
    return _activeTrack ? _activeTrack->isEnd() : false;
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
