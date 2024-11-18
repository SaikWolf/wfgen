
#ifndef ANALOG_HH
#define ANALOG_HH

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <complex>
#include <random>
#include <atomic>
#include <vector>
#include <functional>
#include <liquid/liquid.h>

#include "wav.hh"


//********************** ANALOG ******************************
//
// An analog waveform can be represented at complex baseband
//  or real baseband. The major point of note, though, is that
//  the information source originates in the real domain, thus
//  an analog waveform can be thought of the combination of
//  multiple real 'paths' which are combined and then
//  converted to complex baseband through a functional change.
//
// FM is one functional change, while AM can be more directly
//  thought of as the combinations of real paths before
//  the functional change to complex domain.
//
// Another functional change is the Hilbert transform. Though
//  for now let's just assume all real paths are supposed to
//  be double sided.
//
//
//  AMGEN -- Generator of an AM Waveform
//         + made of N real paths
//         (REAL_PATH) -- real information stream
//            + consists of
//               - an information source that can be Analog/Digital
//               - resampler
//               - modulator
//               (AMMOD) -- generates one real path
//                        + consists of
//                             - amplitude shifter
//                             - frequency shifter
//
//
//  FMGEN -- Generator of an FM Waveform
//         + FMMOD on an AM waveform
//         + resampler
//
//
//////////////////////////////////////////////////////////////////

typedef struct ammod_s * ammod;
typedef struct real_path_s * real_path;
typedef struct real_source_s * real_source;
typedef struct message_source_s * message_source;
typedef struct amgen_s * amgen;
typedef struct fmmod_s * fmmod;
typedef struct fmgen_s * fmgen;

typedef enum real_path_type{
    ANALOG,
    DIGITAL,
    MESSAGE,
} real_path_t;

typedef enum real_parameter_type{
    STATIC,
    DYNAMIC,
} real_param_t;

typedef enum analog_source_type{
    INVALID_ANALOG=-1,
    CONSTANT=0,
    SQUARE,
    SAWTOOTH,
    TRIANGLE,
    SINUSOID,
} a_src_t;

typedef enum digital_source_type{
    INVALID_DIGITAL=-1,
    BYTES=10,
    MASK,
    MANCHESTER,
    PPM,
    PDM
} d_src_t;

typedef enum message_source_type{
    INVALID_MESSAGE=-1,
    RANDOM_GAUSS=20,
    RANDOM_UNI,
    WAV_FILE
} m_src_t;

a_src_t str2analog_source_type(char* type);
inline a_src_t str2analog_source_type(const char* type){ return str2analog_source_type((char*)type);}
d_src_t str2digital_source_type(char* type);
inline d_src_t str2digital_source_type(const char* type){ return str2digital_source_type((char*)type);}
m_src_t str2message_source_type(char* type);
inline m_src_t str2message_source_type(const char* type){ return str2message_source_type((char*)type);}
std::string analog_source_type2str(a_src_t type);
std::string digital_source_type2str(d_src_t type);
std::string message_source_type2str(m_src_t type);

struct ammod_s{
    real_source _mod_idx;
    real_source  bias_src;
    real_source  freq_src;
};
ammod ammod_create();
ammod ammod_create(double mod_idx, double bias, double freq);
void ammod_destroy(ammod *mod);
int ammod_step(ammod mod, float *out);
int ammod_nstep(ammod mod, uint32_t n, float *out);

struct real_source_s{ // abstract all the possible sources
    uint8_t         adm_type;       /// analog or digital path?
    uint8_t         sd_type;        /// static or dynamic source
    uint8_t         src_type;       /// analog/digital source type
    uint8_t         itemsize;       /// bytes per item in one channel
    uint8_t         channels;       /// number of interleaved items in a sample
    wav_mode_t      special;  /// Might need some unique tweaks to multi-channel info
    void*           source;         /// struct consisting of source information
};
real_source real_source_create();
real_source real_source_create(real_path_t mode, real_param_t vari, uint8_t src_type,
                                uint8_t itemsize, uint8_t channels, wav_mode_t special);
real_source real_source_create(void* source, uint8_t itemsize, uint8_t channels, wav_mode_t special);
void real_source_destroy(real_source *src);
void real_source_print(real_source path, uint8_t indent=0);
void real_source_fill_buffer(real_source src); // fill up internal buffer
void real_source_reset(real_source src);// clear internal buffer
void real_source_center(real_source src);// put phi into [-pi,pi)
int real_source_release(real_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int real_source_incr(real_source src, double* delta = NULL, float* out = NULL);
int real_source_nincr(real_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int real_source_step(real_source src, float *out = NULL);/// step in the period
int real_source_nstep(real_source src, uint32_t n, float *out = NULL);/// n steps in the period
int real_source_set(real_source src, double *p1=NULL, double *p2=NULL);//change the amp
int real_source_get(real_source src, double *p1=NULL, double *p2=NULL);//get the amp
int real_source_get_stream(real_source src, float *buf, uint32_t len);// fill buffer of length with amp
int real_source_get_stream_ptr(real_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp

///// real source structs
typedef struct constant_source_s * constant_source;
typedef struct square_source_s * square_source;
typedef struct sawtooth_source_s * sawtooth_source;
typedef struct triangle_source_s * triangle_source;
typedef struct sinusoid_source_s * sinusoid_source;
typedef struct bytes_source_s * bytes_source;
typedef struct mask_source_s * mask_source;
typedef struct manchester_source_s * manchester_source;
typedef struct ppm_source_s * ppm_source;
typedef struct pdm_source_s * pdm_source;


///// analog sources
#pragma pack(push, 1)
struct constant_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    double      _amp;
    cbufferf    buffer;
};
struct square_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    double       _amp;
    double      _freq;
    double      _offset;
    double      _incr;
    cbufferf    buffer;
};
struct sawtooth_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    double       _amp;
    double      _freq;
    double      _offset;
    double      _incr;
    cbufferf    buffer;
};
struct triangle_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    double       _amp;
    double      _freq;
    double      _offset;
    double      _incr;
    cbufferf    buffer;
};
struct sinusoid_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    double      _amp;
    double      _freq;
    double      _phi;
    double      _incr;
    double      _2pi;
    cbufferf    buffer;
};
#pragma pack(pop)

constant_source constant_source_create();//gen create
constant_source constant_source_create(float amp, uint32_t buffer_len=1024);//specific create
void constant_source_destroy(constant_source *src);//cleanup
void constant_source_fill_buffer(constant_source src); // fill up internal buffer
void constant_source_reset(constant_source src);// clear internal buffer
int constant_source_release(constant_source src, uint32_t len, uint32_t *actual_len);// release len amount of items
int constant_source_step(constant_source src, float *out = NULL);/// step in the period
int constant_source_nstep(constant_source src, uint32_t n, float *out = NULL);/// n steps in the period
int constant_source_set(constant_source src, float amp);//change the amp
int constant_source_get(constant_source src, float *amp);//get the amp
int constant_source_get_stream(constant_source src, float *buf, uint32_t len);// fill buffer of length with amp
int constant_source_get_stream_ptr(constant_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp

square_source square_source_create();//gen create
square_source square_source_create(float amp, double freq, double period_offset=0, uint32_t buffer_len=1024);//specific create
void square_source_destroy(square_source *src);//cleanup
void square_source_fill_buffer(square_source src); // fill up internal buffer
void square_source_reset(square_source src, double *offset=NULL);// clear internal buffer
void square_source_center(square_source src);// put phi into [-pi,pi)
int square_source_release(square_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int square_source_incr(square_source src, double* delta = NULL, float* out = NULL);
int square_source_nincr(square_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int square_source_step(square_source src, float *out = NULL);/// step in the period
int square_source_nstep(square_source src, uint32_t n, float *out = NULL);/// n steps in the period
int square_source_set(square_source src, double *amp=NULL, double *freq=NULL);//change the amp
int square_source_get(square_source src, double *amp=NULL, double *freq=NULL);//get the amp
int square_source_get_stream(square_source src, float *buf, uint32_t len);// fill buffer of length with amp
int square_source_get_stream_ptr(square_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp

sawtooth_source sawtooth_source_create();//gen create
sawtooth_source sawtooth_source_create(float amp, double freq, double period_offset=0, uint32_t buffer_len=1024);//specific create
void sawtooth_source_destroy(sawtooth_source *src);//cleanup
void sawtooth_source_fill_buffer(sawtooth_source src); // fill up internal buffer
void sawtooth_source_reset(sawtooth_source src, double *offset=NULL);// clear internal buffer
void sawtooth_source_center(sawtooth_source src);// put phi into [-pi,pi)
int sawtooth_source_release(sawtooth_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int sawtooth_source_incr(sawtooth_source src, double* delta = NULL, float* out = NULL);
int sawtooth_source_nincr(sawtooth_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int sawtooth_source_step(sawtooth_source src, float *out = NULL);/// step in the period
int sawtooth_source_nstep(sawtooth_source src, uint32_t n, float *out = NULL);/// n steps in the period
int sawtooth_source_set(sawtooth_source src, double *amp=NULL, double *freq=NULL);//change the amp
int sawtooth_source_get(sawtooth_source src, double *amp=NULL, double *freq=NULL);//get the amp
int sawtooth_source_get_stream(sawtooth_source src, float *buf, uint32_t len);// fill buffer of length with amp
int sawtooth_source_get_stream_ptr(sawtooth_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp

triangle_source triangle_source_create();//gen create
triangle_source triangle_source_create(float amp, double freq, double period_offset=0, uint32_t buffer_len=1024);//specific create
void triangle_source_destroy(triangle_source *src);//cleanup
void triangle_source_fill_buffer(triangle_source src); // fill up internal buffer
void triangle_source_reset(triangle_source src, double *offset=NULL);// clear internal buffer
void triangle_source_center(triangle_source src);// put phi into [-pi,pi)
int triangle_source_release(triangle_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int triangle_source_incr(triangle_source src, double* delta = NULL, float* out = NULL);
int triangle_source_nincr(triangle_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int triangle_source_step(triangle_source src, float *out = NULL);/// step in the period
int triangle_source_nstep(triangle_source src, uint32_t n, float *out = NULL);/// n steps in the period
int triangle_source_set(triangle_source src, double *amp=NULL, double *freq=NULL);//change the amp
int triangle_source_get(triangle_source src, double *amp=NULL, double *freq=NULL);//get the amp
int triangle_source_get_stream(triangle_source src, float *buf, uint32_t len);// fill buffer of length with amp
int triangle_source_get_stream_ptr(triangle_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp

sinusoid_source sinusoid_source_create();//gen create
sinusoid_source sinusoid_source_create(float amp, double freq, double phi=0, uint32_t buffer_len=1024);//specific create
void sinusoid_source_destroy(sinusoid_source *src);//cleanup
void sinusoid_source_fill_buffer(sinusoid_source src); // fill up internal buffer
void sinusoid_source_reset(sinusoid_source src, double* phi=NULL);// clear internal buffer
void sinusoid_source_center(sinusoid_source src);// put phi into [-pi,pi)
int sinusoid_source_release(sinusoid_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int sinusoid_source_incr(sinusoid_source src, double* delta = NULL, float* out = NULL);
int sinusoid_source_nincr(sinusoid_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int sinusoid_source_step(sinusoid_source src, float *out = NULL);/// step in the period
int sinusoid_source_nstep(sinusoid_source src, uint32_t n, float *out = NULL);/// n steps in the period
int sinusoid_source_set(sinusoid_source src, double *amp=NULL, double *freq=NULL);//change the amp
int sinusoid_source_get(sinusoid_source src, double *amp=NULL, double *freq=NULL);//get the amp
int sinusoid_source_get_stream(sinusoid_source src, float *buf, uint32_t len);// fill buffer of length with amp
int sinusoid_source_get_stream_ptr(sinusoid_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp



///// continuous source structs (dsb/pam/?)
typedef struct rand_gauss_source_s * rand_gauss_source;
typedef struct rand_uni_source_s * rand_uni_source;
typedef struct wav_source_s * wav_source;

#pragma pack(push, 1)
struct rand_gauss_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    float _mean;
    float _stdd;
    std::normal_distribution<float>* _dist;
    std::mt19937_64* _rgen;
    cbufferf buffer;
    // std::function<float()> _gen;
};
struct rand_uni_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    float _mini;
    float _maxi;
    std::uniform_real_distribution<float>* _dist;
    std::mt19937_64* _rgen;
    cbufferf buffer;
    // std::function<float()> _gen;
};
struct wav_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    uint8_t     _special;
    wav_reader  wav_r;
    cbufferf buffer;
    // std::function<float()> _gen;
};
#pragma pack(pop)

rand_gauss_source rand_gauss_source_create();//gen create
rand_gauss_source rand_gauss_source_create(float mean, float stdd, uint32_t buffer_len=1024);//specific create
void rand_gauss_source_destroy(rand_gauss_source *src);//cleanup
void rand_gauss_source_fill_buffer(rand_gauss_source src); // fill up internal buffer
void rand_gauss_source_reset(rand_gauss_source src);// clear internal buffer
int rand_gauss_source_release(rand_gauss_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int rand_gauss_source_step(rand_gauss_source src, float *out = NULL);/// step in the period
int rand_gauss_source_nstep(rand_gauss_source src, uint32_t n, float *out = NULL);/// n steps in the period
int rand_gauss_source_set(rand_gauss_source src, double *mean=NULL, double *stdd=NULL);//change the amp
int rand_gauss_source_get(rand_gauss_source src, double *mean=NULL, double *stdd=NULL);//get the amp
int rand_gauss_source_get_stream(rand_gauss_source src, float *buf, uint32_t len);// fill buffer of length with amp
int rand_gauss_source_get_stream_ptr(rand_gauss_source src, float *buf, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp

rand_uni_source rand_uni_source_create();//gen create
rand_uni_source rand_uni_source_create(float mini, float maxi, uint32_t buffer_len=1024);//specific create
void rand_uni_source_destroy(rand_uni_source *src);//cleanup
void rand_uni_source_fill_buffer(rand_uni_source src); // fill up internal buffer
void rand_uni_source_reset(rand_uni_source src);// clear internal buffer
int rand_uni_source_release(rand_uni_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int rand_uni_source_step(rand_uni_source src, float *out = NULL);/// step in the period
int rand_uni_source_nstep(rand_uni_source src, uint32_t n, float *out = NULL);/// n steps in the period
int rand_uni_source_set(rand_uni_source src, double *mini=NULL, double *maxi=NULL);//change the amp
int rand_uni_source_get(rand_uni_source src, double *mini=NULL, double *maxi=NULL);//get the amp
int rand_uni_source_get_stream(rand_uni_source src, float *buf, uint32_t len);// fill buffer of length with amp
int rand_uni_source_get_stream_ptr(rand_uni_source src, float *buf, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp

wav_source wav_source_create();//gen create
wav_source wav_source_create(wav_mode_t mode, wav_reader reader, uint32_t buffer_len=1024);//specific create
void wav_source_destroy(wav_source *src);//cleanup
void wav_source_fill_buffer(wav_source src); // fill up internal buffer
void wav_source_reset(wav_source src);// clear internal buffer
int wav_source_release(wav_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int wav_source_step(wav_source src, float *out = NULL);/// step in the period
int wav_source_nstep(wav_source src, uint32_t n, float *out = NULL);/// n steps in the period
int wav_source_set(wav_source src, wav_reader reader=NULL);
int wav_source_get(wav_source src, wav_reader reader=NULL);
int wav_source_get_stream(wav_source src, float *buf, uint32_t len);// fill buffer of length with amp
int wav_source_get_stream_ptr(wav_source src, float *buf, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp


///// digital sources

#pragma pack(push, 1)
struct bytes_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    cbufferf buffer;
    void *source;
};
struct mask_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    cbufferf buffer;
    void *source;
};
struct manchester_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    cbufferf buffer;
    void *source;
};
struct ppm_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    cbufferf buffer;
    void *source;
};
struct pdm_source_s{
    uint8_t     _adm;
    uint8_t     _sd;
    uint8_t     _src;
    cbufferf buffer;
    void *source;
};
#pragma pack(pop)

bytes_source bytes_source_create();
void bytes_source_destroy(bytes_source *src);
void bytes_source_fill_buffer(bytes_source src); // fill up internal buffer
void bytes_source_reset(bytes_source src);// clear internal buffer
void bytes_source_center(bytes_source src);// put phi into [-pi,pi)
int bytes_source_release(bytes_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int bytes_source_incr(bytes_source src, double* delta = NULL, float* out = NULL);
int bytes_source_nincr(bytes_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int bytes_source_step(bytes_source src, float *out = NULL);/// step in the period
int bytes_source_nstep(bytes_source src, uint32_t n, float *out = NULL);/// n steps in the period
int bytes_source_set(bytes_source src, double *amp=NULL, double *freq=NULL);//change the amp
int bytes_source_get(bytes_source src, double *amp=NULL, double *freq=NULL);//get the amp
int bytes_source_get_stream(bytes_source src, float *buf, uint32_t len);// fill buffer of length with amp
int bytes_source_get_stream_ptr(bytes_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp


mask_source mask_source_create();
void mask_source_destroy(mask_source *src);
void mask_source_fill_buffer(mask_source src); // fill up internal buffer
void mask_source_reset(mask_source src);// clear internal buffer
void mask_source_center(mask_source src);// put phi into [-pi,pi)
int mask_source_release(mask_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int mask_source_incr(mask_source src, double* delta = NULL, float* out = NULL);
int mask_source_nincr(mask_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int mask_source_step(mask_source src, float *out = NULL);/// step in the period
int mask_source_nstep(mask_source src, uint32_t n, float *out = NULL);/// n steps in the period
int mask_source_set(mask_source src, double *amp=NULL, double *freq=NULL);//change the amp
int mask_source_get(mask_source src, double *amp=NULL, double *freq=NULL);//get the amp
int mask_source_get_stream(mask_source src, float *buf, uint32_t len);// fill buffer of length with amp
int mask_source_get_stream_ptr(mask_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp


manchester_source manchester_source_create();
void manchester_source_destroy(manchester_source *src);
void manchester_source_fill_buffer(manchester_source src); // fill up internal buffer
void manchester_source_reset(manchester_source src);// clear internal buffer
void manchester_source_center(manchester_source src);// put phi into [-pi,pi)
int manchester_source_release(manchester_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int manchester_source_incr(manchester_source src, double* delta = NULL, float* out = NULL);
int manchester_source_nincr(manchester_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int manchester_source_step(manchester_source src, float *out = NULL);/// step in the period
int manchester_source_nstep(manchester_source src, uint32_t n, float *out = NULL);/// n steps in the period
int manchester_source_set(manchester_source src, double *amp=NULL, double *freq=NULL);//change the amp
int manchester_source_get(manchester_source src, double *amp=NULL, double *freq=NULL);//get the amp
int manchester_source_get_stream(manchester_source src, float *buf, uint32_t len);// fill buffer of length with amp
int manchester_source_get_stream_ptr(manchester_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp


ppm_source ppm_source_create();
void ppm_source_destroy(ppm_source *src);
void ppm_source_fill_buffer(ppm_source src); // fill up internal buffer
void ppm_source_reset(ppm_source src);// clear internal buffer
void ppm_source_center(ppm_source src);// put phi into [-pi,pi)
int ppm_source_release(ppm_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int ppm_source_incr(ppm_source src, double* delta = NULL, float* out = NULL);
int ppm_source_nincr(ppm_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int ppm_source_step(ppm_source src, float *out = NULL);/// step in the period
int ppm_source_nstep(ppm_source src, uint32_t n, float *out = NULL);/// n steps in the period
int ppm_source_set(ppm_source src, double *amp=NULL, double *freq=NULL);//change the amp
int ppm_source_get(ppm_source src, double *amp=NULL, double *freq=NULL);//get the amp
int ppm_source_get_stream(ppm_source src, float *buf, uint32_t len);// fill buffer of length with amp
int ppm_source_get_stream_ptr(ppm_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp


pdm_source pdm_source_create();
void pdm_source_destroy(pdm_source *src);
void pdm_source_fill_buffer(pdm_source src); // fill up internal buffer
void pdm_source_reset(pdm_source src);// clear internal buffer
void pdm_source_center(pdm_source src);// put phi into [-pi,pi)
int pdm_source_release(pdm_source src, uint32_t len, uint32_t* actual_len);// release len amount of items
int pdm_source_incr(pdm_source src, double* delta = NULL, float* out = NULL);
int pdm_source_nincr(pdm_source src, uint32_t n, double* delta = NULL, float* out = NULL);
int pdm_source_step(pdm_source src, float *out = NULL);/// step in the period
int pdm_source_nstep(pdm_source src, uint32_t n, float *out = NULL);/// n steps in the period
int pdm_source_set(pdm_source src, double *amp=NULL, double *freq=NULL);//change the amp
int pdm_source_get(pdm_source src, double *amp=NULL, double *freq=NULL);//get the amp
int pdm_source_get_stream(pdm_source src, float *buf, uint32_t len);// fill buffer of length with amp
int pdm_source_get_stream_ptr(pdm_source src, float *ptr, uint32_t len, uint32_t &actual_len);// pointer to a buffer of actual length of amp



///// sources handled
///// paths

struct real_path_s{
    double input_fs;
    double output_fs;
    double bandwidth;
    double max_freq;
    double baud_rate;
    double path_gain;
    double fc_offset;
    uint32_t buf_len;
    cbufferf buffer;
    real_source source;
    ammod modulator;
    msresamp_rrrf resampler;
    // Borrow from LIQUID's SYMSTREAMR
    // // allocate memory for main object
    // SYMSTREAMR() q = (SYMSTREAMR()) malloc( sizeof(struct SYMSTREAMR(_s)) );
    // q->symstream = s;
    //
    // // create arbitrary rate resampler
    // float rate = 0.5f / _bw;
    // q->resamp = MSRESAMP(_create)(rate, 60.0f);
    //
    // // sample buffer
    // // NOTE: buffer size must be at least 2^nextpow2( 0.5/bw )
    // q->buf_len = 1 << liquid_nextpow2((unsigned int)ceilf(0.5f/_bw));
    // q->buf = (TO*) malloc(q->buf_len*sizeof(TO));
};

real_path real_path_create();
real_path real_path_create(float input_fs, float output_fs, float bandwidth, float max_freq,
                            float baud_rate, float gain, float fc_offset,
                            real_source source=NULL, ammod modulator=NULL, msresamp_rrrf resampler=NULL);
void real_path_destroy(real_path *path);
void real_path_print(real_path path, uint8_t indent=0);
int real_path_step(real_path path, float *out=NULL);
int real_path_nstep(real_path path, uint32_t n, float *out=NULL);

struct amgen_s{
    uint8_t num_paths;
    real_source mod_idx;
    double* path_gains;
    real_path* paths;
};

amgen amgen_create();
amgen amgen_create(uint8_t num_paths, float mod_idx, double* gains, real_path *paths);
void amgen_destroy(amgen *gen);
int amgen_step(amgen gen, float *out=NULL);
int amgen_nstep(amgen gen, uint32_t n, float *out=NULL);
int amgen_nstep(amgen gen, uint32_t n, std::complex<float> *out=NULL);

struct fmmod_s{
    real_source tone;
    real_source _mod_idx;
};

fmmod fmmod_create();
fmmod fmmod_create(float mod_idx);
void fmmod_destroy(fmmod *mod);
int fmmod_step(fmmod mod, float mesg, std::complex<float> *out=NULL);
int fmmod_nstep(fmmod mod, uint32_t n, float *mesg, std::complex<float> *out=NULL);

struct fmgen_s{
    amgen source;
    fmmod mod;
    uint32_t buf_len;
    cbufferf buffer;
    msresamp_rrrf resampler;
};

fmgen fmgen_create();
fmgen fmgen_create(float mod_idx, amgen gen, msresamp_rrrf resampler=NULL);
void fmgen_destroy(fmgen *gen);
int fmgen_step(fmgen gen, std::complex<float> *out);
int fmgen_nstep(fmgen gen, uint32_t n, std::complex<float> *out);

/////////////////////// ORIGINAL STUFF
// typedef enum real_waveform_type{
//     ANALOG,
//     DIGITAL
// } real_waveform_type_t;
//
// typedef enum real_path_waveform{
//     MANCHESTER,
//     BPSK,
//     DSBSC,
//     DSB,
//     TONE
// } real_path_waveform_t;
//
// typedef enum real_path_special{
//     NONE,
//     MONO,
//     STEREO,
//     RDS,
// } real_path_special_t;
//
// typedef struct real_path_s{
//     real_waveform_type mode;
//     double input_fs;
//     double output_fs;
//     double bandwidth;
//     double baud_rate;
//     double path_gain;
//     double fc_offset;
//     real_path_waveform_t waveform;
//     real_path_special_t special;
// } real_path_t;
// typedef real_path_t* real_path;
//
// typedef struct wav_obj_s{
//     uint8_t err_state;
//     char* filepath;
//     void* filehandle;
//     size_t filesize;
//     uint16_t audio_fmt;
//     uint16_t num_channels;
//     uint32_t sample_rate;
//     uint32_t byte_rate;
//     uint32_t block_align;
//     uint32_t bit_depth;
//     uint8_t data_type;
//     int64_t data_offset;
//     size_t data_len;
//     void* data_ptr;
// } wav_obj_t;
// typedef wav_obj_t* wav_obj;
//
// // #define LIQUID_CBUFFER_MANGLE_UCHAR(name)  LIQUID_CONCAT(cbuffer8,  name)
// // #define LIQUID_CBUFFER_MANGLE_USHORT(name)  LIQUID_CONCAT(cbuffers,  name)
// // #define LIQUID_CBUFFER_MANGLE_UINT(name)  LIQUID_CONCAT(cbufferi,  name)
// // LIQUID_CBUFFER_DEFINE_API(LIQUID_CBUFFER_MANGLE_FLOAT,  uint8_t)
// // LIQUID_CBUFFER_DEFINE_API(LIQUID_CBUFFER_MANGLE_FLOAT,  uint16_t)
// // LIQUID_CBUFFER_DEFINE_API(LIQUID_CBUFFER_MANGLE_FLOAT,  uint32_t)
//
// typedef struct wav_reader_s{
//     uint8_t file_count;
//     uint8_t file_idx;
//     uint8_t itemsize;
//     uint64_t sample_length;
//     uint64_t processed;
//     uint32_t prefetch;
//     real_path_special_t read_as;
//     wav_obj *wavs;
//     uint32_t *offsets;
//     cbufferf buffer;
// } wav_reader_t;
//
// wav_obj open_wav(std::string fp);
// int close_wav(wav_obj* wav_ptr);
// void destroy_wav_obj(wav_obj* wav_ptr);
// void print_wav_info(wav_obj wav, uint8_t mode, uint32_t offset=0, int32_t length=25);
// int index_wav(wav_obj wav, uint32_t start_idx, int32_t stereo_samples, float* out_ptr);
// void real_path_print(real_path path, uint8_t indent=0);
//
// wav_reader wav_reader_create();
// wav_reader wav_reader_create(real_path_special_t mode, uint8_t file_count, char **filepaths, uint32_t prefetch=20000);
// void wav_reader_destroy(wav_reader *wav);
// uint8_t wav_reader_sample_bytes(wav_reader wav);
// uint64_t wav_reader_fill_buffer(wav_reader wav);
// uint64_t wav_reader_data_ptr(wav_reader wav, float* &ptr);
// uint64_t wav_reader_advance(wav_reader wav, uint64_t samples);
// int wav_reader_set_offset(wav_reader wav, uint64_t offset);
// uint64_t wav_reader_get_len(wav_reader wav);
// uint64_t wav_reader_get_offset(wav_reader wav);
// uint64_t wav_reader_samples_processed(wav_reader wav);
//
// typedef enum{
//     INVALID=-1,
//     SINUSOID_TONE,
//     WAV_FILE,
//     REAL_PATH,
//     RAND_GAUSS,
//     RAND_UNI,
// } analog_source_t;
//
// typedef struct tone_s{
//     double _2pi;
//     double phi;
//     double fc;
// } tone_t;
// typedef tone_t* tone_p;
//
// typedef struct fmmod_s{
//     tone_p tone;
//     double mod_idx;
//     double _s;
// } fmmod_t;
// typedef fmmod_t * fmmod;
//
// typedef struct ammod_s{
//     tone_p tone;
//     double mod_idx;
//     double bias;
// } ammod_t;
// typedef ammod_t * ammod;
//
// typedef struct fmgen_s{
//     analog_source_t source_type;
//     fmmod fm;
//     void* source;
// } * fmgen;
//
//
// typedef struct amgen_s{
//     uint8_t source_count;
//     ammod *mods;
//     analog_source_t *source_types;
//     void** sources;
// } * amgen;
//
// tone_p tone_create();
// tone_p tone_create(double fc, double phi_0 = 0.0);
// void tone_destroy(tone_p *tp);
// void tone_get(tone_p t, float *f);
// void tone_incr(tone_p t, double *dp = NULL);
// void tone_incr(tone_p t, float *dp);
// void tone_step(tone_p t, float *f);
// void tone_nstep(tone_p t, float *f, uint32_t n);
// void tone_center(tone_p t);
//
// fmmod fmmod_create();
// void fmmod_set_mod_idx(fmmod f, double h);
// void fmmod_execute(fmmod f, float v, std::complex<float> &o);
//
// fmgen fmgen_create();
// void fmgen_set_source(fmgen gen, analog_source_t ast, void* source);
//
// void fmgen_execute(fmgen gen, std::complex<float> *buf, uint32_t count);
//
// int analog_source_ptr(void* source, analog_source_t type, float *&ptr, uint32_t &count);
// int analog_source_extract(void* source, analog_source_t type, float *buf, uint32_t count);
//
// std::function<float()> make_guass_gen(std::mt19937_64 *rgen=NULL, std::normal_distribution<float> *d=NULL);
// std::function<float()> make_uni_gen(std::mt19937_64 *rgen=NULL, std::uniform_real_distribution<float> *d=NULL);
#endif // ANALOG_HH