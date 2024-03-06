#pragma once

#include <cmath>
#include <vector>
#include <stdexcept>

//def FFT(x):
//    """
//    A recursive implementation of 
//    the 1D Cooley-Tukey FFT, the 
//    input should have a length of 
//    power of 2. 
//    """
//    N = len(x)
//    
//    if N == 1:
//        return x
//    else:
//        X_even = FFT(x[::2])
//        X_odd = FFT(x[1::2])
//        factor = \
//          np.exp(-2j*np.pi*np.arange(N)/ N)
//        
//        X = np.concatenate(\
//            [X_even+factor[:int(N/2)]*X_odd,
//             X_even+factor[int(N/2):]*X_odd])
//        return X

bool isPowerOfTwo(int n);

void fft(float* inBuffer, size_t stride, int inN, std::vector<float>* outBuffer);
void fftCooleyTukey(float* inBuffer, size_t stide, int inN, 
        std::vector<float>* outBuffer);
