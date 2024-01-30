#include "Track.hpp"

Track::Track(std::string path) : _path(std::move(path)) {
    // Initialize BASS
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        std::cerr << "BASS_Init failed\n";
        // You might want to throw an exception or handle the error in some way
    }

    // Load the track
    _channel = BASS_StreamCreateFile(FALSE, _path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
    if (!_channel) {
        std::cerr << "BASS_StreamCreateFile failed\n";
        BASS_Free(); // Clean up BASS on failure
        // You might want to throw an exception or handle the error in some way
    }
}

Track::~Track() {
    // Free resources and close BASS
    BASS_StreamFree(_channel);
    BASS_Free();
}

void Track::play() {
    // Play the track
    BASS_ChannelPlay(_channel, FALSE);
}

void Track::pause() {
    // Pause the track
    BASS_ChannelPause(_channel);
}

std::string Track::path() {
    return _path;
}
