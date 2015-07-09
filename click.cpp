#define RTAUDIO_DEBUG

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>

using namespace std;
bool shouldClick = false;

int processAudio(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData) {


  float* ob = (float*)outputBuffer;

  ob[0] = ((shouldClick == true)? 1 : 0 );
  shouldClick = false;
  for (unsigned i = 1; i < nBufferFrames; i++)
    ob[i] = 0;

  return 0;
}

int main() {
  unsigned clicks = 0;
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
  } catch (RtAudioError &e) {
    e.printMessage();
    exit(0);
}

  char c;
  do {
    c = getchar();
    clicks++;
	cout<<clicks<<" returns pressed"<<endl;
    shouldClick=true;
  }while(c==10) ; //return char is given


  try {
    dac.stopStream();
  } catch (RtAudioError &e) {
    e.printMessage();
  }
  if (dac.isStreamOpen()) dac.closeStream();
  return 0;
}
