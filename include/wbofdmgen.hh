// wide-band OFDM signal generation
#ifndef __WBOFDMGEN_HH__
#define __WBOFDMGEN_HH__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex>
#include <fftw3.h>
#include "liquid.h"

#include <omp.h>
#define OMP_THREADS (16)

// inhereted classes
class wbofdmgen
{
  public:
    wbofdmgen(unsigned int _nfft=4800,
              unsigned int _cplen=20);
    ~wbofdmgen();

    // get expected output buffer length
    unsigned int get_buf_len() const {
        return (nfft+cplen)*OMP_THREADS; }

    //
    void generate(std::complex<float> * _buf);

  protected:
    unsigned int nfft;      // FFT size
    unsigned int cplen;     // cyclic prefix length
    float *      gain;      // subcarrier gains

    // repeat per thread
    std::complex<float> * buf_time[OMP_THREADS];    // shape: (nfft,)
    std::complex<float> * buf_freq[OMP_THREADS];    // shape: (nfft,)
    fftwf_plan            fft[OMP_THREADS];         //
};

#endif /* __WBOFDMGEN_HH__ */

