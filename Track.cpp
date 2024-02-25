#include "Track.hpp"

Track::Track(std::filesystem::path path) : _path(path) {
    _trackPath = _path.string();
    _trackName = _path.filename().string();
    _channel = BASS_StreamCreateFile(FALSE,
            _trackPath.c_str(),
            0, 0, BASS_SAMPLE_FLOAT);

    if (!_channel) {
        std::cerr << "BASS_StreamCreateFile failed for path: "
            << _trackPath << " [Code: "
            << BASS_ErrorGetCode() << "]" << std::endl;
        BASS_Free();
    }
}

Track::~Track() {
    BASS_StreamFree(_channel);
}

void Track::play() {
    BASS_ChannelPlay(_channel, FALSE);
}

void Track::pause() {
    BASS_ChannelPause(_channel);
}

void Track::setVolume(float volume) {
    BASS_ChannelSetAttribute(_channel,
            BASS_ATTRIB_VOL, volume);
}

bool Track::isEnd() {
    const double threshold = 0.999;
    QWORD pos = BASS_ChannelGetPosition(_channel, BASS_POS_BYTE);
    QWORD total = BASS_ChannelGetLength(_channel, BASS_POS_BYTE);
    return (pos / total) >= threshold;
}

std::filesystem::path Track::path() {
    return _path;
}

std::string Track::name() {
    return _trackName;
}

double Track::getDuration() {
    QWORD pos = BASS_ChannelGetLength(_channel, BASS_POS_BYTE);

    if (pos < 0) {
        return pos;
    }

    return BASS_ChannelBytes2Seconds(_channel, pos);
}

double Track::getPosition() {
    QWORD pos = BASS_ChannelGetPosition(_channel, BASS_POS_BYTE);

    if (pos < 0) {
        return pos;
    }

    return BASS_ChannelBytes2Seconds(_channel, pos);
}

std::vector<float> Track::getChannelFFT() {
    float fftBuffer[BUFF_SZ];
    std::vector<float> fftBufferLite;

    bzero(&fftBuffer, BUFF_SZ);

    DWORD lengthFFT;

    switch(BUFF_SZ) {
        case 128:
            lengthFFT = BASS_DATA_FFT256;
            break;
        case 256:
            lengthFFT = BASS_DATA_FFT512;
            break;
        case 512:
            lengthFFT = BASS_DATA_FFT1024;
            break;
        case 1024:
            lengthFFT = BASS_DATA_FFT2048;
            break;
        case 2048:
            lengthFFT = BASS_DATA_FFT4096;
            break;
    }

    if (BASS_ChannelGetData(_channel, fftBuffer, lengthFFT) == 0b11111111) {
        fftBufferLite = std::vector<float>(BUFF_SZ, 0.0);
        goto RET;
    }

    fftBufferLite = std::vector<float>(fftBuffer, fftBuffer + BUFF_SZ);

RET:
    return fftBufferLite;
}
