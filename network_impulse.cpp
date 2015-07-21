#define RTAUDIO_DEBUG

#include "Cuttlebone/Cuttlebone.hpp"
#include "network_state.hpp"
#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <wiringPi.h>

using namespace std;

unsigned n = 0;

State state = {0};
cuttlebone::Maker<State> maker("192.168.7.255");

int processAudio(void *outputBuffer, void *inputBuffer,
                 unsigned int nBufferFrames, double streamTime,
                 RtAudioStreamStatus status, void *userData) {

  short* buffer = static_cast<short*>(outputBuffer);
  memset(buffer, 0, sizeof(short) * nBufferFrames);

  if (n == 0) {
    maker.set(state); // broadcast packet over the network
    state.n++;
    buffer[0] = 32767; // make audio click
    digitalWrite(0, HIGH); // raise a GPIO pin
    LOG("sent %u", state.n); // log to the console
  }
  else
    digitalWrite(0, LOW);

  n++;
  if (n == 52)
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
  unsigned int bufferFrames = 512;
  double data[2];

  try {
    unsigned got = bufferFrames;
    dac.openStream(&parameters, NULL, RTAUDIO_SINT16, sampleRate, &got,
                   &processAudio, (void *)&data);
    dac.startStream();

    if (got != bufferFrames) {
      printf("did not get %u audio buffer size. got %u\n", bufferFrames, got);
      exit(1);
    }

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
