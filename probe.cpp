#define RTAUDIO_DEBUG

#include <cstdio>
#include <cassert>
#include "RtAudio.h"

//typedef struct RtAudio::DeviceInfo {
//  bool probed;                           // true if the device capabilities were successfully probed.
//  std::string name;                      // Character string device identifier.
//  unsigned int outputChannels;           // Maximum output channels supported by device.
//  unsigned int inputChannels;            // Maximum input channels supported by device.
//  unsigned int duplexChannels;           // Maximum simultaneous input/output channels supported by device.
//  bool isDefaultOutput;                  // true if this is the default output device.
//  bool isDefaultInput;                   // true if this is the default input device.
//  std::vector<unsigned int> sampleRates; // Supported sample rates.
//  RtAudioFormat nativeFormats;           // Bit mask of supported data formats.
//};

int main() {
  RtAudio audio;

  unsigned int deviceCount = audio.getDeviceCount();
  printf("found %u devices\n", deviceCount);

  printf("dev out in dup def info/name\n");
  printf("----------------------------------------------\n");
  for (unsigned deviceNumber = 0; deviceNumber < deviceCount; deviceNumber++) {
    RtAudio::DeviceInfo info = audio.getDeviceInfo(deviceNumber);

    assert(info.probed);

    printf("%3u %3u %2u %3u  %c%c %s \n",
      deviceNumber,
      info.outputChannels,
      info.inputChannels,
      info.duplexChannels,
      (info.isDefaultOutput) ? 'o' : ' ',
      (info.isDefaultInput) ? 'i' : ' ',
      info.name.c_str()
    );
  }

  printf("---(sample rates)-----------------------------\n");
  for (unsigned int deviceNumber = 0; deviceNumber < deviceCount; deviceNumber++) {
    RtAudio::DeviceInfo info = audio.getDeviceInfo(deviceNumber);

    assert(info.probed);

    printf("%3u ", deviceNumber);
    for (int k = 0; k < info.sampleRates.size(); ++k)
      printf("%u ", info.sampleRates[k]);
    printf("\n");
  }

  printf("---(native formats)---------------------------\n");
  for (unsigned int deviceNumber = 0; deviceNumber < deviceCount; deviceNumber++) {
    RtAudio::DeviceInfo info = audio.getDeviceInfo(deviceNumber);

    assert(info.probed);

    printf("%3u ", deviceNumber);
    if (RTAUDIO_SINT8 & info.nativeFormats) {
      printf("SINT8 ");
    } else if (RTAUDIO_SINT16 & info.nativeFormats) {
      printf("SINT16 ");
    } else if (RTAUDIO_SINT24 & info.nativeFormats) {
      printf("SINT24 ");
    } else if (RTAUDIO_SINT32 & info.nativeFormats) {
      printf("SINT32 ");
    } else if (RTAUDIO_FLOAT32 & info.nativeFormats) {
      printf("FLOAT32 ");
    } else if (RTAUDIO_FLOAT64 & info.nativeFormats) {
      printf("FLOAT64 ");
    }
    printf("\n");
  }
}
