/*
  Title:   Click: a latency testing program
  Author:  Hunter Gordon and Karl Yerkes
  Date:    2015-07-10
  Details:
    Upon receipt of an input trigger event (i.e. keypress, network message,
  digital pin) this program responds by playing a sound (i.e. click, ding, etc)
  on the given sound device, flashing white on the given graphics device, and
  raising the given output pin momentarily. The purpose of this program is to
  facilitate data gathering for testing the latency, jitter and synchrony of
  distributed interactive audiovisual systems.

  TODO: encapsulate the the audio stuff into an "easy" class
  TODO: figure out a way to wait for any key press (not just enter)
  TODO: integrate probe program so we know more about the sound devices
  TODO: integrate WiringPi as impulse trigger
  TODO: integrate WiringPi as output response
  TODO: integrate Cuttlebone as impulse trigger
  TODO: integrate command line interface arguments
  TODO: integrate OpenGL/BCM graphics flasher
*/

#define RTAUDIO_DEBUG
#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring> // memset
using namespace std;

bool shouldClick = false;

int processAudio(void *outputBuffer, void *inputBuffer,
                 unsigned int nBufferFrames, double streamTime,
                 RtAudioStreamStatus status, void *userData) {
  bool localShouldClick = shouldClick;
  shouldClick = false;
  short *ob = (short *)outputBuffer;
  memset(ob, 0, sizeof(short) * nBufferFrames * 2);
  if (localShouldClick) ob[0] = ob[1] = 32767;
  return 0;
}

int main() {
  RtAudio dac;
  if (dac.getDeviceCount() < 1) {
    std::cout << "\nNo audio devices found!\n";
    exit(0);
  }
  RtAudio::StreamParameters parameters;
  parameters.deviceId = 1;
  //parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 2;
  parameters.firstChannel = 0;
  RtAudio::StreamOptions options;
  options.flags = RTAUDIO_MINIMIZE_LATENCY | RTAUDIO_HOG_DEVICE | RTAUDIO_SCHEDULE_REALTIME;


  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 512;  // 0 means: ask for the smallest
  double data[2];

  try {
    unsigned got = bufferFrames;
    dac.openStream(&parameters, NULL, RTAUDIO_SINT16, sampleRate, &got, &processAudio, (void *)&data, &options);
    cout << "asked for " << bufferFrames << " and got " << got << endl;

    dac.startStream();
  } catch (RtAudioError &e) {
    e.printMessage();
    exit(0);
  }

  unsigned clickCount = 0;
  char c;
  do {
    c = getchar();
    shouldClick = true;
    clickCount++;
    cout << clickCount << " impulses played";
  } while (c == 10);  // return char is given

  try {
    dac.stopStream();
  } catch (RtAudioError &e) {
    e.printMessage();
  }
  if (dac.isStreamOpen()) dac.closeStream();
  return 0;
}
