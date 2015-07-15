#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
using namespace std;

bool shouldClick = false;
unsigned int channels = 2;

int saw(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *data) {
  //bool shouldClickLocal = shouldClick;
  bool shouldClickLocal = true;
  shouldClick = false;

  extern unsigned int channels;  // XXX important, i guess
  short *buffer = (short *)outputBuffer;

  if (status) cout << "Stream underflow detected!" << endl;

  for (unsigned i = 0; i < nBufferFrames; i++)
    for (unsigned j = 0; j < channels; j++)
      if (i == 0 && shouldClickLocal)
        *buffer++ = 32767;
      else
        *buffer++ = 0;

  return 0;
}

int main(int argc, char *argv[]) {
  unsigned int bufferFrames = 512, fs = 44100, device = 0;

  cout << "using HDMI audio output (LCD speakers / headphone jack)" << endl;
  system("amixer cset numid=3 2"); // use hdmi audio output
  system("amixer -c 0 -- sset PCM 0dB"); // set gain to nominal

  RtAudio dac;
  if (dac.getDeviceCount() < 1) {
    cout << "\nNo audio devices found!\n";
    exit(1);
  }

  dac.showWarnings(true);

  RtAudio::StreamParameters oParams;
  oParams.deviceId = device;
  oParams.nChannels = channels;
  oParams.firstChannel = 0;

  RtAudio::StreamOptions options;
  options.flags |= RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;

  try {
    dac.openStream(&oParams, NULL, RTAUDIO_SINT16, fs, &bufferFrames, &saw,
                   NULL, &options);
    dac.startStream();
  } catch (RtError &e) {
    e.printMessage();
    if (dac.isStreamOpen()) dac.closeStream();
  }

  cout << "Stream latency = " << dac.getStreamLatency() << endl;
  cout << "Buffer size = " << bufferFrames << endl;

  char input;
  unsigned impulseCount = 0;
  do {
    cin.get(input);
    shouldClick = true;
    cout << ++impulseCount << " impulses";
  } while (input == 10);

  try {
    dac.stopStream();
  } catch (RtError &e) {
    e.printMessage();
  }

  if (dac.isStreamOpen()) dac.closeStream();
}
