#define RTAUDIO_DEBUG

#include "Cuttlebone/Cuttlebone.hpp"
#include "network_state.hpp"
#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <wiringPi.h>

using namespace std;

unsigned n = 0;

cuttlebone::Maker<State> maker("192.168.7.255");

int processAudio(void *outputBuffer, void *inputBuffer,
                 unsigned int nBufferFrames, double streamTime,
                 RtAudioStreamStatus status, void *userData) {

  // send network packet
  //
  if (n == 0) {
    state.n++;
    maker.set(state);
    printf("sent %u\n", state.n);
  }

  // pulse pin
  //
  if (n == 0)
    digitalWrite(0, HIGH);
  else
    digitalWrite(0, LOW);

  n++;
  if (n == 100)
    n = 0;
  return 0;
}

int main() {
  wiringPiSetup();

  maker.start();

  RtAudio dac;
  cout << "rtaudio: " << dac.getVersion() << endl;
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
  maker.stop();

  try {
    dac.stopStream();
  } catch (RtAudioError &e) {
    e.printMessage();
  }
  if (dac.isStreamOpen()) dac.closeStream();
  return 0;
}
