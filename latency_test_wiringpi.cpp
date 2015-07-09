#define RTAUDIO_DEBUG

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <wiringPi.h>



using namespace std; bool shouldClick = false;

unsigned clicks = 0;
void callback(void) {
  shouldClick = true;
  printf("%u clicks\n", clicks++);
  fflush(stdout);
}


int processAudio(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData) {


  float* ob = (float*)outputBuffer;

  if (shouldClick) {
    shouldClick = false;
    ob[0] = 1.0f;
  }
  else {
    ob[0] = 0.0f;
  }
  for (unsigned i = 1; i < nBufferFrames; i++)
    ob[i] = 0.0f;

  return 0;
}

int main() {
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
  unsigned int bufferFrames = 512;
  double data[2];

  try {
    dac.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate,
                   &bufferFrames, &processAudio, (void *)&data);
    dac.startStream();


  } catch (RtError &e) {
    e.printMessage();
    exit(0);
  }

    wiringPiSetup();
    wiringPiISR(0, INT_EDGE_RISING, &callback);
    getchar();

  try {
    dac.stopStream();
  } catch (RtError &e) {
    e.printMessage();
  }
  if (dac.isStreamOpen()) dac.closeStream();
  return 0;
}
