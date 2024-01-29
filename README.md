# WAV-GEN

Waveform generator designed to create waveforms in file or over the air format.

This toolkit is under development and no warranty is expressed or implied with.
USE AT OWN RISK.

## Install

This has largely been developed and tested on Ubuntu 20.04/22.04.

Some functionality is currently tied to python and GNU Radio, but are not required (hopefully).

GNU Radio is assumed to be installed with PyBombs, but if not, just make sure to export
the `PYBOMBS_PREFIX=/usr` (on Ubuntu) for functionality.

### Dependencies

- git
- automake
- autoconfg
- pthread
- fftw
- zmq
- czmq
- boost_system
- libyaml
- pip

#### Submodule Dependency

- liquid-dsp

### Initial steps

Assuming tools and libraries are installed and available

For development ease, the repo expects to be wrapped into a virtual environment

```bash
virtualenv wav-gen
```

Then the following is appended to the `activate` script for linking

```bash
if [[ $LD_LIBRARY_PATH != *${VIRTUAL_ENV}* ]]; then
    export LD_LIBRARY_PATH="${VIRTUAL_ENV}/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
fi
if [[ $LIBRARY_PATH != *${VIRTUAL_ENV}* ]]; then
    export LIBRARY_PATH="${VIRTUAL_ENV}/lib${LIBRARY_PATH:+:$LIBRARY_PATH}"
fi
if [ ! -z ${AUDIO_FOLDER+x} ]; then
    if [ ! -d "/mnt/ramdisk/$(basename $AUDIO_FOLDER)" ]; then
        if [ ! -d "/mnt/ramdisk" ]; then
            sudo mkdir /mnt/ramdisk
        fi
        sudo mount -t tmpfs -o size=15g tmpgs /mnt/ramdisk
        cp -r $AUDIO_FOLDER /mnt/ramdisk/
    fi
    if [ -z ${WAVGEN_AUDIO_FOLDER+x} ]; then
        export WAVGEN_AUDIO_FOLDER="/mnt/ramdisk/$(basename $AUDIO_FOLDER)"
    fi
else
    echo "AUDIO_FOLDER isn't set, so analog waveforms won't work properly unless WAVGEN_AUDIO_FOLDER is set manually"
fi
```

The current functionality of analog waveforms assumes there's an `AUDIO_FOLDER` that
contains wav files that can be used as an analog source. If not set before activating
the environment, manually set `WAVGEN_AUDIO_FOLDER` for automatic audio selection.
It is recommended that the audio be on a temp filesystem in RAM for best performance.

Once the `VIRTUAL_ENV` is setup, the make files should be functional and will install
the submodule `liquid-dsp`, but if already installed just tweak the makefile to skip
this step.

The apps are then created in `build/apps` and if installed will go to `VIRTUAL_ENV/bin`

There are logging programs `logger_server` and `logger_file` that are meant for debugging
and are not documented, use at your discretion.

The main waveform apps then start with `wavgen_` and have built in help commands
```bash
wavgen_linmod -h
```

## The CLI

The CLI relies on python module cli, and the module can be installed with pip. Though it can
be installed, the code execution still takes place from the root folder of the repo.

### Install (as tested)

```bash
(wav-gen) wav-gen$ pip install -e .
```

The CLI operations in client/server configuration and can have multiple servers connected, but
multiple clients can result in unexpected behavior.

### Usage

#### Server Side

```bash
python3 wavgen_cli/server.py --help
usage: server.py [-h] [--addr ADDR] [--port PORT] [--octo-addr OCTO_ADDR] [--octo-port OCTO_PORT]
                 [--uhd-args UHD_ARGS] [--log-server]

optional arguments:
  -h, --help            show this help message and exit
  --addr ADDR           Interface address to bind to. Defaults to internet connection.
  --port PORT           Port to bind to (def: 50000)
  --uhd-args UHD_ARGS   Limit to devices whose flag provided will find (def: all uhd devices)
  --log-server          Use if a log-server is active (meant for debugging)

python3 wavgen_cli/server.py --addr 127.0.0.1
Starting server...started
```

At this point the server is up and bound to `tcp://127.0.0.1:50000` and will have the ability to see
all UHD USRP devices. To constrain to only specific radios `--uhd-args` can be used repeatedly with
any UHD arguments that help find the devices. For example, to limit to only the B2XX series devices

```bash
python3 wavgen_cli/server.py --addr 127.0.0.1 --uhd-args type=b200
```

#### Client Side

```bash
python3 wavgen_cli/client.py --help
usage: client.py [-h] [--conn addr port use_ssh] [--verbose] [--dev] [--log-server]

optional arguments:
  -h, --help            show this help message and exit
  --conn addr port use_ssh
                        Unique interface address to connect to. Default: host<internet connection IP> 50000 False
  --verbose             Should the cli spit out everything?
  --dev                 Should the cli spit out dev messages?
  --log-server          Use if a log-server is active (meant for debugging)
```

To connect to the server above

```bash
python3 wavgen_cli/client.py --conn 127.0.0.1 50000 False --verbose

** -------------------------------------------------------------------------- **
** Command List
** - help
**     -- Show this help menu
** - exit
**     -- Closes the cli
** - shutdown
**     -- Closes both the cli and server
** - get_radios
**     -- The process of how the cli figures out radio numbers
** - get_active
**     -- List of tuples -- each tuple is an active process on the server
** - get_finished
**     -- List of tuples -- each tuple is an finished process on the server
** - get_truth local_filename.json
**     -- Saves all sent signals since start/last call into local_filename.json (overwrites)
** - start_radio <device number> <profile type> <profile name> [options]
**     start_radio 0 profile qpsk frequency 2.45e6
**     -- This starts device 0 (with need args) at the specified frequency
**     -- This start_radio command will be revised
** - run_random [options]
**     run_random radios 0,1 bands 2.4e9,2.5e9 duration_limits 2.0,5.0 \
**         bw_limits 50e3,1e6 profiles qpsk,fsk16
**     -- This starts device 0,1 (with need args) with carrier in [2.4e9,2.5e9] Hz
**        with signals of on time in range [2,5] seconds,
**        with energy bandwidth in range [50e3,1e6] Hz
**        with profiles qpsk and fsk16
**     -- This run_random command will be revised
** - run_script [seed int, ... ] <script.json filepath/ dir with main.json>
**     run_script 1 2 3 4 5 configs
**     run_script 1 2 3 4 5 configs/main.json
**     run_script 1 2 3 4 5 truth_scripts/output-1a-truth.json
**     -- This starts all devices queue by the script
**     -- This run_script command will be revised
** - kill <proc_id>
**     -- Send kill signal to subprocess running a profile
** -------------------------------------------------------------------------- **

(Cmd) 

```

The `--conn` flag can be used multiple times to connect to multiple servers at the same time.
The first command will always need to be `get_radios` in order for any other functionality

```python
(Cmd) get_radios
Radio List (2):
     0 server=0   args='type=b200,serial=30875A1'
     1 server=0   args='type=b200,serial=30E6262'

(Cmd)
```

Manual control can then be used to start one radio at a time.

```python
(Cmd) start_radio 0 static bpsk frequency 2.45e9 gain 70 bw 0.7 rate 1e6
['frequency', '2.45e9', 'gain', '70', 'bw', '0.7', 'rate', '1e6']
frequency 2.45e9
gain 70
bw 0.7
rate 1e6
{'frequency': '2.45e9', 'gain': '70', 'bw': '0.7', 'rate': '1e6'}
src_cl: '0 static bpsk frequency 2.45e9 gain 70 bw 0.7 rate 1e6'
src_sl: ['0', 'static', 'bpsk', 'frequency', '2.45e9', 'gain', '70', 'bw', '0.7', 'rate', '1e6']
sending command: ['start_radio', 'static', '-a type=b200,serial=30875A1', 'bpsk', 'frequency', '2.45e9', 'gain', '70', 'bw', '0.7', 'rate', '1e6']
start_radio 0 static bpsk frequency 2.45e9 gain 70 bw 0.7 rate 1e6
reply: Starting process 1903712 /data/local/wavgen_reports/20240129082959_truth/truth_dev_30875A1_instance_00000.json
```

The process can then be ended with

```python
(Cmd) kill 1903712
sending command: ['kill', '1903712']
kill 1903712
reply: Killing process 1903712
```

On the server side that interaction should produce something like this:
```python
python3 wavgen_cli/server.py --addr 127.0.0│** - get_active
.1 --uhd-args type=b200
Starting server...started
['frequency', '2.45e9', 'gain', '70', 'bw', '0.7', 'rate', '1e6', 'json', '/data/local/wavgen_reports/20240129082959_truth/truth_dev_30875A1_instance_00000.json']
frequency 2.45e9
gain 70
bw 0.7
rate 1e6
json /data/local/wavgen_reports/20240129082959_truth/truth_dev_30875A1_instance_00000.json
{'frequency': '2.45e9', 'gain': '70', 'bw': '0.7', 'rate': '1e6', 'json': '/data/local/wavgen_reports/20240129082959_truth/truth_dev_30875A1_instance_00000.json'}
Making a bpsk profile
RUNNING WITH THIS:
wavgen_linmod -a type=b200,serial=30875A1 -b 0.7 -f 2.45e9 -g 70 -r 1e6 -j /data/local/wavgen_reports/20240129082959_truth/truth_dev_30875A1_instance_00000.json -M bpsk
Using:
  modulation:   bpsk
  freq:         2450000000.000
  rate:         1000000.000
  bw:           0.700
  gain:         70.000
  args:         type=b200,serial=30875A1
  grange:       0.000
  cycle:        2.000
[INFO] [UHD] linux; GNU C++ version 9.4.0; Boost_107100; UHD_4.4.0.SCISRS_ECTB_BRANCH-0-g5fac246b
[INFO] [B200] Detected Device: B210
[INFO] [B200] Loading FPGA image: /opt/gnuradio/v3.10/share/uhd/images/usrp_b210_fpga.bin...
[INFO] [B200] Operating over USB 3.
[INFO] [B200] Detecting internal GPSDO....
[INFO] [GPS] No GPSDO found
[INFO] [B200] Initialize CODEC control...
[INFO] [B200] Initialize Radio control...
[INFO] [B200] Performing register loopback test...
[INFO] [B200] Register loopback test passed
[INFO] [B200] Performing register loopback test...
[INFO] [B200] Register loopback test passed
[INFO] [B200] Setting master clock rate selection to 'automatic'.
[INFO] [B200] Asking for clock rate 16.000000 MHz...
[INFO] [B200] Actually got clock rate 16.000000 MHz.
[INFO] [B200] Asking for clock rate 32.000000 MHz...
[INFO] [B200] Actually got clock rate 32.000000 MHz.
2000000,2500
running (hit CTRL-C to stop)
LINMOD ---> ctrl+c received --> exiting
usrp data transfer complete
Timestamp at program start: cpu sec: 1706535016.234323740
Connecting to radio at: cpu sec: 1706535016.234411716
Starting to send at: cpu sec: 1706535026.952406406
Stopping send at: cpu sec: 1706535032.096147776
Radio should be stopped at: cpu sec: 1706535032.159663200
.json log written to /data/local/wavgen_reports/20240129082959_truth/truth_dev_30875A1_instance_00000.json
```






