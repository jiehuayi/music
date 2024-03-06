#include "Math.hpp"

bool isPowerOfTwo(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

void fft(float* inBuffer, size_t stride, int inN, std::vector<float>* outBuffer) {
    if (!isPowerOfTwo(inN)) {
        throw std::runtime_error("ERROR: input buffer must be a power of 2");
    }

    if (outBuffer->size() != (inN * 2)) {
        throw std::runtime_error("ERROR: output buffer must be twice the size\
                of the input buffer");
    }

    return fftCooleyTukey(inBuffer, stride, inN, outBuffer);
}

// Referenced from:
// https://pythonnumericalmethods.berkeley.edu/notebooks/ \
   chapter24.03-Fast-Fourier-Transform.html
// https://github.com/tsoding/musializer/blob/master/src/plug.c#L313
void fftCooleyTukey(float* inBuffer, size_t stride, int inN, 
        std::vector<float>* outBuffer) {
    if (inN == 1) {
        outBuffer->push_back(inBuffer[0]);
        return;
    }

    std::vector<float> even, odd;
    even.reserve(inN / 2);
    odd.reserve(inN / 2);

    for (int i = 0; i < inN; i+=2) {
        even.push_back(inBuffer[i]);
        odd.push_back(inBuffer[i+1]);
    }
    
    size_t rStride = stride * 2;
    int rN = inN / 2;
    fftCooleyTukey(even.data(), 1, inN / 2, outBuffer); // Even
    fftCooleyTukey(odd.data(), 1, inN / 2, outBuffer);

    std::vector<float> factor(inN);
    for (int i = 0; i < inN; ++i) {
        factor[i] = cos(-2 * M_PI * i / inN);
    }

    for (int i = 0; i < inN / 2; ++i) {
        float temp = odd[i] * factor[i];
        outBuffer->push_back(even[i] + temp);
        outBuffer->push_back(even[i] - temp);
    }
}
