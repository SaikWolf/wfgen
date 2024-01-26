
#include <iostream>
#include "wbofdmgen.hh"

wbofdmgen::wbofdmgen(unsigned int _nfft,
                     unsigned int _cplen) :
    nfft(_nfft), cplen(_cplen), gain(new float[nfft])
{
    // TODO: enable cyclic prefix
    cplen = 0;

    // allocate memory for transform(s): repeat per thread
    for (auto i=0U; i<OMP_THREADS; i++) {
        buf_freq[i] = (std::complex<float>*) fftwf_malloc(sizeof(fftwf_complex)*nfft);
        buf_time[i] = (std::complex<float>*) fftwf_malloc(sizeof(fftwf_complex)*nfft);
        fft[i] = fftwf_plan_dft_1d(nfft,(fftwf_complex*)buf_freq[i],(fftwf_complex*)buf_time[i],
                                     FFTW_BACKWARD, FFTW_ESTIMATE);
    }

    // compute subcarrier gains
    unsigned int i0 = (unsigned int)(0.4 * nfft);
    unsigned int i1 = (unsigned int)(0.6 * nfft);
    auto      scale = 0.0f;
    for (auto i=0U; i<nfft; i++) {
        gain[i] = (i==0 || (i > i0 && i < i1)) ? 0.0f : 1.0f;
        scale += gain[i]*gain[i];
    }

    // adjust scale
    scale = 1.0f / sqrtf(scale);
    for (auto i=0U; i<nfft; i++)
        gain[i] *= scale;
}

wbofdmgen::~wbofdmgen()
{
    // repeat per thread
    for (auto i=0U; i<OMP_THREADS; i++) {
        fftwf_free(buf_freq[i]);
        fftwf_free(buf_time[i]);
        fftwf_destroy_plan(fft[i]);
    }
    delete [] gain;
}

void wbofdmgen::generate(std::complex<float> * _buf)
{
    unsigned int i;
    omp_set_num_threads(OMP_THREADS);
#pragma omp parallel for private(i) schedule(static)
    for (i=0U; i<OMP_THREADS; i++)
    {
        int id = omp_get_thread_num();
        //printf("omp_thread_id: %d\n", id);

        // fill buffer with pseudo-random data symbols
        float phi = randf()*2*M_PI;
        float dphi = 1e-2f*randnf();
        for (auto j=0U; j<nfft; j++) {
            float theta = phi + j*j*dphi;
            buf_freq[id][j] = gain[j] * std::polar(1.0f, theta);
        }

        // run transform to get time-domain samples
        fftwf_execute(fft[id]);

        // copy to output buffer
        // TODO: copy cyclic prefix as well
        memmove(_buf + nfft*i, buf_time[id], nfft*sizeof(std::complex<float>));
    }
}

