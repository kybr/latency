#define RTAUDIO_DEBUG
#include "RtAudio.h"
#include <iostream>

int process(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData);

struct AudioIO {
  float *out, *in;
  unsigned frameCount, index;
  bool operator()() {
  }
};

struct Audio {
  RtAudio dac;
  RtAudio::StreamParameters parameters;
  unsigned int sampleRate;
  unsigned int bufferFrames;

  void start(unsigned device = 0, unsigned bufferFrames = 0, unsigned sampleRate = 44100, unsigned outCount = 2, unsigned inCount = 0) {
    this->bufferFrames = bufferFrames;
    this->sampleRate = sampleRate;

    if (dac.getDeviceCount() < 1) {
      std::cout << "\nNo audio devices found!\n";
      exit(0);
    }

    parameters.deviceId = device;
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

    try {
      unsigned int got = bufferFrames;
      dac.openStream(&parameters, NULL, RTAUDIO_SINT16, sampleRate, &got, &process, (void *)this);
      dac.startStream();
      std::cout << "requested " << bufferFrames << " got " << got << std::endl;
    } catch (RtAudioError &e) {
      e.printMessage();
      exit(0);
    }
  }

  void stop() {
    try {
      dac.stopStream();
    } catch (RtAudioError &e) {
      e.printMessage();
    }
    if (dac.isStreamOpen()) dac.closeStream();
  }


  void onSound(
};

int process(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
            double streamTime, RtAudioStreamStatus status, void *userData) {
  assert(status == 0);
  Audio* audio = (audio*)userData;
  audio->onSound(io);
  short *buffer = (short *)outputBuffer;

  short *buffer = (short *)outputBuffer;
  memset(buffer, 0, sizeof(short) * 2 * nBufferFrames);
  *buffer++ = 32767;
  *buffer++ = 32767;

  return 0;
}

