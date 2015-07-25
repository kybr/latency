#define RTAUDIO_DEBUG

#include "Cuttlebone/Cuttlebone.hpp"
#include "network_state.hpp"
#include "RtAudio.h"
#include <iostream>
#include <cstdlib>

using namespace std;

State state = {0};
cuttlebone::Taker<State> taker;

int processAudio(void *outputBuffer, void *inputBuffer,
                 unsigned int nBufferFrames, double streamTime,
                 RtAudioStreamStatus status, void *userData) {
  short *buffer = static_cast<short *>(outputBuffer);
  memset(buffer, 0, sizeof(short) * nBufferFrames);

  if (taker.get(state)) {
    LOG("got %u", state.n);
    for (int i = 0; i < 10; i++) *buffer++ = 32767;
    for (int i = 0; i < 10; i++) *buffer++ = -32767;
  }

  return 0;
}

int main() {
  taker.start();

  RtAudio dac;
  if (dac.getDeviceCount() < 1) {
    std::cout << "\nNo audio devices found!\n";
    exit(0);
  }

  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1;
  parameters.firstChannel = 0;
  unsigned int sampleRate = AUDIO_SAMPLE_RATE;
  unsigned int bufferFrames = AUDIO_BLOCK_SIZE;
  double data[2];

  try {
    dac.openStream(&parameters, NULL, RTAUDIO_SINT16, sampleRate, &bufferFrames,
                   &processAudio, (void *)&data);
    dac.startStream();

  } catch (RtAudioError &e) {
    e.printMessage();
    exit(0);
  }

  getchar();
  taker.stop();

  try {
    dac.stopStream();
  } catch (RtAudioError &e) {
    e.printMessage();
  }
  if (dac.isStreamOpen()) dac.closeStream();
  return 0;
}
