# WAV-GEN

Waveform generator designed to create waveforms in file or over the air format.

## Install

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
