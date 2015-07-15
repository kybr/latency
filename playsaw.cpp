#include "RtAudio.h"
#include <iostream>
#include <cstdlib>

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16
#define SCALE 32767.0
#include <unistd.h>
#define SLEEP(milliseconds) usleep((unsigned long)(milliseconds * 1000.0))

#define BASE_RATE 0.005
#define TIME 1.0

unsigned int channels = 2;
RtAudio::StreamOptions options;
unsigned int frameCounter = 0;
bool checkCount = false;
unsigned int nFrames = 0;
const unsigned int callbackReturnValue = 1;

int saw(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *data) {
  unsigned int i, j;
  extern unsigned int channels;
  MY_TYPE *buffer = (MY_TYPE *)outputBuffer;

  if (status) std::cout << "Stream underflow detected!" << std::endl;

  double increment;
  for (j = 0; j < channels; j++) {
    increment = BASE_RATE * (j + 1 + (j * 0.1));
    for (i = 0; i < nBufferFrames; i++) {
      if (i == 0)
        *buffer++ = 32767;
      else
        *buffer++ = 0;
    }
  }

  frameCounter += nBufferFrames;
  if (checkCount && (frameCounter >= nFrames)) return callbackReturnValue;
  return 0;
}

int main(int argc, char *argv[]) {
  RtAudio dac;
  if (dac.getDeviceCount() < 1) {
    std::cout << "\nNo audio devices found!\n";
    exit(1);
  }

  // Let RtAudio print messages to stderr.
  dac.showWarnings(true);

  unsigned fs = 44100, device = 1, offset = 0, bufferFrames = 512;
  RtAudio::StreamParameters oParams;
  oParams.deviceId = device;
  oParams.nChannels = channels;
  oParams.firstChannel = offset;

  options.flags |= RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;
  options.flags |= RTAUDIO_NONINTERLEAVED;

  try {
    dac.openStream(&oParams, NULL, FORMAT, fs, &bufferFrames, &saw, NULL,
                   &options);
    dac.startStream();
  } catch (RtError &e) {
    e.printMessage();
    goto cleanup;
  }

  if (checkCount) {
    while (dac.isStreamRunning() == true) SLEEP(100);
  } else {
    char input;
    // std::cout << "Stream latency = " << dac.getStreamLatency() << "\n" <<
    // std::endl;
    std::cout << "\nPlaying ... press <enter> to quit (buffer size = "
              << bufferFrames << ").\n";
    std::cin.get(input);

    try {
      // Stop the stream
      dac.stopStream();
    } catch (RtError &e) {
      e.printMessage();
    }
  }

cleanup:
  if (dac.isStreamOpen()) dac.closeStream();

  return 0;
}
