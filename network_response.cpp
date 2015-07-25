#define RTAUDIO_DEBUG

#include "Cuttlebone/Cuttlebone.hpp"
#include "network_state.hpp"
#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <wiringPi.h>

using namespace std;

State state = {0};
cuttlebone::Taker<State> taker;

int processAudio(void *outputBuffer, void *inputBuffer,
                 unsigned int nBufferFrames, double streamTime,
                 RtAudioStreamStatus status, void *userData) {

  short* buffer = static_cast<short*>(outputBuffer);
  memset(buffer, 0, sizeof(short) * nBufferFrames);

  if (taker.get(state)) {
    //digitalWrite(0, HIGH);
    LOG("got %u", state.n);
    //buffer[0] = 32767;

    for (int i = 0; i < 10; i++)
      *buffer++ = 32767;
    for (int i = 0; i < 10; i++)
      *buffer++ = -32767;
  }
  //else {
  //  digitalWrite(0, LOW);
  //  buffer[0] = 0;
  //}

  return 0;
}

int main() {
  wiringPiSetup();
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
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 256;
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
