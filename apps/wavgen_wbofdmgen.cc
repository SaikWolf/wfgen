//
#include <getopt.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <csignal>
#include <uhd/usrp/multi_usrp.hpp>

#include "liquid.h"
#include "labels.hh"
#include "wbofdmgen.hh"

// primitive burst definition
struct burst {
    float fc, bw, t0, dur;
    uint8_t type;
    modulation_scheme ms;
    burst(float _fc, float _bw, float _t0, float _dur, modulation_scheme _ms) :
        fc(_fc), bw(_bw), t0(_t0), dur(_dur), type(0), ms(_ms) {}
    void print() {
        printf("  fc:%9.3f MHz, bw:%9.3f kHz, t0:%9.3f ms, dur:%9.3f ms, mod=%s\n",
            fc*1e-6f, bw*1e-3f, t0*1e3f, dur*1e3f, modulation_types[ms].name);
    }
};

// generate a hop of a particular bw and center frequency
void generate_hop(float fc, std::complex<float> * buf, unsigned int buf_len,
        symstreamrcf gen, nco_crcf mixer);

// generate a sequence of hops
std::vector<burst> generate_sequence(float bw, unsigned int hop_dur, unsigned int num_hops,
        std::complex<float> * buf, float center_freq=0.0f, float sample_rate=1.0f,
        modulation_scheme ms = LIQUID_MODEM_QPSK, bool sweep=false);

void export_json(labels *reporter, std::vector<burst> bursts,
        unsigned int num_loops, double start, double loop_time,
        double center_freq, double sample_rate);

static bool continue_running(true);
void signal_interrupt_handler(int) {
    std::cout << "WBOFDM ---> ctrl+c received --> exiting\n";
    continue_running = false;
}

double get_time(){
    return std::chrono::system_clock::now().time_since_epoch().count()*double(1e-9);
}


int main (int argc, char **argv)
{
    /* +General */
    double frequency = 2.46e9;
    double gain = 60.0;
    double rate = 16.0e6f;
    double gain_range = 0.0f;    // software cycle gain range
    double gain_cycle = 2.0f;    // gain cycle duration
    double band = 0;
    double bandwidth = -1;

    /* +Bursty */
    double dwell = 0.5;   // How long should the waveform be on per burst
    double squelch = 0.5; // How long should the burst be silent before next burst
    double period = 5;    // Used in place of 'leave' for a consistent period of bursts

    size_t channel = 0; //? is this necessary
    std::string args{"type=b200"};
    unsigned int nfft = 2400;
    std::string  json{"usrp_wbofdmgen.json"};

    // init time keeping stuff
    const int max_chrono = 10;
    double chrono_time[max_chrono];
    memset(chrono_time, 0, max_chrono * sizeof(double));
    chrono_time[0] = get_time();


    // get cli options
    int dopt;
    while ((dopt = getopt(argc, argv, "hf:r:g:G:j:b:B;d:s:p:c:a")) != EOF)
    {
        switch (dopt)
        {
        case 'h':
        printf("%s [-f <freq:%.1f MHz>] [-r <rate:%.3f MHz>] [-g <gain:%.1f dB>]\n",
                argv[0], frequency*1e-6f, rate*1e-6f, gain);
        printf(" [-G <gain_range:%.1f>] [-j <gain_cycle:%.1f>] [-b <band:%.1f>\n", gain_range, gain_cycle, band);
        printf(" [-B <bandwidth:%.1f>] [-d <dwell:%.1f>] [-s <squelch:%.1f>\n", bandwidth, dwell, squelch);
        printf(" [-p <period:%.1f>] [-c <channel:%ld>] [-args <args:%s>\n", period, channel, args.c_str());
        printf(" [-a <args:%s>] [-c <channel:%lu>]\n", args.c_str(), channel);
        case 'f': frequency  = atof(optarg); break;
        case 'r': rate       = atof(optarg); break;
        case 'g': gain       = atof(optarg); break;
        case 'G': gain_range = atof(optarg); break;
        case 'j': gain_cycle = atof(optarg); break;
        case 'b': band       = atof(optarg); break;
        case 'B': bandwidth  = atof(optarg); break;
        case 'd': dwell      = atof(optarg); break;
        case 's': squelch    = atof(optarg); break;
        case 'p': period     = atof(optarg); break;
        case 'c': channel    = atoi(optarg); break;
        case 'a': args.assign(optarg); break;
        default: exit(1);
        }
    }

    double loop_time = dwell + squelch; // total time for each loop

    printf("Using:\n");
    printf("  freq:        %.3f\n", frequency);
    printf("  rate:        %.3f\n", rate);
    printf("  gain:        %.3f\n", gain);
    printf("  gain_range:  %.3f\n", gain_range);
    printf("  gain_cycle:  %.3f\n", gain_cycle);
    printf("  band:        %.3f\n", gain_cycle);
    printf("  bandwidth:   %.3f\n", gain_cycle);
    printf("  dwell:       %.3f\n", dwell);
    printf("  squelch:     %.3f\n", squelch);
    printf("  period:      %.3f\n", period);
    printf("  channel:     %ld\n",  channel);
    printf("  args:        %s\n",   args.c_str());

    chrono_time[1] = get_time();

    // set up the metadta flags
    uhd::tx_metadata_t md;
    md.start_of_burst = false;  // never SOB when continuous
    md.end_of_burst   = true;  //
    md.has_time_spec  = false;  // set to false to send immediately

    uhd::device_addr_t args_uhd(args);
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args_uhd);

    // try to configure hardware
    usrp->set_tx_rate(rate);
    usrp->set_tx_freq(frequency);
    usrp->set_tx_gain(0);

    // signal generator
    wbofdmgen gen(nfft);

    // stream
    std::vector<size_t> channel_nums;
    channel_nums.push_back(0);
    uhd::stream_args_t stream_args("fc32", "sc16");
    stream_args.channels = channel_nums;
    uhd::tx_streamer::sptr tx_stream = usrp->get_tx_stream(stream_args);
    // send a mini EOB packet
    tx_stream->send("",0,md);
    usrp->set_tx_gain(gain);

    // get actual rate
    rate = usrp->get_tx_rate();

    md.start_of_burst = true;  // never SOB when continuous
    md.end_of_burst   = true;  //
    md.has_time_spec  = true;  // set to false to send immediately

    // vector buffer to send data to USRP
    auto buf_len = gen.get_buf_len();
    std::vector<std::complex<float>> usrp_buffer(buf_len);

    // buffer of buffers? I guess?
    std::vector<std::complex<float> *> bufs(channel_nums.size(),
                                            &usrp_buffer.front());
    std::vector<std::complex<float> *> buf_ptr(channel_nums.size(),
                                               &usrp_buffer.front());

    std::signal(SIGINT, &signal_interrupt_handler);
    std::cout << "running (hit CTRL-C to stop)" << std::endl;
    std::complex<float> *buf = usrp_buffer.data();
    unsigned int count = 0;
    chrono_time[2] = get_time();
    usrp->set_time_now(uhd::time_spec_t(chrono_time[2]),uhd::usrp::multi_usrp::ALL_MBOARDS);

    labels *reporter;
    if (!json.empty())
    {
        reporter = new labels(json.c_str(),"TXDL T","TXDL SG0000001","TXDL S0000001");
        reporter->modulation = "ofdm";
        reporter->start_reports();
    }
    chrono_time[6] = chrono_time[2] + 0.5;
    md.time_spec = uhd::time_spec_t(chrono_time[6]);
    uint64_t xfer_counter = 0;
    uint64_t xfer = 0, xfer_idx = 0;
    size_t xfer_len = 0;


    //! prints LLLLL in here somehwere
    while (continue_running)
    {
        // generate samples to buffer
        gen.generate(buf);

        // send the result to the USRP
        tx_stream->send(bufs, usrp_buffer.size(), md, 1);

        xfer = 0;
        xfer_idx = 0;
        xfer_counter = 0;
        xfer_len = usrp_buffer.size();
        for(size_t cidx = 0; cidx < channel_nums.size(); cidx++){
            buf_ptr[cidx] = bufs[cidx];
        }
        while((xfer == 0 && xfer_len > 0) && continue_running){
            // send the result to the USRP
            //? changed this to 1 because xfer kept returning 0, now it returns the same as xfer_len
            //? but still saying 'L' for late
            xfer = tx_stream->send(buf_ptr, xfer_len, md, 1); 
            printf("xfer(%ld) = xfer_len(%ld)\n", xfer, xfer_len);

            if(xfer < xfer_len){
                printf("xfer(%ld) < xfer_len(%ld)\n", xfer, xfer_len);
                xfer_counter += xfer;
                xfer_len -= xfer;
                xfer_idx += xfer;
                for(size_t cidx = 0; cidx < channel_nums.size(); cidx++){
                    buf_ptr[cidx] = &(bufs[cidx][xfer_idx]);
                }
                xfer = 0;
                if(md.start_of_burst){
                    md.start_of_burst = false;
                    md.has_time_spec  = false;
                }
            }
        }

        md.start_of_burst = true;
        md.has_time_spec  = true;
        chrono_time[6] += loop_time;
        md.time_spec = uhd::time_spec_t(chrono_time[6]);
    }

    // send a mini EOB packet
    md.start_of_burst = false;
    md.end_of_burst = true;
    tx_stream->send("", 0, md);

    chrono_time[3] = get_time();

    // sleep for a small amount of time to allow USRP buffers to flush
    // usleep(100000);
    while (get_time() < chrono_time[6] + xfer_counter / rate)
        ;

    // finished
    printf("usrp data transfer complete\n");

    chrono_time[4] = get_time();

    printf("Timestamp at program start: cpu sec: %15.9lf\n",chrono_time[0]);
    printf("Connecting to radio at: cpu sec: %15.9lf\n",chrono_time[1]);
    printf("Starting to send at: cpu sec: %15.9lf\n",chrono_time[2]);
    printf("Stopping send at: cpu sec: %15.9lf\n",chrono_time[3]);
    printf("Radio should be stopped at: cpu sec: %15.9lf\n",chrono_time[4]);

    // export to .json if requested
    if (!json.empty())
    {
        // export_json(json, bursts, count, start, loop_time, frequency, rate);
        char misc_buf[100];
        memset(misc_buf, 0, 100);
        snprintf(misc_buf, 100, "        \"start_app\": %.9f,\n", chrono_time[0]);
        reporter->cache_to_misc(std::string(misc_buf));
        memset(misc_buf, 0, 100);
        snprintf(misc_buf, 100, "        \"start_dev\": %.9f,\n", chrono_time[1]);
        reporter->cache_to_misc(std::string(misc_buf));
        memset(misc_buf, 0, 100);
        snprintf(misc_buf, 100, "        \"start_tx\": %.9f,\n", chrono_time[2]);
        reporter->cache_to_misc(std::string(misc_buf));
        memset(misc_buf, 0, 100);
        snprintf(misc_buf, 100, "        \"stop_tx\": %.9f,\n", chrono_time[3]);
        reporter->cache_to_misc(std::string(misc_buf));
        memset(misc_buf, 0, 100);
        snprintf(misc_buf, 100, "        \"stop_app\": %.9f,\n", chrono_time[4]);
        reporter->cache_to_misc(std::string(misc_buf));
        
        std::string meta = "        \"command\": \"" + std::string(argv[0]);
        for(int arg_idx = 1; arg_idx < argc; arg_idx++){
            meta += (std::string(" ") + std::string(argv[arg_idx]));
        }
        reporter->cache_to_misc(meta+"\"\n");
    }

    reporter->append(
        chrono_time[2]+0.5,
        double(xfer_counter)/rate,
        frequency,
        rate*bandwidth);
    reporter->activity_type = "unknown"; // FIXME:::: Come back and fix this
    reporter->modulation = "ofdm";
    reporter->protocol = "unknown";
    reporter->modality = "multi_carrier";
    reporter->activity_type = "lowprob_anomaly";
    reporter->device_origin = args;
    reporter->finalize();
    delete reporter;

    return 0;
}

// generate a hop of a particular bandwidth and center frequency
void generate_hop(float fc, std::complex<float> * buf, unsigned int buf_len,
        symstreamrcf gen, nco_crcf mixer)
{
    auto delay = symstreamrcf_get_delay(gen);

    unsigned int dead_time = (unsigned int) (delay + 1.5f);

    // check values
    if (buf_len < 2*dead_time)
        throw std::runtime_error("requested hop duration too small");

    // fill buffer
    unsigned int num_samples_on = buf_len - 2*dead_time;

    symstreamrcf_set_gain(gen, 0.5f);
    symstreamrcf_write_samples(gen, buf, num_samples_on);
    symstreamrcf_set_gain(gen, 0.0f);
    symstreamrcf_write_samples(gen, buf + num_samples_on, 2*dead_time);

    // mix
    nco_crcf_set_frequency(mixer, 2*M_PI*fc);
    nco_crcf_mix_block_up(mixer, buf, buf, buf_len);
}



float get_rand_fc(){
    return randf() - 0.5f;
}
float get_uniform_fc(unsigned int step, unsigned int total_steps){
    return (float)step/(float)total_steps - 0.5f;
}
float get_channel_fc(int step, float span, unsigned int channels){
    float m = span/(float) channels;
    if (step < 0 || (unsigned int) step >= channels)
        step = rand() % channels;
    return m*(float)step - 0.5f*(span-m);
}

// generate a sequence of hops
std::vector<burst> generate_sequence(float bw, unsigned int hop_dur, unsigned int num_hops,
        std::complex<float> * buf, float center_freq, float sample_rate, modulation_scheme ms,
        bool sweep, float span, int num_channels)
{
    // initialize objects
    unsigned int m = 12;
    symstreamrcf gen = symstreamrcf_create_linear(LIQUID_FIRFILT_ARKAISER, bw, m, 0.25f, ms);
    nco_crcf mixer = nco_crcf_create(LIQUID_VCO);

    //
    std::vector<burst> bursts;

    // generate individual hops
    for (auto i=0U; i<num_hops; i++) {
        float fc;
        if(num_channels <= 0){
            fc = sweep ? span*(1-1.2*bw)*get_uniform_fc(i,num_hops) : span*(1-1.2*bw)*get_rand_fc();
        }
        else{
            fc = sweep ? get_channel_fc(i%num_channels,span,num_channels) : get_channel_fc(-1,span,num_channels);
        }
        generate_hop(fc, buf + i*hop_dur, hop_dur, gen, mixer);

        // append to labels
        bursts.emplace_back(center_freq + fc*sample_rate, bw*sample_rate,
            (float)(i*hop_dur)/sample_rate, (float)hop_dur/sample_rate, ms);
    }

    // free objects
    symstreamrcf_destroy(gen);
    nco_crcf_destroy(mixer);

    // return list of bursts
    return bursts;
}


void export_json(labels *reporter, std::vector<burst> bursts,
        unsigned int num_loops, double start, double loop_time,
        double center_freq, double sample_rate)
{
    for (auto loop=0U; loop<num_loops; loop++) {
        for (auto burst_info: bursts) {
            reporter->set_modulation( burst_info.ms );
            reporter->append(
                start + burst_info.t0 + loop*loop_time,
                burst_info.dur,
                burst_info.fc,
                burst_info.bw);
        }
    }
}

