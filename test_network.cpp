#define RTAUDIO_DEBUG

#include "Cuttlebone/Cuttlebone.hpp"
#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <wiringPi.h>

using namespace std;
bool shouldClick = false;

unsigned clicks = 0;
void callback(void) {
  shouldClick = true;
  printf("%u clicks\n", clicks++);
  fflush(stdout);
}

unsigned n = 0;

int processAudio(void *outputBuffer, void *inputBuffer,
                 unsigned int nBufferFrames, double streamTime,
                 RtAudioStreamStatus status, void *userData) {
  // send network packet

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

  } catch (RtError &e) {
    e.printMessage();
    exit(0);
  }

  getchar();

  try {
    dac.stopStream();
  } catch (RtError &e) {
    e.printMessage();
  }
  if (dac.isStreamOpen()) dac.closeStream();
  return 0;
}
