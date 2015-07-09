#include <cstring>
#include <cassert>
#include <cstdlib>
#include "RtAudio.h"
#include <iostream>

int id;

int process(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
            double streamTime, RtAudioStreamStatus status, void *userData) {
  assert(status == 0);

  //float *buffer = (float *)outputBuffer;
  //memset(buffer, 0, sizeof(float) * 2 * nBufferFrames);
  short *buffer = (short *)outputBuffer;
  memset(buffer, 0, sizeof(short) * 2 * nBufferFrames);
  *buffer++ = 32767;
  *buffer++ = 32767;
  return 0;
}

struct Audio {
  RtAudio dac;
  RtAudio::StreamParameters parameters;
  unsigned int sampleRate;
  unsigned int bufferFrames;
  double data[2];

  void start(unsigned int bufferFrames = 512, unsigned int sampleRate = 44100) {
    this->bufferFrames = bufferFrames;
    this->sampleRate = sampleRate;

    if (dac.getDeviceCount() < 1) {
      std::cout << "\nNo audio devices found!\n";
      exit(0);
    }

    parameters.deviceId = (id == 1) ? 0 : 1;
    RtAudio::DeviceInfo info;
    info = dac.getDeviceInfo(parameters.deviceId);
    std::cout << "device = " << info.name << std::endl;
    //parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

//RtApiAlsa::getDeviceInfo: snd_pcm_open error for device (default), No such file or directory.

    try {
      unsigned int got = bufferFrames;
      dac.openStream(&parameters, NULL, RTAUDIO_SINT16, sampleRate, &got, &process, (void *)&data);
      //dac.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate, &got, &process, (void *)&data);
      dac.startStream();
      std::cout << "requested " << bufferFrames << " but got " << got << std::endl;
    } catch (RtAudioError &e) {
      e.printMessage();
      exit(0);
    }
  }

  void stop() {
    try {
      // Stop the stream
      dac.stopStream();
    } catch (RtAudioError &e) {
      e.printMessage();
    }
    if (dac.isStreamOpen()) dac.closeStream();
  }
};

int main(int argc, char* argv[]) {
  id = argc;

  Audio audio;

  char input;

  for (unsigned int i = 8192; i > 2; i /= 2) {
    audio.start(i);
    sleep(1);
    audio.stop();
  }
}
